//---------------------------------------------------------------------------

#ifndef eDesignerH
#define eDesignerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "rStringGridEd.hpp"
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TForm_eDesigner : public TForm
{
__published:	// IDE-managed Components
	TrStringGridEd *rStringGridEd_tab_dopravniky;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm_eDesigner(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_eDesigner *Form_eDesigner;
//---------------------------------------------------------------------------
#endif
