#pragma once


#include <functional>
#include <string>
#include <tuple>
#include <utility>

#include <rod/Context.hpp>
#include <rod/Contextual.hpp>
#include <rod/configuration/ConfigurationReader.hpp>




namespace rod
{

	template< typename Context >
	class Rod:
		public Contextual< Context >
	{
	public:

		template< template< typename > class Domain >
		void
		enter( const std::string& configurationFilePath )
		{
			auto	configurations = create< configuration::ConfigurationReader >( this )->read( configurationFilePath );
			create< Domain >( this, std::move( std::get< 0 >( configurations ) ), std::move( std::get< 1 >( configurations ) ) )->enter();
		}
	};


	template< template< typename > class Domain >
	void
	enter( const std::string& configurationFilePath )
	{
		using rootContext = CreateContext::r;
		using rodContext = typename rootContext::template CreateChildContext< TypeList<> >::r;
		using boundRodContextual = Rod< rodContext >;

		rootContext			rootContextInstance;
		boundRodContextual	rodContextual;

		rodContextual.bindToParentContext( rootContextInstance );
		rodContextual.enter< Domain >( configurationFilePath );
	}
	
}