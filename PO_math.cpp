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
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(RD_locked)DD=RD*CT;//pro p��pad RD zam�eno, CT odem�eno (zaji�t�no v PO)
			if(DD_locked)//pro p��pad DD zam�eno, CT odem�eno (zaji�t�no v PO)
			{
				RD=DD/CT;
				M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice a za v�po�tem RD
			}
			K=CT/TT;//v�po�et kapacity
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2://PP
			K=CT/TT;//v�po�et kapacity
			DD=K*(UDV()+M);//d�lky kabiny
			P=Pozice();//v�po�et po�tu pozic
		break;
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD
void TPO_math::input_RD()
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(CT_locked)DD=RD*CT;//pro p��pad CT zam�eno, RD odem�eno (zaji�t�no v PO)
			if(DD_locked)CT=DD/RD;//pro p��pad DD zam�eno, RD odem�eno (zaji�t�no v PO)
			M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice
			K=CT/TT;//v�po�et kapacity
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2:break;//PP
	}
}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny DD
void TPO_math::input_DD()
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(RD_locked)CT=DD/RD;//pro p��pad RD zam�eno, DD odem�eno (zaji�t�no v PO)
			if(CT_locked)
			{
				RD=DD/CT;//pro p��pad CT zam�eno, DD odem�eno (zaji�t�no v PO)
				M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice
			}
			K=CT/TT;//v�po�et kapacity
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2:
			K=DD/(UDV()+M);//v�po�et kapacity
			CT=TT*K;//v�po�et CT
			P=Pozice();//v�po�et po�tu pozic
		break;//PP
	}
}
//---------------------------------------------------------------------------
//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K
void TPO_math::input_K(bool prepocet_CT)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1:break;//Kontinu�l
			if(prepocet_CT)CT=TT*K;//v�po�et CT
			input_CT();
			//zakazat p�epo�et K

		case 2://PP
			if(prepocet_CT)CT=TT*K;//v�po�et CT
			DD=K*(UDV()+M);//d�lky kabiny
			P=Pozice();//v�po�et po�tu pozic
		break;
	}
}
//---------------------------------------------------------------------------
void TPO_math::input_P()//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny P
{
	K=P2K();//prvn� si vr�t�m zji�t�nou kapacitu
	input_K();//potom pracuji jako p�i vkl�d�n� kapicity
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny M
void TPO_math::input_M()
{
	//zavol�m prvn� v�po�et RD a potom input RD, ale ji� bez aktualizace M?
}
//---------------------------------------------------------------------------
//vr�t� velikost mezery
double TPO_math::Mezera()
{
	(DD-K*dV)/(K-1);
}
//---------------------------------------------------------------------------
//vr�t� po�et pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
double TPO_math::Pozice()
{
	double P=floor(K);//celo��seln� kapacita
	double DV=UDV();
	double DVM=(DV+M)*(K-P);//d�lka ��sti posledn� voz�ko-mezery v kabin�
	if(DVM>=DV)P++;//nav��� o cel� voz�k, proto�e je minim�ln� cel� voz�k v kabin�
	else P+=DVM/DV;//nav��� o ��st voz�ku, proto�e je jenom ��st voz�ku v kabin�
	return P;
}//---------------------------------------------------------------------------
//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
double  TPO_math::P2K()
{
	if(P-floor(P)>0)//pokud je zad�n necelo��seln� po�et pozic
	{
		return (floor(P)*(UDV()+M)+(UDV()*(P-floor(P))))/(UDV()+M);
	}
	else//celo��seln�
	{
		return (P*UDV()+(P-1)*M)/(UDV()+M);
	}
}
//---------------------------------------------------------------------------
//vr�t� u�itnou d�lku voz�ku dle hodnoty rotace
double TPO_math::UDV()
{ //postupn� roz���it o v�po�et dle zadan�ch stup�� nejenom 0 vs. 90
	if(Rotace==0)return dV;//delka voziku
	else return sV;// ���ka voz�ku
}
//---------------------------------------------------------------------------
