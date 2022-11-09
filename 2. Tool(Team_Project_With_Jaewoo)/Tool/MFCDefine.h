#pragma once

#define TILE_TEX 38

typedef struct tagMonsterPos
{
	D3DXVECTOR4 vPos;
}MONSTER_POS;

typedef struct tagUIPos
{
	CString strName;
	D3DXVECTOR4 vPos;
}UI_POS;

#define ERR_MSG(msg)	\
AfxMessageBox(msg)

#define NULL_CHECK(ptr) \
if(nullptr == (ptr))	\
	return;


#define NULL_CHECK_MSG(ptr, msg)	\
if(nullptr == (ptr))				\
{									\
	ERR_MSG(msg);					\
	return;							\
}

#define NULL_CHECK_RETURN(ptr, returnVal)	\
if(nullptr == (ptr))						\
	return returnVal;

#define NULL_CHECK_MSG_RETURN(ptr, msg, returnVal)	\
if(nullptr == (ptr))								\
{													\
	ERR_MSG(msg);									\
	return returnVal;								\
}

#define FAILED_CHECK(hr)\
if(FAILED(hr))			\
	return;

#define FAILED_CHECK_MSG(hr, msg)	\
if(FAILED(hr))						\
{									\
	ERR_MSG(msg);					\
	return;							\
}

#define FAILED_CHECK_RETURN(hr, returnVal)	\
if(FAILED(hr))								\
	return returnVal;

#define FAILED_CHECK_MSG_RETURN(hr, msg, returnVal)	\
if(FAILED(hr))										\
{													\
	ERR_MSG(msg);									\
	return returnVal;								\
}

