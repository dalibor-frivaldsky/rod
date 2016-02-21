#pragma once




namespace rod
{

	namespace common
	{

		template< int V >
		struct IntToType
		{
			enum { v = V };
		};
		
	}
	
}