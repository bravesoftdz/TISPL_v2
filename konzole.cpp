//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "konzole.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scGPPagers"
#pragma link "scHtmlControls"
#pragma link "scModernControls"
#pragma resource "*.dfm"
TForm_konzole *Form_konzole;
//---------------------------------------------------------------------------
__fastcall TForm_konzole::TForm_konzole(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::FormShow(TObject *Sender)
{
	//centrov�n� formu
	Left=Form1->ClientWidth/2-Form_konzole->Width/2;
	Top=Form1->ClientHeight/2-Form_konzole->Height/2;
	//na�ten� jazykov� mutace
	scGPToolPager->Tabs->operator [](0)->Caption=F->ls->Strings[459];
	scLabel_header->Caption=F->ls->Strings[460];
}
//---------------------------------------------------------------------------
