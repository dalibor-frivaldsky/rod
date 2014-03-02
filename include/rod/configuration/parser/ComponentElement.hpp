#pragma once


#include <string>

#include <rod/Contextual.hpp>
#include <rod/Method.hpp>
#include <rod/TString.hpp>
#include <rod/configuration/Component.hpp>
#include <rod/configuration/Components.hpp>
#include <rod/xml/annotation/Attributes.hpp>
#include <rod/xml/annotation/ChildElement.hpp>
#include <rod/xml/annotation/Element.hpp>
#include <rod/xml/annotation/RootElement.hpp>




namespace rod
{

	namespace configuration
	{

		namespace parser
		{

			template< typename Context >
			class ComponentElement:
				public Contextual< Context >
			{
			private:

				using This = ComponentElement< Context >;


			public:

				void
				processSelf( std::string&& id, std::string&& cls )
				{
					auto&	components = resolve< Components >( this );
					components.configure( Component( std::move( cls ), std::move( id ) ) );
				}

				using Element = xml::annotation::Element
				<
					ROD_TString( "component" ),
					xml::annotation::Attributes
					<
						ROD_TString( "id" ),
						ROD_TString( "class" )
					>,
					ROD_Method( This::processSelf )
				>;
			};
			
		}
		
	}
	
}