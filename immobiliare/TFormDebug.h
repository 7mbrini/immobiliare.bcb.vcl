//---------------------------------------------------------------------------
#ifndef _TFORMDEBUG_H_
#define _TFORMDEBUG_H_

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>

#include <string>

using namespace std;


//---------------------------------------------------------------------------
class TFormDebug : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *MenuActions;
	TMenuItem *ActionsClear;
	TMenuItem *FileSaveAs;
	TSaveDialog *SaveDialog;
	TMenuItem *MenuFile;
	TRichEdit *RichEdit;
	void __fastcall ActionsClearClick(TObject *Sender);
	void __fastcall FileSaveAsClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormDebug(TComponent* Owner);

public:
	void __fastcall Clear();
	void __fastcall Print(AnsiString strMsg);
	void __fastcall Print(TStringList* pList, int nCount=-1);

    void __fastcall Print(std::string strMsg);
};

//---------------------------------------------------------------------------
extern PACKAGE TFormDebug *FormDebug;
//---------------------------------------------------------------------------
#endif
