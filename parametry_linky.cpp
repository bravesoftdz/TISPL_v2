//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "parametry_linky.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_linky::TForm_parametry_linky(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{
	 rStringGridEd1->Cells[0][1]="1";
	 rStringGridEd1->Cells[1][1]="Hlavní dopravník";
	 rStringGridEd1->Cells[2][1]="2";
	 rStringGridEd1->Cells[3][1]="5";
	 rStringGridEd1->Cells[4][1]="540";

	 rStringGridEd1->Cells[0][2]="2";
	 rStringGridEd1->Cells[1][2]="Vedlejší dopravník";
	 rStringGridEd1->Cells[2][2]="1";
	 rStringGridEd1->Cells[3][2]="4";
	 rStringGridEd1->Cells[4][2]="360";

	 Form_parametry_linky->Color=(TColor)RGB(225,225,225);//RGB(43,87,154);

	 scGPButton1->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton1->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton1->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton1->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton1->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton1->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton1->Options->FramePressedColor=Form_parametry_linky->Color;

	 scGPButton2->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton2->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton2->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton2->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton2->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton2->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton2->Options->FramePressedColor=Form_parametry_linky->Color;

	 scGPButton3->Options->NormalColor=Form_parametry_linky->Color;
	 scGPButton3->Options->FocusedColor=Form_parametry_linky->Color;
	 scGPButton3->Options->HotColor=Form_parametry_linky->Color;
	 scGPButton3->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton3->Options->FrameNormalColor=Form_parametry_linky->Color;
	 scGPButton3->Options->PressedColor=Form_parametry_linky->Color;
	 scGPButton3->Options->FramePressedColor=Form_parametry_linky->Color;

	 //nahrání hodnot
	 rEditNum_delkavoziku->Text=Form1->d.v.PP.delka_voziku;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_stornoClick(TObject *Sender)
{

	Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::KonecClick(TObject *Sender)
{
	Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button_saveClick(TObject *Sender)
{
	Form1->d.v.PP.delka_voziku=rEditNum_delkavoziku->Text.ToDouble();
	Form_parametry_linky->Close();
	Form1->DuvodUlozit(true);
}
//---------------------------------------------------------------------------

