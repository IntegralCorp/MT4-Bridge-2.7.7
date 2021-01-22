#pragma once
#include <vector>
#include <ctype.h>
#define TRIM_SPACES " \t\r\n"

std::string trim_right(const std::string & s)
{
	if (s.length() == 0)
	{
		return "";
	}

	std::string d(s);
	std::string::size_type i(d.find_last_not_of(TRIM_SPACES));
	if (i == std::string::npos)
	{
		//LOG_DEBUG("'%s' yields npos", s.c_str());
		return "";
	}
	else
	{
		d = d.erase(d.find_last_not_of(TRIM_SPACES) + 1);
		//LOG_DEBUG("'%s' yields '%s'", s.c_str(), d.c_str());
		return d;
	}
}

std::string trim_left(const std::string & s)
{
	std::string d(s);
	d = d.erase(0, s.find_first_not_of(TRIM_SPACES));
	//LOG_DEBUG("'%s' yields '%s'", s.c_str(), d.c_str());
	return d;
}

std::string trim(const std::string & s)
{
	//string d (s); 
	return trim_left(trim_right(s));
}  // end of trim

std::vector<std::string> split(std::string src, const char *apDelimiter = ",")
{
	std::vector<std::string> strs;
	char *pContext;

	char *pCopy = _strdup(src.c_str());
	char * pch;
	pch = strtok_s(pCopy, apDelimiter, &pContext);
	while (pch != NULL)
	{
		std::string token = pch;
		strs.push_back(trim(token));
		pch = strtok_s(NULL, apDelimiter, &pContext);
	}

	free(pCopy);
	return strs;
}

char *eatWhiteSpace(char *pSrc)
{
	while (isspace(*pSrc))
	{
		pSrc++;
	}
	return pSrc;
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}