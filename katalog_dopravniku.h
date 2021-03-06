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
//#include "rHTMLLabel.hpp"
//#include "rHintWindow.hpp"
#include "scHint.hpp"
#include "scHtmlControls.hpp"
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
  TscHTMLLabel *scHTMLLabel_InfoText;
  TscHint *scHint1;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormPaint(TObject *Sender);
  void __fastcall Button_stornoClick(TObject *Sender);
  void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
  void __fastcall Button_saveClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall KonecClick(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
    void LoadValues();
		void LoadStyles();
    void getCheckSettings();

		TmGrid *K_mGrid;
    TPoint check;
    TColor clBACKGROUND_light,clBACKGROUND_dark,clNORMAL,clOTHER_AFTER_CHOOSE,clSELECTED_BLUE,clBACKGROUND;
    enum Tinput_state{NO,NOTHING,LOADING,JOB};//uchov�v� v�b�r input hodnoty (aby se formul��e necyklyly)
    unsigned int last_Col, last_Row;
    void clearCheck();


public:		// User declarations
  __fastcall TForm_katalog(TComponent* Owner);
  	//metody volan� z Tmgrid
		void OnClick(long Tag,long ID,unsigned long Col,unsigned long Row);
    bool zmena;
    double radius;
    unsigned int katalog_id;
    void vypis(UnicodeString text,bool red=true,bool link=false);
    Tinput_state input_state;//stav vstupu DV,SV,TT...atd
    unsigned long ColCount;
    unsigned long RowCount;

};
//---------------------------------------------------------------------------
extern PACKAGE TForm_katalog *Form_katalog;
//---------------------------------------------------------------------------
#endif
