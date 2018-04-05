//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "kabina_schema.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_kabina_schema *Form_kabina_schema;
//---------------------------------------------------------------------------
__fastcall TForm_kabina_schema::TForm_kabina_schema(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláøe
	Form_kabina_schema->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK
	Form1->m.designButton(scGPButton_OK,Form_kabina_schema,1,1);
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
		if (Key == 13) // ENTER
		{
				Form_kabina_schema->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlaèítko
				Form_kabina_schema->VisibleChanging();// skryje form, stejné jako visible=false
		}
		if (Key == 27) // ESC
		{
				Form_kabina_schema->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
				Form_kabina_schema->VisibleChanging();// skryje form, stejné jako visible=false
		}
}
//---------------------------------------------------------------------------

