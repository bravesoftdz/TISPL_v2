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

	 //Left=Form_parametry_linky->Left;
	 //Top=Form_parametry_linky->Top;
	 //Width=Form_parametry_linky->Width;



	 //zobrazí form uprostøed "mateøského" formu Form_parametry_linky a to i kdyby došlo k posunu formu
	 Form_PL_priority->Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Form_PL_priority->Width/2;
	 Form_PL_priority->Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_PL_priority->Height/2;
}
//---------------------------------------------------------------------------

void __fastcall TForm_PL_priority::rStringGridEd_tabGetCellParams(TObject *Sender,
          int Col, int Row, TFont *AFont, TColor &Background, bool &Highlight)

{
// 	Highlight=false;
//
//	if (Row==1 && scGPRadioButton1->Checked) {
//
//	Background=(TColor)RGB(202,217,240);
//	Highlight=true;
//
//	}
//		if (Row==2 && scGPRadioButton2->Checked) {
//
//		Background=(TColor)RGB(202,217,240);
//		Highlight=true;
//
//	}
//		if (Row==3 && scGPRadioButton3->Checked) {
//
//		Background=(TColor)RGB(202,217,240);
//		Highlight=true;
//
//	}
//		if (Row==4 && scGPRadioButton4->Checked) {
//
//		Background=(TColor)RGB(202,217,240);
//		Highlight=true;
//
//	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_PL_priority::scGPRadioButton1Click(TObject *Sender)
{   //workaround zvyraznovani radku aby fungovala konstukce s HighLight
//Form_PL_priority->rStringGridEd_tab->Visible=false;
//Form_PL_priority->rStringGridEd_tab->Visible=true;

}
//---------------------------------------------------------------------------

void __fastcall TForm_PL_priority::scGPRadioButton2Click(TObject *Sender)
{
//Form_PL_priority->rStringGridEd_tab->Visible=false;
//Form_PL_priority->rStringGridEd_tab->Visible=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm_PL_priority::scGPRadioButton3Click(TObject *Sender)
{
//Form_PL_priority->rStringGridEd_tab->Visible=false;
//Form_PL_priority->rStringGridEd_tab->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PL_priority::scGPRadioButton4Click(TObject *Sender)
{
//	Form_PL_priority->rStringGridEd_tab->Visible=false;
 //	Form_PL_priority->rStringGridEd_tab->Visible=true;
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
		 if(Button_save->Enabled)//pokud jsou zároveò splnìny podmínky pro stisk OK
		 {       ShowMessage("OK");
			Form_PL_priority->ModalResult=mrOk;//vrátí stejnou hodnotu jako tlaèítko
			Form_PL_priority->VisibleChanging();//skryje form, stejné jako visible=false
		 }
		 else MessageBeep(0);//pípnutím upozorní, že nelze
	}
	if(Key==27)//ESC
	{
		 Form_PL_priority->ModalResult=mrCancel;//vrátí stejnou hodnotu jako tlaèítko
		 Form_PL_priority->VisibleChanging();//skryje form, stejné jako visible=false
	}
}
//---------------------------------------------------------------------------
//køížek
void __fastcall TForm_PL_priority::KonecClick(TObject *Sender)
{
		 Form_PL_priority->ModalResult=mrCancel;//vrátí stejnou hodnotu jako tlaèítko
		 Form_PL_priority->VisibleChanging();//skryje form, stejné jako visible=false
}
//---------------------------------------------------------------------------
//dle kliknutí na øádek oznaèí combo v rStringGridEd
void __fastcall TForm_PL_priority::rStringGridEd_tabMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
//if (rStringGridEd_tab->Row==1) scGPRadioButton1->Checked=true;
//if (rStringGridEd_tab->Row==2) scGPRadioButton2->Checked=true;
//if (rStringGridEd_tab->Row==3) scGPRadioButton3->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_PL_priority::rStringGridEd_tabCanEdit(TObject *Sender, int Col,
          int Row, bool &CanEdit)
{
		if(Col==3 && !scGPRadioButton1->Checked)
		{

		 CanEdit=false;
		 Memo1->Lines->Add("jsem zde");

		}
}
//---------------------------------------------------------------------------


void __fastcall TForm_PL_priority::rStringGridEd_tabClick(TObject *Sender)
{

// nastavení podbarvení bunìk u nepoužívaného pohonu
//	 for (int i=1;i<rStringGridEd_tab->RowCount;i++)
//	 {       //nepoužívá se
//		 if(rStringGridEd_tab->Columns->Items[5]->Check )
//		 {
//			 if(Row==i && Col==3)	Background=(TColor)RGB(211,211,211);
//		 //	 if(Row==i && Col==7)	Background=(TColor)RGB(211,211,211);
//		 }
}
//---------------------------------------------------------------------------




void __fastcall TForm_PL_priority::rStringGridEd_tabFinishCellEditing(TObject *Sender,
					int Col, int Row)
{
		if(Col==2)     //v1
		{

			 if(rStringGridEd_tab->BoolCells[Col][Row]) // pokud je checked sloupec 2
			 {
					rStringGridEd_tab->BoolCells[Col+1][Row]=false; //zruším checked  na následujích sloupcích
					rStringGridEd_tab->BoolCells[Col+2][Row]=false;
					rStringGridEd_tab->BoolCells[Col+3][Row]=false;
			 }
		}

		if(Col==3)    //v3
		{

				if(rStringGridEd_tab->BoolCells[Col][Row]) // pokud je checked sloupec 3
			 {
				rStringGridEd_tab->BoolCells[Col-1][Row]=false; //zruším checked na pøedchozím sloupci
				rStringGridEd_tab->BoolCells[Col+1][Row]=false;
				rStringGridEd_tab->BoolCells[Col+2][Row]=false;

			 }

		}

		if(Col==4)    //v4
		{

				if(rStringGridEd_tab->BoolCells[Col][Row]) // pokud je checked sloupec 4
			 {
				rStringGridEd_tab->BoolCells[Col-1][Row]=false; //zruším checked na následujícím sloupci
				rStringGridEd_tab->BoolCells[Col-2][Row]=false;
				rStringGridEd_tab->BoolCells[Col-3][Row]=false;
			 }

		}



}
//---------------------------------------------------------------------------

