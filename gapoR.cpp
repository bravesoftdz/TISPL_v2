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
	//nastavení barvy formuláøe
	F_gapoR->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK
	F->m.designButton(scGPButton_OK,F_gapoR,1,1);

	Offset=10;//odsazení thoto formu po všech stránách od tabulky
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormActivate(TObject *Sender)
{
	////definice tabulky
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=3;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;

	////samotné vytvoøení tabulky s požadovaným poètem sloupcù a øádkù
	unsigned long ColCount=14;//pevný poèet slopcù
	unsigned long RowCount=1;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		if(pohony_zmena[i])RowCount+=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
	}
	mGrid->Create(ColCount,RowCount);//vhodné jako tøetí

	////plnìní daty - hlavièka
	mGrid->Cells[0][0].Text="pouze zmìnìné pohony";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnutí tuèného písma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[1][0].Text="ovlivnìné techn. objekty";
	mGrid->Cells[2][0].Text="RD,DD,M,P";
	mGrid->Cells[3][0].Text="K,CT";
	mGrid->Cells[4][0].Text="CT,RD,K,P,M";
	mGrid->Cells[5][0].Text="DD";
	mGrid->Cells[6][0].Text="CT - Technologický èas"; //mGrid->MergeCells(6,0,7,0);//slouèení zatím nefunguje dobøe
	mGrid->Cells[7][0].Text="RD - Rychlost pohonu";
	mGrid->Cells[8][0].Text="DD - Délka objekt";
	mGrid->Cells[9][0].Text="K - Kapacita";
	mGrid->Cells[10][0].Text="P - Pozice";
	mGrid->Cells[11][0].Text="M - mezera (jig)";
	mGrid->Cells[12][0].Text="M - mezera (vozík)";
	mGrid->Cells[13][0].Text="Rotace";

	////pøiøadí celé oblasti bunìk totožné vlastnosti jako u referenèní buòky
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro první øádek
	//v tomto pøípadì šedý sloupec (stejný jako orámování) jako nemìné hodnoty - nedoøešený problém s posunem v AA
	mGrid->Cells[ColCount-1][1].Background->Color=(TColor)RGB(240,240,240);
	mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);


	//manualfit výšky 0-tého øádku (zatím není pøipravena metoda)
	Canvas->Font=mGrid->Cells[1][0].Font;	//nejdelší použitý text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[1][0].Text)+mGrid->Cells[1][0].BottomMargin+mGrid->Cells[1][0].BottomBorder->Width/2+mGrid->Cells[1][0].TopMargin+mGrid->Cells[1][0].TopBorder->Width/2;
	//manualfit šíøky sloupcù mimo prvního (ten je øešen automaticky níže pomocí SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=23;//ostatní následující sloupce zatím default šíøka

	////jednolivé øádky
//	//první dva sloupce - zatím jenom ilustrativní ukázka + test (pozdìji bude plnìno "dotazy")
//	mGrid->Cells[0][1].Text="Pohon 1";mGrid->Cells[1][1].Text="NAV";mGrid->Cells[0][1].Align=mGrid->LEFT;
//	mGrid->Cells[0][2].Text="Pohon 2";mGrid->Cells[1][2].Text="ION";mGrid->Cells[0][2].Align=mGrid->LEFT;
//																		mGrid->Cells[1][3].Text="CO2";
//																		mGrid->Cells[1][4].Text="LAK";
	//for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		//if(pohony_zmena[i])RowCount+=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
		for(unsigned long j=1;j<=ColCount;i++)
		{
      mGrid->Cells[1][j].Text="OBJ"+AnsiString(i);
			mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
    }
	}

	//nastavení velikosti sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show(), ale lépe pøed merge metodami
	mGrid->SetColumnAutoFit(0);

//	//slouèení bunìk
//	mGrid->MergeCells(0,2,0,4);//slouèení - zatím jenom ilustrativní ukázka + test (pozdìji bude plnìno "dotazy")
//	//volby
//	mGrid->Cells[2][1].Type=mGrid->CHECK;mGrid->Cells[4][1].Type=mGrid->CHECK;mGrid->MergeCells(2,1,3,1);mGrid->MergeCells(4,1,5,1);//slouèení
//	mGrid->Cells[2][2].Type=mGrid->CHECK;mGrid->Cells[4][2].Type=mGrid->CHECK;mGrid->MergeCells(2,2,3,2);mGrid->MergeCells(4,2,5,2);
//	mGrid->Cells[2][3].Type=mGrid->CHECK;mGrid->Cells[4][3].Type=mGrid->CHECK;mGrid->MergeCells(2,3,3,3);mGrid->MergeCells(4,3,5,3);
//	mGrid->Cells[2][4].Type=mGrid->CHECK;mGrid->Cells[4][4].Type=mGrid->CHECK;mGrid->MergeCells(2,4,3,4);mGrid->MergeCells(4,4,5,4);
//	//mGrid->MergeCells(2,1,3,4);mGrid->MergeCells(4,1,5,4);

	//rozdìlení sekcí svislým orámováním
	mGrid->Cells[5][1].RightBorder->Width=mGrid->Cells[5][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[5][1],5,2,5,RowCount-2);
	mGrid->Cells[5][RowCount-1].RightBorder->Width=mGrid->Cells[5][1].RightBorder->Width;

	////autoresize formu_gapo, vhodné nakonec pøed Show
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10;// + 10 offset okolo tlaèítka
	scGPButton_OK->Top=Height-10-scGPButton_OK->Height;
	scGPButton_OK->Left=Width/2-scGPButton_OK->Width/2;
	Button1->Top=scGPButton_OK->Top;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykreslí tabulku
}
//---------------------------------------------------------------------------
//test volání pøi onclick
void TF_gapoR::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	//ShowMessage("Došlo ke kliku v tabulce: "+AnsiString(Tag)+", na buòce: "+AnsiString(Col)+","+AnsiString(Row));
//	TscGPCheckBox  *CH=mGrid->getRadio(Col,Row);
//	CH->Checked;
//	CH=mGrid->getRadio(i,Row);
//	CH=NULL;delete CH;
	//ShowMessage(mGrid->getRadio(Col,Row)->Name);
	//mGrid->HighlightCell(Col,Row);
	//mGrid->HighlightCell(Col,Row,mGrid->DefaultCell.TopBorder->Color,1);//zrušení highlight
	//FormPaint(this);//zajistí pøekreslení bez probliku
	//toto problikává (a tedy nepoužívat): mGrid->Refresh();
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
//provizorní, vy/zapínání AA
void __fastcall TF_gapoR::Button1Click(TObject *Sender)
{
	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);
}
//---------------------------------------------------------------------------



