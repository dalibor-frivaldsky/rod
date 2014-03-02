#pragma once


#include <string>
#include <tuple>

#include <rod/AsContextual.hpp>
#include <rod/Contextual.hpp>
#include <rod/configuration/Components.hpp>
#include <rod/configuration/Interfaces.hpp>
#include <rod/configuration/parser/ContextElement.hpp>
#include <rod/xml/FileXmlContent.hpp>
#include <rod/xml/XmlReader.hpp>




namespace rod
{

	namespace configuration
	{

		template< typename Context >
		class ConfigurationReader:
			public Contextual<
					Context,
					TypeList< AsContextual< parser::ContextElement > > >
		{
		public:

			std::tuple< Components, Interfaces >
			read( const std::string& configurationFilePath )
			{
				Components				components;
				Interfaces				interfaces;
				xml::FileXmlContent		configurationFileContent( configurationFilePath );

				this->template create< xml::XmlReader >( components, interfaces, configurationFileContent )->read();

				return std::make_tuple( std::move( components ), std::move( interfaces ) );
			}
		};
		
	}
	
}