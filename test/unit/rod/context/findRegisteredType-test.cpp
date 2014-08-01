#include "rod/Context.hpp"


#include <type_traits>

#include "rod/Annotation.hpp"




namespace annotation
{

	struct Annotation
	{};

	ROD_Annotation( Annotation )
	
}


struct AnnotatedType
{
	using Annotation = annotation::Annotation;
};


struct UnannotatedType
{};



void
test()
{
	using ctx1 = rod::CreateInitialContext< AnnotatedType >::r;
	using ctx2 = ctx1::CreateChildContext< UnannotatedType >::r;

	static_assert( std::is_same<
						ctx2::FindRegisteredType< annotation::IsAnnotation >::r,
						rod::TypeList< AnnotatedType >
				   >::value,
				   "Unexpected FindAnnotated result" );
}