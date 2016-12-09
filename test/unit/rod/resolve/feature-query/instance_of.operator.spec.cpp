#include <catch.hpp>
#include <rod/resolve/concept/FeatureQueryOperator.concept>
#include <rod/resolve/feature-query/instance_of.operator>

using namespace rod::resolve;


SCENARIO( "instance_of operator is FeatureQueryOperator", "[resolve]" ) {
	static_assert( FeatureQueryOperator< instance_of< int > >() );
}
