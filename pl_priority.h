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
	TscGPRadioButton *scGPRadioButton2;
	TscGPRadioButton *scGPRadioButton3;
	TscGPPanel *scGPPanel_header;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_header;
	TscGPButton *Button_save;
	TscGPButton *Button_storno;
	TscGPRadioButton *scGPRadioButton4;
	TrHTMLLabel *rHTMLLabel_hlavika_tabulky;
	TrStringGridEd *rStringGridEd_tab;
	TMemo *Memo1;
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rStringGridEd_tabGetCellParams(TObject *Sender, int Col, int Row,
          TFont *AFont, TColor &Background, bool &Highlight);
	void __fastcall scGPRadioButton1Click(TObject *Sender);
	void __fastcall scGPRadioButton2Click(TObject *Sender);
	void __fastcall scGPRadioButton3Click(TObject *Sender);
	void __fastcall scGPRadioButton4Click(TObject *Sender);
	void __fastcall rStringGridEd_tabKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rStringGridEd_tabMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall rStringGridEd_tabCanEdit(TObject *Sender, int Col, int Row, bool &CanEdit);
	void __fastcall rStringGridEd_tabClick(TObject *Sender);
	void __fastcall rStringGridEd_tabFinishCellEditing(TObject *Sender, int Col, int Row);




private:	// User declarations
public:		// User declarations
	__fastcall TForm_PL_priority(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_PL_priority *Form_PL_priority;
//---------------------------------------------------------------------------
#endif
