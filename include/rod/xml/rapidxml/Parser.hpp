#pragma once


#include <rapidxml.hpp>




namespace rod
{

	namespace xml
	{

		namespace rapidxml
		{

			template< typename Context >
			class Parser:
				public Contextual< Context >
			{
			private:

				using Document = ::rapidxml::xml_document< char >;


				std::unique_ptr< char[] >		charContent;
				std::unique_ptr< Document >		document = std::unique_ptr< Document >( new Document() );


			public:

				using Node = ::rapidxml::xml_node< char >;

				
				Node*
				getRootNode()
				{
					readXmlContent();
					document->template parse< 0 >( charContent.get() );

					return document->first_node();
				}


			private:

				void
				readXmlContent()
				{
					auto&			xmlContent = resolve< XmlContent >( this );
					std::string		stringContent;

					while( xmlContent.atEnd() == false )
					{
						stringContent += xmlContent.readLine();
					}

					charContent = std::unique_ptr< char[] >( new char[ stringContent.size() + 1 ] );
					std::memcpy( charContent.get(), stringContent.data(), stringContent.size() );
					charContent[ stringContent.size() ] = '\0';
				}

			};
			
		}
		
	}
	
}