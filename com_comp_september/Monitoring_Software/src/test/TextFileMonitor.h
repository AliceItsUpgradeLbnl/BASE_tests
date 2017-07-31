#ifndef __TextFileMonitor_HH
#define __TextFileMonitor_HH

#include "FileMonitor.h"
#include <fstream>
#include <string>

class TextFileMonitor : public FileMonitor
{
public:
	TextFileMonitor(const char *path);
	TextFileMonitor(const char *path, const char *option);
	virtual ~TextFileMonitor();

	void Update();
	std::vector<std::string> GetUpdate();

private:
	std::ifstream 				fStream;
	std::vector<std::string> 	fContent;
	std::vector<std::string> 	fUpdate;

	ClassDef(TextFileMonitor, 0)
};

#endif // __TextFileMonitor_HH