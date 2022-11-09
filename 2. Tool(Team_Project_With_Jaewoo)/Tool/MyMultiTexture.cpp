#include "stdafx.h"
#include "MyMultiTexture.h"


CMyMultiTexture::CMyMultiTexture()
{
}


CMyMultiTexture::~CMyMultiTexture()
{
	Release();
}

const TEX_INFO* CMyMultiTexture::GetTexInfo(
	const wstring & wstrStateKey/* = L""*/,
	const int& iIndex/* = 0*/)
{
	auto iter_find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() == iter_find)
		return nullptr;

	return iter_find->second[iIndex];
}

HRESULT CMyMultiTexture::LoadTexture(
	const wstring& wstrFilePath, /* ..\Texture\Stage\Terrain\Tile\Tile%d.png */
	const wstring& wstrStateKey /*= L""*/,
	const int& iCount/* = 0*/)
{
	auto iter_find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() != iter_find)
		return E_FAIL;

	TCHAR szFilePath[MAX_STR] = L"";

	HRESULT hr = 0;

	for (int i = 0; i < iCount; ++i)
	{
		TEX_INFO* pTexInfo = new TEX_INFO;
		ZeroMemory(pTexInfo, sizeof(TEX_INFO));

		swprintf_s(szFilePath, wstrFilePath.c_str(), i);

		//..\Texture\Stage\Terrain\Tile\Tile0.png
		hr = D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tImgInfo);
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);
		
		// IDIRECT3DTEXTURE9 ����
		hr = D3DXCreateTextureFromFileEx(
			CMFCDeviceMgr::Get_Instance()->Get_Device(), /* ��ġ */
			szFilePath, /* �̹��� ��� */
			pTexInfo->tImgInfo.Width, /* �̹��� ����ũ�� */
			pTexInfo->tImgInfo.Height, /* �̹��� ����ũ�� */
			pTexInfo->tImgInfo.MipLevels, /* mip level */
			0,
			pTexInfo->tImgInfo.Format, /* �̹��� �ȼ� ���� */
			D3DPOOL_MANAGED, /* �޸� ����� */
			D3DX_DEFAULT,	/* �̹��� �׵θ� ǥ��(����) */
			D3DX_DEFAULT,	/* Mip Filter */
			0, /* Color key ������ ���� */
			nullptr, /* D3DXIMAGE_INFO�� ������ */
			nullptr, /* �̹��� �ȷ�Ʈ */
			&pTexInfo->pTexture);
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);

	

		m_mapMultiTex[wstrStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMyMultiTexture::Release()
{
	for (auto& MyPair : m_mapMultiTex)
	{
		for (auto& pTexInfo : MyPair.second)
		{
			pTexInfo->pTexture->Release();
			SAFE_DELETE(pTexInfo);
		}

		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	}

	m_mapMultiTex.clear();
}
