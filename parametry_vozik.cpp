//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "parametry_vozik.h"
#include "unit1.h"
#include "parametry_linky.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scModernControls"
#pragma resource "*.dfm"
TForm_parametry_vozik *Form_parametry_vozik;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_vozik::TForm_parametry_vozik(TComponent* Owner)
	: TForm(Owner)
{
	Form1->m.designButton(Button_save,Form_parametry_vozik,1,2);
	Form1->m.designButton(Button_storno,Form_parametry_vozik,2,2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_vozik::FormShow(TObject *Sender)
{
	NastavDesign();

  if(Form_parametry_linky->Delkaunit==0)
	{
	scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig;//F->d.v.PP.delka_jig;
	scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig;//F->d.v.PP.sirka_jig;
	scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig;
	scGPNumericEdit_delka_podvozek->Value=F->d.v.PP.delka_podvozek;

  rHTMLLabel_delka_jig->Caption="délka <font color=#2b579a>[m]</font>";
  rHTMLLabel_sirka_jig->Caption="šíøka <font color=#2b579a>[m]</font>";
  rHTMLLabel_vyska_jig->Caption="výška <font color=#2b579a>[m]</font>";
  rHTMLLabel_delka_podvozek->Caption="délka <font color=#2b579a>[m]</font>";
  //ShowMessage(F->d.v.PP.delka_jig);

  } else
  {
  scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig*1000.0;
	scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig*1000.0;
	scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig*1000.0;
	scGPNumericEdit_delka_podvozek->Value=F->d.v.PP.delka_podvozek*1000.0;

  rHTMLLabel_delka_jig->Caption="délka <font color=#2b579a>[mm]</font>";
  rHTMLLabel_sirka_jig->Caption="šíøka <font color=#2b579a>[mm]</font>";
  rHTMLLabel_vyska_jig->Caption="výška <font color=#2b579a>[mm]</font>";
  rHTMLLabel_delka_podvozek->Caption="délka <font color=#2b579a>[mm]</font>";
  }
	Form_parametry_linky->Button_save->Enabled=false;
	Form_parametry_linky->Button_storno->Enabled=false;
	scGPNumericEdit_delka_jig->SetFocus();

 if(F->d.v.POHONY->predchozi->n!= Form_parametry_linky->rStringGridEd_tab_dopravniky->RowCount - 1)
 {
 rHTMLLabel_InfoText->Visible=true;
 rHTMLLabel_InfoText->Caption="Pøed zmìnou rozmìrù vozíku je nutné nejdøíve uložit zmìny pohonù.";
 Button_save->Enabled=false;

 } else { rHTMLLabel_InfoText->Visible=false;  Button_save->Enabled=true; }


}
//---------------------------------------------------------------------------

void  TForm_parametry_vozik::NastavDesign()
{
	Form_parametry_vozik->Color=(TColor)RGB(240,240,240);
	scGPButton_vozik->Options->NormalColor=Form_parametry_vozik->Color;
	scGPButton_vozik->Options->FocusedColor=Form_parametry_vozik->Color;
	scGPButton_vozik->Options->HotColor=Form_parametry_vozik->Color;
	scGPButton_vozik->Options->PressedColor=Form_parametry_vozik->Color;
	scGPButton_vozik->Options->FrameNormalColor=Form_parametry_vozik->Color;
	scGPButton_vozik->Options->PressedColor=Form_parametry_vozik->Color;
	scGPButton_vozik->Options->FramePressedColor=Form_parametry_vozik->Color;

	if(Form_parametry_linky->scGPSwitch->State==0) {rHTMLLabel_podvozek_zaves->Caption="Podvozek";   rHTMLLabel_podvozek_zaves->Left=34;  }
	else  { rHTMLLabel_podvozek_zaves->Caption="Závìs";  rHTMLLabel_podvozek_zaves->Left=56; }
}

void __fastcall TForm_parametry_vozik::scGPNumericEdit_delka_jigKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (Button_save->Enabled) Form_parametry_vozik->ModalResult = mrOk;// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;
		 case 27:// ESC
		 {
			Form_parametry_vozik->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			Form_parametry_vozik->VisibleChanging();// skryje form, stejné jako visible=false
			Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;

		 }break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_vozik::scGPNumericEdit_sirka_jigKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (Button_save->Enabled) Form_parametry_vozik->ModalResult = mrOk;// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;
		 case 27:// ESC
		 {
			Form_parametry_vozik->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			Form_parametry_vozik->VisibleChanging();// skryje form, stejné jako visible=false
			Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
		 }break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_vozik::scGPNumericEdit_vyska_jigKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (Button_save->Enabled) Form_parametry_vozik->ModalResult = mrOk;// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;
		 case 27:// ESC
		 {
			Form_parametry_vozik->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			Form_parametry_vozik->VisibleChanging();// skryje form, stejné jako visible=false
			Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
		 }break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_vozik::scGPNumericEdit_delka_podvozekKeyDown(TObject *Sender,
          WORD &Key, TShiftState Shift)
{
 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (Button_save->Enabled) Form_parametry_vozik->ModalResult = mrOk;// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;

		 case 27:// ESC
		 {
			Form_parametry_vozik->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			Form_parametry_vozik->VisibleChanging();// skryje form, stejné jako visible=false
			Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
		 }break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_vozik::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (Button_save->Enabled) Form_parametry_vozik->ModalResult = mrOk;// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;

		 case 27:// ESC
		 {
			Form_parametry_vozik->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			Form_parametry_vozik->VisibleChanging();// skryje form, stejné jako visible=false
			Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
		 }break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_vozik::KonecClick(TObject *Sender)
{
Form_parametry_linky->Button_save->Enabled=true;
Form_parametry_linky->Button_storno->Enabled=true;
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_parametry_vozik::Button_stornoClick(TObject *Sender)
{
	Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
}
//---------------------------------------------------------------------------




