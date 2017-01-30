#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/feature/instance/as>
#include <rod/feature/instance/instance>

using namespace boost::hana;
using namespace rod::feature::query;


struct entity_with_int_instance_feature {
	constexpr auto operator() (rod::feature::instance_tag&&) {
		return [] { return 10; };
	}
};

struct entity_with_no_feature {};


SCENARIO("querying instances with a combination of an instance{ as }", "[integration][feature][instance]") {

	GIVEN("an entity with an int instance feature") {
		entity_with_int_instance_feature intFeatEnt{};

		THEN("the int instance can be queried with an instance{ as }") {
			REQUIRE(instance{ as< int > }(intFeatEnt)() == 10);
		}
	}

	GIVEN("an entity with no features") {
		entity_with_no_feature	noFeatEnt{};

		THEN("void is the return type of an instance{ as } query") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(instance{ as< int > }(noFeatEnt)) > == type_c< void >
			);
		}
	}

}