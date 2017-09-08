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
	TscGPButton *jig_storno;
	TscGPButton *jig_save;
	TrHTMLLabel *rHTMLLabel5;
	TrEditNum *rEditNum4;
	TscLabel *scLabel1;
	TscGPButton *scGPButton2;
	void __fastcall jig_stornoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_jig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_jig *Form_jig;
//---------------------------------------------------------------------------
#endif
