#pragma once


#include <functional>
#include <tuple>
#include <type_traits>

#include <rod/Reduce.hpp>
#include <rod/TypeList.hpp>
#include <rod/common/NullType.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/common/Select.hpp>
#include <rod/common/SuperSubClass.hpp>




namespace rod
{

	template< typename... ComponentHolders >
	struct Container;


	namespace container
	{

		template< typename Type_, typename Holder_ >
		struct ComponentHolder
		{
			using Type = Type_;
			using Holder = Holder_;
		};


		template< typename... Holder >
		struct HolderBase;

		template<>
		struct HolderBase<>
		{
			inline
			HolderBase()
			{}

			inline
			~HolderBase()
			{}
		};

		template< typename Holder >
		struct HolderConstructor:
			public Holder
		{
			template< typename... Arg, int... Seq >
			HolderConstructor( std::tuple< Arg... >& argTuple,
							   common::Sequence< Seq... >&& ):
			  Holder( std::get< Seq >( argTuple )... )
			{}
		};

		template< typename... Holder >
		struct HolderBase:
		  public HolderConstructor< Holder >...
		{
			template< typename... ArgTuple >
			HolderBase( ArgTuple&&... argTuple ):
			  HolderConstructor< Holder >( argTuple, typename common::GenerateSequence< std::tuple_size< ArgTuple >::value >::r() )...
			{}
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


		template< typename Container >
		struct GetDependencies;

		template< typename... ComponentHolder >
		struct GetDependencies< Container< ComponentHolder... > >
		{
		private:
			template< typename Next, typename Result >
			struct MergeDependencies
			{
				using r = typename Result::template AppendAll< Next >::r;
			};

			using merged = typename Reduce<
										MergeDependencies,
										TypeList<>,
										typename ExtractHolder< ComponentHolder >::r::Dependencies... >::r;

		public:
			using r = typename merged::RemoveDuplicates::r;
		};


		template< typename ComponentHolder >
		struct ExtractDeps
		{
		private:
			using deps = typename ComponentHolder::Holder::Dependencies;
			

			template< typename Dependencies, typename Args >
			struct GetSequence;

			template< typename... Dep, typename Args >
			struct GetSequence< TypeList< Dep... >, Args >
			{
			private:
				template< typename Next, typename Result >
				struct AppendArgIndex
				{
					using r = typename Result::template Append<
								Args::template IndexOf< Next >::r >::r;
				};

			public:
				using r = typename Reduce<
									AppendArgIndex,
									common::Sequence<>,
									Dep... >::r;
			};


			template< typename ArgTuple, int... Seq >
			static
			auto
			createTuple( ArgTuple& argTuple, common::Sequence< Seq... >&& )
				-> std::tuple< decltype( std::get< Seq >( argTuple ) )... >
			{
				using depsTuple = std::tuple< decltype( std::get< Seq >( argTuple ) )... >;

				return depsTuple( std::get< Seq >( argTuple )... );
			}


			template< typename ToDecay >
			struct Decay
			{
				using r = typename std::decay< ToDecay >::type;
			};


		public:
			template< typename... Arg, int... Seq >
			static
			auto
			extract( std::tuple< Arg... >& argTuple, common::Sequence< Seq... >&& )
				-> decltype( createTuple(
								argTuple,
								typename GetSequence<
											deps,
											typename TypeList< decltype( std::get< Seq >( argTuple )() )... >::template Apply< Decay >::r >::r() ) )
			{
				using argTypesUndecayed = TypeList< decltype( std::get< Seq >( argTuple )() )... >;
				using argTypes = typename argTypesUndecayed::template Apply< Decay >::r;
				using seq = typename GetSequence< deps, argTypes >::r;

				return createTuple( argTuple, seq() );
			}
		};

	}


	template< typename... ComponentDefinition >
	struct CreateContainer
	{
		using r = typename container::GenerateContainer< ComponentDefinition... >::r;
	};


	template< typename... ComponentHolder >
	struct Container:
		public container::HolderBase< typename container::ExtractHolder< ComponentHolder >::r... >
	{
	private:

		using This = Container< ComponentHolder... >;
		using HolderBase = container::HolderBase< typename container::ExtractHolder< ComponentHolder >::r... >;


	public:

		// MSVC2013 is unable to unpack ComponentHolder into the ExtractDeps
		// when the ComponentHolder parameter pack is of size zero
		// workaround is to create two different constructors
		// and choose one based on the ComponentHolder pack size
		template< typename ArgTuple, typename Holders = TypeList< ComponentHolder... > >
		Container( ArgTuple&& argTuple, typename std::enable_if< (Holders::Length::r > 0), void >::type* hasHolders = 0 ):
			HolderBase( container::ExtractDeps< ComponentHolder >::extract(
							argTuple,
							typename common::GenerateSequence< std::tuple_size< ArgTuple >::value >::r() )... )
		{}

		template< typename ArgTuple, typename Holders = TypeList< ComponentHolder... > >
		Container( ArgTuple&&, typename std::enable_if< (Holders::Length::r == 0), void >::type* hasHolders = 0 ):
			HolderBase()
		{}

		template< typename Type >
		using Access = container::AccessComponentHolder< This, Type >;

		template< typename ContainerToMerge >
		using Merge = container::Merge< This, ContainerToMerge >;
		
		using ContainedTypes = container::ContainedTypes< This >;
		
		template< template< typename > class Selector >
		using Select = container::Select< This, Selector >;

		template< typename Component >
		using Contains = typename ContainedTypes::r::template Contains< Component >;

		template< typename Interface >
		using FindImplementors = container::FindImplementors< This, Interface >;

		template< typename Interfaces >
		using FindImplementorsForAll = container::FindImplementorsForAll< This, Interfaces >;

		using GetDependencies = container::GetDependencies< This >;


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