// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <boost/hana/assert.hpp>
#include <catch.hpp>
#include <rod/feature/type/base_of>
#include <type_traits>

using namespace boost::hana;
using namespace rod::feature::query;


struct Foo {};

struct Bar: Foo {};

struct Baz {};

SCENARIO("querying type by it being a base of other type with 'base_of'", "[unit][feature][type]") {

	GIVEN("A resolver returning a Bar type") {
		auto resolver = [] { return type_c< Foo >; };

		THEN("'base_of' returns the resolver if the resolved type is base of the queried type") {
			BOOST_HANA_CONSTANT_CHECK( base_of< Bar >(resolver)() == type_c< Foo > );
		}

		AND_THEN("'base_of' returns void if the resolved type is not base of the queried type") {
			static_assert(std::is_same_v< decltype(base_of< Foo >(resolver)), void >);
			static_assert(std::is_same_v< decltype(base_of< Baz >(resolver)), void >);
		}
	}

}