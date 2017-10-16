//---------------------------------------------------------------------------

#ifndef cestyH
#define cestyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rStringGridEd.hpp"
#include "RzButton.hpp"
#include <Vcl.Grids.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
//---------------------------------------------------------------------------
class TForm_cesty : public TForm
{
__published:	// IDE-managed Components
	TrStringGridEd *rStringGridEd1;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPGlyphButton *MaxButton;
	TscLabel *scLabel1;
	TscGPButton *scGPButton2;
	TscGPButton *cesta_storno;
	TscGPButton *cesta_save;
	void __fastcall cesta_stornoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations

		nactiNastaveni();

	__fastcall TForm_cesty(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_cesty *Form_cesty;
//---------------------------------------------------------------------------
#endif
