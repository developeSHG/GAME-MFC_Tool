#pragma once



#ifndef CLIENT
typedef struct tagUnitData
{
	enum ID
	{
		PLAYER, MONSTER, ITEM, END
	};

	ID eID;
	CString strName;
	UINT iHp;
	UINT iMp;
	UINT iAtt;
	UINT iDef;
	UINT iLife;
	float fSpeed;
	UINT iGold;
	UINT iLevel;
	double dExp;
}UNIT_DATA;

typedef struct tagUnitAnimData
{
	float m_fFrameSpeed;
}ANIM_DATA;

#endif


typedef struct tagInfo
{
	D3DXVECTOR4 vWPos;	// 월드 위치벡터
	D3DXVECTOR4	vPos;	// 로컬 위치벡터
	
	D3DXVECTOR4	vDir;	// 방향벡터
	D3DXVECTOR4 vSize;	// 크기벡터
	D3DXVECTOR4 vLook;

	D3DXMATRIX mScale;
	D3DXMATRIX mRot;
	D3DXMATRIX mTrans;
	D3DXMATRIX mWorld;
}INFO;

typedef struct tagColInfo
{
	RECT colRect;

	int iColSizeX;
	int iColSizeY;
}COLINFO;

//이거 1개가 텍스쳐 1장
typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9  pTexture;		//불러올 텍스처를 제어하는 COM객체.
	D3DXIMAGE_INFO		tImgInfo;		//불러온 텍스처의 정보.
}TEX_INFO;

typedef vector<TEX_INFO*>					VEC_TEXINFO;
typedef map<const wstring, VEC_TEXINFO>		MAP_MULTITEX;

typedef struct tagFrame
{
	int iFrameStart; // 이미지 시작점
	int iFrameEnd;  // 이미지 끝점 
	int iCurFrame; // 이미지 행동. 
	DWORD dwFrameSpeed; // 재생 속도 
	DWORD dwFrameTime; // 재생 시간. 

}FRAME;
