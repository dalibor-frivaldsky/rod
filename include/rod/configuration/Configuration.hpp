#pragma once


#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include <rapidxml.hpp>

#include <rod/configuration/ComponentConfig.hpp>
#include <rod/configuration/InterfaceConfig.hpp>




namespace rod
{

	namespace configuration
	{

		template< typename CharType = char >
		class Configuration
		{

		private:

			using XmlContent = std::unique_ptr< CharType[] >;
			using ComponentConfigMap = std::map< std::string, ComponentConfig >;
			using InterfaceConfigMap = std::map< std::string, InterfaceConfig >;

			using XmlDocument = rapidxml::xml_document< CharType >;
			using XmlNode = rapidxml::xml_node< CharType >;


			XmlContent				xmlConfigurationContent;
			ComponentConfigMap		componentConfigMap;
			InterfaceConfigMap		interfaceConfigMap;


		public:

			Configuration( const std::string& xmlConfigurationPath )
			{
				readXml( xmlConfigurationPath );
				readConfiguration();
			}


			bool
			componentIsConfigured( const std::string& className )
			{
				return componentConfigMap.find( className ) != componentConfigMap.end();
			}

			ComponentConfig&
			getComponentConfiguration( const std::string& className )
			{
				return componentConfigMap.find( className )->second;
			}

			bool
			interfaceIsConfigured( const std::string& className )
			{
				return interfaceConfigMap.find( className ) != interfaceConfigMap.end();
			}

			InterfaceConfig&
			getInterfaceConfiguration( const std::string& className )
			{
				return interfaceConfigMap.find( className )->second;
			}


		private:

			void
			readXml( const std::string& xmlFilePath )
			{
				if( xmlFilePath.empty() )
				{
					xmlConfigurationContent = XmlContent( new CharType[ 1 ] { '\0' } );
				}

				std::ifstream	fileStream( xmlFilePath );
				std::streamoff	fileSize;

				fileStream.seekg( 0, std::ios::end );
				fileSize = fileStream.tellg();

				if( fileSize == -1 )
				{
					xmlConfigurationContent = XmlContent( new CharType[ 11 ] { '<', 'c', 'o', 'n', 't', 'e', 'x', 't', '/', '>', '\0' } );
				}
				else
				{
					xmlConfigurationContent = XmlContent( new CharType[ (size_t) fileSize + 1 ] );
					fileStream.seekg( 0 );
					fileStream.read( xmlConfigurationContent.get(), fileSize );
					xmlConfigurationContent.get()[ (size_t) fileSize - 13 ] = '\0';
				}
			}

			void
			readConfiguration()
			{
				XmlDocument		doc;

				std::cout << xmlConfigurationContent.get() << std::endl;
				doc.template parse< 0 >( xmlConfigurationContent.get() );

				readContextConfig( doc );
			}

			void
			readContextConfig( XmlDocument& doc )
			{
				XmlNode*	contextNode = doc.first_node( "context" );

				if( contextNode != 0 )
				{
					readComponentConfig( contextNode );
					readInterfaceConfig( contextNode );
				}
			}

			void
			readComponentConfig( XmlNode* parentNode )
			{
				XmlNode*	componentNode = parentNode->first_node( "component" );

				while( componentNode != 0 )
				{
					std::string		cls = componentNode->first_attribute( "class" )->value();
					std::string		id = componentNode->first_attribute( "id" )->value();

					componentConfigMap.emplace( cls, ComponentConfig( cls, id ) );

					componentNode = componentNode->next_sibling( "component" );
				}
			}

			void
			readInterfaceConfig( XmlNode* parentNode )
			{
				XmlNode*	interfaceNode = parentNode->first_node( "interface" );

				while( interfaceNode != 0 )
				{
					std::string		cls = interfaceNode->first_attribute( "class" )->value();
					std::string		providedById = interfaceNode->first_node( "provided-by" )->value();

					interfaceConfigMap.emplace( cls, InterfaceConfig( cls, providedById ) );

					interfaceNode = interfaceNode->next_sibling( "interface" );
				}
			}

		};


		template< typename CharType = char >
		struct NullConfiguration
		{
			static
			Configuration< CharType >&
			value()
			{
				static Configuration< CharType >	nullConfiguration( "" );

				return nullConfiguration;
			}
		};
		
	}
	
}