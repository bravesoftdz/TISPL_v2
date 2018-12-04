//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "kalibrace.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_kalibrace *Form_kalibrace;
//---------------------------------------------------------------------------
__fastcall TForm_kalibrace::TForm_kalibrace(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_kalibrace::scGPButton_OKClick(TObject *Sender)
{
//
F->d.v.PP.raster.X=F->akt_souradnice_kurzoru_PX.x;
F->d.v.PP.raster.Y=F->akt_souradnice_kurzoru_PX.y;
F->REFRESH();
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_kalibrace::FormShow(TObject *Sender)
{
		Form_kalibrace->Left=Form1->ClientWidth/2-Form_kalibrace->Width/2;
		Form_kalibrace->Top=Form1->ClientHeight/2-Form_kalibrace->Height/2;
}
//---------------------------------------------------------------------------
void __fastcall TForm_kalibrace::scGPButton_stornoClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
