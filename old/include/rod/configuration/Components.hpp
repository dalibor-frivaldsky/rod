#pragma once


#include <map>
#include <string>
#include <utility>

#include <rod/configuration/Component.hpp>




namespace rod
{

	namespace configuration
	{

		class Components
		{
		private:

			using ComponentMap = std::map< std::string, Component >;

			ComponentMap	componentMap;


		public:

			template< typename ComponentType >
			void
			configure( ComponentType&& component )
			{
				componentMap.insert( component.cls, std::forward< ComponentType >( component ) );
			}

			inline
			bool
			isConfigured( const std::string& className ) const
			{
				return componentMap.find( className ) != componentMap.end();
			}

			inline
			const Component&
			getConfiguration( const std::string& className ) const
			{
				return componentMap.find( className )->second;
			}

		};
		
	}
	
}