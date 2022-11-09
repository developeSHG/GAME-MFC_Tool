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

	//��ġ �ʱ�ȭ
	//1. IDirect3D9 ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	//2. ��ġ ����		(���� �׷���ī���� ��� �� �������� ����)
	//HAL (Hardward Abstraction Layer)	= �ϵ���� �߻� ����

	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(caps));
	//���� �׷���ī���� ���� ����.
	hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps);

	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);


	//���ؽ� ���μ��� ���� ���� - ���� �׷���ī�忡�� ���ؽ� ���μ����� �����ϴ°�
	//���ؽ� ���μ��� : ������ȯ + ������
	int vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//3. IDirect3DDevice9 ����

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = dwWinCX;									//�ĸ���� ����
	d3dpp.BackBufferHeight = dwWinCY;									//�ĸ���� ����
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;							//�ĸ���� �ȼ�����(32��Ʈ)
	d3dpp.BackBufferCount = 1;											//�ĸ���� ����
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;						//��Ƽ���ø� ��� ����
	d3dpp.MultiSampleQuality = 0;										//��Ƽ���ø� ���� ǰ��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//������ ���(����ü�� ���)
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = eMode;												//â���(True = â���, False = ��üȭ����)
	d3dpp.EnableAutoDepthStencil = true;								//����, ���ٽ� �������(���ٽ� = �׷����ҰͰ� �׷������� ���ƾ��� ���۸� �������ִ� ����)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;						//����, ���ٽ� ���� ����(��32, ���� 24, ���ٽ�8 )
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//â���� �����찡 �ֻ��� ����, ��üȭ���� ����̽��� �����ؾ���)
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
	D3DXFONT_DESC desc;                     // ��Ʈ�� �����Ѵ�.
	desc.CharSet = HANGUL_CHARSET;
	lstrcpy(desc.FaceName, L"���");                   // FaceName�� ��������� Windows Default Font�� ����Ѵ�.
	desc.Height = 10;    // ����
	desc.Width = 5;
	desc.Weight = FW_LIGHT;					//����
	desc.Quality = DEFAULT_QUALITY;
	desc.MipLevels = 1;
	desc.Italic = 0;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.PitchAndFamily = FF_DONTCARE;

	HRESULT hr = D3DXCreateFontIndirect(m_pD3DDevice, &desc, &m_pD3DFont);        // Font ����

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
	//������ ����
	//1. �ĸ���۸� ����.
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 110, 110, 110), 1.0f, 0);		//D3DCLEAR_TARGET = �ĸ����
														//z���۴� ������ 1�� �ʱ�ȭ�ؾ��Ѵ�.

	//2. �ĸ���ۿ� �׸��� �����Ѵ�.
	m_pD3DDevice->BeginScene();
	m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CMFCDeviceMgr::Render_End(HWND hWnd)	//HWND hWnd = nullptr
{
	//3. �ĸ���ۿ� �׸��⸦ ����ģ��.
	m_pD3DSprite->End();
	m_pD3DDevice->EndScene();

	//4. Flip (�ĸ���۸� ������۷� ��ü�Ͽ� �������Ѵ�)
	m_pD3DDevice->Present(nullptr, nullptr, hWnd, nullptr);		//3��°���ڡ�  ���� �׸�������. null�̾�� g_hWnd�ڵ鿡 �ͼӵ� ���� �׸���.

	
}
