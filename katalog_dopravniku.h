//---------------------------------------------------------------------------

#ifndef katalog_dopravnikuH
#define katalog_dopravnikuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "TmGrid.h"
#include "Unit1.h"
#include "vektory.h"
#include "parametry_linky.h"
//---------------------------------------------------------------------------
class TForm_katalog : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *scGPPanel2;
  TscGPGlyphButton *Konec;
  TscLabel *scLabel_header;
  TscGPButton *Button_save;
  TscGPButton *Button_storno;
  TMemo *Memo1;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
  void __fastcall Button_stornoClick(TObject *Sender);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
  void __fastcall Button_saveClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
    void LoadValues();
    void LoadStyles();
public:		// User declarations
  __fastcall TForm_katalog(TComponent* Owner);
  	//metody volan� z Tmgrid
		void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
    bool zmena;
    double radius;
    int katalog_id;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_katalog *Form_katalog;
//---------------------------------------------------------------------------
#endif