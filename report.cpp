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
//	Cvektory::TObjekt *O=F->d.v.OBJEKTY->dalsi;
//	while(O!=NULL)
//	{
//		UnicodeString n=O->n;
//		UnicodeString id=O->id;
//		UnicodeString name=O->name;
//		UnicodeString X=O->X;
//		UnicodeString Y=O->Y;
//		UnicodeString pocet_bodu="0";
//		if(O->body!=NULL && O->body->predchozi->n>0)pocet_bodu=O->body->predchozi->n;
//		UnicodeString sirka_steny=O->sirka_steny;
//		UnicodeString Xt=O->Xt;
//		UnicodeString Yt=O->Yt;
//		UnicodeString orientace_text=O->orientace_text;
//		UnicodeString Xp=O->Xp;
//		UnicodeString Yp=O->Yp;
//		UnicodeString rezim=O->rezim;
//		UnicodeString orientace=O->orientace;
//		UnicodeString pohon="Nepøiøazen/needitován";
//		if(O->pohon!=NULL)pohon=O->pohon->name;
//		UnicodeString prvni_element=O->element->name;
//		UnicodeString min_prujezdni_profil_x=O->min_prujezdni_profil.x;
//		UnicodeString min_prujezdni_profil_y=O->min_prujezdni_profil.y;
//		UnicodeString koty_elementu_offset_x=O->koty_elementu_offset.x;
//		UnicodeString koty_elementu_offset_y=O->koty_elementu_offset.y;
//		UnicodeString pocet_komor=0;
//		if(O->komora!=NULL && O->predchozi->n>0)pocet_komor=O->komora->predchozi->n;
//		UnicodeString zobrazit_koty="Ne";
//		if(O->zobrazit_koty)zobrazit_koty="Ano";
//		UnicodeString zobrazit_mGrid="Ne";
//		if(O->zobrazit_mGrid)zobrazit_mGrid="Ano";
//		UnicodeString uzamknout_nahled="Ne";
//		if(O->uzamknout_nahled)uzamknout_nahled="Ano";
//		//vypsaní - uložení
//
//
//		O=O->dalsi;
//	}
//	delete O;O=NULL;



		AnsiString data_HTML="",data_CSV="",data_export="";//celková textová data k exportu

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
	UnicodeString vyska_voziku=Form1->d.v.PP.vyska_jig*(1+999.0*Sirkaunit);
	UnicodeString delka_podvozek=Form1->d.v.PP.delka_podvozek*(1+999.0*Sirkaunit);
	UnicodeString sirka_podvozek=Form1->d.v.PP.sirka_podvozek*(1+999.0*Sirkaunit);
	UnicodeString uchyt_pozice=Form1->d.v.PP.uchyt_pozice*(1+999.0*Sirkaunit);


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


	try
	{
		//////////VYGENEROVÁNÍ HTML HLAVIÈKY PODLE TOHO ZDA EXISTUJE ZAKÁZKA ÈI NIKOLIV ///////////////////////////////
		if(export_format==3||true)
		{
			////HTML èást
			data_HTML+="<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\" integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";
			data_HTML+="<link rel=\"stylesheet\"  href=\"\../linky_layouts/styly/css/bootstrap.min.css\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";
			//data_HTML+="<script src=\"\../linky_layouts/styly/js/bootstrap.min.js\"></script>";
			data_HTML+="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		 //	data_HTML+="<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js\" integrity=\"sha384-alpBpkh1PFOepccYVYDB4do5UnbKysX5WZXm3XxPqe5iKTfUKjNkCk9SaVuEZflJ\" crossorigin=\"anonymous\"></script>";
			data_HTML+="<div class=\"container-fluid\">";
			data_HTML+="<form></br>";
			data_HTML+="<h4>Parametry linky <b>"+UnicodeString(Form1->scLabel_titulek->Caption)+"</b></h4></br>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Takt Time"+UnicodeString(jednotky_casove)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+PP_TT+"\"></div></div>";
      data_HTML+="<h4>Parametry Jigu</h4></br>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Délka Jigu "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_voziku+"\"></div></div>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Šíøka Jigu "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+sirka_voziku+"\"></div></div>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Výška Jigu "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+vyska_voziku+"\"></div></div>";
			data_HTML+="<h4>Parametry podvozku</h4></br>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Délka podvozku "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_podvozek+"\"></div></div>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Šíøka podvozku "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+sirka_podvozek+"\"></div></div>";
			data_HTML+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Uchopování palce "+UnicodeString(jednotky_rozmery)+"</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+uchyt_pozice+"\"></div></div>";
			data_HTML+="</form></div></br>";
			data_HTML+="<div class=\"container-fluid\"><form>";

			////CSV èást
			data_CSV+="Parametry linky"+UnicodeString(Form1->scLabel_titulek->Caption)+"\n";
			data_CSV+="Takt time "+UnicodeString(jednotky_casove)+S+UnicodeString(PP_TT)+"\n";
			data_CSV+="\n\n";
			data_CSV+="Parametry Jigu\n";
			data_CSV+="Delka jigu "+UnicodeString(jednotky_rozmery)+S+UnicodeString(delka_voziku)+"\n";
			data_CSV+="Šíøka jigu "+UnicodeString(jednotky_rozmery)+S+UnicodeString(sirka_voziku)+"\n";
			data_CSV+="Výška jigu "+UnicodeString(jednotky_rozmery)+S+UnicodeString(vyska_voziku)+"\n";
			data_CSV+="\n\n";
			data_CSV+="Parametry podvozku\n";
			data_CSV+="Delka podvozku "+UnicodeString(jednotky_rozmery)+S+UnicodeString(delka_podvozek)+"\n";
			data_CSV+="Šíøka podvozku "+UnicodeString(jednotky_rozmery)+S+UnicodeString(sirka_podvozek)+"\n";
			data_CSV+="Uchopování palce "+UnicodeString(jednotky_rozmery)+S+UnicodeString(uchyt_pozice)+"\n";
			data_CSV+="\n\n";

			//Pøehled nadefinovaných pohonù - doplnil M
			Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;
			if(P!=NULL)
			{
				////HTML èást
				data_HTML+="<h4>Pøehled pohonù</h4></br>";
				data_HTML+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Název</th><th scope=\"col\">Rychlost od "+jednotky_RD+"</th><th scope=\"col\">Rychlost do "+jednotky_RD+"</th><th scope=\"col\">Akt. rychlost "+jednotky_RD+"</th><th scope=\"col\">Rozteè palcù "+jednotky_vzdalenost+"</th><th scope=\"col\">Používán</th></tr></thead>";
				////CSV èást
				data_CSV+="Pøehled pohonù"+S+"\n";
				data_CSV+="ID"+S+"Název"+S+"Rychlost od "+jednotky_RD+S+"Rychlost do "+jednotky_RD+S+"Akt. rychlost "+jednotky_RD+S+"Rozteè palcù "+jednotky_vzdalenost+S+"Používán"+"\n";
        ////extrakce dat
				while(P!=NULL)
				{
					////naètení dat
					UnicodeString ID=P->n;
					UnicodeString name=P->name;
					UnicodeString rychlost_od=P->rychlost_od*(1+59.0*aRDunit);
					UnicodeString rychlost_do=P->rychlost_do*(1+59.0*aRDunit);
					UnicodeString RD=P->aRD*(1+59.0*aRDunit);
					UnicodeString R=P->roztec*(1+999.0*Runit);
					UnicodeString Pouzit=Form1->d.v.vypis_objekty_vyuzivajici_pohon(P->n);if(Pouzit=="")Pouzit="nepoužíván";/*if(Form1->d.v.pohon_je_pouzivan(P->n))Pouzit="Ano";*/
					////HTML èást
					data_HTML+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+rychlost_od+"</td><td>"+rychlost_do+"</td><td>"+RD+"</td><td>"+R+"</td><td>"+Pouzit+"</td></tr>";
					////CSV èást
					data_CSV+=""+ID+S+name+S+rychlost_od+S+rychlost_do+S+RD+S+R+S+Pouzit+"\n";
          //ukazatelové záležitosti
					P=P->dalsi;
				}
				////HTML èást
				data_HTML+="</tbody></table></br>";
				////CSV èást
				data_CSV+="\n\n";
			}
			P=NULL;delete P;

			//////////////////HTML EXPORT V NÁVRHU///////////////////////////////////

			////////Objekty
      ////HTML èást
			data_HTML+="<h4>Pøehled objektù a jejich parametrù</h4>";
			data_HTML+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">n</th><th scope=\"col\">ID</th><th scope=\"col\">Název</th></tr></thead>";
			////CSV èást
			data_CSV+="Pøehled elementù a jejich parametrù\n";
			data_CSV+="n"+S+"ID"+S+"Název"+S+"Pohon\n";
			Cvektory::TObjekt *O=F->d.v.OBJEKTY->dalsi;
			while(O!=NULL)
			{
				////naètení dat
				UnicodeString pohon="-";
				if(O->pohon)pohon=O->pohon->name;
				////HTML èást
				data_HTML+="<tr><th scope=\"row\">"+UnicodeString(O->n)+"</th><td>"+O->id+"</td><td>"+O->name.UpperCase()+"</td><td>"+pohon+"</td></tr>";
	   		////CSV èást
				data_CSV+=UnicodeString(O->n)+S+O->id+S+O->name.UpperCase()+S+pohon+"\n";
				////ukazatelové záležitosti
				O=O->dalsi;
			}
			////ukazatelové záležitosti
			delete O;O=NULL;
      ////HTML èást
			data_HTML+="</tbody></table></br>";
			////CSV èást
			data_CSV+="\n\n";

			if(Form1->STATUS==Form1->NAVRH)
			{
        ////////Elementy
				//data_HTML+="<h4>Architekt: Pøehled objektù a jejich parametrù</h4></br>";
				////HTML èást
				data_HTML+="<h4>Pøehled elementù a jejich parametrù</h4>";
				data_HTML+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">eID</th><th scope=\"col\">Název</th><th scope=\"col\">Objekt</th><th scope=\"col\">Pohon</th><th scope=\"col\">PT1 [s]</th><th scope=\"col\">LO1 [m]</th><th scope=\"col\">PT2 [s]</th><th scope=\"col\">LO2 [m]</th><th scope=\"col\">LO vyosení [m]</th><th scope=\"col\">PT otoè [s]</th><th scope=\"col\">Delka otoè [m]</th><th scope=\"col\">Zóna pøed [m]</th><th scope=\"col\">Zóna za [m]</th><th scope=\"col\">RT [s]</th><th scope=\"col\">WT palec [s]</th><th scope=\"col\">Èekání [s]</th><th scope=\"col\">Sparovaný</th><th scope=\"col\">Potencionální<br> poèet pozic</th><th scope=\"col\">Nastavený<br> poèet pozic</th></tr></thead>";
				////CSV èást
				data_CSV+="Pøehled elementù a jejich parametrù\n";
				data_CSV+="eID"+S+"Nazev"+S+"Objekt"+S+"Pohon"+S+"PT1 [s]"+S+"LO1 [m]"+S+"PT2 [s]"+S+"LO2 [m]"+S+"LO vyosení [m]"+S+"PT otoè [s]"+S+"Delka otoè [m]"+S+"Zóna pøed [m]"+S+"Zóna za [m]"+S+"RT [s]"+S+"WT palec [s]"+S+"Èekání [s]"+S+"Spárovaný"+S+"Potencionální poèet pozic"+S+"Nastavený poèet pozic\n";
				Cvektory::TElement *E=F->d.v.ELEMENTY->dalsi;
      	while(E!=NULL)
      	{
          ////naètení dat
					UnicodeString eID=E->eID;
					UnicodeString name=E->name;
      		UnicodeString PTotoc=F->m.round2double(E->PTotoc,3);
      		UnicodeString OTOC_delka=F->m.round2double(E->OTOC_delka,3);
      		UnicodeString zona_pred=F->m.round2double(E->zona_pred,3);
      		UnicodeString zona_za=F->m.round2double(E->zona_za,3);
      		UnicodeString WT=F->m.round2double(E->WT,3);
      		UnicodeString PD=F->m.round2double(E->data.PD,3);
      		UnicodeString orientace_jig_pred=E->data.orientace_jig_pred;
      		UnicodeString LO1=F->m.round2double(E->data.LO1,3);
      		UnicodeString LO2=F->m.round2double(E->data.LO2,3);
      		UnicodeString LO_pozice=F->m.round2double(E->data.LO_pozice,3);
      		UnicodeString PT1=F->m.round2double(E->data.PT1,3);
      		UnicodeString PT2=F->m.round2double(E->data.PT2,3);
      		UnicodeString WTstop=F->m.round2double(E->data.WTstop,3);
      		UnicodeString RT_x=F->m.round2double(E->data.RT.x,3);
      		UnicodeString RT_y=F->m.round2double(E->data.RT.y,3);
      		UnicodeString pocet_pozic=E->data.pocet_pozic;
      		UnicodeString pocet_voziku=E->data.pocet_voziku;
      		if(F->d.v.vrat_druh_elementu(E)!=0)pocet_pozic=pocet_voziku="-";
      		UnicodeString objekt_n=E->objekt_n;
      		AnsiString nazev_objektu = F->d.v.vrat_objekt(E->objekt_n)->name;
      		nazev_objektu=nazev_objektu.UpperCase();
      		UnicodeString pohon="-";
      		if(E->pohon!=NULL)pohon=E->pohon->name;
      		UnicodeString sparovany="-";
      		if(E->sparovany!=NULL)sparovany=E->sparovany->name;
      		if(PT1=="0")PT1="-";if(PT2=="0")PT2="-";if(PTotoc=="0")PTotoc="-";
      		if(LO1=="0")LO1="-";if(LO2=="0")LO2="-";if(OTOC_delka=="0")OTOC_delka="-";if(LO_pozice=="0")LO_pozice="-";
					if(zona_pred=="0")zona_pred="-";if(zona_za=="0")zona_za="-";if(RT_x=="0")RT_x="-";if(WT=="0")WT="-";if(WTstop=="0")WTstop="-";
					////zapsání do dat
					if(eID!=MaxInt)
					{
						////HTML èást
						data_HTML+="<tr><th scope=\"row\">"+eID+"</th><td>"+name+"</td><td>"+nazev_objektu+"</td><td>"+pohon+"</td><td>"+PT1+"</td><td>"+LO1+"</td><td>"+PT2+"</td><td>"+LO2+"</td><td>"+LO_pozice+"</td><td>"+PTotoc+"</td><td>"+OTOC_delka+"</td><td>"+zona_pred+"</td><td>"+zona_za+"</td><td>"+RT_x+"</td><td>"+WT+"</td><td>"+WTstop+"</td><td>"+sparovany+"</td><td>"+pocet_pozic+"</td><td>"+pocet_voziku+"</td></tr>";
						////CSV èást
						data_CSV+=eID+S+name+S+nazev_objektu+S+pohon+S+PT1+S+LO1+S+PT2+S+LO2+S+LO_pozice+S+PTotoc+S+OTOC_delka+S+zona_pred+S+zona_za+S+RT_x+S+WT+S+WTstop+S+sparovany+S+pocet_pozic+S+pocet_voziku+"\n";
					}
					//ukazatelové záležitosti
					E=E->dalsi;
				}
				delete E;E=NULL;
				////HTML èást
				data_HTML+="</tbody></table></br>";
				////CSV èást
				data_CSV+="\n\n";

				////////Zprávy
				////HTML èást
				data_HTML+="<h4>Zprávy o lince</h4>";
				data_HTML+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">n</th><th scope=\"col\">Typ</th><th scope=\"col\">Popis</th><th scope=\"col\">Element</th><th scope=\"col\">Objekt</th></tr></thead>";
				////CSV èást
				data_CSV+="Zprávy o lince\n";
				data_CSV+="n"+S+"Typ"+S+"Popis"+S+"Element"+S+"Objekt"+S+"\n";
				Cvektory::TZprava *Z=F->d.v.ZPRAVY;//nepøeskakovat hlavièku
			  while(Z!=NULL)
			  {
					////naètení dat
					if(Z->n>0)
					{
				  	UnicodeString typ="Eror";
				  	if(Z->zID==1)typ="Warning";
				  	////HTML èást
				  	data_HTML+="<tr><th scope=\"row\">"+UnicodeString(Z->n)+"</th><td>"+typ+"</td><td>"+F->d.v.getVID(Z->VID)+"</td><td>"+Z->Element->name+"</td><td>"+F->d.v.vrat_objekt(Z->Element->objekt_n)->name.UpperCase()+"</td></tr>";
				  	////CSV èást
						data_CSV+=UnicodeString(Z->n)+S+typ+S+F->d.v.getVID(Z->VID)+S+Z->Element->name+S+F->d.v.vrat_objekt(Z->Element->objekt_n)->name.UpperCase()+"\n";
					}
					//ukazatelové záležitosti
					Z=Z->dalsi;
			  }
			  delete Z;Z=NULL;
				//data_HTML+="<tbody><tr><td>";
				////HTML èást
				data_HTML+="</form></div></br>";  //celkové ukonèení celého divu
				////CSV èást
				data_CSV+="\n\n";
			}
		}
		//////KONEC HTML A CSV EXPORTU///////////////////////////////////////////////////////


	/////////////////////CSV - EXPORT DAT - ZAKAZEK //////////////////////////////////
//	 if(Form1->STATUS==Form1->OVEROVANI){
//
//		// pokud existuje alespon jedna zakazka, ktera ma objekt a zaroven jsem na urovni Klienta tak vypisu zakazky
//			if(Form1->d.v.ZAKAZKY->predchozi!=NULL  && Form1->d.v.OBJEKTY->predchozi!=NULL)//pokud existuje alespoò jedna zakázka a nìjaký objekt
//		{
//			Cvektory::TZakazka *Z=Form1->d.v.ZAKAZKY->dalsi;//pøeskoèí hlavièku
//	while(Z!=NULL)//prochází jednotlivé zakázky
//	{
//		UnicodeString zakazka_name=Z->name;
//		data+="Pøehled objektù a jejich nastavených parametrù u zakázky:"+S+zakazka_name+"\n";
//		data+="ID"+S+"Název"+S+"Zkratka"+S+"Režim"+S+"CT [s]"+S+"Kapacita doporuèená"+S+"Kapacita nastavená"+S+"Název pohonu"+S+"Rychlost pohonu [m/min]"+S+"Rozsah pohonu [m/min]"+S+"Rozteè palcù [mm]"+S+"Délka kabiny [m]"+S+"Mezera mezi vozíky [m]\n";
//
//		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
//
//		while (O!=NULL)//prochází potenciální segmenty cesty po objektech
//		{
//							//pokud je objekt platným segmentem cesty zakázky, vrátí ukazatel na tento segment
//							Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,Z);
//						if(C!=NULL){
//								UnicodeString ID=C->objekt->n;
//								UnicodeString name=C->objekt->name;
//								UnicodeString short_name=C->objekt->short_name;
//								UnicodeString rezim;
//								UnicodeString CT=C->CT;
//								UnicodeString kapacita=C->objekt->kapacita;
//								UnicodeString kapacita_dop=C->objekt->kapacita_dop;
//								UnicodeString mezera=C->objekt->mezera;
//								UnicodeString nazev_pohonu;
//								UnicodeString roztec_palcu;
//							 //	UnicodeString rychlost_dopravniku;
//								double rozsah_pohonu_od;
//								double rozsah_pohonu_do;
//								UnicodeString rychlost_dopravniku=C->RD;
//                if(C->objekt->pohon!=NULL)
//                {
//								nazev_pohonu=C->objekt->pohon->name;
//								roztec_palcu=C->objekt->pohon->roztec*1000.0;
//								rozsah_pohonu_od=O->pohon->rychlost_od*60.0;
//								rozsah_pohonu_do=O->pohon->rychlost_do*60.0;
//								}
//								else {nazev_pohonu="nepøiøazen";roztec_palcu="";rychlost_dopravniku="žádná";}
//
//								//UnicodeString rychlost_od=C->objekt->pohon->rychlost_od;
//								//UnicodeString rychlost_do=C->objekt->pohon->rychlost_do;
//								UnicodeString delka_dopravniku=C->objekt->delka_dopravniku;
//									switch(C->objekt->rezim)
//								{
//									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantní";delka_dopravniku="nerelevantní"; break;
//									case 1:rezim="KONTINUÁLNÍ";break;
//									case 2:rezim="POSTPROCESNÍ";rychlost_dopravniku="nerelevantní";break;
//								}
////          //html
//								data+=""+ID+S+name+S+short_name+S+rezim+S+CT+S+kapacita+S+kapacita_dop+S+nazev_pohonu+S+rychlost_dopravniku+S+rozsah_pohonu_od+"-"+rozsah_pohonu_do+S+roztec_palcu+S+delka_dopravniku+S+mezera+"\n";
//								}
//					O=O->dalsi;
//							}
//		Z=Z->dalsi;
//		data+="\n";
//					}
//				}
//		data+="\n";
//
//
//		}



		///////////////////////////zapis data do souboru
		if(export_format==3)data_export=data_HTML;else data_export=data_CSV;
		TMemoryStream* MemoryStream=new TMemoryStream();
		MemoryStream->Clear();
		MemoryStream->Write(data_export.c_str(),data_export.Length());//Win kodování
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
