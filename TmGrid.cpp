//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
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
	////nastaven� TABULKY
	Tag=0;//ID komponenty (vyu�iteln� nap�. pokud bude v�ce tabulek, tak se bude v�d�t, v jak� do�lo ke kliku)
	Left=0;Top=0;//um�st�n� cel� komponenty
	RowCount=0;ColCount=0;//po�et ��dk� a sloupc�
	DefaultColWidth=90,DefaultRowHeight=25;//v�choz� v��ka a ���ka ��dku
	Row=0;Col=0;//aktu�ln� ��dek a sloupec
	AntiAliasing_grid=false;
	AntiAliasing_text=true;
	MovingTable=false;
	VisibleComponents=true;
	SetColumnAutoFitColIdx=-3;//nastav� ���ku bun�k dan�ho sloupce dle parametru ColIdx, -3 = nep�izp�sobuje se velikost a u�ije se defaultColWidth,-2 v�echny sloupce stejn� podle nej�ir��ho textu, -1 p�izp�sobuje se ka�d� sloupec individu�ln�, 0 a v�ce jen konkr�tn� sloupec uveden� pomoc ColIdx
	preRowInd=-1;
	Decimal=3;//implicitn� po�et desetinn�ch m�st u numericedit�
	IntegerDecimalNull=false;//pokud je v��e uveden� Decimal na hodnotu vy��� ne� 0, toto nastavuje zda se nuly dopln� do po�tu decim�l i u cel�ch ��sel
	clHighlight=(TColor)RGB(0,120,215);//p�ednastaven� barva zv�razn�n�, slou�� i pro nastaven� barvy focusu komponent, p�vodn� tmav� RGB(43,87,154)
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
	DefaultCell.isNegativeNumber->Size=DefaultCell.Font->Size;
	DefaultCell.isNegativeNumber->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.isNegativeNumber->Orientation=0;
	DefaultCell.isNegativeNumber->Style=TFontStyles();
	DefaultCell.isNegativeNumber->Pitch=TFontPitch::fpVariable;//ka�d� p�smeno fontu stejn� �irok�
	DefaultCell.isNegativeNumber->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isNegativeNumber->Name=DefaultCell.Font->Name;
	*DefaultCell.isNegativeNumber=*DefaultCell.Font;
	*DefaultCell.isZero=*DefaultCell.Font;
	*DefaultCell.isLink=*DefaultCell.Font;
	DefaultCell.isLink->Size=DefaultCell.Font->Size;
	DefaultCell.isLink->Color=(TColor)RGB(43,87,154);//(TColor)RGB(128,128,128);
	DefaultCell.isLink->Orientation=0;
	DefaultCell.isLink->Style=TFontStyles();
	DefaultCell.isLink->Pitch=TFontPitch::fpVariable;//ka�d� p�smeno fontu stejn� �irok�
	DefaultCell.isLink->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.isLink->Name=DefaultCell.Font->Name;
	DefaultCell.TextPositon.X=0;
	DefaultCell.TextPositon.Y=0;
	DefaultCell.Text="";
	//pozice p��padn�ho linku
	DefaultCell.LinkCoordinateStart=TPoint(-1,-1);//kv�li ulo�en� citeln� oblasti pro link dan� bu�ky
	DefaultCell.LinkCoordinateEnd=TPoint(-1,-1);//kv�li ulo�en� citeln� oblasti pro link dan� bu�ky
	//zarovn�n�
	DefaultCell.Align=CENTER;
	DefaultCell.Valign=MIDDLE;
	DefaultCell.TopMargin=1;
	DefaultCell.BottomMargin=1;
	DefaultCell.LeftMargin=1;
	DefaultCell.RightMargin=1;
	//pouze indikuje, zda je bu�ka slou�ena, �i nikoliv, slou�� jako pom�cka p�i vykreslov�n� or�mov�n� slou�en�ch bun�k
	DefaultCell.MergeState=false;
	//pokud je nastaveno na true, nelze vepsat jinou hodnotu ne� ��selnou (to v�etn� re�ln�ho ��sla)
	DefaultCell.InputNumbersOnly=false;
	//v�choz� stav zv�razn�n� bu�ky
	DefaultCell.Highlight=false;
	//pozad�
	DefaultCell.Background->Color=clWhite;
	DefaultCell.Background->Style=bsSolid;
	DefaultCell.isEmpty->Color=clWhite;
	DefaultCell.isEmpty->Style=bsSolid;
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
			//Cells[X][Y]=DefaultCell; //- nelze, takto p�evezme cel� ukazatel a CreateCell(Cells[X][Y]); alokovalo Border znovu
			//alokace pam�ti
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();
			Cells[X][Y].isNegativeNumber=new TFont();
			Cells[X][Y].isZero=new TFont();
			Cells[X][Y].isLink=new TFont();
			Cells[X][Y].isEmpty=new TBrush();
			CopyAreaCell(DefaultCell,Cells[X][Y],true);//kopie vlastnost�, bez or�mov�n�
			////or�mov�n� bun�k ukazatelem
			CreateLinkBorder(X,Y,DefaultCell);
			////��DKY
			Rows[Y].Height=DefaultRowHeight;
			//zat�m nezprovozn�no Rows[Y].Visible=true;
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Y*DefaultRowHeight;else Rows[0].Top=0;
		}
		////SLOUPCE
		Columns[X].Width=DefaultColWidth;
		Columns[X].Visible=true;
		if(X>0){Columns[X].Left=Columns[X-1].Left+X*DefaultColWidth;}else Columns[0].Left=0;
	}
	bufColCount=ColCount;bufRowCount=RowCount;//ur�eno p�i dal�� realokaci pole
	preTop=Top;preLeft=Left;//zaloha �vodn� pozice
	deleteMark=false;//detekce �e doch�z� k odstra�ov�n� mGridu
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
		deleteMark=true;//detekce �e doch�z� k odstra�ov�n� mGridu
		//odstran�n� v tabulce pou�it�ch komponent
		DeleteComponents();
		//uvoln�n� pam�ti
		DeleteTable();
		DeleteCell(DefaultCell);
		mGrid=NULL; delete mGrid;
	}
	catch(...)
	{/*MessageBeep(0);*/}
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
void TmGrid::Show(TCanvas *Canvas)
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

		if(Canvas==NULL)Canvas=Form->Canvas;//pokud Canvas nen� definov�n, je p�edpokl�d�no kreslen� p��mo do Form, kde je mGrid zobrazov�n

		if(AntiAliasing_grid==false && AntiAliasing_text==false)Draw(Canvas);
		else
		{
			Cantialising a;
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=Width*3;bmp_in->Height=Height*3;//velikost canvasu//*3 vypl�v� z logiky algoritmu antialiasingu
			//bmp_in->Canvas->Brush->Color=clRed;
			//bmp_in->Canvas->FillRect(TRect(0,0,bmp_in->Width,bmp_in->Height));
			Draw(bmp_in->Canvas);
			Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutn� do samostatn� bmp, kv�li smaz�n� bitmapy vracen� AA
			Canvas->Draw(Left,Top,bmp_out);
			if(AntiAliasing_grid==false && AntiAliasing_text==true)DrawGrid(Canvas);//kresl� se a� nahoru
			delete (bmp_out);//velice nutn�
			delete (bmp_in);//velice nutn�
		}
		//zaloha �vodn� pozice
		preTop=Top;preLeft=Left;
	}
}
//---------------------------------------------------------------------------
//zajist� vyvol�n� p�ekreslen� cel� tabulky
void TmGrid::Refresh()
{
	 //Form->Repaint();// s p�eblikem, ale l�pe pou��vat p��mo ve v dan�m formul��i FormPaint(this), co� zajist� p�ekreslen� bez probliku
	 Show();
}
//---------------------------------------------------------------------------
void TmGrid::Update()
{
	if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k po�adavku na zm�nu po�tu ��dk� a sloupc� vol� se realokace
	{
		realock();
		Form->Refresh();
	}

	//nastaven� ���ky sloupc� a v��ky ��dk�+autofit sloupc� nastav� Columns[X].ColWidth
	SetColRow();//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby

	//vykresl� se do Canvasu bitmapy o nulov�ch rozm�rech, tj. nedojde k p�ekreslen�
	Graphics::TBitmap *bmp_temp=new Graphics::TBitmap;
	bmp_temp->Width=0;bmp_temp->Height=0;
	Draw(bmp_temp->Canvas);
	delete (bmp_temp);

	//zaloha �vodn� pozice
	preTop=Top;preLeft=Left;
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
		if(!Columns[X].Visible || Columns[X].Width==0){Columns[X].Width=0;continue;}//skryt� sloupce, zat�m ale nefunguje pat�i�n� or�mov�n�
		////oblast bu�ky
		TRect R;//grid
		TRect Rt;//text
		TRect Rb;//text
		TRect Rc;//componenty
		R.Left	=	Left+Columns[X].Left*Zoom_g;
		Rt.Left	=	Columns[X].Left*Zoom_b;//zde nen� Left cel� tabulky, proto�e se pozicije na pozici lev�ho horn�ho rohu tabulky cel� bmp, tak�e zde mus� b�t pouze sou�adnice v r�mci tabulku, nikoliv absolutn� v cel�m formu
		Rb.Left	=	Columns[X].Left*Zoom_b;//zde nen� Left cel� tabulky, proto�e se pozicije na pozici lev�ho horn�ho rohu tabulky cel� bmp, tak�e zde mus� b�t pouze sou�adnice v r�mci tabulku, nikoliv absolutn� v cel�m formu
		Rc.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_g;
		Rt.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;//zde nen� Left cel� tabulky, proto�e se pozicije na pozici lev�ho horn�ho rohu tabulky cel� bmp, tak�e zde mus� b�t pouze sou�adnice v r�mci tabulku, nikoliv absolutn� v cel�m formu
		Rb.Right	=	(Columns[X].Left+Columns[X].Width)*Zoom_b;//zde nen� Left cel� tabulky, proto�e se pozicije na pozici lev�ho horn�ho rohu tabulky cel� bmp, tak�e zde mus� b�t pouze sou�adnice v r�mci tabulku, nikoliv absolutn� v cel�m formu
		Rc.Right	=	Left+(Columns[X].Left+Columns[X].Width);

		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			//nen� dod�l�no ve vztahu s n�e uveden�m if(!Rows[Y].Visible || Rows[Y].Height==0){Rows[Y].Height=0;continue;}//skryt� ��dku, zat�m ale nefunguje pat�i�n� or�mov�n�
			////oblast a um�st�n� bu�ky
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//v�po�et horn�ho okraje bu�ky dle bu�ky p�edchoz�
			R.Top			=	Top+Rows[Y].Top*Zoom_g;
			Rt.Top		=	Rows[Y].Top*Zoom_b;//zde nen� Left cel� tabulky, proto�e se pozicije na pozici lev�ho horn�ho rohu tabulky cel� bmp, tak�e zde mus� b�t pouze sou�adnice v r�mci tabulku, nikoliv absolutn� v cel�m formu
			Rb.Top		=	Rows[Y].Top*Zoom_b;
			Rc.Top		=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_g;
			Rt.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;//zde nen� Left cel� tabulky, proto�e se pozicije na pozici lev�ho horn�ho rohu tabulky cel� bmp, tak�e zde mus� b�t pouze sou�adnice v r�mci tabulku, nikoliv absolutn� v cel�m formu
			Rb.Bottom	=	(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
			Rc.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

			////barva pozad� bu�ky
			//28.2.provizorn� fix if(Cells[X][Y].Text=="")C->Brush->Color=Cells[X][Y].isEmpty->Color;else //podm�n�n� form�tov�n�
			C->Brush->Color=Cells[X][Y].Background->Color;//pro aktivaci podm�n�n�ho form�tov�n� p�idat do else
			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(Rb);

			////komponenta v bu�ce
			SetComponents(C,Rc,Rt,X,Y,Cells[X][Y]);

			////or�mov�n� bu�ky
			//pou��v�m duplicitn� (k DrawGrid) zde, v p��pad� tot�ln� vypnut�ho AA nebo tot�ln� zapnut�ho AA, v takov�m p��pad� potom neb�� DrawGrid, DrawGrid b�� jenom v momentu AntiAliasing_text=false a AntiAliasing_grid=true
			if(!(AntiAliasing_grid==false && AntiAliasing_text==true))
			{
				DrawCellBorder(C,X,Y,R);
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
		if(!Columns[X].Visible || Columns[X].Width==0){Columns[X].Width=0;continue;}//skryt� sloupce, zat�m ale nefunguje pat�i�n� or�mov�n�
		R.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width);
		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			////um�st�n�
			//nen� otestov�no if(!Rows[Y].Visible || Rows[Y].Height==0){Rows[Y].Height=0;continue;}//skryt� ��dku, zat�m ale nefunguje pat�i�n� or�mov�n�
			R.Top			=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);
			////or�mov�n� bu�ky
			DrawCellBorder(C,X,Y,R);
		}
	}
}
//---------------------------------------------------------------------------
//zajist� vykreslen� or�mov�n� jen jedn� bu�ky
void TmGrid::DrawCellBorder(TCanvas *C,unsigned long X,unsigned long Y,TRect R)
{
	//top
	if(Cells[X][Y].TopBorder->Color!=Cells[X][Y].Background->Color)//kv�li slou�en�m bu�k�m
	{
		SetBorder(C,Cells[X][Y].TopBorder);
		if(Cells[X][Y].Type==DRAW && Cells[X][Y].Highlight || Y>0 && Cells[X][Y-1].Type==DRAW && Cells[X][Y-1].Highlight)C->Pen->Color=clHighlight;//v p��pad� highlightu or�mov�n�
		C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
	}
	//bottom
	if(Y==RowCount-1)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
	{
		SetBorder(C,Cells[X][Y].BottomBorder);
		if(Cells[X][Y].Type==DRAW && Cells[X][Y].Highlight)C->Pen->Color=clHighlight;//v p��pad� highlightu or�mov�n�
		C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
	}
	//left
	if(Cells[X][Y].LeftBorder->Color!=Cells[X][Y].Background->Color)//kv�li slou�en�m bu�k�m
	{
		SetBorder(C,Cells[X][Y].LeftBorder);
		if(Cells[X][Y].Type==DRAW && Cells[X][Y].Highlight || X>0 && Cells[X-1][Y].Type==DRAW && Cells[X-1][Y].Highlight)C->Pen->Color=clHighlight;//v p��pad� highlightu or�mov�n�
		C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
	}
	//right
	if(X==ColCount-1)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
	{
		SetBorder(C,Cells[X][Y].RightBorder);
		if(Cells[X][Y].Type==DRAW && Cells[X][Y].Highlight)C->Pen->Color=clHighlight;//v p��pad� highlightu or�mov�n�
		C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
	}
}
//---------------------------------------------------------------------------
//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby
void TmGrid::SetColRow()
{
	executeColumnsAutoFit(Form->Canvas);

	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		if(X!=0 && (!Columns[X-1].Visible || Columns[X-1].Width==0))Columns[X].Left=Columns[X-1].Left;//skryt� sloupce, pokud je p�echoz� skryt�, p�evezme aktu�ln� zpracov�van� (cyklem) pozici p�edchoz�ho
		else if(X>0)Columns[X].Left=Columns[X-1].Left+Columns[X-1].Width;else Columns[0].Left=0;//v�po�et lev�ho okraje bu�ky dle bu�ky p�edchoz�
	}
	for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
	{
		//nen� dod�l�no ve vztahu s n�e uveden�m if(!Rows[Y].Visible || Rows[Y].Height==0){Rows[Y].Height=0;continue;}else//skryt� ��dku, zat�m ale nefunguje pat�i�n� or�mov�n�
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
			/* //28.2.provizorn� fix
			int Orientation=Cell.Font->Orientation;
			if(F->m.null(F->ms.MyToDouble(Cell.Text))<0)Canv->Font=Cell.isNegativeNumber;//podm�n�n� form�tov�n�
			Cell.isZero->Color=clYellow;
			if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))Canv->Font=Cell.isZero;//podm�n�n� form�tov�n�
			Canv->Font->Orientation=Orientation;//mus� je�t� vr�tit orientaci pokud byla podm�n�n�m form�tov�n�m p�eps�na
			*/
			Canv->Font->Size*=Zoom;
			//SetBkMode(canv->Handle,OPAQUE);//nastaven� netransparentn�ho pozad�
			//28.2.provizorn� fix if(Cell.Text=="")Canv->Brush->Color=Cell.isEmpty->Color;else //podm�n�n� form�tov�n�//zde se asi nezohled�uje, sp�e v drawgrid, ale ot�zka je jak bez AA
			Canv->Brush->Color=Cell.Background->Color;//pro podm�n�n� form�tov�n� za�adit do v��e uveden� else v�tve
			Canv->Brush->Style=bsClear;//nastven� netransparentn�ho pozad�
			Canv->Font->Pitch = TFontPitch::fpFixed;//ka�d� p�smeno fontu stejn� �irok� - TEST
			Canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;//asi nep�in�� zcela p��nos - TEST
			//zarovn�n�
			//samotn� v�pis
			long L=Rt.Left,T=Rt.Top;
			int W=getWidthHeightText(Cell).X*Zoom;
			int H=getWidthHeightText(Cell).Y*Zoom;
			//short Rot=1;//slou�� jako pom�cka rotace
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
			unsigned int Pos=Cell.Text.Pos("<a>");//pozice html tagu
			if(Pos>0)//parsov�n� HTML
			{
				AnsiString T1=ms.TrimRightFrom(Cell.Text,"<a>");//AnsiString T1=Cell.Text.SubString(1,Pos-1);
				AnsiString Link=ms.EP(Cell.Text,"<a>","</a>");
				AnsiString T2=ms.TrimLeftFromText(Cell.Text,"</a>");
				Canv->TextOut(L,T,T1);
				short w=Canv->TextWidth(T1);
				Canv->Font=Cell.isLink;
				Canv->Font->Size*=Zoom;
				//Canv->Font->Color=Cell.isLink->Color;zaloha, d��ve kdyby v��e uveden� nefungovalo spr�vn�, mo�no smazat
				Canv->TextOut(L+w,T,Link);
				Cell.LinkCoordinateStart.x=Left+L/Zoom+w/Zoom;//kv�li citeln� oblasti pro link dan� bu�ky
				Cell.LinkCoordinateStart.y=Top+T/Zoom;//kv�li citeln� oblasti pro link dan� bu�ky
				Cell.LinkCoordinateEnd.y=Top+T/Zoom+Canv->TextHeight(Link)/Zoom;//kv�li citeln� oblasti pro link dan� bu�ky
				w+=Canv->TextWidth(Link);
				Cell.LinkCoordinateEnd.x=Left+L/Zoom+w/Zoom;//kv�li citeln� oblasti pro link dan� bu�ky
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
				Ttype TypeTemp=Cell.Type;Cell.Type=DRAW;//nastav� jin� typ, ale jen provizorn�
				SetComponents(Canv,R,Rt,X,Y,Cell);
				Cell.Type=TypeTemp;//navr�cen� p�edchoz�ho typu
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
				Ttype TypeTemp=Cell.Type;Cell.Type=DRAW;//nastav� jin� typ, ale jen provizorn�
				SetComponents(Canv,R,Rt,X,Y,Cell);
				Cell.Type=TypeTemp;//navr�cen� p�edchoz�ho typu
			}
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
//nastav� danou bu�ku na edit, pomocn� metoda v��e uveden�
void TmGrid::SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPEdit *E=createEdit(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��

	//atributy
	if(Cell.Type==EDIT)E->Enabled=true;else E->Enabled=false;
	if(!VisibleComponents || MovingTable)E->Visible=false;else E->Visible=true;//p�i posunu tabulky se skryje EDIT a je m�sto n�j DRAW
	E->AutoSize=false;
	E->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);//ubere velikost komponenty podle ���ky or�mov�n�
	E->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);//ubere velikost komponenty podle ���ky or�mov�n�
	if(Cell.MergeState==false)E->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);   //pokud neplat� nastavuje se p��mo v mergovan�, ubere pouze velikost komponenty podle ���ky or�mov�n�
	/*if(Cell.MergeState==false)*/E->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dod�lat ubere velikost komponenty podle ���ky or�mov�n�//ubere velikost komponenty podle ���ky or�mov�n�
	//E->ShowHint=false;//toto by bylo v�dy u editu na false, pokus automatizace pro dlouh� textif(Cell.Text.Length()>E->Width/(Cell.Font->Size-2))E->ShowHint=true;else //asi nep�esn�
	E->Hint=Cell.Text;//v�choz� text pro hint je hodnota z editu
	if(Cell.Text=="")E->Options->NormalColor=Cell.isEmpty->Color;else E->Options->NormalColor=Cell.Background->Color;
	E->Options->NormalColorAlpha=255;
	if(!Cell.Highlight)E->Options->FrameNormalColor=Cell.Background->Color;//r�me�ek mus� b�t stejnou barvou jakou bu�ka, proto�e m��ka je o 1px na v�echny strany rozta�en�
	else E->Options->FrameNormalColor=clHighlight;
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
	//28.2.provizorn� fix if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))E->Font=Cell.isNegativeNumber;//podm�n�n� form�tov�n�
	//28.2.provizorn� fix if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))E->Font=Cell.isZero;//podm�n�n� form�tov�n�
	//if(!E->Focused())//pokud nen� na bu�ce focus resp. nen� aktivn� - provizorn� odstaveno, zd� se, �e nem� na nic vliv
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
	if(!VisibleComponents || MovingTable)N->Visible=false;else N->Visible=true;//p�i posunu tabulky se skryje EDIT a je m�sto n�j DRAW
	N->AutoSize=false;
	N->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);//ubere velikost komponenty podle ���ky or�mov�n�
	N->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);//ubere velikost komponenty podle ���ky or�mov�n�
	if(Cell.MergeState==false)N->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);	 //pokud neplat� nastavuje se p��mo v mergovan�, ubere pouze velikost komponenty podle ���ky or�mov�n�
	/*if(Cell.MergeState==false)*/N->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dod�lat ubere velikost komponenty podle ���ky or�mov�n�
	N->Decimal=Decimal;
	//if(!N->Focused() && !IntegerDecimalNull && m.cele_cislo(ms.MyToDouble(Cell.Text)))N->Decimal=0;//pokud se jedn� o cel� ��slo, nezobrazuje "re�lnou ��st" cel�ho ��sla tj. po�et nul do po�tu decimal, nastavuje se pouze pokud nen� dan� NUMERIC editovan�
	N->DisplayType=scedtNumeric;
	N->ValueType=scvtFloat;
	N->ShowHint=true;
	N->Hint=Cell.Text;
	//28.2.provizorn� fix if(Cell.Text=="")N->Options->NormalColor=Cell.isEmpty->Color;else //podm�n�n� form�tov�n�
	N->Options->NormalColor=Cell.Background->Color;
	N->Options->NormalColorAlpha=255;
	if(!Cell.Highlight)N->Options->FrameNormalColor=Cell.Background->Color;//r�me�ek mus� b�t stejnou barvou jakou bu�ka, proto�e m��ka je o 1px na v�echny strany rozta�en�
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
	//28.2.provizorn� fix if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))N->Font=Cell.isNegativeNumber;//podm�n�n� form�tov�n�
	//28.2.provizorn� fix if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))N->Font=Cell.isZero;//podm�n�n� form�tov�n�
	//if(!N->Focused())//pokud je na bu�ce focus resp. je aktivn� - provizorn� odstaveno, zd� se, �e nem� na nic vliv
	N->Value=ms.MyToDouble(Cell.Text);

	//vlastn�k
	N->Parent=Form;//mus� b�t a� na konci
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
void TmGrid::SetLabel(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscHTMLLabel *L=createLabel(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��

	//atributy
	L->Visible=VisibleComponents;
	L->AutoSizeHeight=false;L->AutoSizeWidth=false;
	L->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);//ubere velikost komponenty podle ���ky or�mov�n�
	L->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);//ubere velikost komponenty podle ���ky or�mov�n�
	if(Cell.MergeState==false)L->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);	 //pokud neplat� nastavuje se p��mo v mergovan�, ubere pouze velikost komponenty podle ���ky or�mov�n�//pokud neplat� nastavuje se p��mo v mergovan�
	/*if(Cell.MergeState==false)*/L->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dod�lat ubere velikost komponenty podle ���ky or�mov�n�//dod�lat//ubere velikost komponenty podle ���ky or�mov�n�
	L->ShowHint=false;//implicitn� u editu na false, pokus pro dlouh� textif(Cell.Text.Length()>E->Width/(Cell.Font->Size-2))E->ShowHint=true;else //asi nep�esn�
	L->Hint=Cell.Text;//v�choz� text pro hint je hodnota z editu
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

	//text
	L->Font=Cell.Font;
	//28.2.provizorn� fix if(F->m.null(F->ms.MyToDouble(Cell.Text)<0))L->Font=Cell.isNegativeNumber;//podm�n�n� form�tov�n�
	//28.2.provizorn� fix if(F->m.null(F->ms.MyToDouble(Cell.Text))==0 && F->ms.IsNumber(Cell.Text))L->Font=Cell.isZero;//podm�n�n� form�tov�n�
	L->Caption=Cell.Text;

	//vlastn�k
	L->Parent=Form;//mus� b�t a� na konci
	L=NULL;delete L;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na Button, pomocn� metoda objednu v��e uveden�
void TmGrid::SetButton(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPButton *B=createButton(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��

	//atributy
	if(!VisibleComponents || MovingTable)B->Visible=false;else B->Visible=true;//p�i posunu tabulky se skryje, zat�m se m�sto n�j nic nevykresluje
	B->Top=R.Top+ceil(Cell.TopBorder->Width/2.0)+1;
	B->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0)+1;
	B->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0)-1;
	B->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0)-1;
	//B->Options->NormalColor=Cell.Background->Color; nechat
	B->Options->FrameNormalColor=B->Options->NormalColor;

	//text
	B->Font=Cell.Font;
	B->Caption=Cell.Text;

	//vlastn�k
	B->Parent=Form;//mus� b�t a� na konci
	B=NULL;delete B;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na GlyphButton, pomocn� metoda objednu v��e uveden�
void TmGrid::SetGlyphButton(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPGlyphButton *gB=createGlyphButton(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��

	//atributy
	if(!VisibleComponents || MovingTable)gB->Visible=false;else gB->Visible=true;//p�i posunu tabulky se skryje, zat�m se m�sto n�j nic nevykresluje
	gB->Top=R.Top+floor(Cell.TopBorder->Width/2.0)+1;
	gB->Left=R.Left+floor(Cell.LeftBorder->Width/2.0)+1;
	gB->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-floor(Cell.LeftBorder->Width/2.0)-1;
	gB->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-floor(Cell.TopBorder->Width/2.0)-1;
	//gB->Options->NormalColor=Cell.Background->Color; nechat zakomentovan�
	gB->Options->FrameNormalColor=gB->Options->NormalColor;

	//text
	gB->Font=Cell.Font;
	gB->Caption=Cell.Text;

	//vlastn�k
	gB->Parent=Form;//mus� b�t a� na konci
	gB=NULL;delete gB;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na ComboBox, pomocn� metoda objednu v��e uveden�
void TmGrid::SetCombo(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPComboBox *C=createCombo(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��

	//atributy - ty kter� se p�i ka�d�m refresh nastav�
	C->Top=R.Top+ceil(Cell.TopBorder->Width/2.0);
	C->Left=R.Left+ceil(Cell.LeftBorder->Width/2.0);
	if(Cell.MergeState==false)C->Width=Columns[X].Width-floor(Cell.RightBorder->Width/2.0)-ceil(Cell.LeftBorder->Width/2.0);   //pokud neplat� nastavuje se p��mo v mergovan�, ubere pouze velikost komponenty podle ���ky or�mov�n�
	/*if(Cell.MergeState==false)*/C->Height=Rows[Y].Height-floor(Cell.BottomBorder->Width/2.0)-ceil(Cell.TopBorder->Width/2.0);//dod�lat ubere velikost komponenty podle ���ky or�mov�n�
	if(!VisibleComponents || MovingTable)
	{   //rozpracovano p��padn� bitmapov�n� kompomentny p�i p�esunu obrazu
//		TBitmap *bmp=new Graphics::TBitmap();
//		bmp->Width=C->Width;bmp->Height=C->Height;
//		bmp->Canvas->CopyRect(Rect(0,0,C->Width,C->Height),Form->Canvas,Rect(C->Left,C->Top,C->Left+C->Width,C->Top+C->Height));//ulo�� pan v��ez
//		bmp->SaveToFile("test.bmp");
		C->Visible=false;
	}
	else C->Visible=true;//p�i posunu tabulky se skryje, zat�m se m�sto n�j nic nevykresluje

	//voliteln� atributy
	if(!Cell.Highlight)C->Options->FrameNormalColor=Cell.Background->Color;//r�me�ek mus� b�t stejnou barvou jakou bu�ka, proto�e m��ka je o 1px na v�echny strany rozta�en�
	else C->Options->FrameNormalColor=clHighlight;

	C->Options->NormalColor=Cell.Background->Color;
	C->Options->FrameFocusedColor=clHighlight;
	C->Options->FocusedColor=Cell.Background->Color;
	C->Options->HotColor=Cell.Background->Color;
  C->Options->PressedColor=Cell.Background->Color;

	//font
	C->Font=Cell.Font;
	C->Options->FontNormalColor=Cell.Font->Color;
	//C->ItemIndex=1;//nelze p�edozna�n� prvn� polo�ku

	//vlastn�k
	C->Parent=Form;//mus� b�t a� na konci
	C=NULL;delete C;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na Checkbox, pomocn� metoda objednu v��e uveden�
void TmGrid::SetCheck(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPCheckBox *Ch = createCheck(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��

	//atributy
	if(!VisibleComponents || MovingTable)Ch->Visible=false;else Ch->Visible=true;//p�i posunu tabulky se skryje, zat�m se m�sto n�j nic nevykresluje
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
	//p�esunout sp�e do create Ch->Options->FrameNormalColor=(TColor)RGB(128,128,128);//Ch->Font->Color;
	//p�esunout sp�e do create Ch->Options->FrameNormalColorAlpha=255;

	//text
	Ch->Font=Cell.Font;
	Ch->Caption=Cell.Text;

	//vlastn�k
	Ch->Parent=Form;//mus� b�t a� na konci
	Ch=NULL;delete Ch;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na radio, pomocn� metoda objednu v��e uveden�
void TmGrid::SetRadio(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPRadioButton *Ra = createRadio(X,Y);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
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
	Ra->Font=Cell.Font;
	Ra->Caption=Cell.Text;
	//vlastn�k
	Ra->Parent=Form;//mus� b�t a� na konci
	Ra=NULL;delete Ra;
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
		E->Name="mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(E->Tag);

		//ud�losti
		E->OnClick=&getTagOnClick;
		E->OnEnter=&getTagOnEnter;
		E->OnChange=&getTagOnChange;
		E->OnKeyDown=&getTagOnKeyDown;
		E->OnKeyPress=&getTagOnKeyPress;
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
		N->Name="mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(N->Tag);
		//ud�losti
		N->OnClick=&getTagOnClick;
		N->OnEnter=&getTagOnEnter;
		N->OnChange=&getTagOnChange;
		N->OnKeyDown=&getTagOnKeyDown;
		N->OnKeyPress=&getTagOnKeyPress;
	}
	return N;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscHTMLLabel *TmGrid::createLabel(unsigned long Col,unsigned long Row)
{
	TscHTMLLabel *L=getLabel(Col,Row);//pokud ji� existuje
	if(L==NULL)//pokud ne, tak zalo��
	{
		L = new TscHTMLLabel(Form);
		L->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		L->Name="mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(L->Tag);

		//ud�losti
		L->OnClick=&getTagOnClick;
		L->OnEnter=&getTagOnEnter;
	}
	return L;
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
		B->Name="mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(B->Tag);

		//ud�losti
		B->OnClick=&getTagOnClick;
		B->OnEnter=&getTagOnEnter;
	}
	return B;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPGlyphButton *TmGrid::createGlyphButton(unsigned long Col,unsigned long Row)
{
	TscGPGlyphButton *gB=getGlyphButton(Col,Row);//pokud ji� existuje
	if(gB==NULL)//pokud ne, tak zalo��
	{
		gB = new TscGPGlyphButton(Form);
		gB->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		gB->Name="mGrid_GlyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(gB->Tag);

		//ud�losti
		gB->OnClick=&getTagOnClick;
		gB->OnEnter=&getTagOnEnter;
	}
	return gB;
}
//---------------------------------------------------------------------------
//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
TscGPComboBox *TmGrid::createCombo(unsigned long Col,unsigned long Row)
{
	TscGPComboBox *C=getCombo(Col,Row);//pokud ji� existuje
	if(C==NULL)//pokud ne, tak zalo��, pouze poprv�, kdy� neexistuje
	{
		C=new TscGPComboBox(Form);//zalo��
		C->Tag=getTag(Col,Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		C->Name="mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(C->Tag);

		//ud�losti
		C->OnClick=&getTagOnClick;
		C->OnEnter=&getTagOnEnter;
		C->OnChange=&getTagOnChange;

		//v�choz� nastaven�
		C->Options->NormalColorAlpha=255;
		C->Options->FocusedColorAlpha=255;
		C->Options->FrameFocusedColorAlpha=255;
		C->Options->HotColorAlpha=255;
	}
	return C;
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
		Ch->Name="mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(Ch->Tag);

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
		Ra->Name="mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(Ra->Tag);

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
		////nastaven� velikosti sloupce dle ru�n� nastaven� ���ky dan�ho sloupce, tj. ned�lat autofit
		case -4:
		{
		}
		break;
		////defualtn� ���ka
		case -3:
		{
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=DefaultColWidth;
		}
		break;
		////v�echny sloupce stejn� podle nej�ir��ho sloupce
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
			//z�pis do pole ���ky sloupc�
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].Width=MaxColWidth;
		}
		break;
		////v�echny sloupce zarovnat individu�ln� dle ka�d�ho sloupce
		case -1:
		{
			for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
			executeColumnAutoFit(Canv,X);
		}
		break;
		////pouze konkr�tn� 0++
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
	Columns[ColIdx].Width=GetRecommendedColumnWidth(ColIdx);//nalezne nej�ir�� polo�ku a podle toho cel� sloupec i zarovn�
}
//---------------------------------------------------------------------------
//vrat� doporu�enou ���ku sloupce dle jeho obsahu
int TmGrid::GetRecommendedColumnWidth(long ColIdx)
{
	unsigned int ColWidth=getWidthHeightText(Cells[ColIdx][0]).X;//v�choz� hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		short Zoom=1;if(AntiAliasing_text==true)Zoom=3;
		unsigned int W=getWidthHeightText(Cells[ColIdx][Y]).X+Cells[ColIdx][Y].LeftMargin*Zoom+Cells[ColIdx][Y].LeftBorder->Width*Zoom/2+Cells[ColIdx][Y].RightMargin*Zoom+Cells[ColIdx][Y].RightBorder->Width*Zoom/2;
		if(Cells[ColIdx][Y].Type==CHECK || Cells[ColIdx][Y].Type==RADIO)W+=20+4+4;
		if(W>ColWidth)ColWidth=W;//najde nej�i���
	}
	return ColWidth+4;//+4 pouze okrajov� offset
}
//---------------------------------------------------------------------------
//vrat� doporu�enou ���ku sloupce dle dodan�ho textu a aktu�ln�ho nastaven� canvasu
int TmGrid::GetRecommendedColumnWidth(AnsiString Text)
{
	return Form->Canvas->TextWidth(Text)+4;//+4 pouze okrajov� offset;
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
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPGlyphButton *TmGrid::getGlyphButton(unsigned long Col,unsigned long Row){return (TscGPGlyphButton *)Form->FindComponent("mGrid_GlyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));};//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscHTMLLabel *TmGrid::getLabel(unsigned long Col,unsigned long Row){return (TscHTMLLabel *)Form->FindComponent("mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(getTag(Col,Row)));};//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
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
	if(!deleteMark)//detekce �e nedoch�z� k odstra�ov�n� mGridu, p�itom nesm� k ud�losti doch�zet
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
	if(!deleteMark)//detekce �e nedoch�z� k odstra�ov�n� mGridu, p�itom nesm� k ud�losti doch�zet
	{
		//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
		bufText=Cells[Col][Row].Text;//ukl�d� v�choz� hodnotu editboxu p�ed psan�m, pro p��pad stisku ESC
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
	if(!deleteMark)//detekce �e nedoch�z� k odstra�ov�n� mGridu, p�itom nesm� k ud�losti doch�zet
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
    if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnChange(Tag,ID,Col,Row);//z unit1 do unitX
    if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnChange(Tag,Col,Row);
	}
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnKeyDown(TObject *Sender,WORD &Key, TShiftState Shift)
{
	if(!deleteMark)//detekce �e nedoch�z� k odstra�ov�n� mGridu, p�itom nesm� k ud�losti doch�zet
	{
//		Col=getColFromTag(((TComponent*)(Sender))->Tag);
//		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
//		getTextFromComponentToMemoryCell(Col,Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� z dan� komponenty text do pam�ov� bu�ky, slou�� nap�. p�i ud�losti onchange pop�. d�l��ch
//    POZOR!!!, V P�IPAD� U�IT� N�KTER�CH N͎E UVEDEN�CH VOL�N�, P��PADN� DAL��CH P�IDAN�CH NUTNO ODKOMENTOVAT PRVN� T�I V݊E UVEDEN� ��DKY
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
	if(!deleteMark)//detekce �e nedoch�z� k odstra�ov�n� mGridu, p�itom nesm� k ud�losti doch�zet
	{
		//adresace + nezbytn� pln�n� z Editu do pam�ti!!!
		Col=getColFromTag(((TComponent*)(Sender))->Tag);
		Row=getRowFromTag(((TComponent*)(Sender))->Tag);
		if(Key==VK_ESCAPE)//v p��pad� stisku ESC n�vrat p�vodn�ho textu
		{
			TscGPEdit *E=getEdit(Col,Row);//->Sen //Text=bufText;//Cells[Col][Row].Text=bufText;
			TscGPNumericEdit *N=getNumeric(Col,Row);
			if(E!=NULL){E->Text=bufText;E->SelectAll();}E=NULL;delete E;
			if(N!=NULL){N->Value=ms.MyToDouble(bufText);E->SelectAll();}N=NULL;delete N;
		}
		getTextFromComponentToMemoryCell(Col,Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� z dan� komponenty text do pam�ov� bu�ky, slou�� nap�. p�i ud�losti onchange pop�. d�l��ch

		//filtr kl�ves
		if(Cells[Col][Row].InputNumbersOnly && Key!=VK_ESCAPE)Key=ms.numericFilter(Cells[Col][Row].Text,Key);//pokud je nastaveno na true a nen� stisknuta kl�vesa backspace, nelze vepsat jinou hodnotu ne� ��selnou (to v�etn� re�ln�ho ��sla)

		//namapov�n� dce�inn�ch ud�lost� - odkomentovat pat�i�nou + p��padn� upravit po�adovan� parametry
		//if(AnsiString(Tag).SubString(1,1)=="1")F_gapoTT->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="2")F_gapoV->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="3")F_gapoR->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="4")Form2->OnKeyPress(Tag,ID,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="5")Form_poznamky->OnKeyPress(Tag,Col,Row,Key);
		//if(AnsiString(Tag).SubString(1,1)=="6")FormX->OnKeyPress(Tag,ID,Col,Row,Key);//z unit1 do unitX
		//if(AnsiString(Tag).SubString(1,1)=="7")Form_parametry_linky->OnKeyPress(Tag,ID,Col,Row,Key);
	}
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
		Cells[ColCell_1][RowCell_1].MergeState=true;//ozna�� bu�ku jako slou�enou, slou�� pro p�esko�en� vykreslov�n� or�mov�n�, uprost�ed slou�en�ch objekt� ale tak� kv�li nastaven� ���ky komponent

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
						case COMBO:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPComboBox *C=createCombo(ColCell_1,RowCell_1);
							C->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							C->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)C->Width-=floor(Border.Width/2.0);else C->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//je�t� or�mov�n�
							C=NULL;delete C;
						}break;
						case EDIT:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPEdit *E=createEdit(ColCell_1,RowCell_1);
							E->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							E->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)E->Width-=floor(Border.Width/2.0);else E->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//je�t� or�mov�n�
							E->Text=RefCell.Text;
							E=NULL;delete E;
						}break;
						case NUMERIC:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscGPNumericEdit *N=createNumeric(ColCell_1,RowCell_1);
							N->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							N->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)N->Width-=floor(Border.Width/2.0);else N->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//je�t� or�mov�n�
							N->Value=ms.MyToDouble(RefCell.Text);//bere a� z posledn� bu�ky slu�ovan� oblasti
							N=NULL;delete N;
						}break;
						case LABEL:
						{
							Cells[ColCell_1][RowCell_1].Align=CENTER;
							TscHTMLLabel *L=createLabel(ColCell_1,RowCell_1);
							L->Width=Columns[ColCell_2].Left+Columns[ColCell_2].Width-Columns[ColCell_1].Left;
							L->Width-=ceil(Cells[ColCell_1][RowCell_1].LeftBorder->Width/2.0);if(ColCell_2==ColCount-1)L->Width-=floor(Border.Width/2.0);else L->Width-=floor(Cells[ColCell_2][RowCell_2].RightBorder->Width/2.0);//je�t� or�mov�n�
							L->Caption=RefCell.Text;//bere a� z posledn� bu�ky slu�ovan� oblasti
							L=NULL;delete L;
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
						case COMBO:
						{
							Cells[ColCell_1][RowCell_1].Valign=MIDDLE;
							TscGPComboBox *C=createCombo(ColCell_1,RowCell_1);
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
//zajist� trval� (jedn� se sp�e o nastaven�) zv�razn�n� vn�j��ho or�mov�n� bu�ky
void TmGrid::HighlightCell(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width,bool Refresh)
{
// pro p��pad zkonkretizovan� po�adovvku odkomentovat
//	switch(Cells[Col][Row].Type)
//	{
//		case DRAW:
//		{
			TBorder hlBorder;
			hlBorder.Color=Color;
			hlBorder.Width=Width;
			hlBorder.Style=psSolid;
			*Cells[Col][Row].TopBorder=hlBorder;
			*Cells[Col][Row].LeftBorder=hlBorder;
			*Cells[Col][Row].RightBorder=hlBorder;
			*Cells[Col][Row].BottomBorder=hlBorder;
			if(Refresh)Show();
//		}break;
//		case EDIT:HighlightEdit(Col,Row,Color,Width);break;
//		case NUMERIC:HighlightNumeric(Col,Row,Color,Width);break;
//	}
}
//---------------------------------------------------------------------------
//zajist� trval� (jedn� se sp�e o nastaven�) zv�razn�n� dan� komponenty
void TmGrid::HighlightEdit(TscGPEdit *Edit,TColor Color,unsigned short Width)
{
	Edit->Options->FrameNormalColor=Color;
	Edit->Options->FrameWidth=Width;
}
//---------------------------------------------------------------------------
//zajist� trval� (jedn� se sp�e o nastaven�) zv�razn�n� dan� komponenty
void TmGrid::HighlightEdit(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width)
{
	HighlightEdit(getEdit(Col,Row),Color,Width);
}
//---------------------------------------------------------------------------
//zajist� trval� (jedn� se sp�e o nastaven�) zv�razn�n� dan� komponenty
void TmGrid::HighlightNumeric(TscGPNumericEdit *Numeric,TColor Color,unsigned short Width)
{
	Numeric->Options->FrameNormalColor=Color;
	Numeric->Options->FrameWidth=Width;
}
//---------------------------------------------------------------------------
//zajist� trval� (jedn� se sp�e o nastaven�) zv�razn�n� dan� komponenty
void TmGrid::HighlightNumeric(unsigned long Col,unsigned long Row,TColor Color,unsigned short Width)
{
	HighlightNumeric(getNumeric(Col,Row),Color,Width);
}
//---------------------------------------------------------------------------
//odzv�razni v�echna zv�razn�n�
void TmGrid::unHighlightAll()
{
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			Cells[X][Y].Highlight=false;
		}
	}
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
	//dal�� vlastnosti
	CopyCell.InputNumbersOnly=RefCell.InputNumbersOnly;
	CopyCell.Highlight=RefCell.Highlight;
	////pozad�
	//*CopyCell.Background=*RefCell.Background;  - asi nejede
	CopyCell.Background->Color=RefCell.Background->Color;
	CopyCell.Background->Style=RefCell.Background->Style;
	CopyCell.isEmpty->Color=RefCell.isEmpty->Color;
	CopyCell.isEmpty->Style=RefCell.isEmpty->Style;
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
void TmGrid::InsertRow(unsigned long Row,bool copyComponentFromPreviousRow, bool invalidate)
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
			//Show();net�eba //Form->Invalidate();//t�eba
			Show();
		}
	}
	else AddRow(copyComponentFromPreviousRow,invalidate);
}
//---------------------------------------------------------------------------
//sma�e cel� ��dek, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody InsertRow p�imo v u�it�m formul��i
void TmGrid::DeleteRow(unsigned long Row,bool invalidate)
{
	if(Row<=RowCount-1 && RowCount-1>0)//nelze smazat nult� ��dek
	{
		DeleteComponents(0,Row,ColCount-1,Row);//new 15.4. - odmaz�n� komponent na aktu�ln�m ��dku, pozor, je nutn� odevzdat focus mimo mazan� komponenty, jinak nastane pam�ov� chyba
		//p�ekop�ruje ��dek resp. bu�ky z ��dku n�sleduj�c�ho a ubere posledn� ��dek, pokud se nejedn� o jedin� ��dek
		for(unsigned long Y=Row;Y<RowCount-1;Y++)
		{
			for(unsigned long X=0;X<ColCount;X++)
			{
				CopyCell(Cells[X][Y+1],Cells[X][Y],true);
				MoveComponentUP(X,Y+1);//15.4. test v p��pad� zru�en� vr�tit n�e uveden�  DeleteComponents
			}
			Rows[Y]=Rows[Y+1];
		}
		//15.4. ji� je nadbyte�n� DeleteComponents(0,RowCount-1,ColCount-1,RowCount-1);//sma�e komponenty z posledn�ho ��dku, pozor, je nutn� odevzdat focus mimo mazan� komponenty, jinak nastane pam�ov� chyba
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
	TColumns *bufColumns = new TColumns[bufColCount];//vytvo�� dynamick� pole ukazatelu typu TColumns kv�li z�loze ���ky sloupc�
	TRows *bufRows = new TRows[bufRowCount];//vytvo�� dynamick� pole ukazatelu typu TRows kv�li z�loze v��ky sloupc�

	for(unsigned long X=0;X<bufColCount;X++)
	{
		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupc�m dynamamick�ho pole alokuje pam�t pro jednotliv� ��dky- cyklus mus� b�t samostatn�
		bufCells[X] = Cells[X];//zkop�rov�n� p�vodn�ch hodnot do z�lohy //takto asi nevhodn� kop�rovat (z�stane ukazatel), s hv�zdickou sice hodnoty ale je pot�eba p�id�lit pam�t pomoc� new pro TBrush TFont a TBorders
		bufColumns[X].Width=Columns[X].Width;
	}

	for(unsigned long Y=0;Y<bufRowCount;Y++)bufRows[Y].Height=Rows[Y].Height;

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
		Rows[Y].Height=bufRows[Y].Height;//navr�cen� p�vodn� v��ky ��dku, proto�e vol�n�m Create se nastavuje v��ka v�choz�
		for(unsigned long X=0;X<bufColCount;X++)
		{
			//nelze cel� ukazatel
			Cells[X][Y] = bufCells[X][Y];//pop�. CopyCell(bufCells[X][Y],Cells[X][Y],true);//nov� verze
			if(Y==0)Columns[X].Width=bufColumns[X].Width;//navr�cen� p�vodn� ���ky slopc�, proto�e vol�n�m Create se nastavuje ���ka v�choz�
		}
	}
	bufColumns=NULL; delete bufColumns;
	bufRows=NULL; delete bufRows;

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
		case glyphBUTTON:	createGlyphButton(Col,Row);break;
		case COMBO:				createCombo(Col,Row); break;
		case CHECK:				createCheck(Col,Row);break;
		case RADIO:				createRadio(Col,Row);break;
		case DRAW:break;
		case LABEL:				createLabel(Col,Row);break;
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
// pozor odstra�ovan� komponenta nesm� m�t focus (jinak pam�ov� chyba), focus je pot�eba p�i odstra�ov�n� komponent odevzdat n�jak� komponent�, kter� z�st�v� ve formu
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
				case CHECK:{TscGPCheckBox *CH=getCheck(X,Y);CH->Free();CH=NULL;delete CH;break;} ///*CH->DisposeOf();*/ ani toto ani free p�i kliku nefunguj� spr�vn�, chyba byla, �e dan� komponenta m�la focus, focus je pot�eba p�i odstra�ov�n� komponent odevzdat n�jak� komponent�, kter� z�st�v� ve formu
				case RADIO:{TscGPRadioButton *R=getRadio(X,Y);R->Free();R=NULL;delete R;}break;
				case glyphBUTTON:{TscGPGlyphButton *gB=getGlyphButton(X,Y);gB->Free();gB=NULL;delete gB;}break;
			}
		}
	}
}
//---------------------------------------------------------------------------
//posunu komponentu na ��dku o jeden ��dek n�e, pou��v� se nov� na maz�n� ��dku
void TmGrid::MoveComponentUP(unsigned long Col,unsigned long Row)
{
	Row--;//bude se p�esouvat na n힚� ��dek
	switch(Cells[Col][Row+1].Type)
	{
		case readEDIT:
		case EDIT:
		{
			TscGPEdit *E=getEdit(Col,Row+1);
			E->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			E->Name="mGrid_EDIT_"+AnsiString(ID)+"_"+AnsiString(E->Tag);//p�eindexov�n� na ��dek n�e
			E=NULL;delete E;
		}
		break;
		case readNUMERIC:
		case NUMERIC:
		{
			TscGPNumericEdit *N=getNumeric(Col,Row+1);
			N->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			N->Name="mGrid_NUMERIC_"+AnsiString(ID)+"_"+AnsiString(N->Tag);//p�eindexov�n� na ��dek n�e
			N=NULL;delete N;
		}
		break;
		case BUTTON:
		{
			TscGPButton *B=getButton(Col,Row+1);
			B->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			B->Name="mGrid_BUTTON_"+AnsiString(ID)+"_"+AnsiString(B->Tag);//p�eindexov�n� na ��dek n�e
			B=NULL;delete B;
		}
		break;
		case LABEL:
		{
			TscHTMLLabel *L=getLabel(Col,Row+1);
			L->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			L->Name="mGrid_LABEL_"+AnsiString(ID)+"_"+AnsiString(L->Tag);//p�eindexov�n� na ��dek n�e
			L=NULL;delete L;
		}
		break;
		case glyphBUTTON:
		{
			TscGPGlyphButton *gB=getGlyphButton(Col,Row+1);
			gB->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			gB->Name="mGrid_glyphBUTTON_"+AnsiString(ID)+"_"+AnsiString(gB->Tag);//p�eindexov�n� na ��dek n�e
			gB=NULL;delete gB;
		}
		break;
		case COMBO:
		{
			TscGPComboBox *C=getCombo(Col,Row+1);
			C->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			C->Name="mGrid_COMBO_"+AnsiString(ID)+"_"+AnsiString(C->Tag);//p�eindexov�n� na ��dek n�e
			C=NULL;delete C;
		}
		break;
		case CHECK:
		{
			TscGPCheckBox *Ch=getCheck(Col,Row+1);
			Ch->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			Ch->Name="mGrid_CHECK_"+AnsiString(ID)+"_"+AnsiString(Ch->Tag);//p�eindexov�n� na ��dek n�e
			Ch=NULL;delete Ch;
		}
		break;
		case RADIO:
		{
			TscGPRadioButton *R=getRadio(Col,Row+1);
			R->Tag=getTag(Col,Row);//p�eindexov�n� na ��dek n�e
			R->Name="mGrid_RADIO_"+AnsiString(ID)+"_"+AnsiString(R->Tag);//p�eindexov�n� na ��dek n�e
			R=NULL;delete R;
		}
		break;
	}
  //n�e uveden� asi net�eba, proto�e se n�sledn� p�i refresh vol� znovu
	//TRect R=TRect(Left+Columns[Col].Left,Top+Rows[Row].Top,Left+Columns[Col].Left+Columns[Col].Width,Top+Rows[Row].Top+Rows[Row].Height);
	//if(Cells[Col][Row+1].Type!=DRAW)SetComponents(NULL,R,R,Col,Row,Cells[Col][Row]);//nastav� komponenty, mimo DRAW, proto�e se nejedn� o komponentu
}
//---------------------------------------------------------------------------
//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� ��dek
long TmGrid::GetIdxRow(int X,int Y)
{
	long RET=-1;
	if(CheckPTinTable(X,Y))//o�et�en� mimo tabulku + akceler�tor
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
	if(CheckPTinTable(X,Y))//o�et�en� mimo tabulku + akceler�tor
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

	//TPoint body[4]={TPoint(mGrid->Left-C,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top-C),TPoint(mGrid->Left+mGrid->Width,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height),TPoint(mGrid->Left,mGrid->Top+mGrid->Rows[Row].Top+mGrid->Rows[Row].Height)}; //mohlo by d�lat probl�my p�vodn� kostra
	TPoint body[4]={TPoint(Left-C,Top+Rows[Row].Top-C),TPoint(Left+Width,Top+Rows[Row].Top-C),TPoint(Left+Width,Top+Rows[Row].Top+Rows[Row].Height),TPoint(Left,Top+Rows[Row].Top+Rows[Row].Height)};
	Form->Canvas->Polygon(body,3);
	Form->Canvas->Pen->Mode=pmCopy;
}
//---------------------------------------------------------------------------
//zajist� zv�razn�n� or�mov�n� tabulky
void TmGrid::HighlightTable(TColor Color,unsigned short Size,unsigned short Offset,TPenMode PenMode)
{
	Form->Canvas->Pen->Width=Size;
	Form->Canvas->Pen->Color=Color;
	Form->Canvas->Brush->Style=bsClear;
	Form->Canvas->Pen->Mode=PenMode;
	short o=0;if(Size>1)o=1;//pouze grafick� odsazen�
	Form->Canvas->Rectangle(Left-Offset,Top-Offset,Left+Width+Offset+o,Top+Height+Offset+o);
}
//---------------------------------------------------------------------------
//zajist� zv�razn�n� or�mov�n� tabulky, pokud se do ni vstoup� my��
void TmGrid::HighlightTableOnMouse(int X,int Y)
{
	if(CheckPTinTable(X,Y))HighlightTable();
}
//---------------------------------------------------------------------------
//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) zjist�, zda jsou sou�adnice ve vnit� tabulky
bool TmGrid::CheckPTinTable(int X,int Y)
{
	return Left<=X && X<=Left+Width && Y>Top && Y<Top+Height;
}
//---------------------------------------------------------------------------
//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� kladn� ��slo sloupce a ��dku pokud se na dan�m m�st� nach�z� odkaz, pokud ne, vr�cen� hodnoty jsou -1 a -1
TPoint TmGrid::CheckLink(int X,int Y)
{
	long IndRow=GetIdxRow(X,Y);
	long IndCol=GetIdxColum(X,Y);
	if(IndRow>=0 && IndCol>=0 && CheckLink(X,Y,IndCol,IndRow))return TPoint(IndCol,IndRow);//odkaz na dan�ch sou�adnic�ch nalezen
	else return TPoint(-1,-1);
}
//---------------------------------------------------------------------------
//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� zda se na dan� bu�ce a sou�adnic�ch nach�z� odkaz
bool TmGrid::CheckLink(int X,int Y,unsigned long Col,unsigned long Row)
{
	if(Cells[Col][Row].LinkCoordinateStart.x<=X && X<=Cells[Col][Row].LinkCoordinateEnd.x
	&& Cells[Col][Row].LinkCoordinateStart.y<=Y && Y<=Cells[Col][Row].LinkCoordinateEnd.y)
	return true;
	else return false;
}
//---------------------------------------------------------------------------
//zajist� p�ebarven� odkazu v bu�ce danou barvou
void TmGrid::HighlightLink(unsigned long Col,unsigned long Row,short Intensive)
{
	Form->Canvas->Font=Cells[Col][Row].isLink;
	Form->Canvas->Font->Color=m.clIntensive(Cells[Col][Row].Font->Color,Intensive);
	Form->Canvas->Brush->Style=bsClear;
	Form->Canvas->Brush->Color=clWhite;
	Form->Canvas->TextOutW(Cells[Col][Row].LinkCoordinateStart.x,Cells[Col][Row].LinkCoordinateStart.y,ms.EP(Cells[Col][Row].Text,"<a>","</a>"));
}
//---------------------------------------------------------------------------
//podle stavu state bu� zobraz� nebo skryje v�echny komponenty
void TmGrid::SetVisibleComponents(bool state)
{
	for(unsigned long X=0;X<=ColCount-1;X++)//po ��dc�ch
	{
		for(unsigned long Y=0;Y<=RowCount-1;Y++)//po sloupc�ch
		{
			switch(Cells[X][Y].Type)
			{
				case readEDIT: 		getEdit(X,Y)->Visible=state;break;
				case EDIT: 		 		getEdit(X,Y)->Visible=state;break;
				case NUMERIC:  		getNumeric(X,Y)->Visible=state;break;
				case readNUMERIC: getNumeric(X,Y)->Visible=state;break;
				case BUTTON: 			getButton(X,Y)->Visible=state;break;
				case COMBO: 			getCombo(X,Y)->Visible=state;break;
				case CHECK:       getCheck(X,Y)->Visible=state;break;
				case RADIO:				getRadio(X,Y)->Visible=state;break;
				case glyphBUTTON:	getGlyphButton(X,Y)->Visible=state;break;
			}
		}
	}
}
//---------------------------------------------------------------------------
