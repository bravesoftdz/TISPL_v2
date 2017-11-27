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
	TrHTMLLabel *rHTMLLabel_rotace;
	TscGPComboBox *scComboBox_rotace;
	TrHTMLLabel *rHTMLLabel_mezera;
	TscGPNumericEdit *scGPNumericEdit_mezera;
	TscGPCheckBox *scGPCheckBox_pocet_mezer;
	TscGPButton *scGPButton_metry_milimetry;
	TscGPGlyphButton *scGPGlyphButton_copy;
	TscGPGlyphButton *scGPGlyphButton_paste;
	TscButton *scButton1;
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
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button_metry_milimetryClick(TObject *Sender);
	void __fastcall scGPCheckBox_pocet_mezerClick(TObject *Sender);
	void __fastcall scGPGlyphButton_copyClick(TObject *Sender);
	void __fastcall scGPGlyphButton_pasteClick(TObject *Sender);
	void __fastcall rHTMLLabel_CTClick(TObject *Sender);

private:	// User declarations
	enum Tcomponents{POHON,DELKA,CEKANI,ODCHYLKA,KAPACITA,STOPKA,TIME,RYCHLOST,ROTACE,MEZERA,POCET_MEZER};//n·zev souvisejÌcÌch komponent
	enum Tcomponents_state{HIGHLIGHT,ENABLED,DISABLED,READONLY,HIDE};//stav komponent
	enum Tinput_state{NO,NOTHING,CT,DD,RD,C};//uchov·v· v˝bÏr input hodnoty (aby se formul·¯e necyklyly)


	void set(Tcomponents C,Tcomponents_state S);//zajiöùuje zobrazenÌ a napozicov·nÌ pat¯iËnÈ konkrÈtnÌ komponenty a z·roveÚ udrûov·nÌ hodnoty offsetu
	void input_CT();//p¯epoËet hodnot vypl˝vajÌcÌch ze zmÏny CT
	void input_DD();//p¯epoËet hodnot vypl˝vajÌcÌch ze zmÏny DD
	void input_RD();//p¯epoËet hodnot vypl˝vajÌcÌch ze zmÏny RD
	void input_K();//p¯epoËet hodnot vypl˝vajÌcÌch ze zmÏny Kapacity
	void null_input_value();//vynuluje vstupnÌ hodnoty

	short offset;
	short defaultForm_parametryHeight;
	TColor hl_color;//(TColor)RGB(255,141,28);//barva zv˝raznÏnÌ r·meËku komponenty nap¯.pro povinnÈ poloûky
	short hlFrameWidth;//öÌ¯ka zv˝raznÏnÌ r·meËku komponenty nap¯.pro povinnÈ poloûky

public:		// User declarations
	__fastcall TForm_parametry(TComponent* Owner);
	enum Tminsec{S=0,MIN};Tminsec minsec;Tminsec CTunit;//p¯epÌnaË jednotek Ëasu
	enum Tm_mm{M=0,MM};Tm_mm m_mm;//p¯epÌnaË jednotek vzd·lenost
	void setForm4Rezim(unsigned short rezim);
	Tinput_state input_state;
	void vypis(UnicodeString text,bool RED=true);
	short kapacitaSG;//poûadavek na rozpad na vÌce stejn˝ch S&G objekt˘

	void vykresli_vozik(bool na_delku=true);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_parametry *Form_parametry;
//---------------------------------------------------------------------------
#endif
