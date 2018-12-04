//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "adjustace.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_adjustace *Form_adjustace;
//---------------------------------------------------------------------------
__fastcall TForm_adjustace::TForm_adjustace(TComponent* Owner)
  : TForm(Owner)
{
		Form1->m.designButton(scGPButton_OK, Form_adjustace, 1, 2);
		Form1->m.designButton(scGPButton_storno, Form_adjustace, 2, 2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_adjustace::scGPButton_stornoClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_adjustace::KonecClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_adjustace::FormShow(TObject *Sender)
{
		Form_adjustace->Left=Form1->ClientWidth/2-Form_adjustace->Width/2;
		Form_adjustace->Top=Form1->ClientHeight/2-Form_adjustace->Height/2;
}
//---------------------------------------------------------------------------
