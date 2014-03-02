#pragma once


#include <string>

#include <rod/Contextual.hpp>
#include <rod/Method.hpp>
#include <rod/TString.hpp>
#include <rod/configuration/Interface.hpp>
#include <rod/configuration/Interfaces.hpp>
#include <rod/configuration/parser/ProvidedByElement.hpp>
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
			class InterfaceElement:
				public Contextual< Context >
			{
			private:

				using This = InterfaceElement< Context >;

			
			public:

				Interface&
				processSelf( std::string&& cls )
				{
					auto&	interfaces = resolve< Interfaces >( this );
					return interfaces.configure( Interface( std::move( cls ) ) );
				}

				void
				processProvidedBy( Interface& interface, std::string&& providedBy )
				{
					interface.providedById = providedBy;
				}

				using Element = xml::annotation::Element
				<
					ROD_TString( "interface" ),
					xml::annotation::Attributes
					<
						ROD_TString( "class" )
					>,
					ROD_Method( This::processSelf ),
					xml::annotation::ChildElement
					<
						ProvidedByElement,
						ROD_Method( This::processProvidedBy )
					>
				>;
			};
			
		}
		
	}
	
}