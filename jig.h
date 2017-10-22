//---------------------------------------------------------------------------

#ifndef jigH
#define jigH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
//---------------------------------------------------------------------------
class TForm_jig : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPGlyphButton *MaxButton;
	TscGPButton *Button_jig_storno;
	TscGPButton *Button_jig_save;
	TrHTMLLabel *rHTMLLabel_jig_sirka;
	TrEditNum *Edit_jig_pocet_ks;
	TscLabel *scLabel1;
	TscGPButton *scGPButton2;
	TrHTMLLabel *rHTMLLabel_jig_vyska;
	TrEditNum *Edit_jig_vyska;
	TrHTMLLabel *rHTMLLabel_jig_pocetks;
	TrEditNum *Edit_jig_sirka;
	TrHTMLLabel *rHTMLLabel_jig_delka;
	TrEditNum *Edit_jig_delka;
	void __fastcall Button_jig_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_jig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_jig *Form_jig;
//---------------------------------------------------------------------------
#endif
