#pragma once


#include <rod/common/CanConvert.hpp>
#include <rod/common/SameType.hpp>




namespace rod
{

	namespace common
	{

		template< typename SuperClass, typename SubClass >
		struct SuperSubClass
		{
			enum { r = CanConvert< SubClass*, SuperClass* >::r };
		};

		template< typename SuperClass, typename SubClass >
		struct StrictSuperSubClass
		{
			enum { r = CanConvert< SubClass*, SuperClass* >::r &&
					   !SameType< SuperClass*, void* >::r };
		};
		
	}

}