#pragma once


#include <fstream>
#include <utility>

#include <rod/xml/XmlContent.hpp>




namespace rod
{

	namespace xml
	{

		class FileXmlContent:
			public XmlContent
		{
		private:

			std::ifstream	fileInputStream;


		public:

			FileXmlContent( const std::string& filePath ):
			  fileInputStream( filePath )
			{}

			
			virtual
			bool
			atEnd()
			{
				return fileInputStream.eof();
			}

			virtual
			std::string
			readLine()
			{
				std::string		line;

				std::getline( fileInputStream, line );
				line += '\n';

				return line;
			}

		};
		
	}
	
}