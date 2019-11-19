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
   F->d.zobrazit_cele_zpravy=true;
   radek_temp=999;

  scGPPanel_header->FillColor=(TColor)RGB(60,100,162);
  Top=F->scLabel_titulek->Height + 0;
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
      I->Caption =AnsiString(F->d.getVID(Z->VID));
      if(Z->VID==0) I->ImageIndex=71;
      else I->ImageIndex=71;
      Z=Z->dalsi;
      pocet++;
		 }
		 delete Z;
     Form_zpravy->Height = pocet *  scGPListBox_zpravy->ItemHeight + scLabel1->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich
	 }
   F->REFRESH();
}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPListBox_zpravyMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
//F->Memo(floor(Y/(scGPListBox_zpravy->ItemHeight*1.0)));
   int radek= floor(Y/(scGPListBox_zpravy->ItemHeight*1.0));
   scGPListBox_zpravy->Items->Items[radek]->ImageIndex=72;
   radek_temp=radek;

   if(radek!=radek_temp) { scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=71;   radek_temp = radek;  }





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

void __fastcall TForm_zpravy::scGPListBox_zpravyClick(TObject *Sender)
{
//ShowMessage(scGPListBox_zpravy->ItemIndex +1);
//Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::SkrytClick(TObject *Sender)
{
ShowMessage("zde");
Form_zpravy->Visible=false;
ShowMessage("po");
}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scGPListBox_zpravyItemClick(TObject *Sender)
{
F->posun_na_element(scGPListBox_zpravy->ItemIndex +1);
}
//---------------------------------------------------------------------------




