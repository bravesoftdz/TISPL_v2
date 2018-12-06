//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "adjustace.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_adjustace *Form_adjustace;
//---------------------------------------------------------------------------
__fastcall TForm_adjustace::TForm_adjustace(TComponent* Owner)
  : TForm(Owner)
{
		Form1->m.designButton(scGPButton_OK, Form_adjustace, 1, 2);
		Form1->m.designButton(scGPButton_storno, Form_adjustace, 2, 2);
}
//---------------------------------------------------------------------------
void __fastcall TForm_adjustace::scGPButton_stornoClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_adjustace::KonecClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_adjustace::FormShow(TObject *Sender)
{
 		Form_adjustace->Left=Form1->ClientWidth/2-Form_adjustace->Width/2;
 		Form_adjustace->Top=Form1->ClientHeight/2-Form_adjustace->Height/2;
    position(); //napozicování - formuláø se zobrazí 10px od kurzoru
    scGPNumericEdit_vzdalenost->SetFocus();
}
//---------------------------------------------------------------------------
void TForm_adjustace::position()
{
		long X = Form1->akt_souradnice_kurzoru_PX.x + 10;
		long Y = Form1->akt_souradnice_kurzoru_PX.y + 10;

		if(X + Form_adjustace->ClientWidth < Form1->ClientWidth) Form_adjustace->Left = X;
		else Form_adjustace->Left = Form1->ClientWidth - Form_adjustace->ClientWidth - 10;
		if(Y + Form_adjustace->ClientHeight < Form1->ClientHeight) Form_adjustace->Top = Y;
		else Form_adjustace->Top = Form1->ClientHeight - Form_adjustace->ClientHeight - Form1->scGPPanel_statusbar->Height - 10;


}
void __fastcall TForm_adjustace::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key==13)//ENTER
	{
		 if(scGPButton_OK->Enabled)scGPButton_OKClick(Sender);//pokud jsou zároveò splnìny podmínky pro stisk OK
		 else MessageBeep(0);//pípnutím upozorní, že nelze
	}
	if(Key==27)//ESC
	{
			scGPButton_stornoClick(Sender);
			Close();//musí tu být, protože Button_storno ukonèuje form pouze pøes modalresults a bez tohoto by se neukonèilo

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_adjustace::scGPButton_OKClick(TObject *Sender)
{
  ModalResult=mrOk;    //kvùli enter na klávesnici zde je nutné pøidat ještì toto
}
//---------------------------------------------------------------------------

