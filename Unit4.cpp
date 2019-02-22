//---------------------------------------------------------------------------

#include <vcl.h>
#include "Unit1.h"
#pragma hdrstop

#include "Unit4.h"
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
	F->d.v.POHONY->predchozi->name = Edit1->Text;
	F->d.v.POHONY->predchozi->rychlost_od = F->ms.MyToDouble(Edit2->Text);
	F->d.v.POHONY->predchozi->rychlost_do = F->ms.MyToDouble(Edit3->Text);
	F->d.v.POHONY->predchozi->roztec = F->ms.MyToDouble(Edit3->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button_odebratClick(TObject *Sender)
{
	Cvektory::TPohon *O=Form1->d.v.POHONY->dalsi;      // Nechat si lépe vysvìtlit
	 while(O!=NULL)
	 {
		if (O->n == F->ms.MyToDouble(Edit5->Text)) {
		   O->name = Edit1->Text;
		   O->rychlost_od = F->ms.MyToDouble(Edit2->Text);
		   O->rychlost_do = F->ms.MyToDouble(Edit3->Text);
		   O->roztec = F->ms.MyToDouble(Edit3->Text);
		}
		O=O->dalsi;
	 }
}
//---------------------------------------------------------------------------

