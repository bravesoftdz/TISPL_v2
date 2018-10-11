//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "gapoV.h"
#include "unit1.h"
#include "TmGrid.h"
#include "parametry_linky.h"
#include "parametry_vozik.h"
#include "kabina_schema.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHTMLLabel"
#pragma link "rHintWindow"
#pragma link "scGPImages"
#pragma resource "*.dfm"
TF_gapoV *F_gapoV;
//---------------------------------------------------------------------------
__fastcall TF_gapoV::TF_gapoV(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláře
	F_gapoV->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlačítek OK a Storno
	F->m.designButton(scGPButton_OK,F_gapoV,1,2);
	F->m.designButton(scGPButton_storno,F_gapoV,2,2);

	Offset=10;//odsazení thoto formu po všech stránách od tabulky

	//definice barev NEWR
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
  clHiddenValues = (TColor)RGB(212,212,212);

	// nastavení barvy orámování v hlavičce tabulky + orámování checkboxů
	C1=Form1->m.clIntensive(RGB(128,64,0),20);
	C2=Form1->m.clIntensive(RGB(0,128,0),80);
	C3=Form1->m.clIntensive(RGB(0,0,0),80);
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormActivate(TObject *Sender)
{
	//zde nevolat obsah form show, muže zde dělat problémy
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormShow(TObject *Sender)
{
  UlozitGAPOV=false;
	////////jednotky////////
	AnsiString T=F->readINI("nastaveni_form_parametry", "CT");
	if(T=="")CTunit=0;else CTunit=T.ToInt();
//	T=F->readINI("nastaveni_form_parametry","RDt");
//	if(T=="")RDunit=0;else RDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DD");
	if(T=="")DDunit=0;else DDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DM");
	if(T=="")Munit=0; else Munit =T.ToInt();

  RDunit=1;
  aRDunit=1;
  Runit=1;
  Rzunit=1;

  if(Form_parametry_linky->aRDunit==0)  //aRD = S
  {
  RDunit=0;
  aRDunit=0;
  }

  if(Form_parametry_linky->Runit==0)  //Runit = M
  {
  Runit=0;
  }

  if(Form_parametry_linky->Dmunit==0)  //Dmunit = M
  {
  Rzunit=0;
  }


  temp_pocitadlo=0;
  Rx_canEdit=true;
	AnsiString titulek;
	if(F->d.v.PP.delka_jig!=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value) titulek+="délky jigu z "+AnsiString(F->d.v.PP.delka_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value);
	if(F->d.v.PP.sirka_jig!=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value) titulek+="šířky jigu z "+AnsiString(F->d.v.PP.sirka_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value);
	if(F->d.v.PP.vyska_jig!=Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value) titulek+="výšky jigu z "+AnsiString(F->d.v.PP.vyska_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value);
	if(F->d.v.PP.delka_podvozek!=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value)  titulek+="délky podvozku z "+AnsiString(F->d.v.PP.delka_podvozek)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value);

  Form_parametry_linky->Button_save->Enabled=false;
  Form_parametry_linky->Button_storno->Enabled=false;
  myModalResult=mrNone;

	input_state=LOADING;
	pruchod=0;
	leva_oblast=false;
	//workaround odchytávání stisku kláves
	Edit1->SetFocus();
	////////definice tabulky////////
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=2;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodné jako druhé (popř. by bylo nutné překreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;

  unsigned int n_skupin=F->d.v.POHONY->predchozi->n+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
  indikator_skupin=new unsigned int[n_skupin];//dynamické pole, uchovávající indikaci, která oblast dané skupiny byla vybrána
	for(unsigned int i=0;i<n_skupin;i++)//i je index skupiny resp. id(n) pohonu
	{
		indikator_skupin[i]=1;//ID oblasti 1-3
	}

	////////vytvoření tabulky s požadovaným počtem sloupců a řádků////////
	unsigned long ColCount=34;//pevný počet slopců
	unsigned long RowCount=1;//dynamický počet řádků, default 1 je pro 0-tý indexový řádek
	RowCount+=F->d.v.pocet_objektu(-1);
	mGrid->Create(ColCount,RowCount);//samotné vytvoření matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty

 	////////error list - založení////////
	pm.createErrorList(RowCount);

	////////plnění daty - hlavička////////
	mGrid->Cells[0][0].Text="Pohon";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnutí tučného písma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
	mGrid->Cells[1][0].Text="Objekt";
	mGrid->Cells[2][0].Text="M,P";
	mGrid->Cells[3][0].Text="RP, Rz, Rx, R, CT, DD, K";
  //------------------------------------------------
	mGrid->Cells[4][0].Text="RP, M, Rz, Rx, K, CT, P";
	mGrid->Cells[5][0].Text="DD, R";
  mGrid->Cells[6][0].Text="RP, M, Rz, Rx, DD, P";
	mGrid->Cells[7][0].Text="K, CT, R";
  //----------------------------------------------
  mGrid->Cells[8][0].Text="RP, M, Rz, R, K, CT, P";
	mGrid->Cells[9][0].Text="DD, Rx";
	mGrid->Cells[10][0].Text="RP, M, Rz, R, DD, P";
	mGrid->Cells[11][0].Text="K, CT, Rx";
  //------------------------------------------------
	if(CTunit)mGrid->Cells[12][0].Text="CT - Technologický čas [min]";else mGrid->Cells[12][0].Text="CT - Technologický čas [s]";//mGrid->MergeCells(6,0,7,0);//sloučení zatím nefunguje dobře
	if(RDunit)mGrid->Cells[14][0].Text="RP - Rychlost pohonu [m/min]";else mGrid->Cells[14][0].Text="RP - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[16][0].Text="DD - Délka objektu [mm]";else mGrid->Cells[16][0].Text="DD - Délka objektu [m]";
	mGrid->Cells[18][0].Text="K - Kapacita [vozíků + mezer]";
	mGrid->Cells[20][0].Text="P - Pozice [vozíků]";
	if(Munit==0)mGrid->Cells[22][0].Text="M - mezera jig [m]"; else mGrid->Cells[22][0].Text="M - mezera jig [mm]";
	if(Munit==0)mGrid->Cells[24][0].Text="M - mezera vozík [m]"; else mGrid->Cells[24][0].Text="M - mezera vozík [mm]";
	mGrid->Cells[26][0].Text="Rotace";
  if(Runit==0)mGrid->Cells[27][0].Text="R - rozteč [m]"; else mGrid->Cells[27][0].Text="R - rozteč [mm]";
	if(Rzunit==0)mGrid->Cells[29][0].Text="Rz - akt. palce - rozestup [m]"; else mGrid->Cells[29][0].Text="Rz - akt. palce - rozestup [mm]"; //přepínání jednotek dodělat
	mGrid->Cells[31][0].Text="Rx - každy n-tý palec";
	mGrid->Cells[33][0].Text="Náhled";

 
	////////přiřadí celé oblasti buněk totožné vlastnosti jako u referenční buňky////////
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro první řádek

	//přiřazení jiného fontu, neměným položkám - NEWR
	mGrid->Cells[2][0].Font->Color=clUNLOCKED;
	mGrid->Cells[3][0].Font->Color=clLOCKED;
	mGrid->Cells[4][0].Font->Color=clUNLOCKED;
	mGrid->Cells[5][0].Font->Color=clLOCKED;
	mGrid->Cells[6][0].Font->Color=clUNLOCKED;
	mGrid->Cells[7][0].Font->Color=clLOCKED;
	mGrid->Cells[8][0].Font->Color=clUNLOCKED;
	mGrid->Cells[9][0].Font->Color=clLOCKED;
	mGrid->Cells[10][0].Font->Color=clUNLOCKED;
	mGrid->Cells[11][0].Font->Color=clLOCKED;

	//manualfit výšky 0-tého řádku (zatím není připravena metoda)
	unsigned short n=3;//číslo sloupce s nejdelším textem hlavičky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdelší použitý text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
  mGrid->Rows[0].Height= 	mGrid->Rows[0].Height + 30; //30 px je výška zamečku
	//manualfit šířky sloupců mimo nultého (ten je řešen automaticky níže pomocí SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=80;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=23;//ostatní následující sloupce zatím default šířka

	//nastavení velikosti nultého sloupce dle obsahu, může být umístěno kdekoliv před Show(), ale lépe před merge metodami
 //	mGrid->SetColumnAutoFit(0);  //  - odstavení chyba - odskakování
 // mGrid->Columns[0].Width=100;
	//sloučení buněk hlavičky PO  - vhodné za SetColumnAutoFit umístít

  	//manualfit šířky sloupců mimo prvního ten je polomanuální, víceméně provizorní řešení, měl by správně fungovat autofit na daný sloupec v mGridu
	//sloupec název pohony
	unsigned short tMax=0;if(F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false))tMax=mGrid->GetRecommendedColumnWidth("nepřiřazen");Cvektory::TPohon *p=F->d.v.POHONY->dalsi;while(p!=NULL){unsigned short tMaxLoc=mGrid->GetRecommendedColumnWidth(p->name);if(tMax<tMaxLoc)tMax=tMaxLoc;p=p->dalsi;}//najde nejdelší rozměr sloupce dle šířky řetězu
	mGrid->Columns[0].Width=tMax+6;//+6 pouze za orámování
	//sloupec shortname objekty
	if(F->d.v.vrat_pocet_nepouzivanych_pohonu())tMax=mGrid->GetRecommendedColumnWidth("nepřiřazen");else mGrid->GetRecommendedColumnWidth("CO2X");
	mGrid->Columns[1].Width=tMax+6;//+6 pouze za orámování
	//sloupec režim  - u gapoV není
 //	tMax=mGrid->GetRecommendedColumnWidth("S&G");if(F->d.v.pocet_objektu(1))tMax=mGrid->GetRecommendedColumnWidth("Kontinuální");if(F->d.v.pocet_objektu(2))tMax=mGrid->GetRecommendedColumnWidth("Postprocesní");
 //	mGrid->Columns[2].Width=tMax+6;//+6 pouze za orámování
	//ostatní sloupce, další zde neuvedené, jsou s defaultwidth
	mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=23;//ostatní následující sloupce zatím default šířka
	//nastavení velikosti sloupce dle ruční nastavení šířky daného sloupce, může být umístěno kdekoliv před Show(), ale lépe před merge metodami
	mGrid->SetColumnAutoFit(-4);


	mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);mGrid->MergeCells(20,0,21,0);mGrid->MergeCells(22,0,23,0);mGrid->MergeCells(24,0,25,0);mGrid->MergeCells(27,0,28,0);mGrid->MergeCells(29,0,30,0);mGrid->MergeCells(31,0,32,0);

	////////jednolivé řádky////////
	unsigned long j=1;//číslo aktuálně zpracovávaného řádku, musí začínat 1, 0 - je hlavička
	////průchod všemi objekty bez přiřazených pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false,-1);

	for(unsigned long i=1;i<=On_pocet;i++)//0-nultou buňku nevyužíváme necháváme prázdnou (z důvodu totožné indexace)
	{
		//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za účelem dalšího použití, pouze duplikát objektů, proto se nepropíše do spojáku OBJEKTY
		objekty[j]=On[i];
		//pohony

		mGrid->Cells[0][j].Text="nepřiřazen";mGrid->Cells[0][j].Background->Color=clBACKGROUND;
		//objekty
    AnsiString rezim;
    if (On[i].rezim==0) rezim="S&G";
    if (On[i].rezim==1) rezim="KK";
    if (On[i].rezim==2) rezim="PP";
		mGrid->Cells[1][j].Text=On[i].short_name+"-"+rezim;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
		//volby - checkboxy
		mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
		mGrid->MergeCells(2,j,3,j);
		mGrid->MergeCells(4,j,5,j);//sloučení sloupců
		mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
		mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//sloučení sloupců
		mGrid->Cells[10][j].Type=mGrid->CHECK;
		mGrid->MergeCells(10,j,11,j);
     if(On[i].rezim==0) {mGrid->getCheck(2,j)->Visible=false;mGrid->getCheck(4,j)->Visible=false;mGrid->getCheck(6,j)->Visible=false;mGrid->getCheck(8,j)->Visible=false;mGrid->getCheck(10,j)->Visible=false;}
     else  { mGrid->getCheck(2,j)->Visible=true;mGrid->getCheck(4,j)->Visible=true;mGrid->getCheck(6,j)->Visible=true;mGrid->getCheck(8,j)->Visible=true;mGrid->getCheck(10,j)->Visible=true;}
		//parametry objektů
		mGrid->Cells[12][j].Text=F->m.round2double(On[i].CT/(1+59.0*CTunit),2,"..");               mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT; mGrid->Cells[13][j].Font->Color=clUNLOCKED;
	//	mGrid->Cells[14][j].Text=F->m.round2double(On[i].RD*(1+59.0*RDunit),2,"..");                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
		mGrid->Cells[16][j].Text=F->m.round2double(On[i].delka_dopravniku*(1+999*DDunit),2,"..");  mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
		mGrid->Cells[18][j].Text=F->m.round2double(On[i].kapacita,2,"..");                           mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
		mGrid->Cells[20][j].Text=F->m.round2double(On[i].pozice,2,"..");                            mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clUNLOCKED;
		mGrid->Cells[22][j].Text=F->m.round2double(On[i].mezera_jig*(1+999*Munit),2,"..");           mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clUNLOCKED;
		mGrid->Cells[24][j].Text=F->m.round2double(On[i].mezera_podvozek*(1+999*Munit),2,"..");     mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clUNLOCKED;
		mGrid->Cells[26][j].Text=On[i].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
		if(On[i].pohon!=NULL)
		{
     	mGrid->Cells[14][j].Text=F->m.round2double(On[i].pohon->aRD*(1+59.0*RDunit),2,"..");                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
			mGrid->Cells[27][j].Text=F->m.round2double(On[i].pohon->roztec*(1+999.0)*Runit,2,"..");                     mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
			mGrid->Cells[29][j].Text=F->m.round2double(On[i].pohon->Rz*(1+999.0*Rzunit),2,"..");                       mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clUNLOCKED;
			mGrid->Cells[31][j].Text=On[i].pohon->Rx;                       mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
		}
    else
    {
     mGrid->Cells[27][j].Background->Color=clHiddenValues;  mGrid->Cells[28][j].Background->Color=clHiddenValues;
     mGrid->Cells[29][j].Background->Color=clHiddenValues;  mGrid->Cells[30][j].Background->Color=clHiddenValues;
     mGrid->Cells[31][j].Background->Color=clHiddenValues;  mGrid->Cells[32][j].Background->Color=clHiddenValues;
    }
		//výchozí nastavení v druhém levém slouci (popř. upravit, ale je problém s prvním - nelze vždy viz DV+M) - je vždy po zobrazení zaškrnuta tato volba
		mGrid->getCheck(4,j)->Checked=true;//pozor, při getCheck(3,j) mi zde házelo chybu, i přes ošetření R-záležitostí, při překliku na tuto položku po zobrazeném formu, ale OK
		//tlačítko náhledu
		mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		TscGPButton *B=mGrid->createButton(33,j);//vytvoření buttnu, lépě před následujícím cyklem, aby se později mohl parametrizovat
		B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		//zajistí přepočet daného řádku - nových hodnot
		calculate(j);//Rosto: musí být poslední před j++, nelze ho dát před defaultní zaškrtnutí checkboxů
		//posun na další řádek výsledné tabulky
     for(int sl=0;sl<=ColCount-1;sl++) //oddělení pohonů silnější čarou
    {
    mGrid->Cells[sl][j].BottomBorder->Width=2;
    }
		j++;
	}
	On=NULL;delete On;
 //	if(On_pocet>0)mGrid->MergeCells(0,1,0,On_pocet);//sloučení buněk objekty bez pohonu

	////průchod všemi pohony, které jsou přiřazeny k objektům
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buňku nevyužíváme necháváme prázdnou (z důvodu totožné indexace)
	{
   bool DVMmozno=true;
		unsigned long O_pocet=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,-1);
    if(O_pocet==0)//tzn. není objektu přiřazen žádný pohon
		{
			//pohony bez přiřazení k objektům v gapoV nezobrazujeme a neřešíme
		}
		else
		{
			//vratí formou ukazatele na pole objekty přiřazené k danému pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i,-1);
			unsigned long z=0;
      for(;z<O_pocet;z++)
			{
				//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za účelem dalšího použití, pouze duplikát objektů, proto se nepropíše do spojáku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;mGrid->Cells[0][j].Background->Color=clBACKGROUND;
				//objekty
        AnsiString rezim;
        if (O[z].rezim==0) rezim="S&G";
        if (O[z].rezim==1) rezim="Kont.";
        if (O[z].rezim==2) rezim="Post.";
				mGrid->Cells[1][j].Text=O[z].short_name+"-"+rezim;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
				//volby - checkboxy

				mGrid->Cells[4][j].Type=mGrid->CHECK;
//				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) <= 1 )  //KK režim
//				{
//					 mGrid->Cells[2][j].Type=mGrid->CHECK;
//					 mGrid->MergeCells(2,j,3,j);
//				}

        mGrid->Cells[2][j].Type=mGrid->CHECK;
        mGrid->MergeCells(2,j,3,j);

				mGrid->MergeCells(4,j,5,j);//sloučení sloupců
				mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
				mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//sloučení sloupců
				mGrid->Cells[10][j].Type=mGrid->CHECK;mGrid->MergeCells(10,j,11,j);

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK režim
				{   //pokud akt.název pohonu je odlišný od předchozího řádku, nastavím typ na CHECK.
					if(O[z].pohon->name!=mGrid->Cells[0][j-1].Text)
					{
					 //	mGrid->Cells[2][j].Type=mGrid->CHECK;
						//barvu prvního sloupce nastvuji níže, nelze zde
					}
          mGrid->getCheck(2,j)->Options->FrameNormalColor=C1;
					mGrid->getCheck(2,j)->OptionsChecked->FrameNormalColor=C1;

					mGrid->getCheck(4,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(4,j)->OptionsChecked->FrameNormalColor=C2;

					mGrid->getCheck(6,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(6,j)->OptionsChecked->FrameNormalColor=C2;

					mGrid->getCheck(8,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(8,j)->OptionsChecked->FrameNormalColor=C3;

					mGrid->getCheck(10,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(10,j)->OptionsChecked->FrameNormalColor=C3;

				}
        	if(O[z].pohon!=NULL)
				{
					double value=O[z].pohon->Rx;
					if(value!=floor(value))
					{    //samostatná metoda na kontrolu Rx
//						double dop_Rx=Form1->m.round(value);
//						vypis("Změna rozestupu palců (Rx) není možná, jelikož rozestup není celočíselný. Doporučený rozestup : "+ AnsiString(dop_Rx));
//						mGrid->getCheck(4,j)->Enabled=false;
//						mGrid->getCheck(6,j)->Enabled=false;
//						Rx_canEdit=false;
					}
				}

				//parametry objektů
				mGrid->Cells[12][j].Text=F->m.round2double(O[z].CT/(1+59.0*CTunit),2,"..");	             mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT; mGrid->Cells[13][j].Font->Color=clUNLOCKED;
			 //	mGrid->Cells[14][j].Text=F->m.round2double(O[z].RD*(1+59.0*1),2,"..");              mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				mGrid->Cells[16][j].Text=F->m.round2double(O[z].delka_dopravniku*(1+999*DDunit),2,".."); mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
				mGrid->Cells[18][j].Text=F->m.round2double(O[z].kapacita,2,"..");                        mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[20][j].Text=F->m.round2double(O[z].pozice,2,"..");                          mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clUNLOCKED;
				mGrid->Cells[22][j].Text=F->m.round2double(O[z].mezera_jig*(1+999*Munit),2,"..");        mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clUNLOCKED;
				mGrid->Cells[24][j].Text=F->m.round2double(O[z].mezera_podvozek*(1+999*Munit),2,"..");   mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clUNLOCKED;
				mGrid->Cells[26][j].Text=O[z].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
				if(O[z].pohon!=NULL)
				{
					mGrid->Cells[27][j].Text=F->m.round2double(O[z].pohon->roztec*(1+999.0*Runit),2,"..");                    mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
					mGrid->Cells[29][j].Text=F->m.round2double(O[z].pohon->Rz*(1+999.0*Rzunit),2,"..");                       mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;mGrid->Cells[30][j].Align=mGrid->LEFT;mGrid->Cells[30][j].Font->Color=clUNLOCKED;
					mGrid->Cells[31][j].Text=O[z].pohon->Rx;                       mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clOLD;mGrid->Cells[32][j].Align=mGrid->LEFT;mGrid->Cells[32][j].Font->Color=clUNLOCKED;
         	mGrid->Cells[14][j].Text=F->m.round2double(O[z].pohon->aRD*(1+59.0*1),2,"..");              mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				}
				mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
				TscGPButton *B=mGrid->createButton(33,j);B->Options->FontNormalColor=(TColor)RGB(255,128,0);//vytvoření buttnu, lépě před následujícím cyklem, aby se později mohl parametrizovat
				//výchozí nastavení v druhém levém slouci (popř. upravit, ale je problém s prvním - nelze vždy  viz DV+M) - je vždy po zobrazení zaškrnuta tato volba
				mGrid->getCheck(4,j)->Checked=true;
				//zajistí přepočet daného řádku - nových hodnot
			 	calculate(j);//Rosto: musí být poslední před j++, nelze ho dát před výchozí zaškrtnutí checkboxů
        //testování povolení DVM skupiny
        Memo1->Lines->Add("UDV + "+AnsiString(F->m.UDV(O[z].rotace)));
        Memo1->Lines->Add("puv. mezera"+AnsiString(O[z].mezera));
        Memo1->Lines->Add("==UDV +"+AnsiString(F->m.UDV(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace)));
        Memo1->Lines->Add("mezera mezi voz"+AnsiString(F->m.mezera_mezi_voziky(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace,O[z].pohon->roztec,0)));
        if(DVMmozno==true)
        {
         if(F->m.UDV(O[z].rotace)+O[z].mezera==F->m.UDV(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace)+F->m.mezera_mezi_voziky(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace,O[z].pohon->roztec,0))DVMmozno=true;
         else DVMmozno=true;//DVMmozno=false;
        }
				//posun na další řádek výsledné tabulky
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//sloučení buněk pohony
		//	mGrid->MergeCells(2,j-z,3,j-z+O_pocet-1);//sloučení buněk 2-3 - první checkbox
     	mGrid->MergeCells(14,j-z,14,j-z+O_pocet-1);//sloučení RP
      mGrid->MergeCells(15,j-z,15,j-z+O_pocet-1);//sloučení RP - výpočet
      mGrid->MergeCells(27,j-z,27,j-z+O_pocet-1);//sloučení R
      mGrid->MergeCells(28,j-z,28,j-z+O_pocet-1);//sloučení R - výpočet
      mGrid->MergeCells(29,j-z,29,j-z+O_pocet-1);//sloučení Rz
      mGrid->MergeCells(30,j-z,30,j-z+O_pocet-1);//sloučení Rz - výpočet
      mGrid->MergeCells(31,j-z,31,j-z+O_pocet-1);//sloučení Rx
      mGrid->MergeCells(32,j-z,32,j-z+O_pocet-1);//sloučení Rx - výpočet

      for(int sl=0;sl<=ColCount-1;sl++) //silnější oddělení dalšího pohonu
      {
       mGrid->Cells[sl][j-z+O_pocet-1].BottomBorder->Width=2;
      }

      if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK režim
      { //nelze nastavit hned, v horní části, společně s typem Check, ale až zde
      //  mGrid->getCheck(2,j-z)->Options->FrameNormalColor=C1;
      //  mGrid->getCheck(2,j-z)->OptionsChecked->FrameNormalColor=C1;
      }
       // ShowMessage((short)DVMmozno);
      if(DVMmozno) {mGrid->getCheck(2,j-z)->Enabled=true;  mGrid->getCheck(2,j-z)->ShowHint=true; mGrid->getCheck(2,j-z)->Hint="mohu";  }
      else {mGrid->getCheck(2,j-z)->Enabled=true; mGrid->getCheck(2,j-z)->ShowHint=false; mGrid->getCheck(2,j-z)->Hint="nemohu";   }

			O=NULL;delete O;
		}
	}

	////////rozdělení sekcí svislým tlustějším orámováním////////

    for(int i=1;i<=RowCount-1;i++)
    {
     mGrid->Cells[1][i].RightBorder->Width=2;
     mGrid->Cells[11][i].RightBorder->Width=2;
    }

    for(int j=0;j<=ColCount-1;j++)
    {
     mGrid->Cells[j][0].BottomBorder->Width=2;
    }

	////////autoresize a pozice formu_gapo, vhodné nakonec,tj. před Show//////// NEWR
	////velikost gapo formu a umístění komponent
	//šířka
	Width=mGrid->Width+Offset*2+1;
	if(Width<=F->Width)//pokud je užší nebo stejně jako šířka hlavního formu
	{
		scScrollBar_horizont->Visible=false;
	}
	else//je širší
	{
		Width=F->Width;
		scScrollBar_horizont->Visible=true;
		scScrollBar_horizont->Left=0;
		scScrollBar_horizont->Top=0+scGPPanel_hlavicka->Height;
		scScrollBar_horizont->Width=Width;
		scScrollBar_horizont->Position=0;
	}
	//výška
	Height=mGrid->Height+Offset*2+rHTMLLabel_InfoText->Height+scGPPanel_hlavicka->Height+11+scGPButton_OK->Height+11+rHTMLLabel_legenda->Height;// + 11 offset okolo tlačítka
	if(Height<=F->Height)//pokud je kratší než výška hlavní formu
	{
		scScrollBar_vertical->Visible=false;
	}
	else//je delší
	{
		Height=F->Height;
		Width+=scScrollBar_vertical->Width-Offset;//musím ještě rozšířit form, aby se vešel scrollbar
		scScrollBar_vertical->Visible=true;
		scScrollBar_vertical->Left=Width-scScrollBar_vertical->Width;
		scScrollBar_vertical->Top=scGPPanel_hlavicka->Height;
		scScrollBar_vertical->Height=Height-scGPPanel_hlavicka->Height;
		if(scScrollBar_horizont->Visible)//ošetření pokud jsou zobrazeny oba
		{
			scScrollBar_vertical->Top+=scScrollBar_horizont->Height;
			scScrollBar_vertical->Height=Height-scScrollBar_horizont->Width-scGPPanel_hlavicka->Height;
		}
		scScrollBar_vertical->Position=0;
	}

  for(int r=0;r<=RowCount-1;r++)
   {
    mGrid->Cells[13][r].RightBorder->Width=mGrid->Cells[15][r].RightBorder->Width=mGrid->Cells[17][r].RightBorder->Width=mGrid->Cells[19][r].RightBorder->Width=mGrid->Cells[21][r].RightBorder->Width=mGrid->Cells[23][r].RightBorder->Width=mGrid->Cells[25][r].RightBorder->Width=mGrid->Cells[26][r].RightBorder->Width=mGrid->Cells[28][r].RightBorder->Width=mGrid->Cells[30][r].RightBorder->Width=mGrid->Cells[32][r].RightBorder->Width=2;
   }

	////zobrazení orámování
	zobrazitFrameForm=true;


	//barevné orámování hlavičky
	mGrid->Cells[1][0].RightBorder->Color=C1;
	mGrid->Cells[2][0].BottomBorder->Color=mGrid->Cells[3][0].BottomBorder->Color=C1;
	mGrid->Cells[2][0].BottomBorder->Width=mGrid->Cells[3][0].BottomBorder->Width=2;

	mGrid->Cells[3][0].RightBorder->Color=C1;
	mGrid->Cells[3][0].RightBorder->Width=2;

	mGrid->Cells[4][0].BottomBorder->Color=	mGrid->Cells[5][0].BottomBorder->Color=mGrid->Cells[6][0].BottomBorder->Color= mGrid->Cells[7][0].BottomBorder->Color=C2;
	mGrid->Cells[4][0].BottomBorder->Width=	mGrid->Cells[5][0].BottomBorder->Width=mGrid->Cells[6][0].BottomBorder->Width= mGrid->Cells[7][0].BottomBorder->Width=2;

	mGrid->Cells[7][0].RightBorder->Width=2;
	mGrid->Cells[7][0].RightBorder->Color=C2;

	mGrid->Cells[8][0].BottomBorder->Color=mGrid->Cells[9][0].BottomBorder->Color=mGrid->Cells[10][0].BottomBorder->Color=mGrid->Cells[11][0].BottomBorder->Color=C3;
	mGrid->Cells[8][0].BottomBorder->Width=	mGrid->Cells[9][0].BottomBorder->Width=mGrid->Cells[10][0].BottomBorder->Width= mGrid->Cells[11][0].BottomBorder->Width=2;

	mGrid->Cells[11][0].RightBorder->Width=2;
	mGrid->Cells[11][0].RightBorder->Color=C3;

	F->m.designButton(scGPButton_OK,F_gapoV,1,2);
	F->m.designButton(scGPButton_storno,F_gapoV,2,2);
  scGPImage_zamky->Left=mGrid->Left+mGrid->Columns[2].Left+3;
	scGPImage_zamky->Top=mGrid->Top+2;
	//legenda pozice
	rHTMLLabel_InfoText->Top=mGrid->Top+mGrid->Height+1;//+1 kvůli orámování tabulky
	rHTMLLabel_InfoText->Left=mGrid->Left;
	rHTMLLabel_legenda_titulek->Top=rHTMLLabel_InfoText->Top;rHTMLLabel_legenda_titulek->Left=Width-rHTMLLabel_legenda->Width-Offset/2;
	rHTMLLabel_legenda->Top=rHTMLLabel_legenda_titulek->Top+rHTMLLabel_legenda_titulek->Height;rHTMLLabel_legenda->Left=rHTMLLabel_legenda_titulek->Left;
	rHTMLLabel_InfoText->Width=rHTMLLabel_legenda_titulek->Left-rHTMLLabel_InfoText->Left;
	////pozice gapo formu, pokud je stejně velký jako hlavní form, tak na 0 pozici, jinak na střed PL formu
	if(Width==F->Width)Left=0;else Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Width/2;
	if(Height==F->Height)Top=0;else Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_parametry_linky->scGPPanel2->Height/2-Height/2;//umístí na polovinu PL formuláře
	////zobrazení orámování
	zobrazitFrameForm=true;
	////kvůli špatně fungující funkci otáčení kolečka myši
	liche_otoceni_koleckem_mysi=false; //NEWR

	///uvolnění stavu
	input_state=FREE;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormPaint(TObject *Sender)
{
 	mGrid->Show();//vykreslí tabulku
 	if(zobrazitFrameForm)F->m.frameForm(this,clWebOrange,1);//vykreslí orámování okolo formu
}

//---------------------------------------------------------------------------
 void TF_gapoV::vypis(UnicodeString text,bool red,bool link)
{
		scGPButton_OK->Enabled=true;
		scGPButton_OK->Caption = "Uložit";
		if (text != "") // zobrazí a vypíše
		{
				rHTMLHint1->ToString()=text;//natežení do hintu zajišťuje zobrazení celého textu, nepoužívá se klasický hint
				//prodllužení formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl předtím popisek skrytý + kontrola pozice formu

				if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnutí podtrženého písma
				else rHTMLLabel_InfoText->Font->Style = TFontStyles();

				if (red)
				{
					 	scGPButton_OK->Enabled=false;
						rHTMLLabel_InfoText->Font->Color = clRed;
				}
				else
				{
						rHTMLLabel_InfoText->Font->Color = (TColor)RGB(0,128,255);
				}
				rHTMLLabel_InfoText->Left = 8;
				rHTMLLabel_InfoText->Caption = text;
				rHTMLLabel_InfoText->Visible = true;
		}
		else // skryje
		{
				//zkrácení formu if(rHTMLLabel_InfoText->Visible)Height-=(40+19);
				rHTMLLabel_InfoText->Visible = false;
		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TF_gapoV::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
//ZAJISTÍ, ŽE NELZE UDELAT UNCHECK - DEAKTIVOVÁNO, NEFUNGUJE PROTOŽE DOCHÁZÍ K PROGRAMOVÉMU NASTAVOVÁNÍ VIDITELNOSTI CHECKBOXŮ
//	if(Col>=4 && mGrid->getCheck(Col,Row)->Checked==false)
//	{
//		TscGPCheckBox *CH=mGrid->getCheck(Col,Row);
//		CH->Checked=true;
//		CH=NULL;delete CH;
//	}
 	 //	vypis("",false);


  canCalculate=false;

  	if(Col==2 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
    canCalculate=false;
		TscGPCheckBox *CH=mGrid->getCheck(Col+2,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col+4,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+6,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+8,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
    canCalculate=true;
	}

	if(Col==4 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
    canCalculate=false;
		TscGPCheckBox *CH=mGrid->getCheck(Col-2,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col+2,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+4,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+6,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
    canCalculate=true;
	}

	if(Col==6 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
    canCalculate=false;
		TscGPCheckBox *CH=mGrid->getCheck(Col-4,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-2,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+2,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+4,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
    canCalculate=true;
	}

	if(Col==8 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
   canCalculate=false;
		TscGPCheckBox *CH=mGrid->getCheck(Col-6,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-4,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-2,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+2,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
    canCalculate=true;
	}

	if(Col==10 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
    canCalculate=false;
		TscGPCheckBox *CH=mGrid->getCheck(Col-8,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-6,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-4,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col-2,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
    canCalculate=true;
	}




   	if(Col>=2 && Col<=3 && input_state==FREE && objekty[Row].rezim==1)
	{
		if(mGrid->getCheck(2,Row)->Checked==true)
		{
			// podívám se, zda pohon, který je na řádku, kde došlo ke kliku má více objektů v KK režimu, pokud ano, musím projít všechny
			 if(objekty[Row].pohon!=NULL)
			 {
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=1;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
					 //průchod celé tabulky

						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL &&  objekty[i].rezim==1)
							{
								if(pohon_n==objekty[i].pohon->n)
								{
									if (mGrid->getCheck(4,i)->Checked==true   ||  mGrid->getCheck(6,i)->Checked==true ||  mGrid->getCheck(8,i)->Checked==true ||  mGrid->getCheck(10,i)->Checked==true)
									{
                   input_state=PROGRAMOVE;
                //   pocitadlo++;

                    if(input_state==PROGRAMOVE)
                    {
										 vypis("Došlo k automatickému přesunutí výběru do nové oblasti, jelikož rozdílné oblasti nelze nastavit",false);
                     mGrid->getCheck(4,i)->Checked=false;
                     mGrid->getCheck(6,i)->Checked=false;
                     mGrid->getCheck(8,i)->Checked=false;
                     mGrid->getCheck(10,i)->Checked=false;
                     mGrid->getCheck(Col,i)->Checked=true;
                     calculate(i);
                     }
									}
								}
							}
						}
					 //	if(pocitadlo==0) vypis("",false);
					 } else vypis("",false);
			 }
		}
    input_state=FREE;
	 }
     // druhá oblast - prostřední
		if(Col>=4 && Col<=6 && input_state==FREE && objekty[Row].rezim==1)
		{
			if(mGrid->getCheck(4,Row)->Checked==true ||  mGrid->getCheck(6,Row)->Checked==true)
			{
		// podívám se, zda pohon, který je na řádku, kde došlo ke kliku má více objektů v KK režimu, pokud ano, musím projít všechny
				if(objekty[Row].pohon!=NULL)
				{
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=2;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
						//průchod celé tabulky
					 //	pocitadlo=0;
						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL &&  objekty[i].rezim==1)
							{
								if(pohon_n==objekty[i].pohon->n)
								{
									if(mGrid->getCheck(2,i)->Checked==true   ||  mGrid->getCheck(8,i)->Checked==true ||  mGrid->getCheck(10,i)->Checked==true)
									{
							   	//   pocitadlo++;
                    input_state=PROGRAMOVE;
                    if(input_state==PROGRAMOVE)
                    {
                    vypis("Došlo k automatickému přesunutí výběru do nové oblasti, jelikož rozdílné oblasti nelze nastavit",false);
                    // AnsiString T=calculate(Row,3);
                    // if(T!="") vypis(T);
                     mGrid->getCheck(2,i)->Checked=false;
                     mGrid->getCheck(8,i)->Checked=false;
                     mGrid->getCheck(10,i)->Checked=false;
                     mGrid->getCheck(Col,i)->Checked=true;
                     calculate(i);
                       }
                   }
								}
							}
						}
					//	if(pocitadlo==0)
            {
            vypis("",false);

            }
				 }  else
         {
//           AnsiString T=calculate(Row,3);// ShowMessage("tady");
//           if(T!="") vypis(T); else  vypis("",false);
         }
        }
			}
     input_state=FREE;
	 }

//	// třetí oblast - vpravo
		if(Col>=8 && Col<=10 && input_state==FREE && objekty[Row].rezim==1)
		{
			if(mGrid->getCheck(8,Row)->Checked==true ||  mGrid->getCheck(10,Row)->Checked==true)
			{
		// podívám se, zda pohon, který je na řádku, kde došlo ke kliku má více objektů v KK režimu, pokud ano, musím projít všechny
				if(objekty[Row].pohon!=NULL)
				{
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=3;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
						//průchod celé tabulky
					 //	pocitadlo=0;
						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL &&   objekty[i].rezim==1)
							{
								if(pohon_n==objekty[i].pohon->n)
								{
									if(mGrid->getCheck(2,i)->Checked==true   ||  mGrid->getCheck(4,i)->Checked==true ||  mGrid->getCheck(6,i)->Checked==true)
									{
							   //	   pocitadlo++;
                    input_state=PROGRAMOVE;
                    if(input_state==PROGRAMOVE)
                    {
                    vypis("Došlo k automatickému přesunutí výběru do nové oblasti, jelikož rozdílné oblasti nelze nastavit",false);
                     mGrid->getCheck(2,i)->Checked=false;
                     mGrid->getCheck(4,i)->Checked=false;
                     mGrid->getCheck(6,i)->Checked=false;
                     mGrid->getCheck(Col,i)->Checked=true;
                     calculate(i);
                       }
                   }
								}
							}
						}
					 //	if(pocitadlo==0) vypis("",false);
					 } else  vypis("",false);
				 }
			}
     input_state=FREE;
	 }


		//loadnutí default nastavení přepínačů
		 if(input_state==LOADING && Col>2)
	{

			//třetí resp. první výběrový sloupec je vždy předvybrán na true
	    if(objekty[Row].rezim!=0)
      {
      mGrid->getCheck(4,Row)->Checked=true;
      mGrid->getCheck(2,Row)->Visible=true;mGrid->getCheck(4,Row)->Visible=true;mGrid->getCheck(6,Row)->Visible=true;mGrid->getCheck(8,Row)->Visible=true;mGrid->getCheck(10,Row)->Visible=true;

      }
      else{ // SG režim - bez možnosti výběru checkboxu
      	mGrid->getCheck(4,Row)->Checked=false;
        {mGrid->getCheck(2,Row)->Visible=false;mGrid->getCheck(4,Row)->Visible=false;mGrid->getCheck(4,Row)->Visible=false;mGrid->getCheck(6,Row)->Visible=false;mGrid->getCheck(8,Row)->Visible=false;mGrid->getCheck(10,Row)->Visible=false;}

//        if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,0) == 1)
//         {
//         mGrid->getCheck(2,Row)->Visible=false;
//        }

        }
			mGrid->getCheck(6,Row)->Checked=false;
			mGrid->getCheck(8,Row)->Checked=false;
			mGrid->getCheck(10,Row)->Checked=false;


	 }

//

	if(Col==mGrid->ColCount-1)//je kliknuto na náhled objektu
	{
	//	calculate(Row,2);
		scGPButton_OK->Enabled=false;scGPButton_storno->Enabled=false;
		Form_objekt_nahled->zobrazitFrameForm=true;zobrazitFrameForm=false;
		Invalidate();FormPaint(this);//zajistí překreslení bez probliku
		Form_objekt_nahled->Left=Left+Width/2-Form_objekt_nahled->Width/2;
		Form_objekt_nahled->Top=Top+Height/2-Form_objekt_nahled->Height/2;
		Form_objekt_nahled->ShowModal();
		scGPButton_OK->Enabled=true;scGPButton_storno->Enabled=true;zobrazitFrameForm=true;
	}
		else//překliknutí chechboxu pravděpodobně
	{
	 if(input_state==FREE && Col>=2 && Col<=11 && canCalculate)
	 {
//     AnsiString T=calculate(Row,3);
//     if(T!="")F->MB(T);//pokud obsahuje chybový text

   		// podívám se, zda pohon, který je na řádku, kde došlo ke kliku má více objektů v KK režimu, pokud ano, musím projít všechny
     if(mGrid->Cells[0][Row].Text!="nepřiřazen")
     {
			if(objekty[Row].pohon!=NULL)
			{
				int pohon_n=objekty[Row].pohon->n;

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n) > 1)
				{

           slouceny_radek=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
             if(mGrid->Cells[0][i].Text!="nepřiřazen")
             {
							if(objekty[i].pohon!=NULL)
							{
								if(pohon_n==objekty[i].pohon->n)
								 {
                   slouceny_radek=i;
                 } //slouží k odlišení v calculate pro výpočet, abych věděl na který sloučený řádek mám vrátit data
              }
             }
            }

         } else slouceny_radek=0;
        }
      }
    calculate(Row);//zajistí přepočet daného řádku

		FormPaint(this);//zajistí překreslení bez probliku
		//Invalidate();
	 }
	}
}
//---------------------------------------------------------------------------
void TF_gapoV::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TF_gapoV::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//pro daný řádek dle nastaveného checkboxu, dopočítá a dosadí nové hodnoty parametrů daného objektu z daného řádku, v případě SaveTo -1, vrátí formou textu, oddělené středníky, 0 - nevrací nic, 1 uloží do binárky
UnicodeString TF_gapoV::calculate(unsigned long Row,short SaveTo)//NEWR
{
	////input sekce
	pm.TT=F->d.v.PP.TT;
	pm.rezim=objekty[Row].rezim;
	pm.CT=objekty[Row].CT;
	pm.DD=objekty[Row].delka_dopravniku;
	pm.K=objekty[Row].kapacita;
	pm.P=objekty[Row].pozice;
	pm.M=objekty[Row].mezera;
	pm.MJ=objekty[Row].mezera_jig;
	pm.MP=objekty[Row].mezera_podvozek;
  pm.dJ=Form_parametry_linky->scGPNumericEdit_delka_jig->Value/(1+999.0*Form_parametry_linky->Delkaunit);
  pm.sJ=Form_parametry_linky->scGPNumericEdit_sirka_jig->Value/(1+999.0*Form_parametry_linky->Delkaunit);
  pm.dP=Form_parametry_linky->scGPNumericEdit_delka_podvozek->Value/(1+999.0*Form_parametry_linky->Delkaunit);
	pm.Rotace=objekty[Row].rotace;
	if(objekty[Row].pohon!=NULL)
	{
		pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+59.0*Form_parametry_linky->aRDunit);//musí se brát ze stringgridu, kvůli přípapdnému stornu, nikoliv přímo z dat
		pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999.0*Form_parametry_linky->Runit);//musí se brát ze stringgridu, kvůli přípapdnému stornu, nikoliv přímo z dat		if(Rzunit==0)	pm.Rz=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[6][Form_parametry_linky->getROW(objekty[Row].pohon->n)]); else pm.Rz=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[6][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/1000.0;//musím brát ze stringgridu, kvůli stornu, nikoliv přímo z dat
		pm.Rx=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]);//musí se brát ze stringgridu, kvůli přípapdnému stornu, nikoliv přímo z dat
	}
	else
	{
		pm.RD=objekty[Row].RD;//je to správně?neměla by tu být 0?
		pm.R=0;
	}
	double oldRD=pm.RD;//záloha původní hodnoty
	double oldR=pm.R;//záloha původní hodnoty
	double oldRz=pm.Rz;//záloha původní hodnoty
	double oldRx=pm.Rx;//záloha původní hodnoty


	//optimalizace detekce a uchování volby zaškrtnutého checkboxu, aby se nemuselo vyvolávat znovu
	bool CHECK[5];
	if(mGrid->getCheck(2,Row)== NULL) 	CHECK[0]=false;
	else CHECK[0]=mGrid->getCheck(2,Row)->Checked;
	CHECK[1]=mGrid->getCheck(4,Row)->Checked;
	CHECK[2]=mGrid->getCheck(6,Row)->Checked;
	CHECK[3]=mGrid->getCheck(8,Row)->Checked;
	CHECK[4]=mGrid->getCheck(10,Row)->Checked;

	////výpočetní model
	//volání samotného výpočtu dle volby stanovéné pomoci checkboxu
	if(CHECK[0])//mění se M,P, zůstává aRD, RD, Rz, Rx, R, CT, DD, K
	{
		pm.P=pm.K2P(pm.K);//pozice
		pm.M=pm.Mezera(false);//mezera, vzhledem k tomu, že aRD,Rz,R zůstávají mezera se počítá pouze k aktuálnímu aRD resp. RD  - prepocet_Rzalezitosti na false
		if(SaveTo==0)
		{
			mGrid->Cells[13][Row].Font->Color=clLOCKED;//CT
			mGrid->Cells[15][Row].Font->Color=clLOCKED;//RD
			mGrid->Cells[17][Row].Font->Color=clLOCKED;//DD
			mGrid->Cells[19][Row].Font->Color=clLOCKED;//K
			mGrid->Cells[21][Row].Font->Color=clUNLOCKED;//P
			mGrid->Cells[23][Row].Font->Color=clUNLOCKED;//MJ
			mGrid->Cells[25][Row].Font->Color=clUNLOCKED;//MP
			mGrid->Cells[28][Row].Font->Color=clLOCKED;//R
			mGrid->Cells[30][Row].Font->Color=clLOCKED;//Rz
			mGrid->Cells[32][Row].Font->Color=clLOCKED;//Rx
		}
	}
	if(CHECK[1] || CHECK[2])//mění se
	{
		pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);//vrátí doporučenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteč mezi palci i takt
		pm.CT_locked=false;
		pm.DD_locked=true;

		if(CHECK[1])
		{
			mGrid->Cells[17][Row].Font->Color=clLOCKED;//DD
			pm.input_RD(true);
		}
		if(CHECK[2])//zůstává K,CT,R
		{
			pm.CT_locked=true;
			pm.DD_locked=false;
			pm.RD_locked=true;//zajistí přepočet DD v následujícím výpočtu
			pm.input_CT(false);
			pm.M=pm.Mezera(true);
			pm.P=pm.Pozice();
			mGrid->Cells[17][Row].Font->Color=clUNLOCKED;//DD
			mGrid->Cells[19][Row].Font->Color=clLOCKED;//K
			mGrid->Cells[13][Row].Font->Color=clLOCKED;//CT
		}

		if(SaveTo==0)
		{
			mGrid->Cells[13][Row].Font->Color=clUNLOCKED;//CT
			mGrid->Cells[15][Row].Font->Color=clUNLOCKED;//RD
			mGrid->Cells[19][Row].Font->Color=clUNLOCKED;//K
			mGrid->Cells[21][Row].Font->Color=clUNLOCKED;//P
			mGrid->Cells[23][Row].Font->Color=clUNLOCKED;//MJ
			mGrid->Cells[25][Row].Font->Color=clUNLOCKED;//MP
			mGrid->Cells[30][Row].Font->Color=clUNLOCKED;//Rz
			mGrid->Cells[32][Row].Font->Color=clUNLOCKED;//Rx
			mGrid->Cells[17][Row].Font->Color=clLOCKED;//DD
			mGrid->Cells[28][Row].Font->Color=clLOCKED; //R - vždy
		}
	}
	if(CHECK[3] || CHECK[4])//mění se
	{
		pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);//vrátí doporučenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteč mezi palci i takt
		pm.CT_locked=false;
		pm.DD_locked=true;

		if(CHECK[3])//vždy zůstává DD, Rx
		{
			pm.input_RD(true);
			mGrid->Cells[13][Row].Font->Color=clUNLOCKED; //CT
			mGrid->Cells[19][Row].Font->Color=clUNLOCKED;//K
			mGrid->Cells[17][Row].Font->Color=clLOCKED;//DD
			pm.Rx=oldRx;//korekce z původně chybně výpočítávané hodnoty
			pm.R=pm.Rz/pm.Rx;//Rx zůstává původně-původní
		}
		if(CHECK[4])//vždy zůstává K,CT,RX
		{
			pm.CT_locked=true;
			pm.DD_locked=false;
			pm.RD_locked=true;//zajistí přepočet DD v následujícím výpočtu
			pm.input_CT(false);
			pm.Rz=pm.TT*pm.RD;
			pm.R=pm.Rz/pm.Rx;
			pm.M=pm.Mezera();
			pm.P=pm.Pozice();
			mGrid->Cells[13][Row].Font->Color=clLOCKED; //CT
			mGrid->Cells[19][Row].Font->Color=clLOCKED;//K
			mGrid->Cells[17][Row].Font->Color=clUNLOCKED;//DD
		}

		if(SaveTo==0)
		{
			mGrid->Cells[13][Row].Font->Color=clLOCKED;//CT
			mGrid->Cells[15][Row].Font->Color=clUNLOCKED;//RD
			mGrid->Cells[17][Row].Font->Color=clUNLOCKED;//DD
			mGrid->Cells[19][Row].Font->Color=clLOCKED;//K
			mGrid->Cells[21][Row].Font->Color=clUNLOCKED;//P
			mGrid->Cells[23][Row].Font->Color=clUNLOCKED;//MJ
			mGrid->Cells[25][Row].Font->Color=clUNLOCKED;//MP
			mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//R
			mGrid->Cells[30][Row].Font->Color=clUNLOCKED;//Rz
      mGrid->Cells[32][Row].Font->Color=clLOCKED;//Rx
		}
	}
	//jednotné pro výše uvedené CHECK[] - detekce skupin - pokud je v oblasti objekt v KK režimu a další např. v PP- tak PP obj. neovlivní výpočet RD, KK má přednost. pole se plní v onclick událostech, jinými slovy volba dle objektů v KK režimu "přebíjí" volby PP objektu v dané skupině (od daného pohonu)
	if(mGrid->Cells[0][Row].Text != "nepřiřazen")  //MAKR
	{
		if (objekty[Row].pohon != NULL)
		{
			// Memo1->Lines->Add("R: "+AnsiString(Row)+ ", Ind.skupin" + AnsiString(indikator_skupin[objekty[Row].pohon->n]));
			switch (indikator_skupin[objekty[Row].pohon->n])
			{
				case 1://v této oblasti zůstávají původní hodnoty
				{
					pm.RD = oldRD;
					pm.Rz = oldRz;
					pm.Rx = oldRx;
					pm.R  = oldR;
				}break;
				case 2://v této oblasti zůstává R a mění se aRD,Rz,Rx
				{
					pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);
					pm.Rz = pm.RD*pm.TT;
					pm.Rx = pm.Rz/pm.R;
					pm.R  = oldR;
				}break;
				case 3://v této oblastí zůstává Rx a mění se aRD,Rz,Rx
				{
					pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);
					pm.Rz = pm.RD*pm.TT;
					pm.Rx = oldRx;
					pm.R = pm.Rz/pm.Rx;
				}break;
			}
		}
	}

	////output sekce
	AnsiString T="";
	switch(SaveTo)
	{
		 case -1://uložení do textu je-li požadováno
		 {
				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
		 }break;
		 case 0://pouze vrátí text do buněk
		 {
				mGrid->Cells[13][Row].Text = F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
				mGrid->Cells[17][Row].Text = F->m.round2double(pm.DD*(1+999*DDunit),2,"..");
				mGrid->Cells[19][Row].Text = F->m.round2double(pm.K,2,"..");
				mGrid->Cells[21][Row].Text = F->m.round2double(pm.P,2,"..");
				mGrid->Cells[23][Row].Text = F->m.round2double(pm.MJ*(1+999*Munit),2,"..");
				mGrid->Cells[25][Row].Text = F->m.round2double(pm.MP*(1+999*Munit),2,"..");
				if(objekty[Row].pohon!=NULL)
				{
					long chRow=Row;//měněný řádek
					if(slouceny_radek>1)chRow=slouceny_radek;//umístění do sloučených buněk na správnou pozici
					mGrid->Cells[15][chRow].Text = F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
					mGrid->Cells[30][chRow].Text = F->m.round2double(pm.Rz*(1+999*Rzunit),2,"..");
					mGrid->Cells[28][chRow].Text = F->m.round2double(pm.R*(1+999.0*Form_parametry_linky->Runit),2,"..");
					mGrid->Cells[32][chRow].Text = pm.Rx;
				}
				calculate(Row,3); //provede se validace
		 }
		 break;
		 case 1://uložení do spojáku OBJEKTY - je-li požadováno
		 {
				Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
				if(O->pohon!=NULL)
				{
					O->pohon->aRD = pm.RD;  //F->d.v.vrat_pohon(O->pohon->n)->aRD=pm.RD;
					O->pohon->Rz  = pm.Rz;
					O->pohon->Rx     = pm.Rx;
					O->pohon->roztec = pm.R;
				}
				O->CT=pm.CT;
				O->RD=pm.DD/pm.CT;//nelze použít pm.RD přímo, protože u S&G a PP by se RD!=aRD //POZOR: pm.RD je vždy aRD, až v sekci case 1 je vypočítáno skutečné RD=DD/CT
				O->delka_dopravniku=pm.DD;
				O->kapacita=pm.K;
				O->pozice=pm.P;
				O->mezera=pm.M;
				O->mezera_jig=pm.MJ;
				O->mezera_podvozek=pm.MP;
				O=NULL;delete O;
		 }break;
		 case 2://uložení hodnot z ukazatele pro náhled objektu
		 {
				Form_objekt_nahled->pom=new Cvektory::TObjekt;
				//Form_objekt_nahled->pom->pohon=objekty[Row].pohon;//takto by převzal starou rozteč
				Form_objekt_nahled->pom->pohon=new Cvektory::TPohon;
				Form_objekt_nahled->pom->pohon->roztec=pm.R;//ale předávám jen do náhledu R, nic víc od pohonu
				Form_objekt_nahled->pom->rezim=objekty[Row].rezim;
				Form_objekt_nahled->pom->CT=pm.CT;
				Form_objekt_nahled->pom->RD=pm.RD;
				Form_objekt_nahled->pom->delka_dopravniku=pm.DD;
				Form_objekt_nahled->pom->kapacita=pm.K;
				Form_objekt_nahled->pom->pozice=pm.P;
				Form_objekt_nahled->pom->mezera=pm.M;
				Form_objekt_nahled->pom->mezera_jig=pm.MJ;
				Form_objekt_nahled->pom->mezera_podvozek=pm.MP;
		 }break;
      case 3://testování dané volby, pokud není možno, vrácí text s popisem daného problému, jedná se o VALIDACI daného GAPO
		 {
				pm.gapoVALIDACE(objekty,Row,mGrid->RowCount,aRDunit);
				vypis(pm.getErrorText(mGrid->RowCount));
		 }break;

	}
	return T;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPButton_OKClick(TObject *Sender)
{
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		calculate(Row,1);//sice se propočítává opakovaně, ale kvůli možnému zobrazení dat ve zkrácené formě v tabulce. lepe z ostrých dat
		//aktualizace PO, pokud je spuštěné
		if(F->pom!=NULL)
    {//nutno jako samostaný if
			if(F->pom->n==objekty[Row].n)//je spuštěné PO a je tedy nutné zavolat aktualizaci PO
			{
				F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vrátí ostrá data
				F->NPin();
			}
		}
	}
  UlozitGAPOV=true;
  myModalResult=mrOk;
  Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
  Form_parametry_linky->Button_saveClick(Sender);
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] objekty;
  delete[] indikator_skupin;
	pm.deleteErrorList();
	scGPButton_storno->SetFocus();//workaround proti padání mGridu (padalo při odstraňování komponent), Focus se přesune z mazané komponenty na mGridu, na komponentu nemazanou
  mGrid->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPButton_stornoClick(TObject *Sender)
{
	Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
  if(Form_parametry_linky->Delkaunit==0)
  {
		Form_parametry_linky->scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig;
		Form_parametry_linky->scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig;
		Form_parametry_linky->scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig;
	}
	else
	{
		Form_parametry_linky->scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig*1000.0;
		Form_parametry_linky->scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig*1000.0;
		Form_parametry_linky->scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig*1000.0;
	}
  myModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	switch(Key)
	{
		case 13: // ENTER
		{
			if (scGPButton_OK->Enabled)scGPButton_OKClick(Sender);// pokud jsou zároveň splněny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		}break;
		case 27:// ESC
		{
			Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
			F_gapoV->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlačítko
			F_gapoV->VisibleChanging();// skryje form, stejné jako visible=false
			myModalResult=mrCancel;
			Form_parametry_linky->scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig;
			Form_parametry_linky->scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig;
			Form_parametry_linky->scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig;
		}break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
					int Y)
{
	mGrid->HighlightRowOnMouse(X,Y,(TColor)RGB(240,240,240));
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scButton_csvClick(TObject *Sender)
{
	AnsiString T="";
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		T+=calculate(Row,-1)+"\n";//sice se propočítává opakovaně, ale kvůli možnému zobrazení dat ve zkrácené formě v tabulce. lepe z ostrých dat
	}
	F->d.v.Text2CSV(T,Form1->FileName+"_tabulka_gapoV","Uložit hodnoty do CSV...","*.csv","Soubory formátu CSV (*.csv)|*.csv|Soubory formátu XLS (*.xls)|*.xls");
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPGlyphButton_copyClick(TObject *Sender)
{
	mGrid->CopyCells2Clipboard(0,0,mGrid->ColCount-1,mGrid->RowCount-1);
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::KonecClick(TObject *Sender)
{
  scGPButton_stornoClick(Sender);
  Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scScrollBar_horizontChange(TObject *Sender)
{
	mGrid->Left=F->m.round((Width-mGrid->Width-Offset)*scScrollBar_horizont->Position/100.0);
	scGPImage_zamky->Left=mGrid->Left+mGrid->Columns[2].Left+3;
	if(scScrollBar_horizont->Position<scScrollBar_horizont->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci musí překreslit celé
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::Button1Click(TObject *Sender)
{
ShowMessage(rHTMLLabel_InfoText->Caption);
ShowMessage(rHTMLLabel_InfoText->Height);
ShowMessage(rHTMLLabel_InfoText->Width);
rHTMLLabel_InfoText->Visible=true;
rHTMLLabel_InfoText->Caption="adfasdfdasfdasfdasf";
}
//---------------------------------------------------------------------------

