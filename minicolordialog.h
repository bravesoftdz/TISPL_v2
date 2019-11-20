//---------------------------------------------------------------------------

#ifndef minicolordialogH
#define minicolordialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scColorControls.hpp"
#include "scControls.hpp"
#include "scExtControls.hpp"
//---------------------------------------------------------------------------
class TForm_color_dialog : public TForm
{
__published:	// IDE-managed Components
  TscExPanel *scExPanel_colors;
  TscColorGrid *scColorGrid1;
  void __fastcall scColorGrid1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall scColorGrid1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm_color_dialog(TComponent* Owner);
  void            SetDialogPosition(double Row);
  double ROW;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_color_dialog *Form_color_dialog;
//---------------------------------------------------------------------------
#endif
