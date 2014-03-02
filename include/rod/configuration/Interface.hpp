#pragma once


#include <string>




namespace rod
{

	namespace configuration
	{

		class Interface
		{

		public:

			std::string		cls;
			std::string		providedById;


			Interface( std::string&& cls ):
			  cls( cls )
			{}

		};
		
	}
	
}