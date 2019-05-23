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
	void zmena_aRD ();//výpoèty vyvolané zmìnou aRD
	void zmena_R ();//výpoèty vyvolané zmìnou R
	void zmena_Rx ();//výpoèty vyvolané zmìnou Rx
	void aktualizace_tab_elementu (/*int mimo_el_n=9999999999*/);//aktualizuje výpoèty ve všech tabulkách elemntù, parametr mimo_el_n uchovává n elementu který má být pøeskoèen, defaultnì hodnota, které E->n nidky nedosáhne
	void aktualizace_tab_elementu_pOdebran ();//vynuluje všechny hodnoty závislé na pohonu ve všech tabulkách elementù
	void korelace_tab_pohonu(int Row);//zobrazi korelaci v tabulce pohonu
	void korelace_tab_pohonu_elementy();//zobrazí korelaci podle tabulky pohonu ve všech elementech
	void korelace_v_elementech(long ID,long Row);//jedná se o vnitøní korelace, v jednom elementu (jeden element však mùže ovlivnit i tabulku pohonu)
	void zakaz_vseho(bool povolit);//zakazuje èi povolí komponenty v tabulce pohonu a všech tabulkách elementu

	//promìnné pro UnitX
	double dopRD;
	bool editace_pohonu;

public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,long Col,long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key);
	void odstranit_korelaci(bool predat_focus=true);//odstraní veškerou korelaci, nepoviný prametr urèuje zda má být pøedán focus z mGridu, defaultnì focus pøedává
	void validace();//validace zadávané aktuální rychlosti
	void naplneni_dopRD();//doplni doporuèenou rychlost do tabulky pohonu

	//promìnné pro vyøazení OnChange metody pøi vykreslování tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif

