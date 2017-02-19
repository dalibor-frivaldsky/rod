// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/front.hpp>
#include <catch.hpp>
#include <rod/feature/instance/as>
#include <rod/feature/instance/instance>
#include <rod/feature/type/type>
#include <rod/feature/type/is>
#include <rod/resolve>
#include <rod/source/hana_monad>

namespace h = boost::hana;
using namespace rod;
using namespace rod::feature::query;


SCENARIO( "Resolving primitive values from boost::hana::tuple", "[integration][source][hana_tuple]" ) {

	GIVEN( "hana::tuple with primitive values" ) {
		double d = 2.5;
		float f = 5.0f;
		auto primitives = h::tuple{ 10, d, &f };
		
		THEN( "instances of values can be resolved" ) {
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< int > })(primitives)) == h::size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< float > })(primitives)) == h::size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< double > })(primitives)) == h::size_c< 3 >
			);
			
			REQUIRE( std::get< 0 >(resolve(instance{ as< int > })(primitives)[h::int_c< 0 >])() == 10 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< int > })(primitives)[h::int_c< 1 >])() == 2 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< int > })(primitives)[h::int_c< 2 >])() == 5 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< float > })(primitives)[h::int_c< 0 >])() == 10.0f );
			REQUIRE( std::get< 0 >(resolve(instance{ as< float > })(primitives)[h::int_c< 1 >])() == 2.5f );
			REQUIRE( std::get< 0 >(resolve(instance{ as< float > })(primitives)[h::int_c< 2 >])() == 5.0f );
			REQUIRE( std::get< 0 >(resolve(instance{ as< double > })(primitives)[h::int_c< 0 >])() == 10.0 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< double > })(primitives)[h::int_c< 1 >])() == 2.5 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< double > })(primitives)[h::int_c< 2 >])() == 5.0 );
		}

		THEN( "pointers to values can be resolved" ) {
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< int* > })(primitives)) == h::size_c< 1 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< float* > })(primitives)) == h::size_c< 1 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< double* > })(primitives)) == h::size_c< 1 >
			);

			int* intPtr = std::get< 0 >(resolve(instance{ as< int* > })(primitives)[h::int_c< 0 >])();
			REQUIRE( intPtr == &(primitives[h::int_c<0>]) );
			REQUIRE( *intPtr == 10 );

			float* floatPtr = std::get< 0 >(resolve(instance{ as< float* > })(primitives)[h::int_c< 0 >])();
			REQUIRE( floatPtr == &f );
			REQUIRE( *floatPtr == 5.0f );

			double* doublePtr = std::get< 0 >(resolve(instance{ as< double* > })(primitives)[h::int_c< 0 >])();
			REQUIRE( doublePtr == &(primitives[h::int_c<1>]) );
			REQUIRE( *doublePtr == 2.5 );
		}

		THEN( "references to values can be resolved" ) {
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< int& > })(primitives)) == h::size_c< 1 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< float& > })(primitives)) == h::size_c< 1 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(instance{ as< double& > })(primitives)) == h::size_c< 1 >
			);

			int& intRef = std::get< 0 >(resolve(instance{ as< int& > })(primitives)[h::int_c< 0 >])();
			REQUIRE( &intRef == &(primitives[h::int_c<0>]) );
			REQUIRE( intRef == 10 );
			
			float& floatRef = std::get< 0 >(resolve(instance{ as< float& > })(primitives)[h::int_c< 0 >])();
			REQUIRE( &floatRef == &f );
			REQUIRE( floatRef == 5.0f );

			double& doubleRef = std::get< 0 >(resolve(instance{ as< double& > })(primitives)[h::int_c< 0 >])();
			REQUIRE( &doubleRef == &(primitives[h::int_c<1>]) );
			REQUIRE( doubleRef == 2.5 );
		}

		THEN("types can be resolved") {
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{ is< int > })(primitives)) == h::size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{ is< float > })(primitives)) == h::size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{ is< double > })(primitives)) == h::size_c< 3 >
			);
		}
	}

	GIVEN( "hana::tuple with types" ) {
		auto types = h::tuple{ h::type_c< int >, h::type_c< float >, h::type_c< double > };

		THEN("types can be resolved") {
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{})(types)) == h::size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{ is< int > })(types)) == h::size_c< 1 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{ is< float > })(types)) == h::size_c< 1 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				h::length(resolve(type{ is< double > })(types)) == h::size_c< 1 >
			);
		}
	}

}