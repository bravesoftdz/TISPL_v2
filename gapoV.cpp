//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "gapoV.h"
#include "unit1.h"
#include "TmGrid.h"
#include "parametry_vozik.h"
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

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,F_gapoV,1,1);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky

	//definice barev NEWR
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
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
	CTunit=F->readINI("nastaveni_form_parametry", "CT").ToInt();
	RDunit=F->readINI("nastaveni_form_parametry", "RDt").ToInt();
	DDunit=F->readINI("nastaveni_form_parametry", "DD").ToInt();
	Munit =F->readINI("nastaveni_form_parametry", "DM").ToInt();

	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=2;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;

	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=28;//pevn� po�et slopc�
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	RowCount+=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false)+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(true);//P�IDAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
	mGrid->Cells[27][0].Text="N�hled";

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
	unsigned short n=18;if(RDunit)n=14;//��slo sloupce s nejdel��m textem hlavi�ky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
	//manualfit ���ky sloupc� mimo nult�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=23;//ostatn� n�sleduj�c� sloupce zat�m default ���ka

	//nastaven� velikosti nult�ho sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
	mGrid->SetColumnAutoFit(0);

	//slou�en� bun�k hlavi�ky PO  - vhodn� za SetColumnAutoFit um�st�t
	mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);mGrid->MergeCells(20,0,21,0);mGrid->MergeCells(22,0,23,0);mGrid->MergeCells(24,0,25,0);

	////////jednoliv� ��dky////////
	unsigned long j=1;//��slo aktu�ln� zpracov�van�ho ��dku, mus� za��nat 1, 0 - je hlavi�ka
	////pr�chod v�emi objekty bez p�i�azen�ch pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
	for(unsigned long i=0;i<On_pocet;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
		objekty[j]=On[i];
		//pohony
		mGrid->Cells[0][j].Text="nep�i�azen";mGrid->Cells[0][j].Background->Color=clBACKGROUND;
		//objekty
		mGrid->Cells[1][j].Text=On[i].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
		//volby - checkboxy
		mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
		mGrid->MergeCells(2,j,3,j);mGrid->MergeCells(4,j,5,j);//slou�en� sloupc�

		mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
		mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//slou�en� sloupc�

		mGrid->Cells[10][j].Type=mGrid->CHECK;
		mGrid->MergeCells(10,j,11,j);
		//parametry objekt�
		mGrid->Cells[12][j].Text=On[i].CT/(1+59.0*CTunit);                mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[12][j].Align=mGrid->LEFT; mGrid->Cells[9][j].Font->Color=clUNLOCKED;
		mGrid->Cells[14][j].Text=On[i].RD*(1+59.0*RDunit);                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[11][j].Font->Color=clUNLOCKED;
		mGrid->Cells[16][j].Text=On[i].delka_dopravniku*(1+999*DDunit);   mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[13][j].Font->Color=clUNLOCKED;
		mGrid->Cells[18][j].Text=On[i].kapacita;                          mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
		mGrid->Cells[20][j].Text=On[i].pozice;                            mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
		mGrid->Cells[22][j].Text=On[i].mezera_jig*(1+999*Munit);          mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
		mGrid->Cells[24][j].Text=On[i].mezera_podvozek*(1+999*Munit);     mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
		mGrid->Cells[26][j].Text=On[i].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
		mGrid->Cells[27][j].Type=mGrid->BUTTON;mGrid->Cells[27][j].Text="...";mGrid->Cells[27][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
		TscGPButton *B=mGrid->createButton(27,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
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
				mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
				mGrid->MergeCells(2,j,3,j);mGrid->MergeCells(4,j,5,j);//slou�en� sloupc�
				mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
				mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//slou�en� sloupc�
				mGrid->Cells[10][j].Type=mGrid->CHECK;
				mGrid->MergeCells(10,j,11,j);
				//parametry objekt�
				mGrid->Cells[12][j].Text=O[z].CT/(1+59.0*CTunit);                mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[12][j].Align=mGrid->LEFT; mGrid->Cells[9][j].Font->Color=clUNLOCKED;
				mGrid->Cells[14][j].Text=O[z].RD*(1+59.0*RDunit);                mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[11][j].Font->Color=clUNLOCKED;
				mGrid->Cells[16][j].Text=O[z].delka_dopravniku*(1+999*DDunit);   mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[13][j].Font->Color=clUNLOCKED;
				mGrid->Cells[18][j].Text=O[z].kapacita;                          mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				mGrid->Cells[20][j].Text=O[z].pozice;                            mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
				mGrid->Cells[22][j].Text=O[z].mezera_jig*(1+999*Munit);          mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clOLD;mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[24][j].Text=O[z].mezera_podvozek*(1+999*Munit);     mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clOLD;mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[26][j].Text=O[z].rotace;                            mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clOLD;
				mGrid->Cells[27][j].Type=mGrid->BUTTON;mGrid->Cells[27][j].Text="...";mGrid->Cells[27][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
				TscGPButton *B=mGrid->createButton(27,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
				calculate(j);//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot
				//posun na dal�� ��dek v�sledn� tabulky
				j++;
			}                    //Ros�a ov��it, �e je OK
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slou�en� bun�k pohony
			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m or�mov�n�m////////
	mGrid->Cells[1][1].RightBorder->Width=mGrid->Cells[1][0].RightBorder->Width=2;
	mGrid->Cells[11][1].RightBorder->Width=mGrid->Cells[11][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[1][1],1,2,1,RowCount-2);
	mGrid->SetCells(mGrid->Cells[11][1],11,2,11,RowCount-2);
	mGrid->Cells[1][RowCount-1].RightBorder->Width=mGrid->Cells[1][1].RightBorder->Width;
	mGrid->Cells[11][RowCount-1].RightBorder->Width=mGrid->Cells[11][1].RightBorder->Width;

	////////autoresize formu_gapo, vhodn� nakonec,tj. p�ed Show////////
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10;// + 10 offset okolo tla��tka
	scGPButton_OK->Top=Height-10-scGPButton_OK->Height;
	scGPButton_OK->Left=Width/2-scGPButton_OK->Width/2;
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
	pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/60.0);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	pm.DD=objekty[Row].delka_dopravniku;
	pm.K=objekty[Row].kapacita;
	pm.P=objekty[Row].pozice;
	pm.M=objekty[Row].mezera;
	pm.MJ=objekty[Row].mezera_jig;
	pm.MP=objekty[Row].mezera_podvozek;
	pm.dJ=F->ms.MyToDouble(Form_parametry_vozik->scGPNumericEdit_delka_jig);
	pm.sJ=F->ms.MyToDouble(Form_parametry_vozik->scGPNumericEdit_sirka_jig);
	pm.dP=F->ms.MyToDouble(Form_parametry_vozik->scGPNumericEdit_delka_podvozek);
	pm.Rotace=objekty[Row].rotace;
	pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999*Form_parametry_linky->Runit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat

	//vol�n� samotn�ho v�po�tu dle volby stanov�n� pomoci checkboxu
	//if(mGrid->getCheck(2,Row)->Checked)//m�n� se M,P, z�st�v� aRD, RD, Rz, Rx, R, CT,DD,K
	{
//		pm.CT_locked=false;pm.DD_locked=true;
//		pm.input_RD(true);
//		if(SaveTo==0)
//		{
//			mGrid->Cells[11][Row].Font->Color=clLOCKED;//DD
//			mGrid->Cells[7][Row].Font->Color=clUNLOCKED;//CT
//			mGrid->Cells[13][Row].Font->Color=clUNLOCKED;//K
//		}
	}
//	else//m�n� se RD,DD,P,M, z�st�v� CT,K
//	{
//		pm.CT_locked=true;pm.DD_locked=false;
//		pm.input_RD(true);
//		if(SaveTo==0)
//		{
//			mGrid->Cells[11][Row].Font->Color=clUNLOCKED;//DD
//			mGrid->Cells[7][Row].Font->Color =clLOCKED;//CT
//			mGrid->Cells[13][Row].Font->Color=clLOCKED;//K
//		}
//	}
//
//	//output sekce
//	AnsiString T="";
//	switch(SaveTo)
//	{
//		 case -1://ulo�en� do textu je-li po�adov�no
//		 {
//				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
//		 }break;
//		 case 0://pouze vr�t� text do bun�k
//		 {
//				mGrid->Cells[7][Row].Text	= F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
//				mGrid->Cells[9][Row].Text	=	F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
//				mGrid->Cells[11][Row].Text=	F->m.round2double(pm.DD*(1+999*DDunit),2,"..");
//				mGrid->Cells[13][Row].Text= F->m.round2double(pm.K,2,"..");
//				mGrid->Cells[15][Row].Text=	F->m.round2double(pm.P,2,"..");
//				mGrid->Cells[17][Row].Text=	F->m.round2double(pm.MJ*(1+999*Munit),2,"..");
//				mGrid->Cells[19][Row].Text=	F->m.round2double(pm.MP*(1+999*Munit),2,"..");
//		 }break;
//		 case 1://ulo�en� do spoj�ku OBJEKTY - je-li po�adov�no
//		 {
//				Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
//				O->CT=pm.CT;
//				O->RD=pm.RD;
//				O->delka_dopravniku=pm.DD;
//				O->kapacita=pm.K;
//				O->pozice=pm.P;
//				O->mezera=pm.M;
//				O->mezera_jig=pm.MJ;
//				O->mezera_podvozek=pm.MP;
//				O=NULL;delete O;
//		 }break;
//		 case 2://ulo�en� hodnot z ukazatele pro n�hled objektu
//		 {
//				Form_objekt_nahled->pom=new Cvektory::TObjekt;
//				//Form_objekt_nahled->pom->pohon=objekty[Row].pohon;//takto by p�evzal starou rozte�
//				Form_objekt_nahled->pom->pohon=new Cvektory::TPohon;
//				Form_objekt_nahled->pom->pohon->roztec=pm.R;//ale p�ed�v�m jen do n�hledu R, nic v�c od pohonu
//				Form_objekt_nahled->pom->rezim=objekty[Row].rezim;
//				Form_objekt_nahled->pom->CT=pm.CT;
//				Form_objekt_nahled->pom->RD=pm.RD;
//				Form_objekt_nahled->pom->delka_dopravniku=pm.DD;
//				Form_objekt_nahled->pom->kapacita=pm.K;
//				Form_objekt_nahled->pom->pozice=pm.P;
//				Form_objekt_nahled->pom->mezera=pm.M;
//				Form_objekt_nahled->pom->mezera_jig=pm.MJ;
//				Form_objekt_nahled->pom->mezera_podvozek=pm.MP;
//		 }break;
//	}
//	return T;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPButton_OKClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] objekty;
}
//---------------------------------------------------------------------------


