#include "CPathMgr.h"

CPathMgr::CPathMgr()
	:m_wcsPath(L"")
{
}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::Init()
{
	GetCurrentDirectory(MAX_PATH, m_wcsPath);

	for (size_t iNum = wcslen(m_wcsPath) - 1; iNum >= 0; --iNum)
	{
		if (m_wcsPath[iNum] == '\\')
		{
			m_wcsPath[iNum + 1] = NULL;
			break;
		}
	}

	wcscat_s(m_wcsPath,L"content\\");
}

wstring CPathMgr::GetPathSlice(const wstring & _wcsPath)
{
	wstring wcsName;

	for (size_t iNum = wcslen(_wcsPath.c_str()) - 1; iNum >= 0; --iNum)
	{
		if (_wcsPath[iNum] == '\\')
		{
			for (UINT i = iNum + 1; i < wcslen(_wcsPath.c_str()); ++i)
			{
				wcsName += _wcsPath[i];
			}

			return wcsName;
		}
	}

	return wcsName;
}

wstring CPathMgr::AbsoluteToRelativeResPath(const wstring & _wcsPath)
{
	int iIdx = _wcsPath.find(L"content");

	wstring wcsPath = L"..\\";

	for (int i = iIdx; i < _wcsPath.size(); ++i)
	{
		wcsPath += _wcsPath[i];
	}

	return wcsPath;
}