//---------------------------------------------------------------------------

#ifndef reportH
#define reportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <SHDocVw.hpp>
#include <Vcl.OleCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_report : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *WebBrowser1;
	TscGPPanel *scGPPanel_header;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscLabel *scLabel_caption;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *scGPGlyphButton_Info;
	void __fastcall KonecClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	short int ulozit_report(UnicodeString FileName);

	__fastcall TForm_report(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_report *Form_report;
//---------------------------------------------------------------------------
#endif
