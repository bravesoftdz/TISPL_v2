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
input_state=NOTHING; Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID); E->mGrid->Refresh();
}

void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
//
}

//zpracování onchange události - INPUT, výpoèet a OUTPUT zpìt do ovlivnìné buòky
void TFormX::OnChange(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	if(input_state==NOTHING)
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
						E->LO1 = /*F->pom_temp->pohon->aRD*/5.0 * E->PT1; //nahradit aRD                                      //vypocet
						E->mGrid->Cells[Col][Row+1].Text = F->outLO(E->LO1); //OUTPUT
						E->mGrid->Cells[Col][Row+1].Text=E->mGrid->Cells[Col][Row+1].Text;
					}
					if(Col==1 && Row==2) //vstup LO -> vystup PT
					{
						input_state=LO; //nastaveni stavu
						E->LO1 = F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1 = E->LO1 / 5.0/*F->pom_temp->pohon->aRD*/;       //nahradit aRD             //vypocet
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
						E->PT1 = F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//vstup z buòky PT
						E->LO1 = /*F->pom_temp->pohon->aRD*/5.0 * E->PT1; //nahradit aRD
						E->mGrid->Cells[Col][Row+1].Text = F->outLO(E->LO1); //výstup do buòky LO
					}
					if (Row==2)
					{
						input_state=LO;
						E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						E->PT1=E->LO1/5.0/*F->pom_temp->pohon->aRD*/; //nahradit aRD
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PT1);
					}
					if (Row==4)//tøetí øádek není uživatelsky upravitelný
					{
						input_state=DO;//delka otoèe
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						E->PTotoc=E->OTOC_delka/5.0/*F->pom_temp->pohon->aRD*/; //nahradit aRD
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PTotoc);
					}
					if (Row==5)
					{
						input_state=PT;
						E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						E->LO2=/*F->pom_temp->pohon->aRD*/5.0 * E->PT2; //nahradit aRD
						E->mGrid->Cells[Col][Row+1].Text=F->outLO(E->LO2);
					}
					if (Row==6)
					{
						input_state=LO;
						E->LO2=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						E->PT2=E->LO2/5.0/*F->pom_temp->pohon->aRD*/; //nahradit aRD
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
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						E->PTotoc=E->OTOC_delka/5.0/*F->pom_temp->pohon->aRD*/; //nahradit aRD
						E->mGrid->Cells[Col][Row+1].Text = F->outPT(E->PTotoc);
					}
				} break;
				case 6://otoè aktivní (resp. otoè se stop stanicí)
				{
					if (Row==2)
					{
						input_state=PTotoc;
						E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));
						E->OTOC_delka=/*F->pom_temp->pohon->aRD*/5.0*E->PTotoc;
						E->mGrid->Cells[Col][Row-1].Text = F->outDO(E->OTOC_delka);
					}
				} break;
			}
		}
		E->mGrid->Refresh();
		//V Unit1 je obsluha události pøi povolení Timeru, kde je volán i REFRESH();
		F->Timer2->Enabled=true;

		E=NULL;delete E;
		input_state=NOTHING;
		F->nahled_ulozit(true);
	}
}
