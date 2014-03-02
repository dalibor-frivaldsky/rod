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

			inline
			Component() = default;

			template< typename StringType >
			Component( StringType&& cls, StringType&& id ):
			  cls( cls ),
			  id( id )
			{}

		};
		
	}
	
}