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
	//��ġ������ �ʿ��� Com��ü
	//LPDIRECT3DDEVICE9�� ������ �ô´�.
	LPDIRECT3D9				m_pSDK;

	//�׷��� ��ġ�� ������ Com��ü
	LPDIRECT3DDEVICE9		m_pD3DDevice;

	//COM	(Component Object Model)

	LPD3DXFONT				m_pD3DFont;

	//�̹��� ������ Com ��ü.
	LPD3DXSPRITE			m_pD3DSprite;

	//�� ������ Com��ü
	LPD3DXLINE				m_pD3DLine;
};

