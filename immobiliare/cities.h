//---------------------------------------------------------------------------
#ifndef citiesH
#define citiesH

#include <vcl.h>
#pragma hdrstop

#include <vector>
#include <string>


typedef std::vector<std::string> TVecStrings;

struct TCity;
typedef std::vector<TCity> TVecCities;

struct TCity
{
	AnsiString AsString()
    {
		AnsiString strTemp;

        strTemp.sprintf(
        	"\ncomune: %s"
            "\nprovincia: %s"
            "\ncap: %s"
            "\nregione: %s",

            m_strName.c_str(),
            m_strProv.c_str(),
            m_strCap.c_str(),
            m_strRegion.c_str()
        );

        return strTemp;
    }

	AnsiString m_strName, m_strProv, m_strCap, m_strRegion;
};

class TCitiesArchive
{
public:
	TCitiesArchive();

	void Clear();
    int GetCount();

    void GetCities(TVecCities& Cities);
	void GetRegions(TVecStrings& Regions);

	bool LoadFromFile(AnsiString strFileName);

protected:
	TVecCities m_Cities;

    bool Parse(AnsiString strTuple, TCity& City);
	bool IsUnique(TVecStrings& Strings, std::string strVal);
};


//---------------------------------------------------------------------------
#endif
