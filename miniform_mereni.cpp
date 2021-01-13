//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "miniform_mereni.h"
#include "unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma resource "*.dfm"
TForm_mereni *Form_mereni;
//---------------------------------------------------------------------------
__fastcall TForm_mereni::TForm_mereni(TComponent* Owner)
	: TForm(Owner)
{
	mGrid=NULL;
	pocet_radku=0;
	data_tabulky="";
	closing=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_mereni::FormShow(TObject *Sender)
{
	vytvor_napln_mGrid();
	Width=mGrid->Width+2*scGPPanel_header->Height;
	Height=mGrid->Height+3*scGPPanel_header->Height+scGPButton_OK->Height;
	Left=FX=F->Width-Width;
	Top=FY=F->scGPPanel_mainmenu->Height;
	scGPButton_OK->Top=mGrid->Top+mGrid->Height+0.5*scGPPanel_header->Height;
	scGPButton_OK->Left=Width/2.0-scGPButton_OK->Width/2.0;
	closing=false;
	scGPGlyphButton_pripnout->Visible=false;
  pocet_radku=0;
	data_tabulky="";

}
//---------------------------------------------------------------------------
//vytvo�� tabulku s hodnotami m��en�
void TForm_mereni::vytvor_napln_mGrid()
{
  //vytvo�en� a nastaven� mGridu
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=4;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->ID=0;
	mGrid->Left=scGPPanel_header->Height;mGrid->Top=2*scGPPanel_header->Height;//hodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->Border.Width=2;
	mGrid->DefaultCell.Font->Name=F->aFont->Name;
	mGrid->DefaultCell.Font->Size=F->aFont->Size;
	mGrid->DefaultCell.Font->Color=(TColor)RGB(128,128,128);
	mGrid->DefaultCell.isZero=mGrid->DefaultCell.Font;//zamezen� podbarevn� bu�ky obsahuj�c� 0 jinou barvou
	mGrid->Create(4, 2);

	//nastaven� popisk�
	mGrid->Cells[0][0].Text="M��en� v �seku";
	mGrid->Cells[0][1].Text="Prvnky";
	mGrid->Cells[1][1].Text="Vzd�lenost [mm]";
	mGrid->Cells[2][1].Text="IN [s]";
	mGrid->Cells[3][1].Text="OUT [s]";

	//barvy hlavi�ky
	mGrid->Cells[0][0].Font->Color=clBlack;
	mGrid->Cells[0][1].Font->Color=clBlack;
	mGrid->Cells[1][1].Font->Color=clBlack;
	mGrid->Cells[2][1].Font->Color=clBlack;
	mGrid->Cells[3][1].Font->Color=clBlack;

	//���ky a slu�ov�n�bu�ek
  mGrid->SetColumnAutoFit(-4);
	mGrid->Columns[1].Width=150;
	mGrid->Columns[2].Width=130;
	mGrid->Columns[3].Width=130;
	mGrid->MergeCells(0,0,3,0);

	napln_data_tabulky();

	//p�id�n� ��dk� do tabulky
	for(unsigned int i=1;i<=pocet_radku;i++)
	{
		mGrid->AddRow(false,false);
	}
	mGrid->Update();//nutn� po p�id�n� ��dku!!!

  //napln�n� ��dk� daty
	int oddelovac=0;String hodnota="";
	for(unsigned int i=2;i<mGrid->RowCount;i++)
	{
		for(unsigned int j=0;j<mGrid->ColCount;j++)
		{
			//extrakce hodnoty z �etezce
			oddelovac=data_tabulky.Pos(";");
			hodnota=data_tabulky.SubString(1,oddelovac-1);
			data_tabulky.Delete(1,oddelovac);//smaz�n� hodnoty z �et�zce
			mGrid->Cells[j][i].Text=hodnota;
		}
	}
}
//---------------------------------------------------------------------------
//vytvo�� seznam dat o ��dc�ch tabulky m��en�
void TForm_mereni::napln_data_tabulky()
{
	pocet_radku=0;
	data_tabulky="";
	String prejezd=F->ls->Strings[486];//"p�ejezd"

	//line�rn� m��en�
	if(F->d.v.MAG_LASO->sparovany==NULL || F->d.v.MAG_LASO->predchozi->sparovany==NULL)
	{
		data_tabulky="-;"+String(F->m.round2double(F->m.delka(F->d.v.MAG_LASO->Element->geo.X1,F->d.v.MAG_LASO->Element->geo.Y1,F->d.v.MAG_LASO->predchozi->Element->geo.X4,F->d.v.MAG_LASO->predchozi->Element->geo.Y4)*1000,2))+";-;-;";
		pocet_radku++;
	}
	//m��en� magnetick�m lasem
	else
	{
  	double X=0,Y=0,RA=0,OR=0,uhel=0,d=0,delka=0,delka_pom=0,delka_Pud=0,m=0,R=0;
  	TPointD in,out;
  	bool prichyceno=false;/////odstranit!!!!!�

  	Cvektory::TCesta *C=F->d.v.MAG_LASO->dalsi;
  	while(C!=NULL)
  	{
  		////na�ten� popisku do prvn�ho sloupce
  		if(C->dalsi!=NULL)
  		{
  			if(C->Element->eID!=MaxInt)data_tabulky+=C->Element->name+";";else data_tabulky+=prejezd+";";//mus� b�t �e�eno p�es prom�nnou, nelze zad�vat p��mo takto: F->ls->Strings[486]
  		}
  		else
  		{
  			if(C->sparovany->eID!=MaxInt)data_tabulky+=C->sparovany->name+";";else data_tabulky+=prejezd+";";
  		}
  		//nastaven� parametr� pro vykreslen� + v�po�et d�lky
  		X=C->Element->geo.X1;Y=C->Element->geo.Y1;
  		RA=C->Element->geo.rotacni_uhel;
  		OR=C->Element->geo.orientace;
  		uhel=0;
  		if(C->n==1)
  		{
  			X=F->d.v.MAG_LASO->Element->geo.X1;Y=F->d.v.MAG_LASO->Element->geo.Y1;
  			RA=F->d.v.MAG_LASO->sparovany->geo.rotacni_uhel;
  			OR=F->d.v.MAG_LASO->sparovany->geo.orientace;
  		}
  		//vypo�et delky
  		if(C->Element->n!=MaxInt && C->n!=1)
  		{
  			delka_Pud=d=C->Element->geo.delka;
  			if(C->Element->geo.HeightDepp!=0)delka_Pud=C->Element->geo.delkaPud;
  		}
  		else if(C->Element->geo.typ==0)
  		{
  			delka_Pud=d=F->m.delka(X,Y,C->Element->geo.X4,C->Element->geo.Y4);
  			d=F->m.castPrepony(d,C->Element->geo.delkaPud,C->Element->geo.HeightDepp);
  		}
  		else
  		{
  			d=F->m.delka(X,Y,C->Element->geo.X4,C->Element->geo.Y4);
  			uhel=F->m.T2Aarc(C->Element->geo.radius,d);
  			delka_Pud=d=F->m.R2Larc(C->Element->geo.radius,uhel);
  		}
  		delka+=d;   data_tabulky+=String(F->m.round2double(d,5)*1000)+";";
  		delka_pom+=delka_Pud;

  		if(C->Element->geo.typ==0)
  		{
  			R=delka_Pud;
  			uhel=0;
  		}
  		else
  		{
  			R=C->Element->geo.radius;
  			if(C->n==1)R=F->d.v.MAG_LASO->sparovany->geo.radius;
  			if(uhel==0)uhel=RA;//max z rota�n�ho �hlu
  		}
  		//v�po�et �asu
  		if(C->Element!=NULL && C->Element->pohon!=NULL)
  		{
  			//�as p�ejezdu
  			in.x+=d/C->Element->pohon->aRD;in.y+=d/C->Element->pohon->aRD;
  			out.x+=d/C->Element->pohon->aRD;out.y+=d/C->Element->pohon->aRD;
  			//p�ipo��t�vat casy prvn�ho
  			//pokud m�m na��tat dat z prvn�ho elementu, na��t�m pouze WT a latenci
  			if(C->n==1 && F->d.v.MAG_LASO->Element->geo.X1==C->Element->geo.X4 && F->d.v.MAG_LASO->Element->geo.Y1==C->Element->geo.Y4)
  			{
          if(C->Element->eID==0)
  				{
  					//WT - pouze do druh� slo�ky IN
  					in.y+=C->Element->WT;
  					//latence, pokud je nastaveno
  					if(F->scGPCheckBox_meridlo_casy->Checked && C->Element->dalsi!=NULL && C->Element->dalsi->pohon!=NULL){in.x+=F->m.latence_mezi_stopkami(C->Element->dalsi->pohon->aRD);in.y+=F->m.latence_mezi_stopkami(C->Element->dalsi->pohon->aRD);}
  				}
  			}
  			//zaokrouhlen� �asu na takt
  			if((!F->scGPCheckBox_meridlo_casy->Checked && (C->Element!=F->d.v.MAG_LASO->sparovany || (C->Element==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))) || (F->scGPCheckBox_meridlo_casy->Checked && C->n>1) || (F->scGPCheckBox_meridlo_casy->Checked && (C->Element!=F->d.v.MAG_LASO->sparovany || (C->Element==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))))
  			{
  				//jedn� se o element uvnit� seznamu (n�sleduje dal��)
  				/*if(!prichyceno || (prichyceno && C->Element!=F->pom_element))*/if(F->d.v.vrat_druh_elementu(C->Element)==0){in.y=ceil(in.x/F->d.v.PP.TT)*F->d.v.PP.TT;in.x=in.y-C->Element->WT;}
  				/*if(!prichyceno || (prichyceno && C->Element!=F->pom_element))*/if(F->d.v.vrat_druh_elementu(C->Element)==0){out.y=ceil(out.x/F->d.v.PP.TT)*F->d.v.PP.TT;out.x=out.y-C->Element->WT;}
  				//jedn� se o posledn�
  				//if(prichyceno && C->dalsi==NULL && C->Element==F->pom_element && F->d.v.vrat_druh_elementu(F->pom_element)==0){out.y=ceil(out.x/F->d.v.PP.TT)*F->d.v.PP.TT;out.x=out.y-F->pom_element->WT;}
  			}
  		}

  		////na�ten� �asu do posledn�ch dvou sloupc�
  		//prvn� ��dek
  		if(C->n==1 && F->d.v.MAG_LASO->Element->geo.X2==F->d.v.MAG_LASO->Element->geo.X3)
  		{
  			data_tabulky+="-;";
  			data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
  		}
  		//posledn� ��dek
  		else if(C->dalsi==NULL && ((C->Element->geo.X2==C->Element->geo.X3 && F->d.v.vrat_druh_elementu(C->sparovany)!=0) || C->Element->geo.X2!=C->Element->geo.X3))
  		{
  			data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
  			data_tabulky+="-;";
  		}
      //ostatn�
  		else
  		{
  			data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
  			data_tabulky+=String(F->m.round2double(out.x,2))+" - "+String(F->m.round2double(out.y,2))+";";
  		}
  		pocet_radku++;

  		//posun na dal�� segment
  		C=C->dalsi;
  	}
		delete C;C=NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_mereni::SkrytClick(TObject *Sender)
{
	closing=true;
	mGrid->Delete();
	mGrid=NULL;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_mereni::scLabel_headerMouseUp(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	if(!scGPGlyphButton_pripnout->Visible && (Left!=FX || Top!=FY))
		scGPGlyphButton_pripnout->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_mereni::scGPGlyphButton_pripnoutClick(TObject *Sender)
{
	Left=FX;
	Top=FY;
	scGPGlyphButton_pripnout->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_mereni::FormPaint(TObject *Sender)
{
	mGrid->Show(Canvas);
}
//---------------------------------------------------------------------------
