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

	struct TmBorder
	{
		TColor Color;
		unsigned short Width;
		TPenStyle Style;
	};

	struct TColumns
	{
	 unsigned short	ColWidth;
	 long Left;
	};

	struct TCells
	{
		Ttype Type;
		TmBorder TopBorder;
		TmBorder BottomBorder;
		TmBorder LeftBorder;
		TmBorder RightBorder;
		TBrush *Background;
		Talign Align;
		Tvalign Valign;
		TFont *Font;
		UnicodeString Text;
	};

	//metody
	TmGrid();//konstruktor
	~TmGrid();//destruktor
	void Create();//vytvoøí tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//pøetížená metoda - vytvoøí tabulku s pøedepsaným poètem sloupcù a øádkù
	void Delete();//odstraní tabulku
	void Show(TForm *Form);//zajistí vykreslení celé tabulky
	void SetColumnAutoFit(long ColIdx=-1);//nastaví šíøku bunìk daného sloupce dle nejdelšího šíøky textu buòky ze sloupce uvedeného parametrem ColIdx sloupce
	void ClearColumn(unsigned long ColIdx);//smaže celý sloupec
	void ClearRow(unsigned long RowIdx);//smaže celý øádek
	void Clear();//smaže text celé tabulku
	void MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//spojí dvì buòky do jedné
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
	unsigned long Row,Col;//aktuální øádek a sloupec
	TmBorder Border;//orámování celé tabulky
	bool AntiAliasing;//ano x ne
	TCells **Cells;//alokace dvourozmerneho dynamickeho pole bunìk
	TColumns *Columns;//alokace jednorozmìrneho dynamickeho pole sloupcù
	TCells DefaultCell;//deafultní vzorová buòka, podle ní se nastaví všechny

 protected:
	long Width,Height;//velikost komponenty. lze jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat

 private:
	void __fastcall getTagOnClick(TObject *Sender);
	void __fastcall getTagOnEnter(TObject *Sender);
	void __fastcall getTagOnChange(TObject *Sender);

	void DrawGrid(TForm *Form,double Zoom);//zajistí vykreslení celé tabulky
	void SetBorder(TCanvas *C,TmBorder Border);//nastaví grafické pero na požadované parametry
	void SetComponents(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastaví danou buòku dle typu
	void SetEdit(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastaví danou buòku na edit, pomocná metoda výše uvedené
	void SetNumeric(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastaví danou buòku na numericedit, pomocná metoda objednu výše uvedené
	void rcc(unsigned long cc,unsigned long rc);//pouze obejití lokální promìnné, v c++ je na to nìjaké klíèové slovo, ale nevzpomenu si
	void DeleteComponents(TForm *Form);//odstraní dynamicky vytoøené komponenty
	void executeColumnsAutoFit(TCanvas *Canv);//nastaví šíøku bunìk sloupcù dle šíøky textu dle zvoleného parametru
	void executeColumnAutoFit(TCanvas *Canv,long ColIdx);//nastaví šíøku bunìk daného sloupce dle šíøky textu v daném sloupci
  void realock(TForm *Form);//zajistí realokaci pole Cells dle nové velikosti

	unsigned long getWidth();//vrátí celkovou šíøku tabulky
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
