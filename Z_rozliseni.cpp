//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "Z_rozliseni.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma link "scModernControls"
#pragma link "scHtmlControls"
#pragma resource "*.dfm"
TForm_Z_rozliseni *Form_Z_rozliseni;
//---------------------------------------------------------------------------
__fastcall TForm_Z_rozliseni::TForm_Z_rozliseni(TComponent* Owner)
	: TForm(Owner)
{
	//Formul��
	Form_Z_rozliseni->Width=Screen->WorkAreaWidth;
	Form_Z_rozliseni->Height=Screen->WorkAreaHeight;
	Form_Z_rozliseni->Left=Form1->ClientWidth/2.0-Form_Z_rozliseni->Width/2.0;
	Form_Z_rozliseni->Top=Form1->ClientHeight/2.0-Form_Z_rozliseni->Height/2.0;
	//Nadpis part1
	scHTMLLabel_upozorneni->Top = scGPPanel->Height + scGPPanel->Height/2;
	//Tla��tka
	F->m.designButton(scGPButton_OK, Form_Z_rozliseni, 1, 2);
	F->m.designButton(scGPButton_storno, Form_Z_rozliseni, 2, 2);
	//tla��tko pro p�ep�n�n� jazyka
	scLabel_EN->Top=scGPPanel->Height/2.0-scLabel_EN->Height/2.0;
	scGPSwitch_language->Top=scGPPanel->Height/2.0-scGPSwitch_language->Height/2.0;
	scLabel_CZ->Top=scGPPanel->Height/2.0-scLabel_CZ->Height/2.0;
	scLabel_CZ->Left=Form_Z_rozliseni->Width-2*MinButton->Width-scLabel_CZ->Width-5;
	scGPSwitch_language->Left=scLabel_CZ->Left-scGPSwitch_language->Width-5;
	scLabel_EN->Left=scGPSwitch_language->Left-scLabel_EN->Width-5;
	//jazykov� mutace
	ls=new TStringList;
	UnicodeString text="";
	for(unsigned int i=0;i<=6;i++)
	{
		switch(i)
		{
			case 0:text="Exit;Ukon�it";break;
			case 1:text="Cancel;Storno";break;
			case 2:text="Change resolution;Zm�na rozli�en�";break;
			case 3:text="<font color=#2b579a> The scale setting on your computer is not compatible with this application. For proper operation, change the application settings using this manual. </font>;<font color=#2b579a>Nastaven� m���tka na va�em po��ta�i nen� kompatibiln� s touto aplikac�. Pro spr�vn� chod zm��te nastaven� aplikace pomoc� tohoto n�vodu.</font>";break;
			case 4:text="1. Right-click the <font color=#2b579a> TISPL.exe </font> file to display the <br><font color=#2b579a> Properties </font> file. Now go to the <font color=#2b579a> Compatibility </font> tab. <br>Click here the button shown in Figure 1.;1. Klikn�te prav�m tla��tekm my�i na soubor <br><font color=#2b579a>TISPL.exe</font>, zobrazte <font color=#2b579a>Vlastnosti</font> souboru. Nyn� <br>p�ejd�te do z�lo�ky <font color=#2b579a>Kompatibilita</font>. Zde klikn�te na <br>tla��tko zn�zorn�n� na obr�zku �. 1.";break;
			case 5:text="2. Figure 2 shows the default settings for DPI <br>values, these values must be adjusted for optimal <br>display.;2. Na obr�zku �. 2 je zobrazeno z�kladn� nastaven� <br>hodnot DPI, pro optim�ln� zobrazen� je nutn� tyto <br>hodnoty upravit.";break;
			case 6:text="3. Adjust the DPI values as shown in Figure 3. <br>Click Exit to restart the application.;3. Upravte hodnoty DPI podle vyzna�en� na <br>obr�zku �. 3. Klikn�te na tla��tko ukon�it a spus�e <br>aplikaci znova.";break;
		}
		ls->Insert(i,text);
	}
	zmena_jazyka();//prvotn� na�ten� caption� do komponent
}
//---------------------------------------------------------------------------
void TForm_Z_rozliseni::zmena_jazyka()
{
	delete ls_temp;ls_temp=NULL;
	ls_temp=new TStringList;
	UnicodeString text="";
  //na�ten� aktu�ln�ho jazyka
	for(unsigned int i=0;i<ls->Count;i++)
	{
		switch((int)scGPSwitch_language->State)
		{
			case 0:text=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);break;
			case 1:text=ls->Strings[i].SubString(ls->Strings[i].Pos(";")+1,ls->Strings[i].Length());break;
		}
		ls_temp->Insert(i,text);
	}
	//p�epnut� obr�zk�
	switch((int)scGPSwitch_language->State)
	{
		case 0:
		{
			Image1->Visible=false;
     	Image2->Visible=false;
     	Image3->Visible=false;
     	Image4->Visible=true;
     	Image5->Visible=true;
			Image6->Visible=true;
		}break;
		case 1:
		{
			Image1->Visible=true;
    	Image2->Visible=true;
    	Image3->Visible=true;
    	Image4->Visible=false;
    	Image5->Visible=false;
			Image6->Visible=false;
		}break;
	}
	//na�ten� do komponent
	scLabel1->Caption=ls_temp->Strings[2];//"Zm�na rozli�en�"
	scHTMLLabel_upozorneni->Caption=ls_temp->Strings[3];
	scHTMLLabel_Text1->Caption=ls_temp->Strings[4];
	scHTMLLabel_Text2->Caption=ls_temp->Strings[5];
	scHTMLLabel_Text3->Caption=ls_temp->Strings[6];
	scGPButton_OK->Caption=ls_temp->Strings[0];//"Ukon�it";
	scGPButton_storno->Caption=ls_temp->Strings[1];//"Storno";
	//zm�na zarovn�n�, jeden obr�zek je vet�� ne� druh�
	if(Image1->Visible)scHTMLLabel_Text1->Top = Image1->Top + Image1->Height + scGPPanel->Height/2.0;
	else scHTMLLabel_Text1->Top = Image4->Top + Image4->Height + scGPPanel->Height/2.0;
}
void __fastcall TForm_Z_rozliseni::KonecClick(TObject *Sender)
{
	F->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Z_rozliseni::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Z_rozliseni::MinButtonClick(TObject *Sender)
{
	Application->Minimize();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Z_rozliseni::FormShow(TObject *Sender)
{
	F->log(__func__);//logov�n�
	//Obr�zky
	Image1->AutoSize = true;
	Image1->Left = Form_Z_rozliseni->Width/3.0 -Form_Z_rozliseni->Width/3/2 - Image1->Width/2.0;
	Image1->Top = scHTMLLabel_upozorneni->Top + scHTMLLabel_upozorneni->Height + scGPPanel->Height/2.0;

	Image2->AutoSize = true;
	Image2->Left = 2*Form_Z_rozliseni->Width/3.0 -Form_Z_rozliseni->Width/3.0/2.0 - Image1->Width/2.0;
	Image2->Top = Image1->Top;

	Image3->AutoSize = true;
	Image3->Left = Form_Z_rozliseni->Width -Form_Z_rozliseni->Width/3.0/2.0 - Image1->Width/2.0;
	Image3->Top = Image1->Top;

	Image4->AutoSize = true;
	Image4->Left = Form_Z_rozliseni->Width/3.0 -Form_Z_rozliseni->Width/3.0/2.0 - Image1->Width/2.0;
	Image4->Top = scHTMLLabel_upozorneni->Top + scHTMLLabel_upozorneni->Height + scGPPanel->Height/2.0;

	Image5->AutoSize = true;
	Image5->Left = 2*Form_Z_rozliseni->Width/3.0 -Form_Z_rozliseni->Width/3.0/2.0 - Image1->Width/2.0;
	Image5->Top = Image1->Top;

	Image6->AutoSize = true;
	Image6->Left = Form_Z_rozliseni->Width -Form_Z_rozliseni->Width/3.0/2.0 - Image1->Width/2.0;
	Image6->Top = Image1->Top;
	//Nadpis part2
	scHTMLLabel_upozorneni->Left = Form_Z_rozliseni->Width/2.0 - scHTMLLabel_upozorneni->Width/2.0;
	//Text
	scHTMLLabel_Text1->Width = Image1->Width;
	scHTMLLabel_Text1->AutoSizeHeight = true;
	scHTMLLabel_Text1->Left = Image1->Left;
	if(Image1->Visible)scHTMLLabel_Text1->Top = Image1->Top + Image1->Height + scGPPanel->Height/2.0;
	else scHTMLLabel_Text1->Top = Image4->Top + Image4->Height + scGPPanel->Height/2.0;

	scHTMLLabel_Text2->Width = Image2->Width;
	scHTMLLabel_Text2->Left = Image2->Left;
	scHTMLLabel_Text2->Top = Image2->Top + Image2->Height + scGPPanel->Height/2.0;

	scHTMLLabel_Text3->Width = Image3->Width;
	scHTMLLabel_Text3->Left = Image3->Left;
	scHTMLLabel_Text3->Top = Image3->Top + Image3->Height + scGPPanel->Height/2.0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Z_rozliseni::scGPSwitch_languageChangeState(TObject *Sender)
{
	zmena_jazyka();
}
//---------------------------------------------------------------------------

