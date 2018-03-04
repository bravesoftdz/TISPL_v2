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
			double m;//mezera mezi vozíky
			double dV;//užitná délka vozíku (tj. strana dle rotace
			short mV;//rozdíl poèet mezer a vozíkù (vìtšinou o jednu ménì tj. 1)
		};
		TPO PO;
	void input_CT();
	void input_RD();
	void input_DD();
	void input_K();
	void input_M();
	private:
};
//---------------------------------------------------------------------------
#endif
