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

	// nastaven� barvy or�mov�n� v hlavi�ce tabulky + or�mov�n� checkbox�
	C1=Form1->m.clIntensive(RGB(128,64,0),20);
	C2=Form1->m.clIntensive(RGB(0,128,0),80);
	C3=Form1->m.clIntensive(RGB(20,80,100),80);
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
	T=F->readINI("nastaveni_form_parametry","DM").ToInt();
	if(T=="")Munit=0; else Munit =T.ToInt();

	input_state=LOADING;

	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=2;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;

	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=34;//pevn� po�et slopc�
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	RowCount+=F->d.v.OBJEKTY->predchozi->n-F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false,1)  ; //ROSTA -1
	mGrid->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty

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
		mGrid->Cells[12][j].Text=On[i].CT/(1+59.0*CTunit);                mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT; mGrid->Cells[13][j].Font->Color=clUNLOCKED;
		mGrid->Cells[14][j].Text=On[i].RD*(1+59.0*RDunit);                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
		mGrid->Cells[16][j].Text=On[i].delka_dopravniku*(1+999*DDunit);   mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
		mGrid->Cells[18][j].Text=On[i].kapacita;                          mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
		mGrid->Cells[20][j].Text=On[i].pozice;                            mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clUNLOCKED;
		mGrid->Cells[22][j].Text=On[i].mezera_jig*(1+999*Munit);          mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clUNLOCKED;
		mGrid->Cells[24][j].Text=On[i].mezera_podvozek*(1+999*Munit);     mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clUNLOCKED;
		mGrid->Cells[26][j].Text=On[i].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
		if(On[i].pohon!=NULL)
		{
			mGrid->Cells[27][j].Text=On[i].pohon->roztec;                   mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
			mGrid->Cells[29][j].Text=On[i].pohon->Rz;                       mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clUNLOCKED;
			mGrid->Cells[31][j].Text=On[i].pohon->Rx;                       mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clOLD;mGrid->Cells[31][j].Align=mGrid->LEFT;mGrid->Cells[31][j].Font->Color=clUNLOCKED;
		}
		//v�choz� nastaven� v druh�m lev�m slouci (pop�. upravit, ale je probl�m s prvn�m - nelze v�dy viz DV+M) - je v�dy po zobrazen� za�krnuta tato volba
		mGrid->getCheck(4,j)->Checked=true;//pozor, p�i getCheck(3,j) mi zde h�zelo chybu, i p�es o�et�en� R-z�le�itost�, p�i p�ekliku na tuto polo�ku po zobrazen�m formu, ale OK
		//tla��tko n�hledu
		mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
		TscGPButton *B=mGrid->createButton(33,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
		B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot
	//	calculate(j);//Rosto: mus� b�t posledn� p�ed j++, nelze ho d�t p�ed defaultn� za�krtnut� checkbox�
		//posun na dal�� ��dek v�sledn� tabulky
		j++;
	}
	On=NULL;delete On;
	if(On_pocet>0)mGrid->MergeCells(0,1,0,On_pocet);//slou�en� bun�k objekty bez pohonu

	////pr�chod v�emi pohony
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		unsigned long O_pocet=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i);
    if(O_pocet==0)//tzn. nen� objektu p�i�azen ��dn� pohon
		{
			//pohony bez p�i�azen� k objekt�m v gapoV nezobrazujeme a ne�e��me
		}
		else
		{
			//vrat� formou ukazatele na pole objekty p�i�azen� k dan�mu pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i);
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

			//	mGrid->Cells[2][j].Type=mGrid->CHECK;
			//	mGrid->MergeCells(2,j,3,j);
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

				mGrid->Cells[12][j].Text=O[z].CT/(1+59.0*CTunit);                mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT; mGrid->Cells[13][j].Font->Color=clUNLOCKED;
				mGrid->Cells[14][j].Text=O[z].RD*(1+59.0*RDunit);                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				mGrid->Cells[16][j].Text=O[z].delka_dopravniku*(1+999*DDunit);   mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
				mGrid->Cells[18][j].Text=O[z].kapacita;                          mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[20][j].Text=O[z].pozice;                            mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clUNLOCKED;
				mGrid->Cells[22][j].Text=O[z].mezera_jig*(1+999*Munit);          mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clUNLOCKED;
				mGrid->Cells[24][j].Text=O[z].mezera_podvozek*(1+999*Munit);     mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clUNLOCKED;
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
			//	calculate(j);//Rosto: mus� b�t posledn� p�ed j++, nelze ho d�t p�ed v�choz� za�krtnut� checkbox�
				//posun na dal�� ��dek v�sledn� tabulky

				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slou�en� bun�k pohony
			mGrid->MergeCells(2,j-z,3,j-z+O_pocet-1);//slou�en� bun�k 2-3

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK re�im
				{ //nelze nastavit hned, v horn� ��sti, spole�n� s typem Check, ale a� zde
					mGrid->getCheck(2,j-z)->Options->FrameNormalColor=C1;
					mGrid->getCheck(2,j-z)->OptionsChecked->FrameNormalColor=C1;
				}
			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m tlust�j��m or�mov�n�m////////
	mGrid->Cells[1][1].RightBorder->Width=mGrid->Cells[1][0].RightBorder->Width=2;
	mGrid->Cells[11][1].RightBorder->Width=mGrid->Cells[11][0].RightBorder->Width=2;
	mGrid->Cells[26][1].RightBorder->Width=mGrid->Cells[26][0].RightBorder->Width=2;
	mGrid->Cells[32][1].RightBorder->Width=mGrid->Cells[32][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[1][1],1,2,1,RowCount-2);
	mGrid->SetCells(mGrid->Cells[11][1],11,2,11,RowCount-2);
	mGrid->SetCells(mGrid->Cells[26][1],26,2,26,RowCount-2);
	mGrid->SetCells(mGrid->Cells[32][1],32,2,32,RowCount-2);
	mGrid->Cells[1][RowCount-1].RightBorder->Width=mGrid->Cells[1][1].RightBorder->Width;
	mGrid->Cells[11][RowCount-1].RightBorder->Width=mGrid->Cells[11][1].RightBorder->Width;
	mGrid->Cells[26][RowCount-1].RightBorder->Width=mGrid->Cells[26][1].RightBorder->Width;
	mGrid->Cells[32][RowCount-1].RightBorder->Width=mGrid->Cells[32][1].RightBorder->Width;

	////////autoresize formu_gapo, vhodn� nakonec,tj. p�ed Show////////
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10;// + 10 offset okolo tla��tka
	//pozice komponent
	F->m.designButton(scGPButton_OK,F_gapoV,1,2);
	F->m.designButton(scGPButton_storno,F_gapoV,2,2);

	////zobrazen� or�mov�n�
	zobrazitFrameForm=true;

	///uvoln�n� stavu
	input_state=FREE;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormPaint(TObject *Sender)
{
 	mGrid->Show();//vykresl� tabulku
	F->m.frameForm(this,clWebOrange,1);//vykresl� or�mov�n� okolo formu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TF_gapoV::OnClick(long Tag,unsigned long Col,unsigned long Row)
{

 	//ZAJIST�, �E MَE B�T ZAKLIKNUT MAX. 1 CHECKBOX NA ��DKU  a NELZE UDELAT UNCHECK
	if(Col>=2 && mGrid->getCheck(Col,Row)->Checked==false)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col,Row);
		CH->Checked=true;
		CH=NULL;delete CH;
	}



		if(Col==2 &&  mGrid->getCheck(Col,Row)->Checked /*&& mGrid->getCheck(Col,Row)->Options->FrameColor!=C1*/)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col+2,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col+4,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+6,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+8,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
	}

	if(Col==4 &&  mGrid->getCheck(Col,Row)->Checked)
	{

		if(mGrid->getCheck(Col,Row)->OptionsChecked->FrameColor < 1) //default �ed� barva je z�porn�, proto mohu nastavovat
		{
		TscGPCheckBox *CH=mGrid->getCheck(Col-2,Row);
		CH->Checked=false;
		}
		TscGPCheckBox *I=mGrid->getCheck(Col+2,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+4,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+6,Row);
		K->Checked=false;

		/*CH=NULL;delete CH;*/I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
	}

	if(Col==6 &&  mGrid->getCheck(Col,Row)->Checked /*&& mGrid->getCheck(Col,Row)->Options->FrameColor!=C1*/)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-4,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-2,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col+2,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+4,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
	}

	if(Col==8 &&  mGrid->getCheck(Col,Row)->Checked	/*&& mGrid->getCheck(Col,Row)->Options->FrameColor!=C1*/)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-6,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-4,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-2,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col+2,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
	}

	if(Col==10 &&  mGrid->getCheck(Col,Row)->Checked /*&& mGrid->getCheck(Col,Row)->Options->FrameColor!=C1*/)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-8,Row);
		CH->Checked=false;

		TscGPCheckBox *I=mGrid->getCheck(Col-6,Row);
		I->Checked=false;

		TscGPCheckBox *J=mGrid->getCheck(Col-4,Row);
		J->Checked=false;

		TscGPCheckBox *K=mGrid->getCheck(Col-2,Row);
		K->Checked=false;

		CH=NULL;delete CH;I=NULL;delete I;J=NULL;delete J;K=NULL;delete K;
	}

       //konec default nastaven� p�ep�na��
	///////////////////////////////////////////////////////////







	//Rosto, mus� b�t posledn� za klikac�/p�ep�nac� logikou!
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
	//	calculate(Row);//zajist� p�epo�et dan�ho ��dku
		FormPaint(this);//zajist� p�ekreslen� bez probliku
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
																		//Rosta zajistit p�evod
		pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999*Form_parametry_linky->Runit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	}
	else
	{
		pm.RD=objekty[Row].RD;
		pm.R=0;
  }
	//optimalizace detekce a uchov�n� volby za�krtnut�ho checkboxu, aby se nemuselo vyvol�vat znovu
	bool CHECK[5];
	CHECK[0]=mGrid->getCheck(2,Row)->Checked;
	CHECK[1]=mGrid->getCheck(4,Row)->Checked;
	CHECK[2]=mGrid->getCheck(6,Row)->Checked;
	CHECK[3]=mGrid->getCheck(8,Row)->Checked;
	CHECK[4]=mGrid->getCheck(10,Row)->Checked;

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
		if(F->pom!=NULL)//nutno jako samostan� if
		if(F->pom->n==objekty[Row].n)//je spu�t�n� PO a je tedy nutn� zavolat aktualizaci PO
		{
			F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vr�t� ostr� data
			F->NPin();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] objekty;
}
//---------------------------------------------------------------------------


