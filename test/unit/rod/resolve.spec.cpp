// Copyright Dalibor Frivaldsky 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <boost/hana.hpp>
#include <catch.hpp>
#include <rod/resolve>

using namespace boost::hana;
using namespace rod;


namespace test {

	struct Foo {};

	auto query_entities(Foo foo) {
		return int_c< 10 >;
	}

	auto filter_entities(Foo foo) {
		return int_c< 20 >;
	}

}

SCENARIO("rod::resolve calls query_entities function from Source type's namespace") {
	BOOST_HANA_CONSTANT_CHECK(
		query_entities(test::Foo{}) == int_c< 10 >
	);
}

SCENARIO("rod::resolve calls filter_entities function from Source type's namespace") {
	BOOST_HANA_CONSTANT_CHECK(
		filter_entities(test::Foo{}) == int_c< 20 >
	);
}


namespace test {

	struct Bar {};
	struct Baz {};
	struct Bazz {};

	auto query_entities(Bar) {
		return Baz{};
	}

	auto filter_entities(Baz) {
		return Bazz{};
	}

}

SCENARIO("rod::resolve passed result of query_entities to filter_entities") {
	BOOST_HANA_CONSTANT_CHECK(
		type_c< decltype(resolve()(test::Bar{})) > == type_c< test::Bazz >
	);
}


namespace test {

	struct Add {};

	template< int v >
	struct AddResult {
		static constexpr int	value = v;
	};
	
	template< typename... Qs >
	constexpr auto query_entities(Add, Qs&&... qs) {
		return AddResult< decltype((int_c< 0 > + ... + qs))::value >{};
	}

	template< int result, typename... Qs >
	constexpr auto filter_entities(AddResult< result >&&, Qs&&... qs) {
		return (int_c< result > * ... * qs);
	}

}

SCENARIO("rod::resolve passes queries into query_entities and filter_entities") {
	BOOST_HANA_CONSTANT_CHECK(
		resolve(int_c< 2 >, int_c< 3 >)(test::Add{}) == int_c< 30 >
	);
}
