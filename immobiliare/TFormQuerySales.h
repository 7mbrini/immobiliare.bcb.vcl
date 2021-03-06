//---------------------------------------------------------------------------

#ifndef TFormQuerySalesH
#define TFormQuerySalesH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>

#include <VCLTee.Chart.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>

#include <map>

#include "databanker.h"

//---------------------------------------------------------------------------
class TFormQuerySales : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TPanel *PanelLeft;
	TChart *Chart;
	TButton *ButtonClose;
	TButton *ButtonUpdate;
	TComboBox *ComboBoxTypology;
	TLabel *Label2;
	TComboBox *ComboBoxStartDate;
	TLabel *Label1;
	TComboBox *ComboBoxEndDate;
	TLabel *Label3;
	TComboBox *ComboBoxRegion;
	TLabel *Label4;
	TSpeedButton *SpeedButtonClear;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
	void __fastcall ButtonUpdateClick(TObject *Sender);
	void __fastcall SpeedButtonClearClick(TObject *Sender);


private:	// User declarations

public:		// User declarations
	__fastcall TFormQuerySales(TComponent* Owner, TDataBanker* pDB);

protected:
	TDataBanker* m_pDB;
	TBarSeries *m_pBarSeries;
	std::map<AnsiString, AnsiString> m_MapCapRegions;

    void __fastcall Clear();
	bool __fastcall ValidateInput();

	void __fastcall Setup();
	void __fastcall SettingUpTheChart();
    void __fastcall ClearTheChart();

	void __fastcall Update();
    void __fastcall UpdateTheChart(TVecPtrAssets& Assets);

    double __fastcall GetMeanPrice(TVecPtrAssets& Assets, int nYear);
};

//---------------------------------------------------------------------------
//extern PACKAGE TFormQuerySalesHist *FormQuerySalesHist;
//---------------------------------------------------------------------------
#endif
