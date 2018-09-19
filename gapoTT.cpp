//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gapoTT.h"
#include "unit1.h"
#include "TmGrid.h"
#include "PO_math.h"
#include "parametry_linky.h"
#include "parametry.h"
#include "kabina_schema.h"
#include "TT_kalkulator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHintWindow"
#pragma link "rHTMLLabel"
#pragma link "scExtControls"
#pragma link "scGPImages"
#pragma resource "*.dfm"
TF_gapoTT *F_gapoTT;
//---------------------------------------------------------------------------
__fastcall TF_gapoTT::TF_gapoTT(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláøe
	F_gapoTT->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK a STORNO    NEWR
	F->m.designButton(scGPButton_OK,F_gapoTT,1,2);
	F->m.designButton(scGPButton_storno,F_gapoTT,2,2);
	rHTMLLabel_InfoText->Font->Color = (TColor)RGB(43,87,154);//(TColor)RGB(0,128,255);

	Offset=10;//odsazení thoto formu po všech stránách od tabulky

	////definice barev
	//položky
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
  clHiddenValues = (TColor)RGB(250,250,250);//(TColor)RGB(212,212,212);
	//nastavení barvy orámování v hlavièce tabulky + orámování checkboxù
	C1=Form1->m.clIntensive(RGB(128,64,0),20);
	C2=Form1->m.clIntensive(RGB(0,128,0),80);
	C3=Form1->m.clIntensive(RGB(0,0,0),80);
	//legenda NEWR
	rHTMLLabel_legenda_titulek->FontColor=(TColor)RGB(43,87,154);
	rHTMLLabel_legenda->FontColor=clUNLOCKED;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormShow(TObject *Sender)
{
	////////definice tabulky////////
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=1;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;
  mGrid->Border.Width=2;
	input_state=LOADING;
	Rx_canEdit=true;
  AnsiString jednotky;
  temp_pocitadlo=0;
  unsigned int n_skupin=F->d.v.POHONY->predchozi->n+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
  indikator_skupin=new unsigned int[n_skupin];//dynamické pole, uchovávající indikaci, která oblast dané skupiny byla vybrána
  for(unsigned int i=0;i<n_skupin;i++)//i je index skupiny resp. id(n) pohonu
  {
    indikator_skupin[i]=1;//ID oblasti 1-3
  }


	//workaround odchytávání stisku kláves
	Edit1->SetFocus();


	////////jednotky////////
	AnsiString T=F->readINI("nastaveni_form_parametry", "CT");
	if(T=="")CTunit=0;else CTunit=T.ToInt();
 //	T=F->readINI("nastaveni_form_parametry","RDt");   - naèítáno z PL akt. nastavení
	//if(T=="")RDunit=0;else RDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DD");
	if(T=="")DDunit=0;else DDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DM");
	if(T=="")Munit=0; else Munit =T.ToInt();

   if(Form_parametry_linky->Taktunit) jednotky=" [min]"; else  jednotky=" [s]";
  scLabel_titulek->Caption="Globální aktualizace parametrù pohonù a objektù z dùvodu zmìny TT z "+AnsiString(F->d.v.PP.TT)+" na " +AnsiString(Form_TT_kalkulator->rEditNum_takt->Value) + AnsiString(jednotky);

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

	////////vytvoøení tabulky s požadovaným poètem sloupcù a øádkù////////
	unsigned long ColCount=37;//pevný poèet slopcù      //NEWR
	unsigned long RowCount=1;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
	RowCount+=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false)+F->d.v.vrat_pocet_nepouzivanych_pohonu()+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(true);//PØIDAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	mGrid->Create(ColCount,RowCount);//samotné vytvoøení matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty

	////////plnìní daty - hlavièka////////   NEWR
	mGrid->Cells[0][0].Text="pohon";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnutí tuèného písma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
  mGrid->Cells[0][0].BottomBorder->Width=2;

	mGrid->Cells[1][0].Text="objekt";
  mGrid->Cells[1][0].BottomBorder->Width=2;
	mGrid->Cells[2][0].Text="režim";
  mGrid->Cells[2][0].BottomBorder->Width=2;
  scImage_zamky->Top=45;
  scImage_zamky->Left=mGrid->Columns[3].Left-6;
	//------------------------------------------------
	mGrid->Cells[3][0].Text="RP*, CT";
	mGrid->Cells[4][0].Text="DD, K, P, Rz, Rx, R, M";
	//------------------------------------------------
	mGrid->Cells[5][0].Text="RP*, DD, K, P";
	mGrid->Cells[6][0].Text="CT, Rz, Rx, R, M";
	//------------------------------------------------
	mGrid->Cells[7][0].Text="Rz, Rx, M, DD, P, CT";
	mGrid->Cells[8][0].Text="RP, R, K";
	//------------------------------------------------
	mGrid->Cells[9][0].Text="Rz, Rx, M, K, P";
	mGrid->Cells[10][0].Text="RP, R, DD, CT";
	//------------------------------------------------
	mGrid->Cells[11][0].Text="Rz, R, M, DD, P, CT";
	mGrid->Cells[12][0].Text="RP, Rx, K";
	//------------------------------------------------
	mGrid->Cells[13][0].Text="Rz, R, M, K, P";
	mGrid->Cells[14][0].Text="RP, Rx, DD, CT";

	mGrid->Cells[3][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[3][0].Font->Orientation=900;
	mGrid->Cells[3][0].Align=mGrid->CENTER;
	mGrid->Cells[3][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[3][0].BottomMargin=4;mGrid->Cells[3][0].TopMargin=8;
  mGrid->Cells[3][0].BottomBorder->Width=2;

	mGrid->Cells[4][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[4][0].Font->Orientation=900;
	mGrid->Cells[4][0].Align=mGrid->CENTER;
	mGrid->Cells[4][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[4][0].BottomMargin=4;mGrid->Cells[4][0].TopMargin=8;
	mGrid->Cells[4][0].Background->Color=clBACKGROUND;
  mGrid->Cells[4][0].BottomBorder->Width=2;


	mGrid->Cells[5][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[5][0].Font->Orientation=900;
	mGrid->Cells[5][0].Align=mGrid->CENTER;
	mGrid->Cells[5][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[5][0].BottomMargin=4;mGrid->Cells[5][0].TopMargin=8;
	mGrid->Cells[5][0].Background->Color=clBACKGROUND;
  mGrid->Cells[5][0].BottomBorder->Width=2;

	mGrid->Cells[6][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[6][0].Font->Orientation=900;
	mGrid->Cells[6][0].Align=mGrid->CENTER;
	mGrid->Cells[6][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[6][0].BottomMargin=4;mGrid->Cells[6][0].TopMargin=8;
	mGrid->Cells[6][0].Background->Color=clBACKGROUND;
  mGrid->Cells[6][0].BottomBorder->Width=2;

	mGrid->Cells[7][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[7][0].Font->Orientation=900;
	mGrid->Cells[7][0].Align=mGrid->CENTER;
	mGrid->Cells[7][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[7][0].BottomMargin=4;mGrid->Cells[7][0].TopMargin=8;
	mGrid->Cells[7][0].Background->Color=clBACKGROUND;
  mGrid->Cells[7][0].BottomBorder->Width=2;

	mGrid->Cells[8][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[8][0].Font->Orientation=900;
	mGrid->Cells[8][0].Align=mGrid->CENTER;
	mGrid->Cells[8][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[8][0].BottomMargin=4;mGrid->Cells[8][0].TopMargin=8;
	mGrid->Cells[8][0].Background->Color=clBACKGROUND;
  mGrid->Cells[8][0].BottomBorder->Width=2;

	mGrid->Cells[9][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[9][0].Font->Orientation=900;
	mGrid->Cells[9][0].Align=mGrid->CENTER;
	mGrid->Cells[9][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[9][0].BottomMargin=4;mGrid->Cells[9][0].TopMargin=8;
	mGrid->Cells[9][0].Background->Color=clBACKGROUND;
  mGrid->Cells[9][0].BottomBorder->Width=2;

	mGrid->Cells[10][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[10][0].Font->Orientation=900;
	mGrid->Cells[10][0].Align=mGrid->CENTER;
	mGrid->Cells[10][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[10][0].BottomMargin=4;mGrid->Cells[10][0].TopMargin=8;
	mGrid->Cells[10][0].Background->Color=clBACKGROUND;
  mGrid->Cells[10][0].BottomBorder->Width=2;

	mGrid->Cells[11][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[11][0].Font->Orientation=900;
	mGrid->Cells[11][0].Align=mGrid->CENTER;
	mGrid->Cells[11][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[11][0].BottomMargin=4;mGrid->Cells[11][0].TopMargin=8;
	mGrid->Cells[11][0].Background->Color=clBACKGROUND;
  mGrid->Cells[11][0].BottomBorder->Width=2;

	mGrid->Cells[12][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[12][0].Font->Orientation=900;
	mGrid->Cells[12][0].Align=mGrid->CENTER;
	mGrid->Cells[12][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[12][0].BottomMargin=4;mGrid->Cells[12][0].TopMargin=8;
	mGrid->Cells[12][0].Background->Color=clBACKGROUND;
  mGrid->Cells[12][0].BottomBorder->Width=2;

	mGrid->Cells[13][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[13][0].Font->Orientation=900;
	mGrid->Cells[13][0].Align=mGrid->CENTER;
	mGrid->Cells[13][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[13][0].BottomMargin=4;mGrid->Cells[13][0].TopMargin=8;
	mGrid->Cells[13][0].Background->Color=clBACKGROUND;
  mGrid->Cells[14][0].BottomBorder->Width=2;

	mGrid->Cells[14][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[14][0].Font->Orientation=900;
	mGrid->Cells[14][0].Align=mGrid->CENTER;
	mGrid->Cells[14][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[14][0].BottomMargin=4;mGrid->Cells[14][0].TopMargin=8;
	mGrid->Cells[14][0].Background->Color=clBACKGROUND;
  mGrid->Cells[14][0].BottomBorder->Width=2;

	if(CTunit)mGrid->Cells[15][0].Text="CT - Technologický èas [min]";else mGrid->Cells[15][0].Text="CT - Technologický èas [s]";
	if(RDunit)mGrid->Cells[17][0].Text="RP - Rychlost pohonu [m/min]";else mGrid->Cells[17][0].Text="RP - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[19][0].Text="DD - Délka objektu [mm]";else mGrid->Cells[19][0].Text="DD - Délka objektu [m]";
	mGrid->Cells[21][0].Text="K - Kapacita [vozíkù+mezer]";
	mGrid->Cells[23][0].Text="P - Pozice [vozíkù]";
	if(Munit==0)mGrid->Cells[25][0].Text="M - mezera jig [m]"; else mGrid->Cells[25][0].Text="M - mezera jig [mm]";
	if(Munit==0)mGrid->Cells[27][0].Text="M - mezera vozík [m]"; else mGrid->Cells[27][0].Text="M - mezera vozík [mm]";
	mGrid->Cells[29][0].Text="Rotace";
	if(Runit==0)mGrid->Cells[30][0].Text="R - rozteè [m]"; else mGrid->Cells[30][0].Text="R - rozteè [mm]";
	if(Rzunit==0)mGrid->Cells[32][0].Text="Rz - akt. palce - rozestup [m]"; else mGrid->Cells[32][0].Text="Rz - akt. palce - rozestup [mm]"; //pøepínání jednotek dodìlat
	mGrid->Cells[34][0].Text="Rx - každy n-tý palec";

	mGrid->Cells[36][0].Text="náhled";
 // if(aRDunit)mGrid->Cells[37][0].Text="aRD - akt. rychlost pohonu [m/min]";else mGrid->Cells[17][0].Text="aRD -  akt. rychlost pohonu [m/s]";

	////////pøiøadí celé oblasti bunìk totožné vlastnosti jako u referenèní buòky////////
	mGrid->SetCells(mGrid->Cells[0][0],15,0,ColCount-1,0);//pro první øádek   sloupce 15+
	mGrid->SetCells(mGrid->Cells[0][0],1,0,3,0);//pro první øádek   sloupce 1-3

	//pøiøazení jiného fontu, nemìným položkám - NEWR
	mGrid->Cells[3][0].Font->Color=clUNLOCKED;
	mGrid->Cells[4][0].Font->Color=clLOCKED;
	mGrid->Cells[5][0].Font->Color=clUNLOCKED;
	mGrid->Cells[6][0].Font->Color=clLOCKED;
	mGrid->Cells[7][0].Font->Color=clUNLOCKED;
	mGrid->Cells[8][0].Font->Color=clLOCKED;
	mGrid->Cells[9][0].Font->Color=clUNLOCKED;
	mGrid->Cells[10][0].Font->Color=clLOCKED;
	mGrid->Cells[11][0].Font->Color=clUNLOCKED;
	mGrid->Cells[12][0].Font->Color=clLOCKED;
	mGrid->Cells[13][0].Font->Color=clUNLOCKED;
	mGrid->Cells[14][0].Font->Color=clLOCKED;

	//manualfit výšky 0-tého øádku (zatím není pøipravena metoda)
	unsigned short n=37;if(RDunit)n=17;//èíslo sloupce s nejdelším textem hlavièky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdelší použitý text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[17][0].Text)+mGrid->Cells[17][0].BottomMargin+mGrid->Cells[17][0].BottomBorder->Width/2+mGrid->Cells[17][0].TopMargin+mGrid->Cells[17][0].TopBorder->Width/2;
	//manualfit šíøky sloupcù mimo prvního (ten je øešen automaticky níže pomocí SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=100;mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=mGrid->Columns[12].Width=mGrid->Columns[13].Width=mGrid->Columns[14].Width=23;//ostatní následující sloupce zatím default šíøka
	mGrid->Columns[2].Width=120;

	//nastavení velikosti nultého sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show(), ale lépe pøed merge metodami
 	mGrid->SetColumnAutoFit(0);
	//workaround výše uvedeného, protože nefunguje zcela správnì - taky nejede
//	mGrid->Columns[0].Width=100;
	//mGrid->SetColumnAutoFit();

	//slouèení bunìk hlavièky PO  - vhodné za SetColumnAutoFit umístít - NEWR
	mGrid->MergeCells(15,0,16,0);mGrid->MergeCells(17,0,18,0);mGrid->MergeCells(19,0,20,0);
	mGrid->MergeCells(21,0,22,0);mGrid->MergeCells(23,0,24,0);mGrid->MergeCells(25,0,26,0);mGrid->MergeCells(27,0,28,0);
	mGrid->MergeCells(30,0,31,0);mGrid->MergeCells(32,0,33,0);mGrid->MergeCells(34,0,35,0);//mGrid->MergeCells(37,0,38,0);
	////////jednolivé øádky////////
	unsigned long j=1;//èíslo aktuálnì zpracovávaného øádku, musí zaèínat 1, 0 - je hlavièka
	////prùchod všemi objekty bez pøiøazených pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
	for(unsigned long i=1;i<=On_pocet;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		////OBJEKTY BEZ PØIØAZENÉHO POHONU
		//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
		objekty[j]=On[i];

  	//pohony
		mGrid->Cells[0][j].Text="nepøiøazen";
		//objekty
		mGrid->Cells[1][j].Text=On[i].short_name;
		//režim
		AnsiString rezimJ;
		if(On[i].rezim==0) rezimJ="S&G";
		if(On[i].rezim==1) rezimJ="Kontinuální";
		if(On[i].rezim==2) rezimJ="Postprocesní";
		mGrid->Cells[2][j].Text=rezimJ;

		//volby - checkboxy
		mGrid->Cells[3][j].Type=mGrid->CHECK;mGrid->MergeCells(3,j,4,j);

		if(On[i].rezim!=0)//povolení zobrazení checkboxù pro KK a PP režim
		{
			mGrid->Cells[5][j].Type=mGrid->CHECK;mGrid->MergeCells(5,j,6,j);
			mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->MergeCells(7,j,8,j);
			mGrid->Cells[9][j].Type=mGrid->CHECK;mGrid->MergeCells(9,j,10,j);
			mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->MergeCells(11,j,12,j);
			mGrid->Cells[13][j].Type=mGrid->CHECK;mGrid->MergeCells(13,j,14,j);
		}
		else// u SG slouèím pouze buòky, ale checkbox bude nabízen pouze u prvního "výbìrového" sloupce
		{
			mGrid->MergeCells(5,j,6,j);
			mGrid->MergeCells(7,j,8,j);
			mGrid->MergeCells(9,j,10,j);
			mGrid->MergeCells(11,j,12,j);
			mGrid->MergeCells(13,j,14,j);
		}

		//parametry objektù  // cell 17 - pùvodnì používána pro RD, nyní je v ní zobrazováno aRD
		mGrid->Cells[15][j].Text=F->m.round2double(On[i].CT/(1+59.0*CTunit),2,"..");	 								mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clOLD;mGrid->Cells[16][j].Align=mGrid->LEFT; mGrid->Cells[16][j].Font->Color=clUNLOCKED;
	 //	mGrid->Cells[17][j].Text=F->m.round2double(On[i].RD*(1+59.0*RDunit),2,"..");                  mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT; mGrid->Cells[18][j].Font->Color=clUNLOCKED;
		mGrid->Cells[19][j].Text=F->m.round2double(On[i].delka_dopravniku*(1+999.0*DDunit),2,"..");    	mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clOLD;mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clUNLOCKED;
		mGrid->Cells[21][j].Text=F->m.round2double(On[i].kapacita,2,"..");                          	mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clOLD;mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clUNLOCKED;
		mGrid->Cells[23][j].Text=F->m.round2double(On[i].pozice,2,"..");                             	mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clOLD;mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clUNLOCKED;
		mGrid->Cells[25][j].Text=F->m.round2double(On[i].mezera_jig*(1+999.0*Munit),2,"..");           	mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clOLD;mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clUNLOCKED;
		mGrid->Cells[27][j].Text=F->m.round2double(On[i].mezera_podvozek*(1+999.0*Munit),2,"..");  			mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
		mGrid->Cells[29][j].Text=AnsiString(On[i].rotace);             			mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;
		if(On[i].pohon!=NULL)
		{
			mGrid->Cells[30][j].Text=F->m.round2double(On[i].pohon->roztec*(1+999.0*Runit),2,"..");           mGrid->Cells[30][j].Align=mGrid->LEFT;mGrid->Cells[30][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
			mGrid->Cells[32][j].Text=F->m.round2double(On[i].pohon->Rz*(1+999.0*Rzunit),2,"..");               mGrid->Cells[32][j].Align=mGrid->LEFT;mGrid->Cells[32][j].Font->Color=clOLD;mGrid->Cells[33][j].Align=mGrid->LEFT;mGrid->Cells[33][j].Font->Color=clUNLOCKED;
			mGrid->Cells[34][j].Text=On[i].pohon->Rx;               mGrid->Cells[34][j].Align=mGrid->LEFT;mGrid->Cells[34][j].Font->Color=clOLD;mGrid->Cells[35][j].Align=mGrid->LEFT;mGrid->Cells[35][j].Font->Color=clUNLOCKED;
     	mGrid->Cells[17][j].Text=F->m.round2double(On[i].pohon->aRD*(1+59.0*aRDunit),2,"..");              mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clUNLOCKED;
		}
    else
    {
     mGrid->Cells[30][j].Background->Color=clHiddenValues;    mGrid->Cells[31][j].Background->Color=clHiddenValues;
     mGrid->Cells[32][j].Background->Color=clHiddenValues;    mGrid->Cells[33][j].Background->Color=clHiddenValues;
     mGrid->Cells[34][j].Background->Color=clHiddenValues;    mGrid->Cells[35][j].Background->Color=clHiddenValues;
    }
		mGrid->Cells[36][j].Type=mGrid->BUTTON;mGrid->Cells[36][j].Text="...";mGrid->Cells[36][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		TscGPButton *B=mGrid->createButton(36,j);//vytvoøení buttnu, lépì pøed následujícím cyklem, aby se pozdìji mohl parametrizovat
		/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		//B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//padá
		//checkboxy
		if(On[i].rezim==0) //fixní nastavení checkboxu u SG režimù
		{
			TscGPCheckBox *CH=mGrid->getCheck(3,j);
			CH->Checked=true;
			CH->Enabled=false;
			CH=NULL;delete CH;
		}
		else
		{
			TscGPCheckBox *CH=mGrid->getCheck(3,j);
			CH->Checked=true;
			CH=NULL;delete CH;
		}
		//zajistí pøepoèet daného øádku
	 	calculate(j);
    for(int sl=0;sl<=ColCount-1;sl++) //oddìlení pohonù silnìjší èarou
    {
    mGrid->Cells[sl][j].BottomBorder->Width=2;
    }
		//posun na další øádek výsledné tabulky
		j++;
	}
	On=NULL;delete On;
	if(On_pocet>0)mGrid->MergeCells(0,1,0,On_pocet);//slouèení bunìk objekty bez pohonu

	////prùchod všemi pohony
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		unsigned long O_pocet=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
		if(O_pocet==0)//tzn. pohonu není pøiøazen žádný objekt
		{
			//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
			objekty[j].id=100+i;//pøiøadí id 100+èíslo pohonu, nešlo napsat pøímo NULL, nutno poøešit pøi prùchodu cyklem následného vypisování objekty[x]
			//pohony
			mGrid->Cells[0][j].Text=F->d.v.vrat_pohon(i)->name;
			//objekty
			mGrid->Cells[1][j].Text="nepøiøazen";
			//volby - checkboxy
			mGrid->Cells[3][j].Type=mGrid->CHECK;
			mGrid->Cells[5][j].Type=mGrid->CHECK;
			//mGrid->MergeCells(1,j,2,j);
			mGrid->MergeCells(3,j,4,j);
			mGrid->MergeCells(5,j,6,j);//slouèení sloupcù
			mGrid->MergeCells(3,j,5,j);
			//mGrid->Cells[3][j].Align=mGrid->RIGHT;
			mGrid->Cells[7][j].Type=mGrid->CHECK;
			mGrid->Cells[9][j].Type=mGrid->CHECK;
			mGrid->MergeCells(7,j,8,j);
			mGrid->MergeCells(9,j,10,j);//slouèení sloupcù
      mGrid->MergeCells(7,j,9,j);//slouèení sloupcù
			mGrid->Cells[11][j].Type=mGrid->CHECK;
			mGrid->Cells[13][j].Type=mGrid->CHECK;
			mGrid->MergeCells(11,j,12,j);
			mGrid->MergeCells(13,j,14,j);//slouèení sloupcù
      mGrid->MergeCells(11,j,13,j);//slouèení sloupcù
			TscGPCheckBox *CH=mGrid->getCheck(3,j);
			CH->Checked=true;
			CH=NULL;delete CH;
			//mGrid->getCheck(11,j)->Visible=false;
			//mGrid->getCheck(13,j)->Visible=false;
			//posun na další øádek výsledné tabulky
			for(int sl=0;sl<=ColCount-1;sl++)
      {
				mGrid->Cells[sl][j].BottomBorder->Width=2;
			}

			//staré parametry
      mGrid->Cells[17][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->aRD*(1+59.0*aRDunit),2,"..");                      mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clUNLOCKED;
			mGrid->Cells[30][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->roztec*(1+999.0*Runit),2,".."); mGrid->Cells[30][j].Align=mGrid->LEFT;mGrid->Cells[30][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
	 		mGrid->Cells[32][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->Rz*(1+999.0*Rzunit),2,"..");     mGrid->Cells[32][j].Align=mGrid->LEFT;mGrid->Cells[32][j].Font->Color=clOLD;mGrid->Cells[33][j].Align=mGrid->LEFT;mGrid->Cells[33][j].Font->Color=clUNLOCKED;
			mGrid->Cells[34][j].Text=F->d.v.vrat_pohon(i)->Rx;                               mGrid->Cells[34][j].Align=mGrid->LEFT;mGrid->Cells[34][j].Font->Color=clOLD;mGrid->Cells[35][j].Align=mGrid->LEFT;mGrid->Cells[35][j].Font->Color=clUNLOCKED;
    // 	mGrid->Cells[37][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->aRD*(1+59.0*aRDunit),2,"..");                      mGrid->Cells[37][j].Align=mGrid->LEFT;mGrid->Cells[37][j].Font->Color=clOLD;mGrid->Cells[38][j].Align=mGrid->LEFT;mGrid->Cells[38][j].Font->Color=clUNLOCKED;

    //šedivé pozadí pro nepoèítané hodnoty
     mGrid->Cells[15][j].Background->Color=clHiddenValues;    mGrid->Cells[16][j].Background->Color=clHiddenValues;
     mGrid->Cells[19][j].Background->Color=clHiddenValues;    mGrid->Cells[20][j].Background->Color=clHiddenValues;
     mGrid->Cells[21][j].Background->Color=clHiddenValues;    mGrid->Cells[22][j].Background->Color=clHiddenValues;
     mGrid->Cells[23][j].Background->Color=clHiddenValues;    mGrid->Cells[24][j].Background->Color=clHiddenValues;
     mGrid->Cells[25][j].Background->Color=clHiddenValues;    mGrid->Cells[26][j].Background->Color=clHiddenValues;
     mGrid->Cells[27][j].Background->Color=clHiddenValues;    mGrid->Cells[28][j].Background->Color=clHiddenValues;
     mGrid->Cells[29][j].Background->Color=clHiddenValues;
     calculate(j);

			j++;
		}
		else//OBJEKTY S PØIØAZENÝMI POHONY
		{
			//vratí formou ukazatele na pole objekty pøiøazené k danému pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i);
			unsigned long z=0;

			for(;z<O_pocet;z++)
			{
				//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;
				AnsiString rezimZ;
				if(O[z].rezim==0)  rezimZ="S&G";
				if(O[z].rezim==1) {rezimZ="Kontinuální";}
				if(O[z].rezim==2)  rezimZ="Postprocesní";
				//	else rezimZ="-";
				mGrid->Cells[2][j].Text=rezimZ;
				//volby - checkboxy
				mGrid->Cells[3][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);
				//povolení zobrazení checkboxù pro KK a PP režim
				if(O[z].rezim!=0)
				{
					mGrid->Cells[5][j].Type=mGrid->CHECK;mGrid->MergeCells(5,j,6,j);
					mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->MergeCells(7,j,8,j);
					mGrid->Cells[9][j].Type=mGrid->CHECK;mGrid->MergeCells(9,j,10,j);
					mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->MergeCells(11,j,12,j);
					mGrid->Cells[13][j].Type=mGrid->CHECK;mGrid->MergeCells(13,j,14,j);
				}
				else// u SG slouèím pouze buòky, ale checkbox bude nabízen pouze u prvního "výbìrového" sloupce
				{
					mGrid->MergeCells(5,j,6,j);
					mGrid->MergeCells(7,j,8,j);
					mGrid->MergeCells(9,j,10,j);
					mGrid->MergeCells(11,j,12,j);
					mGrid->MergeCells(13,j,14,j);
				}
////
////
				if(O[z].rezim==0) //fixní nastavení checkboxu u SG režimù
				{
					TscGPCheckBox *CH=mGrid->getCheck(3,j);
					CH->Checked=true;
					CH->Enabled=false;
					CH=NULL;delete CH;
				}
				else   // pro ostatní režimy vždy defaultnì zobrazím checkbox zaškrnutý vlevo
				{
					TscGPCheckBox *CH=mGrid->getCheck(3,j);
					CH->Checked=true;
					CH=NULL;delete CH;
				}

				// zobrazení po naètení - pokud je v KK více objektù
				TscGPCheckBox *CH=mGrid->getCheck(3,j);

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 && CH->Checked && O[z].rezim==1)
				{
					//normální barva + checked barva pro 3 -5 checkboxy
					mGrid->getCheck(3,j)->Options->FrameNormalColor=C1;
					mGrid->getCheck(5,j)->Options->FrameNormalColor=C1;
					mGrid->getCheck(3,j)->OptionsChecked->FrameNormalColor=C1;
					mGrid->getCheck(5,j)->OptionsChecked->FrameNormalColor=C1;

					// normální barva + checked barva pro 7-13 checkboxy
					mGrid->getCheck(7,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(9,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(11,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(13,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(7,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(9,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(11,j)->OptionsChecked->FrameNormalColor=C3;
					mGrid->getCheck(13,j)->OptionsChecked->FrameNormalColor=C3;
				}

				// zobrazení po naètení - pokud je v KK max 1 objekt, dovolím vstup do všech sloupcù okamžitì
				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) <= 1 && CH->Checked && O[z].rezim==1)
				{
					TscGPCheckBox *A=mGrid->getCheck(5,j);
					TscGPCheckBox *B=mGrid->getCheck(7,j);
					TscGPCheckBox *C=mGrid->getCheck(9,j);
					TscGPCheckBox *D=mGrid->getCheck(11,j);
					TscGPCheckBox *E=mGrid->getCheck(13,j);
					/*A->Enabled=false*/;B->Enabled=true;C->Enabled=true;D->Enabled=true;E->Enabled=true;
					A=NULL;delete A;B=NULL;delete B;B=NULL;delete B;C=NULL;delete C;D=NULL;delete D;E=NULL;delete E;
				}
				CH=NULL;delete CH;

				if(O[z].pohon!=NULL && O[z].rezim!=0) //vynechání SG režimu (má pøednastavené volby)
				{
					//	ShowMessage(O[z].pohon->Rx);
					double value=F->ms.MyToDouble(O[z].pohon->Rx);
					if(value!=floor(value))
					{
						double dop_Rx=Form1->m.round(F->ms.MyToDouble(O[z].pohon->Rx));
						vypis("Rx: nedovolím editovat, doporuèené Rx: "+ AnsiString(dop_Rx));
						mGrid->getCheck(7,j)->Enabled=false;
						mGrid->getCheck(11,j)->Enabled=false;
						Rx_canEdit=false;
					}
				}

				//parametry objektù
				mGrid->Cells[15][j].Text=F->m.round2double(O[z].CT/(1+59.0*CTunit),2,"..");	 							 	mGrid->Cells[15][j].Align=mGrid->LEFT;	mGrid->Cells[15][j].Font->Color=clOLD;	mGrid->Cells[16][j].Align=mGrid->LEFT; mGrid->Cells[16][j].Font->Color=clUNLOCKED;
		 //		mGrid->Cells[17][j].Text=F->m.round2double(O[z].RD*(1+59.0*RDunit),2,"..");                mGrid->Cells[17][j].Align=mGrid->LEFT;	mGrid->Cells[17][j].Font->Color=clOLD;	mGrid->Cells[18][j].Align=mGrid->LEFT; mGrid->Cells[18][j].Font->Color=clUNLOCKED;
				mGrid->Cells[19][j].Text=F->m.round2double(O[z].delka_dopravniku*(1+999.0*DDunit),2,"..");    mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clOLD;mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clUNLOCKED;
				mGrid->Cells[21][j].Text=F->m.round2double(O[z].kapacita,2,"..");                           mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clOLD;mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clUNLOCKED;
				mGrid->Cells[23][j].Text=F->m.round2double(O[z].pozice,2,"..");                            mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clOLD;mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clUNLOCKED;
				mGrid->Cells[25][j].Text=F->m.round2double(O[z].mezera_jig*(1+999.0*Munit),2,"..");           mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clOLD;mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clUNLOCKED;
				mGrid->Cells[27][j].Text=F->m.round2double(O[z].mezera_podvozek*(1+999.0*Munit),2,"..");  		 	mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
				mGrid->Cells[29][j].Text=AnsiString(O[z].rotace);             	 	mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;

				if(O[z].pohon!=NULL)
				{
					//ShowMessage(O[z].pohon->roztec);
					mGrid->Cells[30][j].Text=F->m.round2double(O[z].pohon->roztec*(1+999.0*Runit),2,"..");                     mGrid->Cells[30][j].Align=mGrid->LEFT;mGrid->Cells[30][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
					mGrid->Cells[32][j].Text=F->m.round2double(O[z].pohon->Rz*(1+999.0*Rzunit),2,"..");                       mGrid->Cells[32][j].Align=mGrid->LEFT;mGrid->Cells[32][j].Font->Color=clOLD;mGrid->Cells[33][j].Align=mGrid->LEFT;mGrid->Cells[33][j].Font->Color=clUNLOCKED;
					mGrid->Cells[34][j].Text=O[z].pohon->Rx;                       mGrid->Cells[34][j].Align=mGrid->LEFT;mGrid->Cells[34][j].Font->Color=clOLD;mGrid->Cells[35][j].Align=mGrid->LEFT;mGrid->Cells[35][j].Font->Color=clUNLOCKED;
        	mGrid->Cells[17][j].Text=F->m.round2double(O[z].pohon->aRD*(1+59.0*aRDunit),2,"..");                      mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clUNLOCKED;

				}

				mGrid->Cells[36][j].Type=mGrid->BUTTON;mGrid->Cells[36][j].Text="...";mGrid->Cells[36][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
				TscGPButton *B=mGrid->createButton(36,j);//vytvoøení buttnu, lépì pøed následujícím cyklem, aby se pozdìji mohl parametrizovat
				/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
				//	B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//padá
				//zajistí pøepoèet daného øádku - nových hodnot
			 	calculate(j);
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slouèení bunìk pohony
      mGrid->MergeCells(17,j-z,17,j-z+O_pocet-1);//RP
      mGrid->MergeCells(18,j-z,18,j-z+O_pocet-1);//RP výpoèet

      mGrid->MergeCells(30,j-z,30,j-z+O_pocet-1);//Rozteè
      mGrid->MergeCells(31,j-z,31,j-z+O_pocet-1);//Rozteè výpoèet
      mGrid->MergeCells(32,j-z,32,j-z+O_pocet-1);//Rz
      mGrid->MergeCells(33,j-z,33,j-z+O_pocet-1);//Rz výpoèet
      mGrid->MergeCells(34,j-z,34,j-z+O_pocet-1);//Rx
      mGrid->MergeCells(35,j-z,35,j-z+O_pocet-1);//Rx výpoèet
      //ShowMessage(j-z);
      for(int sl=0;sl<=ColCount-1;sl++) //silnìjší oddìlení dalšího pohonu
      {
       mGrid->Cells[sl][j-z+O_pocet-1].BottomBorder->Width=2;
      }
			O=NULL;delete O;
		}
	}

	////////rozdìlení sekcí svislým orámováním////////
	for(int i=0;i<=RowCount-1;i++)
	{
		mGrid->Cells[14][i].RightBorder->Width=2;
  	mGrid->Cells[2][i].RightBorder->Width=2;
  }

  for(int r=0;r<=RowCount-1;r++)
  {
    mGrid->Cells[16][r].RightBorder->Width=mGrid->Cells[18][r].RightBorder->Width=mGrid->Cells[20][r].RightBorder->Width=mGrid->Cells[22][r].RightBorder->Width=mGrid->Cells[24][r].RightBorder->Width=mGrid->Cells[26][r].RightBorder->Width=mGrid->Cells[28][r].RightBorder->Width=mGrid->Cells[31][r].RightBorder->Width=mGrid->Cells[33][r].RightBorder->Width=mGrid->Cells[35][r].RightBorder->Width=mGrid->Cells[29][r].RightBorder->Width=2;
  }

	mGrid->Cells[2][0].RightBorder->Color=C1;
	mGrid->Cells[3][0].BottomBorder->Color=mGrid->Cells[4][0].BottomBorder->Color=mGrid->Cells[5][0].BottomBorder->Color=mGrid->Cells[6][0].BottomBorder->Color=C1;
	mGrid->Cells[3][0].BottomBorder->Width=mGrid->Cells[4][0].BottomBorder->Width= mGrid->Cells[5][0].BottomBorder->Width=mGrid->Cells[6][0].BottomBorder->Width=2;

	mGrid->Cells[6][0].RightBorder->Color=C1;
	mGrid->Cells[6][0].RightBorder->Width=2;

	mGrid->Cells[7][0].BottomBorder->Width=	mGrid->Cells[8][0].BottomBorder->Width=mGrid->Cells[9][0].BottomBorder->Width= mGrid->Cells[10][0].BottomBorder->Width=mGrid->Cells[11][0].BottomBorder->Width=mGrid->Cells[12][0].BottomBorder->Width=mGrid->Cells[13][0].BottomBorder->Width=mGrid->Cells[14][0].BottomBorder->Width=2;
	mGrid->Cells[7][0].BottomBorder->Color=	mGrid->Cells[8][0].BottomBorder->Color=mGrid->Cells[9][0].BottomBorder->Color= mGrid->Cells[10][0].BottomBorder->Color=C2;

	mGrid->Cells[10][0].RightBorder->Width=2;
	mGrid->Cells[10][0].RightBorder->Color=C2;

	mGrid->Cells[11][0].BottomBorder->Color=mGrid->Cells[12][0].BottomBorder->Color=mGrid->Cells[13][0].BottomBorder->Color=mGrid->Cells[14][0].BottomBorder->Color=C3;

	mGrid->Cells[14][0].RightBorder->Width=2;
	mGrid->Cells[14][0].RightBorder->Color=C3;

	////////autoresize a pozice formu_gapo, vhodné nakonec,tj. pøed Show//////// NEWR
	////velikost gapo formu a umístìní komponent
	//šíøka
	Width=mGrid->Width+Offset*2+1;
	if(Width<=F->Width)//pokud je užší nebo stejnì jako šíøka hlavního formu
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
	Height=mGrid->Height+Offset*2+rHTMLLabel_InfoText->Height+scGPPanel_hlavicka->Height+11+scGPButton_OK->Height+11+rHTMLLabel_legenda->Height;// + 11 offset okolo tlaèítka
	if(Height<=F->Height)//pokud je kratší než výška hlavní formu
	{
		scScrollBar_vertical->Visible=false;
	}
	else//je delší
	{
		Height=F->Height;
		Width+=scScrollBar_vertical->Width-Offset;//musím ještì rozšíøit form, aby se vešel scrollbar
		scScrollBar_vertical->Visible=true;
		scScrollBar_vertical->Left=Width-scScrollBar_vertical->Width;
		scScrollBar_vertical->Top=scGPPanel_hlavicka->Height;
		scScrollBar_vertical->Height=Height-scGPPanel_hlavicka->Height;
		if(scScrollBar_horizont->Visible)//ošetøení pokud jsou zobrazeny oba
		{
			scScrollBar_vertical->Top+=scScrollBar_horizont->Height;
			scScrollBar_vertical->Height=Height-scScrollBar_horizont->Width-scGPPanel_hlavicka->Height;
		}
		scScrollBar_vertical->Position=0;
	}

	//pozice komponent
	F->m.designButton(scGPButton_OK,F_gapoTT,1,2);
	F->m.designButton(scGPButton_storno,F_gapoTT,2,2);
	//legenda pozice
	rHTMLLabel_InfoText->Top=mGrid->Top+mGrid->Height+1;//+1 kvùli orámování tabulky
	rHTMLLabel_legenda_titulek->Top=rHTMLLabel_InfoText->Top;rHTMLLabel_legenda_titulek->Left=Width-rHTMLLabel_legenda->Width-Offset/2;
	rHTMLLabel_legenda->Top=rHTMLLabel_legenda_titulek->Top+rHTMLLabel_legenda_titulek->Height;rHTMLLabel_legenda->Left=rHTMLLabel_legenda_titulek->Left;
	////pozice gapo formu, pokud je stejnì velký jako hlavní form, tak na 0 pozici, jinak na støed PL formu
	if(Width==F->Width)Left=0;else Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Width/2;
	if(Height==F->Height)Top=0;else Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_parametry_linky->scGPPanel2->Height/2-Height/2;//umístí na polovinu PL formuláøe
	////zobrazení orámování
	zobrazitFrameForm=true;
	////kvùli špatnì fungující funkci otáèení koleèka myši
	liche_otoceni_koleckem_mysi=false; //NEWR

	///uvolnìní stavu
	input_state=FREE;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykreslí tabulku
	if(zobrazitFrameForm)F->m.frameForm(this,clWebOrange,1);//vykreslí orámování okolo formu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TF_gapoTT::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
//PØEPÍNAÈE CHECKBOXÙ - nastaveno pro všechny sloupce, vyjma režimu SG, který má vždy jen jednu pøedem danou volbu
	//NASTAVENÍ, ŽE NELZE ZRUŠIT CHECKED NA AKTUÁLNÌ CHECKED CHECKBOXU
	//ZAJISTÍ, ŽE MÙŽE BÝT ZAKLIKNUT MAX. 1 CHECKBOX NA ØÁDKU  a NELZE UDELAT UNCHECK
  if(Col>=5 &&  Col<=13)
  {
  temp_pocitadlo++;
  //ShowMessage(temp_pocitadlo);
  if(temp_pocitadlo==1)scImage_zamky->Left=mGrid->Left + mGrid->Columns[3].Left + 6;
  if(temp_pocitadlo==2)scImage_zamky->Left=mGrid->Left + mGrid->Columns[3].Left + 6;
  else scImage_zamky->Left=mGrid->Left + mGrid->Columns[3].Left + 4;
  } //workaround ošetøení posunu tabulky
//
//	if(Col>=3  &&  Col<=13  && mGrid->getCheck(Col,Row)->Checked==false)
//	{
//		TscGPCheckBox *CH=mGrid->getCheck(Col,Row);
//		CH->Checked=true;
//		CH=NULL;delete CH;
//	}

	if(Col==3 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col+2,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col+4,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+6,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+8,Row);
		K->Checked=false;

		TscGPCheckBox *L=mGrid->getCheck(Col+10,Row);
		L->Checked=false;
		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;L=NULL;delete L;
	}

	if(Col==5 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-2,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col+2,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+4,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+6,Row);
		K->Checked=false;

		TscGPCheckBox *L=mGrid->getCheck(Col+8,Row);
		L->Checked=false;
		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;L=NULL;delete L;
	}

	if(Col==7 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-4,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-2,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+2,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+4,Row);
		K->Checked=false;

		TscGPCheckBox *L=mGrid->getCheck(Col+6,Row);
		L->Checked=false;
		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;L=NULL;delete L;
	}

	if(Col==9 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-6,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-4,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-2,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+2,Row);
		K->Checked=false;

		TscGPCheckBox *L=mGrid->getCheck(Col+4,Row);
		L->Checked=false;
		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;L=NULL;delete L;
	}

	if(Col==11 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-8,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-6,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-4,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col-2,Row);
		K->Checked=false;

		TscGPCheckBox *L=mGrid->getCheck(Col+2,Row);
		L->Checked=false;
		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;L=NULL;delete L;
	}

	if(Col==13 &&  mGrid->getCheck(Col,Row)->Checked && 	mGrid->Cells[2][Row].Text!="S&G")
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-10,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-8,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-6,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col-4,Row);
		K->Checked=false;

		TscGPCheckBox *L=mGrid->getCheck(Col-2,Row);
		L->Checked=false;
		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;L=NULL;delete L;
	}

//-------------------------------------------------------------------------------------
//ÈÁST VALIDACE OBLASTÍ - povolení/zakázání uložení GAPO
// první oblast   - levá
	if(Col>=3 && Col<=5 && input_state==FREE && mGrid->Cells[2][Row].Text=="Kontinuální" )
	{
		if(mGrid->getCheck(3,Row)->Checked==true ||  mGrid->getCheck(5,Row)->Checked==true)
		{
			// podívám se, zda pohon, který je na øádku, kde došlo ke kliku má více objektù v KK režimu, pokud ano, musím projít všechny
			 if(objekty[Row].pohon!=NULL)
			 {
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=1;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
					 //prùchod celé tabulky
						pocitadlo=0;
						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL && mGrid->Cells[2][i].Text=="Kontinuální")
							{
								if(pohon_n==objekty[i].pohon->n)
								{
									if (mGrid->getCheck(7,i)->Checked==true   ||  mGrid->getCheck(9,i)->Checked==true ||  mGrid->getCheck(11,i)->Checked==true ||  mGrid->getCheck(13,i)->Checked==true)
									{
                   input_state=PROGRAMOVE;
                   pocitadlo++;

                    if(input_state==PROGRAMOVE)
                    {
                      Memo1->Lines->Add("Sloupec: "+AnsiString(Col)+"i: "+AnsiString(i));
										// vypis("Tato varianta nelze uložit, musíte se nacházet ve stejné oblasti výbìru!1");
                     mGrid->getCheck(7,i)->Checked=false;
                     mGrid->getCheck(9,i)->Checked=false;
                     mGrid->getCheck(11,i)->Checked=false;
                     mGrid->getCheck(13,i)->Checked=false;

                     mGrid->getCheck(Col,i)->Checked=true;
                     }
									}
								}
							}
						}
						if(pocitadlo==0) ;//vypis("",false);
					 }
			 }
		}
    input_state=FREE;
	 }
     // druhá oblast - prostøední
		if(Col>=7 && Col<=9 && input_state==FREE && mGrid->Cells[2][Row].Text=="Kontinuální" )
		{
			if(mGrid->getCheck(7,Row)->Checked==true ||  mGrid->getCheck(9,Row)->Checked==true)
			{
		// podívám se, zda pohon, který je na øádku, kde došlo ke kliku má více objektù v KK režimu, pokud ano, musím projít všechny
				if(objekty[Row].pohon!=NULL)
				{
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=2;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
						//prùchod celé tabulky
						pocitadlo=0;
						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL && mGrid->Cells[2][i].Text=="Kontinuální")
							{
								if(pohon_n==objekty[i].pohon->n)
								{
									if(mGrid->getCheck(3,i)->Checked==true   ||  mGrid->getCheck(5,i)->Checked==true ||  mGrid->getCheck(11,i)->Checked==true ||  mGrid->getCheck(13,i)->Checked==true)
									{

										pocitadlo++;
                    input_state=PROGRAMOVE;
									 //	vypis("Tato varianta nelze uložit, musíte se nacházet ve stejné oblasti výbìru!2");
                       if(input_state==PROGRAMOVE)
                        {
                     mGrid->getCheck(3,i)->Checked=false;
                     mGrid->getCheck(5,i)->Checked=false;
                     mGrid->getCheck(11,i)->Checked=false;
                     mGrid->getCheck(13,i)->Checked=false;

                     mGrid->getCheck(Col,i)->Checked=true;
                       }
                   }
								}
							}
						}
						if(pocitadlo==0) ; //	vypis("",false);  //povolím uložení
					 }
				 }
			}
     input_state=FREE;
	 }

	// tøetí oblast - vpravo
	if(Col>=11 && Col<=13 && input_state==FREE  && mGrid->Cells[2][Row].Text=="Kontinuální")
	{
		if(mGrid->getCheck(11,Row)->Checked==true ||  mGrid->getCheck(13,Row)->Checked==true)
		{
		// podívám se, zda pohon, který je na øádku, kde došlo ke kliku má více objektù v KK režimu, pokud ano, musím projít všechny
			if(objekty[Row].pohon!=NULL)
			{
				int pohon_n=objekty[Row].pohon->n;
        indikator_skupin[pohon_n]=3;
				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
				{
					pocitadlo=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
							if(objekty[i].pohon!=NULL && mGrid->Cells[2][i].Text=="Kontinuální")
							{
								if(pohon_n==objekty[i].pohon->n)
								 {
									 if (mGrid->getCheck(3,i)->Checked==true   ||  mGrid->getCheck(5,i)->Checked==true ||  mGrid->getCheck(7,i)->Checked==true ||  mGrid->getCheck(9,i)->Checked==true )
                   pocitadlo++;
                   input_state=PROGRAMOVE;
                    if(input_state==PROGRAMOVE)
                    {
                    //Memo1->Lines->Add("Sloupec: "+AnsiString(Col)+"i: "+AnsiString(i));
                      // vypis("Tato varianta nelze uložit, musíte se nacházet ve stejné oblasti výbìru!3");
                     mGrid->getCheck(3,i)->Checked=false;
                     mGrid->getCheck(5,i)->Checked=false;
                     mGrid->getCheck(7,i)->Checked=false;
                     mGrid->getCheck(9,i)->Checked=false;
                     mGrid->getCheck(Col,i)->Checked=true;
                   }

								 }
							}
					 }   input_state=FREE;
				}
			}
		}

	 }


	//-------------------
	// znovunaètení formuláøe - nastavení Checkboxù
	if(input_state==LOADING && Col>2)
	{
		if(mGrid->Cells[2][Row].Text!="S&G")
		{
			//tøetí resp. první výbìrový sloupec je vždy pøedvybrán na true, zbytek jsou po zobrazení false
			mGrid->getCheck(3,Row)->Checked=true;
			mGrid->getCheck(5,Row)->Checked=false;
			mGrid->getCheck(7,Row)->Checked=false;
			mGrid->getCheck(9,Row)->Checked=false;
			mGrid->getCheck(11,Row)->Checked=false;
			mGrid->getCheck(13,Row)->Checked=false;
		}
	}

	//NEWR
	if(Col==36)//je kliknutu na náhled objektu
	{
		calculate(Row,2);
		scGPButton_OK->Enabled=false;scGPButton_storno->Enabled=false;
		Form_objekt_nahled->zobrazitFrameForm=true;zobrazitFrameForm=false;
		Invalidate();FormPaint(this);//zajistí pøekreslení bez probliku
		Form_objekt_nahled->Left=Left+Width/2-Form_objekt_nahled->Width/2;
		Form_objekt_nahled->Top=Top+Height/2-Form_objekt_nahled->Height/2;
		Form_objekt_nahled->ShowModal();
		scGPButton_OK->Enabled=true;scGPButton_storno->Enabled=true;zobrazitFrameForm=true;
	}
	else//pøekliknutí chechboxu pravdìpodobnì
	{
	 if(input_state==FREE && Col>=3 && Col<=14)
	 {
   		// podívám se, zda pohon, který je na øádku, kde došlo ke kliku má více objektù v KK režimu, pokud ano, musím projít všechny
			if(objekty[Row].pohon!=NULL)
			{
				int pohon_n=objekty[Row].pohon->n;

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n) > 1)
				{

           slouceny_radek=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
							if(objekty[i].pohon!=NULL)
							{
//								if(pohon_n==objekty[i].pohon->n)
//								 {
                   ShowMessage(i);
                   slouceny_radek=8;
               //  } //slouží k odlišení v calculate pro výpoèet, abych vìdìl na který slouèený øádek mám vrátit data
              }
            }

         } else slouceny_radek=0;

      }
    calculate(Row);//zajistí pøepoèet daného øádku

		FormPaint(this);//zajistí pøekreslení bez probliku
		//Invalidate();
	 }
	}
}
//---------------------------------------------------------------------------
void TF_gapoTT::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TF_gapoTT::OnChange(long Tag,unsigned long Col,unsigned long Row)
{


}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::scGPButton_OKClick(TObject *Sender)
{
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		calculate(Row,1);//sice se propoèítává opakovanì, ale kvùli možnému zobrazení dat ve zkrácené formì v tabulce. lepe z ostrých dat

		//aktualizace PO, pokud je spuštìné
		if(F->pom!=NULL && objekty[Row].n<100)//pokud se nejedná o pohon bez pøiøazených objektù
		{//nutno jako samostaný if
			if(F->pom->n==objekty[Row].n)//je spuštìné PO a je tedy nutné zavolat aktualizaci PO
			{
				F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vrátí ostrá data
				F->NPin();
			}
		}
	}
 // F->d.v.PP.TT=Form_parametry_linky->rEditNum_takt->Value*(1+59.0*Form_parametry_linky->Taktunit);
  myModalResult=mrOk;
  Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
  Form_parametry_linky->Button_saveClick(Sender);
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(ModalResult==mrOk)delete[] pohony_zmena;//pokud je stisknuto storno pøi ukonèování, tak se nemaže //NEWR
	delete[] objekty;
  delete[] indikator_skupin;
	Form_objekt_nahled->pom=NULL;delete Form_objekt_nahled->pom;
 // mGrid->Delete();
}
//---------------------------------------------------------------------------
void TF_gapoTT::vypis(UnicodeString text,bool red,bool link)
{
	scGPButton_OK->Enabled=true;
	scGPButton_OK->Caption = "Uložit";
	if (text != "") // zobrazí a vypíše
	{
		rHTMLHint1->ToString()=text;//natežení do hintu zajišuje zobrazení celého textu, nepoužívá se klasický hint
		//prodllužení formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl pøedtím popisek skrytý + kontrola pozice formu

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
		rHTMLLabel_InfoText->Top = scGPButton_OK->Top - 10;
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
UnicodeString TF_gapoTT::calculate(unsigned long Row,short SaveTo)
{
	//instance na PO_math, využívá se stejných výpoètù
	TPO_math pm;

	//input sekce
  //POZOR: pm.RD je vždy aRD, až v sekci case 1 je vypoèítáno skuteèné RD=DD/CT
	pm.TT=Form_TT_kalkulator->rEditNum_takt->Value/(1+59.0*Form_parametry_linky->Taktunit);
	if(objekty[Row].id<100)//pokud id>=100, jedná se o pohon bez pøiøazených objektù
	{
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
	}
	else//pohon bez pøiøazených objektù
	{
		pm.rezim=100;//-1 nejde ani 0,1,2
		pm.CT=0;
		pm.DD=0;
		pm.K=0;
		pm.P=0;
		pm.M=0;
		pm.MJ=0;
		pm.MP=0;
		pm.dJ=0;
		pm.sJ=0;
		pm.dP=0;
		pm.Rotace=0;
		//R-záležitosti   //POZOR: pm.RD je vždy aRD, až v sekci case 1 je vypoèítáno skuteèné RD=DD/CT                                                                                               //vrátí èíslo ve stringgridu øádku dle ID pohonu
		pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].id-100)])/(1+59.0*aRDunit);///musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
    pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].id-100)])/(1+999.0*Runit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
    pm.Rz=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[6][Form_parametry_linky->getROW(objekty[Row].id-100)])/(1+999.0*Rzunit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
   	pm.Rx=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].id-100)]);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
	}
	//R-záležitosti
	if(objekty[Row].pohon!=NULL && pm.rezim!=100)
	{                                                                                                                                         //pøevod
	 	pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+59.0*aRDunit);///musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
	 	pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999.0*Runit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
 	  pm.Rz=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[6][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999.0*Rzunit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
 		pm.Rx=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
	}
	else//pro objekty bez pøiøazeného pohonu
	{
		if(pm.rezim!=100)//pøepisovalo by bez podmínky jinak ob jedno výše uvedené
		{
			pm.RD=objekty[Row].RD;//pozor, mùže být zavádìjící aRD a RD, zde není plnì odlišeno
			pm.Rz=0;
			pm.R=0;
		}
	}

	//optimalizace detekce a uchování volby zaškrtnutého checkboxu, aby se nemuselo vyvolávat znovu
	bool CHECK[6];
	if(pm.rezim==0)//U SG objektu neexistují sloupce 5,7....Checked - volba pouze z jednoho pøednastaveného sloupce
	{
		CHECK[0]=mGrid->getCheck(3,Row)->Checked;//pøedvybraný sloupec u SG režimu
		CHECK[1]=false;
		CHECK[2]=false;
		CHECK[3]=false;
		CHECK[4]=false;
		CHECK[5]=false;
	}
	if(pm.rezim==1 || pm.rezim==2)
	{
		CHECK[0]=mGrid->getCheck(3,Row)->Checked;
		CHECK[1]=mGrid->getCheck(5,Row)->Checked;
		CHECK[2]=mGrid->getCheck(7,Row)->Checked;
		CHECK[3]=mGrid->getCheck(9,Row)->Checked;
		CHECK[4]=mGrid->getCheck(11,Row)->Checked;
		CHECK[5]=mGrid->getCheck(13,Row)->Checked;
	}
	if(pm.rezim==100)//pro objekty bez pøiøazeného pohonu
	{
		CHECK[0]=mGrid->getCheck(3,Row)->Checked;
		CHECK[1]=false;
		CHECK[2]=mGrid->getCheck(7,Row)->Checked;
	 	CHECK[3]=false;
		CHECK[4]=mGrid->getCheck(11,Row)->Checked;
		CHECK[5]=false;
	}
//
	//volání samotného výpoètu dle volby stanovéné pomoci checkboxu
	if(CHECK[0])//mìní se aRD, RD, CT zùstává DD, K, P, Rz, Rx, R, M, jediná varianta, která pøipadá v úvahu pro S&G režim (jiná nejde zaškrtnout/vybrat)
	{
		if(pm.rezim==1 || pm.rezim==100) pm.RD=pm.Rz/pm.TT;//pro všechny režimy
		if(pm.rezim!=100) pm.CT=pm.TT*pm.K;

		mGrid->Cells[20][Row].Font->Color=clLOCKED;//DD
		mGrid->Cells[22][Row].Font->Color=clLOCKED;//K if(mGrid->Cells[21][Row].Text==mGrid->Cells[22][Row].Text && Cheb
		mGrid->Cells[24][Row].Font->Color=clLOCKED;//P
    mGrid->Cells[33][Row].Font->Color=clLOCKED;//Rz
    mGrid->Cells[35][Row].Font->Color=clLOCKED;//Rx
    mGrid->Cells[31][Row].Font->Color=clLOCKED;//R
		mGrid->Cells[26][Row].Font->Color=clLOCKED;//m
		mGrid->Cells[28][Row].Font->Color=clLOCKED;//m

		mGrid->Cells[18][Row].Font->Color=clUNLOCKED;//RD
    mGrid->Cells[16][Row].Font->Color=clUNLOCKED; //CT

	}
	if(CHECK[1])//mìní se aRD, RD, DD, K, P zùstává CT, Rz, Rx, R, M
	{
		mGrid->Cells[16][Row].Font->Color=clLOCKED;//CT
		mGrid->Cells[31][Row].Font->Color=clLOCKED;//R
		mGrid->Cells[33][Row].Font->Color=clLOCKED;//Rz
		mGrid->Cells[35][Row].Font->Color=clLOCKED;//Rx
		mGrid->Cells[26][Row].Font->Color=clLOCKED;//m
		mGrid->Cells[28][Row].Font->Color=clLOCKED;//m

		mGrid->Cells[18][Row].Font->Color=clUNLOCKED;//RD
		mGrid->Cells[20][Row].Font->Color=clUNLOCKED;//DD
		mGrid->Cells[22][Row].Font->Color=clUNLOCKED;//k
		mGrid->Cells[24][Row].Font->Color=clUNLOCKED;//p

    //režim S&G v tomto pøípadì není možná volba

		if(pm.rezim==1)//pouze pro KK režim
		{
			pm.RD=pm.Rz/pm.TT;
			pm.CT_locked=true;
			pm.DD_locked=false;
			pm.input_RD(false);
		}
		if(pm.rezim==2)//pouze pro PP režim
		{
			pm.K=pm.CT/pm.TT;
			pm.DD=pm.Rz/pm.TT*pm.CT; //toto je otázka, zda je to opravdu tøeba???
			pm.P=pm.Pozice();

		}
	}

	if(CHECK[2])//mìní se Rz, Rx, M, DD, P, CT zùstává aRD, RD, R, K
	{
		mGrid->Cells[18][Row].Font->Color=clLOCKED;//RD
		mGrid->Cells[31][Row].Font->Color=clLOCKED;//R
		mGrid->Cells[22][Row].Font->Color=clLOCKED;//K

		mGrid->Cells[33][Row].Font->Color=clUNLOCKED;//Rz
		mGrid->Cells[35][Row].Font->Color=clUNLOCKED;//Rx
		mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[26][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[20][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[16][Row].Font->Color=clUNLOCKED;//ct
		mGrid->Cells[24][Row].Font->Color=clUNLOCKED;//p

		if(pm.rezim==1)//pouze pro KK režim
		{
			pm.Rz=pm.RD*pm.TT;
			pm.Rx=F->m.Rx2(pm.Rz,pm.R);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.DD=pm.RD*pm.TT*pm.K;
			pm.CT=pm.DD/pm.RD;
			pm.P=pm.Pozice();//radìji nechat až za pm.CT
		}
		if(pm.rezim==2)//pouze pro PP režim
		{
			pm.DD=pm.Rz/pm.TT*pm.CT; //toto je otázka, zda je to opravdu tøeba???
			pm.CT=pm.TT*pm.K;
			pm.M=pm.DD/pm.K-pm.UDV();
			pm.P=pm.Pozice();
		}
		if(pm.rezim==100)//pouze pro pohon bez pøiøazených objektù
		{
			pm.Rz=pm.RD*pm.TT;
			pm.Rx=F->m.Rx2(pm.Rz,pm.R);
		}
	}
	if(CHECK[3])//mìní se Rz, Rx, M, K, P zùstává aRD, RD, R, DD, CT
	{
		mGrid->Cells[16][Row].Font->Color=clLOCKED;//CT
		mGrid->Cells[18][Row].Font->Color=clLOCKED;//RD
		mGrid->Cells[31][Row].Font->Color=clLOCKED;//R
		mGrid->Cells[20][Row].Font->Color=clLOCKED;//DD

		mGrid->Cells[33][Row].Font->Color=clUNLOCKED;//Rz
		mGrid->Cells[35][Row].Font->Color=clUNLOCKED;//Rx
		mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[26][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[22][Row].Font->Color=clUNLOCKED;//k
		mGrid->Cells[24][Row].Font->Color=clUNLOCKED;//p

		if(pm.rezim==1)//pouze pro KK režim
		{
			pm.Rz=pm.RD*pm.TT;
			pm.Rx=F->m.Rx2(pm.Rz,pm.R);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.K=pm.DD/pm.RD/pm.TT;
			pm.P=pm.Pozice();
		}
		if(pm.rezim==2)//pouze pro PP režim
		{
			pm.K=pm.CT/pm.TT;
			pm.P=pm.Pozice();
			pm.M=pm.DD/pm.K-pm.UDV();
		}

	}

	if(CHECK[4])//mìní se Rz, R, M, DD, P, CT zùstává aRD, RD, Rx, K
	{
		mGrid->Cells[16][Row].Font->Color=clLOCKED;//CT
		mGrid->Cells[18][Row].Font->Color=clLOCKED;//RD
		mGrid->Cells[31][Row].Font->Color=clLOCKED;//R
		mGrid->Cells[22][Row].Font->Color=clLOCKED;//K
		mGrid->Cells[35][Row].Font->Color=clLOCKED;//Rx

		mGrid->Cells[33][Row].Font->Color=clUNLOCKED;//Rz
		mGrid->Cells[31][Row].Font->Color=clUNLOCKED;//R
		mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[26][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[20][Row].Font->Color=clUNLOCKED;//dd
		mGrid->Cells[24][Row].Font->Color=clUNLOCKED;//p

		if(pm.rezim==1)//pouze pro KK režim
		{
			pm.Rz=pm.RD*pm.TT;
			pm.R=F->m.R(pm.Rz,pm.Rx);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.DD=pm.RD*pm.TT*pm.K;
			pm.CT=pm.DD/pm.RD;
			pm.P=pm.Pozice();
		}
		if(pm.rezim==2)//pouze pro PP režim
		{
			pm.CT=pm.TT*pm.K;
			pm.DD=pm.Rz/pm.TT*pm.CT; //toto je otázka, zda je to opravdu tøeba???
			pm.P=pm.Pozice();
			pm.M=pm.DD/pm.K-pm.UDV();
		}
    if(pm.rezim==100)
		{
			pm.Rz=pm.RD*pm.TT;
			pm.R=F->m.R(pm.Rz,pm.Rx);
    }
	}
	if(CHECK[5])//mìní se Rz, R, M, K, P zùstává aRD, RD, Rx, DD, CT
	{
		mGrid->Cells[16][Row].Font->Color=clLOCKED;//CT
		mGrid->Cells[18][Row].Font->Color=clLOCKED;//RD
		mGrid->Cells[20][Row].Font->Color=clLOCKED;//DD
		mGrid->Cells[35][Row].Font->Color=clLOCKED;//Rx

		mGrid->Cells[33][Row].Font->Color=clUNLOCKED;//Rz
		mGrid->Cells[31][Row].Font->Color=clUNLOCKED;//R
		mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[26][Row].Font->Color=clUNLOCKED;//m
		mGrid->Cells[22][Row].Font->Color=clUNLOCKED;//k
		mGrid->Cells[24][Row].Font->Color=clUNLOCKED;//p

		if(pm.rezim==1)//pouze pro KK režim
		{
			pm.Rz=pm.RD*pm.TT;
			pm.R=F->m.R(pm.Rz,pm.Rx);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.K=pm.DD/pm.RD/pm.TT;
			pm.P=pm.Pozice();
		}
		if(pm.rezim==2)//pouze pro PP režim
		{
			pm.K=pm.CT/pm.TT;
			pm.P=pm.Pozice();
			pm.M=pm.DD/pm.K-pm.UDV();
    }
	}

//  if(objekty[Row].pohon!=NULL)
//    {
//     if(indikator_skupin[objekty[Row].pohon->n]==1) pm.RD=pm.Rz/pm.TT; //výpoèet pm.RD= pro to jak když je kontinuály v prnví v první oblasti
//    }

	//output sekce
	AnsiString T="";
	switch(SaveTo)  //POZOR: pm.RD je vždy aRD, až v sekci case 1 je vypoèítáno skuteèné RD=DD/CT
	{
     case -1://uložení do textu je-li požadováno
		 {
				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
		 }break;

		 case 0://pouze vrátí text do bunìk
		 {
        if(objekty[Row].id<100)//pokud se nejedná o pohon bez pøiøazených objektù
				{
					mGrid->Cells[16][Row].Text = F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
           if(slouceny_radek>1)
          {  //ShowMessage(slouceny_radek);
           	mGrid->Cells[18][slouceny_radek].Text = F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
          } else mGrid->Cells[18][Row].Text = F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
					mGrid->Cells[20][Row].Text = F->m.round2double(pm.DD*(1+999.0*DDunit),2,"..");
					mGrid->Cells[22][Row].Text = F->m.round2double(pm.K,2,"..");
					mGrid->Cells[24][Row].Text = F->m.round2double(pm.P,2,"..");
					mGrid->Cells[26][Row].Text = F->m.round2double(pm.MJ*(1+999.0*Munit),2,"..");
					mGrid->Cells[28][Row].Text = F->m.round2double(pm.MP*(1+999.0*Munit),2,"..");
				}

				if(objekty[Row].pohon!=NULL || objekty[Row].id>100)//objekty s pohonem nebo pohon bez pøiøazených objektù
				{

        if(slouceny_radek>1)
          {  //ShowMessage(slouceny_radek);
          mGrid->Cells[31][slouceny_radek].Text =F->m.round2double(pm.R*(1+999.0*Runit),2,"..");
			    mGrid->Cells[33][slouceny_radek].Text =F->m.round2double(pm.Rz*(1+999.0*Rzunit),2,"..");
          mGrid->Cells[35][slouceny_radek].Text =F->m.round2double(pm.Rx,2,"..");
          mGrid->Cells[18][slouceny_radek].Text =F->m.round2double(pm.RD*(1+59.0*aRDunit),2,"..");
          }
          else
          {
          mGrid->Cells[31][Row].Text =F->m.round2double(pm.R*(1+999.0*Runit),2,"..");
			    mGrid->Cells[33][Row].Text =F->m.round2double(pm.Rz*(1+999.0*Rzunit),2,"..");
          mGrid->Cells[35][Row].Text =F->m.round2double(pm.Rx,2,"..");
          mGrid->Cells[18][Row].Text =F->m.round2double(pm.RD*(1+59.0*aRDunit),2,"..");
          }
				}

		 }break;
		 case 1://uložení do spojáku OBJEKTY - je-li požadováno
		 {
				if(objekty[Row].id<100)//pokud se nejedná o pohon bez pøiøazených objektù
				{
					Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
					if(O->pohon!=NULL)
					{ //POZOR: pm.RD je vždy aRD, až v sekci case 1 je vypoèítáno skuteèné RD=DD/CT
						O->pohon->aRD=pm.RD; F->d.v.vrat_pohon(O->pohon->n)->aRD=pm.RD;
						O->pohon->Rz=F->m.Rz(pm.RD);
						if(CHECK[2] || CHECK[4])O->pohon->Rx     = F->m.Rx2(O->pohon->Rz,pm.R);//zùstává R, mìní se Rx
						if(CHECK[4] || CHECK[6])O->pohon->roztec = F->m.R(O->pohon->Rz,F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]));//zùstává Rx, mìní se R
					}
					O->CT=pm.CT;
					O->RD=pm.DD/pm.CT;//nelze použít pm.RD pøímo, protože u S&G a PP by se RD!=aRD //POZOR: pm.RD je vždy aRD, až v sekci case 1 je vypoèítáno skuteèné RD=DD/CT
					O->delka_dopravniku=pm.DD;
					O->kapacita=pm.K;
					O->pozice=pm.P;
					O->mezera=pm.M;
					O->mezera_jig=pm.MJ;
					O->mezera_podvozek=pm.MP;
					O=NULL;delete O;
				}
				else//pokud se jedná o pohon bez pøiøazených objektù
				{
					Cvektory::TPohon *pohon=F->d.v.vrat_pohon(objekty[Row].id-100);
          pohon->aRD=F->ms.MyToDouble(pm.RD*(1+59.0*aRDunit));
					pohon->roztec=F->ms.MyToDouble(pm.R*(1+999.0*Runit));
				  pohon->Rz=F->ms.MyToDouble(pm.Rz*(1+999.0*Rzunit));//if(Rzunit==0)	pohon->Rz=F->ms.MyToDouble(pm.Rz); else 	pohon->Rz =F->ms.MyToDouble(pm.Rz);
					pohon->Rx=F->ms.MyToDouble(pm.Rx);
					pohon=NULL;delete pohon;
				}
		 }break;
		 case 2://uložení hodnot z ukazatele pro náhled objektu
		 {
				if(pm.rezim<100)//neplatí pro pohony bez pøiøazených objektù
				{
					Form_objekt_nahled->pom=new Cvektory::TObjekt;
					//Form_objekt_nahled->pom->pohon=objekty[Row].pohon;//takto by pøevzal starou rozteè
					Form_objekt_nahled->pom->pohon=new Cvektory::TPohon;
					Form_objekt_nahled->pom->pohon->roztec=pm.R;//ale pøedávám jen do náhledu R, nic víc od pohonu
					Form_objekt_nahled->pom->rezim=objekty[Row].rezim;
					Form_objekt_nahled->pom->CT=pm.CT;
					Form_objekt_nahled->pom->RD=pm.RD;//pozor øešeno pouze kontinuál
					Form_objekt_nahled->pom->delka_dopravniku=pm.DD;
					Form_objekt_nahled->pom->kapacita=pm.K;
					Form_objekt_nahled->pom->pozice=pm.P;
					Form_objekt_nahled->pom->mezera=pm.M;
					Form_objekt_nahled->pom->mezera_jig=pm.MJ;
					Form_objekt_nahled->pom->mezera_podvozek=pm.MP;
				}
				else
				Form_objekt_nahled->pom=NULL;//pro pohony bez pøiøazených objektù
		 }break;
	}
//
	return T;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::scGPButton_stornoClick(TObject *Sender)
{
	Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
  //navrácení pùvodní hodnoty TT, pøi stisku storno na GAPO
  Form_parametry_linky->rEditNum_takt->Value=F->d.v.PP.TT;
  myModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 switch(Key)
 {
	 case 13:// ENTER
	 {
		 if (scGPButton_OK->Enabled)scGPButton_OKClick(Sender);// pokud jsou zároveò splnìny podmínky pro stisk OK
		 else 	MessageBeep(0); // pípnutím upozorní, že nelze
	 }break;
	 case 27:// ESC
	 {
		 Form_parametry_linky->Button_save->Enabled=true;
		 Form_parametry_linky->Button_storno->Enabled=true;
		 F_gapoTT->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
		 F_gapoTT->VisibleChanging();// skryje form, stejné jako visible=false
		 myModalResult=mrCancel;
		 //navrácení pùvodní hodnoty TT, pøi stisku storno na GAPO
		 Form_parametry_linky->rEditNum_takt->Value=F->d.v.PP.TT;
	 }break;
 }
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::scButton_csvClick(TObject *Sender)
{
	AnsiString T="";
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		T+=calculate(Row,-1)+"\n";//sice se propoèítává opakovanì, ale kvùli možnému zobrazení dat ve zkrácené formì v tabulce. lepe z ostrých dat
	}
	F->d.v.Text2CSV(T,Form1->FileName+"_tabulka_gapoTT","Uložit hodnoty do CSV...","*.csv","Soubory formátu CSV (*.csv)|*.csv|Soubory formátu XLS (*.xls)|*.xls");
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::scGPGlyphButton_copyClick(TObject *Sender)
{
	mGrid->CopyCells2Clipboard(0,0,mGrid->ColCount-1,mGrid->RowCount-1);
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormMouseMove(TObject *Sender, TShiftState Shift, int X,int Y)
{
	mGrid->HighlightRowOnMouse(X,Y,(TColor)RGB(240,240,240));
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::scScrollBar_horizontChange(TObject *Sender)
{
	mGrid->Left=F->m.round((Width-mGrid->Width-Offset)*scScrollBar_horizont->Position/100.0);
	//doladit posouvání komponent
	if(scScrollBar_horizont->Position<scScrollBar_horizont->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci musí pøekreslit celé
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::KonecClick(TObject *Sender)
{
  scGPButton_stornoClick(Sender);
	Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::Button1Click(TObject *Sender)
{
Memo1->Lines->Add("Row 4 " + mGrid->Cells[17][4].Text);
Memo1->Lines->Add("Row 5 " + mGrid->Cells[17][5].Text);
Memo1->Lines->Add("Row 6 " + mGrid->Cells[17][6].Text);
Memo1->Lines->Add("Row 7 " + mGrid->Cells[17][7].Text);
Memo1->Lines->Add("Row 8 " + mGrid->Cells[17][8].Text);
Memo1->Lines->Add("Row 9 " + mGrid->Cells[17][9].Text);
Memo1->Lines->Add("Row 10 " + mGrid->Cells[17][10].Text);
Memo1->Lines->Add("Row 11 " + mGrid->Cells[17][11].Text);
Memo1->Lines->Add("Row 12 " + mGrid->Cells[17][12].Text);
Memo1->Lines->Add("Row 13 " + mGrid->Cells[17][13].Text);
}
//---------------------------------------------------------------------------

