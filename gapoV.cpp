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
	//nastaven� barvy formul��e
	F_gapoV->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK a Storno
	F->m.designButton(scGPButton_OK,F_gapoV,1,2);
	F->m.designButton(scGPButton_storno,F_gapoV,2,2);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky

	//definice barev NEWR
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
  clHiddenValues = (TColor)RGB(212,212,212);

	// nastaven� barvy or�mov�n� v hlavi�ce tabulky + or�mov�n� checkbox�
	C1=Form1->m.clIntensive(RGB(128,64,0),20);
	C2=Form1->m.clIntensive(RGB(0,128,0),80);
	C3=Form1->m.clIntensive(RGB(0,0,0),80);
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormActivate(TObject *Sender)
{
	//zde nevolat obsah form show, mu�e zde d�lat probl�my
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormShow(TObject *Sender)
{
	////////jednotky////////
	AnsiString T=F->readINI("nastaveni_form_parametry", "CT");
	if(T=="")CTunit=0;else CTunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","RDt");
	if(T=="")RDunit=0;else RDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DD");
	if(T=="")DDunit=0;else DDunit=T.ToInt();
	T=F->readINI("nastaveni_form_parametry","DM");
	if(T=="")Munit=0; else Munit =T.ToInt();

  temp_pocitadlo=0;
	AnsiString titulek;
	if(F->d.v.PP.delka_jig!=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value) titulek+="d�lky jigu z "+AnsiString(F->d.v.PP.delka_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value);
	if(F->d.v.PP.sirka_jig!=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value) titulek+="���ky jigu z "+AnsiString(F->d.v.PP.sirka_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value);
	if(F->d.v.PP.vyska_jig!=Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value) titulek+="v��ky jigu z "+AnsiString(F->d.v.PP.vyska_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_vyska_jig->Value);
	if(F->d.v.PP.delka_podvozek!=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value)  titulek+="d�lky podvozku z "+AnsiString(F->d.v.PP.delka_podvozek)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value);

  Form_parametry_linky->Button_save->Enabled=false;
  Form_parametry_linky->Button_storno->Enabled=false;
  myModalResult=mrNone;

	input_state=LOADING;
	pruchod=0;
	leva_oblast=false;
	//workaround odchyt�v�n� stisku kl�ves
	Edit1->SetFocus();
	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=2;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;

	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=34;//pevn� po�et slopc�
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	RowCount+=F->d.v.pocet_objektu(1);
	mGrid->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty
  tempCHECK=new short[5];//dynamick� pole uchov�vaj�c� p�edchoz� volby checkbox�

	////////pln�n� daty - hlavi�ka////////
	mGrid->Cells[0][0].Text="Pohon";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnut� tu�n�ho p�sma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
	mGrid->Cells[1][0].Text="Objekt";
	mGrid->Cells[2][0].Text="M,P";
	mGrid->Cells[3][0].Text="aRD, RD, Rz, Rx, R, CT, DD, K";
	mGrid->Cells[4][0].Text="aRD, RD, M, Rz, Rx, K, CT, P";
	mGrid->Cells[5][0].Text="DD, R";
	mGrid->Cells[6][0].Text="aRD, RD, M, Rz, R, K, CT, P";
	mGrid->Cells[7][0].Text="DD, Rx";
	mGrid->Cells[8][0].Text="aRD, RD, M, Rz, Rx, DD, P";
	mGrid->Cells[9][0].Text="K, CT, R";
	mGrid->Cells[10][0].Text="aRD, RD, M, Rz, R, DD, P";
	mGrid->Cells[11][0].Text="K, CT, Rx";
	if(CTunit)mGrid->Cells[12][0].Text="CT - Technologick� �as [min]";else mGrid->Cells[12][0].Text="CT - Technologick� �as [s]";//mGrid->MergeCells(6,0,7,0);//slou�en� zat�m nefunguje dob�e
	if(RDunit)mGrid->Cells[14][0].Text="RD - Rychlost pohonu [m/min]";else mGrid->Cells[14][0].Text="RD - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[16][0].Text="DD - D�lka objekt [mm]";else mGrid->Cells[16][0].Text="DD - D�lka objekt [m]";
	mGrid->Cells[18][0].Text="K - Kapacita [voz�k� + mezer]";
	mGrid->Cells[20][0].Text="P - Pozice [voz�k�]";
	if(Munit)mGrid->Cells[22][0].Text="M - mezera jig [mm]";else mGrid->Cells[22][0].Text="M - mezera jig [m]";
	if(Munit)mGrid->Cells[24][0].Text="M - mezera voz�k [mm]";else mGrid->Cells[24][0].Text="M - mezera voz�k [m]";
	mGrid->Cells[26][0].Text="Rotace";
	if(Form_parametry_linky->Runit)mGrid->Cells[27][0].Text="R - rozte� [mm]";else mGrid->Cells[27][0].Text="R - rozte� [mm]";
	mGrid->Cells[29][0].Text="Rz - akt. palce - rozestup [m]";//p�ep�n�n� jednotek dod�lat
	mGrid->Cells[31][0].Text="Rx - ka�dy n-t� palec";
	mGrid->Cells[33][0].Text="N�hled";

  scGPImage_zamky->Top=45;
  scGPImage_zamky->Left=mGrid->Columns[2].Left-42;
	////////p�i�ad� cel� oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky////////
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro prvn� ��dek

	//p�i�azen� jin�ho fontu, nem�n�m polo�k�m - NEWR
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

	//manualfit v��ky 0-t�ho ��dku (zat�m nen� p�ipravena metoda)
	unsigned short n=3;//��slo sloupce s nejdel��m textem hlavi�ky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
  mGrid->Rows[0].Height= 	mGrid->Rows[0].Height + 30; //30 px je v��ka zame�ku
	//manualfit ���ky sloupc� mimo nult�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=23;//ostatn� n�sleduj�c� sloupce zat�m default ���ka

	//nastaven� velikosti nult�ho sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
	mGrid->SetColumnAutoFit(0);

	//slou�en� bun�k hlavi�ky PO  - vhodn� za SetColumnAutoFit um�st�t
	mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);mGrid->MergeCells(20,0,21,0);mGrid->MergeCells(22,0,23,0);mGrid->MergeCells(24,0,25,0);mGrid->MergeCells(27,0,28,0);mGrid->MergeCells(29,0,30,0);mGrid->MergeCells(31,0,32,0);

	////////jednoliv� ��dky////////
	unsigned long j=1;//��slo aktu�ln� zpracov�van�ho ��dku, mus� za��nat 1, 0 - je hlavi�ka
	////pr�chod v�emi objekty bez p�i�azen�ch pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false,1);
	for(unsigned long i=1;i<=On_pocet;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
		objekty[j]=On[i];
		//pohony
		mGrid->Cells[0][j].Text="nep�i�azen";mGrid->Cells[0][j].Background->Color=clBACKGROUND;
		//objekty
		mGrid->Cells[1][j].Text=On[i].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
		//volby - checkboxy
		mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
		mGrid->MergeCells(2,j,3,j);
		mGrid->MergeCells(4,j,5,j);//slou�en� sloupc�
		mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
		mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//slou�en� sloupc�
		mGrid->Cells[10][j].Type=mGrid->CHECK;
		mGrid->MergeCells(10,j,11,j);
		//parametry objekt�
		mGrid->Cells[12][j].Text=F->m.round2double(On[i].CT/(1+59.0*CTunit),2,"..");               mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT; mGrid->Cells[13][j].Font->Color=clUNLOCKED;
		mGrid->Cells[14][j].Text=F->m.round2double(On[i].RD*(1+59.0*RDunit),2,"..");                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
		mGrid->Cells[16][j].Text=F->m.round2double(On[i].delka_dopravniku*(1+999*DDunit),2,"..");  mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
		mGrid->Cells[18][j].Text=F->m.round2double(On[i].kapacita,2,"..");                           mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
		mGrid->Cells[20][j].Text=F->m.round2double(On[i].pozice,2,"..");                            mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clUNLOCKED;
		mGrid->Cells[22][j].Text=F->m.round2double(On[i].mezera_jig*(1+999*Munit),2,"..");           mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clUNLOCKED;
		mGrid->Cells[24][j].Text=F->m.round2double(On[i].mezera_podvozek*(1+999*Munit),2,"..");     mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clUNLOCKED;
		mGrid->Cells[26][j].Text=On[i].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
		if(On[i].pohon!=NULL)
		{
			mGrid->Cells[27][j].Text=On[i].pohon->roztec;                   mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
			mGrid->Cells[29][j].Text=On[i].pohon->Rz;                       mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clUNLOCKED;
			mGrid->Cells[31][j].Text=On[i].pohon->Rx;                       mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
		}
    else
    {
     mGrid->Cells[27][j].Background->Color=clHiddenValues;  mGrid->Cells[28][j].Background->Color=clHiddenValues;
     mGrid->Cells[29][j].Background->Color=clHiddenValues;  mGrid->Cells[30][j].Background->Color=clHiddenValues;
     mGrid->Cells[31][j].Background->Color=clHiddenValues;  mGrid->Cells[32][j].Background->Color=clHiddenValues;
    }
		//v�choz� nastaven� v druh�m lev�m slouci (pop�. upravit, ale je probl�m s prvn�m - nelze v�dy viz DV+M) - je v�dy po zobrazen� za�krnuta tato volba
		mGrid->getCheck(4,j)->Checked=true;//pozor, p�i getCheck(3,j) mi zde h�zelo chybu, i p�es o�et�en� R-z�le�itost�, p�i p�ekliku na tuto polo�ku po zobrazen�m formu, ale OK
		//tla��tko n�hledu
		mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
		TscGPButton *B=mGrid->createButton(33,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
		B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot
		calculate(j);//Rosto: mus� b�t posledn� p�ed j++, nelze ho d�t p�ed defaultn� za�krtnut� checkbox�
		//posun na dal�� ��dek v�sledn� tabulky
     for(int sl=0;sl<=ColCount-1;sl++) //odd�len� pohon� siln�j�� �arou
    {
    mGrid->Cells[sl][j].BottomBorder->Width=2;
    }
		j++;
	}
	On=NULL;delete On;
	if(On_pocet>0)mGrid->MergeCells(0,1,0,On_pocet);//slou�en� bun�k objekty bez pohonu

	////pr�chod v�emi pohony, kter� jsou p�i�azeny k objekt�m
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
   bool DVMmozno=true;
		unsigned long O_pocet=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1);
    if(O_pocet==0)//tzn. nen� objektu p�i�azen ��dn� pohon
		{
			//pohony bez p�i�azen� k objekt�m v gapoV nezobrazujeme a ne�e��me
		}
		else
		{
			//vrat� formou ukazatele na pole objekty p�i�azen� k dan�mu pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i,1);
			unsigned long z=0;
      for(;z<O_pocet;z++)
			{
				//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;mGrid->Cells[0][j].Background->Color=clBACKGROUND;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
				//volby - checkboxy

				mGrid->Cells[4][j].Type=mGrid->CHECK;
				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) <= 1 )  //KK re�im
				{
					 mGrid->Cells[2][j].Type=mGrid->CHECK;
					 mGrid->MergeCells(2,j,3,j);
				}

				mGrid->MergeCells(4,j,5,j);//slou�en� sloupc�
				mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
				mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//slou�en� sloupc�
				mGrid->Cells[10][j].Type=mGrid->CHECK;mGrid->MergeCells(10,j,11,j);

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK re�im
				{   //pokud akt.n�zev pohonu je odli�n� od p�edchoz�ho ��dku, nastav�m typ na CHECK.
					if(O[z].pohon->name!=mGrid->Cells[0][j-1].Text)
					{
						mGrid->Cells[2][j].Type=mGrid->CHECK;
						//barvu prvn�ho sloupce nastvuji n�e, nelze zde
					}
					mGrid->getCheck(4,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(4,j)->OptionsChecked->FrameNormalColor=C2;

					mGrid->getCheck(6,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(6,j)->OptionsChecked->FrameNormalColor=C2;

					mGrid->getCheck(8,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(8,j)->OptionsChecked->FrameNormalColor=C3;

					mGrid->getCheck(10,j)->Options->FrameNormalColor=C3;
					mGrid->getCheck(10,j)->OptionsChecked->FrameNormalColor=C3;

				}

				//parametry objekt�
				mGrid->Cells[12][j].Text=F->m.round2double(O[z].CT/(1+59.0*CTunit),2,"..");	             mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT; mGrid->Cells[13][j].Font->Color=clUNLOCKED;
				mGrid->Cells[14][j].Text=F->m.round2double(O[z].RD*(1+59.0*1),2,"..");              mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				mGrid->Cells[16][j].Text=F->m.round2double(O[z].delka_dopravniku*(1+999*DDunit),2,".."); mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
				mGrid->Cells[18][j].Text=F->m.round2double(O[z].kapacita,2,"..");                        mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[20][j].Text=F->m.round2double(O[z].pozice,2,"..");                          mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clUNLOCKED;
				mGrid->Cells[22][j].Text=F->m.round2double(O[z].mezera_jig*(1+999*Munit),2,"..");        mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clUNLOCKED;
				mGrid->Cells[24][j].Text=F->m.round2double(O[z].mezera_podvozek*(1+999*Munit),2,"..");   mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clUNLOCKED;
				mGrid->Cells[26][j].Text=O[z].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
				if(O[z].pohon!=NULL)
				{
					mGrid->Cells[27][j].Text=O[z].pohon->roztec;                   mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
					mGrid->Cells[29][j].Text=O[z].pohon->Rz;                       mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;mGrid->Cells[30][j].Align=mGrid->LEFT;mGrid->Cells[30][j].Font->Color=clUNLOCKED;
					mGrid->Cells[31][j].Text=O[z].pohon->Rx;                       mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clOLD;mGrid->Cells[32][j].Align=mGrid->LEFT;mGrid->Cells[32][j].Font->Color=clUNLOCKED;
				}
				mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
				TscGPButton *B=mGrid->createButton(33,j);B->Options->FontNormalColor=(TColor)RGB(255,128,0);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
				//v�choz� nastaven� v druh�m lev�m slouci (pop�. upravit, ale je probl�m s prvn�m - nelze v�dy  viz DV+M) - je v�dy po zobrazen� za�krnuta tato volba
				mGrid->getCheck(4,j)->Checked=true;
				//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot
				calculate(j);//Rosto: mus� b�t posledn� p�ed j++, nelze ho d�t p�ed v�choz� za�krtnut� checkbox�
        //testov�n� povolen� DVM skupiny
        Memo1->Lines->Add("UDV + "+AnsiString(F->m.UDV(O[z].rotace)));
        Memo1->Lines->Add("puv. mezera"+AnsiString(O[z].mezera));
        Memo1->Lines->Add("==UDV +"+AnsiString(F->m.UDV(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace)));
        Memo1->Lines->Add("mezera mezi voz"+AnsiString(F->m.mezera_mezi_voziky(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace,O[z].pohon->roztec,0)));
        if(DVMmozno==true)
        {
         if(F->m.UDV(O[z].rotace)+O[z].mezera==F->m.UDV(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace)+F->m.mezera_mezi_voziky(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace,O[z].pohon->roztec,0))DVMmozno=true;
         else DVMmozno=false;
        }
				//posun na dal�� ��dek v�sledn� tabulky
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slou�en� bun�k pohony
			mGrid->MergeCells(2,j-z,3,j-z+O_pocet-1);//slou�en� bun�k 2-3

      for(int sl=0;sl<=ColCount-1;sl++) //siln�j�� odd�len� dal��ho pohonu
      {
       mGrid->Cells[sl][j-z+O_pocet-1].BottomBorder->Width=2;
      }

      if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK re�im
      { //nelze nastavit hned, v horn� ��sti, spole�n� s typem Check, ale a� zde
        mGrid->getCheck(2,j-z)->Options->FrameNormalColor=C1;
        mGrid->getCheck(2,j-z)->OptionsChecked->FrameNormalColor=C1;
      }
       // ShowMessage((short)DVMmozno);
      if(DVMmozno) {mGrid->getCheck(2,j-z)->Enabled=true;  mGrid->getCheck(2,j-z)->ShowHint=true; mGrid->getCheck(2,j-z)->Hint="mohu";  }
      else {mGrid->getCheck(2,j-z)->Enabled=true; mGrid->getCheck(2,j-z)->ShowHint=false; mGrid->getCheck(2,j-z)->Hint="nemohu";   }

			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m tlust�j��m or�mov�n�m////////

    for(int i=1;i<=RowCount-1;i++)
    {
     mGrid->Cells[1][i].RightBorder->Width=2;
     mGrid->Cells[11][i].RightBorder->Width=2;
    }

    for(int j=0;j<=ColCount-1;j++)
    {
     mGrid->Cells[j][0].BottomBorder->Width=2;
    }

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

  for(int r=0;r<=RowCount-1;r++)
   {
    mGrid->Cells[13][r].RightBorder->Width=mGrid->Cells[15][r].RightBorder->Width=mGrid->Cells[17][r].RightBorder->Width=mGrid->Cells[19][r].RightBorder->Width=mGrid->Cells[21][r].RightBorder->Width=mGrid->Cells[23][r].RightBorder->Width=mGrid->Cells[25][r].RightBorder->Width=mGrid->Cells[26][r].RightBorder->Width=mGrid->Cells[28][r].RightBorder->Width=mGrid->Cells[30][r].RightBorder->Width=mGrid->Cells[32][r].RightBorder->Width=2;
   }

	////zobrazen� or�mov�n�
	zobrazitFrameForm=true;


	//barevn� or�mov�n� hlavi�ky
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
void __fastcall TF_gapoV::FormPaint(TObject *Sender)
{
 	mGrid->Show();//vykresl� tabulku
 	if(zobrazitFrameForm)F->m.frameForm(this,clWebOrange,1);//vykresl� or�mov�n� okolo formu
}

//---------------------------------------------------------------------------
 void TF_gapoV::vypis(UnicodeString text,bool red,bool link)
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
//---------------------------------------------------------------------------
void TF_gapoV::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
//ZAJIST�, �E NELZE UDELAT UNCHECK
//	if(Col>=4 && mGrid->getCheck(Col,Row)->Checked==false)
//	{
//		TscGPCheckBox *CH=mGrid->getCheck(Col,Row);
//		CH->Checked=true;
//		CH=NULL;delete CH;
//	}
		vypis("",false);
  temp_pocitadlo++;
	if(Col==2)
  {
  if(temp_pocitadlo==1)scGPImage_zamky->Left=mGrid->Left + mGrid->Columns[2].Left + 5;
  if(temp_pocitadlo==2)scGPImage_zamky->Left=mGrid->Left + mGrid->Columns[2].Left + 5;
  else scGPImage_zamky->Left=mGrid->Left + mGrid->Columns[2].Left + 3;
  }
	if(Col>=6)
  {
  if(temp_pocitadlo==1)scGPImage_zamky->Left=mGrid->Left + mGrid->Columns[2].Left + 5;
  if(temp_pocitadlo==2)scGPImage_zamky->Left=mGrid->Left + mGrid->Columns[2].Left + 5;
  else scGPImage_zamky->Left=mGrid->Left + mGrid->Columns[2].Left + 3;
  }

	if(Col==2  && mGrid->getCheck(Col,Row)->ShowHint==false && input_state==FREE)
	{
	 vypis("Tato varianta nen� mo�n�, nebo� doch�z� ke zm�n� mezery.",false);
	 mGrid->getCheck(Col,Row)->Checked=false;
	 //mGrid->getCheck(tempCHECK[Row],Row)->Checked=true;//za�krtne p�edchoz� volbu p�esunout do onclick ur�it� p�ed calculate
	}

	if(Col==2 &&  mGrid->getCheck(Col,Row)->Checked && input_state==FREE)
	{

	 if(objekty[Row].pohon!=NULL)
	 {
		int pohon_n=objekty[Row].pohon->n;

		for(int i=1;i<=mGrid->RowCount-1;i++)
		{
			if(objekty[i].pohon!=NULL)   //pokud v oblasti vstoup�m do lev�ho slou�en�ho sloupce, zru��m v�echny ostatn� checked hodnoty
			{
			 if(pohon_n == objekty[i].pohon->n)
			 {
				mGrid->getCheck(Col+2,i)->Checked=false;
				mGrid->getCheck(Col+4,i)->Checked=false;
				mGrid->getCheck(Col+6,i)->Checked=false;
				mGrid->getCheck(Col+8,i)->Checked=false;
				leva_oblast=true;
			 }
			}
		}
	 }  //std chov�n�, mimo oblasti
	 else
	 {
		mGrid->getCheck(Col+2,Row)->Checked=false;
		mGrid->getCheck(Col+4,Row)->Checked=false;
		mGrid->getCheck(Col+6,Row)->Checked=false;
		mGrid->getCheck(Col+8,Row)->Checked=false;
		leva_oblast=false;
	 }
	input_state=FREE;
	}


	if(Col==4 &&  mGrid->getCheck(Col,Row)->Checked && input_state==FREE)
	{
		if(objekty[Row].pohon!=NULL)
		{
			int pohon_n=objekty[Row].pohon->n;
			if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1) //default �ed� barva je z�porn�, proto mohu nastavovat
			{
			//m� pohon, ale m� jen jeden ��dek, tzn mohu nastavit klasicky prvn� sloupec
			//ShowMessage("jsem sam KK, mohu");
			mGrid->getCheck(Col-2,Row)->Checked=false;
			mGrid->getCheck(Col+2,Row)->Checked=false;
			mGrid->getCheck(Col+4,Row)->Checked=false;
			mGrid->getCheck(Col+6,Row)->Checked=false;
			}
			else // mus�m naj�t a vybrat spr�vn� ��dek v prvn�m sloupci, kter� d�m Checked=false
			{
				for(int i=1;i<=mGrid->RowCount-1;i++)
				{
					 if(objekty[i].pohon!=NULL)
					 {
							if(pohon_n == objekty[i].pohon->n)
							{
								 pruchod++;  //workaround -  ve foru najdu prvn� v�skyt a dal�� u� ne�e��m (pam�tov� chyba, kv�li slou�en�m bu�k�m)
								 if(pruchod==1 && input_state==FREE) 	mGrid->getCheck(Col-2,i)->Checked=false;

								 mGrid->getCheck(Col+2,Row)->Checked=false;
								 mGrid->getCheck(Col+4,Row)->Checked=false;
								 mGrid->getCheck(Col+6,Row)->Checked=false;

								 if(leva_oblast)
								 {
									input_state=PROGRAMOVE;
									mGrid->getCheck(4,i)->Checked=true;
								 }
							}
					 }
				}
			}
	 }
	 else //nem�m pohon, mohu nastavit hned klasicky prvn� sloupec
	 {
		 mGrid->getCheck(Col-2,Row)->Checked=false;
		 mGrid->getCheck(Col+2,Row)->Checked=false;
		 mGrid->getCheck(Col+4,Row)->Checked=false;
		 mGrid->getCheck(Col+6,Row)->Checked=false;
	 }
	 pruchod=0;
	 leva_oblast=false;
	 input_state=FREE;
	}

	if(Col==6 &&  mGrid->getCheck(Col,Row)->Checked && input_state==FREE)
	{
	 if(objekty[Row].pohon!=NULL)
	 {
		int pohon_n=objekty[Row].pohon->n;
		if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1)
		{
		//m� pohon, ale m� jen jeden ��dek, tzn mohu nastavit klasicky prvn� sloupec
		//ShowMessage("jsem sam KK, mohu");

			 mGrid->getCheck(Col-4,Row)->Checked=false;
			 mGrid->getCheck(Col-2,Row)->Checked=false;
			 mGrid->getCheck(Col+2,Row)->Checked=false;
			 mGrid->getCheck(Col+4,Row)->Checked=false;
		}
		else // mus�m naj�t a vybrat spr�vn� ��dek v prvn�m sloupci, kter� d�m Checked=false
		{
			for(int i=1;i<=mGrid->RowCount-1;i++)
			{
				if(objekty[i].pohon!=NULL)
				{
					 if(pohon_n == objekty[i].pohon->n)
					 {
						 pruchod++;  //workaround -  ve foru najdu prvn� v�skyt a dal�� u� ne�e��m (pam�tov� chyba, kv�li slou�en�m bu�k�m)
						if(pruchod==1)   	mGrid->getCheck(Col-4,i)->Checked=false;

							 mGrid->getCheck(Col-2,Row)->Checked=false;
							 mGrid->getCheck(Col+2,Row)->Checked=false;
							 mGrid->getCheck(Col+4,Row)->Checked=false;

							 if(leva_oblast)
							 {
								input_state=PROGRAMOVE;
							 	mGrid->getCheck(6,i)->Checked=true;
							 }
					 }

				}
			}

		}
	 }
	 else //nem�m pohon, mohu nastavit hned klasicky prvn� sloupec
	 {
	 //ShowMessage("jsem bez pohonu, mohu");
	 mGrid->getCheck(Col-4,Row)->Checked=false;
	 mGrid->getCheck(Col-2,Row)->Checked=false;
	 mGrid->getCheck(Col+2,Row)->Checked=false;
	 mGrid->getCheck(Col+4,Row)->Checked=false;
	 }
	 pruchod=0;
	 leva_oblast=false;
	 input_state=FREE;
	}

	if(Col==8 &&  mGrid->getCheck(Col,Row)->Checked && input_state==FREE)
	{

	 if(objekty[Row].pohon!=NULL)
	 {
		int pohon_n=objekty[Row].pohon->n;
		if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1) //default �ed� barva je z�porn�, proto mohu nastavovat
		{
		//m� pohon, ale m� jen jeden ��dek, tzn mohu nastavit klasicky prvn� sloupec

		mGrid->getCheck(Col-6,Row)->Checked=false;
		mGrid->getCheck(Col-4,Row)->Checked=false;
		mGrid->getCheck(Col-2,Row)->Checked=false;
		mGrid->getCheck(Col+2,Row)->Checked=false;
		}
		else // mus�m naj�t a vybrat spr�vn� ��dek v prvn�m sloupci, kter� d�m Checked=false
		{

			for(int i=1;i<=mGrid->RowCount-1;i++)
			{
				if(objekty[i].pohon!=NULL)
				{
					 if(pohon_n == objekty[i].pohon->n)
					 {
					  pruchod++;  //workaround -  ve foru najdu prvn� v�skyt a dal�� u� ne�e��m (pam�tov� chyba, kv�li slou�en�m bu�k�m)
						if(pruchod==1)   	mGrid->getCheck(Col-6,i)->Checked=false;

						mGrid->getCheck(Col-4,Row)->Checked=false;
						mGrid->getCheck(Col-2,Row)->Checked=false;
						mGrid->getCheck(Col+2,Row)->Checked=false;

						 if(leva_oblast)
						 {
							input_state=PROGRAMOVE;
							mGrid->getCheck(8,i)->Checked=true;
						 }
					 }
				}
			}
		}
	 }
	 else //nem�m pohon, mohu nastavit hned klasicky prvn� sloupec
	 {
	 //ShowMessage("jsem bez pohonu, mohu");
		mGrid->getCheck(Col-6,Row)->Checked=false;
		mGrid->getCheck(Col-4,Row)->Checked=false;
		mGrid->getCheck(Col-2,Row)->Checked=false;
		mGrid->getCheck(Col+2,Row)->Checked=false;
	 }
	 pruchod=0;
	 leva_oblast=false;
	 input_state=FREE;
	}

	if(Col==10 &&  mGrid->getCheck(Col,Row)->Checked && input_state==FREE)
	{

	 if(objekty[Row].pohon!=NULL)
	 {
		int pohon_n=objekty[Row].pohon->n;
		if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1) //default �ed� barva je z�porn�, proto mohu nastavovat
		{
		//m� pohon, ale m� jen jeden ��dek, tzn mohu nastavit klasicky prvn� sloupec

		mGrid->getCheck(Col-8,Row)->Checked=false;
		mGrid->getCheck(Col-6,Row)->Checked=false;
		mGrid->getCheck(Col-4,Row)->Checked=false;
		mGrid->getCheck(Col-2,Row)->Checked=false;
		}
		else // mus�m naj�t a vybrat spr�vn� ��dek v prvn�m sloupci, kter� d�m Checked=false
		{

			for(int i=1;i<=mGrid->RowCount-1;i++)
			{
				if(objekty[i].pohon!=NULL)
				{
					if(pohon_n == objekty[i].pohon->n)
					{
						 pruchod++;  //workaround -  ve foru najdu prvn� v�skyt a dal�� u� ne�e��m (pam�tov� chyba, kv�li slou�en�m bu�k�m)
						 if(pruchod==1)   	mGrid->getCheck(Col-8,i)->Checked=false;

							mGrid->getCheck(Col-6,Row)->Checked=false;
							mGrid->getCheck(Col-4,Row)->Checked=false;
							mGrid->getCheck(Col-2,Row)->Checked=false;

							if(leva_oblast)
							{
								input_state=PROGRAMOVE;
								mGrid->getCheck(10,i)->Checked=true;
							}
					}
				}
			}
		}
	 }
	 else //nem�m pohon, mohu nastavit hned klasicky prvn� sloupec
	 {
	 //ShowMessage("jsem bez pohonu, mohu");
		mGrid->getCheck(Col-8,Row)->Checked=false;
		mGrid->getCheck(Col-6,Row)->Checked=false;
		mGrid->getCheck(Col-4,Row)->Checked=false;
		mGrid->getCheck(Col-2,Row)->Checked=false;
	 }
	 pruchod=0;
	 leva_oblast=false;
	 input_state=FREE;
	}	 //KONEC P�EP�NA��

	//VALIDA�N� ��ST - HL�D�N� OBLAST�

	if(Col>=4 && Col<=6 && input_state==FREE )  /*&&  mGrid->getCheck(6,Row)->Checked==false*/
		{
	 if(mGrid->getCheck(4,Row)->Checked==true ||  mGrid->getCheck(6,Row)->Checked==true)
	 {
		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
		if(objekty[Row].pohon!=NULL)
		{
			int pohon_n=objekty[Row].pohon->n;
			if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
			{
			 //pr�chod cel� tabulky
						pocitadlo_validace=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
							if(objekty[i].pohon!=NULL)
							{
												if(pohon_n==objekty[i].pohon->n)
												{
													 if (mGrid->getCheck(8,i)->Checked==true   ||  mGrid->getCheck(10,i)->Checked==true )
														{
															//ShowMessage(i);
															pocitadlo_validace++;
															vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!");
														}
												}
						}
							}

					 if(pocitadlo_validace==0)   //pokud jsem nena�el ��dn� zakliknut� bu�ky, mohu povolit vstup do druh� oblasti
					 {                  //projdu op�t cel� cyklem a aktivuji druhou oblast

							// if(pohon_n==objekty[i].pohon->n)
								vypis("",false);

					 }

				}

	 }
	 }

	 }


		if(Col>=8 && Col<=10 && input_state==FREE )  /*&&  mGrid->getCheck(6,Row)->Checked==false*/
		{
	 if(mGrid->getCheck(8,Row)->Checked==true ||  mGrid->getCheck(10,Row)->Checked==true)
	 {
		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
		if(objekty[Row].pohon!=NULL)
		{
			int pohon_n=objekty[Row].pohon->n;
			if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
			{
			 //pr�chod cel� tabulky
						pocitadlo_validace=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
							if(objekty[i].pohon!=NULL)
							{
												if(pohon_n==objekty[i].pohon->n)
												{
													 if (mGrid->getCheck(4,i)->Checked==true   ||  mGrid->getCheck(6,i)->Checked==true )
														{
															//ShowMessage(i);
															pocitadlo_validace++;
															vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!");
														}
												}
						}
							}

					 if(pocitadlo_validace==0) 	vypis("",false);  //povol�m ulo�en�

			 }
		 }
	 }

	 }

	//KONEC VALIDA�N� ��STI - HL�D�N� OBLAST�//

		//loadnut� default nastaven� p�ep�na��
		 if(input_state==LOADING && Col>2)
	{

			//t�et� resp. prvn� v�b�rov� sloupec je v�dy p�edvybr�n na true
			mGrid->getCheck(4,Row)->Checked=true;
			mGrid->getCheck(6,Row)->Checked=false;
			mGrid->getCheck(8,Row)->Checked=false;
			mGrid->getCheck(10,Row)->Checked=false;


	 }

//

	if(Col==mGrid->ColCount-1)//je kliknuto na n�hled objektu
	{
	//	calculate(Row,2);
		scGPButton_OK->Enabled=false;scGPButton_storno->Enabled=false;
		Form_objekt_nahled->zobrazitFrameForm=true;zobrazitFrameForm=false;
		Invalidate();FormPaint(this);//zajist� p�ekreslen� bez probliku
		Form_objekt_nahled->Left=Left+Width/2-Form_objekt_nahled->Width/2;
		Form_objekt_nahled->Top=Top+Height/2-Form_objekt_nahled->Height/2;
		Form_objekt_nahled->ShowModal();
		scGPButton_OK->Enabled=true;scGPButton_storno->Enabled=true;zobrazitFrameForm=true;
	}
	else
	{
	if(input_state==FREE)
	 //p�ekliknut� chechboxu pravd�podobn�
	{
	 	calculate(Row);//zajist� p�epo�et dan�ho ��dku
	 	FormPaint(this);//zajist� p�ekreslen� bez probliku
   //mGrid->Refresh();
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
//pro dan� ��dek dle nastaven�ho checkboxu, dopo��t� a dosad� nov� hodnoty parametr� dan�ho objektu z dan�ho ��dku, v p��pad� SaveTo -1, vr�t� formou textu, odd�len� st�edn�ky, 0 - nevrac� nic, 1 ulo�� do bin�rky
UnicodeString TF_gapoV::calculate(unsigned long Row,short SaveTo)//NEWR
{
	//instance na PO_math, vyu��v� se stejn�ch v�po�t�
	TPO_math pm;

	//input sekce
	pm.TT=F->d.v.PP.TT;
	pm.rezim=objekty[Row].rezim;
	pm.CT=objekty[Row].CT;
	pm.DD=objekty[Row].delka_dopravniku;
	pm.K=objekty[Row].kapacita;
	pm.P=objekty[Row].pozice;
	pm.M=objekty[Row].mezera;
	pm.MJ=objekty[Row].mezera_jig;
	pm.MP=objekty[Row].mezera_podvozek;
	pm.dJ=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value;//zajistit p�evod
	pm.sJ=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value;//zajistit p�evod
	pm.dP=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value;//zajistit p�evod
	pm.Rotace=objekty[Row].rotace;
	if(objekty[Row].pohon!=NULL)
	{
																		//Rosta zajistit p�evod
		pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/60.0);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
		pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999*Form_parametry_linky->Runit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	}
	else
	{
		pm.RD=objekty[Row].RD;
		pm.R=0;
  }

	//optimalizace detekce a uchov�n� volby za�krtnut�ho checkboxu, aby se nemuselo vyvol�vat znovu
	bool CHECK[5];
	if(mGrid->getCheck(2,Row)== NULL) 	CHECK[0]=false;
	else CHECK[0]=mGrid->getCheck(2,Row)->Checked; if(CHECK[0])tempCHECK[Row]=2;
	CHECK[1]=mGrid->getCheck(4,Row)->Checked; if(CHECK[1])tempCHECK[Row]=4;
	CHECK[2]=mGrid->getCheck(6,Row)->Checked; if(CHECK[2])tempCHECK[Row]=6;
	CHECK[3]=mGrid->getCheck(8,Row)->Checked; if(CHECK[3])tempCHECK[Row]=8;
	CHECK[4]=mGrid->getCheck(10,Row)->Checked; if(CHECK[4])tempCHECK[Row]=10;

	//vol�n� samotn�ho v�po�tu dle volby stanov�n� pomoci checkboxu
	if(CHECK[0])//m�n� se M,P, z�st�v� aRD, RD, Rz, Rx, R, CT, DD, K
	{
		pm.P=pm.K2P(pm.K);//pozice
		pm.M=pm.Mezera();//mezera, vzhledem k tomu, �e aRD,Rz,R z�st�vaj� mezera se po��t� pouze k aktu�ln�mu aRD resp. RD
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
	if(CHECK[1] || CHECK[2])//m�n� se aRD, RD, M, Rz, Rx resp. R, K, CT, P z�st�v� DD, R resp. Rx
	{
		pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);//vr�t� doporu�enou nejbli��� rychlost pohonu, k rychlosti zadan� tak, aby se reflektovala rozte� mezi palci i takt
		pm.CT_locked=false;
		pm.DD_locked=true;
		pm.input_RD(true);
		if(SaveTo==0)
		{
			mGrid->Cells[13][Row].Font->Color=clUNLOCKED;//CT
			mGrid->Cells[15][Row].Font->Color=clUNLOCKED;//RD
			mGrid->Cells[17][Row].Font->Color=clLOCKED;//DD
			mGrid->Cells[19][Row].Font->Color=clUNLOCKED;//K
			mGrid->Cells[21][Row].Font->Color=clUNLOCKED;//P
			mGrid->Cells[23][Row].Font->Color=clUNLOCKED;//MJ
			mGrid->Cells[25][Row].Font->Color=clUNLOCKED;//MP
			mGrid->Cells[30][Row].Font->Color=clLOCKED;//Rz
			if(CHECK[1])//z�st�v� R, m�n� se Rx
			{
				mGrid->Cells[28][Row].Font->Color=clLOCKED; //R
				mGrid->Cells[32][Row].Font->Color=clUNLOCKED;//Rx
			}
			else//naopak
			{
				mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//R
				mGrid->Cells[32][Row].Font->Color=clLOCKED;//Rx
			}
		}
	}
	if(CHECK[3] || CHECK[4])//m�n� se aRD, RD, M, Rz, Rx resp. R, DD, P z�st�v� K, CT, R resp. Rx
	{
		pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);//vr�t� doporu�enou nejbli��� rychlost pohonu, k rychlosti zadan� tak, aby se reflektovala rozte� mezi palci i takt
		pm.CT_locked=true;
		pm.DD_locked=false;
		pm.input_RD(true);
		if(SaveTo==0)
		{
			mGrid->Cells[13][Row].Font->Color=clLOCKED;//CT
			mGrid->Cells[15][Row].Font->Color=clUNLOCKED;//RD
			mGrid->Cells[17][Row].Font->Color=clUNLOCKED;//DD
			mGrid->Cells[19][Row].Font->Color=clLOCKED;//K
			mGrid->Cells[21][Row].Font->Color=clUNLOCKED;//P
			mGrid->Cells[23][Row].Font->Color=clUNLOCKED;//MJ
			mGrid->Cells[25][Row].Font->Color=clUNLOCKED;//MP
			mGrid->Cells[30][Row].Font->Color=clLOCKED;//Rz
			if(CHECK[3])//z�st�v� R, m�n� se Rx
			{
				mGrid->Cells[28][Row].Font->Color=clLOCKED; //R
				mGrid->Cells[32][Row].Font->Color=clUNLOCKED;//Rx
			}
			else//naopak
			{
				mGrid->Cells[28][Row].Font->Color=clUNLOCKED;//R
				mGrid->Cells[32][Row].Font->Color=clLOCKED;//Rx
			}
		}
	}
	//output sekce
	AnsiString T="";
	switch(SaveTo)
	{
		 case -1://ulo�en� do textu je-li po�adov�no
		 {
				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
		 }break;
		 case 0://pouze vr�t� text do bun�k      objekty[Row].
		 {
				mGrid->Cells[13][Row].Text = F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
				mGrid->Cells[15][Row].Text = F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
				mGrid->Cells[17][Row].Text = F->m.round2double(pm.DD*(1+999*DDunit),2,"..");
				mGrid->Cells[19][Row].Text = F->m.round2double(pm.K,2,"..");
				mGrid->Cells[21][Row].Text = F->m.round2double(pm.P,2,"..");
				mGrid->Cells[23][Row].Text = F->m.round2double(pm.MJ*(1+999*Munit),2,"..");
				mGrid->Cells[25][Row].Text = F->m.round2double(pm.MP*(1+999*Munit),2,"..");
				if(objekty[Row].pohon!=NULL)
				{
					mGrid->Cells[30][Row].Text = F->m.Rz(pm.RD);
					objekty[Row].pohon->Rz=F->m.Rz(pm.RD);
					if(CHECK[1] || CHECK[3])//z�st�v� R, m�n� se Rx
					{
						mGrid->Cells[28][Row].Text = pm.R;
						mGrid->Cells[32][Row].Text = F->m.Rx2(objekty[Row].pohon->Rz,pm.R);
					}
					if(CHECK[2] || CHECK[4])//z�st�v� Rx, m�n� se R
					{
						mGrid->Cells[32][Row].Text = F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]);
						mGrid->Cells[28][Row].Text = F->m.R(objekty[Row].pohon->Rz,F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]));
					}
				}
		 }break;
		 case 1://ulo�en� do spoj�ku OBJEKTY - je-li po�adov�no
		 {
				Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
				if(O->pohon!=NULL)
				{
					O->pohon->aRD=pm.RD;
					O->pohon->Rz=F->m.Rz(pm.RD);
					if(CHECK[1] || CHECK[3])O->pohon->Rx     = F->m.Rx2(O->pohon->Rz,pm.R);//z�st�v� R, m�n� se Rx
					if(CHECK[2] || CHECK[4])O->pohon->roztec = F->m.R(O->pohon->Rz,F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]));//z�st�v� Rx, m�n� se R
				}
				O->CT=pm.CT;
				O->RD=pm.RD;
				O->delka_dopravniku=pm.DD;
				O->kapacita=pm.K;
				O->pozice=pm.P;
				O->mezera=pm.M;
				O->mezera_jig=pm.MJ;
				O->mezera_podvozek=pm.MP;
				O=NULL;delete O;
		 }break;
		 case 2://ulo�en� hodnot z ukazatele pro n�hled objektu
		 {
				Form_objekt_nahled->pom=new Cvektory::TObjekt;
				//Form_objekt_nahled->pom->pohon=objekty[Row].pohon;//takto by p�evzal starou rozte�
				Form_objekt_nahled->pom->pohon=new Cvektory::TPohon;
				Form_objekt_nahled->pom->pohon->roztec=pm.R;//ale p�ed�v�m jen do n�hledu R, nic v�c od pohonu
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
	}
	return T;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPButton_OKClick(TObject *Sender)
{
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		calculate(Row,1);//sice se propo��t�v� opakovan�, ale kv�li mo�n�mu zobrazen� dat ve zkr�cen� form� v tabulce. lepe z ostr�ch dat
		//aktualizace PO, pokud je spu�t�n�
		if(F->pom!=NULL)
    {//nutno jako samostan� if
		if(F->pom->n==objekty[Row].n)//je spu�t�n� PO a je tedy nutn� zavolat aktualizaci PO
		{
			F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vr�t� ostr� data
			F->NPin();
		}
    }
	}
  myModalResult=mrOk;
  Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] objekty;
  delete[] tempCHECK;
}
//---------------------------------------------------------------------------


void __fastcall TF_gapoV::scGPButton_stornoClick(TObject *Sender)
{
	Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
  Form_parametry_linky->scGPNumericEdit_delka_jig->Value=F->d.v.PP.delka_jig;
  Form_parametry_linky->scGPNumericEdit_sirka_jig->Value=F->d.v.PP.sirka_jig;
  Form_parametry_linky->scGPNumericEdit_vyska_jig->Value=F->d.v.PP.vyska_jig;
  myModalResult=mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TF_gapoV::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (scGPButton_OK->Enabled)scGPButton_OKClick(Sender);// pokud jsou z�rove� spln�ny podm�nky pro stisk OK
			else 	MessageBeep(0); // p�pnut�m upozorn�, �e nelze
		 }break;
		 case 27:// ESC
		 {
		 	Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
			F_gapoV->ModalResult = mrCancel;// vr�t� stejnou hodnotu jako tla��tko
			F_gapoV->VisibleChanging();// skryje form, stejn� jako visible=false
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
		T+=calculate(Row,-1)+"\n";//sice se propo��t�v� opakovan�, ale kv�li mo�n�mu zobrazen� dat ve zkr�cen� form� v tabulce. lepe z ostr�ch dat
	}
	F->d.v.Text2CSV(T,Form1->FileName+"_tabulka_gapoV","Ulo�it hodnoty do CSV...","*.csv","Soubory form�tu CSV (*.csv)|*.csv|Soubory form�tu XLS (*.xls)|*.xls");

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

