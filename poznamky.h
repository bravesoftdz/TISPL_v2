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
	TscGPButton *scGPButton_zamek_PT;
	TscGPButton *scGPButton_zamek_DD;
	TscGPCheckBox *scGPCheckBox_WTlock;
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall scGPMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scGPCheckBox_zaokrouhlitClick(TObject *Sender);
	void __fastcall scGPCheckBox_STOPKAClick(TObject *Sender);
	void __fastcall scButton_zamek_PTaDDClick(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall scGPCheckBox_WTlockClick(TObject *Sender);

private:	// User declarations
	TColor clBACKGROUND,clLOCKED,clUNLOCKED,clTITLE;
	enum Tinput_state{NO,NOTHING,MT1state,PTstate,MT2state,P1state,P2state};//uchovává výbìr input hodnoty (aby se data v buòkách necyklyly)
	Tinput_state input_state;
	void table_head();//definuje hlavièku tabulky
	void calculate(Tinput_state input_state);//vypoèítává ve S&G subparametry
	void setDD(bool locked);void setPT(bool locked);
	Cvektory::TPohon *P,*PD;
	double DD;//délka dopravníku, kvùli navrácení do PO
	short Decimal;//poèet desetinných míst
	AnsiString pz;//zástupný znak pokraèování
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
