/*!****************************************************************************

	@file	customers.h
	@file	customers.cpp

	@brief	Customers modelling

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <vcl.h>
#pragma hdrstop

#include "customers.h"
#include "sorting.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


static const char MAGICWORD[CUSTOMER_MAGICLEN] = "IMMOBILIARE-CUSTOMERS-(C)2021-StudioWARE";

using namespace sort;


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
TCustomer::TCustomer()
{
    m_nCap = 0;
    m_nBudget = 0;
	m_nOwnership = cotNone;
    memset(m_strFirstName, 0, CUSTOMER_NAMESMAXLEN);
    memset(m_strLastName, 0, CUSTOMER_NAMESMAXLEN);
	memset(m_strAddress, 0, CUSTOMER_NAMESMAXLEN);
	memset(m_strCity, 0, CUSTOMER_NAMESMAXLEN);
    memset(m_strEmail, 0, CUSTOMER_NAMESMAXLEN);
    memset(m_strPhone, 0, CUSTOMER_NAMESMAXLEN);
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString TCustomer::AsString()
{
	AnsiString strResult;

	strResult.printf(
    	"\nFirst Name: %s"
        "\nLast Name: %s"
        "\nAddress: %s"
        "\nCity: %s"
        "\nCAP: %d"
        "\nEmail: %s"
        "\nPhone: %s"
        "\nBudget: %d"
        "\nOwnership: %d",
        m_strFirstName,
        m_strLastName,
        m_strAddress,
        m_strCity,
        m_nCap,
        m_strEmail,
        m_strPhone,
        m_nBudget,
        m_nOwnership
    );

    return strResult;
}

/*!****************************************************************************
* @brief	Read the record from file
* @param	fp Pointer to FILE struct
******************************************************************************/
bool TCustomer::Read(FILE* fp)
{
	assert(fp);

    return fread(this, sizeof(*this), 1, fp);
}

/*!****************************************************************************
* @brief	Write the record to file
* @param	fp Pointer to FILE struct
******************************************************************************/
bool TCustomer::Write(FILE* fp)
{
	assert(fp);

	return fwrite(this, sizeof(*this), 1, fp);
}


/*!****************************************************************************
* @brief	Constructor
******************************************************************************/
TCustomersArchive::TCustomersArchive()
{
	;
}

/*!****************************************************************************
* @brief	Destructor
******************************************************************************/
TCustomersArchive::~TCustomersArchive()
{
	Clear();
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TCustomersArchive::LoadFromFile(AnsiString strFileName)
{
	bool bResult = false;

    FILE* pFile = fopen(strFileName.c_str(), "rb");

    if( pFile )
    {
		//m_Customers.clear();
        Clear();

		TFileHeader Header;
        memset(&Header, 0, sizeof(Header));

        fread(&Header, sizeof(Header), 1, pFile);

		if( strcmpi(Header.pMagic, MAGICWORD) )
        {
			fclose(pFile);
			bResult = false;
        }
		else
        {
			for(int i=0; i<Header.nRecords; i++)
            {
                TCustomer* pCustomer = new TCustomer;
                assert(pCustomer);

				pCustomer->Read(pFile);
                m_PtrCustomers.push_back(pCustomer);
            }

            fclose(pFile);
            bResult = true;
		}
    }

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TCustomersArchive::SaveToFile(AnsiString strFileName)
{
	bool bResult = false;

	FILE *pFile = fopen(strFileName.c_str(), "wb");

    if( pFile )
    {
        TFileHeader Header;
        memset(&Header, 0, sizeof(Header));
        strcpy(Header.pMagic, MAGICWORD);
        Header.nRecords = m_PtrCustomers.size();
        fwrite(&Header, sizeof(Header), 1, pFile);

        for(int i=0; i<m_PtrCustomers.size(); i++)
        {
            m_PtrCustomers[i]->Write(pFile);
        }

        fclose(pFile);
        bResult = true;
	}

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCustomersArchive::Clear()
{
    for(int i=0; i<m_PtrCustomers.size(); i++)
    {
    	assert(m_PtrCustomers[i]);
        delete m_PtrCustomers[i];
    }

    m_PtrCustomers.clear();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCustomersArchive::Add(TCustomer *pCustomer)
{
	assert(pCustomer);

	m_PtrCustomers.push_back(pCustomer);
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCustomersArchive::Delete(int nIndex)
{
	if( (nIndex >= 0) && (nIndex < m_PtrCustomers.size()) )
    {
		assert(m_PtrCustomers[nIndex]);
        delete m_PtrCustomers[nIndex];

		m_PtrCustomers.erase( m_PtrCustomers.begin() + nIndex );
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void TCustomersArchive::Update(int nIndex, TCustomer const& Customer)
{
	if( (nIndex >= 0) && (nIndex < m_PtrCustomers.size()) )
    {
		*m_PtrCustomers[nIndex] = Customer;
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
TCustomer* TCustomersArchive::GetRecord(int nId)
{
	TCustomer* pCustomer = NULL;

    if( nId >= 0 && nId < m_PtrCustomers.size() )
    {
    	pCustomer = m_PtrCustomers[nId];
    }

    return pCustomer;
}

/*!****************************************************************************
* @brief
******************************************************************************/
int TCustomersArchive::GetCount()
{
	//return m_Customers.size();
    return m_PtrCustomers.size();
}

/*!****************************************************************************
* @brief
******************************************************************************/
TVecPtrCustomers TCustomersArchive::GetCustomers()
{
	return m_PtrCustomers;
}

/*!****************************************************************************
* @brief
******************************************************************************/
bool TCustomersArchive::GetRecordId(TCustomer* pCustomer, unsigned& nId)
{
	bool bResult = false;

	for(int i=0; i<m_PtrCustomers.size(); i++)
    {
		if( m_PtrCustomers[i] == pCustomer )
        {
        	nId = i;

            bResult = true;
            break;
        }
    }

    return bResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByFirstName(TVecPtrCustomers& PtrCustomers, bool bAsc)
{
	if( PtrCustomers.size() )
    {
        std::vector<TStringSort> Temp;

        for(int i=0; i<PtrCustomers.size(); i++)
        {
			Temp.push_back(TStringSort(PtrCustomers[i]->m_strFirstName, (void*) PtrCustomers[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrCustomers.size());

        TVecPtrCustomers TempPtrCustomers = PtrCustomers;


        for(int i=0; i<Temp.size(); i++)
        {
            PtrCustomers[i] = static_cast<TCustomer*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrCustomers.begin(), PtrCustomers.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByLastName(TVecPtrCustomers& PtrCustomers, bool bAsc)
{
	if( PtrCustomers.size() )
    {
        std::vector<TStringSort> Temp;

        for(int i=0; i<PtrCustomers.size(); i++)
        {
			Temp.push_back(TStringSort(PtrCustomers[i]->m_strLastName, (void*) PtrCustomers[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrCustomers.size());

        TVecPtrCustomers TempPtrCustomers = PtrCustomers;


        for(int i=0; i<Temp.size(); i++)
        {
            PtrCustomers[i] = static_cast<TCustomer*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrCustomers.begin(), PtrCustomers.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByCity(TVecPtrCustomers& PtrCustomers, bool bAsc)
{
	if( PtrCustomers.size() )
    {
        std::vector<TStringSort> Temp;

        for(int i=0; i<PtrCustomers.size(); i++)
        {
			Temp.push_back(TStringSort(PtrCustomers[i]->m_strCity, (void*) PtrCustomers[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrCustomers.size());

        TVecPtrCustomers TempPtrCustomers = PtrCustomers;


        for(int i=0; i<Temp.size(); i++)
        {
            PtrCustomers[i] = static_cast<TCustomer*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrCustomers.begin(), PtrCustomers.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void SortByBudget(TVecPtrCustomers& PtrCustomers, bool bAsc)
{
	if( PtrCustomers.size() )
    {
        std::vector<TIntegerSort> Temp;

        for(int i=0; i<PtrCustomers.size(); i++)
        {
			Temp.push_back(TIntegerSort(PtrCustomers[i]->m_nBudget, (void*) PtrCustomers[i]));
        }

#ifdef _USECUSTOMSORT
		sort::Sort(&Temp[0], Temp.size());
#else
        std::sort(Temp.begin(), Temp.end());
#endif

        assert(Temp.size() == PtrCustomers.size());

        TVecPtrCustomers TempPtrCustomers = PtrCustomers;

        for(int i=0; i<Temp.size(); i++)
        {
            PtrCustomers[i] = static_cast<TCustomer*>(Temp[i].m_pPtr);
        }

        if( !bAsc ) std::reverse(PtrCustomers.begin(), PtrCustomers.end());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByFirstName(TVecPtrCustomers& Customers,
	AnsiString strFirstName, bool bFullMatch)
{
    TVecPtrCustomers TempQuery = Customers;
    Customers.clear();

	if( bFullMatch )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strFirstName).LowerCase() == strFirstName.LowerCase() )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
    }
    else
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strFirstName).LowerCase().Pos( strFirstName.LowerCase() ) )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByLastName(TVecPtrCustomers& Customers, AnsiString strLastName, bool bFullMatch)
{
    TVecPtrCustomers TempQuery = Customers;
    Customers.clear();

	if( bFullMatch )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strLastName).LowerCase() == strLastName.LowerCase() )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
    }
    else
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strLastName).LowerCase().Pos( strLastName.LowerCase() ) )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByCity(TVecPtrCustomers& Customers, AnsiString strCity, bool bFullMatch)
{
    TVecPtrCustomers TempQuery = Customers;
    Customers.clear();

	if( bFullMatch )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strCity).LowerCase() == strCity.LowerCase() )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
    }
    else
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( AnsiString(TempQuery[i]->m_strCity).LowerCase().Pos( strCity.LowerCase() ) )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByOwnership(TVecPtrCustomers& Customers, enCustomerOwnershipType nOwnership)
{
    TVecPtrCustomers TempQuery = Customers;

    Customers.clear();

    for(int i=0; i<TempQuery.size(); i++)
    {
        if( TempQuery[i]->m_nOwnership == nOwnership )
        {
            Customers.push_back(TempQuery[i]);
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
void FilterByBudget(TVecPtrCustomers& Customers, unsigned nBudget, AnsiString strIneq)
{
    TVecPtrCustomers TempQuery = Customers;

    Customers.clear();

    if( strIneq == "=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nBudget == nBudget )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
	}
    else if( strIneq == ">=" )
	{
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nBudget >= nBudget )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
    }
    else if( strIneq == "<=" )
    {
        for(int i=0; i<TempQuery.size(); i++)
        {
            if( TempQuery[i]->m_nBudget <= nBudget )
            {
                Customers.push_back(TempQuery[i]);
            }
        }
    }
}
