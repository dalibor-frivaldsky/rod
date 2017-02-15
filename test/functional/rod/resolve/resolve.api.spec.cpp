#include <catch.hpp>
#include <rod/api/all>

namespace hana = boost::hana;
namespace q = rod::feature::query;


SCENARIO( "resolve API" ) {

	auto tuple = hana::tuple{ 10, 5.0f, 2.5 };
	auto [intValue] = tuple | rod::resolve(q::instance{ q::as< int > }) | rod::get;
	REQUIRE( intValue == 10 );

	auto [floatPtr] = tuple | rod::resolve(q::instance{ q::as< float* > }) | rod::get;
	REQUIRE( floatPtr == &tuple[ hana::int_c<1> ] );
	REQUIRE( *floatPtr == 5.0f );
	
	tuple | rod::with([] (int a, float& b, double* c) {
		REQUIRE( a == 10 );
		REQUIRE( b == 5.0f );
		REQUIRE( *c == 2.5 );
	});

}

SCENARIO( "type-erased resolve API" ) {

	auto tuple = hana::tuple( 10, 5.0f, 2.5 );
	rod::source::type_index	source = tuple | rod::erase;

	auto [intValue] = source | rod::resolve(q::instance{ q::as< int > }) | rod::get;
	REQUIRE( intValue == 10 );

	auto [floatPtr] = source | rod::resolve(q::instance{ q::as< float* > }) | rod::get;
	REQUIRE( floatPtr == &tuple[ hana::int_c<1> ] );
	REQUIRE( *floatPtr == 5.0f );
	
	source | rod::with([] (int a, float& b, double* c) {
		REQUIRE( a == 10 );
		REQUIRE( b == 5.0f );
		REQUIRE( *c == 2.5 );
	});

}
