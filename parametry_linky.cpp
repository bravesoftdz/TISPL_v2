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

		
	 rStringGridEd_tab_dopravniky->Cells[0][1]="1";
	 rStringGridEd_tab_dopravniky->Cells[1][1]="Hlavní dopravník";
	 rStringGridEd_tab_dopravniky->Cells[2][1]="2";
	 rStringGridEd_tab_dopravniky->Cells[3][1]="5";
	 rStringGridEd_tab_dopravniky->Cells[4][1]="540";

	 rStringGridEd_tab_dopravniky->Cells[0][2]="2";
	 rStringGridEd_tab_dopravniky->Cells[1][2]="Vedlejší dopravník";
	 rStringGridEd_tab_dopravniky->Cells[2][2]="1";
	 rStringGridEd_tab_dopravniky->Cells[3][2]="4";
	 rStringGridEd_tab_dopravniky->Cells[4][2]="360";

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
//	Form1->d.v.PP.delka_voziku=rEditNum_delkavoziku->Text.ToDouble();

	Form1->DuvodUlozit(true);

	Form1->d.v.vymaz_seznam_pohony();
	Form1->d.v.hlavicka_pohony();

					for (int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
		{
				 Form1->d.v.vloz_pohon (rStringGridEd_tab_dopravniky->Cells[1][i], //nazev
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]),        //rychlost od
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]),    //rychlost do
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]));      //roztec
				 }

				 Form1->DuvodUlozit(true);
				 Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Button_ADDClick(TObject *Sender)
{
	rStringGridEd_tab_dopravniky->RowCount++;
	rStringGridEd_tab_dopravniky->Cols[0]->Add(rStringGridEd_tab_dopravniky->RowCount - 1);

	if (rStringGridEd_tab_dopravniky->RowCount > 2) {

		int i = rStringGridEd_tab_dopravniky->RowCount - 1;

		rStringGridEd_tab_dopravniky->Cells[0][i] = i;
		rStringGridEd_tab_dopravniky->Cells[1][i] = rStringGridEd_tab_dopravniky->Cells[1][i - 1];
		rStringGridEd_tab_dopravniky->Cells[2][i] = rStringGridEd_tab_dopravniky->Cells[2][i - 1];
		rStringGridEd_tab_dopravniky->Cells[3][i] = rStringGridEd_tab_dopravniky->Cells[3][i - 1];
		rStringGridEd_tab_dopravniky->Cells[4][i] = rStringGridEd_tab_dopravniky->Cells[4][i - 1];

	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_parametry_linky::Button_DELClick(TObject *Sender)
{
			//	rStringGridEd_tab_dopravniky->RowCount - 1;
				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount-1]->Clear();

				if(rStringGridEd_tab_dopravniky->RowCount>=4)
				{
				 rStringGridEd_tab_dopravniky->RowCount--;

				 }

	 //	for (long i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
	 //	rStringGridEd_tab_dopravniky->Cells[0][i] = i;

}
//---------------------------------------------------------------------------

