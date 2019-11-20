//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "minicolordialog.h"
#include "superform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scColorControls"
#pragma link "scControls"
#pragma link "scExtControls"
#pragma resource "*.dfm"

TForm_color_dialog *Form_color_dialog;
//---------------------------------------------------------------------------
__fastcall TForm_color_dialog::TForm_color_dialog(TComponent* Owner)
  : TForm(Owner)
{
ROW=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_color_dialog::scColorGrid1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{

 Form_definice_zakazek->setButtonColor(ROW);
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_color_dialog::scColorGrid1Click(TObject *Sender)
{
  Form_definice_zakazek->setButtonColor(ROW);
}
//---------------------------------------------------------------------------
void  TForm_color_dialog:: SetDialogPosition(double Row)
{
  //umístìní color dialogu
   double TOP = Form_definice_zakazek->scLabel_header->Height + Form_definice_zakazek->scGPButton_plan_vyroby->Height +  10;
   Form_color_dialog->Top=Form_definice_zakazek->Top + TOP+24*Row + 1;
   Form_color_dialog->Left=Form_definice_zakazek->Left + 320;
   ROW=Row;
}
