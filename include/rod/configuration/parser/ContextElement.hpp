#pragma once


#include <rod/Contextual.hpp>
#include <rod/Method.hpp>
#include <rod/TString.hpp>
#include <rod/configuration/Component.hpp>
#include <rod/configuration/parser/ComponentElement.hpp>
#include <rod/configuration/parser/InterfaceElement.hpp>
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
			class ContextElement:
				public Contextual< Context >
			{
			private:

				using This = ContextElement< Context >;


			public:

				using RootElement = xml::annotation::RootElement;


				void
				processSelf()
				{
					std::cout << "ContextElement::processSelf" << std::endl;
				}

				void
				processComponent()
				{}

				void
				processInterface( Interface& )
				{}

				using Element = xml::annotation::Element
				<
					ROD_TString( "context" ),
					xml::annotation::Attributes<>,
					ROD_Method( This::processSelf ),
					xml::annotation::ChildElement
					<
						ComponentElement,
						ROD_Method( This::processComponent )
					>,
					xml::annotation::ChildElement
					<
						InterfaceElement,
						ROD_Method( This::processInterface )
					>
				>;

			};
			
		}
		
	}
	
}