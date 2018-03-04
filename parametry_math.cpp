//---------------------------------------------------------------------------
#pragma hdrstop
#include "parametry_math.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny CT
void TParametry_math::input_CT()
{
	if(RD_locked)DD=RD*CT;//pro p��pad RD zam�eno
	else RD=DD/CT;//pro p��pad DD zam�eno
	K=CT/TT;//v�po�et kapacity
	if(mV==1)//po�et mezer o jednu men�� ne� po�et voz�k�
	{
		M=(DD-K*dV)/(K-1);
	}

}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD
void TParametry_math::input_RD()
{

}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny DD
void TParametry_math::input_DD()
{

}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K
void TParametry_math::input_K()
{

}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny M
void TParametry_math::input_M()
{

}
//---------------------------------------------------------------------------
