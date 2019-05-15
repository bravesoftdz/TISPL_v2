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
 validace_true=false;
}
//---------------------------------------------------------------------------
void TFormX::OnClick(long Tag,long ID,long Col,long Row) //unsigned
{
// pøi kliku do nìjaké buòky nastavím input_state=NOTHING, pokud udìlám zmìnu buòky je v OnChange události switch, který zajistí
// výpoèet konkrétní buòky dle pøedávaných parametrù v události
	input_state=NO;
	if(ID==9999&&Row>=1&&F->PmG->Rows[3].Visible)//pokud je kliknuto do tabulky pohonu, podle buòky vyznèí buòky, které budou zmìnou ovlivnìné
		korelace_tab_pohonu(Row);//spuštìno pouze v pøípadì KK tabulky
	if(ID==9999&&Row>=1)korelace_tab_pohonu_elementy();//pøi S&G režimu je stále nutné zobrazit korelaci v elementech
	if(ID==9999&&Row==-2)//pokud je stisknut exButton v tabulce pohonu
	//funkcionalita exBUTTONu v tabulce pohonu
	{
		AnsiString Hint=F->PmG->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabráni probliku Hintu, toto sloužilo pro tlaèítko, ale nebylo plnì uèinné: int T=F->PmG->exBUTTON->Top;
		if(F->PmG->Rows[4].Visible)
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			F->PmG->VisibleRow(4,false,false);//nepøekreslovat
			F->PmG->VisibleRow(5,false,false);

		}else
		{
			F->PmG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			F->PmG->VisibleRow(4,true,false);//nepøekreslovat
			F->PmG->VisibleRow(5,true,false);
		}
		F->PmG->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt
		F->PmG->Update();
		F->PmG->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt, toto nestaèilo: F->PmG->exBUTTON->Top=T;//zajistí, že se tlaèítko nepøepozicuje
		F->REFRESH();//musí být opravdu REFRESH celého formu nikoliv jen mGridu
		F->PmG->exBUTTON->Hint=Hint;//navrácení pùvodního textu hintu
	}
	if(ID!=9999&&Row==-2)
	//funkcionalita exBUTTONu v tabulkách elementù
	{
		Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID);
		AnsiString Hint=E->mGrid->exBUTTON->Hint;F->PmG->exBUTTON->Hint="";//zabráni probliku Hintu, toto sloužilo pro tlaèítko, ale nebylo plnì uèinné: int T=F->PmG->exBUTTON->Top;
		if(E->mGrid->Rows[2].Visible)
		{
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			E->mGrid->VisibleRow(2,false,false);//nepøekreslovat
		}
		else
		{
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->VisibleRow(2,true,false);//nepøekreslovat
		}
		E->mGrid->exBUTTONLockPosition=true;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt
		E->mGrid->Update();
		E->mGrid->exBUTTONLockPosition=false;//uzamkne pozici exButtonu, aby se nepøepozival bìhem updatu tam a zpìt, toto nestaèilo: F->PmG->exBUTTON->Top=T;//zajistí, že se tlaèítko nepøepozicuje
		F->REFRESH();//musí být opravdu REFRESH celého formu nikoliv jen mGridu
		E->mGrid->exBUTTON->Hint=Hint;//navrácení pùvodního textu hintu
		E=NULL; delete E;
	}
	if(ID!=9999)//korelace v tabulkách elementù
		korelace_v_elementech(ID,Row);
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
					if(Row==1)//zmìna PT
					{
						input_state=PT;
						E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//naètení z mgridu
						E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][2].Text=F->m.round2double(F->outPT(E->RT),3);//výpis do mGridu
					}
					if(Row==3)//zmìna WT
					{
						input_state=WT;
						E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//naètení z mgridu
						E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][3].Text=F->m.round2double(F->outPT(E->RT),3);//výpis do mGridu
					}
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
					if (Row==3)//COMBO
					{
						input_state=COMBO;//úhel otoèe
						switch(E->mGrid->getCombo(1,3)->ItemIndex)
						{
							case 0:E->rotace_jig=-180;break;
							case 1:E->rotace_jig=-90;break;
							case 2:E->rotace_jig=90;break;
							case 3:E->rotace_jig=180;break;
						}
					}
					if (Row==5)//ètvrtý øádek není uživatelsky upravitelný
					{
						input_state=DO;//delka otoèe
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
				case 4://robot s aktivní otoèí (resp. s otoèí a stop stanicí)
				{
					if(Row==1)//zmìna PT1
					{
						input_state=PT;
						E->PT1=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//naètení z mgridu
						E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//výpis do mGridu
					}
					if (Row==2)//COMBO
					{
						input_state=COMBO;//úhel otoèe
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
						input_state=PTotoc;
						E->PTotoc=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//naètení z mgridu
						E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//výpis do mGridu
					}
					if(Row==4)//zmìna PT2
					{
						input_state=PT2;
						E->PT2=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//naètení z mgridu
						E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//výpis do mGridu
					}
					if(Row==6)//zmìna WT
					{
						input_state=WT;
						E->WT=F->inPT(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text));//naètení z mgridu
						E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
						E->mGrid->Cells[Col][5].Text=F->m.round2double(F->outPT(E->RT),3);//výpis do mGridu
					}
				} break;
				case 5://otoè pasivní
				{
					if (Row==1)//COMBO
					{
						input_state=COMBO;//úhel otoèe
						switch(E->mGrid->getCombo(1,1)->ItemIndex)
						{
							case 0:E->rotace_jig=-180;break;
							case 1:E->rotace_jig=-90;break;
							case 2:E->rotace_jig=90;break;
							case 3:E->rotace_jig=180;break;
						}
					}
					if (Row==2)//zde se upravuje pouze délka
					{
						input_state=DO;//delka otoèe
						E->OTOC_delka=F->inDO(F->ms.MyToDouble(E->mGrid->Cells[Col][Row].Text)); //INPUT
						E->PTotoc=E->OTOC_delka/F->pom_temp->pohon->aRD;
						E->mGrid->Cells[Col][Row+1].Text = F->m.round2double(F->outPT(E->PTotoc),3);
					}
				} break;
				case 6://otoè aktivní (resp. otoè se stop stanicí)
				{
					if (Row==1)//COMBO
					{
						input_state=COMBO;//úhel otoèe
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
			case 0://výbìr pohonu
			{
				input_state=COMBO;
				vstoupeno_poh=false;//blokace událostí pøi vkládání elementu
				F->tab_pohon_COMBO(1);//pøiøazení pohonu
				F->aktualizace_ComboPohon();
				if(F->PmG->getCombo(0,0)->ItemIndex!=0)
				{
					if(aRD>0)F->scGPComboBox_prepinacKot->Enabled=true;
					F->scGPGlyphButton_PLAY->Enabled=true;//zapnutí tlaèítka animace
					//aktualizace tabulky
					if(F->PmG->Rows[3].Visible)//pro tabulku v kontinuálním režimu
					{
						F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
						F->PmG->Cells[1][2].Text=F->m.round2double(F->outR(F->pom_temp->pohon->roztec),3);
						F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
						F->PmG->Cells[1][3].Text=F->m.round2double(F->pom_temp->pohon->Rx,0);
						F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
						F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
						//místo pro mezeru mezi podvozky
						if(F->PmG->Rows[7].Visible)//budou zde obì mezeri mezi jigy
						{
							F->PmG->Cells[1][6].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,1));
							F->PmG->Cells[1][7].Text=F->outRz(F->m.mezera(90,F->pom_temp->pohon->Rz,1));
						}
						else if(F->PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
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
					F->scGPGlyphButton_PLAY->Enabled=false;//vypnutí tlaèítka animace
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
			case 4://rozestup, Rz   NEBUDE EDITOVATELNÝ
			break;
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
		}
		FormX->vstoupeno_poh=true;//blokace událostí pøi vkládání elementu
		if(F->PmG->Cells[Col][Row].Text=="")input_state=NOTHING;
		else F->Timer2->Enabled=true;
	}
	if(vstoupeno_elm||vstoupeno_poh) F->nahled_ulozit(true);
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
   		F->REFRESH();
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
void TFormX::zmena_aRD ()
{
	F->aktualizace_ComboPohon();
	//propoèty v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK režimu
	{
		F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->aRD);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->Rx=F->m.Rx(F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][3].Text=F->m.round2double(F->pom_temp->pohon->Rx,0);
		F->PmG->Cells[1][5].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0));
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
	//propoèty v tabulkách elementù
	aktualizace_tab_elementu();
	if(validace_true&&F->PmG->Rows[3].Visible)validace();
}
//---------------------------------------------------------------------------
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou rozteèe
void TFormX::zmena_R ()
{
	//pøepoèet hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//pro tabulku v KK režimu
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz); //prohozené poøadí z dùvodu, že druhý výpoèet potøebuje aktualizovaonu honotu prvního výpoètu
		F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		F->PmG->Cells[1][5].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0));
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
//pøepoèty tabulek elementù a pohonu vyvolané zmìnou Rx
void TFormX::zmena_Rx ()
{
	//pøepoèet hodnot v tabulce pohonu
	if(F->PmG->Rows[3].Visible)//kontinuální režim
	{
    F->pom_temp->pohon->Rz=F->m.Rz(F->pom_temp->pohon->Rx,F->pom_temp->pohon->roztec);
		F->PmG->Cells[1][4].Text=F->m.round2double(F->outRz(F->pom_temp->pohon->Rz),3);
		if(!validace_true)
		{
			F->pom_temp->pohon->aRD=F->m.RD(F->pom_temp->pohon->Rz);
			F->PmG->Cells[1][1].Text=F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3);
		}
		F->PmG->Cells[1][5].Text=F->outRz(F->m.mezera(0,F->pom_temp->pohon->Rz,0));
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
					E->RT=F->m.RT(E->PT1,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
					E->mGrid->Cells[1][2].Text=F->m.round2double(F->outPT(E->RT),3);
				}
				break;
				case 3://robot s pasivní otoèí
				{
					E->PT1=F->m.PT(E->LO1,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][1].Text=F->m.round2double(F->outPT(E->PT1),3);
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][4].Text=F->m.round2double(F->outPT(E->PTotoc),3);
					E->PT2=F->m.PT(E->LO2,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->PT2),3);
				}
				break;
				case 4://robot s aktivní otoèí
 				{
					//validace
					E->WT=F->m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3);
					E->mGrid->Cells[1][6].Text=F->m.round2double(F->outPT(E->WT),3);
					E->RT=F->m.RT(E->PT1+E->PT2+E->PTotoc,F->d.v.vzdalenost_od_predchoziho_elementu(E),F->pom_temp->pohon->aRD,F->pom_temp->pohon->roztec,E->WT);
					E->mGrid->Cells[1][5].Text=F->m.round2double(F->outPT(E->RT),3);
				}break;
 				case 5://otoè pasivní
				{
					E->PTotoc=F->m.PT(E->OTOC_delka,F->pom_temp->pohon->aRD);
					E->mGrid->Cells[1][3].Text = F->m.round2double(F->outPT(E->PTotoc),3);
				}break;
				case 6://otoè aktivní
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
//Naplní hodnoty které závisí na pohonu 0
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
				case 1://robor kontinuální
				{
					E->mGrid->Cells[1][1].Text=0;
				}
				break;
				case 2://robot se stop stanicí
				{
					//validace
					E->mGrid->Cells[1][2].Text=0;
					E->mGrid->Cells[1][3].Text=0;
				}
				break;
				case 3://robot s pasivní otoèí
				{
					E->mGrid->Cells[1][1].Text=0;
					E->mGrid->Cells[1][4].Text=0;
					E->mGrid->Cells[1][6].Text=0;
				}
				break;
				case 4://robot s aktivní otoèí
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
				F->PmG->Cells[1][4].Highlight=true; //rozestup
				F->PmG->Cells[1][3].Highlight=true; //rx
				F->PmG->Cells[1][6].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][7].Highlight=true;
			}
		}break;
		case 2: //zmena rozteèe R
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
			{
				F->PmG->Cells[1][4].Highlight=true;  //rozestup
				F->PmG->Cells[1][1].Highlight=true; //aRD
				F->PmG->Cells[1][6].Highlight=true; //mezera mezi jigy
				F->PmG->Cells[1][7].Highlight=true;
			}
		}break;
		case 3: //zmìna Rx
		{
			if(F->PmG->RowCount>3)//pro tabulku ve S&G režimu
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
//stará se o highlitování políèek, které souvisí s mìnìnou hodnotou (elementy)
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
				case 1://robor kontinuální
				{
					E->mGrid->Cells[1][1].Highlight=true;
				}
				break;
				case 2://robot se stop stanicí
				{
					//validace
					E->mGrid->Cells[1][2].Highlight=true;
					E->mGrid->Cells[1][3].Highlight=true;
				}
				break;
				case 3://robot s pasivní otoèí
				{
					E->mGrid->Cells[1][1].Highlight=true;
					E->mGrid->Cells[1][4].Highlight=true;
					E->mGrid->Cells[1][6].Highlight=true;
				}
				break;
				case 4://robot s aktivní otoèí
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
	Cvektory::TElement *E=F->d.v.vrat_element(F->pom_temp,ID);
	switch(E->eID)
	{
		case 0:break;//stopka
		case 1: //robot (kontinuální)
		{
			if(Row==1)E->mGrid->Cells[1][Row+1].Highlight=true;
			if(Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
		case 2: //robot se stop stanicí
		{
			if(Row==1)E->mGrid->Cells[1][2].Highlight=true;
		} break;
		case 3: //robot s pasivní otoèí
		{
			if (Row==1)E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==2)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==5)E->mGrid->Cells[1][Row-1].Highlight=true;
			if (Row==6)E->mGrid->Cells[1][Row+1].Highlight=true;
			if (Row==7)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
    case 4://robot s aktivní otoèí (resp. s otoèí a stop stanicí)
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
			if (Row==3)E->mGrid->Cells[1][Row-1].Highlight=true;
		} break;
	}
	E->mGrid->Refresh();
	E=NULL; delete E;
}
//odstraní highlight na všech tabulkách
void TFormX::odstranit_korelaci()
{
	F->DrawGrid_knihovna->SetFocus();//po kliku mimo zùstával focus poøád na editu
	F->PmG->unHighlightAll();
	Cvektory::TElement *E=F->pom_temp->elementy;
	while(E!=NULL)
	{
		if(E->n>0)
			E->mGrid->unHighlightAll();
		E=E->dalsi;
	}
	validace_true=false;//tato metoda se volá pøi kliku mimo formuláø, pøíhodné vypnout validaci
}
//---------------------------------------------------------------------------
//validace rychlosti pøi její zmìnì
void TFormX::validace()
{
	bool mimo_rozmezi=false;;
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
			F->PmG->ShowNote("Rychlost neodpovídá rozmezí!",clRed,14);
		}
		//je zvolen pohon, jeho aktuální rychlost se nerovná doporuèené
		if(Combo->ItemIndex!=0 && F->pom_temp->pohon->roztec>0 && F->ms.MyToDouble(dopRD)!= F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
				F->PmG->ShowNote("Zadejte doporuèenou rychlost pohonu: <a>"+AnsiString(F->m.round2double(F->outaRD(dopRD),3))+"</a>",clRed,14);
		}
		//vše je vpoøádku
		if (F->ms.MyToDouble(dopRD)== F->ms.MyToDouble(F->pom_temp->pohon->aRD) && mimo_rozmezi==false)
		{
			F->PmG->ShowNote("",clRed,14);
		}
	}
	else F->PmG->ShowNote("Neplatná hodnota rychlosti pohonu!",clRed,14);
}
//---------------------------------------------------------------------------
//voláno po kliku na link v poznámce, naplní edit aRD doporuèenou rychlostí
void TFormX::naplneni_dopRD()
{
	F->PmG->Cells[1][1].Text=F->outaRD(dopRD);
	F->PmG->ShowNote("",clRed,14);
	//odstranit_korelaci();//pro jistotu zùstavala aktivní po kliku na link
}
//---------------------------------------------------------------------------
