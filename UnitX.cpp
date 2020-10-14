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
{ //výchozí nastavení - pøi zobrazení tab.elementù je totiž vždy volán Onchange, pøi naèítání hodnot do buòek
	// proto je nastaven input_state=NO, aby v tento moment neprobíhal žádný výpoèet v Onchange události
  input_state=NO;
  vstoupeno_poh=false;
  vstoupeno_elm=false;
  validace_true=false;
  editace_pohonu=false;
  //nastavení pozic bunìk v tabulce pohonu
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
	VID="00";//zde se definuje poèet èíslic obažených ve VID
	aut_mazani_PM=false;//je true pouze v èase automatického odmazávání PM
	popisky_pouzivany_pohon=false;
  vykresli_vetev=0;
	//pokud dojde ke zmìnì poøadí øádku, nastavit nové pozice zde, + pøepsání switche pro tabulku pohonu v OnChange + pøepsaní switche v korelace_tab_pohonu()
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
  //pøi kliku do nìjaké buòky nastavím input_state=NOTHING, pokud udìlám zmìnu buòky je v OnChange události switch, který zajistí
	//výpoèet konkrétní buòky dle pøedávaných parametrù v události
	input_state=NO;
	if(ID==6666)F->tab_knihovna_click(-100,-100,Col,Row);//pøeposání události
	//funkcionalita exBUTTONu v tabulce pohonu
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabráni probliku Hintu, toto sloužilo pro tlaèítko, ale nebylo plnì uèinné: int T=F->PmG->exBUTTON->Top;
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
		F->nahled_ulozit(true);//novì se ukládá stav tabulky -> dùvod uložit pøi zmìnì stavu
		F->nastav_focus();
		F->REFRESH(true);//musí být opravdu REFRESH() celého formu + mGridu
		F->PmG->exBUTTON->Hint=Hint;//navrácení pùvodního textu hintu
	}

	if(ID!=9999 && Col==4 && Row==0)//klik na glyphbutton v pohonové tabulce
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		if(E!=NULL && (E->eID==200 || E->eID==300))
		{
			if(F->OBJEKT_akt->stavPM!=1)//budu skrývat
			{
				F->OBJEKT_akt->stavPM=1;//uložení stavu minimized
			}
			else//budu zobrazovat
			{
				if(E->mGrid->exBUTTON->GlyphOptions->Kind==scgpbgkUpArrow)
				{
					F->OBJEKT_akt->stavPM=2;//navrácení do stavu maximazied
				}
				else F->OBJEKT_akt->stavPM=0;//defaultní stav
			}
			aktualizace_tab_elementu();//update PM provede skrytí èi zobrazení øádkù podle stavuPM
			F->nahled_ulozit(true);//novì se ukládá stav tabulky -> dùvod uložit pøi zmìnì stavu
			F->nastav_focus();
			F->REFRESH(true);
		}
		E=NULL;delete E;
	}

	if(ID!=9999&&Row==-2)
	//funkcionalita exBUTTONu v tabulkách elementù
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		AnsiString Hint=E->mGrid->exBUTTON->Hint;//F->PmG->exBUTTON->Hint="";//zabráni probliku Hintu, toto sloužilo pro tlaèítko, ale nebylo plnì uèinné: int T=F->PmG->exBUTTON->Top;
		switch(E->eID)
		{
			case 0://stop stanice
			{
      	if(E->mGrid->Rows[4].Visible)
	    	{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(4,false,false);//nepøekreslovat
					E->mGrid->VisibleRow(5,false,false);
					E->mGrid->VisibleRow(6,false,false);
	    	}
	    	else
				{
					double rotaceJ=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E);
					if(E->mGrid->Cells[2][6].Text==1 && (F->m.Rt90(rotaceJ)==E->geo.orientace || F->m.Rt90(rotaceJ)==F->m.Rt90(E->geo.orientace+180)) && F->d.v.PP.delka_podvozek<F->m.UDJ(rotaceJ))
					E->mGrid->Note.Text="Pøi 0° èi 180° rotaci jigu a poètu nastavených pozic 1 nejsou zobrazeny potenciální pozice";
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(4,true,false);//nepøekreslovat
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
				F->design_element(E,false);//nutné nové slouèení buòìk
			}break;
			case 5://KK otoè
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
			case 200://pøedávací místo
			case 300://výhybka
			{
				if(F->OBJEKT_akt->stavPM==2)
				{
					F->OBJEKT_akt->stavPM=0;//jdu z max do default stavu
				}
				else
				{
					F->OBJEKT_akt->stavPM=2;//jdu z default do max stavu
				}
				aktualizace_tab_elementu();//update PM provede skrytí èi zobrazení øádkù podle stavuPM
				F->nahled_ulozit(true);//novì se ukládá stav tabulky -> dùvod uložit pøi zmìnì stavu
			}break;
			case 400:case 401:case 402://teplomìry
			{
				bool akce;
				if(E->mGrid->exBUTTON->GlyphOptions->Kind==scgpbgkUpArrow)//skrývat
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					akce=false;
				}
				else//zobrazit
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					akce=true;
				}
				//nastavování øádkù, vynechání hlavièky a posledního øádku celkem
				for(unsigned int i=1;i<E->mGrid->RowCount-1;i++)
				{
					E->mGrid->VisibleRow(i,akce,false);
				}
			}break;
		}
		F->mGrid_on_mGrid();//mohlo dojít k pøekryvu, nebo odstranìní pøekryvu ... nutná kontrola
		F->nastav_focus();
		F->REFRESH(true);//musí být opravdu REFRESH() celého formu + mGridu
		E->mGrid->exBUTTON->Hint=Hint;//navrácení pùvodního textu hintu
		E=NULL;delete E;
	}
	if(F->OBJEKT_akt!=NULL && F->editace_textu)F->smaz_kurzor();
	//uvolnìní inputu
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(ID==9999&&Row==1)validace_true=true;//spuštìní validace až po kliku
	//po kliku do vykreslené tabulky lze obnovit událost OnChange
	if(ID==9999)vstoupeno_poh=true;
	else vstoupeno_elm=true;
	if(ID==9999)editace_pohonu=true;
	else editace_pohonu=false;
	//korelace
	odstranit_korelaci(false);//nestaèí volat na úrovni form1 onclick, musí se volat i pøi pøeklikávání mezi komponenty
	if(ID==9999 && Row>=1 && F->PmG->Rows[3].Visible)//pokud je kliknuto do tabulky pohonu, podle buòky vyznèí buòky, které budou zmìnou ovlivnìné
		korelace_tab_pohonu(Row);//spuštìno pouze v pøípadì KK tabulky
	if(ID==9999 && Row>=1)korelace_tab_pohonu_elementy();//pøi S&G režimu je stále nutné zobrazit korelaci v elementech
	if(ID!=9999 && F->OBJEKT_akt->pohon!=NULL)//korelace v tabulkách elementù
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
  	vykresli_vetev=0;//nulování pøed testem
  	if(F->pom_element==NULL && !F->OBJEKT_akt->uzamknout_nahled)F->pom_element=vrat_element_z_tabulky(ID);//pokud je pom_element je NULL naètení z tabulky

  	//kontrola, zda se jedná o výhybku nebo spojku
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

		//pokud došlo ke zmìnì, REFRESH
		if(vykresli_vetev!=puv)F->REFRESH();
	}
}
//---------------------------------------------------------------------------
//zpracování onchange události - INPUT, výpoèet a OUTPUT zpìt do ovlivnìné buòky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING&&ID!=9999&&vstoupeno_elm&&!editace_pohonu)
	{
		F->Timer_neaktivity->Enabled=false;//vypnutí timeru neaktivity, pokud dochází k OnChange rychle za sebou nestpustí timer refresh
		//deklarace ukazatelù
		Cvektory::TElement *E=posledni_E;//pokud edituji stejný element znova není potøeba si jej znova vracet
		Cvektory::TCesta *c=posledni_c;
		//pokud se jedná o jiný element, znova naplnit E
		if(posledni_E==NULL || posledni_E!=NULL && posledni_E->mGrid!=NULL && posledni_E->mGrid->ID!=ID || posledni_E!=NULL && posledni_E->mGrid==NULL)
		{
			E=vrat_element_z_tabulky(ID);//ne vždy je ID tabulky stejné jako n robota, proto nutné hledat ID=ID ne ID=n
			//nahrání aktuálních dat do ukazatele
			if(F->d.v.ZAKAZKA_akt!=NULL && F->d.v.ZAKAZKA_akt->n!=0)//pokud pracuji v nìjaké zakázce
	  	{
				c=F->d.v.vrat_segment_cesty(F->d.v.ZAKAZKA_akt,E);
				if(c!=NULL)E->data=c->data;//pøepsání aktuálních dat ze zakázky do elementu
			}
		}
		//F->posledni_editovany_element=E;//odstaveno, narušuje tvorbu geometrie
		if(F->OBJEKT_akt->pohon!=E->pohon)F->OBJEKT_akt->pohon=E->pohon;
		validovat_pohon=false;
    popisky_pouzivany_pohon=false;
		////výpoèty
		switch(E->eID)
		{
			case 0: //stop
			{
				if(Row==3)//zmìna max. WT stop
				{
					input_state=WT;//nastaveni stavu
					E->data.WTstop=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->data.pocet_voziku=F->m.WT2V(E->data.WTstop,F->d.v.PP.TT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][6].Text=F->m.round2double(E->data.pocet_voziku,3);//OUTPUT
					//pøepoèet RT
					F->d.v.reserve_time(E,c);
				}
				if(Row==6)//max. poèet vozíkù
				{
					input_state=P_VOZ;//nastaveni stavu
					E->data.pocet_voziku=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);//INPUT
					E->data.WTstop=F->m.V2WT(E->data.pocet_voziku,F->d.v.PP.TT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][3].Text=F->m.round2double(F->outPT(E->data.WTstop),3);//OUTPUT
					//pøepoèet RT
					F->d.v.reserve_time(E,c);
				}
				////temp odemknutí editace max WT
				if(Row==4)//Col=2
				{
					input_state=WT;
					E->WT1=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
          F->aktualizace_RT();
				}
				////konec temp
        //pokud nejsou nastaveny žádné vozíky, nastavit stav na 0, musí probìhnout aktualizace spárovaných ukazatelù
				if(E->mGrid->Cells[Col][Row].Text!="")
				{
					if(E->data.pocet_voziku==0){E->stav=0;E->sparovany=NULL;F->d.v.aktualizuj_sparovane_ukazatele();}
					else if(E->stav==0){E->stav=1;F->d.v.aktualizuj_sparovane_ukazatele();}
					if(E->sparovany!=NULL)E->mGrid->Cells[2][1].Text=E->sparovany->name;else E->mGrid->Cells[2][1].Text="N/A";
				}
				//dodìlat plnìní pamìti pøi editaci bunìk
			} break;
			case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinuální)
			{
				if(Row==1)//editace CT
				{
					input_state=CT; //nastaveni stavu
					double CT = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(CT!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/CT;//uložení do pamìti + výpoèet
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//nutné ošetøení podmínkou, v pøípadì editace textu, který je celý oznaèená událost OnChange spuštìna 2x
						//zmìna v ostatních elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG, musí být refresh, PmG refreš obsluhuje druhž case v OnChange
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
					double CT=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][1].Text = F->m.round2double(F->outPT(CT),3); //OUTPUT
          E->data.RT=F->m.KKRT(CT,E->data.PT1);
					E->mGrid->Cells[Col][3].Text = F->m.round2double(F->outPT(E->data.RT),3); //OUTPUT
				}
				if(Row==5)// eidtace LO_pozice
				{
					input_state=LO2; //nastaveni stavu
					E->data.LO_pozice=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//pouze uložení do dat
				}
				if(Row==6)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->data.PD=E->mGrid->getCombo(1,Row)->ItemIndex;//pouze uložení do dat
				}
			} break;
			case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanicí
			{
				if(Row==1)//editace PT
				{
					input_state=PT;//nastaveni stavu
					E->data.PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
				////temp odemknutí editace max WT
				if(Row==3)//Col=1
				{
					input_state=WT;
					E->WT1=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
					F->aktualizace_RT();
				}
				////konec temp
			} break;
			case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivní otoèí
			{
				if(Row==1)//editace CT1
				{
					input_state=CT;//nastaveni stavu
					double CT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)),CT2;
          //ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(CT1!=0)
					{
			  		//ovlivnìní aRD
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/CT1;
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
			  		//zmìna aRD vyvolá následující pøepoèty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][5].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][9].Text=F->m.round2double(F->outPT(CT2),3);
						//zmìna v ostatních elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
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
					double CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);//uložení do pamìti + výpoèet
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
					//aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[3][7].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[3][8].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//pøepoèítání zón otáèení následujících elementù
				}
				//Row = 5 není uživatelsky upravitelný
				if (Row==6)//editace délky otoèe
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PTotoc=E->OTOC_delka/F->OBJEKT_akt->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
          //aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[3][7].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[3][8].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
				if(Row==9)//editace CT2
				{
					input_state=CT;//nastaveni stavu
					double CT1,CT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
          //ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(CT2!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO2/CT2;//uložení do pamìti + výpoèet
						if(F->PmG!=NULL)F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//zmìna aRD vyvolá následující pøepoèty
						CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][1].Text=F->m.round2double(F->outPT(CT1),3);
						E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[3][5].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zmìna v ostatních elementech
						if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						if(F->PmG!=NULL)F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
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
					double CT2=F->m.CT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][9].Text = F->m.round2double(F->outPT(CT2),3);//OUTPUT
          //RT
          double CT1=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
					E->data.RT=F->m.KKRT(CT1,E->data.PT1,CT2,E->data.PT2);
					E->mGrid->Cells[3][12].Text=F->m.round2double(F->outPT(E->data.RT),3);
				}
				if(Row==13)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->data.PD=E->mGrid->getCombo(3,Row)->ItemIndex;//pouze uložení do dat
				}
			} break;
			case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí (resp. s otoèí a stop stanicí)
			{
				if(Row==1)//zmìna PT1
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
				if(Row==3)//zmìna PTo
				{
					input_state=PTotoc;//nastaveni stavu
					E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
				if(Row==4)//zmìna PT2
				{
					input_state=PT2;//nastaveni stavu
					E->data.PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
				if(Row==6)//zmìna WT
				{
					input_state=WT;//nastaveni stavu
					E->WT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
			} break;
			case 5://otoè pasivní
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
					//aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[2][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[2][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//pøepoèítání zón otáèení následujících elementù
				}
				if (Row==2)//zde se upravuje pouze délka
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->PTotoc=E->OTOC_delka/F->OBJEKT_akt->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
					//aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[2][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[2][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
			} break;
			case 6://otoè aktivní (resp. otoè se stop stanicí)
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
					//E->OTOC_delka=F->OBJEKT_akt->pohon->aRD*E->PTotoc;//uložení do pamìti + výpoèet
					//E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outDO(E->OTOC_delka),3);//OUTPUT
					F->d.v.reserve_time(E,c);
				}
				////temp odemknutí editace max WT
				if(Row==4)//Col=1
				{
					input_state=WT;
					E->WT1=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
					F->aktualizace_RT();
				}
				////konec temp
			} break;
			case 200://pøedávací místo
			case 300://výhybka
			{
				posledni_E=E;//pøedèasné uložení posledního editovaného elementu
				switch(Row)
				{
					//nastavování pohonu
					case 2:
					{
						input_state=COMBO;//nastaveni stavu
						if(E->eID==300)prirazeni_pohohonu_vetvi(E,Col);//pro výhybku
						else prirazeni_pohohonu_PM(E,Col);//pro PM
						break;
					}
					//zmìna aRD
					case 3:
					{
						input_state=aRD;
						//naètení aRD z editu
						double aRD=F->inaRD(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//urèení cílového pohonu
						Cvektory::TPohon *p=F->d.v.vrat_pohon(E->mGrid->getCombo(Col,2)->ItemIndex);
						//uložení do pohonu
						F->OBJEKT_akt->pohon=p;//uložení aktuálnì editovaného pohonu
						p->aRD=aRD;
						//výpoèet ovlivnìných dat
						if(E->mGrid->Cells[Col][7].Text!="-")//kontrola, zda je zobrazeno Rx
						{
							p->Rz=F->m.Rz(p->aRD);
							p->Rx=F->m.Rx(p->aRD,p->roztec);
						}
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT1=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o další pohon
						p=NULL;delete p;
						//akticave a deaktivace comba pro zmìnu typu kót
						if(aRD>0)F->scGPComboBox_prepinacKot->Enabled=true;
						else//pokud je zadána nulová rychlost kóty musí být pøepnuty do módu délky
						{
							F->scGPComboBox_prepinacKot->Enabled=false;
							if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
						}
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					//zmìna rozteèe palce
					case 6:
					{
						input_state=R;
						//naètení R z editu
						double R=F->inR(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//urèení cílového pohonu
						Cvektory::TPohon *p=F->d.v.vrat_pohon(E->mGrid->getCombo(Col,2)->ItemIndex);
						//uložení do pohonu
						F->OBJEKT_akt->pohon=p;
						p->roztec=R;
						//výpoèet ovlivnìných dat
						p->Rz=F->m.Rz(p->Rx,p->roztec);
						p->aRD=F->m.RD(p->Rz);
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT1=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o další pohon
						p=NULL;delete p;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					//zmìna násobku rozteèe
					case 7:
					{
						input_state=Rx;
						//naètení Rx z editu
						double Rx=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
						//urèení cílového pohonu
						Cvektory::TPohon *p=F->d.v.vrat_pohon(E->mGrid->getCombo(Col,2)->ItemIndex);
						//uložení do pohonu
						F->OBJEKT_akt->pohon=p;
						p->Rx=Rx;
						//výpoèet ovlivnìných dat
						p->Rz=F->m.Rz(p->Rx,p->roztec);
						p->aRD=F->m.RD(p->Rz);
						if(E->mGrid->Cells[Col][11].Text!="-")E->WT1=F->m.cekani_na_palec(0,p->roztec,p->aRD,3);//pouze pokud jde o další pohon
						p=NULL;delete p;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					////temp odemknutí editace max WT
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
		if(E!=NULL && E->mGrid!=NULL)E->mGrid->Refresh();//refresh aktuálnì upravované tabulky
		posledni_E=E;//uložení posledního editovaného elementu
		posledni_c=c;//uložení posledního editovaného segmentu cesty
		if(F->d.v.ZAKAZKA_akt!=NULL && F->d.v.ZAKAZKA_akt->n!=0 && c!=NULL)c->data=E->data;//navrácení pøepoèítaných dat do zakázky
		E=NULL;delete E;
		c=NULL;delete c;
		vstoupeno_elm=true;
		input_state=NOTHING;F->Timer_neaktivity->Enabled=true;//uvolnìní stavu + zapnuti timeru neaktivity, pokud dokonèí èasování spustí REFRESH
		F->nahled_ulozit(true);//probìhla zmìna tabulky -> je dùvod uložit náhled
	}
	if(input_state==NOTHING&&ID==9999&&vstoupeno_poh)
	{
		F->Timer_neaktivity->Enabled=false;//vypnutí timeru neaktivity, pokud dochází k OnChange rychle za sebou nestpustí timer refresh
		posledni_E=NULL;
		posledni_c=NULL;
		switch(Row)
		{
			case 0://výbìr pohonu
			{
				input_state=COMBO;//nastaveni stavu
				prirazeni_pohonu_defTab();
			}break;
			case 1://aktuální rychlost, aRD
			{
				input_state=aRD;
				//naètení aRD z editu
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text));
				//uložení do spojáku
				F->OBJEKT_akt->pohon->aRD=aRD;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_aRD();
				//akticave a deaktivace comba pro zmìnu typu kót
				if(aRD>0)F->scGPComboBox_prepinacKot->Enabled=true;
				else//pokud je zadána nulová rychlost kóty musí být pøepnuty do módu délky
				{
					F->scGPComboBox_prepinacKot->Enabled=false;
					if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
				}
			}break;
			case 4://rozteè, R
			{
				input_state=R;
				//naètení R z editu
				double R=F->inR(F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text));
				//uložení do spojáku
				F->OBJEKT_akt->pohon->roztec=R;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_R();
			}break;
			case 5://Rx
			{
				input_state=Rx;
				//naètení Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text);
				//uložení do spojáku
				F->OBJEKT_akt->pohon->Rx=Rx;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_Rx();
			}break;
			default:break;
		}
		if(F->PmG!=NULL)F->PmG->Refresh();//v prùbìhu mùže dojít k jejímu odstranìní
		input_state=NOTHING;F->Timer_neaktivity->Enabled=true;
		F->nahled_ulozit(true);
	}
}
//---------------------------------------------------------------------------
void TFormX::OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key)
{
//	if(Row==0&&Col==0&&ID!=9999)//nutné ošetøení aby sem nepadaly vìci z øešené v OnChange
//	{
//		if((Key==VK_ESCAPE||Key==VK_RETURN)&&F->pom_element_temp!=NULL)//stisknutí ESC nebo ENTER
//		{    //pøi esc zrušit oznaèení textu
//			if(Key==VK_ESCAPE)
//			{
//				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text;//pokud je stisknuto ESC, vrátí pùvodní název
//				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=F->pom_element_temp->mGrid->Cells[0][0].Text.Length();//zamezí selectování celého textu pøi stisku ESC
//			}
//			F->index_kurzoru=9999;//musí být pøítomno, zabraòuje smazání editu hned po vytvoøení, timer volán i z OnChange
//			F->Timer2->Enabled=true;//smaže edit a uloží název (pùvodní nebo zmìnìný),edit musí být smazán se spoždením, jinak pamìová chyba
//		}
//		else
//		{
//			//øešeno takto z dùvodu, že v okamziku stisknutí klávesy je v editu text bez pøiètené klávesy, muselo by být implementováno do OnChange, aby fungovalo normálnì
//			if(Key==VK_BACK)//stisknuto backspace
//   			F->pom_element_temp->name=F->pom_element_temp->name.SubString(1,F->pom_element_temp->name.Length()-1);
//			else//ostatní klávesy
//				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text+AnsiString(Key);
//			//pokud se jendá o stopku, je možné editovat pouze koneèné èíslo
//			if(F->pom_element_temp->eID==0&&F->pom_element_temp->name.Length()<=4)
//			{
//				Key=0;//nutné! OnKeyPress je volána 2x
//				F->pom_element_temp->mGrid->Cells[0][0].Text="Stop ";//nahrazení toho co je v editu
//				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=5;//nastavení kurzoru na konec editu
//				MessageBeep(0);//oznámení uživateli
//			}
//			F->REFRESH(true);
//		}
//	}
	if((Row==5 && ID==9999) || (ID!=9999 && Col>=3 && Row==7))//slouží pro nepovolení zapsání desetiné èárky do editu Rx
	{
		if(AnsiString(Key)==F->ms.get_locale_decimal())
		{
			Key=0;
			MessageBeep(0);
		}
	}
}
//---------------------------------------------------------------------------
//slouží pro spuštìní funkcionality ctrl+z a ctrl+y pokud je focus na mGridu
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
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rychlosti
void TFormX::zmena_aRD(Cvektory::TElement *mimo_element)
{
	//pøepoèet parametrù pohonu
	F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
	F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
	if(F->PmG!=NULL)
	{
		//propoèty v tabulce pohonu
		aktualizace_tab_pohonu();
		//validace probìhne až po uplynutí timeru
		//if(F->Akce==F->NIC)validace_aRD();//validace pouze v kontinuálním režimu kabiny
		//F->PmG->Refresh();, není tøeba provede OnChange
	}
	//propoèty v tabulkách elementù
	aktualizace_tab_elementu(mimo_element);
  validovat_pohon=true;
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rozteèe
void TFormX::zmena_R ()
{
  //pøepoèet parametrù pohonu
	F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
	F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz); //prohozené poøadí z dùvodu, že druhý výpoèet potøebuje aktualizovaonu honotu prvního výpoètu
	if(F->PmG!=NULL)
	{
		//pøepoèet hodnot v tabulce pohonu
		aktualizace_tab_pohonu();
		//pøepoèet hodnot v elementech
		aktualizace_tab_elementu();
	}
	//////test validace rozteèe                                                                                                                            //0 = základní jednotky
//	TTextNumber cislo=F->d.v.rVALIDACE(5,F->OBJEKT_akt->pohon->n,F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->Rz,F->OBJEKT_akt->pohon->Rx,0,0,0);//F->aRDunit,F->Runit,F->Rzunit);
//	F->PmG->ShowNote(cislo.text);
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou Rx
void TFormX::zmena_Rx ()
{
	//pøepoèet parametrù pohonu
	F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
	F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz);
	if(F->PmG!=NULL)
	{
		//pøepoèet hodnot v tabulce pohonu
		aktualizace_tab_pohonu();
		//pøepoèet hodnot v elementech
		aktualizace_tab_elementu();
	}
}
//---------------------------------------------------------------------------
//pøepoèet v tabulkách elementù po zmìnì parametrù v tabulce pohonu
void TFormX::aktualizace_tab_elementu (Cvektory::TElement *mimo_element)
{
	unsigned int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		if(E->eID==200 || E->eID==300)zobrazit_skryt_radkyPM(E);
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->OBJEKT_akt->pohon!=NULL && (E->pohon->n==F->OBJEKT_akt->pohon->n || (E->eID==200 || E->eID==300)))//pøeskoèí mimo_element a hlavièku, poze pøepoèet elementùm které mají stejný pohon jako aktuálnì editovaný pohon
		{
			switch(E->eID)
			{
				case 0:
				{
					//pøepoèty
					E->WT1=F->m.cekani_na_palec(0,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->aRD,3);
					E->mGrid->Cells[2][4].Text=F->m.round2double(F->outPT(E->WT1),3);
					F->d.v.reserve_time(E);
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
				{
					//pøepoèty
					double CT=F->m.CT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);//uložení do pamìti + výpoèet
					E->mGrid->Cells[1][1].Text = F->m.round2double(F->outPT(CT),3); //OUTPUT
					E->data.RT=F->m.KKRT(CT,E->data.PT1);//uložení do pamìti + výpoèet
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->data.RT),3); //OUTPUT
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
				{
					//pøepoèty
					E->WT1=F->m.cekani_na_palec(0,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT1),3);
					F->d.v.reserve_time(E);
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otoèí
				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//pøepoèty
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
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí
				{
					//pøepoèty
					E->WT1=F->m.cekani_na_palec(0,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT1),3);
					F->d.v.reserve_time(E);
				}break;
				case 5://otoè pasivní
				{
          //naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//pøepoèty
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[2][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://otoè aktivní
				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//pøepoèty
					E->WT1=F->m.cekani_na_palec(0,roztec,aRD,3);//dùležité pro výpoèet RT, nezobrazuje se
					E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					F->d.v.reserve_time(E);
				}break;
				case 200://pøedávací místo
				case 300://výhybla
				{
					update_hodnot_vyhybky_PM(E);//provede zobrazení hodnot pohonu a výpoèítá nové hodnoty mezer, provede aktualizaci WT
					//validace_RD(E);
				}break;
				case 301://spojka
				{
					F->zmena_editovanych_bunek(E);
				}break;
			}
			if(input_state==COMBO)F->set_enabled_mGrid(E);//pouze pøi zmìnì pohonu, jinak zbyteèné
			//update rozbalení zabalení tabulek
			E->mGrid->Refresh();
		}
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranìní prùchodového spojáku

	//pøedchozí PM
	if(F->predchozi_PM!=NULL && F->predchozi_PM!=mimo_element)
	{
		update_hodnot_vyhybky_PM(F->predchozi_PM);
		//update rozbalení zabalení tabulek
		zobrazit_skryt_radkyPM(F->predchozi_PM);
		F->predchozi_PM->mGrid->Refresh();
	}

	//ukazatelové záležitosti
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//Naplní hodnoty které závisí na pohonu 0
void TFormX::aktualizace_tab_elementu_pOdebran ()
{
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && (E->pohon==NULL || (E->eID==200 || E->eID==300)))//pøeskoèí hlavièku + pøenastaví pouze elementy s odstranìným pohonem
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[2][3].Text=0;
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
				{
					E->mGrid->Cells[1][1].Text=0;
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
				{
					//validace
					E->mGrid->Cells[1][2].Text=0;
					E->mGrid->Cells[1][3].Text=0;
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otoèí
				{
					E->mGrid->Cells[3][1].Text=0;
					E->mGrid->Cells[3][5].Text=0;
					E->mGrid->Cells[3][9].Text=0;
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí
				{
					//validace
					E->mGrid->Cells[1][5].Text=0;
					E->mGrid->Cells[1][6].Text=0;
				}break;
				case 5://otoè pasivní
				{
					E->mGrid->Cells[2][3].Text=0;
				}break;
				case 6://otoè aktivní
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
					update_hodnot_vyhybky_PM(E);//provede zobrazení hodnot pohonu a výpoèítá nové hodnoty mezer, provede aktualizaci WT
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}

	//pøedchozi PM
	if(F->predchozi_PM!=NULL)
	{
		update_hodnot_vyhybky_PM(F->predchozi_PM);
		F->predchozi_PM->mGrid->Refresh();
	}

	//ukazatelové záležitosti
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
//highlightovaní buòìk tabulky pohonu
void TFormX::korelace_tab_pohonu(int Row,bool vypsat_note_pouzivan)
{
	if(F->PmG!=NULL)
	{
  	switch(Row)
  	{
  		case 1: //zmena aRD
			{
				F->PmG->Cells[3][5].Highlight=true; //rx
				F->PmG->Cells[3][3].Highlight=true; //roztec jigù
				F->PmG->Cells[3][6].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[3][7].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[3][8].Highlight=true;
			}break;
			case 4: //zmena rozteèe R
			{
				F->PmG->Cells[3][1].Highlight=true; //aRD
				F->PmG->Cells[3][3].Highlight=true; //roztec jigù
				F->PmG->Cells[3][6].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[3][7].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[3][8].Highlight=true;
  		}break;
			case 5: //zmìna Rx
			{
				F->PmG->Cells[3][1].Highlight=true; //aRD
				F->PmG->Cells[3][3].Highlight=true; //roztec jigù
				F->PmG->Cells[3][6].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[3][7].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[3][8].Highlight=true;
  		}break;
		}
    //vypisování upozornìní u používaných pohonù
		if(vypsat_note_pouzivan && F->OBJEKT_akt->pohon!=NULL && F->PmG->Note.Text=="" && F->je_pohon_pouzivan(F->OBJEKT_akt->pohon->n))F->PmG->ShowNote(F->ls->Strings[493]);//“Tato zmìna ovlivní všechny prvky na tomto pohonu.”
		else F->PmG->Refresh();
	}
	korelace_tab_pohonu_elementy();
}
//---------------------------------------------------------------------------
//stará se o highlitování políèek, které souvisí s mìnìnou hodnotou (elementy)
void TFormX::korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element)
{
	unsigned int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();//vytvoøení prùchodového spojáku
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
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
				{
					E->mGrid->Cells[1][1].Highlight=true;
          E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
				{
					//validace
					E->mGrid->Cells[1][2].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otoèí
				{
					E->mGrid->Cells[3][1].Highlight=true;
					E->mGrid->Cells[3][5].Highlight=true;
					E->mGrid->Cells[3][9].Highlight=true;
          E->mGrid->Cells[3][12].Highlight=true;
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí
				{
					//validace
					E->mGrid->Cells[1][5].Highlight=true;
					E->mGrid->Cells[1][6].Highlight=true;
				}break;
				case 5://otoè pasivní
				{
					E->mGrid->Cells[2][3].Highlight=true;
				}break;
				case 6://otoè aktivní
				{
					E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
				}break;
			}
			E->mGrid->Refresh();
		}
		//PM zvlášt, neplatí na nì podmínka E->pohon == OBJEKT_akt->pohon, protože mají pohony 2
		if(E->n>0 && E->n!=n && mimo_element!=E && (E->eID==200 || E->eID==300))
		{
			//zjištìní, zda má element stejný pohon jako upravovaný pohon, pokud ano zjistí v jakém je sloupci
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
			//pokud byl zjištìn sloupec, pokraèuje
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
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranìní prùchodového spojáku

	//korelace v tabulce pøedchozího pm
	if(F->predchozi_PM!=NULL)
	{
		E=F->predchozi_PM;
		if(E->n!=n && mimo_element!=E)
		{
			//zjištìní, zda má element stejný pohon jako upravovaný pohon, pokud ano zjistí v jakém je sloupci
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
			//pokud byl zjištìn sloupec, pokraèuje
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
//korelace uvnitø elementù
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
		case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinuální)
		{
			if(Row==1){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1,false);}korelace_tab_pohonu_elementy();E->mGrid->Cells[1][3].Highlight=true;}//E->mGrid->Cells[1][Row+1].Highlight=true;
      if(Row==2)E->mGrid->Cells[1][3].Highlight=true;
			if(Row==4){E->mGrid->Cells[1][1].Highlight=true;E->mGrid->Cells[1][3].Highlight=true;}
      //vypisování upozornìní u používaných pohonù
			if(Row==1 && E->pohon!=NULL && E->mGrid->Note.Text=="" && F->je_pohon_pouzivan(E->pohon->n)){E->mGrid->ShowNote(F->ls->Strings[493]);byl_refreshovan=true;}//“Tato zmìna ovlivní všechny prvky na tomto pohonu.”
		} break;
		case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanicí
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivní otoèí
		{
			if(Row==1){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1,false);}korelace_tab_pohonu_elementy();E->mGrid->Cells[3][12].Highlight=true;}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[3][12].Highlight=true;
			if(Row==3){E->mGrid->Cells[3][1].Highlight=true;E->mGrid->Cells[3][12].Highlight=true;}
			if(Row==6){E->mGrid->Cells[3][5].Highlight=true;E->mGrid->Cells[3][7].Highlight=true;E->mGrid->Cells[3][8].Highlight=true;}
			if(Row==9){if(F->PmG!=NULL){F->PmG->Cells[3][rychlost].Highlight=true;korelace_tab_pohonu(1,false);}korelace_tab_pohonu_elementy();E->mGrid->Cells[3][1].Highlight=true;E->mGrid->Cells[3][5].Highlight=true;}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==10)E->mGrid->Cells[3][12].Highlight=true;
			if(Row==11){E->mGrid->Cells[3][9].Highlight=true;E->mGrid->Cells[3][12].Highlight=true;}
      //vypisování upozornìní u používaných pohonù
			if((Row==1 || Row==9) && E->pohon!=NULL && E->mGrid->Note.Text=="" && F->je_pohon_pouzivan(E->pohon->n)){E->mGrid->ShowNote(F->ls->Strings[493]);byl_refreshovan=true;}//“Tato zmìna ovlivní všechny prvky na tomto pohonu.”
		} break;
		case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí (resp. s otoèí a stop stanicí)
		{
			if(Row==1)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==3)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==4)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==6)E->mGrid->Cells[1][5].Highlight=true;
		} break;
		case 5://otoè pasivní
		{
			if (Row==2){E->mGrid->Cells[2][Row+1].Highlight=true;E->mGrid->Cells[2][Row+2].Highlight=true;E->mGrid->Cells[2][Row+3].Highlight=true;}
		} break;
		case 6://otoè aktivní (resp. otoè se stop stanicí)
		{
			if (Row==2){/*E->mGrid->Cells[1][Row-1].Highlight=true;*/E->mGrid->Cells[1][Row+1].Highlight=true;}
		} break;
		case 200://pøedávací místo
		case 300://výhybka
		{
			//pøiøazení akt. pohonu
			unsigned int pohon=0;
			TscGPComboBox *Combo=E->mGrid->getCombo(Col,2);
			if(Combo!=NULL)pohon=Combo->ItemIndex;
			if((F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->pohon->n!=pohon) || F->OBJEKT_akt->pohon==NULL)F->OBJEKT_akt->pohon=F->d.v.vrat_pohon(pohon);
			Combo=NULL;delete Combo;
			//nastavování highlightu
			if(Row==3)
			{
				E->mGrid->Cells[Col][7].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
				if(E->mGrid->Cells[Col][11].Text!="-")E->mGrid->Cells[Col][11].Highlight=true;
				korelace_tab_pohonu_elementy(E);//oznaèení v ostatních tabulkách
			}
			if(Row==6 || Row==7)
			{
				E->mGrid->Cells[Col][3].Highlight=true;
				E->mGrid->Cells[Col][5].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
        if(E->mGrid->Cells[Col][11].Text!="-")E->mGrid->Cells[Col][11].Highlight=true;
				korelace_tab_pohonu_elementy(E);//oznaèení v ostatních tabulkách
			}
      //vypisování upozornìní u používaných pohonù
			if(Row!=0 && Row<11 && E->pohon!=NULL && E->mGrid->Note.Text=="" && F->je_pohon_pouzivan(E->pohon->n)){E->mGrid->ShowNote(F->ls->Strings[493]);byl_refreshovan=true;}//“Tato zmìna ovlivní všechny prvky na tomto pohonu.”
			break;
    }
	}
	if(!byl_refreshovan)E->mGrid->Refresh();
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//odstraní highlight na všech tabulkách
void TFormX::odstranit_korelaci(bool predat_focus)
{
	//pøedávání focusu
	if(predat_focus)
		F->DrawGrid_knihovna->SetFocus();//po kliku mimo zùstával focus poøád na editu

	//pohonová tabulka
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

	//pøedchozí PM
	if(F->predchozi_PM!=NULL)
	{
		F->predchozi_PM->mGrid->unHighlightAll();
		if(F->predchozi_PM->mGrid->Note.Text==F->ls->Strings[493])F->predchozi_PM->mGrid->Note.Text="";
	}
}
//---------------------------------------------------------------------------
//validace rychlosti pøi její zmìnì
void TFormX::validace_aRD(bool pouze_rozmezi)
{
	//kontrola pøi KK stavu objektu, validace všeho
	if(F->PmG!=NULL && F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->rezim!=0)
	{
		//smazání pøedchozí validace z VID
		zapisVID(0,1);//pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
		validovany_pohon=0;//uložit pohon na kterém se to stalo

		AnsiString jednotky;
  	if(F->aRDunit==0)jednotky="[m/s]";
  	else jednotky="[m/min]";
		bool mimo_rozmezi=false;
		unsigned int pro_pohon=0;
		dopRD=0;
		podbarvi_edit(NULL,3,1);
		//kontrola zda je zadaná hodnota v rozmezí
		if(F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do))mimo_rozmezi=false;
		else mimo_rozmezi=true;
  	//zadaná rychlost je mimo rozsah
  	if(mimo_rozmezi && F->OBJEKT_akt->pohon->aRD > 0)
  	{
			F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpovídá rozmezí!"
			podbarvi_edit(NULL,3,1,false);
			pro_pohon=F->OBJEKT_akt->pohon->n;//uložení pro který pohon platí validace
		}
		if(!mimo_rozmezi && F->PmG->Note.Text!=""){F->PmG->ShowNote("",F->d.clError,14);/*povolit_zakazat_editaci(true);*/}
		// nutné ošetøení pro období zadávání/psaní
		if(F->OBJEKT_akt->pohon->aRD > 0 && !pouze_rozmezi)
		{
			//výpoèet doporuèené rychosti
			double dopRD1=0,dopRD2=0,aRD=F->OBJEKT_akt->pohon->aRD;
			unsigned int n=0;
			do
			{
				//navyšování nebo snižování testovací rychlosti, tak aby byla v rozsahu pohonu
				if(n!=0)
				{
					if(F->OBJEKT_akt->pohon->aRD>=F->OBJEKT_akt->pohon->rychlost_do || (F->OBJEKT_akt->pohon->aRD<=F->OBJEKT_akt->pohon->rychlost_do && F->OBJEKT_akt->pohon->rychlost_do-F->OBJEKT_akt->pohon->aRD>0 && F->OBJEKT_akt->pohon->rychlost_do-F->OBJEKT_akt->pohon->aRD<F->OBJEKT_akt->pohon->aRD-F->OBJEKT_akt->pohon->rychlost_od))aRD=0.99*aRD;//snižování hodnoty o 1%
					else aRD=1.01*aRD;//navýšení hodnoty o 1%
				}
				//výpoèet doporuèených rychlostí
				dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,aRD);
				dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,aRD);
				//zapsání menší hodnoty jako dopRD
				if(dopRD1>dopRD2)dopRD=dopRD1;//vypíše vetší hodnotu
				else dopRD=dopRD2;
				n++;
			}while(!F->m.between(dopRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do) && n<5);
			//je zvolen pohon, jeho aktuální rychlost se nerovná doporuèené
			if(F->OBJEKT_akt->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
			{
  			F->PmG->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporuèenou rychlost pohonu:"
				podbarvi_edit(NULL,3,1,false);
				pro_pohon=F->OBJEKT_akt->pohon->n;//uložení pro který pohon platí validace
			}
  		//vše je vpoøádku
  		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
  		{
  			//povolit_zakazat_editaci(true);
  			F->PmG->ShowNote("",F->d.clError,14);
  		}
		}
		else if(!pouze_rozmezi)
		{
			F->PmG->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatná hodnota rychlosti pohonu!"
			podbarvi_edit(NULL,3,1,false);
			pro_pohon=F->OBJEKT_akt->pohon->n;//uložení pro který pohon platí validace
		}
		//pokud probìhla validace s problémem
		if(pro_pohon!=0)
		{
			validovany_pohon=pro_pohon;//uložit pohon na kterém se to stalo
			zapisVID(1,1);//zapsání problému do VID, pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
		}
		povolit_zakazat_editaci();//rozhodne se na základì VIDu
	}
	//kontrola pøi ostatních stavech objektu, pouze rozmezí rychlostí
	if(F->PmG!=NULL && F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->rezim==0)
	{
    //deklarace
		unsigned int pro_pohon=0;
		//defaultní stav
		zapisVID(0,1);//pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
		F->PmG->ShowNote("",F->d.clError,14);
		podbarvi_edit(NULL,3,1);
		validovany_pohon=0;//uložit pohon na kterém se to stalo
		//povolit_zakazat_editaci(true);
		//kontrola zda je zadaná hodnota v rozmezí
		if(!F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do))
		{
			F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpovídá rozmezí!"
      podbarvi_edit(NULL,3,1,false);
			pro_pohon=F->OBJEKT_akt->pohon->n;//uložení pro který pohon platí validace
		}
		//pokud probìhla validace s problémem
		if(pro_pohon!=0)
		{
			validovany_pohon=pro_pohon;//uložit pohon na kterém se to stalo
			zapisVID(1,1);//zapsání problému do VID, pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
		}
		povolit_zakazat_editaci();//rozhodne se na základì VIDu
	}
}
//---------------------------------------------------------------------------
//validace maximálního poètu vozíkù na stopce
void TFormX::validace_max_voziku()
{
	//if(posledni_E!=NULL && F->OBJEKT_akt!=NULL && posledni_E->objekt_n!=F->OBJEKT_akt->n)posledni_E=NULL;//pro pøípad, že se zmìnil náhled
	if(posledni_E!=NULL && posledni_E->objekt_n==F->OBJEKT_akt->n && posledni_E->eID==0)
	{
		////smazání pøedchozí validace z VID
		zapisVID(0,2);//pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
		////deklarace potøebných atributù
		bool validace=true;//pøedpoklad, že je vše OK
		////samotná validace
		posledni_E->data.pocet_pozic=F->max_voziku(posledni_E);
		AnsiString t1=F->ls->Strings[250],t2=F->ls->Strings[251],tip=F->ls->Strings[314]+" ";//"Max. poèet vozikù musí být menší nebo roven","Nelze, pøed Stopstanicí se nachází oblouk","Pro nastavovaný vozík je potøeba buffer o délce "
		if(posledni_E->data.pocet_pozic>0 && posledni_E->data.pocet_pozic<posledni_E->data.pocet_voziku){posledni_E->mGrid->ShowNote(t1+" <a>"+AnsiString(posledni_E->data.pocet_pozic)+"</a>");validace=false;}
		if(posledni_E->data.pocet_pozic==0){posledni_E->mGrid->ShowNote(t2);validace=false;}
		if(posledni_E->data.pocet_pozic>0 && posledni_E->data.pocet_pozic<posledni_E->data.pocet_voziku)F->TIP=tip+AnsiString(F->d.v.PP.delka_podvozek*posledni_E->data.pocet_voziku*1000)+" mm.";
		//validace pøekrytí jigù
		if(posledni_E->data.pocet_voziku>1)
		{
			double rotace=F->m.Rt90(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(posledni_E));
			if(F->d.v.PP.delka_podvozek<F->m.UDJ(rotace))//vozíky rovnobežnì s pohonem, dojde k pøekrytí
			{
				validace=false;
				posledni_E->mGrid->ShowNote(F->ls->Strings[426]+" <a>"+AnsiString(1)+"</a>");//"Kvùli pøekryvu jigù nelze nastavit vìtší poèet vozíkù než"
			}
		}
		////pøepsání maximálního poèctu vozíku do tabulky elementu, pro jistotu
		posledni_E->mGrid->Cells[2][5].Text=posledni_E->data.pocet_pozic;
		////nemožnost uložit pøi chybných hodnotách
		if(validace && F->duvod_ulozit_nahled && !F->scGPButton_ulozit->Enabled)F->nahled_ulozit(true);
		if(!validace)
		{
			zapisVID(1,2);//uložení chyby, pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
			povolit_zakazat_editaci();//rozhodne se na základì VIDu
		}
		if(validace && posledni_E->mGrid->Note.Text!="")posledni_E->mGrid->Note.Text="";
	}
}
//---------------------------------------------------------------------------
//voláno po kliku na link v poznámce, naplní edit aRD doporuèenou rychlostí
void TFormX::naplneni_dopRD()
{
	if(F->PmG!=NULL)
	{
		vstoupeno_poh=false;//musí dojít k blokaci onchange, jinak se do aRD nahraje zaokrouhlená hodnota
		unsigned int opraveny_pohon=validovany_pohon;
		vstoupeno_elm=false;
		F->PmG->Cells[3][rychlost].Text=F->m.round2double(F->outaRD(dopRD),3);
		F->OBJEKT_akt->pohon->aRD=dopRD;
		zmena_aRD();//postará se o aktualizaci všech tabulek
		validace_aRD();//znovuspuštìní valiace
		if(opraveny_pohon==validovany_pohon)validovany_pohon=0;//byla odstranìn problém
		//refresh + pøekreslení vozíkù + aktualizace erroru
		F->duvod_validovat=2;
		F->REFRESH(false);
	}
}
//---------------------------------------------------------------------------
bool TFormX::check_click_Note(double X,double Y,bool check_for_highlight)
{
	bool ret=false;
	//hledání zda má nìkterý element nedokonèenou validaci
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		//hledání elementu, kterému bylo kliknuto na doporuèený poèet vozíkù
		if(E->mGrid!=NULL && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(X,Y)==TPoint(-2,-2)){ret=true;break;}
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranìní prùchodového spojáku
	if(!ret && F->predchozi_PM!=NULL && F->predchozi_PM->mGrid!=NULL && F->predchozi_PM->mGrid->Note.Text!="" && F->predchozi_PM->mGrid->CheckLink(X,Y)==TPoint(-2,-2))
	{
		E=F->predchozi_PM;
		ret=true;
  }
	if(ret && !check_for_highlight)
	{
		switch(E->eID)
		{
			case 0://doplní doporuèený poèet maximálních vozíku po kliku
			{
      	//extrakce poètu z hintu
		  	AnsiString t=E->mGrid->Note.Text;
				int i=1,zacatek=1,pocet=-1;
		  	while(i<=t.Length())
		  	{
		  		if(pocet>=0)pocet++;
		  		if(t.SubString(i,1)==">"){zacatek=i+1;pocet=0;}
		  		if(pocet>0 && t.SubString(i,1)=="<")break;
		  		i++;
		  	}
		  	//naplnìní dat + tabulka
		  	E->data.pocet_voziku=F->ms.MyToDouble(t.SubString(zacatek,pocet-1));
		  	E->mGrid->Cells[2][6].Text=E->data.pocet_voziku;
        posledni_E=E;//dùležité pro metodu validace_max_voziku
				validace_max_voziku();
		  	F->Akce=F->BLOK;
			}break;
			case 200:
			case 300://naplnìní RD
			{
				vstoupeno_elm=false;vstoupeno_poh=false;//musí dojít k blokaci onchange, jinak se do aRD nahraje zaokrouhlená hodnota
				unsigned int opraveny_pohon=validovany_pohon;
				vstoupeno_elm=false;
				Cvektory::TPohon *p=F->d.v.vrat_pohon(validovany_pohon);
				p->aRD=dopRD;
				if(p!=F->OBJEKT_akt->pohon)F->OBJEKT_akt->pohon=p;
				zmena_aRD();//postará se o aktualizaci všech tabulek a znovu spuštìní validace
				validace_RD(E);
				if(opraveny_pohon==validovany_pohon)validovany_pohon=0;//byla odstranìn problém
				p=NULL;delete p;
			}break;
		}
		//refresh + pøekreslení vozíkù + aktualizace erroru
		F->duvod_validovat=2;
		F->REFRESH(false);
	}

	//uakazatelové záležitosti
	E=NULL;delete E;

	//vrácení pro highlight
	return ret;
}
//---------------------------------------------------------------------------
//zakazuje èi povolí ukládání editace na základì VID
void TFormX::povolit_zakazat_editaci()
{
	if(F->ms.MyToDouble(VID)==0)
		{F->scGPButton_ulozit->Enabled=true;}//pokud je dùvod k uložení, ale button uložit je z pøedchozího kroku neaktivní zapne ho
	else
		{F->scGPButton_ulozit->Enabled=false;}//pokud je button uložit zapnut vypne ho
}
//---------------------------------------------------------------------------
//vrátí ukazatel na element, nelze použít vra element stavající, z dùvodu že ID tabulky již nemusí být totožné s n robota
Cvektory::TElement *TFormX::vrat_element_z_tabulky(long ID)
{
	Cvektory::TElement *ret=NULL;
	Cvektory::TElement *E=F->OBJEKT_akt->element;//mùžu pøeskoèit element, metoda voláná po kliku do tabulky elementu
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
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranìní prùchodového spojáku
	if(ret==NULL && F->OBJEKT_akt->teplomery!=NULL)
	{
		Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
		if(T!=NULL && T->posledni->mGrid!=NULL && T->posledni->mGrid->ID==ID)ret=T->posledni;
	}
	E=NULL;delete E;
	return ret;
}
//---------------------------------------------------------------------------
//po zmìnì rotace na elementu E, projde všechny elementy za a pøepoèítá jim zóny otáèení
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
			if(E->eID%2!=0 && E->OTOC_delka>0)//aktualizace zón otáèení
			{
				zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
				E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
				if(E->objekt_n==F->OBJEKT_akt->n)
				{
					int col=3,row=6;if(E->eID==5){col=2;row=4;}//pøepínání od kterého øádku zaèít, rùzné elementy
					E->mGrid->Cells[col][row].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[col][row+1].Text=F->outDO(F->m.round2double(E->zona_za,3));//refresh mGridu není tøeba
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
//zmìní pohon sekundární vìtvi, z výhybky nebo spojky (pokud sekundární vìtev existuje)
void TFormX::prirazeni_pohohonu_vetvi(Cvektory::TElement *E,long Col)
{
	////deklarace potøebných promìnných
	TscGPComboBox *Combo=E->mGrid->getCombo(Col,2);
	Cvektory::TPohon *p=NULL;
	int p_n=0;
	if(Combo!=NULL && Combo->ItemIndex!=0)p=F->d.v.vrat_pohon(Combo->ItemIndex);
	if(p!=NULL)p_n=p->n;
	F->OBJEKT_akt->pohon=p;//uložení aktuálnì editovaného pohonu
	Cvektory::TElement *e=NULL;
	//zjištìní jakou vìtev budu editovat
	bool hlavni=true;
	int prvni=3,druhy=4;
	if(F->prohodit_sloupce_PM(E))
	{
		if(Col==3)Col=4;
		else Col=3;
	}
	if(Col!=3)hlavni=false;
  //aktualizace parametrù
	if(p!=NULL)
	{
		F->OBJEKT_akt->stavPM=2;//rozšíøit mGridy
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

  ////uložení aktuálnì editovaného pohonu
	F->OBJEKT_akt->pohon=p;
	////zmìna pohonu na hlavní vìtvi
	vstoupeno_elm=false;//zamezení, aby pøi aktualizace comba došlo k propadnutí do onchange
	if(hlavni)
	{
		E->pohon=p;
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(F->prohodit_sloupce_PM(e))prvni=4;else prvni=3;
			if(e->eID==200)//pokud narazím na PM zmìním mu pohon a skonèím prùchod
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
			if(e->eID==200 || F->predchozi_PM==NULL && e->objekt_n!=F->OBJEKT_akt->n || F->predchozi_PM!=NULL && F->predchozi_PM==e)break;//pokud narazím na PM NEzmìním! mu pohon a skonèím prùchod, nebo narazím na konec objektu
			e->pohon=p;
			e=e->predchozi;
		}
	}
	////zmìna pohonu na vedlejší vìtvi
	else
	{
		unsigned long o1=E->objekt_n;
		e=E->dalsi2;//pøiøazuji pohon z výhybky
		while(e!=NULL && e->identifikator_vyhybka_spojka!=E->identifikator_vyhybka_spojka)
		{
			e->pohon=p;
			if(e->objekt_n!=o1 && e->objekt_n!=o1)F->d.v.vrat_objekt(e->objekt_n)->pohon=p;//pokud je na sekundární vìtvi nìjaký objekt, pøiøadí mu pohon
			e=e->dalsi;
		}
		update_hodnot_vyhybky_PM(E);//nutno udìlat ruènì, metoda aktualizace_tab_elementu(), aktualizuje pouze elementy na stejném pohonu, co vyhybka není, má pohon hlavní vìtve
		//WT øeší fce. vložení PM
	}

	////naètení dat z pohonu do mGridu
  mazatPM(E);//kontrola a dotaz na mazání elementu + ovlivnìných PM
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT všem elementù, musí být první
	input_state=COMBO;//dùležité pro nastavení enabled komponent v aktualizaci elementù
	if(p!=NULL)aktualizace_tab_elementu();
	else aktualizace_tab_elementu_pOdebran();
	F->d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teplomìry aktualizuje ji, jinak vytvoøí default cestu

	//vytvoøení cesty teplomìrù po pøiøazení pohonu
	Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
	if(F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8 && T==NULL)
	{
		F->d.v.vytvor_default_c_teplomery(F->OBJEKT_akt);
		F->vytvor_aktualizuj_tab_teplomeru();
	}
	//pokud dojde k odstranìní pohonu, dojde ke smazání teplomìrù
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

	////ukazatelové záležitosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
//zobrazí aktuální hodnoty z dat v tabulkách
void TFormX::update_hodnot_vyhybky_PM(Cvektory::TElement *E)
{
	if(E->eID==200 || E->eID==300)
	{
		//kontrola zda budou prohozeny sloupce
		int prvni=3,druhy=4;
  	if(F->prohodit_sloupce_PM(E))
  	{
  		//prohhození sloupcù
  		prvni=4;druhy=3;
  	}

		//aktualizace hodnot
		vstoupeno_elm=false;
		if(E->pohon!=NULL)
		{
			if(input_state==NOTHING && ((E->objekt_n==F->OBJEKT_akt->n && E->pohon->rezim!=0) || (E->objekt_n!=F->OBJEKT_akt->n && E->pohon->rezim!=0)))//pøepoèet po pøidání KK elementu, pouze pøi input_state==NOTHING, tz. nepøepoèítávat pøi uživatelské zmìnì
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
  	if(E->eID==300)//pro výhybku
		{
    	if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)
			{
				if(input_state==NOTHING && E->dalsi2->pohon->rezim!=0)//pøepoèet po pøidání KK elementu
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
			if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;//v pøípadì PM na konci linky
			if(e_pom->pohon!=NULL)
			{
				if(input_state==NOTHING && e_pom->pohon->rezim!=0)//pøepoèet po pøidání KK elementu
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
				E->mGrid->Cells[druhy][11].Text=F->m.round2double(F->outPT(E->WT1),3);//pokud existuje druhý pohon, vždy bude WT + mohlo dojít ke zmìnì
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

		//update rozbalení zabalení tabulek
		//zobrazit_skryt_radkyPM(E);
	}
}
//---------------------------------------------------------------------------
//provede validaci RD
void TFormX::validace_RD(Cvektory::TElement *E)
{
	//smazání pøedchozí validace z VID
	zapisVID(0,1);//pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()

	//deklarace
	AnsiString jednotky;
	if(F->aRDunit==0)jednotky="[m/s]";
	else jednotky="[m/min]";
	AnsiString puv_Note="";
	if(E!=NULL)puv_Note=E->mGrid->Note.Text;
	bool mimo_rozmezi=false;
	//zjištìní n pohonù v tabulce
	unsigned int pro_pohon=0;
	dopRD=0;

	//validace pouze pohonu v elementu jiném než PM
	if(E!=NULL && F->OBJEKT_akt!=NULL && E->eID!=200 && E->eID!=300)
	{
		if(F->OBJEKT_akt->pohon!=E->pohon)F->OBJEKT_akt->pohon=E->pohon;
		E=F->OBJEKT_akt->element;
		Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();
		//hledání PM se stejným pohonem
		while(E!=NULL)
		{
			if(E->eID==200 && (E->pohon==F->OBJEKT_akt->pohon || (E->dalsi!=NULL && E->dalsi->pohon==F->OBJEKT_akt->pohon) || (E->dalsi==NULL && F->d.v.ELEMENTY->dalsi->pohon==F->OBJEKT_akt->pohon)))break;
			if(E->eID==300 && (E->pohon==F->OBJEKT_akt->pohon || (E->dalsi2!=NULL && E->dalsi2->pohon==F->OBJEKT_akt->pohon)))break;
			E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
		}
		F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
    //pokud nebylo nalezeno, hledám v pøedchozím PM
		if(E==NULL && F->predchozi_PM!=NULL && (F->predchozi_PM->pohon==F->OBJEKT_akt->pohon || (F->predchozi_PM->dalsi!=NULL && F->predchozi_PM->dalsi->pohon==F->OBJEKT_akt->pohon) || (F->predchozi_PM->dalsi==NULL && F->d.v.ELEMENTY->dalsi->pohon==F->OBJEKT_akt->pohon)))E=F->predchozi_PM;
	}
	//validovat pouze u PM
	if(E!=NULL && F->OBJEKT_akt!=NULL && (E->eID==200 || E->eID==300))// && F->OBJEKT_akt->pohon->aRD!=0)
	{
		//naètení pohonù
		Cvektory::TElement *e_dalsi=NULL;
		if(E->eID==300)e_dalsi=E->dalsi2;
		else if(E->dalsi!=NULL)e_dalsi=E->dalsi;
		else e_dalsi=F->d.v.ELEMENTY->dalsi;
		Cvektory::TPohon *p=NULL,*p1=E->pohon,*p2=e_dalsi->pohon;
		e_dalsi=NULL;delete e_dalsi;
		int Col=3;

		//dvojtá validace
		//pokud nalezne problém zastaví se a zobrazího, i v pøípadì, že je problémù více, až bude problém vyøešen probìhne validace zda neexistuje další problém
		for(unsigned int i=3;i<=4;i++)
		{
			//naètení požadovaného pohonu pro validaci
			if(i==3)p=p1;
			if(i==4)p=p2;
			Col=i;if(F->prohodit_sloupce_PM(E))
			{
				if(Col==3)Col=4;
				else Col=3;
      }
			//kontrola zda existuje pohon, pokud ne nemá smysl øešit
			if(p!=NULL && F->OBJEKT_akt->rezim!=0)
			{
				//kontrola zda je možné editovat pohon
				//if(E->mGrid->Cells[i][3].Type==E->mGrid->EDIT && p!=NULL)
				{
					podbarvi_edit(E,Col,3);//defaultní nastavní barev
					String Rx1=F->m.round2double(p->Rx,0),Rx2=p->Rx;
					//kontrola zda je zadaná hodnota v rozmezí
					if(F->m.between(p->aRD,p->rychlost_od,p->rychlost_do))mimo_rozmezi=false;
					else mimo_rozmezi=true;
					//zadaná rychlost je mimo rozsah
					if(mimo_rozmezi && p->aRD > 0)
					{
						E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpovídá rozmezí!"
						podbarvi_edit(E,Col,3,false);
						pro_pohon=p->n;//uložení pro který pohon platí validace
						break;//byl nalezen problém, zastavení validace, lze zobrazit jen jeden problém v Note
					}
					if(!mimo_rozmezi && E->mGrid->Note.Text!="")
					{
						E->mGrid->ShowNote("",F->d.clError,14);
						//povolit_zakazat_editaci(true);
					}
					// nutné ošetøení pro období zadávání/psaní
					if (p->aRD > 0)
					{
						//výpoèet doporuèené rychosti
						double dopRD1=0,dopRD2=0,aRD=p->aRD;
						unsigned int n=0;
						do
						{
		  				//navyšování nebo snižování testovací rychlosti, tak aby byla v rozsahu pohonu
							if(n!=0)
							{
								if(p->aRD>=p->rychlost_do || (p->aRD<=p->rychlost_do && p->rychlost_do-p->aRD>0 && p->rychlost_do-p->aRD<p->aRD-p->rychlost_od))aRD=0.99*aRD;//snižování hodnoty o 1%
								else aRD=1.01*aRD;//navýšení hodnoty o 1%
							}
							//výpoèet doporuèených rychlostí
							dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,p->roztec,F->d.v.PP.TT,aRD);
							dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,p->roztec,F->d.v.PP.TT,aRD);
							//zapsání menší hodnoty jako dopRD
							if(dopRD1>dopRD2)dopRD=dopRD1;//vypíše vetší hodnotu
							else dopRD=dopRD2;
							n++;
						}while(!F->m.between(dopRD,p->rychlost_od,p->rychlost_do) && n<5);
						//je zvolen pohon, jeho aktuální rychlost se nerovná doporuèené
						if(p->roztec>0 && dopRD!=p->aRD && Rx1!=Rx2 && mimo_rozmezi==false)
						{
							//if(E->mGrid->Note.Text=="")povolit_zakazat_editaci(false);//ošetøeno podmínkou proti opìtovnému spouštìní
							E->mGrid->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporuèenou rychlost pohonu:"
							pro_pohon=p->n;//uložení pro který pohon platí validace
//							F->Memo("p1: "+p1->name+"; p2: "+p2->name);
//							F->Memo("pro pohon: "+p->name+"; i: "+String(i)+"; Col: "+String(Col));
							podbarvi_edit(E,Col,3,false);//èervené podbarvení
							break;//byl nalezen problém, zastavení validace, lze zobrazit jen jeden problém v Note
						}
						//vše je vpoøádku
						if ((dopRD==p->aRD || Rx1==Rx2) && mimo_rozmezi==false)
						{
							//povolit_zakazat_editaci(true);
							E->mGrid->ShowNote("",F->d.clError,14);
						}
					}
					else E->mGrid->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatná hodnota rychlosti pohonu!"
				}
			}
			//vaidace pro ostatní režimy objektu, nutné kontrolovat rozmezí rychlosti
			else
			{
				if(p!=NULL && !F->m.between(p->aRD,p->rychlost_od,p->rychlost_do))
				{
					E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpovídá rozmezí!"
					pro_pohon=p->n;//uložení pro který pohon platí validace
				}
				else
				{
					E->mGrid->ShowNote("",F->d.clError,14);
					//povolit_zakazat_editaci(true);
				}
			}
		}

		//pokud probìhla validace s problémem
		if(pro_pohon!=0)
		{
			validovany_pohon=pro_pohon;//uložit pohon na kterém se to stalo
			zapisVID(1,1);//zapsání problému do VID, pozice jsou popsány v .h u deklarace VID a vnì metody zapisVID()
		}
		povolit_zakazat_editaci();//rozhodne se na základì VIDu

		//roznesení validace na ostatní
		if(puv_Note!=E->mGrid->Note.Text)//došlo ke zmìnì note
		{
			puv_Note=E->mGrid->Note.Text;
			int prvni=3,druhy=4;
			Cvektory::TElement *e_pom=F->OBJEKT_akt->element;
			if(F->predchozi_PM!=NULL)e_pom=F->predchozi_PM;
			Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();//vytvoøení prùchodového spojáku
			while(e_pom!=NULL)
			{
				try
				{
					if(e_pom!=E && (e_pom->eID==200 || e_pom->eID==300))
					{
						//rozmnožení note po ostatních pm
						if(F->prohodit_sloupce_PM(e_pom)){prvni=4;druhy=3;}
						else {prvni=3;druhy=4;}
						//kontrola prvního sloupce
						if(e_pom->pohon!=NULL && e_pom->pohon->n==validovany_pohon)
						{
							e_pom->mGrid->ShowNote(puv_Note,F->d.clError,14);
							if(puv_Note!="")podbarvi_edit(e_pom,prvni,3,false);//èervené podbarvení
							else podbarvi_edit(e_pom,prvni,3);//default podbarvení
						}
            //kontrola druhého sloupce
						else
						{
							if(e_pom->eID==300)e_dalsi=e_pom->dalsi2;
							else if(e_pom->dalsi!=NULL)e_dalsi=e_pom->dalsi;
							else e_dalsi=F->d.v.ELEMENTY->dalsi;
							if(e_dalsi->pohon!=NULL && e_dalsi->pohon->n==validovany_pohon)
							{
                e_pom->mGrid->ShowNote(puv_Note,F->d.clError,14);
								if(puv_Note!="")podbarvi_edit(e_pom,druhy,3,false);//èervené podbarvení
								else podbarvi_edit(e_pom,druhy,3);//default podbarvení
              }
            }
          }
				}catch(...){;}
        //pøesun na další
				if(e_pom==F->predchozi_PM && F->predchozi_PM!=NULL)e_pom=F->OBJEKT_akt->element;
				else e_pom=F->d.v.dalsi_krok(VYHYBKY,e_pom,F->OBJEKT_akt);
			}
      //ukazatelové záležitosti
			F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranìní prùchodového spojáku
			e_pom=NULL;delete e_pom;
      e_dalsi=NULL;delete e_dalsi;
		}

		//ukazatelové záležitosti
		p=NULL;p1=NULL;p2=NULL;
		delete p;delete p1;delete p2;
	}
	validovat_pohon=false;
}
//---------------------------------------------------------------------------
//nastaví defautlní barvy editu a buòce, nebo podbarvé buòku
void TFormX::podbarvi_edit(Cvektory::TElement *E,long Col,long Row,bool def_nastaveni)
{
	//nastevení podkresové barvy
	TColor barva=F->m.clIntensive(clRed,210);
	TscGPEdit *Edit=NULL;
	//naètení editu
  if(E!=NULL)Edit=E->mGrid->getEdit(Col,Row);
	if(E==NULL)Edit=F->PmG->getEdit(Col,Row);
	//naètení default barvy
	if(def_nastaveni)
	{
		if(Edit==NULL || (Edit!=NULL && !Edit->Enabled))barva=(TColor)RGB(240,240,240);
		else barva=clWhite;
	}

  //kontrola existence elementu a jeho mrgridu
	if(E!=NULL && E->mGrid!=NULL)
	{
		//získání editu
		if(Edit==NULL)Edit=E->mGrid->getEdit(Col,Row);

		//nastavení barev editu
		if(Edit!=NULL)
  	{
			Edit->Options->FocusedColor=barva;
			Edit->Options->NormalColor=barva;
			Edit->Options->HotColor=barva;
		}

		//nastavení barev buòky, nutné!!
		E->mGrid->Cells[Col][Row].Background->Color=barva;
	}

	//kontrola existence PmG
	if(E==NULL && F->PmG!=NULL)
	{
    //získání editu
		if(Edit==NULL)Edit=F->PmG->getEdit(Col,Row);

		//nastavení barev editu
		if(Edit!=NULL)
  	{
			Edit->Options->FocusedColor=barva;
			Edit->Options->NormalColor=barva;
			Edit->Options->HotColor=barva;
		}

		//nastavení barev buòky, nutné!!
		F->PmG->Cells[Col][Row].Background->Color=barva;
	}

	Edit=NULL;delete Edit;
}
//---------------------------------------------------------------------------
//pøiøazení pohonu pøed PM, nebo za PM
void TFormX::prirazeni_pohohonu_PM(Cvektory::TElement *E,long Col)
{
	////deklarace + naètení pohonu, který bude pøiøazován
	TscGPComboBox *Combo;
	Combo=E->mGrid->getCombo(Col,2);
	Cvektory::TPohon *p=NULL;
	if(Combo->ItemIndex!=0)p=F->d.v.vrat_pohon(Combo->ItemIndex);
	int p_n=0;
	if(p!=NULL)p_n=p->n;
	Cvektory::TElement *e=NULL;

	////uložení aktuálnì editovaného pohonu
	F->OBJEKT_akt->pohon=p;
	//aktualizace parametrù
	if(p!=NULL)
	{
		F->OBJEKT_akt->stavPM=2;//rozšíøit mGridy
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

	////prohození sloupcù
	int prvni=3,druhy=4;
	if(F->prohodit_sloupce_PM(E))
	{
		if(Col==3)Col=4;
		else Col=3;
	}

	////pøiøazení pohonu pøed PM
	vstoupeno_elm=false;//zamezení, aby pøi aktualizace comba došlo k propadnutí do onchange
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
				if(Combo!=NULL)Combo->ItemIndex=p_n;//nutná dodateèná kontrola zda nìní NULL (mGrid mimo obraz)
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
		//došel jsem na zaèátek obìktu a mám v pøedchozím PM
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
	////pøiøazení pohonu za PM
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
			if(e->eID==200)//narazil jsem na PM, zapsat nový pohon a konec
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

	////naètení dat z pohonu do mGridu
	mazatPM(E);//kontrola a dotaz na mazání elementu + ovlivnìných PM
	F->d.v.aktualizuj_rezim_pohonu();//aktualizuje režimy všem pohonùm
	F->vlozit_predavaci_misto_aktualizuj_WT();//musí být první!!
	input_state=COMBO;//dùležité pro nastavení enabled komponent v aktualizaci elementù
	if(p!=NULL)aktualizace_tab_elementu();
	else aktualizace_tab_elementu_pOdebran();
	F->d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teplomìry aktualizuje ji, jinak vytvoøí default cestu

  //vytvoøení cesty teplomìrù po pøiøazení pohonu
	Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
	if(F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8 && T==NULL)
	{
		F->d.v.vytvor_default_c_teplomery(F->OBJEKT_akt);
		F->vytvor_aktualizuj_tab_teplomeru();
	}
	//pokud dojde k odstranìní pohonu, dojde ke smazání teplomìrù
	if((F->OBJEKT_akt->pohon==NULL ||  !(F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8)) && T!=NULL)
	{
		F->d.v.vymaz_teplomery(F->OBJEKT_akt,T);
		delete T;T=NULL;
	}
	T=NULL;delete T;

	////povolení / zákaz pøepnutí kót
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

  ////ukazatelové záležitosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;

	//vypsání upozornìní k používaným pohonùm
	popisky_pouzivany_pohon=true;
}
//---------------------------------------------------------------------------
//pøiøazení pohonu z PmG
void TFormX::prirazeni_pohonu_defTab()
{
	//naètení pøiøazovaného pohonu
	int p_n=F->PmG->getCombo(3,0)->ItemIndex;
	Cvektory::TPohon *p=NULL;
	if(p_n!=0)p=F->d.v.vrat_pohon(p_n);

	//zapsání pohonu do objektu, tz. editovaný pohon
	F->OBJEKT_akt->pohon=p;
	//aktualizace knihovny elementù
	F->DrawGrid_knihovna->Refresh();//elementy
	F->DrawGrid_otoce->Refresh();//otoèe
	F->DrawGrid_ostatni->Refresh();//stop + ion tyè
	F->DrawGrid_geometrie->Refresh();//geometrie + PM

	//aktualiace parametrù
	if(p!=NULL && p->rezim!=0)
	{
		F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
		F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
	}

	//prùchod skrze elementy v objektu a pøiøazení toto pohonu
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		E->pohon=p;
		E=E->dalsi;
	}
	input_state=COMBO;//dùležité pro nastavení enabled komponent v aktualizaci elementù
  F->d.v.aktualizuj_rezim_pohonu();//aktualizuje režimy všem pohonùm
	if(p==NULL)aktualizace_tab_elementu_pOdebran();//aktualizace tabulek elemntù
	else aktualizace_tab_elementu();//došlo ke zmìnám v tabulce pohonu, které ovlivní i ostatní elementy

	//kontrola PM
	F->OBJEKT_akt->stavPM=2;//max po zvolení pohonu
	F->vlozit_predavaci_misto_aktualizuj_WT();
	F->aktualizace_tab_pohon(false,true,true);//nebude provedena pokud dojde k odstranìní PmG
	F->d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teplomìry aktualizuje ji, jinak vytvoøí default cestu

  //vytvoøení cesty teplomìrù po pøiøazení pohonu
	Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
	if(F->OBJEKT_akt->pohon!=NULL && F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8 && T==NULL)
	{
		F->d.v.vytvor_default_c_teplomery(F->OBJEKT_akt);
		F->vytvor_aktualizuj_tab_teplomeru();
	}
	//pokud dojde k odstranìní pohonu, dojde ke smazání teplomìrù
	if((F->OBJEKT_akt->pohon==NULL ||  !(F->OBJEKT_akt->id>=6 && F->OBJEKT_akt->id<=8)) && T!=NULL)
	{
		F->d.v.vymaz_teplomery(F->OBJEKT_akt,T);
		delete T;T=NULL;
	}
	T=NULL;delete T;

	//pokud už neexistuje PmG, nalezení PM pro spuštìní validace
	if(F->PmG==NULL)
	{
		posledni_E=F->OBJEKT_akt->element;
		while(posledni_E!=NULL && posledni_E->objekt_n==F->OBJEKT_akt->n)
		{
			if(posledni_E->dalsi==NULL || (posledni_E->dalsi!=NULL && posledni_E->dalsi->objekt_n!=F->OBJEKT_akt->n))break;
			posledni_E=posledni_E->dalsi;
		}
	}

	//povolení / zákaz pøepnutí kót
	if(F->OBJEKT_akt->pohon!=NULL && !F->scGPComboBox_prepinacKot->Enabled)F->scGPComboBox_prepinacKot->Enabled=true;
	if(F->OBJEKT_akt->pohon==NULL && F->scGPComboBox_prepinacKot->Enabled)
	{
		F->scGPComboBox_prepinacKot->Enabled=false;
		if(F->scGPComboBox_prepinacKot->ItemIndex==1)F->scGPComboBox_prepinacKot->ItemIndex=0;
	}

	//ukazatelové záležitosti
	E=NULL;p=NULL;
	delete E;delete p;

  //vypsání upozornìní k používaným pohonùm
	popisky_pouzivany_pohon=true;
}
//---------------------------------------------------------------------------
//zapiše na danou pozici ve VID dané èíslo
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
//vynuluje VID, podle délky nastavené v konstruktoru napø. 2 èíslice
void TFormX::vynulujVID()
{
  //nulování èíslic podle poètu èíslic v øetìzci VID
	for(int i=1;i<=VID.Length();i++)
	{
    zapisVID(0,i);
  }
}
//---------------------------------------------------------------------------
//kontorla a dotaz zda mají být PM na stejném pohonu smazána
void TFormX::mazatPM(Cvektory::TElement *Element)
{
	//deklarace
	bool nalezen=false;
	bool mazat_element=false;
	Cvektory::TElement *E=NULL;

	//kontrola zda mazat Element
	if(Element!=NULL && ((Element->dalsi!=NULL && Element->pohon==Element->dalsi->pohon) || (Element->dalsi==NULL && Element->pohon==F->d.v.ELEMENTY->dalsi->pohon)))mazat_element=true;
	//hledání ovlivnìného PM v celém objektu
	Cvektory::T2Element *VYH=F->d.v.hlavicka_seznam_VYHYBKY();
	E=F->OBJEKT_akt->element;
	if(F->predchozi_PM!=NULL)E=F->predchozi_PM;
	while(E!=NULL)
	{
		//kontrola zda jsem narazil na PM a je možné ho smazat
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
		//pøesun na další
		if(F->predchozi_PM!=NULL && E==F->predchozi_PM)E=F->OBJEKT_akt->element;
		else E=F->d.v.dalsi_krok(VYH,E,F->OBJEKT_akt);
	}
	F->d.v.vymaz_seznam_VYHYBKY(VYH);

  //uložení posunu a zoom pøed posuny na elementy
	TPointD predchozi_posun=F->Posun;
	double predchozi_zoom=F->Zoom;

	//dotaz + smazání samotného PM kde byla vyvolaná zmìna
	aut_mazani_PM=true;
	Cvektory::TElement *smaz=NULL;//pom ukazatel pro mazání prázdné zarážky pøed obloukem
	String name="";
	if(nalezen && E!=NULL)name=E->name;
	//kontrola, zda je možné smazat Element
	if(Element!=NULL && ((Element->dalsi!=NULL && Element->pohon!=Element->dalsi->pohon) || (Element->dalsi==NULL && Element->pohon!=F->d.v.ELEMENTY->dalsi->pohon)))Element=NULL;
	if(Element==E && E!=NULL)E=NULL;
	if(mazat_element && Element!=NULL && Element->eID==200)//zmìna mùže být vyvolána i z výhybky
	{
		if(Element->dalsi!=NULL && Element->dalsi->geo.delka==0)smaz=Element->dalsi;
		F->posun_na_element(Element);
		F->REFRESH(false);//musí následovat pøesunu
		F->pom_element=Element;
		F->pom_element_temp=Element;
		//podstrèení souøadnic pro zobrazení MB
		F->akt_souradnice_kurzoru_PX.x=F->m.L2Px(Element->X);
		F->akt_souradnice_kurzoru_PX.y=F->m.L2Py(Element->Y);
		F->Smazat1Click(this);
		if(smaz!=NULL)F->d.v.smaz_element(smaz);
		delete smaz;smaz=NULL;
	}

	//kontrola zde ovlivnìné PM existuje + dotaz na mazání
	if(nalezen && mazat_element && E!=NULL)
	{
		if(E->dalsi!=NULL && E->dalsi->geo.delka==0)smaz=E->dalsi;
		F->posun_na_element(E);
		F->REFRESH(false);//musí následovat pøesunu
		E->name=name;//pokud došlo k odstranìní napøíklad pøedchozího PM, došlo již i k aktualizaci názvù, napø v praxi smažu první predchozí PM "PM1" a druhy dotaz na Element, který býval "PM2" bude: "Chcete smazat PM1"
		F->pom_element=E;
		F->pom_element_temp=E;
		//podstrèení souøadnic pro zobrazení MB
		F->akt_souradnice_kurzoru_PX.x=F->m.L2Px(E->X);
		F->akt_souradnice_kurzoru_PX.y=F->m.L2Py(E->Y);
		F->Smazat1Click(this);
		if(smaz!=NULL)F->d.v.smaz_element(smaz);
    delete smaz;smaz=NULL;
	}

  //navrácení stavu
	aut_mazani_PM=false;

	//uložení posunu a zoom pøed posuny na elementy
	F->Posun=predchozi_posun;
	F->Zoom=predchozi_zoom;

	//ukazatelové záležitosti
  delete smaz;smaz=NULL;
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//zobrazí èi skryje øádky PM tabulek podle stavu uloženého v objektu
void TFormX::zobrazit_skryt_radkyPM(Cvektory::TElement *E)
{
  //update rozbalení zabalení tabulek
	switch(F->OBJEKT_akt->stavPM)
	{
		//defaultní stav zobrazeny comba a rychlosti
		case 0:
		{
			if(!E->mGrid->Rows[3].Visible)
			{
				E->mGrid->VisibleRow(3,true,false);
				E->mGrid->VisibleRow(4,true,false);
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkUpArrow;
				//zmenšení tabulky, úspora místa
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
		//minimized, skrytí všeho kromì hlavièky a comb
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
				//zmenšení tabulky, úspora místa
				E->mGrid->Columns[0].Width-=20;
				E->mGrid->Columns[1].Width-=20;
				E->mGrid->Columns[2].Width-=40;
			}
			break;
		}
		//vìe zobrazeno
		case 2:
		{
			if(!E->mGrid->Rows[3].Visible)
			{
				E->mGrid->VisibleRow(3,true,false);
				E->mGrid->VisibleRow(4,true,false);
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->getGlyphButton(4,0)->GlyphOptions->Kind=scgpbgkUpArrow;
				//zmenšení tabulky, úspora místa
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
			F->napln_comba_mGridu(E);//nutné pro naplnìní comb WT
			break;
		}
	}
}
//---------------------------------------------------------------------------
//provede aktualizaci èasu v tabulce teplomìrù
void TFormX::aktualizace_teplomeru()
{
	if(F->OBJEKT_akt->teplomery!=NULL)
	{
		Cvektory::TTeplomery *T=F->d.v.vrat_teplomery_podle_zakazky(F->OBJEKT_akt,F->d.v.ZAKAZKA_akt);
		if(T!=NULL)
		{
      //mazání všech rádkù kromì hlavièky
			for(unsigned int i=T->posledni->mGrid->RowCount-1;i>0;i--)
			{
				T->posledni->mGrid->DeleteRow(i,false);
			}
      ////existuje ... aktualizace øádkù
			double cas=0,WT=0,delka=0;
			int pocet_voziku=0;
			bool prejezd=true;
			//výpoèet èasu na zaèátku
			if(T->prvni->sparovany->pohon!=NULL)
			{
				//výpoèet délky oblasti
				delka=F->m.delka(T->prvni->geo.X1,T->prvni->geo.Y1,T->prvni->sparovany->geo.X4,T->prvni->sparovany->geo.Y4);
				delka=F->m.castPrepony(delka,T->prvni->sparovany->geo.delkaPud,T->prvni->sparovany->geo.HeightDepp);
				if(T->prvni->sparovany==T->posledni->sparovany)delka=F->m.delka(T->prvni->geo.X1,T->prvni->geo.Y1,T->posledni->geo.X4,T->posledni->geo.Y4);//pokud jsou oba teplomìry na stejném úseku, mìøit délku mezi jejich souøadnicemi
				//pokud se jedná o stopku poèítat s bufferem
				if(T->prvni->sparovany->eID==0)
				{
					//výpoèet délky bufferu
					double buf=T->prvni->sparovany->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice;
					//pokud je úsek menší než délka bufferu pøipoètení wt podle toho na jakém jsem vozíku
					if(delka<=buf)
					{
						prejezd=false;
						pocet_voziku=ceil((buf-delka)/F->d.v.PP.delka_podvozek);
						if(pocet_voziku<0)pocet_voziku=T->prvni->sparovany->data.pocet_voziku+pocet_voziku;
						cas+=F->m.V2WT(pocet_voziku,F->d.v.PP.TT);//pøipoèítání WT na aktuálním vozíku
					}
					//pokud je úsek vìtší než buffer pøipoètení WTstop a pøejezdu
					else
					{
						delka=delka-buf;
						cas+=delka/T->prvni->sparovany->pohon->aRD;
						//rozpad na pøejezd a buffer
						prejezd=true;
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;
						prejezd=false;
						cas+=T->prvni->sparovany->data.WTstop;
					}
				}
				else cas+=delka/T->prvni->sparovany->pohon->aRD;
				WT+=T->prvni->sparovany->WT1;
			}

			//výpoèet èasu na cestì
			Cvektory::TCesta *CE=T->cesta->dalsi;
			while(CE!=NULL)
			{
				if(CE->Element->pohon!=NULL)
				{
					if(CE->Element->eID==0)
					{
						//výpoèet èasu
						if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl pøed tím buffer, zmìna, potøebuju zapsat pøejezd
						delka=CE->Element->geo.delka-(CE->Element->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice);
						if(delka<0)delka=0;//pokud je délka záporná, znamená to, že v bufferu je PM, nepøidávat žádný èas
						cas+=delka/CE->Element->pohon->aRD;
						prejezd=true;
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//zapsání èásti pøejezdu
						prejezd=false;
						cas+=CE->Element->data.WTstop;
					}
					else
					{
						if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl pøed tím buffer, zmìna, bude následovat pøejezd
						prejezd=true;
						cas+=CE->Element->geo.delka/CE->Element->pohon->aRD;
						//kontrola zda další není stopka a aktuální element není v bufferu
						if(CE->dalsi!=NULL && CE->dalsi->Element->eID==0 && CE->dalsi->Element->geo.delka<CE->dalsi->Element->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice)
						{
              //element je v bufferu odeèíst èást bufferu
							delka=CE->dalsi->Element->geo.delka-(CE->dalsi->Element->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice);
							cas+=delka/CE->Element->pohon->aRD;
            }
					}
					WT+=CE->Element->WT1;
				}
				CE=CE->dalsi;
			}
			//ukazatelové záležitosti
			CE=NULL;delete CE;

			//výpoèet èasu na konci
			if(T->posledni->sparovany->pohon!=NULL && T->posledni->sparovany!=T->prvni->sparovany)
			{
				//výpoèet délky oblasti
				delka=F->m.delka(T->posledni->sparovany->geo.X1,T->posledni->sparovany->geo.Y1,T->posledni->geo.X4,T->posledni->geo.Y4);
				delka=F->m.castPrepony(delka,T->posledni->sparovany->geo.delkaPud,T->posledni->sparovany->geo.HeightDepp);
				//pokud se jedná o stopku poèítat s bufferem
				if(T->posledni->sparovany->eID==0)
				{
					//výpoèet délky bufferu
					double buf=T->posledni->sparovany->data.pocet_voziku*F->d.v.PP.delka_podvozek-F->d.v.PP.uchyt_pozice;
					buf=T->posledni->sparovany->geo.delka-buf;
					//pokud je úsek menší než délka bufferu pøipoètení wt podle toho na jakém jsem vozíku
					if(delka>buf)
					{
						if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl pøejezd, zmìna, bude následovat buffer
						prejezd=true;
						cas+=buf/T->posledni->sparovany->pohon->aRD;
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//pokud byl pøejezd, zmìna, bude následovat buffer
						prejezd=false;
						WT+=T->posledni->sparovany->WT1;
						pocet_voziku=ceil((delka-buf)/F->d.v.PP.delka_podvozek);
						if(pocet_voziku<0)pocet_voziku=T->prvni->sparovany->data.pocet_voziku+pocet_voziku;
						cas+=F->m.V2WT(pocet_voziku,F->d.v.PP.TT);//pøipoèítání WT na aktuálním vozíku
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;
					}
					//pokud je úsek vìtší než buffer pøipoètení WTstop a pøejezdu
					else
					{
						cas+=delka/T->posledni->sparovany->pohon->aRD;
						prejezd=true;
						//rozpad na pøejezd a buffer
						F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//zapsání èásti pøejezdu
					}
				}
				else//není stop
				{
					if(!prejezd){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}
					prejezd=true;
					cas+=delka/T->posledni->sparovany->pohon->aRD;
					WT+=T->posledni->sparovany->WT1;
				}
			}
			//kontrola zda nejsou ještì k vypsání nìjaké hodnoty, mùže nastat, když budou oba teplomìry na jedné stoce
			if(cas>0 || WT>0){F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}
			//vložení øádku souètu
			F->pridej_radek_tab_teplomeru(T->posledni,cas,WT,false,true);
		}

		//ukazatelové záležitosti
		T=NULL;delete T;
	}
}
//---------------------------------------------------------------------------
//aktualizuje položky v pohonových tabulkách, v pøípadì, že došlo ke zmìnì režimu pohonu
void TFormX::zmena_rezimu_pohonu(Cvektory::TPohon *pohon)
{
	//deklarace
	bool probehla_validace=false;
	Cvektory::TElement *E=F->OBJEKT_akt->element;

	//aktualizace parametrù pohonu
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

	//prùchod skrze elementy v objektu
	Cvektory::T2Element *VYHYBKY=F->d.v.hlavicka_seznam_VYHYBKY();//vytvoøení prùchodového spojáku
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->eID==200 || E->eID==300)
		{
			update_hodnot_vyhybky_PM(E);//provede aktualizaci dat a editovaných položek v mGridu
			if(!probehla_validace && E->eID==200)//spouštìt validaci jen jednou a to pokud narazím na PM
			{
				validace_RD(E);//pokud byl zmìnìn režím provede validaci aktuální rychlosti
				probehla_validace=true;//zapsaní, že validace probìhla
			}
		}
		E=F->d.v.dalsi_krok(VYHYBKY,E,F->OBJEKT_akt);
	}

  //aktualizace tabulek mimo objekt, tj. pøedchozí PM a PmG
	if(F->predchozi_PM!=NULL)update_hodnot_vyhybky_PM(F->predchozi_PM);//provede aktualizaci dat a editovaných položek v mGridu u pøedchozího PM
	F->aktualizace_tab_pohon(false,true,true);//obsahuje podmínku.. pokud existuje PmG

	//ukazatelové záležitosti
	F->d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranìní prùchodového spojáku
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//z èísla VIDu vrátí jeho textový popis
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
