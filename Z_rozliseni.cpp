//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "Z_rozliseni.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm_Z_rozliseni *Form_Z_rozliseni;
//---------------------------------------------------------------------------
__fastcall TForm_Z_rozliseni::TForm_Z_rozliseni(TComponent* Owner)
	: TForm(Owner)
{
	//Formuláø
	Form_Z_rozliseni->Width=Screen->WorkAreaWidth;
	Form_Z_rozliseni->Height=Screen->WorkAreaHeight;
	Form_Z_rozliseni->Left=Form1->ClientWidth/2-Form_Z_rozliseni->Width/2;
	Form_Z_rozliseni->Top=Form1->ClientHeight/2-Form_Z_rozliseni->Height/2;
	//Nadpis part1
	rHTMLLabel_upozorneni->Top = scGPPanel2->Height + scGPPanel2->Height/2;
	//Tlaèítka
	F->m.designButton(scGPButton_OK, Form_Z_rozliseni, 1, 2);
	F->m.designButton(scGPButton_storno, Form_Z_rozliseni, 2, 2);
	//Obrázky
	Image1->AutoSize = true;
	Image1->Left = Form_Z_rozliseni->Width/3 -Form_Z_rozliseni->Width/3/2 - Image1->Width/2;
	Image1->Top = rHTMLLabel_upozorneni->Top + rHTMLLabel_upozorneni->Height + scGPPanel2->Height/2;

	Image2->AutoSize = true;
	Image2->Left = 2*Form_Z_rozliseni->Width/3 -Form_Z_rozliseni->Width/3/2 - Image1->Width/2;
	//Image2->Top = Image1->Top + (Image1->Height - Image2->Height);
	Image2->Top = Image1->Top;

	Image3->AutoSize = true;
	Image3->Left = Form_Z_rozliseni->Width -Form_Z_rozliseni->Width/3/2 - Image1->Width/2;
	//Image3->Top = Image1->Top + (Image1->Height - Image3->Height);
	Image3->Top = Image1->Top;
	//Nadpis part2
    rHTMLLabel_upozorneni->AutoSize = true;
	rHTMLLabel_upozorneni->Left = Form_Z_rozliseni->Width/2 - rHTMLLabel_upozorneni->Width/2;
	//Text
	rHTMLLabel_Text1->Width = Image1->Width;
	rHTMLLabel_Text1->WordWrap = true;
	rHTMLLabel_Text1->AutoSize = true;
	rHTMLLabel_Text1->Left = Image1->Left;
	rHTMLLabel_Text1->Top = Image1->Top + Image1->Height + scGPPanel2->Height/2;

	rHTMLLabel_Text2->Width = Image2->Width;
	rHTMLLabel_Text2->WordWrap = true;
	rHTMLLabel_Text2->AutoSize = true;
	rHTMLLabel_Text2->Left = Image2->Left;
	//rHTMLLabel_Text2->Top = rHTMLLabel_Text1->Top;
	rHTMLLabel_Text2->Top = Image2->Top + Image2->Height + scGPPanel2->Height/2;

	rHTMLLabel_Text3->Width = Image3->Width;
	rHTMLLabel_Text3->WordWrap = true;
	rHTMLLabel_Text3->AutoSize = true;
	rHTMLLabel_Text3->Left = Image3->Left;
	//rHTMLLabel_Text3->Top = rHTMLLabel_Text1->Top;
	rHTMLLabel_Text3->Top = Image3->Top + Image3->Height + scGPPanel2->Height/2;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Z_rozliseni::KonecClick(TObject *Sender)
{
	F->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Z_rozliseni::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Z_rozliseni::MinButtonClick(TObject *Sender)
{
	Application->Minimize();
}
//---------------------------------------------------------------------------

