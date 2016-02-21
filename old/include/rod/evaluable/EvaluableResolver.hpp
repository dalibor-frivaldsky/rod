#pragma once


#include <rod/Evaluable.hpp>
#include <rod/annotation/Resolver.hpp>




namespace rod
{

	namespace evaluable
	{

		namespace detail
		{

			template< typename Type >
			struct EvaluableSelector;

			template< typename EvaluableCallback >
			struct EvaluableSelector< Evaluable< EvaluableCallback > >
			{
				enum { r = true };
			};

			template< typename Other >
			struct EvaluableSelector
			{
				enum { r = false };
			};
			
		}

		struct EvaluableResolver
		{
			using Resolver = annotation::Resolver< detail::EvaluableSelector >;

			template< typename EvaluableType, typename Context >
			static
			EvaluableType
			resolve( Context& context )
			{
				return EvaluableType( context );
			}
		};
		
	}
	
}