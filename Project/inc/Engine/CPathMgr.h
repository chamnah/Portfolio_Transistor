#include "global.h"

#pragma once
class CPathMgr
{
private:
	wchar_t m_wcsPath[MAX_PATH];

public:
	void Init();
	wstring GetPathSlice(const wstring& _wcsPath);
	wstring AbsoluteToRelativeResPath(const wstring& _wcsPath);
	wchar_t* GetPath() { return m_wcsPath; }



public:
	SINGLETON(CPathMgr);

};