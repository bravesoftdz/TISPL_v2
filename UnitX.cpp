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
		F->REFRESH(true);//mus� b�t opravdu REFRESH() cel�ho formu + mGridu
		F->PmG->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
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
	//funkcionalita exBUTTONu v tabulk�ch element�
	{
		Cvektory::TElement *E=vrat_element_z_tabulky(ID);
		AnsiString Hint=E->mGrid->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		switch(E->eID)
		{
			case 0://stop stanice
			{
      	if(E->mGrid->Rows[3].Visible)
	    	{
	    		E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
	    		E->mGrid->VisibleRow(3,false,false);//nep�ekreslovat
					E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);
	    	}
	    	else
	    	{
					double rotaceJ=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E);
					if(E->mGrid->Cells[1][6].Text==1 && (F->m.Rt90(rotaceJ)==0 || F->m.Rt90(rotaceJ)==180) && F->d.v.PP.delka_podvozek<F->m.UDJ(rotaceJ))
					E->mGrid->Note.Text="P�i 0� �i 180� rotaci jigu a po�tu nastaven�ch pozic 1 nejsou zobrazeny potenci�ln� pozice";
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
	    		E->mGrid->VisibleRow(3,true,false);//nep�ekreslovat
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
		E->mGrid->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t
		E->mGrid->Update();
		E->mGrid->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t, toto nesta�ilo: F->PmG->exBUTTON->Top=T;//zajist�, �e se tla��tko nep�epozicuje
		F->REFRESH(true);//mus� b�t opravdu REFRESH cel�ho formu + mGridu
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
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
						//nutn� o�et�en� podm�nkou, v p��pad� editace textu, kter� je cel� ozna�en� ud�lost OnChange spu�t�na 2x
			  		zmena_aRD(E);
						F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
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
			  		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
			  		//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			  		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
			  		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
			  		E->data.PT2=F->m.PT(E->data.LO2,F->OBJEKT_akt->pohon->aRD);
						E->mGrid->Cells[1][8].Text=F->m.round2double(F->outPT(E->data.PT2),3);
			  		//zm�na v ostatn�ch elementech
			  		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
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
					switch(E->mGrid->getCombo(1,3)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
					//aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[1][6].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][7].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//p�epo��t�n� z�n ot��en� n�sleduj�c�ch element�
					F->pridani_elementu_tab_pohon(E);//pouze v KK re�imu
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
					E->mGrid->Cells[1][6].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][7].Text=F->outDO(F->m.round2double(E->zona_za,3));
				}
				if (Row==8)//editace PT2
				{
					input_state=PT;//nastaveni stavu
					E->data.PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
					//o�et�en� proti d�len� 0 a proti spou�t�n� p�i pr�zdn�m �et�zci v EDITu
					if(E->data.PT2!=0)
					{
			   		F->OBJEKT_akt->pohon->aRD=E->data.LO2/E->data.PT2;//ulo�en� do pam�ti + v�po�et
			   		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);//OUTPUT
			   		//zm�na aRD vyvol� n�sleduj�c� p�epo�ty
			   		E->data.PT1=F->m.PT(E->data.LO1,F->OBJEKT_akt->pohon->aRD);
			   		E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
			   		E->PTotoc=F->m.PT(E->OTOC_delka,F->OBJEKT_akt->pohon->aRD);
			   		E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
						//zm�na v ostatn�ch elementech
			   		if(E->mGrid->Cells[Col][Row].Text!="")zmena_aRD(E);
						F->PmG->Refresh();//do�lo ke zm�n� hodnot v PmG
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
					E->data.PD=E->mGrid->getCombo(1,Row)->ItemIndex;//pouze ulo�en� do dat
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
					switch(E->mGrid->getCombo(1,1)->ItemIndex)
					{
						case 0:E->rotace_jig=-180;break;
						case 1:E->rotace_jig=-90;break;
						case 2:E->rotace_jig=90;break;
						case 3:E->rotace_jig=180;break;
					}
					//aktualizace z�n ot��en�
					TPointD zony_otaceni=F->m.zona_otaceni(F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
					E->zona_pred=zony_otaceni.x;E->zona_za=zony_otaceni.y;
					E->mGrid->Cells[1][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
					aktualizace_zon_otaceni(E);//p�epo��t�n� z�n ot��en� n�sleduj�c�ch element�
					if(F->OBJEKT_akt->rezim==1)F->pridani_elementu_tab_pohon(E);//pouze v KK re�imu, pasivn� oto� m��e b�t i ve S&G lakovn�
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
					E->mGrid->Cells[1][4].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][5].Text=F->outDO(F->m.round2double(E->zona_za,3));
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
						if(E->eID==300)//pro v�hybku
						{
					  	if(Col==1)prirazeni_pohohonu_vetvi(E);//hlavn� v�tev
					  	else prirazeni_pohohonu_vetvi(E,false);//vedlej�� v�tev
						}
						else prirazeni_pohohonu_PM(E,Col);
						break;
					}
					//zm�na aRD
					case 3:
					{
						input_state=aRD;
						//na�ten� aRD z editu
						double aRD=F->inaRD(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//ur�en� c�lov�ho pohonu
						Cvektory::TElement *e_pom=E;
						if(Col==2 && E->eID==300)e_pom=E->dalsi2;
						if(Col==2 && E->eID==200)
						{
							e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
						}
						//ulo�en� do pohonu
						F->OBJEKT_akt->pohon=e_pom->pohon;//ulo�en� aktu�ln� editovan�ho pohonu
						e_pom->pohon->aRD=aRD;
						//v�po�et ovlivn�n�ch dat
						e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->aRD);
						e_pom->pohon->Rx=F->m.Rx(e_pom->pohon->aRD,e_pom->pohon->roztec);
						if(Col==2)E->WT=F->m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);//pouze pokud jde o dal�� pohon
						e_pom=NULL;delete e_pom;
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
					case 5:
					{
            input_state=R;
						//na�ten� R z editu
						double R=F->inR(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						//ur�en� c�lov�ho pohonu
						Cvektory::TElement *e_pom=E;
						if(Col==2 && E->eID==300)e_pom=E->dalsi2;
						if(Col==2 && E->eID==200)
						{
							e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
						}
						//ulo�en� do pohonu
						F->OBJEKT_akt->pohon=e_pom->pohon;
						e_pom->pohon->roztec=R;
						//v�po�et ovlivn�n�ch dat
						e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->Rx,e_pom->pohon->roztec);
						e_pom->pohon->aRD=F->m.RD(e_pom->pohon->Rz);
						if(Col==2)E->WT=F->m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);//pouze pokud jde o dal�� pohon
						e_pom=NULL;delete e_pom;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
					//zm�na n�sobku rozte�e
					case 6:
					{
            input_state=Rx;
			    	//na�ten� Rx z editu
						double Rx=F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text);
			    	//ur�en� c�lov�ho pohonu
						Cvektory::TElement *e_pom=E;
						if(Col==2 && E->eID==300)e_pom=E->dalsi2;
						if(Col==2 && E->eID==200)
						{
							e_pom=E->dalsi;
							if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;
						}
						//ulo�en� do pohonu
						F->OBJEKT_akt->pohon=e_pom->pohon;
						e_pom->pohon->Rx=Rx;
						//v�po�et ovlivn�n�ch dat
						e_pom->pohon->Rz=F->m.Rz(e_pom->pohon->Rx,e_pom->pohon->roztec);
						e_pom->pohon->aRD=F->m.RD(e_pom->pohon->Rz);
						if(Col==2)E->WT=F->m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);//pouze pokud jde o dal�� pohon
						e_pom=NULL;delete e_pom;
						//update dat tabulek
						update_hodnot_vyhybky_PM(E);//provede zapis do tabulky
						aktualizace_tab_elementu(E);
						break;
					}
				}
			} break;
		}
		E->mGrid->Refresh();//refresh aktu�ln� upravovan� tabulky
		posledni_E=E;//ulo�en� posledn�ho editovan�ho elementu
		posledni_c=c;//ulo�en� posledn�ho editovan�ho segmentu cesty
		if(F->d.v.ZAKAZKA_akt!=NULL && F->d.v.ZAKAZKA_akt->n!=0 && c!=NULL)c->data=E->data;//navr�cen� p�epo��tan�ch dat do zak�zky
		E=NULL;delete E;
		c=NULL;delete c;
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
					//aktualizace tabulek
					aktualizace_PmG();
					aktualizace_tab_elementu();//do�lo ke zm�n�m v tabulce pohonu, kter� ovlivn� i ostatn� elementy
				}
				else//pohon byl odebr�n
				{
					F->scGPComboBox_prepinacKot->Enabled=false;//vypne zm�nu re�imu k�t
					F->scGPComboBox_prepinacKot->ItemIndex=0;
					F->scGPGlyphButton_PLAY->Enabled=false;//vypnut� tla��tka animace
					aktualizace_tab_elementu_pOdebran();//aktualizace tabulek elemnt�
				}
			}break;
			case 1://aktu�ln� rychlost, aRD
			{
				input_state=aRD;
				//na�ten� aRD z editu
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
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
			case 2://rozte�, R
			{
				input_state=R;
				//na�ten� R z editu
				double R=F->inR(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
				//ulo�en� do spoj�ku
				F->OBJEKT_akt->pohon->roztec=R;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
				zmena_R();
			}break;
			case 3://Rx
			{
				input_state=Rx;
				//na�ten� Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[1][Row].Text);
				//ulo�en� do spoj�ku
				F->OBJEKT_akt->pohon->Rx=Rx;
				//vol�n� metody pro p�epo�ty v�ech souvisej�c�ch bun�k
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
			F->REFRESH(true);
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
void TFormX::zmena_aRD (Cvektory::TElement *mimo_element)
{
	F->aktualizace_ComboPohon();
	//propo�ty v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK re�imu
	{
		F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->OBJEKT_akt->pohon->Rx,3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		if(F->Akce==F->NIC)validace_aRD();//validace pouze v kontinu�ln�m re�imu kabiny
	}
	else if(F->Akce==F->NIC)validace_aRD(true);//kontrola pouze zda se rychlost nach�z� v rozmez�
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
    F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz); //prohozen� po�ad� z d�vodu, �e druh� v�po�et pot�ebuje aktualizovaonu honotu prvn�ho v�po�tu
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
	}
	//p�epo�et hodnot v elementech
	F->aktualizace_ComboPohon();//zm�na rychlosti, rychlost je zobrazen� v Combo pohon�
	aktualizace_tab_elementu();
	//////test validace rozte�e                                                                                                                            //0 = z�kladn� jednotky
//	TTextNumber cislo=F->d.v.rVALIDACE(5,F->OBJEKT_akt->pohon->n,F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec,F->OBJEKT_akt->pohon->Rz,F->OBJEKT_akt->pohon->Rx,0,0,0);//F->aRDunit,F->Runit,F->Rzunit);
//	F->PmG->ShowNote(cislo.text);
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou Rx
void TFormX::zmena_Rx ()
{
	//p�epo�et hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//kontinu�ln� re�im
	{
    F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->Rx,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		F->OBJEKT_akt->pohon->aRD=F->m.RD(F->OBJEKT_akt->pohon->Rz);
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][mezera_podvozky].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,0)),3);
		if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1)),3);
			F->PmG->Cells[1][7].Text=F->m.round2double(F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
		else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
		{
			double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->OBJEKT_akt->element);
			F->PmG->Cells[1][6].Text=F->m.round2double(F->outRz(F->m.mezera(uhel,F->OBJEKT_akt->pohon->Rz,1)),3);
		}
	}
	//p�epo�et hodnot v elementech
  F->aktualizace_ComboPohon();//zm�na rychlosti, rychlost je zobrazen� v Combo pohon�
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//aktualizuje hodnoty v PmG
void TFormX::aktualizace_PmG(bool Refresh)
{
  if(F->PmG->Rows[3].Visible)//pro tabulku v kontinu�ln�m re�imu
	{
		F->PmG->Cells[1][rychlost].Text=F->m.round2double(F->outaRD(F->OBJEKT_akt->pohon->aRD),3);
		F->PmG->Cells[1][roztec_palce].Text=F->m.round2double(F->outR(F->OBJEKT_akt->pohon->roztec),3);
		F->OBJEKT_akt->pohon->Rx=F->m.Rx(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->roztec);
		F->PmG->Cells[1][nasobek_roztece].Text=F->m.round2double(F->OBJEKT_akt->pohon->Rx,0);
		F->OBJEKT_akt->pohon->Rz=F->m.Rz(F->OBJEKT_akt->pohon->aRD);
		F->PmG->Cells[1][roztec_jigu].Text=F->m.round2double(F->outRz(F->OBJEKT_akt->pohon->Rz),3);
		//m�sto pro mezeru mezi podvozky
		if(F->PmG->Rows[mezera_jig2].Visible)//budou zde ob� mezeri mezi jigy
		{
			F->PmG->Cells[1][mezera_jig1].Text=F->outRz(F->m.mezera(0,F->OBJEKT_akt->pohon->Rz,1));
			F->PmG->Cells[1][mezera_jig2].Text=F->outRz(F->m.mezera(90,F->OBJEKT_akt->pohon->Rz,1));
		}
		else if(F->PmG->Rows[mezera_jig1].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
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
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->WT),3);
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
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->data.PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,aRD);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->data.PT2=F->m.PT(E->data.LO2,aRD);
					E->mGrid->Cells[1][8].Text=F->m.round2double(F->outPT(E->data.PT2),3);
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
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
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
					E->mGrid->Cells[1][3].Text=0;
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
					E->mGrid->Cells[1][1].Text=0;
					E->mGrid->Cells[1][4].Text=0;
					E->mGrid->Cells[1][8].Text=0;
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
					E->mGrid->Cells[1][3].Text=0;
				}break;
				case 6://oto� aktivn�
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
					E->mGrid->Cells[1][1].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
					E->mGrid->Cells[1][8].Highlight=true;
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
					E->mGrid->Cells[1][3].Highlight=true;
				}break;
				case 6://oto� aktivn�
				{
					//E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}break;
				case 200:
				case 300:
				{
					if(mimo_element!=NULL)
					{
						//zji�t�n�, zda m� element stejn� pohon jako upravovan� pohon, pokud ano zjist� v jak�m je sloupci
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
						//pokud byl zji�t�n sloupec, pokra�uje
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
//korelace uvnit� element�
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
		case 1:case 7:case 11:case 15:case 101:case 105: //robot (kontinu�ln�)
		{
			if(Row==1){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			F->PmG->Refresh();//vol�no samostatn�, jen ve dvou case
		} break;
		case 2:case 8:case 12:case 16:case 102:case 106: //robot se stop stanic�
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3:case 9:case 13:case 17:case 103:case 107: //robot s pasivn� oto��
		{
			if (Row==1){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==5){E->mGrid->Cells[1][Row-1].Highlight=true;E->mGrid->Cells[1][Row+1].Highlight=true;E->mGrid->Cells[1][Row+2].Highlight=true;}
			if (Row==8){F->PmG->Cells[1][rychlost].Highlight=true;korelace_tab_pohonu(rychlost);korelace_tab_pohonu_elementy();}//E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==9)E->mGrid->Cells[1][Row-1].Highlight=true;
			F->PmG->Refresh();
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
			if (Row==2){E->mGrid->Cells[1][Row+1].Highlight=true;E->mGrid->Cells[1][Row+2].Highlight=true;E->mGrid->Cells[1][Row+3].Highlight=true;}
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
			korelace_tab_pohonu_elementy(E);//ozna�en� v ostatn�ch tabulk�ch
			break;
    }
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
	AnsiString jednotky;
	if(F->aRDunit==0)jednotky="[m/s]";
	else jednotky="[m/min]";
	bool mimo_rozmezi=false;
	TscGPComboBox *Combo=F->PmG->getCombo(0,0);
	dopRD=0;
	//kontrola zda je zadan� hodnota v rozmez�
	if(F->m.between(F->OBJEKT_akt->pohon->aRD,F->OBJEKT_akt->pohon->rychlost_od,F->OBJEKT_akt->pohon->rychlost_do)) mimo_rozmezi=false;
	else mimo_rozmezi=true;
	//zadan� rychlost je mimo rozsah
	if(mimo_rozmezi && F->OBJEKT_akt->pohon->aRD > 0)
	{
		if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
		F->PmG->ShowNote(F->ls->Strings[220],F->d.clError,14);//"Rychlost neodpov�d� rozmez�!"
		povolit_zakazat_editaci(false);
	}
	if(!mimo_rozmezi && F->PmG->Note.Text!=""){F->PmG->ShowNote("",F->d.clError,14);povolit_zakazat_editaci(true);}
	// nutn� o�et�en� pro obdob� zad�v�n�/psan�
	if (F->OBJEKT_akt->pohon->aRD > 0 && !pouze_rozmezi)
	{
		//v�po�et doporu�en� rychosti
		if(F->PmG->Rows[7].Visible)//v kabin� jsou 2 r�zn� rotace
		{
			double dopRD1=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,0,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
			double dopRD2=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,90,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
			if(dopRD1>dopRD2)dopRD=dopRD1;//vyp�e v�t�� hodnotu
			else dopRD=dopRD2;
		}
		else
			dopRD=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,F->OBJEKT_akt->rotace,F->OBJEKT_akt->pohon->roztec,F->d.v.PP.TT,F->OBJEKT_akt->pohon->aRD);
		//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
		if(Combo->ItemIndex!=0 && F->OBJEKT_akt->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
		{
			if(F->PmG->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
			F->PmG->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporu�enou rychlost pohonu:"
		}
		//v�e je vpo��dku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->OBJEKT_akt->pohon->aRD) && mimo_rozmezi==false)
		{
			povolit_zakazat_editaci(true);
			F->PmG->ShowNote("",F->d.clError,14);
		}
	}
	else if(!pouze_rozmezi)F->PmG->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatn� hodnota rychlosti pohonu!"
	Combo=NULL;delete Combo;
}
//---------------------------------------------------------------------------
//validace maxim�ln�ho po�tu voz�k� na stopce
void TFormX::validace_max_voziku()
{
	if(posledni_E!=NULL && F->OBJEKT_akt!=NULL && posledni_E->objekt_n!=F->OBJEKT_akt->n)posledni_E=NULL;//pro p��pad, �e se zm�nil n�hled
	if(posledni_E!=NULL && posledni_E->eID==0)
	{
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
			double rotace=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(posledni_E);
			if(rotace==0 || rotace==180)
			{
				validace=false;
				posledni_E->mGrid->ShowNote(F->ls->Strings[426]+" <a>"+AnsiString(1)+"</a>");//"Kv�li p�ekryvu jig� nelze nastavit v�t�� po�et voz�k� ne�"
			}
		}
		////p�eps�n� maxim�ln�ho po�ctu voz�ku do tabulky elementu, pro jistotu
		posledni_E->mGrid->Cells[1][5].Text=posledni_E->data.pocet_pozic;
		////nemo�nost ulo�it p�i chybn�ch hodnot�ch
		if(validace && F->duvod_ulozit_nahled && !F->scGPButton_ulozit->Enabled)F->nahled_ulozit(true);
		if(!validace)F->nahled_ulozit(false);
		if(validace && posledni_E->mGrid->Note.Text!="")posledni_E->mGrid->Note.Text="";
	}
}
//---------------------------------------------------------------------------
//vol�no po kliku na link v pozn�mce, napln� edit aRD doporu�enou rychlost�
void TFormX::naplneni_dopRD()
{
	F->OBJEKT_akt->pohon->aRD=dopRD;
	F->PmG->Cells[1][rychlost].Text=F->outaRD(dopRD);
	zmena_aRD();
	//odstranit_korelaci();//pro jistotu z�stavala aktivn� po kliku na link
	F->PmG->ShowNote("",F->d.clError,14);
	povolit_zakazat_editaci(true);
	F->Akce=F->BLOK;
	F->PmG->Refresh();//do�lo ke zm�n� hodnot v tabulce
}
//---------------------------------------------------------------------------
//dopln� doporu�en� po�et maxim�ln�ch voz�ku po kliku
bool TFormX::naplneni_max_voziku(double X,double Y,bool check_for_highlight)
{
	bool ret=false;
	if(F->d.v.vrat_posledni_element_objektu(F->OBJEKT_akt)->n>1)
	{
		//hled�n� zda m� n�kter� element nedokon�enou validaci
		Cvektory::TElement *E=F->OBJEKT_akt->element;
		while(E!=NULL && E->objekt_n==F->OBJEKT_akt->n)
		{
			//hled�n� elementu, kter�mu bylo kliknuto na doporu�en� po�et voz�k�
			if(E->eID==0 && E->mGrid!=NULL && E->mGrid->Note.Text!="" && E->mGrid->CheckLink(X,Y)==TPoint(-2,-2)){ret=true;break;}
			E=E->dalsi;
		}
		//napln�n� doporu�en�ho max. po�tu voz�k�
		if(ret && E!=NULL && !check_for_highlight)
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
			E->mGrid->Cells[1][6].Text=E->data.pocet_voziku;
			E->mGrid->ShowNote("");
			F->Akce=F->BLOK;
		}
		E=NULL;delete E;
	}
	return ret;
}
//---------------------------------------------------------------------------
//zakazuje �i povol� komponenty v tabulce pohonu a v�ech tabulk�ch elementu
void TFormX::povolit_zakazat_editaci(bool povolit)
{
//	if(povolit)
//		{F->scGPButton_ulozit->Enabled=true;}//pokud je d�vod k ulo�en�, ale button ulo�it je z p�edchoz�ho kroku neaktivn� zapne ho
//	else
//		{F->scGPButton_ulozit->Enabled=false;}//pokud je button ulo�it zapnut vypne ho
//	F->PmG->Update();//mus� b�t, p�i vlo�en� prvn�ho kontinu�ln�ho robota probl�m v zobrazen�
//	F->PmG->SetEnabledComponents(povolit);
//	F->PmG->SetEnabledComponent(1,1,true);//rychlost mus� b�t aktivn� aby ji mohl u�ivatel zm�nit a t�m odemknout ostatn� bu�ky
//	if(F->OBJEKT_akt->elementy->dalsi!=NULL)
//	{
//		Cvektory::TElement *E=F->OBJEKT_akt->elementy->dalsi;//m��u p�esko�it hlavi�ku
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
					int row=6;if(E->eID==5)row=4;//p�ep�n�n� od kter�ho ��dku za��t, r�zn� elementy
					E->mGrid->Cells[1][row].Text=F->outDO(F->m.round2double(E->zona_pred,3));
					E->mGrid->Cells[1][row+1].Text=F->outDO(F->m.round2double(E->zona_za,3));//refresh mGridu nen� t�eba
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
void TFormX::prirazeni_pohohonu_vetvi(Cvektory::TElement *E,bool hlavni)
{
	////deklarace pot�ebn�ch prom�nn�ch
	TscGPComboBox *Combo;
	if(hlavni)Combo=E->mGrid->getCombo(1,2);
	else Combo=E->mGrid->getCombo(2,2);
	Cvektory::TPohon *p=NULL;
	if(Combo->ItemIndex!=0)p=F->d.v.vrat_pohon(Combo->ItemIndex);
	Cvektory::TElement *e=NULL;

  ////ulo�en� aktu�ln� editovan�ho pohonu
	F->OBJEKT_akt->pohon=p;
	////zm�na pohonu na hlavn� v�tvi
	if(hlavni)
	{
		E->pohon=p;
		F->OBJEKT_akt->pohon=p;
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(e->eID==200)break;//pokud naraz�m na PM zm�n�m mu pohon a skon��m pr�chod
			e=e->dalsi;
		}
		e=E->predchozi;
		while(e!=NULL && e->n>0)
		{
			if(e->eID==200 || F->predchozi_PM==NULL && e->objekt_n==F->OBJEKT_akt->n)break;//pokud naraz�m na PM NEzm�n�m! mu pohon a skon��m pr�chod, nebo naraz�m na konec objektu
			e->pohon=p;
			e=e->predchozi;
		}
	}
	////zm�na pohonu na vedlej�� v�tvi
	else
	{
  	e=E->dalsi2;//p�i�azuji pohon z v�hybky
		while(e!=NULL && e->idetifikator_vyhybka_spojka!=E->idetifikator_vyhybka_spojka)
		{
			e->pohon=p;
			e=e->dalsi;
		}
		//WT �e�� fce. vlo�en� PM
	}

	////na�ten� dat z pohonu do mGridu
	update_hodnot_vyhybky_PM(E);
	vstoupeno_elm=false;//zamezen�, aby p�i aktualizace comba do�lo k propadnut� do onchange
	//F->zmena_editovanych_bunek(E);//aktualizuje combo a p�ep�n� bu�ky na edit �i draw
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT v�em element�
	vstoupeno_elm=true;//navr�cen� stavu

	////ukazatelov� z�le�itosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
//zobraz� aktu�ln� hodnoty z dat v tabulk�ch
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
	if(E->eID==300)//pro v�hybku
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
		if(e_pom==NULL)e_pom=F->d.v.ELEMENTY->dalsi;//v p��pad� PM na konci linky
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
	vstoupeno_elm=false;//blokace cyklen�
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

		//na�ten� pohon�
		Cvektory::TPohon *p=NULL,*p1=NULL,*p2=NULL;
  	if(p1_n!=0)p1=F->d.v.vrat_pohon(p1_n);
		if(p2_n!=0)p2=F->d.v.vrat_pohon(p2_n);

		//dvojt� validace
    //pokud nalezne probl�m zastav� se a zobraz�ho, i v p��pad�, �e je probl�m� v�ce, a� bude probl�m vy�e�en prob�hne validace zda neexistuje dal�� probl�m
		if(F->OBJEKT_akt->rezim==1)//kontrola zda je KK re�im, pokud ano validovat
		{
	  	for(unsigned int i=1;i<=2;i++)
			{
	  		//na�ten� po�adovan�ho pohonu pro validaci
	  		if(i==1)p=p1;
	  		if(i==2)p=p2;
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
						povolit_zakazat_editaci(false);//zak�e btn ulo�it v editaci
						validovany_pohon=i;//ulo�en� pro kter� pohon plat� validace
						break;//byl nalezen probl�m, zastaven� validace, lze zobrazit jen jeden probl�m v Note
	     		}
					if(!mimo_rozmezi && E->mGrid->Note.Text!="")
					{
						E->mGrid->ShowNote("",F->d.clError,14);
						povolit_zakazat_editaci(true);
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
							if(E->mGrid->Note.Text=="")povolit_zakazat_editaci(false);//o�et�eno podm�nkou proti op�tovn�mu spou�t�n�
							E->mGrid->ShowNote(F->ls->Strings[221]+" <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a> "+jednotky,F->d.clError,14);//"Zadejte doporu�enou rychlost pohonu:"
							validovany_pohon=i;//ulo�en� pro kter� pohon plat� validace
							break;//byl nalezen probl�m, zastaven� validace, lze zobrazit jen jeden probl�m v Note
						}
	     			//v�e je vpo��dku
						if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(p->aRD) && mimo_rozmezi==false)
	     			{
	     				povolit_zakazat_editaci(true);
							E->mGrid->ShowNote("",F->d.clError,14);
						}
	     		}
					else E->mGrid->ShowNote(F->ls->Strings[222],F->d.clError,14);//"Neplatn� hodnota rychlosti pohonu!"
				}
			}
		}

		//roznesen� validace na ostatn�
		if(puv_Note!=E->mGrid->Note.Text)//do�lo ke zm�n� note
		{
			puv_Note=E->mGrid->Note.Text;
			if(F->predchozi_PM!=NULL && F->predchozi_PM!=E && F->predchozi_PM->mGrid->getCombo(1,2)->ItemIndex==validovany_pohon || F->predchozi_PM->mGrid->getCombo(2,2)->ItemIndex==validovany_pohon)//pokud m� PM stejn� pohon jako validovan� pohon
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

  	//ukazatelov� z�le�itosti
		p=NULL;p1=NULL;p2=NULL;
		delete p;delete p1;delete p2;
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
	Cvektory::TElement *e=NULL;

	////ulo�en� aktu�ln� editovan�ho pohonu
	F->OBJEKT_akt->pohon=p;
	
	////p�i�azen� pohonu p�ed PM
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
		if(e!=NULL && e->n>0 && e->eID==200)update_hodnot_vyhybky_PM(e);//skon�il jsem p�i�azov�n� pohonu na PM, aktualizace spojky
		//do�el jsem na za��tek ob�ktu a m�m v p�edchoz�m PM
		if(e!=NULL && e->n>0 && (e==F->OBJEKT_akt->element || e==F->OBJEKT_akt->element->predchozi) && F->predchozi_PM!=NULL)
		{
			e=F->predchozi_PM->dalsi;
			while(e!=NULL && e!=F->OBJEKT_akt->element)
			{
				e->pohon=p;
				e=e->dalsi;
			}
			update_hodnot_vyhybky_PM(F->predchozi_PM);//aktualizace hodnot na p�edchoz�m PM
		}
	}
	////p�i�azen� pohonu za PM
	if(Col==2)
	{
		e=E->dalsi;
		while(e!=NULL && e->objekt_n==F->OBJEKT_akt->n)
		{
			e->pohon=p;
			if(e->eID==200)break;//narazil jsem na PM, zapsat nov� pohon a konec
			else e=e->predchozi;
		}
		if(e!=NULL && e->eID==200)update_hodnot_vyhybky_PM(e);//skon�il jsem na p�ed�vac�m m�st�, update tohoto PM
		if(E->dalsi!=NULL && E->dalsi->objekt_n==E->objekt_n)E->WT=F->m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
	}

	////na�ten� dat z pohonu do mGridu
	update_hodnot_vyhybky_PM(E);
	vstoupeno_elm=false;//zamezen�, aby p�i aktualizace comba do�lo k propadnut� do onchange
	//F->zmena_editovanych_bunek(E);//aktualizuje combo a p�ep�n� bu�ky na edit �i draw
	F->vlozit_predavaci_misto_aktualizuj_WT();//provede i aktualizaci WT v�em element�
	vstoupeno_elm=true;//navr�cen� stavu

	////ukazatelov� z�le�itosti
	Combo=NULL;delete Combo;
	p=NULL;delete p;
	e=NULL;delete e;
}
//---------------------------------------------------------------------------
