#pragma once


#include <rod/Annotation.hpp>




namespace rod
{

	namespace annotation
	{

		template< template< typename > class Selector_ >
		struct Resolver
		{
			template< typename T >
			using Selector = Selector_< T >;
		};

		ROD_Annotation( Resolver )
		
	}
	
}