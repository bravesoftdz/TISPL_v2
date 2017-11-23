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

	WebBrowser1->Navigate(Form1->FileName);

		Form_report->Left=Form1->ClientWidth/2-Form_report->Width/2;
		Form_report->Top=Form1->ClientHeight/2-Form_report->Height/2;

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
		//ZDM get_LT_a_max_min_TT();//zajist� hodnoty

		//generuj_POHONY(); u� nepou��v�me, ale mo�n� �asem budeme

		AnsiString data="";//celkov� textov� data k exportu

		//zji�t�n� exportovan�ho form�tu
		unsigned short export_format=3;
		if(FileName.SubString(FileName.Length()-2,3).LowerCase() =="xls")export_format=2;
		if(FileName.SubString(FileName.Length()-3,4).LowerCase() =="html")export_format=3;

		//form�tov�n� textu do jednotliv�ch form�t�
		UnicodeString S=";";//SEPARATOR pro CSV
		switch(export_format)//1-csv, 2 - xls,3 - html
		{
			//ZDM case 1:data=get_csv_xls(";");break;//CSV
			//ZDM case 2:data=get_csv_xls("\t");break;//XLS
			//ZDM case 3:data=get_html();break;//html
		}

		//z�pis dat do DB
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



		if(export_format==3)  {

			data+="<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css\" integrity=\"sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb\" crossorigin=\"anonymous\">"; // href=\"\../linky_layouts/styly/css/bootstrap.min.css\">";
			//data+="<script src=\"\../linky_layouts/styly/js/bootstrap.min.js\"></script>";
			data+="<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		 //	data+="<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js\" integrity=\"sha384-alpBpkh1PFOepccYVYDB4do5UnbKysX5WZXm3XxPqe5iKTfUKjNkCk9SaVuEZflJ\" crossorigin=\"anonymous\"></script>";
			data+="<div class=\"container-fluid\">";

		if(Form1->d.v.ZAKAZKY->dalsi!=NULL){   //kdy� existuje zak�zka - vypi� tuto hlavi�ku //UnicodeString(Form1->scLabel_titulek->Caption)
		data+="<form></br>";
		data+="<h4>Parametry zak�zky <b></b></h4></br>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Po�adovan� celkov� mno�stv�</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+PP_mnozstvi+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Po�et pracovn�ch dn� [rok]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+dni_rok+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Po�et pracovn�ch hodin [den]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+hod_den+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Efektivita [%]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+efektivita+"\"></div></div>";
		data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">Za��tek v�roby</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+cas_start+"\"></div></div>";
    data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">D�lka voz�ku</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_voziku+"\"></div></div>";
		data+="</form></div></br>";

		data+="<div class=\"container-fluid\"><form>";  }

		else {   //pokud neexistuje zak�zka vypi� pouze info z N�VRHU

			data+="<form></br>";
			data+="<h4>Parametry linky <b>"+UnicodeString(Form1->scLabel_titulek->Caption)+"</b></h4></br>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">TaktTime [min]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+PP_TT+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">D�lka Jigu [m]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+delka_voziku+"\"></div></div>";
			data+="<div class=\"form-group row\"><label for=\"colFormLabel\" class=\"col-sm-2 col-form-label col-form-label\">���ka Jigu [m]</label><div class=\"col-sm-2\"><input type=\"text\" class=\"form-control form-control\" id=\"colFormLabel\" placeholder=\""+sirka_voziku+"\"></div></div>";

			data+="</form></div></br>";
			data+="<div class=\"container-fluid\"><form>";

		}

			data+="<h4>P�ehled doporu�en�ch rychlost� pohon�</h4></br>";

		  data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">N�zev</th><th scope=\"col\">Rychlost</th></tr></thead>";
			Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

				while(ukaz!=NULL){

				UnicodeString ID=ukaz->n;
				UnicodeString name=ukaz->name;
				UnicodeString rychlost_od=ukaz->rychlost_od;
				UnicodeString rychlost_do=ukaz->rychlost_do;
				UnicodeString roztec=ukaz->roztec;

		data+="<tr><th scope=\"row\">"+name+"</th><td>"+rychlost_od+"</td></tr>";


					ukaz=ukaz->dalsi;

				}
          	data+="</tbody></table></br>";

		if(Form1->STATUS==Form1->NAVRH) {


		data+="<h4>N�VRH: P�ehled objekt� a jejich parametr�</h4></br>";
		data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">N�zev</th><th scope=\"col\">Zkratka</th><th scope=\"col\">Re�im</th><th scope=\"col\">CT [min]</th><th scope=\"col\">Kapacita doporu�en�</th><th scope=\"col\">Kapacita nastaven�</th><th scope=\"col\">N�zev dopravn�ku</th><th scope=\"col\">Rychlost dopravn�ku</th><th scope=\"col\">Rozte� palc� [cm]</th><th scope=\"col\">D�lka dopravn�ku [m]</th></tr></thead>";

		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;

		while (O!=NULL)//proch�z� potenci�ln� segmenty cesty po objektech
		{
								UnicodeString ID=O->id;
								UnicodeString name=O->name;
								UnicodeString short_name=O->short_name;
								UnicodeString rezim;
								UnicodeString CT=O->CT;
								UnicodeString kapacita=O->kapacita;
								UnicodeString kapacita_dop=O->kapacita_dop;
								UnicodeString nazev_pohonu=O->pohon->name;
								UnicodeString roztec_palcu=O->pohon->roztec;
								UnicodeString rychlost_dopravniku=O->RD;
								//UnicodeString rychlost_od=C->objekt->pohon->rychlost_od;
								//UnicodeString rychlost_do=C->objekt->pohon->rychlost_do;
								UnicodeString delka_dopravniku=O->delka_dopravniku;
									switch(O->rezim)
								{
									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantn�";delka_dopravniku="nerelevantn�"; break;
									case 1:rezim="KONTINU�LN�";break;
									case 2:rezim="POSTPROCESN�";rychlost_dopravniku="nerelevantn�";break;
								}
//          //html
								data+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+short_name+"</td><td>"+rezim+"</td><td>"+CT+"</td><td>"+kapacita_dop+"</td><td>"+kapacita+"</td><td>"+nazev_pohonu+"</td><td>"+rychlost_dopravniku+"</td><td>"+roztec_palcu+"</td><td>"+delka_dopravniku+"</td></tr>";

									O=O->dalsi;
								}

		data+="</tbody></table></br>";
		data+="</form></div></br>";



	}




	if(Form1->d.v.ZAKAZKY->predchozi!=NULL  && Form1->d.v.OBJEKTY->predchozi!=NULL)//pokud existuje alespo� jedna zak�zka a n�jak� objekt
{
	Cvektory::TZakazka *Z=Form1->d.v.ZAKAZKY->dalsi;//p�esko�� hlavi�ku
	while(Z!=NULL)//proch�z� jednotliv� zak�zky
	{
		UnicodeString zakazka_name=Z->name;
		data+="<h4>P�ehled objekt� a jejich nastaven�ch parametr� u zak�zky: <b>"+zakazka_name+"</b></h4></br>";
		data+="<table class=\"table table-striped table-responsive\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">N�zev</th><th scope=\"col\">Zkratka</th><th scope=\"col\">Re�im</th><th scope=\"col\">CT [s]</th><th scope=\"col\">Kapacita doporu�en�</th><th scope=\"col\">Kapacita nastaven�</th><th scope=\"col\">N�zev dopravn�ku</th><th scope=\"col\">Rychlost dopravn�ku</th><th scope=\"col\">Rozte� palc� [mm]</th><th scope=\"col\">D�lka dopravn�ku [m]</th></tr></thead>";

		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;

		while (O!=NULL)//proch�z� potenci�ln� segmenty cesty po objektech
		{
							//pokud je objekt platn�m segmentem cesty zak�zky, vr�t� ukazatel na tento segment
							Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,Z);
						if(C!=NULL){
								UnicodeString ID=C->objekt->n;
								UnicodeString name=C->objekt->name;
								UnicodeString short_name=C->objekt->short_name;
								UnicodeString rezim;
								UnicodeString CT=C->CT;
								UnicodeString kapacita=C->objekt->kapacita;
								UnicodeString kapacita_dop=C->objekt->kapacita_dop;
								UnicodeString nazev_pohonu=C->objekt->pohon->name;
								UnicodeString roztec_palcu=C->objekt->pohon->roztec;
								UnicodeString rychlost_dopravniku=C->RD;
								//UnicodeString rychlost_od=C->objekt->pohon->rychlost_od;
								//UnicodeString rychlost_do=C->objekt->pohon->rychlost_do;
								UnicodeString delka_dopravniku=C->objekt->delka_dopravniku;
									switch(C->objekt->rezim)
								{
									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantn�";delka_dopravniku="nerelevantn�"; break;
									case 1:rezim="KONTINU�LN�";break;
									case 2:rezim="POSTPROCESN�";rychlost_dopravniku="nerelevantn�";break;
								}
//          //html
								data+="<tr><th scope=\"row\">"+ID+"</th><td>"+name+"</td><td>"+short_name+"</td><td>"+rezim+"</td><td>"+CT+"</td><td>"+kapacita_dop+"</td><td>"+kapacita+"</td><td>"+nazev_pohonu+"</td><td>"+rychlost_dopravniku+"</td><td>"+roztec_palcu+"</td><td>"+delka_dopravniku+"</td></tr>";
								}
					O=O->dalsi;
		}
		Z=Z->dalsi;

		data+="</tbody></table></br>";
	}
}
		data+="</form></div>";



	}

	////////////////////////////////////////////////////////////////////////////// CSV

		if(export_format==1)  {


		data+="Parametry projektu"+S+UnicodeString(Form1->scLabel_titulek->Caption)+"\n";
		data+="Po�adovan� celkov� mno�stv�"+S+PP_mnozstvi+"\n";
		data+="Po�et pracovn�ch dn� [rok]"+S+dni_rok+"\n";
		data+="Po�et pracovn�ch hodin [den]"+S+hod_den+"\n";
		data+="Efektivita [%]"+S+efektivita+"\n";
		data+="Za��tek v�roby"+S+cas_start+"\n";
		data+="D�lka voz�ku"+S+delka_voziku+"\n";

		data+="\n";

					data+="P�ehled pohon�"+S+"\n";

				Cvektory::TPohon *ukaz=Form1->d.v.POHONY->dalsi;

				data+="ID"+S+"N�zev"+S+"Rychlost od [m/min]"+S+"Rychlost do [m/min]"+S+"Rozte� palc� [cm]"+S+"\n";
				while(ukaz!=NULL){

				UnicodeString ID=ukaz->n;
				UnicodeString name=ukaz->name;
				UnicodeString rychlost_od=ukaz->rychlost_od;
				UnicodeString rychlost_do=ukaz->rychlost_do;
				UnicodeString roztec=ukaz->roztec;

					data+=""+ID+S+name+S+rychlost_od+S+rychlost_do+S+roztec+S+"\n";

					ukaz=ukaz->dalsi;

				}

      	data+="\n";

			if(Form1->d.v.ZAKAZKY->predchozi!=NULL  && Form1->d.v.OBJEKTY->predchozi!=NULL)//pokud existuje alespo� jedna zak�zka a n�jak� objekt
{
	Cvektory::TZakazka *Z=Form1->d.v.ZAKAZKY->dalsi;//p�esko�� hlavi�ku
	while(Z!=NULL)//proch�z� jednotliv� zak�zky
	{
		UnicodeString zakazka_name=Z->name;
		data+="P�ehled objekt� a jejich nastaven�ch parametr� u zak�zky:"+S+zakazka_name+"\n";
		data+="ID"+S+"N�zev"+S+"Zkratka"+S+"Re�im"+S+"CT [s]"+S+"Kapacita doporu�en�"+S+"Kapacita nastaven�"+S+"N�zev dopravn�ku"+S+"Rychlost dopravn�ku"+S+"Rozte� palc� [cm]"+S+"D�lka dopravn�ku [m]\n";

		Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;

		while (O!=NULL)//proch�z� potenci�ln� segmenty cesty po objektech
		{
							//pokud je objekt platn�m segmentem cesty zak�zky, vr�t� ukazatel na tento segment
							Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,Z);
						if(C!=NULL){
								UnicodeString ID=C->objekt->n;
								UnicodeString name=C->objekt->name;
								UnicodeString short_name=C->objekt->short_name;
								UnicodeString rezim;
								UnicodeString CT=C->CT;
								UnicodeString kapacita=C->objekt->kapacita;
								UnicodeString kapacita_dop=C->objekt->kapacita_dop;
								UnicodeString nazev_pohonu=C->objekt->pohon->name;
								UnicodeString roztec_palcu=C->objekt->pohon->roztec;
								UnicodeString rychlost_dopravniku=C->RD;
								//UnicodeString rychlost_od=C->objekt->pohon->rychlost_od;
								//UnicodeString rychlost_do=C->objekt->pohon->rychlost_do;
								UnicodeString delka_dopravniku=C->objekt->delka_dopravniku;
									switch(C->objekt->rezim)
								{
									case 0:rezim="STOP & GO";rychlost_dopravniku="nerelevantn�";delka_dopravniku="nerelevantn�"; break;
									case 1:rezim="KONTINU�LN�";break;
									case 2:rezim="POSTPROCESN�";rychlost_dopravniku="nerelevantn�";break;
								}
//          //html
								data+=""+ID+S+name+S+short_name+S+rezim+S+CT+S+kapacita+S+kapacita_dop+S+nazev_pohonu+S+rychlost_dopravniku+S+roztec_palcu+S+delka_dopravniku+"\n";
								}
					O=O->dalsi;
		}
		Z=Z->dalsi;

		data+="\n";
	}
}
		data+="\n";


	}




	//example tabulka
	 /*	data+="<table class=\"table table-striped\"><thead><tr><th scope=\"col\">ID</th><th scope=\"col\">Objekt</th><th scope=\"col\">Kapacita</th><th scope=\"col\">CT</th></tr></thead>";
		data+="<tbody><tr><th scope=\"row\">1</th><td>LAK</td><td>2</td><td>1</td></tr>";
		data+="<tr><th scope=\"row\">2</th><td>CO2</td><td>5</td><td>1</td></tr>";
		data+="<tr><th scope=\"row\">3</th><td>SU�</td><td>2</td><td>2</td></tr>";
		data+="</tbody></table>";       */



		///////////////////////////zapis data do souboru
		TMemoryStream* MemoryStream=new TMemoryStream();
		MemoryStream->Clear();
		MemoryStream->Write(data.c_str(),data.Length());//Win kodov�n�
		MemoryStream->SaveToFile(FileName);



			//Form1->scExPanel_html->Visible=true;
			//Form1->scExPanel_html->Top=Form1->scGPPanel_mainmenu->Height*2;
			//Form1->scExPanel_html->Left=Form1->scListGroupNastavProjektu->Width*2-50;
		 //	Form1->scExPanel_html->Width=Form1->ClientWidth-400;
		 //	Form1->scExPanel_html->Height=Form1->ClientHeight-100;
		 //	Form1->WebBrowser1->Navigate(FileName);


				WebBrowser1->Navigate(FileName);
				ShowModal();

		delete MemoryStream;
		return 1;

		}
		catch(...){;Form1->MB("Soubor je otev�en, ukon�ete otev�enou aplikaci a akci opakujte");}
}


void __fastcall TForm_report::scButton_csvClick(TObject *Sender)
{
ShowMessage("ahoj");
}
//---------------------------------------------------------------------------

void __fastcall TForm_report::MaxButtonClick(TObject *Sender)
{
	if (FMaximized)//zmen�en�
	{
			 //BoundsRect =  FOldBoundsRect;
			 FMaximized = false;
			// scLabel_titulek->DragForm = true;
			 MaxButton->GlyphOptions->Kind = scgpbgkMaximize;
			// scGPSizeBox->Visible = true;
			 Form_report->Width=Screen->Width/3*2;//zmen�� formul�� na 2/3 jeho velikosti
			 Form_report->Height=Screen->Height/3*2;//zmen�� formul�� na 2/3 jeho velikosti
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

