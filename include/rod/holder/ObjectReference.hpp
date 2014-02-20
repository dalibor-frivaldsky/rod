#pragma once




namespace rod
{

	namespace holder
	{

		template< typename Type >
		struct ObjectReference
		{

			Type*	object = nullptr;

		public:

			void
			setupFrom( Type& object )
			{
				this->object = &object;
			}

			Type*
			get()
			{
				return object;
			}

		};
		
	}
	
}