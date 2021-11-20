//---------------------------------------------------------------------------
#ifndef assetsH
#define assetsH

#include <vcl.h>
#pragma hdrstop

#include <vector>
#include <stdio.h>


#define ASSET_MINPRICE		10
#define ASSET_NAMESMINLEN	1
#define ASSET_NAMESMAXLEN	50
#define ASSET_MAGICLEN      64
#define ASSET_DESCRMAXLEN	(2<<10)	//  2 KB
//#define ASSET_IMAGEMAXSIZE	(2<<16)	// ~128 KB
#define ASSET_IMAGEMAXSIZE	(2<<14)	// max ~32 KB for jpeg files


enum enAssetStatus { asNone = 0, asForRent, asRented, asForSale, asSoldOut };
enum enAssetTypology { atNone = 0, atTerrain, atGarage, atVilla,
	atApartment, atOffice, atCommercial, atIndustrial };
enum enAssetEpgl { aeNone = 0, aeG, aeF, aeE, aeD, aeC, aeB, aeA4, aeA3, aeA2, aeA1 };

class TAsset;
typedef std::vector<TAsset> TVecAssets;
typedef std::vector<TAsset*> TVecPtrAssets;



class TAsset
{
public:
	TAsset();

public:
	bool Read(FILE* fp);
    bool Write(FILE* fp);

	AnsiString AsString();

public:
	char m_strAddress[ASSET_NAMESMAXLEN];
	char m_strCity[ASSET_NAMESMAXLEN];
	char m_strDescription[ASSET_DESCRMAXLEN];

	enAssetEpgl m_nEpgl;
	enAssetTypology m_nTypology;
	enAssetStatus m_nStatus;

    double m_BuildDate, m_SoldDate;

	unsigned m_nValue; // cadastral value
	unsigned m_nPrice; // rent or selling price
    unsigned m_nCap, m_nArea;
	unsigned short m_nRooms, m_nBathrooms;

    bool m_bImage;
    unsigned char m_pImage[ASSET_IMAGEMAXSIZE];
};

class TAssetsArchive
{
public:
	TAssetsArchive();
	~TAssetsArchive();

	void Clear();

    void Add(TAsset* pAsset);
    void Delete(int nIndex);
    void Update(int nIndex, TAsset const& Asset);

    bool LoadFromFile(AnsiString strFileName);
    bool SaveToFile(AnsiString strFileName);

	unsigned GetCount();

    TAsset* const GetRecord(int nId);
    bool GetRecordId(TAsset* pAsset, unsigned& nId);

    TVecPtrAssets GetAssets();

protected:
	struct TFileHeader
    {
    	char pMagic[ASSET_MAGICLEN];
        int nRecords;
    };

protected:
	//TVecAssets m_Assets;
    TVecPtrAssets m_PtrAssets;
};


AnsiString AsString(enAssetEpgl nEpgl);
AnsiString AsString(enAssetStatus nStatus);

void FilterByStatus(TVecPtrAssets& Assets, enAssetStatus nStatus);
void FilterByTypology(TVecPtrAssets& Assets, enAssetTypology nTypology);
void FilterByCity(TVecPtrAssets& Assets, AnsiString strCityName, bool bFullMatch=true);
void FilterByAddress(TVecPtrAssets& Assets, AnsiString strAddress, bool bFullMatch=false);
void FilterByEpgl(TVecPtrAssets& Assets, enAssetEpgl nEpgl, AnsiString strIneq);
void FilterByBuildDate(TVecPtrAssets& Assets, double Date, AnsiString strIneq);
void FilterByPrice(TVecPtrAssets& Assets, unsigned nPrice, AnsiString strIneq);
void FilterByRooms(TVecPtrAssets& Assets, unsigned nRooms, AnsiString strIneq);
void FilterByBathrooms(TVecPtrAssets& Assets, unsigned nRooms, AnsiString strIneq);
void FilterByArea(TVecPtrAssets& Assets, unsigned nArea, AnsiString strIneq);
void FilterByValue(TVecPtrAssets& Assets, unsigned nValue, AnsiString strIneq);

void SortByCity(TVecPtrAssets& Assets, bool bAsc = true);
void SortByPrice(TVecPtrAssets& Assets, bool bAsc = true);
void SortByArea(TVecPtrAssets& Assets, bool bAsc = true);
void SortByValue(TVecPtrAssets& Assets, bool bAsc = true);

//---------------------------------------------------------------------------
#endif


