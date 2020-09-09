//---------------------------------------------------------------------------

#ifndef helpH
#define helpH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scExtControls.hpp"
#include "scToolPager.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include "scGPPagers.hpp"
#include "scAdvancedControls.hpp"
#include "scAppPager.hpp"
#include "scGPFontControls.hpp"
#include "scModernControls.hpp"
#include "scStyledForm.hpp"
#include "scStyleManager.hpp"
#include <Vcl.Mask.hpp>
#include <Vcl.Menus.hpp>
#include "scHtmlControls.hpp"

//---------------------------------------------------------------------------
class TForm_help : public TForm
{
__published:	// IDE-managed Components
  TscGPPanel *BorderPanel;
  TscPageViewer *ClientPageView;
  TscPageViewerPage *scPageViewerPage1;
  TscGPPanel *scGPPanel2;
  TscGPGlyphButton *CloseButton;
  TscLabel *scLabel1;
  TscGPPanel *scGPPanel4;
  TscGPPanel *scGPPanel3;
  TscGPSizeBox *scGPSizeBox1;
  TscGPToolPager *scGPToolPager;
  TscGPToolPagerPage *scGPToolPagerPage3;
  TscLabel *scLabel28;
  TscGPToolPagerPage *scGPToolPagerPage_layout;
  TscGPToolPagerPage *scGPToolPagerPage_PL;
  TscStyledForm *scStyledForm1;
  TscStyleManager *scStyleManager1;
  TscGPToolPagerPage *scGPToolPagerPage4;
  TscLabel *scLabel4;
  TscGPToolPagerPage *scGPToolPagerPage_about;
  TscHTMLLabel *scHTMLLabel_about;
  TscHTMLLabel *scHTMLLabel_PL;
  TscHTMLLabel *scHTMLLabel_layout;
  void __fastcall CloseButtonClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall scGPToolPagerClick(TObject *Sender);
	void __fastcall scHTMLLabel_aboutLinkClick(TObject *Sender, int LinkIndex, UnicodeString LinkText,
          UnicodeString LinkURL);
private:	// User declarations
public:		// User declarations
  __fastcall TForm_help(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_help *Form_help;
//---------------------------------------------------------------------------
#endif
