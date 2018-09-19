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
	//nastaven� barvy formul��e
	F_gapoTT->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK a STORNO    NEWR
	F->m.designButton(scGPButton_OK,F_gapoTT,1,2);
	F->m.designButton(scGPButton_storno,F_gapoTT,2,2);
	rHTMLLabel_InfoText->Font->Color = (TColor)RGB(43,87,154);//(TColor)RGB(0,128,255);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky

	////definice barev
	//polo�ky
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
  clHiddenValues = (TColor)RGB(250,250,250);//(TColor)RGB(212,212,212);
	//nastaven� barvy or�mov�n� v hlavi�ce tabulky + or�mov�n� checkbox�
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
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=1;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;
  mGrid->Border.Width=2;
	input_state=LOADING;
	Rx_canEdit=true;
  AnsiString jednotky;
  temp_pocitadlo=0;
  unsigned int n_skupin=F->d.v.POHONY->predchozi->n+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
  indikator_skupin=new unsigned int[n_skupin];//dynamick� pole, uchov�vaj�c� indikaci, kter� oblast dan� skupiny byla vybr�na
  for(unsigned int i=0;i<n_skupin;i++)//i je index skupiny resp. id(n) pohonu
  {
    indikator_skupin[i]=1;//ID oblasti 1-3
  }


	//workaround odchyt�v�n� stisku kl�ves
	Edit1->SetFocus();


	////////jednotky////////
	AnsiString T=F->readINI("nastaveni_form_parametry", "CT");
	if(T=="")CTunit=0;else CTunit=T.ToInt();
 //	T=F->readINI("nastaveni_form_parametry","RDt");   - na��t�no z PL akt. nastaven�
	//if(T=="")RDunit=0;else RDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DD");
	if(T=="")DDunit=0;else DDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DM");
	if(T=="")Munit=0; else Munit =T.ToInt();

   if(Form_parametry_linky->Taktunit) jednotky=" [min]"; else  jednotky=" [s]";
  scLabel_titulek->Caption="Glob�ln� aktualizace parametr� pohon� a objekt� z d�vodu zm�ny TT z "+AnsiString(F->d.v.PP.TT)+" na " +AnsiString(Form_TT_kalkulator->rEditNum_takt->Value) + AnsiString(jednotky);

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

	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=37;//pevn� po�et slopc�      //NEWR
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	RowCount+=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false)+F->d.v.vrat_pocet_nepouzivanych_pohonu()+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(true);//P�IDAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	mGrid->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty

	////////pln�n� daty - hlavi�ka////////   NEWR
	mGrid->Cells[0][0].Text="pohon";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnut� tu�n�ho p�sma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
  mGrid->Cells[0][0].BottomBorder->Width=2;

	mGrid->Cells[1][0].Text="objekt";
  mGrid->Cells[1][0].BottomBorder->Width=2;
	mGrid->Cells[2][0].Text="re�im";
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

	if(CTunit)mGrid->Cells[15][0].Text="CT - Technologick� �as [min]";else mGrid->Cells[15][0].Text="CT - Technologick� �as [s]";
	if(RDunit)mGrid->Cells[17][0].Text="RP - Rychlost pohonu [m/min]";else mGrid->Cells[17][0].Text="RP - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[19][0].Text="DD - D�lka objektu [mm]";else mGrid->Cells[19][0].Text="DD - D�lka objektu [m]";
	mGrid->Cells[21][0].Text="K - Kapacita [voz�k�+mezer]";
	mGrid->Cells[23][0].Text="P - Pozice [voz�k�]";
	if(Munit==0)mGrid->Cells[25][0].Text="M - mezera jig [m]"; else mGrid->Cells[25][0].Text="M - mezera jig [mm]";
	if(Munit==0)mGrid->Cells[27][0].Text="M - mezera voz�k [m]"; else mGrid->Cells[27][0].Text="M - mezera voz�k [mm]";
	mGrid->Cells[29][0].Text="Rotace";
	if(Runit==0)mGrid->Cells[30][0].Text="R - rozte� [m]"; else mGrid->Cells[30][0].Text="R - rozte� [mm]";
	if(Rzunit==0)mGrid->Cells[32][0].Text="Rz - akt. palce - rozestup [m]"; else mGrid->Cells[32][0].Text="Rz - akt. palce - rozestup [mm]"; //p�ep�n�n� jednotek dod�lat
	mGrid->Cells[34][0].Text="Rx - ka�dy n-t� palec";

	mGrid->Cells[36][0].Text="n�hled";
 // if(aRDunit)mGrid->Cells[37][0].Text="aRD - akt. rychlost pohonu [m/min]";else mGrid->Cells[17][0].Text="aRD -  akt. rychlost pohonu [m/s]";

	////////p�i�ad� cel� oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky////////
	mGrid->SetCells(mGrid->Cells[0][0],15,0,ColCount-1,0);//pro prvn� ��dek   sloupce 15+
	mGrid->SetCells(mGrid->Cells[0][0],1,0,3,0);//pro prvn� ��dek   sloupce 1-3

	//p�i�azen� jin�ho fontu, nem�n�m polo�k�m - NEWR
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

	//manualfit v��ky 0-t�ho ��dku (zat�m nen� p�ipravena metoda)
	unsigned short n=37;if(RDunit)n=17;//��slo sloupce s nejdel��m textem hlavi�ky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[17][0].Text)+mGrid->Cells[17][0].BottomMargin+mGrid->Cells[17][0].BottomBorder->Width/2+mGrid->Cells[17][0].TopMargin+mGrid->Cells[17][0].TopBorder->Width/2;
	//manualfit ���ky sloupc� mimo prvn�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=100;mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=mGrid->Columns[12].Width=mGrid->Columns[13].Width=mGrid->Columns[14].Width=23;//ostatn� n�sleduj�c� sloupce zat�m default ���ka
	mGrid->Columns[2].Width=120;

	//nastaven� velikosti nult�ho sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
 	mGrid->SetColumnAutoFit(0);
	//workaround v��e uveden�ho, proto�e nefunguje zcela spr�vn� - taky nejede
//	mGrid->Columns[0].Width=100;
	//mGrid->SetColumnAutoFit();

	//slou�en� bun�k hlavi�ky PO  - vhodn� za SetColumnAutoFit um�st�t - NEWR
	mGrid->MergeCells(15,0,16,0);mGrid->MergeCells(17,0,18,0);mGrid->MergeCells(19,0,20,0);
	mGrid->MergeCells(21,0,22,0);mGrid->MergeCells(23,0,24,0);mGrid->MergeCells(25,0,26,0);mGrid->MergeCells(27,0,28,0);
	mGrid->MergeCells(30,0,31,0);mGrid->MergeCells(32,0,33,0);mGrid->MergeCells(34,0,35,0);//mGrid->MergeCells(37,0,38,0);
	////////jednoliv� ��dky////////
	unsigned long j=1;//��slo aktu�ln� zpracov�van�ho ��dku, mus� za��nat 1, 0 - je hlavi�ka
	////pr�chod v�emi objekty bez p�i�azen�ch pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
	for(unsigned long i=1;i<=On_pocet;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		////OBJEKTY BEZ P�I�AZEN�HO POHONU
		//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
		objekty[j]=On[i];

  	//pohony
		mGrid->Cells[0][j].Text="nep�i�azen";
		//objekty
		mGrid->Cells[1][j].Text=On[i].short_name;
		//re�im
		AnsiString rezimJ;
		if(On[i].rezim==0) rezimJ="S&G";
		if(On[i].rezim==1) rezimJ="Kontinu�ln�";
		if(On[i].rezim==2) rezimJ="Postprocesn�";
		mGrid->Cells[2][j].Text=rezimJ;

		//volby - checkboxy
		mGrid->Cells[3][j].Type=mGrid->CHECK;mGrid->MergeCells(3,j,4,j);

		if(On[i].rezim!=0)//povolen� zobrazen� checkbox� pro KK a PP re�im
		{
			mGrid->Cells[5][j].Type=mGrid->CHECK;mGrid->MergeCells(5,j,6,j);
			mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->MergeCells(7,j,8,j);
			mGrid->Cells[9][j].Type=mGrid->CHECK;mGrid->MergeCells(9,j,10,j);
			mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->MergeCells(11,j,12,j);
			mGrid->Cells[13][j].Type=mGrid->CHECK;mGrid->MergeCells(13,j,14,j);
		}
		else// u SG slou��m pouze bu�ky, ale checkbox bude nab�zen pouze u prvn�ho "v�b�rov�ho" sloupce
		{
			mGrid->MergeCells(5,j,6,j);
			mGrid->MergeCells(7,j,8,j);
			mGrid->MergeCells(9,j,10,j);
			mGrid->MergeCells(11,j,12,j);
			mGrid->MergeCells(13,j,14,j);
		}

		//parametry objekt�  // cell 17 - p�vodn� pou��v�na pro RD, nyn� je v n� zobrazov�no aRD
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
		mGrid->Cells[36][j].Type=mGrid->BUTTON;mGrid->Cells[36][j].Text="...";mGrid->Cells[36][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
		TscGPButton *B=mGrid->createButton(36,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
		/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		//B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//pad�
		//checkboxy
		if(On[i].rezim==0) //fixn� nastaven� checkboxu u SG re�im�
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
		//zajist� p�epo�et dan�ho ��dku
	 	calculate(j);
    for(int sl=0;sl<=ColCount-1;sl++) //odd�len� pohon� siln�j�� �arou
    {
    mGrid->Cells[sl][j].BottomBorder->Width=2;
    }
		//posun na dal�� ��dek v�sledn� tabulky
		j++;
	}
	On=NULL;delete On;
	if(On_pocet>0)mGrid->MergeCells(0,1,0,On_pocet);//slou�en� bun�k objekty bez pohonu

	////pr�chod v�emi pohony
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		unsigned long O_pocet=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
		if(O_pocet==0)//tzn. pohonu nen� p�i�azen ��dn� objekt
		{
			//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
			objekty[j].id=100+i;//p�i�ad� id 100+��slo pohonu, ne�lo napsat p��mo NULL, nutno po�e�it p�i pr�chodu cyklem n�sledn�ho vypisov�n� objekty[x]
			//pohony
			mGrid->Cells[0][j].Text=F->d.v.vrat_pohon(i)->name;
			//objekty
			mGrid->Cells[1][j].Text="nep�i�azen";
			//volby - checkboxy
			mGrid->Cells[3][j].Type=mGrid->CHECK;
			mGrid->Cells[5][j].Type=mGrid->CHECK;
			//mGrid->MergeCells(1,j,2,j);
			mGrid->MergeCells(3,j,4,j);
			mGrid->MergeCells(5,j,6,j);//slou�en� sloupc�
			mGrid->MergeCells(3,j,5,j);
			//mGrid->Cells[3][j].Align=mGrid->RIGHT;
			mGrid->Cells[7][j].Type=mGrid->CHECK;
			mGrid->Cells[9][j].Type=mGrid->CHECK;
			mGrid->MergeCells(7,j,8,j);
			mGrid->MergeCells(9,j,10,j);//slou�en� sloupc�
      mGrid->MergeCells(7,j,9,j);//slou�en� sloupc�
			mGrid->Cells[11][j].Type=mGrid->CHECK;
			mGrid->Cells[13][j].Type=mGrid->CHECK;
			mGrid->MergeCells(11,j,12,j);
			mGrid->MergeCells(13,j,14,j);//slou�en� sloupc�
      mGrid->MergeCells(11,j,13,j);//slou�en� sloupc�
			TscGPCheckBox *CH=mGrid->getCheck(3,j);
			CH->Checked=true;
			CH=NULL;delete CH;
			//mGrid->getCheck(11,j)->Visible=false;
			//mGrid->getCheck(13,j)->Visible=false;
			//posun na dal�� ��dek v�sledn� tabulky
			for(int sl=0;sl<=ColCount-1;sl++)
      {
				mGrid->Cells[sl][j].BottomBorder->Width=2;
			}

			//star� parametry
      mGrid->Cells[17][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->aRD*(1+59.0*aRDunit),2,"..");                      mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clUNLOCKED;
			mGrid->Cells[30][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->roztec*(1+999.0*Runit),2,".."); mGrid->Cells[30][j].Align=mGrid->LEFT;mGrid->Cells[30][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
	 		mGrid->Cells[32][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->Rz*(1+999.0*Rzunit),2,"..");     mGrid->Cells[32][j].Align=mGrid->LEFT;mGrid->Cells[32][j].Font->Color=clOLD;mGrid->Cells[33][j].Align=mGrid->LEFT;mGrid->Cells[33][j].Font->Color=clUNLOCKED;
			mGrid->Cells[34][j].Text=F->d.v.vrat_pohon(i)->Rx;                               mGrid->Cells[34][j].Align=mGrid->LEFT;mGrid->Cells[34][j].Font->Color=clOLD;mGrid->Cells[35][j].Align=mGrid->LEFT;mGrid->Cells[35][j].Font->Color=clUNLOCKED;
    // 	mGrid->Cells[37][j].Text=F->m.round2double(F->d.v.vrat_pohon(i)->aRD*(1+59.0*aRDunit),2,"..");                      mGrid->Cells[37][j].Align=mGrid->LEFT;mGrid->Cells[37][j].Font->Color=clOLD;mGrid->Cells[38][j].Align=mGrid->LEFT;mGrid->Cells[38][j].Font->Color=clUNLOCKED;

    //�ediv� pozad� pro nepo��tan� hodnoty
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
		else//OBJEKTY S P�I�AZEN�MI POHONY
		{
			//vrat� formou ukazatele na pole objekty p�i�azen� k dan�mu pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i);
			unsigned long z=0;

			for(;z<O_pocet;z++)
			{
				//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;
				AnsiString rezimZ;
				if(O[z].rezim==0)  rezimZ="S&G";
				if(O[z].rezim==1) {rezimZ="Kontinu�ln�";}
				if(O[z].rezim==2)  rezimZ="Postprocesn�";
				//	else rezimZ="-";
				mGrid->Cells[2][j].Text=rezimZ;
				//volby - checkboxy
				mGrid->Cells[3][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);
				//povolen� zobrazen� checkbox� pro KK a PP re�im
				if(O[z].rezim!=0)
				{
					mGrid->Cells[5][j].Type=mGrid->CHECK;mGrid->MergeCells(5,j,6,j);
					mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->MergeCells(7,j,8,j);
					mGrid->Cells[9][j].Type=mGrid->CHECK;mGrid->MergeCells(9,j,10,j);
					mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->MergeCells(11,j,12,j);
					mGrid->Cells[13][j].Type=mGrid->CHECK;mGrid->MergeCells(13,j,14,j);
				}
				else// u SG slou��m pouze bu�ky, ale checkbox bude nab�zen pouze u prvn�ho "v�b�rov�ho" sloupce
				{
					mGrid->MergeCells(5,j,6,j);
					mGrid->MergeCells(7,j,8,j);
					mGrid->MergeCells(9,j,10,j);
					mGrid->MergeCells(11,j,12,j);
					mGrid->MergeCells(13,j,14,j);
				}
////
////
				if(O[z].rezim==0) //fixn� nastaven� checkboxu u SG re�im�
				{
					TscGPCheckBox *CH=mGrid->getCheck(3,j);
					CH->Checked=true;
					CH->Enabled=false;
					CH=NULL;delete CH;
				}
				else   // pro ostatn� re�imy v�dy defaultn� zobraz�m checkbox za�krnut� vlevo
				{
					TscGPCheckBox *CH=mGrid->getCheck(3,j);
					CH->Checked=true;
					CH=NULL;delete CH;
				}

				// zobrazen� po na�ten� - pokud je v KK v�ce objekt�
				TscGPCheckBox *CH=mGrid->getCheck(3,j);

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 && CH->Checked && O[z].rezim==1)
				{
					//norm�ln� barva + checked barva pro 3 -5 checkboxy
					mGrid->getCheck(3,j)->Options->FrameNormalColor=C1;
					mGrid->getCheck(5,j)->Options->FrameNormalColor=C1;
					mGrid->getCheck(3,j)->OptionsChecked->FrameNormalColor=C1;
					mGrid->getCheck(5,j)->OptionsChecked->FrameNormalColor=C1;

					// norm�ln� barva + checked barva pro 7-13 checkboxy
					mGrid->getCheck(7,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(9,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(11,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(13,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(7,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(9,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(11,j)->OptionsChecked->FrameNormalColor=C3;
					mGrid->getCheck(13,j)->OptionsChecked->FrameNormalColor=C3;
				}

				// zobrazen� po na�ten� - pokud je v KK max 1 objekt, dovol�m vstup do v�ech sloupc� okam�it�
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

				if(O[z].pohon!=NULL && O[z].rezim!=0) //vynech�n� SG re�imu (m� p�ednastaven� volby)
				{
					//	ShowMessage(O[z].pohon->Rx);
					double value=F->ms.MyToDouble(O[z].pohon->Rx);
					if(value!=floor(value))
					{
						double dop_Rx=Form1->m.round(F->ms.MyToDouble(O[z].pohon->Rx));
						vypis("Rx: nedovol�m editovat, doporu�en� Rx: "+ AnsiString(dop_Rx));
						mGrid->getCheck(7,j)->Enabled=false;
						mGrid->getCheck(11,j)->Enabled=false;
						Rx_canEdit=false;
					}
				}

				//parametry objekt�
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

				mGrid->Cells[36][j].Type=mGrid->BUTTON;mGrid->Cells[36][j].Text="...";mGrid->Cells[36][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
				TscGPButton *B=mGrid->createButton(36,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
				/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
				//	B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//pad�
				//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot
			 	calculate(j);
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slou�en� bun�k pohony
      mGrid->MergeCells(17,j-z,17,j-z+O_pocet-1);//RP
      mGrid->MergeCells(18,j-z,18,j-z+O_pocet-1);//RP v�po�et

      mGrid->MergeCells(30,j-z,30,j-z+O_pocet-1);//Rozte�
      mGrid->MergeCells(31,j-z,31,j-z+O_pocet-1);//Rozte� v�po�et
      mGrid->MergeCells(32,j-z,32,j-z+O_pocet-1);//Rz
      mGrid->MergeCells(33,j-z,33,j-z+O_pocet-1);//Rz v�po�et
      mGrid->MergeCells(34,j-z,34,j-z+O_pocet-1);//Rx
      mGrid->MergeCells(35,j-z,35,j-z+O_pocet-1);//Rx v�po�et
      //ShowMessage(j-z);
      for(int sl=0;sl<=ColCount-1;sl++) //siln�j�� odd�len� dal��ho pohonu
      {
       mGrid->Cells[sl][j-z+O_pocet-1].BottomBorder->Width=2;
      }
			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m or�mov�n�m////////
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

	////////autoresize a pozice formu_gapo, vhodn� nakonec,tj. p�ed Show//////// NEWR
	////velikost gapo formu a um�st�n� komponent
	//���ka
	Width=mGrid->Width+Offset*2+1;
	if(Width<=F->Width)//pokud je u��� nebo stejn� jako ���ka hlavn�ho formu
	{
		scScrollBar_horizont->Visible=false;
	}
	else//je �ir��
	{
		Width=F->Width;
		scScrollBar_horizont->Visible=true;
		scScrollBar_horizont->Left=0;
		scScrollBar_horizont->Top=0+scGPPanel_hlavicka->Height;
		scScrollBar_horizont->Width=Width;
		scScrollBar_horizont->Position=0;
	}
	//v��ka
	Height=mGrid->Height+Offset*2+rHTMLLabel_InfoText->Height+scGPPanel_hlavicka->Height+11+scGPButton_OK->Height+11+rHTMLLabel_legenda->Height;// + 11 offset okolo tla��tka
	if(Height<=F->Height)//pokud je krat�� ne� v��ka hlavn� formu
	{
		scScrollBar_vertical->Visible=false;
	}
	else//je del��
	{
		Height=F->Height;
		Width+=scScrollBar_vertical->Width-Offset;//mus�m je�t� roz���it form, aby se ve�el scrollbar
		scScrollBar_vertical->Visible=true;
		scScrollBar_vertical->Left=Width-scScrollBar_vertical->Width;
		scScrollBar_vertical->Top=scGPPanel_hlavicka->Height;
		scScrollBar_vertical->Height=Height-scGPPanel_hlavicka->Height;
		if(scScrollBar_horizont->Visible)//o�et�en� pokud jsou zobrazeny oba
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
	rHTMLLabel_InfoText->Top=mGrid->Top+mGrid->Height+1;//+1 kv�li or�mov�n� tabulky
	rHTMLLabel_legenda_titulek->Top=rHTMLLabel_InfoText->Top;rHTMLLabel_legenda_titulek->Left=Width-rHTMLLabel_legenda->Width-Offset/2;
	rHTMLLabel_legenda->Top=rHTMLLabel_legenda_titulek->Top+rHTMLLabel_legenda_titulek->Height;rHTMLLabel_legenda->Left=rHTMLLabel_legenda_titulek->Left;
	////pozice gapo formu, pokud je stejn� velk� jako hlavn� form, tak na 0 pozici, jinak na st�ed PL formu
	if(Width==F->Width)Left=0;else Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Width/2;
	if(Height==F->Height)Top=0;else Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_parametry_linky->scGPPanel2->Height/2-Height/2;//um�st� na polovinu PL formul��e
	////zobrazen� or�mov�n�
	zobrazitFrameForm=true;
	////kv�li �patn� funguj�c� funkci ot��en� kole�ka my�i
	liche_otoceni_koleckem_mysi=false; //NEWR

	///uvoln�n� stavu
	input_state=FREE;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykresl� tabulku
	if(zobrazitFrameForm)F->m.frameForm(this,clWebOrange,1);//vykresl� or�mov�n� okolo formu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TF_gapoTT::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
//P�EP�NA�E CHECKBOX� - nastaveno pro v�echny sloupce, vyjma re�imu SG, kter� m� v�dy jen jednu p�edem danou volbu
	//NASTAVEN�, �E NELZE ZRU�IT CHECKED NA AKTU�LN� CHECKED CHECKBOXU
	//ZAJIST�, �E MَE B�T ZAKLIKNUT MAX. 1 CHECKBOX NA ��DKU  a NELZE UDELAT UNCHECK
  if(Col>=5 &&  Col<=13)
  {
  temp_pocitadlo++;
  //ShowMessage(temp_pocitadlo);
  if(temp_pocitadlo==1)scImage_zamky->Left=mGrid->Left + mGrid->Columns[3].Left + 6;
  if(temp_pocitadlo==2)scImage_zamky->Left=mGrid->Left + mGrid->Columns[3].Left + 6;
  else scImage_zamky->Left=mGrid->Left + mGrid->Columns[3].Left + 4;
  } //workaround o�et�en� posunu tabulky
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
//��ST VALIDACE OBLAST� - povolen�/zak�z�n� ulo�en� GAPO
// prvn� oblast   - lev�
	if(Col>=3 && Col<=5 && input_state==FREE && mGrid->Cells[2][Row].Text=="Kontinu�ln�" )
	{
		if(mGrid->getCheck(3,Row)->Checked==true ||  mGrid->getCheck(5,Row)->Checked==true)
		{
			// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
			 if(objekty[Row].pohon!=NULL)
			 {
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=1;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
					 //pr�chod cel� tabulky
						pocitadlo=0;
						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL && mGrid->Cells[2][i].Text=="Kontinu�ln�")
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
										// vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!1");
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
     // druh� oblast - prost�edn�
		if(Col>=7 && Col<=9 && input_state==FREE && mGrid->Cells[2][Row].Text=="Kontinu�ln�" )
		{
			if(mGrid->getCheck(7,Row)->Checked==true ||  mGrid->getCheck(9,Row)->Checked==true)
			{
		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
				if(objekty[Row].pohon!=NULL)
				{
					int pohon_n=objekty[Row].pohon->n;
          indikator_skupin[pohon_n]=2;
					if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
					{
						//pr�chod cel� tabulky
						pocitadlo=0;
						for(int i=1;i<=mGrid->RowCount-1;i++)
						{
							if(objekty[i].pohon!=NULL && mGrid->Cells[2][i].Text=="Kontinu�ln�")
							{
								if(pohon_n==objekty[i].pohon->n)
								{
									if(mGrid->getCheck(3,i)->Checked==true   ||  mGrid->getCheck(5,i)->Checked==true ||  mGrid->getCheck(11,i)->Checked==true ||  mGrid->getCheck(13,i)->Checked==true)
									{

										pocitadlo++;
                    input_state=PROGRAMOVE;
									 //	vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!2");
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
						if(pocitadlo==0) ; //	vypis("",false);  //povol�m ulo�en�
					 }
				 }
			}
     input_state=FREE;
	 }

	// t�et� oblast - vpravo
	if(Col>=11 && Col<=13 && input_state==FREE  && mGrid->Cells[2][Row].Text=="Kontinu�ln�")
	{
		if(mGrid->getCheck(11,Row)->Checked==true ||  mGrid->getCheck(13,Row)->Checked==true)
		{
		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
			if(objekty[Row].pohon!=NULL)
			{
				int pohon_n=objekty[Row].pohon->n;
        indikator_skupin[pohon_n]=3;
				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
				{
					pocitadlo=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
							if(objekty[i].pohon!=NULL && mGrid->Cells[2][i].Text=="Kontinu�ln�")
							{
								if(pohon_n==objekty[i].pohon->n)
								 {
									 if (mGrid->getCheck(3,i)->Checked==true   ||  mGrid->getCheck(5,i)->Checked==true ||  mGrid->getCheck(7,i)->Checked==true ||  mGrid->getCheck(9,i)->Checked==true )
                   pocitadlo++;
                   input_state=PROGRAMOVE;
                    if(input_state==PROGRAMOVE)
                    {
                    //Memo1->Lines->Add("Sloupec: "+AnsiString(Col)+"i: "+AnsiString(i));
                      // vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!3");
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
	// znovuna�ten� formul��e - nastaven� Checkbox�
	if(input_state==LOADING && Col>2)
	{
		if(mGrid->Cells[2][Row].Text!="S&G")
		{
			//t�et� resp. prvn� v�b�rov� sloupec je v�dy p�edvybr�n na true, zbytek jsou po zobrazen� false
			mGrid->getCheck(3,Row)->Checked=true;
			mGrid->getCheck(5,Row)->Checked=false;
			mGrid->getCheck(7,Row)->Checked=false;
			mGrid->getCheck(9,Row)->Checked=false;
			mGrid->getCheck(11,Row)->Checked=false;
			mGrid->getCheck(13,Row)->Checked=false;
		}
	}

	//NEWR
	if(Col==36)//je kliknutu na n�hled objektu
	{
		calculate(Row,2);
		scGPButton_OK->Enabled=false;scGPButton_storno->Enabled=false;
		Form_objekt_nahled->zobrazitFrameForm=true;zobrazitFrameForm=false;
		Invalidate();FormPaint(this);//zajist� p�ekreslen� bez probliku
		Form_objekt_nahled->Left=Left+Width/2-Form_objekt_nahled->Width/2;
		Form_objekt_nahled->Top=Top+Height/2-Form_objekt_nahled->Height/2;
		Form_objekt_nahled->ShowModal();
		scGPButton_OK->Enabled=true;scGPButton_storno->Enabled=true;zobrazitFrameForm=true;
	}
	else//p�ekliknut� chechboxu pravd�podobn�
	{
	 if(input_state==FREE && Col>=3 && Col<=14)
	 {
   		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
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
               //  } //slou�� k odli�en� v calculate pro v�po�et, abych v�d�l na kter� slou�en� ��dek m�m vr�tit data
              }
            }

         } else slouceny_radek=0;

      }
    calculate(Row);//zajist� p�epo�et dan�ho ��dku

		FormPaint(this);//zajist� p�ekreslen� bez probliku
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
		calculate(Row,1);//sice se propo��t�v� opakovan�, ale kv�li mo�n�mu zobrazen� dat ve zkr�cen� form� v tabulce. lepe z ostr�ch dat

		//aktualizace PO, pokud je spu�t�n�
		if(F->pom!=NULL && objekty[Row].n<100)//pokud se nejedn� o pohon bez p�i�azen�ch objekt�
		{//nutno jako samostan� if
			if(F->pom->n==objekty[Row].n)//je spu�t�n� PO a je tedy nutn� zavolat aktualizaci PO
			{
				F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vr�t� ostr� data
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
	if(ModalResult==mrOk)delete[] pohony_zmena;//pokud je stisknuto storno p�i ukon�ov�n�, tak se nema�e //NEWR
	delete[] objekty;
  delete[] indikator_skupin;
	Form_objekt_nahled->pom=NULL;delete Form_objekt_nahled->pom;
 // mGrid->Delete();
}
//---------------------------------------------------------------------------
void TF_gapoTT::vypis(UnicodeString text,bool red,bool link)
{
	scGPButton_OK->Enabled=true;
	scGPButton_OK->Caption = "Ulo�it";
	if (text != "") // zobraz� a vyp�e
	{
		rHTMLHint1->ToString()=text;//nate�en� do hintu zaji��uje zobrazen� cel�ho textu, nepou��v� se klasick� hint
		//prodllu�en� formu if(!rHTMLLabel_InfoText->Visible){Height+=(40+19);position();}pouze pokud byl p�edt�m popisek skryt� + kontrola pozice formu

		if(link)rHTMLLabel_InfoText->Font->Style = TFontStyles()<< fsUnderline;//zapnut� podtr�en�ho p�sma
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
		//zkr�cen� formu if(rHTMLLabel_InfoText->Visible)Height-=(40+19);
		rHTMLLabel_InfoText->Visible = false;
	}
}
//---------------------------------------------------------------------------
UnicodeString TF_gapoTT::calculate(unsigned long Row,short SaveTo)
{
	//instance na PO_math, vyu��v� se stejn�ch v�po�t�
	TPO_math pm;

	//input sekce
  //POZOR: pm.RD je v�dy aRD, a� v sekci case 1 je vypo��t�no skute�n� RD=DD/CT
	pm.TT=Form_TT_kalkulator->rEditNum_takt->Value/(1+59.0*Form_parametry_linky->Taktunit);
	if(objekty[Row].id<100)//pokud id>=100, jedn� se o pohon bez p�i�azen�ch objekt�
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
	else//pohon bez p�i�azen�ch objekt�
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
		//R-z�le�itosti   //POZOR: pm.RD je v�dy aRD, a� v sekci case 1 je vypo��t�no skute�n� RD=DD/CT                                                                                               //vr�t� ��slo ve stringgridu ��dku dle ID pohonu
		pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].id-100)])/(1+59.0*aRDunit);///mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
    pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].id-100)])/(1+999.0*Runit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
    pm.Rz=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[6][Form_parametry_linky->getROW(objekty[Row].id-100)])/(1+999.0*Rzunit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
   	pm.Rx=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].id-100)]);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	}
	//R-z�le�itosti
	if(objekty[Row].pohon!=NULL && pm.rezim!=100)
	{                                                                                                                                         //p�evod
	 	pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+59.0*aRDunit);///mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	 	pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999.0*Runit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
 	  pm.Rz=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[6][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999.0*Rzunit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
 		pm.Rx=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	}
	else//pro objekty bez p�i�azen�ho pohonu
	{
		if(pm.rezim!=100)//p�episovalo by bez podm�nky jinak ob jedno v��e uveden�
		{
			pm.RD=objekty[Row].RD;//pozor, m��e b�t zav�d�j�c� aRD a RD, zde nen� pln� odli�eno
			pm.Rz=0;
			pm.R=0;
		}
	}

	//optimalizace detekce a uchov�n� volby za�krtnut�ho checkboxu, aby se nemuselo vyvol�vat znovu
	bool CHECK[6];
	if(pm.rezim==0)//U SG objektu neexistuj� sloupce 5,7....Checked - volba pouze z jednoho p�ednastaven�ho sloupce
	{
		CHECK[0]=mGrid->getCheck(3,Row)->Checked;//p�edvybran� sloupec u SG re�imu
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
	if(pm.rezim==100)//pro objekty bez p�i�azen�ho pohonu
	{
		CHECK[0]=mGrid->getCheck(3,Row)->Checked;
		CHECK[1]=false;
		CHECK[2]=mGrid->getCheck(7,Row)->Checked;
	 	CHECK[3]=false;
		CHECK[4]=mGrid->getCheck(11,Row)->Checked;
		CHECK[5]=false;
	}
//
	//vol�n� samotn�ho v�po�tu dle volby stanov�n� pomoci checkboxu
	if(CHECK[0])//m�n� se aRD, RD, CT z�st�v� DD, K, P, Rz, Rx, R, M, jedin� varianta, kter� p�ipad� v �vahu pro S&G re�im (jin� nejde za�krtnout/vybrat)
	{
		if(pm.rezim==1 || pm.rezim==100) pm.RD=pm.Rz/pm.TT;//pro v�echny re�imy
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
	if(CHECK[1])//m�n� se aRD, RD, DD, K, P z�st�v� CT, Rz, Rx, R, M
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

    //re�im S&G v tomto p��pad� nen� mo�n� volba

		if(pm.rezim==1)//pouze pro KK re�im
		{
			pm.RD=pm.Rz/pm.TT;
			pm.CT_locked=true;
			pm.DD_locked=false;
			pm.input_RD(false);
		}
		if(pm.rezim==2)//pouze pro PP re�im
		{
			pm.K=pm.CT/pm.TT;
			pm.DD=pm.Rz/pm.TT*pm.CT; //toto je ot�zka, zda je to opravdu t�eba???
			pm.P=pm.Pozice();

		}
	}

	if(CHECK[2])//m�n� se Rz, Rx, M, DD, P, CT z�st�v� aRD, RD, R, K
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

		if(pm.rezim==1)//pouze pro KK re�im
		{
			pm.Rz=pm.RD*pm.TT;
			pm.Rx=F->m.Rx2(pm.Rz,pm.R);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.DD=pm.RD*pm.TT*pm.K;
			pm.CT=pm.DD/pm.RD;
			pm.P=pm.Pozice();//rad�ji nechat a� za pm.CT
		}
		if(pm.rezim==2)//pouze pro PP re�im
		{
			pm.DD=pm.Rz/pm.TT*pm.CT; //toto je ot�zka, zda je to opravdu t�eba???
			pm.CT=pm.TT*pm.K;
			pm.M=pm.DD/pm.K-pm.UDV();
			pm.P=pm.Pozice();
		}
		if(pm.rezim==100)//pouze pro pohon bez p�i�azen�ch objekt�
		{
			pm.Rz=pm.RD*pm.TT;
			pm.Rx=F->m.Rx2(pm.Rz,pm.R);
		}
	}
	if(CHECK[3])//m�n� se Rz, Rx, M, K, P z�st�v� aRD, RD, R, DD, CT
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

		if(pm.rezim==1)//pouze pro KK re�im
		{
			pm.Rz=pm.RD*pm.TT;
			pm.Rx=F->m.Rx2(pm.Rz,pm.R);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.K=pm.DD/pm.RD/pm.TT;
			pm.P=pm.Pozice();
		}
		if(pm.rezim==2)//pouze pro PP re�im
		{
			pm.K=pm.CT/pm.TT;
			pm.P=pm.Pozice();
			pm.M=pm.DD/pm.K-pm.UDV();
		}

	}

	if(CHECK[4])//m�n� se Rz, R, M, DD, P, CT z�st�v� aRD, RD, Rx, K
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

		if(pm.rezim==1)//pouze pro KK re�im
		{
			pm.Rz=pm.RD*pm.TT;
			pm.R=F->m.R(pm.Rz,pm.Rx);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.DD=pm.RD*pm.TT*pm.K;
			pm.CT=pm.DD/pm.RD;
			pm.P=pm.Pozice();
		}
		if(pm.rezim==2)//pouze pro PP re�im
		{
			pm.CT=pm.TT*pm.K;
			pm.DD=pm.Rz/pm.TT*pm.CT; //toto je ot�zka, zda je to opravdu t�eba???
			pm.P=pm.Pozice();
			pm.M=pm.DD/pm.K-pm.UDV();
		}
    if(pm.rezim==100)
		{
			pm.Rz=pm.RD*pm.TT;
			pm.R=F->m.R(pm.Rz,pm.Rx);
    }
	}
	if(CHECK[5])//m�n� se Rz, R, M, K, P z�st�v� aRD, RD, Rx, DD, CT
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

		if(pm.rezim==1)//pouze pro KK re�im
		{
			pm.Rz=pm.RD*pm.TT;
			pm.R=F->m.R(pm.Rz,pm.Rx);
			pm.M=F->m.mezera(pm.Rotace,pm.Rz);
			pm.K=pm.DD/pm.RD/pm.TT;
			pm.P=pm.Pozice();
		}
		if(pm.rezim==2)//pouze pro PP re�im
		{
			pm.K=pm.CT/pm.TT;
			pm.P=pm.Pozice();
			pm.M=pm.DD/pm.K-pm.UDV();
    }
	}

//  if(objekty[Row].pohon!=NULL)
//    {
//     if(indikator_skupin[objekty[Row].pohon->n]==1) pm.RD=pm.Rz/pm.TT; //v�po�et pm.RD= pro to jak kdy� je kontinu�ly v prnv� v prvn� oblasti
//    }

	//output sekce
	AnsiString T="";
	switch(SaveTo)  //POZOR: pm.RD je v�dy aRD, a� v sekci case 1 je vypo��t�no skute�n� RD=DD/CT
	{
     case -1://ulo�en� do textu je-li po�adov�no
		 {
				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
		 }break;

		 case 0://pouze vr�t� text do bun�k
		 {
        if(objekty[Row].id<100)//pokud se nejedn� o pohon bez p�i�azen�ch objekt�
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

				if(objekty[Row].pohon!=NULL || objekty[Row].id>100)//objekty s pohonem nebo pohon bez p�i�azen�ch objekt�
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
		 case 1://ulo�en� do spoj�ku OBJEKTY - je-li po�adov�no
		 {
				if(objekty[Row].id<100)//pokud se nejedn� o pohon bez p�i�azen�ch objekt�
				{
					Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
					if(O->pohon!=NULL)
					{ //POZOR: pm.RD je v�dy aRD, a� v sekci case 1 je vypo��t�no skute�n� RD=DD/CT
						O->pohon->aRD=pm.RD; F->d.v.vrat_pohon(O->pohon->n)->aRD=pm.RD;
						O->pohon->Rz=F->m.Rz(pm.RD);
						if(CHECK[2] || CHECK[4])O->pohon->Rx     = F->m.Rx2(O->pohon->Rz,pm.R);//z�st�v� R, m�n� se Rx
						if(CHECK[4] || CHECK[6])O->pohon->roztec = F->m.R(O->pohon->Rz,F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]));//z�st�v� Rx, m�n� se R
					}
					O->CT=pm.CT;
					O->RD=pm.DD/pm.CT;//nelze pou��t pm.RD p��mo, proto�e u S&G a PP by se RD!=aRD //POZOR: pm.RD je v�dy aRD, a� v sekci case 1 je vypo��t�no skute�n� RD=DD/CT
					O->delka_dopravniku=pm.DD;
					O->kapacita=pm.K;
					O->pozice=pm.P;
					O->mezera=pm.M;
					O->mezera_jig=pm.MJ;
					O->mezera_podvozek=pm.MP;
					O=NULL;delete O;
				}
				else//pokud se jedn� o pohon bez p�i�azen�ch objekt�
				{
					Cvektory::TPohon *pohon=F->d.v.vrat_pohon(objekty[Row].id-100);
          pohon->aRD=F->ms.MyToDouble(pm.RD*(1+59.0*aRDunit));
					pohon->roztec=F->ms.MyToDouble(pm.R*(1+999.0*Runit));
				  pohon->Rz=F->ms.MyToDouble(pm.Rz*(1+999.0*Rzunit));//if(Rzunit==0)	pohon->Rz=F->ms.MyToDouble(pm.Rz); else 	pohon->Rz =F->ms.MyToDouble(pm.Rz);
					pohon->Rx=F->ms.MyToDouble(pm.Rx);
					pohon=NULL;delete pohon;
				}
		 }break;
		 case 2://ulo�en� hodnot z ukazatele pro n�hled objektu
		 {
				if(pm.rezim<100)//neplat� pro pohony bez p�i�azen�ch objekt�
				{
					Form_objekt_nahled->pom=new Cvektory::TObjekt;
					//Form_objekt_nahled->pom->pohon=objekty[Row].pohon;//takto by p�evzal starou rozte�
					Form_objekt_nahled->pom->pohon=new Cvektory::TPohon;
					Form_objekt_nahled->pom->pohon->roztec=pm.R;//ale p�ed�v�m jen do n�hledu R, nic v�c od pohonu
					Form_objekt_nahled->pom->rezim=objekty[Row].rezim;
					Form_objekt_nahled->pom->CT=pm.CT;
					Form_objekt_nahled->pom->RD=pm.RD;//pozor �e�eno pouze kontinu�l
					Form_objekt_nahled->pom->delka_dopravniku=pm.DD;
					Form_objekt_nahled->pom->kapacita=pm.K;
					Form_objekt_nahled->pom->pozice=pm.P;
					Form_objekt_nahled->pom->mezera=pm.M;
					Form_objekt_nahled->pom->mezera_jig=pm.MJ;
					Form_objekt_nahled->pom->mezera_podvozek=pm.MP;
				}
				else
				Form_objekt_nahled->pom=NULL;//pro pohony bez p�i�azen�ch objekt�
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
  //navr�cen� p�vodn� hodnoty TT, p�i stisku storno na GAPO
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
		 if (scGPButton_OK->Enabled)scGPButton_OKClick(Sender);// pokud jsou z�rove� spln�ny podm�nky pro stisk OK
		 else 	MessageBeep(0); // p�pnut�m upozorn�, �e nelze
	 }break;
	 case 27:// ESC
	 {
		 Form_parametry_linky->Button_save->Enabled=true;
		 Form_parametry_linky->Button_storno->Enabled=true;
		 F_gapoTT->ModalResult = mrCancel;// vr�t� stejnou hodnotu jako tla��tko
		 F_gapoTT->VisibleChanging();// skryje form, stejn� jako visible=false
		 myModalResult=mrCancel;
		 //navr�cen� p�vodn� hodnoty TT, p�i stisku storno na GAPO
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
		T+=calculate(Row,-1)+"\n";//sice se propo��t�v� opakovan�, ale kv�li mo�n�mu zobrazen� dat ve zkr�cen� form� v tabulce. lepe z ostr�ch dat
	}
	F->d.v.Text2CSV(T,Form1->FileName+"_tabulka_gapoTT","Ulo�it hodnoty do CSV...","*.csv","Soubory form�tu CSV (*.csv)|*.csv|Soubory form�tu XLS (*.xls)|*.xls");
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
	//doladit posouv�n� komponent
	if(scScrollBar_horizont->Position<scScrollBar_horizont->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci mus� p�ekreslit cel�
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

