//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "jig.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_jig *Form_jig;
//---------------------------------------------------------------------------
__fastcall TForm_jig::TForm_jig(TComponent* Owner)
	: TForm(Owner)
{
	//na��t�n� a ukl�d�n� dat je realizov�no v superform.cpp
	//designov� nastaven� formul��e  - v�e v konstruktoru
	Form_jig->Color=(TColor)RGB(240,240,240);//RGB(43,87,154);
	scGPButton2->Options->NormalColor=Form_jig->Color;
	scGPButton2->Options->FocusedColor=Form_jig->Color;
	scGPButton2->Options->HotColor=Form_jig->Color;
	scGPButton2->Options->PressedColor=Form_jig->Color;
	scGPButton2->Options->FrameNormalColor=Form_jig->Color;
	scGPButton2->Options->PressedColor=Form_jig->Color;
	scGPButton2->Options->FramePressedColor=Form_jig->Color;

	Form_jig->Left=Form1->ClientWidth/2-Form_jig->Width/2;
	Form_jig->Top=Form1->ClientHeight/2-Form_jig->Height/2;

	Form1->m.designButton(Button_jig_save,Form_jig,1,2);
	Form1->m.designButton(Button_jig_storno,Form_jig,2,2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_jig::KonecClick(TObject *Sender)
{
	Form_jig->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_jig::Button_jig_stornoClick(TObject *Sender)
{
	Form_jig->Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm_jig::Edit_jig_pocet_ksKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 if(Button_jig_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_jig->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_jig->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm_jig::Edit_jig_vyskaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 if(Button_jig_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_jig->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_jig->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm_jig::Edit_jig_delkaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 if(Button_jig_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_jig->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_jig->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------


void __fastcall TForm_jig::Edit_jig_sirkaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 if(Button_jig_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_jig->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_jig->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm_jig::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 if(Button_jig_save->Enabled)//pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 {
			Form_jig->ModalResult=mrOk;//vr�t� stejnou hodnotu jako tla��tko
			Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
		 }
		 else MessageBeep(0);//p�pnut�m upozorn�, �e nelze
 }
 if(Key==27)//ESC
 {
		 Form_jig->ModalResult=mrCancel;//vr�t� stejnou hodnotu jako tla��tko
		 Form_jig->VisibleChanging();//skryje form, stejn� jako visible=false
 }
}
//---------------------------------------------------------------------------

