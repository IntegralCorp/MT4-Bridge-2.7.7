//+------------------------------------------------------------------+
//|                                                           Syslog |
//|                 Copyright � 2001-2006, MetaQuotes Software Corp. |
//|                                         http://www.metaquotes.ru |
//+------------------------------------------------------------------+
#pragma once
#include <handleapi.h>
#include <string>
#include <vector>

//+------------------------------------------------------------------+
//| ����� ������������ ������ �����                                  |
//+------------------------------------------------------------------+
class CStringFile
{
private:
	HANDLE            m_file;                 // ����� �����
	DWORD             m_file_size;            // ������ �����
	BYTE             *m_buffer;               // ����� ��� ������
	int               m_buffer_size;          // ��� ������
	int               m_buffer_index;         // ������� ������� ��������
	int               m_buffer_readed;        // ������ ���������� � ������ ������
	int               m_buffer_line;          // ������� ����� � �����

public:
	CStringFile(const int nBufSize = 65536);
	~CStringFile();
	//----
	bool              Open(LPCTSTR lpFileName, const DWORD dwAccess, const DWORD dwCreationFlags);
	inline void       Close() { if (m_file != INVALID_HANDLE_VALUE) { CloseHandle(m_file); m_file = INVALID_HANDLE_VALUE; } m_file_size = 0; }
	inline DWORD      Size() const { return(m_file_size); }
	int               Read(void  *buffer, const DWORD length);
	int               Write(const void *buffer, const DWORD length);
	void              Reset();
	int               GetNextLine(char *line, const int maxsize);
};
//+------------------------------------------------------------------+

//funcs
typedef std::vector<std::string> StringVector;
void Split(const std::string& str, const std::string& delim, StringVector& output);