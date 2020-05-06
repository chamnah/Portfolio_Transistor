#pragma once
#include "global.h"
#include  <queue>
using std::queue;
using std::priority_queue;



class CScene;
class CCamera;
class CGameObj;
class CLayer;

struct cmp {
	bool operator()(CGameObj* t, CGameObj* u);
};

class CSceneMgr
{
	SINGLETON(CSceneMgr);

private:
	CScene*     m_pCurScene;
	bool        m_bDelObj;
	priority_queue<CGameObj*, vector<CGameObj*>, cmp> m_queueRender;
	vector<CGameObj*> m_vecRender;

public:
	void RegisterCamera(CCamera* _pCam);
	void AddObject(const wstring& _wcsLayer, CGameObj* _pObj);
	void AddObject(const UINT _iIdx, CGameObj* _pObj);
	CLayer* GetLayer(int _iIdx);
	CLayer* GetLayer(const wstring& _wcsName);
	UINT GetLayerIndex(const wstring& _wcsName);
	CGameObj* FindObject(const wstring& _wcsName);
	void AddLayer(const wstring& _wcsLayer);
	CScene* GetCurScene() { return m_pCurScene; }
	const wstring& GetLayerName(UINT _iIdx);
	void ChangeScene(CScene* pScene);
	UINT GetCurLayerCount();
	void FrontSorting(CGameObj* _pObj);
	void SetStop(bool _bStop);
	bool GetStop();
	bool GetObjOfLayer(wstring _wcsLayer, int _iIdx, CGameObj** _pObj);
	void SetDelObj(bool _bDel) { m_bDelObj = _bDel; }
	void PushRenderObj(CGameObj* _pObj) { m_queueRender.push(_pObj); }
	void RenderSort(UINT _iMask);
	void RenderClear();

public:
	void Init();
	void Update();
	void Render();
};