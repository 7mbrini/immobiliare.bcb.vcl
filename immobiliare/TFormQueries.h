//---------------------------------------------------------------------------

#ifndef TFormQueriesH
#define TFormQueriesH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>


#include "databanker.h"
#include <Vcl.ExtCtrls.hpp>


//---------------------------------------------------------------------------
class TFormQueries : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TPanel *PanelTop;
	TButton *ButtonClose;
	TListBox *ListBox;
	TLabel *LabelInfo;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
	void __fastcall ListBoxDblClick(TObject *Sender);


private:	// User declarations

public:		// User declarations
	__fastcall TFormQueries(TComponent* Owner, TDataBanker* pDB);

protected:
	TDataBanker *m_pDB;

	void __fastcall Setup();
};

//---------------------------------------------------------------------------
#endif


