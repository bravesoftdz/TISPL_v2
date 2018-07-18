//---------------------------------------------------------------------------

#ifndef gapoTTH
#define gapoTTH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "vektory.h"
#include "rHintWindow.hpp"
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TF_gapoTT : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_OK;
	TrHTMLHint *rHTMLHint1;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TrHTMLLabel *rHTMLLabel_legenda;
	TrHTMLLabel *rHTMLLabel_legenda_titulek;
	TscGPButton *scGPButton_storno;
	TEdit *Edit1;
	TscGPGlyphButton *scGPGlyphButton_copy;
	TscButton *scButton_csv;
	TscScrollBar *scScrollBar_horizont;
	TscScrollBar *scScrollBar_vertical;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scButton_csvClick(TObject *Sender);
	void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
private:	// User declarations
	short Offset;//odsazen� tabulky po v�ech stran�ch formu
	Cvektory::TObjekt *objekty;//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, pouze duplik�t objekt�
	TColor clOLD,clLOCKED,clUNLOCKED,clBACKGROUND,C1,C2,C3;
	enum Tinput_state{FREE,LOADING};//uchov�v� v�b�r input hodnoty
	short CTunit,RDunit,DDunit,Munit;
	UnicodeString calculate(unsigned long Row,short SaveTo=0);//pro dan� ��dek dle nastaven�ho checkboxu, dopo��t� a dosad� nov� hodnoty parametr� dan�ho objektu z dan�ho ��dku, v p��pad� SaveTo -1, vr�t� formou textu, odd�len� st�edn�ky, 0 - nevrac� nic, 1 ulo�� do bin�rky, 2 do ukazatele na n�hled
	bool liche_otoceni_koleckem_mysi;//kv�li �patn� funguj�c� funkci ot��en� kole�ka my�i
public:		// User declarations
	__fastcall TF_gapoTT(TComponent* Owner);
	//metody volan� z Tmgrid
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	void vypis(UnicodeString text,bool red=true,bool link=false);
	int pocitadlo;
	TPoint *pohony_zmena;//dynamick� pole eviduj�c� zm�ny na PL u pohon�
	bool zobrazitFrameForm;
	bool Rx_canEdit;
	Tinput_state input_state;//stav vstupu
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoTT *F_gapoTT;
//---------------------------------------------------------------------------
#endif
