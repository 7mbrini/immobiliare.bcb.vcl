/*!****************************************************************************

	@file	maths.h
	@file	maths.cpp

	@brief	Maths routines

	@noop	author:	Francesco Settembrini
	@noop	last update: 23/6/2021
	@noop	e-mail:	mailto:francesco.settembrini@poliba.it

******************************************************************************/

#include <assert.h>
#include <algorithm>

#include <math.h>

#include "maths.h"

namespace maths
{

/*!****************************************************************************
* @brief	Generates a random value in a specified range
* @param	Val The interval absolute limits in which a value can be generated
******************************************************************************/
double Rand(double Val)
{
	return double( Val -  2.0 * rand()/double(RAND_MAX) * Val );
}

/*!****************************************************************************
* @brief	Generates a random value in a specified range
* @param	Val The interval limits in which a value can be generated
******************************************************************************/
double AbsRand(double Val)
{
	return double( rand() / double(RAND_MAX) * Val );
}

/*!****************************************************************************
* @brief	Randomly generates a unitary value with sign
* @return	Returns an +1 or -1 value
******************************************************************************/
int RandSign()
{
	double RandVal = -1.0 + 2.0*rand()/double(RAND_MAX);
	
	return maths::Sign(RandVal);
}

/*!****************************************************************************
* @brief	Gets the sign of a specified value
* @return	Returns an +1 or -1 value
******************************************************************************/
int Sign(double Val)
{
	return Val >= 0 ? 1.0 : -1.0;
}

/*!****************************************************************************
* @brief	Generates an integer random value in a specified range Min,Max
* @param	nMin The minimum value for the interval
* @param	nMax The maximum value for the interval
******************************************************************************/
int Rand(int nMin, int nMax)
{
	return nMin + rand() % (( nMax + 1 ) - nMin);
}

}	// namespace maths