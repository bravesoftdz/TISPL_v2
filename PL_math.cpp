//---------------------------------------------------------------------------
#pragma hdrstop
#include "PL_math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny TT
void TPL_math::input_TT()
{
	if(aRD>0 && Rz>0)//Pokud honodty aRD a Rz nejsou k dispozici (jsou bu� k dispozici ob� nebo ��dn� - vypl�v� z dal��ch n�e uveden�ch bod�), tak neprob�h� ��dn� v�po�et, pokud jsou k dispozici, tak je dle z�mk� na aRD a Rz volena jedna z n�sleduj�c�ch variant
	{
		if(aRD_locked)input_Rz(false);
		else aRD=m.RD(Rz);
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny aRD
void TPL_math::input_aRD()
{
	if(aRD>0)
	{
		Rz=m.Rz(aRD);
		input_Rz(false);
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny R
void TPL_math::input_R()
{
	if(aRD>0 || Rx>0)//pokud alespo� jedna z hodnot je nenulov�
	{
		if(aRD_locked && aRD>0 || Rx<=0)
		{
			Rx=m.Rx(aRD,R);
		}
		else
		{
			if(Rx>0/*&& m.kontrola_zda_zmena_R_ovlivni_RzRD(double R_puvodni,double R_nove)*/)
			{
				aRD=m.RD(m.Rz(Rx,R));
				Rz=m.Rz(Rx,R);
			}
		}
	}
}
//---------------------------------------------------------------------------
//pokud je parametr prepocet_aRD=false, nebude se znovu p�epo��t�vat aRD (slou�� pro situaci, kdy input_aRD vol� input_Rz)
void TPL_math::input_Rz(bool prepocet_aRD)
{
	if(Rz>0)
	{
		if(prepocet_aRD)aRD=m.RD(Rz);

		if(R>0 || Rx>0)//pokud alespo� jedna z hodnot je nenulov�
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
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny Rx
void TPL_math::ipnut_Rx()
{
	if(aRD>0 || R>0)//Pokud chyb� hodnoty R i aRD resp. Rz, tak neprob�h� ��dn� v�po�et.
	{
		if(aRD_locked && aRD>0 || R<=0)//pokud nen� aRD resp. Rz k dispozici, rovnou se p�ejde na v�po�et n�sleduj�c�, nebo naopak nen� R k dispozici
		{
			R=m.R(Rz,Rx);
		}
		else
		{
			aRD=m.RD(m.Rz(Rx,R));
			Rz=m.Rz(Rx,R);
		}
	}
}
//---------------------------------------------------------------------------



