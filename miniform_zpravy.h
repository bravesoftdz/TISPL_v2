//---------------------------------------------------------------------------

#ifndef miniform_zpravyH
#define miniform_zpravyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
//#include "RzStatus.hpp"
#include "scGPImages.hpp"
//---------------------------------------------------------------------------
class TForm_zpravy : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *scGPPanel_header;
  TscGPGlyphButton *Skryt;
  TscLabel *scLabel_header;
  TscGPGlyphButton *scGPGlyphButton_pripnout;
  TscGPListBox *scGPListBox_zpravy;
  TscGPPanel *scGPPanel_statusbar;
  TscGPSizeBox *scGPSizeBox;
  TscGPImage *scGPImage_warning;
  TscGPImage *scGPImage_error;
  TscGPGlyphButton *scGPGlyphButton2;
  TscLabel *scLabel_varovani;
  TscLabel *scLabel_chyby;
  TscGPGlyphButton *scGPGlyphButton1;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall scGPListBox_zpravyMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall scGPGlyphButton_pripnoutClick(TObject *Sender);
  void __fastcall SkrytClick(TObject *Sender);
  void __fastcall scGPListBox_zpravyItemClick(TObject *Sender);
  void __fastcall scGPListBox_zpravyMouseLeave(TObject *Sender);
	void __fastcall scGPGlyphButton_pripnoutResize(TObject *Sender);
  void __fastcall scLabel_headerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall scLabel_headerMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);

private:	// User declarations
	long pocet_erroru,pocet_warningu;

public:		// User declarations
  __fastcall TForm_zpravy(TComponent* Owner);
		bool closing;//detekce kv�li hlavn�mu formul��i jinak vol� t�lo formactivate hlavn�ho formu
		void update_zpravy(long pocet_erroru, long pocet_warningu);
		void highlight(int radek=0);
    int radek_temp;
    short mouse_move;
    bool custom_size;
    int POCET_ERRORU;
    int POCET_WARNINGU;
    double default_width;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_zpravy *Form_zpravy;
//---------------------------------------------------------------------------
#endif
