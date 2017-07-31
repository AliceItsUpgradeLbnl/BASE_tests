#include "TextFileMonitor.h"

using namespace std;

TextFileMonitor::TextFileMonitor(const char *path)
	: FileMonitor(path)
	, fStream(path)
	, fContent()
	, fUpdate()
{
	;
}

TextFileMonitor::TextFileMonitor(const char *path, const char *option)
	: FileMonitor(path, option)
	, fStream(path)
	, fContent()
	, fUpdate()
{
	;
}

TextFileMonitor::~TextFileMonitor()
{
	fStream.close();
}

void TextFileMonitor::Update()
{
	for( std::string line; getline( fStream, line ); )
	{
		fContent.push_back(line);
		fUpdate.push_back(line);
	}	
}

std::vector<string> TextFileMonitor::GetUpdate()
{
	std::vector<string> ret = fUpdate;
	fUpdate.clear();
	return ret;
}

