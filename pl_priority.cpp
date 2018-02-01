//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pl_priority.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_PL_priority *Form_PL_priority;
//---------------------------------------------------------------------------
__fastcall TForm_PL_priority::TForm_PL_priority(TComponent* Owner)
	: TForm(Owner)
{
	Form1->m.designButton(Button_save,Form_PL_priority,1,2);
	Form1->m.designButton(Button_storno,Form_PL_priority,2,2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_PL_priority::Button_stornoClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_PL_priority::FormShow(TObject *Sender)
{
	 Form_PL_priority->Color=(TColor)RGB(240,240,240);//RGB(43,87,154);
	// Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
	// Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;

	 Left=Form_parametry_linky->Left;
	 Top=Form_parametry_linky->Top;
	 Width=Form_parametry_linky->Width;


}
//---------------------------------------------------------------------------

void __fastcall TForm_PL_priority::rStringGridEd_tabGetCellParams(TObject *Sender,
          int Col, int Row, TFont *AFont, TColor &Background, bool &Highlight)

{
 	Highlight=false;

	if (Row==1 && scGPRadioButton1->Checked) {

	Background=(TColor)RGB(202,217,240);
	Highlight=true;

	}
		if (Row==2 && scGPRadioButton2->Checked) {

		Background=(TColor)RGB(202,217,240);
		Highlight=true;

	}
		if (Row==3 && scGPRadioButton3->Checked) {

		Background=(TColor)RGB(202,217,240);
		Highlight=true;

	}
		if (Row==4 && scGPRadioButton4->Checked) {

		Background=(TColor)RGB(202,217,240);
		Highlight=true;

	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_PL_priority::scGPRadioButton1Click(TObject *Sender)
{   //workaround zvyraznovani radku aby fungovala konstukce s HighLight
Form_PL_priority->rStringGridEd_tab->Visible=false;
Form_PL_priority->rStringGridEd_tab->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_PL_priority::scGPRadioButton2Click(TObject *Sender)
{
Form_PL_priority->rStringGridEd_tab->Visible=false;
Form_PL_priority->rStringGridEd_tab->Visible=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm_PL_priority::scGPRadioButton3Click(TObject *Sender)
{
Form_PL_priority->rStringGridEd_tab->Visible=false;
Form_PL_priority->rStringGridEd_tab->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PL_priority::scGPRadioButton4Click(TObject *Sender)
{
	Form_PL_priority->rStringGridEd_tab->Visible=false;
	Form_PL_priority->rStringGridEd_tab->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PL_priority::rStringGridEd_tabKeyDown(TObject *Sender, WORD &Key,
					TShiftState Shift)
{
  FormKeyDown(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TForm_PL_priority::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 	if(Key==13)//ENTER
	{
		 if(Button_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {       ShowMessage("OK");
			Form_PL_priority->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_PL_priority->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
	}
	if(Key==27)//ESC
	{
		 Form_PL_priority->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_PL_priority->VisibleChanging();//skryje form, stejn� jako visible=false
	}
}
//---------------------------------------------------------------------------
//k��ek
void __fastcall TForm_PL_priority::KonecClick(TObject *Sender)
{
		 Form_PL_priority->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_PL_priority->VisibleChanging();//skryje form, stejn� jako visible=false
}
//---------------------------------------------------------------------------



