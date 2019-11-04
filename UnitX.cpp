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
	    		E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
	    		E->mGrid->VisibleRow(3,true,false);//nepøekreslovat
	    		E->mGrid->VisibleRow(5,true,false); E->mGrid->VisibleRow(6,true,false);
				}
			}break;
			case 3:case 9:case 13:case 17:case 103:case 107:
			{
				if(E->mGrid->Rows[6].Visible)
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(6,false,false);
				}
				else
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(6,true,false);
				}
			}break;
			case 5://KK otoè
			{
				if(E->mGrid->Rows[4].Visible)
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					E->mGrid->VisibleRow(4,false,false);
				}
				else
				{
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					E->mGrid->VisibleRow(4,true,false);
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
	if(ID!=9999 && F->pom_temp->pohon!=NULL)//korelace v tabulkách elementù
	korelace_v_elementech(ID,Row);
}
//---------------------------------------------------------------------------
//zpracování onchange události - INPUT, výpoèet a OUTPUT zpìt do ovlivnìné buòky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING&&ID!=9999&&vstoupeno_elm&&!editace_pohonu)
	{
		F->Timer_neaktivity->Enabled=false;//vypnutí timeru neaktivity, pokud dochází k OnChange rychle za sebou nestpustí timer refresh
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);//ne vždy je ID tabulky stejné jako n robota, proto nutné hledat ID=ID ne ID=n
    F->posledni_editovany_element=E;
		switch(E->eID)
		{
			case 0: //stop
			{
				if(Row==3)//zmìna max. WT stop
				{
					input_state=WT;//nastaveni stavu
					E->WTstop=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->akt_pocet_voziku=F->m.WT2V(E->WTstop,F->d.v.PP.TT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][6].Text=F->m.round2double(E->akt_pocet_voziku,3);//OUTPUT
				}
				if(Row==6)//max. poèet vozíkù
				{
					input_state=P_VOZ;//nastaveni stavu
					E->akt_pocet_voziku=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);//INPUT
					E->WTstop=F->m.V2WT(E->akt_pocet_voziku,F->d.v.PP.TT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][3].Text=F->m.round2double(F->outPT(E->WTstop),3);//OUTPUT
				}
				//dodìlat plnìní pamìti pøi editaci bunìk
			} break;
			case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinuální)
			{
				if(Row==1)// editace PT
				{
					input_state=PT; //nastaveni stavu
					E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(E->PT1!=0)
					{
						F->pom_temp->pohon->aRD=E->LO1/E->PT1;//uložení do pamìti + výpoèet
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);//OUTPUT
						//nutné ošetøení podmínkou, v pøípadì editace textu, který je celý oznaèená událost OnChange spuštìna 2x
			  		zmena_aRD(E);
						F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
					}
				}
				if(Row==2)// eidtace LO
				{
					input_state=LO; //nastaveni stavu
					E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->PT1=E->LO1/F->pom_temp->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT1),3); //OUTPUT
				}
				if(Row==3)// eidtace LO_pozice
				{
					input_state=LO2; //nastaveni stavu
					E->LO_pozice=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//pouze uložení do dat
				}
				if(Row==4)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->PD=E->mGrid->getCombo(1,Row)->ItemIndex;//pouze uložení do dat
				}
			} break;
			case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanicí
			{
				if(Row==1)//editace PT
				{
					input_state=PT;//nastaveni stavu
					E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][2].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
			} break;
			case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivní otoèí
			{
				if (Row==1)//editace PT1
				{
					input_state=PT;//nastaveni stavu
					E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
          //ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(E->PT1!=0)
					{
			  		//ovlivnìní aRD
			  		F->pom_temp->pohon->aRD=E->LO1/E->PT1;
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
			  		//zmìna aRD vyvolá následující pøepoèty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
			  		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
			  		E->PT2=F->m.PT(E->LO2,F->pom_temp->pohon->aRD);
						E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->PT2),3);
			  		//zmìna v ostatních elementech
			  		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
					}
				}
				if (Row==2)//editace LO1
				{
					input_state=LO;//nastaveni stavu
					E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PT1=E->LO1/F->pom_temp->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT1),3);//OUTPUT
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
					F->pridani_elementu_tab_pohon(E);//pouze v KK režimu
				}
				//Row = 4 není uživatelsky upravitelný
				if (Row==5)//editace délky otoèe
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
				}
				if (Row==7)//editace PT2
				{
					input_state=PT;//nastaveni stavu
					E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					//ošetøení proti dìlení 0 a proti spouštìní pøi prázdném øetìzci v EDITu
					if(E->PT2!=0)
					{
			   		F->pom_temp->pohon->aRD=E->LO2/E->PT2;//uložení do pamìti + výpoèet
			   		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);//OUTPUT
			   		//zmìna aRD vyvolá následující pøepoèty
			   		E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
			   		E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
			   		E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
			   		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zmìna v ostatních elementech
			   		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//došlo ke zmìnì hodnot v PmG
					}
				}
				if (Row==8)//editace LO2
				{
					input_state=LO2;//nastaveni stavu
					E->LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PT2=E->LO2/F->pom_temp->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT2),3);//OUTPUT
				}
				if(Row==9)// eidtace COMBO PD
				{
					input_state=COMBO; //nastaveni stavu
					E->PD=E->mGrid->getCombo(1,Row)->ItemIndex;//pouze uložení do dat
				}
			} break;
			case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí (resp. s otoèí a stop stanicí)
			{
				if(Row==1)//zmìna PT1
				{
					input_state=PT;//nastaveni stavu
					E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
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
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
				if(Row==4)//zmìna PT2
				{
					input_state=PT2;//nastaveni stavu
					E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
				if(Row==6)//zmìna WT
				{
					input_state=WT;//nastaveni stavu
					E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
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
					if(F->pom_temp->rezim==1)F->pridani_elementu_tab_pohon(E);//pouze v KK režimu, pasivní otoè mùže být i ve S&G lakovnì
				}
				if (Row==2)//zde se upravuje pouze délka
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;//uložení do pamìti + výpoèet
					E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
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
					//E->OTOC_delka=F->pom_temp->pohon->aRD*E->PTotoc;//uložení do pamìti + výpoèet
					//E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outDO(E->OTOC_delka),3);//OUTPUT
					E->RT=F->m.RT(E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->RT),3);
				}
			} break;
		}
		E->mGrid->Refresh();//refresh aktuálnì upravované tabulky
		posledni_E=E;//uložení posledního editovaného elementu
		E=NULL;delete E;
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
					//aktualizace tabulky
					if(F->PmG->Rows[3].Visible)//pro tabulku v kontinuálním režimu
					{
						F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
						F->PmG->Cells[1][roztec_palce].Text=F->m.round2double(F->outR(F->pom_temp->pohon->roztec),3);
						F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
						F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->pom_temp->pohon->Rx,0);
						F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
						F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
						//místo pro mezeru mezi podvozky
						if(F->PmG->Rows[mezera_jig2].Visible)//budou zde obì mezeri mezi jigy
						{
							F->PmG->Cells[1][mezera_jig1].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1));
							F->PmG->Cells[1][mezera_jig2].Text=F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1));
						}
						else if(F->PmG->Rows[mezera_jig1].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
						{
							double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
							F->PmG->Cells[1][mezera_jig1].Text=F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1));
						}
					}
					else
					{
						F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
						F->PmG->Cells[1][roztec_palce].Text=F->m.round2double(F->outR(F->pom_temp->pohon->roztec),3);
					}
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
				F->pom_temp->pohon->aRD=aRD;
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
				F->pom_temp->pohon->roztec=R;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_R();
			}break;
			case 3://Rx
			{
				input_state=Rx;
				//naètení Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[1][Row].Text);
				//uložení do spojáku
				F->pom_temp->pohon->Rx=Rx;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_Rx();
			}break;
			case 4://rozestup, Rz   NEBUDE EDITOVATELNÝ
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
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rychlosti
void TFormX::zmena_aRD (Cvektory::TElement *mimo_element)
{
	F->aktualizace_ComboPohon();
	//propoèty v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK režimu
	{
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->pom_temp->pohon->Rx,3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1)),3);
		}
		validace_aRD();//validace pouze v kontinuálním režimu kabiny
	}
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
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz); //prohozené poøadí z dùvodu, že druhý výpoèet potøebuje aktualizovaonu honotu prvního výpoètu
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1)),3);
		}
	}
	//pøepoèet hodnot v elementech
	F->aktualizace_ComboPohon();//zmìna rychlosti, rychlost je zobrazená v Combo pohonù
	aktualizace_tab_elementu();
	//////test validace rozteèe                                                                                                                            //0 = základní jednotky
//	TTextNumber cislo=F->d.v.rVALIDACE(5,F->pom_temp->pohon->n,F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,F->pom_temp->pohon->Rz,F->pom_temp->pohon->Rx,0,0,0);//F->aRDunit,F->Runit,F->Rzunit);
//	F->PmG->ShowNote(cislo.text);
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou Rx
void TFormX::zmena_Rx ()
{
	//pøepoèet hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//kontinuální režim
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1)),3);
		}
	}
	//pøepoèet hodnot v elementech
  F->aktualizace_ComboPohon();//zmìna rychlosti, rychlost je zobrazená v Combo pohonù
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//pøepoèet v tabulkách elementù po zmìnì parametrù v tabulce pohonu
void TFormX::aktualizace_tab_elementu (Cvektory::TElement *mimo_element)
{
	int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->pom_temp->pohon!=NULL && E->pohon->n==F->pom_temp->pohon->n)//pøeskoèí mimo_element a hlavièku, poze pøepoèet elementùm které mají stejný pohon jako aktuálnì editovaný pohon
		{
			switch(E->eID)
			{
				case 0:
				{
					//naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD,roztec=F->pom_temp->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(0,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT+E->WTstop);
					E->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(E->RT),3);
				}
				break;//stop stanice
				case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
				{
          //naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD;
					//pøepoèty
					E->PT1=F->m.PT(E->LO1,aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
				}
				break;
				case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
				{
					//naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD,roztec=F->pom_temp->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT);
					E->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(E->RT),3);
				}
				break;
				case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otoèí
				{
          //naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD,roztec=F->pom_temp->pohon->roztec;
					//pøepoèty
					E->PT1=F->m.PT(E->LO1,aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->PT2=F->m.PT(E->LO2,aRD);
					E->mGrid->Cells[1][7].Text=F->m.round2double(F->outPT(E->PT2),3);
				}
				break;
				case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otoèí
 				{
					//naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD,roztec=F->pom_temp->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT);
					E->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(E->RT),3);
				}break;
 				case 5://otoè pasivní
				{
          //naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD;
					//pøepoèty
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://otoè aktivní
				{
          //naètení hodnot z pohonu
					double aRD=F->pom_temp->pohon->aRD,roztec=F->pom_temp->pohon->roztec;
					//pøepoèty
					E->WT=F->m.cekani_na_palec(0,roztec,aRD,3);//dùležité pro výpoèet RT, nezobrazuje se
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					E->RT=F->m.RT(E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT);
					E->mGrid->Cells[1][4].Text = F->m.round2double(F->outPT(E->RT),3);
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//Naplní hodnoty které závisí na pohonu 0
void TFormX::aktualizace_tab_elementu_pOdebran ()
{
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
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
					E->mGrid->Cells[1][7].Text=0;
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
  int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0 && E->n!=n && E->pohon!=NULL && F->pom_temp->pohon->n==E->pohon->n)
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
					E->mGrid->Cells[1][7].Highlight=true;
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
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//korelace uvnitø elementù
void TFormX::korelace_v_elementech(long ID,long Row)
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
			if (Row==5)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==7){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==8)E->mGrid->Cells[1][Row-1].Highlight=true;
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
			if (Row==2)E->mGrid->Cells[1][Row+1].Highlight=true;
		} break;
		case 6://otoè aktivní (resp. otoè se stop stanicí)
		{
			if (Row==2){/*E->mGrid->Cells[1][Row-1].Highlight=true;*/E->mGrid->Cells[1][Row+1].Highlight=true;}
		} break;
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
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0)
			E->mGrid->unHighlightAll();
		E=E->dalsi;
	}
}
//---------------------------------------------------------------------------
//validace rychlosti pøi její zmìnì
void TFormX::validace_aRD()
{
	AnsiString jednotky;
	if(F->aRDunit==0)jednotky="[m/s]";
	else jednotky="[m/min]";
	bool mimo_rozmezi=false;
	TscGPComboBox *Combo=F->PmG->getCombo(0,0);
	dopRD=0;
	//kontrola zda je zadaná hodnota v rozmezí
	if(F->m.between(F->pom_temp->pohon->aRD,F->pom_temp->pohon->rychlost_od,F->pom_temp->pohon->rychlost_do)) mimo_rozmezi=false;
	else mimo_rozmezi=true;
	// nutné ošetøení pro období zadávání/psaní
	if (F->pom_temp->pohon->aRD > 0)
	{
		//výpoèet doporuèené rychosti
		if(F->PmG->Rows[7].Visible)//v kabinì jsou 2 rùzné rotace
		{
			double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
			double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
			if(dopRD1>dopRD2)dopRD=dopRD1;//vypíše vìtší hodnotu
			else dopRD=dopRD2;
		}
		else
			dopRD=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,F->pom_temp->rotace,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
		//zadaná rychlost je mimo rozsah
		if(mimo_rozmezi)
		{
			if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//ošetøeno podmínkou proti opìtovnému spouštìní
			F->PmG->ShowNote(F->ls->Strings[220],clRed,14);//"Rychlost neodpovídá rozmezí!"
		}
		//je zvolen pohon, jeho aktuální rychlost se nerovná doporuèené
		if(Combo->ItemIndex!=0 && F->pom_temp->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
			if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//ošetøeno podmínkou proti opìtovnému spouštìní
			F->PmG->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,clRed,14);//"Zadejte doporuèenou rychlost pohonu:"
		}
		//vše je vpoøádku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
      povolit_zakazat_editaci(true);
			F->PmG->ShowNote("",clRed,14);
		}
	}
	else F->PmG->ShowNote(F->ls->Strings[222],clRed,14);//"Neplatná hodnota rychlosti pohonu!"
	Combo=NULL;delete Combo;
}
//---------------------------------------------------------------------------
//validace maximálního poètu vozíkù na stopce
void TFormX::validace_max_voziku()
{
	if(posledni_E!=NULL && F->pom_temp!=NULL && posledni_E->objekt_n!=F->pom_temp->n)posledni_E=NULL;//pro pøípad, že se zmìnil náhled
	if(posledni_E!=NULL && posledni_E->eID==0)
	{
		////deklarace potøebných atributù
		bool validace=true;//pøedpoklad, že je vše OK
		////samotná validace
		posledni_E->max_pocet_voziku=F->max_voziku(posledni_E);
		if(posledni_E->max_pocet_voziku>0 && posledni_E->max_pocet_voziku<posledni_E->akt_pocet_voziku){posledni_E->mGrid->ShowNote(F->ls->Strings[250]+" <a>"+AnsiString(posledni_E->max_pocet_voziku)+"</a>");validace=false;}//"Max. poèet vozikù musí být menší nebo roven"
		if(posledni_E->max_pocet_voziku==0){posledni_E->mGrid->ShowNote(F->ls->Strings[251]);validace=false;}//"Nelze, pøed Stopstanicí se nachází oblouk"
		if(posledni_E->max_pocet_voziku>0 && posledni_E->max_pocet_voziku<posledni_E->akt_pocet_voziku)F->TIP="Pro aktuální poèet pozinc je tøeba buffer o délce "+AnsiString(F->d.v.PP.delka_podvozek*posledni_E->akt_pocet_voziku*1000)+" mm.";
		////pøepsání maximálního poèctu vozíku do tabulky elementu, pro jistotu
		posledni_E->mGrid->Cells[1][5].Text=posledni_E->max_pocet_voziku;
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
	F->pom_temp->pohon->aRD=dopRD;
	F->PmG->Cells[1][rychlost].Text=F->outaRD(dopRD);
	zmena_aRD();
	//odstranit_korelaci();//pro jistotu zùstavala aktivní po kliku na link
	F->PmG->ShowNote("",clRed,14);
	povolit_zakazat_editaci(true);
	F->Akce=F->BLOK;
	F->PmG->Refresh();//došlo ke zmìnì hodnot v tabulce
}
//---------------------------------------------------------------------------
//doplní doporuèený poèet maximálních vozíku po kliku
bool TFormX::naplneni_max_voziku(double X,double Y,bool check_for_highlight)
{
	bool ret=false;
	double max_p=0;
	if(F->pom_temp->elementy->predchozi->n>1)
	{
		//hledání zda má nìkterý element nedokonèenou validaci
		Cvektory::TElement *E=F->pom_temp->elementy->dalsi;
		while(E!=NULL)
		{
      //hledání elementu, kterému bylo kliknuto na doporuèený poèet vozíkù
			if(E->eID==0 && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(X,Y)==TPoint(-2,-2)){ret=true;break;}
			E=E->dalsi;
		}
		//naplnìní doporuèeného max. poètu vozíkù
		if(E!=NULL && !check_for_highlight){E->mGrid->Cells[1][6].Text=E->max_pocet_voziku;F->Akce=F->BLOK;}
		E=NULL;delete E;
	}
	return ret;
}
//---------------------------------------------------------------------------
//zakazuje èi povolí komponenty v tabulce pohonu a všech tabulkách elementu
void TFormX::povolit_zakazat_editaci(bool povolit)
{
	if(povolit)
		{F->scGPButton_ulozit->Enabled=true;}//pokud je dùvod k uložení, ale button uložit je z pøedchozího kroku neaktivní zapne ho
	else
		{F->scGPButton_ulozit->Enabled=false;}//pokud je button uložit zapnut vypne ho
//	F->PmG->Update();//musí být, pøi vložení prvního kontinuálního robota problém v zobrazení
//	F->PmG->SetEnabledComponents(povolit);
//	F->PmG->SetEnabledComponent(1,1,true);//rychlost musí být aktivní aby ji mohl uživatel zmìnit a tím odemknout ostatní buòky
//	if(F->pom_temp->elementy->dalsi!=NULL)
//	{
//		Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//mùžu pøeskoèit hlavièku
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
	Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//mùžu pøeskoèit hlavièku, metoda voláná po kliku do tabulky elementu
	while(E!=NULL)
	{
		if(E->mGrid->ID==ID)
		{
			ret=E;
			break;
		}
		E=E->dalsi;
	}
	E=NULL;delete E;
	return ret;
}
//---------------------------------------------------------------------------
