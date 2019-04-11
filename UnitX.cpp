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
}
//---------------------------------------------------------------------------


void TFormX::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
// p�i kliku do n�jak� bu�ky nastav�m input_state=NOTHING, pokud ud�l�m zm�nu bu�ky je v OnChange ud�losti switch, kter� zajist�
// v�po�et konkr�tn� bu�ky dle p�ed�van�ch parametr� v ud�losti
	input_state=NOTHING;
}

void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
//
}

//zpracov�n� onchange ud�losti - INPUT, v�po�et a OUTPUT zp�t do ovlivn�n� bu�ky
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
					//Stop stanice m� pouze �asov� �daje, kter� spolu nesouviseji ?? (wt)
				} break;
				case 1: //robot (kontinu�ln�)
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
						E->mGrid->Cells[Col][Row+1].Text = F->outLO(E->LO1); //v�stup do bu�ky LO
					}
					if (Row==2)
					{
						input_state=LO;
						E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1=E->LO1/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->outPT(E->PT1);
					}
					if (Row==4)//t�et� ��dek nen� u�ivatelsky upraviteln�
					{
						input_state=DO;//delka oto�e
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
						E->mGrid->Cells[Col][Row+1].Text = F->outPT(E->PTotoc);
					}
				} break;
				case 6://oto� aktivn� (resp. oto� se stop stanic�)
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
			case 1://v�b�r pohonu
			{
				F->tab_pohon_COMBO(1);//p�i�azen� pohonu
			}break;
			case 2://aktu�ln� rychlost, aRD
			{
				////////////na�ten� aRD z editu////////////
				double aRD=F->inaRD(F->ms.MyToDouble(F->PmG->Cells[1][Row].Text));
				F->pom_temp->pohon->aRD=aRD;
				////////////propo�ty v tabulce pohonu////////////
				if(F->PmG->RowCount>4)//pro tabulku ve S&G re�imu
				{
					F->m.Rx(aRD,F->pom_temp->pohon->roztec);
					F->PmG->Cells[1][3].Text=F->outR(F->pom_temp->pohon->roztec);
					F->pom_temp->pohon->Rz=F->m.Rz(aRD);
					F->PmG->Cells[1][4].Text=F->outRz(F->pom_temp->pohon->Rz);
				}
				else//pro tabulku v kontinu�ln�m re�imu
				{
					F->m.Rx(aRD,F->pom_temp->pohon->roztec);
					F->PmG->Cells[1][3].Text=F->outR(F->pom_temp->pohon->roztec);
        }
				////////////propo�ty v tabulk�ch element�////////////
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
								E->PT1=E->LO1/aRD;
								E->mGrid->Cells[1][1].Text=F->outPT(E->PT1);
              }
							break;
							case 2://robot se stop stanic�
							{
              	//validace
								E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,aRD*60,3);
								E->mGrid->Cells[1][2].Text=F->outPT(E->WT);
              }
							break;
							case 3://robot s pasivn� oto��
							{

              }
							break;
							case 4://robot s aktivn� oto��
							{
								//validace
								E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,aRD*60,3);
								E->mGrid->Cells[1][5].Text=F->outPT(E->WT);
							}break;
							case 5://oto� pasivn�
							{
								E->OTOC_delka=aRD*E->PTotoc;
								E->mGrid->Cells[1][1].Text = F->outDO(E->OTOC_delka);
							}break;
							case 6://oto� aktivn�
							{
								E->PTotoc=E->OTOC_delka/aRD;
								E->mGrid->Cells[1][2].Text = F->outPT(E->PTotoc);
							}break;
            }
          }
				}


			}break;
			case 3://rozte�, R
			{

			}break;
//			case 4://rozestup, Rz   NEBUDE EDITOVATELN�
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
