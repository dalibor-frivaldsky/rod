#pragma once


#include <string>
#include <memory>

#if defined( __GNUC__ )
#include <cxxabi.h>
#elif defined( _MSC_VER )
#endif




namespace rod
{

	namespace common
	{

#if defined( __GNUC__ )
		template< typename Type >
		std::string
		typeName()
		{
			int						status;
    		std::unique_ptr< char >	demangled =
    				std::unique_ptr< char >(
    						abi::__cxa_demangle( typeid( Type ).name(), 0, 0, &status ) );

    		return std::string( demangled.get() );
		}
#elif defined( _MSC_VER )
		template< typename Type >
		std::string
		typeName()
		{
			std::string		typeName = typeid( Type ).name();
			return typeName.substr( 6 );
		}
#endif
		
	}
	
}