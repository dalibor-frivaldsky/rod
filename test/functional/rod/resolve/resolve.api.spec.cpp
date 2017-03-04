// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/unique.hpp>
#include <catch.hpp>
#include <range/v3/view/for_each.hpp>
#include <rod/api/all>

namespace hana = boost::hana;
namespace q = rod::feature::query;


SCENARIO( "resolve API", "[functional]" ) {

	auto tuple = hana::tuple{ 10, 5.0f, 2.5 };
	auto [intValue] = tuple | rod::resolve(q::instance{ q::as< int > }) | rod::get;
	REQUIRE( intValue == 10 );

	auto [floatPtr] = tuple | rod::resolve(q::instance{ q::as< float* > }) | rod::get;
	REQUIRE( floatPtr == &tuple[ hana::int_c<1> ] );
	REQUIRE( *floatPtr == 5.0f );

	auto [doubleRef] = tuple | rod::resolve(q::instance{ q::as< double& > }) | rod::get;
	REQUIRE( &doubleRef == &tuple[ hana::int_c<2> ] );
	REQUIRE( doubleRef == 2.5 );
	
	tuple | rod::with([] (int a, float& b, double* c) {
		REQUIRE( a == 10 );
		REQUIRE( b == 5.0f );
		REQUIRE( *c == 2.5 );
	});

}

SCENARIO( "type-erased resolve API", "[functional]" ) {

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


struct Foo {};

struct Bar {};

struct Baz {};

struct Bazz: public Baz {};

SCENARIO("type resolve API", "[functional]") {
	auto tuple = hana::tuple{ Foo{}, hana::type_c< Bar >, hana::type_c< Bazz > };

	BOOST_HANA_CONSTANT_CHECK(
		hana::unique(
			hana::transform(
				tuple | rod::resolve(q::type{}) | rod::get_all,
				[] (auto&& e) {
					return std::get< 0 >(e);
				}
			)
		)
		==
		hana::tuple{ hana::type_c< Foo >, hana::type_c< Bar >, hana::type_c< Bazz > }
	);

	auto [fooType] = tuple | rod::resolve(q::type{ q::is< Foo >}) | rod::get;
	BOOST_HANA_CONSTANT_CHECK( fooType == hana::type_c< Foo > );

	auto [ofBazBaseType] = tuple | rod::resolve(q::type{ q::of_base< Baz >}) | rod::get;
	BOOST_HANA_CONSTANT_CHECK( ofBazBaseType == hana::type_c< Bazz > );
}


struct BaseA {
	virtual int operator() () = 0;
};

struct BaseB {
	virtual double operator() () = 0;
};

struct DerivedAA: BaseA {
	int operator() () {
		return 10;
	}
};

struct DerivedBA: BaseA {
	int operator() () {
		return 20;
	}
};

struct DerivedAB: BaseB {
	double operator() () {
		return 2.5;
	}
};

SCENARIO("Object hierarchy resolve API", "[functional]") {
	auto tuple = hana::tuple{ DerivedAA{}, DerivedAB{}, DerivedBA{} };

	auto [b] = tuple | rod::resolve(q::instance{ q::as< BaseB* > }) | rod::get;
	static_assert(std::is_same_v< decltype(b), BaseB* >);
	REQUIRE( (*b)() == 2.5 );

	auto [b2, bType] = tuple
		| rod::resolve(
			q::instance{ q::as< BaseB* > },
			q::type{})
		| rod::get;
	BOOST_HANA_CONSTANT_CHECK( bType == hana::type_c< DerivedAB > );

	auto [a, aType] = tuple
		| rod::resolve(
			q::instance{ q::as< BaseA* > },
			q::type{ q::is< DerivedBA > })
		| rod::get;
	REQUIRE( (*a)() == 20 );

	tuple | rod::with([] (BaseB& b) {
		REQUIRE( b() == 2.5 );
	});

	int sum = 0;
	hana::for_each(
		tuple | rod::resolve(q::instance{ q::as< BaseA& > }) | rod::get_all,
		[&] (auto&& f) {
			auto [baseARef] = f;
			sum += baseARef();
		});
	REQUIRE( sum == 30 );

	auto [baseADerivedType] = tuple | rod::resolve(q::type{ q::of_base< BaseA > }) | rod::get;
	BOOST_HANA_CONSTANT_CHECK(baseADerivedType == hana::type_c< DerivedAA >);
}

SCENARIO("Object hierarchy type-erased resolve API", "[functional]") {
	auto tuple = hana::tuple{ DerivedAA{}, DerivedAB{}, DerivedBA{}, hana::type_c< BaseB >, hana::type_c< BaseA > };
	rod::source::type_index	source = tuple | rod::erase;

	auto [b] = source | rod::resolve(q::instance{ q::as< BaseB* > }) | rod::get;
	static_assert(std::is_same_v< decltype(b), BaseB* >);
	REQUIRE( (*b)() == 2.5 );

	source | rod::with([] (BaseB& b) {
		REQUIRE( b() == 2.5 );
	});

	int sum = 0;
	for(
		BaseA& a:
		source
			| rod::resolve(q::instance{ q::as< BaseA& > })
			| rod::get_all
			| ranges::view::transform([] (auto&& e) -> decltype(auto) { return std::get<0>(e); })
	) {
		sum += a();
	}
	REQUIRE( sum == 30 );
}
