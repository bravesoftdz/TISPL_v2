//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "kabina_schema.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_kabina_schema *Form_kabina_schema;
//---------------------------------------------------------------------------
__fastcall TForm_kabina_schema::TForm_kabina_schema(TComponent* Owner)
	: TForm(Owner)
{
	//nastavení barvy formuláøe
	Form_kabina_schema->Color=(TColor)RGB(240,240,240);

	//defaultní design a pozicování tlaèítek OK
	Form1->m.designButton(scGPButton_OK,Form_kabina_schema,1,1);

	//design pop-up menu
	PopUPmenu->Color=(TColor)RGB(240,240,240); //nastavení barvy
	Item_posouvat->FillColor=PopUPmenu->Color;
	Item_posunout->FillColor=PopUPmenu->Color;
	Item_priblizit->FillColor=PopUPmenu->Color;
	Item_oddalit->FillColor=PopUPmenu->Color;
	Item_vybrat_oknem->FillColor=PopUPmenu->Color;
	Item_cely_pohled->FillColor=PopUPmenu->Color;

}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::FormShow(TObject *Sender)
{
	Zoom_predchozi=Form1->Zoom;
	if(Form1->pom!=NULL)//pokud je náhled volán z PO a jedná se o náhled konkrétního objektu
	{
		Image_nahled->Visible=false;
		Form_kabina_schema->Color=clWhite;
	}
	else//pokud je náhled volán z PL a jedná se pouze o ilustrativní náhled
	{
		Image_nahled->Visible=true;
		Form_kabina_schema->Color=(TColor)RGB(240,240,240);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::scGPButton_stornoClick(TObject *Sender)
{
	Form1->Zoom=Zoom_predchozi;//návrat do pùvodního stavu
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::KonecClick(TObject *Sender)
{
	Form1->Zoom=Zoom_predchozi;//návrat do pùvodního stavu
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 switch(Key)
	 {
		// ENTER
		case 13:
		{
			Form_kabina_schema->ModalResult = mrOk; // vrátí stejnou hodnotu jako tlaèítko
			Form_kabina_schema->VisibleChanging();// skryje form, stejné jako visible=false
		}
		break;
		// ESC
		case 27:
		{
			if(PopUPmenu->Visible)//pokud je zobrazeno popupmenu je to ESC pro nìj
			PopUPmenu->Visible=false;
			else
			{
				Form_kabina_schema->ModalResult = mrCancel;// vrátí stejnou hodnotu jako tlaèítko
				Form_kabina_schema->VisibleChanging();// skryje form, stejné jako visible=false
			}
		}
		break;
		//F7
		case 118:Form1->ZOOM_IN();Invalidate();break;
		//F8
		case 119:Form1->ZOOM_OUT();Invalidate();break;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::FormMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	 if(Button==mbRight && Form1->pom!=NULL)//je stisknuto pravé tlaèítko myši a náhled kabiny není jen v ilustrativním pøíkladu, ale konkrétním náhledu objektu
	 {
			PopUPmenu->Left=X;PopUPmenu->Top=Y;
			PopUPmenu->Visible=true;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm_kabina_schema::FormPaint(TObject *Sender)
{
	if(Form1->pom!=NULL)
	{
		//náhled objektu
		Form1->d.vykresli_objekt(Canvas,Form1->pom,Form1->m.P2Lx(Left+10),Form1->m.P2Ly(Form1->Height/2),1);

		//mìøítko
		Form1->d.meritko(Canvas,Height-10,0+10);
	}

	//orámování formuláøe
	Form1->m.frameForm(Form_kabina_schema,clWebOrange,1);
}
//---------------------------------------------------------------------------
//zajistí pøiblížení obrazu
void __fastcall TForm_kabina_schema::scLabel_priblizitClick(TObject *Sender)
{
  Form1->ZOOM_IN();Invalidate();
}
//---------------------------------------------------------------------------




