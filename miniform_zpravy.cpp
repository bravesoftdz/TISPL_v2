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
	F->scGPButton_warning->Visible=false;
	F->scGPButton_error->Visible=false;

  scGPPanel_header->FillColor=(TColor)RGB(60,100,162);
  if(Top!=F->scLabel_titulek->Height)
  {
		Top = F->Top_backup;
		Left = F->Left_backup;
	}
	TPoint pocet_zprav=F->d.v.vrat_pocet_zprav();
	update_zpravy(pocet_zprav.x,pocet_zprav.y);
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
     }    //ostatn� nastav jako thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==70) scGPListBox_zpravy->Items->Items[i]->ImageIndex=69;  //error thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==72) scGPListBox_zpravy->Items->Items[i]->ImageIndex=71;  //warning thin
     }
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPGlyphButton_infoClick(TObject *Sender)
{
	scGPGlyphButton_info->Visible=false;//p�idal MAKR
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
}
//---------------------------------------------------------------------------
//update MaKr
void  TForm_zpravy::update_zpravy(long pocet_erroru, long pocet_warningu)
{
	TForm_zpravy::pocet_erroru=pocet_erroru;TForm_zpravy::pocet_warningu=pocet_warningu;//p�evzet� do glob�ln�ch prom�nn�ch k dal��mu pou��t� a �spo�e strojov�ho �asu

	if(pocet_erroru==0 && pocet_warningu==0)//POKUD NEJSOU ��dn� ZPR�VY
	{
		scGPListBox_zpravy->Items->Clear();//vymazat p��padn� p�vodn�
		Form_zpravy->Visible=false;//skr�t form
		F->scGPButton_error->Visible=false;F->scGPButton_warning->Visible=false;//skr�t ikony
	}
	else//POKUD EXISTUJ� n�jak� ZPR�VY
	{
		if(Form_zpravy->Visible)//pokud je form zpravy zobrazen,
		{
			//schovan� ikony v horni liste
			F->scGPButton_error->Visible=false;F->scGPButton_warning->Visible=false;

			//smaz�n� a napln�n� nov�mi zpr�vami, pokud existuji, to i ve spojaku (nem�lo by b�t sice v rozkolu, ale l�pe o�et�it)
			if(F->d.v.ZPRAVY!=NULL)
			{
				//smaz�n� p��padn�ch p�vodn�ch zpr�v
				scGPListBox_zpravy->Items->Clear();

				//napln�n� do listboxu
				Cvektory::TZprava *Z=F->d.v.ZPRAVY->dalsi;
				while(Z!=NULL)
				{
					TscGPListBoxItem *I=scGPListBox_zpravy->Items->Add();
					AnsiString Element="";if(Z!=NULL && Z->Element->name!="")Element=Z->Element->name.UpperCase()+": ";//z�sk� n�zev elementu u dan�ho probl�mu
					I->Caption=Element+F->d.v.getVID(Z->VID);
					if(Z->zID==-1)I->ImageIndex=69;//error
					else I->ImageIndex=71;//warning
					I=NULL;delete I;
					Z=Z->dalsi;
				}
				delete Z;
				//napln�n� do statusbaru miniformu
				RzStatusPane_pocet_chyb_value->Caption=pocet_erroru;
				RzStatusPane_pocet_var_value->Caption=pocet_warningu;
				Form_zpravy->Height = (pocet_erroru+pocet_warningu) *  scGPListBox_zpravy->ItemHeight + scLabel_header->Height + scGPPanel_statusbar->Height + 5;   //5px rezervnich
			}
		}
		else//pokud je form skryt�, zobraz� ikonu error� nebo warning� (p��padn� ob� ikony) v horn� li�t�
		{
			if(pocet_erroru>0)	 F->scGPButton_error->Visible=true;  else F->scGPButton_error->Visible=false;  //pokud (ne)jsou zadne E tak v liste (ne)zobrazuj ikonu Erroru
			if(pocet_warningu>0) F->scGPButton_warning->Visible=true;else F->scGPButton_warning->Visible=false;//pokud (ne)jsou zadne W tak v liste (ne)zobrazuj ikonu Warningu
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPListBox_zpravyMouseLeave(TObject *Sender)
{ //pri opusteni zprav navratim posledni warning nebo error na p�vodn� ikonu
	if(F->d.v.ZPRAVY!=NULL && 0<radek_temp && radek_temp<scGPListBox_zpravy->Items->Count)//DODAL MAKR
	 {
		if(scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex==70) scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=69;  //error thin
		if(scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex==72) scGPListBox_zpravy->Items->Items[radek_temp]->ImageIndex=71;  //warning thin
	 }
	 //mo�n� DO�E�IT V�TEV ELSE pro unhigliht
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
     }    //ostatn� nastav jako thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==70) scGPListBox_zpravy->Items->Items[i]->ImageIndex=69;  //error thin
     else if  (scGPListBox_zpravy->Items->Items[i]->ImageIndex==72) scGPListBox_zpravy->Items->Items[i]->ImageIndex=71;  //warning thin
     }
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_zpravy::scGPGlyphButton_infoResize(TObject *Sender)
{
	scGPGlyphButton_info->Visible=true;//p�idal MAKR
}
//---------------------------------------------------------------------------

