//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gapoR.h"
#include "unit1.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TF_gapoR *F_gapoR;
//---------------------------------------------------------------------------
__fastcall TF_gapoR::TF_gapoR(TComponent* Owner)
	: TForm(Owner)
{
	//nastaven� barvy formul��e
	F_gapoR->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,F_gapoR,1,1);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormActivate(TObject *Sender)
{
	////definice tabulky
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=3;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;

	////samotn� vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�
	unsigned long ColCount=14;//pevn� po�et slopc�
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		if(pohony_zmena[i])RowCount+=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
	}
	mGrid->Create(ColCount,RowCount);//vhodn� jako t�et�

	////pln�n� daty - hlavi�ka
	mGrid->Cells[0][0].Text="pouze zm�n�n� pohony";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnut� tu�n�ho p�sma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[1][0].Text="ovlivn�n� techn. objekty";
	mGrid->Cells[2][0].Text="RD,DD,M,P";
	mGrid->Cells[3][0].Text="K,CT";
	mGrid->Cells[4][0].Text="CT,RD,K,P,M";
	mGrid->Cells[5][0].Text="DD";
	mGrid->Cells[6][0].Text="CT - Technologick� �as"; //mGrid->MergeCells(6,0,7,0);//slou�en� zat�m nefunguje dob�e
	mGrid->Cells[7][0].Text="RD - Rychlost pohonu";
	mGrid->Cells[8][0].Text="DD - D�lka objekt";
	mGrid->Cells[9][0].Text="K - Kapacita";
	mGrid->Cells[10][0].Text="P - Pozice";
	mGrid->Cells[11][0].Text="M - mezera (jig)";
	mGrid->Cells[12][0].Text="M - mezera (voz�k)";
	mGrid->Cells[13][0].Text="Rotace";

	////p�i�ad� cel� oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro prvn� ��dek
	//v tomto p��pad� �ed� sloupec (stejn� jako or�mov�n�) jako nem�n� hodnoty - nedo�e�en� probl�m s posunem v AA
	mGrid->Cells[ColCount-1][1].Background->Color=(TColor)RGB(240,240,240);
	mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);


	//manualfit v��ky 0-t�ho ��dku (zat�m nen� p�ipravena metoda)
	Canvas->Font=mGrid->Cells[1][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[1][0].Text)+mGrid->Cells[1][0].BottomMargin+mGrid->Cells[1][0].BottomBorder->Width/2+mGrid->Cells[1][0].TopMargin+mGrid->Cells[1][0].TopBorder->Width/2;
	//manualfit ���ky sloupc� mimo prvn�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=23;//ostatn� n�sleduj�c� sloupce zat�m default ���ka

	////jednoliv� ��dky
//	//prvn� dva sloupce - zat�m jenom ilustrativn� uk�zka + test (pozd�ji bude pln�no "dotazy")
//	mGrid->Cells[0][1].Text="Pohon 1";mGrid->Cells[1][1].Text="NAV";mGrid->Cells[0][1].Align=mGrid->LEFT;
//	mGrid->Cells[0][2].Text="Pohon 2";mGrid->Cells[1][2].Text="ION";mGrid->Cells[0][2].Align=mGrid->LEFT;
//																		mGrid->Cells[1][3].Text="CO2";
//																		mGrid->Cells[1][4].Text="LAK";
	//for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		//if(pohony_zmena[i])RowCount+=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
		for(unsigned long j=1;j<=ColCount;i++)
		{
      mGrid->Cells[1][j].Text="OBJ"+AnsiString(i);
			mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
    }
	}

	//nastaven� velikosti sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
	mGrid->SetColumnAutoFit(0);

//	//slou�en� bun�k
//	mGrid->MergeCells(0,2,0,4);//slou�en� - zat�m jenom ilustrativn� uk�zka + test (pozd�ji bude pln�no "dotazy")
//	//volby
//	mGrid->Cells[2][1].Type=mGrid->CHECK;mGrid->Cells[4][1].Type=mGrid->CHECK;mGrid->MergeCells(2,1,3,1);mGrid->MergeCells(4,1,5,1);//slou�en�
//	mGrid->Cells[2][2].Type=mGrid->CHECK;mGrid->Cells[4][2].Type=mGrid->CHECK;mGrid->MergeCells(2,2,3,2);mGrid->MergeCells(4,2,5,2);
//	mGrid->Cells[2][3].Type=mGrid->CHECK;mGrid->Cells[4][3].Type=mGrid->CHECK;mGrid->MergeCells(2,3,3,3);mGrid->MergeCells(4,3,5,3);
//	mGrid->Cells[2][4].Type=mGrid->CHECK;mGrid->Cells[4][4].Type=mGrid->CHECK;mGrid->MergeCells(2,4,3,4);mGrid->MergeCells(4,4,5,4);
//	//mGrid->MergeCells(2,1,3,4);mGrid->MergeCells(4,1,5,4);

	//rozd�len� sekc� svisl�m or�mov�n�m
	mGrid->Cells[5][1].RightBorder->Width=mGrid->Cells[5][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[5][1],5,2,5,RowCount-2);
	mGrid->Cells[5][RowCount-1].RightBorder->Width=mGrid->Cells[5][1].RightBorder->Width;

	////autoresize formu_gapo, vhodn� nakonec p�ed Show
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10;// + 10 offset okolo tla��tka
	scGPButton_OK->Top=Height-10-scGPButton_OK->Height;
	scGPButton_OK->Left=Width/2-scGPButton_OK->Width/2;
	Button1->Top=scGPButton_OK->Top;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykresl� tabulku
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick
void TF_gapoR::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	//ShowMessage("Do�lo ke kliku v tabulce: "+AnsiString(Tag)+", na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
//	TscGPCheckBox  *CH=mGrid->getRadio(Col,Row);
//	CH->Checked;
//	CH=mGrid->getRadio(i,Row);
//	CH=NULL;delete CH;
	//ShowMessage(mGrid->getRadio(Col,Row)->Name);
	//mGrid->HighlightCell(Col,Row);
	//mGrid->HighlightCell(Col,Row,mGrid->DefaultCell.TopBorder->Color,1);//zru�en� highlight
	//FormPaint(this);//zajist� p�ekreslen� bez probliku
	//toto problik�v� (a tedy nepou��vat): mGrid->Refresh();
}
//---------------------------------------------------------------------------
void TF_gapoR::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TF_gapoR::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::scGPButton_OKClick(TObject *Sender)
{
	pohony_zmena=NULL;delete pohony_zmena;
	Close();
}
//---------------------------------------------------------------------------
//provizorn�, vy/zap�n�n� AA
void __fastcall TF_gapoR::Button1Click(TObject *Sender)
{
	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);
}
//---------------------------------------------------------------------------



