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
{ //výchozí nastavení - pøi zobrazení tab.elementù je totiž vždy volán Onchange, pøi naèítání hodnot do buòek
	// proto je nastaven input_state=NO, aby v tento moment neprobíhal žádný výpoèet v Onchange události
 input_state=NO;
}
//---------------------------------------------------------------------------


void TFormX::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
// pøi kliku do nìjaké buòky nastavím input_state=NOTHING, pokud udìlám zmìnu buòky je v OnChange události switch, který zajistí
// výpoèet konkrétní buòky dle pøedávaných parametrù v události
	input_state=NOTHING;
}

void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
//
}

//zpracování onchange události - INPUT, výpoèet a OUTPUT zpìt do ovlivnìné buòky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING&&ID!=9999)
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
					//Stop stanice má pouze èasové údaje, které spolu nesouviseji ?? (wt)
				} break;
				case 1: //robot (kontinuální)
				{
					if(Col==1 && Row==1) //vstup PT -> vystup LO
					{
						input_state=PT; //nastaveni stavu
						E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO1 = F->pom_temp->pohon->aRD*E->PT1;
						E->mGrid->Cells[Col][Row+1].Text = F->outLO(E->LO1); //OUTPUT
					}
					if(Col==1 && Row==2) //vstup LO -> vystup PT
					{
						input_state=LO; //nastaveni stavu
						E->LO1 = F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1 = E->LO1/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PT1);  //OUTPUT
					}
				} break;
				case 2: //robot se stop stanicí
				{
					//obsahuje 2 èasové údaje jeden zadávaný uživatelm druhý získávaný z pohonu (wt)
				} break;
				case 3: //robot s pasivní otoèí
				{
					if (Row==1)
					{
						input_state=PT;//vstup èas - výstup lakovací okno
						E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO1 = F->pom_temp->pohon->aRD*E->PT1;
						E->mGrid->Cells[Col][Row+1].Text = F->outLO(E->LO1); //výstup do buòky LO
					}
					if (Row==2)
					{
						input_state=LO;
						E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1=E->LO1/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PT1);
					}
					if (Row==4)//tøetí øádek není uživatelsky upravitelný
					{
						input_state=DO;//delka otoèe
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PTotoc);
					}
					if (Row==5)
					{
						input_state=PT;
						E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->LO2=F->pom_temp->pohon->aRD * E->PT2;
						E->mGrid->Cells[Col][Row+1].Text=F->outLO(E->LO2);
					}
					if (Row==6)
					{
						input_state=LO;
						E->LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT2=E->LO2/F->pom_temp->pohon->aRD; //nahradit aRD
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PT2);
					}
				} break;
				case 4://robot s aktivní otoèí (resp. s otoèí a stop stanicí)
				{
					//obsahuje pouze èasové buòky + wt
				} break;
				case 5://otoè pasivní
				{
					if (Row==1)//zde se upravuje pouze délka
					{
						input_state=DO;//delka otoèe
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row+1].Text = F->outPT(E->PTotoc);
					}
				} break;
				case 6://otoè aktivní (resp. otoè se stop stanicí)
				{
					if (Row==2)
					{
						input_state=PTotoc;
						E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->OTOC_delka=F->pom_temp->pohon->aRD*E->PTotoc;
						E->mGrid->Cells[Col][Row-1].Text = F->outDO(E->OTOC_delka);
					}
				} break;
			}
		}
		E=NULL;delete E;
	}
	if(input_state==NOTHING&&ID==9999)
	{
		switch(Row)
		{
			case 1://výbìr pohonu
			{
				F->tab_pohon_COMBO(1);//pøiøazení pohonu
			}break;
			case 2://aktuální rychlost, aRD
			{
				////////////naètení aRD z editu////////////
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
				F->pom_temp->pohon->aRD=aRD;
				////////////propoèty v tabulce pohonu////////////
				if(F->PmG->RowCount>4)//pro tabulku ve S&G režimu
				{
					F->m.Rx(aRD,F->pom_temp->pohon->roztec);
					F->PmG->Cells[1][3].Text=F->outR(F->pom_temp->pohon->roztec);
					F->pom_temp->pohon->Rz=F->m.Rz(aRD);
					F->PmG->Cells[1][4].Text=F->outRz(F->pom_temp->pohon->Rz);
				}
				else//pro tabulku v kontinuálním režimu
				{
					F->m.Rx(aRD,F->pom_temp->pohon->roztec);
					F->PmG->Cells[1][3].Text=F->outR(F->pom_temp->pohon->roztec);
        }
				////////////propoèty v tabulkách elementù////////////
				Cvektory::TElement *E=F->pom_temp->elementy;
				while(E!=NULL)
				{
					if(E->n>0)
					{
						switch(E->eID)
						{
							case 0:break;//stop stanice
							case 1://robor kontinuální
							{
								E->PT1=E->LO1/aRD;
								E->mGrid->Cells[1][1].Text=F->outPT(E->PT1);
              }
							break;
							case 2://robot se stop stanicí
							{
              	//validace
								E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,aRD*60,3);
								E->mGrid->Cells[1][2].Text=F->outPT(E->WT);
              }
							break;
							case 3://robot s pasivní otoèí
							{

              }
							break;
							case 4://robot s aktivní otoèí
							{
								//validace
								E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,aRD*60,3);
								E->mGrid->Cells[1][5].Text=F->outPT(E->WT);
							}break;
							case 5://otoè pasivní
							{
								E->OTOC_delka=aRD*E->PTotoc;
								E->mGrid->Cells[1][1].Text = F->outDO(E->OTOC_delka);
							}break;
							case 6://otoè aktivní
							{
								E->PTotoc=E->OTOC_delka/aRD;
								E->mGrid->Cells[1][2].Text = F->outPT(E->PTotoc);
							}break;
            }
          }
				}


			}break;
			case 3://rozteè, R
			{

			}break;
//			case 4://rozestup, Rz   NEBUDE EDITOVATELNÝ
//			{          //aRD=Rz/TT
//				input_state=Rz;
//				F->pom->pohon->Rz=F->inRz(F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text));
//				F->pom->pohon->aRD=F->pom->pohon->Rz/F->d.v.PP.TT;
//				F->PmG->Cells[1][2].Text=F->outaRD(F->pom->pohon->aRD);
//			}break;
			case 5://Rx
			{        //Rz=Rx*R
				input_state=Rx;
				F->pom->pohon->Rx=F->ms.MyToDouble(F->PmG->Cells[Col][Row].Text);//POM_TEMP
				F->pom->pohon->Rz=F->pom->pohon->Rx*F->pom->pohon->roztec;
				F->PmG->Cells[1][4].Text=F->pom->pohon->Rz;
			}break;
		}
	}
	F->REFRESH();
	input_state=NOTHING;
	F->nahled_ulozit(true);
}
