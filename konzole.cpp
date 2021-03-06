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

	//design tla��tek
  Form1->m.designButton(scGPButton_odeslat,Form_konzole,1,2);
	Form1->m.designButton(scGPButton_storno,Form_konzole,2,2);

	//za�krtnut� odesl�n� souboru projektu
	scGPCheckBox_odeslat_vcetne_projektu->Checked=true;

	//na�ten� jazykov� mutace
 //	scGPToolPager->Tabs->operator [](0)->Caption=F->ls->Strings[459];
	scLabel_header->Caption=F->ls->Strings[460];
  scGPImage1->ImageIndex=-1;
  priloha_cesta=F->get_temp_dir() +"TISPL\\" + "tispl_PrtScr"+F->get_user_name()+"_"+F->get_computer_name()+".png";
  scGPImage1->PngImage->LoadFromFile(priloha_cesta);
  scGPImage1->ImageIndex=0; //nutn� nastavit, nesta�� pouze cesta k filu
  //scGPImage1->Stretch=true;
  scGPImage1->Proportional=true;
  scGPImage1->Height=scGPImage1->PngImage->Height;
 Text->Clear();
 Text->Font->Style = TFontStyles()<< fsItalic;
 Text->Font->Color=(TColor)RGB(190,190,190);
 Text->Lines->Add(F->ls->Strings[473]);   //"pros�m, zadejte sv�j koment��" / please type in your comment
 count_click=0; //pomocn� po��tadlo klik�

	//na�ten� jazyka
	scLabel_header->Caption=F->ls->Strings[471];
	scLabel3->Caption=F->ls->Strings[472];
	scGPCheckBox_odeslat_vcetne_projektu->Caption=F->ls->Strings[474];
	scGPButton_odeslat->Caption=F->ls->Strings[475];
  scGPButton_storno->Caption=F->ls->Strings[71];
 }
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::scGPButton_odeslatClick(TObject *Sender)
{
	//mail odesl�n�
  String typ_zpravy;
  typ_zpravy= scGPComboBox1->Items->operator [](scGPComboBox1->ItemIndex)->Caption;
	Text_formulare=Text->Lines->GetText();// nahr�n� dat z Mema
	String projekt_cesta="";if(scGPCheckBox_odeslat_vcetne_projektu->Checked)projekt_cesta=F->FileName+".bac_"+F->get_user_name()+"_"+F->get_computer_name();
	if(F->mail("smtp.seznam.cz","builderboy@seznam.cz","camaro69","builderboy@seznam.cz","TISPL: "+typ_zpravy,F->LICENCE+"_"+F->get_computer_name()+"_"+F->get_user_name()+"_"+F->FileVersion,Text_formulare,"rosta.slechta@gmail.com","","",priloha_cesta,projekt_cesta))
  {
	F->zobraz_tip(F->ls->Strings[476]+"                       ");//"Odesl�no. D�kujeme za zp�tnou vazbu.", mezery nutn�, kv�li odsazen�
  Close();
  MessageBeep(MB_OK);//zvukov� signalizace
  Text->Clear(); //Sent. Thank you for your feedback.
  }
	else  F->MB(F->ls->Strings[477]);//"Nepoda�ilo se odeslat, zkuste znovu.", mezery nutn�, kv�li odsazen�
}

//---------------------------------------------------------------------------
void __fastcall TForm_konzole::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_konzole::TextClick(TObject *Sender)
{
  count_click++;
  if(count_click==1)  //prvn� klik do mema sma�e pr�vodn� text
  {
  Text->Clear();
  Text->Font->Style = TFontStyles(); // zru�en� kurz�vy a p��padn�ch dal��ch Font style nastaven�
  Text->Font->Color = clBlack;
  }
}
//---------------------------------------------------------------------------

