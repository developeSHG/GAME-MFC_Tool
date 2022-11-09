#pragma once

class CMyTexture;
class CUiTool;
class CMyTextureMgr
{
	DECLARE_SINGLETON(CMyTextureMgr)

public:
	enum TEXTURE_TYPE { SINGLE_TEXTURE, MULTI_TEXTURE };

private:
	CMyTextureMgr();
	~CMyTextureMgr();

public:
	const TEX_INFO* GetTexInfo(
		const wstring& wstrObjectKey,
		const wstring & wstrStateKey = L"",
		const int & iIndex = 0);

public:
	HRESULT LoadTexture(
		TEXTURE_TYPE eTextureType,
		const wstring& wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		const int& iCount = 0);
	void Release();


public:
	void Initialize();
	map<const wstring, CMyTexture*>* Get_mapTexture() { return &m_MapTexture; }

	
	CUiTool* m_pUiTool;
private:
	map<const wstring, CMyTexture*>	m_MapTexture;
};

