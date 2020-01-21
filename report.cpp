//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "report.h"
#include "Unit1.h"
#include "parametry_linky.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scStyledForm"
#pragma resource "*.dfm"
TForm_report *Form_report;
//---------------------------------------------------------------------------
__fastcall TForm_report::TForm_report(TComponent* Owner)
	: TForm(Owner)
{
	//webrowser naète vytvoøené html k zobrazení, v pøípadì soubor nový se naèítá html soubor z adresáøe aplikace
	if(Form1->FileName=="Nový.tispl")
	{
		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
		WebBrowser1->Navigate(ExtractFilePath(Application->ExeName)+"Nový.html");
	}
	else	WebBrowser1->Navigate(Form1->FileName);

	//pozice formuláøe na støed obrazovky
	Form_report->Left=Form1->ClientWidth/2-Form_report->Width/2;
	Form_report->Top=Form1->ClientHeight/2-Form_report->Height/2;

	//nastavení barvy formuláøe
	Form_report->Color=(TColor)RGB(240,240,240);
	//	scGPPanel1->FillColor=(TColor)RGB(240,240,240);
}
//---------------------------------------------------------------------------
void __fastcall TForm_report::KonecClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
short int TForm_report::ulozit_report(UnicodeString FileName)
{
	//////Objekty
	Cvektory::TObjekt *O=F->d.v.OBJEKTY->dalsi;
	while(O!=NULL)
	{
		UnicodeString n=O->n;
		UnicodeString id=O->id;
		UnicodeString name=O->name;
		UnicodeString X=O->X;
		UnicodeString Y=O->Y;
		UnicodeString pocet_bodu="0";
		if(O->body!=NULL && O->body->predchozi->n>0)pocet_bodu=O->body->predchozi->n;
		UnicodeString sirka_steny=O->sirka_steny;
		UnicodeString Xt=O->Xt;
		UnicodeString Yt=O->Yt;
		UnicodeString orientace_text=O->orientace_text;
		UnicodeString Xp=O->Xp;
		UnicodeString Yp=O->Yp;
		UnicodeString rezim=O->rezim;
		UnicodeString orientace=O->orientace;
		UnicodeString pohon="Nepøiøazen/needitován";
		if(O->pohon!=NULL)pohon=O->pohon->name;
		UnicodeString prvni_element=O->element->name;
		UnicodeString min_prujezdni_profil_x=O->min_prujezdni_profil.x;
		UnicodeString min_prujezdni_profil_y=O->min_prujezdni_profil.y;
		UnicodeString koty_elementu_offset_x=O->koty_elementu_offset.x;
		UnicodeString koty_elementu_offset_y=O->koty_elementu_offset.y;
		UnicodeString pocet_komor=0;
		if(O->komora!=NULL && O->predchozi->n>0)pocet_komor=O->komora->predchozi->n;
		UnicodeString zobrazit_koty="Ne";
		if(O->zobrazit_koty)zobrazit_koty="Ano";
		UnicodeString zobrazit_mGrid="Ne";
		if(O->zobrazit_mGrid)zobrazit_mGrid="Ano";
		UnicodeString uzamknout_nahled="Ne";
		if(O->uzamknout_nahled)uzamknout_nahled="Ano";
		//vypsaní - uložení


		O=O->dalsi;
	}
	delete O;O=NULL;



		AnsiString data="";//celková textová data k exportu

    	AnsiString T=F->readINI("nastaveni_form_parametry", "CT");
	if(T=="")CTunit=S;else CTunit=MIN;
	T=F->readINI("nastaveni_form_parametry","RDt");
	if(T=="")aRDunit=S;else aRDunit=MIN;
	T=F->readINI("nastaveni_form_parametry","DD");
	if(T=="")DDunit=M;else DDunit=MM;
	T=F->readINI("nastaveni_form_parametry","DM");
	if(T=="")Munit=M; else Munit =MM;
  T=F->readINI("nastaveni_form_parametry_linky","R");
	if(T=="")Runit=M; else Runit =MM;
  T=F->readINI("nastaveni_form_parametry_linky","rozmery");
	if(T=="")Sirkaunit=M; else Sirkaunit =MM;
  T=F->readINI("nastaveni_form_parametry_linky","TT");
	if(T=="")TTunit=S; else TTunit =MIN;


		//zjištìní exportovaného formátu
		unsigned short export_format=3;
		if(FileName.SubString(FileName.Length()-2,3).LowerCase() =="csv")export_format=1;
		if(FileName.SubString(FileName.Length()-2,3).LowerCase() =="xls")export_format=2;
		if(FileName.SubString(FileName.Length()-3,4).LowerCase() =="html")export_format=3;

		//formátování textu do jednotlivých formátù
		UnicodeString S=";";//SEPARATOR pro CSV
		switch(export_format)//1-csv, 2 - xls,3 - html
		{
			//ZDM case 1:data=get_csv_xls(";");break;//CSV
			//ZDM case 2:data=get_csv_xls("\t");break;//XLS
			//ZDM case 3:data=get_html();break;//html
		}

		//zápis dat do DB
//			AnsiString ID ="1";
//			AnsiString header = "INSERT INTO report_header (app_id,project_name) VALUES (\""+ID+"\",\""+FileName+"\")";
//			Form1->FDConnection1->ExecSQL(header);
//			AnsiString dni=PP.dni_rok;
//			AnsiString mnozstvi=PP.mnozstvi;
//			AnsiString hod_den=PP.hod_den;
//			AnsiString efektivita=PP.efektivita;
//			AnsiString cas_start=PP.cas_start;
//
//			AnsiString sw_vyroba = "INSERT INTO project_params_sw_vyroba (pocet_dni,mnozstvi_kusu,prac_hodin,efektivita,pocatek_vyroby,id_projektu) VALUES (\""+dni+"\",\""+mnozstvi+"\",\""+hod_den+"\",\""+efektivita+"\",\""+cas_start+"\",\""+ID+"\")";
//			Form1->FDConnection1->ExecSQL(sw_vyroba);
//
//			Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;
//
//			while (ukaz){
//
//			AnsiString n=ukaz->n;
//			AnsiString name=ukaz->name;
//			AnsiString rychlost_od=ukaz->rychlost_od;
//			AnsiString rychlost_do=ukaz->rychlost_do;
//			AnsiString roztec=ukaz->roztec;
//
//			AnsiString hw_pohony = "INSERT INTO project_params_hw_pohony (id_pohon,nazev,rychlost_od,rychlost_do,roztec_palcu,id_projektu) VALUES (\""+n+"\",\""+name+"\",\""+rychlost_od+"\",\""+rychlost_do+"\",\""+roztec+"\",\""+ID+"\")";
//			Form1->FDConnection1->ExecSQL(hw_pohony);
//			ukaz=ukaz->dalsi;
//			}

		 UnicodeString titulek_projektu;
		 UnicodeString PP_TT=Form1->d.v.PP.TT/(1+59.0*TTunit);

		 UnicodeString delka_voziku=Form1->d.v.PP.delka_jig*(1+999.0*Sirkaunit);
		 UnicodeString sirka_voziku=Form1->d.v.PP.sirka_jig*(1+999.0*Sirkaunit);


     UnicodeString jednotky_casove;
     UnicodeString jednotky_rozmery;
     UnicodeString jednotky_RD;
     UnicodeString jednotky_vzdalenost;
     UnicodeString jednotky_CT;
     UnicodeString jednotky_kabiny;

     if(aRDunit) jednotky_RD="[m/min]"; else  jednotky_RD="[m/s]";
     if(Runit) jednotky_vzdalenost="[mm]"; else  jednotky_vzdalenost="[m]";
     if(CTunit)  jednotky_CT="[min]"; else  jednotky_CT="[s]";
     if(DDunit) jednotky_kabiny=="[mm]"; else  jednotky_kabiny="[m]";

     if(TTunit) jednotky_casove="[min]"; else  jednotky_casove="[s]";
     if(Sirkaunit) jednotky_rozmery="[mm]"; else  jednotky_rozmery="[m]";


		 try{

	 //////////VYGENEROVÁNÍ HTML HLAVIÈKY PODLE TOHO ZDA EXISTUJE ZAKÁZKA ÈI NIKOLIV ///////////////////////////////

		if(export_format==3)  {

			data+="<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\" integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";
			data+="<link rel=\"stylesheet\"  href=\"\../linky_layouts/styly/css/bootstrap.min.css\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";

			//data+="<script src=\"\../linky_layouts/styly/js/bootstrap.min.js\"></script>";
			data+="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		 //	data+="<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js\" integrity=\"sha384-alpBpkh1PFOepccYVYDB4do5UnbKysX5WZXm3XxPqe5iKTfUKjNkCk9SaVuEZflJ\" crossorigin=\"anonymous\"></script>";
			data+="<div class=\"container-fluid\">";

			data+="<form></br>";
			data+="<h4>Parametry linky <b>"+UnicodeString(Form1->scLabel_titulek->Caption)+"</b></h4></br>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Takt Time"+UnicodeString(jednotky_casove)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+PP_TT+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Délka Jigu "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_voziku+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Šíøka Jigu "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+sirka_voziku+"\"></div></div>";

			data+="</form></div></br>";
			data+="<div class=\"container-fluid\"><form>";


		//Pøehled nadefinovaných pohonù - doplnil M
			Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;
			if(P!=NULL)
			{
				data+="<h4>Pøehled pohonù</h4></br>";
				data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Název</th><th scope=\"col\">Rychlost od "+jednotky_RD+"</th><th scope=\"col\">Rychlost do "+jednotky_RD+"</th><th scope=\"col\">Akt. rychlost "+jednotky_RD+"</th><th scope=\"col\">Rozteè palcù "+jednotky_vzdalenost+"</th><th scope=\"col\">Používán</th></tr></thead>";
				while(P!=NULL)
				{
					UnicodeString ID=P->n;
					UnicodeString name=P->name;
					UnicodeString rychlost_od=P->rychlost_od*(1+59.0*aRDunit);
					UnicodeString rychlost_do=P->rychlost_do*(1+59.0*aRDunit);
					UnicodeString RD=P->aRD*(1+59.0*aRDunit);
					UnicodeString R=P->roztec*(1+999.0*Runit);
				 	UnicodeString Pouzit=Form1->d.v.vypis_objekty_vyuzivajici_pohon(P->n);if(Pouzit=="")Pouzit="nepoužíván";/*if(Form1->d.v.pohon_je_pouzivan(P->n))Pouzit="Ano";*/
					data+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+rychlost_od+"</td><td>"+rychlost_do+"</td><td>"+RD+"</td><td>"+R+"</td><td>"+Pouzit+"</td></tr>";
					P=P->dalsi;
				}
				data+="</tbody></table></br>";
			}
			P=NULL;delete P;

		//////////////////HTML EXPORT V NÁVRHU///////////////////////////////////

		if(Form1->STATUS==Form1->NAVRH)
		{
			//data+="<h4>Architekt: Pøehled objektù a jejich parametrù</h4></br>";
			data+="<h4>Pøehled objektù a jejich parametrù</h4>";
			data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Název</th><th scope=\"col\">Zkratka</th><th scope=\"col\">Režim</th><th scope=\"col\">Název pohonu</th><th scope=\"col\">Technolog. èas "+jednotky_CT+"</th><th scope=\"col\">Rychlost pohonu "+jednotky_RD+"</th><th scope=\"col\">Délka kabiny "+jednotky_kabiny+"</th><th scope=\"col\">Kapacita [vozíku a mezer]</th><th scope=\"col\">Pozice [vozíkù]</th><th scope=\"col\">Orientace[°]</th><th scope=\"col\">Mezera mezi jigy "+jednotky_kabiny+"</th><th scope=\"col\">Mezera mezi podvozky "+jednotky_kabiny+"</th></tr></thead>";
			Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
			//////Elementy
	Cvektory::TElement *E=F->d.v.ELEMENTY->dalsi;
	while(E!=NULL)
	{
		UnicodeString n=E->n;
		UnicodeString eID=E->eID;
		UnicodeString short_name=E->short_name;
		UnicodeString name=E->name;
		UnicodeString orientace=E->orientace;
		UnicodeString rotace_jig=E->rotace_jig;
		UnicodeString stav=E->stav;
		UnicodeString X=E->X;
		UnicodeString Y=E->Y;
		UnicodeString Z=E->Z;
		UnicodeString Xt=E->Xt;
		UnicodeString Yt=E->Yt;
		UnicodeString PTotoc=E->PTotoc;
		UnicodeString OTOC_delka=E->OTOC_delka;
		UnicodeString zona_pred=E->zona_pred;
		UnicodeString zona_za=E->zona_za;
		UnicodeString WT=E->WT;
		UnicodeString PD=E->data.PD;
		UnicodeString orientace_jig_pred=E->data.orientace_jig_pred;
		UnicodeString LO1=E->data.LO1;
		UnicodeString LO2=E->data.LO2;
		UnicodeString LO_pozice=E->data.LO_pozice;
		UnicodeString PT1=E->data.PT1;
		UnicodeString PT2=E->data.PT2;
		UnicodeString WTstop=E->data.WTstop;
		UnicodeString RT_x=E->data.RT.x;
		UnicodeString RT_y=E->data.RT.y;
		UnicodeString pocet_pozic=E->data.pocet_pozic;
		UnicodeString pocet_voziku=E->data.pocet_voziku;
		UnicodeString objekt_n=E->objekt_n;
    AnsiString nazev_objektu = F->d.v.vrat_objekt(E->objekt_n)->name;
		UnicodeString pohon="Nepøiøazen";
		if(E->pohon!=NULL)pohon=E->pohon->name;

  if(eID!=MaxInt)
         {
					data+="<tr><th scope=\"row\">"+eID+"</th><td>"+nazev_objektu+"</td><td>"+short_name+"</td><td>"+pohon+"</td><td>"+LO1+"</td><td>"+LO2+"</td>"+/*<td>"+kapacita_dop+"</td>*/"<td>"+PT1+"</td><td>"+PT2+"</td></tr>";
          }
					E=E->dalsi;
	    }
	    delete E;E=NULL;
			data+="</tbody></table></br>";

       data+="<h4>Zprávy o lince</h4>";
       data+="<table class=\"table table-striped table-responsive\"><thead></thead>";
       data+="<tbody><tr><td>";
			 data+="</form></div></br>";  //celkové ukonèení celého divu

		}
	}
	//////KONEC HTML EXPORTU///////////////////////////////////////////////////////

	/////////////////////CSV///////////////////////////////////////////////////////////
		if(export_format==1)  {

		data+="Pøehled pohonù"+S+"\n";

		Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;
		data+="ID"+S+"Název"+S+"Rychlost od [m/min]"+S+"Rychlost do [m/min]"+S+"Rozteè palcù [mm]"+S+"\n";
		while(ukaz!=NULL)
		{
			UnicodeString ID=ukaz->n;
			UnicodeString name=ukaz->name;
			UnicodeString rychlost_od=ukaz->rychlost_od*60.0;
			UnicodeString rychlost_do=ukaz->rychlost_do*60.0;
			UnicodeString roztec=ukaz->roztec*1000.0;
			data+=""+ID+S+name+S+rychlost_od+S+rychlost_do+S+roztec+S+"\n";
			ukaz=ukaz->dalsi;
		}
		data+="\n";
    data+="\n";
				//KONEC OBECNÉ HLAVIÈKY CSV//
 /////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////CSV EXPORT V REŽIMU NÁVRHU /////////////////////////////////////////
			if(Form1->STATUS==Form1->NAVRH) {


		data+="Pøehled objektù a jejich parametrù\n";
		data+="ID"+S+"Název"+S+"Zkratka"+S+"Režim"+S+"CT [s]"+S+"Délka objektu"+S+"Poèet pozic"+S+"Kapacita [vozíkù a mezer]"+S+"Mezera jig [m]"+S+"Mezera podvozek [m]"+S+"Rozestup [m]"+S+"Každý n-tý palec"+S+"Název pohonu"+S+"Rychlost pohonu [m/min]"+S+"Rozsah pohonu [m/min]"+S+"Rozteè palcù [mm]\n";

		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;

		while (O!=NULL)//prochází potenciální segmenty cesty po objektech
		{
								UnicodeString ID=O->id;
								UnicodeString name=O->name;
								UnicodeString short_name=O->short_name;
								UnicodeString mezera=O->mezera;
								UnicodeString rezim;
								UnicodeString CT=O->CT;
                UnicodeString delka_objektu=O->delka_dopravniku;
                UnicodeString pocet_pozic=O->pozice;
                UnicodeString mezera_jig=O->mezera_jig;
                UnicodeString mezera_podvozek=O->mezera_podvozek;
								UnicodeString kapacita=O->kapacita;
								UnicodeString kapacita_dop=O->kapacita_dop;
								UnicodeString nazev_pohonu;
								UnicodeString roztec_palcu;
								double rozsah_pohonu_od=0;
								double rozsah_pohonu_do=0;
                double Rz=0; double Rx=0;


								if(O->pohon!=NULL)
                {
								nazev_pohonu=O->pohon->name;
								roztec_palcu=O->pohon->roztec*1000.0;
								rozsah_pohonu_od=O->pohon->rychlost_od*60.0;
								rozsah_pohonu_do=O->pohon->rychlost_do*60.0;
                Rz=O->pohon->Rz;
                Rx=O->pohon->Rx;
							 //ShowMessage(rozsah_pohonu_do);
							 //	rozsah_pohonu_od=Form1->ms.MyToDouble(rozsah_pohonu_od)*60.0;
							 //	rozsah_pohonu_do=Form1->ms.MyToDouble(rozsah_pohonu_do)*60.0;
								}
								else {nazev_pohonu="nepøiøazen";roztec_palcu="";}

								UnicodeString rychlost_dopravniku=Form1->ms.MyToDouble(O->RD)*60.0;    //vždy budu zobrazovat v m/min
								UnicodeString delka_dopravniku=O->delka_dopravniku;
									switch(O->rezim)
								{
									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantní";delka_dopravniku="nerelevantní"; break;
									case 1:rezim="KONTINUÁLNÍ";break;
									case 2:rezim="POSTPROCESNÍ";rychlost_dopravniku="nerelevantní";break;
								}
////         	//	 ShowMessage(ID+name+short_name+rezim+CT+kapacita+kapacita_dop+nazev_pohonu+rychlost_dopravniku+delka_dopravniku+roztec_palcu+rozsah_pohonu_od+rozsah_pohonu_do);
							data+=""+ID+S+name+S+short_name+S+rezim+S+CT+S+delka_objektu+S+pocet_pozic+S+kapacita+S+mezera_jig+S+mezera_podvozek+S+Rz+S+Rx+S+nazev_pohonu+S+rychlost_dopravniku+S+rozsah_pohonu_od+"-"+rozsah_pohonu_do+S+roztec_palcu+"\n";
              //  martin
									O=O->dalsi;
		}
	}
	//////////////////////KONEC NAVRHU CSV//////////////////////////////////////////////


	/////////////////////CSV - EXPORT DAT - ZAKAZEK //////////////////////////////////
	 if(Form1->STATUS==Form1->OVEROVANI){

		// pokud existuje alespon jedna zakazka, ktera ma objekt a zaroven jsem na urovni Klienta tak vypisu zakazky
			if(Form1->d.v.ZAKAZKY->predchozi!=NULL  && Form1->d.v.OBJEKTY->predchozi!=NULL)//pokud existuje alespoò jedna zakázka a nìjaký objekt
		{
			Cvektory::TZakazka *Z=Form1->d.v.ZAKAZKY->dalsi;//pøeskoèí hlavièku
	while(Z!=NULL)//prochází jednotlivé zakázky
	{
		UnicodeString zakazka_name=Z->name;
		data+="Pøehled objektù a jejich nastavených parametrù u zakázky:"+S+zakazka_name+"\n";
		data+="ID"+S+"Název"+S+"Zkratka"+S+"Režim"+S+"CT [s]"+S+"Kapacita doporuèená"+S+"Kapacita nastavená"+S+"Název pohonu"+S+"Rychlost pohonu [m/min]"+S+"Rozsah pohonu [m/min]"+S+"Rozteè palcù [mm]"+S+"Délka kabiny [m]"+S+"Mezera mezi vozíky [m]\n";

		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;

		while (O!=NULL)//prochází potenciální segmenty cesty po objektech
		{
							//pokud je objekt platným segmentem cesty zakázky, vrátí ukazatel na tento segment
							Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,Z);
						if(C!=NULL){
								UnicodeString ID=C->objekt->n;
								UnicodeString name=C->objekt->name;
								UnicodeString short_name=C->objekt->short_name;
								UnicodeString rezim;
								UnicodeString CT=C->CT;
								UnicodeString kapacita=C->objekt->kapacita;
								UnicodeString kapacita_dop=C->objekt->kapacita_dop;
								UnicodeString mezera=C->objekt->mezera;
								UnicodeString nazev_pohonu;
								UnicodeString roztec_palcu;
							 //	UnicodeString rychlost_dopravniku;
								double rozsah_pohonu_od;
								double rozsah_pohonu_do;
								UnicodeString rychlost_dopravniku=C->RD;
                if(C->objekt->pohon!=NULL)
                {
								nazev_pohonu=C->objekt->pohon->name;
								roztec_palcu=C->objekt->pohon->roztec*1000.0;
								rozsah_pohonu_od=O->pohon->rychlost_od*60.0;
								rozsah_pohonu_do=O->pohon->rychlost_do*60.0;
								}
								else {nazev_pohonu="nepøiøazen";roztec_palcu="";rychlost_dopravniku="žádná";}

								//UnicodeString rychlost_od=C->objekt->pohon->rychlost_od;
								//UnicodeString rychlost_do=C->objekt->pohon->rychlost_do;
								UnicodeString delka_dopravniku=C->objekt->delka_dopravniku;
									switch(C->objekt->rezim)
								{
									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantní";delka_dopravniku="nerelevantní"; break;
									case 1:rezim="KONTINUÁLNÍ";break;
									case 2:rezim="POSTPROCESNÍ";rychlost_dopravniku="nerelevantní";break;
								}
//          //html
								data+=""+ID+S+name+S+short_name+S+rezim+S+CT+S+kapacita+S+kapacita_dop+S+nazev_pohonu+S+rychlost_dopravniku+S+rozsah_pohonu_od+"-"+rozsah_pohonu_do+S+roztec_palcu+S+delka_dopravniku+S+mezera+"\n";
								}
					O=O->dalsi;
							}
		Z=Z->dalsi;
		data+="\n";
					}
				}
		data+="\n";


		}

	}


		///////////////////////////zapis data do souboru
		TMemoryStream* MemoryStream=new TMemoryStream();
		MemoryStream->Clear();
		MemoryStream->Write(data.c_str(),data.Length());//Win kodování
		MemoryStream->SaveToFile(FileName);


		 if (export_format==3)
		 {
				if(Form1->FileName=="Nový.tispl")WebBrowser1->Navigate(ExtractFilePath(Application->ExeName)+"Nový.html");
				else WebBrowser1->Navigate(FileName);
				ShowModal();
		 }



		delete MemoryStream;
		return 1;

		}
		catch(...){;}
}
//---------------------------------------------------------------------------
void __fastcall TForm_report::scButton_csvClick(TObject *Sender)
{
	if(Form1->d.v.OBJEKTY->dalsi==NULL)Form1->MB("Žádná data k reportu!");//pokud existují nìjaka data
	else
	{
		//pøíprava SaveDialogu
		Form1->SaveDialog->Title="Vytvoøit report...";
		Form1->SaveDialog->DefaultExt="*.csv";
		Form1->SaveDialog->Filter="Soubory formátu CSV (*.csv)|*.csv|Soubory formátu XLS (*.xls)|*.xls";

		//pøedvyplnìní názvem stejnojmeným souboru
		UnicodeString FN=Form1->FileName;
		if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
		Form1->SaveDialog->FileName=FN;

		if(Form1->SaveDialog->Execute())
		{
			Screen->Cursor=crHourGlass;//zmìní kurzor na pøesýpací hodiny
			//nastavení formátu
			UnicodeString export_format="csv";
			if(Form1->SaveDialog->FileName.SubString(Form1->SavePictureDialog1->FileName.Length()-2,3).LowerCase() =="xls")export_format="xls";
			//samotné uložení
			if(Form_report->ulozit_report(Form1->SaveDialog->FileName)==1)Form1->SB("Report do "+export_format+" dokonèen.");else Form1->SB("Chyba pøi tvorbì reportu!");
			//postprocesní záležitost
			Screen->Cursor=crDefault;//zmìní kurzor na default
			ShellExecute(0,L"open",UnicodeString(Form1->SaveDialog->FileName).c_str(),0,0,SW_SHOWNORMAL);;//otevøe výstup
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_report::scButton_htmlClick(TObject *Sender)
{

if(Form1->d.v.OBJEKTY->dalsi==NULL)//pokud existují nìjaka data
		Form1->MB("Žádná data k reportu!");
	else
	{
		//pøíprava SaveDialogu
		Form1->SaveDialog->Title="Vytvoøit report...";
		Form1->SaveDialog->DefaultExt="*.html";
		Form1->SaveDialog->Filter="Soubory formátu HTML (*.html)|*.html";

		//pøedvyplnìní názvem stejnojmeným souboru
		UnicodeString FN=Form1->FileName;
		if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
		Form1->SaveDialog->FileName=FN;

		if(Form1->SaveDialog->Execute())
		{
			Screen->Cursor=crHourGlass;//zmìní kurzor na pøesýpací hodiny
			//nastavení formátu
			UnicodeString export_format="html";
			if(Form1->SaveDialog->FileName.SubString(Form1->SavePictureDialog1->FileName.Length()-2,3).LowerCase() =="html")export_format="html";
			//samotné uložení
			if(Form_report->ulozit_report(Form1->SaveDialog->FileName)==1)Form1->SB("Report do "+export_format+" dokonèen.");else Form1->SB("Chyba pøi tvorbì reportu!");
			//postprocesní záležitost
			Screen->Cursor=crDefault;//zmìní kurzor na default
			ShellExecute(0,L"open",UnicodeString(Form1->SaveDialog->FileName).c_str(),0,0,SW_SHOWNORMAL);;//otevøe výstup
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm_report::MaxButtonClick(TObject *Sender)
{
	if (FMaximized)//zmenšení
	{
			 //BoundsRect =  FOldBoundsRect;
			 FMaximized = false;
			// scLabel_titulek->DragForm = true;
			 MaxButton->GlyphOptions->Kind = scgpbgkMaximize;
			// scGPSizeBox->Visible = true;
			 Form_report->Width=Screen->Width/3*2;//zmenší formuláø na 2/3 jeho velikosti
			 Form_report->Height=Screen->Height/3*2;//zmenší formuláø na 2/3 jeho velikosti
		 //	 scSplitView_OPTIONS->Opened=false;
			// scSplitView_MENU->Opened=false;
	}
	else //maximalizace
	{
			//FOldBoundsRect = BoundsRect;
			BoundsRect = scStyledForm1->GetMaximizeBounds();
			FMaximized = true;
		 //	scLabel_titulek->DragForm = false;
			MaxButton->GlyphOptions->Kind = scgpbgkRestore;
		 //	scGPSizeBox->Visible = false;
		 //	scSplitView_OPTIONS->Opened=false;
		//	scSplitView_MENU->Opened=false;
	}
}
//---------------------------------------------------------------------------
