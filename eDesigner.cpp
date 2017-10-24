//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "eDesigner.h"
//#include "Unit1.h"
#include "parametry_linky.h"
#include "superform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rStringGridEd"
#pragma resource "*.dfm"
TForm_eDesigner *Form_eDesigner;
//---------------------------------------------------------------------------
__fastcall TForm_eDesigner::TForm_eDesigner(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_eDesigner::FormShow(TObject *Sender)
{
	//if(Form_parametry_linky->ShowModal())
	//Form_definice_zakazek->ShowModal();
	//Form1->Button_dopravnik_parametryClick(Sender);//volání parametrù linky
	//Form1->scGPGlyphButton_definice_zakazekClick(Sender);//volání superformuláøe (definice zakázek)
}
//---------------------------------------------------------------------------

