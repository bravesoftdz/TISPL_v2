//---------------------------------------------------------------------------
#ifndef PO_mathH
#define PO_mathH
#include "my.h"
//---------------------------------------------------------------------------
class TPO_math
{
	public:
		double TT;//takt linky
		unsigned short rezim;//rezim objektu 0-S&G,1-Kontin.(line tracking),2-Postprocesn�,3-stopka
		double CT;//celkov� technologick� ��as objektu
		double RD;//rychlost dopravn�ku
		double DD;//d�lka kabiny
		double K;//kapacita
		double P;//kapacita
		double M;//mezera mezi voz�ky
		double dV;//d�lka voz�ku z parametr� linky
		double sV;//���ka voz�ku z parametr� linky
		double Rotace;
		double R;//rozte� palc� aktu�ln�ho dopravn�ku
		double Rz;//rozestup aktivn�ch palc� v m
		double Rx;//rozestup aktivn�ch palc� (po�et aktivn�ch palc�)
		bool CT_locked;//stav z�mku
		bool RD_locked;//stav z�mku
		bool DD_locked;//stav z�mku
		bool K_locked;//stav z�mku
		AnsiString T;//text pro testtest

		void input_CT(bool prepocet_K=true);//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny CT, pokud je parametr prepocet_K=false, nebude se znovu p�epo��t�vat K (slou�� pro situaci, kdy input_CT vol� input_K)
		void input_RD(bool prepocet_M=true);//p�epo�et souvisej�c�ch  hodnot vypl�vaj�c�ch ze zm�ny RD, pokud je parametr prepocet_M=false, nebude se znovu p�epo��t�vat M (slou�� pro situaci, kdy input_M vol� input_RD
		void input_DD();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny DD
		void input_K(bool prepocet_CT=true);//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny K, pokud je parametr prepocet_CT=false, nebude se p�epo��t�vat CT (slou�� pro situaci, kdy CT/TT<=K)
		void input_P(bool prepocet_CT=true); //p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny P
		void input_M(); //p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny M, pokud je parametr prepocet_K=false, bude se p�epo��tavat DD
		double K2P(double K);//vr�t� po�et pozic z kapacity, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
		double P2K(double P);//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
	private:
		Cmy m;
		double Mezera();//vr�t� velikost mezery dle aktu�ln� rychlosti RD, nehled� na rozte�, ale rovnou po��t� Rx,Rz-testov�n�
		double P2K();//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
		double Pozice();//vr�t� po�et pozic z kapacity, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
		double UDV();//vr�t� u�itnou d�lku voz�ku dle hodnoty rotace

};
//---------------------------------------------------------------------------
#endif