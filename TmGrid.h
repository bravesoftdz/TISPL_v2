//---------------------------------------------------------------------------
#ifndef TmGridH
#define TmGridH
#include "scGPControls.hpp"//knihovna kvůli buttonumatp.
#include "scGPExtControls.hpp"//knihovna kvůli editbox
#include "scHtmlControls.hpp"//knihovna kvůli scHTMLLabel
#include "scGPImages.hpp"//knihovna kvůli scGPImage
#include "MyString.h"//kvůli parsování
#include "my.h"
//---------------------------------------------------------------------------
class TmGrid
{
 public:
	 enum Ttype{readEDIT,EDIT,NUMERIC,readNUMERIC,BUTTON,glyphBUTTON,COMBO,COMBOEDIT,CHECK,RADIO,DRAW,LABEL,IMAGE};
	 enum Talign{aNO,LEFT,CENTER,RIGHT};
	 enum Tvalign{vNO,TOP,MIDDLE,BOTTOM};

	struct TBorder//datový typ používaný pro orámování buněk
	{
		TColor Color;//barva orámování
		unsigned short Width;//šířka orámování
		TPenStyle Style;//styl čáry
	};

	struct TColumns//datový typ používaný pro sloupce
	{
		unsigned short	Width;//šířka sloupce
		long Left;//umístění sloupce v px, v rámci tabulky
		bool Visible;//zda bude sloupec zobrazen
	};

	struct TRows//datový typ používaný pro řádky
	{
		unsigned short	Height;//výška řádku
		long Top;//umístění řádku v px, v rámci tabulky
		bool Visible;//zda bude řádek zobrazen
	};

	struct TCells//datový typ používaný pro buňky
	{
		Ttype Type;//typ komponenty v buňce dle Ttype
		TBorder *TopBorder;//horní orámování a jeho vlastnosti dle TBorder ukazuje na stejný objekt jako dolní okraj buňky předchozí na ose Y
		TBorder *BottomBorder;//dolní orámování a jeho vlastnosti dle TBorder ukazuje na stejný objekt jako horní okraj buňky následující na ose Y
		TBorder *LeftBorder;//levé orámování a jeho vlastnosti dle TBorder ukazuje na stejný objekt jako pravý okraj buňky předchozí na ose X
		TBorder *RightBorder;//pravé orámování a jeho vlastnosti dle TBorder ukazuje na stejný objekt jako levý okraj buňky následující na ose X
		TBrush *Background;//vlastnosti pozadí buňky
		Talign Align;//horizontální zarovnání textu
		Tvalign Valign;//vertikální zarovnání textu
		short TopMargin;//horní odsazení textu
		short BottomMargin;//dolní odsazení textu
		short LeftMargin;//levé odsazení textu
		short RightMargin;//pravé odsazení textu
		bool MergeState;//pouze indikuje, zda je buňka sloučena, či nikoliv, slouží jako pomůcka při vykreslování orámování sloučených buněk, zatím zůstává nevyužito
		short InputNumbersOnly;//pokud je nastaveno na 0 lze do buňky vepsat cokoliv, 1 lze zapsat pouze reálná čísla, 2 pouze kladná reálná čísla
		bool Highlight;//indikuje zda je buňka zvýrazněna, barva zvýraznění odpovídá globální proměnné TColor clHighlight, výchozí stav zvýraznění je false
		TFont *Font;//vlastnosti fontu v buňce
		TPoint TextPositon;//pozice textu v buňce (levý horní roh buňky je 0,0)
		//PODMÍNĚNÉ FORMÁTOVÁNÍ - Pozor pro případy kdy se používá jiný font, např. v našem případě Roboto Cn na mgridech v náhledu, je nutné vždy nastavovat i název fontu a velikost nejenom pro normální text, ale i pro podmíněné formátování, defualtně je nastaveno vše na naši modrou + velikost 12 pt, kromě záporného čísla, které je ve výchozím stavu nastaveno na červenou
		TBrush *isEmpty;//podmíněné formátování, pozadí buňky, když je prázdná
		TFont *isNegativeNumber;//podmíněné formátování, pokud je zaporné číslo
		TFont *isZero;//podmíněné formátování, pokud se jedná o nulové číslo
		TFont *isLink;//podmíněné formátování v případě výskytu tagu <a> resp </a>
		TFont *isActiveLink;//podmíněné formátování v případě aktivního výskytu tagu <a> resp </a>, přes který se přejelo myší
		//--
		TPoint LinkCoordinateStart;//kvůli uložení citelné oblasti pro link dané buňky
		TPoint LinkCoordinateEnd;//kvůli uložení citelné oblasti pro link dané buňky
		bool ShowHint;//stav zobrazení hintu dané buňky, zda se budu zobrazovat
		UnicodeString Hint;//text hintu buňky
		UnicodeString Text;//samotný text buňky
		int ImageIndex;//číslo obrázku z scGPImageCollection, pokud je použitou načítaní obrázku z něj v případě použití komponenty IMAGE
	};

	struct TNote//datový typ použitý pro poznámku "pod čarou" - pod či okolo tabulky, TmGrid - poznámka "pod čarou" resp. pod tabulkou, přístup mGrid->Note, možno nastavovat hodnotu textu, font textu, ukládá si citelnou oblast, zarovnává na šířku tabulky pokud se text nevejde zalomí na další řádek (dle poslední mezery na řádku), max zobrazí dva řádky, výchozí barva červená a 11pt velikost písma
	{
		TFont *Font;//vlastnosti fontu
		TRect NoteArea;//souřadnice oblasti, kde se poznámka nachází, ukládá citelnou oblast poznámky
		TRect LinkArea;//souřadnice oblasti odkazu v rámci poznámky
		short margin_left,margin_right,margin_bootom,margin_top;
		UnicodeString Text;//samotný text poznámky
	};TNote Note;

	//metody
	TmGrid(TForm *Owner);//konstruktor
	~TmGrid();//destruktor
	void Create();//vytvoří tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//přetížená metoda - vytvoří tabulku s předepsaným počtem sloupců a řádků
	void Delete();//odstraní tabulku, přidružené komponenty a ukazatel na mGrid z paměti
	void MouseMove(int X,int Y);//metoda vhodná na umístění do rodičovského formuláře Form->FormMouseMove,vrací do globálních proměnných index aktuálního sloupce a řádku
	void Show(TCanvas *Canvas=NULL);//zajistí vykreslení celé tabulky
	void Refresh();//zajistí vyvolání překreslení celé tabulky s přeblikem, ale lépe používat přímo ve v daném formuláři FormPaint(this), což zajistí překreslení bez probliku
	void Update();//zajistí vytvoření kompoment včetněj napozicování bez vykreslení
	void SetColumnAutoFit(long ColIdx=-1);//nastaví šířku buněk daného sloupce dle parametru ColIdx, -4 nastavení velikosti sloupce dle ruční nastavení šířky daného sloupce, tj. nedělat autofit, -3 = nepřizpůsobuje se velikost a užije se defaultColWidth,-2 všechny sloupce stejně podle nejširšího textu, -1 přizpůsobuje se každý sloupec individuálně, 0 a více jen konkrétní sloupec uvedený pomoc ColIdx
	int  GetRecommendedColumnWidth(long ColIdx);//vratí doporučenou šířku sloupce dle jeho obsahu
	int  GetRecommendedColumnWidth(AnsiString Text);//vratí doporučenou šířku sloupce dle dodaného textu a aktuálního nastavení canvasu
	void ClearColumn(unsigned long ColIdx);//smaže text v celém sloupec
	void ClearRow(unsigned long RowIdx);//smaže text v celém řádku
	void Clear();//smaže text celé tabulky
	void AddRow(bool copyComponentFromPreviousRow=true,bool invalidate=true);//přidá řádek za poslední řádek, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z předchozího řádku, pokud je invalidate na true, automaticky po přidání překreslí tabulku, někdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint přímo za voláním metody AddRow přimo v užitém formuláři
	void InsertRow(unsigned long Row,bool copyComponentFromPreviousRow=true,bool invalidate=true);//přídá řádek za řádek uvedený dle parametru Row,, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z předchozího řádku, pokud je invalidate na true, automaticky po přidání překreslí tabulku, někdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint přímo za voláním metody InsertRow přimo v užitém formuláři
	void VisibleRow(unsigned long Row,bool visible=true,bool invalidate=true);//skryje či zobrazí daný řádek
	void DeleteRow(unsigned long Row,bool invalidate=true);//smaže celý řádek, pokud je invalidate na true, automaticky po přidání překreslí tabulku, někdy pokud nechci problikávat tabulku lépe nastavit na false a zavolat formpaint přímo za voláním metody InsertRow přimo v užitém formuláři
	void MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//spojí oblast buněk do jedné buňky,text a vlastnosti převezmé od levé horní, začínat zadávat od nejvyšší a nejvíce vlevo
	void SetCells(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,short setText=0,bool copyComponent=false);//nastaví oblast buněk totožnými vlastnostmi dle referenční buňky, text podle posledního parametru buď -1 -smaže, 0 - zanechá původní (implicitně), 1 zkopíruje všude stejný), začínat zadávat od nejvyšší a nejvíce vlevo
	void SetRegion(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//totožně ohraničí danou oblast buněk dle referenční buňky (zohledňuje i rozdíly horní,dolní,levé pravé orámování), začínat zadávat od nejvyšší a nejvíce vlevo
	void CopyCell(TCells &RefCell,TCells &CopyCell,bool copyComponent=false);//zkopíruje obsah, formát a orámování z buňky na buňku (bez ukazatelového propojení)
	void CopyCells2Clipboard(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,UnicodeString Separator="\t");//zkopíruje danou oblast do schránky, buňky oddělí separátorem
	void CopyAreaCell(TCells &RefCell,TCells &CopyCell,bool copyComponent=false);//zkopíruje obsah, formát (bez orámování) z buňky na buňku (bez ukazatelového propojení)
	void CopyBordesCell(TCells &RefCell,TCells &CopyCell);//zkopíruje orámování z buňky na buňku (bez ukazatelového propojení)
	void HighlightTable(TCanvas *Canvas,TColor Color=(TColor)RGB(0,120,215),unsigned short Size=2,unsigned short Offset=0,TPenMode PenMode=pmCopy);//zajistí zvýraznění orámování tabulky
	void HighlightTableOnMouse(int X,int Y,TCanvas *Canvas=NULL);//zajistí zvýraznění orámování tabulky, pokud se do ni vstoupí myší
	void HighlightRow(long Row,TColor Color=clYellow,bool SelFirstRow=false,bool unHighlightPrevRow=true);//zajistí trvalé (jedná se spíše o nastavení) řádků dle čísla řádku Row
	void HighlightRowOnMouse(int X,int Y,TColor Color=clYellow,bool SelFirstRow=false,bool unHighlightPrevRow=true);//zajistí trvalé (jedná se spíše o nastavení) řádků, přes který se přejíždí myší
//	void HighlightCell(unsigned long Col,unsigned long Row,TColor Color=clRed,unsigned short Width=1,bool Refresh=true);//zajistí trvalé (jedná se spíše o nastavení) zvýraznění vnějšího orámování buňky
//	void HighlightEdit(TscGPEdit *Edit,TColor Color=clRed,unsigned short Width=1);//zajistí  trvalé (jedná se spíše o nastavení)  zvýraznění dané komponenty
//	void HighlightEdit(unsigned long Col,unsigned long Row,TColor Color=clRed,unsigned short Width=1);//zajistí trvalé (jedná se spíše o nastavení) zvýraznění dané komponenty
//	void HighlightNumeric(TscGPNumericEdit *Numeric,TColor Color=clRed,unsigned short Width=1);//zajistí zvýraznění trvalé (jedná se spíše o nastavení) dané komponenty
//	void HighlightNumeric(unsigned long Col,unsigned long Row,TColor Color=clRed,unsigned short Width=1);//zajistí trvalé (jedná se spíše o nastavení) zvýraznění dané komponenty
	void unHighlightAll();//odzvýrazni všechna zvýraznění
	void SetVisibleComponents(bool state);//podle stavu state buď zobrazí nebo skryje všechny komponenty
	void SetVisibleComponent(unsigned long Col,unsigned long Row,bool state);//podle stavu state buď zobrazí nebo skryje komponentu neurčitého typu v dané buňce
	void SetEnabledComponents(bool state);//podle stavu state buď povolí či zakáže všechny komponenty
	void SetEnabledComponent(unsigned long Col,unsigned long Row,bool state);//podle stavu state buď povolí či zakáže komponentu neurčitého typu v dané buňce
	TscGPEdit *getEdit(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPNumericEdit *getNumeric(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPButton *getButton(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPGlyphButton *getGlyphButton(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPComboBox *getCombo(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
  TscGPComboEdit *getComboEdit(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPCheckBox *getCheck(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPRadioButton *getRadio(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscHTMLLabel *getLabel(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPImage *getImage(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	void createComponent(Ttype Type, unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vytvořenou komponentu dle Type, pokud existuje, tak se nic neděje
	TscGPEdit *createEdit(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscGPNumericEdit *createNumeric(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscGPButton *createButton(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscGPGlyphButton *createGlyphButton(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscGPComboBox *createCombo(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
  TscGPComboEdit *createComboEdit(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscGPCheckBox *createCheck(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscGPRadioButton *createRadio(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel na danou vytvořenou komponentu, pokud neexistuje, tak vytvoří
	TscHTMLLabel *createLabel(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	TscGPImage *createImage(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí ukazatel nadanou komponentu
	long GetIdxRow(int X,int Y);//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí řádek
	long GetIdxColumn(int X,int Y);//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí sloupec
	bool CheckPTinTable(int X,int Y);//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) zjistí, zda jsou souřadnice ve vnitř tabulky
	TPoint CheckLink(int X,int Y,bool invalidate=false);//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí kladné číslo sloupce a řádku pokud se na daném místě nachází odkaz, pokud ne, vrácené hodnoty jsou -1 a -1
	bool CheckLink(int X,int Y,unsigned long Col,unsigned long Row);//dle souřadnic ve formuláři, kde je tabulka zobrazena (např. dle myšího kurzoru) vrátí zda se na dané buňce a souřadnicích nachází odkaz
	void ShowNote(UnicodeString Text,TColor Color=clRed,short FontSize=0);//zajistí přímé vykreslení poznámky bez refreshe popř. smázání doszením prázdných uvozovek, nově poznámka má také možnost nastavování margin pomocí Note.margin_left,margin_right,margin_bootom,margin_top;
	void Buffer(bool status=false);//zda se bude tabulka ukladat do rastrového bufferu, pro urychlení vykreslování
	bool buffer;//zda se bude tabulka ukladat do rastrového bufferu při Show, resp. Refresh, pro urychlení vykreslování

	//proměnné a ukazatele
	long Tag;//ID formuláře, v kterém je tabulka či tabuky daného formuláře volány
	long ID;//ID konkrétní tabulky, v jednom formuláři vhodné unikátní číslo, mimo formuláře totožná hodnota nevadí (využitelné např. pokud bude více tabulek, tak se bude vědět, v jaké došlo ke kliku)
	//long typeID;//pomocné ID tabulky, např. pro rozlišení typu tabulky, nemusí být využito, i v rámci jednoho formuláře může být totožné
	long Left,Top,preLeft,preTop;//umístění celé komponenty
	unsigned long ColCount,RowCount;//počet řádků a sloupců
	unsigned short DefaultColWidth,DefaultRowHeight;//výchozí výška a šířka řádku
	TBorder Border;//orámování celé tabulky
	bool AntiAliasing_grid;//ano x ne
	bool AntiAliasing_text;//ano x ne
	TCells **Cells;//alokace dvourozmerneho dynamickeho pole buněk
	TColumns *Columns;//alokace jednorozměrneho dynamickeho pole sloupců
	TRows *Rows;//alokace jednorozměrneho dynamickeho pole řádků
	TCells DefaultCell;//deafultní vzorová VIRTUÁLNÍ buňka, podle ní se nastaví všechny při prvním načtení tabulky, pokud není před Show() ještě nastaveno jinak
	TscGPGlyphButton *exBUTTON;//rozšířené tlačítko pro umístění libovolné funkcionality a glyphu, používat pouze na designování
	Talign exBUTTONalign;//pozice rozšířeného tlačítka vůči tabulce
	Tvalign exBUTTONvalign;//pozice rozšířeného tlačítka vůči tabulce
	bool exBUTTONVisible;//stav zobrazení či skrytí exBUTTNU, nepoužívat přímo exBUTTON->Visible, ale toto exBUTTONVisible, exBUTTON používat pouze na designování
	bool exBUTTONLockPosition;//uzamkne pozici exButtonu (použito při updatu)
	short Decimal;//implicitní počet desetinných míst u numericeditů
	bool IntegerDecimalNull;//pokud je výše uvedené Decimal na hodnotu vyšší než 0, toto nastavuje zda se nuly doplní do počtu decimál i u celých čísel
	short VisibleComponents;//nastaví componenty na skryté (0 či false) či zobrazené (1 či true) nebo pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
	TColor clHighlight;//přednastavená barva zvýraznění, slouží i pro nastavení barvy focusu komponent
	int SleepHint;//zpoždění zobrazení Hintu v ms
	bool Highlight;//zda bude orámování tabulky zvýrazněna nebo nikoliv
	bool Redraw;//zda se bude tabulka nanovo překreslovat
	Graphics::TBitmap *Raster;//buffrovací a případně exportní raster
	TscGPImageCollection *scGPImageCollection;//ukazatel na komponentu z které se načítájí obrázky v případě použití komponenty IMAGE

 //protected: - nefugovalo, jak jsme si představoval
	long Width,Height;//velikost komponenty, jen zobrazovat mimo třídu, nelze hodnotami nic nastavovat
	long Row,Col;//aktuální řádek a sloupec, jen zobrazovat mimo třídu, nelze hodnotami nic nastavovat
	int X,Y;//aktuální hodnoty souřadnic myši, jen zobrazovat mimo třídu, nelze hodnotami nic nastavovat

 private:
	TForm *Form;
	TMyString ms;
	Cmy m;

	//TrHTMLLabel *Hint;
	TscHTMLLabel *Hint;//hint draw buněk - zatím dávám do private sekce z důvodu přehlednosti o použití atributu (např. Hint->Visible=true, Hint->ShowHint, Hint->Hint, atp. nic z toho se nesmí užívat)
	TTimer *Timer;

	void __fastcall getTagOnClick(TObject *Sender);//vrací událost při OnClick
	void __fastcall getTagOnEnter(TObject *Sender);//vrací událost při OnEnter
	void __fastcall getTagOnChange(TObject *Sender);//vrací událost při OnChange
	void __fastcall getTagOnKeyDown(TObject *Sender,WORD &Key, TShiftState Shift);//vrací událost při OnKeyDown
	void __fastcall getTagOnKeyPress(TObject *Sender,System::WideChar &Key);//vrací událost při OnKeyPress
	void __fastcall getTagMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);//vrací událost při přejetí myší přes komponentu
	void __fastcall getTagOnMouseEnter(TObject *Sender);//vrací událost při vstupu či přejetí myší přes komponentu
	void __fastcall OnTimer(TObject *Sender);//událost časovače
	void __fastcall OnMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);//metoda voláná z rodičovského formuláře, pozor u více tabulek ve formuláři beru to poslední
	void getTextFromComponentToMemoryCell(unsigned long Col,unsigned long Row);//dle zadaného čísla sloupce a čísla řádku vrátí z dané komponenty text do paměťové buňky, slouží např. při události onchange popř. dálších

	void Draw(TCanvas *C);//zajistí vykreslení celé tabulky včetně gridu a exBUTTONu a poznámky pod čarou
	void DrawGrid(TCanvas *C);//zajistí vykreslení jen gridu
	void DrawNote(TCanvas *C);//zajistí vykreslení poznámky
	TRect DrawTextLink(TCanvas *C,unsigned long left,unsigned long top,AnsiString Text,TFont *FontText,TFont *FontLink,TFont *FontActiveLink);//vykreslí text s odkazem, odkaz aktivní modrou, vrací zpět oblast, kde se nachazí odkaz
	void DrawCellBorder(TCanvas *C,unsigned long X,unsigned long Y,TRect R);//zajistí vykreslení orámování jen jedné buňky
	void SetColRow();//nastaví velikost sloupců a řádků dle aktuálního nastavení a potřeby
	void SetBorder(TCanvas *C,TBorder *Border);//nastaví grafické pero na požadované parametry
	void SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku dle typu
	void SetDraw(TCanvas *Canv,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na draw, pomocná metoda výše uvedené
	void SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na edit, pomocná metoda výše uvedené
	void SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na numericedit, pomocná metoda objednu výše uvedené
	void SetLabel(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na label, pomocná metoda objednu výše uvedené
	void SetButton(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na button, pomocná metoda objednu výše uvedené
	void SetGlyphButton(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na glyphButton, pomocná metoda objednu výše uvedené
	void SetCombo(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na combo, pomocná metoda objednu výše uvedené
  void SetComboEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na combo, pomocná metoda objednu výše uvedené
	void SetCheck(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na check, pomocná metoda objednu výše uvedené
	void SetRadio(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na radio, pomocná metoda objednu výše uvedené
	void SetImage(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastaví danou buňku na Image, pomocná metoda objednu výše uvedené
	void CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell);//patřičně prolinkuje orámování, že sousední orámování má ukazatel na totožný objekt, vzor orámvání získá dle refCell
	void CreateLinkBorder2(unsigned long X,unsigned long Y,TCells &refCell);//patřičně prolinkuje orámování, že sousední orámování má ukazatel na totožný objekt, vzor orámvání získá dle refCell
	void CreateCell(TCells &NewCell);//vytvoří novou buňku (alokuje ji paměť)
	void DeleteCell(TCells &DelCell);//smaže buňku z paměti
	void DeleteTable();//odstraní pouze tabulku z paměti
	void DeleteComponents();//odstraní dynamicky vytořené komponenty, nutno volat před Delete() // pozor odstraňovaná komponenta nesmí mít focus (jinak paměťová chyba), focus je potřeba při odstraňování komponent odevzdat nějaké komponentě, která zůstává ve formu
	void DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow);//odstraní dynamicky vytořené komponenty do počtu sloupců a řádků, nutno volat před Delete() // pozor odstraňovaná komponenta nesmí mít focus (jinak paměťová chyba), focus je potřeba při odstraňování komponent odevzdat nějaké komponentě, která zůstává ve formu
	void MoveComponentUP(unsigned long Col,unsigned long Row);//posunu komponentu na řádku o jeden řádek níže, použává se nově na mazání řádku
	void executeColumnsAutoFit(TCanvas *Canv);//nastaví šířku buněk sloupců dle šířky textu dle zvoleného parametru
	void executeColumnAutoFit(TCanvas *Canv,long ColIdx);//nastaví šířku buněk daného sloupce dle šířky textu v daném sloupci
	void realock();//zajistí realokaci pole Cells dle nové velikosti
	void rotace_textu(long rotace);//zajistí rotaci textu
	void selRow(long Row,TColor Color,bool newSel);//označí řádek, nebo zruší označení řádku dle vstupního parametru
	int getCountNextVisibleRow(int Row);//vráti počet skrytých řádků po aktuálním řádku následujích až po první zobrazený

	unsigned long getWidth();//vrátí celkovou šířku tabulky
	unsigned long getHeight();//vrátí celkovou výšku tabulky
	TPoint getWidthHeightText(TCells &Cell);//vrátí šířku a výšku textu buňky v pixelech
	unsigned long getTag(unsigned long Col,unsigned long Row);//vratí ID tag komponenty,absolutní pořadí v paměti
	unsigned long getColFromTag(unsigned long Tag);//z tagu vratí číslo sloupce
	unsigned long getRowFromTag(unsigned long Tag);//z tagu vratí číslo řádku

	unsigned long bufColCount,bufRowCount;//předchozí počet řádků a sloupců
	short SetColumnAutoFitColIdx;//typ autofit column
	long preRowInd;//předchozí řádek na kterém byla myš
	long preColInd;//předchozí sloupec na kterém byla myš
	bool deleteMark;//detekce že dochází k odstraňování mGridu
	UnicodeString bufText;//ukládá výchozí hodnotu editboxu před psaním, pro případ stisku ESC
};
//---------------------------------------------------------------------------
extern TmGrid *mGrid;//ukazatel na celou knihovnu
//---------------------------------------------------------------------------
#endif
