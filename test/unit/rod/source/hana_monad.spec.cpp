#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/concept/Resolver>
#include <rod/source/hana_monad>

using namespace boost::hana;
using namespace rod;


SCENARIO("HanaType is a concept modeling boost::hana::type") {
	static_assert(!source::detail::HanaType< int >());
	static_assert(source::detail::HanaType< type< int > >());
	static_assert(source::detail::HanaType< type< int >& >());
}

template< typename feature_tag >
auto allResolvers = [] (auto& entities) {
	return fold(
		transform(
			entities,
			[] (auto a) { return bool_c< Resolver< decltype(a(feature_tag{})) >() >; }
		),
		true_c,
		[] (auto r, auto isResolver) { return r && isResolver; }
	);
};

template< typename feature_tag, typename... R >
auto resolvableTypes = [] (auto& entities) {
	auto types = transform(
		entities,
		[] (auto entity) { return type_c< decltype(entity(feature_tag{})()) >; }
	);
	auto expected = tuple( type_c< R >... );

	return is_subset( types, expected ) && is_subset( expected, types );
};

SCENARIO( "Generating entities", "[unit][source][hana_tuple]" ) {

	GIVEN( "T&" ) {
		int 	a{0};
		int&	refA = a;

		WHEN( "entities are generated" ) {
			auto entities = source::detail::entity_generator< int& >::generate( refA );

			THEN( "they all provide resolvers of an instance and type feature" ) {
				BOOST_HANA_CONSTANT_CHECK( allResolvers< feature::instance_tag >( entities ) );
				BOOST_HANA_CONSTANT_CHECK( allResolvers< feature::type_tag >( entities ) );
			}

			AND_THEN( "they resolve instances of T, T&, T*" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< feature::instance_tag, int, int&, int* >( entities ) );
			}

			AND_THEN( "they resolve type T" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< feature::type_tag, type<int>, type<int>, type<int> >( entities ) );
			}
		}
	}

	GIVEN( "T*" ) {
		int 	a{0};
		int*	ptrA = &a;

		WHEN( "entities are generated" ) {
			auto entities = source::detail::entity_generator< int*& >::generate( ptrA );

			THEN( "they all provide resolvers of an instance and type feature" ) {
				BOOST_HANA_CONSTANT_CHECK( allResolvers< feature::instance_tag >( entities ) );
				BOOST_HANA_CONSTANT_CHECK( allResolvers< feature::type_tag >( entities ) );
			}

			AND_THEN( "they resolve T, T&, T*" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< feature::instance_tag, int, int&, int* >( entities ) );
			}

			AND_THEN( "they resolve type T" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< feature::type_tag, type<int>, type<int>, type<int> >( entities ) );
			}
		}
	}

	GIVEN( "type_c<T>" ) {
		auto intType = type_c< int >;

		WHEN( "entities are generated" ) {
			auto entities = source::detail::entity_generator< basic_type< int > >::generate(intType);
			
			THEN( "they all provide resolvers of a type feature" ) {
				BOOST_HANA_CONSTANT_CHECK( allResolvers< feature::type_tag >( entities ) );
			}

			AND_THEN( "they resolve type T" ) {
				BOOST_HANA_CONSTANT_CHECK( resolvableTypes< feature::type_tag, type<int> >( entities ) );
			}
		}
	}

}


SCENARIO( "querying entities from boost::hana::tuple", "[unit][source][hana_tuple]" ) {

	GIVEN( "boost::hana::tuple with an int value" ) {
		auto t = tuple{ 10 };

		WHEN( "entities are queried" ) {
			auto entities = query_entities( t );

			THEN( "more than one entity is returned" ) {
				BOOST_HANA_CONSTANT_CHECK( length(entities) > size_c< 0 > );
			}
		}		
	}

}


struct feature1_tag {};
struct feature2_tag {};
struct featurex_tag {};

struct nof_entity {};

struct f1_entity {
	constexpr auto operator() (feature1_tag) {
		return [] { return 10; };
	}
};

struct f1f2_entity {
	constexpr auto operator() (feature1_tag) {
		return [] { return 20; };
	}

	constexpr auto operator() (feature2_tag) {
		return [] { return 5.0; };
	}
};

struct feature1 {
	using tag = feature1_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		if constexpr(std::is_callable_v< E(tag) >) {
			return e(tag{});
		}
	}
};

struct feature2 {
	using tag = feature2_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		if constexpr(std::is_callable_v< E(tag) >) {
			return e(tag{});
		}
	}
};

struct featurex {
	using tag = featurex_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		if constexpr(std::is_callable_v< E(tag) >) {
			return e(tag{});
		}
	}
};

SCENARIO( "filtering entities from boost::hana::tuple", "[unit][source][hana_tuple]" ) {

	GIVEN( "boost::hana::tuple of f1 entity, f1f2 entity and no feature entity" ) {
		auto t = tuple{
			f1_entity{},
			f1f2_entity{},
			nof_entity{}
		};

		WHEN( "entities are filtered with no feature queries" ) {
			auto result = filter_entities( t );

			THEN( "empty resolver std::tuples for all entities are returned" ) {
				BOOST_HANA_CONSTANT_CHECK( length(result) == size_c< 3 > );

				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(result[int_c< 0 >]) > == type_c< std::tuple<>& >
				);
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(result[int_c< 1 >]) > == type_c< std::tuple<>& >
				);
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(result[int_c< 2 >]) > == type_c< std::tuple<>& >
				);
			}
		}

		WHEN( "entities are filtered by feature they don't provide" ) {
			auto result = filter_entities( t, featurex{} );

			THEN( "no resolver std::tuples are returned" ) {
				BOOST_HANA_CONSTANT_CHECK( length(result) == size_c< 0 > );
			}
		}

		WHEN( "entities are filtered by feature1" ) {
			auto result = filter_entities( t, feature1{} );

			THEN( "resolver std::tuples of f1 and f1f2 entities are returned" ) {
				BOOST_HANA_CONSTANT_CHECK( length(result) == size_c< 2 > );
			}

			AND_THEN( "the resolver std::tuples contain feature1 resolvers only" ) {
				auto r1 = result[int_c< 0 >];
				static_assert(std::tuple_size_v< decltype(r1) > == 1);
				REQUIRE(std::get< 0 >(r1)() == 10);

				auto r2 = result[int_c< 1 >];
				static_assert(std::tuple_size_v< decltype(r2) > == 1);
				REQUIRE(std::get< 0 >(r2)() == 20);
			}
		}

		WHEN( "entities are filtered by feature2 and feature1" ) {
			auto result = filter_entities( t, feature2{}, feature1{} );

			THEN( "resolver std::tuples of f1f2 entities are returned" ) {
				BOOST_HANA_CONSTANT_CHECK( length(result) == size_c< 1 > );
			}

			AND_THEN( "the resolver std::tuple contains feature2 and feature1 resolvers in that order" ) {
				auto r1 = result[int_c< 0 >];
				static_assert(std::tuple_size_v< decltype(r1) > == 2);
				REQUIRE(std::get< 0 >(r1)() == 5.0);
				REQUIRE(std::get< 1 >(r1)() == 20);
			}
		}
	}

}
