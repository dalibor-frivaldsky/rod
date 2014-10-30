#pragma once


#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <rod/Reduce.hpp>
#include <rod/Resolve.hpp>
#include <rod/TypeList.hpp>
#include <rod/common/NullType.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/common/Select.hpp>
#include <rod/common/SuperSubClass.hpp>
#include <rod/holder/InjectedReference.hpp>
#include <rod/holder/InjectedValue.hpp>




namespace rod
{

	template< typename... ComponentHolders >
	struct Container;


	namespace detail
	{

		template< typename Type_, typename Holder_ >
		struct ComponentHolder
		{
			using Type = Type_;
			using Holder = Holder_;
		};


		template< typename ComponentHolder >
		struct ExtractType
		{
			using r = typename ComponentHolder::Type;
		};


		template< typename ComponentHolder >
		struct ExtractHolder
		{
			using r = typename ComponentHolder::Holder;
		};


		template< typename Holder, typename Dependencies >
		struct HolderConstructor;

		template< typename Holder >
		struct HolderConstructor< Holder, TypeList<> >:
			public Holder
		{
			template< typename Context, typename ToInjectTuple >
			HolderConstructor( Context&, ToInjectTuple& ):
			  Holder()
			{}
		};

		template< typename Holder, typename Type >
		struct HolderConstructor<
			Holder,
			TypeList< holder::InjectedReference< Type > > >:
			public Holder
		{
			template< typename Context, typename... ToInject >
			HolderConstructor( Context&, std::tuple< ToInject... >& toInjectTuple ):
			  Holder(
			  	std::get<
		  			TypeList< ToInject... >::template IndexOf< Type& >::r >( toInjectTuple ) )
			{}
		};

		template< typename Holder, typename Type >
		struct HolderConstructor<
			Holder,
			TypeList< holder::InjectedValue< Type > > >:
			public Holder
		{
			template< typename Context, typename... ToInject >
			HolderConstructor( Context&, std::tuple< ToInject... >& toInjectTuple ):
			  Holder(
			  	std::move(
			  		std::get<
		  				TypeList< ToInject... >::template IndexOf< Type&& >::r >( toInjectTuple ) ) )
			{}
		};

		template< typename Holder, typename... Dependency >
		struct HolderConstructor< Holder, TypeList< Dependency... > >:
			public Holder
		{
			template< typename Context, typename ToInjectTuple >
			HolderConstructor( Context& context, ToInjectTuple& ):
			  Holder( resolve< Dependency >( context )... )
			{}
		};


		template< typename ComponentHolder >
		struct DefineHolderConstructor
		{
		private:
			using Holder = typename ExtractHolder< ComponentHolder >::r;

		public:
			using r = HolderConstructor<
						Holder,
						typename Holder::Dependencies >;
		};


		template< typename... ComponentDefinition >
		struct GenerateContainer
		{
		private:
			template< typename ComponentDef >
			struct GenerateComponentHolder
			{
				using r = ComponentHolder< typename ComponentDef::Type, typename ComponentDef::Holder >;
			};

		public:
			using r = Container< typename GenerateComponentHolder< ComponentDefinition >::r... >;
		};


		template< typename Cont, typename Type >
		struct AccessComponentHolder;

		template< typename Type >
		struct AccessComponentHolder< Container<>, Type >
		{
			using r = void;
		};

		template< typename Type, typename Holder, typename... Rest >
		struct AccessComponentHolder<
					Container<
						ComponentHolder< Type, Holder >,
						Rest... >,
					Type >
		{
			using r = Holder;
		};

		template< typename CompHolder, typename... Rest, typename Type >
		struct AccessComponentHolder<
					Container<
						CompHolder,
						Rest... >,
					Type >
		{
			static_assert( sizeof...( Rest ), "Component holder not found" );

			using r = typename AccessComponentHolder< Container< Rest... >, Type >::r;
		};


		template< typename Container, typename ContainerToMerge >
		struct Merge;

		template< typename... ComponentHolder, typename... ToMergeComponentHolder >
		struct Merge<
				Container< ComponentHolder... >,
				Container< ToMergeComponentHolder... > >
		{
			using r = Container< ComponentHolder..., ToMergeComponentHolder... >;
		};


		template< typename Cont >
		struct ContainedTypes;

		template< typename... ComponentHolder >
		struct ContainedTypes< Container< ComponentHolder... > >
		{
			using r = TypeList< typename ExtractType< ComponentHolder >::r... >;
		};


		template< typename Cont, template< typename > class Selector >
		struct Select;

		template< typename... ComponentHolder, template< typename > class Selector >
		struct Select< Container< ComponentHolder... >, Selector >
		{
		private:
			template< typename Next, typename Result >
			struct TestThenAppend
			{
				using r =
					typename common::Select<
								Selector< typename Next::Holder >::r,
								typename Result::template Append< typename Next::Type >::r,
								Result >::r;
			};

		public:
			using r = typename Reduce< TestThenAppend, TypeList<>, ComponentHolder... >::r;
		};


		template< typename Container, typename Interface >
		struct FindImplementors;

		template< typename... ComponentHolder, typename Interface >
		struct FindImplementors< Container< ComponentHolder... >, Interface >
		{
		private:
			template< typename Next, typename Result >
			struct TestThenAppend
			{
				using r =
					typename common::Select<
								common::SuperSubClass< Interface, typename Next::Type >::r,
								typename Result::template Append< typename Next::Type >::r,
								Result >::r;
			};

		public:
			using r = typename Reduce< TestThenAppend, TypeList<>, ComponentHolder... >::r;
		};


		template< typename Container, typename Interfaces >
		struct FindImplementorsForAll;

		template< typename Container, typename... Interface >
		struct FindImplementorsForAll< Container, TypeList< Interface... > >
		{
		private:
			template< typename Next, typename Result >
			struct AppendImplementors
			{
				using r = typename Result::template Append<
								typename FindImplementors< Container, Next >::r >::r;
			};

		public:
			using r = typename Reduce< AppendImplementors, TypeList<>, Interface... >::r;
		};

	}


	template< typename... ComponentDefinition >
	struct CreateContainer
	{
		using r = typename detail::GenerateContainer< ComponentDefinition... >::r;
	};


	template< typename... ComponentHolder >
	struct Container:
		public detail::DefineHolderConstructor< ComponentHolder >::r...
	{
	private:

		using This = Container< ComponentHolder... >;


	public:

		// MSVC2013 is unable to unpack ComponentHolder into the ExtractDeps
		// when the ComponentHolder parameter pack is of size zero
		// workaround is to create two different constructors
		// and choose one based on the ComponentHolder pack size
		template<
			typename Context,
			typename ToInjectTuple,
			typename Holders = TypeList< ComponentHolder... > >
		Container(
			Context& context,
			ToInjectTuple&& toInjectTuple,
			typename std::enable_if< (Holders::Length::r > 0), void >::type* hasHolders = 0 ):
		  detail::DefineHolderConstructor< ComponentHolder >::r( context, toInjectTuple )...
		{}

		template<
			typename Context,
			typename ToInjectTuple,
			typename Holders = TypeList< ComponentHolder... > >
		Container(
			Context&,
			ToInjectTuple&&,
			typename std::enable_if< (Holders::Length::r == 0), void >::type* hasHolders = 0 )
		{}

		Container( const This& ) = delete;
		Container( This&& ) = delete;

		This& operator = ( const This& ) = delete;
		This& operator = ( This&& ) = delete;


		template< typename Type >
		using Access = detail::AccessComponentHolder< This, Type >;

		template< typename ContainerToMerge >
		using Merge = detail::Merge< This, ContainerToMerge >;
		
		using ContainedTypes = detail::ContainedTypes< This >;
		
		template< template< typename > class Selector >
		using Select = detail::Select< This, Selector >;

		template< typename Component >
		using Contains = typename ContainedTypes::r::template Contains< Component >;

		template< typename Interface >
		using FindImplementors = detail::FindImplementors< This, Interface >;

		template< typename Interfaces >
		using FindImplementorsForAll = detail::FindImplementorsForAll< This, Interfaces >;


		template< typename Type >
		typename Access< Type >::r&
		access()
		{
			return *this;
		}

		template< typename Type >
		const typename Access< Type >::r&
		access() const
		{
			return *this;
		}

	};

}