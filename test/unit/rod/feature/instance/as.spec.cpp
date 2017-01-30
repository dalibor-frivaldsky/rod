#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/feature/instance/as>
#include <type_traits>

using namespace boost::hana;
using namespace rod::feature::query;


struct Foo {};

SCENARIO("querying for return type convertibility with 'as'", "[unit][feature][instance]") {

	GIVEN("a resolver returning an int") {
		auto intResolver = [] { return 10; };

		THEN("'as' returns the resolver when the type matches") {
			REQUIRE(&intResolver == &as< int >(intResolver));
			REQUIRE(as< int >(intResolver)() == 10);
		}

		THEN("'as' returns another resolver returning the requested type when the types are convertible") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< float >(intResolver)()) > == type_c< float >
			);
			REQUIRE(as< float >(intResolver)() == 10.0f);
		}

		THEN("'as' returns void when the types are not convertible") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< Foo >(intResolver)) > == type_c< void >
			);
		}
	}

	GIVEN("a resolver returning a reference to int") {
		int	i = 10;
		auto intRefResolver = [&] () -> int& { return i; };

		THEN("'as' returns the resolver when requesting reference to int") {
			REQUIRE(&intRefResolver == &as< int& >(intRefResolver));
			REQUIRE(as< int& >(intRefResolver)() == 10);
		}

		THEN("'as' returns void when requesting convertible-to type, but not as a reference") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< float >(intRefResolver)) > == type_c< void >
			);
		}
	}

}


namespace test {

	struct convertible_to_any {};

	template< typename C >
	C convert(convertible_to_any) {}

}

SCENARIO("deducing expected typ of 'as'") {

	GIVEN("a resolver returning object convertible to any type") {
		auto ctaR = [] { return test::convertible_to_any{}; };

		THEN("type expected in 'as' can be deduced by the object to 'as'") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< float >(ctaR)()) > == type_c< float >
			);
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< float* >(ctaR)()) > == type_c< float* >
			);
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< float& >(ctaR)()) > == type_c< float& >
			);
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(as< const float& >(ctaR)()) > == type_c< const float& >
			);
		}
	}

}
