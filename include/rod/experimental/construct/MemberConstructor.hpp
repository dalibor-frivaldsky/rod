#pragma once


#include <utility>




namespace rod {
namespace experimental {
namespace construct {

	template< typename Member >
	struct MemberConstructor
	{
		Member	value;

		MemberConstructor( Member&& value ):
		  value( std::forward< Member >( value ) )
		{}
	};
	
}}}