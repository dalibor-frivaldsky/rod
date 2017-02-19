// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <catch.hpp>
#include <rod/pipe>

using namespace rod::operators;


SCENARIO( "Piping arguments to unary function objects", "[unit]" ) {

	GIVEN( "an argument and unary function object accepting it as parameter" ) {
		int	arg = 10;
		auto inc = [] (int i) { return i+1; };

		WHEN( "the argument is piped to the function object" ) {
			int result = arg | inc;

			THEN( "the same result is obtained as if calling the function object directly with the argument" ) {
				REQUIRE( result == inc(arg) );
			}
		}
	}

}