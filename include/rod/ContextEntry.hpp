#pragma once


#include <functional>
#include <string>

#include <rod/Context.hpp>
#include <rod/configuration/Configuration.hpp>
#include <rod/holder/SingletonHolder.hpp>




namespace rod
{

	template< template< typename > class ContextualRoot >
	class ContextEntry
	{

	public:

		using RootContext = CreateContext::r;
		

		ContextEntry( const std::string& xmlConfigurationPath = "" ):
		  config( xmlConfigurationPath )
		{
			rootContext.setConfig( std::ref( this->config ) );
		}

		void
		enter()
		{
			using contextualRootContext = typename RootContext::template CreateChildContext< TypeList<> >::r;
			using boundContextualRoot = ContextualRoot< contextualRootContext >;

			boundContextualRoot		contextualRoot;
			contextualRoot.bindToParentContext( this->rootContext );
			contextualRoot.enter();
		}


		configuration::Configuration<>	config;
		RootContext						rootContext;
	
	};
	
}