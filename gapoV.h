//---------------------------------------------------------------------------

#ifndef gapoVH
#define gapoVH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "vektory.h"
//---------------------------------------------------------------------------
class TF_gapoV : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscButton *scButton_html;
private:	// User declarations
	short Offset;//odsazení tabulky po všech stranách formu
	Cvektory::TObjekt *objekty;//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, pouze duplikát objektù

public:		// User declarations
	__fastcall TF_gapoV(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoV *F_gapoV;
//---------------------------------------------------------------------------
#endif
