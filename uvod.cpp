//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "uvod.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_uvod *Form_uvod;
//---------------------------------------------------------------------------
__fastcall TForm_uvod::TForm_uvod(TComponent* Owner)
  : TForm(Owner)
{
	Form_uvod->Color=(TColor)RGB(43,87,154);//p�vodn� eltep barvy:(TColor)RGB(44,82,157);
	n=0;
}
//---------------------------------------------------------------------------
//redefinice, vytvo�� pop-up okno
void __fastcall TForm_uvod::CreateParams(TCreateParams &Params)
{
 TForm::CreateParams(Params);
 Params.Style=WS_POPUP|WS_DISABLED;
 Form_uvod->Color=(TColor)RGB(43,87,154);
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
void __fastcall TForm_uvod::FormClose(TObject *Sender,
			TCloseAction &Action)
{
	Form1->Show();
}
//---------------------------------------------------------------------------










