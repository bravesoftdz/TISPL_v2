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
//---------------------------------------------------------------------------
class TForm_definice_zakazek : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_OK;
	TRzStringGrid *RzStringGrid1;
	TColorDialog *ColorDialog1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TMemo *Memo1;
	TButton *Memo_spojak;
	TMemo *Memo2;
	TMemo *Memo3;
	TButton *Button_DEL;
	TrHTMLLabel *rHTMLLabel_pocet_prac_hod;
	TrHTMLLabel *rHTMLLabel_pocet_dnu;
	TrHTMLLabel *rHTMLLabel_effektivita;
	TrStringGridEd *rStringGridEd1;
	TrEditNum *rEditNum_pozad_mnozstvi;
	TrEditNum *rEditNum_pocet_prac_hod;
	TrEditNum *rEditNum_effektivita;
	TrEditNum *rEditNum_pocet_dnu;
	TrButton *rButton1;
	TscGPButton *scGPButton2;
	TButton *Button5;
	TscGPButton *scGPButton4;
	TscGPGlyphButton *scGPGlyphButton_add_zakazka;
	TscGPGlyphButton *scGPGlyphButton2;
	TscGPGlyphButton *scGPGlyphButton4;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscLabel *scLabel1;
	TscGPGlyphButton *MaxButton;
	TscGPGlyphButton *scGPGlyphButton15;
	TImageList *ImageList1;
	TrHTMLLabel *rHTMLLabel_pozad_mnozstvi;
	TscGPButton *scGPButton_Ulozit;
	TscGPButton *scGPButton3;
	TButton *button_zakazky_temp;
	TMemo *Memo4;
	TrHTMLLabel *rHTMLLabel_zacatek;
	TscEdit *scEdit_zacatek;
	TButton *zakazky_hlavni_spojak;
	TButton *smaz_temp;
	TEdit *Edit_smaz_temp;
	void __fastcall Button_OKClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall RzStringGrid1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall RzStringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
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







	//void __fastcall DrawGrid1Click(TObject *Sender);
private:	// User declarations
	bool OK_status;
	bool Color_status;
	bool RowMoved;
public:		// User declarations
	__fastcall TForm_definice_zakazek(TComponent* Owner);

		short offset;
		short Pocet_radku;
		bool data_nalezena;  //jsou-li data ve spojaku, nastavi se na true



		struct TBarva
		{
			long	id_radek;
			TColor barva_voziku;
			struct TBarva *predchozi;
			struct TBarva *dalsi;
		};
		TBarva *BARVY;

	//	TForm_vozik_nastaveni();
		void hlavicka_barva();
		void uloz_barvu();
		void uloz_barvu(TColor barva,int i);
		void aktualizuj_barvu(TColor barva,int row);
		void vymaz_barvu();
		void smaz_barvu_radku(TBarva *Barva);
		void sniz_indexy(TBarva *Barva);
		void nacti_voziky();
		void uloz_voziky_a_nastav_zakazky();
		void nacti_defaulni_PP();
		void nacti_PP();
		void nacti_nastaveni_formu();
		void nacti_zakazky();
		void nacti_default_zakazku();
		void uloz_Defaulttemp_zakazku();

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_definice_zakazek *Form_definice_zakazek;
//---------------------------------------------------------------------------
#endif
