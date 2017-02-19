// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <boost/hana/assert.hpp>
#include <catch.hpp>
#include <rod/source/build/entity>
#include <rod/source/build/tag_matcher>

using namespace boost::hana;
using namespace rod::source::build;


struct feature1_tag {};
struct feature2_tag {};
struct feature3_tag {};

struct feature1 {
	using tag = feature1_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		return e(tag{});
	}
};

struct feature2 {
	using tag = feature2_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		return e(tag{});
	}
};

struct feature3 {
	using tag = feature3_tag;

	template< typename E >
	constexpr decltype(auto) operator() (E&& e) {
		return e(tag{});
	}
};


SCENARIO("asserting tag_matcher_case properties") {

	GIVEN("tag_matcher_case with a tag set and generator function") {
		auto matcherCase = tag_matcher_case{
			tuple{ feature1_tag{}, feature2_tag{} },
			[] { return 10; }
		};

		THEN("the tag set is available under 'tags' property") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(matcherCase)::tags > == type_c< tuple< feature1_tag, feature2_tag > >
			);
		}

		AND_THEN("the generator function is called through the () operator") {
			REQUIRE(matcherCase() == 10);
		}
	}

}

SCENARIO("matching tags with no cases") {

	GIVEN("tag_matcher with no cases") {
		auto matcher = tag_matcher{};

		THEN("empty result is returned for any feature queries") {
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(matcher()) >
					==
				type_c< tag_matcher_query_result< tuple<> > >
			);
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(matcher(feature1{})) >
					==
				type_c< tag_matcher_query_result< tuple<> > >
			);
			BOOST_HANA_CONSTANT_CHECK(
				type_c< decltype(matcher(feature1{}, feature2{})) >
					==
				type_c< tag_matcher_query_result< tuple<> > >
			);
		}
	}

}

SCENARIO("matching tags with one case") {

	GIVEN("tag_matcher with one case") {
		auto matcher = tag_matcher{
			tag_matcher_case {
				tuple{ feature1_tag{} },
				[] {
					return 10;
				}
			}
		};

		WHEN("no query is given") {
			THEN("result of the generator function is returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher()) >
						==
					type_c< tag_matcher_query_result< tuple<int> > >
				);
				REQUIRE(matcher().ranges[int_c< 0 >] == 10);
			}
		}

		AND_WHEN("query matches the tag set of the case") {
			THEN("result of the generator function is returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature1{})) >
						==
					type_c< tag_matcher_query_result< tuple<int> > >
				);
				REQUIRE(matcher(feature1{}).ranges[int_c< 0 >] == 10);
			}
		}

		AND_WHEN("queries do not match the case tag set") {
			THEN("and empty result is returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature3{})) >
						==
					type_c< tag_matcher_query_result< tuple<> > >
				);
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature1{}, feature2{})) >
						==
					type_c< tag_matcher_query_result< tuple<> > >
				);
			}
		}
	}

}

SCENARIO("matching tags with multiple") {

	GIVEN("tag_matcher with multiple case") {
		auto matcher = tag_matcher{
			tag_matcher_case {
				tuple{ feature1_tag{} },
				[] {
					return 10;
				}
			},
			tag_matcher_case {
				tuple{ feature1_tag{}, feature3_tag{} },
				[] {
					return 5.0f;
				}
			},
			tag_matcher_case {
				tuple{ feature2_tag{}, feature3_tag{} },
				[] {
					return 2.5;
				}
			}
		};

		WHEN("no query is given") {
			THEN("results of all the generator functions are returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher()) >
						==
					type_c< tag_matcher_query_result< tuple< int, float, double > > >
				);
				REQUIRE(matcher().ranges[int_c< 0 >] == 10);
				REQUIRE(matcher().ranges[int_c< 1 >] == 5.0f);
				REQUIRE(matcher().ranges[int_c< 2 >] == 2.5);
			}
		}

		AND_WHEN("query matches the tag set of the case") {
			THEN("result of the generator function is returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature1{}, feature3{})) >
						==
					type_c< tag_matcher_query_result< tuple< float > > >
				);
				REQUIRE(matcher(feature1{}, feature3{}).ranges[int_c< 0 >] == 5.0f);
			}
		}

		AND_WHEN("query tags are a subset of tag sets of queries") {
			THEN("results of all the generator functions of matched cases are returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature1{})) >
						==
					type_c< tag_matcher_query_result< tuple< int, float > > >
				);
				REQUIRE(matcher(feature1{}).ranges[int_c< 0 >] == 10);
				REQUIRE(matcher(feature1{}).ranges[int_c< 1 >] == 5.0f);

				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature2{})) >
						==
					type_c< tag_matcher_query_result< tuple< double > > >
				);
				REQUIRE(matcher(feature2{}).ranges[int_c< 0 >] == 2.5);
			}
		}

		AND_WHEN("queries do not match any case tag set") {
			THEN("and empty result is returned") {
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature1{}, feature2{})) >
						==
					type_c< tag_matcher_query_result< tuple<> > >
				);
				BOOST_HANA_CONSTANT_CHECK(
					type_c< decltype(matcher(feature1{}, feature2{}, feature3{})) >
						==
					type_c< tag_matcher_query_result< tuple<> > >
				);
			}
		}
	}

}

SCENARIO("definition of TagMatcherQueryResult concept") {
	std::vector< int > v = { 10, 5, 2 };
	auto t = v | ranges::view::transform([] (auto&& v) { return v * 10; });

	static_assert(TagMatcherQueryResult< tuple<> >() == false);
	static_assert(TagMatcherQueryResult< tag_matcher_query_result< tuple<> > >() == true);
	static_assert(TagMatcherQueryResult< tag_matcher_query_result< tuple< decltype(t) > > >() == true);
}

SCENARIO("filtering entities from TagMatcherQueryResults") {

	GIVEN("TagMatcherQueryResult with two batches of entities, both providing feature1") {
		std::function< int() >	int1Resolver = [] { return 10; };
		std::function< int() >	int2Resolver = [] { return 20; };
		std::function< int() >	int3Resolver = [] { return 30; };
		std::function< double() > double1Resolver = [] { return 5.0; };
		std::function< double() > double2Resolver = [] { return 15.0; };

		using Entity1Type = decltype(
			entity{
				feature_provider{
					feature1_tag{},
					std::move(int1Resolver)
				}
			});
		std::vector< Entity1Type >	entity1Batch = {
			entity{
				feature_provider{
					feature1_tag{},
					std::move(int1Resolver)
				}
			}
		};

		using Entity2Type = decltype(
			entity{
				feature_provider{
					feature1_tag{},
					std::move(int2Resolver)
				},
				feature_provider{
					feature2_tag{},
					std::move(double1Resolver)
				}
			});
		std::vector< Entity2Type >	entity2Batch = {
			entity{
				feature_provider{
					feature1_tag{},
					std::move(int2Resolver)
				},
				feature_provider{
					feature2_tag{},
					std::move(double1Resolver)
				}
			},
			entity{
				feature_provider{
					feature1_tag{},
					std::move(int3Resolver)
				},
				feature_provider{
					feature2_tag{},
					std::move(double2Resolver)
				}
			}
		};

		auto result = tag_matcher_query_result{
			tuple{
				entity1Batch,
				entity2Batch
			}
		};

		THEN("feature1 resolvers are filtered from the result") {
			int sum = 0;
			for(auto&& e: filter_entities(result, feature1{})) {
				sum += std::get<0>(e)();
			}
			REQUIRE(sum == 60);
		}
	}

}
