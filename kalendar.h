//---------------------------------------------------------------------------

#ifndef kalendarH
#define kalendarH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rStringGridEd.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.Grids.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_kalendar : public TForm
{
__published:	// IDE-managed Components
	TrStringGridEd *rStringGridEd_tab_dopravniky;
	TscGPButton *scGPButton4;
	TscGPButton *scGPButton_Ulozit;
	TscGPButton *scGPButton3;
	TscGPButton *scGPButton1;
	TscEdit *scEdit_zacatek;
	TscGPButton *scGPButton2;
	TscEdit *scEdit1;
	TrHTMLLabel *rHTMLLabel_zacatek;
	TrHTMLLabel *rHTMLLabel1;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscLabel *scLabel1;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *scGPGlyphButton15;
	void __fastcall scGPButton3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_kalendar(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_kalendar *Form_kalendar;
//---------------------------------------------------------------------------
#endif
