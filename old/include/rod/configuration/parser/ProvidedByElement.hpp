#pragma once


#include <string>

#include <rod/Contextual.hpp>
#include <rod/Method.hpp>
#include <rod/TString.hpp>
#include <rod/xml/annotation/Attributes.hpp>
#include <rod/xml/annotation/ChildElement.hpp>
#include <rod/xml/annotation/DataNode.hpp>
#include <rod/xml/annotation/Element.hpp>
#include <rod/xml/annotation/RootElement.hpp>




namespace rod
{

	namespace configuration
	{

		namespace parser
		{

			template< typename Context >
			class ProvidedByElement:
				public Contextual< Context >
			{
			private:

				using This = ProvidedByElement< Context >;

			
			public:

				std::string
				processSelf()
				{
					return std::string();
				}

				void
				processData( std::string& result, std::string&& providedBy )
				{
					result += providedBy;
				}

				using Element = xml::annotation::Element
				<
					ROD_TString( "provided-by" ),
					xml::annotation::Attributes<>,
					ROD_Method( This::processSelf ),
					xml::annotation::DataNode<
						ROD_Method( This::processData )
					>
				>;
			};
			
		}
		
	}
	
}