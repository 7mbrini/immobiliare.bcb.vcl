//---------------------------------------------------------------------------
#ifndef TFormFindAssetsH
#define TFormFindAssetsH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>

#include "databanker.h"


//---------------------------------------------------------------------------
class TFormFindAssets : public TForm
{
__published:	// IDE-managed Components
	TSpeedButton *SpeedButtonFind;
	TEdit *EditArea;
	TLabel *LabelArea;
	TEdit *EditBaths;
	TLabel *LabelBaths;
	TEdit *EditRooms;
	TLabel *LabelRooms;
	TComboBox *ComboBoxStatus;
	TLabel *Label1;
	TMaskEdit *MaskEditBuildDate;
	TLabel *LabelBuildDate;
	TComboBox *ComboBoxEpgl;
	TLabel *LabelEpgl;
	TComboBox *ComboBoxCity;
	TLabel *Label7;
	TEdit *EditValue;
	TLabel *Label6;
	TComboBox *ComboBoxTypology;
	TLabel *Label2;
	TSpeedButton *SpeedButtonClear;
	TButton *ButtonClose;
	TPanel *PanelBottom;
	TComboBox *ComboBoxBuildDateIneq;
	TPanel *PanelLeft;
	TPanel *PanelRight;
	TStringGrid *Grid;
	TLabel *LabelInfo;
	TComboBox *ComboBoxSorting;
	TLabel *Label3;
	TPanel *PanelInfo;
	TComboBox *ComboBoxEpglIneq;
	TComboBox *ComboBoxValueIneq;
	TEdit *EditAddress;
	TLabel *Label5;
	TCheckBox *CheckBoxCityNameFullMatch;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall SpeedButtonFindClick(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
	void __fastcall SpeedButtonClearClick(TObject *Sender);
	void __fastcall GridDblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);


private:	// User declarations

public:		// User declarations
	__fastcall TFormFindAssets(TComponent* Owner, TDataBanker* const pDB);

protected:
	TDataBanker *m_pDB;
	TVecPtrAssets m_QueryAssets;

	void __fastcall Setup();
	void __fastcall DoQuery();

	void __fastcall Clear();

	void __fastcall ClearTheGrid();
    void __fastcall SetupTheGrid();

    void __fastcall PrintResults(TVecPtrAssets& Assets);
    void __fastcall EditRecord(TVecPtrAssets& Assets, int nGridRow);
};

//---------------------------------------------------------------------------
#endif
