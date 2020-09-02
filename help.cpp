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

//Product version: x.x (CZ - Verze aplikace)
//File version: x.x (CZ - Verze souboru)
//Vytvo�il: Karel Vom��ka , 19.7.2020 20:00 (EN Created by)
//Posledn� zm�na : Milan Vom��ka, 21.7.2020 20:00 (EN Last modified)
//Location  (CZ Um�st�n�:....)
//Nastaven� aplikace (Application Settings)  - viditeln� pouze v DEBUG (je to tam ji� konzipov�no)


	if(scGPToolPager->ActivePage==scGPToolPagerPage_about)
	{
		//deklarace pomocn�ch atribur�
		UnicodeString nastaveni_app=F->get_temp_dir()+"\TISPL",umisteni=ExtractFilePath(Application->ExeName);
		umisteni=umisteni.SubString(1,umisteni.Length()-1);
		String about_text="";
		bool english=true;
		//na�ten� jazyka
		if(F->readINI("Nastaveni_app","jazyk")=="1")english=false;
		//vypis textu
		if(english)
		{
			about_text="<b>Project</b><br>File version: "+String(F->FileVersion)+"<br>"
								+"Created by: "+F->d.v.PP.vytvoril+" , "+DateToStr(F->d.v.PP.cas_start)+" "+TimeToStr(F->d.v.PP.cas_start)+"<br>"
								+"Last modified: "+F->d.v.PP.upravil+" , "+DateToStr(F->d.v.PP.cas_posledni_upravy)+" "+TimeToStr(F->d.v.PP.cas_posledni_upravy)+"<br>"
								+"<br><b>Application</b><br>Product version: "+String(F->ProductVersion)+"<br>"
								+"Location: <a href="+umisteni+">"+umisteni+"</a><br>";
								//+"Application Settings: "+F->get_temp_dir();
		}
		else
		{
			about_text="<b>Projekt</b><br>Verze souboru: "+String(F->FileVersion)+"<br>"
								+"Vytvo�il: "+F->d.v.PP.vytvoril+" , "+DateToStr(F->d.v.PP.cas_start)+" "+TimeToStr(F->d.v.PP.cas_start)+"<br>"
								+"Posledni zm�na: "+F->d.v.PP.upravil+" , "+DateToStr(F->d.v.PP.cas_posledni_upravy)+" "+TimeToStr(F->d.v.PP.cas_posledni_upravy)+"<br>"
								+"<br><b>Aplikace</b><br>Verze aplikace: "+String(F->ProductVersion)+"<br>"
								+"Um�st�n�: <a href="+umisteni+">"+umisteni+"</a><br>";
								//+"Nastaven� aplikace: <a>"+F->get_temp_dir()+"\TISPL</a>";
		}
    //pokud jsem v re�imu debug, p�idat cestu k nastaven� app
		if(DEBUG)
		{
			if(english)about_text+="Application Settings <a href="+nastaveni_app+">"+nastaveni_app+"</a>";
			else about_text+="Nastaven� aplikace <a href="+nastaveni_app+">"+nastaveni_app+"</a>";
    }
		//zaps�n� do komponenty
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

    if(scGPToolPager->ActivePage==scGPToolPagerPage_layout)
     {
     UnicodeString layout_text;

     layout_text+="Prost�ed� na tvorbu layoutu je v�choz�m zobrazen�m p�i spu�t�n� aplikace.</br>";
     layout_text+="Skl�d� se z pracovn� plochy, nab�dky technologick�ch objekt� v lev�m panelu a n�strojov� li�ty v doln� ��sti.<br>Postupn�m vkl�d�n�m technologick�ch objekt� z lev�ho panelu vznik� layout s v�choz�mi rozm�ry objekt�.<br>";
     layout_text+="P�i dvojkliku na vlo�en� objekt se zobraz� editace objektu.</br>Editaci je mo�n� vyvolat i p�es prav� klik a v pop up menu vybrat editovat objekt.<br>";
     layout_text+="Volbou editovat se zm�n� nab�dka lev�ho panelu a roz���� se panel n�stroj� v doln� ��sti pracovn� plochy.</br>Lev� panel je dynamicky zobrazov�n v�dy dle aktu�ln� nastavovan�ho objektu, nab�dka pro objekty se tedy r�zn�.";
     layout_text+="";

     scHTMLLabel_layout->Caption=layout_text;
     }
}
//---------------------------------------------------------------------------




void __fastcall TForm_help::scHTMLLabel_aboutLinkClick(TObject *Sender, int LinkIndex,
					UnicodeString LinkText, UnicodeString LinkURL)
{
	if(LinkText!="")ShellExecute(0,L"open",LinkText.c_str(),0,0,SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

