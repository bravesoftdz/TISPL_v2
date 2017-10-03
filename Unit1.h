//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "RzPanel.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "RzSplit.hpp"
#include "RzStatus.hpp"
#include <Vcl.Grids.hpp>
#include "RzButton.hpp"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include "RzBorder.hpp"
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include "RzBHints.hpp"
#include "RzGrids.hpp"
#include <VCLTee.GanttCh.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "RzLabel.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdDayTime.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdTime.hpp>
#include <IdSNTP.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdHTTP.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.MySQL.hpp>
#include <FireDAC.Phys.MySQLDef.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <Data.FMTBcd.hpp>
#include <Data.SqlExpr.hpp>
#include <Data.DBXMySQL.hpp>
#include <FireDAC.Comp.Script.hpp>
#include <FireDAC.Comp.ScriptCommands.hpp>
#include <FireDAC.Stan.Util.hpp>
#include <FireDAC.Comp.UI.hpp>
#include <FireDAC.VCLUI.Script.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include "rImprovedComps.hpp"
#include "rStringGridEd.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scStyledForm.hpp"
#include "scAdvancedControls.hpp"
#include "scExtControls.hpp"
#include "scModernControls.hpp"
#include <Vcl.Mask.hpp>
#include <Vcl.WinXCtrls.hpp>
#include "scDrawUtils.hpp"
#include "scGPImages.hpp"

#include "MyString.h"
#include "my.h"
#include "vykresli.h"
//ZDM #include "grafy.h"

//#include "vektory.h" //už vkládám pøes vykresli.h
//#include "knihovna_objektu.h" //už vkládám pøes vykresli.h resp. vektory.h

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *PopupMenu1;
	TRzToolbar *RzToolbar1;
	TRzStatusPane *RzStatusPane1;
	TMenuItem *Nastvitparametry1;
	TMenuItem *Smazat1;
	TRzStatusPane *RzStatusPane3;
	TRzStatusPane *RzStatusPane4;
	TButton *Button1;
	TMenuItem *N5;
	TMenuItem *Priblizit2;
	TMenuItem *Oddalit2;
	TMenuItem *Posouvat2;
	TMenuItem *Posunout3;
	TMenuItem *Vybratoknem2;
	TImageList *ImageList1;
	TRzToolButton *RzToolButton1;
	TRzToolButton *RzToolButton2;
	TRzToolButton *RzToolButton3;
	TRzSpacer *RzSpacer1;
	TRzToolButton *RzToolButton4;
	TRzToolButton *RzToolButton5;
	TRzToolButton *RzToolButton6;
	TRzToolButton *RzToolButton7;
	TRzToolButton *RzToolButton8;
	TRzToolButton *RzToolButton9;
	TRzSpacer *RzSpacer2;
	TRzToolButton *RzToolButton10;
	TRzToolButton *RzToolButton11;
	TRzToolButton *RzToolButton12;
	TMemo *Memo1;
	TRzStatusPane *RzStatusPane5;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog1;
	TTimer *Timer_backup;
	TSavePictureDialog *SavePictureDialog1;
	TTimer *Timer_simulace;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TChart *Chart1;
	TChart *Chart2;
	TChart *Chart3;
	TChart *Chart4;
	TLabel *Label_wip;
	TGanttSeries *Series1;
	TBarSeries *Series5;
	TBarSeries *Series6;
	TMenuItem *Rychlexport1;
	TMenuItem *Zobrazitparametry1;
	TButton *Button2;
	TTimer *Timer_neaktivity;
	TButton *ButtonPLAY;
	TTimer *Timer_animace;
	TChart *Chart6;
	TCheckBox *CheckBoxAnimovatSG;
	TLabel *Label_zamerovac;
	TPieSeries *Series7;
	TTeeGDIPlus *TeeGDIPlus1;
	TTeeGDIPlus *TeeGDIPlus2;
	TTeeGDIPlus *TeeGDIPlus3;
	TTeeGDIPlus *TeeGDIPlus4;
	TTeeGDIPlus *TeeGDIPlus5;
	TComboBox *ComboBoxDOmin;
	TComboBox *ComboBoxODmin;
	TLabel *LabelRoletka;
	TBarSeries *Series2;
	TBarSeries *Series3;
	TTimer *TimerMouseWheel;
	TBarSeries *Series9;
	TBarSeries *Series10;
	TIdTime *IdTime1;
	TIdHTTP *IdHTTP1;
	TTimer *Timer_tr;
	TFDConnection *FDConnection1;
	TCheckBox *CheckBox_pouzit_zadane_kapacity;
	TFDQuery *FDQuery1;
	TButton *SQL_processID;
	TFDQuery *FDQuery2;
	TrComboBoxEx *rComboBoxKrok;
	TscGPPanel *scGPPanel_mainmenu;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPButton *editacelinky1;
	TscGPButton *PopupMenuButton;
	TscGPButton *casovosa1;
	TscGPButton *technologickprocesy1;
	TscGPGlyphButton *scGPGlyphButton1;
	TscGPGlyphButton *Ulozit;
	TscGPGlyphButton *NovySoubor;
	TscGPGlyphButton *Otevrit;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *DetailsButton;
	TscGPPanel *scGPPanel_statusbar;
	TscGPTrackBar *scGPTrackBar1;
	TscGPGlyphButton *scGPGlyphButton_ZOOM_MINUS;
	TscGPGlyphButton *scGPGlyphButton_ZOOM_PLUS;
	TscGPSizeBox *scGPSizeBox1;
	TscCheckBox *scCheckBox2;
	TscSplitView *scSplitView_OPTIONS;
	TscCheckBox *scCheckBox1;
	TscStyledForm *scStyledForm1;
	TscRadioGroup *scRadioGroup1;
	TscGPGlyphButton *scGPGlyphButton2;
	TscGPButton *layout;
	TscGPButton *simulace;
	TscButton *scButton3;
	TscButton *scButton1;
	TscButton *scButton4;
	TscButton *scButton5;
	TscGPSwitch *scGPSwitch4;
	TscLabel *scLabel11;
	TscLabel *scLabel16;
	TscGPSwitch *scGPSwitch5;
	TscGPGlyphButton *scGPGlyphButton15;
	TImageList *ImageList48;
	TscGPVirtualImageList *scGPVirtualImageList1;
	TscButton *scButton2;
	TscButton *scButton6;
	TscSplitView *scSplitView_MENU;
	TscButton *scButton7;
	TscButton *scButton8;
	TscButton *scButton9;
	TscSplitView *scSplitView_LEFTTOOLBAR;
	TscListGroupPanel *scListGroupKnihovObjektu;
	TscListGroupPanel *scListGroupNastavProjektu;
	TscLabel *scLabel14;
	TscLabel *scLabel15;
	TscGPGlyphButton *scGPGlyphButton7;
	TscGPGlyphButton *scGPGlyphButton8;
	TDrawGrid *DrawGrid_knihovna;
	TscGPSwitch *scGPSwitch9;
	TscLabel *scLabel19;
	TscLabel *scLabel_ZOOM;
	TMenuItem *Celpohled1;
	TscGPSwitch *scGPSwitch_meritko;
	TscLabel *scLabel5;
	TscButton *scButton_ulozit;
	TscGPCheckBox *CheckBoxVymena_barev;
	TscGPCheckBox *CheckBoxVytizenost;
	TscComboBox *ComboBoxCekani;
	TscGPLabel *pravyoption_nadpis;
	TscExPanel *scExPanel_ostatni;
	TscExPanel *scExPanel_vrstvy;
	TscLabel *scLabel4;
	TscLabel *scLabel_titulek;
	TscLabel *scLabel1;
	void __fastcall Konec1Click(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall DrawGrid_knihovnaDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall testovnkapacity1Click(TObject *Sender);
	void __fastcall casoverezervy1Click(TObject *Sender);
	void __fastcall editacelinky1Click(TObject *Sender);
	void __fastcall DrawGrid_knihovnaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DrawGrid_knihovnaMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
	void __fastcall DrawGrid_knihovnaMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
	void __fastcall DrawGrid_knihovnaMouseLeave(TObject *Sender);
	void __fastcall DrawGrid_knihovnaKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DrawGrid_knihovnaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall RzSizePanel_knihovna_objektu_OLDMouseEnter(TObject *Sender);
	void __fastcall Piblit1Click(TObject *Sender);
	void __fastcall Oddlit1Click(TObject *Sender);
	void __fastcall Predchozipohled1Click(TObject *Sender);
	void __fastcall Priblizit2Click(TObject *Sender);
	void __fastcall Oddalit2Click(TObject *Sender);
	void __fastcall Posouvat1Click(TObject *Sender);
	void __fastcall Posunout2Click(TObject *Sender);
	void __fastcall Vybratoknem1Click(TObject *Sender);
	void __fastcall Vybratoknem2Click(TObject *Sender);
	void __fastcall Posouvat2Click(TObject *Sender);
	void __fastcall Posunout3Click(TObject *Sender);
	void __fastcall RzToolButton8Click(TObject *Sender);
	void __fastcall RzToolButton9Click(TObject *Sender);
	void __fastcall RzToolButton11Click(TObject *Sender);
	void __fastcall Smazat1Click(TObject *Sender);
	void __fastcall Nastvitparametry1Click(TObject *Sender);
	void __fastcall RzStatusPane5Click(TObject *Sender);
	void __fastcall Edit_takt_timeChange(TObject *Sender);
	void __fastcall Edit_pocet_vozikuChange(TObject *Sender);
	void __fastcall UlozitClick(TObject *Sender);
	void __fastcall UlozitjakoClick(TObject *Sender);
	void __fastcall OtevritClick(TObject *Sender);
	void __fastcall NovySouborClick(TObject *Sender);
	void __fastcall Timer_backupTimer(TObject *Sender);
	void __fastcall Obnovitzezlohy1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall OtevritsablonuClick(TObject *Sender);
	void __fastcall Export1Click(TObject *Sender);
	void __fastcall Button_kalkulatorTTClick(TObject *Sender);
	void __fastcall Button_dopravnik_parametry_OLDClick(TObject *Sender);
	void __fastcall Button_vozik_parametry_OLDClick(TObject *Sender);
	void __fastcall Edit_takt_timeEnter(TObject *Sender);
	void __fastcall Obnovitobraz1Click(TObject *Sender);
	void __fastcall csv1Click(TObject *Sender);
	void __fastcall html1Click(TObject *Sender);
	void __fastcall simulace1Click(TObject *Sender);
	void __fastcall RzStatusPane1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Timer_simulaceTimer(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Vypicestuktempu1Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall casovosa1Click(TObject *Sender);
	void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
	void __fastcall CheckBoxPALCE_Click(TObject *Sender);
	void __fastcall Chart1Click(TObject *Sender);
	void __fastcall technologickprocesy1Click(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall Zobrazitparametry1Click(TObject *Sender);
	void __fastcall Rychlexport1Click(TObject *Sender);
	void __fastcall MagnaClick(TObject *Sender);
	void __fastcall SPPP1Click(TObject *Sender);
	void __fastcall Boskovice1Click(TObject *Sender);
	void __fastcall eXtreme1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Timer_neaktivityTimer(TObject *Sender);
	void __fastcall ButtonPLAYClick(TObject *Sender);
	void __fastcall Timer_animaceTimer(TObject *Sender);
	void __fastcall CheckBoxVytizenost_Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall CheckBoxVymena_barev_Click(TObject *Sender);
	void __fastcall ComboBoxODminChange(TObject *Sender);
	void __fastcall ComboBoxDOminChange(TObject *Sender);
	void __fastcall TimerMouseWheelTimer(TObject *Sender);
	void __fastcall Timer_trTimer(TObject *Sender);
	void __fastcall CheckBox_pouzit_zadane_kapacityClick(TObject *Sender);
	void __fastcall SQL_processIDClick(TObject *Sender);
	void __fastcall rComboBoxKrokChange(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall MaxButtonClick(TObject *Sender);
	void __fastcall PopupMenuButtonClick(TObject *Sender);
	void __fastcall scGPGlyphButton1Click(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPGlyphButton_ZOOM_MINUSClick(TObject *Sender);
	void __fastcall scGPGlyphButton_ZOOM_PLUSClick(TObject *Sender);
	void __fastcall scLabel_titulekDblClick(TObject *Sender);
	void __fastcall MinButtonClick(TObject *Sender);
	void __fastcall scGPGlyphButton2Click(TObject *Sender);
	void __fastcall Button_dopravnik_parametryClick(TObject *Sender);
	void __fastcall Button_vozik_parametryClick(TObject *Sender);
	void __fastcall DetailsButtonClick(TObject *Sender);
	void __fastcall scGPTrackBar1Change(TObject *Sender);
	void __fastcall scGPSwitch4ChangeState(TObject *Sender);
	void __fastcall scGPSwitch5ChangeState(TObject *Sender);
	void __fastcall scGPSwitch9ChangeState(TObject *Sender);
	void __fastcall scSplitViewsClosing(TObject *Sender);
	void __fastcall scSplitViewsClosed(TObject *Sender);
	void __fastcall scExPanel_ostatniClick(TObject *Sender);
	void __fastcall scExPanel_vrstvyClick(TObject *Sender);
	void __fastcall scGPSwitch_meritkoChangeState(TObject *Sender);
	void __fastcall scSplitView_OPTIONSMouseLeave(TObject *Sender);
	void __fastcall scSplitView_MENUMouseLeave(TObject *Sender);

private:	// User declarations
	////struktury, výèty
	enum Tedice{DEVELOPER,ARCHITECT,CLIENT,VIEWER,DEMO};Tedice EDICE;
	enum Tmod{NO=0,SCHEMA,CASOVAOSA,TECHNOPROCESY,SIMULACE};Tmod MOD;
	//enum Takce - muselo být pøesunutu do public sekce kvùli AA ve vykresli
	enum TKurzory {standard=0,posun_v,posun_b,posun_p,posun_l,posun_t,kalibrovat,pan,pan_move,window,add_o};
	struct Tnastaveni{bool autosave;unsigned short int minut;bool posledni_file;};Tnastaveni nastaveni;

	////instance
	Graphics::TBitmap *Pan_bmp;
	Cvektory::TObjekt *pom;
	Cvektory::TProces *proces_pom;

	////metody
	void edice();
	short int MB(UnicodeString text, unsigned short int typ=0,UnicodeString titulek="TISPL - Eltep");//vola rychle messabox
	void REFRESH(bool invalidate=true); //vybere buï Invalidate nebo FormPaint(this) dle if(!antialiasing a dle Invalidate=true), tedy když bude zapnutý antialising jde vždy do vìtve else
	void ESC();
	void UP();void DOWN();void RIGHT();void LEFT();void Uloz_predchozi_pohled();//realizují posuny obrazu
	void ZOOM_IN();//pøiblížení
	void ZOOM_OUT();//oddálení
	void ZOOM();//samotný ZOOM
	void ZOOM_WINDOW();//pøiblížení oknem
	void on_change_zoom_change_scGPTrackBar();
	void zneplatnit_minulesouradnice();
	void kurzor(TKurzory typ_kurzor);
	void pan_map(TCanvas * canv, int X, int Y);
	void pan_move_map();
	void add_objekt(int X, int Y);
	Cvektory::TObjekt *add_objekt_za();
	void move_objekt(int X, int Y);
	void zobraz_tip(UnicodeString text);
	void akutalizace_stavu_prichytavani_vSB();
	void vytvor_hlavicku_souboru();
	void Ulozit_soubor();//samotné uložení
	void OtevritSoubor();//realizuje otevøení opendialogu s následným voláním realizace samotného otevøení souboru
	unsigned short int OtevritSoubor(UnicodeString soubor);//realizuje samotné otevøení souboru
	void ulozit_posledni_otevreny();//uloží do ini nazev posledního pracovního souboru
	void vse_odstranit();
	UnicodeString get_computer_name();
	UnicodeString get_user_name();
	UnicodeString get_temp_dir();
	void nacist_nastaveni();
	void ulozit_nastaveni();
	void zavrit_uvod();//zavøe úvodní dialog
	int vrat_max_vysku_grafu();
	void nastaveni_grafickeho_vystupu(Graphics::TBitmap * Bitmap);
	bool ttr(UnicodeString Text);
	void log2web(UnicodeString Text);//automaticky pøidá parametry (èas, uživatel, licence)
	void log2webOnlyText(UnicodeString Text);//pouze text
	void startUP();//pøi aktivaci formuláøe, pro zpøehlednìní kodu
	void NewDesignSettings();

	TDateTime TIME;
	UnicodeString LICENCE;

	////promìnné
	int size_grid;

	int vybrany_objekt;

	TPoint akt_souradnice_kurzoru_PX;//uchová aktuální pozici kurzoru
	TPointD akt_souradnice_kurzoru;//uchová aktuální pozici kurzoru v logických jednotkách, resp. souøadnicích
	TPoint vychozi_souradnice_kurzoru;//uchová výchozí pozici kurzoru
	TPoint predchozi_souradnice_kurzoru;//uchová pùvodní pozici kurzoru pøi stisku tlaèítka myši
	TPoint minule_souradnice_kurzoru;//uchová pùvodní souøadnice pøi posunu

	bool pan_non_locked;
	bool stisknute_leve_tlacitko_mysi;//uchovává stav levého tlaèítka myši
	unsigned short int funkcni_klavesa;//uchovává stav poslední stisknuté funkèní klávesy
	unsigned short int vyska_menu;

	double Zoom_predchozi;
	TPointD Posun_predchozi;
	short jedno_ze_tri_otoceni_koleckem_mysi;
	short doba_neotaceni_mysi;
	short prichytavat_k_mrizce;
	bool uchop_zobrazen;
	bool vycentrovat;
	bool posun_objektu;
	bool probehl_zoom;
	bool add_posledni;
	bool upozornovat_na_zmenu_TT_parametru;
	bool scSplitViews_closing_on_AA;
	bool SplitViewOpen;

	bool duvod_k_ulozeni;
	bool stisknuto_storno;

	UnicodeString Edice_caption;

	short pocitadlo_doby_neaktivity;


public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	double m2px;//uchovává hodnotu prostorového rozlišení programu, nativní rozlišení 0,1 m na 1 pixel pøi zoomu 1x
	TMyString ms;
	Cmy m;
	Cvykresli d;
	//ZDM Cgrafy g;
  UnicodeString FileName;
	enum Takce{NIC=0,PAN,PAN_MOVE,ZOOM_W,ZOOM_W_MENU,ADD,MOVE};Takce Akce;
	double Zoom; //promìnná uchovávajicí velikost Zoomu
	TPointD Posun;//promìnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souøadnicích zaøízení
  bool grid;
	bool zobrazit_barvy_casovych_rezerv;
	bool antialiasing;
	double Zoom_predchozi_AA;//pøi antialiasingu


	void DuvodUlozit(bool stav);
	void SB(UnicodeString Text, unsigned short Pane=4);//domnívám se, že zde má být hodnota 5
	void S(UnicodeString Text="");//Usnadòuje pøístup k ShowMessage
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
extern AnsiString Parametry; //pøíjímá parametry programu, dùležité pro otevírání programu konrkétním souborem
//---------------------------------------------------------------------------
#endif
