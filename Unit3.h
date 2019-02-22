//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPGlyphButton *MaxButton;
	TscLabel *scLabel1;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPNumericEdit *scGPNumericEdit_0;
	TrHTMLLabel *rHTMLLabel_cislo1;
	TscGPNumericEdit *scGPNumericEdit_1;
	TrHTMLLabel *rHTMLLabel1;
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall Button_saveClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
