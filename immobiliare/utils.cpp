/*!****************************************************************************

	@file	utils.h
	@file	utils.cpp

	@brief	Utiltities routines

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "utils.h"
#include "commdefs.h"
#include "TFormDebug.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace boost::filesystem;


namespace utils
{

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString GetDataPath()
{
/*
	assert(Application);
	AnsiString strExePath = Sysutils::ExtractFilePath(Application->ExeName);
    AnsiString strDataPath = strExePath + DATAPATH;
*/

    boost::filesystem::path TheExePath( AnsiString(Application->ExeName).c_str() );
    boost::filesystem::path TheExeDir = TheExePath.parent_path();

    std::string strExeDir = TheExeDir.directory_string();
    std::string strDataPath = strExeDir + DATAPATH;

											// we have to remove some "relative"
                                            // refs inside the data path string
	boost::replace_all(strDataPath, "\\.\\", "\\");
	boost::replace_all(strDataPath, "/./", "\\");

/*
#ifdef _DEBUG
	FormDebug->Print(strExeDir);
    FormDebug->Print(strDataPath);
#endif
*/

	return AnsiString(strDataPath.c_str());
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool ParseDate(AnsiString strDate,
	unsigned short& nYear, unsigned short& nMonth, unsigned short& nDay)
{
	bool bResult = true;

	AnsiString strDay = strDate.SubString(0, strDate.Pos("/") - 1);

    strDate = strDate.SubString(strDate.Pos("/")+1, strDate.Length());
    AnsiString strMonth = strDate.SubString(0, strDate.Pos("/") - 1);

    AnsiString strYear = strDate.SubString(strDate.Pos("/")+1, strDate.Length());

    try
    {
		nDay = strDay.ToInt();
        nMonth = strMonth.ToInt();
        nYear = strYear.ToInt();
    }
    catch(...)
    {
    	bResult = false;
    }

    return bResult;
}

};


