#pragma once


#include <string>
#include <tuple>
#include <utility>

#include <rod/AsContextual.hpp>
#include <rod/Contextual.hpp>
#include <rod/common/Sequence.hpp>
#include <rod/configuration/ComponentsConfiguration.hpp>
#include <rod/configuration/InterfacesConfiguration.hpp>
#include <rod/configuration/annotation/ContextConfiguration.hpp>
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
					TypeList<
						AsContextual< parser::ContextElement >,
						ComponentsConfiguration,
						InterfacesConfiguration > >
		{
		private:

			using This = ConfigurationReader< Context >;


			template< typename... Configuration >
			static
			std::tuple< typename Configuration::Configuration... >
			defineConfigurationsTuple( TypeList< Configuration... >&& )
			{}

			using ConfigurationsTuple =
					decltype(
						defineConfigurationsTuple(
							typename This::template FindAnnotated< annotation::IsContextConfiguration >::r() ) );


			template< typename XmlContent, int... Seq >
			void
			readConfigurationSequential(
					XmlContent&& xmlContent,
					ConfigurationsTuple& configurations, common::Sequence< Seq... >&& )
			{
				create< xml::XmlReader >( this, xmlContent, std::get< Seq >( configurations )... )->read();
			}

			template< typename XmlContent >
			void
			readConfiguration( XmlContent&& xmlContent, ConfigurationsTuple& configurations )
			{
				using seq = typename common::GenerateSequence< std::tuple_size< ConfigurationsTuple >::value >::r;
				readConfigurationSequential(
						std::forward< XmlContent >( xmlContent ),
						configurations, seq() );
			}


		public:

			std::tuple< Components, Interfaces >
			read( const std::string& configurationFilePath )
			{
				ConfigurationsTuple		configurations;
				xml::FileXmlContent		configurationFileContent( configurationFilePath );

				readConfiguration( configurationFileContent, configurations );

				return std::move( configurations );
			}
		};
		
	}
	
}