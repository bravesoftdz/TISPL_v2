//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "katalog_dopravniku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
//#pragma link "rHTMLLabel"
//#pragma link "rHintWindow"
#pragma link "scHint"
#pragma link "scHtmlControls"
#pragma resource "*.dfm"
TForm_katalog *Form_katalog;
//---------------------------------------------------------------------------
__fastcall TForm_katalog::TForm_katalog(TComponent* Owner)
  : TForm(Owner)
{

  zmena=false;
  check.X=0;
  check.Y=0;
 	clNORMAL	 = (TColor)RGB(200,200,200); //(TColor)RGB(128,128,128);
  clOTHER_AFTER_CHOOSE			 = (TColor)RGB(128,128,128); //(TColor)RGB(200,200,200);
  clSELECTED_BLUE = (TColor)RGB(43,87,154); //modrá
  clBACKGROUND = (TColor)RGB(240,240,240);
  input_state=NO;
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
	unsigned long ColCount=19;//pevný počet slopců
	unsigned long RowCount=F->d.v.KATALOG->predchozi->n + 2;//dynamický počet řádků + 2 hlavička (merge)
	K_mGrid->Create(ColCount,RowCount);//samotné vytvoření matice-tabulky
  K_mGrid->Top=scLabel_header->Height + 10;

  K_mGrid->Columns[0].Width=25;
  K_mGrid->Columns[1].Width=90;
	for(unsigned int i=2;i<=ColCount;i++)
  {
    K_mGrid->Columns[i].Width=50;
  }
  K_mGrid->SetColumnAutoFit(-4);

  LoadValues(); //načtení hodnot do tabulky
  K_mGrid->Refresh();


		K_mGrid->MergeCells(1,0,1,1);
		K_mGrid->MergeCells(2,0,4,1);
		K_mGrid->MergeCells(5,0,12,0);
		K_mGrid->MergeCells(5,1,8,1);
		K_mGrid->MergeCells(9,1,12,1);
		K_mGrid->MergeCells(13,0,18,0);
		K_mGrid->MergeCells(13,1,16,1);
		K_mGrid->MergeCells(17,1,18,1);
    K_mGrid->MergeCells(0,0,0,1);
    K_mGrid->MergeCells(0,0,1,0);
    K_mGrid->MergeCells(0,1,1,1);

     LoadStyles(); // načtení designu tabuly
     getCheckSettings();

  //pozice formu - střed obrazovky
  Left=Form1->ClientWidth/2-Form_katalog->Width/2;
  Top=Form1->ClientHeight/2-Form_katalog->Height/2;
  K_mGrid->Left=2;
	Form_katalog->Width=K_mGrid->Width + 4;
	Button_save->Top= K_mGrid->Height + scLabel_header->Height + 30;
	Button_storno->Top= K_mGrid->Height + scLabel_header->Height + 30;
	Form_katalog->Height = scLabel_header->Height +  K_mGrid->Height + Button_save->Height + 70;

	vypis(F->ls->Strings[193],false);//"Kliknutím do seznamu rádiusů vyberete katalog dopravníků"

  	//defaultní design a pozicování tlačítek OK a STORNO
	F->m.designButton(Button_save,Form_katalog,1,2);
	F->m.designButton(Button_storno,Form_katalog,2,2);
  input_state=NOTHING;
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
 // K_mGrid->Cells[0][0].Text="Výběr";
	K_mGrid->Cells[1][0].Text="CALDAN";
	K_mGrid->Cells[2][0].Text=F->ls->Strings[196];//"Rozteč palců [mm]";
	K_mGrid->Cells[5][0].Text=F->ls->Strings[197];//"Horizontální oblouky";
	K_mGrid->Cells[5][1].Text=F->ls->Strings[293];//"Oblouk [°]";
	K_mGrid->Cells[9][1].Text=F->ls->Strings[198];//"Rádius [mm]";
	K_mGrid->Cells[13][0].Text=F->ls->Strings[199];//"Vertikální oblouky";
	K_mGrid->Cells[13][1].Text=F->ls->Strings[293];//"Oblouk [°]";
	K_mGrid->Cells[17][1].Text=F->ls->Strings[198];//"Rádius [mm]";

  Cvektory::Ttyp_dopravniku *K=F->d.v.KATALOG->dalsi;//přeskočí hlavičku
	while(K!=NULL)
	{
    //funkční část
		K_mGrid->Cells[1][K->n+1].Text=K->name;
    Cvektory::TDoubleHodnota *H=K->roztec->dalsi;

    //pro rozteče
    while(H!=NULL)
	  {
			K_mGrid->Cells[1+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //pro horizontální oblouky
    H=K->hOblouk->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[4+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //Hradius
    H=K->hRadius->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[8+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //V oblouk
    H=K->vOblouk->dalsi;
    while(H!=NULL)
	  {
			K_mGrid->Cells[12+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }

    //V radius
    H=K->vRadius->dalsi;
    while(H!=NULL)
	  {
		 	K_mGrid->Cells[16+H->n][K->n+1].Text=H->hodnota;
      H=H->dalsi;
    }
    //ukazatelová část
    delete H;H=NULL;
    K=K->dalsi;
	}
  delete K;K=NULL;

  //nastaveni barev pro zobrazeni vybrane hodnoty - oranzova
	for(unsigned int i=1;i<K_mGrid->ColCount;i++)
  {
	 if(F->ms.MyToDouble(K_mGrid->Cells[i][katalog_id+1].Text) == radius)
	 {
		if(i>=9 && i<=12)
    {
     K_mGrid->Cells[i][katalog_id+1].Font->Color=(TColor)RGB(226,122,21);
     K_mGrid->Cells[i][katalog_id+1].Type=K_mGrid->CHECK;
     K_mGrid->Update();
     TscGPCheckBox *H=K_mGrid->getCheck(i,katalog_id+1);
     H->BiDiMode=bdRightToLeft;
     H->Layout=blGlyphRight;
     H->Checked=true;
     H=NULL;delete H;
     check.X=i;
    }
   }

  }
  //nastaveni barev pro zobrazeni vybrane hodnoty - cerna
	for(unsigned int i=2;i<K_mGrid->RowCount;i++)
  {
   K_mGrid->Cells[0][i].Type=K_mGrid->CHECK;
   K_mGrid->Cells[0][i].RightBorder->Color=clWhite;

		if(i==(unsigned)katalog_id+1)
      {
			 for(unsigned int j=0;j<K_mGrid->ColCount;j++)
       {
        if(F->ms.MyToDouble(K_mGrid->Cells[j][i].Text) != radius)
        {
        K_mGrid->Cells[j][i].Font->Color=clBlack;
        check.Y=i;
        //ShowMessage(j);ShowMessage(i);
        }
       }
      }
  }
}

void TForm_katalog::LoadStyles ()
{

 //clBACKGROUND_light=Form1->m.clIntensive((TColor)RGB(200,200,200),35);
 //clBACKGROUND_dark=clWhite;//Form1->m.clIntensive((TColor)RGB(240,240,240),35);

 for (unsigned int s = 0; s <= K_mGrid->ColCount-1; s++)
 {  //černý font v hlavičcce
	 K_mGrid->Cells[s][0].Font->Color=clBlack;
	 K_mGrid->Cells[s][1].Font->Color=clBlack;
 }

	for (unsigned int r = 2; r <= K_mGrid->RowCount-1; r++)
 {
 // K_mGrid->Cells[0][r].Font->Color=clBlack;
   K_mGrid->Cells[1][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[2][r].Font->Color=clOTHER_AFTER_CHOOSE; //DefaultCell.isZero->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
   K_mGrid->Cells[3][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[4][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[5][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[6][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[7][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[8][r].Font->Color=clOTHER_AFTER_CHOOSE;


   K_mGrid->Cells[9][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[10][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[11][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[12][r].Font->Color=clOTHER_AFTER_CHOOSE;

   K_mGrid->Cells[9][r].Align=K_mGrid->LEFT;
   K_mGrid->Cells[10][r].Align=K_mGrid->LEFT;
   K_mGrid->Cells[11][r].Align=K_mGrid->LEFT;
   K_mGrid->Cells[12][r].Align=K_mGrid->LEFT;

   K_mGrid->Cells[9][r].LeftMargin=3;
   K_mGrid->Cells[10][r].LeftMargin=3;
   K_mGrid->Cells[11][r].LeftMargin=3;
   K_mGrid->Cells[12][r].LeftMargin=3;

//    K_mGrid->Cells[9][r].Type=K_mGrid->CHECK;
//    K_mGrid->Cells[10][r].Type=K_mGrid->CHECK;
//    K_mGrid->Cells[11][r].Type=K_mGrid->CHECK;
//    K_mGrid->Cells[12][r].Type=K_mGrid->CHECK;
//
//    K_mGrid->Update();
//    K_mGrid->getCheck(9,r)->BiDiMode=bdRightToLeft;
//    K_mGrid->getCheck(9,r)->Layout=blGlyphRight;
//
//    K_mGrid->getCheck(10,r)->BiDiMode=bdRightToLeft;
//    K_mGrid->getCheck(10,r)->Layout=blGlyphRight;
//
//    K_mGrid->getCheck(11,r)->BiDiMode=bdRightToLeft;
//    K_mGrid->getCheck(11,r)->Layout=blGlyphRight;
//
//    K_mGrid->getCheck(12,r)->BiDiMode=bdRightToLeft;
//    K_mGrid->getCheck(12,r)->Layout=blGlyphRight;


   K_mGrid->Cells[13][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[14][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[15][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[16][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[17][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[18][r].Font->Color=clOTHER_AFTER_CHOOSE;
 //  K_mGrid->Cells[19][r].Font->Color=clOTHER_AFTER_CHOOSE;
 //  K_mGrid->Cells[20][r].Font->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[13][r].isZero->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[17][r].isZero->Color=clOTHER_AFTER_CHOOSE;
   K_mGrid->Cells[18][r].isZero->Color=clOTHER_AFTER_CHOOSE;



 }
}

void TForm_katalog::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
 if (input_state==NOTHING)
 {

  input_state=JOB;
  if(!zmena)
  {
    zmena=true;
     if(Col==0)
     {
        for(unsigned int i=2;i<K_mGrid->RowCount;i++)
        {

           if(i!=Row)
           {
              K_mGrid->getCheck(0,i)->Checked=false;
              for(unsigned int j=1;j<K_mGrid->ColCount;j++)
               {
               K_mGrid->Cells[j][i].Font->Color=clOTHER_AFTER_CHOOSE;//(TColor)RGB(43,87,154);  //odbarveni  - nevybrano
               if(j>=9 && j<=12) { /*ShowMessage(j);*/ K_mGrid->Cells[j][i].Type=K_mGrid->readEDIT;    }
              // ShowMessage(j);
               }
           }

        }

     }
     if(Col==0)
     {
       for(unsigned int i=2;i<K_mGrid->RowCount;i++)
        {
          if(i==Row)
             {
             //ShowMessage("Row");
              check.Y=i;
              getCheckSettings();
             }

        }
      }
    zmena=false;
   }

  //ziskani hodnot radius a katalog ID
  if(Col>=9 && Col<=12)
   {

   radius=Form1->ms.MyToDouble(K_mGrid->Cells[Col][Row].Text);
   katalog_id=Row-1;
   }


   FormPaint(this);
   Button_save->SetFocus();
   input_state=NOTHING;
  }
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
 //	if(Row>=2 && Row<100)K_mGrid->HighlightRowOnMouse(X,Y,(TColor)RGB(240,240,240));
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
		Button_save->Caption = F->ls->Strings[194];//"Uložit";
		Button_storno->Caption = F->ls->Strings[195];//"Storno";
 //if(text=="m].</b>")text="";//provizorní WA, při změně Rz a byla-li v pořádku to vrací toto  - již není třeba, ale zatím nechávám
		if (text != "") // zobrazí a vypíše
		{
				scHint1->ToString()=text;//natežení do hintu zajišťuje zobrazení celého textu, nepoužívá se klasický hint
				//prodllužení formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl předtím popisek skrytý + kontrola pozice formu

				if(link)scHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnutí podtrženého písma
				else scHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
						Button_save->Enabled=false;  //R - dočasné povolení ukládání při validaci
						scHTMLLabel_InfoText->Font->Color = clRed;
            scHTMLLabel_InfoText->Color=clWhite;
				}
				else
				{
						scHTMLLabel_InfoText->Font->Color = (TColor)RGB(0,128,255);
				}
				scHTMLLabel_InfoText->Top = K_mGrid->Height + scLabel_header->Height + 14;
        scHTMLLabel_InfoText->Left = 4;
				scHTMLLabel_InfoText->Caption = text;
				scHTMLLabel_InfoText->Visible = true;
        scHTMLLabel_InfoText->Color=clWhite;
		}
		else // skryje
		{
				//zkrácení formu if(rHTMLLabel_InfoText->Visible)Height-=(40+19);
				scHTMLLabel_InfoText->Visible = false;
		}
}

void TForm_katalog::getCheckSettings ()
{

   TscGPCheckBox *CH=K_mGrid->getCheck(0,check.Y);  //výchozí stav zašrtávátka
   CH->Checked=true;
   CH=NULL;delete CH;

    K_mGrid->Columns[9].Width=60;
    K_mGrid->Columns[10].Width=60;
    K_mGrid->Columns[11].Width=60;
    K_mGrid->Columns[12].Width=60;

  for(unsigned int i=1;i<K_mGrid->ColCount;i++)
  {
   K_mGrid->Cells[i][check.Y].Font->Color=clSELECTED_BLUE;
   K_mGrid->Cells[i][check.Y].Background->Color=clBACKGROUND;
  // if(check.X==i)
   K_mGrid->Cells[9][check.Y].Type=K_mGrid->CHECK;
   K_mGrid->Cells[10][check.Y].Type=K_mGrid->CHECK;
   K_mGrid->Cells[11][check.Y].Type=K_mGrid->CHECK;
   K_mGrid->Cells[12][check.Y].Type=K_mGrid->CHECK;
  }

    for(unsigned int j=9;j<=12;j++)
  {
    K_mGrid->Cells[j][check.Y].Align=K_mGrid->LEFT;
    K_mGrid->Update();
    TscGPCheckBox *H=K_mGrid->getCheck(j,check.Y);
    if(H->Caption=="") { /*H->Visible=false;*/  K_mGrid->Cells[j][check.Y].Type=K_mGrid->readEDIT;  }
    else
    {
    H->BiDiMode=bdRightToLeft;
    H->Layout=blGlyphRight;
    }
    H=NULL;delete H;

  }


}




