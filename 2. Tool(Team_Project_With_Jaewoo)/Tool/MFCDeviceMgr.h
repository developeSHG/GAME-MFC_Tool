#pragma once
class CMFCDeviceMgr
{
	DECLARE_SINGLETON(CMFCDeviceMgr)

public:
	enum MODE { MODE_FULL = 0, MODE_WIN, MODE_END };

private:
	CMFCDeviceMgr();
	~CMFCDeviceMgr();

public:

	void Initialize();
	void Release();

	LPDIRECT3DDEVICE9 Get_Device() const { return m_pD3DDevice; }
	LPD3DXSPRITE Get_Sprite() const { return m_pD3DSprite; }
	LPD3DXFONT Get_Font() const { return m_pD3DFont; }
	LPD3DXLINE Get_Line() const { return m_pD3DLine; }



	HRESULT DX_Setup(HWND hWnd, const DWORD& dwWinCX, const DWORD& dwWinCY);
	HRESULT Create_Device(HWND hWnd, const DWORD& dwWinCX, const DWORD& dwWinCY, MODE eMode);
	HRESULT Create_Sprite();
	HRESULT Create_DXFont();
	HRESULT Create_Line();

	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);


private:
	//장치검증에 필요한 Com객체
	//LPDIRECT3DDEVICE9의 생성도 맡는다.
	LPDIRECT3D9				m_pSDK;

	//그래픽 장치를 제어할 Com객체
	LPDIRECT3DDEVICE9		m_pD3DDevice;

	//COM	(Component Object Model)

	LPD3DXFONT				m_pD3DFont;

	//이미지 렌더링 Com 객체.
	LPD3DXSPRITE			m_pD3DSprite;

	//선 렌더링 Com객체
	LPD3DXLINE				m_pD3DLine;
};

