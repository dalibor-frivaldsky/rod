#pragma once




namespace rod
{

	namespace configuration
	{

		namespace annotation
		{

			template< template< typename > class XmlElement_ >
			struct XmlConfigurationElement
			{
				template< typename Ctx >
				using XmlElement = XmlElement_< Ctx >;
			};
			
		}
		
	}
	
}