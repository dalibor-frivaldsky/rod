#pragma once


#include <rod/configuration/Interfaces.hpp>
#include <rod/configuration/annotation/ContextConfiguration.hpp>

#if !defined( ROD_NO_XML_CONFIGURATION )
#include <rod/configuration/annotation/XmlConfigurationElement.hpp>
#include <rod/configuration/parser/InterfaceElement.hpp>
#endif




namespace rod
{

	namespace configuration
	{

		struct InterfacesConfiguration
		{
			using ContextConfiguration = annotation::ContextConfiguration;

		#if !defined( ROD_NO_XML_CONFIGURATION )
			using XmlConfigurationElement = annotation::XmlConfigurationElement< parser::InterfaceElement >;
		#endif

			using Configuration = Interfaces;
		};
		
	}
	
}