#include "stdafx.h"
#include "MyTextureMgr.h"
#include "MySingleTexture.h"
#include "MyMultiTexture.h"
#include "UiTool.h"
IMPLEMENT_SINGLETON(CMyTextureMgr)

CMyTextureMgr::CMyTextureMgr()
{
	
}


CMyTextureMgr::~CMyTextureMgr()
{
	Release();
}

const TEX_INFO* CMyTextureMgr::GetTexInfo(
	const wstring & wstrObjectKey,
	const wstring & wstrStateKey,
	const int & iIndex)
{
	auto iter_find = m_MapTexture.find(wstrObjectKey);

	if (m_MapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CMyTextureMgr::LoadTexture(
	TEXTURE_TYPE eTextureType,
	const wstring& wstrFilePath,
	const wstring& wstrObjectKey,
	const wstring& wstrStateKey,
	const int& iCount)
{
	CMyTexture* pTexture = nullptr;

	auto iter_find = m_MapTexture.find(wstrObjectKey);

	switch (eTextureType)
	{
	case SINGLE_TEXTURE:
		if (m_MapTexture.end() != iter_find)
			return E_FAIL;

		pTexture = new CMySingleTexture;
		m_MapTexture[wstrObjectKey] = pTexture;
		break;
	case MULTI_TEXTURE:
		if (m_MapTexture.end() == iter_find)
		{
			pTexture = new CMyMultiTexture;
			m_MapTexture[wstrObjectKey] = pTexture;
		}
		break;
	}

	HRESULT hr = m_MapTexture[wstrObjectKey]->LoadTexture(
		wstrFilePath, wstrStateKey, iCount);
	FAILED_CHECK_MSG_RETURN(hr, L"LoadTexture Failed", E_FAIL);

	return S_OK;
}

void CMyTextureMgr::Release()
{
	for (auto& MyPair : m_MapTexture)
		Safe_Delete(MyPair.second);

	m_MapTexture.clear();
}

void CMyTextureMgr::Initialize()
{
	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/IDLE/RU%d.png",
		L"Player", L"IDLE_RU", 4);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/IDLE/RD%d.png",
		L"Player", L"IDLE_RD", 4);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/IDLE/D%d.png",
		L"Player", L"IDLE_D", 4);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/IDLE/U%d.png",
		L"Player", L"IDLE_U", 4);

	////////////////////////////////////////////////////////
	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/WALK/RU%d.png",
		L"Player", L"WALK_RU", 8);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/WALK/RD%d.png",
		L"Player", L"WALK_RD", 9);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/WALK/D%d.png",
		L"Player", L"WALK_D", 8);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/WALK/U%d.png",
		L"Player", L"WALK_U", 8);

	////////////////////////////////////////////////////////
	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/JUMP/RU%d.png",
		L"Player", L"JUMP_RU", 7);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/JUMP/RD%d.png",
		L"Player", L"JUMP_RD", 7);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/JUMP/D%d.png",
		L"Player", L"JUMP_D", 7);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Player/JUMP/U%d.png",
		L"Player", L"JUMP_U", 7);




	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Gun/Pistol/IDLE/R%d.png",
		L"Pistol", L"IDLE_R", 1);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Gun/Pistol/RELOAD/R%d.png",
		L"Pistol", L"RELOAD_R", 6);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Gun/Pistol/FIRE/R%d.png",
		L"Pistol", L"FIRE_R", 2);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Monster/BulletMonster/IDLE/D%d.png",
		L"BulletMonster", L"IDLE_D", 1);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Monster/RedJelly/IDLE/D%d.png",
		L"RedJelly", L"IDLE_D", 4);

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::MULTI_TEXTURE, L"../Texture/Unit/Monster/BlueJelly/IDLE/D%d.png",
		L"BlueJelly", L"IDLE_D", 4);



	////////////////////////////////////////////////////////////////////////////////////////////
	//UI///

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::SINGLE_TEXTURE, L"../Texture/UI/UI_PLAYERBAR.png",L"UI_CHARACTER");

	CMyTextureMgr::Get_Instance()->LoadTexture(
		CMyTextureMgr::SINGLE_TEXTURE, L"../Texture/UI/UI_SKILLBAR.png", L"UI_SKILLBAR");

}

