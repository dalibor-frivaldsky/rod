#include <catch.hpp>
#include <rod/resolve_framework/type_index_source>
#include <rod/resolve_framework/feature_query/instance_of>

using namespace rod::resolve_framework;


template< typename Range >
int count_elements(Range&& range) {
	int i = 0;

	for(auto&& e: range) {
		++i;
	}

	return i;
}


SCENARIO( "Resolving instances from type_index_source", "[integration][resolve]" ) {

	GIVEN( "type_index_source and int, int& and float resolvers" ) {
		type_index_source source{};

		int i = 12;
		auto intResolver = [] { return 10; };
		auto intRefResolver = [&] () -> int& { return i; };
		auto floatResolver = [] { return 5.0f; };

		WHEN( "they are registered") {
			source.register_resolver( intResolver );
			source.register_resolver( intRefResolver );
			source.register_resolver( floatResolver );

			THEN( "instances can be resolved" ) {
				REQUIRE( count_elements(resolve_impl(source, instance_of< int >)) == 1 );
				REQUIRE( count_elements(resolve_impl(source, instance_of< int& >)) == 1 );
				REQUIRE( count_elements(resolve_impl(source, instance_of< float >)) == 1 );

				REQUIRE( ranges::front(resolve_impl(source, instance_of< int > ))() == 10 );
				REQUIRE( ranges::front(resolve_impl(source, instance_of< float > ))() == 5.0f );

				int& resolvedIntRef = ranges::front(resolve_impl(source, instance_of< int& > ))();
				REQUIRE( &resolvedIntRef == &i );
				REQUIRE( resolvedIntRef == 12 );
			}
		}
	}

}