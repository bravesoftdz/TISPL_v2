//---------------------------------------------------------------------------
#pragma hdrstop
#include "PO_math.h"
#include "math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny CT
void TPO_math::input_CT()
{
	if(RD_locked)DD=RD*CT;//pro p��pad RD zam�eno
	else RD=DD/CT;				//pro p��pad DD zam�eno

	//vyladit
	K=CT/TT;//v�po�et kapacity
	Mezera();
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD
void TPO_math::input_RD()
{
	if(CT_locked)DD=RD*CT;//pro p��pad CT zam�eno
	else CT=DD/RD;				//pro p��pad DD zam�eno
}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny DD
void TPO_math::input_DD()
{
	if(RD_locked)CT=DD/RD;//pro p��pad RD zam�eno
	else RD=DD/CT;				//pro p��pad CT zam�eno
}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K
void TPO_math::input_K()
{

}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny M
void TPO_math::input_M()
{

}
//---------------------------------------------------------------------------
//vr�t� velikost mezery
double TPO_math::Mezera()
{
	if(mV==1)//po�et mezer o jednu men�� ne� po�et voz�k�
	{
		M=(DD-K*dV)/(K-1);
	}
	//else

}
//---------------------------------------------------------------------------
//vr�t� po�et pozic
double TPO_math::Pozice()
{
	 double P=floor(K);//celo��seln� kapacita
	 double DVM=(dV+M)*(K-P);//d�lka necelo��seln� voz�ko mezery v kabin�
	 if(DVM>dV)P++;//nav��� o cel� voz�k
	 else P+=dV*DVM;//nav��� o ��st voz�ku
	 return P;
}
//---------------------------------------------------------------------------
