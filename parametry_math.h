//---------------------------------------------------------------------------

#ifndef parametry_mathH
#define parametry_mathH
class TParametry_math
{
	public:
		struct TPO
		{
			double CT;//celkový technologický èáas objektu
			double RD;//rychlost dopravníku
			double DD;//délka kabiny
			double K;//kapacita
			double M;//mezera mezi vozíky
			double dV;//užitná délka vozíku (tj. strana dle rotace v objektu)
			short  mV;//rozdíl poèet mezer a vozíkù (vìtšinou o jednu ménì tj. 1)
		};
		TPO PO;
		void input_CT();//pøepoèet souvisejících hodnot vyplývajících ze zmìny CT
		void input_RD();//pøepoèet souvisejících  hodnot vyplývajících ze zmìny RD
		void input_DD();//pøepoèet  souvisejících hodnot vyplývajících ze zmìny DD
		void input_K();//pøepoèet  souvisejících hodnot vyplývajících ze zmìny K
		void input_M(); //pøepoèet  souvisejících hodnot vyplývajících ze zmìny M
	private:
};
//---------------------------------------------------------------------------
#endif
