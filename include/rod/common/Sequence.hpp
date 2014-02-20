#pragma once




namespace rod
{

	namespace common
	{

		template< int... N >
		struct Sequence
		{};
		

		template< int N, int... S >
		struct GenerateSequence:
			GenerateSequence< N - 1, N - 1, S... >
		{};

		template< int... S >
		struct GenerateSequence< 0, S... >
		{
			using r = Sequence< S... >;
		};
		
	}
	
}