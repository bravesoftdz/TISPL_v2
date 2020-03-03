//---------------------------------------------------------------------------

#ifndef superformH
#define superformH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "RzGrids.hpp"
#include <Vcl.ColorGrd.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ValEdit.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
#include "rStringGridEd.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
#include "scGPImages.hpp"
#include "scImageCollection.hpp"
#include "scColorControls.hpp"
#include "scExtControls.hpp"
#include "Unit1.h"
//---------------------------------------------------------------------------
class TForm_definice_zakazek : public TForm
{
__published:	// IDE-managed Components
	TColorDialog *ColorDialog1;
	TrEditNum *rEditNum_effektivita;
	TscGPButton *scGPButton2;
	TButton *Button5;
  TscGPButton *scGPButton_plan_vyroby;
	TscGPGlyphButton *scGPGlyphButton_add_zakazka;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
  TscLabel *scLabel_header;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *scGPGlyphButton15;
	TImageList *ImageList1;
	TscGPButton *scGPButton_Ulozit;
	TscGPButton *scGPButton_storno;
	TscEdit *scEdit_zacatek;
	TscGPGlyphButton *scGPGlyphButton_remove;
	TrHTMLLabel *rHTMLLabel_zacatek;
  TscGPImageCollection *scGPImageCollection_layout;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scGPGlyphButton4Click(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton_UlozitClick(TObject *Sender);
	void __fastcall scGPGlyphButton_add_zakazkaClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall rEditNum_pocet_dnuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);









private:	// User declarations
	void nastav_form();
	void nastav_jazyk();
	void nacti_PP();
	void predvypln_default_zakazku();
	//void predvypln_cestu(); již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
	void nacti_zakazky();
	void uloz_Defaulttemp_zakazku();
	void uloz_Default_cestu();
	void getmGridWidth();
	void vloz_davku(Cvektory::TZakazka *Z,Cvektory::TDavka *davka=NULL);

public:		// User declarations
	__fastcall TForm_definice_zakazek(TComponent* Owner);

    	//metody volané z Tmgrid
	void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
  void setButtonColor(long ID);
  void loadHeader(unsigned long zakazka_n=0,bool novy=true);
  enum Typ_buttonu {krizek_davky, krizek,color};

  Typ_buttonu button_type;//zjisteni ktery button ma byt designovan

	void setGlyphButtonDefault(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z);
  void setGlyphButtonDavka_Add(unsigned long ID,unsigned long Col);
	void setGlyphButtonDavka_Remove(unsigned long Col,Cvektory::TZakazka *Z,Cvektory::TDavka *davka);
  void setGlyphButtonColor(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z);
  bool add_zakazka;
  bool add_davka;
	bool zmena_TT;
  bool nacitam_zakazky;
  bool volno;
  bool novy;
  int pocet_davek;
  TColor barva;//barva zakáky
  TColor light_gray; //barva formu
  TColor def_gray; //vychozí barva oddìlující bunky

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_definice_zakazek *Form_definice_zakazek;
//---------------------------------------------------------------------------
#endif
