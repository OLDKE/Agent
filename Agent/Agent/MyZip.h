#pragma once
#include "ZipArchive\ZipArchive.h"
#include "ZipArchive\ZipPlatform.h"
#include "BrowseDir.h"

class MyZip
{
public:
	MyZip(void);
	~MyZip(void);

//	CZipArchive m_zip;

	bool unzip(char* zipfilename,char* unzippath);	
	bool zippara(char* zipfilename,char* zippara);	

};
