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
  TscGPSizeBox *scGPSizeBox1;
  TscGPToolPager *scGPToolPager;
  TscMemo *Text;
  TscGPButton *scGPButton_ulozit_cestu;
  TscGPButton *scGPButton_storno_cesta;
  TscGPImage *scGPImage1;
  TscLabel *scLabel2;
  TscLabel *scLabel3;
  void __fastcall CloseButtonClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall scGPButton_ulozit_cestuClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm_konzole(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_konzole *Form_konzole;
//---------------------------------------------------------------------------
#endif
