#pragma once


#include <rod/configuration/Components.hpp>
#include <rod/configuration/annotation/ContextConfiguration.hpp>
#include <rod/configuration/annotation/XmlConfigurationElement.hpp>
#include <rod/configuration/parser/ComponentElement.hpp>




namespace rod
{

	namespace configuration
	{

		struct ComponentsConfiguration
		{
			using ContextConfiguration = annotation::ContextConfiguration;
			using XmlConfigurationElement = annotation::XmlConfigurationElement< parser::ComponentElement >;

			using Configuration = Components;
		};
		
	}
	
}