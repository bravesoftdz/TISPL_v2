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
#include "scStyledForm.hpp"
//---------------------------------------------------------------------------
class TForm_report : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *WebBrowser1;
	TscGPPanel *scGPPanel_header;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *scGPGlyphButton_Info;
	TscLabel *scLabel1;
	TscGPPanel *scGPPanel1;
	TscStyledForm *scStyledForm1;
	TscButton *scButton_html;
	TscButton *scButton_csv;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scButton_csvClick(TObject *Sender);
	void __fastcall MaxButtonClick(TObject *Sender);
	void __fastcall scButton_htmlClick(TObject *Sender);
private:	// User declarations

	bool FMaximized;
public:		// User declarations

	enum Tm_mm{M=0,MM};Tm_mm DDunit;Tm_mm Munit;Tm_mm Rzunit;Tm_mm Runit;Tm_mm Delkaunit;Tm_mm Sirkaunit;//pøepínaè jednotek vzdálenost
	enum Tminsec{S=0,MIN};Tminsec CTunit;Tminsec TTunit;Tminsec RDunit;Tminsec aRDunit;Tminsec minsec;//pøepínaè jednotek èasu

	short int ulozit_report(UnicodeString FileName);

	__fastcall TForm_report(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_report *Form_report;
//---------------------------------------------------------------------------
#endif
