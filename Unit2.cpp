//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm2 *Form2;

//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormShow(TObject *Sender)
{
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=4;//ID tabulky
	mGrid->Left=0;mGrid->Top=0;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->Create(3,3);//vhodn� jako t�et�

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

	 mGrid->Cells[1][0].Type=mGrid->EDIT; mGrid->Cells[1][0].Text="0";
	 mGrid->Cells[1][1].Type=mGrid->EDIT; mGrid->Cells[1][1].Text="1";
	 mGrid->Cells[1][2].Type=mGrid->EDIT; mGrid->Cells[1][2].Text="2";
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
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormPaint(TObject *Sender)
{
	mGrid->Show();
	//mGrid2->Show();

	//nastav� formul�� dle velikosti tabulky, mus� b�t a� po Show
	//Form2->Width=mGrid->Width+50+50;
	//Form2->Height=mGrid->Height+50+50;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
	//pln�n� comba
	TscGPComboBox *C=mGrid->getCombo(6,5);
	//C->Options->dal�� mo�nosti
	//C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu  - nepou�ivat pokud je pr�zdn�
	TscGPListBoxItem *t=C->Items->Add();t->Caption="test1";
	t=NULL;t=C->Items->Add();t->Caption="test2";
	C->ItemIndex=1;
	t=NULL;delete t;
	C=NULL;delete C;//nejsem si jist� zda pou��vat nebo ne, pokud bude padat, tak vyzkou�et tady zakomentovat
	//aktualizace comba �i obecn� komponent nepot�ebuje refresh

	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
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
	mGrid->RowCount=20;mGrid->ColCount=3;//zajist� realokaci tabulky (pokud je n�sledn� vol�no invalidate)


	//mGrid->AntiAliasing_grid=!mGrid->AntiAliasing_grid;
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick
void TForm2::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	ShowMessage("Do�lo ke kliku v tabulce: "+AnsiString(Tag)+", na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
//	mGrid->HighlightCell(Col,Row);
//	mGrid->Cells[0][0].Text="test";
//	FormPaint(this);//zajist� p�ekreslen� bez probliku
//	//toto problik�v� mGrid->Refresh();
}
//---------------------------------------------------------------------------
void TForm2::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TForm2::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
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
	mGrid->AddRow();
	//test pouze p�id�n� textu:
	mGrid->Cells[1][mGrid->RowCount-1].Text=mGrid->RowCount-1;FormPaint(this);//nutno kv�li aktualizaci textu
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button4Click(TObject *Sender)
{
	mGrid->InsertRow(3);//s problikem zp�sobuje show() v InsertRow
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button3Click(TObject *Sender)
{
	mGrid->DeleteRow(3);
	//mGrid->DeleteRow(mGrid->RowCount-1);//s problikem zp�sobuje show() v DeleteRow
}
//---------------------------------------------------------------------------




