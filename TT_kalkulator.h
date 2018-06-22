//---------------------------------------------------------------------------

#ifndef TT_kalkulatorH
#define TT_kalkulatorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "rHTMLLabel.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_TT_kalkulator : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_hlavicka;
	TscGPGlyphButton *scGPGlyphButton_info;
	TrHTMLLabel *rHTMLLabel_takt;
	TscGPButton *scGPButton_takt;
	TscGPNumericEdit *rEditNum_takt;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_TT_kalkulator(TComponent* Owner);
	void Nastav_design();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_TT_kalkulator *Form_TT_kalkulator;
//---------------------------------------------------------------------------
#endif
