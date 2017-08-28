//---------------------------------------------------------------------------

#ifndef cestyH
#define cestyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rHTMLLabel.hpp"
#include "rStringGridEd.hpp"
#include "RzButton.hpp"
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TForm_cesty : public TForm
{
__published:	// IDE-managed Components
	TrHTMLLabel *rHTMLLabel3;
	TMemo *Memo1;
	TrStringGridEd *rStringGridEd1;
	TButton *Button_OK;
	TButton *Button1;
	TrHTMLLabel *rHTMLLabel2;
	TrHTMLLabel *rHTMLLabel1;
private:	// User declarations
public:		// User declarations
	__fastcall TForm_cesty(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_cesty *Form_cesty;
//---------------------------------------------------------------------------
#endif
