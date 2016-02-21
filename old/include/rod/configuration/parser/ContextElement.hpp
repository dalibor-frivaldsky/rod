#pragma once


#include <rod/Contextual.hpp>
#include <rod/TString.hpp>
#include <rod/configuration/annotation/ContextConfiguration.hpp>
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

				template< typename Configuration >
				struct GenerateChildElement
				{
					using r = xml::annotation::ChildElement<
									Configuration::XmlConfigurationElement::template XmlElement >;
				};

				template< typename Configurations >
				struct GenerateElement;

				template< typename... Configuration >
				struct GenerateElement< TypeList< Configuration... > >
				{
					using r = xml::annotation::Element
					<
						ROD_TString( "context" ),
						xml::annotation::Attributes<>,
						common::NullType,
						typename GenerateChildElement< Configuration >::r...
					>;
				};


			public:

				using RootElement = xml::annotation::RootElement;


				using Element =
						typename GenerateElement<
								typename This::template FindAnnotated< annotation::IsContextConfiguration >::r >::r;

			};
			
		}
		
	}
	
}