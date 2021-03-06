/*!****************************************************************************

	@file	TFormQuerySalesHist.h
	@file	TFormQuerySalesHist.cpp

	@brief	Search for "Supply and Demand" of Assets

	@noop	author:	Francesco Settembrini
	@noop	last update: 8/11/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "TFormMain.h"
#include "TFormDebug.h"
#include "TFormAssets.h"
#include "TFormCustomers.h"
#include "TFormQuerySupplyAndDemand.h"

#include "commdefs.h"
#include "errormsgs.h"
#include "statistics.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//TFormQuerySalesHist *FormQuerySalesHist;


#define WIDTH		940
#define HEIGHT		540
#define MINWIDTH	820
#define MINHEIGHT	480

#define ASSETS_DEFCOLS	13
#define ASSETS_DEFROWS	2
#define ASSETS_DEFCOLW	48
#define ASSETS_DEFCOLH  16

#define CUSTOMERS_DEFCOLS	9
#define CUSTOMERS_DEFROWS	2
#define CUSTOMERS_DEFCOLW 	48
#define CUSTOMERS_DEFCOLH	16


struct TOwnershipChoice
{
	AnsiString strName;
    enCustomerOwnershipType nType;
} static g_OwnershipChoices[] =
{
	{"", cotNone},
    {" for rent", cotForRent},
    {" for purchase", cotForPurchase}
};

enum enCustomerSortingMode { csmNone = 0,
    csmCityAsc, csmCityDesc,
    csmBudgetAsc, csmBudgetDesc };

struct TCustomerSortingChoice
{
	AnsiString strName;
    enCustomerSortingMode nMode;
} static g_CustomerSortingChoices[] =
{
	{"", csmNone},
	{" city, ascending", csmCityAsc},
	{" city, decending", csmCityDesc},
    {" budget, ascending", csmBudgetAsc},
    {" budget, descending", csmBudgetDesc}
};


struct TAssetTypeChoice
{
	AnsiString strName;
    enAssetTypology nTypology;
} static g_AssetTypologyChoices[] =
{
	{"", atNone},
    {"terreno", atTerrain},
    {"garage", atGarage},
    {"villa", atVilla},
    {"appartamento", atApartment},
    {"ufficio", atOffice},
    {"negozio", atCommercial},
    {"capannone", atIndustrial}
};

enum enAssetSortingMode { asmNone = 0,
	asmCityAsc, asmCityDesc, asmPriceAsc, asmPriceDesc };

struct TAssetSortingChoice
{
	AnsiString strName;
    enAssetSortingMode nMode;
} static g_AssetsSortingChoices[] =
{
	{"", asmNone},
	{" city, ascending", asmCityAsc},
	{" city, decending", asmCityDesc},
    {" price, ascending", asmPriceAsc},
    {" price, descending", asmPriceDesc}
};


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
__fastcall TFormQuerySupplyAndDemand::TFormQuerySupplyAndDemand(TComponent* Owner,
	TDataBanker* pDB) : TForm(Owner)
{
	assert(pDB);

    m_pDB = pDB;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::FormCreate(TObject *Sender)
{
	Setup();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::Setup()
{
	this->Caption = AnsiString(APPNAME) + " - Analisi della domanda e dell' offerta";

    this->Width = WIDTH;
    this->Height = HEIGHT;
    this->Constraints->MinWidth = MINWIDTH;
    this->Constraints->MinHeight = MINHEIGHT;

	this->Position = poScreenCenter;

	this->LabelAssets->Caption = "Double Click on listed items for details";
	this->LabelCustomers->Caption = "Click on listed items for Assets Query, Double Click for details";

										// build the Assets and Customers grids
    SetupTheAssetsGrid();
    SetupTheCustomersGrid();

										// build the "ownership" choice combobox
	for(int i=0; i<sizeof(g_OwnershipChoices)/sizeof(g_OwnershipChoices[0]); i++)
    {
		ComboBoxOwnership->Items->Add( g_OwnershipChoices[i].strName );
    }
	ComboBoxOwnership->ItemIndex = 0;

										// build the "budget" combobox
    ComboBoxBudget->Items->Add("");
	for(int i=100; i<1000; i+=100) { ComboBoxBudget->Items->Add(i); }
	for(int i=1000; i<5000; i+= 500) { ComboBoxBudget->Items->Add(i); }
	for(int i=5000; i<50000; i+=5000) { ComboBoxBudget->Items->Add(i); }
	for(int i=50000; i<=1000000; i+=50000) { ComboBoxBudget->Items->Add(i); }
    ComboBoxBudget->ItemIndex = 0;

										// build customers "sorting" mode combobox
	for(int i=0; i<sizeof(g_CustomerSortingChoices)/sizeof(g_CustomerSortingChoices[0]); i++)
    {
		ComboBoxCustomersSorting->Items->Add( g_CustomerSortingChoices[i].strName );
    }
	ComboBoxCustomersSorting->ItemIndex = 0;

										// build the "typology" combobox
    for(int i=0; i<sizeof(g_AssetTypologyChoices)/sizeof(g_AssetTypologyChoices[0]); i++)
    {
		ComboBoxTypology->Items->Add(g_AssetTypologyChoices[i].strName);
    }
	ComboBoxTypology->ItemIndex = atNone;

										// regions comboboxes
	{
		assert(m_pDB);

        TCitiesArchive *pCities = m_pDB->GetCities();
        assert(pCities);

        TVecCities Cities;
        pCities->GetCities(Cities);

        for(int i=0; i<Cities.size(); i++)
        {
			m_MapCapRegions[Cities[i].m_strCap.c_str()] = Cities[i].m_strRegion.c_str();
        }

    	TVecStrings Regions;
        pCities->GetRegions(Regions);

        ComboBoxAssetsRegion->Sorted = true;
        ComboBoxCustomersRegion->Sorted = true;
		ComboBoxAssetsRegion->Items->Add("");
        ComboBoxCustomersRegion->Items->Add("");

        for(int i=0; i<Regions.size(); i++)
        {
        	ComboBoxAssetsRegion->Items->Add(Regions[i].c_str());
            ComboBoxCustomersRegion->Items->Add(Regions[i].c_str());
        }
        ComboBoxAssetsRegion->ItemIndex = 0;
		ComboBoxCustomersRegion->ItemIndex = 0;
    }

										// ordering choice combobox
	for(int i=0; i<sizeof(g_AssetsSortingChoices)/sizeof(g_AssetsSortingChoices[0]); i++)
    {
		ComboBoxAssetsSorting->Items->Add( g_AssetsSortingChoices[i].strName);
    }
	ComboBoxAssetsSorting->ItemIndex = asmNone;

}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::ButtonCloseClick(TObject *Sender)
{
	ModalResult = mrOk;
    Close();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::FormCanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize)
{
	PanelCustomers->Left = 0;
    PanelCustomers->Top = 0;
    PanelCustomers->Width = PanelClient->Width;
    PanelCustomers->Height = PanelClient->Height/2;

    PanelAssets->Left = 0;
    PanelAssets->Top = PanelClient->Height/2;
    PanelAssets->Width = PanelClient->Width;
    PanelAssets->Height = PanelClient->Height/2;

	ButtonClose->Left = (this->Width - ButtonClose->Width)/2;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::ButtonUpdateClick(TObject *Sender)
{
	Update();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::Update()
{
    ClearAssetsGrid();

	m_QueryAssets.clear();
    m_QueryCustomers.clear();

	if( ValidateInput() )
    {
		UpdateCustomers();
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::ClearAssetsGrid()
{
	GridAssets->RowCount = ASSETS_DEFROWS;

    for(int i=0; i<=GridAssets->ColCount; i++)
    {
		GridAssets->Cells[i][1] = "";
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::ClearCustomersGrid()
{
	GridCustomers->RowCount = CUSTOMERS_DEFROWS;

    for(int i=0; i<=GridCustomers->ColCount; i++)
    {
		GridCustomers->Cells[i][1] = "";
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::Clear()
{
	ComboBoxOwnership->ItemIndex = 0;
	ComboBoxTypology->ItemIndex = 0;
    ComboBoxAssetsRegion->ItemIndex = 0;
	ComboBoxCustomersRegion->ItemIndex = 0;
	ComboBoxAssetsSorting->ItemIndex = 0;
	ComboBoxCustomersSorting->ItemIndex = 0;
    ComboBoxBudget->Text = "";

	ClearAssetsGrid();
    ClearCustomersGrid();
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TFormQuerySupplyAndDemand::ValidateInput()
{
	bool bResult = true;

	;

	return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::UpdateCustomers()
{
	assert(m_pDB);
    TCustomersArchive* pCustomers = m_pDB->GetCustomers();
    assert(pCustomers);

    m_QueryCustomers = pCustomers->GetCustomers();

											// filters the customers
    if( m_QueryCustomers.size() )
    {
        									// filter by ownership
        enCustomerOwnershipType nOwnership = g_OwnershipChoices[ ComboBoxOwnership->ItemIndex ].nType;
        if( nOwnership != cotNone )
        {
			FilterByOwnership(m_QueryCustomers, nOwnership);
        }
											// filter by budget
        if( ComboBoxBudget->Text.Length() )
        {
			int nBudget = ComboBoxBudget->Text.ToInt();
            FilterByBudget(m_QueryCustomers, nBudget, "<=");
        }
											// filter by region
		if( ComboBoxCustomersRegion->Text.Length() )
        {
            TVecPtrCustomers TempQuery = m_QueryCustomers;

            m_QueryCustomers.clear();

            for(int i=0; i<TempQuery.size(); i++)
            {
				if( m_MapCapRegions[ TempQuery[i]->m_nCap ].LowerCase() ==
					ComboBoxCustomersRegion->Text.LowerCase() )
                {
					m_QueryCustomers.push_back(TempQuery[i]);
                }
            }
        }
	}

											// sort (eventually) the Customers
	if( m_QueryCustomers.size() )
    {
		if( ComboBoxCustomersSorting->Text.Length() )
        {
            switch( g_CustomerSortingChoices[ComboBoxCustomersSorting->ItemIndex].nMode )
            {
                case csmCityAsc: SortByCity(m_QueryCustomers, true); break;
                case csmCityDesc: SortByCity(m_QueryCustomers, false); break;
                case csmBudgetAsc: SortByBudget(m_QueryCustomers, true); break;
                case csmBudgetDesc: SortByBudget(m_QueryCustomers, false); break;
            }
		}

        PrintCustomers(m_QueryCustomers);
	}

    AnsiString strTemp;
    strTemp.sprintf("total: %d records", m_QueryCustomers.size());
    LabelCustomersResults->Caption = strTemp;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::PrintAssets(TVecPtrAssets& Assets)
{
	AnsiString strTemp;
	int nRows = Assets.size();
    GridAssets->RowCount = nRows + 1;

    for(int i=0; i<nRows; i++)
    {
		TAsset* pAsset = Assets[i];
        assert(pAsset);

											// row number
		GridAssets->Cells[0][i+1] = i+1;

											// typology
		GridAssets->Cells[1][i+1] = g_AssetTypologyChoices[pAsset->m_nTypology].strName;

											// status
		GridAssets->Cells[2][i+1] = AsString(pAsset->m_nStatus);

											// price
        GridAssets->Cells[3][i+1] = pAsset->m_nPrice;

											// city
		GridAssets->Cells[4][i+1] = pAsset->m_strCity;

											// address
		GridAssets->Cells[5][i+1] = pAsset->m_strAddress;

    	if( pAsset->m_nTypology > atTerrain )
        {
	    									// epgl
			GridAssets->Cells[6][i+1] = AsString(pAsset->m_nEpgl);

											// rooms
			GridAssets->Cells[7][i+1] = pAsset->m_nRooms;

        									// bathrooms
			GridAssets->Cells[8][i+1] = pAsset->m_nBathrooms;
        }
        else
        {
        	GridAssets->Cells[6][i+1] = "";
	    	GridAssets->Cells[7][i+1] = "";
	    	GridAssets->Cells[8][i+1] = "";
        }
        									// area
		GridAssets->Cells[9][i+1] = pAsset->m_nArea;

											// building date
        if( pAsset->m_nTypology > atTerrain )
		{
			TDate Date(double(pAsset->m_BuildDate));
            unsigned short nYear = 0, nMonth = 0, nDay = 0;
            Date.DecodeDate(&nYear, &nMonth, &nDay);

            AnsiString strTemp;
            strTemp.sprintf("%02d/%02d/%04d", nDay, nMonth, nYear);

            GridAssets->Cells[10][i+1] = strTemp;
		}
		else
        {
			GridAssets->Cells[10][i+1] = "";
        }
        									// sold out date
		if( pAsset->m_nStatus == asSoldOut )
        {
			TDate Date(double(pAsset->m_SoldDate));
            unsigned short nYear = 0, nMonth = 0, nDay = 0;
            Date.DecodeDate(&nYear, &nMonth, &nDay);

            AnsiString strTemp;
            strTemp.sprintf("%02d/%02d/%04d", nDay, nMonth, nYear);

            GridAssets->Cells[11][i+1] = strTemp;
        }
        else
        {
			GridAssets->Cells[11][i+1] = "";
        }

											// description
        GridAssets->Cells[12][i+1] = pAsset->m_strDescription;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::SetupTheAssetsGrid()
{
	int nRows = ASSETS_DEFROWS;
	int nCols = ASSETS_DEFCOLS;

	GridAssets->ScrollBars = ssBoth;
	GridAssets->DrawingStyle = gdsClassic;	// vip, altrimenti non imposta i colori

	GridAssets->DoubleBuffered = true;

	GridAssets->RowCount = nRows;
    GridAssets->ColCount = nCols;
	GridAssets->FixedRows = 1;
    GridAssets->FixedCols = 1;

	GridAssets->DefaultColWidth = ASSETS_DEFCOLW;
    GridAssets->DefaultRowHeight = ASSETS_DEFCOLH;

    GridAssets->Options << goColSizing << goRowSizing
    	<< goDrawFocusSelected << goRangeSelect
        << goRowMoving << goColMoving << goRowSelect;

	GridAssets->Options >> goEditing;


	GridAssets->Font->Name = "tahoma";
    GridAssets->Font->Size = 10;

    GridAssets->FixedColor = clActiveCaption;
    GridAssets->Color = clMoneyGreen;

	GridAssets->Cells[0][0] = "";
	GridAssets->Cells[1][0] = "typology";
	GridAssets->Cells[2][0] = "status";
    GridAssets->Cells[3][0] = "price";
    GridAssets->Cells[4][0] = "city";
	GridAssets->Cells[5][0] = "address";
    GridAssets->Cells[6][0] = "epgl";
    GridAssets->Cells[7][0] = "rooms";
    GridAssets->Cells[8][0] = "baths";
    GridAssets->Cells[9][0] = "area";
	GridAssets->Cells[10][0] = "build date";
    GridAssets->Cells[11][0] = "sold date";
	GridAssets->Cells[12][0] = "descr";

    GridAssets->ColWidths[1] = 96;
	GridAssets->ColWidths[2] = 80;
    GridAssets->ColWidths[3] = 80;
    GridAssets->ColWidths[4] = 128;
	GridAssets->ColWidths[5] = 150;
	GridAssets->ColWidths[9] = 64;
	GridAssets->ColWidths[10] = 80;
    GridAssets->ColWidths[11] = 80;
    GridAssets->ColWidths[12] = 256;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::SetupTheCustomersGrid()
{
	int nRows = CUSTOMERS_DEFROWS;
	int nCols = CUSTOMERS_DEFCOLS;

	GridCustomers->ScrollBars = ssBoth;
	GridCustomers->DrawingStyle = gdsClassic;	// vip, altrimenti non imposta i colori

	GridCustomers->DoubleBuffered = true;

	GridCustomers->RowCount = nRows;
    GridCustomers->ColCount = nCols;
	GridCustomers->FixedRows = 1;
    GridCustomers->FixedCols = 1;

	GridCustomers->DefaultColWidth = CUSTOMERS_DEFCOLW;
    GridCustomers->DefaultRowHeight = CUSTOMERS_DEFCOLH;

    GridCustomers->Options << goColSizing << goRowSizing
    	<< goDrawFocusSelected << goRangeSelect
        << goRowMoving << goColMoving << goRowSelect;

	GridCustomers->Options >> goEditing;


	GridCustomers->Font->Name = "tahoma";
    GridCustomers->Font->Size = 10;

    GridCustomers->FixedColor = clActiveCaption;
    GridCustomers->Color = clMoneyGreen;

	GridCustomers->Cells[0][0] = "";
	GridCustomers->Cells[1][0] = "first name";
	GridCustomers->Cells[2][0] = "last name";
    GridCustomers->Cells[3][0] = "city";
    GridCustomers->Cells[4][0] = "address";
    GridCustomers->Cells[5][0] = "ownership";
	GridCustomers->Cells[6][0] = "budget";
    GridCustomers->Cells[7][0] = "phone";
	GridCustomers->Cells[8][0] = "email";

    GridCustomers->ColWidths[1] = 128;
	GridCustomers->ColWidths[2] = 128;
    GridCustomers->ColWidths[3] = 150;
    GridCustomers->ColWidths[4] = 200;
	GridCustomers->ColWidths[5] = 96;
	GridCustomers->ColWidths[6] = 96;
	GridCustomers->ColWidths[7] = 96;
    GridCustomers->ColWidths[8] = 256;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::PrintCustomers(TVecPtrCustomers& Customers)
{
	int nCols = 5;
	int nRows = Customers.size();

	AnsiString strTemp;
    GridCustomers->RowCount = nRows + 1;

    for(int i=0; i<nRows; i++)
    {
		TCustomer* pCustomer = Customers[i];
        assert(pCustomer);

											// row number
		GridCustomers->Cells[0][i+1] = i+1;

											// first name
		GridCustomers->Cells[1][i+1] = pCustomer->m_strFirstName;

											// last name
		GridCustomers->Cells[2][i+1] = pCustomer->m_strLastName;

											// city
		GridCustomers->Cells[3][i+1] = pCustomer->m_strCity;

											// address
		GridCustomers->Cells[4][i+1] = pCustomer->m_strAddress;

											// ownership
		GridCustomers->Cells[5][i+1] = g_OwnershipChoices[pCustomer->m_nOwnership].strName;

        									// budget
		GridCustomers->Cells[6][i+1] = pCustomer->m_nBudget;

                                            // phone number
		GridCustomers->Cells[7][i+1] = pCustomer->m_strPhone;

                                            // email
		GridCustomers->Cells[8][i+1] = pCustomer->m_strEmail;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::GridCustomersDblClick(TObject *Sender)
{
    assert(m_pDB);
	assert(FormMain);

	if( m_QueryCustomers.size() )
    {
		unsigned nRow = GridCustomers->Row - 1, nRecId = -1;

        TCustomersArchive* pCustomers = m_pDB->GetCustomers();
        assert(pCustomers);
											// poiche' m_QueryCustomers puo' essere
                                            // stato filtrato il numero di riga
                                            // della listobx degli Customers puo' NON
                                            // corrispondere piu' alla posizione
                                            // del Tasset nel vettore m_QueryCustomers
                                            // per cui bisogna recuperarlo per il
                                            // tramite del suo puntatore (TAsset*)
        if( pCustomers->GetRecordId( m_QueryCustomers[nRow], nRecId) )
        {
			TFormCustomers *pFormCustomers = FormMain->GetFormCustomers();
            assert(pFormCustomers);

            pFormCustomers->SetCurRecord(nRecId);
            pFormCustomers->ShowModal();
        }
		else
        {
			ShowErrorMsg(ERR_DBRECORDNOTFOUND);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::GridCustomersClick(TObject *Sender)
{
	if( m_QueryCustomers.size() )
    {
		ClearAssetsGrid();


		unsigned nRow = GridCustomers->Row - 1, nRecId = -1;

        TCustomersArchive* pCustomers = m_pDB->GetCustomers();
        assert(pCustomers);
											// poiche' m_QueryCustomers puo' essere
                                            // stato filtrato il numero di riga
                                            // della listobx degli Customers puo' NON
                                            // corrispondere piu' alla posizione
                                            // del Tasset nel vettore m_QueryCustomers
                                            // per cui bisogna recuperarlo per il
                                            // tramite del suo puntatore (TAsset*)
        if( pCustomers->GetRecordId( m_QueryCustomers[nRow], nRecId) )
        {
            TCustomer* pCustomer = pCustomers->GetRecord(nRecId);
            assert(pCustomer);

			UpdateAssets(pCustomer);
        }
		else
        {
			ShowErrorMsg(ERR_DBRECORDNOTFOUND);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::GridAssetsDblClick(TObject *Sender)
{
    assert(m_pDB);
	assert(FormMain);

	if( m_QueryAssets.size() )
    {
		unsigned nRow = GridAssets->Row - 1, nRecId = -1;

        TAssetsArchive* pAssets = m_pDB->GetAssets();
        assert(pAssets);
											// poiche' m_QueryAssets puo' essere
                                            // stato filtrato il numero di riga
                                            // della listobx degli Assets puo' NON
                                            // corrispondere piu' alla posizione
                                            // del Tasset nel vettore m_QueryAssets
                                            // per cui bisogna recuperarlo per il
                                            // tramite del suo puntatore (TAsset*)
        if( pAssets->GetRecordId( m_QueryAssets[nRow], nRecId) )
        {
			TFormAssets *pFormAssets = FormMain->GetFormAssets();
            assert(pFormAssets);

            pFormAssets->SetCurRecord(nRecId);
            pFormAssets->ShowModal();
        }
		else
        {
			ShowErrorMsg(ERR_DBRECORDNOTFOUND);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQuerySupplyAndDemand::UpdateAssets(TCustomer* pCustomer)
{
	assert(m_pDB);
	assert(pCustomer);
											// filters the assets

    TAssetsArchive* pAssets = m_pDB->GetAssets();
    assert(pAssets);

    m_QueryAssets = pAssets->GetAssets();

    if( m_QueryAssets.size() )
    {
											// filter assets by typology
		enAssetTypology nTypology = g_AssetTypologyChoices[ ComboBoxTypology->ItemIndex ].nTypology;
        if( nTypology != atNone )
        {
			FilterByTypology(m_QueryAssets, nTypology);
        }
											// filter by region
		if( ComboBoxAssetsRegion->Text.Length() )
        {
            TVecPtrAssets TempQuery = m_QueryAssets;

            m_QueryAssets.clear();

            for(int i=0; i<TempQuery.size(); i++)
            {
				if( m_MapCapRegions[ TempQuery[i]->m_nCap ].LowerCase() ==
					ComboBoxAssetsRegion->Text.LowerCase() )
                {
					m_QueryAssets.push_back(TempQuery[i]);
                }
            }
        }
											// cross-table filters

											// filter assets "for rent" and/or "for sale"
		enCustomerOwnershipType nOwnership = g_OwnershipChoices[ComboBoxOwnership->ItemIndex].nType;

		if( nOwnership == cotNone )
        {
            TVecPtrAssets TempQuery = m_QueryAssets;

            m_QueryAssets.clear();

            for(int i=0; i<TempQuery.size(); i++)
            {
				if( (TempQuery[i]->m_nStatus == asForRent)
					|| (TempQuery[i]->m_nStatus == asForSale) )
                {
					m_QueryAssets.push_back(TempQuery[i]);
                }
            }
        }
		else if( nOwnership == cotForRent )
		{
            TVecPtrAssets TempQuery = m_QueryAssets;

            m_QueryAssets.clear();

            for(int i=0; i<TempQuery.size(); i++)
            {
				if( TempQuery[i]->m_nStatus == asForRent )
                {
					m_QueryAssets.push_back(TempQuery[i]);
                }
            }
        }
		else if( nOwnership == cotForPurchase )
		{
            TVecPtrAssets TempQuery = m_QueryAssets;

            m_QueryAssets.clear();

            for(int i=0; i<TempQuery.size(); i++)
            {
				if( TempQuery[i]->m_nStatus == asForSale )
                {
					m_QueryAssets.push_back(TempQuery[i]);
                }
            }
        }


											// filter by "customer budget"
		if( ComboBoxBudget->Text.Length() )
        {
            TVecPtrAssets TempQuery = m_QueryAssets;
            m_QueryAssets.clear();

            for(int i=0; i<TempQuery.size(); i++)
            {
				if( m_QueryAssets[i]->m_nPrice <= pCustomer->m_nBudget)
                {
					m_QueryAssets.push_back(TempQuery[i]);
                }
            }
        }
	}
											// sort (eventually) the Assets
    if( m_QueryAssets.size() )
    {
        switch( g_AssetsSortingChoices[ComboBoxAssetsSorting->ItemIndex].nMode )
        {
            case asmCityAsc: SortByCity(m_QueryAssets, true); break;
            case asmCityDesc: SortByCity(m_QueryAssets, false); break;
            case asmPriceAsc: SortByPrice(m_QueryAssets, true); break;
            case asmPriceDesc: SortByPrice(m_QueryAssets, false); break;
        }

        PrintAssets(m_QueryAssets);
    }

    AnsiString strTemp;
    strTemp.sprintf("total: %d records", m_QueryAssets.size());
    LabelAssetsResults->Caption = strTemp;
}



