//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
#include "my.h"
#include <Clipbrd.hpp>
#include "gapoTT.h"
#include "gapoV.h"
#include "gapoR.h"
#include "unit2.h"
#include "unitX.h"
#include "poznamky.h"
#include "parametry_linky.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TmGrid *mGrid;
//---------------------------------------------------------------------------
//konstruktor
TmGrid::TmGrid(TForm *Owner)
{
	Form=Owner;
	////nastavení TABULKY
	Tag=0;//ID komponenty (vyuitelné napø. pokud bude více tabulek, tak se bude vìdìt, v jaké došlo ke kliku)
	Left=0;Top=0;//umístìní celé komponenty
	RowCount=0;ColCount=0;//poèet øádkù a sloupcù
	DefaultColWidth=90,DefaultRowHeight=25;//vıchozí vıška a šíøka øádku
	Row=0;Col=0;//aktuální øádek a sloupec
	AntiAliasing_grid=false;
	AntiAliasing_text=true;
	MovingTable=false;
	SetColumnAutoFitColIdx=-3;//nastaví šíøku bunìk daného sloupce dle parametru ColIdx, -3 = nepøizpùsobuje se velikost a uije se defaultColWidth,-2 všechny sloupce stejnì podle nejširšího textu, -1 pøizpùsobuje se kadı sloupec individuálnì, 0 a více jen konkrétní sloupec uvedenı pomoc ColIdx
	preRowInd=-1;
	Decimal=3;//implicitní poèet desetinnıch míst u numericeditù
	//orámování - default
	TBorder defBorder;
	defBorder.Color=(TColor)RGB(200,200,200);
	defBorder.Width=1;
	defBorder.Style=psSolid;
	Border=defBorder;Border.Width=2;

	////nastavení defalní BUÒKY
	//vytvoøí novou buòku (alokuje ji pamì)
	CreateCell(DefaultCell);

	//typ
	DefaultCell.Type = DRAW;//defaultní komponenta
	//text
	DefaultCell.Font->Size=12;
	DefaultCell.Font->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.Font->Orientation=0;
	DefaultCell.Font->Style=TFontStyles();
	DefaultCell.Font->Pitch=TFontPitch::fpVariable;//kadé písmeno fontu stejnì široké
	DefaultCell.Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.Font->Name="Arial";
	DefaultCell.isNegativeNumber->Size=DefaultCell.Font->Size;
	DefaultCell.isNegativeNumber->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.isNegativeNumber->Orientation=0;
	DefaultCell.isNegativeNumber->Style=TFontStyles();
	DefaultCell.isNegativeNumber->Pitch=TFontPitch::fpVariable;//kadé písmeno fontu stejnì široké
	DefaultCell.isNegativeNumber->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isNegativeNumber->Name=DefaultCell.Font->Name;
	*DefaultCell.isNegativeNumber=*DefaultCell.Font;
	*DefaultCell.isZero=*DefaultCell.Font;
	*DefaultCell.isLink=*DefaultCell.Font;
	DefaultCell.isLink->Size=DefaultCell.Font->Size;
	DefaultCell.isLink->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.isLink->Orientation=0;
	DefaultCell.isLink->Style=TFontStyles();
	DefaultCell.isLink->Pitch=TFontPitch::fpVariable;//kadé písmeno fontu stejnì široké
	DefaultCell.isLink->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isLink->Name=DefaultCell.Font->Name;
	DefaultCell.TextPositon.X=0;
	DefaultCell.TextPositon.Y=0;
	DefaultCell.Text="";
	//pozice pøípadného linku
	DefaultCell.LinkCoordinateStart=TPoint(-1,-1);//kvùli uloení citelné oblasti pro link dané buòky
	DefaultCell.LinkCoordinateEnd=TPoint(-1,-1);//kvùli uloení citelné oblasti pro link dané buòky
	//zarovnání
	DefaultCell.Align=CENTER;
	DefaultCell.Valign=MIDDLE;
	DefaultCell.TopMargin=1;
	DefaultCell.BottomMargin=1;
	DefaultCell.LeftMargin=1;
	DefaultCell.RightMargin=1;
	//pouze indikuje, zda je buòka slouèena, èi nikoliv, slouí jako pomùcka pøi vykreslování orámování slouèenıch bunìk
	DefaultCell.MergeState=false;
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
}
//---------------------------------------------------------------------------
//destruktor, probíhá pøi ukonèování programu, tj. zváit zda není pozdì
TmGrid::~TmGrid()
{
	Delete();
}
//---------------------------------------------------------------------------
//vytvoøí tabulku
void TmGrid::Create()
{
	//alokace dvourozmerneho dynamickeho pole
	Cells = new TCells*[ColCount];//vytvoøí dynamické pole ukazatelu typu TCells dle poètu poadovanıch sloupcù
	for(unsigned long X=0;X<ColCount;X++)Cells[X] = new TCells[RowCount];//pruchod po sloupcich, slupcùm dynamamického pole alokuje pamìt pro jednotlivé øádky- cyklus musí bıt samostatnì
	//alokace jednorozmìrného dynamického pole
	Columns = new TColumns[ColCount];
	Rows = new TRows[RowCount];
	//smazání + nastavení hodnot musí bıt v extra cyklu
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			////BUÒKY
			//Cells[X][Y]=DefaultCell; //- nelze, takto pøevezme celı ukazatel a CreateCell(Cells[X][Y]); alokovalo Border znovu
			//alokace pamìti
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();
			Cells[X][Y].isNegativeNumber=new TFont();
			Cells[X][Y].isZero=new TFont();
			Cells[X][Y].isLink=new TFont();
			Cells[X][Y].isEmpty=new TBrush();
			CopyAreaCell(DefaultCell,Cells[X][Y],true);//kopie vlastností, bez orámování
			////orámování bunìk ukazatelem
			CreateLinkBorder(X,Y,DefaultCell);
			////ØÁDKY
			Rows[Y].Height=DefaultRowHeight;
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Y*DefaultRowHeight;else Rows[0].Top=0;
		}
		////SLOUPCE
		Columns[X].Width=DefaultColWidth;
		if(X>0){Columns[X].Left=Columns[X-1].Left+X*DefaultColWidth;}else Columns[0].Left=0;
	}
	bufColCount=ColCount;bufRowCount=RowCount;//urèeno pøi další realokaci pole
	preTop=Top;preLeft=Left;//zaloha úvodní pozice
	deleteMark=false;//detekce e dochází k odstraòování mGridu
}
//---------------------------------------------------------------------------
//pøetíená metoda - vytvoøí tabulku s pøedepsanım poètem sloupcù a øádkù
void TmGrid::Create(unsigned long ColCount,unsigned long RowCount)
{
	rcc(ColCount,RowCount);//pouze obejití lokální promìnné, v c++ je na to nìjaké klíèové slovo, ale nevzpomenu si
	Create();
}
//---------------------------------------------------------------------------
//pouze obejití lokální promìnné, v c++ je na to nìjaké klíèové slovo, ale nevzpomenu si
void TmGrid::rcc(unsigned long cc,unsigned long rc)
{
	ColCount=cc;RowCount=rc;
}
//---------------------------------------------------------------------------
//patøiènì prolinkuje orámování, e sousední orámování má ukazatel na totonı objekt, vzor orámvání získá dle refCell
void TmGrid::CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell)
{
	//top (pøebírá, propojuje se s horním, nejedná-li se o první)
	if(Y==0){Cells[X][Y].TopBorder=new TBorder;*Cells[X][Y].TopBorder=*refCell.TopBorder;}
	else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
	//botom
	Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*refCell.BottomBorder;
	//left (pøebírá, propojuje se s levım, nejedná-li se o první)
	if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*refCell.LeftBorder;}
	else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
	//right
	Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*refCell.RightBorder;
}
//patøiènì prolinkuje orámování, e sousední orámování má ukazatel na totonı objekt, vzor orámvání získá dle refCell
void TmGrid::CreateLinkBorder2(unsigned long X,unsigned long Y,TCells &refCell)
{
	//top (pøebírá, propojuje se s horním, nejedná-li se o první)
	try{
	//if(refCell.TopBorder==NULL)ShowMessage("null");
	if(Y==0){Cells[X][Y].TopBorder=new TBorder;/**Cells[X][Y].TopBorder=*refCell.TopBorder;*/}
	else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
	//botom
	Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*refCell.BottomBorder;
	//left (pøebírá, propojuje se s levım, nejedná-li se o první)
	if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*refCell.LeftBorder;}
	else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
	//right
	Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*refCell.RightBorder;
	}catch(...)
	{
			ShowMessage(Y);
	}
}
//---------------------------------------------------------------------------
//vytvoøí novou buòku (alokuje ji pamì)
void TmGrid::CreateCell(TCells &NewCell)
{
	NewCell.Font=new TFont();
	NewCell.isNegativeNumber=new TFont();
	NewCell.isZero=new TFont();
	NewCell.isLink=new TFont();
	NewCell.Background=new TBrush();
	NewCell.isEmpty=new TBrush();
	NewCell.TopBorder=new TBorder;
	NewCell.BottomBorder=new TBorder;
	NewCell.LeftBorder=new TBorder;
	NewCell.RightBorder=new TBorder;
}
//---------------------------------------------------------------------------
//smae buòku z pamìti
void TmGrid::DeleteCell(TCells &DelCell)
{
	DelCell.TopBorder=NULL;			delete DelCell.TopBorder;
	DelCell.BottomBorder=NULL;  delete DelCell.BottomBorder;
	DelCell.LeftBorder=NULL;    delete DelCell.LeftBorder;
	DelCell.RightBorder=NULL;   delete DelCell.RightBorder;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//odstraní tabulku, pøidruené komponenty a ukazatel na mGrid z pamìti
void TmGrid::Delete()
{
	try
	{
		deleteMark=true;//detekce e dochází k odstraòování mGridu
		//odstranìní v tabulce pouitıch komponent
		DeleteComponents();
		//uvolnìní pamìti
		DeleteTable();
		DeleteCell(DefaultCell);
		mGrid=NULL; delete mGrid;
	}
	catch(...)
	{/*MessageBeep(0);*/}
}
//---------------------------------------------------------------------------
//odstraní pouze tabulku, pomocná metoda vıše uvedené a destruktoru
void TmGrid::DeleteTable()
{
	for(unsigned long X=0;X<ColCount;X++)//po øádcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
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
//zajistí vykreslení celé tabulky
void TmGrid::Show(TCanvas *Canvas)
{
	//if(mGrid!=NULL)
	{
		if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k poadavku na zmìnu poètu øádkù a sloupcù volá se realokace
		{
			realock();
			Form->Refresh();
		}

		//nastavení šíøky sloupcù a vıšky øádkù+autofit sloupcù nastaví Columns[X].ColWidth
		SetColRow();//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøeby

		//ošetøení proti situaci AntiAliasing_text=true; a AntiAliasing_grid=false, která nemùe nastat resp. byla by zbyteèná
		if(AntiAliasing_text==false)AntiAliasing_grid=false;

		if(Canvas==NULL)Canvas=Form->Canvas;//pokud Canvas není definován, je pøedpokládáno kreslení pøímo do Form, kde je mGrid zobrazován

		if(AntiAliasing_grid==false && AntiAliasing_text==false)Draw(Canvas);
		else
		{
			Cantialising a;
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=Width*3;bmp_in->Height=Height*3;//velikost canvasu//*3 vyplıvá z logiky algoritmu antialiasingu
			//bmp_in->Canvas->Brush->Color=clRed;
			//bmp_in->Canvas->FillRect(TRect(0,0,bmp_in->Width,bmp_in->Height));
			Draw(bmp_in->Canvas);
			Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvùli smazání bitmapy vracené AA
			Canvas->Draw(Left,Top,bmp_out);
			if(AntiAliasing_grid==false && AntiAliasing_text==true)DrawGrid(Canvas);//kreslí se a nahoru
			delete (bmp_out);//velice nutné
			delete (bmp_in);//velice nutné
		}
		//zaloha úvodní pozice
		preTop=Top;preLeft=Left;
	}
}
//---------------------------------------------------------------------------
//zajistí vyvolání pøekreslení celé tabulky
void TmGrid::Refresh()
{
	 //Form->Repaint();// s pøeblikem, ale lépe pouívat pøímo ve v daném formuláøi FormPaint(this), co zajistí pøekreslení bez probliku
	 Show();
}
//---------------------------------------------------------------------------
//zajistí vykreslení celé tabulky vèetnì gridu, pokud
void TmGrid::Draw(TCanvas *C)
{
	//////TABULKA - orámování celé, pouze nastavuje jednotlivım buòkám jejich orámování
	if(Border.Width>0)//pokud je border poadován
	{
		TCells RefCell;CreateCell(RefCell);
		*RefCell.TopBorder=*RefCell.LeftBorder=*RefCell.RightBorder=*RefCell.BottomBorder=Border;
		SetRegion(RefCell,0,0,ColCount-1,RowCount-1);
		DeleteCell(RefCell);
	}

	//////BUÒKY jednotlivé
	short Zoom_g=1; if(AntiAliasing_grid)Zoom_g=3;
	short Zoom_b=1; if(AntiAliasing_text)Zoom_b=3;
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		////oblast buòky
		TRect R;//grid
		TRect Rt;//text
		TRect Rb;//text
		TRect Rc;//componenty
		R.Left	=	Left+Columns[X].Left*Zoom_g;
		Rt.Left	=	Columns[X].Left*Zoom_b;//zde není Left celé tabulky, protoe se pozicije na pozici levého horního rohu tabulky celá bmp, take zde musí bıt pouze souøadnice v rámci tabulku, nikoliv absolutnì v celém formu
		Rb.Left	=	Columns[X].Left*Zoom_b;//zde není Left celé tabulky, protoe se pozicije na pozici levého horního rohu tabulky celá bmp, take zde musí bıt pouze souøadnice v rámci tabulku, nikoliv absolutnì v celém formu
		Rc.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_g;
		Rt.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;//zde není Left celé tabulky, protoe se pozicije na pozici levého horního rohu tabulky celá bmp, take zde musí bıt pouze souøadnice v rámci tabulku, nikoliv absolutnì v celém formu
		Rb.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;//zde není Left celé tabulky, protoe se pozicije na pozici levého horního rohu tabulky celá bmp, take zde musí bıt pouze souøadnice v rámci tabulku, nikoliv absolutnì v celém formu
		Rc.Right	=	Left+(Columns[X].Left+Columns[X].Width);

		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			////oblast buòky
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//vıpoèet horního okraje buòky dle buòky pøedchozí
			R.Top			=	Top+Rows[Y].Top*Zoom_g;
			Rt.Top		=	Rows[Y].Top*Zoom_b;//zde není Left celé tabulky, protoe se pozicije na pozici levého horního rohu tabulky celá bmp, take zde musí bıt pouze souøadnice v rámci tabulku, nikoliv absolutnì v celém formu
			Rb.Top		=	Rows[Y].Top*Zoom_b;
			Rc.Top		=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_g;
			Rt.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;//zde není Left celé tabulky, protoe se pozicije na pozici levého horního rohu tabulky celá bmp, take zde musí bıt pouze souøadnice v rámci tabulku, nikoliv absolutnì v celém formu
			Rb.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
			Rc.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

			////barva pozadí buòky
			//28.2.provizorní fix if(Cells[X][Y].Text=="")C->Brush->Color=Cells[X][Y].isEmpty->Color;else //podmínìné formátování
			C->Brush->Color=Cells[X][Y].Background->Color;//pro aktivaci podmínìného formátování pøidat do else

			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(Rb);

			////komponenta v buòce
			SetComponents(C,Rc,Rt,X,Y,Cells[X][Y]);

			////orámování buòky
			//pouívám duplicitnì (k DrawGrid) zde, kvùli akceleraci v pøípadì totálnì vypnutého AA nebo totálnì zapnutého AA, v takovém pøípadì potom nebìí DrawGrid, mohl bych ho sice volat zde, ale chci si ušetøit opakovanı prùchod cykly, DrawGrid bìí jenom v momentu AntiAliasing_text=false a AntiAliasing_grid=true
			if(!(AntiAliasing_grid==false && AntiAliasing_text==true))
			{
				//top
				if(Cells[X][Y].TopBorder->Color!=Cells[X][Y].Background->Color)//kvùli slouèenım buòkám
				{
					SetBorder(C,Cells[X][Y].TopBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
				}
				//bottom
				if(Y==RowCount-1)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
				{
					SetBorder(C,Cells[X][Y].BottomBorder);
					C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
				}
				//left
				if(Cells[X][Y].LeftBorder->Color!=Cells[X][Y].Background->Color)//kvùli slouèenım buòkám
				{
					SetBorder(C,Cells[X][Y].LeftBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
				}
				//right
				if(X==ColCount-1)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
				{
					SetBorder(C,Cells[X][Y].RightBorder);
					C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
//zajistí vykreslení jen gridu
void TmGrid::DrawGrid(TCanvas *C)
{
	TRect R;//grid
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		R.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width);
		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			R.Top			=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);
			////orámování buòky
			//top
			if(Cells[X][Y].TopBorder->Color!=Cells[X][Y].Background->Color)//kvùli slouèenım buòkám
			{
				SetBorder(C,Cells[X][Y].TopBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
			}
			//bottom
			if(Y==RowCount-1)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
			{
				SetBorder(C,Cells[X][Y].BottomBorder);
				C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
			}
			//left
			if(Cells[X][Y].LeftBorder->Color!=Cells[X][Y].Background->Color)//kvùli slouèenım buòkám
			{
				SetBorder(C,Cells[X][Y].LeftBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
			}
			//right
			if(X==ColCount-1)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
			{
				SetBorder(C,Cells[X][Y].RightBorder);
				C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
			}
		}
	}
}
//---------------------------------------------------------------------------
//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøeby
void TmGrid::SetColRow()
{
	executeColumnsAutoFit(Form->Canvas);

	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		if(X>0)Columns[X].Left=Columns[X-1].Left+Columns[X-1].Width;else Columns[0].Left=0;//vıpoèet levého okraje buòky dle buòky pøedchozí
	}
	for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
	{
		if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//vıpoèet horního okraje buòky dle buòky pøedchozí
	}

	////vıpoèet celkové šíøky a vıšky tabulky
	Width=getWidth();//zároveò nastavuje celkovou hodnotu Width, musí bıt a tady!!!, protoe vıše se to teprvé nastavuje
	Height=getHeight();//zároveò nastavuje celkovou hodnotu Height, musí bıt a tady!!!, protoe vıše se to teprvé nastavuje
}
//---------------------------------------------------------------------------
//nastaví grafické pero na poadované parametry
void TmGrid::SetBorder(TCanvas *C,TBorder *Border)
{
	DeleteObject(C->Pen->Handle);//zruší pùvodní pero       //PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE
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
//nastaví danou buòku dle typu
void TmGrid::SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell)
{
	switch(Cell.Type)
	{
		case DRAW:
		{   //Cell.Text=getTag(X,Y);//provizorní vıpis pokud chci oèíslovat jednotlivé buòky
			short Zoom=1;if(AntiAliasing_text)Zoom=3;
			//nastavení fontu
			Canv->Font=Cell.Font;
			/* //28.2.provizorní fix
			int Orientation=Cell.Font->Orientation;
			if(F->m.null(F->ms.MyToDouble(Cell.Text))<0)Canv->Font=Cell.isNegativeNumber;//podmínìné formátování
			Cell.isZero->Color=clYellow;
			if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))Canv->Font=Cell.isZero;//podmínìné formátování
			Canv->Font->Orientation=Orientation;//musí ještì vrátit orientaci pokud byla podmínìnım formátováním pøepsána
			*/
			Canv->Font->Size*=Zoom;
			//SetBkMode(canv->Handle,OPAQUE);//nastavení netransparentního pozadí
			//28.2.provizorní fix if(Cell.Text=="")Canv->Brush->Color=Cell.isEmpty->Color;else //podmínìné formátování//zde se asi nezohledòuje, spíše v drawgrid, ale otázka je jak bez AA
			Canv->Brush->Color=Cell.Background->Color;//pro podmínìné formátování zaøadit do vıše uvedené else vìtve
			Canv->Brush->Style=bsClear;//nastvení netransparentního pozadí
			//zarovnání
			//samotnı vıpis
			long L=Rt.Left,T=Rt.Top;
			int W=getWidthHeightText(Cell).X*Zoom;
			int H=getWidthHeightText(Cell).Y*Zoom;
			short Rot=1;//slouí jako pomùcka rotace
			if(Cell.Font->Orientation==900){Rot=-1;H=0;if(Cell.Valign==MIDDLE)H=-getWidthHeightText(Cell).Y;}
			if(Cell.Font->Orientation==2700){Rot=-1;W=0;if(Cell.Align==LEFT || Cell.Align==CENTER)W=-W;H=0;if(Cell.Valign==MIDDLE)H=getWidthHeightText(Cell).Y;}
			switch(Cell.Align)
			{
				case aNO:   L=Rt.Left+Cell.TextPositon.X*Zoom+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2*Zoom;break;
				case LEFT:	L=Rt.Left+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2*Zoom;break;
				case CENTER:L=(Rt.Left+Rt.Right)/2-W/2;break;
				case RIGHT:	L=Rt.Right-W-Cell.RightMargin*Zoom-Cells[X][Y].RightBorder->Width/2*Zoom;if(Cell.Font->Orientation==2700)L-=H;break;
			}
			switch(Cell.Valign)
			{
				case vNO:
				{
					T=Rt.Top+Cell.TextPositon.Y*Zoom;
					if(Cell.Font->Orientation==0)T+=Cell.TopMargin*Zoom+Cells[X][Y].TopBorder->Width/2*Zoom;
					else T-=(Cell.BottomMargin*Zoom+Cells[X][Y].BottomBorder->Width/2*Zoom);
				}break;
				case TOP:		T=Rt.Top+Cell.TopMargin*Zoom+Cells[X][Y].TopBorder->Width/2*Zoom;break;
				case MIDDLE:T=(Rt.Top+Rt.Bottom)/2-H/2;break;
				case BOTTOM:T=Rt.Bottom-H-Cell.BottomMargin*Zoom-Cells[X][Y].BottomBorder->Width/2*Zoom;break;
			}
			unsigned int Pos=Cell.Text.Pos("<a>");//pozice html tagu
			if(Pos>0)//parsování HTML
			{
				AnsiString T1=ms.TrimRightFrom(Cell.Text,"<a>");//AnsiString T1=Cell.Text.SubString(1,Pos-1);
				AnsiString Link=ms.EP(Cell.Text,"<a>","</a>");
				AnsiString T2=ms.TrimLeftFromText(Cell.Text,"</a>");
				Canv->TextOut(L,T,T1);
				short w=Canv->TextWidth(T1);
				Canv->Font=Cell.isLink;
				Canv->Font->Size*=Zoom;
				//Canv->Font->Color=Cell.isLink->Color;zaloha, døíve kdyby vıše uvedené nefungovalo správnì, mono smazat
				Canv->TextOut(L+w,T,Link);
				Cell.LinkCoordinateStart.x=Left+L/Zoom+w/Zoom;//kvùli citelné oblasti pro link dané buòky
				Cell.LinkCoordinateStart.y=Top+T/Zoom;//kvùli citelné oblasti pro link dané buòky
				Cell.LinkCoordinateEnd.y=Top+T/Zoom+Canv->TextHeight(Link)/Zoom;//kvùli citelné oblasti pro link dané buòky
				w+=Canv->TextWidth(Link);
				Cell.LinkCoordinateEnd.x=Left+L/Zoom+w/Zoom;//kvùli citelné oblasti pro link dané buòky
				Canv->Font=Cell.Font;
				Canv->Font->Size*=Zoom;
				Canv->TextOut(L+w,T,T2);
			}
			else //bez odkazu
			Canv->TextOut(L,T,Cell.Text);
		}break;
		case readEDIT:
		{
			SetEdit(R,X,Y,Cell);
		}break;
		case EDIT:
		{
			SetEdit(R,X,Y,Cell);
			if(MovingTable)
			{
				Ttype TypeTemp=Cell.Type;Cell.Type=DRAW;//nastaví jinı typ, ale jen provizornì
				SetComponents(Canv,R,Rt,X,Y,Cell);
				Cell.Type=TypeTemp;//navrácení pøedchozího typu
			}
		}break;
		case readNUMERIC:
		{
			SetNumeric(R,X,Y,Cell);
		}break;
		case NUMERIC:
		{
			SetNumeric(R,X,Y,Cell);
			if(MovingTable)
			{
				Ttype TypeTemp=Cell.Type;Cell.Type=DRAW;//nastaví jinı typ, ale jen provizornì
				SetComponents(Canv,R,Rt,X,Y,Cell);
				Cell.Type=TypeTemp;//navrácení pøedchozího typu
			}
		}break;
		case LABEL:
		{
			SetLabel(R,X,Y,Cell);
		}break;
		case BUTTON:
		{
			//zaloení + tag + název
			TscGPButton *B=createButton(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
			//atributy
			B->Top=R.Top+floor(Cell.TopBorder->Width/2.0)+1;
			B->Left=R.Left+floor(Cell.LeftBorder->Width/2.0)+1;
			B->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-floor(Cell.LeftBorder->Width/2.0)-1;
			B->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-floor(Cell.TopBorder->Width/2.0)-1;
//			B->Options->NormalColor=Cell.Background->Color; nechat
			B->Options->FrameNormalColor=B->Options->NormalColor;
			B->Font=Cell.Font;
			B->Caption=Cell.Text;
			//vlastník
			B->Parent=Form;//musí bıt a na konci
			B=NULL;delete B;
		}break;
		case glyphBUTTON:
		{
			//zaloení + tag + název
			TscGPGlyphButton *gB=createGlyphButton(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
			//atributy
			gB->Top=R.Top+floor(Cell.TopBorder->Width/2.0)+1;
			gB->Left=R.Left+floor(Cell.LeftBorder->Width/2.0)+1;
			gB->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-floor(Cell.LeftBorder->Width/2.0)-1;
			gB->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-floor(Cell.TopBorder->Width/2.0)-1;
//			gB->Options->NormalColor=Cell.Background->Color; nechat
			gB->Options->FrameNormalColor=gB->Options->NormalColor;
			gB->Font=Cell.Font;
			gB->Caption=Cell.Text;
			//vlastník
			gB->Parent=Form;//musí bıt a na konci
			gB=NULL;delete gB;
		}break;
		case COMBO:
		{
			//zaloení + tag + název
			TscGPComboBox *C=getCombo(X,Y);//pokud ji existuje,vrátí ukazatel
			if(C==NULL)//pokud ne
			{
				C=new TscGPComboBox(Form);//zaloí
				C->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
				C->Name="mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(C->Tag);
				//události
				C->OnClick=&getTagOnClick;
				C->OnEnter=&getTagOnEnter;
				C->OnChange=&getTagOnChange;
			}
			//atributy
			C->Top=R.Top+floor(Cell.TopBorder->Width/2.0);
			C->Left=R.Left+floor(Cell.LeftBorder->Width/2.0);
			C->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-floor(Cell.LeftBorder->Width/2.0);
			C->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-floor(Cell.TopBorder->Width/2.0);
			C->Options->NormalColor=Cell.Background->Color;
			C->Options->NormalColorAlpha=255;
//			C->Options->FrameNormalColor=clWhite;
//			C->Options->FrameNormalColorAlpha=255;
			C->Font=Cell.Font;
//			C->Items->Clear();//smazání pøípadného pùvodního obsahu
//			TscGPListBoxItem *t=C->Items->Add();t->Caption=Cell.Text;
//			t=NULL;delete t;
//			C->ItemIndex=1;//nelze pøedoznaèní první poloku
			//vlastník
			C->Parent=Form;//musí bıt a na konci
			C=NULL;delete C;
		}break;
		case CHECK:
		{
			//zaloení + tag + název
			TscGPCheckBox *Ch = createCheck(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
			//atributy
			switch(Cell.Align)
			{
				case aNO:		Ch->Left+=Left-preLeft;break;
				case LEFT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;break;
				case CENTER:Ch->Width=Ch->OptionsChecked->ShapeSize;Ch->Left=R.Left+Columns[X].Width/2-Ch->Width/2;break;
				case RIGHT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;Ch->BiDiMode=bdRightToLeft;break;
			}
			switch(Cell.Valign)
			{
				case aNO:		Ch->Top+=Top-preTop;break;
				case TOP:		Ch->Top=R.Top+1;Ch->Height=Ch->OptionsChecked->ShapeSize;break;
				case MIDDLE:Ch->Top=R.Top+1;Ch->Height=Rows[Y].Height-2;break;
				case BOTTOM:Ch->Height=Ch->OptionsChecked->ShapeSize;Ch->Top=R.Top+Rows[Y].Height-Ch->Height;break;
			}
			Ch->Options->NormalColor=Cell.Background->Color;
			Ch->Options->NormalColorAlpha=255;
			//nepouívat Ch->Options->FrameNormalColor=(TColor)RGB(128,128,128);//Ch->Font->Color;
			//nepouívat Ch->Options->FrameNormalColorAlpha=255;
			Ch->Font=Cell.Font;
			Ch->Caption=Cell.Text;
			//vlastník
			Ch->Parent=Form;//musí bıt a na konci
			Ch=NULL;delete Ch;
		}break;
		case RADIO:
		{
			//zaloení + tag + název
			TscGPRadioButton *Ra = createRadio(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
			//atributy
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
//			Ra->Options->NormalColor=clWhite;Cell.Background->Color;
//			Ra->Options->NormalColorAlpha=255;
			//Ra->Options->FrameNormalColor=clWhite;
			//Ra->Options->FrameNormalColorAlpha=255;
			Ra->Font=Cell.Font;
			Ra->Caption=Cell.Text;
			//vlastník
			Ra->Parent=Form;//musí bıt a na konci
			Ra=NULL;delete Ra;
		}break;
	}
}
//---------------------------------------------------------------------------
//nastaví danou buòku na edit, pomocná metoda vıše uvedené
void TmGrid::SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zaloení + tag + název
	TscGPEdit *E=createEdit(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	//atributy
	if(Cell.Type==EDIT)E->Enabled=true;else E->Enabled=false;
	E->Visible=!MovingTable;//pøi posunu tabulky se skryje EDIT a je místo nìj DRAW
	E->AutoSize=false;
	E->Top=R.Top+Cell.TopBorder->Width;//ubere velikost komponenty podle šíøky orámování
	E->Left=R.Left+Cell.LeftBorder->Width;//ubere velikost komponenty podle šíøky orámování
	if(Cell.MergeState==false)E->Width=Columns[X].Width-Cell.RightBorder->Width;//ubere velikost komponenty podle šíøky orámování
	/*if(Cell.MergeState==false)*/E->Height=Rows[Y].Height-Cell.BottomBorder->Width;//ubere velikost komponenty podle šíøky orámování
	E->ShowHint=false;//implicitnì u editu na false, pokus pro dlouhı textif(Cell.Text.Length()>E->Width/(Cell.Font->Size-2))E->ShowHint=true;else //asi nepøesné
	E->Hint=Cell.Text;//vıchozí text pro hint je hodnota z editu
	if(Cell.Text=="")E->Options->NormalColor=Cell.isEmpty->Color;else E->Options->NormalColor=Cell.Background->Color;
	E->Options->NormalColorAlpha=255;
	E->Options->FrameNormalColor=Cell.Background->Color;
	E->Options->FrameNormalColorAlpha=255;
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
	E->Font=Cell.Font;
	//28.2.provizorní fix if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))E->Font=Cell.isNegativeNumber;//podmínìné formátování
	//28.2.provizorní fix if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))E->Font=Cell.isZero;//podmínìné formátování
	if(!E->Focused())//pokud není na buòce focus resp. není aktivní
	E->Text=Cell.Text;
	//vlastník
	E->Parent=Form;//musí bıt a na konci
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//nastaví danou buòku na numericedit, pomocná metoda objednu vıše uvedené
void TmGrid::SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zaloení + tag + název
	TscGPNumericEdit *N=createNumeric(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	//atributy
	if(Cell.Type==NUMERIC)N->Enabled=true;else N->Enabled=false;
	N->Visible=!MovingTable;//pøi posunu tabulky se skryje NUMERIC a je místo nìj DRAW
	N->AutoSize=false;
	N->Top=R.Top+Cell.TopBorder->Width;//ubere velikost komponenty podle šíøky orámování
	N->Left=R.Left+Cell.LeftBorder->Width;//ubere velikost komponenty podle šíøky orámování
	if(Cell.MergeState==false)N->Width=Columns[X].Width-Cell.RightBorder->Width;//ubere velikost komponenty podle šíøky orámování
	/*if(Cell.MergeState==false)*/N->Height=Rows[Y].Height-Cell.BottomBorder->Width;//ubere velikost komponenty podle šíøky orámování
	N->Decimal=Decimal;
	N->DisplayType=scedtNumeric;
	N->ValueType=scvtFloat;
	N->ShowHint=true;
	N->Hint=Cell.Text;
	if(Cell.Text=="")N->Options->NormalColor=Cell.isEmpty->Color;else N->Options->NormalColor=Cell.Background->Color;
	N->Options->NormalColorAlpha=255;
	N->Options->FrameNormalColor=Cell.Background->Color;
	N->Options->FrameNormalColorAlpha=255;
	N->Options->FrameDisabledColor=N->Options->DisabledColor;
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
	N->Font=Cell.Font;
	//28.2.provizorní fix if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))N->Font=Cell.isNegativeNumber;//podmínìné formátování
	//28.2.provizorní fix if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))N->Font=Cell.isZero;//podmínìné formátování
	if(!N->Focused())//pokud je na buòce focus resp. je aktivní
	{
		N->Value=ms.MyToDouble(Cell.Text);
	}
	//vlastník
	N->Parent=Form;//musí bıt a na konci
}
//---------------------------------------------------------------------------
//nastaví danou buòku na numericedit, pomocná metoda objednu vıše uvedené
void TmGrid::SetLabel(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zaloení + tag + název
	TscHTMLLabel *L=createLabel(X,Y);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	//atributy
	L->AutoSizeHeight=false;L->AutoSizeWidth=false;
	L->Top=R.Top+Cell.TopBorder->Width;//ubere velikost komponenty podle šíøky orámování
	L->Left=R.Left+Cell.LeftBorder->Width-1;//ubere velikost komponenty podle šíøky orámování
	if(Cell.MergeState==false)L->Width=Columns[X].Width-Cell.RightBorder->Width;//ubere velikost komponenty podle šíøky orámování
	/*if(Cell.MergeState==false)*/L->Height=Rows[Y].Height-Cell.BottomBorder->Width;//ubere velikost komponenty podle šíøky orámování
	L->ShowHint=false;//implicitnì u editu na false, pokus pro dlouhı textif(Cell.Text.Length()>E->Width/(Cell.Font->Size-2))E->ShowHint=true;else //asi nepøesné
	L->Hint=Cell.Text;//vıchozí text pro hint je hodnota z editu
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
	L->Font=Cell.Font;
	//28.2.provizorní fix if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))L->Font=Cell.isNegativeNumber;//podmínìné formátování
	//28.2.provizorní fix if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))L->Font=Cell.isZero;//podmínìné formátování
	L->Caption=Cell.Text;
	//vlastník
	L->Parent=Form;//musí bıt a na konci
	L=NULL;delete L;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
TscGPEdit *TmGrid::createEdit(unsigned long Col,unsigned long Row)
{
	TscGPEdit *E=getEdit(Col,Row);//pokud ji existuje
	if(E==NULL)//pokud ne, tak zaloí
	{
		E = new TscGPEdit(Form);
		E->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		E->Name="mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(E->Tag);

		//události
		E->OnClick=&getTagOnClick;
		E->OnEnter=&getTagOnEnter;
		E->OnChange=&getTagOnChange;
	}
	return E;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
TscGPNumericEdit *TmGrid::createNumeric(unsigned long Col,unsigned long Row)
{
	TscGPNumericEdit *N=getNumeric(Col,Row);//pokud ji existuje
	if(N==NULL)//pokud ne
	{
		N=new TscGPNumericEdit(Form);
		N->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		N->Name="mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(N->Tag);
		//události
		N->OnClick=&getTagOnClick;
		N->OnEnter=&getTagOnEnter;
		N->OnChange=&getTagOnChange;
	}
	return N;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscHTMLLabel *TmGrid::createLabel(unsigned long Col,unsigned long Row)
{
	TscHTMLLabel *L=getLabel(Col,Row);//pokud ji existuje
	if(L==NULL)//pokud ne, tak zaloí
	{
		L = new TscHTMLLabel(Form);
		L->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		L->Name="mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(L->Tag);

		//události
		L->OnClick=&getTagOnClick;
		L->OnEnter=&getTagOnEnter;
	}
	return L;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
TscGPButton *TmGrid::createButton(unsigned long Col,unsigned long Row)
{
	TscGPButton *B=getButton(Col,Row);//pokud ji existuje
	if(B==NULL)//pokud ne, tak zaloí
	{
		B = new TscGPButton(Form);
		B->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		B->Name="mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(B->Tag);

		//události
		B->OnClick=&getTagOnClick;
		B->OnEnter=&getTagOnEnter;
	}
	return B;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
TscGPGlyphButton *TmGrid::createGlyphButton(unsigned long Col,unsigned long Row)
{
	TscGPGlyphButton *gB=getGlyphButton(Col,Row);//pokud ji existuje
	if(gB==NULL)//pokud ne, tak zaloí
	{
		gB = new TscGPGlyphButton(Form);
		gB->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		gB->Name="mGrid_GlyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(gB->Tag);

		//události
		gB->OnClick=&getTagOnClick;
		gB->OnEnter=&getTagOnEnter;
	}
	return gB;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
TscGPCheckBox *TmGrid::createCheck(unsigned long Col,unsigned long Row)
{
	TscGPCheckBox *Ch=getCheck(Col,Row);//pokud ji existuje
	if(Ch==NULL)//pokud ne, tak zaloí
	{
		Ch = new TscGPCheckBox(Form);
		Ch->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		//Cell.Text=Ch->Tag; ShowMessage(Ch->Tag);
		Ch->Name="mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(Ch->Tag);

		//události
		Ch->OnClick=&getTagOnClick;
		Ch->OnEnter=&getTagOnEnter;
	}
	return Ch;
}
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
TscGPRadioButton *TmGrid::createRadio(unsigned long Col,unsigned long Row)
{
	TscGPRadioButton *Ra=getRadio(Col,Row);//pokud ji existuje
	if(Ra==NULL)//pokud ne, tak zaloí
	{
		Ra = new TscGPRadioButton(Form);
		Ra->Tag=getTag(Col,Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
		//Cell.Text=Ra->Tag; ShowMessage(Ra->Tag);
		Ra->Name="mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(Ra->Tag);

		//události
		Ra->OnClick=&getTagOnClick;
		//Ra->OnEnter=&getTagOnEnter;//asi zbyteèná událost
	}
	return Ra;
}
//---------------------------------------------------------------------------
//vratí ID tag komponenty,absolutní poøadí v pamìti
unsigned long TmGrid::getTag(unsigned long Col,unsigned long Row)
{
	return (Col+1)+Row*ColCount;
}
//---------------------------------------------------------------------------
//z tagu vratí èíslo sloupce
unsigned long TmGrid::getColFromTag(unsigned long Tag)
{
	long RET=Tag%ColCount-1;
	if(RET>=0)return RET;
	else return RET=ColCount-1;
}
//---------------------------------------------------------------------------
//z tagu vratí èíslo øádku
unsigned long TmGrid::getRowFromTag(unsigned long Tag)
{
	return ceil(Tag/(double)ColCount)-1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//PUBLIC: nastaví šíøku bunìk daného sloupce dle nejdelšího šíøky textu buòky ze sloupce uvedeného parametrem ColIdx sloupce
void TmGrid::SetColumnAutoFit(long ColIdx)
{
	SetColumnAutoFitColIdx=ColIdx;
	SetColRow();//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøeby
}
//---------------------------------------------------------------------------
//nastaví šíøku bunìk sloupcù dle šíøky textu dle zvoleného parametru
void TmGrid::executeColumnsAutoFit(TCanvas *Canv)
{
	switch(SetColumnAutoFitColIdx)
	{
		////nastavení velikosti sloupce dle ruèní nastavení šíøky daného sloupce, tj. nedìlat autofit
		case -4:
		{
		}
		break;
		////defualtní šíøka
		case -3:
		{
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=DefaultColWidth;
		}
		break;
		////všechny sloupce stejnì podle nejširšího sloupce
		case -2:
		{
			unsigned int MaxColWidth=4;//minimální rozmìr buòky pokud je nastavena na autofit a neobsahuje text
			//nalezení maxima
			for(unsigned long X=0;X<ColCount;X++)//po sloupcích
			{
				for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
				{
					unsigned int W=getWidthHeightText(Cells[X][Y]).X+Cells[X][Y].LeftMargin+Cells[X][Y].LeftBorder->Width/2+Cells[X][Y].RightMargin+Cells[X][Y].RightBorder->Width/2;
					if(Cells[X][Y].Type==CHECK || Cells[X][Y].Type==RADIO)W+=20;
					if(W>MaxColWidth)MaxColWidth=W;//najde nejšiøší
				}
			}
			//zápis do pole šíøky sloupcù
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=MaxColWidth;
		}
		break;
		////všechny sloupce zarovnat individuálnì dle kadého sloupce
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
//nastaví šíøku bunìk daného sloupce dle šíøky textu v daném sloupci
void TmGrid::executeColumnAutoFit(TCanvas *Canv,long ColIdx)
{
	Columns[ColIdx].Width=GetRecommendedColumnWidth(ColIdx);//nalezne nejširší poloku a podle toho celı sloupec i zarovná
}
//---------------------------------------------------------------------------
//vratí doporuèenou šíøku sloupce dle jeho obsahu
int TmGrid::GetRecommendedColumnWidth(long ColIdx)
{
	unsigned int ColWidth=getWidthHeightText(Cells[ColIdx][0]).X;//vıchozí hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		short Zoom=1;if(AntiAliasing_text==true)Zoom=3;
		unsigned int W=getWidthHeightText(Cells[ColIdx][Y]).X+Cells[ColIdx][Y].LeftMargin*Zoom+Cells[ColIdx][Y].LeftBorder->Width*Zoom/2+Cells[ColIdx][Y].RightMargin*Zoom+Cells[ColIdx][Y].RightBorder->Width*Zoom/2;
		if(Cells[ColIdx][Y].Type==CHECK || Cells[ColIdx][Y].Type==RADIO)W+=20+4+4;
		if(W>ColWidth)ColWidth=W;//najde nejšiøší
	}
	return ColWidth+4;//+4 pouze okrajovı offset
}
//---------------------------------------------------------------------------
//vratí doporuèenou šíøku sloupce dle dodaného textu a aktuálního nastavení canvasu
int TmGrid::GetRecommendedColumnWidth(AnsiString Text)
{
	return Form->Canvas->TextWidth(Text)+4;//+4 pouze okrajovı offset;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vrátí celkovou šíøku tabulky
unsigned long TmGrid::getWidth()
{
	return Columns[ColCount-1].Left+Columns[ColCount-1].Width;
}
//---------------------------------------------------------------------------
//vrátí celkovou vıšku tabulky
unsigned long TmGrid::getHeight()
{
	return Rows[RowCount-1].Top+Rows[RowCount-1].Height;
}
//---------------------------------------------------------------------------
//vrátí šíøku a vıšku textu buòky v pixelech
TPoint TmGrid::getWidthHeightText(TCells &Cell)
{
	TPoint RET;
	Form->Canvas->Font=Cell.Font;//nastavení fontu
	AnsiString T=Cell.Text;
	if(Cell.Text.Pos("<a>"))//pokud obsahuje odkaz, tak vyseparovat
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
//dle zadaného  èísla sloupce a èísla øádku  vrátí ukazatel nadanou komponentu
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPGlyphButton *TmGrid::getGlyphButton(unsigned long Col,unsigned long Row){return (TscGPGlyphButton *)Form->FindComponent("mGrid_GlyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));};//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscHTMLLabel *TmGrid::getLabel(unsigned long Col,unsigned long Row){return (TscHTMLLabel *)Form->FindComponent("mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));};//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vrátí z dané komponenty text do pamìové buòky, slouí napø. pøi události onchange popø. dálších
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
	if(!deleteMark)//detekce e nedochází k odstraòování mGridu, pøitom nesmí k události docházet
	{
		//ShowMessage(AnsiString("OnClick ")+IntToStr(((TComponent*)(Sender))->Tag));
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);

		if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnClick(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnClick(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnClick(Tag,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnClick(Tag,ID,Col,Row);
		if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnClick(Tag,Col,Row);
    if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnClick(Tag,ID,Col,Row);   //z unit1 do unitX
    if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnClick(Tag,ID,Col,Row);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnEnter(TObject *Sender)
{
	if(!deleteMark)//detekce e nedochází k odstraòování mGridu, pøitom nesmí k události docházet
	{
		//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
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
	if(!deleteMark)//detekce e nedochází k odstraòování mGridu, pøitom nesmí k události docházet
	{
		//ShowMessage(AnsiString("OnChange ")+IntToStr(((TComponent*)(Sender))->Tag));
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
		getTextFromComponentToMemoryCell(Col,Row);//dle zadaného èísla sloupce a èísla øádku vrátí z dané komponenty text do pamìové buòky, slouí napø. pøi události onchange popø. dálších
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
void __fastcall TmGrid::getTagOnKeyDown(TObject *Sender)
{
	if(!deleteMark)//detekce e nedochází k odstraòování mGridu, pøitom nesmí k události docházet
	{
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);  //asi zámìrnì OnChange?
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
//---------------------------------------------------------------------------
//spojí dvì buòky do jedné
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	if(ColCell_1!=ColCell_2 || RowCell_1!=RowCell_2)//pokud se jedná o slouèení jedné buòky - nelogické slouèení nic nevykoná
	{
		SetColRow();

		////kopie referenèní buòky //TCells RefCell=Cells[ColCell_1][RowCell_1];// - nelze však pouít takto na pøedávání borders, proto metoda níe, pøedávalo by i ukazatel
		TCells RefCell;CreateCell(RefCell);
		CopyCell(Cells[ColCell_1][RowCell_1],RefCell,true);

		////nastavení referenèní buòky kvùli orámování všech bunìk oblasti na totonou barvu
		TBorder B;B.Width=0;B.Style=psSolid;B.Color=Cells[ColCell_1][RowCell_1].Background->Color;
		*Cells[ColCell_1][RowCell_1].TopBorder=*Cells[ColCell_1][RowCell_1].BottomBorder=*Cells[ColCell_1][RowCell_1].LeftBorder=*Cells[ColCell_1][RowCell_1].RightBorder=B;
		Cells[ColCell_1][RowCell_1].MergeState=true;//oznaèí buòku jako slouèenou, slouí pro pøeskoèení vykreslování orámování, uprostøed slouèenıch objektù

		////projde nejdøíve všechny buòky nastaví jim prvnì dle pozadí první buòky stejné pozadí a dle barvy pozadí i barvu orámování
		SetCells(Cells[ColCell_1][RowCell_1],ColCell_1,RowCell_1,ColCell_2,RowCell_2,-1,false);

		////typ první buòky
		Cells[ColCell_1][RowCell_1].Type=RefCell.Type;

		////vytvoøí resp. pøedá orámování oblasti dle referenèní buòky, šlo by øešit v ve vıše volaném prùchodu, bylo by sice systomovì ménì nároèné, ale více komplikované na realizaci
		SetRegion(RefCell,ColCell_1,RowCell_1,ColCell_2,RowCell_2);

		////text
		int W=0;int H=0;
		Cells[ColCell_2][RowCell_2].Text=RefCell.Text;//navrácení ze zálohy do poslední buòky, protoe ta se vykresluje jako poslední
		W=getWidthHeightText(RefCell).X;
		H=getWidthHeightText(RefCell).Y;
		if(RefCell.Font->Orientation==900){H=0;if(RefCell.Valign==MIDDLE)H=-getWidthHeightText(RefCell).X;}
		if(RefCell.Font->Orientation==2700){W=0;if(RefCell.Align==LEFT || RefCell.Align==CENTER)W=-H;H=0;if(RefCell.Valign==MIDDLE)H=getWidthHeightText(RefCell).X;}
		//if(Cell.Font->Orientation==2700)L-=H;

		//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøeby - DÙLEITE pro text!!!
		Cells[ColCell_2][RowCell_2].Align=aNO;
		Cells[ColCell_2][RowCell_2].Valign=vNO;
		//zarovnání (zarovnává dle první buòky, ale pracuje s poslední, protoe ta se vykresluje zcela poslední)
		switch(Cells[ColCell_1][RowCell_1].Align)//HORIZONTÁLNÍ ZAROVNÁNÍ
		{
			 case LEFT:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_1].Left-Columns[ColCell_2].Left;//øeším v setcomponents +RefCell.LeftMargin+RefCell.LeftBorder->Width/2;
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
						case EDIT:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPEdit *E=createEdit(ColCell_1,RowCell_1);
							E->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left-Cells[ColCell_2][RowCell_2].RightBorder->Width;
							E->Text=RefCell.Text;
							E=NULL;delete E;
						}break;
						case NUMERIC:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPNumericEdit *N=createNumeric(ColCell_1,RowCell_1);
							N->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left-Cells[ColCell_2][RowCell_2].RightBorder->Width;
							N->Value=ms.MyToDouble(RefCell.Text);//bere a z poslední buòky sluèované oblasti
							N=NULL;delete N;
						}break;
						case LABEL:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscHTMLLabel *L=createLabel(ColCell_1,RowCell_1);
							L->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left-Cells[ColCell_2][RowCell_2].RightBorder->Width-1;
							L->Caption=RefCell.Text;//bere a z poslední buòky sluèované oblasti
							L=NULL;delete L;
						}break;
						default: Cells[ColCell_2][RowCell_2].TextPositon.X=(Columns[ColCell_1].Left-Columns[ColCell_2].Left+Columns[ColCell_2].Width)/2-W/2;
				 }
			 }break;
			 case RIGHT:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_2].Width-W;//øeším v setcomponents -RefCell.RightMargin-RefCell.RightBorder->Width/2;
			 }break;
		}
		switch(Cells[ColCell_1][RowCell_1].Valign)//VERTIKÁLNÍ ZAROVNÁNÍ
		{
			 case TOP:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_1].Top-Rows[RowCell_2].Top;//øeším v setcomponents +RefCell.TopMargin+RefCell.TopBorder->Width/2;
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

						default: Cells[ColCell_2][RowCell_2].TextPositon.Y=(Rows[RowCell_1].Top-Rows[RowCell_2].Top+Rows[RowCell_2].Height)/2-H/2;
				 }
			 }
			 break;
			 case BOTTOM:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_2].Height-H;//øeším v setcomponents -RefCell.BottomMargin-RefCell.BottomBorder->Width/2;
			 }
			 break;
		}
	}
}
//---------------------------------------------------------------------------
//nastaví oblast bunìk totonımi vlastnostmi dle referenèní buòky, text podle posledního parametru buï -1 -smae, 0 - zanechá pùvodní (implicitnì), 1 zkopíruje všude stejnı), zaèínat zadávat od nejvyšší a nejvíce vlevo
void TmGrid::SetCells(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,short setText,bool copyComponent)
{
	for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
	{
		for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
		{
			UnicodeString Text=Cells[X][Y].Text;//záloha textu, varianta zanechá pùvodní
			if(setText==-1)Text="";//varianta smae
			if(setText==1)Text=RefCell.Text;//varianta všude stejnı
			//Cells[X][Y]=RefCell;// - nelze však pouít na pøedávání borders, proto metoda níe
			CopyCell(RefCell,Cells[X][Y],copyComponent);
			Cells[X][Y].Text=Text;//navrácení textu
		}
	}
}
//---------------------------------------------------------------------------
void TmGrid::SetRegion(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)//totonì ohranièí danou oblast bunìk dle referenèní buòky (zohledòuje i rozdíly horní,dolní,levé pravé orámování), zaèínat zadávat od nejvyšší a nejvíce vlevo
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
void TmGrid::HighlightCell(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width)
{
//	switch(Cell[Col][Row])
//	{
//		case EDIT:
//		{
			TBorder hlBorder;
			hlBorder.Color=Color;
			hlBorder.Width=Width;
			hlBorder.Style=psSolid;
			*Cells[Col][Row].TopBorder=hlBorder;
			*Cells[Col][Row].LeftBorder=hlBorder;
			*Cells[Col][Row].RightBorder=hlBorder;
			*Cells[Col][Row].BottomBorder=hlBorder;
//		}break;
//	}
}
//---------------------------------------------------------------------------
//zkopíruje obsah, formát a orámování z buòky na buòku (bez ukazatelového propojení)
void TmGrid::CopyCell(TCells &RefCell,TCells &CopyCell,bool copyComponent)
{
	//zkopíruje obsah, formát (bez orámování) z buòky na buòku (bez ukazatelového propojení)
	CopyAreaCell(RefCell,CopyCell,copyComponent);
	//zkopíruje orámování z buòky na buòku (bez ukazatelového propojení)
	CopyBordesCell(RefCell,CopyCell);
}
//---------------------------------------------------------------------------
//zkopíruje danou oblast do schránky, buòky oddìlí separátorem
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
		T+="\n";//konec øádku
	}
	c->AsText = T;
	c = NULL; delete c;
}
//---------------------------------------------------------------------------
//zkopíruje obsah, formát (bez orámování) z buòky na buòku (bez ukazatelového propojení)
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
	//indikátor slouèení
	CopyCell.MergeState=RefCell.MergeState;
	////pozadí
	//*CopyCell.Background=*RefCell.Background;  - asi nejede
	CopyCell.Background->Color=RefCell.Background->Color;
	CopyCell.Background->Style=RefCell.Background->Style;
	CopyCell.isEmpty->Color=RefCell.isEmpty->Color;
	CopyCell.isEmpty->Style=RefCell.isEmpty->Style;
}
//---------------------------------------------------------------------------
//zkopíruje orámování z buòky na buòku (bez ukazatelového propojení)
void TmGrid::CopyBordesCell(TCells &RefCell,TCells &CopyCell)
{
	*CopyCell.TopBorder=*RefCell.TopBorder;
	*CopyCell.BottomBorder=*RefCell.BottomBorder;
	*CopyCell.LeftBorder=*RefCell.LeftBorder;
	*CopyCell.RightBorder=*RefCell.RightBorder;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//smae text v celém sloupci
void TmGrid::ClearColumn(unsigned long ColIdx)
{
	for(unsigned long Y=0;Y<=RowCount;Y++)
	{
		Cells[ColIdx][Y].Text="";
	}
}
//---------------------------------------------------------------------------
//smae text v celém øádku
void TmGrid::ClearRow(unsigned long RowIdx)
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		Cells[X][RowIdx].Text="";
	}
}
//---------------------------------------------------------------------------
//smae text celé tabulky
void TmGrid::Clear()
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		ClearColumn(X);
	}
}
//---------------------------------------------------------------------------
//pøidá øádek za poslední øádek, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z pøedchozího øádku, pokud je invalidate na true, automaticky po pøidání pøekreslí tabulku, nìkdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint pøímo za voláním metody AddRow pøimo v uitém formuláøi
void TmGrid::AddRow(bool copyComponentFromPreviousRow,bool invalidate)
{  //padá asi v realock pøí pøidávání øádkù, ale spíše na mgrid2
	//pøi realock asi neudrí vıšku øádkù

  //zvıšení celkového poètu øádkù
	RowCount++;

	//kopie komponent z nadøízeného øádku, jeli-poadováno
	if(copyComponentFromPreviousRow)
	{
		realock();//musí probìhnout pøed následujícím kodem, jinak øeší Show
		for(unsigned long X=0;X<ColCount;X++)
		{
			Cells[X][RowCount-1].Type=Cells[X][RowCount-2].Type;
			//následující není tøeba, poøeší jen vıše uvedené + následné show: createComponent(Cells[X][RowCount-1].Type,X,RowCount-1);//dle zadaného èísla sloupce a èísla øádku vytvoøenou komponentu dle Type, pokud existuje, tak se nic nedìje
		}
	}

	//pokud je poadováno pøekreslení
	if(invalidate)Show();//pøekreslení s problikem, jinak pouít pøímo ve formu formpaint a toto zakomentovat
}
//---------------------------------------------------------------------------
//pøídá øádek za øádek uvedenı dle parametru Row,, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z pøedchozího øádku, pokud je invalidate na true, automaticky po pøidání pøekreslí tabulku, nìkdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint pøímo za voláním metody InsertRow pøimo v uitém formuláøi
void TmGrid::InsertRow(long Row,bool copyComponentFromPreviousRow, bool invalidate)
{
	if(Row<RowCount-1)//pokud se nejedná o poslední øádek, tam je to zbyteèné a øeší else vìtev AddRow
	{
		RowCount++;realock();//pøidá poslední øádek
		//pøekopíruje øádek resp. buòky z øádku následujícího a pøedtím pøidá poslední øádek
		for(unsigned long Y=RowCount-1;Y>Row;Y--)
		{
			for(unsigned long X=0;X<ColCount;X++)
			{
				CopyCell(Cells[X][Y-1],Cells[X][Y],true);
			}
			Rows[Y]=Rows[Y-1];
		}
		ClearRow(Row);//vyprázní vkládanı øádek

		if(invalidate)//pokud je poadováno pøekreslení, zde je nutné celou oblast pøekreslit
		{
			//Show();netøeba
			Form->Invalidate();//tøeba
		}
	}
	else AddRow(copyComponentFromPreviousRow,invalidate);
}
//---------------------------------------------------------------------------
//smae celı øádek, pokud je invalidate na true, automaticky po pøidání pøekreslí tabulku, nìkdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint pøímo za voláním metody InsertRow pøimo v uitém formuláøi
void TmGrid::DeleteRow(long Row,bool invalidate)
{
	if(Row<=RowCount-1 && RowCount-1>0)//nelze smazat pouze jenom jeden øádek
	{ //pøekopíruje øádek resp. buòky na øádku následující a ubere poslední øádek,pokud se nejedná o jedinı øádek
		for(unsigned long Y=Row;Y<RowCount-1;Y++)
		{
			for(unsigned long X=0;X<ColCount;X++)
			{
				CopyCell(Cells[X][Y+1],Cells[X][Y],true);
			}
			Rows[Y]=Rows[Y+1];
		}
		DeleteComponents(0,RowCount-1,ColCount-1,RowCount-1);
		RowCount--;
	}
	if(invalidate)Show();//pokud je poadováno pøekreslení //pøekreslení s problikem, jinak pouít pøímo ve formu formpaint a toto zakomentovat
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajistí realokaci pole Cells dle nové velikosti
void TmGrid::realock()
{
	//kopie do záloního pole a smazání
	TCells **bufCells = new TCells*[bufColCount];//vytvoøí dynamické pole ukazatelu typu TCells dle poètu poadovanıch sloupcù
	for(unsigned long X=0;X<bufColCount;X++)
	{
		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupcùm dynamamického pole alokuje pamìt pro jednotlivé øádky- cyklus musí bıt samostatnì
		bufCells[X] = Cells[X];//zkopírování pùvodních hodnot do zálohy //takto asi nevhodné kopírovat (zùstane ukazatel), s hvìzdickou sice hodnoty ale je potøeba pøidìlit pamìt pomocí new pro TBrush TFont a TBorders
	}

	//poèty øádkù a sloupcù
	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;

	//smazání pùvodních hodnot
	//pùvodní komponenty zachovám,  smau jen ty, které nejsou ji potøeba (pøi zmenšování tabulky), ale údajnì nefunguje zcela správnì
	DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);//nová oblast a stará oblast, není potøeba ošetøovat IFem pro pøípad pøidávání (pohlídají si cykly ve vnitø algoritmu)
	ColCount=bufColCount2;RowCount=bufRowCount2;
	//toto proè ne DeleteTable(); asi nahrazuje Create, otázka však zùstává zda nìco nezùstává v pamìti

	//vytvoøení nového realokovaného pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//urèeno pøi další realokaci pole, create toti pøepisuje buf hodnoty
	Create(ColCount,RowCount);

	//zkopírování pùvodních hodnot zpìt
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je menší
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je menší
	for(unsigned long Y=0;Y<bufRowCount;Y++)
	{
		for(unsigned long X=0;X<bufColCount;X++)
		{
			//nelze celı ukazatel
			Cells[X][Y] = bufCells[X][Y];
		}
	}

	//smazání pùvodního bufCells
	for(unsigned long X=0;X<bufColCount2;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount2;Y++)//po øádcích
		{
			DeleteCell(bufCells[X][Y]);
		}
		bufCells[X]=NULL; delete bufCells[X];
	}
	bufCells=NULL; delete bufCells;
	bufColCount=ColCount;bufRowCount=RowCount;//urèeno pøi další realokaci pole
}
//---------------------------------------------------------------------------
////zajistí realokaci pole Cells dle nové velikosti
//void TmGrid::realock()
//{              //ShowMessage("0");
//	//kopie do záloního pole a smazání
//	TCells **bufCells = new TCells*[bufColCount];//vytvoøí dynamické pole ukazatelu typu TCells dle poètu poadovanıch sloupcù
//	for(unsigned long X=0;X<bufColCount;X++)
//	{
//		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupcùm dynamamického pole alokuje pamìt pro jednotlivé øádky- cyklus musí bıt samostatnì
//		//bufCells[X] = Cells[X];//zkopírování pùvodních hodnot do zálohy //takto asi nevhodné kopírovat (zùstane ukazatel), s hvìzdickou sice hodnoty ale je potøeba pøidìlit pamìt pomocí new pro TBrush TFont a TBorders
//		for(unsigned long Y=0;Y<bufRowCount;Y++)
//		{
//			//toto tu bylo:CreateCell(bufCells[X][Y]);
//			//toto tu bylo:CopyCell(Cells[X][Y],bufCells[X][Y],true);
////			//totu tu nebylo:
////			CopyAreaCell(Cells[X][Y],bufCells[X][Y],true);
////			////orámování
////			if(Y==0){bufCells[X][Y].TopBorder=new TBorder;*bufCells[X][Y].TopBorder=*Cells[X][Y].TopBorder;}
////			else bufCells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
////			//botom
////			bufCells[X][Y].BottomBorder=new TBorder;*bufCells[X][Y].BottomBorder=*Cells[X][Y].BottomBorder;
////			//left (pøebírá, propojuje se s levım, nejedná-li se o první)
////			if(X==0){bufCells[X][Y].LeftBorder=new TBorder;*bufCells[X][Y].LeftBorder=*Cells[X][Y].LeftBorder;}
////			else bufCells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
////			//right
////			bufCells[X][Y].RightBorder=new TBorder;*bufCells[X][Y].RightBorder=*Cells[X][Y].RightBorder;
//		}
//	}
//	 //ShowMessage("1");
//	//smazání pùvodních hodnot
//	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
//	ColCount=bufColCount;RowCount=bufRowCount;
//	//DeleteComponents();//toto bylo odkomentováno a níe zakomentováno DeleteComponents
//	//pokud bych pùvodní chtìl komponenty zachovat, ale nefunguje zcela správnì
//	DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);//nová oblast a stará oblast, není potøeba ošetøovat IFem pro pøípad pøidávání (pohlídají si cykly ve vnitø algoritmu)
//	ColCount=bufColCount2;RowCount=bufRowCount2;
//	//DeleteTable();
//								//ShowMessage("2");
//	//vytvoøení nového realokovaného pole
//	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//urèeno pøi další realokaci pole, create toti pøepisuje buf hodnoty
//	Create(ColCount,RowCount);
//
//	//zkopírování pùvodních hodnot zpìt
//	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je menší
//	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je menší
//
//	for(unsigned long Y=0;Y<bufRowCount;Y++)
//	{        ShowMessage(bufCells[0][Y].TopBorder->Width);
//		for(unsigned long X=0;X<bufColCount;X++)
//		{
//			//nelze celı ukazatel
//			Cells[X][Y] = bufCells[X][Y];
//			////typ
//			Cells[X][Y].Font=new TFont();
//			Cells[X][Y].Background=new TBrush();//alokace pamìti
//			CopyAreaCell(bufCells[X][Y],Cells[X][Y],true);//kopie vlastností, bez orámování
//			////orámování
//			CreateLinkBorder2(X,Y,bufCells[X][Y]);
//		}
//	}
//												//	ShowMessage("4");
//	//smazání pùvodního bufCells
//	for(unsigned long X=0;X<bufColCount2;X++)
//	{
//		for(unsigned long Y=0;Y<bufRowCount2;Y++)//po øádcích
//		{
//			DeleteCell(bufCells[X][Y]);
//		}
//		bufCells[X]=NULL; delete bufCells[X];
//	}
//	bufCells=NULL; delete bufCells;
//	bufColCount=ColCount;bufRowCount=RowCount;//urèeno pøi další realokaci pole
//}

//---------------------------------------------------------------------------
//dle zadaného èísla sloupce a èísla øádku vytvoøenou komponentu dle Type, pokud existuje, tak se nic nedìje
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
		case COMBO:break;
		case CHECK:				createCheck(Col,Row);break;
		case RADIO:				createRadio(Col,Row);break;
		case DRAW:break;
		case LABEL:				createLabel(Col,Row);break;
  }
}
//---------------------------------------------------------------------------
//odstraní dynamicky vytoøené komponenty, nutno volat pøed Delete()
void TmGrid::DeleteComponents()
{
	DeleteComponents(0,0,ColCount-1,RowCount-1);
}
//---------------------------------------------------------------------------
//odstraní dynamicky vytoøené komponenty od - do poètu sloupcù a øádkù, nutno volat pøed Delete()
// pozor odstraòovaná komponenta nesmí mít focus (jinak pamìová chyba), focus je potøeba pøi odstraòování komponent odevzdat nìjaké komponentì, která zùstává ve formu
void TmGrid::DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow)
{
	for(unsigned long X=sCol;X<=fCol;X++)//po øádcích
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
				case CHECK:{TscGPCheckBox *CH=getCheck(X,Y);CH->Free();CH=NULL;delete CH;break;} ///*CH->DisposeOf();*/ ani toto ani free pøi kliku nefungují správnì, chyba byla, e daná komponenta mìla focus, focus je potøeba pøi odstraòování komponent odevzdat nìjaké komponentì, která zùstává ve formu
				case RADIO:{TscGPRadioButton *R=getRadio(X,Y);R->Free();R=NULL;delete R;}break;
			}
		}
	}
}
//---------------------------------------------------------------------------
//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí øádek
long TmGrid::GetIdxRow(int X,int Y)
{
	long RET=-1;
	if(X>=Left && X<Left+Width && Y>=Top && Y<Top+Height)//ošetøení mimo tabulku + akcelerátor
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
//dle souøadnic ve formuláøi,kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí sloupec
long TmGrid::GetIdxColum(int X,int Y)
{
	long RET=-1;
	if(X>=Left && X<Left+Width && Y>=Top && Y<Top+Height)//ošetøení mimo tabulku + akcelerátor
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
//zajistí zvıraznìní øádkù dle èísla øádku Row
void TmGrid::HighlightRow(long Row,TColor Color,bool SelFirstRow,bool unHighlightPrevRow)
{
	//pokud se pohybuje v tabulce
	int FirstRow=0;if(!SelFirstRow)FirstRow=1;
	if(Row>=FirstRow && preRowInd!=Row)
	{
		if(unHighlightPrevRow)if(!(Row==1 && preRowInd==0))selRow(preRowInd,Color,false);//odstranìní pøedchozího oznaèení misto: Show();//Refresh s minimem probliku
		selRow(Row,Color,true);//nové oznaèení
	}
	//pøi odchodu z tabulky, aby nezùstal "viset" oznaèenı øádek
	if(unHighlightPrevRow)
	{
		if(Row<=FirstRow-1 && preRowInd>=FirstRow)selRow(preRowInd,Color,false);//odstranìní pøedchozího oznaèení misto: Show();//Refresh s minimem probliku
		preRowInd=Row;//uloení aktivního øádku pro další pouítí
  }
}
//---------------------------------------------------------------------------
//zajistí zvıraznìní øádkù, pøes kterı se pøejídí myší
void TmGrid::HighlightRowOnMouse(int X,int Y,TColor Color,bool SelFirstRow,bool unHighlightPrevRow)
{
	long Row=GetIdxRow(X,Y);
	HighlightRow(Row,Color,SelFirstRow,unHighlightPrevRow);
}
//---------------------------------------------------------------------------
//oznaèí øádek, nebo zruší oznaèení øádku dle vstupního parametru
void TmGrid::selRow(long Row,TColor Color,bool newSel)
{
	if(newSel)Form->Canvas->Pen->Mode=pmMask;else Form->Canvas->Pen->Mode=pmNotXor;
	Form->Canvas->Brush->Style=bsSolid;
	Form->Canvas->Brush->Color=Color;
	Form->Canvas->Pen->Width=0;
	Form->Canvas->Pen->Color=Color;
	short C=1-(short)newSel;

	//TPoint body[4]={TPoint(mGrid->Left-C,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height),TPoint(mGrid->Left,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height)}; //mohlo by dìlat problémy pùvodní kostra
	TPoint body[4]={TPoint(Left-C,Top+Rows[Row].Top-C),TPoint(Left+Width,Top+Rows[Row].Top-C),TPoint(Left+Width,Top+Rows[Row].Top+Rows[Row].Height),TPoint(Left,Top+Rows[Row].Top+Rows[Row].Height)};
	Form->Canvas->Polygon(body,3);
	Form->Canvas->Pen->Mode=pmCopy;
}
//---------------------------------------------------------------------------
//zajistí zvıraznìní orámování tabulky
void TmGrid::HighlightTable(TColor Color,unsigned short Size,unsigned short Offset,TPenMode PenMode)
{
	Form->Canvas->Pen->Width=Size;
	Form->Canvas->Pen->Color=Color;
	Form->Canvas->Brush->Style=bsClear;
	Form->Canvas->Pen->Mode=PenMode;
	short o=0;if(Size>1)o=1;//pouze grafické odsazení
	Form->Canvas->Rectangle(Left-Offset,Top-Offset,Left+Width+Offset+o,Top+Height+Offset+o);
}
//---------------------------------------------------------------------------
//zajistí zvıraznìní orámování tabulky, pokud se do ni vstoupí myší
void TmGrid::HighlightTableOnMouse(int X,int Y)
{
	 if(Left<=X && X<=Left+Width && Y>Top && Y<Top+Height)HighlightTable();
}
//---------------------------------------------------------------------------
//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí kladné èíslo sloupce a øádku pokud se na daném místì nachází odkaz, pokud ne, vrácené hodnoty jsou -1 a -1
TPoint TmGrid::CheckLink(int X,int Y)
{
	unsigned long IndRow=GetIdxRow(X,Y);
	unsigned long IndCol=GetIdxColum(X,Y);
	if(CheckLink(X,Y,IndCol,IndRow))return TPoint(IndCol,IndRow);//odkaz na danıch souøadnicích nalezen
	else return TPoint(-1,-1);
}
//---------------------------------------------------------------------------
//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí zda se na dané buòce a souøadnicích nachází odkaz
bool TmGrid::CheckLink(int X,int Y,unsigned long Col,unsigned long Row)
{
	if(Cells[Col][Row].LinkCoordinateStart.x<=X && X<=Cells[Col][Row].LinkCoordinateEnd.x
	&& Cells[Col][Row].LinkCoordinateStart.y<=Y && Y<=Cells[Col][Row].LinkCoordinateEnd.y)
	return true;
	else return false;
}
//---------------------------------------------------------------------------
//zajistí pøebarvení odkazu v buòce danou barvou
void  TmGrid::HighlightLink(unsigned long Col,unsigned long Row,short Intensive)
{
	Cmy m;
	Form->Canvas->Font=Cells[Col][Row].Font;
	Form->Canvas->Font->Color=m.clIntensive(Cells[Col][Row].Font->Color,Intensive);
	Form->Canvas->Brush->Style=bsClear;
	Form->Canvas->Brush->Color=clWhite;
	Form->Canvas->TextOutW(Cells[Col][Row].LinkCoordinateStart.x,Cells[Col][Row].LinkCoordinateStart.y,ms.EP(Cells[Col][Row].Text,"<a>","</a>"));
}
//---------------------------------------------------------------------------

