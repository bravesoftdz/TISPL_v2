//---------------------------------------------------------------------------

#ifndef gapoTTH
#define gapoTTH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "vektory.h"
//---------------------------------------------------------------------------
class TF_gapoTT : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscButton *scButton_html;
	TscGPButton *scGPButton_OK;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
	short Offset;//odsazení tabulky po všech stranách formu
	Cvektory::TObjekt *objekty;//dynamické pole, uchovávající ukazatele na objekty v tabulce sloupci objekty, pouze duplikát objektù

public:		// User declarations
	__fastcall TF_gapoTT(TComponent* Owner);
		TPoint *pohony_zmena;//dynamické pole evidující zmìny na PL u pohonù
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoTT *F_gapoTT;
//---------------------------------------------------------------------------
#endif
