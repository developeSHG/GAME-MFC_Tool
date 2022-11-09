#pragma once
class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

	static CString Convert_RelativePath(const CString& strFullPath);
};

