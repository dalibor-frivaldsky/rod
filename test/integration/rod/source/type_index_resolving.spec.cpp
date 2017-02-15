#include <catch.hpp>
#include <rod/feature/instance/instance>
#include <rod/feature/instance/as>
#include <rod/resolve>
#include <rod/source/type_index>

using namespace rod;
using namespace rod::feature::query;


template< typename Range >
int count_elements(Range&& range) {
	int i = 0;

	for(auto&& e: range) {
		++i;
	}

	return i;
}


SCENARIO( "Resolving instances from type_index_source", "[integration][source][type_index]" ) {

	GIVEN( "type_index_source and int, int& and float resolvers" ) {
		source::type_index source{};

		int i = 12;
		auto intResolver = [] { return 10; };
		auto intRefResolver = [&] () -> int& { return i; };
		auto floatResolver = [] { return 5.0f; };

		WHEN( "they are registered") {
			source.register_resolver( intResolver );
			source.register_resolver( intRefResolver );
			source.register_resolver( floatResolver );

			THEN( "instances can be resolved" ) {
				REQUIRE( count_elements(resolve(instance{ as< int > })(source)) == 1 );
				REQUIRE( count_elements(resolve(instance{ as< int& > })(source)) == 1 );
				REQUIRE( count_elements(resolve(instance{ as< float > })(source)) == 1 );

				REQUIRE( std::get< 0 >(ranges::front(resolve(instance{ as< int > })(source)))() == 10 );
				REQUIRE( std::get< 0 >(ranges::front(resolve(instance{ as< float > })(source)))() == 5.0f );

				int& resolvedIntRef = std::get< 0 >(ranges::front(resolve(instance{ as< int& > })(source)))();
				REQUIRE( &resolvedIntRef == &i );
				REQUIRE( resolvedIntRef == 12 );
			}
		}
	}

}