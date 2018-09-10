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
	//nastaven� barvy formul��e
	Form_poznamky->Color=(TColor)RGB(240,240,240);

	//defaultn� design a pozicov�n� tla��tek OK a Storno
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
			Form_poznamky->ModalResult = mrOk; // vr�t� stejnou hodnotu jako tla��tko
			Form_poznamky->VisibleChanging();// skryje form, stejn� jako visible=false
	}
	if (Key == 27) // ESC
	{
			Form_poznamky->ModalResult = mrCancel;// vr�t� stejnou hodnotu jako tla��tko
			Form_poznamky->VisibleChanging();// skryje form, stejn� jako visible=false
	}
}
//---------------------------------------------------------------------------
//vykresl� or�mov�n�
void __fastcall TForm_poznamky::FormPaint(TObject *Sender)
{
	Form1->m.frameForm(Form_poznamky,clWebOrange,1);
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)mGrid->Show();//vykresl� tabulku
}
//---------------------------------------------------------------------------
//p�i zobrazen� formul��e
void __fastcall TForm_poznamky::FormShow(TObject *Sender)
{
	if(Form_parametry->scComboBox_rezim->ItemIndex==0)//pro S&G
	{
		////design
		scLabel_titulek->Caption="Technologick� �as";
		scGPMemo->Top=48+100;
		scGPMemo->Height=233-100;
		rHTMLLabel_InfoText->Top=scGPMemo->Top-rHTMLLabel_InfoText->Height;

		////tabulka
		mGrid=new TmGrid(this);//v�dy nutno jako prvn�
		mGrid->Tag=5;//ID tabulky
		mGrid->Left=10;mGrid->Top=scGPPanel_hlavicka->Height+10;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
		mGrid->AntiAliasing_text=true;//bez toho d�l� b�l� blok, pozad� - posunuto, jedna se o bug v mGridu
		mGrid->Create(6,2);//vhodn� jako t�et�
		//p�evody jednotek
		AnsiString CTunitT="s";if(Form_parametry->CTunit)CTunitT="min";
		//definice bun�k - hlavi�ka
		mGrid->Cells[0][0].Text="MT1 ["+CTunitT+"]";// - �as p�esunu 1";
		mGrid->Cells[0][0].Background->Color=clBACKGROUND;
		mGrid->Cells[1][0].Text="PT ["+CTunitT+"]";// - procesn� �as";
		mGrid->Cells[1][0].Background->Color=clBACKGROUND;mGrid->Cells[1][0].Font->Color=clLOCKED;
		mGrid->Cells[2][0].Text="WT1 ["+CTunitT+"]";// - �as �ek�n� na nejbli��� palec 2";
		mGrid->Cells[2][0].Background->Color=clBACKGROUND;mGrid->Cells[2][0].Font->Color=clLOCKED;
		mGrid->Cells[3][0].Text="MT2 ["+CTunitT+"]";// - �as p�esunu 2";
		mGrid->Cells[3][0].Background->Color=clBACKGROUND;
		mGrid->Cells[4][0].Text="WT2 ["+CTunitT+"]";// - �as �ek�n� na nejbli��� palec 2";
		mGrid->Cells[4][0].Background->Color=clBACKGROUND;mGrid->Cells[4][0].Font->Color=clLOCKED;
		mGrid->Cells[5][0].Text="CT ["+CTunitT+"]";// - Celkov� technologick� �as";
		mGrid->Cells[5][0].Background->Color=clBACKGROUND;mGrid->Cells[5][0].Font->Color=clLOCKED;
		mGrid->Cells[5][0].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
		mGrid->Cells[5][1].Font->Style=TFontStyles()<< fsBold;//zapnut� tu�n�ho p�sma
		//v�po�etn� ��st
		double MT1=0;
		if(F->pom->dalsi->pohon!=NULL)MT1=Form_parametry->scGPNumericEdit_delka_dopravniku->Value/(1.0+Form_parametry->DDunit*999.0)/F->pom->dalsi->pohon->aRD;
		MT1*=(1.0+Form_parametry->CTunit*59.0);
		double WT2=0;
		if(F->pom->dalsi->pohon!=NULL)WT2=F->m.cekani_na_palec(0,F->pom->dalsi->pohon->roztec,F->pom->dalsi->pohon->aRD,3);
		WT2*=(1.0+Form_parametry->CTunit*59.0);
		////definice bun�k - parametry
		//MT1
		mGrid->Cells[0][1].Type=mGrid->EDIT; mGrid->Cells[0][1].Text=F->m.round2double(MT1,2,"..");
		//PT
		double PT=Form_parametry->scGPNumericEdit_CT->Value-MT1-WT2;
		mGrid->Cells[1][1].Text=F->m.round2double(PT,2,"..");
		mGrid->Cells[1][1].Background->Color=clBACKGROUND;
		ShowMessage(F->ms.MyToDouble(mGrid->Cells[1][1].Text));
		if(PT>0)mGrid->Cells[1][1].Font->Color=clLOCKED;else mGrid->Cells[1][1].Font->Color=clRed;
		//WT1
		mGrid->Cells[2][1].Background->Color=clBACKGROUND; mGrid->Cells[2][1].Font->Color=clLOCKED;mGrid->Cells[2][1].Text="0";
		//MT2
		mGrid->Cells[3][1].Type=mGrid->EDIT; mGrid->Cells[3][1].Text="0";
		//WT2
		mGrid->Cells[4][1].Background->Color=clBACKGROUND;mGrid->Cells[4][1].Font->Color=clLOCKED; mGrid->Cells[4][1].Text=F->m.round2double(WT2,2,"..");
		//CT
		mGrid->Cells[5][1].Background->Color=clBACKGROUND;mGrid->Cells[5][1].Font->Color=clLOCKED;mGrid->Cells[5][1].Text=F->m.round2double(Form_parametry->scGPNumericEdit_CT->Value,2,"..");
	}
	else//pro ostatn� re�imy
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
	mGrid->Delete();//pokud chci odstranit a nechci pou��t na dal�� pou�it�
}
//---------------------------------------------------------------------------

