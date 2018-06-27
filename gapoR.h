//---------------------------------------------------------------------------

#ifndef gapoRH
#define gapoRH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "vektory.h"
//---------------------------------------------------------------------------
class TF_gapoR : public TForm
{
__published:	// IDE-managed Components
	TscGPPanel *scGPPanel_hlavicka;
	TscGPGlyphButton *Konec;
	TscLabel *scLabel_titulek;
	TscGPGlyphButton *scGPGlyphButton_info;
	TscGPButton *scGPButton_OK;
	TButton *Button1;
	TscButton *scButton_html;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall scGPButton_OKClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	//Cvektory v;//instance na t��du vektor� - nepou��v�t, padalo
	short Offset;//odsazen� tabulky po v�ech stran�ch formu
	Cvektory::TObjekt *objekty;//dynamick� pole, uchov�vaj�c� ukazatele na objekty v tabulce sloupci objekty, pouze duplik�t objekt�
public:		// User declarations
	__fastcall TF_gapoR(TComponent* Owner);
  //metody volan� z Tmgrid
	void OnClick(long Tag,unsigned long Col,unsigned long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	TPoint *pohony_zmena;//dynamick� pole eviduj�c� zm�ny na PL u pohon�
};
//---------------------------------------------------------------------------
extern PACKAGE TF_gapoR *F_gapoR;
//---------------------------------------------------------------------------
#endif
