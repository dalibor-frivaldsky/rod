#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/resolve_framework/feature_query/instance_of>
#include <rod/boost/hana_resolve_monad>

namespace hana = boost::hana;
using namespace rod::resolve_framework;


SCENARIO( "Resolving primitive values", "[integration][resolve]" ) {

	GIVEN( "Monad with primitive values" ) {
		auto primitives = hana::tuple( 10, 2.5, 5.0f );

		THEN( "instances of values can be resolved" ) {
			REQUIRE((int)(hana::length( resolve_impl( primitives, instance_of< int > ))) == 1);
			REQUIRE((int)(hana::length( resolve_impl( primitives, instance_of< float > ))) == 1);
			REQUIRE((int)(hana::length( resolve_impl( primitives, instance_of< double > ))) == 1);

			REQUIRE( hana::front( resolve_impl( primitives, instance_of< int > ) )() == 10 );
			REQUIRE( hana::front( resolve_impl( primitives, instance_of< float > ) )() == 5.0 );
			REQUIRE( hana::front( resolve_impl( primitives, instance_of< double > ) )() == 2.5 );
		}

		THEN( "instances of pointers to values can be resolved" ) {
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
		}
	}

}