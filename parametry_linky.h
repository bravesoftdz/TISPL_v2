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
//---------------------------------------------------------------------------
class TForm_parametry_linky : public TForm
{
__published:	// IDE-managed Components
	TrHTMLLabel *rHTMLLabel_delka;
	TValueListEditor *ValueListEditor;
	TButton *Button_ADD;
	TButton *Button_DEL;
	TrStringGridEd *rStringGridEd_tab_dopravniky;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPGlyphButton *scGPGlyphButton15;
	TscGPButton *scGPButton_vozik;
	TrEditNum *rEditNum_delkavoziku;
	TscGPButton *scGPButton_pohon;
	TButton *Vypis_pohony;
	TscRadioGroup *scRadioGroup_typVoziku;
	TrHTMLLabel *rHTMLLabel_eDesigner;
	TrEditNum *rEditNum_takt;
	TrHTMLLabel *rHTMLLabel_takt;
	TrHTMLLabel *rHTMLLabel_sirka;
	TrEditNum *rEditNum_sirkavoziku;
	TscGPButton *scGPButton_obecne;
	TrHTMLLabel *rHTMLLabel_doporuc_pohony;
	TscExPanel *scExPanel_doporuc_pohony;
	TscHTMLLabel *scHTMLLabel_doporuc_pohony;
	TscGPButton *scGPButton_doporucene;
	TscGPGlyphButton *scGPGlyphButton_add_mezi_pohony;
	TscLabel *scLabel_TT_change_hint;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall Button_saveClick(TObject *Sender);
	void __fastcall Button_ADDClick(TObject *Sender);
	void __fastcall Button_DELClick(TObject *Sender);
	void __fastcall Vypis_pohonyClick(TObject *Sender);
	void __fastcall scGPButton_doporuceneClick(TObject *Sender);
	void __fastcall scExPanel_doporuc_pohonyClose(TObject *Sender);
	void __fastcall rEditNum_taktChange(TObject *Sender);
	void __fastcall rHTMLLabel_delkaClick(TObject *Sender);
	void __fastcall rHTMLLabel_sirkaClick(TObject *Sender);
	void __fastcall rHTMLLabel_taktClick(TObject *Sender);
	void __fastcall scGPGlyphButton_add_mezi_pohonyClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations

	enum Tinput_state{NO,NOTHING,DV,SV,TT,RZ};//uchovává výbìr input hodnoty (aby se formuláøe necyklyly)

public:		// User declarations
	__fastcall TForm_parametry_linky(TComponent* Owner);

	bool data_nalezena;
	void  nacti_pohony();
	Tinput_state input_state;//stav vstupu CT,RD,DD,K
	enum Tm_mm{M=0,MM};Tm_mm Delkaunit;Tm_mm Sirkaunit;//pøepínaè jednotek vzdálenost
	enum Tminsec{S=0,MIN};Tminsec Taktunit;//pøepínaè jednotek èasu

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
#endif
