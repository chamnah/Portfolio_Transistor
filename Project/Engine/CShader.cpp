#include "CShader.h"
#include "CDirect.h"
#include "CPathMgr.h"

CShader::CShader()
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pCSBlob);
	SAFE_RELEASE(m_pErrBlob);

	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pCS);
}

void CShader::CreateVertexShader(const string& _strFunc, const string& _strVersion)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\std.fx";

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
		assert(false && "CreateVertexShader Error");
}

void CShader::CreatePixelShader(const string& _strFunc, const string& _strVersion)
{

	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\std.fx";

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pPSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), NULL, &m_pPS)))
		assert(false && "CreatePixelShader Error");
}

void CShader::CreateGeoMetryShader(const string & _strFunc, const string & _strVersion)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\std.fx";

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pGSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(), NULL, &m_pGS)))
		assert(false && "CreateGeometryShader Error");
}

void CShader::CreateComputeShader(const string& _strFunc, const string& _strVersion)
{
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Shader\\std.fx";

	UINT iFlag = 0;
	string strErr;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFunc.c_str(), _strVersion.c_str(), iFlag, 0, &m_pCSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		assert(false && strErr.c_str());
	}

	if (FAILED(DEVICE->CreateComputeShader(m_pCSBlob->GetBufferPointer(), m_pCSBlob->GetBufferSize(), NULL, &m_pCS)))
		assert(false && "CreateComputeShader Error");
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_pVS, NULL, NULL);
	CONTEXT->PSSetShader(m_pPS, NULL, NULL);
	CONTEXT->GSSetShader(m_pGS, NULL, NULL);
	CONTEXT->CSSetShader(m_pCS, NULL, NULL);
}