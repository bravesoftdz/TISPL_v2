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
#include "scGPImages.hpp"
#include "scImageCollection.hpp"
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
	TButton *Button6;
	TMemo *Memo1;
	TButton *Button7;
	TscGPImageCollection *scGPImageCollection1;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TButton *Button11;
	TEdit *Edit1;
	TButton *Button12;
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
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);


private:	// User declarations
//TmGrid *mGrid2;

struct TElement
	{
		TmGrid *mG;
		struct TElement *predchozi;//ukazatel na p�edchoz� objekt ve spojov�m seznamu
		struct TElement *dalsi;//ukazatel na dal�� objekt ve spojov�m seznamu
	};
	TElement *ELEMENTY;//testovac� spojov� seznam element�

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
