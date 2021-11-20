//---------------------------------------------------------------------------
#ifndef lastnamesH
#define lastnamesH

#include <vcl.h>
#pragma hdrstop

#include <vector>>

using namespace std;

struct TLastName;
typedef vector<TLastName> TVecLastNames;

struct TLastName
{
	AnsiString m_strName;
    int m_nFreq;
};

class TLastNamesArchive
{
public:
	TLastNamesArchive();

	void __fastcall Clear();
    int __fastcall GetCount();
    void __fastcall GetLastNames(TVecLastNames& LastNames);

	bool __fastcall LoadFromFile(AnsiString strFileName);

protected:
	TVecLastNames m_LastNames;

    bool __fastcall Parse(AnsiString strTuple, TLastName& LastName);
};


//---------------------------------------------------------------------------
#endif
