//---------------------------------------------------------------------------
#ifndef TmGridH
#define TmGridH
//#include <vcl.h>
#include "scGPControls.hpp"//knihovna kv�li buttonum atp.
#include "scGPExtControls.hpp"//knihovna kv�li editbox;m
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
	void Create();//vytvo�� tabulku
	void Create(unsigned long ColCount,unsigned long RowCount);//p�et�en� metoda - vytvo�� tabulku s p�edepsan�m po�tem sloupc� a ��dk�
	void Delete(TForm *Form);//odstran� tabulku, p�idru�en� komponenty a ukazatel na mGrid z pam�ti
	void Show(TForm *Form);//zajist� vykreslen� cel� tabulky
	void SetColumnAutoFit(long ColIdx=-1);//nastav� ���ku bun�k dan�ho sloupce dle parametru ColIdx, -3 = nep�izp�sobuje se velikost a u�ije se defaultColWidth,-2 v�echny sloupce stejn� podle nej�ir��ho textu, -1 p�izp�sobuje se ka�d� sloupec individu�ln�, 0 a v�ce jen konkr�tn� sloupec uveden� pomoc ColIdx
	void ClearColumn(unsigned long ColIdx);//sma�e text v cel�m sloupec
	void ClearRow(unsigned long RowIdx);//sma�e text v cel�m ��dku
	void Clear();//sma�e text cel� tabulky
	void MergeCells(unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//spoj� oblast bun�k do jedn� bu�ky,text a vlastnosti p�evezm� od lev� horn�, za��nat zad�vat od nejvy��� a nejv�ce vlevo
	void SetCells(TCells RefCell,unsigned long ColCell_1,unsigned long RowCell_1,unsigned long ColCell_2,unsigned long RowCell_2);//nastav� oblast bun�k toto�n�mi vlastnostmi dle referen�n� bu�ky (text zanech�), za��nat zad�vat od nejvy��� a nejv�ce vlevo
	void HighlightCell(unsigned long Col,unsigned long Row,unsigned short Width=2,TColor Color=clRed);//zajist� zv�razn�n� dan� bu�ky
	TscGPEdit *getEdit(unsigned long Col,unsigned long Row,TForm *Form);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPButton *getButton(unsigned long Col,unsigned long Row,TForm *Form);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPComboBox *getCombo(unsigned long Col,unsigned long Row,TForm *Form);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPCheckBox *getCheck(unsigned long Col,unsigned long Row,TForm *Form);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPRadioButton *getRadio(unsigned long Col,unsigned long Row,TForm *Form);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu
	TscGPNumericEdit *getNumeric(unsigned long Col,unsigned long Row,TForm *Form);//dle zadan�ho ��sla sloupce a ��sla ��dku vr�t� ukazatel nadanou komponentu

	//prom�nn�
	long Left,Top;//um�st�n� cel� komponenty
	unsigned long ColCount,RowCount;//po�et ��dk� a sloupc�
	unsigned short DefaultColWidth,DefaultRowHeight;//v�choz� v��ka a ���ka ��dku
	TBorder Border;//or�mov�n� cel� tabulky
	bool AntiAliasing;//ano x ne - zat�m nefunguje
	TCells **Cells;//alokace dvourozmerneho dynamickeho pole bun�k
	TColumns *Columns;//alokace jednorozm�rneho dynamickeho pole sloupc�
	TRows *Rows;//alokace jednorozm�rneho dynamickeho pole ��dk�
	TCells DefaultCell;//deafultn� vzorov� bu�ka, podle n� se nastav� v�echny

 //protected: - nefugovalo, jak jsme si p�edstavoval
	long Width,Height;//velikost komponenty, jen zobrazovat mimo t��du, nelze hodnotami nic nastavovat
	unsigned long Row,Col;//aktu�ln� ��dek a sloupec, jen zobrazovat mimo t��du, nelze hodnotami nic nastavovat

 private:
	void __fastcall getTagOnClick(TObject *Sender);
	void __fastcall getTagOnEnter(TObject *Sender);
	void __fastcall getTagOnChange(TObject *Sender);

	void DrawGrid(TForm *Form,double Zoom);//zajist� vykreslen� cel� tabulky
	void SetBorder(TCanvas *C,TBorder *Border);//nastav� grafick� pero na po�adovan� parametry
	void SetComponents(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastav� danou bu�ku dle typu
	void SetEdit(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastav� danou bu�ku na edit, pomocn� metoda v��e uveden�
	void SetNumeric(TForm *Form, TRect R,unsigned long X,unsigned long Y,TCells Cell);//nastav� danou bu�ku na numericedit, pomocn� metoda objednu v��e uveden�
	void rcc(unsigned long cc,unsigned long rc);//pouze obejit� lok�ln� prom�nn�, v c++ je na to n�jak� kl��ov� slovo, ale nevzpomenu si
	void Delete();//odstran� tabulku z pam�ti
	void DeleteComponents(TForm *Form);//odstran� dynamicky vyto�en� komponenty, nutno volat p�ed Delete()
	void executeColumnsAutoFit(TCanvas *Canv);//nastav� ���ku bun�k sloupc� dle ���ky textu dle zvolen�ho parametru
	void executeColumnAutoFit(TCanvas *Canv,long ColIdx);//nastav� ���ku bun�k dan�ho sloupce dle ���ky textu v dan�m sloupci
  void realock(TForm *Form);//zajist� realokaci pole Cells dle nov� velikosti

	unsigned long getWidth();//vr�t� celkovou ���ku tabulky
	unsigned long getHeight();//vr�t� celkovou v��ku tabulky
	unsigned long getTag(unsigned long Col,unsigned long Row);//vrat� ID tag komponenty,absolutn� po�ad� v pam�ti
	unsigned long getColFromTag(unsigned long Tag);//z tagu vrat� ��slo sloupce
	unsigned long getRowFromTag(unsigned long Tag);//z tagu vrat� ��slo ��dku

	unsigned long bufColCount,bufRowCount;//p�edchoz� po�et ��dk� a sloupc�
	short SetColumnAutoFitColIdx;//typ autofit column
};
//---------------------------------------------------------------------------
extern TmGrid *mGrid;//ukazatel na celou knihovnu     PACKAGE
//---------------------------------------------------------------------------
#endif
