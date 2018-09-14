//---------------------------------------------------------------------------

#ifndef poznamkyH
#define poznamkyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include "rHTMLLabel.hpp"
#include "unit1.h"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_poznamky : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPMemo *scGPMemo;
	TscGPButton *scGPButton_OK;
	TscGPButton *scGPButton_storno;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TscGPCheckBox *scGPCheckBox_zaokrouhlit;
	TscGPCheckBox *scGPCheckBox_STOPKA;
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall scGPMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scGPCheckBox_zaokrouhlitClick(TObject *Sender);
	void __fastcall scGPCheckBox_STOPKAClick(TObject *Sender);

private:	// User declarations
	TColor clBACKGROUND,clLOCKED,clUNLOCKED,clLOCKEDhead;
	void table_head();//definuje hlavièku tabulky
	void calculate(short INPUT_state=0);//vypoèítává ve S&G subparametry
	Cvektory::TPohon *P,*PD;
	short Decimal;//poèet desetinných míst
	AnsiString pz;//zástupný znak pokraèování
	bool start;
public:		// User declarations
	__fastcall TForm_poznamky(TComponent* Owner);
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_poznamky *Form_poznamky;
//---------------------------------------------------------------------------
#endif
