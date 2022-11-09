#pragma once



class CTile;
class CToolView;

class CTileMgr
{
	DECLARE_SINGLETON(CTileMgr)

private:
	CTileMgr();
	~CTileMgr();

public:
	void Set_ToolView(CToolView* pView) { m_pToolView = pView; }
	CToolView* Get_ToolView() { return m_pToolView; }

public:
	void Update(float fDeltaTime);
	void LateUpdate();


	void Render();
	void Mini_Render();
	void Render_AnimationTile();

	void Release();
	void Initialize();

	void Change_Tile(POINT tMousePos, int _TexID, bool _b = FALSE, TCHAR _str[MIN_STR]= L"");
	void Init_AnimTile();


public:
	vector<CTile*>* Get_TileVector() { return &m_vecTile; }

private:
	vector <CTile*>	m_vecTile;
	vector <CTile*>	m_vecAnimTile;
	CToolView*			m_pToolView;			//Tile 클래스에서 불러와서 스크롤값 사용.
};

