/*!****************************************************************************

	@file	statistics.h
	@file	statistics.cpp

	@brief	Statistics routines

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/


#include <vcl.h>
#pragma hdrstop

#include <vector>
#include <algorithm>


#include "statistics.h"

using namespace std;

namespace stats
{

/*!****************************************************************************
* @brief
******************************************************************************/
double Min(TVecDoubles& Values)
{
	assert(Values.size());

	double Min = Values[0];

	for(size_t i = 0; i < Values.size(); ++i)
	{
		if (Min > Values[i]) Min = Values[i];
	}

	return Min;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Max(TVecDoubles& Values)
{
	assert(Values.size());

	double Max = Values[0];

	for(size_t i = 0; i < Values.size(); ++i)
	{
		if (Max < Values[i]) Max = Values[i];
	}

	return Max;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Min(TVecDoubles& Values, int& nIndex)
{
	assert(Values.size());

	double Min = Values[0];
	nIndex = 0;

	for(size_t i = 0; i < Values.size(); i++)
	{
		if (Min > Values[i])
		{
			Min = Values[i];
			nIndex = i;
		}
	}

	return Min;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Max(TVecDoubles& Values, int& nIndex)
{
	assert(Values.size());

	double Max = Values[0];
	nIndex = 0;

	for(size_t i = 0; i < Values.size(); i++)
	{
		if (Max < Values[i])
		{
			Max = Values[i];
			nIndex = i;
		}
	}

	return Max;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double RMS(TVecDoubles& Values)
{
	double Rms = 0;
	int nCount = Values.size();

	if( nCount )
	{
		for(int i=0; i<nCount;i++)
		{
			Rms += Values[i]*Values[i];
		}

		Rms = sqrt(Rms / nCount);
	}

	return Rms;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Mean(TVecDoubles& Values)
{
	assert(Values.size());

	double Mean = 0;

	int nCount = Values.size();

	if (nCount)
	{
		for (int i = 0; i < nCount; i++)
		{
			Mean += Values[i];
		}

		Mean /= double(nCount);
	}

	return Mean;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Median(TVecDoubles Values)
{
	assert(Values.size());

	double Median = 0;
	int nSize = Values.size();

	if (nSize)
	{
		std::sort(Values.begin(), Values.end());

		if (!(nSize % 2))
		{
                                            // se il numero di elementi del vettore e` pari
                                            // il valore mediano e` dato dalla media dei due
                                            // valori centrali
			int nPos = floor(nSize / 2.0f) - 1;
			Median = (Values[nPos] + Values[nPos + 1]) / 2.0f;
		}
		else
		{
                                            // se il numero di elementi del vettore e' dispari
                                            // il valore "mediano"` quello del valore centrale
			Median = Values[nSize / 2];
		}
	}

	return Median;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Variance(TVecDoubles& Values)
{
	assert(Values.size());

	double Variance = 0;
	int nSize = Values.size();

	if (nSize)
	{
		double Mean = stats::Mean(Values);

		for (int i = 0; i < nSize; i++)
		{
			Variance += (Values[i] - Mean)*(Values[i] - Mean);
		}

		Variance /= double(nSize);
	}

	return Variance;
}

/*!****************************************************************************
* @brief	Calculates the Standard Deviation
******************************************************************************/
double StdDev(TVecDoubles& Values)
{
	return sqrt(Variance(Values));
}


/*!****************************************************************************
* @brief
******************************************************************************/
struct THistoBar
{
    THistoBar() { Min = Max = Value = 0; nFreqs = 0; }

    void SetInterval(double Min, double Max)
    {
        this->Min = Min;
        this->Max = Max;
    }

    void Add(double Value)
    {
        if ((Value >= Min) && (Value < Max))
        {
            this->Value = (this->Value + Value) / 2.0f;
            nFreqs++;
        }
    }

    int nFreqs;
    double Min, Max, Value;
};

/*!****************************************************************************
* @brief
******************************************************************************/
double Mode(TVecDoubles Values, int nClasses)
{
	assert(nClasses > 0);

	double Mode = 0;
	int nSize = Values.size();
                                            // un istogramma altro non e'
                                            // che un insieme ordinato di
                                            // intervalli (bars)
	typedef vector<THistoBar> TVecHistoBars;

	TVecHistoBars Histogram;
	Histogram.resize(nClasses);
	assert(Histogram.size() == nClasses);

	if ((nSize >= 2) && (nClasses > 0))
	{
		std::sort(Values.begin(), Values.end());

		double Min = Values[0];
		double Max = Values[nSize - 1];
		assert(Min == stats::Min(Values));
		assert(Max == stats::Max(Values));

		double Delta = (Max - Min) / double(nClasses);

		if (Delta > 0)
		{
			for (int i = 0; i < Histogram.size(); i++)
			{
											// imposta i limiti di classe
				//Histogram[i].SetInterval(i*Delta, (i+1)*Delta);
				Histogram[i].SetInterval(Min + i * Delta, Min + (i + 1)*Delta);

                                            // ... e vede se nella classe
                                            // appena definita vi ricadono valori
				for (int j = 0; j < Values.size(); j++)
				{
					Histogram[i].Add(Values[j]);
				}
			}
                                            // la moda e` data dalla classe
                                            // col maggior numero di frequenze
			int nPos = -1;
			int nFreqs = nPos;

			for (int i = 0; i < Histogram.size(); i++)
			{
				if (Histogram[i].nFreqs > nFreqs)
				{
					nFreqs = Histogram[i].nFreqs;
					nPos = i;
				}
			}

			if (nPos >= 0)
			{
                                            // per la moda e` necessario
                                            // avere **almeno 2 ** occorrenze
                                            // in una classe di valori
				if (Histogram[nPos].nFreqs > 1)
				{
					Mode = Histogram[nPos].Value;
				}
			}
		}
	}

	return Mode;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void Sort(TVecDoubles& Values, bool bAscending)
{
	if (bAscending)
	{
		std::sort(Values.begin(), Values.end());
	}
	else
	{
		std::sort(Values.end(), Values.begin());
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
double Sum(TVecDoubles& Values)
{
	double Sum = 0;

	for (int i = 0; i < Values.size(); i++)
	{
		Sum += Values[i];
	}

	return Sum;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void Outliers(TVecDoubles Values, double &OutMin, double &OutMax)
{
	assert(Values.size());

	int nSize = Values.size();

	if (nSize)
	{
		std::sort(Values.begin(), Values.end());

		double Min = Values[nSize * 1.0/4.0f];
		double Max = Values[nSize * 3.0/4.0f];
		double Dist = Max - Min;

		OutMin = Min - 3.0*Dist;		// 3.0 = strong outliers; 1.5 = weak outliers
		OutMax = Max + 3.0*Dist;
	}
}

/*!****************************************************************************
* @brief
******************************************************************************/
THistogram::THistogram(double Min, double Max, int nClasses)
{
	Setup(Min, Max, nClasses);
}

/*!****************************************************************************
* @brief
******************************************************************************/
THistogram::THistogram(TVecDoubles Vals, int nClasses)
{
    assert(Vals.size());

    m_Min = Min(Vals);
    m_Max = Max(Vals);

    Setup(m_Min, m_Max, nClasses);

    for(int i=0; i<Vals.size(); i++)
    {
        Add(Vals[i]);
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
double THistogram::GetMin()
{
	return m_Min;
}

/*!****************************************************************************
* @brief
******************************************************************************/
double THistogram::GetMax()
{
	return m_Max;
}

/*!****************************************************************************
* @brief
******************************************************************************/
int THistogram::GetClasses()
{
	return m_Values.size();
}

/*!****************************************************************************
* @brief
******************************************************************************/
void THistogram::Add(double Value)
{
    for(int i=0; i<m_Values.size(); i++)
    {
        if( i<(m_Values.size()-1) )
        {
            if( (Value >= m_Values[i].Value) && (Value < m_Values[i+1].Value) )
            {
                m_Values[i].nFreqs++;
                break;
            }
        }
        else
        {
            if( (Value >= m_Values[i].Value) )
            {
                m_Values[i].nFreqs++;
            }
        }
    }
}

/*!****************************************************************************
* @brief
******************************************************************************/
THistogram::TVecHistPairs THistogram::GetValues()
{
	return m_Values;
}

/*!****************************************************************************
* @brief
******************************************************************************/
TVecDoubles THistogram::GetNormalizedFreqs()
{
    double MaxFreq = 0;
    TVecDoubles NormFreqs;

    for(int i=0; i<m_Values.size(); i++)
    {
        NormFreqs.push_back(m_Values[i].nFreqs);
        if( MaxFreq < m_Values[i].nFreqs ) MaxFreq = m_Values[i].nFreqs;
    }

    if( MaxFreq > 0 )
    {
        for(int i=0; i<NormFreqs.size(); i++)
        {
            NormFreqs[i] /= MaxFreq;
        }
    }

    return NormFreqs;
}

/*!****************************************************************************
* @brief
******************************************************************************/
AnsiString THistogram::Print()
{
    AnsiString strResult;

    strResult.printf("Histogram classes = %d", GetClasses() );

    for(int i=0; i<m_Values.size(); i++)
    {
        AnsiString strTemp;
        strTemp.printf("\nclass: %d , value: %6.3f , freqs: %d",
            i,
            m_Values[i].Value,
            m_Values[i].nFreqs
            );

        strResult += strTemp;
    }

    return strResult;
}

/*!****************************************************************************
* @brief
******************************************************************************/
void THistogram::Setup(double Min, double Max, int nClasses)
{
    //assert(nClasses>1);
    assert(nClasses > 0);
    assert(Min <= Max);

    m_Min = Min;
    m_Max = Max;

    m_Values.resize(nClasses);

    for(int i=0; i<m_Values.size(); i++)
    {
        m_Values[i].Value = m_Min + (m_Max - m_Min) / double(nClasses) * i;
    }
}

};	// namespace stats


