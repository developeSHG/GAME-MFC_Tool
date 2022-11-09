#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define OBJ_NOEVENT INT_MAX
#define OBJ_DEAD	INT_MIN


#define TOOLCX 300
#define TOOLCY 720

#define WINCX 1280
#define WINCY 720

#define TILE_X 20
#define TILE_Y 30

#define TILECX	130
#define TILECY	68 

#define MIN_STR 64
#define MID_STR 128
#define MAX_STR 256

#define M_JELLY_CX 48
#define M_JELLY_CY 40

#ifndef SAFE_DEL
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = nullptr; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = nullptr; }
#endif


#define NO_COPY(ClassName)									 \
private:													 \
ClassName(const ClassName& obj);							 \
ClassName& operator=(const ClassName& obj);					 


#define DECLARE_SINGLETON(ClassName)						 \
		NO_COPY(ClassName)									 \
public:														 \
	static ClassName* Get_Instance();						 \
	void Destroy_Instance();									 \
private:													 \
	static ClassName* m_pInstance;							 


#define IMPLEMENT_SINGLETON(ClassName)						 \
ClassName* ClassName::m_pInstance = nullptr;				 \
ClassName* ClassName::Get_Instance()							 \
{															 \
	if (m_pInstance == nullptr)								 \
	{														 \
		m_pInstance = new ClassName;						 \
	}														 \
	return m_pInstance;										 \
}															 \
void ClassName::Destroy_Instance()							 \
{															 \
	SAFE_DELETE(m_pInstance);								 \
}




#endif // !__DEFINE_H__