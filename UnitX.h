//---------------------------------------------------------------------------

#ifndef UnitXH
#define UnitXH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormX : public TForm
{
__published:	// IDE-managed Components

private:	// User declarations
	void zmena_aRD ();//v�po�ty vyvolan� zm�nou aRD
	void zmena_R ();//v�po�ty vyvolan� zm�nou R
	void zmena_Rx ();//v�po�ty vyvolan� zm�nou Rx
	void aktualizace_tab_elementu (/*int mimo_el_n=9999999999*/);//aktualizuje v�po�ty ve v�ech tabulk�ch elemnt�, parametr mimo_el_n uchov�v� n elementu kter� m� b�t p�esko�en, defaultn� hodnota, kter� E->n nidky nedos�hne
	void aktualizace_tab_elementu_pOdebran ();//vynuluje v�echny hodnoty z�visl� na pohonu ve v�ech tabulk�ch element�
	void korelace_tab_pohonu(int Row);//zobrazi korelaci v tabulce pohonu
	void korelace_tab_pohonu_elementy();//zobraz� korelaci podle tabulky pohonu ve v�ech elementech
	void korelace_v_elementech(long ID,long Row);//jedn� se o vnit�n� korelace, v jednom elementu (jeden element v�ak m��e ovlivnit i tabulku pohonu)
	void zakaz_vseho(bool povolit);//zakazuje �i povol� komponenty v tabulce pohonu a v�ech tabulk�ch elementu

	//prom�nn� pro UnitX
	double dopRD;
	bool editace_pohonu;

public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,long Col,long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key);
	void odstranit_korelaci(bool predat_focus=true);//odstran� ve�kerou korelaci, nepovin� prametr ur�uje zda m� b�t p�ed�n focus z mGridu, defaultn� focus p�ed�v�
	void validace();//validace zad�van� aktu�ln� rychlosti
	void naplneni_dopRD();//doplni doporu�enou rychlost do tabulky pohonu

	//prom�nn� pro vy�azen� OnChange metody p�i vykreslov�n� tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif

