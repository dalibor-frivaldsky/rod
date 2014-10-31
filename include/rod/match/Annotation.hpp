#pragma once




namespace rod
{

	namespace match
	{

		namespace detail
		{

			template< template< typename > class Annotation, typename Type >
			struct AnnotationMatcher
			{
				enum { r = Annotation< Type >::r };
			};
			
		}

		template< template< typename > class A >
		struct Annotation
		{
			template< typename Type >
			using Match = detail::AnnotationMatcher< A, Type >;
		};
		
	}
	
}