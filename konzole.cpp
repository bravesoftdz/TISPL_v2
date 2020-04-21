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
}
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::FormShow(TObject *Sender)
{
	//centrování formu
	Left=Form1->ClientWidth/2-Form_konzole->Width/2;
	Top=Form1->ClientHeight/2-Form_konzole->Height/2;
	//naètení jazykové mutace
 //	scGPToolPager->Tabs->operator [](0)->Caption=F->ls->Strings[459];
	scLabel_header->Caption=F->ls->Strings[460];
  //scGPImageCollection1->Images->Add()->LoadPngFromFile(F->get_temp_dir() +"TISPL\\" + "tispl_PrtScr"+F->get_user_name()+"_"+F->get_computer_name()+".png");
  scGPImage1->PngImage->LoadFromFile(F->get_temp_dir() +"TISPL\\" + "tispl_PrtScr"+F->get_user_name()+"_"+F->get_computer_name()+".png");
  scGPImage1->ImageIndex=0; //nutné nastavit, nestaèí pouze cesta k filu
  scGPImage1->Stretch=true;
  Text->SetFocus();
 }
//---------------------------------------------------------------------------
void __fastcall TForm_konzole::scGPButton_ulozit_cestuClick(TObject *Sender)
{
//mail odeslání
F->mail("smtp.seznam.cz","builderboy@seznam.cz","camaro69","builderboy@seznam.cz","TISPL","error","ahoj","rosta.slechta@gmail.com","","","");
}
//---------------------------------------------------------------------------

