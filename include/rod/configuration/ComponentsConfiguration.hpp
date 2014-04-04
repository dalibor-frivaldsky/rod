#pragma once


#include <rod/configuration/Components.hpp>
#include <rod/configuration/annotation/ContextConfiguration.hpp>

#if !defined( ROD_NO_XML_CONFIGURATION )
#include <rod/configuration/annotation/XmlConfigurationElement.hpp>
#include <rod/configuration/parser/ComponentElement.hpp>
#endif




namespace rod
{

	namespace configuration
	{

		struct ComponentsConfiguration
		{
			using ContextConfiguration = annotation::ContextConfiguration;

		#if !defined( ROD_NO_XML_CONFIGURATION )
			using XmlConfigurationElement = annotation::XmlConfigurationElement< parser::ComponentElement >;
		#endif

			using Configuration = Components;
		};
		
	}
	
}