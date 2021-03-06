//---------------------------------------------------------------------------
#ifndef TFormMainH
#define TFormMainH

//---------------------------------------------------------------------------
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <System.Classes.hpp>

#include <vector>
#include <map>

#include "cities.h"
#include "roads.h"
#include "firstnames.h"
#include "lastnames.h"

using namespace std;

typedef vector<string> TVecStrings;

struct TFileList
{
	enAssetTypology m_nTypology;
	int m_nDescrCount, m_nImgCount;
    TVecStrings m_ImgList, m_DescrList;
};

typedef std::map< enAssetTypology, TFileList > TMapFileList;


//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TStatusBar *StatusBar1;
	TMenuItem *File1;
	TMenuItem *Exit1;
	TButton *ButtonGetAssets;
	TButton *ButtonGetCustomers;
	TEdit *EditNumRecords;
	TLabel *Label1;
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall ButtonGetAssetsClick(TObject *Sender);
	void __fastcall ButtonGetCustomersClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);

protected:
	void __fastcall Setup();
	void __fastcall Cleanup();

	void __fastcall GenAssetsDB(unsigned nRecords);
    void __fastcall GenCustomersDB(unsigned nRecords);

    AnsiString __fastcall GetDataPath();

    void __fastcall Load(TVecCities& Cities);
    void __fastcall Print(TVecCities& Cities, int nLimits=-1);

    void __fastcall Load(TVecRoads& Roads);
    void __fastcall Print(TVecRoads& Roads, int nLimits=-1);

    void __fastcall Load(TVecFirstNames& FirstNames);
    void __fastcall Print(TVecFirstNames& FirstNames, int nLimits=-1);

    void __fastcall Load(TVecLastNames& LastNames);
    void __fastcall Print(TVecLastNames& LastNames, int nLimits=-1);

    void __fastcall LoadAssetsMap(TMapFileList& Map, enAssetTypology nTypology,
    	AnsiString strDir, int nImagesCount, int nDescrCount);

    void __fastcall AssetLoadDescr(AnsiString strDescrFile, TAsset& Asset);
	void __fastcall AssetLoadImage(AnsiString strImageFile, TAsset& Asset);

    TDate __fastcall RandDate(TDate Start, TDate End);
    AnsiString __fastcall Capitalize(AnsiString strString);
    AnsiString __fastcall Replace(AnsiString strString, char nChar, char nReplace);
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
