#pragma once


#include <functional>
#include <type_traits>
#include <utility>

#include <rod/TypeList.hpp>
#include <rod/holder/InjectedValue.hpp>




namespace rod
{

	namespace holder
	{

		template< typename Type, typename Constructibility = void >
		struct ObjectOwner;

		template< typename Type >
		struct ObjectOwner<
				Type,
				typename std::enable_if<
					std::is_constructible< Type, const Type& >::value >::type >
		{

		private:
			Type	object;


		public:

			using Dependencies = TypeList< InjectedValue< Type > >;

			
			ObjectOwner( std::function< InjectedValue< Type >() >& ref ):
			  object( std::move( ref().object ) )
			{}

			ObjectOwner( const ObjectOwner< Type >& other ):
			  object( other.object )
			{}

			ObjectOwner( ObjectOwner< Type >&& other ):
			  object( std::move( other.object ) )
			{}

			ObjectOwner< Type >&
			operator = ( const ObjectOwner< Type >& other )
			{
				this->object = other.object;

				return *this;
			}

			ObjectOwner< Type >&
			operator = ( ObjectOwner< Type >&& other )
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

		template< typename Type, typename NotCopyConstructible >
		struct ObjectOwner
		{

		private:
			Type	object;


		public:

			using Dependencies = TypeList< InjectedValue< Type > >;

			
			ObjectOwner( std::function< InjectedValue< Type >() >& ref ):
			  object( std::move( ref().object ) )
			{}

			ObjectOwner( const ObjectOwner< Type >& ) = delete;
			ObjectOwner( ObjectOwner< Type >&& ) = delete;

			ObjectOwner< Type >& operator = ( const ObjectOwner< Type >& ) = delete;
			ObjectOwner< Type >& operator = ( ObjectOwner< Type >&& ) = delete;


			Type&
			get()
			{
				return object;
			}

		};
		
	}
	
}