//---------------------------------------------------------------------------
#ifndef parametry_mathH
#define parametry_mathH
//---------------------------------------------------------------------------
class TParametry_math
{
	public:
		double TT;//takt linky
		unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesní,3-stopka
		double CT;//celkový technologický èáas objektu
		double RD;//rychlost dopravníku
		double DD;//délka kabiny
		double K;//kapacita
		double M;//mezera mezi vozíky
		double dV;//délka vozíku z parametrù linky
		double sV;//šíøka vozíku z parametrù linky
		double Rotace;
		double R;//rozteè palcù aktuálního dopravníku
		short  mV;//rozdíl poèet mezer a vozíkù (vìtšinou o jednu ménì tj. 1)
		bool CT_locked;//stav zámku
		bool RD_locked;//stav zámku
		bool DD_locked;//stav zámku

		void input_CT();//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT
		void input_RD();//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD
		void input_DD();//pøepoèet  souvisejících hodnot vyplývajících ze zmìny DD
		void input_K();//pøepoèet  souvisejících hodnot vyplývajících ze zmìny K
		void input_M(); //pøepoèet  souvisejících hodnot vyplývajících ze zmìny M
	private:
		double Mezera();//vrátí velikost mezery
};
//---------------------------------------------------------------------------
#endif
