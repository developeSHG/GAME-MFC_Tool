#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::Convert_RelativePath(const CString & strFullPath)
{
	TCHAR szCurrentPath[MAX_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	//절대경로 -> 상대 경로 변환
	//PathRelativePathTo(상대경로 반환, From, FILE_ATTRIBUTE_DIRECTORY, To, FILE_ATTRIBUTE_DIRECTORY)
	//From에서 To로 가는 상대경로를 만들어주는 함수. 단, From과 To는 같은 드라이브에 존재해야 한다.
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY, 
		strFullPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}
