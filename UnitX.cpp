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
 //pokud dojde ke zmìnì poøadí øádku, nastavit nové pozice zde, + pøepsání switche pro tabulku pohonu v OnChange + pøepsaní switche v korelace_tab_pohonu()
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
// pøi kliku do nìjaké buòky nastavím input_state=NOTHING, pokud udìlám zmìnu buòky je v OnChange události switch, který zajistí
// výpoèet konkrétní buòky dle pøedávaných parametrù v události
	input_state=NO;
	if(ID==6666)F->tab_knihovna_click(-100,-100,Col,Row);//pøeposání události
	//funkcionalita exBUTTONu v tabulce pohonu
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabráni probliku Hintu, toto sloužilo pro tlaèítko, ale nebylo plnì uèinné: int T=F->PmG->exBUTTON->Top;
		if(F->PmG->Rows[mezera_podvozky].Visible)
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			F->PmG->VisibleRow(mezera_podvozky,false,false);

		}else
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			F->PmG->VisibleRow(mezera_podvozky,true,false);
		}
		F->PmG->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt
		F->PmG->Update();
		F->PmG->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt, toto nestaèilo: F->PmG->exBUTTON->Top=T;//zajistí, že se tlaèítko nepøepozicuje
		F->REFRESH(true);//musí být opravdu REFRESH() celého formu + mGridu
		F->PmG->exBUTTON->Hint=Hint;//navrácení pùvodního textu hintu
	}

	if(ID!=9999&&Row!=-2)
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		if(E->eID==200 && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(F->akt_souradnice_kurzoru_PX.x,F->akt_souradnice_kurzoru_PX.y)==TPoint(-2,-2))
		{
			F->d.v.vrat_pohon(validovany_pohon)->aRD=dopRD; dopRD=0;
			zmena_aRD(E);
//			vstoupeno_elm=false;
			validace_RD(E);
//			F->Memo("Click into Note");
//			E->mGrid->Note.Text="";
		}
		E=NULL;delete E;
	}

	if(ID!=9999&&Row==-2)
	//funkcionalita exBUTTONu v tabulkách elementù
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		AnsiString Hint=E->mGrid->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabráni probliku Hintu, toto sloužilo pro tlaèítko, ale nebylo plnì uèinné: int T=F->PmG->exBUTTON->Top;
		switch(E->eID)
		{
			case 0://stop stanice
			{
      	if(E->mGrid->Rows[3].Visible)
	    	{
	    		E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
	    		E->mGrid->VisibleRow(3,false,false);//nepøekreslovat
					E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);
	    	}
	    	else
	    	{
					double rotaceJ=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E);
					if(E->mGrid->Cells[1][6].Text==1 && (F->m.Rt90(rotaceJ)==0 || F->m.Rt90(rotaceJ)==180) && F->d.v.PP.delka_podvozek<F->m.UDJ(rotaceJ))
					E->mGrid->Note.Text="Pøi 0° èi 180° rotaci jigu a poètu nastavených pozic 1 nejsou zobrazeny potenciální pozice";
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
	    		E->mGrid->VisibleRow(3,true,false);//nepøekreslovat
	    		E->mGrid->VisibleRow(5,true,false); E->mGrid->VisibleRow(6,true,false);
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
		E->mGrid->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt
		E->mGrid->Update();
		E->mGrid->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt, toto nestaèilo: F->PmG->exBUTTON->Top=T;//zajistí, že se tlaèítko nepøepozicuje
		F->REFRESH(true);//musí být opravdu REFRESH celého formu + mGridu
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
		F->posledni_editovany_element=E;
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
				//dodìlat plnìní pamìti pøi editaci bunìk
			} break;
			case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinuální)
			{
				if(Row==1)// editace PT
				{
					input_state=PT; //nastaveni stavu
					E->data.PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(E->data.PT1!=0)
					{
						F->OBJEKT_akt->pohon->aRD=E->data.LO1/E->data.PT1;//uložení do pamìti + výpoèet
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//nutné ošetøení podmínkou, v pøípadì editace textu, který je celý oznaèená událost OnChange spuštìna 2x
			  		zmena_aRD(E);
						F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
					}
				}
				if(Row==2)// eidtace LO
				{
					input_state=LO; //nastaveni stavu
					E->data.LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->data.PT1=E->data.LO1/F->OBJEKT_akt->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->data.PT1),3); //OUTPUT
				}
				if(Row==3)// eidtace LO_pozice
				{
					input_state=LO2; //nastaveni stavu
					E->data.LO_pozice=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//pouze uložení do dat
				}
				if(Row==4)// eidtace COMBO PD
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
			} break;
			case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivní otoèí
			{
				if (Row==1)//editace PT1
				{
					input_state=PT;//nastaveni stavu
					E->data.PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
          //ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(E->data.PT1!=0)
					{
			  		//ovlivnìní aRD
			  		F->OBJEKT_akt->pohon->aRD=E->data.LO1/E->data.PT1;
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
			  		//zmìna aRD vyvolá následující pøepoèty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
			  		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
			  		E->data.PT2=F->m.PT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[1][8].Text=F->m.round2double(F->outPT(E->data.PT2),3);
			  		//zmìna v ostatních elementech
			  		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
					}
				}
				if (Row==2)//editace LO1
				{
					input_state=LO;//nastaveni stavu
					E->data.LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->data.PT1=E->data.LO1/F->OBJEKT_akt->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->data.PT1),3);//OUTPUT
				}
				if (Row==3)//editace COMBO
				{
					input_state=COMBO;//nastaveni stavu
					switch(E->mGrid->getCombo(1,3)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
					//aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[1][6].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][7].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//pøepoèítání zón otáèení následujících elementù
					F->pridani_elementu_tab_pohon(E);//pouze v KK režimu
				}
				//Row = 4 není uživatelsky upravitelný
				if (Row==5)//editace délky otoèe
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PTotoc=E->OTOC_delka/F->OBJEKT_akt->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
          //aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[1][6].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][7].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
				if (Row==8)//editace PT2
				{
					input_state=PT;//nastaveni stavu
					E->data.PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					//ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(E->data.PT2!=0)
					{
			   		F->OBJEKT_akt->pohon->aRD=E->data.LO2/E->data.PT2;//uložení do pamìti + výpoèet
			   		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
			   		//zmìna aRD vyvolá následující pøepoèty
			   		E->data.PT1=F->m.PT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
			   		E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
			   		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
			   		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zmìna v ostatních elementech
			   		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
					}
				}
				if (Row==9)//editace LO2
				{
					input_state=LO2;//nastaveni stavu
					E->data.LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->data.PT2=E->data.LO2/F->OBJEKT_akt->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->data.PT2),3);//OUTPUT
				}
				if(Row==10)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->data.PD=E->mGrid->getCombo(1,Row)->ItemIndex;//pouze uložení do dat
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
					E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					F->d.v.reserve_time(E,c);
				}
			} break;
			case 5://otoè pasivní
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
					//aktualizace zón otáèení
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[1][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//pøepoèítání zón otáèení následujících elementù
					if(F->OBJEKT_akt->rezim==1)F->pridani_elementu_tab_pohon(E);//pouze v KK režimu, pasivní otoè mùže být i ve S&G lakovnì
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
					E->mGrid->Cells[1][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
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
						if(E->eID==300)//pro výhybku
						{
					  	if(Col==1)prirazeni_pohohonu_vetvi(E);//hlavní vìtev
					  	else prirazeni_pohohonu_vetvi(E,false);//vedlejší vìtev
						}
						else prirazeni_pohohonu_PM(E,Col);
						break;
					}
					//zmìna aRD
					case 3:
					{
						input_state=aRD;
						//naètení aRD z editu
						double aRD=F->inaRD(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//urèení cílového pohonu
						Cvektory::TElement *e_pom=E;
						if(Col==2 && E->eID==300)e_pom=E->dalsi2;
						if(Col==2 && E->eID==200)
						{
							e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
						}
						//uložení do pohonu
						F->OBJEKT_akt->pohon=e_pom->pohon;//uložení aktuálnì editovaného pohonu
						e_pom->pohon->aRD=aRD;
						//výpoèet ovlivnìných dat
						e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->aRD);
						e_pom->pohon->Rx=F->m.Rx(e_pom->pohon->aRD,e_pom->pohon->roztec);
						if(Col==2)E->WT=F->m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);//pouze pokud jde o další pohon
						e_pom=NULL;delete e_pom;
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
					case 5:
					{
            input_state=R;
						//naètení R z editu
						double R=F->inR(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//urèení cílového pohonu
						Cvektory::TElement *e_pom=E;
						if(Col==2 && E->eID==300)e_pom=E->dalsi2;
						if(Col==2 && E->eID==200)
						{
							e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
						}
						//uložení do pohonu
						F->OBJEKT_akt->pohon=e_pom->pohon;
						e_pom->pohon->roztec=R;
						//výpoèet ovlivnìných dat
						e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->Rx,e_pom->pohon->roztec);
						e_pom->pohon->aRD=F->m.RD(e_pom->pohon->Rz);
						if(Col==2)E->WT=F->m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);//pouze pokud jde o další pohon
						e_pom=NULL;delete e_pom;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					//zmìna násobku rozteèe
					case 6:
					{
            input_state=Rx;
			    	//naètení Rx z editu
						double Rx=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
			    	//urèení cílového pohonu
						Cvektory::TElement *e_pom=E;
						if(Col==2 && E->eID==300)e_pom=E->dalsi2;
						if(Col==2 && E->eID==200)
						{
							e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
						}
						//uložení do pohonu
						F->OBJEKT_akt->pohon=e_pom->pohon;
						e_pom->pohon->Rx=Rx;
						//výpoèet ovlivnìných dat
						e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->Rx,e_pom->pohon->roztec);
						e_pom->pohon->aRD=F->m.RD(e_pom->pohon->Rz);
						if(Col==2)E->WT=F->m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);//pouze pokud jde o další pohon
						e_pom=NULL;delete e_pom;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
				}
			} break;
		}
		E->mGrid->Refresh();//refresh aktuálnì upravované tabulky
		posledni_E=E;//uložení posledního editovaného elementu
		posledni_c=c;//uložení posledního editovaného segmentu cesty
		if(F->d.v.ZAKAZKA_akt!=NULL && F->d.v.ZAKAZKA_akt->n!=0 && c!=NULL)c->data=E->data;//navrácení pøepoèítaných dat do zakázky
		E=NULL;delete E;
		c=NULL;delete c;
		input_state=NOTHING;F->Timer_neaktivity->Enabled=true;//uvolnìní stavu + zapnuti timeru neaktivity, pokud dokonèí èasování spustí REFRESH
		F->nahled_ulozit(true);//probìhla zmìna tabulky -> je dùvod uložit náhled
	}
	if(input_state==NOTHING&&ID==9999&&vstoupeno_poh)
	{
		F->Timer_neaktivity->Enabled=false;//vypnutí timeru neaktivity, pokud dochází k OnChange rychle za sebou nestpustí timer refresh
		switch(Row)
		{
			case 0://výbìr pohonu
			{
				input_state=COMBO;//nastaveni stavu
				F->tab_pohon_COMBO(1);//pøiøazení pohonu
				F->aktualizace_ComboPohon();//aktualizace popiskù pohonu v combo, po pøiøazení zmìna na 0tém øádku z "vyber pohon" na "odeber pohon"
				if(F->PmG->getCombo(0,0)->ItemIndex!=0)//pokud je vybrán nìjaký pohon
				{
					F->scGPComboBox_prepinacKot->Enabled=true;//zapnutí zmìny typu kót, rychlost v tomto kroku bude vždy vìtší než 0
					F->scGPGlyphButton_PLAY->Enabled=true;//zapnutí tlaèítka animace
					//aktualizace tabulek
					aktualizace_PmG();
					aktualizace_tab_elementu();//došlo ke zmìnám v tabulce pohonu, které ovlivní i ostatní elementy
				}
				else//pohon byl odebrán
				{
					F->scGPComboBox_prepinacKot->Enabled=false;//vypne zmìnu režimu kót
					F->scGPComboBox_prepinacKot->ItemIndex=0;
					F->scGPGlyphButton_PLAY->Enabled=false;//vypnutí tlaèítka animace
					aktualizace_tab_elementu_pOdebran();//aktualizace tabulek elemntù
				}
			}break;
			case 1://aktuální rychlost, aRD
			{
				input_state=aRD;
				//naètení aRD z editu
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
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
			case 2://rozteè, R
			{
				input_state=R;
				//naètení R z editu
				double R=F->inR(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
				//uložení do spojáku
				F->OBJEKT_akt->pohon->roztec=R;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_R();
			}break;
			case 3://Rx
			{
				input_state=Rx;
				//naètení Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[1][Row].Text);
				//uložení do spojáku
				F->OBJEKT_akt->pohon->Rx=Rx;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_Rx();
			}break;
			break;
		}
		F->PmG->Refresh();input_state=NOTHING;F->Timer_neaktivity->Enabled=true;
		F->nahled_ulozit(true);
	}
}
//---------------------------------------------------------------------------
void TFormX::OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key)
{
	if(Row==0&&Col==0&&ID!=9999)//nutné ošetøení aby sem nepadaly vìci z øešené v OnChange
	{
		if((Key==VK_ESCAPE||Key==VK_RETURN)&&F->pom_element_temp!=NULL)//stisknutí ESC nebo ENTER
		{    //pøi esc zrušit oznaèení textu
			if(Key==VK_ESCAPE)
			{
				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text;//pokud je stisknuto ESC, vrátí pùvodní název
				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=F->pom_element_temp->mGrid->Cells[0][0].Text.Length();//zamezí selectování celého textu pøi stisku ESC
			}
			F->index_kurzoru=9999;//musí být pøítomno, zabraòuje smazání editu hned po vytvoøení, timer volán i z OnChange
			F->Timer2->Enabled=true;//smaže edit a uloží název (pùvodní nebo zmìnìný),edit musí být smazán se spoždením, jinak pamìová chyba
   	}
		else
   	{
			//øešeno takto z dùvodu, že v okamziku stisknutí klávesy je v editu text bez pøiètené klávesy, muselo by být implementováno do OnChange, aby fungovalo normálnì
			if(Key==VK_BACK)//stisknuto backspace
   			F->pom_element_temp->name=F->pom_element_temp->name.SubString(1,F->pom_element_temp->name.Length()-1);
   		else//ostatní klávesy
				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text+AnsiString(Key);
			//pokud se jendá o stopku, je možné editovat pouze koneèné èíslo
   		if(F->pom_element_temp->eID==0&&F->pom_element_temp->name.Length()<=4)
			{
				Key=0;//nutné! OnKeyPress je volána 2x
				F->pom_element_temp->mGrid->Cells[0][0].Text="Stop ";//nahrazení toho co je v editu
				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=5;//nastavení kurzoru na konec editu
				MessageBeep(0);//oznámení uživateli
			}
			F->REFRESH(true);
   	}
	}
	if(Row==3&&ID==9999)//slouží pro nepovolení zapsání desetiné èárky do editu Rx
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
void TFormX::zmena_aRD (Cvektory::TElement *mimo_element)
{
	F->aktualizace_ComboPohon();
	//propoèty v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK režimu
	{
		F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->OBJEKT_akt->pohon->Rx,3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		if(F->Akce==F->NIC)validace_aRD();//validace pouze v kontinuálním režimu kabiny
	}
	else if(F->Akce==F->NIC)validace_aRD(true);//kontrola pouze zda se rychlost nachází v rozmezí
	//propoèty v tabulkách elementù
	aktualizace_tab_elementu(mimo_element);
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rozteèe
void TFormX::zmena_R ()
{
	//pøepoèet hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK režimu
	{
    F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz); //prohozené poøadí z dùvodu, že druhý výpoèet potøebuje aktualizovaonu honotu prvního výpoètu
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
	}
	//pøepoèet hodnot v elementech
	F->aktualizace_ComboPohon();//zmìna rychlosti, rychlost je zobrazená v Combo pohonù
	aktualizace_tab_elementu();
	//////test validace rozteèe                                                                                                                            //0 = základní jednotky
//	TTextNumber cislo=F->d.v.rVALIDACE(5,F->OBJEKT_akt->pohon->n,F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->Rz,F->OBJEKT_akt->pohon->Rx,0,0,0);//F->aRDunit,F->Runit,F->Rzunit);
//	F->PmG->ShowNote(cislo.text);
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou Rx
void TFormX::zmena_Rx ()
{
	//pøepoèet hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//kontinuální režim
	{
    F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz);
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
	}
	//pøepoèet hodnot v elementech
  F->aktualizace_ComboPohon();//zmìna rychlosti, rychlost je zobrazená v Combo pohonù
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//aktualizuje hodnoty v PmG
void TFormX::aktualizace_PmG(bool Refresh)
{
  if(F->PmG->Rows[3].Visible)//pro tabulku v kontinuálním režimu
	{
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][roztec_palce].Text=F->m.round2double(F->outR(F->OBJEKT_akt->pohon->roztec),3);
		F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->OBJEKT_akt->pohon->Rx,0);
		F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		//místo pro mezeru mezi podvozky
		if(F->PmG->Rows[mezera_jig2].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][mezera_jig1].Text=F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1));
			F->PmG->Cells[1][mezera_jig2].Text=F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1));
		}
		else if(F->PmG->Rows[mezera_jig1].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][mezera_jig1].Text=F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1));
		}
	}
	else
	{
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][roztec_palce].Text=F->m.round2double(F->outR(F->OBJEKT_akt->pohon->roztec),3);
	}
	if(Refresh)F->PmG->Refresh();
}
//---------------------------------------------------------------------------
//pøepoèet v tabulkách elementù po zmìnì parametrù v tabulce pohonu
void TFormX::aktualizace_tab_elementu (Cvektory::TElement *mimo_element)
{
	unsigned int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->OBJEKT_akt->pohon!=NULL && E->pohon->n==F->OBJEKT_akt->pohon->n)//pøeskoèí mimo_element a hlavièku, poze pøepoèet elementùm které mají stejný pohon jako aktuálnì editovaný pohon
		{
			switch(E->eID)
			{
				case 0:
				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->WT),3);
					F->d.v.reserve_time(E);
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
				{
          //naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//pøepoèty
					E->data.PT1=F->m.PT(E->data.LO1,aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT),3);
					F->d.v.reserve_time(E);
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otoèí
				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//pøepoèty
					E->data.PT1=F->m.PT(E->data.LO1,aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->data.PT2=F->m.PT(E->data.LO2,aRD);
					E->mGrid->Cells[1][8].Text=F->m.round2double(F->outPT(E->data.PT2),3);
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí
 				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT),3);
					F->d.v.reserve_time(E);
				}break;
 				case 5://otoè pasivní
				{
          //naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD;
					//pøepoèty
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://otoè aktivní
				{
					//naètení hodnot z pohonu
					double aRD=F->OBJEKT_akt->pohon->aRD,roztec=F->OBJEKT_akt->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);//dùležité pro výpoèet RT, nezobrazuje se
					E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					F->d.v.reserve_time(E);
				}break;
				case 200://pøedávací místo
				case 300://výhybla
				{
					update_hodnot_vyhybky_PM(E);//provede zobrazení hodnot pohonu a výpoèítá nové hodnoty mezer, provede aktualizaci WT
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	if(F->predchozi_PM!=NULL && F->predchozi_PM!=mimo_element)
	{
		update_hodnot_vyhybky_PM(F->predchozi_PM);
		F->predchozi_PM->mGrid->Refresh();
	}

	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//Naplní hodnoty které závisí na pohonu 0
void TFormX::aktualizace_tab_elementu_pOdebran ()
{
	Cvektory::TElement *E=F->OBJEKT_akt->element;
	while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
	{
		if(E->n>0 && E->pohon==NULL)//pøeskoèí hlavièku + pøenastaví pouze elementy s odstranìným pohonem
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[1][3].Text=0;
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
					E->mGrid->Cells[1][1].Text=0;
					E->mGrid->Cells[1][4].Text=0;
					E->mGrid->Cells[1][8].Text=0;
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
					E->mGrid->Cells[1][3].Text=0;
				}break;
				case 6://otoè aktivní
				{
					E->mGrid->Cells[1][3].Text=0;
				}break;
				case 301://spojka
				{
					E->mGrid->Cells[1][2].Text=0;
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//highlightovaní buòìk tabulky pohonu
void TFormX::korelace_tab_pohonu(int Row)
{
	switch(Row)
	{
		case 1: //zmena aRD
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
			{
				F->PmG->Cells[1][roztec_jigu].Highlight=true; //rozestup
				F->PmG->Cells[1][nasobek_roztece].Highlight=true; //rx
				F->PmG->Cells[1][mezera_podvozky].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[1][mezera_jig1].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][mezera_jig2].Highlight=true;
			}
		}break;
		case 2: //zmena rozteèe R
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
			{
				F->PmG->Cells[1][roztec_jigu].Highlight=true;  //rozestup
				F->PmG->Cells[1][rychlost].Highlight=true; //aRD
				F->PmG->Cells[1][mezera_podvozky].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[1][mezera_jig1].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][mezera_jig2].Highlight=true;
			}
		}break;
		case 3: //zmìna Rx
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
			{
				F->PmG->Cells[1][rychlost].Highlight=true; //aRD
				F->PmG->Cells[1][roztec_jigu].Highlight=true; //rozestup Rz
				F->PmG->Cells[1][mezera_podvozky].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[1][mezera_jig1].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][mezera_jig2].Highlight=true;
			}
		}break;
	}
	F->PmG->Refresh();
	korelace_tab_pohonu_elementy();
}
//---------------------------------------------------------------------------
//stará se o highlitování políèek, které souvisí s mìnìnou hodnotou (elementy)
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
					E->mGrid->Cells[1][2].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
				{
					E->mGrid->Cells[1][1].Highlight=true;
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
					E->mGrid->Cells[1][1].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
					E->mGrid->Cells[1][8].Highlight=true;
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
					E->mGrid->Cells[1][3].Highlight=true;
				}break;
				case 6://otoè aktivní
				{
					//E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}break;
				case 200:
				case 300:
				{
					if(mimo_element!=NULL)
					{
						//zjištìní, zda má element stejný pohon jako upravovaný pohon, pokud ano zjistí v jakém je sloupci
						long Col=0;
						if(E->pohon==F->OBJEKT_akt->pohon)Col=1;
						if(E->eID==300 && E->predchozi2!=E->dalsi2 && E->dalsi2->pohon==F->OBJEKT_akt->pohon)Col=2;
						if(E->eID==200)
						{
							Cvektory::TElement *e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
							if(e_pom->pohon==F->OBJEKT_akt->pohon)Col=2;
							e_pom=NULL;delete e_pom;
						}
						//pokud byl zjištìn sloupec, pokraèuje
						if(Col!=0)
						{
							E->mGrid->Cells[Col][3].Highlight=true;
							E->mGrid->Cells[Col][6].Highlight=true;
							E->mGrid->Cells[Col][7].Highlight=true;
							E->mGrid->Cells[Col][8].Highlight=true;
							E->mGrid->Cells[Col][9].Highlight=true;
							E->mGrid->Cells[Col][10].Highlight=true;
            }
          }
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//korelace uvnitø elementù
void TFormX::korelace_v_elementech(long ID,long Col,long Row)
{
	Cvektory::TElement *E=vrat_element_z_tabulky(ID);
	switch(E->eID)
	{
		case 0://stopka
		{
			if(Row==3)E->mGrid->Cells[1][6].Highlight=true;
			if(Row==6)E->mGrid->Cells[1][3].Highlight=true;
		}
		case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinuální)
		{
			if(Row==1){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			F->PmG->Refresh();//voláno samostatnì, jen ve dvou case
		} break;
		case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanicí
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivní otoèí
		{
			if (Row==1){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==5){E->mGrid->Cells[1][Row-1].Highlight=true;E->mGrid->Cells[1][Row+1].Highlight=true;E->mGrid->Cells[1][Row+2].Highlight=true;}
			if (Row==8){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==9)E->mGrid->Cells[1][Row-1].Highlight=true;
			F->PmG->Refresh();
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
			if (Row==2){E->mGrid->Cells[1][Row+1].Highlight=true;E->mGrid->Cells[1][Row+2].Highlight=true;E->mGrid->Cells[1][Row+3].Highlight=true;}
		} break;
		case 6://otoè aktivní (resp. otoè se stop stanicí)
		{
			if (Row==2){/*E->mGrid->Cells[1][Row-1].Highlight=true;*/E->mGrid->Cells[1][Row+1].Highlight=true;}
		} break;
		case 200://pøedávací místo
		case 300://výhybka
		{
			if(Row==3)
			{
				E->mGrid->Cells[Col][6].Highlight=true;
				E->mGrid->Cells[Col][7].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
			}
			if(Row==5 || Row==6)
			{
				E->mGrid->Cells[Col][3].Highlight=true;
				E->mGrid->Cells[Col][7].Highlight=true;
				E->mGrid->Cells[Col][8].Highlight=true;
				E->mGrid->Cells[Col][9].Highlight=true;
				E->mGrid->Cells[Col][10].Highlight=true;
			}
			korelace_tab_pohonu_elementy(E);//oznaèení v ostatních tabulkách
			break;
    }
	}
	E->mGrid->Refresh();
	E=NULL; delete E;
}
//odstraní highlight na všech tabulkách
void TFormX::odstranit_korelaci(bool predat_focus)
{
	if(predat_focus)
		F->DrawGrid_knihovna->SetFocus();//po kliku mimo zùstával focus poøád na editu
	F->PmG->unHighlightAll();
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
//validace rychlosti pøi její zmìnì
void TFormX::validace_aRD(bool pouze_rozmezi)
{
	AnsiString jednotky;
	if(F->aRDunit==0)jednotky="[m/s]";
	else jednotky="[m/min]";
	bool mimo_rozmezi=false;
	TscGPComboBox *Combo=F->PmG->getCombo(0,0);
	dopRD=0;
	//kontrola zda je zadaná hodnota v rozmezí
	if(F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do)) mimo_rozmezi=false;
	else mimo_rozmezi=true;
	//zadaná rychlost je mimo rozsah
	if(mimo_rozmezi && F->OBJEKT_akt->pohon->aRD > 0)
	{
		if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//ošetøeno podmínkou proti opìtovnému spouštìní
		F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpovídá rozmezí!"
		povolit_zakazat_editaci(false);
	}
	if(!mimo_rozmezi && F->PmG->Note.Text!=""){F->PmG->ShowNote("",F->d.clError,14);povolit_zakazat_editaci(true);}
	// nutné ošetøení pro období zadávání/psaní
	if (F->OBJEKT_akt->pohon->aRD > 0 && !pouze_rozmezi)
	{
		//výpoèet doporuèené rychosti
		if(F->PmG->Rows[7].Visible)//v kabinì jsou 2 rùzné rotace
		{
			double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
			double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
			if(dopRD1>dopRD2)dopRD=dopRD1;//vypíše vìtší hodnotu
			else dopRD=dopRD2;
		}
		else
			dopRD=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,F->OBJEKT_akt->rotace,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
		//je zvolen pohon, jeho aktuální rychlost se nerovná doporuèené
		if(Combo->ItemIndex!=0 && F->OBJEKT_akt->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
		{
			if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//ošetøeno podmínkou proti opìtovnému spouštìní
			F->PmG->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporuèenou rychlost pohonu:"
		}
		//vše je vpoøádku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
		{
			povolit_zakazat_editaci(true);
			F->PmG->ShowNote("",F->d.clError,14);
		}
	}
	else if(!pouze_rozmezi)F->PmG->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatná hodnota rychlosti pohonu!"
	Combo=NULL;delete Combo;
}
//---------------------------------------------------------------------------
//validace maximálního poètu vozíkù na stopce
void TFormX::validace_max_voziku()
{
	if(posledni_E!=NULL && F->OBJEKT_akt!=NULL && posledni_E->objekt_n!=F->OBJEKT_akt->n)posledni_E=NULL;//pro pøípad, že se zmìnil náhled
	if(posledni_E!=NULL && posledni_E->eID==0)
	{
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
			double rotace=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(posledni_E);
			if(rotace==0 || rotace==180)
			{
				validace=false;
				posledni_E->mGrid->ShowNote(F->ls->Strings[426]+" <a>"+AnsiString(1)+"</a>");//"Kvùli pøekryvu jigù nelze nastavit vìtší poèet vozíkù než"
			}
		}
		////pøepsání maximálního poèctu vozíku do tabulky elementu, pro jistotu
		posledni_E->mGrid->Cells[1][5].Text=posledni_E->data.pocet_pozic;
		////nemožnost uložit pøi chybných hodnotách
		if(validace && F->duvod_ulozit_nahled && !F->scGPButton_ulozit->Enabled)F->nahled_ulozit(true);
		if(!validace)F->nahled_ulozit(false);
		if(validace && posledni_E->mGrid->Note.Text!="")posledni_E->mGrid->Note.Text="";
	}
}
//---------------------------------------------------------------------------
//voláno po kliku na link v poznámce, naplní edit aRD doporuèenou rychlostí
void TFormX::naplneni_dopRD()
{
	F->OBJEKT_akt->pohon->aRD=dopRD;
	F->PmG->Cells[1][rychlost].Text=F->outaRD(dopRD);
	zmena_aRD();
	//odstranit_korelaci();//pro jistotu zùstavala aktivní po kliku na link
	F->PmG->ShowNote("",F->d.clError,14);
	povolit_zakazat_editaci(true);
	F->Akce=F->BLOK;
	F->PmG->Refresh();//došlo ke zmìnì hodnot v tabulce
}
//---------------------------------------------------------------------------
//doplní doporuèený poèet maximálních vozíku po kliku
bool TFormX::naplneni_max_voziku(double X,double Y,bool check_for_highlight)
{
	bool ret=false;
	if(F->d.v.vrat_posledni_element_objektu(F->OBJEKT_akt)->n>1)
	{
		//hledání zda má nìkterý element nedokonèenou validaci
		Cvektory::TElement *E=F->OBJEKT_akt->element;
		while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
		{
			//hledání elementu, kterému bylo kliknuto na doporuèený poèet vozíkù
			if(E->eID==0 && E->mGrid!=NULL && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(X,Y)==TPoint(-2,-2)){ret=true;break;}
			E=E->dalsi;
		}
		//naplnìní doporuèeného max. poètu vozíkù
		if(ret && E!=NULL && !check_for_highlight)
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
			E->mGrid->Cells[1][6].Text=E->data.pocet_voziku;
			E->mGrid->ShowNote("");
			F->Akce=F->BLOK;
		}
		E=NULL;delete E;
	}
	return ret;
}
//---------------------------------------------------------------------------
//zakazuje èi povolí komponenty v tabulce pohonu a všech tabulkách elementu
void TFormX::povolit_zakazat_editaci(bool povolit)
{
//	if(povolit)
//		{F->scGPButton_ulozit->Enabled=true;}//pokud je dùvod k uložení, ale button uložit je z pøedchozího kroku neaktivní zapne ho
//	else
//		{F->scGPButton_ulozit->Enabled=false;}//pokud je button uložit zapnut vypne ho
//	F->PmG->Update();//musí být, pøi vložení prvního kontinuálního robota problém v zobrazení
//	F->PmG->SetEnabledComponents(povolit);
//	F->PmG->SetEnabledComponent(1,1,true);//rychlost musí být aktivní aby ji mohl uživatel zmìnit a tím odemknout ostatní buòky
//	if(F->OBJEKT_akt->elementy->dalsi!=NULL)
//	{
//		Cvektory::TElement *E=F->OBJEKT_akt->elementy->dalsi;//mùžu pøeskoèit hlavièku
//		while(E!=NULL)
//		{
//			E->mGrid->SetEnabledComponents(povolit);
//			E=E->dalsi;
//		}
//		E=NULL;delete E;
//	}
}
//---------------------------------------------------------------------------
//vrátí ukazatel na element, nelze použít vra element stavající, z dùvodu že ID tabulky již nemusí být totožné s n robota
Cvektory::TElement *TFormX::vrat_element_z_tabulky(long ID)
{
	Cvektory::TElement *ret=NULL;
	Cvektory::TElement *E=F->OBJEKT_akt->element;//mùžu pøeskoèit element, metoda voláná po kliku do tabulky elementu
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
					int row=6;if(E->eID==5)row=4;//pøepínání od kterého øádku zaèít, rùzné elementy
					E->mGrid->Cells[1][row].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][row+1].Text=F->outDO(F->m.round2double(E->zona_za,3));//refresh mGridu není tøeba
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
void TFormX::prirazeni_pohohonu_vetvi(Cvektory::TElement *E,bool hlavni)
{
	////deklarace potøebných promìnných
	TscGPComboBox *Combo;
	if(hlavni)Combo=E->mGrid->getCombo(1,2);
	else Combo=E->mGrid->getCombo(2,2);
	Cvektory::TPohon *p=NULL;
	if(Combo->ItemIndex!=0)p=F->d.v.vrat_pohon(Combo->ItemIndex);
	Cvektory::TElement *e=NULL;

  ////uložení aktuálnì editovaného pohonu
	F->OBJEKT_akt->pohon=p;
	////zmìna pohonu na hlavní vìtvi
	if(hlavni)
	{
		E->pohon=p;
		F->OBJEKT_akt->pohon=p;
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(e->eID==200)break;//pokud narazím na PM zmìním mu pohon a skonèím prùchod
			e=e->dalsi;
		}
		e=E->predchozi;
		while(e!=NULL && e->n>0)
		{
			if(e->eID==200 || F->predchozi_PM==NULL && e->objekt_n==F->OBJEKT_akt->n)break;//pokud narazím na PM NEzmìním! mu pohon a skonèím prùchod, nebo narazím na konec objektu
			e->pohon=p;
			e=e->predchozi;
		}
	}
	////zmìna pohonu na vedlejší vìtvi
	else
	{
  	e=E->dalsi2;//pøiøazuji pohon z výhybky
		while(e!=NULL && e->idetifikator_vyhybka_spojka!=E->idetifikator_vyhybka_spojka)
		{
			e->pohon=p;
			e=e->dalsi;
		}
		//WT øeší fce. vložení PM
	}

	////naètení dat z pohonu do mGridu
	update_hodnot_vyhybky_PM(E);
	vstoupeno_elm=false;//zamezení, aby pøi aktualizace comba došlo k propadnutí do onchange
	//F->zmena_editovanych_bunek(E);//aktualizuje combo a pøepíná buòky na edit èi draw
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT všem elementù
	vstoupeno_elm=true;//navrácení stavu

	////ukazatelové záležitosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
//zobrazí aktuální hodnoty z dat v tabulkách
void TFormX::update_hodnot_vyhybky_PM(Cvektory::TElement *E)
{
	if(E->pohon!=NULL)
	{
		if(input_state!=aRD || input_state==aRD && E!=posledni_E)E->mGrid->Cells[1][3].Text=F->m.round2double(F->outaRD(E->pohon->aRD),3);
		E->mGrid->Cells[1][4].Text=AnsiString(F->m.round2double(F->outaRD(E->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(E->pohon->rychlost_do),3));
		if(input_state!=R || input_state==R && E!=posledni_E)E->mGrid->Cells[1][5].Text=F->m.round2double(F->outR(E->pohon->roztec),3);
		if(input_state!=Rx || input_state==Rx && E!=posledni_E)E->mGrid->Cells[1][6].Text=F->m.round2double(E->pohon->Rx,3);
		E->mGrid->Cells[1][7].Text=F->m.round2double(F->outRz(E->pohon->Rz),3);
		E->mGrid->Cells[1][8].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->pohon->Rz,0)),3);
		E->mGrid->Cells[1][9].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->pohon->Rz,1)),3);
		E->mGrid->Cells[1][10].Text=F->m.round2double(F->outRz(F->m.mezera(90,E->pohon->Rz,1)),3);
	}
	else
	{
		E->mGrid->Cells[1][3].Text=0;
		E->mGrid->Cells[1][4].Text=0;
		E->mGrid->Cells[1][5].Text=0;
		E->mGrid->Cells[1][6].Text=0;
		E->mGrid->Cells[1][7].Text=0;
		E->mGrid->Cells[1][8].Text=0;
		E->mGrid->Cells[1][9].Text=0;
		E->mGrid->Cells[1][10].Text=0;
	}
	if(E->eID==300)//pro výhybku
	{
  	if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)
		{
			if(input_state!=aRD || input_state==aRD && E!=posledni_E)E->mGrid->Cells[2][3].Text=F->m.round2double(F->outaRD(E->dalsi2->pohon->aRD),3);
  		E->mGrid->Cells[2][4].Text=AnsiString(F->m.round2double(F->outaRD(E->dalsi2->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(E->dalsi2->pohon->rychlost_do),3));
			if(input_state!=R || input_state==R && E!=posledni_E)E->mGrid->Cells[2][5].Text=F->m.round2double(F->outR(E->dalsi2->pohon->roztec),3);
			if(input_state!=Rx || input_state==Rx && E!=posledni_E)E->mGrid->Cells[2][6].Text=F->m.round2double(E->dalsi2->pohon->Rx,3);
  		E->mGrid->Cells[2][7].Text=F->m.round2double(F->outRz(E->dalsi2->pohon->Rz),3);
  		E->mGrid->Cells[2][8].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->dalsi2->pohon->Rz,0)),3);
  		E->mGrid->Cells[2][9].Text=F->m.round2double(F->outRz(F->m.mezera(0,E->dalsi2->pohon->Rz,1)),3);
			E->mGrid->Cells[2][10].Text=F->m.round2double(F->outRz(F->m.mezera(90,E->dalsi2->pohon->Rz,1)),3);
			E->mGrid->Cells[2][11].Text=F->m.round2double(F->outPT(E->WT),3);
  	}
  	else
  	{
			E->mGrid->Cells[2][3].Text=0;
			E->mGrid->Cells[2][4].Text=0;
			E->mGrid->Cells[2][5].Text=0;
			E->mGrid->Cells[2][6].Text=0;
			E->mGrid->Cells[2][7].Text=0;
			E->mGrid->Cells[2][8].Text=0;
			E->mGrid->Cells[2][9].Text=0;
			E->mGrid->Cells[2][10].Text=0;
		}
	}
	else//pro PM
	{
    Cvektory::TElement *e_pom=E->dalsi;
		if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;//v pøípadì PM na konci linky
		if(e_pom->pohon!=NULL)
		{
			if(input_state!=aRD || input_state==aRD && E!=posledni_E)E->mGrid->Cells[2][3].Text=F->m.round2double(F->outaRD(e_pom->pohon->aRD),3);
			E->mGrid->Cells[2][4].Text=AnsiString(F->m.round2double(F->outaRD(e_pom->pohon->rychlost_od),3))+" - "+AnsiString(F->m.round2double(F->outaRD(e_pom->pohon->rychlost_do),3));
			if(input_state!=R || input_state==R && E!=posledni_E)E->mGrid->Cells[2][5].Text=F->m.round2double(F->outR(e_pom->pohon->roztec),3);
			if(input_state!=Rx || input_state==Rx && E!=posledni_E)E->mGrid->Cells[2][6].Text=F->m.round2double(e_pom->pohon->Rx,3);
			E->mGrid->Cells[2][7].Text=F->m.round2double(F->outRz(e_pom->pohon->Rz),3);
			E->mGrid->Cells[2][8].Text=F->m.round2double(F->outRz(F->m.mezera(0,e_pom->pohon->Rz,0)),3);
			E->mGrid->Cells[2][9].Text=F->m.round2double(F->outRz(F->m.mezera(0,e_pom->pohon->Rz,1)),3);
			E->mGrid->Cells[2][10].Text=F->m.round2double(F->outRz(F->m.mezera(90,e_pom->pohon->Rz,1)),3);
			E->mGrid->Cells[2][11].Text=F->m.round2double(F->outPT(E->WT),3);
		}
		else
		{
    	E->mGrid->Cells[2][3].Text=0;
			E->mGrid->Cells[2][4].Text=0;
			E->mGrid->Cells[2][5].Text=0;
			E->mGrid->Cells[2][6].Text=0;
			E->mGrid->Cells[2][7].Text=0;
			E->mGrid->Cells[2][8].Text=0;
			E->mGrid->Cells[2][9].Text=0;
			E->mGrid->Cells[2][10].Text=0;
		}
		e_pom=NULL;delete e_pom;
	}
	vstoupeno_elm=false;//blokace cyklení
	F->napln_comba_mGridu(E);
	vstoupeno_elm=true;
}
//---------------------------------------------------------------------------
//provede validaci RD
void TFormX::validace_RD(Cvektory::TElement *E)
{
	if(E->eID==200)//validovat pouze u PM
	{
		//deklarace
		AnsiString jednotky;
  	if(F->aRDunit==0)jednotky="[m/s]";
		else jednotky="[m/min]";
		AnsiString puv_Note=E->mGrid->Note.Text;
  	bool mimo_rozmezi=false;
		bool pouze_rozmezi=false;
		unsigned int p1_n=E->mGrid->getCombo(1,2)->ItemIndex,p2_n=E->mGrid->getCombo(2,2)->ItemIndex;
		double dopRD=0;
		validovany_pohon=0;

		//naètení pohonù
		Cvektory::TPohon *p=NULL,*p1=NULL,*p2=NULL;
  	if(p1_n!=0)p1=F->d.v.vrat_pohon(p1_n);
		if(p2_n!=0)p2=F->d.v.vrat_pohon(p2_n);

		//dvojtá validace
    //pokud nalezne problém zastaví se a zobrazího, i v pøípadì, že je problémù více, až bude problém vyøešen probìhne validace zda neexistuje další problém
		if(F->OBJEKT_akt->rezim==1)//kontrola zda je KK režim, pokud ano validovat
		{
	  	for(unsigned int i=1;i<=2;i++)
			{
	  		//naètení požadovaného pohonu pro validaci
	  		if(i==1)p=p1;
	  		if(i==2)p=p2;
	  		//kontrola zda je možné editovat pohon
	  		if(E->mGrid->Cells[i][3].Type==E->mGrid->EDIT && p!=NULL)
	  		{
					//kontrola zda je zadaná hodnota v rozmezí
					if(F->m.between(p->aRD,p->rychlost_od,p->rychlost_do))mimo_rozmezi=false;
	     		else mimo_rozmezi=true;
	     		//zadaná rychlost je mimo rozsah
					if(mimo_rozmezi && p->aRD > 0)
					{
						E->mGrid->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpovídá rozmezí!"
						povolit_zakazat_editaci(false);//zakáže btn uložit v editaci
						validovany_pohon=i;//uložení pro který pohon platí validace
						break;//byl nalezen problém, zastavení validace, lze zobrazit jen jeden problém v Note
	     		}
					if(!mimo_rozmezi && E->mGrid->Note.Text!="")
					{
						E->mGrid->ShowNote("",F->d.clError,14);
						povolit_zakazat_editaci(true);
					}
       		// nutné ošetøení pro období zadávání/psaní
					if (p->aRD > 0)
	     		{
	     			//výpoèet doporuèené rychosti
						double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,p->roztec,F->d.v.PP.TT,p->aRD);
						double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,p->roztec,F->d.v.PP.TT,p->aRD);
						if(dopRD1>dopRD2)dopRD=dopRD1;//vypíše vìtší hodnotu
	     			else dopRD=dopRD2;
	     			//je zvolen pohon, jeho aktuální rychlost se nerovná doporuèené
						if(p->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(p->aRD) && mimo_rozmezi==false)
	     			{
							if(E->mGrid->Note.Text=="")povolit_zakazat_editaci(false);//ošetøeno podmínkou proti opìtovnému spouštìní
							E->mGrid->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporuèenou rychlost pohonu:"
							validovany_pohon=i;//uložení pro který pohon platí validace
							break;//byl nalezen problém, zastavení validace, lze zobrazit jen jeden problém v Note
						}
	     			//vše je vpoøádku
						if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(p->aRD) && mimo_rozmezi==false)
	     			{
	     				povolit_zakazat_editaci(true);
							E->mGrid->ShowNote("",F->d.clError,14);
						}
	     		}
					else E->mGrid->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatná hodnota rychlosti pohonu!"
				}
			}
		}

		//roznesení validace na ostatní
		if(puv_Note!=E->mGrid->Note.Text)//došlo ke zmìnì note
		{
			puv_Note=E->mGrid->Note.Text;
			if(F->predchozi_PM!=NULL && F->predchozi_PM!=E && F->predchozi_PM->mGrid->getCombo(1,2)->ItemIndex==validovany_pohon || F->predchozi_PM->mGrid->getCombo(2,2)->ItemIndex==validovany_pohon)//pokud má PM stejný pohon jako validovaný pohon
				F->predchozi_PM->mGrid->ShowNote(puv_Note,F->d.clError,14);
			Cvektory::TElement *e_pom=F->OBJEKT_akt->element;
			while(e_pom!=NULL && e_pom->objekt_n==F->OBJEKT_akt->n)
			{
				if(e_pom->eID==200 && (e_pom->mGrid->getCombo(1,2)->ItemIndex==validovany_pohon || e_pom->mGrid->getCombo(2,2)->ItemIndex==validovany_pohon))
					e_pom->mGrid->ShowNote(puv_Note,F->d.clError,14);
				e_pom=F->d.v.dalsi_krok(e_pom,F->OBJEKT_akt);
			}
			e_pom=NULL;delete e_pom;
		}

  	//ukazatelové záležitosti
		p=NULL;p1=NULL;p2=NULL;
		delete p;delete p1;delete p2;
	}
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
	Cvektory::TElement *e=NULL;

	////uložení aktuálnì editovaného pohonu
	F->OBJEKT_akt->pohon=p;
	
	////pøiøazení pohonu pøed PM
	if(Col==1)
	{
		E->pohon=p;
		e=E->predchozi;
		while(e!=NULL && e->n>0 && e->objekt_n==F->OBJEKT_akt->n)
		{
			if(e->eID==200)break;//narazil jsem na PM, konec
			else e->pohon=p;
			e=e->predchozi;
		}
		if(e!=NULL && e->n>0 && e->eID==200)update_hodnot_vyhybky_PM(e);//skonèil jsem pøiøazování pohonu na PM, aktualizace spojky
		//došel jsem na zaèátek obìktu a mám v pøedchozím PM
		if(e!=NULL && e->n>0 && (e==F->OBJEKT_akt->element || e==F->OBJEKT_akt->element->predchozi) && F->predchozi_PM!=NULL)
		{
			e=F->predchozi_PM->dalsi;
			while(e!=NULL && e!=F->OBJEKT_akt->element)
			{
				e->pohon=p;
				e=e->dalsi;
			}
			update_hodnot_vyhybky_PM(F->predchozi_PM);//aktualizace hodnot na pøedchozím PM
		}
	}
	////pøiøazení pohonu za PM
	if(Col==2)
	{
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(e->eID==200)break;//narazil jsem na PM, zapsat nový pohon a konec
			else e=e->predchozi;
		}
		if(e!=NULL && e->eID==200)update_hodnot_vyhybky_PM(e);//skonèil jsem na pøedávacím místì, update tohoto PM
		if(E->dalsi!=NULL && E->dalsi->objekt_n==E->objekt_n)E->WT=F->m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
	}

	////naètení dat z pohonu do mGridu
	update_hodnot_vyhybky_PM(E);
	vstoupeno_elm=false;//zamezení, aby pøi aktualizace comba došlo k propadnutí do onchange
	//F->zmena_editovanych_bunek(E);//aktualizuje combo a pøepíná buòky na edit èi draw
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT všem elementù
	vstoupeno_elm=true;//navrácení stavu

	////ukazatelové záležitosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
