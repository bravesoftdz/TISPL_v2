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
//#include "rHTMLLabel.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
#include "scHtmlControls.hpp"
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
	TscGPButton *Button_OK;
	TscGPButton *Button_Cancel;
	TscGPCheckBox *CheckBox_pamatovat;
	TscGPGlyphButton *scGPGlyphButton_copy;
  TscLabel *Label_text;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
	void __fastcall Button_YesClick(TObject *Sender);
	void __fastcall CheckBox_pamatovatClick(TObject *Sender);

private:	// User declarations
	enum T_mbTYPE{OK=0,OKCANCEL=1,YESNO=4,YESNOCANCEL=3};
public:		// User declarations
	__fastcall TmyMessageBox(TComponent* Owner);
	int Show(long left,long top,UnicodeString text,UnicodeString caption_text="",int mbTYPE=OK,bool centrovat_text=true,bool checkbox_zobrazit=false,int width=366,bool default_button_caption=true,bool copy_zobrazit=false);
	int Show(UnicodeString text,int mbTYPE=OK,bool centrovat_text=true,int width=366,bool default_button_caption=true,bool copy_zobrazit=false);
	bool zobrazitFrameForm;
	bool closing;//detekce kvùli hlavnímu formuláøi jinak volá tìlo formactivate hlavního formu
};
//---------------------------------------------------------------------------
extern PACKAGE TmyMessageBox *myMessageBox;
//---------------------------------------------------------------------------
#endif
