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

		E->mG=new TmGrid(this);//vždy nutno jako první
		E->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
		E->mG->ID=0;
		E->mG->Left=100;E->mG->Top=50;//hodné jako druhé (popø. by bylo nutné pøekreslovat)
		E->mG->AntiAliasing_text=true;
		E->mG->Border.Width=2;
///////////////////
//rosta
   E->mG->Create(5, 6);
		int i=0;
		E->mG->Cells[1][i+2].Text = " poèet";
		E->mG->Cells[1][i+3].Text = " prázdných";
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
//		unsigned long ColCount=3;//pevný poèet slopcù
//		unsigned long RowCount=3;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
//
//		podmínìné formátování
//		E->mG->DefaultCell.isZero->Color=clGreen;
//		E->mG->DefaultCell.isEmpty->Color=F->m.clIntensive(clRed,230);
//
//		E->mG->Create(ColCount,RowCount);//samotné vytvoøení matice-tabulky
//		E->mG->Cells[0][0].Text="0";E->mG->Cells[0][0].Type=E->mG->EDIT;
//		E->mG->Cells[1][0].Text="5,555";E->mG->Cells[1][0].Type=E->mG->EDIT;
//		E->mG->Cells[0][1].Type=E->mG->DRAW;
//		E->mG->Cells[0][1].Text="zaèátek <a>[m]</a>"; E->mG->Cells[0][1].Hint="test hintu";E->mG->Cells[0][1].ShowHint=true;
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


		//test obrázku
		//E->mG->Cells[1][2].Type=E->mG->IMAGE;E->mG->Cells[1][2].ImageIndex=0;
		//E->mG->Cells[2][2].Type=E->mG->IMAGE;E->mG->Cells[2][2].ImageIndex=1;

		//E->mG->Columns[0].Width=800;
		//E->mG->SetColumnAutoFit(0);

		//E->mG->Note.Text="Text výpisu poznámky pod èi nad èarou a <a>link</a> nìjaký další abcdefgeijasdfads dafs";
		//E->mG->ShowNote("Text výpisu poznámky pod èi nad èarou a <a>link</a> nìjaký další abcdefgeijasdfads dafs");

		E->predchozi=NULL;
		E->dalsi=NULL;

		ELEMENTY=E;




//		////////E1
//		TElement *E1=new TElement;
//
//		E1->mG=new TmGrid(this);//vždy nutno jako první
//		E1->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
//		E1->mG->ID=1;
//		E1->mG->Left=200;E1->mG->Top=200;//hodné jako druhé (popø. by bylo nutné pøekreslovat)
//		E1->mG->AntiAliasing_text=true;
//		//E1->mG->MovingTable=true;
//		E1->mG->Border.Width=2;
//
//
//		ColCount=2;//pevný poèet slopcù
//		RowCount=6;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
//
//		E1->mG->Create(ColCount,RowCount);//samotné vytvoøení matice-tabulky
////		E1->mG->Columns[1].Width=50;
//		E1->mG->SetColumnAutoFit(-4);
//
//		E1->mG->Cells[0][0].Type=E1->mG->COMBO;E1->mG->Cells[0][0].Text="hlavièka";
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




//	mGrid=new TmGrid(this);//vždy nutno jako první
//	mGrid->Tag=4;//ID tabulky
//	mGrid->Left=0;mGrid->Top=0;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
//	mGrid->AntiAliasing_text=true;

	//mGrid->DefaultCell.isEmpty->Color=clBlue; //musí být pøed create, jinak nutno nastavovat konkrétní buòku
	//mGrid->DefaultCell.isZero->Color=clLime;  //musí být pøed create, jinak nutno nastavovat konkrétní buòku
	//mGrid->DefaultCell.isNegativeNumber->Color=clRed; //musí být pøed create, jinak nutno nastavovat konkrétní buòku

//	mGrid->Create(3,3);//vhodné jako tøetí




	//mGrid->Border.Width=1;

//	mGrid->Cells[3][2].Text="text v slouèených buòkách";
//	mGrid->Cells[3][2].Font->Color=clWhite;
//	mGrid->Cells[3][2].Align=mGrid->CENTER;
//	mGrid->Cells[3][2].TopBorder->Color=clBlack;mGrid->Cells[3][2].TopBorder->Width=3;
//	*mGrid->Cells[3][2].LeftBorder=*mGrid->Cells[3][2].RightBorder=*mGrid->Cells[3][2].BottomBorder=*mGrid->Cells[3][2].TopBorder;//v tomto øádku ukázka pøebírání vlastnostní orámování, jinak samozøejmì možno zapsat standardnì (a plnit)
//	mGrid->Cells[3][2].Background->Color=clLime;


	//toto propojuje orámování bunìk ukazatelovì, a to je vìtšinou nežádoucí použiv lépe metody
	//mGrid->Cells[1][2]=mGrid->Cells[2][3]

//	mGrid->Rows[1].Height=2*mGrid->DefaultRowHeight;
//	mGrid->Rows[5].Height=100;
//
//	mGrid->Cells[1][3].Background->Color=clRed;
//	mGrid->Cells[1][3].Font->Color=clBlue;
//	mGrid->Cells[1][3].Align=mGrid->RIGHT;
//	mGrid->Cells[1][3].Valign=mGrid->TOP;
//	mGrid->Cells[1][3].Text="nìjaký dlouhý text";
//
//	mGrid->Cells[7][3].Font->Orientation=900;
//	mGrid->Cells[7][3].Font->Color=clBlue;
//	mGrid->Cells[7][3].Text="Orotovaný text";
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
//	mGrid->Cells[5][5].Text="tlaèítko";
//
//	mGrid->Cells[6][5].Text="COMBO";
//	mGrid->Cells[6][5].Type=mGrid->COMBO;

//	//ukázka pøebírání vlastností spoleèného orámování bunìk
//	mGrid->Cells[8][8].TopBorder->Width=3;
//	mGrid->Cells[8][8].TopBorder->Color=clBlue;
//	mGrid->Cells[8][7].BottomBorder->Color=clWebOrange;
////	if(mGrid->Cells[8][7].BottomBorder->Color==clWebOrange)
////	ShowMessage("clWebOrange");
//	mGrid->Cells[8][8].LeftBorder->Color=clWebOrange;
//	mGrid->Cells[7][8].RightBorder->Color=clBlue;
//
//	//nastaví oblast bunìk na stejnými vlastnostmi (vhodné napø. na fixed cells, orámování nìjaké oblasti, disabled/read-only oblast atp.)
//	mGrid->Cells[0][0].Text="Nadpis";
//	mGrid->Cells[0][0].Background->Color=clBtnFace;
//	mGrid->Cells[0][0].Align=mGrid->RIGHT;
//	mGrid->Cells[0][0].Valign=mGrid->TOP;
//	mGrid->SetCells(mGrid->Cells[0][0],0,0,0,7);
//	mGrid->SetCells(mGrid->Cells[0][0],0,0,7,0);


 //	mGrid->SetColumnAutoFit(1);//ukázka nastavení velikosti sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show()

	//merge bunìk musí být, co nejblíže nad Show() a hned po SetColumnAutoFit, popø. by marge mohl být kdekoliv, ale bylo by nutné po nìm volat FormPaint(this)
	//mGrid->MergeCells(3,2,5,3);
	//mGrid->MergeCells(7,3,8,9);

//	mGrid->AntiAliasing_text=false;//nemuže být varianta  AntiAliasing_text=true; a AntiAliasing_grid=false; (je i ošetøeno)
//	mGrid->AntiAliasing_grid=false;

	//tabulka 2 lokální deklarace TmGrit (v Unit2.h)
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
//	mGrid2->Cells[1][1].Text="tlaèítko";

	TElement *el=new TElement;

	//definice jednotek a šíøek
	AnsiString LO,cas,delka_otoce,rychlost,R,Rz;
	//nastavení jednotek podle posledních nastavení
	cas="<a>[min]</a>";//1
	LO="<a>[mm]</a>";//1
	delka_otoce="<a>[m]</a>";//0
	rychlost="<a>[m/min]</a>";//
	R="<a>[mm]</a>";//1
	Rz="<a>[mm]</a>";//1
	//vytvoøení mgridu
	el->mG=new TmGrid(this);
	el->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	el->mG->ID=1;
	el->mG->Create(5,12);
	el->mG->Left=500;
	el->mG->Top=500;
	//nastavení fontu
	TColor clFontLeft = (TColor)RGB(128,128,128);
	//TColor clFontRight = (TColor)RGB(43,87,154);
	//definice fontu a velikosti písma
	el->mG->DefaultCell.Font->Name=F->aFont->Name;
	el->mG->DefaultCell.Font->Size=F->aFont->Size;
	el->mG->DefaultCell.Font->Color=clFontLeft;
	//nastavení popiskù
	el->mG->Border.Width=2;
	el->mG->Cells[0][0].BottomBorder->Width=2;
	el->mG->Cells[0][0].Text="Pøedávací místo 1"; el->mG->Cells[0][0].Font->Color=clBlack; //el->mG->Cells[0][0].TopMargin=-1;//pouze optická záležitost
	el->mG->Cells[3][1].Text="IN";el->mG->Cells[3][1].Align=TmGrid::RIGHT;el->mG->Cells[3][1].RightMargin=5;
	el->mG->Cells[4][1].Text="OUT";el->mG->Cells[4][1].Align=TmGrid::RIGHT;el->mG->Cells[4][1].RightMargin=5;
	el->mG->Cells[0][1].Valign=TmGrid::MIDDLE;
	el->mG->Cells[0][1].Text="Pohon"; el->mG->Cells[0][1].Font->Color=clBlack;

	el->mG->Cells[0][3].Font->Orientation=900;
	el->mG->Cells[0][3].Valign=el->mG->MIDDLE;
	el->mG->Cells[0][3].Text="Rychl.";
	el->mG->Cells[1][3].Font->Orientation=el->mG->Cells[0][3].Font->Orientation;el->mG->Cells[1][3].isLink->Orientation=900;el->mG->Cells[1][3].isActiveLink->Orientation=900;
	el->mG->Cells[1][3].Valign=el->mG->Cells[0][3].Valign;
	el->mG->Cells[1][3].Text="<a>m/min</a>";

	el->mG->Cells[2][3].Text="Nastavená";
	el->mG->Cells[2][4].Text="Rozmezí ";

	el->mG->Cells[0][5].Font->Orientation=900;
	el->mG->Cells[0][5].Valign=TmGrid::MIDDLE;
	el->mG->Cells[0][5].Text="Rozteè";
	el->mG->Cells[1][5].Font->Orientation=900;el->mG->Cells[1][5].isLink->Orientation=900;el->mG->Cells[1][5].isActiveLink->Orientation=900;
	el->mG->Cells[1][5].Valign=TmGrid::MIDDLE;
	el->mG->Cells[1][5].Text="<a>[mm]</a>";

	el->mG->Cells[2][5].Text="Jigy";
	el->mG->Cells[2][6].Text="Palce";
	el->mG->Cells[2][7].Text="Násobek";

	el->mG->Cells[0][8].Font->Orientation=900;el->mG->Cells[1][8].Font->Orientation=900;el->mG->Cells[1][8].isLink->Orientation=900;el->mG->Cells[1][8].isActiveLink->Orientation=900;
	//el->mG->Cells[1][8].Valign=
	el->mG->Cells[0][8].Valign=TmGrid::MIDDLE;
//	el->mG->Cells[1][8].BottomMargin=el->mG->Cells[0][8].Align=el->mG->CENTER;
	el->mG->Cells[1][8].Valign=TmGrid::MIDDLE;
	el->mG->Cells[0][8].Text="Mezera";
	el->mG->Cells[1][8].Text="<a>[m]</a>";


	el->mG->Cells[2][8].Text="Podvozky ";
	el->mG->Cells[2][9].Text="Jigy 0°";
	el->mG->Cells[2][10].Text="Jigy 90°";
	el->mG->Cells[2][11].Text="max WT "+cas+" ";
	//nastavení šíøek
	el->mG->SetColumnAutoFit(-4);
	el->mG->Columns[0].Width=el->mG->Columns[1].Width=el->mG->Rows[0].Height;
	el->mG->Columns[2].Width=90;
	el->mG->Columns[3].Width=145;
	el->mG->Columns[4].Width=145;
	//merge
	el->mG->MergeCells(0,0,4,0);//název tabulky
	el->mG->MergeCells(0,1,2,2);//pohon
	el->mG->MergeCells(0,3,0,4);
	el->mG->MergeCells(1,3,1,4);
	el->mG->MergeCells(0,5,0,7);
	el->mG->MergeCells(1,5,1,6);
	el->mG->MergeCells(0,8,0,10);
	el->mG->MergeCells(1,8,1,10);

	//final design
	el->mG->Cells[3][1].Font->Color=clBlack;
	el->mG->Cells[4][1].Font->Color=clBlack;
	/*el->mG->Cells[2][1].RightBorder->Width=2;*/el->mG->Cells[3][1].RightBorder->Width=2;
	/*el->mG->Cells[2][2].RightBorder->Width=2;*/el->mG->Cells[3][2].RightBorder->Width=2;
	//el->mG->Cells[0][3].TopBorder->Color=el->mG->Cells[1][3].TopBorder->Color=clWhite;
	for(int i=3;i<=el->mG->RowCount-1;i++)
	{
		el->mG->Cells[2][i].Align=el->mG->RIGHT;
		el->mG->Cells[2][i].RightMargin=5;
		el->mG->Cells[2][i].RightBorder->Width=
		el->mG->Cells[3][i].RightBorder->Width=2;
		el->mG->Cells[0][i].RightBorder->Color=clWhite;
		el->mG->Cells[2][i].Font->Color=clFontLeft;
		el->mG->Cells[0][i].Font->Color=clFontLeft;
	}
	el->mG->Cells[2][11].LeftBorder->Color=clWhite;

	for(int i=0;i<=el->mG->ColCount-1;i++)
	{
		el->mG->Cells[i][3].TopBorder->Width=2;
		el->mG->Cells[i][5].TopBorder->Width=2;
		el->mG->Cells[i][8].TopBorder->Width=2;
		el->mG->Cells[i][11].TopBorder->Width=2;
  }

	el->dalsi=NULL;
	el->predchozi=ELEMENTY;
	ELEMENTY->dalsi=el;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormPaint(TObject *Sender)
{
	ELEMENTY->mG->Show();
	ELEMENTY->dalsi->mG->Show();
	//ELEMENTY->dalsi->mG->Show();

//				Form2->Canvas->Pen->Color=clGreen;
//			Form2->Canvas->Brush->Style=bsSolid; Form2->Canvas->Brush->Color=clGreen;
//			Form2->Canvas->FrameRect(ELEMENTY->mG->Note.CoordinateArea);


	//mGrid2->Show();
	//nastaví formuláø dle velikosti tabulky, musí být až po Show
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
//		bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
//		F->Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
//		d.vykresli_robota(bmp_in->Canvas,F->m.L2Px(aktX),F->m.L2Py(aktY),"Robot 1","R1",1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+100+10)*F->Zoom,aktY*F->Zoom,"Robot 2","R2",1,1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+200+20)*F->Zoom,aktY*F->Zoom,"Robot 3","R3",2,1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+300+30)*F->Zoom,aktY*F->Zoom,"Robot 4","R4",3,1);
////		d.vykresli_robota(bmp_in->Canvas,(aktX+400+40)*F->Zoom,aktY*F->Zoom,"Robot 5","R5",0,-1);
////		d.vykresli_stopku(bmp_in->Canvas,F->m.L2Px(aktX)+100,F->m.L2Py(aktY),"Stop 1","S1",1,0);
////		d.vykresli_otoc(bmp_in->Canvas,F->m.L2Px(aktX)+200,F->m.L2Py(aktY),"Otoè pasiv","O1",0,0);
////		d.vykresli_otoc(bmp_in->Canvas,(aktX-500)*F->Zoom,aktY*F->Zoom,"Otoè aktiv","O2",1,1);
//		F->Zoom/=3;//navrácení zoomu na pùvodní hodnotu
//		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvùli smazání bitmapy vracené AA
//		bmp_out->Transparent=true;
//		bmp_out->TransparentColor=clWhite;
//		Canvas->Draw(0,0,bmp_out);
//		delete (bmp_out);//velice nutné
//		delete (bmp_in);//velice nutné
//	}
//	else
//	{
//		d.vykresli_robota(Canvas,aktX,aktY,"Robot 1","R1",0,1);
//		d.vykresli_stopku(Canvas,(aktX-600)*F->Zoom,aktY*F->Zoom,"Stop 1","S1",1,0);
//		d.vykresli_otoc(Canvas,(aktX-500)*F->Zoom,aktY*F->Zoom,"Otoè aktiv","O2",1,1);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
//	//plnìní comba
//	TscGPComboBox *C=mGrid->getCombo(6,5);
//	//C->Options->další možnosti
//	//C->Items->Clear();//smazání pøípadného pùvodního obsahu  - nepouživat pokud je prázdný
//	TscGPListBoxItem *t=C->Items->Add();t->Caption="test1";
//	t=NULL;t=C->Items->Add();t->Caption="test2";
//	C->ItemIndex=1;
//	t=NULL;delete t;
//	C=NULL;delete C;//nejsem si jistý zda používat nebo ne, pokud bude padat, tak vyzkoušet tady zakomentovat
//	//aktualizace comba èi obecnì komponent nepotøebuje refresh
//
//	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
//	FormPaint(this);//volání po Invalidate zajistí, že nedochází k probliku komponent, nemùže být samotné

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

	//testování checkboxu
	//if(mGrid->getRadio(2,5,this)->Checked)ShowMessage("zaškrnuto");
	//else ShowMessage("nezaškrtnuto");

	//zmìna velikosti tabulky
	//mGrid->RowCount=20;mGrid->ColCount=3;//zajistí realokaci tabulky (pokud je následnì voláno invalidate)


	//mGrid->AntiAliasing_grid=!mGrid->AntiAliasing_grid;
	// ELEMENTY->mG->Left+=10;
	//zmìna posunu tabulky
	// ELEMENTY->mG->unHighlightAll();

	//ELEMENTY->mG->Cells[1][2].Text="Vzor.bmp";

	TscGPGlyphButton *H = ELEMENTY->mG->getGlyphButton(0,0);
	H->GlyphOptions->Kind = scgpbgkClose;

	Invalidate();
	FormPaint(this);//volání po Invalidate zajistí, že nedochází k probliku komponent, nemùže být samotné
}
//---------------------------------------------------------------------------
//test volání pøi onclick          //pozor metody musí mít i znamenkové longové Col, Row, kvùli -2 exBUTTOn
void TForm2::OnClick(long Tag,long ID,long Col,long Row)
{
	//ShowMessage("UNIT2\nToto vypisuje metoda OnClick\nDošlo ke kliku v tabulce tag formu: "+AnsiString(Tag)+", ID tabulky: "+AnsiString(ID)+", na buòce: "+AnsiString(Col)+","+AnsiString(Row));
	if(Row==-2)
	{
		//ShowMessage("UNIT2\nexBUTTON");
		if(ELEMENTY->mG->Rows[2].Visible)//skrývání
		{
			ELEMENTY->mG->VisibleRow(2,false);
			ELEMENTY->mG->VisibleRow(3,false);
			ELEMENTY->mG->VisibleRow(4,false);
			ELEMENTY->mG->VisibleRow(5,false);
			ELEMENTY->mG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
		}
		else//zobrazování
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
//	FormPaint(this);//zajistí pøekreslení bez probliku
//	//toto problikává mGrid->Refresh();
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
Button1->SetFocus();//odevzdání focusu na neutrální komponentu
ELEMENTY->mG->Delete(); //ELEMENTY->dalsi->mG->Delete();
//	 mGrid->Delete();//pokud chci odstranit a nechci použít na další použití
//	 mGrid2->Delete();
//	 mGrid2=NULL; delete mGrid2;
}
//---------------------------------------------------------------------------
//pøiklady
/*
///////////vytvvoøení
	mGrid=new TmGrid;
	mGrid->Left=600;mGrid->Top=40;
	//myGrid->ColCount=3;myGrid->RowCount=5;//poèet øádkù a sloupcù
	//myGrid->Create(); lze pøímo vytvoøit níže
	mGrid->Create(4,8);

	mGrid->Cells[0][0].Text="2,5";
	mGrid->Cells[0][0].Type=mGrid->NUMERIC;

	mGrid->Cells[0][5].Text="Checkbox";
	mGrid->Cells[0][5].Type=mGrid->CHECK;

	mGrid->Cells[1][5].Text="Tlaèítko";
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
	FormPaint(this);//zajistí pøekreslení bez probliku

/////////// prùbìžná zmìna
		 mGrid->HighlightCell(1,5);

		mGrid->getEdit(0,6,this)->Text="JO!";
	mGrid->getEdit(0,6,this)->Font->Color=clRed;
//	TscGPEdit *E=mGrid->getEdit(this,0,6);
//	E->Text="JO!";
//	E=NULL;delete E;

		mGrid->MergeCells(1,1,2,1);
		mGrid->MergeCells(1,1,1,2);

TscGPComboBox *C=mGrid->getCombo(3,7,this);
C->Items->Clear();//smazání pøípadného pùvodního obsahu
TscGPListBoxItem *t=NULL;t=C->Items->Add();t->Caption="test1";
t=NULL;t=C->Items->Add();t->Caption="test2";
C->ItemIndex=1;
C=NULL;delete C;

/////////// zmìna poètu øádkù èi sloupc (realokace) zde nefungovalo
		mGrid->Left=500;
		mGrid->ColCount=4;mGrid->RowCount=4;

		mGrid->SetColumnAutoFit(-3);
		FormPaint(this);//zajistí pøekreslení bez probliku
*/
//---------------------------------------------------------------------------
void __fastcall TForm2::Button5Click(TObject *Sender)
{
	ELEMENTY->mG->AddRow();
	//test pouze pøidání textu:
	//mGrid->Cells[1][mGrid->RowCount-1].Text=mGrid->RowCount-1;FormPaint(this);//nutno kvùli aktualizaci textu
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button4Click(TObject *Sender)
{
	ELEMENTY->mG->InsertRow(3);//s problikem zpùsobuje show() v InsertRow
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
	 ELEMENTY->mG->MouseMove(X,Y);//pro Hint a kurzor odkazu, mousemove daného formu sice možno odchytávát pøímo v mGridu, ale toto nutnost pro rozlišení tabulek
	 //ELEMENTY->mG->CheckLink(X,Y,true);//mimo unit2 nepoužívat poslední parametr true, není-li to nutné

	 //if(ELEMENTY->mG->CheckLink(X,Y)==TPoint(-2,-2))Screen->Cursor=crHandPoint;else Screen->Cursor=crDefault; //funkcionalita presunuta pøímo do mGridu
	 //ELEMENTY->mG->CheckLink(X,Y);//nyní si pøi volání automaticky nastavuje Cursor=crHandPoint;, možno zmìnit naším kurzorem
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
		InvalidateRect(Handle,&TRect(ELEMENTY->dalsi->mG->Left-1,ELEMENTY->dalsi->mG->Top-1,ELEMENTY->dalsi->mG->Left+ELEMENTY->dalsi->mG->Width+1,ELEMENTY->dalsi->mG->Top+ELEMENTY->dalsi->mG->Height+1),true);//smaže starou oblast
		//šlo by vylepšit pøekreslovaním pouze obdélníku rozdílu mezi souøadnicemi
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
	 if(ELEMENTY->mG->CheckLink(X,Y)==TPoint(-2,-2))
	 {
			ELEMENTY->mG->ShowNote("");
	 }
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
//	for(unsigned long Y=0;Y<ELEMENTY->mG->RowCount;Y++)//prùchod po jednotlivých øádcích
//	{
//		AnsiString R="";
//		for(unsigned long X=0;X<ELEMENTY->mG->ColCount;X++)//prùchod po jednotlivých sloupcích
//		{
//			 R+=ELEMENTY->mG->Cells[X][Y].Text+" ";
//		}
//		Memo1->Lines->Add(R);
//	}
//		ELEMENTY->mG->ColCount--;
//		ELEMENTY->mG->Refresh();



//	for(unsigned long Y=0;Y<ELEMENTY->mG->RowCount;Y++)//prùchod po jednotlivých øádcích
//	{
//		for(unsigned long X=0;X<ELEMENTY->mG->ColCount;X++)//prùchod po jednotlivých sloupcích
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
	//rozdíl prvkù pùvodní matice a nová matice a pùvodní pr
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



