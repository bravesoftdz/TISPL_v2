//---------------------------------------------------------------------------

#ifndef konzoleH
#define konzoleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include "scGPPagers.hpp"
#include "scHtmlControls.hpp"
#include "scModernControls.hpp"
#include "scExtControls.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "scGPImages.hpp"
#include "scImageCollection.hpp"
//---------------------------------------------------------------------------
class TForm_konzole : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *BorderPanel;
  TscPageViewer *ClientPageView;
  TscPageViewerPage *scPageViewerPage1;
  TscGPPanel *scGPPanel2;
  TscGPGlyphButton *CloseButton;
  TscLabel *scLabel_header;
  TscGPPanel *scGPPanel3;
  TscGPToolPager *scGPToolPager;
  TscMemo *Text;
  TscGPButton *scGPButton_odeslat;
  TscGPButton *scGPButton_storno;
  TscGPImage *scGPImage1;
  TscLabel *scLabel3;
	TscGPCheckBox *scGPCheckBox_odeslat_vcetne_projektu;
  TscGPComboBox *scGPComboBox1;
  void __fastcall CloseButtonClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall scGPButton_odeslatClick(TObject *Sender);
  void __fastcall scGPButton_stornoClick(TObject *Sender);
  void __fastcall TextClick(TObject *Sender);
private:	// User declarations
 String Text_formulare;
 String priloha_cesta;
 int count_click;
public:		// User declarations
  __fastcall TForm_konzole(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_konzole *Form_konzole;
//---------------------------------------------------------------------------
#endif
