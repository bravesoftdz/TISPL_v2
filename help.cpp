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
//nastaven� v�choz� str�nky About
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

      UnicodeString about_text=Form1->ls->Strings[349]+" "+ F->VERZE +"<br>"+Form1->ls->Strings[350]+" "+ Application->ExeName + "<br>"+"Nastaven� aplikace: " + F->get_temp_dir();
      scHTMLLabel_about->Caption=about_text;
  }


}
//---------------------------------------------------------------------------


void __fastcall TForm_help::scGPToolPagerClick(TObject *Sender)
{

  if(scGPToolPager->ActivePage==scGPToolPagerPage_PL)
     {
     UnicodeString PL_text;

     PL_text+="Hlavn� formul�� aplikace, umo��uje zadat a spravovat n�sleduj�c� parametry.</br>";
     PL_text+="<br><b>Takt Time</b> - zad�n� po�adovan�ho taktu linky. Ve�ker� matematick� v�po�ty p�i n�sledn� tvorb� layoutu<br>jsou stav�ny na dodr�en� taktu linky. P�i tvorb� layoutu nelze Takt time ji� m�nit.<br>";
     PL_text+="<b>Rozm�ry voz�ku</b> - nastaven� rozm�r� podvozku a jigu. Posuvn�kem lze nastavit m�sto pro uchyt�v�n� palc�. <br>";
     PL_text+="<b>Dopravn�k</b> - p�ed samotn�m vznikem pohon� je nutn� vybrat z katalogu vhodn� dopravn�k.<br>Po v�b�ru je mo�n� vstoupit do tvorby pohon�.<b>";
     PL_text+="<br>Pohony</b> - tabulka pro spr�vu pohon�. P�i zad�v�n� je kontrolov�no rozmez� pohonu,<br>rozte� je nab�zena dle vybran�ho dopravn�ku.<br>V p��pad� kdy je pohon pou��v�n, nelze na pohonu ji� m�nit jeho pracovn� rozmez�.";

     scHTMLLabel_PL->Caption=PL_text;
     }
}
//---------------------------------------------------------------------------




