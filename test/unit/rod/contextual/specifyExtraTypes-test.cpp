#include "rod/Contextual.hpp"

#include <type_traits>

#include "Common.hpp"




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
							rod::TypeRegistry< Type >
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
		rod::create< Test, Type >( this ).method();
	}
};


void
test()
{
	enter< TestEntry >();
}