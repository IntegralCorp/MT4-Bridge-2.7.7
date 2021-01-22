#include "stdafx.h"
#include "IdcLog.h"

namespace FIX
{
	Log* IdcLogFactory::create()
	{
		return new IdcLog(init("GLOBAL", "global", m_settings.get()));
	}

	Log* IdcLogFactory::create(const SessionID& sessionID)
	{
		Dictionary settings;
		if (m_settings.has(sessionID))
			settings = m_settings.get(sessionID);

		return new IdcLog(sessionID, init(sessionID.toString(), sessionID.getSenderCompID().getString(), settings));
	}

	CLog* IdcLogFactory::init(const std::string& oName, const std::string& oFile, const Dictionary& settings)
	{
		LOG_EVENT(m_log, "------------------------------------ Starting Event (%s) Logs ------------------------------------", oName.c_str());
		return m_log;
	}

	void IdcLogFactory::destroy(Log* pLog)
	{
		delete pLog;
	}
} //namespace FIX