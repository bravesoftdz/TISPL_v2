//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "poznamky.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_poznamky *Form_poznamky;
//---------------------------------------------------------------------------
__fastcall TForm_poznamky::TForm_poznamky(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláøe
	Form_poznamky->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK a Storno
	Form1->m.designButton(scGPButton_OK,Form_poznamky,1,2);
	Form1->m.designButton(scGPButton_storno,Form_poznamky,2,2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
		if (Key == 13) // ENTER
		{
				Form_poznamky->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlaèítko
				Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
		}
		if (Key == 27) // ESC
		{
				Form_poznamky->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
				Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
		}
}
//---------------------------------------------------------------------------

