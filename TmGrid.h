//---------------------------------------------------------------------------
#ifndef TmGridH
#define TmGridH
//#include <vcl.h>
#include "scGPControls.hpp"//knihovna kvùli buttonum atp.
#include "scGPExtControls.hpp"//knihovna kvùli editbox;m
//---------------------------------------------------------------------------
class TmGrid
{
 public:
	enum Ttype{readEDIT,EDIT,NUMERIC,readNUMERIC,BUTTON,COMBO,CHECK,RADIO,/*IMAGE,*/DRAW};
	enum Talign{LEFT,CENTER,RIGHT};
	enum Tvalign{TOP,MIDDLE,BOTTOM};

	struct TBorder
	{
		TColor Color;
		unsigned short Width;
		TPenStyle Style;
	};

	struct TColumns
	{
	 unsigned short	Width;
	 long Left;
	};

	struct TRows
	{
	 unsigned short	Height;
	 long Top;
	};

	struct TCells
	{
		Ttype Type;
		TBorder *TopBorder;
		TBorder *BottomBorder;
		TBorder *LeftBorder;
		TBorder *RightBorder;
		TBrush *Background;
		Talign Align;
		Tvalign Valign;
		short TopMargin;
		short BottomMargin;
		short LeftMargin;
		short RightMargin;
		TFont *Font;
		UnicodeString Text;
	};

	//metody
	TmGrid();//konstruktor
	~TmGrid();//destruktor
	void Create();//vytvoøí tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//pøetížená metoda - vytvoøí tabulku s pøedepsaným poètem sloupcù a øádkù
	void Delete(TForm *Form);//odstraní tabulku, pøidružené komponenty a ukazatel na mGrid z pamìti
	void Show(TForm *Form);//zajistí vykreslení celé tabulky
	void SetColumnAutoFit(long ColIdx=-1);//nastaví šíøku bunìk daného sloupce dle parametru ColIdx, -3 = nepøizpùsobuje se velikost a užije se defaultColWidth,-2 všechny sloupce stejnì podle nejširšího textu, -1 pøizpùsobuje se každý sloupec individuálnì, 0 a více jen konkrétní sloupec uvedený pomoc ColIdx
	void ClearColumn(unsigned long ColIdx);//smaže text v celém sloupec
	void ClearRow(unsigned long RowIdx);//smaže text v celém øádku
	void Clear();//smaže text celé tabulky
	void MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//spojí oblast bunìk do jedné buòky,text a vlastnosti pøevezmé od levé horní, zaèínat zadávat od nejvyšší a nejvíce vlevo
	void SetCells(TCells RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//nastaví oblast bunìk totožnými vlastnostmi dle referenèní buòky (text zanechá), zaèínat zadávat od nejvyšší a nejvíce vlevo
	void HighlightCell(unsigned long Col,unsigned long Row,unsigned short Width=2,TColor Color=clRed);//zajistí zvýraznìní dané buòky
	TscGPEdit *getEdit(unsigned long Col,unsigned long Row,TForm *Form);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPButton *getButton(unsigned long Col,unsigned long Row,TForm *Form);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPComboBox *getCombo(unsigned long Col,unsigned long Row,TForm *Form);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPCheckBox *getCheck(unsigned long Col,unsigned long Row,TForm *Form);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPRadioButton *getRadio(unsigned long Col,unsigned long Row,TForm *Form);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPNumericEdit *getNumeric(unsigned long Col,unsigned long Row,TForm *Form);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu

	//promìnné
	long Left,Top;//umístìní celé komponenty
	unsigned long ColCount,RowCount;//poèet øádkù a sloupcù
	unsigned short DefaultColWidth,DefaultRowHeight;//výchozí výška a šíøka øádku
	TBorder Border;//orámování celé tabulky
	bool AntiAliasing;//ano x ne - zatím nefunguje
	TCells **Cells;//alokace dvourozmerneho dynamickeho pole bunìk
	TColumns *Columns;//alokace jednorozmìrneho dynamickeho pole sloupcù
	TRows *Rows;//alokace jednorozmìrneho dynamickeho pole øádkù
	TCells DefaultCell;//deafultní vzorová buòka, podle ní se nastaví všechny

 //protected: - nefugovalo, jak jsme si pøedstavoval
	long Width,Height;//velikost komponenty, jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat
	unsigned long Row,Col;//aktuální øádek a sloupec, jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat

 private:
	void __fastcall getTagOnClick(TObject *Sender);
	void __fastcall getTagOnEnter(TObject *Sender);
	void __fastcall getTagOnChange(TObject *Sender);

	void DrawGrid(TForm *Form,double Zoom);//zajistí vykreslení celé tabulky
	void SetBorder(TCanvas *C,TBorder *Border);//nastaví grafické pero na požadované parametry
	void SetComponents(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastaví danou buòku dle typu
	void SetEdit(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastaví danou buòku na edit, pomocná metoda výše uvedené
	void SetNumeric(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastaví danou buòku na numericedit, pomocná metoda objednu výše uvedené
	void rcc(unsigned long cc,unsigned long rc);//pouze obejití lokální promìnné, v c++ je na to nìjaké klíèové slovo, ale nevzpomenu si
	void Delete();//odstraní tabulku z pamìti
	void DeleteComponents(TForm *Form);//odstraní dynamicky vytoøené komponenty, nutno volat pøed Delete()
	void executeColumnsAutoFit(TCanvas *Canv);//nastaví šíøku bunìk sloupcù dle šíøky textu dle zvoleného parametru
	void executeColumnAutoFit(TCanvas *Canv,long ColIdx);//nastaví šíøku bunìk daného sloupce dle šíøky textu v daném sloupci
  void realock(TForm *Form);//zajistí realokaci pole Cells dle nové velikosti

	unsigned long getWidth();//vrátí celkovou šíøku tabulky
	unsigned long getHeight();//vrátí celkovou výšku tabulky
	unsigned long getTag(unsigned long Col,unsigned long Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
	unsigned long getColFromTag(unsigned long Tag);//z tagu vratí èíslo sloupce
	unsigned long getRowFromTag(unsigned long Tag);//z tagu vratí èíslo øádku

	unsigned long bufColCount,bufRowCount;//pøedchozí poèet øádkù a sloupcù
	short SetColumnAutoFitColIdx;//typ autofit column
};
//---------------------------------------------------------------------------
extern TmGrid *mGrid;//ukazatel na celou knihovnu     PACKAGE
//---------------------------------------------------------------------------
#endif
