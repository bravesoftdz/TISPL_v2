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
#pragma resource "*.dfm"
TForm1 *Form1;
AnsiString Parametry;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	srand(time(NULL));//nutno tady

	NewDesignSettings();//záležitost s novým designem

	m2px=0.1;//uchovává hodnotu prostorového rozlišení programu, nativní rozlišení 0,1 m na 1 pixel při zoomu 1x

	//vytvoření TEMP adresáře (pro ini)
	MkDir(get_temp_dir()+"TISPL");

	////nastavení aplikace
	upozornovat_na_zmenu_TT_parametru=true;

	//nastavení knihovnky
	//DrawGrid_knihovna->Enabled=false;
	DrawGrid_knihovna->RowCount=pocet_objektu_knihovny/2;//velikosti buněk
	if(pocet_objektu_knihovny>0)vybrany_objekt=0;else//-použival jsem v době kdy jsem chtěl mít implicitní prvek pokud existuje nějaký prvek v knihovně, tak nastaví vybraný prvek jako první
	vybrany_objekt=-1;

	//mřížka
	grid=true; size_grid=10;//velikost je v logických jednotkách (metrech)
	prichytavat_k_mrizce=0;
	SB("přichytávat?",5);
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

	//povolení Automatická záloha
	Timer_backup->Enabled=true;

	pocitadlo_doby_neaktivity=0;

	antialiasing=true;

	//nastavení implicitního souboru
	duvod_k_ulozeni=false;
	NovySouborClick(this);

	//zjistí verzi aplikace
	DWORD  ver=GetFileVersion(Application->ExeName);
	VERZE=UnicodeString(HIWORD(ver))+"."+UnicodeString(LOWORD(ver));
	//VERZE=UnicodeString(UnicodeString(HIWORD(ver) >> 16)+"."+UnicodeString(HIWORD(ver) & 0xFFFF)+"."+UnicodeString(LOWORD(ver) >> 16)+"."+UnicodeString(LOWORD(ver) & 0xFFFF));

	//licence atp.
	Caption="ELTEP - tispl";
	scLabel_titulek->Caption=Caption+" - [Nový.tispl]";
	LICENCE="TRIAL_GALATEK";
	EDICE=ARCHITECT;//ARCHITECT,CLIENT,VIEWER,DEMO
	edice();//zakázání či povolení grafických uživatelských prvků dle úrovně edice

}
//---------------------------------------------------------------------------
//záležitost s novým designem
void TForm1::NewDesignSettings()
{
	//maximalizace formuláře jinak to s novým designem nejde
	Form1->Width=Screen->WorkAreaWidth;
	Form1->Height=Screen->WorkAreaHeight;
	FMaximized=false;MaxButtonClick(this);//aby bylo připraveno minimalizační tlačítko

	//nastavení globálních barev
	TColor light_gray=(TColor)RGB(240,240,240);
	TColor active_blue=(TColor)RGB(0,120,215);

	PopupMenuButton->Left = 0;
	PopupMenuButton->Visible = false;
	DetailsButton->Left = 0;
	DetailsButton->Visible = true;
	scSplitView_OPTIONS->Opened=false;
	scSplitView_OPTIONS->Align=alRight;

	scGPGlyphButton_definice_zakazek->Options->NormalColor=active_blue;
	scGPGlyphButton_parametry_linky->Options->NormalColor=active_blue;
	scListGroupNastavProjektu->HeaderAutoColor=true;
	scListGroupKnihovObjektu->HeaderAutoColor=true;
	scListGroupNastavProjektu->Color=light_gray;
	scListGroupKnihovObjektu->Color=light_gray;
	//scSplitView_OPTIONS->Color=light_gray;
 //	scExPanel_vrstvy->Color=light_gray;
 //	scExPanel_ostatni->Color=light_gray;

	//nastaveni barvy prepinacu modu
	editacelinky1->Options->PressedColor=light_gray;
	layout->Options->PressedColor=light_gray;
	casovosa1->Options->PressedColor=light_gray;
	technologickprocesy1->Options->PressedColor=light_gray;
	simulace->Options->PressedColor=light_gray;

	scExPanel_ostatni->Top=72+27;

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
				NovySoubor->Enabled=false;
				Otevrit->Enabled=false;
			//	Otevritsablonu->Enabled=false;
				Ulozit->Enabled=false;
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
		UnicodeString AKT_VERZE=IdHTTP1->Get("http://85.255.0.237/TISPL/verze.txt");
		//porovnání akt. verze a používané verze aplikace
		if(AKT_VERZE!=VERZE)//je k dispozici aktualizace
		{
			if(mrYes==MB("Je k dispozici aktualizace TISPLu. Chcete ji stáhnout?",MB_YESNO,false))
			{
				MB("Po dokončení staženým souborem přepište současný EXE soubor.");
				zobraz_tip("Probíhá stahování aktualizace, po dokončení stahování bude program ukončen.Staženým souborem přepište současný EXE soubor.");
				SaveDialog->Title="Uložit soubor jako";
				SaveDialog->DefaultExt="*.exe";
				SaveDialog->Filter="Aktualizace TISPLu (*.exe)|*.exe";
				SaveDialog->FileName="TISPL_aktualizace";
				if(SaveDialog->Execute())
				{
						TMemoryStream *MemoryStream=new TMemoryStream();
						IdHTTP1->Get("http://85.255.0.237/TISPL/LIC/"+LICENCE+"/tispl.exe",MemoryStream);
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
	// startUP() - pokud byl zde, dělalo to "chybu v paměti" při spuštění release verze	startUP();//při aktivaci formuláře startující záležitosti, pro zpřehlednění ko


	}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//založí nový soubor, nastavení souboru, nastevení aplikace v konstruktoru
void __fastcall TForm1::NovySouborClick(TObject *Sender)
{
	 scSplitView_MENU->Opened=false;
	 bool novy=true;
	 if(duvod_k_ulozeni)
	 {
			int result=MB(FileName_short(FileName)+" byl změněn.\nChcete ho před ukončením uložit?",MB_YESNOCANCEL);
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
			 d.v.vloz_pohon("Hlavní dopravník",1.5,10.5,32.5);
			 d.v.vloz_pohon("Vedlejší dopravník",1.5,10.5,32.5);

    	 editacelinky1Click(Sender);//MOD EDITACE LINKY
    	 Zoom=1.0; on_change_zoom_change_scGPTrackBar();
    	 Zoom_predchozi=1.0;
			 Posun.x=-scListGroupNastavProjektu->Width;if(vyska_menu>0)Posun.y=-vyska_menu+9;else Posun.y=-29;
			 Posun_predchozi.x=Posun.x;Posun_predchozi.y=Posun.y;
			 jedno_ze_tri_otoceni_koleckem_mysi=1;
			 doba_neotaceni_mysi=0;

			 d.v.PP.cas_start=TDateTime(AnsiString(TIME.CurrentDate().DateString())+" "+"8:00:00");//defaultně dnes v 8:00
			 d.v.PP.mnozstvi=200;
			 d.v.PP.hod_den=8;
			 d.v.PP.dni_rok=365;
			 d.v.PP.efektivita=95;
			 d.v.PP.delka_voziku=1;
			 d.v.PP.typ_voziku=0;

			 Akce=NIC;Screen->Cursor=crDefault;//změní kurzor na default

			 DuvodUlozit(false);
			 RzToolButton4->Enabled=false;
			 RzToolButton5->Enabled=false;
			 uchop_zobrazen=false;
    	 vycentrovat=true;
    	 posun_objektu=false;//nutnost, aby se během realizace posunu neaktivoval další posun
    	 zneplatnit_minulesouradnice();
    	 probehl_zoom=false;
    	 add_posledni=true;
    	 stisknute_leve_tlacitko_mysi=false;
    	 funkcni_klavesa=0;
			 pan_non_locked=false;
			 zobrazit_barvy_casovych_rezerv=false;
			 d.cas=0;

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
		//toto odkomentovat pro spuštění TTR
		if(!ttr("start"))
		{
			Timer_tr->Enabled=false;//ještě je ale z důvodu ochrany enabled=true v object inspectoru, toto je spíše na zmatení
			Close();
		}
		else
		//Timer_tr->Enabled=false;// toto zakomentovat po spuštění TTR
		startUP();//toto vždy odkomentované
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
		FDQuery1->Open("SELECT  DATE_FORMAT(expiration_date ,'%d.%m.%Y %h:%m:%s') AS expiration_date FROM app_setup WHERE id=\"1\"");  //id nahradit id z ini
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
						S(Text_error);//vypršela licence resp. program expiroval;
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
			S(Text_error);
			duvod_k_ulozeni=false;
			Timer_tr->Enabled=false;
			Close();
		}
	}
	catch(...)//nezdařilo se připojení k licenčnímu serveru
	{
		//tady nemůže být log
		S(Text_error);
		duvod_k_ulozeni=false;
		Timer_tr->Enabled=false;
		Close();
	}

	if(!STATUS)//dvojúrovňová ochranu
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
	////zalogování startu aplikace

	AnsiString relation_id=GetCurrentProcessId();
	AnsiString send_log_time= TIME.CurrentDateTime();
	AnsiString ID ="1";
	AnsiString Text="aplikace start";
	AnsiString strSQL = "INSERT INTO app_log (app_id,app_start,username,send_log_time,command,relation_id) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\")";
	FDConnection1->ExecSQL(strSQL);


	//////otevrení posledního souboru
	nastaveni.posledni_file=true;/////////////////provizorní než budu načítat z ini z filu nastavení zda otevírat či neotevírat poslední sobor

	UnicodeString user_file=ms.delete_repeat(ms.delete_repeat(Parametry,"\"",2),"\"").Trim();
	if(user_file!="")//pokud zkouší uživatel otevřít přímo soubor kliknutím na něj mimo aplikaci
	OtevritSoubor(user_file);
	else
	{
		//načtení posledního otevřeného souboru
		if(nastaveni.posledni_file)
		{
			TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
			FileName=ini->ReadString("otevrene_soubory","posledni_soubor",FileName);
			if(FileName!="Nový.tispl" && FileName!="")OtevritSoubor(FileName);
			delete ini;
		}
	}

	//////automatický BACKUP
	//volá obnovu dat ze zálohy, pokud poslední ukončení programu neproběhlo standardně
	TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
	AnsiString status=ini->ReadString("Konec","status",status);
	if(status=="KO")//pokud došlo k pádu programu
	{
		//zavře úvodní dialog
		zavrit_uvod();

		FileName=ini->ReadString("otevrene_soubory","posledni_soubor",FileName);//zjistí název posledního souboru

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
			if(ID_YES==MB("Aplikace nebyla řádně ukončena. Chcete ze zálohy obnovit poslední neuložený soubor?",MB_YESNO))
			{
				if(OtevritSoubor(FileName+".bac_"+get_user_name()+"_"+get_computer_name())==1)
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
	ini->WriteString("Konec","status","KO");
	delete ini;
}
//---------------------------------------------------------------------------
//Zalogování na webu
//automaticky přidá parametry (čas, uživatel, licence)
void TForm1::log2web(UnicodeString Text)
{
	log2webOnlyText(ms.DeleteSpace(LICENCE)+"_"+get_computer_name()+"_"+get_user_name()+"_"+TIME.CurrentDate()+"_"+TIME.CurrentTime()+"|"+Text);
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
	scListGroupKnihovObjektu->Height=scGPPanel_statusbar->Top-(2+scListGroupNastavProjektu->Height+0+DetailsButton->Height);
	if(/*MOD==REZERVY ||*/ MOD==CASOVAOSA)Invalidate();
	else REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::editacelinky1Click(TObject *Sender)
{
	ESC();//zruší případnou rozdělanou akci
	MOD=SCHEMA;
	SB("editace linky",1);
	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	Timer_simulace->Enabled=false;
	//editacelinky1->Checked=true;          //zakomentovano - novy design, nepouzivaji se checkboxy
	//testovnkapacity1->Checked=false;
	//simulace1->Checked=false;
	//casoverezervy1->Checked=false;
	//casovosa1->Checked=false;
	//technologickprocesy1->Checked=false;
	scSplitView_LEFTTOOLBAR->Visible=true;
	scListGroupNastavProjektu->Top=0;
	scListGroupNastavProjektu->Visible=true;
	scListGroupKnihovObjektu->Visible=true;
	scListGroupKnihovObjektu->Top=scListGroupNastavProjektu->Height;
	//PopupMenu1->AutoPopup=true;
	DuvodUlozit(true);
	ButtonPLAY->Visible=false;
	Timer_neaktivity->Enabled=false;
	CheckBoxVytizenost->Visible=false;
	CheckBoxAnimovatSG->Visible=false;
	scLabel_doba_cekani->Visible=false;
	GlyphButton_close_grafy->Visible=false;


	CheckBoxVymena_barev->Visible=false;
	Label_zamerovac->Visible=false;
	ComboBoxODmin->Visible=false;
	ComboBoxDOmin->Visible=false;
	rComboBoxKrok->Visible=false;
	LabelRoletka->Visible=false;
	CheckBox_pouzit_zadane_kapacity->Visible=false;
	g.ShowGrafy(false);
	ComboBoxCekani->Visible=false;

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
//	Invalidate();
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
void __fastcall TForm1::casovosa1Click(TObject *Sender)
{
	if(d.v.ZAKAZKY->dalsi==NULL)//pokud nebyla zakazka definovaná
		MB("Pro zobrazení je nutné ve formuláři definice zakázek zadat plán výroby!");
	else
	{
		if(d.v.VOZIKY->dalsi==NULL)d.v.generuj_VOZIKY();//situace kdy nejsou načtené vozíky ale existuje zakázka z cestou (situace např. po načtení nového souboru), tak se vygeneruji dle zadané zakazky/cesty vozíky

		if(MOD!=CASOVAOSA)//aby se nevolalo zbytečně znovu, pokud už v daném modu je, ale může být dvousečné ve významu uživatelské užitečnosti
		{
			MOD=CASOVAOSA;
			ESC();//zruší případně rozdělanou akci
			SB("zobrazení časové osy technologických procesů",1);
			if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
			Timer_simulace->Enabled=false;
			//testovnkapacity1->Checked=false;
			//editacelinky1->Checked=false;
			//casoverezervy1->Checked=false;
			//simulace1->Checked=false;
			//casovosa1->Checked=true;
			//technologickprocesy1->Checked=false;
			d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
			d.PosunT.y=0;
			//zneplatnit_minulesouradnice();//zrušeno test
			DuvodUlozit(true);
			scSplitView_LEFTTOOLBAR->Visible=false;

			scListGroupNastavProjektu->Visible=false;
			scListGroupKnihovObjektu->Visible=false;
			//PopupMenu1->AutoPopup=true;
			Button3->Visible=false;
			Timer_neaktivity->Enabled=true;
			pocitadlo_doby_neaktivity=0;//implicitní hodnota
			technologickprocesy1->Enabled=true;
			Timer_animace->Enabled=false;
			ButtonPLAY->Visible=false;
			CheckBoxVymena_barev->Visible=true;
			CheckBoxVytizenost->Visible=true;
			scLabel_doba_cekani->Visible=true;
			GlyphButton_close_grafy->Visible=true;

			CheckBoxAnimovatSG->Visible=false;
			ComboBoxODmin->Visible=false;
			ComboBoxDOmin->Visible=false;
			rComboBoxKrok->Visible=false;
			LabelRoletka->Visible=false;
			CheckBox_pouzit_zadane_kapacity->Visible=false;
			ComboBoxCekani->Visible=true;
			d.JIZPOCITANO=false;

			Label_zamerovac->Visible=false;
			Invalidate();
		}
	}
}
//---------------------------------------------------------------------------
//zavře nebo otevře grafy v časových osách
void __fastcall TForm1::GlyphButton_close_grafyClick(TObject *Sender)
{
	if(GlyphButton_close_grafy->GlyphOptions->Kind==scgpbgkClose)
	{
		Form1->GlyphButton_close_grafy->Left=Form1->ClientWidth-Form1->GlyphButton_close_grafy->Width;
		Form1->GlyphButton_close_grafy->Top=scGPPanel_statusbar->Top-Form1->GlyphButton_close_grafy->Height;
		g.ShowGrafy(false);
		GlyphButton_close_grafy->GlyphOptions->Kind=scgpbgkDownArrow;//změní typ ikony
	}
	else
	{
		Form1->GlyphButton_close_grafy->Left=Form1->ClientWidth-Form1->GlyphButton_close_grafy->Width;
		Form1->GlyphButton_close_grafy->Top=Form1->Chart1->Top/*-Form1->GlyphButton_close_grafy->Height*/;
		g.ShowGrafy(true);
		GlyphButton_close_grafy->GlyphOptions->Kind=scgpbgkClose;//změní typ ikony
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::technologickprocesy1Click(TObject *Sender)
{
	/*MOD=TECHNOPROCESY;
	ESC();//zruší případně rozdělanou akci
	SB("zobrazení technologických procesů v čase",1);
	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	Timer_simulace->Enabled=false;
	//testovnkapacity1->Checked=false;
	//editacelinky1->Checked=false;
	//casoverezervy1->Checked=false;
	//simulace1->Checked=false;
	//technologickprocesy1->Checked=true;
	d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
	d.PosunT.y=0;
	zneplatnit_minulesouradnice();

	g.ShowGrafy(false);
	DuvodUlozit(true);
	scListGroupNastavProjektu->Visible=false;
	scListGroupKnihovObjektu->Visible=false;
	//PopupMenu1->AutoPopup=false;
	Button3->Visible=false;
	Timer_neaktivity->Enabled=false;

	CheckBoxVytizenost->Visible=false;
	CheckBoxAnimovatSG->Visible=true;

	CheckBoxVymena_barev->Visible=false;
	scLabel_doba_cekani->Visible=false;
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
	ComboBoxODmin->Visible=true;
	rComboBoxKrok->Visible=true;



	double konec_cas=d.v.vrat_nejpozdejsi_konec_zakazek()/10;
	for (int i = 1; i <= 10; i++) {
		int val=konec_cas*i;
		ComboBoxODmin->Items->Add(val);
		ComboBoxDOmin->Items->Add(val);
	}
	//ComboBoxODmin->Items->Add(d.v.vrat_nejpozdejsi_konec_zakazek());
	ComboBoxDOmin->Visible=true;
	//ComboBoxDOmin->Top=CheckBoxPALCE->Top;
	ComboBoxDOmin->Left=ComboBoxODmin->Left+40+2;
	//ComboBoxDOmin->Items->Add(d.TP.KZ);//plnění komba max časem

	CheckBox_pouzit_zadane_kapacity->Visible=true;
 //	CheckBox_pouzit_zadane_kapacity->Top=CheckBoxPALCE->Top;
	CheckBox_pouzit_zadane_kapacity->Left=CheckBoxAnimovatSG->Left+CheckBoxAnimovatSG->Width+7;

	//---
  ComboBoxCekani->Visible=false;
	Label_zamerovac->Visible=false;
	Invalidate(); */
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
	Label_zamerovac->Visible=false;
	ComboBoxODmin->Visible=false;
	ComboBoxDOmin->Visible=false;
	rComboBoxKrok->Visible=false;
	ComboBoxCekani->Visible=false;
	Invalidate();*/
}
//---------------------------------------------------------------------------
//skryje či zobrazí mřížku
void __fastcall TForm1::scGPSwitch5ChangeState(TObject *Sender)
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
void TForm1::S(UnicodeString Text)
{
		ShowMessage(Text);
}
//---------------------------------------------------------------------------
//vola rychle myMessageBox
int TForm1::MB(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit)
{
	return myMessageBox->Show(left,top,text,caption_text,mbTYPE,centrovat_text,checkbox_zobrazit);
}
int TForm1::MB(UnicodeString text,int mbTYPE,bool centrovat_text)
{
	return myMessageBox->Show(text,mbTYPE,centrovat_text);
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
	//vykreslení gridu
	if(grid && Zoom>0.5 && !antialiasing && /*MOD!=REZERVY &&*/ MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)d.vykresli_grid(Canvas,size_grid);//pokud je velké přiblížení tak nevykreslí

	//jednoltivé mody
	Zoom_predchozi_AA=Zoom;//musí být tu, před mody (mohl by být i před kreslením gridu)
	switch(MOD)
	{
		case SCHEMA://vykreslování všech vektorů ve schématu
		{
			if(!antialiasing)d.vykresli_vektory(Canvas);
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
				d.vykresli_vektory(bmp_in->Canvas);
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_grid,bmp_in); //velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
				Canvas->Draw(0,0,bmp_out);
				delete (bmp_out);//velice nutné
				delete (bmp_grid);//velice nutné
				delete (bmp_in);//velice nutné
			}
			//grafické měřítko
			if(scGPSwitch_meritko->State==true)d.meritko(Canvas);
			break;
		}
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
				Canvas->Draw(0,RzToolbar1->Height,bmp_in);
				delete (bmp_in);//velice nutné
			}
			d.vykresli_svislici_na_casove_osy(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
			break;
		}
//		case TECHNOPROCESY:	//d.vykresli_technologicke_procesy(Canvas); break; puvodni konstrukce
//		if(SplitViewOpen==false)
//		{
//			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
//			bmp_in->Width=ClientWidth;bmp_in->Height=ClientHeight;
//			d.vykresli_technologicke_procesy(bmp_in->Canvas);
//			Canvas->Draw(0,RzToolbar1->Height,bmp_in);
//			delete (bmp_in);//velice nutné
//		}
//			break;
//		//	case SIMULACE:d.vykresli_simulaci(Canvas);break; - probíhá už pomocí timeru, na tomto to navíc se chovalo divně
	}
}
//---------------------------------------------------------------------------
//vybere buď Invalidate nebo FormPaint(this) dle if(!antialiasing)
void TForm1::REFRESH(bool invalidate)
{
	if(!antialiasing && invalidate)Invalidate();
	else {FormPaint(this);if(Label_wip->Visible)Label_wip->Invalidate();}//pokude je zapntutý antialiasing neproblikne, ale jen se "přeplácne" bitmapou nedojde k probliknutí
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	funkcni_klavesa=0;
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
		case 33:;break;
		//PAGE DOWN
		case 34:break;
		//END
		case 35:break;
		//HOME
		case 36:if(d.PosunT.x){d.PosunT.x=0;Invalidate();}break;
		//ŠIPKA DOLŮ
		case 40:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y+1); break;}
		//ŠIPKA LEVÁ
		case 37:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x-1,Mouse->CursorPos.y);break;}
		//ŠIPKA PRAVÁ
		case 39:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x+1,Mouse->CursorPos.y);break;}
		//ŠIPKA NAHORU
		case 38:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y-1);break;}
		//ŠIPKA DOLŮ
		case 98:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y+1); break;}
		//ŠIPKA LEVÁ
		case 100:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x-1,Mouse->CursorPos.y);break;}
		//ŠIPKA PRAVÁ
		case 102:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x+1,Mouse->CursorPos.y);break;}
		//ŠIPKA NAHORU
		case 104:{Mouse->CursorPos=TPoint(Mouse->CursorPos.x,Mouse->CursorPos.y-1);break;}
		//F1 - volání nápovědy
		case 112:break;
		//F2
		case 113:break;
		//F3 - pohled celé schéma
		case 114:RzToolButton11Click(Sender);break;
		//F4
		case 115:Vybratoknem1Click(Sender);break;
		//F5
		case 116:Posouvat1Click(Sender);break;
		//F6
		case 117:Posunout2Click(Sender);break;
		//F7
		case 118:ZOOM_IN();break;
		//F8
		case 119:ZOOM_OUT();break;
		//F9
		case 120:break;
		//CTRL, SHIFT
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
{  //prozatim jen pro účely vývoje
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
	 if(Button==mbLeft/* && MOD!=REZERVY*/)//je stisknuto levé tlačítko myši
	 {
			stisknute_leve_tlacitko_mysi=true;
			vychozi_souradnice_kurzoru=TPoint(X,Y);//výchozí souřadnice

			//aktivuje POSUN OBJEKTU,pokud je kliknuto v místě objektu (v jeho vnitřku)
			if(Akce==NIC && posun_objektu==false && funkcni_klavesa==0)//pokud není aktivovaná jiná akce
			{
				if(MOD==SCHEMA)
				{
					pom=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
					if(pom!=NULL){Akce=MOVE;kurzor(posun_l);posun_objektu=true;minule_souradnice_kurzoru=TPoint(X,Y);}
					else {Akce=PAN;pan_non_locked=true;}//test - přímo dovolení PAN pokud se neposová objekt = Rosťova prosba
				}
				else {Akce=PAN;pan_non_locked=true;}//test - přímo dovolení PAN pokud se neposová objekt   = Rosťova prosba
			}

			if(funkcni_klavesa==1 || Akce==ZOOM_W_MENU)Akce=ZOOM_W;
			///Akce=PAN; pouze pokud bych nechtěl pomocí mezerníku aktivovat PAN
			switch(Akce)
			{
				case PAN:
				{
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
				default: break;
			}
			DuvodUlozit(true);
	 }
	 //POPUP menu
	 else//pokud není stisknuto levé tlačítko, předpokládá se volání pop-up menu
	 {
			 onPopUP(X,Y);//nastavení zobrazení popUPmenu a jeho volání včetně pozice
	 }
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	vyska_menu=Mouse->CursorPos.y-Y;//uchová rozdíl myšího kurzoru a Y-pixelu v pracovní oblasti

	akt_souradnice_kurzoru_PX=TPoint(X,Y);
	akt_souradnice_kurzoru=m.P2L(akt_souradnice_kurzoru_PX);

	if(MOD==CASOVAOSA)//vykreslování posuvné (dle myši) svislice kolmé na osy procesů, slouží jakou ukázovatko času na ose
	{
			d.vykresli_svislici_na_casove_osy(Canvas,minule_souradnice_kurzoru.X,minule_souradnice_kurzoru.Y);
			minule_souradnice_kurzoru=TPoint(X,Y);
			d.vykresli_svislici_na_casove_osy(Canvas,X,Y);
			SB(UnicodeString((X+d.PosunT.x)/d.PX2MIN)+" min",6);//výpis času na ose procesů dle kurzoru
			//hazí stejné souřadnice if(abs((int)minule_souradnice_kurzoru.x-(int)akt_souradnice_kurzoru_PX.x)>1 && abs((int)minule_souradnice_kurzoru.y-(int)akt_souradnice_kurzoru_PX.y)>1)//pokud je změna větší než jeden pixel, pouze ošetření proti divnému chování myši (možná mi docházela baterka, s myší jsem nehýbal, ale přesto docházele k rušení labelu resp. volání metody FormMouseMove)
			Label_zamerovac->Visible=false;
			pocitadlo_doby_neaktivity=0;Timer_neaktivity->Enabled=true;//monitoruje dobu nečinosti od znovu
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
		case ADD://přídávání objektu, posun navigačních linii
		{        //algoritmy v tomto CASE (včetně dílčích algoritmu) by bylo možné sloučit, ale bylo by to dost práce navíc...
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
		case NIC:
		{
			if(MOD!=CASOVAOSA)zneplatnit_minulesouradnice();

			//algoritmus na ověřování zda se kurzor nachází na objektem (a může být tedy povoleno v pop-up menu zobrazení volby nastavit parametry) přesunut do metody mousedownclick, zde se to zbytečně volalo při každém posunu myši
			//povoluje smazání či nastavení parametrů objektů, po přejetí myší přes daný objekt

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
				case ADD:add_objekt(X,Y);zneplatnit_minulesouradnice();break;//přidání objekt
				case MOVE:move_objekt(X,Y);break;//posun objektu
				default: break;
			}
	 }
	 stisknute_leve_tlacitko_mysi=false;
	 //vrat_puvodni_akci();
	 /*if(X<=RzSizePanel_knihovna_objektu->Width) DrawGrid_knihovna->Enabled=true;
	 else DrawGrid_knihovna->Enabled=false;*/
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//nastavení zobrazení popUPmenu a jeho volání včetně pozice
void TForm1::onPopUP(int X, int Y)
{
	close_all_items_popUPmenu(false);//výchozí skrytí položek - pozor záleží na pořadí zobrazování

	//dle modu
	switch(MOD)
	{
		//case EDITACE:break;
		//case TESTOVANI:break;
		//case REZERVY:break;
		case CASOVAOSA:
		{                              //min                  //vozik
			proces_pom=d.v.najdi_proces((X+d.PosunT.x)/d.PX2MIN,ceil((Y+d.PosunT.y-d.KrokY/2-RzToolbar1->Height)/(d.KrokY*1.0)));//vrací nalezen proces, proces_pom se využívá ještě dále
			if(proces_pom!=NULL && !d.mod_vytizenost_objektu)
			{
				PopUPmenu->Item_zobrazit_parametry->Visible=true;//nastavení zobrazení
			}
			PopUPmenu->Item_rychly_export->Visible=true;
			PopUPmenu->Left=akt_souradnice_kurzoru_PX.x;PopUPmenu->Top=akt_souradnice_kurzoru_PX.y;//umístění popup menu
			PopUPmenu->Show();//volání vlastního popup menu + ošetření, pokud je mimo obrazovku
		}break;
		case TECHNOPROCESY:break;
		case SIMULACE:break;
		default://pro SCHEMA
		{
			//následující prapodivný kod je pokus o workaround zobrazování pořadí položek
			PopUPmenu->Item_nastavit_parametry->Visible=true;
			PopUPmenu->Item_smazat->Visible=true;
			close_all_items_popUPmenu(true);
			//povoluje smazání či nastavení parametrů objektů, po přejetí myší přes daný objekt //přídáno 19.4.2017 - zeefktivnění
			pom=d.v.najdi_objekt(m.P2Lx(X),m.P2Ly(Y),d.O_width,d.O_height);
			if(pom!=NULL)// nelze volat přímo metodu najdi objekt, protože pom se používá dále
			{
				PopUPmenu->scLabel_nastavit_parametry->Caption="  Nastavit "+pom->name.UpperCase();
				PopUPmenu->scLabel_smazat->Caption="  Smazat "+pom->name.UpperCase();
			}
			else//pokud nebyl objekt zobrazen skryje
			{
				close_all_items_popUPmenu(false);
			}
			PopUPmenu->Item_priblizit->Visible=true;
			PopUPmenu->Item_oddalit->Visible=true;
			PopUPmenu->Item_posouvat->Visible=true;
			PopUPmenu->Item_posunout->Visible=true;
			PopUPmenu->Item_vybrat_oknem->Visible=true;
			PopUPmenu->Item_cely_pohled->Visible=true;
			//umístění popup menu
			PopUPmenu->Left=akt_souradnice_kurzoru_PX.x;
			PopUPmenu->Top=akt_souradnice_kurzoru_PX.y;
			//volání vlastního popup menu + ošetření, pokud je mimo obrazovku
			PopUPmenu->Show();
		}break;
	}
}
//---------------------------------------------------------------------------
//zajistí skrýtí všech položek popUPmenu
void TForm1::close_all_items_popUPmenu(bool vyjimka)
{
	//výchozí skrytí položek - pozor záleží na pořadí zobrazování a možná i skrývání
	PopUPmenu->Item_zobrazit_parametry->Visible=false;
	PopUPmenu->Item_rychly_export->Visible=false;
	PopUPmenu->Item_cely_pohled->Visible=false;
	PopUPmenu->Item_vybrat_oknem->Visible=false;
	PopUPmenu->Item_posunout->Visible=false;
	PopUPmenu->Item_posouvat->Visible=false;
	PopUPmenu->Item_oddalit->Visible=false;
	PopUPmenu->Item_priblizit->Visible=false;
	if(!vyjimka)PopUPmenu->Item_smazat->Visible=false;
	if(!vyjimka)PopUPmenu->Item_nastavit_parametry->Visible=false;
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
		if(Zoom<5) //max
		{
			Uloz_predchozi_pohled();
			Zoom+=0.5;
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
			Zoom-=0.5;
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
		 Posun.x=-scListGroupNastavProjektu->Width;if(vyska_menu>0)Posun.y=-vyska_menu+9;else Posun.y=-29;
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
	zneplatnit_minulesouradnice();
	//vymaže, překreslí, odznačí provizorní lini
	if(Akce==MOVE)d.odznac_oznac_objekt(Canvas,pom,akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x,akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y);
	if(Akce==ADD)
	{
		if(add_posledni)d.odznac_oznac_objekt_novy_posledni(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
		else d.odznac_oznac_objekt_novy(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,pom);
	}
	pom=NULL;
	proces_pom=NULL;
	kurzor(standard);
	Akce=NIC;
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
					case 1:souradnice.x=m.round(m.P2Lx(X)/(size_grid*1.0*m2px))*size_grid*m2px;souradnice.y=m.round(m.P2Ly(Y)/(size_grid*1.0*m2px))*size_grid*m2px;break;//přichytí automaticky
					case 2: souradnice=m.P2L(TPoint(X,Y));break;//automaticky nepřichyt
			}
			akutalizace_stavu_prichytavani_vSB();
		}
		else souradnice=m.P2L(TPoint(X,Y));

		//uložení do paměti
		if(add_posledni)//vloží poslední prvek
		{
			d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y);
			pom=NULL;
		}
		else//vkládá prvek mezi prvky
		{
			d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y,pom);
			d.v.zvys_indexy(pom);//zvýší indexy nasledujicích bodů
			pom=NULL;
		}
		//ihned vykreslení
		//pokud zruším nutnost invalidate kvůli spojovacím liniim, možno odkomentovat
		//d.vykresli_rectangle(Canvas,souradnice,knihovna_objektu[vybrany_objekt].name,knihovna_objektu[vybrany_objekt].short_name);
		vybrany_objekt=-1;//odznačí objekt logicky, musí se nový vybrat znovu
		Akce=NIC;kurzor(standard);
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
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt \""+AnsiString(pom->name.UpperCase())+"\" umístit v pořadí\nmezi objekty \""+AnsiString(ukaz->name.UpperCase())+"\" a \""+AnsiString(d.v.OBJEKTY->dalsi->name.UpperCase())+"\"?","",MB_YESNO,true,false))
				{
					d.v.zmen_poradi_objektu(pom,d.v.OBJEKTY->predchozi);//volání realizace samotné záměny
				}
			}
			else//ostatní situace
			{

				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt \""+AnsiString(pom->name.UpperCase())+"\" umístit v pořadí\nmezi objekty \""+AnsiString(ukaz->name.UpperCase())+"\" a \""+AnsiString(ukaz->dalsi->name.UpperCase())+"\"?","",MB_YESNO,true,false))
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
void TForm1::zneplatnit_minulesouradnice()
{
	minule_souradnice_kurzoru=TPoint(-200,-200);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
			TGridDrawState State)
{
	TCanvas* C=DrawGrid_knihovna->Canvas;
	int W=DrawGrid_knihovna->DefaultColWidth;
	int H=DrawGrid_knihovna->DefaultRowHeight;
	int P=-1*DrawGrid_knihovna->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

	unsigned short obdelnik_okrajX=10;unsigned short obdelnik_okrajY=5;unsigned short okraj_packy=obdelnik_okrajY;
	C->Font->Style = TFontStyles()<< fsBold;
	if(antialiasing)C->Font->Size=11;else C->Font->Size=10;
	C->Font->Name="Arial";
	C->Pen->Width=1;
	C->Pen->Color=(TColor)RGB(160,160,160);//(TColor)RGB(19,115,169);
	C->Brush->Color=(TColor)RGB(160,160,160);//(TColor)RGB(19,115,169);
	C->Font->Color=clWhite;
	for(unsigned short n=1;n<=pocet_objektu_knihovny;n++)
	{
		UnicodeString text=knihovna_objektu[n-1].short_name;
		//odelník
		C->Rectangle(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+obdelnik_okrajY+P,((n+1)%2+1)*W-obdelnik_okrajX,ceil(n/2.0)*H-obdelnik_okrajY+P);
		//packy
		C->MoveTo(((n+1)%2)*W+okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);
		C->MoveTo(((n+1)%2)*W+W-obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+W-okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);
		//písmo
		C->TextOutW((Rect.Right-Rect.Left-C->TextWidth(text))/2+((n+1)%2)*W,(Rect.Bottom-Rect.Top-C->TextHeight(text))/2+(ceil(n/2.0)-1)*H+P,text);
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	int Col,Row;
	//DrawGrid_knihovna->MouseToCell(X,Y,Col,Row);
	Col=DrawGrid_knihovna->Col; Row=DrawGrid_knihovna->Row;

	SB("Kliknutím na libovolné místo umístíte objekt "+knihovna_objektu[Col+Row+Row].name);
	//SB(AnsiString(DrawGrid_knihovna->TopRow)+" "+AnsiString(Col)+" "+AnsiString(Row)+" "+knihovna_objektu[Col+Row+Row].name);

	if(Col>-1 && Row>-1)
	{
		vybrany_objekt=Col+Row+Row;
		Akce=ADD;kurzor(add_o);//Screen->Cursor=crCross;
		add_posledni=true;pom=NULL;
		//ShowMessage(vybrany_objekt);
	}

	//*pozn n-tý sloupec + (n-tý řádek - 1)* celkový počet slouců
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
void __fastcall TForm1::DrawGrid_knihovnaKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	FormKeyUp(Sender,Key,Shift);
}
//---------------------------------------------------------------------------
//přeposílá událost na form
void __fastcall TForm1::DrawGrid_knihovnaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
 FormKeyDown(Sender,Key,Shift);
}
//---------------------------------------------------------------------------
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
	Canvas->Font->Color=clRed;
	SetBkMode(Canvas->Handle,TRANSPARENT);//nastvení netransparentního pozadí
	Canvas->Font->Size=9;
	Canvas->Font->Name="Arial";
	Canvas->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	Canvas->TextOutW(ClientWidth-Canvas->TextWidth(text)-10,Form1->scGPPanel_statusbar->Top-20,text);
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
			case 0:editacelinky1Click(Sender);break;
			case 1:testovnkapacity1Click(Sender);break;
			case 2:casoverezervy1Click(Sender);break;
			case 3:simulace1Click(Sender);break;
			case 4:editacelinky1Click(Sender);break;
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
		int result=MB(FileName_short(FileName)+" byl změněn.\nChcete ho před ukončením uložit?",MB_YESNOCANCEL);
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
	 //	log2web("konec");
		//pro ochranu v případě pádu programu
		//TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
		TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
		ini->WriteString("Konec","status","OK");
		delete ini;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Smazat1Click(TObject *Sender)
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
}
//---------------------------------------------------------------------------
//zobrazí paramety jednoho procesu na časových osách
void __fastcall TForm1::Zobrazitparametry1Click(TObject *Sender)
{
	double prozatim_delka_voziku=d.v.PP.delka_voziku;
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

	S(/*"n_procesu: "+AnsiString(proces_pom->n)+*/
	"číslo vozíku v zakázce: "+AnsiString(proces_pom->n_v_zakazce)+
	"\nnázev: "+AnsiString(proces_pom->segment_cesty->objekt->name)+
	"\nzkratka: "+AnsiString(proces_pom->segment_cesty->objekt->short_name)+
	"\nrežim: "+AnsiString(rezim)+
	"\nrychlost dopravníku: "+AnsiString(proces_pom->segment_cesty->RD)+" m/min"+
	"\nrozteč palců: "+AnsiString(proces_pom->segment_cesty->objekt->pohon->roztec)+" mm"+
	"\nstřední hodnota doby čekání na palec: "+AnsiString(m.cekani_na_palec(0,proces_pom->segment_cesty->objekt->pohon->roztec,proces_pom->segment_cesty->RD,1))+" min"+
	"\nmax. hodnota doby čekání na palec: "+AnsiString(proces_pom->segment_cesty->objekt->pohon->roztec/1000.0/proces_pom->segment_cesty->RD)+" min"+
	"\nTpoc: "+AnsiString(proces_pom->Tpoc)+" | Tkon: "+AnsiString(proces_pom->Tkon)+" | Tdor: "+AnsiString(proces_pom->Tdor)+" | Tpre: "+AnsiString(proces_pom->Tpre)+" | Tcek: "+AnsiString(proces_pom->Tcek)+
	"\nPT: "+AnsiString(proces_pom->Tkon-proces_pom->Tpoc)+" min"+
	"\nMT: "+AnsiString(proces_pom->Tpre-proces_pom->Tkon)+" min"+
	"\nWT: "+AnsiString(proces_pom->Tcek-proces_pom->Tpre)+" min"+
	"\nCT: "+AnsiString(proces_pom->Tcek-proces_pom->Tpoc)+" min"+
	"\nzadaný CT: "+AnsiString(proces_pom->segment_cesty->CT)+" min"+
	"\ndoporučená kapacita: "+AnsiString(proces_pom->segment_cesty->objekt->kapacita_dop)+", délka objektu: "+delka_dop+" m"+
	"\npožadovaná kapacita: "+AnsiString(proces_pom->segment_cesty->objekt->kapacita)+	", délka objektu: "+delka+" m");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Nastvitparametry1Click(TObject *Sender)
{
	//Cvektory::TObjekt *p=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
	Cvektory::TPohon *ukaz=NULL;
	if(Form1->d.v.POHONY!=NULL)ukaz=Form1->d.v.POHONY->dalsi;//ukazatel na první objekt v seznamu POHONY, přeskočí hlavičku

	if(ukaz!=NULL)
	{
		if(pom!=NULL)
		{
				//ošetření aby zůstal dialog na monitoru
				if(akt_souradnice_kurzoru_PX.x+10+Form_parametry->Width<Form1->Width)
					Form_parametry->Left=akt_souradnice_kurzoru_PX.x+10;
				else
					Form_parametry->Left=Form1->Width-Form_parametry->Width-10;
				if(akt_souradnice_kurzoru_PX.y+10+Form_parametry->Height<Form1->Height)
					Form_parametry->Top=akt_souradnice_kurzoru_PX.y+10;
				else 	Form_parametry->Top=Form1->Height-Form_parametry->Height-scGPPanel_statusbar->Height-10;
					Form_parametry->Caption=pom->name+" - parametry";

				//plnění daty
				Form_parametry->scComboBox_pohon->Items->Clear();
				while (ukaz!=NULL)
				{
					Form_parametry->scComboBox_pohon->Items->Add(ukaz->name);
					ukaz=ukaz->dalsi;
				}

				//předání hodnoty objektů ze souboru resp. strukutry do Form_Parametry
				Form_parametry->scEdit_name->Text=pom->name;
				Form_parametry->scEdit_shortname->Text=pom->short_name;
				Form_parametry->rEditNum_delka_dopravniku->Text=pom->delka_dopravniku;
				Form_parametry->scComboBox_pohon->ItemIndex=pom->pohon->n-1;
				Form_parametry->scComboBox_rezim->ItemIndex=pom->rezim;
				Form_parametry->rEditNum_kapacita->Text=pom->kapacita;
				Form_parametry->rEditNum_odchylka->Text=pom->odchylka;
				Form_parametry->scCheckBox_stopky->Checked=pom->stopka;

				//navrácení dat + volání zobrazení formu
				Form_parametry->ShowModal();
				if(Form_parametry->returnOk)//kvůli tomu, že button nevrací mrOK
				{
						try
						{
							//navrácení hodnot z Form_Parametry, v případě stisku OK
							pom->name=Form_parametry->scEdit_name->Text;
							pom->short_name=Form_parametry->scEdit_shortname->Text;
							pom->delka_dopravniku=ms.MyToDouble(Form_parametry->rEditNum_delka_dopravniku->Text);
							pom->pohon=d.v.vrat_pohon(Form_parametry->scComboBox_pohon->ItemIndex+1);//indexuje se od nuly
							pom->rezim=Form_parametry->scComboBox_rezim->ItemIndex;
							pom->cekat_na_palce=Form_parametry->scComboBox_cekani_pacel->ItemIndex;
							pom->kapacita=Form_parametry->rEditNum_kapacita->Text.ToDouble();
							pom->odchylka=Form_parametry->rEditNum_odchylka->Text.ToDouble();
							pom->stopka=Form_parametry->scCheckBox_stopky->Checked;
							DuvodUlozit(true);
							REFRESH();
						}
						catch(...)
						{
							S("Neplatná hodnota!");
							Nastvitparametry1Click(this);//nové zadání
						}
				}
		}
	}
	else
	{
			MB("Nejdříve je nutné nadefinovat pohony ve formuláři parametry linky!");
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
	else if(duvod_k_ulozeni)Ulozit_soubor();else SB("Soubor byl již uložen...");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UlozitjakoClick(TObject *Sender)
{
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
void __fastcall TForm1::OtevritClick(TObject *Sender)
{
  scSplitView_MENU->Opened=false;
	if(duvod_k_ulozeni)//pokud existuje předcházejicí soubor, který je nutný uložit
  {
		int result=MB(FileName_short(FileName)+" byl změněn.\nChcete ho před ukončením uložit?",MB_YESNOCANCEL);
		switch(result)
		{
			case mrYes:     UlozitClick(this); if(!stisknuto_storno){OtevritSoubor();}break;
			case mrNo:      DuvodUlozit(false); OtevritSoubor(); break;
			case mrCancel:  break;
		}
	}
  else
	OtevritSoubor();
}
//---------------------------------------------------------------------------
void TForm1::OtevritSoubor()//realizuje otevření opendialogu s následným voláním realizace samotného otevření souboru
{
  OpenDialog1->Title="Otevřít soubor";
	OpenDialog1->DefaultExt="*.tispl";
	OpenDialog1->Filter="Soubory formátu TISPL (*.tispl)|*.tispl";
	if(OpenDialog1->Execute())
	{
		//zavolá nový soubor/smaže stávajicí
		NovySouborClick(this);

		//otevrení souboru
		OtevritSoubor(OpenDialog1->FileName);
	}
}
//-------------------------------------------------------------------------
unsigned short int TForm1::OtevritSoubor(UnicodeString soubor)//realizuje samotné otevření souboru
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
			d.v.PP.efektivita=d.v.File_hlavicka.efektivita;
			d.v.PP.delka_voziku=d.v.File_hlavicka.delka_voziku;
			d.v.PP.typ_voziku=d.v.File_hlavicka.typ_voziku;
			MOD=d.v.File_hlavicka.Mod;
			switch(MOD)
			{
					case NO:REFRESH();break; //překreslí obraz pro ostatní případy
					//case SCHEMA: 	editacelinky1Click(this);break;
					//ZDM case TESTOVANI:	testovnkapacity1Click(this);break;
					//ZDM case REZERVY:		casoverezervy1Click(this);break;
					//ZDM case CASOVAOSA:	editacelinky1Click(this);MOD=SCHEMA;/*casovosa1Click(this);*/break;
					//ZDM case TECHNOPROCESY:editacelinky1Click(this);MOD=SCHEMA;break;
					//ZDM case SIMULACE:	editacelinky1Click(this);MOD=SCHEMA;/*simulace1Click(this);*/break;
					default: editacelinky1Click(this);break;
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
	scSplitView_MENU->Opened=false;
	OpenDialog1->Title="Otevřít soubor ze zálohy";
	OpenDialog1->DefaultExt="*tispl.bac_"+get_user_name()+"_"+get_computer_name();
	AnsiString nazev="Soubory zálohy TISPL (*.tispl.bac_"+get_user_name()+"_"+get_computer_name()+")|*.tispl.bac_"+get_user_name()+"_"+get_computer_name();
	OpenDialog1->Filter=nazev;
	if(OpenDialog1->Execute())
	{
		//zavolá nový soubor/smaže stávajicí
		NovySouborClick(this);

		//otevrení souboru
		OtevritSoubor(OpenDialog1->FileName);

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
		NovySouborClick(this);

		//otevrení souboru
		OtevritSoubor(OpenDialog1->FileName);

		//ještě donutí stávajicí soubor uložit pod novým jménem
		FileName="";
		UlozitjakoClick(this);
	}
}
//---------------------------------------------------------------------------
//uložení posledního otevřeného souboru
void TForm1::ulozit_posledni_otevreny()
{
	TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
	ini->WriteString("otevrene_soubory","posledni_soubor",FileName);
	delete ini;
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
		pom=NULL;
		proces_pom=NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
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
	 Ulozit->Enabled=stav;
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
				if(!antialiasing)d.vykresli_vektory(Bitmap->Canvas);//vykreslování všech vektorů
				else
				{
					Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
					bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
					Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
					d.vykresli_vektory(bmp_in->Canvas);
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
		 UnicodeString FileName="export_"+AnsiString(i)+"_"+AnsiString(i+PO)+".png";//název souboru bez data, výše s datem, ale zbytečně se to množí...
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
	if(d.v.OBJEKTY->dalsi==NULL)//pokud existují nějaka data
		S("Žádná data k exportu!");
	else
	{
		//příprava SaveDialogu
		SaveDialog->Title="Vytvořit report...";
		SaveDialog->DefaultExt="*.csv";
		SaveDialog->Filter="Soubory formátu CSV (*.csv)|*.csv|Soubory formátu XLS (*.xls)|*.xls";

		//předvyplnění názvem stejnojmeným souboru
		UnicodeString FN=FileName;
		if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
		SaveDialog->FileName=FN;

		if(SaveDialog->Execute())
		{
			Screen->Cursor=crHourGlass;//změní kurzor na přesýpací hodiny
			//nastavení formátu
			UnicodeString export_format="csv";
			if(SaveDialog->FileName.SubString(SavePictureDialog1->FileName.Length()-2,3).LowerCase() =="xls")export_format="xls";
			//samotné uložení
			if(d.v.ulozit_report(SaveDialog->FileName)==1)SB("Report do "+export_format+" dokončen.");else SB("Chyba při tvorbě reportu!");
			//postprocesní záležitost
			Screen->Cursor=crDefault;//změní kurzor na default
			ShellExecute(0,L"open",UnicodeString(SaveDialog->FileName).c_str(),0,0,SW_SHOWNORMAL);;//otevře výstup
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::html1Click(TObject *Sender)
{
  scSplitView_MENU->Opened=false;
	if(d.v.OBJEKTY->dalsi==NULL)//pokud existují nějaka data
		S("Žádná data k exportu!");
	else
	{
		//příprava SaveDialogu
		SaveDialog->Title="Vytvořit HTML report...";
		SaveDialog->DefaultExt="*.html";
		SaveDialog->Filter="Soubory formátu HTML (*.html)|*.html";

		//předvyplnění názvem stejnojmeným souboru
		UnicodeString FN=FileName;
		if(FN.Pos(".")==FN.Length()-5)FN=FN.SubString(1,FN.Length()-6);
		SaveDialog->FileName=FN;

		if(SaveDialog->Execute())
		{
			Screen->Cursor=crHourGlass;//změní kurzor na přesýpací hodiny
			//samotné uložení
			if(d.v.ulozit_report(SaveDialog->FileName)==1)SB("Report do HTML dokončen.");else SB("Chyba při tvorbě reportu!");
			//postprocesní záležitost
			Screen->Cursor=crDefault;//změní kurzor na default
			ShellExecute(0,L"open",UnicodeString(SaveDialog->FileName).c_str(),0,0,SW_SHOWNORMAL);;//otevře výstup
		}
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
	S(get_temp_dir());
	S(Application->ExeName);
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

void __fastcall TForm1::Button8Click(TObject *Sender)
{
//ZDM
//	ShowMessage("Délka dopravníku je: "+AnsiString(d.v.delka_dopravniku(d.v.OBJEKTY))+" metrů");
//	Memo1->Lines->Add("Délka dopravníku je: "+AnsiString(d.v.delka_dopravniku(d.v.OBJEKTY))+" metrů");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button9Click(TObject *Sender)
{
	/*Memo1->Lines->Add("vypis spojáku VOZIKY:");
			Cvektory::TVozik *ukaz=d.v.VOZIKY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
		while (ukaz!=NULL)
		{
			//akce s ukazatelem
			//Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" ID: ")+ukaz->id+AnsiString(" X: ")+ukaz->X+AnsiString(" Y: ")+ukaz->Y);
			Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" Start: ")+ukaz->start/d.PX2MIN+AnsiString(" Pozice: ")+ukaz->pozice/d.PX2MIN);
			//posun na další prvek v seznamu
			ukaz=ukaz->dalsi;
		} */
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

//		Memo1->Lines->Add("vypis spojáku PROCESY:");
//		Cvektory::TProces *ukaz=d.v.PROCESY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			//akce s ukazatelem
//			//if(ukaz->vozik->n==2)
//			Memo1->Lines->Add(AnsiString("n: ")+ukaz->n+AnsiString(" | n_v_zakazce: ")+ukaz->n_v_zakazce+AnsiString(" | Tpoc: ")+ukaz->Tpoc+AnsiString(" | Tkon: ")+ukaz->Tkon+AnsiString(" | Tdor: ")+ukaz->Tdor+AnsiString(" | Tpre: ")+ukaz->Tpre+AnsiString(" | Tcek: ")+ukaz->Tcek+AnsiString(" | Shortname: ")+ukaz->cesta->objekt->short_name);
//			//posun na další prvek v seznamu
//			ukaz=ukaz->dalsi;
//		}
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
{ //ZDM
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
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonPLAYClick(TObject *Sender)
{
	Timer_animace->Enabled=!Timer_animace->Enabled;
	if(Timer_animace->Enabled)
	{
		CheckBoxAnimovatSG->Visible=true;
		d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
		d.PosunT.y=0;
		ButtonPLAY->Caption="STOP";
		Timer_animace->Interval=40;
		d.TP.K=0.05;//Krok po kolika minutach se bude zobrazovat
		d.TP.DO=-d.TP.K;//výchozí čás (záporný interval, kvůli tomu, aby se začínalo od nuly)
		d.TP.Nod=0;//rozmezí Jaký se vypíše vozik,
		d.TP.Ndo=0;//rozmezí Jaký se vypíše vozik, pokud bude 0 vypisují se všechny
		d.TP.A=true;//jednořádková animace
	}
	else ButtonPLAY->Caption="PLAY";
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
		ButtonPLAY->Caption="PLAY";
		technologickprocesy1Click(Sender);//vratí statický mod
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxVytizenost_Click(TObject *Sender)
{
	if(d.v.PROCESY!=NULL && d.v.PROCESY->predchozi->n>0)//pokud je více objektů
	{
		d.mod_vytizenost_objektu=!d.mod_vytizenost_objektu;
		CheckBoxVymena_barev->Visible=!CheckBoxVymena_barev->Visible;
		SB("");
		Invalidate();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{

	ShowMessage(scListGroupNastavProjektu->TabOrder);
	ShowMessage(scListGroupKnihovObjektu->TabOrder);
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
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CheckBoxVymena_barev_Click(TObject *Sender)
{
	 d.JIZPOCITANO=true;
	 Invalidate();
}

 //-------------------------------------------------------------
void __fastcall TForm1::ComboBoxODminChange(TObject *Sender)
{
		//ještě ošetření aby zadal hodnotu od menší nebo rovno hodnotě do

		d.TP.OD=ms.MyToDouble(ComboBoxODmin->Text);
		Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxDOminChange(TObject *Sender)
{

		//ještě ošetření aby zadal hodnotu od menší nebo rovno hodnotě do
		d.TP.DO=ms.MyToDouble(ComboBoxDOmin->Text);
		Invalidate();
}
//---------------------------------------------------------------------------
//zapne či vypne antialiasing
void __fastcall TForm1::scGPSwitch4ChangeState(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
	//protože zlobí, tak rozepisuji
	if(scGPSwitch4->State==true)
	 antialiasing=true;
	else
	 antialiasing=false;
	//antialiasing=!antialiasing;
	DrawGrid_knihovna->Invalidate();
	Invalidate();
	//REFRESH();
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


void __fastcall TForm1::CheckBox_pouzit_zadane_kapacityClick(TObject *Sender)
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
void __fastcall TForm1::Button12Click(TObject *Sender)
{
 //	WindowState = wsMaximized;
 scSplitView_LEFTTOOLBAR->Opened = !scSplitView_LEFTTOOLBAR->Opened;

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
void __fastcall TForm1::scGPGlyphButton1Click(TObject *Sender)
{
  ESC();//zruší případnou rozdělanou akci
	scSplitView_OPTIONS->Opened = !scSplitView_OPTIONS->Opened;

		if(scSplitView_MENU->Opened) scSplitView_MENU->Close();
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
//volání parametrů linky
void __fastcall TForm1::Button_dopravnik_parametryClick(TObject *Sender)
{
	ESC();//zruší případnou rozdělanou akci
	Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
	Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
	if(IDOK==Form_parametry_linky->ShowModal())DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//volání superformuláře (definice zakázek)
void __fastcall TForm1::scGPGlyphButton_definice_zakazekClick(TObject *Sender)
{
	ESC();//zruší případnou rozdělanou akci
	if(d.v.OBJEKTY->dalsi==NULL)
	{
		MB("Nejdříve je nutné zakreslit schéma linky!");
	}
	else
	{
		Form_definice_zakazek->Left=Form1->ClientWidth/2-Form_definice_zakazek->Width/2;
		Form_definice_zakazek->Top=Form1->ClientHeight/2-Form_definice_zakazek->Height/2;
		Form_definice_zakazek->ShowModal();
		DuvodUlozit(true);//požaduje se vždy, protože i storno při prvním zobrazení ukládá default zakázku s default cestou
	}
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
	scLabel_ZOOM->Caption=AnsiString(Zoom*100)+" %";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPSwitch9ChangeState(TObject *Sender)
{
	scSplitView_MENU->Opened=false;
	if(scLabel19->Caption=="M-design")scLabel19->Caption="R-design";
	else scLabel19->Caption="M-design";
	scListGroupNastavProjektu->HeaderAutoColor=false;
	scListGroupKnihovObjektu->HeaderAutoColor=scListGroupNastavProjektu->HeaderAutoColor;
	scListGroupNastavProjektu->HeaderFont->Color=clWhite;
	scListGroupNastavProjektu->Color=(TColor)RGB(43,87,154);
	scListGroupKnihovObjektu->Color=scListGroupNastavProjektu->Color;
	scListGroupKnihovObjektu->HeaderFont->Color=scListGroupNastavProjektu->HeaderFont->Color;
	scListGroupNastavProjektu->HeaderHeight=35;
	scListGroupKnihovObjektu->HeaderHeight=scListGroupNastavProjektu->HeaderHeight;
	scGPGlyphButton_parametry_linky->Top=41;
	scGPGlyphButton_definice_zakazek->Top=80;
	scGPGlyphButton_parametry_linky->Height=30;
	scGPGlyphButton_definice_zakazek->Height=scGPGlyphButton_parametry_linky->Height;
	scGPGlyphButton_parametry_linky->Options->NormalColor=clBtnShadow;;
	scGPGlyphButton_definice_zakazek->Options->NormalColor=scGPGlyphButton_parametry_linky->Options->NormalColor;
	DrawGrid_knihovna->Top=37;
	scGPGlyphButton_parametry_linky->Options->ShapeStyle=scgpRoundedRect;
	scGPGlyphButton_definice_zakazek->Options->ShapeStyle=scGPGlyphButton_parametry_linky->Options->ShapeStyle;
	scListGroupNastavProjektu->RowLineMargin=scListGroupNastavProjektu->Width;
	scListGroupKnihovObjektu->Top=115;
	scListGroupNastavProjektu->Height=115;

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

	 if(MOD==CASOVAOSA){


			CheckBoxVytizenost->Visible=true;
			CheckBoxVymena_barev->Visible=true;
			ComboBoxCekani->Visible=true;
	}

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
	 NovySouborClick(Sender);//prvně zavřu starý, který ho donutím uložit a vytvořím nový soubor
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
		 OtevritSoubor(ExtractFilePath(Application->ExeName)+FileNameSablona); //volaný *.tispl musí být tam, kde exe tisplu nebo zadat adresu
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

void __fastcall TForm1::Button11Click(TObject *Sender)
{
    Memo2->Visible=true;
		Cvektory::TZakazka *zakazka=d.v.ZAKAZKY->dalsi;
		//načítání dat
		if(zakazka!=NULL)
		if(zakazka->cesta!=NULL)//pokud již byla cesta definovaná
		{
			ShowMessage(zakazka->name);
			Cvektory::TCesta *ukaz=zakazka->cesta->dalsi;//přeskočí hlavičku, jde rovnou na první segment cesty
			while(ukaz!=NULL)
			{
				Memo2->Lines->Add
				(
						AnsiString(ukaz->n)+","+
						AnsiString(ukaz->objekt->short_name)+","+
						AnsiString(ukaz->CT)+","+
						AnsiString(ukaz->RD)+","+
						AnsiString(ukaz->Tc)+","
						//+(AnsiString((int)ukaz->Stav))
				);
				ukaz=ukaz->dalsi;
			}
		}
		else S("Není zakazka");
}
//---------------------------------------------------------------------------









