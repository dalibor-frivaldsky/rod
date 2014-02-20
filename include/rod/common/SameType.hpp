#pragma once




namespace rod
{

	namespace common
	{

		template< typename Left, typename Right >
		struct SameType
		{
			enum { r = false };
		};

		template< typename Same >
		struct SameType< Same, Same >
		{
			enum { r = true };
		};
		
	}
	
}