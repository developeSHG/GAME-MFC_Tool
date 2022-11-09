#include "stdafx.h"
#include "MySingleTexture.h"


CMySingleTexture::CMySingleTexture()
{
}


CMySingleTexture::~CMySingleTexture()
{
	Release();
}

const TEX_INFO* CMySingleTexture::GetTexInfo(
	const wstring & wstrStateKey,
	const int & iIndex)
{
	return m_pTexInfo;
}

HRESULT CMySingleTexture::LoadTexture(
	const wstring & wstrFilePath,
	const wstring & wstrStateKey /*= L""*/,
	const int & iCount/* = 0*/)
{
	m_pTexInfo = new TEX_INFO;
	ZeroMemory(m_pTexInfo, sizeof(TEX_INFO));

	HRESULT hr = 0;
	hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->tImgInfo);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

	// IDIRECT3DTEXTURE9 생성
	hr = D3DXCreateTextureFromFileEx(
		CMFCDeviceMgr::Get_Instance()->Get_Device(), /* 장치 */
		wstrFilePath.c_str(), /* 이미지 경로 */
		m_pTexInfo->tImgInfo.Width, /* 이미지 가로크기 */
		m_pTexInfo->tImgInfo.Height, /* 이미지 세로크기 */
		m_pTexInfo->tImgInfo.MipLevels, /* mip level */
		0,
		m_pTexInfo->tImgInfo.Format, /* 이미지 픽셀 포맷 */
		D3DPOOL_MANAGED, /* 메모리 사용방식 */
		D3DX_DEFAULT,	/* 이미지 테두리 표현(필터) */
		D3DX_DEFAULT,	/* Mip Filter */
		0, /* Color key 제거할 색상 */
		nullptr, /* D3DXIMAGE_INFO의 포인터 */
		nullptr, /* 이미지 팔레트 */
		&m_pTexInfo->pTexture);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);

	return S_OK;
}

void CMySingleTexture::Release()
{
	if (Safe_Release(m_pTexInfo->pTexture))
		ERR_MSG(L"m_pTexInfo->pTexture Release Failed");

	Safe_Delete(m_pTexInfo);
}
