#pragma once




namespace rod
{

	namespace common
	{

		template< typename FirstType, typename SecondType >
		struct Pair
		{
			using First = FirstType;
			using Second = SecondType;
		};
		
	}

}