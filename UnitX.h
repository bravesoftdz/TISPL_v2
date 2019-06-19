//---------------------------------------------------------------------------

#ifndef UnitXH
#define UnitXH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Unit1.h"
//---------------------------------------------------------------------------
class TFormX : public TForm
{
__published:	// IDE-managed Components

private:	// User declarations
	void zmena_aRD (Cvektory::TElement *mimo_element=NULL);//vıpoèty vyvolané zmìnou aRD, parametr mimo_element je zde pouze pøedáván do metody aktualizace_tab_elementu
	void zmena_R ();//vıpoèty vyvolané zmìnou R
	void zmena_Rx ();//vıpoèty vyvolané zmìnou Rx
	void aktualizace_tab_elementu (Cvektory::TElement *mimo_element=NULL);//aktualizuje vıpoèty ve všech tabulkách elemntù, parametr mimo_element je ukazatel na element, kterı má bıt pøeskoèen, defaultnì hodnota, které E->n nidky nedosáhne
	void aktualizace_tab_elementu_pOdebran ();//vynuluje všechny hodnoty závislé na pohonu ve všech tabulkách elementù
	void korelace_tab_pohonu(int Row);//zobrazi korelaci v tabulce pohonu
	void korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element=NULL);//zobrazí korelaci podle tabulky pohonu ve všech elementech,volitelnı parametr, ukazatel na element kterı chci pøeskoèit
	void korelace_v_elementech(long ID,long Row);//jedná se o vnitøní korelace, v jednom elementu (jeden element však mùe ovlivnit i tabulku pohonu)
	Cvektory::TElement *vrat_element_z_tabulky(long ID);//vrátí ukazatel na element, nelze pouít vra element stavající, z dùvodu e ID tabulky ji nemusí bıt totoné s n robota

	//promìnné pro UnitX
	double dopRD;
	bool editace_pohonu;
	long rychlost, roztec_palce, nasobek_roztece, roztec_jigu, mezera_podvozky, mezera_jig1, mezera_jig2;
	//     aRD       R,roztec          Rx          Rz,rozestup
public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,long Col,long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key);
	void odstranit_korelaci(bool predat_focus=true);//odstraní veškerou korelaci, nepovinı prametr urèuje zda má bıt pøedán focus z mGridu, defaultnì focus pøedává
	void validace();//validace zadávané aktuální rychlosti
	void povolit_zakazat_editaci(bool povolit);//zakazuje èi povolí komponenty v tabulce pohonu a všech tabulkách elementu
	void naplneni_dopRD();//doplni doporuèenou rychlost do tabulky pohonu

	//promìnné pro vyøazení OnChange metody pøi vykreslování tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,LO2,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchovává vıbìr input hodnoty (aby se formuláøe necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif

