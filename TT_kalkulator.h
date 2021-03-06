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
  TrHTMLLabel *rHTMLLabel_InfoText;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall rEditNum_taktKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button_saveClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm_TT_kalkulator(TComponent* Owner);
	void Nastav_design();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_TT_kalkulator *Form_TT_kalkulator;
//---------------------------------------------------------------------------
#endif
