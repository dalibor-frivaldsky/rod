#pragma once


#include <rod/TypeList.hpp>




namespace rod
{

	namespace xml
	{

		namespace annotation
		{

			template< typename Name_, typename Attributes_, typename ProcessMethod_, typename... ChildElement_ >
			struct Element
			{
				using Name = Name_;
				using Attributes = Attributes_;
				using ProcessMethod = ProcessMethod_;
				using ChildElements = TypeList< ChildElement_... >;
			};
			
		}
		
	}
	
}