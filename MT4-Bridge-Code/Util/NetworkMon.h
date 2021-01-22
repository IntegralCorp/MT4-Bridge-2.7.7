/***********************************************************************
tcping.exe -- A tcp probe utility
Copyright (C) 2005-2016 Eli Fulkerson

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

----------------------------------------------------------------------

Other license terms may be negotiable.  Contact the author if you would
like a copy that is licensed differently.

Contact information (as well as this program) lives at http://www.elifulkerson.com

----------------------------------------------------------------------

This application includes public domain code from the Winsock Programmer's FAQ:
http://www.tangentsoft.net/wskfaq/
... and a big thank you to the maintainers and contributers therein.

***********************************************************************/
#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "..\TCPing\tee.h"
#include "..\TCPing\tcping.h"

class NetworkMon
{
private:
	std::atomic<int> wSock = 0;

public:
	NetworkMon(const std::string& oSession, const  std::string& oHost, const  int oPort)
	{
		try
		{
			std::thread([=]()
			{
				this->Profile(oSession, oHost, oPort);
			}).detach();
		}
		catch (const std::system_error& e)
		{
			LOG_ERROR(pluginLog, "Caught system_error with code generic: %d(%s) %s", e.code().value(), e.code().message().c_str(), e.what());
		}
	}

	void Profile(const std::string& oSession, const std::string& oHost, const int oPort)
	{
		static threadmap<std::string, bool> threadList;
		std::string thisKey = oSession + oHost + std::to_string(oPort);

		if (threadList.has(thisKey) && threadList[thisKey])
			return;

		//CThreadMon threadMon;
		threadList.set(thisKey, true);
		std::function<void()> cleanupAsync = [&]()
		{
			threadList.set(thisKey, false);
		};

		CAutoFree cFree(cleanupAsync);

		LOG_ERROR(pluginLog, "Session not connected %s; Profiling connectivity to '%s:%d'", oSession.c_str(), oHost.c_str(), oPort);

		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

		// Start Winsock up
		WSAData wsaData;
		int nCode;
		if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0)
		{
			LOG_ERROR(pluginLog, "WSAStartup() returned error code %d", nCode);
			return;
		}
		else
			this->wSock++;

		// Call the main example routine.
		tee out;
		std::vector<char> _Host(oHost.begin(), oHost.end());
		_Host.push_back('\0');

		const std::function<void(std::string)> logFunc = [&](std::string text)
		{
			LOG_INFO(pluginLog, text.c_str());
		};

		out.setLog(logFunc);

		int retval = DoWinsock_Single(&_Host[0], oPort, -1, 1, FALSE, TRUE, 2000, -1, TRUE, TRUE, FALSE, FALSE, NULL, NULL, TRUE, NULL, NULL, FALSE, 4, "", NULL, FALSE, NULL, FALSE, FALSE, FALSE, out, FALSE, NULL, false, FALSE, FALSE);

		// Shut Winsock back down and take off.
		if (this->wSock > 1)
		{
			int oRes = WSACleanup(); //Clean up Winsock
			this->wSock--;

			LOG_INFO(pluginLog, "WSACleanup: %d / %d", oRes, this->wSock);
		}
	}
};