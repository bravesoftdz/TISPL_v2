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
	unsigned int pocet_radku;//udr�uje po�et ��dk�, kter� bude nutno p�idat do tabulky
  String data_tabulky;//hudr�uje hodnoty bu��k tabulky

	void vytvor_napln_mGrid();//vytvo�� tabulku s hodnotami m��en�
	void napln_data_tabulky();//vytvo�� seznam dat o ��dc�ch tabulky m��en�
public:		// User declarations
	__fastcall TForm_mereni(TComponent* Owner);
	bool closing;//detekce kv�li hlavn�mu formul��i jinak vol� t�lo formactivate hlavn�ho formu
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_mereni *Form_mereni;
//---------------------------------------------------------------------------
#endif
