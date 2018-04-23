//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "report.h"
#include "Unit1.h"
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
		//ZDM get_LT_a_max_min_TT();//zajistí hodnoty

		//generuj_POHONY(); už nepoužíváme, ale možná èasem budeme

		AnsiString data="";//celková textová data k exportu

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

		 UnicodeString PP_mnozstvi=Form1->d.v.PP.mnozstvi;
		 UnicodeString dni_rok=Form1->d.v.PP.dni_rok;
		 UnicodeString hod_den=Form1->d.v.PP.hod_den;
		 UnicodeString efektivita=Form1->d.v.PP.efektivita;
		 UnicodeString cas_start=Form1->d.v.PP.cas_start;
		 UnicodeString delka_voziku=Form1->d.v.PP.delka_voziku;
		 UnicodeString titulek_projektu;
		 UnicodeString PP_TT=Form1->d.v.PP.TT;
		 UnicodeString sirka_voziku=Form1->d.v.PP.sirka_voziku;

		 try{

	 //////////VYGENEROVÁNÍ HTML HLAVIÈKY PODLE TOHO ZDA EXISTUJE ZAKÁZKA ÈI NIKOLIV ///////////////////////////////

		if(export_format==3)  {

			data+="<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\" integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";
			data+="<link rel=\"stylesheet\"  href=\"\../linky_layouts/styly/css/bootstrap.min.css\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";

			//data+="<script src=\"\../linky_layouts/styly/js/bootstrap.min.js\"></script>";
			data+="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		 //	data+="<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js\" integrity=\"sha384-alpBpkh1PFOepccYVYDB4do5UnbKysX5WZXm3XxPqe5iKTfUKjNkCk9SaVuEZflJ\" crossorigin=\"anonymous\"></script>";
			data+="<div class=\"container-fluid\">";
		/*TOTO ZAKOMENTOVAL M - 22.3.2018
		if(Form1->d.v.ZAKAZKY->dalsi!=NULL){   //když existuje zakázka - vypiš tuto hlavièku //UnicodeString(Form1->scLabel_titulek->Caption)
		data+="<form></br>";    .
		data+="<h4>Parametry zakázky <b></b></h4></br>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Požadované celkové množství</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+PP_mnozstvi+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Poèet pracovních dní [rok]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+dni_rok+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Poèet pracovních hodin [den]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+hod_den+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Efektivita [%]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+efektivita+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Zaèátek výroby</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+cas_start+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Délka vozíku</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_voziku+"\"></div></div>";

		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Lead Time [min]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.vrat_LT()/60)+"\"></div></div>";

		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP z vozíkù na èasových osách</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(0))+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP souètem kapacit zadaných</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(1))+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP souètem kapacit vypoètených v èasových osách</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(2))+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP tradièní výpoèet</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(3))+"\"></div></div>";


		data+="</form></div></br>";

		data+="<div class=\"container-fluid\"><form>";  }

		else {   //pokud neexistuje zakázka vypiš pouze info z NÁVRHU
		*/

			data+="<form></br>";
			data+="<h4>Parametry linky <b>"+UnicodeString(Form1->scLabel_titulek->Caption)+"</b></h4></br>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">TaktTime [s]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+PP_TT+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Délka Jigu [m]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_voziku+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Šíøka Jigu [m]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+sirka_voziku+"\"></div></div>";

			//LT
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Lead Time [min]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.vrat_LT()/60.0)+"\"></div></div>";
			//WIP
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP z vozíkù na èasových osách</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(0))+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP souètem kapacit zadaných</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(1))+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP souètem kapacit vypoètených v èasových osách</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(2))+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">WIP tradièní výpoèet</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+UnicodeString(Form1->d.v.WIP(3))+"\"></div></div>";


			data+="</form></div></br>";
			data+="<div class=\"container-fluid\"><form>";

		//}

			//Pøehled doporuèených pohonù
			UnicodeString dopP=Form1->d.v.navrhni_POHONY("</tr>");
			if(dopP!="")
			{
				data+="<h4>Pøehled doporuèených pohonù</h4>";
				data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">Název</th><th scope=\"col\">Rychlost[m/min]</th><th scope=\"col\">Rozestup aktivní palce [m]</th></tr></thead>";
				data+="<tr>"+dopP+"</tr>";
				data+="</table></br>";
			}

			//Pøehled nadefinovaných pohonù - doplnil M
			Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;
			if(P!=NULL)
			{
				data+="<h4>Pøehled nadefinovaných pohonù</h4></br>";
				data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Název</th><th scope=\"col\">Rychlost od [m/min]</th><th scope=\"col\">Rychlost do [m/min]</th><th scope=\"col\">Akt. rychlost [m/min]</th><th scope=\"col\">Rozteè palcù [mm]</th><th scope=\"col\">Rozestup vzdálenost aktivní palce [m]</th><th scope=\"col\">Rozestup poèet palcù</th><th scope=\"col\">Používán</th></tr></thead>";
				while(P!=NULL)
				{
					UnicodeString ID=P->n;
					UnicodeString name=P->name;
					UnicodeString rychlost_od=P->rychlost_od*60.0;
					UnicodeString rychlost_do=P->rychlost_do*60.0;
					UnicodeString RD=P->aRD;
					UnicodeString R=P->roztec*1000.0;
					UnicodeString Rz=P->Rz;
					UnicodeString Rx=P->Rx;
					UnicodeString Pouzit=Form1->d.v.vypis_objekty_vyuzivajici_pohon(P->n);if(Pouzit=="")Pouzit="nepoužíván";/*if(Form1->d.v.pohon_je_pouzivan(P->n))Pouzit="Ano";*/
					data+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+rychlost_od+"</td><td>"+rychlost_do+"</td><td>"+RD+"</td><td>"+R+"</td><td>"+Rz+"</td><td>"+Rx+"</td><td>"+Pouzit+"</td></tr>";
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
			data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Název</th><th scope=\"col\">Zkratka</th><th scope=\"col\">Režim</th><th scope=\"col\">Název pohonu</th><th scope=\"col\">Technolog. èas [s]</th><th scope=\"col\">Rychlost pohonu</th><th scope=\"col\">Délka kabiny</th><th scope=\"col\">Kapacita [vozíku a mezer]</th><th scope=\"col\">Pozice [vozíkù]</th><th scope=\"col\">Orientace[°]</th><th scope=\"col\">Mezera mezi vozíky [m]</th></tr></thead>";
			Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
			while (O!=NULL)//prochází potenciální segmenty cesty po objektech
			{
					UnicodeString ID=O->id;
					UnicodeString name=O->name;
					UnicodeString short_name=O->short_name;
					UnicodeString mezera=O->mezera;
					UnicodeString rezim;
					UnicodeString CT=O->CT;
					UnicodeString kapacita=O->kapacita;
					UnicodeString kapacita_dop=O->kapacita_dop;
					UnicodeString pozice=O->pozice;
					UnicodeString nazev_pohonu;

					if(O->pohon!=NULL)nazev_pohonu=O->pohon->name;
					else nazev_pohonu="Nepøiøazen";

					UnicodeString rychlost_dopravniku=Form1->ms.MyToDouble(O->RD)*60.0;    //vždy budu zobrazovat v m/min
					UnicodeString delka_dopravniku=O->delka_dopravniku;
					switch(O->rezim)
					{
						case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantní";delka_dopravniku="nerelevantní"; break;
						case 1:rezim="KONTINUÁLNÍ";break;
						case 2:rezim="POSTPROCESNÍ";rychlost_dopravniku="nerelevantní";break;
					}
					data+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+short_name+"</td><td>"+rezim+"</td><td>"+nazev_pohonu+"</td><td>"+CT+"</td><td>"+rychlost_dopravniku+"</td><td>"+delka_dopravniku+"</td><td>"+kapacita+"</td>"+/*<td>"+kapacita_dop+"</td>*/"<td>"+pozice+"</td><td>"+O->rotace+"</td><td>"+mezera+"</td></tr>";
					O=O->dalsi;
			}
			data+="</tbody></table></br>";
			data+="</form></div></br>";

      //VÝPIS NEVYTÍŽENÝCH OBJEKTÙ
			UnicodeString Nevytizene_objekty=Form1->d.v.vypis_objekty_mimo_100vytizeni(false);
			if(Nevytizene_objekty!="")
			{
				data+="<h4>Pøehled objektù s menší než 100% vytížeností</h4></br>";
				data+=Nevytizene_objekty+"</br>";
      }
		}
		//////////////HTML EXPORT - ZAKÁZKY////////////////////////////////////////////////
	 // zobrazení dat ze zakázek ///////////////////////////////

 		if(Form1->STATUS==Form1->OVEROVANI)  {

	if(Form1->d.v.ZAKAZKY->predchozi!=NULL  && Form1->d.v.OBJEKTY->predchozi!=NULL)//pokud existuje alespoò jedna zakázka a nìjaký objekt
{
	Cvektory::TZakazka *Z=Form1->d.v.ZAKAZKY->dalsi;//pøeskoèí hlavièku
	while(Z!=NULL)//prochází jednotlivé zakázky
	{
		UnicodeString zakazka_name=Z->name;
		data+="<h4>Pøehled objektù a jejich nastavených parametrù u zakázky: <b>"+zakazka_name+"</b></h4></br>";
		data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Název</th><th scope=\"col\">Zkratka</th><th scope=\"col\">Režim</th><th scope=\"col\">CT [s]</th><th scope=\"col\">Kapacita doporuèená</th><th scope=\"col\">Kapacita nastavená</th><th scope=\"col\">Název pohonu</th><th scope=\"col\">Rychlost pohonu [m/min]</th><th scope=\"col\">Rozsah pohonu [m/min]</th><th scope=\"col\">Rozteè palcù [mm]</th><th scope=\"col\">Délka kabiny [m]</th><th scope=\"col\">Mezera mezi vozíky [m]</th></tr></thead>";

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
								UnicodeString rychlost_dopravniku=C->RD*60;
								UnicodeString rozsah_pohonu_od;
								UnicodeString rozsah_pohonu_do;

							if(C->objekt->pohon!=NULL)  {
								nazev_pohonu=C->objekt->pohon->name;
								roztec_palcu=C->objekt->pohon->roztec;
								rozsah_pohonu_od=C->objekt->pohon->rychlost_od;
								rozsah_pohonu_do=C->objekt->pohon->rychlost_do;

								rozsah_pohonu_od=Form1->ms.MyToDouble(rozsah_pohonu_od)*60.0;
								rozsah_pohonu_do=Form1->ms.MyToDouble(rozsah_pohonu_do)*60.0;
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
								data+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+short_name+"</td><td>"+rezim+"</td><td>"+CT+"</td><td>"+kapacita_dop+"</td><td>"+kapacita+"</td><td>"+nazev_pohonu+"</td><td>"+rychlost_dopravniku+"</td><td>"+rozsah_pohonu_od+"-"+rozsah_pohonu_do+"</td><td>"+roztec_palcu+"</td><td>"+delka_dopravniku+"</td><td>"+mezera+"</td></tr>";
								}
					O=O->dalsi;
		}
		Z=Z->dalsi;

		data+="</tbody></table></br>";
			}
		}

		data+="</form></div>";
		}


	}
	//////KONEC HTML EXPORTU///////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	/////////////////////CSV///////////////////////////////////////////////////////////
		if(export_format==1)  {

	 //////////////////OBECNÁ HLAVIÈKA CSV///////////////////////////////////
		data+="Parametry projektu"+S+UnicodeString(Form1->scLabel_titulek->Caption)+"\n";
		/*Toto zakomentoval Martin 22.3.2018
		data+="Požadované celkové množství"+S+PP_mnozstvi+"\n";
		data+="Poèet pracovních dní [rok]"+S+dni_rok+"\n";
		data+="Poèet pracovních hodin [den]"+S+hod_den+"\n";
		data+="Efektivita [%]"+S+efektivita+"\n";
		data+="Zaèátek výroby"+S+cas_start+"\n";
		data+="Délka vozíku"+S+delka_voziku+"\n";
		*/
		data+="TaktTime [s]"+S+PP_TT+"\n";
		data+="Délka Jigu [m]"+S+delka_voziku+"\n";
		data+="Šíøka Jigu [m]"+S+sirka_voziku+"\n";
		data+="Lead Time [min]"+S+UnicodeString(Form1->d.v.vrat_LT()/60.0)+"\n";
		data+="WIP z vozíkù na èasových osách"+S+UnicodeString(Form1->d.v.WIP(0))+"\n";
		data+="WIP souètem kapacit zadaných"+S+UnicodeString(Form1->d.v.WIP(1))+"\n";
		data+="WIP souètem kapacit vypoètených v èasových osách"+S+UnicodeString(Form1->d.v.WIP(2))+"\n";
		data+="WIP tradièní výpoèet"+S+UnicodeString(Form1->d.v.WIP(3))+"\n";

		data+="\n";

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

				//KONEC OBECNÉ HLAVIÈKY CSV//
 /////////////////////////////////////////////////////////////////////////////////////////////


 ////////////////////////CSV EXPORT V REŽIMU NÁVRHU /////////////////////////////////////////
			if(Form1->STATUS==Form1->NAVRH) {


		data+="Architekt: Pøehled objektù a jejich parametrù\n";
		data+="ID"+S+"Název"+S+"Zkratka"+S+"Režim"+S+"CT [s]"+S+"Kapacita doporuèená"+S+"Kapacita nastavená"+S+"Název pohonu"+S+"Rychlost pohonu [m/min]"+S+"Rozsah pohonu [m/min]"+S+"Rozteè palcù [mm]"+S+"Délka kabiny [m]"+S+"Mezera mezi vozíky [m]\n";

		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;

		while (O!=NULL)//prochází potenciální segmenty cesty po objektech
		{
								UnicodeString ID=O->id;
								UnicodeString name=O->name;
								UnicodeString short_name=O->short_name;
								UnicodeString mezera=O->mezera;
								UnicodeString rezim;
								UnicodeString CT=O->CT;
								UnicodeString kapacita=O->kapacita;
								UnicodeString kapacita_dop=O->kapacita_dop;
								UnicodeString nazev_pohonu;
								UnicodeString roztec_palcu;
								double rozsah_pohonu_od;
								double rozsah_pohonu_do;

								if(O->pohon!=NULL)  {
								nazev_pohonu=O->pohon->name;
								roztec_palcu=O->pohon->roztec*1000.0;
								rozsah_pohonu_od=O->pohon->rychlost_od*60.0;
								rozsah_pohonu_do=O->pohon->rychlost_do*60.0;
							 //ShowMessage(rozsah_pohonu_do);
							 //	rozsah_pohonu_od=Form1->ms.MyToDouble(rozsah_pohonu_od)*60.0;
							 //	rozsah_pohonu_do=Form1->ms.MyToDouble(rozsah_pohonu_do)*60.0;
								}
								else {nazev_pohonu="Nepøiøazen";roztec_palcu="";}

								UnicodeString rychlost_dopravniku=Form1->ms.MyToDouble(O->RD)*60.0;    //vždy budu zobrazovat v m/min
								UnicodeString delka_dopravniku=O->delka_dopravniku;
									switch(O->rezim)
								{
									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantní";delka_dopravniku="nerelevantní"; break;
									case 1:rezim="KONTINUÁLNÍ";break;
									case 2:rezim="POSTPROCESNÍ";rychlost_dopravniku="nerelevantní";break;
								}
////         	//	 ShowMessage(ID+name+short_name+rezim+CT+kapacita+kapacita_dop+nazev_pohonu+rychlost_dopravniku+delka_dopravniku+roztec_palcu+rozsah_pohonu_od+rozsah_pohonu_do);
							data+=""+ID+S+name+S+short_name+S+rezim+S+CT+S+kapacita+S+kapacita_dop+S+nazev_pohonu+S+rychlost_dopravniku+S+rozsah_pohonu_od+"-"+rozsah_pohonu_do+S+roztec_palcu+S+delka_dopravniku+S+mezera+"\n";
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
									if(C->objekt->pohon!=NULL)  {
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
