//---------------------------------------------------------------------------
#ifndef TFormCustomersH
#define TFormCustomersH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#include "databanker.h"
#include <Vcl.Buttons.hpp>


class TFormCustomers : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditFirstName;
	TEdit *EditLastName;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TButton *ButtonClose;
	TEdit *EditEmail;
	TEdit *EditPhone;
	TEdit *EditAddress;
	TLabel *Label5;
	TEdit *EditBudget;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EditCap;
	TLabel *Label8;
	TComboBox *ComboBoxCity;
	TBevel *Bevel1;
	TScrollBar *ScrollBar;
	TLabel *LabelInfo;
	TSpeedButton *SpeedButtonAdd;
	TSpeedButton *SpeedButtonDelete;
	TSpeedButton *SpeedButtonUpdate;
	TSpeedButton *SpeedButtonClear;
	TPanel *PanelBottom;
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxCityChange(TObject *Sender);
	void __fastcall ScrollBarScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall SpeedButtonAddClick(TObject *Sender);
	void __fastcall SpeedButtonDeleteClick(TObject *Sender);
	void __fastcall SpeedButtonUpdateClick(TObject *Sender);
	void __fastcall SpeedButtonClearClick(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);



private:	// User declarations

public:		// User declarations
	__fastcall TFormCustomers(TComponent* Owner, TDataBanker* pDB);

public:
	void __fastcall Clear();
    void __fastcall Update();
    void __fastcall SetCurRecord(int nId);

protected:
	int m_nCurRecord;
	TDataBanker *m_pDB;
	TVecCities m_Cities;

	void __fastcall Setup();

	bool __fastcall ValidateInput();
	void __fastcall GetRecord(TCustomer& Customer);

	void __fastcall AddRecord();
	void __fastcall DeleteRecord();
    void __fastcall UpdateRecord();
};

//---------------------------------------------------------------------------
#endif


