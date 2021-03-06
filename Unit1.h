#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//#include "RzPanel.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
//#include "RzSplit.hpp"
//#include "RzStatus.hpp"
#include <Vcl.Grids.hpp>
//#include "RzButton.hpp"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
//#include "RzBorder.hpp"
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
//#include "RzGrids.hpp"
//#include <VCLTee.GanttCh.hpp>
//#include <VCLTee.Chart.hpp>
//#include <VCLTee.Series.hpp>
//#include <VclTee.TeeGDIPlus.hpp>
//#include <VCLTee.TeEngine.hpp>
//#include <VCLTee.TeeProcs.hpp>
//#include "RzLabel.hpp"
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
#include <IdSNMP.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdFTP.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//#include "rImprovedComps.hpp"
//#include "rStringGridEd.hpp"
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
#include <gdiplus.h>

#include "MyString.h"
#include "my.h"
#include "vykresli.h"
#include "grafy.h"
#include "PO_math.h"
#include "scGPExtControls.hpp"
//#include "rHintWindow.hpp"
#include "UnitX.h"
#include "scImageCollection.hpp"
//#include "rHintWindow.hpp"

#include "Tvlakno_obraz.h"
#include "Tvlakno_panCreate.h"
//#include "RzAnimtr.hpp"

//#include "rImprovedComps.hpp"
//#include "rImprovedComps.hpp"
//#include "rImprovedComps.hpp"
//#include <IdSMTPBase.hpp>
//#include <IdMessage.hpp>
//#include <System.Actions.hpp>
//#include <Vcl.ActnList.hpp>
//#include <Vcl.ExtActns.hpp>
//#include "RzSndMsg.hpp"
//#include "knihovna_objektu.h" //u� vkl�d�m p�es vykresli.h resp. vektory.h

//podm�n�n� p�eklad, zda se jedn� �� nejedn� o debug
#ifdef _DEBUG
const bool DEBUG=true;
#else
const bool DEBUG=false;
#endif
//---------------------------------------------------------------------------
void CALLBACK TimerSimulaceEvent(UINT wTimerID,UINT msg,DWORD dwUser,DWORD dw1,DWORD dw2);
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog1;
	TTimer *Timer_backup;
	TSavePictureDialog *SavePictureDialog1;
	TButton *Button2;
	TTimer *Timer_neaktivity;
	TButton *ButtonPLAY_O;
	TTimer *Timer_simulace;
	TTimer *TimerMouseWheel;
	TIdHTTP *IdHTTP1;
	TTimer *Timer_tr;
	TFDConnection *FDConnection1;
	TFDQuery *FDQuery1;
	TButton *SQL_processID;
	TFDQuery *FDQuery2;
	TscGPPanel *scGPPanel_mainmenu;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPButton *Schema;
	TscGPButton *PopupMenuButton;
	TscGPButton *Analyza;
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
	TscGPButton *Layout;
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
	TscGPLabel *pravyoption_nadpis;
	TscExPanel *scExPanel_vrstvy;
	TscLabel *scLabel_titulek;
	TscButton *scButton_novy;
	TButton *ButtonRosta;
	TscButton *scButton_ulozit;
	TscExPanel *scExPanel_html;
	TWebBrowser *WebBrowser1;
	TButton *ButtonMaVl;
	TscLabel *scLabel_mezera;
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
	TMemo *Memo_testy;
	TTimer *TimerSmazEditNazvuElementuVTabulce;
	TscGPPanel *scGPPanel_bottomtoolbar;
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
	TButton *ButtonMaKr;
  TscGPButton *scGPButton_mereni_vzdalenost;
  TscExPanel *scExPanel_ostatni;
	TscGPCheckBox *scGPCheckBox_antialiasing;
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
	TscGPButton *scGPButton_geometrie;
	TscGPCheckBox *scGPCheckBox_rozmisteni_voziku;
	TscGPButton *scGPGlyphButton_odstran_cestu;
	TPopupMenu *PopupMenu1;
	TMenuItem *N11;
	TMenuItem *N21;
  TPopupMenu *PopupMenu_posledni_projekty;
  TMenuItem *N1projekt1;
  TMenuItem *N2projekt1;
  TMenuItem *N3projekt1;
  TscGPGlyphButton *scGPGlyphButton_undo;
  TscGPGlyphButton *scGPGlyphButton_redo;
  TscGPButton *scGPButton_bug_report;
	TTimer *Timer_getjobid;
	TscGPCheckBox *scGPCheckBox_popisek_pohonu;
	TscLabel *scLabel_statusbar_1;
	TscLabel *scLabel_statusbar_0;
	TscLabel *scLabel_statusbar_2;
	TImage *Image_rozdelovac_0;
	TImage *Image_rozdelovac_1;
	TImage *Image_rozdelovac_2;
	TImage *Image_rozdelovac_3;
	TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
  TIdSNTP *IdSNTP1;
	TscGPCheckBox *scGPCheckBox_meridlo_casy;
	TscGPGlyphButton *scGPGlyphButton_PLAYminus;
	TscGPGlyphButton *scGPGlyphButton_PLAYplus;
	TscGPLabel *scGPLabel_zamerovac;
	TscEdit *Edit_proFocus;
	void __fastcall Konec1Click(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall DrawGrid_knihovnaDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall schemaClick(TObject *Sender);
	void __fastcall DrawGrid_knihovnaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
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
	void __fastcall RzStatusPane1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Vypicestuktempu1Click(TObject *Sender);
	void __fastcall AnalyzaClick(TObject *Sender);
	void __fastcall FormShortCut(TWMKey &Msg, bool &Handled);
	void __fastcall CheckBoxPALCE_Click(TObject *Sender);
	void __fastcall Chart1Click(TObject *Sender);
	void __fastcall Zobrazitparametry1Click(TObject *Sender);
	void __fastcall Rychlexport1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Timer_neaktivityTimer(TObject *Sender);
	void __fastcall Timer_simulaceTimer(TObject *Sender);
	void __fastcall ButtonMaVlClick(TObject *Sender);
	void __fastcall CheckBoxVymena_barev_Click(TObject *Sender);
	void __fastcall TimerMouseWheelTimer(TObject *Sender);
	void __fastcall Timer_trTimer(TObject *Sender);
	void __fastcall SQL_processIDClick(TObject *Sender);
	void __fastcall MaxButtonClick(TObject *Sender);
	void __fastcall scGPGlyphButton_OPTIONS_OldClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPGlyphButton_ZOOM_MINUSClick(TObject *Sender);
	void __fastcall scGPGlyphButton_ZOOM_PLUSClick(TObject *Sender);
	void __fastcall scLabel_titulekDblClick(TObject *Sender);
	void __fastcall MinButtonClick(TObject *Sender);
	void __fastcall Button_dopravnik_parametryClick(TObject *Sender);
	void __fastcall DetailsButtonClick(TObject *Sender);
	void __fastcall scGPTrackBar1Change(TObject *Sender);
	void __fastcall scGPSwitch_AAChangeState(TObject *Sender);
	void __fastcall scGPSwitch_gridChangeState(TObject *Sender);
	void __fastcall scGPSwitch_rezimChangeState(TObject *Sender);
	void __fastcall scSplitViewsClosing(TObject *Sender);
	void __fastcall scSplitViewsClosed(TObject *Sender);
	void __fastcall scExPanel_vrstvyClick(TObject *Sender);
	void __fastcall scGPSwitch_meritkoChangeState(TObject *Sender);
	void __fastcall scSplitView_OPTIONSMouseLeave(TObject *Sender);
	void __fastcall scSplitView_MENUMouseLeave(TObject *Sender);
	void __fastcall scButton2Click(TObject *Sender);
	void __fastcall scGPGlyphButton_definice_zakazekClick(TObject *Sender);
	void __fastcall ComboBoxCekaniChange(TObject *Sender);
	void __fastcall scGPGlyphButton_zpravy_ikonaClick(TObject *Sender);
	void __fastcall scSplitView_OPTIONSOpened(TObject *Sender);
	void __fastcall LayoutClick(TObject *Sender);
	void __fastcall scSplitView_OPTIONSPanelPaint(TCanvas *ACanvas, TRect &ARect);
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
  void __fastcall ButtonRostaClick(TObject *Sender);
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
  void __fastcall Button_testClick(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
  void __fastcall scGPEdit1Change(TObject *Sender);
  void __fastcall TimerSmazEditNazvuElementuVTabulceTimer(TObject *Sender);
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
  void __fastcall scGPButton_posun_halyClick(TObject *Sender);
  void __fastcall scGPButton_nakreslit_haluClick(TObject *Sender);
	void __fastcall NahledClick(TObject *Sender);
	void __fastcall scGPImage_zamek_posunuClick(TObject *Sender);
	void __fastcall scButton_zamek_layoutuClick(TObject *Sender);
  void __fastcall scGPTrackBar_intenzitaChange(TObject *Sender);
  void __fastcall scGPCheckBox_zobrazit_rotace_jigu_na_otocichClick(TObject *Sender);
  void __fastcall scGPCheckBox_zobrazit_poziceClick(TObject *Sender);
	void __fastcall Memo_testyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall scGPCheckBox1_popiskyClick(TObject *Sender);
  void __fastcall scGPCheckBox_zobrazit_kolejeClick(TObject *Sender);
  void __fastcall scGPSwitch1ChangeState(TObject *Sender);
	void __fastcall scGPCheckBox_zobrazit_palceClick(TObject *Sender);
	void __fastcall scGPButton_prichytavatClick(TObject *Sender);
  void __fastcall scGPButton_errorClick(TObject *Sender);
  void __fastcall scGPButton_warningClick(TObject *Sender);
	void __fastcall scGPButton_smazatClick(TObject *Sender);
	void __fastcall scGPButton_geometrieClick(TObject *Sender);
	void __fastcall scGPCheckBox_rozmisteni_vozikuClick(TObject *Sender);
	void __fastcall scGPGlyphButton_odstran_cestuClick(TObject *Sender);
	void __fastcall N21Click(TObject *Sender);
	void __fastcall N1projekt1Click(TObject *Sender);
	void __fastcall N2projekt1Click(TObject *Sender);
	void __fastcall N3projekt1Click(TObject *Sender);
  void __fastcall scExPanel1Click(TObject *Sender);
  void __fastcall scGPGlyphButton_undoClick(TObject *Sender);
  void __fastcall scGPGlyphButton_redoClick(TObject *Sender);
  void __fastcall scGPButton_bug_reportClick(TObject *Sender);
	void __fastcall Timer_getjobidTimer(TObject *Sender);
	void __fastcall scGPCheckBox_popisek_pohonuClick(TObject *Sender);
	void __fastcall scLabel_statusbar_0Click(TObject *Sender);
	void __fastcall scExPanel_ostatniClick(TObject *Sender);
	void __fastcall ButtonMaKrClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall scGPCheckBox_meridlo_casyClick(TObject *Sender);
	void __fastcall scGPGlyphButton_PLAYplusClick(TObject *Sender);
	void __fastcall scGPGlyphButton_PLAYminusClick(TObject *Sender);
	void __fastcall scGPCheckBox_antialiasingClick(TObject *Sender);





// User declarations
	////jen public struktury a v��ty
public:
	enum Tmod{NO=0,LAYOUT,CASOVAOSA,TECHNOPROCESY,SIMULACE,EDITACE,TVORBA_CESTY};Tmod MOD;
	enum Tstatus{NAVRH,OVEROVANI};Tstatus STATUS;
	enum Takce{NIC=0,PAN,PAN_MOVE,ZOOM_W,ZOOM_W_MENU,ADD,MOVE,VYH,MEASURE,KALIBRACE,ADJUSTACE,MOVE_ELEMENT,MOVE_TABLE,OFFSET_KOTY,MOVE_KOMORA,ROZMER_KOMORA,DRAW_HALA,MOVE_HALA,MOVE_BOD,MOVE_USECKA,MOVE_TEXT,GEOMETRIE,BLOK,GEOMETRIE_LIGHT,MAGNETICKE_LASO,POSUN_TEPLOMER,S_K,EDITACE_TEXTU};Takce Akce;Takce Akce_temp;//akce temp slou�� ke spu�t�n� akce p�i akci, p�. Akce=GEOMETRIE a p�i n� je pot�eba p�esunout k�ty geo. element�, tud� Akce_temp=OFFSET_KOTY
	enum Tm_mm{M=0,MM,SEKUNDY,MINUTY};Tm_mm DOtocunit,DKunit,LOunit,Runit,Rzunit;//p�ep�na� jednotek vzd�lenost,roz���en o SEKUNDY,MINUTY (probl�m p�i pou�it� SEC a MIN) z d�vodu �asov�ch a vzd�lenostn�ch k�t
	enum Tminsec{SEC=0,MIN};Tminsec PTunit,aRDunit ;//p�ep�na� jednotek �asu
	enum TKurzory {standard=0,posun_v,posun_b,posun_p,posun_l,posun_t,kalibrovat,pan,pan_move,window,add_o,neco,posun_ind,zmena_j,edit_text,zmena_d_x,zmena_d_y,posun_ind_ld,posun_ind_pd,editace_posun,info,close,posun_editace_obj,editace_obj};
	////instance
  TStringList *ls;
	Graphics::TBitmap *Staticka_scena;//bitmapa statick� sc�ny
private:
	enum Tedice{DEVELOPER,ARCHITECT,CLIENT,VIEWER,DEMO};Tedice EDICE;
	struct Tnastaveni{bool autosave;unsigned short int minut;bool posledni_file;};Tnastaveni nastaveni;
	enum Tlanguage{EN,MN,CS};Tlanguage language;
	////ukazatele
	Cvektory::TProces *proces_pom;
	TFileStream *LogFileStream;
	Graphics::TBitmap *Pan_bmp_ALL;//kv�li mGridu jinak sta�� private
	vlakno_panCreate *vlakno_PanCreate;

	////metody
	friend void CALLBACK TimerSimulaceEvent(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);//provede jeden takt multimedi�ln�ho TimeruSimulace
	void TimerKill(UINT wTimerID);//ukon�� dan� (dlew TimerID) timer
	void aut_pozicovani(Cvektory::TElement *E, int X, int Y);
	void edice();
	void ESC();
	void UP();void DOWN();void RIGHT();void LEFT();void Uloz_predchozi_pohled();//realizuj� posuny obrazu
	void ZOOM();//samotn� ZOOM
	void ZOOM_WINDOW();//p�ibl�en� oknem
	void on_change_zoom_change_scGPTrackBar();
	void zneplatnit_minulesouradnice();
	void pan_create();//vytvo�� v��ez pro pan_move
	void pan_map(TCanvas * canv, int X, int Y);
	void pan_move_map();
	Cvektory::TObjekt *add_objekt(int X, int Y);
	Cvektory::TObjekt *add_objekt_za();
	void move_objekt(double X, double Y,Cvektory::TObjekt *Objekt);
	void add_element(int X, int Y);
	void add_vyhybka_spojka();
	void add_komoru();//p�id�v�n� komory kabin� powerwashe, kontrola zda nen� sou�et kabin v�t�� ne� rozm�r kabiny
	short rotace_symbol(short trend,int X_bod,int Y_bod);//dle toho, zda je umis�ovan� element nad osou �i pod osou pohonu je vr�cena rotace symbolu, X_bod,.. je bbod vkl�d�n� elementu (jedna sou�adnice ho v�e na pohon)
	void vytvoreni_tab_knihovna();//vytovo�en� tabulky knihovny objekt�
	void popisky_knihovna_nahled(bool knihovna);//p�ep�n� popisky mezi knihovnou a editac�
	void zmena_jednotek_tab_pohon();
	void prvni_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce,AnsiString rychlost,AnsiString R,AnsiString Rz);
	void dalsi_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce,AnsiString rychlost,AnsiString R,AnsiString Rz);
	void zmen_poradi_objektu(int X, int Y);//testuje zda se nejedn� o zm�nu po�ad� (to mus� je�t� u�ivatel potvrdit)
	void akutalizace_stavu_prichytavani_vSB();
	void Novy_soubor(bool invalidate=true);//samotn� vytvo�en� nov�ho souboru
	void Ulozit_soubor();//samotn� ulo�en�
	void Otevrit_soubor();//realizuje otev�en� opendialogu s n�sledn�m vol�n�m realizace samotn�ho otev�en� souboru
	void Nacist_podklad();//realizuje otev�en� opendialogu s n�sledn�m vol�n�m realizace samotn�ho nacteni podkladu
	unsigned short int Otevrit_soubor(UnicodeString soubor);//realizuje samotn� otev�en� souboru
	unsigned short int Nacist_podklad(UnicodeString soubor,bool akt_filename=false);//realizuje nacteni podkladu
	void ulozit_posledni_otevreny();//ulo�� do ini nazev posledn�ho pracovn�ho souboru
	void ulozit_historie_otevrenych();//ukl�d�n� 3 naposledy otev�en�ch projekt� do historie
	void vse_odstranit();//odstran� ukazatele
public:
	UnicodeString get_computer_name();
	UnicodeString get_user_name();
	UnicodeString get_Windows_dir();
	DWORDLONG get_TotalPhysMemory();//vr�t� celkovou pam� na dan�m za��zen�
	DWORDLONG get_AvailPhysMemory();//vr�t� aktu�ln� dostupnou pam� na dan�m za��zen�
private:
	int get_DPI();
	void redesign_element();
	AnsiString FileName_short(AnsiString FileName);
	void nacist_nastaveni();
	void ulozit_nastaveni();
	void zavrit_uvod();//zav�e �vodn� dialog
	bool najdi_nazev_obj(double X,double Y,Cvektory::TObjekt *Objekt);//vrat� zda jsem v oblasti textu
  short najdi_popisky_PM(double X,double Y,Cvektory::TObjekt *Objekt);//najde na jak�m popisku u PM se nach�z� kurzor, vrac� 0: nenalezeno, 1: pohon 1, 2: pohon 2
	void getJobID(int X, int Y);//vr�t� do glob�ln� prom�nn� JID ID �lohy/funkcionality v m�st� kurzoru, z�rove� pokud v m�st� tabulky �i elementu nahraje ukazatel do glob�ln� prom�nn� pom_element, v�znam jednotliv�ch JID hodnot v koment��i definici metody
	void setJobIDOnMouseMove(int X, int Y);//dle m�sta kurzoru a vr�cen� JID (job id) nastav� �lohu
	void nastaveni_grafickeho_vystupu(Graphics::TBitmap * Bitmap,unsigned int OD,unsigned int PO);
	bool ttr(UnicodeString Text);
	void log2web(UnicodeString Text);//automaticky p�id� parametry (�as, u�ivatel, licence)
	void log2webOnlyText(UnicodeString Text);//pouze text
	void SaveText2File(AnsiString Text,AnsiString FileName);//zap�e dan� textov� �et�zec do dan�ho textov�ho souboru
	void startUP();//p�i aktivaci formul��e, pro zp�ehledn�n� kodu
	void DesignSettings();//nastaven� designu v konstruktoru
	void aktualizace();//kontrola aktu�lnosti verze a p��pad� nab�dka na sta�en� nov�
	void onPopUP(int X, int Y);//nastaven� zobrazen� popUPmenu a jeho vol�n� v�etn� pozice
	void close_all_items_popUPmenu();//zajist� skr�t� v�ech polo�ek popUPmenu
	void ortogonalizace_on_off();//zap�n� �i vyp�n� automatickou ortogonalizaci
	void ortogonalizovat();//ortogonalizuje sch�ma
	void db_connection();  // p�ipojen� k DB serveru
	void akt_tabulek (Cvektory::TElement *E,AnsiString LO,AnsiString delka_otoce,AnsiString cas,AnsiString rychlost,AnsiString R,AnsiString Rz,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla);
	void vykresli_kurzor(int index);
	void zmenJednotekKot();
	int pocet_vyskytu_elementu_s_otoci(Cvektory::TObjekt *Objekt);//prohled� elementy v objektu, vr�t� 0 pokud je rotace v objektu v�ude stejn�, vr�t� 1 pokud je p��tomno v�ce rotac�
	void vykresli_spojinici_EmGrid(TCanvas *Canv,Cvektory::TElement *E);//vykresl� spojnici mezi tabulkou a elementem z nejbli���ho rohu tabulky
	void nacti_podklad(TCanvas *Canv);
	unsigned short load_language(Tlanguage language,bool akt_mGrid=false);
	void change_languagein_mGrid();//aktualizace popisk� v tabulk�ch p�i zm�n� jazyka
	void zmena_editovaneho_objektu();//slou�� k p�echodu z editace jednoho objektu do editace druh�ho objektu
	TPointD uprav_bod_vlozeni_elementu(TPointD bod_vlozeni,short rotace_symbolu,int eID=-1);//uprav� bod kurzoru pro vlo�en� elemntu na bod vykreslen� elementu (robot na konci ramena)
	void vlozeni_editace_geometrie();//vkl�d� novou geometrii nebo edituje ji� st�vaj�c� geometrii
	void smaz_usek_geometrie();//vyma�e aktu�ln� usek geometrie
	void ukonceni_geometrie(bool kontorla=true);//ukon�en� akce geometrie a p��padn� uzav�en� kruhu
	void vloz_bod_haly_objektu(int X,int Y);//vlo�� bod haly nebo objektu na zvolen� sou�adnice (fyzick�), zohledn� p�ichyt�v�n�
	void smaz_bod_haly_objektu(Cvektory::TBod *bod);//sma�e bod haly nebo objektu, pokud existuj� u� jen 2 posledn� body sma�e oba
  void mGrid_mimo_obraz(Cvektory::TElement *E=NULL);//kontrola zde je mGrid mimo obraz, pokud ano vypnut� komponent
  TRect oblast_prekryti_mGridu(Cvektory::TElement *E=NULL);//vr�t� oblast pro kontrolu p�ekryt� mGrid�
	void mGrid_puvodni_stav(Cvektory::TElement *E=NULL);//nadesingnuje tabulky element� nebo tabulku pohonu na p�vodn� stav, obnov� komponenty, napln� comba, provede Update() mGridu
	void set_font(int velikost=14);//nastav� komponent�m aFont
	bool pripnuti_dalsich_objektu(double citlivost=1.5);//pokud p�i ulo�en� editovan�ho objektu je detekov�no, �e konec objketu nenavazuje na za��tek n�sleduj�s�ho objektu je polo�en dotaz a po potvrzen� dojde ke spojen�
	void spojeni_prvni_posledni(double citlivost=1.5);//kontrola zda na sebe prvn� a poledn� objekt navazuj�, pokud jsou bl�zko u sebe, ale nenavazuj� - nav�e je
	bool napojeni_vedlejsi_vetve(Cvektory::TElement *e_posledni,bool kontrola_vzdalenosti=true);//provede kontrolu, zd� je mo�nost geometrii spojit, dot�e se a spoj� geometrii pokud u�ivatel souhlas�
  void vypni_geometrii();//vypne akci geometrie
	void Otevri_posledni_ulozeny(UnicodeString soubor);//otev�e jeden z posledn�ch otev�en�ch soubor�
	void vytvor_obraz(bool stornoUNDO=false);//slou�� k vytvo�en� obrazu pro storno + undo nebo jen undo
	void vymaz_seznam_obrazu();//vyma�e v�echny obrazy v po�adn�ku
	void vytvor_statickou_scenu();//vytvo�� BMP se statickou sc�nou
	void vytvor_statickou_scenu(long SCENA);//p�et�en� v��e uveden� p��mo s parametrem nastaven� sc�ny
	void design_statusbar();//provde zarovn�n� zar�ek a zm�n� velikost v�pis� ve statusbaru
	void vytvorScenar();//testov�c� metody simulace
	void provedScenar();//testov�c� metody simulace
	void PlaySound(short A);//p�ehr�n� zvuku simulace na samostatn�m vl�kn�
	Cvektory::TElement *gS(short n);//testov�c� metody simulace
	double wt();//testov�c� metody simulace
	unsigned int getV();//testov�c� metody simulace
	void vytvor_seznam_pouzivanych_pohonu();//vytvo�� seznam, kam se zap�� pohony, kter� jsou pou��v�ny jinde
	void smaz_seznam_pouzivanych_pohonu();//sma�e seznam pou��van�ch pohon�
	void byly_pohony_editovany();//provede kontrolu jak� pohony byly editov�ny
	void nastav_combo_mGridu(TscGPComboBox *C);//nastav� barvy a chov�n� pro combo
  void napoj_vetev_na_geo();

	////prom�nn�
	ULONG_PTR gdiplusToken;
	//UINT TimerSimulaceID;
	//UINT TimerPresnost;
	TDateTime TIME;
	short n_prihlaseni;
	bool logovat;
	bool ortogonalizace_stav;
	bool kalibrace_hotova;
	bool pan_non_locked;
	short panType;
	TPointD bmpPANall_XY;//ulo�en� sou�adnic lev�ho horn�ho rohu
	bool stisknute_leve_tlacitko_mysi;//uchov�v� stav lev�ho tla��tka my�i
	unsigned short int funkcni_klavesa;//uchov�v� stav posledn� stisknut� funk�n� kl�vesy
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
	bool volat_parametry_linky;//pou�ito p�i soubor nov�
	bool start_ortogonalizace;
	bool stav_kurzoru;//kurzon vykreslen/nevykreslen
	AnsiString nazev_puvodni;// pou��v�no pro uchov�v�n� p�vodn�ho n�zvu objektu z d�vodu zru�en� editace
	AnsiString Caption;
	short pocitadlo_doby_neaktivity;
	TPoint pocitadlo_zmeny_pozice;
	bool FMaximized;
	TRect FOldBoundsRect;
	bool PmGCheckLink;
	bool StopCheckLink;
	bool offset_spolus_rozmerem;//uchov�v� v sob�, zda m� b�t p�i zm�n� rozmer� kabiny zm�n� i offset k�t element�
	UnicodeString Jazyk;
	int count_memo;//counter pro memo
	bool editace_geometrie_spustena;
	int offset_scGPButton_bug_report;
	bool storno;//slou�� k rozli�en� jestli bylo stisknuto storno nebo byl zavol�n jeho stisk z tla��tka ulo�it
	bool refreshovat_scGPTrackBar;//prom�nn� zaji��uj�ci, �e se scGPTracBar nerefreshuje pokud mu p�i zm�n� zoomu mimo scGPTracBar
	long vychozi_stav_sceny;
	TDateTime start;
	short typElementu;//slou�� k rozli�en� vykreslen� u smart_kurzoru
  unsigned long *pouzivane_pohony;//ukl�d� n pohon�, kter� jsou pou��van�

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	//instance
	TMyString ms;
	Cmy m;
	Cvykresli d;
	//Cgrafy g;
	TPO_math pm;//INSTANCE NA V�PO�ETN� ��ST PO tj. PO_math
	//uklazatele
	Graphics::TBitmap *Pan_bmp;//kv�li mGridu jinak sta�� private
	Cvektory::TObjekt *pom,*pom_vyhybka,*OBJEKT_akt,*copyObjekt;
	Cvektory::TElement *pom_element,*pom_element_temp,*posledni_editovany_element,*element_temp,*predchozi_PM;//element_temp je nulov�n p�i ka�d�m p�ejet� kurzoru pou��v�n na v�ci kolem PM
	TmGrid *PmG,*mGrid_knihovna;//ukazatel na mGridovou tabulku pohonu
	unsigned int PmG_VID;//valida�n� ID, uchov�v� ID validace v mGridu, nap�. dopRD
  double PmG_VID_value;//uchov�v� v sob� hodnotu pro validaci, nap�. hodnotu dopRD
	Cvektory::TKomora *pom_komora,*pom_komora_temp;
	Cvektory::TBod *pom_bod,*pom_bod_temp;

	//sou�adnicov� prom�nn�
	TPoint akt_souradnice_kurzoru_PX;//uchov� aktu�ln� pozici kurzoru
	TPointD akt_souradnice_kurzoru;//uchov� aktu�ln� pozici kurzoru v logick�ch jednotk�ch, resp. sou�adnic�ch
	TPoint vychozi_souradnice_kurzoru;//uchov� v�choz� pozici kurzoru
	TPoint predchozi_souradnice_kurzoru;//uchov� p�vodn� pozici kurzoru p�i stisku tla��tka my�i
	TPoint minule_souradnice_kurzoru;//uchov� p�vodn� sou�adnice p�i posunu

	//prom�nn�
	UnicodeString LICENCE;
	UnicodeString FileVersion;
  UnicodeString ProductVersion;
	double m2px;//uchov�v� hodnotu prostorov�ho rozli�en� programu, nativn� rozli�en� 0,1 m na 1 pixel p�i zoomu 1x
	double fps;//frames per second, �etnost sn�mk� za sekundu - pou��v� se pro animace a simulace
	double afps;//frames per second, aktu�ln� �etnost sn�mk� za sekundu - pou��v� se pro animace a simulace
	UnicodeString FileName;
	TFont *aFont;//aktu�ln� nastaven� v�choz� font
	double Zoom; //prom�nn� uchov�vajic� velikost Zoomu
	TPointD Posun;//prom�nn� uchov�vajic� velikost posunu obrazu (pro scrollov�n� atp.), je to ve fyzick�ch sou�adnic�ch za��zen�
	TPointD copyObjektRzRx;
	bool grid;
	int size_grid;
	short prichytavat_k_mrizce;
	bool zobrazit_barvy_casovych_rezerv;
	bool zobrazit_meritko;
	bool antialiasing;
	double Zoom_predchozi_AA;//p�i antialiasingu
	bool nahled_objektu;//uchov�v� stav, zda se jedn� o n�hled objekt �i regulern� zobrazen� ve form1
	bool TZF;//TRIAL_zakazat_funkcionality
	bool dblClick;
	int vybrany_objekt;
	short VyID;//objekt-symbol vyh�bky - ID typu
	int JID;//JOB ID
  int knihovna_id; // id drawgrid knihovny
	int element_id;  // id vybraneho elementu z knihoven
	AnsiString TIP;//uchov�v� text TIPu
	UnicodeString editovany_text;//uchov�v� editovanou hodnotu k�t
	int index_kurzoru;
	bool posun_dalsich_elementu;//indikuje zda je po�adov�n posun dal��ch element�
	bool mazani;
	bool zobrazeni_tabulek;
	double sTIME;//simula�n� �as
	bool refresh_mGrid;//nevykresluje se z buffru ale p��mo
	bool nabuffrovano;//udr�uje, zda je buffer mgridov�ch rastr� aktu�ln�
	bool duvod_ulozit_nahled;//uchov�v� v sob� d�vod k ulo�en� n�hledu
	short duvod_validovat;//uchov�v� v sob� podn�t na pr�b�h valida�n�ho algoritmu 0-ne, 1-po dokone�en� operace validovat, 2 - validovat nyn�
	bool auto_settings_open;
	int ID_tabulky;
	TPointD puv_souradnice;//slou�� k ukl�d�n� p�vodn�ch sou�adnic, nap�. p�i posunu elementu
	short zobrazit_pozice;
	short rotace_jigu;
	short zobrazit_popisky;
	short zobrazit_koleje;
	short zobrazit_palce;
	short zobrazit_rozmisteni_voziku;
	short zobrazit_popisek_pohonu;
	bool zamek_layoutu;
  int Top_backup;  //pomocne promenne pro pozici zprav
  int Left_backup; //pomocne promenne pro pozici zprav
  int zpravy_backup_width;
	int zpravy_backup_height;
	double predchozi_orientace;//uchov�v� rotaci objektu p�ed posunem
	unsigned int vlakno_akce;//uchov�v� akci, kterou m� vl�kno vykonat, 0 - nic, 1 - vytvo�en� obrazu pro UNDO, 2 - vytvo�en� obrazu pro storno a UNDO, 3 - vymaz�n� obraz�
	double velikost_citelne_oblasti_elementu;//ur�uje velikost citeln� oblsati [m] kolem bodu elementu na pohonu
	Tvlakno_obraz *vlakno_obraz;
	//bool vlakno_PanCreateState;
	bool zobrazit_upozorneni_teplomery;
	double CAS;
	bool CASOVEOSY;
	TPointD pocatek_mereni;//ukl�d� .x - X sou�adnici za��tku m��en� a .y - Voz�k -> n
	TPointD konec_mereni;//ukl�d� .x - X sou�adnici za��tku m��en� a .y - Voz�k -> n
	String data_teplomeru;//slou�� k uchov�n� dat do bu��k, vypln�n� dat a� na konci
  unsigned int radku_teplomeru;//uchov�v� v sob� po�et ��dk� teplom�ru

	//metody
	void kurzor(TKurzory typ_kurzor);
	void vypni_editaci();
	void otevri_editaci(); // vol� zobrazen� PO - nahrazuje NP a NPin
	void ZOOM_IN();//p�ibl�en�
	void ZOOM_OUT();//odd�len�
	void REFRESH();
	void REFRESH(bool refreshovat_mGridy);
	void REFRESH(long ZprVozEledElesDopObjHal,bool refreshovat_mGridy);
	void DuvodUlozit(bool stav);
	void nahled_ulozit(bool duvod_ulozit,bool duvod_validovat=true);
	void SB(UnicodeString Text, unsigned short Pane=4);//domn�v�m se, �e zde m� b�t hodnota 5
	void S(UnicodeString Text="");//usnad�uje p��stup k ShowMessage
	void Sk(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnad�uje p��stup k ShowMessage - MaKr
	void Sv(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnad�uje p��stup k ShowMessage - MaVl
	void Sr(UnicodeString Text="",AnsiString umisteni="neuvedeno");//usnad�uje p��stup k ShowMessage - Rostovi
	void Z(UnicodeString Text="",bool add=false,TColor color=clRed);//usnad�uje p��stup ke zpr�v�m, pokud jsou jen pr�zdn� uvozovky (a druh� paremetry na false - co� je implicitn�), vyma�e zpravu, parametr add rozhoduje, zda bude nov� text p�eden k p�ede�l�mu textu �i nikoliv, pokud zpr�va obsahuje n�jak� text, je zobrazena ikona zpr�vy, posledn� parametr je barva ikony zpr�vy
	int MB(long left,long top,UnicodeString text,UnicodeString caption_text="",int mbTYPE=MB_OK,bool centrovat_text=true,bool checkbox_zobrazit=false,int width=366,bool default_button_caption=true,bool copy_zobrazit=false);
	int MB(UnicodeString text,int mbTYPE=MB_OK,bool centrovat_text=true,int width=366,bool default_button_caption=true,bool blurForm1=true,bool copy_zobrazit=false);//pokud je blurForm1 na true - Form1 v dob� zobrazen� MB rozml��/ud�l� bluer efekt
	void writeINI(AnsiString Section,AnsiString Ident,AnsiString Value);//zaji��uje z�pis do INI aplikace
	AnsiString readINI(AnsiString Section,AnsiString Ident);//zaji��uje �ten� z INI aplikace
	void kopirovat_objekt();//pokud je ozna�en� objekt, zajist� jeho zkop�rov�n�, p�ipo��t� index 1,2,3
	void RM();//korekce chyby oskakov�n� prav�ho menu
	void aktualizace_maro_a_roma();//aktualizace a p�epo�et hodnot volan� kv�li �asov�m os�m (maro) a techn.proces�m(roma)
	void aktualizace_combobox_pohony_v_PO(short RDunitD=-1,short RDunitT=-1);//zaktualizuje ve formul��i parametry objekt� combobox na v�pis pohon� v�etn� jednotek uveden�ho rozmez� rychlost�, pokud jsou zanechan� implicitn� parametry short RDunitD=-1,short RDunitT=-1, je na�teno nastevn� jednotek z INI aplikace pro form parametry objektu, v p��padech, kdy uveden� parametry nejsou dan� hodnotou -1, tak se uva�uj� jednotky dle S==0,MIN==1 pro RDunitT, resp. M==0,MM==1 pro RDunitD
	void tab_pohon_COMBO();//na�ten� pohon� do comba
	void tab_knihovna_click(double X,double Y,long Col=-1,long Row=-1);//klik do knihovny objekt�, spou�t�n� akce p�id�v�n� objektu
	short RO; short ROs; short ROst;short ROsts;short Rotace_symbolu_minula;
	TRect vrat_max_oblast(Cvektory::TObjekt *Objekt=NULL,bool pouze_body=false);//vrac� max a min hodnoty x a y sou�adnic, v�echo v layout(element�, objekt�), nebo parametrem Objekt lze hledat max sou�adnice v jednom objektu
	TRect vrat_max_zobrazitelnou_oblast();//vr�t� max zobrazitelnou (na monitoru) oblast z metody v��e uveden� vrat_max_oblast
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
	void Memo(AnsiString Text,bool clear=false,bool count=false,bool copyFinalTextToClipboard=false);//urychlen� vyps�n� do Memo_testy
	void log(AnsiString Text,AnsiString Text2="");//zap�e log do textov�ho souboru a p�id� datum
	TPointD oblast_LO(Cvektory::TElement *E);//vrac� velikosti oblast� lakovac�ch oken, .x = oblast p�ed, .y = oblast za
	short prekryti_LO(Cvektory::TElement *E);//prozkoum� zda se element nep�ekr�va lak. oknem se sousedn�mi,  0=nep�kr�v� se, 1=p�ekr�v� se LO, 2=p�ekr�v� se z�na
	double vzdalenost_meziLO(Cvektory::TElement *E,double orientace);//vrati delku v metrech mezi LO element�
	void design_element(Cvektory::TElement *E,bool prvni_spusteni,bool plnit_comba=true);//nadesignuje tabulky dan�ho elementu
	TPointD bod_vlozeni_elementu(double kontr_x=-1000,double kontr_y=-1000);//vrac� bod vlo�en� elementu, "p�ilepuje" kurzor na geometrii pokud se jedn� o p��mku, parametry kontr_x a y slou�� ke kontrole bodu zda se nach�z� na p��mce (p�i posunu)
	bool bod_na_geometrii(double X, double Y,Cvektory::TElement *Element=NULL,bool posun_elementu=false);//kontroluje zde se bod nach�z� na geometri, vrac� pouze ano/ne, pokud je do metody posl�n ukazatel na element prov��� zda se tento element nach�z� na geometrii
	double max_voziku(Cvektory::TElement *stopka);//vr�t� maxim�ln� mo�n� po�et voz�k� na stopce, podle geometrie p�ed n�
	void aktualizace_RT();//projde v�echny elementy v aktu�ln� editovan�m objektu a uprav�m jim RT
	void posun_na_element(unsigned long n_zpravy);//podle zpr�vy provede posun na dan� elment
	void posun_na_element(Cvektory::TElement *E);//p�esune se na dann� element
	void smaz_kurzor();
	UnicodeString get_temp_dir();
	void vytvoreni_tab_pohon(bool existuje_poh_tabulka);//vytvo�en� tabulky pohonu
	void aktualizace_tab_pohon(bool popisky=true,bool data=true,bool komponenty=true);//zm�n� re�im tabulce pohonu
	void napln_comba_mGridu(Cvektory::TElement *E);
	bool prohodit_sloupce_PM(Cvektory::TElement *E);//rozhodne zda maj� b�t prohozeny sloupce PM, podle trendu geometrie
	void zmena_editovanych_bunek(Cvektory::TElement *E);//automaticky nastav� editovan� polo�ky a needitovateln� polo�ky pro pohonov� tabulky
	void vlozit_predavaci_misto_aktualizuj_WT();//projde elementy a objekty, pokud je n�kde nutnost vlo�it p�ed�vac� m�sto vlo�� ho tam
	void mGrid_komponenta_na_draw(TmGrid *mGrid,long Col,long Row);//smaz� komponentu v dan� bu�ce a zm�n� typ bunky na DRAW
	bool mail(String Host,String Username,String Password,String FromAddress,String FromName,String Subject,String Body,String To,String ccTo="",String bccTo="",String FileName="",String FileName2="");//ode�le e-mail, doru�itel na v�ech t�ech �rovn� To,ccTo,bccTo mohou b�t mnohon�sobn� zadan�, pouze od�len� ��rkou, t�lo e-mailu lze zadat jako html, n�vratov� hodnota vrac� �sp�nost proveden�
	void ortogonalizace();//vol� ortogonalizaci sch�ma, pokud je ortogonalizace povolena
	void nastav_focus();//vr�t� focus na form1, kdykoli je pot�eba vr�tit focus, skrze odchyt�v�n� kl�ves
	void set_enabled_mGrid(Cvektory::TElement *E);//zapne nebo vypne komponenty megridu v z�vislosti na tom zda m� element p�i�azen� pohon
	void zobraz_tip(UnicodeString text="", TCanvas* canv=NULL);//pr�zdn�m (bez paremetr�) vol�n�m  metody se tip sma�e, //pokud nen� parametr canvas uveden, jedn� se o dlouhodob� vykreslov�n� hodnoty TIP//pokud je parametrem p�ed�n Canvas vykresl� se p��mo a jednor�zov�
	void zapnuti_vypnuti_panelEditace(bool zapnout);//zapnout nebo vypnout panel editace, automaticky podle MODu zobraz� �i skryje ur�it� prvky
	void rotuj_objekt_click(double rotace);//zajist� rotaci objektu
	void pan_create2();//vytvo�� v��ez pro pan_move - velk�
	void vytvor_aktualizuj_tab_teplomeru();//vytvo�� nebo aktualizuje mGrid teplomer� pro OBJEKT_akt
  void pridej_radek_tab_teplomeru(Cvektory::TElement *E,double cas,double WT,bool prejezd,bool celkem=false);
	void START();//zapne stopky
	void STOP(bool MB=false);//vypne stopky, pokud je parementr metody nastaven na false (co� je implicitn�), je zaji�t�n v�pis do mema, pokud na true tak do ShowMessage
	void GetTime(short int rezim);
	void reset_teplomeru(Cvektory::TObjekt *Objekt);//zboraz� upozorn�n�, �e do�lo ke zm�n� geometrie a resetuje oblasti teplomer�
	String get_major_version(String version);//vr�t� Major verzi z FileVersion
	void copy_to_clipboard(String text);//kop�ruje text do Clipboardu
	void mGrid_on_mGrid();//prohled� zda se p�ekr�vaj� mGridy
	bool je_pohon_pouzivan(unsigned long n);//provede kontrolu, zda je pohon v seznamu pou��van�ch, vr�t� v�sledek
	void rozmisti_mGridy();//rovnom�rn� rozm�st� mGridny, po��tek je left top
  void vypocet_WT(Cvektory::TElement *E);//vypo��ta WT pro PM, v�hybku a spojku
  };
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
extern PACKAGE TForm1 *F;//pouze zkr�cen� zapis
extern AnsiString Parametry; //p��j�m� parametry programu, d�le�it� pro otev�r�n� programu konrk�tn�m souborem
//---------------------------------------------------------------------------
#endif
