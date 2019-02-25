//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TrHTMLLabel *rHTMLLabel_cislo1;
	TrHTMLLabel *rHTMLLabel1;
	TEdit *E_vstup1;
	TEdit *E_vstup2;
	TButton *Button1;
	TMemo *Memo1;
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall Button_saveClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall E_vstup1Change(TObject *Sender);
	void __fastcall E_vstup2Change(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	void kontrola ();
	void vypocet ();
	bool pom1, pom2;
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
