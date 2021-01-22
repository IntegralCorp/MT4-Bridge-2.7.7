#pragma once
#include <functional>

class tee
{
public:
	tee() { }

	void p(const char* text)
	{
		//NetworkMon::Log(text);
		//LOG_INFO(pluginLog, text);
		logFunc(std::string(text));
		//std::cout << text << std::endl;
	}

	void pf(const char* format, ...)
	{
		char buffer[256];
		va_list args;
		va_start(args, format);
		//vsprintf(buffer, format, args);
		vsprintf_s(buffer, 256, format, args);

		va_end(args);

		this->p(buffer);
	}

	void enable(bool onoff)
	{
		// Nada
	}

	std::function<void(std::string)> logFunc;
	void setLog(const std::function<void(std::string)>& _func)
	{
		logFunc = _func;
	}
};