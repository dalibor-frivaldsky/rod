#pragma once


#include <utility>

#include <rod/annotation/ConstructWith.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct SingletonOwner
		{

			using Dependencies = typename annotation::GetConstructionArguments< Type >::r;


			Type	object;

			template< typename... Arg >
			SingletonOwner( Arg&... arg ):
			  object( arg()... )
			{}

			SingletonOwner( SingletonOwner< Type >&& other ):
			  object( std::move( other.object ) )
			{}

			SingletonOwner( const SingletonOwner< Type >& ) = delete;


			Type&
			get()
			{
				return object;
			}
		};
		
	}
	
}