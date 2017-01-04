#include <any>
#include <catch.hpp>
#include <functional>
#include <iostream>
#include <rod/resolve_framework/type_index_source>
#include <typeindex>

using namespace rod::resolve_framework;


SCENARIO( "Registering resolvers to type_index_source" ) {

	type_index_source source{};

	GIVEN( "int resolver" ) {
		auto intResolver = [] { return 10; };

		WHEN( "it is registered" ) {
			source.register_resolver( intResolver );

			THEN( "it is retrieved as a pair of std::function type_index and std::any" ) {
				auto& resolverPairs = source.get_all_resolvers();

				REQUIRE( ranges::front(resolverPairs).first == std::type_index(typeid(std::function< int() >)) );

				AND_THEN( "the value can by any_cast-ed to std::function wrapping the registered resolver" ) {
					auto intFnResolver = std::any_cast< std::function< int() > >(&ranges::front(resolverPairs).second);

					REQUIRE( (*intFnResolver)() == 10 );
				}
			}
		}
	}

}