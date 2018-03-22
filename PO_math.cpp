//---------------------------------------------------------------------------
#pragma hdrstop
#include "PO_math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT
void TPO_math::input_CT(bool prepocet_K)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			if(RD_locked)DD=RD*CT;//pro pøípad RD zamèeno, CT odemèeno (zajištìno v PO)
			if(DD_locked)//pro pøípad DD zamèeno, CT odemèeno (zajištìno v PO)
			{
				RD=DD/CT;
				M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice a za výpoètem RD
			}
			if(prepocet_K)K=CT/TT;//výpoèet kapacity
			P=Pozice();//výpoèet poètu pozic
		break;
		case 2://PP
			if(prepocet_K)K=CT/TT;//výpoèet kapacity
			DD=K*(UDV()+M);//délky kabiny
			P=Pozice();//výpoèet poètu pozic
		break;
	}
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD
void TPO_math::input_RD(bool prepocet_M)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			if(CT_locked)DD=RD*CT;//pro pøípad CT zamèeno, RD odemèeno (zajištìno v PO)
			if(DD_locked)CT=DD/RD;//pro pøípad DD zamèeno, RD odemèeno (zajištìno v PO)
			if(prepocet_M)M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice
			K=CT/TT;//výpoèet kapacity
			P=Pozice();//výpoèet poètu pozic
		break;
		case 2:break;//PP
	}
}
//---------------------------------------------------------------------------
//pøepoèet  souvisejících hodnot vyplývajících ze zmìny DD
void TPO_math::input_DD()
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			if(RD_locked)CT=DD/RD;//pro pøípad RD zamèeno, DD odemèeno (zajištìno v PO)
			if(CT_locked)
			{
				RD=DD/CT;//pro pøípad CT zamèeno, DD odemèeno (zajištìno v PO)
				M=Mezera();//výpoèet mezery musí být umístìn pøed výpoètem pozice
			}
			K=CT/TT;//výpoèet kapacity
			P=Pozice();//výpoèet poètu pozic
		break;
		case 2:
			K=DD/(UDV()+M);//výpoèet kapacity
			CT=TT*K;//výpoèet CT
			P=Pozice();//výpoèet poètu pozic
		break;//PP
	}
}
//---------------------------------------------------------------------------
//pøepoèet  souvisejících hodnot vyplývajících ze zmìny K
void TPO_math::input_K(bool prepocet_CT)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
			if(prepocet_CT)CT=TT*K;//výpoèet CT
			input_CT(false);//+ zakazaný zpìtný pøepoèet K
			break;
		case 2://PP
			if(!CT_locked)CT=TT*K;//výpoèet CT
			DD=K*(UDV()+M);//délky kabiny
			P=Pozice();//výpoèet poètu pozic
		break;
	}
}
//---------------------------------------------------------------------------
void TPO_math::input_P()//pøepoèet souvisejících hodnot vyplývajících ze zmìny P
{
	K=P2K();//prvnì si vrátím zjištìnou kapacitu
	input_K();//potom pracuji jako pøi vkládání kapacity
	//možná zde bude nutné zakázat zpìtný pøepoèet P
}
//---------------------------------------------------------------------------
//pøepoèet souvisejících hodnot vyplývajících ze zmìny M
void TPO_math::input_M(bool prepocet_K)
{
	switch (rezim)
	{
		case 0:break;//S&G
		case 1://Kontinuál
				RD=(UDV()+M)/TT;//dle mezery spoèítám novou rychlost
				input_RD(false);//již znovu nepøepoèítává mezeru
				break;
		case 2://PP
				if(DD_locked)input_DD();
				else input_K();
		break;
	}
}
//---------------------------------------------------------------------------
//vrátí velikost mezery dle aktuální rychlosti RD, nehledí na rozteè
double TPO_math::Mezera()
{
	return RD*TT-m.UDV(dV,sV,Rotace);
}
//---------------------------------------------------------------------------
//vrátí poèet pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
double TPO_math::Pozice()
{
	double P=floor(K);//celoèíselná kapacita
	double DV=UDV();
	double DVM=(DV+M)*(K-P);//délka èásti poslední vozíko-mezery v kabinì
	if(DVM>=DV)P++;//navýší o celý vozík, protože je minimálnì celý vozík v kabinì
	else P+=DVM/DV;//navýší o èást vozíku, protože je jenom èást vozíku v kabinì
	return P;
}//---------------------------------------------------------------------------
//vrátí kapacitu z poètu pozic, øeší i situaci, kdy je M (mezera) nulová, tj. situace, že K==P
double TPO_math::P2K()
{
	if(P-floor(P)>0)//pokud je zadán neceloèíselný poèet pozic
	{
		return (floor(P)*(UDV()+M)+(UDV()*(P-floor(P))))/(UDV()+M);
	}
	else//celoèíselný
	{
		return (P*UDV()+(P-1)*M)/(UDV()+M);
	}
}
//---------------------------------------------------------------------------
//vrátí užitnou délku vozíku dle hodnoty rotace
double TPO_math::UDV()
{
	return m.UDV(dV,sV,Rotace);
}
//---------------------------------------------------------------------------
