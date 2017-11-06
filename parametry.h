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
	TValueListEditor *ValueListEditor;
	TComboBox *ComboBox_druh_objektu;
	TButton *Button_DEL;
	TEdit *Edit_name;
	TEdit *Edit_shortname;
	TLabel *Label1;
	TLabel *Label2;
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_help;
	TrHTMLLabel *rHTMLLabel_rezim;
	TrHTMLLabel *rHTMLLabel_kapacita;
	TrEditNum *rEditNum_kapacita;
	TscGPButton *scGPButton_OK;
	TscGPButton *scGPButton_storno;
	TEdit *Edit_vzdalenost_voziku;
	TRadioButton *RadioButton_na_sirku;
	TrHTMLLabel *rHTMLLabel_pohon;
	TrHTMLLabel *rHTMLLabel_odchylkaCT;
	TrHTMLLabel *rHTMLLabel_delka_dopravniku;
	TscEdit *scEdit_name;
	TscEdit *scEdit_shortname;
	TrHTMLLabel *rHTMLLabel_name;
	TrHTMLLabel *rHTMLLabel_shortname;
	TrHTMLLabel *rHTMLLabel_cekani;
	TscComboBox *scComboBox_cekani_palec;
	TrEditNum *rEditNum_delka_dopravniku;
	TrEditNum *rEditNum_odchylka;
	TscComboBox *scComboBox_rezim;
	TscComboBox *scComboBox_pohon;
	TscGPGlyphButton *scGPGlyphButton_InfoIcon;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TrHTMLLabel *rHTMLLabel_stopka;
	TscComboBox *scComboBox_stopka;
	TscGPButton *scGPButton_min_sec;
	TrHTMLLabel *rHTMLLabel_CT;
	TrEditNum *rEditNum_CT;
	TrHTMLLabel *rHTMLLabel_RD;
	TrEditNum *rEditNum_RD;
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
	void __fastcall Edit_nameChange(TObject *Sender);
	void __fastcall Edit_vzdalenost_vozikuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scComboBox_rezimChange(TObject *Sender);



private:	// User declarations
	enum Tminsec{MIN=0,SEC};Tminsec minsec;
	enum Tcomponents{POHON,DELKA,CEKANI,ODCHYLKA,KAPACITA,STOPKA,CT,RD};
	enum Tcomponents_state{ENABLED,DISABLED,READONLY,HIDE};

	void set(Tcomponents C,Tcomponents_state S);


	bool navrhar;
	short offset;
	short defaultForm_parametryHeight;


	double get_sum();
	void get_capacity(unsigned int input);
	UnicodeString SG,K,P;
	unsigned short novy_parametr_n;


public:		// User declarations
	__fastcall TForm_parametry(TComponent* Owner);
	void setForm4Rezim(unsigned short rezim);
	void vypis(UnicodeString text);


	void vykresli_vozik(bool na_delku=true);
	short dopravnik_typ;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
#endif
