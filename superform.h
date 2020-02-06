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
//---------------------------------------------------------------------------
class TForm_definice_zakazek : public TForm
{
__published:	// IDE-managed Components
	TColorDialog *ColorDialog1;
	TButton *Button_DEL;
	TrHTMLLabel *rHTMLLabel_pocet_prac_hod;
	TrHTMLLabel *rHTMLLabel_pocet_dnu;
	TrHTMLLabel *rHTMLLabel_effektivita;
	TrStringGridEd *rStringGridEd1;
	TrEditNum *rEditNum_pozad_mnozstvi;
	TrEditNum *rEditNum_pocet_prac_hod;
	TrEditNum *rEditNum_effektivita;
	TrEditNum *rEditNum_pocet_dnu;
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
	TrHTMLLabel *rHTMLLabel_pozad_mnozstvi;
	TscGPButton *scGPButton_Ulozit;
	TscGPButton *scGPButton_storno;
	TButton *button_zakazky_temp;
	TscEdit *scEdit_zacatek;
	TButton *zakazky_hlavni_spojak;
	TscGPGlyphButton *scGPGlyphButton_remove;
	TrHTMLLabel *rHTMLLabel_zacatek;
	TrHTMLLabel *rHTMLLabel_eDesigner;
  TscGPImageCollection *scGPImageCollection_layout;
  TButton *Button1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall RzStringGrid1RowMoved(TObject *Sender, int FromIndex, int ToIndex);
	void __fastcall Memo_spojakClick(TObject *Sender);
	void __fastcall RzStringGrid1GetEditMask(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall Button_DELClick(TObject *Sender);
	void __fastcall RzStringGrid1Click(TObject *Sender);
	void __fastcall Button_OKKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rStringGridEd1Click(TObject *Sender);
	void __fastcall rButton1Click(TObject *Sender);
	void __fastcall scGPGlyphButton4Click(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton_UlozitClick(TObject *Sender);
	void __fastcall scGPGlyphButton_add_zakazkaClick(TObject *Sender);
	void __fastcall button_zakazky_tempClick(TObject *Sender);
	void __fastcall zakazky_hlavni_spojakClick(TObject *Sender);
	void __fastcall smaz_tempClick(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall scGPGlyphButton_removeClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall rEditNum_pocet_dnuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_pocet_prac_hodKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_pozad_mnozstviKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rEditNum_effektivitaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scEdit_zacatekKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall rStringGridEd1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);









private:	// User declarations
	void nastav_form();
	void nacti_PP();
	void predvypln_default_zakazku();
	//void predvypln_cestu(); již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
	void nacti_zakazky();
	void uloz_Defaulttemp_zakazku();
	void uloz_Default_cestu();
  void getmGridWidth();


public:		// User declarations
	__fastcall TForm_definice_zakazek(TComponent* Owner);

    	//metody volané z Tmgrid
	void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
  void setButtonColor(int Row);
  void loadHeader(int Row,bool novy=true);
  enum Typ_buttonu {krizek_davky, krizek};

  Typ_buttonu button_type;//zjisteni ktery button ma byt designovan

  void setGlyphButton(double Row, Typ_buttonu typ);
  bool add_zakazka;
	bool zmena_TT;
  bool nacitam_zakazky;
  TColor barva;//barva zakáky
  TColor light_gray; //barva formu

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_definice_zakazek *Form_definice_zakazek;
//---------------------------------------------------------------------------
#endif
