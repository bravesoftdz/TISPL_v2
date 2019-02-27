//---------------------------------------------------------------------------

#include <vcl.h>
#include "Unit1.h"
#pragma hdrstop

#include "Unit4.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
	//Buttony
	F->m.designButton(Button_pridat, Form4, 1, 2);
	F->m.designButton(Button_odebrat, Form4, 2, 2);
	Button_storno->Top = Button_pridat->Top;
	Button_storno->Left = Button_odebrat->Left - Button_pridat->Left - Button_pridat->Width + Button_odebrat->Left + Button_pridat->Width;
	//Formuláø
	Form4->Width = Button_pridat->Left + Button_pridat->Width + Button_storno->Left;;
	Form4->Left=Form1->ClientWidth/2-Form4->Width/2;
	Form4->Top=Form1->ClientHeight/2-Form4->Height/2;
	//Edity
	Edit1->Top = 2* scGPPanel2->Height;
	Edit1->Left = Button_odebrat->Left;
	Edit1->Width = Button_odebrat->Left - Button_pridat->Left - Button_pridat->Width + 2 * Button_pridat->Width;
	Edit2->Top = Edit1->Top + Edit1->Height + scGPPanel2->Height/2;
	Edit2->Left = Button_odebrat->Left;
	Edit2->Width = Button_odebrat->Left - Button_pridat->Left - Button_pridat->Width + 2 * Button_pridat->Width;
	Edit3->Top = Edit2->Top + Edit1->Height + scGPPanel2->Height/2;
	Edit3->Left = Button_odebrat->Left;
	Edit3->Width = Button_odebrat->Left - Button_pridat->Left - Button_pridat->Width + 2 * Button_pridat->Width;
	Edit4->Top = Edit3->Top + Edit1->Height + scGPPanel2->Height/2;
	Edit4->Left = Button_odebrat->Left;
	Edit4->Width = Button_odebrat->Left - Button_pridat->Left - Button_pridat->Width + 2 * Button_pridat->Width;
	Edit5->Top = Edit4->Top + Edit1->Height + scGPPanel2->Height/2;
	Edit5->Left = Button_odebrat->Left;
	Edit5->Width = Button_odebrat->Left - Button_pridat->Left - Button_pridat->Width + 2 * Button_pridat->Width;
	//Labely
	rHTMLLabel1->Left = Button_pridat->Left;
	rHTMLLabel1->Top = Edit1->Top;
	rHTMLLabel2->Left = Button_pridat->Left;
	rHTMLLabel2->Top = Edit2->Top;
	rHTMLLabel3->Left = Button_pridat->Left;
	rHTMLLabel3->Top = Edit3->Top;
	rHTMLLabel4->Left = Button_pridat->Left;
	rHTMLLabel4->Top = Edit4->Top;
	rHTMLLabel5->Left = Button_pridat->Left;
	rHTMLLabel5->Top = Edit5->Top;
}


//---------------------------------------------------------------------------

void __fastcall TForm4::Button_stornoClick(TObject *Sender)
{
	if (mrOk == F->MB("Neuložený obsah bude ztracen.", MB_OKCANCEL)) {
		Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button_pridatClick(TObject *Sender)
{
	mGrid->AddRow(false,false);
	mGrid->Show();
	F->d.v.POHONY->predchozi->name = Edit1->Text;
	F->d.v.POHONY->predchozi->rychlost_od = F->ms.MyToDouble(Edit2->Text);
	F->d.v.POHONY->predchozi->rychlost_do = F->ms.MyToDouble(Edit3->Text);
	F->d.v.POHONY->predchozi->roztec = F->ms.MyToDouble(Edit3->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button_odebratClick(TObject *Sender)
{
	mGrid->DeleteRow(mGrid->RowCount - 1,false);
	mGrid->Cells[0][mGrid->RowCount].Text = mGrid->Cells[0][mGrid->RowCount - 1].Text;
	mGrid->Refresh();
	Cvektory::TPohon *O=Form1->d.v.POHONY->dalsi;      // Nechat si lépe vysvìtlit
	 while(O!=NULL)
	 {
		if (O->n == F->ms.MyToDouble(Edit5->Text)) {
		   O->name = Edit1->Text;
		   O->rychlost_od = F->ms.MyToDouble(Edit2->Text);
		   O->rychlost_do = F->ms.MyToDouble(Edit3->Text);
		   O->roztec = F->ms.MyToDouble(Edit3->Text);
		   break;
		}
		O=O->dalsi;
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykreslí tabulku
}
//---------------------------------------------------------------------------
void TForm4::OnClick(long Tag,unsigned long Col,unsigned long Row)
{

}

void  TForm4:: OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}

void  TForm4:: OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}

void __fastcall TForm4::FormShow(TObject *Sender)
{
	//mGrid
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=77;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left= rHTMLLabel5->Left;mGrid->Top=rHTMLLabel5->Top + 100;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;

	unsigned long ColCount=3;//pevný poèet slopcù  //NEWR
	unsigned long RowCount=3;

	mGrid->Create(ColCount,RowCount);  //Sloupec, øadek

	mGrid->Cells[0][0].Text="1000";
	mGrid->Cells[0][0].Background->Color = clUNLOCKED;
	mGrid->Cells[0][0].RightBorder->Width = 4;
	//mGrid->Cells[0][0].LeftBorder->Width = 4;
  //	mGrid->SetColumnAutoFit(0);

	//mGrid->Cells[1][0].Ttype::EDIT;
	mGrid->Cells[1][0].Text = "Edit";
	mGrid->Cells[0][1].Text="2000";
	mGrid->Cells[1][1].Text="4";
	mGrid->Cells[0][2].Text="a000";
	mGrid->Cells[2][0].Text=" ";

//	mGrid->SetCells(mGrid->Cells[0][0],0,1,0,RowCount - 1);
//	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount - 1,0);

	mGrid->Cells[1][0].BottomBorder->Width = 4;
	mGrid->Cells[2][0].BottomBorder->Width = 4;

//	mGrid->MergeCells(0,0,1,0);
	//mGrid->MergeCells(1,0,2,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button1Click(TObject *Sender)
{
   //	mGrid->Cells[1][1].Type = mGrid->EDIT;

	mGrid->Cells[1][1].Text="99";

}
//---------------------------------------------------------------------------

