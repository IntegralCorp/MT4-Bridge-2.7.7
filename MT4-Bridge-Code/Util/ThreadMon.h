#pragma once
#include "..\IO\Log.h"

class CThreadMon
{
public:
	CThreadMon()
	{

		//LOG_DEBUG(pluginLog, "Start");
	}

	~CThreadMon()
	{
		//LOG_INFO(pluginLog, "Destroy");
	}
};