#include "stdafx.h"
#include "Tile.h"
#include "TileMgr.h"
#include "ToolView.h"

CTile::CTile()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.mScale);
	D3DXMatrixIdentity(&m_tInfo.mRot);
	D3DXMatrixIdentity(&m_tInfo.mTrans);
	D3DXMatrixIdentity(&m_tInfo.mWorld);

	ZeroMemory(&m_tInfo.vDir, sizeof(D3DXVECTOR4));
	ZeroMemory(&m_tInfo.vLook, sizeof(D3DXVECTOR4));
	ZeroMemory(&m_tInfo.vPos, sizeof(D3DXVECTOR4));
	m_tInfo.vPos.w = 1;
	ZeroMemory(&m_tInfo.vSize, sizeof(D3DXVECTOR4));

}


CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));

	m_tInfo.vSize = { 1.f,1.f,0.f,0.f };
	m_tTileInfo.byDrawID = 2;
	m_tTileInfo.byOption = 0;
	m_tTileInfo.bIsWall = FALSE;
}

int CTile::Update(float fDeltaTime)
{
	return 0;
}

void CTile::Render()
{
	const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"Tile", m_tTileInfo.byDrawID);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	int iScrollX = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(0);
	int iScrollY = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(1);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans, (m_tInfo.vPos.x- iScrollX), (m_tInfo.vPos.y - iScrollY), 0.f);
	
	matWorld = matScale * matRotZ * matTrans;

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(
		pTexInfo->pTexture, /* LPDIRECT3DTEXTURE9 */
		nullptr, /* Rect의 포인터, 그릴 영역 */
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), /* 출력 중심점 */
		nullptr, /* 출력 위치 */
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//wcscat_s(m_tTileInfo.strEvent, L"");
	if (0 < wcslen(m_tTileInfo.strEvent))
	{
		pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			L"Event");

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo->pTexture, /* LPDIRECT3DTEXTURE9 */
			nullptr, /* Rect의 포인터, 그릴 영역 */
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), /* 출력 중심점 */
			nullptr, /* 출력 위치 */
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (TRUE == m_tTileInfo.bIsWall)
	{
		pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			L"Wall"); 

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo->pTexture, /* LPDIRECT3DTEXTURE9 */
			nullptr, /* Rect의 포인터, 그릴 영역 */
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), /* 출력 중심점 */
			nullptr, /* 출력 위치 */
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTile::Release()
{
}

void CTile::Mini_Render()
{
	const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
		L"Terrain", L"Tile", m_tTileInfo.byDrawID);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	//int iScrollX = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(0);
	//int iScrollY = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(1);

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1.f / 2, 1.f / 1.4, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0.f));
	D3DXMatrixTranslation(&matTrans, (m_tInfo.vPos.x /*- iScrollX*/) / 2, (m_tInfo.vPos.y /*- iScrollY*/) / 1.4, 0.f);

	matWorld = matScale * matRotZ * matTrans;

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(
		pTexInfo->pTexture, /* LPDIRECT3DTEXTURE9 */
		nullptr, /* Rect의 포인터, 그릴 영역 */
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), /* 출력 중심점 */
		nullptr, /* 출력 위치 */
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//wcscat_s(m_tTileInfo.strEvent, L"");
	if (0 < wcslen(m_tTileInfo.strEvent))
	{
		pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			L"Event");

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo->pTexture, /* LPDIRECT3DTEXTURE9 */
			nullptr, /* Rect의 포인터, 그릴 영역 */
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), /* 출력 중심점 */
			nullptr, /* 출력 위치 */
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (TRUE == m_tTileInfo.bIsWall)
	{
		pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			L"Wall");

		fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(
			pTexInfo->pTexture, /* LPDIRECT3DTEXTURE9 */
			nullptr, /* Rect의 포인터, 그릴 영역 */
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), /* 출력 중심점 */
			nullptr, /* 출력 위치 */
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
