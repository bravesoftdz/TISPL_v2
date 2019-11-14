//---------------------------------------------------------------------------

#ifndef miniform_zpravyH
#define miniform_zpravyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
//---------------------------------------------------------------------------
class TForm_zpravy : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *scGPPanel2;
  TscGPGlyphButton *Konec;
  TscLabel *scLabel1;
  TscGPGlyphButton *scGPGlyphButton_info;
  TscGPListBox *scGPListBox_zpravy;
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm_zpravy(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_zpravy *Form_zpravy;
//---------------------------------------------------------------------------
#endif
