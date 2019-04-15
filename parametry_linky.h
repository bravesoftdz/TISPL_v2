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
#include <sstream>
#include <iomanip>
//---------------------------------------------------------------------------
class TForm_parametry_linky : public TForm
{
__published:	// IDE-managed Components
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
	TscGPButton *scGPButton_jig;
	TrHTMLLabel *rHTMLLabel_delka_jig;
	TscGPSwitch *scGPSwitch;
	TrHTMLLabel *rHTMLLabel3;
	TrHTMLLabel *rHTMLLabel4;
	TrHTMLLabel *rHTMLLabel_JIG;
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
  TscGPGlyphButton *scGPGlyphButton_ADD_old;
	TscGPGlyphButton *scGPGlyphButton_DEL_nepouzite;
	TrHTMLLabel *rHTMLLabel_vyska_jig;
	TrHTMLLabel *rHTMLLabel_podvozek_zaves;
	TrHTMLLabel *rHTMLLabel_delka_podvozek;
	TscGPGlyphButton *scGPGlyphButton_takttime_kalkulator;
	TscGPNumericEdit *scGPNumericEdit_delka_podvozek;
	TscGPNumericEdit *scGPNumericEdit_vyska_jig;
	TrHTMLHint *rHTMLHint1;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TscGPGlyphButton *scGPGlyphButton_TT;
	TscGPGlyphButton *scGPGlyphButton_vozik_edit;
  TscGPGlyphButton *scGPGlyphButton_refresh;
  TrHTMLLabel *rHTMLLabel_info_zmenaR;
  TscGPGlyphButton *scGPGlyphButton_ADD;
  TscGPGlyphButton *scGPGlyphButton1;
  TscGPGlyphButton *scGPGlyphButton_smazat_pohon;
  TscGPNumericEdit *rEditNum_takt;
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
	void __fastcall scHTMLLabel_doporuc_pohonyClick(TObject *Sender);
	void __fastcall scGPSwitchChangeState(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall rEditNum_taktClick(TObject *Sender);
	void __fastcall scGPGlyphButton_TTClick(TObject *Sender);
	void __fastcall sc(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall rHTMLLabel_InfoTextClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall Button1Click(TObject *Sender);




private:	// User declarations
	TPL_math pm;//INSTANCE NA VÝPOÈETNÍ ÈÁST PL tj. PL_math
	enum Tinput_state{NO,NOTHING,LOADING,JOB,DV,SV,TT,RZ,RX,aRD,R,jednotky_prevod,R_prevod,Rz_prevod,aRD_prevod};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)
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
 	TColor clBACKGROUND;
public:		// User declarations
	__fastcall TForm_parametry_linky(TComponent* Owner);
	int getROW(int PID);//vrátí èíslo øádku dle pohon ID, pokud nenajde vrátí -1
	void vypis(UnicodeString text,bool red=true,bool link=false);
	void input_TT();
	void INPUT(double Sloupec, double Radek);
	void OUTPUT(double i,double Sloupec, double Radek);
	void Roletka_roztec(int Row);
	void show_min_Rz();
	double getTT();
	void VALIDACE(int ACol, int ARow);
  int COL,ROW;

  	//metody volané z Tmgrid
	void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
  void OnKeyPress (TObject *Sender, System::WideChar &Key);
  void getmGridColors();
  void getmGridWidth();
  void getDeleteButtonSettings(int Row);
  void getPrirazeneObjDesign(int Row);
  void setADD_ButtonPosition();
  void setFormHeight();

	short VID;//validation ID
	double VID_value;
	short Row_validace;
  short Col_validace;
	UnicodeString NAZEV;
	double OD;
	double DO;
	bool data_nalezena;
	Tinput_state input_state;//stav vstupu DV,SV,TT...atd
	Tinput_clicked_edit input_clicked_edit;//zjisteni na ktery edit bylo kliknuto
	Tinput_clicked_icon input_clicked_icon;//zjisteni na ktery icon bylo kliknuto
	Tinput_onchange   onchange;
	enum Tm_mm{M=0,MM};Tm_mm Delkaunit;Tm_mm Sirkaunit;Tm_mm Runit;Tm_mm Rzunit;Tm_mm Dmunit;//pøepínaè jednotek vzdálenost
	enum Tminsec{S=0,MIN};Tminsec Taktunit;Tminsec RDunit;Tminsec minsec;Tminsec aRDunit;//pøepínaè jednotek èasu
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
  AnsiString OBJEKTY_POUZIVAJICI_POHON;

	int roletka_data;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
#endif
