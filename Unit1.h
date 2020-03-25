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
#include <Vcl.ComCtrls.hpp>
#include "scHtmlControls.hpp"
#include "scWebBrowser.hpp"
#include <SHDocVw.hpp>
#include <Vcl.OleCtrls.hpp>

#include "MyString.h"
#include "my.h"
#include "vykresli.h"
#include "grafy.h"
#include "PO_math.h"
#include "scGPExtControls.hpp"
#include "rHintWindow.hpp"
#include "UnitX.h"
#include <Vcl.Imaging.pngimage.hpp>
#include "scImageCollection.hpp"
#include "rHintWindow.hpp"
//#include "vektory.h" //u vkládám pøes vykresli.h
//#include "knihovna_objektu.h" //u vkládám pøes vykresli.h resp. vektory.h

//podmínìnı pøeklad, zda se jedná èí nejedná o debug
#ifdef _DEBUG
const bool DEBUG=true;
#else
const bool DEBUG=false;
#endif
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TRzToolbar *RzToolbar1;
	TRzStatusPane *RzStatusPane3;
	TRzStatusPane *RzStatusPane4;
	TButton *Button1;
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
	TButton *Button2;
	TTimer *Timer_neaktivity;
	TButton *ButtonPLAY_O;
	TTimer *Timer_animace;
	TChart *Chart6;
	TCheckBox *CheckBoxAnimovatSG_OLD;
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
	TCheckBox *CheckBox_pouzit_zadane_kapacity_OLD;
	TFDQuery *FDQuery1;
	TButton *SQL_processID;
	TFDQuery *FDQuery2;
	TrComboBoxEx *rComboBoxKrok;
	TscGPPanel *scGPPanel_mainmenu;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPButton *Schema;
	TscGPButton *PopupMenuButton;
	TscGPButton *Analyza;
	TscGPButton *Synteza;
	TscGPGlyphButton *Toolbar_Ulozit;
	TscGPGlyphButton *Toolbar_NovySoubor;
	TscGPGlyphButton *Toolbar_Otevrit;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *DetailsButton;
	TscGPPanel *scGPPanel_statusbar;
	TscGPTrackBar *scGPTrackBar1;
	TscGPGlyphButton *scGPGlyphButton_ZOOM_MINUS;
	TscGPGlyphButton *scGPGlyphButton_ZOOM_PLUS;
	TscGPSizeBox *scGPSizeBox;
	TscSplitView *scSplitView_OPTIONS;
	TscStyledForm *scStyledForm1;
	TscGPGlyphButton *scGPGlyphButton2;
	TscGPButton *Layout;
	TscGPButton *Simulace;
	TscButton *scButton_otevrit;
	TscButton *scButton_konec;
	TscButton *scButton4;
	TscButton *scButton_zaloha;
	TscGPSwitch *scGPSwitch_AA;
	TscLabel *scLabel11;
	TscLabel *scLabel16;
	TscGPSwitch *scGPSwitch_grid;
  TscGPGlyphButton *scGPGlyphButton_napoveda;
	TImageList *ImageList48;
	TscGPVirtualImageList *scGPVirtualImageList1;
	TscButton *scButton_report;
	TscSplitView *scSplitView_MENU;
	TscButton *scButton_ulozjako;
	TscButton *scButton_export;
	TscButton *scButton_posledni_otevreny;
	TscSplitView *scSplitView_LEFTTOOLBAR;
	TscListGroupPanel *scListGroupKnihovObjektu;
	TDrawGrid *DrawGrid_knihovna;
	TscLabel *scLabel_ZOOM;
	TscGPSwitch *scGPSwitch_meritko;
	TscLabel *scLabel5;
	TscGPCheckBox *CheckBoxVymena_barev;
	TscGPCheckBox *CheckBoxVytizenost;
	TscComboBox *ComboBoxCekani;
	TscGPLabel *pravyoption_nadpis;
	TscExPanel *scExPanel_vrstvy;
	TscLabel *scLabel_titulek;
	TButton *hl_spojak_zakazky;
	TMemo *Memo2;
	TButton *button_zakazky_temp;
	TscButton *scButton_novy;
	TButton *Button11;
	TscGPGlyphButton *scGPGlyphButton_close_grafy;
	TscButton *scButton_ulozit;
	TscGPSwitch *scGPSwitch_rezim;
	TscLabel *scLabel_klient;
	TscLabel *scLabel_architekt;
	TscExPanel *scExPanel_html;
	TWebBrowser *WebBrowser1;
	TButton *pohonobjekt;
	TButton *Button12;
	TscGPGlyphButton *scGPGlyphButton_close_legenda_casove_osy;
	TButton *Button13;
	TscGPGlyphButton *scGPButton_generuj;
	TscLabel *scLabel_filtrovat;
	TscLabel *scLabel_doba_cekani;
	TscGPCheckBox *CheckBoxAnimovatSG;
	TscGPCheckBox *CheckBox_pouzit_zadane_kapacity;
	TscGPGlyphButton *ButtonPLAY;
	TscLabel *scLabel_mezera;
	TscGPButton *scGPButton_header_def_zakazek;
	TscGPCheckBox *scGPCheckBox_pocet_voziku_dle_WIP;
	TscGPGlyphButton *scGPGlyphButton_info_cekani;
  TscListGroupPanel *scListGroupPanel_hlavickaOtoce;
  TDrawGrid *DrawGrid_otoce;
  TscListGroupPanel *scListGroupPanel_hlavickaOstatni;
  TDrawGrid *DrawGrid_ostatni;
  TscListGroupPanel *scListGroupPanel_geometrie;
  TDrawGrid *DrawGrid_geometrie;
  TscListGroupPanel *scListGroupPanel_poznamky;
  TDrawGrid *DrawGrid_poznamky;
  TscGPCheckBox *scGPCheckBox_zobraz_podklad;
  TscButton *scButton_nacist_podklad;
  TscGPCheckBox *scGPCheckBox_stupne_sedi;
  TscGPTrackBar *scGPTrackBar_svetelnost_posuvka;
  TscLabel *scLabel1_svetelnost;
  TscGPButton *scGPButton_kalibrace;
  TscGPLabel *scGPLabel_otoce;
  TscGPLabel *scGPLabel_stop;
  TscGPLabel *scGPLabel_geometrie;
  TscGPLabel *scGPLabel_poznamky;
  TscGPLabel *scGPLabel_roboti;
  TscGPButton *scGPButton_adjustace;
  TscExPanel *scExPanel_podklad;
	TMemo *Memo3;
  TTimer *Timer2;
	TscGPPanel *scGPPanel_bottomtoolbar;
	TscGPLabel *scGPLabel1;
	TscGPComboBox *scGPComboBox_orientace;
	TscGPButton *scGPButton_ulozit;
	TscGPButton *scGPButton_zahodit;
	TscButton *scButton_zamek;
	TTimer *TimerKurzor;
	TscGPButton *Nahled;
	TscGPButton *scGPButton_viditelnostmGrid;
	TscGPButton *scGPButton_viditelnostKoty;
	TscGPButton *scGPButton_posun_dalsich_elementu;
	TscGPGlyphButton *scGPGlyphButton_PLAY;
	TscGPComboBox *scGPComboBox_prepinacKot;
	TscGPLabel *scGPLabel_prepinacKot;
	TscGPImage *scGPImage_mereni_vzdalenost;
	TscGPSwitch *scGPSwitch_robot_clovek;
	TscGPPanel *scGPPanel_pomocn_proSwitch;
	TButton *Button14;
  TscGPButton *scGPButton_mereni_vzdalenost;
  TscExPanel *scExPanel_ostatni;
  TscGPCheckBox *scGPCheckBox_ortogon;
  TscExPanel *scExPanel_nastaveni_starych_modu;
	TscGPImage *scGPImage_zamek_posunu;
	TscButton *scButton_zamek_layoutu;
	TscGPImageCollection *scGPImageCollection_Objekty;
	TImage *Image_knihovna_objektu;
  TscLabel *scLabel1_intenzita;
  TscGPTrackBar *scGPTrackBar_intenzita;
  TscGPButton *scGPButton_zmerit_vzdalenost;
	TscGPCheckBox *scGPCheckBox_zobrazit_pozice;
	TscGPCheckBox *scGPCheckBox_zobrazit_rotace_jigu_na_otocich;
  TscExPanel *scExPanel1;
	TscGPCheckBox *scGPCheckBox_zobrazit_koleje;
  TscGPCheckBox *scGPCheckBox1_popisky;
  TscGPSwitch *scGPSwitch1;
  TscLabel *scLabel_CZ;
  TscLabel *scLabel_EN;
	TscGPCheckBox *scGPCheckBox_zobrazit_palce;
	TscGPButton *scGPButton_prichytavat;
  TscGPButton *scGPButton_error;
  TscGPButton *scGPButton_warning;
	TscGPButton *scGPButton_smazat;
  TrHTMLHint *rHTMLHint1;
	TscGPButton *scGPButton_geometrie;
	TscGPCheckBox *scGPCheckBox_rozmisteni_voziku;
	TscGPButton *scGPButton_ulozit_cestu;
	TscGPButton *scGPButton_storno_cesta;
	TscGPButton *scGPGlyphButton_odstran_cestu;
	TPopupMenu *PopupMenu1;
	TMenuItem *N11;
	TMenuItem *N21;
  TPopupMenu *PopupMenu_posledni_projekty;
  TMenuItem *N1projekt1;
  TMenuItem *N2projekt1;
  TMenuItem *N3projekt1;
	void __fastcall Konec1Click(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall DrawGrid_knihovnaDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall testovnkapacity1Click(TObject *Sender);
	void __fastcall casoverezervy1Click(TObject *Sender);
	void __fastcall schemaClick(TObject *Sender);
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
	void __fastcall NastavitparametryClick1Click(TObject *Sender);
	void __fastcall RzStatusPane5Click(TObject *Sender);
	void __fastcall Edit_takt_timeChange(TObject *Sender);
	void __fastcall Edit_pocet_vozikuChange(TObject *Sender);
	void __fastcall UlozitClick(TObject *Sender);
	void __fastcall UlozitjakoClick(TObject *Sender);
	void __fastcall Toolbar_OtevritClick(TObject *Sender);
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
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall AnalyzaClick(TObject *Sender);
	void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
	void __fastcall CheckBoxPALCE_Click(TObject *Sender);
	void __fastcall Chart1Click(TObject *Sender);
	void __fastcall SyntezaClick(TObject *Sender);
	void __fastcall Zobrazitparametry1Click(TObject *Sender);
	void __fastcall Rychlexport1Click(TObject *Sender);
	void __fastcall MagnaClick(TObject *Sender);
	void __fastcall SPPP1Click(TObject *Sender);
	void __fastcall Boskovice1Click(TObject *Sender);
	void __fastcall eXtreme1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Timer_neaktivityTimer(TObject *Sender);
	void __fastcall ButtonPLAY_OClick(TObject *Sender);
	void __fastcall Timer_animaceTimer(TObject *Sender);
	void __fastcall CheckBoxVytizenost_Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall CheckBoxVymena_barev_Click(TObject *Sender);
	void __fastcall ComboBoxODminChange(TObject *Sender);
	void __fastcall ComboBoxDOminChange(TObject *Sender);
	void __fastcall TimerMouseWheelTimer(TObject *Sender);
	void __fastcall Timer_trTimer(TObject *Sender);
	void __fastcall CheckBox_pouzit_zadane_kapacity_OLDClick(TObject *Sender);
	void __fastcall SQL_processIDClick(TObject *Sender);
	void __fastcall rComboBoxKrokChange(TObject *Sender);
	void __fastcall MaxButtonClick(TObject *Sender);
	void __fastcall scGPGlyphButton_OPTIONS_OldClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPGlyphButton_ZOOM_MINUSClick(TObject *Sender);
	void __fastcall scGPGlyphButton_ZOOM_PLUSClick(TObject *Sender);
	void __fastcall scLabel_titulekDblClick(TObject *Sender);
	void __fastcall MinButtonClick(TObject *Sender);
	void __fastcall scGPGlyphButton2Click(TObject *Sender);
	void __fastcall Button_dopravnik_parametryClick(TObject *Sender);
	void __fastcall DetailsButtonClick(TObject *Sender);
	void __fastcall scGPTrackBar1Change(TObject *Sender);
	void __fastcall scGPSwitch_AAChangeState(TObject *Sender);
	void __fastcall scGPSwitch_gridChangeState(TObject *Sender);
	void __fastcall scGPSwitch_rezimChangeState(TObject *Sender);
	void __fastcall scSplitViewsClosing(TObject *Sender);
	void __fastcall scSplitViewsClosed(TObject *Sender);
	void __fastcall scExPanel_ostatniClick(TObject *Sender);
	void __fastcall scExPanel_vrstvyClick(TObject *Sender);
	void __fastcall scGPSwitch_meritkoChangeState(TObject *Sender);
	void __fastcall scSplitView_OPTIONSMouseLeave(TObject *Sender);
	void __fastcall scSplitView_MENUMouseLeave(TObject *Sender);
	void __fastcall hl_spojak_zakazkyClick(TObject *Sender);
	void __fastcall button_zakazky_tempClick(TObject *Sender);
	void __fastcall scButton2Click(TObject *Sender);
	void __fastcall scGPGlyphButton_definice_zakazekClick(TObject *Sender);
	void __fastcall scGPGlyphButton_close_grafyClick(TObject *Sender);
	void __fastcall ComboBoxCekaniChange(TObject *Sender);
	void __fastcall scGPGlyphButton_zpravy_ikonaClick(TObject *Sender);
	void __fastcall scSplitView_OPTIONSOpened(TObject *Sender);
	void __fastcall LayoutClick(TObject *Sender);
	void __fastcall pohonobjektClick(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall scSplitView_OPTIONSPanelPaint(TCanvas *ACanvas, TRect &ARect);
	void __fastcall scGPCheckBox_ortogonClick(TObject *Sender);
	void __fastcall scGPGlyphButton_close_legenda_casove_osyClick(TObject *Sender);
	void __fastcall scGPButton_generujClick(TObject *Sender);
	void __fastcall scExPanel_log_headerClick(TObject *Sender);
	void __fastcall scExPanel_log_headerMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall scExPanel_log_headerMouseActivate(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate);
	void __fastcall scGPCheckBox_pocet_voziku_dle_WIPClick(TObject *Sender);
	void __fastcall scGPGlyphButton_info_cekaniClick(TObject *Sender);
	void __fastcall FormDblClick(TObject *Sender);
  void __fastcall DrawGrid_otoceDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
  void __fastcall DrawGrid_ostatniDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
  void __fastcall DrawGrid_otoceMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall DrawGrid_ostatniMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall Button11Click(TObject *Sender);
  void __fastcall scGPButton_stornoClick(TObject *Sender);
  void __fastcall scButton_nacist_podkladClick(TObject *Sender);
  void __fastcall DrawGrid_geometrieDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall DrawGrid_geometrieMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall scGPCheckBox_zobraz_podkladClick(TObject *Sender);
  void __fastcall scGPButton_kalibraceClick(TObject *Sender);
  void __fastcall scGPButton_adjustaceClick(TObject *Sender);
  void __fastcall DrawGrid_poznamkyDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall scGPGlyphButton_OPTIONSClick(TObject *Sender);
  void __fastcall scGPGlyphButton_OPTIONSMouseEnter(TObject *Sender);
  void __fastcall scGPGlyphButton_OPTIONSMouseLeave(TObject *Sender);
  void __fastcall scExPanel_podkladClick(TObject *Sender);
  void __fastcall scGPTrackBar_svetelnost_posuvkaChange(TObject *Sender);
  void __fastcall scGPCheckBox_stupne_sediClick(TObject *Sender);
  void __fastcall DrawGrid_ostatniMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
  void __fastcall DrawGrid_geometrieMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
  void __fastcall DrawGrid_poznamkyMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
  void __fastcall DrawGrid_ostatniMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
  void __fastcall DrawGrid_geometrieMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
  void __fastcall DrawGrid_poznamkyMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
  void __fastcall Button_testClick(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
  void __fastcall scGPEdit1Change(TObject *Sender);
  void __fastcall Timer2Timer(TObject *Sender);
	void __fastcall scButton_zamekClick(TObject *Sender);
	void __fastcall TimerKurzorTimer(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall scGPButton_viditelnostmGridClick(TObject *Sender);
	void __fastcall scGPButton_viditelnostKotyClick(TObject *Sender);
	void __fastcall scGPButton_posun_dalsich_elementuClick(TObject *Sender);
	void __fastcall scGPGlyphButton_PLAYClick(TObject *Sender);
	void __fastcall scGPImage_mereni_vzdalenostClick(TObject *Sender);
	void __fastcall scGPSwitch_robot_clovekChangeState(TObject *Sender);
	void __fastcall scGPComboBox_prepinacKotClick(TObject *Sender);
	void __fastcall Button14Click(TObject *Sender);
  void __fastcall scGPButton_posun_halyClick(TObject *Sender);
  void __fastcall scGPButton_nakreslit_haluClick(TObject *Sender);
	void __fastcall NahledClick(TObject *Sender);
	void __fastcall scGPImage_zamek_posunuClick(TObject *Sender);
	void __fastcall scButton_zamek_layoutuClick(TObject *Sender);
  void __fastcall scGPTrackBar_intenzitaChange(TObject *Sender);
  void __fastcall scGPCheckBox_zobrazit_rotace_jigu_na_otocichClick(TObject *Sender);
  void __fastcall scGPCheckBox_zobrazit_poziceClick(TObject *Sender);
	void __fastcall Memo3KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall scGPCheckBox1_popiskyClick(TObject *Sender);
  void __fastcall scGPCheckBox_zobrazit_kolejeClick(TObject *Sender);
  void __fastcall scGPSwitch1ChangeState(TObject *Sender);
	void __fastcall scGPCheckBox_zobrazit_palceClick(TObject *Sender);
	void __fastcall scGPButton_prichytavatClick(TObject *Sender);
  void __fastcall scGPButton_errorClick(TObject *Sender);
  void __fastcall scGPButton_warningClick(TObject *Sender);
	void __fastcall scGPButton_smazatClick(TObject *Sender);
	void __fastcall scGPButton_geometrieClick(TObject *Sender);
	void __fastcall scGPButton_storno_cestaClick(TObject *Sender);
	void __fastcall scGPButton_ulozit_cestuClick(TObject *Sender);
	void __fastcall scGPCheckBox_rozmisteni_vozikuClick(TObject *Sender);
	void __fastcall scGPGlyphButton_odstran_cestuClick(TObject *Sender);
	void __fastcall N21Click(TObject *Sender);
	void __fastcall N1projekt1Click(TObject *Sender);
	void __fastcall N2projekt1Click(TObject *Sender);
	void __fastcall N3projekt1Click(TObject *Sender);



// User declarations
	////jen public struktury a vıèty
public:
	enum Tmod{NO=0,SCHEMA,LAYOUT,CASOVAOSA,TECHNOPROCESY,SIMULACE,EDITACE,TVORBA_CESTY};Tmod MOD;
	enum Tstatus{NAVRH,OVEROVANI};Tstatus STATUS;
	enum Takce{NIC=0,PAN,PAN_MOVE,ZOOM_W,ZOOM_W_MENU,ADD,MOVE,VYH,MEASURE,KALIBRACE,ADJUSTACE,MOVE_ELEMENT,MOVE_TABLE,OFFSET_KOTY,MOVE_KOMORA,ROZMER_KOMORA,DRAW_HALA,MOVE_HALA,MOVE_BOD,MOVE_USECKA,MOVE_TEXT,GEOMETRIE,BLOK,GEOMETRIE_LIGHT};Takce Akce;Takce Akce_temp;//akce temp slouí ke spuštìní akce pøi akci, pø. Akce=GEOMETRIE a pøi ní je potøeba pøesunout kóty geo. elementù, tudí Akce_temp=OFFSET_KOTY
	enum Tm_mm{M=0,MM,SEKUNDY,MINUTY};Tm_mm DOtocunit,DKunit,LOunit,Runit,Rzunit;//pøepínaè jednotek vzdálenost,rozšíøen o SEKUNDY,MINUTY (problém pøi pouití SEC a MIN) z dùvodu èasovıch a vzdálenostních kót
	enum Tminsec{SEC=0,MIN};Tminsec PTunit,aRDunit ;//pøepínaè jednotek èasu
	enum TKurzory {standard=0,posun_v,posun_b,posun_p,posun_l,posun_t,kalibrovat,pan,pan_move,window,add_o,neco,posun_ind,zmena_j,edit_text,zmena_d_x,zmena_d_y,posun_ind_ld,posun_ind_pd,editace_posun,info,close,posun_editace_obj,editace_obj};
	////instance
  TStringList *ls;

private:
	enum Tedice{DEVELOPER,ARCHITECT,CLIENT,VIEWER,DEMO};Tedice EDICE;
	struct Tnastaveni{bool autosave;unsigned short int minut;bool posledni_file;};Tnastaveni nastaveni;
	enum Tlanguage{EN,MN,CS};Tlanguage language;
	////instance
	Cvektory::TProces *proces_pom;
	TFileStream *LogFileStream;

	////metody
	void aut_pozicovani(Cvektory::TElement *E, int X, int Y);
	void edice();
	void ESC();
	void UP();void DOWN();void RIGHT();void LEFT();void Uloz_predchozi_pohled();//realizují posuny obrazu
	void ZOOM();//samotnı ZOOM
	void ZOOM_WINDOW();//pøiblíení oknem
	void on_change_zoom_change_scGPTrackBar();
	void zneplatnit_minulesouradnice();
	void pan_create();//vytvoøí vıøez pro pan_move
	void pan_map(TCanvas * canv, int X, int Y);
	void pan_move_map();
	Cvektory::TObjekt *add_objekt(int X, int Y);
	Cvektory::TObjekt *add_objekt_za();
	void move_objekt(double X, double Y,Cvektory::TObjekt *Objekt);
	void add_element(int X, int Y);
	void add_vyhybka_spojka();
	void add_komoru();//pøidávání komory kabinì powerwashe, kontrola zda není souèet kabin vìtší ne rozmìr kabiny
	short rotace_symbol(short trend,int X_bod,int Y_bod);//dle toho, zda je umisovanı element nad osou èi pod osou pohonu je vrácena rotace symbolu, X_bod,.. je bbod vkládání elementu (jedna souøadnice ho váe na pohon)
	void vytvoreni_tab_knihovna();//vytovoøení tabulky knihovny objektù
	void popisky_knihovna_nahled(bool knihovna);//pøepíná popisky mezi knihovnou a editací
	void odstraneni_elementu_tab_pohon(int operace);
	void zmena_jednotek_tab_pohon();
	void prvni_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce);
	void dalsi_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce);
	void zmen_poradi_objektu(int X, int Y);//testuje zda se nejedná o zmìnu poøadí (to musí ještì uivatel potvrdit)
	void zobraz_tip(UnicodeString text="", TCanvas* canv=NULL);//prázdnım (bez paremetrù) voláním  metody se tip smae, //pokud není parametr canvas uveden, jedná se o dlouhodobé vykreslování hodnoty TIP//pokud je parametrem pøedán Canvas vykreslí se pøímo a jednorázovì
	void akutalizace_stavu_prichytavani_vSB();
	void Novy_soubor(bool invalidate=true);//samotné vytvoøení nového souboru
	void Ulozit_soubor();//samotné uloení
	void Otevrit_soubor();//realizuje otevøení opendialogu s následnım voláním realizace samotného otevøení souboru
  void Nacist_podklad();//realizuje otevøení opendialogu s následnım voláním realizace samotného nacteni podkladu
	unsigned short int Otevrit_soubor(UnicodeString soubor);//realizuje samotné otevøení souboru
  unsigned short int Nacist_podklad(UnicodeString soubor);//realizuje nacteni podkladu
	void ulozit_posledni_otevreny();//uloí do ini nazev posledního pracovního souboru
	void vse_odstranit();
	UnicodeString get_computer_name();
	UnicodeString get_user_name();
	UnicodeString get_Windows_dir();
	int get_DPI();
	void redesign_element();
	AnsiString FileName_short(AnsiString FileName);
	void nacist_nastaveni();
	void ulozit_nastaveni();
	void zavrit_uvod();//zavøe úvodní dialog
	int vrat_max_vysku_grafu();//vratí maximální oblast
	bool najdi_nazev_obj(double X,double Y,Cvektory::TObjekt *Objekt);//vratí zda jsem v oblasti textu
  short najdi_popisky_PM(double X,double Y,Cvektory::TObjekt *Objekt);//najde na jakém popisku u PM se nachází kurzor, vrací 0: nenalezeno, 1: pohon 1, 2: pohon 2
	void getJobID(int X, int Y);//vrátí do globální promìnné JID ID úlohy/funkcionality v místì kurzoru, zároveò pokud v místì tabulky èi elementu nahraje ukazatel do globální promìnné pom_element, vıznam jednotlivıch JID hodnot v komentáøi definici metody
	void setJobIDOnMouseMove(int X, int Y);//dle místa kurzoru a vrácené JID (job id) nastaví úlohu
	void nastaveni_grafickeho_vystupu(Graphics::TBitmap * Bitmap,unsigned int OD,unsigned int PO);
	bool ttr(UnicodeString Text);
	void log2web(UnicodeString Text);//automaticky pøidá parametry (èas, uivatel, licence)
	void log2webOnlyText(UnicodeString Text);//pouze text
	void SaveText2File(AnsiString Text,AnsiString FileName);//zapíše danı textovı øetìzec do daného textového souboru
	void startUP();//pøi aktivaci formuláøe, pro zpøehlednìní kodu
	void DesignSettings();//nastavení designu v konstruktoru
	void aktualizace();//kontrola aktuálnosti verze a pøípadì nabídka na staení nové
	void onPopUP(int X, int Y);//nastavení zobrazení popUPmenu a jeho volání vèetnì pozice
	void close_all_items_popUPmenu();//zajistí skrıtí všech poloek popUPmenu
	void ortogonalizace_on_off();//zapíná èi vypíná automatickou ortogonalizaci
	void ortogonalizace();//volá ortogonalizaci schéma, pokud je ortogonalizace povolena
	void ortogonalizovat();//ortogonalizuje schéma
	void db_connection();  // pøipojení k DB serveru
	void akt_tabulek (Cvektory::TElement *E,AnsiString LO,AnsiString delka_otoce,AnsiString cas,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla);
	void vykresli_kurzor(int index);
	void zmenJednotekKot();
	int pocet_vyskytu_elementu_s_otoci(Cvektory::TObjekt *Objekt);//prohledá elementy v objektu, vrátí 0 pokud je rotace v objektu všude stejná, vrátí 1 pokud je pøítomno více rotací
	void vytvor_edit();//vytvoøí edit na místì hlavièky tabulky, slouí ke zmìnì názvu elementu
	void smaz_edit(bool refresh=true);//smae edit, kterı slouil pro zmìnu názvu elementu a novı název zapíše do elementu, defaultnì provede refresh, pokud není pøedáno parametrem jinak
	void vykresli_spojinici_EmGrid(TCanvas *Canv,Cvektory::TElement *E);//vykreslí spojnici mezi tabulkou a elementem z nejblišího rohu tabulky
	void nacti_podklad(TCanvas *Canv);
	unsigned short load_language(Tlanguage language,bool akt_mGrid=false);
	void change_languagein_mGrid();//aktualizace popiskù v tabulkách pøi zmìnì jazyka
	void zmena_editovaneho_objektu();//slouí k pøechodu z editace jednoho objektu do editace druhého objektu
	TPoint uprav_bod_vlozeni_elementu(TPoint bod_vlozeni,short rotace_symbolu,int eID=-1);//upraví bod kurzoru pro vloení elemntu na bod vykreslení elementu (robot na konci ramena)
  void set_enabled_mGrid(Cvektory::TElement *E);//zapne nebo vypne komponenty megridu v závislosti na tom zda má element pøiøazenı pohon
  void vlozeni_editace_geometrie();//vkládá novou geometrii nebo edituje ji stávající geometrii
	void ukonceni_geometrie();//ukonèení akce geometrie a pøípadné uzavøení kruhu
	void vloz_bod_haly_objektu(int X,int Y);//vloí bod haly nebo objektu na zvolené souøadnice (fyzické), zohlední pøichytávání
	void smaz_bod_haly_objektu(Cvektory::TBod *bod);//smae bod haly nebo objektu, pokud existují u jen 2 poslední body smae oba
	void mGrid_on_mGrid();//prohledá zda se pøekrıvají mGridy
  void mGrid_mimo_obraz(Cvektory::TElement *E=NULL);//kontrola zde je mGrid mimo obraz, pokud ano vypnutí komponent
  void mGrid_puvodni_stav(Cvektory::TElement *E=NULL);//nadesingnuje tabulky elementù nebo tabulku pohonu na pùvodní stav, obnovı komponenty, naplní comba, provede Update() mGridu
	void mGrid_komponenta_na_draw(TmGrid *mGrid,long Col,long Row);//smazì komponentu v dané buòce a zmìní typ bunky na DRAW
	void set_font(int velikost=14);//nastaví komponentám aFont
	bool pripnuti_dalsich_objektu();//pokud pøi uloení editovaného objektu je detekováno, e konec objketu nenavazuje na zaèátek následujísího objektu je poloen dotaz a po potvrzení dojde ke spojení
	void spojeni_prvni_posledni(double citlivost=0.5);//kontrola zda na sebe první a polední objekt navazují, pokud jsou blízko u sebe, ale nenavazují - naváe je
	void Otevri_posledni_ulozeny(UnicodeString soubor);//otevøe jeden z posledních otevøenıch souborù

	////promìnné
	TDateTime TIME;
	UnicodeString LICENCE;
	short n_prihlaseni;
	bool logovat;
	bool ortogonalizace_stav;
	bool kalibrace_hotova;
	bool pan_non_locked;
	bool stisknute_leve_tlacitko_mysi;//uchovává stav levého tlaèítka myši
	unsigned short int funkcni_klavesa;//uchovává stav poslední stisknuté funkèní klávesy
	unsigned short int vyska_menu;
	double Zoom_predchozi,Zoom_predchozi2;
	TPointD Posun_predchozi,Posun_predchozi2;
	short jedno_ze_tri_otoceni_koleckem_mysi;
	short doba_neotaceni_mysi;
	bool uchop_zobrazen;
	bool vycentrovat;
	bool posun_objektu;
	bool probehl_zoom;
	bool add_posledni;
	bool upozornovat_na_zmenu_TT_parametru;
	bool scSplitViews_closing_on_AA;
	bool SplitViewOpen;
	bool duvod_k_ulozeni;
	bool nahled_ulozen;
	bool stisknuto_storno;
	bool volat_parametry_linky;//pouito pøi soubor novı
	bool start_ortogonalizace;
	bool stav_kurzoru;//kurzon vykreslen/nevykreslen
	AnsiString nazev_puvodni;// pouíváno pro uchovávání pùvodního názvu objektu z dùvodu zrušení editace
	AnsiString Caption;
	short pocitadlo_doby_neaktivity;
	TPoint pocitadlo_zmeny_pozice;
	bool FMaximized;
	TRect FOldBoundsRect;
	bool PmGCheckLink;
	bool StopCheckLink;
	bool offset_spolus_rozmerem;//uchovává v sobì, zda má bıt pøi zmìnì rozmerù kabiny zmìnì i offset kót elementù
	UnicodeString Jazyk;
	int count_memo;//counter pro memo
	bool editace_geometrie_spustena;

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	//instance
	TMyString ms;
	Cmy m;
	Cvykresli d;
	Cgrafy g;
	TPO_math pm;//INSTANCE NA VİPOÈETNÍ ÈÁST PO tj. PO_math
	Graphics::TBitmap *Pan_bmp;//kvùli mGridu jinak staèí private
	//uklazatele
	Cvektory::TObjekt *pom,*pom_vyhybka,*OBJEKT_akt,*copyObjekt;
	Cvektory::TElement *pom_element,*pom_element_temp,*posledni_editovany_element,*element_temp;//element_temp je nulován pøi kadém pøejetí kurzoru pouíván na vìci kolem PM
	TmGrid *PmG,*mGrid_knihovna;//ukazatel na mGridovou tabulku pohonu
	Cvektory::TKomora *pom_komora,*pom_komora_temp;
	Cvektory::TBod *pom_bod,*pom_bod_temp;

	//souøadnicové promìnné
	TPoint akt_souradnice_kurzoru_PX;//uchová aktuální pozici kurzoru
	TPointD akt_souradnice_kurzoru;//uchová aktuální pozici kurzoru v logickıch jednotkách, resp. souøadnicích
	TPoint vychozi_souradnice_kurzoru;//uchová vıchozí pozici kurzoru
	TPoint predchozi_souradnice_kurzoru;//uchová pùvodní pozici kurzoru pøi stisku tlaèítka myši
	TPoint minule_souradnice_kurzoru;//uchová pùvodní souøadnice pøi posunu

	//promìnné
	UnicodeString VERZE;
	double m2px;//uchovává hodnotu prostorového rozlišení programu, nativní rozlišení 0,1 m na 1 pixel pøi zoomu 1x
	double fps;//frames per second, èetnost snímkù za sekundu - pouívá se pro animace a simulace
	double afps;//frames per second, aktuální èetnost snímkù za sekundu - pouívá se pro animace a simulace
	UnicodeString FileName;
	TFont *aFont;//aktuální nastavenı vıchozí font
	double Zoom; //promìnná uchovávajicí velikost Zoomu
	TPointD Posun;//promìnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzickıch souøadnicích zaøízení
	TPointD copyObjektRzRx;
	bool grid;
	int size_grid;
	short prichytavat_k_mrizce;
	bool zobrazit_barvy_casovych_rezerv;
	bool zobrazit_meritko;
	bool antialiasing;
	double Zoom_predchozi_AA;//pøi antialiasingu
	bool nahled_objektu;//uchovává stav, zda se jedná o náhled objekt èi regulerní zobrazení ve form1
	bool TZF;//TRIAL_zakazat_funkcionality
	bool dblClick;
	int vybrany_objekt;
	short VyID;//objekt-symbol vyhıbky - ID typu
	int JID;//JOB ID
  int knihovna_id; // id drawgrid knihovny
	int element_id;  // id vybraneho elementu z knihoven
	AnsiString TIP;//uchovává text TIPu
	bool editace_textu;//mimo enum akce z dùvodu zobrazování kurozù pøi editaci a pøepínání na jiné akce
	UnicodeString editovany_text;//uchovává editovanou hodnotu kót
	int index_kurzoru;
	bool posun_dalsich_elementu;//indikuje zda je poadován posun dalších elementù
	bool mazani;
	bool zobrazeni_tabulek;
	double Poffset;
	bool refresh_mGrid;//nevykresluje se z buffru ale pøímo
	bool nabuffrovano;//udruje, zda je buffer mgridovıch rastrù aktuální
	bool duvod_ulozit_nahled;//uchovává v sobì dùvod k uloení náhledu
	short duvod_validovat;//uchovává v sobì podnìt na prùbìh validaèního algoritmu 0-ne, 1-po dokoneèení operace validovat, 2 - validovat nyní
	bool auto_settings_open;
	int ID_tabulky;
	TPointD puv_souradnice;//slouí k ukládání pùvodních souøadnic, napø. pøi posunu elementu
	short zobrazit_pozice;
	short rotace_jigu;
	short zobrazit_popisky;
	short zobrazit_koleje;
	short zobrazit_palce;
	short zobrazit_rozmisteni_voziku;
	bool zamek_layoutu;
  int Top_backup;  //pomocne promenne pro pozici zprav
  int Left_backup; //pomocne promenne pro pozici zprav
  int zpravy_backup_width;
	int zpravy_backup_height;
	short smazat;
	double predchozi_orientace;//uchováváa rotaci objektu pøed posunem

	//metody
	void kurzor(TKurzory typ_kurzor);
	void vypni_editaci();
	void NP();//volá form na nastevení parametrù, døívìjší nastavparametry1click
	void NPin();//podpùrná metoda NP(), øeší vstupní èást dat, vyseparováno, z dùvodu toho, e z GAPO aktulizauji pøípadnì spuštìné PO a nemohu volat NP, protoe to v sobì obsahu ShowModal - vedlo k chybì
  void NP_input(); // volá zobrazení PO - nahrazuje NP a NPin
	void ZOOM_IN();//pøiblíení
	void ZOOM_OUT();//oddálení
	void REFRESH();
	void REFRESH(bool refreshovat_mGridy);
	void DuvodUlozit(bool stav);
	void nahled_ulozit(bool duvod_ulozit);
	void SB(UnicodeString Text, unsigned short Pane=4);//domnívám se, e zde má bıt hodnota 5
	void S(UnicodeString Text="");//usnadòuje pøístup k ShowMessage
	void Sk(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnadòuje pøístup k ShowMessage - MaKr
	void Sv(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnadòuje pøístup k ShowMessage - MaVl
	void Sr(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnadòuje pøístup k ShowMessage - Rostovi
	void Z(UnicodeString Text="",bool add=false,TColor color=clRed);//usnadòuje pøístup ke zprávám, pokud jsou jen prázdné uvozovky (a druhı paremetry na false - co je implicitnì), vymae zpravu, parametr add rozhoduje, zda bude novı text pøeden k pøedešlému textu èi nikoliv, pokud zpráva obsahuje nìjakı text, je zobrazena ikona zprávy, poslední parametr je barva ikony zprávy
	int MB(long left,long top,UnicodeString text,UnicodeString caption_text="",int mbTYPE=MB_OK,bool centrovat_text=true,bool checkbox_zobrazit=false,int width=366,bool default_button_caption=true,bool copy_zobrazit=false);
	int MB(UnicodeString text,int mbTYPE=MB_OK,bool centrovat_text=true,int width=366,bool default_button_caption=true,bool blurForm1=true,bool copy_zobrazit=false);//pokud je blurForm1 na true - Form1 v dobì zobrazení MB rozmlí/udìlá bluer efekt
	void writeINI(AnsiString Section,AnsiString Ident,AnsiString Value);//zajišuje zápis do INI aplikace
	AnsiString readINI(AnsiString Section,AnsiString Ident);//zajišuje ètení z INI aplikace
	void kopirovat_objekt();//pokud je oznaèenı objekt, zajistí jeho zkopírování, pøipoèítá index 1,2,3
	void RM();//korekce chyby oskakování pravého menu
	void aktualizace_maro_a_roma();//aktualizace a pøepoèet hodnot volaná kvùli èasovım osám (maro) a techn.procesùm(roma)
	void deaktivace_zamerovace();//deaktivuje zamìøovaè label a svislice a kolmice
	void aktualizace_combobox_pohony_v_PO(short RDunitD=-1,short RDunitT=-1);//zaktualizuje ve formuláøi parametry objektù combobox na vıpis pohonù vèetnì jednotek uvedeného rozmezí rychlostí, pokud jsou zanechané implicitní parametry short RDunitD=-1,short RDunitT=-1, je naèteno nastevní jednotek z INI aplikace pro form parametry objektu, v pøípadech, kdy uvedené parametry nejsou dané hodnotou -1, tak se uvaují jednotky dle S==0,MIN==1 pro RDunitT, resp. M==0,MM==1 pro RDunitD
	void tab_pohon_COMBO (int index);//0=naètení pohonù do COMBA, 1=pøiøazení pohonu kabinì
	void tab_knihovna_click(double X,double Y,long Col=-1,long Row=-1);//klik do knihovny objektù, spouštìní akce pøidávání objektu
	void aktualizace_ComboPohon ();
	short RO; short ROs; short ROst;short ROsts;short Rotace_symbolu_minula;
	TRect vrat_max_oblast(Cvektory::TObjekt *Objekt=NULL);//vrací max a min hodnoty x a y souøadnic, všecho v layout(elementù, objektù), nebo parametrem Objekt lze hledat max souøadnice v jednom objektu
	double inLO  (double inLO);
	double outLO (double outLO);
  double inPT  (double inPT);
	double outPT (double outPT);
  double inDO (double outDO);
	double outDO (double outDO);
	double inDK (double outDK);
	double outDK (double outDK);
	double inaRD(double inaRD);
	double outaRD(double outaRD);
	double inR(double inR);
	double outR(double outR);
	double inRz(double inRz);
	double outRz(double outRz);
	void Memo(AnsiString Text,bool clear=false,bool count=false);//urychlení vypsání do Mema
	void pridani_elementu_tab_pohon(Cvektory::TElement *E);//pøedesign tabulky pohonu po pøidání elementu
	void log(AnsiString Text,AnsiString Text2="");//zapíše log do textového souboru a pøidá datum
	TRect souradnice_LO(Cvektory::TElement *E);//vrací souøadnice (PX) lakovacího okna elementu pokud nìjaké má,pokud ne vrátí souøadnice elementu
	short prekryti_LO(Cvektory::TElement *E);//prozkoumá zda se element nepøekrıva lak. oknem se sousedními,  0=nepøkrıvá se, 1=pøekrıvá se LO, 2=pøekrıvá se zóna
	double vzdalenost_meziLO(Cvektory::TElement *E,double orientace);//vrati delku v metrech mezi LO elementù
	void design_element(Cvektory::TElement *E,bool prvni_spusteni,bool plnit_comba=true);//nadesignuje tabulky daného elementu
	TPoint bod_vlozeni_elementu(double kontr_x=-1000,double kontr_y=-1000);//vrací bod vloení elementu, "pøilepuje" kurzor na geometrii pokud se jedná o pøímku, parametry kontr_x a y slouí ke kontrole bodu zda se nachází na pøímce (pøi posunu)
	bool bod_na_geometrii(double X, double Y,Cvektory::TElement *Element=NULL);//kontroluje zde se bod nachází na geometri, vrací pouze ano/ne, pokud je do metody poslán ukazatel na element provìøí zda se tento element nachází na geometrii
	double max_voziku(Cvektory::TElement *stopka);//vrátí maximální monı poèet vozíkù na stopce, podle geometrie pøed ní
	void prirazeni_pohonu_tab_pohon(unsigned int index_pohonu);//pøedesignuje tabulku pohonu po pøidání elementu, nebo pohonu
	void aktualizace_RT();//projde všechny elementy v aktuálnì editovaném objektu a upravím jim RT
	void posun_na_element(unsigned long n_zpravy);//podle zprávy provede posun na danı elment
	void smaz_kurzor();
	UnicodeString get_temp_dir();
	void vytvoreni_tab_pohon();//vytvoøení tabulky pohonu
	void napln_comba_mGridu(Cvektory::TElement *E);
	void vlozit_predavaci_misto();//projde elementy a objekty, pokud je nìkde nutnost vloit pøedávací místo vloí ho tam
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
extern PACKAGE TForm1 *F;//pouze zkrácenı zapis
extern AnsiString Parametry; //pøíjímá parametry programu, dùleité pro otevírání programu konrkétním souborem
//---------------------------------------------------------------------------
#endif