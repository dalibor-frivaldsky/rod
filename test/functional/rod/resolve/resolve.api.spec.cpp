
#include <catch.hpp>
#include <rod/erase>
#include <rod/get>
#include <rod/pipe>
#include <rod/resolve>
#include <rod/with>
#include <rod/boost/hana_get>
#include <rod/boost/hana_resolve_monad>
#include <rod/range_v3/get>
#include <rod/resolve_framework/type_index_source>

namespace hana = boost::hana;
using namespace rod::operators;
using rod::resolve_framework::instance_of;
using rod::resolve_framework::type_index_source;


SCENARIO( "resolve API" ) {

	auto tuple = hana::tuple( 10, 2.5f, 5.0 );
	REQUIRE( (tuple | rod::resolve(instance_of< int >) | rod::get) == 10 );

	float& floatRef = tuple | rod::resolve(instance_of< float& >) | rod::get;
	REQUIRE( &floatRef == &tuple[ hana::int_c<1> ] );
	
	tuple | rod::with([] (int a, float& b, double* c) {
		REQUIRE( a == 10 );
		REQUIRE( b == 2.5f );
		REQUIRE( *c == 5.0 );
	});

}

SCENARIO( "type-erased resolve API" ) {

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

}
