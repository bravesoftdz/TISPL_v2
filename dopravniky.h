//---------------------------------------------------------------------------

#ifndef dopravnikyH
#define dopravnikyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
#include "rStringGridEd.hpp"
//---------------------------------------------------------------------------
class TForm_dopravnik : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *ValueListEditor;
	TButton *Button_ADD;
	TButton *Button_DEL;
	TButton *Button_OK;
	TStaticText *StaticText1;
	TStaticText *StaticText2;
	TrHTMLLabel *rHTMLLabel4;
	TrEditNum *rEditNum2;
	TrStringGridEd *rStringGridEd1;
	TButton *Button1;
	TrHTMLLabel *rHTMLLabel5;
	TButton *Button2;
	void __fastcall Button_ADDClick(TObject *Sender);
	void __fastcall Button_DELClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button_OKClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	unsigned int count;
	bool OK_status;
	TStringList *origSL;
	unsigned int offsetcomponents;
public:		// User declarations
	__fastcall TForm_dopravnik(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_dopravnik *Form_dopravnik;
//---------------------------------------------------------------------------
#endif
