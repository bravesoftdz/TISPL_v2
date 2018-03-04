//---------------------------------------------------------------------------

#ifndef parametry_mathH
#define parametry_mathH
class TParametry_math
{
	public:
		struct TPO
		{
			double CT;//celkov� technologick� ��as objektu
			double RD;//rychlost dopravn�ku
			double DD;//d�lka kabiny
			double K;//kapacita
			double m;//mezera mezi voz�ky
			double dV;//u�itn� d�lka voz�ku (tj. strana dle rotace
			short mV;//rozd�l po�et mezer a voz�k� (v�t�inou o jednu m�n� tj. 1)
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
