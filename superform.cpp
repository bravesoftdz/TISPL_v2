// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "superform.h"
#include "unit1.h"
//#include "dopravniky.h"
#include "cesty.h"
#include "jig.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzGrids"
#pragma link "rHTMLLabel"
#pragma link "rImprovedComps"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scGPExtControls"
#pragma resource "*.dfm"
TForm_definice_zakazek *Form_definice_zakazek;

// ---------------------------------------------------------------------------
__fastcall TForm_definice_zakazek::TForm_definice_zakazek(TComponent* Owner)
	: TForm(Owner)
{
	nastav_form(); //nastavení barev komponent
}
//---------------------------------------------------------------------------
//nastavení barev komponent
void TForm_definice_zakazek::nastav_form()
{
	//nastavení globálních barev
	TColor light_gray=(TColor)RGB(240,240,240);

	rStringGridEd1->Color=clWhite;//nastavení pozadí barvy formuláøe
	Form_definice_zakazek->Color=light_gray;//RGB(43,87,154);
	rHTMLLabel_effektivita->Font->Color=(TColor)RGB(89,89,89);
	rHTMLLabel_pocet_dnu->Font->Color=rHTMLLabel_effektivita->Font->Color;
	rHTMLLabel_pocet_prac_hod->Font->Color=rHTMLLabel_effektivita->Font->Color;
	rHTMLLabel_pozad_mnozstvi->Font->Color=rHTMLLabel_effektivita->Font->Color;
	rHTMLLabel_zacatek->Font->Color=rHTMLLabel_effektivita->Font->Color;
	//rHTMLLabel3->Font->Color=(TColor)RGB(50,50,50);   //velky nadpis  1

	scGPButton2->Options->NormalColor=Form_definice_zakazek->Color;
	scGPButton2->Options->FocusedColor=Form_definice_zakazek->Color;
	scGPButton2->Options->HotColor=Form_definice_zakazek->Color;
	scGPButton2->Options->PressedColor=Form_definice_zakazek->Color;
	scGPButton2->Options->FramePressedColor=Form_definice_zakazek->Color;

	scGPButton4->Options->NormalColor=Form_definice_zakazek->Color;
	scGPButton4->Options->FocusedColor=Form_definice_zakazek->Color;
	scGPButton4->Options->HotColor=Form_definice_zakazek->Color;
	scGPButton4->Options->PressedColor=Form_definice_zakazek->Color;
	scGPButton4->Options->FrameNormalColor=Form_definice_zakazek->Color;
	scGPButton4->Options->FramePressedColor=Form_definice_zakazek->Color;
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//zobrazení formuláøe
void __fastcall TForm_definice_zakazek::FormShow(TObject *Sender)
{
	////nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
	nacti_PP();

	////naètení zakázek a cest
	if(Form1->d.v.ZAKAZKY->dalsi==NULL)//kdyz je spojak prazdny
	{
		 //vytvoøí defaultní øadek se zakázkou a její cestou
		 predvypln_default_zakazku();
		 //predvypln_cestu(); již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
		 //a hned se uloží do temp spojáku
		 uloz_Defaulttemp_zakazku();
		 uloz_Default_cestu();
		 Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY(); // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
		 //po uložení do hl.spojaku zakázek potøebuji stejnì znovu uložit øádek do temp_zakazek
		 uloz_Defaulttemp_zakazku();
		 uloz_Default_cestu();
	}
	else//pokud je uloženo nìco v zakázkách tak je naètu
	{
		 nacti_zakazky();
	}
}
// ---------------------------------------------------------------------------
//nastaveni PP, defaultní jsou již od souboru novy, který se volá vždy, takže není defaultní nutné volat znovu
void TForm_definice_zakazek::nacti_PP()
{
	rEditNum_pozad_mnozstvi->Text=Form1->d.v.PP.mnozstvi;
	rEditNum_pocet_dnu->Text=Form1->d.v.PP.dni_rok;
	rEditNum_effektivita->Text=Form1->d.v.PP.efektivita;
	rEditNum_pocet_prac_hod->Text=Form1->d.v.PP.hod_den;
	scEdit_zacatek->Text=Form1->d.v.PP.cas_start;
}
// ---------------------------------------------------------------------------
//vytvoøí defaultní øadek se zakázkou
void TForm_definice_zakazek:: predvypln_default_zakazku()
{
	rStringGridEd1->Cells[0][1]="1";
	rStringGridEd1->Cells[1][1]="1";
	rStringGridEd1->Cells[2][1]="Nova zakazka";
	rStringGridEd1->Cells[3][1]="Èervená";
	rStringGridEd1->Cells[4][1]="100";  //pomer
	rStringGridEd1->Cells[5][1]="NASTAVIT"; //tlaèítko pro nastavení JIG
	rStringGridEd1->Cells[6][1]="200";
	rStringGridEd1->Cells[7][1]="5";
	rStringGridEd1->Cells[8][1]="50";
	rStringGridEd1->Cells[9][1]="NASTAVIT";//tlaèítko pro nastavení ceesty
	rStringGridEd1->Cells[10][1]="2";
}
//----------------------------------------------------------------------------
//pøedvyplnìní default cesta
//void TForm_definice_zakazek::predvypln_cestu()
//již se nepoužívá plní se z default_cestu() dat tak jako uživatlsky definované zakázky
//{
//	Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
//	int i=0;
//	while(objekt!=NULL)
//	{
//		i++;
//		Form_cesty->rStringGridEd_cesty->Cells[0][i]=i;//poøadí
//		Form_cesty->rStringGridEd_cesty->Cells[1][i]=objekt->name;
//		if(objekt->rezim==1)//pokud je kontinální tak CT spoèítá sám
//			Form_cesty->rStringGridEd_cesty->Cells[2][i]=60*objekt->delka_dopravniku/objekt->pohon->rychlost_od;//CT
//		else
//			Form_cesty->rStringGridEd_cesty->Cells[2][i]="0";//CT
//		Form_cesty->rStringGridEd_cesty->Cells[3][i]=objekt->pohon->rychlost_od;//Rychlost dopravniku, minimální ze zadaného rozmezí
//
//		//pokud jde o lakovani, predvyplnim hodnoty jinak jsou 0 pro ostatni objekty
//		//!!!!nedìlat pro LAK ale pro objekt typu resp ID odpovídající lak
//		if(objekt->short_name=="LAK")
//		{
//			Form_cesty->rStringGridEd_cesty->Cells[4][i]="0";  //Cas vymeny
//			Form_cesty->rStringGridEd_cesty->Cells[5][i]="0";  //Cas cisteni
//			Form_cesty->rStringGridEd_cesty->Cells[6][i]="0";  //Opakovani
//			Form_cesty->rStringGridEd_cesty->Cells[7][i]="Ano"; //priznak pruchodu cesty
//		}
//		else
//		{
//			Form_cesty->rStringGridEd_cesty->Cells[4][i]="0";  //Cas vymeny
//			Form_cesty->rStringGridEd_cesty->Cells[5][i]="0";  //Cas cisteni
//			Form_cesty->rStringGridEd_cesty->Cells[6][i]="0";  //Opakovani
//			Form_cesty->rStringGridEd_cesty->Cells[7][i]="Ano";  //priznak pruchodu cesty
//		}
//
//		Form_cesty->rStringGridEd_cesty->RowCount=i+1;
//		objekt=objekt->dalsi;
//	}
//}
// ---------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Defaulttemp_zakazku()
{
	Cvektory::TJig j;
	j.sirka=1;j.delka=1;j.vyska=1;j.ks=1;//defaultní hodnoty jigu
	Form1->d.v.vloz_temp_zakazku
	(
		rStringGridEd1->Cells[0][1],
		rStringGridEd1->Cells[1][1].ToInt(),
		rStringGridEd1->Cells[2][1],
		clRed,
		Form1->ms.MyToDouble(rStringGridEd1->Cells[4][1]),
		Form1->ms.MyToDouble(rStringGridEd1->Cells[10][1]),
		j,
		rStringGridEd1->Cells[6][1].ToInt(),
		rStringGridEd1->Cells[7][1].ToInt(),
		rStringGridEd1->Cells[8][1].ToInt()
	);
}
//----------------------------------------------------------------------------
void TForm_definice_zakazek::uloz_Default_cestu()
{
	 Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
	 Cvektory::TZakazka *default_zakazka=Form1->d.v.vrat_temp_zakazku(1);
	 Form1->d.v.inicializace_cesty(default_zakazka);
	 int i=0;
	 while(objekt!=NULL)
	 {  //vložení defaulní cesty                                                                //v tomto pøípadì zámìrnì takto protože se pøebírá vypoètená hodnota
			Form1->d.v.vloz_segment_cesty(default_zakazka,/*sloupec poøadí se neukládá*/objekt->n,Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][++i]),0,0,objekt->pohon->rychlost_od,0);
			objekt=objekt->dalsi;
	 }
}
//---------------------------------------------------------------------------
//naètení hotových zakázek, tato metoda je volána z FormShow(), pokud existují zakázky
void TForm_definice_zakazek::nacti_zakazky()
{
		////vyplnìní stringgridu
		Cvektory::TZakazka *ukaz=Form1->d.v.ZAKAZKY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
		int	i=0;
		//Memo4->Lines->Add(AnsiString(ukaz->id));
		while (ukaz!=NULL)
		{
			i++;

			//naètení barvy
			rStringGridEd1->Cells[3][i] = ukaz->barva;//pokud je uživatelská
			//nebo pokud je pøednastavená
			if(ukaz->barva==(TColor)RGB(128,0,0)) rStringGridEd1->Cells[3][i]="Kaštanová";
			if(ukaz->barva==(TColor)RGB(0,128,0)) rStringGridEd1->Cells[3][i]="Zelená";
			if(ukaz->barva==(TColor)RGB(128,128,0)) rStringGridEd1->Cells[3][i]="Olivová";
			if(ukaz->barva==(TColor)RGB(0,0,128)) rStringGridEd1->Cells[3][i]="Námoønická modrá";
			if(ukaz->barva==(TColor)RGB(128,0,128)) rStringGridEd1->Cells[3][i]="Fialová";
			if(ukaz->barva==(TColor)RGB(0,128,128)) rStringGridEd1->Cells[3][i]="Modrozelená";
			if(ukaz->barva==(TColor)RGB(128,128,128)) rStringGridEd1->Cells[3][i]="Šedá";
			if(ukaz->barva==(TColor)RGB(192,192,192)) rStringGridEd1->Cells[3][i]="Støíbrná";
			if(ukaz->barva==(TColor)RGB(255,0,0)) rStringGridEd1->Cells[3][i]="Èervená";
			if(ukaz->barva==(TColor)RGB(0,255,0)) rStringGridEd1->Cells[3][i]="Svìtle zelená";
			if(ukaz->barva==(TColor)RGB(255,255,0)) rStringGridEd1->Cells[3][i]="Žlutá";
			if(ukaz->barva==(TColor)RGB(0,0,255)) rStringGridEd1->Cells[3][i]="Modrá";
			if(ukaz->barva==(TColor)RGB(255,0,255)) rStringGridEd1->Cells[3][i]="Rùžová";
			if(ukaz->barva==(TColor)RGB(0,255,255)) rStringGridEd1->Cells[3][i]="Svìtle modrá";
			if(ukaz->barva==(TColor)RGB(255,255,255)) rStringGridEd1->Cells[3][i]="Bílá";
			if(ukaz->barva==(TColor)RGB(192,220,192)) rStringGridEd1->Cells[3][i]="Zelenomodrá";
			if(ukaz->barva==(TColor)RGB(166,202,240)) rStringGridEd1->Cells[3][i]="Blankytnì Modrá";
			if(ukaz->barva==(TColor)RGB(255,251,240)) rStringGridEd1->Cells[3][i]="Krémová";
			if(ukaz->barva==(TColor)RGB(160,160,164)) rStringGridEd1->Cells[3][i]="Støednì šedá";

			//naètení zbylých dat
			rStringGridEd1->Cells[0][i] = ukaz->id;
			rStringGridEd1->Cells[1][i] = ukaz->typ;
			//ShowMessage(ukaz->name);
			rStringGridEd1->Cells[2][i] = ukaz->name;
			//rStringGridEd1->Cells[3][i] - barva se nastavuje výše
			rStringGridEd1->Cells[4][i] = ukaz->pomer;
			rStringGridEd1->Cells[5][i] ="NASTAVIT";
			rStringGridEd1->Cells[6][i] = ukaz->pocet_voziku;
			rStringGridEd1->Cells[7][i] = ukaz->serv_vozik_pocet;
			rStringGridEd1->Cells[8][i] = ukaz->opakov_servis;
			rStringGridEd1->Cells[9][i] ="NASTAVIT";
			rStringGridEd1->Cells[10][i] = ukaz->TT;

			//vložím další øádek (resp. navýším jejich poèítadlo, poèítá se od 1)
			rStringGridEd1->RowCount=i+1; //zvysuji podle poctu nacitanych zakazek + 1 kvuli hlavicce tabulky


			//posun na další prvek v seznamu
			ukaz=ukaz->dalsi;
		}

		//uložení ZAKAZKY do ZAKAZKYtemp, aby temp obsahoval stejná data jako jsou zobrazena
		Form1->d.v.kopirujZAKAZKY2ZAKAZKY_temp();//pouze ZAKAZKY_temp=ZAKAZKY
}
//----------------------------------------------------------------------------
//volání sub formuláøù JIG a CESTA a pøípadné ukládání do dat
void __fastcall TForm_definice_zakazek::rStringGridEd1Click(TObject *Sender)
{
	//definice ukazatele na aktuálnì editovanou zakázku     //èíslo øádku kde kliku došlo
	Cvektory::TZakazka *zakazka=Form1->d.v.vrat_temp_zakazku(rStringGridEd1->Row);
	AnsiString prochazet;

	////////////jig form- byl klik na buòku v 5. sloupci
	if(rStringGridEd1->Col==5)
	{
		//naplnìní dat
		Form_jig->Edit_jig_pocet_ks->Text=zakazka->jig.ks;
		Form_jig->Edit_jig_delka->Text=zakazka->jig.delka;
		Form_jig->Edit_jig_sirka->Text=zakazka->jig.sirka;
		Form_jig->Edit_jig_vyska->Text=zakazka->jig.vyska;
		//zobrazení formu + uložení dat pokud je zvoleno OK
		if(mrOk==Form_jig->ShowModal())
		{
			zakazka->jig.ks=Form1->ms.MyToDouble(Form_jig->Edit_jig_pocet_ks->Text);
			zakazka->jig.delka=Form1->ms.MyToDouble(Form_jig->Edit_jig_delka->Text);
			zakazka->jig.sirka=Form1->ms.MyToDouble(Form_jig->Edit_jig_sirka->Text);
			zakazka->jig.vyska=Form1->ms.MyToDouble(Form_jig->Edit_jig_vyska->Text);
		}
	}

	////////////cesty form - byl klik na buòku 9. sloupci
	if(rStringGridEd1->Col==9)
	{
		//Form_cesty->rStringGridEd_cesty->Columns->Clear();
		////naplnìní picklistu
		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Clear();
		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ano");
		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ne");

		////naèítání dat - není tøeba ošetøovat, protože existuje default cesta v seznamu
		//vypíše døíve nadefinovanou cestu vèetnì segmentù z nejdelší možné cesty, které nejsou souèástí aktuální cesty
		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
		//pøídání poètu øádkù
		Form_cesty->rStringGridEd_cesty->RowCount=Form1->d.v.OBJEKTY->predchozi->n+1;//pøidání dalšího øádku     //pridani k zobrazeni if ukaz param
		int i=0;
		while(O!=NULL)//prochází všechnyobjekty a buï je ("Ano") objekt i na cestì nebo není ("Ne")
		{
			i++;
			Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,zakazka);
			if(C!=NULL)//zahrnuté segmenty cesty
			{
					Form_cesty->rStringGridEd_cesty->Cells[0][i]=C->objekt->n;
					Form_cesty->rStringGridEd_cesty->Cells[1][i]=C->objekt->name;
					Form_cesty->rStringGridEd_cesty->Cells[2][i]=C->CT;
					Form_cesty->rStringGridEd_cesty->Cells[3][i]=C->RD;
					Form_cesty->rStringGridEd_cesty->Cells[4][i]=C->Tv;
					Form_cesty->rStringGridEd_cesty->Cells[5][i]=C->Tc;
					Form_cesty->rStringGridEd_cesty->Cells[6][i]=C->Opak;
					Form_cesty->rStringGridEd_cesty->Cells[7][i]="Ano";
			}
			else //nezahrnutý segment nejdelší cesty v aktuální cestì
			{
				 Form_cesty->rStringGridEd_cesty->Cells[0][i]=O->n;
				 Form_cesty->rStringGridEd_cesty->Cells[1][i]=O->name;
				 Form_cesty->rStringGridEd_cesty->Cells[2][i]="0";
				 Form_cesty->rStringGridEd_cesty->Cells[3][i]="0";
				 Form_cesty->rStringGridEd_cesty->Cells[4][i]="0";
				 Form_cesty->rStringGridEd_cesty->Cells[5][i]="0";
				 Form_cesty->rStringGridEd_cesty->Cells[6][i]="0";
				 Form_cesty->rStringGridEd_cesty->Cells[7][i]="Ne";
			}
			O=O->dalsi;
		}

		////--------------

		////ukládání dat - jednotlivého segmentu cesty pokud je považován k zahrnutní ze strany uživatele
		//formuláø na støed
		Form_cesty->Left=Form1->ClientWidth/2-Form_cesty->Width/2;
		Form_cesty->Top=Form1->ClientHeight/2-Form_cesty->Height/2;
		if(mrOk==Form_cesty->ShowModal())
		{
			Form1->d.v.inicializace_cesty(zakazka);
			for(int i=1;i<Form_cesty->rStringGridEd_cesty->RowCount;i++)
			{
				if(Form_cesty->rStringGridEd_cesty->Cells[7][i]=="Ano")//pokud je zaškrnuto neprocházek objekt se neuloží do cesty
				{
						Form1->d.v.vloz_segment_cesty
						(
							zakazka,
							/*sloupec poøadí se neukládá*/ /*pozor na øazení*/
							Form_cesty->rStringGridEd_cesty->Cells[0][i].ToInt(),//ID-doøešit
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][i]),//CT
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[5][i]),//RD
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[4][i]),//Tv
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[3][i]),//Tc
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[6][i])//Opak   //ulozeni stavu pro cestu - roletka
						);
				}
				//vymazání textu z již nepotøebného øádku
				Form_cesty->rStringGridEd_cesty->Rows[i]->Clear();
			}
		}
	}
}
//---------------------------------------------------------------------------
//TLAÈÍTKO ULOŽIT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
		Form1->d.v.PP.mnozstvi=Form1->ms.MyToDouble(rEditNum_pozad_mnozstvi->Text);
		Form1->d.v.PP.dni_rok=Form1->ms.MyToDouble(rEditNum_pocet_dnu->Text);
		Form1->d.v.PP.efektivita=Form1->ms.MyToDouble(rEditNum_effektivita->Text);
		Form1->d.v.PP.hod_den=Form1->ms.MyToDouble(rEditNum_pocet_prac_hod->Text);
		Form1->d.v.PP.cas_start=TDateTime(scEdit_zacatek->Text);


		TColor barva;//musí být v TColor kvùli ukládání do TColor objektu!!!

		//uložení editovaných zakázek
		for (int i = 1; i< rStringGridEd1->RowCount; i++)
		{
			bool usersColor=true;
			//konverze pøednastavených barev
			if(rStringGridEd1->Cells[3][i]=="Kaštanová"){barva=(TColor)RGB(128,0,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Zelená"){barva=(TColor)RGB(0,128,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Olivová"){barva=(TColor)RGB(128,128,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Námoønická modrá"){barva=(TColor)RGB(0,0,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Fialová"){barva=(TColor)RGB(128,0,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Modrozelená"){barva=(TColor)RGB(0,128,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Šedá"){barva=(TColor)RGB(128,128,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Støíbrná"){barva=(TColor)RGB(192,192,192);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Èervená"){barva=(TColor)RGB(255,0,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Svìtle zelená"){barva=(TColor)RGB(0,255,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Žlutá"){barva=(TColor)RGB(255,255,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Modrá"){barva=(TColor)RGB(0,0,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Rùžová"){barva=(TColor)RGB(255,0,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Svìtle modrá"){barva=(TColor)RGB(0,255,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Bílá"){barva=(TColor)RGB(255,255,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Zelenomodrá"){barva=(TColor)RGB(192,220,192);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Blankytnì Modrá"){barva=(TColor)RGB(166,202,240);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Krémová"){barva=(TColor)RGB(255,251,240);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Støednì šedá"){ barva=(TColor)RGB(160,160,164);usersColor=false;}
			if(usersColor)barva=(TColor)rStringGridEd1->Cells[3][i].ToInt();//musí být až tady jinak spadne

			//uložení aktuálních hodnot do dané temp_zakazky
			Form1->d.v.edituj_temp_zakazku
			(
				i,//n
				rStringGridEd1->Cells[0][i],//ID
				rStringGridEd1->Cells[1][i].ToInt(),//TYP
				rStringGridEd1->Cells[2][i],//NAME
				barva,//COLOR
				Form1->ms.MyToDouble(rStringGridEd1->Cells[4][i]),//POMER
				Form1->ms.MyToDouble(rStringGridEd1->Cells[10][i]),//TT
				//JIG se už jako parametr nepožaduje, stejnì jako cesta, jedná se o pøedávání ukazatelem pøi zavírání patøièného formuláøe
				rStringGridEd1->Cells[6][i].ToInt(),//n-vozíku
				rStringGridEd1->Cells[7][i].ToInt(),//n-servis vozíkù
				rStringGridEd1->Cells[8][i].ToInt()//n-opak vozíkù
			);
		}
		//samotné uložení
		Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();//uložení do ostrého spojáku ZAKAZKY+smáznutí ZAKAZKY_temp
		Form1->d.v.generuj_VOZIKY();//vygenerování vozíkù dle zadaných zakázek
		Form_definice_zakazek->Close();//zavøení formuláøe s následným DuvodUlozit(true); po modalshow v unit1
		Form1->REFRESH();//pøekreslení kvùli zobrazení aktuálních cest
}
//---------------------------------------------------------------------------
//Zavøení formuláøe (storno a køížek je to samé)
void __fastcall TForm_definice_zakazek::scGPGlyphButton4Click(TObject *Sender)
{
	 KonecClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::KonecClick(TObject *Sender)
{
	 for(int i=1;i<=rStringGridEd1->RowCount;i++)
	 {
		rStringGridEd1->Rows[i]->Clear();   //promaznuti radku, ktere nebudou ulozeny
	 }
	 Form1->d.v.vymaz_seznam_ZAKAZKY_temp();
	 Form_definice_zakazek->Close();
}
//----------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//ZDM
//void TForm_definice_zakazek::hlavicka_barva()
//{
//	TBarva *novy_uzel = new TBarva;
//
//	novy_uzel->id_radek = 0;
//	novy_uzel->barva_voziku = ColorDialog1->Color;
//
//	novy_uzel->predchozi = novy_uzel; // ukazuje sam na sebe
//	novy_uzel->dalsi = NULL;
//	BARVY = novy_uzel;
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::uloz_barvu()
//{
//	TBarva *novy = new TBarva;
//
//	novy->id_radek = RzStringGrid1->Row; // navýším poèítadlo prvku o jednièku
//	novy->barva_voziku = ColorDialog1->Color;
//
//	BARVY->predchozi->dalsi = novy;
//	// poslednímu prvku pøiøadím ukazatel na nový prvek
//	novy->predchozi = BARVY->predchozi;
//	// novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi = NULL;
//	BARVY->predchozi = novy;
//	// nový poslední prvek zápis do hlavièky,body->predchozi zápis do hlavièky odkaz na poslední prvek seznamu "predchozi" v tomto pøípadì zavádìjicí
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::uloz_barvu(TColor barva, int i)
//{
//	TBarva *novy = new TBarva;
//
//	// ShowMessage(barva);
//
//	novy->id_radek = i; // navýším poèítadlo prvku o jednièku
//	novy->barva_voziku = barva;
//
//	BARVY->predchozi->dalsi = novy;
//	// poslednímu prvku pøiøadím ukazatel na nový prvek
//	novy->predchozi = BARVY->predchozi;
//	// novy prvek se odkazuje na prvek predchozí (v hlavicce body byl ulozen na pozici predchozi, poslední prvek)
//	novy->dalsi = NULL;
//	BARVY->predchozi = novy;
//	// nový poslední prvek zápis do hlavièky,body->predchozi zápis do hlavièky odkaz na poslední prvek seznamu "predchozi" v tomto pøípadì zavádìjicí
//
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::aktualizuj_barvu(TColor barva, int row)
//{
//	TBarva *ukaz = BARVY->dalsi;
//	// ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
//	// uloží do "spojáèku"
//
//	while (ukaz != NULL) {
//	  //	ShowMessage(ukaz->id_radek);
//		//	ShowMessage(row);
//
//		if (ukaz->id_radek == row) {
//			ukaz->barva_voziku = barva;
//		  //	ShowMessage(ukaz->id_radek);
//          //  ShowMessage(row);
//
//			break;
//		}
//
//		ukaz = ukaz->dalsi;
//	}
//
//}
//// ---------------------------------------------------------------------------
//// smaze barvu ze seznamu
//void TForm_definice_zakazek::smaz_barvu_radku(TBarva *Barva)
//{
//	// vyøazení prvku ze seznamu a napojení prvku dalšího na prvek pøedchozí prku mazaného
//	if (Barva->dalsi != NULL) // ošetøení proti poslednímu prvku
//	{
//		Barva->predchozi->dalsi = Barva->dalsi;
//		Barva->dalsi->predchozi = Barva->predchozi;
//	}
//	else // poslední prvek
//	{
//		if (Barva->id_radek == 1) // pokud je mazaný prvek hned za hlavièkou
//		{
//			BARVY->predchozi = Barva->predchozi;
//			// popø hlavièka bude ukazovat sama na sebe
//			BARVY->dalsi = NULL;
//		}
//		else {
//			Barva->predchozi->dalsi = NULL;
//			BARVY->predchozi = Barva->predchozi;
//			// zapis do hlavièky poslední prvek seznamu
//		}
//	}
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::sniz_indexy(TBarva *Barva) {
//	while (Barva != NULL) {
//		Barva = Barva->dalsi; // posun na další prvek
//		if (Barva != NULL)
//			Barva->id_radek--;
//		// sníží indexy nasledujicích bodù,protože optimalizace seznamu nefungovalo, navíc ušetøím strojový èas
//	}
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::vymaz_barvu() {
//	long pocet_smazanych_objektu = 0;
//	while (BARVY != NULL) {
//		pocet_smazanych_objektu++;
//		BARVY->predchozi = NULL;
//		delete BARVY->predchozi;
//		BARVY = BARVY->dalsi;
//	};
//
//};
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::nacti_voziky()
//{//	hlavicka_barva();
//
//	OK_status = false;
//	RowMoved = false;
//	data_nalezena = false;
//	// pokud je false - ve spojaku nejsou data, takze nize nastavim defaultni hodnoty pro pridani prvniho voziku
//	Button_DEL->Visible = false;
//
//	RzStringGrid1->Cols[0]->Add("Vozík");
//	RzStringGrid1->Cols[0]->Add("1");
//	RzStringGrid1->Rows[0]->Add("ID");
//	RzStringGrid1->Rows[0]->Add("Délka");
//	RzStringGrid1->Rows[0]->Add("Šíøka");
//	RzStringGrid1->Rows[0]->Add("Cesta"); //použito pro trasu vozíku
//	RzStringGrid1->Rows[0]->Add("Rotace");
//	RzStringGrid1->Rows[0]->Add("Název");
//	RzStringGrid1->Rows[0]->Add("Max ks");
//	RzStringGrid1->Rows[0]->Add("Poèet ks");
//	RzStringGrid1->Rows[0]->Add("Délka Max");
//	RzStringGrid1->Rows[0]->Add("Šíøka Max");
//	RzStringGrid1->Rows[0]->Add("Výška Max");
//	RzStringGrid1->Rows[0]->Add("Barva");
//	RzStringGrid1->Rows[0]->Add("Stav");
//
//
// /*	rStringGridEd1->Cols[0]->Add("Zakázka");
//	rStringGridEd1->Cols[0]->Add("1");
//	rStringGridEd1->Rows[0]->Add("ID");
//	rStringGridEd1->Rows[0]->Add("Typ");
//	rStringGridEd1->Rows[0]->Add("Název");
//	rStringGridEd1->Rows[0]->Add("Barva");
//	rStringGridEd1->Rows[0]->Add("Pomìr");
//	rStringGridEd1->Rows[0]->Add("Jigy");
//	rStringGridEd1->Rows[0]->Add("Poèet");
//	rStringGridEd1->Rows[0]->Add("Poè.serv.voz");
//	rStringGridEd1->Rows[0]->Add("Opakování");
//	rStringGridEd1->Rows[0]->Add("Technologie");
//	rStringGridEd1->Rows[0]->Add("Takt");
//				*/
//
//
//
//		 //	scGPButton5->Options->FocusedColor=(TColor)RGB(255,140,0);
//		 //	scGPButton5->Options->NormalColor=(TColor)RGB(255,140,0);
//
//
//
//
//
//
//
//
//
//	Color_status = false;
//
//	if (!data_nalezena) // ve spojaku nejsou data, pripravim si novy radek
//	{ // default hodnoty
//		RzStringGrid1->RowCount = 1;
//
//		RzStringGrid1->Cells[0][1] = RzStringGrid1->RowCount++;
//		RzStringGrid1->Cells[1][1] = RzStringGrid1->RowCount + 1;
//		RzStringGrid1->Cells[2][1] = "1";
//		RzStringGrid1->Cells[3][1] = "1";
//		RzStringGrid1->Cells[4][1] = "1";
//		RzStringGrid1->Cells[5][1] = "0";
//		RzStringGrid1->Cells[6][1] = "název";
//		RzStringGrid1->Cells[7][1] = "30";
//		RzStringGrid1->Cells[8][1] = "30";
//		RzStringGrid1->Cells[9][1] = "1";
//		RzStringGrid1->Cells[10][1] = "1";
//		RzStringGrid1->Cells[11][1] = "1";
//
//		// defaultni barvy kreslim ve drawcell metode
//	}
//
//	Cvektory::TVozik *ukaz = Form1->d.v.VOZIKY->dalsi;
//	// ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
//	if (ukaz != NULL) {
//
//		RzStringGrid1->RowCount = Form1->d.v.VOZIKY->predchozi->n + 1;
//
//		for (int i = 1; i < RzStringGrid1->RowCount; i++) {
//
//			data_nalezena = true;
//			// Memo3->Lines->Add(AnsiString("HlSpojak id disabled:") + ukaz->n);
//			// akce s ukazatelem
//			RzStringGrid1->Cells[0][i] = ukaz->n;
//			RzStringGrid1->Cells[1][i] = ukaz->id;
//			RzStringGrid1->Cells[2][i] = ukaz->delka;
//			RzStringGrid1->Cells[3][i] = ukaz->sirka;
//			RzStringGrid1->Cells[4][i] = ukaz->vyska;  //cesta vozíku  prozatím
//			RzStringGrid1->Cells[5][i] = ukaz->rotace;
//			RzStringGrid1->Cells[6][i] = ukaz->nazev_vyrobku;
//			RzStringGrid1->Cells[7][i] = ukaz->max_vyrobku;
//			RzStringGrid1->Cells[8][i] = ukaz->akt_vyrobku;
//			RzStringGrid1->Cells[9][i] = ukaz->delka_vcetne_vyrobku;
//			RzStringGrid1->Cells[10][i] = ukaz->sirka_vcetne_vyrobku;
//			RzStringGrid1->Cells[11][i] = ukaz->vyska_vcetne_vyrobku;
//
//			uloz_barvu(ukaz->barva, i);
//
//			// posun na další prvek v seznamu
//			ukaz = ukaz->dalsi;
//		}
//	}
//	else { // když ve spojáku nic není, nastavím do ID defaultnì ID=1
//
//		RzStringGrid1->Cells[1][1] = RzStringGrid1->RowCount - 1;
//		RzStringGrid1->RowCount = 1;
//
//	}
//}
// --------------------------------------------------------------------------- -------------------------------------
/* ZDM
void __fastcall TForm_definice_zakazek::FormCloseQuery(TObject *Sender,	bool &CanClose)
{
	ZDM
	if (OK_status && RzStringGrid1->RowCount >= 2)
	{
		uloz_voziky_a_nastav_zakazky();
	}
	vymaz_barvu();

}*/
// ---------------------------------------------------------------------------
//ZDM
//void TForm_definice_zakazek::uloz_voziky_a_nastav_zakazky()
//{
//		Form1->d.v.vymaz_seznam_voziku();// smazu stary spojak pred ulozenim noveho
//		Form1->d.v.hlavicka_voziky();// vytvorim si hlavicku pro zavedeni noveho spojaku
//
//		TBarva *ukaz = BARVY->dalsi;
//		for (int i = 1; i < RzStringGrid1->RowCount; i++)
//		{
//			//uložení parametrù vozíkù
//			Form1->d.v.vloz_vozik(RzStringGrid1->Cells[0][i].ToInt(),
//			RzStringGrid1->Cells[1][i],
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[2][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[3][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[4][i]), //sem ukladám výšku
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[5][i]),
//			RzStringGrid1->Cells[6][i],
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[7][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[8][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[9][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[10][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[11][i]),
//			ukaz->barva_voziku,
//			Form1->d.v.vrat_cestu(Form1->ms.MyToDouble(RzStringGrid1->Cells[4][i]))
//			);
//
//			//uloží do cesty barvu cesty-zakázky dle vozíku
//			Form1->d.v.vrat_cestu(Form1->ms.MyToDouble(RzStringGrid1->Cells[4][i]))->barva=ukaz->barva_voziku;
//
//			//posun na další prvek v seznamu
//			ukaz = ukaz->dalsi; //posun na další prvek v seznamu
//		}
//		Form1->Invalidate();
//}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::RzStringGrid1MouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	int Column, Row;
/* ZDM
	Column = RzStringGrid1->Col;
	Row = RzStringGrid1->Row;

	if (Column == 12 && Row >= 1) {

		if (ColorDialog1->Execute()) {

			if (RzStringGrid1->RowCount == 2) {

				// Memo2->Lines->Add(AnsiString("RADEK_ID_uloz:") + RzStringGrid1->Row);

				RzStringGrid1->Canvas->Brush->Color = ColorDialog1->Color;
				RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(12,
					RzStringGrid1->Row));
				// ShowMessage(ColorDialog1->Color);
				aktualizuj_barvu(ColorDialog1->Color, RzStringGrid1->Row);
			}
			else {
				// aktualizuj_barvu(ColorDialog1->Color, RzStringGrid1->Row);
				RzStringGrid1->Canvas->Brush->Color = ColorDialog1->Color;
				RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(12,
					RzStringGrid1->Row));
				aktualizuj_barvu(ColorDialog1->Color, RzStringGrid1->Row);

				Color_status = true;
			}
		}

	}
*/
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::RzStringGrid1DrawCell(TObject *Sender,
	int ACol, int ARow, TRect &Rect, TGridDrawState State) {
/*ZDM
	RzStringGrid1->FixedCols = 1;

	RzStringGrid1->FocusColor = clWhite;

	TForm_definice_zakazek::TBarva *ukaz2 = BARVY->dalsi;

	if (ukaz2 == NULL && RzStringGrid1->RowCount == 2)
		// pokracovani defaultnich hodnot pro novy vozik
	{

		RzStringGrid1->Canvas->Brush->Color = clBlue;
		uloz_barvu(RzStringGrid1->Canvas->Brush->Color, 1);
		RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(12, 1));

		RzStringGrid1->Canvas->Brush->Color = clWhite;
		RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(13, 1));

	}

	if (RowMoved) {
		for (int i = 1; i < RzStringGrid1->RowCount; i++)
			RzStringGrid1->Cells[0][i] = i;
		RowMoved = false;
	}

	TForm_definice_zakazek::TBarva *ukaz = BARVY->dalsi;
	Cvektory::TVozik *ukaz1 = Form1->d.v.VOZIKY->dalsi;
	// ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku

	while (ukaz != NULL)
	{

		// ShowMessage("uka barvu");
		RzStringGrid1->Canvas->Brush->Color = ukaz->barva_voziku;
		RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(12,
			ukaz->id_radek));
*/
//ZDM
//		if (ukaz1 != NULL) {
//			switch (ukaz1->stav) {
//
//			default:
//				RzStringGrid1->Canvas->Brush->Color = clWhite;
//				break;
//			case 0:
//				RzStringGrid1->Canvas->Brush->Color = clRed;
//				break;
//			case 1:
//				RzStringGrid1->Canvas->Brush->Color = clYellow;
//				break;
//			case 2:
//				RzStringGrid1->Canvas->Brush->Color = clGreen;
//				break;
//			}
//
//			RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(13,
//				ukaz->id_radek));
//			ukaz1 = ukaz1->dalsi;
//		}
//		else
//		{
//
//			RzStringGrid1->Canvas->Brush->Color = clWhite;
//			// natvrdo nastavena bila pri nove zadanem voziku
//			RzStringGrid1->Canvas->FillRect(RzStringGrid1->CellRect(13,
//				ukaz->id_radek));
//		}
//
//		ukaz = ukaz->dalsi; // posun na další prvek v seznamu
//
//	}

}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button1Click(TObject *Sender) {
	//RzStringGrid1->ColCount++;
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button2Click(TObject *Sender) {
	//RzStringGrid1->ColCount--;
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button3Click(TObject *Sender)

{
//ZDM
//	RzStringGrid1->RowCount++;
//	RzStringGrid1->Cols[0]->Add(RzStringGrid1->RowCount - 1);
//
//	if (RzStringGrid1->RowCount > 2) {
//		TBarva *ukaz = BARVY->predchozi;
//
//		int i = RzStringGrid1->RowCount - 1;
//
//		RzStringGrid1->Cells[1][i] = i;
//		RzStringGrid1->Cells[2][i] = RzStringGrid1->Cells[2][i - 1];
//		RzStringGrid1->Cells[3][i] = RzStringGrid1->Cells[3][i - 1];
//		RzStringGrid1->Cells[4][i] = RzStringGrid1->Cells[4][i - 1];
//		RzStringGrid1->Cells[5][i] = RzStringGrid1->Cells[5][i - 1];
//		RzStringGrid1->Cells[6][i] = RzStringGrid1->Cells[6][i - 1];
//		RzStringGrid1->Cells[7][i] = RzStringGrid1->Cells[7][i - 1];
//		RzStringGrid1->Cells[8][i] = RzStringGrid1->Cells[8][i - 1];
//		RzStringGrid1->Cells[9][i] = RzStringGrid1->Cells[9][i - 1];
//		RzStringGrid1->Cells[10][i] = RzStringGrid1->Cells[10][i - 1];
//		RzStringGrid1->Cells[11][i] = RzStringGrid1->Cells[11][i - 1];
//		RzStringGrid1->Cells[12][i] = RzStringGrid1->Cells[12][i - 1];
//		uloz_barvu(ukaz->barva_voziku, i);
//
//	}

}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::RzStringGrid1RowMoved(TObject *Sender,
	int FromIndex, int ToIndex) {
//	TColor ColorFromIndex, ColorToIndex;
//	TBarva *ukaz = BARVY->dalsi;
//
//	while (ukaz != NULL) {
//		if (ukaz->id_radek == FromIndex)
//			ColorFromIndex = ukaz->barva_voziku;
//		if (ukaz->id_radek == ToIndex)
//			ColorToIndex = ukaz->barva_voziku;
//		ukaz = ukaz->dalsi; // posun na další prvek v seznamu
//	}
//
//	TBarva *ukaz1 = BARVY->dalsi;
//	while (ukaz1 != NULL) {
//		if (ukaz1->id_radek == FromIndex)
//			ukaz1->barva_voziku = ColorToIndex;
//		if (ukaz1->id_radek == ToIndex)
//			ukaz1->barva_voziku = ColorFromIndex;
//		ukaz1 = ukaz1->dalsi; // posun na další prvek v seznamu
//	}
//
//	RowMoved = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Memo_spojakClick(TObject *Sender) {
//
//	TBarva *ukaz = BARVY->dalsi;
//	while (ukaz != NULL) {
//		Memo1->Lines->Add(AnsiString("id_radek: ") + ukaz->id_radek +
//			AnsiString(" barva: ") + ukaz->barva_voziku);
//
//		ukaz = ukaz->dalsi;
//	}
}

// ---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::RzStringGrid1GetEditMask(TObject *Sender,
	int ACol, int ARow, UnicodeString &Value)

{ // maska pro validaci zaznamu

	if (RzStringGrid1->Col == 1) // ID   - rùzné alfanumerické znaky
			Value = "AAAAAAAAAA";

	if (RzStringGrid1->Col == 2) // Delka  - sežere pouze èíslo
			Value = "0000";

	if (RzStringGrid1->Col == 3) // Sirka
			Value = "0000";

	if (RzStringGrid1->Col == 4) // Vyska
			Value = "0000";

	if (RzStringGrid1->Col == 5) // Rotace
			Value = "00";

	if (RzStringGrid1->Col == 6) // Nazev
			Value = "AAAAAAA";

	if (RzStringGrid1->Col == 7) // Max ks
			Value = "0000";

	if (RzStringGrid1->Col == 8) // Akt kusu
			Value = "0000";

	if (RzStringGrid1->Col == 9) // Max delka
			Value = "0000";
	if (RzStringGrid1->Col == 10) // Max sirka
			Value = "0000";
	if (RzStringGrid1->Col == 11) // Max vyska
			Value = "0000";

}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button_DELClick(TObject *Sender) {

	

//
//	// Tlaèítko na smazání øádku
//	RzStringGrid1->Rows[RzStringGrid1->Row]->Clear();
//	rStringGridEd1->Rows[rStringGridEd1->Row]->Clear();
//
//	TForm_definice_zakazek::TBarva *ukaz = BARVY->dalsi;
//	// ukazatel na první objekt v seznamu OBJEKTU, pøeskoèí hlavièku
//	while (ukaz != NULL) {
//
//		if (ukaz->id_radek == RzStringGrid1->Row) {
//			smaz_barvu_radku(ukaz);
//			sniz_indexy(ukaz);
//
//			// ukaz=NULL;
//			// delete ukaz;  //smazani ukazatele nejak nefungovalo
//			break;
//
//		}
//		// posun na další prvek v seznamu
//		ukaz = ukaz->dalsi;
//	}
//
//	// posun øádkù
//
//	for (int row = RzStringGrid1->Row; row < RzStringGrid1->RowCount - 1; ++row)
//
//	{
//		RzStringGrid1->Rows[row] = RzStringGrid1->Rows[row + 1];
//		//  Memo1->Lines->Add(row);
//		//  Memo1->Lines->Add(RzStringGrid1->Row+1);
//		// aktualizuj_barvu(ukaz->barva_voziku,RzStringGrid1->Row+1);
//
//	}
//
//	RzStringGrid1->RowCount--;
//	// vykrleseni noveho poradi (n)
//	for (long i = 1; i < RzStringGrid1->RowCount; i++)
//		RzStringGrid1->Cells[0][i] = i;
//
}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::RzStringGrid1Click(TObject *Sender) {

//	Button_DEL->Visible = true;
//	Button_DEL->Top = RzStringGrid1->Top + RzStringGrid1->Row * 19 + 1;

}
// ---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::Button_OKKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
//	UINT funkcni_klavesa=0;
//	switch(Key)
//	{
//		//BACKSPACE
//		case 8: break;
//		//ENTER
//		case 13:break;
//		//ESC
//		case 27:Close();break;
//		//MEZERNÍK
//		case 32:break;
//		//PAGE UP
//		case 33:;break;
//		//PAGE DOWN
//		case 34:break;
//		//END
//		case 35:break;
//		//HOME
//		case 36:break;
//		//ŠIPKA DOLÙ
//		case 40:break;
//		//ŠIPKA LEVÁ
//		case 37:break;
//		//ŠIPKA PRAVÁ
//		case 39:break;
//		//ŠIPKA NAHORU
//		case 38:break;
//		//ŠIPKA DOLÙ
//		case 98:break;
//		//ŠIPKA LEVÁ
//		case 100:break;
//		//ŠIPKA PRAVÁ
//		case 102:break;
//		//ŠIPKA NAHORU
//		case 104:break;
//		//CTRL, SHIFT
//		default:
//		{
//			if(Shift.Contains(ssShift) && Shift.Contains(ssCtrl)){funkcni_klavesa=3;}//SHIFT + CTRL
//			else
//			{
//				if(Shift.Contains(ssShift)){funkcni_klavesa=2;}//SHIFT
//				if(Shift.Contains(ssCtrl)){funkcni_klavesa=1;}//CTRL
//			}
//			break;
//		}

						 //	ShowMessage("uloz");
//	}
}
//---------------------------------------------------------------------------
//zkopíruje pøedchozí
void __fastcall TForm_definice_zakazek::rButton1Click(TObject *Sender)
{
	rStringGridEd1->RowCount++;
	rStringGridEd1->Cols[0]->Add(rStringGridEd1->RowCount - 1);

	if (rStringGridEd1->RowCount > 2)
	{
		int i = rStringGridEd1->RowCount - 1;

		rStringGridEd1->Cells[1][i] = i;
		rStringGridEd1->Cells[2][i] = rStringGridEd1->Cells[2][i - 1];
		rStringGridEd1->Cells[3][i] = rStringGridEd1->Cells[3][i - 1];
		rStringGridEd1->Cells[4][i] = rStringGridEd1->Cells[4][i - 1];
		rStringGridEd1->Cells[5][i] = rStringGridEd1->Cells[5][i - 1];
		rStringGridEd1->Cells[6][i] = rStringGridEd1->Cells[6][i - 1];
		rStringGridEd1->Cells[7][i] = rStringGridEd1->Cells[7][i - 1];
		rStringGridEd1->Cells[8][i] = rStringGridEd1->Cells[8][i - 1];
		rStringGridEd1->Cells[9][i] = rStringGridEd1->Cells[9][i - 1];
		rStringGridEd1->Cells[10][i] = rStringGridEd1->Cells[10][i - 1];
		rStringGridEd1->Cells[11][i] = rStringGridEd1->Cells[11][i - 1];
		rStringGridEd1->Cells[12][i] = rStringGridEd1->Cells[12][i - 1];
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::scGPGlyphButton_add_zakazkaClick(TObject *Sender)
{
		rStringGridEd1->RowCount++;

		int i=rStringGridEd1->RowCount-1;
		Cvektory::TJig j;
		j.sirka=1;j.delka=1;j.vyska=1;j.ks=1;


		if(i>1) {// defaultní zakázka je uložena hned v temp_spojaku + hlavnim pri form_show, èili tady ukládám až další øádky
	
	rStringGridEd1->Cells[0][i]=i;
	rStringGridEd1->Cells[1][i]="1";
	rStringGridEd1->Cells[2][i]="Nová zakazka";
	rStringGridEd1->Cells[3][i]="Èervena";
	rStringGridEd1->Cells[4][i]="0";  //pomer
	rStringGridEd1->Cells[5][i]="NASTAVIT";
	rStringGridEd1->Cells[6][i]="0";
	rStringGridEd1->Cells[7][i]="0";
	rStringGridEd1->Cells[8][i]="0";
	rStringGridEd1->Cells[9][i]="NASTAVIT";
	rStringGridEd1->Cells[10][i]="0";

	Memo4->Lines->Add(AnsiString(rStringGridEd1->Cells[0][i])+";"
										+AnsiString(rStringGridEd1->Cells[1][i])+";"
										+AnsiString(rStringGridEd1->Cells[2][i])+";"
										+AnsiString(rStringGridEd1->Cells[4][i])+";"
										+AnsiString(rStringGridEd1->Cells[6][i])+";"
										+AnsiString(rStringGridEd1->Cells[7][i])+";"
										+AnsiString(rStringGridEd1->Cells[8][i])+";"
										+AnsiString(rStringGridEd1->Cells[10][i]));


			Form1->d.v.vloz_temp_zakazku(rStringGridEd1->Cells[0][i],
																		rStringGridEd1->Cells[1][i].ToInt(),
																		rStringGridEd1->Cells[2][i],
																		clRed,
																		Form1->ms.MyToDouble(rStringGridEd1->Cells[4][i]),
																		Form1->ms.MyToDouble(rStringGridEd1->Cells[10][i]),
																		j,
																		rStringGridEd1->Cells[6][i].ToInt(),
																		rStringGridEd1->Cells[7][i].ToInt(),
																		rStringGridEd1->Cells[8][i].ToInt());

		int j=rStringGridEd1->RowCount-1;

		 //pøi pøidání další zakázky uložím do cesty defaultní hodnoty
	 Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
	 Cvektory::TZakazka *nova_zakazka=Form1->d.v.vrat_temp_zakazku(j);
	 Form1->d.v.inicializace_cesty(nova_zakazka);
	 while(objekt!=NULL)
	 {  //vložení defaulní cesty
			Form1->d.v.vloz_segment_cesty(nova_zakazka,/*sloupec poøadí se neukládá*/objekt->n,0,0,0,0,0);
			objekt=objekt->dalsi;


	 }

	 }



}
//---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::button_zakazky_tempClick(TObject *Sender)
{
	Cvektory::TZakazka *ukaz=Form1->d.v.ZAKAZKY_temp->dalsi;
	while (ukaz!=NULL)
	{
		Memo4->Lines->Add(AnsiString(ukaz->name)+";"+AnsiString(ukaz->barva)+";"+AnsiString(ukaz->pomer)+";"+AnsiString(ukaz->pocet_voziku)+";"+AnsiString(ukaz->serv_vozik_pocet)+";"+AnsiString(ukaz->opakov_servis)+";"+AnsiString(ukaz->TT));
		ukaz=ukaz->dalsi;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::zakazky_hlavni_spojakClick(TObject *Sender)
{

	Cvektory::TZakazka *ukaz=Form1->d.v.ZAKAZKY->dalsi;
		while (ukaz!=NULL)
		{

	Memo4->Lines->Add(AnsiString(ukaz->name)+";"+AnsiString(ukaz->barva)+";"+AnsiString(ukaz->pomer)+";"+AnsiString(ukaz->pocet_voziku)+";"+AnsiString(ukaz->serv_vozik_pocet)+";"+AnsiString(ukaz->opakov_servis)+";"+AnsiString(ukaz->TT));

	ukaz=ukaz->dalsi;

			 }
}
//---------------------------------------------------------------------------


void __fastcall TForm_definice_zakazek::smaz_tempClick(TObject *Sender)
{
	Form1->d.v.smaz_temp_zakazku(Edit_smaz_temp->Text.ToInt());
}
//---------------------------------------------------------------------------
void __fastcall TForm_definice_zakazek::Button5Click(TObject *Sender)
{
		Memo4->Lines->Clear();//nejdøíve smázne pøechozí text

		Cvektory::TZakazka *zakazka_temp=Form1->d.v.vrat_temp_zakazku(Edit_n_cesty->Text.ToInt());//inicializace
		//naèítání dat
			AnsiString prochazet;
		if(zakazka_temp!=NULL)
		if(zakazka_temp->cesta!=NULL)//pokud již byla cesta definovaná
		{
			Memo4->Lines->Add("zakázka_temp název:\n"+zakazka_temp->name);
			Cvektory::TCesta *ukaz=zakazka_temp->cesta->dalsi;//pøeskoèí hlavièku, jde rovnou na první segment cesty
			while(ukaz!=NULL)
			{
				Memo4->Lines->Add
				(
						AnsiString(ukaz->n)+","+
						AnsiString(ukaz->objekt->short_name)+","+
						AnsiString(ukaz->CT)+","+
						AnsiString(ukaz->RD)+","+
						AnsiString(ukaz->Tc)+","+
						AnsiString(prochazet)
				);
				ukaz=ukaz->dalsi;
			}
		}

		Cvektory::TZakazka *zakazka=Form1->d.v.ZAKAZKY->dalsi;//inicializace na první zakazku
		//naèítání dat

		if(zakazka!=NULL)
		if(zakazka->cesta!=NULL)//pokud již byla cesta definovaná
		{
			Memo4->Lines->Add("zakázka název:\n"+zakazka->name);
			Cvektory::TCesta *ukaz=zakazka->cesta->dalsi;//pøeskoèí hlavièku, jde rovnou na první segment cesty
			while(ukaz!=NULL)
			{
				Memo4->Lines->Add
				(
						AnsiString(ukaz->n)+","+
						AnsiString(ukaz->objekt->short_name)+","+
						AnsiString(ukaz->CT)+","+
						AnsiString(ukaz->RD)+","+
						AnsiString(ukaz->Tc)+","+
						AnsiString(ukaz->Tv)
				);
				ukaz=ukaz->dalsi;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm_definice_zakazek::scGPGlyphButton_removeClick(TObject *Sender)

{
 	//mazani zakazek
				rStringGridEd1->Rows[rStringGridEd1->RowCount]->Clear();

				if(rStringGridEd1->RowCount>2)
				{
				 Form1->d.v.smaz_temp_zakazku(rStringGridEd1->RowCount-1);
				 rStringGridEd1->RowCount--;
				 }

}
//---------------------------------------------------------------------------
















