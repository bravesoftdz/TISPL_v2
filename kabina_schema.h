//---------------------------------------------------------------------------
#ifndef kabina_schemaH
#define kabina_schemaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TForm_kabina_schema : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPButton *scGPButton_OK;
	TImage *Image_nahled;
	TscGPPanel *PopUPmenu;
	TscGPPanel *Item_cely_pohled;
	TscLabel *scLabel_cely_pohled;
	TscGPGlyphButton *GlyphButton_cely_pohled;
	TscGPPanel *Item_oddalit;
	TscLabel *scLabel_oddálit;
	TscGPGlyphButton *GlyphButton_oddalit;
	TscGPPanel *Item_vybrat_oknem;
	TscLabel *scLabel_vybrat_oknem;
	TscGPGlyphButton *GlyphButton_vybrat_oknem;
	TscGPPanel *Item_priblizit;
	TscLabel *scLabel_priblizit;
	TscGPGlyphButton *GlyphButton_priblizit;
	TscGPPanel *Item_posunout;
	TscLabel *scLabel_posunout;
	TscGPGlyphButton *GlyphButton_posunout;
	TscGPPanel *Item_posouvat;
	TscGPGlyphButton *GlyphButton_posouvat;
	TscLabel *scLabel_posouvat;
	TscGPGlyphButton *GlyphButton_close;
	void __fastcall scGPButton_stornoClick(TObject *Sender);
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall scLabel_priblizitClick(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y);
private:	// User declarations
	double Zoom_predchozi;
public:		// User declarations
	__fastcall TForm_kabina_schema(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_kabina_schema *Form_kabina_schema;
//---------------------------------------------------------------------------
#endif
