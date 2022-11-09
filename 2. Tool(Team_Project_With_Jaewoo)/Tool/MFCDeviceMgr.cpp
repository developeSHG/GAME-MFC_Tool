#include "stdafx.h"
#include "MFCDeviceMgr.h"

IMPLEMENT_SINGLETON(CMFCDeviceMgr)

CMFCDeviceMgr::CMFCDeviceMgr()
{

}


CMFCDeviceMgr::~CMFCDeviceMgr()
{
	Release();
}

void CMFCDeviceMgr::Initialize()
{
	//DX_Setup();
}

void CMFCDeviceMgr::Release()
{
	if (Safe_Release(m_pD3DLine))
		ERR_MSG(L"m_pD3DLine Release Failed");

	if (Safe_Release(m_pD3DFont))
		ERR_MSG(L"m_pD3DFont Release Failed");

	if (Safe_Release(m_pD3DSprite))
		ERR_MSG(L"m_pD3DSprite Release Failed");

	if (Safe_Release(m_pD3DDevice))
		ERR_MSG(L"m_pD3DDevice Release Failed");

	if (Safe_Release(m_pSDK))
		ERR_MSG(L"m_pSDK Release Failed");
	////////////////////////////////////////////////////////////


}

HRESULT CMFCDeviceMgr::DX_Setup(HWND hWnd, const DWORD& dwWinCX, const DWORD& dwWinCY)
{
	Create_Device(hWnd, dwWinCX, dwWinCY, MODE_WIN);

	Create_Sprite();

	Create_DXFont();

	Create_Line();
	return S_OK;
}

HRESULT CMFCDeviceMgr::Create_Device(HWND hWnd, const DWORD& dwWinCX, const DWORD& dwWinCY, MODE eMode)
{
	HRESULT hr = S_OK;

	//장치 초기화
	//1. IDirect3D9 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	//2. 장치 조사		(현재 그래픽카드의 사양 및 지원여부 조사)
	//HAL (Hardward Abstraction Layer)	= 하드웨어 추상 계층

	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(caps));
	//현재 그래픽카드의 정보 조사.
	hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps);

	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);


	//버텍스 프로세싱 유무 조사 - 현재 그래픽카드에서 버텍스 프로세싱을 지원하는가
	//버텍스 프로세싱 : 정점변환 + 조명연산
	int vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//3. IDirect3DDevice9 생성

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = dwWinCX;									//후면버퍼 가로
	d3dpp.BackBufferHeight = dwWinCY;									//후면버퍼 높이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;							//후면버퍼 픽셀포맷(32비트)
	d3dpp.BackBufferCount = 1;											//후면버퍼 개수
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;						//멀티샘플링 사용 유무
	d3dpp.MultiSampleQuality = 0;										//멀티샘플링 사용시 품질
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//렌더링 방식(스왑체인 방식)
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;												//창모드(True = 창모드, False = 전체화면모드)
	d3dpp.EnableAutoDepthStencil = true;								//깊이, 스텐실 사용유무(스텐실 = 그려야할것과 그려야하지 말아야할 버퍼를 구분해주는 역할)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;						//깊이, 스텐실 버퍼 포맷(총32, 깊이 24, 스텐실8 )
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//창모드는 윈도우가 주사율 관리, 전체화면은 디바이스가 개입해야함)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		vp,
		&d3dpp,
		&m_pD3DDevice);

	FAILED_CHECK_MSG_RETURN(hr, L"CreateDevice Failed", E_FAIL);

	return S_OK;
}

HRESULT CMFCDeviceMgr::Create_Sprite()
{
	HRESULT hr = D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	return S_OK;
}

HRESULT CMFCDeviceMgr::Create_DXFont()
{
	D3DXFONT_DESC desc;                     // 폰트를 설정한다.
	desc.CharSet = HANGUL_CHARSET;
	lstrcpy(desc.FaceName, L"고딕");                   // FaceName을 비워놓으면 Windows Default Font를 사용한다.
	desc.Height = 10;    // 주의
	desc.Width = 5;
	desc.Weight = FW_LIGHT;					//굵기
	desc.Quality = DEFAULT_QUALITY;
	desc.MipLevels = 1;
	desc.Italic = 0;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.PitchAndFamily = FF_DONTCARE;

	HRESULT hr = D3DXCreateFontIndirect(m_pD3DDevice, &desc, &m_pD3DFont);        // Font 생성

	FAILED_CHECK_MSG_RETURN(hr, L"CreateFont - Failed", E_FAIL);

	return S_OK;
}

HRESULT CMFCDeviceMgr::Create_Line()
{
	HRESULT hr = D3DXCreateLine(m_pD3DDevice, &m_pD3DLine);

	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateLine Failed", E_FAIL);

	return S_OK;
}

void CMFCDeviceMgr::Render_Begin()
{
	//렌더링 과정
	//1. 후면버퍼를 비운다.
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 110, 110, 110), 1.0f, 0);		//D3DCLEAR_TARGET = 후면버퍼
														//z버퍼는 무조건 1로 초기화해야한다.

	//2. 후면버퍼에 그리기 시작한다.
	m_pD3DDevice->BeginScene();
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMFCDeviceMgr::Render_End(HWND hWnd)	//HWND hWnd = nullptr
{
	//3. 후면버퍼에 그리기를 끝마친다.
	m_pD3DSprite->End();
	m_pD3DDevice->EndScene();

	//4. Flip (후면버퍼를 전면버퍼로 교체하여 렌더링한다)
	m_pD3DDevice->Present(nullptr, nullptr, hWnd, nullptr);		//3번째인자★  어디다 그릴것인지. null이어야 g_hWnd핸들에 귀속된 곳에 그린다.

	
}
