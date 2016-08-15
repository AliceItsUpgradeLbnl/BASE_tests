#ifndef __Options__HH
#define __Options__HH

#ifndef __CINT__

#include <string>
#include <vector>

class Options
{
public:
	Options(int argc, char *argv[]);
	~Options();
	bool 						IsSet(const char *opt);
	std::string 				ArgWith(const char *opt);
	std::vector<std::string> 	ArgsWith(const char *opt);
private:
	std::vector<std::string> fOptions;
};

#endif

#endif // __Options__HH
