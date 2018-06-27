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
	short Offset;//odsazen� tabulky po v�ech stran�ch formu
	Cvektory::TObjekt *objekty;//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, pouze duplik�t objekt�

public:		// User declarations
	__fastcall TF_gapoTT(TComponent* Owner);
		TPoint *pohony_zmena;//dynamick� pole eviduj�c� zm�ny na PL u pohon�
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoTT *F_gapoTT;
//---------------------------------------------------------------------------
#endif
