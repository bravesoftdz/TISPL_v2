//---------------------------------------------------------------------------
#ifndef TmGridH
#define TmGridH
//#include <vcl.h>
#include "scGPControls.hpp"//knihovna kvùli buttonumatp.
#include "scGPExtControls.hpp"//knihovna kvùli editbox
#include "scHtmlControls.hpp"//knihovna kvùli scHTMLLabel
#include "MyString.h"//kvùli parsování
#include "my.h"
//---------------------------------------------------------------------------
class TmGrid
{
 public:
	enum Ttype{readEDIT,EDIT,NUMERIC,readNUMERIC,BUTTON,glyphBUTTON,COMBO,CHECK,RADIO,/*IMAGE,*/DRAW,LABEL};
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
	 bool Visible;//zda bude sloupec zobrazen
	};

	struct TRows//datovı typ pouívanı pro øádky
	{
	 unsigned short	Height;//vıška øádku
	 long Top;//umístìní øádku v px, v rámci tabulky
	 //není dokonèeno bool Visible;//zda bude sloupec zobrazen
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
		bool InputNumbersOnly;//pokud je nastaveno na true, nelze vepsat jinou hodnotu ne èíselnou (to vèetnì reálného èísla)
		TFont *Font;//vlastnosti fontu v buòce
		TPoint TextPositon;//pozice textu v buòce (levı horní roh buòky je 0,0)
		TBrush *isEmpty;//podmínìné formátování, pozadí buòky, kdy je prázdná
		TFont *isNegativeNumber;//podmínìné formátování, pokud je zaporné èíslo
		TFont *isZero;//podmínìné formátování, pokud se jedná o nulové èíslo
		TFont *isLink;//podmínìné formátování v pøípadì vıskytu tagu <a> resp </a>
		TPoint LinkCoordinateStart;//kvùli uloení citelné oblasti pro link dané buòky
		TPoint LinkCoordinateEnd;//kvùli uloení citelné oblasti pro link dané buòky
		UnicodeString Text;//samotnı text buòky
	};

	//metody
	TmGrid(TForm *Owner);//konstruktor
	~TmGrid();//destruktor
	void Create();//vytvoøí tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//pøetíená metoda - vytvoøí tabulku s pøedepsanım poètem sloupcù a øádkù
	void Delete();//odstraní tabulku, pøidruené komponenty a ukazatel na mGrid z pamìti
	void Show(TCanvas *Canvas=NULL);//zajistí vykreslení celé tabulky
	void Refresh();//zajistí vyvolání pøekreslení celé tabulky s pøeblikem, ale lépe pouívat pøímo ve v daném formuláøi FormPaint(this), co zajistí pøekreslení bez probliku
	void SetColumnAutoFit(long ColIdx=-1);//nastaví šíøku bunìk daného sloupce dle parametru ColIdx, -4 nastavení velikosti sloupce dle ruèní nastavení šíøky daného sloupce, tj. nedìlat autofit, -3 = nepøizpùsobuje se velikost a uije se defaultColWidth,-2 všechny sloupce stejnì podle nejširšího textu, -1 pøizpùsobuje se kadı sloupec individuálnì, 0 a více jen konkrétní sloupec uvedenı pomoc ColIdx
	int  GetRecommendedColumnWidth(long ColIdx);//vratí doporuèenou šíøku sloupce dle jeho obsahu
	int  GetRecommendedColumnWidth(AnsiString Text);//vratí doporuèenou šíøku sloupce dle dodaného textu a aktuálního nastavení canvasu
	void ClearColumn(unsigned long ColIdx);//smae text v celém sloupec
	void ClearRow(unsigned long RowIdx);//smae text v celém øádku
	void Clear();//smae text celé tabulky
	void AddRow(bool copyComponentFromPreviousRow=true,bool invalidate=true);//pøidá øádek za poslední øádek, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z pøedchozího øádku, pokud je invalidate na true, automaticky po pøidání pøekreslí tabulku, nìkdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint pøímo za voláním metody AddRow pøimo v uitém formuláøi
	void InsertRow(unsigned long Row,bool copyComponentFromPreviousRow=true,bool invalidate=true);//pøídá øádek za øádek uvedenı dle parametru Row,, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z pøedchozího øádku, pokud je invalidate na true, automaticky po pøidání pøekreslí tabulku, nìkdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint pøímo za voláním metody InsertRow pøimo v uitém formuláøi
	void DeleteRow(unsigned long Row,bool invalidate=true);//smae celı øádek, pokud je invalidate na true, automaticky po pøidání pøekreslí tabulku, nìkdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint pøímo za voláním metody InsertRow pøimo v uitém formuláøi
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
	void HighlightTable(TColor Color=(TColor)RGB(43,87,154),unsigned short Size=2,unsigned short Offset=0,TPenMode PenMode=pmCopy);//zajistí zvıraznìní orámování tabulky
	void HighlightTableOnMouse(int X,int Y);//zajistí zvıraznìní orámování tabulky, pokud se do ni vstoupí myší
	void HighlightLink(unsigned long Col,unsigned long Row,short Intensive=-50);//zajistí pøebarvení odkazu v buòce odstínem barvy odkazu
	void SetVisibleComponents(bool state);//podle stavu state buï zobrazí nebo skryje všechny komponenty
	TscGPEdit *getEdit(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPNumericEdit *getNumeric(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPButton *getButton(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPGlyphButton *getGlyphButton(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPComboBox *getCombo(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPCheckBox *getCheck(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscGPRadioButton *getRadio(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	TscHTMLLabel *getLabel(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	void createComponent(Ttype Type, unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vytvoøenou komponentu dle Type, pokud existuje, tak se nic nedìje
	TscGPEdit *createEdit(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPNumericEdit *createNumeric(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPButton *createButton(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPGlyphButton *createGlyphButton(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPComboBox *createCombo(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPCheckBox *createCheck(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscGPRadioButton *createRadio(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel na danou vytvoøenou komponentu, pokud neexistuje, tak vytvoøí
	TscHTMLLabel *createLabel(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí ukazatel nadanou komponentu
	long GetIdxRow(int X,int Y);//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí øádek
	long GetIdxColum(int X,int Y);//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí sloupec
	bool CheckPTinTable(int X,int Y);//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) zjistí, zda jsou souøadnice ve vnitø tabulky
	TPoint CheckLink(int X,int Y);//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí kladné èíslo sloupce a øádku pokud se na daném místì nachází odkaz, pokud ne, vrácené hodnoty jsou -1 a -1
	bool CheckLink(int X,int Y,unsigned long Col,unsigned long Row);//dle souøadnic ve formuláøi, kde je tabulka zobrazena (napø. dle myšího kurzoru) vrátí zda se na dané buòce a souøadnicích nachází odkaz

	//promìnné
	long Tag;//ID formuláøe, v kterém je tabulka èi tabuky daného formuláøe volány
	long ID;//ID konkrétní tabulky, v jednom formuláøi vhodné unikátní èíslo, mimo formuláøe totoná hodnota nevadí (vyuitelné napø. pokud bude více tabulek, tak se bude vìdìt, v jaké došlo ke kliku)
	//long typeID;//pomocné ID tabulky, napø. pro rozlišení typu tabulky, nemusí bıt vyuito, i v rámci jednoho formuláøe mùe bıt totoné
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
	short Decimal;//implicitní poèet desetinnıch míst u numericeditù
	bool IntegerDecimalNull;//pokud je vıše uvedené Decimal na hodnotu vyšší ne 0, toto nastavuje zda se nuly doplní do poètu decimál i u celıch èísel
	bool MovingTable;//pokud je nastaveno na true, komponenty se zmìní na typ DRAW tj. tak, aby došlo k posunu dané buòky
	bool VisibleComponents;//nastaví componenty na skryté nebo zobrazené

 //protected: - nefugovalo, jak jsme si pøedstavoval
	long Width,Height;//velikost komponenty, jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat
	unsigned long Row,Col;//aktuální øádek a sloupec, jen zobrazovat mimo tøídu, nelze hodnotami nic nastavovat

 private:
	TForm *Form;
	TMyString ms;
	Cmy m;

	void __fastcall getTagOnClick(TObject *Sender);//vrací událost pøi OnClick
	void __fastcall getTagOnEnter(TObject *Sender);//vrací událost pøi OnEnter
	void __fastcall getTagOnChange(TObject *Sender);//vrací událost pøi OnChange
	void __fastcall getTagOnKeyDown(TObject *Sender,WORD &Key, TShiftState Shift);//vrací událost pøi OnKeyDown
	void __fastcall getTagOnKeyPress(TObject *Sender,System::WideChar &Key);//vrací událost pøi OnKeyPress
	void getTextFromComponentToMemoryCell(unsigned long Col,unsigned long Row);//dle zadaného èísla sloupce a èísla øádku vrátí z dané komponenty text do pamìové buòky, slouí napø. pøi události onchange popø. dálších

	void Draw(TCanvas *C);//zajistí vykreslení celé tabulky vèetnì gridu
	void DrawGrid(TCanvas *C);//zajistí vykreslení jen gridu
	void DrawCellBorder(TCanvas *C,unsigned long X,unsigned long Y,TRect R);//zajistí vykreslení orámování jen jedné buòky
	void SetColRow();//nastaví velikost sloupcù a øádkù dle aktuálního nastavení a potøeby
	void SetBorder(TCanvas *C,TBorder *Border);//nastaví grafické pero na poadované parametry
	void SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku dle typu
	void SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku na edit, pomocná metoda vıše uvedené
	void SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku na numericedit, pomocná metoda objednu vıše uvedené
	void SetLabel(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buòku na numericedit, pomocná metoda objednu vıše uvedené
	void rcc(unsigned long cc,unsigned long rc);//pouze obejití lokální promìnné, v c++ je na to nìjaké klíèové slovo, ale nevzpomenu si
	void CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell);//patøiènì prolinkuje orámování, e sousední orámování má ukazatel na totonı objekt, vzor orámvání získá dle refCell
	void CreateLinkBorder2(unsigned long X,unsigned long Y,TCells &refCell);//patøiènì prolinkuje orámování, e sousední orámování má ukazatel na totonı objekt, vzor orámvání získá dle refCell
	void CreateCell(TCells &NewCell);//vytvoøí novou buòku (alokuje ji pamì)
	void DeleteCell(TCells &DelCell);//smae buòku z pamìti
	void DeleteTable();//odstraní pouze tabulku z pamìti
	void DeleteComponents();//odstraní dynamicky vytoøené komponenty, nutno volat pøed Delete() // pozor odstraòovaná komponenta nesmí mít focus (jinak pamìová chyba), focus je potøeba pøi odstraòování komponent odevzdat nìjaké komponentì, která zùstává ve formu
	void DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow);//odstraní dynamicky vytoøené komponenty do poètu sloupcù a øádkù, nutno volat pøed Delete() // pozor odstraòovaná komponenta nesmí mít focus (jinak pamìová chyba), focus je potøeba pøi odstraòování komponent odevzdat nìjaké komponentì, která zùstává ve formu
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
	bool deleteMark;//detekce e dochází k odstraòování mGridu
};
//---------------------------------------------------------------------------
extern TmGrid *mGrid;//ukazatel na celou knihovnu
//---------------------------------------------------------------------------
#endif
