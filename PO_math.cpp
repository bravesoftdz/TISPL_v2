//---------------------------------------------------------------------------
#pragma hdrstop
#include "PO_math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny CT
void TPO_math::input_CT(bool prepocet_K)
{
	switch (rezim)
	{
		case 0:
    RD=DD/CT;//test min.RD
		break;//S&G
		case 1://Kontinu�l
			if(RD_locked)DD=RD*CT;//pro p��pad RD zam�eno, CT odem�eno (zaji�t�no v PO)
			if(DD_locked)//pro p��pad DD zam�eno, CT odem�eno (zaji�t�no v PO)
			{
				RD=DD/CT;
				M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice a za v�po�tem RD
			}
			if(prepocet_K)K=CT/TT;//v�po�et kapacity
			P=Pozice();//v�po�et po�tu pozic
		break;
		case 2://PP
			if(prepocet_K)K=CT/TT;//v�po�et kapacity
			DD=K*(UDV()+M);//d�lky kabiny
			RD=DD/CT;//test min.RD
			P=Pozice();//v�po�et po�tu pozic
		break;
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD
void TPO_math::input_RD(bool prepocet_M)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(CT_locked)DD=RD*CT;//pro p��pad CT zam�eno, RD odem�eno (zaji�t�no v PO)
			if(DD_locked)CT=DD/RD;//pro p��pad DD zam�eno, RD odem�eno (zaji�t�no v PO)
			if(prepocet_M)M=Mezera();//v�po�et mezery mus� b�t um�st�n p�ed v�po�tem pozice
			K=CT/TT;//v�po�et kapacity, v p��pad� zam�en�ho CT se nic nem�n�
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
		case 0:RD=DD/CT;//test min.RD
		break;//S&G
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
			RD=DD/CT;//test min.RD
			P=Pozice();//v�po�et po�tu pozic
		break;//PP
	}
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K
void TPO_math::input_K(bool prepocet_CT)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			if(prepocet_CT)CT=TT*K;//v�po�et CT
			input_CT(false);//+ zakazan� zp�tn� p�epo�et K
			break;
		case 2://PP
			if(prepocet_CT)CT=TT*K;//v�po�et CT
			DD=K*(UDV()+M);//d�lky kabiny
			P=Pozice();//v�po�et po�tu pozic
		break;
	}
}
//---------------------------------------------------------------------------
void TPO_math::input_P(bool prepocet_CT)//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny P
{
	K=P2K();//prvn� si vr�t�m zji�t�nou kapacitu
	input_K(prepocet_CT);//potom pracuji jako p�i vkl�d�n� kapacity
	//mo�n� zde bude nutn� zak�zat zp�tn� p�epo�et P
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny kritick� voz�kov� mezery
void TPO_math::input_M()
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinu�l
			RD=(UDV()+M)/TT;//dle mezery spo��t�m novou rychlost
			input_RD();//d��ve se pou��valo input_RD(false) znovu nep�epo��t�v� mezeru
			break;
		case 2://PP
			if(DD_locked)input_DD();
			else input_K();
			break;
	}
	MJ=M+fabs(m.UDJ(dJ,sJ,Rotace)-m.UDV(dJ,sJ,Rotace));
	MP=M+fabs(dP-m.UDV(dJ,sJ,Rotace));
}
//---------------------------------------------------------------------------
//vr�t� velikost mezery dle aktu�ln� rychlosti RD, nehled� na rozte�, ale rovnou po��t�. Rx,Rz-testov�n�, nevyu�ito
double TPO_math::Mezera()
{
	double mezera=RD*TT-m.UDV(dJ,sJ,Rotace);
	Rx=m.Rx(dJ,sJ,Rotace,mezera,R);
	Rz=m.Rz(dJ,sJ,Rotace,mezera);
	MJ=RD*TT-m.UDJ(dJ,sJ,Rotace);
	MP=RD*TT-dP;
	return mezera;
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny mezery jigu
void TPO_math::input_MJ()
{
	M=MJ-fabs(m.UDJ(dJ,sJ,Rotace)-m.UDV(dJ,sJ,Rotace));
	input_M();
}
//---------------------------------------------------------------------------
//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny mezery podvozku
void TPO_math::input_MP()
{
	M=MP-fabs(dP-m.UDV(dJ,sJ,Rotace));
	input_M();
}
//---------------------------------------------------------------------------
//vr�t� po�et pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
double TPO_math::Pozice()
{
	return K2P(K);
}
//---------------------------------------------------------------------------
//vr�t� po�et pozic z kapacity, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
double TPO_math::K2P(double K)
{
	double P=floor(K);//celo��seln� kapacita
	double DV=UDV();
	double DVM=(DV+M)*(K-P);//d�lka ��sti posledn� voz�ko-mezery v kabin�
	if(DVM>=DV)P++;//nav��� o cel� voz�k, proto�e je minim�ln� cel� voz�k v kabin�
	else P+=DVM/DV;//nav��� o ��st voz�ku, proto�e je jenom ��st voz�ku v kabin�
	return P;
}
//---------------------------------------------------------------------------
double TPO_math::P2K(double P)//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
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
//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. situace, �e K==P
double TPO_math::P2K()
{
	return P2K(P);
}
//---------------------------------------------------------------------------
//vr�t� u�itnou d�lku voz�ku dle hodnoty rotace
double TPO_math::UDV()
{
	return m.UDV(dJ,sJ,Rotace);
}
//---------------------------------------------------------------------------
