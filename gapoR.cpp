//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "gapoR.h"
#include "unit1.h"
#include "TmGrid.h"
#include "PO_math.h"
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
//zde nevolat dìlá problémy
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormShow(TObject *Sender)
{
	////////definice tabulky////////
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=3;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;//NEW_R

	////////vytvoøení tabulky s požadovaným poètem sloupcù a øádkù////////
	unsigned long ColCount=22;//pevný poèet slopcù  //NEW_R
	unsigned long RowCount=1;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
	if(pohony_zmena!=NULL)
	{
		for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
		{
			if(pohony_zmena[i].X)
			{                                                                 //vrátí pouze KK objekty
				pohony_zmena[i].Y=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1);
				RowCount+=pohony_zmena[i].Y;
			}
		}
	}
	mGrid->Create(ColCount,RowCount);//samotné vytvoøení matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty

	////////plnìní daty - hlavièka////////  //NEW_R
	mGrid->Cells[0][0].Text="pouze zmìnìné pohony";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnutí tuèného písma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[1][0].Text="ovlivnìné techn. objekty";
	mGrid->Cells[2][0].Text="CT,RD,K,P,M";
	mGrid->Cells[3][0].Text="DD";
	mGrid->Cells[4][0].Text="RD,DD,P,M";
	mGrid->Cells[5][0].Text="CT,K";
	mGrid->Cells[6][0].Text="CT - Technologický èas [s]";
	mGrid->Cells[8][0].Text="RD - Rychlost pohonu [m/min]";
	mGrid->Cells[10][0].Text="DD - Délka objekt [m]";
	mGrid->Cells[12][0].Text="K - Kapacita [vozíkù+mezer]";
	mGrid->Cells[14][0].Text="P - Pozice [vozíkù]";
	mGrid->Cells[16][0].Text="M - mezera jig [m]";
	mGrid->Cells[18][0].Text="M - mezera vozík [m]";
	mGrid->Cells[20][0].Text="Rotace";

	////////pøiøadí celé oblasti bunìk totožné vlastnosti jako u referenèní buòky////////
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro první øádek

	//pøiøazení jiného fontu, nemìným položkám
	mGrid->Cells[3][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[5][0].Font->Color=(TColor)RGB(128,128,128);

	//v tomto pøípadì šedý sloupec (stejný jako orámování) jako nemìné hodnoty - nedoøešený problém s posunem v AA
//	mGrid->Cells[ColCount-1][1].Background->Color=(TColor)RGB(240,240,240);
//	mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);

	//manualfit výšky 0-tého øádku (zatím není pøipravena metoda)
	unsigned short n=8;//èíslo sloupce s nejdelším textem hlavièky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdelší použitý text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
	//manualfit šíøky sloupcù mimo prvního (ten je øešen automaticky níže pomocí SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=23;//ostatní následující sloupce zatím default šíøka

	//nastavení velikosti nultého sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show(), ale lépe pøed merge metodami
	mGrid->SetColumnAutoFit(0);

	//slouèení bunìk hlavièky PO  - vhodné za SetColumnAutoFit umístít - NEW_R
	mGrid->MergeCells(6,0,7,0);mGrid->MergeCells(8,0,9,0);mGrid->MergeCells(10,0,11,0);mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);mGrid->MergeCells(20,0,21,0);

	////////jednolivé øádky////////
	unsigned long j=1;//èíslo aktuálnì zpracovávaného øádku, musí zaèínat 1, 0 - je hlavièka
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		if(pohony_zmena[i].X)//vypisuje pouze použité pohony, toto vyhodit,pokud budu chtít vypsat všechny pohony a potom je bude totožné s i...
		{
			//vratí formou ukazatelem na pole objekty pøiøazené k danému pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i,1);
			unsigned int z=0;
			for(;z<pohony_zmena[i].Y;z++)
			{
				//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;
				//volby - checkboxy
				mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
				mGrid->MergeCells(2,j,3,j);mGrid->MergeCells(4,j,5,j);//slouèení sloupcù
				//parametry objektù                                               //NEWR
				mGrid->Cells[6][j].Text=O[z].CT;																	mGrid->Cells[6][j].Align=mGrid->LEFT;	mGrid->Cells[7][j].Align=mGrid->LEFT;
				mGrid->Cells[8][j].Text=O[z].RD*60.0;                             mGrid->Cells[8][j].Align=mGrid->LEFT;	mGrid->Cells[9][j].Align=mGrid->LEFT;
				mGrid->Cells[10][j].Text=O[z].delka_dopravniku;                   mGrid->Cells[10][j].Align=mGrid->LEFT;mGrid->Cells[11][j].Align=mGrid->LEFT;
				mGrid->Cells[12][j].Text=O[z].kapacita;                           mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[13][j].Align=mGrid->LEFT;
				mGrid->Cells[14][j].Text=O[z].pozice;                             mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Align=mGrid->LEFT;
				mGrid->Cells[16][j].Text=O[z].mezera_jig;                         mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Align=mGrid->LEFT;
				if(O[z].mezera_podvozek<0.00000004) mGrid->Cells[18][j].Text="0";
				else mGrid->Cells[18][j].Text=O[z].mezera_podvozek;               mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Align=mGrid->LEFT;
				mGrid->Cells[20][j].Text=O[z].rotace;                             mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Align=mGrid->LEFT;
				calculate(j);//zajistí pøepoèet daného øádku - nových hodnot NEWR
				//posun na další øádek výsledné tabulky
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+pohony_zmena[i].Y-1);//slouèení bunìk pohony
			O=NULL;delete O;
		}
	}

	////////rozdìlení sekcí svislým orámováním////////
	mGrid->Cells[5][1].RightBorder->Width=mGrid->Cells[5][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[5][1],5,2,5,RowCount-2);
	mGrid->Cells[5][RowCount-1].RightBorder->Width=mGrid->Cells[5][1].RightBorder->Width;

	////////autoresize formu_gapo, vhodné nakonec,tj. pøed Show////////
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10;// + 10 offset okolo tlaèítka
	scGPButton_OK->Top=Height-10-scGPButton_OK->Height;
	scGPButton_OK->Left=Width/2-scGPButton_OK->Width/2;
	Button1->Top=scGPButton_OK->Top;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykreslí tabulku
	F->m.frameForm(this,clWebOrange,1);//vykreslí orámování okolo formu
}
//---------------------------------------------------------------------------
//test volání pøi onclick
void TF_gapoR::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	calculate(Row);//zajistí pøepoèet daného øádku NEWR
	FormPaint(this);//zajistí pøekreslení bez probliku

//pouze vzor:
//	ShowMessage("Došlo ke kliku v tabulce: "+AnsiString(Tag)+", na buòce: "+AnsiString(Col)+","+AnsiString(Row));
//	TscGPCheckBox *CH=mGrid->getRadio(Col,Row);
//	if(CH->Checked)....
//	CH=mGrid->getRadio(3,i);
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
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] pohony_zmena;
	delete[] objekty;
}
//---------------------------------------------------------------------------
//provizorní, vy/zapínání AA
void __fastcall TF_gapoR::Button1Click(TObject *Sender)
{
	ShowMessage(objekty[1].pohon->n);//pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);
}
//---------------------------------------------------------------------------
//pro daný øádek dle nastaveného checkboxu, dopoèítá a dosadí nové hodnoty parametrù daného objektu z daného øádku
void TF_gapoR::calculate(unsigned long Row)
{
	//instance na PO_math, využívá se stejných výpoètù
	TPO_math pm;

	//input sekce
	pm.TT=F->d.v.PP.TT;
	pm.rezim=objekty[Row].rezim;
	pm.CT=objekty[Row].CT;
	pm.RD=objekty[Row].RD;
	pm.DD=objekty[Row].delka_dopravniku;
	pm.K=objekty[Row].kapacita;
	pm.P=objekty[Row].pozice;
	pm.M=objekty[Row].mezera;
	pm.MJ=objekty[Row].mezera_jig;
	pm.MP=objekty[Row].mezera_podvozek;
	pm.dJ=F->d.v.PP.delka_jig;
	pm.sJ=F->d.v.PP.sirka_jig;
	pm.dP=F->d.v.PP.delka_podvozek;
	pm.Rotace=objekty[Row].rotace;
	pm.R=objekty[Row].pohon->roztec;

	//volání samotného výpoètu dle volby stanovéné pomoci checkboxu
	if(mGrid->getCheck(2,Row)->Checked)//mìní se CT,RD,K,P,M, zùstává DD
	{
		pm.CT_locked=false;pm.DD_locked=true;
		pm.RD=objekty[Row].pohon->aRD;
		pm.input_RD(true);
	}
	else//mìní se RD,DD,P,M, zùstává CT,K
	{
		pm.CT_locked=false;pm.DD_locked=true;
		pm.RD=objekty[Row].pohon->aRD;
		pm.input_RD(true);
	}

	//output sekce
	mGrid->Cells[7][Row].Text=pm.CT;
	mGrid->Cells[9][Row].Text=pm.RD*60.0;
	mGrid->Cells[11][Row].Text=pm.DD;
	mGrid->Cells[13][Row].Text=pm.K;
	mGrid->Cells[15][Row].Text=pm.P;
	mGrid->Cells[17][Row].Text=pm.MJ;
	mGrid->Cells[19][Row].Text=pm.MP;
}
//---------------------------------------------------------------------------

