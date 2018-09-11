//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "poznamky.h"
#include "unit1.h"
#include "parametry.h"
#include "TmGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma link "rHTMLLabel"
#pragma resource "*.dfm"
TForm_poznamky *Form_poznamky;
//---------------------------------------------------------------------------
__fastcall TForm_poznamky::TForm_poznamky(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláøe
	Form_poznamky->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK a Storno
	Form1->m.designButton(scGPButton_OK,Form_poznamky,1,2);
	Form1->m.designButton(scGPButton_storno,Form_poznamky,2,2);

	//definice barev
	clBACKGROUND=(TColor)RGB(250,250,250);
	clLOCKED	  =(TColor)RGB(128,128,128);
	rHTMLLabel_InfoText->Font->Color=(TColor)RGB(43,87,154);
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::scGPButton_stornoClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 13) // ENTER
	{
			Form_poznamky->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlaèítko
			Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
	}
	if (Key == 27) // ESC
	{
			Form_poznamky->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
			Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
	}
}
//---------------------------------------------------------------------------
//vykreslí orámování
void __fastcall TForm_poznamky::FormPaint(TObject *Sender)
{
	Form1->m.frameForm(Form_poznamky,clWebOrange,1);
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)mGrid->Show();//vykreslí tabulku
}
//---------------------------------------------------------------------------
//pøi zobrazení formuláøe
void __fastcall TForm_poznamky::FormShow(TObject *Sender)
{
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)//pro S&G
	{
		////////tabulka
		mGrid=new TmGrid(this);//vždy nutno jako první
		mGrid->Tag=5;//ID tabulky
		mGrid->Left=10;mGrid->Top=scGPPanel_hlavicka->Height+10;//vhodné jako druhé (popø. by bylo nutné pøekreslovat)
		mGrid->AntiAliasing_text=true;//bez toho dìlá bílý blok, pozadí - posunuto, jedna se o bug v mGridu
		mGrid->Create(7,5);//vhodné jako tøetí
		mGrid->Columns[0].Width=10;
		short R=0;//èíslo øádku

		////pøevody jednotek
		AnsiString CTunitT="s";if(Form_parametry->CTunit)CTunitT="min";
		AnsiString DDunitT="m";if(Form_parametry->DDunit)DDunitT="mm";

		////definice bunìk - hlavièka total
		//pohon vlastní
		mGrid->Cells[1][R].Text="Pohon1";
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clLOCKED;mGrid->Cells[1][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		//pohon následující
		if(F->pom->dalsi->pohon!=NULL)mGrid->Cells[5][R].Text=F->pom->dalsi->pohon->name;else mGrid->Cells[5][R].Text="nepøiøazen";
		mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clLOCKED;mGrid->Cells[5][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		//celkem
		mGrid->Cells[6][R].Text="CELKEM";mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma

		////definice bunìk - hlavièka ÈAS
		mGrid->Cells[0][++R].Text="ÈAS";
		mGrid->Cells[0][R].Background->Color=clBACKGROUND;mGrid->Cells[0][R].Font->Color=clLOCKED;mGrid->Cells[0][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		mGrid->Cells[1][R].Text="MT1 ["+CTunitT+"]";// - èas pøesunu 1";
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;
		mGrid->Cells[2][R].Text="PT ["+CTunitT+"]";// - procesní èas";
		mGrid->Cells[2][R].Background->Color=clBACKGROUND;mGrid->Cells[2][R].Font->Color=clLOCKED;
		mGrid->Cells[3][R].Text="max. WT1 ["+CTunitT+"]";// - èas èekání na nejbližší palec 2";
		mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clLOCKED;
		mGrid->Cells[4][R].Text="MT2 ["+CTunitT+"]";// - èas pøesunu 2";
		mGrid->Cells[4][R].Background->Color=clBACKGROUND;
		mGrid->Cells[5][R].Text="max. WT2 ["+CTunitT+"]";// - èas èekání na nejbližší palec 2";
		mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clLOCKED;
		mGrid->Cells[6][R].Text="CT ["+CTunitT+"]";// - Celkový technologický èas";
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;
		mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		mGrid->Cells[6][++R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma

		////výpoèetní èást - ÈAS
		double MT1=0;
		if(F->pom->dalsi->pohon!=NULL)MT1=Form_parametry->scGPNumericEdit_delka_dopravniku->Value/(1.0+Form_parametry->DDunit*999.0)/F->pom->dalsi->pohon->aRD;
		MT1*=(1.0+Form_parametry->CTunit*59.0);
		double WT2=0;
		if(F->pom->dalsi->pohon!=NULL)WT2=F->m.cekani_na_palec(0,F->pom->dalsi->pohon->roztec,F->pom->dalsi->pohon->aRD,3);
		WT2*=(1.0+Form_parametry->CTunit*59.0);

		////definice bunìk - parametry - ÈAS
		//MT1
		mGrid->Cells[1][R].Type=mGrid->EDIT; mGrid->Cells[1][R].Text=F->m.round2double(MT1,2,"..");
		//PT
		double PT=Form_parametry->scGPNumericEdit_CT->Value-MT1-WT2;
		mGrid->Cells[2][R].Text=F->m.round2double(PT,2,"..");
		mGrid->Cells[2][R].Background->Color=clBACKGROUND;
		if(PT>0)mGrid->Cells[2][R].Font->Color=clLOCKED;else mGrid->Cells[2][R].Font->Color=clRed;//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, vìtšinou z dùvodu nestíhání pøejezdu) a zobrazí se èervenì
		//WT1
		mGrid->Cells[3][R].Background->Color=clBACKGROUND; mGrid->Cells[3][R].Font->Color=clLOCKED;mGrid->Cells[3][R].Text="0";
		//MT2
		mGrid->Cells[4][R].Type=mGrid->EDIT; mGrid->Cells[4][R].Text="0";
		//WT2
		mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clLOCKED; mGrid->Cells[5][R].Text=F->m.round2double(WT2,2,"..");
		//CT
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;mGrid->Cells[6][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_CT->Value,2,"..");

		////definice bunìk - hlavièka - DÉLKA
		mGrid->Cells[0][++R].Text="DÉLKA";
		mGrid->Cells[0][R].Background->Color=clBACKGROUND;mGrid->Cells[0][R].Font->Color=clLOCKED;mGrid->Cells[0][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		mGrid->Cells[1][R].Text="Pøejezd 1 ["+DDunitT+"]";
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clLOCKED;
		mGrid->Cells[3][R].Text="Pøejezd 2 ["+DDunitT+"]";
		mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clLOCKED;
		mGrid->Cells[6][R].Text="Objekt ["+DDunitT+"]";
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;
		mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma
		mGrid->Cells[6][++R].Font->Style=TFontStyles()<< fsBold;//zapnutí tuèného písma

		////definice bunìk - parametry - DÉLKA
		//Pøejezd 1
		mGrid->Cells[1][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_delka_dopravniku->Value,2,"..");
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clLOCKED;
		//Pøejezd 2
		mGrid->Cells[3][R].Text="0";
		mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clLOCKED;
		//Celkem
		mGrid->Cells[6][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_delka_dopravniku->Value,2,"..");
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;

		//vymazání nepoužívaných bunìk
		mGrid->Cells[0][0].Background->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].TopBorder->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].LeftBorder->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].BottomBorder->Width=2;mGrid->Cells[0][0].RightBorder->Width=2;

		//Slouèení bunìk
		mGrid->MergeCells(1,0,4,0);
		mGrid->MergeCells(0,1,0,2);
		mGrid->MergeCells(0,3,0,4);
		mGrid->MergeCells(1,R-1,2,R-1);mGrid->MergeCells(1,R,2,R);
		mGrid->MergeCells(3,R-1,4,R-1);mGrid->MergeCells(3,R,4,R);
		mGrid->MergeCells(5,R-1,5,R);

		////////design - musí být až za tabulkou kvùli pozici
		scLabel_titulek->Caption="Rozbor objektu";
		mGrid->Show();//kvùli pøepoèítání a získání výšky výsledné tabulky
		scGPMemo->Top=mGrid->Top+mGrid->Height+10+rHTMLLabel_InfoText->Height;
		scGPMemo->Height=scGPButton_OK->Top-10-scGPMemo->Top;
		rHTMLLabel_InfoText->Top=scGPMemo->Top-rHTMLLabel_InfoText->Height;
	}
	else//pro ostatní režimy
	{
		scLabel_titulek->Caption=rHTMLLabel_InfoText->Caption;
		rHTMLLabel_InfoText->Visible=false;
		scGPMemo->Top=48;
		scGPMemo->Height=233;
	}
}
//---------------------------------------------------------------------------
void TForm_poznamky::OnClick(long Tag,unsigned long Col,unsigned long Row)
{

}
void TForm_poznamky::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
void TForm_poznamky::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_poznamky::FormClose(TObject *Sender, TCloseAction &Action)
{
	mGrid->Delete();//pokud chci odstranit a nechci použít na další použití
}
//---------------------------------------------------------------------------

