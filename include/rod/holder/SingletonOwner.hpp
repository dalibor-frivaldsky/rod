#pragma once




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct SingletonOwner
		{
			Type	object;

			Type&
			get()
			{
				return object;
			}
		};


		template< typename Holder >
		struct SingletonSelector;

		template< typename Type >
		struct SingletonSelector< SingletonOwner< Type > >
		{
			enum { r = true };
		};

		template< typename Holder >
		struct SingletonSelector
		{
			enum { r = false };
		};
		
	}
	
}