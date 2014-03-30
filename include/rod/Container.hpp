#pragma once


#include <rod/Reduce.hpp>
#include <rod/TypeList.hpp>
#include <rod/common/NullType.hpp>
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


		template< typename ComponentDefinitions >
		struct GenerateContainer;

		template< typename... ComponentDefinition >
		struct GenerateContainer< TypeList< ComponentDefinition... > >
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


	template< typename ComponentDefinitions >
	struct CreateContainer
	{
		using r = typename container::GenerateContainer< ComponentDefinitions >::r;
	};


	template< typename... ComponentHolder >
	struct Container:
		public container::ExtractHolder< ComponentHolder >::r...
	{
	private:

		using This = Container< ComponentHolder... >;


	public:

		Container():
			ComponentHolder::Holder()...
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