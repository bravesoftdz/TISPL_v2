//---------------------------------------------------------------------------
#ifndef parametry_linkyH
#define parametry_linkyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//#include "rHTMLLabel.hpp"
//#include "rImprovedComps.hpp"
//#include "rStringGridEd.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include "scExtControls.hpp"
#include "scHtmlControls.hpp"
//#include "pl_priority.h"
#include "scModernControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
//#include "PL_math.h"
//#include "rHintWindow.hpp"
#include "scGPImages.hpp"
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "scStyledForm.hpp"
#include "scImageCollection.hpp"
#include "scGPMeters.hpp"
#include <Vcl.ComCtrls.hpp>
#include "scHint.hpp"
#include <sstream>
#include <iomanip>
#include "TmGrid.h"
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
	TscGPButton *scGPButton_obecne;
	TscGPSwitch *scGPSwitch;
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
	TscGPNumericEdit *scGPNumericEdit_delka_podvozek;
	TscGPNumericEdit *scGPNumericEdit_vyska_jig;
  TscGPGlyphButton *scGPGlyphButton_ADD;
  TscGPNumericEdit *rEditNum_takt;
  TscPanel *scPanel_takt;
  TscPanel *scPanel_vozik;
  TscImage *scImage1;
  TscGPGearActivityIndicator *scGPGlyphButton_OPTIONS;
  TscHTMLLabel *scHTMLLabel_jig_info;
  TscGPGlyphButton *scGPGlyphButton_katalog;
  TscStyledForm *scStyledForm2;
  TscGPTrackBar *scGPTrackBar_uchyceni;
  TscHTMLLabel *scHTMLLabel1;
	TscHTMLLabel *scHTMLLabel_posuvnik;
  TscComboBox *scComboBox_vyber_produkt;
  TscCheckBox *scCheckBox_vyber_produkt;
  TscHTMLLabel *scHTMLLabel_sirka_jig;
  TscHTMLLabel *scHTMLLabel_delka_podvozek;
  TscHTMLLabel *scHTMLLabel_vyska_jig;
  TscHTMLLabel *scHTMLLabel_delka_jig;
  TscHTMLLabel *scHTMLLabel_takt;
  TscHint *scHint1;
  TscLabel *scHTMLLabel_podlahovy;
  TscLabel *scHTMLLabel_podvesny;
  TscHTMLLabel *scHTMLLabel_InfoText;
  TImage *ImageEx_jig_podvesny;
  TImage *ImageEx_jig_podlahovy;
  TscEdit *scEdit_platle;
  TscHTMLLabel *scHTMLLabel_platle;
  TscEdit *scEdit_uchyt;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall Button_saveClick(TObject *Sender);
	void __fastcall Button_ADD_Click(TObject *Sender);
	void __fastcall Vypis_pohonyClick(TObject *Sender);
	void __fastcall rEditNum_takt_Change(TObject *Sender);
	void __fastcall rHTMLLabel_delkavozikuClick(TObject *Sender);
	void __fastcall scHTMLLabel_sirka_jigClick(TObject *Sender);
	void __fastcall rHTMLLabel_takt2Click(TObject *Sender);
	void __fastcall scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_delkavozikuClick(TObject *Sender);
	void __fastcall scHTMLLabel_delka_jigClick(TObject *Sender);
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
	void __fastcall GlyphButton_closeClick(TObject *Sender);
	void __fastcall scLabel_kopirovatClick(TObject *Sender);
	void __fastcall scLabel_smazatClick(TObject *Sender);
	void __fastcall scLabel_smazatMouseLeave(TObject *Sender);
	void __fastcall scLabel_smazatMouseEnter(TObject *Sender);
	void __fastcall scLabel_kopirovatMouseEnter(TObject *Sender);
	void __fastcall scLabel_kopirovatMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_kopirovatMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_kopirovatMouseLeave(TObject *Sender);
	void __fastcall scHTMLLabel_doporuc_pohonyClick(TObject *Sender);
	void __fastcall scGPSwitchChangeState(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall rEditNum_taktClick(TObject *Sender);
	void __fastcall rHTMLLabel_InfoTextClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall scGPGlyphButton_katalogClick(TObject *Sender);
  void __fastcall scGPComboEdit12Click(TObject *Sender);
  void __fastcall scGPComboEdit12DropDown(TObject *Sender);
  void __fastcall scGPTrackBar_uchyceniMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall scGPNumericEdit_delka_podvozekChange(TObject *Sender);
  void __fastcall scComboBox_vyber_produktChange(TObject *Sender);




private:	// User declarations
 //	TPL_math pm;//INSTANCE NA V�PO�ETN� ��ST PL tj. PL_math
	enum Tinput_state{NO,NOTHING,LOADING,JOB,DV,SV,TT,RZ,RX,aRD,R,jednotky_prevod,R_prevod,Rz_prevod,aRD_prevod};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
	enum Tinput_clicked_edit {empty_klik,TT_klik,DV_klik,SV_klik,V_klik,Podvoz_klik,aRD_klik,R_klik,Rz_klik,Rx_klik,nazev_klik,od_klik,do_klik}; //zjisteni na ktery edit nebo bunku ve sloupci bylo kliknuto
	enum Tinput_clicked_icon {empty_klik_ico,aRD_klik_ico,R_klik_ico,Rz_klik_ico,Rx_klik_ico}; //zjisteni na kterou ikonku z�mku bylo kliknuto
	enum Tinput_onchange {NOChange,aRDChange,RChange,RzChange,RxChange}; //zjisteni na kterou ikonku z�mku bylo kliknuto

	void pasiveColor();//nastav� v�echny polo�ky pop-up na pasivn� resp. default barvu
	void top_positon(int top);//hl�d�n� horn� pozice, je-li dan� komponenta horn� kv�li nastaven� k��ku
	TColor clBg,clAcBg,clAcBg2,clGlyph,clAcGlyph;//barvy polo�ek pop-up menu
	unsigned int getPID(int ROW);//vr�t� pohon ID na dan�m ��dku
	unsigned int getMaxPID();//najde max pou�it� ID pohonu (proto�e ID nejsou se�azena,nelze vz�t index posledn�ho ��dku)
	bool *zrusena_prirazeni_PID;
	unsigned int zrusena_prirazeni_PID_size;
	void zrusit_prirazeni_smazanych_ci_odrazenych_pohunu_k_objektum();
	void pozice_scGPGlyphButton_hint();
	bool existuji_nepouzivane_pohony();//testuje zda existuj� nepou��t� pohony, pokud ano,vrac� true jinak false
	void nacti_pohony();
	void Nastav_zamky(Tinput_clicked_icon I,Tinput_clicked_edit E);
	TColor clBACKGROUND;
	void nastav_edity ();//metoda vym�n� edit pro d�lku podvozku a d�lku jigu podle typu voz�ku podlahov�/podv�sn�
	void vozik_zmena_jednotek();

	TmGrid *PL_mGrid;//mGrid pro PL

public:		// User declarations
	__fastcall TForm_parametry_linky(TComponent* Owner);
	int getROW(int PID);//vr�t� ��slo ��dku dle pohon ID, pokud nenajde vr�t� -1
	void vypis(UnicodeString text,bool red=true,bool link=false);
	void input_TT();
	void INPUT(double Sloupec, double Radek);
	void OUTPUT(double i,double Sloupec, double Radek);
	void Roletka_roztec(int Row);
	void show_min_Rz();
	double getTT();
	void VALIDACE(int ACol, int ARow);
  int COL,ROW;
  void vykresli_obdelnik_vpravo();

  	//metody volan� z Tmgrid
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
	int katalog_id;
	double radius;
	bool data_nalezena;
	Tinput_state input_state;//stav vstupu DV,SV,TT...atd
	Tinput_clicked_edit input_clicked_edit;//zjisteni na ktery edit bylo kliknuto
	Tinput_clicked_icon input_clicked_icon;//zjisteni na ktery icon bylo kliknuto
	Tinput_onchange   onchange;
	enum Tm_mm{M=0,MM};Tm_mm Delkaunit;Tm_mm Sirkaunit;Tm_mm Runit;Tm_mm Rzunit;Tm_mm Dmunit;//p�ep�na� jednotek vzd�lenost
	enum Tminsec{S=0,MIN};Tminsec Taktunit;Tminsec RDunit;Tminsec minsec;Tminsec aRDunit;//p�ep�na� jednotek �asu
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
  void getROtherValues(Tm_mm Runit,int Row);

	int roletka_data;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
#endif
