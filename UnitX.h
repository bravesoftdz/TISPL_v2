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
	void zmena_aRD (Cvektory::TElement *mimo_element=NULL);//v�po�ty vyvolan� zm�nou aRD, parametr mimo_element je zde pouze p�ed�v�n do metody aktualizace_tab_elementu
	void zmena_R ();//v�po�ty vyvolan� zm�nou R
	void zmena_Rx ();//v�po�ty vyvolan� zm�nou Rx
	void aktualizace_tab_elementu (Cvektory::TElement *mimo_element=NULL);//aktualizuje v�po�ty ve v�ech tabulk�ch elemnt�, parametr mimo_element je ukazatel na element, kter� m� b�t p�esko�en, defaultn� hodnota, kter� E->n nidky nedos�hne
	void aktualizace_tab_elementu_pOdebran ();//vynuluje v�echny hodnoty z�visl� na pohonu ve v�ech tabulk�ch element�
	void korelace_tab_pohonu(int Row);//zobrazi korelaci v tabulce pohonu
	void korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element=NULL);//zobraz� korelaci podle tabulky pohonu ve v�ech elementech,voliteln� parametr, ukazatel na element kter� chci p�esko�it
	void korelace_v_elementech(long ID,long Row);//jedn� se o vnit�n� korelace, v jednom elementu (jeden element v�ak m��e ovlivnit i tabulku pohonu)
	Cvektory::TElement *vrat_element_z_tabulky(long ID);//vr�t� ukazatel na element, nelze pou��t vra� element stavaj�c�, z d�vodu �e ID tabulky ji� nemus� b�t toto�n� s n robota

	//prom�nn� pro UnitX
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
	void odstranit_korelaci(bool predat_focus=true);//odstran� ve�kerou korelaci, nepovin� prametr ur�uje zda m� b�t p�ed�n focus z mGridu, defaultn� focus p�ed�v�
	void validace();//validace zad�van� aktu�ln� rychlosti
	void povolit_zakazat_editaci(bool povolit);//zakazuje �i povol� komponenty v tabulce pohonu a v�ech tabulk�ch elementu
	void naplneni_dopRD();//doplni doporu�enou rychlost do tabulky pohonu

	//prom�nn� pro vy�azen� OnChange metody p�i vykreslov�n� tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,LO2,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif

