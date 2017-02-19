// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <catch.hpp>
#include <rod/source/build/entity>

using namespace boost::hana;
using namespace rod::source::build;


struct feature1_tag {};
struct feature2_tag {};


SCENARIO("creating entities from feature providers") {

	GIVEN("entity with one feature provider") {
		auto e = entity{
			feature_provider{
				feature1_tag{},
				[] { return 10; }
			}
		};

		THEN("the feature's resolver can be obtained") {
			REQUIRE(e(feature1_tag{})() == 10);
		}
	}

	GIVEN("entity with two feature providers") {
		auto e = entity{
			feature_provider{
				feature1_tag{},
				[] { return 10; }
			},
			feature_provider{
				feature2_tag{},
				[] { return 5.0; }
			}
		};
		
		THEN("both the feature's resolvers can be obtained") {
			REQUIRE(e(feature1_tag{})() == 10);
			REQUIRE(e(feature2_tag{})() == 5.0);
		}
	}
	

}
