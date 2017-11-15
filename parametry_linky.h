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
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall Button_saveClick(TObject *Sender);
	void __fastcall Button_ADDClick(TObject *Sender);
	void __fastcall Button_DELClick(TObject *Sender);
	void __fastcall Vypis_pohonyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_parametry_linky(TComponent* Owner);

	bool data_nalezena;

	void  nacti_pohony();

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
#endif
