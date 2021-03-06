/*!****************************************************************************

	@file	TFormAssets.h
	@file	TFormAssets.cpp

	@brief	Assets form

	@noop	author:	Francesco Settembrini
	@noop	last update: 8/11/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#include <jpeg.hpp>
#pragma hdrstop

#include "cities.h"
#include "utils.h"
#include "commdefs.h"

#include "TFormMain.h"
#include "TFormDebug.h"
#include "TFormAssets.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

struct TAssetStatusChoice
{
	AnsiString strName;
	enAssetStatus nStatus;
} g_AssetStatusChoices[] =
{
	{" ", asNone},
	{"for rent", asForRent},
    {"rented", asRented},
    {"for sale", asForSale},
    {"sold out", asSoldOut}
};

struct TAssetTypeChoice
{
	AnsiString strName;
    enAssetTypology nTypology;
} g_AssetTypologyChoices[] =
{
	{" ", atNone},
    {"terreno", atTerrain},
    {"garage", atGarage},
    {"villa", atVilla},
    {"appartamento", atApartment},
    {"ufficio", atOffice},
    {"negozio", atCommercial},
    {"capannone", atIndustrial}
};

struct TEpglChoice
{
	AnsiString strName;
    enAssetEpgl nEpgl;
} g_AssetEpglChoices[] =
{
	{" ", aeNone},
    {"A1", aeA1},
    {"A2", aeA2},
    {"A3", aeA3},
    {"A4", aeA4},
    {"B", aeB},
    {"C", aeC},
    {"D", aeD},
    {"E", aeE},
    {"F", aeF},
    {"G", aeG}
};

/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
__fastcall TFormAssets::TFormAssets(TComponent* Owner, TDataBanker* pDB)
	: TForm(Owner)
{
	assert(pDB);

    m_pDB = pDB;
	m_pJpeg = NULL;
    m_nCurRecord = 0;

    Setup();
}

/*!****************************************************************************
* @brief	Setup
******************************************************************************/
void __fastcall TFormAssets::Setup()
{
	assert(m_pDB);

    this->BorderStyle = bsSingle;
    this->FormStyle = fsStayOnTop;
	this->Position = poScreenCenter;
	this->BorderIcons >> biMinimize >> biMaximize;
	this->Caption = AnsiString(APPNAME) + " - Assets";

										// limit the length for text fields
	MemoDescription->MaxLength = ASSET_DESCRMAXLEN;
	EditAddress->MaxLength = ASSET_NAMESMAXLEN;
	ComboBoxCity->MaxLength = ASSET_NAMESMAXLEN;

    EnablePriceCtrls(false);
	EnableSoldOutCtrls(false);

										// build the "ownership" combobox
	for(int i=0; i<sizeof(g_AssetStatusChoices)/sizeof(g_AssetStatusChoices[0]); i++)
    {
		ComboBoxStatus->Items->Add(g_AssetStatusChoices[i].strName);
    }
	ComboBoxStatus->ItemIndex = asNone;

										// build the asset "type" combobox
    for(int i=0; i<sizeof(g_AssetTypologyChoices)/sizeof(g_AssetTypologyChoices[0]); i++)
    {
		ComboBoxTypology->Items->Add(g_AssetTypologyChoices[i].strName);
    }
	ComboBoxTypology->ItemIndex = atNone;

										// build the EPGL (energetic class) combobox
	for(int i=0; i<sizeof(g_AssetEpglChoices)/sizeof(g_AssetEpglChoices[0]); i++)
    {
		ComboBoxEpgl->Items->Add( g_AssetEpglChoices[i].strName);
    }
	ComboBoxEpgl->ItemIndex = aeNone;

										// build the "cities" combobox
	m_pDB->GetCities(m_Cities);
    assert(m_Cities.size());

    for(int i=0; i<m_Cities.size(); i++)
    {
		ComboBoxCity->Items->Add(m_Cities[i].m_strName);
    }
	ComboBoxCity->Text = "";
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::Clear()
{
	m_nCurRecord = 0;

	MemoDescription->Lines->Clear();

    EditAddress->Clear();
    EditCap->Clear();
    EditValue->Clear();
	EditPrice->Clear();
	EditRooms->Clear();
    EditBaths->Clear();
    EditArea->Clear();

	ComboBoxCity->Text = "";
	ComboBoxEpgl->ItemIndex = 0;
	ComboBoxTypology->ItemIndex = 0;
    ComboBoxStatus->ItemIndex = 0;

	MaskEditBuildDate->Clear();
    MaskEditSoldDate->Clear();

	ClearImage();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::ButtonCloseClick(TObject *Sender)
{
	this->ModalResult = mrOk;
    Close();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::FormShow(TObject *Sender)
{
	SetCurRecord(m_nCurRecord);

	Update();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::ComboBoxCityChange(TObject *Sender)
{
    int nItem = ComboBoxCity->ItemIndex;

	if( m_Cities.size() && nItem < m_Cities.size() )
    {
	    EditCap->Text = m_Cities[nItem].m_strCap;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::AddRecord()
{
	assert(m_pDB);
	assert(FormMain);

	TAssetsArchive* pAssets = m_pDB->GetAssets();
    assert(pAssets);

	if( ValidateInput() )
    {
/*
        TAsset Asset;
        GetRecord(Asset);
        pAssets->Add(Asset);

#ifdef _DEBUG
FormDebug->Print(Asset.AsString());
#endif
*/
        TAsset *pAsset = new TAsset;
        assert(pAsset);

        GetRecord(*pAsset);

        pAssets->Add(pAsset);


#ifdef _DEBUG
FormDebug->Print(pAsset->AsString());
#endif

        Update();

		::SendMessage((HWND) FormMain->Handle, UM_ASSETSDBMODIFY, 0, 0);
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::DeleteRecord()
{
	assert(m_pDB);

	TAssetsArchive* pAssets = m_pDB->GetAssets();
    assert(pAssets);

	if( (m_nCurRecord >= 0) && (m_nCurRecord < pAssets->GetCount()) )
    {
		pAssets->Delete(m_nCurRecord);

        SetCurRecord(m_nCurRecord--);

        Update();

		::SendMessage((HWND) FormMain->Handle, UM_ASSETSDBMODIFY, 0, 0);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::UpdateRecord()
{
	assert(m_pDB);

	TAssetsArchive* pAssets = m_pDB->GetAssets();
    assert(pAssets);


	if( ValidateInput() && (m_nCurRecord >= 0)
    	&& (m_nCurRecord < pAssets->GetCount()) )
    {
		TAsset Asset;
        GetRecord(Asset);

#ifdef _DEBUG
FormDebug->Print(Asset.AsString());
#endif
		pAssets->Update(m_nCurRecord, Asset);

        Update();

		::SendMessage((HWND) FormMain->Handle, UM_ASSETSDBMODIFY, 0, 0);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::LoadImage()
{
	if( OpenDialog->Execute((HWND) this->Handle) == mrOk )
    {
		ClearImage();

		try
        {
			m_pJpeg = new TJPEGImage;
            assert(m_pJpeg);

			m_pJpeg->LoadFromFile(OpenDialog->FileName);
            Image->Picture->Assign(m_pJpeg);
        }
		catch(...)
        {
			::MessageBox(0, "Wrong file format", "Error:",
            	MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST );
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SpeedButtonAddClick(TObject *Sender)
{
	AddRecord();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SpeedButtonDeleteClick(TObject *Sender)
{
	DeleteRecord();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SpeedButtonUpdateClick(TObject *Sender)
{
	UpdateRecord();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::Update()
{
	assert(m_pDB);

	TAssetsArchive* pAssets = m_pDB->GetAssets();
    assert(pAssets);

	int nCount = pAssets->GetCount();

	if( nCount )
    {
		ScrollBar->Enabled = true;
        ScrollBar->Max = nCount - 1;
		LabelInfo->Visible = true;
		SpeedButtonDelete->Enabled = true;
        SpeedButtonUpdate->Enabled = true;

        AnsiString strTemp;
        strTemp.printf("%d/%d", m_nCurRecord + 1, nCount);
        LabelInfo->Caption = strTemp;
	}
	else
    {
        ScrollBar->Enabled = false;
		LabelInfo->Visible = false;
		SpeedButtonDelete->Enabled = false;
		SpeedButtonUpdate->Enabled = false;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SpeedButtonClearClick(TObject *Sender)
{
	Clear();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SpeedButtonLoadImageClick(TObject *Sender)
{
	LoadImage();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SpeedButtonClearImageClick(TObject *Sender)
{
	ClearImage();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::ScrollBarScroll(TObject *Sender, TScrollCode ScrollCode,
          int &ScrollPos)
{
    SetCurRecord(ScrollPos);

	Update();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::ClearImage()
{
    if( m_pJpeg )
    {
    	delete m_pJpeg;
        m_pJpeg = NULL;
    }

    Image->Picture->Bitmap->FreeImage();
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool __fastcall TFormAssets::ValidateInput()
{
	bool bValid = true;
												// validate "Description" field
    {
		AnsiString strMemo = MemoDescription->Lines->GetText();

		if( strMemo.Length() < ASSET_NAMESMINLEN )
        {
            ::MessageBox(0, "Error: Field \"Description\" too short!", "Error:",
                MB_OK | MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);

            bValid = false;
        }
    }
												// Address
	if( bValid )
    {
		if( EditAddress->Text.Length() < ASSET_NAMESMINLEN )
        {
            ::MessageBox(0, "Field \"Address\" too short!", "Error:",
                MB_OK | MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);

            bValid = false;
        }
    }
												// date
	if( bValid )
    {
		if( g_AssetStatusChoices[ComboBoxStatus->ItemIndex].nStatus == asSoldOut )
        {
			AnsiString strDate = MaskEditSoldDate->Text;

            unsigned short nYear, nMonth, nDay;
			utils::ParseDate(strDate, nYear, nMonth, nDay);

												// TDate Date(2021,11,12)
												// ( anno, mese, giorno )
			try
            {
				TDate Date(nYear, nMonth, nDay);
            }
            catch(...)
            {
            	::MessageBox(0,
                	"Wrong value for Sold Date!",
                	"Error:",
                	MB_OK | MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);

				bValid = false;
            }
        }
    }
												// Image
	if( bValid )
    {
		if( m_pJpeg )
        {
			TMemoryStream *pMS = new TMemoryStream;
            assert(pMS);

            m_pJpeg->SaveToStream(pMS);

            unsigned nSize = pMS->Size;
            delete pMS;

            if( nSize > ASSET_IMAGEMAXSIZE )
            {
            	::MessageBox(0, "Image too big!", "Error:",
                	MB_OK | MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);

                bValid = false;
			}
        }
    }

    return bValid;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::ComboBoxTypologyChange(TObject *Sender)
{
	EnableCtrls( g_AssetTypologyChoices[ComboBoxTypology->ItemIndex].nTypology );
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::ComboBoxStatusChange(TObject *Sender)
{
	//ShowSoldOutCtrls(g_AssetStatusChoices[ComboBoxStatus->ItemIndex].nStatus == asSoldOut);
	EnableSoldOutCtrls(g_AssetStatusChoices[ComboBoxStatus->ItemIndex].nStatus == asSoldOut);

	EnablePriceCtrls( (g_AssetStatusChoices[ComboBoxStatus->ItemIndex].nStatus == asForRent)
    	|| (g_AssetStatusChoices[ComboBoxStatus->ItemIndex].nStatus == asForSale) );
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnableSoldOutCtrls(bool bEnable)
{
	LabelSoldDate->Enabled = bEnable;
	MaskEditSoldDate->Enabled = bEnable;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnablePriceCtrls(bool bEnable)
{
	LabelPrice->Enabled = bEnable;
	EditPrice->Enabled = bEnable;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnableBuildDateCtrls(bool bEnable)
{
	LabelBuildDate->Enabled = bEnable;
    MaskEditBuildDate->Enabled = bEnable;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnableEpglCtrls(bool bEnable)
{
	LabelEpgl->Enabled = bEnable;
    ComboBoxEpgl->Enabled = bEnable;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnableRoomCtrls(bool bEnable)
{
	LabelRooms->Enabled = bEnable;
    EditRooms->Enabled = bEnable;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnableBathsCtrls(bool bEnable)
{
	LabelBaths->Enabled = bEnable;
    EditBaths->Enabled = bEnable;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::EnableCtrls(enAssetTypology nTypology)
{
	if( nTypology > atTerrain )
    {
		EnableEpglCtrls(true);
		EnableRoomCtrls(true);
        EnableBathsCtrls(true);
		EnableBuildDateCtrls(true);
    }
    else
    {
		EnableEpglCtrls(false);
		EnableRoomCtrls(false);
        EnableBathsCtrls(false);
		EnableBuildDateCtrls(false);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::SetCurRecord(unsigned nId)
{
	assert(m_pDB);

    Clear();

	TAssetsArchive* pAssets = m_pDB->GetAssets();
    assert(pAssets);
    //TAsset Asset;
    TAsset const* pAsset = pAssets->GetRecord(nId);

    //if( pAssets->GetRecord(nId, Asset) )
    if( pAsset )
    {
        m_nCurRecord = nId;
        ScrollBar->Position = m_nCurRecord;
											// update the ui controls
                                            // with the 'Asset' values

                                            // description
        {
            TMemoryStream *pMS = new TMemoryStream;
            assert(pMS);

            pMS->Write(pAsset->m_strDescription, ASSET_DESCRMAXLEN);

            pMS->Position = 0L;
            MemoDescription->Lines->LoadFromStream(pMS);

            delete pMS;
        }
											// address
        EditAddress->Text = pAsset->m_strAddress;
        ComboBoxCity->Text = pAsset->m_strCity;

											// cap
        AnsiString strTemp;
        strTemp.sprintf("%d", pAsset->m_nCap);
        EditCap->Text = strTemp;
											// type
		ComboBoxTypology->ItemIndex = g_AssetTypologyChoices[pAsset->m_nTypology].nTypology;

											// enable/disable some ui-controls
                                            // according to the asset typology
        EnableCtrls(pAsset->m_nTypology);

											// energetic class (EPGL)
		if( pAsset->m_nTypology > atTerrain )
        {
    		//EnableEpglCtrls(true);

			ComboBoxEpgl->ItemIndex = g_AssetEpglChoices[pAsset->m_nEpgl].nEpgl;
        }
/*
        else
        {
        	EnableEpglCtrls(false);
        }
*/

											// build date
		if( pAsset->m_nTypology > atTerrain )
        {
			//EnableBuildDateCtrls(true);

            TDate Date((double)pAsset->m_BuildDate);
            unsigned short nYear, nMonth, nDay;
            Date.DecodeDate(&nYear, &nMonth, &nDay);

            AnsiString strTemp;
            strTemp.sprintf("%02d/%02d/%04d", nDay, nMonth, nYear);
            MaskEditBuildDate->Text = strTemp;
        }
/*
        else
        {
        	EnableBuildDateCtrls(false);
        }
*/
											// status
		ComboBoxStatus->ItemIndex = g_AssetStatusChoices[pAsset->m_nStatus].nStatus;

        if( pAsset->m_nStatus == asSoldOut )
        {
            EnableSoldOutCtrls(true);

            TDate Date((double)pAsset->m_SoldDate);
            unsigned short nYear, nMonth, nDay;
            Date.DecodeDate(&nYear, &nMonth, &nDay);

            AnsiString strTemp;
            strTemp.sprintf("%02d/%02d/%04d", nDay, nMonth, nYear);
            MaskEditSoldDate->Text = strTemp;
        }
        else
        {
        	EnableSoldOutCtrls(false);
        }

        if( (pAsset->m_nStatus == asForRent) || (pAsset->m_nStatus == asForSale) )
        {
			EnablePriceCtrls(true);

            AnsiString strPrice;
            strPrice.sprintf("%d", pAsset->m_nPrice);
            EditPrice->Text = strPrice;
        }
		else
        {
			EnablePriceCtrls(false);
        }
											// rooms
		strTemp.sprintf("%d", pAsset->m_nRooms);
        EditRooms->Text = strTemp;
        									// bathrooms
		strTemp.sprintf("%d", pAsset->m_nBathrooms);
        EditBaths->Text = strTemp;
        									// area
		strTemp.sprintf("%d", pAsset->m_nArea);
        EditArea->Text = strTemp;

											// cadastral value
        strTemp.sprintf("%d", pAsset->m_nValue);
        EditValue->Text = strTemp;

											// image
        if( pAsset->m_bImage )
        {
			TMemoryStream *pMS = new TMemoryStream;
            assert(pMS);

            pMS->Write(pAsset->m_pImage, ASSET_IMAGEMAXSIZE);

            if( !m_pJpeg ) m_pJpeg = new TJPEGImage;
            assert(m_pJpeg);

            pMS->Position = 0L;
            m_pJpeg->LoadFromStream(pMS);

            delete pMS;

            Image->Picture->Assign(m_pJpeg);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormAssets::GetRecord(TAsset& Asset)
{
												// Get the "Description" field
	{
		TMemoryStream *pMS = new TMemoryStream;
        assert(pMS);

    	MemoDescription->Lines->SaveToStream(pMS);

		pMS->Position = 0L;
        pMS->Read(Asset.m_strDescription, pMS->Size);

    	delete pMS;
    }
												// address
    strncpy(Asset.m_strAddress, AnsiString(EditAddress->Text).c_str(), ASSET_NAMESMAXLEN - 1);

    											// city
    strncpy(Asset.m_strCity, AnsiString(ComboBoxCity->Text).c_str(), ASSET_NAMESMAXLEN - 1);

												// cap
    Asset.m_nCap = EditCap->Text.ToInt();

    											// typology
	Asset.m_nTypology = g_AssetTypologyChoices[ComboBoxTypology->ItemIndex].nTypology;

												// energetic class
	if( Asset.m_nTypology > atTerrain )
    {
		Asset.m_nEpgl = g_AssetEpglChoices[ComboBoxEpgl->ItemIndex].nEpgl;
    }

												// status
	Asset.m_nStatus = g_AssetStatusChoices[ComboBoxStatus->ItemIndex].nStatus;

												// build date
    {
        AnsiString strDate = MaskEditBuildDate->Text;

        unsigned short nYear, nMonth, nDay;
        utils::ParseDate(strDate, nYear, nMonth, nDay);

                                            	// TDate Date(2021,11,12)
                                            	// ( anno, mese, giorno )
        TDate Date(nYear, nMonth, nDay);
												// Val is a number (double)
        Asset.m_BuildDate = Date.Val;
    }

												// sold out date
	if( (Asset.m_nStatus == asForRent) || (Asset.m_nStatus == asForSale) )
    {
		Asset.m_nPrice = EditPrice->Text.ToInt();
    }
    else if( Asset.m_nStatus == asSoldOut )
    {
        AnsiString strDate = MaskEditSoldDate->Text;

        unsigned short nYear, nMonth, nDay;
        utils::ParseDate(strDate, nYear, nMonth, nDay);

                                            	// TDate Date(2021,11,12)
                                            	// ( anno, mese, giorno )
        TDate Date(nYear, nMonth, nDay);
												// Val is a number (double)
        Asset.m_SoldDate = Date.Val;
    }
												// rooms
	Asset.m_nRooms = EditRooms->Text.ToInt();

												// bathrooms
	Asset.m_nBathrooms = EditBaths->Text.ToInt();

    											// area
	Asset.m_nArea = EditArea->Text.ToInt();

												// cadastral value
    Asset.m_nValue = EditValue->Text.ToInt();

												// image
    if( m_pJpeg )
    {
		TMemoryStream *pMS = new TMemoryStream;
        assert(pMS);

        m_pJpeg->SaveToStream(pMS);

        unsigned nSize = pMS->Size;
        assert(nSize <= ASSET_IMAGEMAXSIZE);
												// write the compressed (jpeg)
                                                // picture in to the image field
		pMS->Position = 0L;
        pMS->Read(Asset.m_pImage, nSize);
        delete pMS;
												// the flag a true indicate
                                                // the presence of image field
                                                // when loading the record
        Asset.m_bImage = true;
    }
}



