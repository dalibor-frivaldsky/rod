#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/front.hpp>
#include <catch.hpp>
#include <rod/feature/instance/as>
#include <rod/feature/instance/instance>
#include <rod/resolve>
#include <rod/source/hana_monad>

using namespace boost::hana;
using namespace rod;
using namespace rod::feature::query;


SCENARIO( "Resolving primitive values from boost::hana::tuple", "[integration][source][hana_tuple]" ) {

	GIVEN( "hana::tuple with primitive values" ) {
		int i = 10;
		auto primitives = boost::hana::tuple{ &i, 2.5, 5.0f };

		THEN( "instances of values can be resolved" ) {
			BOOST_HANA_CONSTANT_CHECK(
				length(resolve(instance{ as< int > })(primitives)) == size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				length(resolve(instance{ as< float > })(primitives)) == size_c< 3 >
			);
			BOOST_HANA_CONSTANT_CHECK(
				length(resolve(instance{ as< double > })(primitives)) == size_c< 3 >
			);
			
			REQUIRE( std::get< 0 >(resolve(instance{})(primitives)[int_c< 0 >])() == 10 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< int > })(primitives)[int_c< 1 >])() == 2 );
			REQUIRE( std::get< 0 >(resolve(instance{ as< int > })(primitives)[int_c< 2 >])() == 5 );
			//REQUIRE( front(resolve(instance{ as< int > }))() == 5.0 );
			//REQUIRE( front(resolve(instance{ as< int > }))() == 2.5 );
		}

		/*THEN( "instances of pointers to values can be resolved" ) {
			REQUIRE( *(hana::front( resolve_impl( primitives, instance_of< int* > ) )()) == 10 );
			REQUIRE( *(hana::front( resolve_impl( primitives, instance_of< float* > ) )()) == 5.0f );
			REQUIRE( *(hana::front( resolve_impl( primitives, instance_of< double* > ) )()) == 2.5 );
		}

		THEN( "instances of references to values can be resolved" ) {
			int& intRef = hana::front( resolve_impl( primitives, instance_of< int& > ) )();
			REQUIRE( &intRef == &(primitives[hana::int_c<0>]) );
			REQUIRE( intRef == 10 );
			
			REQUIRE( hana::front( resolve_impl( primitives, instance_of< float& > ) )() == 5.0 );
			REQUIRE( hana::front( resolve_impl( primitives, instance_of< double& > ) )() == 2.5 );
		}*/
	}

}