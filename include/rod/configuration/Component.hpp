#pragma once


#include <string>




namespace rod
{

	namespace configuration
	{

		class Component
		{

		public:

			std::string		cls;
			std::string		id;


		public:

			Component( std::string&& cls, std::string&& id ):
			  cls( cls ),
			  id( id )
			{}

		};
		
	}
	
}