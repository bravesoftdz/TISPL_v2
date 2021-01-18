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
	j_delka=MM;
	j_cas=SEC;
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
	String delka="<a>[mm]</a>";if(j_delka!=MM)delka="<a>[m]</a>";
  String cas="<a>[s]</a>";if(j_cas!=SEC)cas="<a>[min]</a>";
	mGrid->Cells[0][0].Text="Mìøení v úseku";
	mGrid->Cells[0][1].Text="Prvky";
	mGrid->Cells[1][1].Text="Vzdálenost "+delka;
	mGrid->Cells[2][1].Text="IN "+cas;
	mGrid->Cells[3][1].Text="OUT "+cas;

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
		Cvektory::TPohon *pom_pohon=NULL;;

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
				//pøipoèítávat casy prvního, pokud mám naèítat dat z prvního elementu, naèítám pouze WT a latenci
				if(C->n==1 && F->d.v.MAG_LASO->Element->geo.X1==E->geo.X4 && F->d.v.MAG_LASO->Element->geo.Y1==E->geo.Y4)
				{
					if(F->d.v.vrat_druh_elementu(E)==0)
					{
						//WT - pouze do druhé složky IN
						in.y+=E->WT;out.y+=E->WT;
						//latence
						pom_pohon=NULL;if(E->dalsi!=NULL)pom_pohon=E->dalsi->pohon;if(E->dalsi==NULL)pom_pohon=F->d.v.ELEMENTY->dalsi->pohon;//naètení pohonu do pomocného ukazatele
						if(pom_pohon!=NULL){in.y+=F->m.latence_mezi_stopkami(pom_pohon->aRD);out.y+=F->m.latence_mezi_stopkami(pom_pohon->aRD);}
						//PT robotù / operátorù
						if(E->eID!=0){in.x+=E->PTotoc+E->data.PT1+E->data.PT2;in.y+=E->PTotoc+E->data.PT1+E->data.PT2;out.x+=E->PTotoc+E->data.PT1+E->data.PT2;out.y+=E->PTotoc+E->data.PT1+E->data.PT2;}
						//RT
						if(E->data.RT<0){in.x-=E->data.RT;in.y-=E->data.RT;out.x-=E->data.RT;out.y-=E->data.RT;}//odeètení záporného RT = pøiètení RT
					}
				}
				//pøipoèítávat èasy ostatních elementù
				else//if((!F->scGPCheckBox_meridlo_casy->Checked && (E!=F->d.v.MAG_LASO->sparovany || (E==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))) || (F->scGPCheckBox_meridlo_casy->Checked && C->n>1) || (F->scGPCheckBox_meridlo_casy->Checked && (E!=F->d.v.MAG_LASO->sparovany || (E==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))))
				{
					//jedná se o element uvnitø seznamu (následuje další, již v seznamu, není na nìm kurzor)
					if(F->d.v.vrat_druh_elementu(E)==0)
					{
						//WT
						out.x=in.x;out.y=in.y+E->WT;
						//latence
            pom_pohon=NULL;if(E->dalsi!=NULL)pom_pohon=E->dalsi->pohon;if(E->dalsi==NULL)pom_pohon=F->d.v.ELEMENTY->dalsi->pohon;//naètení pohonu do pomocného ukazatele
						if(pom_pohon!=NULL)out.y+=F->m.latence_mezi_stopkami(pom_pohon->aRD);
						//PT robotù / operátorù
						if(E->eID!=0){out.x+=E->PTotoc+E->data.PT1+E->data.PT2;out.y+=E->PTotoc+E->data.PT1+E->data.PT2;}
            //RT
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
				if((C->dalsi==NULL || (C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)) && pocet_radku==0 && E->geo.X2!=E->geo.X3 && F->d.v.vrat_druh_elementu(E)!=0)force_prvni_radek=true;//nucenì vypsané jako první øádek (pro pøejezdy, tabulka o jednom øádku)
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
		C=NULL;delete C;//nejdøíve NULL potom delete, nìkdy dojde k ukonèení prùchodu na pøedposledním prvku
		E=NULL;delete E;
    pom_pohon=NULL;delete pom_pohon;
	}
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
	if(!scGPGlyphButton_pripnout->Visible && (Left!=FX || Top!=FY))scGPGlyphButton_pripnout->Visible=true;
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
//hledání linkù v tabulce
void __fastcall TForm_mereni::FormMouseMove(TObject *Sender, TShiftState Shift, int X,int Y)
{
	int puvJID=JID;JID=0;
	int IdxCol=mGrid->GetIdxColumn(X,Y);
  int IdxRow=mGrid->GetIdxRow(X,Y);
	if(IdxCol>0 && IdxCol<4 && IdxRow==1 && mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxCol;
	if(JID!=puvJID){mGrid->MouseMove(X,Y);mGrid->Refresh();}
}
//---------------------------------------------------------------------------
//OnClick slouží k detekci zmìny jednotek tabulky
void __fastcall TForm_mereni::FormClick(TObject *Sender)
{
	//zmìna jednotek délky
	if(JID==101)
	{
		if(j_delka==MM)j_delka=M;
    else j_delka=MM;
	}
	//zmìna jednotek èasu
	if(JID==102 || JID==103)
	{
		if(j_cas==SEC)j_cas=MIN;
		else j_cas=SEC;
	}
	//zmìna v tabulce
	if(JID!=0)zmen_jednotky_tabulce();
}
//---------------------------------------------------------------------------
//zmìna jednotek v tabulce
void TForm_mereni::zmen_jednotky_tabulce()
{
  //hlavicka tabulky
  String delka="<a>[mm]</a>";if(j_delka!=MM)delka="<a>[m]</a>";
  String cas="<a>[s]</a>";if(j_cas!=SEC)cas="<a>[min]</a>";
  mGrid->Cells[1][1].Text="Vzdálenost "+delka;
  mGrid->Cells[2][1].Text="IN "+cas;
	mGrid->Cells[3][1].Text="OUT "+cas;
	//hodnoty v tabulce
	data_tabulky=data_tabulky_zaloha;
	int oddelovac=0;String hodnota="";
	double cislo1=0.0,cislo2=0.0;
	for(unsigned int i=2;i<mGrid->RowCount;i++)
	{
		for(unsigned int j=0;j<mGrid->ColCount;j++)
		{
      //extrakce hodnoty z øetezce
	  	oddelovac=data_tabulky.Pos(";");
	  	hodnota=data_tabulky.SubString(1,oddelovac-1);
			data_tabulky.Delete(1,oddelovac);//smazání hodnoty z øetìzce
			//hodnoty
			if(j>0 && hodnota!="-")
			{
				//zmìna na èíslo
				cislo1=F->ms.MyToDouble(hodnota);
        //rozmezí
				if(cislo1==0 && hodnota!="0")
				{
					//extrakce dvou èísel z rozmezí
					int oddelovac=hodnota.Pos(" ");
					cislo1=F->ms.MyToDouble(hodnota.SubString(1,oddelovac-1));
					hodnota.Delete(1,oddelovac);//smazání hodnoty z øetìzce
					oddelovac=hodnota.Pos(" ");
					cislo2=F->ms.MyToDouble(hodnota.SubString(oddelovac+1,hodnota.Length()));
          //pøepoèet podle akt. jednotek
			  	if(j==1)//delka
						{if(j_delka!=MM){cislo1==cislo1/1000.0;cislo2=cislo2/1000.0;}}
			  	else//cas
						{if(j_cas!=SEC){cislo1=cislo1/60.0;cislo2=cislo2/60.0;}}
					hodnota=String(F->m.round2double(cislo1,2))+" - "+String(F->m.round2double(cislo2,2));
				}
				//jedno èíslo
				else
				{
			  	//pøepoèet podle akt. jednotek
			  	if(j==1)//delka
						{if(j_delka!=MM)cislo1=cislo1/1000.0;}
			  	else//cas
						{if(j_cas!=SEC)cislo1=cislo1/60.0;}
					hodnota=F->m.round2double(cislo1,2);
				}
        //zápis do tabulky
				mGrid->Cells[j][i].Text=hodnota;
			}
    }
	}
  mGrid->Refresh();//pøekreslení
}
//---------------------------------------------------------------------------
