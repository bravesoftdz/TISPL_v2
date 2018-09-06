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
	//nastavení barvy formuláøe
	F_gapoR->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK a STORNO    NEWR
	F->m.designButton(scGPButton_OK,F_gapoR,1,2);
	F->m.designButton(scGPButton_storno,F_gapoR,2,2);
	rHTMLLabel_InfoText->Font->Color = (TColor)RGB(43,87,154);//(TColor)RGB(0,128,255);

	Offset=10;//odsazení tohototo formu po všech stránách od tabulky

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
//zde nevolat dìlá problémy
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormShow(TObject *Sender)
{
	//workaround odchytávání stisku kláves NEWR
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
	mGrid=new TmGrid(this);//vždy nutno jako první
	mGrid->Tag=3;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;//NEWR

	////////vytvoøení tabulky s požadovaným poètem sloupcù a øádkù////////
	unsigned long ColCount=22;//pevný poèet slopcù  //NEWR
	unsigned long RowCount=1;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
	if(pohony_zmena!=NULL)
	{
		for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
		{
			if(pohony_zmena[i].X)
			{                                                                 //vrátí pouze KK objekty
				pohony_zmena[i].Y=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1);
				RowCount+=pohony_zmena[i].Y;
			}
		}
	}
	mGrid->Create(ColCount,RowCount);//samotné vytvoøení matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty
	////////plnìní daty - hlavièka////////  //NEWR
	mGrid->Cells[0][0].Text="Pouze zmìnìné pohony";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnutí tuèného písma
	mGrid->Cells[0][0].Font->Orientation=900;
	mGrid->Cells[0][0].Align=mGrid->CENTER;
	mGrid->Cells[0][0].Valign=mGrid->BOTTOM;
	mGrid->Cells[0][0].BottomMargin=4;mGrid->Cells[0][0].TopMargin=8;
	mGrid->Cells[0][0].Background->Color=clBACKGROUND;
	mGrid->Cells[1][0].Text="Ovlivnìné techn. objekty";
	mGrid->Cells[2][0].Text="CT,RD,K,P,M";
	mGrid->Cells[3][0].Text="DD";
	mGrid->Cells[4][0].Text="RD,DD,P,M";
	mGrid->Cells[5][0].Text="CT,K";
	if(CTunit)mGrid->Cells[6][0].Text="CT - Technologický èas [min]";else mGrid->Cells[6][0].Text="CT - Technologický èas [s]";
	if(RDunit)mGrid->Cells[8][0].Text="RD - Rychlost pohonu [m/min]";else mGrid->Cells[8][0].Text="RD - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[10][0].Text="DD - Délka objekt [mm]";else mGrid->Cells[10][0].Text="DD - Délka objekt [m]";
	mGrid->Cells[12][0].Text="K - Kapacita [vozíkù + mezer]";
	mGrid->Cells[14][0].Text="P - Pozice [vozíkù]";
	if(Munit)mGrid->Cells[16][0].Text="M - mezera jig [mm]";else mGrid->Cells[16][0].Text="M - mezera jig [m]";
	if(Munit)mGrid->Cells[18][0].Text="M - mezera vozík [mm]";else mGrid->Cells[18][0].Text="M - mezera vozík [m]";
	mGrid->Cells[20][0].Text="Rotace [°]";
	mGrid->Cells[21][0].Text="Náhled";

  scGPImage_zamky->Top=45;
  scGPImage_zamky->Left=96;

	////////pøiøadí celé oblasti bunìk totožné vlastnosti jako u referenèní buòky////////
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro první øádek

	//pøiøazení jiného fontu, nemìným položkám - NEWR
	mGrid->Cells[2][0].Font->Color=clUNLOCKED;
	mGrid->Cells[3][0].Font->Color=clLOCKED;
	mGrid->Cells[4][0].Font->Color=clUNLOCKED;
	mGrid->Cells[5][0].Font->Color=clLOCKED;

	//manualfit výšky 0-tého øádku (zatím není pøipravena metoda)
	unsigned short n=12;if(RDunit)n=8;//èíslo sloupce s nejdelším textem hlavièky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdelší použitý text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
	//manualfit šíøky sloupcù mimo nultého (ten je øešen automaticky níže pomocí SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=23;//ostatní následující sloupce zatím default šíøka

	//nastavení velikosti nultého sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show(), ale lépe pøed merge metodami
	mGrid->SetColumnAutoFit(0);

	//slouèení bunìk hlavièky PO  - vhodné za SetColumnAutoFit umístít - NEWR
	mGrid->MergeCells(6,0,7,0);mGrid->MergeCells(8,0,9,0);mGrid->MergeCells(10,0,11,0);mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);

	////////jednolivé øádky////////    NEWR
	unsigned long j=1;//èíslo aktuálnì zpracovávaného øádku, musí zaèínat 1, 0 - je hlavièka
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		if(pohony_zmena[i].X)//vypisuje pouze použité pohony, toto vyhodit,pokud budu chtít vypsat všechny pohony a potom j bude totožné s i...
		{
			//vratí formou ukazatelem na pole objekty pøiøazené k danému pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i,1);
			unsigned int z=0;
			for(;z<pohony_zmena[i].Y;z++)
			{
				//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;mGrid->Cells[0][j].Background->Color=clBACKGROUND;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
				//volby - checkboxy
				mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;//NEWR
				mGrid->MergeCells(2,j,3,j);mGrid->MergeCells(4,j,5,j);//slouèení sloupcù
				//parametry objektù           /*NEWR*/                            //NEWR
				mGrid->Cells[6][j].Text=O[z].CT/(1+59.0*CTunit);	 								mGrid->Cells[6][j].Align=mGrid->LEFT;	mGrid->Cells[6][j].Font->Color=clOLD;	mGrid->Cells[7][j].Align=mGrid->LEFT; mGrid->Cells[7][j].Font->Color=clUNLOCKED;
				mGrid->Cells[8][j].Text=O[z].RD*(1+59.0*RDunit);                  mGrid->Cells[8][j].Align=mGrid->LEFT;	mGrid->Cells[8][j].Font->Color=clOLD;	mGrid->Cells[9][j].Align=mGrid->LEFT; mGrid->Cells[9][j].Font->Color=clUNLOCKED;
				mGrid->Cells[10][j].Text=O[z].delka_dopravniku*(1+999*DDunit);    mGrid->Cells[10][j].Align=mGrid->LEFT;mGrid->Cells[10][j].Font->Color=clOLD;mGrid->Cells[11][j].Align=mGrid->LEFT;mGrid->Cells[11][j].Font->Color=clUNLOCKED;
				mGrid->Cells[12][j].Text=O[z].kapacita;                           mGrid->Cells[12][j].Align=mGrid->LEFT;mGrid->Cells[12][j].Font->Color=clOLD;mGrid->Cells[13][j].Align=mGrid->LEFT;mGrid->Cells[13][j].Font->Color=clUNLOCKED;
				mGrid->Cells[14][j].Text=O[z].pozice;                             mGrid->Cells[14][j].Align=mGrid->LEFT;mGrid->Cells[14][j].Font->Color=clOLD;mGrid->Cells[15][j].Align=mGrid->LEFT;mGrid->Cells[15][j].Font->Color=clUNLOCKED;
				mGrid->Cells[16][j].Text=O[z].mezera_jig*(1+999*Munit);           mGrid->Cells[16][j].Align=mGrid->LEFT;mGrid->Cells[16][j].Font->Color=clOLD;mGrid->Cells[17][j].Align=mGrid->LEFT;mGrid->Cells[17][j].Font->Color=clUNLOCKED;
				mGrid->Cells[18][j].Text=O[z].mezera_podvozek*(1+999*Munit); 			mGrid->Cells[18][j].Align=mGrid->LEFT;mGrid->Cells[18][j].Font->Color=clOLD;mGrid->Cells[19][j].Align=mGrid->LEFT;mGrid->Cells[19][j].Font->Color=clUNLOCKED;
				mGrid->Cells[20][j].Text=AnsiString(O[z].rotace);             		mGrid->Cells[20][j].Align=mGrid->LEFT;mGrid->Cells[20][j].Font->Color=clOLD;
				mGrid->Cells[21][j].Type=mGrid->BUTTON;mGrid->Cells[21][j].Text="...";mGrid->Cells[21][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
				TscGPButton *B=mGrid->createButton(21,j);//vytvoøení buttnu, lépì pøed následujícím cyklem, aby se pozdìji mohl parametrizovat
				/*B->Options->NormalColor=clWhite;*/B->Options->FontNormalColor=(TColor)RGB(255,128,0);
				//B->Images->AddImage(F->scGPVirtualImageList1,6);//B->ImageIndex=6;//padá
				//výchozí nastavení v levém slouci - je vždy po zobrazení zaškrnuta tato volba
				mGrid->getCheck(2,j)->Checked=true;
				//zajistí pøepoèet daného øádku, musí být poslední pøed j++- nových hodnot NEWR
				calculate(j);
				//posun na další øádek výsledné tabulky
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+pohony_zmena[i].Y-1);//slouèení bunìk pohony
			O=NULL;delete O;
		}
	}

	////////rozdìlení sekcí svislým orámováním//////// NEWR
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
	Height=mGrid->Height+Offset*2+rHTMLLabel_InfoText->Height+scGPPanel_hlavicka->Height+11+scGPButton_OK->Height+11;// + 11 offset okolo tlaèítka
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
	F->m.designButton(scGPButton_OK,F_gapoR,1,2);
	F->m.designButton(scGPButton_storno,F_gapoR,2,2);
	rHTMLLabel_InfoText->Top=mGrid->Top+mGrid->Height+1;//+1 kvùli orámování tabulky
	rHTMLLabel_legenda_titulek->Top=rHTMLLabel_InfoText->Top;rHTMLLabel_legenda_titulek->Left=Width-rHTMLLabel_legenda->Width-Offset/2;
	rHTMLLabel_legenda->Top=rHTMLLabel_legenda_titulek->Top+rHTMLLabel_legenda_titulek->Height;rHTMLLabel_legenda->Left=rHTMLLabel_legenda_titulek->Left;
	Button1->Top=scGPButton_OK->Top;//prozatim - bude smazáno
	////pozice gapo formu, pokud je stejnì velký jako hlavní form, tak na 0 pozici, jinak na støed PL formu
	if(Width==F->Width)Left=0;else Left=Form_parametry_linky->Left+Form_parametry_linky->Width/2-Width/2;
	if(Height==F->Height)Top=0;else Top=Form_parametry_linky->Top+Form_parametry_linky->Height/2-Form_parametry_linky->scGPPanel2->Height/2-Height/2;//umístí na polovinu PL formuláøe
	////zobrazení orámování
	zobrazitFrameForm=true;
	////kvùli špatnì fungující funkci otáèení koleèka myši
	liche_otoceni_koleckem_mysi=false; //NEWR
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TF_gapoR::FormPaint(TObject *Sender)
{
	mGrid->Show();//vykreslí tabulku
	if(zobrazitFrameForm)F->m.frameForm(this,clWebOrange,1);//vykreslí orámování okolo formu
}
//---------------------------------------------------------------------------
//test volání pøi onclick
void TF_gapoR::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
	//NASTAVENÍ, ŽE NELZE ZRUŠIT CHECKED NA AKTUÁLNÌ CHECKED CHECKBOXU
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

//NEWR - musí být nakonci
	if(Col==mGrid->ColCount-1)//je kliknutu na náhled objektu
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
		calculate(Row);//zajistí pøepoèet daného øádku
		FormPaint(this);//zajistí pøekreslení bez probliku
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
	if(ModalResult==mrOk)delete[] pohony_zmena;//pokud je stisknuto storno pøi ukonèování, tak se nemaže //NEWR
	delete[] objekty;
	Form_objekt_nahled->pom=NULL;delete Form_objekt_nahled->pom;
}
//---------------------------------------------------------------------------
//provizorní, vy/zapínání AA
void __fastcall TF_gapoR::Button1Click(TObject *Sender)
{
	//ShowMessage(objekty[2].pohon->n);//pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
	FormPaint(this);
}
//---------------------------------------------------------------------------
//pro daný øádek dle nastaveného checkboxu, dopoèítá a dosadí nové hodnoty parametrù daného objektu z daného øádku, v pøípadì SaveTo -1, vrátí formou textu, oddìlené støedníky, 0 - nevrací nic, 1 uloží do binárky
UnicodeString TF_gapoR::calculate(unsigned long Row,short SaveTo)//NEWR
{
	//instance na PO_math, využívá se stejných výpoètù
	TPO_math pm;

	//input sekce
	pm.TT=F->d.v.PP.TT;
	pm.rezim=objekty[Row].rezim;
	pm.CT=objekty[Row].CT;
	pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/1+60.0*Form_parametry_linky->aRDunit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
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
	pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999*Form_parametry_linky->Runit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat

	//volání samotného výpoètu dle volby stanovéné pomoci checkboxu
	if(mGrid->getCheck(2,Row)->Checked)//mìní se CT,RD,K,P,M, zùstává DD
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
	else//mìní se RD,DD,P,M, zùstává CT,K
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
		 case -1://uložení do textu je-li požadováno
		 {
				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
		 }break;
		 case 0://pouze vrátí text do bunìk
		 {
				mGrid->Cells[7][Row].Text	= F->m.round2double(pm.CT/(1+59.0*CTunit),2,"..");
				mGrid->Cells[9][Row].Text	=	F->m.round2double(pm.RD*(1+59.0*RDunit),2,"..");
				mGrid->Cells[11][Row].Text=	F->m.round2double(pm.DD*(1+999*DDunit),2,"..");
				mGrid->Cells[13][Row].Text= F->m.round2double(pm.K,2,"..");
				mGrid->Cells[15][Row].Text=	F->m.round2double(pm.P,2,"..");
				mGrid->Cells[17][Row].Text=	F->m.round2double(pm.MJ*(1+999*Munit),2,"..");
				mGrid->Cells[19][Row].Text=	F->m.round2double(pm.MP*(1+999*Munit),2,"..");
		 }break;
		 case 1://uložení do spojáku OBJEKTY - je-li požadováno
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
		 case 2://uložení hodnot z ukazatele pro náhled objektu
		 {
				Form_objekt_nahled->pom=new Cvektory::TObjekt;
				//Form_objekt_nahled->pom->pohon=objekty[Row].pohon;//takto by pøevzal starou rozteè
				Form_objekt_nahled->pom->pohon=new Cvektory::TPohon;
				Form_objekt_nahled->pom->pohon->roztec=pm.R;//ale pøedávám jen do náhledu R, nic víc od pohonu
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
//stiskne-li se OK, uloží do OBJEKTY a následnì ukonèí PL form (pøedáním modalresults) - NEWR
void __fastcall TF_gapoR::scGPButton_OKClick(TObject *Sender)
{
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		calculate(Row,1);//sice se propoèítává opakovanì, ale kvùli možnému zobrazení dat ve zkrácené formì v tabulce. lepe z ostrých dat
		//aktualizace PO, pokud je spuštìné
		if(F->pom!=NULL)//nutno jako samostaný if
		if(F->pom->n==objekty[Row].n)//je spuštìné PO a je tedy nutné zavolat aktualizaci PO
		{
			F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vrátí ostrá data
			F->NPin();
		}
	}
}
//---------------------------------------------------------------------------
//zkopírování obsahu tabulky do schránky - NEWR
void __fastcall TF_gapoR::scGPGlyphButton_copyClick(TObject *Sender)
{
	mGrid->CopyCells2Clipboard(0,0,mGrid->ColCount-1,mGrid->RowCount-1);
}
//---------------------------------------------------------------------------
//zkopíruje data do excelu - NEWR
void __fastcall TF_gapoR::scButton_csvClick(TObject *Sender)
{
	AnsiString T="";
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		T+=calculate(Row,-1)+"\n";//sice se propoèítává opakovanì, ale kvùli možnému zobrazení dat ve zkrácené formì v tabulce. lepe z ostrých dat
	}
	F->d.v.Text2CSV(T,Form1->FileName+"_tabulka_gapoR","Uložit hodnoty do CSV...","*.csv","Soubory formátu CSV (*.csv)|*.csv|Soubory formátu XLS (*.xls)|*.xls");
}
//---------------------------------------------------------------------------
//pøi stisku klávesy (na tuto metodu vede pøesmìrování ze všech komponent) - NEWR
void __fastcall TF_gapoR::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 switch(Key)
	 {
		 case 13: // ENTER
		 {
			if (scGPButton_OK->Enabled)scGPButton_OKClick(Sender);// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;
		 case 27:// ESC
		 {
			F_gapoR->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			F_gapoR->VisibleChanging();// skryje form, stejné jako visible=false
		 }break;
		}
}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::scScrollBar_horizontChange(TObject *Sender)
{
	mGrid->Left=F->m.round((Width-mGrid->Width-Offset)*scScrollBar_horizont->Position/100.0);
	//doladit posouvání komponent
	if(scScrollBar_horizont->Position<scScrollBar_horizont->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci musí pøekreslit celé
}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::scScrollBar_verticalChange(TObject *Sender)
{
	mGrid->Top=F->m.round((Height-mGrid->Height-Offset)*scScrollBar_vertical->Position/100.0);
	//doladit posouvání komponent
	if(scScrollBar_vertical->Position<scScrollBar_vertical->Max)FormPaint(this);
	else {FormPaint(this);Invalidate();}//na konci musí pøekreslit celé

}
//---------------------------------------------------------------------------
//NEWR
void __fastcall TF_gapoR::FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled)
{
	if(liche_otoceni_koleckem_mysi)//Velice nutná konstrukce kvùli špatnì fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
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
	if(liche_otoceni_koleckem_mysi)//Velice nutná konstrukce kvùli špatnì fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
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


