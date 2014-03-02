#pragma once




namespace rod
{

	namespace xml
	{

		namespace annotation
		{

			template< template< typename > class Element_, typename ProcessMethod_ >
			struct ChildElement
			{
				template< typename T >
				using Element = Element_< T >;

				using ProcessMethod = ProcessMethod_;
			};

		}
		
	}
	
}