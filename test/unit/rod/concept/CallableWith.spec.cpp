#include <catch.hpp>
#include <rod/concept/CallableWith>

using namespace rod;


struct Foo {};

SCENARIO( "CallableWith concept models a function type accepting a set of arguments", "[unit]" ) {

	GIVEN( "zero-parameter callable object" ) {
		auto zeroPLamba = [] {};

		THEN( "it models CallableWith with zero arguments" ) {
			static_assert( CallableWith< decltype(zeroPLamba) >() );
		}
	}

	GIVEN( "multi-parameter callable object" ) {
		auto multiPLamba = [] (int a, float b) {};

		THEN( "it models CallableWith with expected arguments" ) {
			static_assert( CallableWith< decltype(multiPLamba), int, float >() );
		}
	}

	GIVEN( "a callable object" ) {
		auto aLamba = [] (Foo f) {};

		THEN( "CallableWith model is refused if argument types dont match" ) {
			static_assert( CallableWith< decltype(aLamba), int, float >() == false );
		}
	}

	GIVEN( "a non-callable object" ) {
		Foo f{};

		THEN( "CallableWith model is refused" ) {
			static_assert( CallableWith< decltype(f) >() == false );
		}
	}

}
