//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include <MATH.h>
#include <inifiles.hpp>
/*#include <registry.hpp>
#include <stdio.h>*/
#include <jpeg.hpp>
#include <pngimage.hpp>
#include "MyMessageBox.h"
#include "parametry.h"
//#include "TT_kalkulator.h"
#include "parametry_linky.h"
#include "superform.h"
#include "uvod.h"
#include "antialiasing.h"
#include "popUP_menu.h"
#include "eDesigner.h"
#include "casovaOsa_info.h"
#include "report.h"
#include "PO_math.h"
#include "kabina_schema.h"
#include "Unit2.h"
#include "gapoR.h"
#include "adjustace.h"
#include "kalibrace.h"
#include "Z_rozliseni.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzPanel"
#pragma link "RzSplit"
#pragma link "RzStatus"
#pragma link "RzButton"
#pragma link "RzBorder"
#pragma link "RzBHints"
#pragma link "RzGrids"
#pragma link "RzLabel"
#pragma link "rStringGridEd"
#pragma link "rStringGridEd"
#pragma link "scControls"
#pragma link "scGPControls"
#pragma link "scStyledForm"
#pragma link "scAdvancedControls"
#pragma link "scExtControls"
#pragma link "scModernControls"
#pragma link "scDrawUtils"
#pragma link "scGPImages"
#pragma link "scHtmlControls"
#pragma link "scWebBrowser"
#pragma link "scHint"
#pragma link "scGPExtControls"
#pragma link "rHintWindow"
#pragma resource "*.dfm"
TForm1 *Form1;
TForm1 *F;//pouze zkrácený zapis
AnsiString Parametry;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	srand(time(NULL));//nutno tady tj. úplně první!!!

	F=Form1;//pouze zkrácení zápisu

	db_connection();//připojení k DB

	Form1->Width=1290;//workaround bílé mezery v záložkové liště - nefunguje

	m2px=0.1;//uchovává hodnotu prostorového rozlišení programu, nativní rozlišení 0,1 m na 1 pixel při zoomu 1x
	fps=24*3;//frames per second, četnost snímků za sekundu - používá se pro animace a simulace, misto 24Hz 60Hz, hráči her doporučují min fps 60 + min obnovovací frekvence monitoru 60Hz
	//https://cdr.cz/blog/30-fps-vs-60-fps-shrnuti-velke-internetove-debaty

	//vytvoření TEMP adresáře (pro ini)
	MkDir(get_temp_dir()+"TISPL");

	////nastavení aplikace
	upozornovat_na_zmenu_TT_parametru=true;
	Application->HintHidePause=20000; //nastavení délky trvání zobrazení Hintů -  20s

	//mřížka
	grid=true; size_grid=10;//velikost je v logických jednotkách (metrech)
	prichytavat_k_mrizce=0;
	SB("přichytávat?",5);
	ortogonalizace_stav=false;

	//bitmapa pro uložení přesovaného obrazu - PAN
	Pan_bmp=new Graphics::TBitmap();
	pan_non_locked=false;

	//načtení nestandardních kurzorů aplikace
	HCURSOR HC;
	HC=LoadCursor(HInstance,L"POSUN_V");
	Screen->Cursors[1]=HC;
	HC=LoadCursor(HInstance,L"POSUN_B");
	Screen->Cursors[2]=HC;
	HC=LoadCursor(HInstance,L"POSUN_P");
	Screen->Cursors[3]=HC;
	HC=LoadCursor(HInstance,L"POSUN_L");
	Screen->Cursors[4]=HC;
	HC=LoadCursor(HInstance,L"POSUN_T");
	Screen->Cursors[5]=HC;
	HC=LoadCursor(HInstance,L"KALIBRACE");
	Screen->Cursors[6]=HC;
	HC=LoadCursor(HInstance,L"PAN");
	Screen->Cursors[7]=HC;
	HC=LoadCursor(HInstance,L"PAN_MOVE");
	Screen->Cursors[8]=HC;
	HC=LoadCursor(HInstance,L"WINDOW");
	Screen->Cursors[9]=HC;
	HC=LoadCursor(HInstance,L"ADD_O");
	Screen->Cursors[10]=HC;
	HC=LoadCursor(HInstance,L"MOVE_POINT");
	Screen->Cursors[11]=HC;
	HC=LoadCursor(HInstance,L"POSUN_IND");
	Screen->Cursors[12]=HC;
	HC=LoadCursor(HInstance,L"ZMENA_J");
	Screen->Cursors[13]=HC;
	
	  //Načtení z INI
	AnsiString T=F->readINI("nastaveni_nahled", "cas");
	if(T=="")PTunit=0;else PTunit=T.ToInt();
	T=F->readINI("nastaveni_nahled","LO");
	if(T=="")LOunit=0;else LOunit=T.ToInt();
	T=F->readINI("nastaveni_nahled","Delka_otoce");
	if(T=="")DOtocunit=0;else DOtocunit=T.ToInt();

	//povolení Automatická záloha
	Timer_backup->Enabled=true;

	pocitadlo_doby_neaktivity=0;

	antialiasing=true;

	//nastavení implicitního souboru
	duvod_k_ulozeni=false;
	Novy_soubor();
	volat_parametry_linky=false;

	//zjistí verzi aplikace
	DWORD ver=GetFileVersion(Application->ExeName);
	VERZE=UnicodeString(HIWORD(ver))+"."+UnicodeString(LOWORD(ver));
	//VERZE=UnicodeString(UnicodeString(HIWORD(ver) >> 16)+"."+UnicodeString(HIWORD(ver) & 0xFFFF)+"."+UnicodeString(LOWORD(ver) >> 16)+"."+UnicodeString(LOWORD(ver) & 0xFFFF));

	//licence atp.
	Caption="ELTEP - tispl";
	scLabel_titulek->Caption=Caption+" - [Nový.tispl]";
	Application->Title="TISPL";
	LICENCE="TRIAL_GALATEK";
	EDICE=ARCHITECT;//ARCHITECT,CLIENT,VIEWER,DEMO
	edice();//zakázání či povolení grafických uživatelských prvků dle úrovně edice
	n_prihlaseni=0;
	TZF=!DEBUG;//TRIAL_zakazat_funkcionality - nyní nastaveno pro RELEASE
	if(TZF)scGPSwitch_rezim->Enabled=false;

  //pomocné objekty pro kopírování paremtrů v PO
	copyObjekt=new Cvektory::TObjekt;
	copyObjektRzRx.x=0;copyObjektRzRx.y=0;

	//načtení řetězů - jsou-li k dispozici
	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	d.v.nacti_CSV_retezy("řetězy.csv");


	//nastavení knihovnky
	//DrawGrid_knihovna->Enabled=false;
	DrawGrid_knihovna->RowCount=pocet_objektu_knihovny/2;//velikosti buněk
	if(pocet_objektu_knihovny>0)vybrany_objekt=0;else//-použival jsem v době kdy jsem chtěl mít implicitní prvek pokud existuje nějaký prvek v knihovně, tak nastaví vybraný prvek jako první
	vybrany_objekt=-1;
	VyID=10;//objekt-symbol vyhýbky - ID typu
	knihovna_id=0;
	element_id=99;
	refresh_mGrid=true;

	DesignSettings();//nastavení designu v konstruktoru
}
//---------------------------------------------------------------------------
//záležitost s novým designem
void TForm1::DesignSettings()
{
	//maximalizace formuláře jinak to s novým designem nejde
	Form1->Width=Screen->WorkAreaWidth;
	Form1->Height=Screen->WorkAreaHeight;
	FMaximized=false;MaxButtonClick(this);//aby bylo připraveno minimalizační tlačítko

	//nastavení globálních barev
	TColor light_gray=(TColor)RGB(240,240,240);
	TColor active_blue=(TColor)RGB(0,120,215);
  TColor clDrawGridHeaderFont=m.clIntensive(RGB(43,87,154),30);

	PopupMenuButton->Left = 0;
	PopupMenuButton->Visible = false;
	DetailsButton->Left = 0;
	DetailsButton->Visible = true;
	scSplitView_OPTIONS->Opened=false;
	scSplitView_OPTIONS->Align=alRight;

  scGPLabel_otoce->Font->Color=clDrawGridHeaderFont;
	scGPLabel_roboti->Font->Color= scGPLabel_otoce->Font->Color;
  scGPLabel_stop->Font->Color= scGPLabel_otoce->Font->Color;
  scGPLabel_geometrie->Font->Color = scGPLabel_otoce->Font->Color;
  scGPLabel_poznamky->Font->Color =  scGPLabel_otoce->Font->Color;

 //	scGPGlyphButton_definice_zakazek->Options->NormalColor=active_blue;
 //	scGPGlyphButton_parametry_linky->Options->NormalColor=active_blue;
 //	scListGroupNastavProjektu->HeaderAutoColor=true;
	scListGroupKnihovObjektu->HeaderAutoColor=true;
	scListGroupKnihovObjektu->Color=light_gray;

  scListGroupPanel_hlavickaOtoce->HeaderAutoColor=scListGroupKnihovObjektu->HeaderAutoColor;
  scListGroupPanel_hlavickaOtoce->Color=scListGroupKnihovObjektu->Color;
  scListGroupPanel_hlavickaOstatni->Color=light_gray;

  scListGroupPanel_hlavickaOstatni->HeaderAutoColor=scListGroupKnihovObjektu->HeaderAutoColor;
  scListGroupPanel_hlavickaOstatni->Color=scListGroupKnihovObjektu->Color;
	//scSplitView_OPTIONS->Color=light_gray;
 //	scExPanel_vrstvy->Color=light_gray;
 //	scExPanel_ostatni->Color=light_gray;

	//nastaveni barvy prepinacu modu
	Schema->Options->PressedColor=light_gray;
	Layout->Options->PressedColor=light_gray;
	Analyza->Options->PressedColor=light_gray;
	Synteza->Options->PressedColor=light_gray;
	Simulace->Options->PressedColor=light_gray;

	scExPanel_ostatni->Top=72+27;

	if(MOD==SCHEMA) //zobrazeni labelu - je hezci, v hlavicce drawgrid knihovny
	{
		scGPLabel_roboti->Visible=true;
		scGPLabel_roboti->Caption="Technolog. objekty";
		scGPLabel_roboti->ContentMarginLeft=4;
		scListGroupKnihovObjektu->Height=1920; // kvůli odstranění bílé linky, která vznikala pod knihovnou objektů
	}

	////design spodní lišty////
	scGPPanel_bottomtoolbar->Top=scGPPanel_statusbar->Top-scGPPanel_bottomtoolbar->Height;
	scGPPanel_bottomtoolbar->Width=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
	scGPPanel_bottomtoolbar->Left=scSplitView_LEFTTOOLBAR->Width;
	//vodorovné zarovnání prvků
	scGPButton_zahodit->Left=scGPPanel_bottomtoolbar->Width/2-scGPLabel_otoce->Width;
	scGPButton_ulozit->Left=scGPButton_zahodit->Left-scGPButton_zahodit->Width-22;
	scGPLabel1->Left=scGPButton_zahodit->Left+scGPButton_zahodit->Width+22;
	scGPComboBox_orientace->Left=scGPLabel1->Left+scGPLabel1->Width;
	scGPCheckBox_viditelnost->Left=scGPComboBox_orientace->Left+scGPComboBox_orientace->Width+22;
	scGPLabel2->Left=scGPCheckBox_viditelnost->Left+scGPCheckBox_viditelnost->Width+22;
	scButton_zamek->Left=scGPLabel2->Left+scGPLabel2->Width;
	//svislé zarovnání prvků
	scGPButton_ulozit->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_ulozit->Height)/2;
	scGPButton_zahodit->Top=scGPButton_ulozit->Top;
	scGPCheckBox_viditelnost->Top=scGPButton_ulozit->Top;
	scGPComboBox_orientace->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_orientace->Height)/2;
	scGPLabel1->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel1->Height)/2;
	scGPLabel2->Top=scGPLabel1->Top;
	scButton_zamek->Top=(scGPPanel_bottomtoolbar->Height-scButton_zamek->Height)/2;

	//pozice ovládacích prvků
//	scListGroupNastavProjektu->Left=0;
//	scListGroupNastavProjektu->Top=0;
//	scListGroupKnihovObjektu->Left=0;
 /*	scListGroupKnihovObjektu->Top=2+scListGroupNastavProjektu->Height+0;
	scListGroupKnihovObjektu->Height=scGPPanel_statusbar->Top-(2+scListGroupNastavProjektu->Height+0+DetailsButton->Height);
	vyska_menu=0;  */
}
//---------------------------------------------------------------------------
//zakázání či povolení grafických uživatelských prvků dle úrovně edice
void TForm1::edice()
{
	//switch na jednotlivé edice v kterém bude následné povolení či zakázání patřičných ovládacíh prvků
	switch (EDICE)
	{
			//case DEVELOPER: Edice_caption="DEVELOPER";*/break;
			case ARCHITECT: /*Edice_caption="ARCHITECT";*/break;
			case CLIENT:		/*Edice_caption="CLIENT";*/break;
			case VIEWER: 		/*Edice_caption="VIEWER";*/break;
			case DEMO:  //demo
				/*Edice_caption="VIEWER - DEMO";*/
				Toolbar_NovySoubor->Enabled=false;
				Toolbar_Otevrit->Enabled=false;
			//	Otevritsablonu->Enabled=false;
				Toolbar_Ulozit->Enabled=false;
				scButton_ulozit->Enabled=false;
			//	Ulozitjako->Enabled=false;
			//	Export1->Enabled=false;
			//	Report1->Enabled=false;
			//	Obnovitzezlohy1->Enabled=false;
			//	SPPP1->Enabled=false;
		 //		Boskovice1->Enabled=false;
			 //	eXtreme1->Enabled=false;
		 //		casoverezervy1->Enabled=false;
		 //		testovnkapacity1->Enabled=false;
			//	simulace1->Enabled=false;
			//	Vzhled1->Enabled=false;
			 //	Button_kalkulatorTT->Enabled=false;
			//M	Button_vozik_parametry->Enabled=false;
				DrawGrid_knihovna->Enabled=false;
			//	Nastvitparametry1->Enabled=false;
				//Smazat1->Enabled=false;

				RzToolButton1->Enabled=false;
				RzToolButton2->Enabled=false;
				RzToolButton3->Enabled=false;

				//Edit_takt_time->Enabled=false;
		 //M			Button_dopravnik_parametry->Enabled=false;
				break;
			default:
				break;
	}
}
//---------------------------------------------------------------------------
//kontrola aktuálnosti verze a případě nabídka na stažení nové
void TForm1::aktualizace()
{
		//zjištění aktuální verze
		UnicodeString AKT_VERZE=IdHTTP1->Get("http://81.2.243.72/TISPL/verze.txt");
		//porovnání akt. verze a používané verze aplikace
		if(AKT_VERZE!=VERZE)//je k dispozici aktualizace
		{
			if(mrYes==MB("Je k dispozici aktualizace TISPLu. Chcete ji stáhnout?",MB_YESNO,false))
			{
				MB("Po dokončení staženým souborem přepište současný EXE soubor.");
				zobraz_tip("Probíhá stahování aktualizace, neukončujte aplikaci!!! Po dokončení stahování bude program ukončen.");
				SaveDialog->Title="Uložit soubor jako";
				SaveDialog->DefaultExt="*.exe";
				SaveDialog->Filter="Aktualizace TISPLu (*.exe)|*.exe";
				SaveDialog->FileName="TISPL_aktualizace";
				if(SaveDialog->Execute())
				{
						TMemoryStream *MemoryStream=new TMemoryStream();
						IdHTTP1->Get("http://81.2.243.72/TISPL/LIC/"+LICENCE+"/tispl.exe",MemoryStream);
						MemoryStream->SaveToFile(SaveDialog->FileName);
						log2web("aktualizace_z_"+AKT_VERZE+"_na_"+VERZE);
						ShellExecute(0,L"open",SaveDialog->FileName.c_str(),0,0,SW_SHOWNORMAL);
						MessageBeep(0);
						Close();
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
    //DPI handling
	int DPI, otherDPI = 0;
	DPI = get_DPI();
	if (DPI != 96)
	{
		otherDPI = DPI * 100 / 96;
		Form_Z_rozliseni->ShowModal();
	}
	// startUP() - pokud byl zde, dělalo to "chybu v paměti" při spuštění release verze	startUP();//při aktivaci formuláře startující záležitosti, pro zpřehlednění ko
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//volání založení nový soubor z menu = nové nastavení souboru, nastevení aplikace je v konstruktoru
void __fastcall TForm1::NovySouborClick(TObject *Sender)
{
	 Novy_soubor();//samotné vytvoření nového souboru
	 //následující slouží pouze při uživatelsky volaném soubor nový
	 Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
	 Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
	 Form_parametry_linky->ShowModal();//zavolání formáláře pro prvotní vyplnění či potvzení hodnot parametrů linky
	 REFRESH();
}
//---------------------------------------------------------------------------
void TForm1::Novy_soubor()//samotné vytvoření nového souboru
{
	 scSplitView_MENU->Opened=false;
	 scButton_novy->Down=false;
	 bool novy=true;
	 if(duvod_k_ulozeni)
	 {
			AnsiString FNs=FileName_short(FileName);
			if(FNs=="")FNs="Nový.tispl";
			int result=MB(FNs+" byl změněn.\nChcete ho před ukončením uložit?",MB_YESNOCANCEL);
			switch(result)
			{
				case mrYes:     UlozitClick(this); if(!stisknuto_storno)novy=true;else novy=false; break;
				case mrNo:      novy=true; break;
				case mrCancel:  novy=false; break;
			}
	 }

	 if(novy)
	 {
			 //odstranění vektorů
			 vse_odstranit();
			 //nové vytvoření hlaviček
			 d.v.hlavicka_OBJEKTY();//založení spojového seznamu pro technologické objekty
			 d.v.hlavicka_POHONY();//založení spojového seznamu pro pohony
			 d.v.hlavicka_ZAKAZKY();//založení spojového seznamu pro zakázky
			 d.v.hlavicka_VOZIKY();// nemusí tu být pokud nebudu ukládat vozíky do filuzaložení spojového seznamu pro vozíky
			 //ZDM d.v.hlavicka_palce();
			 //vytvoření defaltních hodnot
			 //d.v.vloz_pohon("Hlavní dopravník",1.5,10.5,32.5);
			 //d.v.vloz_pohon("Vedlejší dopravník",1.5,10.5,32.5); není nutné prý vedlejší dopravník

			 //tady bude přepnutí založek dodělat
			 schemaClick(this);//volání MODu SCHEMA
			 scGPSwitch_rezim->State=scswOff;
			 SB("NÁVRH",1);

			 Zoom=1.0; on_change_zoom_change_scGPTrackBar();
			 Zoom_predchozi=1.0;
			 Posun.x=-scListGroupKnihovObjektu->Width;if(vyska_menu>0)Posun.y=-vyska_menu+9;else Posun.y=-29;
			 Posun_predchozi.x=Posun.x;Posun_predchozi.y=Posun.y;
			 jedno_ze_tri_otoceni_koleckem_mysi=1;
			 doba_neotaceni_mysi=0;

			 d.v.PP.cas_start=TDateTime(AnsiString(TIME.CurrentDate().DateString())+" "+"8:00:00");//defaultně dnes v 8:00
			 d.v.PP.mnozstvi=20000;
			 d.v.PP.hod_den=8;
			 d.v.PP.dni_rok=365;
			 d.v.PP.TT=120.0;
			 d.v.PP.efektivita=95;
			 d.v.PP.delka_jig=1;
			 d.v.PP.sirka_jig=1;
			 d.v.PP.vyska_jig=1;
			 d.v.PP.delka_podvozek=1;
			 d.v.PP.typ_voziku=0;
			 d.v.PP.raster.filename="";
			 d.v.PP.raster.resolution=m2px;
			 d.v.PP.raster.X=0;
			 d.v.PP.raster.Y=0;
			 d.v.PP.raster.show=false;


			 Akce=NIC;Screen->Cursor=crDefault;//změní kurzor na default

			 DuvodUlozit(false);
			 RzToolButton4->Enabled=false;
			 RzToolButton5->Enabled=false;
			 uchop_zobrazen=false;
			 vycentrovat=true;
			 posun_objektu=false;//nutnost, aby se během realizace posunu neaktivoval další posun
			 zneplatnit_minulesouradnice();
			 dblClick=false;
			 probehl_zoom=false;
			 add_posledni=true;
			 stisknute_leve_tlacitko_mysi=false;
			 funkcni_klavesa=0;
			 pan_non_locked=false;
			 zobrazit_barvy_casovych_rezerv=false;
			 d.cas=0;
			 Analyza->Enabled=false;
			 Analyza->Down=false;
			 Schema->Down=true;
       SB("Kliknutím na libovolné místo přidáte objekt z knihovny");
			 FileName="Nový.tispl";
			 scLabel_titulek->Caption=Caption+" - ["+FileName+"]";
			 Invalidate();//vhodnější invalidate než refresh
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
	if(PopUPmenu->Showing || PopUPmenu->closing)PopUPmenu->Close();//pokud je spuštěné pop-up menu, tak ho vypne
	else
	{
		if(!DEBUG)
		{
			//toto odkomentovat pro spuštění TTR
//			if(!ttr("start"))
//			{
//				Timer_tr->Enabled=false;//ještě je ale z důvodu ochrany enabled=true v object inspectoru, toto je spíše na zmatení
//				Close();
//			}
//			else
			Timer_tr->Enabled=false;// toto zakomentovat pro spuštění TTR
			startUP();//toto vždy odkomentované
		}
		else
		{
			Timer_tr->Enabled=false;
			startUP();
	}
	}

 }
//---------------------------------------------------------------------------
//Metoda pro trial verzi
bool TForm1::ttr(UnicodeString Text)
{
	//licence
	Timer_tr->Enabled=true;
	UnicodeString LIC_FILE	= LICENCE;
	UnicodeString Text_error="Není k dispozici přípojení k internetu nebo vypršela licence, aplikace nebude spuštěna!";
	TIME=TDateTime("1.1.1990 0:00:00");
	TDateTime TIME_expirace;
	UnicodeString Response="error";
	bool STATUS=false;

	try
	{
		//zjištění expirace trialverze
		//Response=IdHTTP1->Get(AnsiString("http://85.255.8.81/tispl/")+LIC_FILE+UnicodeString(".lic"));

		FDQuery1->Active = False;
		FDQuery1->Open("SELECT  DATE_FORMAT(expiration_date ,'%d.%m.%Y %H:%i:%s') AS expiration_date FROM app_setup WHERE id=\"1\"");  //id nahradit id z ini
		FDQuery1->Active = True;
		AnsiString Response = FDQuery1->Fields->Fields[0]->AsAnsiString;

		try
		{
					//např. možné alternativy time serveru: 128.138.140.44 129.6.15.28 129.6.15.29 129.6.15.30
					try
					{
						IdTime1->Host="129.6.15.29";//testovací TIME SERVER, který nefunguje: 192.43.244.18
						TIME=IdTime1->DateTime;
					}
					catch(...)//v případě nedostupnosti timeserveru, zkusí ještě jiný
					{
						IdTime1->Host="128.138.140.44";//testovací TIMESERVER, který nefunguje: 192.43.244.18
						TIME=IdTime1->DateTime;
					}
					Form_uvod->Label_status->Visible=false;

					TIME_expirace	=TDateTime(Response);

					if(TIME_expirace<TIME && TIME!="1.1.1990 0:00:00")
					{
						log2web(ms.replace(Response,"_"," ")+"-"+Text+"_EXPIRACE");
						MB(Text_error);//vypršela licence resp. program expiroval;
						duvod_k_ulozeni=false;
						Timer_tr->Enabled=false;
						Close();
					}
					else //VŠE OK
					{
							log2web(ms.replace(Response,"_"," ")+"-"+Text+"_OK");
							SB("Datum expirace licence: "+TIME_expirace);
							aktualizace();//kontrola dostupnosti aktualizace
							STATUS=true;
					}
		}
		catch(...)//nezdařilo se připojení k time serveru, timeout
		{
			log2web(ms.replace(Response,"_"," ")+"-"+Text+"_TIMESERVER_ERR");
			//todo if(++n_prihlaseni>=3)//až při třetím chybovém stavu
			{
				Text_error="Nezdařilo se připojení k time serveru, aplikace nebude spuštěna!";
				MB(Text_error);
				duvod_k_ulozeni=false;
				Timer_tr->Enabled=false;
				Close();
			}
		}
	}
	catch(...)//nezdařilo se připojení k licenčnímu serveru
	{
		//tady nemůže být log
		Text_error="Nezdařilo se připojení k licenčnímu serveru, aplikace nebude spuštěna!"+AnsiString(n_prihlaseni+1);
		//todo if(++n_prihlaseni>=3)//až při třetím chybovém stavu
		{
			MB(Text_error);
			duvod_k_ulozeni=false;
			Timer_tr->Enabled=false;
			Close();
		}
	}

	if(!STATUS/*//todo  && n_prihlaseni>=3*/)//dvojúrovňová ochranu
	{
		duvod_k_ulozeni=false;
		Timer_tr->Enabled=false;
		Close();
		return false;//dvaapůl úrovňová ochrana
	}
	else
	return true;//dvaapůl úrovňová ochrana
}
//---------------------------------------------------------------------------
//při aktivaci formuláře startující záležitosti, pro zpřehlednění kodu
void TForm1::startUP()
{
	//////otevrení posledního souboru
	log2web("start");
	nastaveni.posledni_file=true;/////////////////provizorní než budu načítat z ini z filu nastavení zda otevírat či neotevírat poslední sobor
  volat_parametry_linky=true;//následně je případně znegováno
	UnicodeString user_file=ms.delete_repeat(ms.delete_repeat(Parametry,"\"",2),"\"").Trim();
	if(user_file!="")//pokud zkouší uživatel otevřít přímo soubor kliknutím na něj mimo aplikaci
	Otevrit_soubor(user_file);
	else
	{
		//načtení posledního otevřeného souboru
		if(nastaveni.posledni_file)
		{
			FileName=readINI("otevrene_soubory","posledni_soubor");
			if(FileName!="Nový.tispl" && FileName!=""){Otevrit_soubor(FileName);volat_parametry_linky=false;}
		}
	}

	//////automatický BACKUP
	//volá obnovu dat ze zálohy, pokud poslední ukončení programu neproběhlo standardně
	AnsiString status=readINI("Konec","status");
	if(status=="KO")//pokud došlo k pádu programu
	{
		//v případě spuštění aplikace po pádu se již znovu nevolá form parametry linky
		volat_parametry_linky=false;

		//zavře úvodní dialog
		zavrit_uvod();

		//načte název posledního souboru
		FileName=readINI("otevrene_soubory","posledni_soubor");

		//prvně porovná jestli otevřený soubor není náhoudou mladší než stejnomený BAC soubor
		FILETIME ftCreate, ftAccess, ftWrite,ftWrite_bac;
		HANDLE hFile=CreateFile(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING, 0, NULL);
		GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
		CloseHandle(hFile);
		hFile=CreateFile((FileName+".bac_"+get_user_name()+"_"+get_computer_name()).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING, 0, NULL);
		GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite_bac);
		CloseHandle(hFile);

		if(ftWrite.dwHighDateTime>=ftWrite_bac.dwHighDateTime)MB("Aplikace nebyla řádně ukončena. Byl obnoven poslední Vámi uložený soubor.");//pokud je uložený soubor mladší nebo stejně starý jako jeho BAC
		else
		{
			if(mrYes==MB("Aplikace nebyla řádně ukončena. Chcete ze zálohy obnovit poslední neuložený soubor?",MB_YESNO))
			{
				if(Otevrit_soubor(FileName+".bac_"+get_user_name()+"_"+get_computer_name())==1)
				{
					//ješti donutí stávajicí soubor uložit pod novým jménem
					//odstraniní koncovky
					//AnsiString jen_nazev=FileName;
					//while(jen_nazev.Pos(".bac")>0)//dokud bude ".bac" obsahovat
					//jen_nazev.Delete(jen_nazev.Pos(".bac"),jen_nazev.Length());
					FileName=ms.TrimRightFrom(FileName,".bac_"+get_user_name()+"_"+get_computer_name());
					UlozitjakoClick(this);
				}
				else
				{
					Obnovitzezlohy1Click(this);//zazálohovaný soubor se nezadařilo najít, proto jej vyhledá uživatel manuálně pomocí nabídnutého dialogu
				}
			}
		}
	}
	//zapíše status pro předčasné ukončení programu pro případ pádu programu
	writeINI("Konec","status","KO");

	//načte dílčí nastavení aplikace
	AnsiString T=readINI("Nastaveni_app","ortogonalizace"); //o_stav musí být až na druhém místě po scGPCheckbox
	start_ortogonalizace=true;//pouze ošetření, aby se nevolalo scGPCheckBox_ortogon_onclick
	if(T=="0" || T==""){scGPCheckBox_ortogon->Checked=false;ortogonalizace_stav=false;}else{scGPCheckBox_ortogon->Checked=true;ortogonalizace_stav=true;}
	start_ortogonalizace=false;
	T=readINI("Nastaveni_app","prichytavat");
	if(T=="0" || T==""){prichytavat_k_mrizce=0;}else{prichytavat_k_mrizce=ms.MyToDouble(T);}
	//zatím nepoužíváme, bude spíše souviset přímo se souborem, v případě použití nutno vyhodit implicitní volbu návrhář v sobuor novy
	//T=readINI("Nastaveni_app","status");
	//if(T=="0" || T=="")STATUS=NAVRH;else STATUS=OVEROVANI;

	//slouží po startu programu k načtení parametrů linky, nemůže být voláno v tomto okamžiku v souboru nový, protože by jinak vedlo k pádu aplikace - pořadí vytváření formulářů, není voláno v případě startu aplikace po pádu
	if(volat_parametry_linky)
	{
		Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
		Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
		Form_parametry_linky->ShowModal();//zavolání formáláře pro prvotní vyplnění či potvzení hodnot parametrů linky
	}
}
//---------------------------------------------------------------------------
//zajišťuje zápis do INI aplikace, museli jsme dát do výjimky, protože jednou hodilo error
void TForm1::writeINI(AnsiString Section,AnsiString Ident,AnsiString Value)
{
	try
	{
		TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
		ini->WriteString(Section,Ident,Value);
		delete ini;
	}
	catch(...)
	{
		;
	}
}
//---------------------------------------------------------------------------
//zajišťuje čtení z INI aplikace
AnsiString TForm1::readINI(AnsiString Section,AnsiString Ident)
{
	try
	{
		TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
		AnsiString status=ini->ReadString(Section,Ident,"");//hlavně nepoužívat poslední parametr!!!jinak bude app padat
		delete ini;
		return status;
	}
	catch(...)
	{
		return AnsiString("");
	}
}
//---------------------------------------------------------------------------
//Zalogování na webu
//automaticky přidá parametry (čas, uživatel, licence)
void TForm1::log2web(UnicodeString Text)
{
	if(!DEBUG)
	{
		//log2webOnlyText(ms.DeleteSpace(LICENCE)+"_"+get_computer_name()+"_"+get_user_name()+"_"+TIME.CurrentDate()+"_"+TIME.CurrentTime()+"|"+Text);
		try
		{
			AnsiString relation_id=GetCurrentProcessId();
			AnsiString send_log_time= TIME.CurrentDateTime();
			AnsiString ID ="1";
			AnsiString strSQL = "INSERT INTO log_table (app_id,app_start,username,send_log_time,command,relation_id,verze) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\",\""+VERZE+"\")";

			FDConnection1->ExecSQL(strSQL);
		}
		catch(...){;}//např. není připojení k internetu, tak pouze nezaloguje, dořešit uložení logu do doby získání připojení a volání opětovného odeslání logu
	}
}
//pouze text
void TForm1::log2webOnlyText(UnicodeString Text)
{

	//   varianta odesílání dat přes GET
	//IdHTTP1->Get(UnicodeString("http://85.255.8.81/tispl/skript_tispl.php?hash=erDSQgregdvgFEFSDDeporhrfFGOI98886732dfgorvmqwerfdvvcBHDE")+Text);

/*    varianta odesílání dat přes POST
		TStringList *request = new TStringList;
				TStringList *response = new TStringList();

				request->Clear();
				//IdHTTP1->IOHandler = IdSSLIOHandlerSocketOpenSSL1;
				IdHTTP1->Request->ContentType = "application/x-www-form-urlencoded";
		 //		Idssl

request->Values["app"]      = "tispl";
request->Values["key"]     = "";
request->Values["pass"]        ="2011_bozp*-";
request->Values["log_text"]     = "2011_bozp*-";

response->Text = IdHTTP1->Post("http://85.255.8.81/tispl/skript_tispl.php", request);
 */







	//catch(...){;}//není připojení k internetu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
	scListGroupKnihovObjektu->Height=scGPPanel_statusbar->Top-(2+scListGroupKnihovObjektu->Height+0+DetailsButton->Height);
	if(/*MOD==REZERVY ||*/ MOD==CASOVAOSA)Invalidate();
	else REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::schemaClick(TObject *Sender)
{
	ESC();//zruší případnou rozdělanou akci
	MOD=SCHEMA;
	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	Timer_simulace->Enabled=false;
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	//scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	scSplitView_LEFTTOOLBAR->Visible=true;
	scListGroupKnihovObjektu->Top=0;
//	scListGroupNastavProjektu->Visible=true;
	scListGroupKnihovObjektu->Visible=true;
 //	scListGroupKnihovObjektu->Top=scListGroupNastavProjektu->Height;
	DuvodUlozit(true);
	ButtonPLAY->Visible=false;
	Timer_neaktivity->Enabled=false;
	CheckBoxVytizenost->Visible=false;
	CheckBoxAnimovatSG->Visible=false;
	scLabel_doba_cekani->Visible=false;
	scGPGlyphButton_info_cekani->Visible=false;
	scGPGlyphButton_close_grafy->Visible=false;
	scExPanel_log_header->Visible=false;

	CheckBoxVymena_barev->Visible=false;
	Label_zamerovac->Visible=false;
	ComboBoxODmin->Visible=false;
	ComboBoxDOmin->Visible=false;
	rComboBoxKrok->Visible=false;
	scLabel_filtrovat->Visible=false;
	LabelRoletka->Visible=false;
	CheckBox_pouzit_zadane_kapacity->Visible=false;
	g.ShowGrafy(false);
	ComboBoxCekani->Visible=false;
	scGPCheckBox_ortogon->Align=alTop;
	scGPCheckBox_ortogon->Left=10;
	scGPCheckBox_ortogon->Visible=true;
  scGPCheckBox_zobraz_podklad->Align=alTop;
  scGPCheckBox_zobraz_podklad->Left=5;
  scGPCheckBox_stupne_sedi->Align=alTop;
  scGPCheckBox_stupne_sedi->Left=scGPCheckBox_zobraz_podklad->Left;
  scLabel1_svetelnost->Top=scGPCheckBox_stupne_sedi->Top + pravyoption_nadpis->Height + 14;  //pár px navíc kvůli vzdušnosti
  scGPTrackBar_svetelnost_posuvka->Top=scLabel1_svetelnost->Top;

  scGPButton_kalibrace->Left = 3;
  scGPButton_adjustace->Left=  scGPButton_kalibrace->Left;
  scGPButton_kalibrace->Top=scGPTrackBar_svetelnost_posuvka->Top + scLabel1_svetelnost->Height + 9;  //pár px navíc kvůli vzdušnosti
  scGPButton_adjustace->Top=scGPButton_kalibrace->Top +  scGPButton_kalibrace->Height ;

  //scGPCheckBox_stupne_sedi->Top=scGPCheckBox_zobraz_podklad->Top+scGPCheckBox_zobraz_podklad->Height;
  //scLabel1_svetelnost->Top= scGPCheckBox_stupne_sedi->Top + scGPCheckBox_stupne_sedi->Height;
 // scGPTrackBar_svetelnost_posuvka->Top  = scLabel1_svetelnost->Top;
  //scGPButton_kalibrace->Top = scLabel1_svetelnost->Top + scLabel1_svetelnost->Height;
  //scGPButton_adjustace->Top = scLabel1_svetelnost->Top + scLabel1_svetelnost->Height;
	scGPCheckBox_pocet_voziku_dle_WIP->Visible=false;
	scGPGlyphButton_close_legenda_casove_osy->Visible=false;
 //	scGPButton_header_def_zakazek->Visible=false;
 //	scGPButton_header_param_linky->Visible=false;
	Pan_bmp->Width=0;Pan_bmp->Height=0;//při přechodu z jiného režimu smaže starou Pan_bmp
	SB("Kliknutím na libovolné místo přidáte objekt z knihovny nebo lze upravit stávájící schéma");

	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::testovnkapacity1Click(TObject *Sender)
{
//	MOD=TESTOVANI;
//	SB("testování kapacity",1);
//	if(!zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=true;}
//	Timer_simulace->Enabled=false;
//	DuvodUlozit(true);
//	//editacelinky1->Checked=false;
//	//testovnkapacity1->Checked=true;
//	//casoverezervy1->Checked=false;
//	//simulace1->Checked=false;
//	//casovosa1->Checked=false;
//	//technologickprocesy1->Checked=false;
//	scListGroupNastavProjektu->Visible=true;
//	scListGroupKnihovObjektu->Visible=true;
//	PopupMenu1->AutoPopup=true;
//	Timer_neaktivity->Enabled=false;
//	Timer_animace->Enabled=false;
//	CheckBoxVytizenost->Visible=false;
//	CheckBoxAnimovatSG->Visible=false;
//	CheckBoxPALCE->Visible=false;
//	CheckBoxVymena_barev->Visible=false;
//	ButtonPLAY->Visible=false;
//	Label_zamerovac->Visible=false;
//	ComboBoxODmin->Visible=false;
//	ComboBoxDOmin->Visible=false;
//	rComboBoxKrok->Visible=false;
//	LabelRoletka->Visible=false;
//	CheckBox_pouzit_zadane_kapacity->Visible=false;
//	g.ShowGrafy(false);
//	ComboBoxCekani->Visible=false;
//	GlyphButton_close_grafy->Visible=false;
//  scGPCheckBox_ortogon->Visible=false;
//	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LayoutClick(TObject *Sender)
{
	ESC();//zruší případnou rozdělanou akci
	MOD=LAYOUT;
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
 //	scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	scSplitView_LEFTTOOLBAR->Visible=false;
	scGPCheckBox_ortogon->Visible=false;
	scGPCheckBox_pocet_voziku_dle_WIP->Visible=false;
	scGPGlyphButton_close_legenda_casove_osy->Visible=false;
	scGPGlyphButton_close_grafy->Visible=false;
	g.ShowGrafy(false);//vypne grafy (případ pokud by se přecházelo z časových os do layoutu)
	SB("Zobrazení navrženého layoutu");
	//Zoom=5;ZOOM();
	Pan_bmp->Width=0;Pan_bmp->Height=0;//při přechodu z jiného režimu smaže starou Pan_bmp
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::casoverezervy1Click(TObject *Sender)
{
//	MOD=REZERVY;
//	ESC();//zruší případně rozdělanou akci
//	SB("časové rezervy",1);
//	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
//	Timer_simulace->Enabled=false;
//	//testovnkapacity1->Checked=false;
//	//editacelinky1->Checked=false;
//	//casoverezervy1->Checked=true;
//	//simulace1->Checked=false;
//	//casovosa1->Checked=false;
//	//technologickprocesy1->Checked=false;
//	DuvodUlozit(true);
//	scListGroupNastavProjektu->Visible=false;
//	scListGroupKnihovObjektu->Visible=false;
//	PopupMenu1->AutoPopup=false;
//	Timer_animace->Enabled=false;
//	ButtonPLAY->Visible=false;
//	CheckBoxVytizenost->Visible=false;
//	CheckBoxAnimovatSG->Visible=false;
//	CheckBoxPALCE->Visible=false;
//	CheckBoxVymena_barev->Visible=false;
//	Label_zamerovac->Visible=false;
//	ComboBoxODmin->Visible=false;
//	ComboBoxDOmin->Visible=false;
//	rComboBoxKrok->Visible=false;
//	LabelRoletka->Visible=false;
//	CheckBox_pouzit_zadane_kapacity->Visible=false;
//	g.ShowGrafy(false);
//	ComboBoxCekani->Visible=false;
//	GlyphButton_close_grafy->Visible=false;
//	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AnalyzaClick(TObject *Sender)
{
	d.v.prvni_zakazka_dle_schematu();//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji pouze přiřadí cestu dle schématu
//	if(d.v.ZAKAZKY->dalsi==NULL)//pokud nebyla zakazka definovaná - nyní řeší příkaz nad
//	{
//		MB("Pro zobrazení je nutné ve formuláři definice zakázek zadat plán výroby!");
//	}
//	else
	{
		if(d.v.VOZIKY->dalsi==NULL)d.v.generuj_VOZIKY();//situace kdy nejsou načtené vozíky ale existuje zakázka s cestou (situace např. po načtení nového souboru), tak se vygeneruji dle zadané zakazky/cesty vozíky

		if(MOD!=CASOVAOSA)//aby se nevolalo zbytečně znovu, pokud už v daném modu je, ale může být dvousečné ve významu uživatelské užitečnosti
		{
			MOD=CASOVAOSA;
			ESC();//zruší případně rozdělanou akci
			if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
			d.mod_vytizenost_objektu=false;
			Timer_simulace->Enabled=false;
			d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
			d.PosunT.y=0;
			//zneplatnit_minulesouradnice();//zrušeno test
			DuvodUlozit(true);
			scSplitView_MENU->Opened=false;//zavře případně otevřené menu
			scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	 //		scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
			scSplitView_LEFTTOOLBAR->Visible=false;
			scListGroupKnihovObjektu->Visible=false;
			Button3->Visible=false;
			Timer_neaktivity->Enabled=true;
			pocitadlo_doby_neaktivity=0;//implicitní hodnota
			Timer_animace->Enabled=false;
			ButtonPLAY->Visible=false;
			CheckBoxVymena_barev->Visible=true;
			CheckBoxVytizenost->Visible=true;
			CheckBoxVytizenost->Checked=false;
			CheckBoxVytizenost->Top=135;
			scLabel_doba_cekani->Visible=true;
			scGPGlyphButton_info_cekani->Visible=true;
			scGPGlyphButton_close_grafy->Visible=true;
			CheckBoxAnimovatSG->Visible=false;
			ComboBoxODmin->Visible=false;
			ComboBoxDOmin->Visible=false;
			rComboBoxKrok->Visible=false;
			LabelRoletka->Visible=false;
			CheckBox_pouzit_zadane_kapacity->Visible=false;
			ComboBoxCekani->Visible=true;
			d.JIZPOCITANO=false;d.RANDOM=true;
			scExPanel_log_header->Visible=true;
			ComboBoxCekani->Width=scSplitView_OPTIONS->OpenedWidth-7;
		 //	scGPGlyphButton_zpravy_ikona->Visible=true;
			if(Form1->ComboBoxCekani->ItemIndex==2)
			{
				scGPButton_generuj->Visible=true;
				ComboBoxCekani->Width=196;
			}

			Label_zamerovac->Visible=false;
			scGPCheckBox_ortogon->Visible=false;
			if(STATUS==NAVRH)scGPCheckBox_pocet_voziku_dle_WIP->Visible=true;
			scGPGlyphButton_close_legenda_casove_osy->Left=0;
			scGPGlyphButton_close_legenda_casove_osy->Top=ClientHeight-scGPPanel_statusbar->Height-ClientHeight/3;
			scGPGlyphButton_close_legenda_casove_osy->Visible=true;
			SB("Zobrazení technologických procesů v čase formou časových os.");
			Invalidate();
		}
	}
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
//zavře nebo otevře grafy v časových osách
void __fastcall TForm1::scGPGlyphButton_close_grafyClick(TObject *Sender)
{
	if(scGPGlyphButton_close_grafy->GlyphOptions->Kind==scgpbgkDownArrow)
	{
		scGPGlyphButton_close_grafy->Top=scGPPanel_statusbar->Top-Form1->scGPGlyphButton_close_grafy->Height;
		g.ShowGrafy(false);
		scGPGlyphButton_close_grafy->GlyphOptions->Kind=scgpbgkUpArrow;//změní typ ikony
		scGPGlyphButton_close_grafy->ShowCaption=true;//zobrazí titulek
		if(scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind==scgpbgkUpArrow)scGPGlyphButton_close_legenda_casove_osy->Top=scGPPanel_statusbar->Top-Form1->scGPGlyphButton_close_grafy->Height;//buttonglyph legendy přesune pokud je minimalizováná
	}
	else
	{
		scGPGlyphButton_close_grafy->Top=Form1->Chart2->Top;
		g.ShowGrafy(true);
		scGPGlyphButton_close_grafy->GlyphOptions->Kind=scgpbgkDownArrow;//změní typ ikony
		scGPGlyphButton_close_grafy->ShowCaption=false;//skryje titulek
		if(scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind==scgpbgkUpArrow)scGPGlyphButton_close_legenda_casove_osy->Top=Form1->Chart2->Top;//buttonglyph legendy přesune pokud je minimalizováná
	}
	scGPGlyphButton_close_grafy->Left=Form1->ClientWidth-Form1->scGPGlyphButton_close_grafy->Width;
	//pozn. šířka Buttnu se zadává pomocí paramerů Width, WidthWithCaption,WidthWitoutCaption v proporties
	REFRESH();
}
//---------------------------------------------------------------------------
//přepíná tlačito na skrytí nebo zobrazí legendu časových os, dle stavu tlačítka dochází poté k samotnému skrývání či zobrazování v Cvykresli vykresli_legendu_casovych_os(TCanvas *canv)
void __fastcall TForm1::scGPGlyphButton_close_legenda_casove_osyClick(TObject *Sender)
{
	if(scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind==scgpbgkDownArrow)
	{
		scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind=scgpbgkUpArrow;//změní typ ikony
		scGPGlyphButton_close_legenda_casove_osy->ShowCaption=true;
	}
	else
	{
		scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind=scgpbgkDownArrow;//změní typ ikony
		scGPGlyphButton_close_legenda_casove_osy->ShowCaption=false;
	}
	//pozn. šířka Buttonu se zadává pomocí paramerů Width, WidthWithCaption,WidthWitoutCaption v proporties
	REFRESH();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::SyntezaClick(TObject *Sender)
{
	MOD=TECHNOPROCESY;
	ESC();//zruší případně rozdělanou akci
	SB("Zobrazení technologických procesů v čase v jednotlivých objektech.");
	//if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	Timer_simulace->Enabled=false;
	d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
	d.PosunT.y=0;
	zneplatnit_minulesouradnice();
	//tady bylo showgrafy(false) - ale zamrzalo
	DuvodUlozit(true);
	//pozor musí být některé z dříve zde uloženýchpoložek dole jinak zamrzá
	//PopupMenu1->AutoPopup=false;
	Button3->Visible=false;
	Timer_neaktivity->Enabled=false;
	CheckBoxVymena_barev->Visible=false;
	scGPButton_generuj->Visible=false;

	CheckBoxVytizenost->Visible=false;
	CheckBoxAnimovatSG->Visible=true;
	scLabel_filtrovat->Visible=true;
	CheckBox_pouzit_zadane_kapacity->Visible=true;
	ComboBoxDOmin->Visible=true;
	ComboBoxODmin->Visible=true;
	rComboBoxKrok->Visible=true;

	scLabel_filtrovat->Top=scLabel_doba_cekani->Top;
	ComboBoxODmin->Top=ComboBoxCekani->Top;
	ComboBoxDOmin->Top=ComboBoxODmin->Top;
	ButtonPLAY->Top=ComboBoxODmin->Top-5;
	ComboBoxDOmin->Left=64;
	rComboBoxKrok->Top=ComboBoxODmin->Top;
	CheckBoxAnimovatSG->Top=CheckBoxVymena_barev->Top;
	CheckBox_pouzit_zadane_kapacity->Top=CheckBoxVytizenost->Top;
  ComboBoxCekani->Visible=true;

	CheckBoxVymena_barev->Visible=false;
	scLabel_doba_cekani->Visible=false;
	scGPGlyphButton_info_cekani->Visible=false;
	CheckBox_pouzit_zadane_kapacity->Visible=true;
	//filtrace
	d.TP.K=0.5;//Krok po kolika minutach se bude zobrazovat
	d.TP.OD=0;//od které min se proces začne vypisovat
  d.TP.KZ=d.v.vrat_nejpozdejsi_konec_zakazek();//konec zakazky v min
	d.TP.DO=d.TP.KZ;
	d.TP.Nod=0;//rozmezí Jaký se vypíše vozik,
	d.TP.Ndo=0;//rozmezí Jaký se vypíše vozik, pokud bude 0 vypisují se všechny
	d.TP.A=false;//jednořádková animace
	Timer_animace->Enabled=false;
	ButtonPLAY->Visible=true;
	ButtonPLAY->Caption="PLAY";
	LabelRoletka->Visible=true;
	LabelRoletka->Caption="Filtr minut";
	LabelRoletka->Font->Color=clBlack;
	scGPCheckBox_ortogon->Visible=false;
  scGPCheckBox_pocet_voziku_dle_WIP->Visible=false;
	scGPGlyphButton_close_legenda_casove_osy->Visible=false;
	scGPGlyphButton_close_grafy->Visible=false;


	int konec_cas=m.round(d.v.vrat_nejpozdejsi_konec_zakazek()/10.0);
	for (int i = 1; i <= 10; i++) {
		int val=konec_cas*i;
		ComboBoxODmin->Items->Add(val);
		ComboBoxDOmin->Items->Add(val);
	}
	//ComboBoxODmin->Items->Add(d.v.vrat_nejpozdejsi_konec_zakazek());
	//ComboBoxDOmin->Visible=true;
	//ComboBoxDOmin->Top=CheckBoxPALCE->Top;
	//ComboBoxDOmin->Left=ComboBoxODmin->Left+40+2;
	//ComboBoxDOmin->Items->Add(d.TP.KZ);//plnění komba max časem

	//CheckBox_pouzit_zadane_kapacity->Visible=true;
 //	CheckBox_pouzit_zadane_kapacity->Top=CheckBoxPALCE->Top;
	//CheckBox_pouzit_zadane_kapacity->Left=CheckBoxAnimovatSG->Left+CheckBoxAnimovatSG->Width+7;

	//---
	//pozor musí být některé z položek dole jinak zamrzá:
	ComboBoxCekani->Visible=false;
	Label_zamerovac->Visible=false;
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	//scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	scSplitView_LEFTTOOLBAR->Visible=false;
	scListGroupKnihovObjektu->Visible=false;
	g.ShowGrafy(false);
	Invalidate();
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
void __fastcall TForm1::simulace1Click(TObject *Sender)
{
	/*MOD=SIMULACE;
	ESC();//zruší případně rozdělanou akci
	SB("zobrazení animované simulace",1);
	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	//testovnkapacity1->Checked=false;
	//editacelinky1->Checked=false;
	//casoverezervy1->Checked=false;
	//simulace1->Checked=true;
	//casovosa1->Checked=false;
	//CheckBoxPALCE->Visible=false;
	g.ShowGrafy(false);
	DuvodUlozit(true);
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	scSplitView_LEFTTOOLBAR->Visible=false;
	scListGroupNastavProjektu->Visible=false;
	scListGroupKnihovObjektu->Visible=false;

	scListGroupNastavProjektu->Visible=false;
	scListGroupKnihovObjektu->Visible=false;
	//PopupMenu1->AutoPopup=false;
	Button3->Visible=true;
	d.cas=0;
	//ZDM d.priprav_palce();
	Timer_simulace->Enabled=true;
	Timer_neaktivity->Enabled=false;
	Timer_animace->Enabled=false;
	ButtonPLAY->Visible=false;
	CheckBoxVytizenost->Visible=false;
	CheckBoxAnimovatSG->Visible=false;
	CheckBoxVymena_barev->Visible=false;
	scLabel_doba_cekani->Visible=false;
	scGPGlyphButton_info_cekani->Visible=false;
	Label_zamerovac->Visible=false;
	ComboBoxODmin->Visible=false;
	ComboBoxDOmin->Visible=false;
	rComboBoxKrok->Visible=false;
	ComboBoxCekani->Visible=false;
	scGPCheckBox_ortogon->Visible=false;
	scGPGlyphButton_close_legenda_casove_osy->Visible=false;
	Invalidate();*/
}
//---------------------------------------------------------------------------
//skryje či zobrazí mřížku
void __fastcall TForm1::scGPSwitch_gridChangeState(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
	grid=!grid;
	if(!grid)SB("mřížka skryta",5);
	else akutalizace_stavu_prichytavani_vSB();
	REFRESH();
}
//---------------------------------------------------------------------------
void TForm1::SB(UnicodeString Text, unsigned short Pane)
{
	switch(Pane)
	{
		case 1:RzStatusPane1->Caption=Text;break;
		//case 2:RzStatusPane2->Caption="Zoom: "+Text+"x";break; už se nepoužívá
		case 3:RzStatusPane3->Caption="["+Text+"] m";break;
		case 4:RzStatusPane4->Caption=Text;break;
		case 5:RzStatusPane5->Caption=Text;break;
		case 6:RzStatusPane3->Caption=Text;break;//pokud nechci vypisovat metry
		default:RzStatusPane5->Caption=Text; break;
	}
}
//---------------------------------------------------------------------------
void TForm1::S(UnicodeString Text)
{
		ShowMessage(Text);
}
//---------------------------------------------------------------------------
//usnadňuje přístup k ShowMessage - MaKr
void TForm1::Sk(UnicodeString Text,AnsiString umisteni)
{
	S(umisteni+"/MaKr\n"+Text);
}
//---------------------------------------------------------------------------
//usnadňuje přístup k ShowMessage - MaVl
void TForm1::Sv(UnicodeString Text,AnsiString umisteni)
{
	S(umisteni+"/MaVl\n"+Text);
}
//---------------------------------------------------------------------------
//vola rychle myMessageBox
int TForm1::MB(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit,int width,bool default_button_caption)
{
	return myMessageBox->Show(left,top,text,caption_text,mbTYPE,centrovat_text,checkbox_zobrazit,width,default_button_caption);
}
int TForm1::MB(UnicodeString text,int mbTYPE,bool centrovat_text,int width,bool default_button_caption)
{
	return myMessageBox->Show(text,mbTYPE,centrovat_text,width,default_button_caption);
}
//---------------------------------------------------------------------------
//usnadňuje přístup ke zprávám, pokud jsou jen prázdné uvozovky (a druhý paremetry na false - což je implicitně), vymaže zpravu, parametr add rozhoduje, zda bude nový text předen k předešlému textu či nikoliv, pokud zpráva obsahuje nějaký text, je zobrazena ikona zprávy, poslední parametr je barva ikony zprávy
void TForm1::Z(UnicodeString Text,bool add,TColor color)
{
	//plnění zprávy textem
	if(add)scHTMLLabel_log_vypis->Caption = scHTMLLabel_log_vypis->Caption+Text;//text má být přidán k předchozímu textu
	else scHTMLLabel_log_vypis->Caption = Text;//text má být přidán samostatně

	//zobrazení či skrývání
	if(Text=="" && add==false)//vše OK
	{
		scGPGlyphButton_zpravy_ikona->GlyphOptions->NormalColor=clWhite;
		scHTMLLabel_log_vypis->Caption="";
		//Memo2->Lines->Clear();
	}
	else
	{
		//není možné: scExPanel_log_header->Visible=true; //expanel s obsahem zprávy
		scGPGlyphButton_zpravy_ikona->Visible=true;//ikona
		scGPGlyphButton_zpravy_ikona->GlyphOptions->NormalColor=color;//barva ikony
	}
	//pozice
	scExPanel_log_header->Left	 = Form1->Width/2-scExPanel_log_header->Width/2;
	scExPanel_log_header->Top 	 = Form1->Height/2-scExPanel_log_header->Height/2;
	scExPanel_log_header->Height = 300;
	scExPanel_log_header->Width	 = 715;
  //Memo2->Visible=true;
	//Memo2->Lines->Add(Text);
}
//---------------------------------------------------------------------------
//metoda volá kurzory aplikace
void TForm1::kurzor(TKurzory typ_kurzor)
{
	if(typ_kurzor>0)Screen->Cursor=(TCursor)typ_kurzor;
	else Screen->Cursor=crDefault;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
//	if(FileName_short(FileName)=="VÝHYBKY_TEST.tispl" || FileName_short(FileName)=="VÝHYBKY_TESTm.tispl" || FileName_short(FileName)=="VÝHYBKY_TESTv.tispl")
//	{
//		SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
//		if(FileExists("conf") && FileExists("conf2") && FileExists("conf3"))
//		{
//			Graphics::TBitmap *bmp=new Graphics::TBitmap;
//			if(FileName_short(FileName)=="VÝHYBKY_TEST.tispl")bmp->LoadFromFile("conf");
//			if(FileName_short(FileName)=="VÝHYBKY_TESTm.tispl")bmp->LoadFromFile("conf2");
//			if(FileName_short(FileName)=="VÝHYBKY_TESTv.tispl")bmp->LoadFromFile("conf3");
//			Canvas->Draw(scSplitView_LEFTTOOLBAR->Width,scGPPanel_mainmenu->Height,bmp);
//			delete(bmp);
//			d.v.PP.raster.show=true;
//		}
//	}


	//načtení rastru
	if(d.v.PP.raster.filename!="" &&  d.v.PP.raster.show)
	{
		if(FileExists(d.v.PP.raster.filename))
		{
			if(d.v.PP.raster.resolution==0)d.v.PP.raster.resolution=m2px;//v případě nového vstupu bez zadaného rozlišení defaultně je m2px
			Graphics::TBitmap *bmp=new Graphics::TBitmap;
			bmp->LoadFromFile(d.v.PP.raster.filename);
			//bílé smazání pozadí nutné v případě AA vektorů a podním načítaným rastrem
			Canvas->Pen->Color=clWhite;Canvas->Brush->Color=clWhite;
			Canvas->Rectangle(scSplitView_LEFTTOOLBAR->Width,scGPPanel_mainmenu->Height,Width,Height);
			//vykreslení strečovaného rastru dle zadaného rozlišení
			Canvas->StretchDraw(TRect(m.L2Px(d.v.PP.raster.X),m.L2Py(d.v.PP.raster.Y),m.round(m.L2Px(d.v.PP.raster.X)+bmp->Width*Zoom*d.v.PP.raster.resolution/m2px),m.round(m.L2Py(d.v.PP.raster.Y)+bmp->Height*Zoom*d.v.PP.raster.resolution/m2px)),bmp);
			delete(bmp);
		}
		else d.v.PP.raster.show=false;
	}

	if(!nahled_objektu)
	{
	//vykreslení gridu
	if(grid && Zoom>0.5 && !antialiasing && MOD!=LAYOUT && MOD!=NAHLED &&/*MOD!=REZERVY &&*/ MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)d.vykresli_grid(Canvas,size_grid);//pokud je velké přiblížení tak nevykreslí

	//jednoltivé mody
	Zoom_predchozi_AA=Zoom;//musí být tu, před mody (mohl by být i před kreslením gridu)
	switch(MOD)
	{
		case NAHLED://vykreslení všech vektorových elementů
		{
			if(!antialiasing)d.vykresli_vektory(Canvas);
			else
			{
				Cantialising a;
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				//zkoušel jsem nastavit plochu antialiasingu bez ovládacích prvků LeftToolbar a menu, ale kopírování do jiné BMP to zpomalovalo více neooptimalizovaná oblast pro 3xbmp
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
				d.vykresli_vektory(bmp_in->Canvas);
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in); //velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
				if(d.v.PP.raster.show)//z důvodu toho, aby pod bmp_out byl vidět rastrový podklad
				{
					bmp_out->Transparent=true;
					bmp_out->TransparentColor=clWhite;
				}
				else bmp_out->Transparent=false;

//				//test odstranění blikání mgridů viz bmp_out->Canvas->Draw(100,100,bmp);do bmp předat hotový mgrid
//				SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
//				Graphics::TBitmap *bmp=new Graphics::TBitmap;
//				bmp->LoadFromFile("test.bmp");
//				bmp_out->Canvas->Draw(100,100,bmp);
//				delete(bmp);
//				//---test

				Canvas->Draw(0,0,bmp_out);
				delete (bmp_out);//velice nutné
				delete (bmp_in);//velice nutné
			}
			//vykreslování mGridu
			if(pom_temp->elementy!=NULL && refresh_mGrid)d.vykresli_mGridy();

			//grafické měřítko
			if(scGPSwitch_meritko->State==true)d.meritko(Canvas);
			break;
    }
		case SCHEMA://vykreslování všech vektorů ve schématu
		{
			if(!antialiasing)d.vykresli_objekty(Canvas);
			else
			{
				Cantialising a;
				Graphics::TBitmap *bmp_grid=new Graphics::TBitmap;
				bmp_grid->Width=0;bmp_grid->Height=0;
				if(grid && Zoom_predchozi_AA>0.5)//je-li grid zobrazen
				{
					bmp_grid->Width=ClientWidth;bmp_grid->Height=ClientHeight;
					d.vykresli_grid(bmp_grid->Canvas,size_grid);//pokud je velké přiblížení tak nevykreslí//vykreslení gridu
				}
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				//zkoušel jsem nastavit plochu antialiasingu bez ovládacích prvků LeftToolbar a menu, ale kopírování do jiné BMP to zpomalovalo více neooptimalizovaná oblast pro 3xbmp
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
				d.vykresli_objekty(bmp_in->Canvas);
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_grid,bmp_in); //velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
				if(d.v.PP.raster.show)//z důvodu toho, aby pod bmp_out byl vidět rastrový podklad
				{
					bmp_out->Transparent=true;
					bmp_out->TransparentColor=clWhite;
				}
				else bmp_out->Transparent=false;

				Canvas->Draw(0,0,bmp_out);
				delete (bmp_out);//velice nutné
				delete (bmp_grid);//velice nutné
				delete (bmp_in);//velice nutné
			}
			//grafické měřítko
			if(scGPSwitch_meritko->State==true)d.meritko(Canvas);
			break;
		}
		case LAYOUT:
		{
			if(!antialiasing)d.vykresli_layout(Canvas);
			else
			{
				Cantialising a;
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				Graphics::TBitmap *bmp_grid=new Graphics::TBitmap;bmp_grid->Width=0;bmp_grid->Height=0;//je nutné mít založeno, ač nemá v tomto případě význam
				//zkoušel jsem nastavit plochu antialiasingu bez ovládacích prvků LeftToolbar a menu, ale kopírování do jiné BMP to zpomalovalo více neooptimalizovaná oblast pro 3xbmp
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
				d.vykresli_layout(bmp_in->Canvas);
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_grid,bmp_in); //velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
				Canvas->Draw(0,0,bmp_out);
				delete (bmp_out);//velice nutné
				delete (bmp_grid);//velice nutné
				delete (bmp_in);//velice nutné
			}
			//grafické měřítko
			if(scGPSwitch_meritko->State==true)d.meritko(Canvas);
		}
		break;
//		case REZERVY: d.vykresli_graf_rezervy(Canvas);break;//vykreslení grafu rezerv
		case CASOVAOSA:
		{
			/* v přípravě	if(!antialiasing)d.vykresli_casove_osy(Canvas);
			else
			{
				Cantialising a;
				Graphics::TBitmap *bmp_grid=new Graphics::TBitmap;
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
				d.vykresli_casove_osy(bmp_in->Canvas);
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Canvas->Draw(0,0,a.antialiasing(bmp_grid,bmp_in));
				delete (bmp_grid);bmp_grid=NULL;//velice nutné
				delete (bmp_in);bmp_in= NULL;//velice nutné
			}
			break;
			*/
			if(SplitViewOpen==false)//souvisí s problémem se zobrazeným splitview menu nebo options bez toho způsobovalo pomalé skrývání těchto menn
			{
				//d.vykresli_casove_osy(Canvas);  //puvodni konstrukce ověřit proč nemůže být použita a místo toho se používá ta bitmapa
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				bmp_in->Width=ClientWidth;bmp_in->Height=ClientHeight;
				d.vykresli_casove_osy(bmp_in->Canvas);
				Canvas->Draw(0,scGPPanel_mainmenu->Height,bmp_in);
				delete (bmp_in);//velice nutné
			}
			//d.vykresli_svislici_na_casove_osy(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);//nový přístup v zobrazování svislic, jen v momentu zobrazování labalu_zamerovac (bylo odkomentováno)
			break;
		}
		case TECHNOPROCESY:	//d.vykresli_technologicke_procesy(Canvas); break; puvodni konstrukce
		if(SplitViewOpen==false)
		{
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=ClientWidth;bmp_in->Height=ClientHeight;
			d.vykresli_technologicke_procesy(bmp_in->Canvas);
			Canvas->Draw(0,scGPPanel_mainmenu->Height,bmp_in);
			delete (bmp_in);//velice nutné
			//grafické měřítko
			if(scGPSwitch_meritko->State==true)d.meritko(Canvas);
		}
		break;
//		//	case SIMULACE:d.vykresli_simulaci(Canvas);break; - probíhá už pomocí timeru, na tomto to navíc se chovalo divně
	}
	}
}
//---------------------------------------------------------------------------
void TForm1::REFRESH(bool mGrid)
{
	if(mGrid)refresh_mGrid=true;else refresh_mGrid=false;
	FormPaint(this);
	refresh_mGrid=true;//vrátí do původního stavu
	if(Label_wip->Visible)Label_wip->Invalidate();//}//pokude je zapntutý antialiasing neproblikne, ale jen se "přeplácne" bitmapou nedojde k probliknutí
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ //ShowMessage(Key);
	funkcni_klavesa=0;
	int HG=0; if(scGPGlyphButton_close_grafy->GlyphOptions->Kind==scgpbgkDownArrow)HG=Chart2->Height;//o výšku grafu
	int PXM=50;int D=Form1->m.round(d.v.PP.delka_jig*PXM);int S=Form1->m.round(d.v.PP.sirka_jig*PXM);short Yofset=D;if(S>D)Yofset=S;//pro posun obrazu v technologických procesech
	switch(Key)
	{
		//BACKSPACE
		case 8: break;
		//ENTER
		case 13:break;
		//ESC
		case 27:ESC();break;
		//MEZERNÍK
		case 32: if(Akce!=PAN_MOVE){Akce=PAN;kurzor(pan);}break;
		//PAGE UP
		case 33:
			if(MOD==CASOVAOSA && d.v.VOZIKY->predchozi!=NULL && d.PosunT.y>=(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height-HG)/(float)d.KrokY*d.KrokY)
			{
				d.PosunT.y-=(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height-HG)/(float)d.KrokY*d.KrokY;
				unsigned int V=ceil((d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0));//zjistí aktuální číslo vozíku; pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
				Cvektory::TVozik *Vozik=d.v.vrat_vozik(V);
				if(Vozik==NULL)d.PosunT.x=0;else d.PosunT.x=Vozik->start;
				Invalidate();
			}
			break;
		//PAGE DOWN
		case 34:
			if(MOD==CASOVAOSA && d.v.VOZIKY->predchozi!=NULL && d.PosunT.y<(d.v.VOZIKY->predchozi->n+1)*d.KrokY-ClientHeight+scGPPanel_statusbar->Height+scLabel_titulek->Height+HG)
			{
				d.PosunT.y+=(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height-HG)/(float)d.KrokY*d.KrokY;
				unsigned int V=ceil((d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0));//zjistí aktuální číslo vozíku; pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
				d.PosunT.x=d.v.vrat_vozik(V)->start;
				Invalidate();
			}
			break;
		//END
		case 35:
			if(MOD==CASOVAOSA && d.v.VOZIKY->predchozi!=NULL)//na časové ose na poslední vozík
			{
				d.PosunT.x=d.v.VOZIKY->predchozi->pozice-ClientWidth+Canvas->TextWidth(d.v.VOZIKY->predchozi->pozice*d.PX2MIN)/2+2;
				d.PosunT.y=(d.v.VOZIKY->predchozi->n+1)*d.KrokY-ClientHeight+scGPPanel_statusbar->Height+scLabel_titulek->Height+HG;
			}
			Invalidate();
			break;
		//HOME
		case 36:if(d.PosunT.x)d.PosunT.x=0;if(d.PosunT.y)d.PosunT.y=0;Invalidate();break;//posun na časových osách na začátek
		//ŠIPKA DOLŮ
		case 40:
			if(MOD==TECHNOPROCESY){d.PosunT.y+=Yofset;Invalidate();}
			else{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y+1);}//jinak posun kurzoru
			break;
		//ŠIPKA LEVÁ
		case 37:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x-1,Mouse->CursorPos.y);break;}
		//ŠIPKA PRAVÁ
		case 39:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x+1,Mouse->CursorPos.y);break;}
		//ŠIPKA NAHORU
		case 38:
			if(MOD==TECHNOPROCESY){d.PosunT.y-=Yofset;Invalidate();}
			else{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y-1);}
			break;
		//ŠIPKA DOLŮ
		case 98:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y+1); break;}
		//ŠIPKA LEVÁ
		case 100:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x-1,Mouse->CursorPos.y);break;}
		//ŠIPKA PRAVÁ
		case 102:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x+1,Mouse->CursorPos.y);break;}
		//ŠIPKA NAHORU
		case 104:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y-1);break;}
		//CTRL+M
		case 77: if(ssCtrl)Akce=MEASURE;kurzor(add_o);break;
		//F1 - volání nápovědy
		case 112:break;
		//F2
		case 113:break;
		//F3 - pohled celé schéma
		case 114:RzToolButton11Click(Sender);break;
		//F4
		case 115:if(!Shift.Contains(ssAlt))Vybratoknem1Click(Sender);break;//pokud se nejedná o ALT+F4
		//F5
		case 116:Posouvat1Click(Sender);break;
		//F6
		case 117:Posunout2Click(Sender);break;
		//F7
		case 118:ZOOM_IN();break;
		//F8
		case 119:ZOOM_OUT();break;
		//F9
		case 120:Button11->Visible!=Button11->Visible;break;
		//F10
		case 121: Invalidate();break;
		//F11
		case 122:ortogonalizace_on_off();break;//přepíná stav automatické ortogonalizace
		//F12
		case 123:ortogonalizovat();SB("Ortogonalizace schématu dokončena.");REFRESH();DuvodUlozit(true);break;//ortogonalizuje schéma
		//CTRL, SHIFT pro další použití, pro klávesové zkratky např. if(ssCtrl && Key==86) jako ctrl+V
		default:
		{
			if(Shift.Contains(ssShift) && Shift.Contains(ssCtrl)){funkcni_klavesa=3;}//SHIFT + CTRL
			else
			{
				if(Shift.Contains(ssShift)){funkcni_klavesa=2;}//SHIFT
				if(Shift.Contains(ssCtrl)){funkcni_klavesa=1;}//CTRL
			}
			break;
		}
	}
/*ascii
  78 - n
86 - v
67 - c
79 - o
83 - s
90 - z
89 - y
70 - f
82 - r
65 - a */

	//pro klávesové zkratky např. if(ssCtrl && Key==86) jako ctrl+V
	/*if(funkcni_klavesa==1)//CTRL
	switch(Key)
	{
		case 33:d.PosunT.y=0;Invalidate();break; //PAGE-UP + CTRL
	}
	*/
	//if(funkcni_klavesa==1 && Key==36)
	//ShowMessage(Key);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(funkcni_klavesa==0)
	{
	switch(Akce)
  {            //mezerník
		case 1: if(Key==32){Akce=NIC; kurzor(standard);/*vrat_puvodni_akci();*/}break;//PAN
		case 2:
		{
			Akce=NIC; kurzor(standard);
			pan_move_map();
			//vrat_puvodni_akci();
		}
		break;//PAN_MOVE
    default: break;
  }
	}
  else
  {
		if(Akce==ZOOM_W){Akce=NIC;ZOOM_WINDOW();}//ukončí mod při puštění ctrl
		funkcni_klavesa=0;
		//vrat_puvodni_akci();
	}
}
//---------------------------------------------------------------------------
//explicitní klávesové zkratky
void __fastcall TForm1::FormShortCut(TWMKey &Msg, bool &Handled)
{
	//if (Msg.CharCode==VK_F9)AnalyzaClick(this);
	//prozatim jen pro účely vývoje
	 /*	if (Msg.CharCode==VK_F11)
		{
			Memo1->Visible=true;
			Button9->Visible=true;
			Button5->Visible=true;
		}
		if (Msg.CharCode==VK_F12)
		{
			Memo1->Visible=false;
			Button9->Visible=false;
			Button5->Visible=false;
		}*/
}
//---------------------------------------------------------------------------
//zatím nelze používat, protože se metody na odchytávání točení kolečka volají z metod točení kolečka nad knihovnou (RzDrawGrid)
//void __fastcall TForm1::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
//					TPoint &MousePos, bool &Handled)
//{
//	short int smer_posunu=WheelDelta/abs(WheelDelta);// zjištění směru točení kolečkem myši
//
//	if(liche_otoceni_koleckem_mysi)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
//	{
//	 liche_otoceni_koleckem_mysi=false;
//
//		switch(funkcni_klavesa)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detekuje stisk kláves ctrl, shift, ctrl+shift
//		{
//			case 1: break;
//			case 2: break;
//			case 3: break;
//			default: if(smer_posunu==1)UP(); else DOWN(); break;//vertikální posun
//		}
//		duvod_k_ulozeni=true;
//	}
//	else liche_otoceni_koleckem_mysi=true;
//}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimerMouseWheelTimer(TObject *Sender)
{
	 if(++doba_neotaceni_mysi==2)
	 {
		TimerMouseWheel->Enabled=false;
		jedno_ze_tri_otoceni_koleckem_mysi=1;
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
					bool &Handled)
{
	if(jedno_ze_tri_otoceni_koleckem_mysi==1)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
	{
			switch(funkcni_klavesa)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detekuje stisk kláves ctrl, shift, ctrl+shift
			{
				case 0://kolečko nahoru = směr nahoru
				{
					DOWN();
				}
				break;

				case 1://kolečko nahoru + CTRL = ZOOM IN
				if(Shift.Contains(ssCtrl))//nutná pojistka
				{
					ZOOM_IN();
				}
				break;

				case 2://kolečko nahoru + SHIFT = doleva
				if(Shift.Contains(ssShift))//nutná pojistka
				{
					LEFT();
				}
				break;

				case 3:break;
				default:break;
			}
	}
	jedno_ze_tri_otoceni_koleckem_mysi++;
	if(jedno_ze_tri_otoceni_koleckem_mysi>4)jedno_ze_tri_otoceni_koleckem_mysi=1;
	doba_neotaceni_mysi=0;TimerMouseWheel->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
					bool &Handled)
{
	if(jedno_ze_tri_otoceni_koleckem_mysi==1)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
	{
			switch(funkcni_klavesa)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detekuje stisk kláves ctrl, shift, ctrl+shift
			{
				case 0://kolečko dolu = směr dolu
				{
					UP();
				}
				break;

				case 1://kolečko dolu + CTRL = ZOOM OUT      //jenom pokud je dokončeno stahování
				if(Shift.Contains(ssCtrl))//nutná pojistka
				{
				 ZOOM_OUT();
				}
				break;

				case 2://kolečko dolu + SHIFT = doprava
				if(Shift.Contains(ssShift))//nutná pojistka
				{
					RIGHT();
				}
				break;

				case 3:break;
				default:break;
			}
	}
	jedno_ze_tri_otoceni_koleckem_mysi++;
	if(jedno_ze_tri_otoceni_koleckem_mysi>4)jedno_ze_tri_otoceni_koleckem_mysi=1;
	doba_neotaceni_mysi=0;TimerMouseWheel->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)//pokud je oteřeno hamburger menu a klikne se do plochy tak se nejdříve zavře
	{
		scSplitView_MENU->Opened=false;
		scSplitView_OPTIONS->Opened=false;
	}
	else
	{
		if(!dblClick)//v případě, že se nejedná o double click
		{
			if(Button==mbLeft/* && MOD!=REZERVY*/)//je stisknuto levé tlačítko myši
			{
					stisknute_leve_tlacitko_mysi=true;
					vychozi_souradnice_kurzoru=TPoint(X,Y);//výchozí souřadnice

					//aktivuje POSUN OBJEKTU, ELEMENTU či TABULKY,pokud je kliknuto v místě objektu (v jeho vnitřku)
					if(Akce==NIC && posun_objektu==false && funkcni_klavesa==0)//pokud není aktivovaná jiná akce
					{
						if(MOD==NAHLED && pom_temp!=NULL)//TABULKA či ELEMENT
						{
								if(JID==-1){Akce=PAN;pan_non_locked=true;}//pouze posun obrazu, protože v aktuálním místě pozici myši se nenachází vektor ani interaktivní text
								if(JID==0){Akce=MOVE_ELEMENT;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//ELEMENT posun
								if(JID==100 || 1000<=JID && JID<2000){Akce=MOVE_TABLE;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//TABULKA posun
								if(100<JID && JID<1000){redesign_element();}//nultý sloupec tabulky, libovolný řádek, přepnutí jednotek
						}
						else
						{
							if(MOD==SCHEMA)//OBJEKT
							{
								pom=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.V_width,d.V_width,VyID);//šlo by nahradit, kruhovým regionem, což by bylo exaktnější
								if(pom==NULL)//akcelerátor,aby se následně nehledalo znovu, pokud byla nalezena výhybka
								pom=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
								if(pom!=NULL){Akce=MOVE;kurzor(posun_l);posun_objektu=true;minule_souradnice_kurzoru=TPoint(X,Y);}
								else {Akce=PAN;pan_non_locked=true;}//přímo dovolení PAN pokud se neposová objekt = Rosťova prosba
							}
							else {Akce=PAN;pan_non_locked=true;}//přímo dovolení PAN pokud se neposová objekt   = Rosťova prosba
						}
					}

					if(funkcni_klavesa==1 || Akce==ZOOM_W_MENU)Akce=ZOOM_W;
					///Akce=PAN; pouze pokud bych nechtěl pomocí mezerníku aktivovat PAN
					switch(Akce)
					{
						case PAN:
						{
							if(pom!=NULL)d.vykresli_mGridy();//slouží k aktualizaci gridu při přesouvání obrazu
							kurzor(pan_move);Akce=PAN_MOVE;//přepne z PAN na PAN_MOVE
							int W=scSplitView_LEFTTOOLBAR->Width;
							if(MOD==CASOVAOSA || MOD==TECHNOPROCESY)W=0;//zajistí, že se posová i číslování vozíků resp.celá oblast
							short H=scGPPanel_mainmenu->Height;// zmena designu RzToolbar1->Height;
							int Gh=vrat_max_vysku_grafu();
							Pan_bmp->Width=ClientWidth;Pan_bmp->Height=ClientHeight-H-Gh;//velikost pan plochy, bylo to ještě +10
							Pan_bmp->Canvas->CopyRect(Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh));//uloží pan výřez
							//Pan_bmp->SaveToFile("test.bmp");  //pro testovací účely
							break;
						}
						case ZOOM_W:
						{
							kurzor(window);
							predchozi_souradnice_kurzoru=vychozi_souradnice_kurzoru;
							break;
						}
						case MOVE: d.odznac_oznac_objekt(Canvas,pom,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y); break;
						case MEASURE:minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;break;
						case KALIBRACE:
						{
							minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;
							//+změnit kurzor
						break;
						}
						case ADJUSTACE:minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;break;
						default: break;
					}
					DuvodUlozit(true);
			}
			//POPUP menu
			else//pokud není stisknuto levé tlačítko, předpokládá se volání pop-up menu
			{
					//nejdříve deaktiviace zaměřovače, je-li aktivní
					deaktivace_zamerovace();
					//nastavení zobrazení popUPmenu a jeho volání včetně pozice
					onPopUP(X,Y);
			}
		}
		else dblClick=false;
	}
}
//---------------------------------------------------------------------------
//při double clicku volá přímo formulář parametry objektu
void __fastcall TForm1::FormDblClick(TObject *Sender)
{
	dblClick=true;Akce=NIC;
	long X=akt_souradnice_kurzoru_PX.x;long Y=akt_souradnice_kurzoru_PX.y;//pouze zkrácení zápisu
	switch(MOD)
	{
		case CASOVAOSA:
		{														 //min                      //vozik
			proces_pom=d.v.najdi_proces((X+d.PosunT.x)/d.PX2MIN*60,ceil((Y+d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0)));//vrací nalezen proces, proces_pom se využívá ještě dále
			if(proces_pom!=NULL && !d.mod_vytizenost_objektu)
			{
				if(STATUS==NAVRH)//měnit parametry na časových osách je možné pouze v návrháři/architektovi
				{
					pom=proces_pom->segment_cesty->objekt;
				}
			}
		}break;
		case TECHNOPROCESY:
		{
			pom=d.v.vrat_objekt_z_roma(akt_souradnice_kurzoru_PX.x-d.Xofset+d.PosunT.x);
		}
		break;
		case SIMULACE:break;
		default://pro SCHEMA
		{
			//povoluje nastavení položek kopírování či smazání objektu
			pom=d.v.najdi_objekt(m.P2Lx(X),m.P2Ly(Y),d.O_width,d.O_height);
		}break;
	}
	if(pom!=NULL)
	{
		NP();//dřívější volání nastavparametry1click
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	vyska_menu=Mouse->CursorPos.y-Y;//uchová rozdíl myšího kurzoru a Y-pixelu v pracovní oblasti

	akt_souradnice_kurzoru_PX=TPoint(X,Y);
	akt_souradnice_kurzoru=m.P2L(akt_souradnice_kurzoru_PX);

	if(MOD==CASOVAOSA)//vykreslování posuvné (dle myši) svislice kolmé na osy procesů, slouží jakou ukázovatko času na ose
	{ //nový přístup v zobrazování svislic, jen v momentu zobrazování labalu_zamerovac (if zde nebyl)
		if(Label_zamerovac->Visible)d.vykresli_svislici_na_casove_osy(Canvas,minule_souradnice_kurzoru.X,minule_souradnice_kurzoru.Y);
		minule_souradnice_kurzoru=TPoint(X,Y);
		//d.vykresli_svislici_na_casove_osy(Canvas,X,Y);//nový přístup v zobrazování svislic, jen v momentu zobrazování labalu_zamerovac (bylo odkomentováno)
		SB(UnicodeString((X+d.PosunT.x)/d.PX2MIN)+" min",6);//výpis času na ose procesů dle kurzoru
		//hazí stejné souřadnice if(abs((int)minule_souradnice_kurzoru.x-(int)akt_souradnice_kurzoru_PX.x)>1 && abs((int)minule_souradnice_kurzoru.y-(int)akt_souradnice_kurzoru_PX.y)>1)//pokud je změna větší než jeden pixel, pouze ošetření proti divnému chování myši (možná mi docházela baterka, s myší jsem nehýbal, ale přesto docházele k rušení labelu resp. volání metody FormMouseMove)
		Label_zamerovac->Visible=false;pocitadlo_doby_neaktivity=0;//deaktivace_zamerovace();nelze
		Timer_neaktivity->Enabled=false;
		Timer_neaktivity->Interval=1000;
		if(scSplitView_OPTIONS->Opened==false && scSplitView_MENU->Opened==false && PopUPmenu->Showing==false && Form_parametry_linky->Showing==false && Form_definice_zakazek->Showing==false && Form_osa_info->Showing==false)Timer_neaktivity->Enabled=true;//spoustí pouze pokud nejsou zobrazeny formuláře z podmínky
	}
	else //výpis metrických souřadnic
	{
		if(Zoom<1)SB(UnicodeString(m.round(m.P2Lx(X)))+";"+UnicodeString(m.round(m.P2Ly(Y))),3);
		else SB(FloatToStrF(m.P2Lx(X),ffFixed,10,1)+";"+FloatToStrF(m.P2Ly(Y),ffFixed,10,1),3);
		//vypsat fyzické souřadniceSB(UnicodeString(X)+";"+UnicodeString(Y));
	}

	switch(Akce)
	{
		case PAN_MOVE:
		{
			pan_map(Canvas,X,Y);
			break;
		}
		case ZOOM_W://ZOOM_WINDOW
		{
			d.editacni_okno(Canvas, vychozi_souradnice_kurzoru,predchozi_souradnice_kurzoru,5,(TColor)RGB(0,128,192));//vymaže předchozí okno
			predchozi_souradnice_kurzoru=akt_souradnice_kurzoru_PX;//uloží souřadnice pro další vymazání
			d.editacni_okno(Canvas, vychozi_souradnice_kurzoru,akt_souradnice_kurzoru_PX,5,(TColor)RGB(0,128,192));//nakreslí nové okno
      break;
		}
		case ADD://přídávání objektu či elementu, posun navigačních linii
		{        //algoritmy v tomto CASE (včetně dílčích algoritmu) by bylo možné sloučit, ale bylo by to dost práce navíc...
			if(MOD!=NAHLED)
			{
				if(probehl_zoom==false)//ošetření proti nežádoucímu chování po zoomu
				{
					if(d.v.OBJEKTY->predchozi->n>=2)//pokud už existují alespoň dva prvky, jinak nemá smysl
					{
						Cvektory::TObjekt *p=add_objekt_za();//testuje zda se bude přidávat objekt mimo situace poslední-první objekt-prvek
						if(p!=NULL && p!=pom)//byl nalezen meziprostor k přidávání, porovnám tedy jestli se jedná o nový
						{
							if(add_posledni)d.odznac_oznac_objekt_novy_posledni(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y);
							else if(pom!=NULL)d.odznac_oznac_objekt_novy(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
							zneplatnit_minulesouradnice();pom=p;add_posledni=false;
						}          // p==NULL a  || d.v.OBJEKTY->predchozi->n==2 zajišťuje možnost přídání i mezi dva prvky nebo přídání za dva prvky standardně
						if(p==NULL && (d.lezi_v_pasmu_poslednim(Canvas,X,Y) || d.v.OBJEKTY->predchozi->n==2))//kurzor se nenachází v prostoru mezi prkvy, je tedy možné přidávat mezi poslední a první prvek, tedy na konec spojového seznamu
						{
							if(add_posledni)d.odznac_oznac_objekt_novy_posledni(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y);
							else if(pom!=NULL)d.odznac_oznac_objekt_novy(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
							zneplatnit_minulesouradnice();add_posledni=true;pom=NULL;
						}
					}
					if(!add_posledni)//pro situaci přidávání mezi prvky
					{
						d.odznac_oznac_objekt_novy(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
						minule_souradnice_kurzoru=TPoint(X,Y);
						d.odznac_oznac_objekt_novy(Canvas,X,Y,pom);
					}
					else//pro situaci poslední první objekt, standardní přidávání nakonec
					{
						d.odznac_oznac_objekt_novy_posledni(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y);
						minule_souradnice_kurzoru=TPoint(X,Y);
						d.odznac_oznac_objekt_novy_posledni(Canvas,X,Y);
					}
				}
				probehl_zoom=false;
			}
			if(MOD==NAHLED)
			{
				//--prozatim
				//rotace dle umístění na ose Y
				short rotace_symbolu=0;
				//if((ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2.0>Y){rotace_symbolu=180;}
				rotace_symbolu=rotace_symbol(m.Rt90(d.trend(pom)),X,Y);
				//--
				d.vykresli_element(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"","",element_id,-1,Rotace_symbolu_minula);
				minule_souradnice_kurzoru=TPoint(X,Y);
				d.vykresli_element(Canvas,X,Y,"","",element_id,-1,rotace_symbolu);
				Rotace_symbolu_minula=rotace_symbolu;
			}
			break;
		}
		case MOVE://posun objektu
		{
			if(posun_objektu)
			{
				d.odznac_oznac_objekt(Canvas,pom,minule_souradnice_kurzoru.x-vychozi_souradnice_kurzoru.x,minule_souradnice_kurzoru.y-vychozi_souradnice_kurzoru.y);
				minule_souradnice_kurzoru=TPoint(X,Y);
				d.odznac_oznac_objekt(Canvas,pom,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y);
			}
			break;
		}
		case MOVE_TABLE://posun tabulky elementu
		{
			pom_element->Xt+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
			pom_element->Yt+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
			minule_souradnice_kurzoru=TPoint(X,Y);
			REFRESH();
			d.linie(Canvas,m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt),m.L2Py(pom_element->Yt),2,(TColor)RGB(200,200,200));//vykreslí provizorní spojovací linii mezi elementem a tabulkou při posouvání, kvůli znázornění příslušnosti
			break;
		}
		case MOVE_ELEMENT://posun elementu
		{
			if (pom_element->rotace_symbolu==0 || pom_element->rotace_symbolu==180)
				pom_element->X+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
			else
				pom_element->Y+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
			minule_souradnice_kurzoru=TPoint(X,Y);
			REFRESH();
			d.linie(Canvas,m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt),m.L2Py(pom_element->Yt),2,(TColor)RGB(200,200,200));//vykreslí provizorní spojovací linii mezi elementem a tabulkou při posouvání, kvůli znázornění příslušnosti
			//zatím jen pro posun po ose x
			if (pom_element->X<pom_temp->Xk||pom_element->X>pom_temp->Xk+pom_temp->rozmer_kabiny.x)
				Smazat1Click(Sender);
			break;
		}
		case VYH://přidávání vyhýbky
		{
			d.odznac_oznac_vetev(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom_vyhybka);
			minule_souradnice_kurzoru=TPoint(X,Y);
			d.odznac_oznac_vetev(Canvas,X,Y,pom_vyhybka);
			break;
		}
		case MEASURE://liniové měření vzdálenosti,vykreslení provizorní měřící linie
		{
			if(stisknute_leve_tlacitko_mysi)
			{
				d.vykresli_meridlo(Canvas,X,Y);
			}
		}break;
		case KALIBRACE:
		{   //zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
    zobraz_tip("Tažením myši z vybraného bodu na podkladu směřujte do vybraného technolog. objektu, po puštění myši dojde ke kalibraci obrazu.");
			if(stisknute_leve_tlacitko_mysi)
			{
				d.vykresli_meridlo(Canvas,X,Y,true);
			}
		}break;
		case ADJUSTACE:
		{ //zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
      zobraz_tip("Tažením myši z výchozího bodu směřujte do cílového bodu podkladu, po puštění myši zadejte skutečnou vzdálenost mezi body v metrech či milimetrech.");
			if(stisknute_leve_tlacitko_mysi)
			{
				d.vykresli_meridlo(Canvas,X,Y);
			}
		}break;
		case NIC://přejíždění po ploše aplikace, bez aktuálně nastavené akce
		{
			if(MOD!=CASOVAOSA)zneplatnit_minulesouradnice();
			if(MOD==NAHLED && pom_temp!=NULL)
			{
				pocitadlo_doby_neaktivity=0; Timer_neaktivity->Interval=20;
				Timer_neaktivity->Enabled=true;//volá se zpožděním kvůli optimalizaci getJobID(X,Y);
			}
			//algoritmus na ověřování zda se kurzor nachází na objektem (a může být tedy povoleno v pop-up menu zobrazení volby nastavit parametry) přesunut do metody mousedownclick, zde se to zbytečně volalo při každém posunu myši
			break;
		}
		default: break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
	 if(Button==mbLeft)//zohlední jenom stisk levého tlačítka myši
	 {
			if(pan_non_locked){pan_non_locked=false;Akce=NIC; kurzor(standard);pan_move_map();}
    	switch(Akce)
			{
				case PAN_MOVE://vratí z PAN_MOVE na PAN
				{
					kurzor(pan); Akce=PAN;
					pan_move_map();
					break;
				}
				case ZOOM_W:ZOOM_WINDOW();break;//ZOOM_WINDOW
				case ADD://přidání objekt či elementu
				{
					if(MOD==SCHEMA)add_objekt(X,Y);//přídání objektu v modu SCHEMA
					else {d.vykresli_element(Canvas,X,Y,"","",element_id,-1);add_element(X,Y);}//přídání elementu v modu NAHLED
					zneplatnit_minulesouradnice();
					break;
				}
				case VYH:Akce=ADD;add_objekt(X,Y);zneplatnit_minulesouradnice();break;//přidání objekt
				case MOVE:move_objekt(X,Y);break;//posun objektu
				case MOVE_TABLE:Akce=NIC;kurzor(standard);REFRESH();break;//posun tabulky elementu
				case MOVE_ELEMENT:Akce=NIC;kurzor(standard);REFRESH();break;//posun elementu
				case MEASURE:
				{
					double delka=m.delka(m.P2Lx(vychozi_souradnice_kurzoru.X),m.P2Ly(vychozi_souradnice_kurzoru.Y),m.P2Lx(X),m.P2Ly(Y));
					MB(AnsiString(delka)+" [metrů]");
					Akce=NIC;kurzor(standard);
					Invalidate();
					break;
				}
				case KALIBRACE:
				{
					d.v.PP.raster.X=m.P2Lx(X)+d.v.PP.raster.X-m.P2Lx(vychozi_souradnice_kurzoru.X);
					d.v.PP.raster.Y=m.P2Ly(Y)+d.v.PP.raster.Y-m.P2Ly(vychozi_souradnice_kurzoru.Y);
					Akce=NIC;kurzor(standard);
					Invalidate();
					break;
				}
				case ADJUSTACE:
				{
          if(Form_adjustace->ShowModal()==mrOk)//MB "Zadejte vzdálenost v metrech"
		      {

            double vzdalenost=Form_adjustace->scGPNumericEdit_vzdalenost->Value/(1+999.0*Form_adjustace->Delkaunit);
						d.v.PP.raster.resolution=m.getResolution(vychozi_souradnice_kurzoru.X,vychozi_souradnice_kurzoru.Y,X,Y,vzdalenost);
          }
          else
          {
            d.v.PP.raster.show=false;
            d.v.PP.raster.filename="";
          }
					Akce=NIC;kurzor(standard);
					Invalidate();
					break;
				}
				default: break;
			}
	 }
	 stisknute_leve_tlacitko_mysi=false;

	 //vrat_puvodni_akci();
	 /*if(X<=RzSizePanel_knihovna_objektu->Width) DrawGrid_knihovna->Enabled=true;
	 else DrawGrid_knihovna->Enabled=false;*/
}
//---------------------------------------------------------------------------
//vrátí do globální proměnné JID ID úlohy/funkcionality v místě kurzoru, -1 žádná, 0 - 9 rezervováno pro element, 10 - 99 - interaktivní text kóty, 100- a výše rezervováno pro tabuku, kde 100 znamená nultý řádek, zároveň pokud bylo kliknuto na tabulku či element nahraje ukazatel do globální proměnné pom_element
void TForm1::getJobID(int X, int Y)
{
	JID=-1;//výchozí stav, nic nenalezeno
	//nejdříve testování zda se nepřejelo myší přes obrys kabiny
						//roteč linií  //šířka linie
	short Ov=Zoom*0.4+F->Zoom*0.2/2.0;
	if
	(
		m.L2Px(pom_temp->Xk)-Ov<=X && X<=m.L2Px(pom_temp->Xk)+Ov && m.L2Py(pom_temp->Yk)-Ov<=Y && Y<=m.L2Py(pom_temp->Yk-pom_temp->rozmer_kabiny.y)+Ov ||//svislá levá
		m.L2Px(pom_temp->Xk+pom_temp->rozmer_kabiny.x)-Ov<=X && X<=m.L2Px(pom_temp->Xk+pom_temp->rozmer_kabiny.x)+Ov && m.L2Py(pom_temp->Yk)-Ov<=Y && Y<=m.L2Py(pom_temp->Yk-pom_temp->rozmer_kabiny.y)+Ov ||//svislá pravá
		m.L2Px(pom_temp->Xk)-Ov<=X && X<=m.L2Px(pom_temp->Xk+pom_temp->rozmer_kabiny.x) && m.L2Py(pom_temp->Yk)-Ov<=Y && m.L2Py(pom_temp->Yk)+Ov>=Y ||//vodorovná horní
		m.L2Px(pom_temp->Xk)-Ov<=X && X<=m.L2Px(pom_temp->Xk+pom_temp->rozmer_kabiny.x) && m.L2Py(pom_temp->Yk-pom_temp->rozmer_kabiny.y)-Ov<=Y && m.L2Py(pom_temp->Yk-pom_temp->rozmer_kabiny.y)+Ov>=Y//vodorovná dolní
	)JID=-2;
	else//dále tabulky či elementy
	{
		pom_element=F->d.v.najdi_tabulku(pom_temp,m.P2Lx(X),m.P2Ly(Y));//TABULKA
		if(pom_element!=NULL)//tabulka nalezena, tzn. klik či přejetí myší přes tabulku
		{
			int IdxRow=pom_element->mGrid->GetIdxRow(X,Y);
			if(IdxRow==0)JID=100+0;//hlavička
			if(IdxRow>0)//nějaký z řádků mimo nultého tj. hlavičky, nelze použít else, protože IdxRow -1 bude také možný výsledek
			{
				int IdxCol=pom_element->mGrid->GetIdxColum(X,Y);
				if(IdxCol==0)//řádky v prvním sloupeci
				{
					if(pom_element->mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxRow;//na daném řádku a daných myších souřadnicích se nachází odkaz
					else JID=1000+IdxRow;
				}
				else JID=2000+IdxRow;//řádky v dalších sloupcích
			}
		}
		else //tabulka nenalezena, takže zkouší najít ELEMENT
		{
			pom_element=F->d.v.najdi_element(pom_temp,m.P2Lx(X),m.P2Ly(Y));
			if(pom_element!=NULL)//element nalezen, tzn. klik či přejetí myší přes elemement nikoliv tabulku
			{
				JID=0;
			}
			else //ani element nenalezen, hledá tedy interaktivní TEXT, např. kóty atp.
			{
				//if()RET=10-99 zcela doplnit
			}
		}
	}
	Memo3->Visible=true;
	Memo3->Lines->Add(s_mazat++);
}
//---------------------------------------------------------------------------
//dle místa kurzoru a vrácené JID (job id) nastaví úlohu
void TForm1::setJobIDOnMouseMove(int X, int Y)
{
	kurzor(standard);//umístít na začátek
	if(pom_element!=NULL)//ODSTRANĚNÍ předchozí případného highlightnutí buď tabulky, elementu či odkazu
	{
		if(pom_element->mGrid!=NULL)pom_element->mGrid->HighlightTable((TColor)RGB(200,200,200),2,0);//TABULKA
		if(pom_element->mGrid!=NULL && 100<JID && JID<1000)pom_element->mGrid->HighlightLink(0,JID-100,0);//ODKAZ v TABULCE
		if(JID==0){pom_element->stav=1;}//ELEMENT
	}
	int puvJID=JID;
	Cvektory::TElement *pom_element_puv=pom_element;//pouze ošetření, aby neproblikával mGrid elementu, při přejíždění přes element, možno odstranit, až se mGrid bude posílat do celkové bitmapy
	getJobID(X,Y);
	if(JID==0){kurzor(posun_ind);pom_element->stav=2;}//ELEMENT
	if((puvJID!=JID || pom_element!=pom_element_puv) && (puvJID==0 || JID==0)){REFRESH();}//důvod k REFRESH, pouze v případě změny elementu
	if(JID==100 || 1000<=JID && JID<2000){kurzor(posun_ind);if(pom_element->mGrid!=NULL)pom_element->mGrid->HighlightTable(m.clIntensive(pom_element->mGrid->Border.Color,-50),2,0);}//indikace posunutí TABULKY
	if(100<JID && JID<1000){kurzor(zmena_j);pom_element->mGrid->HighlightLink(0,JID-100,-50);}//první sloupec tabulky, libovolný řádek, v místě, kde je ODKAZ
	pom_element_puv=NULL;delete pom_element_puv;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//deaktivuje zaměřovač label a svislice a kolmice
void TForm1::deaktivace_zamerovace()
{
	if(Label_zamerovac->Visible)
	{
		 d.vykresli_svislici_na_casove_osy(Canvas,minule_souradnice_kurzoru.X,minule_souradnice_kurzoru.Y);
		 Label_zamerovac->Visible=false;pocitadlo_doby_neaktivity=0;Timer_neaktivity->Enabled=false;//monitoruje dobu nečinosti od znovu
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//nastavení zobrazení popUPmenu a jeho volání včetně pozice
void TForm1::onPopUP(int X, int Y)
{
	//výchozí skrytí všech položek, další postup je založen na postupném odkrývání a zvětšování panelu UP nebo DOWN
	close_all_items_popUPmenu();
	PopUPmenu->Item_zobrazit_parametry->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
	//dle statusu Architek x Klient resp. Návrh x Ověrování
	AnsiString N="Nastavit";if(STATUS==OVEROVANI)N="Zobrazit";
	//dle modu zobrazí položky, pozor záleží zvláštně!!! na pořadí položek
	switch(MOD)
	{
		case CASOVAOSA:
		{														 //min                      //vozik
			proces_pom=d.v.najdi_proces((X+d.PosunT.x)/d.PX2MIN*60,ceil((Y+d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0)));//vrací nalezen proces, proces_pom se využívá ještě dále
			PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			if(proces_pom!=NULL && !d.mod_vytizenost_objektu)
			{
				PopUPmenu->Item_zobrazit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;//nastavení zobrazení
				PopUPmenu->Item_zobrazit_parametry->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
				PopUPmenu->Item_rychly_export->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				if(STATUS==NAVRH)//měnit parametry na časových osách je možné pouze v návrháři/architektovi
				{
					pom=proces_pom->segment_cesty->objekt;
					if(AnsiString(N+" "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom->name.UpperCase();
					else
					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom->name.UpperCase();
					PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				}
			}
		}break;
		case TECHNOPROCESY:
		{
			pom=d.v.vrat_objekt_z_roma(akt_souradnice_kurzoru_PX.x-d.Xofset+d.PosunT.x);
			if(pom!=NULL)
			{
				PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
				PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
				//PopUPmenu->Item_rychly_export->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				if(AnsiString(N+" "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
				PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom->name.UpperCase();
				else
				PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom->name.UpperCase();
				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
		}
		break;
		case SIMULACE:break;
		case NAHLED:
		{

			pom_element_smazat=pom_element;
			if (pom_element_smazat!=NULL)//Pokud bylo kliknuto na element
			{
				if(AnsiString("Nastavit "+pom_element->name).Length()>19) PopUPmenu->scLabel_smazat->Caption="  Smazat\n  "+pom_element->name.UpperCase();
				else PopUPmenu->scLabel_smazat->Caption="  Smazat "+pom_element->name.UpperCase();
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			else
			{
			PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_priblizit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_oddalit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_vybrat_oknem->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_cely_pohled->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			}
			break;
		}
		default://pro SCHEMA
		{
			//povoluje nastavení položek kopírování či smazání objektu
			pom=d.v.najdi_objekt(m.P2Lx(X),m.P2Ly(Y),d.O_width,d.O_height);
			if(pom!=NULL)// nelze volat přímo metodu najdi objekt, protože pom se používá dále
			{
				if(AnsiString("Nastavit "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
				{
					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom->name.UpperCase();
					PopUPmenu->scLabel_kopirovat->Caption="  Kopie\n  "+pom->name.UpperCase();
					PopUPmenu->scLabel_smazat->Caption="  Smazat\n  "+pom->name.UpperCase();
				}
				else
				{
					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom->name.UpperCase();
					PopUPmenu->scLabel_kopirovat->Caption="  Kopie "+pom->name.UpperCase();
					PopUPmenu->scLabel_smazat->Caption="  Smazat "+pom->name.UpperCase();
				}
				//pozor rozhoduje pořadí
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				PopUPmenu->Item_kopirovat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			//zobrazení běžných položek, pozor rozhoduje pořadí
			PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_priblizit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_oddalit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_vybrat_oknem->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_cely_pohled->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
		}break;
	}
	//horní panel nahoře - nutnost
	PopUPmenu->Panel_UP->Top=0;
	//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje
	//!!!případně začlenit další položky, které budou zlobit
	//PopUPmenu->Item_zobrazit_parametry->FillColor=(TColor)RGB(240,240,240); to musí být zařazeno výše
	PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
	PopUPmenu->Item_cely_pohled->FillColor=(TColor)RGB(240,240,240);
	//umístění popup menu
	PopUPmenu->Left=akt_souradnice_kurzoru_PX.x;
	PopUPmenu->Top=akt_souradnice_kurzoru_PX.y;
	//volání vlastního popup menu + ošetření, pokud je mimo obrazovku
	PopUPmenu->Show();
}
//---------------------------------------------------------------------------
//zajistí skrýtí všech položek popUPmenu
void TForm1::close_all_items_popUPmenu()
{
	PopUPmenu->Item_poznamka->Visible=false;
	PopUPmenu->Item_cely_pohled->Visible=false;
	PopUPmenu->Item_vybrat_oknem->Visible=false;
	PopUPmenu->Item_posunout->Visible=false;
	PopUPmenu->Item_posouvat->Visible=false;
	PopUPmenu->Item_oddalit->Visible=false;
	PopUPmenu->Item_priblizit->Visible=false;
	PopUPmenu->Item_nastavit_parametry->Visible=false;
	PopUPmenu->Item_zobrazit_parametry->Visible=false;
	PopUPmenu->Item_rychly_export->Visible=false;
	PopUPmenu->Item_smazat->Visible=false;
	PopUPmenu->Item_kopirovat->Visible=false;

	PopUPmenu->Panel_UP->Height=0;
	PopUPmenu->Panel_DOWN->Height=0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Piblit1Click(TObject *Sender)
{
	ZOOM_IN();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Priblizit2Click(TObject *Sender)
{
	ZOOM_IN();
}
void __fastcall TForm1::RzToolButton8Click(TObject *Sender)//Zoom in z toolbaru
{
 vycentrovat=false;
 akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
 ZOOM_IN();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Oddlit1Click(TObject *Sender)
{
	ZOOM_OUT();
}
void __fastcall TForm1::Oddalit2Click(TObject *Sender)
{
	ZOOM_OUT();
}
void __fastcall TForm1::RzToolButton9Click(TObject *Sender)//Zoom out z toolbaru
{
	vycentrovat=false;
	akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
	ZOOM_OUT();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//přiblížení
void TForm1::ZOOM_IN()
{
		if(Zoom==0.25)//při odchodu z 0.25
		{
				Uloz_predchozi_pohled();
				Zoom=0.5;
				ZOOM();
		}
		else
		if(Zoom<10) //max
		{
			Uloz_predchozi_pohled();
      if(Zoom>=5) Zoom+=1.0;
			else Zoom+=0.5;
			ZOOM();
		}
}
//---------------------------------------------------------------------------
//oddálení
void TForm1::ZOOM_OUT()
{
		if(Zoom>0.5)
		{
			Uloz_predchozi_pohled();
      if(Zoom>=5) Zoom-=1.0;
			else Zoom-=0.5;
			ZOOM();
		}
		else
		{
			if(Zoom==0.5)//pro přechod z 0.5 na 0.25
			{
				Uloz_predchozi_pohled();
				Zoom=0.25;
				ZOOM();
			}
		}

}
//---------------------------------------------------------------------------
//samotný ZOOM
void TForm1::ZOOM()
{
	if(!nahled_objektu)//uchovává stav, zda se jedná o náhled objekt či regulerní zobrazení ve form1, zajišťuje, aby se zbytečně nepřekresloval obraz
	{
		probehl_zoom=true;
		zneplatnit_minulesouradnice();

		//je to nějaké nepřesné
		Posun.x=m.round(akt_souradnice_kurzoru.x/m2px-(ClientWidth+scSplitView_LEFTTOOLBAR->Width)/2/Zoom);
		Posun.y=m.round(-akt_souradnice_kurzoru.y/m2px-(ClientHeight)/2/Zoom);
		//SB(Zoom,2); už se nepoužívá
		on_change_zoom_change_scGPTrackBar();

		//vycentruje kurzor na střed monitoru - na X nefunguje přesně
		//Mouse->CursorPos=TPoint(ClientWidth/2,(ClientHeight-RzStatusBar1->Height)/2); - špatně
		if(vycentrovat)Mouse->CursorPos=TPoint(m.L2Px(akt_souradnice_kurzoru.x),m.L2Py(akt_souradnice_kurzoru.y)+vyska_menu);
		vycentrovat=true;
		REFRESH();
		DuvodUlozit(true);
	}
}
//---------------------------------------------------------------------------
void TForm1::ZOOM_WINDOW()
{
	Uloz_predchozi_pohled();
	Akce=NIC;//zrušení pokračování akce, tzn. konec překreslování rozšiřování editačního okna

	//zapamatovani si středu obrazovky dle vybrané oblasti (resp) jejího středu
	TPoint Centr;
	Centr.x=(m.P2Lx(vychozi_souradnice_kurzoru.x)+m.P2Lx(akt_souradnice_kurzoru_PX.x))/2;
	Centr.y=(m.P2Ly(vychozi_souradnice_kurzoru.y)+m.P2Ly(akt_souradnice_kurzoru_PX.y))/2;


	//zjištění (vyzkoušení) velikosti zoomu
	float LHx=m.P2Lx(vychozi_souradnice_kurzoru.x);float LHy=m.P2Ly(vychozi_souradnice_kurzoru.y);float PDx=m.P2Lx(akt_souradnice_kurzoru_PX.x);float PDy=m.P2Ly(akt_souradnice_kurzoru_PX.y);
	while(m.P2Lx(0)<=LHx && m.P2Ly(0)>=LHy && m.P2Lx(ClientWidth)>=PDx && m.P2Ly(ClientHeight)<=PDy)
	{
		if(Zoom>=4){Zoom=4;break;}
		Zoom+=0.5;
	}

	//vycentrování obrazu
	Posun.x=m.round(Centr.x/m2px-(ClientWidth+scSplitView_LEFTTOOLBAR->Width)/2/Zoom);
	Posun.y=m.round(-Centr.y/m2px-(ClientHeight)/2/Zoom);
	//SB(Zoom,2);už se nepoužívá
	on_change_zoom_change_scGPTrackBar();

	REFRESH();
	//aktualizace_statusbaru(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
	//checkZoomMenuItems();//povolení či zakázní zoomu
	kurzor(standard);
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Predchozipohled1Click(TObject *Sender)
{
	Zoom=Zoom_predchozi;
	Posun=Posun_predchozi;
	//Predchozipohled1->Enabled=false;
	RzToolButton12->Enabled=false;
	REFRESH();
	DuvodUlozit(true);
}
void TForm1::Uloz_predchozi_pohled()
{
	Zoom_predchozi=Zoom;
	Posun_predchozi=Posun;
	//Predchozipohled1->Enabled=true;
	RzToolButton12->Enabled=true;
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void TForm1::DOWN()//smer dolu
{
		probehl_zoom=true;
		Uloz_predchozi_pohled();
		if(MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)
		{
			Posun.y-=m.round(ClientWidth/(20*Zoom));//o Xtinu obrazu
			zneplatnit_minulesouradnice();
		}
		else
		{
			bool prekreslovat=false;if(d.PosunT.y!=0)prekreslovat=true;
			if(d.PosunT.y-d.KrokY<0){d.PosunT.y=0;if(prekreslovat)Invalidate();}
			else {d.PosunT.y-=d.KrokY;}
		}
		REFRESH();
		DuvodUlozit(true);
}
void TForm1::UP()//smer nahoru
{
		probehl_zoom=true;
		Uloz_predchozi_pohled();
		if(MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)
		{
			Posun.y+=m.round(ClientWidth/(20*Zoom));//o Xtinu obrazu
			zneplatnit_minulesouradnice();
		}
		else
		{
			d.PosunT.y+=d.KrokY;
		}
		REFRESH();
		DuvodUlozit(true);
}
void TForm1::RIGHT()//smer doprava
{
		probehl_zoom=true;
		Uloz_predchozi_pohled();
		if(MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)
		{
			Posun.x+=m.round(ClientWidth/(20*Zoom));//o Xtinu obrazu
			zneplatnit_minulesouradnice();
		}
		else
		{
			d.PosunT.x+=ClientWidth/3;
		}
		REFRESH();
		DuvodUlozit(true);
}
void TForm1::LEFT()//smer doleva
{
		probehl_zoom=true;
		Uloz_predchozi_pohled();
		if(MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)
		{
			Posun.x-=m.round(ClientWidth/(20*Zoom));//o Xtinu obrazu
			zneplatnit_minulesouradnice();
		}
		else
		{
			bool prekreslovat=false;if(d.PosunT.x!=0)prekreslovat=true;
			if(d.PosunT.x-ClientWidth/3<0){d.PosunT.x=0;if(prekreslovat)Invalidate();}
			else {d.PosunT.x-=ClientWidth/3;}
		}
		REFRESH();
		DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Posouvá výřez mapy při stisknutém mezerníku a L-myši
void TForm1::pan_map(TCanvas * canv, int X, int Y)
{
	 canv->Brush->Color=clWhite/*clDkGray*/;canv->Brush->Style=bsSolid;
	 //maže při posouvání obrazu starý obraz
	 canv->FillRect(TRect(0,0,/*X-kurzor_souradnice_puvodni.x+Pan_bmp->Width*/ClientWidth,Y-vychozi_souradnice_kurzoru.y));//horní okraj
	 canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height));//levy okraj
	 canv->FillRect(TRect(X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,Y-vychozi_souradnice_kurzoru.y,ClientWidth,ClientHeight));//pravy okraj
	 canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height,X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,ClientHeight));//dolní okraj
   //samotné posouvání Pan_bmp
	 canv->Draw(X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y,Pan_bmp);
	 //canv->Brush->Color=clWhite;canv->Brush->Style=bsSolid;
}
//---------------------------------------------------------------------------
//realizuje posunutí obrazu
void TForm1::pan_move_map()
{
	Uloz_predchozi_pohled();
	if(MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)
	{
		Posun.x-=(akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x)/Zoom;
		Posun.y-=(akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y)/Zoom;
	}
	else
	{
	 //osetrit zbytecne invalidovani
		if(d.PosunT.x-(akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x)<0)d.PosunT.x=0;
		else d.PosunT.x-=(akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x);
		if(d.PosunT.y-(akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y)<0)d.PosunT.y=0;
		else d.PosunT.y-=(akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y);
	}
	REFRESH();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Posouvat1Click(TObject *Sender)
{
	Akce=PAN;
	kurzor(pan);
	zobraz_tip("TIP: Posun obrazu lze také vykonat pomocí stisknutého levého tlačítka myší a posunem myši požadovaným směrem.");
}
void __fastcall TForm1::Posouvat2Click(TObject *Sender)
{
	Posouvat1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Posunout2Click(TObject *Sender)
{
	Akce=PAN;
  kurzor(pan);
	pan_non_locked=true;
	zobraz_tip("TIP: Posun obrazu lze také vykonat pomocí stisknutého mezerníku a posunem myši při stisknutém levém tlačítku.");
}
void __fastcall TForm1::Posunout3Click(TObject *Sender)
{
	Posunout2Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Vybratoknem1Click(TObject *Sender)
{
	kurzor(window);
	Akce=ZOOM_W_MENU;
	zobraz_tip("TIP: Přiblížení oknem je možné také realizovat pomocí stisknuté klávesy CTRL a výběrem požadované oblasti pomocí myši při stisknutém levém tlačítku.");
}
void __fastcall TForm1::Vybratoknem2Click(TObject *Sender)
{
	Vybratoknem1Click(Sender);
}
//---------------------------------------------------------------------------
//Celá "mapa"
void __fastcall TForm1::RzToolButton11Click(TObject *Sender)
{
	Uloz_predchozi_pohled();

	try
	{
	double MaxX=-5000000.0;double MaxY=-5000000.0;double MinX=5000000.0;double MinY=5000000.0;

	Cvektory::TObjekt *ukaz;
	ukaz=d.v.OBJEKTY->dalsi;//přeskočí hlavičku

	while (ukaz!=NULL)
	{
			MaxX=ukaz->X>MaxX?ukaz->X:MaxX;
			MaxY=ukaz->Y>MaxY?ukaz->Y:MaxY;
			MinX=ukaz->X<MinX?ukaz->X:MinX;
			MinY=ukaz->Y<MinY?ukaz->Y:MinY;
			ukaz=ukaz->dalsi;//posun na další prvek
	}

	int PD_x=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
	int PD_y=ClientHeight-vyska_menu-scGPPanel_statusbar->Height;//-vyska_menu-RzStatusBar1->Height je navíc nemá tam co dělat

	if((MaxX-MinX)!=0 && (MaxX+MinX)!=0)
	Zoom=
	abs(m.P2Lx(PD_x))/(MaxX-MinX)<abs(m.P2Ly(PD_y))/(MaxY-MinY)
	?abs(m.P2Lx(PD_x))/(MaxX-MinX):abs(m.P2Ly(PD_y))/(MaxY-MinY);
	else Zoom=1;


	/*Memo1->Lines->Add(MinX);Memo1->Lines->Add(MinY);
	Memo1->Lines->Add(MaxX);Memo1->Lines->Add(MaxY);
	Memo1->Lines->Add(m.P2Lx(PD_x));Memo1->Lines->Add(m.P2Ly(PD_y));
	Memo1->Lines->Add(Zoom); */

	Zoom-=fmod(Zoom,0.5);//přepočte na používaný krok zoomu
	if(Zoom<0.5)Zoom=0.5;if(Zoom>4)Zoom=4;

	//vycentrování obrazu
	Posun.x=m.round((MaxX+MinX)/2/m2px-(PD_x/2)/Zoom);
	Posun.y=-m.round((MaxY+MinY)/2/m2px+(PD_y/2)/Zoom);

	if(MaxX+MinX==0)//v případě, že není objekt
	{
		 Posun.x=-scListGroupKnihovObjektu->Width;if(vyska_menu>0)Posun.y=-vyska_menu+9;else Posun.y=-29;
	}

	//SB(Zoom,2); už se používá jinak
	on_change_zoom_change_scGPTrackBar();

  }catch(...){};

	REFRESH();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm1::ESC()
{
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	zneplatnit_minulesouradnice();
	//vymaže, překreslí, odznačí provizorní lini
	switch(Akce)
	{
		case MOVE:d.odznac_oznac_objekt(Canvas,pom,akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x,akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y);break;
		case ADD:
		{
			if(MOD!=NAHLED)
			{
				if(add_posledni)d.odznac_oznac_objekt_novy_posledni(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
				else d.odznac_oznac_objekt_novy(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,pom);
				pom=NULL;//nesmí být v další sekci
			}
			else
			{
				d.vykresli_element(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"","",element_id,-1);
      }
		}break;
		case VYH:
		{
			d.odznac_oznac_vetev(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,pom_vyhybka);
			d.v.smaz_objekt(pom_vyhybka);
		}break;
	}
	pom_vyhybka=NULL;
	proces_pom=NULL;
	kurzor(standard);
	Akce=NIC;//musí být nad refresh
	REFRESH();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm1::add_objekt(int X, int Y)
{
	if(vybrany_objekt>-1)
	{
		TPointD souradnice;
		if(grid)//pokud je zobrazena mřížka
		{
			switch(prichytavat_k_mrizce)
			{
					case 0:
					{
						if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete přichytit objekt k mřížce?","",MB_YESNO,true,true))
						{
							if(myMessageBox->CheckBox_pamatovat->Checked)prichytavat_k_mrizce=1;
							souradnice.x=m.round(m.P2Lx(X)/(size_grid*1.0*m2px))*size_grid*m2px;souradnice.y=m.round(m.P2Ly(Y)/(size_grid*1.0*m2px))*size_grid*m2px;
						}
						else
						{
							if(myMessageBox->CheckBox_pamatovat->Checked)prichytavat_k_mrizce=2;
							souradnice=m.P2L(TPoint(X,Y));
						}
						break;
					}
					case 1:	souradnice.x=m.round(m.P2Lx(X)/(size_grid*1.0*m2px))*size_grid*m2px;souradnice.y=m.round(m.P2Ly(Y)/(size_grid*1.0*m2px))*size_grid*m2px;break;//přichytí automaticky
					case 2: souradnice=m.P2L(TPoint(X,Y));break;//automaticky nepřichyt
			}
			akutalizace_stavu_prichytavani_vSB();
		}
		else souradnice=m.P2L(TPoint(X,Y));

		//uložení do paměti
		bool spojka=false;
		if(pom==NULL && pom_vyhybka!=NULL){pom=pom_vyhybka;spojka=true;}//druhokolové přidávání tzn. spojka
		if(add_posledni)//vloží poslední prvek
		{ //do pom_vyhybka přebírá pouze pro případné účely vyhýbky, pro ostatní objekty má význam metoda bez návratové hodnoty
			pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y);
		}
		else//vkládá prvek mezi prvky
		{ //do pom_vyhybka přebírá pouze pro případné účely vyhýbky,, pro ostatní objekty má význam metoda bez návratové hodnoty
			pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y,pom);
			d.v.zvys_indexy(pom);//zvýší indexy nasledujicích bodů
		}
		pom=NULL;//odsranění pomocného ukazatele
		//ihned vykreslení
		//pokud zruším nutnost invalidate kvůli spojovacím liniim, možno odkomentovat
		//d.vykresli_rectangle(Canvas,souradnice,knihovna_objektu[vybrany_objekt].name,knihovna_objektu[vybrany_objekt].short_name);
                               //pokud se jedná o spojku bude již ukončeno
		if(vybrany_objekt==VyID && spojka==false)//vyhybka
		{
			Akce=VYH;//kurzor(doplnit)
		}
		else//vše ostatní či spojka
		{
			pom_vyhybka=NULL;//odsranění pomocného ukazatele
			Akce=NIC;//musí být nad REFRESH
			kurzor(standard);
			ortogonalizace();
			vybrany_objekt=-1;//odznačí objekt logicky, musí se nový vybrat znovu
		}
		REFRESH();
		DuvodUlozit(true);
	}
}
//---------------------------------------------------------------------------
Cvektory::TObjekt *TForm1::add_objekt_za()
{
	Cvektory::TObjekt *p=d.v.OBJEKTY->dalsi;//přeskočí hlavičku
	Cvektory::TObjekt *ret=NULL;
	while (p!=NULL)
	{
		if(p->dalsi!=NULL)//aby se neřešila situace poslední-prní prvek
		{
			if(d.lezi_v_pasmu(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,p))
			{
				ret=p;
				break;
			}
		}
		p=p->dalsi;//posun na další prvek
	}
	return ret;
}
//---------------------------------------------------------------------------
void TForm1::move_objekt(int X, int Y)
{
		//posouvám v bodě uchopení, tj. rozdíl refernčního bodu a bodu uchopení odčítám resp. přičítám k místu puštění myši, pokud bych chtěl na referenční bod stačí pom->X a pom->Y;
		pom->X=m.P2Lx(X)-m.P2Lx(vychozi_souradnice_kurzoru.x)+pom->X;pom->Y=m.P2Ly(Y)+pom->Y-m.P2Ly(vychozi_souradnice_kurzoru.y);
		if(grid)//pokud je zobrazena mřížka
		{
			switch(prichytavat_k_mrizce)
			{
					//přichytávat dotaz
					case 0:
					{ //ano
						if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt přichytit k mřížce?","",MB_YESNO,true,true))
						{
							if(myMessageBox->CheckBox_pamatovat->Checked)prichytavat_k_mrizce=1;//pamatovat ano
							pom->X=m.round(pom->X/(size_grid*1.0*m2px))*size_grid*m2px;pom->Y=m.round(pom->Y/(size_grid*1.0*m2px))*size_grid*m2px;
						}
						else//ne
						{
							if(myMessageBox->CheckBox_pamatovat->Checked)prichytavat_k_mrizce=2;//pamatovat ne
							//- souřadnice již zadáné nahoře
						}
						break;
					}
					//přichytí automaticky
					case 1: pom->X=m.round(pom->X/(size_grid*1.0*m2px))*size_grid*m2px;pom->Y=m.round(pom->Y/(size_grid*1.0*m2px))*size_grid*m2px;break;
					case 2:break;//automaticky nepřichyt - souřadnice již zadáný nahoře
			}
			akutalizace_stavu_prichytavani_vSB();
		}
		posun_objektu=false;Akce=NIC;kurzor(standard);//REFRESH();DuvodUlozit(true);
		zmen_poradi_objektu(X,Y);//testuje zda se nejedná o změnu pořadí (to musí ještě uživatel potvrdit), pokud ano, vrácí true
		ortogonalizace();
		REFRESH();DuvodUlozit(true);
		pom=NULL;
}
//---------------------------------------------------------------------------
void TForm1::zmen_poradi_objektu(int X, int Y)//testuje zda se nejedná o změnu pořadí (to musí ještě uživatel potvrdit)
{
		//zjištění oblasti, vynechává situace, kdy se nejedná o změnu pořadí
		bool RET=false;
		Cvektory::TObjekt *ukaz=d.v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
		while (ukaz!=NULL)//mimo posledního prvku
		{
			if(ukaz!=pom && ukaz!=pom->predchozi && ukaz->dalsi!=pom->dalsi)//přeskakuje situaci, pokud by se chtěl vkládat na původní pozici
			{
					if(ukaz==d.v.OBJEKTY->predchozi)//poslední prvek versus první prvek
					{
						if(d.lezi_v_pasmu_poslednim(Canvas,X,Y,false))
						{
							RET=TRUE;
							break;
						}
					}
					else//ostatní situace
					{
						if(d.lezi_v_pasmu(Canvas,X,Y,ukaz,false))
						{
							RET=TRUE;
							break;
						}
					}
			}
			ukaz=ukaz->dalsi;//posun na další prvek v seznamu
		}
		//pokud se může jednat o snahu (zjištěno z předchozí navrácenoho RET) o vložení ještě se na to dotazuje u uživatele
		if(RET)
		{
			if(ukaz==d.v.OBJEKTY->predchozi)//první prvek versus poslední
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt \""+AnsiString(pom->name.UpperCase())+"\" umístit v pořadí mezi objekty \""+AnsiString(ukaz->name.UpperCase())+"\" a \""+AnsiString(d.v.OBJEKTY->dalsi->name.UpperCase())+"\"?","",MB_YESNO,true,false))
				{
					d.v.zmen_poradi_objektu(pom,d.v.OBJEKTY->predchozi);//volání realizace samotné záměny
				}
			}
			else//ostatní situace
			{

				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt \""+AnsiString(pom->name.UpperCase())+"\" umístit v pořadí mezi objekty \""+AnsiString(ukaz->name.UpperCase())+"\" a \""+AnsiString(ukaz->dalsi->name.UpperCase())+"\"?","",MB_YESNO,true,false))
				{
					if(pom->n<ukaz->n)//vkládání dozadu
						d.v.zmen_poradi_objektu(pom,ukaz);//volání realizace samotné záměny
					else//dopředné vkládání
						d.v.zmen_poradi_objektu(pom,ukaz->dalsi);//volání realizace samotné záměny
				}
			}
		}
}
//---------------------------------------------------------------------------
void TForm1::add_element (int X, int Y)
{
	////ČÁSTEČNĚ PROVIZORNĚ
	//rotace dle umístění na ose Y či X dle trendu
	short trend=m.Rt90(d.trend(pom));
	short rotace_symbolu=rotace_symbol(trend,X,Y);

	//ovlivňování souřadnic, aby element byl umístěn přímo na osou - provizorní pro robota
	double DoSkRB=0;
	if(1<=element_id && element_id<=4)//pro roboty, které mají uchopovací bod jinde než referenční
	{
		DoSkRB=d.DoSkRB*Zoom/m2px;//délka od středu (uchopovacího bodu) k referenčnímu bodu, doplnit konstanty
		if(rotace_symbolu==90 || rotace_symbolu==180)DoSkRB*=-1;
	}
	if(trend==90 || trend==270)Y=m.round((ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2.0+DoSkRB);//vodorovný pohon
	else X=m.round(F->scSplitView_LEFTTOOLBAR->Width+(F->ClientWidth-F->scSplitView_LEFTTOOLBAR->Width)/2.0+DoSkRB);//svislý pohon
	////---- konec PROVIZORNĚ

	//vložení elementu na dané souřadnice a do patřičného spojáku - pozor jedná se o chybu návrhu, nemělo by se vkládát do pom resp. ostrého spojáku objektů pro případ storna....
	Cvektory::TElement *E=d.v.vloz_element(pom_temp,element_id,m.P2Lx(X),m.P2Ly(Y));

	//navrácení rotace dle umístění v objektu
	E->rotace_symbolu=rotace_symbolu;

	//nadesignuje tabulky daného elementu
	design_element(E);

	//automatické výchozí umístění mGridové tabulky dle rotace elementu a nadesignováné tabulky (jejích rozměrů) - proto musí být až za nastevením designu
	aut_pozicovani(E,X,Y);

	//až na konec:
	E=NULL;delete E;
	Akce=NIC;
	REFRESH();
	DrawGrid_knihovna->Invalidate();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//automatické nekonfliktní pozicování tabulek podle tabulek ostatních elementů
void TForm1::aut_pozicovani(Cvektory::TElement *E, int X, int Y)
{
	short O=10*3;//hodnota odsazení
	double x=0,x1=0,y=0,y1=0;//původní a překlopené souřadnice tabulky
	bool hor=false,ver=false,pom=true;
	int prekryti=0;//počet překrytí
	switch(E->rotace_symbolu)
	{
		case 0:
		hor=true;//jedná se o horizontálně orientovanou kabinu
		x=X-E->mGrid->Width/2.0;//neměnná souřadnice
		if (1>element_id||element_id>4)//otoče a stop stanice
		{
			y=Y+O;//původní hodnota
			y1=Y-d.DoSkRB*Zoom/m2px-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Height; //překlopená hodnota
		}
		else//roboti
		{
			y=Y-E->mGrid->Height-d.DoSkRB*Zoom/m2px-O;
			y1=Y+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;
		}
		break;
		case 90:
		ver=true;//jedná se o vertikálně orientovanou kabinu
		y=Y-E->mGrid->Height/2.0;
		if (1>element_id||element_id>4)
		{
			x=X-O-E->mGrid->Width;
			x1=X+d.DoSkRB*Zoom/m2px+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;//Zarovnání tabulky nerobotu s robo tabulkami
		}
		else
		{
			x=X+d.DoSkRB*Zoom/m2px+O;
			x1=X-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Width;
		}
		break;//pozor invertované DoSkRB
		case 180:
		hor=true;
		x=X-E->mGrid->Width/2.0;
		if (1>element_id||element_id>4)
		{
			y=Y-O-E->mGrid->Height;
			y1=Y+d.DoSkRB*Zoom/m2px+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;
		}
		else
		{
			y=Y+d.DoSkRB*Zoom/m2px+O;
			y1=Y-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Height;
		}
		break;//pozor invertované DoSkRB
		case 270:
		ver=true;
		y=Y-E->mGrid->Height/2.0;
		if (1>element_id||element_id>4)
		{
			 x=X+O;
			 x1=X-d.DoSkRB*Zoom/m2px-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Width;
		}
		else
		{
			x=X-d.DoSkRB*Zoom/m2px-E->mGrid->Width-O;
			x1=X+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;
		}
		break;
	}
	//Pro horizontální překlápění
	if (E->predchozi->n>=1 && hor)//Kontrola zda je už vložený nějaký element
	{
		Cvektory::TElement *O=E->predchozi;
		while(O->n >= 1)//kontrola překrytí napříč všemi dosut přidanými elementy
		{
			if ((x<=m.L2Px(O->Xt)+O->mGrid->Width && x>=m.L2Px(O->Xt)) ||//Překrývají se v X souřadnici
			(x+E->mGrid->Width<=m.L2Px(O->Xt)+O->mGrid->Width && x+E->mGrid->Width>=m.L2Px(O->Xt)))
			{
				prekryti++;//počet překrytí
				switch (prekryti)
				{
					case 1://první překrytí = přehodí objekt v ose y
					{
						if (y==m.L2Py(O->Yt)||y+E->mGrid->Height==m.L2Py(O->Yt)+O->mGrid->Height||(y>=m.L2Py(O->Yt) && y<=m.L2Py(O->Yt)+O->mGrid->Height) ||
						(y+E->mGrid->Height>=m.L2Py(O->Yt) && y+E->mGrid->Height<=m.L2Py(O->Yt)+O->mGrid->Height))
						y=y1;
					}
					case 2://druhé překrytí = není možné vložit tabulku nekonfliktně
					{      //vložení do defaultní polohy = uživatel musí přesunout ručně
						if (y==m.L2Py(O->Yt)||y+E->mGrid->Height==m.L2Py(O->Yt)+O->mGrid->Height||(y>=m.L2Py(O->Yt) && y<=m.L2Py(O->Yt)+O->mGrid->Height) ||
						(y+E->mGrid->Height<=m.L2Py(O->Yt) && y+E->mGrid->Height>=m.L2Py(O->Yt)+O->mGrid->Height))
						{
							x=X;
							y=Y;
						}
						break;
					}
				}
			}
			O=O->predchozi;
		}
		O=NULL;delete O;
	}
	//Pro vertikální překlápění
	if (E->predchozi->n>=1 && ver)
	{
		Cvektory::TElement *O=E->predchozi;
		while(O->n >= 1)
		{
			if ((y<=m.L2Py(O->Yt)+O->mGrid->Height && y>=m.L2Py(O->Yt)) ||
			(y+E->mGrid->Height<=m.L2Py(O->Yt)+O->mGrid->Height && y+E->mGrid->Height>=m.L2Py(O->Yt)))
			{
				prekryti++;
				switch (prekryti)
				{
					case 1:
					{
						if (x==m.L2Px(O->Xt)||x+E->mGrid->Width==m.L2Px(O->Xt)+O->mGrid->Width||(x>=m.L2Px(O->Xt) && x<=m.L2Px(O->Xt)+O->mGrid->Width) ||
						(x+E->mGrid->Width>=m.L2Px(O->Xt) && x+E->mGrid->Width<=m.L2Px(O->Xt)+O->mGrid->Width))
						x=x1;
					}
					case 2:
					{
						if (x==m.L2Px(O->Xt)||x+E->mGrid->Width==m.L2Px(O->Xt)+O->mGrid->Width||(x>=m.L2Px(O->Xt) && x<=m.L2Px(O->Xt)+O->mGrid->Width) ||
						(x+E->mGrid->Width>=m.L2Px(O->Xt) && x+E->mGrid->Width<=m.L2Px(O->Xt)+O->mGrid->Width))
						{
							x=X;
							y=Y;
						}
						break;
					}
				}
			}
			O=O->predchozi;
		}
		O=NULL;delete O;
	}
  //zapsaní nejlepších souřadnic
	E->Xt=m.P2Lx(x);
	E->Yt=m.P2Ly(y);
	//vynulování a smazání ukazatele
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//provizorně
short TForm1::rotace_symbol(short trend,int X, int Y)
{
	short rotace_symbolu=trend-90;

	if(trend==90 || trend==270)//pohon vodorovně
	{
		if((ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2.0>Y){rotace_symbolu+=180;}
	}
	else//pohon svisle
	{
		if(F->scSplitView_LEFTTOOLBAR->Width+(F->ClientWidth-F->scSplitView_LEFTTOOLBAR->Width)/2.0<X)rotace_symbolu+=180;
	}
	return rotace_symbolu;
}
//---------------------------------------------------------------------------
//nadesignuje tabulky daného elementu
void TForm1::design_element(Cvektory::TElement *E)
{
	//definice barev
	TColor clHeaderFont=clBlack;
	TColor clBackgroundHidden=m.clIntensive(RGB(128,128,128),105);
	TColor clBorder= (TColor)RGB(128,128,128);
	TColor clFontLeft = (TColor)RGB(128,128,128);
	TColor clFontRight = (TColor)RGB(43,87,154);
	TColor clBottomBorder = clBlack;
	TColor clRightBorder  = clBlack;
	//definice jednotek a šířek
	AnsiString LO,cas,delka_otoce;
	short sirka=80,sirka1=60,sirka_o=80,sirka_o1=60;//hodnoty pro základní jednotky
	//Nastavení jednotek a šířek podle posledních nastavení
	if (PTunit==0) {cas="<a>[s]</a>";sirka1=80;sirka_o1=80;}//0
	else {cas="<a>[min]</a>";sirka=95;}//1
	if (LOunit==0) {LO="<a>[m]</a>";}//0
	else {LO="<a>[mm]</a>";sirka=95;sirka1=80;}//1
	if (DOtocunit==0) {delka_otoce="<a>[m]</a>";}//0
	else {delka_otoce="<a>[mm]</a>";sirka_o=95;sirka_o1=80;}//1
	//nadesignování tabulek dle typu elementu
	switch(element_id)
	{
		case 0://stop stanice
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,6);
			//definice buněk
			E->mGrid->Cells[0][1].Text="výběr párové STOP";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text="max. WT stop "+cas;//chybí v elementu
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(25);
			E->mGrid->Cells[0][3].Text="WT palec "+cas;//chybí v elementu
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outPT(3);
			E->mGrid->Cells[0][4].Text="akt. počet vozíků";
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=6;
			E->mGrid->Cells[0][5].Text="max. počet vozíků";
			E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;E->mGrid->Cells[1][5].Text=7;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=170;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}

		case 1://robot (kontinuální)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,3);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			/////Test NUMERIC
			E->mGrid->Cells[1][1].Type=E->mGrid->NUMERIC;
			E->mGrid->Cells[1][1].Text=outPT(2);//2minuty
			//E->mGrid->getNumeric(1,1)->Hint=10;//ms.MyToDouble(E->mGrid->Cells[1][1].Text);
			//Memo3->Lines->Add(E->mGrid->getNumeric(1,1)->Name);
			//E->mGrid->getNumeric(1,1)->Value=0.33333;
			//E->mGrid->getNumeric(1,1)->Decimal=2;
			//E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=120/(1+59.0*Tcas);
			E->mGrid->Cells[0][2].Text="LO "+LO;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outLO(1.5);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 2://robot se stop stanicí
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,3);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(120);
			E->mGrid->Cells[0][2].Text="max WT "+cas;E->mGrid->Cells[1][2].Text=outPT(20);//chybí v elementu
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka+30;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 3://robot s pasivní otočí
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(120);
			E->mGrid->Cells[0][2].Text="LO1 "+LO;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outLO(1.2);
			E->mGrid->Cells[0][2].BottomBorder->Width=2;
			E->mGrid->Cells[1][2].BottomBorder->Width=2;
			E->mGrid->Cells[0][3].Text="otoč "+cas;
			E->mGrid->Cells[1][3].Text=outPT(60);//PTOtoc??
			E->mGrid->Cells[0][4].Text="otoč "+delka_otoce;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outDO(0.6);
			E->mGrid->Cells[0][4].BottomBorder->Width=2;
			E->mGrid->Cells[1][4].BottomBorder->Width=2;
			E->mGrid->Cells[0][5].Text="PT2 "+cas;
			E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;E->mGrid->Cells[1][5].Text=outPT(140);
			E->mGrid->Cells[0][6].Text="LO2 "+LO;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outLO(1.4);
			E->mGrid->Cells[0][6].BottomBorder->Width=2;
			E->mGrid->Cells[1][6].BottomBorder->Width=2;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka+10;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,5);
			E->mGrid->DefaultCell.isLink->Color=clFontRight;//přiřazení barvy fontu
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(80);
			E->mGrid->Cells[0][2].Text="PTo "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(20);
			E->mGrid->Cells[0][3].Text="PT2 "+cas;
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outPT(80);
			E->mGrid->Cells[0][4].Text="WT "+cas;//není v elementu / TIME ?
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(5);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka+5;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 5://otoč pasivní
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,3);
			//definice buněk
			E->mGrid->Cells[0][1].Text="délka "+delka_otoce;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outDO(0.8);
			E->mGrid->Cells[0][2].Text="PT "+cas;//PT u pasivní nelze zadat
			E->mGrid->Cells[1][2].Text=outPT(120);//původně EDIT, ale background lze nastavit pouze pro text, EDIT se jen slabě orámuje
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_o+15;//Delší text
			E->mGrid->Columns[1].Width=sirka_o1;
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,3);
			//definice buněk
			E->mGrid->Cells[0][1].Text="délka "+delka_otoce;//D u aktivní nelze zadat
			E->mGrid->Cells[1][1].Text=outDO(0.8);//původně EDIT, ale background lze nastavit pouze pro text, EDIT se jen slabě orámuje
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(30);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_o+15;
			E->mGrid->Columns[1].Width=sirka_o1;
			break;
		}
	}
	//formátování hlavičky tabulky (vždy stejné)
	E->mGrid->Border.Width=2;
	E->mGrid->Cells[0][0].Text=E->name;
	E->mGrid->Cells[0][0].Font->Color=clHeaderFont;
	E->mGrid->Cells[0][0].BottomBorder->Width=2;
	//formátování buněk tabulky (vždy stejn=)
	for(int i=1;i<=E->mGrid->RowCount-1;i++)
	{
		if (E->mGrid->Cells[1][i].Type!=E->mGrid->EDIT)
		{
			E->mGrid->Cells[1][i].Font->Color=clFontLeft;
			E->mGrid->Cells[1][i].Background->Color=clBackgroundHidden;
		}
		E->mGrid->Cells[0][i].Align=mGrid->RIGHT;//vypnout zarovnání
		E->mGrid->Cells[0][i].RightMargin = 15;
		E->mGrid->Cells[0][i].Font->Color=clFontLeft;
	}
	//sloučení buněk hlavičky
	E->mGrid->MergeCells(0,0,1,0);
}
//---------------------------------------------------------------------------
//slouží k vyčtení stávajícího nastavení jednotek, k jejich úpravě a zanesení do INI
void TForm1::redesign_element()
{
	AnsiString delka_otoce,LO,cas;
	short sirka=80,sirka1=60,sirka_o=80,sirka_o1=60;
	bool zcas=false,zLO=false,zdelka_otoce=false;
	//zjištění požadavku (co změnit)
	switch (pom_element->eID)
	{
		case (0):
		{
			zcas=true;//obsahuje pouze casové buňky
			break;
		}
		case 1:
		{
			if (JID==101) zcas=true;//časové buňky
			if (JID==102) zLO=true;//délkové buňky
			break;
		}
		case 2:
		{
			zcas=true;//obsahuje pouze casové buňky
			break;
		}
		case 3:
		{
			if (JID==101 || JID==103 || JID==105) zcas=true;//čas
			if (JID==102 || JID==104 || JID==106) zLO=true;//delka
			break;
		}
		case 4:
		{
			zcas=true;//pouze čas
			break;
		}
		case 5:
		{
			if (JID==101) zdelka_otoce=true;//délka
			if (JID==102) zcas=true;//čas
			break;
		}
		case 6:
		{
			if (JID==101) zdelka_otoce=true;//délka
			if (JID==102) zcas=true;//čas
			break;
		}
	}
	//překlopení aktuálních jednotek
	if (zcas)
	{
		if (PTunit==0) {PTunit=1;}
		else {PTunit=0;}
	}
	if (zLO)
	{
		if (LOunit==0) {LOunit=1;}
		else {LOunit=0;}
	}
	if (zdelka_otoce)
	{
		if (DOtocunit==0) {DOtocunit=1;}
		else {DOtocunit=0;}
	}
	//Nastavení jednotek a šířek
	if (PTunit==0) {cas="<a>[s]</a>";sirka1=80;sirka_o1=80;}//0
	else {cas="<a>[min]</a>";sirka=95;}//1
	if (LOunit==0) {LO="<a>[m]</a>";}//0
	else {LO="<a>[mm]</a>";sirka=95;sirka1=80;}//1
	if (DOtocunit==0) {delka_otoce="<a>[m]</a>";}//0
	else {delka_otoce="<a>[mm]</a>";sirka_o=95;sirka_o1=80;}//1
	//procházení pomocného spojitého seznamu
	Cvektory::TElement *E=pom_temp->elementy->dalsi;//zde lze přeskočit hlavičku
	while (E!=NULL)
	{
		akt_tabulek(E,LO,delka_otoce,cas,sirka,sirka1,sirka_o,sirka_o1);
		E=E->dalsi;
	}
	E=NULL; delete E;
	//procházení ostrého spojitého seznamu
	Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
	while (O!=NULL)
	{
		Cvektory::TElement *E=O->elementy;//////////tady se nesmí přeskakovat hlavička
		while (E!=NULL)
		{
			if(E->n>0)//přeskočí funkčně hlavičku
			akt_tabulek(E,LO,delka_otoce,cas,sirka,sirka1,sirka_o,sirka_o1);
			E=E->dalsi;
		}
		E=NULL; delete E;
		O=O->dalsi;
	}
	O=NULL; delete O;
  //zápis změn do INI
	writeINI("nastaveni_nahled", "cas", PTunit);
	writeINI("nastaveni_nahled", "LO", LOunit);
	writeINI("nastaveni_nahled", "delka_otoce", DOtocunit);
	REFRESH();
}
//---------------------------------------------------------------------------
//přepisuje jednotky a upravuje šířku sloupců
void TForm1::akt_tabulek (Cvektory::TElement *E,AnsiString LO,AnsiString delka_otoce,AnsiString cas,short sirka,short sirka1,short sirka_o,short sirka_o1)
{
	switch(E->eID)
	{
		case 0://stop stanice
		{
			E->mGrid->Cells[0][2].Text="max. WT stop "+cas;
			E->mGrid->Cells[0][3].Text="WT palec "+cas;
			E->mGrid->Cells[1][2].Text=outPT(25);
			E->mGrid->Cells[1][3].Text=outPT(3);
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 1://robot (kontinuální)
		{
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[0][2].Text="LO "+LO;
			E->mGrid->Cells[1][1].Text=outPT(120);
			E->mGrid->Cells[1][2].Text=outLO(1.5);
			E->mGrid->Columns[0].Width=sirka;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 2://robot se stop stanicí
		{
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[0][2].Text="max WT "+cas;
			E->mGrid->Cells[1][1].Text=outPT(120);
			E->mGrid->Cells[1][2].Text=outPT(20);
			E->mGrid->Columns[0].Width=sirka+30;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 3://robot s pasivní otočí
		{
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[0][2].Text="LO1 "+LO;
			E->mGrid->Cells[0][3].Text="otoč "+cas;
			E->mGrid->Cells[0][4].Text="otoč "+delka_otoce;
			E->mGrid->Cells[0][5].Text="PT2 "+cas;
			E->mGrid->Cells[0][6].Text="LO2 "+LO;
			E->mGrid->Cells[1][1].Text=outPT(120);
			E->mGrid->Cells[1][2].Text=outLO(1.2);
			E->mGrid->Cells[1][3].Text=outPT(60);
			E->mGrid->Cells[1][4].Text=outDO(0.6);
			E->mGrid->Cells[1][5].Text=outPT(140);
			E->mGrid->Cells[1][6].Text=outLO(1.4);
			E->mGrid->Columns[0].Width=sirka+10;
			E->mGrid->Columns[1].Width=sirka1;
			break;
		}
		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[0][2].Text="PTo "+cas;
			E->mGrid->Cells[0][3].Text="PT2 "+cas;
			E->mGrid->Cells[0][4].Text="WT "+cas;
			E->mGrid->Cells[1][1].Text=outPT(80);
			E->mGrid->Cells[1][2].Text=outPT(20);
			E->mGrid->Cells[1][3].Text=outPT(80);
			E->mGrid->Cells[1][4].Text=outPT(5);
			E->mGrid->Columns[0].Width=sirka+5;
			E->mGrid->Columns[1].Width=sirka1;
		break;
		}
		case 5://otoč pasivní
		{
			E->mGrid->Cells[0][1].Text="délka "+delka_otoce;
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][1].Text=outDO(0.8);
			E->mGrid->Cells[1][2].Text=outPT(120);
			E->mGrid->Columns[0].Width=sirka_o+15;
			E->mGrid->Columns[1].Width=sirka_o1;
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
			E->mGrid->Cells[0][1].Text="délka "+delka_otoce;//D u aktivní nelze zadat
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][1].Text=outDO(0.8);
			E->mGrid->Cells[1][2].Text=outPT(30);
			E->mGrid->Columns[0].Width=sirka_o+15;
			E->mGrid->Columns[1].Width=sirka_o1;
			break;
		}
	}
}
//---------------------------------------------------------------------------
//zapíná či vypíná automatickou ortogonalizaci
void TForm1::ortogonalizace_on_off()
{
	 if(ortogonalizace_stav)//pokud je zapnuta
	 {
		 ortogonalizace_stav=false;//tak vypne
		 SB("Ortogonalizace vypnuta.");
	 }
	 else//pokud vypnuta, tak zapne, zortogonalizuje a zapíše stav do registru
	 {
		 ortogonalizace_stav=true;
		 ortogonalizovat();
		 REFRESH();
		 SB("Ortogonalizace zapnuta.");
	 }
}
//---------------------------------------------------------------------------
//volá ortogonalizaci schéma, pokud je ortogonalizace povolena
void TForm1::ortogonalizace()
{
	 if(ortogonalizace_stav)ortogonalizovat();
}
//---------------------------------------------------------------------------
//ortogonalizuje schéma + přichytne ke mřížce pokud je požadováno
void TForm1::ortogonalizovat()
{
  d.v.ortogonalizovat();
}
//---------------------------------------------------------------------------
void TForm1::zneplatnit_minulesouradnice()
{
	minule_souradnice_kurzoru=TPoint(-200,-200);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_otoceDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
	int W=DrawGrid_otoce->DefaultColWidth  *Z;
	int H=DrawGrid_otoce->DefaultRowHeight  *Z;
	int P=-1*DrawGrid_otoce->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

	Cantialising a;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=DrawGrid_otoce->Width*Z;bmp_in->Height=DrawGrid_otoce->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
	double Zoom_back=Zoom;//záloha zoomu
	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
	short pocet_elementu=2;
  AnsiString label1;
  AnsiString label2;



 	for(unsigned short n=1;n<=pocet_elementu;n++)
	{
    if(n==1){ label1= "pasivní"; label2=""; }
    if(n==2){ label1= "aktivní"; label2=""; }
    if(pom->id!=3)
    {
         d.vykresli_otoc(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P - 15,label1,label2,n+4,0,0,1);
    }
    else d.vykresli_otoc(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P - 15,label1,label2,n+4,0,0,-1);
	}

	Zoom=Zoom_back;//návrácení původního zoomu
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	DrawGrid_otoce->Canvas->Draw(0,0,bmp_out);
	delete (bmp_out);//velice nutné
	delete (bmp_in);//velice nutné
}
//------------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_ostatniDrawCell(TObject *Sender, int ACol, int ARow,
					TRect &Rect, TGridDrawState State)
{
	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
	int W=DrawGrid_ostatni->DefaultColWidth  *Z;
	int H=DrawGrid_ostatni->DefaultRowHeight  *Z;
	int P=-1*DrawGrid_ostatni->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

	Cantialising a;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=DrawGrid_ostatni->Width*Z;bmp_in->Height=DrawGrid_ostatni->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
	double Zoom_back=Zoom;//záloha zoomu
	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
	short pocet_elementu=1;
	for(unsigned short n=1;n<=pocet_elementu;n++)
	{
	if(pom->id!=3)
  {
					d.vykresli_stopku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 2,"STOP","");
  } else  d.vykresli_stopku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 2,"STOP","",0,0,-1);


	}
	Zoom=Zoom_back;//návrácení původního zoomu
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	DrawGrid_ostatni->Canvas->Draw(0,0,bmp_out);
	delete (bmp_out);//velice nutné
	delete (bmp_in);//velice nutné
}

//------------------------------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_poznamkyDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
	int W=DrawGrid_poznamky->DefaultColWidth  *Z;
	int H=DrawGrid_poznamky->DefaultRowHeight  *Z;
	int P=-1*DrawGrid_poznamky->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

	Cantialising a;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=DrawGrid_poznamky->Width*Z;bmp_in->Height=DrawGrid_poznamky->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
	double Zoom_back=Zoom;//záloha zoomu
	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
	short pocet_elementu=2;
  AnsiString label1;
  AnsiString label2;
	for(unsigned short n=1;n<=pocet_elementu;n++)
	{
    if(n==1)
    {
     label1= "text";
     label2="";
     d.vykresli_ikonu_textu(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1);
   //	d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 30,label1,label2,n);
     }
    if(n==2)
    {
     label1= "šipka";
     label2="";
     d.vykresli_ikonu_sipky(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1);
     }

	}
	Zoom=Zoom_back;//návrácení původního zoomu
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	DrawGrid_poznamky->Canvas->Draw(0,0,bmp_out);
	delete (bmp_out);//velice nutné
	delete (bmp_in);//velice nutné
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
			TGridDrawState State)
{
if(MOD==NAHLED)
{
	scListGroupKnihovObjektu->Caption="Roboti";
	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
	int W=DrawGrid_knihovna->DefaultColWidth  *Z;
	int H=DrawGrid_knihovna->DefaultRowHeight  *Z;
	int P=-1*DrawGrid_knihovna->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

	Cantialising a;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=DrawGrid_knihovna->Width*Z;bmp_in->Height=DrawGrid_knihovna->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
	double Zoom_back=Zoom;//záloha zoomu
	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
  AnsiString label1;
  AnsiString label2;
	short pocet_elementu=4;
  int EID=d.v.vrat_eID_prvniho_pouziteho_robota(pom);
	for(unsigned short n=1;n<=pocet_elementu;n++)
	{
  if(n==1){ label1= "kontinuální"; label2="lakování"; }
  if(n==2){ label1= "S&G"; label2="lakování"; }
  if(n==3){ label1= "kontinuální s";  label2="pasiv. otočí"; }
  if(n==4){ label1= "S&G s";  label2="akt. otočí"; }

  if(pom->id==3)
    {
    if(EID==-1) d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 30,label1,label2,n);
    }else d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 30,label1,label2,n,0,0,-1);
	}
   if(pom->id==3)
   {
   if(EID==1 || EID==3)
   {
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P + 30,label1,label2,1);
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P + 30,label1,label2,2,0,0,-1);
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P + 30,label1,label2,3);
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P + 30,label1,label2,4,0,0,-1);
   }
	 else if (EID==2 || EID==4)
   {
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P + 30,label1,label2,1,0,0,-1);
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P + 30,label1,label2,2);
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P + 30,label1,label2,3,0,0,-1);
    d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P + 30,label1,label2,4);

   }

   }

	Zoom=Zoom_back;//návrácení původního zoomu
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	DrawGrid_knihovna->Canvas->Draw(0,0,bmp_out);
	delete (bmp_out);//velice nutné
	delete (bmp_in);//velice nutné
  }
  if(MOD==SCHEMA)
	{
	////////////////////neAA verze
	scListGroupKnihovObjektu->Caption="Technolog.objekty";
	TCanvas* C=DrawGrid_knihovna->Canvas;
	int W=DrawGrid_knihovna->DefaultColWidth;
	int H=DrawGrid_knihovna->DefaultRowHeight;
	int P=-1*DrawGrid_knihovna->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování


	unsigned short obdelnik_okrajX=10;unsigned short obdelnik_okrajY=5;unsigned short okraj_packy=obdelnik_okrajY;
	C->Font->Style = TFontStyles()<< fsBold;
	C->Font->Size=12;
	C->Font->Name="Arial";
	C->Pen->Width=1;
	C->Pen->Color=(TColor)RGB(190,190,190);//(TColor)RGB(19,115,169);
	C->Brush->Color=(TColor)RGB(190,190,190);//(TColor)RGB(19,115,169);
	C->Font->Color=clWhite;
	for(unsigned short n=1;n<=pocet_objektu_knihovny;n++)
	{
		UnicodeString text=knihovna_objektu[n-1].short_name;
		//symbol objektu
		if(VyID!=n-1)
		{
			//obdélník
			C->Rectangle(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+obdelnik_okrajY+P,((n+1)%2+1)*W-obdelnik_okrajX,ceil(n/2.0)*H-obdelnik_okrajY+P);
			//packy
			C->MoveTo(((n+1)%2)*W+okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);
			C->MoveTo(((n+1)%2)*W+W-obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+W-okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);
			//písmo
			C->TextOutW((Rect.Right-Rect.Left-C->TextWidth(text))/2+((n+1)%2)*W,(Rect.Bottom-Rect.Top-C->TextHeight(text))/2+(ceil(n/2.0)-1)*H+P,text);
		}
		else//výhybka v bmp
		{
			Graphics::TBitmap *bmp=new Graphics::TBitmap();
			ImageList48->GetBitmap(51,bmp);
			C->Draw(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+P,bmp);
			bmp=NULL;delete bmp;
		}
	}
 }
//--------------------------------------------

////	////////////////////AA verze
//	short Z=3;
//	int W=DrawGrid_knihovna->DefaultColWidth*Z;
//	int H=DrawGrid_knihovna->DefaultRowHeight*Z;
//
//	int P=-1*DrawGrid_knihovna->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
//
//	Cantialising a;
//	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
//	bmp_in->Width=DrawGrid_knihovna->Width*Z;bmp_in->Height=DrawGrid_knihovna->Height*Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
//	TCanvas* C=bmp_in->Canvas;
//
//	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;unsigned short okraj_packy=obdelnik_okrajY;
//	C->Font->Style = TFontStyles()<< fsBold;
//	//if(antialiasing)C->Font->Size=12;else
//	C->Font->Size=12*Z;
//	C->Font->Name="Arial";
//	C->Pen->Width=1*Z;
//	C->Pen->Color=(TColor)RGB(190,190,190);//(TColor)RGB(19,115,169);
//	C->Brush->Color=(TColor)RGB(190,190,190);//(TColor)RGB(19,115,169);
//	C->Font->Color=clWhite;
//	int X,Y;
//	double Zoom_back=Zoom;//záloha zoomu
//	Zoom=1;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
//	for(unsigned short n=1;n<=pocet_objektu_knihovny;n++)
//	{
//		UnicodeString text=knihovna_objektu[n-1].short_name;
//		//symbol objektu
//		if(VyID!=n-1)
//		{
//			//obdélník
//			C->Rectangle(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+obdelnik_okrajY+P,((n+1)%2+1)*W-obdelnik_okrajX,ceil(n/2.0)*H-obdelnik_okrajY+P);
//			//packy
//			C->MoveTo(((n+1)%2)*W+okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);
//			C->MoveTo(((n+1)%2)*W+W-obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+W-okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);
//			//písmo
//			C->TextOutW((Rect.Right*Z-Rect.Left*Z-C->TextWidth(text))/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z-C->TextHeight(text))/2+(ceil(n/2.0)-1)*H+P,text);
//		}
//		else//výhybka v bmp
//		{
//			X=((n+1)%2)*W/Z+obdelnik_okrajX/Z;
//			Y=(ceil(n/2.0)-1)*H/Z+P/Z;
//		}
//	}
//	Zoom_back=Zoom;//návrácení původního zoomu
//	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
//	DrawGrid_knihovna->Canvas->Draw(0,0,bmp_out);
//	delete (bmp_out);//velice nutné
//	delete (bmp_in);//velice nutné
//
//	//výhybka v bmp
//	Graphics::TBitmap *bmp=new Graphics::TBitmap();
//	ImageList48->GetBitmap(51,bmp);
//	DrawGrid_knihovna->Canvas->Draw(X,Y,bmp);
//	bmp=NULL;delete bmp;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	int Col,Row;
	//DrawGrid_knihovna->MouseToCell(X,Y,Col,Row);
	Col=DrawGrid_knihovna->Col; Row=DrawGrid_knihovna->Row;

	if(MOD==NAHLED)
  {
		knihovna_id=1;
		if(Row==0)element_id=Col+1;
		if(Row==1)element_id=Col+3;
		SB("Kliknutím na libovolné místo umístíte vybraný element.");
		Akce=ADD;kurzor(add_o);
	}
	else//pro SCHEMA
	{
		SB("Kliknutím na libovolné místo umístíte objekt "+knihovna_objektu[Col+Row+Row].name);
		//SB(AnsiString(DrawGrid_knihovna->TopRow)+" "+AnsiString(Col)+" "+AnsiString(Row)+" "+knihovna_objektu[Col+Row+Row].name);
		if(Col>-1 && Row>-1)
		{
			vybrany_objekt=Col+Row+Row;
			Akce=ADD;kurzor(add_o);//Screen->Cursor=crCross;
			add_posledni=true;pom=NULL;
			//ShowMessage(vybrany_objekt);
			if(VyID==vybrany_objekt && d.v.OBJEKTY->predchozi->n<3)//pokud je vybraná vyhýbka nejsou alespoň 3 objekty
			{
				MB("Výhybku lze nastavit, pokud jsou k dispozici minimálně 3 technologické objekty!");
				Akce=NIC;kurzor(standard);
			}
		}
  //ShowMessage(element_id);
	//*pozn n-tý sloupec + (n-tý řádek - 1)* celkový počet slouců
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_otoceMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	int Col,Row;
	Col=DrawGrid_otoce->Col; Row=DrawGrid_otoce->Row;
	knihovna_id=2;
	if(Row==0) element_id=Col+5;
	SB("Kliknutím na libovolné místo umístíte vybraný element.");
	Akce=ADD;kurzor(add_o);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_ostatniMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	int Col,Row;
	Col=DrawGrid_ostatni->Col; Row=DrawGrid_ostatni->Row;
	knihovna_id=3;
	if(Row==0)  element_id=0;
	SB("Kliknutím na libovolné místo umístíte vybraný element.");
	Akce=ADD;kurzor(add_o);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_geometrieMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	int Col,Row;
	Col=DrawGrid_ostatni->Col; Row=DrawGrid_ostatni->Row;
	knihovna_id=4;
	if(Row==0 && Col==0)  element_id=7;
	if(Row==0 && Col==1)  element_id=8;
	SB("Kliknutím na libovolné místo umístíte vybraný element.");
	Akce=ADD;kurzor(add_o);
}
//---------------------------------------------------------------------------
//přeposílá událost na form
void __fastcall TForm1::DrawGrid_knihovnaMouseWheelDown(TObject *Sender, TShiftState Shift,
					TPoint &MousePos, bool &Handled)
{
	//SB("down");
	FormMouseWheelDown(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------
//přeposílá událost na form
void __fastcall TForm1::DrawGrid_knihovnaMouseWheelUp(TObject *Sender, TShiftState Shift,
					TPoint &MousePos, bool &Handled)
{
	//SB("up");
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RzSizePanel_knihovna_objektu_OLDMouseEnter(TObject *Sender)
{
//DrawGrid_knihovna->Enabled=true;

		//kvůli navigačním liniim, pokud se v průběhu přidávní bodu dostane uživatel zpět do panelu, tak aby zde nezůstaly "viset" chybné line
		if(add_posledni)d.odznac_oznac_objekt_novy_posledni(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y);
		else if(pom!=NULL)d.odznac_oznac_objekt_novy(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
		probehl_zoom=true;zneplatnit_minulesouradnice();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaMouseLeave(TObject *Sender)
{
		//DrawGrid_knihovna->Enabled=false;
}
//---------------------------------------------------------------------------
//přeposílá událost na form
//přeposílá událost na form
void __fastcall TForm1::Button1Click(TObject *Sender)
{
//TDateTime TIME;
//TIME=TDateTime(AnsiString(TIME.CurrentDate().DateString())+" "+"8:00:00");
//S(TIME);

//S(MB_OK);
//S(MB_YESNO);
//
//Cvektory::TObjekt *ukaz=d.v.OBJEKTY->dalsi;
//Memo2->Clear();
//Memo2->Visible=true;
//while(ukaz!=NULL)
//{
//		Memo2->Lines->Add(AnsiString(ukaz->n)+" "+ukaz->name);
//		ukaz=ukaz->dalsi;
//}
//S(m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.v.OBJEKTY->dalsi->X,d.v.OBJEKTY->dalsi->Y,d.v.OBJEKTY->dalsi->dalsi->X,d.v.OBJEKTY->dalsi->dalsi->Y));
			/*HRGN hreg=CreateEllipticRgn(100,100,300,200);
				Canvas->Brush->Color=clRed;
				FillRgn(Canvas->Handle,hreg,Canvas->Brush->Handle);
				FrameRgn(Canvas->Handle,hreg,Canvas->Brush->Handle,2,2);
				InvertRgn(Canvas->Handle,hreg);*/
/*
				POINT *body=new POINT[5];

body[0].x=100; body[0].y=100;
body[1].x=350; body[1].y=50;
body[2].x=250; body[2].y=300;
body[3].x=500; body[3].y=500;
body[4].x=20; body[4].y=500;
HRGN hreg=CreatePolygonRgn(body,5,WINDING);//vytvoření regionu
				Canvas->Brush->Color=clRed;
				Canvas->Polygon((TPoint*)body,4);
				Canvas->Brush->Color=clYellow;
				FrameRgn(Canvas->Handle,hreg,Canvas->Brush->Handle,5,5);
				//InvertRgn(Canvas->Handle,hreg);
				//FillRgn(Canvas->Handle,hreg,Canvas->Brush->Handle);

				if(PtInRegion(hreg,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y))S("a");
				else S("není");
				//delete body;body=NULL;
				//DeleteObject(hreg);


	 //DrawGrid_knihovna->Enabled=false;;

	//DrawGrid_knihovna->MouseToCell(X,Y,Col,Row);


	//S(d.v.OBJEKTY->dalsi->X);
	//S(m.L2Px(d.v.OBJEKTY->dalsi->X));
	/*S(ClientHeight);
	S(Height-RzStatusBar1->Height-RzToolbar1->Height);
	S(Form1->Height);
	S(m.round(abs(Posun.x))%size_grid);
	Posun.x=d.v.OBJEKTY->dalsi->X-Width/2;
	Posun.y=-d.v.OBJEKTY->dalsi->Y-(Height-RzStatusBar1->Height-RzToolbar1->Height)/2;
	Invalidate();*/
	//S(d.v.OBJEKTY->dalsi->X); S(d.v.OBJEKTY->dalsi->Y);
	//Memo1->Lines->Add(AnsiString(m.rotace(Edit1->Text.ToDouble(),Edit2->Text.ToDouble(),Edit3->Text.ToDouble()).x)+";"+AnsiString(m.rotace(Edit1->Text.ToDouble(),Edit2->Text.ToDouble(),Edit3->Text.ToDouble()).y));
	/*R->Canvas->CopyRect(Rect(0,0,ClientWidth,ClientHeight),Canvas,Rect(0,0,ClientWidth,ClientHeight));//uloží výřez
	R->Canvas->MoveTo(0,0);
	R->Canvas->LineTo(500,500);
	Canvas->Draw(0,0,R);
	R->SaveToFile("smazat.bmp");*/
	//Canvas->Draw(0,0,R);

////////////---ZKOUŠKA ZAKÁZKY

	//zkouška hlavičky
	//d.v.vymaz_seznam_ZAKAZKY_temp(); - UŽ NENÍ POTŘEBA, VOLÁ SE AUTOMATICKY PŘI KOPIROVÁNÍ kopirujZAKAZKY_temp2ZAKAZKY();
	//d.v.hlavicka_ZAKAZKY_temp();- UŽ NENÍ POTŘEBA, VOLÁ SE VE VLOZ_TEMP_ZAKAZKU, JE-LI TŘEBA
	Cvektory::TJig j;
	j.sirka=1;
	j.delka=2;
	j.vyska=3;
  j.ks=4;

	//zkouška vkládání jednotlivých zakázek
	d.v.vloz_temp_zakazku("prvni",0,"prvni_zakazka",clRed,50,2.0,j,30,0,0);
	d.v.vloz_temp_zakazku("druha",0,"druha_zakazka",clGreen,50,2.0,j,30,0,0);
	d.v.vloz_temp_zakazku("treti",0,"treti_zakazka",clBlue,50,2.0,j,30,0,0);
	d.v.vloz_temp_zakazku("ctvrta",0,"ctvrta_zakazka",clBlue,50,2.0,j,30,0,0);
	d.v.vloz_temp_zakazku("pata",0,"pata_zakazka",clBlue,50,2.0,j,30,0,0);

	//zkouška editace
	d.v.edituj_temp_zakazku(2,"druha_edit",0,"druha_zakazka_edit",clBlue,50,2.0,30,0,0);

	//zkouška výpisu ZAKAZKY_temp
	Memo2->Clear();
	Memo2->Visible=true;
	Cvektory::TZakazka *ukaz=d.v.ZAKAZKY_temp;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	int i=0;
	while (ukaz!=NULL)
	{
		if(i==0)Memo2->Lines->Add(AnsiString(i++)+": "+AnsiString(ukaz->n)+" "+ukaz->name+" /"+AnsiString(ukaz->predchozi->name));
		else
		Memo2->Lines->Add(AnsiString(i++)+": "+AnsiString(ukaz->n)+" "+ukaz->name);//akce s ukazatelem
		ukaz=ukaz->dalsi;//posun na další prvek v seznamu
	}

	//zkouška změny pořadí
	Memo2->Lines->Add("zmena poradi:");
	//d.v.zmen_poradi_temp_zakazky(Edit1->Text.ToInt(),Edit2->Text.ToInt());
	d.v.zmen_poradi_temp_zakazky(5,3);
	ukaz=d.v.ZAKAZKY_temp;
	i=0;
	while (ukaz!=NULL)
	{
		if(i==0)Memo2->Lines->Add(AnsiString(i++)+": "+AnsiString(ukaz->n)+" "+ukaz->name+" /"+AnsiString(ukaz->predchozi->name));
		else
		Memo2->Lines->Add(AnsiString(i++)+": "+AnsiString(ukaz->n)+" "+ukaz->name);//akce s ukazatelem
		ukaz=ukaz->dalsi;//posun na další prvek v seznamu
	}

	//zkouška uložení do ZAKAZKY
	d.v.kopirujZAKAZKY_temp2ZAKAZKY();
	Memo2->Lines->Add("ZAKAZKY:");
	ukaz=d.v.ZAKAZKY->dalsi;
	while (ukaz!=NULL)
	{
		Memo2->Lines->Add(AnsiString(ukaz->n)+" "+ukaz->name+" /"+AnsiString(ukaz->predchozi->name)+" "+ukaz->jig.vyska);//akce s ukazatelem
		ukaz=ukaz->dalsi;//posun na další prvek v seznamu
	}
	Memo2->Lines->Add("ZAKAZKY_temp:");
	if(d.v.ZAKAZKY_temp!=NULL)
	{
		ukaz=d.v.ZAKAZKY_temp->dalsi;
		while (ukaz!=NULL)
		{
			Memo2->Lines->Add(AnsiString(ukaz->n)+" "+ukaz->name);//akce s ukazatelem
			ukaz=ukaz->dalsi;//posun na další prvek v seznamu
		}
	}
	else Memo2->Lines->Add("prázdný");

	//zkouška vozíky výpis
	d.v.generuj_VOZIKY();
	Memo2->Lines->Add("Vozíky:");
	Cvektory::TVozik *v=d.v.VOZIKY->dalsi;
	while (v!=NULL)
	{
			Memo2->Lines->Add(AnsiString(v->n)+" "+v->zakazka->n);//akce s ukazatelem
			v=v->dalsi;//posun na další prvek v seznamu
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm1::zobraz_tip(UnicodeString text)
{
	Canvas->Font->Color=m.clIntensive(clRed,100);
 	//SetBkMode(Canvas->Handle,TRANSPARENT);//nastvení netransparentního pozadí
	Canvas->Font->Size=14;
	Canvas->Font->Name="Arial";
  Canvas->Brush->Color=clWhite;
	Canvas->Font->Style = TFontStyles();//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	Canvas->TextOutW(ClientWidth-Canvas->TextWidth(text)-10,Form1->scGPPanel_statusbar->Top-25,text);
	Canvas->Font->Color=clBlack;
}
//---------------------------------------------------------------------------
void TForm1::akutalizace_stavu_prichytavani_vSB()
{
		switch(prichytavat_k_mrizce)
		{
			case 0:SB("přichytávat?",5);grid=true;myMessageBox->CheckBox_pamatovat->Checked=false;break;
			case 1:SB("přichytávat",5);grid=true;break;//přichytí automaticky
			case 2:SB("nepřichytávat",5);break;//automaticky nepřichyt
		}
}
//---------------------------------------------------------------------------
//přepínání dotazu, přichytávání, nepřychtávání k mřížce
void __fastcall TForm1::RzStatusPane5Click(TObject *Sender)
{
		if(grid)//aby se po zobrazení mřížky zachoval stavající stav,tedy nebude zohledněn další if
		if(++prichytavat_k_mrizce==3)prichytavat_k_mrizce=0;

		akutalizace_stavu_prichytavani_vSB();
		Invalidate();
}
//---------------------------------------------------------------------------
//přepíná mody zobrazení
void __fastcall TForm1::RzStatusPane1Click(TObject *Sender)
{
		switch(MOD)
		{
			case 0:schemaClick(Sender);break;
			case 1:testovnkapacity1Click(Sender);break;
			case 2:casoverezervy1Click(Sender);break;
			case 3:simulace1Click(Sender);break;
			case 4:schemaClick(Sender);break;
		}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Zavreni programu volbami Konec - ALT+F4 z menu, odchytáváno včetně všeho dalšího pomocí metoda FormCloseQuery
void __fastcall TForm1::Konec1Click(TObject *Sender)
{
 Close();//ukončí aplikaci
}
//---------------------------------------------------------------------------
//Zavreni programu volbou krizek + reaguje na reakci zavirani formulare pri Konec1Click
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if(duvod_k_ulozeni)
	{
		AnsiString FNs=FileName_short(FileName);
		if(FNs=="")FNs="Nový.tispl";
		int result=MB(FNs+" byl změněn.\nChcete ho před ukončením uložit?",MB_YESNOCANCEL);
		switch(result)
		{
			case mrYes:     UlozitClick(this); if(!stisknuto_storno){/*ulozit_posledni_otevreny();*/ vse_odstranit(); CanClose=true;}else CanClose=false; break;
			case mrNo:      ulozit_posledni_otevreny(); vse_odstranit();CanClose=true; break;
			case mrCancel:  CanClose=false; break;
		}
	}
	else
	{
		if(nastaveni.posledni_file)ulozit_posledni_otevreny();
		vse_odstranit();
		CanClose=true;
	}

  //v případě uzavírání aplikace
	if(CanClose)
	{
		log2web("konec");
		//pro ochranu v případě pádu programu
		//TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
		writeINI("Konec","status","OK");
		//zápis dalšího nastavení
		writeINI("Nastaveni_app","prichytavat",prichytavat_k_mrizce);
		writeINI("Nastaveni_app","ortogonalizace",(short)ortogonalizace_stav);
		//zatím nepoužíváme writeINI("Nastaveni_app","status",STATUS);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Smazat1Click(TObject *Sender)
{
	switch (MOD)
	{
		case NAHLED:
		{
			if (pom_element_smazat!=NULL) pom_element=pom_element_smazat;
			if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu smazat \""+pom_element->name.UpperCase()+"\"?","",MB_YESNO))
			{
				d.v.smaz_element(pom_element);
				pom_element=NULL;
				Akce=NIC;
				REFRESH();
				DuvodUlozit(true);
			}
			pom_element_smazat=NULL; delete pom_element_smazat;
			break;
		}
		default:
		{
			//ať to nemusí znovu hledat beru z pom Cvektory::TObjekt *p=d.v.najdi_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
			if(pom!=NULL)//pokud byl prvek nalezen
			{
				Cvektory::TZakazka *Z=d.v.obsahuje_segment_cesty_objekt(pom);
				if(Z!=NULL)
					MB("Nelze smazat objekt, který je součástí technologické cesty zakázky např.: "+UnicodeString(Z->name));
				else
				{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu objekt \""+pom->name.UpperCase()+"\" smazat?","",MB_YESNO))
				{
					d.v.smaz_objekt(pom);//nalezeny můžeme odstranit odstranit
					d.v.sniz_indexy(pom);
					pom=NULL;//delete p; nepoužívat delete je to ukazatel na ostra data
					REFRESH();
					DuvodUlozit(true);
				}
				}
			}
			else
			S("nenalezen");
			break;
		}

	}
}
//---------------------------------------------------------------------------
//zobrazí paramety jednoho procesu na časových osách
void __fastcall TForm1::Zobrazitparametry1Click(TObject *Sender)
{
	double prozatim_delka_voziku=m.UDV(0);//doplnit rotaci, bylo zde Form1->d.v.PP.delka_jigu;
	AnsiString rezim="";
	AnsiString delka="v tuto chvíli neznamá";
	AnsiString delka_dop=delka;
	switch(proces_pom->segment_cesty->objekt->rezim)
	{
			case 0:rezim="stop & go";break;
			case 1:rezim="kontinuální";delka=proces_pom->segment_cesty->RD*proces_pom->segment_cesty->CT;delka_dop=delka;break;
			case 2:
				rezim="postprocesní";
				delka=proces_pom->segment_cesty->objekt->kapacita*prozatim_delka_voziku;
				delka_dop=proces_pom->segment_cesty->objekt->kapacita_dop*prozatim_delka_voziku;
			break;
	}

	Form_osa_info->rHTMLLabel_nazev_vypis->Caption=proces_pom->segment_cesty->objekt->name;
	Form_osa_info->rHTMLLabel_ct_vypis->Caption=proces_pom->segment_cesty->CT;
	Form_osa_info->rHTMLLabel_cislo_voziku_vypis->Caption=proces_pom->n_v_zakazce;
	Form_osa_info->rHTMLLabel_rezim_vypis->Caption=rezim;
	Form_osa_info->rHTMLLabel_dop_kap_vypis->Caption=proces_pom->segment_cesty->objekt->kapacita_dop;
	Form_osa_info->rHTMLLabel_pozad_kap_vypis->Caption=proces_pom->segment_cesty->objekt->kapacita;
	Form_osa_info->rHTMLLabel_rd_vypis->Caption=proces_pom->segment_cesty->RD*60.0;
	Form_osa_info->rHTMLLabel_zkratka_vypis->Caption=proces_pom->segment_cesty->objekt->short_name;

	if(proces_pom->segment_cesty->objekt->pohon)
	{
		Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption=AnsiString(m.cekani_na_palec(0,proces_pom->segment_cesty->objekt->pohon->roztec,proces_pom->segment_cesty->RD,1))+" [sec]";
		Form_osa_info->rHTMLLabel_nahodna_doba_cekani_vypis->Caption=AnsiString(proces_pom->Trand)+" [sec] - abs. čas na časové ose";
		Form_osa_info->rHTMLLabel_max_doba_cekani_vypis->Caption=AnsiString(m.cekani_na_palec(0,proces_pom->segment_cesty->objekt->pohon->roztec,proces_pom->segment_cesty->RD,3))+" [sec]";
		Form_osa_info->rHTMLLabel_palce_vypis->Caption=AnsiString(proces_pom->segment_cesty->objekt->pohon->roztec) +" [m]";
	}
	else
	{
		Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption="neznámá - nepřiřazen pohon";
		Form_osa_info->rHTMLLabel_nahodna_doba_cekani_vypis->Caption=Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption;
		Form_osa_info->rHTMLLabel_max_doba_cekani_vypis->Caption=Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption;
		Form_osa_info->rHTMLLabel_palce_vypis->Caption=Form_osa_info->rHTMLLabel_max_doba_cekani_vypis->Caption;
	}

	UnicodeString Tpoc=proces_pom->Tpoc;
	UnicodeString Tkon=proces_pom->Tkon;
	UnicodeString Tdor=proces_pom->Tdor;
	UnicodeString Tpre=proces_pom->Tpre;
	UnicodeString Tcek=proces_pom->Tcek;


	Tpoc=Tpoc.SubString(1,5);
	Tkon=Tkon.SubString(1,5);
	Tdor=Tdor.SubString(1,5);
	Tpre=Tpre.SubString(1,5);
	Tcek=Tcek.SubString(1,5);

	Form_osa_info->rHTMLLabel_tpoc_vypis->Caption=Tpoc;
	Form_osa_info->rHTMLLabel_tkon_vypis->Caption=Tkon;
	Form_osa_info->rHTMLLabel_tdor_vypis->Caption=Tdor;
	Form_osa_info->rHTMLLabel_tpre_vypis->Caption=Tpre;
	Form_osa_info->rHTMLLabel_tcek_vypis->Caption=Tcek;

	Form_osa_info->rHTMLLabel_pt_vypis->Caption=proces_pom->Tkon-proces_pom->Tpoc;
	Form_osa_info->rHTMLLabel_mt_vypis->Caption=proces_pom->Tpre-proces_pom->Tkon;
	Form_osa_info->rHTMLLabel_wt_vypis->Caption=proces_pom->Tcek-proces_pom->Tpre;
	Form_osa_info->rHTMLLabel_CT_n_vypis->Caption=proces_pom->Tcek-proces_pom->Tpoc;

	//pozice
	Form_osa_info->Left=ClientWidth/2-Form_osa_info->Width/2;
	Form_osa_info->Top=ClientHeight/2-Form_osa_info->Height/2;
	//samotné volání modálního okna
	Form_osa_info->ShowModal();

//
//	S(/*"n_procesu: "+AnsiString(proces_pom->n)+*/
//	"číslo vozíku v zakázce: "+AnsiString(proces_pom->n_v_zakazce)+
//	"\nnázev: "+AnsiString(proces_pom->segment_cesty->objekt->name)+
//	"\nzkratka: "+AnsiString(proces_pom->segment_cesty->objekt->short_name)+
//	"\nrežim: "+AnsiString(rezim)+
//	"\nrychlost dopravníku: "+AnsiString(proces_pom->segment_cesty->RD)+" m/min"+
//	"\nrozteč palců: "+AnsiString(proces_pom->segment_cesty->objekt->pohon->roztec)+" mm"+
//	"\nstřední hodnota doby čekání na palec: "+AnsiString(m.cekani_na_palec(0,proces_pom->segment_cesty->objekt->pohon->roztec,proces_pom->segment_cesty->RD,1))+" min"+
//	"\nmax. hodnota doby čekání na palec: "+AnsiString(proces_pom->segment_cesty->objekt->pohon->roztec/1000.0/proces_pom->segment_cesty->RD)+" min"+
//	"\nTpoc: "+AnsiString(proces_pom->Tpoc)+" | Tkon: "+AnsiString(proces_pom->Tkon)+" | Tdor: "+AnsiString(proces_pom->Tdor)+" | Tpre: "+AnsiString(proces_pom->Tpre)+" | Tcek: "+AnsiString(proces_pom->Tcek)+
//	"\nPT: "+AnsiString(proces_pom->Tkon-proces_pom->Tpoc)+" min"+
//	"\nMT: "+AnsiString(proces_pom->Tpre-proces_pom->Tkon)+" min"+
//	"\nWT: "+AnsiString(proces_pom->Tcek-proces_pom->Tpre)+" min"+
//	"\nCT: "+AnsiString(proces_pom->Tcek-proces_pom->Tpoc)+" min"+
//	"\nzadaný CT: "+AnsiString(proces_pom->segment_cesty->CT)+" min"+
//	"\ndoporučená kapacita: "+AnsiString(proces_pom->segment_cesty->objekt->kapacita_dop)+", délka objektu: "+delka_dop+" m"+
//	"\npožadovaná kapacita: "+AnsiString(proces_pom->segment_cesty->objekt->kapacita)+	", délka objektu: "+delka+" m");

}
//---------------------------------------------------------------------------
//podpůrná metoda řeší vstupní část dat, vyseparováno, z důvodu toho, že z GAPO aktulizauji případně spuštěné PO a nemohu volat NP, protože to v sobě obsahu ShowModal - vedlo k chybě, nutno řešit převody jednotek
void TForm1::NPin()
{
	short CTunit=0;if(Form1->readINI("nastaveni_form_parametry", "CT") == "1") CTunit=1;
	short RDunit=0;if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1")RDunit=1;
	short DDunit=0;if(Form1->readINI("nastaveni_form_parametry", "DD") == "1") DDunit=1;
	short Munit=0;if(Form1->readINI("nastaveni_form_parametry", "DM") == "1")  Munit=1;
	//////////////////////////plnění dat do formu z daného objektu
	////plnění daty
	Form_parametry->scComboBox_rotace->ItemIndex=pom->rotace;//musí být nad aktualizací
	aktualizace_combobox_pohony_v_PO();
	if(pom->pohon!=NULL)Form_parametry->scComboBox_pohon->ItemIndex=pom->pohon->n;else Form_parametry->scComboBox_pohon->ItemIndex=0;//musí být takto separé, protože metoda se volá z více míst
	//předání hodnoty objektů ze souboru resp. strukutry do Form_Parametry v SI jednotkách
	Form_parametry->input_state=0;//zakázání akcí vyplývající ze změny editů
	Form_parametry->input_clicked_edit=0; //při načítání dat není kliknuto na žádný editbox
	//název
	Form_parametry->scGPEdit_name->Text=pom->name;
	Form_parametry->scGPEdit_shortname->Text=pom->short_name;
	//režim
	Form_parametry->scComboBox_rezim->ItemIndex=pom->rezim;
	//CT
	Form_parametry->scGPNumericEdit_CT->Value=pom->CT/(1+59.0*CTunit);
	//RD
	Form_parametry->scGPNumericEdit_RD->Value=pom->RD*(1+59.0*RDunit);
	//DD
	Form_parametry->scGPNumericEdit_delka_dopravniku->Value=pom->delka_dopravniku*(1+999*DDunit);
	//MEZERY
	Form_parametry->scGPNumericEdit_mezera->Value=pom->mezera*(1+999*Munit);
	Form_parametry->scGPNumericEdit_mezera_JIG->Value=pom->mezera_jig*(1+999*Munit);
	Form_parametry->scGPNumericEdit_mezera_PODVOZEK->Value=pom->mezera_podvozek*(1+999*Munit);
	//R-záležitosti
	if(pom->pohon!=NULL)
	{
		Form_parametry->scGPNumericEdit_rx->Value=pom->pohon->Rx;
		Form_parametry->scGPNumericEdit_rozestup->Value=pom->pohon->Rz*(1+999*Munit);
	}
	//ostatni
	Form_parametry->scComboBox_cekani_palec->ItemIndex=pom->cekat_na_palce;
	Form_parametry->scGPNumericEdit_kapacita->Value=pom->kapacita;
	Form_parametry->scGPNumericEdit_pozice->Value=pom->pozice;
	Form_parametry->scGPNumericEdit_odchylka->Value=pom->odchylka;
	Form_parametry->scComboBox_stopka->ItemIndex=pom->stopka;
	Form_parametry->poznamka=pom->poznamka;

	//nadesignování formu podle právě vypisováných hodnot
	Form_parametry->vypis("");
	Form_parametry->setForm4Rezim(pom->rezim);
}
//---------------------------------------------------------------------------
//volá form na nastevení parametrů, dřívější nastavparametry1click, převody jednotek se řeší při formshow formu
void TForm1::NP()
{
	if(pom!=NULL)
	{
		//////////////////////////plnění dat do formu z daného objektu
		////plnění daty
		Form_parametry->scComboBox_rotace->ItemIndex=pom->rotace;//musí být nad aktualizací
		aktualizace_combobox_pohony_v_PO();
		if(pom->pohon!=NULL)Form_parametry->scComboBox_pohon->ItemIndex=pom->pohon->n;else Form_parametry->scComboBox_pohon->ItemIndex=0;//musí být takto separé, protože metoda se volá z více míst
		//předání hodnoty objektů ze souboru resp. strukutry do Form_Parametry v SI jednotkách
		Form_parametry->input_state=0;//zakázání akcí vyplývající ze změny editů
		Form_parametry->input_clicked_edit=0; //při načítání dat není kliknuto na žádný editbox
		//název
		Form_parametry->scGPEdit_name->Text=pom->name;
		Form_parametry->scGPEdit_shortname->Text=pom->short_name;
		//režim
		Form_parametry->scComboBox_rezim->ItemIndex=pom->rezim;
		//CT
		Form_parametry->scGPNumericEdit_CT->Value=pom->CT;
		//RD
		Form_parametry->scGPNumericEdit_RD->Value=pom->RD;
		//DD
		Form_parametry->scGPNumericEdit_delka_dopravniku->Value=pom->delka_dopravniku;
		//MEZERY
		Form_parametry->scGPNumericEdit_mezera->Value=pom->mezera;
		Form_parametry->scGPNumericEdit_mezera_JIG->Value=pom->mezera_jig;
		Form_parametry->scGPNumericEdit_mezera_PODVOZEK->Value=pom->mezera_podvozek;
		//R-záležitosti
		if(pom->pohon!=NULL)
		{
			Form_parametry->scGPNumericEdit_rx->Value=pom->pohon->Rx;
			Form_parametry->scGPNumericEdit_rozestup->Value=pom->pohon->Rz;
		}
		//ostatni
		Form_parametry->scComboBox_cekani_palec->ItemIndex=pom->cekat_na_palce;
		Form_parametry->scGPNumericEdit_kapacita->Value=pom->kapacita;
		Form_parametry->scGPNumericEdit_pozice->Value=pom->pozice;
		Form_parametry->scGPNumericEdit_odchylka->Value=pom->odchylka;
		Form_parametry->scComboBox_stopka->ItemIndex=pom->stopka;
		Form_parametry->poznamka=pom->poznamka;

		//nadesignování formu podle právě vypisováných hodnot
		Form_parametry->vypis("");
		Form_parametry->setForm4Rezim(pom->rezim);

		//ošetření aby zůstal dialog na monitoru
		if(akt_souradnice_kurzoru_PX.x+10+Form_parametry->ClientWidth<Form1->ClientWidth)
			Form_parametry->Left=akt_souradnice_kurzoru_PX.x+10;
		else
			Form_parametry->Left=Form1->ClientWidth-Form_parametry->ClientWidth-10;
		if(akt_souradnice_kurzoru_PX.y+10+Form_parametry->ClientHeight<Form1->ClientHeight)
			Form_parametry->Top=akt_souradnice_kurzoru_PX.y+10;
		else
			Form_parametry->Top=Form1->ClientHeight-Form_parametry->ClientHeight-scGPPanel_statusbar->ClientHeight-10;

		//nastevní titulku
		Form_parametry->scLabel_titulek->Caption=pom->name.UpperCase()+" - parametry";

		//////////////////////////navrácení dat + volání zobrazení formu
		if(Form_parametry->ShowModal()==mrOk)
		{
			try
			{
				//navrácení hodnot z Form_Parametry, v případě stisku OK
				double jednotky_cas=60.0;double jednotky_vzdalenost=1000.0;
				pom->name=Form_parametry->scGPEdit_name->Text;
				pom->short_name=Form_parametry->scGPEdit_shortname->Text;
				pom->pohon=d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex);//indexuje se od nuly,ale pohony od 1 (nicméně nově je na prvním místě položka nepřiřazen), pokud pohon neexituje vráti null
				pom->rezim=Form_parametry->scComboBox_rezim->ItemIndex;
				pom->cekat_na_palce=Form_parametry->scComboBox_cekani_palec->ItemIndex;
				pom->kapacita=Form_parametry->scGPNumericEdit_kapacita->Value;
				pom->pozice=Form_parametry->scGPNumericEdit_pozice->Value;
				pom->odchylka=Form_parametry->scGPNumericEdit_odchylka->Value;
				pom->stopka=Form_parametry->scComboBox_stopka->ItemIndex;
				//DD
				if(Form_parametry->DDunit==Form_parametry->MM)jednotky_vzdalenost=1000.0;else jednotky_vzdalenost=1.0;
				pom->delka_dopravniku=Form_parametry->scGPNumericEdit_delka_dopravniku->Value/jednotky_vzdalenost;
				//RD
				if(Form_parametry->RDunitT==Form_parametry->MIN)jednotky_cas=60.0;else jednotky_cas=1.0;
				//if(Form_parametry->RDunitD==Form_parametry->MM)jednotky_vzdalenost=1000.0;else jednotky_vzdalenost=1.0;
				pom->RD=Form_parametry->scGPNumericEdit_RD->Value/jednotky_cas/jednotky_vzdalenost;
				if(pom->pohon!=NULL && (pom->rezim==1 || pom->pohon->aRD==0))pom->pohon->aRD=pom->RD;//uloží aktulání rychlost pohonu, pokud se jedná o kontinuální režim nebo nebyla ješt rychlost nastavena
				//MEZERY
				if(Form_parametry->DMunit==Form_parametry->MM)jednotky_vzdalenost=1000.0;else jednotky_vzdalenost=1.0;
				pom->mezera=Form_parametry->scGPNumericEdit_mezera->Value/jednotky_vzdalenost;
				pom->mezera_jig=Form_parametry->scGPNumericEdit_mezera_JIG->Value/jednotky_vzdalenost;
				pom->mezera_podvozek=Form_parametry->scGPNumericEdit_mezera_PODVOZEK->Value/jednotky_vzdalenost;
				//Rz,Rx
				pom->rotace=Form_parametry->scComboBox_rotace->ItemIndex;
				if(Form_parametry->scComboBox_pohon->ItemIndex!=0 && pom->rezim==1)//pouze pokud je prirazen pohon a jedná se o KK režim, tak ulozim do nej hodnoty Rx,Rz
				{
					pom->pohon->Rx=Form_parametry->scGPNumericEdit_rx->Value;
					pom->pohon->Rz=Form_parametry->scGPNumericEdit_rozestup->Value/(1+999*Form_parametry->DMunit);
				}
				//Poznámka
				pom->poznamka=Form_parametry->poznamka;
				//CT
				if(Form_parametry->CTunit==Form_parametry->MIN)jednotky_cas=60.0;else jednotky_cas=1.0;
				if(Form_parametry->kapacitaSG>1 && pom->rezim==0)//pokud je požadovaný rozklad objektu na více objektů, pouze u S&G
				{
						pom->CT=Form_parametry->scGPNumericEdit_CT->Value/Form_parametry->kapacitaSG*jednotky_cas;//navrácení správného CT
						Cvektory::TObjekt *cop=new Cvektory::TObjekt;cop=NULL;
						short N=(int)!ortogonalizace_stav;//pokud je ortogonalizeace aktivní tak N=1, zajištuje, aby se vložilo ortogonalizovaně
						for(unsigned int i=2;i<=Form_parametry->kapacitaSG;i++)
						{
							if(cop==NULL)//kopíruje za originál  //pokud je ortoganalizace zapnuta bude odsazení nových objektů větší a algoritmus objekty rovná jen po X ose
							cop=d.v.kopiruj_objekt(pom,(3+3*N)*(i-1),-6*(i-1)*N,i,false,pom);//zkopíruje objekt do totožných objektů odsazených o 20m vertikálně i horizonátlně
							else //vkládá za předchozí kopii, aby bylo řazeno orig,1,2,n nikoliv n,2,1,orig
							cop=d.v.kopiruj_objekt(pom,(3+3*N)*(i-1),-6*(i-1)*N,i,false,cop);//zkopíruje objekt do totožných objektů odsazených o 20m vertikálně i horizonátlně
						}
						cop=NULL;delete cop;
						pom->name+="1";pom->short_name+="1";//oindexuje i název origánálu, musí být na závěr
						ortogonalizace();
				}
				else //pro kontinuál a postprocesní
				{
				 pom->CT=Form_parametry->scGPNumericEdit_CT->Value*jednotky_cas;
				}
				//AKTUALIZACE hodnot první zakázky pokud již existuje, to samé akorát obráceně (z objektů do zakazky) probíhá v při načtení SF - definice zakázek, z určitého pohledu se jedná o duplicitní algoritmus, ale v případě aktualizací jinak než přes parametry objektu lze považovat za nutnost
				if(d.v.ZAKAZKY->dalsi!=NULL)
				{
					Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;
					while(O!=NULL)//prochází všechny objekty a buď je ("Ano") objekt i na cestě nebo není ("Ne")
					{
						Cvektory::TCesta *C=Form1->d.v.obsahuje_segment_cesty_objekt(O,d.v.ZAKAZKY->dalsi);
						if(C!=NULL)//objekt je segmentem cesty
						{
							C->CT=O->CT;C->RD=O->RD;//v případě první zakázky se berou hodnoty z parametrů objektu nikoliv zakázky, což zajistí patřičnou aktuliazaci
							C->objekt->pohon=O->pohon;
						}
						O=O->dalsi;
						C=NULL;delete C;
					}
					delete O;
					//aktualizace a přepočet kvůli časovým osám a techn.procesům(roma)
					aktualizace_maro_a_roma();//aktualizace a přepočet hodnot volaná kvůli časovým osám (maro) a techn.procesům(roma)
				}
				DuvodUlozit(true);
				REFRESH();
			}
			catch(...)
			{
				if(mrOk==MB("Neplatná hodnota!"))//lepé přes mrOk
				NP();//nové zadání, //volá form na nastevení parametrů, dřívější nastavparemetry1click
			}
		}
		if(Form_objekt_nahled->Visible)//pokud je případně zobrazen náhled objektu
		{
			Zoom=Form_objekt_nahled->Zoom_predchozi;//navrátí schématu původní správný zoom
			Form_objekt_nahled->Visible=false;//tak skryje
			nahled_objektu=false;
			REFRESH();
		}
		Form_parametry->form_zobrazen=false;//detekuje zda je form aktuálně zobrazen, slouží proto aby při změně combo režim pokud si nastavil uživatel formulař jinam, aby zůstal nastaven dle uživatele
	}
}
//---------------------------------------------------------------------------
void TForm1::NP_input()
{
	 MOD=NAHLED;

	 //založení pomocného tempového ukazatele pro akutálně editovaný objekt a překopírování jeho atributů
	 pom_temp=new Cvektory::TObjekt; pom_temp->elementy=NULL;
	 //zkopíruje atributy objektu bez ukazatelového propojení, kopírování proběhne včetně spojového seznamu elemementu opět bez ukazatelového propojení s originálem, pouze mGrid je propojen
	 d.v.kopiruj_objekt(pom,pom_temp);//pokud elementy existují nakopíruje je do pomocného nezávislého spojáku pomocného objektu

	 ////řešení nového zoomu a posunu obrazu pro účely náhldeu
	 //zazálohování hodnot posunu a zoomu
	 Posun_predchozi2=Posun_predchozi=Posun;
	 Zoom_predchozi2=Zoom_predchozi=Zoom;
	 //nastavení zoomu na vhodný náhled
	 Zoom=5.0;
	 probehl_zoom=true;
	 zneplatnit_minulesouradnice();
	 Posun.x=m.round(pom->X/m2px-(ClientWidth+scSplitView_LEFTTOOLBAR->Width)/2/Zoom);
	 Posun.y=m.round(-pom->Y/m2px-(ClientHeight)/2/Zoom);
	 on_change_zoom_change_scGPTrackBar();
	 //vycentruje kurzor na střed monitoru - na X nefunguje přesně
	 if(vycentrovat)Mouse->CursorPos=TPoint(m.L2Px(akt_souradnice_kurzoru.x),m.L2Py(akt_souradnice_kurzoru.y)+vyska_menu);
	 vycentrovat=true;

	 DrawGrid_knihovna->DefaultRowHeight=140;
	 DrawGrid_knihovna->DefaultColWidth=80;
	 DrawGrid_knihovna->Left=3;
	 DrawGrid_knihovna->Height=DrawGrid_knihovna->DefaultRowHeight*2; // dle počtu řádků
	 DrawGrid_knihovna->Invalidate();

	 DrawGrid_otoce->DefaultColWidth=80;

	 scGPLabel_roboti->Visible=true;
	 scGPLabel_otoce->Visible=true;
	 scGPLabel_stop->Visible=true;
	 scGPLabel_geometrie->Visible=true;
	 scGPLabel_poznamky->Visible=true;

	 scGPLabel_roboti->Top=scGPPanel_mainmenu->Height;

	 scListGroupPanel_hlavickaOtoce->Visible=true;
	 scListGroupPanel_hlavickaOtoce->Top=DrawGrid_knihovna->Height + scGPPanel_mainmenu->Height;
	 scGPLabel_otoce->Top = scListGroupPanel_hlavickaOtoce->Top  + scGPPanel_mainmenu->Height;//scListGroupPanel_hlavickaOtoce->Top + scGPPanel_mainmenu->Height;
	 DrawGrid_otoce->Visible=true;

	 scListGroupPanel_hlavickaOstatni->Visible=true;
	 scListGroupPanel_hlavickaOstatni->Top=scListGroupPanel_hlavickaOtoce->Top + scListGroupPanel_hlavickaOtoce->Height;
	 scGPLabel_stop->Top=scListGroupPanel_hlavickaOstatni->Top + scGPPanel_mainmenu->Height;
	 DrawGrid_ostatni->Visible=true;

	 scListGroupPanel_geometrie->Visible=true;
	 scListGroupPanel_geometrie->Top=scListGroupPanel_hlavickaOstatni->Top +   scListGroupPanel_hlavickaOstatni->Height;
	 scGPLabel_geometrie->Top=scListGroupPanel_geometrie->Top + scGPPanel_mainmenu->Height;
	 DrawGrid_geometrie->Visible=true;

	 scListGroupPanel_poznamky->Visible=true;
	 scListGroupPanel_poznamky->Top= scListGroupPanel_geometrie->Top + scListGroupPanel_geometrie->Height;
	 scGPLabel_poznamky->Top=scListGroupPanel_poznamky->Top + scGPPanel_mainmenu->Height;
	 DrawGrid_poznamky->Visible=true;

	 //musí být nastaven i zde, protože se tento label používá jak ve schematu tak i zde
	 scGPLabel_roboti->Font->Style =  TFontStyles() << fsBold;
	 scGPLabel_roboti->Visible=true;
	 scGPLabel_roboti->Caption="Roboti";
	 scGPLabel_roboti->ContentMarginLeft=10;

//	 scGPButton_OK->Visible=true;
//	 scGPButton_storno->Visible=true;

	 //matamaticky exaktní napozicování tlačítek OK a storno
	 Form1->m.designButton(scGPButton_OK, Form1, 1, 2);
	 Form1->m.designButton(scGPButton_storno, Form1, 2, 2);

	 //prozatim definice kabiny
	 pom_temp->rozmer_kabiny.x=10;//default délka 10 m
	 pom_temp->rozmer_kabiny.y=6;//default šířka 6 m
	 pom_temp->Xk=m.P2Lx(scSplitView_LEFTTOOLBAR->Width+100);pom_temp->Yk=m.P2Ly((ClientHeight-F->scGPPanel_statusbar->Height-F->scLabel_titulek->Height)/2.0)+pom_temp->rozmer_kabiny.y/2.0;//provizorní vložení

	 //zapnutí spodního panelu
	 scGPPanel_bottomtoolbar->Visible=true;
	 //zmena horní lišty
	 scGPPanel_mainmenu->Visible=false;
	 scGPPanel_schema->Visible=true;

	 Invalidate();
	 REFRESH();
}
//---------------------------------------------------------------------------
//zaktualizuje ve formuláři parametry objektů combobox na výpis pohonů včetně jednotek uvedeného rozmezí rychlostí, pokud jsou zanechané implicitní parametry short RDunitD=-1,short RDunitT=-1, je načteno nastevní jednotek z INI aplikace pro form parametry objektu, v případech, kdy uvedené parametry nejsou dané hodnotou -1, tak se uvažují jednotky dle S==0,MIN==1 pro RDunitT, resp. M==0,MM==1 pro RDunitD
void TForm1::aktualizace_combobox_pohony_v_PO(short RDunitD,short RDunitT)
{
		Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;//ukazatel na pohony, přeskakuje hlavičku, která je již vytvořena
		Form_parametry->scComboBox_pohon->Items->Clear();//smazání původního obsahu
		TscGPListBoxItem *t=NULL;
		if(P==NULL)//pokud neexitustuje žádný pohon
		{      //Form_parametry->scComboBox_pohon->Items->
			t=Form_parametry->scComboBox_pohon->Items->Add(/*tady nelze parametr*/);
			t->Caption="nebyl nadefinován";
			Form_parametry->existuje_pohon=false;
			//nesmí tu být, způsobuje cyklení z důvodu vytoření onchange pohonu Form_parametry->scComboBox_pohon->ItemIndex=0;//pohon nedefinován
		}
		else//pokud existuje přidá na první pozici nabídku nepřiřazen dále začne plnit existujícím pohny
		{
			Form_parametry->existuje_pohon=true;

			//vytvoření položky nepřiřazen
			t=Form_parametry->scComboBox_pohon->Items->Add(/*tady nelze parametr*/);
			t->Caption="nepřiřazen";

			////příprava vypisovaných jednotek
			//čas
			double jednotky_cas_pohon=60.0;AnsiString Tcas="min";//tzn. min (ač 60 působí nelogicky)
			if(RDunitT==-1)
			{
				if(readINI("nastaveni_form_parametry","RDt")=="0"){jednotky_cas_pohon=1.0;Tcas="s";}//tzn. sec při načítání z ini
			}
			else
			{
				if(RDunitT==0){jednotky_cas_pohon=1.0;Tcas="s";}//tzn. sec z parametru
			}

			//délka
			double jednotky_delka_pohon=1000.0;AnsiString Td="mm";//tzn. mm
			if(RDunitD==-1)
			{
				if(readINI("nastaveni_form_parametry","RDd")=="0"){jednotky_delka_pohon=1.0;Td="m";}//tzn. m z ini
			}
			else
			{
				if(RDunitD==0){jednotky_delka_pohon=1.0;Td="m";}//tzn. m z parametru
			}
			UnicodeString caption_jednotky=Td+"/"+Tcas;

			//plnění existujícím pohony
			while (P!=NULL)
			{
				AnsiString dopRD="";
				t=Form_parametry->scComboBox_pohon->Items->Add(/*tady nelze parametr*/);
				if(Form_parametry->scComboBox_pohon->ItemIndex==P->n && !Form_parametry->pohon_pouzivan && !Form_parametry->ComboBoxPohonyProbihaZmena)//pokud se jedná o aktuální pohon a pohon lze editovat a neprobíhá změna comboboxu a již existuje nějaký pohon
				{     //pouze pro KK
					if(Form_parametry->scComboBox_rezim->ItemIndex==1 && m.round2double(Form_parametry->scGPNumericEdit_RD->Value/(1+59.0*RDunitT),8)==m.round2double(m.dopRD(d.v.PP.delka_jig,d.v.PP.sirka_jig,Form_parametry->scComboBox_rotace->ItemIndex,P->roztec,d.v.PP.TT,Form_parametry->scGPNumericEdit_RD->Value/(1+59.0*RDunitT)),8))dopRD="  dopor.";
					t->Caption=P->name+" - "+m.round2double(Form_parametry->scGPNumericEdit_RD->Value,3,"..")+" ("+AnsiString(m.round2double(P->rychlost_od*jednotky_cas_pohon*jednotky_delka_pohon,2))+"-"+AnsiString(m.round2double(P->rychlost_do*jednotky_cas_pohon*jednotky_delka_pohon,2))+") "+caption_jednotky+dopRD;//vypíše aktuální editovanou hodnotu
				}
				else//pro ostatní pohony, či aktální používaný (přiřazen více objektům) pohon - většinová situace
				{   //pouze pro KK
					if(Form_parametry->scComboBox_rezim->ItemIndex==1 && m.round2double(P->aRD,8)==m.round2double(m.dopRD(d.v.PP.delka_jig,d.v.PP.sirka_jig,Form_parametry->scComboBox_rotace->ItemIndex,P->roztec,d.v.PP.TT,P->aRD),8))dopRD="  dopor.";
					t->Caption=P->name+" - "+m.round2double(P->aRD*(1+59.0*RDunitT),3,"..")+" ("+AnsiString(m.round2double(P->rychlost_od*jednotky_cas_pohon*jednotky_delka_pohon,2))+"-"+AnsiString(m.round2double(P->rychlost_do*jednotky_cas_pohon*jednotky_delka_pohon,2))+") "+caption_jednotky+dopRD;
				}
				P=P->dalsi;//přesun na další pohon
			}
			//nastavení comba, pokud neexistuje pohon
			if(d.v.POHONY->dalsi==NULL)Form_parametry->scComboBox_pohon->ItemIndex=0;//nepřiřazen
		}
		Form_parametry->scComboBox_pohon->Refresh();
}
//---------------------------------------------------------------------------
//aktualizace a přepočet hodnot volaná kvůli časovým osám (maro) a techn.procesům(roma)
void TForm1::aktualizace_maro_a_roma()
{                //toto je v testování - prvni_zakazka.....
		d.v.prvni_zakazka_dle_schematu();//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji pouze přiřadí cestu dle schématu
		if(scGPCheckBox_pocet_voziku_dle_WIP->Checked)//pokud je aktulizace dle hodnoty WIP+1 povolena
		{
			short WIP=d.v.WIP(1);
			if(WIP+1!=d.v.VOZIKY->n && STATUS==NAVRH)//v případě, že je v návrháři a aktulní WIP neodpovídá počtu vozíku, zakatulizuje se i počet vozíku, resp. se vygenerují nové ve správném novém počtu (podmínka použita pouze pro šetření strojového času)
			{
				d.v.ZAKAZKY->dalsi->pocet_voziku=WIP+1;
				d.v.generuj_VOZIKY();
			}
		}
		d.JIZPOCITANO=false;//nutnost zakutalizovat časové osy
		if(MOD==TECHNOPROCESY)//v případě technologických procesů (ROMA) i jejich přepočítání
		{
			Graphics::TBitmap *bmp_temp=new Graphics::TBitmap;bmp_temp->Width=0;bmp_temp->Height=0;//aby se nevykreslovalo přímo do Form1->Canvasu a neproběhl problik časových os, bmp obcházím nutnost vykreslování do canvasu, protože mi jde jen o přepočet časových os, bohužel ve vykresli_casove osy je společně jak výpočet, tak vykreslování, což není z tohoto pohledu dobře (zachováno bylo z důvodu efektivity při vykreslování/výpočtu)
			d.vykresli_casove_osy(bmp_temp->Canvas);//u ROMA ještě nutno předtím zaktulizovat výpočet na časových osách
			g.ShowGrafy(false);
			bmp_temp=NULL; delete bmp_temp;
		}
}
//---------------------------------------------------------------------------
//pokud je označený objekt, zajistí jeho zkopírování, připočítá index 1,2,3
void TForm1::kopirovat_objekt()
{
		if(pom!=NULL)//pokud je vybraný objekt
		{
				bool remove_pre_index=true;
				if(pom->short_name=="CO2")remove_pre_index=false;//pokud se jedná CO2, tak aby nedával CO3, CO4 atp
				if(pom->dalsi!=NULL)//pokud po vybraném následuje další objekt, tak nový vkládá přesně mezi ně
				{
					d.v.kopiruj_objekt(pom,(pom->X+pom->dalsi->X)/2-pom->X,(pom->Y+pom->dalsi->Y)/2-pom->Y,ms.a2i(pom->short_name.SubString(pom->short_name.Length(),1))+1,remove_pre_index,pom);
				}
				else //jinak odsazeně
				{
					if(pom==d.v.OBJEKTY->predchozi && pom->n==1)//pokud je jenom jeden objekt
					d.v.kopiruj_objekt(pom,6,0,ms.a2i(pom->short_name.SubString(pom->short_name.Length(),1))+1,remove_pre_index);
					if(pom==d.v.OBJEKTY->predchozi)//pokud se jedná o poslední prvek
					d.v.kopiruj_objekt(pom,(pom->X+d.v.OBJEKTY->dalsi->X)/2-pom->X,(pom->Y+d.v.OBJEKTY->dalsi->Y)/2-pom->Y,ms.a2i(pom->short_name.SubString(pom->short_name.Length(),1))+1,remove_pre_index);
				}
				ortogonalizace();
				REFRESH();
				DuvodUlozit(true);
		}
}
//---------------------------------------------------------------------------
//volá dialog kalkulátor TT
void __fastcall TForm1::Button_kalkulatorTTClick(TObject *Sender)
{
//ZDM
//	Form_TT_kalkulator->Left=0;//akt_souradnice_kurzoru_PX.x+0;
//	Form_TT_kalkulator->Top=0+vyska_menu+RzToolbar1->Height;//akt_souradnice_kurzoru_PX.y+0;
//	Form_TT_kalkulator->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_takt_timeChange(TObject *Sender)
{
//	if(Edit_takt_time->Focused())
//	{
//		PP.TT=ms.MyToDouble(Edit_takt_time->Text);
//		if(Edit_takt_time->Text!="" && zobrazit_barvy_casovych_rezerv)//pokud má smysl překreslit
//		REFRESH();
//		DuvodUlozit(true);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_takt_timeEnter(TObject *Sender)
{
//	if(Edit_takt_time->Text!="" && Edit_takt_time->Text!=0 && upozornovat_na_zmenu_TT_parametru)
//	{
//		MyMessageBox->Button_Yes->Visible=false;
//		MyMessageBox->Button_No->Visible=false;
//		MyMessageBox->Button_OK->Visible=true;
//		MyMessageBox->CheckBox_pamatovat->Checked=false;
//		MyMessageBox->ShowMyMessageBox(0,0+vyska_menu+RzToolbar1->Height,"TISPL - Eltep","Při změně hodnoty Takt Timu, dojde ke změně parametrů, z kterých se Takt Time vypočítává!");
//		MyMessageBox->Button_Yes->Visible=true;
//		MyMessageBox->Button_No->Visible=true;
//		MyMessageBox->Button_OK->Visible=false;
//		if(MyMessageBox->CheckBox_pamatovat->Checked)upozornovat_na_zmenu_TT_parametru=false;
//		MyMessageBox->CheckBox_pamatovat->Checked=false;
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_pocet_vozikuChange(TObject *Sender)
{
	 //Edit_takt_timeChange(this);
	 DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//volá dialog k nastavení parametrů voziku
void __fastcall TForm1::Button_vozik_parametry_OLDClick(TObject *Sender)
{
//	if(d.v.VOZIKY->dalsi->cesta==NULL)
//	Button2Click(Sender);//zatím provizorní záležitost načtení pálnu výroby
	//ZDM
//	Form_vozik_nastaveni->Left=0;
//	Form_vozik_nastaveni->Top=0+vyska_menu+RzToolbar1->Height;
//	if(IDOK==Form_vozik_nastaveni->ShowModal())DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//volá dialog k nastavení dopravníků
void __fastcall TForm1::Button_dopravnik_parametry_OLDClick(TObject *Sender)
{
//ZDM
//	Form_dopravnik->Left=0;
//	Form_dopravnik->Top=0+vyska_menu+RzToolbar1->Height;
//	if(IDOK==Form_dopravnik->ShowModal())DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
AnsiString TForm1::FileName_short(AnsiString FileName)
{
	return ms.delete_repeat_all(FileName,"\\");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UlozitClick(TObject *Sender)
{
	if(FileName=="Nový.tispl" || FileName.Pos(".tisplTemp"))UlozitjakoClick(this);
	else
	{
		scSplitView_MENU->Opened=false;
		if(duvod_k_ulozeni)Ulozit_soubor();
		else SB("Soubor byl již uložen...");
	}
	scButton_ulozit->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UlozitjakoClick(TObject *Sender)
{
  scButton_ulozjako->Down=false;
	scSplitView_MENU->Opened=false;
	SaveDialog->Title="Uložit soubor jako";
	SaveDialog->DefaultExt="*.tispl";
	SaveDialog->Filter="Soubory formátu TISPL (*.tispl)|*.tispl|Soubory šablon TISPL (*.tisplTemp)|*.tisplTemp";
	SaveDialog->FileName=FileName;
	UnicodeString origFileName=FileName;
	if(SaveDialog->Execute())
	{
		FileName=SaveDialog->FileName;
		stisknuto_storno=false;
		duvod_k_ulozeni=true;
		Ulozit_soubor();
		if(FileName.Pos(".tisplTemp")){FileName=origFileName;DuvodUlozit(true);}
		scLabel_titulek->Caption=Caption+" - ["+FileName_short(FileName)+"]";
	}
	else//stisknuto storno
	stisknuto_storno=true;
}
//---------------------------------------------------------------------------
//samotné uložení
void TForm1::Ulozit_soubor()
{
		//zapis dat do souboru
		d.v.uloz_do_souboru(FileName);

		//nastavení komponent + výpis
		SB("Soubor uložen...");
		DuvodUlozit(false);



    //zakazuje UNDO
		//ToolButton_undo->Enabled=false;//přepne příslušně nabídky menu
		//Zpet->Enabled=false;//přepne příslušně nabídky menu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//otevře soubor
void __fastcall TForm1::Toolbar_OtevritClick(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
	scButton_otevrit->Down=false;
	if(duvod_k_ulozeni)//pokud existuje předcházejicí soubor, který je nutný uložit
	{
		AnsiString FNs=FileName_short(FileName);
		if(FNs=="")FNs="Nový.tispl";
		int result=MB(FNs+" byl změněn.\nChcete ho před ukončením uložit?",MB_YESNOCANCEL);
		switch(result)
		{
			case mrYes:     UlozitClick(this); if(!stisknuto_storno){Otevrit_soubor();}break;
			case mrNo:      DuvodUlozit(false); Otevrit_soubor(); break;
			case mrCancel:  break;
		}
	}
  else
	Otevrit_soubor();
}
//---------------------------------------------------------------------------
void TForm1::Otevrit_soubor()//realizuje otevření opendialogu s následným voláním realizace samotného otevření souboru
{
	OpenDialog1->Title="Otevřít soubor";
	OpenDialog1->DefaultExt="*.tispl";
	OpenDialog1->Filter="Soubory formátu TISPL (*.tispl)|*.tispl";
	if(OpenDialog1->Execute())
	{
		//zavolá nový soubor/smaže stávajicí
		Novy_soubor();

		//otevrení souboru
		Otevrit_soubor(OpenDialog1->FileName);
	}
}
//-------------------------------------------------------------------------
unsigned short int TForm1::Otevrit_soubor(UnicodeString soubor)//realizuje samotné otevření souboru
{
	//načte data ze souboru a reaguje na návratovou hodnotu
	switch(d.v.nacti_ze_souboru(soubor))
	{
		case 0://Soubor nebyl nalezen
		{
			zavrit_uvod();//v případě chybové situace, např. soubor nenalezen, nebo špatný formát souboru zavře úvodní dialog
			MB("Soubor "+FileName_short(FileName)+" nebyl nalezen!");
			//zavrit_uvod();
			FileName="Nový.tispl";
			return 0;
		}break;
		case 1://Soubor byl nalezen
		{
			FileName=soubor;//pro globální využití
			scLabel_titulek->Caption=Caption+" - ["+FileName_short(FileName)+"]";
			//načtení dat z hlavičky souboru
			Zoom=d.v.File_hlavicka.Zoom;
			Posun.x=d.v.File_hlavicka.PosunutiX;
			Posun.y=d.v.File_hlavicka.PosunutiY;
			d.v.PP.cas_start=d.v.File_hlavicka.cas_start;
			d.v.PP.mnozstvi=d.v.File_hlavicka.mnozstvi;
			d.v.PP.hod_den=d.v.File_hlavicka.hod_den;
			d.v.PP.dni_rok=d.v.File_hlavicka.dni_rok;
			d.v.PP.TT=d.v.File_hlavicka.TT;
			d.v.PP.efektivita=d.v.File_hlavicka.efektivita;
			d.v.PP.typ_voziku=d.v.File_hlavicka.typ_vozik;
			d.v.PP.delka_jig=d.v.File_hlavicka.delka_jig;
			d.v.PP.sirka_jig=d.v.File_hlavicka.sirka_jig;
			d.v.PP.vyska_jig=d.v.File_hlavicka.vyska_jig;
			d.v.PP.delka_podvozek=d.v.File_hlavicka.delka_podvozek;
			MOD=d.v.File_hlavicka.Mod;
			switch(MOD)
			{
					case NO:REFRESH();break; //překreslí obraz pro ostatní případy
					case SCHEMA: 	schemaClick(this);break;
					//ZDM case TESTOVANI:	testovnkapacity1Click(this);break;
					//ZDM case REZERVY:		casoverezervy1Click(this);break;
					//ZDM case CASOVAOSA:	editacelinky1Click(this);MOD=SCHEMA;/*casovosa1Click(this);*/break;
					//ZDM case TECHNOPROCESY:editacelinky1Click(this);MOD=SCHEMA;break;
					//ZDM case SIMULACE:	editacelinky1Click(this);MOD=SCHEMA;/*simulace1Click(this);*/break;
					default: schemaClick(this);break;
			}
			DuvodUlozit(false);
			//aktualizace statusbaru
			on_change_zoom_change_scGPTrackBar();

			return 1;
		}break;
		case 2://jiná chyba pravděpodbně špatný formát souboru
		{
			zavrit_uvod();//v případě chybové situace, např. soubor nenalezen, nebo špatný formát souboru zavře úvodní dialog
			MB("Neplatná verze souboru formátu *.tispl!",MB_OK);
			FileName="Nový.tispl";
			return 2;
		}break;
		default: return 2;break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Obnovitzezlohy1Click(TObject *Sender)
{
	scButton_zaloha->Down=false;
	scSplitView_MENU->Opened=false;
	OpenDialog1->Title="Otevřít soubor ze zálohy";
	OpenDialog1->DefaultExt="*tispl.bac_"+get_user_name()+"_"+get_computer_name();
	AnsiString nazev="Soubory zálohy TISPL (*.tispl.bac_"+get_user_name()+"_"+get_computer_name()+")|*.tispl.bac_"+get_user_name()+"_"+get_computer_name();
	OpenDialog1->Filter=nazev;
	if(OpenDialog1->Execute())
	{
		//zavolá nový soubor/smaže stávajicí
		Novy_soubor();

		//otevrení souboru
		Otevrit_soubor(OpenDialog1->FileName);

		//ještě donutí stávajicí soubor uložit pod novým jménem
		//odstranění koncovky
		AnsiString jen_nazev=FileName;
		while(jen_nazev.Pos(".bac")>0)//dokud bude ".bac" obsahovat
		jen_nazev.Delete(jen_nazev.Pos(".bac"),jen_nazev.Length());
		FileName=jen_nazev;
		UlozitjakoClick(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OtevritsablonuClick(TObject *Sender)
{
	OpenDialog1->Title="Otevřít šablonu";
	OpenDialog1->DefaultExt="*tisplTemp";
	AnsiString nazev="Soubory šablon TISPL (*.tisplTemp)|*.tisplTemp";
	OpenDialog1->Filter=nazev;
	if(OpenDialog1->Execute())
	{
		//zavolá nový soubor/smaže stávajicí
		Novy_soubor();

		//otevrení souboru
		Otevrit_soubor(OpenDialog1->FileName);

		//ještě donutí stávajicí soubor uložit pod novým jménem
		FileName="";
		UlozitjakoClick(this);
	}
}
//---------------------------------------------------------------------------
//uložení posledního otevřeného souboru
void TForm1::ulozit_posledni_otevreny()
{
	//-TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
	//-ini->WriteString("otevrene_soubory","posledni_soubor",FileName);
	//-delete ini;
	writeINI("otevrene_soubory","posledni_soubor",FileName);
}
//---------------------------------------------------------------------------
//zavře úvodní dialog
void TForm1::zavrit_uvod()
{
		Form_uvod->Timer1->Enabled=false;
		Form_uvod->Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajišťuje průběžné zálohování aplikace
void __fastcall TForm1::Timer_backupTimer(TObject *Sender)
{
		 //zapis hlavičky do souboru
		//test vytvor_hlavicku_souboru();

		//nastevení adresáře bac souboru k adresáři aplikace pokud se jedná o nový soubor, ještě neuložen pod novým názvem//přidáno 3.6 důsledky neověřeny
		if(FileName=="Nový.omap")SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

		//zapis dat do souboru
		d.v.uloz_do_souboru(FileName+".bac_"+get_user_name()+"_"+get_computer_name());

		//odstranění dlouhé adresy
		AnsiString jen_nazev=FileName;
		while(jen_nazev.Pos("\\")>0)//dokud bude lomítko obsahovat
		jen_nazev.Delete(1,jen_nazev.Pos("\\"));

		SB(jen_nazev+" zálohován do "+FileName+".bac_"+get_user_name()+"_"+get_computer_name());

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vše smaže z paměti
void TForm1::vse_odstranit()
{
		d.v.vse_odstranit();
		pom=NULL;delete pom;
		pom_temp=NULL;delete pom_temp;
		pom_element=NULL;delete pom_element;
		pom_element_smazat=NULL;delete pom_element_smazat;
		proces_pom=NULL;delete proces_pom;
		copyObjekt=NULL;delete copyObjekt;
		copyObjektRzRx.x=0;copyObjektRzRx.y=0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//načte a vrátí DPI zobraovače
int TForm1::get_DPI ()
{
	//Deklarace
	HDC desktopDc;
	int DPI;
	//Načte DPI zobrazovače
	desktopDc = GetDC(NULL);
	DPI = GetDeviceCaps(desktopDc,LOGPIXELSX);
	return DPI;
}
//---------------------------------------------------------------------------
UnicodeString TForm1::get_computer_name()
{
	 wchar_t *pc=new wchar_t[MAX_COMPUTERNAME_LENGTH+1];
   DWORD dwSize=MAX_COMPUTERNAME_LENGTH+1;
   if(GetComputerName(pc,&dwSize))return AnsiString(pc);
   else return "";
}
//---------------------------------------------------------------------------
UnicodeString TForm1::get_user_name()
{
	 wchar_t *user=new wchar_t[20+1];
   DWORD dwSize_user=20+1;
	 if(GetUserName(user,&dwSize_user))return AnsiString(user);
   else return "";
}
//---------------------------------------------------------------------------
UnicodeString TForm1::get_temp_dir()
{
	 WCHAR lpTempPathBuffer[MAX_PATH];
	 GetTempPathW(MAX_PATH,lpTempPathBuffer);
	 UnicodeString TempPath=lpTempPathBuffer;
	 return TempPath;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//načte nastavení programu z INI souboru
void TForm1::nacist_nastaveni()
{

	/*ChangeFileExt(Application->ExeName, ".ini");
  TIniFile *ini;
  if(FileExists(ExtractFilePath(Application->ExeName) + "omap_editor_"+get_user_name()+"_"+get_computer_name()+".ini"))//načte uživatelovo nastavení
  {
    ini = new TIniFile(ExtractFilePath(Application->ExeName) + "omap_editor_"+get_user_name()+"_"+get_computer_name()+".ini");
  }
  else ini = new TIniFile(ExtractFilePath(Application->ExeName) + "omap_editor.ini"); //načte implicitni

  nastaveni.autosave=ini->ReadInteger("nastaveni","autosave",nastaveni.autosave);
  nastaveni.minut=ini->ReadInteger("nastaveni","interval",nastaveni.minut);
  nastaveni.temp=ini->ReadInteger("nastaveni","temp",nastaveni.temp);
  nastaveni.posledni_file=ini->ReadInteger("nastaveni","autoload",nastaveni.posledni_file);
  nastaveni.aktualizace_mapove_podklady=ini->ReadInteger("nastaveni","mapovy_podklad",nastaveni.aktualizace_mapove_podklady);
  nastaveni.aktualizace_vyskovy_grid=ini->ReadInteger("nastaveni","vyskovy_grid",nastaveni.aktualizace_vyskovy_grid);
  delete ini;

  if(nastaveni.autosave==false)
  {
    Timer_aplikace->Enabled=true;
    Timer_aplikace->Interval=nastaveni.minut*60000;
  }
	else Timer_aplikace->Enabled=false;*/
}
//---------------------------------------------------------------------------
//uloží nastavení programu do INI souboru
void TForm1::ulozit_nastaveni()
{
	ChangeFileExt(Application->ExeName, ".ini");
	//TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
	TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");

	ini->WriteInteger("nastaveni","autosave",nastaveni.autosave);
	ini->WriteInteger("nastaveni","interval",nastaveni.minut);
	ini->WriteInteger("nastaveni","autoload",nastaveni.posledni_file);
	delete ini;

	/*if(nastaveni.autosave==false)
  {
		Timer_aplikace->Enabled=true;
		Timer_aplikace->Interval=nastaveni.minut*60000;
  }
	else Timer_aplikace->Enabled=false;*/
}
//---------------------------------------------------------------------------
void TForm1::DuvodUlozit(bool stav)
{
	 //nastavení stavu ovládacích prvků
	 RzToolButton3->Enabled=stav;
	 Toolbar_Ulozit->Enabled=stav;
	 scButton_ulozit->Enabled=stav;
	 //samotný indikátor změny a důvodu k uložení
	 duvod_k_ulozeni=stav;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajistí export do rastrového formátu
void __fastcall TForm1::Export1Click(TObject *Sender)
{
	 scSplitView_MENU->Opened=false;
	 scButton_export->Down=false;
	 SavePictureDialog1->Filter="Soubory formátu BMP (*.bmp)|*.bmp|Soubory formátu JPG (*.jpg)|*.jpg|Soubory formátu PNG (*.png)|*.png";

	 //předvyplnění názvem stejnojmeným souboru
	 UnicodeString FN=FileName;
	 if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
	 SavePictureDialog1->FileName=FN;

	 if(SavePictureDialog1->Execute())
	 {
			Screen->Cursor=crHourGlass;//změní kurzor na přesýpací hodiny

			Graphics::TBitmap * Bitmap = new Graphics::TBitmap;
			Bitmap->Height = ClientHeight;
			Bitmap->Width = ClientWidth;

			//uložení současných hodnot
			double Zoom_aktualni=Zoom; TPointD Posun_aktualni=Posun;

			//výpočet nových provizorních hodnot
			//zatim neřeším a jen se vezme aktuální screen
			//nebo oddálit na celou mapu
			//Zoom=m2px/Edit_m2px->Text.ToDouble();//zoom dle zadaného rozlišení
			//PosunutiX=my.round(exportni_okno_rozmer.MinX/m2px);PosunutiY=my.round((-1)*exportni_okno_rozmer.MaxY/m2px);//posunutí obrazu na levý horní roh exportního okna

			switch(MOD)//uloží obraz dle daného modu zobrazení
			{
				case SCHEMA:
				if(!antialiasing)d.vykresli_objekty(Bitmap->Canvas);//vykreslování všech vektorů
				else
				{
					Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
					bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
					Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
					d.vykresli_objekty(bmp_in->Canvas);
					Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
					Cantialising a;
					Graphics::TBitmap *bmp_grid=new Graphics::TBitmap; //grid zasílám nulovou bitmapu jako parametr, na NULL ač bylo ošetřené tak padalo
					Bitmap=a.antialiasing(bmp_grid,bmp_in);
					delete (bmp_grid);bmp_grid=NULL;//velice nutné
					delete (bmp_in);bmp_in=NULL;//velice nutné
				}
				break;
				//ZDM case REZERVY: d.vykresli_graf_rezervy(Bitmap->Canvas);break;//vykreslení grafu rezerv
				//ZDM case CASOVAOSA:	nastaveni_grafickeho_vystupu(Bitmap);break;
			}

			//vraceni puvodních hodnot
			Zoom=Zoom_aktualni; Posun=Posun_aktualni;

			UnicodeString export_format="bmp";
			if(SavePictureDialog1->FileName.SubString(SavePictureDialog1->FileName.Length()-2,3).LowerCase() == "jpg")export_format="jpg";
			if(SavePictureDialog1->FileName.SubString(SavePictureDialog1->FileName.Length()-2,3).LowerCase() == "png")export_format="png";

			TJPEGImage *Jpeg = new TJPEGImage();
			if(export_format=="jpg")
			{
				Jpeg->Assign(Bitmap);
				delete Bitmap;

				/*if(ScrollBar_kvalita_vystupu->Position<100)//i při 100 snižuje kvalitu, proto podmínka
				{
					Jpeg->CompressionQuality=ScrollBar_kvalita_vystupu->Position;
					Jpeg->Compress();
				}*/
			}

			TPngImage* Png=new TPngImage;
			if(export_format=="png")
			{
				Png->Assign(Bitmap);
				delete Bitmap;
			}

     //uložení
			if(!SavePictureDialog1->FileName.IsEmpty())//ošetření prázdnosti, možná už zajišťuje podmínka nad
			{
				UnicodeString F;
				if(SavePictureDialog1->FileName.Pos("."+export_format))F=SavePictureDialog1->FileName;//ošetření přípony, pokud obsahuje
				else F=SavePictureDialog1->FileName+"."+export_format;//pokud neobsahuje

				if(export_format=="jpg"){Jpeg->SaveToFile(F);delete Jpeg;}
				if(export_format=="bmp"){Bitmap->SaveToFile(F);delete Bitmap;}
				if(export_format=="png"){Png->SaveToFile(F);delete Png;}

				//ShellExecute(0,L"open",UnicodeString(F).c_str(),0,0,SW_SHOWNORMAL);//otevře výstup
			}
			Screen->Cursor=crDefault;//změní kurzor na default
			SB("Export do "+export_format+" dokončen.");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Rychlexport1Click(TObject *Sender)
{
	 Screen->Cursor=crHourGlass;//změní kurzor na přesýpací hodiny

	 unsigned int PO=100;//po kolika vozících se bude stránkovat export
	 for(unsigned int i=1;i<=d.v.VOZIKY->predchozi->n;i+=PO)
	 {
     //založení Bitmapy včetně parametrů
		 Graphics::TBitmap * Bitmap = new Graphics::TBitmap;
		 nastaveni_grafickeho_vystupu(Bitmap,i,PO);

     //uložení z Bitmapy do Png (do Jpg dělalo drobnou grafickou chybu)
     TPngImage* Png=new TPngImage;
     Png->Assign(Bitmap);
     delete Bitmap;

		 //uložení do finálního souboru včetně otevření souboru
		 //UnicodeString FileName="export_"+UnicodeString(DateToStr(Now()))+"_"+ms.replace(TimeToStr(Now()),"_",":")+"_"+AnsiString(i)+"_"+AnsiString(i+PO)+".png";
		 if(d.v.VOZIKY->predchozi->n<=PO)UnicodeString FileName="export_"+AnsiString(i)+"_"+AnsiString(d.v.VOZIKY->predchozi->n)+".png";//pokud se nestránkuje, název souboru bez data, výše s datem, ale zbytečně se to množí...
		 else UnicodeString FileName="export_"+AnsiString(i)+"_"+AnsiString(i+PO)+".png";//název souboru bez data, výše s datem, ale zbytečně se to množí...
		 Png->SaveToFile(FileName);delete Png;//uloží PNG do souboru a smaže na něj ukazatel
		 ShellExecute(0,L"open",UnicodeString(FileName).c_str(),0,0,SW_SHOWNORMAL);//otevře výstup
	 }

	 Screen->Cursor=crDefault;//změní kurzor na default
	 SB("Rychlý export dokončen.");
}
//---------------------------------------------------------------------------
void TForm1::nastaveni_grafickeho_vystupu(Graphics::TBitmap * Bitmap,unsigned int OD,unsigned int PO)
{

	 if(OD==0)OD=1;//ošetření kvůli následujícímu OD-1=aby se nešlo do minusu resp. 0 ošetřuje d.v.vrat_start_a_pozici_vozikuPX
	 //souřadnice X pro pozici a šířku (pozor: .y mate jedná se o pravou souřadnici X)
	 int L=0;int R=0;
	 //souřadnice Y
	 int B=0;

	 if(d.v.VOZIKY->predchozi->n<=PO)//pokud není třeba stránkovat
	 {
			 R=d.WidthCanvasCasoveOsy-5;//-5korekce následujích +5
			 L=0;
			 B=d.HeightCanvasCasoveOsy;
	 }
	 else//je hodně (resp. více jak PO-čet) vozíku je třeba stránkovat
	 {
			B=d.KrokY*(PO+2);
			L=d.v.vrat_start_a_pozici_vozikuPX(OD-1).x;
			R=d.v.vrat_start_a_pozici_vozikuPX(OD+PO+2).y-L;//odečte offset z levé strany
			if(R<=0)R=d.WidthCanvasCasoveOsy-L-5;//pokud nastane situace, že už poslední vozík není nalezen//-5korekce následujích +5
	 }
	 //ShowMessage(AnsiString(OD)+"_"+AnsiString(L)+"_"+AnsiString(R));

	 ///kvůli nadpisu výstupu skládám výslednou mapu ze dvou bitmap, nelze řešit jen přes posun, protože se mi neposunou popisky svislých os
	 Graphics::TBitmap * Bitmap_MARO = new Graphics::TBitmap;
	 Bitmap_MARO->Width=/*d.WidthCanvasCasoveOsy*/R+5;
	 Bitmap_MARO->Height=/*d.HeightCanvasCasoveOsy;*/B;//+1 kousek navíc

	 TPointD puvPosunT=d.PosunT;//zazálohování aktuálního posunu
	 d.PosunT.x=L;d.PosunT.y=d.KrokY*(OD-1);//provizorní navrácení na výchozí pozici -1 kousek navíc
	 d.vykresli_casove_osy(Bitmap_MARO->Canvas);
	 d.PosunT=puvPosunT;//navrácení do stavu, aby uživatel posun do výchozí pozice nezaznamenal

	 //vstupně/výstupní bitmapa
	 Bitmap->Width=/*d.WidthCanvasCasoveOsy*/R+5;              //+5 kousek navíc
	 Bitmap->Height=/*d.HeightCanvasCasoveOsy*/B+scGPPanel_mainmenu->Height-10;
	 Bitmap->Canvas->Draw(0,scGPPanel_mainmenu->Height-10,Bitmap_MARO);//Bitmap_MARO vykreslím do výsledné mapy níže o výšku scGPPanel_mainmenu->Height kvůli prostoru na nadpis, ten se vytváří níže
	 delete Bitmap_MARO;

	 //nadpis výstupu
	 Bitmap->Canvas->Font->Color=clGray;
	 Bitmap->Canvas->Font->Size=12;
	 Bitmap->Canvas->Font->Name="Arial";
	 Bitmap->Canvas->Brush->Style=bsSolid;
	 Bitmap->Canvas->Brush->Color=clWhite;
	 Bitmap->Canvas->Pen->Color=clWhite;
	 Bitmap->Canvas->Pen->Mode=pmCopy;
	 Bitmap->Canvas->Rectangle(0,0,d.WidthCanvasCasoveOsy,d.KrokY);//pouze bílé pozadí titulku
	 Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	 AnsiString T="Zobrazení časových os technologických procesů projektu: ";
	 Bitmap->Canvas->TextOutW(8,5,T);
	 short W=Bitmap->Canvas->TextWidth(T);
	 Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold << fsItalic;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	 Bitmap->Canvas->TextOutW(8+W,5,FileName);

}
//---------------------------------------------------------------------------
//zajistí report do Formátu CSV nebo JPG
void __fastcall TForm1::csv1Click(TObject *Sender)
{
	// puvodni export do csv

}
//---------------------------------------------------------------------------
void __fastcall TForm1::html1Click(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
	scButton_report->Down=false;
	if(d.v.OBJEKTY->dalsi==NULL)//pokud existují nějaka data
		MB("Žádná data k reportu!");
	else
	{

			UnicodeString FN=FileName;
			if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
			Form_report->ulozit_report(FN+".html");


	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*
void __fastcall TForm1::Button2Click(TObject *Sender)
{
 AnsiString pripona="tispl";
 AnsiString alias="TISPL";
 AnsiString aplikace=Application->ExeName.c_str();

 TRegistry *r=new TRegistry();
 char str[1024];
 r->RootKey=HKEY_CLASSES_ROOT;
 r->OpenKey(pripona,true);
 r->WriteString("",alias);
 r->CloseKey();

 sprintf(str,"%s\\DefaultIcon",alias);
 if(r->OpenKey(str,true))
 {
  sprintf(str,"%s,-%i",aplikace,-1);
  r->WriteString("",str);
 }
 r->CloseKey();

 sprintf(str,"%s\\shell\\open\\comand",alias);
 if(r->OpenKey(str,true))
 {
  sprintf(str,"%s \%1",aplikace);
  r->WriteString("",str);
 }
	delete(r);


}
//---------------------------------------------------------------------------
*/
void __fastcall TForm1::Obnovitobraz1Click(TObject *Sender)
{
	//nezabira zneplatnit_minulesouradnice();
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_simulaceTimer(TObject *Sender)
{
	 Canvas->TextOutW(90,90,d.cas);
	 if(d.cas%100==0) //&& cas>0)
	 {
			//při taktu zajistí navěšení dalšího vozíku ve frontě, který je v režimu stop
			Cvektory::TVozik *ukaz;
			ukaz=d.v.VOZIKY->dalsi;//přeskočí hlavičku
			while (ukaz!=NULL)
			{
				//ZDM if(ukaz->stav==-1){ukaz->segment=d.v.OBJEKTY->dalsi; ukaz->stav=1;break;}//vozík bude čekat na "oranžovou" na palec
				ukaz=ukaz->dalsi;//posun na další prvek
			}
	 }
	 //ZDM d.vykresli_simulaci(Canvas);
	 d.cas++;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	/*d.v.vloz_vozik();
	d.v.VOZIKY->predchozi->segment=d.v.OBJEKTY->dalsi;//inicializace
	d.v.VOZIKY->predchozi->pozice=0;
	d.v.VOZIKY->predchozi->delka=PP.delka_voziku;
	d.v.VOZIKY->predchozi->sirka=PP.sirka_voziku;
	if(rand()%2){
		d.v.VOZIKY->predchozi->rotace=90;
	}
	else
		d.v.VOZIKY->predchozi->rotace=0;
	*/

	//zásobník ještě nenavěšených vozíků
	Cvektory::TVozik *ukaz;
	ukaz=d.v.VOZIKY->dalsi;//přeskočí hlavičku
	while (ukaz!=NULL)
	{
			//ZDM if(ukaz->stav==0){ukaz->segment=d.v.OBJEKTY->dalsi; ukaz->stav=1;break;}//pustí vozík
			ukaz=ukaz->dalsi;//posun na další prvek
	}


	Button3->Caption="Navěsit vozík ("+AnsiString(d.v.VOZIKY->predchozi->n)+")";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Vypicestuktempu1Click(TObject *Sender)
{
	MB(-1,-1,"<b>Verze: </b>"+VERZE+"<br><br><b>Umístění: </b>"+Application->ExeName+"<br><br><b>Temp : </b>"+get_temp_dir(),"Informace o aplikaci");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	Timer_simulace->Enabled=!Timer_simulace->Enabled;
	if(Button4->Caption=="Pauza simulace")Button4->Caption="Start simulace";
	else Button4->Caption="Pauza simulace";
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
//ZDM
//	Cvektory::TObjekt *ukaz=d.v.OBJEKTY->predchozi->predchozi;//přeskočí hlavičku
//	ZDM if(ukaz->stav==0)ukaz->stav=1;
//	else ukaz->stav=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
	//ZDM
//	Cvektory::TVozik *ukaz=d.v.VOZIKY->dalsi;//přeskočí hlavičku
//	if(ukaz->stav==0)ukaz->stav=1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
	/*//canv->TextOutW(50,70,AnsiString(m.rotace(1,180-(360-U),A).x*size));
	//canv->TextOutW(50,90,AnsiString(m.rotace(1,180-(360-U),A).y*size));
	POINT body[4]=
	{
		{m.round(X+m.rotace(1,360-U,A).x*size),m.round(Y+m.rotace(1,360-U,A).y*size)},
		{m.round(X+m.rotace(1,180-(360-U),A).x*size),m.round(Y+m.rotace(1,180-(360-U),A).y*size)},
		{m.round(X+m.rotace(1,180-U,A).x*size),m.round(Y+m.rotace(1,180-U,A).y*size)},
		{m.round(X+m.rotace(1,U,A).x*size),m.round(Y+m.rotace(1,U,A).y*size)}*/

		//ShowMessage(m.rotace(1,-45,23.54).x*21.21);
		//ShowMessage(fmod(Edit_takt_time->Text.ToDouble(),360.0));

		Timer_simulaceTimer(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button9Click(TObject *Sender)
{
		Memo1->Lines->Clear();
		Memo1->Visible=true;
//		Memo1->Lines->Add("vypis spojáku VOZIKY:");
//			Cvektory::TVozik *ukaz=d.v.VOZIKY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			//akce s ukazatelem
//			//Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" ID: ")+ukaz->id+AnsiString(" X: ")+ukaz->X+AnsiString(" Y: ")+ukaz->Y);
//			Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" Start: ")+ukaz->start/d.PX2MIN+AnsiString(" Pozice: ")+ukaz->pozice/d.PX2MIN);
//			//posun na další prvek v seznamu
//			ukaz=ukaz->dalsi;
//		}
		//Memo1->Lines->Add("vypis spojáku OBJEKTY:");
	/*	Cvektory::TObjekt *ukaz=d.v.CESTA2;//ukazatel na první objekt v seznamu OBJEKTU, NEpřeskočí hlavičku
		while (ukaz!=NULL)
		{
			//akce s ukazatelem
			Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" CT: ")+ukaz->CT);//+AnsiString(" Y: ")+ukaz->Y);

			//posun na další prvek v seznamu
			ukaz=ukaz->dalsi;
		} */
		//ShowMessage(d.v.WIP());
		//S(d.v.vrat_WT_voziku(d.v.VOZIKY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi));
		//ShowMessage(m.cekani_na_palec(60,32.5,3));
		//ShowMessage(d.v.vrat_prumerne_TT_zakazky(d.v.CESTY->dalsi));
		//ShowMessage(d.v.vrat_prumerne_TT_zakazky(d.v.CESTY->dalsi->dalsi));

		   //		Pan_bmp->Width=ClientWidth-W;Pan_bmp->Height=ClientHeight-H-Gh;//velikost pan plochy
			//		Pan_bmp->Canvas->CopyRect(Rect(W,H,ClientWidth,ClientHeight-RzStatusBar1->Height-Gh),Canvas,Rect(W,H,ClientWidth,ClientHeight-RzStatusBar1->Height-Gh));//uloží pan výřez


				   //	int W=RzSizePanel_knihovna_objektu->Width;
				   //	Pan_bmp->Width=ClientWidth-W;Pan_bmp->Height=ClientHeight-20;//velikost pan plochy
				   //	Pan_bmp->Canvas->CopyRect(Rect(W,H,ClientWidth,ClientHeight-RzStatusBar1->Height-Gh),Canvas,Rect(W,H,ClientWidth,ClientHeight-RzStatusBar1->Height-Gh));//uloží pan výřez

					//Pan_bmp->Canvas->CopyRect(Rect(0+W,0+H,ClientWidth+W,ClientHeight+RzStatusBar1->Height+H+Gh),Canvas,Rect(0+W,0+H,ClientWidth+W,ClientHeight+RzStatusBar1->Height+H+Gh));//uloží pan výřez
					//Pan_bmp->Canvas->CopyRect(Rect(0+W,0+H,ClientWidth,ClientHeight-RzStatusBar1->Height-H-Gh),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight-RzStatusBar1->Height-H-Gh));//uloží pan výřez

					 //	Pan_bmp->SaveToFile("test.bmp");

		Memo1->Lines->Add("vypis spojáku PROCESY:");
		Cvektory::TProces *ukaz=d.v.PROCESY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
		while (ukaz!=NULL)
		{
			//akce s ukazatelem
			//if(ukaz->vozik->n==2)
			Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" | n_v_zakazce: ")+ukaz->n_v_zakazce+AnsiString(" | Tpoc: ")+ukaz->Tpoc+AnsiString(" | Tkon: ")+ukaz->Tkon+AnsiString(" | Tdor: ")+ukaz->Tdor+AnsiString(" | Tpre: ")+ukaz->Tpre+AnsiString(" | Tcek: ")+ukaz->Tpre+AnsiString(" | Trand: ")+ukaz->Trand+AnsiString(" | Shortname: "));
			//posun na další prvek v seznamu
			ukaz=ukaz->dalsi;
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button10Click(TObject *Sender)
{
//	TPointDbool N=m.zkratit_polygon_na_roztec(d.v.delka_dopravniku(d.v.OBJEKTY),10.0,d.v.OBJEKTY->predchozi->X,d.v.OBJEKTY->predchozi->Y,d.v.OBJEKTY->predchozi->predchozi->X,d.v.OBJEKTY->predchozi->predchozi->Y,d.v.OBJEKTY->dalsi->X,d.v.OBJEKTY->dalsi->Y);
//	if(N.b)
//	{
//		 d.v.OBJEKTY->predchozi->X=N.x;
//		 d.v.OBJEKTY->predchozi->Y=N.y;
//	}

	//
	//d.v.OBJEKTY->predchozi->X=46.25575147;
	//d.v.OBJEKTY->predchozi->Y=-21.95662734;

	/*d.v.OBJEKTY->predchozi->X=24.25156834;
	d.v.OBJEKTY->predchozi->Y=-26.08241167; */

	//d.v.OBJEKTY->predchozi->X=35.25365991;
	//d.v.OBJEKTY->predchozi->Y=-24.0195195;

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxPALCE_Click(TObject *Sender)
{
 d.JIZPOCITANO=true;
 Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int TForm1::vrat_max_vysku_grafu()
{
 int RET=0;
 if(RET<=Chart1->Height && Chart1->Visible)RET=Chart1->Height;
 if(RET<=Chart2->Height && Chart2->Visible)RET=Chart2->Height;
 if(RET<=Chart3->Height && Chart3->Visible)RET=Chart3->Height;
 if(RET<=Chart4->Height && Chart4->Visible)RET=Chart4->Height;
 if(RET<=Chart6->Height && Chart6->Visible)RET=Chart6->Height;
 //případně doplnit další grafy!!!!!
 return RET;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TForm1::Chart1Click(TObject *Sender)
{
	// Panel1->Visible=true;
	// Panel1->Width=ClientWidth;
	// Panel1->Height=ClientHeight;




}
//---------------------------------------------------------------------------
void __fastcall TForm1::MagnaClick(TObject *Sender)
{
	scButton_posledni_otevreny->Down=false;
	scSplitView_MENU->Opened=false;
	Otevrit_soubor(FileName);
//ZDM
//	scSplitView_MENU->Opened=false;
//	//otevřít soubor
//	OtevritSoubor(FileName);
//	d.v.hlavicka_seznamu_cest();
//
//	//definice pohonů
//	double D1v=4;/*4.5*/ 		double D1r=540;
//	double D2v=1.89;/*4.5*/ double D2r=378;
//	double D3v=0.75;/*4.5*/ double D3r=1620;
//	double D4v=0.75;/*4.5*/ double D4r=1620;
//	double D5v=0.75;/*4.5*/ double D5r=1620;
//
//	//načíst plán výroby
//	///////MAGNA//////////////////////////////
//	//cesta 1
//	Cvektory::TSeznam_cest *cesta_pom=new Cvektory::TSeznam_cest;
//	d.v.hlavicka_jedne_cesty(cesta_pom);
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r);//nav - režim,kapacita,ct
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi,1,1,4.345,D1v,D1r);//pre1
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi,1,1,2,D2v,D2r);//pow
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,1,1,1.64,D2v,D2r);//pre2  = souset dilcich vzd 1,25m+1,25m
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D2v,D2r);//ofuk
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,1.34,D1v,D1r);//pre3  - stoupaní před sušením
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,10,20,D1v,D1r);//sus1
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//chlaz1
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,0.35,D1v,D1r);//pre4
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D2v,D2r);//ion
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,1.64,D2v,D2r);//pre5
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,2,14.66,D3v,D3r);//PRIMER
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,7,10,D1v,D1r);//vyt1
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,15,30,D1v,D1r);//sus2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,4,10,D1v,D1r);//chlaz2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,4,14.66,D4v,D4r);//BASECOAT
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//vyt2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,2,14.66,D5v,D5r);//CLEARCOAT
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//vyt3
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,15,30,D1v,D1r);//sus3
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//chlaz3
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,0.79,D1v,D1r);//pr6
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//sves
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,3,4.87,D1v,D1r);//pre7
//
//
//	//cesta 2
//	Cvektory::TSeznam_cest *cesta_pom2=new Cvektory::TSeznam_cest;
//	d.v.hlavicka_jedne_cesty(cesta_pom2);
//
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r);//nav - režim,kapacita,ct
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi,1,1,4.345,D1v,D1r);//pre1
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi,1,1,2,D2v,D2r);//pow
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,1,1,1.64,D2v,D2r);//pre2  = souset dilcich vzd 1,25m+1,25m
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D2v,D2r);//ofuk
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,1.34,D1v,D1r);//pre3  - stoupaní před sušením
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,10,20,D1v,D1r);//sus1
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//chlaz1
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,0.35,D1v,D1r);//pre4
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D2v,D2r);//ion
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,1.64,D2v,D2r);//pre5
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,2,14.66,D3v,D3r);//PRIMER
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,7,10,D1v,D1r);//vyt1
////d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,18,30,D1v,D1r);//sus2
////d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,4,10,D1v,D1r);//chlaz2
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,4,14.66,D4v,D4r);//BASECOAT
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//vyt2
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,2,14.66,D5v,D5r);//CLEARCOAT
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//vyt3
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,15,30,D1v,D1r);//sus3
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,5,10,D1v,D1r);//chlaz3
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,0.79,D1v,D1r);//pr6
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//sves
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,3,4.87,D1v,D1r);//pre7
//
//
//	//vloží novou hotovou cestu do spoj.seznamu cest   */
//	d.v.vloz_cestu(cesta_pom2);//vloží novou hotovou cestu do spoj.seznamu cest
//	d.v.vloz_cestu(cesta_pom);
//
//	//automatické volání z historie - PROZATIM
//	Form_vozik_nastaveni->nacti_voziky();
//	Form_vozik_nastaveni->uloz_voziky_a_nastav_zakazky();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SPPP1Click(TObject *Sender)
{
//	OtevritSoubor("SPPP.tispl");
//	d.v.hlavicka_seznamu_cest();
//
//	//definice pohonů
//	double D1v=2;double D1r=32.5;
//
//	Cvektory::TSeznam_cest *cesta_pom=new Cvektory::TSeznam_cest;
//
//	d.v.hlavicka_jedne_cesty(cesta_pom);
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r); //nav - režim,kapacita,ct
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi,0,1,2,D1v,D1r);//ion
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//otoč
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,0,1,30,D1v,D1r);//ion
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//primer
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,10,13.3,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//base
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,2,10,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//base2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,18,10,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//CLEARCOAT
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,6,10,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D1v,D1r);//vytah
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,18,40,D1v,D1r);//suš
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,8.5,5,D1v,D1r);//chlaz
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//svěšování
//
//	d.v.vloz_cestu(cesta_pom);
//
//	Cvektory::TSeznam_cest *cesta_pom2=new Cvektory::TSeznam_cest;
//	d.v.hlavicka_jedne_cesty(cesta_pom2);
//
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r); //nav - režim,kapacita,ct
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi,0,1,2,D1v,D1r);//ion
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//otoč
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//ion
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//primer
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,10,13.3,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//base
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,2,10,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//base2
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,18,10,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//clear
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,6,10,D1v,D1r);//vyt
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D1v,D1r);//vytah
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,18,40,D1v,D1r);//suš
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,8.5,5,D1v,D1r);//chlaz
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//svěšování
//
//	d.v.vloz_cestu(cesta_pom2);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Boskovice1Click(TObject *Sender)
{
//	OtevritSoubor("boskovice.tispl");
//	d.v.hlavicka_seznamu_cest();
//
//	//definice pohonů
//	double D1v=2;double D1r=32.5;
//
//
//	Cvektory::TSeznam_cest *cesta_pom=new Cvektory::TSeznam_cest;
//	d.v.hlavicka_jedne_cesty(cesta_pom);
//
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r); //nav - režim,kapacita,ct
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi,0,1,2,D1v,D1r);//ožeh
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//ion
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,1,1,2,D1v,D1r);//lak1
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,2,4,6.5,D1v,D1r);//vyt1
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,1,1,2,D1v,D1r);//lak2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,4,3.5,D1v,D1r);//vyt2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,17,30,D1v,D1r);//SUŠ
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,2,9,2,D1v,D1r);//CHLAZ
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi->dalsi,0,1,2,D1v,D1r);//SVĚŠ
//	d.v.vloz_cestu(cesta_pom);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::eXtreme1Click(TObject *Sender)
{
//	OtevritSoubor("extreme.tispl");
//	d.v.hlavicka_seznamu_cest();
//
//	//definice pohonů
//	double D1v=2;double D1r=32.5;
//
//	//cesta 1
//	Cvektory::TSeznam_cest *cesta_pom=new Cvektory::TSeznam_cest;
//	d.v.hlavicka_jedne_cesty(cesta_pom);
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r);//nav
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi,1,1,5,D1v,D1r);//co2
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi,1,1,8,D1v,D1r);//ION
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,0,1,4,D1v,D1r);//lak
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,2,2,4,D1v,D1r);//vyť
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,0,1,4,D1v,D1r);//lak
//	d.v.vloz_segment_cesty(cesta_pom,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,2,2,4,D1v,D1r);//vyť
//	d.v.vloz_cestu(cesta_pom);//vloží novou hotovou cestu do spoj.seznamu cest
//	//cesta 2
//	Cvektory::TSeznam_cest *cesta_pom2=new Cvektory::TSeznam_cest;
//	d.v.hlavicka_jedne_cesty(cesta_pom2);
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi,0,1,2,D1v,D1r);//nav
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi,1,1,2.0,D1v,D1r);//co2
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi,0,1,4,D1v,D1r);//lak
//	d.v.vloz_segment_cesty(cesta_pom2,d.v.OBJEKTY->dalsi->dalsi->dalsi->dalsi->dalsi,2,2,4,D1v,D1r);//vyť
//	d.v.vloz_cestu(cesta_pom2);//vloží novou hotovou cestu do spoj.seznamu cest
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	 switch(d.NOLIEX)
	 {
			 case 0:d.NOLIEX++;Button2->Caption="LINEAR";break;
			 case 1:d.NOLIEX++;Button2->Caption="EXPO";break;
			 case 2:d.NOLIEX=0;Button2->Caption="NO";break;
	 }
	 Invalidate();
}
//---------------------------------------------------------------------------
//skryje v době neaktivity (po 50 sec) svislice na myši v modu časové osy (kvůli spořiči obrazovky)
void __fastcall TForm1::Timer_neaktivityTimer(TObject *Sender)
{
		if(MOD==CASOVAOSA && ++pocitadlo_doby_neaktivity==2)
		{
			Timer_neaktivity->Enabled=false;
			d.zobrazit_label_zamerovac(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
		}
		if(MOD==NAHLED && ++pocitadlo_doby_neaktivity==2)
		{
			Timer_neaktivity->Enabled=false;
			setJobIDOnMouseMove(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonPLAY_OClick(TObject *Sender)
{
	Timer_animace->Enabled=!Timer_animace->Enabled;
	if(Timer_animace->Enabled)
	{
		CheckBoxAnimovatSG->Visible=true;
		d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
		d.PosunT.y=0;
		ButtonPLAY->GlyphOptions->Kind=scgpbgkStop;
		ButtonPLAY->Hint="zastavit animaci";
		Timer_animace->Interval=40;
		d.TP.K=0.05;//Krok po kolika minutach se bude zobrazovat
		d.TP.DO=-d.TP.K;//výchozí čás (záporný interval, kvůli tomu, aby se začínalo od nuly)
		d.TP.Nod=0;//rozmezí Jaký se vypíše vozik,
		d.TP.Ndo=0;//rozmezí Jaký se vypíše vozik, pokud bude 0 vypisují se všechny
		d.TP.A=true;//jednořádková animace
	}
	else
	{
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
		ButtonPLAY->Hint="spustit animaci";
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_animaceTimer(TObject *Sender)
{
	d.TP.DO+=d.TP.K;//konec zakazky v min
	d.TP.OD=d.TP.DO;//od které min se proces začne vypisovat
	if(d.TP.DO<=d.TP.KZ)
	{
		REFRESH();
	}
	else
	{
		Timer_animace->Enabled=false;
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
		ButtonPLAY->Hint="spustit animaci";
		SyntezaClick(Sender);//vratí statický mod
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxVytizenost_Click(TObject *Sender)
{
	if(d.v.PROCESY!=NULL && d.v.PROCESY->predchozi->n>0)//pokud je více objektů
	{
		d.mod_vytizenost_objektu=!d.mod_vytizenost_objektu;
		CheckBoxVymena_barev->Visible=!CheckBoxVymena_barev->Visible;
		ComboBoxCekani->Visible=!ComboBoxCekani->Visible;
		scLabel_doba_cekani->Visible=!scLabel_doba_cekani->Visible;
		scGPGlyphButton_info_cekani->Visible=!scGPGlyphButton_info_cekani->Visible;
		scGPCheckBox_pocet_voziku_dle_WIP->Visible=!scGPCheckBox_pocet_voziku_dle_WIP->Visible;
		//pozice
		if(d.mod_vytizenost_objektu)CheckBoxVytizenost->Top=scLabel_doba_cekani->Top;
		else CheckBoxVytizenost->Top=135;
		SB("");
		Invalidate();
		RM();//korekce chyby oskakování pravého menu
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button13Click(TObject *Sender)
{
 //S(m.mezera_mezi_voziky(1,0.325,0));
 //	ShowMessage(scListGroupNastavProjektu->TabOrder);
 //	ShowMessage(scListGroupKnihovObjektu->TabOrder);

 //ShowMessage(STATUS);
 //	scListGroupNastavProjektu->TabOrder;

	/*	Memo1->Visible=true;

	Memo1->Lines->Add("Výpis OBJEKTY:");
	Cvektory::TObjekt *ukaz=d.v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		//akce s ukazatelem
		Memo1->Lines->Add(AnsiString(ukaz->n)+";"+AnsiString(ukaz->short_name)+";"+AnsiString(ukaz->rezim));

		//posun na další prvek v seznamu
		ukaz=ukaz->dalsi;
	}

	Memo1->Lines->Add("Výpis obsahu cesta1:");
	Cvektory::TCesta *C=d.v.CESTY->dalsi->cesta->dalsi;
	while (C!=NULL)
	{
		Memo1->Lines->Add(AnsiString(C->n)+";"+AnsiString(C->objekt->n)+";"+AnsiString(C->objekt->short_name)+";"+AnsiString(C->objekt->rezim));
		C=C->dalsi;
	}
		 */
		//Zoom*=2;
		//Form2->ShowModal();

		//Akce=MEASURE;

		//RO-=(1.2*Zoom/m2px)/20.0;
		//REFRESH();
		Timer1->Enabled=!Timer1->Enabled;
		scGPSwitch_meritko->State=!Timer1->Enabled;
		d.v.PP.raster.show=!Timer1->Enabled;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBoxVymena_barev_Click(TObject *Sender)
{
	 d.JIZPOCITANO=false;//zajistí přepočet časových os
	 Invalidate();
	 RM();//korekce chyby oskakování pravého menu
}

//-------------------------------------------------------------
void __fastcall TForm1::ComboBoxODminChange(TObject *Sender)
{
		//ještě ošetření aby zadal hodnotu od menší nebo rovno hodnotě do

		d.TP.OD=ms.MyToDouble(ComboBoxODmin->Text);
		Invalidate();
		RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxDOminChange(TObject *Sender)
{

		//ještě ošetření aby zadal hodnotu od menší nebo rovno hodnotě do
		d.TP.DO=ms.MyToDouble(ComboBoxDOmin->Text);
		Invalidate();
		RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
//zapne či vypne antialiasing
void __fastcall TForm1::scGPSwitch_AAChangeState(TObject *Sender)
{
	antialiasing=!antialiasing;//musí být před scSplitView_MENU->Opened!!!
	scSplitView_MENU->Opened=false;
	DrawGrid_knihovna->Invalidate();
	Invalidate();//v tomto případě lépe než REFRESH - kvůli efektu
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_trTimer(TObject *Sender)
{
	if(!ttr("TimerTr")){Timer_tr->Enabled=false;Close();}//kontrola zda nevypršela trial verze
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SQL_processIDClick(TObject *Sender)
{

	FDQuery1->Active = False;
	FDQuery1->Open("select * from app_setup where id=\"1\"");  //id nahradit id z ini     a udelat podmínku zda platí lokální údaje o pc s uloženými
	FDQuery1->Active = True;


	//ZDM if(get_computer_name()!=FDQuery1->Fields->Fields[2]->AsAnsiString || get_user_name()!=FDQuery1->Fields->Fields[3]->AsAnsiString) {
	//ZDM ShowMessage("neplatne udaje v PC a na serveru");
	//ZDM }

	//ZDM AnsiString send_log_time= TIME.CurrentDateTime();
	AnsiString relation_id=GetCurrentProcessId();


 //	AnsiString date= TIME.CurrentDate();
 //ZDM 		AnsiString strSQL = "INSERT INTO log_table (app_id,app_start,app_stop,send_log_time,command,relation_id) VALUES (\""+FDQuery1->Fields->Fields[0]->AsAnsiString+"\",\""+send_log_time+"\",\"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\")";
		//S(strSQL);


		//ZDM FDConnection1->ExecSQL(strSQL);



			

}
//---------------------------------------------------------------------------


void __fastcall TForm1::CheckBox_pouzit_zadane_kapacity_OLDClick(TObject *Sender)
{
Invalidate();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::rComboBoxKrokChange(TObject *Sender)
{
		d.TP.K=ms.MyToDouble(rComboBoxKrok->Text);
		Invalidate();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::MaxButtonClick(TObject *Sender)
{
	if (FMaximized)//zmenšení
	{
			 //BoundsRect =  FOldBoundsRect;
			 FMaximized = false;
			 scLabel_titulek->DragForm = true;
			 MaxButton->GlyphOptions->Kind = scgpbgkMaximize;
			 scGPSizeBox->Visible = true;
			 Form1->Width=Screen->Width/3*2;//zmenší formulář na 2/3 jeho velikosti
			 Form1->Height=Screen->Height/3*2;//zmenší formulář na 2/3 jeho velikosti
			 scSplitView_OPTIONS->Opened=false;
			 scSplitView_MENU->Opened=false;
	}
	else //maximalizace
	{
			//FOldBoundsRect = BoundsRect;
			BoundsRect = scStyledForm1->GetMaximizeBounds();
			FMaximized = true;
			scLabel_titulek->DragForm = false;
			MaxButton->GlyphOptions->Kind = scgpbgkRestore;
			scGPSizeBox->Visible = false;
			scSplitView_OPTIONS->Opened=false;
			scSplitView_MENU->Opened=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_OPTIONS_OldClick(TObject *Sender)
{
  ESC();//zruší případnou rozdělanou akci
	scSplitView_OPTIONS->Opened = !scSplitView_OPTIONS->Opened;

	if(scSplitView_MENU->Opened)
	{
		scSplitView_MENU->Close();
		deaktivace_zamerovace();
	}
  //if(d.v.PP.raster.show==true) scGPCheckBox_zobraz_podklad->Checked=true;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::KonecClick(TObject *Sender)
{
	//zalogování vypnutí aplikace
//	AnsiString relation_id=GetCurrentProcessId();
//	AnsiString send_log_time= TIME.CurrentDateTime();
//	AnsiString ID ="1";
//	AnsiString Text="aplikace stop";
//	AnsiString strSQL = "INSERT INTO app_log (app_id,app_start,username,send_log_time,command,relation_id) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\")";
//	FDConnection1->ExecSQL(strSQL);
	scButton_konec->Down=false;
	Close();//ukončí aplikaci
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_ZOOM_MINUSClick(TObject *Sender)
{
	vycentrovat=false;
	akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
	ZOOM_OUT();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_ZOOM_PLUSClick(TObject *Sender)
{
 vycentrovat=false;
 akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
 ZOOM_IN();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scLabel_titulekDblClick(TObject *Sender)
{
  //zajistí maximalizaci či zmenšení okna po double clicku na titulek
	MaxButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MinButtonClick(TObject *Sender)
{
	Application->Minimize();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton2Click(TObject *Sender)
{
	scSplitView_LEFTTOOLBAR->Opened = !scSplitView_LEFTTOOLBAR->Opened;
}
//---------------------------------------------------------------------------
//volání parametrů linky nebo definice zakázek - dle zvoleného režimu
void __fastcall TForm1::Button_dopravnik_parametryClick(TObject *Sender)
{
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
 //	scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	scButton_parmlinky_defzakazek->Down=false;
	if(scGPButton_header_projekt->ImageIndex==49)
	{
		ESC();//zruší případnou rozdělanou akci
		Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
		Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
		Form_parametry_linky->ShowModal();//návratová hodnota se řeši v knihovně
		REFRESH();
	}
	else
	{
		ESC();//zruší případnou rozdělanou akci
		if(d.v.OBJEKTY->dalsi==NULL)
		{
			MB("Nejdříve je nutné zakreslit schéma linky!");
		}
		else
		{
			{
				Form_definice_zakazek->Left=Form1->ClientWidth/2-Form_definice_zakazek->Width/2;
				Form_definice_zakazek->Top=Form1->ClientHeight/2-Form_definice_zakazek->Height/2;
				Form_definice_zakazek->ShowModal();
				if(!TZF)Analyza->Enabled=true;//stačí takto pokud první zakázka nepůjde smazat nebo se v případě neexistence bude vytvářet nová, což se momentálně děje při příchodu do časových os
				DuvodUlozit(true);//požaduje se vždy, protože i storno při prvním zobrazení ukládá default zakázku s default cestou
				REFRESH();//požaduje se vždy, protože i storno při prvním zobrazení ukládá default zakázku s default cestou a je tedy potřeba překreslit
			}
		}
	}
}
//---------------------------------------------------------------------------
//volání superformuláře (definice zakázek)
void __fastcall TForm1::scGPGlyphButton_definice_zakazekClick(TObject *Sender)
{

// není aktuálně využíváno, volá se pres společnou metodu Button_dopravnik_parametryClick
//	ESC();//zruší případnou rozdělanou akci
//	if(d.v.OBJEKTY->dalsi==NULL)
//	{
//		MB("Nejdříve je nutné zakreslit schéma linky!");
//	}
//	else
//	{
//		{
//			Form_definice_zakazek->Left=Form1->ClientWidth/2-Form_definice_zakazek->Width/2;
//			Form_definice_zakazek->Top=Form1->ClientHeight/2-Form_definice_zakazek->Height/2;
//			Form_definice_zakazek->ShowModal();
//			casovosa1->Enabled=true;//stačí takto pokud první zakázka nepůjde smazat nebo se v případě neexistence bude vytvářet nová, což se momentálně děje při příchodu do časových os
//			DuvodUlozit(true);//požaduje se vždy, protože i storno při prvním zobrazení ukládá default zakázku s default cestou
//			REFRESH();//požaduje se vždy, protože i storno při prvním zobrazení ukládá default zakázku s default cestou a je tedy potřeba překreslit
//		}
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DetailsButtonClick(TObject *Sender)
{
	ESC();//zruší případnou rozdělanou akci
	scSplitView_MENU->Opened = !scSplitView_MENU->Opened;
	if(scSplitView_OPTIONS->Opened) scSplitView_OPTIONS->Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//při změnně palce na tracBaru aktualizuje zoom a zavolá překreslení obrazu
void __fastcall TForm1::scGPTrackBar1Change(TObject *Sender)
{
		switch(scGPTrackBar1->Value)
		{
			case 0:Zoom=0.25;break;
			case 1:Zoom=0.5;break;
			case 2:Zoom=1;break;
			case 3:Zoom=1.5;break;
			case 4:Zoom=2;break;
			case 5:Zoom=2.5;break;
			case 6:Zoom=3;break;
			case 7:Zoom=3.5;break;
			case 8:Zoom=4;break;
			case 9:Zoom=4.5;break;
			case 10:Zoom=5;break;
      case 11:Zoom=6;break;
      case 12:Zoom=7;break;
      case 13:Zoom=8;break;
      case 14:Zoom=9;break;
      case 15:Zoom=10;break;
		}
		scLabel_ZOOM->Caption=AnsiString(Zoom*100)+" %";
		REFRESH();
}
//---------------------------------------------------------------------------
//při změně zoomu aktulizuje palec na trackBaru, inverzní funkce k funkci předchozí
void TForm1::on_change_zoom_change_scGPTrackBar()
{
	if(Zoom==0.25)	scGPTrackBar1->Value=0;
	if(Zoom==0.5)		scGPTrackBar1->Value=1;
	if(Zoom==1)			scGPTrackBar1->Value=2;
	if(Zoom==1.5)		scGPTrackBar1->Value=3;
	if(Zoom==2)			scGPTrackBar1->Value=4;
	if(Zoom==2.5)		scGPTrackBar1->Value=5;
	if(Zoom==3)			scGPTrackBar1->Value=6;
	if(Zoom==3.5)		scGPTrackBar1->Value=7;
	if(Zoom==4)			scGPTrackBar1->Value=8;
	if(Zoom==4.5)		scGPTrackBar1->Value=9;
	if(Zoom==5)			scGPTrackBar1->Value=10;
  if(Zoom==6)		  scGPTrackBar1->Value=11;
  if(Zoom==7)		  scGPTrackBar1->Value=12;
  if(Zoom==8)		  scGPTrackBar1->Value=13;
  if(Zoom==9)		  scGPTrackBar1->Value=14;
  if(Zoom==10)		scGPTrackBar1->Value=15;
	scLabel_ZOOM->Caption=AnsiString(Zoom*100)+" %";
}
//---------------------------------------------------------------------------
//přepínání režimu aplikace návrh ověřování
void __fastcall TForm1::scGPSwitch_rezimChangeState(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
 //	ShowMessage(STATUS);
	if(STATUS==NAVRH)
	{
		STATUS=OVEROVANI;

	 //	scGPGlyphButton_definice_zakazek->Visible=true;
	 //	scGPGlyphButton_parametry_linky->Visible=false;
	//	scGPGlyphButton_definice_zakazek->Top=scGPGlyphButton_parametry_linky->Top;
		//scLabel14->Visible=false;
		//scLabel14->Caption="Definice zakázek";
		SB("OVĚŘOVÁNÍ",1);
	 //	ShowMessage(STATUS);

			// scGPButton_header_def_zakazek->Visible=true;
			// scGPButton_header_param_linky->Visible=false;
			scGPButton_header_projekt->ImageIndex=48;
			scGPButton_header_projekt->Hint="Definice zakázek";


	}
	else
	{
		STATUS=NAVRH;
	 //	scLabel14->Visible=true;
	//	scLabel15->Visible=false;
		//scLabel14->Caption="Parametry linky";
	//	scGPGlyphButton_parametry_linky->Visible=true;
	//	scGPGlyphButton_parametry_linky->Top=scGPGlyphButton_definice_zakazek->Top;
	//	scGPGlyphButton_definice_zakazek->Visible=false;

	 //	 scGPButton_header_def_zakazek->Visible=false;
	 //	 scGPButton_header_param_linky->Visible=true;

		scGPButton_header_projekt->ImageIndex=49;
		scGPButton_header_projekt->Hint="Parametry linky";


		SB("NÁVRH",1);
	 //	ShowMessage(STATUS);
	}
}
//---------------------------------------------------------------------------
//pokud je při zavírání okna aktivní AA, tak udělá kopii pracovní oblasti AA vypne a zavře okno, bez toho by se zavíralo trhaně
void __fastcall TForm1::scSplitViewsClosing(TObject *Sender)
{
	scSplitViews_closing_on_AA=false;
	SplitViewOpen=true;
	if(antialiasing)
	{
		scSplitViews_closing_on_AA=true;
		int W=scSplitView_LEFTTOOLBAR->Width;
		short H=scLabel_titulek->Height;
		int Gh=vrat_max_vysku_grafu();
		Graphics::TBitmap *bmp_OPT_CLOSE=new Graphics::TBitmap;
		bmp_OPT_CLOSE->Width=ClientWidth-W;bmp_OPT_CLOSE->Height=ClientHeight-H-Gh;
		bmp_OPT_CLOSE->Canvas->CopyRect(Rect(0,0,ClientWidth-W,ClientHeight-H-Gh),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight-Gh));//zkopíruje aktuální výřez
		antialiasing=false;
		Canvas->Draw(0+W,0+H,bmp_OPT_CLOSE);
		delete (bmp_OPT_CLOSE);//velice nutné
	}
}
//---------------------------------------------------------------------------
//pokud byl AA zapnutý, tak navrácení do původního stavu na true
void __fastcall TForm1::scSplitViewsClosed(TObject *Sender)
{
		if(scSplitViews_closing_on_AA)antialiasing=true;
		SplitViewOpen=false;
		if(MOD==CASOVAOSA || TECHNOPROCESY)REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scExPanel_vrstvyClick(TObject *Sender)
{
	scExPanel_vrstvy->RollUpState=!scExPanel_vrstvy->RollUpState;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scExPanel_ostatniClick(TObject *Sender)
{
	scExPanel_ostatni->RollUpState=!scExPanel_ostatni->RollUpState;
	if(MOD==CASOVAOSA)
	{
		CheckBoxVytizenost->Visible=true;
		CheckBoxVymena_barev->Visible=true;
		ComboBoxCekani->Visible=true;
		scGPGlyphButton_info_cekani->Visible=true;
	}
}
//---------------------------------------------------------------------------
//vypnutí či zapnutí ortogonolazice
void __fastcall TForm1::scGPCheckBox_ortogonClick(TObject *Sender)
{
	if(!start_ortogonalizace)//pokud se nejedná o start aplikace, aby se nevolalo v tomto okamžiku
	ortogonalizace_on_off();
}
//---------------------------------------------------------------------------
//vypnutí měřítko
void __fastcall TForm1::scGPSwitch_meritkoChangeState(TObject *Sender)
{
	 scSplitView_MENU->Opened=false;
	 REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scSplitView_OPTIONSMouseLeave(TObject *Sender)
{
//    scSplitView_OPTIONS->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scSplitView_MENUMouseLeave(TObject *Sender)
{
//   scSplitView_MENU->Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::hl_spojak_zakazkyClick(TObject *Sender)
{
//	Cvektory::TObjekt *p=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
//
//	Memo2->Lines->Add(AnsiString(p->name)+";"+AnsiString(p->short_name)+";"+AnsiString(p->rezim)+";"+AnsiString(p->pohon->n)+";"+AnsiString(p->delka_dopravniku)+";"+AnsiString(p->cekat_na_palce)+";"+AnsiString(p->odchylka)+";"+AnsiString(p->kapacita));

		Cvektory::TZakazka *ukaz2=d.v.ZAKAZKY->dalsi;
		if (ukaz2==NULL)
		{
ShowMessage("NEmam data");
Analyza->Enabled=false;
		}
		else {
		 ShowMessage("mam data");
		 Analyza->Enabled=true;
		 } //	casovosa1->Enabled=true; }




		Cvektory::TZakazka *ukaz=d.v.ZAKAZKY->dalsi;
		while (ukaz!=NULL)
		{
			Memo2->Lines->Add(AnsiString(ukaz->name)+";/predchozi:"+AnsiString(ukaz->predchozi->name)+"/;"+AnsiString(ukaz->barva)+";"+AnsiString(ukaz->pomer)+";"+AnsiString(ukaz->pocet_voziku)+";"+AnsiString(ukaz->serv_vozik_pocet)+";"+AnsiString(ukaz->TT));
			ukaz=ukaz->dalsi;
		}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::button_zakazky_tempClick(TObject *Sender)
{
		Cvektory::TZakazka *ukaz=d.v.ZAKAZKY_temp->dalsi;
		while (ukaz!=NULL)
		{
				Memo2->Lines->Add(AnsiString(ukaz->name)+";"+AnsiString(ukaz->barva)+";"+AnsiString(ukaz->pomer)+";"+AnsiString(ukaz->pocet_voziku)+";"+AnsiString(ukaz->serv_vozik_pocet)+";"+AnsiString(ukaz->TT));
				ukaz=ukaz->dalsi;
		}
}
//---------------------------------------------------------------------------
//volání eDesignera
void __fastcall TForm1::scButton2Click(TObject *Sender)
{
	 scSplitView_MENU->Close();//zavře menu
	 Novy_soubor();//prvně zavřu starý, který ho donutím uložit a vytvořím nový soubor
	 //Form_eDesigner->Show();
	 AnsiString FileNameSablona="sablona.tisplTemp";

	 //pokud šablona neexistuje, zkusí ji stáhnout
	 //mohla by být v onshow v uvodu
	 if(!FileExists(ExtractFilePath(Application->ExeName)+FileNameSablona))
	 {
			try
			{
					zobraz_tip("Probíhá stahování šablony...");
					TMemoryStream *MemoryStream=new TMemoryStream();
					IdHTTP1->Get("http://85.255.0.237/TISPL/"+FileNameSablona,MemoryStream);
					MemoryStream->SaveToFile(ExtractFilePath(Application->ExeName)+FileNameSablona);//stáhne k souboru exe
			}
			catch(...){;}//v případě nedostupnosti internetu atp.
	 }

	 //pokud soubor šablony existuje
	 if(FileExists(ExtractFilePath(Application->ExeName)+FileNameSablona))
	 {
		 AnsiString T1="Následně bude otevřena šablona. Zadejte název projektu, pod kterým bude uložena.";
		 MB(T1);SB(T1);
		 Otevrit_soubor(ExtractFilePath(Application->ExeName)+FileNameSablona); //volaný *.tispl musí být tam, kde exe tisplu nebo zadat adresu
		 DuvodUlozit(true);
		 UlozitjakoClick(this);

		 AnsiString T2="Zadejte parametry linky nebo zvolte storno.";
		 //MB(T2);
		 Form_parametry_linky->rHTMLLabel_eDesigner->Visible=true;
		 Form_parametry_linky->rHTMLLabel_eDesigner->Caption=T2;
		 SB(T2);//zobraz_tip(T2);
		 Button_dopravnik_parametryClick(Sender);//volání parametrů linky

		 AnsiString T3="Zadejte parametry zakázek nebo zvolte storno.";
		 //MB(T3);
		 Form_definice_zakazek->rHTMLLabel_eDesigner->Visible=true;
		 Form_definice_zakazek->rHTMLLabel_eDesigner->Caption=T3;
		 SB(T3);//zobraz_tip(T3);
		 scGPGlyphButton_definice_zakazekClick(Sender);//volání superformuláře (definice zakázek)

		 AnsiString T4="Vytvořte schéma a pomocí pravého tlačítka nastavte parametry objektů.";
		 SB(T4);zobraz_tip(T4);MB(T4,MB_OK,false);

		 Form_parametry_linky->rHTMLLabel_eDesigner->Visible=false;
		 Form_definice_zakazek->rHTMLLabel_eDesigner->Visible=false;
		 DuvodUlozit(true);
	 }
	 else//nebyla nalezena šablona
	 {
		 MB("Nepodařilo se najít nebo stáhnout šablonu. Průvodce nebude spuštěn!");
	 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxCekaniChange(TObject *Sender)
{
	//pokud bych chtěl používat i v v modu vytzení objektu, ale při znovu výberu náhodne hodnoty házelo chybu:if(d.mod_vytizenost_objektu){d.JIZPOCITANO=false;d.mod_vytizenost_objektu=false;d.vykresli_casove_osy(Canvas);d.mod_vytizenost_objektu=true;}
	if(ComboBoxCekani->ItemIndex==2)//tak se zobrazí vedle toho comba tlačítko pro nové generování
	{
		scGPButton_generuj->Visible=true;
		ComboBoxCekani->Width=196;
	}
	else
	{
		Form1->scGPButton_generuj->Visible=false;
		ComboBoxCekani->Width=scSplitView_OPTIONS->Width-7;
	}
	//pro uživatele kontrola, zda mají objekty přiřazené pohony a pohony, zda mají přiřazené rozteče
	if(ComboBoxCekani->ItemIndex>0)
	{
		//kontrola přiřazení pohonů + nabídky na nápravu (pouze ve statusu NAVRH)
		AnsiString T=d.v.vypsat_objekty_bez_prirazenych_pohonu();
		if(T!="")
		{
			if(mrYes==MB("Pozor, pro objekt "+T+" nebyl přiřazen pohon. Doba čekání na palce není u těchto objektů zohledněna!<br><b>Chcete pohony objektům přiřadit nyní?</b>",MB_YESNO) && STATUS==NAVRH)
			{
				Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
				while(O!=NULL)
				{
					if(O->pohon==NULL)
					{
						MessageBeep(0);
						pom=O;
						if(Form_parametry->ClientHeight==646){int H=366;if(O->rezim==1)H=526;if(O->rezim==1)H=486;Form_parametry->ClientHeight=H;}//pokud se jedná o první spuštění, protože jinak je neznámá výška formu
						akt_souradnice_kurzoru_PX.x=Form1->ClientWidth/2-Form_parametry->ClientWidth/2-10;
						akt_souradnice_kurzoru_PX.y=Form1->ClientHeight/2-Form_parametry->ClientHeight/2-10;
						NP();//volá formulář parametry objektů pro přiřazení pohonu ke konkrétnímu objektu//volá form na nastevení parametrů, dřívější nastavparemetry1click
					}
					O=O->dalsi;
				}
				O=NULL;delete O;
			}
		}
		//kontrola přiřazení rozteče + nabídka na nápravu (pouze ve statusu NAVRH)
		T=d.v.vypis_objekty_s_pohony_bez_roztece();
		if(T!="")
		{
			if(mrYes==MB("Pozor, následující pohony nemají uvedenou rozteč palců řetezu: "+T+". Doba čekání na palce není tedy u uvedených objektů zohledněna!<br><b>Chcete chybějící rozteče pohonů nyní zadat?</b>",MB_YESNO) && STATUS==NAVRH)
			{
				Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
				Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
				Form_parametry_linky->ShowModal();//návratová hodnota se řeši v knihovně
			}
		}
	}
	if(ComboBoxCekani->ItemIndex==2 && d.RANDOM)d.JIZPOCITANO=false;//musí být před, zajístí přepočet dle nových hodnot
	REFRESH();
	if(ComboBoxCekani->ItemIndex==2)d.RANDOM=false;//musí být až za refresh
	RM();//korekce chyby oskakování pravého menu, je zajímavé, že tu musí být znovu

}
//---------------------------------------------------------------------------


void __fastcall TForm1::scExPanel_log_headerClose(TObject *Sender)
{
//	scExPanel_log_header->Visible=false;
	scGPGlyphButton_zpravy_ikona->Visible=true;
	scExPanel_log_header->Visible=false;
	scSplitView_OPTIONS->Opened=false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPGlyphButton_zpravy_ikonaClick(TObject *Sender)
{
	if(scExPanel_log_header->Visible==false)
	{
		scExPanel_log_header->Visible=true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scSplitView_OPTIONSOpened(TObject *Sender)
{
	scSplitView_OPTIONS->Left=ClientWidth-scSplitView_OPTIONS->OpenedWidth;

	scGPButton_generuj->Options->NormalColor=scSplitView_OPTIONS->Color;
	scGPButton_generuj->Options->FocusedColor=scGPButton_generuj->Options->NormalColor;
	scGPButton_generuj->Options->HotColor=scGPButton_generuj->Options->NormalColor;
	scGPButton_generuj->Options->PressedColor=scGPButton_generuj->Options->NormalColor;
	scGPButton_generuj->Options->FramePressedColor=scGPButton_generuj->Options->NormalColor;

	ButtonPLAY->Options->NormalColor=scSplitView_OPTIONS->Color;
	ButtonPLAY->Options->FocusedColor=scGPButton_generuj->Options->NormalColor;
	ButtonPLAY->Options->HotColor=scGPButton_generuj->Options->NormalColor;
	ButtonPLAY->Options->PressedColor=scGPButton_generuj->Options->NormalColor;
	ButtonPLAY->Options->FramePressedColor=scGPButton_generuj->Options->NormalColor;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::pohonobjektClick(TObject *Sender)
{
	Cvektory::TObjekt *O=Form1->d.v.OBJEKTY->dalsi;


		while(O!=NULL)
		{
	 Memo2->Lines->Add(O->pohon->n);

		O=O->dalsi;
		 }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender)
{
	Memo2->Visible=true;
	Cvektory::TPohon *P=d.v.POHONY->dalsi;
	while(P!=NULL)
	{
		Memo2->Lines->Add(P->name);
		P=P->dalsi;
	}

	Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
	while(O!=NULL)
	{
		Memo2->Lines->Add(O->pohon->name);
		if(O->pohon==d.v.POHONY->dalsi)
		Memo2->Lines->Add("stejný");
		else Memo2->Lines->Add("není stejný");


			O=O->dalsi;
	}
}
//---------------------------------------------------------------------------
//při otevření panelu
void __fastcall TForm1::scSplitView_OPTIONSPanelPaint(TCanvas *ACanvas, TRect &ARect)
{
//užitečná metoda při otevření panelu, onclosing nějak nefunguje, toto ano
}
//---------------------------------------------------------------------------
//zajišťuje vygenerování nové doby čekání na palec
void __fastcall TForm1::scGPButton_generujClick(TObject *Sender)
{
		d.RANDOM=true;
		d.JIZPOCITANO=false;
		REFRESH();
		d.RANDOM=false;
}
//---------------------------------------------------------------------------
//korekce chyby oskakování pravého menu
void TForm1::RM()
{
	if(scSplitView_OPTIONS->Visible)scSplitView_OPTIONS->Align=alRight;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scExPanel_log_headerClick(TObject *Sender)
{
scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scExPanel_log_headerMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::scExPanel_log_headerMouseActivate(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)

{
scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scSplitView_MENUOpened(TObject *Sender)
{
	// generování buttonu na PL nebo DF dle přepínače režimu
	if(STATUS==NAVRH)
	{
		scButton_parmlinky_defzakazek->Caption="Parametry linky";
		scButton_parmlinky_defzakazek->ImageIndex=49;
	}
	if (STATUS==OVEROVANI)
	{
		scButton_parmlinky_defzakazek->Caption="Definice zakázek";
		scButton_parmlinky_defzakazek->ImageIndex=48;
	}
}
//---------------------------------------------------------------------------
//v případě zakliknutí či odkliknutí zohlední změny dle počtu vozíku (buď podle WIP či nikoliv)
void __fastcall TForm1::scGPCheckBox_pocet_voziku_dle_WIPClick(TObject *Sender)
{
	aktualizace_maro_a_roma();
	REFRESH();
}
//---------------------------------------------------------------------------
//vypíše nápovědu k algoritmu čekání na palce
void __fastcall TForm1::scGPGlyphButton_info_cekaniClick(TObject *Sender)
{
	MB("<b>Čekání na palce nastane</b> dle nastavení v parametrech objektu (není-li uživatelsky zakázáno):"+AnsiString("<br>")+
	"• po odchodu z objektu v S&G režimu<br>"+
	"• mezi kontinuálními objekty, mají-li jiný pohon<br>"+
	"• mezi postprocesními objekty, mají-li jiný pohon<br>"+
	"• mezi kontinuálním a postprocesním objektem, mají-li jiný pohon<br>"+
	"• mezi postprocesním a kontinuálním objektem<br>"+
	"• vždy, když je za objektem definovaná stopka<br>"+
	"• vždy, když je čekání uživatelsky nařízeno",MB_OK,false,489);
}
//---------------------------------------------------------------------------
void TForm1::db_connection()
{
	FDConnection1->Params->Database="tispl_db";
	FDConnection1->Params->UserName="app_tispl";
	FDConnection1->Params->Password="NbtDnKJqGX76rVeB";
	FDConnection1->Params->DriverID="Mysql";
	FDConnection1->Params->Add("Server=81.2.243.72");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button11Click(TObject *Sender)
{
//	Form2->ShowModal();
Memo3->Visible=true;
Cvektory::TElement *E=d.v.OBJEKTY->dalsi->elementy->dalsi;
while(E!=NULL)
{
	 Memo3->Lines->Add(E->n);
	 E=E->dalsi;
}
E=NULL;delete E;

// if(MOD==NAHLED)
//   {
//   DrawGrid_knihovna->DefaultRowHeight=50;
//   scListGroupPanel_hlavickaOtoce->Visible=false;
//   scListGroupPanel_hlavickaOstatni->Visible=false;
//   scListGroupPanel_geometrie->Visible=false;
//   scListGroupPanel_poznamky->Visible=false;
//   DrawGrid_knihovna->Height=400;
//   scListGroupKnihovObjektu->Align=alLeft;
//   DrawGrid_knihovna->Invalidate();
//   MOD=SCHEMA;
//   }
//   else
//   {
//
//   MOD=NAHLED;
//   DrawGrid_knihovna->DefaultRowHeight=100;
//   DrawGrid_knihovna->Height=DrawGrid_knihovna->DefaultRowHeight*2; // dle počtu řádků
//   DrawGrid_knihovna->Invalidate();
//   scListGroupPanel_hlavickaOtoce->Visible=true;
//   scListGroupPanel_hlavickaOtoce->Top=DrawGrid_knihovna->Height + scGPPanel_mainmenu->Height;
//   DrawGrid_otoce->Visible=true;
//   scListGroupPanel_hlavickaOstatni->Visible=true;
//   scListGroupPanel_hlavickaOstatni->Top=scListGroupPanel_hlavickaOtoce->Top + scListGroupPanel_hlavickaOtoce->Height;
//   DrawGrid_ostatni->Visible=true;
//   scListGroupPanel_geometrie->Visible=true;
//   scListGroupPanel_geometrie->Top=scListGroupPanel_hlavickaOstatni->Top +   scListGroupPanel_hlavickaOstatni->Height;
//   DrawGrid_geometrie->Visible=true;
//   scListGroupPanel_poznamky->Visible=true;
//   scListGroupPanel_poznamky->Top= scListGroupPanel_geometrie->Top + scListGroupPanel_geometrie->Height;
//   DrawGrid_poznamky->Visible=true;
//   }

//	d.v.rotace_elementu(pom_temp,90);
//	REFRESH();



}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_stornoClick(TObject *Sender)
{
	if(MOD==NAHLED)  //navrácení původní knihovny do módu schema
	{
		//vypnutí spodního panelu
		scGPPanel_bottomtoolbar->Visible=false;
		//změna horní lišty
		scGPPanel_mainmenu->Visible=true;
		scGPPanel_schema->Visible=false;

		//navrácení zoomu a posunu do původních hodnt
		Zoom=Zoom_predchozi2;
		on_change_zoom_change_scGPTrackBar();
		Posun.x=Posun_predchozi2.x;
		Posun.y=Posun_predchozi2.y;

		DrawGrid_knihovna->DefaultRowHeight=50;
		DrawGrid_knihovna->DefaultColWidth=70;
		DrawGrid_knihovna->Left=14;

		scListGroupPanel_hlavickaOtoce->Visible=false;
		scListGroupPanel_hlavickaOstatni->Visible=false;
		scListGroupPanel_geometrie->Visible=false;
		scListGroupPanel_poznamky->Visible=false;
		DrawGrid_knihovna->Height=400;
		scListGroupKnihovObjektu->Align=alLeft;
		DrawGrid_knihovna->Invalidate();

		scGPLabel_roboti->Visible=false;
		scGPLabel_otoce->Visible=false;
		scGPLabel_stop->Visible=false;
		scGPLabel_geometrie->Visible=false;
		scGPLabel_poznamky->Visible=false;

		scGPButton_OK->Visible=false;
		scGPButton_storno->Visible=false;

		scGPLabel_roboti->Font->Style = TFontStyles(); // zrušení tučného písma resp. všech případných dalších Font style nastavení
		scGPLabel_roboti->Visible=true;
		scGPLabel_roboti->Caption="Technolog. objekty";
		scGPLabel_roboti->ContentMarginLeft=4;

		MOD=SCHEMA;
		REFRESH();
		pom=NULL;
		d.v.vymaz_elementy(pom_temp,false);
		pom_temp=NULL; delete pom_temp;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton_nacist_podkladClick(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
	OpenDialog1->Title="Načíst podklad";
	OpenDialog1->DefaultExt="*.bmp";
	OpenDialog1->Filter="Soubory formátu bmp (*.bmp)|*.bmp";
	if(OpenDialog1->Execute())
	{
		//načtení podkladu
		Nacist_podklad(OpenDialog1->FileName);
	}
}
//---------------------------------------------------------------------------
 unsigned short int  TForm1::Nacist_podklad(UnicodeString soubor)
 {
  d.v.PP.raster.show=true;
	d.v.PP.raster.filename=soubor;
	d.v.PP.raster.X=m.P2Lx(scSplitView_LEFTTOOLBAR->Width +10);d.v.PP.raster.Y=m.P2Ly(scGPPanel_mainmenu->Height + 10);//souřadnice v metrech
	d.v.PP.raster.resolution=m2px;  //výpočet metry děleno počet PX, výchozí zobrazení v nativním rozlišení (bez usazení do metrického měřítka) je 0.1
  scGPCheckBox_zobraz_podklad->Checked=true;
  scButton_nacist_podklad->Down=false;  //ošetření proti tmavému vysvícení při dalším zobrazení mainmenu
  REFRESH();

  zobraz_tip("Pro správné umístění a nastavení měřítka podkladu, využijte volbu v pravém horním menu.");
  if(mrOk==MB("Pro správné umístění a nastavení měřítka podkladu, využijte volbu v pravém horním menu. Přejít přímo do nastavení?",MB_OKCANCEL))
  {
  //scGPGlyphButton_OPTIONSClick(Sender);  //tohle nesežral
  scSplitView_OPTIONS->Opened=true;

  scGPCheckBox_zobraz_podklad->Enabled=true;
  scGPCheckBox_stupne_sedi->Enabled=true;
  scGPTrackBar_svetelnost_posuvka->Enabled=true;
  scLabel1_svetelnost->Enabled=true;
  scGPButton_kalibrace->Enabled=true;
  scGPButton_adjustace->Enabled=true;
  }
  else
  {
  scGPGlyphButton_OPTIONS->Active=true;
  //scGPGlyphButton_OPTIONS->GlyphColor= (TColor)RGB(255, 128, 0);
  //scGPGlyphButton_OPTIONS->GlyphThickness=2;
  scGPGlyphButton_OPTIONS->ShowHint=true;
  }

}
 //--------------------------------------------------------------
void __fastcall TForm1::DrawGrid_geometrieDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{

	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
	int W=DrawGrid_geometrie->DefaultColWidth  *Z;
	int H=DrawGrid_geometrie->DefaultRowHeight  *Z;
	int P=-1*DrawGrid_geometrie->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

	Cantialising a;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=DrawGrid_geometrie->Width*Z;bmp_in->Height=DrawGrid_geometrie->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
	double Zoom_back=Zoom;//záloha zoomu
	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
	short pocet_elementu=2;
  AnsiString label1;
  AnsiString label2;
	for(unsigned short n=1;n<=pocet_elementu;n++)
	{
    if(n==1)
    {
     label1= "linie";
     label2="";
    if(pom->id!=3) d.vykresli_ikonu_linie(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1);
    else           d.vykresli_ikonu_linie(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1,-1);
     }
    if(n==2)
    {
     label1= "oblouky";
     label2="";
     if(pom->id!=3) d.vykresli_ikonu_oblouku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1);
     else           d.vykresli_ikonu_oblouku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1,-1);
     }

	}
	Zoom=Zoom_back;//návrácení původního zoomu
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	DrawGrid_geometrie->Canvas->Draw(0,0,bmp_out);
	delete (bmp_out);//velice nutné
	delete (bmp_in);//velice nutné
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_zobraz_podkladClick(TObject *Sender)
{
	if(!scGPCheckBox_zobraz_podklad->Checked && scSplitView_OPTIONS->Opened)
	{
		d.v.PP.raster.show=false;
		scSplitView_OPTIONS->Opened=false;
		scGPCheckBox_zobraz_podklad->Checked=false;
		DuvodUlozit(true);
	}

	if(scGPCheckBox_zobraz_podklad->Checked && scSplitView_OPTIONS->Opened)
	{
		d.v.PP.raster.show=true;
		scSplitView_OPTIONS->Opened=false;
		scGPCheckBox_zobraz_podklad->Checked=true;
		DuvodUlozit(true);
	}
	REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_kalibraceClick(TObject *Sender)
{
	kurzor(add_o);//pozor kurzor kalibrovat neni vycentrovaný, je třeba jej imageeditoru předělat
	Akce=KALIBRACE;
  scSplitView_OPTIONS->Opened=false;
   }
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_adjustaceClick(TObject *Sender)
{
	kurzor(add_o);
  Akce=ADJUSTACE;
  scSplitView_OPTIONS->Opened=false;
  }
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPGlyphButton_OPTIONSClick(TObject *Sender)
{
  ESC();//zruší případnou rozdělanou akci  - způsobuje problém při zavírání splitview
  scSplitView_OPTIONS->Opened = !scSplitView_OPTIONS->Opened;

  scGPGlyphButton_OPTIONS->Active=false;
  scGPGlyphButton_OPTIONS->GlyphColor=clWhite;
  scGPGlyphButton_OPTIONS->GlyphThickness=1;

 	//nastavení checkboxu na checked pokud je již rastr uložen a má být zobrazen
	if(d.v.PP.raster.filename!="" &&  d.v.PP.raster.show)
	{
		if(FileExists(d.v.PP.raster.filename))
    {
   // ShowMessage(d.v.PP.raster.dim);
    scGPCheckBox_zobraz_podklad->Checked=true;
    scGPTrackBar_svetelnost_posuvka->Value=d.v.PP.raster.dim;
    if(d.v.PP.raster.grayscale) scGPCheckBox_stupne_sedi->Checked=true;
    }
  }

  if(d.v.PP.raster.filename=="")
  {
  scGPCheckBox_zobraz_podklad->Enabled=false;
  scGPCheckBox_stupne_sedi->Enabled=false;
  scGPTrackBar_svetelnost_posuvka->Enabled=false;
  scLabel1_svetelnost->Enabled=false;
  scGPButton_adjustace->Enabled=false;
  scGPButton_kalibrace->Enabled=false;
  scLabel1_svetelnost->Enabled=false;

  }
  else
  {
  scGPCheckBox_zobraz_podklad->Enabled=true;
  scGPCheckBox_stupne_sedi->Enabled=true;
  scGPTrackBar_svetelnost_posuvka->Enabled=true;
  scLabel1_svetelnost->Enabled=true;
  scGPButton_adjustace->Enabled=true;
  scGPButton_kalibrace->Enabled=true;
  scLabel1_svetelnost->Enabled=true;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::scGPGlyphButton_OPTIONSMouseEnter(TObject *Sender)
{
    scGPGlyphButton_OPTIONS->GlyphColor=(TColor)RGB(0,120,215);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::scGPGlyphButton_OPTIONSMouseLeave(TObject *Sender)
{
scGPGlyphButton_OPTIONS->GlyphColor=clWhite;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::scExPanel_podkladClick(TObject *Sender)
{
//
	scExPanel_podklad->RollUpState=!scExPanel_podklad->RollUpState;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPTrackBar_svetelnost_posuvkaChange(TObject *Sender)
{
d.v.PP.raster.dim=scGPTrackBar_svetelnost_posuvka->Value;
REFRESH();
DuvodUlozit(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPCheckBox_stupne_sediClick(TObject *Sender)
{
 if(!scGPCheckBox_stupne_sedi->Checked && scSplitView_OPTIONS->Opened)
    {
    d.v.PP.raster.grayscale=false;
    scSplitView_OPTIONS->Opened=false;
    scGPCheckBox_stupne_sedi->Checked=false;
    DuvodUlozit(true);
    }

 if(scGPCheckBox_stupne_sedi->Checked && scSplitView_OPTIONS->Opened)
    {
    d.v.PP.raster.grayscale=true;
    scSplitView_OPTIONS->Opened=false;
    scGPCheckBox_stupne_sedi->Checked=true;
    DuvodUlozit(true);
    }

 REFRESH();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::DrawGrid_ostatniMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
FormMouseWheelDown(Sender,Shift,MousePos,Handled);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_geometrieMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
//FormMouseWheelDown(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_poznamkyMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
FormMouseWheelDown(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_ostatniMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_geometrieMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_poznamkyMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button_testClick(TObject *Sender)
{
MOD=SCHEMA;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::Timer1Timer(TObject *Sender)
{



		if(ROs==0)
		{
			if(RO>-(1.2*Zoom/m2px))
			{
				RO-=(1.2*Zoom/m2px)/20.0;
			}
			else ROs=1;
		}
		else
		{
			if(RO<(1.2*Zoom/m2px))
			{
				RO+=(1.2*Zoom/m2px)/20.0;
			}
			else ROs=0;
		}

		if(ROsts==0)
		{
			if(ROst<30)ROst+=5;
			else ROsts=1;
		}
		else
		{
			if(ROst>-30)ROst-=5;
			else ROsts=0;
		}

		REFRESH();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPButton_OKClick(TObject *Sender)
{
	d.v.vymaz_elementy(pom,false);
	d.v.kopiruj_objekt(pom_temp,pom);
	scGPButton_stornoClick(Sender);//další funkcionalita je již stejná jako ve stornu, včetně vymazání ukazatele pom_temp včetně jeho elementů
}
//---------------------------------------------------------------------------

double TForm1::inLO(double inLO)
{
//input metoda na převod jednotek LO do Si
	double LO=inLO;
	if (LOunit == MM) LO=inLO/1000.0;
	return LO;
}

double TForm1::outLO(double outLO)
{
//output metoda na převod jednotek z jádra do jednotek dle INI
	double LO=outLO;
	if (LOunit == MM) LO=outLO*1000.0;
	return LO;
}
double TForm1::inPT(double inPT)
{
	double PT=inPT;
	if (PTunit == MIN) PT=inPT*60.0;
	return PT;

}

double TForm1::outPT(double outPT)
{
	double PT=outPT;
	if (PTunit == MIN) PT=outPT/60.0;
	return PT;
}

double TForm1::inDO(double inDO)
{
	double DO=inDO;
	if (DOtocunit == MM) DO=inDO/1000.0;
	return DO;
}

double TForm1::outDO(double outDO)
{
	double DO=outDO;
	if (DOtocunit == MM) DO=outDO*1000.0;
	return DO;
}


void __fastcall TForm1::scGPEdit1Change(TObject *Sender)
{
Memo3->Lines->Add("onchange");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
 F->REFRESH();
 Timer2->Enabled=false;
}
//---------------------------------------------------------------------------

