#pragma once


#include <rod/xml/annotation/XmlParserProvider.hpp>
#include <rod/xml/rapidxml/NodeTool.hpp>
#include <rod/xml/rapidxml/Parser.hpp>




namespace rod
{

	namespace xml
	{

		namespace rapidxml
		{

			struct RapidXml
			{
				using XmlParserProvider = annotation::XmlParserProvider<
					Parser,
					NodeTool
				>;
			};
			
		}
		
	}
	
}