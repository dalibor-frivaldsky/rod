#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/feature/type/type>
#include <utility>

namespace h = boost::hana;
using namespace rod::feature::query;


struct entity_with_int_type_feature {
	constexpr auto operator() (rod::feature::type_tag&&) const {
		return [] { return h::type_c< int >; };
	}
};

struct entity_with_no_features {};


SCENARIO( "querying for a type feature of an entity", "[unit][feature][type]" ) {

	GIVEN( "an entity with type feature" ) {
		entity_with_int_type_feature e{};

		THEN( "type resolver is extracted from the entity" ) {
			BOOST_HANA_CONSTANT_CHECK( type{}(e)() == h::type_c< int > );
		}
	}

	GIVEN( "an entity without type feature" ) {
		entity_with_no_features e{};

		THEN( "void is extracted from the entity" ) {
			BOOST_HANA_CONSTANT_CHECK( h::type_c< decltype(type{}(e)) > == h::type_c< void > );
		}
	}

}


struct subquery {
	template< typename T >
	constexpr decltype(auto) operator() (T&& t) const {
		if constexpr( t() == h::type_c< int > ) {
			return h::true_c;
		}
		else {
			return;
		}
	}
};

SCENARIO( "passing type feature to subqueries", "[unit][feature][type]" ) {

	GIVEN( "an entity with int type feature" ) {
		entity_with_int_type_feature e{};

		THEN( "type feature from the entity is passed to the subquery" ) {
			BOOST_HANA_CONSTANT_CHECK( type{ subquery{} }(e) );
		}
	}

}
