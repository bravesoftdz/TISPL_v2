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
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	enum T_mbTYPE{OK=0,OKCANCEL=1,YESNO=4,YESNOCANCEL=3};
public:		// User declarations
	__fastcall TmyMessageBox(TComponent* Owner);
	int Show(long left,long top,UnicodeString text,UnicodeString caption_text="",int mbTYPE=OK,bool centrovat_text=true,bool checkbox_zobrazit=false);
	int Show(UnicodeString text,int mbTYPE=OK,bool centrovat_text=true);
};
//---------------------------------------------------------------------------
extern PACKAGE TmyMessageBox *myMessageBox;
//---------------------------------------------------------------------------
#endif
