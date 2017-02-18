#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/feature/instance/instance>
#include <utility>

using namespace boost::hana;
using namespace rod::feature::query;


struct entity_with_int_instance_feature {
	constexpr auto operator() (rod::feature::instance_tag&&) const {
		return [] { return 10; };
	}
};

struct entity_with_no_features {};


SCENARIO( "querying for an instance feature of an entity", "[unit][feature][instance]" ) {

	GIVEN( "an entity with instance feature" ) {
		entity_with_int_instance_feature e{};

		THEN( "instance resolver is extracted from the entity" ) {
			BOOST_HANA_CONSTANT_CHECK( type_c< decltype(instance{}(e)) > != type_c< void > );
			BOOST_HANA_CONSTANT_CHECK( type_c< decltype(instance{}(e)()) > == type_c< int > );
		}
	}

	GIVEN( "an entity without instance feature" ) {
		entity_with_no_features e{};

		THEN( "void is extracted from the entity" ) {
			BOOST_HANA_CONSTANT_CHECK( type_c< decltype(instance{}(e)) > == type_c< void > );
		}
	}

}


struct subquery {
	template< typename I >
	constexpr decltype(auto) operator() (I&&) const {
		if constexpr( std::is_same_v< std::result_of_t< I() >, int > ) {
			return [] { return 10.0; };
		}
		else {
			return;
		}
	}
};

SCENARIO( "passing instance feature to subqueries", "[unit][feature][instance]" ) {

	GIVEN( "an entity with int instance feature" ) {
		entity_with_int_instance_feature e{};

		THEN( "instance feature from the entity is passed to the subquery" ) {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(instance{ subquery{} }(e)()) > == type_c< double >
			);
		}
	}

}
