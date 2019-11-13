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
}
//---------------------------------------------------------------------------
void __fastcall TForm_katalog::FormShow(TObject *Sender)
{
	//načtení hodnot z PP
	katalog_id=F->d.v.PP.katalog;
	radius=F->d.v.PP.radius*1000.0;
	//pokud nebyl žádny dopravník ještě uložen vezmě si data z temp dopravníku na PL
	if(katalog_id==0)
	{
		katalog_id=Form_parametry_linky->katalog_id;
		radius=Form_parametry_linky->radius*1000.0;
  }

	K_mGrid=new TmGrid(this);//vždy nutno jako první
  K_mGrid->Tag=8;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR

	Form_parametry_linky->scStyledForm2->InActiveClientBlurAmount=1;
  Form_parametry_linky->scStyledForm2->ShowClientInActiveEffect();

  F->scStyledForm1->InActiveClientBlurAmount=1;
  F->scStyledForm1->ShowClientInActiveEffect();

  ////////vytvoření tabulky s požadovaným počtem sloupců a řádků////////
	unsigned long ColCount=20;//pevný počet slopců
	unsigned long RowCount=16;//pevný počet řádků
	K_mGrid->Create(ColCount,RowCount);//samotné vytvoření matice-tabulky
  K_mGrid->Top=scLabel_header->Height + 10;


  K_mGrid->Columns[0].Width=120;
  for(int i=1;i<=ColCount;i++)
  {
    K_mGrid->Columns[i].Width=50;
  }
  K_mGrid->SetColumnAutoFit(-4);

  LoadValues(); //načtení hodnot do tabulky
  K_mGrid->Refresh();
  LoadStyles(); // načtení designu tabuly

		K_mGrid->MergeCells(0,0,0,1);
		K_mGrid->MergeCells(1,0,3,1);
		K_mGrid->MergeCells(4,0,11,0);
		K_mGrid->MergeCells(4,1,7,1);
		K_mGrid->MergeCells(8,1,11,1);
		K_mGrid->MergeCells(12,0,19,0);
		K_mGrid->MergeCells(12,1,15,1);
		K_mGrid->MergeCells(16,1,19,1);
  //pozice formu - střed obrazovky
  Left=Form1->ClientWidth/2-Form_katalog->Width/2;
  Top=Form1->ClientHeight/2-Form_katalog->Height/2;
  K_mGrid->Left=2;
	Form_katalog->Width=K_mGrid->Width + 4;
	Button_save->Top= K_mGrid->Height + scLabel_header->Height + 30;
	Button_storno->Top= K_mGrid->Height + scLabel_header->Height + 30;
	Form_katalog->Height = scLabel_header->Height +  K_mGrid->Height + Button_save->Height + 70;

	if(F->ls->Strings[193]!="")vypis(F->ls->Strings[193],false);else vypis("Kliknutím do seznamu rádiusů vyberete katalog dopravníků",false);
}
//---------------------------------------------------------------------------

void __fastcall TForm_katalog::FormPaint(TObject *Sender)
{
  K_mGrid->Show();//vykreslí tabulku
}
//---------------------------------------------------------------------------

void TForm_katalog::LoadValues ()
{
  // vytvoření hlavičky
	K_mGrid->Cells[0][0].Text="CALDAN";
	if(F->ls->Strings[196]!="")K_mGrid->Cells[1][0].Text=F->ls->Strings[196];else K_mGrid->Cells[1][0].Text="Rozteč palců [mm]";
	if(F->ls->Strings[197]!="")K_mGrid->Cells[4][0].Text=F->ls->Strings[197];else K_mGrid->Cells[4][0].Text="Horizontální oblouky";
	if(F->ls->Strings[293]!="")K_mGrid->Cells[4][1].Text=F->ls->Strings[293];else K_mGrid->Cells[4][1].Text="Oblouk [°]";
	if(F->ls->Strings[198]!="")K_mGrid->Cells[8][1].Text=F->ls->Strings[198];else K_mGrid->Cells[8][1].Text="Rádius [mm]";
	if(F->ls->Strings[199]!="")K_mGrid->Cells[12][0].Text=F->ls->Strings[199];else K_mGrid->Cells[12][0].Text="Vertikální oblouky";
	if(F->ls->Strings[293]!="")K_mGrid->Cells[12][1].Text=F->ls->Strings[293];else K_mGrid->Cells[12][1].Text="Oblouk [°]";
	if(F->ls->Strings[198]!="")K_mGrid->Cells[16][1].Text=F->ls->Strings[198];else K_mGrid->Cells[16][1].Text="Rádius [mm]";

  Cvektory::Ttyp_dopravniku *K=F->d.v.KATALOG->dalsi;//přeskočí hlavičku
	while(K!=NULL)
	{
    //funkční část
		K_mGrid->Cells[0][K->n+1].Text=K->name;
    Cvektory::TDoubleHodnota *H=K->roztec->dalsi;

    //pro rozteče
    while(H!=NULL)
	  {
			K_mGrid->Cells[H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //pro horizontální oblouky
    H=K->hOblouk->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[3+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //Hradius
    H=K->hRadius->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[7+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //V oblouk
    H=K->vOblouk->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[11+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //V radius
    H=K->vRadius->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[15+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }
    //ukazatelová část
    delete H;H=NULL;
    K=K->dalsi;
	}
  delete K;K=NULL;

  //nastaveni barev pro zobrazeni vybrane hodnoty - oranzova
	for(int i=1;i<K_mGrid->ColCount;i++)
  {
	 if(F->ms.MyToDouble(K_mGrid->Cells[i][katalog_id+1].Text) == radius)
	 {
		if(i>=8 && i<=11) K_mGrid->Cells[i][katalog_id+1].Font->Color=(TColor)RGB(226,122,21);
   }

  }
  //nastaveni barev pro zobrazeni vybrane hodnoty - cerna
	for(int i=2;i<K_mGrid->RowCount;i++)
  {
		if(i==katalog_id+1)
      {
       for(int j=0;j<K_mGrid->ColCount;j++)
       {
        if(F->ms.MyToDouble(K_mGrid->Cells[j][i].Text) != radius) K_mGrid->Cells[j][i].Font->Color=clBlack;
       }
      }
  }
}

void TForm_katalog::LoadStyles ()
{
 TColor clBACKGROUND_light,clBACKGROUND_dark;
 clBACKGROUND_light=Form1->m.clIntensive((TColor)RGB(200,200,200),35);
 clBACKGROUND_dark=clWhite;//Form1->m.clIntensive((TColor)RGB(240,240,240),35);

 for (int s = 0; s < K_mGrid->ColCount-1; s++)
 {  //černý font v hlavičcce
	 K_mGrid->Cells[s][0].Font->Color=clBlack;
	 K_mGrid->Cells[s][1].Font->Color=clBlack;
 }

	for (int r = 2; r <= K_mGrid->RowCount-1; r++)
 {
   K_mGrid->Cells[8][r].Type=K_mGrid->EDIT;
   K_mGrid->Cells[9][r].Type=K_mGrid->EDIT;
   K_mGrid->Cells[10][r].Type=K_mGrid->EDIT;
   K_mGrid->Cells[11][r].Type=K_mGrid->EDIT;
 }
}

void TForm_katalog::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{

 if(!zmena)
 {
   if(Col>=8 && Col<=11)
   {
    zmena=true;
      for(int i=1;i<K_mGrid->RowCount;i++)
      {
         if(i!=Row)
         {
            for(int j=0;j<K_mGrid->ColCount;j++)
             {
             K_mGrid->Cells[j][i].Font->Color=(TColor)RGB(43,87,154);  //odbarveni  - nevybrano
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
      for(int i=1;i<K_mGrid->RowCount;i++)
      {
         if(i==Row)
         {
            for(int j=0;j<K_mGrid->ColCount;j++)
             {
             K_mGrid->Cells[j][i].Font->Color=clBlack;  //nabarveni - vybrano
             }
         }
      }

   }
  zmena=false;
 }
  //ziskani hodnot radius a katalog ID
  if(Col>=8 && Col<=11)
   {

   radius=Form1->ms.MyToDouble(K_mGrid->Cells[Col][Row].Text);
   katalog_id=Row-1;
   }


 FormPaint(this);
 Button_save->SetFocus();
}

void __fastcall TForm_katalog::Button_stornoClick(TObject *Sender)
{
F->scStyledForm1->HideClientInActiveEffect();
Form_parametry_linky->scStyledForm2->HideClientInActiveEffect();
Button_storno->SetFocus();//workaround proti padání K_mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na K_mGridu, na komponentu nemazanou
K_mGrid->Delete();

}
//---------------------------------------------------------------------------
void __fastcall TForm_katalog::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	unsigned long Row=K_mGrid->GetIdxRow(X,Y);
	if(Row>=2 && Row<100)K_mGrid->HighlightRowOnMouse(X,Y,(TColor)RGB(240,240,240));
}
//---------------------------------------------------------------------------


void __fastcall TForm_katalog::Button_saveClick(TObject *Sender)
{
Form_parametry_linky->katalog_id=katalog_id;
Form_parametry_linky->radius=radius/1000.0;

F->scStyledForm1->HideClientInActiveEffect();
Form_parametry_linky->scStyledForm2->HideClientInActiveEffect();

Button_storno->SetFocus();
//Form_parametry_linky->Close();

//Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_katalog::FormClose(TObject *Sender, TCloseAction &Action)
{
Button_storno->SetFocus();//workaround proti padání K_mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na K_mGridu, na komponentu nemazanou
K_mGrid->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TForm_katalog::KonecClick(TObject *Sender)
{
F->scStyledForm1->HideClientInActiveEffect();
Form_parametry_linky->scStyledForm2->HideClientInActiveEffect();
Button_storno->SetFocus();//workaround proti padání K_mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na K_mGridu, na komponentu nemazanou
K_mGrid->Delete();
K_mGrid=NULL;
Close();
}
//---------------------------------------------------------------------------
void TForm_katalog::vypis(UnicodeString text,bool red,bool link)
{
    F->log(__func__); //logování
		Button_save->Enabled=true;
		if(F->ls->Strings[194]!="")Button_save->Caption = F->ls->Strings[194];else Button_save->Caption = "Uložit";
		if(F->ls->Strings[195]!="")Button_storno->Caption = F->ls->Strings[195];else Button_storno->Caption = "Storno";
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
				rHTMLLabel_InfoText->Top = K_mGrid->Height + scLabel_header->Height + 14;
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
