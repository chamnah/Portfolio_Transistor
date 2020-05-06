// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "CCore.h"
#include "CSaveLoadMgr.h"
#include "CPathMgr.h"
#include "CGameMgr.h"
#include "CResMgr.h"
#include "CSound.h"
#include "CSceneMgr.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HWND                g_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

	ShowCursor(false);

	CCore::GetInst()->Init(tResolution(1920, 1080), g_hWnd);
	CSaveLoadMgr::SetClient(true);

	wstring wcsPath = CPathMgr::GetInst()->GetPath();
	wcsPath += L"Sound\\Start.mp3";
	CSound* sound = new CSound;
	sound->Load(wcsPath, L"Start");
	CResMgr::GetInst()->AddResource<CSound>(L"Start", sound);
	CResMgr::GetInst()->Load<CSound>(L"Start")->Play(-1);

	wstring strPath =  CPathMgr::GetInst()->GetPath();
	strPath += L"\\Scene\\Start.scene";
	CSaveLoadMgr::LoadScene(strPath);

	CGameMgr::GetInst()->MouseInit();

    // 기본 메시지 루프입니다:
    while (true)
    {
		if ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (CSaveLoadMgr::IsLoadScene())
			{
				CSaveLoadMgr::SetLoadScene(false);
				CSaveLoadMgr::LoadScene();
			}

			if (CSaveLoadMgr::IsChangeScene())
			{
				CSaveLoadMgr::SetChangeScene(false);
				CSceneMgr::GetInst()->ChangeScene(CSaveLoadMgr::GetScene());
				CSaveLoadMgr::SetScene(nullptr);
			}

			CCore::GetInst()->Update();
			CCore::GetInst()->Render();
		}

		if (msg.message == WM_QUIT)
			break;
    }

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor		=  LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   RECT rt = {0,0,1920,995};

   AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW,false);
   SetWindowPos(g_hWnd,NULL,-10,0,rt.right - rt.left,rt.bottom - rt.top,0);

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}