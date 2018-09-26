//---------------------------------------------------------------------------

#ifndef gapoVH
#define gapoVH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "vektory.h"
#include "rHTMLLabel.hpp"
#include "rHintWindow.hpp"
#include "scGPImages.hpp"
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
class TF_gapoV : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_OK;
	TscGPButton *scGPButton_storno;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TrHTMLHint *rHTMLHint1;
	TMemo *Memo1;
	TrHTMLLabel *rHTMLLabel_legenda;
	TscScrollBar *scScrollBar_horizont;
	TscScrollBar *scScrollBar_vertical;
	TrHTMLLabel *rHTMLLabel_legenda_titulek;
	TEdit *Edit1;
  TscGPGlyphButton *scGPGlyphButton_copy;
  TscButton *scButton_csv;
  TscGPImage *scGPImage_zamky;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
  void __fastcall scButton_csvClick(TObject *Sender);
  void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
  void __fastcall KonecClick(TObject *Sender);

private:	// User declarations
	short Offset;//odsazen� tabulky po v�ech stran�ch formu
	Cvektory::TObjekt *objekty;//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, pouze duplik�t objekt�
  short *tempCHECK;//dynamick� pole uchov�vaj�c� p�edchoz� volby checkbox�
	UnicodeString calculate(unsigned long Row,short SaveTo=0);//pro dan� ��dek dle nastaven�ho checkboxu, dopo��t� a dosad� nov� hodnoty parametr� dan�ho objektu z dan�ho ��dku, v p��pad� SaveTo -1, vr�t� formou textu, odd�len� st�edn�ky, 0 - nevrac� nic, 1 ulo�� do bin�rky, 2 do ukazatele na n�hled
	TColor clOLD,clLOCKED,clUNLOCKED,clBACKGROUND,C1,C2,C3,clHiddenValues;
	enum Tinput_state{FREE,LOADING,PROGRAMOVE};//uchov�v� v�b�r input hodnoty
 	short CTunit,RDunit,DDunit,Munit,aRDunit,Rzunit,Runit;
	bool liche_otoceni_koleckem_mysi;//kv�li �patn� funguj�c� funkci ot��en� kole�ka my�i
public:		// User declarations
	__fastcall TF_gapoV(TComponent* Owner);
	//metody volan� z Tmgrid
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	void vypis(UnicodeString text,bool red=true,bool link=false);
	bool zobrazitFrameForm;
	int pruchod;
	int calc_pruchod;
	int pocitadlo_validace;
	bool leva_oblast;
	Tinput_state input_state;//stav vstupu
  int myModalResult;
  int temp_pocitadlo; // docasne reseni - kvuli posunu gridu
  int slouceny_radek;
  bool Rx_canEdit;
 // enum Tm_mm{M=0,MM};Tm_mm Delkaunit;Tm_mm Sirkaunit;Tm_mm Runit;Tm_mm Rzunit;Tm_mm Dmunit;//p�ep�na� jednotek vzd�lenost
	//enum Tminsec{S=0,MIN};Tminsec Taktunit;Tminsec RDunit;Tminsec minsec;Tminsec aRDunit;//p�ep�na� jednotek �asu
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoV *F_gapoV;
//---------------------------------------------------------------------------
#endif
