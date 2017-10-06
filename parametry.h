//---------------------------------------------------------------------------

#ifndef parametryH
#define parametryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "rHTMLLabel.hpp"
#include "rImprovedComps.hpp"
#include "scControls.hpp"
#include "scGPControls.hpp"
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm_parametry : public TForm
{
__published:	// IDE-managed Components
	TButton *Button_OK;
	TValueListEditor *ValueListEditor;
	TLabel *Label_CT;
	TComboBox *ComboBox_druh_objektu;
	TLabel *Label_kapacita;
	TLabel *Label_kapacita_hodnota;
	TLabel *Label_CT_hodnota;
	TComboBox *ComboBox_dopravnik;
	TLabel *Label_vypis;
	TLabel *Label_TT;
	TLabel *Label_TT_hodnota;
	TButton *Button_min_sec;
	TLabel *Label_delka_prepravniku;
	TLabel *Label_delka_prepravniku_hodnota;
	TButton *Button_DEL;
	TEdit *Edit_name;
	TEdit *Edit_shortname;
	TLabel *Label1;
	TLabel *Label2;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPGlyphButton *scGPGlyphButton15;
	TrHTMLLabel *rHTMLLabel5_rezim;
	TrHTMLLabel *rHTMLLabel_kapacita;
	TrEditNum *rEditNum_kapacita;
	TscGPButton *scGPButton1_OK;
	TscGPButton *scGPButton3;
	TRadioButton *RadioButton_na_delku;
	TEdit *Edit_vzdalenost_voziku;
	TRadioButton *RadioButton_na_sirku;
	TImage *Image_vozik;
	TrHTMLLabel *rHTMLLabel_pohon;
	TscCheckBox *scCheckBox_stopky;
	TrHTMLLabel *rHTMLLabel_odchylkaCT;
	TrHTMLLabel *rHTMLLabel_delka_dopravniku;
	TscEdit *scEdit_name;
	TscEdit *scEdit_shortname;
	TrHTMLLabel *rHTMLLabel_name;
	TrHTMLLabel *rHTMLLabel_shortname;
	TrHTMLLabel *rHTMLLabel_cekani;
	TscComboBox *scComboBox1;
	TrHTMLLabel *rHTMLLabel_doporuc_cekani_value;
	TrEditNum *rEditNum_delka_dopravniku;
	TrEditNum *rEditNum_odchylka;
	TscComboBox *scComboBox_rezim;
	TscComboBox *scComboBox_pohon;
	TscGPGlyphButton *scGPGlyphButton1;
	void __fastcall Edit_CTKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ValueListEditorStringsChange(TObject *Sender);
	void __fastcall ComboBox_druh_objektuChange(TObject *Sender);
	void __fastcall ComboBox_dopravnikChange(TObject *Sender);
	void __fastcall RadioButton_na_delkuClick(TObject *Sender);
	void __fastcall RadioButton_na_sirkuClick(TObject *Sender);
	void __fastcall Image_vozikClick(TObject *Sender);
	void __fastcall ValueListEditorClick(TObject *Sender);
	void __fastcall Edit_vzdalenost_vozikuChange(TObject *Sender);
	void __fastcall Button_min_secClick(TObject *Sender);
	void __fastcall Button_DELClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Edit_nameChange(TObject *Sender);
	void __fastcall Edit_vzdalenost_vozikuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scGPButton3Click(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton1_OKClick(TObject *Sender);



private:	// User declarations
	enum Tminsec{MIN=0,SEC};Tminsec minsec;
	double get_sum();

	void get_capacity(unsigned int input);

	UnicodeString SG,K,P;
	unsigned short novy_parametr_n;
	short offset;

public:		// User declarations
	__fastcall TForm_parametry(TComponent* Owner);
	void vykresli_vozik(bool na_delku=true);
	void setForm4Rezim(unsigned short rezim,bool default_text=true);
	short dopravnik_typ;
	void nacist_data();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
#endif
