//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MyMessageBox.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TMyMessageBox *MyMessageBox;
//---------------------------------------------------------------------------
__fastcall TMyMessageBox::TMyMessageBox(TComponent* Owner)
	: TForm(Owner)
{
	//nové nastavení barvy formuláøe
	MyMessageBox->Color=(TColor)RGB(240,240,240);
	ModalResult=false;
}
//---------------------------------------------------------------------------
void TMyMessageBox::dynamicka_velikost_formulare()
{
	//porovná, co je delší, zda Label èi Checkbox, podle toho s okrajem nastaví šíøku formuláøe
	if(Label1->Width+Label1->Left>CheckBox_pamatovat->Left+CheckBox_pamatovat->Width)
		MyMessageBox->Width=Label1->Width+(Label1->Left*2);
	else
		MyMessageBox->Width=(CheckBox_pamatovat->Width+(CheckBox_pamatovat->Left*2));

	Button_OK->Left=MyMessageBox->Width/2-Button_OK->Width/2;//vycentruje
	Button_Yes->Left=MyMessageBox->Width/2-Button_Yes->Width/2-Button_Yes->Width;
	Button_No->Left=MyMessageBox->Width/2+Button_No->Width/2+Button_No->Width;
}
//---------------------------------------------------------------------------
int TMyMessageBox::ShowMyMessageBox(long Left,long Top,UnicodeString Caption_text, UnicodeString Label1_text,bool checkbox_zobrazit)
{
	CheckBox_pamatovat->Visible=checkbox_zobrazit;
	MyMessageBox->Caption=Caption_text;
	MyMessageBox->Left=Left;
	MyMessageBox->Top=Top;
	Label1->Caption=Label1_text;
	dynamicka_velikost_formulare();//volání nastavení dynamická velikost formuláøe podle písma
	return ShowModal();
}
//---------------------------------------------------------------------------
//Køížek
void __fastcall TMyMessageBox::KonecClick(TObject *Sender)
{
	ModalResult=false;
	Close();
}
//---------------------------------------------------------------------------
//NE
void __fastcall TMyMessageBox::scGPButton3Click(TObject *Sender)
{
	KonecClick(Sender);
}
//---------------------------------------------------------------------------
//ANO
void __fastcall TMyMessageBox::scGPButton1_OKClick(TObject *Sender)
{
	ModalResult=true;
	Close();
}
//---------------------------------------------------------------------------

