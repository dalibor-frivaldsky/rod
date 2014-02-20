#pragma once




namespace rod
{

	struct generate
	{
		template< typename... T >
		generate( T&&... )
		{}
	};
	
}