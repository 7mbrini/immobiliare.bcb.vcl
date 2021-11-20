//---------------------------------------------------------------------------
#ifndef TFormAssetsH
#define TFormAssetsH

//---------------------------------------------------------------------------
#include <jpeg.hpp>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>

#include "databanker.h"
#include <Vcl.Mask.hpp>

class TFormAssets : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonClose;
	TEdit *EditAddress;
	TLabel *Label5;
	TEdit *EditValue;
	TLabel *LabelValue;
	TLabel *Label7;
	TEdit *EditCap;
	TLabel *Label8;
	TComboBox *ComboBoxCity;
	TImage *Image;
	TBevel *Bevel1;
	TShape *Shape1;
	TOpenDialog *OpenDialog;
	TLabel *LabelInfo;
	TScrollBar *ScrollBar;
	TSpeedButton *SpeedButtonUpdate;
	TSpeedButton *SpeedButtonDelete;
	TSpeedButton *SpeedButtonAdd;
	TSpeedButton *SpeedButtonClear;
	TSpeedButton *SpeedButtonLoadImage;
	TSpeedButton *SpeedButtonClearImage;
	TLabel *Label1;
	TComboBox *ComboBoxStatus;
	TComboBox *ComboBoxTypology;
	TLabel *Label2;
	TLabel *Label3;
	TMemo *MemoDescription;
	TEdit *EditRooms;
	TLabel *LabelRooms;
	TEdit *EditBaths;
	TLabel *LabelBaths;
	TEdit *EditArea;
	TLabel *LabelArea;
	TLabel *LabelSoldDate;
	TLabel *Label12;
	TMaskEdit *MaskEditSoldDate;
	TMaskEdit *MaskEditBuildDate;
	TLabel *LabelBuildDate;
	TComboBox *ComboBoxEpgl;
	TLabel *LabelEpgl;
	TEdit *EditPrice;
	TLabel *LabelPrice;
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxCityChange(TObject *Sender);
	void __fastcall SpeedButtonAddClick(TObject *Sender);
	void __fastcall SpeedButtonDeleteClick(TObject *Sender);
	void __fastcall SpeedButtonUpdateClick(TObject *Sender);
	void __fastcall SpeedButtonClearClick(TObject *Sender);
	void __fastcall SpeedButtonLoadImageClick(TObject *Sender);
	void __fastcall SpeedButtonClearImageClick(TObject *Sender);
	void __fastcall ScrollBarScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall ComboBoxStatusChange(TObject *Sender);
	void __fastcall ComboBoxTypologyChange(TObject *Sender);


private:	// User declarations

public:		// User declarations
	__fastcall TFormAssets(TComponent* Owner, TDataBanker* pDB);

public:
	void __fastcall Clear();
    void __fastcall Update();
    void __fastcall SetCurRecord(unsigned nId);

protected:
	int m_nCurRecord;
	TDataBanker *m_pDB;
	TVecCities m_Cities;
    TJPEGImage *m_pJpeg;

	void __fastcall Setup();

    void __fastcall LoadImage();
    void __fastcall ClearImage();

	bool __fastcall ValidateInput();
	void __fastcall GetRecord(TAsset& Asset);

	void __fastcall AddRecord();
	void __fastcall DeleteRecord();
    void __fastcall UpdateRecord();

	void __fastcall EnablePriceCtrls(bool bEnable);
    void __fastcall EnableSoldOutCtrls(bool bEnable);

    void __fastcall EnableBuildDateCtrls(bool bEnable);
	void __fastcall EnableEpglCtrls(bool bEnable);
	void __fastcall EnableRoomCtrls(bool bEnable);
    void __fastcall EnableBathsCtrls(bool bEnable);

	void __fastcall EnableCtrls(enAssetTypology nTypology);
};

//---------------------------------------------------------------------------
#endif
