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
//---------------------------------------------------------------------------
class TForm_parametry_linky : public TForm
{
__published:	// IDE-managed Components
	TrHTMLLabel *rHTMLLabel_delka_voziku;
	TrStringGridEd *rStringGridEd_tab_dopravniky;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_vozik;
	TrEditNum *rEditNum_delkavoziku;
	TscGPButton *scGPButton_pohon;
	TButton *Vypis_pohony;
	TrHTMLLabel *rHTMLLabel_eDesigner;
	TrEditNum *rEditNum_takt_OLD;
	TrHTMLLabel *rHTMLLabel_takt;
	TrHTMLLabel *rHTMLLabel_sirka;
	TrEditNum *rEditNum_sirka_jigu_OLD;
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
	TrHTMLLabel *rHTMLLabel_delka_jigu;
	TrEditNum *rEditNum_delka_jigu_old;
	TscGPSwitch *scGPSwitch;
	TrHTMLLabel *rHTMLLabel3;
	TrHTMLLabel *rHTMLLabel4;
	TrHTMLLabel *rHTMLLabel1;
	TrHTMLLabel *rHTMLLabel2;
	TscGPGlyphButton *Button_ADD;
	TscGPGlyphButton *Button_DEL;
	TrStringGridEd *rStringGridEd_hlavicka_tabulky;
	TscGPNumericEdit *rEditNum_takt;
	TscGPGlyphButton *scGPGlyphButton_hint_Rz;
	TscGPNumericEdit *rEditNum_delka_jigu;
	TscGPNumericEdit *rEditNum_sirka_jigu;
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
	void __fastcall rHTMLLabel_sirkaClick(TObject *Sender);
	void __fastcall rHTMLLabel_taktClick(TObject *Sender);
	void __fastcall scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall rStringGridEd_tab_dopravnikyGetEditStyle(TObject *Sender, int Col,
          int Row, TrStringGridEdEditStyle &EditStyle);
	void __fastcall rStringGridEd_tab_dopravnikyCanEdit(TObject *Sender, int Col, int Row,
          bool &CanEdit);
	void __fastcall rEditNum_delkavozikuClick(TObject *Sender);
	void __fastcall rHTMLLabel_delka_jiguClick(TObject *Sender);
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
	void __fastcall rStringGridEd_tab_dopravnikyKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_sirka_jiguChange(TObject *Sender);
	void __fastcall rEditNum_delka_jiguChange(TObject *Sender);
	void __fastcall scGPGlyphButton_infoClick(TObject *Sender);








private:	// User declarations

	enum Tinput_state{NO,NOTHING,DV,SV,TT,RZ};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)

public:		// User declarations
	__fastcall TForm_parametry_linky(TComponent* Owner);

	bool data_nalezena;
	void  nacti_pohony();
	void show_min_Rz();
	Tinput_state input_state;//stav vstupu CT,RD,DD,K
	enum Tm_mm{M=0,MM};Tm_mm Delkaunit;Tm_mm Sirkaunit;//p�ep�na� jednotek vzd�lenost
	enum Tminsec{S=0,MIN};Tminsec Taktunit;//p�ep�na� jednotek �asu
	bool Changes;  //obecna zmena = zmena PP ci TT
	bool Changes_TT;    // konkretni zmena TT
	bool Changes_PP;   // konkretni zmena PP
	bool Ulozit;
	bool zobrazitFrameForm;

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
#endif
