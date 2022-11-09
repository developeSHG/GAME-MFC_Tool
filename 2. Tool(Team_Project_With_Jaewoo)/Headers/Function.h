#pragma once

class CMyStrCmp
{
public:
	CMyStrCmp() {}
	CMyStrCmp(const TCHAR* pString)
		:m_pString(pString)
	{}
	~CMyStrCmp() {}
public:
	template<class T>
	bool operator()(T& rObj)
	{
		return !lstrcmp(rObj.first, m_pString);

	}
private:
	const TCHAR* m_pString;

};

class CObj;
template <typename T>
T DCast(CObj* _pCObj)
{

	return dynamic_cast<T>(_pCObj);
}

template <typename T>
void Safe_Delete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}




template <typename T>
DWORD Safe_Release(T& com)
{
	DWORD refcnt = com->Release();

	if (0 == refcnt)
		com = nullptr;

	return refcnt;
}
