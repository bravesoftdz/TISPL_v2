//---------------------------------------------------------------------------

#ifndef cestyH
#define cestyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//#include "rHTMLLabel.hpp"
//#include "rStringGridEd.hpp"
//#include "RzButton.hpp"
#include <Vcl.Grids.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
//---------------------------------------------------------------------------
class TForm_cesty : public TForm
{
__published:	// IDE-managed Components
	TrStringGridEd *rStringGridEd_cesty;
	TscGPPanel *scGPPanel2;
	TscGPGlyphButton *Konec;
	TscGPGlyphButton *MinButton;
	TscGPGlyphButton *MaxButton;
	TscLabel *scLabel1;
	TscGPButton *scGPButton2;
	TscGPButton *cesta_storno;
	TscGPButton *Button_ulozit;
	TMemo *Memo1;
	TButton *vypis_objekty;
	TscGPButton *scGPButton_min_sec;
	void __fastcall cesta_stornoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall vypis_objektyClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall scGPButton_min_secClick(TObject *Sender);
	void __fastcall rStringGridEd_cestyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
public:		// User declarations

		void nactiNastaveni();
		enum Tminsec{S=0,MIN};Tminsec CasoveJednotky;//pøepínaè jednotek èasu

	__fastcall TForm_cesty(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_cesty *Form_cesty;
//---------------------------------------------------------------------------
#endif
