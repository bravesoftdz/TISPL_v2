//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "katalog_dopravniku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma link "rHintWindow"
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
	//načtení hodnot z PP
//	katalog_id=F->d.v.PP.katalog;
//	radius=F->d.v.PP.radius*1000.0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_katalog::FormShow(TObject *Sender)
{
	//načtení hodnot z PP
	katalog_id=F->d.v.PP.katalog;
	radius=F->d.v.PP.radius*1000.0;

	mGrid=new TmGrid(this);//vždy nutno jako první
  mGrid->Tag=8;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR

	Form_parametry_linky->scStyledForm2->InActiveClientBlurAmount=1;
  Form_parametry_linky->scStyledForm2->ShowClientInActiveEffect();

  F->scStyledForm1->InActiveClientBlurAmount=1;
  F->scStyledForm1->ShowClientInActiveEffect();

  ////////vytvoření tabulky s požadovaným počtem sloupců a řádků////////
	unsigned long ColCount=20;//pevný počet slopců
	unsigned long RowCount=15;//pevný počet řádků
  mGrid->Create(ColCount,RowCount);//samotné vytvoření matice-tabulky
  mGrid->Top=scLabel_header->Height + 10;


  mGrid->Columns[0].Width=120;
  for(int i=1;i<=ColCount;i++)
  {
    mGrid->Columns[i].Width=50;
  }
  mGrid->SetColumnAutoFit(-4);

  LoadValues(); //načtení hodnot do tabulky
  mGrid->Refresh();
  LoadStyles(); // načtení designu tabuly

  	mGrid->MergeCells(1,0,3,0);
    mGrid->MergeCells(4,0,7,0);
    mGrid->MergeCells(8,0,11,0);
    mGrid->MergeCells(12,0,15,0);
    mGrid->MergeCells(16,0,19,0);
  //pozice formu - střed obrazovky
  Left=Form1->ClientWidth/2-Form_katalog->Width/2;
  Top=Form1->ClientHeight/2-Form_katalog->Height/2;
  mGrid->Left=2;
	Form_katalog->Width=mGrid->Width + 4;
	Button_save->Top= mGrid->Height + scLabel_header->Height + 30;
	Button_storno->Top= mGrid->Height + scLabel_header->Height + 30;
	Form_katalog->Height = scLabel_header->Height +  mGrid->Height + Button_save->Height + 70;

	vypis("Kliknutím do seznamu rádiusů vyberete katalog dopravníků",false);
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

  //nastaveni barev pro zobrazeni vybrane hodnoty - oranzova
  for(int i=1;i<mGrid->ColCount;i++)
  {
   if(F->ms.MyToDouble(mGrid->Cells[i][katalog_id].Text) == radius)
	 {
		if(i>=8 && i<=11) mGrid->Cells[i][katalog_id].Font->Color=(TColor)RGB(226,122,21);
   }

  }
  //nastaveni barev pro zobrazeni vybrane hodnoty - cerna
  for(int i=1;i<mGrid->RowCount;i++)
  {
    if(i==katalog_id)
      {
       for(int j=0;j<mGrid->ColCount;j++)
       {
        if(F->ms.MyToDouble(mGrid->Cells[j][i].Text) != radius) mGrid->Cells[j][i].Font->Color=clBlack;
       }
      }
  }
}

void TForm_katalog::LoadStyles ()
{
 TColor clBACKGROUND_light,clBACKGROUND_dark;
 clBACKGROUND_light=Form1->m.clIntensive((TColor)RGB(200,200,200),35);
 clBACKGROUND_dark=clWhite;//Form1->m.clIntensive((TColor)RGB(240,240,240),35);

 for (int s = 0; s < mGrid->ColCount-1; s++)
 {  //černý font v hlavičcce
   mGrid->Cells[s][0].Font->Color=clBlack;
 }

  for (int r = 1; r <= mGrid->RowCount-1; r++)
 {
   mGrid->Cells[8][r].Type=mGrid->EDIT;
   mGrid->Cells[9][r].Type=mGrid->EDIT;
   mGrid->Cells[10][r].Type=mGrid->EDIT;
   mGrid->Cells[11][r].Type=mGrid->EDIT;
 }
}

void TForm_katalog::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{

 if(!zmena)
 {
   if(Col>=8 && Col<=11)
   {
    zmena=true;
      for(int i=1;i<mGrid->RowCount;i++)
      {
         if(i!=Row)
         {
            for(int j=0;j<mGrid->ColCount;j++)
             {
             mGrid->Cells[j][i].Font->Color=(TColor)RGB(43,87,154);  //odbarveni  - nevybrano
             }
         }
      }

   }
  zmena=false;
 }

 if(!zmena)
 {
   if(Col>=8 && Col<=11)
   {
    zmena=true;
      for(int i=1;i<mGrid->RowCount;i++)
      {
         if(i==Row)
         {
            for(int j=0;j<mGrid->ColCount;j++)
             {
             mGrid->Cells[j][i].Font->Color=clBlack;  //nabarveni - vybrano
             }
         }
      }

   }
  zmena=false;
 }
  //ziskani hodnot radius a katalog ID
  if(Col>=8 && Col<=11)
   {

   radius=Form1->ms.MyToDouble(mGrid->Cells[Col][Row].Text);
   katalog_id=Row;
   }


 FormPaint(this);
 Button_save->SetFocus();
}

void __fastcall TForm_katalog::Button_stornoClick(TObject *Sender)
{
F->scStyledForm1->HideClientInActiveEffect();
Form_parametry_linky->scStyledForm2->HideClientInActiveEffect();
Button_storno->SetFocus();//workaround proti padání mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na mGridu, na komponentu nemazanou
mGrid->Delete();

}
//---------------------------------------------------------------------------
void __fastcall TForm_katalog::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
mGrid->HighlightRowOnMouse(X,Y,(TColor)RGB(240,240,240));
}
//---------------------------------------------------------------------------


void __fastcall TForm_katalog::Button_saveClick(TObject *Sender)
{
F->d.v.PP.katalog=katalog_id;
F->d.v.PP.radius=radius/1000.0;

F->scStyledForm1->HideClientInActiveEffect();
Form_parametry_linky->scStyledForm2->HideClientInActiveEffect();

Button_storno->SetFocus();
//Form_parametry_linky->Close();

//Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_katalog::FormClose(TObject *Sender, TCloseAction &Action)
{
Button_storno->SetFocus();//workaround proti padání mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na mGridu, na komponentu nemazanou
mGrid->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TForm_katalog::KonecClick(TObject *Sender)
{
F->scStyledForm1->HideClientInActiveEffect();
Form_parametry_linky->scStyledForm2->HideClientInActiveEffect();
Button_storno->SetFocus();//workaround proti padání mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na mGridu, na komponentu nemazanou
mGrid->Delete();
Close();
}
//---------------------------------------------------------------------------
void TForm_katalog::vypis(UnicodeString text,bool red,bool link)
{
    F->log(__func__); //logování
		Button_save->Enabled=true;
		Button_save->Caption = "Uložit";
 //if(text=="m].</b>")text="";//provizorní WA, při změně Rz a byla-li v pořádku to vrací toto  - již není třeba, ale zatím nechávám
		if (text != "") // zobrazí a vypíše
		{
				rHTMLHint1->ToString()=text;//natežení do hintu zajišťuje zobrazení celého textu, nepoužívá se klasický hint
				//prodllužení formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl předtím popisek skrytý + kontrola pozice formu

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnutí podtrženého písma
				else rHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
						Button_save->Enabled=false;  //R - dočasné povolení ukládání při validaci
						rHTMLLabel_InfoText->Font->Color = clRed;
            rHTMLLabel_InfoText->Color=clWhite;
				}
				else
				{
						rHTMLLabel_InfoText->Font->Color = (TColor)RGB(0,128,255);
				}
        rHTMLLabel_InfoText->Top = mGrid->Height + scLabel_header->Height + 14;
        rHTMLLabel_InfoText->Left = 4;
				rHTMLLabel_InfoText->Caption = text;
				rHTMLLabel_InfoText->Visible = true;
        rHTMLLabel_InfoText->Color=clWhite;
		}
		else // skryje
		{
				//zkrácení formu if(rHTMLLabel_InfoText->Visible)Height-=(40+19);
				rHTMLLabel_InfoText->Visible = false;
		}
}
