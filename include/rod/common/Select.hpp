#pragma once




namespace rod
{

	namespace common
	{

		template< bool query, typename TrueResult, typename FalseResult >
		struct Select
		{
			using r = TrueResult;
		};

		template< typename TrueResult, typename FalseResult >
		struct Select< false, TrueResult, FalseResult >
		{
			using r = FalseResult;
		};
		
	}

}