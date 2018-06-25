//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TT_kalkulator.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_TT_kalkulator *Form_TT_kalkulator;
//---------------------------------------------------------------------------
__fastcall TForm_TT_kalkulator::TForm_TT_kalkulator(TComponent* Owner)
	: TForm(Owner)
{
	Form1->m.designButton(Button_save,Form_TT_kalkulator,1,2);
	Form1->m.designButton(Button_storno,Form_TT_kalkulator,2,2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_TT_kalkulator::KonecClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_TT_kalkulator::FormShow(TObject *Sender)
{

rEditNum_takt->Value=F->d.v.PP.TT;

Nastav_design();

}
//---------------------------------------------------------------------------


void TForm_TT_kalkulator::Nastav_design()
{
	 Form_TT_kalkulator->Color=(TColor)RGB(240,240,240);
	 scGPButton_takt->Options->NormalColor=Form_TT_kalkulator->Color;
	 scGPButton_takt->Options->FocusedColor=Form_TT_kalkulator->Color;
	 scGPButton_takt->Options->HotColor=Form_TT_kalkulator->Color;
	 scGPButton_takt->Options->PressedColor=Form_TT_kalkulator->Color;
	 scGPButton_takt->Options->FrameNormalColor=Form_TT_kalkulator->Color;
	 scGPButton_takt->Options->PressedColor=Form_TT_kalkulator->Color;
	 scGPButton_takt->Options->FramePressedColor=Form_TT_kalkulator->Color;


}

void __fastcall TForm_TT_kalkulator::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 if(Key==13)//ENTER
 {
		 if(Button_save->Enabled)//pokud jsou zároveò splnìny podmínky pro stisk OK
		 {
			Form_TT_kalkulator->ModalResult=mrOk;//vrátí stejnou hodnotu jako tlaèítko
			Form_TT_kalkulator->VisibleChanging();//skryje form, stejné jako visible=false
		 }
		 else MessageBeep(0);//pípnutím upozorní, že nelze
 }
 if(Key==27)//ESC
 {
		 Form_TT_kalkulator->ModalResult=mrCancel;//vrátí stejnou hodnotu jako tlaèítko
		 Form_TT_kalkulator->VisibleChanging();//skryje form, stejné jako visible=false
 }
}
//---------------------------------------------------------------------------

