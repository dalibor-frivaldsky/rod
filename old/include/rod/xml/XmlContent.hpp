#pragma once




namespace rod
{

	namespace xml
	{

		class XmlContent
		{
		public:

			virtual bool			atEnd() = 0;
			virtual std::string		readLine() = 0;
		};
		
	}
	
}