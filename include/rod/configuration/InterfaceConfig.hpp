#pragma once


#include <string>




namespace rod
{

	namespace configuration
	{

		class InterfaceConfig
		{

		public:

			std::string		cls;
			std::string		providedById;


			InterfaceConfig( const std::string& cls, const std::string& providedById ):
			  cls( cls ),
			  providedById( providedById )
			{}

		};
		
	}
	
}