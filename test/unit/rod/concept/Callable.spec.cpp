// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <catch.hpp>
#include <rod/concept/Callable>

using namespace rod;


int fn(float) { return 0; }

SCENARIO( "Callable concept models a function type", "[unit]" ) {

	GIVEN( "lambda" ) {
		auto lambda = [](int i) {};
		
		THEN( "it models Callable" ) {
			static_assert( Callable< decltype(lambda) >() );
		}
	}

	GIVEN( "function pointer" ) {
		auto fnPtr = &fn;
		
		THEN( "it models Callable" ) {
			static_assert( Callable< decltype(fnPtr) >() );
		}
	}

	GIVEN( "value object" ) {
		int value = 10;

		THEN( "it doesn't model Callable" ) {
			static_assert( !Callable< decltype(value) >() );
		}
	}

}
