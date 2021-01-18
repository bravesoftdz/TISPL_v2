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
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormClick(TObject *Sender);


private:	// User declarations
  int FX,FY;//poloha formu
	TmGrid *mGrid;
	unsigned int pocet_radku;//udržuje poèet øádkù, které bude nutno pøidat do tabulky
	String data_tabulky;//hudržuje hodnoty buòìk tabulky
	String data_tabulky_zaloha;//pøi vytváøení tabulky dojde ke smazání originálních dat, pro další práci (zmìna jednotek) nutné uložit
	int JID;//Job ID
	enum Tm_mm{MM=0,M,};Tm_mm j_delka;//pøepínaè jednotek vzdálenost
	enum Tminsec{SEC=0,MIN};Tminsec j_cas;//pøepínaè jednotek èasu

	void vytvor_napln_mGrid();//vytvoøí tabulku s hodnotami mìøení
	void napln_data_tabulky();//vytvoøí seznam dat o øádcích tabulky mìøení
  void zmen_jednotky_tabulce();//zmìna jednotek v tabulce
public:		// User declarations
	__fastcall TForm_mereni(TComponent* Owner);
	bool closing;//detekce kvùli hlavnímu formuláøi jinak volá tìlo formactivate hlavního formu
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_mereni *Form_mereni;
//---------------------------------------------------------------------------
#endif
