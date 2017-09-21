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
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
//---------------------------------------------------------------------------
class TForm_dopravnik : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *ValueListEditor;
	TButton *Button_ADD;
	TButton *Button_DEL;
	TrStringGridEd *rStringGridEd1;
	TscGPButton *dopravnik_save;
	TscGPButton *dopravnik_storno;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscLabel *scLabel1;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *scGPGlyphButton15;
	TscGPButton *scGPButton2;
	TrHTMLLabel *rHTMLLabel5;
	TrEditNum *rEditNum4;
	TscGPButton *scGPButton1;
	TscGPButton *scGPButton3;
	TscGPComboBox *scGPComboBox1;
	TrHTMLLabel *rHTMLLabel1;
	void __fastcall Button_ADDClick(TObject *Sender);
	void __fastcall Button_DELClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button_OKClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall dopravnik_stornoClick(TObject *Sender);

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
