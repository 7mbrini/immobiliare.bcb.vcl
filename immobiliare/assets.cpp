/*!****************************************************************************

	@file	assets.h
	@file	assets.cpp

	@brief	Assets modelling

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "assets.h"
#include "sorting.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)


#define _USECUSTOMSORT


static const char MAGICWORD[ASSET_MAGICLEN] = "IMMOBILIARE-ASSETS-(C)2021-StudioWARE";


using namespace sort;


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
TAsset::TAsset()
{
	memset(m_strDescription, 0, ASSET_DESCRMAXLEN);
	memset(m_strAddress, 0, ASSET_NAMESMAXLEN);
	memset(m_strCity, 0, ASSET_NAMESMAXLEN);

    m_nStatus = asNone;
	m_nTypology = atNone;

	m_SoldDate = m_BuildDate = 0;
    m_nValue = m_nCap = m_nPrice = 0;
	//m_nFloor = m_nRooms = m_nBathrooms = m_nArea = 0;
	m_nRooms = m_nBathrooms = m_nArea = 0;

	m_bImage = false;
	memset(m_pImage, 0, ASSET_IMAGEMAXSIZE);
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString TAsset::AsString()
{
	AnsiString strResult;

	strResult.printf(
    	"\nDescription: %s"
        "\nAddress: %s"
        "\nCity: %s"
        "\nCAP: %d"
        "\nTypology: %d"
        "\nStatus: %d"
        "\nRooms: %d"
		"\nBathrooms: %d"
        "\nArea: %d"
		"\nCadastral value: %d",
        "\nPrice: %d"
        "\nSold date: %f"
        "\nHave Image: %s",
        m_strDescription,
        m_strAddress,
        m_strCity,
        m_nCap,
        m_nTypology,
 		m_nStatus,
        m_nRooms,
        m_nBathrooms,
        m_nArea,
        m_nValue,
        m_nPrice,
        m_SoldDate,
        m_bImage ? "Yes" : "No"
    );

    return strResult;
}

/*!****************************************************************************
* @brief	Read the record from file
* @param	fp Pointer to FILE struct
******************************************************************************/
bool TAsset::Read(FILE* fp)
{
	assert(fp);

    return fread(this, sizeof(*this), 1, fp);
}

/*!****************************************************************************
* @brief	Write the record to file
* @param	fp Pointer to FILE struct
******************************************************************************/
bool TAsset::Write(FILE* fp)
{
	assert(fp);

	return fwrite(this, sizeof(*this), 1, fp);
}

/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
TAssetsArchive::TAssetsArchive()
{
	;
}

/*!****************************************************************************
* @brief	Destructor
******************************************************************************/
TAssetsArchive::~TAssetsArchive()
{
	Clear();
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TAssetsArchive::LoadFromFile(AnsiString strFileName)
{
	bool bResult = false;

    FILE* pFile = fopen(strFileName.c_str(), "rb");

    if( pFile )
    {
		Clear();

		TFileHeader Header;
        memset(&Header, 0, sizeof(Header));

        fread(&Header, sizeof(Header), 1, pFile);

		if( strcmpi(Header.pMagic, MAGICWORD) )
        {
			fclose(pFile);
			bResult = false;
        }
		else
        {
			for(int i=0; i<Header.nRecords; i++)
            {
                TAsset *pAsset = new TAsset;
                assert(pAsset);
                pAsset->Read(pFile);
                Add(pAsset);
            }

            fclose(pFile);
            bResult = true;
		}
    }

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TAssetsArchive::SaveToFile(AnsiString strFileName)
{
	bool bResult = false;

	FILE *pFile = fopen(strFileName.c_str(), "wb");

    if( pFile )
    {
        TFileHeader Header;
        memset(&Header, 0, sizeof(Header));
        strcpy(Header.pMagic, MAGICWORD);
        Header.nRecords = m_PtrAssets.size();
        fwrite(&Header, sizeof(Header), 1, pFile);

        for(int i=0; i<m_PtrAssets.size(); i++)
        {
            m_PtrAssets[i]->Write(pFile);
        }

        fclose(pFile);
        bResult = true;
	}

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TAssetsArchive::Clear()
{
	for(int i=0; i<m_PtrAssets.size(); i++)
    {
		delete m_PtrAssets[i];
    }

	m_PtrAssets.clear();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TAssetsArchive::Add(TAsset* pAsset)
{
	assert(pAsset);

	m_PtrAssets.push_back(pAsset);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TAssetsArchive::Delete(int nIndex)
{
	if( (nIndex >= 0) && (nIndex < m_PtrAssets.size()) )
    {
		assert(m_PtrAssets[nIndex]);
        delete m_PtrAssets[nIndex];

		m_PtrAssets.erase( m_PtrAssets.begin() + nIndex );
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TAssetsArchive::Update(int nIndex, TAsset const& Asset)
{
	if( (nIndex >= 0) && (nIndex < m_PtrAssets.size()) )
    {
		//m_PtrAssets[nIndex] = Asset;
		*(m_PtrAssets[nIndex]) = Asset;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
TAsset* const TAssetsArchive::GetRecord(int nId)
{
	TAsset *pAsset = NULL;

	if( nId >=0 && nId < m_PtrAssets.size() )
    {
		pAsset = m_PtrAssets[nId];
    }

    return pAsset;
}

/*!****************************************************************************
* @brief
******************************************************************************/
unsigned TAssetsArchive::GetCount()
{
	return m_PtrAssets.size();
}

/*!****************************************************************************
* @brief
******************************************************************************/
TVecPtrAssets TAssetsArchive::GetAssets()
{
	return m_PtrAssets;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TAssetsArchive::GetRecordId(TAsset* pAsset, unsigned& nId)
{
	bool bResult = false;

	for(int i=0; i<m_PtrAssets.size(); i++)
    {
		if( m_PtrAssets[i] == pAsset )
        {
        	nId = i;

            bResult = true;
            break;
        }
    }

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByCity(TVecPtrAssets& PtrAssets, bool bAsc)
{
	if( PtrAssets.size() )
    {
        std::vector<TStringSort> Temp;

        for(int i=0; i<PtrAssets.size(); i++)
        {
			Temp.push_back(TStringSort(PtrAssets[i]->m_strCity, (void*) PtrAssets[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrAssets.size());

        TVecPtrAssets TempPtrAssets = PtrAssets;


        for(int i=0; i<Temp.size(); i++)
        {
            PtrAssets[i] = static_cast<TAsset*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrAssets.begin(), PtrAssets.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByValue(TVecPtrAssets& PtrAssets, bool bAsc)
{
	if( PtrAssets.size() )
    {
        std::vector<TIntegerSort> Temp;

        for(int i=0; i<PtrAssets.size(); i++)
        {
			Temp.push_back(TIntegerSort(PtrAssets[i]->m_nValue, (void*) PtrAssets[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrAssets.size());

        TVecPtrAssets TempPtrAssets = PtrAssets;

        for(int i=0; i<Temp.size(); i++)
        {
            PtrAssets[i] = static_cast<TAsset*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrAssets.begin(), PtrAssets.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByPrice(TVecPtrAssets& PtrAssets, bool bAsc)
{
	if( PtrAssets.size() )
    {
        std::vector<TIntegerSort> Temp;

        for(int i=0; i<PtrAssets.size(); i++)
        {
			Temp.push_back(TIntegerSort(PtrAssets[i]->m_nPrice, (void*) PtrAssets[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrAssets.size());

        TVecPtrAssets TempPtrAssets = PtrAssets;

        for(int i=0; i<Temp.size(); i++)
        {
            PtrAssets[i] = static_cast<TAsset*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrAssets.begin(), PtrAssets.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByArea(TVecPtrAssets& PtrAssets, bool bAsc)
{
	if( PtrAssets.size() )
    {
        std::vector<TIntegerSort> Temp;

        for(int i=0; i<PtrAssets.size(); i++)
        {
			Temp.push_back(TIntegerSort(PtrAssets[i]->m_nArea, (void*) PtrAssets[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrAssets.size());

        TVecPtrAssets TempPtrAssets = PtrAssets;

        for(int i=0; i<Temp.size(); i++)
        {
            PtrAssets[i] = static_cast<TAsset*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrAssets.begin(), PtrAssets.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByStatus(TVecPtrAssets& Assets, enAssetStatus nStatus)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

    for(int i=0; i<TempQuery.size(); i++)
    {
        if( TempQuery[i]->m_nStatus == nStatus )
        {
            Assets.push_back(TempQuery[i]);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByTypology(TVecPtrAssets& Assets, enAssetTypology nTypology)
{
    TVecPtrAssets TempQuery = Assets;

    Assets.clear();

    for(int i=0; i<TempQuery.size(); i++)
    {
        if( TempQuery[i]->m_nTypology == nTypology )
        {
            Assets.push_back(TempQuery[i]);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByCity(TVecPtrAssets& Assets, AnsiString strCityName, bool bFullMatch)
{
    TVecPtrAssets TempQuery = Assets;

    Assets.clear();

	if( bFullMatch )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strCity).LowerCase() == strCityName.LowerCase() )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
    else
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strCity).LowerCase().Pos( strCityName.LowerCase() ) )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByAddress(TVecPtrAssets& Assets, AnsiString strAddress, bool bFullMatch)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

	if( bFullMatch )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strAddress).LowerCase() == strAddress.LowerCase() )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
    else
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strAddress).LowerCase().Pos( strAddress.LowerCase() ) )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByBuildDate(TVecPtrAssets& Assets, double Date, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;

    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_BuildDate == Date)
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_BuildDate >= Date)
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_BuildDate <= Date)
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByEpgl(TVecPtrAssets& Assets, enAssetEpgl nEpgl, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;

    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nEpgl == nEpgl )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
    {
		for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nEpgl >= nEpgl )
            {
                Assets.push_back(TempQuery[i]);
            }
		}
    }
    else if( strIneq == "<=" )
    {
    	for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nEpgl <= nEpgl )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByValue(TVecPtrAssets& Assets, unsigned nValue, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nValue == nValue )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
	{
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nValue >= nValue )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nValue <= nValue )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByPrice(TVecPtrAssets& Assets, unsigned nPrice, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nPrice == nPrice )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
	{
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nPrice >= nPrice )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nPrice <= nPrice )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByRooms(TVecPtrAssets& Assets, unsigned nRooms, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nRooms == nRooms )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
	{
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nRooms >= nRooms )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nRooms <= nRooms )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByBathrooms(TVecPtrAssets& Assets, unsigned nBathrooms, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nBathrooms == nBathrooms )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
	{
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nBathrooms >= nBathrooms )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nBathrooms <= nBathrooms )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByArea(TVecPtrAssets& Assets, unsigned nArea, AnsiString strIneq)
{
    TVecPtrAssets TempQuery = Assets;
    Assets.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nArea == nArea )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
	{
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nArea >= nArea )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nArea <= nArea )
            {
                Assets.push_back(TempQuery[i]);
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString AsString(enAssetEpgl nEpgl)
{
	AnsiString strEpgl[] =
    	{ "none", "G", "F", "E", "D", "C", "B", "A4", "A3", "A2", "A1" };

    return strEpgl[nEpgl];
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString AsString(enAssetStatus nStatus)
{
	AnsiString strStatus[] =
    	{ "none", "for rent", "rented", "for sale", "sold out" };

    return strStatus[nStatus];
}


