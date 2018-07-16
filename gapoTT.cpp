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
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "rHintWindow"
#pragma link "rHTMLLabel"
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


	//definice barev NEWR
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);

	//legenda NEWR
	rHTMLLabel_legenda_titulek->FontColor=(TColor)RGB(43,87,154);
	rHTMLLabel_legenda->FontColor=clUNLOCKED;

	// nastaven� barvy or�mov�n� v hlavi�ce tabulky + or�mov�n� checkbox�
	 C1=Form1->m.clIntensive(RGB(128,64,0),20);
	 C2=Form1->m.clIntensive(RGB(0,128,0),80);

}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormShow(TObject *Sender)
{
	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=1;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;//NEWR
	input_state=LOADING;

	//workaround odchyt�v�n� stisku kl�ves NEWR
	Edit1->SetFocus();

		////////jednotky////////  NEWR
	CTunit=F->readINI("nastaveni_form_parametry", "CT").ToInt();
	RDunit=F->readINI("nastaveni_form_parametry", "RDt").ToInt();
	DDunit=F->readINI("nastaveni_form_parametry", "DD").ToInt();
	Munit =F->readINI("nastaveni_form_parametry", "DM").ToInt();



	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=32;//pevn� po�et slopc�      //NEWR
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
	mGrid->Cells[4][0].Background->Color=clBACKGROUND;


	mGrid->Cells[5][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[5][0].Font->Orientation=900;
	mGrid->Cells[5][0].Align=mGrid->CENTER;
	mGrid->Cells[5][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[5][0].BottomMargin=4;mGrid->Cells[5][0].TopMargin=8;
	mGrid->Cells[5][0].Background->Color=clBACKGROUND;

	mGrid->Cells[6][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[6][0].Font->Orientation=900;
	mGrid->Cells[6][0].Align=mGrid->CENTER;
	mGrid->Cells[6][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[6][0].BottomMargin=4;mGrid->Cells[6][0].TopMargin=8;
	mGrid->Cells[6][0].Background->Color=clBACKGROUND;

	mGrid->Cells[7][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[7][0].Font->Orientation=900;
	mGrid->Cells[7][0].Align=mGrid->CENTER;
	mGrid->Cells[7][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[7][0].BottomMargin=4;mGrid->Cells[7][0].TopMargin=8;
	mGrid->Cells[7][0].Background->Color=clBACKGROUND;

	mGrid->Cells[8][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[8][0].Font->Orientation=900;
	mGrid->Cells[8][0].Align=mGrid->CENTER;
	mGrid->Cells[8][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[8][0].BottomMargin=4;mGrid->Cells[8][0].TopMargin=8;
	mGrid->Cells[8][0].Background->Color=clBACKGROUND;

	mGrid->Cells[9][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[9][0].Font->Orientation=900;
	mGrid->Cells[9][0].Align=mGrid->CENTER;
	mGrid->Cells[9][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[9][0].BottomMargin=4;mGrid->Cells[9][0].TopMargin=8;
	mGrid->Cells[9][0].Background->Color=clBACKGROUND;

	mGrid->Cells[10][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[10][0].Font->Orientation=900;
	mGrid->Cells[10][0].Align=mGrid->CENTER;
	mGrid->Cells[10][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[10][0].BottomMargin=4;mGrid->Cells[10][0].TopMargin=8;
	mGrid->Cells[10][0].Background->Color=clBACKGROUND;

	mGrid->Cells[11][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[11][0].Font->Orientation=900;
	mGrid->Cells[11][0].Align=mGrid->CENTER;
	mGrid->Cells[11][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[11][0].BottomMargin=4;mGrid->Cells[11][0].TopMargin=8;
	mGrid->Cells[11][0].Background->Color=clBACKGROUND;

	mGrid->Cells[12][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[12][0].Font->Orientation=900;
	mGrid->Cells[12][0].Align=mGrid->CENTER;
	mGrid->Cells[12][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[12][0].BottomMargin=4;mGrid->Cells[12][0].TopMargin=8;
	mGrid->Cells[12][0].Background->Color=clBACKGROUND;

	mGrid->Cells[13][0].Font->Color=(TColor)RGB(43,87,154);
	mGrid->Cells[13][0].Font->Orientation=900;
	mGrid->Cells[13][0].Align=mGrid->CENTER;
	mGrid->Cells[13][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[13][0].BottomMargin=4;mGrid->Cells[13][0].TopMargin=8;
	mGrid->Cells[13][0].Background->Color=clBACKGROUND;

	mGrid->Cells[14][0].Font->Color=(TColor)RGB(128,128,128);
	mGrid->Cells[14][0].Font->Orientation=900;
	mGrid->Cells[14][0].Align=mGrid->CENTER;
	mGrid->Cells[14][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[14][0].BottomMargin=4;mGrid->Cells[14][0].TopMargin=8;
	mGrid->Cells[14][0].Background->Color=clBACKGROUND;


	if(CTunit)mGrid->Cells[15][0].Text="CT - Technologick� �as [min]";else mGrid->Cells[15][0].Text="CT - Technologick� �as [s]";
	if(RDunit)mGrid->Cells[17][0].Text="RD - Rychlost pohonu [m/min]";else mGrid->Cells[17][0].Text="RD - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[19][0].Text="DD - D�lka objekt [mm]";else mGrid->Cells[19][0].Text="DD - D�lka objekt [m]";
	mGrid->Cells[21][0].Text="K - Kapacita [voz�k�+mezer]";
	mGrid->Cells[23][0].Text="P - Pozice [voz�k�]";
	mGrid->Cells[25][0].Text="M - mezera jig [m]";
	mGrid->Cells[27][0].Text="M - mezera voz�k [m]";
	mGrid->Cells[29][0].Text="Rotace";
	mGrid->Cells[30][0].Text="n�hled";
	mGrid->Cells[31][0].Text="ID_pohon";
	////////p�i�ad� cel� oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky////////
	mGrid->SetCells(mGrid->Cells[0][0],15,0,ColCount-1,0);//pro prvn� ��dek   sloupce 15+
	mGrid->SetCells(mGrid->Cells[0][0],1,0,3,0);//pro prvn� ��dek   sloupce 1-3
	//v tomto p��pad� �ed� sloupec (stejn� jako or�mov�n�) jako nem�n� hodnoty - nedo�e�en� probl�m s posunem v AA
	//mGrid->Cells[ColCount-1][1].Background->Color=(TColor)RGB(200,240,240);
//	mGrid->SetCells(mGrid->Cells[ColCount-1][1],ColCount-1,2,ColCount-1,RowCount-1);

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
	unsigned short n=15;if(RDunit)n=17;//��slo sloupce s nejdel��m textem hlavi�ky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[17][0].Text)+mGrid->Cells[17][0].BottomMargin+mGrid->Cells[17][0].BottomBorder->Width/2+mGrid->Cells[17][0].TopMargin+mGrid->Cells[17][0].TopBorder->Width/2;
	//manualfit ���ky sloupc� mimo prvn�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=90;mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=mGrid->Columns[12].Width=mGrid->Columns[13].Width=mGrid->Columns[14].Width=23;//ostatn� n�sleduj�c� sloupce zat�m default ���ka
	mGrid->Columns[2].Width=120;

	//nastaven� velikosti nult�ho sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
	mGrid->SetColumnAutoFit(0);

		//slou�en� bun�k hlavi�ky PO  - vhodn� za SetColumnAutoFit um�st�t - NEWR
	mGrid->MergeCells(15,0,16,0);mGrid->MergeCells(17,0,18,0);mGrid->MergeCells(19,0,20,0);
	mGrid->MergeCells(21,0,22,0);mGrid->MergeCells(23,0,24,0);mGrid->MergeCells(25,0,26,0);mGrid->MergeCells(27,0,28,0);


	////////jednoliv� ��dky////////
	AnsiString rezimJ;
	unsigned long j=1;//��slo aktu�ln� zpracov�van�ho ��dku, mus� za��nat 1, 0 - je hlavi�ka
	////pr�chod v�emi objekty bez p�i�azen�ch pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false);
	for(unsigned long i=1;i<=On_pocet;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{  //OBJEKTY BEZ P�I�AZEN�HO POHONU
		//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
		objekty[j]=On[i];
		//pohony
		mGrid->Cells[0][j].Text="nep�i�azen";
		//objekty
		mGrid->Cells[1][j].Text=On[i].short_name;
		//re�im
		if(On[i].rezim==0) rezimJ="S&G";
		if(On[i].rezim==1) rezimJ="Kontinu�ln�";
		if(On[i].rezim==2) rezimJ="Postprocesn�";
	 //	else rezimJ="-";
		mGrid->Cells[2][j].Text=rezimJ;

		//volby - checkboxy
				mGrid->Cells[3][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);

			if(On[i].rezim!=0)   //povolen� zobrazen� checkbox� pro KK a PP re�im
				{
				mGrid->Cells[5][j].Type=mGrid->CHECK;mGrid->MergeCells(5,j,6,j);
				mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->MergeCells(7,j,8,j);
				mGrid->Cells[9][j].Type=mGrid->CHECK;mGrid->MergeCells(9,j,10,j);
				mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->MergeCells(11,j,12,j);
				mGrid->Cells[13][j].Type=mGrid->CHECK;mGrid->MergeCells(13,j,14,j);
				}
				else
				{     // u SG slou��m pouze bu�ky, ale checkbox bude nab�zen pouze u prvn�ho "v�b�rov�ho" sloupce
				mGrid->MergeCells(5,j,6,j);
				mGrid->MergeCells(7,j,8,j);
				mGrid->MergeCells(9,j,10,j);
				mGrid->MergeCells(11,j,12,j);
				mGrid->MergeCells(13,j,14,j);
				}

//
 //			parametry objekt�           /*NEWR*/                            //NEWR
				mGrid->Cells[15][j].Text=F->m.round2double(On[i].CT/(1+59.0*CTunit),2,"..");	 								mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clOLD;mGrid->Cells[16][j].Align=mGrid->LEFT; mGrid->Cells[16][j].Font->Color=clUNLOCKED;
				mGrid->Cells[17][j].Text=F->m.round2double(On[i].RD*(1+59.0*RDunit),2,"..");                 mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clOLD;mGrid->Cells[18][j].Align=mGrid->LEFT; mGrid->Cells[18][j].Font->Color=clUNLOCKED;
				mGrid->Cells[19][j].Text=F->m.round2double(On[i].delka_dopravniku*(1+999*DDunit),2,"..");    	mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clOLD;mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clUNLOCKED;
				mGrid->Cells[21][j].Text=F->m.round2double(On[i].kapacita,2,"..");                          	mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clOLD;mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clUNLOCKED;
				mGrid->Cells[23][j].Text=F->m.round2double(On[i].pozice,2,"..");                             	mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clOLD;mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clUNLOCKED;
				mGrid->Cells[25][j].Text=F->m.round2double(On[i].mezera_jig*(1+999*Munit),2,"..");           	mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clOLD;mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clUNLOCKED;
				mGrid->Cells[27][j].Text=F->m.round2double(On[i].mezera_podvozek*(1+999*Munit),2,"..");  			mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
				mGrid->Cells[29][j].Text=AnsiString(On[i].rotace);             			mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;
				mGrid->Cells[30][j].Type=mGrid->BUTTON;mGrid->Cells[30][j].Text="...";mGrid->Cells[30][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
				TscGPButton *B=mGrid->createButton(30,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
				/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		 //		B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//pad�

		//		calculate(j);//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot NEWR


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
			//volby - checkboxy
			mGrid->Cells[3][j].Type=mGrid->CHECK;
			mGrid->Cells[5][j].Type=mGrid->CHECK;

			 //	mGrid->MergeCells(1,j,2,j);


				mGrid->MergeCells(3,j,4,j);
				mGrid->MergeCells(5,j,6,j);//slou�en� sloupc�
				mGrid->MergeCells(3,j,5,j);

				//mGrid->Cells[3][j].Align=mGrid->RIGHT;

			mGrid->Cells[7][j].Type=mGrid->CHECK;
			mGrid->Cells[9][j].Type=mGrid->CHECK;
			mGrid->MergeCells(7,j,8,j);
			mGrid->MergeCells(9,j,10,j);//slou�en� sloupc�


			mGrid->Cells[11][j].Type=mGrid->CHECK;
			mGrid->Cells[13][j].Type=mGrid->CHECK;
			mGrid->MergeCells(11,j,12,j);
			mGrid->MergeCells(13,j,14,j);//slou�en� sloupc�


					TscGPCheckBox *CH=mGrid->getCheck(3,j);
					CH->Checked=true;
					CH=NULL;delete CH;

					mGrid->getCheck(11,j)->Visible=false;
					mGrid->getCheck(13,j)->Visible=false;


			//posun na dal�� ��dek v�sledn� tabulky
			j++;
		}
		else    //OBJEKTY S P�I�AZEN�MI POHONY
		{
			//vrat� formou ukazatele na pole objekty p�i�azen� k dan�mu pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i);
			AnsiString rezimZ;
			unsigned long z=0;
			for(;z<O_pocet;z++)
			{
				//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;
				if(O[z].rezim==0)  rezimZ="S&G";
				if(O[z].rezim==1) {rezimZ="Kontinu�ln�";}
				if(O[z].rezim==2)  rezimZ="Postprocesn�";
			 //	else rezimZ="-";
				mGrid->Cells[2][j].Text=rezimZ;
				 //volby - checkboxy
				mGrid->Cells[3][j].Type=mGrid->CHECK;
				mGrid->MergeCells(3,j,4,j);

				 if(O[z].rezim!=0)   //povolen� zobrazen� checkbox� pro KK a PP re�im
				{
				mGrid->Cells[5][j].Type=mGrid->CHECK;mGrid->MergeCells(5,j,6,j);
				mGrid->Cells[7][j].Type=mGrid->CHECK;mGrid->MergeCells(7,j,8,j);
				mGrid->Cells[9][j].Type=mGrid->CHECK;mGrid->MergeCells(9,j,10,j);
				mGrid->Cells[11][j].Type=mGrid->CHECK;mGrid->MergeCells(11,j,12,j);
				mGrid->Cells[13][j].Type=mGrid->CHECK;mGrid->MergeCells(13,j,14,j);
				}
				else

				{     // u SG slou��m pouze bu�ky, ale checkbox bude nab�zen pouze u prvn�ho "v�b�rov�ho" sloupce
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
//
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
					mGrid->getCheck(11,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(13,j)->Options->FrameNormalColor=C2;
					mGrid->getCheck(7,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(9,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(11,j)->OptionsChecked->FrameNormalColor=C2;
					mGrid->getCheck(13,j)->OptionsChecked->FrameNormalColor=C2;
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

	 //			parametry objekt�           /*NEWR*/                            //NEWR
				mGrid->Cells[15][j].Text=F->m.round2double(O[z].CT/(1+59.0*CTunit),2,"..");	 							 	mGrid->Cells[15][j].Align=mGrid->LEFT;	mGrid->Cells[15][j].Font->Color=clOLD;	mGrid->Cells[16][j].Align=mGrid->LEFT; mGrid->Cells[16][j].Font->Color=clUNLOCKED;
				mGrid->Cells[17][j].Text=F->m.round2double(O[z].RD*(1+59.0*RDunit),2,"..");                mGrid->Cells[17][j].Align=mGrid->LEFT;	mGrid->Cells[17][j].Font->Color=clOLD;	mGrid->Cells[18][j].Align=mGrid->LEFT; mGrid->Cells[18][j].Font->Color=clUNLOCKED;
				mGrid->Cells[19][j].Text=F->m.round2double(O[z].delka_dopravniku*(1+999*DDunit),2,"..");    mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clOLD;mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clUNLOCKED;
				mGrid->Cells[21][j].Text=F->m.round2double(O[z].kapacita,2,"..");                           mGrid->Cells[21][j].Align=mGrid->LEFT;mGrid->Cells[21][j].Font->Color=clOLD;mGrid->Cells[22][j].Align=mGrid->LEFT;mGrid->Cells[22][j].Font->Color=clUNLOCKED;
				mGrid->Cells[23][j].Text=F->m.round2double(O[z].pozice,2,"..");                            mGrid->Cells[23][j].Align=mGrid->LEFT;mGrid->Cells[23][j].Font->Color=clOLD;mGrid->Cells[24][j].Align=mGrid->LEFT;mGrid->Cells[24][j].Font->Color=clUNLOCKED;
				mGrid->Cells[25][j].Text=F->m.round2double(O[z].mezera_jig*(1+999*Munit),2,"..");           mGrid->Cells[25][j].Align=mGrid->LEFT;mGrid->Cells[25][j].Font->Color=clOLD;mGrid->Cells[26][j].Align=mGrid->LEFT;mGrid->Cells[26][j].Font->Color=clUNLOCKED;
				mGrid->Cells[27][j].Text=F->m.round2double(O[z].mezera_podvozek*(1+999*Munit),2,"..");  		 	mGrid->Cells[27][j].Align=mGrid->LEFT;mGrid->Cells[27][j].Font->Color=clOLD;mGrid->Cells[28][j].Align=mGrid->LEFT;mGrid->Cells[28][j].Font->Color=clUNLOCKED;
				mGrid->Cells[29][j].Text=AnsiString(O[z].rotace);             	 	mGrid->Cells[29][j].Align=mGrid->LEFT;mGrid->Cells[29][j].Font->Color=clOLD;
				mGrid->Cells[30][j].Type=mGrid->BUTTON;mGrid->Cells[30][j].Text="...";mGrid->Cells[30][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
				TscGPButton *B=mGrid->createButton(30,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
				/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
			//	B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//pad�
				mGrid->Cells[31][j].Text=O[z].pohon->n;
//
		//		calculate(j);//zajist� p�epo�et dan�ho ��dku - nov�ch hodnot NEWR

				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slou�en� bun�k pohony
			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m or�mov�n�m////////

	mGrid->Cells[2][1].RightBorder->Width=mGrid->Cells[2][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[2][1],2,2,2,RowCount-2);
	mGrid->Cells[2][RowCount-1].RightBorder->Width=mGrid->Cells[2][1].RightBorder->Width;

	 for(int i=1;i<=RowCount-1;i++){
			mGrid->Cells[14][i].RightBorder->Width=2;
			mGrid->Cells[0][i].Background->Color=clBACKGROUND;
			mGrid->Cells[0][i].BottomBorder->Color=clBACKGROUND;
			mGrid->Cells[1][i].Background->Color=clBACKGROUND;
			mGrid->Cells[2][i].Background->Color=clBACKGROUND;

	 }



	mGrid->Cells[2][0].RightBorder->Color=C1;
	mGrid->Cells[3][0].BottomBorder->Color=mGrid->Cells[4][0].BottomBorder->Color=mGrid->Cells[5][0].BottomBorder->Color=mGrid->Cells[6][0].BottomBorder->Color=C1;
	mGrid->Cells[3][0].BottomBorder->Width=mGrid->Cells[4][0].BottomBorder->Width= mGrid->Cells[5][0].BottomBorder->Width=mGrid->Cells[6][0].BottomBorder->Width=2;

	mGrid->Cells[6][0].RightBorder->Color=C1;
	mGrid->Cells[6][0].RightBorder->Width=2;

	mGrid->Cells[7][0].BottomBorder->Width=	mGrid->Cells[8][0].BottomBorder->Width=mGrid->Cells[9][0].BottomBorder->Width= mGrid->Cells[10][0].BottomBorder->Width=mGrid->Cells[11][0].BottomBorder->Width=mGrid->Cells[12][0].BottomBorder->Width=mGrid->Cells[13][0].BottomBorder->Width=mGrid->Cells[14][0].BottomBorder->Width=2;
	mGrid->Cells[7][0].BottomBorder->Color=	mGrid->Cells[8][0].BottomBorder->Color=mGrid->Cells[9][0].BottomBorder->Color= mGrid->Cells[10][0].BottomBorder->Color=mGrid->Cells[11][0].BottomBorder->Color=mGrid->Cells[12][0].BottomBorder->Color=mGrid->Cells[13][0].BottomBorder->Color=mGrid->Cells[14][0].BottomBorder->Color=C2;

	mGrid->Cells[14][0].RightBorder->Width=2;
	mGrid->Cells[14][0].RightBorder->Color=C2;

	mGrid->Cells[3][0].TopBorder->Color=C1;
	mGrid->Cells[3][0].TopBorder->Width=3;

//

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
	Height=mGrid->Height+Offset*2+rHTMLLabel_InfoText->Height+scGPPanel_hlavicka->Height+11+scGPButton_OK->Height+11;// + 11 offset okolo tla��tka
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


//	////////autoresize formu_gapo, vhodn� nakonec,tj. p�ed Show////////
	Width=mGrid->Width+Offset*2;
	Height=mGrid->Height+Offset*2+scGPPanel_hlavicka->Height+10+scGPButton_OK->Height+10+rHTMLLabel_legenda->Height;// + 10 offset okolo tla��tka
  //NEWR
	F->m.designButton(scGPButton_OK,F_gapoTT,1,2);
	F->m.designButton(scGPButton_storno,F_gapoTT,2,2);
	//NEWR   legenda pozice
	rHTMLLabel_InfoText->Top=mGrid->Top+mGrid->Height+1;//+1 kv�li or�mov�n� tabulky
	rHTMLLabel_legenda_titulek->Top=rHTMLLabel_InfoText->Top;rHTMLLabel_legenda_titulek->Left=Width-rHTMLLabel_legenda->Width-Offset/2;
	rHTMLLabel_legenda->Top=rHTMLLabel_legenda_titulek->Top+rHTMLLabel_legenda_titulek->Height;rHTMLLabel_legenda->Left=rHTMLLabel_legenda_titulek->Left;

		////pozice gapo formu
	Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Width/2;
	Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_parametry_linky->scGPPanel2->Height/2-Height/2;//um�st� na polovinu PL formul��e
	////zobrazen� or�mov�n�
	zobrazitFrameForm=true;
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
		if(Col>=3 && mGrid->getCheck(Col,Row)->Checked==false){
			TscGPCheckBox *CH=mGrid->getCheck(Col,Row);
			CH->Checked=true;
			CH=NULL;delete CH;
		}

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

	////////////////////////////////////////////////////////////////////////////////////

		 // mechanismus na povolen� vstupu do druh� oblasti tzn aktivace sloupc� 9 a� 13 v p��pad�, �e
		 //objekty v KK re�imu nemaj� ��dn� vybran� sloupec 3 nebo 5

	 //	 objekty[Row].pohon->n;

		 // pokud jsou ob� bu�ky 3 + 5 nevybran� a sou�asn� jsou v KK re�imu
		 // vstup z lev�ho sloupce na prav�
	if(Col<=5 &&  mGrid->getCheck(3,Row)->Checked==false &&  mGrid->getCheck(5,Row)->Checked==false && mGrid->Cells[2][Row].Text=="Kontinu�ln�")
		{

		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
			 if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(F->ms.MyToDouble(mGrid->Cells[31][Row].Text),1) > 1)  //pokud m� KK pohon v�ce objekt�  projdu cyklem
			 {
				 double pohon_id=F->ms.MyToDouble(mGrid->Cells[31][Row].Text);

					 //pr�chod cel� tabulky
						pocitadlo=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
									 //kdy� je sloupec 3 nebo 5 checked
									 if (mGrid->getCheck(3,i)->Checked==true   ||  mGrid->getCheck(5,i)->Checked==true )
									 {
									 // a kdy� najdu stejn� ID a m� re�im kontinu�ln� tak nemohu povolit vstup do dal�� oblasti
											 if(pohon_id==F->ms.MyToDouble(mGrid->Cells[31][i].Text) && mGrid->Cells[2][i].Text=="Kontinu�ln�")
												{
													pocitadlo++;
													vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!");
												}
									 }
						}

					 if(pocitadlo==0)   //pokud jsem nena�el ��dn� zakliknut� bu�ky, mohu povolit vstup do druh� oblasti
					 {                  //projdu op�t cel� cyklem a aktivuji druhou oblast
								for(int i=1;i<=mGrid->RowCount-1;i++)
								{
										 if(pohon_id==F->ms.MyToDouble(mGrid->Cells[31][i].Text)  && mGrid->Cells[2][i].Text=="Kontinu�ln�")
											{
											mGrid->getCheck(7,i)->Enabled=true;
											mGrid->getCheck(9,i)->Enabled=true;
											mGrid->getCheck(11,i)->Enabled=true;
											mGrid->getCheck(13,i)->Enabled=true;
											vypis("",false);
											}
								}
					 }

				}

	 }

		if(Col>5  &&  mGrid->getCheck(7,Row)->Checked==false &&  mGrid->getCheck(9,Row)->Checked==false &&  mGrid->getCheck(11,Row)->Checked==false &&  mGrid->getCheck(13,Row)->Checked==false && mGrid->Cells[2][Row].Text=="Kontinu�ln�")
		{
		// pod�v�m se, zda pohon, kter� je na ��dku, kde do�lo ke kliku m� v�ce objekt� v KK re�imu, pokud ano, mus�m proj�t v�echny
			 if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(F->ms.MyToDouble(mGrid->Cells[31][Row].Text),1) > 1)  //pokud m� KK pohon v�ce objekt�  projdu cyklem
			 {
				 double pohon_id=F->ms.MyToDouble(mGrid->Cells[31][Row].Text);

					 //pr�chod cel� tabulky
						pocitadlo=0;
					 for(int i=1;i<=mGrid->RowCount-1;i++)
					 {
									 //kdy� je sloupec 3 nebo 5 checked
									 if(mGrid->Cells[2][i].Text=="Kontinu�ln�")
									 {
											 if (mGrid->getCheck(7,i)->Checked==true   ||  mGrid->getCheck(9,i)->Checked==true ||  mGrid->getCheck(11,i)->Checked==true  ||  mGrid->getCheck(13,i)->Checked==true)
											 {
								 //	  a kdy� najdu stejn� ID a m� re�im kontinu�ln� tak nemohu povolit vstup do dal�� oblasti
											 if(pohon_id==F->ms.MyToDouble(mGrid->Cells[31][i].Text) && mGrid->Cells[2][i].Text=="Kontinu�ln�")
												{
													pocitadlo++;
													vypis("Tato varianta nelze ulo�it, mus�te se nach�zet ve stejn� oblasti v�b�ru!");
												}
											}
									 }
						}

					 if(pocitadlo==0)   //pokud jsem nena�el ��dn� zakliknut� bu�ky, mohu povolit vstup do druh� oblasti
					 {                  //projdu op�t cel� cyklem a aktivuji druhou oblast
								for(int i=1;i<=mGrid->RowCount-1;i++)
								{
										 if(pohon_id==F->ms.MyToDouble(mGrid->Cells[31][i].Text)  && mGrid->Cells[2][i].Text=="Kontinu�ln�")
											{
											mGrid->getCheck(3,i)->Enabled=true;
											mGrid->getCheck(5,i)->Enabled=true;
											vypis("",false);
											}
								}
					 }
				}
	 }

	 // pokud kliknu do sloupc� 5 a v�c tak na sloupc�ch 3 a 5 d�m enabled false
	 // plat� pouze pokud je pohon a m� KK pro v�ce jak 1 objekt
//		if(Col>5 && mGrid->getCheck(3,Row)->Checked==false  && mGrid->getCheck(5,Row)->Checked==false &&  mGrid->Cells[2][Row].Text=="Kontinu�ln�")
//		{
//		if(mGrid->getCheck(7,Row)->Checked==true ||  mGrid->getCheck(9,Row)->Checked==true || mGrid->getCheck(11,Row)->Checked==true || mGrid->getCheck(13,Row)->Checked==true)
//		{
//				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(F->ms.MyToDouble(mGrid->Cells[31][Row].Text)) >1)
//					{
//					 double pohon_id=F->ms.MyToDouble(mGrid->Cells[31][Row].Text);
//
//							 for(int i=1;i<=mGrid->RowCount-1;i++)
//							 {
//									if(pohon_id==F->ms.MyToDouble(mGrid->Cells[31][i].Text)  && mGrid->Cells[2][i].Text=="Kontinu�ln�")
//									{
//
//							 //		mGrid->getCheck(3,i)->Enabled=false;
//							 //		mGrid->getCheck(5,i)->Enabled=false;
//							 //aktu�ln� nepou��van� sekce - dovoluje nastavit zamknut� LEV� strany
//
//									}
//							 }
//					}
//			}
//		}

	// aktivace lev� ��sti a zdisablov�n� prav� ��sti

//		if(Col<=5 && mGrid->Cells[2][Row].Text=="Kontinu�ln�")
//		{
//				if(mGrid->getCheck(7,Row)->Checked==false  && mGrid->getCheck(9,Row)->Checked==false && mGrid->getCheck(11,Row)->Checked==false && mGrid->getCheck(13,Row)->Checked==false)
//				{
//					if(mGrid->getCheck(3,Row)->Checked==true ||  mGrid->getCheck(5,Row)->Checked==true)
//					{
//						if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(F->ms.MyToDouble(mGrid->Cells[31][Row].Text)) >1)
//							{
//							 double pohon_id=F->ms.MyToDouble(mGrid->Cells[31][Row].Text);
//
//									 for(int i=1;i<=mGrid->RowCount-1;i++)
//									 {
//											if(pohon_id==F->ms.MyToDouble(mGrid->Cells[31][i].Text)  && mGrid->Cells[2][i].Text=="Kontinu�ln�")
//											{
//								 //			mGrid->getCheck(7,i)->Enabled=false;
//								 //			mGrid->getCheck(9,i)->Enabled=false;
//								 //			mGrid->getCheck(11,i)->Enabled=false;
//								 //			mGrid->getCheck(13,i)->Enabled=false;
//									//aktu�ln� nepou��van� sekce - dovoluje nastavit zamknut� PRAV� strany
//
//											}
//									 }
//							}
//					}
//				}
//		}

  //NEWR
	if(Col==mGrid->ColCount-1)//je kliknutu na n�hled objektu
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
	 if(input_state==FREE)
	 {
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
		if(F->pom!=NULL)//nutno jako samostan� if
		if(F->pom->n==objekty[Row].n)//je spu�t�n� PO a je tedy nutn� zavolat aktualizaci PO
		{
			F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vr�t� ostr� data
			F->NPin();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoTT::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(ModalResult==mrOk)delete[] pohony_zmena;//pokud je stisknuto storno p�i ukon�ov�n�, tak se nema�e //NEWR
	delete[] objekty;
	Form_objekt_nahled->pom=NULL;delete Form_objekt_nahled->pom;
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



UnicodeString TF_gapoTT::calculate(unsigned long Row,short SaveTo)//NEWR
{
//
//	//instance na PO_math, vyu��v� se stejn�ch v�po�t�
//	TPO_math pm;
////
////	//input sekce
//	pm.TT=F->d.v.PP.TT;
//	pm.rezim=objekty[Row].rezim;
//	pm.CT=objekty[Row].CT;
//	//pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/60.0);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
//	pm.DD=objekty[Row].delka_dopravniku;
//	pm.K=objekty[Row].kapacita;
//	pm.P=objekty[Row].pozice;
//	pm.M=objekty[Row].mezera;
//	pm.MJ=objekty[Row].mezera_jig;
//	pm.MP=objekty[Row].mezera_podvozek;
//	pm.dJ=F->d.v.PP.delka_jig;
//	pm.sJ=F->d.v.PP.sirka_jig;
//	pm.dP=F->d.v.PP.delka_podvozek;
//	pm.Rotace=objekty[Row].rotace;
// //	pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/60.0);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
//
//	//vol�n� samotn�ho v�po�tu dle volby stanov�n� pomoci checkboxu
//	if(mGrid->getCheck(3,Row)->Checked)//m�n� se CT,RD,K,P,M, z�st�v� DD
//	{
//		pm.CT_locked=false;pm.DD_locked=true;
//		pm.input_RD(true);
//		if(SaveTo==0)
//		{
//			mGrid->Cells[11][Row].Font->Color=clLOCKED;//DD
//			mGrid->Cells[7][Row].Font->Color=clUNLOCKED;//CT
//			mGrid->Cells[13][Row].Font->Color=clUNLOCKED;//K
//		}
//	}
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
//				mGrid->Cells[15][Row].Text	= F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
//				mGrid->Cells[17][Row].Text	=	F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
//				mGrid->Cells[19][Row].Text=	F->m.round2double(pm.DD*(1+999*DDunit),2,"..");
//				mGrid->Cells[21][Row].Text= F->m.round2double(pm.K,2,"..");
//				mGrid->Cells[23][Row].Text=	F->m.round2double(pm.P,2,"..");
//				mGrid->Cells[25][Row].Text=	F->m.round2double(pm.MJ*(1+999*Munit),2,"..");
//				mGrid->Cells[27][Row].Text=	F->m.round2double(pm.MP*(1+999*Munit),2,"..");
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
//		 case 2://ulo�en� hodnot z ukazatele
//		 {
//				Form_objekt_nahled->pom=new Cvektory::TObjekt;
//				Form_objekt_nahled->pom->pohon=objekty[Row].pohon;
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
//



}



void __fastcall TF_gapoTT::scGPButton_stornoClick(TObject *Sender)
{
	Form_parametry_linky->Button_save->Enabled=true;
	Form_parametry_linky->Button_storno->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TF_gapoTT::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

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
			F_gapoTT->ModalResult = mrCancel;// vr�t� stejnou hodnotu jako tla��tko
			F_gapoTT->VisibleChanging();// skryje form, stejn� jako visible=false
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

