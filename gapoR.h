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
private:	// User declarations
	//Cvektory v;//instance na tøídu vektorù - nepoužívát, padalo
	short Offset;//odsazení tabulky po všech stranách formu
	Cvektory::TObjekt *objekty;//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, pouze duplikát objektù
	UnicodeString calculate(unsigned long Row,short SaveTo=0);//pro daný øádek dle nastaveného checkboxu, dopoèítá a dosadí nové hodnoty parametrù daného objektu z daného øádku, v pøípadì SaveTo -1, vrátí formou textu, oddìlené støedníky, 0 - nevrací nic, 1 uloží do binárky, 2 do ukazatele na náhled
	TColor clOLD,clLOCKED,clUNLOCKED,clBACKGROUND;
	short CTunit,RDunit,DDunit,Munit;
public:		// User declarations
	__fastcall TF_gapoR(TComponent* Owner);
	//metody volané z Tmgrid
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	TPoint *pohony_zmena;//dynamické pole evidující zmìny na PL u pohonù
	bool zobrazitFrameForm;
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoR *F_gapoR;
//---------------------------------------------------------------------------
#endif
