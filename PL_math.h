//---------------------------------------------------------------------------
#ifndef PL_mathH
#define PL_mathH
#include "my.h"
//---------------------------------------------------------------------------
class TPL_math
{
	public:
		double TT;//takt linky
		double aRD;//aktu�ln� rychlost aktu�ln� �e�en�ho pohonu
		double R;//rozte� palc� aktu�ln� �e�en�ho pohonu
		double Rz;//rozestup aktivn�ch palc� v m
		double Rx;//rozestup aktivn�ch palc� (po�et aktivn�ch palc�)

		bool aRD_locked;//stav z�mku
		bool R_locked;//stav z�mku
		bool Rz_locked;//stav z�mku
		bool Rx_locked;//stav z�mku

		AnsiString T;//text pro testovac� v�pis

    //plnohodnotn� dokumentace k metod�m se nach�z� v aDoc a pDoc
		void input_TT();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny TT
		void input_aRD();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny aRD
		void input_R();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny R
		void input_Rz(bool prepocet_aRD=true);//pokud je parametr prepocet_aRD=false, nebude se znovu p�epo��t�vat aRD (slou�� pro situaci, kdy input_aRD vol� input_Rz)
		void ipnut_Rx();//p�epo�et souvisej�c�ch hodnot vypl�vaj�c�ch ze zm�ny Rx

	private:
		Cmy m;
};
//---------------------------------------------------------------------------
#endif
