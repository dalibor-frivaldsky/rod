#pragma once


#include <rod/TypeList.hpp>




namespace rod
{

	template< typename... Type >
	struct TypeRegistry;


	namespace typeRegistry
	{

		template< typename Reg, template< typename > class Selector >
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

		template< template< typename > class Selector >
		using Find = typeRegistry::Find< This, Selector >;
	};
	
}