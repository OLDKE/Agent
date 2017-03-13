#include "StdAfx.h"
#include "MyZip.h"

MyZip::MyZip(void)
{
}

MyZip::~MyZip(void)
{
}

bool MyZip::unzip(char* zipfilename,char* unzippath)
{
	CString tmpstr;

	try
	{
		tmpstr=zipfilename;
//		m_zip.Open((LPCTSTR)tmpstr,CZipArchive::zipOpen);
		tmpstr=unzippath;
		int filenum ;//= m_zip.GetCount();
		for(int i=0;i<filenum;i++)
		{
//			if(!m_zip.ExtractFile(i,(LPCTSTR)tmpstr,true,NULL))
			{
//				m_zip.Close();
				return false;
			}			
		}
	}
	catch (CException* e)
	{
//		m_zip.Close();
		return false;		
	}
//	m_zip.Close();
	return true;

}

bool MyZip::zippara(char* zipfilename,char* zippara)
{
	CString tmpstr;
	CBrowseDir m_browerdir;
	CBrowseDir DirTest;
	int i;
	try
	{
		tmpstr=zipfilename;
//		m_zip.Open((LPCTSTR)tmpstr,CZipArchive::zipCreate);

		char* tmpbuf;
		CString tmpfilename;
		tmpbuf=strtok(zippara,"~");
		while (1)
		{
			if(tmpbuf == NULL )
				break;

			tmpfilename=tmpbuf;
			if( (strchr(tmpbuf,'*') != NULL ) || (strchr(tmpbuf,'?') != NULL ))
			{
				char* pdest;
				char tpf[256];
				memset(tpf,0,sizeof(tpf));
				pdest = strrchr(tmpbuf,'\\');

				strcpy(tpf,pdest+1);
				pdest[0]='\0';

				tmpfilename=tmpbuf;
				if(::GetFileAttributes(tmpfilename) & FILE_ATTRIBUTE_DIRECTORY)
				{
					DirTest.SetInitDir(tmpbuf);
					DirTest.BeginBrowse(tpf);
					int filenum= DirTest.fileArray.GetSize();//.GetCount();
					int foldernum= DirTest.folderArray.GetSize();//.GetCount();

					for(i=0;i<foldernum;i++)
					{
//						m_zip.AddNewFile((LPCTSTR)DirTest.folderArray.GetAt(i),9,true);
					}

					for(i=0;i<filenum;i++)
					{
//						m_zip.AddNewFile((LPCTSTR)DirTest.fileArray.GetAt(i),9,true);
					}
				}
			}
			else if(::GetFileAttributes(tmpfilename) & FILE_ATTRIBUTE_ARCHIVE)
			{
//				m_zip.AddNewFile((LPCTSTR)tmpfilename,9,true);
			}
			else if(::GetFileAttributes(tmpfilename) & FILE_ATTRIBUTE_DIRECTORY)
			{
				DirTest.SetInitDir(tmpbuf);
				DirTest.BeginBrowse("*.*");
				int filenum= DirTest.fileArray.GetSize();//.GetCount();
				int foldernum= DirTest.folderArray.GetSize();//.GetCount();

				for(i=0;i<foldernum;i++)
				{
//					m_zip.AddNewFile((LPCTSTR)DirTest.folderArray.GetAt(i),9,true);
				}

				for(i=0;i<filenum;i++)
				{
//					m_zip.AddNewFile((LPCTSTR)DirTest.fileArray.GetAt(i),9,true);
				}
			}
			tmpbuf = strtok(NULL,"~");

		}

	}
	catch (CException* e)
	{
//		m_zip.Close();
		return false;		
	}
//	m_zip.Close();
	return true;

}
