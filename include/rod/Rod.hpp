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




namespace rod
{

	/*template< typename Context >
	class Rod:
		public Contextual< Context >
	{
	private:

		template< template< typename > class Domain, typename InitialObjectsTuple, int... Seq >
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
		}


	public:

		template< template< typename > class Domain >
		void
		enter( const std::string& configurationFilePath )
		{
			enterDomain< Domain >( create< configuration::ConfigurationReader >( this )->read( configurationFilePath ) );
		}
	};*/


	template< template< typename > class Domain >
	void
	enter( const std::string& configurationFilePath = "" )
	{
		/*using rootContext = CreateContext::r;
		using rodContext = typename rootContext::template CreateChildContext< TypeList<> >::r;
		using boundRodContextual = Rod< rodContext >;

		rootContext			rootContextInstance;
		boundRodContextual	rodContextual;

		rodContextual.bindToParentContext( rootContextInstance );
		rodContextual.enter< Domain >( configurationFilePath );*/
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