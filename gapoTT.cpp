//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gapoTT.h"
#include "unit1.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TF_gapoTT *F_gapoTT;
//---------------------------------------------------------------------------
__fastcall TF_gapoTT::TF_gapoTT(TComponent* Owner)
	: TForm(Owner)
{
	//nastaven� barvy formul��e
	F_gapoTT->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK
	F->m.designButton(scGPButton_OK,F_gapoTT,1,1);

	Offset=10;//odsazen� thoto formu po v�ech str�n�ch od tabulky

}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormShow(TObject *Sender)
{
	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=1;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;

	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=23;//pevn� po�et slopc�
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	RowCount+=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false)+F->d.v.vrat_pocet_nepouzivanych_pohonu()+F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(true);//P�IDAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	mGrid->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
	objekty=new Cvektory::TObjekt[ColCount];//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty

	////////pln�n� daty - hlavi�ka////////
	mGrid->Cells[0][0].Text="pohon";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnut� tu�n�ho p�sma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[1][0].Text="objekt";
	mGrid->Cells[2][0].Text="re�im";
	mGrid->Cells[3][0].Text="aRD, RD, CT";
	mGrid->Cells[4][0].Text="DD, K, P, Rz, Rx, R, M";
	mGrid->Cells[5][0].Text="aRD, RD, DD, K, P";
	mGrid->Cells[6][0].Text="CT, Rz, Rx, R, M";
	mGrid->Cells[7][0].Text="Rz, Rx, (R), M, DD, P, CT";
	mGrid->Cells[8][0].Text="Rz, Rx, (R), M, DD, P, CT";
	mGrid->Cells[9][0].Text="Rz, Rx, (R), M, DD, P, CT";
	mGrid->Cells[10][0].Text="aRD, RD, DD, CT";
	mGrid->Cells[11][0].Text="Rz, Rx, M, DD, P, CT";
	mGrid->Cells[12][0].Text="aRD, RD, R,  DD, CT";
	mGrid->Cells[13][0].Text="Rz, R, M, DD, P, CT";
	mGrid->Cells[14][0].Text="aRD, RD, Rz,  DD, CT";

	mGrid->Cells[3][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[3][0].Font->Orientation=900;
	mGrid->Cells[3][0].Align=mGrid->CENTER;
	mGrid->Cells[3][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[3][0].BottomMargin=4;mGrid->Cells[3][0].TopMargin=8;

	mGrid->Cells[4][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[4][0].Font->Orientation=900;
	mGrid->Cells[4][0].Align=mGrid->CENTER;
	mGrid->Cells[4][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[4][0].BottomMargin=4;mGrid->Cells[4][0].TopMargin=8;


	mGrid->Cells[5][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[5][0].Font->Orientation=900;
	mGrid->Cells[5][0].Align=mGrid->CENTER;
	mGrid->Cells[5][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[5][0].BottomMargin=4;mGrid->Cells[5][0].TopMargin=8;

	mGrid->Cells[6][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[6][0].Font->Orientation=900;
	mGrid->Cells[6][0].Align=mGrid->CENTER;
	mGrid->Cells[6][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[6][0].BottomMargin=4;mGrid->Cells[6][0].TopMargin=8;

	mGrid->Cells[7][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[7][0].Font->Orientation=900;
	mGrid->Cells[7][0].Align=mGrid->CENTER;
	mGrid->Cells[7][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[7][0].BottomMargin=4;mGrid->Cells[7][0].TopMargin=8;

	mGrid->Cells[8][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[8][0].Font->Orientation=900;
	mGrid->Cells[8][0].Align=mGrid->CENTER;
	mGrid->Cells[8][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[8][0].BottomMargin=4;mGrid->Cells[8][0].TopMargin=8;

	mGrid->Cells[9][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[9][0].Font->Orientation=900;
	mGrid->Cells[9][0].Align=mGrid->CENTER;
	mGrid->Cells[9][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[9][0].BottomMargin=4;mGrid->Cells[9][0].TopMargin=8;

	mGrid->Cells[10][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[10][0].Font->Orientation=900;
	mGrid->Cells[10][0].Align=mGrid->CENTER;
	mGrid->Cells[10][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[10][0].BottomMargin=4;mGrid->Cells[10][0].TopMargin=8;

	mGrid->Cells[11][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[11][0].Font->Orientation=900;
	mGrid->Cells[11][0].Align=mGrid->CENTER;
	mGrid->Cells[11][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[11][0].BottomMargin=4;mGrid->Cells[11][0].TopMargin=8;

	mGrid->Cells[12][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[12][0].Font->Orientation=900;
	mGrid->Cells[12][0].Align=mGrid->CENTER;
	mGrid->Cells[12][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[12][0].BottomMargin=4;mGrid->Cells[12][0].TopMargin=8;

	mGrid->Cells[13][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[13][0].Font->Orientation=900;
	mGrid->Cells[13][0].Align=mGrid->CENTER;
	mGrid->Cells[13][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[13][0].BottomMargin=4;mGrid->Cells[13][0].TopMargin=8;

	mGrid->Cells[14][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[14][0].Font->Orientation=900;
	mGrid->Cells[14][0].Align=mGrid->CENTER;
	mGrid->Cells[14][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[14][0].BottomMargin=4;mGrid->Cells[14][0].TopMargin=8;


	mGrid->Cells[15][0].Text="CT - Technologick� �as [s]"; //mGrid->MergeCells(6,0,7,0);//slou�en� zat�m nefunguje dob�e
	mGrid->Cells[16][0].Text="RD - Rychlost pohonu [m/min]";
	mGrid->Cells[17][0].Text="DD - D�lka objekt [m]";
	mGrid->Cells[18][0].Text="K - Kapacita [voz�k�+mezer]";
	mGrid->Cells[19][0].Text="P - Pozice [voz�k�]";
	mGrid->Cells[20][0].Text="M - mezera jig [m]";
	mGrid->Cells[21][0].Text="M - mezera voz�k [m]";
	mGrid->Cells[22][0].Text="Rotace";

	////////p�i�ad� cel� oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky////////
	mGrid->SetCells(mGrid->Cells[0][0],15,0,ColCount-1,0);//pro prvn� ��dek
	mGrid->SetCells(mGrid->Cells[0][0],1,0,3,0);//pro prvn� ��dek
//	//v tomto p��pad� �ed� sloupec (stejn� jako or�mov�n�) jako nem�n� hodnoty - nedo�e�en� probl�m s posunem v AA
//	mGrid->Cells[ColCount-1][1].Background->Color=(TColor)RGB(240,240,240);
//	mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);
//
//	//manualfit v��ky 0-t�ho ��dku (zat�m nen� p�ipravena metoda)
	Canvas->Font=mGrid->Cells[16][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[16][0].Text)+mGrid->Cells[16][0].BottomMargin+mGrid->Cells[16][0].BottomBorder->Width/2+mGrid->Cells[16][0].TopMargin+mGrid->Cells[16][0].TopBorder->Width/2;
//	//manualfit ���ky sloupc� mimo prvn�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=100;mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=mGrid->Columns[12].Width=mGrid->Columns[13].Width=mGrid->Columns[14].Width=35;//ostatn� n�sleduj�c� sloupce zat�m default ���ka
	mGrid->Columns[2].Width=100;
	//
//	//nastaven� velikosti nult�ho sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
	mGrid->SetColumnAutoFit(0);


 ////////jednoliv� ��dky////////
	unsigned long j=1;//��slo aktu�ln� zpracov�van�ho ��dku, mus� za��nat 1, 0 - je hlavi�ka
	////pr�chod v�emi objekty bez p�i�azen�ch pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
	for(unsigned long i=1;i<On_pocet;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
			//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
			objekty[j]=On[i];
			//pohony
			mGrid->Cells[0][j].Text="nep�i�azen";
			//objekty
			mGrid->Cells[1][j].Text=On[i].short_name;//On[i].short_name;
			//volby - checkboxy  - Rosta dodel�
				mGrid->Cells[3][j].Type=mGrid->CHECK;mGrid->Cells[5][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);mGrid->MergeCells(5,j,6,j);//slou�en� sloupc�

				mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->Cells[9][j].Type=mGrid->CHECK;
				mGrid->MergeCells(7,j,8,j);mGrid->MergeCells(9,j,10,j);//slou�en� sloupc�

				mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->Cells[13][j].Type=mGrid->CHECK;
				mGrid->MergeCells(11,j,12,j);mGrid->MergeCells(13,j,14,j);//slou�en� sloupc�

			//parametry objekt�
			mGrid->Cells[15][j].Text=On[i].CT;
			mGrid->Cells[16][j].Text=On[i].RD;
			mGrid->Cells[17][j].Text=On[i].delka_dopravniku;
			mGrid->Cells[18][j].Text=On[i].kapacita;
			mGrid->Cells[19][j].Text=On[i].pozice;
			mGrid->Cells[20][j].Text=On[i].mezera_jig;
			mGrid->Cells[21][j].Text=On[i].mezera_podvozek;
			mGrid->Cells[22][j].Text=On[i].rotace;
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
			//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
			objekty[j].id=0;//p�i�ad� id 0, ne�lo napsat p��mo NULL, nutno po�e�it p�i pr�chodu cyklem n�sledn�ho vypisov�n� objekty[x]
			//pohony
			mGrid->Cells[0][j].Text=F->d.v.vrat_pohon(i)->name;
			//objekty
			mGrid->Cells[1][j].Text="nep�i�azen";
			//volby - checkboxy - Rost� dod�l�

				mGrid->Cells[3][j].Type=mGrid->CHECK;mGrid->Cells[5][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);mGrid->MergeCells(5,j,6,j);//slou�en� sloupc�

				mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->Cells[9][j].Type=mGrid->CHECK;
				mGrid->MergeCells(7,j,8,j);mGrid->MergeCells(9,j,10,j);//slou�en� sloupc�

				mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->Cells[13][j].Type=mGrid->CHECK;
				mGrid->MergeCells(11,j,12,j);mGrid->MergeCells(13,j,14,j);//slou�en� sloupc�

			//posun na dal�� ��dek v�sledn� tabulky
			j++;
		}
		else
		{
			//vrat� formou ukazatele na pole objekty p�i�azen� k dan�mu pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i);
			AnsiString rezim;
			unsigned long z=0;
			for(;z<O_pocet;z++)
			{
				 //pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
				 objekty[j]=O[z];
				 //pohony
				 mGrid->Cells[0][j].Text=O[z].pohon->name;
				 //objekty
				 mGrid->Cells[1][j].Text=O[z].short_name;
				 if(O[z].rezim==0) rezim="S&G";
				 if(O[z].rezim==1) rezim="Kontinu�ln�";
				 if(O[z].rezim==2) rezim="Postprocesn�";
				 mGrid->Cells[2][j].Text=rezim;
				 //volby - checkboxy
				mGrid->Cells[3][j].Type=mGrid->CHECK;mGrid->Cells[5][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);mGrid->MergeCells(5,j,6,j);//slou�en� sloupc�
			//	 if(O[z].rezim=!1)
		 //		{
				mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->Cells[9][j].Type=mGrid->CHECK;
				mGrid->MergeCells(7,j,8,j);mGrid->MergeCells(9,j,10,j);//slou�en� sloupc�

				mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->Cells[13][j].Type=mGrid->CHECK;
				mGrid->MergeCells(11,j,12,j);mGrid->MergeCells(13,j,14,j);//slou�en� sloupc�
			//	}

				 //parametry objekt�
				 mGrid->Cells[15][j].Text=O[z].CT;
				 mGrid->Cells[16][j].Text=O[z].RD*60.0;
				 mGrid->Cells[17][j].Text=O[z].delka_dopravniku;
				 mGrid->Cells[18][j].Text=O[z].kapacita;
				 mGrid->Cells[19][j].Text=O[z].pozice;
				 mGrid->Cells[20][j].Text=O[z].mezera_jig;
				 if(O[z].mezera_podvozek<0.00000004) mGrid->Cells[21][j].Text="0";
			  	else mGrid->Cells[21][j].Text=O[z].mezera_podvozek;
				 mGrid->Cells[22][j].Text=O[z].rotace;
				 //posun na dal�� ��dek v�sledn� tabulky
				 j++;
      }                    //Ros�a ov��it, �e je OK
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slou�en� bun�k pohony
			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m or�mov�n�m////////
	mGrid->Cells[2][1].RightBorder->Width=mGrid->Cells[2][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[2][1],2,2,2,RowCount-2);
	mGrid->Cells[2][RowCount-1].RightBorder->Width=mGrid->Cells[2][1].RightBorder->Width;
//
//	////////autoresize formu_gapo, vhodn� nakonec,tj. p�ed Show////////
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10;// + 10 offset okolo tla��tka
	scGPButton_OK->Top=Height-10-scGPButton_OK->Height;
	scGPButton_OK->Left=Width/2-scGPButton_OK->Width/2;
 //	Button1->Top=scGPButton_OK->Top;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykresl� tabulku
	F->m.frameForm(this,clWebOrange,1);//vykresl� or�mov�n� okolo formu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TF_gapoTT::OnClick(long Tag,unsigned long Col,unsigned long Row)
{

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
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] objekty;
}
//---------------------------------------------------------------------------

