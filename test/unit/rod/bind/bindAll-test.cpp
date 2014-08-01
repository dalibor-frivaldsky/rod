#include <type_traits>

#include "rod/AsSingleton.hpp"
#include "rod/Bind.hpp"
#include "rod/Contextual.hpp"
#include "rod/Rod.hpp"




struct Object
{};

struct Type
{};

struct Component
{};



template< typename Context >
class ToBind:
	public rod::Contextual<
			Context,
			rod::AsSingleton< Component > >
{
public:
	ROD_Contextual_Constructor( ToBind )
};


template< typename Context >
class Test:
	public rod::Contextual< Context >
{
private:
	using This = Test< Context >;


public:

	ROD_Contextual_Constructor( Test )

	void
	enter()
	{
		using bound = typename rod::Bind< This, ToBind >
									::template Add< Type >::r
									::template Inject< Object& >::r
									::r;

		Object	o;
		bound b = rod::create< ToBind, Type >( this, o );
	}
};


void
test()
{
	rod::enterPlain< Test >();
}