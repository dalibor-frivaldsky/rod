#pragma once


#include <utility>

#include <rod/TypeList.hpp>
#include <rod/annotation/Requires.hpp>
#include <rod/experimental/construct/TypeConstructor.hpp>




namespace rod {
namespace experimental {
namespace construct {

	namespace detail
	{

		template< typename Type >
		struct DefineTypeConstructor
		{
		private:
			using Requirements = typename annotation::GetRequirements< Type >::r;


			template< typename Layout >
			struct FromLayout;

			template< typename... Layout >
			struct FromLayout< TypeList< Layout... > >
			{
				using r = TypeConstructor< Layout... >;
			};


		public:
			using r = typename FromLayout< Requirements >::r;
		};
		
	}


	template< typename Type >
	union TypeCreator
	{
	private:
		using Constructor = typename detail::DefineTypeConstructor< Type >::r;


	public:
		Type			object;
		Constructor		objectConstructor;

		template< typename... Arg >
		TypeCreator( Arg&&... arg )
		{
			new (&objectConstructor) Constructor( std::forward< Arg >( arg )... );
		}

		~TypeCreator()
		{}

		operator Type&& ()
		{
			return std::move( object );
		}
	};
	
}}}