//---------------------------------------------------------------------------
#ifndef TmGridH
#define TmGridH
//#include <vcl.h>
#include "scGPControls.hpp"//knihovna kvùli buttonumatp.
#include "scGPExtControls.hpp"//knihovna kvùli editbox
//---------------------------------------------------------------------------
class TmGrid
{
 public:
	enum Ttype{readEDIT,EDIT,NUMERIC,readNUMERIC,BUTTON,COMBO,CHECK,RADIO,/*IMAGE,*/DRAW};
	enum Talign{aNO,LEFT,CENTER,RIGHT};
	enum Tvalign{vNO,TOP,MIDDLE,BOTTOM};

	struct TBorder//datovı typ pouívanı pro orámování bunìk
	{
		TColor Color;//barva orámování
		unsigned short Width;//šíøka orámování
		TPenStyle Style;//styl èáry
	};

	struct TColumns//datovı typ pouívanı pro sloupce
	{
	 unsigned short	Width;//šíøka sloupce
	 long Left;//umístìní sloupce v px, v rámci tabulky
	};

	struct TRows//datovı typ pouívanı pro øádky
	{
	 unsigned short	Height;//vıška øádku
	 long Top;//umístìní øádku v px, v rámci tabulky
	};

	struct TCells//datovı typ pouívanı pro buòky
	{
		Ttype Type;//typ komponenty v buòce dle Ttype
		TBorder *TopBorder;//horní orámování a jeho vlastnosti dle TBorder ukazuje na stejnı objekt jako dolní okraj buòky pøedchozí na ose Y
		TBorder *BottomBorder;//dolní orámování a jeho vlastnosti dle TBorder ukazuje na stejnı objekt jako horní okraj buòky následující na ose Y
		TBorder *LeftBorder;//levé orámování a jeho vlastnosti dle TBorder ukazuje na stejnı objekt jako pravı okraj buòky pøedchozí na ose X
		TBorder *RightBorder;//pravé orámování a jeho vlastnosti dle TBorder ukazuje na stejnı objekt jako levı okraj buòky následující na ose X
		TBrush *Background;//vlastnosti pozadí buòky
		Talign Align;//horizontální zarovnání textu
		Tvalign Valign;//vertikální zarovnání textu
		short TopMargin;//horní odsazení textu
		short BottomMargin;//dolní odsazení textu
		short LeftMargin;//levé odsazení textu
		short RightMargin;//pravé odsazení textu
		bool MergeState;//pouze indikuje, zda je buòka slouèena, èi nikoliv, slouí jako pomùcka pøi vykreslování orámování slouèenıch bunìk, zatím zùstává nevyuito
		TFont *Font;//vlastnosti fontu v buòce
		TPoint TextPositon;//pozice textu v buòce (levı horní roh buòky je 0,0)
		UnicodeString Text;//samotnı text buòky
	};

	//metody
	TmGrid(TForm *Owner);//konstruktor
	~TmGrid();//destruktor
	void Create();//vytvoøí tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//pøetíená metoda - vytvoøí tabulku s pøedepsanım poètem sloupcù a øádkù
	void Delete();//odstraní tabulku, pøidruené komponenty a ukazatel na mGrid z pamìti
	void Show();//zajistí vykreslení celé tabulky
	void Refresh();//zajistí vyvolání pøekreslení celé tabulky s pøeblikem, ale lépe pouívat pøímo ve v daném formuláøi FormPaint(this), co zajistí pøekreslení bez probliku
	void SetColumnAutoFit(long ColIdx=-1);//nastaví šíøku bunìk daného sloupce dle parametru ColIdx, -3 = nepøizpùsobuje se velikost a uije se defaultColWidth,-2 všechny sloupce stejnì podle nejširšího textu, -1 pøizpùsobuje se kadı sloupec individuálnì, 0 a více jen konkrétní sloupec uvedenı pomoc ColIdx
	void ClearColumn(unsigned long ColIdx);//smae text v celém sloupec
	void ClearRow(unsigned long RowIdx);//smae text v celém øádku
	void Clear();//smae text celé tabulky
	void AddRow();//pøidá øádek za poslední øádek
	void InsertRow(long Row);//pøídá øádek za øádek uvedenı dle parametru Row
	void DeleteRow(long Row);//smae celı øádek
	void MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//spojí oblast bunìk do jedné buòky,text a vlastnosti pøevezmé od levé horní, zaèínat zadávat od nejvyšší a nejvíce vlevo
	void SetCells(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,short setText=0,bool copyComponent=false);//nastaví oblast bunìk totonımi vlastnostmi dle referenèní buòky, text podle posledního parametru buï -1 -smae, 0 - zanechá pùvodní (implicitnì), 1 zkopíruje všude stejnı), zaèínat zadávat od nejvyšší a nejvíce vlevo
	void SetRegion(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//totonì ohranièí danou oblast bunìk dle referenèní buòky (zohledòuje i rozdíly horní,dolní,levé pravé orámování), zaèínat zadávat od nejvyšší a nejvíce vlevo
	void CopyCell(TCells &RefCell,TCells &CopyCell,bool copyComponent=false);//zkopíruje obsah, formát a orámování z buòky na buòku (bez ukazatelového propojení)
	void CopyCells2Clipboard(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,UnicodeString Separator="\t");//zkopíruje danou oblast do schránky, buòky oddìlí separátorem
	void CopyAreaCell(TCells &RefCell,TCells &CopyCell,bool copyComponent=false);//zkopíruje obsah, formát (bez orámování) z buòky na buòku (bez ukazatelového propojení)
	void CopyBordesCell(TCells &RefCell,TCells &CopyCell);//zkopíruje orámování z buòky na buòku (bez ukazatelového propojení)
	void HighlightCell(unsigned long Col,unsigned long Row,TColor Color=clRed,unsigned short Width=2);//zajistí zvıraznìní dané buòky
	void HighlightRow(long Row,TColor Color=clYellow,bool SelFirstRow=false,bool unHighlightPrevRow=true);//zajistí zvıraznìní øádkù dle èísla øádku Row
	void HighlightRowOnMouse(int X,int Y,TColor Color=clYellow,bool SelFirstRow=false,bool unHighlightPrevRow=true);//zajistí zvıraznìní øádkù, pøes kterı se pøejídí myší
	TscGPEdit *getEdit(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPButton *getButton(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPComboBox *getCombo(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPCheckBox *getCheck(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPRadioButton *getRadio(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPNumericEdit *getNumeric(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPButton *createButton(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPRadioButton *createRadio(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPCheckBox *createCheck(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	long GetIdxRow(int X,int Y);//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí øádek
	long GetIdxColum(int X,int Y);//dle souøadnic ve formuláøi,kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí sloupec
	//promìnné
	long Tag;//ID komponenty (vyuitelné napø. pokud bude více tabulek, tak se bude vìdìt, v jaké došlo ke kliku)
	long Left,Top,preLeft,preTop;//umístìní celé komponenty
	unsigned long ColCount,RowCount;//poèet øádkù a sloupcù
	unsigned short DefaultColWidth,DefaultRowHeight;//vıchozí vıška a šíøka øádku
	TBorder Border;//orámování celé tabulky
	bool AntiAliasing_grid;//ano x ne
	bool AntiAliasing_text;//ano x ne
	TCells **Cells;//alokace dvourozmerneho dynamickeho pole bunìk
	TColumns *Columns;//alokace jednorozmìrneho dynamickeho pole sloupcù
	TRows *Rows;//alokace jednorozmìrneho dynamickeho pole øádkù
	TCells DefaultCell;//deafultní vzorová VIRTUÁLNÍ buòka, podle ní se nastaví všechny pøi prvním naètení tabulky, pokud není pøed Show() ještì nastaveno jinak

 //protected: - nefugovalo, jak jsme si pøedstavoval
	long Width,Height;//velikost komponenty, jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat
	unsigned long Row,Col;//aktuální øádek a sloupec, jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat

 private:
	TForm *Form;

	void __fastcall getTagOnClick(TObject *Sender);//vrací událost pøi OnClick
	void __fastcall getTagOnEnter(TObject *Sender);//vrací událost pøi OnEnter
	void __fastcall getTagOnChange(TObject *Sender);//vrací událost pøi OnChange
	void __fastcall getTagOnKeyDown(TObject *Sender);//vrací událost pøi OnKeyDown - NEDOKONÈENO

	void Draw(TCanvas *C);//zajistí vykreslení celé tabulky vèetnì gridu
	void DrawGrid(TCanvas *C);//zajistí vykreslení jen gridu
	void SetColRow();//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøebe
	void SetBorder(TCanvas *C,TBorder *Border);//nastaví grafické pero na poadované parametry
	void SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku dle typu
	void SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku na edit, pomocná metoda vıše uvedené
	void SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku na numericedit, pomocná metoda objednu vıše uvedené
	void rcc(unsigned long cc,unsigned long rc);//pouze obejití lokální promìnné, v c++ je na to nìjaké klíèové slovo, ale nevzpomenu si
	void CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell);//patøiènì prolinkuje orámování, e sousední orámování má ukazatel na totonı objekt, vzor orámvání získá dle refCell
	void CreateCell(TCells &NewCell);//vytvoøí novou buòku (alokuje ji pamì)
	void DeleteCell(TCells &DelCell);//smae buòku z pamìti
	void DeleteTable();//odstraní pouze tabulku z pamìti
	void DeleteComponents();//odstraní dynamicky vytoøené komponenty, nutno volat pøed Delete()
	void DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow);//odstraní dynamicky vytoøené komponenty do poètu sloupcù a øádkù, nutno volat pøed Delete()
	void executeColumnsAutoFit(TCanvas *Canv);//nastaví šíøku bunìk sloupcù dle šíøky textu dle zvoleného parametru
	void executeColumnAutoFit(TCanvas *Canv,long ColIdx);//nastaví šíøku bunìk daného sloupce dle šíøky textu v daném sloupci
	void realock();//zajistí realokaci pole Cells dle nové velikosti
	void rotace_textu(long rotace);//zajistí rotaci textu
	void selRow(long Row,TColor Color,bool newSel);//oznaèí øádek, nebo zruší oznaèení øádku dle vstupního parametru

	unsigned long getWidth();//vrátí celkovou šíøku tabulky
	unsigned long getHeight();//vrátí celkovou vıšku tabulky
	TPoint getWidthHeightText(TCells &Cell);//vrátí šíøku a vıšku textu buòky v pixelech
	unsigned long getTag(unsigned long Col,unsigned long Row);//vratí ID tag komponenty,absolutní poøadí v pamìti
	unsigned long getColFromTag(unsigned long Tag);//z tagu vratí èíslo sloupce
	unsigned long getRowFromTag(unsigned long Tag);//z tagu vratí èíslo øádku

	unsigned long bufColCount,bufRowCount;//pøedchozí poèet øádkù a sloupcù
	short SetColumnAutoFitColIdx;//typ autofit column
	long preRowInd;//pøedchozí øádek na kterém byla myš

};
//---------------------------------------------------------------------------
extern TmGrid *mGrid;//ukazatel na celou knihovnu
//---------------------------------------------------------------------------
#endif
