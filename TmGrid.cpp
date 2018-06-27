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
	////nastaven� TABULKY
	Tag=0;//ID komponenty (vyu�iteln� nap�. pokud bude v�ce tabulek, tak se bude v�d�t, v jak� do�lo ke kliku)
	Left=0;Top=0;//um�st�n� cel� komponenty
	RowCount=0;ColCount=0;//po�et ��dk� a sloupc�
	DefaultColWidth=90,DefaultRowHeight=25;//v�choz� v��ka a ���ka ��dku
	Row=0;Col=0;//aktu�ln� ��dek a sloupec
	AntiAliasing_grid=false;
	AntiAliasing_text=false;
	SetColumnAutoFitColIdx=-3;//nastav� ���ku bun�k dan�ho sloupce dle parametru ColIdx, -3 = nep�izp�sobuje se velikost a u�ije se defaultColWidth,-2 v�echny sloupce stejn� podle nej�ir��ho textu, -1 p�izp�sobuje se ka�d� sloupec individu�ln�, 0 a v�ce jen konkr�tn� sloupec uveden� pomoc ColIdx
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
		TRect Rb;//pozad� bu�ky + text
		TRect Rc;//componenty
		R.Left	=	Left+Columns[X].Left*Zoom_g;
		Rb.Left	=	Left+Columns[X].Left*Zoom_b;
		Rc.Left	=	Left+Columns[X].Left;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_g;
		Rb.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom_b;
		Rc.Right	=	Left+(Columns[X].Left+Columns[X].Width);

		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			////oblast bu�ky
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//v�po�et horn�ho okraje bu�ky dle bu�ky p�edchoz�
			R.Top			=	Top+Rows[Y].Top*Zoom_g;
			Rb.Top		=	Top+Rows[Y].Top*Zoom_b;
			Rc.Top		=	Top+Rows[Y].Top;
			R.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_g;
			Rb.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom_b;
			Rc.Bottom	=	Top+(Rows[Y].Top+Rows[Y].Height);

			////barva pozad� bu�ky
			C->Brush->Color=Cells[X][Y].Background->Color;
			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(Rb);

			////komponenta v bu�ce
			SetComponents(C,Rc,Rb,X,Y,Cells[X][Y]);

			////or�mov�n� bu�ky
			//pou��v�m duplicitn� (k DrawGrid) zde, kv�li akceleraci v p��pad� tot�ln� vypnut�ho AA nebo tot�ln� zapnut�ho AA, v takov�m p��pad� potom neb�� DrawGrid, mohl bych ho sice volat zde, ale chci si u�et�it opakovan� pr�chod cykly, DrawGrid b�� jenom v momentu AntiAliasing_text=false a AntiAliasing_grid=true
			if(!(AntiAliasing_grid==false && AntiAliasing_text==true))
			{
				//top
				//nefunguje spr�vn�:if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedn� se o slou�enou bu�ku, slab� podm�nka
				{
					SetBorder(C,Cells[X][Y].TopBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
				}
				//bottom
				if(Y==RowCount-1)//nefunguje spr�vn�: && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
				{
					SetBorder(C,Cells[X][Y].BottomBorder);
					C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
				}
				//left
				//nefunguje spr�vn�:if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedn� se o slou�enou bu�ku, slab� podm�nka
				{
					SetBorder(C,Cells[X][Y].LeftBorder);
					C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
				}
				//right                                 ud�lat p��znak na slou�enu bu�ku t� nekreslit st�e
				if(X==ColCount-1)//nefunguje spr�vn�: && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
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
			//nefunguje spr�vn�: if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedn� se o slou�enou bu�ku, slab� podm�nka
			{
				SetBorder(C,Cells[X][Y].TopBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
			}
			//bottom
			if(Y==RowCount-1)//nefunguje spr�vn�:  && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
			{
				SetBorder(C,Cells[X][Y].BottomBorder);
				C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
			}
			//left
			//nefunguje spr�vn�: if(Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//pokud se nejedn� se o slou�enou bu�ku, slab� podm�nka
			{
				SetBorder(C,Cells[X][Y].LeftBorder);
				C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
			}
			//right
			if(X==ColCount-1) //nefunguje spr�vn�: && Cells[X][Y].TextPositon.X>=0 && Cells[X][Y].TextPositon.Y>=0)//akceler�tor, aby se zbyte�n� nevykreslovalo, pokud by bylo zbyte�n�, vykresl� jenom posledn�, invertn� filozofie ne� ukazov�n� na stejn� or�mov�n�, ale zde z d�vodu mo�n�ho p�ekryvu s n�pln� p�edchoz� bu�ky
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
			int W=getWidthHeightText(Cell).X*Zoom;if(AntiAliasing_text)W/=1.3;
			int H=getWidthHeightText(Cell).Y*Zoom;
			if(Cell.Font->Orientation==900){H=0;if(Cell.Valign==MIDDLE)H=-getWidthHeightText(Cell).Y;}
			if(Cell.Font->Orientation==2700){W=0;if(Cell.Align==LEFT || Cell.Align==CENTER)W=-W;H=0;if(Cell.Valign==MIDDLE)H=getWidthHeightText(Cell).Y;}
			short WA=0;if(AntiAliasing_text)WA=1;//zajimav� workaround - p���in� nerozum�m (pro� Left*2 tomu pat�i�n� pom��e)
			switch(Cell.Align)
			{               //zajimav� workaround
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
			TscGPButton *B=getButton(X,Y);//pokud ji� existuje
			if(B==NULL)
			{
				B=new TscGPButton(Form);//pokud ne
				B->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
				B->Name="mGrid_BUTTON_"+AnsiString(B->Tag);
				//ud�losti
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
			//vlastn�k
			B->Parent=Form;//mus� b�t a� na konci
			B=NULL;delete B;
		}break;
		case COMBO:
		{
			//zalo�en� + tag + n�zev
			TscGPComboBox *C=getCombo(X,Y);//pokud ji� existuje
			if(C==NULL)
			{
				C=new TscGPComboBox(Form);//pokud ne
				C->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
				C->Name="mGrid_COMBO_"+AnsiString(C->Tag);
				//ud�losti
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
	TscGPEdit *E=getEdit(X,Y);//pokud ji� existuje
	if(E==NULL)//pokud ne
	{
		E=new TscGPEdit(Form);
		E->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		E->Name="mGrid_EDIT_"+AnsiString(E->Tag);
		//ud�losti
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
	//vlastn�k
	E->Parent=Form;//mus� b�t a� na konci
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
void TmGrid::SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell)
{
	//zalo�en� + tag + n�zev
	TscGPNumericEdit *N=getNumeric(X,Y);//pokud ji� existuje
	if(N==NULL)//pokud ne
	{
		N=new TscGPNumericEdit(Form);
		N->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
		N->Name="mGrid_NUMERIC_"+AnsiString(N->Tag);
		//ud�losti
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
	//vlastn�k
	N->Parent=Form;//mus� b�t a� na konci
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
		unsigned int W=getWidthHeightText(Cells[ColIdx][Y]).X+Cells[ColIdx][Y].LeftMargin+Cells[ColIdx][Y].LeftBorder->Width/2+Cells[ColIdx][Y].RightMargin+Cells[ColIdx][Y].RightBorder->Width/2;
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
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(getTag(Col,Row)));}
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(getTag(Col,Row)));}
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(getTag(Col,Row)));}
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(getTag(Col,Row)));};//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
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
//spoj� dv� bu�ky do jedn�
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	SetColRow();

	////kopie referen�n� bu�ky //TCells RefCell=Cells[ColCell_1][RowCell_1];// - nelze v�ak pou��t takto na p�ed�v�n� borders, proto metoda n�e, p�ed�valo by i ukazatel
	TCells RefCell;CreateCell(RefCell);
	CopyCell(Cells[ColCell_1][RowCell_1],RefCell,true);

	////nastaven� referen�n� bu�ky kv�li or�mov�n� v�ech bun�k oblasti na toto�nou barvu
	TBorder B;B.Width=0;B.Style=psSolid;B.Color=Cells[ColCell_1][RowCell_1].Background->Color;
	*Cells[ColCell_1][RowCell_1].TopBorder=*Cells[ColCell_1][RowCell_1].BottomBorder=*Cells[ColCell_1][RowCell_1].LeftBorder=*Cells[ColCell_1][RowCell_1].RightBorder=B;

	////projde nejd��ve v�echny bu�ky nastav� jim prvn� dle pozad� prvn� bu�ky stejn� pozad� a dle barvy pozad� i barvu or�mov�n�
	SetCells(Cells[ColCell_1][RowCell_1],ColCell_1,RowCell_1,ColCell_2,RowCell_2,-1,false);

	//typ prvn� bu�ky
	Cells[ColCell_1][RowCell_1].Type=RefCell.Type;

	////vytvo�� resp. p�ed� or�mov�n� oblasti dle referen�n� bu�ky, �lo by �e�it v ve v��e volan�m pr�chodu, bylo by sice systomov� m�n� n�ro�n�, ale v�ce komplikovan� na realizaci
	SetRegion(RefCell,ColCell_1,RowCell_1,ColCell_2,RowCell_2);

	////text
	unsigned int W=0;unsigned int H=0;
	Cells[ColCell_2][RowCell_2].Text=RefCell.Text;//navr�cen� ze z�lohy do posledn� bu�ky, proto�e ta se vykresluje jako posledn�
	W=getWidthHeightText(RefCell).X;
	H=getWidthHeightText(RefCell).Y;
	if(RefCell.Font->Orientation==900){W=H;H=0;if(RefCell.Valign==MIDDLE)H=-getWidthHeightText(RefCell).X;}
	if(RefCell.Font->Orientation==2700){W=0;if(RefCell.Align==LEFT || RefCell.Align==CENTER)W=-H;H=0;if(RefCell.Valign==MIDDLE)H=getWidthHeightText(RefCell).X;}
		//if(Cell.Font->Orientation==2700)L-=H;

	//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby - D�LE�ITE!!!
	Cells[ColCell_2][RowCell_2].Align=aNO;
	Cells[ColCell_2][RowCell_2].Valign=vNO;
	//zarovn�n� (zarovn�v� dle prvn� bu�ky, ale pracuje s posledn�, proto�e ta se vykresluje zcela posledn�)
	switch(Cells[ColCell_1][RowCell_1].Align)
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
			 Cells[ColCell_2][RowCell_2].TextPositon.X=Columns[ColCell_2].Width-W;//�e��m v setcomponents -RefCell.RightMargin-RefCell.RightBorder->Width/2;
		 }break;
	}
	switch(Cells[ColCell_1][RowCell_1].Valign)
	{
		 case TOP:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_1].Top-Rows[RowCell_2].Top;//�e��m v setcomponents +RefCell.TopMargin+RefCell.TopBorder->Width/2;
		 }break;
		 case MIDDLE:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.Y=(Rows[RowCell_1].Top-Rows[RowCell_2].Top+Rows[RowCell_2].Height)/2-H/2;
		 }
		 break;
		 case BOTTOM:
		 {
			 Cells[ColCell_2][RowCell_2].TextPositon.Y=Rows[RowCell_2].Height-H;//�e��m v setcomponents -RefCell.BottomMargin-RefCell.BottomBorder->Width/2;
		 }
		 break;
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
//zkop�ruje obsah, form�t (bez or�mov�n�) z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
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
	////zarovn�n�
	CopyCell.Align=RefCell.Align;
	CopyCell.Valign=RefCell.Valign;
	//osazen�
	CopyCell.TopMargin=RefCell.TopMargin;
	CopyCell.BottomMargin=RefCell.BottomMargin;
	CopyCell.LeftMargin=RefCell.LeftMargin;
	CopyCell.RightMargin=RefCell.RightMargin;
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
//sma�e text cel� tabulku
void TmGrid::Clear()
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		ClearColumn(X);
	}
}
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
		//*bufCells[X] = *Cells[X];//zkop�rov�n� p�vodn�ch hodnot do z�lohy //takto asi nevhodn� kop�rovat (z�stane ukazatel), s hv�zdickou sice hodnoty ale je pot�eba p�id�lit pam�t pomoc� new pro TBrush TFont a TBorders
		for(unsigned long Y=0;Y<bufRowCount;Y++)
		{
			CreateCell(bufCells[X][Y]);
			CopyCell(Cells[X][Y],bufCells[X][Y],true);
		}
	}

	//smaz�n� p�vodn�ch hodnot
	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;
	DeleteComponents();
	//pokud bych p�vodn� cht�l komponenty zachovat, ale nefunguje zcela spr�vn� DeleteComponents(bufColCount2,bufRowCount2,ColCount-1,RowCount-1);
	ColCount=bufColCount2;RowCount=bufRowCount2;
	DeleteTable();

	//vytvo�en� nov�ho realokovan�ho pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//ur�eno p�i dal�� realokaci pole, create toti� p�episuje buf hodnoty
	Create(ColCount,RowCount);

	//zkop�rov�n� p�vodn�ch hodnot zp�t
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je men��
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je men��
	for(unsigned long X=0;X<bufColCount;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount;Y++)
		{
			//nelze cel� ukazatel Cells[X][Y] = bufCells[X][Y];
			////typ
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Background=new TBrush();//alokace pam�ti
			CopyAreaCell(bufCells[X][Y],Cells[X][Y]);//kopie vlastnost�, bez or�mov�n�
			////or�mov�n�
			CreateLinkBorder(X,Y,bufCells[X][Y]);
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

