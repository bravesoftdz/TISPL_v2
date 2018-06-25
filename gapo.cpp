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
	mGrid->AntiAliasing_text=true;

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
			bool *pohony_zmena=new bool[F->d.v.POHONY->predchozi->n];//vytvoøí pole bool dle poètu pohonù, každy pohon mùže být nastaven na true èi false, že byl èi nebyl zmìnìn
			pohony_zmena[0]=true;pohony_zmena[1]=true;pohony_zmena[2]=false;
			unsigned long RowCount=5;

			unsigned long ColCount=14;
			mGrid->Create(ColCount,RowCount);//vhodné jako tøetí

			////plnìní daty - hlavièka
			mGrid->Cells[0][0].Text="pouze zmìnìné pohony";
			mGrid->Cells[0][0].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
			mGrid->Cells[0][0].Font->Orientation=900;
			mGrid->Cells[0][0].Align=mGrid->CENTER;
			mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
			mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=10;

			mGrid->Cells[1][0].Text="ovlivnìné techn. objekty";
			mGrid->Cells[2][0].Text="RD,DD,M,P";
			mGrid->Cells[3][0].Text="K,CT";
			mGrid->Cells[4][0].Text="CT,RD,K,P,M";
			mGrid->Cells[5][0].Text="DD";
			mGrid->Cells[6][0].Text="CT - Technologický èas"; //mGrid->MergeCells(6,0,7,0);//slouèení
			mGrid->Cells[7][0].Text="RD - Rychlost pohonu";
			mGrid->Cells[8][0].Text="DD - Délka objekt";
			mGrid->Cells[9][0].Text="K - Kapacita";
			mGrid->Cells[10][0].Text="P - Pozice";
			mGrid->Cells[11][0].Text="M - mezera (jig)";
			mGrid->Cells[12][0].Text="M - mezera (vozík)";
			mGrid->Cells[13][0].Text="Rotace";

			//pøiøadí cele oblasti bunìk totožné vlastnosti jako u referenèní buòky
			mGrid->SetCells(mGrid->Cells[0][0],0,0,ColCount-1,0);
			//v tomto pøípadì šedý sloupec (stejný jako orámování) jako nemìné hodnoty
			//mGrid->Cells[ColCount-1][1].Background->Color=mGrid->Cells[ColCount-1][1].TopBorder->Color;
			//mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);


			//manualfit výšky 0tého øádku (zatím není pøipravena metoda)
			Canvas->Font=mGrid->Cells[6][0].Font;	//nejdelší použitý text
			mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[1][0].Text)+mGrid->Cells[1][0].BottomMargin+mGrid->Cells[1][0].BottomBorder->Width/2+mGrid->Cells[1][0].TopMargin+mGrid->Cells[1][0].TopBorder->Width/2;

			////jednolivé øádky
			//první dva sloupce
			mGrid->Cells[0][1].Text="Pohon 1";mGrid->Cells[1][1].Text="NAV";mGrid->Cells[0][1].Align=mGrid->LEFT;
			mGrid->Cells[0][2].Text="Pohon 2";mGrid->Cells[1][2].Text="ION";mGrid->Cells[0][2].Align=mGrid->LEFT;
																				mGrid->Cells[1][3].Text="CO2";
																				mGrid->Cells[1][4].Text="LAK";

			//nastavení velikosti sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show(), ale lépe pøed merge
			mGrid->SetColumnAutoFit(-1);

			mGrid->MergeCells(0,2,0,4);//slouèení
			//volby
			mGrid->Cells[2][1].Align=mGrid->CENTER;
			mGrid->Cells[2][1].Type=mGrid->CHECK;mGrid->Cells[4][1].Type=mGrid->CHECK;mGrid->MergeCells(2,1,3,1);mGrid->MergeCells(4,1,5,1);//slouèení
			mGrid->Cells[2][2].Type=mGrid->CHECK;mGrid->Cells[4][2].Type=mGrid->CHECK;mGrid->MergeCells(2,2,3,2);mGrid->MergeCells(4,2,5,2);
			mGrid->Cells[2][3].Type=mGrid->CHECK;mGrid->Cells[4][3].Type=mGrid->CHECK;mGrid->MergeCells(2,3,3,3);mGrid->MergeCells(4,3,5,3);
			mGrid->Cells[2][4].Type=mGrid->CHECK;mGrid->Cells[4][4].Type=mGrid->CHECK;mGrid->MergeCells(2,4,3,4);mGrid->MergeCells(4,4,5,4);

			//rozdìlení sekcí
			mGrid->Cells[5][1].RightBorder->Width=mGrid->Cells[5][0].RightBorder->Width=2;
			mGrid->SetCells(mGrid->Cells[5][1],5,2,5,3);
			mGrid->Cells[5][4].RightBorder->Width=mGrid->Cells[5][1].RightBorder->Width;
		}break;
	}
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
	//ShowMessage("Došlo ke kliku v tabulce: "+AnsiString(Tag)+", na buòce: "+AnsiString(Col)+","+AnsiString(Row));
	//ShowMessage(mGrid->getRadio(Col,Row)->Name);
	//mGrid->HighlightCell(Col,Row);
	//FormPaint(this);//zajistí pøekreslení bez probliku
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
void __fastcall TForm_gapo::scGPButton_OKClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_gapo::Button1Click(TObject *Sender)
{
	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);
}
//---------------------------------------------------------------------------



