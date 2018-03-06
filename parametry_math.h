//---------------------------------------------------------------------------
#ifndef parametry_mathH
#define parametry_mathH
//---------------------------------------------------------------------------
class TParametry_math
{
	public:
		double TT;//takt linky
		unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesn�,3-stopka
		double CT;//celkov� technologick� ��as objektu
		double RD;//rychlost dopravn�ku
		double DD;//d�lka kabiny
		double K;//kapacita
		double M;//mezera mezi voz�ky
		double dV;//d�lka voz�ku z parametr� linky
		double sV;//���ka voz�ku z parametr� linky
		double Rotace;
		double R;//rozte� palc� aktu�ln�ho dopravn�ku
		short  mV;//rozd�l po�et mezer a voz�k� (v�t�inou o jednu m�n� tj. 1)
		bool CT_locked;//stav z�mku
		bool RD_locked;//stav z�mku
		bool DD_locked;//stav z�mku

		void input_CT();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny CT
		void input_RD();//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD
		void input_DD();//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny DD
		void input_K();//p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K
		void input_M(); //p�epo�et  souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny M
	private:
		double Mezera();//vr�t� velikost mezery
};
//---------------------------------------------------------------------------
#endif
