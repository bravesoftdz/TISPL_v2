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
//---------------------------------------------------------------------------
class TForm_zpravy : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *scGPPanel_header;
  TscGPGlyphButton *Skryt;
  TscLabel *scLabel1;
  TscGPGlyphButton *scGPGlyphButton_info;
  TscGPListBox *scGPListBox_zpravy;
  TscGPPanel *scGPPanel_statusbar;
  TRzStatusPane *RzStatusPane3;
  TscGPSizeBox *scGPSizeBox;
  TscGPGlyphButton *scGPGlyphButton1;
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
    void update_zpravy(double rezim=1); // 1 - celkový update zpráv, - 0 - update pouze horní lišty Warningù a Errorù
    int radek_temp;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_zpravy *Form_zpravy;
//---------------------------------------------------------------------------
#endif
