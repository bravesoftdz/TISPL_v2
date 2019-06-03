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
 //pokud dojde ke zm�n� po�ad� ��dku, nastavit nov� pozice zde, + p�eps�n� switche pro tabulku pohonu v OnChange + p�epsan� switche v korelace_tab_pohonu()
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
// p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
// v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
	input_state=NO;
	//funkcionalita exBUTTONu v tabulce pohonu
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		if(F->PmG->Rows[mezera_podvozky].Visible)
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			F->PmG->VisibleRow(mezera_podvozky,false,false);

		}else
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			F->PmG->VisibleRow(mezera_podvozky,true,false);
		}
		F->PmG->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t
		F->PmG->Update();
		F->PmG->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t, toto nesta�ilo: F->PmG->exBUTTON->Top=T;//zajist�, �e se tla��tko nep�epozicuje
		F->REFRESH();//mus� b�t opravdu REFRESH() cel�ho formu + mGridu
		F->PmG->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
	}
	if(ID!=9999&&Row==-2)
	//funkcionalita exBUTTONu v tabulk�ch element�
	{
		Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID);
		AnsiString Hint=E->mGrid->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		if(E->mGrid->Rows[2].Visible)
		{
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			E->mGrid->VisibleRow(2,false,false);//nep�ekreslovat
		}
		else
		{
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->VisibleRow(2,true,false);//nep�ekreslovat
		}
		E->mGrid->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t
		E->mGrid->Update();
		E->mGrid->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t, toto nesta�ilo: F->PmG->exBUTTON->Top=T;//zajist�, �e se tla��tko nep�epozicuje
		F->REFRESH();//mus� b�t opravdu REFRESH cel�ho formu + mGridu
		E->mGrid->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
		E=NULL; delete E;
	}
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
	if(ID==9999&&Row>=1&&F->PmG->Rows[3].Visible)//pokud je kliknuto do tabulky pohonu, podle bu�ky vyzn�� bu�ky, kter� budou zm�nou ovlivn�n�
		korelace_tab_pohonu(Row);//spu�t�no pouze v p��pad� KK tabulky
	if(ID==9999&&Row>=1)korelace_tab_pohonu_elementy();//p�i S&G re�imu je st�le nutn� zobrazit korelaci v elementech
	if(ID!=9999)//korelace v tabulk�ch element�
	korelace_v_elementech(ID,Row);
}
//---------------------------------------------------------------------------
//zpracov�n� onchange ud�losti - INPUT, v�po�et a OUTPUT zp�t do ovlivn�n� bu�ky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING&&ID!=9999&&vstoupeno_elm&&!editace_pohonu)
	{
		F->Timer_neaktivity->Enabled=false;//vypnut� timeru neaktivity, pokud doch�z� k OnChange rychle za sebou nestpust� timer refresh
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);//ne v�dy je ID tabulky stejn� jako n robota, proto nutn� hledat ID=ID ne ID=n
		switch(E->eID)
		{
			case 0: //stop
			{
				if(Row==1)// editace comba
				{
					input_state=COMBO;
					F->d.v.uloz_sparovany_element(E);
				}
				//dod�lat pln�n� pam�ti p�i editaci bun�k
			} break;
			case 1: //robot (kontinu�ln�)
			{
				if(Row==1)// editace PT
				{
					input_state=PT; //nastaveni stavu
					E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					//o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->PT1!=0)
					{
			  		F->pom_temp->pohon->aRD=E->LO1/E->PT1;//ulo�en� do pam�ti + v�po�et
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);//OUTPUT
						//nutn� o�et�en� podm�nkou, v p��pad� editace textu, kter� je cel� ozna�en� ud�lost OnChange spu�t�na 2x
			  		zmena_aRD(E);
						F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
				}
				if(Row==2)// eidtace LO
				{
					input_state=LO; //nastaveni stavu
					E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->PT1=E->LO1/F->pom_temp->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT1),3); //OUTPUT
				}
			} break;
			case 2: //robot se stop stanic�
			{
				if(Row==1)//editace PT
				{
					input_state=PT;//nastaveni stavu
					E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][2].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
			} break;
			case 3: //robot s pasivn� oto��
			{
				if (Row==1)//editace PT1
				{
					input_state=PT;//nastaveni stavu
					E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
          //o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->PT1!=0)
					{
			  		//ovlivn�n� aRD
			  		F->pom_temp->pohon->aRD=E->LO1/E->PT1;
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
			  		//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
			  		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
			  		E->PT2=F->m.PT(E->LO2,F->pom_temp->pohon->aRD);
						E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->PT2),3);
			  		//zm�na v ostatn�ch elementech
			  		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
				}
				if (Row==2)//editace LO1
				{
					input_state=LO;//nastaveni stavu
					E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PT1=E->LO1/F->pom_temp->pohon->aRD;//ulo�en� do pam�ti + v�po�et
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
				}
				//Row = 4 nen� u�ivatelsky upraviteln�
				if (Row==5)//editace d�lky oto�e
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
				}
				if (Row==6)//editace PT2
				{
					input_state=PT;//nastaveni stavu
					E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					//o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->PT2!=0)
					{
			   		F->pom_temp->pohon->aRD=E->LO2/E->PT2;//ulo�en� do pam�ti + v�po�et
			   		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);//OUTPUT
			   		//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			   		E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
			   		E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
			   		E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
			   		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zm�na v ostatn�ch elementech
			   		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
					}
				}
				if (Row==7)//editace LO2
				{
					input_state=LO;//nastaveni stavu
					E->LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					E->PT2=E->LO2/F->pom_temp->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT2),3);//OUTPUT
				}
			} break;
			case 4://robot s aktivn� oto�� (resp. s oto�� a stop stanic�)
			{
				if(Row==1)//zm�na PT1
				{
					input_state=PT;//nastaveni stavu
					E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//ulo�en� do pam�ti + v�po�et
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
				if(Row==3)//zm�na PTo
				{
					input_state=PTotoc;//nastaveni stavu
					E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
				if(Row==4)//zm�na PT2
				{
					input_state=PT2;//nastaveni stavu
					E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
				if(Row==6)//zm�na WT
				{
					input_state=WT;//nastaveni stavu
					E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//OUTPUT
				}
			} break;
			case 5://oto� pasivn�
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
				if (Row==2)//zde se upravuje pouze d�lka
				{
					input_state=DO;//nastaveni stavu
					E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);//OUTPUT
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
				if (Row==3)
				{
					input_state=PTotoc;//nastaveni stavu
					E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//INPUT
					E->OTOC_delka=F->pom_temp->pohon->aRD*E->PTotoc;//ulo�en� do pam�ti + v�po�et
					E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outDO(E->OTOC_delka),3);//OUTPUT
				}
			} break;
		}
		E->mGrid->Refresh();//refresh aktu�ln� upravovan� tabulky
		E=NULL;delete E;
		input_state=NOTHING;F->Timer_neaktivity->Enabled=true;//uvoln�n� stavu + zapnuti timeru neaktivity, pokud dokon�� �asov�n� spust� REFRESH
		F->nahled_ulozit(true);//prob�hla zm�na tabulky -> je d�vod ulo�it n�hled
	}
	if(input_state==NOTHING&&ID==9999&&vstoupeno_poh)
	{
		F->Timer_neaktivity->Enabled=false;//vypnut� timeru neaktivity, pokud doch�z� k OnChange rychle za sebou nestpust� timer refresh
		switch(Row)
		{
			case 0://v�b�r pohonu
			{
				input_state=COMBO;//nastaveni stavu
				F->tab_pohon_COMBO(1);//p�i�azen� pohonu
				F->aktualizace_ComboPohon();//aktualizace popisk� pohonu v combo, po p�i�azen� zm�na na 0t�m ��dku z "vyber pohon" na "odeber pohon"
				if(F->PmG->getCombo(0,0)->ItemIndex!=0)//pokud je vybr�n n�jak� pohon
				{
					F->scGPComboBox_prepinacKot->Enabled=true;//zapnut� zm�ny typu k�t, rychlost v tomto kroku bude v�dy v�t�� ne� 0
					F->scGPGlyphButton_PLAY->Enabled=true;//zapnut� tla��tka animace
					//aktualizace tabulky
					if(F->PmG->Rows[3].Visible)//pro tabulku v kontinu�ln�m re�imu
					{
						F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
						F->PmG->Cells[1][roztec_palce].Text=F->m.round2double(F->outR(F->pom_temp->pohon->roztec),3);
						F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
						F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->pom_temp->pohon->Rx,0);
						F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
						F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
						//m�sto pro mezeru mezi podvozky
						if(F->PmG->Rows[mezera_jig2].Visible)//budou zde ob� mezeri mezi jigy
						{
							F->PmG->Cells[1][mezera_jig1].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1));
							F->PmG->Cells[1][mezera_jig2].Text=F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1));
						}
						else if(F->PmG->Rows[mezera_jig1].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
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
					aktualizace_tab_elementu();//do�lo ke zm�n�m v tabulce pohonu, kter� ovlivn� i ostatn� elementy
				}
				else//pohon byl odebr�n
				{
					aktualizace_tab_elementu_pOdebran();//p�i�ad� v�em polo�k�m v elementech z�visl�ch na hodnot�ch pohonu 0
					F->scGPComboBox_prepinacKot->Enabled=false;//vypne zm�nu re�imu k�t
					F->scGPGlyphButton_PLAY->Enabled=false;//vypnut� tla��tka animace
				}
			}break;
			case 1://aktu�ln� rychlost, aRD
			{
				input_state=aRD;
				//na�ten� aRD z editu
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
				//ulo�en� do spoj�ku
				F->pom_temp->pohon->aRD=aRD;
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
			case 2://rozte�, R
			{
				input_state=R;
				//na�ten� R z editu
				double R=F->inR(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
				//ulo�en� do spoj�ku
				F->pom_temp->pohon->roztec=R;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
				zmena_R();
			}break;
			case 3://Rx
			{
				input_state=Rx;
				//na�ten� Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[1][Row].Text);
				//ulo�en� do spoj�ku
				F->pom_temp->pohon->Rx=Rx;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
				zmena_Rx();
			}break;
			case 4://rozestup, Rz   NEBUDE EDITOVATELN�
			break;
		}
		F->PmG->Refresh();input_state=NOTHING;F->Timer_neaktivity->Enabled=true;
		F->nahled_ulozit(true);
	}
}
//---------------------------------------------------------------------------
void TFormX::OnKeyPress(long Tag,long ID,unsigned long Col,unsigned long Row,System::WideChar &Key)
{
	if(Row==0&&Col==0&&ID!=9999)//nutn� o�et�en� aby sem nepadaly v�ci z �e�en� v OnChange
	{
		if((Key==VK_ESCAPE||Key==VK_RETURN)&&F->pom_element_temp!=NULL)//stisknut� ESC nebo ENTER
		{    //p�i esc zru�it ozna�en� textu
			if(Key==VK_ESCAPE)
			{
				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text;//pokud je stisknuto ESC, vr�t� p�vodn� n�zev
				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=F->pom_element_temp->mGrid->Cells[0][0].Text.Length();//zamez� selectov�n� cel�ho textu p�i stisku ESC
			}
			F->index_kurzoru=9999;//mus� b�t p��tomno, zabra�uje smaz�n� editu hned po vytvo�en�, timer vol�n i z OnChange
			F->Timer2->Enabled=true;//sma�e edit a ulo�� n�zev (p�vodn� nebo zm�n�n�),edit mus� b�t smaz�n se spo�den�m, jinak pam�ov� chyba
   	}
		else
   	{
			//�e�eno takto z d�vodu, �e v okamziku stisknut� kl�vesy je v editu text bez p�i�ten� kl�vesy, muselo by b�t implementov�no do OnChange, aby fungovalo norm�ln�
			if(Key==VK_BACK)//stisknuto backspace
   			F->pom_element_temp->name=F->pom_element_temp->name.SubString(1,F->pom_element_temp->name.Length()-1);
   		else//ostatn� kl�vesy
				F->pom_element_temp->name=F->pom_element_temp->mGrid->Cells[0][0].Text+AnsiString(Key);
   		//pokud se jend� o stopku, je mo�n� editovat pouze kone�n� ��slo
   		if(F->pom_element_temp->eID==0&&F->pom_element_temp->name.Length()<=4)
			{
				Key=0;//nutn�! OnKeyPress je vol�na 2x
				F->pom_element_temp->mGrid->Cells[0][0].Text="Stop ";//nahrazen� toho co je v editu
				F->pom_element_temp->mGrid->getEdit(0,0)->SelStart=5;//nastaven� kurzoru na konec editu
				MessageBeep(0);//ozn�men� u�ivateli
			}
			F->REFRESH(false);
   	}
	}
	if(Row==3&&ID==9999)//slou�� pro nepovolen� zaps�n� desetin� ��rky do editu Rx
	{
		if(AnsiString(Key)==F->ms.get_locale_decimal())
		{
			Key=0;
			MessageBeep(0);
		}
	}
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rychlosti
void TFormX::zmena_aRD (Cvektory::TElement *mimo_element)
{
	F->aktualizace_ComboPohon();
	//propo�ty v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK re�imu
	{
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->pom_temp->pohon->Rx,3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1)),3);
		}
		validace();//validace pouze v kontinu�ln�m re�imu kabiny
	}
	//propo�ty v tabulk�ch element�
	aktualizace_tab_elementu(mimo_element);
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rozte�e
void TFormX::zmena_R ()
{
	//p�epo�et hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK re�imu
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz); //prohozen� po�ad� z d�vodu, �e druh� v�po�et pot�ebuje aktualizovaonu honotu prvn�ho v�po�tu
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1)),3);
		}
	}
	//p�epo�et hodnot v elementech
	F->aktualizace_ComboPohon();//zm�na rychlosti, rychlost je zobrazen� v Combo pohon�
	aktualizace_tab_elementu();
	//////test validace rozte�e                                                                                                                            //0 = z�kladn� jednotky
//	TTextNumber cislo=F->d.v.rVALIDACE(5,F->pom_temp->pohon->n,F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,F->pom_temp->pohon->Rz,F->pom_temp->pohon->Rx,0,0,0);//F->aRDunit,F->Runit,F->Rzunit);
//	F->PmG->ShowNote(cislo.text);
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou Rx
void TFormX::zmena_Rx ()
{
	//p�epo�et hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//kontinu�ln� re�im
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1)),3);
		}
	}
	//p�epo�et hodnot v elementech
  F->aktualizace_ComboPohon();//zm�na rychlosti, rychlost je zobrazen� v Combo pohon�
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//p�epo�et v tabulk�ch element� po zm�n� parametr� v tabulce pohonu
void TFormX::aktualizace_tab_elementu (Cvektory::TElement *mimo_element)
{
	int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0&&E->n!=n)//p�esko�� mimo_element a hlavi�ku
		{
			switch(E->eID)
			{
				case 0:
				{
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
//					E->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(E->WT),3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT),3);
				}
				break;//stop stanice
				case 1://robor kontinu�ln�
				{
					E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
				}
				break;
				case 2://robot se stop stanic�
				{
					//validace
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
					E->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(E->RT),3);
				}
				break;
				case 3://robot s pasivn� oto��
				{
					E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->PT2=F->m.PT(E->LO2,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->PT2),3);
				}
				break;
				case 4://robot s aktivn� oto��
 				{
					//validace
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E,true),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
					E->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(E->RT),3);
				}break;
 				case 5://oto� pasivn�
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://oto� aktivn�
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//Napln� hodnoty kter� z�vis� na pohonu 0
void TFormX::aktualizace_tab_elementu_pOdebran ()
{
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0)
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[1][3].Text=0;
				}
				break;//stop stanice
				case 1://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Text=0;
				}
				break;
				case 2://robot se stop stanic�
				{
					//validace
					E->mGrid->Cells[1][2].Text=0;
					E->mGrid->Cells[1][3].Text=0;
				}
				break;
				case 3://robot s pasivn� oto��
				{
					E->mGrid->Cells[1][1].Text=0;
					E->mGrid->Cells[1][4].Text=0;
					E->mGrid->Cells[1][6].Text=0;
				}
				break;
				case 4://robot s aktivn� oto��
				{
					//validace
					E->mGrid->Cells[1][5].Text=0;
					E->mGrid->Cells[1][6].Text=0;
				}break;
				case 5://oto� pasivn�
				{
					E->mGrid->Cells[1][3].Text=0;
				}break;
				case 6://oto� aktivn�
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
//highlightovan� bu��k tabulky pohonu
void TFormX::korelace_tab_pohonu(int Row)
{
	switch(Row)
	{
		case 1: //zmena aRD
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][roztec_jigu].Highlight=true; //rozestup
				F->PmG->Cells[1][nasobek_roztece].Highlight=true; //rx
				F->PmG->Cells[1][mezera_podvozky].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[1][mezera_jig1].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][mezera_jig2].Highlight=true;
			}
		}break;
		case 2: //zmena rozte�e R
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][roztec_jigu].Highlight=true;  //rozestup
				F->PmG->Cells[1][rychlost].Highlight=true; //aRD
				F->PmG->Cells[1][mezera_podvozky].Highlight=true; //mezera mezi podvozky
				F->PmG->Cells[1][mezera_jig1].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][mezera_jig2].Highlight=true;
			}
		}break;
		case 3: //zm�na Rx
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
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
//star� se o highlitov�n� pol��ek, kter� souvis� s m�n�nou hodnotou (elementy)
void TFormX::korelace_tab_pohonu_elementy(Cvektory::TElement *mimo_element)
{
  int n=999999999;
	if(mimo_element!=NULL)n=mimo_element->n;
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0&&E->n!=n)
		{
			switch(E->eID)
			{
				case 0:
				{
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;//stop stanice
				case 1://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Highlight=true;
				}
				break;
				case 2://robot se stop stanic�
				{
					//validace
					E->mGrid->Cells[1][2].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 3://robot s pasivn� oto��
				{
					E->mGrid->Cells[1][1].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
					E->mGrid->Cells[1][6].Highlight=true;
				}
				break;
				case 4://robot s aktivn� oto��
				{
					//validace
					E->mGrid->Cells[1][5].Highlight=true;
					E->mGrid->Cells[1][6].Highlight=true;
				}break;
				case 5://oto� pasivn�
				{
					E->mGrid->Cells[1][3].Highlight=true;
				}break;
				case 6://oto� aktivn�
				{
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
//korelace uvnit� element�
void TFormX::korelace_v_elementech(long ID,long Row)
{
	Cvektory::TElement *E=vrat_element_z_tabulky(ID);
	switch(E->eID)
	{
		case 0:break;//stopka
		case 1: //robot (kontinu�ln�)
		{
			if(Row==1){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			F->PmG->Refresh();//vol�no samostatn�, jen ve dvou case
		} break;
		case 2: //robot se stop stanic�
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3: //robot s pasivn� oto��
		{
			if (Row==1){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==5)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==6){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==7)E->mGrid->Cells[1][Row-1].Highlight=true;
			F->PmG->Refresh();
		} break;
		case 4://robot s aktivn� oto�� (resp. s oto�� a stop stanic�)
		{
			if(Row==1)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==3)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==4)E->mGrid->Cells[1][5].Highlight=true;
			if(Row==6)E->mGrid->Cells[1][5].Highlight=true;
		} break;
		case 5://oto� pasivn�
		{
			if (Row==2)E->mGrid->Cells[1][Row+1].Highlight=true;
		} break;
		case 6://oto� aktivn� (resp. oto� se stop stanic�)
		{
			if (Row==3)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
	}
	E->mGrid->Refresh();
	E=NULL; delete E;
}
//odstran� highlight na v�ech tabulk�ch
void TFormX::odstranit_korelaci(bool predat_focus)
{
	if(predat_focus)
		F->DrawGrid_knihovna->SetFocus();//po kliku mimo z�st�val focus po��d na editu
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
//validace rychlosti p�i jej� zm�n�
void TFormX::validace()
{
	AnsiString jednotky;
	if(F->aRDunit==0)jednotky="[m/s]";
	else jednotky="[m/min]";
	bool mimo_rozmezi=false;
	TscGPComboBox *Combo=F->PmG->getCombo(0,0);
	dopRD=0;
	//kontrola zda je zadan� hodnota v rozmez�
	if(F->m.between(F->pom_temp->pohon->aRD,F->pom_temp->pohon->rychlost_od,F->pom_temp->pohon->rychlost_do)) mimo_rozmezi=false;
	else mimo_rozmezi=true;
	// nutn� o�et�en� pro obdob� zad�v�n�/psan�
	if (F->pom_temp->pohon->aRD > 0)
	{
		//v�po�et doporu�en� rychosti
		if(F->PmG->Rows[7].Visible)//v kabin� jsou 2 r�zn� rotace
		{
			double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
			double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
			if(dopRD1>dopRD2)dopRD=dopRD1;//vyp�e v�t�� hodnotu
			else dopRD=dopRD2;
		}
		else
			dopRD=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,F->pom_temp->rotace,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
		//zadan� rychlost je mimo rozsah
		if(mimo_rozmezi)
		{
			if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
			F->PmG->ShowNote("Rychlost neodpov�d� rozmez�!",clRed,14);
		}
		//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
		if(Combo->ItemIndex!=0 && F->pom_temp->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
			if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
			F->PmG->ShowNote("Zadejte doporu�enou rychlost pohonu: <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,clRed,14);
		}
		//v�e je vpo��dku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
      povolit_zakazat_editaci(true);
			F->PmG->ShowNote("",clRed,14);
		}
	}
	else F->PmG->ShowNote("Neplatn� hodnota rychlosti pohonu!",clRed,14);
	Combo=NULL;delete Combo;
}
//---------------------------------------------------------------------------
//vol�no po kliku na link v pozn�mce, napln� edit aRD doporu�enou rychlost�
void TFormX::naplneni_dopRD()
{
	F->pom_temp->pohon->aRD=dopRD;
	F->PmG->Cells[1][rychlost].Text=F->outaRD(dopRD);
	zmena_aRD();
	//odstranit_korelaci();//pro jistotu z�stavala aktivn� po kliku na link
	F->PmG->ShowNote("",clRed,14);
	povolit_zakazat_editaci(true);
	F->PmG->Refresh();//do�lo ke zm�n� hodnot v tabulce
}
//---------------------------------------------------------------------------
//zakazuje �i povol� komponenty v tabulce pohonu a v�ech tabulk�ch elementu
void TFormX::povolit_zakazat_editaci(bool povolit)
{
	if(povolit)
		{if(F->duvod_ulozit_nahled)F->scGPButton_ulozit->Enabled=true;}//pokud je d�vod k ulo�en�, ale button ulo�it je z p�edchoz�ho kroku neaktivn� zapne ho
	else
		{F->scGPButton_ulozit->Enabled=false;}//pokud je button ulo�it zapnut vypne ho
//	F->PmG->Update();//mus� b�t, p�i vlo�en� prvn�ho kontinu�ln�ho robota probl�m v zobrazen�
//	F->PmG->SetEnabledComponents(povolit);
//	F->PmG->SetEnabledComponent(1,1,true);//rychlost mus� b�t aktivn� aby ji mohl u�ivatel zm�nit a t�m odemknout ostatn� bu�ky
//	if(F->pom_temp->elementy->dalsi!=NULL)
//	{
//		Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//m��u p�esko�it hlavi�ku
//		while(E!=NULL)
//		{
//			E->mGrid->SetEnabledComponents(povolit);
//			E=E->dalsi;
//		}
//		E=NULL;delete E;
//	}
}
//---------------------------------------------------------------------------
//vr�t� ukazatel na element, nelze pou��t vra� element stavaj�c�, z d�vodu �e ID tabulky ji� nemus� b�t toto�n� s n robota
Cvektory::TElement *TFormX::vrat_element_z_tabulky(long ID)
{
	Cvektory::TElement *ret=NULL;
	Cvektory::TElement *E=F->pom_temp->elementy->dalsi;//m��u p�esko�it hlavi�ku, metoda vol�n� po kliku do tabulky elementu
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
