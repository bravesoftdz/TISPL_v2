//---------------------------------------------------------------------------

#ifndef gapoRH
#define gapoRH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "vektory.h"
#include "rHTMLLabel.hpp"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include "scDrawUtils.hpp"
#include "scGPImages.hpp"
#include <Vcl.Imaging.pngimage.hpp>
#include "PO_math.h"
#include "rHintWindow.hpp"
//---------------------------------------------------------------------------
class TF_gapoR : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_OK;
	TButton *Button1;
	TscButton *scButton_csv;
	TscGPButton *scGPButton_storno;
	TscGPGlyphButton *scGPGlyphButton_copy;
	TEdit *Edit1;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TrHTMLLabel *rHTMLLabel_legenda;
	TrHTMLLabel *rHTMLLabel_legenda_titulek;
	TscScrollBar *scScrollBar_horizont;
	TscScrollBar *scScrollBar_vertical;
  TscGPImage *scGPImage_zamky;
  TMemo *Memo1;
  TMemo *Memo2;
  TrHTMLHint *rHTMLHint_vypis;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
	void __fastcall scButton_csvClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scScrollBar_horizontChange(TObject *Sender);
	void __fastcall scScrollBar_verticalChange(TObject *Sender);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

private:	// User declarations
	//Cvektory v;//instance na t��du vektor� - nepou��v�t, padalo
	short Offset;//odsazen� tabulky po v�ech stran�ch formu
  TPO_math pm;//instance na PO_math, vyu��v� se z ��sti stejn�ch v�po�t�
	Cvektory::TObjekt *objekty;//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, pouze duplik�t objekt�
	UnicodeString calculate(unsigned long Row,short SaveTo=0);//pro dan� ��dek dle nastaven�ho checkboxu, dopo��t� a dosad� nov� hodnoty parametr� dan�ho objektu z dan�ho ��dku, v p��pad� SaveTo -1, vr�t� formou textu, odd�len� st�edn�ky, 0 - nevrac� nic, 1 ulo�� do bin�rky, 2 do ukazatele na n�hled
	TColor clOLD,clLOCKED,clUNLOCKED,clBACKGROUND;
	short CTunit,RDunit,DDunit,Munit,aRDunit,Rzunit,Runit;
	bool liche_otoceni_koleckem_mysi;//kv�li �patn� funguj�c� funkci ot��en� kole�ka my�i

public:		// User declarations
	__fastcall TF_gapoR(TComponent* Owner);
	//metody volan� z Tmgrid
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	TPoint *pohony_zmena;//dynamick� pole eviduj�c� zm�ny na PL u pohon�
	bool zobrazitFrameForm;
  void vypis(UnicodeString text,bool red=true,bool link=false);
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoR *F_gapoR;
//---------------------------------------------------------------------------
#endif
