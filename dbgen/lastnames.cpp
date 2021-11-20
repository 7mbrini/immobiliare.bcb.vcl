/*!****************************************************************************

	@file	lastnames.h
	@file	lastnames.cpp

	@brief	Data access for "lastnames" file archive

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "TFormDebug.h"
#include "lastnames.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


TLastNamesArchive::TLastNamesArchive()
{

}

void __fastcall TLastNamesArchive::Clear()
{
	m_LastNames.clear();
}

int __fastcall TLastNamesArchive::GetCount()
{
	return m_LastNames.size();
}

bool __fastcall TLastNamesArchive::LoadFromFile(AnsiString strFileName)
{
	bool bResult = false;

    if( ::FileExists(strFileName) )
    {
                                            // First of all, clear old data!
        Clear();

        TStringList *pList = new TStringList;
        assert(pList);

        pList->LoadFromFile(strFileName);

                                                // i=1 --> skip first heading row
        for(int i=1; i<pList->Count; i++)
        {
            TLastName LastName;

            if( Parse(pList->Strings[i], LastName) )
            {
                m_LastNames.push_back(LastName);
            }
        }

        delete pList;

        bResult = true;
	}

	return bResult;
}

bool __fastcall TLastNamesArchive::Parse(AnsiString strTuple, TLastName& LastName)
{
	bool bResult = true;

    LastName.m_strName = strTuple.SubString(0, strTuple.Pos(",") - 1);

    strTuple = strTuple.SubString(strTuple.Pos(",") + 1, strTuple.Length());

    if( !strTuple.IsEmpty() )
    {
		LastName.m_nFreq = strTuple.ToInt();
    }
	else
    {
    	bResult = false;
    }

    return bResult && LastName.m_strName.Length();
}

void __fastcall TLastNamesArchive::GetLastNames(TVecLastNames& LastNames)
{
	LastNames = m_LastNames;
}

