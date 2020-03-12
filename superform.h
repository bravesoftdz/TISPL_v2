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
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_definice_zakazek : public TForm
{
__published:	// IDE-managed Components
	TColorDialog *ColorDialog1;
	TrEditNum *rEditNum_effektivita;
	TscGPButton *scGPButton2;
	TButton *Button5;
	TscGPGlyphButton *scGPGlyphButton_add_zakazka;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
  TscLabel *scLabel_header;
	TImageList *ImageList1;
	TscGPButton *scGPButton_Ulozit;
	TscGPButton *scGPButton_storno;
	TscEdit *scEdit_zacatek;
	TrHTMLLabel *rHTMLLabel_zacatek;
  TscGPImageCollection *scGPImageCollection_layout;
	TTimer *TimerMouseWheel;
	TEdit *Edit_for_Focus;
  TscGPGlyphButton *scGPGlyphButton_small;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scGPGlyphButton4Click(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton_UlozitClick(TObject *Sender);
	void __fastcall scGPGlyphButton_add_zakazkaClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall rEditNum_pocet_dnuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall TimerMouseWheelTimer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scLabel_headerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall scLabel_headerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);



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
	void pan_create();
	void pan_map(TCanvas * canv, int X, int Y);
	void pan_move_map();
	void mGrid_mimo_obraz(Cvektory::TZakazka *Z);
	void vazat_posun();//upravý globální promìnnou posun, tak aby byl posun vázaný, tz. nelze posunout pravý konec tabulky dál než 5px od okraje formu

	enum Takce{NIC=0,PAN,PAN_MOVE,BLOK};Takce Akce;
	bool pan_non_locked;
	TPoint akt_souradnice_kurzoru_PX;//uchová aktuální pozici kurzoru
	TPoint vychozi_souradnice_kurzoru;//uchová výchozí pozici kurzoru
	TPoint Posun;//promìnné uchovávajicí velikost posunu obrazu (pro scrollování atp.), je to ve fyzických souøadnicích zaøízení
	TPoint Posun_predchozi;
	TRect max_oblast_mGridu;//uchovává si oblast leveho horního rohu prvního mGridu po poslední a nejšírší mGrid
	short jedno_ze_tri_otoceni_koleckem_mysi;
	short doba_neotaceni_mysi;
	unsigned short int funkcni_klavesa;//uchovává stav poslední stisknuté funkèní klávesy


public:		// User declarations
	__fastcall TForm_definice_zakazek(TComponent* Owner);

    	//metody volané z Tmgrid
	void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,long ID,unsigned long Col,unsigned long Row);
  void setButtonColor(long ID);
  void loadHeader(unsigned long zakazka_n=0,bool novy=true);
  enum Typ_buttonu {krizek_davky, krizek,color};
  enum TAkce_obrazku {load,add,remove};

	Typ_buttonu button_type;//zjisteni ktery button ma byt designovan

	void setGlyphButtonDefault(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z);
  void setGlyphButtonDavka_Add(unsigned long ID,unsigned long Col);
	void setGlyphButtonDavka_Remove(unsigned long Col,Cvektory::TZakazka *Z,Cvektory::TDavka *davka);
  void setGlyphButtonColor(unsigned long Row,unsigned long Col, Typ_buttonu typ, Cvektory::TZakazka *Z);
  void set_formHW_button_positions();
	void GetImages(Cvektory::TZakazka *Z);
  bool add_zakazka;
  bool add_davka;
	bool zmena_TT;
  bool nacitam_zakazky;
  bool volno;
  bool novy;
  int pocet_davek;
  int pocet_zakazek;
  TColor barva;//barva zakáky
  TColor light_gray; //barva formu
	TColor def_gray; //vychozí barva oddìlující bunky
	Graphics::TBitmap *Pan_bmp;//kvùli mGridu jinak staèí private

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_definice_zakazek *Form_definice_zakazek;
//---------------------------------------------------------------------------
#endif
