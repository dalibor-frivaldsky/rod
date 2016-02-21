#pragma once


#include <type_traits>

#include <rod/TypeList.hpp>




namespace rod {
namespace trait
{

	template< typename Type, typename Requirements >
	struct IsContextMovable;

	template< typename Type, typename... Requirement >
	struct IsContextMovable< Type, TypeList< Requirement... > >
	{
		enum { r = std::is_constructible< Type, Type&&, Requirement... >::value };
	};

	template<
		template< typename > class CustomContext,
		typename Context,
		typename... Requirement >
	struct IsContextMovable< CustomContext< Context >, TypeList< Requirement... > >
	{
		enum { r = std::is_constructible<
						CustomContext< Context >,
						CustomContext< Context >&&,
						Context&,
						Requirement... >::value };
	};
	
}}