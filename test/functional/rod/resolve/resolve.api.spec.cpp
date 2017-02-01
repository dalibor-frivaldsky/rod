#include <catch.hpp>
#include <rod/api/all>
/*#include <rod/erase>
#include <rod/get>
#include <rod/pipe>
#include <rod/resolve>
#include <rod/with>
#include <rod/boost/hana_get>
#include <rod/boost/hana_resolve_monad>
#include <rod/range_v3/get>
#include <rod/resolve_framework/type_index_source>*/

namespace hana = boost::hana;
namespace q = rod::feature::query;
/*using namespace rod::operators;
using rod::resolve_framework::instance_of;
using rod::resolve_framework::type_index_source;*/


SCENARIO( "resolve API" ) {

	auto tuple = hana::tuple{ 10, 5.0f, 2.5 };
	auto [intValue] = tuple | rod::resolve(q::instance{ q::as< int > }) | rod::get;
	REQUIRE( intValue == 10 );

	auto [floatPtr] = tuple | rod::resolve(q::instance{ q::as< float* > }) | rod::get;
	REQUIRE( floatPtr == &tuple[ hana::int_c<1> ] );
	
	tuple | rod::with([] (int a, float& b, double* c) {
		REQUIRE( a == 10 );
		REQUIRE( b == 5.0f );
		REQUIRE( *c == 2.5 );
	});

}

/*SCENARIO( "type-erased resolve API" ) {

	auto tuple = hana::tuple( 10, 2.5f, 5.0 );
	type_index_source	source = tuple | rod::erase;

	REQUIRE( (source | rod::resolve(instance_of< int >) | rod::get) == 10 );

	float& floatRef = source | rod::resolve(instance_of< float& >) | rod::get;
	REQUIRE( &floatRef == &tuple[ hana::int_c<1> ] );
	
	source | rod::with([] (int a, float& b, double* c) {
		REQUIRE( a == 10 );
		REQUIRE( b == 2.5f );
		REQUIRE( *c == 5.0 );
	});

}*/
