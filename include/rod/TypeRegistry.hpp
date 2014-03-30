#pragma once




namespace rod
{

	template< typename... Type >
	struct TypeRegistry;


	template< typename... Type >
	struct CreateTypeRegistry
	{
		using r = TypeRegistry< Type... >;
	};


	template< typename... Type >
	struct TypeRegistry
	{};
	
}