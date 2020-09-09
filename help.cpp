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

//Product version: x.x (CZ - Verze aplikace)
//File version: x.x (CZ - Verze souboru)
//Vytvoøil: Karel Vomáèka , 19.7.2020 20:00 (EN Created by)
//Poslední zmìna : Milan Vomáèka, 21.7.2020 20:00 (EN Last modified)
//Location  (CZ Umístìní:....)
//Nastavení aplikace (Application Settings)  - viditelné pouze v DEBUG (je to tam již konzipováno)


	if(scGPToolPager->ActivePage==scGPToolPagerPage_about)
	{
		//deklarace pomocných atriburù
		UnicodeString nastaveni_app=F->get_temp_dir()+"\TISPL",umisteni=ExtractFilePath(Application->ExeName);
		umisteni=umisteni.SubString(1,umisteni.Length()-1);
		String about_text="";
		bool english=true;
		//naètení jazyka
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
								+"Vytvoøil: "+F->d.v.PP.vytvoril+" , "+DateToStr(F->d.v.PP.cas_start)+" "+TimeToStr(F->d.v.PP.cas_start)+"<br>"
								+"Posledni zmìna: "+F->d.v.PP.upravil+" , "+DateToStr(F->d.v.PP.cas_posledni_upravy)+" "+TimeToStr(F->d.v.PP.cas_posledni_upravy)+"<br>"
								+"<br><b>Aplikace</b><br>Verze aplikace: "+String(F->ProductVersion)+"<br>"
								+"Umístìní: <a href="+umisteni+">"+umisteni+"</a><br>";
								//+"Nastavení aplikace: <a>"+F->get_temp_dir()+"\TISPL</a>";
		}
    //pokud jsem v režimu debug, pøidat cestu k nastavení app
		if(DEBUG)
		{
			if(english)about_text+="Application Settings <a href="+nastaveni_app+">"+nastaveni_app+"</a>";
			else about_text+="Nastavení aplikace <a href="+nastaveni_app+">"+nastaveni_app+"</a>";
    }
		//zapsání do komponenty
		scHTMLLabel_about->Caption=about_text;
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm_help::scGPToolPagerClick(TObject *Sender)
{

  if(scGPToolPager->ActivePage==scGPToolPagerPage_PL)
     {
     UnicodeString PL_text;

     PL_text+="Hlavní formuláø aplikace, umožòuje zadat a spravovat následující parametry.</br>";
     PL_text+="<br><b>Takt Time</b> - zadání požadovaného taktu linky. Veškeré matematické výpoèty pøi následné tvorbì layoutu<br>jsou stavìny na dodržení taktu linky. Pøi tvorbì layoutu nelze Takt time již mìnit.<br>";
     PL_text+="<b>Rozmìry vozíku</b> - nastavení rozmìrù podvozku a jigu. Posuvníkem lze nastavit místo pro uchytávání palcù. <br>";
     PL_text+="<b>Dopravník</b> - pøed samotným vznikem pohonù je nutné vybrat z katalogu vhodný dopravník.<br>Po výbìru je možné vstoupit do tvorby pohonù.<b>";
     PL_text+="<br>Pohony</b> - tabulka pro správu pohonù. Pøi zadávání je kontrolováno rozmezí pohonu,<br>rozteè je nabízena dle vybraného dopravníku.<br>V pøípadì kdy je pohon používán, nelze na pohonu již mìnit jeho pracovní rozmezí.";

     scHTMLLabel_PL->Caption=PL_text;
     }

    if(scGPToolPager->ActivePage==scGPToolPagerPage_layout)
     {
     UnicodeString layout_text;

     layout_text+="Prostøedí na tvorbu layoutu je výchozím zobrazením pøi spuštìní aplikace.</br>";
     layout_text+="Skládá se z pracovní plochy, nabídky technologických objektù v levém panelu a nástrojové lišty v dolní èásti.<br>Postupným vkládáním technologických objektù z levého panelu vzniká layout s výchozími rozmìry objektù.<br>";
     layout_text+="Pøi dvojkliku na vložený objekt se zobrazí editace objektu.</br>Editaci je možné vyvolat i pøes pravý klik a v pop up menu vybrat editovat objekt.<br>";
     layout_text+="Volbou editovat se zmìní nabídka levého panelu a rozšíøí se panel nástrojù v dolní èásti pracovní plochy.</br>Levý panel je dynamicky zobrazován vždy dle aktuálnì nastavovaného objektu, nabídka pro objekty se tedy rùzní.";
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

