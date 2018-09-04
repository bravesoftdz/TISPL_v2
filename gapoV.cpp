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
	//nastavení barvy formuláøe
	F_gapoV->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK a Storno
	F->m.designButton(scGPButton_OK,F_gapoV,1,2);
	F->m.designButton(scGPButton_storno,F_gapoV,2,2);

	Offset=10;//odsazení thoto formu po všech stránách od tabulky

	//definice barev NEWR
	clOLD			 = (TColor)RGB(200,200,200);
	clLOCKED	 = (TColor)RGB(128,128,128);
	clUNLOCKED = (TColor)RGB(255,128,0);
	clBACKGROUND=(TColor)RGB(250,250,250);
  clHiddenValues = (TColor)RGB(212,212,212);

	// nastavení barvy orámování v hlavièce tabulky + orámování checkboxù
	C1=Form1->m.clIntensive(RGB(128,64,0),20);
	C2=Form1->m.clIntensive(RGB(0,128,0),80);
	C3=Form1->m.clIntensive(RGB(0,0,0),80);
}
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::FormActivate(TObject *Sender)
{
	//zde nevolat obsah form show, muže zde dìlat problémy
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
	if(F->d.v.PP.delka_jig!=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value) titulek+="délky jigu z "+AnsiString(F->d.v.PP.delka_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value);
	if(F->d.v.PP.sirka_jig!=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value) titulek+="šíøky jigu z "+AnsiString(F->d.v.PP.sirka_jig)+" na "+AnsiString(Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value);
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
	mGrid->Left=Offset;mGrid->Top=scGPPanel_hlavicka->Height+Offset;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->DefaultColWidth/=2;

	////////vytvoøení tabulky s požadovaným poètem sloupcù a øádkù////////
	unsigned long ColCount=34;//pevný poèet slopcù
	unsigned long RowCount=1;//dynamický poèet øádkù, default 1 je pro 0-tý indexový øádek
	RowCount+=F->d.v.pocet_objektu(1);
	mGrid->Create(ColCount,RowCount);//samotné vytvoøení matice-tabulky
	objekty=new Cvektory::TObjekt[RowCount];//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty
  tempCHECK=new short[5];//dynamické pole uchovávající pøedchozí volby checkboxù

	////////plnìní daty - hlavièka////////
	mGrid->Cells[0][0].Text="Pohon";
	mGrid->Cells[0][0].Font->Style=TFontStyles();//<< fsBold;//zapnutí tuèného písma
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
	if(CTunit)mGrid->Cells[12][0].Text="CT - Technologický èas [min]";else mGrid->Cells[12][0].Text="CT - Technologický èas [s]";//mGrid->MergeCells(6,0,7,0);//slouèení zatím nefunguje dobøe
	if(RDunit)mGrid->Cells[14][0].Text="RD - Rychlost pohonu [m/min]";else mGrid->Cells[14][0].Text="RD - Rychlost pohonu [m/s]";
	if(DDunit)mGrid->Cells[16][0].Text="DD - Délka objekt [mm]";else mGrid->Cells[16][0].Text="DD - Délka objekt [m]";
	mGrid->Cells[18][0].Text="K - Kapacita [vozíkù + mezer]";
	mGrid->Cells[20][0].Text="P - Pozice [vozíkù]";
	if(Munit)mGrid->Cells[22][0].Text="M - mezera jig [mm]";else mGrid->Cells[22][0].Text="M - mezera jig [m]";
	if(Munit)mGrid->Cells[24][0].Text="M - mezera vozík [mm]";else mGrid->Cells[24][0].Text="M - mezera vozík [m]";
	mGrid->Cells[26][0].Text="Rotace";
	if(Form_parametry_linky->Runit)mGrid->Cells[27][0].Text="R - rozteè [mm]";else mGrid->Cells[27][0].Text="R - rozteè [mm]";
	mGrid->Cells[29][0].Text="Rz - akt. palce - rozestup [m]";//pøepínání jednotek dodìlat
	mGrid->Cells[31][0].Text="Rx - každy n-tý palec";
	mGrid->Cells[33][0].Text="Náhled";

  scGPImage_zamky->Top=45;
  scGPImage_zamky->Left=mGrid->Columns[2].Left-42;
	////////pøiøadí celé oblasti bunìk totožné vlastnosti jako u referenèní buòky////////
	mGrid->SetCells(mGrid->Cells[0][0],1,0,ColCount-1,0);//pro první øádek

	//pøiøazení jiného fontu, nemìným položkám - NEWR
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

	//manualfit výšky 0-tého øádku (zatím není pøipravena metoda)
	unsigned short n=3;//èíslo sloupce s nejdelším textem hlavièky
	Canvas->Font=mGrid->Cells[n][0].Font;	//nejdelší použitý text
	mGrid->Rows[0].Height=Canvas->TextWidth(mGrid->Cells[n][0].Text)+mGrid->Cells[n][0].BottomMargin+mGrid->Cells[n][0].BottomBorder->Width/2+mGrid->Cells[n][0].TopMargin+mGrid->Cells[n][0].TopBorder->Width/2;
  mGrid->Rows[0].Height= 	mGrid->Rows[0].Height + 30; //30 px je výška zameèku
	//manualfit šíøky sloupcù mimo nultého (ten je øešen automaticky níže pomocí SetColumnAutoFit(0);)
	mGrid->Columns[1].Width=50;mGrid->Columns[2].Width=mGrid->Columns[3].Width=mGrid->Columns[4].Width=mGrid->Columns[5].Width=mGrid->Columns[6].Width=mGrid->Columns[7].Width=mGrid->Columns[8].Width=mGrid->Columns[9].Width=mGrid->Columns[10].Width=mGrid->Columns[11].Width=23;//ostatní následující sloupce zatím default šíøka

	//nastavení velikosti nultého sloupce dle obsahu, mùže být umístìno kdekoliv pøed Show(), ale lépe pøed merge metodami
	mGrid->SetColumnAutoFit(0);

	//slouèení bunìk hlavièky PO  - vhodné za SetColumnAutoFit umístít
	mGrid->MergeCells(12,0,13,0);mGrid->MergeCells(14,0,15,0);mGrid->MergeCells(16,0,17,0);mGrid->MergeCells(18,0,19,0);mGrid->MergeCells(20,0,21,0);mGrid->MergeCells(22,0,23,0);mGrid->MergeCells(24,0,25,0);mGrid->MergeCells(27,0,28,0);mGrid->MergeCells(29,0,30,0);mGrid->MergeCells(31,0,32,0);

	////////jednolivé øádky////////
	unsigned long j=1;//èíslo aktuálnì zpracovávaného øádku, musí zaèínat 1, 0 - je hlavièka
	////prùchod všemi objekty bez pøiøazených pohonu
	Cvektory::TObjekt *On=F->d.v.vrat_objekty_bez_pohonu();
	unsigned long On_pocet=F->d.v.vrat_pocet_objektu_bezNEBOs_prirazenymi_pohonu(false,1);
	for(unsigned long i=1;i<=On_pocet;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
		//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
		objekty[j]=On[i];
		//pohony
		mGrid->Cells[0][j].Text="nepøiøazen";mGrid->Cells[0][j].Background->Color=clBACKGROUND;
		//objekty
		mGrid->Cells[1][j].Text=On[i].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
		//volby - checkboxy
		mGrid->Cells[2][j].Type=mGrid->CHECK;mGrid->Cells[4][j].Type=mGrid->CHECK;
		mGrid->MergeCells(2,j,3,j);
		mGrid->MergeCells(4,j,5,j);//slouèení sloupcù
		mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
		mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//slouèení sloupcù
		mGrid->Cells[10][j].Type=mGrid->CHECK;
		mGrid->MergeCells(10,j,11,j);
		//parametry objektù
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
		//výchozí nastavení v druhém levém slouci (popø. upravit, ale je problém s prvním - nelze vždy viz DV+M) - je vždy po zobrazení zaškrnuta tato volba
		mGrid->getCheck(4,j)->Checked=true;//pozor, pøi getCheck(3,j) mi zde házelo chybu, i pøes ošetøení R-záležitostí, pøi pøekliku na tuto položku po zobrazeném formu, ale OK
		//tlaèítko náhledu
		mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		TscGPButton *B=mGrid->createButton(33,j);//vytvoøení buttnu, lépì pøed následujícím cyklem, aby se pozdìji mohl parametrizovat
		B->Options->FontNormalColor=(TColor)RGB(255,128,0);
		//zajistí pøepoèet daného øádku - nových hodnot
		calculate(j);//Rosto: musí být poslední pøed j++, nelze ho dát pøed defaultní zaškrtnutí checkboxù
		//posun na další øádek výsledné tabulky
     for(int sl=0;sl<=ColCount-1;sl++) //oddìlení pohonù silnìjší èarou
    {
    mGrid->Cells[sl][j].BottomBorder->Width=2;
    }
		j++;
	}
	On=NULL;delete On;
	if(On_pocet>0)mGrid->MergeCells(0,1,0,On_pocet);//slouèení bunìk objekty bez pohonu

	////prùchod všemi pohony, které jsou pøiøazeny k objektùm
	for(unsigned long i=1;i<=F->d.v.POHONY->predchozi->n;i++)//0-nultou buòku nevyužíváme necháváme prázdnou (z dùvodu totožné indexace)
	{
   bool DVMmozno=true;
		unsigned long O_pocet=F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1);
    if(O_pocet==0)//tzn. není objektu pøiøazen žádný pohon
		{
			//pohony bez pøiøazení k objektùm v gapoV nezobrazujeme a neøešíme
		}
		else
		{
			//vratí formou ukazatele na pole objekty pøiøazené k danému pohonu
			Cvektory::TObjekt *O=F->d.v.vrat_objekty_vyuzivajici_pohon(i,1);
			unsigned long z=0;
      for(;z<O_pocet;z++)
			{
				//pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, za úèelem dalšího použití, pouze duplikát objektù, proto se nepropíše do spojáku OBJEKTY
				objekty[j]=O[z];
				//pohony
				mGrid->Cells[0][j].Text=O[z].pohon->name;mGrid->Cells[0][j].Background->Color=clBACKGROUND;
				//objekty
				mGrid->Cells[1][j].Text=O[z].short_name;mGrid->Cells[1][j].Background->Color=clBACKGROUND;
				//volby - checkboxy

				mGrid->Cells[4][j].Type=mGrid->CHECK;
				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) <= 1 )  //KK režim
				{
					 mGrid->Cells[2][j].Type=mGrid->CHECK;
					 mGrid->MergeCells(2,j,3,j);
				}

				mGrid->MergeCells(4,j,5,j);//slouèení sloupcù
				mGrid->Cells[6][j].Type=mGrid->CHECK;mGrid->Cells[8][j].Type=mGrid->CHECK;
				mGrid->MergeCells(6,j,7,j);mGrid->MergeCells(8,j,9,j);//slouèení sloupcù
				mGrid->Cells[10][j].Type=mGrid->CHECK;mGrid->MergeCells(10,j,11,j);

				if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK režim
				{   //pokud akt.název pohonu je odlišný od pøedchozího øádku, nastavím typ na CHECK.
					if(O[z].pohon->name!=mGrid->Cells[0][j-1].Text)
					{
						mGrid->Cells[2][j].Type=mGrid->CHECK;
						//barvu prvního sloupce nastvuji níže, nelze zde
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

				//parametry objektù
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
				mGrid->Cells[33][j].Type=mGrid->BUTTON;mGrid->Cells[33][j].Text="...";mGrid->Cells[33][j].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
				TscGPButton *B=mGrid->createButton(33,j);B->Options->FontNormalColor=(TColor)RGB(255,128,0);//vytvoøení buttnu, lépì pøed následujícím cyklem, aby se pozdìji mohl parametrizovat
				//výchozí nastavení v druhém levém slouci (popø. upravit, ale je problém s prvním - nelze vždy  viz DV+M) - je vždy po zobrazení zaškrnuta tato volba
				mGrid->getCheck(4,j)->Checked=true;
				//zajistí pøepoèet daného øádku - nových hodnot
				calculate(j);//Rosto: musí být poslední pøed j++, nelze ho dát pøed výchozí zaškrtnutí checkboxù
        //testování povolení DVM skupiny
        Memo1->Lines->Add("UDV + "+AnsiString(F->m.UDV(O[z].rotace)));
        Memo1->Lines->Add("puv. mezera"+AnsiString(O[z].mezera));
        Memo1->Lines->Add("==UDV +"+AnsiString(F->m.UDV(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace)));
        Memo1->Lines->Add("mezera mezi voz"+AnsiString(F->m.mezera_mezi_voziky(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace,O[z].pohon->roztec,0)));
        if(DVMmozno==true)
        {
         if(F->m.UDV(O[z].rotace)+O[z].mezera==F->m.UDV(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace)+F->m.mezera_mezi_voziky(Form_parametry_vozik->scGPNumericEdit_delka_jig->Value,Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value,O[z].rotace,O[z].pohon->roztec,0))DVMmozno=true;
         else DVMmozno=false;
        }
				//posun na další øádek výsledné tabulky
				j++;
			}
			mGrid->MergeCells(0,j-z,0,j-z+O_pocet-1);//slouèení bunìk pohony
			mGrid->MergeCells(2,j-z,3,j-z+O_pocet-1);//slouèení bunìk 2-3

      for(int sl=0;sl<=ColCount-1;sl++) //silnìjší oddìlení dalšího pohonu
      {
       mGrid->Cells[sl][j-z+O_pocet-1].BottomBorder->Width=2;
      }

      if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(i,1) > 1 )  //KK režim
      { //nelze nastavit hned, v horní èásti, spoleènì s typem Check, ale až zde
        mGrid->getCheck(2,j-z)->Options->FrameNormalColor=C1;
        mGrid->getCheck(2,j-z)->OptionsChecked->FrameNormalColor=C1;
      }
       // ShowMessage((short)DVMmozno);
      if(DVMmozno) {mGrid->getCheck(2,j-z)->Enabled=true;  mGrid->getCheck(2,j-z)->ShowHint=true; mGrid->getCheck(2,j-z)->Hint="mohu";  }
      else {mGrid->getCheck(2,j-z)->Enabled=true; mGrid->getCheck(2,j-z)->ShowHint=false; mGrid->getCheck(2,j-z)->Hint="nemohu";   }

			O=NULL;delete O;
		}
	}

	////////rozdìlení sekcí svislým tlustìjším orámováním////////

    for(int i=1;i<=RowCount-1;i++)
    {
     mGrid->Cells[1][i].RightBorder->Width=2;
     mGrid->Cells[11][i].RightBorder->Width=2;
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

  for(int r=0;r<=RowCount-1;r++)
   {
    mGrid->Cells[13][r].RightBorder->Width=mGrid->Cells[15][r].RightBorder->Width=mGrid->Cells[17][r].RightBorder->Width=mGrid->Cells[19][r].RightBorder->Width=mGrid->Cells[21][r].RightBorder->Width=mGrid->Cells[23][r].RightBorder->Width=mGrid->Cells[25][r].RightBorder->Width=mGrid->Cells[26][r].RightBorder->Width=mGrid->Cells[28][r].RightBorder->Width=mGrid->Cells[30][r].RightBorder->Width=mGrid->Cells[32][r].RightBorder->Width=2;
   }

	////zobrazení orámování
	zobrazitFrameForm=true;


	//barevné orámování hlavièky
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
//---------------------------------------------------------------------------
void TF_gapoV::OnClick(long Tag,unsigned long Col,unsigned long Row)
{
//ZAJISTÍ, ŽE NELZE UDELAT UNCHECK
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
	 vypis("Tato varianta není možná, nebo dochází ke zmìnì mezery.",false);
	 mGrid->getCheck(Col,Row)->Checked=false;
	 //mGrid->getCheck(tempCHECK[Row],Row)->Checked=true;//zaškrtne pøedchozí volbu pøesunout do onclick urèitì pøed calculate
	}

	if(Col==2 &&  mGrid->getCheck(Col,Row)->Checked && input_state==FREE)
	{

	 if(objekty[Row].pohon!=NULL)
	 {
		int pohon_n=objekty[Row].pohon->n;

		for(int i=1;i<=mGrid->RowCount-1;i++)
		{
			if(objekty[i].pohon!=NULL)   //pokud v oblasti vstoupím do levého slouèeného sloupce, zruším všechny ostatní checked hodnoty
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
	 }  //std chování, mimo oblasti
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
			if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1) //default šedá barva je záporná, proto mohu nastavovat
			{
			//má pohon, ale má jen jeden øádek, tzn mohu nastavit klasicky první sloupec
			//ShowMessage("jsem sam KK, mohu");
			mGrid->getCheck(Col-2,Row)->Checked=false;
			mGrid->getCheck(Col+2,Row)->Checked=false;
			mGrid->getCheck(Col+4,Row)->Checked=false;
			mGrid->getCheck(Col+6,Row)->Checked=false;
			}
			else // musím najít a vybrat správný øádek v prvním sloupci, který dám Checked=false
			{
				for(int i=1;i<=mGrid->RowCount-1;i++)
				{
					 if(objekty[i].pohon!=NULL)
					 {
							if(pohon_n == objekty[i].pohon->n)
							{
								 pruchod++;  //workaround -  ve foru najdu první výskyt a další už neøeším (pamìtová chyba, kvùli slouèeným buòkám)
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
	 else //nemám pohon, mohu nastavit hned klasicky první sloupec
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
		//má pohon, ale má jen jeden øádek, tzn mohu nastavit klasicky první sloupec
		//ShowMessage("jsem sam KK, mohu");

			 mGrid->getCheck(Col-4,Row)->Checked=false;
			 mGrid->getCheck(Col-2,Row)->Checked=false;
			 mGrid->getCheck(Col+2,Row)->Checked=false;
			 mGrid->getCheck(Col+4,Row)->Checked=false;
		}
		else // musím najít a vybrat správný øádek v prvním sloupci, který dám Checked=false
		{
			for(int i=1;i<=mGrid->RowCount-1;i++)
			{
				if(objekty[i].pohon!=NULL)
				{
					 if(pohon_n == objekty[i].pohon->n)
					 {
						 pruchod++;  //workaround -  ve foru najdu první výskyt a další už neøeším (pamìtová chyba, kvùli slouèeným buòkám)
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
	 else //nemám pohon, mohu nastavit hned klasicky první sloupec
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
		if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1) //default šedá barva je záporná, proto mohu nastavovat
		{
		//má pohon, ale má jen jeden øádek, tzn mohu nastavit klasicky první sloupec

		mGrid->getCheck(Col-6,Row)->Checked=false;
		mGrid->getCheck(Col-4,Row)->Checked=false;
		mGrid->getCheck(Col-2,Row)->Checked=false;
		mGrid->getCheck(Col+2,Row)->Checked=false;
		}
		else // musím najít a vybrat správný øádek v prvním sloupci, který dám Checked=false
		{

			for(int i=1;i<=mGrid->RowCount-1;i++)
			{
				if(objekty[i].pohon!=NULL)
				{
					 if(pohon_n == objekty[i].pohon->n)
					 {
					  pruchod++;  //workaround -  ve foru najdu první výskyt a další už neøeším (pamìtová chyba, kvùli slouèeným buòkám)
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
	 else //nemám pohon, mohu nastavit hned klasicky první sloupec
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
		if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) <= 1) //default šedá barva je záporná, proto mohu nastavovat
		{
		//má pohon, ale má jen jeden øádek, tzn mohu nastavit klasicky první sloupec

		mGrid->getCheck(Col-8,Row)->Checked=false;
		mGrid->getCheck(Col-6,Row)->Checked=false;
		mGrid->getCheck(Col-4,Row)->Checked=false;
		mGrid->getCheck(Col-2,Row)->Checked=false;
		}
		else // musím najít a vybrat správný øádek v prvním sloupci, který dám Checked=false
		{

			for(int i=1;i<=mGrid->RowCount-1;i++)
			{
				if(objekty[i].pohon!=NULL)
				{
					if(pohon_n == objekty[i].pohon->n)
					{
						 pruchod++;  //workaround -  ve foru najdu první výskyt a další už neøeším (pamìtová chyba, kvùli slouèeným buòkám)
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
	 else //nemám pohon, mohu nastavit hned klasicky první sloupec
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
	}	 //KONEC PØEPÍNAÈÙ

	//VALIDAÈNÍ ÈÁST - HLÍDÁNÍ OBLASTÍ

	if(Col>=4 && Col<=6 && input_state==FREE )  /*&&  mGrid->getCheck(6,Row)->Checked==false*/
		{
	 if(mGrid->getCheck(4,Row)->Checked==true ||  mGrid->getCheck(6,Row)->Checked==true)
	 {
		// podívám se, zda pohon, který je na øádku, kde došlo ke kliku má více objektù v KK režimu, pokud ano, musím projít všechny
		if(objekty[Row].pohon!=NULL)
		{
			int pohon_n=objekty[Row].pohon->n;
			if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
			{
			 //prùchod celé tabulky
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
															vypis("Tato varianta nelze uložit, musíte se nacházet ve stejné oblasti výbìru!");
														}
												}
						}
							}

					 if(pocitadlo_validace==0)   //pokud jsem nenašel žádné zakliknuté buòky, mohu povolit vstup do druhé oblasti
					 {                  //projdu opìt celé cyklem a aktivuji druhou oblast

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
		// podívám se, zda pohon, který je na øádku, kde došlo ke kliku má více objektù v KK režimu, pokud ano, musím projít všechny
		if(objekty[Row].pohon!=NULL)
		{
			int pohon_n=objekty[Row].pohon->n;
			if(F->d.v.vrat_pocet_objektu_vyuzivajici_pohon(objekty[Row].pohon->n,1) > 1)
			{
			 //prùchod celé tabulky
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
															vypis("Tato varianta nelze uložit, musíte se nacházet ve stejné oblasti výbìru!");
														}
												}
						}
							}

					 if(pocitadlo_validace==0) 	vypis("",false);  //povolím uložení

			 }
		 }
	 }

	 }

	//KONEC VALIDAÈNÍ ÈÁSTI - HLÍDÁNÍ OBLASTÍ//

		//loadnutí default nastavení pøepínaèù
		 if(input_state==LOADING && Col>2)
	{

			//tøetí resp. první výbìrový sloupec je vždy pøedvybrán na true
			mGrid->getCheck(4,Row)->Checked=true;
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
		Invalidate();FormPaint(this);//zajistí pøekreslení bez probliku
		Form_objekt_nahled->Left=Left+Width/2-Form_objekt_nahled->Width/2;
		Form_objekt_nahled->Top=Top+Height/2-Form_objekt_nahled->Height/2;
		Form_objekt_nahled->ShowModal();
		scGPButton_OK->Enabled=true;scGPButton_storno->Enabled=true;zobrazitFrameForm=true;
	}
	else
	{
	if(input_state==FREE)
	 //pøekliknutí chechboxu pravdìpodobnì
	{
	 	calculate(Row);//zajistí pøepoèet daného øádku
	 	FormPaint(this);//zajistí pøekreslení bez probliku
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
//pro daný øádek dle nastaveného checkboxu, dopoèítá a dosadí nové hodnoty parametrù daného objektu z daného øádku, v pøípadì SaveTo -1, vrátí formou textu, oddìlené støedníky, 0 - nevrací nic, 1 uloží do binárky
UnicodeString TF_gapoV::calculate(unsigned long Row,short SaveTo)//NEWR
{
	//instance na PO_math, využívá se stejných výpoètù
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
	pm.dJ=Form_parametry_vozik->scGPNumericEdit_delka_jig->Value;//zajistit pøevod
	pm.sJ=Form_parametry_vozik->scGPNumericEdit_sirka_jig->Value;//zajistit pøevod
	pm.dP=Form_parametry_vozik->scGPNumericEdit_delka_podvozek->Value;//zajistit pøevod
	pm.Rotace=objekty[Row].rotace;
	if(objekty[Row].pohon!=NULL)
	{
																		//Rosta zajistit pøevod
		pm.RD=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[4][Form_parametry_linky->getROW(objekty[Row].pohon->n)]/60.0);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
		pm.R=F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[5][Form_parametry_linky->getROW(objekty[Row].pohon->n)])/(1+999*Form_parametry_linky->Runit);//musím brát ze stringgridu, kvùli stornu, nikoliv pøímo z dat
	}
	else
	{
		pm.RD=objekty[Row].RD;
		pm.R=0;
  }

	//optimalizace detekce a uchování volby zaškrtnutého checkboxu, aby se nemuselo vyvolávat znovu
	bool CHECK[5];
	if(mGrid->getCheck(2,Row)== NULL) 	CHECK[0]=false;
	else CHECK[0]=mGrid->getCheck(2,Row)->Checked; if(CHECK[0])tempCHECK[Row]=2;
	CHECK[1]=mGrid->getCheck(4,Row)->Checked; if(CHECK[1])tempCHECK[Row]=4;
	CHECK[2]=mGrid->getCheck(6,Row)->Checked; if(CHECK[2])tempCHECK[Row]=6;
	CHECK[3]=mGrid->getCheck(8,Row)->Checked; if(CHECK[3])tempCHECK[Row]=8;
	CHECK[4]=mGrid->getCheck(10,Row)->Checked; if(CHECK[4])tempCHECK[Row]=10;

	//volání samotného výpoètu dle volby stanovéné pomoci checkboxu
	if(CHECK[0])//mìní se M,P, zùstává aRD, RD, Rz, Rx, R, CT, DD, K
	{
		pm.P=pm.K2P(pm.K);//pozice
		pm.M=pm.Mezera();//mezera, vzhledem k tomu, že aRD,Rz,R zùstávají mezera se poèítá pouze k aktuálnímu aRD resp. RD
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
	if(CHECK[1] || CHECK[2])//mìní se aRD, RD, M, Rz, Rx resp. R, K, CT, P zùstává DD, R resp. Rx
	{
		pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);//vrátí doporuèenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteè mezi palci i takt
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
			if(CHECK[1])//zùstává R, mìní se Rx
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
	if(CHECK[3] || CHECK[4])//mìní se aRD, RD, M, Rz, Rx resp. R, DD, P zùstává K, CT, R resp. Rx
	{
		pm.RD=F->m.dopRD(pm.dJ,pm.sJ,pm.Rotace,pm.R,pm.TT,pm.RD);//vrátí doporuèenou nejbližší rychlost pohonu, k rychlosti zadané tak, aby se reflektovala rozteè mezi palci i takt
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
			if(CHECK[3])//zùstává R, mìní se Rx
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
		 case -1://uložení do textu je-li požadováno
		 {
				T=objekty[Row].short_name+";"+AnsiString(pm.CT/(1+59.0*CTunit))+";"+AnsiString(pm.RD*(1+59.0*RDunit))+";"+AnsiString(pm.DD*(1+999*DDunit))+";"+AnsiString(pm.K)+";"+AnsiString(pm.P)+";"+AnsiString(pm.MJ*(1+999*Munit))+";"+AnsiString(pm.MP*(1+999*Munit));
		 }break;
		 case 0://pouze vrátí text do bunìk      objekty[Row].
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
					if(CHECK[1] || CHECK[3])//zùstává R, mìní se Rx
					{
						mGrid->Cells[28][Row].Text = pm.R;
						mGrid->Cells[32][Row].Text = F->m.Rx2(objekty[Row].pohon->Rz,pm.R);
					}
					if(CHECK[2] || CHECK[4])//zùstává Rx, mìní se R
					{
						mGrid->Cells[32][Row].Text = F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]);
						mGrid->Cells[28][Row].Text = F->m.R(objekty[Row].pohon->Rz,F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]));
					}
				}
		 }break;
		 case 1://uložení do spojáku OBJEKTY - je-li požadováno
		 {
				Cvektory::TObjekt *O=F->d.v.vrat_objekt(objekty[Row].n);
				if(O->pohon!=NULL)
				{
					O->pohon->aRD=pm.RD;
					O->pohon->Rz=F->m.Rz(pm.RD);
					if(CHECK[1] || CHECK[3])O->pohon->Rx     = F->m.Rx2(O->pohon->Rz,pm.R);//zùstává R, mìní se Rx
					if(CHECK[2] || CHECK[4])O->pohon->roztec = F->m.R(O->pohon->Rz,F->ms.MyToDouble(Form_parametry_linky->rStringGridEd_tab_dopravniky->Cells[7][Form_parametry_linky->getROW(objekty[Row].pohon->n)]));//zùstává Rx, mìní se R
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
//---------------------------------------------------------------------------
void __fastcall TF_gapoV::scGPButton_OKClick(TObject *Sender)
{
	for(unsigned long Row=1;Row<mGrid->RowCount;Row++)
	{
		calculate(Row,1);//sice se propoèítává opakovanì, ale kvùli možnému zobrazení dat ve zkrácené formì v tabulce. lepe z ostrých dat
		//aktualizace PO, pokud je spuštìné
		if(F->pom!=NULL)
    {//nutno jako samostaný if
		if(F->pom->n==objekty[Row].n)//je spuštìné PO a je tedy nutné zavolat aktualizaci PO
		{
			F->pom=F->d.v.vrat_objekt(objekty[Row].n);//vrátí ostrá data
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
			if (scGPButton_OK->Enabled)scGPButton_OKClick(Sender);// pokud jsou zároveò splnìny podmínky pro stisk OK
			else 	MessageBeep(0); // pípnutím upozorní, že nelze
		 }break;
		 case 27:// ESC
		 {
		 	Form_parametry_linky->Button_save->Enabled=true;
			Form_parametry_linky->Button_storno->Enabled=true;
			F_gapoV->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
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
		T+=calculate(Row,-1)+"\n";//sice se propoèítává opakovanì, ale kvùli možnému zobrazení dat ve zkrácené formì v tabulce. lepe z ostrých dat
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

