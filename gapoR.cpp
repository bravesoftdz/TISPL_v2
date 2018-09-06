//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//NEWR
#include "gapoR.h"
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
#pragma link "rHTMLLabel"
#pragma link "scDrawUtils"
#pragma link "scGPImages"
#pragma resource "*.dfm"
TF_gapoR *F_gapoR;
//---------------------------------------------------------------------------
__fastcall TF_gapoR::TF_gapoR(TComponent* Owner)
	: TForm(Owner)
{
	//nastaven� barvy formul��e
	F_gapoR->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK a STORNO    NEWR
	F->m.designButton(scGPButton_OK,F_gapoR,1,2);
	F->m.designButton(scGPButton_storno,F_gapoR,2,2);
	rHTMLLabel_InfoText->Font->Color = (TColor)RGB(43,87,154);//(TColor)RGB(0,128,255);

	Offset=10;//odsazen� tohototo formu po v�ech str�n�ch od tabulky

	//definice barev NEWR
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);

	//legenda NEWR
	rHTMLLabel_legenda_titulek->FontColor=(TColor)RGB(43,87,154);
	rHTMLLabel_legenda->FontColor=clUNLOCKED;
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormActivate(TObject *Sender)
{
//zde nevolat d�l� probl�my
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormShow(TObject *Sender)
{
	//workaround odchyt�v�n� stisku kl�ves NEWR
	Edit1->SetFocus();

	////////jednotky////////  NEWR

	AnsiString T=F->readINI("nastaveni_form_parametry", "CT");
	if(T=="")CTunit=0;else CTunit=T.ToInt();
	//T=F->readINI("nastaveni_form_parametry","RDt");
	//if(T=="")RDunit=0;else RDunit=T.ToInt();
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

	////////definice tabulky////////
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=3;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;//NEWR

	////////vytvo�en� tabulky s po�adovan�m po�tem sloupc� a ��dk�////////
	unsigned long ColCount=22;//pevn� po�et slopc�  //NEWR
	unsigned long RowCount=1;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek
	if(pohony_zmena!=NULL)
	{
		for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
		{
			if(pohony_zmena[i].X)
			{                                                                 //vr�t� pouze KK objekty
				pohony_zmena[i].Y=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1);
				RowCount+=pohony_zmena[i].Y;
			}
		}
	}
	mGrid->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty
	////////pln�n� daty - hlavi�ka////////  //NEWR
	mGrid->Cells[0][0].Text="Pouze zm�n�n� pohony";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnut� tu�n�ho p�sma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
	mGrid->Cells[1][0].Text="Ovlivn�n� techn. objekty";
	mGrid->Cells[2][0].Text="CT,RD,K,P,M";
	mGrid->Cells[3][0].Text="DD";
	mGrid->Cells[4][0].Text="RD,DD,P,M";
	mGrid->Cells[5][0].Text="CT,K";
	if(CTunit)mGrid->Cells[6][0].Text="CT - Technologick� �as [min]";else mGrid->Cells[6][0].Text="CT - Technologick� �as [s]";
	if(RDunit)mGrid->Cells[8][0].Text="RD - Rychlost pohonu [m/min]";else mGrid->Cells[8][0].Text="RD - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[10][0].Text="DD - D�lka objekt [mm]";else mGrid->Cells[10][0].Text="DD - D�lka objekt [m]";
	mGrid->Cells[12][0].Text="K - Kapacita [voz�k� + mezer]";
	mGrid->Cells[14][0].Text="P - Pozice [voz�k�]";
	if(Munit)mGrid->Cells[16][0].Text="M - mezera jig [mm]";else mGrid->Cells[16][0].Text="M - mezera jig [m]";
	if(Munit)mGrid->Cells[18][0].Text="M - mezera voz�k [mm]";else mGrid->Cells[18][0].Text="M - mezera voz�k [m]";
	mGrid->Cells[20][0].Text="Rotace [�]";
	mGrid->Cells[21][0].Text="N�hled";

  scGPImage_zamky->Top=45;
  scGPImage_zamky->Left=96;

	////////p�i�ad� cel� oblasti bun�k toto�n� vlastnosti jako u referen�n� bu�ky////////
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro prvn� ��dek

	//p�i�azen� jin�ho fontu, nem�n�m polo�k�m - NEWR
	mGrid->Cells[2][0].Font->Color=clUNLOCKED;
	mGrid->Cells[3][0].Font->Color=clLOCKED;
	mGrid->Cells[4][0].Font->Color=clUNLOCKED;
	mGrid->Cells[5][0].Font->Color=clLOCKED;

	//manualfit v��ky 0-t�ho ��dku (zat�m nen� p�ipravena metoda)
	unsigned short n=12;if(RDunit)n=8;//��slo sloupce s nejdel��m textem hlavi�ky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdel�� pou�it� text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
	//manualfit ���ky sloupc� mimo nult�ho (ten je �e�en automaticky n�e pomoc� SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=23;//ostatn� n�sleduj�c� sloupce zat�m default ���ka

	//nastaven� velikosti nult�ho sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show(), ale l�pe p�ed merge metodami
	mGrid->SetColumnAutoFit(0);

	//slou�en� bun�k hlavi�ky PO  - vhodn� za SetColumnAutoFit um�st�t - NEWR
	mGrid->MergeCells(6,0,7,0);mGrid->MergeCells(8,0,9,0);mGrid->MergeCells(10,0,11,0);mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);

	////////jednoliv� ��dky////////    NEWR
	unsigned long j=1;//��slo aktu�ln� zpracov�van�ho ��dku, mus� za��nat 1, 0 - je hlavi�ka
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou bu�ku nevyu��v�me nech�v�me pr�zdnou (z d�vodu toto�n� indexace)
	{
		if(pohony_zmena[i].X)//vypisuje pouze pou�it� pohony, toto vyhodit,pokud budu cht�t vypsat v�echny pohony a potom j bude toto�n� s i...
		{
			//vrat� formou ukazatelem na pole objekty p�i�azen� k dan�mu pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i,1);
			unsigned int z=0;
			for(;z<pohony_zmena[i].Y;z++)
			{
				//pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, za ��elem dal��ho pou�it�, pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;mGrid->Cells[0][j].Background->Color=clBACKGROUND;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
				//volby - checkboxy
				mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;//NEWR
				mGrid->MergeCells(2,j,3,j);mGrid->MergeCells(4,j,5,j);//slou�en� sloupc�
				//parametry objekt�           /*NEWR*/                            //NEWR
				mGrid->Cells[6][j].Text=O[z].CT/(1+59.0*CTunit);	 								mGrid->Cells[6][j].Align=mGrid->LEFT;	mGrid->Cells[6][j].Font->Color=clOLD;	mGrid->Cells[7][j].Align=mGrid->LEFT; mGrid->Cells[7][j].Font->Color=clUNLOCKED;
				mGrid->Cells[8][j].Text=O[z].RD*(1+59.0*RDunit);                  mGrid->Cells[8][j].Align=mGrid->LEFT;	mGrid->Cells[8][j].Font->Color=clOLD;	mGrid->Cells[9][j].Align=mGrid->LEFT; mGrid->Cells[9][j].Font->Color=clUNLOCKED;
				mGrid->Cells[10][j].Text=O[z].delka_dopravniku*(1+999*DDunit);    mGrid->Cells[10][j].Align=mGrid->LEFT;mGrid->Cells[10][j].Font->Color=clOLD;mGrid->Cells[11][j].Align=mGrid->LEFT;mGrid->Cells[11][j].Font->Color=clUNLOCKED;
				mGrid->Cells[12][j].Text=O[z].kapacita;                           mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT;mGrid->Cells[13][j].Font->Color=clUNLOCKED;
				mGrid->Cells[14][j].Text=O[z].pozice;                             mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				mGrid->Cells[16][j].Text=O[z].mezera_jig*(1+999*Munit);           mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
				mGrid->Cells[18][j].Text=O[z].mezera_podvozek*(1+999*Munit); 			mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[20][j].Text=AnsiString(O[z].rotace);             		mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;
				mGrid->Cells[21][j].Type=mGrid->BUTTON;mGrid->Cells[21][j].Text="...";mGrid->Cells[21][j].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
				TscGPButton *B=mGrid->createButton(21,j);//vytvo�en� buttnu, l�p� p�ed n�sleduj�c�m cyklem, aby se pozd�ji mohl parametrizovat
				/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
				//B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//pad�
				//v�choz� nastaven� v lev�m slouci - je v�dy po zobrazen� za�krnuta tato volba
				mGrid->getCheck(2,j)->Checked=true;
				//zajist� p�epo�et dan�ho ��dku, mus� b�t posledn� p�ed j++- nov�ch hodnot NEWR
				calculate(j);
				//posun na dal�� ��dek v�sledn� tabulky
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+pohony_zmena[i].Y-1);//slou�en� bun�k pohony
			O=NULL;delete O;
		}
	}

	////////rozd�len� sekc� svisl�m or�mov�n�m//////// NEWR
	mGrid->Cells[1][1].RightBorder->Width=mGrid->Cells[1][0].RightBorder->Width=2;
	mGrid->Cells[5][1].RightBorder->Width=mGrid->Cells[5][0].RightBorder->Width=2;
	mGrid->SetCells(mGrid->Cells[1][1],1,2,1,RowCount-2);
	mGrid->SetCells(mGrid->Cells[5][1],5,2,5,RowCount-2);
	mGrid->Cells[1][RowCount-1].RightBorder->Width=mGrid->Cells[1][1].RightBorder->Width;
	mGrid->Cells[5][RowCount-1].RightBorder->Width=mGrid->Cells[5][1].RightBorder->Width;

   for(int r=0;r<=RowCount-1;r++)
   {
    mGrid->Cells[7][r].RightBorder->Width=mGrid->Cells[9][r].RightBorder->Width=mGrid->Cells[11][r].RightBorder->Width=mGrid->Cells[13][r].RightBorder->Width=mGrid->Cells[15][r].RightBorder->Width=mGrid->Cells[17][r].RightBorder->Width=mGrid->Cells[19][r].RightBorder->Width=mGrid->Cells[20][r].RightBorder->Width=2;
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
	//pozice komponent
	F->m.designButton(scGPButton_OK,F_gapoR,1,2);
	F->m.designButton(scGPButton_storno,F_gapoR,2,2);
	rHTMLLabel_InfoText->Top=mGrid->Top+mGrid->Height+1;//+1 kv�li or�mov�n� tabulky
	rHTMLLabel_legenda_titulek->Top=rHTMLLabel_InfoText->Top;rHTMLLabel_legenda_titulek->Left=Width-rHTMLLabel_legenda->Width-Offset/2;
	rHTMLLabel_legenda->Top=rHTMLLabel_legenda_titulek->Top+rHTMLLabel_legenda_titulek->Height;rHTMLLabel_legenda->Left=rHTMLLabel_legenda_titulek->Left;
	Button1->Top=scGPButton_OK->Top;//prozatim - bude smaz�no
	////pozice gapo formu, pokud je stejn� velk� jako hlavn� form, tak na 0 pozici, jinak na st�ed PL formu
	if(Width==F->Width)Left=0;else Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Width/2;
	if(Height==F->Height)Top=0;else Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_parametry_linky->scGPPanel2->Height/2-Height/2;//um�st� na polovinu PL formul��e
	////zobrazen� or�mov�n�
	zobrazitFrameForm=true;
	////kv�li �patn� funguj�c� funkci ot��en� kole�ka my�i
	liche_otoceni_koleckem_mysi=false; //NEWR
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykresl� tabulku
	if(zobrazitFrameForm)F->m.frameForm(this,clWebOrange,1);//vykresl� or�mov�n� okolo formu
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick
void TF_gapoR::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	//NASTAVEN�, �E NELZE ZRU�IT CHECKED NA AKTU�LN� CHECKED CHECKBOXU
	if(Col>=2 && mGrid->getCheck(Col,Row)->Checked==false)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col,Row);
		CH->Checked=true;
		CH=NULL;delete CH;
	}

	if(Col==2 &&  mGrid->getCheck(Col,Row)->Checked)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col+2,Row);
		CH->Checked=false;
		CH=NULL;delete CH;
	}

	if(Col==4 &&  mGrid->getCheck(Col,Row)->Checked)
	{
		TscGPCheckBox *CH=mGrid->getCheck(Col-2,Row);
		CH->Checked=false;
		CH=NULL;delete CH;
	}

//NEWR - mus� b�t nakonci
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
		calculate(Row);//zajist� p�epo�et dan�ho ��dku
		FormPaint(this);//zajist� p�ekreslen� bez probliku
	}
}
//---------------------------------------------------------------------------
void TF_gapoR::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TF_gapoR::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(ModalResult==mrOk)delete[] pohony_zmena;//pokud je stisknuto storno p�i ukon�ov�n�, tak se nema�e //NEWR
	delete[] objekty;
	Form_objekt_nahled->pom=NULL;delete Form_objekt_nahled->pom;
}
//---------------------------------------------------------------------------
//provizorn�, vy/zap�n�n� AA
void __fastcall TF_gapoR::Button1Click(TObject *Sender)
{
	//ShowMessage(objekty[2].pohon->n);//pouze duplik�t objekt�, proto se neprop�e do spoj�ku OBJEKTY
	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);
}
//---------------------------------------------------------------------------
//pro dan� ��dek dle nastaven�ho checkboxu, dopo��t� a dosad� nov� hodnoty parametr� dan�ho objektu z dan�ho ��dku, v p��pad� SaveTo -1, vr�t� formou textu, odd�len� st�edn�ky, 0 - nevrac� nic, 1 ulo�� do bin�rky
UnicodeString TF_gapoR::calculate(unsigned long Row,short SaveTo)//NEWR
{
	//instance na PO_math, vyu��v� se stejn�ch v�po�t�
	TPO_math pm;

	//input sekce
	pm.TT=F->d.v.PP.TT;
	pm.rezim=objekty[Row].rezim;
	pm.CT=objekty[Row].CT;
	pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/1+60.0*Form_parametry_linky->aRDunit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat
	pm.DD=objekty[Row].delka_dopravniku;
	pm.K=objekty[Row].kapacita;
	pm.P=objekty[Row].pozice;
	pm.M=objekty[Row].mezera;
	pm.MJ=objekty[Row].mezera_jig;
	pm.MP=objekty[Row].mezera_podvozek;
	pm.dJ=F->d.v.PP.delka_jig;
	pm.sJ=F->d.v.PP.sirka_jig;
	pm.dP=F->d.v.PP.delka_podvozek;
	pm.Rotace=objekty[Row].rotace;
	pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999*Form_parametry_linky->Runit);//mus�m br�t ze stringgridu, kv�li stornu, nikoliv p��mo z dat

	//vol�n� samotn�ho v�po�tu dle volby stanov�n� pomoci checkboxu
	if(mGrid->getCheck(2,Row)->Checked)//m�n� se CT,RD,K,P,M, z�st�v� DD
	{
		pm.CT_locked=false;pm.DD_locked=true;
		pm.input_RD(true);
		if(SaveTo==0)
		{
			mGrid->Cells[11][Row].Font->Color=clLOCKED;//DD
			mGrid->Cells[7][Row].Font->Color=clUNLOCKED;//CT
			mGrid->Cells[13][Row].Font->Color=clUNLOCKED;//K
		}
	}
	else//m�n� se RD,DD,P,M, z�st�v� CT,K
	{
		pm.CT_locked=true;pm.DD_locked=false;
		pm.input_RD(true);
		if(SaveTo==0)
		{
			mGrid->Cells[11][Row].Font->Color=clUNLOCKED;//DD
			mGrid->Cells[7][Row].Font->Color =clLOCKED;//CT
			mGrid->Cells[13][Row].Font->Color=clLOCKED;//K
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
		 case 0://pouze vr�t� text do bun�k
		 {
				mGrid->Cells[7][Row].Text	= F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
				mGrid->Cells[9][Row].Text	=	F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
				mGrid->Cells[11][Row].Text=	F->m.round2double(pm.DD*(1+999*DDunit),2,"..");
				mGrid->Cells[13][Row].Text= F->m.round2double(pm.K,2,"..");
				mGrid->Cells[15][Row].Text=	F->m.round2double(pm.P,2,"..");
				mGrid->Cells[17][Row].Text=	F->m.round2double(pm.MJ*(1+999*Munit),2,"..");
				mGrid->Cells[19][Row].Text=	F->m.round2double(pm.MP*(1+999*Munit),2,"..");
		 }break;
		 case 1://ulo�en� do spoj�ku OBJEKTY - je-li po�adov�no
		 {
				Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
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
//stiskne-li se OK, ulo�� do OBJEKTY a n�sledn� ukon�� PL form (p�ed�n�m modalresults) - NEWR
void __fastcall TF_gapoR::scGPButton_OKClick(TObject *Sender)
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
//zkop�rov�n� obsahu tabulky do schr�nky - NEWR
void __fastcall TF_gapoR::scGPGlyphButton_copyClick(TObject *Sender)
{
	mGrid->CopyCells2Clipboard(0,0,mGrid->ColCount-1,mGrid->RowCount-1);
}
//---------------------------------------------------------------------------
//zkop�ruje data do excelu - NEWR
void __fastcall TF_gapoR::scButton_csvClick(TObject *Sender)
{
	AnsiString T="";
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		T+=calculate(Row,-1)+"\n";//sice se propo��t�v� opakovan�, ale kv�li mo�n�mu zobrazen� dat ve zkr�cen� form� v tabulce. lepe z ostr�ch dat
	}
	F->d.v.Text2CSV(T,Form1->FileName+"_tabulka_gapoR","Ulo�it hodnoty do CSV...","*.csv","Soubory form�tu CSV (*.csv)|*.csv|Soubory form�tu XLS (*.xls)|*.xls");
}
//---------------------------------------------------------------------------
//p�i stisku kl�vesy (na tuto metodu vede p�esm�rov�n� ze v�ech komponent) - NEWR
void __fastcall TF_gapoR::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
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
			F_gapoR->ModalResult = mrCancel;// vr�t� stejnou hodnotu jako tla��tko
			F_gapoR->VisibleChanging();// skryje form, stejn� jako visible=false
		 }break;
		}
}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::scScrollBar_horizontChange(TObject *Sender)
{
	mGrid->Left=F->m.round((Width-mGrid->Width-Offset)*scScrollBar_horizont->Position/100.0);
	//doladit posouv�n� komponent
	if(scScrollBar_horizont->Position<scScrollBar_horizont->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci mus� p�ekreslit cel�
}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::scScrollBar_verticalChange(TObject *Sender)
{
	mGrid->Top=F->m.round((Height-mGrid->Height-Offset)*scScrollBar_vertical->Position/100.0);
	//doladit posouv�n� komponent
	if(scScrollBar_vertical->Position<scScrollBar_vertical->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci mus� p�ekreslit cel�

}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled)
{
	if(liche_otoceni_koleckem_mysi)//Velice nutn� konstrukce kv�li �patn� fungujic� funkci OnMouseWheel, detukuje ka�d� druhou ud�lost FormMouseWheel
	{
		liche_otoceni_koleckem_mysi=false;
		scScrollBar_vertical->Position-=scScrollBar_vertical->SmallChange;
	}
	else liche_otoceni_koleckem_mysi=true;
}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled)
{
	if(liche_otoceni_koleckem_mysi)//Velice nutn� konstrukce kv�li �patn� fungujic� funkci OnMouseWheel, detukuje ka�d� druhou ud�lost FormMouseWheel
	{
		liche_otoceni_koleckem_mysi=false;
		scScrollBar_vertical->Position+=scScrollBar_vertical->SmallChange;
	}
	else liche_otoceni_koleckem_mysi=true;
}
//---------------------------------------------------------------------------


void __fastcall TF_gapoR::FormMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
mGrid->HighlightRowOnMouse(X,Y,(TColor)RGB(240,240,240));
}
//---------------------------------------------------------------------------


