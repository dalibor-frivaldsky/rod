#pragma once


#include <string>




namespace rod
{

	namespace configuration
	{

		class ComponentConfig
		{

		public:

			std::string		cls;
			std::string		id;


			ComponentConfig( const std::string& cls, const std::string& id ):
			  cls( cls ),
			  id( id )
			{}

		};
		
	}
	
}