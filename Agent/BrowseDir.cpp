#include "StdAfx.h"
#include "BrowseDir.h"
#include "stdlib.h"
#include "direct.h"
#include "string.h"
#include "io.h"

#include "browsedir.h"

CBrowseDir::CBrowseDir()
{
	//用当前目录初始化m_szInitDir
	_getcwd(m_szInitDir,_MAX_PATH);

	//如果目录的最后一个字母不是'\',则在最后加上一个'\'
	int len=strlen(m_szInitDir);
	if (m_szInitDir[len-1] != '\\')
		strcat(m_szInitDir,"\\");
}

BOOL CBrowseDir::IsExistFile(CString filePath)
{
	CFileStatus status;
	return CFile::GetStatus(filePath,status);
}
//目录是否存在
BOOL CBrowseDir::IsExistDir(CString filePath)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	if(filePath.Right(1) == "\\")
		filePath = filePath.Left(filePath.GetLength()-1);
	//检测驱动器是否存在
	if(filePath.GetLength()==2)
	{
		int iDriveType = GetDriveType(filePath);
		if(iDriveType == DRIVE_FIXED || 
			iDriveType == DRIVE_REMOVABLE ||
			iDriveType == DRIVE_REMOTE)
		{
			return TRUE;
		}
	}
    HANDLE hFind = FindFirstFile(filePath, &fd);
    if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		//if (hFind != INVALID_HANDLE_VALUE)
    {
		//目录存在
		ret = TRUE;
    }
    FindClose(hFind);
	return ret;  
}

bool CBrowseDir::GetAllDirFile(CString strDir, CStringArray &array)
{
	CFileFind find;
	//*********************
	//CUtils::FixDirPath(strDir);
	if( (strDir.Right(1) != "\\") && (strDir.Right(1) != "/") )
	{
		CString strTmp;
		strTmp.Format("%s\\", strDir);
		strDir = strTmp;
	}

	strDir += "*.*";
	BOOL blFind = find.FindFile(strDir);
	while(blFind)
	{
		blFind = find.FindNextFile();
		if (find.IsDots())
			continue;

		if(!find.IsDirectory())
		{
			array.Add(find.GetFileName());
		}
	}
	return true;
}


bool CBrowseDir::SetInitDir(const char *dir)
{
	////先把dir转换为绝对路径
 //	if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
 //		return false;

	//判断目录是否存在
	strcpy(m_szInitDir,dir);
	if (_chdir(m_szInitDir) != 0)
		return false;

	//如果目录的最后一个字母不是'\',则在最后加上一个'\'
	int len=strlen(m_szInitDir);
	if (m_szInitDir[len-1] != '\\')
		strcat(m_szInitDir,"\\");
	return true;
}

bool CBrowseDir::BeginBrowse(const char *filespec)
{
	fileArray.RemoveAll();
	folderArray.RemoveAll();

	ProcessDir(m_szInitDir,NULL);
	return BrowseDir(m_szInitDir,filespec);
}

bool CBrowseDir::BeginBrowseA(const char *filespec)
{
	fileArray.RemoveAll();
	folderArray.RemoveAll();

	ProcessDir(m_szInitDir,NULL);
	return BrowseDirA(m_szInitDir,filespec);
}

bool CBrowseDir::BrowseDir
(const char *dir,const char *filespec)
{
	_chdir(dir);

	//首先查找dir中符合要求的文件
	long hFile;
	_finddata_t fileinfo;
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
	{
		do
		{
			//检查是不是目录
			//如果不是,则进行处理
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy(filename,dir);
				strcat(filename,fileinfo.name);
				if (!ProcessFile(filename))
					return false;
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}

	//查找dir中的子目录
	//因为在处理dir中的文件时，派生类的ProcessFile有可能改变了
	//当前目录，因此还要重新设置当前目录为dir。
	//执行过_findfirst后，可能系统记录下了相关信息，因此改变目录
	//对_findnext没有影响。
	_chdir(dir);
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
	{
		do
		{
			//检查是不是目录
			//如果是,再检查是不是 . 或 .. 
			//如果不是,进行迭代
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name,".") != 0 && strcmp
					(fileinfo.name,"..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy(subdir,dir);
					strcat(subdir,fileinfo.name);
					strcat(subdir,"\\");
					ProcessDir(subdir,dir);
					if (!BrowseDir(subdir,filespec))
						return false;
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
	return true;
}

bool CBrowseDir::BrowseDirA
(const char *dir,const char *filespec)//改得爽啊
{
	_chdir(dir);
	long hFile;
	_finddata_t fileinfo;
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy(filename,dir);
				strcat(filename,fileinfo.name);
				if (!ProcessFile(filename))
					return false;
			}
			else if((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name,".") != 0 && strcmp
					(fileinfo.name,"..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy(subdir,dir);
					strcat(subdir,fileinfo.name);
					strcat(subdir,"\\");
					ProcessDir(subdir,dir);
					if (!BrowseDirA(subdir,"*"))
						return false;
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}	
	return true;
}

bool CBrowseDir::ProcessFile(const char *filename)
{
	CString tmpfilename;
	tmpfilename=filename;
	fileArray.Add(tmpfilename);
	return true;
}

void CBrowseDir::ProcessDir(const char *currentdir,const char *parentdir)
{
	CString tmpfoldername;
	tmpfoldername=currentdir;
	folderArray.Add(tmpfoldername);
}
