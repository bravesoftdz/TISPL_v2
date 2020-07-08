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
	void aktualizace_tab_elementu_pOdebran ();//vynuluje v�echny hodnoty z�visl� na pohonu ve v�ech tabulk�ch element�
	void aktualizace_tab_pohonu();
	void korelace_tab_pohonu(int Row);//zobrazi korelaci v tabulce pohonu
	void korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element=NULL);//zobraz� korelaci podle tabulky pohonu ve v�ech elementech,voliteln� parametr, ukazatel na element kter� chci p�esko�it
	void korelace_v_elementech(long ID,long Col,long Row);//jedn� se o vnit�n� korelace, v jednom elementu (jeden element v�ak m��e ovlivnit i tabulku pohonu)
	Cvektory::TElement *vrat_element_z_tabulky(long ID);//vr�t� ukazatel na element, nelze pou��t vra� element stavaj�c�, z d�vodu �e ID tabulky ji� nemus� b�t toto�n� s n robota
	void prirazeni_pohohonu_vetvi(Cvektory::TElement *E,long Col);//zm�n� pohon sekund�rn� v�tvi, z v�hybky nebo spojky (pokud sekund�rn� v�tev existuje)
	void prirazeni_pohohonu_PM(Cvektory::TElement *E,long Col);//p�i�azen� pohonu p�ed PM, nebo za PM
	void prirazeni_pohonu_defTab();//p�i�azen� pohonu z PmG
	void zapisVID(int zapis,int pozice);//zapi�e na danou pozici ve VID dan� ��slo
	void zobrazit_skryt_radkyPM(Cvektory::TElement *E);//zobraz� �i skryje ��dky PM tabulek podle stavu ulo�en�ho v objektu
  void aktualizace_teplomeru();//provede aktualizaci �asu v tabulce teplom�r�

	//prom�nn� pro UnitX
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
	void OnKeyDown(long Tag,unsigned long Col,unsigned long Row,WORD &Key,TShiftState Shift);//slou�� pro spu�t�n� funkcionality ctrl+z a ctrl+y pokud je focus na mGridu
	void odstranit_korelaci(bool predat_focus=true);//odstran� ve�kerou korelaci, nepovin� prametr ur�uje zda m� b�t p�ed�n focus z mGridu, defaultn� focus p�ed�v�
	void validace_aRD(bool pouze_rozmezi=false);//validace zad�van� aktu�ln� rychlosti
	void validace_max_voziku();//validace maxim�ln�ho po�tu voz�k� na stopce
	void povolit_zakazat_editaci();//zakazuje �i povol� ukl�d�n� editace na z�klad� VID
	void naplneni_dopRD();//doplni doporu�enou rychlost do tabulky pohonu
  bool check_click_Note(double X,double Y,bool check_for_highlight=false);
	void aktualizace_zon_otaceni(Cvektory::TElement *E);//po zm�n� rotace na elementu E, projde v�echny elementy za a p�epo��t� jim z�ny ot��en�
	void validace_RD(Cvektory::TElement *E);//provede validaci RD
	void update_hodnot_vyhybky_PM(Cvektory::TElement *E);//zobraz� aktu�ln� hodnoty z dat v tabulk�ch
	void vynulujVID();//vynuluje VID, podle d�lky nastaven� v konstruktoru (viz. popisek deklarace VID) nap�. 2 ��slice
	void aktualizace_tab_elementu (Cvektory::TElement *mimo_element=NULL);//aktualizuje v�po�ty ve v�ech tabulk�ch elemnt�, parametr mimo_element je ukazatel na element, kter� m� b�t p�esko�en, defaultn� hodnota, kter� E->n nidky nedos�hne
  void mazatPM(Cvektory::TElement *Element=NULL);//kontorla a dotaz zda maj� b�t PM na stejn�m pohonu smaz�na

	//prom�nn� pro vy�azen� OnChange metody p�i vykreslov�n� tabulek
  enum Tinput_state{NO,NOTHING,PT,PT2,LO,LO2,WT,DO,PTotoc,COMBO,aRD,R,Rz,Rx,P_VOZ};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
	Tinput_state input_state;//stav vstupu PT,LO,DelkaOtoce...
	bool vstoupeno_poh, vstoupeno_elm,validace_true;
	Cvektory::TElement *posledni_E;//slou�� pro ulo�en� editovan�ho elementu, nemus�m p�i ka�d�m stisku kl�vesy vracet E
	Cvektory::TCesta *posledni_c;
	int validovany_pohon;
	AnsiString VID;//dvoucifern� ��slo, prvn� ��slo zn�zor�uje validaci pohonu, druh� validaci stopek, pokud je ��slo 00 = bez chyb, 10 = chyba na pohonu, stop v po��dku (viz. 777 webov� soubory), po�et ��slic nastaven v konstruktoru, m�nit pouze tam!!
};
//---------------------------------------------------------------------------
extern PACKAGE TFormX *FormX;
//---------------------------------------------------------------------------
#endif
