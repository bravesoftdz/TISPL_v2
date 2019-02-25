//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "rHTMLLabel.hpp"
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel1;
	TscGPButton *Button_pridat;
	TscGPButton *Button_odebrat;
	TscGPButton *Button_storno;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TrHTMLLabel *rHTMLLabel1;
	TrHTMLLabel *rHTMLLabel2;
	TrHTMLLabel *rHTMLLabel3;
	TrHTMLLabel *rHTMLLabel4;
	TEdit *Edit5;
	TrHTMLLabel *rHTMLLabel5;
	void __fastcall Button_stornoClick(TObject *Sender);
	void __fastcall Button_pridatClick(TObject *Sender);
	void __fastcall Button_odebratClick(TObject *Sender);
private:	// User declarations
//	struct TPohon
//	{
//		string name;
//		double min_rychost;
//		double max_rychlost;
//		double roztec;
//	};
public:		// User declarations
	__fastcall TForm4(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
