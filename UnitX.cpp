//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitX.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormX *FormX;
//---------------------------------------------------------------------------
__fastcall TFormX::TFormX(TComponent* Owner)
  : TForm(Owner)
{ //v�choz� nastaven� - p�i zobrazen� tab.element� je toti� v�dy vol�n Onchange, p�i na��t�n� hodnot do bu�ek
	// proto je nastaven input_state=NO, aby v tento moment neprob�hal ��dn� v�po�et v Onchange ud�losti
  input_state=NO;
  vstoupeno_poh=false;
  vstoupeno_elm=false;
  validace_true=false;
  editace_pohonu=false;
  //nastaven� pozic bun�k v tabulce pohonu
  rychlost=1;
  roztec_palce=2;
  nasobek_roztece=3;
  roztec_jigu=4;
  mezera_podvozky=5;
  mezera_jig1=6;
  mezera_jig2=7;
  posledni_E=NULL;
	validovany_pohon=0;
	VID="00";//zde se definuje po�et ��slic oba�en�ch ve VID
	//pokud dojde ke zm�n� po�ad� ��dku, nastavit nov� pozice zde, + p�eps�n� switche pro tabulku pohonu v OnChange + p�epsan� switche v korelace_tab_pohonu()
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
// p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
// v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
	input_state=NO;
	if(ID==6666)F->tab_knihovna_click(-100,-100,Col,Row);//p�epos�n� ud�losti
	//funkcionalita exBUTTONu v tabulce pohonu
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		if(F->PmG->Rows[3].Visible)
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			F->PmG->VisibleRow(3,false,false);
			F->PmG->VisibleRow(4,false,false);
			F->PmG->VisibleRow(5,false,false);
			F->PmG->VisibleRow(6,false,false);
			F->PmG->VisibleRow(7,false,false);
			F->PmG->VisibleRow(8,false,false);
		}
		else
    {
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			F->PmG->VisibleRow(3,true,false);
			F->PmG->VisibleRow(4,true,false);
			F->PmG->VisibleRow(5,true,false);
			F->PmG->VisibleRow(6,true,false);
			F->PmG->VisibleRow(7,true,false);
			F->PmG->VisibleRow(8,true,false);
		}
		F->nastav_focus();
		F->REFRESH(true);//mus� b�t opravdu REFRESH() cel�ho formu + mGridu
		F->PmG->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
	}

	if(ID!=9999 && Col==4 && Row==0)//klik na glyphbutton v pohonov� tabulce
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		if(E->eID==200 || E->eID==300)
		{
			if(E->mGrid->Rows[3].Visible)//budu skr�vat
			{
        //zmen�en� tabulky, �spora m�sta
				E->mGrid->Columns[0].Width-=20;
				E->mGrid->Columns[1].Width-=20;
				E->mGrid->Columns[2].Width-=40;
				E->mGrid->VisibleRow(3,false,false);    //E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
				E->mGrid->VisibleRow(4,false,false);
				if(E->mGrid->Rows[5].Visible)
				{
					E->mGrid->VisibleRow(5,false,false);
					E->mGrid->VisibleRow(6,false,false);
					E->mGrid->VisibleRow(7,false,false);
					E->mGrid->VisibleRow(8,false,false);
					E->mGrid->VisibleRow(9,false,false);
					E->mGrid->VisibleRow(10,false,false);
					E->mGrid->VisibleRow(11,false,false);
				}
				E->mGrid->exBUTTONVisible=false;
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkDownArrow;
			}
			else//budu zobrazovat
			{
				//zv�t�en� tabulky na p�vodn� rozm�ry
				E->mGrid->Columns[0].Width+=20;
				E->mGrid->Columns[1].Width+=20;
				E->mGrid->Columns[2].Width+=40;
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->VisibleRow(3,true,false);
				E->mGrid->VisibleRow(4,true,false);
				if(E->mGrid->exBUTTON->GlyphOptions->Kind==scgpbgkUpArrow)
				{
        	E->mGrid->VisibleRow(5,true,false);
					E->mGrid->VisibleRow(6,true,false);
					E->mGrid->VisibleRow(7,true,false);
					E->mGrid->VisibleRow(8,true,false);
					E->mGrid->VisibleRow(9,true,false);
					E->mGrid->VisibleRow(10,true,false);
					E->mGrid->VisibleRow(11,true,false);
				}
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkUpArrow;
			}
			F->nastav_focus();
			F->REFRESH(true);
		}
		E=NULL;delete E;
  }

	if(ID!=9999&&Row==-2)
	//funkcionalita exBUTTONu v tabulk�ch element�
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		AnsiString Hint=E->mGrid->exBUTTON->Hint;//F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		switch(E->eID)
		{
			case 0://stop stanice
			{
      	if(E->mGrid->Rows[4].Visible)
	    	{
	    		E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(4,false,false);//nep�ekreslovat
					E->mGrid->VisibleRow(5,false,false);
					E->mGrid->VisibleRow(6,false,false);
	    	}
	    	else
	    	{
					double rotaceJ=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E);
					if(E->mGrid->Cells[2][6].Text==1 && (F->m.Rt90(rotaceJ)==E->geo.orientace || F->m.Rt90(rotaceJ)==F->m.Rt90(E->geo.orientace+180)) && F->d.v.PP.delka_podvozek<F->m.UDJ(rotaceJ))
					E->mGrid->Note.Text="P�i 0� �i 180� rotaci jigu a po�tu nastaven�ch pozic 1 nejsou zobrazeny potenci�ln� pozice";
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(4,true,false);//nep�ekreslovat
					E->mGrid->VisibleRow(5,true,false);
					E->mGrid->VisibleRow(6,true,false);
				}
				F->design_element(E,false);
			}break;
			case 3:case 9:case 13:case 17:case 103:case 107:
			{
				if(E->mGrid->Rows[6].Visible)
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(6,false,false);
					E->mGrid->VisibleRow(7,false,false);
				}
				else
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(6,true,false);
					E->mGrid->VisibleRow(7,true,false);
				}
				F->design_element(E,false);//nutn� nov� slou�en� bu��k
			}break;
			case 5://KK oto�
			{
				if(E->mGrid->Rows[4].Visible)
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(4,false,false);
					E->mGrid->VisibleRow(5,false,false);
				}
				else
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(4,true,false);
					E->mGrid->VisibleRow(5,true,false);
				}
			}break;
			case 200://p�ed�vac� m�sto
			case 300://v�hybka
			{
				if(E->mGrid->Rows[5].Visible)
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(5,false,false);
					E->mGrid->VisibleRow(6,false,false);
					E->mGrid->VisibleRow(7,false,false);
					E->mGrid->VisibleRow(8,false,false);
					E->mGrid->VisibleRow(9,false,false);
					E->mGrid->VisibleRow(10,false,false);
					E->mGrid->VisibleRow(11,false,false);
				}
				else
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(5,true,false);
					E->mGrid->VisibleRow(6,true,false);
					E->mGrid->VisibleRow(7,true,false);
					E->mGrid->VisibleRow(8,true,false);
					E->mGrid->VisibleRow(9,true,false);
					E->mGrid->VisibleRow(10,true,false);
					E->mGrid->VisibleRow(11,true,false);
				}
			}break;
		}
		F->nastav_focus();
		F->REFRESH(true);//mus� b�t opravdu REFRESH() cel�ho formu + mGridu
		E->mGrid->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
		E=NULL;delete E;
	}
	if(F->OBJEKT_akt!=NULL && F->editace_textu)F->smaz_kurzor();
	//uvoln�n� inputu
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(ID==9999&&Row==1)validace_true=true;//spu�t�n� validace a� po kliku
	//po kliku do vykreslen� tabulky lze obnovit ud�lost OnChange
	if(ID==9999)vstoupeno_poh=true;
	else vstoupeno_elm=true;
	if(ID==9999)editace_pohonu=true;
	else editace_pohonu=false;
	//korelace
	odstranit_korelaci(false);//nesta�� volat na �rovni form1 onclick, mus� se volat i p�i p�eklik�v�n� mezi komponenty
	if(ID==9999 && Row>=1 && F->PmG->Rows[3].Visible)//pokud je kliknuto do tabulky pohonu, podle bu�ky vyzn�� bu�ky, kter� budou zm�nou ovlivn�n�
		korelace_tab_pohonu(Row);//spu�t�no pouze v p��pad� KK tabulky
	if(ID==9999 && Row>=1)korelace_tab_pohonu_elementy();//p�i S&G re�imu je st�le nutn� zobrazit korelaci v elementech
	if(ID!=9999 && F->OBJEKT_akt->pohon!=NULL)//korelace v tabulk�ch element�
	korelace_v_elementech(ID,Col,Row);
}
//---------------------------------------------------------------------------
//zpracov�n� onchange ud�losti - INPUT, v�po�et a OUTPUT zp�t do ovlivn�n� bu�ky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING&&ID!=9999&&vstoupeno_elm&&!editace_pohonu)
	{
		F->Timer_neaktivity->Enabled=false;//vypnut� timeru neaktivity, pokud doch�z� k OnChange rychle za sebou nestpust� timer refresh
    //deklarace ukazatel�
		Cvektory::TElement *E=posledni_E;//pokud edituji stejn� element znova nen� pot�eba si jej znova vracet
		Cvektory::TCesta *c=posledni_c;
		//pokud se jedn� o jin� element, znova naplnit E
		if(posledni_E==NULL || posledni_E!=NULL && posledni_E->mGrid!=NULL && posledni_E->mGrid->ID!=ID || posledni_E!=NULL && posledni_E->mGrid==NULL)
		{
			E=vrat_element_z_tabulky(ID);//ne v�dy je ID tabulky stejn� jako n robota, proto nutn� hledat ID=ID ne ID=n
	  	//nahr�n� aktu�ln�ch dat do ukazatele
			if(F->d.v.ZAKAZKA_akt!=NULL && F->d.v.ZAKAZKA_akt->n!=0)//pokud pracuji v n�jak� zak�zce
	  	{
				c=F->d.v.vrat_segment_cesty(F->d.v.ZAKAZKA_akt,E);
				if(c!=NULL)E->data=c->data;//p�eps�n� aktu�ln�ch dat ze zak�zky do elementu
			}
		}
		F->posledni_editovany_element=E;
		////v�po�ty
		switch(E->eID)
		{
			case 0: //stop
			{
				if(Row==3)//zm�na max. WT stop
				{
					input_state=WT;//nastaveni stavu
					E->data.WTstop=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->data.pocet_voziku=F->m.WT2V(E->data.WTstop,F->d.v.PP.TT);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][6].Text=F->m.round2double(E->data.pocet_voziku,3);//OUTPUT
					//p�epo�et RT
					F->d.v.reserve_time(E,c);
				}
				if(Row==6)//max. po�et voz�k�
				{
					input_state=P_VOZ;//nastaveni stavu
					E->data.pocet_voziku=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);//INPUT
					E->data.WTstop=F->m.V2WT(E->data.pocet_voziku,F->d.v.PP.TT);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][3].Text=F->m.round2double(F->outPT(E->data.WTstop),3);//OUTPUT
					//p�epo�et RT
					F->d.v.reserve_time(E,c);
				}
				//dod�lat pln�n� pam�ti p�i editaci bun�k
			} break;
			case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinu�ln�)
			{
				if(Row==1)// editace PT
				{
					input_state=PT; //nastaveni stavu
					E->data.PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->data.PT1!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/E->data.PT1;//ulo�en� do pam�ti + v�po�et
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//nutn� o�et�en� podm�nkou, v p��pad� editace textu, kter� je cel� ozna�en� ud�lost OnChange spu�t�na 2x
			  		//zm�na v ostatn�ch elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG, mus� b�t refresh, PmG refre� obsluhuje druh� case v OnChange
					}
				}
				if(Row==2)// eidtace LO
				{
					input_state=LO; //nastaveni stavu
					E->data.LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->data.PT1=E->data.LO1/F->OBJEKT_akt->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->data.PT1),3); //OUTPUT
				}
				if(Row==3)// eidtace LO_pozice
				{
					input_state=LO2; //nastaveni stavu
					E->data.LO_pozice=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//pouze ulo�en� do dat
				}
				if(Row==4)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->data.PD=E->mGrid->getCombo(1,Row)->ItemIndex;//pouze ulo�en� do dat
				}
			} break;
			case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanic�
			{
				if(Row==1)//editace PT
				{
					input_state=PT;//nastaveni stavu
					E->data.PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
			} break;
			case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivn� oto��
			{
				if (Row==1)//editace PT1
				{
					input_state=PT;//nastaveni stavu
					E->data.PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
          //o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->data.PT1!=0)
					{
			  		//ovlivn�n� aRD
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/E->data.PT1;
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
			  		//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
			  		E->data.PT2=F->m.PT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][8].Text=F->m.round2double(F->outPT(E->data.PT2),3);
						//zm�na v ostatn�ch elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
				}
				if (Row==2)//editace LO1
				{
					input_state=LO;//nastaveni stavu
					E->data.LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->data.PT1=E->data.LO1/F->OBJEKT_akt->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->data.PT1),3);//OUTPUT
				}
				if (Row==3)//editace COMBO
				{
					input_state=COMBO;//nastaveni stavu
					switch(E->mGrid->getCombo(3,3)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
					//aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[3][6].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[3][7].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//p�epo��t�n� z�n ot��en� n�sleduj�c�ch element�
				}
				//Row = 4 nen� u�ivatelsky upraviteln�
				if (Row==5)//editace d�lky oto�e
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PTotoc=E->OTOC_delka/F->OBJEKT_akt->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
          //aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[3][6].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[3][7].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
				if (Row==8)//editace PT2
				{
					input_state=PT;//nastaveni stavu
					E->data.PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					//o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->data.PT2!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO2/E->data.PT2;//ulo�en� do pam�ti + v�po�et
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			   		E->data.PT1=F->m.PT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
						E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zm�na v ostatn�ch elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
				}
				if (Row==9)//editace LO2
				{
					input_state=LO2;//nastaveni stavu
					E->data.LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->data.PT2=E->data.LO2/F->OBJEKT_akt->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->data.PT2),3);//OUTPUT
				}
				if(Row==10)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->data.PD=E->mGrid->getCombo(3,Row)->ItemIndex;//pouze ulo�en� do dat
				}
			} break;
			case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivn� oto�� (resp. s oto�� a stop stanic�)
			{
				if(Row==1)//zm�na PT1
				{
					input_state=PT;//nastaveni stavu
					E->data.PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
				if (Row==2)//COMBO
				{
					input_state=COMBO;//nastaveni stavu
					switch(E->mGrid->getCombo(1,2)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
				}
				if(Row==3)//zm�na PTo
				{
					input_state=PTotoc;//nastaveni stavu
					E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
				if(Row==4)//zm�na PT2
				{
					input_state=PT2;//nastaveni stavu
					E->data.PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
				if(Row==6)//zm�na WT
				{
					input_state=WT;//nastaveni stavu
					E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
			} break;
			case 5://oto� pasivn�
			{
				if (Row==1)//COMBO
				{
					input_state=COMBO;//nastaveni stavu
					switch(E->mGrid->getCombo(2,1)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
					//aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[2][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[2][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//p�epo��t�n� z�n ot��en� n�sleduj�c�ch element�
				}
				if (Row==2)//zde se upravuje pouze d�lka
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->PTotoc=E->OTOC_delka/F->OBJEKT_akt->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
					//aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[2][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[2][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
			} break;
			case 6://oto� aktivn� (resp. oto� se stop stanic�)
			{
				if (Row==1)//COMBO
				{
					input_state=COMBO;//nastaveni stavu
					switch(E->mGrid->getCombo(1,1)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
				}
				if (Row==2)
				{
					input_state=PTotoc;//nastaveni stavu
					E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//E->OTOC_delka=F->OBJEKT_akt->pohon->aRD*E->PTotoc;//ulo�en� do pam�ti + v�po�et
					//E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outDO(E->OTOC_delka),3);//OUTPUT
					F->d.v.reserve_time(E,c);
				}
			} break;
      case 200://p�ed�vac� m�sto
			case 300://v�hybka
			{
				posledni_E=E;//p�ed�asn� ulo�en� posledn�ho editovan�ho elementu
				switch(Row)
				{
					//nastavov�n� pohonu
					case 2:
					{
						input_state=COMBO;//nastaveni stavu
						if(E->eID==300)prirazeni_pohohonu_vetvi(E,Col);//pro v�hybku
						else prirazeni_pohohonu_PM(E,Col);//pro PM
						break;
					}
					//zm�na aRD
					case 3:
					{
						input_state=aRD;
						//na�ten� aRD z editu
						double aRD=F->inaRD(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//ur�en� c�lov�ho pohonu
						Cvektory::TPohon *p=F->d.v.vrat_pohon(E->mGrid->getCombo(Col,2)->ItemIndex);
						//ulo�en� do pohonu
						F->OBJEKT_akt->pohon=p;//ulo�en� aktu�ln� editovan�ho pohonu
						p->aRD=aRD;
						//v�po�et ovlivn�n�ch dat
						p->Rz=F->m.Rz(p->aRD);
						p->Rx=F->m.Rx(p->aRD,p->roztec);
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o dal�� pohon
						p=NULL;delete p;
						//akticave a deaktivace comba pro zm�nu typu k�t
						if(aRD>0)F->scGPComboBox_prepinacKot->Enabled=true;
						else//pokud je zad�na nulov� rychlost k�ty mus� b�t p�epnuty do m�du d�lky
						{
							F->scGPComboBox_prepinacKot->Enabled=false;
							if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
						}
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
          //zm�na rozte�e palce
					case 6:
					{
            input_state=R;
						//na�ten� R z editu
						double R=F->inR(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//ur�en� c�lov�ho pohonu
						Cvektory::TPohon *p=F->d.v.vrat_pohon(E->mGrid->getCombo(Col,2)->ItemIndex);
						//ulo�en� do pohonu
						F->OBJEKT_akt->pohon=p;
						p->roztec=R;
						//v�po�et ovlivn�n�ch dat
						p->Rz=F->m.Rz(p->Rx,p->roztec);
						p->aRD=F->m.RD(p->Rz);
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o dal�� pohon
						p=NULL;delete p;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					//zm�na n�sobku rozte�e
					case 7:
					{
            input_state=Rx;
			    	//na�ten� Rx z editu
						double Rx=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
			    	//ur�en� c�lov�ho pohonu
						Cvektory::TPohon *p=F->d.v.vrat_pohon(E->mGrid->getCombo(Col,2)->ItemIndex);
						//ulo�en� do pohonu
						F->OBJEKT_akt->pohon=p;
						p->Rx=Rx;
						//v�po�et ovlivn�n�ch dat
						p->Rz=F->m.Rz(p->Rx,p->roztec);
						p->aRD=F->m.RD(p->Rz);
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o dal�� pohon
						p=NULL;delete p;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
				}
			} break;
		}
		if(E!=NULL && E->mGrid!=NULL)E->mGrid->Refresh();//refresh aktu�ln� upravovan� tabulky
		posledni_E=E;//ulo�en� posledn�ho editovan�ho elementu
		posledni_c=c;//ulo�en� posledn�ho editovan�ho segmentu cesty
		if(F->d.v.ZAKAZKA_akt!=NULL && F->d.v.ZAKAZKA_akt->n!=0 && c!=NULL)c->data=E->data;//navr�cen� p�epo��tan�ch dat do zak�zky
		E=NULL;delete E;
		c=NULL;delete c;
		vstoupeno_elm=true;
		input_state=NOTHING;F->Timer_neaktivity->Enabled=true;//uvoln�n� stavu + zapnuti timeru neaktivity, pokud dokon�� �asov�n� spust� REFRESH
		F->nahled_ulozit(true);//prob�hla zm�na tabulky -> je d�vod ulo�it n�hled
	}
	if(input_state==NOTHING&&ID==9999&&vstoupeno_poh)
	{
		F->Timer_neaktivity->Enabled=false;//vypnut� timeru neaktivity, pokud doch�z� k OnChange rychle za sebou nestpust� timer refresh
		posledni_E=NULL;
		posledni_c=NULL;
		switch(Row)
		{
			case 0://v�b�r pohonu
			{
				input_state=COMBO;//nastaveni stavu
				prirazeni_pohonu_defTab();
			}break;
			case 1://aktu�ln� rychlost, aRD
			{
				input_state=aRD;
				//na�ten� aRD z editu
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text));
				//ulo�en� do spoj�ku
				F->OBJEKT_akt->pohon->aRD=aRD;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
				zmena_aRD();
				//akticave a deaktivace comba pro zm�nu typu k�t
				if(aRD>0)F->scGPComboBox_prepinacKot->Enabled=true;
				else//pokud je zad�na nulov� rychlost k�ty mus� b�t p�epnuty do m�du d�lky
				{
					F->scGPComboBox_prepinacKot->Enabled=false;
					if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
				}
			}break;
			case 4://rozte�, R
			{
				input_state=R;
				//na�ten� R z editu
				double R=F->inR(F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text));
				//ulo�en� do spoj�ku
				F->OBJEKT_akt->pohon->roztec=R;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
				zmena_R();
			}break;
			case 5://Rx
			{
				input_state=Rx;
				//na�ten� Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text);
				//ulo�en� do spoj�ku
				F->OBJEKT_akt->pohon->Rx=Rx;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
				zmena_Rx();
			}break;
			default:break;
		}
		if(F->PmG!=NULL)F->PmG->Refresh();//v pr�b�hu m��e doj�t k jej�mu odstran�n�
		input_state=NOTHING;F->Timer_neaktivity->Enabled=true;
		F->nahled_ulozit(true);
	}
}
//---------------------------------------------------------------------------
void TFormX::OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key)
{
//	if(Row==0&&Col==0&&ID!=9999)//nutn� o�et�en� aby sem nepadaly v�ci z �e�en� v OnChange
//	{
//		if((Key==VK_ESCAPE||Key==VK_RETURN)&&F->pom_element_temp!=NULL)//stisknut� ESC nebo ENTER
//		{    //p�i esc zru�it ozna�en� textu
//			if(Key==VK_ESCAPE)
//			{
//				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text;//pokud je stisknuto ESC, vr�t� p�vodn� n�zev
//				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=F->pom_element_temp->mGrid->Cells[0][0].Text.Length();//zamez� selectov�n� cel�ho textu p�i stisku ESC
//			}
//			F->index_kurzoru=9999;//mus� b�t p��tomno, zabra�uje smaz�n� editu hned po vytvo�en�, timer vol�n i z OnChange
//			F->Timer2->Enabled=true;//sma�e edit a ulo�� n�zev (p�vodn� nebo zm�n�n�),edit mus� b�t smaz�n se spo�den�m, jinak pam�ov� chyba
//		}
//		else
//		{
//			//�e�eno takto z d�vodu, �e v okamziku stisknut� kl�vesy je v editu text bez p�i�ten� kl�vesy, muselo by b�t implementov�no do OnChange, aby fungovalo norm�ln�
//			if(Key==VK_BACK)//stisknuto backspace
//   			F->pom_element_temp->name=F->pom_element_temp->name.SubString(1,F->pom_element_temp->name.Length()-1);
//			else//ostatn� kl�vesy
//				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text+AnsiString(Key);
//			//pokud se jend� o stopku, je mo�n� editovat pouze kone�n� ��slo
//			if(F->pom_element_temp->eID==0&&F->pom_element_temp->name.Length()<=4)
//			{
//				Key=0;//nutn�! OnKeyPress je vol�na 2x
//				F->pom_element_temp->mGrid->Cells[0][0].Text="Stop ";//nahrazen� toho co je v editu
//				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=5;//nastaven� kurzoru na konec editu
//				MessageBeep(0);//ozn�men� u�ivateli
//			}
//			F->REFRESH(true);
//		}
//	}
	if((Row==5 && ID==9999) || (ID!=9999 && Col>=3 && Row==7))//slou�� pro nepovolen� zaps�n� desetin� ��rky do editu Rx
	{
		if(AnsiString(Key)==F->ms.get_locale_decimal())
		{
			Key=0;
			MessageBeep(0);
		}
	}
}
//---------------------------------------------------------------------------
//slou�� pro spu�t�n� funkcionality ctrl+z a ctrl+y pokud je focus na mGridu
void TFormX::OnKeyDown(long Tag,unsigned long Col,unsigned long Row,WORD &Key,TShiftState Shift)
{
	if(Shift.Contains(ssCtrl) && Key==89)//ctrl+y
	{
		if(F->d.v.pozice_data!=F->d.v.pocet_kroku && F->d.v.pozice_data!=0)//pokud nejsem na konci
		{
			vstoupeno_elm=false;vstoupeno_poh=false;
			F->DrawGrid_knihovna->SetFocus();
			F->d.v.pozice_data+=1;
			F->d.v.nacti_z_obrazu_DATA();
			F->REFRESH();
		}
	}
	if(Shift.Contains(ssCtrl) && Key==90)//ctrl+z
	{
		if(F->d.v.pozice_data!=1)//pokud nejsem na konci
		{
			vstoupeno_elm=false;vstoupeno_poh=false;
			F->DrawGrid_knihovna->SetFocus();
			if(F->d.v.pozice_data==0)F->d.v.pozice_data=F->d.v.DATA->predchozi->predchozi->n;
			else F->d.v.pozice_data-=1;
			F->d.v.nacti_z_obrazu_DATA();
			F->REFRESH();
		}
	}
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rychlosti
void TFormX::zmena_aRD(Cvektory::TElement *mimo_element)
{
	//p�epo�et parametr� pohonu
	F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
	F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
	if(F->PmG!=NULL)
	{
		//propo�ty v tabulce pohonu
		aktualizace_tab_pohonu();
		//validace prob�hne a� po uplynut� timeru
		//if(F->Akce==F->NIC)validace_aRD();//validace pouze v kontinu�ln�m re�imu kabiny
		//F->PmG->Refresh();, nen� t�eba provede OnChange
	}
	//propo�ty v tabulk�ch element�
	aktualizace_tab_elementu(mimo_element);
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rozte�e
void TFormX::zmena_R ()
{
  //p�epo�et parametr� pohonu
	F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
	F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz); //prohozen� po�ad� z d�vodu, �e druh� v�po�et pot�ebuje aktualizovaonu honotu prvn�ho v�po�tu
	if(F->PmG!=NULL)
	{
		//p�epo�et hodnot v tabulce pohonu
		aktualizace_tab_pohonu();
		//p�epo�et hodnot v elementech
		aktualizace_tab_elementu();
	}
	//////test validace rozte�e                                                                                                                            //0 = z�kladn� jednotky
//	TTextNumber cislo=F->d.v.rVALIDACE(5,F->OBJEKT_akt->pohon->n,F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->Rz,F->OBJEKT_akt->pohon->Rx,0,0,0);//F->aRDunit,F->Runit,F->Rzunit);
//	F->PmG->ShowNote(cislo.text);
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou Rx
void TFormX::zmena_Rx ()
{
	//p�epo�et parametr� pohonu
	F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
	F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz);
	if(F->PmG!=NULL)
	{
		//p�epo�et hodnot v tabulce pohonu
		aktualizace_tab_pohonu();
  	//p�epo�et hodnot v elementech
		aktualizace_tab_elementu();
	}
}
//---------------------------------------------------------------------------
//p�epo�et v tabulk�ch element� po zm�n� parametr� v tabulce pohonu
void TFormX::aktualizace_tab_elementu (Cvektory::TElement *mimo_element)
{
	unsigned int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->OBJEKT_akt->pohon!=NULL && E->pohon->n==F->OBJEKT_akt->pohon->n)//p�esko�� mimo_element a hlavi�ku, poze p�epo�et element�m kter� maj� stejn� pohon jako aktu�ln� editovan� pohon
		{
			switch(E->eID)
			{
				case 0:
				{
					//na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//p�epo�ty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[2][4].Text=F->m.round2double(F->outPT(E->WT),3);
					F->d.v.reserve_time(E);
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinu�ln�
				{
          //na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//p�epo�ty
					E->data.PT1=F->m.PT(E->data.LO1,aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanic�
				{
					//na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//p�epo�ty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT),3);
					F->d.v.reserve_time(E);
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivn� oto��
				{
					//na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//p�epo�ty
					E->data.PT1=F->m.PT(E->data.LO1,aRD);
					E->mGrid->Cells[3][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[3][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->data.PT2=F->m.PT(E->data.LO2,aRD);
					E->mGrid->Cells[3][8].Text=F->m.round2double(F->outPT(E->data.PT2),3);
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivn� oto��
 				{
					//na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//p�epo�ty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT),3);
					F->d.v.reserve_time(E);
				}break;
 				case 5://oto� pasivn�
				{
          //na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//p�epo�ty
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[2][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://oto� aktivn�
				{
					//na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//p�epo�ty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);//d�le�it� pro v�po�et RT, nezobrazuje se
					E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					F->d.v.reserve_time(E);
				}break;
				case 200://p�ed�vac� m�sto
				case 300://v�hybla
				{
					update_hodnot_vyhybky_PM(E);//provede zobrazen� hodnot pohonu a v�po��t� nov� hodnoty mezer, provede aktualizaci WT
					validace_RD(E);
				}break;
			}
			if(input_state==COMBO)F->set_enabled_mGrid(E);//pouze p�i zm�n� pohonu, jinak zbyte�n�
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}

	//p�edchoz� PM
	if(F->predchozi_PM!=NULL && F->predchozi_PM!=mimo_element)
	{
		update_hodnot_vyhybky_PM(F->predchozi_PM);
		F->predchozi_PM->mGrid->Refresh();
	}

	//ukazatelov� z�le�itosti
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//Napln� hodnoty kter� z�vis� na pohonu 0
void TFormX::aktualizace_tab_elementu_pOdebran ()
{
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && E->pohon==NULL)//p�esko�� hlavi�ku + p�enastav� pouze elementy s odstran�n�m pohonem
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[2][3].Text=0;
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Text=0;
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanic�
				{
					//validace
					E->mGrid->Cells[1][2].Text=0;
					E->mGrid->Cells[1][3].Text=0;
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivn� oto��
				{
					E->mGrid->Cells[3][1].Text=0;
					E->mGrid->Cells[3][4].Text=0;
					E->mGrid->Cells[3][8].Text=0;
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivn� oto��
				{
					//validace
					E->mGrid->Cells[1][5].Text=0;
					E->mGrid->Cells[1][6].Text=0;
				}break;
				case 5://oto� pasivn�
				{
					E->mGrid->Cells[2][3].Text=0;
				}break;
				case 6://oto� aktivn�
				{
					E->mGrid->Cells[1][3].Text=0;
				}break;
				case 301://spojka
				{
					E->mGrid->Cells[1][2].Text=0;
				}break;
				case 200:
				case 300:
				{
        	update_hodnot_vyhybky_PM(E);//provede zobrazen� hodnot pohonu a v�po��t� nov� hodnoty mezer, provede aktualizaci WT
        }break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}

	//p�edchozi PM
	if(F->predchozi_PM!=NULL)
	{
		update_hodnot_vyhybky_PM(F->predchozi_PM);
		F->predchozi_PM->mGrid->Refresh();
	}

	//ukazatelov� z�le�itosti
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//aktualizje data v mGridu podle dat elementu
void TFormX::aktualizace_tab_pohonu()
{
	if(F->PmG!=NULL)
	{
  	if(F->OBJEKT_akt->pohon!=NULL)
  	{
			if(input_state!=aRD && posledni_E==NULL || posledni_E!=NULL)F->PmG->Cells[3][1].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
			F->PmG->Cells[3][2].Text=AnsiString(F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->rychlost_do),3));
			if(input_state!=R && posledni_E==NULL || posledni_E!=NULL)F->PmG->Cells[3][4].Text=F->m.round2double(F->outR(F->OBJEKT_akt->pohon->roztec),3);
			if(input_state!=Rx && posledni_E==NULL || posledni_E!=NULL)F->PmG->Cells[3][5].Text=F->m.round2double(F->OBJEKT_akt->pohon->Rx,3);
			F->PmG->Cells[3][3].Text=F->m.round2double(F->outR(F->OBJEKT_akt->pohon->Rz),3);
			F->PmG->Cells[3][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
			F->PmG->Cells[3][7].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[3][8].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else
		{
			F->PmG->Cells[3][1].Text=0;
			F->PmG->Cells[3][2].Text=0;
			F->PmG->Cells[3][3].Text=0;
			F->PmG->Cells[3][4].Text=0;
			F->PmG->Cells[3][5].Text=0;
			F->PmG->Cells[3][6].Text=0;
			F->PmG->Cells[3][7].Text=0;
			F->PmG->Cells[3][8].Text=0;
		}

		//aktualizace komponent
		F->aktualizace_tab_pohon(false,false,true);
	}
}
//---------------------------------------------------------------------------
//highlightovan� bu��k tabulky pohonu
void TFormX::korelace_tab_pohonu(int Row)
{
	if(F->PmG!=NULL)
	{
  	switch(Row)
  	{
  		case 1: //zmena aRD
			{
				F->PmG->Cells[3][5].Highlight=true; //rx
				F->PmG->Cells[3][3].Highlight=true; //roztec jig�
				F->PmG->Cells[3][6].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[3][7].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[3][8].Highlight=true;
			}break;
			case 4: //zmena rozte�e R
			{
				F->PmG->Cells[3][1].Highlight=true; //aRD
				F->PmG->Cells[3][3].Highlight=true; //roztec jig�
				F->PmG->Cells[3][6].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[3][7].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[3][8].Highlight=true;
  		}break;
			case 5: //zm�na Rx
			{
				F->PmG->Cells[3][1].Highlight=true; //aRD
				F->PmG->Cells[3][3].Highlight=true; //roztec jig�
				F->PmG->Cells[3][6].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[3][7].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[3][8].Highlight=true;
  		}break;
  	}
  	F->PmG->Refresh();
	}
	korelace_tab_pohonu_elementy();
}
//---------------------------------------------------------------------------
//star� se o highlitov�n� pol��ek, kter� souvis� s m�n�nou hodnotou (elementy)
void TFormX::korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element)
{
	unsigned int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->OBJEKT_akt->pohon->n==E->pohon->n)
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[2][2].Highlight=true;
					E->mGrid->Cells[2][3].Highlight=true;
					E->mGrid->Cells[2][4].Highlight=true;
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Highlight=true;
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanic�
				{
					//validace
					E->mGrid->Cells[1][2].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivn� oto��
				{
					E->mGrid->Cells[3][1].Highlight=true;
					E->mGrid->Cells[3][4].Highlight=true;
					E->mGrid->Cells[3][8].Highlight=true;
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivn� oto��
				{
					//validace
					E->mGrid->Cells[1][5].Highlight=true;
					E->mGrid->Cells[1][6].Highlight=true;
				}break;
				case 5://oto� pasivn�
				{
					E->mGrid->Cells[2][3].Highlight=true;
				}break;
				case 6://oto� aktivn�
				{
					//E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}break;
			}
			E->mGrid->Refresh();
		}
		//PM zvl�t, neplat� na n� podm�nka E->pohon == OBJEKT_akt->pohon, proto�e maj� pohony 2
		if(E->n>0 && E->n!=n && mimo_element!=E && (E->eID==200 || E->eID==300))
		{
			//zji�t�n�, zda m� element stejn� pohon jako upravovan� pohon, pokud ano zjist� v jak�m je sloupci
			long Col=0;
			if(E->pohon==F->OBJEKT_akt->pohon)Col=3;
			if(E->eID==300 && E->predchozi2!=E->dalsi2 && E->dalsi2->pohon==F->OBJEKT_akt->pohon)Col=4;
			if(E->eID==200)
			{
				Cvektory::TElement *e_pom=E->dalsi;
				if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
				if(e_pom->pohon==F->OBJEKT_akt->pohon)Col=4;
				e_pom=NULL;delete e_pom;
			}
			//pokud byl zji�t�n sloupec, pokra�uje
			if(Col!=0)
			{
				if(F->prohodit_sloupce_PM(E))
				{
					if(Col==4)Col=3;
					else Col=4;
				}
				E->mGrid->Cells[Col][3].Highlight=true;
				E->mGrid->Cells[Col][7].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
			}
    }
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//korelace uvnit� element�
void TFormX::korelace_v_elementech(long ID,long Col,long Row)
{
	Cvektory::TElement *E=vrat_element_z_tabulky(ID);
	switch(E->eID)
	{
		case 0://stopka
		{
			if(Row==3)E->mGrid->Cells[2][6].Highlight=true;
			if(Row==6)E->mGrid->Cells[2][3].Highlight=true;
		}
		case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinu�ln�)
		{
			if(Row==1){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1);}korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
		case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanic�
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivn� oto��
		{
			if (Row==1){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1);}korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==5){E->mGrid->Cells[1][Row-1].Highlight=true;E->mGrid->Cells[1][Row+1].Highlight=true;E->mGrid->Cells[1][Row+2].Highlight=true;}
			if (Row==8){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1);}korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==9)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
		case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivn� oto�� (resp. s oto�� a stop stanic�)
		{
			if(Row==1)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==3)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==4)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==6)E->mGrid->Cells[1][5].Highlight=true;
		} break;
		case 5://oto� pasivn�
		{
			if (Row==2){E->mGrid->Cells[2][Row+1].Highlight=true;E->mGrid->Cells[2][Row+2].Highlight=true;E->mGrid->Cells[2][Row+3].Highlight=true;}
		} break;
		case 6://oto� aktivn� (resp. oto� se stop stanic�)
		{
			if (Row==2){/*E->mGrid->Cells[1][Row-1].Highlight=true;*/E->mGrid->Cells[1][Row+1].Highlight=true;}
		} break;
		case 200://p�ed�vac� m�sto
		case 300://v�hybka
		{
			if(Row==3)
			{
				E->mGrid->Cells[Col][7].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
				korelace_tab_pohonu_elementy(E);//ozna�en� v ostatn�ch tabulk�ch
			}
			if(Row==6 || Row==7)
			{
				E->mGrid->Cells[Col][3].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
				korelace_tab_pohonu_elementy(E);//ozna�en� v ostatn�ch tabulk�ch
			}
			break;
    }
	}
	E->mGrid->Refresh();
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//odstran� highlight na v�ech tabulk�ch
void TFormX::odstranit_korelaci(bool predat_focus)
{
	if(predat_focus)
		F->DrawGrid_knihovna->SetFocus();//po kliku mimo z�st�val focus po��d na editu
	if(F->PmG!=NULL)F->PmG->unHighlightAll();
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0)
			E->mGrid->unHighlightAll();
		E=E->dalsi;
	}
	if(F->predchozi_PM!=NULL)F->predchozi_PM->mGrid->unHighlightAll();
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//validace rychlosti p�i jej� zm�n�
void TFormX::validace_aRD(bool pouze_rozmezi)
{
	//kontrola p�i KK stavu objektu, validace v�eho
	if(F->PmG!=NULL && F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->rezim!=0)
	{
    //smaz�n� p�edchoz� validace z VID
		zapisVID(0,1);//pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
		validovany_pohon=0;//ulo�it pohon na kter�m se to stalo

		AnsiString jednotky;
  	if(F->aRDunit==0)jednotky="[m/s]";
  	else jednotky="[m/min]";
		bool mimo_rozmezi=false;
		unsigned int pro_pohon=0;
  	dopRD=0;
		//kontrola zda je zadan� hodnota v rozmez�
		if(F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do)) mimo_rozmezi=false;
		else mimo_rozmezi=true;
  	//zadan� rychlost je mimo rozsah
  	if(mimo_rozmezi && F->OBJEKT_akt->pohon->aRD > 0)
  	{
			//if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
  		F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
			//povolit_zakazat_editaci(false);
			pro_pohon=F->OBJEKT_akt->pohon->n;//ulo�en� pro kter� pohon plat� validace
		}
		if(!mimo_rozmezi && F->PmG->Note.Text!=""){F->PmG->ShowNote("",F->d.clError,14);/*povolit_zakazat_editaci(true);*/}
  	// nutn� o�et�en� pro obdob� zad�v�n�/psan�
  	if(F->OBJEKT_akt->pohon->aRD > 0 && !pouze_rozmezi)
  	{
			//v�po�et doporu�en� rychosti
			double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
			double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
			if(dopRD1>dopRD2)dopRD=dopRD1;//vyp�e v�t�� hodnotu
			else dopRD=dopRD2;
			//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
			if(F->OBJEKT_akt->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
			{
				//if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
  			F->PmG->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporu�enou rychlost pohonu:"
      	pro_pohon=F->OBJEKT_akt->pohon->n;//ulo�en� pro kter� pohon plat� validace
			}
  		//v�e je vpo��dku
  		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
  		{
  			//povolit_zakazat_editaci(true);
  			F->PmG->ShowNote("",F->d.clError,14);
  		}
  	}
		else if(!pouze_rozmezi)
		{
			F->PmG->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatn� hodnota rychlosti pohonu!"
			pro_pohon=F->OBJEKT_akt->pohon->n;//ulo�en� pro kter� pohon plat� validace
		}
		//pokud prob�hla validace s probl�mem
		if(pro_pohon!=0)
		{
			validovany_pohon=pro_pohon;//ulo�it pohon na kter�m se to stalo
			zapisVID(1,1);//zaps�n� probl�mu do VID, pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
		}
		povolit_zakazat_editaci();//rozhodne se na z�klad� VIDu
	}
	//kontrola p�i ostatn�ch stavech objektu, pouze rozmez� rychlost�
	if(F->PmG!=NULL && F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->rezim==0)
	{
    //deklarace
		unsigned int pro_pohon=0;
		//defaultn� stav
		zapisVID(0,1);//pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
		F->PmG->ShowNote("",F->d.clError,14);
		validovany_pohon=0;//ulo�it pohon na kter�m se to stalo
		//povolit_zakazat_editaci(true);
		//kontrola zda je zadan� hodnota v rozmez�
		if(!F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do))
		{
    	F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
			//povolit_zakazat_editaci(false);
			pro_pohon=F->OBJEKT_akt->pohon->n;//ulo�en� pro kter� pohon plat� validace
		}
		//pokud prob�hla validace s probl�mem
		if(pro_pohon!=0)
		{
			validovany_pohon=pro_pohon;//ulo�it pohon na kter�m se to stalo
			zapisVID(1,1);//zaps�n� probl�mu do VID, pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
		}
		povolit_zakazat_editaci();//rozhodne se na z�klad� VIDu
	}
}
//---------------------------------------------------------------------------
//validace maxim�ln�ho po�tu voz�k� na stopce
void TFormX::validace_max_voziku()
{
	if(posledni_E!=NULL && F->OBJEKT_akt!=NULL && posledni_E->objekt_n!=F->OBJEKT_akt->n)posledni_E=NULL;//pro p��pad, �e se zm�nil n�hled
	if(posledni_E!=NULL && posledni_E->eID==0)
	{
		////smaz�n� p�edchoz� validace z VID
		zapisVID(0,2);//pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
		////deklarace pot�ebn�ch atribut�
		bool validace=true;//p�edpoklad, �e je v�e OK
		////samotn� validace
		posledni_E->data.pocet_pozic=F->max_voziku(posledni_E);
		AnsiString t1=F->ls->Strings[250],t2=F->ls->Strings[251],tip=F->ls->Strings[314]+" ";//"Max. po�et vozik� mus� b�t men�� nebo roven","Nelze, p�ed Stopstanic� se nach�z� oblouk","Pro nastavovan� voz�k je pot�eba buffer o d�lce "
		if(posledni_E->data.pocet_pozic>0 && posledni_E->data.pocet_pozic<posledni_E->data.pocet_voziku){posledni_E->mGrid->ShowNote(t1+" <a>"+AnsiString(posledni_E->data.pocet_pozic)+"</a>");validace=false;}
		if(posledni_E->data.pocet_pozic==0){posledni_E->mGrid->ShowNote(t2);validace=false;}
		if(posledni_E->data.pocet_pozic>0 && posledni_E->data.pocet_pozic<posledni_E->data.pocet_voziku)F->TIP=tip+AnsiString(F->d.v.PP.delka_podvozek*posledni_E->data.pocet_voziku*1000)+" mm.";
		//validace p�ekryt� jig�
		if(posledni_E->data.pocet_voziku>1)
		{
			double rotace=F->m.Rt90(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(posledni_E)+90);
			if(rotace==posledni_E->geo.orientace || rotace==F->m.Rt90(posledni_E->geo.orientace+180))
			{
				validace=false;
				posledni_E->mGrid->ShowNote(F->ls->Strings[426]+" <a>"+AnsiString(1)+"</a>");//"Kv�li p�ekryvu jig� nelze nastavit v�t�� po�et voz�k� ne�"
			}
		}
		////p�eps�n� maxim�ln�ho po�ctu voz�ku do tabulky elementu, pro jistotu
		posledni_E->mGrid->Cells[2][5].Text=posledni_E->data.pocet_pozic;
		////nemo�nost ulo�it p�i chybn�ch hodnot�ch
		if(validace && F->duvod_ulozit_nahled && !F->scGPButton_ulozit->Enabled)F->nahled_ulozit(true);
		if(!validace)
		{
			zapisVID(1,2);//ulo�en� chyby, pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
			povolit_zakazat_editaci();//rozhodne se na z�klad� VIDu
		}
		if(validace && posledni_E->mGrid->Note.Text!="")posledni_E->mGrid->Note.Text="";
	}
}
//---------------------------------------------------------------------------
//vol�no po kliku na link v pozn�mce, napln� edit aRD doporu�enou rychlost�
void TFormX::naplneni_dopRD()
{
	if(F->PmG!=NULL)
	{
//  	F->OBJEKT_akt->pohon->aRD=dopRD;
//		F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(dopRD),3);
//  	zmena_aRD();
//  	//odstranit_korelaci();//pro jistotu z�stavala aktivn� po kliku na link
//  	F->PmG->ShowNote("",F->d.clError,14);
//  	povolit_zakazat_editaci();//rozhodne se na z�klad� VIDu
//		F->Akce=F->BLOK;

		int opraveny_pohon=validovany_pohon;
		vstoupeno_elm=false;
		F->OBJEKT_akt->pohon->aRD=dopRD;
		F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(dopRD),3);
		zmena_aRD();//postar� se o aktualizaci v�ech tabulek
		validace_aRD();//znovuspu�t�n� valiace
		if(opraveny_pohon==validovany_pohon)validovany_pohon=0;//byla odstran�n probl�m
	}
}
//---------------------------------------------------------------------------
bool TFormX::check_click_Note(double X,double Y,bool check_for_highlight)
{
	bool ret=false;
	//hled�n� zda m� n�kter� element nedokon�enou validaci
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		//hled�n� elementu, kter�mu bylo kliknuto na doporu�en� po�et voz�k�
		if(E->mGrid!=NULL && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(X,Y)==TPoint(-2,-2)){ret=true;break;}
		E=F->d.v.dalsi_krok(E,F->OBJEKT_akt);
	}
	if(!ret && F->predchozi_PM!=NULL && F->predchozi_PM->mGrid!=NULL && F->predchozi_PM->mGrid->Note.Text!="" && F->predchozi_PM->mGrid->CheckLink(X,Y)==TPoint(-2,-2))
	{
		E=F->predchozi_PM;
		ret=true;
  }
	if(ret && !check_for_highlight)
	{
		switch(E->eID)
		{
			case 0://dopln� doporu�en� po�et maxim�ln�ch voz�ku po kliku
			{
      	//extrakce po�tu z hintu
		  	AnsiString t=E->mGrid->Note.Text;
				int i=1,zacatek=1,pocet=-1;
		  	while(i<=t.Length())
		  	{
		  		if(pocet>=0)pocet++;
		  		if(t.SubString(i,1)==">"){zacatek=i+1;pocet=0;}
		  		if(pocet>0 && t.SubString(i,1)=="<")break;
		  		i++;
		  	}
		  	//napln�n� dat + tabulka
		  	E->data.pocet_voziku=F->ms.MyToDouble(t.SubString(zacatek,pocet-1));
		  	E->mGrid->Cells[2][6].Text=E->data.pocet_voziku;
        posledni_E=E;//d�le�it� pro metodu validace_max_voziku
				validace_max_voziku();
		  	F->Akce=F->BLOK;
			}break;
			case 200:
			case 300://napln�n� RD
			{
				int opraveny_pohon=validovany_pohon;
				vstoupeno_elm=false;
				Cvektory::TPohon *p=F->d.v.vrat_pohon(validovany_pohon);
				p->aRD=dopRD;
				if(p!=F->OBJEKT_akt->pohon)F->OBJEKT_akt->pohon=p;
				zmena_aRD();//postar� se o aktualizaci v�ech tabulek a znovu spu�t�n� validace
				if(opraveny_pohon==validovany_pohon)validovany_pohon=0;//byla odstran�n probl�m
				p=NULL;delete p;
			}break;
		}
	}

	//uakazatelov� z�le�itosti
	E=NULL;delete E;

	//vr�cen� pro highlight
	return ret;
}
//---------------------------------------------------------------------------
//zakazuje �i povol� ukl�d�n� editace na z�klad� VID
void TFormX::povolit_zakazat_editaci()
{
	if(F->ms.MyToDouble(VID)==0)
		{F->scGPButton_ulozit->Enabled=true;}//pokud je d�vod k ulo�en�, ale button ulo�it je z p�edchoz�ho kroku neaktivn� zapne ho
	else
		{F->scGPButton_ulozit->Enabled=false;}//pokud je button ulo�it zapnut vypne ho
}
//---------------------------------------------------------------------------
//vr�t� ukazatel na element, nelze pou��t vra� element stavaj�c�, z d�vodu �e ID tabulky ji� nemus� b�t toto�n� s n robota
Cvektory::TElement *TFormX::vrat_element_z_tabulky(long ID)
{
	Cvektory::TElement *ret=NULL;
	Cvektory::TElement *E=F->OBJEKT_akt->element;//m��u p�esko�it element, metoda vol�n� po kliku do tabulky elementu
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->mGrid->ID==ID)
		{
			ret=E;
			break;
		}
		E=F->d.v.dalsi_krok(E,F->OBJEKT_akt);
	}
	if(ret==NULL && F->predchozi_PM!=NULL && F->predchozi_PM->mGrid->ID==ID)ret=F->predchozi_PM; 
	E=NULL;delete E;
	return ret;
}
//---------------------------------------------------------------------------
//po zm�n� rotace na elementu E, projde v�echny elementy za a p�epo��t� jim z�ny ot��en�
void TFormX::aktualizace_zon_otaceni(Cvektory::TElement *E)
{
	TPointD zony_otaceni;
	Cvektory::TObjekt *O=F->pom;
	E=E->dalsi;
	while(O!=NULL)
	{
		if(O->n!=F->pom->n)E=O->element->dalsi;
		while(E!=NULL && E->objekt_n==O->n)
		{
			if(E->eID%2!=0 && E->OTOC_delka>0)//aktualizace z�n ot��en�
			{
				zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
				E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
				if(E->objekt_n==F->OBJEKT_akt->n)
				{
					int col=3,row=6;if(E->eID==5){col=2;row=4;}//p�ep�n�n� od kter�ho ��dku za��t, r�zn� elementy
					E->mGrid->Cells[col][row].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[col][row+1].Text=F->outDO(F->m.round2double(E->zona_za,3));//refresh mGridu nen� t�eba
				}
			}
			E=E->dalsi;
		}
		E=NULL;delete E;
		O=O->dalsi;
	}
	delete O;O=NULL;
}
//---------------------------------------------------------------------------
//zm�n� pohon sekund�rn� v�tvi, z v�hybky nebo spojky (pokud sekund�rn� v�tev existuje)
void TFormX::prirazeni_pohohonu_vetvi(Cvektory::TElement *E,long Col)
{
	////deklarace pot�ebn�ch prom�nn�ch
	TscGPComboBox *Combo=E->mGrid->getCombo(Col,2);
	Cvektory::TPohon *p=NULL;
	int p_n=0;
	if(Combo!=NULL && Combo->ItemIndex!=0)p=F->d.v.vrat_pohon(Combo->ItemIndex);
	if(p!=NULL)p_n=p->n;
	F->OBJEKT_akt->pohon=p;//ulo�en� aktu�ln� editovan�ho pohonu
	Cvektory::TElement *e=NULL;
	//zji�t�n� jakou v�tev budu editovat
	bool hlavni=true;
	int prvni=3,druhy=4;
	if(F->prohodit_sloupce_PM(E))
	{
		if(Col==3)Col=4;
		else Col=3;
	}
	if(Col!=3)hlavni=false;

  ////ulo�en� aktu�ln� editovan�ho pohonu
	F->OBJEKT_akt->pohon=p;
	////zm�na pohonu na hlavn� v�tvi
	vstoupeno_elm=false;//zamezen�, aby p�i aktualizace comba do�lo k propadnut� do onchange
	if(hlavni)
	{
		E->pohon=p;
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(F->prohodit_sloupce_PM(e))prvni=4;else prvni=3;
			if(e->eID==200)//pokud naraz�m na PM zm�n�m mu pohon a skon��m pr�chod
			{
				if(e->mGrid->Cells[prvni][2].Type==TmGrid::COMBO)
				{
					Combo=e->mGrid->getCombo(prvni,2);
					if(Combo!=NULL)Combo->ItemIndex=p_n;
				}
				break;
			}
			e=e->dalsi;
		}
		e=E->predchozi;
		while(e!=NULL && e->n>0)
		{
			if(F->prohodit_sloupce_PM(e))druhy=3;else druhy=4;
			if(e->eID==200 && e->mGrid->Cells[druhy][2].Type==TmGrid::COMBO)
			{
				Combo=e->mGrid->getCombo(druhy,2);
				if(Combo!=NULL)Combo->ItemIndex=p_n;
			}
			if(e->eID==200 || F->predchozi_PM==NULL && e->objekt_n!=F->OBJEKT_akt->n || F->predchozi_PM!=NULL && F->predchozi_PM==e)break;//pokud naraz�m na PM NEzm�n�m! mu pohon a skon��m pr�chod, nebo naraz�m na konec objektu
			e->pohon=p;
			e=e->predchozi;
		}
	}
	////zm�na pohonu na vedlej�� v�tvi
	else
	{
		unsigned long o1=E->objekt_n,o2=E->predchozi2->objekt_n;
		e=E->dalsi2;//p�i�azuji pohon z v�hybky
		while(e!=NULL && e->idetifikator_vyhybka_spojka!=E->idetifikator_vyhybka_spojka)
		{
			e->pohon=p;
			if(e->objekt_n!=o1 && e->objekt_n!=o1)F->d.v.vrat_objekt(e->objekt_n)->pohon=p;//pokud je na sekund�rn� v�tvi n�jak� objekt, p�i�ad� mu pohon
			e=e->dalsi;
		}
		update_hodnot_vyhybky_PM(E);//nutno ud�lat ru�n�, metoda aktualizace_tab_elementu(), aktualizuje pouze elementy na stejn�m pohonu, co vyhybka nen�, m� pohon hlavn� v�tve
		//WT �e�� fce. vlo�en� PM
	}

	////na�ten� dat z pohonu do mGridu
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT v�em element�, mus� b�t prvn�
	input_state=COMBO;//d�le�it� pro nastaven� enabled komponent v aktualizaci element�
	aktualizace_tab_elementu();

	////ukazatelov� z�le�itosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
//zobraz� aktu�ln� hodnoty z dat v tabulk�ch
void TFormX::update_hodnot_vyhybky_PM(Cvektory::TElement *E)
{
	if(E->eID==200 || E->eID==300)
	{
		//kontrola zda budou prohozeny sloupce
		int prvni=3,druhy=4;
  	if(F->prohodit_sloupce_PM(E))
  	{
  		//prohhozen� sloupc�
  		prvni=4;druhy=3;
  	}

		//aktualizace hodnot
		vstoupeno_elm=false;
  	if(E->pohon!=NULL)
		{
			if(input_state==NOTHING && ((E->objekt_n==F->OBJEKT_akt->n && F->OBJEKT_akt->rezim!=0) || (E->objekt_n!=F->OBJEKT_akt->n && F->d.v.vrat_objekt(E->objekt_n)->rezim!=0)))//p�epo�et po p�id�n� KK elementu, pouze p�i input_state==NOTHING, tz. nep�epo��t�vat p�i u�ivatelsk� zm�n�
			{
				E->pohon->Rz=F->m.Rz(E->pohon->aRD);
				E->pohon->Rx=F->m.Rx(E->pohon->aRD,E->pohon->roztec);
			}
			if(input_state!=aRD || input_state==aRD && E!=posledni_E)E->mGrid->Cells[prvni][3].Text=F->m.round2double(F->outaRD(E->pohon->aRD),3);
  		E->mGrid->Cells[prvni][4].Text=AnsiString(F->m.round2double(F->outaRD(E->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(E->pohon->rychlost_do),3));
			if(input_state!=R || input_state==R && E!=posledni_E)E->mGrid->Cells[prvni][6].Text=F->m.round2double(F->outR(E->pohon->roztec),3);
			if(input_state!=Rx || input_state==Rx && E!=posledni_E)E->mGrid->Cells[prvni][7].Text=F->m.round2double(E->pohon->Rx,3);
			E->mGrid->Cells[prvni][5].Text=F->m.round2double(F->outR(E->pohon->Rz),3);
  		E->mGrid->Cells[prvni][8].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->pohon->Rz,0)),3);
  		E->mGrid->Cells[prvni][9].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->pohon->Rz,1)),3);
  		E->mGrid->Cells[prvni][10].Text=F->m.round2double(F->outRz(F->m.mezera(90,E->pohon->Rz,1)),3);
  	}
  	else
		{
			E->mGrid->Cells[prvni][3].Text=0;
  		E->mGrid->Cells[prvni][4].Text=0;
  		E->mGrid->Cells[prvni][5].Text=0;
  		E->mGrid->Cells[prvni][6].Text=0;
  		E->mGrid->Cells[prvni][7].Text=0;
  		E->mGrid->Cells[prvni][8].Text=0;
  		E->mGrid->Cells[prvni][9].Text=0;
  		E->mGrid->Cells[prvni][10].Text=0;
  	}
  	if(E->eID==300)//pro v�hybku
  	{
    	if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)
			{
				if(input_state==NOTHING && F->OBJEKT_akt->rezim!=0)//p�epo�et po p�id�n� KK elementu
		  	{
					E->dalsi2->pohon->Rz=F->m.Rz(E->dalsi2->pohon->aRD);
					E->dalsi2->pohon->Rx=F->m.Rx(E->dalsi2->pohon->aRD,E->dalsi2->pohon->roztec);
				}
				if(input_state!=aRD || input_state==aRD && E!=posledni_E)E->mGrid->Cells[druhy][3].Text=F->m.round2double(F->outaRD(E->dalsi2->pohon->aRD),3);
    		E->mGrid->Cells[druhy][4].Text=AnsiString(F->m.round2double(F->outaRD(E->dalsi2->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(E->dalsi2->pohon->rychlost_do),3));
				if(input_state!=R || input_state==R && E!=posledni_E)E->mGrid->Cells[druhy][6].Text=F->m.round2double(F->outR(E->dalsi2->pohon->roztec),3);
				if(input_state!=Rx || input_state==Rx && E!=posledni_E)E->mGrid->Cells[druhy][7].Text=F->m.round2double(E->dalsi2->pohon->Rx,3);
				E->mGrid->Cells[druhy][5].Text=F->m.round2double(F->outR(E->dalsi2->pohon->Rz),3);
  			E->mGrid->Cells[druhy][8].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->dalsi2->pohon->Rz,0)),3);
    		E->mGrid->Cells[druhy][9].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->dalsi2->pohon->Rz,1)),3);
  			E->mGrid->Cells[druhy][10].Text=F->m.round2double(F->outRz(F->m.mezera(90,E->dalsi2->pohon->Rz,1)),3);
  			E->mGrid->Cells[druhy][11].Text=F->m.round2double(F->outPT(E->WT),3);
    	}
    	else
			{
				E->mGrid->Cells[druhy][3].Text=0;
  			E->mGrid->Cells[druhy][4].Text=0;
  			E->mGrid->Cells[druhy][5].Text=0;
  			E->mGrid->Cells[druhy][6].Text=0;
  			E->mGrid->Cells[druhy][7].Text=0;
  			E->mGrid->Cells[druhy][8].Text=0;
  			E->mGrid->Cells[druhy][9].Text=0;
  			E->mGrid->Cells[druhy][10].Text=0;
  		}
  	}
  	else//pro PM
  	{
      Cvektory::TElement *e_pom=E->dalsi;
  		if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;//v p��pad� PM na konci linky
			if(e_pom->pohon!=NULL)
			{
				if(input_state==NOTHING && F->d.v.vrat_objekt(e_pom->objekt_n)->rezim!=0)//p�epo�et po p�id�n� KK elementu
		  	{
					e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->aRD);
					e_pom->pohon->Rx=F->m.Rx(e_pom->pohon->aRD,e_pom->pohon->roztec);
				}
				if(input_state!=aRD || input_state==aRD && E!=posledni_E)E->mGrid->Cells[druhy][3].Text=F->m.round2double(F->outaRD(e_pom->pohon->aRD),3);
  			E->mGrid->Cells[druhy][4].Text=AnsiString(F->m.round2double(F->outaRD(e_pom->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(e_pom->pohon->rychlost_do),3));
				if(input_state!=R || input_state==R && E!=posledni_E)E->mGrid->Cells[druhy][6].Text=F->m.round2double(F->outR(e_pom->pohon->roztec),3);
				if(input_state!=Rx || input_state==Rx && E!=posledni_E)E->mGrid->Cells[druhy][7].Text=F->m.round2double(e_pom->pohon->Rx,3);
  			E->mGrid->Cells[druhy][5].Text=F->m.round2double(F->outR(e_pom->pohon->Rz),3);
  			E->mGrid->Cells[druhy][8].Text=F->m.round2double(F->outRz(F->m.mezera(0,e_pom->pohon->Rz,0)),3);
  			E->mGrid->Cells[druhy][9].Text=F->m.round2double(F->outRz(F->m.mezera(0,e_pom->pohon->Rz,1)),3);
				E->mGrid->Cells[druhy][10].Text=F->m.round2double(F->outRz(F->m.mezera(90,e_pom->pohon->Rz,1)),3);
				E->mGrid->Cells[druhy][11].Text=F->m.round2double(F->outPT(E->WT),3);
			}
			else
			{
				E->mGrid->Cells[druhy][3].Text=0;
  			E->mGrid->Cells[druhy][4].Text=0;
  			E->mGrid->Cells[druhy][5].Text=0;
  			E->mGrid->Cells[druhy][6].Text=0;
  			E->mGrid->Cells[druhy][7].Text=0;
  			E->mGrid->Cells[druhy][8].Text=0;
  			E->mGrid->Cells[druhy][9].Text=0;
  			E->mGrid->Cells[druhy][10].Text=0;
  		}
			e_pom=NULL;delete e_pom;
		}
		F->zmena_editovanych_bunek(E);
	}
}
//---------------------------------------------------------------------------
//provede validaci RD
void TFormX::validace_RD(Cvektory::TElement *E)
{
	if(E->eID==200)//validovat pouze u PM
	{
		//smaz�n� p�edchoz� validace z VID
		zapisVID(0,1);//pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()

		//deklarace
		AnsiString jednotky;
  	if(F->aRDunit==0)jednotky="[m/s]";
		else jednotky="[m/min]";
		AnsiString puv_Note=E->mGrid->Note.Text;
  	bool mimo_rozmezi=false;
		bool pouze_rozmezi=false;
		//zji�t�n� n pohon� v tabulce
		unsigned int p1_n=0,p2_n=0;
		TscGPComboBox *C=E->mGrid->getCombo(3,2);
		if(C!=NULL && C->Enabled)p1_n=C->ItemIndex;
		C=E->mGrid->getCombo(4,2);
		if(C!=NULL && C->Enabled)p2_n=C->ItemIndex;
		unsigned int pro_pohon=0;
		dopRD=0;

		//na�ten� pohon�
		Cvektory::TPohon *p=NULL,*p1=NULL,*p2=NULL;
  	if(p1_n!=0)p1=F->d.v.vrat_pohon(p1_n);
		if(p2_n!=0)p2=F->d.v.vrat_pohon(p2_n);

		//dvojt� validace
    //pokud nalezne probl�m zastav� se a zobraz�ho, i v p��pad�, �e je probl�m� v�ce, a� bude probl�m vy�e�en prob�hne validace zda neexistuje dal�� probl�m
		if(F->OBJEKT_akt->rezim!=0)//kontrola zda je KK re�im, pokud ano validovat
		{
			for(unsigned int i=3;i<=4;i++)
			{
	  		//na�ten� po�adovan�ho pohonu pro validaci
				if(i==3)p=p1;
				if(i==4)p=p2;
	  		//kontrola zda je mo�n� editovat pohon
	  		if(E->mGrid->Cells[i][3].Type==E->mGrid->EDIT && p!=NULL)
	  		{
					//kontrola zda je zadan� hodnota v rozmez�
					if(F->m.between(p->aRD,p->rychlost_od,p->rychlost_do))mimo_rozmezi=false;
	     		else mimo_rozmezi=true;
	     		//zadan� rychlost je mimo rozsah
					if(mimo_rozmezi && p->aRD > 0)
					{
						E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
						//povolit_zakazat_editaci(false);//zak�e btn ulo�it v editaci
						pro_pohon=p->n;//ulo�en� pro kter� pohon plat� validace
						break;//byl nalezen probl�m, zastaven� validace, lze zobrazit jen jeden probl�m v Note
	     		}
					if(!mimo_rozmezi && E->mGrid->Note.Text!="")
					{
						E->mGrid->ShowNote("",F->d.clError,14);
						//povolit_zakazat_editaci(true);
					}
       		// nutn� o�et�en� pro obdob� zad�v�n�/psan�
					if (p->aRD > 0)
	     		{
	     			//v�po�et doporu�en� rychosti
						double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,p->roztec,F->d.v.PP.TT,p->aRD);
						double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,p->roztec,F->d.v.PP.TT,p->aRD);
						if(dopRD1>dopRD2)dopRD=dopRD1;//vyp�e v�t�� hodnotu
	     			else dopRD=dopRD2;
	     			//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
						if(p->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(p->aRD) && mimo_rozmezi==false)
	     			{
							//if(E->mGrid->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
							E->mGrid->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporu�enou rychlost pohonu:"
							pro_pohon=p->n;//ulo�en� pro kter� pohon plat� validace
							break;//byl nalezen probl�m, zastaven� validace, lze zobrazit jen jeden probl�m v Note
						}
	     			//v�e je vpo��dku
						if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(p->aRD) && mimo_rozmezi==false)
	     			{
	     				//povolit_zakazat_editaci(true);
							E->mGrid->ShowNote("",F->d.clError,14);
						}
	     		}
					else E->mGrid->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatn� hodnota rychlosti pohonu!"
				}
			}
		}
		//vaidace pro ostatn� re�imy objektu, nutn� kontrolovat rozmez� rychlosti
		else
		{
			if((p1!=NULL && !F->m.between(p1->aRD,p1->rychlost_od,p1->rychlost_do)) || (p2!=NULL && !F->m.between(p2->aRD,p2->rychlost_od,p2->rychlost_do)))
			{
      	E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
				//povolit_zakazat_editaci(false);//zak�e btn ulo�it v editaci
				if(p1!=NULL && !F->m.between(p1->aRD,p1->rychlost_od,p1->rychlost_do))pro_pohon=p1->n;//ulo�en� pro kter� pohon plat� validace
				else pro_pohon=p2->n;
			}
			else
			{
				E->mGrid->ShowNote("",F->d.clError,14);
				//povolit_zakazat_editaci(true);
			}
		}

		//pokud prob�hla validace s probl�mem
		if(pro_pohon!=0)
		{
			validovany_pohon=pro_pohon;//ulo�it pohon na kter�m se to stalo
			zapisVID(1,1);//zaps�n� probl�mu do VID, pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()
		}
		povolit_zakazat_editaci();//rozhodne se na z�klad� VIDu

		//roznesen� validace na ostatn�
		if(puv_Note!=E->mGrid->Note.Text)//do�lo ke zm�n� note
		{
			puv_Note=E->mGrid->Note.Text;
			try//v p��pad� �e by neexistovalo combo, mGrid mimo obraz
			{
				if(F->predchozi_PM!=NULL && F->predchozi_PM!=E && (F->predchozi_PM->mGrid->getCombo(3,2)->ItemIndex==validovany_pohon || F->predchozi_PM->mGrid->getCombo(4,2)->ItemIndex==validovany_pohon))//pokud m� PM stejn� pohon jako validovan� pohon
					F->predchozi_PM->mGrid->ShowNote(puv_Note,F->d.clError,14);
			}catch(...){;}
			Cvektory::TElement *e_pom=F->OBJEKT_akt->element;
			while(e_pom!=NULL && e_pom->objekt_n==F->OBJEKT_akt->n)
			{
				try
				{
					if((e_pom->eID==200 || e_pom->eID==300) && ((e_pom->mGrid->Cells[3][2].Type==TmGrid::COMBO && e_pom->mGrid->getCombo(3,2)->ItemIndex==validovany_pohon) || (e_pom->mGrid->Cells[3][2].Type==TmGrid::COMBO && e_pom->mGrid->getCombo(4,2)->ItemIndex==validovany_pohon)))
						e_pom->mGrid->ShowNote(puv_Note,F->d.clError,14);
				}catch(...){;}
				e_pom=F->d.v.dalsi_krok(e_pom,F->OBJEKT_akt);
			}
			e_pom=NULL;delete e_pom;
		}

  	//ukazatelov� z�le�itosti
		C=NULL;p=NULL;p1=NULL;p2=NULL;
		delete C;delete p;delete p1;delete p2;
	}
}
//---------------------------------------------------------------------------
//p�i�azen� pohonu p�ed PM, nebo za PM
void TFormX::prirazeni_pohohonu_PM(Cvektory::TElement *E,long Col)
{
	////deklarace + na�ten� pohonu, kter� bude p�i�azov�n
	TscGPComboBox *Combo;
	Combo=E->mGrid->getCombo(Col,2);
	Cvektory::TPohon *p=NULL;
	if(Combo->ItemIndex!=0)p=F->d.v.vrat_pohon(Combo->ItemIndex);
	int p_n=0;
	if(p!=NULL)p_n=p->n;
	Cvektory::TElement *e=NULL;

	////ulo�en� aktu�ln� editovan�ho pohonu
	F->OBJEKT_akt->pohon=p;

	////prohozen� sloupc�
	int prvni=3,druhy=4;
	if(F->prohodit_sloupce_PM(E))
	{
		if(Col==3)Col=4;
		else Col=3;
	}

	////p�i�azen� pohonu p�ed PM
	vstoupeno_elm=false;//zamezen�, aby p�i aktualizace comba do�lo k propadnut� do onchange
	if(Col==3)
	{
		E->pohon=p;
		e=E->predchozi;
		while(e!=NULL && e->n>0 && e->objekt_n==F->OBJEKT_akt->n)
		{
			if(F->prohodit_sloupce_PM(e))druhy=3;else druhy=4;
			if(e->eID==300 && e->mGrid->Cells[prvni][2].Type==TmGrid::COMBO)
			{
				Combo=e->mGrid->getCombo(prvni,2);//extrakce ukazatele na combo v tabulce
				if(Combo!=NULL)Combo->ItemIndex=p_n;//nutn� dodate�n� kontrola zda n�n� NULL (mGrid mimo obraz)
			}
			if(e->eID==200)//narazil jsem na PM, konec
			{
				if(e->mGrid->Cells[druhy][2].Type==TmGrid::COMBO)
				{
					Combo=e->mGrid->getCombo(druhy,2);
					if(Combo!=NULL)Combo->ItemIndex=p_n;
				}
				break;
			}
			else e->pohon=p;
			e=e->predchozi;
		}
		//do�el jsem na za��tek ob�ktu a m�m v p�edchoz�m PM
		if(e!=NULL && e->n>0 && (e==F->OBJEKT_akt->element || e==F->OBJEKT_akt->element->predchozi) && F->predchozi_PM!=NULL)
		{
      F->d.v.vrat_objekt(F->predchozi_PM->objekt_n)->pohon=p;
			if(F->prohodit_sloupce_PM(F->predchozi_PM))druhy=3;else druhy=4;
			if(F->predchozi_PM->mGrid->Cells[druhy][2].Type==TmGrid::COMBO)
			{
				Combo=F->predchozi_PM->mGrid->getCombo(druhy,2);
				if(Combo!=NULL)Combo->ItemIndex=p_n;
			}
			e=F->predchozi_PM->dalsi;
			while(e!=NULL && e!=F->OBJEKT_akt->element)
			{
				e->pohon=p;
				e=e->dalsi;
			}
		}
	}
	////p�i�azen� pohonu za PM
	if(Col==4)
	{
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(e->objekt_n!=E->objekt_n)F->d.v.vrat_objekt(e->objekt_n)->pohon=p;
			if(F->prohodit_sloupce_PM(e))prvni=4;else prvni=3;
			if(e->eID==300 && e->mGrid->Cells[prvni][2].Type==TmGrid::COMBO)
			{
				Combo=e->mGrid->getCombo(prvni,2);
				if(Combo!=NULL)Combo->ItemIndex=p_n;
			}
			if(e->eID==200)//narazil jsem na PM, zapsat nov� pohon a konec
			{
				if(e->mGrid->Cells[prvni][2].Type==TmGrid::COMBO)
				{
					Combo=e->mGrid->getCombo(prvni,2);
					if(Combo!=NULL)Combo->ItemIndex=p_n;
				}
				break;
			}
			else e=e->dalsi;
		}
		if(E->dalsi!=NULL && E->dalsi->objekt_n==E->objekt_n)E->WT=F->m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
	}

	////na�ten� dat z pohonu do mGridu
	mazatPM(E);//kontrola a dotaz na maz�n� elementu + ovlivn�n�ch PM
	F->vlozit_predavaci_misto_aktualizuj_WT();//mus� b�t prvn�!!
	input_state=COMBO;//d�le�it� pro nastaven� enabled komponent v aktualizaci element�
	if(p!=NULL)aktualizace_tab_elementu();
	else aktualizace_tab_elementu_pOdebran();
	////aktualizace knihoven
	F->DrawGrid_knihovna->Refresh();
	F->DrawGrid_ostatni->Refresh();
	F->DrawGrid_otoce->Refresh();

	////ukazatelov� z�le�itosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
//p�i�azen� pohonu z PmG
void TFormX::prirazeni_pohonu_defTab()
{
	//na�ten� p�i�azovan�ho pohonu
	int p_n=F->PmG->getCombo(3,0)->ItemIndex;
	Cvektory::TPohon *p=NULL;
	if(p_n!=0)p=F->d.v.vrat_pohon(p_n);

	//zaps�n� pohonu do objektu, tz. editovan� pohon
	F->OBJEKT_akt->pohon=p;
	//aktualizace knihovny element�
	F->DrawGrid_knihovna->Refresh();//elementy
	F->DrawGrid_otoce->Refresh();//oto�e
	F->DrawGrid_ostatni->Refresh();//stop + ion ty�
	F->DrawGrid_geometrie->Refresh();//geometrie + PM

  //aktualiace parametr�
	if(F->OBJEKT_akt->rezim!=0)
	{
		F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
		F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
	}

	//pr�chod skrze elementy v objektu a p�i�azen� toto pohonu
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		E->pohon=p;
		E=E->dalsi;
	}
	input_state=COMBO;//d�le�it� pro nastaven� enabled komponent v aktualizaci element�
	if(p==NULL)aktualizace_tab_elementu_pOdebran();//aktualizace tabulek elemnt�
	else aktualizace_tab_elementu();//do�lo ke zm�n�m v tabulce pohonu, kter� ovlivn� i ostatn� elementy

	//kontrola PM
	F->vlozit_predavaci_misto_aktualizuj_WT();
	F->aktualizace_tab_pohon(false,true,true);//nebude provedena pokud dojde k odstran�n� PmG

	//pokud u� neexistuje PmG, nalezen� PM pro spu�t�n� validace
	if(F->PmG==NULL)
	{
		posledni_E=F->OBJEKT_akt->element;
		while(posledni_E!=NULL && posledni_E->objekt_n==F->OBJEKT_akt->n)
		{
			if(posledni_E->dalsi==NULL || (posledni_E->dalsi!=NULL && posledni_E->dalsi->objekt_n!=F->OBJEKT_akt->n))break;
      posledni_E=posledni_E->dalsi;
		}
	}

	//ukazatelov� z�le�itosti
	E=NULL;p=NULL;
	delete E;delete p;
}
//---------------------------------------------------------------------------
//zapi�e na danou pozici ve VID dan� ��slo
void TFormX::zapisVID(int zapis,int pozice)
{
	//pozice = 1 ... validace pohonu
	//pozice = 2 ... validace stopek
	if(pozice<=VID.Length())
	{
		VID=VID.SubString(1,pozice-1)+AnsiString(zapis)+VID.SubString(pozice+1,VID.Length());
	}
}
//---------------------------------------------------------------------------
//vynuluje VID, podle d�lky nastaven� v konstruktoru nap�. 2 ��slice
void TFormX::vynulujVID()
{
  //nulov�n� ��slic podle po�tu ��slic v �et�zci VID
	for(int i=1;i<=VID.Length();i++)
	{
    zapisVID(0,i);
  }
}
//---------------------------------------------------------------------------
//kontorla a dotaz zda maj� b�t PM na stejn�m pohonu smaz�na
void TFormX::mazatPM(Cvektory::TElement *Element)
{
	//deklarace
	bool nalezen=false;
	bool mazat_element=false;
	Cvektory::TElement *E=NULL;

	//hled�n� ovlivn�n�ho PM v cel�m objektu
	if(Element==NULL)
	{
		//hled�n� ovlivn�n�ho PM
		E=F->OBJEKT_akt->element;
		while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
  	{
  		//kontrola zda jsem narazil na PM a je mo�n� ho smazat
  		if(E->eID==200 && (E->dalsi!=NULL && E->pohon==E->dalsi->pohon) || (E->dalsi==NULL && E->pohon==F->d.v.ELEMENTY->dalsi->pohon))
  		{
				Element=E;
				mazat_element=true;
  			break;
  		}
  		else E=E->dalsi;
		}
    //kontrola predchoziho PM
		if(F->predchozi_PM!=NULL && ((F->predchozi_PM->dalsi!=NULL && F->predchozi_PM->pohon==F->predchozi_PM->dalsi->pohon) || (F->predchozi_PM->dalsi==NULL && F->predchozi_PM->pohon==F->d.v.ELEMENTY->dalsi->pohon)))
		{
			nalezen=true;
			E=F->predchozi_PM;
		}
	}

	//hled�n� ovlivn�n�ho PM p�ed a za PM, kter� vyvolalo zm�nu
	else
	{
		if((Element->dalsi!=NULL && Element->pohon==Element->dalsi->pohon) || (Element->dalsi==NULL && Element->pohon==F->d.v.ELEMENTY->dalsi->pohon))mazat_element=true;
		//hled�n� ovlivn�n�ho PM
		E=Element->dalsi;
		while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
		{
			//kontrola zda jsem narazil na PM a je mo�n� ho smazat
			if(E->eID==200 && (E->dalsi!=NULL && E->pohon==E->dalsi->pohon) || (E->dalsi==NULL && E->pohon==F->d.v.ELEMENTY->dalsi->pohon))
			{
				nalezen=true;
				break;
			}
			else E=E->dalsi;
		}
		//kontrola zda sem za PM na�el ovlivn�n� PM, pokud ne kontrola p�ed PM
		if(!nalezen)
		{
			E=Element->predchozi;
			while(E!=NULL && E->n>0 && E->objekt_n==F->OBJEKT_akt->n)
			{
				//kontrola zda jsem narazil na PM a je mo�n� ho smazat
				if(E->eID==200 && (E->dalsi!=NULL && E->pohon==E->dalsi->pohon) || (E->dalsi==NULL && E->pohon==F->d.v.ELEMENTY->dalsi->pohon))
				{
					nalezen=true;
					break;
				}
				else E=E->predchozi;
      }
		}
  	//kontrola predchoziho PM
		if(!nalezen && F->predchozi_PM!=NULL && ((F->predchozi_PM->dalsi!=NULL && F->predchozi_PM->pohon==F->predchozi_PM->dalsi->pohon) || (F->predchozi_PM->dalsi==NULL && F->predchozi_PM->pohon==F->d.v.ELEMENTY->dalsi->pohon)))
		{
			nalezen=true;
			E=F->predchozi_PM;
		}
	}

  //ulo�en� posunu a zoom p�ed posuny na elementy
	TPointD predchozi_posun=F->Posun;
	double predchozi_zoom=F->Zoom;

	//dotaz + smaz�n� samotn�ho PM kde byla vyvolan� zm�na
	String name="";
	if(nalezen)name=E->name;
	if(mazat_element && Element!=NULL && Element->eID==200)//zm�na m��e b�t vyvol�na i z v�hybky
	{
		F->posun_na_element(Element);
		F->REFRESH(false);//mus� n�sledovat p�esunu
		F->pom_element=Element;
		F->pom_element_temp=Element;
		//podstr�en� sou�adnic pro zobrazen� MB
		F->akt_souradnice_kurzoru_PX.x=F->m.L2Px(Element->geo.X4+0.5);
		F->akt_souradnice_kurzoru_PX.y=F->m.L2Py(Element->geo.Y4+0.5);
		F->Smazat1Click(this);
	}

	//kontrola zde ovlivn�n� PM existuje + dotaz na maz�n�
	if(nalezen)
	{
		F->posun_na_element(E);
		F->REFRESH(false);//mus� n�sledovat p�esunu
		E->name=name;//pokud do�lo k odstran�n� nap��klad p�edchoz�ho PM, do�lo ji� i k aktualizaci n�zv�, nap� v praxi sma�u prvn� predchoz� PM "PM1" a druhy dotaz na Element, kter� b�val "PM2" bude: "Chcete smazat PM1"
		F->pom_element=E;
		F->pom_element_temp=E;
		//podstr�en� sou�adnic pro zobrazen� MB
		F->akt_souradnice_kurzoru_PX.x=F->m.L2Px(E->geo.X4+0.5);
		F->akt_souradnice_kurzoru_PX.y=F->m.L2Py(E->geo.Y4+0.5);
		F->Smazat1Click(this);
	}

	//ulo�en� posunu a zoom p�ed posuny na elementy
	F->Posun=predchozi_posun;
	F->Zoom=predchozi_zoom;

	//ukazatelov� z�le�itosti
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
