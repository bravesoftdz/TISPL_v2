//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "help.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scExtControls"
#pragma link "scToolPager"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scGPPagers"
#pragma link "scAdvancedControls"
#pragma link "scAppPager"
#pragma link "scGPFontControls"
#pragma link "scModernControls"
#pragma link "scStyledForm"
#pragma link "scStyleManager"
#pragma link "scHtmlControls"
#pragma resource "*.dfm"
TForm_help *Form_help;
//---------------------------------------------------------------------------
__fastcall TForm_help::TForm_help(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm_help::CloseButtonClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_help::FormShow(TObject *Sender)
{
Left=Form1->ClientWidth/2-Form_help->Width/2;
Top=Form1->ClientHeight/2-Form_help->Height/2;
}
//---------------------------------------------------------------------------


void __fastcall TForm_help::scGPToolPagerClick(TObject *Sender)
{

if(scGPToolPager->ActivePage==scGPToolPagerPage_about)
{

   	UnicodeString text="<b>"+Form1->ls->Strings[349]+" </b>",text_1="<br><br><b>"+Form1->ls->Strings[350]+" </b>",text_2=Form1->ls->Strings[351];//"<b>Verze: </b>","<br><br><b>Umístìní: </b>","Informace o aplikaci"
 //	MB(-1,-1,text+VERZE+text_1+Application->ExeName+"<br><br><b>Temp : </b>"+get_temp_dir(),text_2);

  scHTMLLabel_about->Caption=text;
}
}
//---------------------------------------------------------------------------

