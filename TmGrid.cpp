//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
#include "MyString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TmGrid *mGrid;
//---------------------------------------------------------------------------
//konstruktor
TmGrid::TmGrid()
{
	////nastaven� TABULKY
	Left=0;Top=0;//um�st�n� cel� komponenty
	RowCount=0;ColCount=0;//po�et ��dk� a sloupc�
	DefaultColWidth=90,DefaultRowHeight=25;//v�choz� v��ka a ���ka ��dku
	Row=0;Col=0;//aktu�ln� ��dek a sloupec
	AntiAliasing=false;
	SetColumnAutoFitColIdx=-3;//-3, nep�izp�sobuje se velikost,-2 v�e stejn� podle nejv�t��, -1 p�izp�sobuje se ka�d� sloupec individu�ln�, 0 a v�ce jen konkr�tn� sloupec
	//or�mov�n� - default
	TmBorder defBorder;
	defBorder.Color=clLtGray;
	defBorder.Width=1;
	defBorder.Style=psSolid;
	Border=defBorder;Border.Width=2;

	////nastaven� defaln� BU�KY
	//typ
	DefaultCell.Type = DRAW;//defaultn� komponenta
	//text
	DefaultCell.Font=new TFont();
	DefaultCell.Font->Size=12;
	DefaultCell.Font->Color=clBlack;
	DefaultCell.Font->Orientation=0;
	DefaultCell.Font->Style=TFontStyles();
	DefaultCell.Font->Pitch=TFontPitch::fpVariable;//ka�d� p�smeno fontu stejn� �irok�
	DefaultCell.Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.Font->Name="Arial";
	DefaultCell.Text="";
	//zarovn�n�
	DefaultCell.Align=CENTER;
	DefaultCell.Valign=MIDDLE;
	//pozad�
	DefaultCell.Background=new TBrush();
	DefaultCell.Background->Color=clWhite;
	DefaultCell.Background->Style=bsSolid;
	//or�mov�n�
	DefaultCell.TopBorder=defBorder;
	DefaultCell.BottomBorder=defBorder;
	DefaultCell.LeftBorder=defBorder;
	DefaultCell.RightBorder=defBorder;
}
//---------------------------------------------------------------------------
//destruktor, prob�h� p�i ukon�ov�n� programu, tj. zv�it zda nen� pozd�
TmGrid::~TmGrid()
{
	//uvoln�n� pam�ti
	Delete();
	mGrid=NULL; delete mGrid;
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

	//smaz�n� + nastaven� hodnot mus� b�t v extra cyklu
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			//Cells[X][Y]=DefaultCell; //- nelze p�evezme cel� ukazatel
			//typ
			Cells[X][Y].Type=DefaultCell.Type;
			//text + font
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Font->Size=DefaultCell.Font->Size;
			Cells[X][Y].Font->Color=DefaultCell.Font->Color;
			Cells[X][Y].Font->Orientation=DefaultCell.Font->Orientation;
			Cells[X][Y].Font->Style=DefaultCell.Font->Style;
			Cells[X][Y].Font->Pitch=DefaultCell.Font->Pitch;
			Cells[X][Y].Font->Name=DefaultCell.Font->Name;
			Cells[X][Y].Text=DefaultCell.Text;
			//zarovn�n�
			Cells[X][Y].Align=DefaultCell.Align=CENTER;
			Cells[X][Y].Valign=DefaultCell.Valign=MIDDLE;
			//pozad�
			Cells[X][Y].Background=new TBrush();
			Cells[X][Y].Background->Color=DefaultCell.Background->Color;
			Cells[X][Y].Background->Style=DefaultCell.Background->Style;
			//or�mov�n�
			Cells[X][Y].TopBorder=DefaultCell.TopBorder;
			Cells[X][Y].BottomBorder=DefaultCell.BottomBorder;
			Cells[X][Y].LeftBorder=DefaultCell.LeftBorder;
			Cells[X][Y].RightBorder=DefaultCell.RightBorder;
		}
		Columns[X].ColWidth=DefaultColWidth;
		if(X>0)Columns[X].Left=Columns[X-1].Left+X*DefaultColWidth;else Columns[X].Left=0;
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
//odstran� tabulku
void TmGrid::Delete()
{
	for(unsigned long X=0;X<ColCount;X++)//po ��dc�ch
	{
		Cells[X]=NULL; delete Cells[X];
	}
	Cells=NULL; delete Cells;
	Columns=NULL; delete Columns;
}
//---------------------------------------------------------------------------
//zajist� vykreslen� cel� tabulky
void TmGrid::Show(TForm *Form)
{
	if(mGrid!=NULL)
	{
		if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k po�adavku na zm�nu po�tu ��dk� a sloupc� vol� se realokace
		{
			realock(Form);
		}

		if(!AntiAliasing)DrawGrid(Form,1);
//	else
//	{
//		Cantialising a;
//		Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
//    pozor width a height pot�ebu je zn�t hodnoty po autofit
//		bmp_in->Width=ColWidth*ColCount*3;bmp_in->Height=RowHeight*RowCount*3;//velikost canvasu//*3 vypl�v� z logiky algoritmu antialiasingu
//		DrawGrid(Canvas,Form,3);
//		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutn� do samostatn� bmp, kv�li smaz�n� bitmapy vracen� AA
//		Canvas->Draw(Left,Top,bmp_out);
//		delete (bmp_out);//velice nutn�
//		delete (bmp_in);//velice nutn�
//	}
	}
}
//---------------------------------------------------------------------------
//zajist� vykreslen� cel� tabulky
void TmGrid::DrawGrid(TForm *Form,double Zoom)
{
	TCanvas *C=Form->Canvas;//pouze zkr�cen� zapisu

	//autofit sloupc� nastav� Columns[X].ColWidth
	executeColumnsAutoFit(Form->Canvas);

	//////BU�KY jednotliv�
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		////oblast bu�ky
		TRect R;
		if(X>0)Columns[X].Left=Columns[X-1].Left+Columns[X-1].ColWidth;else Columns[0].Left=0;
		R.Left	=	Left+Columns[X].Left*Zoom;
		R.Right	=	Left+(Columns[X].Left+Columns[X].ColWidth)*Zoom;

		for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
		{
			////oblast bu�ky
			R.Top		=	(Top	+((Y)*DefaultRowHeight))*Zoom;
			R.Bottom=	(Top	+((Y+1)*DefaultRowHeight))*Zoom;

			////barva pozad� bu�ky
			C->Brush->Color=Cells[X][Y].Background->Color;
			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(R);

			////komponenta v bu�ce
			SetComponents(Form,R,X,Y,Cells[X][Y]);

			////or�mov�n� bu�ky
			//top
			SetBorder(C,Cells[X][Y].TopBorder);
			C->MoveTo(R.Left,R.Top);C->LineTo(R.Right,R.Top);
			//bottom
			SetBorder(C,Cells[X][Y].BottomBorder);
			C->MoveTo(R.Left,R.Bottom);C->LineTo(R.Right,R.Bottom);
			//left
			SetBorder(C,Cells[X][Y].LeftBorder);
			C->MoveTo(R.Left,R.Top);C->LineTo(R.Left,R.Bottom);
			//right
			SetBorder(C,Cells[X][Y].RightBorder);
			C->MoveTo(R.Right,R.Top);C->LineTo(R.Right,R.Bottom);
		}
	}

	//////TABULKA - or�mov�n� cel� + v�po�et celkov� ���ky a v��ky
	SetBorder(C,Border);
	Width=getWidth();//z�rove� nastavuje celkovou hodnotu Width
	Height=RowCount*DefaultRowHeight;//z�rove� nastavuje celkovou hodnotu Height
	C->MoveTo(Left,Top);C->LineTo(Width+Left,Top);C->LineTo(Width+Left,Top);C->LineTo(Width+Left,Height+Top);C->LineTo(Left,Height+Top);C->LineTo(Left,Top);

	C=NULL; delete C;
}
//---------------------------------------------------------------------------
//nastav� grafick� pero na po�adovan� parametry
void TmGrid::SetBorder(TCanvas *C,TmBorder Border)
{
	DeleteObject(C->Pen->Handle);//zru�� p�vodn� pero       //PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE
	DWORD pStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE /*| PS_JOIN_BEVEL*/ | PS_INSIDEFRAME;
	DWORD pWidth = Border.Width;

	LOGBRUSH lBrush;
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbColor = Border.Color;
	lBrush.lbHatch = 0;

	C->Pen->Handle = ExtCreatePen(pStyle, pWidth, &lBrush, NULL, NULL);
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku dle typu
void TmGrid::SetComponents(TForm *Form,TRect R,unsigned long X,unsigned long Y,TCells Cell)
{
	switch(Cell.Type)
	{
		case DRAW:
		{   Cell.Text=getTag(X,Y);//provizorn�
			TCanvas *Canv=Form->Canvas;//pouze zkr�cen� zapisu
			//nastaven� fontu
			Canv->Font=Cell.Font;
			//SetBkMode(canv->Handle,OPAQUE);//nastven� netransparentn�ho pozad�
			Canv->Brush->Color=clWhite;Canv->Brush->Style=bsClear;//nastven� netransparentn�ho pozad�
			//zarovn�n�
			//samotn� v�pis
			//Canv->TextRect(R,0,0,"test"); - nefunguje
			long L=R.Left,T=R.Top;
			unsigned int W=Canv->TextWidth(Cell.Text);
			unsigned int H=Canv->TextHeight(Cell.Text);
			if(Cell.Font->Orientation==900){W=H;H=Canv->TextWidth(Cell.Text);}
			switch(Cell.Align)
			{
				case LEFT:	L=L;break;
				case CENTER:L=(R.Left+R.Right)/2-W/2;break;
				case RIGHT:	L=R.Right-W;break;
			}
			switch(Cell.Valign)
			{
				case TOP:		T=T;break;
				case MIDDLE:T=(R.Top+R.Bottom)/2-H/2;break;
				case BOTTOM:T=R.Bottom-H;break;
			}
			Canv->TextOut(L,T,Cell.Text);
			Canv=NULL; delete Canv;
		}break;
		case readEDIT:
		{
			SetEdit(Form,R,X,Y,Cell);
		}break;
		case EDIT:
		{
			SetEdit(Form,R,X,Y,Cell);
		}break;
		case readNUMERIC:
		{
			SetNumeric(Form,R,X,Y,Cell);
		}break;
		case NUMERIC:
		{
			SetNumeric(Form,R,X,Y,Cell);
		}break;
		case BUTTON:
		{
			//zalo�en� + tag + n�zev
			TscGPButton *B=getButton(X,Y,Form);//pokud ji� existuje
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
			B->Width=Columns[X].ColWidth-1;
			B->Height=DefaultRowHeight-1;
//			B->Options->NormalColor=Cell.Background->Color;
			B->Options->FrameNormalColor=B->Options->NormalColor;
			B->Font=Cell.Font;
			B->Caption=Cell.Text;
			//vlastn�k
			B->Parent=Form;//mus� b�t a� na konci
		}break;
		case COMBO:
		{
			//zalo�en� + tag + n�zev
			TscGPComboBox *C=getCombo(X,Y,Form);//pokud ji� existuje
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
			C->Width=Columns[X].ColWidth-1;
			C->Height=DefaultRowHeight-1;
			C->Options->NormalColor=Cell.Background->Color;
			C->Options->NormalColorAlpha=255;
//			C->Options->FrameNormalColor=clWhite;
//			C->Options->FrameNormalColorAlpha=255;
			C->Font=Cell.Font;
			//C->Text=Cell.Text;
			//vlastn�k
			C->Parent=Form;//mus� b�t a� na konci
		}break;
		case CHECK:
		{
			//zalo�en� + tag + n�zev
			TscGPCheckBox *Ch = getCheck(X,Y,Form);//pokud ji� existuje
			if(Ch==NULL)
			{
				Ch = new TscGPCheckBox(Form);//pokud ne
				Ch->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
				Ch->Name="mGrid_CHECK_"+AnsiString(Ch->Tag);
				//ud�losti
				Ch->OnClick=&getTagOnClick;
				Ch->OnEnter=&getTagOnEnter;
			}
			//atributy
			Ch->Top=R.Top+1;
			Ch->Left=R.Left+1;
			Ch->Width=Columns[X].ColWidth-1;
			Ch->Height=DefaultRowHeight-1;
//			Ch->Options->NormalColor=Cell.Background->Color;
//			Ch->Options->NormalColorAlpha=255;
//			Ch->Options->FrameNormalColor=clWhite;
//			Ch->Options->FrameNormalColorAlpha=255;
			Ch->Font=Cell.Font;
			Ch->Caption=Cell.Text;
			//vlastn�k
			Ch->Parent=Form;//mus� b�t a� na konci
		}break;
		case RADIO:
		{
			//zalo�en� + tag + n�zev
			TscGPRadioButton *Ra = getRadio(X,Y,Form);//pokud ji� existuje
			if(Ra==NULL)//pokud ne
			{
				Ra = new TscGPRadioButton(Form);
				Ra->Tag=getTag(X,Y);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
				Ra->Name="mGrid_RADIO_"+AnsiString(Ra->Tag);

				//ud�losti
				Ra->OnClick=&getTagOnClick;
				Ra->OnEnter=&getTagOnEnter;
			}
			//atributy
			Ra->Top=R.Top+1;
			Ra->Left=R.Left+1;
			Ra->Width=Columns[X].ColWidth-1;
			Ra->Height=DefaultRowHeight-1;
//			Ra->Options->NormalColor=Cell.Background->Color;
//			Ra->Options->NormalColorAlpha=255;
//			Ra->Options->FrameNormalColor=clWhite;
//			Ra->Options->FrameNormalColorAlpha=255;
			Ra->Font=Cell.Font;
			Ra->Caption=Cell.Text;
			//vlastn�k
			Ra->Parent=Form;//mus� b�t a� na konci
		}break;
	}
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na edit, pomocn� metoda v��e uveden�
void TmGrid::SetEdit(TForm *Form,TRect R,unsigned long X,unsigned long Y,TCells Cell)
{
	//zalo�en� + tag + n�zev
	TscGPEdit *E=getEdit(X,Y,Form);//pokud ji� existuje
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
	E->Width=Columns[X].ColWidth-1;
	E->Height=DefaultRowHeight-1;
	E->Options->NormalColor=Cell.Background->Color;
	E->Options->NormalColorAlpha=255;
	E->Options->FrameNormalColor=clWhite;
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
	Form->Canvas->Font=Cell.Font;
	switch(Cell.Valign)
	{
		case TOP:		E->ContentMarginTop=0;break;
		case MIDDLE:E->ContentMarginTop=E->Height/2-Form->Canvas->TextHeight(Cell.Text)/2;break;
		case BOTTOM:E->ContentMarginTop=E->Height-Form->Canvas->TextHeight(Cell.Text);break;
	}
	E->Font=Cell.Font;
	E->Text=Cell.Text;
	//vlastn�k
	E->Parent=Form;//mus� b�t a� na konci
}
//---------------------------------------------------------------------------
//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
void TmGrid::SetNumeric(TForm *Form,TRect R,unsigned long X,unsigned long Y,TCells Cell)
{
	//zalo�en� + tag + n�zev
	TscGPNumericEdit *N=getNumeric(X,Y,Form);//pokud ji� existuje
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
	N->Width=Columns[X].ColWidth-1;
	N->Height=DefaultRowHeight-1;
	N->Options->NormalColor=Cell.Background->Color;
	N->Options->NormalColorAlpha=255;
	N->Options->FrameNormalColor=clWhite;
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
	Form->Canvas->Font=Cell.Font;
	switch(Cell.Valign)
	{
		case TOP:		N->ContentMarginTop=0;break;
		case MIDDLE:N->ContentMarginTop=N->Height/2-Form->Canvas->TextHeight(Cell.Text)/2;break;
		case BOTTOM:N->ContentMarginTop=N->Height-Form->Canvas->TextHeight(Cell.Text);break;
	}
	N->Font=Cell.Font;
	TMyString ms;
	N->Value=ms.MyToDouble(Cell.Text);
	//vlastn�k
	N->Parent=Form;//mus� b�t a� na konci
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
	return Tag%ColCount;
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
			Columns[X].ColWidth=DefaultColWidth;
		}
		break;
		////v�echny sloupce stejn� podle nej�ir��ho
		case -2:
		{
			unsigned int  MaxColWidth=0;
			//nalezen� maxima
			for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
			{
				for(unsigned long Y=0;Y<RowCount;Y++)//po ��dc�ch
				{
					Canv->Font=Cells[X][Y].Font;
					unsigned int W=Canv->TextWidth(Cells[X][Y].Text);
					if(Cells[X][Y].Type==CHECK || Cells[X][Y].Type==RADIO)W+=20+4+4;
					if(W>MaxColWidth)MaxColWidth=W;//najde nej�i���
				}
			}
			//zapis do pole ���ky sloupc�
			for(unsigned long X=0;X<ColCount;X++)
			Columns[X].ColWidth=MaxColWidth+4;//+4 pouze offset;
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
	Canv->Font=Cells[ColIdx][0].Font;
	unsigned int ColWidth=Canv->TextWidth(Cells[ColIdx][0].Text); //v�choz� hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		Canv->Font=Cells[ColIdx][Y].Font;
		unsigned int W=Canv->TextWidth(Cells[ColIdx][Y].Text);
		if(Cells[ColIdx][Y].Type==CHECK || Cells[ColIdx][Y].Type==RADIO)W+=20+4+4;
		if(W>ColWidth)ColWidth=W;//najde nej�i���
	}
	Columns[ColIdx].ColWidth=ColWidth+4;//+4 pouze offset
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vr�t� celkovou ���ku tabulky
unsigned long TmGrid::getWidth()
{
	unsigned long RET=0;
	for(unsigned long X=0;X<ColCount;X++)//po sloupc�ch
	{
		RET+=Columns[X].ColWidth;
		//ShowMessage(Columns[X].ColWidth);
	}
	return RET;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//dle zadan�ho  ��sla sloupce a ��sla ��dku  vr�t� ukazatel nadanou komponentu
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(getTag(Col,Row)));}
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(getTag(Col,Row)));}
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(getTag(Col,Row)));}
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(getTag(Col,Row)));}//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(getTag(Col,Row)));};//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnClick(TObject *Sender)
{
	ShowMessage(AnsiString("OnClick ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnEnter(TObject *Sender)
{
	//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnChange(TObject *Sender)
{
	//ShowMessage(AnsiString("OnChange ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//spoj� dv� bu�ky do jedn�
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	 if(ColCell_1==ColCell_2)//varianta pod sebou
	 {
		 Cells[ColCell_1][RowCell_1].BottomBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
		 Cells[ColCell_2][RowCell_2].TopBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
		 //zan�st do bu�ky 11, asi novou ���ku
	 }
	 if(RowCell_1==RowCell_2)//varianta vedle sebe
	 {
		 Cells[ColCell_1][RowCell_1].RightBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
		 Cells[ColCell_2][RowCell_2].LeftBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
	 }
	 Cells[ColCell_2][RowCell_2].Background->Color=Cells[ColCell_1][RowCell_1].Background->Color;
}
//---------------------------------------------------------------------------
void TmGrid::HighlightCell(unsigned long Col,unsigned long Row,unsigned short Width,TColor Color)
{
//	switch(Cell[Col][Row])
//	{
//		case EDIT:
//		{
			TmBorder hlBorder;
			hlBorder.Color=Color;
			hlBorder.Width=Width;
			hlBorder.Style=psSolid;
			Cells[Col][Row].TopBorder=hlBorder;
			Cells[Col][Row].LeftBorder=hlBorder;
			Cells[Col][Row].RightBorder=hlBorder;
			Cells[Col][Row].BottomBorder=hlBorder;
			if(Col<ColCount-1)Cells[Col+1][Row].LeftBorder=hlBorder;//je�t� sousedn� bu�ky
			if(Row<RowCount-1)Cells[Col][Row+1].TopBorder=hlBorder;//je�t� sousedn� bu�ky
//		}break;
//	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//sma�e text cel�ho sloupce
void TmGrid::ClearColumn(unsigned long ColIdx)
{
	for(unsigned long Y=0;Y<=RowCount;Y++)
	{
		Cells[ColIdx][Y].Text="";
	}
}
//---------------------------------------------------------------------------
//sma�e cel� ��dek
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
void TmGrid::realock(TForm *Form)
{
	//kopie do z�lo�n�ho pole a smaz�n�
	TCells **bufCells = new TCells*[bufColCount];//vytvo�� dynamick� pole ukazatelu typu TCells dle po�tu po�adovan�ch sloupc�
	for(unsigned long X=0;X<bufColCount;X++)
	{
		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupc�m dynamamick�ho pole alokuje pam�t pro jednotliv� ��dky- cyklus mus� b�t samostatn�
		bufCells[X] = Cells[X];//zkop�rov�n� p�vodn�ch hodnot do z�lohy
	}

	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;//provizorn� vr�cen�
	DeleteComponents(Form);
	Delete();
	ColCount=bufColCount2;RowCount=bufRowCount2;//zp�t

	//vytvo�en� nov�ho realokovan�ho pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//ur�eno p�i dal�� realokaci pole, create toti� p�episuje buf hodnoty
	Create();

	//zkop�rov�n� p�vodn�ch hodnot zp�t
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je men��
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je men��
	for(unsigned long X=0;X<bufColCount;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount;Y++)
		{
			Cells[X][Y] = bufCells[X][Y];
		}
	}

	//smaz�n� p�vodn�ho bufCells
	for(unsigned long X=0;X<bufColCount2;X++)
	{
		bufCells[X]=NULL; delete bufCells[X];
	}
	bufCells=NULL; delete bufCells;
	bufColCount=ColCount;bufRowCount=RowCount;//ur�eno p�i dal�� realokaci pole
}
//---------------------------------------------------------------------------
//odstran� dynamicky vyto�en� komponenty
void TmGrid::DeleteComponents(TForm *Form)
{
	//smaz�n� komponent m�l by obstart Form v kter�m je um�st�n�
	for(unsigned long X=0;X<ColCount;X++)//po ��dc�ch
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po sloupc�ch
		{
			switch(Cells[X][Y].Type)
			{
				case readEDIT: {TscGPEdit *E=getEdit(X,Y,Form);E->Free();E=NULL;delete E;}break;
				case EDIT: {TscGPEdit *E=getEdit(X,Y,Form);E->Free();E=NULL;delete E;}break;
				case NUMERIC: {TscGPNumericEdit *N=getNumeric(X,Y,Form);N->Free();N=NULL;delete N;}break;
				case readNUMERIC: {TscGPNumericEdit *N=getNumeric(X,Y,Form);N->Free();N=NULL;delete N;}break;
				case BUTTON: {TscGPButton *B=getButton(X,Y,Form);B->Free();B=NULL;delete B;}break;
				case COMBO: {TscGPComboBox *C=getCombo(X,Y,Form);C->Free();C=NULL;delete C;}break;
				case CHECK:{TscGPCheckBox *CH=getCheck(X,Y,Form);CH->Free();CH=NULL;delete CH;}break;
				case RADIO:{TscGPRadioButton *R=getRadio(X,Y,Form);R=NULL;delete R;}break;
			}
		}
	}
}
//---------------------------------------------------------------------------

