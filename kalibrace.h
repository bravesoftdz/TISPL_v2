//---------------------------------------------------------------------------

#ifndef kalibraceH
#define kalibraceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//#include "rHTMLLabel.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_kalibrace : public TForm
{
__published:	// IDE-managed Components
//  TrHTMLLabel *rHTMLLabel_vzdalenost;
  TscGPNumericEdit *scGPNumericEdit_vzdalenost;
  TscGPPanel *scGPPanel_hlavicka;
  TscGPGlyphButton *Konec;
  TscLabel *scLabel_titulek;
//  TrHTMLLabel *rHTMLLabel1;
  TscGPNumericEdit *scGPNumericEdit1;
  TscGPButton *scGPButton_OK;
  TscGPButton *scGPButton_storno;
  void __fastcall scGPButton_OKClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall scGPButton_stornoClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm_kalibrace(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_kalibrace *Form_kalibrace;
//---------------------------------------------------------------------------
#endif
