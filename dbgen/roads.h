//---------------------------------------------------------------------------
#ifndef roadsH
#define roadsH

#include <vcl.h>
#pragma hdrstop

#include <vector>>

using namespace std;

struct TRoad;
typedef vector<TRoad> TVecRoads;

struct TRoad
{
	AnsiString m_strName;
    int m_nCode;
};

class TRoadsArchive
{
public:
	TRoadsArchive();

	void __fastcall Clear();
    int __fastcall GetCount();
    void __fastcall GetRoads(TVecRoads& Roads);

	bool __fastcall LoadFromFile(AnsiString strFileName);

protected:
	TVecRoads m_Roads;

    bool __fastcall Parse(AnsiString strTuple, TRoad& Road);
};


//---------------------------------------------------------------------------
#endif
