/*!****************************************************************************

	@file	TFormQueries.h
	@file	TFormQueries.cpp

	@brief	Queries form

	@noop	author:	Francesco Settembrini
	@noop	last update: 8/11/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "TFormDebug.h"
#include "TFormQueries.h"
#include "TFormQuerySales.h"
#include "TFormQuerySalesHist.h"
#include "TFormQuerySupplyAndDemand.h"


#include "commdefs.h"
#include "errormsgs.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


#define MINHEIGHT	240
#define MINWIDTH	400


enum enQueryChoice { qcSupplyAndDemand, qcMeanSalesPrice, qcSalesHistogram};

struct TQueryChoice
{
	AnsiString strName;
	enQueryChoice nQuery;
} g_QueryChoices[] =
{
    { "  Analisi della domanda e dell'offerta di immobili", qcSupplyAndDemand },
	{ "  Analisi del valore medio delle compravendite", qcMeanSalesPrice },
	{ "  Istogramma del numero di compravendite annuali", qcSalesHistogram }
};


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
__fastcall TFormQueries::TFormQueries(TComponent* Owner, TDataBanker* pDB) : TForm(Owner)
{
	assert(pDB);

    m_pDB = pDB;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQueries::FormCreate(TObject *Sender)
{
	Setup();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQueries::Setup()
{
	this->Caption = AnsiString(APPNAME) + " - Queries";
    this->Position = poScreenCenter;
	this->FormStyle = fsStayOnTop;
    //this->BorderStyle = bsSingle;
    this->Constraints->MinHeight = MINHEIGHT;
    this->Constraints->MinWidth = MINWIDTH;
    this->Width = MINWIDTH;;
    this->Height = MINHEIGHT;

	this->BorderIcons >> biMinimize >> biMaximize;

	LabelInfo->Caption = "Double click on the list to make your choice";


    for(int i=0; i<sizeof(g_QueryChoices)/sizeof(g_QueryChoices[0]); i++)
    {
		ListBox->Items->Add(g_QueryChoices[i].strName);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQueries::ButtonCloseClick(TObject *Sender)
{
	this->ModalResult = mrOk;
    this->Close();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQueries::FormCanResize(TObject *Sender,
	 int &NewWidth, int &NewHeight, bool &Resize)
{
	ButtonClose->Left = (this->Width - ButtonClose->Width)/2;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormQueries::ListBoxDblClick(TObject *Sender)
{
	assert(m_pDB);

	if( g_QueryChoices[ListBox->ItemIndex].nQuery == qcSalesHistogram )
    {
		if( bool(m_pDB->GetAssets()) )
        {
            TFormQuerySalesHist *pFormQuery = new TFormQuerySalesHist(NULL, m_pDB);
            assert(pFormQuery);

            pFormQuery->Visible = true;
		}
        else
        {
			ShowErrorMsg(ERR_DBNOASSETS);
        }
    }
	else if( g_QueryChoices[ListBox->ItemIndex].nQuery == qcSupplyAndDemand )
    {
        if( bool(m_pDB->GetAssets()) && bool(m_pDB->GetCustomers()) )
        {
            TFormQuerySupplyAndDemand *pFormQuery = new TFormQuerySupplyAndDemand(NULL, m_pDB);
            assert(pFormQuery);

            pFormQuery->Visible = true;
        }
        else
        {
            ShowErrorMsg(ERR_DBNODATA);
        }
    }
	else if( g_QueryChoices[ListBox->ItemIndex].nQuery == qcMeanSalesPrice )
    {
		if( bool(m_pDB->GetAssets()) )
		{
            TFormQuerySales *pFormQuery = new TFormQuerySales(NULL, m_pDB);
            assert(pFormQuery);

            pFormQuery->Visible = true;
		}
        else
        {
			ShowErrorMsg(ERR_DBNOASSETS);
        }
    }
}



