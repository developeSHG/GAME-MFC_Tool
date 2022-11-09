#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"
#include "ToolView.h"
IMPLEMENT_SINGLETON(CTileMgr)

CTileMgr::CTileMgr()
{
	HRESULT hr = CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		L"Terrain", L"Tile", TILE_TEX);
	FAILED_CHECK(hr);

	hr = CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::SINGLE_TEXTURE, L"../Texture/Stage/Terrain/Event.png",
		L"Event");
	FAILED_CHECK(hr);

	hr = CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::SINGLE_TEXTURE, L"../Texture/Stage/Terrain/Wall.png",
		L"Wall");
	FAILED_CHECK(hr);
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Update(float fDeltaTime)
{

}

void CTileMgr::LateUpdate()
{
}

void CTileMgr::Render()
{
	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Render();

		swprintf_s(szIndex, L"%d", i);

		
		CMFCDeviceMgr::Get_Instance()->Get_Font()->DrawTextW(
			CMFCDeviceMgr::Get_Instance()->Get_Sprite(), /* 다이렉트 폰트는 출력마다 이미지를 생성 */
			szIndex, /* 출력할 텍스트 */
			lstrlen(szIndex), /* 텍스트의 길이 */
			nullptr, /* 사각형 영역 */
			DT_CENTER /* 사각형 영역 안에 출력할 때의 설정 값 */,
			D3DCOLOR_ARGB(255, 255, 255, 255)/* 출력 색상 */);

	}
}

void CTileMgr::Mini_Render()
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Mini_Render();
	}
}

void CTileMgr::Render_AnimationTile()
{
	for (size_t i = 0; i < m_vecAnimTile.size(); ++i)
		m_vecAnimTile[i]->Render();
}

void CTileMgr::Release()
{
	for_each(m_vecAnimTile.begin(), m_vecAnimTile.end(), Safe_Delete<CTile*>);
	m_vecAnimTile.clear();
	m_vecAnimTile.shrink_to_fit();

	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CTile*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Initialize()
{
	m_vecTile.reserve(TILE_X * TILE_Y);
	m_vecAnimTile.reserve(20);
	CTile* pTile = nullptr;

	for (int i = 0; i < TILE_Y; ++i)
	{
		for (int j = 0; j < TILE_X; ++j)
		{
			pTile = new CTile;
			pTile->Initialize();
			float centerX = (j * TILECX) + (i % 2) * (TILECX * 0.5f);;
			float centerY = i * (TILECY * 0.5f);;
			pTile->Set_Pos((float)centerX, (float)centerY);


			m_vecTile.push_back(pTile);
		}
	}

	Init_AnimTile();

}

void CTileMgr::Change_Tile(POINT tMousePos, int _TexID, bool _b, TCHAR _str[MIN_STR])
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		D3DXVECTOR4 tilePos = m_vecTile[i]->Get_Pos();


		D3DXVECTOR4 MousePos;
		MousePos.x = tMousePos.x - tilePos.x;
		MousePos.y = tMousePos.y - tilePos.y;
		MousePos.w = MousePos.z = 0;


		//1. 내적 이용
		/*D3DXVECTOR4 TilePosUp, TilePosLeft, TilePosDown, TilePosRight;
		ZeroMemory(&TilePosUp, sizeof(D3DXVECTOR4));
		ZeroMemory(&TilePosLeft, sizeof(D3DXVECTOR4));
		ZeroMemory(&TilePosDown, sizeof(D3DXVECTOR4));
		ZeroMemory(&TilePosRight, sizeof(D3DXVECTOR4));

		TilePosUp.y = -34.f;
		TilePosLeft.x = -65.f;
		TilePosDown.y = 34.f;
		TilePosRight.x = 65.f;


		D3DXVECTOR4 MouseToUp = TilePosUp - MousePos;
		D3DXVec4Normalize(&MouseToUp, &MouseToUp);
		D3DXVECTOR4 MouseToLeft = TilePosLeft - MousePos;
		D3DXVec4Normalize(&MouseToLeft, &MouseToLeft);
		D3DXVECTOR4 MouseToDown = TilePosDown - MousePos;
		D3DXVec4Normalize(&MouseToDown, &MouseToDown);
		D3DXVECTOR4 MouseToRight = TilePosRight - MousePos;
		D3DXVec4Normalize(&MouseToRight, &MouseToRight);

		float fAngle1 = D3DXToDegree(acosf(D3DXVec4Dot(&MouseToUp, &MouseToLeft)));
		float fAngle2 = D3DXToDegree(acosf(D3DXVec4Dot(&MouseToLeft, &MouseToDown)));
		float fAngle3 = D3DXToDegree(acosf(D3DXVec4Dot(&MouseToDown, &MouseToRight)));
		float fAngle4 = D3DXToDegree(acosf(D3DXVec4Dot(&MouseToRight, &MouseToUp)));

		float fTotalAngle = fAngle1 + fAngle2 + fAngle3 + fAngle4;


		if (fTotalAngle >= 359.f && fTotalAngle <= 361.f)
		{
			dynamic_cast<CTile*>(m_pTileArr[i][j])->Set_Index(3);
			bClick = true;
			break;
		}*/

		//2. 마름모 방정식 이용
		if (abs((tMousePos.x - tilePos.x) / 65) + abs((tMousePos.y - tilePos.y) / 34) < 1)
		{
			dynamic_cast<CTile*>(m_vecTile[i])->Set_Index(_TexID);
			dynamic_cast<CTile*>(m_vecTile[i])->Set_WallActive(_b);
			dynamic_cast<CTile*>(m_vecTile[i])->Set_EventActive(_str);
			break;
		}

	}
}

void CTileMgr::Init_AnimTile()
{
	CTile* pTile = new CTile;
	*pTile = *m_vecTile[165];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[184];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[185];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[204];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[205];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[206];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[223];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[224];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[225];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[226];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[244];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[245];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[246];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[264];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[265];
	m_vecAnimTile.push_back(pTile);
	pTile = new CTile;
	*pTile = *m_vecTile[285];
	m_vecAnimTile.push_back(pTile);


}
