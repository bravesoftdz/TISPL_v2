//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
    //Form3->Height = E_vstup2->Top + E_vstup2->Height + 2 * (E_vstup2->Top + E_vstup2->Height - E_vstup1->Top);
	F->m.designButton(Button_save,Form3,1,2);
	F->m.designButton(Button_storno,Form3,2,2);                 // Nastaven� vzhledu tla��tek
	Form3->Color=(TColor)RGB(240,240,240);    					// Nastaven� barvy formul��e
	Form3->Left=Form1->ClientWidth/2-Form3->Width/2;
	Form3->Top=Form1->ClientHeight/2-Form3->Height/2;           // Nastaven� otev�r�n� okna ve st�edu obrazovky
    Button1->Width = Button_save->Width;
	Button1->Top=Button_save->Top;
	Button1->Left=(Button_storno->Left - Button_save->Left) + Button_storno->Left;
	Button1->Height = Button_save->Height;
	E_vstup1->Width = Form3->Width - E_vstup1->Left - (Form3->Width - Button1->Left - Button1->Width);
	E_vstup2->Width = Form3->Width - E_vstup2->Left - (Form3->Width - Button1->Left - Button1->Width);
	Form3->Width = E_vstup1->Left + E_vstup1->Width + rHTMLLabel_cislo1->Left;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button_stornoClick(TObject *Sender)
{
	if (mrOk == F->MB("Obsah bude ztracen, opravdu chcete pokra�ovat?",MB_OKCANCEL)) {
		Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button_saveClick(TObject *Sender)
{
	 vypocet ();
}
//---------------------------------------------------------------------------

void TForm3::vypocet ()
{
	double cislo1,cislo2,vysledek;
	cislo1 = F->ms.MyToDouble(E_vstup1->Text);
	cislo2 = F->ms.MyToDouble(E_vstup2->Text);
	vysledek = cislo1 + cislo2;
	F->MB("V�sledek sou�tu je: "+AnsiString(vysledek));
}

void TForm3::kontrola ()
{
	 if (pom1 && pom2) {
		Button_save->Enabled = true;
	 }
}

void __fastcall TForm3::FormShow(TObject *Sender)
{
	 pom1 = false;
	 pom2 = false;
	 Button_save->Enabled = false;
	 E_vstup1->Clear();
	 E_vstup2->Clear();
	 Memo1->Clear();

//	 if (F->d.v.POHONY->predchozi->name != ""){                // Nefunk�n� nev�m pro�
//		Memo1->Lines->Add("Nejsou zad�ny ��dn� pohony!");
//	 }

	 Cvektory::TPohon *O=Form1->d.v.POHONY->dalsi;      // Nechat si l�pe vysv�tlit
	 while(O!=NULL)
	 {
		Memo1->Lines->Add(O->name);
		O=O->dalsi;
	 }
	 Memo1->Height = Memo1->Lines->Count * 21;		// Nastaven� v��ky polde po�tu ��dk�
}
//---------------------------------------------------------------------------

void __fastcall TForm3::E_vstup1Change(TObject *Sender)
{
	 if (E_vstup1->Text!="") {
		pom1 = true;
		kontrola();
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::E_vstup2Change(TObject *Sender)
{
	 if (E_vstup2->Text!="") {
		pom2 = true;
		kontrola();
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 13) { 	// 13 je enter, funguje divn� zeptat se !
		vypocet ();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button1Click(TObject *Sender)
{
//	if (F->d.v.POHONY->predchozi->name != "") {
//		F->MB("N�zev posledn�ho pohonu je: " + F->d.v.POHONY->predchozi->name + "<br>      "); // Pro lep�� zobrazen�
//	}
//	else F->MB("Nebyl zad�n ��dn� pohon!");
	F->MB("Pohon posledn�ho p�idan�ho objektu je: <br>" + F->d.v.OBJEKTY->predchozi->pohon->name);
}
//---------------------------------------------------------------------------

