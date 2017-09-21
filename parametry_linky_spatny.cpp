//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "parametry_linky_spatny.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_parametry_linky *Form_parametry_linky;
//---------------------------------------------------------------------------
__fastcall TForm_parametry_linky::TForm_parametry_linky(TComponent* Owner)
	: TForm(Owner)
{
	count=0;
	offsetcomponents=0;
	//ValueListEditor->ItemProps[1]->EditStyle = esEllipsis;  //http://www.functionx.com/bcb/controls/valuelist.htm
	//ValueListEditor1->ItemProps[1]->PickList = ListOfStates;TStringList *ListOfStates = new TStringList;ListOfStates->Add("SD");// Put a combo box in the second item
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormShow(TObject *Sender)
{
//	 rStringGridEd1->Cells[0][1]="1";
//	 rStringGridEd1->Cells[1][1]="Hlavní dopravník";
//	 rStringGridEd1->Cells[2][1]="2";
//	 rStringGridEd1->Cells[3][1]="5";
//	 rStringGridEd1->Cells[4][1]="540";
//
//	 rStringGridEd1->Cells[0][2]="2";
//	 rStringGridEd1->Cells[1][2]="Vedlejší dopravník";
//	 rStringGridEd1->Cells[2][2]="1";
//	 rStringGridEd1->Cells[3][2]="4";
//	 rStringGridEd1->Cells[4][2]="360";
//
//	 Form_parametry_linky->Color=(TColor)RGB(225,225,225);//RGB(43,87,154);
//
//	 scGPButton1->Options->NormalColor=Form_parametry_linky->Color;
//	 scGPButton1->Options->FocusedColor=Form_parametry_linky->Color;
//	 scGPButton1->Options->HotColor=Form_parametry_linky->Color;
//	 scGPButton1->Options->PressedColor=Form_parametry_linky->Color;
//	 scGPButton1->Options->FrameNormalColor=Form_parametry_linky->Color;
//	 scGPButton1->Options->PressedColor=Form_parametry_linky->Color;
//	 scGPButton1->Options->FramePressedColor=Form_parametry_linky->Color;
//
//	 scGPButton2->Options->NormalColor=Form_parametry_linky->Color;
//	 scGPButton2->Options->FocusedColor=Form_parametry_linky->Color;
//	 scGPButton2->Options->HotColor=Form_parametry_linky->Color;
//	 scGPButton2->Options->PressedColor=Form_parametry_linky->Color;
//	 scGPButton2->Options->FrameNormalColor=Form_parametry_linky->Color;
//	 scGPButton2->Options->PressedColor=Form_parametry_linky->Color;
//	 scGPButton2->Options->FramePressedColor=Form_parametry_linky->Color;
//
//	 scGPButton3->Options->NormalColor=Form_parametry_linky->Color;
//	 scGPButton3->Options->FocusedColor=Form_parametry_linky->Color;
//	 scGPButton3->Options->HotColor=Form_parametry_linky->Color;
//	 scGPButton3->Options->PressedColor=Form_parametry_linky->Color;
//	 scGPButton3->Options->FrameNormalColor=Form_parametry_linky->Color;
//	 scGPButton3->Options->PressedColor=Form_parametry_linky->Color;
//	 scGPButton3->Options->FramePressedColor=Form_parametry_linky->Color;

//	OK_status=false;
//	if(Form1->d.v.seznam_dopravniku!="")//pokud jsou data dopravníkù k dispozici
//	{
//		ValueListEditor->Strings->SetText(Form1->d.v.seznam_dopravniku.c_str());
//		//ShowMessage(ValueListEditor->RowCount-3);
//	 //	Button_ADD->Top=63+19*(ValueListEditor->RowCount-3);
//	 //	Button_DEL->Top=63+19*(ValueListEditor->RowCount-3);
//	 //	Button_OK->Top=107+19*(ValueListEditor->RowCount-3);
//	 //	ValueListEditor->Height=57+19*(ValueListEditor->RowCount-3);
//	 //	Form_dopravnik->Height=165+19*(ValueListEditor->RowCount-3);
//		offsetcomponents=0;
//	}
//	else//pro pøípad, že není nic naèteno nebo se jedná o nový soubor, tak zobrazí implicitní informace
//	{
//		ValueListEditor->Strings->SetText(L"hlavní dopravník=5\nvedlejší dopravník=3\n");
//	 //	Button_ADD->Top=63;
//	 //	Button_DEL->Top=63;
//	 //	Button_OK->Top=107;
//	 //	ValueListEditor->Height=57;
//	 //	Form_dopravnik->Height=165;
//		offsetcomponents=0;
//	}
//
//	origSL=new TStringList();
//	origSL->Assign(ValueListEditor->Strings);
//	if(offsetcomponents)//v pøípadì stisku storna pøi pøedcházejícím zobrazení vrácí pùvodní polohu komponent
//	{
//		ValueListEditor->Height-=offsetcomponents;
//		Form_dopravnik->Height-=offsetcomponents;
//	 //	Button_ADD->Top-=offsetcomponents;
//	 //	Button_DEL->Top-=offsetcomponents;
//	 //	Button_OK->Top-=offsetcomponents;
//		offsetcomponents-=offsetcomponents;
//	}
//	offsetcomponents=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_ADDClick(TObject *Sender)
{
// try
// {
//		//if(ValueListEditor->Cells[0][ValueListEditor->RowCount-1]=="")
//		//ValueListEditor->Cells[0][ValueListEditor->RowCount-1]="bez názvu "+ UnicodeString(++count);
//		{
//				ValueListEditor->Height+=19;
//				Form_parametry_linky->Height+=19;
//			 //	Button_ADD->Top+=19;
//			 //	Button_DEL->Top+=19;
//			 //	Button_OK->Top+=19;
//				//Button_CANCEL->Top+=19;
//				offsetcomponents+=19;
//				//ValueListEditor->InsertRow("","",true);//false pro insert + ->ADD
//				//ValueListEditor->Cells[0][ValueListEditor->RowCount-1]="bez názvu "+ UnicodeString(++count);ValueListEditor->Cells[1][ValueListEditor->RowCount-1]=ValueListEditor->Cells[1][1];//add na konec, nebo by šlo také pøes add
//				ValueListEditor->Strings->Insert(ValueListEditor->Row,"bez názvu "+ UnicodeString(++count)+"="+ValueListEditor->Cells[1][1]);
//		}
// }
// catch(...)//v pøípadì, že je objekt již obsažen
// {
//		++count;
//		Button_ADDClick(Sender);
// }

}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_DELClick(TObject *Sender)
{
//	if(Form1->d.v.kontrola_existence_dopravniku(ValueListEditor->Row-1))
//		ShowMessage("Tento dopravník je používán, nelze ho smazat!");
//	else
//	{
//			//if(ValueListEditor->RowCount==2) ShowMessage("Musí být nadefinovaný minimálnì jeden dopravník!");
//			if(ValueListEditor->Row==1) ShowMessage("Nelze smazat hlavní dopravník!");//už bezpøedmìtná podmínka
//			else
//			{
//				Form1->d.v.seznam_dopravniku=ValueListEditor->Strings->GetText();
//				Form1->d.v.aktualizace_indexu_uzitych_dopravniku(ValueListEditor->Row-1);
//				ValueListEditor->DeleteRow(ValueListEditor->Row);
//				ValueListEditor->Height-=19;
//				Form_dopravnik->Height-=19;
//			//	Button_ADD->Top-=19;
//			//	Button_DEL->Top-=19;
//			//	Button_OK->Top-=19;
//				offsetcomponents-=19;
//				//Button_CANCEL->Top-=19;
//			}
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::Button_OKClick(TObject *Sender)
{
	offsetcomponents=0;
	OK_status=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_parametry_linky::FormClose(TObject *Sender, TCloseAction &Action)
{
//	if(!OK_status)
//	{
//		ValueListEditor->Strings->Assign(origSL);
//		delete origSL;
//	}
//	else
//	{
//		Form1->d.v.seznam_dopravniku=ValueListEditor->Strings->GetText();
//	}
}
//---------------------------------------------------------------------------
//zavírání formuláøe stornem
void __fastcall TForm_parametry_linky::Button_stornoClick(TObject *Sender)
{
	Form_parametry_linky->Close();
}
//---------------------------------------------------------------------------


