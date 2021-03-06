//---------------------------------------------------------------------------

#ifndef TFormQuerySupplyAndDemandH
#define TFormQuerySupplyAndDemandH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Grids.hpp>

#include <map>

#include "databanker.h"

//---------------------------------------------------------------------------
class TFormQuerySupplyAndDemand : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TPanel *PanelLeft;
	TButton *ButtonClose;
	TButton *ButtonUpdate;
	TComboBox *ComboBoxTypology;
	TLabel *Label2;
	TComboBox *ComboBoxAssetsRegion;
	TLabel *Label4;
	TComboBox *ComboBoxBudget;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label5;
	TPanel *PanelCustomers;
	TPanel *PanelAssets;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *PanelClient;
	TLabel *LabelCustomers;
	TLabel *LabelAssets;
	TComboBox *ComboBoxOwnership;
	TLabel *Label8;
	TComboBox *ComboBoxAssetsSorting;
	TLabel *Label9;
	TComboBox *ComboBoxCustomersSorting;
	TLabel *Label10;
	TPanel *Panel3;
	TLabel *LabelCustomersResults;
	TPanel *Panel4;
	TLabel *LabelAssetsResults;
	TLabel *Label6;
	TLabel *Label7;
	TStringGrid *GridCustomers;
	TStringGrid *GridAssets;
	TComboBox *ComboBoxCustomersRegion;
	TLabel *Label11;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
	void __fastcall ButtonUpdateClick(TObject *Sender);
	void __fastcall GridCustomersDblClick(TObject *Sender);
	void __fastcall GridCustomersClick(TObject *Sender);
	void __fastcall GridAssetsDblClick(TObject *Sender);


private:	// User declarations

public:		// User declarations
	__fastcall TFormQuerySupplyAndDemand(TComponent* Owner, TDataBanker* pDB);

protected:
	TDataBanker* m_pDB;
	TVecPtrAssets m_QueryAssets;
    TVecPtrCustomers m_QueryCustomers;
	std::map<AnsiString, AnsiString> m_MapCapRegions;

    void __fastcall Clear();
	bool __fastcall ValidateInput();

	void __fastcall Setup();
	void __fastcall SetupTheAssetsGrid();
    void __fastcall SetupTheCustomersGrid();
	void __fastcall ClearAssetsGrid();
    void __fastcall ClearCustomersGrid();

    void __fastcall Update();
	void __fastcall UpdateCustomers();
	void __fastcall UpdateAssets(TCustomer* pCustomer);

    void __fastcall PrintAssets(TVecPtrAssets& Assets);
    void __fastcall PrintCustomers(TVecPtrCustomers& Customers);
};

//---------------------------------------------------------------------------
//extern PACKAGE TFormQuerySalesHist *FormQuerySalesHist;
//---------------------------------------------------------------------------
#endif
