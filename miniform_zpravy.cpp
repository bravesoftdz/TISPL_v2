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
#pragma resource "*.dfm"
TForm_zpravy *Form_zpravy;
//---------------------------------------------------------------------------
__fastcall TForm_zpravy::TForm_zpravy(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::FormShow(TObject *Sender)
{
  Top=F->scLabel_titulek->Height + 1;
  Left=F->ClientWidth - scGPListBox_zpravy->Width;


   TscGPListBox *C= scGPListBox_zpravy;
   TscGPListBoxItem *I;
   int pocet=0;
  	 if(F->d.v.ZPRAVY!=NULL)
	 {
		 Cvektory::TZprava *Z=F->d.v.ZPRAVY->dalsi;
		 while(Z!=NULL)
		 {
      I=C->Items->Add();
      I->Caption =AnsiString(Z->Popisek);
      if(Z->VID==0) I->ImageIndex=69;
      else I->ImageIndex=70;
      Z=Z->dalsi;
      pocet++;
		 }
		 delete Z;
     Form_zpravy->Height = pocet *  scGPListBox_zpravy->ItemHeight + scLabel1->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich
	 }

}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPListBox_zpravyMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
F->Memo(floor(Y/(scGPListBox_zpravy->ItemHeight*1.0)));
int radek= floor(Y/(scGPListBox_zpravy->ItemHeight*1.0));

//if(scGPListBox_zpravy->Items->Items[radek]->ImageIndex==70) scGPListBox_zpravy->Items->Items[radek]->ImageIndex=69;
//else scGPListBox_zpravy->Items->Items[radek]->ImageIndex=70;

}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPGlyphButton_infoClick(TObject *Sender)
{

 if(Top==F->scLabel_titulek->Height + 10) {Top=F->scLabel_titulek->Height + 10;   Left=F->ClientWidth - scGPListBox_zpravy->Width - 10;    }
 else
 {
  Top=F->scLabel_titulek->Height + 1;
  Left=F->ClientWidth - scGPListBox_zpravy->Width;
 }


}
//---------------------------------------------------------------------------

