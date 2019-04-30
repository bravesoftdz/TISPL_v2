//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "scControls.hpp"
#include "scGPControls.hpp"
#include "scGPExtControls.hpp"
#include <Vcl.Mask.hpp>
#include "TmGrid.h"
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TscGPButton *scGPButton_zamek_roztec;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
//TmGrid *mGrid2;

struct TElement
	{
		TmGrid *mG;
		struct TElement *predchozi;//ukazatel na pøedchozí objekt ve spojovém seznamu
		struct TElement *dalsi;//ukazatel na další objekt ve spojovém seznamu
	};
	TElement *ELEMENTY;//testovací spojový seznam elementù

	bool AA;
	long puvX,puvY;
	double aktX,aktY;
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
	void OnClick(long Tag,long ID, long Col, long Row);
	void OnEnter(long Tag,unsigned long Col,unsigned long Row);
	void OnChange(long Tag,unsigned long Col,unsigned long Row);
	void OnKeyPress(long Tag,unsigned long Col,unsigned long Row,System::WideChar &Key);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
