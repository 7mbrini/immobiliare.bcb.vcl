/*!****************************************************************************

	@file	TFormMain.h
	@file	TFormMain.cpp

	@brief	Main form for the "Immobiliare" project Database Generator (dbgen)

	@noop	author:	Francesco Settembrini
	@noop	last update: 8/11/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/


#include <vcl.h>
#include <jpeg.hpp>
#pragma hdrstop

#include <string>
#include <cctype>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "cities.h"
#include "assets.h"
#include "customers.h"
#include "maths.h"

#include "TFormMain.h"
#include "TFormDebug.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;

//---------------------------------------------------------------------------

#define DATAPATH		"\\data\\"
#define ASSETSFILE		"assets.ast"
#define CUSTOMERSFILE	"customers.cst"

#define	DEFWIDTH        420
#define DEFHEIGHT       380

#define NRECORDS		100


/*!****************************************************************************
* @brief	Costruttore
******************************************************************************/
__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{
	;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    Setup();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
	Cleanup();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Setup()
{
	this->Left = 300;
    this->Top = 200;
    this->Width = DEFWIDTH;
    this->Height = DEFHEIGHT;
	this->Constraints->MinWidth = DEFWIDTH;
    this->Constraints->MaxWidth = DEFWIDTH;
    this->Constraints->MinHeight = DEFHEIGHT;
    this->Constraints->MaxHeight = DEFHEIGHT;


#ifndef _DEBUG
this->Position = poScreenCenter;
#endif

	FormDebug = new TFormDebug(NULL);
    assert(FormDebug);
    FormDebug->Left = this->Left + this->Width;
    FormDebug->Top = this->Top;
    FormDebug->Width = DEFWIDTH;
    FormDebug->Height = this->Height;
    FormDebug->Constraints->MinWidth = DEFWIDTH;
    FormDebug->Constraints->MinHeight = DEFHEIGHT;
#ifdef _DEBUG
	FormDebug->Visible = true;
#else
	FormDebug->Visible = false;
#endif

    EditNumRecords->Text = NRECORDS;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Cleanup()
{
	delete FormDebug;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Exit1Click(TObject *Sender)
{
	PostQuitMessage(0);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ButtonGetAssetsClick(TObject *Sender)
{
	try
    {
        unsigned nRecords = EditNumRecords->Text.ToInt();

        GenAssetsDB(nRecords);
	}
    catch(Exception& Err)
    {
		::ShowMessage(Err.ToString());
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ButtonGetCustomersClick(TObject *Sender)
{

	try
    {
        unsigned nRecords = EditNumRecords->Text.ToInt();

        GenCustomersDB(nRecords);
	}
    catch(Exception& Err)
    {
		::ShowMessage(Err.ToString());
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString __fastcall TFormMain::GetDataPath()
{
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
void __fastcall TFormMain::Load(TVecCities& Cities)
{
    AnsiString strDataPath = GetDataPath();

    TCitiesArchive *pCities = new TCitiesArchive();
    assert(pCities);

    AnsiString strFileName = strDataPath + "comuni.csv";
    pCities->LoadFromFile(strFileName);

    assert(pCities->GetCount());
    pCities->GetCities(Cities);

	delete pCities;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Print(TVecCities& Cities, int nLimits)
{
    AnsiString strTemp;

    int nCount = nLimits > 0 ? nLimits : Cities.size();

    for(int i=0; i<nCount; i++)
    {
        TCity City = Cities[i];

        strTemp.sprintf(
            "\n Name: %s"
            "\n Prov: %s"
            "\n CAP: %s"
            "\n Region: %s",
            City.m_strName.c_str(),
            City.m_strProv.c_str(),
            City.m_strCap.c_str(),
            City.m_strRegion.c_str()
        );

        FormDebug->Print(strTemp);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Load(TVecRoads& Roads)
{
    AnsiString strDataPath = GetDataPath();

    TRoadsArchive *pRoads = new TRoadsArchive();
    assert(pRoads);

    AnsiString strFileName = strDataPath + "stradario.csv";

    pRoads->LoadFromFile(strFileName);
    assert(pRoads->GetCount());

	TVecRoads Temp;
    pRoads->GetRoads(Temp);

    									// prende solo le "vie", "viali" e "piazze"
	for(int i=0; i<Temp.size(); i++)
    {
		AnsiString strName = Temp[i].m_strName;

		if(
        	strstr(strName.LowerCase().c_str(), "via")
        	|| strstr(strName.LowerCase().c_str(), "viale")
        	|| strstr(strName.LowerCase().c_str(), "piazza")
        	|| strstr(strName.LowerCase().c_str(), "piazzale")
        )
        {
			Roads.push_back(Temp[i]);
        }
    }

    for(int i=0; i<Roads.size(); i++)
	{
    	Roads[i].m_strName = Capitalize(Roads[i].m_strName);
    }

	delete pRoads;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Print(TVecRoads& Roads, int nLimits)
{
    AnsiString strTemp;

    int nCount = nLimits > 0 ? nLimits : Roads.size();

    for(int i=0; i<nCount; i++)
    {
        TRoad Road = Roads[i];

        strTemp.sprintf(
            "\n Name: %s"
            "\n Code: %d",
            Road.m_strName.c_str(),
            Road.m_nCode
        );

        FormDebug->Print(strTemp);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Load(TVecFirstNames& FirstNames)
{
    AnsiString strDataPath = GetDataPath();

    TFirstNamesArchive *pFirstNames = new TFirstNamesArchive();
    assert(pFirstNames);

    AnsiString strFileName = strDataPath + "nomi.csv";
    pFirstNames->LoadFromFile(strFileName);

    assert(pFirstNames->GetCount());
    pFirstNames->GetFirstNames(FirstNames);

    for(int i=0; i<FirstNames.size(); i++)
	{
    	FirstNames[i].m_strName = Capitalize(FirstNames[i].m_strName);
    }

	delete pFirstNames;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Print(TVecFirstNames& FirstNames, int nLimits)
{
    AnsiString strTemp;

    int nCount = nLimits > 0 ? nLimits : FirstNames.size();

    for(int i=0; i<nCount; i++)
    {
        TFirstName FirstName = FirstNames[i];

        strTemp.sprintf(
            "\n Name: %s"
            "\n Freq: %d",
            FirstName.m_strName.c_str(),
            FirstName.m_nFreq
        );

        FormDebug->Print(strTemp);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString __fastcall TFormMain::Capitalize(AnsiString strString)
{
	strString = strString.LowerCase();

    std::vector<int> VecBlanks;
    int nLen = strString.Length();

	strString.c_str()[0] = toupper( strString.c_str()[0] );

											// in caso di nomi composti
                                            // es. Gian Giacomo, ecc...
    for(int i=0; i<nLen; i++)
    {
		if( strString.c_str()[i] == ' ' )
        {
        	VecBlanks.push_back(i);
        }
    }

	if(VecBlanks.size())
    {
		for(int i=0; i<VecBlanks.size(); i++)
        {
			strString.c_str()[ VecBlanks[i] + 1] =
            	toupper( strString.c_str()[ VecBlanks[i] + 1]);
        }
    }

	return strString;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Load(TVecLastNames& LastNames)
{
    AnsiString strDataPath = GetDataPath();

    TLastNamesArchive *pLastNames = new TLastNamesArchive();
    assert(pLastNames);

    AnsiString strFileName = strDataPath + "cognomi.csv";
    pLastNames->LoadFromFile(strFileName);
    assert(pLastNames->GetCount());

	pLastNames->GetLastNames(LastNames);

    for(int i=0; i<LastNames.size(); i++)
	{
    	LastNames[i].m_strName = Capitalize(LastNames[i].m_strName);
    }

	delete pLastNames;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Print(TVecLastNames& LastNames, int nLimits)
{
    AnsiString strTemp;

    int nCount = nLimits > 0 ? nLimits : LastNames.size();

    for(int i=0; i<nCount; i++)
    {
        TLastName LastName = LastNames[i];

        strTemp.sprintf(
            "\n Name: %s"
            "\n Freq: %d",
            LastName.m_strName.c_str(),
            LastName.m_nFreq
        );

        FormDebug->Print(strTemp);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString __fastcall TFormMain::Replace(AnsiString strString, char nChar, char nReplace)
{
	for(int i=0; i<strString.Length(); i++)
    {
    	if( strString.c_str()[i] == nChar )
        {
        	strString.c_str()[i] = nReplace;
        }
    }

    return strString;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::AssetLoadDescr(AnsiString strDescrFile, TAsset& Asset)
{
	if( ::FileExists(strDescrFile) )
    {
		TStringList *pList = new TStringList;
        assert(pList);

        pList->LoadFromFile(strDescrFile);

        AnsiString strText = pList->GetText();

        strncpy(Asset.m_strDescription, strText.c_str(), ASSET_DESCRMAXLEN - 1);

        delete pList;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::AssetLoadImage(AnsiString strImageFile, TAsset& Asset)
{
	if( ::FileExists(strImageFile) )
    {
		TJPEGImage *pJpeg = new TJPEGImage;
        assert(pJpeg);

		pJpeg->LoadFromFile(strImageFile);

        TMemoryStream *pMS = new TMemoryStream;
        assert(pMS);

		pJpeg->SaveToStream(pMS);

        pMS->Position = 0L;
        unsigned nSize = pMS->Size;

        if( nSize <= ASSET_IMAGEMAXSIZE )
        {
			pMS->Read(Asset.m_pImage, nSize);
			Asset.m_bImage = true;
        }

		delete pMS;
        delete pJpeg;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::LoadAssetsMap(TMapFileList& Map,
	enAssetTypology nTypology, AnsiString strDir, int nImagesCount, int nDescrCount)
{
    TFileList List;
	List.m_nTypology = nTypology;

    {
                                                // immagini
        List.m_nImgCount = nImagesCount;

        for(int i=0; i<nImagesCount; i++)
        {
            AnsiString strFileName;
            strFileName.sprintf("%02d-xxs-c.jpg", i);

            AnsiString strFilePath = strDir + strFileName;
            assert(::FileExists(strFilePath));

            List.m_ImgList.push_back(strFilePath.c_str());
        }
    }

    {
                                                // descrizioni
        List.m_nDescrCount = nDescrCount;

        for(int i=0; i<nDescrCount; i++)
        {
            AnsiString strFileName;
            strFileName.sprintf("%02d-descr.txt", i);

            AnsiString strFilePath = strDir + strFileName;
            assert(::FileExists(strFilePath));

            List.m_DescrList.push_back(strFilePath.c_str());
        }
    }

    Map.insert(std::make_pair(nTypology, List));
}

/*!****************************************************************************
* @brief	Generate Customers DB
******************************************************************************/
void __fastcall TFormMain::GenCustomersDB(unsigned nRecords)
{
											// carica la lista dei nomi
	TVecFirstNames FirstNames;
    Load(FirstNames);
	//Print(FirstNames, 5);
											// carica la lista dei cognomi
	TVecLastNames LastNames;
    Load(LastNames);
	//Print(LastNames, 5);
											// carica la lista delle citta
	TVecCities Cities;
    Load(Cities);
	//Print(Cities, 5);
											// carica la lista della strade
	TVecRoads Roads;
    Load(Roads);
	//Print(Roads, 5);

    //unsigned nRecords = NRECORDS;
	::srand(::GetCurrentTime());

	TCustomersArchive *pCustomers = new TCustomersArchive;
    assert(pCustomers);

    for(int i=0; i<nRecords; i++)
    {
		//TCustomer Customer;
        TCustomer *pCustomer = new TCustomer;
        assert(pCustomer);

		AnsiString strFirstName = FirstNames[ maths::Rand(0, FirstNames.size() - 1) ].m_strName;
        AnsiString strLastName = LastNames[ maths::Rand(0, LastNames.size() - 1) ].m_strName;

											// first name
		strncpy(pCustomer->m_strFirstName, strFirstName.c_str(), CUSTOMER_NAMESMAXLEN - 1);

											// last name
        strncpy(pCustomer->m_strLastName, strLastName.c_str(), CUSTOMER_NAMESMAXLEN - 1);

											// address
		strncpy(pCustomer->m_strAddress,
        	AnsiString(Roads[ maths::Rand(0, Roads.size() - 1) ].m_strName + ", "
            	+ AnsiString( maths::Rand(1, 300))).c_str(),
            CUSTOMER_NAMESMAXLEN - 1);

											// city
		{
            TCity City = Cities[ maths::Rand(0, Cities.size() -1) ];
            strncpy(pCustomer->m_strCity, Capitalize(City.m_strName).c_str(),
                CUSTOMER_NAMESMAXLEN - 1);

            pCustomer->m_nCap = City.m_strCap.ToInt();
        }
											// email
		{
			const int nProviders =3;
			AnsiString strProviders[nProviders] =
            {
				"@gmail.com", "@yahoo.com", "@outlook.com"
            };


        	AnsiString strMail = Replace(strLastName, ' ', '_').LowerCase()
            	+ "." + Replace(strFirstName, ' ', '_').LowerCase();

            strMail += strProviders[ maths::Rand(0, nProviders - 1) ];

            strncpy(pCustomer->m_strEmail, strMail.c_str(), CUSTOMER_NAMESMAXLEN - 1);
        }

												// phone
		{
			const int nPrefixes = 10;
            AnsiString strPrefixes[nPrefixes] =
            {
				"330", "337", "339", "320", "324",
                "340", "343", "349", "347", "383"
            };

			AnsiString strPhone = strPrefixes[ maths::Rand(0, nPrefixes - 1) ]
            	+ int(maths::AbsRand(999999.0));

            strncpy(pCustomer->m_strPhone, strPhone.c_str(), CUSTOMER_NAMESMAXLEN - 1);

        }

											// budget
		{
            const unsigned nBudgets = 28;
            unsigned Budgets[nBudgets] =
            {
                100, 200, 500, 750,
                1000, 2000, 3000, 5000,
                10000, 25000, 50000, 75000,
                100000, 125000, 150000, 175000,
                200000, 250000, 250000, 275000,
                300000, 325000, 350000, 375000,
                500000, 650000, 750000, 1000000
            };

			pCustomer->m_nBudget = Budgets[maths::Rand(0, nBudgets - 1)];
			assert(pCustomer->m_nBudget > 0);
		}
											// ownership
        pCustomer->m_nOwnership = maths::RandSign() > 0 ? cotForRent : cotForPurchase;


        pCustomers->Add(pCustomer);
    }

	assert(pCustomers->GetCount() == nRecords);

/*
for(int i=0; i<5; i++)
{
    TCustomer Customer;
    pCustomers->GetRecord(i, Customer);
    FormDebug->Print( Customer.AsString() );
}
*/

    AnsiString strCustomersFile = GetDataPath() + CUSTOMERSFILE;

/*
FormDebug->Print(AnsiString("\n"));
FormDebug->Print(strCustomersFile);
*/

    pCustomers->SaveToFile(strCustomersFile);

    delete pCustomers;

    ::ShowMessage("Customers database done!");
}

/*!****************************************************************************
* @brief
******************************************************************************/
TDate __fastcall TFormMain::RandDate(TDate Start, TDate End)
{
	unsigned StartVal = Start.Val;
    unsigned EndVal = End.Val;

    assert(StartVal <= EndVal);

    unsigned RandVal = maths::Rand(StartVal, EndVal);

    return TDate(double(RandVal));
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::GenAssetsDB(unsigned nRecords)
{
											// lista delle citta
	TVecCities Cities;
    Load(Cities);
	//Print(Cities, 5);
											// lista della strade
	TVecRoads Roads;
    Load(Roads);
	//Print(Roads, 5);

											// carica le liste degli assets
	TMapFileList AssetsMap;
    AnsiString strPath = GetDataPath() + "immobiliare\\";

	LoadAssetsMap(AssetsMap, atTerrain, strPath + "\\terreni\\", 20, 10);
	LoadAssetsMap(AssetsMap, atGarage, strPath + "\\garages\\", 20, 10);
	LoadAssetsMap(AssetsMap, atVilla, strPath + "\\ville\\", 20, 10);
    LoadAssetsMap(AssetsMap, atApartment, strPath + "\\appartamenti\\", 100, 10);
    LoadAssetsMap(AssetsMap, atOffice, strPath + "\\uffici\\", 20, 10);
    LoadAssetsMap(AssetsMap, atCommercial, strPath + "\\negozi\\", 20, 10);
    LoadAssetsMap(AssetsMap, atIndustrial, strPath + "\\capannoni\\", 20, 10);


											// genera il database degli "assets"
	//unsigned nRecords = NRECORDS;
	::srand(::GetCurrentTime());

	TAssetsArchive *pAssets = new TAssetsArchive();
    assert(pAssets);

    for(int i=0; i<nRecords; i++)
    {
		//TAsset Asset;					// no!	Aumentare la dimensione dello stack
       									//		Oppure allocare nello heap!

        TAsset *pAsset = new TAsset;
        assert(pAsset);

											// typology
        enAssetTypology nTypology = maths::Rand(atTerrain, atIndustrial);
		pAsset->m_nTypology = nTypology;

											// build date
		if( pAsset->m_nTypology > atTerrain )
        {
            pAsset->m_BuildDate = RandDate(TDate(1990, 1,1), TDate(1999,1,1)).Val;
        }

											// status
		{
            pAsset->m_nStatus = maths::Rand(asForRent, asSoldOut);

            if( pAsset->m_nStatus == asSoldOut )
            {
				pAsset->m_SoldDate = RandDate(TDate(2000, 1, 1), TDate::CurrentDate()).Val;
            }
		}
                                        	// load description field
		{
            TFileList FileList = AssetsMap.find(enAssetTypology(nTypology))->second;

            int nDescrId = maths::Rand(0, FileList.m_nDescrCount - 1);
            AnsiString strDescrFile = FileList.m_DescrList[nDescrId].c_str();
            assert(::FileExists(strDescrFile));

            AssetLoadDescr(strDescrFile, *pAsset);

                                            // load image field
            int nImageId = maths::Rand(0, FileList.m_nImgCount - 1);
            AnsiString strImageFile = FileList.m_ImgList[nImageId].c_str();
            assert(::FileExists(strImageFile));
            AssetLoadImage(strImageFile, *pAsset);
        }

											// address
		strncpy(pAsset->m_strAddress,
        	AnsiString(Roads[ maths::Rand(0, Roads.size() - 1) ].m_strName + ", "
            	+ AnsiString( maths::Rand(1, 300))).c_str(),
            CUSTOMER_NAMESMAXLEN - 1);

											// city
		{
            TCity City = Cities[ maths::Rand(0, Cities.size() -1) ];
            strncpy(pAsset->m_strCity, Capitalize(City.m_strName).c_str(),
                CUSTOMER_NAMESMAXLEN - 1);

            pAsset->m_nCap = City.m_strCap.ToInt();
        }

											// price, rooms, etc ...
                                            // depends on the typology
		switch( pAsset->m_nTypology )
        {
			case atTerrain:
                pAsset->m_nRooms = 0;
				pAsset->m_nBathrooms = 0;
				pAsset->m_nEpgl = aeNone;
                pAsset->m_nArea = 10000 * maths::Rand(1, 10);
            	pAsset->m_nValue = 10000 * maths::Rand(1, 10);
			break;

            case atGarage:
            	pAsset->m_nRooms = maths::Rand(1,2);
                pAsset->m_nBathrooms = maths::Rand(0,1);
				pAsset->m_nEpgl = maths::Rand(aeG, aeE);
                pAsset->m_nArea = 5 * maths::Rand(5, 20);
                pAsset->m_nValue = 5000 * maths::Rand(5, 10);
            break;

            case atVilla:
            	pAsset->m_nRooms = maths::Rand(5,20);
                pAsset->m_nBathrooms = maths::Rand(1,3);
				pAsset->m_nEpgl = maths::Rand(aeA4, aeA1);
                pAsset->m_nArea = 100 * maths::Rand(1, 10);
                pAsset->m_nValue = 10000 * maths::Rand(5, 100);
            break;

            case atOffice:
            case atApartment:
			case atCommercial:
            	pAsset->m_nRooms = maths::Rand(3,5);
                pAsset->m_nBathrooms = maths::Rand(1,2);
				pAsset->m_nEpgl = maths::Rand(aeD, aeA1);
                pAsset->m_nArea = 10 * maths::Rand(5, 15);
                pAsset->m_nValue = 10000 * maths::Rand(5, 50);
            break;

			case atIndustrial:
            	pAsset->m_nRooms = maths::Rand(5, 20);
                pAsset->m_nBathrooms = maths::Rand(0,2);
				pAsset->m_nEpgl = maths::Rand(aeG, aeD);
                pAsset->m_nArea = 1000 * maths::Rand(5, 10);
                pAsset->m_nValue = 10000 * maths::Rand(10, 500);
            break;
        }

		if( pAsset->m_nStatus == asForRent )
        {
			pAsset->m_nPrice = ceil(pAsset->m_nValue / 1000.0);
        }
		else if( (pAsset->m_nStatus == asForSale) || (pAsset->m_nStatus == asSoldOut) )
        {
			pAsset->m_nPrice = ceil(pAsset->m_nValue * 1.5);
        }

        pAssets->Add(pAsset);
    }

    AnsiString strAssetsFile = GetDataPath() + ASSETSFILE;
    pAssets->SaveToFile(strAssetsFile);

    delete pAssets;

    ::ShowMessage("Assets database done!");
}

//---------------------------------------------------------------------------


