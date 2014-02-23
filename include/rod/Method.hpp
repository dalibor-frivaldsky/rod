#pragma once


#include <utility>

#include <rod/TypeList.hpp>




namespace rod
{

	template< typename MethodType, MethodType >
	struct Method;

	template< typename Class_, typename Return_, typename... Args_, Return_ (Class_::*m)( Args_... ) >
	struct Method< Return_ (Class_::*)( Args_... ), m >
	{
		using Type = Return_ (Class_::*)( Args_... );
		using Class = Class_;
		using Return = Return_;
		using Args = TypeList< Args_... >;

		static
		Return
		call( Class& obj, Args_&&... args )
		{
			return (obj.*m)( std::forward< Args_ >( args )... );
		}
	};


	namespace method
	{

		template< typename Class, typename Return, typename... Args >
		auto
		deduceMethod( Return (Class::*m)( Args... ) ) -> Return (Class::*)( Args... )
		{}
		
	}
	
}


#define ROD_Method( m ) rod::Method< decltype( rod::method::deduceMethod( &m ) ), &m >
