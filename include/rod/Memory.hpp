#pragma once



#include <memory>



namespace rod
{

	template< typename Type >
	auto
	asShared( Type* ptr )
		-> decltype( std::shared_ptr< Type >( ptr ) )
	{
		return std::shared_ptr< Type >( ptr );
	}

	template< typename Type >
	auto
	asUnique( Type* ptr )
		-> decltype( std::unique_ptr< Type >( ptr ) )
	{
		return std::unique_ptr< Type >( ptr );
	}
	
}