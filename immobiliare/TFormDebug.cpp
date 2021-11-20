/*!****************************************************************************

	@file	TFormDebug.h
	@file	TFormDebug.cpp

	@brief	Debug form

	@noop	author:	Francesco Settembrini
	@noop	last update: 8/11/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "TFormDebug.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDebug *FormDebug;


TStringList* g_pDebugList;

/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
__fastcall TFormDebug::TFormDebug(TComponent* Owner) : TForm(Owner)
{
	g_pDebugList = new TStringList;
	assert(g_pDebugList);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void __fastcall TFormDebug::FormCreate(TObject *Sender)
{
	RichEdit->ReadOnly = true;
}

/*!****************************************************************************
* @brief	Clear the debug form
******************************************************************************/
void __fastcall TFormDebug::Clear()
{
	RichEdit->Lines->Clear();
}

/*!****************************************************************************
* @brief	Print a string message in the Edit control
******************************************************************************/
void __fastcall TFormDebug::Print(std::string strMsg)
{
	Print(AnsiString(strMsg.c_str()));
}

/*!****************************************************************************
* @brief	Print a string message in the Edit control
******************************************************************************/
void __fastcall TFormDebug::Print(AnsiString Msg)
{
	RichEdit->Lines->Add(Msg);
}

/*!****************************************************************************
* @brief	Print a list of string messages in the Edit control
******************************************************************************/
void __fastcall TFormDebug::Print(TStringList* pList, int nLimits)
{
	int nCount = pList->Count;

	if( (nLimits > 0) && (nLimits < pList->Count) ) nCount = nLimits;

    //for(int i=0; i<pList->Count; i++)
    for(int i=0; i<nCount; i++)
        RichEdit->Lines->Add(pList->Strings[i]);
}

/*!****************************************************************************
* @brief	UI response message for Clear command
******************************************************************************/
void __fastcall TFormDebug::ActionsClearClick(TObject *Sender)
{
	Clear();
}

/*!****************************************************************************
* @brief	Saves to file the Edit contents
******************************************************************************/
void __fastcall TFormDebug::FileSaveAsClick(TObject *Sender)
{
	if( SaveDialog->Execute() == mrOk )
	{
		RichEdit->Lines->SaveToFile( SaveDialog->FileName );
	}
}


//---------------------------------------------------------------------------

