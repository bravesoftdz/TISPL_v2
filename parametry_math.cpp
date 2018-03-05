//---------------------------------------------------------------------------
#pragma hdrstop
#include "parametry_math.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT
void TParametry_math::input_CT()
{
	if(RD_locked)DD=RD*CT;//pro pøípad RD zamèeno
	else RD=DD/CT;				//pro pøípad DD zamèeno

	//vyladit
	K=CT/TT;//výpoèet kapacity
	if(mV==1)//poèet mezer o jednu menší než poèet vozíkù
	{
		M=(DD-K*dV)/(K-1);
	}

}
//---------------------------------------------------------------------------
//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD
void TParametry_math::input_RD()
{
	if(CT_locked)DD=RD*CT;//pro pøípad CT zamèeno
	else CT=DD/RD;				//pro pøípad DD zamèeno
}
//---------------------------------------------------------------------------
//pøepoèet  souvisejících hodnot vyplývajících ze zmìny DD
void TParametry_math::input_DD()
{
	if(RD_locked)CT=DD/RD;//pro pøípad RD zamèeno
	else RD=DD/CT;				//pro pøípad CT zamèeno
}
//---------------------------------------------------------------------------
//pøepoèet  souvisejících hodnot vyplývajících ze zmìny K
void TParametry_math::input_K()
{

}
//---------------------------------------------------------------------------
//pøepoèet  souvisejících hodnot vyplývajících ze zmìny M
void TParametry_math::input_M()
{

}
//---------------------------------------------------------------------------
