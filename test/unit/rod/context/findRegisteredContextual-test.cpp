#include "rod/Context.hpp"


#include <type_traits>

#include "rod/Annotation.hpp"
#include "rod/AsContextual.hpp"
#include "rod/Contextual.hpp"




namespace annotation
{

	struct Annotation
	{};

	ROD_Annotation( Annotation )
	
}


template< typename Context >
struct AnnotatedContextual:
	private rod::Contextual< Context >
{
	using Annotation = annotation::Annotation;
};


template< typename Context >
struct UnannotatedContextual:
	private rod::Contextual< Context >
{};


template< typename Context >
struct AnnotatedContextual2:
	private rod::Contextual< Context >
{
	using Annotation = annotation::Annotation;
};



void
test()
{
	using ctx1 = rod::CreateInitialContext< rod::AsContextual< AnnotatedContextual > >::r;
	using ctx2 = ctx1::CreateChildContext<
							rod::AsContextual< UnannotatedContextual >,
							rod::AsContextual< AnnotatedContextual2 > >::r;

	static_assert( std::is_same<
						ctx2::FindRegisteredContextual< annotation::IsAnnotation >::r,
						rod::TypeList<
								rod::AsContextual< AnnotatedContextual >,
								rod::AsContextual< AnnotatedContextual2 > >
				   >::value,
				   "Unexpected FindAnnotatedContextual result" );
}