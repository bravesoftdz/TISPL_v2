//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "poznamky.h"
//#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_parametry_poznamky *Form_parametry_poznamky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_poznamky::TForm_parametry_poznamky(TComponent* Owner)
	: TForm(Owner)
{
	//defaultní design a pozicování tlačítek OK
	Form1->m.designButton(scGPButton_OK,Form_parametry_poznamky,1,1);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_poznamky::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_poznamky::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

