//---------------------------------------------------------------------------

#ifndef Z_rozliseniH
#define Z_rozliseniH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TForm_Z_rozliseni : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPButton *scGPButton_storno;
	TscGPButton *scGPButton_OK;
	TImage *Image1;
	TImage *Image2;
	TImage *Image3;
	TrHTMLLabel *rHTMLLabel_Text1;
	TrHTMLLabel *rHTMLLabel_Text2;
	TrHTMLLabel *rHTMLLabel_Text3;
	TrHTMLLabel *rHTMLLabel_upozorneni;
	TscGPGlyphButton *MinButton;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall MinButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_Z_rozliseni(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Z_rozliseni *Form_Z_rozliseni;
//---------------------------------------------------------------------------
#endif
