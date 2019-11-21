//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "miniform_zpravy.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "RzStatus"
#pragma link "scGPImages"
#pragma resource "*.dfm"
TForm_zpravy *Form_zpravy;
//---------------------------------------------------------------------------
__fastcall TForm_zpravy::TForm_zpravy(TComponent* Owner)
  : TForm(Owner)
{
  Top=F->scLabel_titulek->Height;
  Left=F->ClientWidth - scGPListBox_zpravy->Width;

}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::FormShow(TObject *Sender)
{
   closing=false;
   F->scGPButton_warning->Visible=false;
   F->scGPButton_error->Visible=false;

  scGPPanel_header->FillColor=(TColor)RGB(60,100,162);
  if(Top!=F->scLabel_titulek->Height)
  {
  Top = F->Top_backup;
  Left = F->Left_backup;
  }
  //update_zpravy();  // vola si pri VALIDACI, neni nutne volat znovu pri formshow
}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPListBox_zpravyMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
   //highlight pri posunu mysi
   int radek= floor(Y/(scGPListBox_zpravy->ItemHeight*1.0));

     for(int i=0; i<scGPListBox_zpravy->Items->Count;i++)
     {
     if(i==radek)
     {  //bold
      if(scGPListBox_zpravy->Items->Items[radek]->ImageIndex==69)  { scGPListBox_zpravy->Items->Items[radek]->ImageIndex=70; }
      if(scGPListBox_zpravy->Items->Items[radek]->ImageIndex==71)  { scGPListBox_zpravy->Items->Items[radek]->ImageIndex=72; }
      F->d.zprava_highlight=radek+1;  F->REFRESH();
      radek_temp = radek;
     }    //ostatní nastav jako thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==70) scGPListBox_zpravy->Items->Items[i]->ImageIndex=69;  //error thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==72) scGPListBox_zpravy->Items->Items[i]->ImageIndex=71;  //warning thin
     }
}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPGlyphButton_infoClick(TObject *Sender)
{

 if(Top==F->scLabel_titulek->Height + 10) {Top=F->scLabel_titulek->Height + 10;   Left=F->ClientWidth - scGPListBox_zpravy->Width - 10;    }
 else
 {
  Top=F->scLabel_titulek->Height + 0;
  Left=F->ClientWidth - scGPListBox_zpravy->Width;
 }

}
//---------------------------------------------------------------------------


void __fastcall TForm_zpravy::SkrytClick(TObject *Sender)
{
  closing=true;
  F->scGPButton_warning->Visible=true;
  F->scGPButton_error->Visible=true;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPListBox_zpravyItemClick(TObject *Sender)
{
  F->Top_backup=Form_zpravy->Top;
  F->Left_backup=Form_zpravy->Left;
  closing=true;
  F->posun_na_element(scGPListBox_zpravy->ItemIndex +1);

}                                                  
//---------------------------------------------------------------------------

void  TForm_zpravy::update_zpravy(long pocet_erroru, long pocet_warningu)
{
  if(pocet_erroru>0 || pocet_warningu>0)
  {
   //pokud je form zpravy zobrazen tak schovej vzdy ikony v horni liste
  if(Form_zpravy->Visible) {   F->scGPButton_error->Visible=false; F->scGPButton_warning->Visible=false; }
  TscGPListBox *C= scGPListBox_zpravy;
  TscGPListBoxItem *I;

	C->Items->Clear();

	if(F->d.v.ZPRAVY!=NULL)
     {

       Cvektory::TZprava *Z=F->d.v.ZPRAVY->dalsi;
       while(Z!=NULL)
       {
        I=C->Items->Add();
        I->Caption =AnsiString(F->d.v.getVID(Z->VID));
        if(Z->zID==-1)  {I->ImageIndex=69;} //error
        else {I->ImageIndex=71;} //warning
        Z=Z->dalsi;
       }
       delete Z;
       RzStatusPane_pocet_chyb_value->Caption=pocet_erroru;
       RzStatusPane_pocet_var_value->Caption=pocet_warningu;
       Form_zpravy->Height = (pocet_erroru+pocet_warningu) *  scGPListBox_zpravy->ItemHeight + scLabel_header->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich

       if(Form_zpravy->Visible==false && pocet_erroru>0) F->scGPButton_error->Visible=true;  //pokud jsou zpravy zavreny a mám E tak v lištì zobraz ikonu E
       if(Form_zpravy->Visible==false && pocet_warningu>0) F->scGPButton_warning->Visible=true; //pokud jsou zpravy zavreny a mám W tak v lištì zobraz ikonu W
     }

	}
  if(pocet_erroru==0)
  {
   if(Form_zpravy->Visible==false) F->scGPButton_error->Visible=false; //pokud nejsou zadne E tak v liste nezobrazuj ikonu Erroru
  }
  if(pocet_warningu==0)
  {
   if(Form_zpravy->Visible==false) F->scGPButton_warning->Visible=false; //pokud nejsou zadne W tak v liste nezobrazuj ikonu Warningu
  }

	//else zavøít miniform + skrýt ikony w E
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPListBox_zpravyMouseLeave(TObject *Sender)
{ //pri opusteni zprav navratim posledni warning nebo error na pùvodní ikonu
  if(F->d.v.ZPRAVY!=NULL)
	 {
 if(scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex==70) scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=69;  //error thin
 if(scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex==72) scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=71;  //warning thin
	 }
}

//---------------------------------------------------------------------------

void TForm_zpravy::highlight(int radek)  {

 //highlight z layoutu
  if(Form_zpravy->Visible)
  {
  for(int i=0; i<scGPListBox_zpravy->Items->Count;i++)
     {
     if(i==radek)
     {  //bold
      if(scGPListBox_zpravy->Items->Items[radek]->ImageIndex==69)  { scGPListBox_zpravy->Items->Items[radek]->ImageIndex=70; }
      if(scGPListBox_zpravy->Items->Items[radek]->ImageIndex==71)  { scGPListBox_zpravy->Items->Items[radek]->ImageIndex=72; }
      F->d.zprava_highlight=radek+1;  F->REFRESH();
     }    //ostatní nastav jako thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==70) scGPListBox_zpravy->Items->Items[i]->ImageIndex=69;  //error thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==72) scGPListBox_zpravy->Items->Items[i]->ImageIndex=71;  //warning thin
     }

  }

}

