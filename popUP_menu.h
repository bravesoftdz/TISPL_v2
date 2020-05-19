//---------------------------------------------------------------------------

#ifndef popUP_menuH
#define popUP_menuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scModernControls.hpp"
//---------------------------------------------------------------------------
class TPopUPmenu : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *Item_zobrazit_parametry;
	TscLabel *scLabel_zobrazit_parametry;
	TscGPGlyphButton *GlyphButton_zobrazit_parametry;
	TscGPPanel *Item_posouvat;
	TscLabel *scLabel_posouvat;
	TscGPGlyphButton *GlyphButton_posouvat;
	TscGPPanel *Item_rychly_export;
	TscLabel *scLabel_rychly_export;
	TscGPGlyphButton *GlyphButton_rychly_export;
	TscGPPanel *Item_smazat;
	TscGPPanel *Item_cely_pohled;
	TscLabel *scLabel_cely_pohled;
	TscGPGlyphButton *GlyphButton_cely_pohled;
	TscGPPanel *Item_vybrat_oknem;
	TscLabel *scLabel_vybrat_oknem;
	TscGPGlyphButton *GlyphButton_vybrat_oknem;
	TscGPPanel *Item_posunout;
	TscLabel *scLabel_posunout;
	TscGPPanel *Item_oddalit;
	TscLabel *scLabel_oddálit;
	TscGPGlyphButton *GlyphButton_oddalit;
	TscGPPanel *Item_priblizit;
	TscLabel *scLabel_priblizit;
	TscGPGlyphButton *GlyphButton_priblizit;
	TscGPPanel *Item_nastavit_parametry;
	TscLabel *scLabel_nastavit_parametry;
	TscGPGlyphButton *GlyphButton_nastavit_parametry;
	TscGPGlyphButton *GlyphButton_posunout;
	TscGPGlyphButton *GlyphButton_smazat;
	TscGPGlyphButton *GlyphButton_close;
	TscGPPanel *Item_poznamka;
	TscLabel *scLabel_poznamka;
	TscGPGlyphButton *GlyphButton_poznamka;
	TscGPPanel *Item_kopirovat;
	TscLabel *scLabel_kopirovat;
	TscGPGlyphButton *GlyphButton_kopirovat;
	TscGPPanel *Panel_UP;
	TscGPPanel *Panel_DOWN;
	TscLabel *scLabel_smazat;
	TscGPPanel *Item_posun_obrysu;
	TscLabel *scLabel_posun_obrysu;
	TscGPGlyphButton *GlyphButton_posun_obrysu;
	TscGPPanel *Item_otocit_doleva;
	TscLabel *scLabel_otocit_doleva;
	TscGPGlyphButton *GlyphButton_otocit_doleva;
	TscGPPanel *Item_otocit_doprava;
	TscLabel *scLabel_otocit_doprava;
	TscGPGlyphButton *GlyphButton_otocit_doprava;
	TscGPPanel *Item_zobrazitskryt_steny;
	TscLabel *scLabel_zobrazitskryt_steny;
	TscGPGlyphButton *scGPGlyphButton_zobrazitskryt_steny;
	void __fastcall scLabel_smazatClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall scLabel_zobrazit_parametryMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_closeClick(TObject *Sender);
	void __fastcall scLabel_nastavit_parametryMouseEnter(TObject *Sender);
	void __fastcall scLabel_smazatMouseEnter(TObject *Sender);
	void __fastcall scLabel_rychly_exportMouseEnter(TObject *Sender);
	void __fastcall scLabel_posouvatMouseEnter(TObject *Sender);
	void __fastcall scLabel_posunoutMouseEnter(TObject *Sender);
	void __fastcall scLabel_priblizitMouseEnter(TObject *Sender);
	void __fastcall scLabel_oddálitMouseEnter(TObject *Sender);
	void __fastcall scLabel_vybrat_oknemMouseEnter(TObject *Sender);
	void __fastcall scLabel_cely_pohledMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_closeMouseEnter(TObject *Sender);
	void __fastcall scLabel_nastavit_parametryMouseLeave(TObject *Sender);
	void __fastcall scLabel_zobrazit_parametryMouseLeave(TObject *Sender);
	void __fastcall scLabel_smazatMouseLeave(TObject *Sender);
	void __fastcall scLabel_rychly_exportMouseLeave(TObject *Sender);
	void __fastcall scLabel_posouvatMouseLeave(TObject *Sender);
	void __fastcall scLabel_posunoutMouseLeave(TObject *Sender);
	void __fastcall scLabel_priblizitMouseLeave(TObject *Sender);
	void __fastcall scLabel_oddálitMouseLeave(TObject *Sender);
	void __fastcall scLabel_vybrat_oknemMouseLeave(TObject *Sender);
	void __fastcall scLabel_cely_pohledMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_nastavit_parametryMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_nastavit_parametryMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_zobrazit_parametryMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_zobrazit_parametryMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_smazatMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_smazatMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_rychly_exportMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_rychly_exportMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_posouvatMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_posouvatMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_posunoutMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_posunoutMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_priblizitMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_priblizitMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_oddalitMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_oddalitMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_vybrat_oknemMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_vybrat_oknemMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_cely_pohledMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_cely_pohledMouseLeave(TObject *Sender);
	void __fastcall scLabel_nastavit_parametryClick(TObject *Sender);
	void __fastcall scLabel_zobrazit_parametryClick(TObject *Sender);
	void __fastcall scLabel_rychly_exportClick(TObject *Sender);
	void __fastcall scLabel_posouvatClick(TObject *Sender);
	void __fastcall scLabel_posunoutClick(TObject *Sender);
	void __fastcall scLabel_priblizitClick(TObject *Sender);
	void __fastcall scLabel_oddálitClick(TObject *Sender);
	void __fastcall scLabel_vybrat_oknemClick(TObject *Sender);
	void __fastcall scLabel_cely_pohledClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scLabel_kopirovatClick(TObject *Sender);
	void __fastcall GlyphButton_kopirovatMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_kopirovatMouseLeave(TObject *Sender);
	void __fastcall scLabel_kopirovatMouseEnter(TObject *Sender);
	void __fastcall scLabel_smazatLeave(TObject *Sender);
	void __fastcall scLabel_otocit_dopravaMouseEnter(TObject *Sender);
	void __fastcall scLabel_otocit_dopravaMouseLeave(TObject *Sender);
	void __fastcall scLabel_otocit_dopravaClick(TObject *Sender);
	void __fastcall GlyphButton_otocit_dopravaMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_otocit_dopravaMouseLeave(TObject *Sender);
	void __fastcall scLabel_otocit_dolevaClick(TObject *Sender);
	void __fastcall scLabel_otocit_dolevaMouseEnter(TObject *Sender);
	void __fastcall scLabel_otocit_dolevaMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_otocit_dolevaMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_otocit_dolevaMouseLeave(TObject *Sender);
	void __fastcall scLabel_posun_obrysuClick(TObject *Sender);
	void __fastcall scLabel_posun_obrysuMouseEnter(TObject *Sender);
	void __fastcall scLabel_posun_obrysuMouseLeave(TObject *Sender);
	void __fastcall GlyphButton_posun_obrysuMouseEnter(TObject *Sender);
	void __fastcall GlyphButton_posun_obrysuMouseLeave(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall scLabel_zobrazitskryt_stenyMouseEnter(TObject *Sender);
	void __fastcall scLabel_zobrazitskryt_stenyMouseLeave(TObject *Sender);
	void __fastcall scLabel_zobrazitskryt_stenyClick(TObject *Sender);
	void __fastcall scGPGlyphButton_zobrazitskryt_stenyMouseEnter(TObject *Sender);
	void __fastcall scGPGlyphButton_zobrazitskryt_stenyMouseLeave(TObject *Sender);


private:	// User declarations
	TColor clBg,clAcBg,clAcBg2,clGlyph,clAcGlyph;
	void top_positon(int top);//hlídání horní pozice, je-li daná komponenta horní
public:		// User declarations
	bool closing;//detekce kvùli hlavnímu formuláøi jinak volá tìlo formactivate hlavního formu
  void pasiveColor();
	__fastcall TPopUPmenu(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPopUPmenu *PopUPmenu;
//---------------------------------------------------------------------------
#endif
