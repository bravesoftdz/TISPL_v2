// ---------------------------------------------------------------------------

#pragma hdrstop

#include "grafy.h"
#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

void Cgrafy::ShowGrafy(bool stav) {

		if (stav) {
				nastaveni();
				graf2(); // prumerny TT zakazek
				graf6(); // graf kapacit
				// graf1(); //casove stavy zakazek
				// graf3();   //vytizenost
				// graf4();   //pomer voziku
			 //	zpravy();
			 //	nastav_zpravy();

		}
		Form1->Chart1->Visible = false;
		Form1->Chart2->Visible = stav;
		Form1->Chart3->Visible = false;
		Form1->Chart4->Visible = false;
		Form1->Chart6->Visible = stav;

		// Form1->Label_wip->Visible = stav;
		// Form1->Memo1->Visible = stav;
		// Form1->scExPanel_log_header->Visible=stav;
}

/////////////////////////////////////////////////////
void Cgrafy::nastaveni() {
		// WIP label
		Form1->Label_wip->Font->Color = (TColor)RGB(100, 100, 100);
		// Form1->Label_wip->Visible = true;
		Form1->Label_wip->Font->Size = 20;
		Form1->Label_wip->Top = Form1->Chart1->Top - 35;
		Form1->Label_wip->Left = 20;
		Form1->Label_wip->Caption = "WIP: " + AnsiString(Form1->d.v.WIP());
		// Form1->scHTMLLabel_log_vypis->Caption="Linka v po��dku";


		// globalni nastaveni grafu
		Form1->Chart1->Legend->Visible = false;
		Form1->Chart1->Color = clWhite;
		Form1->Chart1->Title->Font->Size = 16;
		Form1->Chart1->Title->Font->Name = "Arial";
		Form1->Chart1->Title->Font->Color = (TColor)RGB(100, 100, 100);
		// RGB(84,83,82);
		Form1->Chart1->LeftAxis->Title->Font->Size = 14;
		Form1->Chart1->BottomAxis->Title->Font->Size = 14;
		Form1->Chart1->LeftAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart1->LeftAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart1->BottomAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart1->BottomAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart1->LeftAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart1->LeftAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart1->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart1->LeftAxis->Items->Format->Font->Size = 12;
		Form1->Chart1->BottomAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart1->LeftAxis->Items->Format->Font->Color =
				(TColor)RGB(100, 100, 100); // RGB(84,83,82);
		Form1->Chart1->BottomAxis->Items->Format->Font->Size = 12;
		Form1->Chart1->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->LeftAxis->Items->Format->Font->Color;
		Form1->Chart1->Border->Visible = false;
		// u gant grafu nejde border nastavit v kodu, musi se v editoru
		Form1->Chart1->LeftAxis->Axis->Width = 1;
		Form1->Chart1->BottomAxis->Axis->Width = 1;
		Form1->Chart1->Walls->Visible = false;
		Form1->Chart1->BottomAxis->Grid->Visible = false;
		// Form1->Series2->Marks->Font->Size=5;
		Form1->Series1->Marks->Arrow->Visible = false;
		Form1->Series2->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;
		Form1->Series3->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;
		Form1->Series5->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;
		Form1->Series6->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;
		Form1->Series7->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;
		Form1->Series9->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;
		Form1->Series10->Marks->Arrow->Visible =
				Form1->Series1->Marks->Arrow->Visible;

		Form1->Series1->Marks->BackColor = clWhite;
		Form1->Series2->Marks->BackColor = Form1->Series1->Marks->BackColor;
		Form1->Series3->Marks->BackColor = Form1->Series1->Marks->BackColor;
		Form1->Series5->Marks->BackColor = Form1->Series1->Marks->BackColor;
		Form1->Series6->Marks->BackColor = Form1->Series1->Marks->BackColor;
		Form1->Series7->Marks->BackColor = Form1->Series1->Marks->BackColor;
		Form1->Series9->Marks->BackColor = Form1->Series1->Marks->BackColor;
		Form1->Series10->Marks->BackColor = Form1->Series1->Marks->BackColor;

		Form1->Series1->Marks->Font->Size = 10;
		Form1->Series2->Marks->Font->Size = Form1->Series1->Marks->Font->Size;
		Form1->Series3->Marks->Font->Size = Form1->Series1->Marks->Font->Size;
		Form1->Series5->Marks->Font->Size = Form1->Series1->Marks->Font->Size;
		Form1->Series6->Marks->Font->Size = Form1->Series1->Marks->Font->Size;
		Form1->Series7->Marks->Font->Size = Form1->Series1->Marks->Font->Size;
		Form1->Series9->Marks->Font->Size = Form1->Series1->Marks->Font->Size;
		Form1->Series10->Marks->Font->Size = Form1->Series1->Marks->Font->Size;

		Form1->Series1->Marks->Font->Color = (TColor)RGB(100, 100, 100);
		Form1->Series2->Marks->Font->Color = Form1->Series1->Marks->Font->Color;
		Form1->Series3->Marks->Font->Color = Form1->Series1->Marks->Font->Color;
		Form1->Series5->Marks->Font->Color = Form1->Series1->Marks->Font->Color;
		Form1->Series5->Marks->Font->Style = TFontStyles() << fsBold;
		Form1->Series6->Marks->Font->Color = Form1->Series1->Marks->Font->Color;
		Form1->Series7->Marks->Font->Color = clWhite;
		Form1->Series7->Marks->Font->Style = TFontStyles() << fsBold;
		Form1->Series9->Marks->Font->Color = Form1->Series1->Marks->Font->Color;
		Form1->Series10->Marks->Font->Color = Form1->Series1->Marks->Font->Color;

		///////////////////////////
		Form1->Chart2->LeftAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart2->BottomAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart2->LeftAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart2->BottomAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart2->LeftAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart2->BottomAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart2->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->LeftAxis->Items->Format->Font->Color;
		Form1->Chart2->Legend->Visible = Form1->Chart1->Legend->Visible;
		Form1->Chart2->Color = Form1->Chart1->Color;
		Form1->Chart2->Title->Font->Name = Form1->Chart1->Title->Font->Name;
		Form1->Chart2->Title->Font->Color = Form1->Chart1->Title->Font->Color;
		Form1->Chart2->Title->Font->Size = Form1->Chart1->Title->Font->Size;
		Form1->Chart2->LeftAxis->Title->Font->Size =
				Form1->Chart1->LeftAxis->Title->Font->Size;
		Form1->Chart2->BottomAxis->Title->Font->Size =
				Form1->Chart1->BottomAxis->Title->Font->Size;
		Form1->Chart2->Title->Color = Form1->Chart1->Title->Color;
		Form1->Chart2->LeftAxis->Items->Format->Font->Size =
				Form1->Chart1->LeftAxis->Items->Format->Font->Size;
		Form1->Chart2->BottomAxis->Items->Format->Font->Size =
				Form1->Chart1->BottomAxis->Items->Format->Font->Size;
		Form1->Chart2->Border->Visible = Form1->Chart1->Border->Visible;
		Form1->Chart2->LeftAxis->Axis->Width = Form1->Chart1->LeftAxis->Axis->Width;
		Form1->Chart2->BottomAxis->Axis->Width =
				Form1->Chart1->BottomAxis->Axis->Width;
		Form1->Chart2->Walls->Visible = Form1->Chart1->Walls->Visible;
		Form1->Chart2->BottomAxis->Grid->Visible =
				Form1->Chart1->BottomAxis->Grid->Visible;
		Form1->Chart2->LeftAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart2->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;

		Form1->Chart3->LeftAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart3->BottomAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart3->LeftAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart3->LeftAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart3->BottomAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart3->BottomAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart3->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->LeftAxis->Items->Format->Font->Color;
		Form1->Chart3->Legend->Visible = Form1->Chart1->Legend->Visible;
		Form1->Chart3->Color = Form1->Chart1->Color;
		Form1->Chart3->Title->Font->Name = Form1->Chart1->Title->Font->Name;
		Form1->Chart3->Title->Font->Color = Form1->Chart1->Title->Font->Color;
		Form1->Chart3->Title->Font->Size = Form1->Chart1->Title->Font->Size;
		Form1->Chart3->LeftAxis->Title->Font->Size =
				Form1->Chart1->LeftAxis->Title->Font->Size;
		Form1->Chart3->BottomAxis->Title->Font->Size =
				Form1->Chart1->BottomAxis->Title->Font->Size;
		Form1->Chart3->Title->Color = Form1->Chart1->Title->Color;
		Form1->Chart3->LeftAxis->Items->Format->Font->Size =
				Form1->Chart1->LeftAxis->Items->Format->Font->Size;
		Form1->Chart3->BottomAxis->Items->Format->Font->Size =
				Form1->Chart1->BottomAxis->Items->Format->Font->Size;
		Form1->Chart3->Border->Visible = Form1->Chart1->Border->Visible;
		Form1->Chart3->LeftAxis->Axis->Width = Form1->Chart1->LeftAxis->Axis->Width;
		Form1->Chart3->BottomAxis->Axis->Width =
				Form1->Chart1->BottomAxis->Axis->Width;
		Form1->Chart3->Walls->Visible = Form1->Chart1->Walls->Visible;
		Form1->Chart3->BottomAxis->Grid->Visible =
				Form1->Chart1->BottomAxis->Grid->Visible;
		Form1->Chart3->LeftAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart3->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;

		Form1->Chart4->LeftAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart4->BottomAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart4->LeftAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart4->LeftAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart4->BottomAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart4->BottomAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart4->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->LeftAxis->Items->Format->Font->Color;
		Form1->Chart4->Legend->Visible = Form1->Chart1->Legend->Visible;
		Form1->Chart4->Color = Form1->Chart1->Color;
		Form1->Chart4->Title->Font->Name = Form1->Chart1->Title->Font->Name;
		Form1->Chart4->Title->Font->Color = Form1->Chart1->Title->Font->Color;
		Form1->Chart4->Title->Font->Size = Form1->Chart1->Title->Font->Size;
		Form1->Chart4->LeftAxis->Title->Font->Size =
				Form1->Chart1->LeftAxis->Title->Font->Size;
		Form1->Chart4->BottomAxis->Title->Font->Size =
				Form1->Chart1->BottomAxis->Title->Font->Size;
		Form1->Chart4->Title->Color = Form1->Chart1->Title->Color;
		Form1->Chart4->LeftAxis->Items->Format->Font->Size =
				Form1->Chart1->LeftAxis->Items->Format->Font->Size;
		Form1->Chart4->BottomAxis->Items->Format->Font->Size =
				Form1->Chart1->BottomAxis->Items->Format->Font->Size;
		Form1->Chart4->LeftAxis->Axis->Width = Form1->Chart1->LeftAxis->Axis->Width;
		Form1->Chart4->BottomAxis->Axis->Width =
				Form1->Chart1->BottomAxis->Axis->Width;
		Form1->Chart4->Walls->Visible = Form1->Chart1->Walls->Visible;
		Form1->Chart4->BottomAxis->Grid->Visible =
				Form1->Chart1->BottomAxis->Grid->Visible;
		Form1->Chart4->LeftAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart4->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;

		Form1->Chart6->LeftAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart6->BottomAxis->Title->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart6->LeftAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart6->LeftAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart6->BottomAxis->Title->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart6->BottomAxis->Items->Format->Font->Name =
				Form1->Chart1->Title->Font->Name;
		Form1->Chart6->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->LeftAxis->Items->Format->Font->Color;
		Form1->Chart6->Legend->Visible = Form1->Chart1->Legend->Visible;
		Form1->Chart6->Color = Form1->Chart1->Color;
		Form1->Chart6->Title->Font->Name = Form1->Chart1->Title->Font->Name;
		Form1->Chart6->Title->Font->Color = Form1->Chart1->Title->Font->Color;
		Form1->Chart6->Title->Font->Size = Form1->Chart1->Title->Font->Size;
		Form1->Chart6->LeftAxis->Title->Font->Size =
				Form1->Chart1->LeftAxis->Title->Font->Size;
		Form1->Chart6->BottomAxis->Title->Font->Size =
				Form1->Chart1->BottomAxis->Title->Font->Size;
		Form1->Chart6->Title->Color = Form1->Chart1->Title->Color;
		Form1->Chart6->LeftAxis->Items->Format->Font->Size =
				Form1->Chart1->LeftAxis->Items->Format->Font->Size;
		Form1->Chart6->BottomAxis->Items->Format->Font->Size =
				Form1->Chart1->BottomAxis->Items->Format->Font->Size;
		Form1->Chart6->Border->Visible = Form1->Chart1->Border->Visible;
		Form1->Chart6->LeftAxis->Axis->Width = Form1->Chart1->LeftAxis->Axis->Width;
		Form1->Chart6->BottomAxis->Axis->Width =
				Form1->Chart1->BottomAxis->Axis->Width;
		Form1->Chart6->Walls->Visible = Form1->Chart1->Walls->Visible;
		Form1->Chart6->BottomAxis->Grid->Visible =
				Form1->Chart1->BottomAxis->Grid->Visible;
		Form1->Chart6->LeftAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
		Form1->Chart6->BottomAxis->Items->Format->Font->Color =
				Form1->Chart1->Title->Font->Color;
}

//// ---------------------------------------------------------------------------
void Cgrafy::graf2() {

		// defaultn� vykreslen� grafu (checkbox re�� ud�lost onclick)

		// if (Form1->scHTMLLabel_log_vypis->Visible) {
		// Form1->Chart2->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height - Form1->Chart2->Height;
		// }
		// else {
		// Form1->Chart2->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height - Form1->Chart2->Height;
		// }

    Form1->Chart2->Height = Form1->ClientHeight / 3;
		Form1->Chart2->Top = Form1->ClientHeight -
				Form1->scGPPanel_statusbar->Height - Form1->Chart2->Height;
		Form1->Chart2->Title->Caption = "Pr�m�rn� TT zak�zek";
		Form1->Chart2->LeftAxis->Title->Caption = "min";
		Form1->Chart2->BottomAxis->Title->Caption = "zak�zka";

		Form1->Chart2->Left = 0; // Form1->Chart1->Width;
		Form1->Chart2->Width = Form1->ClientWidth / 2; // 5 * 1, 5;


		Form1->Series2->Clear();
		Form1->Series3->Clear();
		// Form1->Series4->Clear(); // sou�et technolog.�as� - cel� zak�zka v�roby

		Form1->Series3->Marks->Visible = true;
		// tohle se nezobrazi, nevim Form1->Series3->Marks->Text="text";
		Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;

		// if (Form1->d.PROZATIM) //nefunguje p�i prvn�m pr�chodu a� p�i druh�m...
		// {

		// Form1->Memo1->Lines->Clear();
		// Form1->Memo1->Font->Color=clRed;

		Form1->Series2->Clear();
		Form1->Series3->Clear();

		while (ukaz != NULL) {
				// Form1->scHTMLLabel_log_vypis->Caption=ukaz->n;
				Form1->Series2->Add(ukaz->TT, ukaz->n, ukaz->barva);

				if (Form1->d.v.vrat_AVG_TT_zakazky(ukaz) != ukaz->TT) {

						Form1->Series3->Add(Form1->d.v.vrat_AVG_TT_zakazky(ukaz)*60 -
								ukaz->TT, ukaz->n, Form1->m.clIntensive(ukaz->barva, 80));
						// Form1->Memo1->Lines->Add(AnsiString("Chyba - Uskute�niteln� TT: ") + Form1->d.v.vrat_AVG_TT_zakazky(ukaz) + AnsiString(" Po�adovan� TT: ") + ukaz->TT + AnsiString(" = Zak�zka ��slo: ") + ukaz->n);
						AnsiString tt_uskutec = Form1->d.v.vrat_AVG_TT_zakazky(ukaz);
						AnsiString tt_pozad = ukaz->TT;
						AnsiString zakazka_n = ukaz->n;

						// Form1->scHTMLLabel_log_vypis->Caption="Chyba u zak�zky �."+zakazka_n+" - Uskute�niteln� TT: <b>"+tt_uskutec+"</b>. Po�adovan� TT: <b>"+tt_pozad+"</b>";
				}

				ukaz = ukaz->dalsi;
		}

		// }

}

void Cgrafy::graf6() { // Kapacity

		if (Form1->scHTMLLabel_log_vypis->Visible) {
				Form1->Chart6->Top = Form1->ClientHeight -
						Form1->scGPPanel_statusbar->Height - Form1->Chart2->Height;
		}
		else {
				Form1->Chart6->Top = Form1->ClientHeight -
						Form1->scGPPanel_statusbar->Height - Form1->Chart6->Height;
		}

		Form1->Chart6->Left = Form1->Chart2->Width;
		Form1->Chart6->Width = Form1->ClientWidth / 2; // 5 * 1, 5; ;
		Form1->Chart6->Height = Form1->ClientHeight / 3 * 1;

		Form1->Chart6->AxisVisible = true;

		Form1->Chart6->Title->Caption = "Nastaven� vs. doporu�en� kapacity";
		Form1->Chart6->LeftAxis->Title->Caption = "kapacity";
		Form1->Chart6->BottomAxis->Title->Caption = "objekty";

		// Form1->Memo1->Font->Color=clBlack;

		Form1->Series9->Clear();
		Form1->Series10->Clear();

		Cvektory::TObjekt *ukaz = Form1->d.v.OBJEKTY->dalsi;
		// Cvektory::TVozik *ukaz1 = Form1->d.v.VOZIKY->dalsi;
		while (ukaz != NULL) {

				if (ukaz->kapacita != ukaz->kapacita_dop) {
						Form1->Series9->Add(ukaz->kapacita, ukaz->short_name,
								(TColor) RGB(0, 128, 255));

						Form1->Series10->Add(ukaz->kapacita_dop, "",
								Form1->m.clIntensive((TColor)RGB(0, 128, 255), 80));
				}
				if (ukaz->kapacita != ukaz->kapacita_dop) {

						// Form1->Memo1->Lines->Add(AnsiString("Varov�n� - nastaven� kapacita: ") + ukaz->kapacita_objektu + AnsiString(". Doporu�en� kapacita: ") + ukaz->dop_kapacita_objektu + AnsiString(" - pro objekt: ") + ukaz->short_name);
						// zpravy+="Objekt:"+ukaz->short_name+"- nastaven� kapacita:"+ukaz->kapacita+" - doporu�en� kapacita"+ ukaz->dop_kapacita_objektu +;
				}

				ukaz = ukaz->dalsi;
		}

}

// ---------------------------------------------------------------------------

//// ---------------------------------------------------------------------------
void Cgrafy::graf3() {

		// // defaultn� vykreslen� grafu (checkbox re�� ud�lost onclick)
		//
		// if (Form1->Memo1->Visible) {
		// Form1->Chart3->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
		// Form1->Memo1->Height - Form1->Chart1->Height;
		// }
		// else {
		// Form1->Chart3->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
		// Form1->Chart1->Height;
		// }
		//
		// Form1->Chart3->LeftAxis->Title->Caption = "min";
		// Form1->Chart3->BottomAxis->Title->Caption = "zak�zka";
		// Form1->Chart3->Title->Caption = "Vyt�enost zak�zky";
		//
		// Form1->Chart3->Left = Form1->Chart1->Width * 3;
		// Form1->Chart3->Width = Form1->ClientWidth / 5 * 1, 5;
		// Form1->Chart3->Height = Form1->ClientHeight / 3 * 1, 5;
		//
		// Form1->Series5->Clear();
		// Form1->Series6->Clear();
		//
		//
		//
		//
		// Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
		// while (ukaz != NULL) {
		//
		// Form1->Series5->Add(Form1->d.v.vrat_AVGsumPT_zakazky(ukaz), ukaz->n,ukaz->barva);
		//
		// if(Form1->d.v.vrat_AVGsumWT_zakazky(ukaz)>0)
		// {
		// Form1->Series5->Marks->Font->Color=clWhite;
		// Form1->Series6->Add(Form1->d.v.vrat_AVGsumWT_zakazky(ukaz), ukaz->n,Form1->m.clIntensive(ukaz->barva,80));
		// }
		// ukaz = ukaz->dalsi;
		// }

}

//// ---------------------------------------------------------------------------
//
void Cgrafy::graf4() {

		// // defaultn� vykreslen� grafu (checkbox re�� ud�lost onclick)
		//
		// if (Form1->Memo1->Visible) {
		// Form1->Chart4->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
		// Form1->Memo1->Height - Form1->Chart1->Height;
		// }
		// else {
		// Form1->Chart4->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
		// Form1->Chart1->Height;
		// }
		// Form1->Chart4->Title->Caption = "Pom�r voz�k�";
		// Form1->Chart4->LeftAxis->Title->Caption = "min";
		// Form1->Chart4->BottomAxis->Title->Caption = "zak�zka";
		//
		// Form1->Chart4->Left = Form1->Chart1->Width * 4;
		// Form1->Chart4->Width = Form1->ClientWidth / 5 * 1, 5;
		// Form1->Chart4->Height = Form1->ClientHeight / 3 * 1, 5;
		//
		// Form1->Series7->Clear();
		//
		// Form1->Chart4->Border->Color=clGreen;
		//
		// // sou�et technolog. �as� vs sou�et prostoj� dle zak�zek
		// Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
		// // Cvektory::TVozik *ukaz1 = Form1->d.v.VOZIKY->dalsi;
		// while (ukaz != NULL) {
		//
		// Form1->Series7->Add(Form1->d.v.vrat_pocet_voziku_zakazky(ukaz), ukaz->n,
		// ukaz->barva);
		//
		// ukaz = ukaz->dalsi;
//}

}

//// ---------------------------------------------------------------------------
//
void Cgrafy::graf5() {

/* if (Form1->Memo1->Visible) {
 Form1->Chart5->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
 Form1->Memo1->Height - Form1->Chart1->Height;
 }
 else {
 Form1->Chart5->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
 Form1->Chart1->Height;
 }

 Form1->Chart5->Left = Form1->Chart1->Width + Form1->Chart2->Width +
 Form1->Chart3->Width + Form1->Chart4->Width;
 Form1->Chart5->Width = Form1->ClientWidth / 5 * 1, 5; ;
 Form1->Chart5->Height = Form1->ClientHeight / 3 * 1, 5;

 Form1->Chart5->AxisVisible = false; */

}

//// ---------------------------------------------------------------------------
//

void Cgrafy::graf1() {

//
// //	Form1->Chart1->AddSeries(new TBarSeries(this));
// //  Form1->Series1[]
//
// Form1->Chart1->Left =Form1->Chart2->Width +  Form1->Chart6->Width;//0;
// Form1->Chart1->Width = Form1->ClientWidth / 5 * 1, 5;
// Form1->Chart1->Height = Form1->ClientHeight / 3 * 1, 5;
//
// Form1->Series1->Clear();
// Form1->Chart1->LeftAxis->Title->Caption = "zak�zka";
// Form1->Chart1->BottomAxis->Title->Caption = "min";
// Form1->Chart1->Title->Caption = "�asov� stavy zak�zek";
//
// if (Form1->Memo1->Visible) {
// Form1->Chart1->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
// Form1->Memo1->Height - Form1->Chart1->Height;
// }
// else {
// Form1->Chart1->Top = Form1->ClientHeight - Form1->scGPPanel_statusbar->Height -
// Form1->Chart1->Height;
// }
//
// Cvektory::TZakazka *ukaz = Form1->d.v.ZAKAZKY->dalsi;
// // ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
// while (ukaz != NULL)
// {
// TPointD z = Form1->d.v.vrat_zacatek_a_konec_zakazky(ukaz);
////if(ukaz->n==1) {
////
////
//// int a=0;
////		Form1->Series1->AddGanttColor(Form1->m.round2double(0,2), Form1->m.round2double(z.y,2), ukaz->n,
////		AnsiString(Form1->m.round2double(0,2)) + "-" + AnsiString(Form1->m.round2double(z.y,2)),ukaz->barva);
////		 }
////		 else
////			Form1->Series1->AddGanttColor(Form1->m.round2double(0,2), Form1->m.round2double(z.y,2), ukaz->n,
////		AnsiString(Form1->m.round2double(0,2)) + "-" + AnsiString(Form1->m.round2double(z.y,2)),ukaz->barva);
// //	Form1->Chart1->Hover->Frame->Color=Form1->m.clIntensive(ukaz->barva,80);
// /*	if(ukaz->n==1)
// {
// Form1->Series1->AddGanttColor(0, Form1->m.round2double(z.y,2), ukaz->n,
// AnsiString(0) + " - " + AnsiString(Form1->m.round2double(z.y,2)),ukaz->barva);
// }
// */
// Form1->Series1->AddGanttColor(Form1->m.round2double(z.x,2), Form1->m.round2double(z.y,2), ukaz->n,
// AnsiString(Form1->m.round2double(z.x,2)) + "-" + AnsiString(Form1->m.round2double(z.y,2)),ukaz->barva);
//
// ukaz = ukaz->dalsi;
//
// }

}
/////////////////////////////////////////////////////

 void Cgrafy::nastav_zpravy(){

	 //	if (!Form1->d.JIZPOCITANO) { // pro prvn� zobrazen� nastav�m default rozm�ry - podm�nka realizov�na nyn� ve vykresli


				Form1->scExPanel_log_header->Left = Form1->zalozka_schema->Left-100;
				Form1->scExPanel_log_header->Top = Form1->zalozka_schema->Height + 10;
				Form1->scExPanel_log_header->Height = 300;
	 //	}


 }

/////////////////////////////////////////////

void Cgrafy::zpravy() {

		Cvektory::TZakazka *zakazka = Form1->d.v.ZAKAZKY->dalsi;
		takt_splnen=true;
		UnicodeString zpravy;
		UnicodeString zprava_dlouha_probox;
		zpravy += "<b>Po�et zak�zek:";
		zpravy += Form1->d.v.ZAKAZKY->predchozi->n;
		zpravy += "</b></br>";

	while (zakazka != NULL) {

		UnicodeString tt = zakazka->TT;
		UnicodeString id = zakazka->id;
		UnicodeString name = zakazka->name;
		UnicodeString stav = "spln�n";
		UnicodeString rozdil_tt = 0;

		if (Form1->d.v.vrat_AVG_TT_zakazky(zakazka)*60 != zakazka->TT) {
				stav = "nespln�n";
				takt_splnen=false;
				rozdil_tt = Form1->d.v.vrat_AVG_TT_zakazky(zakazka) * 60;

		}

		zpravy += "Zak�zka �." + id + "- " + name + ", po�adovan� takt " + tt + " sekund <b>" + stav + "</b>";
		zprava_dlouha_probox+="Zak�zka �." + id + "- " + name + ", po�adovan� takt " + tt + " sekund <b>" + stav + "</b>";

		if(stav=="spln�n"){
		zpravy +="</br>";
		takt_splnen=true;
		zprava_dlouha_probox+="</br>";

		}

		if (rozdil_tt > 0) {
				zpravy +=", vypo��tan� pr�m�rn� takt " + rozdil_tt + " s</br>";
				zprava_dlouha_probox+=", vypo��tan� pr�m�rn� takt " + rozdil_tt + " s</br>";
		}

	 //	int delka;
	// delka=	zprava_dlouha_probox.Length();   //vrati 111
	// ShowMessage(Form1->Canvas->TextWidth(zprava_dlouha_probox)); //vrati 575

	//Form1->Canvas->Font=Form1->scHTMLLabel_log_vypis->Font;
	//Form1->Canvas->Font->Size=Form1->scHTMLLabel_log_vypis->Font->Size;
	//ShowMessage(Form1->Canvas->TextWidth(zprava_dlouha_probox));

	 //ShowMessage(delka);

		zakazka = zakazka->dalsi;
}

	if(takt_splnen){ sirka_boxu=450;} else sirka_boxu=715;
			Form1->scExPanel_log_header->Width = sirka_boxu;//sirka_boxu;


		zpravy += "<br><b>Zobrazen� rozd�ln�ch kapacit:</b></br>";
		Cvektory::TObjekt *ukaz = Form1->d.v.OBJEKTY->dalsi;

bool rozdilne_kapacity=false;
while (ukaz != NULL) {

		if (ukaz->kapacita != ukaz->kapacita_dop) {
				// Form1->Memo1->Lines->Add(AnsiString("Varov�n� - nastaven� kapacita: ") + ukaz->kapacita_objektu + AnsiString(". Doporu�en� kapacita: ") + ukaz->dop_kapacita_objektu + AnsiString(" - pro objekt: ") + ukaz->short_name);
				zpravy += ukaz->short_name + "- nastaven� kapacita: " + ukaz->kapacita +
						" - doporu�en� kapacita: " + ukaz->kapacita_dop + "</br>";

		rozdilne_kapacity=true;
		}
 ukaz = ukaz->dalsi;
}

	if(!rozdilne_kapacity) zpravy+="<i>Nejsou evidov�ny ��dn� rozd�ln� kapacity.</i>";

Form1->scHTMLLabel_log_vypis->Caption = zpravy;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
