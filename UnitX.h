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
	void zmena_aRD (Cvektory::TElement *mimo_element=NULL);//výpoèty vyvolané zmìnou aRD, parametr mimo_element je zde pouze pøedáván do metody aktualizace_tab_elementu
	void zmena_R ();//výpoèty vyvolané zmìnou R
	void zmena_Rx ();//výpoèty vyvolané zmìnou Rx
	void aktualizace_tab_elementu_pOdebran ();//vynuluje všechny hodnoty závislé na pohonu ve všech tabulkách elementù
	void aktualizace_tab_pohonu();
	void korelace_tab_pohonu(int Row);//zobrazi korelaci v tabulce pohonu
	void korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element=NULL);//zobrazí korelaci podle tabulky pohonu ve všech elementech,volitelný parametr, ukazatel na element který chci pøeskoèit
	void korelace_v_elementech(long ID,long Col,long Row);//jedná se o vnitøní korelace, v jednom elementu (jeden element však mùže ovlivnit i tabulku pohonu)
	Cvektory::TElement *vrat_element_z_tabulky(long ID);//vrátí ukazatel na element, nelze použít vra element stavající, z dùvodu že ID tabulky již nemusí být totožné s n robota
	void prirazeni_pohohonu_vetvi(Cvektory::TElement *E,long Col);//zmìní pohon sekundární vìtvi, z výhybky nebo spojky (pokud sekundární vìtev existuje)
	void prirazeni_pohohonu_PM(Cvektory::TElement *E,long Col);//pøiøazení pohonu pøed PM, nebo za PM
	void prirazeni_pohonu_defTab();//pøiøazení pohonu z PmG
	void zapisVID(int zapis,int pozice);//zapiše na danou pozici ve VID dané èíslo
	void zobrazit_skryt_radkyPM(Cvektory::TElement *E);//zobrazí èi skryje øádky PM tabulek podle stavu uloženého v objektu
  void aktualizace_teplomeru();//provede aktualizaci èasu v tabulce teplomìrù

	//promìnné pro UnitX
	bool editace_pohonu;
	long rychlost, roztec_palce, nasobek_roztece, roztec_jigu, mezera_podvozky, mezera_jig1, mezera_jig2;
	double dopRD;
	//     aRD       R,roztec          Rx          Rz,rozestup
public:		// User declarations
  __fastcall TFormX(TComponent* Owner);
	void OnClick(long Tag,long ID,long Col,long Row);
	void OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key);
	void OnKeyDown(long Tag,unsigned long Col,unsigned long Row,WORD &Key,TShiftState Shift);//slouží pro spuštìní funkcionality ctrl+z a ctrl+y pokud je focus na mGridu
	void odstranit_korelaci(bool predat_focus=true);//odstraní veškerou korelaci, nepoviný prametr urèuje zda má být pøedán focus z mGridu, defaultnì focus pøedává
	void validace_aRD(bool pouze_rozmezi=false);//validace zadávané aktuální rychlosti
	void validace_max_voziku();//validace maximálního poètu vozíkù na stopce
	void povolit_zakazat_editaci();//zakazuje èi povolí ukládání editace na základì VID
	void naplneni_dopRD();//doplni doporuèenou rychlost do tabulky pohonu
  bool check_click_Note(double X,double Y,bool check_for_highlight=false);
	void aktualizace_zon_otaceni(Cvektory::TElement *E);//po zmìnì rotace na elementu E, projde všechny elementy za a pøepoèítá jim zóny otáèení
	void validace_RD(Cvektory::TElement *E);//provede validaci RD
	void update_hodnot_vyhybky_PM(Cvektory::TElement *E);//zobrazí aktuální hodnoty z dat v tabulkách
	void vynulujVID();//vynuluje VID, podle délky nastavené v konstruktoru (viz. popisek deklarace VID) napø. 2 èíslice
	void aktualizace_tab_elementu (Cvektory::TElement *mimo_element=NULL);//aktualizuje výpoèty ve všech tabulkách elemntù, parametr mimo_element je ukazatel na element, který má být pøeskoèen, defaultnì hodnota, které E->n nidky nedosáhne
  void mazatPM(Cvektory::TElement *Element=NULL);//kontorla a dotaz zda mají být PM na stejném pohonu smazána

	//promìnné pro vyøazení OnChange metody pøi vykreslování tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,LO2,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx,P_VOZ};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
	Cvektory::TElement *posledni_E;//slouží pro uložení editovaného elementu, nemusím pøi každém stisku klávesy vracet E
	Cvektory::TCesta *posledni_c;
	int validovany_pohon;
	AnsiString VID;//dvouciferné èíslo, první èíslo znázoròuje validaci pohonu, druhé validaci stopek, pokud je èíslo 00 = bez chyb, 10 = chyba na pohonu, stop v poøádku (viz. 777 webové soubory), poèet èíslic nastaven v konstruktoru, mìnit pouze tam!!
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif
