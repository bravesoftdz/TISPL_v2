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
	//naèítání a ukládání dat je realizováno v superform.cpp
	//designové nastavení formuláøe  - vše v konstruktoru
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


