//---------------------------------------------------------------------------

#ifndef parametry_vozikH
#define parametry_vozikH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include "scModernControls.hpp"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_parametry_vozik : public TForm
{
__published:	// IDE-managed Components
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPButton *scGPButton_vozik;
	TrHTMLLabel *rHTMLLabel_delka_jig;
	TrHTMLLabel *rHTMLLabel_JIG;
	TscGPNumericEdit *scGPNumericEdit_delka_jig;
	TscGPNumericEdit *scGPNumericEdit_sirka_jig;
	TrHTMLLabel *rHTMLLabel_sirka_jig;
	TrHTMLLabel *rHTMLLabel_vyska_jig;
	TscGPNumericEdit *scGPNumericEdit_vyska_jig;
	TscGPNumericEdit *scGPNumericEdit_delka_podvozek;
	TrHTMLLabel *rHTMLLabel_delka_podvozek;
	TrHTMLLabel *rHTMLLabel_podvozek_zaves;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPGlyphButton *scGPGlyphButton_info;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_parametry_vozik(TComponent* Owner);
	void NastavDesign();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_vozik *Form_parametry_vozik;
//---------------------------------------------------------------------------
#endif
