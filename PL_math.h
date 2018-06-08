//---------------------------------------------------------------------------
#ifndef PL_mathH
#define PL_mathH
#include "my.h"
//---------------------------------------------------------------------------
class TPL_math
{
	public:
		double TT;//takt linky
		double aRD;//aktuální rychlost aktuálnì øešeného pohonu
		double R;//rozteè palcù aktuálnì øešeného pohonu
		double Rz;//rozestup aktivních palcù v m
		double Rx;//rozestup aktivních palcù (poèet aktivních palcù)

		bool aRD_locked;//stav zámku
		bool R_locked;//stav zámku
		bool Rz_locked;//stav zámku
		bool Rx_locked;//stav zámku

		AnsiString T;//text pro testovací výpis

    //plnohodnotná dokumentace k metodám se nachází v aDoc a pDoc
		void input_TT();//pøepoèet souvisejících hodnot vyplývajících ze zmìny TT
		void input_aRD();//pøepoèet souvisejících hodnot vyplývajících ze zmìny aRD
		void input_R();//pøepoèet souvisejících hodnot vyplývajících ze zmìny R
		void input_Rz(bool prepocet_aRD=true);//pokud je parametr prepocet_aRD=false, nebude se znovu pøepoèítávat aRD (slouží pro situaci, kdy input_aRD volá input_Rz)
		void ipnut_Rx();//pøepoèet souvisejících hodnot vyplývajících ze zmìny Rx

	private:
		Cmy m;
};
//---------------------------------------------------------------------------
#endif
