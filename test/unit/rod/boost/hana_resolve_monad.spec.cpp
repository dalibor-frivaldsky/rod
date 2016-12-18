#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/resolve_framework/concept/Resolver>
#include <rod/boost/hana_resolve_monad>

namespace hana = boost::hana;
using namespace hana::literals;
using namespace rod;


auto allResolvers = [] (auto& resolvers) {
	return hana::fold(
		hana::transform(
			resolvers,
			[] (auto a) { return hana::bool_c< resolve_framework::Resolver< decltype(a) >() >; }
		),
		hana::true_c,
		[] (auto r, auto isResolver) { return r && isResolver; }
	);
};

template< typename... R >
auto resolvableTypes = [] (auto& resolvers) {
	auto types = hana::transform(
		resolvers,
		[] (auto resolver) { return hana::type_c< decltype(resolver()) >; }
	);
	auto expected = hana::tuple( hana::type_c< R >... );

	return hana::is_subset( types, expected ) && hana::is_subset( expected, types );
};

SCENARIO( "Generating resolvers", "[unit][resolve]" ) {

	GIVEN( "T&" ) {
		int 	a{0};
		int&	refA = a;

		WHEN( "resolvers are generated" ) {
			auto resolvers = boost_detail::generate_resolvers( refA );

			THEN( "all fulfill the Resolver concept" ) {
				BOOST_HANA_CONSTANT_CHECK( allResolvers( resolvers ) );
			}

			AND_THEN( "they resolve T, T&, T*" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< int, int&, int* >( resolvers ) );
			}
		}
	}

	GIVEN( "T*" ) {
		int 	a{0};
		int*	ptrA = &a;

		WHEN( "resolvers are generated" ) {
			auto resolvers = boost_detail::generate_resolvers( ptrA );

			THEN( "all fulfill the Resolver concept" ) {
				BOOST_HANA_CONSTANT_CHECK( allResolvers( resolvers ) );
			}

			AND_THEN( "they resolve T, T&, T*" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< int, int&, int* >( resolvers ) );
			}
		}
	}

}

SCENARIO( "Resolving boost::hana::tuple", "[unit][resolve]" ) {

	GIVEN( "boost::hana::tuple with an int value" ) {
		auto tuple = hana::tuple( 10 );

		WHEN( "all resolvers are requested" ) {
			auto resolvers = hana::resolve_impl( tuple, [] (auto a) { return hana::true_c; } );

			THEN( "all return value from the tuple" ) {
				hana::for_each(
					resolvers,
					[] (auto resolver) {
						if constexpr( std::is_pointer< decltype(resolver()) >::value ) {
							REQUIRE( *resolver() == 10 );
						}
						else {
							REQUIRE( resolver() == 10 );
						}
					});
			}
		}

		WHEN( "no resolvers are requested" ) {
			auto resolvers = hana::resolve_impl( tuple, [] (auto a) { return hana::false_c; } );

			THEN( "an empty monad is returned" ) {
				BOOST_HANA_CONSTANT_CHECK(hana::length( resolvers ) == hana::size_c<0> );
			}
		}
		
	}

}