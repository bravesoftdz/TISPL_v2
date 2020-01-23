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
//nastavení výchozí stránky About
scGPToolPager->ActivePage=scGPToolPagerPage_about;
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


   if(scGPToolPager->ActivePage==scGPToolPagerPage_about)
  {

      UnicodeString about_text=Form1->ls->Strings[349]+" "+ F->VERZE +"<br>"+Form1->ls->Strings[350]+" "+ Application->ExeName + "<br>"+"Nastavení aplikace: " + F->get_temp_dir();
      scHTMLLabel_about->Caption=about_text;
  }


}
//---------------------------------------------------------------------------


void __fastcall TForm_help::scGPToolPagerClick(TObject *Sender)
{

  if(scGPToolPager->ActivePage==scGPToolPagerPage_PL)
     {
     scLabel_PL->Caption="PL info";
     }

}
//---------------------------------------------------------------------------




