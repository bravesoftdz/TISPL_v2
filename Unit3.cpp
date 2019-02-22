//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
	F->m.designButton(Button_save,Form3,1,2);
	F->m.designButton(Button_storno,Form3,2,2);                 // Nastavení vzhledu tlaèítek
	Form3->Color=(TColor)RGB(240,240,240);    					// Nastavení barvy formuláøe
	Form3->Left=Form1->ClientWidth/2-Form3->Width/2;
	Form3->Top=Form1->ClientHeight/2-Form3->Height/2;           // Nastavení otevírání okna ve støedu obrazovky
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button_stornoClick(TObject *Sender)
{
	if (mrOk == F->MB("Obsah bude ztracen, opravdu chcete pokraèovat?",MB_OKCANCEL)) {
		Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button_saveClick(TObject *Sender)
{
	double cislo1,cislo2,vysledek;
	cislo1 = scGPNumericEdit_0->Value;
	cislo2 = scGPNumericEdit_1->Value;
	vysledek = cislo1 + cislo2;
	F->MB("Výsledek souètu je: "+AnsiString(vysledek));
}
//---------------------------------------------------------------------------

