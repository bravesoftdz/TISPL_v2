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
	void korelace_v_elementech(long ID,long Col,long Row);//jedná se o vnitøní korelace, v jednom elementu (jeden element však mùe ovlivnit i tabulku pohonu)
	Cvektory::TElement *vrat_element_z_tabulky(long ID);//vrátí ukazatel na element, nelze pouít vra element stavající, z dùvodu e ID tabulky ji nemusí bıt totoné s n robota
	void prirazeni_pohohonu_vetvi(Cvektory::TElement *E,bool hlavni=true);//zmìní pohon sekundární vìtvi, z vıhybky nebo spojky (pokud sekundární vìtev existuje)
  void update_hodnot_vyhybky_PM(Cvektory::TElement *E);//zobrazí aktuální hodnoty z dat v tabulkách
  void prirazeni_pohohonu_PM(Cvektory::TElement *E,long Col);//pøiøazení pohonu pøed PM, nebo za PM

	//promìnné pro UnitX
	bool editace_pohonu;
	long rychlost, roztec_palce, nasobek_roztece, roztec_jigu, mezera_podvozky, mezera_jig1, mezera_jig2;
	//     aRD       R,roztec          Rx          Rz,rozestup
public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,long Col,long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key);
	void OnKeyDown(long Tag,unsigned long Col,unsigned long Row,WORD &Key,TShiftState Shift);//slouí pro spuštìní funkcionality ctrl+z a ctrl+y pokud je focus na mGridu
	void odstranit_korelaci(bool predat_focus=true);//odstraní veškerou korelaci, nepovinı prametr urèuje zda má bıt pøedán focus z mGridu, defaultnì focus pøedává
	void validace_aRD(bool pouze_rozmezi=false);//validace zadávané aktuální rychlosti
	void validace_max_voziku();//validace maximálního poètu vozíkù na stopce
	void povolit_zakazat_editaci(bool povolit);//zakazuje èi povolí komponenty v tabulce pohonu a všech tabulkách elementu
	void naplneni_dopRD();//doplni doporuèenou rychlost do tabulky pohonu
	bool naplneni_max_voziku(double X,double Y,bool check_for_highlight=false);//doplní doporuèenı poèet maximálních vozíku po kliku
	void aktualizace_PmG(bool Refresh=false);//aktualizuje hodnoty v PmG
	void aktualizace_zon_otaceni(Cvektory::TElement *E);//po zmìnì rotace na elementu E, projde všechny elementy za a pøepoèítá jim zóny otáèení

	//promìnné pro vyøazení OnChange metody pøi vykreslování tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,LO2,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx,P_VOZ};//uchovává vıbìr input hodnoty (aby se formuláøe necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
	double dopRD;
	Cvektory::TElement *posledni_E;//slouí pro uloení editovaného elementu, nemusím pøi kadém stisku klávesy vracet E
	Cvektory::TCesta *posledni_c;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif
