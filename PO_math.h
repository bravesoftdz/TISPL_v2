//---------------------------------------------------------------------------
#ifndef PO_mathH
#define PO_mathH
#include "my.h"
//---------------------------------------------------------------------------
class TPO_math
{
	public:
		double TT;//takt linky
		unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní,3-stopka
		double CT;//celkový technologický èáas objektu
		double RD;//rychlost dopravníku
		double DD;//délka kabiny
		double K;//kapacita
		double P;//kapacita
		double M;//mezera mezi vozíky
		double dV;//délka vozíku z parametrù linky
		double sV;//šíøka vozíku z parametrù linky
		double Rotace;
		double R;//rozteè palcù aktuálního dopravníku
		short  mV;//rozdíl poèet mezer a vozíkù (vìtšinou o jednu ménì tj. 1)
		bool CT_locked;//stav zámku
		bool RD_locked;//stav zámku
		bool DD_locked;//stav zámku
		AnsiString T;//test

		void input_CT(bool prepocet_K=true);//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT, pokud je parametr prepocet_K=false, nebude se znovu pøepoèítávat K (slouží pro situaci, kdy input_CT volá input_K)
		void input_RD(bool prepocet_M=true);//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD, pokud je parametr prepocet_M=false, nebude se znovu pøepoèítávat M (slouží pro situaci, kdy input_M volá input_RD
		void input_DD();//pøepoèet souvisejících hodnot vyplývajících ze zmìny DD
		void input_K(bool prepocet_CT=true);//pøepoèet souvisejících hodnot vyplývajících ze zmìny K, pokud je parametr prepocet_CT=false, nebude se pøepoèítávat CT (slouží pro situaci, kdy CT/TT<=K)
		void input_P(); //pøepoèet souvisejících hodnot vyplývajících ze zmìny P
		void input_M(bool prepocet_K=true); //pøepoèet souvisejících hodnot vyplývajících ze zmìny M, pokud je parametr prepocet_K=false, bude se pøepoèítavat DD
	private:
		Cmy m;
		double Mezera();//vrátí velikost mezery
		double Pozice();//vrátí poèet pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
		double P2K();//vrátí kapacitu z poètu pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
		double UDV();//vrátí užitnou délku vozíku dle hodnoty rotace

};
//---------------------------------------------------------------------------
#endif
