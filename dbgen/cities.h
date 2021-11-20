//---------------------------------------------------------------------------
#ifndef citiesH
#define citiesH

#include <vcl.h>
#pragma hdrstop

#include <vector>>

using namespace std;

struct TCity;
typedef vector<TCity> TVecCities;

struct TCity
{
	AnsiString m_strName, m_strProv, m_strCap, m_strRegion;
};

class TCitiesArchive
{
public:
	TCitiesArchive();

	void __fastcall Clear();
    int __fastcall GetCount();
    void __fastcall GetCities(TVecCities& Cities);

	bool __fastcall LoadFromFile(AnsiString strFileName);

protected:
	TVecCities m_Cities;

    bool __fastcall Parse(AnsiString strTuple, TCity& City);
};


//---------------------------------------------------------------------------
#endif
