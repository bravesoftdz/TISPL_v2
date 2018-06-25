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
	//nastaven� barvy formul��e
	Form_gapo->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,Form_gapo,1,1);
}
//---------------------------------------------------------------------------
void __fastcall TForm_gapo::FormActivate(TObject *Sender)
{
	//definice tabulky
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=1;//ID tabulky
	mGrid->Left=10;mGrid->Top=scGPPanel_hlavicka->Height+10;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;

	switch(gapoTyp)
	{
		//TAKT TIME
		case TT:
		{

		}break;
		////VOZ�K
		case V:
		{

		}break;
		////R - z�le�itosti
		case R:
		{
			////samotn� vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�
			bool *pohony_zmena=new bool[F->d.v.POHONY->predchozi->n];//vytvo�� pole bool dle po�tu pohon�, ka�dy pohon m��e b�t nastaven na true �i false, �e byl �i nebyl zm�n�n
			pohony_zmena[0]=true;pohony_zmena[1]=true;pohony_zmena[2]=false;
			unsigned long RowCount=5;

			unsigned long ColCount=14;
			mGrid->Create(ColCount,RowCount);//vhodn� jako t�et�

			////pln�n� daty - hlavi�ka
			mGrid->Cells[0][0].Text="pouze zm�n�n� pohony";
			mGrid->Cells[0][0].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
			mGrid->Cells[0][0].Font->Orientation=900;
			mGrid->Cells[0][0].Align=mGrid->CENTER;
			mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
			mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=10;

			mGrid->Cells[1][0].Text="ovlivn�n� techn. objekty";
			mGrid->Cells[2][0].Text="RD,DD,M,P";
			mGrid->Cells[3][0].Text="K,CT";
			mGrid->Cells[4][0].Text="CT,RD,K,P,M";
			mGrid->Cells[5][0].Text="DD";
			mGrid->Cells[6][0].Text="CT - Technologick� �as"; //mGrid->MergeCells(6,0,7,0);//slou�en�
			mGrid->Cells[7][0].Text="RD - Rychlost pohonu";
			mGrid->Cells[8][0].Text="DD - D�lka objekt";
			mGrid->Cells[9][0].Text="K - Kapacita";
			mGrid->Cells[10][0].Text="P - Pozice";
			mGrid->Cells[11][0].Text="M - mezera (jig)";
			mGrid->Cells[12][0].Text="M - mezera (voz�k)";
			mGrid->Cells[13][0].Text="Rotace";

			//p�i�ad� cele oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky
			mGrid->SetCells(mGrid->Cells[0][0],0,0,ColCount-1,0);
			//v tomto p��pad� �ed� sloupec (stejn� jako or�mov�n�) jako nem�n� hodnoty
			//mGrid->Cells[ColCount-1][1].Background->Color=mGrid->Cells[ColCount-1][1].TopBorder->Color;
			//mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);


			//manualfit v��ky 0t�ho ��dku (zat�m nen� p�ipravena metoda)
			Canvas->Font=mGrid->Cells[6][0].Font;	//nejdel�� pou�it� text
			mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[1][0].Text)+mGrid->Cells[1][0].BottomMargin+mGrid->Cells[1][0].BottomBorder->Width/2+mGrid->Cells[1][0].TopMargin+mGrid->Cells[1][0].TopBorder->Width/2;

			////jednoliv� ��dky
			//prvn� dva sloupce
			mGrid->Cells[0][1].Text="Pohon 1";mGrid->Cells[1][1].Text="NAV";mGrid->Cells[0][1].Align=mGrid->LEFT;
			mGrid->Cells[0][2].Text="Pohon 2";mGrid->Cells[1][2].Text="ION";mGrid->Cells[0][2].Align=mGrid->LEFT;
																				mGrid->Cells[1][3].Text="CO2";
																				mGrid->Cells[1][4].Text="LAK";

			//nastaven� velikosti sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge
			mGrid->SetColumnAutoFit(-1);

			mGrid->MergeCells(0,2,0,4);//slou�en�
			//volby
			mGrid->Cells[2][1].Align=mGrid->CENTER;
			mGrid->Cells[2][1].Type=mGrid->CHECK;mGrid->Cells[4][1].Type=mGrid->CHECK;mGrid->MergeCells(2,1,3,1);mGrid->MergeCells(4,1,5,1);//slou�en�
			mGrid->Cells[2][2].Type=mGrid->CHECK;mGrid->Cells[4][2].Type=mGrid->CHECK;mGrid->MergeCells(2,2,3,2);mGrid->MergeCells(4,2,5,2);
			mGrid->Cells[2][3].Type=mGrid->CHECK;mGrid->Cells[4][3].Type=mGrid->CHECK;mGrid->MergeCells(2,3,3,3);mGrid->MergeCells(4,3,5,3);
			mGrid->Cells[2][4].Type=mGrid->CHECK;mGrid->Cells[4][4].Type=mGrid->CHECK;mGrid->MergeCells(2,4,3,4);mGrid->MergeCells(4,4,5,4);

			//rozd�len� sekc�
			mGrid->Cells[5][1].RightBorder->Width=mGrid->Cells[5][0].RightBorder->Width=2;
			mGrid->SetCells(mGrid->Cells[5][1],5,2,5,3);
			mGrid->Cells[5][4].RightBorder->Width=mGrid->Cells[5][1].RightBorder->Width;
		}break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_gapo::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykresl� tabulku
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick
void TForm_gapo::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	//ShowMessage("Do�lo ke kliku v tabulce: "+AnsiString(Tag)+", na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
	//ShowMessage(mGrid->getRadio(Col,Row)->Name);
	//mGrid->HighlightCell(Col,Row);
	//FormPaint(this);//zajist� p�ekreslen� bez probliku
	//toto problik�v� mGrid->Refresh();
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



