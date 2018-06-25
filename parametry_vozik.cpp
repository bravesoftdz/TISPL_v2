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

		 scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig;
		 scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig;
		 scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig;
		 scGPNumericEdit_delka_podvozek->Value=F->d.v.PP.delka_podvozek;
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
