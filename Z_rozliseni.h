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
#include <Vcl.Imaging.jpeg.hpp>
#include "scModernControls.hpp"
#include "scHtmlControls.hpp"
//---------------------------------------------------------------------------
class TForm_Z_rozliseni : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPButton *scGPButton_storno;
	TscGPButton *scGPButton_OK;
	TImage *Image1;
	TImage *Image2;
	TImage *Image3;
	TscGPGlyphButton *MinButton;
	TImage *Image4;
	TImage *Image5;
	TImage *Image6;
	TscGPSwitch *scGPSwitch_language;
	TscLabel *scLabel_CZ;
	TscLabel *scLabel_EN;
	TscHTMLLabel *scHTMLLabel_Text1;
	TscHTMLLabel *scHTMLLabel_Text2;
	TscHTMLLabel *scHTMLLabel_Text3;
	TscHTMLLabel *scHTMLLabel_upozorneni;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall MinButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scGPSwitch_languageChangeState(TObject *Sender);
private:	// User declarations
	void zmena_jazyka();
	TStringList *ls,*ls_temp;
public:		// User declarations
	__fastcall TForm_Z_rozliseni(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Z_rozliseni *Form_Z_rozliseni;
//---------------------------------------------------------------------------
#endif
