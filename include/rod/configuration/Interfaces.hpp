#pragma once


#include <map>
#include <string>
#include <utility>

#include <rod/configuration/Interface.hpp>




namespace rod
{

	namespace configuration
	{

		class Interfaces
		{
		private:

			using InterfaceMap = std::map< std::string, Interface >;

			InterfaceMap	interfaceMap;


		public:

			template< typename InterfaceType >
			Interface&
			configure( InterfaceType&& interface )
			{
				return interfaceMap.emplace( interface.cls, std::move( interface ) ).first->second;
			}

			inline
			bool
			isConfigured( const std::string& className ) const
			{
				return interfaceMap.find( className ) != interfaceMap.end();
			}

			const Interface&
			getInterfaceConfiguration( const std::string& className ) const
			{
				return interfaceMap.find( className )->second;
			}

		};
		
	}
	
}