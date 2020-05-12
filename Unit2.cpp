//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "vykresli.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scGPImages"
#pragma link "scImageCollection"
#pragma resource "*.dfm"
TForm2 *Form2;
#include "antialiasing.h"
#include "unit1.h"
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
	AA=true;
	puvX=-100,puvY=-100;
	aktX=-100,aktY=-100;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormShow(TObject *Sender)
{
		////////E0
		TElement *E=new TElement;

		E->mG=new TmGrid(this);//v�dy nutno jako prvn�
		E->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
		E->mG->ID=0;
		E->mG->Left=100;E->mG->Top=50;//hodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
		E->mG->AntiAliasing_text=true;
		E->mG->Border.Width=2;
///////////////////
//rosta
   E->mG->Create(5, 6);
		int i=0;
		E->mG->Cells[1][i+2].Text = " po�et";
		E->mG->Cells[1][i+3].Text = " pr�zdn�ch";
		E->mG->Cells[1][i+4].Text = " celkem";
		E->mG->Cells[2][i+2].Type = mGrid->EDIT; //
		E->mG->Cells[2][i+3].Type = mGrid->EDIT;
		E->mG->Cells[2][i+4].Type = mGrid->EDIT;
		E->mG->Cells[2][i+2].Text = "100"; // value
		E->mG->Cells[2][i+3].Text = "3"; // value
		E->mG->Cells[2][i+4].Text = "103"; // value

		E->mG->Cells[3][i+2].Type = mGrid->EDIT; //
		E->mG->Cells[3][i+3].Type = mGrid->EDIT;
		E->mG->Cells[3][i+4].Type = mGrid->EDIT;
		E->mG->Cells[3][i+2].Text = "100"; // value
		E->mG->Cells[3][i+3].Text = "3"; // value
		E->mG->Cells[3][i+4].Text = "103"; // value

		E->mG->Cells[4][i+3].Type = mGrid->glyphBUTTON; //
//		E->mG->Cells[4][i+3].Type = mGrid->BUTTON;
//		E->mG->Cells[4][i+4].Type = mGrid->BUTTON;
		//E->mG->MergeCells(4,2,4,4);E->mG->Cells[4][2].Valign=E->mG->MIDDLE;

////////////////////

		//E->mG->scGPImageCollection=scGPImageCollection1;
		//test exBUTTONU
		//E->mG->exBUTTONVisible=true;
		//E->mG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;

		//velikost tabulky
//		unsigned long ColCount=3;//pevn� po�et slopc�
//		unsigned long RowCount=3;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
//
//		podm�n�n� form�tov�n�
//		E->mG->DefaultCell.isZero->Color=clGreen;
//		E->mG->DefaultCell.isEmpty->Color=F->m.clIntensive(clRed,230);
//
//		E->mG->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
//		E->mG->Cells[0][0].Text="0";E->mG->Cells[0][0].Type=E->mG->EDIT;
//		E->mG->Cells[1][0].Text="5,555";E->mG->Cells[1][0].Type=E->mG->EDIT;
//		E->mG->Cells[0][1].Type=E->mG->DRAW;
//		E->mG->Cells[0][1].Text="za��tek <a>[m]</a>"; E->mG->Cells[0][1].Hint="test hintu";E->mG->Cells[0][1].ShowHint=true;
//		E->mG->Cells[0][1].isLink->Color=clRed;
//		E->mG->Cells[1][1].Type=E->mG->EDIT;
//		E->mG->Cells[1][1].InputNumbersOnly=true;
//		E->mG->Cells[1][2].Type=E->mG->EDIT;
		//E->mG->Cells[0][0].Text="0";
		//E->mG->Cells[0][0].Type=E->mG->EDIT;
//		E->mG->Cells[0][0].Type=E->mG->glyphBUTTON;
//		E->mG->Cells[0][0].Text="1";
//		E->mG->Cells[0][1].Text="2";
//		E->mG->Cells[0][2].Text="3";
//		E->mG->Cells[0][4].Text="4";
//		E->mG->Cells[0][5].Text="5";

//		//E->mG->Cells[0][0].Type=E->mG->EDIT;
//		E->mG->Cells[0][1].Type=E->mG->EDIT;
//		E->mG->Cells[0][2].Type=E->mG->EDIT;
//
//		//E->mG->Cells[1][0].Type=E->mG->EDIT;
//		E->mG->Cells[1][1].Type=E->mG->EDIT;
//		E->mG->Cells[1][2].Type=E->mG->EDIT;
//
//	 ///	E->mG->Cells[2][0].Type=E->mG->EDIT;
//		E->mG->Cells[2][1].Type=E->mG->EDIT;
//		E->mG->Cells[2][2].Type=E->mG->EDIT;

//		E->mG->Cells[0][4].Type=E->mG->EDIT;
//		E->mG->Cells[0][5].Type=E->mG->EDIT;


		//test obr�zku
		//E->mG->Cells[1][2].Type=E->mG->IMAGE;E->mG->Cells[1][2].ImageIndex=0;
		//E->mG->Cells[2][2].Type=E->mG->IMAGE;E->mG->Cells[2][2].ImageIndex=1;

		//E->mG->Columns[0].Width=800;
		//E->mG->SetColumnAutoFit(0);

		//E->mG->Note.Text="Text v�pisu pozn�mky pod �i nad �arou a <a>link</a> n�jak� dal�� abcdefgeijasdfads dafs";
		//E->mG->ShowNote("Text v�pisu pozn�mky pod �i nad �arou a <a>link</a> n�jak� dal�� abcdefgeijasdfads dafs");

		E->predchozi=NULL;
		E->dalsi=NULL;

		ELEMENTY=E;




//		////////E1
//		TElement *E1=new TElement;
//
//		E1->mG=new TmGrid(this);//v�dy nutno jako prvn�
//		E1->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
//		E1->mG->ID=1;
//		E1->mG->Left=200;E1->mG->Top=200;//hodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
//		E1->mG->AntiAliasing_text=true;
//		//E1->mG->MovingTable=true;
//		E1->mG->Border.Width=2;
//
//
//		ColCount=2;//pevn� po�et slopc�
//		RowCount=6;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
//
//		E1->mG->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
////		E1->mG->Columns[1].Width=50;
//		E1->mG->SetColumnAutoFit(-4);
//
//		E1->mG->Cells[0][0].Type=E1->mG->COMBO;E1->mG->Cells[0][0].Text="hlavi�ka";
//
//
//		E1->mG->Cells[0][1].Type=E1->mG->EDIT;E1->mG->Cells[0][1].Text=1;//E1->mG->Cells[1][1].Text=1;E1->mG->Cells[1][1].Type=E1->mG->EDIT;
//		E1->mG->Cells[0][2].Type=E1->mG->EDIT;E1->mG->Cells[0][2].Text=2;E1->mG->Cells[1][2].Text=2;E1->mG->Cells[1][2].Type=E1->mG->EDIT;
//		E1->mG->Cells[0][3].Type=E1->mG->EDIT;E1->mG->Cells[0][3].Text=3;E1->mG->Cells[1][3].Text=3;E1->mG->Cells[1][3].Type=E1->mG->EDIT;
//		E1->mG->Cells[0][4].Type=E1->mG->EDIT;E1->mG->Cells[0][4].Text=4;E1->mG->Cells[1][4].Text=4;E1->mG->Cells[1][4].Type=E1->mG->EDIT;
//		E1->mG->Cells[0][5].Type=E1->mG->EDIT;E1->mG->Cells[0][5].Text=5;E1->mG->Cells[1][5].Text=5;E1->mG->Cells[1][5].Type=E1->mG->EDIT;

//		E1->mG->Update();
//		TscGPListBoxItem *t=NULL;
//		t=E1->mG->getCombo(1,3)->Items->Add(/*tady nelze parametr*/);
//		t->Caption="text 1";
//		t=E1->mG->getCombo(1,3)->Items->Add(/*tady nelze parametr*/);
//		t->Caption="text 2";

//		//E1->mG->Cells[0][1].Type=E1->mG->EDIT;
//		E1->mG->Cells[0][0].Text="OOO";
//		E1->mG->Cells[0][0].Type=E1->mG->EDIT;
//		//E1->mG->Cells[0][0].Background->Color=clRed;
//		//E1->mG->Cells[0][0].Font->Name="Roboto Cn";
//		//E1->mG->Cells[0][0].Font->Color=clYellow;
//		//E1->mG->Cells[3][1].Type=E1->mG->BUTTON;
//		//E1->mG->Cells[1][1].Type=E1->mG->EDIT;
//
//		E1->mG->Cells[1][1].Text="abc1";
//
//		//E1->mG->Cells[1][2].Type=E1->mG->NUMERIC;
//		E1->mG->Cells[1][2].Text="0.333";
//		E1->mG->Cells[3][2].Type=E1->mG->EDIT;
//
//		//E1->mG->Cells[1][3].Type=E1->mG->EDIT;
//		E1->mG->Cells[1][3].Text="abc3";
//		E1->mG->Cells[3][3].Type=E1->mG->EDIT;
//
//		//E1->mG->Cells[1][4].Type=E1->mG->EDIT;
//		E1->mG->Cells[1][4].Text="abc4";
//		E1->mG->Cells[3][4].Type=E1->mG->EDIT;
//
//		E1->mG->MergeCells(0,0,1,0);
//		E1->mG->MergeCells(2,0,2,1);
//			E1->mG->MergeCells(0,0,1,0);
//			E1->mG->MergeCells(0,1,1,1);




		//E1->predchozi=NULL;
		//E1->dalsi=NULL;



	 //	ELEMENTY->dalsi=E1;




//	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
//	mGrid->Tag=4;//ID tabulky
//	mGrid->Left=0;mGrid->Top=0;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
//	mGrid->AntiAliasing_text=true;

	//mGrid->DefaultCell.isEmpty->Color=clBlue; //mus� b�t p�ed create, jinak nutno nastavovat konkr�tn� bu�ku
	//mGrid->DefaultCell.isZero->Color=clLime;  //mus� b�t p�ed create, jinak nutno nastavovat konkr�tn� bu�ku
	//mGrid->DefaultCell.isNegativeNumber->Color=clRed; //mus� b�t p�ed create, jinak nutno nastavovat konkr�tn� bu�ku

//	mGrid->Create(3,3);//vhodn� jako t�et�




	//mGrid->Border.Width=1;

//	mGrid->Cells[3][2].Text="text v slou�en�ch bu�k�ch";
//	mGrid->Cells[3][2].Font->Color=clWhite;
//	mGrid->Cells[3][2].Align=mGrid->CENTER;
//	mGrid->Cells[3][2].TopBorder->Color=clBlack;mGrid->Cells[3][2].TopBorder->Width=3;
//	*mGrid->Cells[3][2].LeftBorder=*mGrid->Cells[3][2].RightBorder=*mGrid->Cells[3][2].BottomBorder=*mGrid->Cells[3][2].TopBorder;//v tomto ��dku uk�zka p�eb�r�n� vlastnostn� or�mov�n�, jinak samoz�ejm� mo�no zapsat standardn� (a plnit)
//	mGrid->Cells[3][2].Background->Color=clLime;


	//toto propojuje or�mov�n� bun�k ukazatelov�, a to je v�t�inou ne��douc� pou�iv l�pe metody
	//mGrid->Cells[1][2]=mGrid->Cells[2][3]

//	mGrid->Rows[1].Height=2*mGrid->DefaultRowHeight;
//	mGrid->Rows[5].Height=100;
//
//	mGrid->Cells[1][3].Background->Color=clRed;
//	mGrid->Cells[1][3].Font->Color=clBlue;
//	mGrid->Cells[1][3].Align=mGrid->RIGHT;
//	mGrid->Cells[1][3].Valign=mGrid->TOP;
//	mGrid->Cells[1][3].Text="n�jak� dlouh� text";
//
//	mGrid->Cells[7][3].Font->Orientation=900;
//	mGrid->Cells[7][3].Font->Color=clBlue;
//	mGrid->Cells[7][3].Text="Orotovan� text";
//	mGrid->HighlightCell(7,3,clBlue,1);
//	mGrid->Cells[7][3].Align=mGrid->CENTER;
//	mGrid->Cells[7][3].Valign=mGrid->BOTTOM;
//	mGrid->Cells[7][3].TopMargin=0;
//																						mGrid->Cells[0][1].Text="abcdef";
//	 mGrid->Cells[1][0].Type=mGrid->DRAW; mGrid->Cells[1][0].Text="0";
//	 mGrid->Cells[1][1].Type=mGrid->DRAW; mGrid->Cells[1][1].Text="-1";
//	 mGrid->Cells[1][2].Type=mGrid->DRAW; mGrid->Cells[1][2].Text="";
//	 mGrid->Cells[1][2].isNegativeNumber->Color=clRed;
//	 mGrid->Cells[1][2].isEmpty->Color=clBlue;
//	 mGrid->Cells[1][2].isZero->Color=clYellow;
//	 mGrid->Cells[1][3].Type=mGrid->EDIT; mGrid->Cells[1][3].Text="3";
//	 mGrid->Cells[1][4].Type=mGrid->EDIT; mGrid->Cells[1][4].Text="4";
//	 mGrid->Cells[1][5].Type=mGrid->EDIT; mGrid->Cells[1][5].Text="5";
//	 mGrid->Cells[1][6].Type=mGrid->EDIT; mGrid->Cells[1][6].Text="6";
//	 mGrid->Cells[1][7].Type=mGrid->EDIT; mGrid->Cells[1][7].Text="7";
//	 mGrid->Cells[1][8].Type=mGrid->EDIT; mGrid->Cells[1][8].Text="8";
//	 mGrid->Cells[1][9].Type=mGrid->EDIT; mGrid->Cells[1][9].Text="9";

//	mGrid->Cells[1][5].Type=mGrid->CHECK;
//	//mGrid->Cells[1][5].Valign=mGrid->TOP;
//	//mGrid->Cells[1][5].Align=mGrid->LEFT;
//
//	mGrid->Cells[2][5].Type=mGrid->RADIO;
//
//	mGrid->Cells[3][5].Type=mGrid->NUMERIC;
//
//	mGrid->Cells[4][5].Type=mGrid->EDIT;
//	mGrid->Cells[4][5].Text="tady je edit";
//
//	mGrid->Cells[5][5].Type=mGrid->BUTTON;
//	mGrid->Cells[5][5].Text="tla��tko";
//
//	mGrid->Cells[6][5].Text="COMBO";
//	mGrid->Cells[6][5].Type=mGrid->COMBO;

//	//uk�zka p�eb�r�n� vlastnost� spole�n�ho or�mov�n� bun�k
//	mGrid->Cells[8][8].TopBorder->Width=3;
//	mGrid->Cells[8][8].TopBorder->Color=clBlue;
//	mGrid->Cells[8][7].BottomBorder->Color=clWebOrange;
////	if(mGrid->Cells[8][7].BottomBorder->Color==clWebOrange)
////	ShowMessage("clWebOrange");
//	mGrid->Cells[8][8].LeftBorder->Color=clWebOrange;
//	mGrid->Cells[7][8].RightBorder->Color=clBlue;
//
//	//nastav� oblast bun�k na stejn�mi vlastnostmi (vhodn� nap�. na fixed cells, or�mov�n� n�jak� oblasti, disabled/read-only oblast atp.)
//	mGrid->Cells[0][0].Text="Nadpis";
//	mGrid->Cells[0][0].Background->Color=clBtnFace;
//	mGrid->Cells[0][0].Align=mGrid->RIGHT;
//	mGrid->Cells[0][0].Valign=mGrid->TOP;
//	mGrid->SetCells(mGrid->Cells[0][0],0,0,0,7);
//	mGrid->SetCells(mGrid->Cells[0][0],0,0,7,0);


 //	mGrid->SetColumnAutoFit(1);//uk�zka nastaven� velikosti sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show()

	//merge bun�k mus� b�t, co nejbl�e nad Show() a hned po SetColumnAutoFit, pop�. by marge mohl b�t kdekoliv, ale bylo by nutn� po n�m volat FormPaint(this)
	//mGrid->MergeCells(3,2,5,3);
	//mGrid->MergeCells(7,3,8,9);

//	mGrid->AntiAliasing_text=false;//nemu�e b�t varianta  AntiAliasing_text=true; a AntiAliasing_grid=false; (je i o�et�eno)
//	mGrid->AntiAliasing_grid=false;

	//tabulka 2 lok�ln� deklarace TmGrit (v Unit2.h)
//	mGrid2=new TmGrid(this);
//	mGrid2->Tag=2;
//	mGrid2->Top=mGrid->Top+mGrid->Height+10;
//	mGrid2->Left=mGrid->Left+10;
//	mGrid2->Create(3,10);
//	mGrid2->Cells[1][0].Text="0";
//	mGrid2->Cells[1][1].Text="1";
//	mGrid2->Cells[1][2].Text="2";
//	mGrid2->Cells[1][3].Text="3";
//	mGrid2->Cells[1][4].Text="4";
//	mGrid2->Cells[1][5].Text="5";
//	mGrid2->Cells[1][6].Text="6";
//	mGrid2->Cells[1][7].Text="7";
//	mGrid2->Cells[1][8].Text="8";
//	mGrid2->Cells[1][9].Text="9";
//
//	mGrid2->Cells[1][1].Type=mGrid->BUTTON;
//	mGrid2->Cells[1][1].Text="tla��tko";

	TElement *el=new TElement;

	//definice jednotek a ���ek
	AnsiString LO,cas,delka_otoce,rychlost,R,Rz;
	//nastaven� jednotek podle posledn�ch nastaven�
	cas="<a>[min]</a>";//1
	LO="<a>[mm]</a>";//1
	delka_otoce="<a>[m]</a>";//0
	rychlost="<a>m/min</a>";//
	R="y<a>[m1]</a>z";//1
	Rz="a<a>[m2]</a>b";//1
	//vytvo�en� mgridu
	el->mG=new TmGrid(this);
	el->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	el->mG->ID=1;
	//design deklarace
	TColor clHeaderFont=clBlack;
	TColor clBackgroundHidden=(TColor)RGB(240,240,240);//m.clIntensive((TColor)RGB(128,128,128),115);
	TColor clFontLeft = (TColor)RGB(128,128,128);
	//TColor clFontRight = (TColor)RGB(43,87,154);
	//definice fontu a velikosti p�sma
	el->mG->DefaultCell.Font->Name=F->aFont->Name;
	el->mG->DefaultCell.Font->Size=F->aFont->Size;
	el->mG->DefaultCell.isNegativeNumber->Name=F->aFont->Name;
	el->mG->DefaultCell.isNegativeNumber->Size=F->aFont->Size;
	el->mG->DefaultCell.isZero->Name=F->aFont->Name;
	el->mG->DefaultCell.isZero->Size=F->aFont->Size;
	el->mG->DefaultCell.isLink->Name=F->aFont->Name;
	el->mG->DefaultCell.isLink->Size=F->aFont->Size;
	el->mG->DefaultCell.isActiveLink->Name=F->aFont->Name;
	el->mG->DefaultCell.isActiveLink->Size=F->aFont->Size;
	el->mG->Note.Font->Name=F->aFont->Name;
	el->mG->Note.Font->Size=14;
	//ur�en� zda budou prohozeny sloupce
	el->mG->Create(5,12);
	el->mG->Left=500;
	el->mG->Top=300;
	int prvni=3,druhy=4;
	//nastaven� popisk�
	el->mG->Cells[prvni][1].Text="IN";
	el->mG->Cells[druhy][1].Text="OUT";
	el->mG->Cells[0][1].Text=F->ls->Strings[447];//"V�b�r pohonu "
	el->mG->Cells[2][3].Text=F->ls->Strings[451];//"Nastaven�"
	el->mG->Cells[2][4].Text=F->ls->Strings[448];//"Rozmez�"
	el->mG->Cells[2][5].Text=F->ls->Strings[209];//"Palce"
	el->mG->Cells[2][6].Text=F->ls->Strings[210];//"N�sobek"
	el->mG->Cells[2][7].Text=F->ls->Strings[211];//"Jig�"
	el->mG->Cells[2][8].Text=F->ls->Strings[212];//"Podvozky"
	el->mG->Cells[2][9].Text=F->ls->Strings[213];//"Jigy 0�"
	el->mG->Cells[2][10].Text=F->ls->Strings[214];//"Jigy 90�"
	el->mG->Cells[0][11].Text="max WT "+cas;
	//nastaven� slou�en�ch sloupc�
	el->mG->Cells[0][3].Text=F->ls->Strings[452];//+" "+rychlost;//"Rychlost"
	el->mG->Cells[1][3].Text=rychlost;//jednotky
	el->mG->Cells[0][3].Font->Orientation=900;el->mG->Cells[0][3].Valign=TmGrid::MIDDLE;
	el->mG->Cells[1][3].Font->Orientation=900;el->mG->Cells[1][3].Valign=TmGrid::MIDDLE;
	el->mG->Cells[0][5].Text=F->ls->Strings[453];//"Rozte�"
	el->mG->Cells[1][5].Text=R;//jednotky
	el->mG->Cells[0][5].Font->Orientation=900;el->mG->Cells[0][5].Valign=TmGrid::MIDDLE;
	el->mG->Cells[1][5].Font->Orientation=2700;el->mG->Cells[1][5].Valign=TmGrid::MIDDLE;
	el->mG->Cells[0][8].Text=F->ls->Strings[215];//"Mezera"
	el->mG->Cells[1][8].Text=Rz;//jednotky
	el->mG->Cells[0][8].Font->Orientation=900;el->mG->Cells[0][8].Valign=TmGrid::MIDDLE;
	el->mG->Cells[1][8].Font->Orientation=900;el->mG->Cells[1][8].Valign=TmGrid::MIDDLE;
	//nastaven� ���ek
	el->mG->SetColumnAutoFit(-4);
	el->mG->Columns[0].Width=el->mG->Columns[1].Width=el->mG->Rows[0].Height;
	el->mG->Columns[2].Width=90;
	el->mG->Columns[3].Width=145;
	el->mG->Columns[4].Width=145;
	//nastaven� exButtonu, skr�v�n� ��dku max.WT Stop
	el->mG->exBUTTONVisible=true;
	el->mG->exBUTTON->ShowHint=true;
	el->mG->exBUTTON->Hint=F->ls->Strings[231];//"Roz���en� polo�ky";
	//hinty
	el->mG->Cells[2][11].Hint=F->ls->Strings[229];//"maxim�ln� mo�n� doba �ek�n� na palec";
	el->mG->Cells[2][11].ShowHint=true;

	//design tabulky
	int prvni_sloupec=2;
	for(unsigned int i=1;i<=el->mG->RowCount-1;i++)
	{
		//prvn� sloupec
		el->mG->Cells[prvni_sloupec][i].RightMargin = 3;
		el->mG->Cells[prvni_sloupec][i].Align=mGrid->RIGHT;
		el->mG->Cells[prvni_sloupec][i].Font->Color=clFontLeft;
		if(prvni_sloupec>0)el->mG->Cells[0][i].Font->Color=clFontLeft;
		//proch�zen� dal��ch sloupc�
		for(unsigned int j=prvni_sloupec+1;j<=el->mG->ColCount-1;j++)
		{
			if(el->mG->Cells[j][i].Type==el->mG->EDIT && (el->mG->Cells[j][i].Text==0 || F->ms.MyToDouble(el->mG->Cells[j][i].Text)!=0))el->mG->Cells[j][i].InputNumbersOnly=2;
			el->mG->Cells[j][i].RightMargin = 3;
			el->mG->Cells[j][i].Align=mGrid->RIGHT;
			if(F->ms.MyToDouble(el->mG->Cells[j][i].Text)!=0)el->mG->Cells[j][i].Text=F->m.round2double(F->ms.MyToDouble(el->mG->Cells[j][i].Text),3);//p�esko�en� prvn�ho ��dku v tabulce stopky
			if(i!=1 && el->mG->Cells[j][i].Type!=el->mG->EDIT&&el->mG->Cells[j][i].Type!=el->mG->COMBO)
			{
				el->mG->Cells[j][i].Background->Color=clBackgroundHidden;
				el->mG->Cells[j][i].RightMargin=5;
				el->mG->Cells[j][i].Font->Color=clFontLeft;
			}
			if(i==1)el->mG->Cells[j][i].Font->Color=clHeaderFont;
		}
		el->mG->Cells[3][i].LeftBorder->Width=el->mG->Cells[3][i].RightBorder->Width=2;
	}

	//slou�en� bun�k + design pro border
	el->mG->MergeCells(0,0,4,0);//hlavi�ka tabulky
	el->mG->Cells[0][1].Font->Color=clHeaderFont;
	el->mG->Cells[0][1].RightBorder->Width=2;
	el->mG->MergeCells(0,1,2,2);
	el->mG->Cells[0][3].RightBorder->Color=clWhite;
	el->mG->MergeCells(0,3,0,4);el->mG->MergeCells(1,3,1,4);//slou�en� pro rychlost
	el->mG->Cells[1][5].LeftBorder->Color=el->mG->Cells[1][7].LeftBorder->Color=clWhite;
	el->mG->MergeCells(0,5,0,7);el->mG->MergeCells(1,5,1,6);//slou�en� pro rozte�
	el->mG->Cells[0][8].RightBorder->Color=clWhite;
	el->mG->MergeCells(0,8,0,10);el->mG->MergeCells(1,8,1,10);//slou�en� pro mezery
	el->mG->Cells[0][11].RightMargin = 3;
	el->mG->Cells[0][11].Align=mGrid->RIGHT;
	el->mG->Cells[0][11].Font->Color=clFontLeft;
	el->mG->MergeCells(0,11,2,11);

	//vykreslen� horizont�ln�ho rozd�len� sekc�
	for(unsigned int i=0;i<=el->mG->ColCount-1;i++)
	{
		el->mG->Cells[i][3].TopBorder->Width=2;
		el->mG->Cells[i][5].TopBorder->Width=2;
		el->mG->Cells[i][8].TopBorder->Width=2;
		el->mG->Cells[i][11].TopBorder->Width=2;
	}

  	///////////////////////////////Skryt� ��dku///////////////////////////////
	el->mG->Show();//vykresl�n� p�ed skryt�m, zkou�el jsem Update i Refresh
	el->mG->VisibleRow(5,false,false);
	el->mG->VisibleRow(6,false,false);
	el->mG->VisibleRow(7,false,false);
	el->mG->VisibleRow(8,false,false);
	el->mG->VisibleRow(9,false,false);
	el->mG->VisibleRow(10,false,false);
	el->mG->VisibleRow(11,false,false);
	///////////////////////////////Konec skryt� ��dku///////////////////////////////



	el->dalsi=NULL;
	el->predchozi=ELEMENTY;
	ELEMENTY->dalsi=el;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormPaint(TObject *Sender)
{
	ELEMENTY->mG->Show();
	ELEMENTY->dalsi->mG->Show();//vykreslen� tabulky, m��e b�t nekompletn� nap�. jen 3 ��dky
	if(!ELEMENTY->dalsi->mG->Rows[5].Visible)//pokud m� tabulka skryt� ��dky, zobraz� je a znova vykresl� celou tabulku
	{
		ELEMENTY->dalsi->mG->VisibleRow(5,true,false);
		ELEMENTY->dalsi->mG->VisibleRow(6,true,false);
		ELEMENTY->dalsi->mG->VisibleRow(7,true,false);
		ELEMENTY->dalsi->mG->VisibleRow(8,true,false);
		ELEMENTY->dalsi->mG->VisibleRow(9,true,false);
		ELEMENTY->dalsi->mG->VisibleRow(10,true,false);
		ELEMENTY->dalsi->mG->VisibleRow(11,true,false);
		ELEMENTY->dalsi->mG->Show();//znovu vykresl�n� ji� cel� tabulky
	}
	//ELEMENTY->dalsi->mG->Show();

//				Form2->Canvas->Pen->Color=clGreen;
//			Form2->Canvas->Brush->Style=bsSolid; Form2->Canvas->Brush->Color=clGreen;
//			Form2->Canvas->FrameRect(ELEMENTY->mG->Note.CoordinateArea);


	//mGrid2->Show();
	//nastav� formul�� dle velikosti tabulky, mus� b�t a� po Show
	//Form2->Width=mGrid->Width+50+50;
	//Form2->Height=mGrid->Height+50+50;


//	Canvas->Pen->Width=1;
//	Canvas->Pen->Style=psDashDotDot;
//	Canvas->MoveTo(200,200);
//	Canvas->LineTo(400,400);


//	//if(FileName_short(FileName)=="kabina_base_coat.bmp")
//	{
//		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
//		if(FileExists("kabina_base_coat.bmp"))
//		{
//			Graphics::TBitmap *bmp=new Graphics::TBitmap;
//			bmp->LoadFromFile("kabina_base_coat.bmp");
//			long X=10,Y=-10;long double resolution=0.01200428724544480171489817792069;
//			Canvas->StretchDraw(TRect(F->m.L2Px(X),F->m.L2Py(Y),F->m.round(F->m.L2Px(X)+bmp->Width*F->Zoom*resolution/F->m2px),F->m.round(F->m.L2Py(Y)+bmp->Height*F->Zoom*resolution/F->m2px)),bmp);
//			delete(bmp);
//		}
//	}
//
//
//	Cvykresli d;
////	if(AA)
////	{
//		Cantialising a;
//		Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
//		bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vypl�v� z logiky algoritmu antialiasingu
//		F->Zoom*=3;//*3 vypl�v� z logiky algoritmu antialiasingu
//		d.vykresli_robota(bmp_in->Canvas,F->m.L2Px(aktX),F->m.L2Py(aktY),"Robot 1","R1",1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+100+10)*F->Zoom,aktY*F->Zoom,"Robot 2","R2",1,1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+200+20)*F->Zoom,aktY*F->Zoom,"Robot 3","R3",2,1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+300+30)*F->Zoom,aktY*F->Zoom,"Robot 4","R4",3,1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+400+40)*F->Zoom,aktY*F->Zoom,"Robot 5","R5",0,-1);
////		d.vykresli_stopku(bmp_in->Canvas,F->m.L2Px(aktX)+100,F->m.L2Py(aktY),"Stop 1","S1",1,0);
////		d.vykresli_otoc(bmp_in->Canvas,F->m.L2Px(aktX)+200,F->m.L2Py(aktY),"Oto� pasiv","O1",0,0);
////		d.vykresli_otoc(bmp_in->Canvas,(aktX-500)*F->Zoom,aktY*F->Zoom,"Oto� aktiv","O2",1,1);
//		F->Zoom/=3;//navr�cen� zoomu na p�vodn� hodnotu
//		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutn� do samostatn� bmp, kv�li smaz�n� bitmapy vracen� AA
//		bmp_out->Transparent=true;
//		bmp_out->TransparentColor=clWhite;
//		Canvas->Draw(0,0,bmp_out);
//		delete (bmp_out);//velice nutn�
//		delete (bmp_in);//velice nutn�
//	}
//	else
//	{
//		d.vykresli_robota(Canvas,aktX,aktY,"Robot 1","R1",0,1);
//		d.vykresli_stopku(Canvas,(aktX-600)*F->Zoom,aktY*F->Zoom,"Stop 1","S1",1,0);
//		d.vykresli_otoc(Canvas,(aktX-500)*F->Zoom,aktY*F->Zoom,"Oto� aktiv","O2",1,1);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
//	//pln�n� comba
//	TscGPComboBox *C=mGrid->getCombo(6,5);
//	//C->Options->dal�� mo�nosti
//	//C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu  - nepou�ivat pokud je pr�zdn�
//	TscGPListBoxItem *t=C->Items->Add();t->Caption="test1";
//	t=NULL;t=C->Items->Add();t->Caption="test2";
//	C->ItemIndex=1;
//	t=NULL;delete t;
//	C=NULL;delete C;//nejsem si jist� zda pou��vat nebo ne, pokud bude padat, tak vyzkou�et tady zakomentovat
//	//aktualizace comba �i obecn� komponent nepot�ebuje refresh
//
//	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
//	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�

		//AA=!AA;
		//Invalidate();

		//ELEMENTY->mG->Cells[1][1].LeftBorder->Color=(TColor)10114859;
//	 ELEMENTY->mG->Cells[1][1].Highlight=!ELEMENTY->mG->Cells[1][1].Highlight;
//	 ELEMENTY->mG->Cells[0][1].Highlight=!ELEMENTY->mG->Cells[0][1].Highlight;
//	 ELEMENTY->mG->Refresh();


		TscGPImage *I=ELEMENTY->mG->getImage(1,2);
		I->Images=scGPImageCollection1;
		I->ImageIndex=1;

		I=ELEMENTY->mG->getImage(2,2);
		I->Images=scGPImageCollection1;
		I->ImageIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
//	for(unsigned long X=0;X<mGrid->ColCount;X++)
//	ShowMessage(	mGrid->Columns[X].Left);

	//testov�n� checkboxu
	//if(mGrid->getRadio(2,5,this)->Checked)ShowMessage("za�krnuto");
	//else ShowMessage("neza�krtnuto");

	//zm�na velikosti tabulky
	//mGrid->RowCount=20;mGrid->ColCount=3;//zajist� realokaci tabulky (pokud je n�sledn� vol�no invalidate)


	//mGrid->AntiAliasing_grid=!mGrid->AntiAliasing_grid;
	// ELEMENTY->mG->Left+=10;
	//zm�na posunu tabulky
	// ELEMENTY->mG->unHighlightAll();

	//ELEMENTY->mG->Cells[1][2].Text="Vzor.bmp";

	TscGPGlyphButton *H = ELEMENTY->mG->getGlyphButton(0,0);
	H->GlyphOptions->Kind = scgpbgkClose;

	Invalidate();
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick          //pozor metody mus� m�t i znamenkov� longov� Col, Row, kv�li -2 exBUTTOn
void TForm2::OnClick(long Tag,long ID,long Col,long Row)
{
	//ShowMessage("UNIT2\nToto vypisuje metoda OnClick\nDo�lo ke kliku v tabulce tag formu: "+AnsiString(Tag)+", ID tabulky: "+AnsiString(ID)+", na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
	if(Row==-2)
	{
		//ShowMessage("UNIT2\nexBUTTON");
		if(ELEMENTY->mG->Rows[2].Visible)//skr�v�n�
		{
			ELEMENTY->mG->VisibleRow(2,false);
			ELEMENTY->mG->VisibleRow(3,false);
			ELEMENTY->mG->VisibleRow(4,false);
			ELEMENTY->mG->VisibleRow(5,false);
			ELEMENTY->mG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
		}
		else//zobrazov�n�
		{
			ELEMENTY->mG->VisibleRow(2,true);
			ELEMENTY->mG->VisibleRow(3,true);
			ELEMENTY->mG->VisibleRow(4,true);
			ELEMENTY->mG->VisibleRow(5,true);
			ELEMENTY->mG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
		}
	}
//	mGrid->HighlightCell(Col,Row);
//	mGrid->Cells[0][0].Text="test";
//	FormPaint(this);//zajist� p�ekreslen� bez probliku
//	//toto problik�v� mGrid->Refresh();
//if(Col==3)
//{
//	ELEMENTY->dalsi->mG->DeleteRow(Row);
//}

}
//---------------------------------------------------------------------------
void TForm2::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TForm2::OnChange(long Tag,unsigned long Col,unsigned long Row)
{
	//ELEMENTY->mG->Cells[1][2].Text=F->ms.MyToDouble(ELEMENTY->mG->Cells[1][1].Text)*2.0;
	//ELEMENTY->mG->Refresh();
}
//---------------------------------------------------------------------------
void TForm2::OnKeyPress(long Tag,unsigned long Col,unsigned long Row,System::WideChar &Key)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
Button1->SetFocus();//odevzd�n� focusu na neutr�ln� komponentu
ELEMENTY->mG->Delete(); //ELEMENTY->dalsi->mG->Delete();
//	 mGrid->Delete();//pokud chci odstranit a nechci pou��t na dal�� pou�it�
//	 mGrid2->Delete();
//	 mGrid2=NULL; delete mGrid2;
}
//---------------------------------------------------------------------------
//p�iklady
/*
///////////vytvvo�en�
	mGrid=new TmGrid;
	mGrid->Left=600;mGrid->Top=40;
	//myGrid->ColCount=3;myGrid->RowCount=5;//po�et ��dk� a sloupc�
	//myGrid->Create(); lze p��mo vytvo�it n�e
	mGrid->Create(4,8);

	mGrid->Cells[0][0].Text="2,5";
	mGrid->Cells[0][0].Type=mGrid->NUMERIC;

	mGrid->Cells[0][5].Text="Checkbox";
	mGrid->Cells[0][5].Type=mGrid->CHECK;

	mGrid->Cells[1][5].Text="Tla��tko";
	mGrid->Cells[1][5].Type=mGrid->BUTTON;
	mGrid->Cells[1][5].Background->Color=clGreen;

	mGrid->Cells[0][6].Text="Edit enabled";
	mGrid->Cells[0][6].Type=mGrid->EDIT;

	mGrid->Cells[1][6].Text="Edit disabled";
	mGrid->Cells[1][6].Type=mGrid->readEDIT;

	mGrid->Cells[2][5].Text="text";
	mGrid->Cells[2][5].Font->Color=clRed;
	mGrid->Cells[2][5].Background->Color=clGreen;

	mGrid->Cells[0][1].Text="Radio";
	mGrid->Cells[0][1].Type=mGrid->RADIO;

	mGrid->Cells[3][7].Text="COMBO";
	mGrid->Cells[3][7].Type=mGrid->COMBO;

	mGrid->Cells[1][1].Font->Orientation=900;
	mGrid->Cells[1][1].Valign=mGrid->BOTTOM;

	mGrid->SetColumnAutoFit(-1);
	FormPaint(this);//zajist� p�ekreslen� bez probliku

/////////// pr�b�n� zm�na
		 mGrid->HighlightCell(1,5);

		mGrid->getEdit(0,6,this)->Text="JO!";
	mGrid->getEdit(0,6,this)->Font->Color=clRed;
//	TscGPEdit *E=mGrid->getEdit(this,0,6);
//	E->Text="JO!";
//	E=NULL;delete E;

		mGrid->MergeCells(1,1,2,1);
		mGrid->MergeCells(1,1,1,2);

TscGPComboBox *C=mGrid->getCombo(3,7,this);
C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu
TscGPListBoxItem *t=NULL;t=C->Items->Add();t->Caption="test1";
t=NULL;t=C->Items->Add();t->Caption="test2";
C->ItemIndex=1;
C=NULL;delete C;

/////////// zm�na po�tu ��dk� �i sloupc (realokace) zde nefungovalo
		mGrid->Left=500;
		mGrid->ColCount=4;mGrid->RowCount=4;

		mGrid->SetColumnAutoFit(-3);
		FormPaint(this);//zajist� p�ekreslen� bez probliku
*/
//---------------------------------------------------------------------------
void __fastcall TForm2::Button5Click(TObject *Sender)
{
	ELEMENTY->mG->AddRow();
	//test pouze p�id�n� textu:
	//mGrid->Cells[1][mGrid->RowCount-1].Text=mGrid->RowCount-1;FormPaint(this);//nutno kv�li aktualizaci textu
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button4Click(TObject *Sender)
{
	ELEMENTY->mG->InsertRow(3);//s problikem zp�sobuje show() v InsertRow
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button3Click(TObject *Sender)
{
	ELEMENTY->mG->DeleteRow(3);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	 //Memo1->Lines->Add(AnsiString(X)+" "+AnsiString(Y));
	 ELEMENTY->mG->MouseMove(X,Y);//pro Hint a kurzor odkazu, mousemove dan�ho formu sice mo�no odchyt�v�t p��mo v mGridu, ale toto nutnost pro rozli�en� tabulek
	 ELEMENTY->dalsi->mG->MouseMove(X,Y);     FormPaint(this);
	 //ELEMENTY->mG->CheckLink(X,Y,true);//mimo unit2 nepou��vat posledn� parametr true, nen�-li to nutn�

	 //if(ELEMENTY->mG->CheckLink(X,Y)==TPoint(-2,-2))Screen->Cursor=crHandPoint;else Screen->Cursor=crDefault; //funkcionalita presunuta p��mo do mGridu
	 //ELEMENTY->mG->CheckLink(X,Y);//nyn� si p�i vol�n� automaticky nastavuje Cursor=crHandPoint;, mo�no zm�nit na��m kurzorem
   //FormPaint(this);
	 //	if(aktX==-50000 && aktY==-50000)
//	{
//		Cvykresli d;
//		//d.vykresli_robota(Canvas,puvX,puvY,"","",3,1,0,true);
//		//d.vykresli_robota(Canvas,X,Y,"","",3,1,0,true);
//
//		//d.vykresli_stopku(Canvas,puvX,puvY,"","",0,0,true);
//		//d.vykresli_stopku(Canvas,X,Y,"","",0,0,true);
//
//		d.vykresli_otoc(Canvas,puvX,puvY,"","",1,1,0,true);
//		d.vykresli_otoc(Canvas,X,Y,"","",1,1,0,true);
//
//
//		puvX=X,puvY=Y;
//	}
	if(Shift.Contains(ssShift)){aktX=F->m.P2Lx(X);aktY=F->m.P2Ly(Y);Invalidate();FormPaint(this);}

	if(Shift.Contains(ssCtrl))
	{
		InvalidateRect(Handle,&TRect(ELEMENTY->dalsi->mG->Left-1,ELEMENTY->dalsi->mG->Top-1,ELEMENTY->dalsi->mG->Left+ELEMENTY->dalsi->mG->Width+1,ELEMENTY->dalsi->mG->Top+ELEMENTY->dalsi->mG->Height+1),true);//sma�e starou oblast
		//�lo by vylep�it p�ekreslovan�m pouze obd�ln�ku rozd�lu mezi sou�adnicemi
		ELEMENTY->dalsi->mG->Left=X;ELEMENTY->dalsi->mG->Top=Y;
		FormPaint(this);
		//Invalidate();FormPaint(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
	aktX=F->m.P2Lx(X);aktY=F->m.P2Ly(Y);
	FormPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
//	 if(ELEMENTY->mG->CheckLink(X,Y)==TPoint(-2,-2))
//	 {
//			ELEMENTY->mG->ShowNote("");
//	 }

		 ShowMessage(String(ELEMENTY->dalsi->mG->CheckLink(X,Y).x)+" "+String(ELEMENTY->dalsi->mG->CheckLink(X,Y).y));


}

//---------------------------------------------------------------------------

void __fastcall TForm2::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 switch(Key)
	 {
		//F5
		case 116: aktX=-50000;aktY=-50000;Invalidate();break;
		//F7
		case 118:F->ZOOM_IN();break;
		//F8
		case 119:F->ZOOM_OUT();break;
	 }
}
//---------------------------------------------------------------------------






void __fastcall TForm2::Button6Click(TObject *Sender)
{
//ELEMENTY->mG->Note.Text="";
//InvalidateRect(Form2->Handle,&ELEMENTY->mG->Note.NoteArea,true);
	ELEMENTY->mG->ShowNote("");
	ELEMENTY->mG->SetEnabledComponents(false);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button7Click(TObject *Sender)
{
	ELEMENTY->mG->SetEnabledComponents(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button8Click(TObject *Sender)
{
	ELEMENTY->mG->AddColumn();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button12Click(TObject *Sender)
{
	//ELEMENTY->mG->InsertColumn(4);

	if(Edit1->Text=="")ELEMENTY->mG->InsertColumn(ELEMENTY->mG->ColCount-1,true);
	else ELEMENTY->mG->InsertColumn(Edit1->Text.ToInt(),true);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button9Click(TObject *Sender)
{
//F->Memo("",true);
if(Edit1->Text=="")ELEMENTY->mG->DeleteColumn(ELEMENTY->mG->ColCount-1,true);
else ELEMENTY->mG->DeleteColumn(Edit1->Text.ToInt(),true);

}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button10Click(TObject *Sender)
{
//	for(unsigned long Y=0;Y<ELEMENTY->mG->RowCount;Y++)//pr�chod po jednotliv�ch ��dc�ch
//	{
//		AnsiString R="";
//		for(unsigned long X=0;X<ELEMENTY->mG->ColCount;X++)//pr�chod po jednotliv�ch sloupc�ch
//		{
//			 R+=ELEMENTY->mG->Cells[X][Y].Text+" ";
//		}
//		Memo1->Lines->Add(R);
//	}
//		ELEMENTY->mG->ColCount--;
//		ELEMENTY->mG->Refresh();



//	for(unsigned long Y=0;Y<ELEMENTY->mG->RowCount;Y++)//pr�chod po jednotliv�ch ��dc�ch
//	{
//		for(unsigned long X=0;X<ELEMENTY->mG->ColCount;X++)//pr�chod po jednotliv�ch sloupc�ch
//		{
//			//ELEMENTY->mG->RenameComponent(X,Y,X,Y);
//			//ShowMessage(ELEMENTY->mG->getEdit(X,Y)->Tag);
//			ShowMessage(ELEMENTY->mG->getEdit(X,Y)->Name);
//		}
//	}

	//if(Form2->FindComponent("mGrid_EDIT_"+AnsiString(0)+"_"+AnsiString(7))==NULL)ShowMessage("NULL");else  ShowMessage(Form2->FindComponent("mGrid_EDIT_"+AnsiString(0)+"_"+AnsiString(7))->Tag);
	TscGPEdit * E=(TscGPEdit*)Form2->FindComponent("mGrid_EDIT_"+AnsiString(0)+"_"+AnsiString(7));
	E->Top+=50;

	E=(TscGPEdit*)Form2->FindComponent("mGrid_EDIT_"+AnsiString(0)+"_"+AnsiString(8));
	E->Top+=50;




}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button11Click(TObject *Sender)
{
	//rozd�l prvk� p�vodn� matice a nov� matice a p�vodn� pr
//	unsigned long akt_pocet=ELEMENTY->mG->RowCount*ELEMENTY->mG->ColCount;
//	for(unsigned long i=akt_pocet+1;i<9;i++)
//	{
//		TComponent *C=Form2->FindComponent("mGrid_EDIT_"+AnsiString(0)+"_"+AnsiString(i));
//		if(C!=NULL){C->Free();C=NULL;delete C;}
//	}
//	for(unsigned long X=0;X<ELEMENTY->mG->ColCount;X++)
//	{
//		for(unsigned long Y=0;Y<ELEMENTY->mG->RowCount;Y++)
//		{
//			TComponent *C=Form2->FindComponent("mGrid_EDIT_"+AnsiString(0)+"_"+AnsiString((X+1)+Y*ELEMENTY->mG->ColCount));
//			if(C!=NULL)ELEMENTY->mG->Cells[X][Y].Text=C->Tag;
//		}
//	}
//	Invalidate();

	//ShowMessage(ELEMENTY->mG->Cells[2][2].Text);
	 //ELEMENTY->mG->DeleteCell(4,3);
	 Invalidate();
	 TscGPEdit * E=(TscGPEdit*)Form2->FindComponent("mGrid_BUTTON_"+AnsiString(0)+"_"+AnsiString(20));
	E->Top+=50;
}
//---------------------------------------------------------------------------



