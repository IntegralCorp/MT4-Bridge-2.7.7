#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <dbghelp.h>
#include <shlwapi.h>
#include <crtdbg.h>
#include "eh.h"

namespace DebugUtil
{
	class CAutoCheckMemomyLeaks
	{
	public:
		CAutoCheckMemomyLeaks()
		{
			_CrtMemCheckpoint(&m_stateOld);
		}
		~CAutoCheckMemomyLeaks()
		{
#ifdef _DEBUG
			_CrtMemState stateNew, stateDiff;
			_CrtMemCheckpoint(&stateNew);
			if (_CrtMemDifference(&stateDiff, &m_stateOld, &stateNew))
			{
				_CrtMemDumpStatistics(&stateDiff);
				_CrtDumpMemoryLeaks();
				_ASSERT(false);
			}
#endif
		}

	private:
		_CrtMemState    m_stateOld;
	};


	class CUnhandledExceptionFilter
	{
	public:

		static HANDLE idcModule;
		CUnhandledExceptionFilter() throw()
		{
			m_prevFilter = ::SetUnhandledExceptionFilter(&OnUnhandledException);
		}
		~CUnhandledExceptionFilter() throw()
		{
			::SetUnhandledExceptionFilter(m_prevFilter);
		}

	private:
		typedef BOOL(WINAPI *FnMiniDumpWriteDump)(
			IN HANDLE hProcess,
			IN DWORD ProcessId,
			IN HANDLE hFile,
			IN MINIDUMP_TYPE DumpType,
			IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
			IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
			IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

		static LONG WINAPI OnUnhandledException(EXCEPTION_POINTERS *ExceptionInfo)
		{
			HMODULE hModule = LoadLibrary(TEXT("dbghelp.dll"));

			if (hModule)
			{
				FnMiniDumpWriteDump fnMiniDumpWriteDump = ( FnMiniDumpWriteDump )GetProcAddress(hModule, "MiniDumpWriteDump");

				if (fnMiniDumpWriteDump)
				{
					SYSTEMTIME st;
					GetLocalTime(&st);
					TCHAR path[MAX_PATH];
					TCHAR fileName[MAX_PATH];

					GetModuleFileName(( HMODULE )idcModule, path, MAX_PATH);
					PathRemoveFileSpec(path);

					wsprintf(fileName, TEXT("IDC.Crash.%04hu-%02hu-%02hu-%02hu%02hu.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

					//wsprintf(fileName, TEXT("IDC.Crash.%04hu-%02hu-%02hu-%02hu-%02hu-%02hu.dmp"),
					//		 st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

					PathAppend(path, "Logs");
					PathAppend(path, fileName);

					HANDLE hFile = CreateFile(path,
											  GENERIC_WRITE, FILE_SHARE_READ, NULL,
											  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

					if (hFile != INVALID_HANDLE_VALUE)
					{
						MINIDUMP_EXCEPTION_INFORMATION mei;
						mei.ThreadId = GetCurrentThreadId();
						mei.ExceptionPointers = ExceptionInfo;
						mei.ClientPointers = TRUE;
						fnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
											hFile, MiniDumpNormal, &mei, NULL, NULL);

						CloseHandle(hFile);
					}
				}
			}
			return EXCEPTION_CONTINUE_SEARCH;
		}

	private:
		LPTOP_LEVEL_EXCEPTION_FILTER    m_prevFilter;
	};

#pragma warning(push)
#pragma warning(disable:4535) // calling _set_se_translator() requires /EHa

	class CTransparentSEHTraslator
	{
	public:
		CTransparentSEHTraslator() throw()
		{
			m_prevTranlator = _set_se_translator(&OnTranslateSE);
		}
		~CTransparentSEHTraslator() throw()
		{
			_set_se_translator(m_prevTranlator);
		}

	private:
		static void OnTranslateSE(unsigned int, _EXCEPTION_POINTERS *)
		{
			throw;
		}

	private:
		_se_translator_function m_prevTranlator;
	};

#pragma warning(pop)

}