#pragma once




namespace rod
{

	namespace common
	{
		template< int... N >
		struct Sequence;


		namespace sequence
		{
			template< typename Seq, int ToAppend >
			struct Append;

			template< int ToAppend, int... Seq >
			struct Append< Sequence< Seq... >, ToAppend >
			{
				using r = Sequence< Seq..., ToAppend >;
			};
		}


		template< int N, int... S >
		struct GenerateSequence:
			GenerateSequence< N - 1, N - 1, S... >
		{};

		template< int... S >
		struct GenerateSequence< 0, S... >
		{
			using r = Sequence< S... >;
		};


		template< int... N >
		struct Sequence
		{
		private:
			using This = Sequence< N... >;


		public:
			template< int ToAppend >
			using Append = sequence::Append< This, ToAppend >;
		};
		
	}
	
}