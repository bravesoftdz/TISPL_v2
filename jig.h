//---------------------------------------------------------------------------

#ifndef jigH
#define jigH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
//---------------------------------------------------------------------------
class TForm_jig : public TForm
{
__published:	// IDE-managed Components
	TrHTMLLabel *rHTMLLabel3;
	TrHTMLLabel *rHTMLLabel2;
	TrEditNum *rEditNum1;
	TButton *Button_OK;
	TButton *Button1;
private:	// User declarations
public:		// User declarations
	__fastcall TForm_jig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_jig *Form_jig;
//---------------------------------------------------------------------------
#endif
