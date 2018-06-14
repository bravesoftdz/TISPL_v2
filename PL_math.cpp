//---------------------------------------------------------------------------
#pragma hdrstop
#include "PL_math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny TT
void TPL_math::input_TT()
{
	if(aRD>0 && Rz>0)//Pokud honodty aRD a Rz nejsou k dispozici (jsou buï k dispozici obì nebo žádná - vyplývá z dalších níže uvedených bodù), tak neprobíhá žádný výpoèet, pokud jsou k dispozici, tak je dle zámkù na aRD a Rz volena jedna z následujících variant
	{
		if(aRD_locked){Rz=TT*aRD;input_Rz(false);}
		else aRD=Rz/TT;
	}
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny aRD
void TPL_math::input_aRD()
{
	if(aRD>0)
	{
		Rz=aRD*TT;
		input_Rz(false);
	}
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny R
void TPL_math::input_R()
{
	if(aRD>0 || Rx>0)//pokud alespoò jedna z hodnot je nenulová
	{
		if(aRD_locked && aRD>0 || Rx<=0)
		{
			Rx=m.Rx2(aRD*TT,R);
		}
		else
		{
			if(Rx>0/*&& popø.m.kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove)*/)
			{
				Rz=m.Rz(Rx,R);
				aRD=Rz/TT;
			}
		}
	}
}
//---------------------------------------------------------------------------
//pokud je parametr prepocet_aRD=false, nebude se znovu pøepoèítávat aRD (slouží pro situaci, kdy input_aRD volá input_Rz)
void TPL_math::input_Rz(bool prepocet_aRD)
{
	if(Rz>0)
	{
		if(prepocet_aRD)aRD=Rz/TT;

		if(R>0 || Rx>0)//pokud alespoò jedna z hodnot je nenulová
		{
			if(R_locked && R>0 || Rx<=0)Rx=m.Rx(Rz,R);
			else
			{
				if(Rx>0)R=m.R(Rz,Rx);
			}
		}
	}
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny Rx
void TPL_math::ipnut_Rx()
{
	if(aRD>0 || R>0)//Pokud chybí hodnoty R i aRD resp. Rz, tak neprobíhá žádný výpoèet.
	{
		if(aRD_locked && aRD>0 || R<=0)//pokud není aRD resp. Rz k dispozici, rovnou se pøejde na výpoèet následující, nebo naopak není R k dispozici
		{
			R=m.R(Rz,Rx);
		}
		else
		{
			Rz=m.Rz(Rx,R);
			aRD=Rz/TT;
		}
	}
}
//---------------------------------------------------------------------------



