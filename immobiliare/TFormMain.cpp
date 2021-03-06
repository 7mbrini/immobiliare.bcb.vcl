/*!****************************************************************************

	@file	TFormMain.h
	@file	TFormMain.cpp

	@brief	Main form for the "Immobiliare" project

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
#include "TFormFindAssets.h"
#include "TFormFindCustomers.h"
#include "TFormQueries.h"

#include "TFormQuerySales.h"
#include "TFormQuerySalesHist.h"
#include "TFormQuerySupplyAndDemand.h"

#include "utils.h"
#include "errormsgs.h"
#include "databanker.h"
#include "commdefs.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{
    m_pDB = NULL;
	m_pFormAssets = NULL;
    m_pFormCustomers = NULL;
	m_pFormFindAssets = NULL;
    m_pFormFindCustomers = NULL;
	m_pFormQueries = NULL;
	m_bAssetsDBModify = m_bCustomersDBModify = false;
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
void __fastcall TFormMain::FormShow(TObject *Sender)
{

#ifdef _DEBUG
	dev_LoadAssets();
    dev_LoadCustomers();

	//dev_Assets();
    //dev_Customers();

	//dev_FindAssets();
    //dev_FindCustomers();

    //dev_Queries();
    //dev_QuerySales();
    //dev_QuerySalesHist();
    dev_QuerySupplyAndDemand();
#endif

}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Setup()
{
	this->Caption = APPNAME;

#ifndef _DEBUG
	this->Width = APPWIDTH;
    this->Height = APPHEIGHT;
    this->Position = poScreenCenter;
#else
	this->Left = 400;
    this->Top = 200;
    this->Width = APPWIDTH;
    this->Height = APPHEIGHT;
    this->Position = poDefaultPosOnly;

	FormDebug = new TFormDebug(this);
    assert(FormDebug);
	FormDebug->Top = this->Top;
    FormDebug->Left = this->Left + this->Width;
    FormDebug->Width = 400;
    FormDebug->Height = this->Height;
    FormDebug->Visible = true;
#endif
											// build the DataBanker
	m_pDB = new TDataBanker();
    assert(m_pDB);
    AnsiString strFileName = utils::GetDataPath() + CITIES;

											// load cities archive
    if( !m_pDB->CitiesLoadFromFile(strFileName) )
    {
		ShowErrorMsg(ERR_DBNOCITIESFILE);

        exit(-1);
    }
											// build ui forms
	m_pFormAssets = new TFormAssets(NULL, m_pDB);
    assert(m_pFormAssets);
    m_pFormAssets->Visible = false;

	m_pFormCustomers = new TFormCustomers(NULL, m_pDB);
    assert(m_pFormCustomers);
    m_pFormCustomers->Visible = false;

    m_pFormFindAssets = new TFormFindAssets(NULL, m_pDB);
    assert(m_pFormFindAssets);
    m_pFormFindAssets->Visible = false;

    m_pFormFindCustomers = new TFormFindCustomers(NULL, m_pDB);
    assert(m_pFormFindCustomers);
    m_pFormFindCustomers->Visible = false;

    m_pFormQueries = new TFormQueries(NULL, m_pDB);
    assert(m_pFormQueries);
    m_pFormQueries->Visible = false;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::Cleanup()
{
	assert(m_pDB);

    delete m_pDB;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	if( IsDirty() )
    {
    	AnsiString strMsg, strCaption;

		strCaption = AnsiString("Exit from ") + APPNAME;

		if( m_bAssetsDBModify && m_bCustomersDBModify )
        {
        	strMsg = "Databases \"Asset\" and \"Customer\" have been modified without saving :"
				"\n\nConfirm Exit ?";
        }
        else if( m_bAssetsDBModify )
        {
        	strMsg = "Database \"Asset\" has been modified without saving :"
				"\n\nConfirm Exit ?";
        }
        else
        {
        	strMsg = "Database \"Customers\" has been modified without saving :"
				"\n\nConfirm Exit ?";
        }

		::MessageBeep(-1);

        if( ::MessageBox(0, strMsg.c_str(), strCaption.c_str(),
        	MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL | MB_TOPMOST) == IDNO )
        {
			Action = caNone;
		}
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FormCustomersClick(TObject *Sender)
{
	ShowFormCustomers();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FileQuitClick(TObject *Sender)
{
	PostQuitMessage(0);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FileOpenCustomersArchiveClick(TObject *Sender)
{
	LoadCustomersArchive();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::HelpAboutClick(TObject *Sender)
{
	AnsiString strTitle;
    strTitle.printf("About '%s'", APPNAME);

    AnsiString strCaption;
    strCaption.printf("%s"
        "\n\n(C) 2021 Francesco Settembrini"
        "\n\nfrancesco.settembrini@poliba.it"
        "\nf.settembrini1@studenti.uniba.it",
        APPNAME);

	::MessageBox(0, strCaption.c_str(), strTitle.c_str(),
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL | MB_TOPMOST );
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ShowFormQueries()
{
	assert(m_pDB);
    assert(m_pFormQueries);

	m_pFormQueries->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::LoadAssetsArchive()
{
	assert(m_pDB);

	OpenDialog->Filter = "Immobiliare Assets Archive|*.ast";
    OpenDialog->DefaultExt = "*.ast";

#ifdef _DEBUG
    OpenDialog->InitialDir = utils::GetDataPath();
#endif

	if( OpenDialog->Execute() == mrOk )
    {
        if( !m_pDB->AssetsLoadFromFile(OpenDialog->FileName) )
        {
			ShowErrorMsg(ERR_OPENASSETS);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::LoadCustomersArchive()
{
	assert(m_pDB);

	OpenDialog->Filter = "Immobiliare Customers Archive|*.cst";
    OpenDialog->DefaultExt = "*.cst";

#ifdef _DEBUG
    OpenDialog->InitialDir = utils::GetDataPath();
#endif

	if( OpenDialog->Execute() == mrOk )
    {
		if( !m_pDB->CustomersLoadFromFile(OpenDialog->FileName) )
        {
			ShowErrorMsg(ERR_OPENCUSTOMERS);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SaveAssets()
{
	assert(m_pDB);

    if( bool(m_pDB->GetAssets()) )
    {
        SaveDialog->Filter = "Immobiliare Assets Archive|*.ast";
        SaveDialog->DefaultExt = "*.ast";

#ifdef _DEBUG
    SaveDialog->InitialDir = utils::GetDataPath();
#endif

        if( SaveDialog->Execute() == mrOk )
        {
            if( !m_pDB->AssetsSaveToFile(SaveDialog->FileName) )
            {
                ShowErrorMsg(ERR_DBSAVEASSETS);
            }
            else
            {
                m_bAssetsDBModify = false;
            }
        }
    }
    else
    {
        ShowErrorMsg(ERR_DBNOASSETS);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SaveCustomers()
{
	assert(m_pDB);

    if( bool(m_pDB->GetCustomers()) )
    {
        SaveDialog->Filter = "Immobiliare Customers Archive|*.cst";
        SaveDialog->DefaultExt = "*.cst";

#ifdef _DEBUG
    SaveDialog->InitialDir = utils::GetDataPath();
#endif

        if( SaveDialog->Execute() == mrOk )
        {
            if( !m_pDB->CustomersSaveToFile(SaveDialog->FileName) )
            {
                ShowErrorMsg(ERR_DBSAVECUSTOMERS);
            }
            else
            {
                m_bCustomersDBModify = false;
            }
        }
    }
    else
    {
        ShowErrorMsg(ERR_DBNOCUSTOMERS);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FileOpenAssetsArchiveClick(TObject *Sender)
{
	LoadAssetsArchive();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FileSaveAssetsArchiveClick(TObject *Sender)
{
	SaveAssets();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FileSaveCustomersArchiveClick(TObject *Sender)
{
	SaveCustomers();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SpeedButtonAssetsClick(TObject *Sender)
{
	ShowFormAssets();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SpeedButtonCustomersClick(TObject *Sender)
{
	ShowFormCustomers();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::FormsAssetsClick(TObject *Sender)
{
	ShowFormAssets();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ShowFormAssets()
{
	assert(m_pDB);
	assert(m_pFormAssets);

	if( bool(m_pDB->GetAssets()) )
    {
	    m_pFormAssets->ShowModal();
    }
    else
    {
		ShowErrorMsg(ERR_DBNOASSETS);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ShowFormCustomers()
{
	assert(m_pDB);
	assert(m_pFormCustomers);

	if( bool(m_pDB->GetCustomers()) )
    {
		m_pFormCustomers->ShowModal();
    }
    else
    {
		ShowErrorMsg(ERR_DBNOCUSTOMERS);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TFormMain::IsDirty()
{
	return m_bAssetsDBModify || m_bCustomersDBModify;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::OnAssetsDBModify(TMessage& Message)
{
	m_bAssetsDBModify = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::OnCustomersDBModify(TMessage& Message)
{
	m_bCustomersDBModify = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::QueriesFindAssetsClick(TObject *Sender)
{
	ShowFormFindAssets();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::QueriesFindCustomersClick(TObject *Sender)
{
	ShowFormFindCustomers();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::QueriesMoreClick(TObject *Sender)
{
	ShowFormQueries();
}

/*!****************************************************************************
* @brief
******************************************************************************/
TFormAssets* const __fastcall TFormMain::GetFormAssets()
{
	assert(m_pFormAssets);

    return m_pFormAssets;
}

/*!****************************************************************************
* @brief
******************************************************************************/
TFormCustomers* const __fastcall TFormMain::GetFormCustomers()
{
	assert(m_pFormCustomers);

    return m_pFormCustomers;
}


#ifdef _DEBUG
/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_LoadAssets()
{
	assert(m_pDB);

    assert(m_pDB->AssetsLoadFromFile(utils::GetDataPath() + "assets.ast"));
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_LoadCustomers()
{
	assert(m_pDB);

    assert(m_pDB->CustomersLoadFromFile(utils::GetDataPath() + "customers.cst"));
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_Assets()
{
	assert(m_pFormAssets);

	m_pFormAssets->Position = poDesigned;
	m_pFormAssets->Left = this->Left - 100;
    m_pFormAssets->Top = this->Top + 100;
	m_pFormAssets->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_Customers()
{
	assert(m_pFormCustomers);

	m_pFormCustomers->Position = poDesigned;
	m_pFormCustomers->Left = this->Left - 100;
    m_pFormCustomers->Top = this->Top + 100;
	m_pFormCustomers->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_FindAssets()
{
	assert(m_pFormFindAssets);

	m_pFormFindAssets->Position = poDesigned;
	m_pFormFindAssets->Left = this->Left - 100;
    m_pFormFindAssets->Top = this->Top + 100;
	m_pFormFindAssets->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_FindCustomers()
{
	assert(m_pFormFindCustomers);

	m_pFormFindCustomers->Position = poDesigned;
	m_pFormFindCustomers->Left = this->Left - 100;
    m_pFormFindCustomers->Top = this->Top + 100;
	m_pFormFindCustomers->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_Queries()
{
	assert(m_pFormQueries);

	m_pFormQueries->Position = poDesigned;
	m_pFormQueries->Left = this->Left - 200;
    m_pFormQueries->Top = this->Top + 100;
	m_pFormQueries->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_QuerySalesHist()
{
    TFormQuerySalesHist *pFormQuery = new TFormQuerySalesHist(NULL, m_pDB);
	assert(pFormQuery);

    pFormQuery->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_QuerySales()
{
    TFormQuerySales *pFormQuery = new TFormQuerySales(NULL, m_pDB);
    assert(pFormQuery);

    pFormQuery->Visible = true;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::dev_QuerySupplyAndDemand()
{
    TFormQuerySupplyAndDemand *pFormQuery = new TFormQuerySupplyAndDemand(NULL, m_pDB);
    assert(pFormQuery);

    pFormQuery->Visible = true;
}


#endif	// _DEBUG

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SpeedButtonQueriesClick(TObject *Sender)
{
	ShowFormQueries();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SpeedButtonFindAssetsClick(TObject *Sender)
{
	ShowFormFindAssets();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::AssetsArchive1Click(TObject *Sender)
{
	NewAssetsArchive();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::CustomersArchive1Click(TObject *Sender)
{
	NewCustomersArchive();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::SpeedButtonFindCustomersClick(TObject *Sender)
{
	ShowFormFindCustomers();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ShowFormFindAssets()
{
	assert(m_pDB);
    assert(m_pFormFindAssets);

	if( bool(m_pDB->GetAssets()) )
    {
	    m_pFormFindAssets->ShowModal();
    }
    else
    {
		ShowErrorMsg(ERR_DBNOASSETS);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::ShowFormFindCustomers()
{
	assert(m_pDB);
    assert(m_pFormFindCustomers);

	if( bool(m_pDB->GetCustomers()) )
    {
		m_pFormFindCustomers->ShowModal();
    }
    else
    {
		ShowErrorMsg(ERR_DBNOCUSTOMERS);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::NewAssetsArchive()
{
	assert(m_pDB);

	m_pDB->AssetsCreateArchive();

    if( bool(m_pDB->GetAssets()) )
    {
		::MessageBox(0,
        	"\"Assets\" archive successfully created",
            AnsiString(AnsiString(APPNAME) + AnsiString(" Info:")).c_str(),
            MB_OK | MB_ICONINFORMATION | MB_TASKMODAL | MB_TOPMOST );
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormMain::NewCustomersArchive()
{
	assert(m_pDB);

    m_pDB->CustomersCreateArchive();

    if( bool(m_pDB->GetCustomers()) )
    {
		::MessageBox(0,
        	"\"Customers\" archive successfully created",
            AnsiString(AnsiString(APPNAME) + AnsiString(" Info:")).c_str(),
            MB_OK | MB_ICONINFORMATION | MB_TASKMODAL | MB_TOPMOST );
    }
}


