#include "stdafx.h"
#include "Reports.h"
#include <iostream>
#include <fstream>
//+------------------------------------------------------------------+
//| Global object                                                    |
//+------------------------------------------------------------------+
CReports ExtReports;
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CReports::CReports(void)
{
	
}

CReports::~CReports()
{

}

bool CReports::Initialize(void)
  {
  m_sync.Lock();
  //--- remember root directory
  m_directory.Format("%s\\%s",ExtProgramPath,"Reports");
  LOG_INFO(pluginLog,"checking directory %s",m_directory.Str());
  //--- check directory
  if(!CMTFile::DirectoryCreate(m_directory))
    {
    LOG_ERROR(pluginLog,"%s directory create error",m_directory.Str());
	 m_sync.Unlock();
    return(false);
    }
  //---
  m_sync.Unlock();
  return(true);
  }

void CReports::AccountBalance(time_t rawtime, LPCTSTR apFormat, ...)
{
	char buf[2048] = { 0 };
	StringTypePath file_name;

	va_list args;
	va_start(args, apFormat);
	vsprintf_s(buf, sizeof(buf) - 1, apFormat, args);
	va_end(args);

	struct tm * timeinfo;
	char buffer[80];

	////time(&rawtime);
	timeinfo = gmtime(&rawtime);

	strftime(buffer, 80, "\\Reports\\AccountBalances.%Y-%m-%d.csv", timeinfo);
	std::string timeString(buffer);
	file_name.Format("%s%s",ExtProgramPath,timeString.c_str()); 

	m_sync.Lock();
	{
		bool isFirstWrite = !std::ifstream(file_name.Str()).good();

		FILE* pFile = fopen(file_name.Str(), "a");

		if (pFile != NULL)
		{
			try
			{
				if (isFirstWrite)
				{
					//fputs("MT4\n", pFile);
					fputs("date, login, group, ccy, balance", pFile);
					fputs("\n", pFile);
				}

				// print to report file
				fprintf(pFile, "%s", buf);
				fputs("\n", pFile);
			}
			catch (...)
			{
				//break;
			}
		}

		if (pFile != NULL)
			fclose(pFile);
	}
	m_sync.Unlock();
}

void CReports::ClosingPrice(time_t rawtime, LPCTSTR apFormat, ...)
{
	char buf[2048] = { 0 };
	 StringTypePath file_name;

	va_list args;
	va_start(args, apFormat);
	vsprintf_s(buf, sizeof(buf) - 1, apFormat, args);
	va_end(args);

	struct tm * timeinfo;
	char buffer[80];

	////time(&rawtime);
	timeinfo = gmtime(&rawtime);

	strftime(buffer, 80, "\\Reports\\ClosingPrices.%Y-%m-%d.csv", timeinfo);
	std::string timeString(buffer);
	file_name.Format("%s%s",ExtProgramPath,timeString.c_str());

	m_sync.Lock();
	{
		bool isFirstWrite = !std::ifstream(file_name.Str()).good();

		FILE* pFile = fopen(file_name.Str(), "a");

		if (pFile != NULL)
		{
			try
			{
				if (isFirstWrite)
				{
					//fputs("MT4\n", pFile);
					fputs("date, symbol, bid, ask", pFile);
					fputs("\n", pFile);
				}

				// print to report file
				fprintf(pFile, "%s", buf);
				fputs("\n", pFile);
			}
			catch (...)
			{
				//break;
			}
		}

		if (pFile != NULL)
			fclose(pFile);
	}
	m_sync.Unlock();
}

void CReports::ExecutedTrade(time_t rawtime, LPCTSTR apFormat, ...)
{
	char buf[2048] = { 0 };
	StringTypePath file_name;

	va_list args;
	va_start(args, apFormat);
	vsprintf_s(buf, sizeof(buf) - 1, apFormat, args);
	va_end(args);

	struct tm * timeinfo;
	char buffer[80];

	////time(&rawtime);
	timeinfo = gmtime(&rawtime);

	strftime(buffer, 80, "\\Reports\\ExecutedTrades.%Y-%m-%d.csv", timeinfo);
	std::string timeString(buffer);

	file_name.Format("%s%s",ExtProgramPath,timeString.c_str());

	m_sync.Lock();
	{
		bool isFirstWrite = !std::ifstream(file_name.Str()).good();

		FILE* pFile = fopen(file_name.Str(), "a");

		if (pFile != NULL)
		{
			try
			{
				if (isFirstWrite)
				{
					//fputs("MT4\n", pFile);
					fputs("date of transaction, transaction id, type of transaction, client number, instrument, order type, buy - sell, order quantity, order price, stop loss, take profit, order date, order execution date, executed quantity, execution price, market bid price, market offer price, ip, leverage, commission, tax, swap, profit and loss, instrument pnl, spread profit and loss, conversion rate, lp price", pFile);
					fputs("\n", pFile);
				}

				// print to report file
				fprintf(pFile, "%s", buf);
				fputs("\n", pFile);
			}
			catch (...)
			{
				//break;
			}
		}

		if (pFile != NULL)
			fclose(pFile);
	}
	m_sync.Unlock();
}

void CReports::OpenPosition(time_t rawtime, LPCTSTR apFormat, ...)
{
	char buf[2048] = { 0 };
	StringTypePath file_name;
	va_list args;
	va_start(args, apFormat);
	vsprintf_s(buf, sizeof(buf) - 1, apFormat, args);
	va_end(args);

	struct tm * timeinfo;
	char buffer[80];

	////time(&rawtime);
	timeinfo = gmtime(&rawtime);

	strftime(buffer, 80, "\\Reports\\OpenPositions.%Y-%m-%d.csv", timeinfo);
	std::string timeString(buffer);

	file_name.Format("%s%s",ExtProgramPath,timeString.c_str());

	m_sync.Lock();
	{
		bool isFirstWrite = !std::ifstream(file_name.Str()).good();

		FILE* pFile = fopen(file_name.Str(), "a");

		if (pFile != NULL)
		{
			try
			{
				if (isFirstWrite)
				{
					//fputs("MT4\n", pFile);
					fputs("date, login, group, symbol, buy, amount, swaps, profit, sell, amount, swaps, profit, net, amount, swaps, profit", pFile);
					fputs("\n", pFile);
				}

				// print to report file
				fprintf(pFile, "%s", buf);
				fputs("\n", pFile);
			}
			catch (...)
			{
				//break;
			}
		}

		if (pFile != NULL)
			fclose(pFile);
	}
	m_sync.Unlock();
}

void CReports::Transaction(time_t rawtime, LPCTSTR apFormat, ...)
{
	char buf[2048] = { 0 };
	 StringTypePath file_name;

	va_list args;
	va_start(args, apFormat);
	vsprintf_s(buf, sizeof(buf) - 1, apFormat, args);
	va_end(args);

	struct tm * timeinfo;
	char buffer[80];

	////time(&rawtime);
	timeinfo = gmtime(&rawtime);

	strftime(buffer, 80, "\\Reports\\AllTransactions.%Y-%m-%d.csv", timeinfo);
	std::string timeString(buffer);

	file_name.Format("%s%s",ExtProgramPath,timeString.c_str());

	m_sync.Lock();
	{
		bool isFirstWrite = !std::ifstream(file_name.Str()).good();

		FILE* pFile = fopen(file_name.Str(), "a");

		if (pFile != NULL)
		{
			try
			{
				if (isFirstWrite)
				{
					//fputs("MT4\n", pFile);
					fputs("date of transaction, transaction id, type of transaction, client number, instrument, order type, buy-sell, order quantity, order price, stop loss, take profit, order date, order execution date, executed quantity, execution price, market bid price, market offer price, order status, ip, leverage, commission, tax, swap, profitandloss, instrumentpnl, spreadprofitandloss, crossprice, lp price", pFile);
					fputs("\n", pFile);
				}

				// print to report file
				fprintf(pFile, "%s", buf);
				fputs("\n", pFile);
			}
			catch (...)
			{
				//break;
			}
		}

		if (pFile != NULL)
			fclose(pFile);
	}
	m_sync.Unlock();
}