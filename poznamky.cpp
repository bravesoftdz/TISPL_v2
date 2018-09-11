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
	//nastavení barvy formuláře
	Form_poznamky->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlačítek OK a Storno
	Form1->m.designButton(scGPButton_OK,Form_poznamky,1,2);
	Form1->m.designButton(scGPButton_storno,Form_poznamky,2,2);

	//definice barev
	clBACKGROUND=(TColor)RGB(250,250,250);
	clLOCKED	  =(TColor)RGB(128,128,128);
	clLOCKEDhead=(TColor)RGB(43,87,154);
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
			Form_poznamky->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlačítko
			Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
	}
	if (Key == 27) // ESC
	{
			Form_poznamky->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlačítko
			Form_poznamky->VisibleChanging();// skryje form, stejné jako visible=false
	}
}
//---------------------------------------------------------------------------
//vykreslí orámování
void __fastcall TForm_poznamky::FormPaint(TObject *Sender)
{
	Form1->m.frameForm(Form_poznamky,clWebOrange,1);
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)
	{
		mGrid->Show();//vykreslí tabulku

		//workaround - zrušení orámování okolo nepoužitých vnějších buněk
		Canvas->Pen->Width=2;
		Canvas->Pen->Color=(TColor)RGB(240,240,240);

		Canvas->MoveTo(mGrid->Left,mGrid->Top+mGrid->Rows[0].Height-2);
		Canvas->LineTo(mGrid->Left,mGrid->Top);
		Canvas->LineTo(mGrid->Left+mGrid->Columns[0].Width-2,mGrid->Top);

		Canvas->MoveTo(mGrid->Left+mGrid->Columns[5].Left,mGrid->Top+mGrid->Height);
		Canvas->LineTo(mGrid->Left+mGrid->Columns[5].Left+mGrid->Columns[5].Width-2,mGrid->Top+mGrid->Height);
	}
}
//---------------------------------------------------------------------------
//při zobrazení formuláře
void __fastcall TForm_poznamky::FormShow(TObject *Sender)
{
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)//pro S&G
	{
		////////tabulka
		mGrid=new TmGrid(this);//vždy nutno jako první
		mGrid->Tag=5;//ID tabulky
		mGrid->Left=10;mGrid->Top=scGPPanel_hlavicka->Height+10;//vhodné jako druhé (popř. by bylo nutné překreslovat)
		mGrid->AntiAliasing_text=true;//bez toho dělá bílý blok, pozadí - posunuto, jedna se o bug v mGridu
		mGrid->DefaultColWidth*=1.3;
		mGrid->Create(7,5);//vhodné jako třetí

		mGrid->SetColumnAutoFit(0);

		short R=0;//číslo řádku

		////převody jednotek
		AnsiString CTunitT="s";if(Form_parametry->CTunit)CTunitT="min";
		AnsiString DDunitT="m";if(Form_parametry->DDunit)DDunitT="mm";

		////definice buněk - hlavička total
		//pohon vlastní
		mGrid->Cells[1][R].Text="Pohon1";
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clLOCKED;mGrid->Cells[1][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		//pohon následující
		if(F->pom->dalsi->pohon!=NULL)mGrid->Cells[5][R].Text=F->pom->dalsi->pohon->name;else mGrid->Cells[5][R].Text="nepřiřazen";
		mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clLOCKED;mGrid->Cells[5][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		//celkem
		mGrid->Cells[6][R].Text="CELKEM";mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma

		////definice buněk - hlavička ČAS
		mGrid->Cells[0][++R].Text="ČAS";mGrid->Cells[0][R].LeftMargin=4;
		mGrid->Cells[0][R].Background->Color=clBACKGROUND;mGrid->Cells[0][R].Font->Color=clLOCKED;mGrid->Cells[0][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		mGrid->Cells[1][R].Text="MT1 ["+CTunitT+"]";// - čas přesunu 1";
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;
		mGrid->Cells[2][R].Text="PT ["+CTunitT+"]";// - procesní čas";
		mGrid->Cells[2][R].Background->Color=clBACKGROUND;mGrid->Cells[2][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[3][R].Text="max.WT1 ["+CTunitT+"]";// - čas čekání na nejbližší palec 2";
		mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[4][R].Text="MT2 ["+CTunitT+"]";// - čas přesunu 2";
		mGrid->Cells[4][R].Background->Color=clBACKGROUND;
		mGrid->Cells[5][R].Text="max.WT2 ["+CTunitT+"]";// - čas čekání na nejbližší palec 2";
		mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[6][R].Text="CT ["+CTunitT+"]";// - Celkový technologický čas";
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		mGrid->Cells[6][++R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma

		////výpočetní část - ČAS
		double MT1=0;
		if(F->pom->dalsi->pohon!=NULL)MT1=Form_parametry->scGPNumericEdit_delka_dopravniku->Value/(1.0+Form_parametry->DDunit*999.0)/F->pom->dalsi->pohon->aRD;
		MT1*=(1.0+Form_parametry->CTunit*59.0);
		double WT2=0;
		if(F->pom->dalsi->pohon!=NULL)WT2=F->m.cekani_na_palec(0,F->pom->dalsi->pohon->roztec,F->pom->dalsi->pohon->aRD,3);
		WT2*=(1.0+Form_parametry->CTunit*59.0);

		////definice buněk - parametry - ČAS
		//MT1
		mGrid->Cells[1][R].Type=mGrid->EDIT; mGrid->Cells[1][R].Text=F->m.round2double(MT1,3,"..");
		//PT
		double PT=Form_parametry->scGPNumericEdit_CT->Value-MT1-WT2;
		mGrid->Cells[2][R].Text=F->m.round2double(PT,3,"..");
		mGrid->Cells[2][R].Background->Color=clBACKGROUND;
		if(PT>0)mGrid->Cells[2][R].Font->Color=clLOCKED;else mGrid->Cells[2][R].Font->Color=clRed;//pokud vyjde PT záporné, je to problém (nestíhá se samotný technologický výkon, většinou z důvodu nestíhání přejezdu) a zobrazí se červeně
		//WT1
		mGrid->Cells[3][R].Background->Color=clBACKGROUND; mGrid->Cells[3][R].Font->Color=clLOCKED;mGrid->Cells[3][R].Text="0";
		//MT2
		mGrid->Cells[4][R].Type=mGrid->EDIT; mGrid->Cells[4][R].Text="0";
		//WT2
		mGrid->Cells[5][R].Background->Color=clBACKGROUND;mGrid->Cells[5][R].Font->Color=clLOCKED; mGrid->Cells[5][R].Text=F->m.round2double(WT2,3,"..");
		//CT
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;mGrid->Cells[6][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_CT->Value,3,"..");

		////definice buněk - hlavička - DÉLKA
		mGrid->Cells[0][++R].Text="DÉLKA";mGrid->Cells[0][R].LeftMargin=4;
		mGrid->Cells[0][R].Background->Color=clBACKGROUND;mGrid->Cells[0][R].Font->Color=clLOCKED;mGrid->Cells[0][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		mGrid->Cells[1][R].Text="Přejezd 1 ["+DDunitT+"]";
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[3][R].Text="Přejezd 2 ["+DDunitT+"]";
		mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[6][R].Text="Délka ["+DDunitT+"]";
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKEDhead;
		mGrid->Cells[6][R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma
		mGrid->Cells[6][++R].Font->Style=TFontStyles()<< fsBold;//zapnutí tučného písma

		////definice buněk - parametry - DÉLKA
		//Přejezd 1
		mGrid->Cells[1][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_delka_dopravniku->Value,3,"..");
		mGrid->Cells[1][R].Background->Color=clBACKGROUND;mGrid->Cells[1][R].Font->Color=clLOCKED;
		//Přejezd 2
		mGrid->Cells[3][R].Text="0";
		mGrid->Cells[3][R].Background->Color=clBACKGROUND;mGrid->Cells[3][R].Font->Color=clLOCKED;
		//Celkem
		mGrid->Cells[6][R].Text=F->m.round2double(Form_parametry->scGPNumericEdit_delka_dopravniku->Value,3,"..");
		mGrid->Cells[6][R].Background->Color=clBACKGROUND;mGrid->Cells[6][R].Font->Color=clLOCKED;

		//vymazání nepoužívaných buněk
		mGrid->Cells[0][0].Background->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].TopBorder->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].LeftBorder->Color=(TColor)RGB(240,240,240);mGrid->Cells[0][0].BottomBorder->Width=2;mGrid->Cells[0][0].RightBorder->Width=2;

		mGrid->Cells[5][R-1].Background->Color=(TColor)RGB(240,240,240);
		mGrid->Cells[5][R-1].TopBorder->Width=2;
		mGrid->Cells[5-1][R-1].LeftBorder->Width=2;
		mGrid->Cells[5][R-1].RightBorder->Width=2;

		mGrid->Cells[5][R].Background->Color=(TColor)RGB(240,240,240);
		mGrid->Cells[5-1][R].LeftBorder->Width=2;
		mGrid->Cells[5][R].RightBorder->Width=2;

		//rozdělení čas a délka
		//for(short i=0;i<=6;i++)mGrid->Cells[i][0].BottomBorder->Width=2;//mGrid->Cells[i][2].BottomBorder->Width=2;

		//Sloučení buněk
		mGrid->MergeCells(1,0,4,0);
		mGrid->MergeCells(0,1,0,2);
		mGrid->MergeCells(0,3,0,4);
		mGrid->MergeCells(1,R-1,2,R-1);mGrid->MergeCells(1,R,2,R);
		mGrid->MergeCells(3,R-1,4,R-1);mGrid->MergeCells(3,R,4,R);
		mGrid->MergeCells(5,R-1,5,R);

		////////design - musí být až za tabulkou kvůli pozici
		scLabel_titulek->Caption="Rozbor objektu";
		mGrid->Show();//kvůli přepočítání a získání výšky a šířky výsledné tabulky
		Width=mGrid->Width+2*10+2;
		Left=Form_parametry->Left+Form_parametry->Width/2-Width/2;


		scGPMemo->Width=mGrid->Width+2;
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

