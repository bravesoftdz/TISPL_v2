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
	nastav_form(); //nastaven� barev komponent
}
//---------------------------------------------------------------------------
//nastaven� barev komponent
void TForm_definice_zakazek::nastav_form()
{
	//nastaven� glob�ln�ch barev
	TColor light_gray=(TColor)RGB(240,240,240);

	rStringGridEd1->Color=clWhite;//nastaven� pozad� barvy formul��e
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
//zobrazen� formul��e
void __fastcall TForm_definice_zakazek::FormShow(TObject *Sender)
{
	////nastaveni PP, defaultn� jsou ji� od souboru novy, kter� se vol� v�dy, tak�e nen� defaultn� nutn� volat znovu
	nacti_PP();

	////na�ten� zak�zek a cest
	if(Form1->d.v.ZAKAZKY->dalsi==NULL)//kdyz je spojak prazdny
	{
		 //vytvo�� defaultn� �adek se zak�zkou a jej� cestou
		 predvypln_default_zakazku();
		 //predvypln_cestu(); ji� se nepou��v� pln� se z default_cestu() dat tak jako u�ivatlsky definovan� zak�zky
		 //a hned se ulo�� do temp spoj�ku
		 uloz_Defaulttemp_zakazku();
		 uloz_Default_cestu();
		 Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY(); // defaultni zakazku vlozim do hl.spojaku - nesmim mast uzivatele pri zobrazeni dialogu
		 //po ulo�en� do hl.spojaku zak�zek pot�ebuji stejn� znovu ulo�it ��dek do temp_zakazek
		 uloz_Defaulttemp_zakazku();
		 uloz_Default_cestu();
	}
	else//pokud je ulo�eno n�co v zak�zk�ch tak je na�tu
	{
		 nacti_zakazky();
	}
}
// ---------------------------------------------------------------------------
//nastaveni PP, defaultn� jsou ji� od souboru novy, kter� se vol� v�dy, tak�e nen� defaultn� nutn� volat znovu
void TForm_definice_zakazek::nacti_PP()
{
	rEditNum_pozad_mnozstvi->Text=Form1->d.v.PP.mnozstvi;
	rEditNum_pocet_dnu->Text=Form1->d.v.PP.dni_rok;
	rEditNum_effektivita->Text=Form1->d.v.PP.efektivita;
	rEditNum_pocet_prac_hod->Text=Form1->d.v.PP.hod_den;
	scEdit_zacatek->Text=Form1->d.v.PP.cas_start;
}
// ---------------------------------------------------------------------------
//vytvo�� defaultn� �adek se zak�zkou
void TForm_definice_zakazek:: predvypln_default_zakazku()
{
	rStringGridEd1->Cells[0][1]="1";
	rStringGridEd1->Cells[1][1]="1";
	rStringGridEd1->Cells[2][1]="Nova zakazka";
	rStringGridEd1->Cells[3][1]="�erven�";
	rStringGridEd1->Cells[4][1]="100";  //pomer
	rStringGridEd1->Cells[5][1]="NASTAVIT"; //tla��tko pro nastaven� JIG
	rStringGridEd1->Cells[6][1]="200";
	rStringGridEd1->Cells[7][1]="5";
	rStringGridEd1->Cells[8][1]="50";
	rStringGridEd1->Cells[9][1]="NASTAVIT";//tla��tko pro nastaven� ceesty
	rStringGridEd1->Cells[10][1]="2";
}
//----------------------------------------------------------------------------
//p�edvypln�n� default cesta
//void TForm_definice_zakazek::predvypln_cestu()
//ji� se nepou��v� pln� se z default_cestu() dat tak jako u�ivatlsky definovan� zak�zky
//{
//	Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
//	int i=0;
//	while(objekt!=NULL)
//	{
//		i++;
//		Form_cesty->rStringGridEd_cesty->Cells[0][i]=i;//po�ad�
//		Form_cesty->rStringGridEd_cesty->Cells[1][i]=objekt->name;
//		if(objekt->rezim==1)//pokud je kontin�ln� tak CT spo��t� s�m
//			Form_cesty->rStringGridEd_cesty->Cells[2][i]=60*objekt->delka_dopravniku/objekt->pohon->rychlost_od;//CT
//		else
//			Form_cesty->rStringGridEd_cesty->Cells[2][i]="0";//CT
//		Form_cesty->rStringGridEd_cesty->Cells[3][i]=objekt->pohon->rychlost_od;//Rychlost dopravniku, minim�ln� ze zadan�ho rozmez�
//
//		//pokud jde o lakovani, predvyplnim hodnoty jinak jsou 0 pro ostatni objekty
//		//!!!!ned�lat pro LAK ale pro objekt typu resp ID odpov�daj�c� lak
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
	j.sirka=1;j.delka=1;j.vyska=1;j.ks=1;//defaultn� hodnoty jigu
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
	 {  //vlo�en� defauln� cesty                                                                //v tomto p��pad� z�m�rn� takto proto�e se p�eb�r� vypo�ten� hodnota
			Form1->d.v.vloz_segment_cesty(default_zakazka,/*sloupec po�ad� se neukl�d�*/objekt->n,Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][++i]),0,0,objekt->pohon->rychlost_od,0);
			objekt=objekt->dalsi;
	 }
}
//---------------------------------------------------------------------------
//na�ten� hotov�ch zak�zek, tato metoda je vol�na z FormShow(), pokud existuj� zak�zky
void TForm_definice_zakazek::nacti_zakazky()
{
		////vypln�n� stringgridu
		Cvektory::TZakazka *ukaz=Form1->d.v.ZAKAZKY->dalsi;//ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
		int	i=0;
		//Memo4->Lines->Add(AnsiString(ukaz->id));
		while (ukaz!=NULL)
		{
			i++;

			//na�ten� barvy
			rStringGridEd1->Cells[3][i] = ukaz->barva;//pokud je u�ivatelsk�
			//nebo pokud je p�ednastaven�
			if(ukaz->barva==(TColor)RGB(128,0,0)) rStringGridEd1->Cells[3][i]="Ka�tanov�";
			if(ukaz->barva==(TColor)RGB(0,128,0)) rStringGridEd1->Cells[3][i]="Zelen�";
			if(ukaz->barva==(TColor)RGB(128,128,0)) rStringGridEd1->Cells[3][i]="Olivov�";
			if(ukaz->barva==(TColor)RGB(0,0,128)) rStringGridEd1->Cells[3][i]="N�mo�nick� modr�";
			if(ukaz->barva==(TColor)RGB(128,0,128)) rStringGridEd1->Cells[3][i]="Fialov�";
			if(ukaz->barva==(TColor)RGB(0,128,128)) rStringGridEd1->Cells[3][i]="Modrozelen�";
			if(ukaz->barva==(TColor)RGB(128,128,128)) rStringGridEd1->Cells[3][i]="�ed�";
			if(ukaz->barva==(TColor)RGB(192,192,192)) rStringGridEd1->Cells[3][i]="St��brn�";
			if(ukaz->barva==(TColor)RGB(255,0,0)) rStringGridEd1->Cells[3][i]="�erven�";
			if(ukaz->barva==(TColor)RGB(0,255,0)) rStringGridEd1->Cells[3][i]="Sv�tle zelen�";
			if(ukaz->barva==(TColor)RGB(255,255,0)) rStringGridEd1->Cells[3][i]="�lut�";
			if(ukaz->barva==(TColor)RGB(0,0,255)) rStringGridEd1->Cells[3][i]="Modr�";
			if(ukaz->barva==(TColor)RGB(255,0,255)) rStringGridEd1->Cells[3][i]="R��ov�";
			if(ukaz->barva==(TColor)RGB(0,255,255)) rStringGridEd1->Cells[3][i]="Sv�tle modr�";
			if(ukaz->barva==(TColor)RGB(255,255,255)) rStringGridEd1->Cells[3][i]="B�l�";
			if(ukaz->barva==(TColor)RGB(192,220,192)) rStringGridEd1->Cells[3][i]="Zelenomodr�";
			if(ukaz->barva==(TColor)RGB(166,202,240)) rStringGridEd1->Cells[3][i]="Blankytn� Modr�";
			if(ukaz->barva==(TColor)RGB(255,251,240)) rStringGridEd1->Cells[3][i]="Kr�mov�";
			if(ukaz->barva==(TColor)RGB(160,160,164)) rStringGridEd1->Cells[3][i]="St�edn� �ed�";

			//na�ten� zbyl�ch dat
			rStringGridEd1->Cells[0][i] = ukaz->id;
			rStringGridEd1->Cells[1][i] = ukaz->typ;
			//ShowMessage(ukaz->name);
			rStringGridEd1->Cells[2][i] = ukaz->name;
			//rStringGridEd1->Cells[3][i] - barva se nastavuje v��e
			rStringGridEd1->Cells[4][i] = ukaz->pomer;
			rStringGridEd1->Cells[5][i] ="NASTAVIT";
			rStringGridEd1->Cells[6][i] = ukaz->pocet_voziku;
			rStringGridEd1->Cells[7][i] = ukaz->serv_vozik_pocet;
			rStringGridEd1->Cells[8][i] = ukaz->opakov_servis;
			rStringGridEd1->Cells[9][i] ="NASTAVIT";
			rStringGridEd1->Cells[10][i] = ukaz->TT;

			//vlo��m dal�� ��dek (resp. nav���m jejich po��tadlo, po��t� se od 1)
			rStringGridEd1->RowCount=i+1; //zvysuji podle poctu nacitanych zakazek + 1 kvuli hlavicce tabulky


			//posun na dal�� prvek v seznamu
			ukaz=ukaz->dalsi;
		}

		//ulo�en� ZAKAZKY do ZAKAZKYtemp, aby temp obsahoval stejn� data jako jsou zobrazena
		Form1->d.v.kopirujZAKAZKY2ZAKAZKY_temp();//pouze ZAKAZKY_temp=ZAKAZKY
}
//----------------------------------------------------------------------------
//vol�n� sub formul��� JIG a CESTA a p��padn� ukl�d�n� do dat
void __fastcall TForm_definice_zakazek::rStringGridEd1Click(TObject *Sender)
{
	//definice ukazatele na aktu�ln� editovanou zak�zku     //��slo ��dku kde kliku do�lo
	Cvektory::TZakazka *zakazka=Form1->d.v.vrat_temp_zakazku(rStringGridEd1->Row);
	AnsiString prochazet;

	////////////jig form- byl klik na bu�ku v 5. sloupci
	if(rStringGridEd1->Col==5)
	{
		//napln�n� dat
		Form_jig->Edit_jig_pocet_ks->Text=zakazka->jig.ks;
		Form_jig->Edit_jig_delka->Text=zakazka->jig.delka;
		Form_jig->Edit_jig_sirka->Text=zakazka->jig.sirka;
		Form_jig->Edit_jig_vyska->Text=zakazka->jig.vyska;
		//zobrazen� formu + ulo�en� dat pokud je zvoleno OK
		if(mrOk==Form_jig->ShowModal())
		{
			zakazka->jig.ks=Form1->ms.MyToDouble(Form_jig->Edit_jig_pocet_ks->Text);
			zakazka->jig.delka=Form1->ms.MyToDouble(Form_jig->Edit_jig_delka->Text);
			zakazka->jig.sirka=Form1->ms.MyToDouble(Form_jig->Edit_jig_sirka->Text);
			zakazka->jig.vyska=Form1->ms.MyToDouble(Form_jig->Edit_jig_vyska->Text);
		}
	}

	////////////cesty form - byl klik na bu�ku 9. sloupci
	if(rStringGridEd1->Col==9)
	{
		//Form_cesty->rStringGridEd_cesty->Columns->Clear();
		////napln�n� picklistu
		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Clear();
		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ano");
		Form_cesty->rStringGridEd_cesty->Columns->Items[7]->PickList->Add("Ne");

		////na��t�n� dat - nen� t�eba o�et�ovat, proto�e existuje default cesta v seznamu
		//vyp�e d��ve nadefinovanou cestu v�etn� segment� z nejdel�� mo�n� cesty, kter� nejsou sou��st� aktu�ln� cesty
		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
		//p��d�n� po�tu ��dk�
		Form_cesty->rStringGridEd_cesty->RowCount=Form1->d.v.OBJEKTY->predchozi->n+1;//p�id�n� dal��ho ��dku     //pridani k zobrazeni if ukaz param
		int i=0;
		while(O!=NULL)//proch�z� v�echnyobjekty a bu� je ("Ano") objekt i na cest� nebo nen� ("Ne")
		{
			i++;
			Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,zakazka);
			if(C!=NULL)//zahrnut� segmenty cesty
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
			else //nezahrnut� segment nejdel�� cesty v aktu�ln� cest�
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

		////ukl�d�n� dat - jednotliv�ho segmentu cesty pokud je pova�ov�n k zahrnutn� ze strany u�ivatele
		//formul�� na st�ed
		Form_cesty->Left=Form1->ClientWidth/2-Form_cesty->Width/2;
		Form_cesty->Top=Form1->ClientHeight/2-Form_cesty->Height/2;
		if(mrOk==Form_cesty->ShowModal())
		{
			Form1->d.v.inicializace_cesty(zakazka);
			for(int i=1;i<Form_cesty->rStringGridEd_cesty->RowCount;i++)
			{
				if(Form_cesty->rStringGridEd_cesty->Cells[7][i]=="Ano")//pokud je za�krnuto neproch�zek objekt se neulo�� do cesty
				{
						Form1->d.v.vloz_segment_cesty
						(
							zakazka,
							/*sloupec po�ad� se neukl�d�*/ /*pozor na �azen�*/
							Form_cesty->rStringGridEd_cesty->Cells[0][i].ToInt(),//ID-do�e�it
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[2][i]),//CT
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[5][i]),//RD
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[4][i]),//Tv
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[3][i]),//Tc
							Form1->ms.MyToDouble(Form_cesty->rStringGridEd_cesty->Cells[6][i])//Opak   //ulozeni stavu pro cestu - roletka
						);
				}
				//vymaz�n� textu z ji� nepot�ebn�ho ��dku
				Form_cesty->rStringGridEd_cesty->Rows[i]->Clear();
			}
		}
	}
}
//---------------------------------------------------------------------------
//TLA��TKO ULO�IT
void __fastcall TForm_definice_zakazek::scGPButton_UlozitClick(TObject *Sender)
{
		Form1->d.v.PP.mnozstvi=Form1->ms.MyToDouble(rEditNum_pozad_mnozstvi->Text);
		Form1->d.v.PP.dni_rok=Form1->ms.MyToDouble(rEditNum_pocet_dnu->Text);
		Form1->d.v.PP.efektivita=Form1->ms.MyToDouble(rEditNum_effektivita->Text);
		Form1->d.v.PP.hod_den=Form1->ms.MyToDouble(rEditNum_pocet_prac_hod->Text);
		Form1->d.v.PP.cas_start=TDateTime(scEdit_zacatek->Text);


		TColor barva;//mus� b�t v TColor kv�li ukl�d�n� do TColor objektu!!!

		//ulo�en� editovan�ch zak�zek
		for (int i = 1; i< rStringGridEd1->RowCount; i++)
		{
			bool usersColor=true;
			//konverze p�ednastaven�ch barev
			if(rStringGridEd1->Cells[3][i]=="Ka�tanov�"){barva=(TColor)RGB(128,0,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Zelen�"){barva=(TColor)RGB(0,128,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Olivov�"){barva=(TColor)RGB(128,128,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="N�mo�nick� modr�"){barva=(TColor)RGB(0,0,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Fialov�"){barva=(TColor)RGB(128,0,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Modrozelen�"){barva=(TColor)RGB(0,128,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="�ed�"){barva=(TColor)RGB(128,128,128);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="St��brn�"){barva=(TColor)RGB(192,192,192);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="�erven�"){barva=(TColor)RGB(255,0,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Sv�tle zelen�"){barva=(TColor)RGB(0,255,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="�lut�"){barva=(TColor)RGB(255,255,0);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Modr�"){barva=(TColor)RGB(0,0,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="R��ov�"){barva=(TColor)RGB(255,0,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Sv�tle modr�"){barva=(TColor)RGB(0,255,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="B�l�"){barva=(TColor)RGB(255,255,255);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Zelenomodr�"){barva=(TColor)RGB(192,220,192);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Blankytn� Modr�"){barva=(TColor)RGB(166,202,240);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="Kr�mov�"){barva=(TColor)RGB(255,251,240);usersColor=false;}
			if(rStringGridEd1->Cells[3][i]=="St�edn� �ed�"){ barva=(TColor)RGB(160,160,164);usersColor=false;}
			if(usersColor)barva=(TColor)rStringGridEd1->Cells[3][i].ToInt();//mus� b�t a� tady jinak spadne

			//ulo�en� aktu�ln�ch hodnot do dan� temp_zakazky
			Form1->d.v.edituj_temp_zakazku
			(
				i,//n
				rStringGridEd1->Cells[0][i],//ID
				rStringGridEd1->Cells[1][i].ToInt(),//TYP
				rStringGridEd1->Cells[2][i],//NAME
				barva,//COLOR
				Form1->ms.MyToDouble(rStringGridEd1->Cells[4][i]),//POMER
				Form1->ms.MyToDouble(rStringGridEd1->Cells[10][i]),//TT
				//JIG se u� jako parametr nepo�aduje, stejn� jako cesta, jedn� se o p�ed�v�n� ukazatelem p�i zav�r�n� pat�i�n�ho formul��e
				rStringGridEd1->Cells[6][i].ToInt(),//n-voz�ku
				rStringGridEd1->Cells[7][i].ToInt(),//n-servis voz�k�
				rStringGridEd1->Cells[8][i].ToInt()//n-opak voz�k�
			);
		}
		//samotn� ulo�en�
		Form1->d.v.kopirujZAKAZKY_temp2ZAKAZKY();//ulo�en� do ostr�ho spoj�ku ZAKAZKY+sm�znut� ZAKAZKY_temp
		Form1->d.v.generuj_VOZIKY();//vygenerov�n� voz�k� dle zadan�ch zak�zek
		Form_definice_zakazek->Close();//zav�en� formul��e s n�sledn�m DuvodUlozit(true); po modalshow v unit1
		Form1->REFRESH();//p�ekreslen� kv�li zobrazen� aktu�ln�ch cest
}
//---------------------------------------------------------------------------
//Zav�en� formul��e (storno a k��ek je to sam�)
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
//	novy->id_radek = RzStringGrid1->Row; // nav���m po��tadlo prvku o jedni�ku
//	novy->barva_voziku = ColorDialog1->Color;
//
//	BARVY->predchozi->dalsi = novy;
//	// posledn�mu prvku p�i�ad�m ukazatel na nov� prvek
//	novy->predchozi = BARVY->predchozi;
//	// novy prvek se odkazuje na prvek predchoz� (v hlavicce body byl ulozen na pozici predchozi, posledn� prvek)
//	novy->dalsi = NULL;
//	BARVY->predchozi = novy;
//	// nov� posledn� prvek z�pis do hlavi�ky,body->predchozi z�pis do hlavi�ky odkaz na posledn� prvek seznamu "predchozi" v tomto p��pad� zav�d�jic�
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::uloz_barvu(TColor barva, int i)
//{
//	TBarva *novy = new TBarva;
//
//	// ShowMessage(barva);
//
//	novy->id_radek = i; // nav���m po��tadlo prvku o jedni�ku
//	novy->barva_voziku = barva;
//
//	BARVY->predchozi->dalsi = novy;
//	// posledn�mu prvku p�i�ad�m ukazatel na nov� prvek
//	novy->predchozi = BARVY->predchozi;
//	// novy prvek se odkazuje na prvek predchoz� (v hlavicce body byl ulozen na pozici predchozi, posledn� prvek)
//	novy->dalsi = NULL;
//	BARVY->predchozi = novy;
//	// nov� posledn� prvek z�pis do hlavi�ky,body->predchozi z�pis do hlavi�ky odkaz na posledn� prvek seznamu "predchozi" v tomto p��pad� zav�d�jic�
//
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::aktualizuj_barvu(TColor barva, int row)
//{
//	TBarva *ukaz = BARVY->dalsi;
//	// ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
//	// ulo�� do "spoj��ku"
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
//	// vy�azen� prvku ze seznamu a napojen� prvku dal��ho na prvek p�edchoz� prku mazan�ho
//	if (Barva->dalsi != NULL) // o�et�en� proti posledn�mu prvku
//	{
//		Barva->predchozi->dalsi = Barva->dalsi;
//		Barva->dalsi->predchozi = Barva->predchozi;
//	}
//	else // posledn� prvek
//	{
//		if (Barva->id_radek == 1) // pokud je mazan� prvek hned za hlavi�kou
//		{
//			BARVY->predchozi = Barva->predchozi;
//			// pop� hlavi�ka bude ukazovat sama na sebe
//			BARVY->dalsi = NULL;
//		}
//		else {
//			Barva->predchozi->dalsi = NULL;
//			BARVY->predchozi = Barva->predchozi;
//			// zapis do hlavi�ky posledn� prvek seznamu
//		}
//	}
//}
//// ---------------------------------------------------------------------------
//void TForm_definice_zakazek::sniz_indexy(TBarva *Barva) {
//	while (Barva != NULL) {
//		Barva = Barva->dalsi; // posun na dal�� prvek
//		if (Barva != NULL)
//			Barva->id_radek--;
//		// sn�� indexy nasledujic�ch bod�,proto�e optimalizace seznamu nefungovalo, nav�c u�et��m strojov� �as
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
//	RzStringGrid1->Cols[0]->Add("Voz�k");
//	RzStringGrid1->Cols[0]->Add("1");
//	RzStringGrid1->Rows[0]->Add("ID");
//	RzStringGrid1->Rows[0]->Add("D�lka");
//	RzStringGrid1->Rows[0]->Add("���ka");
//	RzStringGrid1->Rows[0]->Add("Cesta"); //pou�ito pro trasu voz�ku
//	RzStringGrid1->Rows[0]->Add("Rotace");
//	RzStringGrid1->Rows[0]->Add("N�zev");
//	RzStringGrid1->Rows[0]->Add("Max ks");
//	RzStringGrid1->Rows[0]->Add("Po�et ks");
//	RzStringGrid1->Rows[0]->Add("D�lka Max");
//	RzStringGrid1->Rows[0]->Add("���ka Max");
//	RzStringGrid1->Rows[0]->Add("V��ka Max");
//	RzStringGrid1->Rows[0]->Add("Barva");
//	RzStringGrid1->Rows[0]->Add("Stav");
//
//
// /*	rStringGridEd1->Cols[0]->Add("Zak�zka");
//	rStringGridEd1->Cols[0]->Add("1");
//	rStringGridEd1->Rows[0]->Add("ID");
//	rStringGridEd1->Rows[0]->Add("Typ");
//	rStringGridEd1->Rows[0]->Add("N�zev");
//	rStringGridEd1->Rows[0]->Add("Barva");
//	rStringGridEd1->Rows[0]->Add("Pom�r");
//	rStringGridEd1->Rows[0]->Add("Jigy");
//	rStringGridEd1->Rows[0]->Add("Po�et");
//	rStringGridEd1->Rows[0]->Add("Po�.serv.voz");
//	rStringGridEd1->Rows[0]->Add("Opakov�n�");
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
//		RzStringGrid1->Cells[6][1] = "n�zev";
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
//	// ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
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
//			RzStringGrid1->Cells[4][i] = ukaz->vyska;  //cesta voz�ku  prozat�m
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
//			// posun na dal�� prvek v seznamu
//			ukaz = ukaz->dalsi;
//		}
//	}
//	else { // kdy� ve spoj�ku nic nen�, nastav�m do ID defaultn� ID=1
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
//			//ulo�en� parametr� voz�k�
//			Form1->d.v.vloz_vozik(RzStringGrid1->Cells[0][i].ToInt(),
//			RzStringGrid1->Cells[1][i],
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[2][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[3][i]),
//			Form1->ms.MyToDouble(RzStringGrid1->Cells[4][i]), //sem uklad�m v��ku
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
//			//ulo�� do cesty barvu cesty-zak�zky dle voz�ku
//			Form1->d.v.vrat_cestu(Form1->ms.MyToDouble(RzStringGrid1->Cells[4][i]))->barva=ukaz->barva_voziku;
//
//			//posun na dal�� prvek v seznamu
//			ukaz = ukaz->dalsi; //posun na dal�� prvek v seznamu
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
	// ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku

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
//		ukaz = ukaz->dalsi; // posun na dal�� prvek v seznamu
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
//		ukaz = ukaz->dalsi; // posun na dal�� prvek v seznamu
//	}
//
//	TBarva *ukaz1 = BARVY->dalsi;
//	while (ukaz1 != NULL) {
//		if (ukaz1->id_radek == FromIndex)
//			ukaz1->barva_voziku = ColorToIndex;
//		if (ukaz1->id_radek == ToIndex)
//			ukaz1->barva_voziku = ColorFromIndex;
//		ukaz1 = ukaz1->dalsi; // posun na dal�� prvek v seznamu
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

	if (RzStringGrid1->Col == 1) // ID   - r�zn� alfanumerick� znaky
			Value = "AAAAAAAAAA";

	if (RzStringGrid1->Col == 2) // Delka  - se�ere pouze ��slo
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
//	// Tla��tko na smaz�n� ��dku
//	RzStringGrid1->Rows[RzStringGrid1->Row]->Clear();
//	rStringGridEd1->Rows[rStringGridEd1->Row]->Clear();
//
//	TForm_definice_zakazek::TBarva *ukaz = BARVY->dalsi;
//	// ukazatel na prvn� objekt v seznamu OBJEKTU, p�esko�� hlavi�ku
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
//		// posun na dal�� prvek v seznamu
//		ukaz = ukaz->dalsi;
//	}
//
//	// posun ��dk�
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
//		//MEZERN�K
//		case 32:break;
//		//PAGE UP
//		case 33:;break;
//		//PAGE DOWN
//		case 34:break;
//		//END
//		case 35:break;
//		//HOME
//		case 36:break;
//		//�IPKA DOL�
//		case 40:break;
//		//�IPKA LEV�
//		case 37:break;
//		//�IPKA PRAV�
//		case 39:break;
//		//�IPKA NAHORU
//		case 38:break;
//		//�IPKA DOL�
//		case 98:break;
//		//�IPKA LEV�
//		case 100:break;
//		//�IPKA PRAV�
//		case 102:break;
//		//�IPKA NAHORU
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
//zkop�ruje p�edchoz�
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


		if(i>1) {// defaultn� zak�zka je ulo�ena hned v temp_spojaku + hlavnim pri form_show, �ili tady ukl�d�m a� dal�� ��dky
	
	rStringGridEd1->Cells[0][i]=i;
	rStringGridEd1->Cells[1][i]="1";
	rStringGridEd1->Cells[2][i]="Nov� zakazka";
	rStringGridEd1->Cells[3][i]="�ervena";
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

		 //p�i p�id�n� dal�� zak�zky ulo��m do cesty defaultn� hodnoty
	 Cvektory::TObjekt *objekt=Form1->d.v.OBJEKTY->dalsi;//inicializace
	 Cvektory::TZakazka *nova_zakazka=Form1->d.v.vrat_temp_zakazku(j);
	 Form1->d.v.inicializace_cesty(nova_zakazka);
	 while(objekt!=NULL)
	 {  //vlo�en� defauln� cesty
			Form1->d.v.vloz_segment_cesty(nova_zakazka,/*sloupec po�ad� se neukl�d�*/objekt->n,0,0,0,0,0);
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
		Memo4->Lines->Clear();//nejd��ve sm�zne p�echoz� text

		Cvektory::TZakazka *zakazka_temp=Form1->d.v.vrat_temp_zakazku(Edit_n_cesty->Text.ToInt());//inicializace
		//na��t�n� dat
			AnsiString prochazet;
		if(zakazka_temp!=NULL)
		if(zakazka_temp->cesta!=NULL)//pokud ji� byla cesta definovan�
		{
			Memo4->Lines->Add("zak�zka_temp n�zev:\n"+zakazka_temp->name);
			Cvektory::TCesta *ukaz=zakazka_temp->cesta->dalsi;//p�esko�� hlavi�ku, jde rovnou na prvn� segment cesty
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

		Cvektory::TZakazka *zakazka=Form1->d.v.ZAKAZKY->dalsi;//inicializace na prvn� zakazku
		//na��t�n� dat

		if(zakazka!=NULL)
		if(zakazka->cesta!=NULL)//pokud ji� byla cesta definovan�
		{
			Memo4->Lines->Add("zak�zka n�zev:\n"+zakazka->name);
			Cvektory::TCesta *ukaz=zakazka->cesta->dalsi;//p�esko�� hlavi�ku, jde rovnou na prvn� segment cesty
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
















