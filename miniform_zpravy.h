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
#include "RzStatus.hpp"
#include "scGPImages.hpp"
//---------------------------------------------------------------------------
class TForm_zpravy : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *scGPPanel_header;
  TscGPGlyphButton *Skryt;
  TscLabel *scLabel_header;
  TscGPGlyphButton *scGPGlyphButton_info;
  TscGPListBox *scGPListBox_zpravy;
  TscGPPanel *scGPPanel_statusbar;
  TRzStatusPane *RzStatusPane_pocet_chyb_value;
  TscGPSizeBox *scGPSizeBox;
  TRzStatusPane *RzStatusPane__chyby_caption;
  TRzStatusPane *RzStatusPane_var_header;
  TRzStatusPane *RzStatusPane_pocet_var_value;
  TscGPImage *scGPImage_warning;
  TscGPImage *scGPImage_error;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall scGPListBox_zpravyMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall scGPGlyphButton_infoClick(TObject *Sender);
  void __fastcall SkrytClick(TObject *Sender);
  void __fastcall scGPListBox_zpravyItemClick(TObject *Sender);
  void __fastcall scGPListBox_zpravyMouseLeave(TObject *Sender);
private:	// User declarations

public:		// User declarations
  __fastcall TForm_zpravy(TComponent* Owner);
  	bool closing;//detekce kvùli hlavnímu formuláøi jinak volá tìlo formactivate hlavního formu
    void update_zpravy(long pocet_erroru, long pocet_warningu); // 1 - celkový update zpráv, - 0 - update pouze horní lišty Warningù a Errorù
    void highlight(int radek=0);
    int radek_temp;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_zpravy *Form_zpravy;
//---------------------------------------------------------------------------
#endif
