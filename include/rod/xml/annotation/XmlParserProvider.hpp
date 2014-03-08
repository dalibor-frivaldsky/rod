#pragma once


#include <rod/Annotation.hpp>




namespace rod
{

	namespace xml
	{

		namespace annotation
		{

			template< template< typename > class XmlParser_, typename NodeTool_ >
			struct XmlParserProvider
			{
				template< typename Context >
				using XmlParser = XmlParser_< Context >;

				using NodeTool = NodeTool_;
			};

			ROD_Annotation( XmlParserProvider )
			
		}
		
	}
	
}