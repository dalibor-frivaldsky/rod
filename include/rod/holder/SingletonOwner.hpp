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

			SingletonOwner( const SingletonOwner< Type >& ) = delete;

			SingletonOwner( SingletonOwner< Type >&& other ):
			  object( std::move( other.object ) )
			{}

			SingletonOwner< Type >& operator = ( const SingletonOwner< Type >& ) = delete;

			SingletonOwner< Type >&
			operator = ( SingletonOwner< Type >&& other )
			{
				this->object = std::move( other.object );

				return *this;
			}


			Type&
			get()
			{
				return object;
			}
		};
		
	}
	
}