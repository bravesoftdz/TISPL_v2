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
#include "scHint.hpp"
#include "scGPExtControls.hpp"
#include "rHintWindow.hpp"
#include "UnitX.h"
//#include "vektory.h" //uû vkl·d·m p¯es vykresli.h
//#include "knihovna_objektu.h" //uû vkl·d·m p¯es vykresli.h resp. vektory.h

//podmÌnÏn˝ p¯eklad, zda se jedn· ËÌ nejedn· o debug
#ifdef _DEBUG
bool DEBUG=true;
#else
bool DEBUG=false;
#endif
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TRzToolbar *RzToolbar1;
	TRzStatusPane *RzStatusPane1;
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
	TscGPGlyphButton *scGPGlyphButton15;
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
	TscExPanel *scExPanel_ostatni;
	TscExPanel *scExPanel_vrstvy;
	TscLabel *scLabel4;
	TscLabel *scLabel_titulek;
	TButton *hl_spojak_zakazky;
	TMemo *Memo2;
	TButton *button_zakazky_temp;
	TscButton *scButton_novy;
	TButton *Button11;
	TscGPGlyphButton *scGPGlyphButton_close_grafy;
	TscButton *scButton_ulozit;
	TscExPanel *scExPanel_log_header;
	TscHTMLLabel *scHTMLLabel_log_vypis;
	TscGPGlyphButton *scGPGlyphButton_zpravy_ikona;
	TscGPSwitch *scGPSwitch_rezim;
	TscLabel *scLabel_klient;
	TscLabel *scLabel_architekt;
	TscExPanel *scExPanel_html;
	TWebBrowser *WebBrowser1;
	TButton *pohonobjekt;
	TButton *Button12;
	TscGPCheckBox *scGPCheckBox_ortogon;
	TscGPGlyphButton *scGPGlyphButton_close_legenda_casove_osy;
	TButton *Button13;
	TscGPGlyphButton *scGPButton_generuj;
	TscLabel *scLabel_filtrovat;
	TscLabel *scLabel_doba_cekani;
	TscGPCheckBox *CheckBoxAnimovatSG;
	TscGPCheckBox *CheckBox_pouzit_zadane_kapacity;
	TscGPGlyphButton *ButtonPLAY;
	TscLabel *scLabel_mezera;
	TscGPButton *scGPButton_header_projekt;
	TscGPButton *scGPButton_header_def_zakazek;
	TscButton *scButton_parmlinky_defzakazek;
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
  TscGPButton *scGPButton_OK;
  TscGPButton *scGPButton_storno;
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
  TscGPGearActivityIndicator *scGPGlyphButton_OPTIONS;
  TscExPanel *scExPanel_podklad;
	TTimer *Timer1;
	TMemo *Memo3;
  TTimer *Timer2;
	TscGPPanel *scGPPanel_bottomtoolbar;
	TscGPCheckBox *scGPCheckBox_viditelnost;
	TscGPPanel *scGPPanel_schema;
	TscGPGlyphButton *scGPGlyphButton1;
	TscGPGlyphButton *scGPGlyphButton3;
	TscGPGlyphButton *scGPGlyphButton7;
	TscGPLabel *scGPLabel1;
	TscGPComboBox *scGPComboBox_orientace;
	TscGPButton *scGPButton_ulozit;
	TscGPButton *scGPButton_zahodit;
	TscGPLabel *scGPLabel2;
	TscButton *scButton_zamek;
	TscLabel *scLabel1;
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
	void __fastcall scExPanel_log_headerClose(TObject *Sender);
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
	void __fastcall scSplitView_MENUOpened(TObject *Sender);
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
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
  void __fastcall scGPEdit1Change(TObject *Sender);
  void __fastcall Timer2Timer(TObject *Sender);


// User declarations
	////struktury, v˝Ëty
public:
	enum Tmod{NO=0,SCHEMA,LAYOUT,CASOVAOSA,TECHNOPROCESY,SIMULACE,NAHLED};Tmod MOD;
	enum Tstatus{NAVRH,OVEROVANI};Tstatus STATUS;
	enum Takce{NIC=0,PAN,PAN_MOVE,ZOOM_W,ZOOM_W_MENU,ADD,MOVE,VYH,MEASURE,KALIBRACE,ADJUSTACE,MOVE_ELEMENT,MOVE_TABLE};Takce Akce;
  enum Tm_mm{M=0,MM};Tm_mm DOtocunit;//p¯epÌnaË jednotek vzd·lenost
	enum Tminsec{SEC=0,MIN};Tminsec PTunit;Tminsec LOunit;//p¯epÌnaË jednotek Ëasu
	Cvektory::TObjekt *pom,*pom_vyhybka,*pom_temp,*copyObjekt;
	Cvektory::TElement *pom_element,*pom_element_smazat;
	TPointD copyObjektRzRx;
	TPO_math pm;//INSTANCE NA V›PO»ETNÕ »¡ST PO tj. PO_math
	UnicodeString get_user_name();
  DrawGridWndProc(TMessage &Message);

private:
	enum Tedice{DEVELOPER,ARCHITECT,CLIENT,VIEWER,DEMO};Tedice EDICE;
	enum TKurzory {standard=0,posun_v,posun_b,posun_p,posun_l,posun_t,kalibrovat,pan,pan_move,window,add_o,neco,posun_ind,zmena_j};
	struct Tnastaveni{bool autosave;unsigned short int minut;bool posledni_file;};Tnastaveni nastaveni;

  TWndMethod PreviousWndProc;

	////instance
	Graphics::TBitmap *Pan_bmp;
	Cvektory::TProces *proces_pom;

	////metody
	void aut_pozicovani(Cvektory::TElement *E, int X, int Y);
	void edice();
	void ESC();
	void UP();void DOWN();void RIGHT();void LEFT();void Uloz_predchozi_pohled();//realizujÌ posuny obrazu
	void ZOOM();//samotn˝ ZOOM
	void ZOOM_WINDOW();//p¯iblÌûenÌ oknem
	void on_change_zoom_change_scGPTrackBar();
	void zneplatnit_minulesouradnice();
	void kurzor(TKurzory typ_kurzor);
	void pan_map(TCanvas * canv, int X, int Y);
	void pan_move_map();
	void add_objekt(int X, int Y);
	Cvektory::TObjekt *add_objekt_za();
	void move_objekt(int X, int Y);
	void add_element(int X, int Y);
	short rotace_symbol(short trend,int X, int Y);
	void design_element(Cvektory::TElement *E);//nadesignuje tabulky danÈho elementu
	void zmen_poradi_objektu(int X, int Y);//testuje zda se nejedn· o zmÏnu po¯adÌ (to musÌ jeötÏ uûivatel potvrdit)
	void zobraz_tip(UnicodeString text);
	void akutalizace_stavu_prichytavani_vSB();
	void Novy_soubor();//samotnÈ vytvo¯enÌ novÈho souboru
	void Ulozit_soubor();//samotnÈ uloûenÌ
	void Otevrit_soubor();//realizuje otev¯enÌ opendialogu s n·sledn˝m vol·nÌm realizace samotnÈho otev¯enÌ souboru
  void Nacist_podklad();//realizuje otev¯enÌ opendialogu s n·sledn˝m vol·nÌm realizace samotnÈho nacteni podkladu
	unsigned short int Otevrit_soubor(UnicodeString soubor);//realizuje samotnÈ otev¯enÌ souboru
  unsigned short int Nacist_podklad(UnicodeString soubor);//realizuje nacteni podkladu
	void ulozit_posledni_otevreny();//uloûÌ do ini nazev poslednÌho pracovnÌho souboru
	void vse_odstranit();
	UnicodeString get_computer_name();
	int get_DPI();//MV
	void redesign_element();
	UnicodeString get_temp_dir();
	AnsiString FileName_short(AnsiString FileName);
	void nacist_nastaveni();
	void ulozit_nastaveni();
	void zavrit_uvod();//zav¯e ˙vodnÌ dialog
	int vrat_max_vysku_grafu();
	void getJobID_OnClick(int X, int Y);//vr·tÌ do glob·lnÌ promÏnnÈ JID ID ˙lohy/funkcionality v mÌstÏ kliku, -1 û·dn·, 0 - 9 rezervov·no pro element, 10 - 99 - interaktivnÌ text kÛty, 100- a v˝öe rezervov·no pro tabuku, kde 100 znamen· nult˝ ¯·dek, z·roveÚ pokud bylo kliknuto na tabulku Ëi element nahraje ukazatel do glob·lnÌ promÏnnÈ pom_element
	void nastaveni_grafickeho_vystupu(Graphics::TBitmap * Bitmap,unsigned int OD,unsigned int PO);
	bool ttr(UnicodeString Text);
	void log2web(UnicodeString Text);//automaticky p¯id· parametry (Ëas, uûivatel, licence)
	void log2webOnlyText(UnicodeString Text);//pouze text
	void startUP();//p¯i aktivaci formul·¯e, pro zp¯ehlednÏnÌ kodu
	void DesignSettings();//nastavenÌ designu v konstruktoru
	void aktualizace();//kontrola aktu·lnosti verze a p¯ÌpadÏ nabÌdka na staûenÌ novÈ
	void onPopUP(int X, int Y);//nastavenÌ zobrazenÌ popUPmenu a jeho vol·nÌ vËetnÏ pozice
	void close_all_items_popUPmenu();//zajistÌ skr˝tÌ vöech poloûek popUPmenu
	void ortogonalizace_on_off();//zapÌn· Ëi vypÌn· automatickou ortogonalizaci
	void ortogonalizace();//vol· ortogonalizaci schÈma, pokud je ortogonalizace povolena
	void ortogonalizovat();//ortogonalizuje schÈma
	void db_connection();  // p¯ipojenÌ k DB serveru
	void akt_tabulek (Cvektory::TElement *E,AnsiString LO,AnsiString delka_otoce,AnsiString cas,short sirka,short sirka1,short sirka_o,short sirka_o1);

	////promÏnnÈ
	TDateTime TIME;
	UnicodeString LICENCE;
	short n_prihlaseni;
	bool ortogonalizace_stav;
  bool kalibrace_hotova;

private:
	bool pan_non_locked;
	bool stisknute_leve_tlacitko_mysi;//uchov·v· stav levÈho tlaËÌtka myöi
	unsigned short int funkcni_klavesa;//uchov·v· stav poslednÌ stisknutÈ funkËnÌ kl·vesy
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
	bool refresh_mGrid;

	bool duvod_k_ulozeni;
	bool stisknuto_storno;
	bool volat_parametry_linky;//pouûito p¯i soubor nov˝
	bool start_ortogonalizace;

	AnsiString Caption;

	short pocitadlo_doby_neaktivity;

	bool FMaximized;
	TRect FOldBoundsRect;


public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

	TPoint akt_souradnice_kurzoru_PX;//uchov· aktu·lnÌ pozici kurzoru
	TPointD akt_souradnice_kurzoru;//uchov· aktu·lnÌ pozici kurzoru v logick˝ch jednotk·ch, resp. sou¯adnicÌch
	TPoint vychozi_souradnice_kurzoru;//uchov· v˝chozÌ pozici kurzoru
	TPoint predchozi_souradnice_kurzoru;//uchov· p˘vodnÌ pozici kurzoru p¯i stisku tlaËÌtka myöi
	TPoint minule_souradnice_kurzoru;//uchov· p˘vodnÌ sou¯adnice p¯i posunu

	UnicodeString VERZE;
	double m2px;//uchov·v· hodnotu prostorovÈho rozliöenÌ programu, nativnÌ rozliöenÌ 0,1 m na 1 pixel p¯i zoomu 1x
	double fps;//frames per second, Ëetnost snÌmk˘ za sekundu - pouûÌv· se pro animace a simulace
	double afps;//frames per second, aktu·lnÌ Ëetnost snÌmk˘ za sekundu - pouûÌv· se pro animace a simulace
	TMyString ms;
	Cmy m;
	Cvykresli d;
	Cgrafy g;
	UnicodeString FileName;
	double Zoom; //promÏnn· uchov·vajicÌ velikost Zoomu
	TPointD Posun;//promÏnnÈ uchov·vajicÌ velikost posunu obrazu (pro scrollov·nÌ atp.), je to ve fyzick˝ch sou¯adnicÌch za¯ÌzenÌ
	bool grid;
	int size_grid;
	short prichytavat_k_mrizce;
	bool zobrazit_barvy_casovych_rezerv;
	bool antialiasing;
	double Zoom_predchozi_AA;//p¯i antialiasingu
	bool nahled_objektu;//uchov·v· stav, zda se jedn· o n·hled objekt Ëi regulernÌ zobrazenÌ ve form1
	bool TZF;//TRIAL_zakazat_funkcionality
	bool dblClick;
	int vybrany_objekt;
	short VyID;//objekt-symbol vyh˝bky - ID typu
	int JID;//JOB ID
  int knihovna_id; // id drawgrid knihovny
	int element_id;  // id vybraneho elementu z knihoven

	void NP();//vol· form na nastevenÌ parametr˘, d¯ÌvÏjöÌ nastavparametry1click
	void NPin();//podp˘rn· metoda NP(), ¯eöÌ vstupnÌ Ë·st dat, vyseparov·no, z d˘vodu toho, ûe z GAPO aktulizauji p¯ÌpadnÏ spuötÏnÈ PO a nemohu volat NP, protoûe to v sobÏ obsahu ShowModal - vedlo k chybÏ
  void NP_input(); // vol· zobrazenÌ PO - nahrazuje NP a NPin
	void ZOOM_IN();//p¯iblÌûenÌ
	void ZOOM_OUT();//odd·lenÌ
	void REFRESH(bool mGrid=true); //vybere buÔ Invalidate nebo FormPaint(this) dle if(!antialiasing a dle Invalidate=true), tedy kdyû bude zapnut˝ antialising jde vûdy do vÏtve else
	void DuvodUlozit(bool stav);
	void SB(UnicodeString Text, unsigned short Pane=4);//domnÌv·m se, ûe zde m· b˝t hodnota 5
	void S(UnicodeString Text="");//usnadÚuje p¯Ìstup k ShowMessage
	void Sk(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnadÚuje p¯Ìstup k ShowMessage - MaKr
	void Sv(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnadÚuje p¯Ìstup k ShowMessage - MaVl
	void Z(UnicodeString Text="",bool add=false,TColor color=clRed);//usnadÚuje p¯Ìstup ke zpr·v·m, pokud jsou jen pr·zdnÈ uvozovky (a druh˝ paremetry na false - coû je implicitnÏ), vymaûe zpravu, parametr add rozhoduje, zda bude nov˝ text p¯eden k p¯edeölÈmu textu Ëi nikoliv, pokud zpr·va obsahuje nÏjak˝ text, je zobrazena ikona zpr·vy, poslednÌ parametr je barva ikony zpr·vy
	int MB(long left,long top,UnicodeString text,UnicodeString caption_text="",int mbTYPE=MB_OK,bool centrovat_text=true,bool checkbox_zobrazit=false,int width=366,bool default_button_caption=true);
	int MB(UnicodeString text,int mbTYPE=MB_OK,bool centrovat_text=true,int width=366,bool default_button_caption=true);
	void writeINI(AnsiString Section,AnsiString Ident,AnsiString Value);//zajiöùuje z·pis do INI aplikace
	AnsiString readINI(AnsiString Section,AnsiString Ident);//zajiöùuje ËtenÌ z INI aplikace
	void kopirovat_objekt();//pokud je oznaËen˝ objekt, zajistÌ jeho zkopÌrov·nÌ, p¯ipoËÌt· index 1,2,3
	void RM();//korekce chyby oskakov·nÌ pravÈho menu
	void aktualizace_maro_a_roma();//aktualizace a p¯epoËet hodnot volan· kv˘li Ëasov˝m os·m (maro) a techn.proces˘m(roma)
	void deaktivace_zamerovace();//deaktivuje zamÏ¯ovaË label a svislice a kolmice
	void aktualizace_combobox_pohony_v_PO(short RDunitD=-1,short RDunitT=-1);//zaktualizuje ve formul·¯i parametry objekt˘ combobox na v˝pis pohon˘ vËetnÏ jednotek uvedenÈho rozmezÌ rychlostÌ, pokud jsou zanechanÈ implicitnÌ parametry short RDunitD=-1,short RDunitT=-1, je naËteno nastevnÌ jednotek z INI aplikace pro form parametry objektu, v p¯Ìpadech, kdy uvedenÈ parametry nejsou danÈ hodnotou -1, tak se uvaûujÌ jednotky dle S==0,MIN==1 pro RDunitT, resp. M==0,MM==1 pro RDunitD
  short RO; short ROs; short ROst;short ROsts;short Rotace_symbolu_minula;
	double inLO  (double inLO);
  double outLO (double outLO);
  double inPT  (double inPT);
	double outPT (double outPT);
  double inDO (double outDO);
	double outDO (double outDO);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
extern PACKAGE TForm1 *F;//pouze zkr·cen˝ zapis
extern AnsiString Parametry; //p¯ÌjÌm· parametry programu, d˘leûitÈ pro otevÌr·nÌ programu konrkÈtnÌm souborem
//---------------------------------------------------------------------------
#endif
