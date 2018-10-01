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
		double CT;//celkov� technologick� ��s objektu
		double PT;//procesn� �as - prozat�m vyu�ito jen v GAPO
		double MT1;//�as p�esunu objektu 1  - prozat�m vyu�ito jen v GAPO
		double MT2;//�as p�esunu objektu 1 - prozat�m vyu�ito jen v GAPO
		double WT1;//�as �ek�n� 1 - prozat�m vyu�ito jen v GAPO
		double WT2;//�as �ek�n� 2  - prozat�m vyu�ito jen v GAPO
		double RD;//rychlost dopravn�ku
		double DD;//d�lka kabiny
		double K;//kapacita
		double P;//pozice
		double M;//mezera mezi voz�ky
		double MJ;//mezera mezi jig
		double MP;//mezera mezi podvozky
		double dJ;//d�lka jigu z parametr� linky
		double sJ;//���ka jigu z parametr� linky
		double dP;//d�lka jigu z parametr� linky
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
		void input_M(); //p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny kritick� voz�kov� mezery
		void input_MJ();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny mezery jigu
		void input_MP();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny mezery podvozku
		double Pozice();//vr�t� po�et pozic z kapacity, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
		double K2P(double K);//vr�t� po�et pozic z kapacity, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
		double P2K(double P);//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
		double Mezera();//vr�t� velikost mezery dle aktu�ln� rychlosti RD, nehled� na rozte�, ale rovnou po��t� Rx,Rz-testov�n� (to se nepou��v�, proto�e se pou��v� nacti_rx p��mo v PO)
		double UDV();//vr�t� u�itnou d�lku voz�ku dle hodnoty rotace
	private:
		Cmy m;
		double P2K();//vr�t� kapacitu z po�tu pozic, �e�� i situaci, kdy je M (mezera) nulov�, tj. K==P
};
//---------------------------------------------------------------------------
#endif
