//---------------------------------------------------------------------------
#ifndef databankerH
#define databankerH

#include <vcl.h>
#pragma hdrstop

#include "cities.h"
#include "assets.h"
#include "customers.h"


class TDataBanker
{
public:
	TDataBanker();
	~TDataBanker();

	void __fastcall CitiesCreateArchive();
    void __fastcall AssetsCreateArchive();
    void __fastcall CustomersCreateArchive();

	TAssetsArchive* const __fastcall GetAssets();
	TCustomersArchive* const __fastcall GetCustomers();

	TCitiesArchive* const __fastcall GetCities();
	void __fastcall GetCities(TVecCities& Cities);

	bool __fastcall CitiesLoadFromFile(AnsiString strFileName);
	bool __fastcall AssetsLoadFromFile(AnsiString strFileName);
    bool __fastcall CustomersLoadFromFile(AnsiString strFileName);

	bool __fastcall AssetsSaveToFile(AnsiString strFileName);
    bool __fastcall CustomersSaveToFile(AnsiString strFileName);

protected:
    TCitiesArchive* m_pCities;
    TAssetsArchive* m_pAssets;
	TCustomersArchive* m_pCustomers;
};

//---------------------------------------------------------------------------
#endif
