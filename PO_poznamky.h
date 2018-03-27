//---------------------------------------------------------------------------

#ifndef PO_poznamkyH
#define PO_poznamkyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
//---------------------------------------------------------------------------
class TForm_parametry_poznamky : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPMemo *scGPMemo;
	TscGPButton *scGPButton_OK;
	TscGPButton *scGPButton_storno;
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_parametry_poznamky(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry_poznamky *Form_parametry_poznamky;
//---------------------------------------------------------------------------
#endif
