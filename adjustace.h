//---------------------------------------------------------------------------

#ifndef adjustaceH
#define adjustaceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_adjustace : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *scGPPanel_hlavicka;
  TscGPGlyphButton *Konec;
  TscLabel *scLabel_titulek;
  TscGPButton *scGPButton_storno;
  TscGPButton *scGPButton_OK;
  TscGPNumericEdit *scGPNumericEdit_vzdalenost;
  TrHTMLLabel *rHTMLLabel_vzdalenost;
  void __fastcall scGPButton_stornoClick(TObject *Sender);
  void __fastcall KonecClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall scGPButton_OKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

   void position();

  __fastcall TForm_adjustace(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_adjustace *Form_adjustace;
//---------------------------------------------------------------------------
#endif
