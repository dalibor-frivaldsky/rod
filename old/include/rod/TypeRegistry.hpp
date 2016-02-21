#pragma once


#include <rod/TypeList.hpp>




namespace rod
{

	template< typename... Type >
	struct TypeRegistry;


	namespace typeRegistry
	{
		template< typename TypeReg, typename OtherReg >
		struct Merge;

		template< typename... Type, typename... OtherType >
		struct Merge< TypeRegistry< Type... >, TypeRegistry< OtherType... > >
		{
			using r = TypeRegistry< Type..., OtherType... >;
		};


		template< typename TypeReg, template< typename > class Selector >
		struct Find;

		template< typename... Type, template< typename > class Selector >
		struct Find< TypeRegistry< Type... >, Selector >
		{
			using r = typename TypeList< Type... >::template Select< Selector >::r;
		};
		
	}


	template< typename... Type >
	struct CreateTypeRegistry
	{
		using r = TypeRegistry< Type... >;
	};


	template< typename... Type >
	struct TypeRegistry
	{
	private:

		using This = TypeRegistry< Type... >;


	public:

		template< typename Other >
		using Merge = typeRegistry::Merge< This, Other >;

		template< template< typename > class Selector >
		using Find = typeRegistry::Find< This, Selector >;
	};
	
}