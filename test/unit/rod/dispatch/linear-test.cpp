#include "rod/Context.hpp"


#include <cassert>

#include "rod/Annotation.hpp"
#include "rod/AsContextual.hpp"
#include "rod/Contextual.hpp"
#include "rod/Rod.hpp"
#include "rod/dispatch/Linear.hpp"




int	value = 0;


template< typename Context >
struct Contextual1:
	private rod::Contextual< Context >
{
	enum { id = 1 };

	ROD_Contextual_Constructor( Contextual1 );

	void
	method()
	{
		value = 10;
	}
};


template< typename Context >
struct Contextual2:
	private rod::Contextual< Context >
{
	enum { id = 2 };

	ROD_Contextual_Constructor( Contextual2 );

	void
	method()
	{
		value = 20;
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
	perform( Contextual* contextual )
	{
		rod::create< Branch >( contextual ).method();
	}
};


template< typename Context >
class Domain:
  public rod::Contextual<
  				Context >
{
private:
	using This = Domain< Context >;


public:
	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		using linear = rod::dispatch::Linear<
							This,
							Handle,
							Performer,
							rod::TypeList<
								rod::AsContextual< Contextual1 >,
								rod::AsContextual< Contextual2 > > >;
		linear	l( this );
		
		l.dispatch( 1 );
		assert( value == 10 );

		l.dispatch( 2 );
		assert( value == 20 );
	}
};


void
test()
{
	rod::enterPlain< Domain >();
}