#include <boost/hana.hpp>
#include <catch.hpp>
#include <functional>
#include <rod/resolve_framework/concept/FeatureQueryOperator>
#include <rod/resolve_framework/feature_query/instance_of>

using namespace rod::resolve_framework;


SCENARIO( "instance_of_impl is a FeatureQueryOperator", "[unit][resolve]" ) {
	static_assert( FeatureQueryOperator< detail::instance_of_impl< int > >() );
}


int intResolverFn() { return 0; }

int intNonResolverFn( int a ) { return a; };

struct IntResolverFnStruct {
	int operator () () { return 0; }
};

auto intResolverLambda = [] { return 0; };

SCENARIO( "instance_of operator evaluates TypeRepC-ed zero-parameter function types with return value", "[unit][resolve]" ) {

	GIVEN( "A function") {
		auto resolverType = boost::hana::type_c< decltype(&intResolverFn) >;
		
		THEN( "it is evaluated" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(resolverType) == boost::hana::true_c );
		}
	}

	GIVEN( "A struct with operator() overload") {
		auto resolverType = boost::hana::type_c< IntResolverFnStruct >;
		
		THEN( "it is evaluated" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(resolverType) == boost::hana::true_c );
		}
	}

	GIVEN( "A lamba") {
		auto resolverType = boost::hana::type_c< decltype(intResolverLambda) >;
		
		THEN( "it is evaluated" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(resolverType) == boost::hana::true_c );
		}
	}

	GIVEN( "A non-zero parameter function") {
		auto nonResolverType = boost::hana::type_c< decltype(&intNonResolverFn) >;
		
		THEN( "it is not evaluated" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(nonResolverType) == boost::hana::false_c );
		}
	}

	GIVEN( "A non-fuction type") {
		auto nonResolverType = boost::hana::type_c< int >;
		
		THEN( "it is not evaluated" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(nonResolverType) == boost::hana::false_c );
		}
	}

}

SCENARIO( "instance_of operator evaluation of primitive/non-derived types", "[unit][resolve]" ) {

	GIVEN( "A non-cv qualified value return type" ) {
		auto value = [] { return 0; };
		auto resolverType = boost::hana::type_c< decltype(value) >;

		THEN( "only value type is accepted" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(resolverType) == boost::hana::true_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< int& >(resolverType) == boost::hana::false_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< int* >(resolverType) == boost::hana::false_c );
		}
	}

	GIVEN( "A non-cv qualified reference return type" ) {
		int a = 0;
		auto value = [&] () -> int& { return a; };
		auto resolverType = boost::hana::type_c< decltype(value) >;

		THEN( "only reference type is accepted" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(resolverType) == boost::hana::false_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< int& >(resolverType) == boost::hana::true_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< int* >(resolverType) == boost::hana::false_c );
		}
	}

	GIVEN( "A non-cv qualified pointer return type" ) {
		int a = 0;
		auto value = [&] { return &a; };
		auto resolverType = boost::hana::type_c< decltype(value) >;

		THEN( "only pointer type is accepted" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< int >(resolverType) == boost::hana::false_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< int& >(resolverType) == boost::hana::false_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< int* >(resolverType) == boost::hana::true_c );
		}
	}

}


struct Foo {};

struct Bar: public Foo {};

SCENARIO( "instance_of operator evaluation of type hierarchies", "[unit][resolve]" ) {

	GIVEN( "A non-cv qualified value derived object return type" ) {
		auto value = [] { return Bar{}; };
		auto resolverType = boost::hana::type_c< decltype(value) >;

		THEN( "only exact value type is accepted" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< Foo >(resolverType) == boost::hana::false_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< Bar >(resolverType) == boost::hana::true_c );
		}
	}

	GIVEN( "A non-cv qualified reference derived object return type" ) {
		Bar bar{};
		auto value = [&] ()->Bar& { return bar; };
		auto resolverType = boost::hana::type_c< decltype(value) >;

		THEN( "base and derived reference types are accepted" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< Foo& >(resolverType) == boost::hana::true_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< Bar& >(resolverType) == boost::hana::true_c );
		}
	}

	GIVEN( "A non-cv qualified pointer derived object return type" ) {
		Bar bar{};
		auto value = [&] { return &bar; };
		auto resolverType = boost::hana::type_c< decltype(value) >;

		THEN( "base and exact pointer types are accepted" ) {
			BOOST_HANA_CONSTANT_CHECK( instance_of< Foo* >(resolverType) == boost::hana::true_c );
			BOOST_HANA_CONSTANT_CHECK( instance_of< Bar* >(resolverType) == boost::hana::true_c );
		}
	}

}


SCENARIO( "instance_of operator evaluates TypeRepR-ed std::function wrappers of resolvers", "[unit][resolve]" ) {

	GIVEN( "std::type_index of and std::function wrapper of zero-parameter non-void return function object" ) {
		auto iFnIndex = std::type_index{ typeid(std::function< int() >) };

		THEN( "it is accepted" ) {
			REQUIRE( instance_of< int >( iFnIndex ) == query_result< int >{ true } );
		}
	}

	GIVEN( "std::typeindex of non std::function wrapper" ) {
		auto iIndex = std::type_index{ typeid(int) };

		THEN( "it is not accepted" ) {
			REQUIRE( instance_of< int >( iIndex ) == query_result< int >{ false } );
		}
	}

}
