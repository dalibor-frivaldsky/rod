#include <boost/hana/assert.hpp>
#include <catch.hpp>
#include <rod/feature/type/of_base>
#include <type_traits>

using namespace boost::hana;
using namespace rod::feature::query;


struct Foo {};

struct Bar: Foo {};

struct Baz {};

SCENARIO("querying type by its base type with 'of_base'", "[unit][feature][type]") {

	GIVEN("A resolver returning a Bar type of base type Foo") {
		auto resolver = [] { return type_c< Bar >; };

		THEN("'of_base' returns the resolver if the queried type is base of the resolved type") {
			BOOST_HANA_CONSTANT_CHECK( of_base< Foo >(resolver)() == type_c< Bar > );
		}

		AND_THEN("'of_base' returns void if the queried type is not base of the resolved type") {
			static_assert(std::is_same_v< decltype(of_base< Bar >(resolver)), void >);
			static_assert(std::is_same_v< decltype(of_base< Baz >(resolver)), void >);
		}
	}

}