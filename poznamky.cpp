//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "poznamky.h"
#include "parametry.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm_poznamky *Form_poznamky;
//---------------------------------------------------------------------------
__fastcall TForm_poznamky::TForm_poznamky(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláře
	Form_poznamky->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlačítek OK a Storno
	Form1->m.designButton(scGPButton_OK,Form_poznamky,1,2);
	Form1->m.designButton(scGPButton_storno,Form_poznamky,2,2);

	//definice barev
	clBACKGROUND=(TColor)RGB(250,250,250);
	clLOCKED	  =(TColor)RGB(128,128,128);
	clUNLOCKED	=(TColor)RGB(43,87,154);
	clTITLE=clUNLOCKED;
	rHTMLLabel_InfoText->Font->Color=clLOCKED;

	//nastavení výstupů
	Decimal=3;//počet desetinných míst
	pz="";//zástupný znak pokračování

	//zámky výchozí stav
	scGPButton_zamek_PT->ImageIndex=38;//odemčeno
	scGPButton_zamek_DD->ImageIndex=37;//zamčeno
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 13) // ENTER
	{
			Form_poznamky->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlačítko
			Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
	}
	if (Key == 27) // ESC
	{
			Form_poznamky->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlačítko
			Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
	}
}
//---------------------------------------------------------------------------
//vykreslí orámování
void __fastcall TForm_poznamky::FormPaint(TObject *Sender)
{
	Form1->m.frameForm(Form_poznamky,clWebOrange,1);
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)
	{
		mGrid->Show();//vykreslí tabulku

		//workaround - zrušení orámování okolo nepoužitých vnějších buněk
		Canvas->Pen->Width=2;
		Canvas->Pen->Color=(TColor)RGB(240,240,240);

		Canvas->MoveTo(mGrid->Left,mGrid->Top+mGrid->Rows[0].Height-2);
		Canvas->LineTo(mGrid->Left,mGrid->Top);
		Canvas->LineTo(mGrid->Left+mGrid->Columns[0].Width-2,mGrid->Top);

		Canvas->MoveTo(mGrid->Left+mGrid->Columns[5].Left,mGrid->Top+mGrid->Height);
		Canvas->LineTo(mGrid->Left+mGrid->Columns[5].Left+mGrid->Columns[5].Width-2,mGrid->Top+mGrid->Height);
	}
}
//---------------------------------------------------------------------------
//při zobrazení formuláře
void __fastcall TForm_poznamky::FormShow(TObject *Sender)
{
	input_state=NO;//při startu
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)//pro S&G
	{
		////////tabulka
		mGrid=new TmGrid(this);//vždy nutno jako první
		mGrid->Tag=5;//ID tabulky
		mGrid->Left=10;mGrid->Top=scGPPanel_hlavicka->Height+10;//vhodné jako druhé (popř. by bylo nutné překreslovat)
		mGrid->AntiAliasing_text=true;//bez toho dělá bílý blok, pozadí - posunuto, jedna se o bug v mGridu
		mGrid->DefaultColWidth*=1.3;
		mGrid->Create(7,5);//vhodné jako třetí
		mGrid->Columns[5].Width=250;//pohon následující
		mGrid->Decimal=Decimal;//počet desetinných míst u numericeditů
		mGrid->SetColumnAutoFit(0);

		////vytvoření hlavičky a názvů sloupců a všech popisků tabulky
		table_head();

		////////design - musí být až za tabulkou kvůli pozici
		scLabel_titulek->Caption=Form_parametry->scGPEdit_name->Text.UpperCase()+" - rozbor objektu ";
		mGrid->Show();//kvůli přepočítání a získání výšky a šířky výsledné tabulky
		//form
		Width=mGrid->Width+2*10+2;
		//centrování na PO form, zrušeno Left=Form_parametry->Left+Form_parametry->Width/2-Width/2;
		//defaultní design a pozicování tlačítek OK a Storno
		Form1->m.designButton(scGPButton_OK,Form_poznamky,1,2);
		Form1->m.designButton(scGPButton_storno,Form_poznamky,2,2);
		//scGPCheckBox_WTlock
		scGPCheckBox_WTlock->Visible=false;
		scGPCheckBox_WTlock->Checked=false;
		scGPCheckBox_WTlock->Top=mGrid->Top+mGrid->Height+1;
		scGPCheckBox_WTlock->Left=mGrid->Left-1;
		//memo
		scGPMemo->Left-=1;
		scGPMemo->Width=mGrid->Width+4;
		scGPMemo->Top=mGrid->Top+mGrid->Height/*+10*/+rHTMLLabel_InfoText->Height+scGPCheckBox_WTlock->Height;
		scGPMemo->Height=scGPButton_OK->Top-10-scGPMemo->Top;
		rHTMLLabel_InfoText->Top=scGPMemo->Top-rHTMLLabel_InfoText->Height;
		//ovládací prvky, +-1 pouze grafická korekce kvůli orámování tablky
		//PT
		scGPButton_zamek_PT->Visible=true;
		scGPButton_zamek_PT->ImageIndex=38;//odemčeno
		scGPButton_zamek_PT->Left=mGrid->Left+mGrid->Columns[3].Left-scGPButton_zamek_PT->Width;
		scGPButton_zamek_PT->Top=mGrid->Top+mGrid->Rows[1].Top+2;
		//DD
		scGPButton_zamek_DD->Visible=true;
		scGPButton_zamek_DD->ImageIndex=37;//zamčeno
		scGPButton_zamek_DD->Left=mGrid->Left+mGrid->Width-scGPButton_zamek_DD->Width-1;
		scGPButton_zamek_DD->Top=mGrid->Top+mGrid->Rows[3].Top+2;
		//scGPCheckBox_zaokrouhlit->Visible=true; //zatím nefunguje správně následná funkcionalita
	}
	else//pro ostatní režimy
	{
		scLabel_titulek->Caption=rHTMLLabel_InfoText->Caption;
		rHTMLLabel_InfoText->Visible=false;
		scGPMemo->Top=48;
		scGPMemo->Height=233;
		scGPCheckBox_zaokrouhlit->Visible=false;
		scGPCheckBox_STOPKA->Visible=false;
		scGPButton_zamek_PT->Visible=false;
		scGPButton_zamek_DD->Visible=false;
		scGPCheckBox_WTlock->Visible=false;
	}
	input_state=NOTHING;//po startu
}
//---------------------------------------------------------------------------
//definuje hlavičku tabulky
void TForm_poznamky::table_head()
{
	short R=0;//číslo aktuálně zpracovávaného řádku

	////převody jednotek
	AnsiString CTunitT="s";if(Form_parametry->CTunit)CTunitT="min";
	AnsiString DDunitT="m";if(Form_parametry->DDunit)DDunitT="mm";
	AnsiString RDunitT="m/s";if(Form_parametry->RDunitT)RDunitT="m/min";
	AnsiString RunitT="m";if(Form_parametry->DMunit)RunitT="mm";

	////definice buněk - hlavička total
	//pohon vlastní
	P=F->d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex);
	if(P!=NULL)
	{
		mGrid->Cells[1][R].Text=AnsiString(P->name)+" "+AnsiString(F->m.round2double(P->aRD*(1+Form_parametry->RDunitT*59),Decimal,pz))+" ["+RDunitT+"], "+AnsiString(F->m.round2double(P->roztec*(1+Form_parametry->DMunit*999),Decimal,pz))+" ["+RunitT+"]";
		mGrid->Cells[1][R].Font->Color=clLOCKED;
	}
	else
	{
		mGrid->Cells[1][R].Text="nepřiřazen";
		mGrid->Cells[1][R].Font->Color=clRed;
	}
	mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	//pohon následující
	if(F->pom->dalsi!=NULL)PD=F->pom->dalsi->pohon;else PD=F->d.v.OBJEKTY->dalsi->pohon;//ošetření pokud se nejedná o poslední prvek
	if(PD!=NULL)
	{
		mGrid->Cells[5][R].Text=AnsiString(PD->name)+" "+AnsiString(F->m.round2double(PD->aRD*(1+Form_parametry->RDunitT*59),Decimal,pz))+" ["+RDunitT+"], "+AnsiString(F->m.round2double(PD->roztec*(1+Form_parametry->DMunit*999),Decimal,pz))+" ["+RunitT+"]";
		mGrid->Cells[5][R].Font->Color=clLOCKED;
	}
	else
	{
		mGrid->Cells[5][R].Text="nepřiřazen";
		mGrid->Cells[5][R].Font->Color=clRed;
	}
	mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	//celkem
	mGrid->Cells[6][R].Text="CELKEM";mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma

	////definice buněk - hlavička ČAS
	mGrid->Cells[0][++R].Text="ČAS";mGrid->Cells[0][R].LeftMargin=4;
	mGrid->Cells[0][R].Background->Color=clBACKGROUND;mGrid->Cells[0][R].Font->Color=clLOCKED;mGrid->Cells[0][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	mGrid->Cells[1][R].Text="MT1 ["+CTunitT+"]";// - čas přesunu 1";
	mGrid->Cells[1][R].Background->Color=clBACKGROUND;
	mGrid->Cells[2][R].Text="PT ["+CTunitT+"]";// - procesní čas";
	mGrid->Cells[2][R].Background->Color=clBACKGROUND;
	mGrid->Cells[2][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	mGrid->Cells[3][R].Text="max.WT1 ["+CTunitT+"]";// - čas čekání na nejbližší palec - v případě nadefinování MT2";
	mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clTITLE;
	mGrid->Cells[4][R].Text="MT2 ["+CTunitT+"]";// - čas přesunu 2";
	mGrid->Cells[4][R].Background->Color=clBACKGROUND;
	mGrid->Cells[5][R].Text="max.WT výstup ["+CTunitT+"]";// - čas čekání na nejbližší palec - výstup";
	mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clTITLE;
	mGrid->Cells[6][R].Text="CT ["+CTunitT+"]";// - Celkový technologický čas";
	mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;
	mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	mGrid->Cells[6][++R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma

	////definice buněk - parametry - ČAS
	//MT1
	mGrid->Cells[1][R].Type=mGrid->NUMERIC;
	//PT
	mGrid->Cells[2][R].Type=mGrid->NUMERIC;mGrid->Cells[3][R].Background->Color=clWhite;mGrid->Cells[2][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	mGrid->Cells[6][3].Font->Color=clUNLOCKED;mGrid->Cells[6][4].Font->Color=clUNLOCKED;
	//WT1
	mGrid->Cells[3][R].Background->Color=clBACKGROUND;
	//MT2
	mGrid->Cells[4][R].Type=mGrid->NUMERIC;
	//WT2
	mGrid->Cells[5][R].Background->Color=clBACKGROUND;
	//CT
	mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;mGrid->Cells[6][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_CT->Value,Decimal,pz,true);

	////definice buněk - hlavička - DÉLKA
	mGrid->Cells[0][++R].Text="DÉLKA";mGrid->Cells[0][R].LeftMargin=4;
	mGrid->Cells[0][R].Background->Color=clBACKGROUND;mGrid->Cells[0][R].Font->Color=clLOCKED;mGrid->Cells[0][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	mGrid->Cells[1][R].Text="přejezd 1 ["+DDunitT+"]";
	mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clTITLE;
	mGrid->Cells[3][R].Text="přejezd 2 ["+DDunitT+"]";
	mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clTITLE;
	mGrid->Cells[6][R].Text="délka ["+DDunitT+"]";
	mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;
	mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
	mGrid->Cells[6][++R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma

	////definice buněk - parametry - DÉLKA
	//Přejezd 1
	mGrid->Cells[1][R].Type=mGrid->NUMERIC;
	//Přejezd 2
	mGrid->Cells[3][R].Type=mGrid->NUMERIC;
	//Celkem
	//mGrid->Cells[6][R].Type=mGrid->readNUMERIC;
	mGrid->Cells[6][R].Background->Color=clBACKGROUND;

	////Výpočet hodnot
	calculate(0);

	//vymazání nepoužívaných buněk
	mGrid->Cells[0][0].Background->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].TopBorder->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].LeftBorder->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].BottomBorder->Width=2;mGrid->Cells[0][0].RightBorder->Width=2;

	mGrid->Cells[5][R-1].Background->Color=(TColor)RGB(240,240,240);
	mGrid->Cells[5][R-1].TopBorder->Width=2;
	mGrid->Cells[5-1][R-1].LeftBorder->Width=2;
	mGrid->Cells[5][R-1].RightBorder->Width=2;

	mGrid->Cells[5][R].Background->Color=(TColor)RGB(240,240,240);
	mGrid->Cells[5-1][R].LeftBorder->Width=2;
	mGrid->Cells[5][R].RightBorder->Width=2;

	//Sloučení buněk
	mGrid->MergeCells(1,0,4,0);//P
	mGrid->MergeCells(0,1,0,2);//ČAS
	mGrid->MergeCells(0,3,0,4);//DÉLKA
	mGrid->MergeCells(1,R-1,2,R-1);mGrid->MergeCells(1,R,2,R);//přejezd 1
	mGrid->MergeCells(3,R-1,4,R-1);mGrid->MergeCells(3,R,4,R);//přejezd 2
	mGrid->MergeCells(5,R-1,5,R);//výsek tabulky

	//rozdělení sekcí pohony, čas a délka - šířka 2px orámování
	for(short i=0;i<=6;i++){mGrid->Cells[i][0].BottomBorder->Width=2;mGrid->Cells[i][2].BottomBorder->Width=2;}
}
//---------------------------------------------------------------------------
void TForm_poznamky::OnClick(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
//popis principu, pokud se vstopu do dané buňky a nejedná se o start (input_state==NO), nastaví input state dané vstupu, sice mGrid má tyto situace ošetřené, ale stejně e bezpečnější input_state také používat
//zámek DD se automaticky odemkne při vstupu do PT (jsou oba odemčené), jinak je vždy stav že buď jeden nebo druhý zámek, v případě, že jsou oba odemčené, a klikne se do MT1,2, či P1,2, tak dojde k defaultnímu nastavení zámků tj. DD zamčeno a PT odemčeno
void TForm_poznamky::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{                        //eliminace u startu
	if(Col==1 && Row==2 && input_state!=NO){input_state=MT1state;if(scGPButton_zamek_PT->ImageIndex==scGPButton_zamek_DD->ImageIndex){setPT(false);setDD(true);FormPaint(this);}}//funguje i bez toho, ale je toto je lepší
	if(Col==2 && Row==2 && input_state!=NO){input_state=PTstate;setDD(false);FormPaint(this);}//funguje i bez toho, ale je toto je lepší
	if(Col==4 && Row==2 && input_state!=NO){input_state=MT2state;if(scGPButton_zamek_PT->ImageIndex==scGPButton_zamek_DD->ImageIndex){setPT(false);setDD(true);FormPaint(this);}}//funguje i bez toho, ale je toto je lepší
	if(Col==1 && Row==4 && input_state!=NO){input_state=P1state;if(scGPButton_zamek_PT->ImageIndex==scGPButton_zamek_DD->ImageIndex){setPT(false);setDD(true);FormPaint(this);}}//funguje i bez toho, ale je toto je lepší
	if(Col==3 && Row==4 && input_state!=NO){input_state=P2state;if(scGPButton_zamek_PT->ImageIndex==scGPButton_zamek_DD->ImageIndex){setPT(false);setDD(true);FormPaint(this);}}//funguje i bez toho, ale je toto je lepší
}
//---------------------------------------------------------------------------
void TForm_poznamky::OnChange(long Tag,unsigned long Col,unsigned long Row)
{
	//if(Col==1 && Row==2)scLabel_titulek->Caption=mGrid->Cells[Col][Row].Text;//pouze test //scLabel_titulek->Caption=scLabel_titulek->Caption+"a"; //toto by nemělo nastat
	//funguje i bez toho, ale je toto je lepší
	if(Col==1 && Row==2 && input_state==MT1state)
	{
		calculate(MT1state);//stačilo by input_state obecně, ale toto raději pro přehlednost
		FormPaint(this);
	}
	if(Col==2 && Row==2 && input_state==PTstate && scGPButton_zamek_PT->ImageIndex==38)
	{
		calculate(PTstate);
		FormPaint(this);
	}
	if(Col==4 && Row==2 && input_state==MT2state)
	{
		calculate(MT2state);
		FormPaint(this);
	}
	if(Col==1 && Row==4 && input_state==P1state)
	{
		calculate(P1state);
		FormPaint(this);
	}
	if(Col==3 && Row==4 && input_state==P2state)
	{
		calculate(P2state);
		FormPaint(this);
	}
}
//---------------------------------------------------------------------------
//vypočítává ve S&G subparametry
void TForm_poznamky::calculate(Tinput_state input_state)
{
	 ////INPUT SEKCE - včetně převodu do SI jednotek
	 //časy
	 double CT  = F->ms.MyToDouble(Form_parametry->scGPNumericEdit_CT->Value)*(1.0+Form_parametry->CTunit*59.0);
	 double MT1 = F->ms.MyToDouble(mGrid->Cells[1][2].Text)*(1.0+Form_parametry->CTunit*59.0);
	 double PT	= F->ms.MyToDouble(mGrid->Cells[2][2].Text)*(1.0+Form_parametry->CTunit*59.0);
	 double WT1 = F->ms.MyToDouble(mGrid->Cells[3][2].Text)*(1.0+Form_parametry->CTunit*59.0);
	 double MT2 = F->ms.MyToDouble(mGrid->Cells[4][2].Text)*(1.0+Form_parametry->CTunit*59.0);
	 double WT2 = F->ms.MyToDouble(mGrid->Cells[5][2].Text)*(1.0+Form_parametry->CTunit*59.0);
	 //přejezdy
	 DD  = Form_parametry->scGPNumericEdit_delka_dopravniku->Value/(1.0+Form_parametry->DDunit*999.0);
	 double P1	= F->ms.MyToDouble(mGrid->Cells[1][4].Text)/(1+Form_parametry->DDunit*999);
	 double P2	= F->ms.MyToDouble(mGrid->Cells[3][4].Text)/(1+Form_parametry->DDunit*999);

	 ////VÝPOČETNÍ SEKCE
	 switch(input_state)
	 {
		 //při otevření formu
		 case 0:
		 {
			 //MT1+MT2
			 if(F->m.null(Form_parametry->MT)==0)//pokud nebylo MT uloženo
			 {
				 if(P!=NULL)MT1=DD/P->aRD;//pokud existuje pohon
				 else MT1=0;//pokud ne
				 MT2=0;
			 }
			 else //pokud bylo MT uloženo
			 {
				 MT1=Form_parametry->MT;
				 if(P!=NULL)MT2=DD/P->aRD-MT1;//pokud existuje pohon
				 else MT2=0;//pokud ne, bylo zde MT1
			 }
			 //WT2 - výstupní
			 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
			 else WT2=0;//pokud ne
			 //výpočet délek přejezdů
			 P1=DD;if(F->m.null(MT2)!=0 && P!=NULL)P1=MT1*P->aRD;
			 P2=0;if(F->m.null(MT2)!=0 && P!=NULL)P2=MT2*P->aRD;
			 if(F->m.null(MT2)!=0 && P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
			 //dopočítaní WT1
			 if(P!=NULL && F->m.null(MT2)!=0)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
		 }
		 break;
		 //při změně MT1
		 case MT1state:
		 {
			 if(scGPButton_zamek_PT->ImageIndex==38)//odemčeno
			 {
				 //MT2
				 if(P!=NULL)MT2=DD/P->aRD-MT1;else MT2=0;//pokud existuje pohon
				 //WT2 - výstupní
				 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
				 else WT2=0;//pokud ne
				 if(MT2>0 && P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
				 //dopočítaní WT1
				 if(P!=NULL && F->m.null(MT2)!=0)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
			 }
			 else//zamčeno
			 {
				 //WT2 - výstupní
				 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
				 else WT2=0;//pokud ne
				 if(MT2>0 && P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
				 //WT1
				 if(P!=NULL)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
				 //MT2
				 if(!scGPCheckBox_WTlock->Checked)
					 MT2=CT-MT1-PT-WT1-WT2;//čas zbývající na MT2, manipulovat lze už jen s MT2 to je časový flexibilní fond v tomto případě
				 else
					 WT1=CT-PT-MT1-MT2-WT2;//čas zbývající na WT1, manipulovat lze už jen s WT1 to je časový flexibilní fond v tomto případě
			 }
			 //výpočet délek přejezdů
			 P1=DD;if(F->m.null(MT2)!=0 && P!=NULL)P1=MT1*P->aRD;
			 P2=0;if(F->m.null(MT2)!=0 && P!=NULL)P2=MT2*P->aRD;
		 }
		 break;
		 //při změně MT2
		 case MT2state:
		 {
			 //MT1 a je PT odemčeno
			 if(scGPButton_zamek_PT->ImageIndex==38 && P!=NULL)MT1=DD/P->aRD-MT2;//pokud existuje pohon a je PT odemčeno
			 //WT2 - výstupní
			 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
			 else WT2=0;//pokud ne
			 if(MT2!=0 && P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
			 //dopočítaní WT1
			 if(P!=NULL && F->m.null(MT2)!=0)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
			 //MT1 a je PT zamčeno
			 if(scGPButton_zamek_PT->ImageIndex==37 && P!=NULL)MT1=CT-PT-WT1-WT2-MT2;//pokud existuje pohon a je PT odemčeno
			 //výpočet délek přejezdů
			 P1=DD;if(F->m.null(MT2)!=0 && P!=NULL)P1=MT1*P->aRD;
			 P2=0;if(F->m.null(MT2)!=0 && P!=NULL)P2=MT2*P->aRD;
		 }
		 break;
		 //při změně P1
		 case P1state:
		 {
			 //MT1
			 if(P!=NULL)MT1=P1/P->aRD;else MT1=0;
			 //ostatní pokud je PT odemčeno
			 if(scGPButton_zamek_PT->ImageIndex==38)//pokud je PT odemčeno
			 {
				 //MT2
				 if(P!=NULL)MT2=DD/P->aRD-MT1;else MT2=0;//pokud existuje pohon
				 //WT1
				 if(P!=NULL && F->m.null(MT2)!=0)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
				 //WT2 - výstupní
				 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
				 else WT2=0;//pokud ne
				 if(MT2!=0 && P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
			 }
			 else
			 {
					//WT1
				 if(P!=NULL)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
				 //WT2 - výstupní
				 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
				 else WT2=0;//pokud ne
				 if(P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
				 //MT2
				 MT2=CT-MT1-PT-WT1-WT2;//čas zbývající na MT2, WT1,WT2, manipulovat lze už jen s MT2 to je časový flexibilní fond v tomto případě
       }
			 //výpočet délek přejezdu
			 P2=0;if(F->m.null(MT2)!=0 && P!=NULL)P2=MT2*P->aRD;
		 }
		 break;
		 //při změně P2
		 case P2state:
		 {
			 //MT2
			 if(P!=NULL)MT2=P2/P->aRD;else MT2=0;
			 //MT1, pokud je PT odemčeno
			 if(P!=NULL && scGPButton_zamek_PT->ImageIndex==38)MT1=DD/P->aRD-MT2;//pokud je PT odemčeno a pokud existuje pohon
			 //WT1
			 if(P!=NULL && F->m.null(MT2)!=0)WT1=F->m.cekani_na_palec(0,P->roztec,P->aRD,3);else WT1=0;
			 //WT2 - výstupní
			 if(PD!=NULL)WT2=F->m.cekani_na_palec(0,PD->roztec,PD->aRD,3);//pokud existuje pohon další
			 else WT2=0;//pokud ne
			 if(F->m.null(MT2)!=0 && P==PD && !scGPCheckBox_STOPKA->Checked)WT2=0;//pokud bude MT2>0 a pokud P==PD bude WT2=0, protože se nečeká na palec, protože se jede na stejném pohonu, pokud tedy není za objektem stopka
			 //MT1, pokud je zamčeno PT,odečítám WT
			 if(P!=NULL && scGPButton_zamek_PT->ImageIndex==37)MT1=CT-PT-MT2-WT1-WT2;//pokud je PT zamčeno a pokud existuje pohon
			 //výpočet délek přejezdu
			 P1=DD;if(F->m.null(MT2)!=0 && P!=NULL)P1=MT1*P->aRD;
		 }
		 break;
		 //při změně PT
		 case PTstate:
		 {
			 //rozdíl od původní hodnoty
			 double difPT=(CT-MT1-WT1-MT2-WT2)-PT;
			 //poměrové dopočítání
			 MT1=MT1+MT1*difPT/(MT1+MT2);
			 MT2=MT2+MT2*difPT/(MT1+MT2);
			 //výpočet délek přejezdů
			 P1=0;if(P!=NULL)P1=MT1*P->aRD;
			 P2=0;if(P!=NULL)P2=MT2*P->aRD;
		 }
		 break;
	 }
	 //závěrečný výpočet PT - musí být na konci
	 if(input_state!=PTstate && scGPButton_zamek_PT->ImageIndex==38 && !scGPCheckBox_WTlock->Checked)
	 PT=CT-MT1-WT1-MT2-WT2;
	 //závěrečný výpočet DD
	 if(scGPButton_zamek_DD->ImageIndex==38)//podmínka není zatím zcela třeba
	 DD=P1+P2;

	 ////OUTPUT SEKCE včetně zaokrohlení a převodů do požadovaných jednotek a kontroly hodnot
	 bool VALID=true;
	 //časy
	 //MT
	 mGrid->Cells[1][2].Text=MT1/(1.0+Form_parametry->CTunit*59.0);
	 if(F->m.null(MT1)>0)mGrid->Cells[1][2].Font->Color=clUNLOCKED;else {mGrid->Cells[1][2].Font->Color=clRed;VALID=false;}//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
	 //PT
	 mGrid->Cells[2][2].Text=F->m.round2double(PT/(1.0+Form_parametry->CTunit*59.0),Decimal,pz);
	 if(F->m.null(PT)>0)
	 {
		if(scGPButton_zamek_PT->ImageIndex==37)
		{mGrid->Cells[2][2].Type=mGrid->readNUMERIC; mGrid->Cells[2][1].Font->Color=clLOCKED;mGrid->Cells[2][2].Font->Color=clLOCKED;}
		else
		{mGrid->Cells[2][2].Type=mGrid->NUMERIC;mGrid->Cells[2][1].Font->Color=clUNLOCKED;mGrid->Cells[2][2].Font->Color=clUNLOCKED;}
	 }
	 else mGrid->Cells[2][2].Font->Color=clRed;//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
	 //WT1
	 mGrid->Cells[3][2].Text=F->m.round2double(WT1/(1.0+Form_parametry->CTunit*59.0),Decimal,pz,true);
	 if(F->m.null(WT1)>=0)mGrid->Cells[3][2].Font->Color=clUNLOCKED;else {mGrid->Cells[3][2].Font->Color=clRed;VALID=false;}//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
	 //MT2
	 mGrid->Cells[4][2].Text=MT2/(1.0+Form_parametry->CTunit*59.0);
	 if(F->m.null(MT2)>=0)mGrid->Cells[4][2].Font->Color=clUNLOCKED;else {mGrid->Cells[4][2].Font->Color=clRed;VALID=false;}//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
	 //WT2
	 mGrid->Cells[5][2].Text=F->m.round2double(WT2/(1.0+Form_parametry->CTunit*59.0),Decimal,pz,true);
	 if(F->m.null(WT2)>=0)mGrid->Cells[5][2].Font->Color=clUNLOCKED;else {mGrid->Cells[5][2].Font->Color=clRed;VALID=false;}//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně

	 //přejezdy
	 //P1
	 mGrid->Cells[1][4].Text=P1*(1+Form_parametry->DDunit*999);
	 if(F->m.null(P1)>0)mGrid->Cells[1][4].Font->Color=clUNLOCKED;else {mGrid->Cells[1][4].Font->Color=clRed;VALID=false;}//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
	 //P2
	 mGrid->Cells[3][4].Text=P2*(1+Form_parametry->DDunit*999);
	 if(F->m.null(P2)>=0)mGrid->Cells[3][4].Font->Color=clUNLOCKED;else {mGrid->Cells[3][4].Font->Color=clRed;VALID=false;}//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
	 //Délka
	 mGrid->Cells[6][4].Text=F->m.round2double(DD*(1+Form_parametry->DDunit*999),Decimal,pz,true);
	 //mGrid->Cells[6][4].Text=DD*(1+Form_parametry->DDunit*999);
	 if(scGPButton_zamek_DD->ImageIndex==37){mGrid->Cells[6][3].Font->Color=clLOCKED;mGrid->Cells[6][4].Font->Color=clLOCKED;}else {mGrid->Cells[6][3].Font->Color=clUNLOCKED;mGrid->Cells[6][4].Font->Color=clUNLOCKED;}
	 if(F->m.null(DD)<0){mGrid->Cells[6][3].Font->Color=clRed;VALID=false;}

	 //volba stopka se zobrazí pokud, je to nutné pro rozhodnutí, zda se bude čekat či nikoliv (to je potřeba rozhodnout u přechodu na stejný pohon za předpokladu, že se předpokládá přejezd 2, jinak je jisté, že je stopka automaticky)
	 if(P==PD && MT2!=0)
	 {   //výchozí stav stopky vhodno nastavit dle PO stopky (až bude zobrazena)
		scGPCheckBox_STOPKA->Visible=true;
		scGPCheckBox_STOPKA->Top=mGrid->Top+mGrid->Height+2;
	 	scGPCheckBox_STOPKA->Left=mGrid->Left+mGrid->Columns[5].Left-scGPCheckBox_STOPKA->OptionsChecked->ShapeSize/2;
	 }
	 else scGPCheckBox_STOPKA->Visible=false;

	 //VALIDACE
	 if(VALID)scGPButton_OK->Enabled=true;else scGPButton_OK->Enabled=false;
}
//---------------------------------------------------------------------------
//Ctrl + A u Mema
void __fastcall TForm_poznamky::scGPMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Shift.Contains(ssCtrl) && Key==65)scGPMemo->SelectAll();
}
//---------------------------------------------------------------------------
//přepíná zobrazení mezi počtem 3 a 10 desetinných míst - nefunguje správně nedokončeno
void __fastcall TForm_poznamky::scGPCheckBox_zaokrouhlitClick(TObject *Sender)
{
	if(scGPCheckBox_zaokrouhlit)Decimal=3;else Decimal=10;
	mGrid->Decimal=Decimal;
	table_head();
	FormPaint(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::scGPCheckBox_STOPKAClick(TObject *Sender)
{
	calculate(input_state);
	FormPaint(this);
}
//---------------------------------------------------------------------------
//přepínání zámku PT a DD
void __fastcall TForm_poznamky::scButton_zamek_PTaDDClick(TObject *Sender)
{
	if(scGPButton_zamek_PT->ImageIndex==38)//když PT odemčeno, tak
	{
		setPT(true); //zamkni
		setDD(false);//odemkni
	}
	else//když zamčeno, tak
	{
		setPT(false);//odemkni
		setDD(true); //zamkni
	}
	FormPaint(this);
}
//---------------------------------------------------------------------------
void TForm_poznamky::setPT(bool locked)
{
	if(locked){scGPButton_zamek_PT->ImageIndex=37;mGrid->Cells[2][2].Type=mGrid->readNUMERIC;mGrid->Cells[2][1].Font->Color=clLOCKED;mGrid->Cells[2][2].Font->Color=clLOCKED;mGrid->Cells[2][2].Background->Color=clBACKGROUND;}//zamkni
	else {scGPButton_zamek_PT->ImageIndex=38;mGrid->Cells[2][2].Type=mGrid->NUMERIC;mGrid->Cells[2][1].Font->Color=clUNLOCKED;mGrid->Cells[2][2].Font->Color=clUNLOCKED;mGrid->Cells[2][2].Background->Color=clWhite;}//odemkni
	scGPCheckBox_WTlock->Visible=locked;
	if(!locked)scGPCheckBox_STOPKA->Checked=false;//při odemčení PT odškrtnout
}
//---------------------------------------------------------------------------
void TForm_poznamky::setDD(bool locked)
{
	if(locked){scGPButton_zamek_DD->ImageIndex=37;/*mGrid->Cells[6][4].Type=mGrid->NUMERIC;*/mGrid->Cells[6][3].Font->Color=clLOCKED;mGrid->Cells[6][4].Font->Color=clLOCKED;mGrid->Cells[6][4].Background->Color=clBACKGROUND;}//zamkni
	else {scGPButton_zamek_DD->ImageIndex=38;/*mGrid->Cells[6][4].Type=mGrid->NUMERIC;*/mGrid->Cells[6][3].Font->Color=clUNLOCKED;mGrid->Cells[6][4].Font->Color=clUNLOCKED;/*mGrid->Cells[6][4].Background->Color=clWhite;*/}//odemkni
}
//---------------------------------------------------------------------------
//po stisku OK, vrácení do PP - DD
void __fastcall TForm_poznamky::scGPButton_OKClick(TObject *Sender)
{
	Form_parametry->scGPNumericEdit_delka_dopravniku->Value=DD*(1+Form_parametry->DDunit*999);
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::FormClose(TObject *Sender, TCloseAction &Action)
{
	mGrid->Delete();//pokud chci odstranit a nechci použít na další použití
	P=NULL;delete P;
	PD=NULL;delete PD;
}
//---------------------------------------------------------------------------
//
void __fastcall TForm_poznamky::scGPCheckBox_WTlockClick(TObject *Sender)
{
	if(scGPButton_zamek_PT->ImageIndex==37 && scGPCheckBox_STOPKA->Checked)//událost nastane pouze pokud je zámek zamčen a je checkbox zaškrtnut
	{
		calculate(input_state);//zavolá poslední akci
	}
}
//---------------------------------------------------------------------------

