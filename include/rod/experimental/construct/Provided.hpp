#pragma once


#include <utility>




namespace rod {
namespace experimental {
namespace construct {

	struct Provided
	{
		static Provided	ref;

		template< typename T >
		operator T& ()
		{
			return *((T*)(nullptr));
		}
	};
	
}}}