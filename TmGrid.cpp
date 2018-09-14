//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
#include "MyString.h"
#include <Clipbrd.hpp>
#include "gapoTT.h"
#include "gapoV.h"
#include "gapoR.h"
#include "unit2.h"
#include "poznamky.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TmGrid *mGrid;
//---------------------------------------------------------------------------
//konstruktor
TmGrid::TmGrid(TForm *Owner)
{
	Form=Owner;
	////nastaven� TABULKY
	Tag=0;//ID komponenty (vyu�iteln� nap�. pokud bude v�ce tabulek, tak se bude v�d�t, v jak� do�lo ke kliku)
	Left=0;Top=0;//um�st�n� cel� komponenty
	RowCount=0;ColCount=0;//po�et ��dk� a sloupc�
	DefaultColWidth=90,DefaultRowHeight=25;//v�choz� v��ka a ���ka ��dku
	Row=0;Col=0;//aktu�ln� ��dek a sloupec
	AntiAliasing_grid=false;
	AntiAliasing_text=false;
	SetColumnAutoFitColIdx=-3;//nastav� ���ku bun�k dan�ho sloupce dle parametru ColIdx, -3 = nep�izp�sobuje se velikost a u�ije se defaultColWidth,-2 v�echny sloupce stejn� podle nej�ir��ho textu, -1 p�izp�sobuje se ka�d� sloupec individu�ln�, 0 a v�ce jen konkr�tn� sloupec uveden� pomoc ColIdx
	preRowInd=-1;
	Decimal=3;//implicitn� po�et desetinn�ch m�st u numericedit�
	//or�mov�n� - default
	TBorder defBorder;
	defBorder.Color=(TColor)RGB(200,200,200);
	defBorder.Width=1;
	defBorder.Style=psSolid;
	Border=defBorder;Border.Width=2;

	////nastaven� defaln� BU�KY
	//vytvo�� novou bu�ku (alokuje ji pam�)
	CreateCell(DefaultCell);

	//typ
	DefaultCell.Type = DRAW;//defaultn� komponenta
	//text
	DefaultCell.Font->Size=12;
	DefaultCell.Font->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.Font->Orientation=0;
	DefaultCell.Font->Style=TFontStyles();
	DefaultCell.Font->Pitch=TFontPitch::fpVariable;//ka�d� p�smeno fontu stejn� �irok�
	DefaultCell.Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.Font->Name="Arial";
	DefaultCell.TextPositon.X=0;
	DefaultCell.TextPositon.Y=0;
	DefaultCell.Text="";
	//zarovn�n�
	DefaultCell.Align=CENTER;
	DefaultCell.Valign=MIDDLE;
	DefaultCell.TopMargin=1;
	DefaultCell.BottomMargin=1;
	DefaultCell.LeftMargin=1;
	DefaultCell.RightMargin=1;
	//pouze indikuje, zda je bu�ka slou�ena, �i nikoliv, slou�� jako pom�cka p�i vykreslov�n� or�mov�n� slou�en�ch bun�k
	DefaultCell.MergeState=false;
	//pozad�
	DefaultCell.Background->Color=clWhite;
	DefaultCell.Background->Style=bsSolid;
	//or�mov�n�
	*DefaultCell.TopBorder=defBorder;
	*DefaultCell.BottomBorder=defBorder;
	*DefaultCell.LeftBorder=defBorder;
	*DefaultCell.RightBorder=defBorder;
}
//---------------------------------------------------------------------------
//destruktor, prob�h� p�i ukon�ov�n� programu, tj. zv�it zda nen� pozd�
TmGrid::~TmGrid()
{
		Delete();
}
//---------------------------------------------------------------------------
//vytvo�� tabulku
void TmGrid::Create()
{
	//alokace dvourozmerneho dynamickeho pole
	Cells = new TCells*[ColCount];//vytvo�� dynamick� pole ukazatelu typu TCells dle po�tu po�adovan�ch sloupc�
	for(unsigned long X=0;X<ColCount;X++)Cells[X] = new TCells[RowCount];//pruchod po sloupcich, slupc�m dynamamick�ho pole alokuje pam�t pro jednotliv� ��dky- cyklus mus� b�t samostatn�
	//alokace jednorozm�rn�ho dynamick�ho pole
	Columns = new TColumns[ColCount];
	Rows = new TRows[RowCount];
	//smaz�n� + nastaven� hodnot mus� b�t v extra cyklu
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			////BU�KY
			//Cells[X][Y]=DefaultCell; //- nelze, takto p�evezme cel� ukazatel
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();//alokace pam�ti
			CopyAreaCell(DefaultCell,Cells[X][Y],true);//kopie vlastnost�, bez or�mov�n�
			////or�mov�n� bun�k ukazatelem
			CreateLinkBorder(X,Y,DefaultCell);
			////��DKY
			Rows[Y].Height=DefaultRowHeight;
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Y*DefaultRowHeight;else Rows[0].Top=0;
		}
		////SLOUPCE
		Columns[X].Width=DefaultColWidth;
		if(X>0){Columns[X].Left=Columns[X-1].Left+X*DefaultColWidth;}else Columns[0].Left=0;
	}
	bufColCount=ColCount;bufRowCount=RowCount;//ur�eno p�i dal�� realokaci pole
	preTop=Top;preLeft=Left;//zaloha �vodn� pozice
}
//---------------------------------------------------------------------------
//p�et�en� metoda - vytvo�� tabulku s p�edepsan�m po�tem sloupc� a ��dk�
void TmGrid::Create(unsigned long ColCount,unsigned long RowCount)
{
	rcc(ColCount,RowCount);//pouze obejit� lok�ln� prom�nn�, v c++ je na to n�jak� kl��ov� slovo, ale nevzpomenu si
	Create();
}
//---------------------------------------------------------------------------
//pouze obejit� lok�ln� prom�nn�, v c++ je na to n�jak� kl��ov� slovo, ale nevzpomenu si
void TmGrid::rcc(unsigned long cc,unsigned long rc)
{
	ColCount=cc;RowCount=rc;
}
//---------------------------------------------------------------------------
//pat�i�n� prolinkuje or�mov�n�, �e sousedn� or�mov�n� m� ukazatel na toto�n� objekt, vzor or�mv�n� z�sk� dle refCell
void TmGrid::CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell)
{
	//top (p�eb�r�, propojuje se s horn�m, nejedn�-li se o prvn�)
	if(Y==0){Cells[X][Y].TopBorder=new TBorder;*Cells[X][Y].TopBorder=*refCell.TopBorder;}
	else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spole�n� ohrani�en�, jedn� se o jeden objekt
	//botom
	Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*refCell.BottomBorder;
	//left (p�eb�r�, propojuje se s lev�m, nejedn�-li se o prvn�)
	if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*refCell.LeftBorder;}
	else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na spole�n� ohrani�en�, jedn� se o jeden objekt
	//right
	Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*refCell.RightBorder;
}
//pat�i�n� prolinkuje or�mov�n�, �e sousedn� or�mov�n� m� ukazatel na toto�n� objekt, vzor or�mv�n� z�sk� dle refCell
void TmGrid::CreateLinkBorder2(unsigned long X,unsigned long Y,TCells &refCell)
{
	//top (p�eb�r�, propojuje se s horn�m, nejedn�-li se o prvn�)
	try{
	//if(refCell.TopBorder==NULL)ShowMessage("null");
	if(Y==0){Cells[X][Y].TopBorder=new TBorder;/**Cells[X][Y].TopBorder=*refCell.TopBorder;*/}
	else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spole�n� ohrani�en�, jedn� se o jeden objekt
	//botom
	Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*refCell.BottomBorder;
	//left (p�eb�r�, propojuje se s lev�m, nejedn�-li se o prvn�)
	if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*refCell.LeftBorder;}
	else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na spole�n� ohrani�en�, jedn� se o jeden objekt
	//right
	Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*refCell.RightBorder;
	}catch(...)
	{
			ShowMessage(Y);
	}
}
//---------------------------------------------------------------------------
//vytvo�� novou bu�ku (alokuje ji pam�)
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
//sma�e bu�ku z pam�ti
void TmGrid::DeleteCell(TCells &DelCell)
{
	DelCell.TopBorder=NULL;			delete DelCell.TopBorder;
	DelCell.BottomBorder=NULL;  delete DelCell.BottomBorder;
	DelCell.LeftBorder=NULL;    delete DelCell.LeftBorder;
	DelCell.RightBorder=NULL;   delete DelCell.RightBorder;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//odstran� tabulku, p�idru�en� komponenty a ukazatel na mGrid z pam�ti
void TmGrid::Delete()
{
	try
	{
		//odstran�n� v tabulce pou�it�ch komponent
		DeleteComponents();
		//uvoln�n� pam�ti
		DeleteTable();
		DeleteCell(DefaultCell);
		mGrid=NULL; delete mGrid;
	}
	catch(...)
	{MessageBeep(0);}
}
//---------------------------------------------------------------------------
//odstran� pouze tabulku, pomocn� metoda v��e uveden� a destruktoru
void TmGrid::DeleteTable()
{
	for(unsigned long X=0;X<ColCount;X++)//po ��dc�ch
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
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
//zajist� vykreslen� cel� tabulky
void TmGrid::Show()
{
	//if(mGrid!=NULL)
	{
		if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k po�adavku na zm�nu po�tu ��dk� a sloupc� vol� se realokace
		{
			realock();
			Form->Refresh();
		}

		//nastaven� ���ky sloupc� a v��ky ��dk�+autofit sloupc� nastav� Columns[X].ColWidth
		SetColRow();//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby

		//o�et�en� proti situaci AntiAliasing_text=true; a AntiAliasing_grid=false, kter� nem��e nastat resp. byla by zbyte�n�
		if(AntiAliasing_text==false)AntiAliasing_grid=false;

		if(AntiAliasing_grid==false && AntiAliasing_text==false)Draw(Form->Canvas);
		else
		{
			Cantialising a;
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=Width*3;bmp_in->Height=Height*3;//velikost canvasu//*3 vypl�v� z logiky algoritmu antialiasingu
			Draw(bmp_in->Canvas);
			Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutn� do samostatn� bmp, kv�li smaz�n� bitmapy vracen� AA
			Form->Canvas->Draw(Left,Top,bmp_out);
			if(AntiAliasing_grid==false && AntiAliasing_text==true)DrawGrid(Form->Canvas);//kresl� se a� nahoru
			delete (bmp_out);//velice nutn�
			delete (bmp_in);//velice nutn�
		}
		//zaloha �vodn� pozice
		preTop=Top;preLeft=Left;
	}
}
//---------------------------------------------------------------------------
//zajist� vyvol�n� p�ekreslen� cel� tabulky s p�eblikem, ale l�pe pou��vat p��mo ve v dan�m formul��i FormPaint(this), co� zajist� p�ekreslen� bez probliku
void TmGrid::Refresh()
{
	 Form->Repaint();
}
//---------------------------------------------------------------------------
//zajist� vykreslen� cel� tabulky v�etn� gridu, pokud
void TmGrid::Draw(TCanvas *C)
{
	//////TABULKA - or�mov�n� cel�, pouze nastavuje jednotliv�m bu�k�m jejich or�mov�n�
	if(Border.Width>0)//pokud je border po�adov�n
	{
		TCells RefCell;CreateCell(RefCell);
		*RefCell.TopBorder=*RefCell.LeftBorder=*RefCell.RightBorder=*RefCell.BottomBorder=Border;
		SetRegion(RefCell,0,0,ColCount-1,RowCount-1);
		DeleteCell(RefCell);
	}

	//////BU�KY jednotliv�
	short Zoom_g=1; if(AntiAliasing_grid)Zoom_g=3;
	short Zoom_b=1; if(AntiAliasing_text)Zoom_b=3;
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		////oblast bu�ky
		TRect R;//grid
		TRect Rt;//text
		TRect Rb;//text
		TRect Rc;//componenty
		R.Left	=	Left+Columns[X].Left*Zoom_g;
		Rt.Left	=	Left+Columns[X].Left*Zoom_b;
		Rb.Left	=	Columns[X].Left*Zoom_b;
		Rc.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_g;
		Rt.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_b;
		Rb.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;
		Rc.Right	=	Left+(Columns[X].Left+Columns[X].Width);

		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			////oblast bu�ky
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//v�po�et horn�ho okraje bu�ky dle bu�ky p�edchoz�
			R.Top			=	Top+Rows[Y].Top*Zoom_g;
			Rt.Top		=	Top+Rows[Y].Top*Zoom_b;
			Rb.Top		=	Rows[Y].Top*Zoom_b;
			Rc.Top		=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_g;
			Rt.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
			Rb.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
			Rc.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

			////barva pozad� bu�ky
			C->Brush->Color=Cells[X][Y].Background->Color;
			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(Rb);

			////komponenta v bu�ce
			SetComponents(C,Rc,Rt,X,Y,Cells[X][Y]);

			////or�mov�n� bu�ky
			//pou��v�m duplicitn� (k DrawGrid) zde, kv�li akceleraci v p��pad� tot�ln� vypnut�ho AA nebo tot�ln� zapnut�ho AA, v takov�m p��pad� potom neb�� DrawGrid, mohl bych ho sice volat zde, ale chci si u�et�it opakovan� pr�chod cykly, DrawGrid b�� jenom v momentu AntiAliasing_text=false a AntiAliasing_grid=true
			if(!(AntiAliasing_grid==false && AntiAliasing_text==true))
			{
				//top
				if(Cells[X][Y].TopBorder->Color!=Cells[X][Y].Background->Color)//kv�li slou�en�m bu�k�m
				{
					SetBorder(C,Cells[X][Y].TopBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
				}
				//bottom
				if(Y==RowCount-1)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
				{
					SetBorder(C,Cells[X][Y].BottomBorder);
					C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
				}
				//left
				if(Cells[X][Y].LeftBorder->Color!=Cells[X][Y].Background->Color)//kv�li slou�en�m bu�k�m
				{
					SetBorder(C,Cells[X][Y].LeftBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
				}
				//right
				if(X==ColCount-1)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
				{
					SetBorder(C,Cells[X][Y].RightBorder);
					C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
//zajist� vykreslen� jen gridu
void TmGrid::DrawGrid(TCanvas *C)
{
	TRect R;//grid
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		R.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width);
		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			R.Top			=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);
			////or�mov�n� bu�ky
			//top
			if(Cells[X][Y].TopBorder->Color!=Cells[X][Y].Background->Color)//kv�li slou�en�m bu�k�m
			{
				SetBorder(C,Cells[X][Y].TopBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
			}
			//bottom
			if(Y==RowCount-1)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
			{
				SetBorder(C,Cells[X][Y].BottomBorder);
				C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
			}
			//left
			if(Cells[X][Y].LeftBorder->Color!=Cells[X][Y].Background->Color)//kv�li slou�en�m bu�k�m
			{
				SetBorder(C,Cells[X][Y].LeftBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
			}
			//right
			if(X==ColCount-1)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
			{
				SetBorder(C,Cells[X][Y].RightBorder);
				C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
			}
		}
	}
}
//---------------------------------------------------------------------------
//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby
void TmGrid::SetColRow()
{
	executeColumnsAutoFit(Form->Canvas);

	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		if(X>0)Columns[X].Left=Columns[X-1].Left+Columns[X-1].Width;else Columns[0].Left=0;//v�po�et lev�ho okraje bu�ky dle bu�ky p�edchoz�
	}
	for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
	{
		if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//v�po�et horn�ho okraje bu�ky dle bu�ky p�edchoz�
	}

	////v�po�et celkov� ���ky a v��ky tabulky
	Width=getWidth();//z�rove� nastavuje celkovou hodnotu Width, mus� b�t a� tady!!!, proto�e v��e se to teprv� nastavuje
	Height=getHeight();//z�rove� nastavuje celkovou hodnotu Height, mus� b�t a� tady!!!, proto�e v��e se to teprv� nastavuje
}
//---------------------------------------------------------------------------
//nastav� grafick� pero na po�adovan� parametry
void TmGrid::SetBorder(TCanvas *C,TBorder *Border)
{
	DeleteObject(C->Pen->Handle);//zru�� p�vodn� pero       //PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE
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
//nastav� danou bu�ku dle typu
void TmGrid::SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell)
{
	switch(Cell.Type)
	{
		case DRAW:
		{   //Cell.Text=getTag(X,Y);//provizorn� v�pis pokud chci o��slovat jednotliv� bu�ky
			short Zoom=1;if(AntiAliasing_text)Zoom=3;
			//nastaven� fontu
			Canv->Font=Cell.Font;
			Canv->Font->Size*=Zoom;
			//SetBkMode(canv->Handle,OPAQUE);//nastven� netransparentn�ho pozad�
			Canv->Brush->Color=Cell.Background->Color;Canv->Brush->Style=bsClear;//nastven� netransparentn�ho pozad�
			//zarovn�n�
			//samotn� v�pis
			long L=Rt.Left,T=Rt.Top;              //zajimav� workaround - p���in� nerozum�m
			int W=getWidthHeightText(Cell).X*Zoom;//if(AntiAliasing_text && (Cell.Font->Orientation!=0 || Cell.Align!=aNO))W/=1.3;// - provizorn� odstaveno - chov� se to bez toho l�pe u neslou�en�ch bun�k na st�ed, ale zase m��e chyb�t n�kd� jinde
			int H=getWidthHeightText(Cell).Y*Zoom;
			short Rot=1;//slou�� jako pom�cka rotace
			if(Cell.Font->Orientation==900){Rot=-1;H=0;if(Cell.Valign==MIDDLE)H=-getWidthHeightText(Cell).Y;}
			if(Cell.Font->Orientation==2700){Rot=-1;W=0;if(Cell.Align==LEFT || Cell.Align==CENTER)W=-W;H=0;if(Cell.Valign==MIDDLE)H=getWidthHeightText(Cell).Y;}
			short WA=0;if(AntiAliasing_text && (Cell.Font->Orientation!=0 || Cell.Align!=aNO))WA=1;//zajimav� workaround - p���in� nerozum�m (pro� Left*2 tomu pat�i�n� pom��e)
			switch(Cell.Align)
			{               //zajimav� workaround
				case aNO:   L=WA*Left*1.3+Rt.Left+Cell.TextPositon.X*Zoom+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2*Zoom;break;
				case LEFT:	L=WA*Left*1.3+Rt.Left+Cell.LeftMargin*Zoom+Cells[X][Y].LeftBorder->Width/2*Zoom;break;
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
			}                                    //*2 zajimav� workaround
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
			//zalo�en� + tag + n�zev
			TscGPButton *B=createButton(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
			//atributy
			B->Top=R.Top+floor(Cell.TopBorder->Width/2.0);
			B->Left=R.Left+floor(Cell.LeftBorder->Width/2.0);
			B->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-floor(Cell.LeftBorder->Width/2.0);
			B->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-floor(Cell.TopBorder->Width/2.0);
//			B->Options->NormalColor=Cell.Background->Color;
			B->Options->FrameNormalColor=B->Options->NormalColor;
			B->Font=Cell.Font;
			B->Caption=Cell.Text;
			//vlastn�k
			B->Parent=Form;//mus� b�t a� na konci
			B=NULL;delete B;
		}break;
		case COMBO:
		{
			//zalo�en� + tag + n�zev
			TscGPComboBox *C=getCombo(X,Y);//pokud ji� existuje,vr�t� ukazatel
			if(C==NULL)//pokud ne
			{
				C=new TscGPComboBox(Form);//zalo��
				C->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
				C->Name="mGrid_COMBO_"+AnsiString(C->Tag);
				//ud�losti
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
//			C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu
//			TscGPListBoxItem *t=C->Items->Add();t->Caption=Cell.Text;
//			t=NULL;delete t;
//			C->ItemIndex=1;//nelze p�edozna�n� prvn� polo�ku
			//vlastn�k
			C->Parent=Form;//mus� b�t a� na konci
			C=NULL;delete C;
		}break;
		case CHECK:
		{
			//zalo�en� + tag + n�zev
			TscGPCheckBox *Ch = createCheck(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
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
			//nepou��vat Ch->Options->FrameNormalColor=(TColor)RGB(128,128,128);//Ch->Font->Color;
			//nepou��vat Ch->Options->FrameNormalColorAlpha=255;
			Ch->Font=Cell.Font;
			Ch->Caption=Cell.Text;
			//vlastn�k
			Ch->Parent=Form;//mus� b�t a� na konci
			Ch=NULL;delete Ch;
		}break;
		case RADIO:
		{
			//zalo�en� + tag + n�zev
			TscGPRadioButton *Ra = createRadio(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
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
			//vlastn�k
			Ra->Parent=Form;//mus� b�t a� na konci
			Ra=NULL;delete Ra;
		}break;
	}
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na edit, pomocn� metoda v��e uveden�
void TmGrid::SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPEdit *E=createEdit(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	//atributy
	if(Cell.Type==EDIT)E->Enabled=true;else E->Enabled=false;
	E->AutoSize=false;
	E->Top=R.Top+Cell.TopBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	E->Left=R.Left+Cell.LeftBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	E->Width=Columns[X].Width-Cell.RightBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	E->Height=Rows[Y].Height-Cell.BottomBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	E->ShowHint=false;//implicitn� u editu na false, pokus pro dlouh� textif(Cell.Text.Length()>E->Width/(Cell.Font->Size-2))E->ShowHint=true;else //asi nep�esn�
	E->Hint=Cell.Text;//v�choz� text pro hint je hodnota z editu
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
	//vlastn�k
	E->Parent=Form;//mus� b�t a� na konci
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
void TmGrid::SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPNumericEdit *N=createNumeric(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	//atributy
	if(Cell.Type==NUMERIC)N->Enabled=true;else N->Enabled=false;
	N->AutoSize=false;
	N->Top=R.Top+Cell.TopBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	N->Left=R.Left+Cell.LeftBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	if(Cell.MergeState==false)N->Width=Columns[X].Width-Cell.RightBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	N->Height=Rows[Y].Height-Cell.BottomBorder->Width;//ubere velikost komponenty podle ���ky or�mov�n�
	N->Decimal=Decimal;
	N->DisplayType=scedtNumeric;
	N->ValueType=scvtFloat;
	N->ShowHint=true;
	N->Hint=Cell.Text;
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
	//if(Col!=X && Row!=Y)
	{
		TMyString ms;
		N->Value=ms.MyToDouble(Cell.Text);
	}
	//vlastn�k
	N->Parent=Form;//mus� b�t a� na konci
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPEdit *TmGrid::createEdit(unsigned long Col,unsigned long Row)
{
	TscGPEdit *E=getEdit(Col,Row);//pokud ji� existuje
	if(E==NULL)//pokud ne, tak zalo��
	{
		E = new TscGPEdit(Form);
		E->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		E->Name="mGrid_EDIT_"+AnsiString(E->Tag);

		//ud�losti
		E->OnClick=&getTagOnClick;
		E->OnEnter=&getTagOnEnter;
		E->OnChange=&getTagOnChange;
	}
	return E;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPNumericEdit *TmGrid::createNumeric(unsigned long Col,unsigned long Row)
{
	TscGPNumericEdit *N=getNumeric(Col,Row);//pokud ji� existuje
	if(N==NULL)//pokud ne
	{
		N=new TscGPNumericEdit(Form);
		N->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		N->Name="mGrid_NUMERIC_"+AnsiString(N->Tag);
		//ud�losti
		N->OnClick=&getTagOnClick;
		N->OnEnter=&getTagOnEnter;
		N->OnChange=&getTagOnChange;
	}
	return N;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPButton *TmGrid::createButton(unsigned long Col,unsigned long Row)
{
	TscGPButton *B=getButton(Col,Row);//pokud ji� existuje
	if(B==NULL)//pokud ne, tak zalo��
	{
		B = new TscGPButton(Form);
		B->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		B->Name="mGrid_BUTTON_"+AnsiString(B->Tag);

		//ud�losti
		B->OnClick=&getTagOnClick;
		B->OnEnter=&getTagOnEnter;
	}
	return B;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPCheckBox *TmGrid::createCheck(unsigned long Col,unsigned long Row)
{
	TscGPCheckBox *Ch=getCheck(Col,Row);//pokud ji� existuje
	if(Ch==NULL)//pokud ne, tak zalo��
	{
		Ch = new TscGPCheckBox(Form);
		Ch->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		//Cell.Text=Ch->Tag; ShowMessage(Ch->Tag);
		Ch->Name="mGrid_CHECK_"+AnsiString(Ch->Tag);

		//ud�losti
		Ch->OnClick=&getTagOnClick;
		Ch->OnEnter=&getTagOnEnter;
	}
	return Ch;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPRadioButton *TmGrid::createRadio(unsigned long Col,unsigned long Row)
{
	TscGPRadioButton *Ra=getRadio(Col,Row);//pokud ji� existuje
	if(Ra==NULL)//pokud ne, tak zalo��
	{
		Ra = new TscGPRadioButton(Form);
		Ra->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		//Cell.Text=Ra->Tag; ShowMessage(Ra->Tag);
		Ra->Name="mGrid_RADIO_"+AnsiString(Ra->Tag);

		//ud�losti
		Ra->OnClick=&getTagOnClick;
		//Ra->OnEnter=&getTagOnEnter;//asi zbyte�n� ud�lost
	}
	return Ra;
}
//---------------------------------------------------------------------------
//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
unsigned long TmGrid::getTag(unsigned long Col,unsigned long Row)
{
	return (Col+1)+Row*ColCount;
}
//---------------------------------------------------------------------------
//z tagu vrat� ��slo sloupce
unsigned long TmGrid::getColFromTag(unsigned long Tag)
{
	long RET=Tag%ColCount-1;
	if(RET>=0)return RET;
	else return RET=ColCount-1;
}
//---------------------------------------------------------------------------
//z tagu vrat� ��slo ��dku
unsigned long TmGrid::getRowFromTag(unsigned long Tag)
{
	return ceil(Tag/(double)ColCount)-1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//PUBLIC: nastav� ���ku bun�k dan�ho sloupce dle nejdel��ho ���ky textu bu�ky ze sloupce uveden�ho parametrem ColIdx sloupce
void TmGrid::SetColumnAutoFit(long ColIdx)
{
	SetColumnAutoFitColIdx=ColIdx;
	SetColRow();//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby
}
//---------------------------------------------------------------------------
//nastav� ���ku bun�k sloupc� dle ���ky textu dle zvolen�ho parametru
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
		////v�echny sloupce stejn� podle nej�ir��ho
		case -2:
		{
			unsigned int MaxColWidth=4;//minim�ln� rozm�r bu�ky pokud je nastavena na autofit a neobsahuje text
			//nalezen� maxima
			for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
			{
				for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
				{
					unsigned int W=getWidthHeightText(Cells[X][Y]).X+Cells[X][Y].LeftMargin+Cells[X][Y].LeftBorder->Width/2+Cells[X][Y].RightMargin+Cells[X][Y].RightBorder->Width/2;
					if(Cells[X][Y].Type==CHECK || Cells[X][Y].Type==RADIO)W+=20;
					if(W>MaxColWidth)MaxColWidth=W;//najde nej�i���
				}
			}
			//zapis do pole ���ky sloupc�
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=MaxColWidth;
		}
		break;
		////v�echny sloupce individu�ln�
		case -1:
		{
			for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
			executeColumnAutoFit(Canv,X);
		}
		break;
		////dle konkr�tn�ho
		default:
		{
			executeColumnAutoFit(Canv,SetColumnAutoFitColIdx);
		}
		break;
	}
}
//---------------------------------------------------------------------------
//nastav� ���ku bun�k dan�ho sloupce dle ���ky textu v dan�m sloupci
void TmGrid::executeColumnAutoFit(TCanvas *Canv,long ColIdx)
{
	unsigned int ColWidth=getWidthHeightText(Cells[ColIdx][0]).X;//v�choz� hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		short Zoom=1;if(AntiAliasing_text==true)Zoom=3;
		unsigned int W=getWidthHeightText(Cells[ColIdx][Y]).X+Cells[ColIdx][Y].LeftMargin*Zoom+Cells[ColIdx][Y].LeftBorder->Width*Zoom/2+Cells[ColIdx][Y].RightMargin*Zoom+Cells[ColIdx][Y].RightBorder->Width*Zoom/2;
		if(Cells[ColIdx][Y].Type==CHECK || Cells[ColIdx][Y].Type==RADIO)W+=20+4+4;
		if(W>ColWidth)ColWidth=W;//najde nej�i���
	}
	Columns[ColIdx].Width=ColWidth+4;//+4 pouze offset
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vr�t� celkovou ���ku tabulky
unsigned long TmGrid::getWidth()
{
	return Columns[ColCount-1].Left+Columns[ColCount-1].Width;
}
//---------------------------------------------------------------------------
//vr�t� celkovou v��ku tabulky
unsigned long TmGrid::getHeight()
{
	return Rows[RowCount-1].Top+Rows[RowCount-1].Height;
}
//---------------------------------------------------------------------------
//vr�t� ���ku a v��ku textu bu�ky v pixelech
TPoint TmGrid::getWidthHeightText(TCells &Cell)
{
	TPoint RET;
	Form->Canvas->Font=Cell.Font;//nastaven� fontu
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
//zajist� rotaci textu
void TmGrid::rotace_textu(long rotace)
{
	LOGFONT LogRec;
	GetObject(Form->Canvas->Font->Handle,sizeof(LogRec),&LogRec);
	LogRec.lfEscapement=rotace;
	Form->Canvas->Font->Handle=CreateFontIndirect(&LogRec);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//dle zadan�ho  ��sla sloupce a ��sla ��dku  vr�t� ukazatel nadanou komponentu
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(getTag(Col,Row)));};//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� z dan� komponenty text do pam�ov� bu�ky, slou�� nap�. p�i ud�losti onchange pop�. d�l��ch
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
	//ShowMessage(AnsiString("OnClick ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);

	if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnClick(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnClick(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnClick(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnClick(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnClick(Tag,Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnEnter(TObject *Sender)
{
	//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);

	if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnEnter(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnEnter(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnEnter(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnEnter(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnEnter(Tag,Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnChange(TObject *Sender)
{
	//ShowMessage(AnsiString("OnChange ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);

	getTextFromComponentToMemoryCell(Col,Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� z dan� komponenty text do pam�ov� bu�ky, slou�� nap�. p�i ud�losti onchange pop�. d�l��ch

	if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnChange(Tag,Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnKeyDown(TObject *Sender)
{
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);  //asi z�m�rn� OnChange?
	if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnChange(Tag,Col,Row);
	if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnChange(Tag,Col,Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//spoj� dv� bu�ky do jedn�
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	if(ColCell_1!=ColCell_2 || RowCell_1!=RowCell_2)//pokud se jedn� o slou�en� jedn� bu�ky - nelogick� slou�en� nic nevykon�
	{
		SetColRow();

		////kopie referen�n� bu�ky //TCells RefCell=Cells[ColCell_1][RowCell_1];// - nelze v�ak pou��t takto na p�ed�v�n� borders, proto metoda n�e, p�ed�valo by i ukazatel
		TCells RefCell;CreateCell(RefCell);
		CopyCell(Cells[ColCell_1][RowCell_1],RefCell,true);

		////nastaven� referen�n� bu�ky kv�li or�mov�n� v�ech bun�k oblasti na toto�nou barvu
		TBorder B;B.Width=0;B.Style=psSolid;B.Color=Cells[ColCell_1][RowCell_1].Background->Color;
		*Cells[ColCell_1][RowCell_1].TopBorder=*Cells[ColCell_1][RowCell_1].BottomBorder=*Cells[ColCell_1][RowCell_1].LeftBorder=*Cells[ColCell_1][RowCell_1].RightBorder=B;
		Cells[ColCell_1][RowCell_1].MergeState=true;//ozna�� bu�ku jako slou�enou, slou�� pro p�esko�en� vykreslov�n� or�mov�n�, uprost�ed slou�en�ch objekt�

		////projde nejd��ve v�echny bu�ky nastav� jim prvn� dle pozad� prvn� bu�ky stejn� pozad� a dle barvy pozad� i barvu or�mov�n�
		SetCells(Cells[ColCell_1][RowCell_1],ColCell_1,RowCell_1,ColCell_2,RowCell_2,-1,false);

		////typ prvn� bu�ky
		Cells[ColCell_1][RowCell_1].Type=RefCell.Type;

		////vytvo�� resp. p�ed� or�mov�n� oblasti dle referen�n� bu�ky, �lo by �e�it v ve v��e volan�m pr�chodu, bylo by sice systomov� m�n� n�ro�n�, ale v�ce komplikovan� na realizaci
		SetRegion(RefCell,ColCell_1,RowCell_1,ColCell_2,RowCell_2);

		////text
		int W=0;int H=0;
		Cells[ColCell_2][RowCell_2].Text=RefCell.Text;//navr�cen� ze z�lohy do posledn� bu�ky, proto�e ta se vykresluje jako posledn�
		W=getWidthHeightText(RefCell).X;
		H=getWidthHeightText(RefCell).Y;
		if(RefCell.Font->Orientation==900){H=0;if(RefCell.Valign==MIDDLE)H=-getWidthHeightText(RefCell).X;}
		if(RefCell.Font->Orientation==2700){W=0;if(RefCell.Align==LEFT || RefCell.Align==CENTER)W=-H;H=0;if(RefCell.Valign==MIDDLE)H=getWidthHeightText(RefCell).X;}
		//if(Cell.Font->Orientation==2700)L-=H;

		//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby - D�LE�ITE pro text!!!
		Cells[ColCell_2][RowCell_2].Align=aNO;
		Cells[ColCell_2][RowCell_2].Valign=vNO;
		//zarovn�n� (zarovn�v� dle prvn� bu�ky, ale pracuje s posledn�, proto�e ta se vykresluje zcela posledn�)
		switch(Cells[ColCell_1][RowCell_1].Align)//HORIZONT�LN� ZAROVN�N�
		{
			 case LEFT:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_1].Left-Columns[ColCell_2].Left;//�e��m v setcomponents +RefCell.LeftMargin+RefCell.LeftBorder->Width/2;
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
							N->Value=F->ms.MyToDouble(RefCell.Text);//bere a� z posledn� bu�ky slu�ovan� oblasti
							N=NULL;delete N;
						}break;
						default: Cells[ColCell_2][RowCell_2].TextPositon.X=(Columns[ColCell_1].Left-Columns[ColCell_2].Left+Columns[ColCell_2].Width)/2-W/2;
				 }
			 }break;
			 case RIGHT:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_2].Width-W;//�e��m v setcomponents -RefCell.RightMargin-RefCell.RightBorder->Width/2;
			 }break;
		}
		switch(Cells[ColCell_1][RowCell_1].Valign)//VERTIK�LN� ZAROVN�N�
		{
			 case TOP:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_1].Top-Rows[RowCell_2].Top;//�e��m v setcomponents +RefCell.TopMargin+RefCell.TopBorder->Width/2;
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
							Cells[ColCell_1][RowCell_1].Align=MIDDLE;
							TscGPEdit *E=createEdit(ColCell_1,RowCell_1);
							E->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							E=NULL;delete E;
						}break;
						case NUMERIC:
						{
							Cells[ColCell_1][RowCell_1].Align=MIDDLE;
							TscGPNumericEdit *N=createNumeric(ColCell_1,RowCell_1);
							N->Height=Rows[ColCell_2].Top+Rows[ColCell_2].Height-Rows[ColCell_1].Top-Cells[ColCell_2][RowCell_2].BottomBorder->Width;
							N=NULL;delete N;
						}break;

						default: Cells[ColCell_2][RowCell_2].TextPositon.Y=(Rows[RowCell_1].Top-Rows[RowCell_2].Top+Rows[RowCell_2].Height)/2-H/2;
				 }
			 }
			 break;
			 case BOTTOM:
			 {
				 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_2].Height-H;//�e��m v setcomponents -RefCell.BottomMargin-RefCell.BottomBorder->Width/2;
			 }
			 break;
		}
	}
}
//---------------------------------------------------------------------------
//nastav� oblast bun�k toto�n�mi vlastnostmi dle referen�n� bu�ky, text podle posledn�ho parametru bu� -1 -sma�e, 0 - zanech� p�vodn� (implicitn�), 1 zkop�ruje v�ude stejn�), za��nat zad�vat od nejvy��� a nejv�ce vlevo
void TmGrid::SetCells(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,short setText,bool copyComponent)
{
	for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
	{
		for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
		{
			UnicodeString Text=Cells[X][Y].Text;//z�loha textu, varianta zanech� p�vodn�
			if(setText==-1)Text="";//varianta sma�e
			if(setText==1)Text=RefCell.Text;//varianta v�ude stejn�
			//Cells[X][Y]=RefCell;// - nelze v�ak pou��t na p�ed�v�n� borders, proto metoda n�e
			CopyCell(RefCell,Cells[X][Y],copyComponent);
			Cells[X][Y].Text=Text;//navr�cen� textu
		}
	}
}
//---------------------------------------------------------------------------
void TmGrid::SetRegion(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)//toto�n� ohrani�� danou oblast bun�k dle referen�n� bu�ky (zohled�uje i rozd�ly horn�,doln�,lev� prav� or�mov�n�), za��nat zad�vat od nejvy��� a nejv�ce vlevo
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
//zkop�ruje obsah, form�t a or�mov�n� z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
void TmGrid::CopyCell(TCells &RefCell,TCells &CopyCell,bool copyComponent)
{
	//zkop�ruje obsah, form�t (bez or�mov�n�) z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
	CopyAreaCell(RefCell,CopyCell,copyComponent);
	//zkop�ruje or�mov�n� z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
	CopyBordesCell(RefCell,CopyCell);
}
//---------------------------------------------------------------------------
//zkop�ruje danou oblast do schr�nky, bu�ky odd�l� separ�torem
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
		T+="\n";//konec ��dku
	}
	c->AsText = T;
	c = NULL; delete c;
}
//---------------------------------------------------------------------------
//zkop�ruje obsah, form�t (bez or�mov�n�) z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
void TmGrid::CopyAreaCell(TCells &RefCell,TCells &CopyCell,bool copyComponent)
{
	////typ
	if(copyComponent){CopyCell.Type=RefCell.Type;/*createComponent(RefCell.Type,)*/}
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
	////zarovn�n�
	CopyCell.Align=RefCell.Align;
	CopyCell.Valign=RefCell.Valign;
	//osazen�
	CopyCell.TopMargin=RefCell.TopMargin;
	CopyCell.BottomMargin=RefCell.BottomMargin;
	CopyCell.LeftMargin=RefCell.LeftMargin;
	CopyCell.RightMargin=RefCell.RightMargin;
	//indik�tor slou�en�
	CopyCell.MergeState=RefCell.MergeState;
	////pozad�
	//*CopyCell.Background=*RefCell.Background;
	CopyCell.Background->Color=RefCell.Background->Color;
	CopyCell.Background->Style=RefCell.Background->Style;
}
//---------------------------------------------------------------------------
//zkop�ruje or�mov�n� z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
void TmGrid::CopyBordesCell(TCells &RefCell,TCells &CopyCell)
{
	*CopyCell.TopBorder=*RefCell.TopBorder;
	*CopyCell.BottomBorder=*RefCell.BottomBorder;
	*CopyCell.LeftBorder=*RefCell.LeftBorder;
	*CopyCell.RightBorder=*RefCell.RightBorder;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//sma�e text v cel�m sloupci
void TmGrid::ClearColumn(unsigned long ColIdx)
{
	for(unsigned long Y=0;Y<=RowCount;Y++)
	{
		Cells[ColIdx][Y].Text="";
	}
}
//---------------------------------------------------------------------------
//sma�e text v cel�m ��dku
void TmGrid::ClearRow(unsigned long RowIdx)
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		Cells[X][RowIdx].Text="";
	}
}
//---------------------------------------------------------------------------
//sma�e text cel� tabulky
void TmGrid::Clear()
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		ClearColumn(X);
	}
}
//---------------------------------------------------------------------------
//p�id� ��dek za posledn� ��dek, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z p�edchoz�ho ��dku, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody AddRow p�imo v u�it�m formul��i
void TmGrid::AddRow(bool copyComponentFromPreviousRow,bool invalidate)
{  //pad� asi v realock p�� p�id�v�n� ��dk�, ale sp�e na mgrid2
	//p�i realock asi neudr�� v��ku ��dk�

  //zv��en� celkov�ho po�tu ��dk�
	RowCount++;

	//kopie komponent z nad��zen�ho ��dku, jeli-po�adov�no
	if(copyComponentFromPreviousRow)
	{
		realock();//mus� prob�hnout p�ed n�sleduj�c�m kodem, jinak �e�� Show
		for(unsigned long X=0;X<ColCount;X++)
		{
			Cells[X][RowCount-1].Type=Cells[X][RowCount-2].Type;
			//n�sleduj�c� nen� t�eba, po�e�� jen v��e uveden� + n�sledn� show: createComponent(Cells[X][RowCount-1].Type,X,RowCount-1);//dle zadan�ho ��sla sloupce a ��sla ��dku vytvo�enou komponentu dle Type, pokud existuje, tak se nic ned�je
		}
	}

	//pokud je po�adov�no p�ekreslen�
	if(invalidate)Show();//p�ekreslen� s problikem, jinak pou��t p��mo ve formu formpaint a toto zakomentovat
}
//---------------------------------------------------------------------------
//p��d� ��dek za ��dek uveden� dle parametru Row,, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z p�edchoz�ho ��dku, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody InsertRow p�imo v u�it�m formul��i
void TmGrid::InsertRow(long Row,bool copyComponentFromPreviousRow, bool invalidate)
{
	if(Row<RowCount-1)//pokud se nejedn� o posledn� ��dek, tam je to zbyte�n� a �e�� else v�tev AddRow
	{
		RowCount++;realock();//p�id� posledn� ��dek
		//p�ekop�ruje ��dek resp. bu�ky z ��dku n�sleduj�c�ho a p�edt�m p�id� posledn� ��dek
		for(unsigned long Y=RowCount-1;Y>Row;Y--)
		{
			for(unsigned long X=0;X<ColCount;X++)
			{
				CopyCell(Cells[X][Y-1],Cells[X][Y],true);
			}
			Rows[Y]=Rows[Y-1];
		}
		ClearRow(Row);//vypr�zn� vkl�dan� ��dek

		if(invalidate)//pokud je po�adov�no p�ekreslen�, zde je nutn� celou oblast p�ekreslit
		{
			//Show();net�eba
			Form->Invalidate();//t�eba
		}
	}
	else AddRow(copyComponentFromPreviousRow,invalidate);
}
//---------------------------------------------------------------------------
//sma�e cel� ��dek, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody InsertRow p�imo v u�it�m formul��i
void TmGrid::DeleteRow(long Row,bool invalidate)
{
	if(Row<=RowCount-1 && RowCount-1>0)//nelze smazat pouze jenom jeden ��dek
	{ //p�ekop�ruje ��dek resp. bu�ky na ��dku n�sleduj�c� a ubere posledn� ��dek,pokud se nejedn� o jedin� ��dek
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
	if(invalidate)Show();//pokud je po�adov�no p�ekreslen� //p�ekreslen� s problikem, jinak pou��t p��mo ve formu formpaint a toto zakomentovat
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajist� realokaci pole Cells dle nov� velikosti
void TmGrid::realock()
{
	//kopie do z�lo�n�ho pole a smaz�n�
	TCells **bufCells = new TCells*[bufColCount];//vytvo�� dynamick� pole ukazatelu typu TCells dle po�tu po�adovan�ch sloupc�
	for(unsigned long X=0;X<bufColCount;X++)
	{
		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupc�m dynamamick�ho pole alokuje pam�t pro jednotliv� ��dky- cyklus mus� b�t samostatn�
		bufCells[X] = Cells[X];//zkop�rov�n� p�vodn�ch hodnot do z�lohy //takto asi nevhodn� kop�rovat (z�stane ukazatel), s hv�zdickou sice hodnoty ale je pot�eba p�id�lit pam�t pomoc� new pro TBrush TFont a TBorders
	}

	//po�ty ��dk� a sloupc�
	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;

	//smaz�n� p�vodn�ch hodnot
	//p�vodn� komponenty zachov�m,  sma�u jen ty, kter� nejsou ji� pot�eba (p�i zmen�ov�n� tabulky), ale �dajn� nefunguje zcela spr�vn�
	DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);//nov� oblast a� star� oblast, nen� pot�eba o�et�ovat IFem pro p��pad p�id�v�n� (pohl�daj� si cykly ve vnit� algoritmu)
	ColCount=bufColCount2;RowCount=bufRowCount2;
	//toto pro� ne DeleteTable(); asi nahrazuje Create, ot�zka v�ak z�st�v� zda n�co nez�st�v� v pam�ti

	//vytvo�en� nov�ho realokovan�ho pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//ur�eno p�i dal�� realokaci pole, create toti� p�episuje buf hodnoty
	Create(ColCount,RowCount);

	//zkop�rov�n� p�vodn�ch hodnot zp�t
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je men��
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je men��
	for(unsigned long Y=0;Y<bufRowCount;Y++)
	{
		for(unsigned long X=0;X<bufColCount;X++)
		{
			//nelze cel� ukazatel
			Cells[X][Y] = bufCells[X][Y];
		}
	}

	//smaz�n� p�vodn�ho bufCells
	for(unsigned long X=0;X<bufColCount2;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount2;Y++)//po ��dc�ch
		{
			DeleteCell(bufCells[X][Y]);
		}
		bufCells[X]=NULL; delete bufCells[X];
	}
	bufCells=NULL; delete bufCells;
	bufColCount=ColCount;bufRowCount=RowCount;//ur�eno p�i dal�� realokaci pole
}
//---------------------------------------------------------------------------
////zajist� realokaci pole Cells dle nov� velikosti
//void TmGrid::realock()
//{              //ShowMessage("0");
//	//kopie do z�lo�n�ho pole a smaz�n�
//	TCells **bufCells = new TCells*[bufColCount];//vytvo�� dynamick� pole ukazatelu typu TCells dle po�tu po�adovan�ch sloupc�
//	for(unsigned long X=0;X<bufColCount;X++)
//	{
//		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupc�m dynamamick�ho pole alokuje pam�t pro jednotliv� ��dky- cyklus mus� b�t samostatn�
//		//bufCells[X] = Cells[X];//zkop�rov�n� p�vodn�ch hodnot do z�lohy //takto asi nevhodn� kop�rovat (z�stane ukazatel), s hv�zdickou sice hodnoty ale je pot�eba p�id�lit pam�t pomoc� new pro TBrush TFont a TBorders
//		for(unsigned long Y=0;Y<bufRowCount;Y++)
//		{
//			//toto tu bylo:CreateCell(bufCells[X][Y]);
//			//toto tu bylo:CopyCell(Cells[X][Y],bufCells[X][Y],true);
////			//totu tu nebylo:
////			CopyAreaCell(Cells[X][Y],bufCells[X][Y],true);
////			////or�mov�n�
////			if(Y==0){bufCells[X][Y].TopBorder=new TBorder;*bufCells[X][Y].TopBorder=*Cells[X][Y].TopBorder;}
////			else bufCells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spole�n� ohrani�en�, jedn� se o jeden objekt
////			//botom
////			bufCells[X][Y].BottomBorder=new TBorder;*bufCells[X][Y].BottomBorder=*Cells[X][Y].BottomBorder;
////			//left (p�eb�r�, propojuje se s lev�m, nejedn�-li se o prvn�)
////			if(X==0){bufCells[X][Y].LeftBorder=new TBorder;*bufCells[X][Y].LeftBorder=*Cells[X][Y].LeftBorder;}
////			else bufCells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;//pokud ne, odkazuje na spole�n� ohrani�en�, jedn� se o jeden objekt
////			//right
////			bufCells[X][Y].RightBorder=new TBorder;*bufCells[X][Y].RightBorder=*Cells[X][Y].RightBorder;
//		}
//	}
//	 //ShowMessage("1");
//	//smaz�n� p�vodn�ch hodnot
//	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
//	ColCount=bufColCount;RowCount=bufRowCount;
//	//DeleteComponents();//toto bylo odkomentov�no a n�e zakomentov�no DeleteComponents
//	//pokud bych p�vodn� cht�l komponenty zachovat, ale nefunguje zcela spr�vn�
//	DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);//nov� oblast a� star� oblast, nen� pot�eba o�et�ovat IFem pro p��pad p�id�v�n� (pohl�daj� si cykly ve vnit� algoritmu)
//	ColCount=bufColCount2;RowCount=bufRowCount2;
//	//DeleteTable();
//								//ShowMessage("2");
//	//vytvo�en� nov�ho realokovan�ho pole
//	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//ur�eno p�i dal�� realokaci pole, create toti� p�episuje buf hodnoty
//	Create(ColCount,RowCount);
//
//	//zkop�rov�n� p�vodn�ch hodnot zp�t
//	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je men��
//	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je men��
//
//	for(unsigned long Y=0;Y<bufRowCount;Y++)
//	{        ShowMessage(bufCells[0][Y].TopBorder->Width);
//		for(unsigned long X=0;X<bufColCount;X++)
//		{
//			//nelze cel� ukazatel
//			Cells[X][Y] = bufCells[X][Y];
//			////typ
//			Cells[X][Y].Font=new TFont();
//			Cells[X][Y].Background=new TBrush();//alokace pam�ti
//			CopyAreaCell(bufCells[X][Y],Cells[X][Y],true);//kopie vlastnost�, bez or�mov�n�
//			////or�mov�n�
//			CreateLinkBorder2(X,Y,bufCells[X][Y]);
//		}
//	}
//												//	ShowMessage("4");
//	//smaz�n� p�vodn�ho bufCells
//	for(unsigned long X=0;X<bufColCount2;X++)
//	{
//		for(unsigned long Y=0;Y<bufRowCount2;Y++)//po ��dc�ch
//		{
//			DeleteCell(bufCells[X][Y]);
//		}
//		bufCells[X]=NULL; delete bufCells[X];
//	}
//	bufCells=NULL; delete bufCells;
//	bufColCount=ColCount;bufRowCount=RowCount;//ur�eno p�i dal�� realokaci pole
//}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vytvo�enou komponentu dle Type, pokud existuje, tak se nic ned�je
void TmGrid::createComponent(Ttype Type, unsigned long Col,unsigned long Row)
{
	switch(Type)
	{
		case readEDIT:break;
		case EDIT:				createEdit(Col,Row);break;
		case NUMERIC:			createNumeric(Col,Row);break;
		case readNUMERIC:break;
		case BUTTON:		 	createButton(Col,Row);break;
		case COMBO:break;
		case CHECK:				createCheck(Col,Row);break;
		case RADIO:				createRadio(Col,Row);break;
		case DRAW:break;
  }
}
//---------------------------------------------------------------------------
//odstran� dynamicky vyto�en� komponenty, nutno volat p�ed Delete()
void TmGrid::DeleteComponents()
{
	DeleteComponents(0,0,ColCount-1,RowCount-1);
}
//---------------------------------------------------------------------------
//odstran� dynamicky vyto�en� komponenty od - do po�tu sloupc� a ��dk�, nutno volat p�ed Delete()
void TmGrid::DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow)
{
	for(unsigned long X=sCol;X<=fCol;X++)//po ��dc�ch
	{
		for(unsigned long Y=sRow;Y<=fRow;Y++)//po sloupc�ch
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
//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� ��dek
long TmGrid::GetIdxRow(int X,int Y)
{
	long RET=-1;
	if(X>=Left && X<Left+Width && Y>=Top && Y<Top+Height)//o�et�en� mimo tabulku + akceler�tor
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
//dle sou�adnic ve formul��i,kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� sloupec
long TmGrid::GetIdxColum(int X,int Y)
{
	long RET=-1;
	if(X>=Left && X<Left+Width && Y>=Top && Y<Top+Height)//o�et�en� mimo tabulku + akceler�tor
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
//zajist� zv�razn�n� ��dk� dle ��sla ��dku Row
void TmGrid::HighlightRow(long Row,TColor Color,bool SelFirstRow,bool unHighlightPrevRow)
{
	//pokud se pohybuje v tabulce
	int FirstRow=0;if(!SelFirstRow)FirstRow=1;
	if(Row>=FirstRow && preRowInd!=Row)
	{
		if(unHighlightPrevRow)if(!(Row==1 && preRowInd==0))selRow(preRowInd,Color,false);//odstran�n� p�edchoz�ho ozna�en� misto: Show();//Refresh s minimem probliku
		selRow(Row,Color,true);//nov� ozna�en�
	}
	//p�i odchodu z tabulky, aby nez�stal "viset" ozna�en� ��dek
	if(unHighlightPrevRow)
	{
		if(Row<=FirstRow-1 && preRowInd>=FirstRow)selRow(preRowInd,Color,false);//odstran�n� p�edchoz�ho ozna�en� misto: Show();//Refresh s minimem probliku
		preRowInd=Row;//ulo�en� aktivn�ho ��dku pro dal�� pou��t�
  }
}
//---------------------------------------------------------------------------
//zajist� zv�razn�n� ��dk�, p�es kter� se p�ej�d� my��
void TmGrid::HighlightRowOnMouse(int X,int Y,TColor Color,bool SelFirstRow,bool unHighlightPrevRow)
{
	long Row=GetIdxRow(X,Y);
	HighlightRow(Row,Color,SelFirstRow,unHighlightPrevRow);
}
//---------------------------------------------------------------------------
//ozna�� ��dek, nebo zru�� ozna�en� ��dku dle vstupn�ho parametru
void TmGrid::selRow(long Row,TColor Color,bool newSel)
{
	if(newSel)Form->Canvas->Pen->Mode=pmMask;else Form->Canvas->Pen->Mode=pmNotXor;
	Form->Canvas->Brush->Style=bsSolid;
	Form->Canvas->Brush->Color=Color;
	Form->Canvas->Pen->Width=0;
	Form->Canvas->Pen->Color=Color;
	short C=1-(short)newSel;

	TPoint body[4]={TPoint(mGrid->Left-C,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height),TPoint(mGrid->Left,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height)};
	Form->Canvas->Polygon(body,3);
	Form->Canvas->Pen->Mode=pmCopy;
}
//---------------------------------------------------------------------------
