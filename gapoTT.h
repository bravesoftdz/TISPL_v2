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
#include "scExtControls.hpp"
#include <Vcl.Imaging.pngimage.hpp>
#include "scGPImages.hpp"
//---------------------------------------------------------------------------
class TF_gapoTT : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_OK;
  TrHTMLHint *rHTMLHint_vypis;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TrHTMLLabel *rHTMLLabel_legenda;
	TrHTMLLabel *rHTMLLabel_legenda_titulek;
	TscGPButton *scGPButton_storno;
	TEdit *Edit1;
	TscGPGlyphButton *scGPGlyphButton_copy;
	TscButton *scButton_csv;
	TscScrollBar *scScrollBar_horizont;
	TscScrollBar *scScrollBar_vertical;
  TscGPButton *scGPButton_zamek_locked;
  TscGPImage *scImage_zamky;
  TMemo *Memo1;
  TButton *Button1;
	TscGPCheckBox *scGPCheckBox_prepocitatPT;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scButton_csvClick(TObject *Sender);
	void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
  void __fastcall scScrollBar_horizontChange(TObject *Sender);
  void __fastcall KonecClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
	short Offset;//odsazení tabulky po všech stranách formu
	Cvektory::TObjekt *objekty;//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, pouze duplikát objektù
  unsigned int *indikator_skupin;//dynamické pole, uchovávající indikaci, která oblast dané skupiny byla vybrána
	TColor clOLD,clLOCKED,clUNLOCKED,clBACKGROUND,C1,C2,C3,clHiddenValues,clEMPTYVALUE;
	enum Tinput_state{FREE,LOADING,PROGRAMOVE};//uchovává výbìr input hodnoty
	short CTunit,RDunit,DDunit,Munit,aRDunit,Rzunit,Runit;
	UnicodeString calculate(unsigned long Row,short SaveTo=0);//pro daný øádek dle nastaveného checkboxu, dopoèítá a dosadí nové hodnoty parametrù daného objektu z daného øádku, v pøípadì SaveTo -1, vrátí formou textu, oddìlené støedníky, 0 - nevrací nic, 1 uloží do binárky, 2 do ukazatele na náhled
	bool liche_otoceni_koleckem_mysi;//kvùli špatnì fungující funkci otáèení koleèka myši
  bool nahled;
public:		// User declarations
	__fastcall TF_gapoTT(TComponent* Owner);
	//metody volané z Tmgrid
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	void vypis(UnicodeString text,bool red=true,bool link=false);
	int pocitadlo;
  int slouceny_radek;
	TPoint *pohony_zmena;//dynamické pole evidující zmìny na PL u pohonù
	bool zobrazitFrameForm;
	bool Rx_canEdit;
	Tinput_state input_state;//stav vstupu
  int myModalResult;
  int temp_pocitadlo; // docasne reseni - kvuli posunu gridu
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoTT *F_gapoTT;
//---------------------------------------------------------------------------
#endif
