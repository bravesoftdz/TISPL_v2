//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "casovaOsa_info.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm_osa_info *Form_osa_info;
//---------------------------------------------------------------------------
__fastcall TForm_osa_info::TForm_osa_info(TComponent* Owner)
	: TForm(Owner)
{

	Form1->m.designButton((TButton*)ioo_ok,Form_osa_info,1,1);
	TColor light_gray=(TColor)RGB(240,240,240);
	Form_osa_info->Color=light_gray;
  minsec="min";

}
//---------------------------------------------------------------------------
