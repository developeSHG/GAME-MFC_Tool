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

	//���� �۾� ��θ� ������ �Լ�
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	//������ -> ��� ��� ��ȯ
	//PathRelativePathTo(����� ��ȯ, From, FILE_ATTRIBUTE_DIRECTORY, To, FILE_ATTRIBUTE_DIRECTORY)
	//From���� To�� ���� ����θ� ������ִ� �Լ�. ��, From�� To�� ���� ����̺꿡 �����ؾ� �Ѵ�.
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY, 
		strFullPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}
