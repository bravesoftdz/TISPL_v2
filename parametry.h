//---------------------------------------------------------------------------

#ifndef parametryH
#define parametryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.Mask.hpp>
#include "scGPExtControls.hpp"
#include "PO_math.h"
#include "scHint.hpp"
#include "rHintWindow.hpp"

//---------------------------------------------------------------------------
class TForm_parametry : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit_name;
	TEdit *Edit_shortname;
	TLabel *Label1;
	TLabel *Label2;
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TrHTMLLabel *rHTMLLabel_rezim;
	TrHTMLLabel *rHTMLLabel_kapacita;
	TscGPButton *scGPButton_OK;
	TscGPButton *scGPButton_storno;
	TrHTMLLabel *rHTMLLabel_pohon;
	TrHTMLLabel *rHTMLLabel_odchylka;
	TrHTMLLabel *rHTMLLabel_delka_dopravniku;
	TrHTMLLabel *rHTMLLabel_name;
	TrHTMLLabel *rHTMLLabel_shortname;
	TrHTMLLabel *rHTMLLabel_cekani;
	TrHTMLLabel *rHTMLLabel_stopka;
	TscGPButton *scGPButton_min_sec;
	TrHTMLLabel *rHTMLLabel_CT;
	TrHTMLLabel *rHTMLLabel_RD;
	TscGPNumericEdit *scGPNumericEdit_CT;
	TscGPComboBox *scComboBox_rezim;
	TscGPComboBox *scComboBox_pohon;
	TscGPEdit *scGPEdit_name;
	TscGPComboBox *scComboBox_cekani_palec;
	TscGPComboBox *scComboBox_stopka;
	TscGPNumericEdit *scGPNumericEdit_delka_dopravniku;
	TscGPEdit *scGPEdit_shortname;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TscGPNumericEdit *scGPNumericEdit_odchylka;
	TscGPNumericEdit *scGPNumericEdit_kapacita;
	TrHTMLLabel *rHTMLLabel_rotace;
	TscGPComboBox *scComboBox_rotace;
	TrHTMLLabel *rHTMLLabel_mezera_jednotky;
	TscGPButton *scGPButton_metry_milimetry;
	TscGPGlyphButton *scGPGlyphButton_copy;
	TscGPGlyphButton *scGPGlyphButton_paste;
	TscButton *scButton_zamek_CT;
	TscButton *scButton_zamek_DD;
	TscButton *scButton_zamek_RD;
	TscGPNumericEdit *scGPNumericEdit_mezera;
	TMemo *Memo1;
	TscGPButton *scGPButton_header_projekt;
	TscGPNumericEdit *scGPNumericEdit_RD;
	TscGPGlyphButton *scGPGlyphButton_PO_text_memo;
	TrHTMLLabel *rHTMLLabel_rozestup;
	TscGPNumericEdit *scGPNumericEdit_rozestup;
	TrHTMLLabel *rHTMLLabel_pozice;
	TscGPNumericEdit *scGPNumericEdit_pozice;
	TscButton *scButton_K_zamek;
	TscGPNumericEdit *scGPNumericEdit_rx;
	TrHTMLLabel *rHTMLLabel_palec_vzd;
	TrHTMLLabel *rHTMLLabel_jednotky_vzdalenostpalcu;
	TrHTMLHint *rHTMLHint1;
	TscGPGlyphButton *scGPGlyphButton_view;
	TButton *Button1;
	TscGPCheckBox *scGPCheckBox_zaokrouhlit;
	TscGPNumericEdit *scGPNumericEdit_mezera_JIG;
	TscGPNumericEdit *scGPNumericEdit_mezera_PODVOZEK;
	TrHTMLLabel *rHTMLLabel_mezera;
	TrHTMLLabel *rHTMLLabel_jig_podvozek;
	TrHTMLLabel *rHTMLLabel_kriticka;
  TscGPGlyphButton *scGPGlyphButton_refresh;
	TscGPGlyphButton *scGPGlyphButton_PO_text_memo2;
  TscGPGlyphButton *scGPGlyphButton_odchylka;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RadioButton_na_delkuClick(TObject *Sender);
	void __fastcall RadioButton_na_sirkuClick(TObject *Sender);
	void __fastcall Image_vozikClick(TObject *Sender);
	void __fastcall Button_min_secClick(TObject *Sender);
	void __fastcall scComboBox_rezimChange(TObject *Sender);
	void __fastcall scGPEdit_nameChange(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scGPNumericEdit_CTChange(TObject *Sender);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPNumericEdit_delka_dopravnikuChange(TObject *Sender);
	void __fastcall scGPNumericEdit_kapacitaChange(TObject *Sender);
	void __fastcall scGPNumericEdit_RD_Change(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button_metry_milimetryClick(TObject *Sender);
	void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
	void __fastcall scGPGlyphButton_pasteClick(TObject *Sender);
	void __fastcall rHTMLLabel_CTClick(TObject *Sender);
	void __fastcall rHTMLLabel_delka_dopravnikuClick(TObject *Sender);
	void __fastcall rHTMLLabel_mezera_jednotkyClick(TObject *Sender);
	void __fastcall rHTMLLabel_RDClick(TObject *Sender);
	void __fastcall scButton_zamek_CTClick(TObject *Sender);
	void __fastcall scButton_zamek_DDClick(TObject *Sender);
	void __fastcall rHTMLLabel_InfoTextClick(TObject *Sender);
	void __fastcall scComboBox_pohonChange(TObject *Sender);
	void __fastcall scGPNumericEdit_kapacitaClick(TObject *Sender);
	void __fastcall scGPNumericEdit_CTClick(TObject *Sender);
	void __fastcall scGPNumericEdit_RD_Click(TObject *Sender);
	void __fastcall scGPNumericEdit_delka_dopravnikuClick(TObject *Sender);
	void __fastcall scGPNumericEdit_mezeraClick(TObject *Sender);
	void __fastcall scGPNumericEdit_mezeraChange(TObject *Sender);
	void __fastcall Button_dopravnik_parametryClick(TObject *Sender);
	void __fastcall scButton_zamek_RDClick(TObject *Sender);
	void __fastcall scGPGlyphButton_PO_text_memoClick(TObject *Sender);
	void __fastcall scGPNumericEdit_poziceChange(TObject *Sender);
	void __fastcall scGPNumericEdit_poziceClick(TObject *Sender);
	void __fastcall scComboBox_rotaceChange(TObject *Sender);
	void __fastcall scButton_K_zamekClick(TObject *Sender);
	void __fastcall scComboBox_rotaceEnter(TObject *Sender);
	void __fastcall scGPNumericEdit_rxChange(TObject *Sender);
	void __fastcall scGPNumericEdit_rxClick(TObject *Sender);
	void __fastcall scGPCheckBox_zaokrouhlitClick(TObject *Sender);
	void __fastcall scGPGlyphButton_viewClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall scComboBox_rotaceClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPNumericEdit_mezera_JIGChange(TObject *Sender);
	void __fastcall scGPNumericEdit_mezera_PODVOZEKChange(TObject *Sender);
	void __fastcall scGPNumericEdit_rozestupChange(TObject *Sender);
	void __fastcall scGPNumericEdit_rozestupClick(TObject *Sender);
  void __fastcall scGPGlyphButton_refreshClick(TObject *Sender);
  void __fastcall scGPGlyphButton_odchylkaClick(TObject *Sender);

private:	// User declarations
	enum Tcomponents{POHON,DELKA,CEKANI,ODCHYLKA,KAPACITA,POZICE,STOPKA,TIME,RYCHLOST,ROTACE,MEZERA,MEZERA_JIG,MEZERA_PODVOZEK,ROZESTUP};//název souvisejících komponent
	enum Tcomponents_state{HIGHLIGHT,ENABLED,DISABLED,READONLY,HIDE};//stav komponent
	enum Tinput_state{NO,NOTHING,CT,DD,RD,K,P,mezera,mezera_jig,mezera_podvozek,Rx};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
	enum Tinput_clicked_edit {empty_klik,CT_klik,DD_klik,RD_klik,C_klik,mezera_klik,P_klik,Rotace_klik,Rx_klik,Rz_klik}; //zjisteni na ktery edit bylo kliknuto
	enum Tinput_clicked_icon {empty_klik_ico,CT_klik_ico,DD_klik_ico,RD_klik_ico,C_klik_ico,mezera_klik_ico,P_klik_ico}; //zjisteni na ktery edit bylo kliknuto

	void position();//hlídá a øeší pozici formuláøe,// napozicování celého formuláøe resp. ošetøení aby zùstal dialog na monitoru, pouze pro prvotní zobrazení dle souøadnic kurzoru myši, jinak dle uživatele
	void set(Tcomponents C,Tcomponents_state S,bool move=true);//zajišuje zobrazení a napozicování patøièné konkrétní komponenty a zároveò udržování hodnoty offsetu - to pokud je move==true, jinak jen nastaví komponenty
	void input_CT();//pøepoèet hodnot vyplývajících ze zmìny CT
	void input_DD();//pøepoèet hodnot vyplývajících ze zmìny DD
	void input_RD();//pøepoèet hodnot vyplývajících ze zmìny RD
	void input_K();//pøepoèet hodnot vyplývajících ze zmìny Kapacity
	void input_M(); //pøepoèet hodnot vyplývajících ze zmìny mezery
	void input_P(); //pøepoèet hodnot vyplývajících ze zmìny poètu pozic
	void null_input_value();//vynuluje vstupní hodnoty
	double Kontrola_mezery();
	void Nastav_zamky(double rezim,Tinput_clicked_icon I,Tinput_clicked_edit E,bool ikonka=true);
	void Pohon_pouzivan(); //kontrola zdali je vybrany pohon pouzivan - dle toho nastav viditelne polozky
	void Check_rozmezi_RD(); //kontrola  RD vùèi vybranému pohonu
	void Nastav_M_R_Rx();
	void Nacti_rx();
	void INPUT();
	void OUTPUT();
	void packa_RDzamek(TCanvas *canv);//vykreslí packu od zamèeného zámku RD k souvisejícím hodnotám
	void packa(Tcomponents start,Tcomponents end);//vykreslí packu mezi edity a comby
	double RDunitD_funkce(double RD);//podpùrná funkce na pøepoèet jednotek RD
	double getM();//vrátí hodnotu mezery z editboxu dle nastavených jednotek mezery mezera pøevedenou do SI jednotek + ošetøuje divné chování okolo nuly
	double getM(double M);//vrátí hodnotu parametru dle nastavených jednotek mezery pøevedenou do SI jednotek + ošetøuje divné chování okolo nuly
	void frameCorrelation(bool default_value=false);//stejnou barvou orámuje hodnoty v korelaci, pokud je default_value na true, nastaví všechny komponenty do výchozího stavu
	void cCT(TColor Color){scGPNumericEdit_CT->Options->FrameNormalColor=Color;}//inline metoda
	void cRD(TColor Color){scGPNumericEdit_RD->Options->FrameNormalColor=Color;}//inline metoda
	void cDD(TColor Color){scGPNumericEdit_delka_dopravniku->Options->FrameNormalColor=Color;}//inline metoda
	void cK(TColor Color){scGPNumericEdit_kapacita->Options->FrameNormalColor=Color;}//inline metoda
	void cP(TColor Color){scGPNumericEdit_pozice->Options->FrameNormalColor=Color;}//inline metoda
	void cRo(TColor Color){scComboBox_rotace->Options->FrameNormalColor=Color;}//inline metoda
	void cM(TColor Color){scGPNumericEdit_mezera->Options->FrameNormalColor=Color;}//inline metoda
	void cMJ(TColor Color){scGPNumericEdit_mezera_JIG->Options->FrameNormalColor=Color;}//inline metoda
	void cMP(TColor Color){scGPNumericEdit_mezera_PODVOZEK->Options->FrameNormalColor=Color;}//inline metoda
	void cRx(TColor Color){scGPNumericEdit_rx->Options->FrameNormalColor=Color;}//inline metoda
	TPoint getRMComponent(Tcomponents C);//vrátí souøadnice pravého okraje a horního okraje+poloviny výšky komponenty
	void frameKritickaMezer(TCanvas *C);//obkreslí obrysem kritickou mezeru
	bool JKM();//vratí true, pokude je mezera_JIG kritická mezera, false pokud je mezera_PODVOZEK kritická mezera
	void refresh_data();

	TPO_math pm;//INSTANCE NA VÝPOÈETNÍ ÈÁST PO tj. PO_math

	short offset;
	short defaultForm_parametryHeight;
	short ComboRotace_predchozi_stav;//uloží pøedchozí stav pro další použití

	TColor hl_color;//(TColor)RGB(255,141,28);//barva zvýraznìní rámeèku komponenty napø.pro povinné položky
	short hlFrameWidth;//šíøka zvýraznìní rámeèku komponenty napø.pro povinné položky

public:		// User declarations
	__fastcall TForm_parametry(TComponent* Owner);
	enum Tminsec{S=0,MIN};Tminsec minsec;Tminsec CTunit;Tminsec RDunitT;//pøepínaè jednotek èasu
	enum Tm_mm{M=0,MM};Tm_mm m_mm;Tm_mm DDunit;Tm_mm DMunit;//pøepínaè jednotek vzdálenost
	enum Tzamek {LOCKED,UNLOCKED};Tzamek CT_zamek;Tzamek RD_zamek;Tzamek DD_zamek;Tzamek K_zamek;

	Tinput_state input_state;//stav vstupu CT,RD,DD,K
	Tinput_clicked_edit input_clicked_edit;//zjisteni na ktery edit bylo kliknuto
	Tinput_clicked_icon input_clicked_icon;//zjisteni na ktery icon bylo kliknuto

	void setForm4Rezim(unsigned short rezim);
	void vypis(UnicodeString text,bool red=true,bool link=false);
	void VALIDACE(Tinput_state input_state=NOTHING);//validace všech hodnot po pøepoètu z PO_math, input_state slouží pro rozlišení volání ze vstupu - pøedevším P a K
	//void vykresli_vozik(bool na_delku=true);

	unsigned int kapacitaSG;//požadavek na rozpad na více stejných S&G objektù
	bool existuje_pohon;
	bool MIMO_ROZMEZI;
	bool pohon_pouzivan;
	bool ComboBoxPohonyProbihaZmena;//používá se kvùli aktualizaci náplnì comba
	short VID;//validation ID
	double VID_value;
	bool input_state_Rz;
	bool form_zobrazen;//detekuje zda je form aktuálnì zobrazen, slouží proto aby pøi zmìnì combo režim pokud si nastavil uživatel formulaø jinam, aby zùstal nastaven dle uživatele
	UnicodeString poznamka;
	double MT;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
#endif
