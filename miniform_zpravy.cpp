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
      if(Z->VID==0) I->ImageIndex=70;
      else I->ImageIndex=69;
      Z=Z->dalsi;
      pocet++;
		 }
		 delete Z;
     Form_zpravy->Height = pocet *  scGPListBox_zpravy->ItemHeight + scLabel1->Height + 5;   //5px rezervnich
	 }

}
//---------------------------------------------------------------------------

