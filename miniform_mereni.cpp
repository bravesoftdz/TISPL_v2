//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "miniform_mereni.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_mereni *Form_mereni;
//---------------------------------------------------------------------------
//konstruktor
__fastcall TForm_mereni::TForm_mereni(TComponent* Owner)
	: TForm(Owner)
{
	mGrid=NULL;
	pocet_radku=0;
	data_tabulky="";
	data_tabulky_zaloha="";
	JID=0;
	closing=false;
}
//---------------------------------------------------------------------------
//zobrazení formu
void __fastcall TForm_mereni::FormShow(TObject *Sender)
{
  pocet_radku=0;
	data_tabulky="";
	data_tabulky_zaloha="";
  JID=0;
	vytvor_napln_mGrid();
	Width=mGrid->Width+2*scGPPanel_header->Height;
	Height=mGrid->Height+3*scGPPanel_header->Height+scGPButton_OK->Height;
	Left=FX=F->Width-Width;
	Top=FY=F->scGPPanel_mainmenu->Height;
	scGPButton_OK->Top=mGrid->Top+mGrid->Height+0.5*scGPPanel_header->Height;
	scGPButton_OK->Left=Width/2.0-scGPButton_OK->Width/2.0;
	closing=false;
	scGPGlyphButton_pripnout->Visible=false;
}
//---------------------------------------------------------------------------
//vytvoøí tabulku s hodnotami mìøení
void TForm_mereni::vytvor_napln_mGrid()
{
  //vytvoøení a nastavení mGridu
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=10;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->ID=0;
	mGrid->Left=scGPPanel_header->Height;mGrid->Top=2*scGPPanel_header->Height;//hodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->Border.Width=2;
	mGrid->DefaultCell.Font->Name=F->aFont->Name;
	mGrid->DefaultCell.Font->Size=F->aFont->Size;
	mGrid->DefaultCell.Font->Color=(TColor)RGB(128,128,128);
	mGrid->DefaultCell.isZero=mGrid->DefaultCell.Font;//zamezení podbarevní buòky obsahující 0 jinou barvou
	mGrid->DefaultCell.isLink->Name=F->aFont->Name;
	mGrid->DefaultCell.isLink->Size=F->aFont->Size;
	mGrid->DefaultCell.isActiveLink->Name=F->aFont->Name;
	mGrid->DefaultCell.isActiveLink->Size=F->aFont->Size;
	mGrid->Create(4, 2);

	//nastavení popiskù
	mGrid->Cells[0][0].Text="Mìøení v úseku";
	mGrid->Cells[0][1].Text="Prvky";
	mGrid->Cells[1][1].Text="Vzdálenost <a>[mm]</a>";
	mGrid->Cells[2][1].Text="IN  <a>[s]</a>";
	mGrid->Cells[3][1].Text="OUT  <a>[s]</a>";

	//barvy hlavièky
	mGrid->Cells[0][0].Font->Color=clBlack;
	mGrid->Cells[0][1].Font->Color=clBlack;
	mGrid->Cells[1][1].Font->Color=clBlack;
	mGrid->Cells[2][1].Font->Color=clBlack;
	mGrid->Cells[3][1].Font->Color=clBlack;

	//šíøky a sluèováníbuòek
	mGrid->SetColumnAutoFit(-4);
	mGrid->Columns[0].Width=150;
	mGrid->Columns[1].Width=150;
	mGrid->Columns[2].Width=150;
	mGrid->Columns[3].Width=150;
	mGrid->MergeCells(0,0,3,0);

	//nasbírání dat a poètu øádkù, které je pro nì tøeba vytvoøit
	napln_data_tabulky();
  data_tabulky_zaloha=data_tabulky;//uložení do zálohy, pøi vytvoøení tabulky dojde k odstranìní dat, slouží pro pøístup k datùm napø. pøi zmìnì jednotek

	//pøidání øádkù do tabulky
	for(unsigned int i=1;i<=pocet_radku;i++)
	{
		mGrid->AddRow(false,false);
	}
	mGrid->Update();//nutné po pøidání øádku!!!

  //naplnìní øádkù daty
	int oddelovac=0;String hodnota="";
	for(unsigned int i=2;i<mGrid->RowCount;i++)
	{
		for(unsigned int j=0;j<mGrid->ColCount;j++)
		{
			//extrakce hodnoty z øetezce
			oddelovac=data_tabulky.Pos(";");
			hodnota=data_tabulky.SubString(1,oddelovac-1);
			data_tabulky.Delete(1,oddelovac);//smazání hodnoty z øetìzce
			mGrid->Cells[j][i].Text=hodnota;
		}
	}

	//ohranièení, musí být po pøidávání øádkù
	if(pocet_radku>0)
	{
		mGrid->Cells[0][2].TopBorder->Width=2;
		mGrid->Cells[1][2].TopBorder->Width=2;
		mGrid->Cells[2][2].TopBorder->Width=2;
		mGrid->Cells[3][2].TopBorder->Width=2;
	}
}
//---------------------------------------------------------------------------
//vytvoøí seznam dat o øádcích tabulky mìøení
void TForm_mereni::napln_data_tabulky()
{
	pocet_radku=0;
	data_tabulky="";
	String prejezd=F->ls->Strings[486];//"pøejezd"

	//lineární mìøení
	if(F->d.v.MAG_LASO->sparovany==NULL || F->d.v.MAG_LASO->predchozi->sparovany==NULL)
	{
		data_tabulky="-;"+String(F->m.round2double(F->m.delka(F->d.v.MAG_LASO->Element->geo.X1,F->d.v.MAG_LASO->Element->geo.Y1,F->d.v.MAG_LASO->predchozi->Element->geo.X4,F->d.v.MAG_LASO->predchozi->Element->geo.Y4)*1000,2))+";-;-;";
		pocet_radku++;
	}
	//mìøení magnetickým lasem
	else
	{
		double X=0,Y=0,uhel=0,delka=0,delka_prejezdu=0;
		TPointD in,out;
		in.x=0;in.y=0;out.x=0;out.y=0;
    bool force_prvni_radek=false;//nahrazuje podmínku pro nucenoé vypsání jako první øádek
		Cvektory::TElement *E=NULL;

		Cvektory::TCesta *C=F->d.v.MAG_LASO->dalsi;
		while(C!=NULL && (C->dalsi!=NULL || (C->dalsi==NULL && C->sparovany!=C->predchozi->Element)))//pøeskakovat posledí element, pokud se jedná o pøichycený (tzn. v pøedchozím segmentu se již nacházel)
		{
      ////naètení C->Elementu do E, pomocný ukazatel
			E=C->Element;

			////vypoèet delky
			//nastavení parametrù pro výpoèet délky
			X=E->geo.X1;Y=E->geo.Y1;
			if(C->n==1){X=F->d.v.MAG_LASO->Element->geo.X1;Y=F->d.v.MAG_LASO->Element->geo.Y1;}
			//element v seznamu (ani první, ani poslení) pouze naètení délky z geo
			if(E->n!=MaxInt && C->n!=1)delka=E->geo.delka;
			//první, nebo poslední - linie
			else if(E->geo.typ==0)
			{
				delka=F->m.delka(X,Y,E->geo.X4,E->geo.Y4);
				delka=F->m.castPrepony(delka,E->geo.delkaPud,E->geo.HeightDepp);
			}
			//první, nebo poslední - oblouk
			else
  		{
				delka=F->m.delka(X,Y,E->geo.X4,E->geo.Y4);
				uhel=F->m.T2Aarc(E->geo.radius,delka);
				delka=F->m.R2Larc(E->geo.radius,uhel);
			}

			////výpoèet èasu
			if(C->dalsi==NULL)E=C->sparovany;//pro výpoèet èasu jsou nutné informace z pohonu elementu, poslední segment cesty má Element->pohon=NULL, nutné vzít si pohon z sparovany!
			if(E!=NULL && E->pohon!=NULL)
			{
				//èas pøejezdu
				in.x+=delka/E->pohon->aRD;in.y+=delka/E->pohon->aRD;
				out.x+=delka/E->pohon->aRD;out.y+=delka/E->pohon->aRD;
				//pøipoèítávat casy prvního
				//pokud mám naèítat dat z prvního elementu, naèítám pouze WT a latenci
				if(C->n==1 && F->d.v.MAG_LASO->Element->geo.X1==E->geo.X4 && F->d.v.MAG_LASO->Element->geo.Y1==E->geo.Y4)
				{
					if(F->d.v.vrat_druh_elementu(E)==0)
					{
						//WT - pouze do druhé složky IN
						in.y+=E->WT;
						//odeètení záporného RT = pøiètení RT
						if(E->data.RT<0){in.x-=E->data.RT;in.y-=E->data.RT;out.x-=E->data.RT;out.y-=E->data.RT;}
						//latence, pokud je nastaveno
						//if(F->scGPCheckBox_meridlo_casy->Checked && E->eID==0 && E->dalsi!=NULL && E->dalsi->pohon!=NULL){in.x+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);in.y+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);out.x+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);out.y+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);}
					}
				}
				//zaokrouhlení èasu na takt
				if((!F->scGPCheckBox_meridlo_casy->Checked && (E!=F->d.v.MAG_LASO->sparovany || (E==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))) || (F->scGPCheckBox_meridlo_casy->Checked && C->n>1) || (F->scGPCheckBox_meridlo_casy->Checked && (E!=F->d.v.MAG_LASO->sparovany || (E==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))))
				{
					//jedná se o element uvnitø seznamu (následuje další, již v seznamu, není na nìm kurzor)
					if(/*(!prichyceno || (prichyceno && E!=F->pom_element))C->dalsi!=NULL && */F->d.v.vrat_druh_elementu(E)==0)
					{
						out.y=ceil(out.x/F->d.v.PP.TT)*F->d.v.PP.TT;out.x=out.y-E->WT;
						if(E->data.RT<0){out.x-=E->data.RT;out.y-=E->data.RT;}//odeètení záporného RT = pøiètení RT
					}
				}
			}
			E=C->Element;//navrácení Elementu, v nasledujících podmínkách používán

			////uložení dat pro tabulku
      //zarážky, vnímat pouze jako pøejezd, pokud se nejedná o poslední segment (ukonèení, pøejezd - pøejezd -> pøejezd), nelze nezakonèit
			if(E->eID==MaxInt && C->dalsi!=NULL)delka_prejezdu+=delka;//pøièíst délku k délce pøejezdu, èas není tøeba nìjak sèítat (sèítán pøi krocích výpoètu na rozdíl od délky)
			//ostatní elementy (robotí, stoky, ...)
			else
			{
		  	//uložení názvù
		  	if(C->dalsi!=NULL)
				{
		  		if(E->eID!=MaxInt)data_tabulky+=E->name+";";else data_tabulky+=prejezd+";";//musí být øešeno pøes promìnnou, nelze zadávat pøímo takto: F->ls->Strings[486]
		  	}
		  	else
		  	{
					if(C->sparovany->eID!=MaxInt && C->Element->geo.X2==C->Element->geo.X3)data_tabulky+=C->sparovany->name+";";else data_tabulky+=prejezd+";";
				}
				//uložení délky
        delka+=delka_prejezdu;//pøiètení pøedchozího pøejezdu, pokud existoval (pokud ne delka_prejezdu==0)
				data_tabulky+=String(F->m.round2double(delka,5)*1000)+";";
		  	//uložení èasu
				//první øádek
        if((C->dalsi==NULL || (C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)) && pocet_radku==0 && E->geo.X2!=E->geo.X3)force_prvni_radek=true;//nucenì vypsané jako první øádek (pro pøejezdy, tabulka o jednom øádku)
		  	if(force_prvni_radek || (C->n==1 && (F->d.v.vrat_druh_elementu(E)!=0 || (F->d.v.vrat_druh_elementu(E)==0 && F->d.v.MAG_LASO->Element->geo.X2==F->d.v.MAG_LASO->Element->geo.X3)) &&  !(C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)))//pouze pro KK elementy, brát v potaz i pøeskakovaný segment
				{
		  		data_tabulky+="-;";
		  		if(in.x!=in.y)data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
		  		else data_tabulky+=String(F->m.round2double(in.x,2))+";";
		  	}
		  	//poslední øádek
				else if((C->dalsi==NULL || (C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)) && (F->d.v.vrat_druh_elementu(E)!=0 || (F->d.v.vrat_druh_elementu(E)==0 && C->dalsi==NULL && E->geo.X2!=E->geo.X3)))//pokud není pøichyceno na S&G element
				{
		  		if(in.x!=in.y)data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
          else data_tabulky+=String(F->m.round2double(in.x,2))+";";
  	  		data_tabulky+="-;";
		  	}
		  	//ostatní
				else
				{
		  		if(in.x!=in.y)data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
		  		else data_tabulky+=String(F->m.round2double(in.x,2))+";";
		  		if(out.x!=out.y)data_tabulky+=String(F->m.round2double(out.x,2))+" - "+String(F->m.round2double(out.y,2))+";";
		  		else data_tabulky+=String(F->m.round2double(out.x,2))+";";
		  	}
        //navíšení poètu øádku tabulky
				pocet_radku++;
        //došlo k ukonèení pøejezdu - nulovat
				delka_prejezdu=0;
			}
			////srovnání in a out èasù pro další výpoèty, pøi pøejetí pøes element, se zaokrouhluje na takt pouze out v dalším výpoètu je tøeba in i out zaokrouhlené na takt
			if(in.x<out.x)in.x=out.x;
			if(in.y<out.y)in.y=out.y;

  		////posun na další segment
			C=C->dalsi;
		}
    //ukazatelové záležitosti
		delete C;C=NULL;
    E=NULL;delete E;
	}
}
//---------------------------------------------------------------------------
//OnClick mGridu
void TForm_mereni::OnClick(long Tag,long ID,long Col,long Row)
{
	F->Memo(String(__func__)+" ",true,true);
	//F->Sv(JID);
}
//---------------------------------------------------------------------------
//MouseMove mGridu
void TForm_mereni::OnMouseMove(long Tag,long ID,int X,int Y,long Col,long Row)
{
	F->Memo(String(__func__)+" ",true,true);
	JID=0;
	int IdxCol=mGrid->GetIdxColumn(X,Y);
  int IdxRow=mGrid->GetIdxRow(X,Y);
	if(IdxCol>0 && IdxCol<4 && IdxRow==1 && mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxCol;//na daném øádku a daných myších souøadnicích se nachází odkaz
}
//---------------------------------------------------------------------------
//klik pro zavøení formu
void __fastcall TForm_mereni::SkrytClick(TObject *Sender)
{
	closing=true;
	mGrid->Delete();
	mGrid=NULL;
	Close();
}
//---------------------------------------------------------------------------
//indikace pøesunu formu
void __fastcall TForm_mereni::scLabel_headerMouseUp(TObject *Sender, TMouseButton Button,TShiftState Shift, int X, int Y)
{
	if(!scGPGlyphButton_pripnout->Visible && (Left!=FX || Top!=FY))
		scGPGlyphButton_pripnout->Visible=true;
}
//---------------------------------------------------------------------------
//klik na pøipnutí formu k horní lištì doprava
void __fastcall TForm_mereni::scGPGlyphButton_pripnoutClick(TObject *Sender)
{
	Left=FX;
	Top=FY;
	scGPGlyphButton_pripnout->Visible=false;
}
//---------------------------------------------------------------------------
//vykreslení tabulky
void __fastcall TForm_mereni::FormPaint(TObject *Sender)
{
	if(mGrid!=NULL)mGrid->Show(Canvas);
}
//---------------------------------------------------------------------------
