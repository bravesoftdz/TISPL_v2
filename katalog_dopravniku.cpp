//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "katalog_dopravniku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_katalog *Form_katalog;
//---------------------------------------------------------------------------
__fastcall TForm_katalog::TForm_katalog(TComponent* Owner)
  : TForm(Owner)
{
 	//defaultní design a pozicování tlačítek OK a STORNO
	F->m.designButton(Button_save,Form_katalog,1,2);
	F->m.designButton(Button_storno,Form_katalog,2,2);
  zmena=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_katalog::FormShow(TObject *Sender)
{
	mGrid=new TmGrid(this);//vždy nutno jako první
  mGrid->Tag=8;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR

  ////////vytvoření tabulky s požadovaným počtem sloupců a řádků////////
	unsigned long ColCount=21;//pevný počet slopců
	unsigned long RowCount=15;//pevný počet řádků
  mGrid->Create(ColCount,RowCount);//samotné vytvoření matice-tabulky
  mGrid->Top=scLabel_header->Height + 10;
  mGrid->Left= 20;

  mGrid->Columns[0].Width=120;
  for(int i=1;i<=ColCount;i++)
  {
    mGrid->Columns[i].Width=50;
  }
  mGrid->SetColumnAutoFit(-4);

  LoadValues(); //načtení hodnot do tabulky
  LoadStyles(); // načtení designu tabuly

  	mGrid->MergeCells(1,0,3,0);
    mGrid->MergeCells(4,0,7,0);
    mGrid->MergeCells(8,0,11,0);
    mGrid->MergeCells(12,0,15,0);
    mGrid->MergeCells(16,0,19,0);
  //pozice katalogu - střed obrazovky
  Left=Form1->ClientWidth/2-Form_katalog->Width/2;
  Top=Form1->ClientHeight/2-Form_katalog->Height/2;
  //mGrid->Refresh();  //kvůli getCheck nutný refresh, jinak pamět chyba
 // if(F->d.v.PP.katalog==0) mGrid->getCheck(6,1)->Checked=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_katalog::FormPaint(TObject *Sender)
{
  mGrid->Show();//vykreslí tabulku
}
//---------------------------------------------------------------------------

void TForm_katalog::LoadValues ()
{
  // vytvoření hlavičky
  mGrid->Cells[0][0].Text="CALDAN";
  mGrid->Cells[1][0].Text="Rozteč palců [mm]";
  mGrid->Cells[4][0].Text="Horizontální oblouky [°]";
  mGrid->Cells[8][0].Text="Rádius [mm]";
	mGrid->Cells[12][0].Text="Vertikální oblouky [°]";
  mGrid->Cells[16][0].Text="Rádius [mm]";

  Cvektory::Ttyp_dopravniku *K=F->d.v.KATALOG->dalsi;//přeskočí hlavičku
	while(K!=NULL)
	{
    //funkční část
    mGrid->Cells[0][K->n].Text=K->name;
    Cvektory::TDoubleHodnota *H=K->roztec->dalsi;

    //pro rozteče
    while(H!=NULL)
	  {
      mGrid->Cells[H->n][K->n].Text=H->hodnota;
      H=H->dalsi;
    }

    //pro horizontální oblouky
    H=K->hOblouk->dalsi;
    while(H!=NULL)
	  {
      mGrid->Cells[3+H->n][K->n].Text=H->hodnota;
      H=H->dalsi;
    }

    //Hradius
    H=K->hRadius->dalsi;
    while(H!=NULL)
	  {
      mGrid->Cells[7+H->n][K->n].Text=H->hodnota;
      H=H->dalsi;
    }

    //V oblouk
    H=K->vOblouk->dalsi;
    while(H!=NULL)
	  {
      mGrid->Cells[11+H->n][K->n].Text=H->hodnota;
      H=H->dalsi;
    }

    //V radius
    H=K->vRadius->dalsi;
    while(H!=NULL)
	  {
      mGrid->Cells[15+H->n][K->n].Text=H->hodnota;
      H=H->dalsi;
    }

    //ukazatelová část
    delete H;H=NULL;
    K=K->dalsi;
	}
  delete K;K=NULL;

}

void TForm_katalog::LoadStyles ()
{
 TColor clBACKGROUND_light,clBACKGROUND_dark;
 clBACKGROUND_light=Form1->m.clIntensive((TColor)RGB(200,200,200),35);
 clBACKGROUND_dark=clWhite;//Form1->m.clIntensive((TColor)RGB(240,240,240),35);
 //clCHECKED=
 for(int i=0;i<mGrid->RowCount;i++)
 {
  mGrid->Cells[0][i].Align=mGrid->LEFT;
     if(i%2 && i>0)  //obarvování řádků - sudý a lichý
     {
      for(int j=0; j<mGrid->ColCount-1;j++) //poslední sloupec nebudu nikdy obarvovat
      {
      mGrid->Cells[j][i].Background->Color=clBACKGROUND_dark;
      }
     }
    else
      {
         for(int j=0; j<mGrid->ColCount-1;j++)//poslední sloupec nebudu nikdy obarvovat
         {
          mGrid->Cells[j][i].Background->Color=clBACKGROUND_light;
         }
     }
 }
 for (int s = 0; s < mGrid->ColCount-1; s++)
 {  //černý font v hlavičcce
   mGrid->Cells[s][0].Font->Color=clBlack;
 }


}

void TForm_katalog::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
// if(!zmena) //bool hodnota, ošetření kvůli cyklení onclick události (programové nastavení způsobuje volání onclick!)
// {
//      zmena=true;
//      for(int i=1;i<mGrid->RowCount;i++)
//      {
//      if(i!=Row)
//          {
//          mGrid->getCheck(Col,i)->Checked=false;
//         	mGrid->Cells[0][i].Font->Color=clBlack;
//
//          }
//      }
//  }
//  zmena=false;
//
//
//  if(mGrid->getCheck(Col,Row)->Checked)
//  {
//  mGrid->Cells[0][Row].Font->Color=clBlue;
//  }
//
//  FormPaint(this);

}

void __fastcall TForm_katalog::Button1Click(TObject *Sender)
{
mGrid->getCheck(6,2)->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_katalog::Button_stornoClick(TObject *Sender)
{
mGrid->Delete();
Close();
}
//---------------------------------------------------------------------------
