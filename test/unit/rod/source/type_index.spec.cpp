#include <boost/hana/assert.hpp>
#include <boost/hana/length.hpp>
#include <catch.hpp>
#include <rod/feature/instance/as>
#include <rod/feature/instance/instance>
#include <rod/source/type_index>

using namespace boost::hana;
using namespace rod::feature;
using namespace rod::feature::query;
using namespace rod::source;


template< typename Range >
int count_elements(Range&& range) {
	int i = 0;

	for(auto&& e: range) {
		++i;
	}

	return i;
}


SCENARIO( "Registering resolvers to type_index_source", "[unit][source][type_index]" ) {

	type_index source{};

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


struct Foo {};
struct Bar {};

struct featurex_tag {};

struct featurex {
	using tag = featurex_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		if constexpr(std::is_callable_v< E(tag) >) {
			return e(tag{});
		}
	}
};

SCENARIO("Querying entities", "[unit][source][type_index]") {

	type_index	source{};

	GIVEN("int, int, Foo&, Bar* resolvers") {
		Foo	foo{};
		Bar bar{};
		auto intResolver = [] { return 10; };
		auto int2Resolver = [] { return 20; };
		auto fooRefResolver = [&] () -> Foo& { return foo; };
		auto barPtrResolver = [&] { return &bar; };

		WHEN("they are registered") {
			source.register_resolver(intResolver);
			source.register_resolver(int2Resolver);
			source.register_resolver(fooRefResolver);
			source.register_resolver(barPtrResolver);

			THEN("the query result is hana::tuple tagged with tag_matcher_query_result") {
				auto result =  query_entities(source);
				static_assert(build::TagMatcherQueryResult< decltype(result) >());
			}

			THEN("an entity is generated for each of them") {
				auto result = query_entities(source);
				REQUIRE(count_elements(result.ranges[int_c<0>]) == 4);
			}

			THEN("instance entity is generated for an instance query") {
				auto anyResult = query_entities(source, instance{});
				REQUIRE(count_elements(anyResult.ranges[int_c<0>]) == 4);
				for(auto&& e: anyResult.ranges[int_c<0>]) {
					BOOST_HANA_CONSTANT_CHECK(
						type_c< std::decay_t< decltype(e(instance_tag{})()) > >
							==
						type_c< std::any >
					);
				}

				auto intResult = query_entities(source, instance{ as< int > });
				REQUIRE(count_elements(intResult.ranges[int_c<0>]) == 2);
				auto checker = 1;
				for(auto&& e: intResult.ranges[int_c<0>]) {
					if(e(instance_tag{})() == 10) {
						checker *= 2;
					}
					if(e(instance_tag{})() == 20) {
						checker *= 3;
					}
				}
				REQUIRE(checker == 6);
			}

			THEN("no instances entities are returned when queried for unregistered instancee") {
				auto result = query_entities(source, instance{ as< int& > });
				REQUIRE(count_elements(result.ranges[int_c<0>]) == 0);
			}

			THEN("an empty view of entities is returned when asked for unsupported feature") {
				auto result =  query_entities(source, featurex{});
				BOOST_HANA_CONSTANT_CHECK(
					length(result.ranges) == size_c< 0 >
				);
			}
		}
	}

}
