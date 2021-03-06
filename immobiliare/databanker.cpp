/*!****************************************************************************

	@file	databanker.h
	@file	databanker.cpp

	@brief	Database read/write routines

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "utils.h"
#include "databanker.h"
#include "commdefs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
TDataBanker::TDataBanker()
{
	m_pCities = NULL;
    m_pAssets = NULL;
    m_pCustomers = NULL;
}

/*!****************************************************************************
* @brief	Destructor
******************************************************************************/
TDataBanker::~TDataBanker()
{
    if( m_pCities ) delete m_pCities;
	if( m_pAssets ) delete m_pAssets;
    if( m_pCustomers ) delete m_pCustomers;
}

/*!****************************************************************************
* @brief	Destructor
******************************************************************************/
void __fastcall TDataBanker::CitiesCreateArchive()
{
	if( m_pCities ) delete m_pCities;

    m_pCities = new TCitiesArchive();
    assert(m_pCities);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TDataBanker::AssetsCreateArchive()
{
	if( m_pAssets ) delete m_pAssets;

	m_pAssets = new TAssetsArchive();
    assert(m_pAssets);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TDataBanker::CustomersCreateArchive()
{
	if( m_pCustomers ) delete m_pCustomers;

	m_pCustomers = new TCustomersArchive();
    assert(m_pCustomers);
}

/*!****************************************************************************
* @brief
******************************************************************************/
TCitiesArchive* const __fastcall TDataBanker::GetCities()
{
	return m_pCities;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TDataBanker::GetCities(TVecCities& Cities)
{
	assert(m_pCities);

    m_pCities->GetCities(Cities);
}

/*!****************************************************************************
* @brief
******************************************************************************/
TAssetsArchive* const __fastcall TDataBanker::GetAssets()
{
	return m_pAssets;
}

/*!****************************************************************************
* @brief
******************************************************************************/
TCustomersArchive* const __fastcall TDataBanker::GetCustomers()
{
	return m_pCustomers;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TDataBanker::CitiesLoadFromFile(AnsiString strFileName)
{
	if( m_pCities ) delete m_pCities;

    m_pCities = new TCitiesArchive();
    assert(m_pCities);

	return m_pCities->LoadFromFile(strFileName);
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TDataBanker::AssetsLoadFromFile(AnsiString strFileName)
{
	if( m_pAssets ) delete m_pAssets;

    m_pAssets = new TAssetsArchive();
    assert(m_pAssets);

    return m_pAssets->LoadFromFile(strFileName);
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TDataBanker::CustomersLoadFromFile(AnsiString strFileName)
{
	if( m_pCustomers ) delete m_pCustomers;

    m_pCustomers = new TCustomersArchive();
    assert(m_pCustomers);

    return m_pCustomers->LoadFromFile(strFileName);
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TDataBanker::AssetsSaveToFile(AnsiString strFileName)
{
	bool bResult = false;

	if( m_pAssets )
    {
		bResult = m_pAssets->SaveToFile(strFileName);
    }

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TDataBanker::CustomersSaveToFile(AnsiString strFileName)
{
	bool bResult = false;

	if( m_pCustomers )
    {
    	bResult = m_pCustomers->SaveToFile(strFileName);
    }

    return bResult;
}


