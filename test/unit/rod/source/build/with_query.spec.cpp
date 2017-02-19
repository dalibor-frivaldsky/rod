// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <catch.hpp>
#include <rod/source/build/with_query>

using namespace rod::source::build;


struct feature1_tag {};
struct feature2_tag {};
struct featurex_tag {};

struct feature1 {
	using tag = feature1_tag;
	auto operator() () {
		return 10;
	}
};

struct feature2 {
	using tag = feature2_tag;
	auto operator() () {
		return 5.0;
	}
};


SCENARIO("selecting a query from a set of queries") {

	GIVEN("a set of queries") {
		THEN("query can be selected by a feature tag") {
			bool called = false;
			REQUIRE(
				with_query< feature1_tag >(
					[&] (auto&& query) {
						called = true;
						return query();
					},
					[] {},
					feature1{}, feature2{}
				) == 10
			);

			REQUIRE(called == true);
		}

		THEN("default function is called when a query of a specified tag is not present") {
			bool called = false;
			REQUIRE(
				with_query< featurex_tag >(
					[] (auto&& query) {},
					[&] {
						called = true;
						return 2.5f;
					},
					feature1{}, feature2{}
				) == 2.5f
			);

			REQUIRE(called == true);
		}
	}

}
