//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "kalendar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_kalendar *Form_kalendar;
//---------------------------------------------------------------------------
__fastcall TForm_kalendar::TForm_kalendar(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_kalendar::scGPButton3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
