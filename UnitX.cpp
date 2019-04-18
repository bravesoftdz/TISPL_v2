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
 vstoupeno_poh=false;
 vstoupeno_elm=false;
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,unsigned long Col,unsigned long Row)
{
// pøi kliku do nìjaké buòky nastavím input_state=NOTHING, pokud udìlám zmìnu buòky je v OnChange události switch, který zajistí
// výpoèet konkrétní buòky dle pøedávaných parametrù v události
	unhighlight_tabulky();
	if(ID==9999&&Row>=1)
		highlight_tab_pohonu(Row);
	input_state=NOTHING;
}
//---------------------------------------------------------------------------
void TFormX::OnEnter(long Tag,long ID,unsigned long Col,unsigned long Row)
{
	//po kliku do vykreslené tabulky lze obnovit událost OnChange
	if(ID==9999)vstoupeno_poh=true;
	else vstoupeno_elm=true;
}
//---------------------------------------------------------------------------
//zpracování onchange události - INPUT, výpoèet a OUTPUT zpìt do ovlivnìné buòky
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
					//Stop stanice má pouze èasové údaje, které spolu nesouviseji ?? (wt)
				} break;
				case 1: //robot (kontinuální)
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
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outLO(E->LO1),3); //výstup do buòky LO
					}
					if (Row==2)
					{
						input_state=LO;
						E->LO1=F->inLO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PT1=E->LO1/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row-1].Text = F->m.round2double(F->outPT(E->PT1),3);
					}
					if (Row==4)//tøetí øádek není uživatelsky upravitelný
					{
						input_state=DO;//delka otoèe
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
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					}
				} break;
				case 6://otoè aktivní (resp. otoè se stop stanicí)
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
			case 0://výbìr pohonu
			{
				input_state=COMBO;
				F->tab_pohon_COMBO(1);//pøiøazení pohonu
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
			case 3://rozestup, Rz   NEBUDE EDITOVATELNÝ
			break;
			case 4://Rx
			{
				input_state=Rx;
				//naètení Rx z editu
				double Rx=F->ms.MyToDouble(F->PmG->Cells[1][Row].Text);
				//uložení do spojáku
				F->pom_temp->pohon->Rx=Rx;
				//volání metody pro pøepoèty všech souvisejících bunìk
				zmena_Rx();
			}break;
		}
		if(F->PmG->Cells[Col][Row].Text=="")input_state=NOTHING;
		else F->Timer2->Enabled=true;
	}
	if(vstoupeno_elm||vstoupeno_poh) F->nahled_ulozit(true);
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rychlosti
void TFormX::zmena_aRD ()
{
	F->aktualizace_ComboPohon();
	//propoèty v tabulce pohonu
	if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
	{
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->pom_temp->pohon->Rx,3);
	}
	//propoèty v tabulkách elementù
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rozteèe
void TFormX::zmena_R ()
{
	//pøepoèet hodnot v tabulce pohonu
	if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz); //prohozené poøadí z dùvodu, že druhý výpoèet potøebuje aktualizovaonu honotu prvního výpoètu
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
	}
	//pøepoèet hodnot v elementech
	F->aktualizace_ComboPohon();
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou Rx
void TFormX::zmena_Rx ()
{
  //pøepoèet hodnot v tabulce pohonu
	if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
	{
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
	}
	else//pro tabulku v kontinuálním režimu
	{
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
	}
	//pøepoèet hodnot v elementech
  F->aktualizace_ComboPohon();
	aktualizace_tab_elementu();
}
//---------------------------------------------------------------------------
//pøepoèet v tabulkách elementù po zmìnì parametrù v tabulce pohonu
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
				case 1://robor kontinuální
				{
					E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
				}
				break;
				case 2://robot se stop stanicí
				{
					//validace
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->WT),3);
				}
				break;
				case 3://robot s pasivní otoèí
				{
					E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][3].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->PT2=F->m.PT(E->LO2,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(E->PT2),3);
				}
				break;
				case 4://robot s aktivní otoèí
 				{
					//validace
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT),3);
				}break;
 				case 5://otoè pasivní
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][2].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://otoè aktivní
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][2].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
			}
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
}
//---------------------------------------------------------------------------
//highlightovaní buòìk tabulky pohonu
void TFormX::highlight_tab_pohonu(int Row)
{
	switch(Row)
	{
		case 1: //zmena aRD
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
			{
				F->PmG->Cells[1][3].Highlight=true; //rozestup
				F->PmG->Cells[1][4].Highlight=true; //rx
			}
		}break;
		case 2: //zmena rozteèe R
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
			{
				F->PmG->Cells[1][3].Highlight=true;  //rozestup
				F->PmG->Cells[1][1].Highlight=true; //aRD
			}
		}break;
		case 4: //zmìna Rx
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
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
//stará se o highlitování políèek, které souvisí s mìnìnou hodnotou (elementy)
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
				case 1://robor kontinuální
				{
					E->mGrid->Cells[1][1].Highlight=true;
				}
				break;
				case 2://robot se stop stanicí
				{
					//validace
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 3://robot s pasivní otoèí
				{
					E->mGrid->Cells[1][1].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
					E->mGrid->Cells[1][5].Highlight=true;
				}
				break;
				case 4://robot s aktivní otoèí
 				{
					//validace
					E->mGrid->Cells[1][6].Highlight=true;
				}break;
 				case 5://otoè pasivní
				{
					E->mGrid->Cells[1][2].Highlight=true;
				}break;
				case 6://otoè aktivní
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
//odstraní highlight na všech tabulkách
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



