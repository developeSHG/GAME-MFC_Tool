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
	D3DXVECTOR4 vWPos;	// ���� ��ġ����
	D3DXVECTOR4	vPos;	// ���� ��ġ����
	
	D3DXVECTOR4	vDir;	// ���⺤��
	D3DXVECTOR4 vSize;	// ũ�⺤��
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

//�̰� 1���� �ؽ��� 1��
typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9  pTexture;		//�ҷ��� �ؽ�ó�� �����ϴ� COM��ü.
	D3DXIMAGE_INFO		tImgInfo;		//�ҷ��� �ؽ�ó�� ����.
}TEX_INFO;

typedef vector<TEX_INFO*>					VEC_TEXINFO;
typedef map<const wstring, VEC_TEXINFO>		MAP_MULTITEX;

typedef struct tagFrame
{
	int iFrameStart; // �̹��� ������
	int iFrameEnd;  // �̹��� ���� 
	int iCurFrame; // �̹��� �ൿ. 
	DWORD dwFrameSpeed; // ��� �ӵ� 
	DWORD dwFrameTime; // ��� �ð�. 

}FRAME;
