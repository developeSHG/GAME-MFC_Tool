#include "stdafx.h"
#include "RenderMgr.h"
#include "TileMgr.h"
#include "UnitTool.h"
#include "AnimationTool.h"
#include "ToolView.h"
IMPLEMENT_SINGLETON(CRenderMgr)

CRenderMgr::CRenderMgr()
{

}


CRenderMgr::~CRenderMgr()
{
	Release();
}

void CRenderMgr::Update(float fDeltaTime)
{
}

void CRenderMgr::LateUpdate()
{
}

void CRenderMgr::Render()
{
	CMFCDeviceMgr::Get_Instance()->Render_Begin();

	if (m_eID == TOOL_ID::UNIT)
	{
		Render_Unit();
	}
	else if (m_eID == TOOL_ID::ANIMATION)
	{
		CTileMgr::Get_Instance()->Render_AnimationTile();
		
		Render_Animation();
		
	}
	else if (m_eID == TOOL_ID::TILE)
	{
		Render_Tile();
	}
	else if (m_eID == TOOL_ID::MAP)
	{
		CTileMgr::Get_Instance()->Render();
		Render_MapTool();

		/*if (m_pMonsterPreview)
			Render_MonsterPreview();*/
	}
	else if (m_eID == TOOL_ID::UI)
	{
		Render_UI();
	}
	CMFCDeviceMgr::Get_Instance()->Render_End();
}

void CRenderMgr::Release()
{
	
}

void CRenderMgr::Initialize()
{
	m_bIsAnimation = false;
	m_fCurrentFrame = 0.f;
	m_fMaxFrameCount = 0.f;
	m_fFrameSpeed = 1.f;

	m_iTileIndex = -1;

	m_pMonsterPreview = nullptr;
}

void CRenderMgr::Render_Animation()
{
	if (m_bIsAnimation)
	{
		FrameMove();
		m_pAnimTool->m_SliderCtrl.SetPos(m_fCurrentFrame / m_fMaxFrameCount * 100);

		//if(m_fCurrentFrame!=)
		m_pAnimTool->Set_SelectIndex((int)m_fCurrentFrame);

	}

	int iScrollX = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(0);
	int iScrollY = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(1);

	//m_pAnimTool->Set_FrameIndex(m_fCurrentFrame);
	//m_pAnimTool->Invalidate(FALSE);
	
	const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
		m_strUnit, m_strKey,(int)m_fCurrentFrame);
	NULL_CHECK(pTexInfo);


	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, 1.f, 1.f, 0.f);
	
	D3DXMatrixTranslation(&mTrans, WINCX / 2 - 10 - iScrollX, WINCY / 2 - 25 - iScrollY, 0.f);
	mWorld = mScale*mTrans;

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CRenderMgr::Render_Unit()
{
	
	map<CString, UNIT_DATA*>*	m_pMapUnit = &(m_pUnitTool->m_mapUnitData);

	UNIT_DATA::ID eNewID = UNIT_DATA::END;
	UNIT_DATA::ID eOldID = UNIT_DATA::END;

	int i = 0;
	int j = 0;

	int iMonsterCount = 0;
	int iItemCount = 0;

	for (auto& MyUnit : *m_pMapUnit)
	{
		if(MyUnit.second->eID== m_eCurRenderID)
		{
			for (auto& MyTex : *CMyTextureMgr::Get_Instance()->Get_mapTexture())
			{
				if (MyUnit.first.GetString() == MyTex.first)
				{
					eNewID = MyUnit.second->eID;
					if (eNewID != eOldID && eOldID != UNIT_DATA::END)
						++i;

					wstring unitName = MyUnit.first.GetString();

					const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
						unitName, L"IDLE_R", 0);
					if (pTexInfo == nullptr)
						pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
							unitName, L"IDLE_D", 0);


					D3DXMATRIX mScale, mWorld, mTrans;
					D3DXMatrixScaling(&mScale, 1.f, 1.f, 0.f);

					D3DXMatrixTranslation(&mTrans, j * 100.f, 0 , 0.f);
					mWorld = mScale*mTrans;

					CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

					CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
						&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					++j;
					break;
				}
			}
		}
		else if (m_eCurRenderID == UNIT_DATA::END)
		{
			
		
			for (auto& MyTex : *CMyTextureMgr::Get_Instance()->Get_mapTexture())
			{
				
				if (MyUnit.first.GetString() == MyTex.first)
				{
					wstring unitName = MyUnit.first.GetString();

					const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
						unitName, L"IDLE_R", 0);
					if (pTexInfo == nullptr)
						pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
							unitName, L"IDLE_D", 0);


					D3DXMATRIX mScale, mWorld, mTrans;
					D3DXMatrixScaling(&mScale, 1.f, 1.f, 0.f);

					if (MyUnit.second->eID == UNIT_DATA::PLAYER)
					{
						D3DXMatrixTranslation(&mTrans, 0, MyUnit.second->eID * 100, 0.f);
					}
					else if (MyUnit.second->eID == UNIT_DATA::MONSTER)
					{
						D3DXMatrixTranslation(&mTrans, iMonsterCount * 100.f, MyUnit.second->eID * 100, 0.f);
						iMonsterCount++;
					}
					else if (MyUnit.second->eID == UNIT_DATA::ITEM)
					{
						D3DXMatrixTranslation(&mTrans, iItemCount * 100.f, MyUnit.second->eID * 100, 0.f);
						iItemCount++;
					}
					mWorld = mScale*mTrans;
					CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

					CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
						&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
					
					break;
				}
			}
		}
		
	}
}


void CRenderMgr::FrameMove()
{
	// 1초에 m_fMaxFrameCount만큼 애니메이션 재생.
	m_fCurrentFrame += m_fMaxFrameCount * CMyTimeMgr::Get_Instance()->GetDeltaTime() * m_fFrameSpeed;


	if ((int)m_fCurrentFrame >= (int)(m_fMaxFrameCount + 1))
		m_fCurrentFrame = 0.f;
}

void CRenderMgr::Render_Tile()
{
	if (m_iTileIndex == -1)
		return;

}

void CRenderMgr::Render_MapTool()
{

	int iScrollX = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(0);
	int iScrollY = CTileMgr::Get_Instance()->Get_ToolView()->GetScrollPos(1);

	for (auto& Monster : *m_pMapMonsterInfo)
	{
		for (size_t i = 0; i < Monster.second.size(); ++i)
		{


			wstring unitName = Monster.first.GetString();

			const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
				unitName, L"IDLE_R", 0);
			if (pTexInfo == nullptr)
				pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
					unitName, L"IDLE_D", 0);

			D3DXMATRIX mScale, mWorld, mTrans;
			D3DXMatrixScaling(&mScale, 1.f, 1.f, 0.f);

			D3DXMatrixTranslation(&mTrans, Monster.second[i]->vPos.x - iScrollX, Monster.second[i]->vPos.y - iScrollY, 0.f);
			mWorld = mScale*mTrans;

			CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

			CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CRenderMgr::Render_MapTool_MiniMap()
{
	for (auto& Monster : *m_pMapMonsterInfo)
	{
		for (size_t i = 0; i < Monster.second.size(); ++i)
		{
			wstring unitName = Monster.first.GetString();

			const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
				unitName, L"IDLE_R", 0);
			if (pTexInfo == nullptr)
				pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
					unitName, L"IDLE_D", 0);

			D3DXMATRIX mScale, mWorld, mTrans;
			D3DXMatrixScaling(&mScale, 1.f/2.f, 1.f/1.4f, 0.f);

			D3DXMatrixTranslation(&mTrans, Monster.second[i]->vPos.x/2.f, Monster.second[i]->vPos.y/1.4f, 0.f);
			mWorld = mScale*mTrans;

			CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

			CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CRenderMgr::Render_MonsterPreview(HWND hWnd)
{
	CMFCDeviceMgr::Get_Instance()->Render_Begin();
	wstring unitName = m_pMonsterPreview->strName.GetString();

	const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
		unitName, L"IDLE_R", 0);
	if (pTexInfo == nullptr)
		pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			unitName, L"IDLE_D", 0);

	D3DXMATRIX mScale, mWorld, mTrans;
	D3DXMatrixScaling(&mScale, WINCX / pTexInfo->tImgInfo.Width , WINCY / pTexInfo->tImgInfo.Height, 0.f);

	D3DXMatrixTranslation(&mTrans, 0.f, 0.f, 0.f);
	mWorld = mScale*mTrans;

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

	CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	CMFCDeviceMgr::Get_Instance()->Render_End(hWnd);
}



void CRenderMgr::Render_UI()
{
	for (auto& UI : *m_pUiInfo)
	{
		
		wstring unitName = UI.first.GetString();

		const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			unitName);


		D3DXMATRIX mScale, mWorld, mTrans;
		D3DXMatrixScaling(&mScale, 1.f, 1.f, 0.f);

		D3DXMatrixTranslation(&mTrans, UI.second->vPos.x, UI.second->vPos.y, 0.f);
		mWorld = mScale*mTrans;

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		
	}
}

void CRenderMgr::Render_UiTool()
{
	for (auto& mapUI : *m_pUiInfo)
	{
		wstring UIName = mapUI.first.GetString();

		const TEX_INFO* pTexInfo = CMyTextureMgr::Get_Instance()->GetTexInfo(
			UIName);
		
		D3DXMATRIX mScale, mWorld, mTrans;
		D3DXMatrixScaling(&mScale, 1.f, 1.f, 0.f);

		D3DXMatrixTranslation(&mTrans, mapUI.second->vPos.x, mapUI.second->vPos.y, 0.f);
		mWorld = mScale*mTrans;

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->SetTransform(&mWorld);

		CMFCDeviceMgr::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
