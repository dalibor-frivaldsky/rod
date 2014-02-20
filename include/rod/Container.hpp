#pragma once


#include <rod/TypeList.hpp>
#include <rod/common/NullType.hpp>
#include <rod/common/Select.hpp>
#include <rod/common/SuperSubClass.hpp>




namespace rod
{

	namespace container
	{

		template< typename ComponentType, typename ObjectContainer, typename InnerContainer >
		struct ContainerDef:
			public ObjectContainer,
			public InnerContainer
		{
			using Type = ComponentType;
			using Holder = ObjectContainer;
			using Inner = InnerContainer;
		};


		template< typename ComponentDefinitions >
		struct GenerateContainer;

		template<>
		struct GenerateContainer< TypeList<> >
		{
			using r = common::NullType;
		};

		template< typename ComponentDefinitions >
		struct GenerateContainer
		{
		private:
			using innerContainer =
					typename GenerateContainer<
							typename ComponentDefinitions::Tail::r >::r;
			using componentDefinition = typename ComponentDefinitions::Head::r;

		public:
			using r = ContainerDef<
							typename componentDefinition::Type,
							typename componentDefinition::Holder,
							innerContainer >;
		};


		template< typename Container, typename ContainerToMerge >
		struct Merge;

		template< typename ContainerToMerge >
		struct Merge< common::NullType, ContainerToMerge >
		{
			using r = ContainerToMerge;
		};

		template< typename Container, typename ContainerToMerge >
		struct Merge
		{
		private:
			using innerContainer = typename Merge< typename Container::Inner, ContainerToMerge >::r;

		public:
			using r = ContainerDef< typename Container::Type, typename Container::Holder, innerContainer >;
		};


		template< typename Container >
		struct Transcribe;

		template<>
		struct Transcribe< common::NullType >
		{
			using r = common::NullType;
		};

		template< typename Container >
		struct Transcribe
		{
		private:
			using innerTranscript = typename Transcribe< typename Container::Inner >::r;

		public:
			using r = ContainerDef< typename Container::Type, typename Container::Holder::TransferContainer, innerTranscript >;
		};


		template< typename ContainerDef >
		struct ContainedTypes;

		template<>
		struct ContainedTypes< common::NullType >
		{
			using r = TypeList<>;
		};

		template< typename ComponentType, typename ComponentHolder, typename InnerContainer >
		struct ContainedTypes< ContainerDef< ComponentType, ComponentHolder, InnerContainer > >
		{
		private:
			using innerContainedTypes = typename ContainedTypes< InnerContainer >::r;

		public:
			using r = typename innerContainedTypes::template Prepend< ComponentType >::r;
		};


		template< typename ContainerDef, typename Interface >
		struct FindImplementors;

		template< typename Interface >
		struct FindImplementors< common::NullType, Interface >
		{
			using r = TypeList<>;
		};

		template< typename ComponentType, typename ComponentHolder, typename InnerContainer, typename Interface >
		struct FindImplementors< ContainerDef< ComponentType, ComponentHolder, InnerContainer >, Interface >
		{
		private:
			enum { currentImplements = common::SuperSubClass< Interface, ComponentType >::r };
			using innerImplementors = typename FindImplementors< InnerContainer, Interface >::r;

		public:
			using r = typename common::Select< currentImplements,
											   typename innerImplementors::template Prepend< ComponentType >::r,
											   innerImplementors >::r;
		};


		template< typename ContainerDef, typename InterfaceTypeList >
		struct FindImplementorsForAll;

		template< typename ContainerDef >
		struct FindImplementorsForAll< ContainerDef, TypeList<> >
		{
			using r = TypeList<>;
		};

		template< typename ContainerDef, typename InterfaceTypeList >
		struct FindImplementorsForAll
		{
		private:
			using headImplementors = typename FindImplementors< ContainerDef, typename InterfaceTypeList::Head::r >::r;
			using restImplementors = typename FindImplementorsForAll< ContainerDef, typename InterfaceTypeList::template Split< 1 >::r::Second >::r;

		public:
			using r = typename restImplementors::template Prepend< headImplementors >::r;
		};


		template< typename ContainerDef, typename Type >
		struct AccessContainerNode;

		template< typename InnerContainer, typename ComponentType, typename ComponentHolder >
		struct AccessContainerNode< ContainerDef< ComponentType, ComponentHolder, InnerContainer >, ComponentType >
		{
			using r = ContainerDef< ComponentType, ComponentHolder, InnerContainer >;
		};

		template< typename ContainerDef, typename Type >
		struct AccessContainerNode
		{
			using r = typename AccessContainerNode< typename ContainerDef::Inner, Type >::r;
		};


		template< typename ContainerDef, typename Type >
		struct AccessObjectContainer
		{
			using r = typename AccessContainerNode< ContainerDef, Type >::r::Holder;
		};


		template< typename ContainerDef, template< typename > class Selector >
		struct Select;

		template< template< typename > class Selector >
		struct Select< common::NullType, Selector >
		{
			using r = TypeList<>;
		};

		template< typename ContainerDef, template< typename > class Selector >
		struct Select
		{
		private:
			using innerSelected = typename Select< typename ContainerDef::Inner, Selector >::r;
			using currentSelected = 
					typename common::Select<
							Selector< typename ContainerDef::Holder >::r,
							typename innerSelected::template Prepend< typename ContainerDef::Type >::r,
							innerSelected >::r;

		public:
			using r = currentSelected;
		};
		
	}


	template< typename ContainerDef >
	struct Container;


	template< typename ComponentDefinitions >
	struct CreateContainer
	{
		using r = Container< typename container::GenerateContainer< ComponentDefinitions >::r >;
	};


	template< typename Def >
	struct Container:
		public Def
	{

	public:

		using ContainerDef = Def;


		template< typename Type >
		using Access = container::AccessObjectContainer< ContainerDef, Type >;

		template< typename ContainerToMerge >
		struct Merge
		{
			using r = Container< typename container::Merge< ContainerDef, typename ContainerToMerge::ContainerDef >::r >;
		};

		struct Transcribe
		{
			using r = Container< typename container::Transcribe< ContainerDef >::r >;
		};
		
		using ContainedTypes = container::ContainedTypes< ContainerDef >;

		template< template< typename > class Selector >
		using Select = container::Select< ContainerDef, Selector >;

		template< typename Component >
		using Contains = typename ContainedTypes::r::template Contains< Component >;

		template< typename Interface >
		using FindImplementors = container::FindImplementors< ContainerDef, Interface >;

		template< typename InterfaceTypeList >
		using FindImplementorsForAll = container::FindImplementorsForAll< ContainerDef, InterfaceTypeList >;


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