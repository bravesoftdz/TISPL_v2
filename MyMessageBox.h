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
class TmyMessageBox : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_caption;
	TscGPGlyphButton *scGPGlyph_info;
	TscGPButton *Button_Yes;
	TscGPButton *Button_No;
	TscCheckBox *CheckBox_pamatovat;
	TrHTMLLabel *Label1;
	TrHTMLLabel *Label2;
	TscGPButton *Button_OK;
	TscGPButton *Button_Cancel;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	enum T_mbTYPE{OK=0,OKCANCEL,YESNO,YESNOCANCEL};
public:		// User declarations
	__fastcall TmyMessageBox(TComponent* Owner);
	int Show(long Left,long Top,UnicodeString Label1_text,UnicodeString Label2_text="",UnicodeString Caption_text="",int mbTYPE=OK,bool checkbox_zobrazit=false);
	int Show(UnicodeString Label1_text,int mbTYPE=OK);
	int Show(UnicodeString Label1_text,UnicodeString Label2_text,int mbTYPE=OK);
};
//---------------------------------------------------------------------------
extern PACKAGE TmyMessageBox *myMessageBox;
//---------------------------------------------------------------------------
#endif
