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
#include "vektory.h"
//---------------------------------------------------------------------------
class TForm_objekt_nahled : public TForm
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
	TscLabel *scLabel_odd�lit;
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
	TscGPGlyphButton *reposition_windowButton;
	TscGPGlyphButton *Max_Min_Button;
	TTimer *Timer_animace;
	TscGPGlyphButton *ButtonPLAY;
	TscGPLabel *scGPLabel_info;
	TButton *Button1;
	void __fastcall KonecClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y);
	void __fastcall GlyphButton_closeClick(TObject *Sender);
	void __fastcall GlyphButton_oddalitClick(TObject *Sender);
	void __fastcall GlyphButton_priblizitClick(TObject *Sender);
	void __fastcall GlyphButton_cely_pohledClick(TObject *Sender);
	void __fastcall reposition_windowButtonClick(TObject *Sender);
	void __fastcall Max_Min_ButtonClick(TObject *Sender);
	void __fastcall ButtonPLAYClick(TObject *Sender);
	void __fastcall Timer_animaceTimer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);



private:	// User declarations
	void PREVIEW();
	void MODEL();
	void REFRESH(bool invalidate=true);//vybere bu� Invalidate nebo FormPaint(this) dle if(!antialiasing)
	void ENTIRE();//cely_nahled, zajist� v�po�et zoomu tak, aby se objekt zobrazil p�es cel� okno, ale v�etn� okraje Ox
	void OUTPUT();//napln� data do lok�ln� (glob�ln� v tomto formul��i) prom�nn� pom,nejedn� se o F->pom, ta toto�i nen� v tomto momentu z d�vodu prob�haj�c�ch zm�na zcela aktu�ln�
	void START_POZICE();//nov� v�choz� pozice po rotaci voz�ku

	unsigned short MAX_pozic;//maxim�ln� hodnota zobrazen�ch pozic, jinak nastane ilustrativn� n�hled
	unsigned short Ox;//okraj
	double Poffset;
	double timerTakt;

public:		// User declarations
	__fastcall TForm_objekt_nahled(TComponent* Owner);
	void REFRESH_DATA();//obnov� dat ve formu Form_objekt_nahled v�etn� p�ekreslen�
	Cvektory::TObjekt *pom;
	double Zoom_predchozi;//slou�� na uchov�n� hodnoty aktu�ln�ho zoom ve sch�matu
	bool zobrazitFrameForm;//zda se m� �i nem� zobrazit or�mov�n� formul��e
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_objekt_nahled *Form_objekt_nahled;
//---------------------------------------------------------------------------
#endif
