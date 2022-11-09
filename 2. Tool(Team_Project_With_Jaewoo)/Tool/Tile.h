#pragma once

typedef struct tagTileInfo
{
	BYTE		byDrawID;
	BYTE		byOption;
	bool		bIsWall;
	TCHAR		strEvent[MIN_STR] = L"";
}TILE_INFO;

class CTile
{
public:
	CTile();
	~CTile();

	// CMyObj을(를) 통해 상속됨
	void Initialize();
	int Update(float fDeltaTime);
	void Render();
	void Release();

public:
	void Set_Info(INFO& Info) { m_tInfo = Info; }
	void Set_TileInfo(TILE_INFO& tileInfo) { m_tTileInfo = tileInfo; }
	void Set_Pos(float _x, float _y) { m_tInfo.vPos.x = _x; m_tInfo.vPos.y = _y; }
	void Set_Index(int _i) { m_tTileInfo.byDrawID = _i; }
	void Set_WallActive(bool _b) { m_tTileInfo.bIsWall = _b; }
	void Set_EventActive(TCHAR _str[MIN_STR]) { wcscpy_s(m_tTileInfo.strEvent, _str); }

public:
	INFO& Get_Info() { return m_tInfo; }
	TILE_INFO& Get_TileInfo() { return m_tTileInfo; }
	D3DXVECTOR4 Get_Pos() { return m_tInfo.vPos; }
	BYTE Get_Index() { return m_tTileInfo.byDrawID; }
	
public:
	void Mini_Render();

private:
	TILE_INFO m_tTileInfo;
	INFO m_tInfo;
	// CMyObj을(를) 통해 상속됨
};

