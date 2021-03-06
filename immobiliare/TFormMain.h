//---------------------------------------------------------------------------

#ifndef TFormMainH
#define TFormMainH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>


#include "commdefs.h"
#include "databanker.h"

class TFormAssets;
class TFormCustomers;
class TFormFindAssets;
class TFormFindCustomers;
class TFormQueries;

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TPanel *PanelTop;
	TMenuItem *File1;
	TPanel *PanelStatus;
	TImage *Image;
	TMenuItem *FileOpen;
	TMenuItem *FileOpenCustomersArchive;
	TMenuItem *FileOpenAssetsArchive;
	TMenuItem *FileSave;
	TMenuItem *FileSaveAssetsArchive;
	TMenuItem *FileSaveCustomersArchive;
	TMenuItem *Forms1;
	TMenuItem *FormsAssets;
	TMenuItem *FormCustomers;
	TMenuItem *Help;
	TMenuItem *HelpAbout;
	TMenuItem *N1;
	TMenuItem *FileQuit;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TMenuItem *Queries;
	TSpeedButton *SpeedButtonQueries;
	TSpeedButton *SpeedButtonAssets;
	TSpeedButton *SpeedButtonCustomers;
	TMenuItem *QueriesFindAssets;
	TMenuItem *QueriesFindCustomers;
	TMenuItem *QueriesMore;
	TSpeedButton *SpeedButtonFindCustomers;
	TSpeedButton *SpeedButtonFindAssets;
	TMenuItem *New1;
	TMenuItem *AssetsArchive1;
	TMenuItem *CustomersArchive1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormCustomersClick(TObject *Sender);
	void __fastcall FileQuitClick(TObject *Sender);
	void __fastcall FileOpenCustomersArchiveClick(TObject *Sender);
	void __fastcall HelpAboutClick(TObject *Sender);
	void __fastcall FileOpenAssetsArchiveClick(TObject *Sender);
	void __fastcall FileSaveAssetsArchiveClick(TObject *Sender);
	void __fastcall FileSaveCustomersArchiveClick(TObject *Sender);
	void __fastcall SpeedButtonAssetsClick(TObject *Sender);
	void __fastcall SpeedButtonCustomersClick(TObject *Sender);
	void __fastcall FormsAssetsClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall QueriesFindAssetsClick(TObject *Sender);
	void __fastcall QueriesFindCustomersClick(TObject *Sender);
	void __fastcall QueriesMoreClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SpeedButtonQueriesClick(TObject *Sender);
	void __fastcall SpeedButtonFindAssetsClick(TObject *Sender);
	void __fastcall SpeedButtonFindCustomersClick(TObject *Sender);
	void __fastcall AssetsArchive1Click(TObject *Sender);
	void __fastcall CustomersArchive1Click(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);

public:
	TFormAssets* const __fastcall GetFormAssets();
	TFormCustomers* const __fastcall GetFormCustomers();

protected:
	void __fastcall Setup();
	void __fastcall Cleanup();

protected:
    TDataBanker* m_pDB;
	TFormAssets* m_pFormAssets;
	TFormCustomers* m_pFormCustomers;
    TFormFindAssets* m_pFormFindAssets;
    TFormFindCustomers* m_pFormFindCustomers;
	TFormQueries* m_pFormQueries;
	bool m_bAssetsDBModify, m_bCustomersDBModify;

	bool __fastcall IsDirty();

	void __fastcall NewAssetsArchive();
	void __fastcall NewCustomersArchive();

    void __fastcall LoadAssetsArchive();
    void __fastcall LoadCustomersArchive();

    void __fastcall SaveAssets();
    void __fastcall SaveCustomers();

	void __fastcall ShowFormAssets();
	void __fastcall ShowFormCustomers();

    void __fastcall ShowFormQueries();
	void __fastcall ShowFormFindAssets();
    void __fastcall ShowFormFindCustomers();

#ifdef _DEBUG
	void __fastcall dev_LoadAssets();
	void __fastcall dev_LoadCustomers();

    void __fastcall dev_Assets();
	void __fastcall dev_Customers();
	void __fastcall dev_FindAssets();
    void __fastcall dev_FindCustomers();

    void __fastcall dev_Queries();
    void __fastcall dev_QuerySales();
	void __fastcall dev_QuerySalesHist();
    void __fastcall dev_QuerySupplyAndDemand();
#endif


	void __fastcall OnAssetsDBModify(TMessage& Message);
    void __fastcall OnCustomersDBModify(TMessage& Message);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(UM_ASSETSDBMODIFY, TMessage, OnAssetsDBModify);
        MESSAGE_HANDLER(UM_CUSTOMERSDBMODIFY, TMessage, OnCustomersDBModify);
	END_MESSAGE_MAP(TForm)
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif


