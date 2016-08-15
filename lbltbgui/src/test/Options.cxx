#ifndef __CINT__

#include "Options.h"

using namespace std;

Options::Options(int argc, char *argv[])
	: fOptions()
{
	for (int i = 0; i < argc; i++)
	{
		string s(argv[i]);
		fOptions.push_back(s);
	}
}

Options::~Options()
{
	;
}

bool Options::IsSet(const char *opt)
{
	string s(opt);
	for (unsigned int i = 0; i < fOptions.size(); i++)
	{
		if (fOptions[i] == s)
		{
			return true;
		}
	}
	return false;
}

string Options::ArgWith(const char *opt)
{
	string s(opt);
	string sret = "";
	for (unsigned int i = 0; i < fOptions.size(); i++)
	{
		if (fOptions[i] == s)
		{
			if (i+1 < fOptions.size())
			{
				sret = fOptions[i+1];
			}
		}
	}
	return sret;
}

std::vector<string> Options::ArgsWith(const char *opt)
{
	string s(opt);
	std::vector<string> sret;
	for (unsigned int i = 0; i < fOptions.size(); i++)
	{
		if (fOptions[i] == s)
		{
			if (i+1 < fOptions.size())
			{
				if(fOptions[i+1].size() > 0)
					sret.push_back(fOptions[i+1]);
			}
		}
	}
	return sret;
}

#endif
