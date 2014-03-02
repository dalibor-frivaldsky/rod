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
					std::cout << "InterfaceElement::processSelf(" << cls << ")" << std::endl;

					auto&	interfaces = resolve< Interfaces >( this );
					auto&	interface = interfaces.configure( Interface( cls ) );
					interface.providedById = "aaa";
					return interface;
				}

				void
				processProvidedBy( Interface& interface, std::string&& providedBy )
				{
					std::cout << interface.cls << " provided by " << providedBy << std::endl;
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