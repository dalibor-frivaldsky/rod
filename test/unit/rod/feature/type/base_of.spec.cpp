#include <boost/hana/assert.hpp>
#include <catch.hpp>
#include <rod/feature/type/base_of>
#include <type_traits>

using namespace boost::hana;
using namespace rod::feature::query;


struct Foo {};

struct Bar: Foo {};

struct Baz {};

SCENARIO("querying for base type with 'base_of'", "[unit][feature][type]") {

	GIVEN("A resolver returning a Bar type") {
		auto resolver = [] { return type_c< Bar >; };

		THEN("'base_of' returns the resolver if the queried type is base of the resolved type") {
			BOOST_HANA_CONSTANT_CHECK( base_of< Foo >(resolver)() == type_c< Bar > );
		}

		AND_THEN("'base_of' returns void if the queried type is not base of the resolved type") {
			static_assert(std::is_same_v< decltype(base_of< Bar >(resolver)), void >);
			static_assert(std::is_same_v< decltype(base_of< Baz >(resolver)), void >);
		}
	}

}