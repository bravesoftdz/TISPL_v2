//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "jig.h"
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
	//designov� nastaven� formul��e
	Form_jig->Color=(TColor)RGB(225,225,225);//RGB(43,87,154);
	scGPButton2->Options->NormalColor=Form_jig->Color;
	scGPButton2->Options->FocusedColor=Form_jig->Color;
	scGPButton2->Options->HotColor=Form_jig->Color;
	scGPButton2->Options->PressedColor=Form_jig->Color;
	scGPButton2->Options->FrameNormalColor=Form_jig->Color;
	scGPButton2->Options->PressedColor=Form_jig->Color;
 	scGPButton2->Options->FramePressedColor=Form_jig->Color;
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


