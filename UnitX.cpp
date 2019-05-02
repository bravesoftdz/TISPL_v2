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
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
// p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
// v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
	unhighlight_tabulky();
	if(ID==9999&&Row>=1)
		highlight_tab_pohonu(Row);
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
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
					//obsahuje 2 �asov� �daje jeden zad�van� u�ivatelm druh� z�sk�van� z pohonu (wt)
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
					if (Row==4)//t�et� ��dek nen� u�ivatelsky upraviteln�
					{
						input_state=DO;//delka oto�e
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					}
					if (Row==5)
					{
						input_state=PT;
						E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO2=F->pom_temp->pohon->aRD * E->PT2;
						E->mGrid->Cells[Col][Row+1].Text=F->m.round2double(F->outLO(E->LO2),3);
					}
					if (Row==6)
					{
						input_state=LO;
						E->LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT2=E->LO2/F->pom_temp->pohon->aRD; //nahradit aRD
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT2),3);
					}
				} break;
				case 4://robot s aktivn� oto�� (resp. s oto�� a stop stanic�)
				{
					//obsahuje pouze �asov� bu�ky + wt
				} break;
				case 5://oto� pasivn�
				{
					if (Row==1)//zde se upravuje pouze d�lka
					{
						input_state=DO;//delka oto�e
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					}
				} break;
				case 6://oto� aktivn� (resp. oto� se stop stanic�)
				{
					if (Row==2)
					{
						input_state=PTotoc;
						E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->OTOC_delka=F->pom_temp->pohon->aRD*E->PTotoc;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outDO(E->OTOC_delka),3);
					}
				} break;
			}
		}
		if(E->mGrid->Cells[Col][Row].Text=="")input_state=NOTHING;
		else F->Timer2->Enabled=true;
		E=NULL;delete E;
	}
	if(input_state==NOTHING&&ID==9999&&vstoupeno_poh)
	{
		switch(Row)
		{
			case 0://v�b�r pohonu
			{
				input_state=COMBO;
				F->tab_pohon_COMBO(1);//p�i�azen� pohonu
				F->aktualizace_ComboPohon();
				if(F->PmG->getCombo(0,0)->ItemIndex!=0)
					aktualizace_tab_elementu();
				else aktualizace_tab_elementu_pOdebran();
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
			case 3://rozestup, Rz   NEBUDE EDITOVATELN�
			break;
			case 4://Rx
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
		if(F->PmG->Cells[Col][Row].Text=="")input_state=NOTHING;
		else F->Timer2->Enabled=true;
	}
	if(vstoupeno_elm||vstoupeno_poh) F->nahled_ulozit(true);
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rychlosti
void TFormX::zmena_aRD ()
{
	F->aktualizace_ComboPohon();
	//propo�ty v tabulce pohonu
	if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
	{
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->pom_temp->pohon->Rx,3);
	}
	//propo�ty v tabulk�ch element�
	aktualizace_tab_elementu();validace();
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou rozte�e
void TFormX::zmena_R ()
{
	//p�epo�et hodnot v tabulce pohonu
	if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz); //prohozen� po�ad� z d�vodu, �e druh� v�po�et pot�ebuje aktualizovaonu honotu prvn�ho v�po�tu
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
	}
	//p�epo�et hodnot v elementech
	F->aktualizace_ComboPohon();
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//p�epo�ty tabulek element� a pohonu vyvolan� zm�nou Rx
void TFormX::zmena_Rx ()
{
  //p�epo�et hodnot v tabulce pohonu
	if(F->PmG->RowCount>3)//kontinu�ln� re�im
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
	}
	//p�epo�et hodnot v elementech
  F->aktualizace_ComboPohon();
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
				case 0:break;//stop stanice
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
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->PT2=F->m.PT(E->LO2,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(E->PT2),3);
				}
				break;
				case 4://robot s aktivn� oto��
 				{
					//validace
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
					E->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(E->PT1+E->PT2,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->RT),3);
				}break;
 				case 5://oto� pasivn�
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][2].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://oto� aktivn�
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][2].Text = F->m.round2double(F->outPT(E->PTotoc),3);
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
				case 0:break;//stop stanice
				case 1://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Text=0;
				}
				break;
				case 2://robot se stop stanic�
				{
					E->mGrid->Cells[1][3].Text=0;
					E->mGrid->Cells[1][2].Text=0;
				}
				break;
				case 3://robot s pasivn� oto��
				{
					E->mGrid->Cells[1][1].Text=0;
					E->mGrid->Cells[1][3].Text=0;
					E->mGrid->Cells[1][5].Text=0;
				}
				break;
				case 4://robot s aktivn� oto��
				{
					E->mGrid->Cells[1][5].Text=0;
					E->mGrid->Cells[1][4].Text=0;
				}break;
 				case 5://oto� pasivn�
				{
					E->mGrid->Cells[1][2].Text=0;
				}break;
				case 6://oto� aktivn�
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
void TFormX::highlight_tab_pohonu(int Row)
{
	switch(Row)
	{
		case 1: //zmena aRD
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][3].Highlight=true; //rozestup
				F->PmG->Cells[1][4].Highlight=true; //rx
			}
		}break;
		case 2: //zmena rozte�e R
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][3].Highlight=true;  //rozestup
				F->PmG->Cells[1][1].Highlight=true; //aRD
			}
		}break;
		case 4: //zm�na Rx
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G re�imu
			{
				F->PmG->Cells[1][1].Highlight=true; //aRD
				F->PmG->Cells[1][3].Highlight=true; //rozestup Rz
			}
		}break;
	}
	F->PmG->Refresh();
	highlight_tab_elementu();
}
//---------------------------------------------------------------------------
//star� se o highlitov�n� pol��ek, kter� souvis� s m�n�nou hodnotou (elementy)
void TFormX::highlight_tab_elementu()
{
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0)
		{
			switch(E->eID)
			{
				case 0:break;//stop stanice
				case 1://robor kontinu�ln�
				{
					E->mGrid->Cells[1][1].Highlight=true;
				}
				break;
				case 2://robot se stop stanic�
				{
					//validace
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 3://robot s pasivn� oto��
				{
					E->mGrid->Cells[1][1].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][5].Highlight=true;
				}
				break;
				case 4://robot s aktivn� oto��
 				{
					//validace
					E->mGrid->Cells[1][5].Highlight=true;
				}break;
 				case 5://oto� pasivn�
				{
					E->mGrid->Cells[1][2].Highlight=true;
				}break;
				case 6://oto� aktivn�
				{
					E->mGrid->Cells[1][2].Highlight=true;
				}break;
			}
			E->mGrid->Refresh();
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//odstran� highlight na v�ech tabulk�ch
void TFormX::unhighlight_tabulky()
{
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
void TFormX::validace ()
{
	bool mimo_rozmezi=false;;
	TscGPComboBox *Combo=F->PmG->getCombo(0,0);
	F->TIP="";
	//kontrola zda je zadan� hodnota v rozmez�
	if(F->m.between(F->pom_temp->pohon->aRD,F->pom_temp->pohon->rychlost_od,F->pom_temp->pohon->rychlost_do)) mimo_rozmezi=false;
	else mimo_rozmezi=true;
	// nutn� o�et�en� pro obdob� zad�v�n�/psan�
	if (F->pom_temp->pohon->aRD > 0)
	{
    //v�po�et doporu�en� rychosti
		double dopRD=F->m.dopRD(F->d.v.PP.delka_jig,F->d.v.PP.sirka_jig,F->pom_temp->rotace,F->pom_temp->pohon->roztec,F->d.v.PP.TT,F->pom_temp->pohon->aRD);
		//zadan� rychlost je mimo rozsah
		if(mimo_rozmezi)
		{
			F->TIP="Rychlost neodpov�d� rozmez�!";
		}
//		if(F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi)
//		{
//			F->TIP="Rychlost neodpov�d� rozmez�!";
//		}
		//je zvolen pohon, jeho aktu�ln� rychlost se nerovn� doporu�en�
		if(Combo->ItemIndex!=0 && F->pom_temp->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
				F->TIP="Zadejte doporu�enou rychlost pohonu: "+AnsiString(F->outaRD(dopRD));
		}
//		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi)
//		{
//			F->TIP="Rychlost neodpov�d� rozmez�!";
//		}
    //v�e je vpo��dku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
			F->TIP="OK";
		}
	}
	else F->TIP="Neplatn� hodnota rychlosti pohonu!";
}
//---------------------------------------------------------------------------


