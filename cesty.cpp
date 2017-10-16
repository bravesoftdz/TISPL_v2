//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "cesty.h"
#include "superform.h"
#include "unit1.h"
//#include "dopravniky.h"
#include "jig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rStringGridEd"
#pragma link "RzButton"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_cesty *Form_cesty;
//---------------------------------------------------------------------------
__fastcall TForm_cesty::TForm_cesty(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm_cesty::cesta_stornoClick(TObject *Sender)
{
	Form_cesty->Close();
}
//---------------------------------------------------------------------------

void TForm_cesty::nactiNastaveni (){

		 Form_cesty->Color=(TColor)RGB(225,225,225);//RGB(43,87,154);
		 scGPButton2->Options->NormalColor=Form_cesty->Color;
		 scGPButton2->Options->FocusedColor=Form_cesty->Color;
		 scGPButton2->Options->HotColor=Form_cesty->Color;
		 scGPButton2->Options->PressedColor=Form_cesty->Color;
		 scGPButton2->Options->FrameNormalColor=Form_cesty->Color;


}

void __fastcall TForm_cesty::FormShow(TObject *Sender)
{
//zobraz data


}
//---------------------------------------------------------------------------

