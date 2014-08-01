#include "rod/Contextual.hpp"


#include <cassert>
#include <type_traits>

#include "Common.hpp"




template< typename Context >
class Test:
	public rod::Contextual< Context >
{
private:

	using This = Test< Context >;
	

public:

	ROD_Contextual_Constructor( Test )

	void
	method()
	{
		static_assert( std::is_same<
							typename This::Ctx::GetTypeRegistry::r,
							rod::TypeRegistry< Trackable >
					   >::value,
					   "Unexpected TypeRegistry" );
	}
};


template< typename Context >
class TestEntry:
	public rod::Contextual< Context >
{
public:

	ROD_Contextual_Constructor( TestEntry )

	void
	enter()
	{
		Trackable o;

		rod::create< Test >( this, o ).method();
	}
};


void
test()
{
	enter< TestEntry >();
	assertTrackable( 1, 0, 0, 0, 0, 1 );
}