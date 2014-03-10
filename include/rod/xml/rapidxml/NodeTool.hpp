#pragma once


#include <rod/xml/XmlNodeType.hpp>
#include <rod/xml/rapidxml/NodeIterator.hpp>

#include <rapidxml.hpp>




namespace rod
{

	namespace xml
	{

		namespace rapidxml
		{

			class NodeTool
			{
			public:

				using Node = ::rapidxml::xml_node< char >;


			private:

				Node*	node;


			public:

				inline
				NodeTool( Node* node ):
					node( node )
				{}


				inline
				std::string
				getAttributeValue( const std::string& attrName )
				{
					return node->first_attribute( attrName.c_str() )->value();
				}

				inline
				bool
				isValid()
				{
					return node != nullptr;
				}

				inline
				XmlNodeType
				getNodeType()
				{
					switch( node->type() )
					{
						case ::rapidxml::node_element:
							return XmlNodeType::ElementNode;

						case ::rapidxml::node_data:
							return XmlNodeType::DataNode;

						default:
							return XmlNodeType::UnsupportedXmlNodeType;
					}
				}

				inline
				std::string
				getNodeName()
				{
					return node->name();
				}

				inline
				std::string
				getNodeValue()
				{
					return node->value();
				}

				inline
				NodeIterator
				getChildrenIterator()
				{
					return NodeIterator( node->first_node() );
				}

			};
			
		}
		
	}
	
}