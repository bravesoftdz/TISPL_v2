//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm2 *Form2;
#include "TmGrid.h"
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormShow(TObject *Sender)
{
	mGrid=new TmGrid;//v�dy nutno jako prvn�
	mGrid->Left=50;mGrid->Top=50;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat
	mGrid->Create(10,10);//vhodn� jako t�et�

	//mGrid->Border.Width=1;

	mGrid->MergeCells(1,2,2,2);

	mGrid->Rows[1].Height=2*mGrid->DefaultRowHeight;
	mGrid->Rows[5].Height=100;

	mGrid->Cells[2][3].Background->Color=clRed;
	mGrid->Cells[2][3].Font->Color=clWhite;
	mGrid->Cells[2][3].Align=mGrid->RIGHT;
	mGrid->Cells[2][3].Valign=mGrid->TOP;
	mGrid->Cells[2][3].Text="n�jak� dlouh� text";

	mGrid->Cells[1][1].Font->Orientation=900;

	mGrid->HighlightCell(1,1);
	mGrid->SetColumnAutoFit(2);

	mGrid->Cells[1][5].Type=mGrid->CHECK;
	//mGrid->Cells[1][5].Valign=mGrid->TOP;
	//mGrid->Cells[1][5].Align=mGrid->LEFT;

	mGrid->Cells[2][5].Type=mGrid->RADIO;

	mGrid->Cells[3][5].Type=mGrid->readNUMERIC;

	mGrid->Cells[4][5].Type=mGrid->EDIT;
	mGrid->Cells[4][5].Text="tady je edit";

	mGrid->Cells[5][5].Type=mGrid->BUTTON;
	mGrid->Cells[5][5].Text="tla��tko";

	mGrid->Cells[6][5].Text="COMBO";
	mGrid->Cells[6][5].Type=mGrid->COMBO;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormPaint(TObject *Sender)
{
	mGrid->Show(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
	TscGPComboBox *C=mGrid->getCombo(6,5,this);
	//C->Options->dal�� mo�nosti
	C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu
	TscGPListBoxItem *t=C->Items->Add();t->Caption="test1";
	t=NULL;t=C->Items->Add();t->Caption="test2";
	C->ItemIndex=1;
	t=NULL;delete t;
	C=NULL;delete C;

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
	//if(mGrid->getRadio(2,5,this)->Checked)ShowMessage("za�krnuto");
	//else ShowMessage("neza�krtnuto");

	mGrid->RowCount=6;mGrid->ColCount=6;//zajist� realokaci tabulky
	Invalidate();
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick
void TForm2::OnClick(unsigned long Col,unsigned long Row)
{
	//ShowMessage("Do�lo ke kliku na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
	mGrid->HighlightCell(Col,Row);
	mGrid->Cells[0][0].Text="test";
	Invalidate();
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
void TForm2::OnEnter(unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TForm2::OnChange(unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	 mGrid->Delete(this);//pokud chci odstranit a nechci pou��t na dal�� pou�it�
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
	Invalidate();
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�

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
			Invalidate();//kv�li v�em pack�m + zaji�tuje z�rove� vol�n� frameCorrelation()//stejnou barvou or�muje hodnoty v korelaci + vykresl� korela�n� packy
		FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�

*/





