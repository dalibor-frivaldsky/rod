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


				using Element = xml::annotation::Element
				<
					ROD_TString( "context" ),
					xml::annotation::Attributes<>,
					common::NullType,
					xml::annotation::ChildElement
					<
						ComponentElement
					>,
					xml::annotation::ChildElement
					<
						InterfaceElement
					>
				>;

			};
			
		}
		
	}
	
}