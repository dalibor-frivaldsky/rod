#include "rod/Contextual.hpp"


#include <memory>
#include <type_traits>
#include <utility>

#include "Common.hpp"




struct Object
{
	std::unique_ptr< int >	ptr;

	Object()
	{}

	Object( Object&& other ):
	  ptr( std::move( other.ptr ) )
	{}
};

struct Type
{};


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
							rod::TypeRegistry< Trackable, Type >
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
		rod::create< Test, Type >( this, Trackable() ).method();
	}
};


void
test()
{
	enter< TestEntry >();
	assertTrackable( 1, 0, 1, 0, 0, 2 );
}