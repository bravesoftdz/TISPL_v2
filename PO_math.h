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
		double CT;//celkový technologický èás objektu
		double PT;//procesní èas - prozatím využito jen v GAPO
		double MT1;//èas pøesunu objektu 1  - prozatím využito jen v GAPO
		double MT2;//èas pøesunu objektu 1 - prozatím využito jen v GAPO
		double WT1;//èas èekání 1 - prozatím využito jen v GAPO
		double WT2;//èas èekání 2  - prozatím využito jen v GAPO
		double RD;//rychlost dopravníku
		double DD;//délka kabiny
		double K;//kapacita
		double P;//pozice
		double M;//mezera mezi vozíky
		double MJ;//mezera mezi jig
		double MP;//mezera mezi podvozky
		double dJ;//délka jigu z parametrù linky
		double sJ;//šíøka jigu z parametrù linky
		double dP;//délka jigu z parametrù linky
		double Rotace;
		double R;//rozteè palcù aktuálního dopravníku
		double Rz;//rozestup aktivních palcù v m
		double Rx;//rozestup aktivních palcù (poèet aktivních palcù)
		bool CT_locked;//stav zámku
		bool RD_locked;//stav zámku
		bool DD_locked;//stav zámku
		bool K_locked;//stav zámku
		AnsiString T;//text pro testtest

		void input_CT(bool prepocet_K=true);//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT, pokud je parametr prepocet_K=false, nebude se znovu pøepoèítávat K (slouží pro situaci, kdy input_CT volá input_K)
		void input_RD(bool prepocet_M=true);//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD, pokud je parametr prepocet_M=false, nebude se znovu pøepoèítávat M (slouží pro situaci, kdy input_M volá input_RD
		void input_DD();//pøepoèet souvisejících hodnot vyplývajících ze zmìny DD
		void input_K(bool prepocet_CT=true);//pøepoèet souvisejících hodnot vyplývajících ze zmìny K, pokud je parametr prepocet_CT=false, nebude se pøepoèítávat CT (slouží pro situaci, kdy CT/TT<=K)
		void input_P(bool prepocet_CT=true); //pøepoèet souvisejících hodnot vyplývajících ze zmìny P
		void input_M(); //pøepoèet souvisejících hodnot vyplývajících ze zmìny kritické vozíkové mezery
		void input_MJ();//pøepoèet souvisejících hodnot vyplývajících ze zmìny mezery jigu
		void input_MP();//pøepoèet souvisejících hodnot vyplývajících ze zmìny mezery podvozku
		double Pozice();//vrátí poèet pozic z kapacity, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
		double K2P(double K);//vrátí poèet pozic z kapacity, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
		double P2K(double P);//vrátí kapacitu z poètu pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
		double Mezera();//vrátí velikost mezery dle aktuální rychlosti RD, nehledí na rozteè, ale rovnou poèítá Rx,Rz-testování (to se nepoužívá, protože se používá nacti_rx pøímo v PO)
		double UDV();//vrátí užitnou délku vozíku dle hodnoty rotace
	private:
		Cmy m;
		double P2K();//vrátí kapacitu z poètu pozic, øeší i situaci, kdy je M (mezera) nulová, tj. K==P
};
//---------------------------------------------------------------------------
#endif
