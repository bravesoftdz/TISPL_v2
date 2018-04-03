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
	TscGPGlyphButton *scGPGlyphButton_InfoIcon;
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
	TrHTMLLabel *rHTMLLabel_mezera;
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
	TscGPNumericEdit *scGPNumericEdit1_rx;
	TrHTMLLabel *rHTMLLabel_palec_vzd;
	TrHTMLLabel *rHTMLLabel_jednotky_vzdalenostpalcu;
	TrHTMLHint *rHTMLHint1;
	TscHint *scHint1;
	TscCheckBox *scCheckBox_zaokrouhlit;
	TscGPGlyphButton *scGPGlyphButton1;
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
	void __fastcall rHTMLLabel_mezeraClick(TObject *Sender);
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
	void __fastcall scComboBox_rotaceClick(TObject *Sender);
	void __fastcall scGPNumericEdit1_rxChange(TObject *Sender);
	void __fastcall scGPNumericEdit1_rxClick(TObject *Sender);
	void __fastcall scCheckBox_zaokrouhlitClick(TObject *Sender);
	void __fastcall scGPGlyphButton1Click(TObject *Sender);

private:	// User declarations
	enum Tcomponents{POHON,DELKA,CEKANI,ODCHYLKA,KAPACITA,POZICE,STOPKA,TIME,RYCHLOST,ROTACE,MEZERA,ROZESTUP};//n�zev souvisej�c�ch komponent
	enum Tcomponents_state{HIGHLIGHT,ENABLED,DISABLED,READONLY,HIDE};//stav komponent
	enum Tinput_state{NO,NOTHING,CT,DD,RD,K,mezera,P,Rx};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
	enum Tinput_clicked_edit {empty_klik,CT_klik,DD_klik,RD_klik,C_klik,mezera_klik,P_klik,Rotace_klik,Rx_klik}; //zjisteni na ktery edit bylo kliknuto
	enum Tinput_clicked_icon {empty_klik_ico,CT_klik_ico,DD_klik_ico,RD_klik_ico,C_klik_ico,mezera_klik_ico,P_klik_ico}; //zjisteni na ktery edit bylo kliknuto
	enum Tzamek {LOCKED,UNLOCKED};Tzamek CT_zamek;Tzamek RD_zamek;Tzamek DD_zamek;Tzamek K_zamek;

	void set(Tcomponents C,Tcomponents_state S,bool move=true);//zaji��uje zobrazen� a napozicov�n� pat�i�n� konkr�tn� komponenty a z�rove� udr�ov�n� hodnoty offsetu - to pokud je move==true, jinak jen nastav� komponenty
	void input_CT();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny CT
	void input_DD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny DD
	void input_RD();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny RD
	void input_K();//p�epo�et hodnot vypl�vaj�c�ch ze zm�ny Kapacity
	void input_M(); //p�epo�et hodnot vypl�vaj�c�ch ze zm�ny mezery
	void input_P(); //p�epo�et hodnot vypl�vaj�c�ch ze zm�ny po�tu pozic
	void null_input_value();//vynuluje vstupn� hodnoty
	void INPUT();
	void OUTPUT();
	double Kontrola_mezery();
	void Nastav_zamky(double rezim,Tinput_clicked_icon I,Tinput_clicked_edit E,bool ikonka=true);
	void Pohon_pouzivan(); //kontrola zdali je vybrany pohon pouzivan - dle toho nastav viditelne polozky
	void Check_rozmezi_RD(); //kontrola  RD v��i vybran�mu pohonu
	void Nacti_rx();
	void VALIDACE(Tinput_state input_state=NOTHING);//validace v�ech hodnot po p�epo�tu z PO_math, input_state slou�� pro rozli�en� vol�n� ze vstupu - p�edev��m P a K
	TPO_math pm;//INSTANCE NA V�PO�ETN� ��ST PO tj. PO_math

	double RDunitD_funkce(double RD);//podp�rn� funkce na p�epo�et jednotek d�lky
	short offset;
	short defaultForm_parametryHeight;
	short VID;//validation ID
	TColor hl_color;//(TColor)RGB(255,141,28);//barva zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky
	short hlFrameWidth;//���ka zv�razn�n� r�me�ku komponenty nap�.pro povinn� polo�ky

public:		// User declarations
	__fastcall TForm_parametry(TComponent* Owner);
	enum Tminsec{S=0,MIN};Tminsec minsec;Tminsec CTunit;Tminsec RDunitT;//p�ep�na� jednotek �asu
	enum Tm_mm{M=0,MM};Tm_mm m_mm;Tm_mm DDunit;Tm_mm DMunit;Tm_mm RDunitD;//p�ep�na� jednotek vzd�lenost
	void setForm4Rezim(unsigned short rezim);
	void vypis(UnicodeString text,bool RED=true);
	Tinput_state input_state;//stav vstupu CT,RD,DD,K
	Tinput_clicked_edit input_clicked_edit;//zjisteni na ktery edit bylo kliknuto
  Tinput_clicked_icon input_clicked_icon;//zjisteni na ktery icon bylo kliknuto
	short kapacitaSG;//po�adavek na rozpad na v�ce stejn�ch S&G objekt�
	bool existuje_pohon;
	bool form_zobrazen;//detekuje zda je form aktu�ln� zobrazen, slou�� proto aby p�i zm�n� combo re�im pokud si nastavil u�ivatel formula� jinam, aby z�stal nastaven dle u�ivatele

	void vykresli_vozik(bool na_delku=true);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
#endif
