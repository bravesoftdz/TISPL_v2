//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "uvod.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_uvod *Form_uvod;
//---------------------------------------------------------------------------
__fastcall TForm_uvod::TForm_uvod(TComponent* Owner)
  : TForm(Owner)
{
	Form_uvod->Color=(TColor)RGB(26,87,178);//(TColor)RGB(17,58,150);//(TColor)RGB(43,87,154);//p�vodn� eltep barvy:(TColor)RGB(44,82,157);
	scGPLabel_TISPL->Font->Color=(TColor)RGB(220,220,220);
	n=0;
	//nastaven� popisku pro jazykov� mutace
	if(Form1->readINI("Nastaveni_app","jazyk")!="1")Label_status->Caption="Connecting to server ...";
}
//---------------------------------------------------------------------------
//redefinice, vytvo�� pop-up okno
void __fastcall TForm_uvod::CreateParams(TCreateParams &Params)
{
	TForm::CreateParams(Params);
	Params.Style=WS_POPUP|WS_DISABLED;
	Form_uvod->Color=(TColor)RGB(26,87,178);
}
//---------------------------------------------------------------------------
void __fastcall TForm_uvod::Timer1Timer(TObject *Sender)
{
	if(++n==9)Label_status->Visible=true;//po 0,75sec
	if(n==10)//a� p�i n-tem pr�chodu, tj. po 1 sec
	{
		Timer1->Enabled=false;//nezapomenout vypnout timer (potom m��u hledat chybu p�l dne...)
		Close();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_uvod::FormClose(TObject *Sender,TCloseAction &Action)
{
	AnimateWindow(Form1->Handle,1000,0x00080000);//pouze efekt
	Form1->Show();
	Form1->REFRESH();//tento refresh nen� t�eba pokud nebude AnimateWindow pou�ito
}
//---------------------------------------------------------------------------


