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
	Form1->m.designButton((TButton*)Button_save,Form_parametry_linky,1,2);
	Form1->m.designButton((TButton*)Button_storno,Form_parametry_linky,2,2);



}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{

		nacti_pohony();

		if (!data_nalezena) {
		rStringGridEd_tab_dopravniky->RowCount=3;    //defaultní poèet øádkù - hlavièka, hl.dopravník,vedl.dopravník
		}

		//	Form1->d.v.vymaz_seznam_POHONY();

	 Form_parametry_linky->Color=(TColor)RGB(240,240,240);//RGB(43,87,154);
	 rStringGridEd_tab_dopravniky->Columns->Items[0]->Visible=false;

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


	 //nahrání hodnot
	 rEditNum_delkavoziku->Text=Form1->d.v.PP.delka_voziku;
	 scRadioGroup_typVoziku->ItemIndex=Form1->d.v.PP.typ_voziku;
}
//---------------------------------------------------------------------------

void TForm_parametry_linky::nacti_pohony (){

   data_nalezena=false;
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

	 if (ukaz!=NULL) {

				rStringGridEd_tab_dopravniky->RowCount = Form1->d.v.POHONY->predchozi->n + 1;
				data_nalezena=true; //pokud jsou ve spojaku nejaka data, nastavit na true
				 for (int i=1;i<rStringGridEd_tab_dopravniky->RowCount;i++)  {

				 //ShowMessage(ukaz->n);
			rStringGridEd_tab_dopravniky->Cells[0][i] = ukaz->n;
			rStringGridEd_tab_dopravniky->Cells[1][i] = ukaz->name;
			rStringGridEd_tab_dopravniky->Cells[2][i] = ukaz->rychlost_od;
			rStringGridEd_tab_dopravniky->Cells[3][i] = ukaz->rychlost_do;
			rStringGridEd_tab_dopravniky->Cells[4][i] = ukaz->roztec;

				ukaz = ukaz->dalsi;
				 }

	}
	else {  //pokud je spoják prázdný, zobrazím tyto pøednastavené hodnoty

	data_nalezena=false; // default se nepoužívá resp. po soubor nový se okamžitì vloží do spojáku def.pohon
//naèítá se stejnì z unit1
//   rStringGridEd_tab_dopravniky->Cells[0][1]="1";
//	 rStringGridEd_tab_dopravniky->Cells[1][1]="Hlavní dopravník";
//	 rStringGridEd_tab_dopravniky->Cells[2][1]="";
//	 rStringGridEd_tab_dopravniky->Cells[3][1]="";
//	 rStringGridEd_tab_dopravniky->Cells[4][1]="";
	}
}


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
	Form1->d.v.vymaz_seznam_POHONY();
	Form1->d.v.hlavicka_POHONY();

					for (int i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
		{
				 Form1->d.v.vloz_pohon (rStringGridEd_tab_dopravniky->Cells[1][i], //nazev
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[2][i]),        //rychlost od
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[3][i]),    //rychlost do
																Form1->ms.MyToDouble(rStringGridEd_tab_dopravniky->Cells[4][i]));      //roztec
				 }

				 Form1->d.v.PP.delka_voziku=Form1->ms.MyToDouble(rEditNum_delkavoziku->Text);
				 Form1->d.v.PP.typ_voziku=Form1->ms.MyToDouble(scRadioGroup_typVoziku->ItemIndex);

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
				rStringGridEd_tab_dopravniky->Rows[rStringGridEd_tab_dopravniky->RowCount]->Clear();

				if(rStringGridEd_tab_dopravniky->RowCount>=3)
				{
				 rStringGridEd_tab_dopravniky->RowCount--;

				 }
				 else {

				 Form1->MB("Linka musí obsahovat alespoò jeden pohon.");
				 }

	 //	for (long i = 1; i < rStringGridEd_tab_dopravniky->RowCount; i++)
	 //	rStringGridEd_tab_dopravniky->Cells[0][i] = i;

}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_linky::Vypis_pohonyClick(TObject *Sender)
{
	 Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

				 while (ukaz!=NULL) {

				 ShowMessage(ukaz->n);
				 ShowMessage(ukaz->name);
				 ShowMessage(ukaz->roztec);

				 	ukaz = ukaz->dalsi;

				 }
}
//---------------------------------------------------------------------------




