/*!****************************************************************************

	@file	cities.h
	@file	cities.cpp

	@brief	Data access for "cities" file archive

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "TFormDebug.h"
#include "cities.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


/*!****************************************************************************
* @brief
******************************************************************************/
TCitiesArchive::TCitiesArchive()
{

}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCitiesArchive::Clear()
{
	m_Cities.clear();
}

/*!****************************************************************************
* @brief
******************************************************************************/
int TCitiesArchive::GetCount()
{
	return m_Cities.size();
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TCitiesArchive::LoadFromFile(AnsiString strFileName)
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
            TCity City;

            if( Parse(pList->Strings[i], City) )
            {
                m_Cities.push_back(City);
            }
        }

        delete pList;

        bResult = true;
	}

	return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TCitiesArchive::Parse(AnsiString strTuple, TCity& City)
{
    City.m_strName = strTuple.SubString(0, strTuple.Pos(",") - 1);

    strTuple = strTuple.SubString(strTuple.Pos(",") + 1, strTuple.Length());
    City.m_strProv = strTuple.SubString(0, strTuple.Pos(",") - 1);

    strTuple = strTuple.SubString(strTuple.Pos(",") + 1, strTuple.Length());
    City.m_strCap = strTuple.SubString(0, strTuple.Pos(",") - 1);

    strTuple = strTuple.SubString(strTuple.Pos(",") + 1, strTuple.Length());
    City.m_strRegion = strTuple;

#ifdef _DEBUG
//	FormDebug->Print(City.AsString());
#endif
										// some little checks ...
	return City.m_strName.Length()
    	+ City.m_strProv.Length()
        + City.m_strCap.Length()
        + City.m_strRegion.Length();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCitiesArchive::GetCities(TVecCities& Cities)
{
	Cities = m_Cities;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TCitiesArchive::IsUnique(TVecStrings& Strings, std::string strVal)
{
	bool bUnique = true;

	for(int i=0; i<Strings.size(); i++)
    {
		if( strVal == Strings[i] )
        {
        	bUnique = false;
            break;
        }
    }

    return bUnique;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCitiesArchive::GetRegions(TVecStrings& Regions)
{
	for(int i=0; i<m_Cities.size(); i++)
    {
		if( IsUnique(Regions, m_Cities[i].m_strRegion.c_str()) )
        {
            Regions.push_back(m_Cities[i].m_strRegion.c_str());
        }
    }
}


