//---------------------------------------------------------------------------

#ifndef casovaOsa_infoH
#define casovaOsa_infoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TForm_osa_info : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPGlyphButton *MaxButton;
	TscLabel *scLabel1;
	TscGPButton *ioo_ok;
	TrHTMLLabel *rHTMLLabel_nazev;
	TrHTMLLabel *rHTMLLabel_rezim;
	TrHTMLLabel *rHTMLLabel_ct;
	TrHTMLLabel *rHTMLLabel_nazev_vypis;
	TrHTMLLabel *rHTMLLabel_rezim_vypis;
	TrHTMLLabel *rHTMLLabel_ct_vypis;
	TrHTMLLabel *rHTMLLabel_dop_kap;
	TrHTMLLabel *rHTMLLabel_dop_kap_vypis;
	TrHTMLLabel *rHTMLLabel_pozad_kap;
	TrHTMLLabel *rHTMLLabel_pozad_kap_vypis;
	TrHTMLLabel *rHTMLLabel_rd;
	TrHTMLLabel *rHTMLLabel_rd_vypis;
	TrHTMLLabel *rHTMLLabel_palce;
	TrHTMLLabel *rHTMLLabel_palce_vypis;
	TrHTMLLabel *rHTMLLabel_zkratka;
	TrHTMLLabel *rHTMLLabel_zkratka_vypis;
	TrHTMLLabel *rHTMLLabel_cislo_voziku_vypis;
	TrHTMLLabel *rHTMLLabel_cislo_voziku;
	TrHTMLLabel *rHTMLLabel_mt_vypis;
	TrHTMLLabel *rHTMLLabel_tpoc;
	TrHTMLLabel *rHTMLLabel_tpoc_vypis;
	TrHTMLLabel *rHTMLLabel_tkon;
	TrHTMLLabel *rHTMLLabel_wt_vypis;
	TrHTMLLabel *rHTMLLabel_mt;
	TrHTMLLabel *rHTMLLabel_str_dob_cek_vypis;
	TrHTMLLabel *rHTMLLabel_str_dob_cekani;
	TrHTMLLabel *rHTMLLabel_max_doba_cekani_vypis;
	TrHTMLLabel *rHTMLLabel_max_dob_cekani;
	TrHTMLLabel *rHTMLLabel_pt_vypis;
	TrHTMLLabel *rHTMLLabel_wt;
	TrHTMLLabel *rHTMLLabel_tkon_vypis;
	TrHTMLLabel *rHTMLLabel_pt;
	TrHTMLLabel *rHTMLLabel_tdor;
	TrHTMLLabel *rHTMLLabel_tdor_vypis;
	TrHTMLLabel *rHTMLLabel_tpre;
	TrHTMLLabel *rHTMLLabel_tpre_vypis;
	TrHTMLLabel *rHTMLLabel_tcek;
	TrHTMLLabel *rHTMLLabel_tcek_vypis;
	TrHTMLLabel *rHTMLLabel_CTn;
	TrHTMLLabel *rHTMLLabel_CT_n_vypis;
private:	// User declarations
public:		// User declarations
	__fastcall TForm_osa_info(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_osa_info *Form_osa_info;
//---------------------------------------------------------------------------
#endif
