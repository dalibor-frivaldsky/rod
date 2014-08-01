#include "rod/Context.hpp"


#include <cassert>

#include "rod/Annotation.hpp"
#include "rod/AsContextual.hpp"
#include "rod/Contextual.hpp"
#include "rod/Dispatcher.hpp"
#include "rod/Rod.hpp"




int	value = 0;


namespace annotation
{

	struct Annotation
	{};

	ROD_Annotation( Annotation )
	
}


template< typename Context >
struct Contextual1:
	private rod::Contextual< Context >
{
	using Annotation = annotation::Annotation;

	enum { id = 1 };

	ROD_Contextual_Constructor( Contextual1 );

	void
	method( int multiplier )
	{
		value = 10 * multiplier;
	}
};


template< typename Context >
struct Contextual2:
	private rod::Contextual< Context >
{
	using Annotation = annotation::Annotation;

	enum { id = 2 };

	ROD_Contextual_Constructor( Contextual2 );

	void
	method( int multiplier )
	{
		value = 20 * multiplier;
	}
};


struct Handle
{
	template< typename Branch >
	static
	int
	handle()
	{
		return Branch::id;
	}
};


struct Performer
{
	template< template< typename > class Branch, typename Contextual >
	static
	void
	perform( Contextual* contextual, int multiplier )
	{
		rod::create< Branch >( contextual ).method( multiplier );
	}
};


template< typename Context >
class Domain:
  public rod::Contextual<
  				Context,
  				rod::AsContextual< Contextual2 >,
  				rod::AsContextual< Contextual1 > >
{
private:
	using This = Domain< Context >;


public:
	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		auto	dispatcher = rod::dispatcher<
									annotation::IsAnnotation,
									Handle,
									Performer >( this );

		dispatcher.dispatch( 1, 7 );
		assert( value == 70 );
		
		dispatcher.dispatch( 2, 3 );
		assert( value == 60 );
	}
};


void
test()
{
	rod::enterPlain< Domain >();
}