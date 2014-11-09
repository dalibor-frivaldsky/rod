#pragma once


#include <rod/experimental/construct/MemberConstructor.hpp>




namespace rod {
namespace experimental {
namespace construct {

	template< typename... Layout >
	struct TypeConstructor;

	template<>
	struct TypeConstructor<>
	{};

	template< typename Member, typename... Layout >
	struct TypeConstructor< Member, Layout... >:
		public MemberConstructor< Member >,
		public TypeConstructor< Layout... >
	{
		TypeConstructor( Member&& value, Layout&&... layoutValue ):
		  MemberConstructor< Member >( std::forward< Member >( value ) ),
		  TypeConstructor< Layout... >( std::forward< Layout >( layoutValue )... )
		{}
	};
	
}}}