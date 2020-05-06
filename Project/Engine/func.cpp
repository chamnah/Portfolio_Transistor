#include "func.h"

void SaveWString(const wstring & _str, FILE * _pFile)
{
	assert(!(255 < _str.length()));

	BYTE length = (BYTE)_str.length();
	fwrite(&length, sizeof(BYTE), 1, _pFile);
	fwrite(_str.c_str(), sizeof(wchar_t), length, _pFile);
}

wstring LoadWString(FILE * _pFile)
{
	wchar_t wcsBuff[100] = {};
	BYTE length = 0;

	fread(&length, sizeof(BYTE), 1, _pFile);
	fread(wcsBuff,sizeof(wchar_t),length,_pFile);

	return wcsBuff;
}

float Gradient(Vec2 _vStart, Vec2 _vEnd)
{
	float fGradient = (_vEnd.y - _vStart.y) / (_vEnd.x - _vStart.x);
	return fGradient;
}

float Gradient(Vec3 _vStart, Vec3 _vEnd)
{
	float fGradient = (_vEnd.y - _vStart.y) / (_vEnd.x - _vStart.x);
	return fGradient;
}

float RadianToDegree(float _fAngle)
{
	return _fAngle * 180.f / XM_PI;
}

float DegreeToRadian(float _fAngle)
{
	return _fAngle * XM_PI / 180.f;
}

Vec3 GetVec2Dir(Vec3 _vPos, Vec3 _vTarget)
{
	_vPos.z = 0.f;
	_vTarget.z = 0.f;
	Vec3 vDir = _vTarget - _vPos;
	vDir.Normalize();

	return vDir;
}

float Lerf(float _fStart, float _fDest, float _fRatio)
{
	return _fStart + (_fDest - _fStart) * _fRatio;
}

Vec3 Lerf(Vec3 _vStart, Vec3 _vDest, float _fRatio)
{
	Vec3 vLerf;
	vLerf.x = Lerf(_vStart.x, _vDest.x, _fRatio);
	vLerf.y = Lerf(_vStart.y, _vDest.y, _fRatio);

	return vLerf;
}