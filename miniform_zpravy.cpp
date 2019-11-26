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
	pocet_erroru=0;pocet_warningu=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::FormShow(TObject *Sender)
{
	closing=false;
  mouse_move=0;
	F->scGPButton_warning->Visible=false;
	F->scGPButton_error->Visible=false;
  scGPGlyphButton_pripnout->Visible=false;
  scGPSizeBox->Visible=false;
  custom_size=false;

  scGPPanel_header->FillColor=(TColor)RGB(60,100,162);
 // scGPPanel_header->FillColor=(TColor)RGB(221,221,221);
 // scGPPanel_statusbar->FillColor=(TColor)RGB(221,221,221);
//  scLabel_header->Font->Color=(TColor)RGB(73,117,184);
//  RzStatusPane__chyby_caption->Font->Color=(TColor)RGB(73,117,184);
 // RzStatusPane_var_header->Font->Color=(TColor)RGB(73,117,184);
  default_width=360;  //defaultní šíøka formuláøe - použita pøi pøipnutí ke stranì
  if(Top!=F->scLabel_titulek->Height)
  {
		Top = F->Top_backup;
		Left = F->Left_backup;
    scGPListBox_zpravy->Width = F->zpravy_backup_width;
    scGPListBox_zpravy->Height= F->zpravy_backup_height;
    scGPGlyphButton_pripnout->Visible=true;
    scGPSizeBox->Visible=true;
    custom_size=true;
	}
	TPoint pocet_zprav=F->d.v.vrat_pocet_zprav();
	update_zpravy(pocet_zprav.x,pocet_zprav.y);
  POCET_ERRORU = pocet_zprav.x;
  POCET_WARNINGU= pocet_zprav.y;
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
void __fastcall TForm_zpravy::scGPGlyphButton_pripnoutClick(TObject *Sender)
{
	scGPGlyphButton_pripnout->Visible=false;//pøidal MAKR
  scGPGlyphButton_pripnout->Visible=false;
  Form_zpravy->Width=default_width;
  //musí být vypoèítaná "Form_height" ale nelze to vzít pøímo z této promìnné protože mùže být custom, èili do podmínky musí být novì vypoèítána
  double vyska_temp = (POCET_ERRORU+POCET_WARNINGU) *  scGPListBox_zpravy->ItemHeight + scLabel_header->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich
  scGPSizeBox->Visible=false;
  if(vyska_temp > F->ClientHeight)   Form_zpravy->Height =  F->ClientHeight  -  scGPPanel_statusbar->Height - scLabel_header->Height - F->scLabel_titulek->Height;
  else Form_zpravy->Height = (POCET_ERRORU+POCET_WARNINGU) *  scGPListBox_zpravy->ItemHeight + scLabel_header->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich

	if(Top==F->scLabel_titulek->Height + 10) {Top=F->scLabel_titulek->Height + 10;   Left=F->ClientWidth - scGPListBox_zpravy->Width - 10;    }
	else
	{
		Top=F->scLabel_titulek->Height + 0;
		Left=F->ClientWidth - scGPListBox_zpravy->Width;

	}
   F->Memo(Form_zpravy->Height);
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::SkrytClick(TObject *Sender)
{
  F->Top_backup=Form_zpravy->Top;
  F->Left_backup=Form_zpravy->Left;
  F->zpravy_backup_width = scGPListBox_zpravy->Width;
  F->zpravy_backup_height = scGPListBox_zpravy->Height;
	closing=true;
	if(pocet_erroru)F->scGPButton_error->Visible=true;
	if(pocet_warningu)F->scGPButton_warning->Visible=true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPListBox_zpravyItemClick(TObject *Sender)
{
  F->Top_backup=Form_zpravy->Top;
  F->Left_backup=Form_zpravy->Left;
  closing=true;
  F->posun_na_element(scGPListBox_zpravy->ItemIndex +1);
 // scGPListBox_zpravy->Items->Items[scGPListBox_zpravy->ItemIndex]->Active=false;
}
//---------------------------------------------------------------------------
//update MaKr
void  TForm_zpravy::update_zpravy(long pocet_erroru, long pocet_warningu)
{
	TForm_zpravy::pocet_erroru=pocet_erroru;TForm_zpravy::pocet_warningu=pocet_warningu;//pøevzetí do globálních promìnných k dalšímu použítí a úspoøe strojového èasu

	if(pocet_erroru==0 && pocet_warningu==0)//POKUD NEJSOU žádné ZPRÁVY
	{
		scGPListBox_zpravy->Items->Clear();//vymazat pøípadné pùvodní
		Form_zpravy->Visible=false;//skrýt form
		F->scGPButton_error->Visible=false;F->scGPButton_warning->Visible=false;//skrýt ikony
	}
	else//POKUD EXISTUJÍ nìjaké ZPRÁVY
	{
		if(Form_zpravy->Visible)//pokud je form zpravy zobrazen,
		{
			//schovaní ikony v horni liste
			F->scGPButton_error->Visible=false;F->scGPButton_warning->Visible=false;

			//smazání a naplnìní novými zprávami, pokud existuji, to i ve spojaku (nemìlo by být sice v rozkolu, ale lépe ošetøit)
			if(F->d.v.ZPRAVY!=NULL)
			{
				//smazání pøípadných pùvodních zpráv
				scGPListBox_zpravy->Items->Clear();

				//naplnìní do listboxu
				Cvektory::TZprava *Z=F->d.v.ZPRAVY->dalsi;
				while(Z!=NULL)
				{
					TscGPListBoxItem *I=scGPListBox_zpravy->Items->Add();
					AnsiString Element="";if(Z!=NULL && Z->Element->name!="")Element=Z->Element->name.UpperCase()+": ";//získá název elementu u daného problému
					I->Caption=Element+F->d.v.getVID(Z->VID);
					if(Z->zID==-1)I->ImageIndex=69;//error
					else I->ImageIndex=71;//warning
					I=NULL;delete I;
					Z=Z->dalsi;
				}
				delete Z;
				//naplnìní do statusbaru miniformu
				//RzStatusPane_pocet_chyb_value->Caption=pocet_erroru;
        RzStatusPane__chyby_caption->Caption="Poèet chyb "+AnsiString(pocet_erroru);
			 //	RzStatusPane_pocet_var_value->Caption=pocet_warningu;
        RzStatusPane_var_header->Caption= "Poèet varování "+AnsiString(pocet_warningu);
			  if(custom_size==false)	Form_zpravy->Height = (pocet_erroru+pocet_warningu) *  scGPListBox_zpravy->ItemHeight + scLabel_header->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich
        if(Form_zpravy->Height > F->ClientHeight)   Form_zpravy->Height =  F->ClientHeight  -  scGPPanel_statusbar->Height - scLabel_header->Height - F->scLabel_titulek->Height;
       F->Memo(Form_zpravy->Height);
			}
		}
		else//pokud je form skrytý, zobrazí ikonu errorù nebo warningù (pøípadnì obì ikony) v horní lištì
		{
			if(pocet_erroru>0)	 F->scGPButton_error->Visible=true;  else F->scGPButton_error->Visible=false;  //pokud (ne)jsou zadne E tak v liste (ne)zobrazuj ikonu Erroru
			if(pocet_warningu>0) F->scGPButton_warning->Visible=true;else F->scGPButton_warning->Visible=false;//pokud (ne)jsou zadne W tak v liste (ne)zobrazuj ikonu Warningu
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPListBox_zpravyMouseLeave(TObject *Sender)
{ //pri opusteni zprav navratim posledni warning nebo error na pùvodní ikonu
	if(F->d.v.ZPRAVY!=NULL && 0<radek_temp && radek_temp<scGPListBox_zpravy->Items->Count)//DODAL MAKR
	 {
		if(scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex==70) scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=69;  //error thin
		if(scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex==72) scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=71;  //warning thin
	 }
	 //možná DOØEŠIT VÌTEV ELSE pro unhigliht
}
//---------------------------------------------------------------------------
void TForm_zpravy::highlight(int radek)
{
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
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPGlyphButton_pripnoutResize(TObject *Sender)
{
	scGPGlyphButton_pripnout->Visible=true;//pøidal MAKR
}
//---------------------------------------------------------------------------


void __fastcall TForm_zpravy::scLabel_headerMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
if(mouse_move==1)
  {
  scGPSizeBox->Visible=true;
  scGPGlyphButton_pripnout->Visible=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm_zpravy::scLabel_headerMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
mouse_move=1;
}
//---------------------------------------------------------------------------

