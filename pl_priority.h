//---------------------------------------------------------------------------

#ifndef pl_priorityH
#define pl_priorityH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rStringGridEd.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.Grids.hpp>
#include "parametry_linky.h"
#include "Unit1.h"

//---------------------------------------------------------------------------
class TForm_PL_priority : public TForm
{
__published:	// IDE-managed Components
	TrHTMLLabel *rHTMLLabel_text;
	TscGPRadioButton *scGPRadioButton1;
	TrStringGridEd *rStringGridEd_tab;
	TscGPRadioButton *scGPRadioButton2;
	TscGPRadioButton *scGPRadioButton3;
	TscGPPanel *scGPPanel_header;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_header;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPRadioButton *scGPRadioButton4;
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rStringGridEd_tabGetCellParams(TObject *Sender, int Col, int Row,
          TFont *AFont, TColor &Background, bool &Highlight);
	void __fastcall scGPRadioButton1Click(TObject *Sender);
	void __fastcall scGPRadioButton2Click(TObject *Sender);
	void __fastcall scGPRadioButton3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_PL_priority(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_PL_priority *Form_PL_priority;
//---------------------------------------------------------------------------
#endif
