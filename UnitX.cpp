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
	validovat_pohon=false;
	VID="00";//zde se definuje po�et ��slic oba�en�ch ve VID
	aut_mazani_PM=false;//je true pouze v �ase automatick�ho odmaz�v�n� PM
	popisky_pouzivany_pohon=false;
  vykresli_vetev=0;
	//pokud dojde ke zm�n� po�ad� ��dku, nastavit nov� pozice zde, + p�eps�n� switche pro tabulku pohonu v OnChange + p�epsan� switche v korelace_tab_pohonu()
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
  //p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
	//v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
	input_state=NO;
	if(ID==6666)F->tab_knihovna_click(-100,-100,Col,Row);//p�epos�n� ud�losti
	//funkcionalita exBUTTONu v tabulce pohonu
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		if(F->OBJEKT_akt->stavPM==2)
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			F->PmG->VisibleRow(3,false,false);
			F->PmG->VisibleRow(4,false,false);
			F->PmG->VisibleRow(5,false,false);
			F->PmG->VisibleRow(6,false,false);
			F->PmG->VisibleRow(7,false,false);
			F->PmG->VisibleRow(8,false,false);
			F->OBJEKT_akt->stavPM=0;//default
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
			F->OBJEKT_akt->stavPM=2;//max
		}
		F->nahled_ulozit(true);//nov� se ukl�d� stav tabulky -> d�vod ulo�it p�i zm�n� stavu
		F->nastav_focus();
		F->REFRESH(true);//mus� b�t opravdu REFRESH() cel�ho formu + mGridu
		F->PmG->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
	}

	if(ID!=9999 && Col==4 && Row==0)//klik na glyphbutton v pohonov� tabulce
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		if(E!=NULL && (E->eID==200 || E->eID==300))
		{
			if(F->OBJEKT_akt->stavPM!=1)//budu skr�vat
			{
				F->OBJEKT_akt->stavPM=1;//ulo�en� stavu minimized
			}
			else//budu zobrazovat
			{
				if(E->mGrid->exBUTTON->GlyphOptions->Kind==scgpbgkUpArrow)
				{
					F->OBJEKT_akt->stavPM=2;//navr�cen� do stavu maximazied
				}
				else F->OBJEKT_akt->stavPM=0;//defaultn� stav
			}
			aktualizace_tab_elementu();//update PM provede skryt� �i zobrazen� ��dk� podle stavuPM
			F->nahled_ulozit(true);//nov� se ukl�d� stav tabulky -> d�vod ulo�it p�i zm�n� stavu
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
				if(E->mGrid->Rows[7].Visible)
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(7,false,false);
					E->mGrid->VisibleRow(8,false,false);
				}
				else
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(7,true,false);
					E->mGrid->VisibleRow(8,true,false);
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
				if(F->OBJEKT_akt->stavPM==2)
				{
					F->OBJEKT_akt->stavPM=0;//jdu z max do default stavu
				}
				else
				{
					F->OBJEKT_akt->stavPM=2;//jdu z default do max stavu
				}
				aktualizace_tab_elementu();//update PM provede skryt� �i zobrazen� ��dk� podle stavuPM
				F->nahled_ulozit(true);//nov� se ukl�d� stav tabulky -> d�vod ulo�it p�i zm�n� stavu
			}break;
			case 400:case 401:case 402://teplom�ry
			{
				bool akce;
				if(E->mGrid->exBUTTON->GlyphOptions->Kind==scgpbgkUpArrow)//skr�vat
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					akce=false;
				}
				else//zobrazit
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					akce=true;
				}
				//nastavov�n� ��dk�, vynech�n� hlavi�ky a posledn�ho ��dku celkem
				for(unsigned int i=1;i<E->mGrid->RowCount-1;i++)
				{
					E->mGrid->VisibleRow(i,akce,false);
				}
			}break;
		}
		F->mGrid_on_mGrid();//mohlo doj�t k p�ekryvu, nebo odstran�n� p�ekryvu ... nutn� kontrola
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
//MouseMove z komponent v mGridu
void TFormX::OnMouseMove(long Tag,long ID,int X,int Y,unsigned long Col,unsigned long Row)
{
	if(F->OBJEKT_akt!=NULL)
	{
  	//deklarace
  	int puv=vykresli_vetev;
  	vykresli_vetev=0;//nulov�n� p�ed testem
  	if(F->pom_element==NULL && !F->OBJEKT_akt->uzamknout_nahled)F->pom_element=vrat_element_z_tabulky(ID);//pokud je pom_element je NULL na�ten� z tabulky

  	//kontrola, zda se jedn� o v�hybku nebo spojku
  	if(F->pom_element!=NULL && !F->OBJEKT_akt->uzamknout_nahled && (F->pom_element->eID==300 || F->pom_element->eID==301))
  	{
  		if(F->pom_element->eID==300)Col=Col-2;
  		if(F->prohodit_sloupce_PM(F->pom_element))
			{
  			if(Col==1)Col=2;
  			else Col=1;
  		}
  		vykresli_vetev=Col;
  	}

		//pokud do�lo ke zm�n�, REFRESH
		if(vykresli_vetev!=puv)F->REFRESH();
	}
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
		//F->posledni_editovany_element=E;//odstaveno, naru�uje tvorbu geometrie
		if(F->OBJEKT_akt->pohon!=E->pohon)F->OBJEKT_akt->pohon=E->pohon;
		validovat_pohon=false;
    popisky_pouzivany_pohon=false;
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
				////temp odemknut� editace max WT
				if(Row==4)//Col=2
				{
					input_state=WT;
					E->WT1=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
          F->aktualizace_RT();
				}
				////konec temp
        //pokud nejsou nastaveny ��dn� voz�ky, nastavit stav na 0, mus� prob�hnout aktualizace sp�rovan�ch ukazatel�
				if(E->mGrid->Cells[Col][Row].Text!="")
				{
					if(E->data.pocet_voziku==0){E->stav=0;E->sparovany=NULL;F->d.v.aktualizuj_sparovane_ukazatele();}
					else if(E->stav==0){E->stav=1;F->d.v.aktualizuj_sparovane_ukazatele();}
					if(E->sparovany!=NULL)E->mGrid->Cells[2][1].Text=E->sparovany->name;else E->mGrid->Cells[2][1].Text="N/A";
				}
				//dod�lat pln�n� pam�ti p�i editaci bun�k
			} break;
			case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinu�ln�)
			{
				if(Row==1)//editace CT
				{
					input_state=CT; //nastaveni stavu
					double CT = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(CT!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/CT;//ulo�en� do pam�ti + v�po�et
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//nutn� o�et�en� podm�nkou, v p��pad� editace textu, kter� je cel� ozna�en� ud�lost OnChange spu�t�na 2x
						//zm�na v ostatn�ch elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG, mus� b�t refresh, PmG refre� obsluhuje druh� case v OnChange
					}
					E->data.RT=F->m.KKRT(CT,E->data.PT1);
					E->mGrid->Cells[Col][3].Text = F->m.round2double(F->outPT(E->data.RT),3); //OUTPUT
        }
				if(Row==2)// editace PT
				{
					input_state=PT; //nastaveni stavu
          double CT=F->m.CT(E->data.LO1,E->pohon->aRD);
					E->data.PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->data.RT=F->m.KKRT(CT,E->data.PT1);
					E->mGrid->Cells[Col][3].Text = F->m.round2double(F->outPT(E->data.RT),3); //OUTPUT
				}
				if(Row==4)// eidtace LO
				{
					input_state=LO; //nastaveni stavu
					E->data.LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					double CT=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][1].Text = F->m.round2double(F->outPT(CT),3); //OUTPUT
          E->data.RT=F->m.KKRT(CT,E->data.PT1);
					E->mGrid->Cells[Col][3].Text = F->m.round2double(F->outPT(E->data.RT),3); //OUTPUT
				}
				if(Row==5)// eidtace LO_pozice
				{
					input_state=LO2; //nastaveni stavu
					E->data.LO_pozice=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//pouze ulo�en� do dat
				}
				if(Row==6)// eidtace COMBO PD
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
				////temp odemknut� editace max WT
				if(Row==3)//Col=1
				{
					input_state=WT;
					E->WT1=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
					F->aktualizace_RT();
				}
				////konec temp
			} break;
			case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivn� oto��
			{
				if(Row==1)//editace CT1
				{
					input_state=CT;//nastaveni stavu
					double CT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)),CT2;
          //o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(CT1!=0)
					{
			  		//ovlivn�n� aRD
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/CT1;
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
			  		//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][5].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][9].Text=F->m.round2double(F->outPT(CT2),3);
						//zm�na v ostatn�ch elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
					//RT
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
        }
				if (Row==2)//editace PT1
				{
					input_state=PT;//nastaveni stavu
					E->data.PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					//RT
          double CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD),CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				if (Row==3)//editace LO1
				{
					input_state=LO;//nastaveni stavu
					E->data.LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					double CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][1].Text = F->m.round2double(F->outPT(CT1),3);//OUTPUT
          //RT
					double CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				if (Row==4)//editace COMBO
				{
					input_state=COMBO;//nastaveni stavu
					switch(E->mGrid->getCombo(Col,Row)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
					//aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[3][7].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[3][8].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//p�epo��t�n� z�n ot��en� n�sleduj�c�ch element�
				}
				//Row = 5 nen� u�ivatelsky upraviteln�
				if (Row==6)//editace d�lky oto�e
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PTotoc=E->OTOC_delka/F->OBJEKT_akt->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
          //aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[3][7].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[3][8].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
				if(Row==9)//editace CT2
				{
					input_state=CT;//nastaveni stavu
					double CT1,CT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
          //o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(CT2!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO2/CT2;//ulo�en� do pam�ti + v�po�et
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
						CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][1].Text=F->m.round2double(F->outPT(CT1),3);
						E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][5].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zm�na v ostatn�ch elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
					//RT
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				if (Row==10)//editace PT2
				{
					input_state=PT;//nastaveni stavu
					E->data.PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
          //RT
          double CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD),CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				if (Row==11)//editace LO2
				{
					input_state=LO2;//nastaveni stavu
					E->data.LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					double CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][9].Text = F->m.round2double(F->outPT(CT2),3);//OUTPUT
          //RT
          double CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				if(Row==13)// eidtace COMBO PD
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
					E->WT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
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
				////temp odemknut� editace max WT
				if(Row==4)//Col=1
				{
					input_state=WT;
					E->WT1=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
					F->aktualizace_RT();
				}
				////konec temp
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
						if(E->mGrid->Cells[Col][7].Text!="-")//kontrola, zda je zobrazeno Rx
						{
							p->Rz=F->m.Rz(p->aRD);
							p->Rx=F->m.Rx(p->aRD,p->roztec);
						}
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT1=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o dal�� pohon
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
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT1=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o dal�� pohon
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
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT1=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o dal�� pohon
						p=NULL;delete p;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					////temp odemknut� editace max WT
					case 11:
					{
						input_state=WT;
            F->zmena_editovanych_bunek(E);
						break;
					}
          ////konec temp
				}
			}break;
			//spojka
			case 301:
			{
				switch(Row)
				{
					case 3:
					{
						input_state=WT;
						F->zmena_editovanych_bunek(E);
					}break;
				}
			}break;
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
  validovat_pohon=true;
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
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		if(E->eID==200 || E->eID==300)zobrazit_skryt_radkyPM(E);
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->OBJEKT_akt->pohon!=NULL && (E->pohon->n==F->OBJEKT_akt->pohon->n || (E->eID==200 || E->eID==300)))//p�esko�� mimo_element a hlavi�ku, poze p�epo�et element�m kter� maj� stejn� pohon jako aktu�ln� editovan� pohon
		{
			switch(E->eID)
			{
				case 0:
				{
					//p�epo�ty
					E->WT1=F->m.cekani_na_palec(0,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->aRD,3);
					E->mGrid->Cells[2][4].Text=F->m.round2double(F->outPT(E->WT1),3);
					F->d.v.reserve_time(E);
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinu�ln�
				{
					//p�epo�ty
					double CT=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[1][1].Text = F->m.round2double(F->outPT(CT),3); //OUTPUT
					E->data.RT=F->m.KKRT(CT,E->data.PT1);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->data.RT),3); //OUTPUT
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanic�
				{
					//p�epo�ty
					E->WT1=F->m.cekani_na_palec(0,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT1),3);
					F->d.v.reserve_time(E);
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivn� oto��
				{
					//na�ten� hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//p�epo�ty
					double CT1,CT2;
					CT1=F->m.CT(E->data.LO1,aRD);
					E->mGrid->Cells[3][1].Text=F->m.round2double(F->outPT(CT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[3][5].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					CT2=F->m.CT(E->data.LO2,aRD);
					E->mGrid->Cells[3][9].Text=F->m.round2double(F->outPT(CT2),3);
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivn� oto��
				{
					//p�epo�ty
					E->WT1=F->m.cekani_na_palec(0,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT1),3);
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
					E->WT1=F->m.cekani_na_palec(0,roztec,aRD,3);//d�le�it� pro v�po�et RT, nezobrazuje se
					E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					F->d.v.reserve_time(E);
				}break;
				case 200://p�ed�vac� m�sto
				case 300://v�hybla
				{
					update_hodnot_vyhybky_PM(E);//provede zobrazen� hodnot pohonu a v�po��t� nov� hodnoty mezer, provede aktualizaci WT
					//validace_RD(E);
				}break;
				case 301://spojka
				{
					F->zmena_editovanych_bunek(E);
				}break;
			}
			if(input_state==COMBO)F->set_enabled_mGrid(E);//pouze p�i zm�n� pohonu, jinak zbyte�n�
			//update rozbalen� zabalen� tabulek
			E->mGrid->Refresh();
		}
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstran�n� pr�chodov�ho spoj�ku

	//p�edchoz� PM
	if(F->predchozi_PM!=NULL && F->predchozi_PM!=mimo_element)
	{
		update_hodnot_vyhybky_PM(F->predchozi_PM);
		//update rozbalen� zabalen� tabulek
		zobrazit_skryt_radkyPM(F->predchozi_PM);
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
		if(E->n>0 && (E->pohon==NULL || (E->eID==200 || E->eID==300)))//p�esko�� hlavi�ku + p�enastav� pouze elementy s odstran�n�m pohonem
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
					E->mGrid->Cells[3][5].Text=0;
					E->mGrid->Cells[3][9].Text=0;
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
					F->zmena_editovanych_bunek(E);
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
void TFormX::korelace_tab_pohonu(int Row,bool vypsat_note_pouzivan)
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
    //vypisov�n� upozorn�n� u pou��van�ch pohon�
		if(vypsat_note_pouzivan && F->OBJEKT_akt->pohon!=NULL && F->PmG->Note.Text=="" && F->je_pohon_pouzivan(F->OBJEKT_akt->pohon->n))F->PmG->ShowNote(F->ls->Strings[493]);//�Tato zm�na ovlivn� v�echny prvky na tomto pohonu.�
		else F->PmG->Refresh();
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
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();//vytvo�en� pr�chodov�ho spoj�ku
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->OBJEKT_akt->pohon==E->pohon)
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[2][2].Highlight=true;
					//E->mGrid->Cells[2][3].Highlight=true;
					E->mGrid->Cells[2][4].Highlight=true;
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Highlight=true;
          E->mGrid->Cells[1][3].Highlight=true;
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
					E->mGrid->Cells[3][5].Highlight=true;
					E->mGrid->Cells[3][9].Highlight=true;
          E->mGrid->Cells[3][12].Highlight=true;
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
					E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
				}break;
			}
			E->mGrid->Refresh();
		}
		//PM zvl�t, neplat� na n� podm�nka E->pohon == OBJEKT_akt->pohon, proto�e maj� pohony 2
		if(E->n>0 && E->n!=n && mimo_element!=E && (E->eID==200 || E->eID==300))
		{
			//zji�t�n�, zda m� element stejn� pohon jako upravovan� pohon, pokud ano zjist� v jak�m je sloupci
			long Col=3;
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
				if(E->mGrid->Cells[Col][11].Text!="-")E->mGrid->Cells[Col][11].Highlight=true;
			}
			E->mGrid->Refresh();
    }
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstran�n� pr�chodov�ho spoj�ku

	//korelace v tabulce p�edchoz�ho pm
	if(F->predchozi_PM!=NULL)
	{
		E=F->predchozi_PM;
		if(E->n!=n && mimo_element!=E)
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
				if(E->mGrid->Cells[Col][11].Text!="-")E->mGrid->Cells[Col][11].Highlight=true;
			}
		}
    E->mGrid->Refresh();
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//korelace uvnit� element�
void TFormX::korelace_v_elementech(long ID,long Col,long Row)
{
	Cvektory::TElement *E=vrat_element_z_tabulky(ID);
	bool byl_refreshovan=false;
	if(F->OBJEKT_akt->pohon!=E->pohon)F->OBJEKT_akt->pohon=E->pohon;
	switch(E->eID)
	{
		case 0://stopka
		{
			if(Row==3)E->mGrid->Cells[2][6].Highlight=true;
			if(Row==6)E->mGrid->Cells[2][3].Highlight=true;
      break;
		}
		case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinu�ln�)
		{
			if(Row==1){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1,false);}korelace_tab_pohonu_elementy();E->mGrid->Cells[1][3].Highlight=true;}//E->mGrid->Cells[1][Row+1].Highlight=true;
      if(Row==2)E->mGrid->Cells[1][3].Highlight=true;
			if(Row==4){E->mGrid->Cells[1][1].Highlight=true;E->mGrid->Cells[1][3].Highlight=true;}
      //vypisov�n� upozorn�n� u pou��van�ch pohon�
			if(Row==1 && E->pohon!=NULL && E->mGrid->Note.Text=="" && F->je_pohon_pouzivan(E->pohon->n)){E->mGrid->ShowNote(F->ls->Strings[493]);byl_refreshovan=true;}//�Tato zm�na ovlivn� v�echny prvky na tomto pohonu.�
		} break;
		case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanic�
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivn� oto��
		{
			if(Row==1){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1,false);}korelace_tab_pohonu_elementy();E->mGrid->Cells[3][12].Highlight=true;}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[3][12].Highlight=true;
			if(Row==3){E->mGrid->Cells[3][1].Highlight=true;E->mGrid->Cells[3][12].Highlight=true;}
			if(Row==6){E->mGrid->Cells[3][5].Highlight=true;E->mGrid->Cells[3][7].Highlight=true;E->mGrid->Cells[3][8].Highlight=true;}
			if(Row==9){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1,false);}korelace_tab_pohonu_elementy();E->mGrid->Cells[3][1].Highlight=true;E->mGrid->Cells[3][5].Highlight=true;}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==10)E->mGrid->Cells[3][12].Highlight=true;
			if(Row==11){E->mGrid->Cells[3][9].Highlight=true;E->mGrid->Cells[3][12].Highlight=true;}
      //vypisov�n� upozorn�n� u pou��van�ch pohon�
			if((Row==1 || Row==9) && E->pohon!=NULL && E->mGrid->Note.Text=="" && F->je_pohon_pouzivan(E->pohon->n)){E->mGrid->ShowNote(F->ls->Strings[493]);byl_refreshovan=true;}//�Tato zm�na ovlivn� v�echny prvky na tomto pohonu.�
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
			//p�i�azen� akt. pohonu
			unsigned int pohon=0;
			TscGPComboBox *Combo=E->mGrid->getCombo(Col,2);
			if(Combo!=NULL)pohon=Combo->ItemIndex;
			if((F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->pohon->n!=pohon) || F->OBJEKT_akt->pohon==NULL)F->OBJEKT_akt->pohon=F->d.v.vrat_pohon(pohon);
			Combo=NULL;delete Combo;
			//nastavov�n� highlightu
			if(Row==3)
			{
				E->mGrid->Cells[Col][7].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
				if(E->mGrid->Cells[Col][11].Text!="-")E->mGrid->Cells[Col][11].Highlight=true;
				korelace_tab_pohonu_elementy(E);//ozna�en� v ostatn�ch tabulk�ch
			}
			if(Row==6 || Row==7)
			{
				E->mGrid->Cells[Col][3].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
        if(E->mGrid->Cells[Col][11].Text!="-")E->mGrid->Cells[Col][11].Highlight=true;
				korelace_tab_pohonu_elementy(E);//ozna�en� v ostatn�ch tabulk�ch
			}
      //vypisov�n� upozorn�n� u pou��van�ch pohon�
			if(Row!=0 && Row<11 && E->pohon!=NULL && E->mGrid->Note.Text=="" && F->je_pohon_pouzivan(E->pohon->n)){E->mGrid->ShowNote(F->ls->Strings[493]);byl_refreshovan=true;}//�Tato zm�na ovlivn� v�echny prvky na tomto pohonu.�
			break;
    }
	}
	if(!byl_refreshovan)E->mGrid->Refresh();
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//odstran� highlight na v�ech tabulk�ch
void TFormX::odstranit_korelaci(bool predat_focus)
{
	//p�ed�v�n� focusu
	if(predat_focus)
		F->DrawGrid_knihovna->SetFocus();//po kliku mimo z�st�val focus po��d na editu

	//pohonov� tabulka
	if(F->PmG!=NULL)
	{
		F->PmG->unHighlightAll();
		if(F->PmG->Note.Text==F->ls->Strings[493])F->PmG->Note.Text="";
	}

	//elementy v objektu
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	Cvektory::T2Element *VYH=F->d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0)
		{
			E->mGrid->unHighlightAll();
			if(E->mGrid->Note.Text==F->ls->Strings[493])E->mGrid->Note.Text="";
		}
		E=E->dalsi;
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYH);
	E=NULL;delete E;

	//p�edchoz� PM
	if(F->predchozi_PM!=NULL)
	{
		F->predchozi_PM->mGrid->unHighlightAll();
		if(F->predchozi_PM->mGrid->Note.Text==F->ls->Strings[493])F->predchozi_PM->mGrid->Note.Text="";
	}
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
		podbarvi_edit(NULL,3,1);
		//kontrola zda je zadan� hodnota v rozmez�
		if(F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do))mimo_rozmezi=false;
		else mimo_rozmezi=true;
  	//zadan� rychlost je mimo rozsah
  	if(mimo_rozmezi && F->OBJEKT_akt->pohon->aRD > 0)
  	{
			F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
			podbarvi_edit(NULL,3,1,false);
			pro_pohon=F->OBJEKT_akt->pohon->n;//ulo�en� pro kter� pohon plat� validace
		}
		if(!mimo_rozmezi && F->PmG->Note.Text!=""){F->PmG->ShowNote("",F->d.clError,14);/*povolit_zakazat_editaci(true);*/}
		// nutn� o�et�en� pro obdob� zad�v�n�/psan�
		if(F->OBJEKT_akt->pohon->aRD > 0 && !pouze_rozmezi)
		{
			//v�po�et doporu�en� rychosti
			double dopRD1=0,dopRD2=0,aRD=F->OBJEKT_akt->pohon->aRD;
			unsigned int n=0;
			do
			{
				//navy�ov�n� nebo sni�ov�n� testovac� rychlosti, tak aby byla v rozsahu pohonu
				if(n!=0)
				{
					if(F->OBJEKT_akt->pohon->aRD>=F->OBJEKT_akt->pohon->rychlost_do || (F->OBJEKT_akt->pohon->aRD<=F->OBJEKT_akt->pohon->rychlost_do && F->OBJEKT_akt->pohon->rychlost_do-F->OBJEKT_akt->pohon->aRD>0 && F->OBJEKT_akt->pohon->rychlost_do-F->OBJEKT_akt->pohon->aRD<F->OBJEKT_akt->pohon->aRD-F->OBJEKT_akt->pohon->rychlost_od))aRD=0.99*aRD;//sni�ov�n� hodnoty o 1%
					else aRD=1.01*aRD;//nav��en� hodnoty o 1%
				}
				//v�po�et doporu�en�ch rychlost�
				dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,aRD);
				dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,aRD);
				//zaps�n� men�� hodnoty jako dopRD
				if(dopRD1>dopRD2)dopRD=dopRD1;//vyp�e vet�� hodnotu
				else dopRD=dopRD2;
				n++;
			}while(!F->m.between(dopRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do) && n<5);
			//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
			if(F->OBJEKT_akt->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
			{
  			F->PmG->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporu�enou rychlost pohonu:"
				podbarvi_edit(NULL,3,1,false);
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
			podbarvi_edit(NULL,3,1,false);
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
		podbarvi_edit(NULL,3,1);
		validovany_pohon=0;//ulo�it pohon na kter�m se to stalo
		//povolit_zakazat_editaci(true);
		//kontrola zda je zadan� hodnota v rozmez�
		if(!F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do))
		{
			F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
      podbarvi_edit(NULL,3,1,false);
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
	//if(posledni_E!=NULL && F->OBJEKT_akt!=NULL && posledni_E->objekt_n!=F->OBJEKT_akt->n)posledni_E=NULL;//pro p��pad, �e se zm�nil n�hled
	if(posledni_E!=NULL && posledni_E->objekt_n==F->OBJEKT_akt->n && posledni_E->eID==0)
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
			double rotace=F->m.Rt90(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(posledni_E));
			if(F->d.v.PP.delka_podvozek<F->m.UDJ(rotace))//voz�ky rovnobe�n� s pohonem, dojde k p�ekryt�
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
		vstoupeno_poh=false;//mus� doj�t k blokaci onchange, jinak se do aRD nahraje zaokrouhlen� hodnota
		unsigned int opraveny_pohon=validovany_pohon;
		vstoupeno_elm=false;
		F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(dopRD),3);
		F->OBJEKT_akt->pohon->aRD=dopRD;
		zmena_aRD();//postar� se o aktualizaci v�ech tabulek
		validace_aRD();//znovuspu�t�n� valiace
		if(opraveny_pohon==validovany_pohon)validovany_pohon=0;//byla odstran�n probl�m
		//refresh + p�ekreslen� voz�k� + aktualizace erroru
		F->duvod_validovat=2;
		F->REFRESH(false);
	}
}
//---------------------------------------------------------------------------
bool TFormX::check_click_Note(double X,double Y,bool check_for_highlight)
{
	bool ret=false;
	//hled�n� zda m� n�kter� element nedokon�enou validaci
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		//hled�n� elementu, kter�mu bylo kliknuto na doporu�en� po�et voz�k�
		if(E->mGrid!=NULL && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(X,Y)==TPoint(-2,-2)){ret=true;break;}
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstran�n� pr�chodov�ho spoj�ku
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
				vstoupeno_elm=false;vstoupeno_poh=false;//mus� doj�t k blokaci onchange, jinak se do aRD nahraje zaokrouhlen� hodnota
				unsigned int opraveny_pohon=validovany_pohon;
				vstoupeno_elm=false;
				Cvektory::TPohon *p=F->d.v.vrat_pohon(validovany_pohon);
				p->aRD=dopRD;
				if(p!=F->OBJEKT_akt->pohon)F->OBJEKT_akt->pohon=p;
				zmena_aRD();//postar� se o aktualizaci v�ech tabulek a znovu spu�t�n� validace
				validace_RD(E);
				if(opraveny_pohon==validovany_pohon)validovany_pohon=0;//byla odstran�n probl�m
				p=NULL;delete p;
			}break;
		}
		//refresh + p�ekreslen� voz�k� + aktualizace erroru
		F->duvod_validovat=2;
		F->REFRESH(false);
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
	if(F->predchozi_PM!=NULL)E=F->predchozi_PM;
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		if(E->mGrid->ID==ID)
		{
			ret=E;
			break;
		}
		if(F->predchozi_PM!=NULL && E==F->predchozi_PM)E=F->OBJEKT_akt->element;
		else E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstran�n� pr�chodov�ho spoj�ku
	if(ret==NULL && F->OBJEKT_akt->teplomery!=NULL)
	{
		Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
		if(T!=NULL && T->posledni->mGrid!=NULL && T->posledni->mGrid->ID==ID)ret=T->posledni;
	}
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
  //aktualizace parametr�
	if(p!=NULL)
	{
		F->OBJEKT_akt->stavPM=2;//roz���it mGridy
		if(p->rezim!=0)
		{
			p->Rz=F->m.Rz(p->aRD);
			p->Rx=F->m.Rx(p->aRD,p->roztec);
		}
		else
		{
			p->Rz=0;
      p->Rx=0;
    }
	}

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
		unsigned long o1=E->objekt_n;
		e=E->dalsi2;//p�i�azuji pohon z v�hybky
		while(e!=NULL && e->identifikator_vyhybka_spojka!=E->identifikator_vyhybka_spojka)
		{
			e->pohon=p;
			if(e->objekt_n!=o1 && e->objekt_n!=o1)F->d.v.vrat_objekt(e->objekt_n)->pohon=p;//pokud je na sekund�rn� v�tvi n�jak� objekt, p�i�ad� mu pohon
			e=e->dalsi;
		}
		update_hodnot_vyhybky_PM(E);//nutno ud�lat ru�n�, metoda aktualizace_tab_elementu(), aktualizuje pouze elementy na stejn�m pohonu, co vyhybka nen�, m� pohon hlavn� v�tve
		//WT �e�� fce. vlo�en� PM
	}

	////na�ten� dat z pohonu do mGridu
  mazatPM(E);//kontrola a dotaz na maz�n� elementu + ovlivn�n�ch PM
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT v�em element�, mus� b�t prvn�
	input_state=COMBO;//d�le�it� pro nastaven� enabled komponent v aktualizaci element�
	if(p!=NULL)aktualizace_tab_elementu();
	else aktualizace_tab_elementu_pOdebran();
	F->d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teplom�ry aktualizuje ji, jinak vytvo�� default cestu

	//vytvo�en� cesty teplom�r� po p�i�azen� pohonu
	Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
	if(F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8 && T==NULL)
	{
		F->d.v.vytvor_default_c_teplomery(F->OBJEKT_akt);
		F->vytvor_aktualizuj_tab_teplomeru();
	}
	//pokud dojde k odstran�n� pohonu, dojde ke smaz�n� teplom�r�
	if((F->OBJEKT_akt->pohon==NULL ||  !(F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8)) && T!=NULL)
	{
		F->d.v.vymaz_teplomery(F->OBJEKT_akt,T);
		delete T;T=NULL;
	}
	T=NULL;delete T;

  ////aktualizace knihoven
	F->DrawGrid_knihovna->Refresh();
	F->DrawGrid_ostatni->Refresh();
	F->DrawGrid_otoce->Refresh();
	F->DrawGrid_geometrie->Refresh();

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
			if(input_state==NOTHING && ((E->objekt_n==F->OBJEKT_akt->n && E->pohon->rezim!=0) || (E->objekt_n!=F->OBJEKT_akt->n && E->pohon->rezim!=0)))//p�epo�et po p�id�n� KK elementu, pouze p�i input_state==NOTHING, tz. nep�epo��t�vat p�i u�ivatelsk� zm�n�
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
			E->mGrid->Cells[prvni][3].Text="-";
  		E->mGrid->Cells[prvni][4].Text="-";
  		E->mGrid->Cells[prvni][5].Text="-";
  		E->mGrid->Cells[prvni][6].Text="-";
  		E->mGrid->Cells[prvni][7].Text="-";
  		E->mGrid->Cells[prvni][8].Text="-";
  		E->mGrid->Cells[prvni][9].Text="-";
			E->mGrid->Cells[prvni][10].Text="-";
      E->mGrid->Cells[prvni][11].Text="-";
  	}
  	if(E->eID==300)//pro v�hybku
		{
    	if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)
			{
				if(input_state==NOTHING && E->dalsi2->pohon->rezim!=0)//p�epo�et po p�id�n� KK elementu
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
				if(E->WT1!=0)E->mGrid->Cells[druhy][11].Text=F->m.round2double(F->outPT(E->WT1),3);
				else E->mGrid->Cells[druhy][11].Text="-";
    	}
    	else
			{
				E->mGrid->Cells[druhy][3].Text="-";
				E->mGrid->Cells[druhy][4].Text="-";
				E->mGrid->Cells[druhy][5].Text="-";
				E->mGrid->Cells[druhy][6].Text="-";
				E->mGrid->Cells[druhy][7].Text="-";
				E->mGrid->Cells[druhy][8].Text="-";
				E->mGrid->Cells[druhy][9].Text="-";
				E->mGrid->Cells[druhy][10].Text="-";
        E->mGrid->Cells[druhy][11].Text="-";
  		}
  	}
  	else//pro PM
		{
      Cvektory::TElement *e_pom=E->dalsi;
			if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;//v p��pad� PM na konci linky
			if(e_pom->pohon!=NULL)
			{
				if(input_state==NOTHING && e_pom->pohon->rezim!=0)//p�epo�et po p�id�n� KK elementu
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
				E->mGrid->Cells[druhy][11].Text=F->m.round2double(F->outPT(E->WT1),3);//pokud existuje druh� pohon, v�dy bude WT + mohlo doj�t ke zm�n�
			}
			else
			{
				E->mGrid->Cells[druhy][3].Text="-";
				E->mGrid->Cells[druhy][4].Text="-";
				E->mGrid->Cells[druhy][5].Text="-";
				E->mGrid->Cells[druhy][6].Text="-";
				E->mGrid->Cells[druhy][7].Text="-";
				E->mGrid->Cells[druhy][8].Text="-";
				E->mGrid->Cells[druhy][9].Text="-";
				E->mGrid->Cells[druhy][10].Text="-";
        E->mGrid->Cells[druhy][11].Text="-";
  		}
			e_pom=NULL;delete e_pom;
		}

		//update co lze editovat
		F->zmena_editovanych_bunek(E);

		//update rozbalen� zabalen� tabulek
		//zobrazit_skryt_radkyPM(E);
	}
}
//---------------------------------------------------------------------------
//provede validaci RD
void TFormX::validace_RD(Cvektory::TElement *E)
{
	//smaz�n� p�edchoz� validace z VID
	zapisVID(0,1);//pozice jsou pops�ny v .h u deklarace VID a vn� metody zapisVID()

	//deklarace
	AnsiString jednotky;
	if(F->aRDunit==0)jednotky="[m/s]";
	else jednotky="[m/min]";
	AnsiString puv_Note="";
	if(E!=NULL)puv_Note=E->mGrid->Note.Text;
	bool mimo_rozmezi=false;
	//zji�t�n� n pohon� v tabulce
	unsigned int pro_pohon=0;
	dopRD=0;

	//validace pouze pohonu v elementu jin�m ne� PM
	if(E!=NULL && F->OBJEKT_akt!=NULL && E->eID!=200 && E->eID!=300)
	{
		if(F->OBJEKT_akt->pohon!=E->pohon)F->OBJEKT_akt->pohon=E->pohon;
		E=F->OBJEKT_akt->element;
		Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
		//hled�n� PM se stejn�m pohonem
		while(E!=NULL)
		{
			if(E->eID==200 && (E->pohon==F->OBJEKT_akt->pohon || (E->dalsi!=NULL && E->dalsi->pohon==F->OBJEKT_akt->pohon) || (E->dalsi==NULL && F->d.v.ELEMENTY->dalsi->pohon==F->OBJEKT_akt->pohon)))break;
			if(E->eID==300 && (E->pohon==F->OBJEKT_akt->pohon || (E->dalsi2!=NULL && E->dalsi2->pohon==F->OBJEKT_akt->pohon)))break;
			E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
		}
		F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
    //pokud nebylo nalezeno, hled�m v p�edchoz�m PM
		if(E==NULL && F->predchozi_PM!=NULL && (F->predchozi_PM->pohon==F->OBJEKT_akt->pohon || (F->predchozi_PM->dalsi!=NULL && F->predchozi_PM->dalsi->pohon==F->OBJEKT_akt->pohon) || (F->predchozi_PM->dalsi==NULL && F->d.v.ELEMENTY->dalsi->pohon==F->OBJEKT_akt->pohon)))E=F->predchozi_PM;
	}
	//validovat pouze u PM
	if(E!=NULL && F->OBJEKT_akt!=NULL && (E->eID==200 || E->eID==300))// && F->OBJEKT_akt->pohon->aRD!=0)
	{
		//na�ten� pohon�
		Cvektory::TElement *e_dalsi=NULL;
		if(E->eID==300)e_dalsi=E->dalsi2;
		else if(E->dalsi!=NULL)e_dalsi=E->dalsi;
		else e_dalsi=F->d.v.ELEMENTY->dalsi;
		Cvektory::TPohon *p=NULL,*p1=E->pohon,*p2=e_dalsi->pohon;
		e_dalsi=NULL;delete e_dalsi;
		int Col=3;

		//dvojt� validace
		//pokud nalezne probl�m zastav� se a zobraz�ho, i v p��pad�, �e je probl�m� v�ce, a� bude probl�m vy�e�en prob�hne validace zda neexistuje dal�� probl�m
		for(unsigned int i=3;i<=4;i++)
		{
			//na�ten� po�adovan�ho pohonu pro validaci
			if(i==3)p=p1;
			if(i==4)p=p2;
			Col=i;if(F->prohodit_sloupce_PM(E))
			{
				if(Col==3)Col=4;
				else Col=3;
      }
			//kontrola zda existuje pohon, pokud ne nem� smysl �e�it
			if(p!=NULL && F->OBJEKT_akt->rezim!=0)
			{
				//kontrola zda je mo�n� editovat pohon
				//if(E->mGrid->Cells[i][3].Type==E->mGrid->EDIT && p!=NULL)
				{
					podbarvi_edit(E,Col,3);//defaultn� nastavn� barev
					String Rx1=F->m.round2double(p->Rx,0),Rx2=p->Rx;
					//kontrola zda je zadan� hodnota v rozmez�
					if(F->m.between(p->aRD,p->rychlost_od,p->rychlost_do))mimo_rozmezi=false;
					else mimo_rozmezi=true;
					//zadan� rychlost je mimo rozsah
					if(mimo_rozmezi && p->aRD > 0)
					{
						E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
						podbarvi_edit(E,Col,3,false);
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
						double dopRD1=0,dopRD2=0,aRD=p->aRD;
						unsigned int n=0;
						do
						{
		  				//navy�ov�n� nebo sni�ov�n� testovac� rychlosti, tak aby byla v rozsahu pohonu
							if(n!=0)
							{
								if(p->aRD>=p->rychlost_do || (p->aRD<=p->rychlost_do && p->rychlost_do-p->aRD>0 && p->rychlost_do-p->aRD<p->aRD-p->rychlost_od))aRD=0.99*aRD;//sni�ov�n� hodnoty o 1%
								else aRD=1.01*aRD;//nav��en� hodnoty o 1%
							}
							//v�po�et doporu�en�ch rychlost�
							dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,p->roztec,F->d.v.PP.TT,aRD);
							dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,p->roztec,F->d.v.PP.TT,aRD);
							//zaps�n� men�� hodnoty jako dopRD
							if(dopRD1>dopRD2)dopRD=dopRD1;//vyp�e vet�� hodnotu
							else dopRD=dopRD2;
							n++;
						}while(!F->m.between(dopRD,p->rychlost_od,p->rychlost_do) && n<5);
						//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
						if(p->roztec>0 && dopRD!=p->aRD && Rx1!=Rx2 && mimo_rozmezi==false)
						{
							//if(E->mGrid->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
							E->mGrid->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporu�enou rychlost pohonu:"
							pro_pohon=p->n;//ulo�en� pro kter� pohon plat� validace
//							F->Memo("p1: "+p1->name+"; p2: "+p2->name);
//							F->Memo("pro pohon: "+p->name+"; i: "+String(i)+"; Col: "+String(Col));
							podbarvi_edit(E,Col,3,false);//�erven� podbarven�
							break;//byl nalezen probl�m, zastaven� validace, lze zobrazit jen jeden probl�m v Note
						}
						//v�e je vpo��dku
						if ((dopRD==p->aRD || Rx1==Rx2) && mimo_rozmezi==false)
						{
							//povolit_zakazat_editaci(true);
							E->mGrid->ShowNote("",F->d.clError,14);
						}
					}
					else E->mGrid->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatn� hodnota rychlosti pohonu!"
				}
			}
			//vaidace pro ostatn� re�imy objektu, nutn� kontrolovat rozmez� rychlosti
			else
			{
				if(p!=NULL && !F->m.between(p->aRD,p->rychlost_od,p->rychlost_do))
				{
					E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
					pro_pohon=p->n;//ulo�en� pro kter� pohon plat� validace
				}
				else
				{
					E->mGrid->ShowNote("",F->d.clError,14);
					//povolit_zakazat_editaci(true);
				}
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
			int prvni=3,druhy=4;
			Cvektory::TElement *e_pom=F->OBJEKT_akt->element;
			if(F->predchozi_PM!=NULL)e_pom=F->predchozi_PM;
			Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();//vytvo�en� pr�chodov�ho spoj�ku
			while(e_pom!=NULL)
			{
				try
				{
					if(e_pom!=E && (e_pom->eID==200 || e_pom->eID==300))
					{
						//rozmno�en� note po ostatn�ch pm
						if(F->prohodit_sloupce_PM(e_pom)){prvni=4;druhy=3;}
						else {prvni=3;druhy=4;}
						//kontrola prvn�ho sloupce
						if(e_pom->pohon!=NULL && e_pom->pohon->n==validovany_pohon)
						{
							e_pom->mGrid->ShowNote(puv_Note,F->d.clError,14);
							if(puv_Note!="")podbarvi_edit(e_pom,prvni,3,false);//�erven� podbarven�
							else podbarvi_edit(e_pom,prvni,3);//default podbarven�
						}
            //kontrola druh�ho sloupce
						else
						{
							if(e_pom->eID==300)e_dalsi=e_pom->dalsi2;
							else if(e_pom->dalsi!=NULL)e_dalsi=e_pom->dalsi;
							else e_dalsi=F->d.v.ELEMENTY->dalsi;
							if(e_dalsi->pohon!=NULL && e_dalsi->pohon->n==validovany_pohon)
							{
                e_pom->mGrid->ShowNote(puv_Note,F->d.clError,14);
								if(puv_Note!="")podbarvi_edit(e_pom,druhy,3,false);//�erven� podbarven�
								else podbarvi_edit(e_pom,druhy,3);//default podbarven�
              }
            }
          }
				}catch(...){;}
        //p�esun na dal��
				if(e_pom==F->predchozi_PM && F->predchozi_PM!=NULL)e_pom=F->OBJEKT_akt->element;
				else e_pom=F->d.v.dalsi_krok(VYHYBKY,e_pom,F->OBJEKT_akt);
			}
      //ukazatelov� z�le�itosti
			F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstran�n� pr�chodov�ho spoj�ku
			e_pom=NULL;delete e_pom;
      e_dalsi=NULL;delete e_dalsi;
		}

		//ukazatelov� z�le�itosti
		p=NULL;p1=NULL;p2=NULL;
		delete p;delete p1;delete p2;
	}
	validovat_pohon=false;
}
//---------------------------------------------------------------------------
//nastav� defautln� barvy editu a bu�ce, nebo podbarv� bu�ku
void TFormX::podbarvi_edit(Cvektory::TElement *E,long Col,long Row,bool def_nastaveni)
{
	//nasteven� podkresov� barvy
	TColor barva=F->m.clIntensive(clRed,210);
	TscGPEdit *Edit=NULL;
	//na�ten� editu
  if(E!=NULL)Edit=E->mGrid->getEdit(Col,Row);
	if(E==NULL)Edit=F->PmG->getEdit(Col,Row);
	//na�ten� default barvy
	if(def_nastaveni)
	{
		if(Edit==NULL || (Edit!=NULL && !Edit->Enabled))barva=(TColor)RGB(240,240,240);
		else barva=clWhite;
	}

  //kontrola existence elementu a jeho mrgridu
	if(E!=NULL && E->mGrid!=NULL)
	{
		//z�sk�n� editu
		if(Edit==NULL)Edit=E->mGrid->getEdit(Col,Row);

		//nastaven� barev editu
		if(Edit!=NULL)
  	{
			Edit->Options->FocusedColor=barva;
			Edit->Options->NormalColor=barva;
			Edit->Options->HotColor=barva;
		}

		//nastaven� barev bu�ky, nutn�!!
		E->mGrid->Cells[Col][Row].Background->Color=barva;
	}

	//kontrola existence PmG
	if(E==NULL && F->PmG!=NULL)
	{
    //z�sk�n� editu
		if(Edit==NULL)Edit=F->PmG->getEdit(Col,Row);

		//nastaven� barev editu
		if(Edit!=NULL)
  	{
			Edit->Options->FocusedColor=barva;
			Edit->Options->NormalColor=barva;
			Edit->Options->HotColor=barva;
		}

		//nastaven� barev bu�ky, nutn�!!
		F->PmG->Cells[Col][Row].Background->Color=barva;
	}

	Edit=NULL;delete Edit;
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
	//aktualizace parametr�
	if(p!=NULL)
	{
		F->OBJEKT_akt->stavPM=2;//roz���it mGridy
		if(p->rezim!=0)
		{
			p->Rz=F->m.Rz(p->aRD);
			p->Rx=F->m.Rx(p->aRD,p->roztec);
		}
		else
		{
			p->Rz=0;
			p->Rx=0;
    }
	}

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
		while(e!=NULL && (e->objekt_n==F->OBJEKT_akt->n || (E==F->predchozi_PM && e->dalsi!=NULL && e->dalsi->objekt_n==F->OBJEKT_akt->n)))
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
		if(E->dalsi!=NULL && E->dalsi->pohon!=NULL && E->dalsi->objekt_n==E->objekt_n)E->WT1=F->m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
	}

	////na�ten� dat z pohonu do mGridu
	mazatPM(E);//kontrola a dotaz na maz�n� elementu + ovlivn�n�ch PM
	F->d.v.aktualizuj_rezim_pohonu();//aktualizuje re�imy v�em pohon�m
	F->vlozit_predavaci_misto_aktualizuj_WT();//mus� b�t prvn�!!
	input_state=COMBO;//d�le�it� pro nastaven� enabled komponent v aktualizaci element�
	if(p!=NULL)aktualizace_tab_elementu();
	else aktualizace_tab_elementu_pOdebran();
	F->d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teplom�ry aktualizuje ji, jinak vytvo�� default cestu

  //vytvo�en� cesty teplom�r� po p�i�azen� pohonu
	Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
	if(F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8 && T==NULL)
	{
		F->d.v.vytvor_default_c_teplomery(F->OBJEKT_akt);
		F->vytvor_aktualizuj_tab_teplomeru();
	}
	//pokud dojde k odstran�n� pohonu, dojde ke smaz�n� teplom�r�
	if((F->OBJEKT_akt->pohon==NULL ||  !(F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8)) && T!=NULL)
	{
		F->d.v.vymaz_teplomery(F->OBJEKT_akt,T);
		delete T;T=NULL;
	}
	T=NULL;delete T;

	////povolen� / z�kaz p�epnut� k�t
	if(F->OBJEKT_akt->pohon!=NULL && !F->scGPComboBox_prepinacKot->Enabled)F->scGPComboBox_prepinacKot->Enabled=true;
	if(F->OBJEKT_akt->pohon==NULL && F->scGPComboBox_prepinacKot->Enabled)
	{
		F->scGPComboBox_prepinacKot->Enabled=false;
		if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
	}

	////aktualizace knihoven
	F->DrawGrid_knihovna->Refresh();
	F->DrawGrid_ostatni->Refresh();
	F->DrawGrid_otoce->Refresh();
	F->DrawGrid_geometrie->Refresh();

  ////ukazatelov� z�le�itosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;

	//vyps�n� upozorn�n� k pou��van�m pohon�m
	popisky_pouzivany_pohon=true;
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
	if(p!=NULL && p->rezim!=0)
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
  F->d.v.aktualizuj_rezim_pohonu();//aktualizuje re�imy v�em pohon�m
	if(p==NULL)aktualizace_tab_elementu_pOdebran();//aktualizace tabulek elemnt�
	else aktualizace_tab_elementu();//do�lo ke zm�n�m v tabulce pohonu, kter� ovlivn� i ostatn� elementy

	//kontrola PM
	F->OBJEKT_akt->stavPM=2;//max po zvolen� pohonu
	F->vlozit_predavaci_misto_aktualizuj_WT();
	F->aktualizace_tab_pohon(false,true,true);//nebude provedena pokud dojde k odstran�n� PmG
	F->d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teplom�ry aktualizuje ji, jinak vytvo�� default cestu

  //vytvo�en� cesty teplom�r� po p�i�azen� pohonu
	Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
	if(F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8 && T==NULL)
	{
		F->d.v.vytvor_default_c_teplomery(F->OBJEKT_akt);
		F->vytvor_aktualizuj_tab_teplomeru();
	}
	//pokud dojde k odstran�n� pohonu, dojde ke smaz�n� teplom�r�
	if((F->OBJEKT_akt->pohon==NULL ||  !(F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8)) && T!=NULL)
	{
		F->d.v.vymaz_teplomery(F->OBJEKT_akt,T);
		delete T;T=NULL;
	}
	T=NULL;delete T;

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

	//povolen� / z�kaz p�epnut� k�t
	if(F->OBJEKT_akt->pohon!=NULL && !F->scGPComboBox_prepinacKot->Enabled)F->scGPComboBox_prepinacKot->Enabled=true;
	if(F->OBJEKT_akt->pohon==NULL && F->scGPComboBox_prepinacKot->Enabled)
	{
		F->scGPComboBox_prepinacKot->Enabled=false;
		if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
	}

	//ukazatelov� z�le�itosti
	E=NULL;p=NULL;
	delete E;delete p;

  //vyps�n� upozorn�n� k pou��van�m pohon�m
	popisky_pouzivany_pohon=true;
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

	//kontrola zda mazat Element
	if(Element!=NULL && ((Element->dalsi!=NULL && Element->pohon==Element->dalsi->pohon) || (Element->dalsi==NULL && Element->pohon==F->d.v.ELEMENTY->dalsi->pohon)))mazat_element=true;
	//hled�n� ovlivn�n�ho PM v cel�m objektu
	Cvektory::T2Element *VYH=F->d.v.hlavicka_seznam_VYHYBKY();
	E=F->OBJEKT_akt->element;
	if(F->predchozi_PM!=NULL)E=F->predchozi_PM;
	while(E!=NULL)
	{
		//kontrola zda jsem narazil na PM a je mo�n� ho smazat
		if(E->eID==200 && Element!=E && ((E->dalsi!=NULL && E->pohon==E->dalsi->pohon) || (E->dalsi==NULL && E->pohon==F->d.v.ELEMENTY->dalsi->pohon)))
		{
			if(Element==NULL)
			{
				Element=E;
				mazat_element=true;
			}
			else nalezen=true;
			break;
		}
		//p�esun na dal��
		if(F->predchozi_PM!=NULL && E==F->predchozi_PM)E=F->OBJEKT_akt->element;
		else E=F->d.v.dalsi_krok(VYH,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYH);

  //ulo�en� posunu a zoom p�ed posuny na elementy
	TPointD predchozi_posun=F->Posun;
	double predchozi_zoom=F->Zoom;

	//dotaz + smaz�n� samotn�ho PM kde byla vyvolan� zm�na
	aut_mazani_PM=true;
	Cvektory::TElement *smaz=NULL;//pom ukazatel pro maz�n� pr�zdn� zar�ky p�ed obloukem
	String name="";
	if(nalezen && E!=NULL)name=E->name;
	//kontrola, zda je mo�n� smazat Element
	if(Element!=NULL && ((Element->dalsi!=NULL && Element->pohon!=Element->dalsi->pohon) || (Element->dalsi==NULL && Element->pohon!=F->d.v.ELEMENTY->dalsi->pohon)))Element=NULL;
	if(Element==E && E!=NULL)E=NULL;
	if(mazat_element && Element!=NULL && Element->eID==200)//zm�na m��e b�t vyvol�na i z v�hybky
	{
		if(Element->dalsi!=NULL && Element->dalsi->geo.delka==0)smaz=Element->dalsi;
		F->posun_na_element(Element);
		F->REFRESH(false);//mus� n�sledovat p�esunu
		F->pom_element=Element;
		F->pom_element_temp=Element;
		//podstr�en� sou�adnic pro zobrazen� MB
		F->akt_souradnice_kurzoru_PX.x=F->m.L2Px(Element->X);
		F->akt_souradnice_kurzoru_PX.y=F->m.L2Py(Element->Y);
		F->Smazat1Click(this);
		if(smaz!=NULL)F->d.v.smaz_element(smaz);
		delete smaz;smaz=NULL;
	}

	//kontrola zde ovlivn�n� PM existuje + dotaz na maz�n�
	if(nalezen && mazat_element && E!=NULL)
	{
		if(E->dalsi!=NULL && E->dalsi->geo.delka==0)smaz=E->dalsi;
		F->posun_na_element(E);
		F->REFRESH(false);//mus� n�sledovat p�esunu
		E->name=name;//pokud do�lo k odstran�n� nap��klad p�edchoz�ho PM, do�lo ji� i k aktualizaci n�zv�, nap� v praxi sma�u prvn� predchoz� PM "PM1" a druhy dotaz na Element, kter� b�val "PM2" bude: "Chcete smazat PM1"
		F->pom_element=E;
		F->pom_element_temp=E;
		//podstr�en� sou�adnic pro zobrazen� MB
		F->akt_souradnice_kurzoru_PX.x=F->m.L2Px(E->X);
		F->akt_souradnice_kurzoru_PX.y=F->m.L2Py(E->Y);
		F->Smazat1Click(this);
		if(smaz!=NULL)F->d.v.smaz_element(smaz);
    delete smaz;smaz=NULL;
	}

  //navr�cen� stavu
	aut_mazani_PM=false;

	//ulo�en� posunu a zoom p�ed posuny na elementy
	F->Posun=predchozi_posun;
	F->Zoom=predchozi_zoom;

	//ukazatelov� z�le�itosti
  delete smaz;smaz=NULL;
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//zobraz� �i skryje ��dky PM tabulek podle stavu ulo�en�ho v objektu
void TFormX::zobrazit_skryt_radkyPM(Cvektory::TElement *E)
{
  //update rozbalen� zabalen� tabulek
	switch(F->OBJEKT_akt->stavPM)
	{
		//defaultn� stav zobrazeny comba a rychlosti
		case 0:
		{
			if(!E->mGrid->Rows[3].Visible)
			{
				E->mGrid->VisibleRow(3,true,false);
				E->mGrid->VisibleRow(4,true,false);
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkUpArrow;
				//zmen�en� tabulky, �spora m�sta
				E->mGrid->Columns[0].Width+=20;
				E->mGrid->Columns[1].Width+=20;
				E->mGrid->Columns[2].Width+=40;
			}
			if(E->mGrid->Rows[5].Visible)
			{
				E->mGrid->VisibleRow(5,false,false);
				E->mGrid->VisibleRow(6,false,false);
				E->mGrid->VisibleRow(7,false,false);
				E->mGrid->VisibleRow(8,false,false);
				E->mGrid->VisibleRow(9,false,false);
				E->mGrid->VisibleRow(10,false,false);
				E->mGrid->VisibleRow(11,false,false);
				E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			}
			break;
		}
		//minimized, skryt� v�eho krom� hlavi�ky a comb
		case 1:
		{
			if(E->mGrid->Rows[3].Visible)
			{
				E->mGrid->VisibleRow(3,false,false);
				E->mGrid->VisibleRow(4,false,false);
				E->mGrid->VisibleRow(5,false,false);
		  	E->mGrid->VisibleRow(6,false,false);
		  	E->mGrid->VisibleRow(7,false,false);
				E->mGrid->VisibleRow(8,false,false);
				E->mGrid->VisibleRow(9,false,false);
		  	E->mGrid->VisibleRow(10,false,false);
				E->mGrid->VisibleRow(11,false,false);
				E->mGrid->exBUTTONVisible=false;
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkDownArrow;
				//zmen�en� tabulky, �spora m�sta
				E->mGrid->Columns[0].Width-=20;
				E->mGrid->Columns[1].Width-=20;
				E->mGrid->Columns[2].Width-=40;
			}
			break;
		}
		//v�e zobrazeno
		case 2:
		{
			if(!E->mGrid->Rows[3].Visible)
			{
				E->mGrid->VisibleRow(3,true,false);
				E->mGrid->VisibleRow(4,true,false);
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkUpArrow;
				//zmen�en� tabulky, �spora m�sta
				E->mGrid->Columns[0].Width+=20;
				E->mGrid->Columns[1].Width+=20;
				E->mGrid->Columns[2].Width+=40;
			}
			if(!E->mGrid->Rows[5].Visible)
			{
				E->mGrid->VisibleRow(5,true,false);
				E->mGrid->VisibleRow(6,true,false);
				E->mGrid->VisibleRow(7,true,false);
				E->mGrid->VisibleRow(8,true,false);
				E->mGrid->VisibleRow(9,true,false);
				E->mGrid->VisibleRow(10,true,false);
				E->mGrid->VisibleRow(11,true,false);
			}
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			F->napln_comba_mGridu(E);//nutn� pro napln�n� comb WT
			break;
		}
	}
}
//---------------------------------------------------------------------------
//provede aktualizaci �asu v tabulce teplom�r�
void TFormX::aktualizace_teplomeru()
{
	if(F->OBJEKT_akt->teplomery!=NULL)
	{
		Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
		if(T!=NULL)
		{
      //maz�n� v�ech r�dk� krom� hlavi�ky
			for(unsigned int i=T->posledni->mGrid->RowCount-1;i>0;i--)
			{
				T->posledni->mGrid->DeleteRow(i,false);
			}
      ////existuje ... aktualizace ��dk�
			double cas=0,WT=0,delka=0;
			int pocet_voziku=0;
			bool prejezd=true;
			//v�po�et �asu na za��tku
			if(T->prvni->sparovany->pohon!=NULL)
			{
				//v�po�et d�lky oblasti
				delka=F->m.delka(T->prvni->geo.X1,T->prvni->geo.Y1,T->prvni->sparovany->geo.X4,T->prvni->sparovany->geo.Y4);
				delka=F->m.castPrepony(delka,T->prvni->sparovany->geo.delkaPud,T->prvni->sparovany->geo.HeightDepp);
				if(T->prvni->sparovany==T->posledni->sparovany)delka=F->m.delka(T->prvni->geo.X1,T->prvni->geo.Y1,T->posledni->geo.X4,T->posledni->geo.Y4);//pokud jsou oba teplom�ry na stejn�m �seku, m��it d�lku mezi jejich sou�adnicemi
				//pokud se jedn� o stopku po��tat s bufferem
				if(T->prvni->sparovany->eID==0)
				{
					//v�po�et d�lky bufferu
					double buf=T->prvni->sparovany->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice;
					//pokud je �sek men�� ne� d�lka bufferu p�ipo�ten� wt podle toho na jak�m jsem voz�ku
					if(delka<=buf)
					{
						prejezd=false;
						pocet_voziku=ceil((buf-delka)/F->d.v.PP.delka_podvozek);
						if(pocet_voziku<0)pocet_voziku=T->prvni->sparovany->data.pocet_voziku+pocet_voziku;
						cas+=F->m.V2WT(pocet_voziku,F->d.v.PP.TT);//p�ipo��t�n� WT na aktu�ln�m voz�ku
					}
					//pokud je �sek v�t�� ne� buffer p�ipo�ten� WTstop a p�ejezdu
					else
					{
						delka=delka-buf;
						cas+=delka/T->prvni->sparovany->pohon->aRD;
						//rozpad na p�ejezd a buffer
						prejezd=true;
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;
						prejezd=false;
						cas+=T->prvni->sparovany->data.WTstop;
					}
				}
				else cas+=delka/T->prvni->sparovany->pohon->aRD;
				WT+=T->prvni->sparovany->WT1;
			}

			//v�po�et �asu na cest�
			Cvektory::TCesta *CE=T->cesta->dalsi;
			while(CE!=NULL)
			{
				if(CE->Element->pohon!=NULL)
				{
					if(CE->Element->eID==0)
					{
						//v�po�et �asu
						if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl p�ed t�m buffer, zm�na, pot�ebuju zapsat p�ejezd
						delka=CE->Element->geo.delka-(CE->Element->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice);
						if(delka<0)delka=0;//pokud je d�lka z�porn�, znamen� to, �e v bufferu je PM, nep�id�vat ��dn� �as
						cas+=delka/CE->Element->pohon->aRD;
						prejezd=true;
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//zaps�n� ��sti p�ejezdu
						prejezd=false;
						cas+=CE->Element->data.WTstop;
					}
					else
					{
						if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl p�ed t�m buffer, zm�na, bude n�sledovat p�ejezd
						prejezd=true;
						cas+=CE->Element->geo.delka/CE->Element->pohon->aRD;
						//kontrola zda dal�� nen� stopka a aktu�ln� element nen� v bufferu
						if(CE->dalsi!=NULL && CE->dalsi->Element->eID==0 && CE->dalsi->Element->geo.delka<CE->dalsi->Element->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice)
						{
              //element je v bufferu ode��st ��st bufferu
							delka=CE->dalsi->Element->geo.delka-(CE->dalsi->Element->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice);
							cas+=delka/CE->Element->pohon->aRD;
            }
					}
					WT+=CE->Element->WT1;
				}
				CE=CE->dalsi;
			}
			//ukazatelov� z�le�itosti
			CE=NULL;delete CE;

			//v�po�et �asu na konci
			if(T->posledni->sparovany->pohon!=NULL && T->posledni->sparovany!=T->prvni->sparovany)
			{
				//v�po�et d�lky oblasti
				delka=F->m.delka(T->posledni->sparovany->geo.X1,T->posledni->sparovany->geo.Y1,T->posledni->geo.X4,T->posledni->geo.Y4);
				delka=F->m.castPrepony(delka,T->posledni->sparovany->geo.delkaPud,T->posledni->sparovany->geo.HeightDepp);
				//pokud se jedn� o stopku po��tat s bufferem
				if(T->posledni->sparovany->eID==0)
				{
					//v�po�et d�lky bufferu
					double buf=T->posledni->sparovany->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice;
					buf=T->posledni->sparovany->geo.delka-buf;
					//pokud je �sek men�� ne� d�lka bufferu p�ipo�ten� wt podle toho na jak�m jsem voz�ku
					if(delka>buf)
					{
						if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl p�ejezd, zm�na, bude n�sledovat buffer
						prejezd=true;
						cas+=buf/T->posledni->sparovany->pohon->aRD;
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//pokud byl p�ejezd, zm�na, bude n�sledovat buffer
						prejezd=false;
						WT+=T->posledni->sparovany->WT1;
						pocet_voziku=ceil((delka-buf)/F->d.v.PP.delka_podvozek);
						if(pocet_voziku<0)pocet_voziku=T->prvni->sparovany->data.pocet_voziku+pocet_voziku;
						cas+=F->m.V2WT(pocet_voziku,F->d.v.PP.TT);//p�ipo��t�n� WT na aktu�ln�m voz�ku
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;
					}
					//pokud je �sek v�t�� ne� buffer p�ipo�ten� WTstop a p�ejezdu
					else
					{
						cas+=delka/T->posledni->sparovany->pohon->aRD;
						prejezd=true;
						//rozpad na p�ejezd a buffer
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//zaps�n� ��sti p�ejezdu
					}
				}
				else//nen� stop
				{
					if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}
					prejezd=true;
					cas+=delka/T->posledni->sparovany->pohon->aRD;
					WT+=T->posledni->sparovany->WT1;
				}
			}
			//kontrola zda nejsou je�t� k vyps�n� n�jak� hodnoty, m��e nastat, kdy� budou oba teplom�ry na jedn� stoce
			if(cas>0 || WT>0){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}
			//vlo�en� ��dku sou�tu
			F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,false,true);
		}

		//ukazatelov� z�le�itosti
		T=NULL;delete T;
	}
}
//---------------------------------------------------------------------------
//aktualizuje polo�ky v pohonov�ch tabulk�ch, v p��pad�, �e do�lo ke zm�n� re�imu pohonu
void TFormX::zmena_rezimu_pohonu(Cvektory::TPohon *pohon)
{
	//deklarace
	bool probehla_validace=false;
	Cvektory::TElement *E=F->OBJEKT_akt->element;

	//aktualizace parametr� pohonu
	if(pohon==NULL)pohon=F->OBJEKT_akt->pohon;
	if(pohon!=NULL)
	{
		if(pohon->rezim==0)
		{
			pohon->Rx=0;
			pohon->Rz=0;
		}
		else
		{
			pohon->Rz=F->m.Rz(pohon->aRD);
			pohon->Rx=F->m.Rx(pohon->aRD,pohon->roztec);
    }
	}

	//pr�chod skrze elementy v objektu
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();//vytvo�en� pr�chodov�ho spoj�ku
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->eID==200 || E->eID==300)
		{
			update_hodnot_vyhybky_PM(E);//provede aktualizaci dat a editovan�ch polo�ek v mGridu
			if(!probehla_validace && E->eID==200)//spou�t�t validaci jen jednou a to pokud naraz�m na PM
			{
				validace_RD(E);//pokud byl zm�n�n re��m provede validaci aktu�ln� rychlosti
				probehla_validace=true;//zapsan�, �e validace prob�hla
			}
		}
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}

  //aktualizace tabulek mimo objekt, tj. p�edchoz� PM a PmG
	if(F->predchozi_PM!=NULL)update_hodnot_vyhybky_PM(F->predchozi_PM);//provede aktualizaci dat a editovan�ch polo�ek v mGridu u p�edchoz�ho PM
	F->aktualizace_tab_pohon(false,true,true);//obsahuje podm�nku.. pokud existuje PmG

	//ukazatelov� z�le�itosti
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstran�n� pr�chodov�ho spoj�ku
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//z ��sla VIDu vr�t� jeho textov� popis
UnicodeString TFormX::getVID(unsigned int VID)
{
	UnicodeString Text="";
	switch(VID)
	{
    default:break;
  }
	return Text;
}
//---------------------------------------------------------------------------
