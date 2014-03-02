#pragma once


#include <rod/configuration/Interfaces.hpp>
#include <rod/configuration/annotation/ContextConfiguration.hpp>
#include <rod/configuration/annotation/XmlConfigurationElement.hpp>
#include <rod/configuration/parser/InterfaceElement.hpp>




namespace rod
{

	namespace configuration
	{

		struct InterfacesConfiguration
		{
			using ContextConfiguration = annotation::ContextConfiguration;
			using XmlConfigurationElement = annotation::XmlConfigurationElement< parser::InterfaceElement >;

			using Configuration = Interfaces;
		};
		
	}
	
}