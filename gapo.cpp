//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gapo.h"
#include "unit1.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_gapo *Form_gapo;
//---------------------------------------------------------------------------
__fastcall TForm_gapo::TForm_gapo(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláøe
	Form_gapo->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK
	F->m.designButton(scGPButton_OK,Form_gapo,1,1);
}
//---------------------------------------------------------------------------
void __fastcall TForm_gapo::FormActivate(TObject *Sender)
{
	//definice tabulky
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=1;//ID tabulky
	mGrid->Left=10;mGrid->Top=scGPPanel_hlavicka->Height+10;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)

	switch(gapoTyp)
	{
		//TAKT TIME
		case TT:
		{

		}break;
		////VOZÍK
		case V:
		{

		}break;
		////R - záležitosti
		case R:
		{
			////samotné vytvoøení tabulky s požadovaným poètem sloupcù a øádkù
			mGrid->Create(14,5);//vhodné jako tøetí

			////plnìní daty - hlavièka
			mGrid->Cells[0][0].Text="pouze zmìnìné pohony";
			mGrid->Cells[0][0].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
			mGrid->Cells[0][0].Font->Orientation=900;
			mGrid->Cells[0][0].Align=mGrid->CENTER;
			mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
			mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=10;

			mGrid->Cells[1][0].Text="ovlivnìné objekty";
			mGrid->Cells[2][0].Text="RD,DD,M,P";
			mGrid->Cells[3][0].Text="K,CT";
			mGrid->Cells[4][0].Text="CT,RD,K,P,M";
			mGrid->Cells[5][0].Text="DD";
			mGrid->Cells[6][0].Text="CT - Technologický èas";
			mGrid->Cells[7][0].Text="RD - Rychlost pohonu";
			mGrid->Cells[8][0].Text="DD - Délka objekt";
			mGrid->Cells[9][0].Text="K - Kapacita";
			mGrid->Cells[10][0].Text="P - Pozice";
			mGrid->Cells[11][0].Text="M - mezera (jig)";
			mGrid->Cells[12][0].Text="M - mezera (vozík)";
			mGrid->Cells[13][0].Text="Rotace";

			//pøiøadí celo oblasti bunìk totožné vlastnosti jako u referenèní buòky
			mGrid->SetCells(mGrid->Cells[0][0],0,0,13,0,0,false);

			//manualfit výšky 0tého øádku (zatím není pøipravena metoda)
			Canvas->Font=mGrid->Cells[6][0].Font;	//nejdelší použitý text
			mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[6][0].Text)+mGrid->Cells[6][0].BottomMargin+mGrid->Cells[6][0].BottomBorder->Width/2+mGrid->Cells[6][0].TopMargin+mGrid->Cells[6][0].TopBorder->Width/2;

			////jednolivé øádky
			//první dva sloupce
			mGrid->Cells[0][1].Text="Pohon 1";mGrid->Cells[1][1].Text="NAV";
			mGrid->Cells[0][2].Text="Pohon 2";mGrid->Cells[1][2].Text="ION";
																				mGrid->Cells[1][3].Text="CO2";
																				mGrid->Cells[1][4].Text="LAK";
			mGrid->MergeCells(0,2,0,4);//slouèení
			//volby
			mGrid->Cells[2][1].Type=mGrid->RADIO;mGrid->Cells[4][1].Type=mGrid->RADIO;mGrid->MergeCells(2,1,3,1);mGrid->RADIO;mGrid->MergeCells(4,1,5,1);//slouèení
			mGrid->Cells[2][2].Type=mGrid->RADIO;mGrid->Cells[4][2].Type=mGrid->RADIO;mGrid->MergeCells(2,2,3,2);mGrid->RADIO;mGrid->MergeCells(4,2,5,2);
			mGrid->Cells[2][3].Type=mGrid->RADIO;mGrid->Cells[4][3].Type=mGrid->RADIO;mGrid->MergeCells(2,3,3,3);mGrid->RADIO;mGrid->MergeCells(4,3,5,3);
			mGrid->Cells[2][4].Type=mGrid->RADIO;mGrid->Cells[4][4].Type=mGrid->RADIO;mGrid->MergeCells(2,4,3,4);mGrid->RADIO;mGrid->MergeCells(4,4,5,4);
		}break;
	}
	//nastavení velikosti sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show()
	mGrid->SetColumnAutoFit(-1);
}
//---------------------------------------------------------------------------
void __fastcall TForm_gapo::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykreslí tabulku
}
//---------------------------------------------------------------------------
//test volání pøi onclick
void TForm_gapo::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	ShowMessage("Došlo ke kliku v tabulce: "+AnsiString(Tag)+", na buòce: "+AnsiString(Col)+","+AnsiString(Row));
	mGrid->HighlightCell(Col,Row);
	FormPaint(this);//zajistí pøekreslení bez probliku
	//toto problikává mGrid->Refresh();
}
//---------------------------------------------------------------------------
void TForm_gapo::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TForm_gapo::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_gapo::FormClose(TObject *Sender, TCloseAction &Action)
{
	//mGrid->Delete();//pokud chci odstranit a nechci použít na další použití
}
//---------------------------------------------------------------------------
void __fastcall TForm_gapo::scGPButton_OKClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

