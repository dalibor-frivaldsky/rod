#include <boost/hana/assert.hpp>
#include <catch.hpp>
#include <rod/feature/type/is>
#include <type_traits>

using namespace boost::hana;
using namespace rod::feature::query;


SCENARIO("querying for exact type match with 'is'", "[unit][feature][type]") {

	GIVEN("A resolver returning an int type") {
		auto resolver = [] { return type_c< int >; };

		THEN("'is' returns the resolver if the queried type matches") {
			BOOST_HANA_CONSTANT_CHECK( is< int >(resolver)() == type_c< int > );
		}

		AND_THEN("'is' returns void if the queried type does not match") {
			static_assert(std::is_same_v< decltype(is< float >(resolver)), void >);
		}
	}

}