#pragma once


/*#include <functional>
#include <tuple>
#include <utility>

#include <rod/Context.hpp>
#include <rod/Contextual.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/configuration/ConfigurationReader.hpp>*/


#include <string>

#include <rod/Context.hpp>
#include <rod/Contextual.hpp>
#include <rod/configuration/ConfigurationReader.hpp>




namespace rod
{

	template< typename Context >
	class Rod:
		public Contextual< Context >
	{
	private:

		/*template< template< typename > class Domain, typename InitialObjectsTuple, int... Seq >
		void
		enterDomainSequential( InitialObjectsTuple&& initialObjects, common::Sequence< Seq... >&& )
		{
			create< Domain >( this, std::get< Seq >( std::move( initialObjects ) )... )->enter();
		}

		template< template< typename > class Domain, typename... InitialObject >
		void
		enterDomain( std::tuple< InitialObject... >&& initialObjects )
		{
			using seq = typename common::GenerateSequence< sizeof...( InitialObject ) >::r;
			enterDomainSequential< Domain >( std::move( initialObjects ), seq() );
		}*/


	public:

		ROD_Contextual_Constructor( Rod )


		template< template< typename > class Domain, typename... ToInject >
		void
		enter( ToInject&&... toInject )
		{
			//enterDomain< Domain >( create< configuration::ConfigurationReader >( this )->read( configurationFilePath ) );
			::rod::create< configuration::ConfigurationReader >( this ).read( "" );
			::rod::create< Domain >( this, std::forward< ToInject >( toInject )... ).enter();
		}
	};


	template< template< typename > class Domain, typename... ToInject >
	void
	enter( ToInject&&... toInject )
	{
		using initialContext = CreateInitialContext<>::r;
		using boundRod = Rod< initialContext >;

		auto		nullCtx = createNullContext();
		boundRod	r( nullCtx );
		r.enter< Domain >( std::forward< ToInject >( toInject )... );
	}

	template< template< typename > class Domain >
	void
	enterPlain()
	{
		using initialContext = CreateInitialContext<>::r;
		using boundDomain = Domain< initialContext >;

		auto			nullCtx = createNullContext();
		boundDomain		domain( nullCtx );
		domain.enter();
	}
	
}