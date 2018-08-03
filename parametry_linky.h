//---------------------------------------------------------------------------
#ifndef parametry_linkyH
#define parametry_linkyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
#include "rStringGridEd.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include "scExtControls.hpp"
#include "scHtmlControls.hpp"
#include "pl_priority.h"
#include "scModernControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
#include "PL_math.h"
#include "rHintWindow.hpp"
//---------------------------------------------------------------------------
class TForm_parametry_linky : public TForm
{
__published:	// IDE-managed Components
	TrStringGridEd *rStringGridEd_tab_dopravniky;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_vozik;
	TscGPButton *scGPButton_pohon;
	TButton *Vypis_pohony;
	TrHTMLLabel *rHTMLLabel_eDesigner;
	TrHTMLLabel *rHTMLLabel_takt;
	TrHTMLLabel *rHTMLLabel_sirka_jig;
	TscGPButton *scGPButton_obecne;
	TrHTMLLabel *rHTMLLabel_doporuc_pohony;
	TscExPanel *scExPanel_doporuc_pohony;
	TscHTMLLabel *scHTMLLabel_doporuc_pohony;
	TscGPButton *scGPButton_doporucene;
	TscGPGlyphButton *scGPGlyphButton_add_mezi_pohony;
	TscLabel *scLabel_TT_change_hint;
	TMemo *Memo1;
	TButton *Button1;
	TscGPButton *scGPButton_jig;
	TrHTMLLabel *rHTMLLabel_delka_jig;
	TscGPSwitch *scGPSwitch;
	TrHTMLLabel *rHTMLLabel3;
	TrHTMLLabel *rHTMLLabel4;
	TrHTMLLabel *rHTMLLabel_JIG;
	TscGPGlyphButton *Button_ADD;
	TscGPGlyphButton *Button_DEL;
	TrStringGridEd *rStringGridEd_hlavicka_tabulky;
	TscGPNumericEdit *rEditNum_takt;
	TscGPNumericEdit *scGPNumericEdit_delka_jig;
	TscGPNumericEdit *scGPNumericEdit_sirka_jig;
	TscGPPanel *PopUPmenu;
	TscGPPanel *Item_zobrazit_parametry;
	TscLabel *scLabel_zobrazit_parametry;
	TscGPGlyphButton *GlyphButton_zobrazit_parametry;
	TscGPPanel *Item_smazat;
	TscGPGlyphButton *GlyphButton_smazat;
	TscLabel *scLabel_smazat;
	TscGPPanel *Item_smazat_nepouzite;
	TscLabel *scLabel_smazat_nepouzite;
	TscGPGlyphButton *GlyphButton_smazat_nepouzite;
	TscGPPanel *Item_kopirovat;
	TscLabel *scLabel_kopirovat;
	TscGPGlyphButton *GlyphButton_kopirovat;
	TscGPPanel *Item_nastavit_parametry;
	TscLabel *scLabel_nastavit_parametry;
	TscGPGlyphButton *GlyphButton_nastavit_parametry;
	TscGPGlyphButton *GlyphButton_close;
	TscGPGlyphButton *scGPGlyphButton_ADD;
	TscGPGlyphButton *scGPGlyphButton_DEL_nepouzite;
	TrHTMLLabel *rHTMLLabel_vyska_jig;
	TrHTMLLabel *rHTMLLabel_podvozek_zaves;
	TrHTMLLabel *rHTMLLabel_delka_podvozek;
	TscGPGlyphButton *scGPGlyphButton_takttime_kalkulator;
	TscGPNumericEdit *scGPNumericEdit_delka_podvozek;
	TscGPNumericEdit *scGPNumericEdit_vyska_jig;
	TrMemoEx *rMemoEx_ID;
	TrMemoEx *rMemoEx_Nazev;
	TrMemoEx *rMemoEx1_rychlost;
	TrMemoEx *rMemoEx1_roztec;
	TrMemoEx *rMemoEx1_rozestup;
	TrMemoEx *rMemoEx2_prirazen;
	TMemo *Memo2;
	TMemo *Memo3;
	TrMemoEx *rMemoEx1_rozestup_akt_unas;
	TscGPButton *scGPButton_zamek_aRD;
	TscGPButton *scGPButton_zamek_roztec;
	TscGPButton *scGPButton_zamek_Rz;
	TscGPButton *scGPButton_zamek_Rx;
	TrHTMLHint *rHTMLHint1;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TMemo *Memo4;
	TscGPGlyphButton *scGPGlyphButton_TT;
	TscGPGlyphButton *scGPGlyphButton_vozik_edit;
	TButton *Button2;
	TButton *Button3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall Button_saveClick(TObject *Sender);
	void __fastcall Button_ADD_Click(TObject *Sender);
	void __fastcall Button_DEL_Click(TObject *Sender);
	void __fastcall Vypis_pohonyClick(TObject *Sender);
	void __fastcall scGPButton_doporuceneClick(TObject *Sender);
	void __fastcall scExPanel_doporuc_pohonyClose(TObject *Sender);
	void __fastcall rEditNum_takt_Change(TObject *Sender);
	void __fastcall rHTMLLabel_delkavozikuClick(TObject *Sender);
	void __fastcall rHTMLLabel_sirka_jigClick(TObject *Sender);
	void __fastcall rHTMLLabel_taktClick(TObject *Sender);
	void __fastcall scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikyGetEditStyle(TObject *Sender, int Col,
          int Row, TrStringGridEdEditStyle &EditStyle);
	void __fastcall rStringGridEd_tab_dopravnikyCanEdit(TObject *Sender, int Col, int Row,
          bool &CanEdit);
	void __fastcall rEditNum_delkavozikuClick(TObject *Sender);
	void __fastcall rHTMLLabel_delka_jigClick(TObject *Sender);
	void __fastcall rHTMLLabel_delka_vozikuClick(TObject *Sender);
	void __fastcall Button_DELMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall rEditNum_delka_jigu_oldKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_sirka_jigu_OLDKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_takt_OLDKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rStringGridEd_tab_dopravnikyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall rStringGridEd_tab_dopravnikyEnter(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPGlyphButton_infoClick(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikyMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall GlyphButton_closeClick(TObject *Sender);
	void __fastcall scLabel_kopirovatClick(TObject *Sender);
	void __fastcall scLabel_smazatClick(TObject *Sender);
	void __fastcall scLabel_smazat_nepouziteClick(TObject *Sender);
	void __fastcall scLabel_smazat_nepouziteMouseEnter(TObject *Sender);
	void __fastcall scLabel_smazat_nepouziteMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_smazat_nepouziteMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_smazat_nepouziteMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_smazatMouseLeave(TObject *Sender);
	void __fastcall scLabel_smazatMouseLeave(TObject *Sender);
	void __fastcall scLabel_smazatMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_smazatMouseEnter(TObject *Sender);
	void __fastcall scLabel_kopirovatMouseEnter(TObject *Sender);
	void __fastcall scLabel_kopirovatMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_kopirovatMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_kopirovatMouseLeave(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikyGetCellParams(TObject *Sender, int Col,
          int Row, TFont *AFont, TColor &Background, bool &Highlight);
	void __fastcall scHTMLLabel_doporuc_pohonyClick(TObject *Sender);
	void __fastcall scGPSwitchChangeState(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall rEditNum_taktClick(TObject *Sender);
	void __fastcall scGPButton_zamek_aRDClick(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikyCellClick(TObject *Sender, int ACol,
          int ARow);
	void __fastcall scGPButton_zamek_RxClick(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikySetEditText(TObject *Sender, int ACol,
          int ARow, const UnicodeString Value);
	void __fastcall rStringGridEd_tab_dopravnikyGetEditText(TObject *Sender, int ACol,
          int ARow, UnicodeString &Value);
	void __fastcall scGPButton_zamek_roztecClick(TObject *Sender);
	void __fastcall scGPButton_zamek_RzClick(TObject *Sender);
	void __fastcall FormClick(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikyDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall rStringGridEd_tab_dopravnikyPicklistDropdown(TObject *Sender, int Col,
          int Row, TStringList *&PickList);
	void __fastcall rMemoEx1_roztecClick(TObject *Sender);
	void __fastcall scGPGlyphButton_TTClick(TObject *Sender);
	void __fastcall scGPGlyphButton_vozik_editClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall rHTMLLabel_InfoTextClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);


private:	// User declarations
	TPL_math pm;//INSTANCE NA VÝPOÈETNÍ ÈÁST PL tj. PL_math
	enum Tinput_state{NO,NOTHING,DV,SV,TT,RZ,RX,aRD,R,jednotky_prevod};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
	enum Tinput_clicked_edit {empty_klik,TT_klik,DV_klik,SV_klik,V_klik,Podvoz_klik,aRD_klik,R_klik,Rz_klik,Rx_klik,nazev_klik,od_klik,do_klik}; //zjisteni na ktery edit nebo bunku ve sloupci bylo kliknuto
	enum Tinput_clicked_icon {empty_klik_ico,aRD_klik_ico,R_klik_ico,Rz_klik_ico,Rx_klik_ico}; //zjisteni na kterou ikonku zámku bylo kliknuto
	enum Tinput_onchange {NOChange,aRDChange,RChange,RzChange,RxChange}; //zjisteni na kterou ikonku zámku bylo kliknuto

	void pasiveColor();//nastaví všechny položky pop-up na pasivní resp. default barvu
	void top_positon(int top);//hlídání horní pozice, je-li daná komponenta horní kvùli nastavení køížku
	TColor clBg,clAcBg,clAcBg2,clGlyph,clAcGlyph;//barvy položek pop-up menu
	unsigned int getPID(int ROW);//vrátí pohon ID na daném øádku
	unsigned int getMaxPID();//najde max použité ID pohonu (protože ID nejsou seøazena,nelze vzít index posledního øádku)
	bool *zrusena_prirazeni_PID;
	unsigned int zrusena_prirazeni_PID_size;
	void zrusit_prirazeni_smazanych_ci_odrazenych_pohunu_k_objektum();
	void pozice_scGPGlyphButton_hint();
	bool existuji_nepouzivane_pohony();//testuje zda existují nepoužíté pohony, pokud ano,vrací true jinak false
	void nacti_pohony();
	void Nastav_zamky(Tinput_clicked_icon I,Tinput_clicked_edit E);

public:		// User declarations
	__fastcall TForm_parametry_linky(TComponent* Owner);
	int getROW(int PID);//vrátí èíslo øádku dle pohon ID, pokud nenajde vrátí -1
	void vypis(UnicodeString text,bool red=true,bool link=false);
	void input_TT();
	void INPUT(double Sloupec, double Radek);
	void OUTPUT(double i,double Sloupec, double Radek);
	void Roletka_roztec(double Row);
	double getRz(double i);
	void show_min_Rz();
	double getTT();
	void VALIDACE(int ACol, int ARow);

	short VID;//validation ID
	double VID_value;
	short Row_validace;
	UnicodeString NAZEV;
	double OD;
	double DO;
	bool data_nalezena;
	Tinput_state input_state;//stav vstupu DV,SV,TT...atd
	Tinput_clicked_edit input_clicked_edit;//zjisteni na ktery edit bylo kliknuto
	Tinput_clicked_icon input_clicked_icon;//zjisteni na ktery icon bylo kliknuto
	Tinput_onchange   onchange;
	enum Tm_mm{M=0,MM};Tm_mm Delkaunit;Tm_mm Sirkaunit;Tm_mm Runit;Tm_mm Rzunit;//pøepínaè jednotek vzdálenost
	enum Tminsec{S=0,MIN};Tminsec Taktunit;//pøepínaè jednotek èasu
	bool Changes;  //obecna zmena = zmena PP ci TT
	bool Changes_TT;    // konkretni zmena TT
	bool Changes_PP;   // konkretni zmena PP
	bool Changes_roztec;   // konkretni zmena PP
	bool Changes_prirazen;
	bool Changes_aRD;
	bool Changes_Rz;
	bool Changes_Rx;
	bool Ulozit;
	bool Storno;
	bool zobrazitFrameForm;
	bool zobrazOramovani;

	int roletka_data;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
#endif
