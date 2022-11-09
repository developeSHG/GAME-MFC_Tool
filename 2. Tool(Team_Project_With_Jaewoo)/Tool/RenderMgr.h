#pragma once

class CUnitTool;
class CAnimationTool;
class CRenderMgr
{
	DECLARE_SINGLETON(CRenderMgr)

public:
	enum TOOL_ID { UNIT, ANIMATION, TILE, MAP, UI, ID_END };
private:
	CRenderMgr();
	~CRenderMgr();

public:

public:
	void Update(float fDeltaTime);
	void LateUpdate();
	void Render();
	void Release();
	void Initialize();

	void Render_Animation();
	void Render_Unit();

public:	//for Animation Tool

	void Set_IsAnimation(bool _b) { m_bIsAnimation = _b; }
	void Set_Unit(wstring _Unit) { m_strUnit = _Unit; }
	void Set_Key(wstring _Key) { m_strKey = _Key; }

	void Set_ToolID(TOOL_ID eID) { m_eID = eID; }
	void Set_FrameStart(float _f) { m_fCurrentFrame = _f; }
	void Set_FrameEnd(float _f) { m_fMaxFrameCount = _f; }
	void Set_FrameSpeed(float _f) { m_fFrameSpeed = _f; }

	void FrameMove();
	void Set_AnimTool(CAnimationTool* pTool) { m_pAnimTool = pTool; }
	TOOL_ID Get_ToolID() { return m_eID; }

public:	//for Unit Tool
	void Set_UnitTool(CUnitTool* pTool) { m_pUnitTool = pTool; }
	CUnitTool* m_pUnitTool;
	void Set_CurRenderID(UNIT_DATA::ID eID) { m_eCurRenderID = eID; }
	UNIT_DATA::ID	 m_eCurRenderID;

public:	//for Tile Tool
	int m_iTileIndex;
	void Set_TileIndex(int _index) { m_iTileIndex = _index; }
	void Render_Tile();
	

public:	//for Map Tool
	void Set_MapMosterInfo(map<CString, vector<MONSTER_POS*>>* pMap) { m_pMapMonsterInfo = pMap; }
	void Render_MapTool();
	void Render_MapTool_MiniMap();
	void Render_MonsterPreview(HWND hWnd);

	UNIT_DATA* m_pMonsterPreview;

private:			//for Animation Tool
	wstring m_strUnit;
	wstring m_strKey;

	bool m_bIsAnimation;

	TOOL_ID		m_eID;
	float m_fCurrentFrame;
	float m_fMaxFrameCount;
	float m_fFrameSpeed;
	CAnimationTool* m_pAnimTool;
	map<CString, vector<MONSTER_POS*>>* m_pMapMonsterInfo;

public:				//for UI Tool
	void Render_UI();
	void Set_MapUiInfo(map<CString, UI_POS*>* pUI) { m_pUiInfo = pUI; }
	void Render_UiTool();
	
	
private:			//for UI Tool
	map<CString, UI_POS*>* m_pUiInfo;
};

