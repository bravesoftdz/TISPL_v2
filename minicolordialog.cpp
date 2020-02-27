//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "minicolordialog.h"
#include "superform.h"
#include "vektory.h"
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
ID=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_color_dialog::scColorGrid1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{

 Form_definice_zakazek->setButtonColor(ID);
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_color_dialog::scColorGrid1Click(TObject *Sender)
{
  Form_definice_zakazek->setButtonColor(ID);
}
//---------------------------------------------------------------------------
void  TForm_color_dialog:: SetDialogPosition(double Row, double Id)
{
  //umístìní color dialogu
  Cvektory::TZakazka *Z = F->d.v.vrat_temp_zakazku(Id);
   double TOP = Z->mGrid->Top + Z->mGrid->DefaultRowHeight;
   Top=Form_definice_zakazek->Top + TOP+24*Row + 1;
   Left=Form_definice_zakazek->Left + Z->mGrid->Left + Z->mGrid->Columns[0].Width + 1;
   ROW=Row;
   ID=Id;
}

