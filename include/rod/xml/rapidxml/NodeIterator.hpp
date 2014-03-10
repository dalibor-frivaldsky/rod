#pragma once


#include <rapidxml.hpp>




namespace rod
{

	namespace xml
	{

		namespace rapidxml
		{

			class NodeIterator
			{
			public:

				using Node = ::rapidxml::xml_node< char >;


			private:

				Node*	node;


			public:

				inline
				NodeIterator( Node* node ):
					node( node )
				{}


				inline
				bool
				hasNext()
				{
					return node != nullptr;
				}

				inline
				Node*
				getNext()
				{
					Node*	toReturn = node;
					node = node->next_sibling();

					return toReturn;
				}

			};
			
		}
		
	}
	
}