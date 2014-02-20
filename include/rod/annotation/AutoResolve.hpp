#pragma once


#include <rod/TypeList.hpp>
#include <rod/common/MakeVoid.hpp>




namespace rod
{

	namespace annotation
	{

		template< typename... ToResolve >
		struct AutoResolve
		{
			using Types = TypeList< ToResolve... >;
		};


		template< typename NotAnnotated, typename = void >
		struct GetAutoResolves
		{
			using r = AutoResolve<>;
		};

		template< typename Annotated >
		struct GetAutoResolves< Annotated, typename common::MakeVoid< typename Annotated::AutoResolve >::r >
		{
			using r = typename Annotated::AutoResolve;
		};
		
	}
	
}