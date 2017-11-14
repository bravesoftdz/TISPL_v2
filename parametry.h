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
#include "scGPExtControls.hpp"
//---------------------------------------------------------------------------
class TForm_parametry : public TForm
{
__published:	// IDE-managed Components
	TComboBox *ComboBox_druh_objektu;
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
	TscGPButton *scGPButton_OK;
	TscGPButton *scGPButton_storno;
	TEdit *Edit_vzdalenost_voziku;
	TRadioButton *RadioButton_na_sirku;
	TrHTMLLabel *rHTMLLabel_pohon;
	TrHTMLLabel *rHTMLLabel_odchylka;
	TrHTMLLabel *rHTMLLabel_delka_dopravniku;
	TrHTMLLabel *rHTMLLabel_name;
	TrHTMLLabel *rHTMLLabel_shortname;
	TrHTMLLabel *rHTMLLabel_cekani;
	TscGPGlyphButton *scGPGlyphButton_InfoIcon;
	TrHTMLLabel *rHTMLLabel_stopka;
	TscGPButton *scGPButton_min_sec;
	TrHTMLLabel *rHTMLLabel_CT;
	TrHTMLLabel *rHTMLLabel_RD;
	TscGPNumericEdit *scGPNumericEdit_CT;
	TscGPNumericEdit *scGPNumericEdit_RD;
	TscGPComboBox *scComboBox_rezim;
	TscGPComboBox *scComboBox_pohon;
	TscGPEdit *scGPEdit_name;
	TscGPComboBox *scComboBox_cekani_palec;
	TscGPComboBox *scComboBox_stopka;
	TscGPNumericEdit *scGPNumericEdit_delka_dopravniku;
	TscGPEdit *scGPEdit_shortname;
	TrHTMLLabel *rHTMLLabel_InfoText;
	TscGPNumericEdit *scGPNumericEdit_odchylka;
	TscGPNumericEdit *scGPNumericEdit_kapacita;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RadioButton_na_delkuClick(TObject *Sender);
	void __fastcall RadioButton_na_sirkuClick(TObject *Sender);
	void __fastcall Image_vozikClick(TObject *Sender);
	void __fastcall Button_min_secClick(TObject *Sender);
	void __fastcall scComboBox_rezimChange(TObject *Sender);
	void __fastcall scGPEdit_nameChange(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scGPNumericEdit_CTChange(TObject *Sender);
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPNumericEdit_delka_dopravnikuChange(TObject *Sender);
	void __fastcall scGPNumericEdit_kapacitaChange(TObject *Sender);
	void __fastcall scGPNumericEdit_RDChange(TObject *Sender);





private:	// User declarations
	enum Tminsec{MIN=0,SEC};Tminsec minsec;
	enum Tcomponents{POHON,DELKA,CEKANI,ODCHYLKA,KAPACITA,STOPKA,TIME,RYCHLOST};
	enum Tcomponents_state{HIGHLIGHT,ENABLED,DISABLED,READONLY,HIDE};
	enum Tinput_state{NO,NOTHING,CT,DD,RD,C};//uchovává vybìr input hodnoty (aby se formuláøe necyklyly)


	void set(Tcomponents C,Tcomponents_state S);
	void input_CT();//pøepoèet hodnot vyplývajících ze zmìny CT
	void input_DD();//pøepoèet hodnot vyplývajících ze zmìny DD
	void input_RD();//pøepoèet hodnot vyplývajících ze zmìny RD
	void input_K();//pøepoèet hodnot vyplývajících ze zmìny Kapacity
	void null_input_value();//vynuluje vstupní hodnoty

	short offset;
	short defaultForm_parametryHeight;

	//UnicodeString SG,K,P;
	//unsigned short novy_parametr_n;


public:		// User declarations
	__fastcall TForm_parametry(TComponent* Owner);
	void setForm4Rezim(unsigned short rezim);
	Tinput_state input_state;
	void vypis(UnicodeString text,bool RED=true);
	short kapacitaSG;//požadavek na rozpad na více stejných S&G objektù


	void vykresli_vozik(bool na_delku=true);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
#endif
