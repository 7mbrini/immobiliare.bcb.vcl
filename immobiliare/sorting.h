/*!****************************************************************************

	@file	sorting.h

	@brief	Sorting routines

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#ifndef sortingH
#define sortingH

#include <vector>
#include <algorithm>


namespace sort
{

class T;

/*!****************************************************************************
* @brief	Reorders a vector of T
* @param	VecInts A reference to a vector of T
* @note		Uses the Bubble Sort algorithm
******************************************************************************/
template <class T>
void Sort(T *pT, int nSize)
{
	assert(pT);

	for(int i=0; i<nSize; i++)
	{
		for(int j = i+1; j<nSize; j++)
		{
            if( pT[j] < pT[i] )
			{
                std::swap(pT[i], pT[j]);
			}
		}
	}
}


typedef std::vector<int> TVecIntegers;

/*!****************************************************************************
* @brief	Reorders a vector of integer values
* @param	VecInts A reference to a vector of integers
* @note		Uses the Bubble Sort algorithm
******************************************************************************/
void Sort(TVecIntegers& VecInts)
{
	for(int i=0; i<VecInts.size(); i++)
	{
		for(int j = i+1; j<VecInts.size(); j++)
		{
			if( VecInts[i] > VecInts[j] )
			{
				std::swap(VecInts[i], VecInts[j]);
			}
		}
	}
}

class TStringSort
{
public:
	TStringSort()
    {
    	m_pPtr = NULL;
		m_strName = "";
    }

	TStringSort(AnsiString strName, void* pPtr)
    {
		m_strName = strName;
        m_pPtr = pPtr;
    }

    bool operator < (TStringSort& Other)
    {
		return bool(m_strName < Other.m_strName);
    }

public:
    void* m_pPtr;
    AnsiString m_strName;
};

class TIntegerSort
{
public:
	TIntegerSort()
    {
    	m_nVal = 0;
        m_pPtr = NULL;
    }

	TIntegerSort(int nVal, void* pPtr)
    {
		m_nVal = nVal;
        m_pPtr = pPtr;
    }

	bool operator < (TIntegerSort& Other)
    {
    	return bool(m_nVal < Other.m_nVal);
    }

public:
	int m_nVal;
    void* m_pPtr;
};

};

//---------------------------------------------------------------------------
#endif
