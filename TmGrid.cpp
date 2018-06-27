//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
#include "MyString.h"
#include "gapoR.h"
#include "gapoTT.h"
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
	AntiAliasing_text=false;
	SetColumnAutoFitColIdx=-3;//nastaví šíøku bunìk daného sloupce dle parametru ColIdx, -3 = nepøizpùsobuje se velikost a uije se defaultColWidth,-2 všechny sloupce stejnì podle nejširšího textu, -1 pøizpùsobuje se kadı sloupec individuálnì, 0 a více jen konkrétní sloupec uvedenı pomoc ColIdx
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
	//pozadí
	DefaultCell.Background->Color=clWhite;
	DefaultCell.Background->Style=bsSolid;
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
			//Cells[X][Y]=DefaultCell; //- nelze, takto pøevezme celı ukazatel
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();//alokace pamìti
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
//---------------------------------------------------------------------------
//vytvoøí novou buòku (alokuje ji pamì)
void TmGrid::CreateCell(TCells &NewCell)
{
	NewCell.Font=new TFont();
	NewCell.Background=new TBrush();
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
		//odstranìní v tabulce pouitıch komponent
		DeleteComponents();
		//uvolnìní pamìti
		DeleteTable();
		DeleteCell(DefaultCell);
		mGrid=NULL; delete mGrid;
	}
	catch(...)
	{MessageBeep(0);}
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
void TmGrid::Show()
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

		if(AntiAliasing_grid==false && AntiAliasing_text==false)Draw(Form->Canvas);
		else
		{
			Cantialising a;
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=Width*3;bmp_in->Height=Height*3;//velikost canvasu//*3 vyplıvá z logiky algoritmu antialiasingu
			Draw(bmp_in->Canvas);
			Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvùli smazání bitmapy vracené AA
			Form->Canvas->Draw(Left,Top,bmp_out);
			if(AntiAliasing_grid==false && AntiAliasing_text==true)DrawGrid(Form->Canvas);//kreslí se a nahoru
			delete (bmp_out);//velice nutné
			delete (bmp_in);//velice nutné
		}
	}
}
//---------------------------------------------------------------------------
//zajistí vyvolání pøekreslení celé tabulky s pøeblikem, ale lépe pouívat pøímo ve v daném formuláøi FormPaint(this), co zajistí pøekreslení bez probliku
void TmGrid::Refresh()
{
	 Form->Repaint();
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
		TRect Rb;//pozadí buòky + text
		TRect Rc;//componenty
		R.Left	=	Left+Columns[X].Left*Zoom_g;
		Rb.Left	=	Left+Columns[X].Left*Zoom_b;
		Rc.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_g;
		Rb.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_b;
		Rc.Right	=	Left+(Columns[X].Left+Columns[X].Width);

		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			////oblast buòky
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//vıpoèet horního okraje buòky dle buòky pøedchozí
			R.Top			=	Top+Rows[Y].Top*Zoom_g;
			Rb.Top		=	Top+Rows[Y].Top*Zoom_b;
			Rc.Top		=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_g;
			Rb.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
			Rc.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

			////barva pozadí buòky
			C->Brush->Color=Cells[X][Y].Background->Color;
			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(Rb);

			////komponenta v buòce
			SetComponents(C,Rc,Rb,X,Y,Cells[X][Y]);

			////orámování buòky
			//pouívám duplicitnì (k DrawGrid) zde, kvùli akceleraci v pøípadì totálnì vypnutého AA nebo totálnì zapnutého AA, v takovém pøípadì potom nebìí DrawGrid, mohl bych ho sice volat zde, ale chci si ušetøit opakovanı prùchod cykly, DrawGrid bìí jenom v momentu AntiAliasing_text=false a AntiAliasing_grid=true
			if(!(AntiAliasing_grid==false && AntiAliasing_text==true))
			{
				//top
				//nefunguje správnì:if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedná se o slouèenou buòku, slabá podmínka
				{
					SetBorder(C,Cells[X][Y].TopBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
				}
				//bottom
				if(Y==RowCount-1)//nefunguje správnì: && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
				{
					SetBorder(C,Cells[X][Y].BottomBorder);
					C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
				}
				//left
				//nefunguje správnì:if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedná se o slouèenou buòku, slabá podmínka
				{
					SetBorder(C,Cells[X][Y].LeftBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
				}
				//right                                 udìlat pøíznak na slouèenu buòku té nekreslit støe
				if(X==ColCount-1)//nefunguje správnì: && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
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
			//nefunguje správnì: if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedná se o slouèenou buòku, slabá podmínka
			{
				SetBorder(C,Cells[X][Y].TopBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
			}
			//bottom
			if(Y==RowCount-1)//nefunguje správnì:  && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
			{
				SetBorder(C,Cells[X][Y].BottomBorder);
				C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
			}
			//left
			//nefunguje správnì: if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedná se o slouèenou buòku, slabá podmínka
			{
				SetBorder(C,Cells[X][Y].LeftBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
			}
			//right
			if(X==ColCount-1) //nefunguje správnì: && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akcelerátor, aby se zbyteènì nevykreslovalo, pokud by bylo zbyteèné, vykreslí jenom poslední, invertní filozofie ne ukazování na stejné orámování, ale zde z dùvodu moného pøekryvu s náplní pøedchozí buòky
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
	DWORD pStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE/* PS_ENDCAP_ROUND PS_ENDCAP_SQUARE PS_ENDCAP_FLAT | PS_JOIN_BEVEL | PS_INSIDEFRAME*/;
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
			Canv->Font->Size*=Zoom;
			//SetBkMode(canv->Handle,OPAQUE);//nastvení netransparentního pozadí
			Canv->Brush->Color=Cell.Background->Color;Canv->Brush->Style=bsClear;//nastvení netransparentního pozadí
			//zarovnání
			//samotnı vıpis
			long L=Rt.Left,T=Rt.Top;              //zajimavı workaround - pøíèinì nerozumím
			int W=getWidthHeightText(Cell).X*Zoom;if(AntiAliasing_text)W/=1.3;
			int H=getWidthHeightText(Cell).Y*Zoom;
			if(Cell.Font->Orientation==900){H=0;if(Cell.Valign==MIDDLE)H=-getWidthHeightText(Cell).Y;}
			if(Cell.Font->Orientation==2700){W=0;if(Cell.Align==LEFT || Cell.Align==CENTER)W=-W;H=0;if(Cell.Valign==MIDDLE)H=getWidthHeightText(Cell).Y;}
			short WA=0;if(AntiAliasing_text)WA=1;//zajimavı workaround - pøíèinì nerozumím (proè Left*2 tomu patøiènì pomùe)
			switch(Cell.Align)
			{               //zajimavı workaround
				case aNO:   L=WA*Left*1.3+Rt.Left+Cell.TextPositon.X*Zoom+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2*Zoom;break;
				case LEFT:	L=WA*Left*1.3+Rt.Left+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2*Zoom;break;
				case CENTER:L=(Rt.Left+Rt.Right)/2-W/2;break;
				case RIGHT:	L=Rt.Right-W-Cell.RightMargin*Zoom-Cells[X][Y].RightBorder->Width/2*Zoom;if(Cell.Font->Orientation==2700)L-=H;break;
			}
			switch(Cell.Valign)
			{
				case vNO:		T=Rt.Top+Cell.TextPositon.Y*Zoom+Cell.TopMargin*Zoom+Cells[X][Y].TopBorder->Width/2*Zoom;break;
				case TOP:		T=Rt.Top+Cell.TopMargin*Zoom+Cells[X][Y].TopBorder->Width/2*Zoom;break;
				case MIDDLE:T=(Rt.Top+Rt.Bottom)/2-H/2;break;
				case BOTTOM:T=Rt.Bottom-H-Cell.BottomMargin*Zoom-Cells[X][Y].BottomBorder->Width/2*Zoom;break;
			}                                    //*2 zajimavı workaround
			if(AntiAliasing_text)Canv->TextOut(L-Left*2,T-Top,Cell.Text);
			else Canv->TextOut(L,T,Cell.Text);
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
		case BUTTON:
		{
			//zaloení + tag + název
			TscGPButton *B=getButton(X,Y);//pokud ji existuje
			if(B==NULL)
			{
				B=new TscGPButton(Form);//pokud ne
				B->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
				B->Name="mGrid_BUTTON_"+AnsiString(B->Tag);
				//události
				B->OnClick=&getTagOnClick;
			}
			//atributy
			B->Top=R.Top+1;
			B->Left=R.Left+1;
			B->Width=Columns[X].Width-1;
			B->Height=Rows[Y].Height-1;
//			B->Options->NormalColor=Cell.Background->Color;
			B->Options->FrameNormalColor=B->Options->NormalColor;
			B->Font=Cell.Font;
			B->Caption=Cell.Text;
			//vlastník
			B->Parent=Form;//musí bıt a na konci
			B=NULL;delete B;
		}break;
		case COMBO:
		{
			//zaloení + tag + název
			TscGPComboBox *C=getCombo(X,Y);//pokud ji existuje
			if(C==NULL)
			{
				C=new TscGPComboBox(Form);//pokud ne
				C->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
				C->Name="mGrid_COMBO_"+AnsiString(C->Tag);
				//události
				C->OnClick=&getTagOnClick;
				C->OnEnter=&getTagOnEnter;
				C->OnChange=&getTagOnChange;
			}
			//atributy
			C->Top=R.Top+1;
			C->Left=R.Left+1;
			C->Width=Columns[X].Width-1;
			C->Height=Rows[Y].Height-1;
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
				case aNO:break;
				case LEFT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;break;
				case CENTER:Ch->Width=Ch->OptionsChecked->ShapeSize;Ch->Left=R.Left+Columns[X].Width/2-Ch->Width/2;break;
				case RIGHT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;Ch->BiDiMode=bdRightToLeft;break;
			}
			switch(Cell.Valign)
			{
				case aNO:break;
				case TOP:		Ch->Top=R.Top+1;Ch->Height=Ch->OptionsChecked->ShapeSize;break;
				case MIDDLE:Ch->Top=R.Top+1;Ch->Height=Rows[Y].Height-2;break;
				case BOTTOM:Ch->Height=Ch->OptionsChecked->ShapeSize;Ch->Top=R.Top+Rows[Y].Height-Ch->Height;break;
			}
			Ch->Options->NormalColor=Cell.Background->Color;
			Ch->Options->NormalColorAlpha=255;
			Ch->Options->FrameNormalColor=(TColor)RGB(128,128,128);//Ch->Font->Color;
			Ch->Options->FrameNormalColorAlpha=255;
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
	TscGPEdit *E=getEdit(X,Y);//pokud ji existuje
	if(E==NULL)//pokud ne
	{
		E=new TscGPEdit(Form);
		E->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
		E->Name="mGrid_EDIT_"+AnsiString(E->Tag);
		//události
		E->OnClick=&getTagOnClick;
		E->OnEnter=&getTagOnEnter;
		E->OnChange=&getTagOnChange;
	}
	//atributy
	if(Cell.Type==EDIT)E->Enabled=true;else E->Enabled=false;
	E->AutoSize=false;
	E->Top=R.Top+1;
	E->Left=R.Left+1;
	E->Width=Columns[X].Width-1;
	E->Height=Rows[Y].Height-1;
	E->Options->NormalColor=Cell.Background->Color;
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
	switch(Cell.Valign)
	{
		case TOP:		E->ContentMarginTop=0;break;
		case MIDDLE:E->ContentMarginTop=E->Height/2-getWidthHeightText(Cell).Y/2;break;
		case BOTTOM:E->ContentMarginTop=E->Height-getWidthHeightText(Cell).Y;break;
	}
	E->Font=Cell.Font;
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
	TscGPNumericEdit *N=getNumeric(X,Y);//pokud ji existuje
	if(N==NULL)//pokud ne
	{
		N=new TscGPNumericEdit(Form);
		N->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
		N->Name="mGrid_NUMERIC_"+AnsiString(N->Tag);
		//události
		N->OnClick=&getTagOnClick;
		N->OnEnter=&getTagOnEnter;
		N->OnChange=&getTagOnChange;
	}
	//atributy
	if(Cell.Type==NUMERIC)N->Enabled=true;else N->Enabled=false;
	N->AutoSize=false;
	N->Decimal=2;
	N->Top=R.Top+1;
	N->Left=R.Left+1;
	N->Width=Columns[X].Width-1;
	N->Height=Rows[Y].Height-1;
	N->Options->NormalColor=Cell.Background->Color;
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
	switch(Cell.Valign)
	{
		case TOP:		N->ContentMarginTop=0;break;
		case MIDDLE:N->ContentMarginTop=N->Height/2-getWidthHeightText(Cell).Y/2;break;
		case BOTTOM:N->ContentMarginTop=N->Height-getWidthHeightText(Cell).Y;break;
	}
	N->Font=Cell.Font;
	TMyString ms;
	N->Value=ms.MyToDouble(Cell.Text);
	//vlastník
	N->Parent=Form;//musí bıt a na konci
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
		Ch->Name="mGrid_CHECK_"+AnsiString(Ch->Tag);

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
		Ra->Name="mGrid_RADIO_"+AnsiString(Ra->Tag);

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
		////defualt
		case -3:
		{
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=DefaultColWidth;
		}
		break;
		////všechny sloupce stejnì podle nejširšího
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
			//zapis do pole šíøky sloupcù
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=MaxColWidth;
		}
		break;
		////všechny sloupce individuálnì
		case -1:
		{
			for(unsigned long X=0;X<ColCount;X++)//po sloupcích
			executeColumnAutoFit(Canv,X);
		}
		break;
		////dle konkrétního
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
	unsigned int ColWidth=getWidthHeightText(Cells[ColIdx][0]).X;//vıchozí hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		unsigned int W=getWidthHeightText(Cells[ColIdx][Y]).X+Cells[ColIdx][Y].LeftMargin+Cells[ColIdx][Y].LeftBorder->Width/2+Cells[ColIdx][Y].RightMargin+Cells[ColIdx][Y].RightBorder->Width/2;
		if(Cells[ColIdx][Y].Type==CHECK || Cells[ColIdx][Y].Type==RADIO)W+=20+4+4;
		if(W>ColWidth)ColWidth=W;//najde nejšiøší
	}
	Columns[ColIdx].Width=ColWidth+4;//+4 pouze offset
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
	if(Cell.Font->Orientation==900 || Cell.Font->Orientation==2700)
	{
		RET.Y=Form->Canvas->TextWidth(Cell.Text);
		RET.X=Form->Canvas->TextHeight(Cell.Text);
	}
	else
	{
		RET.X=Form->Canvas->TextWidth(Cell.Text);
		RET.Y=Form->Canvas->TextHeight(Cell.Text);
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
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(getTag(Col,Row)));}
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(getTag(Col,Row)));}
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(getTag(Col,Row)));}
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(getTag(Col,Row)));};//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnClick(TObject *Sender)
{
	//ShowMessage(AnsiString("OnClick ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
	TComponent *K;
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnClick(Tag,Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnEnter(TObject *Sender)
{
	//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnEnter(Tag,Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnChange(TObject *Sender)
{
	//ShowMessage(AnsiString("OnChange ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnChange(Tag,Col,Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//spojí dvì buòky do jedné
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	SetColRow();

	////kopie referenèní buòky //TCells RefCell=Cells[ColCell_1][RowCell_1];// - nelze však pouít takto na pøedávání borders, proto metoda níe, pøedávalo by i ukazatel
	TCells RefCell;CreateCell(RefCell);
	CopyCell(Cells[ColCell_1][RowCell_1],RefCell,true);

	////nastavení referenèní buòky kvùli orámování všech bunìk oblasti na totonou barvu
	TBorder B;B.Width=0;B.Style=psSolid;B.Color=Cells[ColCell_1][RowCell_1].Background->Color;
	*Cells[ColCell_1][RowCell_1].TopBorder=*Cells[ColCell_1][RowCell_1].BottomBorder=*Cells[ColCell_1][RowCell_1].LeftBorder=*Cells[ColCell_1][RowCell_1].RightBorder=B;

	////projde nejdøíve všechny buòky nastaví jim prvnì dle pozadí první buòky stejné pozadí a dle barvy pozadí i barvu orámování
	SetCells(Cells[ColCell_1][RowCell_1],ColCell_1,RowCell_1,ColCell_2,RowCell_2,-1,false);

	//typ první buòky
	Cells[ColCell_1][RowCell_1].Type=RefCell.Type;

	////vytvoøí resp. pøedá orámování oblasti dle referenèní buòky, šlo by øešit v ve vıše volaném prùchodu, bylo by sice systomovì ménì nároèné, ale více komplikované na realizaci
	SetRegion(RefCell,ColCell_1,RowCell_1,ColCell_2,RowCell_2);

	////text
	unsigned int W=0;unsigned int H=0;
	Cells[ColCell_2][RowCell_2].Text=RefCell.Text;//navrácení ze zálohy do poslední buòky, protoe ta se vykresluje jako poslední
	W=getWidthHeightText(RefCell).X;
	H=getWidthHeightText(RefCell).Y;
	if(RefCell.Font->Orientation==900){W=H;H=0;if(RefCell.Valign==MIDDLE)H=-getWidthHeightText(RefCell).X;}
	if(RefCell.Font->Orientation==2700){W=0;if(RefCell.Align==LEFT || RefCell.Align==CENTER)W=-H;H=0;if(RefCell.Valign==MIDDLE)H=getWidthHeightText(RefCell).X;}
		//if(Cell.Font->Orientation==2700)L-=H;

	//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøeby - DÙLEITE!!!
	Cells[ColCell_2][RowCell_2].Align=aNO;
	Cells[ColCell_2][RowCell_2].Valign=vNO;
	//zarovnání (zarovnává dle první buòky, ale pracuje s poslední, protoe ta se vykresluje zcela poslední)
	switch(Cells[ColCell_1][RowCell_1].Align)
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
						Ch->Left=Left+(Columns[ColCell_1].Left+Columns[ColCell_2].Left+Columns[ColCell_2].Width)/2-Ch->Width/2;
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
					default: Cells[ColCell_2][RowCell_2].TextPositon.X=(Columns[ColCell_1].Left-Columns[ColCell_2].Left+Columns[ColCell_2].Width)/2-W/2;
			 }
		 }break;
		 case RIGHT:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_2].Width-W;//øeším v setcomponents -RefCell.RightMargin-RefCell.RightBorder->Width/2;
		 }break;
	}
	switch(Cells[ColCell_1][RowCell_1].Valign)
	{
		 case TOP:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_1].Top-Rows[RowCell_2].Top;//øeším v setcomponents +RefCell.TopMargin+RefCell.TopBorder->Width/2;
		 }break;
		 case MIDDLE:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.Y=(Rows[RowCell_1].Top-Rows[RowCell_2].Top+Rows[RowCell_2].Height)/2-H/2;
		 }
		 break;
		 case BOTTOM:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_2].Height-H;//øeším v setcomponents -RefCell.BottomMargin-RefCell.BottomBorder->Width/2;
		 }
		 break;
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
//zkopíruje obsah, formát (bez orámování) z buòky na buòku (bez ukazatelového propojení)
void TmGrid::CopyAreaCell(TCells &RefCell,TCells &CopyCell,bool copyComponent)
{
	////typ
	if(copyComponent)CopyCell.Type=RefCell.Type;
	else CopyCell.Type=DRAW;
	////text + font
	//*CopyCell.Font=*RefCell.Font;
	CopyCell.Font->Size=RefCell.Font->Size;
	CopyCell.Font->Color=RefCell.Font->Color;
	CopyCell.Font->Orientation=RefCell.Font->Orientation;
	CopyCell.Font->Style=RefCell.Font->Style;
	CopyCell.Font->Pitch=RefCell.Font->Pitch;
	CopyCell.Font->Name=RefCell.Font->Name;
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
	////pozadí
	//*CopyCell.Background=*RefCell.Background;
	CopyCell.Background->Color=RefCell.Background->Color;
	CopyCell.Background->Style=RefCell.Background->Style;
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
//smae text celé tabulku
void TmGrid::Clear()
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		ClearColumn(X);
	}
}
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
		//*bufCells[X] = *Cells[X];//zkopírování pùvodních hodnot do zálohy //takto asi nevhodné kopírovat (zùstane ukazatel), s hvìzdickou sice hodnoty ale je potøeba pøidìlit pamìt pomocí new pro TBrush TFont a TBorders
		for(unsigned long Y=0;Y<bufRowCount;Y++)
		{
			CreateCell(bufCells[X][Y]);
			CopyCell(Cells[X][Y],bufCells[X][Y],true);
		}
	}

	//smazání pùvodních hodnot
	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;
	DeleteComponents();
	//pokud bych pùvodní chtìl komponenty zachovat, ale nefunguje zcela správnì DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);
	ColCount=bufColCount2;RowCount=bufRowCount2;
	DeleteTable();

	//vytvoøení nového realokovaného pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//urèeno pøi další realokaci pole, create toti pøepisuje buf hodnoty
	Create(ColCount,RowCount);

	//zkopírování pùvodních hodnot zpìt
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je menší
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je menší
	for(unsigned long X=0;X<bufColCount;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount;Y++)
		{
			//nelze celı ukazatel Cells[X][Y] = bufCells[X][Y];
			////typ
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();//alokace pamìti
			CopyAreaCell(bufCells[X][Y],Cells[X][Y]);//kopie vlastností, bez orámování
			////orámování
			CreateLinkBorder(X,Y,bufCells[X][Y]);
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
//odstraní dynamicky vytoøené komponenty, nutno volat pøed Delete()
void TmGrid::DeleteComponents()
{
	DeleteComponents(0,0,ColCount-1,RowCount-1);
}
//---------------------------------------------------------------------------
//odstraní dynamicky vytoøené komponenty od - do poètu sloupcù a øádkù, nutno volat pøed Delete()
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
				case CHECK:{TscGPCheckBox *CH=getCheck(X,Y);CH->Free();CH=NULL;delete CH;}break;
				case RADIO:{TscGPRadioButton *R=getRadio(X,Y);R->Free();R=NULL;delete R;}break;
			}
		}
	}
}
//---------------------------------------------------------------------------

