#pragma once




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct SingletonOwner
		{
			Type*	object = nullptr;

			~SingletonOwner()
			{
				delete object;
			}

			Type*
			get() const
			{
				return object;
			}

			void
			setupFrom( Type* object )
			{
				this->object = object;
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