#include "stdafx.h"
#include "HTTP.h"
#include <WinInet.h>
#include <tchar.h>
#include <atlstr.h>
//#include "..\Gateway.h"

HTTP::HTTP(std::string& Host, int Port, std::string& Path, int Login, std::string& Org, std::string& PrimeBroker, std::string& Broker, std::string& Stream)
{
	try
	{
		TCHAR frmdata[128], frmServer[128], frmPath[128];
		TCHAR hdrs[] = _T("Content-Type: application/x-www-form-urlencoded");
		LPCSTR accept[2] = { "*/*", NULL };

		std::string PostData =
			std::string("LE_ShortName=") + std::to_string(Login) +
			std::string("&LE_LongName=") + std::to_string(Login) +
			std::string("&LE_Org=") + Org +
			std::string("&BrokerOrg=") + Broker +
			std::string("&PrimeBroker=") + PrimeBroker +
			std::string("&Stream=") + Stream;

		LOG_INFO(pluginLog, "HTTP - %s:%d%s?%s", Host.c_str(), Port, Path.c_str(), PostData.c_str());

		_tcscpy_s(frmdata, CA2T(PostData.c_str()));
		_tcscpy_s(frmServer, CA2T(Host.c_str()));
		_tcscpy_s(frmPath, CA2T(Path.c_str()));

		HINTERNET hSession = InternetOpen("MT4.Bridge", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hSession == NULL)
		{
			this->hasFailed = true;
			LOG_ERROR(pluginLog, "InternetOpen Failed");
			return;
		}

		HINTERNET hConnect = InternetConnect(hSession, frmServer, Port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
		if (hSession == NULL)
		{
			this->hasFailed = true;
			LOG_ERROR(pluginLog, "InternetConnect Failed (%s:%d)", Host.c_str(), Port);
			goto cleanup;
		}

		HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", frmPath, NULL, NULL, accept, 0, 1);
		if (hSession == NULL)
		{
			this->hasFailed = true;
			LOG_ERROR(pluginLog, "HttpOpenRequest Failed (%s)", Path.c_str());
			goto cleanup;
		}

		if (!HttpSendRequest(hRequest, hdrs, strlen(hdrs), frmdata, strlen(frmdata)))
		{
			this->hasFailed = true;
			LOG_ERROR(pluginLog, "HttpSendRequest Failed (%s)", PostData.c_str());
			goto cleanup;
		}

		CHAR szBuffer[1025] = "";
		DWORD dwRead = 0;

		try
		{
			while (::InternetReadFile(hRequest, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
			{
				szBuffer[dwRead] = 0;
				//OutputDebugStringA(szBuffer);
				LOG_DEBUG(pluginLog, "AdminIntegration: %s", szBuffer);
				break;// dwRead = 0;
			}
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(pluginLog, e.what());
		}

		// If we are here, it succeeded.
		//hasFailed = false;
		hasFailed = (dwRead != 47);

		//HttpQueryInfo -- https://msdn.microsoft.com/en-us/library/windows/desktop/aa385373(v=vs.85).aspx

	cleanup:

		if (hSession != NULL)
			::InternetCloseHandle(hSession);

		if (hConnect != NULL)
			::InternetCloseHandle(hConnect);

		if (hRequest != NULL)
			::InternetCloseHandle(hRequest);

		LOG_INFO(pluginLog, "AdminIntegration '%d', Result: %s", Login, hasFailed ? "Failed" : "Succeeded");
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(pluginLog, e.what());
	}
}


HTTP::~HTTP()
{
	// Nothing needed
}

bool HTTP::Succeeded()
{
	return !this->hasFailed;
}
