//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "konzole.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "scGPPagers"
#pragma link "scHtmlControls"
#pragma link "scModernControls"
#pragma link "scExtControls"
#pragma link "scGPImages"
#pragma link "scImageCollection"
#pragma resource "*.dfm"
TForm_konzole *Form_konzole;
//---------------------------------------------------------------------------
__fastcall TForm_konzole::TForm_konzole(TComponent* Owner)
  : TForm(Owner)
{
Text_formulare="";
priloha_cesta="";
}
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::FormShow(TObject *Sender)
{
	//centrov�n� formu
	Left=Form1->ClientWidth/2-Form_konzole->Width/2;
	Top=Form1->ClientHeight/2-Form_konzole->Height/2;

  Form1->m.designButton(scGPButton_odeslat,Form_konzole,1,2);
  Form1->m.designButton(scGPButton_storno,Form_konzole,2,2);
	//na�ten� jazykov� mutace
 //	scGPToolPager->Tabs->operator [](0)->Caption=F->ls->Strings[459];
	scLabel_header->Caption=F->ls->Strings[460];
  scGPImage1->ImageIndex=-1;
  priloha_cesta=F->get_temp_dir() +"TISPL\\" + "tispl_PrtScr"+F->get_user_name()+"_"+F->get_computer_name()+".png";
  scGPImage1->PngImage->LoadFromFile(priloha_cesta);
  scGPImage1->ImageIndex=0; //nutn� nastavit, nesta�� pouze cesta k filu
  scGPImage1->Stretch=true;
  Text->SetFocus();
 }
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::scGPButton_odeslatClick(TObject *Sender)
{
//mail odesl�n�
//ShowMessage(priloha_cesta);
//"c:\\Users\\rosta\\AppData\\Local\\Temp\\TISPL\\tispl_PrtScrrosta_ROSTA.png"
Text_formulare=Text->Lines->GetText(); // nahr�n� dat z Mema
F->mail("smtp.seznam.cz","builderboy@seznam.cz","camaro69","builderboy@seznam.cz","TISPL","error",Text_formulare,"rosta.slechta@gmail.com","","","c:\\Users\\rosta\\AppData\\Local\\Temp\\TISPL\\tispl_PrtScrrosta_ROSTA.png");
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_konzole::scGPButton_stornoClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

