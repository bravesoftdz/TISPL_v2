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
//konstruktor
__fastcall TForm_mereni::TForm_mereni(TComponent* Owner)
	: TForm(Owner)
{
	mGrid=NULL;
	pocet_radku=0;
	data_tabulky="";
	data_tabulky_zaloha="";
	JID=0;
	closing=false;
}
//---------------------------------------------------------------------------
//zobrazen� formu
void __fastcall TForm_mereni::FormShow(TObject *Sender)
{
  pocet_radku=0;
	data_tabulky="";
	data_tabulky_zaloha="";
  JID=0;
	vytvor_napln_mGrid();
	Width=mGrid->Width+2*scGPPanel_header->Height;
	Height=mGrid->Height+3*scGPPanel_header->Height+scGPButton_OK->Height;
	Left=FX=F->Width-Width;
	Top=FY=F->scGPPanel_mainmenu->Height;
	scGPButton_OK->Top=mGrid->Top+mGrid->Height+0.5*scGPPanel_header->Height;
	scGPButton_OK->Left=Width/2.0-scGPButton_OK->Width/2.0;
	closing=false;
	scGPGlyphButton_pripnout->Visible=false;
}
//---------------------------------------------------------------------------
//vytvo�� tabulku s hodnotami m��en�
void TForm_mereni::vytvor_napln_mGrid()
{
  //vytvo�en� a nastaven� mGridu
	mGrid=new TmGrid(this);//v�dy nutno jako prvn�
	mGrid->Tag=10;//ID tabulky,resp. formu //1...-gapoTT, 2... - gapoV, 3... - gapoR
	mGrid->ID=0;
	mGrid->Left=scGPPanel_header->Height;mGrid->Top=2*scGPPanel_header->Height;//hodn� jako druh� (pop�. by bylo nutn� p�ekreslovat)
	mGrid->AntiAliasing_text=true;
	mGrid->Border.Width=2;
	mGrid->DefaultCell.Font->Name=F->aFont->Name;
	mGrid->DefaultCell.Font->Size=F->aFont->Size;
	mGrid->DefaultCell.Font->Color=(TColor)RGB(128,128,128);
	mGrid->DefaultCell.isZero=mGrid->DefaultCell.Font;//zamezen� podbarevn� bu�ky obsahuj�c� 0 jinou barvou
	mGrid->DefaultCell.isLink->Name=F->aFont->Name;
	mGrid->DefaultCell.isLink->Size=F->aFont->Size;
	mGrid->DefaultCell.isActiveLink->Name=F->aFont->Name;
	mGrid->DefaultCell.isActiveLink->Size=F->aFont->Size;
	mGrid->Create(4, 2);

	//nastaven� popisk�
	mGrid->Cells[0][0].Text="M��en� v �seku";
	mGrid->Cells[0][1].Text="Prvky";
	mGrid->Cells[1][1].Text="Vzd�lenost <a>[mm]</a>";
	mGrid->Cells[2][1].Text="IN  <a>[s]</a>";
	mGrid->Cells[3][1].Text="OUT  <a>[s]</a>";

	//barvy hlavi�ky
	mGrid->Cells[0][0].Font->Color=clBlack;
	mGrid->Cells[0][1].Font->Color=clBlack;
	mGrid->Cells[1][1].Font->Color=clBlack;
	mGrid->Cells[2][1].Font->Color=clBlack;
	mGrid->Cells[3][1].Font->Color=clBlack;

	//���ky a slu�ov�n�bu�ek
	mGrid->SetColumnAutoFit(-4);
	mGrid->Columns[0].Width=150;
	mGrid->Columns[1].Width=150;
	mGrid->Columns[2].Width=150;
	mGrid->Columns[3].Width=150;
	mGrid->MergeCells(0,0,3,0);

	//nasb�r�n� dat a po�tu ��dk�, kter� je pro n� t�eba vytvo�it
	napln_data_tabulky();
  data_tabulky_zaloha=data_tabulky;//ulo�en� do z�lohy, p�i vytvo�en� tabulky dojde k odstran�n� dat, slou�� pro p��stup k dat�m nap�. p�i zm�n� jednotek

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

	//ohrani�en�, mus� b�t po p�id�v�n� ��dk�
	if(pocet_radku>0)
	{
		mGrid->Cells[0][2].TopBorder->Width=2;
		mGrid->Cells[1][2].TopBorder->Width=2;
		mGrid->Cells[2][2].TopBorder->Width=2;
		mGrid->Cells[3][2].TopBorder->Width=2;
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
		double X=0,Y=0,uhel=0,delka=0,delka_prejezdu=0;
		TPointD in,out;
		in.x=0;in.y=0;out.x=0;out.y=0;
    bool force_prvni_radek=false;//nahrazuje podm�nku pro nuceno� vyps�n� jako prvn� ��dek
		Cvektory::TElement *E=NULL;

		Cvektory::TCesta *C=F->d.v.MAG_LASO->dalsi;
		while(C!=NULL && (C->dalsi!=NULL || (C->dalsi==NULL && C->sparovany!=C->predchozi->Element)))//p�eskakovat posled� element, pokud se jedn� o p�ichycen� (tzn. v p�edchoz�m segmentu se ji� nach�zel)
		{
      ////na�ten� C->Elementu do E, pomocn� ukazatel
			E=C->Element;

			////vypo�et delky
			//nastaven� parametr� pro v�po�et d�lky
			X=E->geo.X1;Y=E->geo.Y1;
			if(C->n==1){X=F->d.v.MAG_LASO->Element->geo.X1;Y=F->d.v.MAG_LASO->Element->geo.Y1;}
			//element v seznamu (ani prvn�, ani poslen�) pouze na�ten� d�lky z geo
			if(E->n!=MaxInt && C->n!=1)delka=E->geo.delka;
			//prvn�, nebo posledn� - linie
			else if(E->geo.typ==0)
			{
				delka=F->m.delka(X,Y,E->geo.X4,E->geo.Y4);
				delka=F->m.castPrepony(delka,E->geo.delkaPud,E->geo.HeightDepp);
			}
			//prvn�, nebo posledn� - oblouk
			else
  		{
				delka=F->m.delka(X,Y,E->geo.X4,E->geo.Y4);
				uhel=F->m.T2Aarc(E->geo.radius,delka);
				delka=F->m.R2Larc(E->geo.radius,uhel);
			}

			////v�po�et �asu
			if(C->dalsi==NULL)E=C->sparovany;//pro v�po�et �asu jsou nutn� informace z pohonu elementu, posledn� segment cesty m� Element->pohon=NULL, nutn� vz�t si pohon z sparovany!
			if(E!=NULL && E->pohon!=NULL)
			{
				//�as p�ejezdu
				in.x+=delka/E->pohon->aRD;in.y+=delka/E->pohon->aRD;
				out.x+=delka/E->pohon->aRD;out.y+=delka/E->pohon->aRD;
				//p�ipo��t�vat casy prvn�ho
				//pokud m�m na��tat dat z prvn�ho elementu, na��t�m pouze WT a latenci
				if(C->n==1 && F->d.v.MAG_LASO->Element->geo.X1==E->geo.X4 && F->d.v.MAG_LASO->Element->geo.Y1==E->geo.Y4)
				{
					if(F->d.v.vrat_druh_elementu(E)==0)
					{
						//WT - pouze do druh� slo�ky IN
						in.y+=E->WT;
						//ode�ten� z�porn�ho RT = p�i�ten� RT
						if(E->data.RT<0){in.x-=E->data.RT;in.y-=E->data.RT;out.x-=E->data.RT;out.y-=E->data.RT;}
						//latence, pokud je nastaveno
						//if(F->scGPCheckBox_meridlo_casy->Checked && E->eID==0 && E->dalsi!=NULL && E->dalsi->pohon!=NULL){in.x+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);in.y+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);out.x+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);out.y+=m.latence_mezi_stopkami(E->dalsi->pohon->aRD);}
					}
				}
				//zaokrouhlen� �asu na takt
				if((!F->scGPCheckBox_meridlo_casy->Checked && (E!=F->d.v.MAG_LASO->sparovany || (E==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))) || (F->scGPCheckBox_meridlo_casy->Checked && C->n>1) || (F->scGPCheckBox_meridlo_casy->Checked && (E!=F->d.v.MAG_LASO->sparovany || (E==F->d.v.MAG_LASO->sparovany && F->d.v.MAG_LASO->sparovany!=NULL && F->d.v.MAG_LASO->Element->geo.X1!=F->d.v.MAG_LASO->sparovany->geo.X4))))
				{
					//jedn� se o element uvnit� seznamu (n�sleduje dal��, ji� v seznamu, nen� na n�m kurzor)
					if(/*(!prichyceno || (prichyceno && E!=F->pom_element))C->dalsi!=NULL && */F->d.v.vrat_druh_elementu(E)==0)
					{
						out.y=ceil(out.x/F->d.v.PP.TT)*F->d.v.PP.TT;out.x=out.y-E->WT;
						if(E->data.RT<0){out.x-=E->data.RT;out.y-=E->data.RT;}//ode�ten� z�porn�ho RT = p�i�ten� RT
					}
				}
			}
			E=C->Element;//navr�cen� Elementu, v nasleduj�c�ch podm�nk�ch pou��v�n

			////ulo�en� dat pro tabulku
      //zar�ky, vn�mat pouze jako p�ejezd, pokud se nejedn� o posledn� segment (ukon�en�, p�ejezd - p�ejezd -> p�ejezd), nelze nezakon�it
			if(E->eID==MaxInt && C->dalsi!=NULL)delka_prejezdu+=delka;//p�i��st d�lku k d�lce p�ejezdu, �as nen� t�eba n�jak s��tat (s��t�n p�i kroc�ch v�po�tu na rozd�l od d�lky)
			//ostatn� elementy (robot�, stoky, ...)
			else
			{
		  	//ulo�en� n�zv�
		  	if(C->dalsi!=NULL)
				{
		  		if(E->eID!=MaxInt)data_tabulky+=E->name+";";else data_tabulky+=prejezd+";";//mus� b�t �e�eno p�es prom�nnou, nelze zad�vat p��mo takto: F->ls->Strings[486]
		  	}
		  	else
		  	{
					if(C->sparovany->eID!=MaxInt && C->Element->geo.X2==C->Element->geo.X3)data_tabulky+=C->sparovany->name+";";else data_tabulky+=prejezd+";";
				}
				//ulo�en� d�lky
        delka+=delka_prejezdu;//p�i�ten� p�edchoz�ho p�ejezdu, pokud existoval (pokud ne delka_prejezdu==0)
				data_tabulky+=String(F->m.round2double(delka,5)*1000)+";";
		  	//ulo�en� �asu
				//prvn� ��dek
        if((C->dalsi==NULL || (C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)) && pocet_radku==0 && E->geo.X2!=E->geo.X3)force_prvni_radek=true;//nucen� vypsan� jako prvn� ��dek (pro p�ejezdy, tabulka o jednom ��dku)
		  	if(force_prvni_radek || (C->n==1 && (F->d.v.vrat_druh_elementu(E)!=0 || (F->d.v.vrat_druh_elementu(E)==0 && F->d.v.MAG_LASO->Element->geo.X2==F->d.v.MAG_LASO->Element->geo.X3)) &&  !(C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)))//pouze pro KK elementy, br�t v potaz i p�eskakovan� segment
				{
		  		data_tabulky+="-;";
		  		if(in.x!=in.y)data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
		  		else data_tabulky+=String(F->m.round2double(in.x,2))+";";
		  	}
		  	//posledn� ��dek
				else if((C->dalsi==NULL || (C->dalsi!=NULL && C->dalsi->dalsi==NULL && C->Element==C->dalsi->sparovany)) && (F->d.v.vrat_druh_elementu(E)!=0 || (F->d.v.vrat_druh_elementu(E)==0 && C->dalsi==NULL && E->geo.X2!=E->geo.X3)))//pokud nen� p�ichyceno na S&G element
				{
		  		if(in.x!=in.y)data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
          else data_tabulky+=String(F->m.round2double(in.x,2))+";";
  	  		data_tabulky+="-;";
		  	}
		  	//ostatn�
				else
				{
		  		if(in.x!=in.y)data_tabulky+=String(F->m.round2double(in.x,2))+" - "+String(F->m.round2double(in.y,2))+";";
		  		else data_tabulky+=String(F->m.round2double(in.x,2))+";";
		  		if(out.x!=out.y)data_tabulky+=String(F->m.round2double(out.x,2))+" - "+String(F->m.round2double(out.y,2))+";";
		  		else data_tabulky+=String(F->m.round2double(out.x,2))+";";
		  	}
        //nav�en� po�tu ��dku tabulky
				pocet_radku++;
        //do�lo k ukon�en� p�ejezdu - nulovat
				delka_prejezdu=0;
			}
			////srovn�n� in a out �as� pro dal�� v�po�ty, p�i p�ejet� p�es element, se zaokrouhluje na takt pouze out v dal��m v�po�tu je t�eba in i out zaokrouhlen� na takt
			if(in.x<out.x)in.x=out.x;
			if(in.y<out.y)in.y=out.y;

  		////posun na dal�� segment
			C=C->dalsi;
		}
    //ukazatelov� z�le�itosti
		delete C;C=NULL;
    E=NULL;delete E;
	}
}
//---------------------------------------------------------------------------
//OnClick mGridu
void TForm_mereni::OnClick(long Tag,long ID,long Col,long Row)
{
	F->Memo(String(__func__)+" ",true,true);
	//F->Sv(JID);
}
//---------------------------------------------------------------------------
//MouseMove mGridu
void TForm_mereni::OnMouseMove(long Tag,long ID,int X,int Y,long Col,long Row)
{
	F->Memo(String(__func__)+" ",true,true);
	JID=0;
	int IdxCol=mGrid->GetIdxColumn(X,Y);
  int IdxRow=mGrid->GetIdxRow(X,Y);
	if(IdxCol>0 && IdxCol<4 && IdxRow==1 && mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxCol;//na dan�m ��dku a dan�ch my��ch sou�adnic�ch se nach�z� odkaz
}
//---------------------------------------------------------------------------
//klik pro zav�en� formu
void __fastcall TForm_mereni::SkrytClick(TObject *Sender)
{
	closing=true;
	mGrid->Delete();
	mGrid=NULL;
	Close();
}
//---------------------------------------------------------------------------
//indikace p�esunu formu
void __fastcall TForm_mereni::scLabel_headerMouseUp(TObject *Sender, TMouseButton Button,TShiftState Shift, int X, int Y)
{
	if(!scGPGlyphButton_pripnout->Visible && (Left!=FX || Top!=FY))
		scGPGlyphButton_pripnout->Visible=true;
}
//---------------------------------------------------------------------------
//klik na p�ipnut� formu k horn� li�t� doprava
void __fastcall TForm_mereni::scGPGlyphButton_pripnoutClick(TObject *Sender)
{
	Left=FX;
	Top=FY;
	scGPGlyphButton_pripnout->Visible=false;
}
//---------------------------------------------------------------------------
//vykreslen� tabulky
void __fastcall TForm_mereni::FormPaint(TObject *Sender)
{
	if(mGrid!=NULL)mGrid->Show(Canvas);
}
//---------------------------------------------------------------------------
