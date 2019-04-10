//---------------------------------------------------------------------------
#ifndef TmGridH
#define TmGridH
//#include <vcl.h>
#include "scGPControls.hpp"//knihovna kv�li buttonumatp.
#include "scGPExtControls.hpp"//knihovna kv�li editbox
#include "scHtmlControls.hpp"//knihovna kv�li scHTMLLabel
#include "MyString.h"//kv�li parsov�n�
#include "my.h"
//---------------------------------------------------------------------------
class TmGrid
{
 public:
	enum Ttype{readEDIT,EDIT,NUMERIC,readNUMERIC,BUTTON,glyphBUTTON,COMBO,CHECK,RADIO,/*IMAGE,*/DRAW,LABEL};
	enum Talign{aNO,LEFT,CENTER,RIGHT};
	enum Tvalign{vNO,TOP,MIDDLE,BOTTOM};

	struct TBorder//datov� typ pou��van� pro or�mov�n� bun�k
	{
		TColor Color;//barva or�mov�n�
		unsigned short Width;//���ka or�mov�n�
		TPenStyle Style;//styl ��ry
	};

	struct TColumns//datov� typ pou��van� pro sloupce
	{
	 unsigned short	Width;//���ka sloupce
	 long Left;//um�st�n� sloupce v px, v r�mci tabulky
	 bool Visible;//zda bude sloupec zobrazen
	};

	struct TRows//datov� typ pou��van� pro ��dky
	{
	 unsigned short	Height;//v��ka ��dku
	 long Top;//um�st�n� ��dku v px, v r�mci tabulky
	 //nen� dokon�eno bool Visible;//zda bude sloupec zobrazen
	};

	struct TCells//datov� typ pou��van� pro bu�ky
	{
		Ttype Type;//typ komponenty v bu�ce dle Ttype
		TBorder *TopBorder;//horn� or�mov�n� a jeho vlastnosti dle TBorder ukazuje na stejn� objekt jako doln� okraj bu�ky p�edchoz� na ose Y
		TBorder *BottomBorder;//doln� or�mov�n� a jeho vlastnosti dle TBorder ukazuje na stejn� objekt jako horn� okraj bu�ky n�sleduj�c� na ose Y
		TBorder *LeftBorder;//lev� or�mov�n� a jeho vlastnosti dle TBorder ukazuje na stejn� objekt jako prav� okraj bu�ky p�edchoz� na ose X
		TBorder *RightBorder;//prav� or�mov�n� a jeho vlastnosti dle TBorder ukazuje na stejn� objekt jako lev� okraj bu�ky n�sleduj�c� na ose X
		TBrush *Background;//vlastnosti pozad� bu�ky
		Talign Align;//horizont�ln� zarovn�n� textu
		Tvalign Valign;//vertik�ln� zarovn�n� textu
		short TopMargin;//horn� odsazen� textu
		short BottomMargin;//doln� odsazen� textu
		short LeftMargin;//lev� odsazen� textu
		short RightMargin;//prav� odsazen� textu
		bool MergeState;//pouze indikuje, zda je bu�ka slou�ena, �i nikoliv, slou�� jako pom�cka p�i vykreslov�n� or�mov�n� slou�en�ch bun�k, zat�m z�st�v� nevyu�ito
		bool InputNumbersOnly;//pokud je nastaveno na true, nelze vepsat jinou hodnotu ne� ��selnou (to v�etn� re�ln�ho ��sla)
		TFont *Font;//vlastnosti fontu v bu�ce
		TPoint TextPositon;//pozice textu v bu�ce (lev� horn� roh bu�ky je 0,0)
		TBrush *isEmpty;//podm�n�n� form�tov�n�, pozad� bu�ky, kdy� je pr�zdn�
		TFont *isNegativeNumber;//podm�n�n� form�tov�n�, pokud je zaporn� ��slo
		TFont *isZero;//podm�n�n� form�tov�n�, pokud se jedn� o nulov� ��slo
		TFont *isLink;//podm�n�n� form�tov�n� v p��pad� v�skytu tagu <a> resp </a>
		TPoint LinkCoordinateStart;//kv�li ulo�en� citeln� oblasti pro link dan� bu�ky
		TPoint LinkCoordinateEnd;//kv�li ulo�en� citeln� oblasti pro link dan� bu�ky
		UnicodeString Text;//samotn� text bu�ky
	};

	//metody
	TmGrid(TForm *Owner);//konstruktor
	~TmGrid();//destruktor
	void Create();//vytvo�� tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//p�et�en� metoda - vytvo�� tabulku s p�edepsan�m po�tem sloupc� a ��dk�
	void Delete();//odstran� tabulku, p�idru�en� komponenty a ukazatel na mGrid z pam�ti
	void Show(TCanvas *Canvas=NULL);//zajist� vykreslen� cel� tabulky
	void Refresh();//zajist� vyvol�n� p�ekreslen� cel� tabulky s p�eblikem, ale l�pe pou��vat p��mo ve v dan�m formul��i FormPaint(this), co� zajist� p�ekreslen� bez probliku
	void SetColumnAutoFit(long ColIdx=-1);//nastav� ���ku bun�k dan�ho sloupce dle parametru ColIdx, -4 nastaven� velikosti sloupce dle ru�n� nastaven� ���ky dan�ho sloupce, tj. ned�lat autofit, -3 = nep�izp�sobuje se velikost a u�ije se defaultColWidth,-2 v�echny sloupce stejn� podle nej�ir��ho textu, -1 p�izp�sobuje se ka�d� sloupec individu�ln�, 0 a v�ce jen konkr�tn� sloupec uveden� pomoc ColIdx
	int  GetRecommendedColumnWidth(long ColIdx);//vrat� doporu�enou ���ku sloupce dle jeho obsahu
	int  GetRecommendedColumnWidth(AnsiString Text);//vrat� doporu�enou ���ku sloupce dle dodan�ho textu a aktu�ln�ho nastaven� canvasu
	void ClearColumn(unsigned long ColIdx);//sma�e text v cel�m sloupec
	void ClearRow(unsigned long RowIdx);//sma�e text v cel�m ��dku
	void Clear();//sma�e text cel� tabulky
	void AddRow(bool copyComponentFromPreviousRow=true,bool invalidate=true);//p�id� ��dek za posledn� ��dek, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z p�edchoz�ho ��dku, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody AddRow p�imo v u�it�m formul��i
	void InsertRow(unsigned long Row,bool copyComponentFromPreviousRow=true,bool invalidate=true);//p��d� ��dek za ��dek uveden� dle parametru Row,, pokud copyComponentFromPreviousRow je na true, zkopiruje kompomenty z p�edchoz�ho ��dku, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody InsertRow p�imo v u�it�m formul��i
	void DeleteRow(unsigned long Row,bool invalidate=true);//sma�e cel� ��dek, pokud je invalidate na true, automaticky po p�id�n� p�ekresl� tabulku, n�kdy pokud nechci problik�vat tabulku l�pe nastavit na false a zavolat formpaint p��mo za vol�n�m metody InsertRow p�imo v u�it�m formul��i
	void MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//spoj� oblast bun�k do jedn� bu�ky,text a vlastnosti p�evezm� od lev� horn�, za��nat zad�vat od nejvy��� a nejv�ce vlevo
	void SetCells(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,short setText=0,bool copyComponent=false);//nastav� oblast bun�k toto�n�mi vlastnostmi dle referen�n� bu�ky, text podle posledn�ho parametru bu� -1 -sma�e, 0 - zanech� p�vodn� (implicitn�), 1 zkop�ruje v�ude stejn�), za��nat zad�vat od nejvy��� a nejv�ce vlevo
	void SetRegion(TCells &RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//toto�n� ohrani�� danou oblast bun�k dle referen�n� bu�ky (zohled�uje i rozd�ly horn�,doln�,lev� prav� or�mov�n�), za��nat zad�vat od nejvy��� a nejv�ce vlevo
	void CopyCell(TCells &RefCell,TCells &CopyCell,bool copyComponent=false);//zkop�ruje obsah, form�t a or�mov�n� z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
	void CopyCells2Clipboard(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2,UnicodeString Separator="\t");//zkop�ruje danou oblast do schr�nky, bu�ky odd�l� separ�torem
	void CopyAreaCell(TCells &RefCell,TCells &CopyCell,bool copyComponent=false);//zkop�ruje obsah, form�t (bez or�mov�n�) z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
	void CopyBordesCell(TCells &RefCell,TCells &CopyCell);//zkop�ruje or�mov�n� z bu�ky na bu�ku (bez ukazatelov�ho propojen�)
	void HighlightCell(unsigned long Col,unsigned long Row,TColor Color=clRed,unsigned short Width=2);//zajist� zv�razn�n� dan� bu�ky
	void HighlightRow(long Row,TColor Color=clYellow,bool SelFirstRow=false,bool unHighlightPrevRow=true);//zajist� zv�razn�n� ��dk� dle ��sla ��dku Row
	void HighlightRowOnMouse(int X,int Y,TColor Color=clYellow,bool SelFirstRow=false,bool unHighlightPrevRow=true);//zajist� zv�razn�n� ��dk�, p�es kter� se p�ej�d� my��
	void HighlightTable(TColor Color=(TColor)RGB(43,87,154),unsigned short Size=2,unsigned short Offset=0,TPenMode PenMode=pmCopy);//zajist� zv�razn�n� or�mov�n� tabulky
	void HighlightTableOnMouse(int X,int Y);//zajist� zv�razn�n� or�mov�n� tabulky, pokud se do ni vstoup� my��
	void HighlightLink(unsigned long Col,unsigned long Row,short Intensive=-50);//zajist� p�ebarven� odkazu v bu�ce odst�nem barvy odkazu
	void SetVisibleComponents(bool state);//podle stavu state bu� zobraz� nebo skryje v�echny komponenty
	TscGPEdit *getEdit(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPNumericEdit *getNumeric(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPButton *getButton(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPGlyphButton *getGlyphButton(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPComboBox *getCombo(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPCheckBox *getCheck(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPRadioButton *getRadio(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscHTMLLabel *getLabel(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	void createComponent(Ttype Type, unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vytvo�enou komponentu dle Type, pokud existuje, tak se nic ned�je
	TscGPEdit *createEdit(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscGPNumericEdit *createNumeric(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscGPButton *createButton(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscGPGlyphButton *createGlyphButton(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscGPComboBox *createCombo(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscGPCheckBox *createCheck(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscGPRadioButton *createRadio(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel na danou vytvo�enou komponentu, pokud neexistuje, tak vytvo��
	TscHTMLLabel *createLabel(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	long GetIdxRow(int X,int Y);//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� ��dek
	long GetIdxColum(int X,int Y);//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� sloupec
	bool CheckPTinTable(int X,int Y);//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) zjist�, zda jsou sou�adnice ve vnit� tabulky
	TPoint CheckLink(int X,int Y);//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� kladn� ��slo sloupce a ��dku pokud se na dan�m m�st� nach�z� odkaz, pokud ne, vr�cen� hodnoty jsou -1 a -1
	bool CheckLink(int X,int Y,unsigned long Col,unsigned long Row);//dle sou�adnic ve formul��i, kde je tabulka zobrazena (nap�. dle my��ho kurzoru) vr�t� zda se na dan� bu�ce a sou�adnic�ch nach�z� odkaz

	//prom�nn�
	long Tag;//ID formul��e, v kter�m je tabulka �i tabuky dan�ho formul��e vol�ny
	long ID;//ID konkr�tn� tabulky, v jednom formul��i vhodn� unik�tn� ��slo, mimo formul��e toto�n� hodnota nevad� (vyu�iteln� nap�. pokud bude v�ce tabulek, tak se bude v�d�t, v jak� do�lo ke kliku)
	//long typeID;//pomocn� ID tabulky, nap�. pro rozli�en� typu tabulky, nemus� b�t vyu�ito, i v r�mci jednoho formul��e m��e b�t toto�n�
	long Left,Top,preLeft,preTop;//um�st�n� cel� komponenty
	unsigned long ColCount,RowCount;//po�et ��dk� a sloupc�
	unsigned short DefaultColWidth,DefaultRowHeight;//v�choz� v��ka a ���ka ��dku
	TBorder Border;//or�mov�n� cel� tabulky
	bool AntiAliasing_grid;//ano x ne
	bool AntiAliasing_text;//ano x ne
	TCells **Cells;//alokace dvourozmerneho dynamickeho pole bun�k
	TColumns *Columns;//alokace jednorozm�rneho dynamickeho pole sloupc�
	TRows *Rows;//alokace jednorozm�rneho dynamickeho pole ��dk�
	TCells DefaultCell;//deafultn� vzorov� VIRTU�LN� bu�ka, podle n� se nastav� v�echny p�i prvn�m na�ten� tabulky, pokud nen� p�ed Show() je�t� nastaveno jinak
	short Decimal;//implicitn� po�et desetinn�ch m�st u numericedit�
	bool IntegerDecimalNull;//pokud je v��e uveden� Decimal na hodnotu vy��� ne� 0, toto nastavuje zda se nuly dopln� do po�tu decim�l i u cel�ch ��sel
	bool MovingTable;//pokud je nastaveno na true, komponenty se zm�n� na typ DRAW tj. tak, aby do�lo k posunu dan� bu�ky
	bool VisibleComponents;//nastav� componenty na skryt� nebo zobrazen�

 //protected: - nefugovalo, jak jsme si p�edstavoval
	long Width,Height;//velikost komponenty, jen zobrazovat mimo t��du, nelze hodnotami nic nastavovat
	unsigned long Row,Col;//aktu�ln� ��dek a sloupec, jen zobrazovat mimo t��du, nelze hodnotami nic nastavovat

 private:
	TForm *Form;
	TMyString ms;
	Cmy m;

	void __fastcall getTagOnClick(TObject *Sender);//vrac� ud�lost p�i OnClick
	void __fastcall getTagOnEnter(TObject *Sender);//vrac� ud�lost p�i OnEnter
	void __fastcall getTagOnChange(TObject *Sender);//vrac� ud�lost p�i OnChange
	void __fastcall getTagOnKeyDown(TObject *Sender,WORD &Key, TShiftState Shift);//vrac� ud�lost p�i OnKeyDown
	void __fastcall getTagOnKeyPress(TObject *Sender,System::WideChar &Key);//vrac� ud�lost p�i OnKeyPress
	void getTextFromComponentToMemoryCell(unsigned long Col,unsigned long Row);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� z dan� komponenty text do pam�ov� bu�ky, slou�� nap�. p�i ud�losti onchange pop�. d�l��ch

	void Draw(TCanvas *C);//zajist� vykreslen� cel� tabulky v�etn� gridu
	void DrawGrid(TCanvas *C);//zajist� vykreslen� jen gridu
	void DrawCellBorder(TCanvas *C,unsigned long X,unsigned long Y,TRect R);//zajist� vykreslen� or�mov�n� jen jedn� bu�ky
	void SetColRow();//nastav� velikost sloupc� a ��dk� dle aktu�ln�ho nastaven� a pot�eby
	void SetBorder(TCanvas *C,TBorder *Border);//nastav� grafick� pero na po�adovan� parametry
	void SetComponents(TCanvas *Canv,TRect R,TRect Rt,unsigned long X,unsigned long Y,TCells &Cell);//nastav� danou bu�ku dle typu
	void SetEdit(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastav� danou bu�ku na edit, pomocn� metoda v��e uveden�
	void SetNumeric(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
	void SetLabel(TRect R,unsigned long X,unsigned long Y,TCells &Cell);//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
	void rcc(unsigned long cc,unsigned long rc);//pouze obejit� lok�ln� prom�nn�, v c++ je na to n�jak� kl��ov� slovo, ale nevzpomenu si
	void CreateLinkBorder(unsigned long X,unsigned long Y,TCells &refCell);//pat�i�n� prolinkuje or�mov�n�, �e sousedn� or�mov�n� m� ukazatel na toto�n� objekt, vzor or�mv�n� z�sk� dle refCell
	void CreateLinkBorder2(unsigned long X,unsigned long Y,TCells &refCell);//pat�i�n� prolinkuje or�mov�n�, �e sousedn� or�mov�n� m� ukazatel na toto�n� objekt, vzor or�mv�n� z�sk� dle refCell
	void CreateCell(TCells &NewCell);//vytvo�� novou bu�ku (alokuje ji pam�)
	void DeleteCell(TCells &DelCell);//sma�e bu�ku z pam�ti
	void DeleteTable();//odstran� pouze tabulku z pam�ti
	void DeleteComponents();//odstran� dynamicky vyto�en� komponenty, nutno volat p�ed Delete() // pozor odstra�ovan� komponenta nesm� m�t focus (jinak pam�ov� chyba), focus je pot�eba p�i odstra�ov�n� komponent odevzdat n�jak� komponent�, kter� z�st�v� ve formu
	void DeleteComponents(unsigned long sCol,unsigned long sRow,unsigned long fCol,unsigned long fRow);//odstran� dynamicky vyto�en� komponenty do po�tu sloupc� a ��dk�, nutno volat p�ed Delete() // pozor odstra�ovan� komponenta nesm� m�t focus (jinak pam�ov� chyba), focus je pot�eba p�i odstra�ov�n� komponent odevzdat n�jak� komponent�, kter� z�st�v� ve formu
	void executeColumnsAutoFit(TCanvas *Canv);//nastav� ���ku bun�k sloupc� dle ���ky textu dle zvolen�ho parametru
	void executeColumnAutoFit(TCanvas *Canv,long ColIdx);//nastav� ���ku bun�k dan�ho sloupce dle ���ky textu v dan�m sloupci
	void realock();//zajist� realokaci pole Cells dle nov� velikosti
	void rotace_textu(long rotace);//zajist� rotaci textu
	void selRow(long Row,TColor Color,bool newSel);//ozna�� ��dek, nebo zru�� ozna�en� ��dku dle vstupn�ho parametru

	unsigned long getWidth();//vr�t� celkovou ���ku tabulky
	unsigned long getHeight();//vr�t� celkovou v��ku tabulky
	TPoint getWidthHeightText(TCells &Cell);//vr�t� ���ku a v��ku textu bu�ky v pixelech
	unsigned long getTag(unsigned long Col,unsigned long Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
	unsigned long getColFromTag(unsigned long Tag);//z tagu vrat� ��slo sloupce
	unsigned long getRowFromTag(unsigned long Tag);//z tagu vrat� ��slo ��dku

	unsigned long bufColCount,bufRowCount;//p�edchoz� po�et ��dk� a sloupc�
	short SetColumnAutoFitColIdx;//typ autofit column
	long preRowInd;//p�edchoz� ��dek na kter�m byla my�
	bool deleteMark;//detekce �e doch�z� k odstra�ov�n� mGridu
};
//---------------------------------------------------------------------------
extern TmGrid *mGrid;//ukazatel na celou knihovnu
//---------------------------------------------------------------------------
#endif
