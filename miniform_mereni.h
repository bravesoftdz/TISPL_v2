//---------------------------------------------------------------------------

#ifndef miniform_mereniH
#define miniform_mereniH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "TmGrid.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_mereni : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_header;
	TscGPGlyphButton *Skryt;
	TscLabel *scLabel_header;
	TscGPGlyphButton *scGPGlyphButton_pripnout;
	TscGPButton *scGPButton_OK;
	void __fastcall SkrytClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scLabel_headerMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall scGPGlyphButton_pripnoutClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
  int FX,FY;//poloha formu
	TmGrid *mGrid;
	unsigned int pocet_radku;//udržuje poèet øádkù, které bude nutno pøidat do tabulky
  String data_tabulky;//hudržuje hodnoty buòìk tabulky

	void vytvor_napln_mGrid();//vytvoøí tabulku s hodnotami mìøení
	void napln_data_tabulky();//vytvoøí seznam dat o øádcích tabulky mìøení
public:		// User declarations
	__fastcall TForm_mereni(TComponent* Owner);
	bool closing;//detekce kvùli hlavnímu formuláøi jinak volá tìlo formactivate hlavního formu
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_mereni *Form_mereni;
//---------------------------------------------------------------------------
#endif
