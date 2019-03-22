//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "vykresli.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm2 *Form2;
#include "antialiasing.h"
#include "unit1.h"
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
	AA=true;
	puvX=-100,puvY=-100;
	aktX=-100,aktY=-100;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormShow(TObject *Sender)
{
		////////E0
		TElement *E=new TElement;

		E->mG=new TmGrid(this);//v�dy nutno jako prvn�
		E->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
		E->mG->ID=0;
		E->mG->Left=50;E->mG->Top=50;//hodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
		E->mG->AntiAliasing_text=true;
		E->mG->MovingTable=true;
		E->mG->Border.Width=2;

		unsigned long ColCount=3;//pevn� po�et slopc�
		unsigned long RowCount=3;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek


		E->mG->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky

		E->mG->Cells[0][1].Type=E->mG->DRAW;
		E->mG->Cells[0][1].Text="za��tek <a>[m]</a>";
		E->mG->Cells[0][1].isLink->Color=clRed;
		//E->mG->Cells[1][1].Type=E->mG->EDIT;
		//E->mG->Cells[1][1].Text="abc0";

		//E->mG->Columns[0].Width=800;
		//E->mG->SetColumnAutoFit(0);

		E->predchozi=NULL;
		E->dalsi=NULL;

		ELEMENTY=E;


		////////E1
		TElement *E1=new TElement;

		E1->mG=new TmGrid(this);//v�dy nutno jako prvn�
		E1->mG->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
		E1->mG->ID=1;
		E1->mG->Left=200;E1->mG->Top=200;//hodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
		E1->mG->AntiAliasing_text=true;
		//E1->mG->MovingTable=true;
		E1->mG->Border.Width=2;

		ColCount=2;//pevn� po�et slopc�
		RowCount=5;//dynamick� po�et ��dk�, default 1 je pro 0-t� indexov� ��dek

		E1->mG->Create(ColCount,RowCount);//samotn� vytvo�en� matice-tabulky
		//E1->mG->Cells[0][1].Type=E1->mG->EDIT;
		E1->mG->Cells[0][1].Text="OOO";
		E1->mG->Cells[1][1].Type=E1->mG->EDIT;
		E1->mG->Cells[1][1].Text="abc1";

		E1->predchozi=NULL;
		E1->dalsi=NULL;

		ELEMENTY->dalsi=E1;




//	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
//	mGrid->Tag=4;//ID tabulky
//	mGrid->Left=0;mGrid->Top=0;//vhodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
//	mGrid->AntiAliasing_text=true;

	//mGrid->DefaultCell.isEmpty->Color=clBlue; //mus� b�t p�ed create, jinak nutno nastavovat konkr�tn� bu�ku
	//mGrid->DefaultCell.isZero->Color=clLime;  //mus� b�t p�ed create, jinak nutno nastavovat konkr�tn� bu�ku
	//mGrid->DefaultCell.isNegativeNumber->Color=clRed; //mus� b�t p�ed create, jinak nutno nastavovat konkr�tn� bu�ku

//	mGrid->Create(3,3);//vhodn� jako t�et�




	//mGrid->Border.Width=1;

//	mGrid->Cells[3][2].Text="text v slou�en�ch bu�k�ch";
//	mGrid->Cells[3][2].Font->Color=clWhite;
//	mGrid->Cells[3][2].Align=mGrid->CENTER;
//	mGrid->Cells[3][2].TopBorder->Color=clBlack;mGrid->Cells[3][2].TopBorder->Width=3;
//	*mGrid->Cells[3][2].LeftBorder=*mGrid->Cells[3][2].RightBorder=*mGrid->Cells[3][2].BottomBorder=*mGrid->Cells[3][2].TopBorder;//v tomto ��dku uk�zka p�eb�r�n� vlastnostn� or�mov�n�, jinak samoz�ejm� mo�no zapsat standardn� (a plnit)
//	mGrid->Cells[3][2].Background->Color=clLime;


	//toto propojuje or�mov�n� bun�k ukazatelov�, a to je v�t�inou ne��douc� pou�iv l�pe metody
	//mGrid->Cells[1][2]=mGrid->Cells[2][3]

//	mGrid->Rows[1].Height=2*mGrid->DefaultRowHeight;
//	mGrid->Rows[5].Height=100;
//
//	mGrid->Cells[1][3].Background->Color=clRed;
//	mGrid->Cells[1][3].Font->Color=clBlue;
//	mGrid->Cells[1][3].Align=mGrid->RIGHT;
//	mGrid->Cells[1][3].Valign=mGrid->TOP;
//	mGrid->Cells[1][3].Text="n�jak� dlouh� text";
//
//	mGrid->Cells[7][3].Font->Orientation=900;
//	mGrid->Cells[7][3].Font->Color=clBlue;
//	mGrid->Cells[7][3].Text="Orotovan� text";
//	mGrid->HighlightCell(7,3,clBlue,1);
//	mGrid->Cells[7][3].Align=mGrid->CENTER;
//	mGrid->Cells[7][3].Valign=mGrid->BOTTOM;
//	mGrid->Cells[7][3].TopMargin=0;
//																						mGrid->Cells[0][1].Text="abcdef";
//	 mGrid->Cells[1][0].Type=mGrid->DRAW; mGrid->Cells[1][0].Text="0";
//	 mGrid->Cells[1][1].Type=mGrid->DRAW; mGrid->Cells[1][1].Text="-1";
//	 mGrid->Cells[1][2].Type=mGrid->DRAW; mGrid->Cells[1][2].Text="";
//	 mGrid->Cells[1][2].isNegativeNumber->Color=clRed;
//	 mGrid->Cells[1][2].isEmpty->Color=clBlue;
//	 mGrid->Cells[1][2].isZero->Color=clYellow;
//	 mGrid->Cells[1][3].Type=mGrid->EDIT; mGrid->Cells[1][3].Text="3";
//	 mGrid->Cells[1][4].Type=mGrid->EDIT; mGrid->Cells[1][4].Text="4";
//	 mGrid->Cells[1][5].Type=mGrid->EDIT; mGrid->Cells[1][5].Text="5";
//	 mGrid->Cells[1][6].Type=mGrid->EDIT; mGrid->Cells[1][6].Text="6";
//	 mGrid->Cells[1][7].Type=mGrid->EDIT; mGrid->Cells[1][7].Text="7";
//	 mGrid->Cells[1][8].Type=mGrid->EDIT; mGrid->Cells[1][8].Text="8";
//	 mGrid->Cells[1][9].Type=mGrid->EDIT; mGrid->Cells[1][9].Text="9";

//	mGrid->Cells[1][5].Type=mGrid->CHECK;
//	//mGrid->Cells[1][5].Valign=mGrid->TOP;
//	//mGrid->Cells[1][5].Align=mGrid->LEFT;
//
//	mGrid->Cells[2][5].Type=mGrid->RADIO;
//
//	mGrid->Cells[3][5].Type=mGrid->NUMERIC;
//
//	mGrid->Cells[4][5].Type=mGrid->EDIT;
//	mGrid->Cells[4][5].Text="tady je edit";
//
//	mGrid->Cells[5][5].Type=mGrid->BUTTON;
//	mGrid->Cells[5][5].Text="tla��tko";
//
//	mGrid->Cells[6][5].Text="COMBO";
//	mGrid->Cells[6][5].Type=mGrid->COMBO;

//	//uk�zka p�eb�r�n� vlastnost� spole�n�ho or�mov�n� bun�k
//	mGrid->Cells[8][8].TopBorder->Width=3;
//	mGrid->Cells[8][8].TopBorder->Color=clBlue;
//	mGrid->Cells[8][7].BottomBorder->Color=clWebOrange;
////	if(mGrid->Cells[8][7].BottomBorder->Color==clWebOrange)
////	ShowMessage("clWebOrange");
//	mGrid->Cells[8][8].LeftBorder->Color=clWebOrange;
//	mGrid->Cells[7][8].RightBorder->Color=clBlue;
//
//	//nastav� oblast bun�k na stejn�mi vlastnostmi (vhodn� nap�. na fixed cells, or�mov�n� n�jak� oblasti, disabled/read-only oblast atp.)
//	mGrid->Cells[0][0].Text="Nadpis";
//	mGrid->Cells[0][0].Background->Color=clBtnFace;
//	mGrid->Cells[0][0].Align=mGrid->RIGHT;
//	mGrid->Cells[0][0].Valign=mGrid->TOP;
//	mGrid->SetCells(mGrid->Cells[0][0],0,0,0,7);
//	mGrid->SetCells(mGrid->Cells[0][0],0,0,7,0);


 //	mGrid->SetColumnAutoFit(1);//uk�zka nastaven� velikosti sloupce dle obsahu, m��e b�t um�st�no kdekoliv p�ed Show()

	//merge bun�k mus� b�t, co nejbl�e nad Show() a hned po SetColumnAutoFit, pop�. by marge mohl b�t kdekoliv, ale bylo by nutn� po n�m volat FormPaint(this)
	//mGrid->MergeCells(3,2,5,3);
	//mGrid->MergeCells(7,3,8,9);

//	mGrid->AntiAliasing_text=false;//nemu�e b�t varianta  AntiAliasing_text=true; a AntiAliasing_grid=false; (je i o�et�eno)
//	mGrid->AntiAliasing_grid=false;

	//tabulka 2 lok�ln� deklarace TmGrit (v Unit2.h)
//	mGrid2=new TmGrid(this);
//	mGrid2->Tag=2;
//	mGrid2->Top=mGrid->Top+mGrid->Height+10;
//	mGrid2->Left=mGrid->Left+10;
//	mGrid2->Create(3,10);
//	mGrid2->Cells[1][0].Text="0";
//	mGrid2->Cells[1][1].Text="1";
//	mGrid2->Cells[1][2].Text="2";
//	mGrid2->Cells[1][3].Text="3";
//	mGrid2->Cells[1][4].Text="4";
//	mGrid2->Cells[1][5].Text="5";
//	mGrid2->Cells[1][6].Text="6";
//	mGrid2->Cells[1][7].Text="7";
//	mGrid2->Cells[1][8].Text="8";
//	mGrid2->Cells[1][9].Text="9";
//
//	mGrid2->Cells[1][1].Type=mGrid->BUTTON;
//	mGrid2->Cells[1][1].Text="tla��tko";
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormPaint(TObject *Sender)
{
	ELEMENTY->mG->Show();
	ELEMENTY->dalsi->mG->Show();

	//mGrid2->Show();
	//nastav� formul�� dle velikosti tabulky, mus� b�t a� po Show
	//Form2->Width=mGrid->Width+50+50;
	//Form2->Height=mGrid->Height+50+50;


//	Canvas->Pen->Width=1;
//	Canvas->Pen->Style=psDashDotDot;
//	Canvas->MoveTo(200,200);
//	Canvas->LineTo(400,400);


//	//if(FileName_short(FileName)=="kabina_base_coat.bmp")
//	{
//		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
//		if(FileExists("kabina_base_coat.bmp"))
//		{
//			Graphics::TBitmap *bmp=new Graphics::TBitmap;
//			bmp->LoadFromFile("kabina_base_coat.bmp");
//			long X=10,Y=-10;long double resolution=0.01200428724544480171489817792069;
//			Canvas->StretchDraw(TRect(F->m.L2Px(X),F->m.L2Py(Y),F->m.round(F->m.L2Px(X)+bmp->Width*F->Zoom*resolution/F->m2px),F->m.round(F->m.L2Py(Y)+bmp->Height*F->Zoom*resolution/F->m2px)),bmp);
//			delete(bmp);
//		}
//	}
//
//
	Cvykresli d;
//	if(AA)
//	{
		Cantialising a;
		Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
		bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vypl�v� z logiky algoritmu antialiasingu
		F->Zoom*=3;//*3 vypl�v� z logiky algoritmu antialiasingu
		d.vykresli_robota(bmp_in->Canvas,F->m.L2Px(aktX),F->m.L2Py(aktY),"Robot 1","R1",1);
//		d.vykresli_robota(bmp_in->Canvas,(aktX+100+10)*F->Zoom,aktY*F->Zoom,"Robot 2","R2",1,1);
//		d.vykresli_robota(bmp_in->Canvas,(aktX+200+20)*F->Zoom,aktY*F->Zoom,"Robot 3","R3",2,1);
//		d.vykresli_robota(bmp_in->Canvas,(aktX+300+30)*F->Zoom,aktY*F->Zoom,"Robot 4","R4",3,1);
//		d.vykresli_robota(bmp_in->Canvas,(aktX+400+40)*F->Zoom,aktY*F->Zoom,"Robot 5","R5",0,-1);
//		d.vykresli_stopku(bmp_in->Canvas,F->m.L2Px(aktX)+100,F->m.L2Py(aktY),"Stop 1","S1",1,0);
//		d.vykresli_otoc(bmp_in->Canvas,F->m.L2Px(aktX)+200,F->m.L2Py(aktY),"Oto� pasiv","O1",0,0);
//		d.vykresli_otoc(bmp_in->Canvas,(aktX-500)*F->Zoom,aktY*F->Zoom,"Oto� aktiv","O2",1,1);
		F->Zoom/=3;//navr�cen� zoomu na p�vodn� hodnotu
		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutn� do samostatn� bmp, kv�li smaz�n� bitmapy vracen� AA
		bmp_out->Transparent=true;
		bmp_out->TransparentColor=clWhite;
		Canvas->Draw(0,0,bmp_out);
		delete (bmp_out);//velice nutn�
		delete (bmp_in);//velice nutn�
//	}
//	else
//	{
//		d.vykresli_robota(Canvas,aktX,aktY,"Robot 1","R1",0,1);
//		d.vykresli_stopku(Canvas,(aktX-600)*F->Zoom,aktY*F->Zoom,"Stop 1","S1",1,0);
//		d.vykresli_otoc(Canvas,(aktX-500)*F->Zoom,aktY*F->Zoom,"Oto� aktiv","O2",1,1);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
//	//pln�n� comba
//	TscGPComboBox *C=mGrid->getCombo(6,5);
//	//C->Options->dal�� mo�nosti
//	//C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu  - nepou�ivat pokud je pr�zdn�
//	TscGPListBoxItem *t=C->Items->Add();t->Caption="test1";
//	t=NULL;t=C->Items->Add();t->Caption="test2";
//	C->ItemIndex=1;
//	t=NULL;delete t;
//	C=NULL;delete C;//nejsem si jist� zda pou��vat nebo ne, pokud bude padat, tak vyzkou�et tady zakomentovat
//	//aktualizace comba �i obecn� komponent nepot�ebuje refresh
//
//	mGrid->AntiAliasing_text=!mGrid->AntiAliasing_text;
//	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�

		AA=!AA;
		Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
//	for(unsigned long X=0;X<mGrid->ColCount;X++)
//	ShowMessage(	mGrid->Columns[X].Left);

	//testov�n� checkboxu
	//if(mGrid->getRadio(2,5,this)->Checked)ShowMessage("za�krnuto");
	//else ShowMessage("neza�krtnuto");

	//zm�na velikosti tabulky
	//mGrid->RowCount=20;mGrid->ColCount=3;//zajist� realokaci tabulky (pokud je n�sledn� vol�no invalidate)


	//mGrid->AntiAliasing_grid=!mGrid->AntiAliasing_grid;
	 ELEMENTY->mG->Left+=10;
	//zm�na posunu tabulky

	Invalidate();
	FormPaint(this);//vol�n� po Invalidate zajist�, �e nedoch�z� k probliku komponent, nem��e b�t samotn�
}
//---------------------------------------------------------------------------
//test vol�n� p�i onclick
void TForm2::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	//ShowMessage("UNIT2\nDo�lo ke kliku v tabulce tag formu: "+AnsiString(Tag)+", ID tabulky: "+AnsiString(ID)+", na bu�ce: "+AnsiString(Col)+","+AnsiString(Row));
//	mGrid->HighlightCell(Col,Row);
//	mGrid->Cells[0][0].Text="test";
//	FormPaint(this);//zajist� p�ekreslen� bez probliku
//	//toto problik�v� mGrid->Refresh();
}
//---------------------------------------------------------------------------
void TForm2::OnEnter(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void TForm2::OnChange(long Tag,unsigned long Col,unsigned long Row)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
//	 mGrid->Delete();//pokud chci odstranit a nechci pou��t na dal�� pou�it�
//	 mGrid2->Delete();
//	 mGrid2=NULL; delete mGrid2;
}
//---------------------------------------------------------------------------
//p�iklady
/*
///////////vytvvo�en�
	mGrid=new TmGrid;
	mGrid->Left=600;mGrid->Top=40;
	//myGrid->ColCount=3;myGrid->RowCount=5;//po�et ��dk� a sloupc�
	//myGrid->Create(); lze p��mo vytvo�it n�e
	mGrid->Create(4,8);

	mGrid->Cells[0][0].Text="2,5";
	mGrid->Cells[0][0].Type=mGrid->NUMERIC;

	mGrid->Cells[0][5].Text="Checkbox";
	mGrid->Cells[0][5].Type=mGrid->CHECK;

	mGrid->Cells[1][5].Text="Tla��tko";
	mGrid->Cells[1][5].Type=mGrid->BUTTON;
	mGrid->Cells[1][5].Background->Color=clGreen;

	mGrid->Cells[0][6].Text="Edit enabled";
	mGrid->Cells[0][6].Type=mGrid->EDIT;

	mGrid->Cells[1][6].Text="Edit disabled";
	mGrid->Cells[1][6].Type=mGrid->readEDIT;

	mGrid->Cells[2][5].Text="text";
	mGrid->Cells[2][5].Font->Color=clRed;
	mGrid->Cells[2][5].Background->Color=clGreen;

	mGrid->Cells[0][1].Text="Radio";
	mGrid->Cells[0][1].Type=mGrid->RADIO;

	mGrid->Cells[3][7].Text="COMBO";
	mGrid->Cells[3][7].Type=mGrid->COMBO;

	mGrid->Cells[1][1].Font->Orientation=900;
	mGrid->Cells[1][1].Valign=mGrid->BOTTOM;

	mGrid->SetColumnAutoFit(-1);
	FormPaint(this);//zajist� p�ekreslen� bez probliku

/////////// pr�b�n� zm�na
		 mGrid->HighlightCell(1,5);

		mGrid->getEdit(0,6,this)->Text="JO!";
	mGrid->getEdit(0,6,this)->Font->Color=clRed;
//	TscGPEdit *E=mGrid->getEdit(this,0,6);
//	E->Text="JO!";
//	E=NULL;delete E;

		mGrid->MergeCells(1,1,2,1);
		mGrid->MergeCells(1,1,1,2);

TscGPComboBox *C=mGrid->getCombo(3,7,this);
C->Items->Clear();//smaz�n� p��padn�ho p�vodn�ho obsahu
TscGPListBoxItem *t=NULL;t=C->Items->Add();t->Caption="test1";
t=NULL;t=C->Items->Add();t->Caption="test2";
C->ItemIndex=1;
C=NULL;delete C;

/////////// zm�na po�tu ��dk� �i sloupc (realokace) zde nefungovalo
		mGrid->Left=500;
		mGrid->ColCount=4;mGrid->RowCount=4;

		mGrid->SetColumnAutoFit(-3);
		FormPaint(this);//zajist� p�ekreslen� bez probliku
*/
//---------------------------------------------------------------------------
void __fastcall TForm2::Button5Click(TObject *Sender)
{
	mGrid->AddRow();
	//test pouze p�id�n� textu:
	mGrid->Cells[1][mGrid->RowCount-1].Text=mGrid->RowCount-1;FormPaint(this);//nutno kv�li aktualizaci textu
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button4Click(TObject *Sender)
{
	mGrid->InsertRow(3);//s problikem zp�sobuje show() v InsertRow
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button3Click(TObject *Sender)
{
	mGrid->DeleteRow(3);
	//mGrid->DeleteRow(mGrid->RowCount-1);//s problikem zp�sobuje show() v DeleteRow
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
//	if(aktX==-50000 && aktY==-50000)
//	{
//		Cvykresli d;
//		//d.vykresli_robota(Canvas,puvX,puvY,"","",3,1,0,true);
//		//d.vykresli_robota(Canvas,X,Y,"","",3,1,0,true);
//
//		//d.vykresli_stopku(Canvas,puvX,puvY,"","",0,0,true);
//		//d.vykresli_stopku(Canvas,X,Y,"","",0,0,true);
//
//		d.vykresli_otoc(Canvas,puvX,puvY,"","",1,1,0,true);
//		d.vykresli_otoc(Canvas,X,Y,"","",1,1,0,true);
//
//
//		puvX=X,puvY=Y;
//	}
	if(Shift.Contains(ssShift)){aktX=F->m.P2Lx(X);aktY=F->m.P2Ly(Y);Invalidate();FormPaint(this);}

	if(Shift.Contains(ssCtrl))
	{
		InvalidateRect(Handle,&TRect(ELEMENTY->dalsi->mG->Left-1,ELEMENTY->dalsi->mG->Top-1,ELEMENTY->dalsi->mG->Left+ELEMENTY->dalsi->mG->Width+1,ELEMENTY->dalsi->mG->Top+ELEMENTY->dalsi->mG->Height+1),true);//sma�e starou oblast
		//�lo by vylep�it p�ekreslovan�m pouze obd�ln�ku rozd�lu mezi sou�adnicemi
		ELEMENTY->dalsi->mG->Left=X;ELEMENTY->dalsi->mG->Top=Y;
		FormPaint(this);
		//Invalidate();FormPaint(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
	aktX=F->m.P2Lx(X);aktY=F->m.P2Ly(Y);
	FormPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	 switch(Key)
	 {
		//F5
		case 116: aktX=-50000;aktY=-50000;Invalidate();break;
		//F7
		case 118:F->ZOOM_IN();break;
		//F8
		case 119:F->ZOOM_OUT();break;
	 }
}
//---------------------------------------------------------------------------





