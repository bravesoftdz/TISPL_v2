//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitX.h"
#include "Unit1.h"
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
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
// p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
// v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
	input_state=NO;
	if(ID==9999&&Row>=1&&F->PmG->Rows[3].Visible)//pokud je kliknuto do tabulky pohonu, podle bu�ky vyzn�� bu�ky, kter� budou zm�nou ovlivn�n�
		korelace_tab_pohonu(Row);//spu�t�no pouze v p��pad� KK tabulky
	if(ID==9999&&Row>=1)korelace_tab_pohonu_elementy();//p�i S&G re�imu je st�le nutn� zobrazit korelaci v elementech
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	//funkcionalita exBUTTONu v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabr�ni probliku Hintu, toto slou�ilo pro tla��tko, ale nebylo pln� u�inn�: int T=F->PmG->exBUTTON->Top;
		if(F->PmG->Rows[4].Visible)
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			F->PmG->VisibleRow(4,false,false);//nep�ekreslovat
			F->PmG->VisibleRow(5,false,false);

		}else
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			F->PmG->VisibleRow(4,true,false);//nep�ekreslovat
			F->PmG->VisibleRow(5,true,false);
		}
		F->PmG->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t
		F->PmG->Update();
		F->PmG->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nep�epozival b�hem updatu tam a zp�t, toto nesta�ilo: F->PmG->exBUTTON->Top=T;//zajist�, �e se tla��tko nep�epozicuje
		F->REFRESH();//mus� b�t opravdu REFRESH cel�ho formu nikoliv jen mGridu
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
		F->REFRESH();//mus� b�t opravdu REFRESH cel�ho formu nikoliv jen mGridu
		E->mGrid->exBUTTON->Hint=Hint;//navr�cen� p�vodn�ho textu hintu
		E=NULL; delete E;
	}
	if(ID!=9999)//korelace v tabulk�ch element�
		korelace_v_elementech(ID,Row);
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
}
//---------------------------------------------------------------------------
//zpracov�n� onchange ud�losti - INPUT, v�po�et a OUTPUT zp�t do ovlivn�n� bu�ky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING&&ID!=9999&&vstoupeno_elm)
	{
		Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID);
		if(ID>100000)
		{
			//switch pohon - vypocty
		}
		else
		{
			switch(E->eID)
			{
				case 0: // stop
				{
					if(Col==1 && Row==1)
					{
						input_state=COMBO;
						F->d.v.uloz_sparovany_element(E);
					}
					//Stop stanice m� pouze �asov� �daje, kter� spolu nesouviseji ?? (wt)
				} break;
				case 1: //robot (kontinu�ln�)
				{
					if(Col==1 && Row==1) //vstup PT -> vystup LO
					{
						input_state=PT; //nastaveni stavu
						E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO1 = F->pom_temp->pohon->aRD*E->PT1;
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outLO(E->LO1),3); //OUTPUT
					}
					if(Col==1 && Row==2) //vstup LO -> vystup PT
					{
						input_state=LO; //nastaveni stavu
						E->LO1 = F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1 = E->LO1/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT1),3);  //OUTPUT
					}
				} break;
				case 2: //robot se stop stanic�
				{
					if(Row==1)//zm�na PT
					{
						input_state=PT;
						E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//na�ten� z mgridu
						E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][2].Text=F->m.round2double(F->outPT(E->RT),3);//v�pis do mGridu
					}
					if(Row==3)//zm�na WT
					{
						input_state=WT;
						E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//na�ten� z mgridu
						E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][3].Text=F->m.round2double(F->outPT(E->RT),3);//v�pis do mGridu
					}
				} break;
				case 3: //robot s pasivn� oto��
				{
					if (Row==1)
					{
						input_state=PT;//vstup �as - v�stup lakovac� okno
						E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO1 = F->pom_temp->pohon->aRD*E->PT1;
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outLO(E->LO1),3); //v�stup do bu�ky LO
					}
					if (Row==2)
					{
						input_state=LO;
						E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1=E->LO1/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT1),3);
					}
					if (Row==3)//COMBO
					{
						input_state=COMBO;//�hel oto�e
						switch(E->mGrid->getCombo(1,3)->ItemIndex)
						{
							case 0:E->rotace_jig=-180;break;
							case 1:E->rotace_jig=-90;break;
							case 2:E->rotace_jig=90;break;
							case 3:E->rotace_jig=180;break;
						}
					}
					if (Row==5)//�tvrt� ��dek nen� u�ivatelsky upraviteln�
					{
						input_state=DO;//delka oto�e
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					}
					if (Row==6)
					{
						input_state=PT;
						E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO2=F->pom_temp->pohon->aRD * E->PT2;
						E->mGrid->Cells[Col][Row+1].Text=F->m.round2double(F->outLO(E->LO2),3);
					}
					if (Row==7)
					{
						input_state=LO;
						E->LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT2=E->LO2/F->pom_temp->pohon->aRD; //nahradit aRD
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT2),3);
					}
				} break;
				case 4://robot s aktivn� oto�� (resp. s oto�� a stop stanic�)
				{
					if(Row==1)//zm�na PT1
					{
						input_state=PT;
						E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//na�ten� z mgridu
						E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//v�pis do mGridu
					}
					if (Row==2)//COMBO
					{
						input_state=COMBO;//�hel oto�e
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
						input_state=PTotoc;
						E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//na�ten� z mgridu
						E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//v�pis do mGridu
					}
					if(Row==4)//zm�na PT2
					{
						input_state=PT2;
						E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//na�ten� z mgridu
						E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//v�pis do mGridu
					}
					if(Row==6)//zm�na WT
					{
						input_state=WT;
						E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//na�ten� z mgridu
						E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//v�pis do mGridu
					}
				} break;
				case 5://oto� pasivn�
				{
					if (Row==1)//COMBO
					{
						input_state=COMBO;//�hel oto�e
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
						input_state=DO;//delka oto�e
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					}
				} break;
				case 6://oto� aktivn� (resp. oto� se stop stanic�)
				{
					if (Row==1)//COMBO
					{
						input_state=COMBO;//�hel oto�e
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
						input_state=PTotoc;
						E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->OTOC_delka=F->pom_temp->pohon->aRD*E->PTotoc;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outDO(E->OTOC_delka),3);
					}
				} break;
			}
		}
//		E->mGrid->Update();
		if(E->mGrid->Cells[Col][Row].Text=="")input_state=NOTHING;
		else F->Timer2->Enabled=true;
		E=NULL;delete E;
//		input_state=NOTHING;
	}
	if(input_state==NOTHING&&ID==9999&&vstoupeno_poh)
	{
		switch(Row)
		{
			case 0://v�b�r pohonu
			{
				input_state=COMBO;
				vstoupeno_poh=false;//blokace ud�lost� p�i vkl�d�n� elementu
				F->tab_pohon_COMBO(1);//p�i�azen� pohonu
				F->aktualizace_ComboPohon();
				if(F->PmG->getCombo(0,0)->ItemIndex!=0)
				{
					if(aRD>0)F->scGPComboBox_prepinacKot->Enabled=true;
					F->scGPGlyphButton_PLAY->Enabled=true;//zapnut� tla��tka animace
					//aktualizace tabulky
					if(F->PmG->Rows[3].Visible)//pro tabulku v kontinu�ln�m re�imu
					{
						F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
						F->PmG->Cells[1][2].Text=F->m.round2double(F->outR(F->pom_temp->pohon->roztec),3);
						F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
						F->PmG->Cells[1][3].Text=F->m.round2double(F->pom_temp->pohon->Rx,0);
						F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
						F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
						//m�sto pro mezeru mezi podvozky
						if(F->PmG->Rows[7].Visible)//budou zde ob� mezeri mezi jigy
						{
							F->PmG->Cells[1][6].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1));
							F->PmG->Cells[1][7].Text=F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1));
						}
						else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutn� dal�� kontrola, padaly by sem v�echny varianty
						{
							double uhel=F->d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(F->pom_temp,F->pom_temp->elementy->dalsi);
							F->PmG->Cells[1][6].Text=F->outRz(F->m.mezera(uhel,F->pom_temp->pohon->Rz,1));
						}
					}
					else
					{
						F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
						F->PmG->Cells[1][2].Text=F->m.round2double(F->outR(F->pom_temp->pohon->roztec),3);
					}
					F->PmG->Refresh();
					aktualizace_tab_elementu();
				}
				else
				{
					aktualizace_tab_elementu_pOdebran();
					F->scGPComboBox_prepinacKot->Enabled=false;
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
			case 4://rozestup, Rz   NEBUDE EDITOVATELN�
			break;
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
		}
		FormX->vstoupeno_poh=true;//blokace ud�lost� p�i vkl�d�n� elementu
		if(F->PmG->Cells[Col][Row].Text=="")input_state=NOTHING;
		else F->Timer2->Enabled=true;
	}
	if(vstoupeno_elm||vstoupeno_poh) F->nahled_ulozit(true);
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
   		F->REFRESH();
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
void TFormX::zmena_aRD ()
{
	F->aktualizace_ComboPohon();
	//propo�ty v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK re�imu
	{
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->pom_temp->pohon->Rx,0);
		F->PmG->Cells[1][5].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0));
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
	//propo�ty v tabulk�ch element�
	aktualizace_tab_elementu();
	if(validace_true&&F->PmG->Rows[3].Visible)validace();
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rozte�e
void TFormX::zmena_R ()
{
	//p�epo�et hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK re�imu
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz); //prohozen� po�ad� z d�vodu, �e druh� v�po�et pot�ebuje aktualizovaonu honotu prvn�ho v�po�tu
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->PmG->Cells[1][5].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0));
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
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou Rx
void TFormX::zmena_Rx ()
{
	//p�epo�et hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//kontinu�ln� re�im
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		if(!validace_true)
		{
			F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
			F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		}
		F->PmG->Cells[1][5].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0));
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
void TFormX::aktualizace_tab_elementu ()
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
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
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
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
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
				F->PmG->Cells[1][4].Highlight=true; //rozestup
				F->PmG->Cells[1][3].Highlight=true; //rx
				F->PmG->Cells[1][6].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][7].Highlight=true;
			}
		}break;
		case 2: //zmena rozte�e R
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][4].Highlight=true;  //rozestup
				F->PmG->Cells[1][1].Highlight=true; //aRD
				F->PmG->Cells[1][6].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][7].Highlight=true;
			}
		}break;
		case 3: //zm�na Rx
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][1].Highlight=true; //aRD
				F->PmG->Cells[1][4].Highlight=true; //rozestup Rz
				F->PmG->Cells[1][6].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][7].Highlight=true;
			}
		}break;
	}
	F->PmG->Refresh();
	korelace_tab_pohonu_elementy();
}
//---------------------------------------------------------------------------
//star� se o highlitov�n� pol��ek, kter� souvis� s m�n�nou hodnotou (elementy)
void TFormX::korelace_tab_pohonu_elementy()
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
	Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID);
	switch(E->eID)
	{
		case 0:break;//stopka
		case 1: //robot (kontinu�ln�)
		{
			if(Row==1)E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
		case 2: //robot se stop stanic�
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3: //robot s pasivn� oto��
		{
			if (Row==1)E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==5)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==6)E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==7)E->mGrid->Cells[1][Row-1].Highlight=true;
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
void TFormX::odstranit_korelaci()
{
	F->DrawGrid_knihovna->SetFocus();//po kliku mimo z�st�val focus po��d na editu
	F->PmG->unHighlightAll();
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0)
			E->mGrid->unHighlightAll();
		E=E->dalsi;
	}
	validace_true=false;//tato metoda se vol� p�i kliku mimo formul��, p��hodn� vypnout validaci
}
//---------------------------------------------------------------------------
//validace rychlosti p�i jej� zm�n�
void TFormX::validace()
{
	bool mimo_rozmezi=false;;
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
			F->PmG->ShowNote("Rychlost neodpov�d� rozmez�!",clRed,14);
		}
		//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
		if(Combo->ItemIndex!=0 && F->pom_temp->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
				F->PmG->ShowNote("Zadejte doporu�enou rychlost pohonu: <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a>",clRed,14);
		}
		//v�e je vpo��dku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
			F->PmG->ShowNote("",clRed,14);
		}
	}
	else F->PmG->ShowNote("Neplatn� hodnota rychlosti pohonu!",clRed,14);
}
//---------------------------------------------------------------------------
//vol�no po kliku na link v pozn�mce, napln� edit aRD doporu�enou rychlost�
void TFormX::naplneni_dopRD()
{
	F->PmG->Cells[1][1].Text=F->outaRD(dopRD);
	F->PmG->ShowNote("",clRed,14);
	//odstranit_korelaci();//pro jistotu z�stavala aktivn� po kliku na link
}
//---------------------------------------------------------------------------
