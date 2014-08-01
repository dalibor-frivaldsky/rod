#include "rod/Rod.hpp"


#include <cassert>
#include <utility>

#include "rod/AsSingleton.hpp"
#include "rod/Bind.hpp"
#include "rod/Contextual.hpp"




struct Component
{
	Component()
	{}

	Component( Component&& )
	{}

	Component( const Component& ) = delete;
};


template< typename Context >
class Movable:
  public rod::Contextual< Context, rod::AsSingleton< Component > >
{
private:
	using This = Movable< Context >;


public:
	ROD_Contextual_Constructor( Movable );

	Movable( Movable&& other ):
	  This::ContextualBase( std::move( other ) )
	{}
};


template< typename Context >
class Domain:
  public rod::Contextual< Context >
{
private:
	using This = Domain< Context >;


public:

	ROD_Contextual_Constructor( Domain )

	void
	enter()
	{
		using movableType = typename rod::Bind< This, Movable >::r;

		auto		m = rod::create< Movable >( this );
		movableType	m2 = std::move( m );
	}
};


void
test()
{
	rod::enterPlain< Domain >();
}