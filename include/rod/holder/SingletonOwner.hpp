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
		public:
			using Dependencies = typename annotation::GetConstructionArguments< Type >::r;


		private:
			Type		object;


		public:
			template< typename... Dependency >
			SingletonOwner( Dependency&&... dependency ):
			  object( std::forward< Dependency >( dependency )... )
			{}

			SingletonOwner( const SingletonOwner< Type >& ) = delete;
			SingletonOwner( SingletonOwner< Type >&& ) = delete;

			SingletonOwner< Type >& operator = ( const SingletonOwner< Type >& ) = delete;
			SingletonOwner< Type >& operator = ( SingletonOwner< Type >&&  ) = delete;


			Type&
			get()
			{
				return object;
			}
		};
		
	}
	
}