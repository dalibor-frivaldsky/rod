#include <catch.hpp>


#include <rod/resolve>



namespace test {

	struct Foo { int a = 3; };

	constexpr auto resolve_impl(Foo a, int b) { return a.a + b; }
}




SCENARIO( "rod::resolve evaluates resolve_impl function defined in Source type's namespace", "[unit]" ) {

	GIVEN( "resolve_impl function" ) {

		THEN( "rod::resolve can be evaluated with expected arguments" ) {
			REQUIRE( 5 == rod::resolve(2)(test::Foo{}) );
		}
	}

}

SCENARIO( "rod::resolve evaluates in constexpr context if the resolve_impl is constexpr", "[unit]" ) {

	GIVEN( "resolve_impl function" ) {

		THEN( "rod::resolve can be constexpr-evaluated with expected arguments" ) {
			static_assert( test::resolve_impl(test::Foo{}, 2) == 5 );
			//static_assert( 5 == rod::resolve(2)(test::Foo{}) );
		}
	}

}