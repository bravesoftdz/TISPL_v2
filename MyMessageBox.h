//---------------------------------------------------------------------------

#ifndef MyMessageBoxH
#define MyMessageBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <pngimage.hpp>
#include <Buttons.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TMyMessageBox : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_Yes;
	TButton *Button_No;
	TButton *Button_OK;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPGlyphButton *scGPGlyphButton15;
	TscGPButton *scGPButton1_OK;
	TscGPButton *scGPButton3;
	TscCheckBox *CheckBox_pamatovat;
	TrHTMLLabel *Label1;
	void __fastcall KonecClick(TObject *Sender);
private:	// User declarations
	void dynamicka_velikost_formulare();
public:		// User declarations
	__fastcall TMyMessageBox(TComponent* Owner);
	int ShowMyMessageBox(long Left,long Top,UnicodeString Caption_text, UnicodeString Label1_text,bool checkbox_zobrazit=true);
};
//---------------------------------------------------------------------------
extern PACKAGE TMyMessageBox *MyMessageBox;
//---------------------------------------------------------------------------
#endif
