//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
#include "vykresli.h"
#include <Clipbrd.hpp>
#include "gapoTT.h"
#include "gapoV.h"
#include "gapoR.h"
#include "unit2.h"
#include "unitX.h"
#include "poznamky.h"
#include "parametry_linky.h"
#include "katalog_dopravniku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TmGrid *mGrid;
//---------------------------------------------------------------------------
//konstruktor
TmGrid::TmGrid(TForm *Owner)
{
	Form=Owner;
	//přidělené události
	//Form->OnMouseMove=&OnMouseMove;//odstaveno
	////nastavení TABULKY
	Tag=0;//ID formuláře, v kterém je tabulka či tabuky daného formuláře volány
	ID=0;//ID konkrétní tabulky, v jednom formuláři vhodné unikátní číslo, mimo formuláře totožná hodnota nevadí (využitelné např. pokud bude více tabulek, tak se bude vědět, v jaké došlo ke kliku)
	Left=0;Top=0;//umístění celé komponenty
	RowCount=0;ColCount=0;//počet řádků a sloupců
	DefaultColWidth=90,DefaultRowHeight=25;//výchozí výška a šířka řádku
	Row=0;Col=0;//aktuální řádek a sloupec
	AntiAliasing_grid=false;
	AntiAliasing_text=true;
	VisibleComponents=true;
	SetColumnAutoFitColIdx=-3;//nastaví šířku buněk daného sloupce dle parametru ColIdx, -3 = nepřizpůsobuje se velikost a užije se defaultColWidth,-2 všechny sloupce stejně podle nejširšího textu, -1 přizpůsobuje se každý sloupec individuálně, 0 a více jen konkrétní sloupec uvedený pomoc ColIdx
	preRowInd=-1;preColInd=-1;
	Decimal=3;//implicitní počet desetinných míst u numericeditů
	IntegerDecimalNull=false;//pokud je výše uvedené Decimal na hodnotu vyšší než 0, toto nastavuje zda se nuly doplní do počtu decimál i u celých čísel
	clHighlight=(TColor)RGB(226,122,21);//(TColor)RGB(225,128,0);//(TColor)RGB(226,122,21);//(TColor)RGB(0,120,215);//přednastavená barva zvýraznění, slouží i pro nastavení barvy focusu komponent, původní tmavá RGB(43,87,154)
	//orámování - default
	TBorder defBorder;
	defBorder.Color=(TColor)RGB(200,200,200);
	defBorder.Width=1;
	defBorder.Style=psSolid;
	Border=defBorder;Border.Width=2;
	Highlight=false;

	////nastavení defalní BUŇKY
	//vytvoří novou buňku (alokuje ji paměť)
	CreateCell(DefaultCell);
	//typ
	DefaultCell.Type = DRAW;//defaultní komponenta
	//běžný text
	DefaultCell.Font->Size=12;
	DefaultCell.Font->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.Font->Orientation=0;
	DefaultCell.Font->Style=TFontStyles();
	DefaultCell.Font->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	DefaultCell.Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.Font->Name="Arial";
	//záporné hodnoty
	DefaultCell.isNegativeNumber->Size=DefaultCell.Font->Size;
	DefaultCell.isNegativeNumber->Color=clRed;//pozor zde rovnou nastaveno na výchozí hodnotu červené barvy
	DefaultCell.isNegativeNumber->Orientation=0;
	DefaultCell.isNegativeNumber->Style=TFontStyles();
	DefaultCell.isNegativeNumber->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	DefaultCell.isNegativeNumber->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isNegativeNumber->Name=DefaultCell.Font->Name;
	//nulová hodnota
	DefaultCell.isZero->Size=DefaultCell.Font->Size;
	DefaultCell.isZero->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.isZero->Orientation=0;
	DefaultCell.isZero->Style=TFontStyles();
	DefaultCell.isZero->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	DefaultCell.isZero->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isZero->Name=DefaultCell.Font->Name;
	//odkaz
	DefaultCell.isLink->Size=DefaultCell.Font->Size;
	DefaultCell.isLink->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.isLink->Orientation=0;
	DefaultCell.isLink->Style=TFontStyles();
	DefaultCell.isLink->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	DefaultCell.isLink->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isLink->Name=DefaultCell.Font->Name;
	//aktivní odkaz
	DefaultCell.isActiveLink->Size=DefaultCell.isLink->Size;
	DefaultCell.isActiveLink->Color=DefaultCell.isLink->Color;
	DefaultCell.isActiveLink->Orientation=DefaultCell.isLink->Orientation;
	DefaultCell.isActiveLink->Style=TFontStyles()<<fsBold;
	DefaultCell.isActiveLink->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejně široké
	DefaultCell.isActiveLink->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isActiveLink->Name=DefaultCell.isLink->Name;
	//pozice případného linku
	DefaultCell.LinkCoordinateStart=TPoint(-1,-1);//kvůli uložení citelné oblasti pro link dané buňky
	DefaultCell.LinkCoordinateEnd=TPoint(-1,-1);//kvůli uložení citelné oblasti pro link dané buňky
	//samotný text
	DefaultCell.TextPositon.X=0;
	DefaultCell.TextPositon.Y=0;
	DefaultCell.Text="";
	//zarovnání
	DefaultCell.Align=CENTER;
	DefaultCell.Valign=MIDDLE;
	DefaultCell.TopMargin=1;
	DefaultCell.BottomMargin=1;
	DefaultCell.LeftMargin=1;
	DefaultCell.RightMargin=1;
	//pouze indikuje, zda je buňka sloučena, či nikoliv, slouží jako pomůcka při vykreslování orámování sloučených buněk
	DefaultCell.MergeState=false;
	//pokud je nastaveno na true, nelze vepsat jinou hodnotu než číselnou (to včetně reálného čísla)
	DefaultCell.InputNumbersOnly=false;
	//výchozí stav zvýraznění buňky
	DefaultCell.Highlight=false;
	//pozadí
	DefaultCell.Background->Color=clWhite;
	DefaultCell.Background->Style=bsSolid;
	DefaultCell.isEmpty->Color=clWhite;
	DefaultCell.isEmpty->Style=bsSolid;
	//orámování
	*DefaultCell.TopBorder=defBorder;
	*DefaultCell.BottomBorder=defBorder;
	*DefaultCell.LeftBorder=defBorder;
	*DefaultCell.RightBorder=defBorder;
	//hint
	DefaultCell.ShowHint=false;//výchozí stav zobrazení hintu dané buňky

	////HINT
	Hint=new TscHTMLLabel(Form);//TrHTMLLabel(Form);
	Hint->Tag=-1;//name musí být však až později, klvůli ID, které ještě  nyní neexistuje
	Hint->Visible=false;
	Hint->Color=(TColor)RGB(255,255,156);
	Hint->Font->Size=12;
	if(Hint->Font->Name=="Roboto Cn")Hint->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else Hint->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	Timer=new TTimer(Form);
	Timer->OnTimer=&OnTimer;
	SleepHint=750;//zpoždění zobrazení Hintu v ms

	////EXTENDED BUTTON - rozšířené tlačítko pro umístění libovolné funkcionality a glyphu
	exBUTTON= new TscGPGlyphButton(Form);
	exBUTTON->Left=-50;exBUTTON->Top=-50;
	exBUTTON->Width=25;exBUTTON->Height=25;
	exBUTTON->Visible=exBUTTONVisible=false;//stav zobrazení či skrytí exBUTTNU, nepoužívat přímo exBUTTON->Visible, ale toto exBUTTONVisible!
	exBUTTON->Tag=-2;//name musí být však až později, kvůli ID, které ještě  nyní neexistuje
	exBUTTON->TransparentBackground=false;
	exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
	exBUTTON->GlyphOptions->NormalColor=clBlack;
	exBUTTON->GlyphOptions->NormalColorAlpha=200;
	exBUTTON->Options->FrameNormalColor=defBorder.Color;//(TColor)RGB(43,87,154);
	exBUTTON->Options->FrameNormalColorAlpha=255;
	exBUTTON->Options->NormalColor=m.clIntensive((TColor)RGB(128,128,128),105);//(TColor)RGB(43,87,154);
	exBUTTON->Options->NormalColorAlpha=255;
	exBUTTON->Options->FrameWidth=Border.Width;//orámování stejně široké jako orámování tabulky
	exBUTTON->OnClick=&getTagOnClick;
	exBUTTON->Parent=Form;
	exBUTTONalign=RIGHT;//pozice rozšířeného tlačítka vůči tabulce
	exBUTTONvalign=BOTTOM;//pozice rozšířeného tlačítka vůči tabulce
	exBUTTONLockPosition=false;//uzamkne pozici exButtonu (použito při updatu)

	////POZNÁMKA - výchozí nastavení
	Note.Font=new TFont();
	*Note.Font=*DefaultCell.Font;
	Note.Font->Size=11;
	Note.Font->Color=clRed;
	Note.NoteArea=TRect(-1,-1,-1,-1);
	Note.margin_left=Note.margin_right=Note.margin_bootom=Note.margin_top=0;

	////BUFFERování tabulky
	Redraw=true;buffer=false;
	Raster=new Graphics::TBitmap;
}
//---------------------------------------------------------------------------
//destruktor, probíhá při ukončování programu, tj. zvážit zda není pozdě
TmGrid::~TmGrid()
{
	Delete();
}
//---------------------------------------------------------------------------
//vytvoří tabulku
void TmGrid::Create()
{
	//alokace dvourozmerneho dynamickeho pole
	Cells = new TCells*[ColCount];//vytvoří dynamické pole ukazatelu typu TCells dle počtu požadovaných sloupců
	for(unsigned long X=0;X<ColCount;X++)Cells[X] = new TCells[RowCount];//pruchod po sloupcich, slupcům dynamamického pole alokuje pamět pro jednotlivé řádky- cyklus musí být samostatně
	//alokace jednorozměrného dynamického pole
	Columns = new TColumns[ColCount];
	Rows = new TRows[RowCount];
	//smazání + nastavení hodnot musí být v extra cyklu
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
		{
			////BUŇKY
			//Cells[X][Y]=DefaultCell; //- nelze, takto převezme celý ukazatel a CreateCell(Cells[X][Y]); alokovalo Border znovu
			//alokace paměti
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();
			Cells[X][Y].isNegativeNumber=new TFont();
			Cells[X][Y].isZero=new TFont();
			Cells[X][Y].isLink=new TFont();
			Cells[X][Y].isActiveLink=new TFont();
			Cells[X][Y].isEmpty=new TBrush();
			CopyAreaCell(DefaultCell,Cells[X][Y],true);//kopie vlastností, bez orámování
			////orámování buněk ukazatelem
			CreateLinkBorder(X,Y,DefaultCell);
			////ŘÁDKY
			Rows[Y].Height=DefaultRowHeight;
			Rows[Y].Visible=true;
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Y*DefaultRowHeight;else Rows[0].Top=0;
		}
		////SLOUPCE
		Columns[X].Width=DefaultColWidth;
		Columns[X].Visible=true;
		if(X>0){Columns[X].Left=Columns[X-1].Left+X*DefaultColWidth;}else Columns[0].Left=0;
	}
	bufColCount=ColCount;bufRowCount=RowCount;//určeno při další realokaci pole
	preTop=Top;preLeft=Left;//zaloha úvodní pozice
	deleteMark=false;//detekce že dochází k odstraňování mGridu
}
//---------------------------------------------------------------------------
//přetížená metoda - vytvoří tabulku s předepsaným počtem sloupců a řádků
void TmGrid::Create(unsigned long ColCount,unsigned long RowCount)
{
	TmGrid::ColCount=ColCount;TmGrid::RowCount=RowCount;//předání do globální proměnné
	Create();
}
//---------------------------------------------------------------------------
//patřičně prolinkuje orámování, že sousední orámování má ukazatel na totožný objekt, vzor orámvání získá dle refCell
void TmGrid::CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell)
{
	//top (přebírá, propojuje se s horním, nejedná-li se o první)
	if(Y==0){Cells[X][Y].TopBorder=new TBorder;*Cells[X][Y].TopBorder=*refCell.TopBorder;}
	else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na společné ohraničení, jedná se o jeden objekt
	//botom
	Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*refCell.BottomBorder;
	//left (přebírá, propojuje se s levým, nejedná-li se o první)
	if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*refCell.LeftBorder;}
	else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na společné ohraničení, jedná se o jeden objekt
	//right
	Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*refCell.RightBorder;
}
//patřičně prolinkuje orámování, že sousední orámování má ukazatel na totožný objekt, vzor orámvání získá dle refCell
void TmGrid::CreateLinkBorder2(unsigned long X,unsigned long Y,TCells &refCell)
{
	//top (přebírá, propojuje se s horním, nejedná-li se o první)
	try{
	//if(refCell.TopBorder==NULL)ShowMessage("null");
	if(Y==0){Cells[X][Y].TopBorder=new TBorder;/**Cells[X][Y].TopBorder=*refCell.TopBorder;*/}
	else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na společné ohraničení, jedná se o jeden objekt
	//botom
	Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*refCell.BottomBorder;
	//left (přebírá, propojuje se s levým, nejedná-li se o první)
	if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*refCell.LeftBorder;}
	else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na společné ohraničení, jedná se o jeden objekt
	//right
	Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*refCell.RightBorder;
	}catch(...)
	{
			ShowMessage(Y);
	}
}
//---------------------------------------------------------------------------
//vytvoří novou buňku (alokuje ji paměť)
void TmGrid::CreateCell(TCells &NewCell)
{
	NewCell.Font=new TFont();
	NewCell.isNegativeNumber=new TFont();
	NewCell.isZero=new TFont();
	NewCell.isLink=new TFont();
	NewCell.isActiveLink=new TFont();
	NewCell.Background=new TBrush();
	NewCell.isEmpty=new TBrush();
	NewCell.TopBorder=new TBorder;
	NewCell.BottomBorder=new TBorder;
	NewCell.LeftBorder=new TBorder;
	NewCell.RightBorder=new TBorder;
}
//---------------------------------------------------------------------------
//smaže buňku z paměti
void TmGrid::DeleteCell(TCells &DelCell)
{
	DelCell.TopBorder=NULL;			delete DelCell.TopBorder;
	DelCell.BottomBorder=NULL;  delete DelCell.BottomBorder;
	DelCell.LeftBorder=NULL;    delete DelCell.LeftBorder;
	DelCell.RightBorder=NULL;   delete DelCell.RightBorder;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//odstraní tabulku, přidružené komponenty a ukazatel na mGrid z paměti
void TmGrid::Delete()
{
	try
	{
		deleteMark=true;//detekce že dochází k odstraňování mGridu
		//odstranění v tabulce použitých komponent
		DeleteComponents();
		delete (Raster);
		Hint->Free();Hint=NULL;delete Hint;
		Timer->Free();Timer=NULL;delete Timer;
		exBUTTON->Free();exBUTTON=NULL;delete exBUTTON;//pozor nesmí mít při ukončování formu focus
		//uvolnění paměti
		DeleteTable();
		DeleteCell(DefaultCell);
		mGrid=NULL; delete mGrid;
	}
	catch(...)
	{/*MessageBeep(0);*/}
}
//---------------------------------------------------------------------------
//odstraní pouze tabulku, pomocná metoda výše uvedené a destruktoru
void TmGrid::DeleteTable()
{
	for(unsigned long X=0;X<ColCount;X++)//po řádcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
		{
			DeleteCell(Cells[X][Y]);
		}
		Cells[X]=NULL; delete Cells[X];
	}
	Cells=NULL; delete Cells;
	Columns=NULL; delete Columns;
	Rows=NULL; delete Rows;
}
//---------------------------------------------------------------------------
//metoda vhodná na umístění do rodičovského formuláře Form->FormMouseMove,vrací do globálních proměnných index aktuálního sloupce a řádku
void TmGrid::MouseMove(int X, int Y)
{
	////předání do globálních proměnných pro případné použití
	Col=GetIdxColumn(X,Y);Row=GetIdxRow(X,Y);
	TmGrid::X=X;TmGrid::Y=Y;//předání do globální proměnné
	//zobrazení hintu na DRAW
	Timer->Enabled=false;//ruší případné čekání na zobrazení Hintu
	if(Col>-1 && Row>-1)
	{				 //ShowMessage(AnsiString(preColInd)+"_"+AnsiString(Col)+AnsiString(preRowInd)+"_"+AnsiString(Row));
		if(Cells[Col][Row].ShowHint && Cells[Col][Row].Type==DRAW && VisibleComponents && CheckLink(X,Y)==TPoint(-1,-1))//pokud je požadavek na zobrazení hintu na buňce typu DRAW a pokud v dané citelné oblasti není odkaz
		{
			if(Hint->Visible==false ||  preColInd!=Col || preRowInd!=Row)
			{
				Hint->Caption=Cells[Col][Row].Hint;
				Hint->Name="mGrid_HINT_"+AnsiString(ID);
				//Hint->Left=Left+Columns[X].Left+10;Hint->Top=Top+Rows[Y].Height+30;//nefunguje správně a problikává nerozumím příčině
				Hint->Left=X+10;Hint->Top=Y+5;
				Hint->Parent=Form;//musí být až na konci kvůli nakreslení Hintu až nahoru
				//timer zajišťuje volání Hint->Visible=true;
				Timer->Interval=SleepHint;//zpoždění zobrazení Hintu v ms;
				Timer->Enabled=true;
			}
		}
		else Hint->Visible=false;

		//test pokus převod DRAW do EDITu bez přebliknutí při přejetí přes danou buňku myší, mělo by význam při udržování komponent v draw kvůli lepšímu vzhledu
		//if(Cells[Col][Row].Type==DRAW)SetEdit(TRect(Left+Columns[Col].Left,Top+Rows[Row].Top,Left+Columns[Col].Left+Columns[Col].Width,Top+Rows[Row].Top+Rows[Row].Height),Col,Row,Cells[Col][Row]);
	}
	else Hint->Visible=false;

	////ukládání předchozích hodnot
	preColInd=Col; preRowInd=Row;
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::OnMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	 //v konstruktoru odstaveno  propojení Form->OnMouseMove=&OnMouseMove;, bralo pouze poslední tabulku
}
//---------------------------------------------------------------------------
//zajistí vykreslení celé tabulky
void TmGrid::Show(TCanvas *Canvas)
{
	//if(mGrid!=NULL)
	{
		if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k požadavku na změnu počtu řádků a sloupců volá se realokace
		{
			realock();
			Form->Refresh();
		}

		//nastavení šířky sloupců a výšky řádků+autofit sloupců nastaví Columns[X].ColWidth
		SetColRow();//nastaví velikost sloupců a řádků dle aktuálního nastavení a potřeby

		//ošetření proti situaci AntiAliasing_text=true; a AntiAliasing_grid=false, která nemůže nastat resp. byla by zbytečná
		if(AntiAliasing_text==false)AntiAliasing_grid=false;

		//pokud Canvas není definován, je předpokládáno kreslení přímo do Form, kde je mGrid zobrazován
		if(Canvas==NULL)Canvas=Form->Canvas;

		//samotné vykreslení
		if(AntiAliasing_grid==false && AntiAliasing_text==false)
		{
			Draw(Canvas);
			DrawNote(Canvas);
		}
		else
		{
			short O=Border.Width-1;//pozor na dvou místech, zvětšení oblasti o orámování, nepřidávat +(short)Highlight
			if(Redraw/* || !buffer*/)//pokud je požadováno vykreslení nebo není dovoleno buffrování
			{
				////vykreslení pozadí, komponent a textu buněk
				Cantialising a;
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				bmp_in->Width=Width*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				bmp_in->Height=Height*3+(Note.Text!="")*(Note.NoteArea.Height());//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu  + příčítám výšku poznámky, je-li k dispozici
				//bmp_in->Canvas->Brush->Color=clRed;
				//bmp_in->Canvas->FillRect(TRect(0,0,bmp_in->Width,bmp_in->Height));
				Draw(bmp_in->Canvas);
				DrawNote(bmp_in->Canvas);
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
				Canvas->Draw(Left,Top,bmp_out);
				delete (bmp_in);delete (bmp_out);//velice nutné

				////vykreslení gridu
				if(AntiAliasing_grid==false && AntiAliasing_text==true)DrawGrid(Canvas);//kreslí se až nahoru, nekreslím do bmp_in, aby neprošlo přes AA, a nekreslím do bmp_out, protože má šírší okraj
				//MessageBeep(0);
				////uložení tabulky (doposud vykresleného) do bufferu resp. rasteru
				Buffer(buffer);
			}
			else Canvas->Draw(Left-O,Top-O,Raster);//vykreslení tabulky z bufferu
		}
		//zaloha úvodní pozice
		preTop=Top;preLeft=Left;
	}

	////HIGHLIGHT TABULKY - musí být tady, protože je mimo vykreslovanou oblast tabulky
	if(Highlight)HighlightTable(Canvas,Border.Color,Border.Width,2);
}
//---------------------------------------------------------------------------
//zda se bude tabulka ukladat do rastrového bufferu, pro urychlení vykreslování
void TmGrid::Buffer(bool status)
{
	if(status)
	{
		buffer=true;
		if(buffer)
		{
			short O=Border.Width-1;//pozor na dvou místech
			//AE-aktiv area, pracovní plocha bez komponent - předělat do univerzálna //zde provizorně ztráta univerzality mGridu, kvůli tomu, aby se nevykresloval do posun komponenty ze statusbarů
			//int W=F->scSplitView_LEFTTOOLBAR->Width;short H=F->scGPPanel_mainmenu->Height;short Gh=F->scGPPanel_bottomtoolbar->Height-6;//-6=WA, z nějaké důvodu to chce odebrat, aby byla posouváná plocha kompletní
			//int CW=F->ClientWidth;int CH=F->ClientHeight;
			//Graphics::TBitmap *AE_bmp=new Graphics::TBitmap;
			//AE_bmp->Width=ClientWidth;AE_bmp->Height=ClientHeight-H-Gh;//velikost pan plochy
			//AE_bmp->Canvas->CopyRect(TRect(0+W,0+H,ClientWidth,ClientHeight-H-Gh),Form->Canvas,TRect(0+W,0+H,ClientWidth,ClientHeight-H-Gh));//uloží pan výřez
			//Raster
			Raster->Width=Width+O*2;Raster->Height=Height+O*2;//+O*2 kvůli většímu orámování než je bmp
			Raster->Canvas->CopyRect(TRect(0,0,Raster->Width,Raster->Height),Form->Canvas,TRect(Left-O,Top-O,Left+Raster->Width-O,Top+Raster->Height-O));
			//Raster->Canvas->Font->Size=20;Raster->Canvas->Font->Color=clRed;Raster->Canvas->TextOutW(0,0,"toto je raster");//smazat pouze pro test
			//Raster->Canvas->CopyRect(TRect(0,0,Raster->Width,Raster->Height),AE_bmp->Canvas,TRect(Left-O,Top-O,Left+Raster->Width-O,Top+Raster->Height-O));
			//delete (AE_bmp);
		}
	}
	else buffer=false;
}
//---------------------------------------------------------------------------
//zajistí vyvolání překreslení celé tabulky
void TmGrid::Refresh()
{
	 Redraw=true;
	 Show();
}
//---------------------------------------------------------------------------
void TmGrid::Update()
{
	if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k požadavku na změnu počtu řádků a sloupců volá se realokace
	{
		realock();
		Form->Refresh();
	}

	//nastavení šířky sloupců a výšky řádků+autofit sloupců nastaví Columns[X].ColWidth
	SetColRow();//nastaví velikost sloupců a řádků dle aktuálního nastavení a potřeby

	//vykreslí se do Canvasu bitmapy o nulových rozměrech, tj. nedojde k překreslení
	Graphics::TBitmap *bmp_temp=new Graphics::TBitmap;
	bmp_temp->Width=0;bmp_temp->Height=0;
	Draw(bmp_temp->Canvas);
	delete (bmp_temp);

	//zaloha úvodní pozice
	preTop=Top;preLeft=Left;
}
//---------------------------------------------------------------------------
//zajistí vykreslení celé tabulky včetně gridu a exBUTTONu a poznámky pod čarou
void TmGrid::Draw(TCanvas *C)
{
	//////TABULKA - orámování celé, pouze nastavuje jednotlivým buňkám jejich orámování
	if(Border.Width>0)//pokud je border požadován
	{
		TCells RefCell;CreateCell(RefCell);
		*RefCell.TopBorder=*RefCell.LeftBorder=*RefCell.RightBorder=*RefCell.BottomBorder=Border;
		SetRegion(RefCell,0,0,ColCount-1,RowCount-1);
		DeleteCell(RefCell);
	}

	//////BUŇKY jednotlivé
	short Zoom_g=1; if(AntiAliasing_grid)Zoom_g=3;
	short Zoom_b=1; if(AntiAliasing_text)Zoom_b=3;
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		if(!Columns[X].Visible || Columns[X].Width==0){Columns[X].Width=0;continue;}//skrytí sloupce, zatím ale nefunguje patřičně orámování
		////oblast buňky
		TRect R;//grid
		TRect Rt;//text
		TRect Rb;//text
		TRect Rc;//componenty
		R.Left	=	Left+Columns[X].Left*Zoom_g;
		Rt.Left	=	Columns[X].Left*Zoom_b;//zde není Left celé tabulky, protože se pozicije na pozici levého horního rohu tabulky celá bmp, takže zde musí být pouze souřadnice v rámci tabulku, nikoliv absolutně v celém formu
		Rb.Left	=	Columns[X].Left*Zoom_b;//zde není Left celé tabulky, protože se pozicije na pozici levého horního rohu tabulky celá bmp, takže zde musí být pouze souřadnice v rámci tabulku, nikoliv absolutně v celém formu
		Rc.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_g;
		Rt.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;//zde není Left celé tabulky, protože se pozicije na pozici levého horního rohu tabulky celá bmp, takže zde musí být pouze souřadnice v rámci tabulku, nikoliv absolutně v celém formu
		Rb.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;//zde není Left celé tabulky, protože se pozicije na pozici levého horního rohu tabulky celá bmp, takže zde musí být pouze souřadnice v rámci tabulku, nikoliv absolutně v celém formu
		Rc.Right	=	Left+(Columns[X].Left+Columns[X].Width);

		for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
		{  //není doděláno ve vztahu s níže uvedeným
			if(!Rows[Y].Visible || Rows[Y].Height==0){Rows[Y].Height=0;SetVisibleComponent(X,Y,false);}//skrytí řádku, zatím ale možná nefunguje patřičně orámování
			else//zobrazený řádek
			{
				////oblast a umístění buňky
				if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//výpočet horního okraje buňky dle buňky předchozí
				R.Top			=	Top+Rows[Y].Top*Zoom_g;
				Rt.Top		=	Rows[Y].Top*Zoom_b;//zde není Top celé tabulky, protože se pozicije na pozici levého horního rohu tabulky celá bmp, takže zde musí být pouze souřadnice v rámci tabulku, nikoliv absolutně v celém formu
				Rb.Top		=	Rows[Y].Top*Zoom_b;
				Rc.Top		=	Top+Rows[Y].Top;
				R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_g;
				Rt.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;//zde není Top celé tabulky, protože se pozicije na pozici levého horního rohu tabulky celá bmp, takže zde musí být pouze souřadnice v rámci tabulku, nikoliv absolutně v celém formu
				Rb.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
				Rc.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

				////barva pozadí buňky                 //neplatí pro margované položky - zatím nedokonalé
				if(Cells[X][Y].Text=="" && Cells[X][Y].MergeState==false)C->Brush->Color=Cells[X][Y].isEmpty->Color;//podmíněné formátování
				else C->Brush->Color=Cells[X][Y].Background->Color;//vyplněná buňka
				C->Brush->Style=Cells[X][Y].Background->Style;
				C->FillRect(Rb);

				////komponenta v buňce
				SetComponents(C,Rc,Rt,X,Y,Cells[X][Y]);

				////orámování buňky
				//používám duplicitně (v DrawGrid) zde, v případě totálně vypnutého AA nebo totálně zapnutého AA, v takovém případě potom neběží DrawGrid, DrawGrid běží jenom v momentu AntiAliasing_text=false a AntiAliasing_grid=true
				if(!(AntiAliasing_grid==false && AntiAliasing_text==true))
				{
					DrawCellBorder(C,X,Y,R);
				}
			}
		}
	}

	////exBUTTON - pozice - ještě odladit border
	Width=getWidth();
	if(VisibleComponents && exBUTTONVisible)
	{
		if(!exBUTTONLockPosition)
		{
			switch(exBUTTONalign)
			{
				case LEFT:exBUTTON->Left=Left;break;
				case CENTER:exBUTTON->Left=Left+(Width-exBUTTON->Width)/2;break;
				case RIGHT:exBUTTON->Left=Left+Width-exBUTTON->Width+m.round(Border.Width/2.0);break;
			}
			switch(exBUTTONvalign)
			{
				case TOP:exBUTTON->Top=Top-exBUTTON->Height;break;
				case MIDDLE:exBUTTON->Top=Top+(Rows[RowCount-1].Top+Rows[RowCount-1].Height+Border.Width-exBUTTON->Height)/2;break;
				case BOTTOM:exBUTTON->Top=Top+(Rows[RowCount-1].Top+Rows[RowCount-1].Height)-m.round(Border.Width/2.0);break;
			}
		}
		exBUTTON->Visible=true;
	}else exBUTTON->Visible=false;
}
//---------------------------------------------------------------------------
//zajistí vykreslení jen gridu
void TmGrid::DrawGrid(TCanvas *C)
{
	TRect R;//grid
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		if(!Columns[X].Visible || Columns[X].Width==0){Columns[X].Width=0;continue;}//skrytí sloupce, zatím ale nefunguje patřičně orámování
		R.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width);
		for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
		{
			////umístění
			if(!Rows[Y].Visible || Rows[Y].Height==0){Rows[Y].Height=0;continue;}//skrytí řádku, zatím ale nefunguje patřičně orámování
			R.Top			=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

			////orámování buňky
			DrawCellBorder(C,X,Y,R);
		}
	}
}
//---------------------------------------------------------------------------
//zajistí vykreslení orámování jen jedné buňky
void TmGrid::DrawCellBorder(TCanvas *C,unsigned long X,unsigned long Y,TRect R)
{
	//top
	if(Cells[X][Y].TopBorder->Color!=Cells[X][Y].Background->Color)//kvůli sloučeným buňkám
	{
		SetBorder(C,Cells[X][Y].TopBorder);
		C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
	}

	//bottom
	if(Y==RowCount-1 && Rows[Y].Visible==true)//akcelerátor, aby se zbytečně nevykreslovalo, vykreslí jenom poslední!!!, invertní filozofie než ukazování na stejné orámování, ale zde z důvodu možného překryvu s náplní předchozí buňky
	{
		SetBorder(C,Cells[X][Y].BottomBorder);
		C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
	}
	if(Y+1<=RowCount-1 && Rows[Y].Visible==true && Rows[Y+1].Visible==false)//pokud bude následující řádek skrytý a nejedná se o poslední řádek aktuální také nebude skrytý
	{
		SetBorder(C,Cells[X][Y+getCountNextVisibleRow(Y)].BottomBorder);
		C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
	}

	//left
	if(Cells[X][Y].LeftBorder->Color!=Cells[X][Y].Background->Color)//kvůli sloučeným buňkám
	{
		SetBorder(C,Cells[X][Y].LeftBorder);
		C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
	}

	//right
	if(X==ColCount-1)//akcelerátor, aby se zbytečně nevykreslovalo, vykreslí jenom poslední!!!, invertní filozofie než ukazování na stejné orámování, ale zde z důvodu možného překryvu s náplní předchozí buňky
	{
		SetBorder(C,Cells[X][Y].RightBorder);
		C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
	}
	if(Y==ColCount-2 && (!Columns[ColCount-1].Visible || Columns[ColCount-1].Width==0))//pokud je poslední sloupe skryt, převezme jeho pravé orámování předposlední, u jiné situace netřeba
	{ //podmínka chce dodělat jako pro skrývání řádků výše
		SetBorder(C,Cells[X+1][Y].RightBorder);
		C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
	}

}
//---------------------------------------------------------------------------
//vráti počet skrytých řádků po aktuálním řádku následujích až po první zobrazený
int TmGrid::getCountNextVisibleRow(int Row)
{
	int Count=0;
	while(Row+1<RowCount && !Rows[++Row].Visible)Count++;
	return Count;
}
//---------------------------------------------------------------------------
//zajistí vykreslení poznámky "pod čarou" resp. pod tabulkou
void TmGrid::DrawNote(TCanvas *C)
{
	////POZNÁMKA POD ČAROU -TmGrid - poznámka "pod čarou" resp. pod tabulkou, přístup mGrid->Note, možno nastavovat hodnotu textu, font textu, ukládá si citelnou oblast, zarovnává na šířku tabulky pokud se text nevejde zalomí na další řádek (dle poslední mezery na řádku), max zobrazí dva řádky, výchozí barva červená a 11pt velikost písma
	if(Note.Text!=""/* && ColCount>0 && RowCount>0*/)
	{
		//zarovnání dle exBUTTONu
		short leftOffset=0,rightOffset=0;
		switch(exBUTTONalign)
		{
			case LEFT:exBUTTON->Left=Left;leftOffset=exBUTTON->Width;break;
			case RIGHT:exBUTTON->Left=Left+Width-exBUTTON->Width+m.round(Border.Width/2.0);rightOffset=exBUTTON->Width;break;
		}
		Width=getWidth();
		Height=getHeight();
		short Zoom_b=1; if(AntiAliasing_text)Zoom_b=3;
		short margin_left=Note.margin_left*Zoom_b,margin_right=Note.margin_right*Zoom_b,margin_bootom=Note.margin_bootom*Zoom_b,margin_top=Note.margin_top*Zoom_b;
		C->Font=Note.Font;C->Font->Size*=Zoom_b;
		TFont *FontLink=new TFont();FontLink->Name=Note.Font->Name;FontLink->Size=Note.Font->Size;FontLink->Color=DefaultCell.isLink->Color;FontLink->Style = TFontStyles()<<fsUnderline;
		TFont *FontActiveLink=new TFont();FontActiveLink->Name=Note.Font->Name;FontActiveLink->Size=Note.Font->Size;FontActiveLink->Color=FontLink->Color;FontActiveLink->Style = TFontStyles()<<fsBold<<fsUnderline;
		int W=(Width-leftOffset-rightOffset-margin_left-margin_right)*Zoom_b;
		int Wt=C->TextWidth(Note.Text);
		if(W<Wt)//pokud je text poznámky delší, řeší ještě zalamování textu
		{
			int L=Note.Text.Length();                                   //zajistí odřádkování po poslední mezeře na daném řádku
			AnsiString T=Note.Text.SubString(1,floor(W/(Wt/(L*1.0)))-1);T=T.SubString(1,ms.lastPos(T," ")-1);
			//C->TextOutW(leftOffset+margin_left,(Height+Border.Width)*Zoom_b+margin_top,T);
			TRect R_temp=DrawTextLink(C,leftOffset+margin_left,(Height+Border.Width)*Zoom_b+margin_top,T,Note.Font,FontLink,FontActiveLink);
			if(R_temp!=TRect(-1,-1,-1,-1))Note.LinkArea=R_temp;
			AnsiString T1=Note.Text.SubString(T.Length()+1,L).TrimLeft();
			//C->TextOutW(leftOffset+margin_left,(Height+Border.Width)*Zoom_b+margin_top+C->TextHeight(T),T1);
			R_temp=DrawTextLink(C,leftOffset+margin_left,(Height+Border.Width)*Zoom_b+margin_top+C->TextHeight(T),T1,Note.Font,FontLink,FontActiveLink);
			if(R_temp!=TRect(-1,-1,-1,-1))Note.LinkArea=R_temp;
			Note.NoteArea=TRect(Left+leftOffset,Top+Height,Left+W+rightOffset+margin_left+margin_right,Top+Height+Border.Width+C->TextHeight(T)+C->TextHeight(T1)+margin_bootom);
		}                                                      //zpětná korekce, takže +
		else//jednořádkový text
		{
			//C->TextOutW(leftOffset+margin_left,(Height+Border.Width)*Zoom_b+margin_top,Note.Text);
			Note.LinkArea=DrawTextLink(C,leftOffset+margin_left,(Height+Border.Width)*Zoom_b+margin_top,Note.Text,Note.Font,FontLink,FontActiveLink);
			Note.NoteArea=TRect(Left+leftOffset,Top+Height,Left+W+rightOffset+margin_left+margin_right,Top+Height+Border.Width+C->TextHeight(Note.Text)+margin_bootom);
		}                                                      //zpětná korekce, takže +
		FontLink=NULL;delete FontLink;FontActiveLink=NULL;delete FontActiveLink;
	}
	else
	{
		if(Note.NoteArea!=TRect(-1,-1,-1,-1))
		{
			InvalidateRect(Form->Handle,&Note.NoteArea,true);
			Note.NoteArea=TRect(-1,-1,-1,-1);
		}
	}
}
//---------------------------------------------------------------------------
//zajistí přímé vykreslení poznámky bez refreshe popř. smázání doszením prázdných uvozovek, nově poznámka má také možnost nastavování margin pomocí Note.margin_left,margin_right,margin_bootom,margin_top;
void TmGrid::ShowNote(UnicodeString Text,TColor Color,short FontSize)
{
	if(Text!=Note.Text)//ošetření probliku
	{
		if(Text!="")
		{
			Note.Text=Text;
			if(FontSize!=0)Note.Font->Size=FontSize;
			Note.Font->Color=clRed;
			Cantialising a;
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=Note.NoteArea.Width();
			bmp_in->Height=Note.NoteArea.Height();
			//bmp_in->Canvas->Brush->Color=clRed;
			//bmp_in->Canvas->FillRect(TRect(0,0,bmp_in->Width,bmp_in->Height));
			DrawNote(bmp_in->Canvas);
			Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
			Form->Canvas->Draw(Note.NoteArea.Left,Note.NoteArea.Top,bmp_out);
			delete (bmp_out);delete (bmp_in);//velice nutné
			InvalidateRect(Form->Handle,&Note.NoteArea,true);
		}
		else
		{
			Note.Text="";
			InvalidateRect(Form->Handle,&Note.NoteArea,true);
			Note.NoteArea=TRect(-1,-1,-1,-1);
			Note.LinkArea=TRect(-1,-1,-1,-1);
		}
	}
}
//---------------------------------------------------------------------------
//vykreslí text s odkazem, odkaz aktivní modrou, vrací zpět oblast, kde se nachazí odkaz
TRect TmGrid::DrawTextLink(TCanvas *C,unsigned long left,unsigned long top,AnsiString Text,TFont *FontText,TFont *FontLink,TFont *FontActiveLink)
{
	TRect RET;
	short Zoom=1;if(AntiAliasing_text)Zoom=3;
	unsigned int Pos=Text.Pos("<a>");//pozice html tagu
	if(Pos>0)//parsování HTML
	{
		AnsiString T1=ms.TrimRightFrom(Text,"<a>");
		AnsiString Link=ms.EP(Text,"<a>","</a>");
		AnsiString T2=ms.TrimLeftFromText(Text,"</a>");
		//text před odkazem
		C->Font=FontText;
		C->Font->Size*=Zoom;
		C->TextOut(left,top,T1);
		//odkaz
		C->Font=FontLink;
		C->Font->Size*=Zoom;
		short w=C->TextWidth(T1);
		RET.left=m.round(Left+left/Zoom+w/Zoom);//kvůli citelné oblasti pro link dané buňky
		RET.top=m.round(Top+top/Zoom);//kvůli citelné oblasti pro link dané buňky
		RET.bottom=m.round(Top+top/Zoom+C->TextHeight(Link)/Zoom);//kvůli citelné oblasti pro link dané buňky
		RET.right=m.round(Left+left/Zoom+(w+C->TextWidth(Link))/Zoom);//kvůli citelné oblasti pro link dané buňky
		if(RET.PtInRect(TPoint(X,Y))){C->Font=FontActiveLink;C->Font->Size*=Zoom;}//aktivní odkaz (je přes něj myš)
//Form2->Memo1->Lines->Add(AnsiString(X)+";"+AnsiString(Y));
		C->TextOut(left+w,top,Link);
		//text za odkazem
		w+=C->TextWidth(Link);
		C->Font=FontText;
		C->Font->Size*=Zoom;
		C->TextOut(left+w,top,T2);
	}
	else //bez odkazu
	{
		C->TextOut(left,top,Text);
		RET=TRect(-1,-1,-1,-1);
	}
	return RET;
}
//---------------------------------------------------------------------------
//nastaví velikost sloupců a řádků dle aktuálního nastavení a potřeby
void TmGrid::SetColRow()
{
	executeColumnsAutoFit(Form->Canvas);

	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		if(X!=0 && (!Columns[X-1].Visible || Columns[X-1].Width==0))Columns[X].Left=Columns[X-1].Left;//skrytí sloupce, pokud je přechozí skrytý, převezme aktuální zpracovávaný (cyklem) pozici předchozího
		else if(X>0)Columns[X].Left=Columns[X-1].Left+Columns[X-1].Width;else Columns[0].Left=0;//výpočet levého okraje buňky dle buňky předchozí
	}
	for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
	{ //není doděláno ve vztahu s níže uvedeným
		if(Y!=0 && (!Rows[Y-1].Visible || Rows[Y-1].Height==0)){Rows[Y].Top=Rows[Y-1].Top;}else//skrytí řádku, zatím ale nefunguje patřičně orámování
		if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//výpočet horního okraje buňky dle buňky předchozí
	}

	////výpočet celkové šířky a výšky tabulky
	Width=getWidth();//zároveň nastavuje celkovou hodnotu Width, musí být až tady!!!, protože výše se to teprvé nastavuje
	Height=getHeight();//zároveň nastavuje celkovou hodnotu Height, musí být až tady!!!, protože výše se to teprvé nastavuje
}
//---------------------------------------------------------------------------
//nastaví grafické pero na požadované parametry
void TmGrid::SetBorder(TCanvas *C,TBorder *Border)
{
	DeleteObject(C->Pen->Handle);//zruší původní pero       //PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE
	DWORD pStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE /* PS_ENDCAP_ROUND PS_ENDCAP_SQUARE PS_ENDCAP_FLAT | PS_JOIN_BEVEL | PS_INSIDEFRAME*/;
	DWORD pWidth = Border->Width;
	if(AntiAliasing_grid)pWidth *=3;

	LOGBRUSH lBrush;
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbColor = Border->Color;
	lBrush.lbHatch = 0;

	C->Pen->Handle = ExtCreatePen(pStyle, pWidth, &lBrush, NULL, NULL);
}
//---------------------------------------------------------------------------
//nastaví danou buňku dle typu
void TmGrid::SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell)
{
	switch(Cell.Type)
	{
		case DRAW:
		{
			SetDraw(Canv,Rt,X,Y,Cell);
		}break;
		case readEDIT:
		{
			SetEdit(R,X,Y,Cell);
		}break;
		case EDIT:
		{
			SetEdit(R,X,Y,Cell);
		}break;
		case readNUMERIC:
		{
			SetNumeric(R,X,Y,Cell);
		}break;
		case NUMERIC:
		{
			SetNumeric(R,X,Y,Cell);
		}break;
		case LABEL:
		{
			SetLabel(R,X,Y,Cell);
		}break;
		case BUTTON:
		{
			SetButton(R,X,Y,Cell);
		}break;
		case glyphBUTTON:
		{
			SetGlyphButton(R,X,Y,Cell);
		}break;
		case COMBO:
		{
			SetCombo(R,X,Y,Cell);
		}break;
    case COMBOEDIT:
		{
			SetComboEdit(R,X,Y,Cell);
		}break;
		case CHECK:
		{
			SetCheck(R,X,Y,Cell);
		}break;
		case RADIO:
		{
			SetRadio(R,X,Y,Cell);
		}break;
	}
}
//---------------------------------------------------------------------------
//nastaví danou buňku na draw, pomocná metoda výše uvedené
void TmGrid::SetDraw(TCanvas *Canv,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell)
{
	//Cell.Text=getTag(X,Y);//provizorní výpis pokud chci očíslovat jednotlivé buňky
	short Zoom=1;if(AntiAliasing_text)Zoom=3;
	//nastavení fontu
	Canv->Font=Cell.Font;
	int Orientation=Cell.Font->Orientation;
	if(F->m.null(F->ms.MyToDouble(Cell.Text))<0)Canv->Font=Cell.isNegativeNumber;//podmíněné formátování záporné hodnoty
	if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))Canv->Font=Cell.isZero;//podmíněné formátování nulové hodnoty
  if(Cell.Highlight)Canv->Font->Color=clHighlight;//highlignutí formou změny barvy textu
	Canv->Font->Orientation=Orientation;//musí ještě vrátit orientaci pokud byla podmíněným formátováním přepsána
	Canv->Font->Size*=Zoom;
	Canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké - TEST
	Canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;//asi nepřináší zcela přínos - TEST
	//SetBkMode(canv->Handle,OPAQUE);//nastavení netransparentního pozadí
	if(Cell.Text=="")Canv->Brush->Color=Cell.isEmpty->Color;//podmíněné formátování//zde se asi nezohledňuje, spíše v drawgrid, ale otázka je jak bez AA
	else Canv->Brush->Color=Cell.Background->Color;//vyplněná buňka
	Canv->Brush->Style=bsClear;//nastvení netransparentního pozadí
	//zarovnání
	//samotný výpis
	long L=Rt.Left,T=Rt.Top;
	int W=getWidthHeightText(Cell).X*Zoom;
	int H=getWidthHeightText(Cell).Y*Zoom;
	//short Rot=1;//slouží jako pomůcka rotace
	if(Cell.Font->Orientation==900){/*Rot=-1;*/H=0;if(Cell.Valign==MIDDLE)H=-getWidthHeightText(Cell).Y;}
	if(Cell.Font->Orientation==2700){/*Rot=-1;*/W=0;if(Cell.Align==LEFT || Cell.Align==CENTER)W=-W;H=0;if(Cell.Valign==MIDDLE)H=getWidthHeightText(Cell).Y;}
	switch(Cell.Align)
	{
		case aNO:   L=m.round(Rt.Left+Cell.TextPositon.X*Zoom+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2.0*Zoom);break;
		case LEFT:	L=m.round(Rt.Left+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2.0*Zoom);break;
		case CENTER:L=m.round((Rt.Left+Rt.Right)/2.0-W/2.0);break;
		case RIGHT:	L=m.round(Rt.Right-W-Cell.RightMargin*Zoom-Cells[X][Y].RightBorder->Width/2.0*Zoom);if(Cell.Font->Orientation==2700)L-=H;break;
	}
	switch(Cell.Valign)
	{
		case vNO:
		{
			T=Rt.Top+Cell.TextPositon.Y*Zoom;
			if(Cell.Font->Orientation==0)T+=m.round(Cell.TopMargin*Zoom+Cells[X][Y].TopBorder->Width/2.0*Zoom);
			else T-=m.round(Cell.BottomMargin*Zoom+Cells[X][Y].BottomBorder->Width/2.0*Zoom);
		}break;
		case TOP:		T=m.round(Rt.Top+Cell.TopMargin*Zoom+Cells[X][Y].TopBorder->Width/2.0*Zoom);break;
		case MIDDLE:T=m.round((Rt.Top+Rt.Bottom)/2.0-H/2.0);break;
		case BOTTOM:T=m.round(Rt.Bottom-H-Cell.BottomMargin*Zoom-Cells[X][Y].BottomBorder->Width/2.0*Zoom);break;
	}
	TRect Rect=DrawTextLink(Canv,L,T,Cell.Text,Cell.Font,Cell.isLink,Cell.isActiveLink);//vykreslí text včetně odkazu a vrátí citelnou oblast odkazu
	Cell.LinkCoordinateStart.x=Rect.left;Cell.LinkCoordinateStart.y=Rect.top;Cell.LinkCoordinateEnd.x=Rect.right;Cell.LinkCoordinateEnd.y=Rect.bottom;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na edit, pomocná metoda výše uvedené
void TmGrid::SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPEdit *E=createEdit(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//pozice
	E->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);//ubere velikost komponenty podle šířky orámování
	E->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);//ubere velikost komponenty podle šířky orámování

	//velikost
	int o=0;
	//šířka
	if(Y==ColCount-2 && (!Columns[ColCount-1].Visible || Columns[ColCount-1].Width==0))o=1;else o=0;//dodělat podle výšky!!! pokud je poslední sloupe skryt, převezme jeho pravé orámování předposlední, u jiné situace netřeba
	if(Cell.MergeState==false)E->Width=Columns[X].Width-floor(Cells[X+o][Y].RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);//pokud neplatí nastavuje se přímo v mergovaní, ubere pouze velikost komponenty podle šířky orámování
	//výška
	if(Y+1<=RowCount-1 && Rows[Y].Visible==true && Rows[Y+1].Visible==false)o=getCountNextVisibleRow(Y);else o=0;//pokud bude následující řádek skrytý a nejedná se o poslední řádek aktuální také nebude skrytý, převezme jeho spodní orámování předposlední, u jiné situace netřeba
	/*if(Cell.MergeState==false)*/E->Height=Rows[Y].Height-floor(Cells[X][Y+o].BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dodělat ubere velikost komponenty podle šířky orámování//ubere velikost komponenty podle šířky orámování

	//atributy
	if(Cell.Type==readEDIT)E->Enabled=false;
	E->AutoSize=false;//nutnost
	E->Transparent=false;//nutnost
	if(!VisibleComponents)E->Visible=false;else E->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!
	//E->ShowHint=false;//toto by bylo vždy u editu na false, pokus automatizace pro dlouhý textif(Cell.Text.Length()>E->Width/(Cell.Font->Size-2))E->ShowHint=true;else //asi nepřesné
	if(Cell.ShowHint){E->ShowHint=true;E->Hint=Cell.Hint;}
	if(Cell.Text=="")E->Options->NormalColor=Cell.isEmpty->Color;else E->Options->NormalColor=Cell.Background->Color;
	E->Options->NormalColorAlpha=255;
	//zrušeno if(Cell.Highlight)E->Options->FrameNormalColor=clHighlight;else//rámeček musí být stejnou barvou jakou buňka, protože mřížka je o 1px na všechny strany roztažená
	E->Options->FrameNormalColor=Cell.Background->Color;
	E->Options->FrameNormalColorAlpha=255;
	E->Options->FrameFocusedColor=clHighlight;
	E->Options->FrameFocusedColorAlpha=255;
	E->Options->FrameDisabledColor=E->Options->DisabledColor;
	E->Margins->Left=0;E->Margins->Right=0;E->Margins->Top=0;E->Margins->Bottom=0;
	switch(Cell.Align)
	{
		case LEFT:	E->Alignment=taLeftJustify;break;
		case CENTER:E->Alignment=taCenter;break;
		case RIGHT:	E->Alignment=taRightJustify;break;
	}
	E->ContentMarginBottom=0;
	if(Cell.Text!="")
	{
		switch(Cell.Valign)
		{
			case TOP:		E->ContentMarginTop=0;break;
			case MIDDLE:E->ContentMarginTop=E->Height/2-getWidthHeightText(Cell).Y/2;break;
			case BOTTOM:E->ContentMarginTop=E->Height-getWidthHeightText(Cell).Y;break;
		}
	}

	//text
	E->Font=Cell.Font;
	//if(F->ms.IsNumber(Cell.Text) && E->Font->Name=="Roboto Cn"){E->Font->Size--; E->Font->Name="Arial";E->Font->Quality=System::Uitypes::TFontQuality::fqDefault;}else //pro čísla v případě robota cn nastavuje jen Arial - nevyužito
	if(E->Font->Name=="Roboto Cn")E->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else E->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))E->Font=Cell.isNegativeNumber;//podmíněné formátování
	if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))E->Font=Cell.isZero;//podmíněné formátování
	if(Cell.Highlight || E->Focused()){E->Font->Color=clHighlight;E->Font->Style = TFontStyles()<< fsBold;}//highlignutí formou změny barvy textu
	//if(!E->Focused())//pokud není na buňce focus resp. není aktivní - provizorně odstaveno, zdá se, že nemá na nic vliv
	E->Text=Cell.Text;

	//vlastník
	E->Parent=Form;//musí být až na konci
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na numericedit, pomocná metoda objednu výše uvedené
void TmGrid::SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPNumericEdit *N=createNumeric(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//pozice
	N->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);//ubere velikost komponenty podle šířky orámování
	N->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);//ubere velikost komponenty podle šířky orámování

	//velikost
	int o=0;
	//šířka
	if(Y==ColCount-2 && (!Columns[ColCount-1].Visible || Columns[ColCount-1].Width==0))o=1;else o=0;//dodělat podle výšky!!! pokud je poslední sloupe skryt, převezme jeho pravé orámování předposlední, u jiné situace netřeba
	if(Cell.MergeState==false)N->Width=Columns[X].Width-floor(Cells[X+o][Y].RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);//pokud neplatí nastavuje se přímo v mergovaní, ubere pouze velikost komponenty podle šířky orámování
	//výška
	if(Y+1<=RowCount-1 && Rows[Y].Visible==true && Rows[Y+1].Visible==false)o=getCountNextVisibleRow(Y);else o=0;//pokud bude následující řádek skrytý a nejedná se o poslední řádek aktuální také nebude skrytý, převezme jeho spodní orámování předposlední, u jiné situace netřeba
	/*if(Cell.MergeState==false)*/N->Height=Rows[Y].Height-floor(Cells[X][Y+o].BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dodělat ubere velikost komponenty podle šířky orámování//ubere velikost komponenty podle šířky orámování

	//atributy
	if(Cell.Type==NUMERIC)N->Enabled=true;else N->Enabled=false;
	N->AutoSize=false;
	N->Transparent=false;
	if(!VisibleComponents)N->Visible=false;else N->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!
	N->Decimal=Decimal;
	//if(!N->Focused() && !IntegerDecimalNull && m.cele_cislo(ms.MyToDouble(Cell.Text)))N->Decimal=0;//pokud se jedná o celé číslo, nezobrazuje "reálnou část" celého čísla tj. počet nul do počtu decimal, nastavuje se pouze pokud není daný NUMERIC editovaný
	N->DisplayType=scedtNumeric;
	N->ValueType=scvtFloat;
	if(Cell.ShowHint){N->ShowHint=true;N->Hint=Cell.Hint;}
	//28.2.provizorní fix if(Cell.Text=="")N->Options->NormalColor=Cell.isEmpty->Color;else //podmíněné formátování
	N->Options->NormalColor=Cell.Background->Color;
	N->Options->NormalColorAlpha=255;
	if(!Cell.Highlight)N->Options->FrameNormalColor=Cell.Background->Color;//rámeček musí být stejnou barvou jakou buňka, protože mřížka je o 1px na všechny strany roztažená
	else N->Options->FrameNormalColor=clHighlight;
	N->Options->FrameNormalColorAlpha=255;
	N->Options->FrameDisabledColor=N->Options->DisabledColor;
	N->Options->FrameFocusedColor=clHighlight;
	N->Options->FrameFocusedColorAlpha=255;
	N->Margins->Left=0;N->Margins->Right=0;N->Margins->Top=0;N->Margins->Bottom=0;
	switch(Cell.Align)
	{
		case LEFT:	N->Alignment=taLeftJustify;break;
		case CENTER:N->Alignment=taCenter;break;
		case RIGHT:	N->Alignment=taRightJustify;break;
	}
	N->ContentMarginBottom=0;
	if(Cell.Text!="")
	{
		switch(Cell.Valign)
		{
			case TOP:		N->ContentMarginTop=0;break;
			case MIDDLE:N->ContentMarginTop=N->Height/2-getWidthHeightText(Cell).Y/2;break;
			case BOTTOM:N->ContentMarginTop=N->Height-getWidthHeightText(Cell).Y;break;
		}
	}

	//text
	N->Font=Cell.Font;
	if(N->Font->Name=="Roboto Cn")N->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else N->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))N->Font=Cell.isNegativeNumber;//podmíněné formátování
	if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))N->Font=Cell.isZero;//podmíněné formátování
	if(Cell.Highlight || N->Focused()){N->Font->Color=clHighlight;N->Font->Style = TFontStyles()<< fsBold;}//highlignutí formou změny barvy textu
	//if(!N->Focused())//pokud je na buňce focus resp. je aktivní - provizorně odstaveno, zdá se, že nemá na nic vliv
	N->Value=ms.MyToDouble(Cell.Text);

	//vlastník
	N->Parent=Form;//musí být až na konci
}
//---------------------------------------------------------------------------
//nastaví danou buňku na numericedit, pomocná metoda objednu výše uvedené
void TmGrid::SetLabel(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscHTMLLabel *L=createLabel(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//atributy
	L->AutoSizeHeight=false;L->AutoSizeWidth=false;
	L->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);//ubere velikost komponenty podle šířky orámování
	L->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);//ubere velikost komponenty podle šířky orámování
	if(Cell.MergeState==false)L->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);	 //pokud neplatí nastavuje se přímo v mergovaní, ubere pouze velikost komponenty podle šířky orámování//pokud neplatí nastavuje se přímo v mergovaní
	/*if(Cell.MergeState==false)*/L->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dodělat ubere velikost komponenty podle šířky orámování//dodělat//ubere velikost komponenty podle šířky orámování
	if(Cell.ShowHint){L->ShowHint=true;L->Hint=Cell.Hint;}
	if(Cell.Text=="")L->Color=Cell.isEmpty->Color;else L->Color=Cell.Background->Color;
	L->Margins->Left=0;L->Margins->Right=0;L->Margins->Top=0;L->Margins->Bottom=0;
	switch(Cell.Align)
	{
		case LEFT:	L->TextAlignment=taLeftJustify;break;
		case CENTER:L->TextAlignment=taCenter;break;
		case RIGHT:	L->TextAlignment=taRightJustify;break;
	}
//	L->ContentMarginBottom=0;
//	switch(Cell.Valign)
//	{
//		case TOP:		L->ContentMarginTop=0;break;
//		case MIDDLE:L->ContentMarginTop=E->Height/2-getWidthHeightText(Cell).Y/2;break;
//		case BOTTOM:L->ContentMarginTop=E->Height-getWidthHeightText(Cell).Y;break;
//	}
	L->Visible=VisibleComponents;//musí být až za nastavováním pozice kvůli posunu obrazu!!!

	//text
	L->Font=Cell.Font;
	if(L->Font->Name=="Roboto Cn")L->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else L->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))L->Font=Cell.isNegativeNumber;//podmíněné formátování
	if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))L->Font=Cell.isZero;//podmíněné formátování
	L->Caption=Cell.Text;

	//vlastník
	L->Parent=Form;//musí být až na konci
	L=NULL;delete L;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na Button, pomocná metoda objednu výše uvedené
void TmGrid::SetButton(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPButton *B=createButton(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//atributy
	B->Top=R.Top+ceil(Cell.TopBorder->Width/2.0)+1;
	B->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0)+1;
	B->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0)-1;
	B->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0)-1;
	if(!VisibleComponents)B->Visible=false;else B->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!
	if(Cell.ShowHint){B->ShowHint=true;B->Hint=Cell.Hint;}
	//B->Options->NormalColor=Cell.Background->Color; zde nenastavovat!
	B->Options->FrameNormalColor=B->Options->NormalColor;
	if(Cell.ShowHint){B->ShowHint=true;B->Hint=Cell.Hint;}

	//text
	B->Font=Cell.Font;
	if(B->Font->Name=="Roboto Cn")B->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else B->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	B->Caption=Cell.Text;

	//vlastník
	B->Parent=Form;//musí být až na konci
	B=NULL;delete B;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na GlyphButton, pomocná metoda objednu výše uvedené
void TmGrid::SetGlyphButton(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPGlyphButton *gB=createGlyphButton(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//atributy
	gB->Top=R.Top+floor(Cell.TopBorder->Width/2.0)+1;
	gB->Left=R.Left+floor(Cell.LeftBorder->Width/2.0)+1;
	gB->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-floor(Cell.LeftBorder->Width/2.0)-1;
	gB->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-floor(Cell.TopBorder->Width/2.0)-1;
	if(!VisibleComponents)gB->Visible=false;else gB->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!
	//gB->Options->NormalColor=Cell.Background->Color; zde nenastavovat!
	if(Cell.ShowHint){gB->ShowHint=true;gB->Hint=Cell.Hint;}

	//text
	gB->Font=Cell.Font;
	gB->Caption=Cell.Text;

	//vlastník
	gB->Parent=Form;//musí být až na konci
	gB=NULL;delete gB;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na ComboBox, pomocná metoda objednu výše uvedené
void TmGrid::SetCombo(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPComboBox *C=createCombo(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//pozice
	C->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);
	C->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);

	//velikost
	short o=0;
	if(Y==ColCount-2 && (!Columns[ColCount-1].Visible || Columns[ColCount-1].Width==0))o=1;else o=0;//pokud je poslední sloupe skryt, převezme jeho pravé orámování předposlední, u jiné situace netřeba
	if(Cell.MergeState==false)C->Width=Columns[X].Width-floor(Cells[X+o][Y].RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);   //pokud neplatí nastavuje se přímo v mergovaní, ubere pouze velikost komponenty podle šířky orámování
	if(Y==RowCount-2 && (!Rows[RowCount-1].Visible || Rows[RowCount-1].Height==0))o=1;else o=0;//pokud je poslední řádek skryt, převezme jeho spodní orámování předposlední, u jiné situace netřeba
	/*if(Cell.MergeState==false)*/C->Height=Rows[Y].Height-floor(Cells[X+o][Y].BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dodělat ubere velikost komponenty podle šířky orámování

	//zobrazení komponenty
	if(!VisibleComponents)C->Visible=false;else C->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!

	//volitelné atributy
	if(!Cell.Highlight)C->Options->FrameNormalColor=Cell.Background->Color;//rámeček musí být stejnou barvou jakou buňka, protože mřížka je o 1px na všechny strany roztažená
	else C->Options->FrameNormalColor=clHighlight;

	C->Options->NormalColor=Cell.Background->Color;
	C->Options->FrameFocusedColor=clHighlight;
	C->Options->FocusedColor=Cell.Background->Color;
	C->Options->HotColor=Cell.Background->Color;
	C->Options->PressedColor=Cell.Background->Color;

	//font
	C->Font=Cell.Font;
	if(C->Font->Name=="Roboto Cn")C->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else C->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	C->Options->FontNormalColor=Cell.Font->Color;
	//C->ItemIndex=1;//nelze předoznační první položku

	if(Cell.ShowHint){C->ShowHint=true;C->Hint=Cell.Hint;}

	//vlastník
	C->Parent=Form;//musí být až na konci
	C=NULL;delete C;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na ComboBox, pomocná metoda objednu výše uvedené
void TmGrid::SetComboEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPComboEdit *C=createComboEdit(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//pozice
	C->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);
	C->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);

	//velikost
	short o=0;
	if(Y==ColCount-2 && (!Columns[ColCount-1].Visible || Columns[ColCount-1].Width==0))o=1;else o=0;//pokud je poslední sloupe skryt, převezme jeho pravé orámování předposlední, u jiné situace netřeba
	if(Cell.MergeState==false)C->Width=Columns[X].Width-floor(Cells[X+o][Y].RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);   //pokud neplatí nastavuje se přímo v mergovaní, ubere pouze velikost komponenty podle šířky orámování
	if(Y==RowCount-2 && (!Rows[RowCount-1].Visible || Rows[RowCount-1].Height==0))o=1;else o=0;//pokud je poslední řádek skryt, převezme jeho spodní orámování předposlední, u jiné situace netřeba
	/*if(Cell.MergeState==false)*/C->Height=Rows[Y].Height-floor(Cells[X+o][Y].BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dodělat ubere velikost komponenty podle šířky orámování

	//zobrazení komponenty
	if(!VisibleComponents)C->Visible=false;else C->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!

	//volitelné atributy
	if(!Cell.Highlight)C->Options->FrameNormalColor=Cell.Background->Color;//rámeček musí být stejnou barvou jakou buňka, protože mřížka je o 1px na všechny strany roztažená
	else C->Options->FrameNormalColor=clHighlight;

	C->Options->NormalColor=Cell.Background->Color;
	C->Options->FrameFocusedColor=clHighlight;
	C->Options->FocusedColor=Cell.Background->Color;
	C->Options->HotColor=Cell.Background->Color;

	//font
	C->Font=Cell.Font;
	if(C->Font->Name=="Roboto Cn")C->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else C->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	C->Options->FontNormalColor=Cell.Font->Color;
	//C->ItemIndex=1;//nelze předoznační první položku

	if(Cell.ShowHint){C->ShowHint=true;C->Hint=Cell.Hint;}

	//vlastník
	C->Parent=Form;//musí být až na konci
	C=NULL;delete C;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na Checkbox, pomocná metoda objednu výše uvedené
void TmGrid::SetCheck(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPCheckBox *Ch = createCheck(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří

	//atributy
	//nastavení velikosti checkboxu - přesunuto do create
	//pozice checkboxu v buňce //upraveno - odstaveno nastavování velikosti Checkboxu v CENTER+MIDDLE, ostatní ponechány původní
	switch(Cell.Align)
	{
		case aNO:		Ch->Left+=Left-preLeft;break;
		case LEFT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;break;
		case CENTER:Ch->Left=R.Left+Columns[X].Width/2-Ch->Width/2;break;
		case RIGHT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;Ch->BiDiMode=bdRightToLeft;break;
	}
	switch(Cell.Valign)
	{
		case aNO:		Ch->Top+=Top-preTop;break;
		case TOP:		Ch->Top=R.Top+1;break;
	 	case MIDDLE:Ch->Top=R.Top+m.round(Rows[Y].Height/2.0-Ch->Height/2.0);break;
		case BOTTOM:Ch->Height=Ch->OptionsChecked->ShapeSize;Ch->Top=R.Top+Rows[Y].Height-Ch->Height;break;
	}
	if(!VisibleComponents)Ch->Visible=false;else Ch->Visible=true;//musí být až za nastavováním pozice kvůli posunu obrazu!!!
	Ch->Options->NormalColor=Cell.Background->Color;
	Ch->Options->NormalColorAlpha=255;
	if(Cell.ShowHint){Ch->ShowHint=true;Ch->Hint=Cell.Hint;}

	//text
	Ch->Font=Cell.Font;
	if(Ch->Font->Name=="Roboto Cn")Ch->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else Ch->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	Ch->Caption=Cell.Text;

	//vlastník
	Ch->Parent=Form;//musí být až na konci
	Ch=NULL;delete Ch;
}
//---------------------------------------------------------------------------
//nastaví danou buňku na radio, pomocná metoda objednu výše uvedené
void TmGrid::SetRadio(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//založení + tag + název
	TscGPRadioButton *Ra = createRadio(X,Y);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	//atributy
	Ra->Visible=VisibleComponents;
	switch(Cell.Align)
	{
		case aNO:break;
		case LEFT:	Ra->Width=Columns[X].Width-2;Ra->Left=R.Left+1;break;
		case CENTER:Ra->Width=Ra->OptionsChecked->ShapeSize;Ra->Left=R.Left+Columns[X].Width/2-Ra->Width/2;break;
		case RIGHT:	Ra->Width=Columns[X].Width-2;Ra->Left=R.Left+1;Ra->BiDiMode=bdRightToLeft;break;
	}
	switch(Cell.Valign)
	{
		case aNO:break;
		case TOP:		Ra->Top=R.Top+1;Ra->Height=Ra->OptionsChecked->ShapeSize;break;
		case MIDDLE:Ra->Top=R.Top+1;Ra->Height=Rows[Y].Height-2;break;
		case BOTTOM:Ra->Height=Ra->OptionsChecked->ShapeSize;Ra->Top=R.Top+Rows[Y].Height-Ra->Height;break;
	}
	//Ra->Options->NormalColor=clWhite;Cell.Background->Color;
	//Ra->Options->NormalColorAlpha=255;
	//Ra->Options->FrameNormalColor=clWhite;
	//Ra->Options->FrameNormalColorAlpha=255;
	if(Cell.ShowHint){Ra->ShowHint=true;Ra->Hint=Cell.Hint;}
	Ra->Font=Cell.Font;
	if(Ra->Font->Name=="Roboto Cn")Ra->Font->Quality=System::Uitypes::TFontQuality::fqAntialiased;else Ra->Font->Quality=System::Uitypes::TFontQuality::fqDefault;//zapíná AA, pozor může dělat problémy při zvětšování písma, alternativa fqProof či fqClearType
	Ra->Caption=Cell.Text;
	//vlastník
	Ra->Parent=Form;//musí být až na konci
	Ra=NULL;delete Ra;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPEdit *TmGrid::createEdit(unsigned long Col,unsigned long Row)
{
	TscGPEdit *E=getEdit(Col,Row);//pokud již existuje
	if(E==NULL)//pokud ne, tak založí
	{
		E = new TscGPEdit(Form);
		E->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		E->Name="mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(E->Tag);

		//výchozí atributy
		E->Options->FrameNormalColor=clHighlight;
		E->Options->FrameNormalColorAlpha=255;
		E->Options->FrameWidth=2;

		//události
		E->OnClick=&getTagOnClick;
		E->OnEnter=&getTagOnEnter;
		E->OnChange=&getTagOnChange;
		E->OnKeyDown=&getTagOnKeyDown;
		E->OnKeyPress=&getTagOnKeyPress;
		E->OnMouseEnter=&getTagOnMouseEnter;
		E->OnMouseMove=&getTagMouseMove;
	}
	return E;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPNumericEdit *TmGrid::createNumeric(unsigned long Col,unsigned long Row)
{
	TscGPNumericEdit *N=getNumeric(Col,Row);//pokud již existuje
	if(N==NULL)//pokud ne
	{
		N=new TscGPNumericEdit(Form);
		N->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		N->Name="mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(N->Tag);
		//události
		N->OnClick=&getTagOnClick;
		N->OnEnter=&getTagOnEnter;
		N->OnChange=&getTagOnChange;
		N->OnKeyDown=&getTagOnKeyDown;
		N->OnKeyPress=&getTagOnKeyPress;
		N->OnMouseEnter=&getTagOnMouseEnter;
		N->OnMouseMove=&getTagMouseMove;
	}
	return N;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscHTMLLabel *TmGrid::createLabel(unsigned long Col,unsigned long Row)
{
	TscHTMLLabel *L=getLabel(Col,Row);//pokud již existuje
	if(L==NULL)//pokud ne, tak založí
	{
		L = new TscHTMLLabel(Form);
		L->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		L->Name="mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(L->Tag);

		//události
		L->OnClick=&getTagOnClick;
		L->OnEnter=&getTagOnEnter;
		L->OnMouseEnter=&getTagOnMouseEnter;
		L->OnMouseMove=&getTagMouseMove;
	}
	return L;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPButton *TmGrid::createButton(unsigned long Col,unsigned long Row)
{
	TscGPButton *B=getButton(Col,Row);//pokud již existuje
	if(B==NULL)//pokud ne, tak založí
	{
		B = new TscGPButton(Form);
		B->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		B->Name="mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(B->Tag);

		//události
		B->OnClick=&getTagOnClick;
		B->OnEnter=&getTagOnEnter;
		B->OnMouseEnter=&getTagOnMouseEnter;
		B->OnMouseMove=&getTagMouseMove;
	}
	return B;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPGlyphButton *TmGrid::createGlyphButton(unsigned long Col,unsigned long Row)
{
	TscGPGlyphButton *gB=getGlyphButton(Col,Row);//pokud již existuje
	if(gB==NULL)//pokud ne, tak založí
	{
		gB = new TscGPGlyphButton(Form);
		gB->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		gB->Name="mGrid_GlyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(gB->Tag);

    gB->Options->FrameNormalColorAlpha=255;
		gB->Options->FrameFocusedColorAlpha=255;
		gB->Options->FramePressedColorAlpha=255;
	  gB->Options->FrameNormalColor=gB->Options->NormalColor;
    gB->Options->FrameFocusedColor=gB->Options->NormalColor;
    gB->Options->FramePressedColor=gB->Options->NormalColor;

		//události
		gB->OnClick=&getTagOnClick;
		gB->OnEnter=&getTagOnEnter;
		gB->OnMouseEnter=&getTagOnMouseEnter;
		gB->OnMouseMove=&getTagMouseMove;
	}
	return gB;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPComboBox *TmGrid::createCombo(unsigned long Col,unsigned long Row)
{
	TscGPComboBox *C=getCombo(Col,Row);//pokud již existuje
	if(C==NULL)//pokud ne, tak založí, pouze poprvé, když neexistuje
	{
		C=new TscGPComboBox(Form);//založí
		C->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		C->Name="mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(C->Tag);

		//události
		C->OnClick=&getTagOnClick;
		C->OnEnter=&getTagOnEnter;
		C->OnChange=&getTagOnChange;
		C->OnMouseEnter=&getTagOnMouseEnter;
		C->OnMouseMove=&getTagMouseMove;

		//výchozí nastavení
		C->Options->NormalColorAlpha=255;
		C->Options->FocusedColorAlpha=255;
		C->Options->FrameFocusedColorAlpha=255;
		C->Options->HotColorAlpha=255;
	}
	return C;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPComboEdit *TmGrid::createComboEdit(unsigned long Col,unsigned long Row)
{
	TscGPComboEdit *C=getComboEdit(Col,Row);//pokud již existuje
	if(C==NULL)//pokud ne, tak založí, pouze poprvé, když neexistuje
	{
		C=new TscGPComboEdit(Form);//založí
		C->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		C->Name="mGrid_COMBOEDIT_"+AnsiString(ID)+"_"+AnsiString(C->Tag);

		//události
		C->OnClick=&getTagOnClick;
		C->OnEnter=&getTagOnEnter;
		C->OnChange=&getTagOnChange;
		C->OnMouseEnter=&getTagOnMouseEnter;
		C->OnMouseMove=&getTagMouseMove;

		//výchozí nastavení
		C->Options->NormalColorAlpha=255;
		C->Options->FocusedColorAlpha=255;
		C->Options->FrameFocusedColorAlpha=255;
		C->Options->HotColorAlpha=255;
	}
	return C;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPCheckBox *TmGrid::createCheck(unsigned long Col,unsigned long Row)
{
	TscGPCheckBox *Ch=getCheck(Col,Row);//pokud již existuje
	if(Ch==NULL)//pokud ne, tak založí
	{
		Ch = new TscGPCheckBox(Form);
		Ch->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		//Cell.Text=Ch->Tag; ShowMessage(Ch->Tag);
		Ch->Name="mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(Ch->Tag);
    Ch->Options->ShapeSize=20;
    Ch->OptionsChecked->ShapeSize=Ch->Options->ShapeSize;
    Ch->Height=Ch->Options->ShapeSize;
    Ch->Width=Ch->Options->ShapeSize;

		//události
		Ch->OnClick=&getTagOnClick;
		Ch->OnEnter=&getTagOnEnter;
		Ch->OnMouseEnter=&getTagOnMouseEnter;
		Ch->OnMouseMove=&getTagMouseMove;
	}
	return Ch;
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
TscGPRadioButton *TmGrid::createRadio(unsigned long Col,unsigned long Row)
{
	TscGPRadioButton *Ra=getRadio(Col,Row);//pokud již existuje
	if(Ra==NULL)//pokud ne, tak založí
	{
		Ra = new TscGPRadioButton(Form);
		Ra->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní pořadí v paměti
		//Cell.Text=Ra->Tag; ShowMessage(Ra->Tag);
		Ra->Name="mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(Ra->Tag);

		//události
		Ra->OnClick=&getTagOnClick;
		//Ra->OnEnter=&getTagOnEnter;//asi zbytečná událost
		Ra->OnMouseEnter=&getTagOnMouseEnter;
		Ra->OnMouseMove=&getTagMouseMove;
	}
	return Ra;
}
//---------------------------------------------------------------------------
//vratí ID tag komponenty,absolutní pořadí v paměti
unsigned long TmGrid::getTag(unsigned long Col,unsigned long Row)
{
	return (Col+1)+Row*ColCount;
}
//---------------------------------------------------------------------------
//z tagu vratí číslo sloupce
unsigned long TmGrid::getColFromTag(unsigned long Tag)
{
	long RET=Tag%ColCount-1;
	if(RET>=0)return RET;
	else return RET=ColCount-1;
}
//---------------------------------------------------------------------------
//z tagu vratí číslo řádku
unsigned long TmGrid::getRowFromTag(unsigned long Tag)
{
	return ceil(Tag/(double)ColCount)-1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//PUBLIC: nastaví šířku buněk daného sloupce dle nejdelšího šířky textu buňky ze sloupce uvedeného parametrem ColIdx sloupce
void TmGrid::SetColumnAutoFit(long ColIdx)
{
	SetColumnAutoFitColIdx=ColIdx;
	SetColRow();//nastaví velikost sloupců a řádků dle aktuálního nastavení a potřeby
}
//---------------------------------------------------------------------------
//nastaví šířku buněk sloupců dle šířky textu dle zvoleného parametru
void TmGrid::executeColumnsAutoFit(TCanvas *Canv)
{
	switch(SetColumnAutoFitColIdx)
	{
		////nastavení velikosti sloupce dle ruční nastavení šířky daného sloupce, tj. nedělat autofit
		case -4:
		{
		}
		break;
		////defualtní šířka
		case -3:
		{
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=DefaultColWidth;
		}
		break;
		////všechny sloupce stejně podle nejširšího sloupce
		case -2:
		{
			unsigned int MaxColWidth=4;//minimální rozměr buňky pokud je nastavena na autofit a neobsahuje text
			//nalezení maxima
			for(unsigned long X=0;X<ColCount;X++)//po sloupcích
			{
				for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
				{
					unsigned int W=getWidthHeightText(Cells[X][Y]).X+Cells[X][Y].LeftMargin+Cells[X][Y].LeftBorder->Width/2+Cells[X][Y].RightMargin+Cells[X][Y].RightBorder->Width/2;
					if(Cells[X][Y].Type==CHECK || Cells[X][Y].Type==RADIO)W+=20;
					if(W>MaxColWidth)MaxColWidth=W;//najde nejšiřší
				}
			}
			//zápis do pole šířky sloupců
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=MaxColWidth;
		}
		break;
		////všechny sloupce zarovnat individuálně dle každého sloupce
		case -1:
		{
			for(unsigned long X=0;X<ColCount;X++)//po sloupcích
			executeColumnAutoFit(Canv,X);
		}
		break;
		////pouze konkrétní 0++
		default:
		{
			executeColumnAutoFit(Canv,SetColumnAutoFitColIdx);
		}
		break;
	}
}
//---------------------------------------------------------------------------
//nastaví šířku buněk daného sloupce dle šířky textu v daném sloupci
void TmGrid::executeColumnAutoFit(TCanvas *Canv,long ColIdx)
{
	Columns[ColIdx].Width=GetRecommendedColumnWidth(ColIdx);//nalezne nejširší položku a podle toho celý sloupec i zarovná
}
//---------------------------------------------------------------------------
//vratí doporučenou šířku sloupce dle jeho obsahu
int TmGrid::GetRecommendedColumnWidth(long ColIdx)
{
	unsigned int ColWidth=getWidthHeightText(Cells[ColIdx][0]).X;//výchozí hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		short Zoom=1;if(AntiAliasing_text==true)Zoom=3;
		unsigned int W=getWidthHeightText(Cells[ColIdx][Y]).X+Cells[ColIdx][Y].LeftMargin*Zoom+Cells[ColIdx][Y].LeftBorder->Width*Zoom/2+Cells[ColIdx][Y].RightMargin*Zoom+Cells[ColIdx][Y].RightBorder->Width*Zoom/2;
		if(Cells[ColIdx][Y].Type==CHECK || Cells[ColIdx][Y].Type==RADIO)W+=20+4+4;
		if(W>ColWidth)ColWidth=W;//najde nejšiřší
	}
	return ColWidth+4;//+4 pouze okrajový offset
}
//---------------------------------------------------------------------------
//vratí doporučenou šířku sloupce dle dodaného textu a aktuálního nastavení canvasu
int TmGrid::GetRecommendedColumnWidth(AnsiString Text)
{
	return Form->Canvas->TextWidth(Text)+4;//+4 pouze okrajový offset;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vrátí celkovou šířku tabulky
unsigned long TmGrid::getWidth()
{
	return Columns[ColCount-1].Left+Columns[ColCount-1].Width;
}
//---------------------------------------------------------------------------
//vrátí celkovou výšku tabulky
unsigned long TmGrid::getHeight()
{
	return Rows[RowCount-1].Top+Rows[RowCount-1].Height;
}
//---------------------------------------------------------------------------
//vrátí šířku a výšku textu buňky v pixelech
TPoint TmGrid::getWidthHeightText(TCells &Cell)
{
	TPoint RET;
	Form->Canvas->Font=Cell.Font;//nastavení fontu
	AnsiString T=Cell.Text;
	if(Cell.Text.Pos("<a>"))//pokud obsahuje odkaz, tak vyseparovat tagy
	T=ms.delete_repeat(ms.delete_repeat(T,"<a>"),"</a>");

	if(Cell.Font->Orientation==900 || Cell.Font->Orientation==2700)
	{
		RET.Y=Form->Canvas->TextWidth(T);
		RET.X=Form->Canvas->TextHeight(T);
	}
	else
	{
		RET.X=Form->Canvas->TextWidth(T);
		RET.Y=Form->Canvas->TextHeight(T);
  }
	return RET;
}
//---------------------------------------------------------------------------
//zajistí rotaci textu
void TmGrid::rotace_textu(long rotace)
{
	LOGFONT LogRec;
	GetObject(Form->Canvas->Font->Handle,sizeof(LogRec),&LogRec);
	LogRec.lfEscapement=rotace;
	Form->Canvas->Font->Handle=CreateFontIndirect(&LogRec);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//dle zadaného  čísla sloupce a čísla řádku  vrátí ukazatel nadanou komponentu
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPGlyphButton *TmGrid::getGlyphButton(unsigned long Col,unsigned long Row){return (TscGPGlyphButton *)Form->FindComponent("mGrid_GlyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPComboEdit *TmGrid::getComboEdit(unsigned long Col,unsigned long Row){return (TscGPComboEdit *)Form->FindComponent("mGrid_COMBOEDIT_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));};//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
TscHTMLLabel *TmGrid::getLabel(unsigned long Col,unsigned long Row){return (TscHTMLLabel *)Form->FindComponent("mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));};//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vrátí z dané komponenty text do paměťové buňky, slouží např. při události onchange popř. dálších
void TmGrid::getTextFromComponentToMemoryCell(unsigned long Col,unsigned long Row)
{
	switch(Cells[Col][Row].Type)
	{
		case EDIT:Cells[Col][Row].Text=getEdit(Col,Row)->Text;break;
		case NUMERIC:Cells[Col][Row].Text=getNumeric(Col,Row)->Value;break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnClick(TObject *Sender)
{
	if(!deleteMark)//detekce že nedochází k odstraňování mGridu, přitom nesmí k události docházet
	{
		//ShowMessage(AnsiString("OnClick ")+IntToStr(((TComponent*)(Sender))->Tag));
		switch(((TComponent*)(Sender))->Tag)
		{
			case -2:Col=-2;Row=-2;break;//exBUTTON
			case -1:break;//Hint - kdyby měl události kliku, tu nyní nemá
			default://komponenty v tabulce
			Col=getColFromTag(((TComponent*)(Sender))->Tag);
			Row=getRowFromTag(((TComponent*)(Sender))->Tag);
			break;
		}
																					 //pozor metody musí mít i znamenkové longové Col, Row , kvůli -2 exBUTTON
		if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnClick(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnClick(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnClick(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnClick(Tag,ID,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnClick(Tag,Col,Row);
    if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnClick(Tag,ID,Col,Row);   //z unit1 do unitX
		if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnClick(Tag,ID,Col,Row);
    if(AnsiString(Tag).SubString(1,1)=="8")Form_katalog->OnClick(Tag,ID,Col,Row);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnEnter(TObject *Sender)
{
  Hint->Visible=false;//asi zbytečné
	if(!deleteMark)//detekce že nedochází k odstraňování mGridu, přitom nesmí k události docházet
	{
		//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
		bufText=Cells[Col][Row].Text;//ukládá výchozí hodnotu editboxu před psaním, pro případ stisku ESC
		if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnEnter(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnEnter(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnEnter(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnEnter(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnEnter(Tag,Col,Row);
    if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnEnter(Tag,ID,Col,Row);//z unit1 do unitX
    if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnEnter(Tag,Col,Row);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnChange(TObject *Sender)
{
	if(!deleteMark)//detekce že nedochází k odstraňování mGridu, přitom nesmí k události docházet
	{
		//ShowMessage(AnsiString("OnChange ")+IntToStr(((TComponent*)(Sender))->Tag));
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
		getTextFromComponentToMemoryCell(Col,Row);//dle zadaného čísla sloupce a čísla řádku vrátí z dané komponenty text do paměťové buňky, slouží např. při události onchange popř. dálších
		if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnChange(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnChange(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnChange(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnChange(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnChange(Tag,Col,Row);
    if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnChange(Tag,ID,Col,Row);//z unit1 do unitX
    if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnChange(Tag,Col,Row);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnKeyDown(TObject *Sender,WORD &Key, TShiftState Shift)
{
	if(!deleteMark)//detekce že nedochází k odstraňování mGridu, přitom nesmí k události docházet
	{
//		Col=getColFromTag(((TComponent*)(Sender))->Tag);
//		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
//		getTextFromComponentToMemoryCell(Col,Row);//dle zadaného čísla sloupce a čísla řádku vrátí z dané komponenty text do paměťové buňky, slouží např. při události onchange popř. dálších
//    POZOR!!!, V PŘIPADĚ UŽITÍ NĚKTERÝCH NÍŽE UVEDENÝCH VOLÁNÍ, PŘÍPADNĚ DALŠÍCH PŘIDANÝCH NUTNO ODKOMENTOVAT PRVNÍ TŘI VÝŠE UVEDENÉ ŘÁDKY
//		if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnKeyDown(Tag,Col,Row,Key);
//		if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnKeyDown(Tag,Col,Row,Key);
//		if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnKeyDown(Tag,Col,Row,Key);
//		if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnKeyDown(Tag,Col,Row,Key);
//		if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnKeyDown(Tag,Col,Row,Key);
//		if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnKeyDown(Tag,ID,Col,Row,Key);//z unit1 do unitX
//		if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnKeyDown(Tag,Col,Row,Key);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnKeyPress(TObject *Sender,System::WideChar &Key)
{
	if(!deleteMark)//detekce že nedochází k odstraňování mGridu, přitom nesmí k události docházet
	{
		//adresace + nezbytné plnění z Editu do paměti!!!
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
		if(Key==VK_ESCAPE)//v případě stisku ESC návrat původního textu
		{
			TscGPEdit *E=getEdit(Col,Row);//->Sen //Text=bufText;//Cells[Col][Row].Text=bufText;
			TscGPNumericEdit *N=getNumeric(Col,Row);
			if(E!=NULL){E->Text=bufText;E->SelectAll();}E=NULL;delete E;
			if(N!=NULL){N->Value=ms.MyToDouble(bufText);E->SelectAll();}N=NULL;delete N;
		}
		getTextFromComponentToMemoryCell(Col,Row);//dle zadaného čísla sloupce a čísla řádku vrátí z dané komponenty text do paměťové buňky, slouží např. při události onchange popř. dálších

		//filtr kláves          přepsat 0 bez omezení 1 R 2 pouze kladná
		if(Cells[Col][Row].InputNumbersOnly!=0 && Key!=VK_ESCAPE)Key=ms.numericFilter(Cells[Col][Row].Text,Key,true,Cells[Col][Row].InputNumbersOnly);//pokud je nastaveno na true a není stisknuta klávesa backspace, nelze vepsat jinou hodnotu než číselnou (to včetně reálného čísla)

		//namapování dceřinných událostí - odkomentovat patřičnou + případně upravit požadované parametry
		//if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnKeyPress(Tag,Col,Row,Key);
		if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnKeyPress(Tag,ID,Col,Row,Key);//z unit1 do unitX
		//if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnKeyPress(Tag,ID,Col,Row,Key);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	Form1->FormMouseMove(Sender,Shift,X,Y);
}
//---------------------------------------------------------------------------
//vrací událost při vstupu či přejetí myší přes komponentu
void __fastcall TmGrid::getTagOnMouseEnter(TObject *Sender)
{
  Hint->Visible=false;//zruší případný Hint na DRAW
}
//---------------------------------------------------------------------------
//událost časovače
void __fastcall TmGrid::OnTimer(TObject *Sender)
{
	Hint->Visible=true;//zobrazí timer až po uplynutí daného času
	Timer->Enabled=false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//spojí dvě buňky do jedné
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	if(ColCell_1!=ColCell_2 || RowCell_1!=RowCell_2)//pokud se jedná o sloučení jedné buňky - nelogické sloučení nic nevykoná
	{
		SetColRow();

		////kopie referenční buňky //TCells RefCell=Cells[ColCell_1][RowCell_1];// - nelze však použít takto na předávání borders, proto metoda níže, předávalo by i ukazatel
		TCells RefCell;CreateCell(RefCell);
		CopyCell(Cells[ColCell_1][RowCell_1],RefCell,true);

		////nastavení referenční buňky kvůli orámování všech buněk oblasti na totožnou barvu
		TBorder B;B.Width=0;B.Style=psSolid;B.Color=Cells[ColCell_1][RowCell_1].Background->Color;
		*Cells[ColCell_1][RowCell_1].TopBorder=*Cells[ColCell_1][RowCell_1].BottomBorder=*Cells[ColCell_1][RowCell_1].LeftBorder=*Cells[ColCell_1][RowCell_1].RightBorder=B;
		Cells[ColCell_1][RowCell_1].MergeState=true;//označí buňku jako sloučenou, slouží pro přeskočení vykreslování orámování, uprostřed sloučených objektů ale také kvůli nastavení šířky komponent

		////projde nejdříve všechny buňky nastaví jim prvně dle pozadí první buňky stejné pozadí a dle barvy pozadí i barvu orámování
		SetCells(Cells[ColCell_1][RowCell_1],ColCell_1,RowCell_1,ColCell_2,RowCell_2,-1,false);

		////typ první buňky
		Cells[ColCell_1][RowCell_1].Type=RefCell.Type;

		////vytvoří resp. předá orámování oblasti dle referenční buňky, šlo by řešit v ve výše volaném průchodu, bylo by sice systomově méně náročné, ale více komplikované na realizaci
		SetRegion(RefCell,ColCell_1,RowCell_1,ColCell_2,RowCell_2);

		////text
		int W=0;int H=0;
		Cells[ColCell_2][RowCell_2].Text=RefCell.Text;//navrácení ze zálohy do poslední buňky, protože ta se vykresluje jako poslední
		W=getWidthHeightText(RefCell).X;
		H=getWidthHeightText(RefCell).Y;
		if(RefCell.Font->Orientation==900){H=0;if(RefCell.Valign==MIDDLE)H=-getWidthHeightText(RefCell).X;}
		if(RefCell.Font->Orientation==2700){W=0;if(RefCell.Align==LEFT || RefCell.Align==CENTER)W=-H;H=0;if(RefCell.Valign==MIDDLE)H=getWidthHeightText(RefCell).X;}
		//if(Cell.Font->Orientation==2700)L-=H;

		//nastaví velikost sloupců a řádků dle aktuálního nastavení a potřeby - DŮLEŽITE pro text!!!
		Cells[ColCell_2][RowCell_2].Align=aNO;
		Cells[ColCell_2][RowCell_2].Valign=vNO;
		//zarovnání (zarovnává dle první buňky, ale pracuje s poslední, protože ta se vykresluje zcela poslední)
		switch(Cells[ColCell_1][RowCell_1].Align)//HORIZONTÁLNÍ ZAROVNÁNÍ
		{
			 case LEFT:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_1].Left-Columns[ColCell_2].Left;//řeším v setcomponents +RefCell.LeftMargin+RefCell.LeftBorder->Width/2;
			 }
			 break;
			 case CENTER:
			 {
				 switch(RefCell.Type)
				 {
						case CHECK:
						{
							Cells[ColCell_1][RowCell_1].Align=aNO;
							TscGPCheckBox *Ch=createCheck(ColCell_1,RowCell_1);
							Ch->Width=Ch->OptionsChecked->ShapeSize;
							Ch->Left=Left+(Columns[ColCell_1].Left+Columns[ColCell_2].Left+Columns[ColCell_2].Width)/2-(Ch->Width-Ch->Options->FrameWidth)/2;
							Ch=NULL;delete Ch;
						}break;
						case RADIO:
						{
							Cells[ColCell_1][RowCell_1].Align=aNO;
							TscGPRadioButton *Ra=createRadio(ColCell_1,RowCell_1);
							Ra->Width=Ra->OptionsChecked->ShapeSize;
							Ra->Left=Left+(Columns[ColCell_1].Left+Columns[ColCell_2].Left+Columns[ColCell_2].Width)/2-Ra->Width/2;
							Ra=NULL;delete Ra;
						}break;
						case COMBO:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPComboBox *C=createCombo(ColCell_1,RowCell_1);
							C->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							C->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)C->Width-=floor(Border.Width/2.0);else C->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//ještě orámování
							C=NULL;delete C;
						}break;
            case COMBOEDIT:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPComboEdit *C=createComboEdit(ColCell_1,RowCell_1);
							C->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							C->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)C->Width-=floor(Border.Width/2.0);else C->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//ještě orámování
							C=NULL;delete C;
						}break;
						case EDIT:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPEdit *E=createEdit(ColCell_1,RowCell_1);
							E->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							E->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)E->Width-=floor(Border.Width/2.0);else E->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//ještě orámování
							E->Text=RefCell.Text;
							E=NULL;delete E;
						}break;
						case NUMERIC:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPNumericEdit *N=createNumeric(ColCell_1,RowCell_1);
							N->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							N->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)N->Width-=floor(Border.Width/2.0);else N->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//ještě orámování
							N->Value=ms.MyToDouble(RefCell.Text);//bere až z poslední buňky slučované oblasti
							N=NULL;delete N;
						}break;
						case LABEL:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscHTMLLabel *L=createLabel(ColCell_1,RowCell_1);
							L->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							L->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)L->Width-=floor(Border.Width/2.0);else L->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//ještě orámování
							L->Caption=RefCell.Text;//bere až z poslední buňky slučované oblasti
							L=NULL;delete L;
						}break;
						default: Cells[ColCell_2][RowCell_2].TextPositon.X=m.round((Columns[ColCell_1].Left-Columns[ColCell_2].Left+Columns[ColCell_2].Width+Cells[ColCell_1][RowCell_1].LeftBorder->Width-Cells[ColCell_2][RowCell_2].RightBorder->Width)/2.0-W/2.0);break;
				 }
			 }break;
			 case RIGHT:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_2].Width-W;//řeším v setcomponents -RefCell.RightMargin-RefCell.RightBorder->Width/2;
			 }break;
		}
		switch(Cells[ColCell_1][RowCell_1].Valign)//VERTIKÁLNÍ ZAROVNÁNÍ
		{
			 case TOP:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_1].Top-Rows[RowCell_2].Top;//řeším v setcomponents +RefCell.TopMargin+RefCell.TopBorder->Width/2;
			 }break;
			 case MIDDLE:
			 {
				 switch(RefCell.Type)
				 {
						case CHECK:
						{
							Cells[ColCell_1][RowCell_1].Valign=vNO;
							TscGPCheckBox *Ch=createCheck(ColCell_1,RowCell_1);
							Ch->Height=Ch->OptionsChecked->ShapeSize;
							Ch->Top=Top+(Rows[RowCell_1].Top+Rows[RowCell_2].Top+Rows[RowCell_2].Height)/2-(Ch->Height-Ch->Options->FrameWidth)/2;
							Ch=NULL;delete Ch;
						}break;
						case RADIO:
						{
							Cells[ColCell_1][RowCell_1].Valign=vNO;
							TscGPRadioButton *Ra=createRadio(ColCell_1,RowCell_1);
							Ra->Height=Ra->OptionsChecked->ShapeSize;
							Ra->Top=Top+(Rows[RowCell_1].Top+Rows[RowCell_2].Top+Rows[RowCell_2].Height)/2-Ra->Height/2;
							Ra=NULL;delete Ra;
						}break;
						case COMBO:
						{
							Cells[ColCell_1][RowCell_1].Valign=MIDDLE;
							TscGPComboBox *C=createCombo(ColCell_1,RowCell_1);
							C->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							C=NULL;delete C;
						}break;
            case COMBOEDIT:
						{
							Cells[ColCell_1][RowCell_1].Valign=MIDDLE;
							TscGPComboEdit *C=createComboEdit(ColCell_1,RowCell_1);
							C->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							C=NULL;delete C;
						}break;
						case EDIT:
						{
							Cells[ColCell_1][RowCell_1].Valign=MIDDLE;
							TscGPEdit *E=createEdit(ColCell_1,RowCell_1);
							E->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							E=NULL;delete E;
						}break;
						case NUMERIC:
						{
							Cells[ColCell_1][RowCell_1].Valign=MIDDLE;
							TscGPNumericEdit *N=createNumeric(ColCell_1,RowCell_1);
							N->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							N=NULL;delete N;
						}break;
						case LABEL:
						{
							Cells[ColCell_1][RowCell_1].Valign=MIDDLE;
							TscHTMLLabel *L=createLabel(ColCell_1,RowCell_1);
							L->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							L=NULL;delete L;
						}break;
						default:
						short o=0;//if(RefCell.Font->Size==14 && RefCell.Font->Name=="Roboto Cn")o=1;/*+(1-RefCell.Font->Size%2)/2.0*/// pouze grafická korekce pro fonty se sudou velikostí
						Cells[ColCell_2][RowCell_2].TextPositon.Y=m.round((Rows[RowCell_1].Top-Rows[RowCell_2].Top+Rows[RowCell_2].Height-Cells[ColCell_1][RowCell_1].TopBorder->Width-Cells[ColCell_2][RowCell_2].BottomBorder->Width)/2.0-H/2.0+o/2.0);break;
				 }
			 }
			 break;
			 case BOTTOM:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_2].Height-H;//řeším v setcomponents -RefCell.BottomMargin-RefCell.BottomBorder->Width/2;
			 }
			 break;
		}
	}
}
//---------------------------------------------------------------------------
//nastaví oblast buněk totožnými vlastnostmi dle referenční buňky, text podle posledního parametru buď -1 -smaže, 0 - zanechá původní (implicitně), 1 zkopíruje všude stejný), začínat zadávat od nejvyšší a nejvíce vlevo
void TmGrid::SetCells(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,short setText,bool copyComponent)
{
	for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
	{
		for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
		{
			UnicodeString Text=Cells[X][Y].Text;//záloha textu, varianta zanechá původní
			if(setText==-1)Text="";//varianta smaže
			if(setText==1)Text=RefCell.Text;//varianta všude stejný
			//Cells[X][Y]=RefCell;// - nelze však použít na předávání borders, proto metoda níže
			CopyCell(RefCell,Cells[X][Y],copyComponent);
			Cells[X][Y].Text=Text;//navrácení textu
		}
	}
}
//---------------------------------------------------------------------------
void TmGrid::SetRegion(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)//totožně ohraničí danou oblast buněk dle referenční buňky (zohledňuje i rozdíly horní,dolní,levé pravé orámování), začínat zadávat od nejvyšší a nejvíce vlevo
{
	//top a bottom okraj
	for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
	{
		*Cells[X][RowCell_1].TopBorder=*RefCell.TopBorder;
		*Cells[X][RowCell_2].BottomBorder=*RefCell.BottomBorder;
	}
	//left a right okraj
	for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
	{
		*Cells[ColCell_1][Y].LeftBorder=*RefCell.LeftBorder;
		*Cells[ColCell_2][Y].RightBorder=*RefCell.RightBorder;
	}
}
//---------------------------------------------------------------------------
////zajistí trvalé (jedná se spíše o nastavení) zvýraznění vnějšího orámování buňky
//void TmGrid::HighlightCell(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width,bool Refresh)
//{
//// pro případ zkonkretizovaní požadovvku odkomentovat
////	switch(Cells[Col][Row].Type)
////	{
////		case DRAW:
////		{
////			TBorder hlBorder;
////			hlBorder.Color=Color;
////			hlBorder.Width=Width;
////			hlBorder.Style=psSolid;
////			*Cells[Col][Row].TopBorder=hlBorder;
////			*Cells[Col][Row].LeftBorder=hlBorder;
////			*Cells[Col][Row].RightBorder=hlBorder;
////			*Cells[Col][Row].BottomBorder=hlBorder;
////if(Refresh)Refresh();
//
//			Cells[Col][Row].Font->Color=(TColor)RGB(225,128,0);
//			if(Refresh)TmGrid::Refresh();
//
////		}break;
////		case EDIT:HighlightEdit(Col,Row,Color,Width);break;
////		case NUMERIC:HighlightNumeric(Col,Row,Color,Width);break;
////	}
//}
////---------------------------------------------------------------------------
////zajistí trvalé (jedná se spíše o nastavení) zvýraznění dané komponenty
//void TmGrid::HighlightEdit(TscGPEdit *Edit,TColor Color,unsigned short Width)
//{
//	Edit->Options->FrameNormalColor=Color;
//	Edit->Options->FrameWidth=Width;
//}
////---------------------------------------------------------------------------
////zajistí trvalé (jedná se spíše o nastavení) zvýraznění dané komponenty
//void TmGrid::HighlightEdit(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width)
//{
//	HighlightEdit(getEdit(Col,Row),Color,Width);
//}
////---------------------------------------------------------------------------
////zajistí trvalé (jedná se spíše o nastavení) zvýraznění dané komponenty
//void TmGrid::HighlightNumeric(TscGPNumericEdit *Numeric,TColor Color,unsigned short Width)
//{
//	Numeric->Options->FrameNormalColor=Color;
//	Numeric->Options->FrameWidth=Width;
//}
////---------------------------------------------------------------------------
////zajistí trvalé (jedná se spíše o nastavení) zvýraznění dané komponenty
//void TmGrid::HighlightNumeric(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width)
//{
//	HighlightNumeric(getNumeric(Col,Row),Color,Width);
//}
////---------------------------------------------------------------------------
//odzvýrazni všechna zvýraznění
void TmGrid::unHighlightAll()
{
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po řádcích
		{
			Cells[X][Y].Highlight=false;
		}
	}
}
//---------------------------------------------------------------------------
//zkopíruje obsah, formát a orámování z buňky na buňku (bez ukazatelového propojení)
void TmGrid::CopyCell(TCells &RefCell,TCells &CopyCell,bool copyComponent)
{
	//zkopíruje obsah, formát (bez orámování) z buňky na buňku (bez ukazatelového propojení)
	CopyAreaCell(RefCell,CopyCell,copyComponent);
	//zkopíruje orámování z buňky na buňku (bez ukazatelového propojení)
	CopyBordesCell(RefCell,CopyCell);
}
//---------------------------------------------------------------------------
//zkopíruje danou oblast do schránky, buňky oddělí separátorem
void TmGrid::CopyCells2Clipboard(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,UnicodeString Separator)
{
	TClipboard *c = new TClipboard;
	UnicodeString T="";
	for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
	{
    for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
		{
			T+=mGrid->Cells[X][Y].Text+Separator;
		}
		T+="\n";//konec řádku
	}
	c->AsText = T;
	c = NULL; delete c;
}
//---------------------------------------------------------------------------
//zkopíruje obsah, formát (bez orámování) z buňky na buňku (bez ukazatelového propojení)
void TmGrid::CopyAreaCell(TCells &RefCell,TCells &CopyCell,bool copyComponent)
{
	////typ
	if(copyComponent){CopyCell.Type=RefCell.Type;/*createComponent(RefCell.Type,)*/}
	else CopyCell.Type=DRAW;
	////text + font
	//*CopyCell.Font=*RefCell.Font; - asi nejede
	CopyCell.Font->Size=RefCell.Font->Size;
	CopyCell.Font->Color=RefCell.Font->Color;
	CopyCell.Font->Orientation=RefCell.Font->Orientation;
	CopyCell.Font->Style=RefCell.Font->Style;
	CopyCell.Font->Pitch=RefCell.Font->Pitch;
	CopyCell.Font->Name=RefCell.Font->Name;
	*CopyCell.isNegativeNumber=*RefCell.isNegativeNumber;
	*CopyCell.isZero=*RefCell.isZero;
	*CopyCell.isLink=*RefCell.isLink;
	*CopyCell.isActiveLink=*RefCell.isActiveLink;
	CopyCell.TextPositon.X=RefCell.TextPositon.X;
	CopyCell.TextPositon.Y=RefCell.TextPositon.Y;
	CopyCell.Text=RefCell.Text;
	////zarovnání
	CopyCell.Align=RefCell.Align;
	CopyCell.Valign=RefCell.Valign;
	//osazení
	CopyCell.TopMargin=RefCell.TopMargin;
	CopyCell.BottomMargin=RefCell.BottomMargin;
	CopyCell.LeftMargin=RefCell.LeftMargin;
	CopyCell.RightMargin=RefCell.RightMargin;
	//indikátor sloučení
	CopyCell.MergeState=RefCell.MergeState;
	//hint
	CopyCell.ShowHint=RefCell.ShowHint;
	//další vlastnosti
	CopyCell.InputNumbersOnly=RefCell.InputNumbersOnly;
	CopyCell.Highlight=RefCell.Highlight;
	////pozadí
	//*CopyCell.Background=*RefCell.Background;  - asi nejede
	CopyCell.Background->Color=RefCell.Background->Color;
	CopyCell.Background->Style=RefCell.Background->Style;
	CopyCell.isEmpty->Color=RefCell.isEmpty->Color;
	CopyCell.isEmpty->Style=RefCell.isEmpty->Style;
}
//---------------------------------------------------------------------------
//zkopíruje orámování z buňky na buňku (bez ukazatelového propojení)
void TmGrid::CopyBordesCell(TCells &RefCell,TCells &CopyCell)
{
	*CopyCell.TopBorder=*RefCell.TopBorder;
	*CopyCell.BottomBorder=*RefCell.BottomBorder;
	*CopyCell.LeftBorder=*RefCell.LeftBorder;
	*CopyCell.RightBorder=*RefCell.RightBorder;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//smaže text v celém sloupci
void TmGrid::ClearColumn(unsigned long ColIdx)
{
	for(unsigned long Y=0;Y<=RowCount;Y++)
	{
		Cells[ColIdx][Y].Text="";
	}
}
//---------------------------------------------------------------------------
//smaže text v celém řádku
void TmGrid::ClearRow(unsigned long RowIdx)
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		Cells[X][RowIdx].Text="";
	}
}
//---------------------------------------------------------------------------
//smaže text celé tabulky
void TmGrid::Clear()
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		ClearColumn(X);
	}
}
//---------------------------------------------------------------------------
//přidá řádek za poslední řádek, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z předchozího řádku, pokud je invalidate na true, automaticky po přidání překreslí tabulku, někdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint přímo za voláním metody AddRow přimo v užitém formuláři
void TmGrid::AddRow(bool copyComponentFromPreviousRow,bool invalidate)
{  //padá asi v realock pří přidávání řádků, ale spíše na mgrid2
	//při realock asi neudrží výšku řádků

  //zvýšení celkového počtu řádků
	RowCount++;

	//kopie komponent z nadřízeného řádku, jeli-požadováno
	if(copyComponentFromPreviousRow)
	{
		realock();//musí proběhnout před následujícím kodem, jinak řeší Show
		for(unsigned long X=0;X<ColCount;X++)
		{
			Cells[X][RowCount-1].Type=Cells[X][RowCount-2].Type;
			//následující není třeba, pořeší jen výše uvedené + následné show: createComponent(Cells[X][RowCount-1].Type,X,RowCount-1);//dle zadaného čísla sloupce a čísla řádku vytvořenou komponentu dle Type, pokud existuje, tak se nic neděje
		}
	}

	//pokud je požadováno překreslení
	if(invalidate)Show();//překreslení s problikem, jinak použít přímo ve formu formpaint a toto zakomentovat
}
//---------------------------------------------------------------------------
//přídá řádek za řádek uvedený dle parametru Row,, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z předchozího řádku, pokud je invalidate na true, automaticky po přidání překreslí tabulku, někdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint přímo za voláním metody InsertRow přimo v užitém formuláři
void TmGrid::InsertRow(unsigned long Row,bool copyComponentFromPreviousRow, bool invalidate)
{
	if(Row<RowCount-1)//pokud se nejedná o poslední řádek, tam je to zbytečné a řeší else větev AddRow
	{
		RowCount++;realock();//přidá poslední řádek
		//překopíruje řádek resp. buňky z řádku následujícího a předtím přidá poslední řádek
		for(unsigned long Y=RowCount-1;Y>Row;Y--)
		{
			for(unsigned long X=0;X<ColCount;X++)
			{
				CopyCell(Cells[X][Y-1],Cells[X][Y],true);
			}
			Rows[Y]=Rows[Y-1];
		}
		ClearRow(Row);//vyprázní vkládaný řádek

		if(invalidate)//pokud je požadováno překreslení, zde je nutné celou oblast překreslit
		{
			//Show();netřeba //Form->Invalidate();//třeba
			Show();
		}
	}
	else AddRow(copyComponentFromPreviousRow,invalidate);
}
//---------------------------------------------------------------------------
//skryje či zobrazí daný řádek - pozor ještě optimálně nefunguje orámování pokud je na skrývaném řádku Editbox a následujícím také- převezme se této buňce šířka orámování původního editboxu
void TmGrid::VisibleRow(unsigned long Row,bool visible,bool invalidate)
{
	if(visible==false)//skrývání
	{
		AnsiString T=Note.Text;
		if(Note.Text!="")ShowNote("");//kvůli posunu poznámky
		Rows[Row].Visible=false;
		if(T!="")ShowNote(T);
		if(invalidate)InvalidateRect(Form->Handle,&TRect(Left-Cells[0][Row].LeftBorder->Width,Top+Height-Rows[Row].Height-Cells[0][Row].TopBorder->Width,Left+Width+Cells[ColCount-1][Row].RightBorder->Width,Top+Height+Cells[0][Row].TopBorder->Width),true);
		if(invalidate)Refresh();
	}
	else//zobrazování
	{
		Rows[Row].Height=DefaultRowHeight;//vratí původní resp. defaultní výšku
		Rows[Row].Visible=true;
		if(invalidate)Refresh();
	}
}
//---------------------------------------------------------------------------
//smaže celý řádek, pokud je invalidate na true, automaticky po přidání překreslí tabulku, někdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint přímo za voláním metody InsertRow přimo v užitém formuláři
void TmGrid::DeleteRow(unsigned long Row,bool invalidate)
{
	if(Row<=RowCount-1 && RowCount-1>0)//nelze smazat nultý řádek
	{
		DeleteComponents(0,Row,ColCount-1,Row);//new 15.4. - odmazání komponent na aktuálním řádku, pozor, je nutné odevzdat focus mimo mazané komponenty, jinak nastane paměťová chyba
		//překopíruje řádek resp. buňky z řádku následujícího a ubere poslední řádek, pokud se nejedná o jediný řádek
		for(unsigned long Y=Row;Y<RowCount-1;Y++)
		{
			for(unsigned long X=0;X<ColCount;X++)
			{
				CopyCell(Cells[X][Y+1],Cells[X][Y],true);
				MoveComponentUP(X,Y+1);//15.4. test v případě zrušení vrátit níže uvedené  DeleteComponents
			}
			Rows[Y]=Rows[Y+1];
		}
		//15.4. již je nadbytečné DeleteComponents(0,RowCount-1,ColCount-1,RowCount-1);//smaže komponenty z posledního řádku, pozor, je nutné odevzdat focus mimo mazané komponenty, jinak nastane paměťová chyba
		RowCount--;
	}
	if(invalidate)Show();//pokud je požadováno překreslení //překreslení s problikem, jinak použít přímo ve formu formpaint a toto zakomentovat
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajistí realokaci pole Cells dle nové velikosti
void TmGrid::realock()
{
	//kopie do záložního pole a smazání
	TCells **bufCells = new TCells*[bufColCount];//vytvoří dynamické pole ukazatelu typu TCells dle počtu požadovaných sloupců
	TColumns *bufColumns = new TColumns[bufColCount];//vytvoří dynamické pole ukazatelu typu TColumns kvůli záloze šířky sloupců
	TRows *bufRows = new TRows[bufRowCount];//vytvoří dynamické pole ukazatelu typu TRows kvůli záloze výšky sloupců

	for(unsigned long X=0;X<bufColCount;X++)
	{
		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupcům dynamamického pole alokuje pamět pro jednotlivé řádky- cyklus musí být samostatně
		bufCells[X] = Cells[X];//zkopírování původních hodnot do zálohy //takto asi nevhodné kopírovat (zůstane ukazatel), s hvězdickou sice hodnoty ale je potřeba přidělit pamět pomocí new pro TBrush TFont a TBorders
		bufColumns[X].Width=Columns[X].Width;
	}

	for(unsigned long Y=0;Y<bufRowCount;Y++)bufRows[Y].Height=Rows[Y].Height;

	//počty řádků a sloupců
	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;

	//smazání původních hodnot
	//původní komponenty zachovám,  smažu jen ty, které nejsou již potřeba (při zmenšování tabulky), ale údajně nefunguje zcela správně
	DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);//nová oblast až stará oblast, není potřeba ošetřovat IFem pro případ přidávání (pohlídají si cykly ve vnitř algoritmu)
	ColCount=bufColCount2;RowCount=bufRowCount2;
	//toto proč ne DeleteTable(); asi nahrazuje Create, otázka však zůstává zda něco nezůstává v paměti

	//vytvoření nového realokovaného pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//určeno při další realokaci pole, create totiž přepisuje buf hodnoty
	Create(ColCount,RowCount);

	//zkopírování původních hodnot zpět
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je menší
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je menší
	for(unsigned long Y=0;Y<bufRowCount;Y++)
	{
		Rows[Y].Height=bufRows[Y].Height;//navrácení původní výšky řádku, protože voláním Create se nastavuje výška výchozí
		for(unsigned long X=0;X<bufColCount;X++)
		{
			//nelze celý ukazatel
			Cells[X][Y] = bufCells[X][Y];//popř. CopyCell(bufCells[X][Y],Cells[X][Y],true);//nová verze
			if(Y==0)Columns[X].Width=bufColumns[X].Width;//navrácení původní šířky slopců, protože voláním Create se nastavuje šířka výchozí
		}
	}
	bufColumns=NULL; delete bufColumns;
	bufRows=NULL; delete bufRows;

	//smazání původního bufCells
	for(unsigned long X=0;X<bufColCount2;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount2;Y++)//po řádcích
		{
			DeleteCell(bufCells[X][Y]);
		}
		bufCells[X]=NULL; delete bufCells[X];
	}
	bufCells=NULL; delete bufCells;
	bufColCount=ColCount;bufRowCount=RowCount;//určeno při další realokaci pole
}
//---------------------------------------------------------------------------
//dle zadaného čísla sloupce a čísla řádku vytvořenou komponentu dle Type, pokud existuje, tak se nic neděje
void TmGrid::createComponent(Ttype Type, unsigned long Col,unsigned long Row)
{
	switch(Type)
	{
		case readEDIT:break;
		case EDIT:				createEdit(Col,Row);break;
		case NUMERIC:			createNumeric(Col,Row);break;
		case readNUMERIC:break;
		case BUTTON:		 	createButton(Col,Row);break;
		case glyphBUTTON:	createGlyphButton(Col,Row);break;
		case COMBO:				createCombo(Col,Row); break;
    case COMBOEDIT:		createComboEdit(Col,Row); break;
		case CHECK:				createCheck(Col,Row);break;
		case RADIO:				createRadio(Col,Row);break;
		case DRAW:break;
		case LABEL:				createLabel(Col,Row);break;
	}
}
//---------------------------------------------------------------------------
//odstraní dynamicky vytořené komponenty, nutno volat před Delete()
void TmGrid::DeleteComponents()
{
	DeleteComponents(0,0,ColCount-1,RowCount-1);
}
//---------------------------------------------------------------------------
//odstraní dynamicky vytořené komponenty od - do počtu sloupců a řádků, nutno volat před Delete()
// pozor odstraňovaná komponenta nesmí mít focus (jinak paměťová chyba), focus je potřeba při odstraňování komponent odevzdat nějaké komponentě, která zůstává ve formu
void TmGrid::DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow)
{
	for(unsigned long X=sCol;X<=fCol;X++)//po řádcích
	{
		for(unsigned long Y=sRow;Y<=fRow;Y++)//po sloupcích
		{
			switch(Cells[X][Y].Type)
			{
				case readEDIT: {TscGPEdit *E=getEdit(X,Y);E->Free();E=NULL;delete E;}break;
				case EDIT: {TscGPEdit *E=getEdit(X,Y);E->Free();E=NULL;delete E;}break;
				case NUMERIC: {TscGPNumericEdit *N=getNumeric(X,Y);N->Free();N=NULL;delete N;}break;
				case readNUMERIC: {TscGPNumericEdit *N=getNumeric(X,Y);N->Free();N=NULL;delete N;}break;
				case BUTTON: {TscGPButton *B=getButton(X,Y);B->Free();B=NULL;delete B;}break;
				case COMBO: {TscGPComboBox *C=getCombo(X,Y);C->Free();C=NULL;delete C;}break;
        case COMBOEDIT: {TscGPComboEdit *C=getComboEdit(X,Y);C->Free();C=NULL;delete C;}break;
				case CHECK:{TscGPCheckBox *CH=getCheck(X,Y);CH->Free();CH=NULL;delete CH;break;} ///*CH->DisposeOf();*/ ani toto ani free při kliku nefungují správně, chyba byla, že daná komponenta měla focus, focus je potřeba při odstraňování komponent odevzdat nějaké komponentě, která zůstává ve formu
				case RADIO:{TscGPRadioButton *R=getRadio(X,Y);R->Free();R=NULL;delete R;}break;
				case glyphBUTTON:{TscGPGlyphButton *gB=getGlyphButton(X,Y);gB->Free();gB=NULL;delete gB;}break;
			}
		}
	}
}
//---------------------------------------------------------------------------
//posunu komponentu na řádku o jeden řádek níže, použává se nově na mazání řádku
void TmGrid::MoveComponentUP(unsigned long Col,unsigned long Row)
{
	Row--;//bude se přesouvat na nížší řádek
	switch(Cells[Col][Row+1].Type)
	{
		case readEDIT:
		case EDIT:
		{
			TscGPEdit *E=getEdit(Col,Row+1);
			E->Tag=getTag(Col,Row);//přeindexování na řádek níže
			E->Name="mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(E->Tag);//přeindexování na řádek níže
			E=NULL;delete E;
		}
		break;
		case readNUMERIC:
		case NUMERIC:
		{
			TscGPNumericEdit *N=getNumeric(Col,Row+1);
			N->Tag=getTag(Col,Row);//přeindexování na řádek níže
			N->Name="mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(N->Tag);//přeindexování na řádek níže
			N=NULL;delete N;
		}
		break;
		case BUTTON:
		{
			TscGPButton *B=getButton(Col,Row+1);
			B->Tag=getTag(Col,Row);//přeindexování na řádek níže
			B->Name="mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(B->Tag);//přeindexování na řádek níže
			B=NULL;delete B;
		}
		break;
		case LABEL:
		{
			TscHTMLLabel *L=getLabel(Col,Row+1);
			L->Tag=getTag(Col,Row);//přeindexování na řádek níže
			L->Name="mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(L->Tag);//přeindexování na řádek níže
			L=NULL;delete L;
		}
		break;
		case glyphBUTTON:
		{
			TscGPGlyphButton *gB=getGlyphButton(Col,Row+1);
			gB->Tag=getTag(Col,Row);//přeindexování na řádek níže
			gB->Name="mGrid_glyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(gB->Tag);//přeindexování na řádek níže
			gB=NULL;delete gB;
		}
		break;
		case COMBO:
		{
			TscGPComboBox *C=getCombo(Col,Row+1);
			C->Tag=getTag(Col,Row);//přeindexování na řádek níže
			C->Name="mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(C->Tag);//přeindexování na řádek níže
			C=NULL;delete C;
		}
		break;
    case COMBOEDIT:
		{
			TscGPComboEdit *C=getComboEdit(Col,Row+1);
			C->Tag=getTag(Col,Row);//přeindexování na řádek níže
			C->Name="mGrid_COMBOEDIT_"+AnsiString(ID)+"_"+AnsiString(C->Tag);//přeindexování na řádek níže
			C=NULL;delete C;
		}
		break;
		case CHECK:
		{
			TscGPCheckBox *Ch=getCheck(Col,Row+1);
			Ch->Tag=getTag(Col,Row);//přeindexování na řádek níže
			Ch->Name="mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(Ch->Tag);//přeindexování na řádek níže
			Ch=NULL;delete Ch;
		}
		break;
		case RADIO:
		{
			TscGPRadioButton *R=getRadio(Col,Row+1);
			R->Tag=getTag(Col,Row);//přeindexování na řádek níže
			R->Name="mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(R->Tag);//přeindexování na řádek níže
			R=NULL;delete R;
		}
		break;
	}
  //níže uvedené asi netřeba, protože se následně při refresh volá znovu
	//TRect R=TRect(Left+Columns[Col].Left,Top+Rows[Row].Top,Left+Columns[Col].Left+Columns[Col].Width,Top+Rows[Row].Top+Rows[Row].Height);
	//if(Cells[Col][Row+1].Type!=DRAW)SetComponents(NULL,R,R,Col,Row,Cells[Col][Row]);//nastaví komponenty, mimo DRAW, protože se nejedná o komponentu
}
//---------------------------------------------------------------------------
//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí řádek
long TmGrid::GetIdxRow(int X,int Y)
{
	long RET=-1;
	if(CheckPTinTable(X,Y))//ošetření mimo tabulku + akcelerátor
	{
		for (unsigned long i=0;i<RowCount;i++)
		{
			if(Rows[i].Top<=Y-Top && Y-Top<Rows[i].Top+Rows[i].Height)
			RET=i;
		}
	}
	return RET;
}
//---------------------------------------------------------------------------
//dle souřadnic ve formuláři,kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí sloupec
long TmGrid::GetIdxColumn(int X,int Y)
{
	long RET=-1;
	if(CheckPTinTable(X,Y))//ošetření mimo tabulku + akcelerátor
	{
		for (unsigned long i=0;i<ColCount;i++)
		{
			if(Columns[i].Left<=X-Left && X-Left<Columns[i].Left+Columns[i].Width)
			RET=i;
		}
	}
	return RET;
}
//---------------------------------------------------------------------------
//zajistí zvýraznění řádků dle čísla řádku Row
void TmGrid::HighlightRow(long Row,TColor Color,bool SelFirstRow,bool unHighlightPrevRow)
{
	//pokud se pohybuje v tabulce
	int FirstRow=0;if(!SelFirstRow)FirstRow=1;
	if(Row>=FirstRow && preRowInd!=Row)
	{
		if(unHighlightPrevRow)if(!(Row==1 && preRowInd==0))selRow(preRowInd,Color,false);//odstranění předchozího označení misto: Show();//Refresh s minimem probliku
		selRow(Row,Color,true);//nové označení
	}
	//při odchodu z tabulky, aby nezůstal "viset" označený řádek
	if(unHighlightPrevRow)
	{
		if(Row<=FirstRow-1 && preRowInd>=FirstRow)selRow(preRowInd,Color,false);//odstranění předchozího označení misto: Show();//Refresh s minimem probliku
		preRowInd=Row;//uložení aktivního řádku pro další použítí
  }
}
//---------------------------------------------------------------------------
//zajistí zvýraznění řádků, přes který se přejíždí myší
void TmGrid::HighlightRowOnMouse(int X,int Y,TColor Color,bool SelFirstRow,bool unHighlightPrevRow)
{
	long Row=GetIdxRow(X,Y);
	HighlightRow(Row,Color,SelFirstRow,unHighlightPrevRow);
}
//---------------------------------------------------------------------------
//označí řádek, nebo zruší označení řádku dle vstupního parametru
void TmGrid::selRow(long Row,TColor Color,bool newSel)
{
	if(newSel)Form->Canvas->Pen->Mode=pmMask;else Form->Canvas->Pen->Mode=pmNotXor;
	Form->Canvas->Brush->Style=bsSolid;
	Form->Canvas->Brush->Color=Color;
	Form->Canvas->Pen->Width=0;
	Form->Canvas->Pen->Color=Color;
	short C=1-(short)newSel;

	//TPoint body[4]={TPoint(mGrid->Left-C,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height),TPoint(mGrid->Left,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height)}; //mohlo by dělat problémy původní kostra
	TPoint body[4]={TPoint(Left-C,Top+Rows[Row].Top-C),TPoint(Left+Width,Top+Rows[Row].Top-C),TPoint(Left+Width,Top+Rows[Row].Top+Rows[Row].Height),TPoint(Left,Top+Rows[Row].Top+Rows[Row].Height)};
	Form->Canvas->Polygon(body,3);
	Form->Canvas->Pen->Mode=pmCopy;
}
//---------------------------------------------------------------------------
//zajistí zvýraznění orámování tabulky
void TmGrid::HighlightTable(TCanvas *Canvas,TColor Color,unsigned short Size,unsigned short Offset,TPenMode PenMode)
{
	Cvykresli d; d.set_pen(Canvas,Color,Size,PS_ENDCAP_SQUARE);
	Canvas->Brush->Style=bsClear;
	Canvas->Rectangle(Left-Offset,Top-Offset,Left+Width+Offset,Top+Height+Offset);
}
//---------------------------------------------------------------------------
//zajistí zvýraznění orámování tabulky, pokud se do ni vstoupí myší
void TmGrid::HighlightTableOnMouse(int X,int Y,TCanvas *Canvas)
{
	if(CheckPTinTable(X,Y))
	{
		if(Canvas==NULL)HighlightTable(Form->Canvas);
		else HighlightTable(Canvas);
	}
}
//---------------------------------------------------------------------------
//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) zjistí, zda jsou souřadnice ve vnitř tabulky
bool TmGrid::CheckPTinTable(int X,int Y)
{
	return Left<=X && X<=Left+Width && Y>Top && Y<Top+Height;
}
//---------------------------------------------------------------------------
//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí kladné číslo sloupce a řádku pokud se na daném místě nachází odkaz, pokud ne, vrácené hodnoty jsou -1 a -1
TPoint TmGrid::CheckLink(int X,int Y,bool invalidate)
{
	TmGrid::X=X;TmGrid::Y=Y;//předání do globální proměnné
	if(Note.LinkArea.PtInRect(TPoint(X,Y)) && Note.LinkArea!=TRect(-1,-1,-1,-1))//odkaz v poznámce
	{
		if(invalidate)
		{
			Form->Cursor=crHandPoint;
			Refresh();
    }
		return TPoint(-2,-2);
	}
	else//většina odkazů v tabulkách
	{
		Row=GetIdxRow(X,Y);
		Col=GetIdxColumn(X,Y);
		if(Row>=0 && Col>=0 && CheckLink(X,Y,Col,Row))
		{
			if(invalidate)
			{
				Form->Cursor=crHandPoint;
				Refresh();
			}
			return TPoint(Col,Row);//odkaz na daných souřadnicích nalezen
		}
		else
		{
			if(invalidate)
			{
				Form->Cursor=crDefault;
				Refresh();
			}
			return TPoint(-1,-1);
		}
	}
}
//---------------------------------------------------------------------------
//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí zda se na dané buňce a souřadnicích nachází odkaz
bool TmGrid::CheckLink(int X,int Y,unsigned long Col,unsigned long Row)
{
	if(Cells[Col][Row].LinkCoordinateStart.x<=X && X<=Cells[Col][Row].LinkCoordinateEnd.x
	&& Cells[Col][Row].LinkCoordinateStart.y<=Y && Y<=Cells[Col][Row].LinkCoordinateEnd.y)
	return true;
	else return false;
}
//---------------------------------------------------------------------------
//podle stavu state buď zobrazí nebo skryje všechny komponenty
void TmGrid::SetVisibleComponents(bool state)
{
	for(unsigned long X=0;X<=ColCount-1;X++)//po řádcích
	{
		for(unsigned long Y=0;Y<=RowCount-1;Y++)//po sloupcích
		{
			SetVisibleComponent(X,Y,state);//skryje komponentu na dané pozici
		}
	}
	if(exBUTTONVisible)exBUTTON->Visible=state;
}
//---------------------------------------------------------------------------
//podle stavu state buď zobrazí nebo skryje komponentu neurčitého typu v dané buňce
void TmGrid::SetVisibleComponent(unsigned long Col,unsigned long Row,bool state)
{
	switch(Cells[Col][Row].Type) //- musí se zároveń ověřovat, zda není NULL, nutné při znovuotevření náhledu
	{
		case readEDIT: 		{TscGPEdit *E=getEdit(Col,Row);if(E!=NULL)E->Visible=state;E=NULL;delete E;}break;
		case EDIT: 		 		{TscGPEdit *E=getEdit(Col,Row);if(E!=NULL)E->Visible=state;E=NULL;delete E;}break;
		case NUMERIC:  		{TscGPNumericEdit *N=getNumeric(Col,Row);if(N!=NULL)N->Visible=state;N=NULL;delete N;}break;
		case readNUMERIC:	{TscGPNumericEdit *N=getNumeric(Col,Row);if(N!=NULL)N->Visible=state;N=NULL;delete N;}break;
		case BUTTON: 			{TscGPButton *B=getButton(Col,Row);if(B!=NULL)B->Visible=state;B=NULL;delete B;}break;
		case COMBO:	 			{TscGPComboBox *C=getCombo(Col,Row);if(C!=NULL)C->Visible=state;C=NULL;delete C;}break;
    case COMBOEDIT:	 	{TscGPComboEdit *C=getComboEdit(Col,Row);if(C!=NULL)C->Visible=state;C=NULL;delete C;}break;
		case CHECK:				{TscGPCheckBox *CH=getCheck(Col,Row);if(CH!=NULL)CH->Visible=state;CH=NULL;delete CH;}break;
		case RADIO:				{TscGPRadioButton *R=getRadio(Col,Row);if(R!=NULL)R->Visible=state;R=NULL;delete R;}break;
		case glyphBUTTON:	{TscGPGlyphButton *gB=getGlyphButton(Col,Row);if(gB!=NULL)gB->Visible=state;gB=NULL;delete gB;}break;
	}
}
//---------------------------------------------------------------------------
//podle stavu state buď povolí či zakáže všechny komponenty
void TmGrid::SetEnabledComponents(bool state)
{
	for(unsigned long X=0;X<=ColCount-1;X++)//po řádcích
	{
		for(unsigned long Y=0;Y<=RowCount-1;Y++)//po sloupcích
		{
			SetEnabledComponent(X,Y,state);//skryje komponentu na dané pozici
		}
	}
	//if(exBUTTONEnabled)exBUTTON->Enabled=state;
}
//---------------------------------------------------------------------------
//podle stavu state buď povolí či zakáže komponentu neurčitého typu v dané buňce
void TmGrid::SetEnabledComponent(unsigned long Col,unsigned long Row,bool state)
{
	switch(Cells[Col][Row].Type) //- musí se zároveń ověřovat, zda není NULL, nutné při znovuotevření náhledu
	{
		case readEDIT: 		{TscGPEdit *E=getEdit(Col,Row);if(E!=NULL)E->Enabled=state;E=NULL;delete E;}break;
		case EDIT: 		 		{TscGPEdit *E=getEdit(Col,Row);if(E!=NULL)E->Enabled=state;E=NULL;delete E;}break;
		case NUMERIC:  		{TscGPNumericEdit *N=getNumeric(Col,Row);if(N!=NULL)N->Enabled=state;N=NULL;delete N;}break;
		case readNUMERIC:	{TscGPNumericEdit *N=getNumeric(Col,Row);if(N!=NULL)N->Enabled=false;N=NULL;delete N;}break;
		case BUTTON: 			{TscGPButton *B=getButton(Col,Row);if(B!=NULL)B->Enabled=state;B=NULL;delete B;}break;
		case COMBO:	 			{TscGPComboBox *C=getCombo(Col,Row);if(C!=NULL)C->Enabled=state;C=NULL;delete C;}break;
    case COMBOEDIT:	 	{TscGPComboEdit *C=getComboEdit(Col,Row);if(C!=NULL)C->Enabled=state;C=NULL;delete C;}break;
		case CHECK:				{TscGPCheckBox *CH=getCheck(Col,Row);if(CH!=NULL)CH->Enabled=state;CH=NULL;delete CH;}break;
		case RADIO:				{TscGPRadioButton *R=getRadio(Col,Row);if(R!=NULL)R->Enabled=state;R=NULL;delete R;}break;
		case glyphBUTTON:	{TscGPGlyphButton *gB=getGlyphButton(Col,Row);if(gB!=NULL)gB->Enabled=state;gB=NULL;delete gB;}break;
	}
}
//---------------------------------------------------------------------------
