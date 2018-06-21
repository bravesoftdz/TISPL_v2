//---------------------------------------------------------------------------
#pragma hdrstop
#include "TmGrid.h"
#include "antialiasing.h"
#include "MyString.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TmGrid *mGrid;
//---------------------------------------------------------------------------
//konstruktor
TmGrid::TmGrid()
{
	////nastavení TABULKY
	Left=0;Top=0;//umístìní celé komponenty
	RowCount=0;ColCount=0;//poèet øádkù a sloupcù
	DefaultColWidth=90,DefaultRowHeight=25;//výchozí výška a šíøka øádku
	Row=0;Col=0;//aktuální øádek a sloupec
	AntiAliasing=false;
	SetColumnAutoFitColIdx=-3;//nastaví šíøku bunìk daného sloupce dle parametru ColIdx, -3 = nepøizpùsobuje se velikost a užije se defaultColWidth,-2 všechny sloupce stejnì podle nejširšího textu, -1 pøizpùsobuje se každý sloupec individuálnì, 0 a více jen konkrétní sloupec uvedený pomoc ColIdx
	//orámování - default
	TBorder defBorder;
	defBorder.Color=clLtGray;
	defBorder.Width=1;
	defBorder.Style=psSolid;
	Border=defBorder;Border.Width=2;

	////nastavení defalní BUÒKY
	//typ
	DefaultCell.Type = DRAW;//defaultní komponenta
	//text
	DefaultCell.Font=new TFont();
	DefaultCell.Font->Size=12;
	DefaultCell.Font->Color=clBlack;
	DefaultCell.Font->Orientation=0;
	DefaultCell.Font->Style=TFontStyles();
	DefaultCell.Font->Pitch=TFontPitch::fpVariable;//každé písmeno fontu stejnì široké
	DefaultCell.Font->Pitch=System::Uitypes::TFontPitch::fpVariable;
	DefaultCell.Font->Name="Arial";
	DefaultCell.Text="";
	//zarovnání
	DefaultCell.Align=CENTER;
	DefaultCell.Valign=MIDDLE;
	DefaultCell.TopMargin=2;
	DefaultCell.BottomMargin=2;
	DefaultCell.LeftMargin=2;
	DefaultCell.RightMargin=2;
	//pozadí
	DefaultCell.Background=new TBrush();
	DefaultCell.Background->Color=clWhite;
	DefaultCell.Background->Style=bsSolid;
	//orámování
	DefaultCell.TopBorder=new TBorder;		*DefaultCell.TopBorder=defBorder;
	DefaultCell.BottomBorder=new TBorder;	*DefaultCell.BottomBorder=defBorder;
	DefaultCell.LeftBorder=new TBorder;		*DefaultCell.LeftBorder=defBorder;
	DefaultCell.RightBorder=new TBorder;	*DefaultCell.RightBorder=defBorder;
}
//---------------------------------------------------------------------------
//destruktor, probíhá pøi ukonèování programu, tj. zvážit zda není pozdì
TmGrid::~TmGrid()
{
	//uvolnìní pamìti
	Delete();
	mGrid=NULL; delete mGrid;
}
//---------------------------------------------------------------------------
//vytvoøí tabulku
void TmGrid::Create()
{
	//alokace dvourozmerneho dynamickeho pole
	Cells = new TCells*[ColCount];//vytvoøí dynamické pole ukazatelu typu TCells dle poètu požadovaných sloupcù
	for(unsigned long X=0;X<ColCount;X++)Cells[X] = new TCells[RowCount];//pruchod po sloupcich, slupcùm dynamamického pole alokuje pamìt pro jednotlivé øádky- cyklus musí být samostatnì
	//alokace jednorozmìrného dynamického pole
	Columns = new TColumns[ColCount];
	Rows = new TRows[RowCount];

	//smazání + nastavení hodnot musí být v extra cyklu
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			//Cells[X][Y]=DefaultCell; //- nelze, takto pøevezme celý ukazatel
			////typ
			Cells[X][Y].Type=DefaultCell.Type;
			////text + font
			Cells[X][Y].Font=new TFont();
			Cells[X][Y].Font->Size=DefaultCell.Font->Size;
			Cells[X][Y].Font->Color=DefaultCell.Font->Color;
			Cells[X][Y].Font->Orientation=DefaultCell.Font->Orientation;
			Cells[X][Y].Font->Style=DefaultCell.Font->Style;
			Cells[X][Y].Font->Pitch=DefaultCell.Font->Pitch;
			Cells[X][Y].Font->Name=DefaultCell.Font->Name;
			Cells[X][Y].Text=DefaultCell.Text;
			////zarovnání
			Cells[X][Y].Align=DefaultCell.Align;
			Cells[X][Y].Valign=DefaultCell.Valign;
			Cells[X][Y].TopMargin=DefaultCell.TopMargin;
			Cells[X][Y].BottomMargin=DefaultCell.BottomMargin;
			Cells[X][Y].LeftMargin=DefaultCell.LeftMargin;
			Cells[X][Y].RightMargin=DefaultCell.RightMargin;
			////pozadí
			Cells[X][Y].Background=new TBrush();
			Cells[X][Y].Background->Color=DefaultCell.Background->Color;
			Cells[X][Y].Background->Style=DefaultCell.Background->Style;
			////orámování
			//top (pøebírá, propojuje se s horním, nejedná-li se o první)
			if(Y==0){Cells[X][Y].TopBorder=new TBorder;*Cells[X][Y].TopBorder=*DefaultCell.TopBorder;}
			else Cells[X][Y].TopBorder=Cells[X][Y-1].BottomBorder;//pokud ne, odkazuje na spoleèné ohranièení, jedná se o jeden objekt
			//botom
			Cells[X][Y].BottomBorder=new TBorder;*Cells[X][Y].BottomBorder=*DefaultCell.BottomBorder;
			//left (pøebírá, propojuje se s levým, nejedná-li se o první)
			if(X==0){Cells[X][Y].LeftBorder=new TBorder;*Cells[X][Y].LeftBorder=*DefaultCell.LeftBorder;}
			else Cells[X][Y].LeftBorder=Cells[X-1][Y].RightBorder;
			//right
			Cells[X][Y].RightBorder=new TBorder;*Cells[X][Y].RightBorder=*DefaultCell.RightBorder;
			////øádky
			Rows[Y].Height=DefaultRowHeight;
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Y*DefaultRowHeight;else Rows[0].Top=0;
		}
		Columns[X].Width=DefaultColWidth;
		if(X>0)Columns[X].Left=Columns[X-1].Left+X*DefaultColWidth;else Columns[0].Left=0;
	}
	bufColCount=ColCount;bufRowCount=RowCount;//urèeno pøi další realokaci pole
}
//---------------------------------------------------------------------------
//pøetížená metoda - vytvoøí tabulku s pøedepsaným poètem sloupcù a øádkù
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
//odstraní tabulku, pøidružené komponenty a ukazatel na mGrid z pamìti
void TmGrid::Delete(TForm *Form)
{
	//odstranìní v tabulce použitých komponent
	DeleteComponents(Form);
	//uvolnìní pamìti
	Delete();
	mGrid=NULL; delete mGrid;
}
//---------------------------------------------------------------------------
//odstraní tabulku
void TmGrid::Delete()
{
	for(unsigned long X=0;X<ColCount;X++)//po øádcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			Cells[X][Y].TopBorder=NULL;			delete Cells[X][Y].TopBorder;
			Cells[X][Y].BottomBorder=NULL;  delete Cells[X][Y].BottomBorder;
			Cells[X][Y].LeftBorder=NULL;    delete Cells[X][Y].LeftBorder;
			Cells[X][Y].RightBorder=NULL;   delete Cells[X][Y].RightBorder;
		}
		Cells[X]=NULL; delete Cells[X];
	}
	Cells=NULL; delete Cells;
	Columns=NULL; delete Columns;
	Rows=NULL; delete Rows;
	DefaultCell.TopBorder=NULL;			delete DefaultCell.TopBorder;
	DefaultCell.BottomBorder=NULL;  delete DefaultCell.BottomBorder;
	DefaultCell.LeftBorder=NULL;    delete DefaultCell.LeftBorder;
	DefaultCell.RightBorder=NULL;   delete DefaultCell.RightBorder;
}
//---------------------------------------------------------------------------
//zajistí vykreslení celé tabulky
void TmGrid::Show(TForm *Form)
{
	if(mGrid!=NULL)
	{
		if(ColCount!=bufColCount || RowCount!=bufRowCount)//pokud dojde k požadavku na zmìnu poètu øádkù a sloupcù volá se realokace
		{
			realock(Form);
		}

		if(!AntiAliasing)DrawGrid(Form,1);
//	else
//	{
//		Cantialising a;
//		Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
//    pozor width a height potøebu je znát hodnoty po autofit
//		bmp_in->Width=ColWidth*ColCount*3;bmp_in->Height=RowHeight*RowCount*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
//		DrawGrid(Canvas,Form,3);
//		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvùli smazání bitmapy vracené AA
//		Canvas->Draw(Left,Top,bmp_out);
//		delete (bmp_out);//velice nutné
//		delete (bmp_in);//velice nutné
//	}
	}
}
//---------------------------------------------------------------------------
//zajistí vykreslení celé tabulky
void TmGrid::DrawGrid(TForm *Form,double Zoom)
{
	TCanvas *C=Form->Canvas;//pouze zkrácení zapisu

	//autofit sloupcù nastaví Columns[X].ColWidth
	executeColumnsAutoFit(Form->Canvas);

	//////BUÒKY jednotlivé
	for(unsigned long X=0;X<ColCount;X++)//po sloupcích
	{
		////oblast buòky
		TRect R;
		if(X>0)Columns[X].Left=Columns[X-1].Left+Columns[X-1].Width;else Columns[0].Left=0;//výpoèet levého okraje buòky dle buòky pøedchozí
		R.Left	=	Left+Columns[X].Left*Zoom;
		R.Right	=	Left+(Columns[X].Left+Columns[X].Width)*Zoom;

		for(unsigned long Y=0;Y<RowCount;Y++)//po øádcích
		{
			////oblast buòky
			if(Y>0)Rows[Y].Top=Rows[Y-1].Top+Rows[Y-1].Height;else Rows[0].Top=0;//výpoèet horního okraje buòky dle buòky pøedchozí
			R.Top		=	Top+Rows[Y].Top*Zoom;
			R.Bottom=	Top+(Rows[Y].Top+Rows[Y].Height)*Zoom;

			////barva pozadí buòky
			C->Brush->Color=Cells[X][Y].Background->Color;
			C->Brush->Style=Cells[X][Y].Background->Style;
			C->FillRect(R);

			////komponenta v buòce
			SetComponents(Form,R,X,Y,Cells[X][Y]);

			////orámování buòky
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

	//////TABULKA - orámování celé + výpoèet celkové šíøky a výšky
	TBorder *B=new TBorder;*B=Border;
	SetBorder(C,B);B=NULL;delete B;
	Width=getWidth();//zároveò nastavuje celkovou hodnotu Width
	Height=getHeight();//zároveò nastavuje celkovou hodnotu Height
	C->MoveTo(Left,Top);C->LineTo(Width+Left,Top);C->LineTo(Width+Left,Top);C->LineTo(Width+Left,Height+Top);C->LineTo(Left,Height+Top);C->LineTo(Left,Top);

	C=NULL; delete C;
}
//---------------------------------------------------------------------------
//nastaví grafické pero na požadované parametry
void TmGrid::SetBorder(TCanvas *C,TBorder *Border)
{
	DeleteObject(C->Pen->Handle);//zruší pùvodní pero       //PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE
	DWORD pStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE /*| PS_JOIN_BEVEL*/ | PS_INSIDEFRAME;
	DWORD pWidth = Border->Width;

	LOGBRUSH lBrush;
	lBrush.lbStyle = BS_SOLID;
	lBrush.lbColor = Border->Color;
	lBrush.lbHatch = 0;

	C->Pen->Handle = ExtCreatePen(pStyle, pWidth, &lBrush, NULL, NULL);
}
//---------------------------------------------------------------------------
//nastaví danou buòku dle typu
void TmGrid::SetComponents(TForm *Form,TRect R,unsigned long X,unsigned long Y,TCells Cell)
{
	switch(Cell.Type)
	{
		case DRAW:
		{   //Cell.Text=getTag(X,Y);//provizorní
			TCanvas *Canv=Form->Canvas;//pouze zkrácení zapisu
			//nastavení fontu
			Canv->Font=Cell.Font;
			//SetBkMode(canv->Handle,OPAQUE);//nastvení netransparentního pozadí
			Canv->Brush->Color=clWhite;Canv->Brush->Style=bsClear;//nastvení netransparentního pozadí
			//zarovnání
			//samotný výpis
			//Canv->TextRect(R,0,0,"test"); - nefunguje
			long L=R.Left,T=R.Top;
			unsigned int W=Canv->TextWidth(Cell.Text);
			unsigned int H=Canv->TextHeight(Cell.Text);
			if(Cell.Font->Orientation==900){W=H;H=Canv->TextWidth(Cell.Text);}
			switch(Cell.Align)
			{
				case LEFT:	L=L+Cell.LeftMargin;break;
				case CENTER:L=(R.Left+R.Right)/2-W/2;break;
				case RIGHT:	L=R.Right-W-Cell.RightMargin;break;
			}
			switch(Cell.Valign)
			{
				case TOP:		T=T+Cell.TopMargin;break;
				case MIDDLE:T=(R.Top+R.Bottom)/2-H/2;break;
				case BOTTOM:T=R.Bottom-H-Cell.BottomMargin;break;
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
			//založení + tag + název
			TscGPButton *B=getButton(X,Y,Form);//pokud již existuje
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
			B->Parent=Form;//musí být až na konci
			B=NULL;delete B;
		}break;
		case COMBO:
		{
			//založení + tag + název
			TscGPComboBox *C=getCombo(X,Y,Form);//pokud již existuje
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
//			C->ItemIndex=1;//nelze pøedoznaèní první položku
			//vlastník
			C->Parent=Form;//musí být až na konci
			C=NULL;delete C;
		}break;
		case CHECK:
		{
			//založení + tag + název
			TscGPCheckBox *Ch = getCheck(X,Y,Form);//pokud již existuje
			if(Ch==NULL)
			{
				Ch = new TscGPCheckBox(Form);//pokud ne
				Ch->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
				Ch->Name="mGrid_CHECK_"+AnsiString(Ch->Tag);
				//události
				Ch->OnClick=&getTagOnClick;
				Ch->OnEnter=&getTagOnEnter;
			}
			//atributy
			switch(Cell.Align)
			{
				case LEFT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;break;
				case CENTER:Ch->Width=Ch->OptionsChecked->ShapeSize;Ch->Left=R.Left+Columns[X].Width/2-Ch->Width/2;break;
				case RIGHT:	Ch->Width=Columns[X].Width-2;Ch->Left=R.Left+1;Ch->BiDiMode=bdRightToLeft;break;
			}
			switch(Cell.Valign)
			{
				case TOP:		Ch->Top=R.Top+1;Ch->Height=Ch->OptionsChecked->ShapeSize;break;
				case MIDDLE:Ch->Top=R.Top+1;Ch->Height=Rows[Y].Height-2;break;
				case BOTTOM:Ch->Height=Ch->OptionsChecked->ShapeSize;Ch->Top=R.Top+Rows[Y].Height-Ch->Height;break;
			}
//			Ch->Options->NormalColor=Cell.Background->Color;
//			Ch->Options->NormalColorAlpha=255;
//			Ch->Options->FrameNormalColor=clWhite;
//			Ch->Options->FrameNormalColorAlpha=255;
			Ch->Font=Cell.Font;
			Ch->Caption=Cell.Text;
			//vlastník
			Ch->Parent=Form;//musí být až na konci
			Ch=NULL;delete Ch;
		}break;
		case RADIO:
		{
			//založení + tag + název
			TscGPRadioButton *Ra = getRadio(X,Y,Form);//pokud již existuje
			if(Ra==NULL)//pokud ne
			{
				Ra = new TscGPRadioButton(Form);
				Ra->Tag=getTag(X,Y);//vratí ID tag komponenty,absolutní poøadí v pamìti
				Ra->Name="mGrid_RADIO_"+AnsiString(Ra->Tag);

				//události
				Ra->OnClick=&getTagOnClick;
				Ra->OnEnter=&getTagOnEnter;
			}
			//atributy
			switch(Cell.Align)
			{
				case LEFT:	Ra->Width=Columns[X].Width-2;Ra->Left=R.Left+1;break;
				case CENTER:Ra->Width=Ra->OptionsChecked->ShapeSize;Ra->Left=R.Left+Columns[X].Width/2-Ra->Width/2;break;
				case RIGHT:	Ra->Width=Columns[X].Width-2;Ra->Left=R.Left+1;Ra->BiDiMode=bdRightToLeft;break;
			}
			switch(Cell.Valign)
			{
				case TOP:		Ra->Top=R.Top+1;Ra->Height=Ra->OptionsChecked->ShapeSize;break;
				case MIDDLE:Ra->Top=R.Top+1;Ra->Height=Rows[Y].Height-2;break;
				case BOTTOM:Ra->Height=Ra->OptionsChecked->ShapeSize;Ra->Top=R.Top+Rows[Y].Height-Ra->Height;break;
			}
//			Ra->Options->NormalColor=Cell.Background->Color;
//			Ra->Options->NormalColorAlpha=255;
//			Ra->Options->FrameNormalColor=clWhite;
//			Ra->Options->FrameNormalColorAlpha=255;
			Ra->Font=Cell.Font;
			Ra->Caption=Cell.Text;
			//vlastník
			Ra->Parent=Form;//musí být až na konci
			Ra=NULL;delete Ra;
		}break;
	}
}
//---------------------------------------------------------------------------
//nastaví danou buòku na edit, pomocná metoda výše uvedené
void TmGrid::SetEdit(TForm *Form,TRect R,unsigned long X,unsigned long Y,TCells Cell)
{
	//založení + tag + název
	TscGPEdit *E=getEdit(X,Y,Form);//pokud již existuje
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
	//vlastník
	E->Parent=Form;//musí být až na konci
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//nastaví danou buòku na numericedit, pomocná metoda objednu výše uvedené
void TmGrid::SetNumeric(TForm *Form,TRect R,unsigned long X,unsigned long Y,TCells Cell)
{
	//založení + tag + název
	TscGPNumericEdit *N=getNumeric(X,Y,Form);//pokud již existuje
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
	//vlastník
	N->Parent=Form;//musí být až na konci
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
	return Tag%ColCount;
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
					Canv->Font=Cells[X][Y].Font;
					unsigned int W=Canv->TextWidth(Cells[X][Y].Text)+Cells[X][Y].LeftMargin+Cells[X][Y].RightMargin;
					if(Cells[X][Y].Type==CHECK || Cells[X][Y].Type==RADIO)W+=20+Cells[X][Y].LeftMargin+Cells[X][Y].RightMargin;//zámìrnì podruhé
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
	Canv->Font=Cells[ColIdx][0].Font;
	unsigned int ColWidth=Canv->TextWidth(Cells[ColIdx][0].Text); //výchozí hodnota
	for(unsigned long Y=1;Y<RowCount;Y++)
	{
		Canv->Font=Cells[ColIdx][Y].Font;
		unsigned int W=Canv->TextWidth(Cells[ColIdx][Y].Text);
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
//vrátí celkovou výšku tabulky
unsigned long TmGrid::getHeight()
{
	return Rows[RowCount-1].Top+Rows[RowCount-1].Height;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//dle zadaného  èísla sloupce a èísla øádku  vrátí ukazatel nadanou komponentu
TscGPEdit *TmGrid::getEdit(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPEdit *)Form->FindComponent("mGrid_EDIT_"+AnsiString(getTag(Col,Row)));}
TscGPButton *TmGrid::getButton(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPButton *)Form->FindComponent("mGrid_BUTTON_"+AnsiString(getTag(Col,Row)));}
TscGPComboBox *TmGrid::getCombo(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPComboBox *)Form->FindComponent("mGrid_COMBO_"+AnsiString(getTag(Col,Row)));}
TscGPCheckBox *TmGrid::getCheck(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPCheckBox *)Form->FindComponent("mGrid_CHECK_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPRadioButton *TmGrid::getRadio(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPRadioButton *)Form->FindComponent("mGrid_RADIO_"+AnsiString(getTag(Col,Row)));}//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
TscGPNumericEdit *TmGrid::getNumeric(unsigned long Col,unsigned long Row,TForm *Form){return (TscGPNumericEdit *)Form->FindComponent("mGrid_NUMERIC_"+AnsiString(getTag(Col,Row)));};//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnClick(TObject *Sender)
{
	//ShowMessage(AnsiString("OnClick ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
	Form2->OnClick(Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnEnter(TObject *Sender)
{
	//ShowMessage(AnsiString("OnEnter ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
	Form2->OnEnter(Col,Row);
}
//---------------------------------------------------------------------------
void __fastcall TmGrid::getTagOnChange(TObject *Sender)
{
	//ShowMessage(AnsiString("OnChange ")+IntToStr(((TComponent*)(Sender))->Tag));
	Col=getColFromTag(((TComponent*)(Sender))->Tag);
	Row=getRowFromTag(((TComponent*)(Sender))->Tag);
	Form2->OnChange(Col,Row);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//spojí dvì buòky do jedné
void TmGrid::MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
//	for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
//	{
//		for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
//		{
//			UnicodeString Text=Cells[X][Y].Text;//záloha textu
//			Cells[X][Y]=RefCell;
//			Cells[X][Y].Text=Text;//navrácení textu
//		}
//	}
//	 if(ColCell_1==ColCell_2)//varianta pod sebou
//	 {
//		 Cells[ColCell_1][RowCell_1].BottomBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
//		 Cells[ColCell_2][RowCell_2].TopBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
//		 //zanést do buòky 1-1, asi novou šíøku
//	 }
//	 if(RowCell_1==RowCell_2)//varianta vedle sebe
//	 {
//		 Cells[ColCell_1][RowCell_1].RightBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
//		 Cells[ColCell_2][RowCell_2].LeftBorder.Color=Cells[ColCell_1][RowCell_1].Background->Color;
//	 }
//	 Cells[ColCell_2][RowCell_2].Background->Color=Cells[ColCell_1][RowCell_1].Background->Color;
}
//---------------------------------------------------------------------------
//nastaví oblast bunìk totožnými vlastnostmi dle referenèní buòky (text zanechá), zaèínat zadávat od nejvyšší a nejvíce vlevo
void TmGrid::SetCells(TCells RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2)
{
	for(unsigned long X=ColCell_1;X<=ColCell_2;X++)
	{
		for(unsigned long Y=RowCell_1;Y<=RowCell_2;Y++)
		{
			UnicodeString Text=Cells[X][Y].Text;//záloha textu
			Cells[X][Y]=RefCell;
			Cells[X][Y].Text=Text;//navrácení textu
		}
	}
}
//---------------------------------------------------------------------------
void TmGrid::HighlightCell(unsigned long Col,unsigned long Row,unsigned short Width,TColor Color)
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
//smaže text v celém øádku
void TmGrid::ClearRow(unsigned long RowIdx)
{
	for(unsigned long X=0;X<=ColCount;X++)
	{
		Cells[X][RowIdx].Text="";
	}
}
//---------------------------------------------------------------------------
//smaže text celé tabulku
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
void TmGrid::realock(TForm *Form)
{
	//kopie do záložního pole a smazání
	TCells **bufCells = new TCells*[bufColCount];//vytvoøí dynamické pole ukazatelu typu TCells dle poètu požadovaných sloupcù
	for(unsigned long X=0;X<bufColCount;X++)
	{
		bufCells[X] = new TCells[bufRowCount];//pruchod po sloupcich, slupcùm dynamamického pole alokuje pamìt pro jednotlivé øádky- cyklus musí být samostatnì
		bufCells[X] = Cells[X];//zkopírování pùvodních hodnot do zálohy
	}

	unsigned long bufColCount2=ColCount;unsigned long bufRowCount2=RowCount;
	ColCount=bufColCount;RowCount=bufRowCount;//provizorní vrácení
	DeleteComponents(Form);
	Delete();
	ColCount=bufColCount2;RowCount=bufRowCount2;//zpìt

	//vytvoøení nového realokovaného pole
	bufColCount2=bufColCount;bufRowCount2=bufRowCount;//urèeno pøi další realokaci pole, create totiž pøepisuje buf hodnoty
	Create();

	//zkopírování pùvodních hodnot zpìt
	if(bufColCount2>ColCount)bufColCount=ColCount;else bufColCount=bufColCount2;//podle toho, co je menší
	if(bufRowCount2>RowCount)bufRowCount=RowCount;else bufRowCount=bufRowCount2;//podle toho, co je menší
	for(unsigned long X=0;X<bufColCount;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount;Y++)
		{
			Cells[X][Y] = bufCells[X][Y];
		}
	}

	//smazání pùvodního bufCells
	for(unsigned long X=0;X<bufColCount2;X++)
	{
		for(unsigned long Y=0;Y<bufRowCount2;Y++)//po øádcích
		{
			Cells[X][Y].TopBorder=NULL;		 delete Cells[X][Y].TopBorder;
			Cells[X][Y].BottomBorder=NULL; delete Cells[X][Y].BottomBorder;
			Cells[X][Y].LeftBorder=NULL;   delete Cells[X][Y].LeftBorder;
			Cells[X][Y].RightBorder=NULL;  delete Cells[X][Y].RightBorder;
		}
		bufCells[X]=NULL; delete bufCells[X];
	}
	bufCells=NULL; delete bufCells;
	bufColCount=ColCount;bufRowCount=RowCount;//urèeno pøi další realokaci pole
}
//---------------------------------------------------------------------------
//odstraní dynamicky vytoøené komponenty, nutno volat pøed Delete()
void TmGrid::DeleteComponents(TForm *Form)
{
	for(unsigned long X=0;X<ColCount;X++)//po øádcích
	{
		for(unsigned long Y=0;Y<RowCount;Y++)//po sloupcích
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
				case RADIO:{TscGPRadioButton *R=getRadio(X,Y,Form);R->Free();R=NULL;delete R;}break;
			}
		}
	}
}
//---------------------------------------------------------------------------

