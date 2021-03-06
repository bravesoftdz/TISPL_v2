﻿//---------------------------------------------------------------------------
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
//#include "parametry.h"
//#include "TT_kalkulator.h"
#include "parametry_linky.h"
#include "superform.h"
#include "uvod.h"
#include "antialiasing.h"
#include "popUP_menu.h"
//#include "eDesigner.h"
//#include "casovaOsa_info.h"
#include "report.h"
//#include "PO_math.h"
//#include "kabina_schema.h"
#include "Unit2.h"
//#include "gapoR.h"
#include "adjustace.h"
#include "kalibrace.h"
#include "Z_rozliseni.h"
#include "TmGrid.h"
#include "katalog_dopravniku.h"
#include "miniform_zpravy.h"
#include "miniform_mereni.h"
#include "help.h"
#include "MyString.h"
#include "konzole.h"
#include "Tvlakno_sound.h"
#include <idattachmentfile.hpp>//přílohy mailů
//#include <mmsystem.h>//multimediální timer
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "RzPanel"
//#pragma link "RzSplit"
//#pragma link "RzStatus"
//#pragma link "RzButton"
//#pragma link "RzBorder"
//#pragma link "RzGrids"
//#pragma link "RzLabel"
//#pragma link "rStringGridEd"
//#pragma link "rStringGridEd"
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
#pragma link "scGPExtControls"
//#pragma link "rHintWindow"
//#pragma link "rHTMLLabel"
#pragma link "scImageCollection"
#pragma link "scHint"
#pragma link "scHint"
//#pragma link "rHintWindow"
//#pragma link "rImprovedComps"
//#pragma link "RzAnimtr"
#pragma resource "*.dfm"
TForm1 *Form1, *F;//pouze zkrácený zapis
AnsiString Parametry;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	srand(time(NULL));//nutno tady tj. úplně první!!!

  //start GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	d.GDIplus=true;

	Akce=BLOK;Akce_temp=NIC;MOD=NO;Screen->Cursor=crDefault;//změní kurzor na default + zapniti blokace akce, zabrání spouštění onclick událostí

	F=Form1;//pouze zkrácení zápisu

	//db_connection();//připojení k DB

	//Form1->Width=1290;//workaround bílé mezery v záložkové liště - nefunguje

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

	//bitmapa pro uložení přesovaného obrazu - PAN, a pro statickou scénu
	Pan_bmp=new Graphics::TBitmap();Pan_bmp_ALL=NULL;
	pan_non_locked=false;
	Staticka_scena=NULL;
	vlakno_PanCreate=NULL;

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
	HC=LoadCursor(HInstance,L"EDIT_TEXT");
	Screen->Cursors[14]=HC;
	HC=LoadCursor(HInstance,L"ZMENA_D_X");
	Screen->Cursors[15]=HC;
	HC=LoadCursor(HInstance,L"ZMENA_D_Y");
	Screen->Cursors[16]=HC;
	HC=LoadCursor(HInstance,L"POSUN_IND_LD");
	Screen->Cursors[17]=HC;
	HC=LoadCursor(HInstance,L"POSUN_IND_PD");
	Screen->Cursors[18]=HC;
	HC=LoadCursor(HInstance,L"EDITACE_POSUN");
	Screen->Cursors[19]=HC;
	HC=LoadCursor(HInstance,L"INFO");
	Screen->Cursors[20]=HC;
	HC=LoadCursor(HInstance,L"CLOSE");
	Screen->Cursors[21]=HC;
	HC=LoadCursor(HInstance,L"POSUN_EDITACE_OBJ");
	Screen->Cursors[22]=HC;
	HC=LoadCursor(HInstance,L"EDITACE_OBJ");
	Screen->Cursors[23]=HC;

	//Načtení z INI
	AnsiString T=readINI("nastaveni_nahled", "cas");
	if(T==0 || T=="")PTunit=SEC;else PTunit=MIN;
	if(T=="")writeINI("nastaveni_nahled","cas",PTunit);
	T=F->readINI("nastaveni_nahled","LO");
	if(T==0)LOunit=M;else LOunit=MM;
	if(T=="")writeINI("nastaveni_nahled","LO",LOunit);
	T=F->readINI("nastaveni_nahled","Delka_otoce");
	if(T==0)DOtocunit=M;else DOtocunit=MM;
	if(T=="")writeINI("nastaveni_nahled","Delka_otoce",DOtocunit);
	T=readINI("nastaveni_nahled","koty_delka");//složiteji řešené, z důvodu kót časových a zároveň délkových
	DKunit=MM;
	if(T=="")writeINI("nastaveni_nahled","koty_delka",DKunit);
	//pro pohon
	T=readINI("nastaveni_form_parametry","RDt");//aktuální rychlost
	if(T==0)aRDunit=SEC;else aRDunit=MIN;
	if(T=="")writeINI("nastaveni_form_parametry","RDt",aRDunit);
	T=readINI("nastaveni_nahled","R"); //rozteč
	if(T==0)Runit=M;else Runit=MM;
	if(T=="")writeINI("nastaveni_form_parametry","R",Runit);
	T=readINI("nastaveni_nahled","Rz"); //rozestup
	if(T==0)Rzunit=M;else Rzunit=MM;
	if(T=="")writeINI("nastaveni_form_parametry","Rz",Rzunit);

	T=readINI("nastaveni_editace","intenzita_vektory"); //intenzita vektoru needitovanych objektu
	if(T==0 || T=="")d.pasivni_vektory_intenzita=50;else d.pasivni_vektory_intenzita=F->ms.MyToDouble(T);
	scGPTrackBar_intenzita->Value = d.pasivni_vektory_intenzita;
	if(T=="")writeINI("nastaveni_editace","intenzita_vektory",50);//pokud neexistuje zápis, vytvoří se, zápis nutný pro další fungivání (vypínání geometrie, navracení vykreslovacích vrstev)
	T=readINI("nastaveni_editace","rotace_jigu"); //zobrazit rotaci jigu
	if(T==1 || T=="")rotace_jigu=1;else rotace_jigu=0;
	if(T=="")writeINI("nastaveni_editace","rotace_jigu",rotace_jigu);
	T=readINI("nastaveni_editace","zobrazeni_pozic"); //zobrazit pozice
	if(T==1 || T=="")zobrazit_pozice=1;else zobrazit_pozice=0;
	if(T=="")writeINI("nastaveni_editace","zobrazeni_pozic",zobrazit_pozice);
	T=readINI("nastaveni_editace","zobrazit_popisky"); //zobrazit popisky
	if(T==1 || T=="")zobrazit_popisky=1;else zobrazit_popisky=0;
	if(T=="")writeINI("nastaveni_editace","zobrazit_popisky",zobrazit_popisky);
	T=readINI("nastaveni_editace","zobrazit_koleje"); //zobrazit koleje
	if(T==1 || T=="")zobrazit_koleje=1;else zobrazit_koleje=0;
	if(T=="")writeINI("nastaveni_editace","zobrazit_koleje",zobrazit_koleje);
	T=readINI("nastaveni_editace","zobrazit_palce"); //zobrazit palce
	if(T==1 || T=="")zobrazit_palce=1;else zobrazit_palce=0;
	if(T=="")writeINI("nastaveni_editace","zobrazit_palce",zobrazit_palce);
	T=readINI("nastaveni_editace","zobrazit_rozmisteni_voziku"); //zobrazit_rozmisteni_jigu
	if(T==1 || T=="")zobrazit_rozmisteni_voziku=1;else zobrazit_rozmisteni_voziku=0;
	if(T=="")writeINI("nastaveni_editace","zobrazit_rozmisteni_voziku",zobrazit_rozmisteni_voziku);
	T=readINI("nastaveni_editace","zobrazit_popisek_pohonu"); //zobrazit_popisek_pohonu
	if(T==1 || T=="")zobrazit_popisek_pohonu=1;else zobrazit_popisek_pohonu=0;
	if(T=="")writeINI("nastaveni_editace","zobrazit_popisek_pohonu",zobrazit_popisek_pohonu);
  //temp
	T=readINI("nastaveni_editace","pripocitavat_casy_prvniho_el");
	if(T==0 || T=="")scGPCheckBox_meridlo_casy->Checked=false;else scGPCheckBox_meridlo_casy->Checked=true;
	//nastavení app
	T=readINI("Nastaveni_app","antialiasing"); //zobrazit rotaci jigu
	if(T==0)antialiasing=false;else antialiasing=true;


	if(rotace_jigu==1) scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
	else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;

	if(zobrazit_pozice==1)scGPCheckBox_zobrazit_pozice->Checked=true;
	else   scGPCheckBox_zobrazit_pozice->Checked=false;

	if(zobrazit_popisky==1) scGPCheckBox1_popisky->Checked=true;
	else scGPCheckBox1_popisky->Checked=false;

	if(zobrazit_koleje==1) scGPCheckBox_zobrazit_koleje->Checked=true;
	else scGPCheckBox_zobrazit_koleje->Checked=false;

	if(zobrazit_palce==1)scGPCheckBox_zobrazit_palce->Checked=true;
	else   scGPCheckBox_zobrazit_palce->Checked=false;

	if(zobrazit_rozmisteni_voziku==1)scGPCheckBox_rozmisteni_voziku->Checked=true;
	else   scGPCheckBox_rozmisteni_voziku->Checked=false;

	if(zobrazit_popisek_pohonu==1)scGPCheckBox_popisek_pohonu->Checked=true;
	else   scGPCheckBox_popisek_pohonu->Checked=false;

	if(antialiasing)scGPCheckBox_antialiasing->Checked=true;
	else   scGPCheckBox_antialiasing->Checked=false;

  //načtení posledních uložených souborů
	N1projekt1->Caption=readINI("historie","posledni_soubor_1");
	N2projekt1->Caption=readINI("historie","posledni_soubor_2");
	N3projekt1->Caption=readINI("historie","posledni_soubor_3");
	//zobrazení či skrytí položek
	if(N1projekt1->Caption=="")N1projekt1->Visible=false;
	else N1projekt1->Visible=true;
	if(N2projekt1->Caption=="")N2projekt1->Visible=false;
	else N2projekt1->Visible=true;
	if(N3projekt1->Caption=="")N3projekt1->Visible=false;
	else N3projekt1->Visible=true;
	//kontrola zda soubory pořád existují
	if(!FileExists(N1projekt1->Caption))N1projekt1->Enabled=false;
	if(!FileExists(N2projekt1->Caption))N2projekt1->Enabled=false;
	if(!FileExists(N3projekt1->Caption))N3projekt1->Enabled=false;
	//enabled btn
	if(N1projekt1->Caption=="" && N2projekt1->Caption=="" && N3projekt1->Caption=="")scButton_posledni_otevreny->Enabled=false;

	//povolení Automatická záloha, timery
	Timer_backup->Enabled=true;
	pocitadlo_doby_neaktivity=0;

	//nastavení zobrazení
	d.SCENA=1111111;
	vychozi_stav_sceny=d.SCENA;

	//nastavení implicitního souboru
	duvod_k_ulozeni=false;
	Novy_soubor();
	volat_parametry_linky=false;

	//zjistí verzi aplikace
	DWORD ver=GetFileVersion(Application->ExeName);
	FileVersion=UnicodeString(HIWORD(ver))+"."+UnicodeString(LOWORD(ver));
	unsigned  major,minor,build;
	GetProductVersion(Application->ExeName,major,minor,build);
	ProductVersion=UnicodeString(major)+"."+UnicodeString(minor);
	//VERZE=UnicodeString(UnicodeString(HIWORD(ver) >> 16)+"."+UnicodeString(HIWORD(ver) & 0xFFFF)+"."+UnicodeString(LOWORD(ver) >> 16)+"."+UnicodeString(LOWORD(ver) & 0xFFFF)); nefunguje

	//licence, logování atp.
	Caption="ELTEP - tispl";
	scLabel_titulek->Caption=Caption+" - [Nový.tispl]";
	Application->Title="TISPL";
	LICENCE="TRIAL_ELTEP";
	EDICE=ARCHITECT;//ARCHITECT,CLIENT,VIEWER,DEMO
	edice();//zakázání či povolení grafických uživatelských prvků dle úrovně edice
	n_prihlaseni=0;
	TZF=!DEBUG;//TRIAL_zakazat_funkcionality - nyní nastaveno pro RELEASE
	LogFileStream=new TFileStream("LOGFILE.txt",fmOpenWrite|fmCreate|fmShareDenyNone);
	logovat=true;

  //pomocné objekty pro kopírování paremtrů v PO
	copyObjekt=new Cvektory::TObjekt;
	copyObjektRzRx.x=0;copyObjektRzRx.y=0;

	//načtení aktuálního Fontu - aFont dále v kódu neměnit!!!
	aFont=new TFont();//aktuální nastavený výchozí font
	aFont->Name="Arial";aFont->Size=12;aFont->Color=clBlack;
	if(FileExists(get_Windows_dir()+"\\Fonts\\Roboto-Condensed.ttf")){aFont->Name="Roboto Cn";aFont->Size=14;}//pokud je k dispozici Roboto Cn, tak ho nastaví
	if(FileExists(get_Windows_dir()+"\\Fonts\\Roboto Cn.ttf")){aFont->Name="Roboto Cn";aFont->Size=14;}//pokud je k dispozici Roboto Cn, tak ho nastaví

	//nastavení knihovnky
	//nastavení počtu řádku knihovny podle počtu objektů
	if(pocet_objektu_knihovny%2!=0)DrawGrid_knihovna->RowCount=(pocet_objektu_knihovny+1)/2,0;//pokud je počet objektu lichý
	else DrawGrid_knihovna->RowCount=m.round2double(pocet_objektu_knihovny/2,0);//sudý počet objektů
	if(pocet_objektu_knihovny>0)vybrany_objekt=0;else//-použival jsem v době kdy jsem chtěl mít implicitní prvek pokud existuje nějaký prvek v knihovně, tak nastaví vybraný prvek jako první
	vybrany_objekt=-1;
	VyID=13;//objekt-symbol vyhýbky - ID typu
	JID=-1;
	knihovna_id=0;
	element_id=99;
	duvod_ulozit_nahled=false;

	refresh_mGrid=true;//nevykresluje se z buffru ale přímo
	posun_dalsich_elementu=false;
	zobrazit_meritko=true;

	//nastavení designu v konstruktoru
	DesignSettings();

	//příprava na jazyk mutace
	if(readINI("Nastaveni_app","jazyk")=="1"){language=CS;/* scGPSwitch1->State=scswOn;*/} else { language=EN; /*scGPSwitch1->State=scswOff; */}

	//vytvoření katalogu dopravníků
	d.v.vytvor_KATALOG();

	//ostatní
	Image_knihovna_objektu->Height=ClientHeight-34;
	scSplitView_LEFTTOOLBAR->Visible=true;//zapnuti levého toolbaru
	ID_tabulky=0;//uchovává ID tabulky, použiváné při mousemove
	count_memo=0;//jednoduchý counter zobrazovaný v memo3
	predchozi_PM=NULL;
	scGPTrackBar1->MaxValue=17;
	if(DEBUG)scGPTrackBar1->MaxValue=20;
	refreshovat_scGPTrackBar=true;
	vlakno_akce=0;//defaultní stav
  vlakno_obraz=NULL;
	storno=true;//defaultně počítat se stištěným stornem
	velikost_citelne_oblasti_elementu=0.12;//v metrech, 0.114285714285714 šířka pouzdra pohonu
	zobrazit_upozorneni_teplomery=true;
	typElementu=0;
	CAS=0;
  pouzivane_pohony=NULL;

	//vývojářské featury
	if(DEBUG && get_user_name()+get_computer_name()=="MartinMARTIN-NOTEBOOK"){ButtonMaVl->Visible=false;}//pokud se dělá překlad u MaKr, je skryto MV tlačítko testovací tlačítko, MaKr testovací se volá přes F9
	if(!DEBUG && ButtonRosta->Visible)ButtonRosta->Visible=false;
	if(!DEBUG && ButtonMaVl->Visible)ButtonMaVl->Visible=false;
	if(!DEBUG && ButtonMaKr->Visible)ButtonMaKr->Visible=false;

	//temp zde, po sloučení DFM nastavovat v něm
	scListGroupPanel_geometrie->Height=147;
  DrawGrid_geometrie->Height=127;
}
//---------------------------------------------------------------------------
//nastaví komponentám aFont
void TForm1::set_font(int velikost)
{
	log(__func__);//logování
	TColor barva;

	barva=ButtonPLAY_O->Font->Color;
  ButtonPLAY_O->Font=aFont;
  ButtonPLAY_O->Font->Color=barva;
  ButtonPLAY_O->Font->Size=velikost;
  barva=Konec->Font->Color;
  Konec->Font=aFont;
  Konec->Font->Color=barva;
  Konec->Font->Size=velikost;
  barva=Schema->Font->Color;
  Schema->Font=aFont;
  Schema->Font->Color=barva;
  Schema->Font->Size=velikost;
  barva=PopupMenuButton->Font->Color;
  PopupMenuButton->Font=aFont;
  PopupMenuButton->Font->Color=barva;
  PopupMenuButton->Font->Size=velikost;
  barva=Toolbar_Ulozit->Font->Color;
  Toolbar_Ulozit->Font=aFont;
	Toolbar_Ulozit->Font->Color=barva;
  Toolbar_Ulozit->Font->Size=velikost;
  barva=Toolbar_NovySoubor->Font->Color;
  Toolbar_NovySoubor->Font=aFont;
  Toolbar_NovySoubor->Font->Color=barva;
  Toolbar_NovySoubor->Font->Size=velikost;
  barva=Toolbar_Otevrit->Font->Color;
  Toolbar_Otevrit->Font=aFont;
  Toolbar_Otevrit->Font->Color=barva;
  Toolbar_Otevrit->Font->Size=velikost;
  barva=DetailsButton->Font->Color;
  DetailsButton->Font=aFont;
  DetailsButton->Font->Color=barva;
  DetailsButton->Font->Size=velikost;
  barva=Layout->Font->Color;
  Layout->Font=aFont;
  Layout->Font->Color=barva;
  Layout->Font->Size=velikost;
  barva=scGPGlyphButton_napoveda->Font->Color;
  scGPGlyphButton_napoveda->Font=aFont;
  scGPGlyphButton_napoveda->Font->Color=barva;
  scGPGlyphButton_napoveda->Font->Size=velikost;
  barva=scLabel_titulek->Font->Color;
  scLabel_titulek->Font=aFont;
  scLabel_titulek->Font->Color=barva;
  scLabel_titulek->Font->Size=velikost;
	barva=Nahled->Font->Color;
  Nahled->Font=aFont;
  Nahled->Font->Color=barva;
  Nahled->Font->Size=velikost;
  barva=scGPGlyphButton_PLAY->Font->Color;
  scGPGlyphButton_PLAY->Font=aFont;
  scGPGlyphButton_PLAY->Font->Color=barva;
  scGPGlyphButton_PLAY->Font->Size=velikost;
//	barva=RzStatusPane3->Font->Color;
//	RzStatusPane3->Font=aFont;
//	RzStatusPane3->Font->Color=barva;
//	RzStatusPane3->Font->Size=velikost;
//	barva=RzStatusPane4->Font->Color;
//	RzStatusPane4->Font=aFont;
//	RzStatusPane4->Font->Color=barva;
//	RzStatusPane4->Font->Size=velikost;
//	barva=RzStatusPane5->Font->Color;
//	RzStatusPane5->Font=aFont;
//	RzStatusPane5->Font->Color=barva;
//  RzStatusPane5->Font->Size=velikost;
  barva=scButton_zamek_layoutu->Font->Color;
  scButton_zamek_layoutu->Font=aFont;
  scButton_zamek_layoutu->Font->Color=barva;
  scButton_zamek_layoutu->Font->Size=velikost;
  barva=scGPButton_zmerit_vzdalenost->Font->Color;
  scGPButton_zmerit_vzdalenost->Font=aFont;
  scGPButton_zmerit_vzdalenost->Font->Color=barva;
  scGPButton_zmerit_vzdalenost->Font->Size=velikost;
  barva=scListGroupKnihovObjektu->Font->Color;
  scListGroupKnihovObjektu->Font=aFont;
  scListGroupKnihovObjektu->Font->Color=barva;
  scListGroupKnihovObjektu->Font->Size=velikost;
  barva=scListGroupPanel_hlavickaOstatni->Font->Color;
  scListGroupPanel_hlavickaOstatni->Font=aFont;
  scListGroupPanel_hlavickaOstatni->Font->Color=barva;
  scListGroupPanel_hlavickaOstatni->Font->Size=velikost;
  barva=scListGroupPanel_hlavickaOtoce->Font->Color;
  scListGroupPanel_hlavickaOtoce->Font=aFont;
  scListGroupPanel_hlavickaOtoce->Font->Color=barva;
  scListGroupPanel_hlavickaOtoce->Font->Size=velikost;
  barva=scListGroupPanel_geometrie->Font->Color;
  scListGroupPanel_geometrie->Font=aFont;
  scListGroupPanel_geometrie->Font->Color=barva;
  scListGroupPanel_geometrie->Font->Size=velikost;
  barva=scListGroupPanel_poznamky->Font->Color;
  scListGroupPanel_poznamky->Font=aFont;
  scListGroupPanel_poznamky->Font->Color=barva;
  scListGroupPanel_poznamky->Font->Size=velikost;
  barva=scGPLabel_roboti->Font->Color;
  scGPLabel_roboti->Font=aFont;
  scGPLabel_roboti->Font->Color=barva;
  scGPLabel_roboti->Font->Size=velikost;
  barva=scGPLabel_otoce->Font->Color;
  scGPLabel_otoce->Font=aFont;
  scGPLabel_otoce->Font->Color=barva;
  scGPLabel_otoce->Font->Size=velikost;
  barva=scGPLabel_stop->Font->Color;
  scGPLabel_stop->Font=aFont;
  scGPLabel_stop->Font->Color=barva;
  scGPLabel_stop->Font->Size=velikost;
  barva=scGPLabel_geometrie->Font->Color;
  scGPLabel_geometrie->Font=aFont;
  scGPLabel_geometrie->Font->Color=barva;
  scGPLabel_geometrie->Font->Size=velikost;
  barva=scGPLabel_poznamky->Font->Color;
  scGPLabel_poznamky->Font=aFont;
  scGPLabel_poznamky->Font->Color=barva;
  scGPLabel_poznamky->Font->Size=velikost;
	barva=scExPanel_html->Font->Color;
  scExPanel_html->Font=aFont;
  scExPanel_html->Font->Color=barva;
	scExPanel_html->Font->Size=velikost;
	barva=scGPButton_ulozit->Font->Color;
	scGPButton_ulozit->Font=aFont;
  scGPButton_ulozit->Font->Color=barva;
  scGPButton_ulozit->Font->Size=velikost;
  barva=scGPButton_zahodit->Font->Color;
  scGPButton_zahodit->Font=aFont;
  scGPButton_zahodit->Font->Color=barva;
  scGPButton_zahodit->Font->Size=velikost;
  barva=scGPLabel_prepinacKot->Font->Color;
  scGPLabel_prepinacKot->Font=aFont;
  scGPLabel_prepinacKot->Font->Color=barva;
  scGPLabel_prepinacKot->Font->Size=velikost;
  barva=scGPImage_mereni_vzdalenost->Font->Color;
  scGPImage_mereni_vzdalenost->Font=aFont;
  scGPImage_mereni_vzdalenost->Font->Color=barva;
  scGPImage_mereni_vzdalenost->Font->Size=velikost;
  barva=scGPImage_zamek_posunu->Font->Color;
  scGPImage_zamek_posunu->Font=aFont;
  scGPImage_zamek_posunu->Font->Color=barva;
  scGPImage_zamek_posunu->Font->Size=velikost;
  barva=scButton_konec->Font->Color;
  scButton_konec->Font=aFont;
  scButton_konec->Font->Color=barva;
  scButton_konec->Font->Size=velikost;
  barva=scButton_otevrit->Font->Color;
  scButton_otevrit->Font=aFont;
  scButton_otevrit->Font->Color=barva;
  scButton_otevrit->Font->Size=velikost;
  barva=scButton4->Font->Color;
  scButton4->Font=aFont;
  scButton4->Font->Color=barva;
  scButton4->Font->Size=velikost;
  barva=scButton_zaloha->Font->Color;
  scButton_zaloha->Font=aFont;
  scButton_zaloha->Font->Color=barva;
  scButton_zaloha->Font->Size=velikost;
  barva=scButton_report->Font->Color;
  scButton_report->Font=aFont;
  scButton_report->Font->Color=barva;
  scButton_report->Font->Size=velikost;
  barva=scLabel11->Font->Color;
  scLabel11->Font=aFont;
  scLabel11->Font->Color=barva;
  scLabel11->Font->Size=velikost;
  barva=scButton_ulozjako->Font->Color;
  scButton_ulozjako->Font=aFont;
  scButton_ulozjako->Font->Color=barva;
  scButton_ulozjako->Font->Size=velikost;
  barva=scButton_export->Font->Color;
  scButton_export->Font=aFont;
  scButton_export->Font->Color=barva;
  scButton_export->Font->Size=velikost;
  barva=scButton_posledni_otevreny->Font->Color;
  scButton_posledni_otevreny->Font=aFont;
  scButton_posledni_otevreny->Font->Color=barva;
  scButton_posledni_otevreny->Font->Size=velikost;
  barva=scButton_novy->Font->Color;
  scButton_novy->Font=aFont;
  scButton_novy->Font->Color=barva;
  scButton_novy->Font->Size=velikost;
  barva=scButton_ulozit->Font->Color;
  scButton_ulozit->Font=aFont;
  scButton_ulozit->Font->Color=barva;
  scButton_ulozit->Font->Size=velikost;
	barva=scButton_nacist_podklad->Font->Color;
  scButton_nacist_podklad->Font=aFont;
  scButton_nacist_podklad->Font->Color=barva;
  scButton_nacist_podklad->Font->Size=velikost;
  barva=pravyoption_nadpis->Font->Color;
  pravyoption_nadpis->Font=aFont;
  pravyoption_nadpis->Font->Color=barva;
  pravyoption_nadpis->Font->Size=velikost;
  barva=scExPanel_vrstvy->Font->Color;
  scExPanel_vrstvy->Font=aFont;
  scExPanel_vrstvy->Font->Color=barva;
  scExPanel_vrstvy->Font->Size=velikost;
  barva=scGPCheckBox_zobrazit_koleje->Font->Color;
  scGPCheckBox_zobrazit_koleje->Font=aFont;
  scGPCheckBox_zobrazit_koleje->Font->Color=barva;
  scGPCheckBox_zobrazit_koleje->Font->Size=velikost;
  barva=scGPCheckBox_zobrazit_pozice->Font->Color;
  scGPCheckBox_zobrazit_pozice->Font=aFont;
  scGPCheckBox_zobrazit_pozice->Font->Color=barva;
  scGPCheckBox_zobrazit_pozice->Font->Size=velikost;
  barva=scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Font->Color;
  scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Font=aFont;
  scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Font->Color=barva;
  scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Font->Size=velikost;
  barva=scGPCheckBox1_popisky->Font->Color;
  scGPCheckBox1_popisky->Font=aFont;
  scGPCheckBox1_popisky->Font->Color=barva;
  scGPCheckBox1_popisky->Font->Size=velikost;
  barva=scExPanel_podklad->Font->Color;
  scExPanel_podklad->Font=aFont;
  scExPanel_podklad->Font->Color=barva;
  scExPanel_podklad->Font->Size=velikost;
  barva=scGPButton_adjustace->Font->Color;
  scGPButton_adjustace->Font=aFont;
  scGPButton_adjustace->Font->Color=barva;
  scGPButton_adjustace->Font->Size=velikost;
  barva=scGPButton_kalibrace->Font->Color;
  scGPButton_kalibrace->Font=aFont;
  scGPButton_kalibrace->Font->Color=barva;
  scGPButton_kalibrace->Font->Size=velikost;
  barva=scGPCheckBox_stupne_sedi->Font->Color;
  scGPCheckBox_stupne_sedi->Font=aFont;
  scGPCheckBox_stupne_sedi->Font->Color=barva;
  scGPCheckBox_stupne_sedi->Font->Size=velikost;
  barva=scGPCheckBox_zobraz_podklad->Font->Color;
  scGPCheckBox_zobraz_podklad->Font=aFont;
  scGPCheckBox_zobraz_podklad->Font->Color=barva;
  scGPCheckBox_zobraz_podklad->Font->Size=velikost;
  barva=scLabel1_svetelnost->Font->Color;
  scLabel1_svetelnost->Font=aFont;
  scLabel1_svetelnost->Font->Color=barva;
  scLabel1_svetelnost->Font->Size=velikost;
  barva=scExPanel_ostatni->Font->Color;
  scExPanel_ostatni->Font=aFont;
  scExPanel_ostatni->Font->Color=barva;
  scExPanel_ostatni->Font->Size=velikost;
  barva=scLabel16->Font->Color;
  scLabel16->Font=aFont;
  scLabel16->Font->Color=barva;
  scLabel16->Font->Size=velikost;
  barva=scLabel5->Font->Color;
  scLabel5->Font=aFont;
  scLabel5->Font->Color=barva;
  scLabel5->Font->Size=velikost;
  barva=scLabel1_intenzita->Font->Color;
  scLabel1_intenzita->Font=aFont;
  scLabel1_intenzita->Font->Color=barva;
  scLabel1_intenzita->Font->Size=velikost;
  barva=scGPTrackBar_intenzita->Font->Color;
  scGPTrackBar_intenzita->Font=aFont;
  scGPTrackBar_intenzita->Font->Color=barva;
  scGPTrackBar_intenzita->Font->Size=velikost;
  barva=scExPanel1->Font->Color;
  scExPanel1->Font=aFont;
  scExPanel1->Font->Color=barva;
  scExPanel1->Font->Size=velikost;
  barva=scGPButton_mereni_vzdalenost->Font->Color;
  scGPButton_mereni_vzdalenost->Font=aFont;
  scGPButton_mereni_vzdalenost->Font->Color=barva;
  scGPButton_mereni_vzdalenost->Font->Size=velikost;
  barva=Analyza->Font->Color;
  Analyza->Font=aFont;
  Analyza->Font->Color=barva;
  Analyza->Font->Size=velikost;
  barva=Form_parametry_linky->scHTMLLabel_podlahovy->Font->Color;
  Form_parametry_linky->scHTMLLabel_podlahovy->Font=aFont;
  Form_parametry_linky->scHTMLLabel_podlahovy->Font->Color=barva;
  Form_parametry_linky->scHTMLLabel_podlahovy->Font->Size=velikost;
  barva=Form_parametry_linky->scHTMLLabel_podvesny->Font->Color;
  Form_parametry_linky->scHTMLLabel_podvesny->Font=aFont;
  Form_parametry_linky->scHTMLLabel_podvesny->Font->Color=barva;
  Form_parametry_linky->scHTMLLabel_podvesny->Font->Size=velikost;
  barva=Form_parametry_linky->Button_save->Font->Color;
  Form_parametry_linky->Button_save->Font=aFont;
  Form_parametry_linky->Button_save->Font->Color=barva;
  Form_parametry_linky->Button_save->Font->Size=velikost;
  barva=Form_parametry_linky->Button_storno->Font->Color;
  Form_parametry_linky->Button_storno->Font=aFont;
  Form_parametry_linky->Button_storno->Font->Color=barva;
  Form_parametry_linky->Button_storno->Font->Size=velikost;
  barva=Form_parametry_linky->Konec->Font->Color;
  Form_parametry_linky->Konec->Font=aFont;
  Form_parametry_linky->Konec->Font->Color=barva;
  Form_parametry_linky->Konec->Font->Size=velikost;
  barva=Form_parametry_linky->scLabel1->Font->Color;
  Form_parametry_linky->scLabel1->Font=aFont;
  Form_parametry_linky->scLabel1->Font->Color=barva;
  Form_parametry_linky->scLabel1->Font->Size=velikost;
  barva=Form_parametry_linky->scGPGlyphButton_info->Font->Color;
  Form_parametry_linky->scGPGlyphButton_info->Font=aFont;
  Form_parametry_linky->scGPGlyphButton_info->Font->Color=barva;
  Form_parametry_linky->scGPGlyphButton_info->Font->Size=velikost;
	barva=Form_parametry_linky->scGPButton_vozik->Font->Color;
  Form_parametry_linky->scGPButton_vozik->Font=aFont;
  Form_parametry_linky->scGPButton_vozik->Font->Color=barva;
  Form_parametry_linky->scGPButton_vozik->Font->Size=velikost;
  barva=Form_parametry_linky->scGPButton_pohon->Font->Color;
  Form_parametry_linky->scGPButton_pohon->Font=aFont;
  Form_parametry_linky->scGPButton_pohon->Font->Color=barva;
  Form_parametry_linky->scGPButton_pohon->Font->Size=velikost;
  barva=Form_parametry_linky->scLabel_zobrazit_parametry->Font->Color;
  Form_parametry_linky->scLabel_zobrazit_parametry->Font=aFont;
  Form_parametry_linky->scLabel_zobrazit_parametry->Font->Color=barva;
  Form_parametry_linky->scLabel_zobrazit_parametry->Font->Size=velikost;
  barva=Form_parametry_linky->scLabel_smazat->Font->Color;
  Form_parametry_linky->scLabel_smazat->Font=aFont;
  Form_parametry_linky->scLabel_smazat->Font->Color=barva;
  Form_parametry_linky->scLabel_smazat->Font->Size=velikost;
  barva=Form_parametry_linky->scLabel_smazat_nepouzite->Font->Color;
  Form_parametry_linky->scLabel_smazat_nepouzite->Font=aFont;
  Form_parametry_linky->scLabel_smazat_nepouzite->Font->Color=barva;
  Form_parametry_linky->scLabel_smazat_nepouzite->Font->Size=velikost;
  barva=Form_parametry_linky->scLabel_kopirovat->Font->Color;
  Form_parametry_linky->scLabel_kopirovat->Font=aFont;
  Form_parametry_linky->scLabel_kopirovat->Font->Color=barva;
  Form_parametry_linky->scLabel_kopirovat->Font->Size=velikost;
  barva=Form_parametry_linky->scLabel_nastavit_parametry->Font->Color;
  Form_parametry_linky->scLabel_nastavit_parametry->Font=aFont;
  Form_parametry_linky->scLabel_nastavit_parametry->Font->Color=barva;
  Form_parametry_linky->scLabel_nastavit_parametry->Font->Size=velikost;
  barva=Form_parametry_linky->scGPGlyphButton_ADD->Font->Color;
  Form_parametry_linky->scGPGlyphButton_ADD->Font=aFont;
  Form_parametry_linky->scGPGlyphButton_ADD->Font->Color=barva;
  Form_parametry_linky->scGPGlyphButton_ADD->Font->Size=velikost;
  barva=Form_parametry_linky->scGPButton_obecne->Font->Color;
  Form_parametry_linky->scGPButton_obecne->Font=aFont;
  Form_parametry_linky->scGPButton_obecne->Font->Color=barva;
  Form_parametry_linky->scGPButton_obecne->Font->Size=velikost;
  barva=Form_parametry_linky->scHTMLLabel_jig_info->Font->Color;
  Form_parametry_linky->scHTMLLabel_jig_info->Font=aFont;
  Form_parametry_linky->scHTMLLabel_jig_info->Font->Color=barva;
  Form_parametry_linky->scHTMLLabel_jig_info->Font->Size=velikost;
  barva=Form_parametry_linky->scGPGlyphButton_ADD_old->Font->Color;
  Form_parametry_linky->scGPGlyphButton_ADD_old->Font=aFont;
  Form_parametry_linky->scGPGlyphButton_ADD_old->Font->Color=barva;
  Form_parametry_linky->scGPGlyphButton_ADD_old->Font->Size=velikost;
  barva=Form_parametry_linky->scGPGlyphButton_OPTIONS->Font->Color;
  Form_parametry_linky->scGPGlyphButton_OPTIONS->Font=aFont;
  Form_parametry_linky->scGPGlyphButton_OPTIONS->Font->Color=barva;
  Form_parametry_linky->scGPGlyphButton_OPTIONS->Font->Size=velikost;
  barva=Form_parametry_linky->scGPGlyphButton_katalog->Font->Color;
  Form_parametry_linky->scGPGlyphButton_katalog->Font=aFont;
  Form_parametry_linky->scGPGlyphButton_katalog->Font->Color=barva;
  Form_parametry_linky->scGPGlyphButton_katalog->Font->Size=velikost;
  barva=Form_parametry_linky->scGPTrackBar_uchyceni->Font->Color;
  Form_parametry_linky->scGPTrackBar_uchyceni->Font=aFont;
  Form_parametry_linky->scGPTrackBar_uchyceni->Font->Color=barva;
  Form_parametry_linky->scGPTrackBar_uchyceni->Font->Size=velikost;
	barva=Form_parametry_linky->Button_save->Font->Color;
	Form_parametry_linky->Button_save->Font=aFont;
	Form_parametry_linky->Button_save->Font->Color=barva;
	Form_parametry_linky->Button_save->Font->Size=velikost;
	////
	barva=scGPButton_viditelnostmGrid->Font->Color;
	scGPButton_viditelnostmGrid->Font=aFont;
	scGPButton_viditelnostmGrid->Font->Color=barva;
	scGPButton_viditelnostmGrid->Font->Size=velikost;
	barva=scGPButton_viditelnostKoty->Font->Color;
	scGPButton_viditelnostKoty->Font=aFont;
	scGPButton_viditelnostKoty->Font->Color=barva;
	scGPButton_viditelnostKoty->Font->Size=velikost;
	barva=scButton_zamek->Font->Color;
	scButton_zamek->Font=aFont;
	scButton_zamek->Font->Color=barva;
	scButton_zamek->Font->Size=velikost;
	barva=scGPButton_posun_dalsich_elementu->Font->Color;
	scGPButton_posun_dalsich_elementu->Font=aFont;
	scGPButton_posun_dalsich_elementu->Font->Color=barva;
	scGPButton_posun_dalsich_elementu->Font->Size=velikost;
	////
	scGPComboBox_prepinacKot->Left=scGPLabel_prepinacKot->Left+scGPLabel_prepinacKot->Width;//změna zarovnání
}
//---------------------------------------------------------------------------
//záležitost s novým designem
void TForm1::DesignSettings()
{
	log(__func__);//logování
	//maximalizace formuláře jinak to s novým designem nejde
	Form1->Width=Screen->WorkAreaWidth;
	Form1->Height=Screen->WorkAreaHeight;
	FMaximized=false;MaxButtonClick(this);//aby bylo připraveno minimalizační tlačítko
	//nastavení globálních barev
	TColor light_gray=(TColor)RGB(240,240,240);
 //	TColor active_blue=(TColor)RGB(0,120,215);
	TColor clDrawGridHeaderFont=m.clIntensive((TColor)RGB(43,87,154),30);
	PopupMenuButton->Left = 0;
	PopupMenuButton->Visible = false;
	DetailsButton->Left = 0;
	DetailsButton->Visible = true;
	scSplitView_OPTIONS->Opened=false;
	scSplitView_OPTIONS->Align=alRight;
	scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPlay;
	scGPGlyphButton_PLAY->Visible=true;

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

	//nastaveni barvy prepinacu modu

	Analyza->Options->PressedColor=DetailsButton->Options->NormalColor;
	Nahled->Options->PressedColor=DetailsButton->Options->NormalColor;
  //nastavení barvy fontu stisknutému tlačítku
	//Schema->Options->FontPressedColor=clWhite;
	//Layout->Options->FontPressedColor=clWhite;
	Analyza->Options->FontPressedColor=clWhite;
	Nahled->Options->FontPressedColor=clWhite;

	//nastavení barvy pro knihovnu
	scGPLabel_roboti->FillColor=light_gray;
	scGPLabel_roboti->FillColor2=light_gray;
	scGPLabel_otoce->FillColor=light_gray;
	scGPLabel_otoce->FillColor2=light_gray;
	scGPLabel_stop->FillColor=light_gray;
	scGPLabel_stop->FillColor2=light_gray;
	scGPLabel_geometrie->FillColor=light_gray;
	scGPLabel_geometrie->FillColor2=light_gray;
	scGPLabel_poznamky->FillColor=light_gray;
	scGPLabel_poznamky->FillColor2=light_gray;
	scSplitView_LEFTTOOLBAR->ShadowBorderColor=light_gray;
	scGPPanel_bottomtoolbar->FrameColor=light_gray;

	//scExPanel_ostatni->Top=72+27;

	if(MOD==LAYOUT) //zobrazeni labelu - je hezci, v hlavicce drawgrid knihovny
	{
		//scGPLabel_roboti->Visible=true;
//		scGPLabel_roboti->Caption="Technolog. objekty";
//		scGPLabel_roboti->ContentMarginLeft=4;
	 //	scListGroupKnihovObjektu->Visible=false;
	  DrawGrid_knihovna->Top=10000;
		scListGroupKnihovObjektu->Height=1920; // kvůli odstranění bílé linky, která vznikala pod knihovnou objektů
	}

	m.designButton(scGPButton_ulozit,Form1,1,2);
	m.designButton(scGPButton_zahodit,Form1,2,2);
	////design spodní lišty////
	scGPPanel_bottomtoolbar->Top=scGPPanel_statusbar->Top-scGPPanel_bottomtoolbar->Height;
	scGPPanel_bottomtoolbar->Width=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
	scGPPanel_bottomtoolbar->Left=scSplitView_LEFTTOOLBAR->Width;
	//vodorovné zarovnání prvků
	scGPButton_zahodit->Left=scGPPanel_bottomtoolbar->Width/2+11-68;
	scGPButton_ulozit->Left=scGPButton_zahodit->Left-scGPButton_zahodit->Width-22;
	scGPImage_mereni_vzdalenost->Left=22-4;//scGPImage_zamek_posunu->Left+scGPImage_zamek_posunu->Width+22-13;
	scGPImage_zamek_posunu->Left=scGPImage_mereni_vzdalenost->Left+scGPImage_mereni_vzdalenost->Width+22-7;//okraj komponenty != okraji obrázku
 //	scGPLabel1->Left=scGPImage_mereni_vzdalenost->Left+scGPImage_mereni_vzdalenost->Width+22-8;
 scGPLabel_prepinacKot->Left=scGPImage_zamek_posunu->Left+scGPImage_zamek_posunu->Width+22;//label k přepínači kót
 //	scGPComboBox_orientace->Left=scGPLabel1->Left+scGPLabel1->Width;
	scGPComboBox_prepinacKot->Left=scGPLabel_prepinacKot->Left+scGPLabel_prepinacKot->Width;//combobox na přepínání mezi kotami čas -- delka
	scGPButton_posun_dalsich_elementu->Left=scGPPanel_bottomtoolbar->Width-scGPButton_posun_dalsich_elementu->Width-25;
	scButton_zamek->Left=scGPButton_posun_dalsich_elementu->Left-scButton_zamek->Width-18;
	scGPButton_viditelnostKoty->Left=scButton_zamek->Left-scGPButton_viditelnostKoty->Width-19;
	scGPButton_viditelnostmGrid->Left=scGPButton_viditelnostKoty->Left-scGPButton_viditelnostmGrid->Width-22;
	scGPButton_geometrie->Left=scGPButton_viditelnostmGrid->Left-scGPButton_geometrie->Width-22;
	scGPGlyphButton_odstran_cestu->Left=scGPButton_zahodit->Left+scGPButton_zahodit->Width+22;
	//svislé zarovnání prvků
	scGPButton_ulozit->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_ulozit->Height)/2;
	scGPButton_zahodit->Top=scGPButton_ulozit->Top;
//	scGPComboBox_orientace->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_orientace->Height)/2;
	scGPComboBox_prepinacKot->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_prepinacKot->Height)/2;//combobox na přepínání mezi kotami čas -- delka
 //	scGPLabel1->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel1->Height)/2;
	scGPLabel_prepinacKot->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel_prepinacKot->Height)/2;
	offset_scGPButton_bug_report=20;//10;//používá se na více místech
	scGPButton_bug_report->Top=ClientHeight-scGPPanel_statusbar->Height-scGPButton_bug_report->Height-offset_scGPButton_bug_report;
	scGPButton_bug_report->Left=ClientWidth-scGPButton_bug_report->Width-offset_scGPButton_bug_report;
	scGPImage_mereni_vzdalenost->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_mereni_vzdalenost->Height)/2;
	scGPImage_zamek_posunu->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_zamek_posunu->Height)/2;
	scGPButton_posun_dalsich_elementu->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_posun_dalsich_elementu->Height)/2;
	scButton_zamek->Top=(scGPPanel_bottomtoolbar->Height-scButton_zamek->Height)/2;
	scGPButton_viditelnostKoty->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostKoty->Height)/2;
	scGPButton_viditelnostmGrid->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostmGrid->Height)/2;
	scGPButton_geometrie->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_geometrie->Height)/2;
	scGPGlyphButton_odstran_cestu->Top=(scGPPanel_bottomtoolbar->Height-scGPGlyphButton_odstran_cestu->Height)/2;
	////design přepínače člověk robot////
	scGPPanel_pomocn_proSwitch->FillColor=scGPLabel_roboti->FillColor;//barva panelu pod přepínačem určuje barvu pozadí přepínače
	scGPPanel_pomocn_proSwitch->FillColor2=scGPLabel_roboti->FillColor2;
	scGPPanel_pomocn_proSwitch->FillColorAlpha=scGPLabel_roboti->FillColorAlpha;
	//design statusbaru
	design_statusbar();
	//pozice ovládacích prvků
//	scListGroupNastavProjektu->Left=0;
//	scListGroupNastavProjektu->Top=0;
//	scListGroupKnihovObjektu->Left=0;
 /*	scListGroupKnihovObjektu->Top=2+scListGroupNastavProjektu->Height+0;
	scListGroupKnihovObjektu->Height=scGPPanel_statusbar->Top-(2+scListGroupNastavProjektu->Height+0+DetailsButton->Height);
	vyska_menu=0;  */

	//dočasné vypnutí záložky editace
	Nahled->Visible=false;

	////default plnění ls
	ls=new TStringList;
	UnicodeString text="";
	for(unsigned short i=0;i<=522;i++)
	{
		switch(i)
		{
			case 0:text="Jazyk";break;
			case 1:text="čeština";break;
			case 2:text="mongolština";break;
			case 3:text="angličtina";break;
			case 4:text="Smazat cestu";break;
			case 5:text="Smazat úsek cesty";break;
			case 6:text="Nová zakázka s hlavní cestou";break;
			case 7:text="Nová zakázka s cestou poslední zakázky";break;
			case 8:text="nový pohon";break;
			case 9:text="Výhybka";break;
			case 10:text="Spojka";break;
			case 11:text="V";break;
			case 12:text="S";break;
			case 13:text="Přesunem dojde k překrytí pracovních oblastí, chcete element přesunout?";break;
			case 14:text="  Rozmístit tabulky";break;
			case 15:text="Automaticky dopojit geometrii na nejbližší pohon?";break;
      case 16:text="Předchozí pohled";break;
			case 17:text="Pauza simulace";break;
			case 18:text="PLAY";break;
			case 19:text="Konec";break;
      case 20:text="Layout";break;
			case 21:text="Menu";break;
			case 22:text="Menu";break;
      case 23:text="Animace";break;
			case 24:text="Uložit";break;
			case 25:text="Nový";break;
      case 26:text="Otevřít";break;
      case 27:text="Otevřít";break;
			case 28:text="Menu";break;
      case 29:text="Parametry linky";break;
      case 30:text="Analýza";break;
      case 31:text="O aplikaci";break;
			case 32:text="Ověřování";break;
      case 33:text="Návrh";break;
      case 34:text="parametry linky";break;
      case 35:text="Nastavení";break;
      case 36:text="Editace";break;
			case 37:text="spustit animaci";break;
      case 38:text="Aktuální polohu kurzoru v souřadnicích aplikace";break;
			case 39:text="Kliknutím na libovolné místo přidáte objekt z knihovny nebo lze upravit stávájící layout";break;
      case 40:text="Rychlá nápověda aplikace";break;
      case 41:text="přichytávat";break;
      case 42:text="Kliknutím změníte režim přichytávání objektu ke mřížce";break;
      case 43:text="Uzamknout rozmístění objektů";break;
      case 44:text="Změřit vzdálenost";break;
      case 45:text="Technolog. objekty";break;
			case 46:text="Stop stanice";break;
      case 47:text="Otoče";break;
      case 48:text="Geometrie linky";break;
      case 49:text="Poznámky";break;
      case 50:text="Hala";break;
      case 51:text="Nav/Svěšování";break;
      case 52:text="Technologie";break;
			case 53:text="Ostatní";break;
      case 54:text="Ostatní";break;
			case 55:text="Robot           Operátor";break;
      case 56:text="Sekce";break;
      case 57:text="Operátoři";break;
      case 58:text="Roboti";break;
      case 59:text="Otoče";break;
			case 60:text="Stop stanice";break;
      case 61:text="Geometrie linky";break;
      case 62:text="Zobrazit grafy";break;
      case 63:text="Zprávy ";break;
			case 64:text="Report ";break;
      case 65:text="Pohon - objektu";break;
      case 66:text="Zobrazit legendu";break;
			case 67:text="použít zadané kapacity objektů";break;
      case 68:text="animovat i jednokapacitní";break;
      case 69:text="Orientace náhledu:";break;
      case 70:text="Uložit";break;
      case 71:text="Storno";break;
      case 72:text="Hodnoty kót:";break;
			case 73:text="Měření vzdálenosti";break;
			case 74:text="Povolit posun objektu";break;
      case 75:text="spustit animaci";break;
      case 76:text="Použít zadané kapacity objektů";break;
      case 77:text="Animovat i jednokapacitní";break;
      case 78:text="Zohlednit čištění pistole a výměnu barev";break;
      case 79:text="Zobrazit vytíženost objektů";break;
      case 80:text="Počet vozíků dle WIP+1";break;
			case 81:text=" Filtrovat časové úseky";break;
			case 82:text=" Doba čekání na palec";break;
      case 83:text="přepočítej";break;
      case 84:text="definice zakázek";break;
      case 85:text="Konec";break;
      case 86:text="Otevřít";break;
      case 87:text="Nastavení";break;
			case 88:text="Obnovit ze zálohy";break;
      case 89:text="Report";break;
      case 90:text="Antialiasing";break;
			case 91:text="Uložit jako...";break;
      case 92:text="Export...";break;
      case 93:text="Naposledy otevřený";break;
      case 94:text="Nový...";break;
			case 95:text="Uložit";break;
      case 96:text="Načíst nový podklad";break;
      case 97:text="Nastavení";break;
      case 98:text="                  Vrstvy";break;
      case 99:text="Koleje dopravníku";break;
			case 100:text="Pozice v buffrech";break;
      case 101:text="Znázornění rotace jigů";break;
			case 102:text="Popisky prvků";break;
      case 103:text="                  Podklad";break;
      case 104:text="Nastavit měřítko";break;
      case 105:text="Umístit podklad";break;
      case 106:text="Podklad ve stupních šedi";break;
      case 107:text="Zobrazit podklad";break;
      case 108:text="Průhlednost podkladu";break;
			case 109:text="                  Obecné";break;
      case 110:text="Pravoúhlé zadávání";break;
      case 111:text="Zajišťuje zadavání obrysu haly či objektu pravoúhle ";break;
      case 112:text="Mřížka";break;
      case 113:text="Měřítko";break;
      case 114:text="Intenzita pasiv. Objektů";break;
      case 115:text="Nastavení intenzity needitovaných objektů";break;
			case 116:text="                  Nástroje";break;
      case 117:text="Změřit vzdálenost";break;
			case 118:text="Zakázky";break;
      case 119:text="Odemknout editaci";break;
      case 120:text="Zamknout editaci mimo hodnot tabulek";break;
      case 121:text="Skrýt tabulky";break;
      case 122:text="Zobrazit tabulky";break;
			case 123:text="Skrýt kóty";break;
      case 124:text="Zobrazit kóty";break;
      case 125:text="Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";break;
      case 126:text="Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";break;
			case 127:text="Zadejte parametry linky nebo zvolte storno.";break;
      case 128:text="Navržené pohony pro objekty bez přiřazených pohonů:";break;
      case 129:text="podlahový";break;
			case 130:text="podvěsný";break;
      case 131:text="Uložit";break;
      case 132:text="Storno";break;
      case 133:text="Konec";break;
      case 134:text="Parametry linky";break;
      case 135:text="Info";break;
			case 136:text="Info";break;
			case 137:text="Načíst původní uložená data";break;
      case 138:text=" Vozík";break;
      case 139:text=" Pohony";break;
      case 140:text="Navržené pohony pro objekty bez přiřazených pohonů";break;
      case 141:text="Přidat do seznamu použitelných pohonů";break;
      case 142:text="  Jig";break;
      case 143:text="  Zobrazit parametry";break;
			case 144:text="  Smazat pohon";break;
			case 145:text="  Smazat nepoužité";break;
      case 146:text="  Kopírovat pohon";break;
      case 147:text="  Nastavit parametry";break;
      case 148:text="Info";break;
      case 149:text="Smazat nepoužívané pohony";break;
      case 150:text="nastavit TaktTime";break;
			case 151:text="nastavit jig/podvozek";break;
      case 152:text="přidat nový pohon";break;
      case 153:text=" Takt Time";break;
			case 154:text="Jig";break;
      case 155:text="Podvozek";break;
      case 156:text="maximální rozměry vozíku<br>směr pohybu linky";break;
      case 157:text="Přidat nový pohon";break;
			case 158:text="Nastavení aplikace";break;
      case 159:text="Katalog dopravníků";break;
      case 160:text="pozice palce";break;
      case 161:text="Uložit";break;
      case 162:text="pohled na jig při navěšování";break;
			case 163:text="  Zobrazit";break;
      case 164:text="  Editovat";break;
			case 165:text="  Změnit typ sekce";break;
      case 166:text="  Přidat bod";break;
      case 167:text="  Vložit zarážku";break;
      case 168:text="  Zobrazit";break;
      case 169:text="  Kopírovat";break;
      case 170:text="  Smazat";break;
      case 171:text="  Smazat sekci č.";break;
			case 172:text="  Smazat bod č.";break;
      case 173:text="  Otočit objekt doleva";break;
      case 174:text="  Otočit objekt doprava";break;
      case 175:text="  Posun obrysu";break;
      case 176:text="  Rychlý export…";break;
      case 177:text="  Posouvat";break;
      case 178:text="  Posunout";break;
			case 179:text="  Přiblížit";break;
      case 180:text="  Oddálit";break;
			case 181:text="  Vybrat oknem…";break;
      case 182:text="  Celý pohled";break;
      case 183:text="  Poznámka";break;
      case 184:text="Kopírovat hodnoty";break;
      case 185:text="Ano";break;
			case 186:text="OK";break;
      case 187:text="Ne";break;
      case 188:text="Storno";break;
      case 189:text="Pamatovat si volbu";break;
			case 190:text="Vybrat dopravník";break;
      case 191:text="rádius";break;
      case 192:text="Katalog dopravníků";break;
			case 193:text="Kliknutím do seznamu rádiusů vyberete katalog dopravníků";break;
      case 194:text="Uložit";break;
      case 195:text="Storno";break;
      case 196:text="Rozteč palců [mm]";break;
      case 197:text="Horizontální oblouky";break;
      case 198:text="Rádius [mm]";break;
			case 199:text="Vertikální oblouky";break;
			case 200:text="Název";break;
      case 201:text="Rozmezí a rychlost pohonu";break;
      case 202:text="Rozteč palce";break;
      case 203:text="Používán";break;
      case 204:text="od";break;
      case 205:text="do";break;
      case 206:text="aktuální  ";break;
			case 207:text="Smazat tento pohon";break;
			case 208:text="Rychlost";break;
      case 209:text="Palce";break;
      case 210:text="Násobek";break;
      case 211:text="Jigů";break;
      case 212:text="Podvozky";break;
      case 213:text="Jigy 0°";break;
			case 214:text="Jigy 90°";break;
      case 215:text="Mezera";break;
      case 216:text="Rozšířené položky";break;
			case 217:text="Žádný pohon";break;
      case 218:text="Vyberte pohon";break;
      case 219:text="Pohon nepřiřazen";break;
      case 220:text="Rychlost neodpovídá rozmezí!";break;
			case 221:text="Zadejte doporučenou rychlost pohonu:";break;
      case 222:text="Neplatná hodnota rychlosti pohonu!";break;
      case 223:text="Párová stop";break;
      case 224:text="WT palec";break;
      case 225:text="potenciál";break;
			case 226:text="použité";break;
      case 227:text="časová rezerva";break;
			case 228:text="čas uzavřené STOP stanice";break;
      case 229:text="maximální možná doba čekání na palec";break;
      case 230:text="maximální možný počet pozic v bufferu";break;
      case 231:text="Rozšířené položky";break;
      case 232:text="LO";break;
      case 233:text="délka lakovácího okna";break;
      case 234:text="PO";break;
			case 235:text="délka pracovní oblasti";break;
      case 236:text="Vyosení";break;
      case 237:text="celkový čas procesu, který je složen z dílčích časů";break;
      case 238:text="vyosení lakovácího okna";break;
      case 239:text="Rotace";break;
      case 240:text="Otoč";break;
      case 241:text="Otáčení";break;
			case 242:text="celkový čas procesu otoče";break;
      case 243:text="délka otoče";break;
			case 244:text="délka otáčení";break;
      case 245:text="PTo";break;
      case 246:text="Délka";break;
      case 247:text="začátek";break;
      case 248:text="střed";break;
			case 249:text="celý";break;
      case 250:text="Max. počet voziků musí být menší nebo roven";break;
      case 251:text="Nelze, před Stopstanicí se nachází oblouk";break;
      case 252:text="okap";break;
			case 253:text="postřikový rám ";break;
      case 254:text="navěšování ";break;
      case 255:text="svěšování";break;
			case 256:text="kontinuální";break;
      case 257:text="kontinuální s  ";break;
      case 258:text="pasiv. otočí";break;
      case 259:text="akt. otočí";break;
      case 260:text="ožeh";break;
			case 261:text="ionizace";break;
      case 262:text="lakování";break;
			case 263:text="aktivní";break;
      case 264:text="pasivní";break;
			case 265:text="ionizační tyč";break;
      case 266:text="text ";break;
      case 267:text="šipka";break;
      case 268:text="geometrie  ";break;
			case 269:text="před. místo";break;
			case 270:text="ION Tyč";break;
      case 271:text="Předávací místo";break;
      case 272:text="Operátor";break;
			case 273:text="Otoč";break;
			case 274:text="pohon nevybrán";break;
      case 275:text="Před";break;
			case 276:text="Po";break;
			case 277:text="Zóna otáčení před středem";break;
      case 278:text="Zóna otáčení za středem";break;
      case 279:text="vytěkání";break;
      case 280:text="sušení";break;
      case 281:text="chlazení";break;
			case 282:text="výtah";break;
      case 283:text="přejezd";break;
			case 284:text="výhybka";break;
      case 285:text="nedefinovaný";break;
      case 286:text="Nastavte správný rozsah a rychlost pohonu.";break;
			case 287:text="Spodní hranice rychlosti musí být větší nebo rovna";break;
      case 288:text="Horní hranice rychlosti musí být menší nebo rovna";break;
      case 289:text="Spodní hranice rychlosti musí být >=";break;
      case 290:text="Nebo horní hranice musí být <=";break;
			case 291:text="Parametry pohonu již nelze měnit, protože pohon je používán na jiných objektech";break;
			case 292:text="Palce řetězu";break;
      case 293:text="Úhel [°]";break;
      case 294:text="Probíhá stahování aktualizace, neukončujte aplikaci!!! Po dokončení stahování bude program ukončen.";break;
      case 295:text="Tažením myši z vybraného bodu do cílového bodu zobrazíte vzdálenost mezi těmito body.";break;
      case 296:text="Tažením myši z vybraného bodu na podkladu směřujte do vybraného technolog. objektu, po puštění myši dojde ke kalibraci obrazu.";break;
			case 297:text="Tažením myši z výchozího bodu směřujte do cílového bodu podkladu, po puštění myši zadejte skutečnou vzdálenost mezi body v metrech či milimetrech.";break;
      case 298:text="TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru.";break;
      case 299:text="TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu.";break;
      case 300:text="TIP: Posun obrazu lze také vykonat pomocí stisknutého levého tlačítka myší a posunem myši požadovaným směrem.";break;
      case 301:text="TIP: Přiblížení oknem je možné také realizovat pomocí stisknuté klávesy CTRL a výběrem požadované oblasti pomocí myši při stisknutém levém tlačítku.";break;
			case 302:text="Probíhá stahování šablony...";break;
			case 303:text="Nový text: Správné umístění a nastavení měřítka je možné konfigurovat v Nastavení->Podklad";break;
      case 304:text="Kliknutím na objekt v knihovně objektu, tažením a následným usazením přidáte objekt.";break;
      case 305:text="Pro zakreslení haly je nutné zadat minimálně 3 body.";break;
      case 306:text="Pro uzavření haly stiskněte ENTER nebo dvakrát klikněte.";break;
			case 307:text="Prvek nelze přesunout!";break;
      case 308:text="Kliknutím určíte bod posunu, táhnutím posunete obrys objektu.";break;
			case 309:text="Lze vkládat pouze na linie.";break;
      case 310:text="Přidávání prvků je možné až po výběru pohonu.";break;
      case 311:text="Nelze smazat.";break;
			case 312:text="Klinutím na levé tlačítko myši přidejte bod.";break;
      case 313:text="Kliknutím a tažením myši posunujete celý objekt.";break;
      case 314:text="Pro nastavovaný vozík je potřeba buffer o délce";break;
			case 315:text="Nelze odstranit předávací místo.";break;
      case 316:text="Je k dispozici aktualizace TISPLu. Chcete ji stáhnout?";break;
			case 317:text="Po dokončení staženým souborem přepište současný EXE soubor.";break;
      case 318:text="byl změněn. Chcete ho před ukončením uložit?";break;
      case 319:text="Není k dispozici přípojení k internetu nebo vypršela licence, aplikace nebude spuštěna!";break;
      case 320:text="Nezdařilo se připojení k serveru, aplikace nebude spuštěna!";break;
			case 321:text="Nezdařilo se připojení k licenčnímu serveru, aplikace nebude spuštěna!";break;
			case 322:text="Aplikace nebyla řádně ukončena. Byl obnoven poslední Vámi uložený soubor.";break;
      case 323:text="Aplikace nebyla řádně ukončena. Chcete ze zálohy obnovit poslední neuložený soubor?";break;
      case 324:text="Přesunem dojde k překrytí lakovacích oken, chcete element přesunout?";break;
      case 325:text="Přesunem dojde k zásahu do zóny otáčení, chcete element přesunout?";break;
      case 326:text="Vzdálenost";break;
			case 327:text="čas";break;
      case 328:text="Objekty nenavazují, automaticky posunout následující objekt pro zachování návaznosti?";break;
      case 329:text="Chcete objekt";break;
      case 330:text="umístit v pořadí mezi objekty";break;
      case 331:text="a";break;
      case 332:text="Vložením dojde k překrytí lakovacích oken, chcete element vložit?";break;
			case 333:text="Vložením dojde k narušení zóny otáčení, chcete element vložit?";break;
      case 334:text="Výhybku lze nastavit, pokud jsou k dispozici minimálně 3 technologické objekty!";break;
      case 335:text="Chcete opravdu smazat";break;
      case 336:text="Chcete opravdu smazat bod č.";break;
      case 337:text="Chcete opravdu smazat sekci číslo";break;
      case 338:text="Nelze smazat sekci, v objektu musí být nejméně dvě sekce!";break;
			case 339:text="Nelze smazat objekt, který je součástí technologické cesty zakázky např.:";break;
			case 340:text="Chcete opravdu objekt";break;
      case 341:text="smazat?";break;
      case 342:text="Neplatná hodnota!";break;
      case 343:text="Chcete uložit změny objektu";break;
      case 344:text="Zadaná data nejsou validní a nebudou uložena. Opravdu přejít do jiného objektu?";break;
			case 345:text="Soubor";break;
      case 346:text="nebyl nalezen!";break;
      case 347:text="Neplatná verze souboru formátu *.tispl!";break;
      case 348:text="Žádná data k reportu!";break;
			case 349:text="Verze";break;
      case 350:text="Umístění:";break;
			case 351:text="Informace o aplikaci";break;
      case 352:text="Nejdříve je nutné zakreslit schéma linky!";break;
      case 353:text="Nepodařilo se najít nebo stáhnout šablonu. Průvodce nebude spuštěn!";break;
      case 354:text="Pozor, pro objekt";break;
      case 355:text="nebyl přiřazen pohon. Doba čekání na palce není u těchto objektů zohledněna!";break;
      case 356:text="Chcete pohony objektům přiřadit nyní?";break;
			case 357:text="Pozor, následující pohony nemají uvedenou rozteč palců řetezu:";break;
			case 358:text="Doba čekání na palce není tedy u uvedených objektů zohledněna!";break;
      case 359:text="Chcete chybějící rozteče pohonů nyní zadat?";break;
      case 360:text="Čekání na palce nastane</b> dle nastavení v parametrech objektu (není-li uživatelsky zakázáno):";break;
      case 361:text="po odchodu z objektu v S&G režimu";break;
      case 362:text="mezi kontinuálními objekty, mají-li jiný pohon";break;
			case 363:text="mezi postprocesními objekty, mají-li jiný pohon";break;
      case 364:text="mezi kontinuálním a postprocesním objektem, mají-li jiný pohon";break;
      case 365:text="mezi postprocesním a kontinuálním objektem";break;
      case 366:text="vždy, když je za objektem definovaná stopka";break;
			case 367:text="vždy, když je čekání uživatelsky nařízeno";break;
      case 368:text="Dojde k odstranění haly, chcete pokračovat?";break;
			case 369:text="Pozor, nelze uložit hodnoty rozmezí pohonů, protože následující objekty mají rychlost mimo nově nastavený rozsah:";break;
      case 370:text="Není vybrán žádný dopravník, není možné přidat pohon";break;
      case 371:text="Opravdu chcete smazat nepoužívané pohony?";break;
      case 372:text="Pohon je používáný, opravdu má být zrušeno přiřazení?";break;
      case 373:text="Pohon je používán objekty:";break;
      case 374:text="Opravdu má být pohon smazán?";break;
			case 375:text="přichytávat";break;
			case 376:text="Kliknutím na libovolné místo přidáte objekt z knihovny";break;
      case 377:text="Datum expirace licence:";break;
      case 378:text="Kliknutím na libovolné místo přidáte objekt z knihovny nebo lze upravit stávájící schéma";break;
      case 379:text="Zobrazení navrženého layoutu";break;
      case 380:text="Zobrazení technologických procesů v čase formou časových os.";break;
			case 381:text="Zobrazení technologických procesů v čase v jednotlivých objektech.";break;
      case 382:text="mřížka skryta";break;
      case 383:text="Ortogonalizace schématu dokončena.";break;
      case 384:text="Kliknutím na libovolné místo umístíte objekt";break;
			case 385:text="Ortogonalizace vypnuta.";break;
      case 386:text="Ortogonalizace zapnuta.";break;
			case 387:text="Kliknutím na libovolné místo umístíte vybraný element.";break;
      case 388:text="nepřichytávat";break;
      case 389:text="Soubor byl již uložen...";break;
      case 390:text="Soubor uložen...";break;
      case 391:text="zálohován do";break;
			case 392:text="Export do";break;
      case 393:text="dokončen.";break;
			case 394:text="Rychlý export dokončen.";break;
      case 395:text="OVĚŘOVÁNÍ";break;
      case 396:text="NÁVRH";break;
      case 397:text="Následně bude otevřena šablona. Zadejte název projektu, pod kterým bude uložena.";break;
      case 398:text="Zadejte parametry linky nebo zvolte storno.";break;
			case 399:text="Vytvořte schéma a pomocí pravého tlačítka nastavte parametry objektů.";break;
      case 400:text="Vozík:";break;
      case 401:text="Rotace neodpovídá orientaci JIGů na začátku linky!";break;
      case 402:text="Pozor, překrytí JIGů!";break;
			case 403:text="Zadejte parametry zakázek nebo zvolte storno.";break;
      case 404:text="  Otočit název doleva";break;
      case 405:text="  Otočit název doprava";break;
      case 406:text="Nestíhá se přejezd, záporná časová rezerva!";break;
      case 407:text="Nulová časová rezerva.";break;
			case 408:text="Vložit jako novou sekci? Ne - změnit na jiný typ sekce.";break;
      case 409:text="Odemknout rozmístění objektů";break;
      case 410:text="Zapnout přichytávání";break;
      case 411:text="Vypnout přichytávání";break;
			case 412:text="Zprávy ";break;
      case 413:text="Připnout k pravé části";break;
      case 414:text="Počet chyb";break;
      case 415:text="Počet varování";break;
      case 416:text="Objekt byl přesunut, souhlasíte s aktuálním umístěním?";break;
			case 417:text="maximální čas";break;
      case 418:text="včetně max. WT palec";break;
      case 419:text="Čas STOP";break;
      case 420:text="doba čekání v bufferu";break;
			case 421:text="RT není relevantní, některý z objektů nemá pohon!";break;
      case 422:text="Nerelevantní hodnota časové rezervy (RT), na některém objektu není přiřazen pohon!";break;
      case 423:text="Nelze editovat, layout je zamčený";break;
			case 424:text="Nepodařilo se načíst podklad";break;
			case 425:text=", chcete provést nové načtení?";break;
			case 426:text="Kvůli překryvu jigů nelze nastavit větší počet vozíků než";break;
      case 427:text="místo pro úchyt palce";break;
      case 428:text="Smazat podklad";break;
			case 429:text="S&G s";break;
			case 430:text="Geometrie následujícího objektu nenavazuje, po uložení bude možné geometrii navázat";break;
      case 431:text="Linka nenavazuje, přejete si automaticky dokončit?";break;
      case 432:text="Definice zakázek";break;
      case 433:text="Přidat novou zakázku";break;
      case 434:text="Název zakázky";break;
			case 435:text="počet";break;
      case 436:text="prázdných";break;
      case 437:text="celkem";break;
      case 438:text="Vytvořit novou dávku";break;
			case 439:text="Smazat zakázku";break;
      case 440:text="Dávka";break;
      case 441:text="Smazat dávku";break;
      case 442:text="Barva zakázky";break;
      case 443:text="Zobrazit kóty geometrie";break;
			case 444:text="Skrýt kóty geometrie";break;
      case 445:text="Rozmístění vozíků";break;
      case 446:text="Kliknutím potvrdíte cestu";break;
      case 447:text="Pohon";break;
			case 448:text="Rozmezí";break;
      case 449:text="Zpět";break;
      case 450:text="Vpřed";break;
      case 451:text="Nastavená";break;
      case 452:text="Rychl.";break;
			case 453:text="Rozteč";break;
      case 454:text="Editace úseku, souhlasíte? Rozdíl =";break;
      case 455:text="Chcete automaticky spojit geometrii?";break;
      case 456:text="Nelze uložit zakázky, nebyla zvolena aktuální zakázka.";break;
			case 457:text="Automaticky umístit na střed objektu?";break;
      case 458:text="Nahlásit";break;
      case 459:text="Popis";break;
      case 460:text="Zaslat chybu, dotaz nebo připomínku";break;
      case 461:text="Směr pohonu";break;
			case 462:text="Nelze kombinovat použití S&G a kontinuálního pracovního režimu.";break;
      case 463:text="Zóna";break;
      case 464:text="Pozice";break;
      case 465:text="V úseku za předávacím místem bude nastaven pohon:";break;
			case 466:text="Vložením dojde k překrytí pracovních oblastí, chcete element vložit?";break;
      case 467:text="Počátek a konec linky byl automaticky spojen.";break;
      case 468:text="Pozor, na následujícím úseku přejezdu dochází k překrytí JIGů!";break;
      case 469:text="Nerelevantní hodnota časové rezervy, na některém objektu není přiřazen pohon!";break;
      case 470:text="Nelze provést! Odstranění provedete výběrem totožného pohonu před a za předávacím místem";break;
			case 471:text="Zaslat chybu, dotaz nebo připomínku";break;
      case 472:text="Popis";break;
      case 473:text="prosím, zadejte svůj komentář";break;
      case 474:text="Odeslat včetně projektu";break;
			case 475:text="Odeslat";break;
      case 476:text="Odesláno. Děkujeme za zpětnou vazbu.";break;
      case 477:text="Nepodařilo se odeslat, zkuste znovu.";break;
      case 478:text="Poklepáním na vzniklý úsek zahájíte tvorbu geometrie";break;
      case 479:text="  Zobrazit stěny";break;
      case 480:text="  Skrýt stěny";break;
      case 481:text="Změna se projeví po přechodu do Layout";break;
      case 482:text="vyosení pracovní oblasti";break;
      case 483:text="Kliknutím do libovolné oblasti zahájíte měření";break;
			case 484:text="Úsek";break;
			case 485:text="Čas";break;
      case 486:text="přejezd";break;
      case 487:text="celkem";break;
      case 488:text="Projekt nelze otevřít. Verze načítaného projektu -";break;
			case 489:text=". Požadovaná verze -";break;
      case 490:text="stoupání";break;
      case 491:text="klesání";break;
			case 492:text="Automaticky smazat";break;
			case 493:text="Změna hodnoty ovlivní všechny prvky na tomto pohonu.";break;
      case 494:text="Došlo ke změně parametrů prvků na pohonu";break;
			case 495:text=". Verze aplikace -";break;
			case 496:text="Podklad již existuje, přejete si načíst nový?";break;
      case 497:text="žádné";break;
      case 498:text="maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)";break;
      case 499:text="Stoupání či klesání lze zadat pouze po směru linky.";break;
      case 500:text="Vkládaní se nezdařilo, musíte se držet v oblasti pohonu.";break;
			case 501:text="Kliknutím do oblasti pohonu vložíte začátek.";break;
			case 502:text="Tažením myši a následným kliknutím vložíte koncový bod.";break;
			case 503:text="Tažením myši přizpůsobujete oblast měření. Kliknutím ukončíte.";break;
			case 504:text="Uchopením a tažením v oblasti pohonu změníte umístění teploměru.";break;
			case 505:text="Dop. hodnota PT je maximálně";break;
			case 506:text="Přichyceno k";break;
			case 507:text="Přichyceno k hraně kabiny";break;
			case 508:text="Přichyceno k vozíku č.";break;
			case 509:text="Přichyceno k začátku S/K";break;
			case 510:text="Přichyceno ke konci S/K";break;
			case 511:text="Byla změněna geometrie linky, oblast teploměrů bude obnovena.";break;
			case 512:text="Součet procesních časů může být maximálně";break;
			case 513:text="Dop. hodnota PT1 je maximálně";break;
			case 514:text="Dop. hodnota PTo je maximálně";break;
			case 515:text="Dop. hodnota PT2 je maximálně";break;
			case 516:text="Upozornění,v bufferu je vyšší počet vozíku, než je nastaveno.";break;
			case 517:text="Duplicitní název.";break;
			case 518:text="Nepovedlo se automaticky dopojit geometrii.";break;
			case 519:text="Přichyceno k pohonu";break;
			case 520:text="Skrýt";break;
			case 521:text="Měření";break;
			case 522:text="Zprávy o lince";break;
			default:text="";break;
		}
		ls->Insert(i,text);//vyčištění řetězců, ale hlavně založení pro default! proto nelze použít  ls->Clear();
	}
}
//---------------------------------------------------------------------------
//zakázání či povolení grafických uživatelských prvků dle úrovně edice
void TForm1::edice()
{
	log(__func__);//logování
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
				Toolbar_Ulozit->Enabled=false;
				scButton_ulozit->Enabled=false;
				scButton_ulozjako->Enabled=false;
        scButton_report->Enabled=false;
        scButton_zaloha->Enabled=false;
        scButton_posledni_otevreny->Enabled=false;
				scButton_otevrit->Enabled=false;
				Toolbar_Ulozit->Enabled=false;
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
				//DrawGrid_knihovna->Enabled=false;
			//	Nastvitparametry1->Enabled=false;
				//Smazat1->Enabled=false;
        scGPButton_mereni_vzdalenost->Enabled=true;
				scLabel1_svetelnost->Enabled=false;
				scGPCheckBox_stupne_sedi->Enabled=false;


//				RzToolButton1->Enabled=false;
//				RzToolButton2->Enabled=false;
//				RzToolButton3->Enabled=false;

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
    log(__func__);//logování
		//zjištění aktuální verze
		UnicodeString AKT_VERZE=IdHTTP1->Get("http://81.2.243.72/TISPL/verze.txt");
		//porovnání akt. verze a používané verze aplikace
		if(AKT_VERZE!=FileVersion)//je k dispozici aktualizace
		{
			UnicodeString text=ls->Strings[316],text_1=ls->Strings[317],tip=ls->Strings[294];
			if(mrYes==MB(text,MB_YESNO,false))
			{
				MB(text_1);
				zobraz_tip(tip);
				SaveDialog->Title="Uložit soubor jako";
				SaveDialog->DefaultExt="*.exe";
				SaveDialog->Filter="Aktualizace TISPLu (*.exe)|*.exe";
				SaveDialog->FileName="TISPL_aktualizace";
				if(SaveDialog->Execute())
				{
						TMemoryStream *MemoryStream=new TMemoryStream();
						IdHTTP1->Get("http://81.2.243.72/TISPL/LIC/"+LICENCE+"/tispl.exe",MemoryStream);
						MemoryStream->SaveToFile(SaveDialog->FileName);
						log2web("aktualizace_z_"+AKT_VERZE+"_na_"+FileVersion);
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
  log(__func__);//logování
  //DPI handling
	int DPI;
	DPI = get_DPI();
	if (DPI != 96) Form_Z_rozliseni->ShowModal();
	// startUP() - pokud byl zde, dělalo to "chybu v paměti" při spuštění release verze	startUP();//při aktivaci formuláře startující záležitosti, pro zpřehlednění ko
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//volání založení nový soubor z menu = nové nastavení souboru, samotné výchozí nastevení aplikace je v konstruktoru
void __fastcall TForm1::NovySouborClick(TObject *Sender)
{
	 log(__func__);//logování
	 ESC();//ukončení případné akce
	 //if(duvod_k_ulozeni) UlozitClick(this);
	 Novy_soubor();//samotné vytvoření nového souboru
	 if(!duvod_k_ulozeni)//pouze pokud byl založen nový soubor
	 {
		 //následující slouží pouze při uživatelsky volaném soubor nový
		 TIP=ls->Strings[304];
		 SB(ls->Strings[304],4);
		 Nahled->Enabled=false;
		 akutalizace_stavu_prichytavani_vSB();//v novém souboru dochází k nastavení na přichytávat, proto aktualizace stavu
		 Button_dopravnik_parametryClick(this);//zavolání formáláře pro prvotní vyplnění či potvzení hodnot parametrů linky
		 //pokud byl v přechozím projektu zamčen layout je potřeba přepnout zámky na aktuální stav, při novém projektu vždy otevřeno
		 if(scButton_zamek_layoutu->ImageIndex!=68)
		 {
			 scButton_zamek_layoutu->ImageIndex=68;
			 Schema->ImageIndex=78;
			 scButton_zamek_layoutu->Hint=ls->Strings[43];//"Zamknout layout";
		 }
		 //odstranění staré aktuální zakázky
		 d.v.ZAKAZKA_akt=NULL;
		 d.v.update_akt_zakazky();//aktualizace defaultní
   	 REFRESH();
	 }
}
//---------------------------------------------------------------------------
//samotné vytvoření nového souboru
void TForm1::Novy_soubor(bool invalidate)
{
   log(__func__);//logování
	 scSplitView_MENU->Opened=false;
	 scButton_novy->Down=false;
	 bool novy=true;
	 if(duvod_k_ulozeni)
	 {
		 AnsiString FNs=FileName_short(FileName),text=" "+ls->Strings[318];
		 if(FNs=="")FNs="Nový.tispl";
		 int result=MB(FNs+text,MB_YESNOCANCEL);
		 switch(result)
		 {
			 case mrYes:     UlozitClick(this); if(!stisknuto_storno)novy=true;else novy=false; break;
			 case mrNo:      novy=true; break;
			 case mrCancel:  novy=false; break;
		 }
	 }

	 if(novy)
	 {
		 if(MOD==EDITACE)vypni_editaci();//vypne editaci, nemanipuluje s daty, ani nepřekresluje
		 //odstranění vektorů
		 vse_odstranit();
		 //nové vytvoření hlaviček
		 d.v.HALA.body=NULL;
		 d.v.hlavicka_OBJEKTY();//založení spojového seznamu pro technologické objekty
		 d.v.hlavicka_POHONY();//založení spojového seznamu pro pohony
		 d.v.hlavicka_ZAKAZKY();//založení spojového seznamu pro zakázky
		 d.v.hlavicka_VOZIKY();// nemusí tu být pokud nebudu ukládat vozíky do filuzaložení spojového seznamu pro vozíky

		 //tady bude přepnutí založek dodělat
			 if(MOD!=LAYOUT)schemaClick(this);//volání MODu LAYOUT

		 Zoom=1.0; on_change_zoom_change_scGPTrackBar();
		 Zoom_predchozi=1.0;
		 Posun.x=-scListGroupKnihovObjektu->Width;if(vyska_menu>0)Posun.y=-vyska_menu+9;else Posun.y=-29;
		 Posun_predchozi.x=Posun.x;Posun_predchozi.y=Posun.y;
		 jedno_ze_tri_otoceni_koleckem_mysi=1;
		 doba_neotaceni_mysi=0;

		 d.v.PP.cas_start=TIME.CurrentDateTime();//TDateTime(AnsiString(TIME.CurrentDate().DateString())+" "+"8:00:00");//defaultně dnes v 8:00
		 d.v.PP.vytvoril=get_user_name();
		 d.v.PP.cas_posledni_upravy=d.v.PP.cas_start;
		 d.v.PP.upravil=d.v.PP.vytvoril;
		 d.v.PP.mnozstvi=20000;
		 d.v.PP.hod_den=8;
		 d.v.PP.dni_rok=365;
		 d.v.PP.TT=120.0;
		 d.v.PP.efektivita=95;
		 d.v.PP.delka_jig=1;
		 d.v.PP.sirka_jig=1;
		 d.v.PP.vyska_jig=1;
		 d.v.PP.delka_podvozek=1;
		 d.v.PP.sirka_podvozek=0.34;
		 d.v.PP.uchyt_pozice=d.v.PP.delka_jig/2.0;//výchozí umístění vozíku je v polovině
		 d.v.PP.zamek_layoutu=false;
		 d.v.PP.typ_linky=0;
		 d.v.PP.raster.filename="";
		 d.v.PP.raster.resolution=m2px;
		 d.v.PP.raster.X=0;
		 d.v.PP.raster.Y=0;
		 d.v.PP.raster.show=false;
		 d.v.PP.katalog=0;
		 d.v.PP.radius=1;
		 d.v.pocet_vyhybek=0;

		 //nastavení def. hodnot přichytávání a ortogonalizace
		 prichytavat_k_mrizce=1;
		 ortogonalizace_stav=true;

		 //Akce=NIC;Akce_temp=NIC;Screen->Cursor=crDefault;//změní kurzor na default

		 DuvodUlozit(false);
		 //	 RzToolButton4->Enabled=false;
		 //	 RzToolButton5->Enabled=false;
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
		 Analyza->Down=false;
		 scGPButton_bug_report->Top=ClientHeight-scGPPanel_statusbar->Height-scGPButton_bug_report->Height-offset_scGPButton_bug_report;//zarovnání buttonu
		 scGPButton_bug_report->Left=ClientWidth-scGPButton_bug_report->Width-offset_scGPButton_bug_report;
		 //Schema->Down=true;
		 FileName="Nový.tispl";
		 scLabel_titulek->Caption=Caption+" - ["+FileName+"]";
		 TIP="";
     //kontrola, zda nemám zamčeny layout z předchozího projektu, pokud ano odemknout
		 if(scButton_zamek_layoutu->ImageIndex==67)scButton_zamek_layoutuClick(this);
		 d.SCENA=vychozi_stav_sceny;
		 if(invalidate)Invalidate();//vhodnější invalidate než refresh, způsobuje dvojtý problik při otevírání souboru!!
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
  log(__func__);//logování
	if (/*DEBUG||*/!DEBUG)//	if(!DEBUG)  R - úprava 5.6.2020 - test konektivity i pro DEBUG verzi   //RELEASE
	{
		//toto odkomentovat pro spuštění TTR
		if(!ttr("start"))
		{
			Timer_tr->Enabled=false;//ještě je ale z důvodu ochrany enabled=true v object inspectoru, toto je spíše na zmatení
			Close();
		}
		else//toto odkomentovat pro spuštění TTR a slouží pro startUP
		//Timer_tr->Enabled=false;// toto zakomentovat pro spuštění TTR
		{startUP();}//toto vždy odkomentované
	}
	else//DEBUG
	{
		Timer_tr->Enabled=false;
    startUP();
	}
}
//---------------------------------------------------------------------------
//Metoda pro trial verzi
bool TForm1::ttr(UnicodeString Text)
{
  log(__func__);//logování
	//licence
	Timer_tr->Enabled=true;
	UnicodeString LIC_FILE	= LICENCE;
	UnicodeString Text_error=ls->Strings[319];
	TIME=TDateTime("1.1.1990 0:00:00");
	TDateTime TIME_expirace;
	UnicodeString Response="error";
	bool STATUS=false;

	try
	{
		////zjištění expirace trialverze
		////textaková verze
		Response=IdHTTP1->Get(AnsiString("http://www.lyzarskejihlavsko.cz/tispl/")+LIC_FILE+UnicodeString(".lic"));
		////DB - řešení, ověřeno bylo funkční
		//FDQuery1->Active = False;
		//FDQuery1->Open("SELECT  DATE_FORMAT(expiration_date ,'%d.%m.%Y %H:%i:%s') AS expiration_date FROM app_setup WHERE id=\"1\"");  //id nahradit id z ini
		//FDQuery1->Active = True;
		//AnsiString Response = FDQuery1->Fields->Fields[0]->AsAnsiString;

		try
		{
			//timeservrové řešení
			try
			{
				IdSNTP1->Host="pool.ntp.org";//	IdTime1->Host="128.138.140.44";//stará konstukce - občas nevracela čas
				TIME=IdSNTP1->DateTime;
			}
			catch(...)//v případě nedostupnosti timeserveru, zkusí ještě jiný
			{
			 IdSNTP1->Host="cz.pool.ntp.org";
       TIME=IdSNTP1->DateTime;
			}
			Form_uvod->Label_status->Visible=false;
			TIME_expirace=TDateTime(Response);
     // AnsiString t;
     // t=TIME.TimeString();
     // Memo(t);
			if(TIME_expirace<TIME && TIME!="1.1.1990 0:00:00")
			//if(Response!="ano")//verze bez timeservru
			{
				log2web(ms.replace(Response,"_"," ")+"-"+Text+"_EXPIRACE");
				MB(Text_error);//vypršela licence resp. program expiroval;
				duvod_k_ulozeni=false;
				Timer_tr->Enabled=false;
				Close();
			}
			else //VŠE OK
			{
				UnicodeString text=ls->Strings[377]+" ";
				log2web(ms.replace(Response,"_"," ")+"-"+Text+"_OK");
				SB(text+TIME_expirace);//pouze výpis do StatusBaru, do kdy je platná verze
				//aktualizace();//kontrola dostupnosti aktualizace
				STATUS=true;
			}
		}
		catch(...)//nezdařilo se připojení k time serveru, timeout
		{
			log2web(ms.replace(Response,"_"," ")+"-"+Text+"_TIMESERVER_ERR");
			//todo if(++n_prihlaseni>=3)//až při třetím chybovém stavu
			{
				//Text_error="Nezdařilo se připojení k time serveru, aplikace nebude spuštěna!";
				Text_error=ls->Strings[320];
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
		Text_error=ls->Strings[321];
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
	log(__func__);//logování
	if(!Form_mereni->closing && !Form_mereni->Showing &&!Form_zpravy->closing && !Form_zpravy->Showing && !PopUPmenu->Showing && !PopUPmenu->closing && !myMessageBox->Showing && !myMessageBox->closing && !Form_definice_zakazek->Showing && !Form_definice_zakazek->closing)//pozn. dole ještě větev else if(PopUPmenu->Showing || PopUPmenu->closing)PopUPmenu->Close();//pokud je spuštěné pop-up menu, tak ho vypne
	{
    //načtení jazykové mutace, nemůže být v konstruktoru, protože ještě neexistují všechny dílčí formuláře = nelze k nim přistoupit
		//language=(TForm1::Tlanguage)load_language(language,false);//aktivovani jazyk mutaci, problém s přepnutím jazyka při nenalezení souboru, proto metoda varací zvolený jazyk
		//změna stavu vyvolává load_language!!!!!!
		int old_state=scGPSwitch1->State;
		if(language==CS)scGPSwitch1->State=scswOn;//nastavení switche jazyků do zpávné polohy
		else scGPSwitch1->State=scswOff;
		if(old_state==scGPSwitch1->State)language=(TForm1::Tlanguage)load_language(language,false);//neproběhla změna stavu potřeba udělat ručně
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
      UnicodeString text=ls->Strings[322],text_1=ls->Strings[323];
      if(ftWrite.dwHighDateTime>=ftWrite_bac.dwHighDateTime)MB(text);//pokud je uložený soubor mladší nebo stejně starý jako jeho BAC
      else
      {
        if(mrYes==MB(text_1,MB_YESNO))
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
    if(T=="0" || T==""){ortogonalizace_stav=false;}else{ortogonalizace_stav=true;}
    start_ortogonalizace=false;
    T=readINI("Nastaveni_app","prichytavat");
    if(T=="0" || T==""){prichytavat_k_mrizce=1;}else{prichytavat_k_mrizce=ms.MyToDouble(T);}
    //zatím nepoužíváme, bude spíše souviset přímo se souborem, v případě použití nutno vyhodit implicitní volbu návrhář v sobuor novy
    //T=readINI("Nastaveni_app","status");
    //if(T=="0" || T=="")STATUS=NAVRH;else STATUS=OVEROVANI;

    //aktivace tlačítka editace
    if(d.v.OBJEKTY->dalsi!=NULL)Nahled->Enabled=true;
    else Nahled->Enabled=false;
    //slouží po startu programu k načtení parametrů linky, nemůže být voláno v tomto okamžiku v souboru nový, protože by jinak vedlo k pádu aplikace - pořadí vytváření formulářů, není voláno v případě startu aplikace po pádu
		//if(volat_parametry_linky)Button_dopravnik_parametryClick(this);//zavolání formáláře pro prvotní vyplnění či potvzení hodnot parametrů linky, původní pozice
    if(d.v.OBJEKTY->dalsi!=NULL)TIP="";//v případě, že jsou vložené nějaké objekty tak dojde k odmazání tipu pro vkládání objektů
		//set_font();//nastavení fontu komponentám
		nastav_focus();//nutné při spouštění předávat focus, přesměrování všech události (např. KeyDown) na Form1
		//načtení zámku layoutu
		if(d.v.PP.zamek_layoutu)//zamčen
  	{
			scSplitView_LEFTTOOLBAR->Opened=false;
			scGPButton_prichytavat->Visible=false;
			//RzStatusPane5->Visible=false;
			scButton_zamek_layoutu->ImageIndex=67;
  		Schema->ImageIndex=79;
  		scButton_zamek_layoutu->Hint=ls->Strings[409];//"Odemknout layout";
  	}
		else//odemčen
		{
			vytvoreni_tab_knihovna();//vytvoření knihovny objektů
			scButton_zamek_layoutu->ImageIndex=68;
  		Schema->ImageIndex=78;
			scButton_zamek_layoutu->Hint=ls->Strings[43];//"Zamknout layout";
		}
		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Open();//pokud bylo nastavení otevřeno nastavení pro editaci podkladu, nutné znova otevřít po vytvoření tabulky objektů
		if(d.v.OBJEKTY->dalsi==NULL)
    {
			TIP=ls->Strings[304];
			SB(ls->Strings[304],4);
		}
		SB(ls->Strings[378]+" ");
		akutalizace_stavu_prichytavani_vSB();
		if(volat_parametry_linky)Button_dopravnik_parametryClick(this);//zavolání formáláře pro prvotní vyplnění či potvzení hodnot parametrů linky, testovací pozice
		vytvor_obraz();//vytvoření prvotního obrazu
		d.v.update_akt_zakazky();//pokud je uživatelská provede aktualizace, pokud ne aktualizuje defaultní
	}
	else if(PopUPmenu->Showing || PopUPmenu->closing)PopUPmenu->Close();//pokud je spuštěné pop-up menu, tak ho vypne
	Akce=NIC;
}
//---------------------------------------------------------------------------
//zajišťuje zápis do INI aplikace, museli jsme dát do výjimky, protože jednou hodilo error
void TForm1::writeINI(AnsiString Section,AnsiString Ident,AnsiString Value)
{
  log(__func__);//logování
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
  log(__func__);//logování
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
//odešle e-mail, doručitel na všech třech úrovní To,ccTo,bccTo mohou být mnohonásobně zadaní, pouze odělené čárkou, tělo e-mailu lze zadat jako html, návratová hodnota vrací úspěšnost provedení
bool TForm1::mail(String Host,String Username,String Password,String FromAddress,String FromName,String Subject,String Body,String To,String ccTo,String bccTo,String FileName,String FileName2)
{
	try
	{
		TIdMessage *MAIL=new TIdMessage(this);
		MAIL->Clear();
		MAIL->From->Address=FromAddress;
		MAIL->From->Name=FromName;
		MAIL->Subject=Subject;
		MAIL->Recipients->EMailAddresses=To;
		MAIL->CCList->EMailAddresses=ccTo;
		MAIL->BccList->EMailAddresses=bccTo;
	// 	if(FileName!="" || FileExists(FileName)) MAIL->ContentType="multipart/text";//text/plain
	//  else  MAIL->ContentType="text/html";//text/plain
		MAIL->CharSet="UTF-8";
		MAIL->Body->Text=AnsiToUtf8(Body);
		TIdAttachmentFile *Attach;
		if(FileName!="") Attach=new TIdAttachmentFile(MAIL->MessageParts,FileName);//potřebuje #include <idattachmentfile.hpp>
		if(FileName2!="")Attach=new TIdAttachmentFile(MAIL->MessageParts,FileName2);//potřebuje #include <idattachmentfile.hpp>
		TIdSMTP *SMTP=new TIdSMTP(this);
		SMTP->Host=Host;//"smtp.seznam.cz";
		SMTP->Username=Username;
		SMTP->Password=Password;
		SMTP->Port=587;//25;//SMTP->UseTLS=utNoTLSSupport; případně použít, pro použití SSL jiný port a zároveň potřeba s SMTP propojit přes IO handler SSL komponentu + 2x patřičné DLL
		SMTP->Connect();
		SMTP->Send(MAIL);
		SMTP->Disconnect(true);
		delete Attach;//musí být jako první
		delete MAIL;
		delete SMTP;
		return true;
	}
	catch(Exception &exeption)//v případě chyby odeslání
	{
    log(__func__,exeption.Message);
		return false;//"Odeslání se nezdařilo! či jiná chyba
	}
}
//---------------------------------------------------------------------------
//Zalogování na webu
//automaticky přidá parametry (čas, uživatel, licence)
void TForm1::log2web(UnicodeString Text)
{
	log(__func__);//logování
	if(!DEBUG)
	{
		try
		{
				//textáková verze
				log2webOnlyText(ms.DeleteSpace(LICENCE)+"_"+get_computer_name()+"_"+get_user_name()+"_"+TIME.CurrentDate()+"_"+TIME.CurrentTime()+"|"+Text);

				//DB funkční verze - nemazata
//			AnsiString relation_id=GetCurrentProcessId();
//			AnsiString send_log_time= TIME.CurrentDateTime();
//			AnsiString ID ="1";
//			AnsiString strSQL = "INSERT INTO log_table (app_id,app_start,username,send_log_time,command,relation_id,verze) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\",\""+VERZE+"\")";
//			FDConnection1->ExecSQL(strSQL);
		}
		catch(...){;}//např. není připojení k internetu, tak pouze nezaloguje, dořešit uložení logu do doby získání připojení a volání opětovného odeslání logu
	}
}
//---------------------------------------------------------------------------
//pouze text
void TForm1::log2webOnlyText(UnicodeString Text)
{
  log(__func__);//logování
	//   varianta odesílání dat přes GET
	//IdHTTP1->Get(UnicodeString("http://85.255.8.81/tispl/skript_tispl.php?hash=erDSQgregdvgFEFSDDeporhrfFGOI98886732dfgorvmqwerfdvvcBHDE")+Text);

	//varianta odesílání dat přes POST
	TStringList *request = new TStringList;
	TStringList *response = new TStringList();

	request->Clear();
	//IdHTTP1->IOHandler = IdSSLIOHandlerSocketOpenSSL1;
	IdHTTP1->Request->ContentType = "application/x-www-form-urlencoded";
	request->Values["heslo"]    ="erDSQgregdvgFEFSDDeporhrfFGOI98886732dfgorvmqwerfdvvcBHDE";
	request->Values["data"]     = Text;

	response->Text = IdHTTP1->Post("http://www.lyzarskejihlavsko.cz/tispl/skript_tispl.php", request);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zapíše log do textového souboru a přidá datum
void TForm1::log(AnsiString Text,AnsiString Text2)
{
	if(DEBUG && logovat && LogFileStream!=NULL)//pouze pro DEBUG
	{
		//přídání datumu k textu
		Text=TIME.CurrentDate().DateString()+"_"+TIME.CurrentTime().TimeString()+"_"+Text+"_"+Text2+"\r\n";
		//samotný zápis do sobourou
		LogFileStream->Write(Text.c_str(),Text.Length());
  }
}
//---------------------------------------------------------------------------
void TForm1::SaveText2File(AnsiString Text,AnsiString FileName)
{
	log(__func__);//logování
	///////////////////////////převod dat do UTF8
	/*WideString WData=Text;
	char *CHData=new char[Text.Length()+1];
	WideCharToMultiByte(/*CP_ACP*//*CP_UTF8,0,WData,Text.Length()+1,CHData,Text.Length()+1,NULL,NULL);
	*/

	//zapis data do souboru
	TMemoryStream* MemoryStream=new TMemoryStream();
	MemoryStream->Clear();
	MemoryStream->Write(Text.c_str(),Text.Length());//Win
	//MemoryStream->Write(CHData,Text.Length());//UTF8
	MemoryStream->SaveToFile(FileName);
	delete MemoryStream;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  log(__func__);//logování
	////design spodní lišty////
	scGPPanel_bottomtoolbar->Top=scGPPanel_statusbar->Top-scGPPanel_bottomtoolbar->Height;
	scGPPanel_bottomtoolbar->Width=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
	scGPPanel_bottomtoolbar->Left=scSplitView_LEFTTOOLBAR->Width;
	//vodorovné zarovnání prvků
	scGPButton_zahodit->Left=scGPPanel_bottomtoolbar->Width/2+11-68;
	scGPButton_ulozit->Left=scGPButton_zahodit->Left-scGPButton_zahodit->Width-22;
	scGPImage_mereni_vzdalenost->Left=22-4;//scGPImage_zamek_posunu->Left+scGPImage_zamek_posunu->Width+22-13;
	scGPImage_zamek_posunu->Left=scGPImage_mereni_vzdalenost->Left+scGPImage_mereni_vzdalenost->Width+22-7;//okraj komponenty != okraji obrázku
 //	scGPLabel1->Left=scGPImage_mereni_vzdalenost->Left+scGPImage_mereni_vzdalenost->Width+22-8;
 scGPLabel_prepinacKot->Left=scGPImage_zamek_posunu->Left+scGPImage_zamek_posunu->Width+22;//label k přepínači kót
 //	scGPComboBox_orientace->Left=scGPLabel1->Left+scGPLabel1->Width;
	scGPComboBox_prepinacKot->Left=scGPLabel_prepinacKot->Left+scGPLabel_prepinacKot->Width;//combobox na přepínání mezi kotami čas -- delka
	scGPButton_posun_dalsich_elementu->Left=scGPPanel_bottomtoolbar->Width-scGPButton_posun_dalsich_elementu->Width-25;
	scButton_zamek->Left=scGPButton_posun_dalsich_elementu->Left-scButton_zamek->Width-18;
	scGPButton_viditelnostKoty->Left=scButton_zamek->Left-scGPButton_viditelnostKoty->Width-19;
	scGPButton_viditelnostmGrid->Left=scGPButton_viditelnostKoty->Left-scGPButton_viditelnostmGrid->Width-22;
	scGPButton_geometrie->Left=scGPButton_viditelnostmGrid->Left-scGPButton_geometrie->Width-22;
	scGPGlyphButton_odstran_cestu->Left=scGPButton_zahodit->Left+scGPButton_zahodit->Width+22;
	//svislé zarovnání prvků
	scGPButton_ulozit->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_ulozit->Height)/2;
	scGPButton_zahodit->Top=scGPButton_ulozit->Top;
//	scGPComboBox_orientace->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_orientace->Height)/2;
	scGPComboBox_prepinacKot->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_prepinacKot->Height)/2;//combobox na přepínání mezi kotami čas -- delka
 //	scGPLabel1->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel1->Height)/2;
	scGPLabel_prepinacKot->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel_prepinacKot->Height)/2;
	offset_scGPButton_bug_report=20;//10;//používá se na více místech
	scGPButton_bug_report->Top=ClientHeight-scGPPanel_statusbar->Height-scGPButton_bug_report->Height-offset_scGPButton_bug_report;
	if(OBJEKT_akt!=NULL)scGPButton_bug_report->Top-=scGPPanel_bottomtoolbar->Height;
	scGPButton_bug_report->Left=ClientWidth-scGPButton_bug_report->Width-offset_scGPButton_bug_report;
	scGPImage_mereni_vzdalenost->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_mereni_vzdalenost->Height)/2;
	scGPImage_zamek_posunu->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_zamek_posunu->Height)/2;
	scGPButton_posun_dalsich_elementu->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_posun_dalsich_elementu->Height)/2;
	scButton_zamek->Top=(scGPPanel_bottomtoolbar->Height-scButton_zamek->Height)/2;
	scGPButton_viditelnostKoty->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostKoty->Height)/2;
	scGPButton_viditelnostmGrid->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostmGrid->Height)/2;
	scGPButton_geometrie->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_geometrie->Height)/2;
	scGPGlyphButton_odstran_cestu->Top=(scGPPanel_bottomtoolbar->Height-scGPGlyphButton_odstran_cestu->Height)/2;
  //design statusbaru
	design_statusbar();
	//horní lišta
//	if(MOD==EDITACE)scGPGlyphButton_zpravy_ikona->Left=Nahled->Left-scGPGlyphButton_zpravy_ikona->Width;
//	else scGPGlyphButton_zpravy_ikona->Left=Schema->Left-scGPGlyphButton_zpravy_ikona->Width;
	if(/*MOD==REZERVY ||*/ MOD==CASOVAOSA)Invalidate();
	else REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::schemaClick(TObject *Sender)
{
	log(__func__);//logování
	if(MOD==EDITACE && Schema->ImageIndex!=-1)scButton_zamekClick(this);
	if(MOD==LAYOUT)scButton_zamek_layoutuClick(this);
	if(MOD!=LAYOUT && MOD!=EDITACE)
	{
	ESC();//zruší případnou rozdělanou akci
	MOD=LAYOUT;
	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	//scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	//scSplitView_LEFTTOOLBAR->Visible=true;
	scListGroupKnihovObjektu->Top=0;
//	scListGroupNastavProjektu->Visible=true;
	//scListGroupKnihovObjektu->Visible=true;
 //	scListGroupKnihovObjektu->Top=scListGroupNastavProjektu->Height;
	DuvodUlozit(true);
	Timer_neaktivity->Enabled=false;
  scGPCheckBox_zobraz_podklad->Align=alTop;
  scGPCheckBox_zobraz_podklad->Left=5;
	scGPCheckBox_stupne_sedi->Align=alTop;
  scGPCheckBox_stupne_sedi->Left=scGPCheckBox_zobraz_podklad->Left;
	//scLabel1_svetelnost->Top=scGPCheckBox_stupne_sedi->Top +  44;  //pár px navíc kvůli vzdušnosti
  scGPTrackBar_svetelnost_posuvka->Top=scLabel1_svetelnost->Top;

	scGPButton_kalibrace->Left=3;
	scGPButton_adjustace->Left=scGPButton_kalibrace->Left;
	scGPButton_smazat->Left=scGPButton_kalibrace->Left;
	scGPButton_kalibrace->Top=scGPTrackBar_svetelnost_posuvka->Top + scLabel1_svetelnost->Height + 9;  //pár px navíc kvůli vzdušnosti
	scGPButton_adjustace->Top=scGPButton_kalibrace->Top +  scGPButton_kalibrace->Height ;
	scGPButton_smazat->Top=scGPButton_adjustace->Top+scGPButton_adjustace->Height;

	//scGPCheckBox_stupne_sedi->Top=scGPCheckBox_zobraz_podklad->Top+scGPCheckBox_zobraz_podklad->Height;
	//scLabel1_svetelnost->Top= scGPCheckBox_stupne_sedi->Top + scGPCheckBox_stupne_sedi->Height;
 // scGPTrackBar_svetelnost_posuvka->Top  = scLabel1_svetelnost->Top;
	//scGPButton_kalibrace->Top = scLabel1_svetelnost->Top + scLabel1_svetelnost->Height;
  //scGPButton_adjustace->Top = scLabel1_svetelnost->Top + scLabel1_svetelnost->Height;
 //	scGPButton_header_def_zakazek->Visible=false;
 //	scGPButton_header_param_linky->Visible=false;
	Pan_bmp->Width=0;Pan_bmp->Height=0;//při přechodu z jiného režimu smaže starou Pan_bmp
	//Invalidate();
	}
	//else Nahled->Down=true;//nebylo potvrzeno uzavření náhledu, úprava lišty
	Schema->Options->NormalColor=DetailsButton->Options->NormalColor;
	Schema->Options->FrameNormalColor=DetailsButton->Options->NormalColor;
	Layout->Options->NormalColor=scGPPanel_mainmenu->FillColor;
	Layout->Options->FrameNormalColor=scGPPanel_mainmenu->FillColor;
	Schema->Down=false;//po stisku zůstává tlačítko down
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LayoutClick(TObject *Sender)
{
	log(__func__);//logování
	ESC();//zruší případnou rozdělanou akci
	MOD=LAYOUT;
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
 //	scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	//scSplitView_LEFTTOOLBAR->Visible=false;
	SB(ls->Strings[379]);
	//Zoom=5;ZOOM();
	Pan_bmp->Width=0;Pan_bmp->Height=0;//při přechodu z jiného režimu smaže starou Pan_bmp
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AnalyzaClick(TObject *Sender)
{
	log(__func__);//logování
	log("Otevřeni editace zakázek, MOD=LAYOUT, Form=Superform");
//	d.v.vytvor_default_zakazku();//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji pouze přiřadí cestu dle schématu
////	if(d.v.ZAKAZKY->dalsi==NULL)//pokud nebyla zakazka definovaná - nyní řeší příkaz nad
////	{
////		MB("Pro zobrazení je nutné ve formuláři definice zakázek zadat plán výroby!");
////	}
////	else
//	{
//		if(d.v.VOZIKY->dalsi==NULL)d.v.generuj_VOZIKY();//situace kdy nejsou načtené vozíky ale existuje zakázka s cestou (situace např. po načtení nového souboru), tak se vygeneruji dle zadané zakazky/cesty vozíky
//
//		if(MOD!=CASOVAOSA)//aby se nevolalo zbytečně znovu, pokud už v daném modu je, ale může být dvousečné ve významu uživatelské užitečnosti
//		{
//			MOD=CASOVAOSA;
//			ESC();//zruší případně rozdělanou akci
//			if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
//			d.mod_vytizenost_objektu=false;
//			Timer_simulace->Enabled=false;
//			d.PosunT.x=0;//výchozí posunutí obrazu Posunu na časových osách, kvůli možnosti posouvání obrazu
//			d.PosunT.y=0;
//			//zneplatnit_minulesouradnice();//zrušeno test
//			DuvodUlozit(true);
//			scSplitView_MENU->Opened=false;//zavře případně otevřené menu
//			scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
//	 //		scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
//			//scSplitView_LEFTTOOLBAR->Visible=false;
//			//scListGroupKnihovObjektu->Visible=false;
//			Button3->Visible=false;
//			Timer_neaktivity->Enabled=true;
//			pocitadlo_doby_neaktivity=0;//implicitní hodnota
//			Timer_animace->Enabled=false;
//			ButtonPLAY->Visible=false;
//			CheckBoxVymena_barev->Visible=true;
//			CheckBoxVytizenost->Visible=true;
//			CheckBoxVytizenost->Checked=false;
//			CheckBoxVytizenost->Top=135;
//			scLabel_doba_cekani->Visible=true;
//			scGPGlyphButton_info_cekani->Visible=true;
//			scGPGlyphButton_close_grafy->Visible=true;
//			CheckBoxAnimovatSG->Visible=false;
//			ComboBoxODmin->Visible=false;
//			ComboBoxDOmin->Visible=false;
//			rComboBoxKrok->Visible=false;
//			LabelRoletka->Visible=false;
//			CheckBox_pouzit_zadane_kapacity->Visible=false;
//			ComboBoxCekani->Visible=true;
//			d.JIZPOCITANO=false;d.RANDOM=true;
//			ComboBoxCekani->Width=scSplitView_OPTIONS->OpenedWidth-7;
//			if(Form1->ComboBoxCekani->ItemIndex==2)
//			{
//				scGPButton_generuj->Visible=true;
//				ComboBoxCekani->Width=196;
//			}
//
//			Label_zamerovac->Visible=false;
//			scGPCheckBox_ortogon->Visible=false;
//			if(STATUS==NAVRH)scGPCheckBox_pocet_voziku_dle_WIP->Visible=true;
//			scGPGlyphButton_close_legenda_casove_osy->Left=0;
//			scGPGlyphButton_close_legenda_casove_osy->Top=ClientHeight-scGPPanel_statusbar->Height-ClientHeight/3;
//			scGPGlyphButton_close_legenda_casove_osy->Visible=true;
//			SB(ls->Strings[380]);
//			Invalidate();
//		}
//	}
//	RM();//korekce chyby oskakování pravého menu
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	Analyza->Down=false;//musí se uvlonit po stisku
	if(MOD!=EDITACE)
	{
		ESC();//zruší případnou rozdělanou akci
		Analyza->Options->NormalColor=DetailsButton->Options->NormalColor;
		Analyza->Options->FrameNormalColor=DetailsButton->Options->NormalColor;
		Schema->Options->NormalColor=scGPPanel_mainmenu->FillColor;
		Schema->Options->FrameNormalColor=scGPPanel_mainmenu->FillColor;
		Form_definice_zakazek->ShowModal();
		REFRESH();
	}
	else
	{
		KonecClick(this);
		if(OBJEKT_akt==NULL)//došlo k ukončení náhledu
		{
			Analyza->Options->NormalColor=DetailsButton->Options->NormalColor;
			Analyza->Options->FrameNormalColor=DetailsButton->Options->NormalColor;
			Schema->Options->NormalColor=scGPPanel_mainmenu->FillColor;
			Schema->Options->FrameNormalColor=scGPPanel_mainmenu->FillColor;
			Form_definice_zakazek->ShowModal();//návratová hodnota se řeši v knihovně
		}
	}
}
//---------------------------------------------------------------------------
//skryje či zobrazí mřížku
void __fastcall TForm1::scGPSwitch_gridChangeState(TObject *Sender)
{
  log(__func__);//logování
	scSplitView_MENU->Opened=false;
	grid=!grid;
	if(!grid)SB(ls->Strings[382],5);
	else akutalizace_stavu_prichytavani_vSB();
	REFRESH();
}
//---------------------------------------------------------------------------
void TForm1::SB(UnicodeString Text, unsigned short Pane)
{
	log(__func__);//logování
	switch(Pane)
	{
	  //case 1:RzStatusPane1->Caption=Text;break;  //zrušený statuspane
		//case 2:RzStatusPane2->Caption="Zoom: "+Text+"x";break; už se nepoužívá
		case 3:scLabel_statusbar_1->Caption="["+Text+"] m";break;
		case 4:scLabel_statusbar_2->Caption=Text;break;
		case 5:scLabel_statusbar_0->Caption=Text;break;
		case 6:scLabel_statusbar_1->Caption=Text;break;//pokud nechci vypisovat metry
		default:scLabel_statusbar_0->Caption=Text; break;

		//case 3:RzStatusPane3->Caption="["+Text+"] m";break;
//		case 4:RzStatusPane4->Caption=Text;break;
//		case 5:RzStatusPane5->Caption=Text;break;
//		case 6:RzStatusPane3->Caption=Text;break;//pokud nechci vypisovat metry
//		default:RzStatusPane5->Caption=Text; break;
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
//usnadňuje přístup k ShowMessage - Rosta
void TForm1::Sr(UnicodeString Text,AnsiString umisteni)
{
	S(umisteni+"/Rosta\n"+Text);
}
//---------------------------------------------------------------------------
//vola rychle myMessageBox
int TForm1::MB(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit,int width,bool default_button_caption,bool copy_zobrazit)
{
  log(__func__);//logování
	return myMessageBox->Show(left,top,text,caption_text,mbTYPE,centrovat_text,checkbox_zobrazit,width,default_button_caption,copy_zobrazit);
}
int TForm1::MB(UnicodeString text,int mbTYPE,bool centrovat_text,int width,bool default_button_caption,bool blurForm1,bool copy_zobrazit)
{
  log(__func__);//logování
	if(blurForm1)
	{
		scStyledForm1->InActiveClientBlurAmount=1;
		scStyledForm1->ShowClientInActiveEffect();
	}
	int RET=myMessageBox->Show(text,mbTYPE,centrovat_text,width,default_button_caption,copy_zobrazit);
	if(blurForm1)scStyledForm1->HideClientInActiveEffect();
	return RET;
}
//---------------------------------------------------------------------------
//usnadňuje přístup ke zprávám, pokud jsou jen prázdné uvozovky (a druhý paremetry na false - což je implicitně), vymaže zpravu, parametr add rozhoduje, zda bude nový text předen k předešlému textu či nikoliv, pokud zpráva obsahuje nějaký text, je zobrazena ikona zprávy, poslední parametr je barva ikony zprávy
void TForm1::Z(UnicodeString Text,bool add,TColor color)
{
  log(__func__);//logování
	//plnění zprávy textem
//	if(add)scHTMLLabel_log_vypis->Caption = scHTMLLabel_log_vypis->Caption+Text;//text má být přidán k předchozímu textu
//	else scHTMLLabel_log_vypis->Caption = Text;//text má být přidán samostatně

	//zobrazení či skrývání
	if(Text=="" && add==false)//vše OK
	{
//		scGPGlyphButton_zpravy_ikona->GlyphOptions->NormalColor=clWhite;
//		scHTMLLabel_log_vypis->Caption="";
		//Memo2->Lines->Clear();
	}
	else
	{
		//není možné: scExPanel_log_header->Visible=true; //expanel s obsahem zprávy
//		scGPGlyphButton_zpravy_ikona->Visible=true;//ikona
//		scGPGlyphButton_zpravy_ikona->GlyphOptions->NormalColor=color;//barva ikony
	}

  //Memo2->Visible=true;
	//Memo2->Lines->Add(Text);
}
//---------------------------------------------------------------------------
//metoda volá kurzory aplikace
void TForm1::kurzor(TKurzory typ_kurzor)
{
	log(__func__);//logování
	if(typ_kurzor>0)Screen->Cursor=(TCursor)typ_kurzor;
	else Screen->Cursor=crDefault;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
	////////při změně rozlišení nebo obrazovky dojde k MAXIMALIZACI OKNA programu  - problém při ruční minimalizaci!
	if(ClientWidth!=Monitor->Width&&FMaximized)
	{
		//maximalizace formuláře jinak to s novým designem nejde
		Form1->Width=Screen->WorkAreaWidth;
		Form1->Height=Screen->WorkAreaHeight;
		FMaximized=false;MaxButtonClick(this);//aby bylo připraveno minimalizační tlačítko
	}

	Zoom_predchozi_AA=Zoom;//musí být tu, před MÓDy (mohl by být i před kreslením gridu)

	///////při přechodu z nebo do módu TVORBA_CESTY
	if(MOD==TVORBA_CESTY && scSplitView_LEFTTOOLBAR->Visible)
	{
		scSplitView_LEFTTOOLBAR->Visible=false;
		mGrid_knihovna->SetVisibleComponents(false);
	}
	if(MOD!=TVORBA_CESTY && MOD!=SIMULACE && !scSplitView_LEFTTOOLBAR->Visible)
	{
		scSplitView_LEFTTOOLBAR->Visible=true;
		//musí dojít ke smazání a znova vytvoření
		mGrid_knihovna->Delete();mGrid_knihovna=NULL;
		vytvoreni_tab_knihovna();
	}

	////////jednolivé VRSTVY
	if(Akce!=PAN_MOVE && MOD!=SIMULACE) //MOD!=SIMULACE - provizorně, smazat
	{
		Graphics::TBitmap *bmp_total=new Graphics::TBitmap;bmp_total->Width=ClientWidth;bmp_total->Height=ClientHeight;//vytvoření finální bmp, která bude vykreslena do Canvasu formu
		//bmp_total->Canvas->Brush->Color=RGB(50,50,50);bmp_total->Canvas->Rectangle(0,0,ClientWidth,ClientHeight);
		////rastrový uživatelský POKDKLAD
		if(d.v.PP.raster.show && MOD!=SIMULACE)nacti_podklad(bmp_total->Canvas);
		////vykreslení GRIDu
		if(grid && Zoom_predchozi_AA>0.5 && (Akce==MOVE_BOD||Akce==DRAW_HALA) && prichytavat_k_mrizce==1 && MOD!=SIMULACE)d.vykresli_grid(bmp_total->Canvas,size_grid);//pokud je velké přiblížení tak nevykreslí//vykreslení gridu
		////VEKTORY
		if((d.SCENA==1111111 || d.SCENA==111111) && MOD!=TVORBA_CESTY)//vše STATICKÁ scéna - totální statická scena a nejsou žádně akce nebo případně, že je v totálně statické označen (vybrán) nějaká objekt resp. hrana tohoto objektu
		{
			bmp_total->Canvas->Draw(0,0,Staticka_scena);//varianta, kdy je už přeantialiasingovaná
			if(pom!=NULL && (Akce!=EDITACE_TEXTU || (Akce==EDITACE_TEXTU && index_kurzoru!=-8)) && Akce!=POSUN_TEPLOMER)d.vykresli_objekt(bmp_total->Canvas,pom);//případne, že je v totálně statické označen (vybrán) nějaká objekt resp. hrana tohoto objektu, ale je zde bez AA, také slouží k vykreslení aktuálně editovaných položek objektu (editace názvu objektu)
		}
		else
		{
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;
			if(d.SCENA>0 && d.SCENA!=2222222)bmp_in->Canvas->Draw(0,0,Staticka_scena);//STATICKÁ scéna, je volaná pouze pokud to má smysl, není přeantialiasingovaná(AA), je jen připravená (3x větší) pro AA (aby byl již dříve AAnemá to smysl, pouze je 3x větší BMP, ale jinak by se nejednalo o úsporu)
			if(antialiasing) Zoom_predchozi_AA=Zoom;Zoom*=3;//záloha původního zoomu,nový *3 vyplývá z logiky algoritmu antialiasingu
			short s=2;if(d.SCENA==0)s=0;//řešení pro vykreslit VŠE
			d.vykresli_vektory(bmp_in->Canvas,s);//DYNAMICKÁ scéna, pokud není vše do statické nebo je aktivní pom objekt (např. výběr hrany atp.), tak se řešeí dynamická scena, jinak ne, protože nemá smysl
			if(MOD==TVORBA_CESTY)d.kurzor_cesta(bmp_in->Canvas);
			if(antialiasing)
			{
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Cantialising a;
				Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in,true);delete(bmp_in);//velice nutné do samostatné bmp_out, kvůli smazání bitmapy vracené AA
				bmp_total->Canvas->Draw(0,0,bmp_out);delete(bmp_out);
			}
			else {bmp_total->Canvas->Draw(0,0,bmp_in);delete(bmp_in);}//není aktivní AA
		}
		//následující položky není třeba antialiasingovat, buď jsou (příklad mgridů, nebo to není vyžadovánováno z vizuálního hlediska)
		////mGRIDY
		if(MOD!=SIMULACE)d.vykresli_mGridy(bmp_total->Canvas);//přesunuto do vnitř metody: OBJEKT_akt->elementy!=NULL kvůli pohonům
		////časové osy pomocné grafické metody mimo AA
		if(d.v.SIM!=NULL)d.vykresli_svislici_na_casove_osy(bmp_total->Canvas);
		////grafické MĚŘÍTKO
		//již nepoužíváno if(MOD!=SIMULACE && zobrazit_meritko && Akce!=MOVE_HALA && MOD!=TVORBA_CESTY)d.meritko(bmp_total->Canvas);
		//editované položky (editace textu)
		d.vykresli_editovane_polozky(bmp_total->Canvas);//vykresli nad statickou scénu aktuálně editované položky, používá se při editaci textu, ale je zde bez AA
		////vykreslení magnetického lasa
		if(Akce==MAGNETICKE_LASO)d.vykresli_meridlo(bmp_total->Canvas);
		////smart kurzor pro geometrii
		if(Akce==GEOMETRIE && Akce_temp==NIC)d.smart_kurzor(bmp_total->Canvas,posledni_editovany_element,typElementu);//0,1,2
		////FINALNÍ vykreslení bmp_total do Canvasu
		Canvas->Draw(0,0,bmp_total);//finální předání bmp_out do Canvasu
		delete (bmp_total);//velice nutné
		////TIP
		if(MOD!=SIMULACE)d.vykresli_tip(Canvas);
	}

	////////vykreslování tabulky pro přidávání objektů, temp řešení
	if(mGrid_knihovna!=NULL && OBJEKT_akt==NULL && MOD!=TVORBA_CESTY && MOD!=SIMULACE)
	{
		mGrid_knihovna->Redraw=true;
		mGrid_knihovna->buffer=true;//změna filozofie zajistí průběžné buffrování při vykreslování jinak mGrid_knihovna->Buffer(false);
		if(mGrid_knihovna->VisibleComponents>-1)mGrid_knihovna->VisibleComponents=true;//stačí volat toto, protože se pomocí Show (resp. Draw-SetCompontens-Set...) cyklem všechny komponenty na základě tohoto zobrazí pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
		mGrid_knihovna->Show(Image_knihovna_objektu->Canvas);
	}

	//test výpisu aktuálního nastavení scén
//	short p=0,a=0;if(pom!=NULL)p=1;if(OBJEKT_akt!=NULL)a=10;
//	Memo(String(p+a)+" "+String(d.SCENA),true);
}
//---------------------------------------------------------------------------
//vytvoří BMP se statickou scénou, NEVYTVÁŘÍ POUZE pro kompletní dynamickou scénu
void TForm1::vytvor_statickou_scenu()
{
	log(__func__,String(d.SCENA));//logování
	if(Staticka_scena!=NULL){delete Staticka_scena;Staticka_scena=NULL;}//před vytvořením nové kvůli realokaci nejdříve nutné odstranit

	if(antialiasing)
	{
		Zoom_predchozi_AA=Zoom;//záloha původního zoomu
		Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
		if((d.SCENA==1111111 || d.SCENA==111111) && MOD!=TVORBA_CESTY /* toto tu bylo proč? && Akce!=MAGNETICKE_LASO*/)//vše STATICKÁ scéna, nejsou žádně akce, pokud se jedná o kompletně statickou scenu, vytvoří již bmp Staticka_scena rovnou přeantialiasingovanoou
		{
			Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
			bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
			d.vykresli_vektory(bmp_in->Canvas,1);
			Cantialising a;Staticka_scena=a.antialiasing(bmp_in,true);delete(bmp_in);//velice nutné do samostatné bmp_out, kvůli smazání bitmapy vracené AA
			//Staticka_scena->SaveToFile("staticka_scena_sAA.bmp");
		}
		else
		if(d.SCENA>0 && d.SCENA!=2222222)//pro případy kdy se nejedná o kompletní statickou a ani kompletní dynamickou scenu, v tomto případě nemá smysl nyní Statickou scenu antialiasingovat
		{
			Staticka_scena=new Graphics::TBitmap;
			Staticka_scena->Width=ClientWidth*3;Staticka_scena->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu, nicméně nemá smysl nyní Statickou scenu antialiasingovat, protože by se stejně antialiasingovala samostatná dynamická scéna a navíc to způsobovalo zde umístěné grafické chyby
			d.vykresli_vektory(Staticka_scena->Canvas,1);
			//Staticka_scena->SaveToFile("staticka_scena_sAA.bmp");
		}
		Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
	}
	else//není povolen AA
	{
		Staticka_scena=new Graphics::TBitmap;
		Staticka_scena->Width=ClientWidth;Staticka_scena->Height=ClientHeight;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
		Staticka_scena->Width=ClientWidth;Staticka_scena->Height=ClientHeight;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
		d.vykresli_vektory(Staticka_scena->Canvas,1);
	}

//  log(__func__,"vlakno_pan_create");//logování
//	Memo((short)vlakno_PanCreateState);
//	if(vlakno_PanCreateState==false)//vykonná se pouze pokud zrovna ještě neběží
//	{
//		vlakno_PanCreateState=true;
//		vlakno_PanCreate=new vlakno_panCreate(true);//vytvoří vlákno
//		vlakno_PanCreate->FreeOnTerminate=true;//po skončení bude uvolněno
//		vlakno_PanCreate->Start();
//	}
}
//---------------------------------------------------------------------------
//přetížená výše uvedené přímo s parametrem nastavení scény
void TForm1::vytvor_statickou_scenu(long SCENA)
{
	d.SCENA=SCENA;
	vytvor_statickou_scenu();
}
//---------------------------------------------------------------------------
void TForm1::nacti_podklad(TCanvas *Canv)
{
	log(__func__);//logování
	//načtení rastru
	if(d.v.PP.raster.filename!="" &&  d.v.PP.raster.show)
	{
		if(FileExists(d.v.PP.raster.filename))
		{
			if(d.v.PP.raster.resolution==0)d.v.PP.raster.resolution=m2px;//v případě nového vstupu bez zadaného rozlišení defaultně je m2px
			Graphics::TBitmap *bmp=new Graphics::TBitmap;
			bmp->LoadFromFile(d.v.PP.raster.filename);
			//bílé smazání pozadí nutné v případě AA vektorů a podním načítaným rastrem pokud není voláno součástí bmp_out,pokud není voláno součástí bmp_out
//			Canv->Pen->Color=clWhite;Canvas->Brush->Color=clWhite;
//			Canv->Rectangle(scSplitView_LEFTTOOLBAR->Width,scGPPanel_mainmenu->Height,Width,Height);
			//vykreslení strečovaného rastru dle zadaného rozlišení
			Canv->StretchDraw(TRect(m.L2Px(d.v.PP.raster.X),m.L2Py(d.v.PP.raster.Y),m.round(m.L2Px(d.v.PP.raster.X)+bmp->Width*Zoom*d.v.PP.raster.resolution/m2px),m.round(m.L2Py(d.v.PP.raster.Y)+bmp->Height*Zoom*d.v.PP.raster.resolution/m2px)),bmp);
			delete(bmp);
		}
		else
		{
			d.v.PP.raster.show=false;
		}
	}
}
//---------------------------------------------------------------------------
void TForm1::REFRESH()
{
	log(__func__);//logování
	//d.SCENA=0;
	FormPaint(this);
}
//---------------------------------------------------------------------------
void TForm1::REFRESH(bool refreshovat_mGridy)
{
	log(AnsiString(__func__)+"pozn. parametr refreshovat_mGridy"); //logování
	refresh_mGrid=refreshovat_mGridy;
	REFRESH();
}
//---------------------------------------------------------------------------
void TForm1::REFRESH(long ZprVozEledElesDopObjHal,bool refreshovat_mGridy)
{
	log(AnsiString(__func__)+"pozn. parametry scena a refreshovat_mGridy"); //logování
	//d.SCENA=22111;
	//Memo("__________________");
	//Memo(scena);
	bool zmena=false;if(d.SCENA!=ZprVozEledElesDopObjHal)zmena=true;
	d.SCENA=ZprVozEledElesDopObjHal;
	if(zmena){/*Memo("vytvarim_statickou_scenu");*/vytvor_statickou_scenu();}//načtení statických záležitostí do statické scény, pokud nastala změna scény, musí být před REFRESH, d.SCENA>0 je ošetřeno ve vnitř metody
	if(m.getValueFromPosition(d.SCENA,4)>0 || d.SCENA==0)refresh_mGrid=refreshovat_mGridy;///else if(OBJEKT_akt!=NULL && zmena){OBJEKT_akt->zobrazit_mGrid=false;d.vykresli_objekty(Canvas);} !!! mGridy se vykreslují jako samostatná vrstva, je tedy nutné ještě vymyslet jinak
	FormPaint(this);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	log(__func__);//logování
	funkcni_klavesa=0;
	int HG=0;
	int PXM=50;int D=Form1->m.round(d.v.PP.delka_jig*PXM);int S=Form1->m.round(d.v.PP.sirka_jig*PXM);short Yofset=D;if(S>D)Yofset=S;//pro posun obrazu v technologických procesech
	switch(Key)
	{
		//BACKSPACE
		case 8:
		{
			////Cesta
			if(MOD==TVORBA_CESTY)N21Click(this);//odstranění úseku
			////Hala
			if(Akce==DRAW_HALA&&d.v.HALA.body->predchozi->n!=0){d.v.smaz_bod(d.v.HALA.body->predchozi);REFRESH();vytvor_obraz();}
			else if(Akce==DRAW_HALA){Akce=NIC;kurzor(standard);}
			////Geometrie
			smaz_usek_geometrie();
		}break;
		//ENTER
		case 13:
		{
			////Geometrie
			if(Akce==GEOMETRIE && Akce_temp==NIC)ukonceni_geometrie();
			////Potvrzení tvorby cesty
			if(MOD==TVORBA_CESTY && scGPButton_ulozit->Enabled)scGPButton_OKClick(this);
			////Hala
			if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();
			if(Akce==DRAW_HALA&&d.v.HALA.body!=NULL&&d.v.HALA.body->predchozi->n>2){d.v.vloz_bod(d.v.HALA.body->dalsi->X,d.v.HALA.body->dalsi->Y,pom,NULL,ortogonalizace_stav,true);Akce=NIC;kurzor(standard);TIP="";REFRESH();}
			else if(Akce==DRAW_HALA){d.v.vymaz_body();Akce=NIC;kurzor(standard);TIP="";REFRESH();}
		}break;
		//ESC
		case 27:
		{
			//ukončení editace textu, metoda ESC () je volána i po kliku např. do knihovny elementů, po tomto kliku má dojit k uložení editace, zahození editace pouze po stisku klávesy ESC, proto přesunuto sem
			bool blok=false;
			bool editace_textu=false;
			if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)editace_textu=true;
			double Z=0;
			if(editace_textu)
			{
    		switch (index_kurzoru)
    		{
					case 1:pom_element_temp->name=nazev_puvodni;break;
    			case -2:
					{
    				Cvektory::TBod *A=pom_bod_temp->predchozi;
						if(pom_bod_temp->n==1&&pom==NULL)A=d.v.HALA.body->predchozi;
    				if(pom_bod_temp->n==1&&pom!=NULL&&pom->body!=NULL)A=d.v.HALA.body->predchozi;
    				editovany_text=outDK(m.round2double(m.delka(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),3));//převod na mm
    				A=NULL;delete A;
					}break;
					case -6:OBJEKT_akt->name=nazev_puvodni;break;
					case -8:pom_element_temp->name=nazev_puvodni;break;//uprava popisku teploměrů
					case -5:if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(OBJEKT_akt->body->predchozi->X,OBJEKT_akt->body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);if(DKunit==2||DKunit==3)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));break;
					case -13:editovany_text="";if(pom_element_temp->predchozi!=NULL)Z=pom_element_temp->predchozi->Z;if(Z+pom_element_temp->geo.HeightDepp==0)editovany_text="±";if(Z+pom_element_temp->geo.HeightDepp>0)editovany_text="+";editovany_text+=m.round2double((Z+pom_element_temp->geo.HeightDepp)*1000,0);break;//editace konce S/K elementu
					case -14:editovany_text="";if(pom_element_temp->predchozi!=NULL)Z=pom_element_temp->predchozi->Z;if(Z==0)editovany_text="±";if(Z>0)editovany_text="+";editovany_text+=m.round2double(Z*1000,0);//editace začátku S/K elementu
					default:break;
				}
				if(index_kurzoru==-9||index_kurzoru==-8)//editace kót kabiny
    		{
    			if(DKunit==2||DKunit==3)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;//pokud jsou kóty v časovém režimu převede vzdálenost na čas
    			editovany_text=outDK(ms.MyToDouble(editovany_text));//převede na aktuálně používané jednotky
				}
				if((index_kurzoru==-11 || index_kurzoru==-101) && (OBJEKT_akt->id!=3 || (OBJEKT_akt->id==3 && (Akce==GEOMETRIE || Akce==GEOMETRIE_LIGHT))))//editace kót elementů
    		{
					if(index_kurzoru!=-101)
					{
						editovany_text=pom_element_temp->geo.delka;//načtení vzdálenosti
						if(pom_element_temp->geo.HeightDepp!=0)editovany_text=pom_element_temp->geo.delkaPud;//načtení vzdálenosti
					}
					else editovany_text=vzdalenost_meziLO(pom_element_temp,OBJEKT_akt->orientace);
					if(Akce==GEOMETRIE || Akce==GEOMETRIE_LIGHT){editovany_text=pom_element_temp->geo.delka;if(pom_element_temp->geo.HeightDepp!=0)editovany_text=pom_element_temp->geo.delkaPud;}
    			if(DKunit==2||DKunit==3)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;//pokud jsou kóty v časovém režimu převede vzdálenost na čas
					editovany_text=outDK(ms.MyToDouble(editovany_text));//převede na aktuálně používané jednotky
				}
				if(index_kurzoru==-11 && OBJEKT_akt->id==3 && pom_komora_temp!=NULL)editovany_text=outDK(pom_komora_temp->velikost);
    		smaz_kurzor();//uložení změn editace a odstranění kurzoru
				if(scGPImage_zamek_posunu->ImageIndex!=28)scGPImage_zamek_posunu->ImageIndex=28;//zamčen posun
				blok=true;
			}
			//volání metody ESC();
			else if(Akce==NIC && OBJEKT_akt!=NULL && !blok)KonecClick(this);else ESC();
		}
		break;
		//MEZERNÍK
		case 32: if(Akce!=PAN_MOVE && Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU){Akce=PAN;kurzor(pan);pan_non_locked=true;}break;
		//DELETE
		case 46:
		{
			////Geometrie
			smaz_usek_geometrie();
		}break;
		//PAGE UP
		case 33:
			if(MOD==CASOVAOSA && d.v.VOZIKY->predchozi!=NULL && d.PosunT.y>=(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height-HG)/(float)d.KrokY*d.KrokY)
			{
				d.PosunT.y-=(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height-HG)/(float)d.KrokY*d.KrokY;
				//unsigned int V=ceil((d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0));//zjistí aktuální číslo vozíku; pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
				//Cvektory::TVozik *Vozik=d.v.vrat_vozik(V);
				//odstaveno kvůli zDM if(Vozik==NULL)d.PosunT.x=0;else d.PosunT.x=Vozik->start;
				Invalidate();
			}
			break;
		//PAGE DOWN
		case 34:
			if(MOD==CASOVAOSA && d.v.VOZIKY->predchozi!=NULL && d.PosunT.y<(d.v.VOZIKY->predchozi->n+1)*d.KrokY-ClientHeight+scGPPanel_statusbar->Height+scLabel_titulek->Height+HG)
			{
				d.PosunT.y+=(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height-HG)/(float)d.KrokY*d.KrokY;
				//unsigned int V=ceil((d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0));//zjistí aktuální číslo vozíku; pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
				//odstaveno kvůli zDM d.PosunT.x=d.v.vrat_vozik(V)->start;
				Invalidate();
			}
			break;
		//END
		case 35:
			if(MOD==CASOVAOSA && d.v.VOZIKY->predchozi!=NULL)//na časové ose na poslední vozík
			{
				//odstaveno kvůli zDM d.PosunT.x=d.v.VOZIKY->predchozi->pozice-ClientWidth+Canvas->TextWidth(d.v.VOZIKY->predchozi->pozice*d.PX2MIN)/2+2;
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
		//CTRL+d
		//case 68: if(ssCtrl)ShowMessage("ctrl + d");break;
		//CTRL+m
		case 77: if(Shift.Contains(ssCtrl) && (Akce==NIC || Akce==MAGNETICKE_LASO))scGPImage_mereni_vzdalenostClick(this);break;
		//CTRL+u
		//case 85: if(ssCtrl)ShowMessage("ctrl + u");break;
		//CTRL+s
		case 83:if(Shift.Contains(ssCtrl) && Toolbar_Ulozit->Enabled)UlozitClick(this);break;
		//+
		case 107:if(Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU)ZOOM_IN();break;
		//-
		case 109:if(Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU)ZOOM_OUT();break;
		//F1 - volání nápovědy
		case 112:break;
		//F2
		case 113:if(DEBUG && Memo_testy->Visible){Memo_testy->SelectAll();Memo_testy->CopyToClipboard();Memo_testy->Clear();}break;
		//F3 - pohled celé schéma
		case 114:RzToolButton11Click(Sender);break;
		//F4
		case 115:if(!Shift.Contains(ssAlt))Vybratoknem1Click(Sender);break;//pokud se nejedná o ALT+F4
		//F5
		case 116:Invalidate();break;//Posouvat1Click(Sender);break;
		//F6
		case 117:Posunout2Click(Sender);break;
		//F7
		case 118:ZOOM_IN();break;
		//F8
		case 119:ZOOM_OUT();break;
		//F9
		case 120:if(DEBUG)
		{
			ButtonMaVl->Visible!=ButtonMaVl->Visible;ButtonMaKr->Visible=!ButtonMaKr->Visible;Invalidate();
			ButtonMaVl->Left=scSplitView_LEFTTOOLBAR->Width;ButtonMaKr->Left=ButtonMaVl->Left;
			ButtonMaVl->Top=scGPPanel_mainmenu->Height;ButtonMaKr->Top=ButtonMaVl->Top;
			if(ButtonMaKr->Visible)ButtonMaKr->SetFocus();
		}break;
		//F10
		case 121:Invalidate();  if(DEBUG) if(ButtonRosta->Visible)ButtonRosta->SetFocus(); break;
		//F11
		case 122:ortogonalizace_on_off();break;//přepíná stav automatické ortogonalizace
		//F12
		case 123:
		{
			ortogonalizovat();
			SB(ls->Strings[383]);
			REFRESH();
			DuvodUlozit(true);
		}
		break;//ortogonalizuje schéma
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
	//CTRL+Y (REDO), musí být až po nastavení funkční klávesy
	if(Key==89 && funkcni_klavesa==1)scGPGlyphButton_redoClick(this);
	//CTRL+Z (UNDO), musí být až po nastavení funkční klávesy
	if(Key==90 && funkcni_klavesa==1)scGPGlyphButton_undoClick(this);
	//ctrl+c
	if(Shift.Contains(ssCtrl) && Key==67 && (Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU))
	{
		switch(index_kurzoru)
		{
			case -6:copy_to_clipboard(OBJEKT_akt->name);break;
			case  1:copy_to_clipboard(pom_element_temp->name);break;
			default:copy_to_clipboard(editovany_text);break;
		}
	}
	//ctrl+v
	if(Shift.Contains(ssCtrl) && Key==86 && (Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU))
	{
		switch(index_kurzoru)
		{
//			case -6:OBJEKT_akt->name=Clipboard()->AsText;REFRESH(false);break;
//			case  1:pom_element_temp->name=Clipboard()->AsText;REFRESH(false);break;
//			default:editovany_text=Clipboard()->AsText;break;
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
//odchytávání znaků pro editaci názvů
void __fastcall TForm1::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	log(__func__);//logování
	////////filtr kláves
	AnsiString key=Key;
	if(AnsiString(Key)==ms.get_locale_decimal()&&editovany_text.Length()==0)key="";
	if(!((Key>=L'0')&&(Key<=L'9') || String(Key)==ms.get_locale_decimal() || ((index_kurzoru==-13 || index_kurzoru==-14) && editovany_text.Length()==0 && (String(Key)=="+" || String(Key)=="-"))))
		key="";
	if(AnsiString(Key)==ms.get_locale_decimal()&&Pos(ms.get_locale_decimal(),editovany_text)>0)
		key="";
	if(Key==L'0'&&Pos(0,editovany_text)>0&&editovany_text.Length()==1)
		key="";
	////////
	//key = pouze čísla, Key = všechny znaky
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)
	{
		if(index_kurzoru==-6 && funkcni_klavesa==0)//editace nadpisu kabiny, není třeba nic provádět
		{
			//pokud je stisknut backspace
			if(Key==8)OBJEKT_akt->name=OBJEKT_akt->name.SubString(1,OBJEKT_akt->name.Length()-1);
			else OBJEKT_akt->name+=Key;
			REFRESH(false);
		}
		if(index_kurzoru==-7)//editace short nadpisu kabiny
		{
			if(Key==8)//pokud je stisknut backspace
				OBJEKT_akt->short_name=OBJEKT_akt->short_name.SubString(1,OBJEKT_akt->short_name.Length()-1);
			else if(OBJEKT_akt->short_name.Length()!=4)
				OBJEKT_akt->short_name+=Key;
			else MessageBeep(0);
			REFRESH(false);
		}
		if(index_kurzoru==-101||index_kurzoru==-14||index_kurzoru==-13||index_kurzoru==-11||index_kurzoru==-2||index_kurzoru==-5)//editace vzdálenosti LO,začatek S/K elementu,konec S/K elementu,kót elementů,kót haly nebo objektu, kót kabiny
  	{
			if(Key==8)//pokud je stisknut backspace
				editovany_text=editovany_text.SubString(1,editovany_text.Length()-1);
  		else
  		{
				editovany_text+=key;
				if(key=="")MessageBeep(0);
  		}
			duvod_validovat=1;
			REFRESH(false);
		}
		if(index_kurzoru==1)//editace názvu elementu skrze popisek elementu
		{
			//2 rozdílné přistupy, u robotu a u totočí jiné
			if(pom_element_temp->eID!=0 && pom_element_temp->eID!=5 && pom_element_temp->eID!=6)//roboti+otoče
			{
				if(Key==8)//pokud je stisknut backspace
					pom_element_temp->name=pom_element_temp->name.SubString(1,pom_element_temp->name.Length()-1);
				else
					pom_element_temp->name+=Key;//nutné Key s velkým K, toto Key neprochází numerickým filtrem
			}
			else//globálně číslované elementy (stop, otoč)
			{
				if(Key==8)//pokud je stisknut backspace
				{
					int konec=5;
					if(pom_element_temp->eID!=0 && pom_element_temp->name.SubString(1,1)!="O")konec=8;
					if((pom_element_temp->eID==0 && pom_element_temp->name.Length()>konec) || ((pom_element_temp->eID==5 || pom_element_temp->eID==6) && pom_element_temp->name.Length()>konec))//v tomto případě povolit pouze editaci čísla, názvem "Stop " needitovatelný
						pom_element_temp->name=pom_element_temp->name.SubString(1,pom_element_temp->name.Length()-1);
					else MessageBeep(0);
				}
				else
					pom_element_temp->name+=key;//key s malým k, prochází numerickým filtrem, v tomto případě žádoucí
			}
  		//propsání nového názvu do mGridu
			int prvni_sloupec=0;
			if(pom_element_temp->eID==200 || pom_element_temp->eID==300)prvni_sloupec=3;
			pom_element_temp->mGrid->Cells[prvni_sloupec][prvni_sloupec].Text="<a>"+pom_element_temp->name+"</a>";//nasazení linku
			//znovusloučení buňěk
			if(pom_element_temp->eID!=200 && pom_element_temp->eID!=300)pom_element_temp->mGrid->MergeCells(0,0,pom_element_temp->mGrid->ColCount-1,0);
			REFRESH(false);
		}
		if(index_kurzoru==-8 && pom_element_temp!=NULL)//editace popisku teploměrů
		{
			if(Key==8)//pokud je stisknut backspace
			{
				pom_element_temp->name=pom_element_temp->name.SubString(1,pom_element_temp->name.Length()-1);
			}
			else
			{
				pom_element_temp->name+=key;
				if(key=="")MessageBeep(0);
			}
			REFRESH(false);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	log(__func__);//logování
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
//explicitní klávesové zkratky - zanechat jako vzor použití!!!
void __fastcall TForm1::FormShortCut(TWMKey &Msg, bool &Handled)
{
	//log(__func__);//logování
	//if (Msg.CharCode==VK_F9)...
	//if (Msg.CharCode==VK_F11)...
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
   log(__func__);//logování
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
  log(__func__);//logování
	if(jedno_ze_tri_otoceni_koleckem_mysi==1)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
	{
			switch(funkcni_klavesa)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detekuje stisk kláves ctrl, shift, ctrl+shift
			{
//				case 0://kolečko nahoru = směr nahoru
//				{
//					DOWN();
//				}
//				break;

				case 1://kolečko nahoru + CTRL = ZOOM IN
				if(Shift.Contains(ssCtrl))//nutná pojistka
				{
					ZOOM_IN();
				}
				break;

//				case 2://kolečko nahoru + SHIFT = doleva
//				if(Shift.Contains(ssShift))//nutná pojistka
//				{
//					LEFT();
//				}
//				break;

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
  log(__func__);//logování
	if(jedno_ze_tri_otoceni_koleckem_mysi==1)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detukuje každé druhou událost FormMouseWheel
	{
		switch(funkcni_klavesa)//Velice nutná konstrukce kvůli špatně fungujicí funkci OnMouseWheel, detekuje stisk kláves ctrl, shift, ctrl+shift
		{
//			case 0://kolečko dolu = směr dolu
//			{
//				UP();
//			}
//			break;

			case 1://kolečko dolu + CTRL = ZOOM OUT      //jenom pokud je dokončeno stahování
			if(Shift.Contains(ssCtrl))//nutná pojistka
			{
			 ZOOM_OUT();
			}
			break;

//			case 2://kolečko dolu + SHIFT = doprava
//			if(Shift.Contains(ssShift))//nutná pojistka
//			{
//				RIGHT();
//			}
//			break;

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
	log(__func__);//logování
	//if(OBJEKT_akt==NULL && mGrid_knihovna!=NULL && mGrid_knihovna->CheckPTinTable(X,Y))tab_knihovna_click(X,Y);
	if(TIP!="Kliknutím na objekt v knihovně objektu, tažením a následným usazením přidáte objekt." || ls->Strings[304]!="" && TIP!=ls->Strings[304])TIP="";//odstranění zobrazeného tipu
	if(MOD==EDITACE && PmG!=NULL && PmG->CheckLink(X,Y)==TPoint(-2,-2))FormX->naplneni_dopRD();//pokud je mod náhled a bylo kliknuto na dopRD vrátí tuto rychlost do tabulky pohonu
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();
	if(MOD==EDITACE){FormX->odstranit_korelaci();FormX->check_click_Note(X,Y);}
	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
	{
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
	}
	else
	{
		if(!dblClick)//v případě, že se nejedná o double click
		{
			if(Button==mbLeft)//je stisknuto levé tlačítko myši
			{
				if(Akce==NIC)getJobID(X,Y);//zajištění aktuálních JID, nemusí být aktuální skrze zpoždění setjobid při mousemove
				stisknute_leve_tlacitko_mysi=true;
				vychozi_souradnice_kurzoru=TPoint(X,Y);//výchozí souřadnice
				//aktivuje POSUN OBJEKTU, ELEMENTU či TABULKY,pokud je kliknuto v místě objektu (v jeho vnitřku)
				if(Akce==NIC && posun_objektu==false && funkcni_klavesa==0)//pokud není aktivovaná jiná akce
				{
					if(MOD==EDITACE && OBJEKT_akt!=NULL)//TABULKA či ELEMENT
					{
            double Z=0;//pro potřeby JID -13 a -14
						if(JID==-1){Akce=PAN;pan_non_locked=true;}//pouze posun obrazu, protože v aktuálním místě pozici myši se nenachází vektor ani interaktivní text
						if(JID==0&&pom_komora==NULL&&pom_element!=NULL || JID==1){Akce=MOVE_ELEMENT;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;mazani=true;pom_element_temp=pom_element;puv_souradnice.x=pom_element->X;puv_souradnice.y=pom_element->Y;d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//ELEMENT posun
						if(1000<=JID && JID<2000 || JID>2000 || JID==100){Akce=MOVE_TABLE;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;pom_element->mGrid->Highlight;refresh_mGrid=false;d.nabuffrovat_mGridy();puv_souradnice.x=pom_element->Xt;puv_souradnice.y=pom_element->Yt;d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//TABULKA posun
						if(100<JID && JID<900){redesign_element();}//nultý sloupec tabulky, libovolný řádek, přepnutí jednotek
						if(JID==100 || JID==1000)redesign_element();//popisky jednotek v tabulce teplomerů
						//if(JID==-6) {nastav_focus();stav_kurzoru=false;Akce=EDITACE_TEXTU;index_kurzoru=-6;nazev_puvodni=OBJEKT_akt->name;TimerKurzor->Enabled=true;}//editace názvu
						if(JID==-6) {Akce=MOVE_TEXT;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;nahled_ulozit(true);d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//posun názvu
						if(JID==-10)zmenJednotekKot();//přepnutí jednotek všech kót                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    //ZprVozEledElesDopObjHal
						if((JID==-11 || JID==-101) && OBJEKT_akt->id!=3){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;if(JID!=-101){editovany_text=pom_element_temp->geo.delka;if(pom_element_temp->geo.HeightDepp!=0)editovany_text=pom_element_temp->geo.delkaPud;}else editovany_text=vzdalenost_meziLO(pom_element,OBJEKT_akt->orientace);if(DKunit==2||DKunit==3)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));puv_souradnice.x=pom_element->X;puv_souradnice.y=pom_element->Y;editovany_text=m.round2double(ms.MyToDouble(editovany_text),0);d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//editace kót elementu
						if(JID==-9&OBJEKT_akt->id==3){Akce=ROZMER_KOMORA;pom_komora_temp=pom_komora;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}
						if(JID==-11&OBJEKT_akt->id==3){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_komora_temp=pom_komora;editovany_text=m.round2double(outDK(pom_komora->velikost),0);}
						if(JID==13){Akce=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//změna offsetu kót elementů, nebo změna rozměru jednotlivých kabin
						if(JID>=6&&JID<=11)zmena_jednotek_tab_pohon();//změna jednotek v tabulce pohonů
						if(JID==-13){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;editovany_text="";if(pom_element->predchozi!=NULL)Z=pom_element->predchozi->Z;if(Z+pom_element->geo.HeightDepp==0)editovany_text="±";if(Z+pom_element->geo.HeightDepp>0)editovany_text="+";editovany_text+=m.round2double((Z+pom_element->geo.HeightDepp)*1000,0);}//editace konce S/K elementu
						if(JID==-14){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;editovany_text="";if(pom_element->predchozi!=NULL)Z=pom_element->predchozi->Z;if(Z==0)editovany_text="±";if(Z>0)editovany_text="+";editovany_text+=m.round2double(Z*1000,0);}//editace začátku S/K elementu
						//if(JID==100)vytvor_edit();//změna názvu elementu skrze mGrid .. odstaveno
						//if(JID==1){nastav_focus();stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;nazev_puvodni=pom_element_temp->name;Akce=EDITACE_TEXTU;TimerKurzor->Enabled=true;}
						if(JID==-7 || JID==-8){Akce=POSUN_TEPLOMER;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;/*bool stav=OBJEKT_akt->zobrazit_mGrid;OBJEKT_akt->zobrazit_mGrid=false;REFRESH();Akce=POSUN_TEPLOMER;d.SCENA=1111111;vytvor_statickou_scenu();OBJEKT_akt->zobrazit_mGrid=stav;*/puv_souradnice.x=pom_element->X;puv_souradnice.y=pom_element->Y;d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//kliknutí na tělo teploměru, nebo na jeho popisek
						//if(JID==-8);//popisek teploměru
						if(JID==0&&pom_komora!=NULL&&pom_element==NULL){Akce=MOVE_KOMORA;pom_komora_temp=pom_komora;}//uchopení a přesun komory, sloužící k jejímu odstranění
						//nové JID pro objekt
						if(JID==-2){Akce=MOVE_USECKA;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;nahled_ulozit(true);d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//posun úsečky
						if(JID==-3){Akce=MOVE_BOD;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;ortogonalizace_stav=false;nahled_ulozit(true);}//posun jednoho bodu
						if(JID==-4){Akce=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;nahled_ulozit(true);}//změna offsetu kóty
						if(JID==-5){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_bod_temp=pom_bod;if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(OBJEKT_akt->body->predchozi->X,OBJEKT_akt->body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);if(DKunit==2||DKunit==3)editovany_text=m.round2double(editovany_text/OBJEKT_akt->pohon->aRD,3);editovany_text=outDK(ms.MyToDouble(editovany_text));nahled_ulozit(true);d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//editace kót kabiny
						if(JID==4){Akce=MOVE_TABLE;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;refresh_mGrid=false;d.nabuffrovat_mGridy();puv_souradnice.x=OBJEKT_akt->Xp;puv_souradnice.y=OBJEKT_akt->Yp;d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}//posun tabulky pohonu
						if(JID==-102){if(d.zprava_highlight!=d.zobrazit_celou_zpravu){d.zobrazit_celou_zpravu=d.zprava_highlight;kurzor(close);}else {d.zobrazit_celou_zpravu=0;kurzor(info);}REFRESH(false);}//rozbalení nebo skrytí zpráv
						//if(JID==-201)Memo("prvni");//oblasti předávacího místa
						//if(JID==-202)Memo("druhý");//oblasti předávacího místa
					}
					else
					{
						if(MOD==LAYOUT)//OBJEKT
						{
							if(JID==3&&!d.v.PP.zamek_layoutu){Akce=MOVE;kurzor(posun_l);minule_souradnice_kurzoru=TPoint(X,Y);puv_souradnice.x=pom->element->geo.X1;puv_souradnice.y=pom->element->geo.Y1;predchozi_orientace=pom->orientace;}
							else if(JID==-1&&Akce==NIC){Akce=PAN;pan_non_locked=true;}//přímo dovolení PAN pokud se neposová objekt = Rosťova prosba
							if(JID==-102){if(d.zprava_highlight!=d.zobrazit_celou_zpravu){d.zobrazit_celou_zpravu=d.zprava_highlight;kurzor(close);}else {d.zobrazit_celou_zpravu=0;kurzor(info);}REFRESH(false);vytvor_statickou_scenu();}//rozbalení nebo skrytí zpráv
							if(JID==-2){nastav_focus();TimerKurzor->Enabled=true;Akce=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_bod_temp=pom_bod;if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(d.v.HALA.body->predchozi->X,d.v.HALA.body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);editovany_text=outDK(ms.MyToDouble(editovany_text));}//převod na mm
							if(JID==0){Akce=MOVE_BOD;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;ortogonalizace_stav=false;}//posun jednoho bodu
							if(JID==1||JID==4){Akce=MOVE_USECKA;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//posun úsečky
							if(JID==2){Akce=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//změna offsetu kót
							if(JID==-6){Akce=MOVE_TEXT;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;vytvor_statickou_scenu();}//posun názvu v layoutu                                                                                                                                           //ZprVozEledElesDopObjHal
							if(JID==5 && pom_element!=NULL && (pom_element->eID==300 || pom_element->eID==301)){Akce=MOVE_ELEMENT;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;mazani=true;pom_element_temp=pom_element;puv_souradnice.x=pom_element->X;puv_souradnice.y=pom_element->Y;d.SCENA=1122111;vytvor_statickou_scenu();REFRESH();}//ELEMENT posun
						}
						else if(MOD==TVORBA_CESTY)
						{
							if(pom_element_temp!=NULL)
							{
								Cvektory::TElement *epom=NULL;
								bool pridat=true;
								while(pom_element_temp!=NULL)
					  		{
									if(pridat)d.v.vloz_segment_cesty(Form_definice_zakazek->Z_cesta,pom_element_temp);
									if(pom_element_temp->eID==300)break;
									epom=pom_element_temp;
									pom_element_temp=pom_element_temp->dalsi;
									if(pom_element_temp!=NULL && pom_element_temp->eID==301 && pom_element_temp->predchozi2==epom)pridat=false;
									else pridat=true;
					  		}
								pom_element_temp=NULL;
								epom=NULL;delete epom;
								if(Form_definice_zakazek->Z_cesta->cesta->predchozi->Element==d.v.ELEMENTY->predchozi)scGPButton_ulozit->Enabled=true;
								else scGPButton_ulozit->Enabled=false;
                vytvor_statickou_scenu();
					  	}
							else
							{
								Akce=PAN;
								pan_non_locked=true;
							}
						}
						else {Akce=PAN;pan_non_locked=true;}//přímo dovolení PAN pokud se neposová objekt   = Rosťova prosba
					}
				}
				if(funkcni_klavesa==3 && (MOD==LAYOUT && JID==1 || MOD==EDITACE && OBJEKT_akt->id!=3 && JID==-2))vloz_bod_haly_objektu(X,Y);//ctrl+shift+klik na přímku -> přidání bodu
				if(funkcni_klavesa==1 && (MOD==LAYOUT && JID==0 || MOD==EDITACE && JID==-3))smaz_bod_haly_objektu(pom_bod);//ctrl+klik na bod -> odstranění bodu
				else if(funkcni_klavesa==1 && Akce!=GEOMETRIE || Akce==ZOOM_W_MENU)Akce=ZOOM_W;
				switch(Akce)
				{
					case PAN:
					{
						puv_souradnice.x=X;puv_souradnice.y=Y;
						kurzor(pan_move);Akce=PAN_MOVE;//přepne z PAN na PAN_MOVE
						pan_create();//vytvoří výřez pro pan_move
						break;
					}
					case ZOOM_W:
					{
						kurzor(window);
						predchozi_souradnice_kurzoru=vychozi_souradnice_kurzoru;
						break;
					}
					//case MOVE:vytvor_obraz();break;//uložení stavu před posunem //d.odznac_oznac_objekt(Canvas,pom,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y); break;
					case MEASURE:minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;break;
					case KALIBRACE:
					{
						minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;
						//+změnit kurzor
					break;
					}
					case ADJUSTACE:minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;break;
					case DRAW_HALA://kreslení haly
					{
						vloz_bod_haly_objektu(X,Y);
						pom_bod=d.v.HALA.body->predchozi;//plnění ukazatele (vložený bod) pro účely vykreslování spojnice v mousemove
						switch(pom_bod->n)
						{
							case 1:
							case 2:TIP=ls->Strings[305];break;
							case 3:TIP=ls->Strings[306];break;
						}
						if(pom_bod->n>3)TIP=ls->Strings[306];
						if(pom_bod->n>1)//vykreslovat pouze v případě jednoho a více bodů
						{
							vytvor_statickou_scenu();//aktualizuje BMP statické scény o nový objekt, musí být před REFRESH, není důvod měnit nastavení d.SCENA
							REFRESH();
						}
						zneplatnit_minulesouradnice();//pro vykreslení spojnice (mousemove)
						break;
					}
					case GEOMETRIE:
					{
						if(d.geoTemp.typ!=-1000 && Screen->Cursor==standard && funkcni_klavesa==0)vlozeni_editace_geometrie();//vložení nebo editace geometrie (obsáhlé přesunuto do metody)
						else//spouštění temp akci nad akcemi
						{
							if(funkcni_klavesa==1 && pom_element!=NULL)Smazat1Click(Sender);
							if(funkcni_klavesa==3)////////////rozpracováno
							{
								TPointD bod=bod_vlozeni_elementu();
								if(bod.x!=-1000&&bod.y!=-1000)
								{
									short rotace=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),m.L2Px(bod.x),m.L2Py(bod.y));
									Cvektory::TElement *E=d.v.vloz_element(OBJEKT_akt,MaxInt,bod.x,bod.y,rotace,pom_element_temp);//použito force řazení, tzn. nebude přiřazena geometrie a kontrolováno pořadí
									design_element(E,false);//nutné!!!
									//vložení geometrie
									if(E->predchozi->n>0)d.v.vloz_G_element(E,0,E->predchozi->geo.X4,E->predchozi->geo.Y4,0,0,0,0,E->X,E->Y,E->predchozi->geo.orientace);
									else d.v.vloz_G_element(E,0,E->dalsi->geo.X1,E->dalsi->geo.Y1,0,0,0,0,E->X,E->Y,E->dalsi->geo.orientace);
									d.v.vloz_G_element(E->dalsi,0,E->X,E->Y,0,0,0,0,E->dalsi->geo.X4,E->dalsi->geo.Y4,E->dalsi->geo.orientace);
									E=NULL;delete E;
								}
							}
							if(JID==-11 && Akce_temp!=EDITACE_TEXTU){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce_temp=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;editovany_text=pom_element->geo.delka;if(pom_element_temp->geo.HeightDepp!=0)editovany_text=pom_element_temp->geo.delkaPud;if((DKunit==2||DKunit==3)&&pom_element->pohon!=NULL)editovany_text=editovany_text/pom_element->pohon->aRD;editovany_text=outDK(m.round2double(ms.MyToDouble(editovany_text),3));vytvor_statickou_scenu();REFRESH();}//editace textu
							if(JID==13){Akce_temp=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//offset kót
						}
						pan_create();//pro případ posunu obrazu bez akce PAN
						REFRESH(false);
					}break;
					case GEOMETRIE_LIGHT:
					{
						if(JID==-11 && Akce_temp!=EDITACE_TEXTU){nastav_focus();nahled_ulozit(true);TimerKurzor->Enabled=true;Akce_temp=EDITACE_TEXTU;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;editovany_text=pom_element->geo.delka;if(pom_element_temp->geo.HeightDepp!=0)editovany_text=pom_element_temp->geo.delkaPud;if((DKunit==2||DKunit==3)&&pom_element->pohon!=NULL)editovany_text=editovany_text/pom_element->pohon->aRD;editovany_text=outDK(m.round2double(ms.MyToDouble(editovany_text),3));vytvor_statickou_scenu();REFRESH();}//editace textu
						if(JID==13){Akce_temp=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//offset kót
					}break;
					case MAGNETICKE_LASO:
					{
            //pan_create();//pro případ posunu obrazu bez akce PAN
					}break;
					case BLOK:Akce=NIC;break;//uvolnění blokace
					default: break;
				}
				DuvodUlozit(true);
			}
			//POPUP menu
			else if(Button==mbRight)//pokud není stisknuto levé tlačítko, předpokládá se volání pop-up menu
			{
				if(Akce==NIC)getJobID(X,Y);//zajištění aktuálních JID, nemusí být aktuální skrze zpoždění setjobid při mousemove
				//nastavení zobrazení popUPmenu a jeho volání včetně pozice
				onPopUP(X,Y);
			}
			else//posun obrazu stiskem kolečka
			{
				//if(PmG!=NULL && OBJEKT_akt!=NULL && OBJEKT_akt->zobrazit_mGrid)PmG->getCombo(0,0)->CloseUp(false);//v editaci je nutné zavírat combo.. nelze rozlišit zda je otevřené
				vychozi_souradnice_kurzoru=TPoint(X,Y);
				puv_souradnice.x=X;puv_souradnice.y=Y;
				pan_non_locked=true;
				kurzor(pan_move);Akce=PAN_MOVE;//přepne z PAN na PAN_MOVE
				pan_create();//vytvoří výřez pro pan_move
			}
			if(Button!=mbRight)nastav_focus();//předávání událostí na form
		}
		else
		{
			dblClick=false;
		}
	}
}
//---------------------------------------------------------------------------
//při double clicku volá přímo formulář parametry objektu
void __fastcall TForm1::FormDblClick(TObject *Sender)
{
	log(__func__);//logování
	if(MOD!=EDITACE)//v náhledu je doubleclick zcela odstaven
	{
		dblClick=true;
		long X=akt_souradnice_kurzoru_PX.x;long Y=akt_souradnice_kurzoru_PX.y;//pouze zkrácení zápisu
		switch(MOD)
		{
//			case CASOVAOSA:
//			{														 //min                      //vozik
//				proces_pom=d.v.najdi_proces((X+d.PosunT.x)/d.PX2MIN*60,ceil((Y+d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0)));//vrací nalezen proces, proces_pom se využívá ještě dále
//				if(proces_pom!=NULL && !d.mod_vytizenost_objektu)
//				{
//					if(STATUS==NAVRH)//měnit parametry na časových osách je možné pouze v návrháři/architektovi
//					{
//						//pom=proces_pom->segment_cesty->objekt;
//					}
//				}
//			}break;
//			case TECHNOPROCESY:
//			{
//				pom=d.v.vrat_objekt_z_roma(akt_souradnice_kurzoru_PX.x-d.Xofset+d.PosunT.x);
//			}
//			break;
			case SIMULACE:break;
			default://pro LAYOUT
			{
				//povoluje nastavení položek kopírování či smazání objektu
				if(Akce==DRAW_HALA)
				{
					d.v.vloz_bod(d.v.HALA.body->dalsi->X,d.v.HALA.body->dalsi->Y,pom,NULL,ortogonalizace_stav,true);
					kurzor(standard);
					TIP="";
		  		REFRESH();
				}
				else
				{
					pom=d.v.PtInObjekt();//hledání zda jsem nad polygonem objektu
					if(pom==NULL && d.v.OBJEKTY->dalsi!=NULL)//hledání zda nejsem nad nadpisem objektu
					{
						Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
						while(O!=NULL)
						{
							if(najdi_nazev_obj(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,O)){pom=O;break;}
							O=O->dalsi;
						}
						O=NULL;delete O;
					}
					//element mimo kabinu
					if((JID==5 || JID==6) && pom_element!=NULL)pom=d.v.vrat_objekt(pom_element->objekt_n);
					if(pom!=NULL && Akce==NIC /*&& !d.v.PP.zamek_layoutu*/)otevri_editaci();//otevření editace
					//if(pom!=NULL && d.v.PP.zamek_layoutu){MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,ls->Strings[423],"",MB_OK,true,false);pom=NULL;}//info o zamčeném layoutu
					if(JID==1)vloz_bod_haly_objektu(X,Y);//přidání bodu haly
				}
			}break;
		}
		if(MOD==TVORBA_CESTY && scGPButton_ulozit->Enabled)scGPButton_OKClick(this);
		if(Akce!=GEOMETRIE){Akce=NIC;Akce_temp=NIC;}//kontrola zda sem neotevřel editace a rovnou jsme nepřešel do editace geometrie výhybky
	}
	else//jsem v náhledu
	{
		if(Akce==NIC)
		{
			pom_vyhybka=d.v.PtInObjekt();//hledání zda jsem nad polygonem objektu
			if(pom_vyhybka==NULL && d.v.OBJEKTY->dalsi!=NULL)//hledání zda nejsem nad nadpisem objektu
			{
				Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
				while(O!=NULL)
				{
					if(najdi_nazev_obj(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,O)){pom_vyhybka=O;break;}
					O=O->dalsi;
				}
				O=NULL;delete O;
			}
			if(pom_vyhybka!=NULL && (OBJEKT_akt==NULL || OBJEKT_akt!=NULL && OBJEKT_akt->n!=pom_vyhybka->n))zmena_editovaneho_objektu();//otevření náhledu
			if(JID==-2 && OBJEKT_akt->id!=3)vloz_bod_haly_objektu(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);//přidání bodu objektu
			if(pom_vyhybka==NULL && OBJEKT_akt!=NULL && scGPButton_ulozit->Enabled && !FormX->existuje_validace())
			{
				KonecClick(this);
				Akce=BLOK;
			}//blokace spuštění akce pan
		}
		if(Akce==GEOMETRIE)ukonceni_geometrie();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if(X!=akt_souradnice_kurzoru_PX.X || Y!=akt_souradnice_kurzoru_PX.Y)//ošetření proti samovolným pulzům myši
	{
		vyska_menu=Mouse->CursorPos.y-Y;//uchová rozdíl myšího kurzoru a Y-pixelu v pracovní oblasti
		akt_souradnice_kurzoru_PX=TPoint(X,Y);
		akt_souradnice_kurzoru=m.P2L(akt_souradnice_kurzoru_PX);
		//Memo(X);Memo(Y);
		if(d.v.SIM!=NULL)//if(MOD==CASOVAOSA)//vykreslování posuvné (dle myši) svislice kolmé na osy procesů, slouží jakou ukázovatko času na ose
		{
	//		pocitadlo_doby_neaktivity=0;
	//		minule_souradnice_kurzoru=TPoint(X,Y);
	//		//d.vykresli_svislici_na_casove_osy(Canvas,X,Y);//nový přístup v zobrazování svislic, jen v momentu zobrazování labalu_zamerovac (bylo odkomentováno)
			unsigned int V=getV();
			if(V>0)SB(String(V)+" voz.|"+String(m.round((X+d.PosunT.x)/d.PX2SEC*3))+"[s]",6);//výpis času na ose procesů dle kurzoru
			else SB("",6);
	//		//hazí stejné souřadnice if(abs((int)minule_souradnice_kurzoru.x-(int)akt_souradnice_kurzoru_PX.x)>1 && abs((int)minule_souradnice_kurzoru.y-(int)akt_souradnice_kurzoru_PX.y)>1)//pokud je změna větší než jeden pixel, pouze ošetření proti divnému chování myši (možná mi docházela baterka, s myší jsem nehýbal, ale přesto docházele k rušení labelu resp. volání metody FormMouseMove)
	//		//Timer_neaktivity->Enabled=false;
	//
	//		Timer_neaktivity->Interval=1000;                                                                                                                                                             //R 21.1.2020 - odebrání časové osy z projektu
	//		//if(scSplitView_OPTIONS->Opened==false && scSplitView_MENU->Opened==false && PopUPmenu->Showing==false && Form_parametry_linky->Showing==false && Form_definice_zakazek->Showing==false /*&& Form_osa_info->Showing==false*/)
	//		Timer_neaktivity->Enabled=true;//spoustí pouze pokud nejsou zobrazeny formuláře z podmínky
		}
		else if(MOD==TVORBA_CESTY)
		{
			if(Akce==NIC)REFRESH();
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
				if(MOD!=EDITACE)
				{                           //oštření proti chybnému vykreslení (chybné souřadnice z tmgrid mousemove)
					if(probehl_zoom==false && akt_souradnice_kurzoru_PX.x>168)//ošetření proti nežádoucímu chování po zoomu
					{
						if(d.v.OBJEKTY->predchozi->n>=2)//pokud už existují alespoň dva prvky, jinak nemá smysl
						{
							pom_element_temp=NULL;
							Cvektory::TObjekt *p=add_objekt_za();//testuje zda se bude přidávat objekt mimo situace poslední-první objekt-prvek
							if(pom_element_temp!=NULL)//vkládání na vedlejší větev
							{
								d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
								zneplatnit_minulesouradnice();
								pom_element=pom_element_temp;
							}
							if(pom_element_temp==NULL && p!=NULL)
							{
								d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
								zneplatnit_minulesouradnice();
								pom_element=NULL;
							}
						}
						d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
						minule_souradnice_kurzoru=TPoint(X,Y);
						d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
					}
					probehl_zoom=false;
				}
				if(MOD==EDITACE)//vkládání elementů a komor
				{
					if(pom->id==3 && element_id!=200)//vykreslování ikony komory v POW objektu
					{
						d.vykresli_ikonu_komory(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"",element_id);
						minule_souradnice_kurzoru=TPoint(X,Y);
						d.vykresli_ikonu_komory(Canvas,X,Y,"",element_id);
					}
					else//vykreslování elementu + přichytávání na pohon
					{
						TPointD souradnice=bod_vlozeni_elementu();//"přilepení" kurzoru na pohon
						short rotace_symbolu=0;if(OBJEKT_akt->orientace==0 || OBJEKT_akt->orientace==180)rotace_symbolu=90;
						if(souradnice.x==-1000)souradnice=akt_souradnice_kurzoru;
						else if(pom_element_temp!=NULL)rotace_symbolu=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),m.L2Px(souradnice.x),m.L2Py(souradnice.y));//zjistění rotace symbolu
						souradnice=uprav_bod_vlozeni_elementu(souradnice,rotace_symbolu);//uprava souřadnic posun robota, bod vkládání na ramenu
						d.vykresli_element(Canvas,0,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"","",element_id,-1,Rotace_symbolu_minula);
						minule_souradnice_kurzoru=m.L2P(souradnice);
						d.vykresli_element(Canvas,0,m.L2Px(souradnice.x),m.L2Py(souradnice.y),"","",element_id,-1,rotace_symbolu);
						Rotace_symbolu_minula=rotace_symbolu;
					}
					pom_element_temp=NULL;//mazání ukazatele, slouží k uchovávání elementu na jehož geometrii budu vkládat element
				}
				break;
			}
			case VYH:
			{
				TPointD souradnice=bod_vlozeni_elementu();//"přilepení" kurzoru na pohon, naplní pom_element_temp
				short rotace_symbolu=0;if(pom!=NULL && (pom->orientace==0 || pom->orientace==180))rotace_symbolu=90;
				if(souradnice.x==-1000)souradnice=akt_souradnice_kurzoru;
				else if(pom_element_temp!=NULL)rotace_symbolu=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),m.L2Px(souradnice.x),m.L2Py(souradnice.y));//zjistění rotace symbolu
				souradnice=uprav_bod_vlozeni_elementu(souradnice,rotace_symbolu);//uprava souřadnic posun robota, bod vkládání na ramenu
				//ošetření proti vykreslení, když je kurzor mimo pracovní plochu
				if(X>scSplitView_LEFTTOOLBAR->Width && Y<scGPPanel_statusbar->Top && Y>scGPPanel_mainmenu->Height)
				{
					unsigned int velikost=m.round(2*F->Zoom);//pro vykreslení
					Canvas->Pen->Color=clBlack;
					Canvas->Pen->Mode=pmNotXor;
					Canvas->Pen->Style=psDot;
					Canvas->Pen->Width=1;
					Canvas->Brush->Color=clWhite;
					Canvas->Brush->Style=bsClear;
					Canvas->Rectangle(minule_souradnice_kurzoru.x-velikost,minule_souradnice_kurzoru.y-velikost,minule_souradnice_kurzoru.x+velikost,minule_souradnice_kurzoru.y+velikost);
					if(d.v.vyhybka_pom!=NULL && minule_souradnice_kurzoru.x>168 && minule_souradnice_kurzoru.y>34)d.line(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,m.L2Px(d.v.vyhybka_pom->geo.X4),m.L2Py(d.v.vyhybka_pom->geo.Y4));
					minule_souradnice_kurzoru=m.L2P(souradnice);
					Canvas->Rectangle(m.L2Px(souradnice.x)-velikost,m.L2Py(souradnice.y)-velikost,m.L2Px(souradnice.x)+velikost,m.L2Py(souradnice.y)+velikost);
					if(d.v.vyhybka_pom!=NULL)d.line(Canvas,m.L2Px(souradnice.x),m.L2Py(souradnice.y),m.L2Px(d.v.vyhybka_pom->geo.X4),m.L2Py(d.v.vyhybka_pom->geo.Y4));
				}
        //vykreslení už vložené výhybky
				if(d.v.vyhybka_pom!=NULL)
				{
					unsigned int velikost=m.round(2*F->Zoom);//pro vykreslení
					Canvas->Pen->Color=clBlack;
					Canvas->Pen->Mode=pmNotXor;
					Canvas->Pen->Style=psDot;
					Canvas->Pen->Width=1;
					Canvas->Brush->Color=clWhite;
					Canvas->Brush->Style=bsClear;
					Canvas->Rectangle(m.L2Px(d.v.vyhybka_pom->geo.X4)-velikost,m.L2Py(d.v.vyhybka_pom->geo.Y4)-velikost,m.L2Px(d.v.vyhybka_pom->geo.X4)+velikost,m.L2Py(d.v.vyhybka_pom->geo.Y4)+velikost);
				}
			}break;
			case MOVE://posun objektu, v editaci funkce posunu objektu odstavena
			{
				//zapnutí kurzoru
				//if(OBJEKT_akt!=NULL&&stisknute_leve_tlacitko_mysi&&Screen->Cursor!=pan_move)kurzor(pan_move);
				//posun
				//if(OBJEKT_akt!=NULL&&stisknute_leve_tlacitko_mysi)d.v.posun_objekt(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),OBJEKT_akt,false,false);
				if(OBJEKT_akt==NULL)
				{
					//if(abs(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x))>=0.001 && abs(akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y))>=0.001)
					d.v.posun_objekt(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),pom);
					minule_souradnice_kurzoru=TPoint(X,Y);
					REFRESH(2222221,false);//ZprVozEledElesDopObjHal
				}else Akce=NIC;
				break;
			}
			case DRAW_HALA:
			{
				if(d.v.HALA.body!=NULL&&d.v.HALA.body->dalsi!=NULL)
				{
					TPointD souradnice;
					//přidat
					//set_pen();
					if(minule_souradnice_kurzoru.x!=-200)
					{
						d.linie(Canvas,m.L2Px(d.v.HALA.body->predchozi->X),m.L2Py(d.v.HALA.body->predchozi->Y),minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,m.m2px(0.4),m.clIntensive(clBlue,200),psSolid,pmNotXor);
						if(d.v.HALA.body->predchozi->n>1)d.linie(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,m.L2Px(d.v.HALA.body->dalsi->X),m.L2Py(d.v.HALA.body->dalsi->Y),m.m2px(0.4),m.clIntensive(clBlue,200),psSolid,pmNotXor);
					}
					if(grid)//přichytávání k bodu
					{
						switch(prichytavat_k_mrizce)
						{
							case 1:	souradnice.x=m.round(m.P2Lx(X)/(size_grid*1.0*m2px))*size_grid*m2px;souradnice.y=m.round(m.P2Ly(Y)/(size_grid*1.0*m2px))*size_grid*m2px;break;//přichytí automaticky
							case 2: souradnice=m.P2L(TPoint(X,Y));break;//automaticky nepřichyt
						}
					}
					else souradnice=m.P2L(TPoint(X,Y));
					if(ortogonalizace_stav)//ortogonalizace
					{
						Cvektory::TBod *B=pom_bod;
						if(m.abs_d(B->X-souradnice.x)<m.abs_d(B->Y-souradnice.y))souradnice.x=B->X;//zarovnat dle X
						else souradnice.y=B->Y;//zarovnat dle Y
						B=NULL;delete B;
					}
					souradnice.x=m.L2Px(souradnice.x);
					souradnice.y=m.L2Py(souradnice.y);
					minule_souradnice_kurzoru=TPoint(souradnice.x,souradnice.y);
					d.linie(Canvas,m.L2Px(d.v.HALA.body->predchozi->X),m.L2Py(d.v.HALA.body->predchozi->Y),souradnice.x,souradnice.y,m.m2px(0.4),m.clIntensive(clBlue,200),psSolid,pmNotXor);
					if(d.v.HALA.body->predchozi->n>1)d.linie(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,m.L2Px(d.v.HALA.body->dalsi->X),m.L2Py(d.v.HALA.body->dalsi->Y),m.m2px(0.4),m.clIntensive(clBlue,200),psSolid,pmNotXor);
				}
			}break;
			case MOVE_TABLE://posun tabulky elementu
			{
				if(pom_element!=NULL)//posun tabulek elementů
				{
					pom_element->Xt+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
					pom_element->Yt+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
					pom_element->mGrid->Highlight;//pro udržení, někdy zdá se vypadává
					REFRESH(false);
					//vykreslení spojnice tabulky a elementu
					vykresli_spojinici_EmGrid(Canvas,pom_element);
				}
				else if(JID==4 && PmG!=NULL)//posun tabulky pohonu
				{
					OBJEKT_akt->Xp+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
					OBJEKT_akt->Yp+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
					REFRESH(false);
				}
				minule_souradnice_kurzoru=TPoint(X,Y);
				nahled_ulozit(true,false);
				break;
			}
			case MOVE_ELEMENT://posun elementu + příprava na kontrolu
			{
				bool nemazat=true;
				//samotný pohyb, který je vázán na pohon
				if (pom_element->geo.orientace-pom_element->geo.rotacni_uhel==90 || pom_element->geo.orientace-pom_element->geo.rotacni_uhel==270)
					nemazat=d.v.posun_element(pom_element,akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),posun_dalsich_elementu,true);
				else
					nemazat=d.v.posun_element(pom_element,akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),posun_dalsich_elementu,true);
				//aktuální souřadnice se po posunu zapíší do minule_souřadnice, z důvodu okamžité změny známénka posuvu
				minule_souradnice_kurzoru=TPoint(X,Y);
				//if(!nemazat && !posun_dalsich_elementu)Smazat1Click(this);
				if(!nemazat)TIP=ls->Strings[307];else TIP="";//"Prvek nelze přeunout!"
				REFRESH(false);
				//když nejsou viditelné tabulky elementu, a když se nejedná o element, který nemá tabulku -> nevykresli spojnici mezi elementem a tabulkou
				if(OBJEKT_akt!=NULL)
				{
					if(OBJEKT_akt->zobrazit_mGrid && pom_element->eID!=100 && pom_element->eID!=MaxInt)vykresli_spojinici_EmGrid(Canvas,pom_element);
					vykresli_spojinici_EmGrid(Canvas,pom_element);
					nahled_ulozit(true);
				}
				break;
			}
			case MOVE_KOMORA://posun komory vedoucí k jejímu odstranění
			{
				d.vykresli_ikonu_komory(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"",-1);
				minule_souradnice_kurzoru=TPoint(X,Y);
				d.vykresli_ikonu_komory(Canvas,X,Y,"",-1);
				if(OBJEKT_akt->orientace==90 && (m.P2Lx(X)<OBJEKT_akt->body->dalsi->X || m.P2Lx(X)>OBJEKT_akt->body->dalsi->dalsi->X))Smazat1Click(this);
				if(OBJEKT_akt->orientace==180 && (m.P2Ly(Y)>OBJEKT_akt->body->dalsi->Y || m.P2Ly(Y)<OBJEKT_akt->body->dalsi->dalsi->Y))Smazat1Click(this);
				if(OBJEKT_akt->orientace==270 && (m.P2Lx(X)>OBJEKT_akt->body->dalsi->X || m.P2Lx(X)<OBJEKT_akt->body->dalsi->dalsi->X))Smazat1Click(this);
				if(OBJEKT_akt->orientace==0 && (m.P2Ly(Y)<OBJEKT_akt->body->dalsi->Y || m.P2Ly(Y)>OBJEKT_akt->body->dalsi->dalsi->Y))Smazat1Click(this);
				nahled_ulozit(true);
				break;
			}
			case MOVE_BOD://posun bodu pokud je zapnuté přichytávání k mřížce posun bodu po bodech mřížky
			{
				TPointD souradnice;
				if(grid)//pokud je zobrazena mřížka
				{
					int vel_gridu=size_grid;//nesmí se pracovat se size_grid ->ovlivnilo by to vykreslování
					if(OBJEKT_akt!=NULL)vel_gridu/=10;//pokud jsem v editaci, grid je jemější
					switch(prichytavat_k_mrizce)
					{
						case 1:	souradnice.x=m.round(m.P2Lx(X)/(vel_gridu*1.0*m2px))*vel_gridu*m2px;souradnice.y=m.round(m.P2Ly(Y)/(vel_gridu*1.0*m2px))*vel_gridu*m2px;break;//přichytí automaticky
						case 2: souradnice=m.P2L(TPoint(X,Y));break;//automaticky nepřichyt
					}
				}
				else souradnice=m.P2L(TPoint(X,Y));
				d.v.posun_bod(souradnice.x,souradnice.y,pom_bod);
				minule_souradnice_kurzoru=TPoint(souradnice.x,souradnice.y);
				REFRESH();
				break;
			}
			case MOVE_USECKA:
			{
				Cvektory::TBod *A,*B;//body úsečky
				if(pom_bod->n==1)//poslední úsečka
				{
					if(OBJEKT_akt!=NULL){A=OBJEKT_akt->body->predchozi;B=OBJEKT_akt->body->dalsi;}//posun úsečky objektu v náhledu
					else if(pom!=NULL){A=pom->body->predchozi;B=pom->body->dalsi;}//posun úsečky objektu ve schématu
					else {A=d.v.HALA.body->predchozi;B=d.v.HALA.body->dalsi;}//posun úsečky haly
				}
				else {A=pom_bod->predchozi;B=pom_bod;}//ostatní úsečky
				if(m.round2double(A->X,2)==m.round2double(B->X,2))//posun po x
					d.v.posun_hranu(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),0,A,B);
				else if(m.round2double(A->Y,2)==m.round2double(B->Y,2))//posun po y
					d.v.posun_hranu(0,akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),A,B);
				else//skosená úsečka
				{
					double posunx,posuny,vysledek;
					posunx=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);//posun po x
					posuny=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);//posun po y
					if(m.abs_d(posunx)>m.abs_d(posuny))posuny=0;else posunx=0;//vylepšení "pocitu" z posunu, není nezbytně nutné
					d.v.posun_hranu(posunx,posuny,A,B);
				}
				//rozšiřování el. osy + komor
				if((pom!=NULL && OBJEKT_akt==NULL && pom->body->predchozi->n==4 && (pom->element->dalsi==NULL || pom->element->dalsi!=NULL && pom->element->dalsi->objekt_n!=pom->n) && pom->element->geo.typ==0) || (OBJEKT_akt!=NULL && OBJEKT_akt->body->predchozi->n==4 && (OBJEKT_akt->element->dalsi==NULL || (OBJEKT_akt->element->dalsi!=NULL && OBJEKT_akt->element->dalsi->objekt_n!=OBJEKT_akt->n)) && OBJEKT_akt->element->geo.typ==0))//speciální uprava objektu POW při posunu jeho přímek, z layoutu + z editace
				{
					//bool zmena_pohonu=false;
					Cvektory::TObjekt *O=pom;
					if(OBJEKT_akt!=NULL)O=OBJEKT_akt;
					//////uchopeno za boky .. budu měnit délku pohonu a komor
					if(A->n==2 || A->n==4)
					{
						double posun_x=0,posun_y=0,pom1=1,pom2=1,zmena=0,zbytek=0;
						//vypočet změny
						if(B->n==1 && A->n==4){pom1=-1;pom2=-1;}
						if(O->orientace==0)posun_y=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
						if(O->orientace==90)posun_x=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
						if(O->orientace==180){posun_y=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);pom2*=-1;}
						if(O->orientace==270){posun_x=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);pom1*=-1;}
						zmena=pom1*posun_x+pom2*posun_y;
						//pokud změnu provádím z editace měním rozměry poslendí nebo první komory
						if(O->id==3)
						{
							if(A->n==2)
							{
								O->komora->predchozi->velikost+=zmena;
								while(O->komora->predchozi->velikost>O->komora->dalsi->velikost*1.10)//10% na víc
								{
									d.v.vloz_komoru(O,O->komora->dalsi->velikost,NULL/*O->komora->predchozi*/,abs(O->komora->predchozi->typ-1));
									zbytek=O->komora->predchozi->predchozi->velikost-O->komora->dalsi->velikost;
									O->komora->predchozi->predchozi->velikost=O->komora->dalsi->velikost;
									O->komora->predchozi->velikost=O->komora->predchozi->velikost-O->komora->dalsi->velikost+zbytek;
								}
								while(O->komora->predchozi->velikost<0)d.v.smaz_komoru(O,O->komora->predchozi);
							}
							if(A->n==4)
							{
								O->komora->dalsi->velikost+=zmena;
								while(O->komora->dalsi->velikost>O->komora->predchozi->velikost*1.10)//10% na víc
								{
									d.v.vloz_komoru(O,O->komora->predchozi->velikost,O->komora->dalsi,abs(O->komora->dalsi->dalsi->typ-1));
									zbytek=O->komora->dalsi->dalsi->velikost-O->komora->predchozi->velikost;
									O->komora->dalsi->dalsi->velikost=O->komora->predchozi->velikost;
									O->komora->dalsi->velikost=O->komora->dalsi->velikost-O->komora->predchozi->velikost+zbytek;
								}
								while(O->komora->dalsi->velikost<0)d.v.smaz_komoru(O,O->komora->dalsi);
							}
						}
						//rozšíření pohonu
						if(O->element->pohon==NULL)
						{
							if(B->n==1 && A->n==4)
							{
								O->element->geo.X1+=posun_x;
								O->element->geo.Y1+=posun_y;
							}
							else
							{
								O->element->X+=posun_x;
								O->element->Y+=posun_y;
							}
							d.v.vloz_G_element(O->element,0,O->element->geo.X1,O->element->geo.Y1,0,0,0,0,O->element->X,O->element->Y,O->element->geo.orientace);
						}
						//zmena_pohonu=true;
					}
					//////uchopeno za vrh/spodek .. budu posouvat osu pohonu
					else
					{
	//					double delka_hrany=abs(O->body->dalsi->X-O->body->predchozi->X+O->body->dalsi->Y-O->body->predchozi->Y),posun=0;
	//					if(O->orientace==90 || O->orientace==270)
	//					{
	//						if(O->orientace==90)posun=O->body->dalsi->Y-delka_hrany/2.0;
	//						else posun=O->body->predchozi->Y-delka_hrany/2.0;
	//						O->element->Y=O->element->geo.Y1=O->element->geo.Y2=O->element->geo.Y3=O->element->geo.Y4=posun;
	//					}
	//					else
	//					{
	//						if(O->orientace==0)posun=O->body->dalsi->X+delka_hrany/2.0;
	//						else posun=O->body->predchozi->X+delka_hrany/2.0;
	//						O->element->X=O->element->geo.X1=O->element->geo.X2=O->element->geo.X3=O->element->geo.X4=posun;
	//          }
					}
					O=NULL;delete O;
				}
				minule_souradnice_kurzoru=TPoint(X,Y);
	//			if(MOD==LAYOUT)//čekání na funkčnost pom v statické scéně
	//			{
	//				//ZprVozEledElesDopObjHal
	//				long scena=1111121;//pouze změna objektu
	//				if(zmena_pohonu)scena=1111221;//změna objektu a pohonu
	//				if(pom==NULL)scena=1111112;//změna haly
	//				REFRESH(scena,false);
	//			}
	//			else
				REFRESH();
				A=NULL;B=NULL;delete A;delete B;
				break;
			}
			case MOVE_HALA:
			{
				if(stisknute_leve_tlacitko_mysi)//pokud je stisknuto levé tlačítko
				{
					if(Screen->Cursor!=pan_move)kurzor(pan_move);//změna kurzoru pouze v případě prvního posunu po kliku
					//posun haly nebo kabiny objektu z layoutu
					if(OBJEKT_akt==NULL)d.v.posun_body(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),pom);
					//posun obrysu v editaci
					else d.v.posun_body(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),OBJEKT_akt);
					if(TIP!="")TIP="";
					refresh_mGrid=false;
					REFRESH(1111112,false);
					refresh_mGrid=true;
				}
				minule_souradnice_kurzoru=TPoint(X,Y);//slouží i k určení výchozího bodu pro posun
				break;
			}
			case MOVE_TEXT:
			{
				if(OBJEKT_akt!=NULL)
				{
					OBJEKT_akt->Xt+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
					OBJEKT_akt->Yt+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				}
				else
				{
					pom->Xt+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
					pom->Yt+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				}
				minule_souradnice_kurzoru=TPoint(X,Y);
				REFRESH();
				break;
			}
			case ROZMER_KOMORA://změna rozmerů komor pomocí úchopu
			{
				Cvektory::TKomora *ovlivneny=pom_komora_temp->dalsi;
				if(ovlivneny!=NULL)//ošetření, může se stát že se spustí tato událost místo MOVE_USECKA - poslední komora nemá nasledující
				{
					if((OBJEKT_akt->orientace==90||OBJEKT_akt->orientace==270)&&ovlivneny->velikost>=0.5&&pom_komora_temp->velikost>=0.5)
					{
						double posun=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
						if(OBJEKT_akt->orientace==270)posun*=-1;
						pom_komora_temp->velikost+=posun;
						ovlivneny->velikost-=posun;
					}
					else if(ovlivneny->velikost>=0.5&&pom_komora_temp->velikost>=0.5)
					{
						double posun=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
						if(OBJEKT_akt->orientace==0)posun*=-1;
						pom_komora_temp->velikost-=posun;
						ovlivneny->velikost+=posun;
					}
					else//překročil jsem rozmery ovlivněného nebo aktuální komory
					{
						double rozdil=0;
						if(pom_komora_temp->velikost<=0.5){rozdil=0.5-pom_komora_temp->velikost;pom_komora_temp->velikost=0.5;ovlivneny->velikost-=rozdil;}
						if(ovlivneny->velikost<=0.5){rozdil=0.5-ovlivneny->velikost;ovlivneny->velikost=0.5;pom_komora_temp->velikost-=rozdil;}
						Akce=NIC;
					}
					minule_souradnice_kurzoru=TPoint(X,Y);
					refresh_mGrid=false;
					REFRESH();
					refresh_mGrid=true;
					ovlivneny=NULL;delete ovlivneny;
					nahled_ulozit(true);
				}
				break;
			}
			case OFFSET_KOTY:
			{
				if(JID==13)//offset kót elementu a komor
				{
					if(OBJEKT_akt->id==3)
					{
						if(OBJEKT_akt->orientace==90||OBJEKT_akt->orientace==270)OBJEKT_akt->koty_elementu_offset.x-=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
						else OBJEKT_akt->koty_elementu_offset.x+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
					}
					else
					{
						switch((int)pom_element->geo.orientace)
						{
							case 0:case 180:OBJEKT_akt->koty_elementu_offset.x+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);break;
							case 90:case 270:OBJEKT_akt->koty_elementu_offset.x-=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);break;
						}
					}
					minule_souradnice_kurzoru=TPoint(X,Y);
					REFRESH(false);
					nahled_ulozit(true);
				}
				else
				{
					//definice předchozího bodu
					Cvektory::TBod *Bod;
					if(pom_bod->n!=1)Bod=pom_bod->predchozi;
					else if(OBJEKT_akt==NULL)Bod=d.v.HALA.body->predchozi;
					else Bod=OBJEKT_akt->body->predchozi;
					//výpočet posunů
					double posunx,posuny,vysledek=0,x1,y1;
					posunx=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);posuny=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
					if(m.abs_d(posunx)>m.abs_d(posuny))vysledek=sin(DegToRad(fmod(m.azimut(Bod->X,Bod->Y,pom_bod->X,pom_bod->Y)+90,360)))*posunx;
					else vysledek=cos(DegToRad(fmod(m.azimut(Bod->X,Bod->Y,pom_bod->X,pom_bod->Y)+90,360)))*posuny;
					//uložení
					pom_bod->kota_offset+=vysledek*30;//musí být násobeno, offset je -70 a posun je 0.xxx
					//nutné na konec
					Bod=NULL;delete Bod;
					minule_souradnice_kurzoru=TPoint(X,Y);
					REFRESH();
				}
				break;
			}
			case MEASURE://liniové měření vzdálenosti,vykreslení provizorní měřící linie
			{
				//zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
				zobraz_tip(ls->Strings[295],Canvas);//"Tažením myši z vybraného bodu do cílového bodu zobrazíte vzdálenost mezi těmito body."
					if(stisknute_leve_tlacitko_mysi)
				{
					d.vykresli_meridlo(Canvas,X,Y);
				}
			}break;
			case KALIBRACE:
			{
				//zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
				zobraz_tip(ls->Strings[296],Canvas);//"Tažením myši z vybraného bodu na podkladu směřujte do vybraného technolog. objektu, po puštění myši dojde ke kalibraci obrazu."
				if(stisknute_leve_tlacitko_mysi)
				{
					d.vykresli_meridlo(Canvas,X,Y,true);
				}
			}break;
			case ADJUSTACE:
			{
				//zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
				zobraz_tip(ls->Strings[297],Canvas);//"Tažením myši z výchozího bodu směřujte do cílového bodu podkladu, po puštění myši zadejte skutečnou vzdálenost mezi body v metrech či milimetrech."
				if(stisknute_leve_tlacitko_mysi)
				{
					d.vykresli_meridlo(Canvas,X,Y);
				}
			}break;
			case GEOMETRIE:
			{
				log(__func__);
				if(Akce_temp==NIC)
				{
					bool puv_sek_vetev=false,sek_vetev=false;if(posledni_editovany_element!=NULL && ((posledni_editovany_element->eID==300) || (posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && posledni_editovany_element->dalsi->dalsi2!=posledni_editovany_element->dalsi->predchozi2)))puv_sek_vetev=true;
					Cvektory::TElement *e_poslendi=posledni_editovany_element;
					//pan_move při stisknutém levém tlačítku
					if(stisknute_leve_tlacitko_mysi){pan_map(Canvas,X,Y);kurzor(pan_move);}else if(Screen->Cursor==pan_move)kurzor(standard);
					//vykreslení spojnice mezi posledním editovaným elementem a dalším elementem v objektu (pokud existuje)
//					if(e_poslendi!=NULL && e_poslendi->dalsi!=NULL && e_poslendi->geo.X4!=e_poslendi->dalsi->geo.X1 && e_poslendi->geo.X4!=e_poslendi->dalsi->geo.X1)
//					{
//						//nastavení pera
//						Canvas->Pen->Color=clBlack;
//						Canvas->Pen->Width=1;
//						Canvas->Pen->Style=psDot;//nastevení čarkované čáry
//						Canvas->Pen->Mode=pmNotXor;
//						Canvas->Brush->Style=bsClear;
//						//nastavení souřadnic
//						double X=0,Y=0,X2=0,Y2=0;
//						if(d.geoTemp.typ!=-1000)
//						{
//							X=d.geoTemp.X4;
//							Y=d.geoTemp.Y4;
//						}
//						else
//						{
//							X=e_poslendi->X;
//							Y=e_poslendi->Y;
//						}
//						X=m.L2Px(X);Y=m.L2Py(Y);
//						X2=m.L2Px(e_poslendi->dalsi->geo.X1);Y2=m.L2Py(e_poslendi->dalsi->geo.Y1);
//						if(e_poslendi->dalsi->eID==301 && e_poslendi->dalsi->predchozi2==e_poslendi){X2=m.L2Px(e_poslendi->dalsi->geo.X4);Y2=m.L2Py(e_poslendi->dalsi->geo.Y4);}
//						Canvas->MoveTo(X,Y);
//						Canvas->LineTo(X2,Y2);
//					}
					//hledání elementu na kterém je kurzor
					pom_element=NULL;
					Cvektory::TElement *E=OBJEKT_akt->element;
					Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
					while(E!=NULL)
					{
						if(E->geo.delka!=0 && m.PtInCircle(E->geo.X4,E->geo.Y4,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,velikost_citelne_oblasti_elementu))
						{
							pom_element=E;
							//break;//musím najít poslední element, který souhlasí se souřadnicemi, důvod: překrytí zarážky a spojky (konec vedlejší větve)
						}
						if(E!=OBJEKT_akt->element && E->predchozi->objekt_n!=OBJEKT_akt->n && E->predchozi->geo.delka!=0 && m.PtInCircle(E->predchozi->geo.X4,E->predchozi->geo.Y4,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,velikost_citelne_oblasti_elementu))
						{
							pom_element=E->predchozi;
							//break;
						}
						if(E->eID==301 && E->predchozi2->objekt_n!=OBJEKT_akt->n && E->predchozi2->geo.delka!=0 && m.PtInCircle(E->predchozi2->geo.X4,E->predchozi2->geo.Y4,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,velikost_citelne_oblasti_elementu))
						{
							pom_element=E->predchozi2;
							//break;
						}
						E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
					}
					d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
					E=NULL;delete E;
					if((pom_element!=NULL && posledni_editovany_element!=NULL && pom_element!=posledni_editovany_element) || (pom_element!=NULL && posledni_editovany_element==NULL)){posledni_editovany_element=pom_element;editace_geometrie_spustena=true;}
					else if(OBJEKT_akt->element->geo.X1!=OBJEKT_akt->element->geo.X4 && m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,OBJEKT_akt->element->geo.X1,OBJEKT_akt->element->geo.Y1,velikost_citelne_oblasti_elementu) && posledni_editovany_element!=NULL){posledni_editovany_element=NULL;editace_geometrie_spustena=true;}
					//vypisování tipu s přichycením
					TIP="";
					if(posledni_editovany_element==NULL)TIP="Přichyceno na začátek objektu";
					else if(posledni_editovany_element->eID!=MaxInt)TIP="Přichyceno na "+posledni_editovany_element->name;
					//////setjobid, při geometrii je třeba mít zjišťování jidů kolem kót elementu (editace přímek)
					int puvJID=JID;//záloha původního JID
					//////getjobid
					JID=-1;
					if(OBJEKT_akt->zobrazit_koty)//pouze pokud je náhled povolen a jsou kóty zobrazeny
					{
						short PtInKota_elementu=d.v.PtInKota_elementu(OBJEKT_akt,X,Y);
						//jednotky kóty buď kabiny nebo kót elementů JID=-10
						if(PtInKota_elementu==3)JID=-101;//hodnota LO kóty
						else if(OBJEKT_akt->kabinaKotaX_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || OBJEKT_akt->kabinaKotaY_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || PtInKota_elementu==2)JID=-10;
						else if(OBJEKT_akt->uzamknout_nahled==false)//hledám kóty kabiny
						{
							short PtInKota_bod=d.v.PtInKota_bod(OBJEKT_akt);//metoda vrací zda jsem v oblasti kóty nebo v její hodnotě + ukládá ukazatel na bod do pom_bod
							if(PtInKota_bod==0 && pom_bod!=NULL)JID=-4;//oblast kóty - posun kóty
							else if(PtInKota_bod==1 && pom_bod!=NULL)JID=-5;//hodnota kóty
							else//kóty elementů RET=11-99
							{
								if(PtInKota_elementu==0 && pom_element!=NULL)JID=13;//oblast kóty - posun kóty
								if(PtInKota_elementu==1 && pom_element!=NULL)JID=-11;//hodnota kóty
							}
						}
					}
					//////setjobid
					if(puvJID!=JID)//pokud došlo ke změně JID, nebo změně bodu bez změny JID, jinak nemá smysl řešit
					{
						kurzor(standard);
						if(JID==-11 && Akce_temp!=EDITACE_TEXTU){kurzor(edit_text);refresh_mGrid=false;}//kurzor pro editaci textu
						if(JID==13){if(pom_element->geo.orientace==90||pom_element->geo.orientace==270)kurzor(zmena_d_y);else kurzor(zmena_d_x);refresh_mGrid=false;}//interaktivní kóty elementů
					}
					//////přepínání typu elementu pro smart_kurzor
					if(posledni_editovany_element!=NULL)
					{
						if(posledni_editovany_element->eID==300)
						{
							if((posledni_editovany_element->dalsi->geo.delka>0 && posledni_editovany_element->dalsi->geo.typ==0) || (posledni_editovany_element->dalsi->dalsi!=NULL && posledni_editovany_element->dalsi->dalsi->geo.delka==0 && posledni_editovany_element->dalsi->dalsi->geo.typ==0))
								typElementu=1;//povolit oblouky
							else typElementu=2;//povolit linie
						}else typElementu=0;//povolit vše
					}
					//při pan_move nesmí dojít k refreshi, v ostatních případech musí
					if(Screen->Cursor!=pan_move)REFRESH();
					e_poslendi=NULL;delete e_poslendi;
					if(posledni_editovany_element!=NULL && ((posledni_editovany_element->eID==300) || (posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && posledni_editovany_element->dalsi->dalsi2!=posledni_editovany_element->dalsi->predchozi2)))sek_vetev=true;
					if(sek_vetev!=puv_sek_vetev){vytvor_statickou_scenu();REFRESH();}//změna větve, proběhne změna aktivních pohonu -> nutné překreslit
				}
			}break;
			case GEOMETRIE_LIGHT://pouze editace z kót, nezobrazuje se smart kurzor
			{
				//////setjobid, při geometrii je třeba mít zjišťování jidů kolem kót elementu (editace přímek)
				int puvJID=JID;//záloha původního JID
				//////getjobid
				JID=-1;
				if(OBJEKT_akt->zobrazit_koty)//pouze pokud je náhled povolen a jsou kóty zobrazeny
				{
					short PtInKota_elementu=d.v.PtInKota_elementu(OBJEKT_akt,X,Y);
					//jednotky kóty buď kabiny nebo kót elementů JID=-10
					if(PtInKota_elementu==3)JID=-101;//hodnota LO kóty
					else if(OBJEKT_akt->kabinaKotaX_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || OBJEKT_akt->kabinaKotaY_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || PtInKota_elementu==2)JID=-10;
					else if(OBJEKT_akt->uzamknout_nahled==false)//hledám kóty kabiny
					{
						short PtInKota_bod=d.v.PtInKota_bod(OBJEKT_akt);//metoda vrací zda jsem v oblasti kóty nebo v její hodnotě + ukládá ukazatel na bod do pom_bod
						if(PtInKota_bod==0 && pom_bod!=NULL)JID=-4;//oblast kóty - posun kóty
						else if(PtInKota_bod==1 && pom_bod!=NULL)JID=-5;//hodnota kóty
						else//kóty elementů RET=11-99
						{
							if(PtInKota_elementu==0 && pom_element!=NULL)JID=13;//oblast kóty - posun kóty
							if(PtInKota_elementu==1 && pom_element!=NULL)JID=-11;//hodnota kóty
						}
					}
				}
				//////setjobid
				if(puvJID!=JID)//pokud došlo ke změně JID, nebo změně bodu bez změny JID, jinak nemá smysl řešit
				{
					kurzor(standard);
					if(JID<=-11 && Akce_temp!=EDITACE_TEXTU){kurzor(edit_text);refresh_mGrid=false;}//kurzor pro editaci textu
					if(JID==13){if(pom_element->geo.orientace==90||pom_element->geo.orientace==270)kurzor(zmena_d_y);else kurzor(zmena_d_x);refresh_mGrid=false;}//interaktivní kóty elementů
				}
			}break;
			case POSUN_TEPLOMER:
			{
				//posun prvního teploměru
				double orientace=m.Rt90(pom_element->geo.orientace-pom_element->geo.rotacni_uhel);
				TPointD korekce;
				korekce.x=-(d.v.PP.sirka_podvozek/2.0+0.4);korekce.y=0;
				if(orientace==90 || orientace==270){korekce.x=0;korekce.y=-(d.v.PP.sirka_podvozek/2.0+0.4);}
				//přichytávání
				int prichyceno=0;
				pocatek_mereni=akt_souradnice_kurzoru;//výpújčení k uložení přichcených souřadnic
				pom_element->short_name="";//identifikátor, že teploměr byl přichycen na element nebo kabinu
				pom_element_temp=d.v.ELEMENTY->dalsi;
				Cvektory::T2Element *VYH=d.v.hlavicka_seznam_VYHYBKY();
				while(pom_element_temp!=NULL)
				{
					//kontrola, zda jsem na elementu
					if(pom_element_temp->eID!=MaxInt && m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom_element_temp->geo.X4,pom_element_temp->geo.Y4,velikost_citelne_oblasti_elementu))
					{
						//přilepení na element
						TIP=ls->Strings[506]+" "+pom_element_temp->name;//"Přichyceno k"
						pom_element->short_name=pom_element_temp->name;
						prichyceno=1;
						pocatek_mereni.x=pom_element_temp->geo.X4;
						pocatek_mereni.y=pom_element_temp->geo.Y4;
						break;
					}
					//kontrola, zda sem na geometrii
					if(d.v.PtInSegment(pom_element_temp,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
					{
						//kontrola, zda nejsem na hraně objektu
						TPointD P;
						P.x=-1*MaxInt;P.y=-1*MaxInt;
						if(prichytavat_k_mrizce==1)P=d.v.InVrata(pom_element_temp);
						if(P.x!=-1*MaxInt && P.y!=-1*MaxInt)
						{
							//přilepení souřadnic na hranu kabiny
							TIP=ls->Strings[507];//"Přichyceno k hraně kabiny"
							prichyceno=1;
							pom_element->short_name=pom_element_temp->name;
							pocatek_mereni.x=pom_element_temp->geo.X4;
							pocatek_mereni.y=pom_element_temp->geo.Y4;
							break;
						}
						//kontrola začátku a konce stoupání
						if(pom_element_temp->geo.HeightDepp!=0)
						{
							if(m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom_element_temp->geo.X1,pom_element_temp->geo.Y1,velikost_citelne_oblasti_elementu))
							{
								TIP=ls->Strings[509];//"Přichyceno k začátku S/K"
								prichyceno=1;
								pocatek_mereni.x=pom_element_temp->geo.X4;
								pocatek_mereni.y=pom_element_temp->geo.Y4;
								break;
							}
							if(m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom_element_temp->geo.X4,pom_element_temp->geo.Y4,velikost_citelne_oblasti_elementu))
							{
								TIP=ls->Strings[510];//"Přichyceno ke konci S/K"
								prichyceno=1;
								pocatek_mereni.x=pom_element_temp->geo.X4;
								pocatek_mereni.y=pom_element_temp->geo.Y4;
								break;
							}
						}
						//pouze na geometrii
						if(d.v.PtInSegment(pom_element_temp,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
						{
							//přilepení souřadnic na geometrii
							prichyceno=1;
							TIP=ls->Strings[519];//"Přichyceno na pohon"
						}
						break;
					}
					pom_element_temp=d.v.dalsi_krok(VYH,pom_element_temp);
				}
				d.v.vymaz_seznam_VYHYBKY(VYH);
				pom_element_temp=NULL;delete pom_element_temp;
				//posun teploměru na kurzor
				pom_element->X=akt_souradnice_kurzoru.x+korekce.x;pom_element->Y=akt_souradnice_kurzoru.y+korekce.y;
				minule_souradnice_kurzoru=TPoint(X,Y);
				//zobrazení tipu a přiblížení k pohonu
				if(prichyceno==0)TIP=ls->Strings[504];//"Uchopením a tažením v oblasti pohonu změníte umístění teploměru."
				REFRESH();
			}break;
			case NIC://přejíždění po ploše aplikace, bez aktuálně nastavené akce
			case BLOK:
			{
				Akce=NIC;
				if(MOD!=CASOVAOSA)zneplatnit_minulesouradnice();
				if(MOD==EDITACE && OBJEKT_akt!=NULL || MOD==LAYOUT)
				{    //testování odstaveno
	//					pocitadlo_doby_neaktivity=0; Timer_neaktivity->Interval=20;
	//					if(++pocitadlo_zmeny_pozice.x>10 || ++pocitadlo_zmeny_pozice.y>10){pocitadlo_zmeny_pozice.x=0;pocitadlo_zmeny_pozice.y=0;pocitadlo_doby_neaktivity=1;}//naopak akcelerátor, aby se při rychlém pohybu myší zkontrolovala změna
	//					Timer_neaktivity->Enabled=true;//volá se zpožděním kvůli optimalizaci setJobIDOnMouseMove(X,Y);
	//					//pokud bych odstavil výše uvedený timer takto toto zprovoznit:
						//setJobIDOnMouseMove(X,Y);
						Timer_getjobid->Enabled=true;//restart timeru pro setjobid
				}
				//algoritmus na ověřování zda se kurzor nachází na objektem (a může být tedy povoleno v pop-up menu zobrazení volby nastavit parametry) přesunut do metody mousedownclick, zde se to zbytečně volalo při každém posunu myši
				break;
			}
			case MAGNETICKE_LASO:
			{
				//pan_move při stisknutém levém tlačítku
				//if(stisknute_leve_tlacitko_mysi){pan_map(Canvas,X,Y);kurzor(pan_move);Akce_temp=PAN;}else if(Screen->Cursor==pan_move)kurzor(add_o);
				//běžná fce.
				/*else*/ getJobID(X,Y);
			}break;
			//definice stoupání / klesání
			case S_K:
			{
				if(d.v.MAG_LASO!=NULL && d.v.MAG_LASO->dalsi!=NULL)
				{
					REFRESH(false);
					String popisek="";
					double X=d.v.MAG_LASO->dalsi->Element->geo.X1,Y=d.v.MAG_LASO->dalsi->Element->geo.Y1,azimut,delka;
					TPointD souradnice=bod_vlozeni_elementu();//"přilepení" kurzoru na pohon
					if(souradnice.x!=-1000 && pom_element_temp!=NULL && pom_element_temp==d.v.MAG_LASO->dalsi->sparovany)akt_souradnice_kurzoru=souradnice;//přichycovat na pohon poůze v případě, že jsem na jednom úseku
					azimut=m.azimut(X,Y,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y);
					delka=m.delka(X,Y,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y);
					if(souradnice.x!=-1000 && pom_element_temp!=NULL && pom_element_temp==d.v.MAG_LASO->dalsi->sparovany)popisek=String(m.round2double(delka*1000,2))+" [mm]";
					d.vykresli_Gelement(Canvas,X,Y,azimut,0,delka,m.clIntensive(clGray,-50),2,popisek,"",3);
				}
			}break;
			default: break;
		}
		////akce nad akcemi
		switch(Akce_temp)
		{
			case OFFSET_KOTY:
			{
				if(Screen->Cursor==zmena_d_y)OBJEKT_akt->koty_elementu_offset.y-=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				else OBJEKT_akt->koty_elementu_offset.y+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
				minule_souradnice_kurzoru=TPoint(X,Y);
				REFRESH(false);
				nahled_ulozit(true);
			}break;
			default:break;
		}
			//fix na pridani 1 obj pro demo
	//    if(EDICE==DEMO && MOD==LAYOUT)
	//    {
	//    if(d.v.OBJEKTY->predchozi->n >= 1) DrawGrid_knihovna->Visible=false;
	//    else   DrawGrid_knihovna->Visible=true;
	//    }
	//    if(MOD==EDITACE) DrawGrid_knihovna->Visible=true;
	}
}
//---------------------------------------------------------------------------
//vykreslí spojnici mezi tabulkou a elementem z nejbližšího rohu tabulky
void TForm1::vykresli_spojinici_EmGrid(TCanvas *Canv,Cvektory::TElement *E)
{
	log(__func__);//logování
	double levyhorni,pravyhorni,levydolni,pravydolni;
	bool vykresleno=false;
	if(E==NULL && predchozi_PM!=NULL && d.v.oblast_elementu(predchozi_PM,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))E=predchozi_PM;
	if(OBJEKT_akt!=NULL && OBJEKT_akt->zobrazit_mGrid && E!=NULL && !(JID<=-11 && JID>=-100) && !(JID<=99 && JID>=13)&& (d.v.vrat_druh_elementu(E)!=-1 || E->eID==200 || E->eID==300 || E->eID==301 || E->eID==400 || E->eID==401 || E->eID==402) && E->eID!=100 && E->mGrid!=NULL && (E->objekt_n==OBJEKT_akt->n || E==predchozi_PM))
	{
		//výpočet vzdáleností od každého rohu tabulky ke středu elementu
		levyhorni=m.delka(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt),m.L2Py(E->Yt));//ok
		pravyhorni=m.delka(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt)+E->mGrid->Width,m.L2Py(E->Yt));//ok
		levydolni=m.delka(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt),m.L2Py(E->Yt)+E->mGrid->Height);//ok
		pravydolni=m.delka(m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt)+E->mGrid->Width,m.L2Py(E->Yt)+E->mGrid->Height);//ok
		//stanovení nejmenší vzdálenosti
		double delka=Min(Min(levyhorni,pravyhorni),Min(levydolni,pravydolni));
		//vykreslí spojinici v nejmenší délce
		if(delka==levyhorni&&!vykresleno){d.linie(Canv,m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt),m.L2Py(E->Yt),2,(TColor)RGB(200,200,200),psSolid,pmCopy,true);vykresleno=true;}
		if(delka==pravyhorni&&!vykresleno){d.linie(Canv,m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt)+E->mGrid->Width,m.L2Py(E->Yt),2,(TColor)RGB(200,200,200),psSolid,pmCopy,true);vykresleno=true;}
		if(delka==levydolni&&!vykresleno){d.linie(Canv,m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt),m.L2Py(E->Yt)+E->mGrid->Height,2,(TColor)RGB(200,200,200),psSolid,pmCopy,true);vykresleno=true;}
		if(delka==pravydolni&&!vykresleno){d.linie(Canv,m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4),m.L2Px(E->Xt)+E->mGrid->Width,m.L2Py(E->Yt)+E->mGrid->Height,2,(TColor)RGB(200,200,200),psSolid,pmCopy,true);vykresleno=true;}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
	log(__func__);//logování
	if(Button==mbLeft)//zohlední jenom stisk levého tlačítka myši
	{
		refresh_mGrid=true;//globální navracení stavu
		if(pan_non_locked){pan_non_locked=false;Akce=NIC; kurzor(standard);pan_move_map();if(OBJEKT_akt!=NULL && puv_souradnice.x!=X && puv_souradnice.y!=Y)mGrid_on_mGrid();}//kontrola, zda nejsou překryty mGridy elementů a PmG
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
				if(MOD==LAYOUT)add_objekt(X,Y);//přídání objektu v modu LAYOUT
				else {if(element_id>=0)add_element(X,Y);else add_komoru();/*d.vykresli_element(Canvas,X,Y,"","",element_id,-1,Rotace_symbolu_minula);-již není třeba, někde se refreshuje*/}//přídání elementu v modu EDITACE
				zneplatnit_minulesouradnice();
				kurzor(standard);
				break;
			}
			case VYH:add_vyhybka_spojka();zneplatnit_minulesouradnice();break;//přidání objekt
			case MOVE:
			{
				//pokud byl objekt posunut a obsahuje už pohon a geometrii, zobrazen dotaz zda souhlasím z posunem
				if(OBJEKT_akt==NULL && pom->element->pohon!=NULL && ((puv_souradnice.x!=pom->element->geo.X1 && puv_souradnice.y!=pom->element->geo.Y1) || pom->orientace!=predchozi_orientace) && mrNo==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,ls->Strings[416],"",MB_YESNO,true,false))//"Objekt byl přesunut, souhlasíte s aktuálním umístěním?"
				{
					d.v.posun_objekt(puv_souradnice.x-pom->element->geo.X1,puv_souradnice.y-pom->element->geo.Y1,pom,false,false);
					if(pom->orientace!=predchozi_orientace)d.v.rotuj_objekt(pom,pom->orientace-predchozi_orientace);
				}
				else if((predchozi_souradnice_kurzoru.x!=m.L2Px(pom->element->geo.X1) && predchozi_souradnice_kurzoru.y!=m.L2Px(pom->element->geo.Y1)) || pom->orientace!=predchozi_orientace){if(pom->teplomery!=NULL)d.v.vytvor_default_c_teplomery(pom);vytvor_obraz();}
				duvod_validovat=1;//pozor vyvolává na závěr metody ještě REFRESH(); ale docela byl přínosný
				Akce=NIC;kurzor(standard);if(OBJEKT_akt!=NULL){scGPImage_zamek_posunu->ClipFrameFillColor=clWhite;scGPImage_zamek_posunu->ImageIndex=28;}//zamčen posun
			}break;//posun objektu
			case MOVE_TABLE:
			{
				//posun tabulek elementů
				if(pom_element!=NULL)
				{
					if(puv_souradnice.x!=pom_element->Xt && puv_souradnice.y!=pom_element->Yt)mGrid_on_mGrid();//kontrola, zda nejsou překryty mGridy elementů a PmG
					JID=-1;setJobIDOnMouseMove(X,Y);kurzor(posun_l);//kvůli rychlé aktualizaci po přesunu včetně Highlightu
				}
				//posun def. tabulky pohonu
				else if(PmG!=NULL)
				{
					if(puv_souradnice.x!=OBJEKT_akt->Xp && puv_souradnice.y!=OBJEKT_akt->Yp)mGrid_on_mGrid();//kontrola, zda nejsou překryty mGridy elementů a PmG
					JID=-1;setJobIDOnMouseMove(X,Y);kurzor(posun_l);//kvůli rychlé aktualizaci po přesunu včetně Highlightu
				}
				//rušení akce a kurzoru
				Akce=NIC;kurzor(standard);//REFRESH();znovu zakomentován, protože je volán v setJobIDOnMouseMove
				//vytvoření obrazu pro UNDO a REDO
				vytvor_obraz();
				d.SCENA=0;
				REFRESH();
			}break;
			case MOVE_ELEMENT:
			{
				//souřadnice byly změněny = posun
				if(m.round2double(pom_element_temp->X,4)!=m.round2double(puv_souradnice.x,4) || m.round2double(pom_element_temp->Y,4)!=m.round2double(puv_souradnice.y,4))
				{
					short chybne=prekryti_LO(pom_element);//kontrola zda se element nepřekrývá lak. oknem s jiným elementem
					FormX->odstranit_korelaci();//přidáno z důvodu odmazávání korelace při posuvu elementu
		   		Akce=NIC;kurzor(standard);
		   		UnicodeString text="";
		   		switch(chybne)
		   		{
		   			case 0: default:break;
						case 1:text=ls->Strings[324];break;//"Přesunem dojde k překrytí lakovacícho okna, chcete element přesunout?"
						case 2:text=ls->Strings[325];break;//"Přesunem dojde k překrytí zóny otáčení, chcete element přesunout?"
						case 3:text=ls->Strings[13];break;//"Přesunem dojde k překrytí pracovní oblasti, chcete element přesunout?"
					}
					if(chybne>0 && mrYes!=MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text,"",MB_YESNO))
					{
						double posunx=puv_souradnice.x-pom_element_temp->X,posuny=puv_souradnice.y-pom_element_temp->Y;
		   			pom_element_temp->X=puv_souradnice.x;pom_element_temp->Y=puv_souradnice.y;
		   			d.v.vloz_G_element(pom_element_temp,0,pom_element_temp->geo.X1,pom_element_temp->geo.Y1,0,0,0,0,pom_element_temp->geo.X4+posunx,pom_element_temp->geo.Y4+posuny,pom_element_temp->geo.orientace,pom_element_temp->geo.rotacni_uhel);
		   			//aktualizace geometrie všem ovlivněným elementům
		   			if(posun_dalsich_elementu)
						{
		   				//vrácení elementů ovlivněných vázaným posunem
		   	  		Cvektory::TElement *E=pom_element_temp->dalsi;
		   	  		while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
		   				{
		   	  			if(E->geo.typ!=NULL)break;
		   	  			if(E->eID!=MaxInt && E->eID!=200)
		   	  			{
		   						E->X+=posunx;
		   	  				E->Y+=posuny;
		   	  				d.v.vloz_G_element(E,0,E->predchozi->geo.X4,E->predchozi->geo.Y4,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
		   	  				if(E->dalsi!=NULL)d.v.vloz_G_element(E->dalsi,0,E->geo.X4,E->geo.Y4,0,0,0,0,E->dalsi->geo.X4,E->dalsi->geo.Y4,E->dalsi->geo.orientace);
		   	  			}
		   					E=E->dalsi;
		   	  		}
		   	  		E=NULL;delete E;
		   			}
		   			//aktualizace geometrie pouze dalšího
		   			else
		   			{
		   				if(pom_element_temp->dalsi!=NULL)
		   					d.v.vloz_G_element(pom_element_temp->dalsi,0,pom_element_temp->geo.X4,pom_element_temp->geo.Y4,0,0,0,0,pom_element_temp->dalsi->geo.X4,pom_element_temp->dalsi->geo.Y4,pom_element_temp->dalsi->geo.orientace,pom_element_temp->dalsi->geo.rotacni_uhel);
		   			}
		   		}
		   		else//došlo k posunu
					{
		   			if(m.delka(puv_souradnice.x,puv_souradnice.y,pom_element_temp->X,pom_element_temp->Y)>=1)//pokud byl posun větší něž jeden
		   			{
		   				pom_element_temp->Xt-=puv_souradnice.x-pom_element_temp->X;
		   				pom_element_temp->Yt-=puv_souradnice.y-pom_element_temp->Y;
		   			}
		   			//posunuta spojka kontrola dopojení geometrie vedlejší větve
		   			if(pom_element->eID==301 && (pom_element->predchozi2->geo.X4!=pom_element->geo.X4 || pom_element->predchozi2->geo.Y4!=pom_element->geo.Y4) && m.delka(pom_element->predchozi2->geo.X4,pom_element->predchozi2->geo.Y4,pom_element->geo.X4,pom_element->geo.Y4)<=1)
		   			{
		   				napojeni_vedlejsi_vetve(pom_element->predchozi2);
		   			}
		   			d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teploměry aktualizuje ji, jinak vytvoří default cestu
		   			duvod_validovat=2;//validovat při refreshi
		   			//validace PT podle RT, u S&G elementů (kromě stopky)
						if(d.v.vrat_druh_elementu(pom_element)==0 && pom_element->eID!=0)FormX->validace_PT(pom_element);
					}
					pom_element_temp=NULL; delete pom_element_temp;
					vlozit_predavaci_misto_aktualizuj_WT();//kontrola zda nebyly přesunuty 2 PM na sebe
					vytvor_obraz();
          d.SCENA=0;
					REFRESH();
				}
				//nejednalo se o posun ale o editaci textu
				else if(OBJEKT_akt!=NULL){kurzor(standard);nastav_focus();nahled_ulozit(true);stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;nazev_puvodni=pom_element_temp->name;Akce=EDITACE_TEXTU;TimerKurzor->Enabled=true;vytvor_statickou_scenu();REFRESH();}
				else {Akce=NIC;kurzor(standard);pom_element_temp=NULL;delete pom_element_temp;d.SCENA=1111111;vytvor_statickou_scenu();REFRESH();}
				break;//posun elementu
			}
			case MOVE_KOMORA:
			{
				d.v.presun_komoru(OBJEKT_akt,pom_komora_temp,d.v.najdi_komoru(OBJEKT_akt));
				pom_komora_temp=NULL;delete pom_komora_temp;
				Akce=NIC;
				refresh_mGrid=false;
				REFRESH();
				refresh_mGrid=true;
				vytvor_obraz();
				break;
			}
			case MOVE_TEXT:if(OBJEKT_akt!=NULL && vychozi_souradnice_kurzoru.x==minule_souradnice_kurzoru.x && vychozi_souradnice_kurzoru.y==minule_souradnice_kurzoru.y){nastav_focus();nahled_ulozit(true);Akce=EDITACE_TEXTU;index_kurzoru=-6;nazev_puvodni=pom->name;stav_kurzoru=false;TimerKurzor->Enabled=true;}else {Akce=NIC;d.SCENA=0;REFRESH();}kurzor(standard);pom_vyhybka=NULL;vytvor_obraz();break;
			case MOVE_BOD:
			case MOVE_USECKA:JID=-1;Akce=NIC;kurzor(standard);vytvor_obraz();if(OBJEKT_akt!=NULL)d.SCENA=0;REFRESH();break;
			case MOVE_HALA:Akce=NIC;kurzor(standard);vytvor_obraz();REFRESH();break;//refresh z důvodu znovu zapnutí měřítka a gridu
			case ROZMER_KOMORA:Akce=NIC;break;
			case OFFSET_KOTY:Akce=NIC;break;
			case MEASURE:
			{
				UnicodeString text=ls->Strings[326]+" ",text_1=ls->Strings[327]+" ";//"Vzdálenost: ","Čas: "
				double delka=m.delka(m.P2Lx(vychozi_souradnice_kurzoru.X),m.P2Ly(vychozi_souradnice_kurzoru.Y),m.P2Lx(X),m.P2Ly(Y));
				if(OBJEKT_akt!=NULL&&OBJEKT_akt->pohon!=NULL)MB(text+AnsiString(m.round2double((delka*1000.0),3))+" [mm]; "+text_1+AnsiString(m.round2double((delka/OBJEKT_akt->pohon->aRD),3))+" [s]",MB_OK,true,366,true,true,true);
				else MB(AnsiString(m.round2double((delka*1000.0),3))+" [mm]");
				Akce=NIC;kurzor(standard);
				zobraz_tip("");//nahrazuje zároveň Refresh
				scGPImage_mereni_vzdalenost->ClipFrameFillColor=clWhite;
				nastav_focus();//nutné!!!! bez tohoto nefunguje formkeydown
				break;
			}
			case KALIBRACE:
			{
				d.v.PP.raster.X=m.P2Lx(X)+d.v.PP.raster.X-m.P2Lx(vychozi_souradnice_kurzoru.X);
				d.v.PP.raster.Y=m.P2Ly(Y)+d.v.PP.raster.Y-m.P2Ly(vychozi_souradnice_kurzoru.Y);
				Akce=NIC;kurzor(standard);
				zobraz_tip("");//nahrazuje zároveň Refresh
				break;
			}
			case ADJUSTACE:
			{
				if(Form_adjustace->ShowModal()==mrOk)//MB "Zadejte vzdálenost v metrech"
				{
					double vzdalenost=Form_adjustace->scGPNumericEdit_vzdalenost->Value/(1+999.0*Form_adjustace->Delkaunit);
					d.v.PP.raster.resolution=m.getResolution(vychozi_souradnice_kurzoru.X,vychozi_souradnice_kurzoru.Y,X,Y,vzdalenost);
          RzToolButton11Click(Sender);//celý pohled
				}
				Akce=NIC;kurzor(standard);
				zobraz_tip("");//nahrazuje zároveň Refresh
				break;
			}
			case GEOMETRIE:if(Akce_temp==NIC)pan_move_map();break;
			case MAGNETICKE_LASO:
			{
				unsigned int V=0;
				if(d.v.SIM!=NULL)
				{
					V=getV();//číslo vozíku
					if(V>0)//kliknuto do grafu
					{
						if(pocatek_mereni.x==-1 && pocatek_mereni.y==-1)
						{
							pocatek_mereni.x=akt_souradnice_kurzoru_PX.x;
							if(prichytavat_k_mrizce==1)pocatek_mereni.x=d.v.prichytit_cas_SIM(V);
							pocatek_mereni.y=V;
						}
						else
						{
              scGPImage_mereni_vzdalenostClick(this);
            }
          }
        }
				//začátek a konec měření na lince
				if(Akce_temp==NIC && V==0)
				{
					if(pom_element!=NULL)akt_souradnice_kurzoru=d.v.bod_na_geometrii(pom_element);
			  	Cvektory::TElement *E=new Cvektory::TElement;
					E->n=MaxInt;
          E->pohon=NULL;
			  	E->geo.rotacni_uhel=0;
			  	E->geo.orientace=0;
			  	E->geo.radius=0;
					E->geo.delka=0;
					E->geo.typ=0;
			  	E->geo.X2=2;E->geo.Y2=2;
			  	E->geo.X3=3;E->geo.Y3=3;
					//kontrola přichytávání na elementy při počátečním a koncovém bodu
					Cvektory::TElement *el=d.v.ELEMENTY->dalsi;
					Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
					bool prichiceno=false;
					while(el!=NULL)
					{
						//kontrola oblastí elementů
						if(prichytavat_k_mrizce==1 && (el->eID!=MaxInt || (el->eID==MaxInt && el->dalsi==NULL)) && m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,el->geo.X4,el->geo.Y4,velikost_citelne_oblasti_elementu))
						{
							pom_element=el;
							//přichytit souřadnicemi na element, X2 a X3 se používají jako příznak přichycení, pokud je přichicenou jsou nenulové a rovnají se X4
							E->geo.X2=E->geo.X3=akt_souradnice_kurzoru.x=el->geo.X4;
							E->geo.Y2=E->geo.Y3=akt_souradnice_kurzoru.y=el->geo.Y4;
							E->name=ls->Strings[506]+" "+el->name;//"Přichyceno k"
							//highlight, pouze pro prvního
			  			if(d.v.MAG_LASO->dalsi==NULL && d.v.MAG_LASO->Element==NULL)pom_element->stav=2;
							prichiceno=true;
			  			break;
			  		}
			  		//kontrola přichycení na začátek nebo konec stouání / klesání
			  		else if(prichytavat_k_mrizce==1 && el->geo.HeightDepp!=0)
						{
			  			//začátek stoupání / klesání
			  			if(m.delka(el->geo.X1,el->geo.Y1,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y)<=velikost_citelne_oblasti_elementu)
							{
			  				pom_element=el;
			  				E->geo.X2=E->geo.X3=akt_souradnice_kurzoru.x=el->geo.X1;
								E->geo.Y2=E->geo.Y3=akt_souradnice_kurzoru.y=el->geo.Y1;
								E->name=F->ls->Strings[509];//"Přichyceno k začátku S/K"
								prichiceno=true;
								break;
			  			}
							//konec stoupání / klesání
			  			if(m.delka(el->geo.X4,el->geo.Y4,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y)<=velikost_citelne_oblasti_elementu)
							{
								pom_element=el;
								E->geo.X2=E->geo.X3=akt_souradnice_kurzoru.x=el->geo.X4;
								E->geo.Y2=E->geo.Y3=akt_souradnice_kurzoru.y=el->geo.Y4;
								E->name=F->ls->Strings[510];//"Přichyceno ke koneci S/K"
								prichiceno=true;
								break;
							}
						}
						//kontrola přichycení na hranu objektu
						if(prichytavat_k_mrizce==1 && el->geo.typ==0 && m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,el->geo.X1,el->geo.Y1,el->geo.X4,el->geo.Y4)<=0.5)
						{
							//kontrola zda jsem na hraně objektu
							TPointD P;
							P.x=-1*MaxInt;P.y=-1*MaxInt;
							P=d.v.InVrata(el);
							if(P.x!=-1*MaxInt && P.y!=-1*MaxInt)
							{
								pom_element=el;
								//přichytit souřadnicemi na element, X2 a X3 se používají jako příznak přichycení, pokud je přichicenou jsou nenulové a rovnají se X4
								akt_souradnice_kurzoru=P;
								E->geo.X2=E->geo.X3=akt_souradnice_kurzoru.x;
								E->geo.Y2=E->geo.Y3=akt_souradnice_kurzoru.y;
								E->name=ls->Strings[507];//"Přichyceno k hraně kabiny"
                prichiceno=true;
								break;
							}
						}
						//kontrola zda jsem na pohonu
						if(d.v.PtInSegment(el,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
						{
							pom_element=el;
							akt_souradnice_kurzoru=d.v.bod_na_geometrii(pom_element);
						}
						el=d.v.dalsi_krok(VYHYBKY,el);
			  	}
					d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
			  	el=NULL;delete el;
			  	//kontrola přichycení na vozík
			  	Cvektory::TVozik *V=d.v.VOZIKY->dalsi;
					while(!prichiceno && V!=NULL && zobrazit_rozmisteni_voziku==1)
					{
						if(m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,V->X,V->Y,velikost_citelne_oblasti_elementu))
			  		{
			  			E->geo.X2=E->geo.X3=akt_souradnice_kurzoru.x=V->X;
			  			E->geo.Y2=E->geo.Y3=akt_souradnice_kurzoru.y=V->Y;
              E->name=ls->Strings[508]+" "+String(V->n);//"Přichyceno k vozíku č."
			  			break;
	        	}
	        	V=V->dalsi;
			  	}
			  	V=NULL;delete V;
          //uložení parametrů geometrie z úseku, stoupání, typ atd.
			  	if(pom_element!=NULL)
					{
            E->eID=pom_element->eID;
						E->geo.typ=pom_element->geo.typ;
			  		E->geo.radius=pom_element->geo.radius;
			  		E->geo.delka=pom_element->geo.delka;
			  		E->geo.delkaPud=pom_element->geo.delkaPud;
			  		E->geo.HeightDepp=pom_element->geo.HeightDepp;
			  	}
			  	//vkládání počátečního bodu pro měření
			  	if(d.v.MAG_LASO->dalsi==NULL && d.v.MAG_LASO->Element==NULL)
					{
						E->geo.X1=akt_souradnice_kurzoru.x;
			  		E->geo.Y1=akt_souradnice_kurzoru.y;
						E->geo.X4=E->geo.X1;
						E->geo.Y4=E->geo.Y1;
			  		d.v.MAG_LASO->Element=E;
			  		d.v.MAG_LASO->sparovany=NULL;
						//ukládání počátečního elementu pokud jsem začal na geometrii
						if(pom_element!=NULL)d.v.MAG_LASO->sparovany=pom_element;
            REFRESH();//slouží pro okamžité zobrazení měřidla
			  	}
			  	//vkládání koncového bodu a ukončení měření
			  	else
					{
			  		//vkládání posledního elementu
			  		if(pom_element==NULL && d.v.MAG_LASO->predchozi->n>0)d.v.smaz_segment_MAG_LASA(d.v.MAG_LASO->dalsi->Element);//pokud jsem kliknul mimo pohon, jedná se o lineární měření, cesta nění třeba
			  		E->geo.X1=d.v.MAG_LASO->predchozi->Element->geo.X4;
			  		E->geo.Y1=d.v.MAG_LASO->predchozi->Element->geo.Y4;
			  		E->geo.X4=akt_souradnice_kurzoru.x;
			  		E->geo.Y4=akt_souradnice_kurzoru.y;
			  		d.v.vloz_segment_MAG_LASA(E);
			  		if(pom_element!=NULL)d.v.MAG_LASO->predchozi->sparovany=pom_element;//ukládání počátečního elementu pokud jsem začal na geometrii
			  		E=NULL;delete E;
			  		scGPImage_mereni_vzdalenostClick(this);
					}
					E=NULL;delete E;
				}
        //posun obrazu při akci měření
				else if(V==0)pan_move_map();
			}break;
			case POSUN_TEPLOMER:
			{
				d.v.posun_teplomeru(pom_element);//ukončení posunu, rozhodnutí zda uložit cestu, neuložit, dolnující dotaz na výhybce
				Akce=NIC;
				TIP="";
				if(OBJEKT_akt!=NULL && JID==-8 && vychozi_souradnice_kurzoru.x==minule_souradnice_kurzoru.x && vychozi_souradnice_kurzoru.y==minule_souradnice_kurzoru.y){nastav_focus();nahled_ulozit(true);stav_kurzoru=false;index_kurzoru=JID;nazev_puvodni=pom_element->name;pom_element_temp=pom_element;Akce=EDITACE_TEXTU;TimerKurzor->Enabled=true;vytvor_statickou_scenu();}
				else {nahled_ulozit(true);d.SCENA=0;}
        REFRESH();
				pom_element=NULL;
			}break;
			//stoupání klesání
			case S_K:
			{
				//hledání segmentu, do kterého bylo kliknuto
				Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
        Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
				while(E!=NULL)
				{
					if(d.v.PtInSegment(E,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y)){pom_element=E;break;}
					E=d.v.dalsi_krok(VYHYBKY,E);
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//odstranění průchodového spojáku
				E=NULL;delete E;

				//bylo kliknuto do segmentu
				if(pom_element!=NULL)
				{
        	akt_souradnice_kurzoru=d.v.bod_na_geometrii(pom_element);
					//vložení do spojáku, použit spoják mag_lasa
					E=new Cvektory::TElement;
					E->geo=pom_element->geo;
					E->geo.X1=E->geo.X2=E->geo.X3=E->geo.X4=akt_souradnice_kurzoru.x;
					E->geo.Y1=E->geo.Y2=E->geo.Y3=E->geo.Y4=akt_souradnice_kurzoru.y;
					d.v.vloz_segment_MAG_LASA(E);
          d.v.MAG_LASO->predchozi->sparovany=pom_element;
					pom_element=NULL;
					if(d.v.MAG_LASO->predchozi->n==1)TIP=ls->Strings[502];//"Tažením myši a následným kliknutím vložíte koncový bod."
					//ukončení
					if(d.v.MAG_LASO->predchozi->n==2 && d.v.MAG_LASO->dalsi->sparovany==d.v.MAG_LASO->predchozi->sparovany && m.Rt90(m.azimut(d.v.MAG_LASO->dalsi->Element->geo.X1,d.v.MAG_LASO->dalsi->Element->geo.Y1,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))==d.v.MAG_LASO->dalsi->sparovany->geo.orientace)
					{
            //vkládání 2 zarážek
						Akce=ADD;//nutné pro vkládání!!!
						E=d.v.vloz_element(OBJEKT_akt,MaxInt,d.v.MAG_LASO->dalsi->Element->geo.X1,d.v.MAG_LASO->dalsi->Element->geo.Y1,0);
						E->mGrid=new TmGrid(F);
						E->mGrid->Tag=6;//ID formu
						E->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;
						design_element(E,true);//znovuvytvoření tabulek
						E=NULL;delete E;
						E=d.v.vloz_element(OBJEKT_akt,MaxInt,d.v.MAG_LASO->predchozi->Element->geo.X1,d.v.MAG_LASO->predchozi->Element->geo.Y1,0);
            E->mGrid=new TmGrid(F);
						E->mGrid->Tag=6;//ID formu
						E->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;
						design_element(E,true);//znovuvytvoření tabulek
						//vložení S/K
            double delka=E->geo.delka;
						double HeightDepp=1;//default klesání, stoupání
						double delkaSklon=m.delkaSklon(delka,HeightDepp);
						if(element_id==-1*MaxInt)HeightDepp*=-1;
			    	E->geo.delkaPud=delka;
			    	E->geo.delka=delkaSklon;
						E->geo.HeightDepp=HeightDepp;
						//aktualizace bufferů a validace
            if(E->dalsi!=NULL && E->dalsi->eID==0)
						{
              //výpočet nových hodnot
							E->dalsi->data.pocet_pozic=max_voziku(E->dalsi);
							if(E->dalsi->data.pocet_pozic<E->dalsi->data.pocet_voziku)E->dalsi->data.pocet_voziku=E->dalsi->data.pocet_pozic;
							//zapsání nových hodnot do mGridu
							if(E->dalsi->objekt_n==OBJEKT_akt->n && E->dalsi->mGrid!=NULL)
							{
								E->dalsi->mGrid->Cells[2][5].Text=E->dalsi->data.pocet_pozic;
								E->dalsi->mGrid->Cells[2][6].Text=E->dalsi->data.pocet_voziku;
              }
							duvod_validovat=2;//přegeneruje vozíky
						}
            //aktualizace teploměrů
						d.v.aktualizuj_cestu_teplomeru();
						//aktualizace Z elementům
						Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
						while(E!=NULL)
						{
              if(E->predchozi->n!=0)E->Z=E->predchozi->Z+E->geo.HeightDepp;
							else E->Z=0;
							E=d.v.dalsi_krok(VYHYBKY,E);
						}
						d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
            delete E;E=NULL;
						//důvody k uložení
						DuvodUlozit(true);
            nahled_ulozit(true);
					}
					else if(d.v.MAG_LASO->predchozi->n>1)TIP=ls->Strings[499];//"Lezení nebo klesání lze zadat pouze ve směru čáry."
					if(d.v.MAG_LASO->predchozi->n>1)
					{
						Akce=NIC;
						kurzor(standard);
						//smazání seznamu MAG_LASO
						d.v.vymaz_seznam_MAG_LASO();
						d.v.hlavicka_MAG_LASO();
						d.SCENA=0;
						REFRESH(false);
					}
				}
				//bylo kliknuto mimo, ukončení akce
				else //if(d.v.MAG_LASO->predchozi->n>0)
				{
					Akce=NIC;
					kurzor(standard);
					//smazání seznamu MAG_LASO
					d.v.vymaz_seznam_MAG_LASO();
					d.v.hlavicka_MAG_LASO();
					TIP=ls->Strings[500];//"Vkládaní se nezdařilo, musíte se držet v oblasti pohonu."
          d.SCENA=0;
					REFRESH(false);
        }
			}break;
			//case TVORBA_CESTY:if(Pan_bmp!=NULL)pan_move_map();break;
			default: break;
		}
    //navrácení do plně statické scény
		if(MOD==LAYOUT && Akce==NIC && d.v.SIM==NULL && (d.SCENA!=1111111 || pom!=NULL))REFRESH(1111111,false);//zobrazení aktuální scény, např při ukončení posuvu musí být změny promtnuty do BMP scény a ta následně zobrazena

		switch(Akce_temp)
		{
      case EDITACE_TEXTU:break;
			default:if(Akce_temp!=NIC)Akce_temp=NIC;break;
    }
	}
	if(Button==mbMiddle && Akce==PAN_MOVE)//ukončení posunu pomocí stisknutého kolečka
	{
		pan_non_locked=false;
		Akce=NIC;
		kurzor(standard);
		pan_move_map();
		if(OBJEKT_akt!=NULL && puv_souradnice.x!=X && puv_souradnice.y!=Y)mGrid_on_mGrid();//kontrola, zda nejsou překryty mGridy elementů a PmG
	}
	stisknute_leve_tlacitko_mysi=false;
	if(duvod_validovat==1){duvod_validovat=2;REFRESH();/*d.v.VALIDACE();*/}//pokud je důvod validovat, tak až po dokonečení akce, nešlo monitorovat dle Akce==NIC, volání VALIDACE pomocí REFRESH (aplikace zde takto, někdy není odchytnut REFRESH po dokončení akce)
	//vrat_puvodni_akci();
	/*if(X<=RzSizePanel_knihovna_objektu->Width) DrawGrid_knihovna->Enabled=true;
	else DrawGrid_knihovna->Enabled=false;*/

}
//---------------------------------------------------------------------------
//vrátí do globální proměnné JID ID úlohy/funkcionality v místě kurzoru, zároveň pokud v místě tabulky či elementu nahraje ukazatel do globální proměnné pom_element
//JID=-2;//hrana polygonu objektu							Původní: svislá levá hrana objektu
//JID=-3;//bod polygonu objektu								Původní: vodorovná horní hrana objektu
//JID=-4;//oblast kót objektu									Původní: svislá pravá hrana objektu
//JID=-5;//hodnoty kót objektu								Původní: vodorovná dolní hrana objektu
//JID=-6;//název objektu
//JID=-7;//tělo teploměru
//JID=-8;//popisek teploměru
//JID=-9;//hrana komory POW										Původní: uchop tabulky pohonu
//JID=-10;//jednotky kóty
//JID=-11;//hodnota kóty elementu; hodnota kóty komory
//JID=-12;//trojúhelník S/K elementu
//JID=-13;//popisek konece S/K elementu
//JID=-14;//popisek začátku S/K elementu
//JID=-101;//hodnota LO kóty elementu
//JID=-102;//citelná oblast zprávy
//JID=-201;//pohon1 na předávacím místě
//JID=-202;//pohon2 na předávacím místě
//JID=-203;//jiný objekt než objekt editovaný
//JID=-1 žádná
//JID= 0 - 3 rezervováno pro element (komoru), 0 - element mimo název (komora, kurzor nad oblastí komory), 1 - citelná oblast název elementu, 2,3 - rezerva, bližší identifikace elementu probíhá pomocí pom_element
//JID= 4;//citelná oblast celé tabulky pohonu
//JID= 5 - 12 první až poslední řádek tabulky pohonů, hlavička nevyužito z důvodu obsazení COMBOBOXem, 5 - aRD, 6 - rozteč, 7 - nevyužito, 8 - rozestup, 9 - mezera jig, 10 - mezera jig
//JID= 13 - oblast kóty (elementů nebo komor - dle použitého objektu (v POW komory jsou vnímány jako elementy)), 10+pom_element->n nebo komora->n - oblast kóty/posun kóty a n elementu
//JID= 100-999 rezervováno pro odkazy v tabulce, kde 100 znamená nultý řádek libovolného sloupce (ideální pro sloučenou hlavičku), u 100 se více neže o odkaz jedná o interaktivní text, který je určen k editaci formou změny na EDIT, ale definovaný odkazem pomocí tagu <a>název</a>, to zajistí naplnění citelné oblasti
//JID= 1000-1999 rezervováno pro řádky nultého slopce tabulky (v místě, kde není odkaz), kde 1000 znamená nultý řádek tabulky nehledě na sloupec (ideální pro sloučenou hlavičku), kde není odkaz
//JID= 2000-rezervováno pro 2 či další sloupce resp. řádky, 2000 - nevyužito,2001 - první řádek (po nultém)
void TForm1::getJobID(int X, int Y)
{
	log(__func__);//logování
	JID=-1;//výchozí stav, nic nenalezeno
  FormX->vykresli_vetev=0;
	pom_element=NULL;
	pom_komora=NULL;
	pom_bod=NULL;
	d.zprava_highlight=0;
	if(Akce!=MAGNETICKE_LASO)
	{
		if(MOD==EDITACE && !OBJEKT_akt->uzamknout_nahled && Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU)
		{
			//nejdříve se zkouší hledat souřadnice myši v TABULCE POHONů
  		if(PmG!=NULL && OBJEKT_akt->zobrazit_mGrid)
			{
				int IdxRow=PmG->GetIdxRow(X,Y);
  			//if(IdxRow==0)JID=4;//hlavička NEVYUŽITO, je tam COMBO, zachováno jako rezerva
  			if(IdxRow>0)//nějaký z řádků mimo nultého tj. hlavičky, nelze použít else, protože IdxRow -1 bude také možný výsledek
  			{
  				int IdxCol=PmG->GetIdxColumn(X,Y);
  				//if(IdxCol==1)//řádky v prvním sloupeci
  				{
  					if(PmG->CheckLink(X,Y,IdxCol,IdxRow))JID=4+IdxRow;//na daném řádku a daných myších souřadnicích se nachází odkaz
  					//else JID=XX+IdxRow;//řádek v nultém sloupci mimo odkaz - NEVYUŽITO
  				}
  				//else JID=XX+IdxRow;//řádky v dalších sloupcích - NEVYUŽITO
  			}
  			//pokud nic nenalezeno snažím se najít PmG i nad komponenty v tabulce
  			if(JID==-1 && ((m.L2Px(OBJEKT_akt->Xp)<X+m.L2Px(OBJEKT_akt->Xp) && X+m.L2Px(OBJEKT_akt->Xp)<=m.L2Px(OBJEKT_akt->Xp)+PmG->Width && ID_tabulky==9999)||(m.L2Px(OBJEKT_akt->Xp)<X && X<=m.L2Px(OBJEKT_akt->Xp)+PmG->Width)) && ((m.L2Py(OBJEKT_akt->Yp)<Y+m.L2Py(OBJEKT_akt->Yp) && Y+m.L2Py(OBJEKT_akt->Yp)<m.L2Py(OBJEKT_akt->Yp)+PmG->Height && ID_tabulky==9999) || (m.L2Py(OBJEKT_akt->Yp)<Y && Y<m.L2Py(OBJEKT_akt->Yp)+PmG->Height)))JID=4;
  		}
  		if(JID==-1)//pokud nebyla tabulka pohonu nalezena zkouší hledat další aktivní prvky náhledu
  		{
    		//dále TABULKY ELEMENTŮ
				pom_element=d.v.najdi_tabulku(OBJEKT_akt,m.P2Lx(X),m.P2Ly(Y));
  			if(pom_element!=NULL && OBJEKT_akt->zobrazit_mGrid)//možné měnit rozmístění a rozměry a tabulka nalezena, tzn. klik či přejetí myší přes tabulku
				{
  				int IdxRow=pom_element->mGrid->GetIdxRow(X,Y);
  				if(IdxRow==0)
    			{                                 //kvůli sloučeným buňkám
						int IdxCol=pom_element->mGrid->GetIdxColumn(X,Y);
						if(pom_element->mGrid->CheckLink(X,Y,IdxCol,0))JID=100+0;//text názvu (pseudointeraktivní text - formou editu a definovaný odkazem <a>název</a>) v hlavičce
						else if(OBJEKT_akt->uzamknout_nahled==false)JID=1000+0;//hlavička - posouvat tabulku je možné pouze při odemčeném náhledu
    			}
    			if(IdxRow>0)//nějaký z řádků mimo nultého tj. hlavičky, nelze použít else, protože IdxRow -1 bude také možný výsledek
					{
						int IdxCol=pom_element->mGrid->GetIdxColumn(X,Y);
						if(IdxCol!=-1 && pom_element->mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxRow;//na daném řádku a daných myších souřadnicích se nachází odkaz
						else if(IdxCol==0)//řádky v prvním sloupeci
  					{
  						//if(pom_element->mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxRow;//na daném řádku a daných myších souřadnicích se nachází odkaz
  						/*else */if(OBJEKT_akt->uzamknout_nahled==false)JID=1000+IdxRow;//řádky bez odkazu možné posouvat tabulku je možné pouze při odemčeném náhledu
						}
						else if(IdxCol!=-1 && OBJEKT_akt->uzamknout_nahled==false)JID=(IdxCol+1)*1000+IdxRow;//řádky v druhém a dalších sloupcích
						//nastavenní vykreslovací větve
						if(pom_element->eID==300 && ((JID>=4001 && JID<=4010) || (JID>=5001 && JID<=5010)))
						{
							if(JID>=5001)FormX->vykresli_vetev=2;
							else FormX->vykresli_vetev=1;
							if(prohodit_sloupce_PM(pom_element))
							{
								if(FormX->vykresli_vetev==1)FormX->vykresli_vetev=2;
                else FormX->vykresli_vetev=1;
              }
						}
						if(pom_element->eID==301 && (JID>=2001 && JID<=2002 || JID>=3001 && JID<=3002))
						{
							if(JID>3000)FormX->vykresli_vetev=2;
							else FormX->vykresli_vetev=1;
						}
					}
    		}
				else//tabulka nenalezena, takže zkouší najít ELEMENT
  			{
  				d.zprava_highlight=d.v.PtInZpravy();
  				if(d.zprava_highlight>0)JID=-102;//hledání citelné oblasti zprávy
  				else
  				{
  					pom_element=NULL;
  					if(OBJEKT_akt->uzamknout_nahled==false && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))pom_element=F->d.v.najdi_element(OBJEKT_akt,m.P2Lx(X),m.P2Ly(Y));//pouze pokud je možné měnit rozmístění a rozměry,nutné jako samostatná podmínka
						if(pom_element!=NULL)//element nalezen, tzn. klik či přejetí myší přes elemement nikoliv tabulku
						{                                      //volá se v oblast_elementu duplicitně také, toto by se dalo nahradit, protože oblast_elementu vrací v případě, že se jedná o název hodnotu 2
							if(scGPCheckBox1_popisky->Checked && pom_element->citelna_oblast.rect3.PtInRect(TPoint(X,Y)))JID=1;//byl nalezen název elementu
							else JID=0; //byl nálezen element nikoliv jeho název, určeno k smazání či posunu elementu
  					}
  					else //ani element nenalezen, hledá tedy interaktivní text, obrys a kóty atp.
						{
							pom_element=NULL;
							short teplomery=d.v.najdi_teplomer();
							if(pom_element!=NULL && teplomery==1)JID=-7;//nalezení těla teploměru
							else if(pom_element!=NULL && teplomery==2)JID=-8;//nalezení popisku teploměru
							else
							{
					   		pom_bod=d.v.najdi_bod(OBJEKT_akt);//pokouším se najít bod v obrysu kabiny
					   		if(pom_bod!=NULL)JID=-3;//bod nalezen
					   		else //bod nenalezen, pokouším se najít hranu kabiny
					   		{
					   			pom_bod=d.v.najdi_usecku(OBJEKT_akt,2);
					   			if(pom_bod!=NULL)JID=-2;//hrana nalezena
					   			else
  				   			{ //testování zda se nejedná o NÁZEV či ZKRATKA objektu, ZATÍM NEREFLEKTUJE ORIENTACI NÁHLEDU
					   				d.nastavit_text_popisu_objektu_v_nahledu(Canvas);
  				   				if(najdi_nazev_obj(X,Y,OBJEKT_akt))JID=-6;//název objektu
					   				if(JID==-1)//hledání předávacího místa, pohon 1 nebo pohon 2
					   				{
  			     					short pohon=najdi_popisky_PM(X,Y,OBJEKT_akt);
					   					if(pohon>0)JID=-200-pohon;
  			     				}
  			     				if(JID==-1)//nejedná tj. testují se KÓTY
  				   				{
					   					if(OBJEKT_akt->zobrazit_koty)//pouze pokud je náhled povolen a jsou kóty zobrazeny
  			     					{
					   						short PtInKota_elementu=-1;
					   						if(d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0)PtInKota_elementu=d.v.PtInKota_elementu(OBJEKT_akt,X,Y);
					   						//jednotky kóty buď kabiny nebo kót elementů JID=-10
												if(PtInKota_elementu==3)JID=-101;//hodnota LO kóty
												else if(OBJEKT_akt->kabinaKotaX_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || OBJEKT_akt->kabinaKotaY_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || PtInKota_elementu==2)JID=-10;
												else if(OBJEKT_akt->uzamknout_nahled==false)//hledám kóty kabiny
					   						{
													short PtInKota_bod=d.v.PtInKota_bod(OBJEKT_akt);//metoda vrací zda jsem v oblasti kóty nebo v její hodnotě + ukládá ukazatel na bod do pom_bod
													if(PtInKota_elementu==1 && pom_element!=NULL)JID=-11;//(10+pom_element->n)*(-1);//hodnota kóty
													else if(PtInKota_elementu==0 && pom_element!=NULL)JID=13;//oblast kóty - posun kóty
													else if(PtInKota_bod==0 && pom_bod!=NULL)JID=-4;//oblast kóty - posun kóty
													else if(PtInKota_bod==1 && pom_bod!=NULL)JID=-5;//hodnota kóty
												}
											}
					   				}
					   			}
								}
							}
						}
					}
  			}
  		}
			if(JID==-1&&OBJEKT_akt->uzamknout_nahled==false&&OBJEKT_akt->id==3)
			{
				pom_komora=d.v.najdi_komoru(OBJEKT_akt);
  			if(pom_komora!=NULL)//komora nalezena
  			{
					JID=0;//uložení komory do JID
  				//detekce hrany komory
					switch((int)OBJEKT_akt->orientace)
  				{
						case 0:if(Y<=m.L2Py(OBJEKT_akt->body->dalsi->Y+d.v.vrat_velikosti_komor(pom_komora))+9&&Y>=m.L2Py(OBJEKT_akt->body->dalsi->Y+d.v.vrat_velikosti_komor(pom_komora))-3)JID=-9;/*(10+pom_komora->n)*(-1);*/break;
						case 90:if(X<=m.L2Px(OBJEKT_akt->body->dalsi->X+d.v.vrat_velikosti_komor(pom_komora))+9&&X>=m.L2Px(OBJEKT_akt->body->dalsi->X+d.v.vrat_velikosti_komor(pom_komora))-3)JID=-9;break;
						case 180:if(Y>=m.L2Py(OBJEKT_akt->body->dalsi->Y-d.v.vrat_velikosti_komor(pom_komora))-9&&Y<=m.L2Py(OBJEKT_akt->body->dalsi->Y-d.v.vrat_velikosti_komor(pom_komora))+3)JID=-9;break;
						case 270:if(X>=m.L2Px(OBJEKT_akt->body->dalsi->X-d.v.vrat_velikosti_komor(pom_komora))-9&&X<=m.L2Px(OBJEKT_akt->body->dalsi->X-d.v.vrat_velikosti_komor(pom_komora))+3)JID=-9;break;
  				}
  			}
  			else if(OBJEKT_akt->zobrazit_koty)//prohledávání kót
  			{
					short PtInKota_komory=d.v.PtInKota_komory(OBJEKT_akt,X,Y);
					if(PtInKota_komory==0 && pom_komora!=NULL)JID=10+pom_komora->n;//oblast kóty - posun kóty
  				if(PtInKota_komory==1 && pom_komora!=NULL)JID=-11;//hodnota kóty
  			}
			}
			//hledání trojúhelníku S/K
			if(JID==-1)
			{
				TRect oblast,oblast_pom;
				pom_element=OBJEKT_akt->element;
				Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();//vytvoření průchodového spojáku
				while(pom_element!=NULL)
				{
					if(pom_element->geo.HeightDepp!=0)//pouye pro elementy s S/K
					{
				  	//naplnění oblasti
						oblast.left=(pom_element->citelna_oblast.rect6.left+pom_element->citelna_oblast.rect6.right)/2;
						oblast.right=(pom_element->citelna_oblast.rect7.left+pom_element->citelna_oblast.rect7.right)/2;
						if(pom_element->geo.orientace==90 || pom_element->geo.orientace==270)
				  	{
							oblast.top=pom_element->citelna_oblast.rect6.bottom;
           		oblast.bottom=pom_element->citelna_oblast.rect7.bottom;
				  	}
						else
						{
							oblast.left=pom_element->citelna_oblast.rect6.left-5;
							oblast.right=pom_element->citelna_oblast.rect7.left-5;
							oblast.top=(pom_element->citelna_oblast.rect6.top+pom_element->citelna_oblast.rect6.bottom)/2;
							oblast.bottom=(pom_element->citelna_oblast.rect7.top+pom_element->citelna_oblast.rect7.bottom)/2;

						}

						//hledání popisku začátku
						oblast_pom=pom_element->citelna_oblast.rect6;
						if(oblast_pom.left<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.right && oblast_pom.top<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.bottom){JID=-14;break;}
						if(oblast_pom.right<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.left && oblast_pom.top<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.bottom){JID=-14;break;}
						if(oblast_pom.left<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.right && oblast_pom.bottom<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.top){JID=-14;break;}
						if(oblast_pom.right<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.left && oblast_pom.bottom<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.top){JID=-14;break;}
						//hledání popisku konce
						oblast_pom=pom_element->citelna_oblast.rect7;
						if(oblast_pom.left<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.right && oblast_pom.top<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.bottom){JID=-13;break;}
						if(oblast_pom.right<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.left && oblast_pom.top<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.bottom){JID=-13;break;}
						if(oblast_pom.left<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.right && oblast_pom.bottom<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.top){JID=-13;break;}
						if(oblast_pom.right<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast_pom.left && oblast_pom.bottom<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast_pom.top){JID=-13;break;}
						//hledání, zda jsem v oblasti tojúhelníku, nelze použít PtInRect, vracel nekorektní výsledky pro určité orientace
						if(oblast.left<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast.right && oblast.top<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast.bottom){JID=-12;break;}
						if(oblast.right<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast.left && oblast.top<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast.bottom){JID=-12;break;}
						if(oblast.left<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast.right && oblast.bottom<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast.top){JID=-12;break;}
						if(oblast.right<=akt_souradnice_kurzoru_PX.x && akt_souradnice_kurzoru_PX.x<=oblast.left && oblast.bottom<=akt_souradnice_kurzoru_PX.y && akt_souradnice_kurzoru_PX.y<=oblast.top){JID=-12;break;}
					}
					//přesun na další element
					pom_element=d.v.dalsi_krok(VYHYBKY,pom_element,OBJEKT_akt);
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);//mazání průchodového spojáku
				if(JID==-1)pom_element=NULL;
			}
			//jiný objekt než objekt editovaný
			if(JID==-1)
  		{
  			Cvektory::TObjekt *O=d.v.PtInObjekt();
  			if(O!=NULL && O->n!=OBJEKT_akt->n)JID=-203;
				O=NULL;delete O;
			}
		}
		if(MOD==LAYOUT && !d.v.PP.zamek_layoutu)//mod LAYOUT - stav odemčeno, zjišťování jidů
		{
			/////////////JID udává pouze akci, není třeba aby se k němu přičítalo i číslo bodu, bod je držen jako ukazatel pom_bod/////////////
  		//-102; citelná oblast zprávy
  		//-6; název objektu
  		//-2; hodnota kóty bodu (přímka [A,B] uložena v bodě B)
  		//-1=NIC!!!!!!!!!!!!!!!
			//0; bod haly nebo objektu
  		//1; úsečka haly nebo objektu
  		//2; oblast kóty bodu (přímka [A,B] uložena v bodě B)
			//3; oblas objektu
			//4; hrana objektu
			//5; element v objektu
			//6; výhybka nebo spojnice mezi výhybkou a spojoku
			d.zprava_highlight=d.v.PtInZpravy();
			if(d.zprava_highlight>0)JID=-102;//hledání citelné oblasti zprávy
      //hledání citelných oblastí elementů pro otevírání náhledu (element mimo kabinu),!!!!!!!!!!!!!!!!!!!!!!způsobí zamrzání (nově předěláno - sledovat)!!!!!!!!!!!!!!!!!!!
	  	if(JID==-1)
	  	{
	  		pom_element=NULL;
	  		Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
				Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	  		while(E!=NULL)
	  		{
          //hledání zda nejsem na vedlejší větvi výhybky
					if(E->eID==300 && E->dalsi2==E->predchozi2 && m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,E->geo.X4,E->geo.Y4,E->predchozi2->geo.X4,E->predchozi2->geo.Y4)<=0.5)
					{
						JID=6;
						pom_element=E;
					}
          //hledání zda jsem v oblasti elementu
					if(d.v.oblast_elementu(E,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
	  			{
						JID=5;
						pom_element=E;
						break;//pokud chci první nalezený element ... break, pokud chci poslední nalezeny bez breaku (např. problém překrývání 2 citelných oblastí)
					}
					E=d.v.dalsi_krok(VYHYBKY,E);
	  		}
	  		E=NULL;delete E;
	  		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
			}
			if(JID==-1 && d.v.OBJEKTY->dalsi!=NULL)//hledání nadpisu objektu
			{
  			Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
  			while(O!=NULL)
  			{
					if(najdi_nazev_obj(X,Y,O)){JID=-6;pom=O;break;}//název objektu
					O=O->dalsi;
				}
				O=NULL;delete O;
			}
  		if(JID==-1 && d.v.OBJEKTY->dalsi!=NULL && Akce==NIC)
			{
  			pom=NULL;pom_bod=NULL;
				pom=d.v.PtInObjekt();
				if(pom!=NULL)//byl nalezen objekt
  			{
					pom_bod=d.v.najdi_usecku(pom,5);
  				if(pom_bod!=NULL)JID=4;//nalezena hrana objektu
					else JID=3;//nalezen pouze objekt
  			}
			}
  		if(JID==-1 && (d.v.HALA.body!=NULL || (pom!=NULL && pom->body!=NULL)) && Akce==NIC)//má smysl pouze pokd existuje hala nebo objekt
			{
				pom_bod=d.v.najdi_bod(pom);//pokouším se najít bod
  			if(pom_bod!=NULL)JID=0;//bod nalezen
  			else//bod nenalezen, pokusí se najít úsečku
  	  	{
  				pom_bod=d.v.najdi_usecku(pom,5);//druhý parametr udává přesnost s jakou hledám, popřípadě velikost oblasti kolem úsečky
  				if(pom_bod!=NULL)JID=1;//usečka nalezena
  				else//usečka nenalezena, pokusí se najít kótu
  				{
  					short PtInKota_bod=d.v.PtInKota_bod(pom);//metoda vrací zda jsem v oblasti kóty nebo v její hodnotě + ukládá ukazatel na bod do pom_bod
  					if(PtInKota_bod==0 && pom_bod!=NULL)JID=2;//oblast kóty - posun kóty
  					if(PtInKota_bod==1 && pom_bod!=NULL)JID=-2;//hodnota kóty
  				}
				}
			}
		}
		//hledání citelné oblasti zprávy, vždy vyhledávat(i při zamčeném layoutu či editaci)!!
		if(JID==-1 && (d.v.PP.zamek_layoutu || OBJEKT_akt!=NULL && OBJEKT_akt->uzamknout_nahled))
		{
			d.zprava_highlight=d.v.PtInZpravy();
			if(d.zprava_highlight>0)JID=-102;
		}
	}

	////magnetické laso
	else
	{
		pom_element=NULL;
		if(d.v.MAG_LASO->Element!=NULL && d.v.MAG_LASO->sparovany!=NULL)//spouštět pouze v příadě, že se nejedná o 100% lineární měření, nespouštět na začátku
		{
			Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
      Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
			while(E!=NULL)
			{
        //nulování stavu, pokud se nejedná o počáteční bod přichycený na element
				//if(E!=d.v.MAG_LASO->sparovany || (d.v.MAG_LASO->Element->geo.X4!=d.v.MAG_LASO->sparovany->geo.X4 || d.v.MAG_LASO->Element->geo.Y4!=d.v.MAG_LASO->sparovany->geo.Y4))
				{
					E->stav=1;
          if(E->eID==0 && E->data.pocet_voziku==0)E->stav=0;
				}
				//kontrola zda jsem na segmentu
				if(d.v.PtInSegment(E,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
				{
					//pokud se změnit pom_element nebo je nulový ulož
					if(!(pom_element!=NULL && (pom_element->predchozi==d.v.MAG_LASO->predchozi->Element || (pom_element->predchozi->eID==301 && pom_element->predchozi->predchozi2==d.v.MAG_LASO->predchozi->Element) || (pom_element->predchozi->eID==301 && pom_element->predchozi->predchozi==d.v.MAG_LASO->predchozi->Element))) &&
						 !(pom_element!=NULL && (pom_element->dalsi==d.v.MAG_LASO->predchozi->Element || pom_element->dalsi2==d.v.MAG_LASO->predchozi->Element || (d.v.MAG_LASO->dalsi==NULL && d.v.MAG_LASO->sparovany==pom_element) || (pom_element->dalsi!=NULL && pom_element->dalsi->dalsi==d.v.MAG_LASO->predchozi->Element))))
					{
						pom_element=E;//uložení elementu, slouží pro přenos informace zda m.PtInSegmet je true
					}

          //kontorla + případné vložení elementu do spojáku mag lasa
					d.v.kontrola_vlozeni_do_mag_lasa(E);
				}
        //načtení dat ze zakázky do elementů, pro budoucí výpočty
        if(count_memo==0 && d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n!=0)//pokud pracuji v nějaké zakázce
				{
					count_memo++;
					Cvektory::TCesta *c=d.v.vrat_segment_cesty(d.v.ZAKAZKA_akt,E);
					if(c!=NULL)E->data=c->data;//přepsání aktuálních dat ze zakázky do elementu
					c=NULL;delete c;
				}
				E=d.v.dalsi_krok(VYHYBKY,E);//posun na další element, průchod celé linky
			}

			//ukazatelové záležitosti
			E=NULL;delete E;
			d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		}

    ////měření na časových dráhách
		unsigned int V=getV();//číslo vozíku
		if(V>0 && !(pocatek_mereni.x==-1 && pocatek_mereni.y==-1))
		{
			konec_mereni.x=akt_souradnice_kurzoru_PX.x;
			if(prichytavat_k_mrizce==1)konec_mereni.x=d.v.prichytit_cas_SIM(V);
			konec_mereni.y=V;
		}
    //testovací výpis obsahu mag. lasa
//		Cvektory::TCesta *C=d.v.MAG_LASO->dalsi;Memo_testy->Clear();     int pocet=0;
//		if(d.v.MAG_LASO->sparovany!=NULL)Memo("sparovany0: "+d.v.MAG_LASO->sparovany->name);
//		while(C!=NULL)
//		{         pocet++;
//			Memo(C->Element->name);
//			C=C->dalsi;
//		}
//		delete C;C=NULL;  Memo("pocet = "+String(pocet));

		//přichycení souřadnic na pohon, až na konec před vykreslením, aby první element neovlivnil hledání dalších elementu
		akt_souradnice_kurzoru=d.v.bod_na_geometrii(pom_element);//přichycení souřadnic na pohon, užitečné do budoucna
		//kontrola zda nejsem kurzorem na výchozím bodě měření, pokud ano smazat záznamy
		if(pom_element!=NULL && d.v.MAG_LASO!=NULL && d.v.MAG_LASO->dalsi!=NULL && d.v.MAG_LASO->sparovany==pom_element)d.v.smaz_segment_MAG_LASA(d.v.MAG_LASO->dalsi->Element);
		//vykreslení měřidla, pouze v případě, že mám nadefinovaný první bod
		//if(d.v.MAG_LASO->Element!=NULL)
		if(!Timer_simulace->Enabled)REFRESH();//slouží k vykreslení a překreslení mag. lasa, pouze v případě, že není spuštěna simulace
	}
	//pouze na test zatížení Memo3->Visible=true;Memo3->Lines->Add(s_mazat++);
}
//---------------------------------------------------------------------------
//dle místa kurzoru a vrácené JID (job id) nastaví úlohu
void TForm1::setJobIDOnMouseMove(int X, int Y)
{
	log(__func__);//logování
	if(MOD==EDITACE)
	{
		refresh_mGrid=true;//defaultně se tabulky refreshují, zakázání refreshe se provádí při určitých úkonech, toto zajistí zapnutí refreshe po ukončení úkonu při kterém byl tento refresh potlačen
		if(pom_element!=NULL)//ODSTRANĚNÍ předchozí případného highlightnutí elementu či tabulky
		{
			if(JID==0 || JID==1){pom_element->stav=1;refresh_mGrid=true;}//ELEMENT
			try//může se stát, že k tomuto dojde v době načítání z datového obrazu ... proto ošetření
			{pom_element->mGrid->Highlight=false;}//tabulka zrušení highlightnutí
			catch(...){;}
		}     
		int puvJID=JID;//záloha původního JID
		int puvVV=FormX->vykresli_vetev;
		Cvektory::TElement *pom_element_puv=pom_element;//pouze ošetření, aby neproblikával mGrid elementu, při přejíždění přes element
		Cvektory::TKomora *pom_komora_puv=pom_komora;
		getJobID(X,Y);//zjištění aktuálního JID
		if(puvJID!=JID || pom_element_puv!=pom_element || pom_komora_puv!=pom_komora)//pokud došlo ke změně JID, nebo změně elementu bez změny JID (např. situace dva roboti vedle sebe nebo rychlý přesun), jinak nemá smysl řešit
		{
  		//výchozí nastavení
			kurzor(standard);//umístít na začátek
			PmGCheckLink=false;
			StopCheckLink=false;

			////volání akce dle JID            //toto bez otestovaní
  		if(JID==-1 || pom_element==NULL || pom_element_puv!=pom_element)//není již job ID nebo ukazatel na pohon (může nastat situace přechod tabulka citelná oblast kót či nadpisu kabiny a nic by se bez tohoto nestalo
			{
				try//může se stát, že k tomuto dojde v době načítání z datového obrazu ... proto ošetření
				{
					if(pom_element_puv!=NULL && OBJEKT_akt->zobrazit_mGrid)pom_element_puv->mGrid->MouseMove(X,Y);//najistotu zrušení hintů a highlignutí odkazu normálních tabulek dodáním pouze aktuálních souřadnic
					if(puvJID>=4 && puvJID<=12 && PmG!=NULL && OBJEKT_akt->zobrazit_mGrid)PmG->MouseMove(X,Y);//najistotu hintů a zrušení highlignutí tabulky pohonu dodáním pouze aktuálních souřadnic
      	}catch(...){;}
			}
			if(JID==-102){if(d.zprava_highlight==d.zobrazit_celou_zpravu)kurzor(close);else kurzor(info);}//zprávy
			if(JID==0&&OBJEKT_akt->id!=3){if(pom_element->geo.orientace-pom_element->geo.rotacni_uhel==90||pom_element->geo.orientace-pom_element->geo.rotacni_uhel==270)kurzor(zmena_d_x);else kurzor(zmena_d_y);pom_element->stav=2;refresh_mGrid=false;}//posun ELEMENT
			if(JID==0&&OBJEKT_akt->id==3){kurzor(posun_ind);}//posun komory
			if(JID==1){kurzor(editace_posun);pom_element->stav=3;refresh_mGrid=false;}//ELEMENT název
  		//použit závěrečný REFRESH if(pom_eleF->bod_na_geometrii(0,0,Element)ment!=pom_element_puv && (puvJID==0 || JID==0)/* || (puvJID==0 && JID==1) || (puvJID==1 && JID==0)*/){REFRESH();}//důvod k REFRESH, pouze v případě změny elementu či přechodu z názvu na celý element a opačně
  		//použit závěrečný REFRESH if(10<JID && JID<1000){REFRESH();}//hodnota kóty
			//if(JID==100){kurzor(edit_text);pom_element->mGrid->CheckLink(X,Y);refresh_mGrid=true;}//název elementu v hlavičce tabulky - aktivace dodáním pouze aktuálních souřadnic
  		if(JID==1000){pom_element->mGrid->CheckLink(X,Y);refresh_mGrid=true;}//pouze pro přechod název hlavička, aby název nezůstal tučně - aktivace dodáním pouze aktuálních souřadnic
			if(1000<=JID && JID<2000 || JID>2000 && JID<3000 || JID==100){kurzor(posun_ind);pom_element->mGrid->Highlight=true;pom_element->mGrid->MouseMove(X,Y);refresh_mGrid=true;}//indikace posunutí TABULKY, jeji highlignutí probíhá výše a případné volání HINTu
			if(100<JID && JID<1000){kurzor(zmena_j);pom_element->mGrid->CheckLink(X,Y);refresh_mGrid=true;}//první sloupec tabulky, libovolný řádek, v místě, kde je ODKAZ  - aktivace dodáním pouze aktuálních souřadnic
			if(JID==-6)kurzor(editace_posun);
			if(JID==-7)kurzor(posun_ind);//tělo teploměru
			if(JID==-8)kurzor(posun_editace_obj);//popisek teploměru
			if((JID==-11 || JID==-13 || JID==-14 || JID==-101) && Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU){kurzor(edit_text);refresh_mGrid=false;}//kurzor pro editaci textu, kóty + S/K, koty mezi LO
			if(JID==-9 && d.v.PtInKota_komory(OBJEKT_akt,X,Y)==-1){if(OBJEKT_akt->orientace==90||OBJEKT_akt->orientace==270)kurzor(zmena_d_x);else kurzor(zmena_d_y);}//změna rozměru komory
			//použit závěrečný REFRESH if(-9<=JID && JID<=-6){REFRESH();}//refresh při akci s nadpisem či kótou kabiny
			if(JID==-10){/*REFRESH();*/kurzor(zmena_j);}//indikace možnosti změnit jednotky na kótách
			if(JID==13){if(OBJEKT_akt->orientace==90||OBJEKT_akt->orientace==270)kurzor(zmena_d_y);else kurzor(zmena_d_x);refresh_mGrid=false;}//interaktivní kóty elementů
			if(JID>=5 && JID<=12){kurzor(zmena_j);if(PmG->CheckLink(X,Y)!=TPoint(-1,-1));refresh_mGrid=true;PmG->Refresh();}//pohonová tabulka odkazy - aktivace dodáním pouze aktuálních souřadnic
			if(JID==4 && PmG!=NULL){PmG->Highlight=true;kurzor(posun_ind);}//highlightování tabulky pohonů při jejím přejetí
			if(JID==-2)//posun úsečky objektu
			{
				refresh_mGrid=false;
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else A=OBJEKT_akt->body->predchozi;
				//zjištění azimutu úsečky + nastavení kurzoru
				if(A->X==B->X)kurzor(zmena_d_x);
				else if(A->Y==B->Y)kurzor(zmena_d_y);
				//úsečka a kóta se posouvají rozdílně, proto jiné kurzory
				else kurzor(posun_ind);
				//smazání pomocných ukazatelů
				A=NULL;B=NULL;delete A;delete B;
			}
			if(JID==-3){kurzor(posun_ind);refresh_mGrid=false;}//kurzor bodu objektu
			if(JID==-4)//oblast kóty objektu
			{
				refresh_mGrid=false;
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else A=OBJEKT_akt->body->predchozi;
				//azimut kóty
				float azimut=fmod(m.azimut(A->X,A->Y,B->X,B->Y),360);
				if(azimut==90||azimut==270)kurzor(zmena_d_y);
				else if(azimut==0||azimut==180)kurzor(zmena_d_x);
				else if((azimut>0&&azimut<90)||(azimut>180&&azimut<270))kurzor(posun_ind_pd);
				else if((azimut>270&&azimut<360)||(azimut>90&&azimut<180))kurzor(posun_ind_ld);
				//smazání pomocných ukazatelů
				A=NULL;B=NULL;delete A;delete B;
			}
			if(JID==-5){kurzor(edit_text);refresh_mGrid=false;}//hodnota kóty objektu
			if(JID==-201 || JID==-202)kurzor(zmena_j);//provizorní
			if(JID==-203)kurzor(editace_obj);//editace jiného objektu

			////inteligentní REFRESH
  		//if(!refresh_mGrid/* && !nabuffrovano*/){d.nabuffrovat_mGridy();nabuffrovano=true;}
  		//d.nabuffrovat_mGridy(pom_element->mGrid);
			//if(!refresh_mGrid)Memo("false");else Memo("true");
			if(element_id!=-1 && Akce !=ADD){refresh_mGrid=true;element_id=-1;}//mazání eid po vkládání, podmínka nutná ke správnému vykreslení mgridů

			//refreshování pouze pro překreslení spojnice mezi mgridy
			Cvektory::TElement *E=pom_element;
			if(E==NULL)E=pom_element_puv;
			if((pom_element_puv!=NULL && pom_element==NULL) || (pom_element_puv==NULL && pom_element!=NULL) && !(JID<=-11 && JID>=-100) && !(JID<=99 && JID>=13) && (d.v.vrat_druh_elementu(E)!=-1 || E->eID==200 || E->eID==300 || E->eID==301 || E->eID==400 || E->eID==401 || E->eID==402) && E->eID!=100)
				REFRESH();
			E=NULL;delete E;
			if(JID==-6 || JID==-3 || JID==-2 || puvJID==-6 || puvJID==-3 || puvJID==-2)REFRESH(false);//highlighty kabiny
		}
		if(puvVV!=FormX->vykresli_vetev)REFRESH();
		if(puvJID==-1 && JID==-1 && PmG!=NULL && PmG->Highlight){PmG->Highlight=false;REFRESH();}//odstranění highlightu tab pohonu pokud je třeba, zamezuje problikávání highlightu
		if(JID==puvJID && (JID==-2 || JID==-3))REFRESH(false);//highlighty kabiny
  	////oblasti poznámek pod čarou - NOTE, nejdou přes JID
//  	if(JID==-1)
//		{                                                       //zajištuje unhighlight odkazu
//			if(PmG!=NULL)if(PmG->CheckLink(X,Y)==TPoint(-2,-2) || PmGCheckLink){PmGCheckLink=true;refresh_mGrid=true;REFRESH();}
//			if(FormX->check_click_Note(X,Y,true) || StopCheckLink){StopCheckLink=true;refresh_mGrid=true;REFRESH();}
//			//pokud budu chtít ještě získat CheckLink z tabulek elementů budu muset rozšířit testování oblasti tabulky a aby vrátila platný ukazatel na element resp. jeho mgrid
//		}

		if(d.zprava_highlight>0 && Form_zpravy->Visible)Form_zpravy->highlight(d.zprava_highlight-1);//highlight řádků v miniformu zprávy

		////vynulování a odstranění pomocného ukazatele na element
		pom_element_puv=NULL;delete pom_element_puv;
		pom_komora_puv=NULL;delete pom_komora_puv;
	}
	if(MOD==LAYOUT && !d.v.PP.zamek_layoutu)//pro schéma, změna kurzorů pro body a úsečky
	{
		int puvJID=JID;//záloha původního JID
		Cvektory::TBod *pom_bod_puv=pom_bod;
		Cvektory::TObjekt *predchozi_pom=pom;
		getJobID(X,Y);//zjištění aktuálního JID
		if(puvJID!=JID || pom_bod_puv!=pom_bod)//pokud došlo ke změně JID, nebo změně bodu bez změny JID, jinak nemá smysl řešit
		{
			long scena=1111111;//ZprVozEledElesDopObjHal
			//kurzory (provizorně), ...
			kurzor(standard);
			if(JID==-102){if(d.zprava_highlight==d.zobrazit_celou_zpravu)kurzor(close);else kurzor(info);scena=2111111;}//zprávy
			if(JID==-6){kurzor(posun_editace_obj);/*scena=1111121;*/}//posun nadpisu objektu / editace objektu
			if(JID==-2){kurzor(edit_text);scena=1111112;}//hodnota kóty
			if(JID==0){kurzor(posun_ind);if(pom==NULL)scena=1111112;/*else scena=1111121;*/}//posun bodu
			if(JID==3)kurzor(posun_editace_obj);//oblast objektu, není vysvěcován
			if(JID==5 || JID==6)kurzor(editace_obj);//element mimo objekt - pouze editace
			if(JID==1||JID==4)//posun úsečky
			{
				if(pom==NULL)scena=1111112;//else scena=1111121;
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else if(pom!=NULL&&pom->body!=NULL)A=pom->body->predchozi;
				else A=d.v.HALA.body->predchozi;
				//zjištění azimutu úsečky + nastavení kurzoru
				if(m.round2double(A->X,2)==m.round2double(B->X,2))kurzor(zmena_d_x);
				else if(m.round2double(A->Y,2)==m.round2double(B->Y,2))kurzor(zmena_d_y);
				//úsečka a kóta se posouvají rozdílně, proto jiné kurzory
				else kurzor(posun_ind);
				//smazání pomocných ukazatelů
				A=NULL;B=NULL;delete A;delete B;
			}
			if(JID==2)//oblast kóty, kóty zobrazeny pouze pro halu
			{
				scena=1111112;
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else if(pom!=NULL&&pom->body!=NULL)A=pom->body->predchozi;
				else A=d.v.HALA.body->predchozi;
				//azimut kóty
				float azimut=fmod(m.azimut(A->X,A->Y,B->X,B->Y),360);
				if(azimut==90||azimut==270)kurzor(zmena_d_y);
				else if(azimut==0||azimut==180)kurzor(zmena_d_x);
				else if((azimut>0&&azimut<90)||(azimut>180&&azimut<270))kurzor(posun_ind_pd);
				else if((azimut>270&&azimut<360)||(azimut>90&&azimut<180))kurzor(posun_ind_ld);
				//smazání pomocných ukazatelů
				A=NULL;B=NULL;delete A;delete B;
			}
			if((pom!=NULL && pom!=predchozi_pom) || (pom==NULL && predchozi_pom!=NULL) || JID==6)vytvor_statickou_scenu();
			REFRESH(scena,false);
		}

		if(d.zprava_highlight>0 && Form_zpravy->Visible)Form_zpravy->highlight(d.zprava_highlight-1);//highlight řádků v miniformu zprávy
		pom_bod_puv=NULL;delete pom_bod_puv;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//nastavení zobrazení popUPmenu a jeho volání včetně pozice
void TForm1::onPopUP(int X, int Y)
{
	log(__func__);//logování
	if(Screen->Cursor!=standard)kurzor(standard);//ošetření proti zůstávání kurzoru
	//výchozí skrytí všech položek, další postup je založen na postupném odkrývání a zvětšování panelu UP nebo DOWN
	close_all_items_popUPmenu();
	PopUPmenu->Item_zobrazit_parametry->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
	//dle statusu Architek x Klient resp. Návrh x Ověrování
	AnsiString N,otocit_doleva,otocit_doprava;
	N=ls->Strings[164];if(STATUS==OVEROVANI)N=ls->Strings[163];//"  Editovat","  Zobrazit"
	AnsiString smazat=ls->Strings[170];//,kopirovat=ls->Strings[169];
	//rotace názvu
	if(OBJEKT_akt!=NULL || JID==-6){otocit_doleva=ls->Strings[404];otocit_doprava=ls->Strings[405];}
	else {otocit_doleva=ls->Strings[173];otocit_doprava=ls->Strings[174];}
	//caption se musí přepsat takto !!
	PopUPmenu->scLabel_otocit_doleva->Caption=otocit_doleva;
	PopUPmenu->scLabel_otocit_doprava->Caption=otocit_doprava;
	//dle modu zobrazí položky, pozor záleží zvláštně!!! na pořadí položek
	switch(MOD)
	{
//		case CASOVAOSA:
//		{														 //min                      //vozik
//			proces_pom=d.v.najdi_proces((X+d.PosunT.x)/d.PX2MIN*60,ceil((Y+d.PosunT.y-d.KrokY/2-scGPPanel_mainmenu->Height)/(d.KrokY*1.0)));//vrací nalezen proces, proces_pom se využívá ještě dále
//			PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
//			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
//			if(proces_pom!=NULL && !d.mod_vytizenost_objektu)
//			{
//				PopUPmenu->Item_zobrazit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;//nastavení zobrazení
//				PopUPmenu->Item_zobrazit_parametry->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
//				PopUPmenu->Item_rychly_export->Visible=true;PopUPmenu->Panel_UP->Height+=34;
//				if(STATUS==NAVRH)//měnit parametry na časových osách je možné pouze v návrháři/architektovi
//				{
//					//pom=proces_pom->segment_cesty->objekt;
//					if(AnsiString(N+" "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
//					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom->name.UpperCase();
//					else
//					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom->name.UpperCase();
//					PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
//				}
//			}
//		}break;
//		case TECHNOPROCESY:
//		{
//			pom=d.v.vrat_objekt_z_roma(akt_souradnice_kurzoru_PX.x-d.Xofset+d.PosunT.x);
//			if(pom!=NULL)
//			{
//				PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
//				PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
//				//PopUPmenu->Item_rychly_export->Visible=true;PopUPmenu->Panel_UP->Height+=34;
//				if(AnsiString(N+" "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
//				PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom->name.UpperCase();
//				else
//				PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom->name.UpperCase();
//				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
//			}
//		}
//		break;
		//case SIMULACE:break;
		case EDITACE:
		{
			pom_vyhybka=d.v.PtInObjekt();//využití prázdného ukazatele pro uchovávání nalezeného objektu (přechod z náhledu do náhledu)
			//vypisování editace objektu vždy
			if(pom_vyhybka!=NULL && pom_vyhybka!=OBJEKT_akt)PopUPmenu->Item_nastavit_parametry->Visible=true;
			if(pom_vyhybka==NULL)pom_vyhybka=OBJEKT_akt;
			pom_element_temp=pom_element;
			mazani=true;
			////////nastavení horního panelu
			//základní nastavení captionu pro tlačítko nastavit
			if(AnsiString(N+" "+pom->name).Length()>19)PopUPmenu->scLabel_nastavit_parametry->Caption=N+"\n  "+pom_vyhybka->name.UpperCase();
			else PopUPmenu->scLabel_nastavit_parametry->Caption=N+" "+pom_vyhybka->name.UpperCase();
			//smazání elementu, vynechat element teploměrů (nelze smazat uživatelsky)
			if(pom_element!=NULL && JID!=-7 && (pom_element->eID!=200 || (pom_element->eID==200 && ((pom_element->dalsi!=NULL && pom_element->dalsi->pohon==pom_element->pohon) || (pom_element->dalsi==NULL && d.v.ELEMENTY->dalsi->pohon==pom_element->pohon)))))//Pokud bylo kliknuto na element + ošetření pom_element je používan i při tvorbě geometrie + nejedná se o předávací místo
			{
				if(AnsiString(N+" "+pom_element->name).Length()>19) PopUPmenu->scLabel_smazat->Caption=smazat+"\n  "+pom_element->name.UpperCase();
				else PopUPmenu->scLabel_smazat->Caption=smazat+" "+pom_element->name.UpperCase();
        //mazání S/K
				if(pom_element->geo.HeightDepp>0 && (JID==-12 || JID==-13 || JID==-14))PopUPmenu->scLabel_smazat->Caption=smazat+" "+ls->Strings[490];
				if(pom_element->geo.HeightDepp<0 && (JID==-12 || JID==-13 || JID==-14))PopUPmenu->scLabel_smazat->Caption=smazat+" "+ls->Strings[491];
				PopUPmenu->Item_nastavit_parametry->Visible=false;
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;
			}
			//změna typu komory
			if(pom_komora!=NULL)
			{
				pom_komora_temp=pom_komora;
				PopUPmenu->scLabel_nastavit_parametry->Caption=ls->Strings[165];//"  Změnit typ sekce";
				PopUPmenu->scLabel_smazat->Caption=ls->Strings[171]+" "+AnsiString(pom_komora->n);//"  Smazat sekci č. "
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;
			}
			//mazání a přidávání bodu do polygonu obrysu kabiny
			if(OBJEKT_akt->id!=3 && pom_bod!=NULL && JID==-3)//bod = mazání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_smazat->Caption=ls->Strings[172]+" "+AnsiString(pom_bod->n);//"  Smazat bod č. "
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;
			}
			//přidávání bodu
			if(OBJEKT_akt->id!=3 && pom_bod!=NULL && (JID==-2 || JID==-3))//usečka = přidávání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_nastavit_parametry->Caption=ls->Strings[166];//"  Přidat bod";
				PopUPmenu->Item_nastavit_parametry->Visible=true;
			}
			//vkládání zarážky při geometrii
			if(Akce==GEOMETRIE && bod_vlozeni_elementu().x!=-1000)PopUPmenu->scLabel_nastavit_parametry->Caption=ls->Strings[167];//"  Vložit zarážku";}
      //skrývání, zobrazování stěn objektu
			if(pom_vyhybka!=NULL && pom_vyhybka==OBJEKT_akt && (OBJEKT_akt->id==0 || OBJEKT_akt->id==9 || OBJEKT_akt->id==12))
			{
				if(OBJEKT_akt->sirka_steny==0)PopUPmenu->scLabel_zobrazitskryt_steny->Caption=ls->Strings[479];
				else PopUPmenu->scLabel_zobrazitskryt_steny->Caption=ls->Strings[480];
				PopUPmenu->Item_zobrazitskryt_steny->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_zobrazitskryt_steny->Visible=true;
			}
			//posun obrysu
			if(OBJEKT_akt->id!=3 && (pom_bod!=NULL || (pom_vyhybka!=NULL && pom_vyhybka->n==OBJEKT_akt->n)))PopUPmenu->Item_posun_obrysu->Visible=true;
			//rotace textu kabiny
			if(JID==-6)
			{
				PopUPmenu->Item_otocit_doleva->Visible=true;
				PopUPmenu->Item_otocit_doprava->Visible=true;
			}
      //pokud není zobrazeno tlačítko editace náhledu zobrazí se
			//if(!PopUPmenu->Item_nastavit_parametry->Visible){PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
			////
      ////////nastavení výšek horního panelu
			unsigned int i=0;
			if(PopUPmenu->Item_kopirovat->Visible)i++;
			if(PopUPmenu->Item_nastavit_parametry->Visible)i++;
			if(PopUPmenu->Item_otocit_doleva->Visible)i++;
			if(PopUPmenu->Item_otocit_doprava->Visible)i++;
			if(PopUPmenu->Item_posun_obrysu->Visible)i++;
			if(PopUPmenu->Item_rychly_export->Visible)i++;
			if(PopUPmenu->Item_smazat->Visible)i++;
			if(PopUPmenu->Item_zobrazit_parametry->Visible)i++;
			if(PopUPmenu->Item_zobrazitskryt_steny->Visible)i++;
			PopUPmenu->Panel_UP->Height+=i*34;
			////////nastavení spodního panelu
			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_priblizit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_oddalit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_vybrat_oknem->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_cely_pohled->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			if(OBJEKT_akt->zobrazit_mGrid){PopUPmenu->Item_rozmistit_mgridy->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;}
			break;
		}  
		case LAYOUT://pro LAYOUT
		{
			//povoluje nastavení položek kopírování či smazání objektu
//			pom=d.v.najdi_objekt(m.P2Lx(X),m.P2Ly(Y),d.O_width*m2px,d.O_height*m2px);
//			pom_bod=d.v.najdi_bod(pom);
			//element mimo kabinu
			if(pom==NULL && (JID==5 || JID==6) && pom_element!=NULL)pom=d.v.vrat_objekt(pom_element->objekt_n);
			if(d.v.PP.zamek_layoutu)pom=d.v.PtInObjekt();
			pom_vyhybka=pom;//uchovávání ukazatele pro případ, že uživatel po zobrazení popup menu sjede kurzorem z objektu
			pom_element_temp=NULL;
			if(pom!=NULL && JID!=-6)// nelze volat přímo metodu najdi objekt, protože pom se používá dále
			{
				if(AnsiString(ls->Strings[164]+" "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
				{
					PopUPmenu->scLabel_nastavit_parametry->Caption=N+"\n  "+pom->name.UpperCase();
					//PopUPmenu->scLabel_kopirovat->Caption=kopirovat+"\n  "+pom->name.UpperCase();
					if(d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0)PopUPmenu->scLabel_smazat->Caption=smazat+"\n  "+pom->name.UpperCase();
				}
				else
				{
					PopUPmenu->scLabel_nastavit_parametry->Caption=N+" "+pom->name.UpperCase();
					//PopUPmenu->scLabel_kopirovat->Caption=kopirovat+" "+pom->name.UpperCase();
					if(d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0)PopUPmenu->scLabel_smazat->Caption=smazat+" "+pom->name.UpperCase();
				}
				if(!d.v.PP.zamek_layoutu)
				{
			  	PopUPmenu->Item_otocit_doleva->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			  	PopUPmenu->Item_otocit_doprava->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			  	//pozor rozhoduje pořadí
			  	if(d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0)PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
			  	if(d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0){PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
				}
				if((long)pom->id!=VyID&&(long)pom->id!=pocet_objektu_knihovny+1)
				{PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
			}
			if(pom_bod!=NULL && JID==0)//bod = mazání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_smazat->Caption=ls->Strings[172]+" "+AnsiString(pom_bod->n);//"  Smazat bod č "
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_bod!=NULL && JID==1)//usečka = přidávání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_nastavit_parametry->Caption=ls->Strings[166];//"  Přidat bod";
				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(!d.v.PP.zamek_layoutu && (pom_bod!=NULL || d.v.PtInBody()!=NULL || pom!=NULL)){PopUPmenu->Item_posun_obrysu->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
			if((JID==5 || JID==6) && pom_element!=NULL && (pom_element->eID==300 || pom_element->eID==301))//nabízení možnosti odstranění výhybky nebo spojky
			{
        pom_element_temp=pom_element;//uložení pro případ ztráty ukazatele při pohybu kurzorem
        //pom_vyhybka=NULL;//odstrannění uloženého ukazatele, usnadnění pro metodu Smazat1Click
				if(AnsiString(N+" "+pom_element->name).Length()>19) PopUPmenu->scLabel_smazat->Caption=smazat+"\n  "+pom_element->name.UpperCase();
				else PopUPmenu->scLabel_smazat->Caption=smazat+" "+pom_element->name.UpperCase();
			}
			//rotace textu kabiny
			if(JID==-6)
			{
				PopUPmenu->Item_otocit_doleva->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				PopUPmenu->Item_otocit_doprava->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			//skrývání, zobrazování stěn objektu
			if(pom_vyhybka!=NULL && !d.v.PP.zamek_layoutu)// && (pom_vyhybka->id==0 || pom_vyhybka->id==9 || pom_vyhybka->id==12))
			{
				if(pom_vyhybka->sirka_steny==0)PopUPmenu->scLabel_zobrazitskryt_steny->Caption=ls->Strings[479];
				else PopUPmenu->scLabel_zobrazitskryt_steny->Caption=ls->Strings[480];
				PopUPmenu->Item_zobrazitskryt_steny->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_zobrazitskryt_steny->Visible=true;PopUPmenu->Panel_UP->Height+=34;
      }
			//zobrazení běžných položek, pozor rozhoduje pořadí
			//PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_priblizit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_oddalit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_vybrat_oknem->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_cely_pohled->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
		}break;
		default:
		{
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
	PopUPmenu->Left=Form1->Left+akt_souradnice_kurzoru_PX.x; //musí být top a left formu, z důvodu více obrazovek
	PopUPmenu->Top=Form1->Top+akt_souradnice_kurzoru_PX.y;
	//volání vlastního popup menu + ošetření, pokud je mimo obrazovku
	PopUPmenu->Show();
}
//---------------------------------------------------------------------------
//zajistí skrýtí všech položek popUPmenu
void TForm1::close_all_items_popUPmenu()
{
	log(__func__);//logování
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
  //nové
	PopUPmenu->Item_otocit_doleva->Visible=false;
	PopUPmenu->Item_otocit_doprava->Visible=false;
	PopUPmenu->Item_posun_obrysu->Visible=false;
	PopUPmenu->Item_zobrazitskryt_steny->Visible=false;
  PopUPmenu->Item_rozmistit_mgridy->Visible=false;

	PopUPmenu->Panel_UP->Height=0;
	PopUPmenu->Panel_DOWN->Height=0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Piblit1Click(TObject *Sender)
{
	log(__func__);//logování
	ZOOM_IN();
	zobraz_tip(ls->Strings[298]);//"TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru."
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Priblizit2Click(TObject *Sender)
{
	log(__func__);//logování
	ZOOM_IN();
	zobraz_tip(ls->Strings[298]);//"TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru."
}
void __fastcall TForm1::RzToolButton8Click(TObject *Sender)//Zoom in z toolbaru
{
 log(__func__);//logování
 vycentrovat=false;
 akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
 ZOOM_IN();
 zobraz_tip(ls->Strings[298]);//"TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru."
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Oddlit1Click(TObject *Sender)
{
	log(__func__);//logování
	ZOOM_OUT();
	zobraz_tip(ls->Strings[299]);//"TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu."
}
void __fastcall TForm1::Oddalit2Click(TObject *Sender)
{
	log(__func__);//logování
	ZOOM_OUT();
	zobraz_tip(ls->Strings[299]);//"TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu."
}
void __fastcall TForm1::RzToolButton9Click(TObject *Sender)//Zoom out z toolbaru
{
	log(__func__);//logování
	vycentrovat=false;
	akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
	ZOOM_OUT();
	zobraz_tip(ls->Strings[299]);//"TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu."
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//přiblížení
void TForm1::ZOOM_IN()
{
	log(__func__);//logování
	if(Zoom==0.25)//při odchodu z 0.25
	{
			Uloz_predchozi_pohled();
			Zoom=0.5;
			ZOOM();
	}
	else
	if((Zoom<20 && !DEBUG) || (Zoom<30 && DEBUG)) //max
	{
		Uloz_predchozi_pohled();
		if(Zoom>=10) Zoom+=5.0;
		else if(Zoom>=5) Zoom+=1.0;
		else Zoom+=0.5;
		ZOOM();
	}
}
//---------------------------------------------------------------------------
//oddálení
void TForm1::ZOOM_OUT()
{
	log(__func__);//logování
//	if(MOD==EDITACE)
//	{
//		if(Zoom>=3)zobrazeni_tabulek=OBJEKT_akt->zobrazit_mGrid;
//	}
	if(Zoom>0.5)
	{
		Uloz_predchozi_pohled();
		if(Zoom>10) Zoom-=5.0;
		else if(Zoom>5) Zoom-=1.0;//zvětšení skoku
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
	log(__func__);//logování
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
		vytvor_statickou_scenu();//vytvoří BMP s novou statickou scénou, musí být před REFRESH()
		REFRESH();
		DuvodUlozit(true);
	}
}
//---------------------------------------------------------------------------
void TForm1::ZOOM_WINDOW()
{
	log(__func__);//logování
	Uloz_predchozi_pohled();
	Akce=NIC;//zrušení pokračování akce, tzn. konec překreslování rozšiřování editačního okna
	zneplatnit_minulesouradnice();

	//zapamatovani si středu obrazovky dle vybrané oblasti (resp) jejího středu
	TPoint Centr;
	Centr.x=(m.P2Lx(vychozi_souradnice_kurzoru.x)+m.P2Lx(akt_souradnice_kurzoru_PX.x))/2;
	Centr.y=(m.P2Ly(vychozi_souradnice_kurzoru.y)+m.P2Ly(akt_souradnice_kurzoru_PX.y))/2;


	//zjištění (vyzkoušení) velikosti zoomu
//	float LHx=m.P2Lx(vychozi_souradnice_kurzoru.x);float LHy=m.P2Ly(vychozi_souradnice_kurzoru.y);float PDx=m.P2Lx(akt_souradnice_kurzoru_PX.x);float PDy=m.P2Ly(akt_souradnice_kurzoru_PX.y);
//	while(m.P2Lx(0)<=LHx && m.P2Ly(0)>=LHy && m.P2Lx(ClientWidth)>=PDx && m.P2Ly(ClientHeight)<=PDy)
//	{
//		if(Zoom>=4){Zoom=4;break;}
//		Zoom+=0.5;
//	}

  //deklarace
	int MaxX=akt_souradnice_kurzoru_PX.x,MaxY=akt_souradnice_kurzoru_PX.y,MinX=vychozi_souradnice_kurzoru.x,MinY=vychozi_souradnice_kurzoru.y;
	int PD_x=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
	int PD_y=ClientHeight-vyska_menu-scGPPanel_statusbar->Height-scGPPanel_mainmenu->Height;//-vyska_menu-RzStatusBar1->Height je navíc nemá tam co dělat
	TPointD centr;
	centr.x=m.P2Lx((MaxX+MinX)/2.0);
	centr.y=m.P2Ly((MaxY+MinY)/2.0);

	//výpočet nového Zoomu
	double Z1=abs(Zoom*PD_x/(MaxX-MinX)),Z2=abs(Zoom*PD_y/abs(MaxY-MinY));
	Z1=m.getZoomValue(Z1);Z2=m.getZoomValue(Z2);
	if(Z1>Z2)Zoom=Z2;else Zoom=Z1;
	if(Zoom<0.5)Zoom=0.5;
	if(Zoom>20 && !DEBUG)Zoom=20;
	if(Zoom>30 && DEBUG)Zoom=30;

	//vycentrování obrazu
	PD_x=(ClientWidth+scSplitView_LEFTTOOLBAR->Width)/2.0;
	PD_y=(ClientHeight+(scGPPanel_mainmenu->Height-scGPPanel_statusbar->Height))/2.0;
	Posun.x+=(m.L2Px(centr.x)-PD_x)/Zoom;
	Posun.y+=(m.L2Py(centr.y)-PD_y)/Zoom;

	//SB(Zoom,2);už se nepoužívá
	on_change_zoom_change_scGPTrackBar();
	vytvor_statickou_scenu();//vytvoří BMP se statickou scénou, musí být před REFRESH()
	REFRESH();
	//aktualizace_statusbaru(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
	//checkZoomMenuItems();//povolení či zakázní zoomu
	kurzor(standard);
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Predchozipohled1Click(TObject *Sender)
{
	log(__func__);//logování
	Zoom=Zoom_predchozi;
	Posun=Posun_predchozi;
	//Predchozipohled1->Enabled=false;
//	RzToolButton12->Enabled=false;
	REFRESH();
	DuvodUlozit(true);
}
void TForm1::Uloz_predchozi_pohled()
{
	log(__func__);//logování
	Zoom_predchozi=Zoom;
	Posun_predchozi=Posun;
	//Predchozipohled1->Enabled=true;
 //	RzToolButton12->Enabled=true;
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void TForm1::DOWN()//smer dolu
{
	log(__func__);//logování
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
	log(__func__);//logování
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
	log(__func__);//logování
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
	log(__func__);//logování
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
//vytvoří výřez pro pan_move
void TForm1::pan_create()
{
	log(__func__);//logování

	TRect oblast=vrat_max_oblast();
	short R=20;//nutná rezerva okraje v px
	long MaxX=oblast.right+R,MaxY=oblast.bottom+R,MinX=oblast.left-R,MinY=oblast.top-R;//pozor na znaménka Y
	float K=1.2;//koeficient zvětšení, 1.5 už méně efektivní, než pokrytím dokonalá (optimální) varianta 2
	short o=0;if(scSplitView_LEFTTOOLBAR->Visible)o=scSplitView_LEFTTOOLBAR->Width;
	if(MaxY-MinY>K*ClientHeight || MOD==EDITACE || 0+o<=MinX && MaxX<=ClientWidth && 0<=MinY && MaxY<=ClientHeight)//pokud by se jednalo o příliš velkou oblast nebo editaci nebo je daná oblast menší než zobrazovaná
	{
		//varianta 0) - původní varianta sreenu obrazovky, bez nutnosti AA - velice rychlá, bez nabuffrovaného přesahu, tj. při každém posunu je vidět bílá nepřipravená plocha + nevýhodou je obtisk všech tlačítek či měřítka
		panType=0;
		int W=scSplitView_LEFTTOOLBAR->Width;
		if(MOD==CASOVAOSA || MOD==TECHNOPROCESY)W=0;//zajistí, že se posová i číslování vozíků resp.celá oblast
		short H=scGPPanel_mainmenu->Height;
		int Gh=0;if(scGPPanel_bottomtoolbar->Visible)Gh=scGPPanel_bottomtoolbar->Height;Gh-=6;//WA, z nějaké důvodu to chce odebrat, aby byla posouváná plocha kompletní
		scGPButton_bug_report->Visible=false;
		Pan_bmp->Width=ClientWidth;Pan_bmp->Height=ClientHeight-H-Gh;//velikost pan plochy
		Pan_bmp->Canvas->CopyRect(Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh));//uloží pan výřez
		if(MOD!=SIMULACE)scGPButton_bug_report->Visible=true;
		//Pan_bmp->SaveToFile("test.bmp");  //pro testovací účely

	/////////////////////
	//varianta 1) pro vlákno - neví se kam se bude posun odehrávat, tj. vytvoří oblast 3x3 obrazovek, ve středu je daná aktuální
//	Zoom_predchozi_AA=Zoom;//záloha původního zoomu
//	TPointD Posun_predchozi=Posun;
//	Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
//	if(Pan_bmp_ALL!=NULL){delete Pan_bmp_ALL;Pan_bmp_ALL=NULL;}//před vytvořením nové kvůli realokaci nejdříve nutné odstranit
//	Posun.x-=ClientWidth/Zoom*3;Posun.y-=ClientHeight/Zoom*3;
//	Graphics::TBitmap *bmp_test=new Graphics::TBitmap;
//	bmp_test->Width=ClientWidth*3*3;bmp_test->Height=ClientHeight*3*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu, *3 z principu algoritmu (tři nabuffrované obrazovky, neví se kam, se bude obraz posouvat)
//	d.vykresli_vektory(bmp_test->Canvas,1);
//	Cantialising a; Pan_bmp_ALL=a.antialiasing(bmp_test,false);delete(bmp_test);//velice nutné do samostatné bmp_out, kvůli smazání bitmapy vracené AA
//	//Pan_bmp_ALL->SaveToFile("testALL.bmp");
//	Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
//	Posun=Posun_predchozi;
//	MessageBeep(0);

//	//varianta 2) pro použítí zde optimalizovaná, ale stále pomalá, předpokládá se v momentu kliku (nelze ji připravit předem např. na vlákně), kdy se připraví přípraví max. dostatečná oblast napravo a nalevo resp. nahoru o dolu oblast, kterou je maximálně možné zobrazit, nic navíc
//	Zoom_predchozi_AA=Zoom;//záloha původního zoomu
//	TPointD Posun_predchozi=Posun;
//	Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
//	if(Pan_bmp_ALL!=NULL){delete Pan_bmp_ALL;Pan_bmp_ALL=NULL;}//před vytvořením nové kvůli realokaci nejdříve nutné odstranit
//	Posun.x-=(ClientWidth-akt_souradnice_kurzoru_PX.x)/Zoom*3;Posun.y-=(ClientHeight-akt_souradnice_kurzoru_PX.y)/Zoom*3;
//	T.x=ClientWidth-akt_souradnice_kurzoru_PX.x;T.y=ClientHeight-akt_souradnice_kurzoru_PX.y;
//	Graphics::TBitmap *bmp_test=new Graphics::TBitmap;
//	bmp_test->Width=ClientWidth*3*2;bmp_test->Height=ClientHeight*3*2;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu, *2 z principu algoritmu (dvě nabuffrované obrazovky)
//	d.vykresli_vektory(bmp_test->Canvas,1);
//	Cantialising a; Pan_bmp_ALL=a.antialiasing(bmp_test,false);delete(bmp_test);//velice nutné do samostatné bmp_out, kvůli smazání bitmapy vracené AA
//	Pan_bmp_ALL->SaveToFile("testALL.bmp");
//	Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
//	Posun=Posun_predchozi;
//	MessageBeep(0);
	}
	else
	{
	 //varianta 3)rozměr bmp dle rozměru vektorových dat, nebo pokud by se jednalo o příliš velkou oblast, tak jen max násobek dle K obrazovky
//	 TRect oblast=vrat_max_oblast();
//	 short R=20;//nutná rezerva okraje v px
//	 long MaxX=oblast.right+R,MaxY=oblast.bottom+R,MinX=oblast.left-R,MinY=oblast.top-R;//pozor na znaménka Y
//	 float K=1.2;//koeficient zvětšení, 1.5 už méně efektivní, než pokrytím dokonalá (optimální) varianta 2
//	 if(MaxY-MinY>K*ClientHeight)//pokud by se jednalo o příliš velkou oblast, tak jen max násobek dle K obrazovky
//	 {
//			int Ox=m.round(((MaxX-MinX)*K-(MaxX-MinX))/2.0);//odsazení na X
//			int Oy=m.round(((MaxY-MinY)*K-(MaxY-MinY))/2.0);//odsazení na Y
//			MinX=0-Ox;
//			MaxX=ClientWidth+Ox;
//			MinY=0-Oy;
//			MaxY=ClientHeight+Oy;
//	 }

		panType=3;
		bmpPANall_XY.x=m.P2Lx(MinX);bmpPANall_XY.y=m.P2Ly(MinY);
		Zoom_predchozi_AA=Zoom;//záloha původního zoomu
		TPointD Posun_predchozi=Posun;
		if(Pan_bmp_ALL!=NULL){delete Pan_bmp_ALL;Pan_bmp_ALL=NULL;}//před vytvořením nové kvůli realokaci nejdříve nutné odstranit
		Posun.x+=m.round(MinX/Zoom);Posun.y+=m.round(MinY/Zoom);//lépe před zoomem,levý horní roh oblasti
		Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu, lépe za posunem
		Graphics::TBitmap *bmp_test=new Graphics::TBitmap;
		bmp_test->Width=ceil(abs((double)MaxX-MinX)*3);bmp_test->Height=ceil(abs((double)MaxY-MinY)*3);//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
		if(d.v.PP.raster.show && MOD!=SIMULACE)nacti_podklad(bmp_test->Canvas);//ale není dobré, že se přeantialiasingovává, nebo by se muselo prohnat přes  a.antialiasing(bmp_test,s alokovaným Pan_bmp_ALL a zapsaným s rastrem), nebo je varianta zcela odříznout tuto variantu pan_create při spuštěném rastru
		d.vykresli_vektory(bmp_test->Canvas,1);
		Cantialising a;
		Pan_bmp_ALL=a.antialiasing(bmp_test,false);delete(bmp_test);//velice nutné do samostatné bmp_out, kvůli smazání bitmapy vracené AA
		//Pan_bmp_ALL->SaveToFile("testALL.bmp");
		Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
		Posun=Posun_predchozi;
		//MessageBeep(0);
	}
}
//---------------------------------------------------------------------------
//vytvoří výřez pro pan_move - velký  - varianta 1 - voláná z vlákna při vytvor_statickou_scenu
void TForm1::pan_create2()
{
//	Zoom_predchozi_AA=Zoom;//záloha původního zoomu
//	TPointD Posun_predchozi=Posun;
//	Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
//	if(Pan_bmp_ALL!=NULL){delete Pan_bmp_ALL;Pan_bmp_ALL=NULL;}//před vytvořením nové kvůli realokaci nejdříve nutné odstranit
//	Posun.x-=ClientWidth/Zoom*3;Posun.y-=ClientHeight/Zoom*3;
//	Graphics::TBitmap *bmp_test=new Graphics::TBitmap;
//	bmp_test->Width=ClientWidth*3*3;bmp_test->Height=ClientHeight*3*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu, *3 z principu algoritmu (tři nabuffrované obrazovky, neví se kam, se bude obraz posouvat)
//	d.SCENA=1111111;
//	d.vykresli_vektory(bmp_test->Canvas,1);
//	Cantialising a;Pan_bmp_ALL=a.antialiasing(bmp_test,false);delete(bmp_test);//velice nutné do samostatné bmp_out, kvůli smazání bitmapy vracené AA
//	//Pan_bmp_ALL->SaveToFile("testALL.bmp");
//	Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
//	Posun=Posun_predchozi;
//	MessageBeep(0);
//	vlakno_PanCreateState=false;
//	vlakno_PanCreate->Terminate();
//	vlakno_PanCreate->Free();
//	vlakno_PanCreate=NULL;delete vlakno_PanCreate;
}
//---------------------------------------------------------------------------
//Posouvá výřez mapy při stisknutém mezerníku a L-myši
void TForm1::pan_map(TCanvas * canv, int X, int Y)
{
	log(__func__);//logování
	////zajištění skrytí komponent, vedlejší produkt metody d.vykresli_mGridy() protože má v sobě podmínky při pan_move
	if(OBJEKT_akt!=NULL)if(OBJEKT_akt->zobrazit_mGrid)d.vykresli_mGridy();

	////vykreslení aktuální pan_bmp
	canv->Brush->Color=clWhite;/*clLtGray*/canv->Brush->Style=bsSolid;
	if(panType==0)
	{
		//maže při posouvání obrazu starý obraz
		canv->FillRect(TRect(0,0,/*X-kurzor_souradnice_puvodni.x+Pan_bmp->Width*/ClientWidth,Y-vychozi_souradnice_kurzoru.y));//horní okraj
		canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height));//levy okraj
		canv->FillRect(TRect(X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,Y-vychozi_souradnice_kurzoru.y,ClientWidth,ClientHeight));//pravy okraj
		canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height,X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,ClientHeight));//dolní okraj
		//samotné posouvání Pan_bmp
		//if(vlakno_PanCreateState==1)
		canv->Draw(X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y,Pan_bmp);//varianta 0
		//else canv->Draw(X-vychozi_souradnice_kurzoru.x-ClientWidth,Y-vychozi_souradnice_kurzoru.y-ClientHeight,Pan_bmp_ALL);//varianta 1) viz pan_create
		//canv->Draw(X-vychozi_souradnice_kurzoru.x-T.x,Y-vychozi_souradnice_kurzoru.y-T.y,Pan_bmp_ALL);//varianta 2) viz pan_create
	}
	//varianta 3
	if(panType==3)
	{
		int x=X-vychozi_souradnice_kurzoru.x+m.L2Px(bmpPANall_XY.x);
		int y=Y-vychozi_souradnice_kurzoru.y+m.L2Py(bmpPANall_XY.y);
		//maže při posouvání obrazu starý obraz
		canv->FillRect(TRect(0,0,ClientWidth,y));//horní okraj
		canv->FillRect(TRect(0,y,x,ClientHeight));//levy okraj
		canv->FillRect(TRect(x+Pan_bmp_ALL->Width,y,ClientWidth,ClientHeight));//pravy okraj
		canv->FillRect(TRect(x,y+Pan_bmp_ALL->Height,x+Pan_bmp_ALL->Width,ClientHeight));//dolní okraj
		//samotná bmp obrazu
		canv->Draw(x,y,Pan_bmp_ALL);
	}
}
//---------------------------------------------------------------------------
//realizuje posunutí obrazu
void TForm1::pan_move_map()
{
	log(__func__);//logování
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
	vytvor_statickou_scenu();//vytvoří BMP se statickou scénou, musí být před REFRESH()
	REFRESH();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Posouvat1Click(TObject *Sender)
{
	log(__func__);//logování
	if(pom_vyhybka!=NULL || pom_bod!=NULL || (d.v.PtInBody()!=NULL && OBJEKT_akt==NULL) || pom!=NULL)//posun obrysu objektu (z editace i z layoutu) a posun haly (pouze layout)
	{
		Akce=MOVE_HALA;
		if(OBJEKT_akt!=NULL)nahled_ulozit(true);
		TIP=ls->Strings[308];//"Kliknutím určíte bod posunu, táhnutím posunete obrys objektu."
		refresh_mGrid=false;
		REFRESH();
		refresh_mGrid=true;
	}
	else//funkce posouvat
	{
  	Akce=PAN;
		kurzor(pan);
		zobraz_tip(ls->Strings[300]);//"TIP: Posun obrazu lze také vykonat pomocí stisknutého levého tlačítka myší a posunem myši požadovaným směrem."
	}
}
void __fastcall TForm1::Posouvat2Click(TObject *Sender)
{
	log(__func__);//logování
	Posouvat1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Posunout2Click(TObject *Sender)
{
	log(__func__);//logování
	Akce=PAN;
	kurzor(pan);
	pan_non_locked=true;
	zobraz_tip(ls->Strings[300]);//"TIP: Posun obrazu lze také vykonat pomocí stisknutého levého tlačítka myší a posunem myši požadovaným směrem."
}
void __fastcall TForm1::Posunout3Click(TObject *Sender)
{
	log(__func__);//logování
	Posunout2Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Vybratoknem1Click(TObject *Sender)
{
	log(__func__);//logování
	kurzor(window);
	Akce=ZOOM_W_MENU;
	zobraz_tip(ls->Strings[301]);//"TIP: Přiblížení oknem je možné také realizovat pomocí stisknuté klávesy CTRL a výběrem požadované oblasti pomocí myši při stisknutém levém tlačítku."
}
void __fastcall TForm1::Vybratoknem2Click(TObject *Sender)
{
	log(__func__);//logování
	Vybratoknem1Click(Sender);
}
//---------------------------------------------------------------------------
//Celá "mapa"
void __fastcall TForm1::RzToolButton11Click(TObject *Sender)
{
	log(__func__);//logování
	Uloz_predchozi_pohled();
	try
	{
		TRect oblast=vrat_max_oblast();//fce. defaultně vrací MaxInt a -MaxInt
		if(oblast.left!=MaxInt && oblast.right!=MaxInt*(-1))
		{
			//deklarace
			int MaxX=oblast.right,MaxY=oblast.top,MinX=oblast.left,MinY=oblast.bottom;
			int PD_x=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
			int PD_y=ClientHeight-vyska_menu-scGPPanel_statusbar->Height-scGPPanel_mainmenu->Height;
			TPointD centr;
			centr.x=m.P2Lx((MaxX+MinX)/2.0);
			centr.y=m.P2Ly((MaxY+MinY)/2.0);

			//výpočet nového Zoomu
			double Z1=abs(Zoom*PD_x/(MaxX-MinX)),Z2=abs(Zoom*PD_y/abs(MaxY-MinY));
			Z1=m.getZoomValue(Z1);Z2=m.getZoomValue(Z2);
			if(Z1>Z2)Zoom=Z2;else Zoom=Z1;
			if(Zoom<0.5)Zoom=0.5;if(Zoom>10)Zoom=10;

			//vycentrování obrazu
			PD_x=(ClientWidth+scSplitView_LEFTTOOLBAR->Width)/2.0;
			PD_y=(ClientHeight/*+(scGPPanel_mainmenu->Height-scGPPanel_statusbar->Height)*/)/2.0;
			Posun.x+=(m.L2Px(centr.x)-PD_x)/Zoom;
			Posun.y+=(m.L2Py(centr.y)-PD_y)/Zoom;

			//překreslení
			on_change_zoom_change_scGPTrackBar();
		}
	}catch(...){};
	vytvor_statickou_scenu();
	REFRESH();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm1::ESC()
{
	log(__func__);//logování
	if(TIP!="Kliknutím na objekt v knihovně objektu, tažením a následným usazením přidáte objekt." || ls->Strings[304]!="" && TIP!=ls->Strings[304])TIP="";//smazání zobrazeného tipu
	if(Akce==DRAW_HALA&&d.v.HALA.body!=NULL&&d.v.HALA.body->predchozi->n>2){d.v.vloz_bod(d.v.HALA.body->dalsi->X,d.v.HALA.body->dalsi->Y,pom,NULL,ortogonalizace_stav,true);Akce=NIC;kurzor(standard);REFRESH();}
	else if(Akce==DRAW_HALA){d.v.vymaz_body();Akce=NIC;kurzor(standard);REFRESH();}
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	zneplatnit_minulesouradnice();
	//vymaže, překreslí, odznačí provizorní lini
	switch(Akce)
	{
		//case MOVE:d.odznac_oznac_objekt(Canvas,pom,akt_souradnice_kurzoru_PX.x-vychozi_souradnice_kurzoru.x,akt_souradnice_kurzoru_PX.y-vychozi_souradnice_kurzoru.y);break;
		case ADD:
		{
			if(MOD!=EDITACE)
			{
				d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,pom);
				pom=NULL;//nesmí být v další sekci
			}
			else
			{
				if(pom->id==3)d.vykresli_ikonu_komory(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"",-1);
				else d.vykresli_element(Canvas,0,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"","",element_id,-1,Rotace_symbolu_minula);
			}
			REFRESH(false);//odstranění vykresleného kurzoru
		}break;
		case VYH:
		{
			//pokud už je vložená výhybka a dojde k ukončení, odstranit výhybku, řešeno zde, metoda d.v.smaz_element neumí smazat nedokončenou výhybku - spojku
			if(d.v.vyhybka_pom!=NULL)
			{
		  	Cvektory::TElement *smaz=d.v.vyhybka_pom;
				d.v.vyhybka_pom=NULL;
				//vlastní mazání
				d.v.smaz_element(smaz,true);
				delete smaz;smaz=NULL;
        d.v.pocet_vyhybek--;
			}
			//navrácení vrstev
			Akce=BLOK;
			AnsiString T;
			if(T==0 || T=="")rotace_jigu=0;else rotace_jigu=1;
			T=readINI("nastaveni_editace","zobrazeni_pozic"); //zobrazit pozice
			if(T==0 || T=="")zobrazit_pozice=0;else zobrazit_pozice=1;
			T=readINI("nastaveni_editace","zobrazit_popisky"); //zobrazit popisky
			if(T==0 || T=="")zobrazit_popisky=0;else zobrazit_popisky=1;
			T=readINI("nastaveni_editace","zobrazit_koleje"); //zobrazit koleje
			if(T==0 || T=="")zobrazit_koleje=0;else zobrazit_koleje=1;
			T=readINI("nastaveni_editace","zobrazit_palce"); //zobrazit_palce
			if(T==0 || T=="")zobrazit_palce=0;else zobrazit_palce=1;
			T=readINI("nastaveni_editace","rotace_jigu"); //zobrazit rotaci jigu
			if(T==0 || T=="")rotace_jigu=0;else rotace_jigu=1;
			T=readINI("nastaveni_editace","zobrazit_rozmisteni_voziku"); //zobrazit_rozmisteni_jigu
			if(T==0 || T=="")zobrazit_rozmisteni_voziku=0;else zobrazit_rozmisteni_voziku=1;
			T=readINI("nastaveni_editace","zobrazit_popisek_pohonu"); //zobrazit_popisek_pohonu
			if(T==1 || T=="")zobrazit_popisek_pohonu=1;else zobrazit_popisek_pohonu=0;
			if(rotace_jigu==1)scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
			else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
			if(zobrazit_pozice==1)scGPCheckBox_zobrazit_pozice->Checked=true;
			else scGPCheckBox_zobrazit_pozice->Checked=false;
			if(zobrazit_popisky==1)scGPCheckBox1_popisky->Checked=true;
			else scGPCheckBox1_popisky->Checked=false;
			if(zobrazit_koleje==1)scGPCheckBox_zobrazit_koleje->Checked=true;
			else scGPCheckBox_zobrazit_koleje->Checked=false;
			if(zobrazit_palce==1)scGPCheckBox_zobrazit_palce->Checked=true;
			else scGPCheckBox_zobrazit_palce->Checked=false;
			if(rotace_jigu==1) scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
			else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
			if(zobrazit_rozmisteni_voziku==1) scGPCheckBox_rozmisteni_voziku->Checked=true;
			else scGPCheckBox_rozmisteni_voziku->Checked=false;
			if(zobrazit_popisek_pohonu==1)scGPCheckBox_popisek_pohonu->Checked=true;
			else scGPCheckBox_popisek_pohonu->Checked=false;
			//překreslení
			vytvor_statickou_scenu();
			REFRESH();
		}break;
		case GEOMETRIE:if(Akce_temp!=EDITACE_TEXTU)ukonceni_geometrie();else smaz_kurzor();break;
		case GEOMETRIE_LIGHT:if(Akce_temp!=EDITACE_TEXTU)scGPButton_geometrieClick(this);else smaz_kurzor();break;
		case MAGNETICKE_LASO:
		{
			scGPImage_mereni_vzdalenostClick(this);
		}break;
		case S_K:
		{
      Akce=NIC;//musí být před REFRESH
			kurzor(standard);
			//smazání seznamu MAG_LASO
			d.v.vymaz_seznam_MAG_LASO();
			d.v.hlavicka_MAG_LASO();
      d.SCENA=0;
      REFRESH(false);
    }
	}
	if(MOD==TVORBA_CESTY)scGPButton_OKClick(this);
	pom_vyhybka=NULL;
	proces_pom=NULL;
	kurzor(standard);
	if(Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU)Akce=NIC;//musí být nad refresh
	if(OBJEKT_akt!=NULL){scGPImage_mereni_vzdalenost->ClipFrameFillColor=clWhite;scGPImage_zamek_posunu->ClipFrameFillColor=clWhite;}//pro případ ukončení akce měření
	//vrácení původního textu při ukončení editace
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
Cvektory::TObjekt *TForm1::add_objekt(int X, int Y)
{
	log(__func__);//logování
	Cvektory::TObjekt *ret=NULL;
	if(pom_element==NULL && d.v.ELEMENTY->dalsi!=NULL)pom_element=d.v.ELEMENTY->predchozi;
	if(pom_element!=NULL)pom=d.v.vrat_objekt(pom_element->objekt_n);//ošetření proti prvnímu vložení objektu
	if(vybrany_objekt>-1)
	{
		TPointD souradnice;
		if(grid)//pokud je zobrazena mřížka
		{
			switch(prichytavat_k_mrizce)
			{
				case 1:	souradnice.x=m.round(m.P2Lx(X)/(size_grid*1.0*m2px))*size_grid*m2px;souradnice.y=m.round(m.P2Ly(Y)/(size_grid*1.0*m2px))*size_grid*m2px;break;//přichytí automaticky
				case 2: souradnice=m.P2L(TPoint(X,Y));break;//automaticky nepřichyt
			}
		}
		else souradnice=m.P2L(TPoint(X,Y));
		//připínání objektu na ostatní
		short oblast=0;
		Cvektory::TElement *e_posledni_pom=pom_element;
		Cvektory::TElement *e_posledni=d.v.ELEMENTY->predchozi;
		if(pom_element!=NULL)oblast=d.v.oblast_objektu(pom,X,Y);
		if(prichytavat_k_mrizce==1 && oblast==1 && pom!=NULL){souradnice.x=e_posledni_pom->geo.X4;souradnice.y=e_posledni_pom->geo.Y4;}//za objekt
		if(prichytavat_k_mrizce==1 && oblast==1 && pom==NULL){souradnice.x=e_posledni->geo.X4;souradnice.y=e_posledni->geo.Y4;}//za objekt
		if(prichytavat_k_mrizce==1 && oblast==2 && d.v.OBJEKTY->predchozi->n==1){souradnice.x=d.v.OBJEKTY->predchozi->element->geo.X1;souradnice.y=d.v.OBJEKTY->predchozi->element->geo.Y1;}//před první objekt
		if(oblast==2 && d.v.OBJEKTY->predchozi->n>1){souradnice.x=pom->element->geo.X1;souradnice.y=pom->element->geo.Y1;}     //Memo(pom->name);
		//vložení objektu na konec
		if(pom_element==NULL || pom_element!=NULL && (pom_element==d.v.ELEMENTY->predchozi || (pom_element->eID==300 && pom_element->dalsi2!=NULL && pom_element->dalsi2->eID==301 && pom_element->dalsi2->predchozi2==pom_element) || (pom_element->dalsi!=NULL && pom_element->dalsi->eID==301 && pom_element->dalsi->predchozi2==pom_element)))//vloží za poslední prvek, vyhybku nebo za poslední element v sekundární větvi před výhybkou
		{
			ret=pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y);
		}
		else//vkládá prvek mezi objekty
		{
//			if(oblast==2 && d.v.OBJEKTY->predchozi->n>1)ret=pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y,d.v.OBJEKTY,pom);
//			else ret=pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y,pom,pom->dalsi);
			if(pom_element!=NULL && pom_element->eID==300)pom=d.v.vrat_objekt(pom_element->dalsi2->objekt_n);//pokudvkládám na vedlejší větev před existující objekty
			ret=pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y,pom,pom->dalsi);
			d.v.nove_indexy();//zvýší indexy nasledujicích bodů
		}
		pom=NULL;//odsranění pomocného ukazatele
		//posun ostatních objektů
		if(pom_vyhybka->dalsi!=NULL && pom_vyhybka->element->geo.X1==pom_vyhybka->dalsi->element->geo.X1 && pom_vyhybka->element->geo.Y1==pom_vyhybka->dalsi->element->geo.Y1)
		{
			Cvektory::TElement *e=d.v.vrat_posledni_element_objektu(pom_vyhybka);
			double x=0,y=0,delka=e->geo.delka;
			switch((int)e->geo.orientace)
			{
				case 0:y=delka;break;
				case 90:x=delka;break;
				case 180:y=-delka;break;
				case 270:x=-delka;break;
			}
			pom_vyhybka=pom_vyhybka->dalsi;
			while(pom_vyhybka!=NULL)
			{
				move_objekt(x,y,pom_vyhybka);
				pom_vyhybka=pom_vyhybka->dalsi;
			}
			e=NULL;delete e;
		}
		//ukazatelové záležitosti + ostatní
		if(d.v.OBJEKTY->predchozi->n>1)vlozit_predavaci_misto_aktualizuj_WT();
		pom_vyhybka=NULL;//odsranění pomocného ukazatele na vložený objekt
		Akce=NIC;//musí být nad REFRESH
		kurzor(standard);
		//ortogonalizace();//přesunuto do vektorové metodys
		vybrany_objekt=-1;//odznačí objekt logicky, musí se nový vybrat znovu
		if(d.v.OBJEKTY->dalsi!=NULL && d.v.OBJEKTY->predchozi->n==1)Nahled->Enabled=true;
		TIP="";//odstranění původní nápovědy pro přidávání objektu
    vytvor_statickou_scenu();//aktualizuje BMP statické scény o nový objekt, musí být před REFRESH, není důvod měnit nastavení d.SCENA
		REFRESH();
		vytvor_obraz();
		DuvodUlozit(true);
		e_posledni_pom=NULL;delete e_posledni_pom;
		e_posledni=NULL;delete e_posledni;
	}
	return ret;
}
//---------------------------------------------------------------------------
Cvektory::TObjekt *TForm1::add_objekt_za()
{
	//////Procházení všech objektů navrácení posledního, u větších objektů může citelná oblast překrývat ty malé, proto se musí prověřit všechny oblasti
	log(__func__);//logování
	Cvektory::TObjekt *O=NULL,*ret=NULL;
	short oblast=0,citlivost=10;//citlivost pro algoritmy kontrolující zda je bod v oblasti linie

	////kontrola vedlejší větve
	//if(d.v.pocet_vyhybek>0)
	{
		Cvektory::TElement *E=d.v.ELEMENTY->dalsi,*pom=NULL;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		TPointD bod;
		while(E!=NULL)
		{
			if(E->eID==300 && E->dalsi2->eID==301 && m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,E->X,E->Y,E->dalsi2->X,E->dalsi2->Y)<=citlivost)
			{
				pom_element_temp=E;
				break;
			}

			pom=E->dalsi;
	  	if(pom!=NULL)
	  	{
	  		if(pom->eID==301 && pom->predchozi2==E){bod.x=pom->X;bod.y=pom->Y;}
	  		else {bod.x=pom->geo.X1;bod.y=pom->geo.Y1;}
	  		if(E->geo.X4!=bod.x || E->geo.Y4!=bod.y)
				{
					if(m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,E->geo.X4,E->geo.Y4,bod.x,bod.y)<=citlivost)
	     		{
						pom_element_temp=E;
	     			break;
					}
				}
			}
			else
	  	{
	  		if(E->geo.X4!=d.v.ELEMENTY->dalsi->geo.X1 || E->geo.Y4!=d.v.ELEMENTY->dalsi->geo.Y1)
	  		{
					if(m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,E->geo.X4,E->geo.Y4,d.v.ELEMENTY->dalsi->geo.X1,d.v.ELEMENTY->dalsi->geo.Y1)<=citlivost)
	     		{
						pom_element_temp=E;
	     			break;
					}
				}
	  	}
	  	pom=E->dalsi2;
	  	if(E->eID==301)pom=NULL;//spojka si v tomto ukazateli uchovává svoji párovou výhybku
	  	if(pom!=NULL)
	  	{
	  		if(pom->eID==301 && pom->predchozi2==E){bod.x=pom->X;bod.y=pom->Y;}
	  		else {bod.x=pom->geo.X1;bod.y=pom->geo.Y1;}
	  		if(m.LeziVblizkostiUsecky(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,E->geo.X4,E->geo.Y4,bod.x,bod.y)<=citlivost)
				{
					pom_element_temp=E;
					break;
				}
			}

			E=d.v.dalsi_krok(VYHYBKY,E);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		E=NULL;delete E;
		pom=NULL;delete pom;
	}

	////kontrola mezi objekty
	if(d.v.OBJEKTY->predchozi->n>=2 && prichytavat_k_mrizce==1)
	{
		O=d.v.OBJEKTY->dalsi;
		while(O!=NULL)//nekontrolovat poslední objekt
		{
			//při přichytávání kontrolovat oblasti objektů
			oblast=d.v.oblast_objektu(O,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
			if(oblast==1 || O->n==1 && oblast==2){pom_element_temp=d.v.vrat_posledni_element_objektu(O);ret=O;}//ret=O;//pokud jsem za objektem vrátím ho
			O=O->dalsi;
		}
	}
	return ret;
}
//---------------------------------------------------------------------------
//pokud při uložení editovaného objektu je detekováno, že konec objketu nenavazuje na začátek následujísího objektu je položen dotaz a po potvrzení dojde ke spojení
bool TForm1::pripnuti_dalsich_objektu(double citlivost)
{																																																																								 																									//"Objekty nenavazují, automaticky posunout následující objekt pro zachování návaznosti?"
	log(__func__);//logování
	bool ret=false;
	double posun_x,posun_y;
	Cvektory::TElement *e_posledni=d.v.vrat_posledni_element_objektu(OBJEKT_akt);
	////připnutí dalších objektů na hlavní větvi
	if(e_posledni->dalsi!=NULL && !(e_posledni->dalsi->eID==301 && e_posledni->dalsi->predchozi2==e_posledni) && !(m.round2double(e_posledni->geo.X4,2)==m.round2double(e_posledni->dalsi->geo.X1,2) && m.round2double(e_posledni->geo.Y4,2)==m.round2double(e_posledni->dalsi->geo.Y1,2)) && m.delka(e_posledni->geo.X4,e_posledni->geo.Y4,e_posledni->dalsi->geo.X1,e_posledni->dalsi->geo.Y1)<=citlivost && mrYes==MB(ls->Strings[328],MB_YESNO,true))
	{
		//zjištění jednotlivých délek posunů
		posun_x=-e_posledni->dalsi->geo.X1+e_posledni->geo.X4;
		posun_y=-e_posledni->dalsi->geo.Y1+e_posledni->geo.Y4;
		//posun dalších elementů a objektů
		Cvektory::T2Element *V=d.v.hlavicka_seznam_VYHYBKY();
		unsigned long objekt_n=e_posledni->objekt_n;
		e_posledni=e_posledni->dalsi;
		while(e_posledni!=NULL)
		{
			//posun elementu
			d.v.vloz_G_element(e_posledni,e_posledni->geo.typ,e_posledni->geo.X1+posun_x,e_posledni->geo.Y1+posun_y,e_posledni->geo.X2+posun_x,e_posledni->geo.Y2+posun_y,e_posledni->geo.X3+posun_x,e_posledni->geo.Y3+posun_y,e_posledni->geo.X4+posun_x,e_posledni->geo.Y4+posun_y,e_posledni->geo.orientace,e_posledni->geo.rotacni_uhel,e_posledni->geo.radius,e_posledni->geo.delka);
			e_posledni->X+=posun_x;
			e_posledni->Y+=posun_y;
			//posun souřadnic tabulky, jestliže je vytvořená
			if(e_posledni->Xt!=-100)
			{
				e_posledni->Xt+=posun_x;
				e_posledni->Yt+=posun_y;
			}
			//posun objektu, pokud sem na jeho posledním elementu
			if(objekt_n!=e_posledni->objekt_n)
			{
				Cvektory::TObjekt *O=d.v.vrat_objekt(e_posledni->objekt_n);
				d.v.posun_body(posun_x,posun_y,O);
				O->Xt+=posun_x;
				O->Yt+=posun_y;
				O->Xp+=posun_x;
				O->Yp+=posun_y;
				O=NULL;delete O;
			}
			objekt_n=e_posledni->objekt_n;
			//posun na další
//			if(e_posledni->dalsi!=NULL && e_posledni->dalsi->eID==301 && e_posledni->dalsi->predchozi2==e_posledni)e_posledni=NULL;//posouvám objekty ve vedlejší větvi, neovlivní hlavní větev
//			else e_posledni=e_posledni->dalsi;//další, jdu pouze po hlavní větvi projektu
			e_posledni=d.v.dalsi_krok(V,e_posledni);
		}
    d.v.vymaz_seznam_VYHYBKY(V);
		delete e_posledni;e_posledni=NULL;
		ret=true;

		//ukončení akce
		vypni_geometrii();
	}

	e_posledni=NULL;delete e_posledni;
	return ret;
}
//---------------------------------------------------------------------------
//kontrola zda na sebe první a polední objekt navazují, pokud jsou blízko u sebe, ale nenavazují - naváže je
void TForm1::spojeni_prvni_posledni(double citlivost)
{
	log(__func__);//logování
	if(prichytavat_k_mrizce==1 && d.v.ELEMENTY->dalsi->geo.typ==0 && d.v.ELEMENTY->predchozi->geo.typ==0 && (m.round2double(d.v.ELEMENTY->dalsi->geo.X1,4)!=m.round2double(d.v.ELEMENTY->predchozi->geo.X4,4) || m.round2double(d.v.ELEMENTY->dalsi->geo.Y1,4)!=m.round2double(d.v.ELEMENTY->predchozi->geo.Y4,4)) && m.delka(d.v.ELEMENTY->dalsi->geo.X1,d.v.ELEMENTY->dalsi->geo.Y1,d.v.ELEMENTY->predchozi->geo.X4,d.v.ELEMENTY->predchozi->geo.Y4)<=citlivost && mrYes==MB(ls->Strings[431],MB_YESNO,true))//"Linka nenavazuje, přejete si automaticky dokončit?"
	{
		Cvektory::TGeometrie zaloha,zaloha_posledni=d.v.ELEMENTY->predchozi->geo;
		bool pridan=false;
		//pokud poslední element neni linie a první ano
		if(d.v.ELEMENTY->dalsi->geo.typ==0 && d.v.ELEMENTY->predchozi->geo.typ!=0)
		{
			//souřadnice pro vkládání nového elementu + jeho orientace
			double orientace=m.Rt90(d.v.ELEMENTY->predchozi->geo.orientace-d.v.ELEMENTY->predchozi->geo.rotacni_uhel),X=d.v.ELEMENTY->predchozi->geo.X4,Y=d.v.ELEMENTY->predchozi->geo.Y4;
			if(orientace==90 || orientace==270)X+=0.2;else Y+=0.2;
			Cvektory::TElement *novy=d.v.vloz_element(d.v.OBJEKTY->predchozi,MaxInt,X,Y,0,NULL);
			if(OBJEKT_akt!=NULL && OBJEKT_akt->n==novy->objekt_n)design_element(novy,true);
			d.v.vloz_G_element(novy,0,novy->predchozi->geo.X4,novy->predchozi->geo.Y4,0,0,0,0,X,Y,orientace);
			pridan=true;
			novy=NULL;delete novy;
		}
		//zjištění rozdílu
		bool hor=false,ver=false;
		TPointD rozdil;
		rozdil.x=d.v.ELEMENTY->dalsi->geo.X1-d.v.ELEMENTY->predchozi->geo.X4;
		rozdil.y=d.v.ELEMENTY->dalsi->geo.Y1-d.v.ELEMENTY->predchozi->geo.Y4;
		//uprava první souřadnice
		if(d.v.ELEMENTY->predchozi->geo.orientace==90 || d.v.ELEMENTY->predchozi->geo.orientace==270)
			{d.v.ELEMENTY->predchozi->X+=rozdil.x;d.v.vloz_G_element(d.v.ELEMENTY->predchozi,0,d.v.ELEMENTY->predchozi->geo.X1,d.v.ELEMENTY->predchozi->geo.Y1,0,0,0,0,d.v.ELEMENTY->predchozi->geo.X4+rozdil.x,d.v.ELEMENTY->predchozi->geo.Y4,d.v.ELEMENTY->predchozi->geo.orientace);hor=true;}
		else
			{d.v.ELEMENTY->predchozi->Y+=rozdil.y;d.v.vloz_G_element(d.v.ELEMENTY->predchozi,0,d.v.ELEMENTY->predchozi->geo.X1,d.v.ELEMENTY->predchozi->geo.Y1,0,0,0,0,d.v.ELEMENTY->predchozi->geo.X4,d.v.ELEMENTY->predchozi->geo.Y4+rozdil.y,d.v.ELEMENTY->predchozi->geo.orientace);ver=true;}
		//uprava druhé souřadnice
		Cvektory::TElement *E=d.v.ELEMENTY->predchozi->predchozi,*upraven=NULL;
		while(E!=NULL && E->n>0)
		{
			if((hor && (rozdil.y==0 || m.abs_d(rozdil.y)<0.00000000000001)) || (ver && (rozdil.x==0 || m.abs_d(rozdil.x)<0.00000000000001)))break;//konrola zde je nutné upravovat druhou
			if(hor && E->geo.typ==0 && (E->eID==MaxInt || E->eID==200) && (E->geo.orientace==0 || E->geo.orientace==180))
			{
				E->Y+=rozdil.y;
				zaloha=E->geo;
				d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1,0,0,0,0,E->geo.X4,E->geo.Y4+rozdil.y,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius);//prodloužení prvního
				upraven=E;
				//posun dalších
				E=E->dalsi;
				Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
				while(E!=NULL)
				{
          E->Y+=rozdil.y;
					d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1+rozdil.y,E->geo.X2,E->geo.Y2+rozdil.y,E->geo.X3,E->geo.Y3+rozdil.y,E->geo.X4,E->geo.Y4+rozdil.y,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius);
					E=d.v.dalsi_krok(VYHYBKY,E);
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
				break;
			}
			if(ver && E->geo.typ==0 && (E->eID==MaxInt || E->eID==200) && (E->geo.orientace==90 || E->geo.orientace==270))
			{
				E->X+=rozdil.x;
				zaloha=E->geo;
				d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1,0,0,0,0,E->geo.X4+rozdil.x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius);//prodloužení prvního
				upraven=E;
				//posun dalších
				E=E->dalsi;
				Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
				while(E!=NULL)
				{
					E->X+=rozdil.x;
					d.v.vloz_G_element(E,E->geo.typ,E->geo.X1+rozdil.x,E->geo.Y1,E->geo.X2+rozdil.x,E->geo.Y2,E->geo.X3+rozdil.x,E->geo.Y3,E->geo.X4+rozdil.x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius);
					E=d.v.dalsi_krok(VYHYBKY,E);
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
				break;
			}
			E=E->predchozi;
		}
		//pokud došlo ke změně dotaz na schválení, neschváleno navrácení změn
		if(upraven!=NULL)
		{
			//přesun pohledu na upravenou část geometrie
			TPoint Centr;
			Centr.x=upraven->geo.X1;
			Centr.y=upraven->geo.Y1;
			//vycentrování obrazu na střed
			Posun_predchozi=Posun;
			Posun.x=Centr.x/m2px-ClientWidth/2/Zoom;
			Posun.y=-Centr.y/m2px-(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2/Zoom; //ClientHeight-scGPPanel_bottomtoolbar->Height
			//překreslení, fyzický přesun
			REFRESH(false);
			//vykreslení úseku geometrie varovnou barvou, prozatím
			Canvas->Pen->Color=clYellow;
			d.line(Canvas,m.L2Px(upraven->geo.X1),m.L2Py(upraven->geo.Y1),m.L2Px(upraven->geo.X4),m.L2Py(upraven->geo.Y4));
			//otázka zda uživatel souhlasí, pokud ne navrácení do úůvodního stavu
			AnsiString T="";
			if(hor)T=rozdil.y;else T=rozdil.x; ///padne do Y
			if(T.Length()>7)T=T.SubString(1,7);
			if(mrNo==MB(m.L2Px(upraven->geo.X1),m.L2Py(upraven->geo.Y1),ls->Strings[454]+" "+T+".","",MB_YESNO,true,false,366,true,true))//"Editace úseku, souhlasíte? Rozdil ="
			{
        if(pridan)d.v.smaz_element(d.v.ELEMENTY->predchozi);
				upraven->geo=zaloha;
				E=upraven->dalsi;
        Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
				while(E!=NULL)
				{
					if(hor)
					{
            E->Y-=rozdil.y;
						d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1-rozdil.y,E->geo.X2,E->geo.Y2-rozdil.y,E->geo.X3,E->geo.Y3-rozdil.y,E->geo.X4,E->geo.Y4-rozdil.y,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius);
					}
					else
					{
            E->X-=rozdil.x;
						d.v.vloz_G_element(E,E->geo.typ,E->geo.X1-rozdil.x,E->geo.Y1,E->geo.X2-rozdil.x,E->geo.Y2,E->geo.X3-rozdil.x,E->geo.Y3,E->geo.X4-rozdil.x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius);
					}
					E=d.v.dalsi_krok(VYHYBKY,E);
				}
				d.v.ELEMENTY->predchozi->geo=zaloha_posledni;
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
			}
      //navrácení na původní posun
			Posun=Posun_predchozi;
		}
		E=NULL;delete E;

    //ukončení akce
		vypni_geometrii();
	}
}
//---------------------------------------------------------------------------
//kontroluje, zde se pokouší uživatel dopojit větev na geometrii a spojí
void TForm1::napoj_vetev_na_geo()
{
	log(__func__);
	//počáteční kontrola, zda jsem v geometrii a edituju vedlejší větev před spojkou
	if(/*Akce==GEOMETRIE && */posledni_editovany_element!=NULL && posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && m.delka(posledni_editovany_element->dalsi->dalsi2->geo.X4,posledni_editovany_element->dalsi->dalsi2->geo.Y4,posledni_editovany_element->geo.X4,posledni_editovany_element->geo.Y4)>2.0)
	{
		//blokování akce, nutné nastavit před zobrazením MB
		Akce=BLOK;
		//deklarace
		bool nalezeno=false;
		double X=posledni_editovany_element->geo.X4,Y=posledni_editovany_element->geo.Y4,citlivost=0.5;
		Cvektory::TElement *E_break=posledni_editovany_element->dalsi->dalsi2;//výhybka z aktuálně kreslené větve
		Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
		Cvektory::T2Element *VYH=d.v.hlavicka_seznam_VYHYBKY();
		//hledání do výhybky
		while(E!=NULL)
		{
			if(E->geo.typ==0 && !d.v.je_element_ve_vetvi(E,E_break) && ((E->geo.delka!=0 && m.LeziVblizkostiUsecky(X,Y,E->geo.X1,E->geo.Y1,E->geo.X4,E->geo.Y4)<=citlivost) || (E->geo.delka==0 && m.delka(X,Y,E->geo.X4,E->geo.Y4)<=citlivost))){nalezeno=true;E_break=E;break;}
			E=d.v.dalsi_krok(VYH,E);
		}
	  E=NULL;delete E;
		d.v.vymaz_seznam_VYHYBKY(VYH);

		//přesun a napojení geometrue
		if(nalezeno && E_break!=NULL && mrYes==MB(ls->Strings[15],MB_YESNO))//"Automaticky dopojit geometrii na nejbližší pohon?"
		{
			TPointD B=d.v.bod_na_geometrii(E_break,X,Y);
      Cvektory::TObjekt *O=NULL;
			Cvektory::TElement *S=posledni_editovany_element->dalsi;//uložení spojky
			if(E_break!=posledni_editovany_element->dalsi && E_break!=posledni_editovany_element->dalsi->dalsi)//pokud se nalezený element nerovná samotné spojce nebo elemntu za ní, výjmout spojku ze seznamu a vložit ji na nové místo
			{
				//vyjmutí spojky
				O=d.v.vrat_objekt(S->objekt_n);
				d.v.vloz_G_element(S->dalsi,0,S->geo.X1,S->geo.Y1,0,0,0,0,S->dalsi->geo.X4,S->dalsi->geo.Y4,S->dalsi->geo.orientace,S->dalsi->geo.rotacni_uhel,S->dalsi->geo.radius);
				if(O->element==S)O->element=S->dalsi;
	    	S->predchozi->dalsi=S->dalsi;
				S->dalsi->predchozi=S->predchozi;
				S->predchozi=NULL;S->dalsi=NULL;
				d.v.aktualizuj_cestu_teplomeru(O);//došlo k přesunu elementů
	    	//vložení spojky
				O=d.v.vrat_objekt(E_break->objekt_n);
	    	d.v.vloz_G_element(S,0,E_break->geo.X1,E_break->geo.Y1,0,0,0,0,B.x,B.y,E_break->geo.orientace,E_break->geo.rotacni_uhel,E_break->geo.radius);
		  	d.v.vloz_G_element(E_break,0,S->geo.X4,S->geo.Y4,0,0,0,0,E_break->geo.X4,E_break->geo.Y4,E_break->geo.orientace,E_break->geo.rotacni_uhel,E_break->geo.radius);
	    	S->X=B.x;S->Y=B.y;
	    	S->Xt=B.x;S->Yt=B.y;
				S->objekt_n=E_break->objekt_n;
        if(O->element==E_break)O->element=S;
	    	E_break->predchozi->dalsi=S;
				S->predchozi=E_break->predchozi;
				E_break->predchozi=S;
				S->dalsi=E_break;
				if(O!=NULL && O->n==OBJEKT_akt->n)
				{
					S->mGrid=new TmGrid(F);
					S->mGrid->Tag=6;//ID formu
					S->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;//předávání nejvetšího možného ID, aktualizace n elementů se provede, až po vložení, proto přiřadit větší ID aby nedošlo ke kolizi s jiným mGridem
					design_element(S,false);//nutné!
				}
				d.v.aktualizuj_cestu_teplomeru(O);//došlo k přesunu elementů
				//ukazatelové záležitosti
				O=NULL;delete O;
			}
			//přesun spojky na nové souřadnice
			else if(E_break->geo.delka!=0)
			{
				O=d.v.vrat_objekt(S->objekt_n);
				S->X=B.x;S->Y=B.y;
				d.v.vloz_G_element(S,0,S->geo.X1,S->geo.Y1,0,0,0,0,B.x,B.y,S->geo.orientace,S->geo.rotacni_uhel,S->geo.radius);
        d.v.vloz_G_element(S->dalsi,0,S->geo.X4,S->geo.Y4,0,0,0,0,S->dalsi->geo.X4,S->dalsi->geo.Y4,S->dalsi->geo.orientace,S->dalsi->geo.rotacni_uhel,S->dalsi->geo.radius);
				d.v.aktualizuj_cestu_teplomeru(O);//došlo k přesunu elementů
				O=NULL;delete O;
			}
			//napojení vedlejší větve + kontrola zda proběhlo správně
			if(!napojeni_vedlejsi_vetve(posledni_editovany_element,false))TIP=ls->Strings[518];//"Nepovedlo se automaticky dopojit geometrii."
			//přeindexování elementů
			S=d.v.ELEMENTY->dalsi;
			unsigned long n=1;
			Cvektory::T2Element *VYH=d.v.hlavicka_seznam_VYHYBKY();
			while(S!=NULL)
			{
				S->n=n;
				n++;
				S=d.v.dalsi_krok(VYH,S);
			}
			d.v.vymaz_seznam_VYHYBKY(VYH);
      //ukazatelové záležitosti
			S=NULL;delete S;
			O=NULL;delete O;
		}

		//ukazatelové záležitosti
	  E=NULL;delete E;
		E_break=NULL;delete E_break;
		if(Akce==BLOK)
		{
			JID=-1;stisknute_leve_tlacitko_mysi=false;
			//Akce=GEOMETRIE;//WA pro zobrazení MB
		}
	}
}
//---------------------------------------------------------------------------
//provede kontrolu, zdá je možnost geometrii spojit, dotáže se a spojí geometrii pokud uživatel souhlasí
bool TForm1::napojeni_vedlejsi_vetve(Cvektory::TElement *e_posledni,bool kontrola_vzdalenosti)
{
	log(__func__);
	double citlivost=0.5;if(!kontrola_vzdalenosti)citlivost=9999999.0;
	bool jeOK=true;
	//kontrola, zda se orientace vetve == orientaci spojky, pokud ne vloží oblouk
	if(e_posledni!=NULL && e_posledni->dalsi!=NULL && e_posledni->dalsi->eID==301 && e_posledni->dalsi->predchozi2==e_posledni && (m.Rt90(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel)!=m.Rt90(e_posledni->dalsi->geo.orientace-e_posledni->dalsi->geo.rotacni_uhel) || m.Rt90(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel)!=m.Rt90(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel)) && m.delka(e_posledni->dalsi->geo.X4,e_posledni->dalsi->geo.Y4,e_posledni->geo.X4,e_posledni->geo.Y4)<=citlivost)
	{
		double orientace=m.a360(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel);
		double rotacni_uhel=orientace-e_posledni->dalsi->geo.orientace;
		if(e_posledni->geo.typ!=0 && e_posledni->geo.rotacni_uhel!=rotacni_uhel)jeOK=false;//pokud napojuji odlišný oblouk na oblouk, nepovolit
		if(jeOK && d.v.hodnota_v_katalogu(d.v.PP.katalog,m.abs_d(rotacni_uhel)))
		{
			//parametry geometrie
			d.geoTemp.typ=1;
	  	d.geoTemp.radius=d.v.PP.radius;
			d.geoTemp.orientace=m.a360(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel);
			d.geoTemp.rotacni_uhel=orientace-e_posledni->dalsi->geo.orientace;//m.a360(e_posledni->dalsi->geo.orientace-d.geoTemp.orientace);
			d.geoTemp.delka=m.R2Larc(d.geoTemp.radius,d.geoTemp.rotacni_uhel);
			TPointD *PL=m.getArcLine(e_posledni->geo.X4,e_posledni->geo.Y4,d.geoTemp.orientace,d.geoTemp.rotacni_uhel,d.geoTemp.radius);
			d.geoTemp.X1=PL[0].x;d.geoTemp.Y1=PL[0].y;d.geoTemp.X2=PL[1].x;d.geoTemp.Y2=PL[1].y;d.geoTemp.X3=PL[2].x;d.geoTemp.Y3=PL[2].y;d.geoTemp.X4=PL[3].x;d.geoTemp.Y4=PL[3].y;

	  	//kontrola a uchovávání zda edituji hlavní nebo veldejší větev
			bool vypnout=false;
			if(Akce!=GEOMETRIE){Akce=GEOMETRIE;vypnout=true;}
			if(e_posledni->dalsi!=NULL && e_posledni->dalsi->eID==301 && e_posledni->dalsi->predchozi2==e_posledni)d.v.vyhybka_pom=e_posledni;
			e_posledni=d.v.vloz_element(d.v.vrat_objekt(e_posledni->objekt_n),MaxInt,d.geoTemp.X4,d.geoTemp.Y4,e_posledni->orientace,e_posledni->dalsi);
			if(vypnout)Akce=NIC;
			e_posledni->geo=d.geoTemp;
			if(e_posledni->objekt_n==OBJEKT_akt->n)design_element(e_posledni,true);//nutné!!!!!!!!
			posledni_editovany_element=e_posledni;
			d.v.vyhybka_pom=NULL;
		}
	}
	if(e_posledni!=NULL && e_posledni->dalsi!=NULL && e_posledni->dalsi->eID==301 && e_posledni->dalsi->geo.delka!=0 && e_posledni->geo.typ==0)jeOK=false;//nelze napojit linii na linii
	//e_posledni == poslední element ve vedlejší větvi
	if(jeOK && e_posledni!=NULL && e_posledni->dalsi!=NULL && e_posledni->dalsi->eID==301 && e_posledni->dalsi->predchozi2==e_posledni && m.Rt90(e_posledni->geo.orientace-e_posledni->geo.rotacni_uhel)==m.Rt90(e_posledni->dalsi->geo.orientace-e_posledni->dalsi->geo.rotacni_uhel) && m.delka(e_posledni->dalsi->geo.X4,e_posledni->dalsi->geo.Y4,e_posledni->geo.X4,e_posledni->geo.Y4)<=citlivost)
	{
		double posun_x,posun_y;
		short orientace=m.Rt90(e_posledni->dalsi->geo.orientace-e_posledni->dalsi->geo.rotacni_uhel);
		Cvektory::TElement *E=NULL;
		//zjištění jednotlivých délek posunů
		posun_x=-e_posledni->dalsi->geo.X4+e_posledni->geo.X4;
		posun_y=-e_posledni->dalsi->geo.Y4+e_posledni->geo.Y4;

		//zajištěni spojení
		//vertikální geometriie spojky
		if(orientace==0 || orientace==180)
		{
			//posun spojky na místo, pokud je spojka na linii, nebo pokud má geometrii
			if(e_posledni->dalsi->geo.delka!=0)
			{
		  	if(e_posledni->dalsi->geo.typ==0 && e_posledni->dalsi->geo.delka!=0 && m.abs_d(posun_y)<e_posledni->dalsi->geo.delka && ((e_posledni->dalsi->dalsi!=NULL && m.abs_d(posun_y)<e_posledni->dalsi->dalsi->geo.delka) || e_posledni->dalsi==NULL))//kontrola zde je kam posouvat
		  	{
		  		d.v.vloz_G_element(e_posledni->dalsi,0,e_posledni->dalsi->geo.X1,e_posledni->dalsi->geo.Y1,0,0,0,0,e_posledni->dalsi->geo.X4,e_posledni->dalsi->geo.Y4+posun_y,e_posledni->dalsi->geo.orientace);
		  		e_posledni->dalsi->Y+=posun_y;
		  		d.v.vloz_G_element(e_posledni->dalsi->dalsi,0,e_posledni->dalsi->geo.X4,e_posledni->dalsi->geo.Y4,0,0,0,0,e_posledni->dalsi->dalsi->geo.X4,e_posledni->dalsi->dalsi->geo.Y4,e_posledni->dalsi->dalsi->geo.orientace);
		  		posun_y=0;
		  	}
				else jeOK=false;
			}
			//vertikální posun (Y)
			if(posun_y!=0 && jeOK)//kontrola zda je třeba
			{
				E=e_posledni;
		  	while(E!=NULL)
		  	{
		  		if(E->eID==300 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka){E=NULL;break;}//narazil sem na konec větve ukončení
					if(E->geo.orientace==orientace && E->geo.typ==0)break;
		  		E=E->predchozi;
		  	}
		  	if(E!=NULL)
				{
					if(E->geo.typ==0 && m.round2double(E->geo.delka-posun_y,2)<=0)jeOK=false;
					else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1,0,0,0,0,E->geo.X4,E->geo.Y4-posun_y,E->geo.orientace,E->geo.rotacni_uhel);
		  		E->Y-=posun_y;
		  		E=E->dalsi;
					while(!(E->eID==301 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka) && jeOK)
					{
						if(E->eID==301 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka)break;
						if(E->geo.typ==0 && m.round2double(E->geo.delka-posun_y,2)<=0)jeOK=false;
						else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1-posun_y,E->geo.X2,E->geo.Y2-posun_y,E->geo.X3,E->geo.Y3-posun_y,E->geo.X4,E->geo.Y4-posun_y,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delka);
		  			E->Y-=posun_y;
						E=E->dalsi;
		  		}
				}
			}
			//horizontální posun (X)
			if(posun_x!=0 && jeOK)//kontrola zda je třeba
			{
				E=e_posledni;//->dalsi->predchozi;//procházení hlavní větve
		  	while(E!=NULL && E->n>0)
				{
					if(E->eID==300 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka){E=NULL;break;}//narazil sem na konec hlavní větve, ukončení
					if((E->geo.orientace==m.Rt90(orientace+90) || E->geo.orientace==m.Rt90(orientace-90)) && E->geo.typ==0)break;
		  		E=E->predchozi;
				}
		  	if(E!=NULL)
				{
					posun_x*=-1;
					if(E->geo.typ==0 && m.round2double(E->geo.delka+posun_x,2)<=0)jeOK=false;
					else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1,0,0,0,0,E->geo.X4+posun_x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel);
					E->X+=posun_x;
		  		E=E->dalsi;
					while(E!=NULL && jeOK)
		  		{
						//kontrola, zda nejsem na spojce
						if(E->eID==301 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka)break;
						if(E->geo.typ==0 && m.round2double(E->geo.delka-posun_x,2)<=0)jeOK=false;
						else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1+posun_x,E->geo.Y1,E->geo.X2+posun_x,E->geo.Y2,E->geo.X3+posun_x,E->geo.Y3,E->geo.X4+posun_x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delka);
		  			E->X+=posun_x;
						//posun na další element
						E=E->dalsi;
					}
		  	}
			}
		}
		else//horizontální geometriie spojky
		{
			//posun spojky na místo, pokud je spojka na lini, nebo pokud má geometrii
			if(e_posledni->dalsi->geo.delka!=0)
			{
		  	if(e_posledni->dalsi->geo.typ==0 && e_posledni->dalsi->geo.delka!=0 && m.abs_d(posun_x)<e_posledni->dalsi->geo.delka && ((e_posledni->dalsi->dalsi!=NULL && m.abs_d(posun_x)<e_posledni->dalsi->dalsi->geo.delka) || e_posledni->dalsi==NULL))//kontrola zde je kam posouvat
		  	{
		  		d.v.vloz_G_element(e_posledni->dalsi,0,e_posledni->dalsi->geo.X1,e_posledni->dalsi->geo.Y1,0,0,0,0,e_posledni->dalsi->geo.X4+posun_x,e_posledni->dalsi->geo.Y4,e_posledni->dalsi->geo.orientace);
		  		e_posledni->dalsi->X+=posun_x;
		  		d.v.vloz_G_element(e_posledni->dalsi->dalsi,0,e_posledni->dalsi->geo.X4,e_posledni->dalsi->geo.Y4,0,0,0,0,e_posledni->dalsi->dalsi->geo.X4,e_posledni->dalsi->dalsi->geo.Y4,e_posledni->dalsi->dalsi->geo.orientace);
		  		posun_x=0;
		  	}
		  	else jeOK=false;
			}
			//horizontální posun (X)
			if(posun_x!=0 && jeOK)//kontrola zda je třeba
			{
				E=e_posledni;
				while(E!=NULL && E->n>0)
				{
					if(E->eID==300 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka){E=NULL;break;}//narazil sem na konec větve ukončení
					if(E->geo.orientace==orientace && E->geo.typ==0)break;
					E=E->predchozi;
				}
		  	if(E!=NULL)
				{
					if(E->geo.typ==0 && m.round2double(E->geo.delka-posun_x,2)<=0)jeOK=false;
					else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1,0,0,0,0,E->geo.X4-posun_x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel);
					E->X-=posun_x;
					E=E->dalsi;
					while(!(E->eID==301 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka) && jeOK)
					{
						if(E->geo.typ==0 && m.round2double(E->geo.delka-posun_x,2)<=0)jeOK=false;
						else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1-posun_x,E->geo.Y1,E->geo.X2-posun_x,E->geo.Y2,E->geo.X3-posun_x,E->geo.Y3,E->geo.X4-posun_x,E->geo.Y4,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delka);
						E->X-=posun_x;
						E=E->dalsi;
					}
				}
			}
			//vertikální posun (Y)
			if(posun_y!=0 && jeOK)//kontrola zda je třeba
			{
				E=e_posledni;//->dalsi->predchozi;//procházení hlavní větve
				while(E!=NULL && E->n>0)
				{
					if(E->eID==300 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka){E=NULL;break;}//narazil sem na konec hlavní větve, ukončení
					if((E->geo.orientace==m.Rt90(orientace+90) || E->geo.orientace==m.Rt90(orientace-90)) && E->geo.typ==0)break;
		  		E=E->predchozi;
		  	}
		  	if(E!=NULL)
				{
					posun_y*=-1;
					if(E->geo.typ==0 && m.round2double(E->geo.delka+posun_y,2)<=0)jeOK=false;
					else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1,0,0,0,0,E->geo.X4,E->geo.Y4+posun_y,E->geo.orientace,E->geo.rotacni_uhel);
		  		E->Y+=posun_y;
					E=E->dalsi;
					while(E!=NULL && jeOK)
					{
						if(E->eID==301 && E->identifikator_vyhybka_spojka==e_posledni->dalsi->identifikator_vyhybka_spojka)break;
						if(E->geo.typ==0 && m.round2double(E->geo.delka+posun_y,2)<=0)jeOK=false;
						else d.v.vloz_G_element(E,E->geo.typ,E->geo.X1,E->geo.Y1+posun_y,E->geo.X2,E->geo.Y2+posun_y,E->geo.X3,E->geo.Y3+posun_y,E->geo.X4,E->geo.Y4+posun_y,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delka);
						E->Y+=posun_y;
		  			//posun na další element
						E=E->dalsi;
					}
				}
			}
		}
		E=NULL;delete E;
	}
	else jeOK=false;
	//ukončení editace geometrie
	Akce=BLOK;
//	if(jeOK)
//	{
//		vypni_geometrii();
//		Akce=NIC;Akce_temp=NIC;
//		//aktualizace comb ve výhybkách + refreshování všech tabulek, musí být, aby nedošlo k chybnému vykreslení mgridů, způsobuje zobrazení MB
//		Cvektory::TElement *E=OBJEKT_akt->element;
//		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
//  	while(E!=NULL)
//  	{
//			if(E->eID==300)napln_comba_mGridu(E);
//			E->mGrid->Refresh();
//  		E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
//		}
//  	E=NULL;delete E;
//		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
//  	//refresh ostatních tabulek
//		if(predchozi_PM!=NULL)predchozi_PM->mGrid->Refresh();
//		Cvektory::TTeplomery *Tep=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
//  	if(Tep!=NULL)Tep->posledni->mGrid->Refresh();
//		Tep=NULL;delete Tep;
//		d.SCENA=0;
//		REFRESH(true);
//		d.nabuffrovat_mGridy();//nutné pro zprávné zobrazení, pokud mGridy nebyly vykresleny před zahájením geometrie
//	}
	//return zda bylo vše OK
  return jeOK;
}
//---------------------------------------------------------------------------
//vypnu akci geometrie
void TForm1::vypni_geometrii()
{
  Akce=BLOK;//blokace akce, aby se nespoštěl refresh při změně viditelnosti vrstev
	AnsiString T;
	if(T==0 || T=="")rotace_jigu=0;else rotace_jigu=1;
	T=readINI("nastaveni_editace","zobrazeni_pozic"); //zobrazit pozice
	if(T==0 || T=="")zobrazit_pozice=0;else zobrazit_pozice=1;
	T=readINI("nastaveni_editace","zobrazit_popisky"); //zobrazit popisky
	if(T==0 || T=="")zobrazit_popisky=0;else zobrazit_popisky=1;
	T=readINI("nastaveni_editace","zobrazit_koleje"); //zobrazit koleje
	if(T==0 || T=="")zobrazit_koleje=0;else zobrazit_koleje=1;
	T=readINI("nastaveni_editace","zobrazit_palce"); //zobrazit_palce
	if(T==0 || T=="")zobrazit_palce=0;else zobrazit_palce=1;
	T=readINI("nastaveni_editace","rotace_jigu"); //zobrazit rotaci jigu
	if(T==0 || T=="")rotace_jigu=0;else rotace_jigu=1;
	T=readINI("nastaveni_editace","zobrazit_rozmisteni_voziku"); //zobrazit_rozmisteni_jigu
	if(T==0 || T=="")zobrazit_rozmisteni_voziku=0;else zobrazit_rozmisteni_voziku=1;
	T=readINI("nastaveni_editace","zobrazit_popisek_pohonu"); //zobrazit_popisek_pohonu
	if(T==1 || T=="")zobrazit_popisek_pohonu=1;else zobrazit_popisek_pohonu=0;
	if(rotace_jigu==1)scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
	else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
	if(zobrazit_pozice==1)scGPCheckBox_zobrazit_pozice->Checked=true;
	else scGPCheckBox_zobrazit_pozice->Checked=false;
	if(zobrazit_popisky==1)scGPCheckBox1_popisky->Checked=true;
	else scGPCheckBox1_popisky->Checked=false;
	if(zobrazit_koleje==1)scGPCheckBox_zobrazit_koleje->Checked=true;
	else scGPCheckBox_zobrazit_koleje->Checked=false;
	if(zobrazit_palce==1)scGPCheckBox_zobrazit_palce->Checked=true;
	else scGPCheckBox_zobrazit_palce->Checked=false;
	if(rotace_jigu==1) scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
	else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
	if(zobrazit_rozmisteni_voziku==1) scGPCheckBox_rozmisteni_voziku->Checked=true;
	else scGPCheckBox_rozmisteni_voziku->Checked=false;
	if(zobrazit_popisek_pohonu==1)scGPCheckBox_popisek_pohonu->Checked=true;
	else scGPCheckBox_popisek_pohonu->Checked=false;
}
//---------------------------------------------------------------------------
void TForm1::move_objekt(double X, double Y,Cvektory::TObjekt *Objekt)
{
	log(__func__);//logování
	////posun kabiny-polygonu
	d.v.posun_body(X,Y,Objekt);
	////posun nadpisu
	Objekt->Xt+=X;
	Objekt->Yt+=Y;
	////posun tabulky pohonů
	Objekt->Xp+=X;
	Objekt->Yp+=Y;
	////posun elementů
	Cvektory::TElement *E=Objekt->element;//objekt má vždy element (zarážka)
	while(E!=NULL && E->objekt_n==Objekt->n)
	{
		E->X+=X;E->Y+=Y;//souřadnice elementu
		if(E->Xt!=-100){E->Xt+=X;E->Yt+=Y;}//souřadnice tabulky + kontrola zda je vytvořená
		//geometrie elementu
		E->geo.X1+=X;E->geo.X2+=X;E->geo.X3+=X;E->geo.X4+=X;
		E->geo.Y1+=Y;E->geo.Y2+=Y;E->geo.Y3+=Y;E->geo.Y4+=Y;
		E=E->dalsi;
	}
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
void TForm1::zmen_poradi_objektu(int X, int Y)//testuje zda se nejedná o změnu pořadí (to musí ještě uživatel potvrdit)
{
	log(__func__);//logování
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
				if(d.lezi_v_pasmu(Canvas,X,Y,ukaz,false)>0)
				{
					RET=TRUE;
					break;
				}
			}
		}
		ukaz=ukaz->dalsi;
	}
	//pokud se může jednat o snahu (zjištěno z předchozí navrácenoho RET) o vložení ještě se na to dotazuje u uživatele
	if(RET)
	{
		UnicodeString text=ls->Strings[329]+" ",text_1=" "+ls->Strings[330]+" ",text_2=" "+ls->Strings[331]+" ";//"Chcete objekt "," umístit v pořadí mezi objekty "," a "
		if(ukaz==d.v.OBJEKTY->predchozi)//první prvek versus poslední
		{
			if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text+AnsiString(pom->name.UpperCase())+text_1+AnsiString(ukaz->name.UpperCase())+text_2+AnsiString(d.v.OBJEKTY->dalsi->name.UpperCase())+"?","",MB_YESNO,true,false))
			{
				d.v.zmen_poradi_objektu(pom,d.v.OBJEKTY->predchozi);//volání realizace samotné záměny
			}
		}
		else//ostatní situace
		{

			if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text+AnsiString(pom->name.UpperCase())+text_1+AnsiString(ukaz->name.UpperCase())+text_2+AnsiString(ukaz->dalsi->name.UpperCase())+"?","",MB_YESNO,true,false))
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
	log(__func__);//logování
	////ČÁSTEČNĚ PROVIZORNĚ
	//rotace dle umístění na ose Y či X dle trendu
	TPointD bod_vlozeni=bod_vlozeni_elementu();
	if(bod_vlozeni.x!=-1000 && pom_element_temp!=NULL)//kontrola zda vkládám na přímku
	{
		bool vkladani_stred=false;
		double delka_objektu=0.0;
		if(OBJEKT_akt->element->geo.typ==0 && (OBJEKT_akt->element->dalsi==NULL || (OBJEKT_akt->element->dalsi!=NULL && OBJEKT_akt->element->dalsi->objekt_n!=OBJEKT_akt->n))){vkladani_stred=true;delka_objektu=OBJEKT_akt->element->geo.delka;}
		FormX->vstoupeno_poh=false;//blokace událostí při vkládání elementu
		FormX->vstoupeno_elm=false;
		short rotace_symbolu=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),m.L2Px(bod_vlozeni.x),m.L2Py(bod_vlozeni.y));
		bod_vlozeni=uprav_bod_vlozeni_elementu(bod_vlozeni,rotace_symbolu);
		//vložení elementu na dané souřadnice a do patřičného pomocného spojáku, pro případ storna
		TIP="";//smazání tipu, pro jistotu
		Cvektory::TElement *E=d.v.vloz_element(OBJEKT_akt,element_id,bod_vlozeni.x,bod_vlozeni.y,rotace_symbolu);
		//nadesignuje tabulky daného elementu
		design_element(E,true);
		//kontrola zda může být vložen + možné odstranění
		short prekryti=prekryti_LO(E);
		UnicodeString text="";
		switch(prekryti)
		{
			case 0: default:break;
			case 1:text=ls->Strings[332];break;//"Vložením dojde k překrytí lakovacích oken, chcete element vložit?"
			case 2:text=ls->Strings[333];break;//"Vložením dojde k překrytí zóny otáčení, chcete element vložit?"
			case 3:text=ls->Strings[466];break;//"Vložením dojde k překrytí pracovní oblasti, chcete element vložit?"
		}
		if(prekryti>0 && mrYes!=MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text,"",MB_YESNO))
			{d.v.smaz_element(E);E=NULL;}
		//element byl vložen
		if(E!=NULL)
		{
      duvod_validovat=1;//při dalším refreshi dojde k validaci + generování vozíků, důležité např. pro vkládání otoče
			d.v.update_akt_zakazky();//protřeba mít nový element v cestě
			d.v.aktualizuj_sparovane_ukazatele();//aktualizace spárovaných ukazatelů
			//přiřazení režimu kabině + aktualizace pohon tabulek
			//d.v.aktualizace_rezimu_objektu(OBJEKT_akt);
			if(E->eID!=5 && E->eID!=6 && E->eID!=100 && E->eID!=200)d.v.aktualizuj_rezim_pohonu(E->pohon,d.v.vrat_druh_elementu(E));//přeskakovat elementy co neudávají režim, podle režimu pohonu jsou upravovány některá data
			d.v.aktualizace_rezimu_objektu(OBJEKT_akt);//aktualizace rezimu objektu, podle něj se řídí tabulky a vkládání robotů
			//aktualizace a validace
			vypocet_WT(E);
			if(E->dalsi!=NULL && E->eID==0 && E->dalsi->eID==0)design_element(E->dalsi,false);//aktualizace počtů pozic, pokud je stopka vložena do oblasti jiné stopky
			if(E->eID==5 || E->eID==6)FormX->validace_max_voziku();//přidána otoč do kabiny, aktualizace validace
			if(d.v.vrat_druh_elementu(E)==0 && E->eID!=0)FormX->validace_PT(E);//validace PT podle RT, u S&G elementů (kromě stopky)
			FormX->validace_aRD();//metoda kontroluje zda exituje PmG, nenulový pohon a režim objektu == 1 ... pokud platí provede validaci
			//bylo vloženo předávací místo
			if(E->eID==200)
			{
				design_element(E,false);//nutno předesignovat!! tabulka PM má v druhém sloupci totožný pohon jako v prvním, tento pohon byl z dalších elementů odstraněn v cyklu výše
   			//byla vložena pohonová tabulka, není třeba mít default poh tabulku
   			if(PmG!=NULL)//pokud je zobrazená default tabulka, smazat
   			{
   				nastav_focus();
   				PmG->Delete();
   				delete PmG;PmG=NULL;
				}
			}
			//přilepení elemntu na střed objektu
			if(E->eID!=0 && E->eID!=5 && E->eID!=6 && E->eID!=100 && E->eID<200 && vkladani_stred && E->geo.delka>=delka_objektu/2.0-delka_objektu*0.2 && E->geo.delka<=delka_objektu/2.0+delka_objektu*0.2 && mrYes==MB(ls->Strings[457],MB_YESNO))//"Chcete umístit element do středu objektu?"
			{
				d.v.posun_element(E,delka_objektu/2.0);
			}
			//pokud má element mGrid .. napozicuj
			if(E->eID!=100 && E->eID!=MaxInt /*&& E->eID!=200*/ && E->mGrid!=NULL)//pokud je alokovaná paměť pro mGrid element bude mít tabulku, pokud není element nebude mít tabulku
	  	{
	  		E->mGrid->Refresh();//nutné skrze aktuální údaje šířek tabulky
	  		//automatické výchozí umístění mGridové tabulky dle rotace elementu a nadesignováné tabulky (jejích rozměrů) - proto musí být až za nastevením designu
	  		aut_pozicovani(E,X,Y);//automatické pozicování pro nový DM při vkládání PM
			}
      //kontrolazda nejsou tabulky přes sebe + řešení
			mGrid_on_mGrid();
			OBJEKT_akt->pohon=E->pohon;//aktualizace aktuálně editovaného pohonu
		}
		//až na konec:
		E=NULL;delete E;
		Akce=NIC;
		element_id=99999;//ošetření pro správné zobrazování mgridů
		DrawGrid_knihovna->Refresh();
		DuvodUlozit(true);
		//Zde vložit podmínku pro kontrolu jaký element byl vložen, na základě toho znemožnit klik na roboty opačné funkcionality
		nahled_ulozit(true);//důvod k uložení náhledu
		refresh_mGrid=true;//nutné pro správné zobrazení mgridů po přidání elementu
		d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teploměry aktualizuje ji, jinak vytvoří default cestu
		vytvor_obraz();
	}
	else
	{
		TIP=ls->Strings[309];//"Lze vkládat pouze na linie."
	}
	REFRESH();
}
//---------------------------------------------------------------------------
void TForm1::add_vyhybka_spojka()
{
	log(__func__);//logování
	//rotace dle umístění na ose Y či X dle trendu
	TPointD bod_vlozeni=bod_vlozeni_elementu();
	if(bod_vlozeni.x!=-1000 && pom_element_temp!=NULL)//kontrola zda vkládám na přímku
	{
		Cvektory::TElement *E=NULL,*el=NULL;
		//úprava a zjištění bodu vložení a rotace
		short rotace_symbolu=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),m.L2Px(bod_vlozeni.x),m.L2Py(bod_vlozeni.y));
		bod_vlozeni=uprav_bod_vlozeni_elementu(bod_vlozeni,rotace_symbolu);

		//vkládání elementu, první vložení ... výhybka
		if(pom_element_temp->geo.X1==bod_vlozeni.x && pom_element_temp->geo.Y1==bod_vlozeni.y)el=pom_element_temp->predchozi;//vkládání na začátek oblouku
		if(pom_element_temp->geo.X4==bod_vlozeni.x && pom_element_temp->geo.Y4==bod_vlozeni.y)el=pom_element_temp;//vkládání na konec oblouku
		//vložení nového elementu
		if(E==NULL || (E!=NULL && E->eID!=MaxInt))
		{
			if(pom_element_temp->geo.typ!=0 && bod_vlozeni.x==pom_element_temp->geo.X1 && bod_vlozeni.y==pom_element_temp->geo.Y1 && pom_element_temp->predchozi->n>0)pom_element_temp=pom_element_temp->predchozi;
			if(pom_element_temp->geo.typ!=0 && bod_vlozeni.x==pom_element_temp->geo.X4 && bod_vlozeni.y==pom_element_temp->geo.Y4 && pom_element_temp->dalsi!=NULL)pom_element_temp=pom_element_temp->dalsi;
			pom=d.v.vrat_objekt(pom_element_temp->objekt_n);
			E=d.v.vloz_element(pom,element_id,bod_vlozeni.x,bod_vlozeni.y,rotace_symbolu);
			if(el!=NULL)
			{
				if(E->dalsi!=NULL && E->dalsi->objekt_n!=pom->n)
				{
					E->objekt_n=E->dalsi->objekt_n;
					d.v.vrat_objekt(E->dalsi->objekt_n)->element=E;
				}
				d.v.vloz_G_element(E,0,E->X,E->Y,0,0,0,0,E->X,E->Y,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,0);
				//pokud je výhybka / spojka před obloukem prohodit geometrii, takto půjde libovolně posouvat
				if(E->dalsi!=NULL && E->dalsi->dalsi!=NULL && E->dalsi->dalsi->geo.typ!=0 && E->geo.X4==E->dalsi->dalsi->geo.X1 && E->geo.Y4==E->dalsi->dalsi->geo.Y1)
				{
					d.geoTemp=E->geo;
					E->geo=E->dalsi->geo;
					E->dalsi->geo=d.geoTemp;
				}
			}
		}
		//změna stávajícího elemntu
		else
		{
			//změna eID
			E->eID=element_id;
			//defaultní data
			E->X=E->geo.X4;
			E->Y=E->geo.Y4;
			E->Xt=-100;
			E->Yt=-100;
			E->data.LO1=1.5;
			E->OTOC_delka=0;
			E->zona_pred=0;
			E->zona_za=0;
			E->data.LO2=0;
			E->data.LO_pozice=0;
			E->data.PT1=0;
    	E->PTotoc=0;
    	E->data.PT2=0;
			E->WT=0;//čekání na palec
      E->WT_index=0;
			E->data.WTstop=0;//čekání na stopce
			E->data.RT=0;//ryzí reserve time
			E->data.pocet_voziku=0;
			E->data.pocet_pozic=0;
			E->rotace_jig=0;
			E->stav=1;
			E->data.PD=-1;//defaultní stav pro S&G roboty
			//ukazatelové záležitosti
			E->dalsi2=NULL;
			E->predchozi2=NULL;
			if(element_id==300)
			{
				E->name=ls->Strings[9];//"Výhybka"
				E->short_name=ls->Strings[11];//"V"
				d.v.vyhybka_pom=E;
				d.v.pocet_vyhybek++;
			}
			else
			{
				E->name=ls->Strings[10];//"Spojka"
        E->short_name=ls->Strings[12];//"S"
				E->dalsi2=d.v.vyhybka_pom;
				E->predchozi2=d.v.vyhybka_pom;
				d.v.vyhybka_pom->predchozi2=E;
				d.v.vyhybka_pom->dalsi2=E;
        d.v.vyhybka_pom=NULL;
			}
			E->identifikator_vyhybka_spojka=d.v.pocet_vyhybek;
			E->name+=" "+String(E->identifikator_vyhybka_spojka);
			E->short_name+=E->identifikator_vyhybka_spojka;
		}

		//změna na vkládání spojky
		if(element_id==300)element_id=301;//vložena výhybka přechod na vkládání spojky
		//vkládání spojky
		else
		{
			//vypočet WT
			vypocet_WT(E);vypocet_WT(E->dalsi2);
			Akce=NIC;//vložena výhybka i spojka ukončení vkládání
			vytvor_obraz();
			//navrácení původního stavu vykreslovacích vrstev
			Akce=BLOK;
			AnsiString T;
			if(T==0 || T=="")rotace_jigu=0;else rotace_jigu=1;
    	T=readINI("nastaveni_editace","zobrazeni_pozic"); //zobrazit pozice
			if(T==0 || T=="")zobrazit_pozice=0;else zobrazit_pozice=1;
    	T=readINI("nastaveni_editace","zobrazit_popisky"); //zobrazit popisky
			if(T==0 || T=="")zobrazit_popisky=0;else zobrazit_popisky=1;
    	T=readINI("nastaveni_editace","zobrazit_koleje"); //zobrazit koleje
			if(T==0 || T=="")zobrazit_koleje=0;else zobrazit_koleje=1;
    	T=readINI("nastaveni_editace","zobrazit_palce"); //zobrazit_palce
			if(T==0 || T=="")zobrazit_palce=0;else zobrazit_palce=1;
    	T=readINI("nastaveni_editace","rotace_jigu"); //zobrazit rotaci jigu
    	if(T==0 || T=="")rotace_jigu=0;else rotace_jigu=1;
			T=readINI("nastaveni_editace","zobrazit_rozmisteni_voziku"); //zobrazit_rozmisteni_jigu
			if(T==0 || T=="")zobrazit_rozmisteni_voziku=0;else zobrazit_rozmisteni_voziku=1;
			T=readINI("nastaveni_editace","zobrazit_popisek_pohonu"); //zobrazit_popisek_pohonu
			if(T==1 || T=="")zobrazit_popisek_pohonu=1;else zobrazit_popisek_pohonu=0;
			if(rotace_jigu==1)scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
			else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
			if(zobrazit_pozice==1)scGPCheckBox_zobrazit_pozice->Checked=true;
			else scGPCheckBox_zobrazit_pozice->Checked=false;
			if(zobrazit_popisky==1)scGPCheckBox1_popisky->Checked=true;
			else scGPCheckBox1_popisky->Checked=false;
			if(zobrazit_koleje==1)scGPCheckBox_zobrazit_koleje->Checked=true;
			else scGPCheckBox_zobrazit_koleje->Checked=false;
			if(zobrazit_palce==1)scGPCheckBox_zobrazit_palce->Checked=true;
			else scGPCheckBox_zobrazit_palce->Checked=false;
			if(rotace_jigu==1) scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
    	else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
			if(zobrazit_rozmisteni_voziku==1) scGPCheckBox_rozmisteni_voziku->Checked=true;
			else scGPCheckBox_rozmisteni_voziku->Checked=false;
			if(zobrazit_popisek_pohonu==1)scGPCheckBox_popisek_pohonu->Checked=true;
			else scGPCheckBox_popisek_pohonu->Checked=false;
			TIP=ls->Strings[478];//zobrazení nápovědy jak otevřít editace sekundární větve po dokončení vložení výhybky
			d.v.aktualizuj_cestu_teplomeru();
			vytvor_statickou_scenu();//aktualizace BMP statické scény, nově výhybka a spojka, nutné aktualizovat
		}
		//pozicování mgridu, doladit podle finálních rozměrů tabulky
		if(E->orientace==0 || E->orientace==180){E->Xt=E->X-1.9;E->Yt=E->Y+2.1;}
		else{E->Xt=E->X+0.6;E->Yt=E->Y+0.5;}
		//ukazatelové záležitosti
		E=NULL;delete E;
    el=NULL;delete el;
	}
	else TIP=ls->Strings[309];//"Lze vkládat pouze na linie."
	REFRESH();
}
//---------------------------------------------------------------------------
//přidávání komory kabině powerwashe, kontrola zda není součet kabin větší než rozměr kabiny
void TForm1::add_komoru()
{                 //stejné komory přidat jejich šířku jinak 2.5
	log(__func__);//logování
	Cvektory::TKomora *pred=d.v.najdi_komoru(OBJEKT_akt);
	if(element_id==-1)element_id=0;else element_id=1;//převedení eid na typ kabiny (1=spracha, 0=okap)               //"Vložit jako novou sekci? Ne - změnit na jiný typ sekce."
	if(pred!=NULL && pred->typ!=element_id && mrNo==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,ls->Strings[408],"",MB_YESNO))//dotaz na změnu typu komory
	{
		if(pred->typ==0)pred->typ=1;
		else pred->typ=0;
	}
	else//přidání nové komory
	{
  	//určení velikosti komory
  	Cvektory::TKomora *k=OBJEKT_akt->komora->dalsi;
  	double velikost=k->velikost;
  	while(k!=NULL)//procházení přes všechny komory, suma jejich velikostí
  	{
  		if(velikost!=k->velikost){velikost=-100;break;}
  		k=k->dalsi;
  	}
  	k=NULL;
  	if(velikost==-100)velikost=2.5;//pokud nejsou stejné vložím defaul rozměr
  	//vložení komory
  	d.v.vloz_komoru(OBJEKT_akt,velikost,d.v.najdi_komoru(OBJEKT_akt),element_id);//vloží novou komoru, mezi ostatní či jako poslední
  	//změna rozmerů kabiny
  	k=OBJEKT_akt->komora->dalsi;
  	double celkem=0;
  	while(k!=NULL)//procházení přes všechny komory, suma jejich velikostí
  	{
  		celkem+=k->velikost;
  		k=k->dalsi;
  	}
  	k=NULL;delete k;
  	//změna rozměru kabiny
  	double rozmer_kabiny=OBJEKT_akt->element->geo.delka,posun_x=0,posun_y=0;
  	switch((int)OBJEKT_akt->orientace)
  	{
  		case 0:posun_y=celkem-rozmer_kabiny;break;
  		case 90:posun_x=celkem-rozmer_kabiny;break;
  		case 180:posun_y=-celkem+rozmer_kabiny;break;
  		case 270:posun_x=-celkem+rozmer_kabiny;break;
  	}
  	d.v.posun_hranu(posun_x,posun_y,OBJEKT_akt->body->dalsi->dalsi,OBJEKT_akt->body->dalsi->dalsi->dalsi);
  	//rozšíření pohonu
  	OBJEKT_akt->element->X+=posun_x;
  	OBJEKT_akt->element->Y+=posun_y;
  	d.v.vloz_G_element(OBJEKT_akt->element,0,OBJEKT_akt->element->geo.X1,OBJEKT_akt->element->geo.Y1,0,0,0,0,OBJEKT_akt->element->X,OBJEKT_akt->element->Y,OBJEKT_akt->element->geo.orientace);
	}
  //ostatní
	Akce=NIC;
	refresh_mGrid=false;
	REFRESH();
	refresh_mGrid=true;
	vytvor_obraz();
}
//---------------------------------------------------------------------------
//projde elementy a objekty, pokud je někde nutnost vložit předávací místo vloží ho tam
void TForm1::vlozit_predavaci_misto_aktualizuj_WT()
{
	log(__func__);//logování
	FormX->vstoupeno_elm=false;
	UnicodeString name=ls->Strings[271];//"Předávací místo"
	Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
	TPoint *tab_pruchodu=new TPoint[d.v.pocet_vyhybek];//.x uchovává počet průchodu přes výhybku, .y uchovává počet průchodů přes spojku
	while(E!=NULL)
	{
		//deklarace atributů
		if(E->eID!=300 && E->eID!=301 && !(E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->predchozi2==E))//výhybky a spojky jsou "PM"
		{
			//////////Vkládání předávacího místa
			if(E->eID!=200 && ((E->dalsi!=NULL && E->dalsi->pohon!=NULL && E->pohon!=NULL && E->dalsi->pohon->n!=E->pohon->n) || (E->dalsi!=NULL && (E->dalsi->pohon==NULL && E->pohon!=NULL || E->dalsi->pohon!=NULL && E->pohon==NULL))))
			{
				//změna zarážky na PM
				if(E->eID==MaxInt && E->geo.typ==0 && (E->geo.delka==0 || E->dalsi==NULL || (E->dalsi!=NULL && E->dalsi->geo.typ==0)))
				{
					//smazání a znovuvytvoření mGridu elementu
					if(OBJEKT_akt!=NULL && E->objekt_n==OBJEKT_akt->n)
					{
						nastav_focus();
		   			E->mGrid->Delete();
		   			E->mGrid=NULL;
					}
					//WT přiřazení
					E->WT=0;//čekání na palec
					E->WT_index=0;
					//souřadnice tabulky
		   		if(E->orientace==0 || E->orientace==180){E->Xt=E->X-1.9;E->Yt=E->Y+1.6;}
		   		else{E->Xt=E->X+0.6;E->Yt=E->Y+0.5;}
					//změna elemetnu na předávací místo
					E->eID=200;
		   		//názvy
					E->name=name;
					//d.v.uprav_popisky_elementu(E);
					pom_vyhybka=NULL;
					//znovuvytvoření mGridu elementu
					if(OBJEKT_akt!=NULL && E->objekt_n==OBJEKT_akt->n)
					{
						E->mGrid=new TmGrid(F);
						E->mGrid->Tag=6;//ID formu
						E->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;//předávání nejvetšího možného ID, aktualizace n elementů se provede, až po vložení, proto přiřadit větší ID aby nedošlo ke kolizi s jiným mGridem
						design_element(E,false);//nutné!
					}
				}
				//vložení PM s nulovou geometrií
				else
				{
					Akce=ADD;
					Cvektory::TObjekt *O=d.v.vrat_objekt(E->objekt_n);
					Cvektory::TElement *el=d.v.vloz_element(O,200,E->geo.X4,E->geo.Y4,E->orientace);
					el->pohon=E->pohon;
					//WT přiřazení
					el->WT=0;//čekání na palec
					el->WT_index=0;
					//souřadnice tabulky
					if(el->orientace==0 || el->orientace==180){el->Xt=el->X-1.9;el->Yt=el->Y+1.6;}
					else{el->Xt=el->X+0.6;el->Yt=el->Y+0.5;}
					//vytvoření mGridu elementu
					if(OBJEKT_akt!=NULL && el->objekt_n==OBJEKT_akt->n)design_element(el,false);//nutné!
					//geometrie
					d.v.vloz_G_element(el,0,E->geo.X4,E->geo.Y4,0,0,0,0,E->geo.X4,E->geo.Y4,el->geo.orientace,el->geo.rotacni_uhel,el->geo.radius,0);
					//kontroly, zda jsme před obloukem + ošetření
					if(E->dalsi!=NULL)
					{
						//prohození geometrie, pokud vkládám PM před oblouku, umožní posun PM
						if(E->dalsi->geo.typ!=0)
						{
							d.geoTemp=el->geo;
							el->geo=E->geo;
							E->geo=d.geoTemp;
						}
						//kontrola pohonů, pokud je za prázdnou zarážkou jiný pohon, přiřadit tento pohon i zarážce, dodržení PM má jeden pohon a za ní je druhý pohon
						if(E->pohon!=E->dalsi->pohon)E->pohon=E->dalsi->pohon;
					}
					Akce=NIC;
					//ukazatelové záležitosti
					el=NULL;delete el;
					O=NULL;delete O;
				}
			}
	  	//////////Aktualizace WT
			if(E->eID==200)vypocet_WT(E);
		}
		//aktualizace WT u výhybky
		if(E->eID==300)vypocet_WT(E);
		//aktualizace WT u spojky
		if(E->eID==301)vypocet_WT(E);
		//ukazatelové záležitosti
		E=d.v.sekvencni_zapis_cteni(E,tab_pruchodu,NULL);//nutné procházet tímto algoritmem, jeho nepoužitím může dojít k přeskočení kontroly elementu před spojkou (na hlavní větvi)
	}

	//////Předávací místo mezi posledním a prvním
	if(d.v.ELEMENTY->dalsi!=NULL && d.v.ELEMENTY->predchozi->n>0)// && d.v.ELEMENTY->predchozi->objekt_n!=d.v.ELEMENTY->dalsi->objekt_n)
	{
		Cvektory::TElement *e_prvni=d.v.ELEMENTY->dalsi,*e_posledni=d.v.ELEMENTY->predchozi;
		E=e_posledni;
		//////////Vkládání předávacího místa
		if(e_posledni->eID!=200 && (e_prvni->pohon!=NULL && e_posledni->pohon!=NULL && e_prvni->pohon->n!=e_posledni->pohon->n || e_prvni->pohon!=NULL && e_posledni->pohon==NULL || e_prvni->pohon==NULL && e_posledni->pohon!=NULL))
		{
      //změna zarážky na PM
			if(E->eID==MaxInt && E->geo.typ==0 && (E->dalsi==NULL || (E->dalsi!=NULL && E->dalsi->geo.typ==0)))
			{
		  	//mazání mGridu
				if(OBJEKT_akt!=NULL && e_posledni->objekt_n==OBJEKT_akt->n)
				{
					E->mGrid->Delete();
					E->mGrid=NULL;
				}
		  	//WT přiřazení
				E->WT=0;//čekání na palec
				E->WT_index=0;
		  	//souřadnice tabulky
		  	if(e_posledni->orientace==0 || e_posledni->orientace==180){E->Xt=E->X-1.9;E->Yt=E->Y+1.6;}
		  	else{E->Xt=E->X+0.6;E->Yt=E->Y+0.5;}
				//změna elemetnu na předávací místo
		  	E->eID=200;
		  	//názvy výhybek prozatím neřešeny
				E->name=name;
				//d.v.uprav_popisky_elementu(E);
				//smazání a znovuvytvoření mGridu elementu
				if(OBJEKT_akt!=NULL && e_posledni->objekt_n==OBJEKT_akt->n)
				{
					E->mGrid=new TmGrid(F);
					E->mGrid->Tag=6;//ID formu
					E->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;//předávání nejvetšího možného ID, aktualizace n elementů se provede, až po vložení, proto přiřadit větší ID aby nedošlo ke kolizi s jiným mGridem
					design_element(E,false);//nutné!
				}
			}
      //vložení PM s nulovou geometrií
			else
			{
				Akce=ADD;
				Cvektory::TObjekt *O=d.v.vrat_objekt(E->objekt_n);
				Cvektory::TElement *el=d.v.vloz_element(O,200,E->geo.X4,E->geo.Y4,E->orientace);
				//WT přiřazení
				el->WT=0;//čekání na palec
				el->WT_index=0;
				//souřadnice tabulky
				if(el->orientace==0 || el->orientace==180){el->Xt=el->X-1.9;el->Yt=el->Y+1.6;}
				else{el->Xt=el->X+0.6;el->Yt=el->Y+0.5;}
				//vytvoření mGridu elementu
				if(OBJEKT_akt!=NULL && el->objekt_n==OBJEKT_akt->n)design_element(el,true);//nutné!
				//geometrie
				d.v.vloz_G_element(el,0,E->geo.X4,E->geo.Y4,0,0,0,0,E->geo.X4,E->geo.Y4,el->geo.orientace,el->geo.rotacni_uhel,el->geo.radius,0);
        //kontroly, zda jsme před obloukem + ošetření
				if(E->dalsi!=NULL)
				{
					//prohození geometrie, pokud vkládám PM před oblouku, umožní posun PM
					if(E->dalsi->geo.typ!=0)
					{
						d.geoTemp=el->geo;
						el->geo=E->geo;
						E->geo=d.geoTemp;
					}
					//kontrola pohonů, pokud je za prázdnou zarážkou jiný pohon, přiřadit tento pohon i zarážce, dodržení PM má jeden pohon a za ní je druhý pohon
					if(E->pohon!=E->dalsi->pohon)E->pohon=E->dalsi->pohon;
				}
				//ukazatelové záležitosti
				el=NULL;delete el;
				O=NULL;delete O;
				Akce=NIC;
			}
		}
		//////////Aktualizace WT
		if(e_posledni->eID==200)vypocet_WT(E);
		//////////Mazání pomocných ukazatelů
		e_prvni=NULL;delete e_prvni;
		e_posledni=NULL;delete e_posledni;
		delete []tab_pruchodu;tab_pruchodu=NULL;
	}

	//aktualizace předchozího PM a zobrazení či skrytí default tabulky pro pohon
	if(OBJEKT_akt!=NULL)
	{
		//deklarace
		Cvektory::TElement *E=NULL;
		//kontrola zda se změnilo předchozí PM
		if(OBJEKT_akt->element->predchozi->n>0 || (OBJEKT_akt->element->predchozi->n==0 && OBJEKT_akt->dalsi!=NULL))
		{
			//zjištění nového předchozího PM
			if(OBJEKT_akt->element->predchozi->n>0)E=d.v.najdi_posledni_element_podle_eID(200,d.v.vrat_objekt(OBJEKT_akt->element->predchozi->objekt_n));
			else E=d.v.najdi_posledni_element_podle_eID(200,d.v.vrat_objekt(d.v.ELEMENTY->predchozi->objekt_n));
			//pokud existuje staré PM a zároveň se nerovná novému, musí být smazán mGrid
			if(predchozi_PM!=NULL && predchozi_PM!=E)
			{
				predchozi_PM->mGrid->Delete();
				predchozi_PM->mGrid=NULL;
			}
			//pokud stare PM se nerovná novému, bude vytvořen mGrid a přepsán ukazatel
			if(predchozi_PM!=E)
			{
				predchozi_PM=E;
				predchozi_PM->mGrid=new TmGrid(F);
				predchozi_PM->mGrid->Tag=6;//ID formu
				predchozi_PM->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;//předávání nejvetšího možného ID, aktualizace n elementů se provede, až po vložení, proto přiřadit větší ID aby nedošlo ke kolizi s jiným mGridem
				design_element(predchozi_PM,false);//nutné!
			}
		}
		//zobrazení či skryří tabulky pohonu
		bool existuje_tab_pohonu=false;
		E=OBJEKT_akt->element;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
		{
			if(E->eID==200 || E->eID==300){existuje_tab_pohonu=true;break;}
			else E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
		}
		if(predchozi_PM!=NULL)existuje_tab_pohonu=true;
		//pokud existuje tabulka pohonu a zároveň defaultní, smaže defaultní
		if(existuje_tab_pohonu && PmG!=NULL)
		{
			nastav_focus();
			PmG->Delete();
			PmG=NULL;delete PmG;
		}
		//pokud neexistuje žádná tabulka pohonu a zároveň není zovrazená default tabulka pohonu, zobrazí ji
		if(!existuje_tab_pohonu && PmG==NULL)vytvoreni_tab_pohon(existuje_tab_pohonu);
		//ukazatelové záležitosti
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	}

	//ukazatelové záležitosti
	E=NULL;delete E;
}
//---------------------------------------------------------------------------
//vloží bod haly na zvolené souřadnice (fyzické), zohlední přichytávání
void TForm1::vloz_bod_haly_objektu(int X,int Y)
{
	log(__func__);//logování
	//////Nastavení proměných
	TPointD souradnice;
	int vel_gridu=size_grid;//nesmí se pracovat se size_grid ->ovlivnilo by to vykreslování
	if(OBJEKT_akt!=NULL)vel_gridu/=10;//pokud jsem v editaci, grid je jemější
	//////Kostra přichitávání k mřížce
	if(grid)//pokud je zobrazena mřížka
	{
		switch(prichytavat_k_mrizce)
		{
			case 1:	souradnice.x=m.round(m.P2Lx(X)/(vel_gridu*1.0*m2px))*vel_gridu*m2px;souradnice.y=m.round(m.P2Ly(Y)/(vel_gridu*1.0*m2px))*vel_gridu*m2px;break;//přichytí automaticky
			case 2: souradnice=m.P2L(TPoint(X,Y));break;//automaticky nepřichyt
		}
	}
	else souradnice=m.P2L(TPoint(X,Y));
	if(JID==-3){souradnice.x=pom_bod->X;souradnice.y=pom_bod->Y;}//vkládání bod na bod
	//////Získání bodu před
	Cvektory::TBod *zabod=NULL;
	if(OBJEKT_akt!=NULL)pom_bod_temp=pom_bod;//plnění temp bodu pro jistotu
	if(pom_bod_temp!=NULL && pom_bod_temp->predchozi->n>0)zabod=pom_bod_temp->predchozi;
	if(funkcni_klavesa==3 && pom_bod!=NULL && pom_bod->n!=1)zabod=pom_bod->predchozi;
	if(OBJEKT_akt==NULL && funkcni_klavesa==3 && pom_bod!=NULL && pom_bod->n==1)zabod=d.v.HALA.body->predchozi;
	if(OBJEKT_akt!=NULL && funkcni_klavesa==3 && pom_bod!=NULL && pom_bod->n==1)zabod=OBJEKT_akt->body->predchozi;
	//////Vložení bodu
	d.v.vloz_bod(souradnice.x,souradnice.y,OBJEKT_akt,zabod,ortogonalizace_stav);
	//////Důvody k uložení
	DuvodUlozit(true);
	if(OBJEKT_akt!=NULL)nahled_ulozit(true);
	vytvor_obraz();
}
//---------------------------------------------------------------------------
//smaže bod haly nebo objektu, pokud existují už jen 2 poslední body smaže oba
void TForm1::smaz_bod_haly_objektu(Cvektory::TBod *bod)
{
	log(__func__);//logování
	//////Smazání
	if(OBJEKT_akt!=NULL && OBJEKT_akt->body->predchozi->n!=2 || OBJEKT_akt==NULL)//nedovolit odstranit vše v objektu 2 body musí vždy zůstat!!
	{
		d.v.smaz_bod(bod,OBJEKT_akt);
		bod=NULL;
	}
	//////Mazání poslendího bodu
	if(OBJEKT_akt==NULL && d.v.HALA.body->predchozi->n==1)//pokud zbyl v hale pouze poslední bod
	{
		d.v.smaz_bod(d.v.HALA.body->predchozi);
		d.v.smaz_bod(d.v.HALA.body);//mazat a nulovat hlavičku - velmi nutné !!!!
		d.v.HALA.body=NULL;
	}
	//////Důvody k uložení
	DuvodUlozit(true);
	if(OBJEKT_akt!=NULL)nahled_ulozit(true);
	vytvor_obraz();
}
//---------------------------------------------------------------------------
//vkládá novou geometrii nebo edituje již stávající geometrii
void TForm1::vlozeni_editace_geometrie()
{
	log(__func__);//logování
	//////deklarace atributů
	double posunx=0,posuny=0;
	//////nastavení orientace
	short orientace=0;
	if(posledni_editovany_element!=NULL){orientace=posledni_editovany_element->orientace;}else {orientace=d.v.vrat_posledni_element_objektu(OBJEKT_akt)->orientace;}
	//////pokud edituji úsek PM, změnit PM na zarážku
	Cvektory::TElement *E=posledni_editovany_element;
	//if(E!=NULL && E->eID!=200 && E->predchozi->n>0)E=E->predchozi;
  if(E==NULL)E=OBJEKT_akt->element;
	if(E!=NULL && E->eID==200 && d.geoTemp.typ!=-1)
	{
		//smazání a znovuvytvoření mGridu elementu
		if(E->mGrid!=NULL)
		{
			nastav_focus();
			E->mGrid->Delete();
			E->mGrid=NULL;
		}
		//nulování WT
		E->WT=0;//čekání na palec
    E->WT_index=0;
		//změna elemetnu na zarážku
		E->eID=MaxInt;
		//název
		if(DEBUG)
		{
			E->name="Zarážka "+String(d.v.vrat_pocet_elementu_eID(E)+1);
			//d.v.uprav_popisky_elementu(E);
		}
		else E->name="";
		E->mGrid=new TmGrid(F);
		E->mGrid->Tag=6;//ID formu
		E->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
		design_element(E,false);//nutné!
	}

	//////dokončování / začátek geometrie sekundární větve v objektu spojky
	if(posledni_editovany_element!=NULL && posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && posledni_editovany_element->objekt_n!=OBJEKT_akt->n && posledni_editovany_element->dalsi->objekt_n==OBJEKT_akt->n && posledni_editovany_element->dalsi->predchozi2==posledni_editovany_element || posledni_editovany_element!=NULL && posledni_editovany_element->dalsi2!=NULL && posledni_editovany_element->dalsi2->eID==301 && posledni_editovany_element->objekt_n!=OBJEKT_akt->n && posledni_editovany_element->dalsi2->objekt_n==OBJEKT_akt->n && posledni_editovany_element->dalsi2->predchozi2==posledni_editovany_element)
	{
		unsigned long n=posledni_editovany_element->objekt_n;
		Cvektory::TElement *E=NULL;
		if(posledni_editovany_element->eID==300)
		{
			posledni_editovany_element=d.v.vloz_element(OBJEKT_akt,MaxInt,posledni_editovany_element->geo.X4,posledni_editovany_element->geo.Y4,orientace,posledni_editovany_element);
			posledni_editovany_element->geo=d.geoTemp;
			posledni_editovany_element->X=d.geoTemp.X4;
			posledni_editovany_element->Y=d.geoTemp.Y4;
			//vytvoření mgridu, element byl přesunut z jiného objektu, nemá alokovanou paměť pro mGrid
			posledni_editovany_element->mGrid=new TmGrid(F);
			posledni_editovany_element->mGrid->Tag=6;//ID formu
			posledni_editovany_element->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;
			design_element(posledni_editovany_element,true);//nutné!!!!!!!!
		}
		else if(n!=OBJEKT_akt->n)
		{
			Cvektory::TPohon *p=posledni_editovany_element->pohon;
			E=d.v.vloz_element(OBJEKT_akt,MaxInt,posledni_editovany_element->geo.X4,posledni_editovany_element->geo.Y4,orientace,posledni_editovany_element);
			E->objekt_n=n;//navrácení zprávného objekt_n
			posledni_editovany_element->objekt_n=OBJEKT_akt->n;
			E->geo=posledni_editovany_element->geo;
			posledni_editovany_element->geo=d.geoTemp;
			posledni_editovany_element->X+=posledni_editovany_element->geo.X4-posledni_editovany_element->geo.X1;
			posledni_editovany_element->Y+=posledni_editovany_element->geo.Y4-posledni_editovany_element->geo.Y1;
			posledni_editovany_element->pohon=E->pohon;
			E->pohon=p;
			//vytvoření mgridu, element byl přesunut z jiného objektu, nemá alokovanou paměť pro mGrid
			posledni_editovany_element->mGrid=new TmGrid(F);
			posledni_editovany_element->mGrid->Tag=6;//ID formu
			posledni_editovany_element->mGrid->ID=d.v.vrat_nejvetsi_ID_tabulek(OBJEKT_akt)+1;
			design_element(posledni_editovany_element,true);//nutné!!!!!!!!
			//posledni_editovany_element=E;
			p=NULL;delete p;
		}
		else
		{
			E=d.v.vloz_element(OBJEKT_akt,MaxInt,posledni_editovany_element->geo.X4,posledni_editovany_element->geo.Y4,orientace,posledni_editovany_element);
			E->geo=posledni_editovany_element->geo;
			design_element(E,true);//nutné!!!!!!!!
			posledni_editovany_element->geo=d.geoTemp;
			posledni_editovany_element->X=posledni_editovany_element->geo.X4;posledni_editovany_element->Y=posledni_editovany_element->geo.Y4;//souřadnice elementu
		}
		E=NULL;delete E;  //REFRESH();
	}
	//////definice sekundární větve z výhybky
	else if(posledni_editovany_element!=NULL && posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->eID==300 && posledni_editovany_element->objekt_n==OBJEKT_akt->n)// && d.geoTemp.orientace-d.geoTemp.rotacni_uhel!=posledni_editovany_element->dalsi->geo.orientace-posledni_editovany_element->dalsi->geo.rotacni_uhel)
	{
		//pokud již byla nadefinovaná sekundární větev, bude odmazána
		if(posledni_editovany_element->dalsi2!=posledni_editovany_element->predchozi2)
		{
			Cvektory::TElement *E=posledni_editovany_element->dalsi2,*smaz=E;
			while(E!=NULL)
			{
				if(E==posledni_editovany_element->predchozi2)break;
				smaz=E;
				E=E->dalsi;
				if(smaz->eID!=300 && smaz->eID!=301)d.v.smaz_element(smaz,true,1);//musí zde být fiktivní číslo objektu, to zaručí, že element bude úplně smazán ne nahrazen zarážkou
        else d.v.smaz_element(smaz);//u výhybek je třeba provést kontroly a nechat odmazat celé větve
			}
			E=NULL;delete E;
			smaz=NULL;delete smaz;
		}
		if(posledni_editovany_element->dalsi2->eID==301)d.v.vyhybka_pom=posledni_editovany_element->dalsi2;//pokud je další v sekundární větvi spojka, jedná se o nestandartní situaci, nutno rozlišit
		posledni_editovany_element=d.v.vloz_element(OBJEKT_akt,MaxInt,d.geoTemp.X4,d.geoTemp.Y4,orientace,posledni_editovany_element->dalsi2);
		design_element(posledni_editovany_element,true);//nutné!!!!!!!!
		posledni_editovany_element->geo=d.geoTemp;
		d.v.vyhybka_pom=NULL;//uvedení do původního stavu
	}
	//////první vložení geometrie
	else if(posledni_editovany_element==NULL)
	{
		//vložení nového úseku před první element
		if(posun_dalsich_elementu)
		{
    	posunx=d.geoTemp.X4-d.geoTemp.X1;posuny=d.geoTemp.Y4-d.geoTemp.Y1;
			posledni_editovany_element=d.v.vloz_element(OBJEKT_akt,MaxInt,d.geoTemp.X4,d.geoTemp.Y4,orientace,OBJEKT_akt->element);
			design_element(posledni_editovany_element,true);//nutné!!!!!!!!
			posledni_editovany_element->geo=d.geoTemp;
		}
		//přepsání geometrie prvnímu elementu v kabině
		else
		{
//			short rot=m.Rt90(OBJEKT_akt->element->orientace-OBJEKT_akt->element->geo.orientace-OBJEKT_akt->element->geo.rotacni_uhel);
//			rot=m.Rt90(d.geoTemp.orientace+d.geoTemp.rotacni_uhel+rot);  //element se nerotuje okolo konečného bodu ramena
			posunx=d.geoTemp.X4-OBJEKT_akt->element->geo.X4;posuny=d.geoTemp.Y4-OBJEKT_akt->element->geo.Y4;
			OBJEKT_akt->element->geo=d.geoTemp;
			OBJEKT_akt->element->X+=posunx;OBJEKT_akt->element->Y+=posuny;
			posledni_editovany_element=OBJEKT_akt->element;
			if(abs(posledni_editovany_element->geo.rotacni_uhel)==90 && posledni_editovany_element->eID==200)posledni_editovany_element->orientace=m.Rt90(posledni_editovany_element->geo.orientace-posledni_editovany_element->geo.rotacni_uhel-90);//změna orientace podle trendu, nefunguje pro oblouky
		}
		//posun následujících elementů, pokud existují
		Cvektory::TElement *E=posledni_editovany_element->dalsi;
		while(E!=NULL && E->objekt_n==posledni_editovany_element->objekt_n)
		{
			E->X+=posunx;E->Y+=posuny;//souřadnice elementu
			//geometrie elementu
			E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
			E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
			if(E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->predchozi2==E)break;
			E=E->dalsi;
		}
		E=NULL;delete E;
	}
	//////editace pohonu ve středu
	else if(posledni_editovany_element!=NULL && posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID!=301 && posledni_editovany_element->dalsi->objekt_n==posledni_editovany_element->objekt_n)
	{
    //rozšíření poslední zarážky o novou geometrii
		if(posledni_editovany_element->geo.typ==0 && posledni_editovany_element->eID==MaxInt && d.geoTemp.typ==0)
		{
			d.v.vloz_G_element(posledni_editovany_element,0,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,0,0,0,0,d.geoTemp.X4,d.geoTemp.Y4,posledni_editovany_element->geo.orientace,posledni_editovany_element->geo.rotacni_uhel,posledni_editovany_element->geo.radius);
			if(posledni_editovany_element->dalsi!=NULL && !posun_dalsich_elementu && posledni_editovany_element->dalsi->eID==MaxInt)d.v.smaz_element(posledni_editovany_element->dalsi,true,1);//smazat bez kontrol
			if(posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->objekt_n==posledni_editovany_element->objekt_n)
			{
				posunx=-posledni_editovany_element->dalsi->geo.X1+posledni_editovany_element->geo.X4;
				posuny=-posledni_editovany_element->dalsi->geo.Y1+posledni_editovany_element->geo.Y4;
      }
		}
		//vložení nové zarážky s geometrii
		else
		{
	  	//vložení úseku
			if(posun_dalsich_elementu)
	  	{
	  		posunx=d.geoTemp.X4-d.geoTemp.X1;posuny=d.geoTemp.Y4-d.geoTemp.Y1;
	  		posledni_editovany_element=d.v.vloz_element(OBJEKT_akt,MaxInt,d.geoTemp.X4,d.geoTemp.Y4,orientace,posledni_editovany_element->dalsi);
	  		design_element(posledni_editovany_element,true);//nutné!!!!!!!!
	  		posledni_editovany_element->geo=d.geoTemp;
	  	}
	  	//změna úseku
	  	else
	  	{
	  		posunx=d.geoTemp.X4-posledni_editovany_element->dalsi->geo.X4;posuny=d.geoTemp.Y4-posledni_editovany_element->dalsi->geo.Y4;
	  		posledni_editovany_element->dalsi->geo=d.geoTemp;
	  		posledni_editovany_element->dalsi->X+=posunx;posledni_editovany_element->dalsi->Y+=posuny;
	  		posledni_editovany_element=posledni_editovany_element->dalsi;
			}
		}
    //posun ostatních úseků geometrie
		Cvektory::TElement *E=posledni_editovany_element->dalsi;
		while(E!=NULL && E->objekt_n==posledni_editovany_element->objekt_n)
		{
			E->X+=posunx;E->Y+=posuny;//souřadnice elementu
			//geometrie elementu
	  	E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
			E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
			if(E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->predchozi2==E)break;
			E=E->dalsi;
		}
		E=NULL;delete E;
	}
	//////přidávání za poslední geometrii (hlavní i vedlejší)
	else if(posledni_editovany_element!=NULL && (posledni_editovany_element->dalsi==NULL || (posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301) || (posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->objekt_n!=posledni_editovany_element->objekt_n)))
	{
		//rozšíření poslední zarážky o novou geometrii
		if(posledni_editovany_element->geo.typ==0 && posledni_editovany_element->eID==MaxInt && d.geoTemp.typ==0)
		{
			d.v.vloz_G_element(posledni_editovany_element,0,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,0,0,0,0,d.geoTemp.X4,d.geoTemp.Y4,posledni_editovany_element->geo.orientace,posledni_editovany_element->geo.rotacni_uhel,posledni_editovany_element->geo.radius);
		}
		//rozšíření oblouku, pokud je možno
		else if(posledni_editovany_element->geo.typ!=0 && posledni_editovany_element->eID==MaxInt && d.geoTemp.typ!=0 && posledni_editovany_element->geo.rotacni_uhel/m.abs_d(posledni_editovany_element->geo.rotacni_uhel)==d.geoTemp.rotacni_uhel/m.abs_d(d.geoTemp.rotacni_uhel) && d.v.hodnota_v_katalogu(d.v.PP.katalog,m.abs_d(posledni_editovany_element->geo.rotacni_uhel+d.geoTemp.rotacni_uhel)))
		{
			posledni_editovany_element->geo.rotacni_uhel+=d.geoTemp.rotacni_uhel;
			posledni_editovany_element->geo.delka=m.R2Larc(posledni_editovany_element->geo.radius,posledni_editovany_element->geo.rotacni_uhel);
			TPointD *PL=m.getArcLine(posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.orientace,posledni_editovany_element->geo.rotacni_uhel,posledni_editovany_element->geo.radius);
			posledni_editovany_element->geo.X1=PL[0].x;posledni_editovany_element->geo.Y1=PL[0].y;posledni_editovany_element->geo.X2=PL[1].x;posledni_editovany_element->geo.Y2=PL[1].y;posledni_editovany_element->geo.X3=PL[2].x;posledni_editovany_element->geo.Y3=PL[2].y;posledni_editovany_element->geo.X4=PL[3].x;posledni_editovany_element->geo.Y4=PL[3].y;
    }
		//vložení nové zarážky s geometrii
		else
		{
	  	//vložím nový prvek, který převezme geometrii posledniho a zařadí se před nej, poslední pak převezme novou geometrii - tz. posouvám poslední prvek stále před sebou
	  	if(posledni_editovany_element->geo.delka!=0)//normální provoz
	  	{
				//kontrola a uchovávání zda edituji hlavní nebo veldejší větev
				if(posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && posledni_editovany_element->dalsi->predchozi2==posledni_editovany_element)d.v.vyhybka_pom=posledni_editovany_element;
				posledni_editovany_element=d.v.vloz_element(OBJEKT_akt,MaxInt,d.geoTemp.X4,d.geoTemp.Y4,orientace,posledni_editovany_element->dalsi);
				posledni_editovany_element->geo=d.geoTemp;
				design_element(posledni_editovany_element,true);//nutné!!!!!!!!
				d.v.vyhybka_pom=NULL;
	  	}
	  	else//pokud je veškerá geometrie odstraněna z kabiny
	  	{
				posledni_editovany_element->geo=d.geoTemp;
	  		posledni_editovany_element->X=posledni_editovany_element->geo.X4;
	  		posledni_editovany_element->Y=posledni_editovany_element->geo.Y4;
			}
		}
	}

  ////aktualizace indexů
	unsigned long n=1;
	E=d.v.ELEMENTY->dalsi;
	Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		E->n=n;
		n++;
		E=d.v.dalsi_krok(VYHYBKY,E);
	}
	d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	delete E;E=NULL;

	////připnutí vedlejší větve na hlavní
	//napojeni_vedlejsi_vetve(posledni_editovany_element);//temp zakomentování
	////připnutí objektů
	pripnuti_dalsich_objektu();//dalšího objektu
	if(OBJEKT_akt->n==1 || OBJEKT_akt->n==d.v.OBJEKTY->predchozi->n)spojeni_prvni_posledni();//provázání prvni poslední objekt
	//důvod uložit
	nahled_ulozit(true);
	//vytvoření obrazu pro UNDO a REDO, nelze načíst obrazi při editaci geometrie ... pam. chyba
	vytvor_obraz();
	vytvor_statickou_scenu();
	//napoj_vetev_na_geo();//test pozice
}
//---------------------------------------------------------------------------
//vymaže aktuální usek geometrie
void TForm1::smaz_usek_geometrie()
{
	log(__func__);
	if(Akce==GEOMETRIE && Akce_temp!=EDITACE_TEXTU && posledni_editovany_element!=NULL && posledni_editovany_element->eID==MaxInt && posledni_editovany_element->objekt_n==OBJEKT_akt->n)
	{
		if(posledni_editovany_element->n!=OBJEKT_akt->element->n || posledni_editovany_element->n==OBJEKT_akt->element->n && posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->objekt_n==OBJEKT_akt->n)
		{
			bool hl_vetev=true; bool posun=true;
			double posunx=posledni_editovany_element->geo.X1-posledni_editovany_element->geo.X4,posuny=posledni_editovany_element->geo.Y1-posledni_editovany_element->geo.Y4;
			Cvektory::TElement *E=NULL;
			if(posledni_editovany_element->predchozi->n>0/* && posledni_editovany_element->predchozi->objekt_n==OBJEKT_akt->n*/)E=posledni_editovany_element->predchozi;
			if(posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && posledni_editovany_element->dalsi->predchozi2==posledni_editovany_element)posun=false;//mažu poslední element vedlejší větve před spojkou, nepovolit posun
			if(posledni_editovany_element->predchozi->eID==300 && posledni_editovany_element->predchozi->dalsi2==posledni_editovany_element)hl_vetev=false;//mazání prvního elementu sekundární větve za vyhybkou, posun další geometrie pouze na sekundární větvi
			d.v.smaz_element(posledni_editovany_element,true,OBJEKT_akt->n);
			posledni_editovany_element=E;
			E=OBJEKT_akt->element;
			if(posledni_editovany_element!=NULL)E=posledni_editovany_element->dalsi;
			if(!hl_vetev)E=posledni_editovany_element->dalsi2;
			Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
			while(E!=NULL && E->objekt_n==OBJEKT_akt->n && posun)
			{
				E->X+=posunx;E->Y+=posuny;//souřadnice elementu
				//geometrie elementu
				E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
				E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
				if(E->dalsi!=NULL && E->dalsi->eID==301 && E->dalsi->predchozi2==E)break;
				E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
			}
			d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
			E=NULL;delete E;
		}
		else
		{
			posledni_editovany_element->X=posledni_editovany_element->geo.X1;
			posledni_editovany_element->Y=posledni_editovany_element->geo.Y1;
			d.v.vloz_G_element(posledni_editovany_element,0,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,OBJEKT_akt->orientace);
			posledni_editovany_element=NULL;
		}
		TIP="";
		vytvor_statickou_scenu();
		REFRESH();
		vytvor_obraz();
		nahled_ulozit(true);
	}
	else if(Akce==GEOMETRIE && Akce_temp!=EDITACE_TEXTU && TIP=="")zobraz_tip(ls->Strings[311]);
}
//---------------------------------------------------------------------------
//ukončení akce geometrie a případné uzavření kruhu
void TForm1::ukonceni_geometrie(bool kontrola)
{
	log(__func__);//logování

	////aktualizace popisků
	//if(kontrola)d.v.uprav_popisky_elementu(NULL);

	//////vypnutí akce
	Akce=NIC;
	if(Akce_temp!=EDITACE_TEXTU)Akce_temp=NIC;//musí být ještě před refresh

	////kontrola větví
	posledni_editovany_element=OBJEKT_akt->element;
	Cvektory::T2Element *VYH=d.v.hlavicka_seznam_VYHYBKY();
	while(posledni_editovany_element!=NULL)
	{
		if(posledni_editovany_element->dalsi!=NULL && posledni_editovany_element->dalsi->eID==301 && posledni_editovany_element->dalsi->predchozi2==posledni_editovany_element){napoj_vetev_na_geo();break;}
		posledni_editovany_element=d.v.dalsi_krok(VYH,posledni_editovany_element,OBJEKT_akt);
	}
	d.v.vymaz_seznam_VYHYBKY(VYH);
  posledni_editovany_element=NULL;delete posledni_editovany_element;
	////připnutí objektů
	if(kontrola)
	{
		bool zmena=pripnuti_dalsich_objektu();
		if(OBJEKT_akt->n==1 || OBJEKT_akt->n==d.v.OBJEKTY->predchozi->n || zmena)
		{
			if(zmena)REFRESH();
			spojeni_prvni_posledni();//dalšího objektu
		}
	}
	//////navrácení původních hodnot
	vypni_geometrii();
	//pokud existují výhybky může dojít v průběhu editace k umožnění nebo znemožnění přiřazovat pohon na vedlejší větev, tato metoda nastaví mGridy na výchozí
	//if(d.v.pocet_vyhybek>0)mGrid_on_mGrid();
	//kontrola zda editací geometrie nedošlo ke změně
	if(kontrola)d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teploměry aktualizuje ji, jinak vytvoří default cestu
	if(kontrola)vlozit_predavaci_misto_aktualizuj_WT();
	nahled_ulozit(true);
	//aktualizace comb ve výhybkách
	Cvektory::TElement *E=OBJEKT_akt->element;
	Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		if(E->eID==300)
		{
			napln_comba_mGridu(E);
			FormX->update_hodnot_vyhybky_PM(E);//musí být skrze možnost smazání sekundární větve
		}
		E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
	}
	d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	E=NULL;delete E;
	//validovat
	duvod_validovat=2;
	//kurzor
	if(Screen->Cursor!=standard)kurzor(standard);
	//překreslení
	d.SCENA=0;
	REFRESH();
  d.nabuffrovat_mGridy();//nutné pro zprávné zobrazení, pokud mGridy nebyly vykresleny před zahájením geometrie
}
//---------------------------------------------------------------------------
//vrátí maximální možný počet vozíků na stopce, podle geometrie před ní
double TForm1::max_voziku(Cvektory::TElement *stopka)
{
	log(__func__);//logování
	double ret=1;
	double delka=stopka->geo.delka;
	if(delka>0 && !(d.v.PP.delka_podvozek<m.UDJ(d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(stopka))))//musí být něco v délce, pokud nula tak problém
	{
		Cvektory::TElement *E=stopka->predchozi;
		while(E!=NULL && E->n>0)//cyklus projde předchazijicí liniové úseky (z nich počítá délku) a zastaví až na nelionovém nebo nalezeném funkčním elementu
		{
			if(E!=NULL && E->n>0 && E->geo.typ==0 && E->objekt_n==stopka->objekt_n && E->eID==200 && E->geo.orientace==stopka->geo.orientace)delka+=E->geo.delka;
			else break;
			E=E->predchozi;
		}
		short predchazi_SG_element=1;/*NE*/ if(stopka->predchozi!=NULL && stopka->predchozi->data.pocet_voziku>0)predchazi_SG_element=0;/*ANO*///pokud je buffer až předchozímu S&G elementu, potom uchyt pozice nemám vliv na zvětšení bufferu, protože i na předcházející stopce zabírá (přesahuje) stejný prostor
		E=NULL;delete E;
		if(delka>0)ret=floor((delka+d.v.PP.uchyt_pozice*predchazi_SG_element)/d.v.PP.delka_podvozek);
		if(ret<1)ret=1;
	}
	//kontrola při posunu, zda sem nepřekročil možnosti
	if(Akce==MOVE_ELEMENT && ret<stopka->data.pocet_voziku)stopka->data.pocet_voziku=ret;
	//navracení hodnoty
	return ret;
}
//---------------------------------------------------------------------------
//kontrola zde je mGrid mimo obraz, pokud ano vypnutí komponent
void TForm1::mGrid_mimo_obraz(Cvektory::TElement *E)
{
	log(__func__);//logování
	double presah;
	int pocet_radku;//nemůže být unsigned
	if(E!=NULL && E->eID!=100 && E->eID!=MaxInt)///////////////tabulky elementů
	{
		//////kontrola, zda jsou řádky pod spodní lištou
		presah=m.L2Py(E->Yt)+E->mGrid->Height-scGPPanel_bottomtoolbar->Top;
  	if(presah>0)
		{
			pocet_radku=Ceil(presah/(double)E->mGrid->DefaultRowHeight);
			E->mGrid->exBUTTONVisible=false;
			long radek=E->mGrid->RowCount-1;
			while(pocet_radku!=0 && radek!=0)
			{
				if(E->mGrid->Rows[radek].Visible)
				{
			  	for(unsigned int j=1;j<=E->mGrid->ColCount-1;j++)
					{
						if(E->mGrid->Cells[j][radek].Type!=E->mGrid->DRAW)mGrid_komponenta_na_draw(E->mGrid,j,radek);
					}
					pocet_radku--;
				}
				radek--;
			}
		}
		//////kontrola, zda je sloupec s komponenty za levou knihovnou
		long sirka=m.L2Px(E->Xt);//default hodnota, ke které se bude připočítávat šířka
		for(unsigned int j=1;j<E->mGrid->ColCount;j++)
		{
			sirka+=E->mGrid->Columns[j-1].Width;
			if(sirka<scSplitView_LEFTTOOLBAR->Width)
			{
				for(unsigned int i=1;i<=E->mGrid->RowCount-1;i++)
				{
					if(E->mGrid->Cells[j][i].Type!=E->mGrid->DRAW)mGrid_komponenta_na_draw(E->mGrid,j,i);
				}
			}
		}
  	//////kontrola horní lišta
  	presah=scGPPanel_mainmenu->Height-m.L2Py(E->Yt);
  	pocet_radku=Ceil(presah/(double)E->mGrid->DefaultRowHeight);
		if(pocet_radku>=1)
		{
			for (unsigned int i=0;i<=E->mGrid->RowCount-1 && i<(unsigned)pocet_radku; i++)
  		{
				for(unsigned int j=1;j<=E->mGrid->ColCount-1;j++)
				{
					if(E->mGrid->Cells[j][i].Type!=E->mGrid->DRAW)mGrid_komponenta_na_draw(E->mGrid,j,i);
				}
  		}
		}
	}

  /////////PmG
	if(E==NULL && PmG!=NULL)
	{
  	//////kontrola, zda jsou řádky pod spodní lištou
  	presah=m.L2Py(OBJEKT_akt->Yp)+PmG->Height-scGPPanel_bottomtoolbar->Top;
  	if(presah>0)
  	{
  		pocet_radku=Ceil(presah/(double)PmG->DefaultRowHeight);
			PmG->exBUTTONVisible=false;
			int p_temp=0,i=PmG->RowCount-1;
			while(p_temp!=pocet_radku && i!=0)
			{
				if(PmG->Rows[i].Visible)p_temp++;
				if(PmG->Cells[3][i].Type!=PmG->DRAW)mGrid_komponenta_na_draw(PmG,3,i);
				i--;
			}
  	}
  	//////kontrola, zda je první sloupec tabulky pohonu pod knihovnou
  	if(m.L2Px(OBJEKT_akt->Xp)<scSplitView_LEFTTOOLBAR->Width && PmG->Cells[3][0].Type!=PmG->DRAW)mGrid_komponenta_na_draw(PmG,3,0);
  	//////kontrola, zda je sloupec s komponenty za levou knihovnou
  	if(m.L2Px(OBJEKT_akt->Xp)+PmG->Columns[0].Width+PmG->Columns[1].Width+PmG->Columns[2].Width<scSplitView_LEFTTOOLBAR->Width)
  	{
			for(unsigned int i=0;i<=PmG->RowCount-1;i++)
  		{
				if(PmG->Cells[3][i].Type!=PmG->DRAW)mGrid_komponenta_na_draw(PmG,3,i);
  		}
  	}
  	//////kontrola horní lišta
  	presah=scGPPanel_mainmenu->Height-m.L2Py(OBJEKT_akt->Yp);
		pocet_radku=Ceil(presah/(double)PmG->DefaultRowHeight);
		if(pocet_radku>=1)
		{
			unsigned int p_temp=0,i=0;
			while(p_temp!=(unsigned)pocet_radku && i<=PmG->RowCount-1 )
			{
				if(PmG->Rows[i].Visible)p_temp++;
				//if(PmG->Cells[0][i].Type!=PmG->DRAW)mGrid_komponenta_na_draw(PmG,0,i);
				if(PmG->Cells[3][i].Type!=PmG->DRAW)mGrid_komponenta_na_draw(PmG,3,i);
				i++;
			}
		}
	}
}
//---------------------------------------------------------------------------
//vrátí oblast pro kontrolu překrytí mGridů
TRect TForm1::oblast_prekryti_mGridu(Cvektory::TElement *E)
{
	//deklarace
	TRect ret;
	long odX=0,odY=0;

	//oblsat pro tabulku pohonu
	if(E==NULL && PmG!=NULL)
	{
		ret.left=m.L2Px(OBJEKT_akt->Xp);
		ret.top=m.L2Py(OBJEKT_akt->Yp);
		ret.right=m.L2Px(OBJEKT_akt->Xp)+PmG->Width;
		ret.bottom=m.L2Py(OBJEKT_akt->Yp)+PmG->Height;
	}

	//oblast pro tabulky elementů
	if(E!=NULL)
	{
		for(unsigned int i=0;i<E->mGrid->ColCount-1;i++){if((E->eID==200 || E->eID==300 || E->eID==301) && i==E->mGrid->ColCount-2)break;odX+=E->mGrid->Columns[i].Width;}
		ret.left=m.L2Px(E->Xt)+odX;
		ret.right=m.L2Px(E->Xt)+E->mGrid->Width;
		for(unsigned int j=0;j<E->mGrid->RowCount;j++){if(E->mGrid->Cells[E->mGrid->ColCount-1][j].Type!=E->mGrid->Ttype::DRAW && E->mGrid->Cells[E->mGrid->ColCount-1][j].Type!=E->mGrid->Ttype::glyphBUTTON)break;odY+=E->mGrid->Rows[j].Height;}
		ret.top=m.L2Py(E->Yt)+odY;
		odY=E->mGrid->Height;
		for(unsigned int j=E->mGrid->RowCount-1;j>=0;j--){if(E->mGrid->Cells[E->mGrid->ColCount-1][j].Type!=E->mGrid->Ttype::DRAW)break;odY-=E->mGrid->Rows[j].Height;}
		ret.bottom=m.L2Py(E->Yt)+odY;
	}

  //vracení výsledku
	return ret;
}
//---------------------------------------------------------------------------
//prohledá zda se překrývají mGridy
void TForm1::mGrid_on_mGrid()
{
	log(__func__);//logování
	if(OBJEKT_akt!=NULL && OBJEKT_akt->zobrazit_mGrid)//pokud existuje editovaný objekt a jsou zobrazeny mGridy
	{
    ////zablokování OnChange tabulek
		FormX->input_state=FormX->NO;
		FormX->vstoupeno_poh=false;
		FormX->vstoupeno_elm=false;
		////deklarace proměnných
		Cvektory::TElement *E=OBJEKT_akt->element,*prekryty=NULL;
		Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
		TRect tab1,tab_PmG,R;
		TPoint p1,p2,p3,p4;

		////načtení rozměrů PmG
		if(PmG!=NULL)
		{
			tab_PmG=oblast_prekryti_mGridu();
			////uvedení do původního stavu + kontrola zda není mGrid mimo obraz
			mGrid_puvodni_stav();//uvedení PmG do původního stavu
			mGrid_mimo_obraz();//kontrola + ošetření když je PmG mimo obraz
		}

		////původní stavy tabulek + tabulky mimo obraz
		if(predchozi_PM!=NULL)E=predchozi_PM;//zahrnutí předchozího PM do průchodu
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		while(E!=NULL)
		{
			mGrid_puvodni_stav(E);
			mGrid_mimo_obraz(E);//kontrola + ošetření mGridů, ktěré se nacházejí mimo obraz
			if(predchozi_PM!=NULL && E==predchozi_PM)E=OBJEKT_akt->element;//pokud jsem byl na předchozím PM přesun na první element v objektu
			else E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);//přesun po elementech v objektu
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
    //teploměry
		if(T!=NULL)
		{
			mGrid_puvodni_stav(T->posledni);
			mGrid_mimo_obraz(T->posledni);//kontrola + ošetření mGridů, ktěré se nacházejí mimo obraz
		}
			;
		////kontrola překrytí
		if(predchozi_PM!=NULL)E=predchozi_PM;//zahrnutí předchozího PM do průchodu
		else E=OBJEKT_akt->element;
		Cvektory::T2Element *VYH=d.v.hlavicka_seznam_VYHYBKY();
		while(E!=NULL)
		{
			if(E->eID!=100 && E->eID!=MaxInt)//pouze pro elementy, které mají tabulku, jen pro tabulky co jsou zobrazené
			{
				//naplnění TRectu oblastí tabulky
				tab1=oblast_prekryti_mGridu(E);
				//průchod všemi ostatními elementy, hledání zda se nepřekrývají s tab1
				Cvektory::TElement *E_temp=E->dalsi;
        if(E==predchozi_PM)E_temp=OBJEKT_akt->element;
				Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
				while(E_temp!=NULL && E_temp->objekt_n==OBJEKT_akt->n)
				{
					if(E_temp->eID!=100 && E_temp->eID!=MaxInt)//přeskakovat element s tab1, pouze pro elementy, které mají tabulku
					{
						//definice bodů tabulky
						R.left=m.L2Px(E_temp->Xt);R.right=m.L2Px(E_temp->Xt)+E_temp->mGrid->Width;
						R.top=m.L2Py(E_temp->Yt);R.bottom=m.L2Py(E_temp->Yt)+E_temp->mGrid->Height;
						//kontrola, zda se některý z bodů druhé tabulky nenachází v první tabulce
						if(tab1.Intersects(R))
						{
							//nalezeno překrytí, uložení tabulky která je vykreslena dříve
							prekryty=E;
						}
					}
					E_temp=d.v.dalsi_krok(VYHYBKY,E_temp,OBJEKT_akt);
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
				E_temp=NULL;delete E_temp;
				//kontrola překrytí s tabulkou pohonu
				if(prekryty==NULL && PmG!=NULL && tab1.Intersects(tab_PmG))
				{
					//nalezeno překrytí, uložení tabulky elementu
					prekryty=E;
				}
				//nebyla nalezena tabulka elementu v překryvu, kontrola tabulky teploměrů
				if(prekryty==NULL && T!=NULL)
				{
					//definice bodů tabulky
					R.left=m.L2Px(T->posledni->Xt);R.right=m.L2Px(T->posledni->Xt)+T->posledni->mGrid->Width;
					R.top=m.L2Py(T->posledni->Yt);R.bottom=m.L2Py(T->posledni->Yt)+T->posledni->mGrid->Height;
          //kontrola, zda se některý z bodů druhé tabulky nenachází v první tabulce
					if(tab1.Intersects(R))
					{
						//nalezeno překrytí, uložení tabulky která je vykreslena dříve
						prekryty=E;
					}
				}
			}

      //řešení překrytí
			if(prekryty!=NULL)
			{
	  		unsigned int Col=prekryty->mGrid->ColCount-1;
				if(prekryty->eID==200 || prekryty->eID==300)Col=3;
        if(prekryty->eID==301)Col=1;
				for(unsigned int j=Col;j<prekryty->mGrid->ColCount;j++)
	  		{
	  			for(unsigned int i=1;i<prekryty->mGrid->RowCount;i++)
	  			{
	  				if(prekryty->mGrid->Cells[j][i].Type!=TmGrid::DRAW && prekryty->mGrid->Cells[j][i].Type!=TmGrid::glyphBUTTON)mGrid_komponenta_na_draw(prekryty->mGrid,j,i);//kontrola zda jsem narazil na komponentu nikoliv na buňku, změna na DRAW
	  			}
				}
				prekryty=NULL;//nulování pro další použití
	  	}

			//posun na další
			if(predchozi_PM!=NULL && E==predchozi_PM)E=OBJEKT_akt->element;//pokud jsem byl na předchozím PM přesun na první element v objektu
			else E=d.v.dalsi_krok(VYH,E,OBJEKT_akt);//posun po elementech v objektu
		}
		d.v.vymaz_seznam_VYHYBKY(VYH);

		////navrácení akce
		FormX->input_state=FormX->NOTHING;

		////ukazatelové záležitosti
    E=NULL;delete E;
		prekryty=NULL;delete prekryty;
		T=NULL;delete T;
	}
}
//---------------------------------------------------------------------------
//smazě komponentu v dané buňce a změní typ bunky na DRAW
void TForm1::mGrid_komponenta_na_draw(TmGrid *mGrid,long Col,long Row)
{
	log(__func__);//logování
	bool prepnout=true;
  nastav_focus();//předání focusu, následuje mazání komponenty
	//načtení komponent, v mGridech pouze edity a comba
	TscGPEdit *E=mGrid->getEdit(Col,Row);
	TscGPComboBox *C=mGrid->getCombo(Col,Row);
	TscGPGlyphButton *B=mGrid->getGlyphButton(Col,Row);
	//smazání komponent
	if(E!=NULL)
	{
		if(!E->Enabled)
		{
			mGrid->Cells[Col][Row].Background->Color=(TColor)RGB(240,240,240);
			mGrid->Cells[Col][Row].Font->Color=(TColor)RGB(128,128,128);
		}
		E->Free();
		E=NULL;delete E;
	}
	if(C!=NULL)
	{
		//přepsání výberu z comba do buňky
		mGrid->Cells[Col][Row].Text=C->Items->operator [](C->ItemIndex)->Caption;
		if(!C->Enabled)
		{
			mGrid->Cells[Col][Row].Background->Color=(TColor)RGB(240,240,240);
			mGrid->Cells[Col][Row].Font->Color=(TColor)RGB(128,128,128);
		}
		C->Free();
		C=NULL;delete C;
	}
	if(B!=NULL)
	{
		B->Visible=false;
		prepnout=false;
  }
	//mazání ukazatelů
	B=NULL;delete B;
	E=NULL;C=NULL;delete E;delete C;
	//změna typu buňky
	if(prepnout)mGrid->Cells[Col][Row].Type=mGrid->DRAW;
}
//---------------------------------------------------------------------------
//nadesingnuje tabulky elementů nebo tabulku pohonu na původní stav, obnový komponenty, naplní comba, provede Update() mGridu
void TForm1::mGrid_puvodni_stav(Cvektory::TElement *E)
{
	log(__func__);//logování
	bool doslo_ke_zmene=false;
	if(E==NULL && PmG!=NULL)
	{
		bool update_probehl=false;
		if(PmG->Cells[3][0].Type!=PmG->COMBO)
		{
			PmG->Cells[3][0].Type=PmG->COMBO;
			PmG->Update();update_probehl=true;
			tab_pohon_COMBO();
		}
		if(!PmG->exBUTTONVisible)PmG->exBUTTONVisible=true;
		aktualizace_tab_pohon(false,false,true);//aktualizace komponent
		if(!update_probehl)PmG->Update();
	}
	else if(E->mGrid!=NULL)
	{

		switch(E->eID)
		{
  		case 0://stop stanice, nastavování režimů podle ID objektu
  		{
				E->mGrid->Cells[2][3].Type=E->mGrid->EDIT;E->mGrid->Cells[2][6].Type=E->mGrid->EDIT;
				////temp odemknutí editace max WT
				E->mGrid->Cells[2][4].Type=E->mGrid->EDIT;//cokoliv
				////konec temp
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->Update();
  			break;
  		}
  		case 7:case 11:case 15:case 101:case 105:
  		case 1://robot (kontinuální)
  		{
				E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
				E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;
				E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;
				E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;
				if(E->mGrid->Cells[1][6].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[1][6].Type=E->mGrid->COMBO;
				E->mGrid->Update();
  			break;
  		}
  		case 8:case 12:case 16:case 102:case 106:
  		case 2://robot se stop stanicí
  		{
				E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
        ////temp odemknutí editace max WT
				E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;//cokoliv
				////konec temp
				E->mGrid->Update();
  			break;
  		}
  		case 9:case 13:case 17:case 103:case 107:
  		case 3://robot s pasivní otočí
  		{
				E->mGrid->Cells[3][1].Type=E->mGrid->EDIT;
				E->mGrid->Cells[3][2].Type=E->mGrid->EDIT;
				E->mGrid->Cells[3][3].Type=E->mGrid->EDIT;
				if(E->mGrid->Cells[3][4].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[3][4].Type=E->mGrid->COMBO;
				E->mGrid->Cells[3][6].Type=E->mGrid->EDIT;
        E->mGrid->Cells[3][9].Type=E->mGrid->EDIT;
				E->mGrid->Cells[3][10].Type=E->mGrid->EDIT;
				E->mGrid->Cells[3][11].Type=E->mGrid->EDIT;
				if(E->mGrid->Cells[3][13].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[3][13].Type=E->mGrid->COMBO;
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->Update();
  			break;
  		}
  		case 10:case 14:case 18:case 104:case 108:
  		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
  		{
				E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
				if(E->mGrid->Cells[1][1].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
  			E->mGrid->Cells[1][2].Type=E->mGrid->COMBO;
  			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;
  			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;
				E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;
				E->mGrid->Update();
  			break;
  		}
  		case 5://otoč pasivní
			{
				if(E->mGrid->Cells[2][1].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[2][1].Type=E->mGrid->COMBO;
  			E->mGrid->Cells[2][2].Type=E->mGrid->EDIT;
				E->mGrid->exBUTTONVisible=true;
				E->mGrid->Update();
  			break;
  		}
  		case 6://otoč aktivní (resp. otoč se stop stanicí)
  		{
				if(E->mGrid->Cells[1][1].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
				E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;
        E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;
				E->mGrid->Update();
  			break;
			}
			case 200:
			case 300:
			{
				TscGPGlyphButton *B=E->mGrid->getGlyphButton(4,0);
				//if(B!=NULL)B->Visible=true;
				if(E->mGrid->Cells[3][2].Type!=E->mGrid->COMBO || E->mGrid->Cells[4][2].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[3][2].Type=E->mGrid->COMBO;
				E->mGrid->Cells[4][2].Type=E->mGrid->COMBO;
				////temp odemknutí editace max WT
        if(E->mGrid->Cells[3][11].Type!=E->mGrid->COMBO || E->mGrid->Cells[4][11].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[3][11].Type=E->mGrid->COMBO;
				E->mGrid->Cells[4][11].Type=E->mGrid->COMBO;
				////konec temp
				if(E->mGrid->Rows[3].Visible)E->mGrid->exBUTTONVisible=true;
        E->mGrid->Update();
				B=NULL;delete B;
				//edity zařídí fce napln_comba_mGridu(), dynamicky se mění
				break;
			}
			case 301:
			{
				if(E->mGrid->Cells[1][3].Type!=E->mGrid->COMBO || E->mGrid->Cells[2][3].Type!=E->mGrid->COMBO)doslo_ke_zmene=true;
				E->mGrid->Cells[1][3].Type=E->mGrid->COMBO;
				E->mGrid->Cells[2][3].Type=E->mGrid->COMBO;
				break;
			}
			case 400:case 401:case 402:
			{
				E->mGrid->exBUTTONVisible=true;
        E->mGrid->Update();
				break;
      }
			default:break;
		}
		
		////naplnění comb
		if(doslo_ke_zmene)
		{
			napln_comba_mGridu(E);//pro PM a výhybku nastaví edity
			set_enabled_mGrid(E);
		}
	}   
}
//---------------------------------------------------------------------------
//vrací max a min hodnoty x a y souřadnic, všecho v layout(elementů, objektů), nebo parametrem Objekt lze hledat max souřadnice v jednom objektu
TRect TForm1::vrat_max_oblast(Cvektory::TObjekt *Objekt,bool pouze_body)
{
	log(__func__);//logování
	TRect ret;
	double konst=Zoom/5.0;
	//nastavení základníh hodnot sloužících pro vyhledávání
	ret.left=MaxInt;ret.right=MaxInt*(-1);
	ret.top=MaxInt;ret.bottom=MaxInt*(-1);
	//hledání souřadnic v jednom objektu
	if(Objekt!=NULL)
	{
		Cvektory::TBod *B=Objekt->body->dalsi;
		while(B!=NULL)
		{
			if(m.L2Px(B->X)<ret.left)ret.left=m.L2Px(B->X);
			if(m.L2Px(B->X)>ret.right)ret.right=m.L2Px(B->X);
			if(m.L2Py(B->Y)<ret.top)ret.top=m.L2Py(B->Y);
			if(m.L2Py(B->Y)>ret.bottom)ret.bottom=m.L2Py(B->Y);
			B=B->dalsi;
		}
		delete B;B=NULL;
		//kontrola názvu
		if(!pouze_body)
		{
	  	d.nastavit_text_popisu_objektu_v_nahledu(Canvas);
	  	AnsiString Tn=Objekt->name.UpperCase();
	  	short Wn=Canvas->TextWidth(Tn)/2.0;
	  	short Hn=Canvas->TextHeight(Tn);
	  	if(Objekt->orientace_text==0 || Objekt->orientace_text==180){Wn=Hn;Hn=Canvas->TextWidth(Tn)/2.0;}
	  	if(Objekt->orientace_text!=180 && m.L2Px(Objekt->Xt)-Wn<ret.left)ret.left=m.L2Px(Objekt->Xt)-Wn;
	  	if(Objekt->orientace_text==180 && m.L2Px(Objekt->Xt)<ret.left)ret.left=m.L2Px(Objekt->Xt);
	  	if(m.L2Px(Objekt->Xt)+Wn>ret.right)ret.right=m.L2Px(Objekt->Xt)+Wn;
	  	if(m.L2Py(Objekt->Yt)-Hn<ret.top)ret.top=m.L2Py(Objekt->Yt)-Hn;
	  	if(Objekt->orientace==90 || Objekt->orientace==270)Hn=0;
	  	if(m.L2Py(Objekt->Yt)+Hn>ret.bottom)ret.bottom=m.L2Py(Objekt->Yt)+Hn;
		}
		//kontrola tabulky předchozího PM
		if(!pouze_body && predchozi_PM!=NULL)
		{
			if(m.L2Px(predchozi_PM->Xt)<ret.left)ret.left=m.L2Px(predchozi_PM->Xt);
			if(predchozi_PM->mGrid!=NULL && m.L2Px(predchozi_PM->Xt)+predchozi_PM->mGrid->Width*konst>ret.right)ret.right=m.L2Px(predchozi_PM->Xt)+predchozi_PM->mGrid->Width*konst;
			if(m.L2Py(predchozi_PM->Yt)<ret.top)ret.top=m.L2Py(predchozi_PM->Yt);
			if(predchozi_PM->mGrid!=NULL && m.L2Py(predchozi_PM->Yt)+predchozi_PM->mGrid->Height*konst>ret.bottom)ret.bottom=m.L2Py(predchozi_PM->Yt)+predchozi_PM->mGrid->Height*konst;
		}
		//kontrola tabulky teploměrů
		if(!pouze_body && Objekt->teplomery!=NULL)
		{
			Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(Objekt,d.v.ZAKAZKA_akt);
			if(T!=NULL && T->posledni->mGrid!=NULL)
			{
				if(m.L2Px(T->posledni->Xt)<ret.left)ret.left=m.L2Px(T->posledni->Xt);
				if(T->posledni->mGrid!=NULL && m.L2Px(T->posledni->Xt)+T->posledni->mGrid->Width*konst>ret.right)ret.right=m.L2Px(T->posledni->Xt)+T->posledni->mGrid->Width*konst;
				if(m.L2Py(T->posledni->Yt)<ret.top)ret.top=m.L2Py(T->posledni->Yt);
				if(T->posledni->mGrid!=NULL && m.L2Py(T->posledni->Yt)+T->posledni->mGrid->Height*konst>ret.bottom)ret.bottom=m.L2Py(T->posledni->Yt)+T->posledni->mGrid->Height*konst;
			}
			T=NULL;delete T;
		}
		//kontrola def tabulky pohonu
		if(!pouze_body && PmG!=NULL)
		{
			if(m.L2Px(Objekt->Xp)<ret.left)ret.left=m.L2Px(Objekt->Xp);
			if(PmG!=NULL && m.L2Px(Objekt->Xp)+PmG->Width*konst>ret.right)ret.right=m.L2Px(Objekt->Xp)+PmG->Width*konst;
			if(m.L2Py(Objekt->Yp)<ret.top)ret.top=m.L2Py(Objekt->Yp);
			if(PmG!=NULL && m.L2Py(Objekt->Yp)+PmG->Height*konst>ret.bottom)ret.bottom=m.L2Py(Objekt->Yp)+PmG->Height*konst;
		}
		//kontrola tabulek elementů v objektu
		if(!pouze_body && Objekt==OBJEKT_akt)
		{
			Cvektory::TElement *E=Objekt->element;
      Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
			while(E!=NULL)
			{
				if(E->eID!=100 && E->eID!=MaxInt)//vynechat elementy co nemají tabulky
				{
			  	if(m.L2Px(E->Xt)<ret.left)ret.left=m.L2Px(E->Xt);
					if(E->mGrid!=NULL && m.L2Px(E->Xt)+E->mGrid->Width*konst>ret.right)ret.right=m.L2Px(E->Xt)+E->mGrid->Width*konst;
					if(m.L2Py(E->Yt)<ret.top)ret.top=m.L2Py(E->Yt);
					if(E->mGrid!=NULL && m.L2Py(E->Yt)+E->mGrid->Height*konst>ret.bottom)ret.bottom=m.L2Py(E->Yt)+E->mGrid->Height*konst;
				}
        //kontrola zda nejsou elementy mimo kabinu
				if(m.L2Px(E->geo.X4)<ret.left)ret.left=m.L2Px(E->geo.X4);
				if(m.L2Px(E->geo.X4)>ret.right)ret.right=m.L2Px(E->geo.X4);
				if(m.L2Py(E->geo.Y4)<ret.top)ret.top=m.L2Py(E->geo.Y4);
				if(m.L2Py(E->geo.Y4)>ret.bottom)ret.bottom=m.L2Py(E->geo.Y4);
				E=d.v.dalsi_krok(VYHYBKY,E,Objekt);
			}
			d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
			E=NULL;delete E;
		}
	}
	//hledání souřadnic v celém layoutu
	else
	{
		Cvektory::TObjekt *O=d.v.OBJEKTY;
		while(O!=NULL)
		{
			if(O->n>0)//ošetření proti práznému spojáku
			{
				//kontrola bodů
				Cvektory::TBod *B=O->body->dalsi;
				while(B!=NULL)
				{
					if(m.L2Px(B->X)<ret.left)ret.left=m.L2Px(B->X);
					if(m.L2Px(B->X)>ret.right)ret.right=m.L2Px(B->X);
					if(m.L2Py(B->Y)<ret.top)ret.top=m.L2Py(B->Y);
					if(m.L2Py(B->Y)>ret.bottom)ret.bottom=m.L2Py(B->Y);
					B=B->dalsi;
				}
				delete B;B=NULL;
				//kontrola názvu
				d.nastavit_text_popisu_objektu_v_nahledu(Canvas);
				AnsiString Tn=O->name.UpperCase();
				short Wn=Canvas->TextWidth(Tn)/2.0;
				short Hn=Canvas->TextHeight(Tn);
				if(O->orientace_text==0 || O->orientace_text==180){Wn=Hn;Hn=Canvas->TextWidth(Tn)/2.0;}
				if(O->orientace_text!=180 && m.L2Px(O->Xt)-Wn<ret.left)ret.left=m.L2Px(O->Xt)-Wn;
				if(O->orientace_text==180 && m.L2Px(O->Xt)<ret.left)ret.left=m.L2Px(O->Xt);
				if(m.L2Px(O->Xt)+Wn>ret.right)ret.right=m.L2Px(O->Xt)+Wn;
				if(m.L2Py(O->Yt)-Hn<ret.top)ret.top=m.L2Py(O->Yt)-Hn;
				if(O->orientace==90 || O->orientace==270)Hn=0;
				if(m.L2Py(O->Yt)+Hn>ret.bottom)ret.bottom=m.L2Py(O->Yt)+Hn;
			}
			O=O->dalsi;
		}
		delete O,O=NULL;
		//kontrola elementů, v případě, že je nakreslena geometrie mimo kabinu
		Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		while(E!=NULL)
		{
			if(m.L2Px(E->X)<ret.left)ret.left=m.L2Px(E->X);
			if(m.L2Px(E->X)>ret.right)ret.right=m.L2Px(E->X);
			if(m.L2Py(E->Y)<ret.top)ret.top=m.L2Py(E->Y);
			if(m.L2Py(E->Y)>ret.bottom)ret.bottom=m.L2Py(E->Y);
			E=d.v.dalsi_krok(VYHYBKY,E);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		delete E;E=NULL;
		//kontrola haly
		if(d.v.HALA.body!=NULL && d.v.HALA.body->dalsi!=NULL)
		{
    	Cvektory::TBod *B=d.v.HALA.body->dalsi;
			while(B!=NULL)
			{
				if(m.L2Px(B->X)<ret.left)ret.left=m.L2Px(B->X);
				if(m.L2Px(B->X)>ret.right)ret.right=m.L2Px(B->X);
				if(m.L2Py(B->Y)<ret.top)ret.top=m.L2Py(B->Y);
				if(m.L2Py(B->Y)>ret.bottom)ret.bottom=m.L2Py(B->Y);
				B=B->dalsi;
			}
			delete B;B=NULL;
    }
		//kontrola rastru
		if(d.v.PP.raster.show && FileExists(d.v.PP.raster.filename))
		{
      Graphics::TBitmap *bmp=new Graphics::TBitmap;
			bmp->LoadFromFile(d.v.PP.raster.filename);
			//kontrola krajních bodů rastru
			if(m.L2Px(d.v.PP.raster.X)<ret.left)ret.left=m.L2Px(d.v.PP.raster.X);
			if(m.L2Px(d.v.PP.raster.X)+bmp->Width*Zoom*d.v.PP.raster.resolution/m2px>ret.right)ret.right=m.L2Px(d.v.PP.raster.X)+bmp->Width*Zoom*d.v.PP.raster.resolution/m2px;
			if(m.L2Py(d.v.PP.raster.Y)<ret.top)ret.top=m.L2Py(d.v.PP.raster.Y);
			if(m.L2Py(d.v.PP.raster.Y)+bmp->Height*Zoom*d.v.PP.raster.resolution/m2px>ret.bottom)ret.bottom=m.L2Py(d.v.PP.raster.Y)+bmp->Height*Zoom*d.v.PP.raster.resolution/m2px;
			delete(bmp);
		}
	}
	return ret;
}
//---------------------------------------------------------------------------
//vrátí max zobrazitelnou (na monitoru) oblast z metody výše uvedené vrat_max_oblast
TRect TForm1::vrat_max_zobrazitelnou_oblast()
{
	TRect oblast=vrat_max_oblast();//vrácení celé aktuální oblasti ve fyzických souřadnicích
	short R=50;//prozatimní neexaktní řešení
	oblast.Left-=R;
	oblast.Right+=R;
	oblast.Top-=R;
	oblast.Bottom+=R;
	if(oblast.Left<0)oblast.Left=0;
	if(ClientWidth-1<oblast.Right)oblast.Right=ClientWidth-1;
	if(oblast.Top<0)oblast.Top=0;
	if(ClientHeight-1<oblast.Bottom)oblast.Bottom=ClientHeight-1;
	return oblast;
}
//---------------------------------------------------------------------------
//vrací velikosti oblastí lakovacích oken, .x = oblast před, .y = oblast za
TPointD TForm1::oblast_LO(Cvektory::TElement *E)
{
	log(__func__);
	//deklarce
	TPointD ret;
	ret.x=0;ret.y=0;

	//má smysl pouze u KK elementů
	if(d.v.vrat_druh_elementu(E)==1)
	{
		switch(E->eID)
		{
			//otoč
			case 5:
			{
				ret.x=E->zona_pred;
				ret.y=E->zona_za;
				break;
			}
			//ion. tyč, PM, výhybka, spojka, zarážka
			case 100:case 200:case 300:case 301:case MaxInt:break;
			//roboti a operátoři
			default:
  		{
				//element se 2 lakovacími okny
				if(E->data.LO2>0)
				{
					ret.x=E->data.LO1+(E->OTOC_delka)/2.0+E->data.LO_pozice;
  				ret.y=E->data.LO2+(E->OTOC_delka)/2.0+E->data.LO_pozice;
				}
  			//element s 1 lakovacími okny
  	  	else
				{
  				ret.x=ret.y=E->data.LO1/2.0+E->data.LO_pozice;
				}
				break;
			}
		}
	}

	//vracení výsledku
	return ret;
}
//---------------------------------------------------------------------------
//prozkoumá zda se element nepřekrýva lak. oknem se sousedními, validace elementu
short TForm1::prekryti_LO(Cvektory::TElement *E)
{
	log(__func__);
	//deklarace
	bool prekryto=false;
	short typ=0;//ukládá v sobě typ překrytí: 0 = žádné, 1 = došlo k překrytí s LO, 2 = došlo k překrytí zón otáčení, 3 = došlo k překrytí PO
	TPointD obE=oblast_LO(E);//.x = oblast před, .y = oblast po

	//první vlna testování
	if(E->geo.delka<obE.x || (E->dalsi!=NULL && E->dalsi->geo.delka<obE.y) || E->dalsi==NULL){prekryto=true;typ=1;if(E->eID==5)typ=2;if(E->eID==7 || E->eID==11 || E->eID==15 || E->eID==101 || E->eID==105 || E->eID==8 || E->eID==12 || E->eID==16 || E->eID==102 || E->eID==106)typ=3;}
	//druhá vlna testování
	if(!prekryto && E->predchozi->n>0)
	{
		TPointD obP=oblast_LO(E->predchozi);
		if(E->geo.delka-obP.y<obE.x || E->geo.delka-obP.y<0){prekryto=true;typ=1;if(E->predchozi->eID==5)typ=2;if(E->predchozi->eID==7 || E->predchozi->eID==11 || E->predchozi->eID==15 || E->predchozi->eID==101 || E->predchozi->eID==105 || E->predchozi->eID==8 || E->predchozi->eID==12 || E->predchozi->eID==16 || E->predchozi->eID==102 || E->predchozi->eID==106)typ=3;}
	}
	//třetí vlna testování
	if(!prekryto && E->dalsi!=NULL)
	{
		TPointD obD=oblast_LO(E->dalsi);
		if(E->dalsi->geo.delka-obD.x<obE.y || E->dalsi->geo.delka-obD.x<0){prekryto=true;typ=1;if(E->dalsi->eID==5)typ=2;if(E->dalsi->eID==7 || E->dalsi->eID==11 || E->dalsi->eID==15 || E->dalsi->eID==101 || E->dalsi->eID==105 || E->dalsi->eID==8 || E->dalsi->eID==12 || E->dalsi->eID==16 || E->dalsi->eID==102 || E->dalsi->eID==106)typ=3;}
	}

	//vracení výsledku
	return typ;
}
//---------------------------------------------------------------------------
//vrati delku v metrech mezi LO elementů
double TForm1::vzdalenost_meziLO(Cvektory::TElement *E,double orientace)
{
	log(__func__);//logování
	double ret=0;
	//hledání předchozího elementu
	Cvektory::TElement *E_pred=E->predchozi;
	while(E_pred!=NULL && E_pred->n>0 && E_pred->objekt_n==E->objekt_n)
	{
		if(E_pred->geo.typ!=0)break;
		if(E_pred->eID!=MaxInt)break;//kota element - element
		E_pred=E_pred->predchozi;
	}
	if(E_pred->objekt_n!=E->objekt_n)E_pred=NULL;
	if(E_pred!=NULL && (E_pred->data.LO1>0 || E_pred->data.LO2>0))
	{
		ret=E->geo.delka-oblast_LO(E_pred).y-oblast_LO(E).x;
	}
	E_pred=NULL;delete E_pred;
	return ret;
}
//---------------------------------------------------------------------------
//projde všechny elementy v aktuálně editovaném objektu a upravím jim RT
void TForm1::aktualizace_RT()
{
	log(__func__);//logování
	if(OBJEKT_akt!=NULL)
	{
		Cvektory::TElement *E=OBJEKT_akt->element;
		Cvektory::TCesta *C=NULL;
		while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
		{
			//aktualizace max počtu vozíků
			if(E->eID==0)
			{
				if(d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n!=0){C=d.v.vrat_segment_cesty(d.v.ZAKAZKA_akt,E);if(C!=NULL)E->data=C->data;}//nahrání aktuálních dat
				E->data.pocet_pozic=max_voziku(E);
				try
				{
					E->mGrid->Cells[2][5].Text=E->data.pocet_pozic;
				}catch(...){}
				d.v.reserve_time(E,C);
				if(C!=NULL)C->data=E->data;//vrácení přepočítaných dat do zakázky
			}
			//aktualizace RT
			if(d.v.vrat_druh_elementu(E)==0 && E->eID!=0)d.v.reserve_time(E);
			E=E->dalsi;
		}
		E=NULL;delete E;
		C=NULL;delete C;
	}
}
//vratí zda jsem v oblasti textu
bool TForm1::najdi_nazev_obj(double X,double Y,Cvektory::TObjekt *Objekt)
{
	log(__func__);//logování
	bool ret=false;
	//nastavení canvasu
	d.nastavit_text_popisu_objektu_v_nahledu(Canvas);AnsiString Tn=Objekt->name.UpperCase();short Wn=Canvas->TextWidth(Tn);//název objektu - nastavení
	//hledání textu
	switch((int)Objekt->orientace_text)
	{
		case 0:if(m.L2Px(Objekt->Xt)>X && X>m.L2Px(Objekt->Xt)-Canvas->TextHeight(Tn) && m.L2Py(Objekt->Yt)-m.round(Wn)/2.0<Y && Y<m.L2Py(Objekt->Yt)+m.round(Wn)/2.0)ret=true;break;
		case 90:case 270:if(m.L2Px(Objekt->Xt)-m.round(Wn)/2.0<=X && X<=m.L2Px(Objekt->Xt)+m.round(Wn/2.0) && m.L2Py(Objekt->Yt)>=Y && Y>=m.L2Py(Objekt->Yt)-Canvas->TextHeight(Tn))ret=true;break;
		case 180:if(m.L2Px(Objekt->Xt)<X && X<m.L2Px(Objekt->Xt)+Canvas->TextHeight(Tn) && m.L2Py(Objekt->Yt)-m.round(Wn)/2.0<Y && Y<m.L2Py(Objekt->Yt)+m.round(Wn)/2.0)ret=true;break;
	}
	return ret;
}
//---------------------------------------------------------------------------
//najde na jakém popisku u PM se nachází kurzor, vrací 0: nenalezeno, 1: pohon 1, 2: pohon 2
short TForm1::najdi_popisky_PM(double X,double Y,Cvektory::TObjekt *Objekt)
{
	log(__func__);//logování
	//deklarace atributů
	short ret=0;
	if(Akce==NIC && Akce_temp!=EDITACE_TEXTU)
	{
		double Xe,Ye,x1,y1,x2,y2;//pozice popisků u elementu
		Canvas->Font->Size=F->m.round(2.8*Zoom);if(F->aFont->Size==12)Canvas->Font->Size=m.round(2*Zoom);
		int w1,w2;
		int h;
		short K=0.25*Zoom;
		element_temp=NULL;
		//průchod objektu
  	Cvektory::TElement *E=Objekt->element;
  	while(E!=NULL && E->objekt_n==Objekt->n)
  	{
			if(E->eID==200)
			{
				AnsiString T1=ls->Strings[271],T2=ls->Strings[271];//"pohon nevybrán"
	    	if(E->pohon!=NULL)T1=E->pohon->name;
				if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)T2=E->dalsi->pohon->name;
				if(E->dalsi==NULL && d.v.ELEMENTY->dalsi->pohon!=NULL)T2=d.v.ELEMENTY->dalsi->pohon->name;
				if(OBJEKT_akt->orientace==270){AnsiString pom=T1;T1=T2;T2=T1;}//prohození popisků
				w1=Canvas->TextWidth(T1);w2=Canvas->TextWidth(T2);h=Canvas->TextHeight(T1);
				Xe=m.L2Px(E->X);Ye=m.L2Py(E->Y);
				//definice výchozích bodů popisků + zjištění oblasti
				switch((int)m.Rt90(E->geo.orientace-E->geo.rotacni_uhel))
				{
          case 0://ok
					{
						x2=Xe-K;y2=Ye-K-m.m2px(0.2);
						x1=Xe+h-K;y1=Ye+w1+m.m2px(0.4);
						w2=(-1)*(w2+m.m2px(0.2));
						w1=(-1)*(w1-m.m2px(0.2));
					}break;
			  	case 90://ok
			  	{
						x1=Xe-w1-m.m2px(0.4);y1=Ye+K+h;
						x2=Xe+m.m2px(0.2);y2=Ye-K;
						h*=-1;
						w1+=m.m2px(0.2);w2+=m.m2px(0.2);
			  	}break;
			  	case 180:
			  	{
						x1=Xe-K;y1=Ye-w1-m.m2px(0.4);
						x2=Xe+h+K;y2=Ye+m.m2px(0.2);
						w1+=m.m2px(0.2);w2+=m.m2px(0.2);
					}break;
					case 270://ok
			  	{
						x2=Xe-w2-m.m2px(0.4);y1=Ye+K;
						x1=Xe+m.m2px(0.2);y2=Ye-K-h;
						w2+=m.m2px(0.2);w1+=m.m2px(0.2);
					}break;
				}
        //hledání v olbasti
				if(m.Rt90(E->geo.orientace-E->geo.rotacni_uhel)==90 || m.Rt90(E->geo.orientace-E->geo.rotacni_uhel)==270)
				{
					if(m.PtInRectangle(x1,y1,x1+w1,y1+h,X,Y)){ret=1;pom_element=E;break;}
					if(m.PtInRectangle(x2,y2,x2+w2,y2+h,X,Y)){ret=2;pom_element=E;break;}
				}
				else
				{
					if(m.PtInRectangle(x1,y1,x1-h,y1+w1,X,Y)){ret=1;pom_element=E;break;}
					if(m.PtInRectangle(x2,y2,x2-h,y2+w2,X,Y)){ret=2;pom_element=E;break;}
				}
			}
			E=E->dalsi;
		}
		E=NULL;delete E;
	}
	return ret;
}
//---------------------------------------------------------------------------
//vrací bod vložení elementu, "přilepuje" kurzor na geometrii pokud se jedná o přímku, parametry kontr_x a y slouží ke kontrole bodu zda se nachází na přímce (při posunu)
TPointD TForm1::bod_vlozeni_elementu(double kontr_x,double kontr_y)
{
	log(__func__);//logování
	//nastavení atributů
	pom_element_temp=NULL;
	TPointD ret;
	ret.x=-1000;ret.y=-1000;
	double oblast=1,X1,Y1,X2,Y2,x,y;
	if(kontr_x!=-1000){x=kontr_x;y=kontr_y;}
	else {x=akt_souradnice_kurzoru.x;y=akt_souradnice_kurzoru.y;}
	//průchod elementů objektu
	Cvektory::TElement *E=d.v.ELEMENTY->dalsi,*E_pom=NULL;
  Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	if(OBJEKT_akt!=NULL)E=OBJEKT_akt->element;
	while(E!=NULL)
	{
		//přichytávání na konec začátek a konec oblouku
		if(Akce==VYH && prichytavat_k_mrizce==1 && E->geo.typ!=0)
		{
			if(m.delka(x,y,E->geo.X1,E->geo.Y1)<1){ret.x=E->geo.X1;ret.y=E->geo.Y1;pom_element_temp=E;break;}
			if(m.delka(x,y,E->geo.X4,E->geo.Y4)<1){ret.x=E->geo.X4;ret.y=E->geo.Y4;pom_element_temp=E;break;}
		}
		if(E->geo.typ==0 && E->geo.HeightDepp==0 && (E->geo.orientace==m.Rt90(E->geo.orientace) || E->geo.orientace==360))//jen pro přímky 0,90,180,270°, bez S/K
		{
			//nastavení oblasti
			if(E->geo.orientace==90 || E->geo.orientace==270){X1=E->geo.X1;X2=E->geo.X4;Y1=E->geo.Y1+oblast;Y2=E->geo.Y4-oblast;}
			else {X1=E->geo.X1+oblast;X2=E->geo.X4-oblast;Y1=E->geo.Y1;Y2=E->geo.Y4;}
			//hledání zda se kurzor nachází v této oblasti
			if(m.PtInRectangle(X1,Y1,X2,Y2,x,y))
			{
				//přiřazení souřadnic pro vložení
				ret=d.v.bod_na_geometrii(E);
				pom_element_temp=E;//uchování ukazatele pro následné zjišťování orientce symbolu, nelze se již řídit orientaci kabiny
				if(prichytavat_k_mrizce!=1 && Akce==VYH)break;//zastavení cyklu
			}
		}
		if(OBJEKT_akt!=NULL)E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
		else E=d.v.dalsi_krok(VYHYBKY,E);
	}
	d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	//mazání ukazatele
	E=NULL;delete E;
	E_pom=NULL;delete E_pom;
	//vracení souřadnic elementu v logických
	return ret;
}
//---------------------------------------------------------------------------
//upraví bod kurzoru pro vložení elemntu na bod vykreslení elementu (robot na konci ramena)
TPointD TForm1::uprav_bod_vlozeni_elementu(TPointD bod_vlozeni,short rotace_symbolu,int eID)
{
	log(__func__);//logování
	//nastavení atribuutů
	double DoSkRB=0;
	double orientace=90;//default hodnota
	if(OBJEKT_akt!=NULL)orientace=OBJEKT_akt->orientace;
	if(pom_element_temp!=NULL)orientace=pom_element_temp->geo.orientace;//pokud je element vkládán na přímku, ukazatel na element, který uchovává tuto přímku
	if(eID<0)eID=element_id;
	//zohlednění posunu bodu vložení u robotů
	if(1<=eID && eID<=4 || 7<=eID && eID<=18)//pro roboty, které mají uchopovací bod jinde než referenční
	{
		DoSkRB=d.DoSkRB;//délka od středu (uchopovacího bodu) k referenčnímu bodu, doplnit konstanty
		if(rotace_symbolu==90 || rotace_symbolu==180)DoSkRB*=-1;
	}
	if(101<=eID && eID<=108)//pro lidské roboty, které mají uchopovací bod jinde než referenční
	{
		DoSkRB=d.DkRB;//délka od středu (uchopovacího bodu) k referenčnímu bodu, doplnit konstanty
		if(rotace_symbolu==90 || rotace_symbolu==180)DoSkRB*=-1;
	}
	//úprava souřadnic = přichycení na pohon

	if(orientace==90 || orientace==270)bod_vlozeni.y-=DoSkRB;
	else bod_vlozeni.x+=DoSkRB;
	//vrácení upravených souřadnic
	return bod_vlozeni;
}
//---------------------------------------------------------------------------
//kontroluje zde se bod nachází na geometri, vrací pouze ano/ne, pokud je do metody poslán ukazatel na element prověří zda se tento element nachází na geometrii
bool TForm1::bod_na_geometrii(double X, double Y,Cvektory::TElement *Element,bool posun_elementu)
{
	log(__func__);//logování
	bool ret=false;
	if(Element!=NULL){X=d.Rxy(Element).x;Y=d.Rxy(Element).y;}
  //kontrola, zda před elementem není začátek linky nebo oblouk, pokud ano a jsem skoro na 0 zastavení posunu
	if(((Element->predchozi->n>0 && Element->predchozi->geo.typ!=0) || (Element->predchozi->n==0)) && m.delka(X,Y,Element->geo.X1,Element->geo.Y1)<0.01)ret=false;
	else
	{
		Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		while(E!=NULL)
		{
			if((!posun_elementu || (posun_elementu && OBJEKT_akt!=NULL && E->objekt_n==Element->objekt_n) || (posun_elementu && OBJEKT_akt==NULL)) && E->geo.typ==0 && m.PtInLine(E->geo.X1,E->geo.Y1,E->geo.X4,E->geo.Y4,X,Y)){ret=true;break;}
			E=d.v.dalsi_krok(VYHYBKY,E);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		E=NULL;delete E;
	}
	return ret;
}
//---------------------------------------------------------------------------
//metoda pro sledování zda je nutné náhled uložit
void TForm1::nahled_ulozit (bool duvod_ulozit,bool duvod_validovat)
{
	log(__func__);//logování
	//aby mohlo být tlačítko aktivované musí k tomu vzniknout důvod (přidání robota, editace hodnot, ...), ale zároveň nesmí být chybná hodnota rychlosti (validace)
	if(duvod_ulozit && !FormX->existuje_validace()) {scGPButton_ulozit->Enabled=true;duvod_ulozit_nahled=true;DuvodUlozit(true);}
	if(!duvod_ulozit) {scGPButton_ulozit->Enabled=false;duvod_ulozit_nahled=false;}
	if(duvod_ulozit_nahled && Akce!=EDITACE_TEXTU && Akce_temp!=EDITACE_TEXTU && duvod_validovat){if(stisknute_leve_tlacitko_mysi)duvod_validovat=1;else duvod_validovat=2;}//pokud je důvod validovat, tj. až po dokončení operace=1, aby neustále neproblikávalo i během posunu elementů
}
//---------------------------------------------------------------------------
//automatické nekonfliktní pozicování tabulek podle tabulek ostatních elementů
void TForm1::aut_pozicovani(Cvektory::TElement *E, int X, int Y)
{
	log(__func__);//logování
	//deklarace
	short O=120;//hodnota odsazení  30
	double x=0,x1=0,y=0,y1=0;//původní a překlopené souřadnice tabulky
	bool hor=false,ver=false;
	X=m.L2Px(E->geo.X4);Y=m.L2Py(E->geo.Y4);
  //nastavení dvou možných pozic mGridu
	switch(E->orientace)
	{
		case 0:x=X-E->mGrid->Width/2.0;y=Y-O-E->mGrid->Height;y1=Y+O;hor=true;break;
		case 90:x=X+O;x1=X-O-E->mGrid->Width;y=Y-E->mGrid->Height/2.0;ver=true;break;
		case 180:x=X-E->mGrid->Width/2.0;y=Y+O;y1=Y-O-E->mGrid->Height;hor=true;break;
		case 270:x=X-O-E->mGrid->Width;x1=X+O;y=Y-E->mGrid->Height/2.0;ver=true;break;
	}

	//deklarace pro test
	Cvektory::TElement *el=NULL;
	Cvektory::T2Element *VYH=NULL;
	bool prekryto=false,konec=false;
	TRect E_tab,el_tab,PmG_tab,T_tab;
	int pokus=0;
	//načtení oblastí tabulek PmG a teploměrů
	if(PmG!=NULL)
	{
		PmG_tab.left=m.L2Px(OBJEKT_akt->Xp);PmG_tab.right=PmG_tab.left+PmG->Width;
		PmG_tab.top=m.L2Py(OBJEKT_akt->Yp);PmG_tab.bottom=PmG_tab.top+PmG->Height;
	}
	Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
	if(T!=NULL)
	{
		T_tab.left=m.L2Px(T->posledni->Xt);T_tab.right=T_tab.left+T->posledni->mGrid->Width;
		T_tab.top=m.L2Py(T->posledni->Yt);T_tab.bottom=T_tab.top+T->posledni->mGrid->Height;
	}

	//test překrytí ve 2 krocích, prvni průchod kontrola překryvu tabulky na akt. pozici (např. x,y), pokud dojde ke konfliktu nastane druhý průchod, kontrola překryvu tabulky na nové pozici (např. x1,y), pokud nastane kolize, vložení tabulky na souřadnice elementu
	do
	{
		//načtení oblasti vkládané tabulky
		E_tab.left=x;E_tab.right=x+E->mGrid->Width;
		E_tab.top=y;E_tab.bottom=y+E->mGrid->Height;
		//nastavení parametrů
		prekryto=false;
		pokus++;
    el=OBJEKT_akt->element;
		if(predchozi_PM!=NULL)el=predchozi_PM;
		VYH=d.v.hlavicka_seznam_VYHYBKY();

		//průchod skrze všechny elementy v objektu
		while(el!=NULL)
		{
			if(el!=E && E->eID!=MaxInt && E->eID!=100)//vynechat vkládaný element a elementy bez viditelných mGridů
			{
				//načtení oblasti tabulky elementu
				el_tab.left=m.L2Px(el->Xt);el_tab.right=el_tab.left+el->mGrid->Width;
				el_tab.top=m.L2Py(el->Yt);el_tab.bottom=el_tab.top+el->mGrid->Width;
				//kontrola zda je s ní v překryvu
				if(E_tab.Intersects(el_tab)){prekryto=true;break;}
			}
      //posun na další
			if(predchozi_PM!=NULL && el==predchozi_PM)el=OBJEKT_akt->element;
      else el=d.v.dalsi_krok(VYH,el,OBJEKT_akt);
		}

    //kontrola překrytí s PmG a teplomery
    if(!prekryto && PmG!=NULL && E_tab.Intersects(PmG_tab))prekryto=true;
		if(!prekryto && T!=NULL && E_tab.Intersects(T_tab))prekryto=true;

    //nastalo překrytí, řešení
		if(hor && prekryto)
		{
			if(pokus==1)y=y1;//v prvním kroku, posun mGridu na alternativní pozici
			else {x=X;y=Y;konec=true;}//v druhém kroku, posun mGridu na souřadnice elementu, ukončení testu
		}
		if(ver && prekryto)
		{
			if(pokus==1)x=x1;//v prvním kroku, posun mGridu na alternativní pozici
			else {x=X;y=Y;konec=true;}//v druhém kroku, posun mGridu na souřadnice elementu, ukončení testu
		}

		//uakzatelové záležitosti
		d.v.vymaz_seznam_VYHYBKY(VYH);
		el=NULL;delete el;
    //kontrola zda nebylo vše OK při prvním průchodu, pokud ano ukončení testu
		if(!prekryto)konec=true;
  }
	while(!konec && pokus<2);//ukončit test pokud byl dokončen, nebo pokud byl naplněn maximální počet pokusů

	//zapsaní nejlepších souřadnic
	E->Xt=m.P2Lx(x);
	E->Yt=m.P2Ly(y);
	//vynulování a smazání ukazatele
	E=NULL;delete E;
  T=NULL;delete T;
}
//---------------------------------------------------------------------------
//dle toho, zda je umisťovaný element nad osou či pod osou pohonu je vrácena rotace symbolu, X_bod,.. je bbod vkládání elementu (jedna souřadnice ho váže na pohon)
short TForm1::rotace_symbol(short trend,int X_bod,int Y_bod)
{
	log(__func__);//logování
	short rotace_symbolu=trend-90;
	int X_kur=akt_souradnice_kurzoru_PX.x,Y_kur=akt_souradnice_kurzoru_PX.y;
	switch(trend)
	{
		//pohon vodorovně
		case 90:	if(Y_bod>Y_kur)rotace_symbolu=180;else rotace_symbolu=0;break;
		case 270:	if(Y_bod<Y_kur)rotace_symbolu=0;else rotace_symbolu=180;break;
		//pohon svisle
		case 180: if(X_bod<X_kur)rotace_symbolu=270;else rotace_symbolu=90;break;
		case 0: if(X_bod>X_kur)rotace_symbolu=90;else rotace_symbolu=270;break;
	}
	return m.Rt90(rotace_symbolu);
}
//---------------------------------------------------------------------------
//vytovoření tabulky knihovny objektů
void TForm1::vytvoreni_tab_knihovna()
{
	log(__func__);//logování
	/////////mGrid_knihovny/////////
	mGrid_knihovna=new TmGrid(this);//vždy nutno jako první
	/////////nastavení defaultního designu
	mGrid_knihovna->Left=-500;mGrid_knihovna->Top=-500;//pouze aby se při prvním zobrazení nezobrazovala formou probliku vlevo nahoře
	mGrid_knihovna->DefaultCell.Font->Name=aFont->Name;
	mGrid_knihovna->DefaultCell.Font->Size=aFont->Size;
	mGrid_knihovna->DefaultCell.isNegativeNumber->Name=aFont->Name;
	mGrid_knihovna->DefaultCell.isNegativeNumber->Size=aFont->Size;
	mGrid_knihovna->DefaultCell.isZero->Name=aFont->Name;
	mGrid_knihovna->DefaultCell.isZero->Size=aFont->Size;
	mGrid_knihovna->DefaultCell.isLink->Name=aFont->Name;
	mGrid_knihovna->DefaultCell.isLink->Size=aFont->Size;
	mGrid_knihovna->DefaultCell.isActiveLink->Name=aFont->Name;
	mGrid_knihovna->DefaultCell.isActiveLink->Size=aFont->Size;
	mGrid_knihovna->Note.Font->Name=aFont->Name;
	mGrid_knihovna->DefaultCell.RightBorder->Color=clWhite;
	mGrid_knihovna->DefaultCell.TopBorder->Color=clWhite;
	mGrid_knihovna->DefaultCell.BottomBorder->Color=clWhite;
	mGrid_knihovna->Border.Width=1;
	mGrid_knihovna->Border.Color=(TColor)(RGB(240,240,240));
	mGrid_knihovna->scGPImageCollection=scGPImageCollection_Objekty;
	mGrid_knihovna->ID=6666;
	mGrid_knihovna->Tag=6;//ID formu
	/////////vytvoření konkrétní tabulky podle režimu kabiny
	mGrid_knihovna->Create(2,14);//vytvoření celé tabulky najednou
	mGrid_knihovna->Left=-1;
	mGrid_knihovna->Top=-1;
	mGrid_knihovna->SetColumnAutoFit(-4);
	mGrid_knihovna->Columns[0].Width=mGrid_knihovna->Columns[1].Width=84;//polovina šířky left toolbaru
	/////////nadpisy
	mGrid_knihovna->Cells[0][0].Text="Hala";
	mGrid_knihovna->Cells[0][3].Text="Nav/svěšování";
	mGrid_knihovna->Cells[0][5].Text="Technologie";
//	mGrid_knihovna->Cells[0][8].Text="Lakovna";
	mGrid_knihovna->Cells[0][11].Text="Ostatní";
	/////////úprava výšky řádků
	for(unsigned int i=0;i<=mGrid_knihovna->RowCount-1;i++)
	{
		if(i!=0 && i!=1 && i!=2 && i!=3 && i!=5 && i!=11)mGrid_knihovna->Rows[i].Height=59;
		else
		{
			mGrid_knihovna->Cells[0][i].Align=mGrid_knihovna->Talign::LEFT;
			//mGrid_knihovna->Cells[0][i].Background->Color=clRed;//clBtnFace;
			mGrid_knihovna->Rows[i].Height=32;
			mGrid_knihovna->Cells[0][i].LeftMargin=10;
			mGrid_knihovna->Cells[0][i].Font=scGPLabel_roboti->Font;
		}
	}
	mGrid_knihovna->Rows[13].Height=scSplitView_LEFTTOOLBAR->Height-(mGrid_knihovna->Top+mGrid_knihovna->Height);//přesah mimo obrazovku, nezobrazování spodní hranice tabulky
	/////////sloučení nadpisů
	mGrid_knihovna->MergeCells(0,0,1,0);
	mGrid_knihovna->MergeCells(0,3,1,3);
	mGrid_knihovna->MergeCells(0,5,1,5);
	mGrid_knihovna->MergeCells(0,11,1,11);
	/////////hala
	mGrid_knihovna->Cells[0][1].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][1].ImageIndex=15;
	if(language==EN)mGrid_knihovna->Cells[0][1].ImageIndex=27;
	mGrid_knihovna->Cells[0][2].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][2].ImageIndex=16;
	/////////objekty
	//navěšování/svěšování
	mGrid_knihovna->Cells[0][4].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][4].ImageIndex=0;
	if(language==EN)mGrid_knihovna->Cells[0][4].ImageIndex=24;
	mGrid_knihovna->Cells[0][4].Align=mGrid_knihovna->CENTER;
	mGrid_knihovna->Cells[1][4].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[1][4].ImageIndex=9;
	if(language==EN)mGrid_knihovna->Cells[1][4].ImageIndex=25;
	mGrid_knihovna->Cells[1][4].Align=mGrid_knihovna->CENTER;
	mGrid_knihovna->Rows[4].Height=36;
	//technologie
	mGrid_knihovna->Cells[0][6].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][6].ImageIndex=1;
	mGrid_knihovna->Cells[1][6].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][6].ImageIndex=4;
	mGrid_knihovna->Cells[0][7].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][7].ImageIndex=2;
	if(language==EN)mGrid_knihovna->Cells[0][7].ImageIndex=17;
	mGrid_knihovna->Cells[1][7].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][7].ImageIndex=3;
	mGrid_knihovna->Cells[0][8].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][8].ImageIndex=5;
	if(language==EN)mGrid_knihovna->Cells[0][8].ImageIndex=19;
	mGrid_knihovna->Cells[1][8].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[1][8].ImageIndex=6;
	if(language==EN)mGrid_knihovna->Cells[1][8].ImageIndex=20;
	mGrid_knihovna->Cells[0][9].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][9].ImageIndex=7;
	if(language==EN)mGrid_knihovna->Cells[0][9].ImageIndex=21;
	mGrid_knihovna->Cells[0][9].Align=mGrid_knihovna->LEFT;
	mGrid_knihovna->Rows[9].Height=37;
	mGrid_knihovna->Cells[0][10].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][10].ImageIndex=8;
	if(language==EN)mGrid_knihovna->Cells[0][10].ImageIndex=22;
	mGrid_knihovna->Cells[0][10].Align=mGrid_knihovna->LEFT;
	mGrid_knihovna->Rows[10].Height=61;
	//ostatní
	mGrid_knihovna->Cells[0][12].Type=mGrid_knihovna->IMAGE;
	if(language==CS)mGrid_knihovna->Cells[0][12].ImageIndex=12;//přejezd
	if(language==EN)mGrid_knihovna->Cells[0][12].ImageIndex=26;
	mGrid_knihovna->Cells[1][12].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][12].ImageIndex=13;//vyhybka
	mGrid_knihovna->Cells[1][12].Align=mGrid_knihovna->LEFT;
	mGrid_knihovna->Cells[1][12].Hint=ls->Strings[9]+"/ "+ls->Strings[10];//"Výhybka / Spojka"
	mGrid_knihovna->Cells[1][12].ShowHint=true;
	/////////centrování komponent
	mGrid_knihovna->Update();
	TscGPImage *I=NULL;
	int odsazeni=34;//změna odsazení
	for (unsigned int i=1;i<=mGrid_knihovna->RowCount-1; i++)
	{
		if(i==7||i==8||i==9||i==10/*||i==13||i==14*/)odsazeni-=5;
		//pro první sloupec
		if(mGrid_knihovna->Cells[0][i].Type==mGrid_knihovna->IMAGE)I=mGrid_knihovna->getImage(0,i);
		if(I!=NULL)
		{
			mGrid_knihovna->Cells[0][i].Valign=mGrid_knihovna->vNO;
			I->Top+=odsazeni;
			I=NULL;//nulování pro další sloupec
		}
		//pro druhý sloupec
		if(mGrid_knihovna->Cells[1][i].Type==mGrid_knihovna->IMAGE)I=mGrid_knihovna->getImage(1,i);
		if(I!=NULL)
		{
			mGrid_knihovna->Cells[1][i].Valign=mGrid_knihovna->vNO;
			mGrid_knihovna->Cells[1][i].Align=mGrid_knihovna->Talign::aNO;
			I->Top+=odsazeni;
			I->Left-=1;
		}
		I=NULL;//nulování pro další krok
	}
	delete I;I=NULL;

	//////nastavení popisků pro knihovnu
	popisky_knihovna_nahled(true);
}
//---------------------------------------------------------------------------
//přepíná popisky mezi knihovnou a editací
void TForm1::popisky_knihovna_nahled(bool knihovna)
{
	log(__func__);//logování
	if(knihovna)//popisky pro knihovnu
	{
		scGPLabel_roboti->Caption=ls->Strings[50];//"Hala";
		scGPLabel_otoce->Caption=ls->Strings[51];//"Nav/Svěšování";
		scGPLabel_otoce->Top=95;
		scGPLabel_stop->Caption=ls->Strings[52];//"Technologie";
		scGPLabel_stop->Top=163;
		scGPLabel_geometrie->Caption=ls->Strings[53];//"Ostatní";
		scGPLabel_geometrie->Top=450;
		scGPLabel_poznamky->Visible=false;
	}
	else
	{
		scGPLabel_otoce->Caption=ls->Strings[59];//"Otoče";
		scGPLabel_otoce->Top=224;
		scGPLabel_stop->Caption=ls->Strings[60];//"Stop stanice";
		scGPLabel_stop->Top=315;
		scGPLabel_geometrie->Caption=ls->Strings[61];//"Geometrie linky";
		scGPLabel_geometrie->Top=398;
		scGPLabel_poznamky->Visible=true;
	}
}
//---------------------------------------------------------------------------
//klik do knihovny objektů, spouštění akce přidávání objektu
void TForm1::tab_knihovna_click(double X,double Y,long Col,long Row)
{
	log(__func__);//logování
	if(Akce==NIC && MOD==LAYOUT)
	{
  	if(Col<0 || Row<0)
  	{
  		Col=mGrid_knihovna->GetIdxColumn(X,Y);
  		Row=mGrid_knihovna->GetIdxRow(X,Y);
  	}
  	if(mGrid_knihovna->Cells[Col][Row].Type==mGrid_knihovna->IMAGE)
  	{
			int id=mGrid_knihovna->Cells[Col][Row].ImageIndex;
			if(mGrid_knihovna->Cells[Col][Row].ImageIndex==27)id=15;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==24)id=0;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==25)id=9;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==18)id=4;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==17)id=2;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==19)id=5;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==20)id=6;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==21)id=7;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==22)id=8;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==23)id=11;
  		if(mGrid_knihovna->Cells[Col][Row].ImageIndex==26)id=12;
  		vybrany_objekt=id;
  		if(vybrany_objekt<15 && vybrany_objekt!=13)//klik na objekty
  		{
  			UnicodeString name=knihovna_objektu[vybrany_objekt].name;
  			switch(vybrany_objekt)
  			{
       		case 0:name=F->ls->Strings[254];break;//"navěšování"
       		case 2:name=F->ls->Strings[260];break;//"ožeh"
       		case 4:name=F->ls->Strings[261];break;//"ionizace"
       		case 5:name=F->ls->Strings[262];break;//"lakování"
  				case 6:name=F->ls->Strings[279];break;//"vytěkání"
       		case 7:name=F->ls->Strings[280];break;//"sušení"
       		case 8:name=F->ls->Strings[281];break;//"chlazení"
       		case 9:name=F->ls->Strings[255];break;//"svěšování"
       		case 11:name=F->ls->Strings[282];break;//"výtah"
  				case 12:name=F->ls->Strings[283];break;//"přejezd"
       		case 14:name=F->ls->Strings[285];break;//"nedefinovaný"
  			}
  			SB(ls->Strings[384]+" "+name);//"Kliknutím na libovolné místo umístíte objekt "
  			Akce=ADD;kurzor(add_o);
  			d.predchozi_oblast=0;
  			add_posledni=true;pom=NULL;
  			pom_element=NULL;pom_element_temp=NULL;//důležité ukazatele pro vkládání výhybky a objektů
  		}
  		if(vybrany_objekt==13 && d.v.OBJEKTY->predchozi->n<3)//pokud je vybraná vyhýbka nejsou alespoň 3 objekty
  		{
  			MB(ls->Strings[334]);//"Výhybku lze nastavit, pokud jsou k dispozici minimálně 3 technologické objekty!"
  			Akce=NIC;kurzor(standard);
  		}
  		else if(vybrany_objekt==13)
			{
  			Akce=VYH;
  			element_id=300;//při prvním kliku budu vždy vkládat výhybku
				pom_element=NULL;pom_element_temp=NULL;//důležité ukazatele pro vkládání výhybky a objektů
				//vypnutí vykreslovacích vrstev
				scGPCheckBox1_popisky->Checked=false;//vypnutí zobrazení popisků, v budoucnu rozšířit na uložení předchozího stavu
	    	scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
	    	scGPCheckBox_zobrazit_pozice->Checked=false;
	    	scGPCheckBox_zobrazit_palce->Checked=false;
				scGPCheckBox_rozmisteni_voziku->Checked=false;
				scGPCheckBox_popisek_pohonu->Checked=false;
				vytvor_statickou_scenu();
				REFRESH();
  		}
  		if(vybrany_objekt==15)//vytvoř halu
  		{
  			scGPButton_nakreslit_haluClick(this);
  		}
  		if(vybrany_objekt==16)//import hala
  		{
  			scButton_nacist_podkladClick(this);
  		}
  	}
	}
	nastav_focus();
}
//---------------------------------------------------------------------------
//vytvoří tabulku pohonu
void TForm1::vytvoreni_tab_pohon(bool existuje_poh_tabulka)
{
	log(__func__);//logování
	//pokud již nejaká tabulka existuje nevytvoří default tabulku pohonu
	if(existuje_poh_tabulka)
	{
		PmG=NULL;
	}
	//neexistuje žádná pohonová tabulka, vytvoří default tabulku
	else
	{
		FormX->vstoupeno_elm=false;
		FormX->vstoupeno_poh=false;
		PmG=new TmGrid(this);//vždy nutno jako první
  	//nastavení defaultního designu
  	PmG->Left=-500;PmG->Top=-500;//pouze aby se při prvním zobrazení nezobrazovala formou probliku vlevo nahoře
  	PmG->DefaultCell.Font->Name=aFont->Name;
  	PmG->DefaultCell.Font->Size=aFont->Size;
  	PmG->DefaultCell.isNegativeNumber->Name=aFont->Name;
  	PmG->DefaultCell.isNegativeNumber->Size=aFont->Size;
		PmG->DefaultCell.isZero->Name=aFont->Name;
		PmG->DefaultCell.isZero->Size=aFont->Size;
		PmG->DefaultCell.isZero->Color=(TColor)RGB(128,128,128);//řeší problém s barvou fontu, pokud je v buňce nulová hodnota
		PmG->DefaultCell.isLink->Name=aFont->Name;
		PmG->DefaultCell.isLink->Size=aFont->Size;
		PmG->DefaultCell.isActiveLink->Name=aFont->Name;
		PmG->DefaultCell.isActiveLink->Size=aFont->Size;
		PmG->Note.Font->Name=aFont->Name;
		PmG->Note.Font->Size=14;
		PmG->Note.Font->Color=d.clError;//musí být defaultní
		PmG->AntiAliasing_text=true;
		PmG->Border.Width=2;
		PmG->ID=9999;
		PmG->Tag=6;//ID formu
		//vytvoření konkrétní tabulky podle režimu kabiny
		PmG->Create(4,9);//vytvoření celé tabulky najednou

		//nastavení šířek
		PmG->SetColumnAutoFit(-4);
		PmG->Columns[0].Width=PmG->Columns[1].Width=PmG->Rows[0].Height;
		PmG->Columns[2].Width=90;
		PmG->Columns[3].Width=145;
    //formátování hlavičky tabulky
		PmG->Cells[1][0].Font->Color=clBlack;
		PmG->Cells[3][0].Type=PmG->COMBO;
		//nastavení popisků
		aktualizace_tab_pohon();
		//nastavení exButtonu, skrývání řádku max.WT Stop
		PmG->exBUTTONVisible=true;
		PmG->exBUTTON->ShowHint=true;
		PmG->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
		//finální desing
		PmG->Cells[0][0].Align=mGrid->RIGHT;
		PmG->Cells[0][0].Font->Color=(TColor)RGB(128,128,128);
		PmG->Cells[0][0].RightMargin=3;
		for(int i=1;i<=ms.MyToDouble(PmG->RowCount-1);i++)
		{
			if (PmG->Cells[3][i].Type==PmG->EDIT)
				PmG->Cells[3][i].InputNumbersOnly=2;
  		else
  		{
				PmG->Cells[3][i].Font->Color=(TColor)RGB(128,128,128);
				PmG->Cells[3][i].RightMargin=5;
				PmG->Cells[3][i].Background->Color=(TColor)RGB(240,240,240);//m.clIntensive((TColor)RGB(128,128,128),115);
  		}
			PmG->Cells[2][i].Align=mGrid->RIGHT;
			PmG->Cells[3][i].Align=mGrid->RIGHT;
			PmG->Cells[0][i].Font->Color=(TColor)RGB(128,128,128);
			PmG->Cells[1][i].Font->Color=(TColor)RGB(128,128,128);
			PmG->Cells[2][i].Font->Color=(TColor)RGB(128,128,128);
			PmG->Cells[2][i].RightMargin=3;
		}
		PmG->Cells[0][0].Font->Color=clBlack;
		//merge + design hranic, vyjímečný případ, musí být před skrytím řádků !!!!!!!!!!!!!
		PmG->MergeCells(0,0,2,0);
		PmG->Cells[2][0].Align=mGrid->RIGHT;PmG->Cells[2][0].RightMargin=3;//chová se divně, musí být nastaveno po merge
		PmG->Cells[0][1].RightBorder->Color=clWhite;
		PmG->MergeCells(0,1,0,2);PmG->MergeCells(1,1,1,2);//sloučení pro rychlost
		PmG->Cells[1][3].LeftBorder->Color=PmG->Cells[1][5].LeftBorder->Color=clWhite;
		PmG->MergeCells(0,3,0,5);PmG->MergeCells(1,3,1,4);//sloučení pro rozteč
		PmG->Cells[0][6].RightBorder->Color=clWhite;
		PmG->MergeCells(0,6,0,8);PmG->MergeCells(1,6,1,8);//sloučení pro mezery
		//umístění tabulky
		TRect oblast_kabiny=vrat_max_oblast(OBJEKT_akt,true);
		if(OBJEKT_akt->Xp<0 && OBJEKT_akt->Yp<0)//definice pozice při prvním otevření objektu
		{
			OBJEKT_akt->Xp=m.P2Lx(oblast_kabiny.right+10);    //+30
			OBJEKT_akt->Yp=m.P2Ly(oblast_kabiny.top-F->PmG->Rows->Height);//-30
		}
		//naplnění comba hodnotami
		PmG->Update();
		tab_pohon_COMBO();
		//obnova stavu zobrazení tabulky
		switch(OBJEKT_akt->stavPM)
		{
			//minimized, PmG nepoužívá + defaltní stav
			case 0:case 1:
			{
        PmG->VisibleRow(3,false,false);
	    	PmG->VisibleRow(4,false,false);
	    	PmG->VisibleRow(5,false,false);
	    	PmG->VisibleRow(6,false,false);
	    	PmG->VisibleRow(7,false,false);
				PmG->VisibleRow(8,false,false);
				PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
				break;
			}
			//maximized
			case 2:
			{
        PmG->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
				break;
			}
    }
		//kontrola zda není PmG mimo obraz, pokud ano zobrazit na def. místo
		//PmG->Refresh();
	}
}
//---------------------------------------------------------------------------
//změní režim tabulce pohonu
void TForm1::aktualizace_tab_pohon(bool popisky,bool data,bool komponenty)
{
	log(__func__);//logování
	////aktualizace popisků tabulky
	if(popisky && PmG!=NULL)
	{
		AnsiString aRD,R,Rz,cas;
		//nastavení jednotek podle posledních nastavení
		if (aRDunit==SEC) aRD="<a>[m/s]</a>";
		else aRD="<a>m/min</a>";
		if (Runit==M) R="<a>[m]</a>";
		else R="<a>[mm]</a>";//1
		if (Rzunit==M) Rz="<a>[m]</a>";
		else Rz="<a>[mm]</a>";
		if (PTunit==SEC) cas="<a>[s]</a>";
		else cas="<a>[min]</a>";
		//plnění buněk
		PmG->Cells[0][0].Text=ls->Strings[447];//"Výběr pohonu "
		PmG->Cells[2][1].Text=ls->Strings[451];//"Nastavená"
		PmG->Cells[2][2].Text=ls->Strings[448];//"Rozmezí"
		PmG->Cells[2][4].Text=ls->Strings[209];//"Palce"
		PmG->Cells[2][5].Text=ls->Strings[210];//"Násobek"
		PmG->Cells[2][3].Text=ls->Strings[211];//"Jigů"
		PmG->Cells[2][6].Text=ls->Strings[212];//"Podvozky"
		PmG->Cells[2][7].Text=ls->Strings[213];//"Jigy 0°"
		PmG->Cells[2][8].Text=ls->Strings[214];//"Jigy 90°"
		//nastavení sloučených sloupců
		PmG->Cells[0][1].Text=ls->Strings[452];//+" "+rychlost;//"Rychlost"
		PmG->Cells[1][1].Text=aRD;//jednotky
		PmG->Cells[0][1].Font->Orientation=900;PmG->Cells[0][1].Valign=TmGrid::MIDDLE;
		PmG->Cells[1][1].Font->Orientation=900;PmG->Cells[1][1].Valign=TmGrid::MIDDLE;
		PmG->Cells[0][3].Text=ls->Strings[453];//"Rozteč"
		PmG->Cells[1][3].Text=R;//jednotky
		PmG->Cells[0][3].Font->Orientation=900;PmG->Cells[0][3].Valign=TmGrid::MIDDLE;
		PmG->Cells[1][3].Font->Orientation=900;PmG->Cells[1][3].Valign=TmGrid::MIDDLE;
		PmG->Cells[0][6].Text=ls->Strings[215];//"Mezera"
		PmG->Cells[1][6].Text=Rz;//jednotky
		PmG->Cells[0][6].Font->Orientation=900;PmG->Cells[0][6].Valign=TmGrid::MIDDLE;
		PmG->Cells[1][6].Font->Orientation=900;PmG->Cells[1][6].Valign=TmGrid::MIDDLE;
	}

	////aktualizace dat
	if(data && PmG!=NULL)
	{
  	if(OBJEKT_akt->pohon!=NULL)
		{
			if(OBJEKT_akt->pohon->rezim!=0)//přepočet po přidání KK elementu
			{
				OBJEKT_akt->pohon->Rz=m.Rz(OBJEKT_akt->pohon->aRD);
				OBJEKT_akt->pohon->Rx=m.Rx(OBJEKT_akt->pohon->aRD,OBJEKT_akt->pohon->roztec);
			}
			PmG->Cells[3][1].Text=m.round2double(outaRD(OBJEKT_akt->pohon->aRD),3);
  		PmG->Cells[3][2].Text=AnsiString(m.round2double(outaRD(OBJEKT_akt->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(OBJEKT_akt->pohon->rychlost_do),3));
			PmG->Cells[3][4].Text=m.round2double(outR(OBJEKT_akt->pohon->roztec),3);
			PmG->Cells[3][5].Text=m.round2double(OBJEKT_akt->pohon->Rx,3);
			PmG->Cells[3][3].Text=m.round2double(outR(OBJEKT_akt->pohon->Rz),3);
			PmG->Cells[3][6].Text=m.round2double(outRz(m.mezera(0,OBJEKT_akt->pohon->Rz,0)),3);
			PmG->Cells[3][7].Text=m.round2double(outRz(m.mezera(0,OBJEKT_akt->pohon->Rz,1)),3);
			PmG->Cells[3][8].Text=m.round2double(outRz(m.mezera(90,OBJEKT_akt->pohon->Rz,1)),3);
  	}
  	else
		{
			PmG->Cells[3][1].Text="-";
  		PmG->Cells[3][2].Text="-";
			PmG->Cells[3][3].Text="-";
			PmG->Cells[3][4].Text="-";
			PmG->Cells[3][5].Text="-";
			PmG->Cells[3][6].Text="-";
			PmG->Cells[3][7].Text="-";
			PmG->Cells[3][8].Text="-";
		}
	}

	////aktualizace editovatelných polozek
	if(komponenty && PmG!=NULL)
	{
		//pokud pohon exituje a není používaný
		if(OBJEKT_akt->pohon!=NULL)// && !d.v.pohon_je_pouzivan(OBJEKT_akt->pohon->n,false))
		{
			if(OBJEKT_akt->pohon->rezim!=0)//povolit editaci všeho
			{
				PmG->Cells[3][1].Type=PmG->EDIT;if(PmG_VID==0){PmG->Cells[3][1].Background->Color=clWhite;PmG->Cells[3][1].Font->Color=(TColor)RGB(43,87,154);}
				PmG->Cells[3][5].Type=PmG->EDIT;if(PmG_VID==0){PmG->Cells[3][5].Background->Color=clWhite;PmG->Cells[3][5].Font->Color=(TColor)RGB(43,87,154);}
			}
			else//povolit editaci RD a rozteče palce
			{
				PmG->Cells[3][1].Type=PmG->EDIT;if(PmG_VID==0){PmG->Cells[3][1].Background->Color=clWhite;PmG->Cells[3][1].Font->Color=(TColor)RGB(43,87,154);}
				if(PmG->Cells[3][5].Type==PmG->EDIT){mGrid_komponenta_na_draw(PmG,3,5);if(PmG_VID==0){PmG->Cells[3][5].Background->Color=(TColor)RGB(240,240,240);PmG->Cells[3][5].Font->Color=(TColor)RGB(128,128,128);}}
			}
		}
		else//vše zakázat pokud je editace povolená
		{
			if(PmG->Cells[3][1].Type==PmG->EDIT){mGrid_komponenta_na_draw(PmG,3,1);if(PmG_VID==0){PmG->Cells[3][1].Background->Color=(TColor)RGB(240,240,240);PmG->Cells[3][1].Font->Color=(TColor)RGB(128,128,128);}}
			if(PmG->Cells[3][5].Type==PmG->EDIT){mGrid_komponenta_na_draw(PmG,3,5);if(PmG_VID==0){PmG->Cells[3][5].Background->Color=(TColor)RGB(240,240,240);PmG->Cells[3][5].Font->Color=(TColor)RGB(128,128,128);}}
		}
		//kontrola režimu objektu
		if(OBJEKT_akt->pohon!=NULL && OBJEKT_akt->pohon->rezim==0)//pokud je režim S&G, většina informací je nerelevantních
		{
			PmG->Cells[3][3].Text="-";
			PmG->Cells[3][5].Text="-";
			PmG->Cells[3][6].Text="-";
			PmG->Cells[3][7].Text="-";
			PmG->Cells[3][8].Text="-";
			PmG_VID=0;PmG_VID_value=0;
		}
	}
}
//---------------------------------------------------------------------------
//provede změnu jednotek v tabulce pohonu
void TForm1::zmena_jednotek_tab_pohon()
{
	log(__func__);//logování
	if(PmG!=NULL)
	{
   	FormX->vstoupeno_poh=false;//blokace událostí při vkládání elementu
   	FormX->vstoupeno_elm=false;
   	//překlopení jednotek
   	switch(JID)
   	{
			//rychlost
			case 6:
   		{
   			if (aRDunit==SEC) aRDunit=MIN;
   			else aRDunit=SEC;
			}break;
			//rozteče
			case 8:
   		{
   			if (Runit==M) Runit=MM;
   			else Runit=M;
			}break;
			//mezery
			case 11:
			{
				if (Rzunit==M) Rzunit=MM;
				else Rzunit=M;
			}break;
   		default:break;
		}
   	//zapsání nových jednotek do INI
   	writeINI("nastaveni_form_parametry","RDt", aRDunit);
   	writeINI("nastaveni_nahled","R", Runit);
		writeINI("nastaveni_nahled","Rz", Rzunit);
		AnsiString aRD,R,Rz,cas;
  	//nastavení jednotek podle posledních nastavení
  	if (aRDunit==SEC) aRD="<a>[m/s]</a>";
  	else aRD="<a>m/min</a>";
  	if (Runit==M) R="<a>[m]</a>";
  	else R="<a>[mm]</a>";//1
  	if (Rzunit==M) Rz="<a>[m]</a>";
  	else Rz="<a>[mm]</a>";
  	if (PTunit==SEC) cas="<a>[s]</a>";
  	else cas="<a>[min]</a>";
		//změna v tabulce, změna popisků zde !!!!
		PmG->Cells[2][0].Text=ls->Strings[447];//"Výběr pohonu "
   	PmG->Cells[2][1].Text=ls->Strings[451];//"Nastavená"
   	PmG->Cells[2][2].Text=ls->Strings[448];//"Rozmezí"
   	PmG->Cells[2][4].Text=ls->Strings[209];//"Palce"
   	PmG->Cells[2][5].Text=ls->Strings[210];//"Násobek"
   	PmG->Cells[2][3].Text=ls->Strings[211];//"Jigů"
   	PmG->Cells[2][6].Text=ls->Strings[212];//"Podvozky"
   	PmG->Cells[2][7].Text=ls->Strings[213];//"Jigy 0°"
   	PmG->Cells[2][8].Text=ls->Strings[214];//"Jigy 90°"
   	//nastavení sloučených sloupců
   	PmG->Cells[0][2].Text=ls->Strings[452];//+" "+rychlost;//"Rychlost"
   	PmG->Cells[1][2].Text=aRD;//jednotky
   	PmG->Cells[0][5].Text=ls->Strings[453];//"Rozteč"
   	PmG->Cells[1][4].Text=R;//jednotky
   	PmG->Cells[0][8].Text=ls->Strings[215];//"Mezera"
		PmG->Cells[1][8].Text=Rz;//jednotky
		aktualizace_tab_pohon(false,true,false);//aktualizace popisků a dat
    FormX->zmen_jednotky_validace();
		PmG->Refresh();
	}
}
//---------------------------------------------------------------------------
//prohledá elementy v objektu, vrátí 0 pokud se vyskytuje pouze jedna rotace, vrátí 1 pokud se vyskytují 2 rotace
int TForm1::pocet_vyskytu_elementu_s_otoci(Cvektory::TObjekt *Objekt)
{
	log(__func__);//logování
	int ret=0;int rotace=0;
	Cvektory::TElement *E=Objekt->element;//nepřeskakovat hlavičku
	rotace=Objekt->orientace;
	E=E->dalsi;
	while(E!=NULL)
	{
		if(abs(E->rotace_jig)!=rotace)//nalezena rozdílná rotace v kabině
			{ret++;break;}
		E=E->dalsi;
	}
	E=NULL; delete E;
	return ret;
}
//---------------------------------------------------------------------------
//načtení pohonů do comba
void TForm1::tab_pohon_COMBO()
{
	log(__func__);//logování
	TscGPComboBox *PCombo=PmG->getCombo(3,0);
	Cvektory::TPohon *P=d.v.POHONY->dalsi;//ukazatel na pohony, přeskakuje hlavičku, která je již vytvořena
	TscGPListBoxItem *t=NULL;

	//nastavení comba
	PCombo->Items->Clear();//smazání původního obsahu
	PCombo->Font->Color=(TColor)RGB(43,87,154);
	PCombo->Options->FontHotColor=(TColor)RGB(43,87,154);
	PCombo->Options->FontPressedColor=(TColor)RGB(43,87,154);
	PCombo->Options->FontFocusedColor=(TColor)RGB(43,87,154);
	PCombo->Options->FrameDisabledColor=(TColor)RGB(240,240,240);
	PCombo->BiDiMode=bdRightToLeft;

	//plnění comba
	if(P==NULL)//pokud neexitustuje žádný pohon
	{
		t=PCombo->Items->Add(/*tady nelze parametr*/);  //32 znaků se vleze do COMBA
		t->Caption=ls->Strings[217];//"Žádný pohon";
		//nesmí tu být, způsobuje cyklení z důvodu vytoření onchange pohonu Form_parametry->scComboBox_pohon->ItemIndex=0;//pohon nedefinován
	}
	else//pokud existuje přidá na první pozici nabídku nepřiřazen dále začne plnit existujícím pohny
	{
		//vytvoření položky nepřiřazen
		t=PCombo->Items->Add(/*tady nelze parametr*/);
		t->Caption=ls->Strings[218];//"Vyberte pohon";
		//plnění existujícím pohony
		while (P!=NULL)
		{
			t=PCombo->Items->Add(/*tady nelze parametr*/);
			t->Caption=P->name;
			P=P->dalsi;//přesun na další pohon
		}
	}
	if(OBJEKT_akt->pohon!=NULL)PCombo->ItemIndex=OBJEKT_akt->pohon->n;
	else PCombo->ItemIndex=0;

	t=NULL; delete t;
	P=NULL; delete P;
	PCombo=NULL; delete PCombo;
	nastav_focus();
}
//---------------------------------------------------------------------------
//zapne nebo vypne komponenty megridu v závislosti na tom zda má element přiřazený pohon
void TForm1::set_enabled_mGrid(Cvektory::TElement *E)
{
	log(__func__);//logování
	//nastavení stavu
//	bool stav=true;
//	if(E->pohon==NULL)stav=false;
//	if(E->pohon!=NULL && d.v.pohon_je_pouzivan(E->pohon->n,false))stav=false;
//	//přepnutí buněk
//	switch(E->eID)
//	{
//		case 0:
//		{
//      //neovlivnuje pohon
//			//if(E->mGrid->Cells[1][3].Type==E->mGrid->EDIT)E->mGrid->SetEnabledComponent(1,3,stav);//měnit pouze pokud se jedná o edit
//		}
//		break;//stop stanice
//		case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
//		{
//			E->mGrid->SetEnabledComponent(1,1,stav);
//			//E->mGrid->SetEnabledComponent(1,2,stav);
//		}
//		break;
//		case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
//		{
//			//obsahuje pouze needitovatelné položky
//			//E->mGrid->SetEnabledComponent(1,2,stav);
//			//E->mGrid->SetEnabledComponent(1,3,stav);
//		}
//		break;
//		case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otočí
//		{
//			E->mGrid->SetEnabledComponent(3,1,stav);
//			//E->mGrid->SetEnabledComponent(1,2,stav);
//			//E->mGrid->SetEnabledComponent(1,5,stav);
//			E->mGrid->SetEnabledComponent(3,8,stav);
//			//E->mGrid->SetEnabledComponent(1,7,stav);
//		}
//		break;
//		case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otočí
//		{
//			//žádná položka neovlivňuje pohon
//			//E->mGrid->SetEnabledComponent(1,5,stav);
//			//E->mGrid->SetEnabledComponent(1,6,stav);
//		}break;
//		default:break;
//	}
}
//---------------------------------------------------------------------------
//nadesignuje tabulky daného elementu
void TForm1::design_element(Cvektory::TElement *E,bool prvni_spusteni,bool plnit_comba)
{
	log(__func__);//logování
  FormX->vstoupeno_elm=false;
	E->stav=1;//pro jistotu odstraňuje uložený highlight kóty, highlight je temp zaležitost pro editaci
	//definice barev
	TColor clHeaderFont=clBlack;
	TColor clBackgroundHidden=(TColor)RGB(240,240,240);//m.clIntensive((TColor)RGB(128,128,128),115);
	TColor clFontLeft = (TColor)RGB(128,128,128);
	//TColor clFontRight = (TColor)RGB(43,87,154);
	//definice fontu a velikosti písma
	E->mGrid->DefaultCell.Font->Name=aFont->Name;
	E->mGrid->DefaultCell.Font->Size=aFont->Size;
	E->mGrid->DefaultCell.isNegativeNumber->Name=aFont->Name;
	E->mGrid->DefaultCell.isNegativeNumber->Size=aFont->Size;
	E->mGrid->DefaultCell.isZero->Name=aFont->Name;
	E->mGrid->DefaultCell.isZero->Size=aFont->Size;
	E->mGrid->DefaultCell.isZero->Color=clFontLeft;//řeší problém s barvou fontu, pokud je v buňce nulová hodnota
	E->mGrid->DefaultCell.isLink->Name=aFont->Name;
	E->mGrid->DefaultCell.isLink->Size=aFont->Size;
	E->mGrid->DefaultCell.isActiveLink->Name=aFont->Name;
	E->mGrid->DefaultCell.isActiveLink->Size=aFont->Size;
	E->mGrid->Note.Font->Name=aFont->Name;
	E->mGrid->Note.Font->Size=14;

	//definice jednotek a šířek
	AnsiString LO,cas,delka_otoce,rychlost,R,Rz;
	short sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla;//hodnoty pro základní jednotky
	//když není font roboto nutno rozšířit buňky s textem
	if(aFont->Name!="Roboto Cn"){sirka_0=10;sirka_1=10;sirka_2=10;sirka_3=10;sirka_4=10;sirka_56=25;}
	//nastavení jednotek podle posledních nastavení
	if (PTunit==0) cas="<a>[s]</a>";//0
	else cas="<a>[min]</a>";//1
	if (LOunit==0) LO="<a>[m]</a>";//0
	else LO="<a>[mm]</a>";//1
	if (DOtocunit==0) delka_otoce="<a>[m]</a>";//0
	else delka_otoce="<a>[mm]</a>";//1
	if (aRDunit==SEC) rychlost="<a>[m/s]</a>";//0
	else rychlost="<a>m/min</a>";//1
	if (Runit==M) R="<a>[m]</a>";//0
	else R="<a>[mm]</a>";//1
	if (Rzunit==M) Rz="<a>[m]</a>";//0
	else Rz="<a>[mm]</a>";//1

	//nastavení šířek
	if(PTunit==SEC && LOunit==M)sirka_1=90;
	else sirka_1=105;
	if(PTunit==0)sirka_2=95;
	else sirka_2=107;
	if(PTunit==SEC)sirka_3=45;
	else sirka_3=55;
	if(language==Tlanguage::EN)sirka_3+=15;
	sirka_cisla=90;
	if(PTunit==Tminsec::SEC)sirka_0=80;
	else sirka_0=100;
	if(language!=Tlanguage::CS)sirka_0+=5;
	sirka_4=95;
	sirka_56=96;

	//načtení aktuálních informací do elementu
	Cvektory::TCesta *C=NULL;
	if(d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n!=0)
	{
		C=d.v.vrat_segment_cesty(d.v.ZAKAZKA_akt,E);
		if(C!=NULL)E->data=C->data;
	}

	//nadesignování tabulek dle typu elementu
	E->mGrid->Left=-1000;
	E->mGrid->Top=-1000;
	if(prvni_spusteni)prvni_vytvoreni_tab_elementu(E,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla,LO,cas,delka_otoce,rychlost,R,Rz);
	else dalsi_vytvoreni_tab_elementu(E,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla,LO,cas,delka_otoce,rychlost,R,Rz);

	//vrácení přepočítaných dat do segmentu cesty
	if(C!=NULL)C->data=E->data;
	C=NULL;delete C;

	//formátování hlavičky tabulky (vždy stejné)
	int prvni_sloupec=0;
	if(E->eID==200 || E->eID==300)prvni_sloupec=3;
	E->mGrid->Border.Width=2;
	if(!(E->eID>=400 && E->eID<=402))E->mGrid->Cells[prvni_sloupec][0].Text="<a>"+E->name+"</a>";//nasazení linku
  //specifické nastavení pro tabulku teploměrů
	else
	{
		E->mGrid->Cells[0][0].Font->Color=clHeaderFont;E->mGrid->Cells[1][0].Font->Color=clHeaderFont;E->mGrid->Cells[2][0].Font->Color=clHeaderFont;
		E->mGrid->Cells[0][0].RightBorder->Width=2;E->mGrid->Cells[1][0].BottomBorder->Width=2;E->mGrid->Cells[2][0].BottomBorder->Width=2;
	}
	E->mGrid->Cells[prvni_sloupec][0].isLink->Color=clHeaderFont;
	E->mGrid->Cells[prvni_sloupec][0].isActiveLink->Color=clHeaderFont;
	E->mGrid->Cells[prvni_sloupec][0].BottomBorder->Width=2;
	//formátování buněk tabulky (vždy stejné)
	prvni_sloupec=0;
	switch(E->eID)
	{
		case 0:prvni_sloupec=1;break;
		case 3:case 9:case 13:case 17:case 103:case 107:prvni_sloupec=2;break;
		case 5:prvni_sloupec=1;break;
		case 200:case 300:prvni_sloupec=2;break;
		//case 400:case 401:case 402:prvni_sloupec=1;break;
	}
	for(unsigned int i=1;i<=E->mGrid->RowCount-1;i++)
	{
		//první sloupec
		E->mGrid->Cells[prvni_sloupec][i].RightMargin = 3;
		E->mGrid->Cells[prvni_sloupec][i].Align=mGrid->RIGHT;
		E->mGrid->Cells[prvni_sloupec][i].Font->Color=clFontLeft;
		if(prvni_sloupec>0)E->mGrid->Cells[0][i].Font->Color=clFontLeft;
		if(prvni_sloupec>1)E->mGrid->Cells[1][i].Font->Color=clFontLeft;
		//procházení dalších sloupců
		for(unsigned int j=prvni_sloupec+1;j<E->mGrid->ColCount;j++)
		{
			if(E->mGrid->Cells[j][i].Type==E->mGrid->EDIT && (E->mGrid->Cells[j][i].Text==0 || ms.MyToDouble(E->mGrid->Cells[j][i].Text)!=0))E->mGrid->Cells[j][i].InputNumbersOnly=2;
			E->mGrid->Cells[j][i].RightMargin = 3;
			E->mGrid->Cells[j][i].Align=mGrid->RIGHT;
			if(ms.MyToDouble(E->mGrid->Cells[j][i].Text)!=0)E->mGrid->Cells[j][i].Text=m.round2double(ms.MyToDouble(E->mGrid->Cells[j][i].Text),3);//přeskočení prvního řádku v tabulce stopky
			if((((E->eID==300 || E->eID==200 || E->eID==300 || E->eID==301) && i!=1) || (E->eID!=300 && E->eID!=200 && E->eID!=300 && E->eID!=301)) && E->mGrid->Cells[j][i].Type!=E->mGrid->EDIT&&E->mGrid->Cells[j][i].Type!=E->mGrid->COMBO)
			{
				E->mGrid->Cells[j][i].Background->Color=clBackgroundHidden;
				E->mGrid->Cells[j][i].RightMargin=5;
				E->mGrid->Cells[j][i].Font->Color=clFontLeft;
			}
			if((E->eID==300 || E->eID==200 || E->eID==301) && i==1)E->mGrid->Cells[j][i].Font->Color=clHeaderFont;
		}
		if(E->eID==300 || E->eID==200)E->mGrid->Cells[3][i].LeftBorder->Width=E->mGrid->Cells[3][i].RightBorder->Width=2;
	}
	//sloučení buněk hlavičky
	if(E->eID==300 || E->eID==200)
	{
		//E->mGrid->MergeCells(0,0,4,0);//hlavička tabulky
		E->mGrid->Cells[0][1].Font->Color=clBlack;
		E->mGrid->Cells[0][1].RightBorder->Width=2;
    E->mGrid->MergeCells(0,1,2,2);//sloučení pro pole pohon
	}
	else if(!(E->eID>=400 && E->eID<=402))E->mGrid->MergeCells(0,0,E->mGrid->ColCount-1,0);
	//design + sloučení pro pole pohon
	if(E->eID==301)
	{
		E->mGrid->Cells[0][1].Font->Color=clBlack;
    E->mGrid->Cells[0][1].Align=E->mGrid->CENTER;
		E->mGrid->Cells[0][1].Valign=E->mGrid->MIDDLE;
		E->mGrid->MergeCells(0,1,0,2);
  }
	//rozdělení sekcí v tabulkách
	if(E->eID==300 || E->eID==200)
	{
		for(unsigned int i=0;i<=E->mGrid->ColCount-1;i++)
		{
			E->mGrid->Cells[i][3].TopBorder->Width=2;
			E->mGrid->Cells[i][5].TopBorder->Width=2;
			E->mGrid->Cells[i][8].TopBorder->Width=2;
			E->mGrid->Cells[i][11].TopBorder->Width=2;
		}
		////temp odemknutí editace max WT
		E->mGrid->Cells[3][11].InputNumbersOnly=0;//cokoliv
		E->mGrid->Cells[4][11].InputNumbersOnly=0;
		////konec temp
	}
	////temp odemknutí editace max WT
	if(E->eID==0)E->mGrid->Cells[2][4].InputNumbersOnly=0;//cokoliv
	////konec temp

	if(plnit_comba)napln_comba_mGridu(E);
	set_enabled_mGrid(E);

	//////dočasně
	if(E->eID==1||E->eID==7||E->eID==11||E->eID==15||E->eID==101||E->eID==105)E->mGrid->Cells[1][3].InputNumbersOnly=1;
}
//---------------------------------------------------------------------------
void TForm1::napln_comba_mGridu(Cvektory::TElement *E)
{
	log(__func__);
	FormX->vstoupeno_elm=false;
	//naplnění a přiřazení COMBA rotace
	if(E->eID==3||E->eID==4||E->eID==5||E->eID==6||E->eID==9||E->eID==10||E->eID==13||E->eID==14||E->eID==17||E->eID==18||E->eID==103||E->eID==104||E->eID==107||E->eID==108)//elementy s otočí
	{
		E->mGrid->Update();//musí být přítomen před zakazováním komponent, před Update tabulka ještě neexistuje
		int pozice,sloupec=1;
		switch(E->eID)//nutnost zjistit pozici komba
		{
			case 3:case 9:case 13:case 17:case 103:case 107:sloupec=3;pozice=4;break;
			case 4:case 10:case 14:case 18:case 104:case 108:pozice=2;break;
			case 5:sloupec=2;pozice=1;break;
			case 6:pozice=1;break;
		}
		TscGPComboBox *C=E->mGrid->getCombo(sloupec,pozice);
		if(C!=NULL)
		{
			C->Clear();
			C->Font->Color=(TColor)RGB(43,87,154);
			C->Options->FontHotColor=(TColor)RGB(43,87,154);
			C->Options->FontPressedColor=(TColor)RGB(43,87,154);
			C->Options->FontFocusedColor=(TColor)RGB(43,87,154);
      C->Options->FrameDisabledColor=(TColor)RGB(240,240,240);
			C->BiDiMode=bdRightToLeft;
			TscGPListBoxItem *I;
			I=C->Items->Add();
	  	I->Caption="180-";//kvůli opačnému zarovnání musí být číslo zapsáno jako řetězec se znaménkem na konci!
			I=C->Items->Add();
			I->Caption="90-";
			I=C->Items->Add();
			I->Caption=90;
			I=C->Items->Add();
	  	I->Caption=180;
	  	I=NULL;delete I;
	  	//přiřazení COMBA
	  	if(E->rotace_jig==-180)C->ItemIndex=0;
			if(E->rotace_jig==-90)C->ItemIndex=1;
	  	if(E->rotace_jig==90)C->ItemIndex=2;
	  	if(E->rotace_jig==180)C->ItemIndex=3;
		}
		C=NULL;delete C;
	}
	//naplnění a přiřazení COMBA PD
	if(E->eID==1||E->eID==3||E->eID==7||E->eID==9||E->eID==11||E->eID==13||E->eID==15||E->eID==17||E->eID==101||E->eID==103||E->eID==105||E->eID==107)
	{
		E->mGrid->Update();//musí být přítomen před zakazováním komponent, před Update tabulka ještě neexistuje
    int sloupec;
		switch(E->eID)
		{
			case 3:case 9:case 13:case 17:case 103:case 107:sloupec=3;break;
			default:sloupec=1;break;
		}
		TscGPComboBox *C=E->mGrid->getCombo(sloupec,E->mGrid->RowCount-1);
		if(C!=NULL)
		{
	  	C->Clear();
			C->Font->Color=(TColor)RGB(43,87,154);
			C->Options->FontHotColor=(TColor)RGB(43,87,154);
			C->Options->FontPressedColor=(TColor)RGB(43,87,154);
			C->Options->FontFocusedColor=(TColor)RGB(43,87,154);
			C->Options->FrameDisabledColor=(TColor)RGB(240,240,240);
	  	C->BiDiMode=bdRightToLeft;
	  	TscGPListBoxItem *I;
	  	I=C->Items->Add();
	  	I->Caption=ls->Strings[247];//"začátek";//kvůli opačnému zarovnání musí být číslo zapsáno jako řetězec se znaménkem na konci!
	  	I=C->Items->Add();
	  	I->Caption=ls->Strings[248];//"střed";
			I=C->Items->Add();
			I->Caption=ls->Strings[249];//"celý";
	  	I=NULL;delete I;
	  	//přiřazení COMBA
			C->ItemIndex=E->data.PD;
			//if(E->mGrid->Columns[sloupec].Width<100)E->mGrid->Columns[1].Width+=15;//rozčíření skrze combo
		}
		C=NULL;delete C;
	}
	//PM + výhybka
	if(E->eID==200 || E->eID==300)
	{
		E->mGrid->Update();//musí být přítomen před zakazováním komponent, před Update tabulka ještě neexistuje
		//default barvy buňěk pod combem, pokud existuje vedlejší větev
		if(E->eID==300 && E->dalsi2!=E->predchozi2)
		{
			E->mGrid->Cells[3][2].Background->Color=clWhite;E->mGrid->Cells[4][2].Background->Color=clWhite;
			E->mGrid->Cells[3][2].Font->Color=(TColor)RGB(43,87,154);;E->mGrid->Cells[4][2].Font->Color=(TColor)RGB(43,87,154);
		}
		TscGPComboBox *C1=E->mGrid->getCombo(3,2),*C2=E->mGrid->getCombo(4,2),*C_pom=NULL;
		if(C1!=NULL && C2!=NULL)//může dojít k NULL pokud je pohonová tabulka mimo obraz
		{
			C1->Clear();C2->Clear();
			//nastavení default barev comba
			nastav_combo_mGridu(C1);nastav_combo_mGridu(C2);
			//přidávání itemů do comba
			TscGPListBoxItem *I1,*I2;
			I1=C1->Items->Add();I2=C2->Items->Add();
			if(d.v.POHONY->dalsi!=NULL){I1->Caption=ls->Strings[218];I2->Caption=ls->Strings[218];}//vyberte pohon
			else {I1->Caption=ls->Strings[217];I2->Caption=ls->Strings[217];}//"Žádný pohon";
			Cvektory::TPohon *p=d.v.POHONY->dalsi;
			while(p!=NULL)
			{
				I1=C1->Items->Add();I2=C2->Items->Add();
				I1->Caption=p->name;I2->Caption=p->name;
				p=p->dalsi;
			}
			delete p;p=NULL;
			//přiřazení itemindexu podle pohonu na vedlejší větvi, pokud je definovaná
			C1->ItemIndex=0;C2->ItemIndex=0;
			//kontrola zda budou prohozeny sloupce
			if(prohodit_sloupce_PM(E))
			{
				//prohhození comb
				C_pom=C1;
				C1=C2;
				C2=C_pom;
			}
			C_pom=NULL;delete C_pom;
			//přiřazení itemindexů
			if(E->pohon!=NULL)C1->ItemIndex=E->pohon->n;
			if(E->eID==300)//pro výhybku
			{
				//default C2
				if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)C2->ItemIndex=E->dalsi2->pohon->n;
				if(E->dalsi2==E->predchozi2)C2->Enabled=false;
			}
   		else//pro PM
			{
				if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)C2->ItemIndex=E->dalsi->pohon->n;
				if(E->dalsi==NULL && d.v.ELEMENTY->dalsi->pohon!=NULL)C2->ItemIndex=d.v.ELEMENTY->dalsi->pohon->n;
   			//zakazování comb
				if(E->objekt_n!=OBJEKT_akt->n)C1->Enabled=false;
				if((E->dalsi!=NULL && E->dalsi->objekt_n!=E->objekt_n || E->dalsi==NULL) && predchozi_PM!=E)C2->Enabled=false;
			}

			//kontrola zda můžu editovat pohon
			zmena_editovanych_bunek(E);//automaticky nastaví editované položky a needitovatelné položky pro pohonové tabulky

			//ukazatelové záležitosti
			I1=NULL;delete I1;
			I2=NULL;delete I2;
		}

		//nastavení scgpbuttonu
		TscGPGlyphButton *B=E->mGrid->getGlyphButton(4,0);
		if(B!=NULL)
		{
			B->GlyphOptions->Kind=scgpbgkUpArrow;
			if(!E->mGrid->Rows[3].Visible)
			{
				B->GlyphOptions->Kind=scgpbgkDownArrow;
				//zmenšení tabulky, úspora místa
				E->mGrid->Columns[0].Width=5;
				E->mGrid->Columns[1].Width=5;
				E->mGrid->Columns[2].Width=50;
				E->mGrid->exBUTTONVisible=false;
			}
			B->Width=25;
			B->TransparentBackground=false;
			B->GlyphOptions->NormalColor=clBlack;
			B->GlyphOptions->NormalColorAlpha=200;
			B->Options->FrameNormalColor=E->mGrid->Border.Color;//(TColor)RGB(43,87,154);
			B->Options->FrameNormalColorAlpha=255;
			B->Options->NormalColor=m.clIntensive((TColor)RGB(128,128,128),105);//(TColor)RGB(43,87,154);
			B->Options->NormalColorAlpha=255;
			B->Options->FrameWidth=E->mGrid->Border.Width;//orámování stejně široké jako orámování tabulky
			B->Options->FocusedColor=B->Options->NormalColor;
			B->Options->FocusedColorAlpha=255;
			B->Options->FrameFocusedColor=B->Options->FrameNormalColor;
			B->Options->FrameFocusedColorAlpha=255;
			B->Parent=Form1;
		}

		//nastavování comb WT
		C1=E->mGrid->getCombo(3,11);C2=E->mGrid->getCombo(4,11);
		if(C1!=NULL && C2!=NULL)
		{
			//kontrola zda je zvoleno správně a pokud nedošlo ke změně pohonů
			Cvektory::TElement *e_pom=E->dalsi;
			if(E->eID==200 && E->dalsi==NULL)e_pom=d.v.ELEMENTY->dalsi;
			if(E->eID==300)e_pom=E->dalsi2;
			if(E->WT_index==1 && e_pom!=NULL && E->pohon!=e_pom->pohon){E->WT_index=2;vypocet_WT(E);}
			e_pom=NULL;delete e_pom;
			//prohození, je-li potřeba
			if(prohodit_sloupce_PM(E))
			{
				//prohhození comb
				C_pom=C1;
				C1=C2;
				C2=C_pom;
			}
			C1->Clear();C2->Clear();
			//nastavení default barev comba
			nastav_combo_mGridu(C1);nastav_combo_mGridu(C2);
			//přidání itemů
			TscGPListBoxItem *I1=NULL,*I2=NULL;
			I1=C1->Items->Add();I2=C2->Items->Add();
			I1->Caption=ls->Strings[497];I2->Caption=ls->Strings[497];//"žádné"
			I1=C1->Items->Add();I2=C2->Items->Add();
			I1->Caption=String(m.round2double(outPT(E->WT),3));I2->Caption=String(m.round2double(outPT(E->WT),3));
			I1=NULL;delete I1;
			I2=NULL;delete I2;
			//vybrání položky
			C1->ItemIndex=0;
			C2->ItemIndex=0;
			//kontrola zda můžu editovat pohon
			zmena_editovanych_bunek(E);//automaticky nastaví editované položky a needitovatelné položky pro pohonové tabulky
		}

		//ukazatelové záležitosti
		B=NULL;delete B;
		C1=NULL;delete C1;
		C2=NULL;delete C2;
		C_pom=NULL;delete C_pom;
	}
	//spojka
	if(E->eID==301)
	{
		E->mGrid->Update();//musí být přítomen před zakazováním komponent, před Update tabulka ještě neexistuje
		TscGPComboBox *C1=E->mGrid->getCombo(1,3),*C2=E->mGrid->getCombo(2,3);
		if(C1!=NULL && C2!=NULL)
		{
      //kontrola zda je zvoleno správně a pokud nedošlo ke změně pohonů
			Cvektory::TElement *e_pom=E->dalsi;
			if(E->eID==200 && E->dalsi==NULL)e_pom=d.v.ELEMENTY->dalsi;
			if(E->eID==300)e_pom=E->dalsi2;
			if(E->WT_index==1 && e_pom!=NULL && E->pohon!=e_pom->pohon){E->WT_index=2;vypocet_WT(E);}
			e_pom=NULL;delete e_pom;
			C1->Clear();C2->Clear();
			//nastavení default barev comba
			nastav_combo_mGridu(C1);nastav_combo_mGridu(C2);
			//přidání itemů
			TscGPListBoxItem *I1=NULL,*I2=NULL;
			I1=C1->Items->Add();I2=C2->Items->Add();
			I1->Caption=ls->Strings[497];I2->Caption=ls->Strings[497];//"žádné"
			I1=C1->Items->Add();I2=C2->Items->Add();
			I1->Caption=String(m.round2double(outPT(E->WT),3));I2->Caption=String(m.round2double(outPT(E->WT),3));
			I1=NULL;delete I1;
			I2=NULL;delete I2;
			//vybrání položky
			C1->ItemIndex=0;
			C2->ItemIndex=0;
			//kontrola zda můžu editovat pohon
			zmena_editovanych_bunek(E);//automaticky nastaví editované položky a needitovatelné položky pro pohonové tabulky
		}
		C1=NULL;C2=NULL;
		delete C1;delete C2;
	}
}
//---------------------------------------------------------------------------
//nastaví barvy a chování pro combo
void TForm1::nastav_combo_mGridu(TscGPComboBox *C)
{
	log(__func__);
	C->Font->Color=(TColor)RGB(43,87,154);
	C->Options->FontNormalColor=(TColor)RGB(43,87,154);
	C->Options->FontHotColor=(TColor)RGB(43,87,154);
	C->Options->FontPressedColor=(TColor)RGB(43,87,154);
	C->Options->FontFocusedColor=(TColor)RGB(43,87,154);
	C->Options->FrameDisabledColor=(TColor)RGB(240,240,240);
	C->BiDiMode=bdRightToLeft;
	C->Enabled=true;
}
//---------------------------------------------------------------------------
//rozhodne zda mají být prohozeny sloupce PM, podle trendu geometrie
bool TForm1::prohodit_sloupce_PM(Cvektory::TElement *E)
{
  log(__func__);
	bool ret=false;
	if(E->eID==200 || E->eID==300)
	{
		short orientace=m.Rt90(E->geo.orientace-E->geo.rotacni_uhel);
		if(orientace==270)ret=true;
	}
	return ret;
}
//---------------------------------------------------------------------------
//automaticky nastaví editované položky a needitovatelné položky pro pohonové tabulky
void TForm1::zmena_editovanych_bunek(Cvektory::TElement *E)
{
	log(__func__);//logování
	FormX->vstoupeno_elm=false;
	if(E->eID==200 || E->eID==300)//pouze pro pohonové tabulky
	{
		TscGPComboBox *C1=E->mGrid->getCombo(3,2),*C2=E->mGrid->getCombo(4,2),*C_pom=NULL;
		int prvni=3,druhy=4,rezim=-1;
		//kontrola zda budou prohozeny sloupce
		if(prohodit_sloupce_PM(E))
		{
			//prohhození comb
			C_pom=C1;
			C1=C2;
			C2=C_pom;
			prvni=4;druhy=3;
		}
		C_pom=NULL;delete C_pom;
		//kontrola zda jsou buňky na draw
		if(C1!=NULL && !C1->Enabled)
		{
			if(E->mGrid->Cells[prvni][3].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,prvni,3);if(E->VID==0){E->mGrid->Cells[prvni][3].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[prvni][3].Font->Color=(TColor)RGB(128,128,128);}}
			if(E->mGrid->Cells[prvni][7].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,prvni,7);if(E->VID==0){E->mGrid->Cells[prvni][7].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[prvni][7].Font->Color=(TColor)RGB(128,128,128);}}
		}
		if(C2!=NULL && !C2->Enabled)
		{
			if(E->mGrid->Cells[druhy][3].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,druhy,3);if(E->VID==0){E->mGrid->Cells[druhy][3].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[druhy][3].Font->Color=(TColor)RGB(128,128,128);}}
			if(E->mGrid->Cells[druhy][7].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,druhy,7);if(E->VID==0){E->mGrid->Cells[druhy][7].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[druhy][7].Font->Color=(TColor)RGB(128,128,128);}}
		}

		////první sloupec
		if(E->pohon!=NULL)rezim=E->pohon->rezim;
		if(C1!=NULL && C1->Enabled)
		{
	  	//pokud pohon exituje a není používaný
			if(E->pohon!=NULL)
			{
				if(rezim!=0)//povolit editaci všeho
				{
					E->mGrid->Cells[prvni][3].Type=E->mGrid->EDIT;if(E->VID==0){E->mGrid->Cells[prvni][3].Background->Color=clWhite;E->mGrid->Cells[prvni][3].Font->Color=(TColor)RGB(43,87,154);}
					E->mGrid->Cells[prvni][7].Type=E->mGrid->EDIT;if(E->VID==0){E->mGrid->Cells[prvni][7].Background->Color=clWhite;E->mGrid->Cells[prvni][7].Font->Color=(TColor)RGB(43,87,154);}
	  		}
	  		else//povolit editaci RD a rozteče palce
				{
					E->mGrid->Cells[prvni][3].Type=E->mGrid->EDIT;if(E->VID==0){E->mGrid->Cells[prvni][3].Background->Color=clWhite;E->mGrid->Cells[prvni][3].Font->Color=(TColor)RGB(43,87,154);}
					if(E->mGrid->Cells[prvni][7].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,prvni,7);if(E->VID==0){E->mGrid->Cells[prvni][7].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[prvni][7].Font->Color=(TColor)RGB(128,128,128);}}
				}
	  	}
	  	else//vše zakázat pokud je editace povolená
			{
				if(E->mGrid->Cells[prvni][3].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,prvni,3);if(E->VID==0){E->mGrid->Cells[prvni][3].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[prvni][3].Font->Color=(TColor)RGB(128,128,128);}}
				if(E->mGrid->Cells[prvni][7].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,prvni,7);if(E->VID==0){E->mGrid->Cells[prvni][7].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[prvni][7].Font->Color=(TColor)RGB(128,128,128);}}
			}
		}
		//nerelevantní data, skrýt
		if(rezim==0)
		{
			E->mGrid->Cells[prvni][5].Text="-";
			E->mGrid->Cells[prvni][7].Text="-";
			E->mGrid->Cells[prvni][8].Text="-";
			E->mGrid->Cells[prvni][9].Text="-";
			E->mGrid->Cells[prvni][10].Text="-";
		}

		////druhý sloupec
		Cvektory::TElement *e_pom=E->dalsi2;
		if(E->eID==200 && E->dalsi!=NULL)e_pom=E->dalsi;
		if(E->eID==200 && E->dalsi==NULL)e_pom=d.v.ELEMENTY->dalsi;
		rezim=-1;
		if(e_pom!=NULL && e_pom->pohon!=NULL)rezim=e_pom->pohon->rezim;
		if(C2!=NULL && C2->Enabled)
		{
			//pokud pohon exituje a není používaný
			if(e_pom->pohon!=NULL)
			{
				if(rezim!=0)//povolit editaci všeho
				{
					E->mGrid->Cells[druhy][3].Type=E->mGrid->EDIT;if(E->VID==0){E->mGrid->Cells[druhy][3].Background->Color=clWhite;E->mGrid->Cells[druhy][3].Font->Color=(TColor)RGB(43,87,154);}
					E->mGrid->Cells[druhy][7].Type=E->mGrid->EDIT;if(E->VID==0){E->mGrid->Cells[druhy][7].Background->Color=clWhite;E->mGrid->Cells[druhy][7].Font->Color=(TColor)RGB(43,87,154);}
				}
				else//povolit editaci RD a rozteče palce
				{
					E->mGrid->Cells[druhy][3].Type=E->mGrid->EDIT;if(E->VID==0){E->mGrid->Cells[druhy][3].Background->Color=clWhite;E->mGrid->Cells[druhy][3].Font->Color=(TColor)RGB(43,87,154);}
					if(E->mGrid->Cells[druhy][7].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,druhy,7);if(E->VID==0){E->mGrid->Cells[druhy][7].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[druhy][7].Font->Color=(TColor)RGB(128,128,128);}}
				}
			}
			else//vše zakázat pokud je editace povolená
			{
				if(E->mGrid->Cells[druhy][3].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,druhy,3);if(E->VID==0){E->mGrid->Cells[druhy][3].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[druhy][3].Font->Color=(TColor)RGB(128,128,128);}}
				if(E->mGrid->Cells[druhy][7].Type==E->mGrid->EDIT){mGrid_komponenta_na_draw(E->mGrid,druhy,7);if(E->VID==0){E->mGrid->Cells[druhy][7].Background->Color=(TColor)RGB(240,240,240);E->mGrid->Cells[druhy][7].Font->Color=(TColor)RGB(128,128,128);}}
			}
		}
		//nerelevantní data, skrýt
		if(rezim==0)
		{
			E->mGrid->Cells[druhy][5].Text="-";
			E->mGrid->Cells[druhy][7].Text="-";
			E->mGrid->Cells[druhy][8].Text="-";
			E->mGrid->Cells[druhy][9].Text="-";
			E->mGrid->Cells[druhy][10].Text="-";
		}

		//nastavení WT comb
		e_pom=E->dalsi;if(e_pom==NULL)e_pom=d.v.ELEMENTY->dalsi;
		if(E->eID==300)
		{
			e_pom=E->dalsi2;
			if(E->predchozi2==E->dalsi2)e_pom=NULL;
		}
		C1=NULL;C2=NULL;
		C1=E->mGrid->getCombo(prvni,11);C2=E->mGrid->getCombo(druhy,11);
		if(E->eID==300){E->mGrid->Cells[prvni][1].Text="IN/OUT";E->mGrid->Cells[druhy][1].Text="OUT";}//default nastavení nadpisů
		if(C1!=NULL && C2!=NULL)
		{
			//itemindex
			if(E->WT_index==1)C1->ItemIndex=1;
			if(E->WT_index==2)C2->ItemIndex=1;
			//zakázání / povolení
      C1->Enabled=true;C2->Enabled=true;//defaultní nastavení
			if(E->WT_index==0 && ((e_pom!=NULL && E->pohon!=e_pom->pohon) || e_pom==NULL)){C1->Enabled=false;C2->Enabled=false;}
			if(E->WT_index==1)C2->Enabled=false;
			if(E->WT_index==2)C1->Enabled=false;
			if(E->WT_index==1 && e_pom!=NULL && E->pohon!=e_pom->pohon){C1->Enabled=false;C2->Enabled=false;}
			if(E->WT_index==2 && e_pom!=NULL && E->pohon!=e_pom->pohon){C1->Enabled=false;C2->Enabled=false;}
			//nastavení barev
			if(C1->Enabled){E->mGrid->Cells[prvni][11].Background->Color=clWhite;E->mGrid->Cells[prvni][11].Font->Color=(TColor)RGB(43,87,154);}
			if(C2->Enabled){E->mGrid->Cells[druhy][11].Background->Color=clWhite;E->mGrid->Cells[druhy][11].Font->Color=(TColor)RGB(43,87,154);}
			//nastavení nadpisů sloupců
			if(C1->ItemIndex!=0 && E->eID==300 && e_pom!=NULL && e_pom->pohon==E->pohon && E->pohon!=NULL){E->mGrid->Cells[prvni][1].Text="OUT";E->mGrid->Cells[druhy][1].Text="IN/OUT";}
			if(C2->ItemIndex!=0 && E->eID==300 && e_pom!=NULL && e_pom->pohon==E->pohon && E->pohon!=NULL){E->mGrid->Cells[prvni][1].Text="IN/OUT";E->mGrid->Cells[druhy][1].Text="OUT";}
		}

		//ukazatelové záležitosti
		e_pom=NULL;delete e_pom;

		////ukazatelové záležitosti
		C1=NULL;C2=NULL;
		delete C1;delete C2;
	}
	//spojka
	if(E->eID==301)
	{       
		//aktualizace textu pohonů
		String p1=ls->Strings[217],p2=ls->Strings[217];//"Žádný pohon";
		if(E->pohon!=NULL)p1=E->pohon->name;if(E->predchozi2->pohon!=NULL && E->dalsi2!=E->predchozi2)p2=E->predchozi2->pohon->name;
		E->mGrid->Cells[1][1].Text="IN/OUT";E->mGrid->Cells[2][1].Text="IN";//default nastaveno
		E->mGrid->Cells[1][2].Text=p1;E->mGrid->Cells[2][2].Text=p2;
		//povolení / zákaz comb
		Cvektory::TElement *e_pom=E->predchozi2;
		if(E->predchozi2==E->dalsi2)e_pom=NULL;
		TscGPComboBox *C1=E->mGrid->getCombo(1,3),*C2=E->mGrid->getCombo(2,3);
		if(C1!=NULL && C2!=NULL)
		{
			//itemindex
			if(E->WT_index==1)C1->ItemIndex=1;
			if(E->WT_index==2)C2->ItemIndex=1;
			//zakázání / povolení
			C1->Enabled=true;C2->Enabled=true;//defaultní nastavení
			if(E->WT_index==0 && ((e_pom!=NULL && E->pohon!=e_pom->pohon) || e_pom==NULL)){C1->Enabled=false;C2->Enabled=false;}
			if(E->WT_index==1)C2->Enabled=false;
			if(E->WT_index==2)C1->Enabled=false;
      if(E->WT_index==1 && e_pom!=NULL && E->pohon!=e_pom->pohon){C1->Enabled=false;C2->Enabled=false;}
			if(E->WT_index==2 && e_pom!=NULL && E->pohon!=e_pom->pohon){C1->Enabled=false;C2->Enabled=false;}
			//nastavení barev
			if(C1->Enabled){E->mGrid->Cells[1][3].Background->Color=clWhite;E->mGrid->Cells[2][3].Font->Color=(TColor)RGB(43,87,154);}
			if(C2->Enabled){E->mGrid->Cells[2][3].Background->Color=clWhite;E->mGrid->Cells[2][3].Font->Color=(TColor)RGB(43,87,154);}
			//nastavení nadpisů sloupců
			if(C1->ItemIndex!=0 && e_pom!=NULL && e_pom->pohon==E->pohon && E->pohon!=NULL){E->mGrid->Cells[1][1].Text="IN";E->mGrid->Cells[2][1].Text="IN/OUT";}
			if(C2->ItemIndex!=0 && e_pom!=NULL && e_pom->pohon==E->pohon && E->pohon!=NULL){E->mGrid->Cells[1][1].Text="IN/OUT";E->mGrid->Cells[2][1].Text="IN";}
		}
		C1=NULL;C2=NULL;e_pom=NULL;
		delete C1;delete C2;delete e_pom;
  }
}
//---------------------------------------------------------------------------
//vytvoření tabulek, první výpočty a zapsání do spojáku
void TForm1::prvni_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce,AnsiString rychlost,AnsiString R,AnsiString Rz)
{
	log(__func__);//logování
	double aRD=0,roztec=0;//proměnné pro ukládání parametrů pohonu
	switch(E->eID)
	{
		case 0://stop stanice
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(3,7);
			//nastavování režimů podle ID objektu
			unsigned int id=d.v.vrat_objekt(E->objekt_n)->id;
			E->mGrid->Cells[2][3].Type=E->mGrid->EDIT;E->mGrid->Cells[2][6].Type=E->mGrid->EDIT;
			UnicodeString hint=ls->Strings[420];//čekání,doba čekání v bufferu
			if(id>=6 && id<=10){hint=ls->Strings[417]+" "+OBJEKT_akt->name.LowerCase()+" "+ls->Strings[418];}//čas "název objektu",maximální čas "název objektu" včetně max. WT palec
			//definice buněk
			E->mGrid->Cells[0][1].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[2][1].Text=E->sparovany->name;else E->mGrid->Cells[2][1].Text="N/A";
			E->mGrid->Cells[0][2].Text="RT "+cas;
			E->mGrid->Cells[0][3].Text=ls->Strings[419]+" "+cas;
			E->data.WTstop=m.V2WT(E->data.pocet_voziku,d.v.PP.TT);
			E->mGrid->Cells[2][3].Text=outPT(E->data.WTstop);
			E->mGrid->Cells[0][4].Text=ls->Strings[224]+" "+cas;//"WT palec "
			////temp odemknutí editace max WT
			if(E->WT==0 && E->pohon!=NULL)E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
      ////konec temp
			E->mGrid->Cells[2][4].Text=outPT(m.round2double(E->WT,3));
			E->mGrid->Cells[0][5].Font->Orientation=900;E->mGrid->Cells[0][5].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][5].Text=ls->Strings[464];
			E->mGrid->Cells[1][5].Text=ls->Strings[225];//"počet pozic";
			E->data.pocet_pozic=max_voziku(E);
			E->mGrid->Cells[2][5].Text=E->data.pocet_pozic;
			E->mGrid->Cells[1][6].Text=ls->Strings[226];//"počet vozíků";
			E->mGrid->Cells[2][6].Text=E->data.pocet_voziku;
			d.v.reserve_time(E);//výpočet a vypsání
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[1].Width=sirka_0;
			E->mGrid->Columns[2].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=hint;
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[1][5].Hint=ls->Strings[230];//"maximální možný počet pozic";
			E->mGrid->Cells[1][5].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//merge
			E->mGrid->MergeCells(0,1,1,1);
			E->mGrid->MergeCells(0,2,1,2);
			E->mGrid->MergeCells(0,3,1,3);
			E->mGrid->MergeCells(0,4,1,4);
			E->mGrid->MergeCells(0,5,0,6);
			////temp odemknutí editace max WT
			E->mGrid->Cells[2][4].Type=E->mGrid->EDIT;//cokoliv
			////konec temp
			//skrytí řádků
			if(!(id>=6 && id<=10 || id==12)){E->mGrid->VisibleRow(4,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
			break;
		}
		case 7:case 11:case 15:case 101:case 105:
		case 1://robot (kontinuální)
		{
			//načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL)aRD=E->pohon->aRD;
			//načtení popisků
			AnsiString t=ls->Strings[232]+" ",t_hint=ls->Strings[233],t_hint2=ls->Strings[238];//"LO ","délka lakovácího okna","vyosení lakovácího okna"
			if(E->eID!=1)
			{
				t=ls->Strings[234]+" ";//"PO ";
				t_hint=ls->Strings[235];//"délka pracovní oblasti";
				t_hint2=ls->Strings[482];//"vyosení pracovní oblasti";
			}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
			//definice buněk
			E->mGrid->Cells[0][1].Text="CT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(m.CT(E->data.LO1,aRD)),3);//temp do změny binárky
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;
			E->data.PT1=m.CT(E->data.LO1,aRD);
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->data.RT=0;
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[0][4].Text=t+LO;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outLO(E->data.LO1);
			E->mGrid->Cells[0][5].Text=ls->Strings[236]+" "+LO;//"vyosení "
			E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;E->mGrid->Cells[1][5].Text=outLO(E->data.LO_pozice);
			E->mGrid->Cells[1][5].isNegativeNumber->Color=(TColor)RGB(43,87,154);//musí být nastaveno ručně, vymazání podmíněného formátování
			E->mGrid->Cells[0][6].Text="PD jig";
			E->mGrid->Cells[1][6].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_1;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint=ls->Strings[498];//"maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)"
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,..."
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint=t_hint;
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint=t_hint2;
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint="part detect";
			E->mGrid->Cells[0][6].ShowHint=true;
			break;
		}
		case 8:case 12:case 16:case 102:case 106:
		case 2://robot se stop stanicí
		{
			//načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL){aRD=E->pohon->aRD;roztec=E->pohon->roztec;}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,5);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->data.PT1);
			E->mGrid->Cells[0][3].Text="max WT "+cas;
			E->mGrid->Cells[1][3].Text=outPT(m.cekani_na_palec(0,roztec,aRD,3));
			E->WT=inPT(ms.MyToDouble(E->mGrid->Cells[1][3].Text));
			E->mGrid->Cells[0][4].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][4].Text=E->sparovany->name;else E->mGrid->Cells[1][4].Text="N/A";
			E->mGrid->Cells[0][2].Text="RT "+cas;
			d.v.reserve_time(E);//výpočet a vypsání
			//pokud je záporné RT, změn PT
			if(E->data.RT<0)
			{
				E->data.PT1=E->data.PT1+E->data.RT;
				if(E->data.PT1<0)E->data.PT1=0;
				E->mGrid->Cells[1][1].Text=outPT(E->data.PT1);
				//d.v.reserve_time(E);//opětovná validace
			}
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_2;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][3].ShowHint=true;
			////temp odemknutí editace max WT
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;//cokoliv
			////konec temp
			break;
		}
		case 9:case 13:case 17:case 103:case 107:
		case 3://robot s pasivní otočí
		{
			//načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL)aRD=E->pohon->aRD;
			TPointD zony_otaceni=m.zona_otaceni(d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
			//načtení popisků
			AnsiString t=ls->Strings[232],t_hint=ls->Strings[233];//"LO","délka lakovácího okna"
			if(E->eID!=3)
			{
				t=ls->Strings[234];//"PO";
				t_hint=ls->Strings[235];//"délka pracovní oblasti";
			}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(4,14);
			//definice buněk
			E->mGrid->Cells[0][1].Text="CT1 "+cas;
			E->mGrid->Cells[3][1].Type=E->mGrid->EDIT;E->mGrid->Cells[3][1].Text=m.round2double(outPT(m.CT(E->data.LO1,aRD)),3);//temp do změny binárky
			E->mGrid->Cells[0][2].Text="PT1 "+cas;
			E->data.PT1=m.CT(E->data.LO1,aRD);
			E->mGrid->Cells[3][2].Type=E->mGrid->EDIT;E->mGrid->Cells[3][2].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[0][3].Text=t+"1 "+LO;
			E->mGrid->Cells[3][3].Type=E->mGrid->EDIT;E->mGrid->Cells[3][3].Text=m.round2double(outLO(E->data.LO1),3);
			E->mGrid->Cells[0][3].BottomBorder->Width=2;
			E->mGrid->Cells[3][3].BottomBorder->Width=2;
			E->mGrid->Cells[0][4].Font->Orientation=900;E->mGrid->Cells[0][4].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][4].Text=ls->Strings[240];//"Otoč"
			E->mGrid->Cells[1][4].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[3][4].Type=E->mGrid->COMBO;
			E->mGrid->Cells[1][5].Text=ls->Strings[241]+" "+cas;//"Otáčení"
			E->mGrid->Cells[3][5].Text=outPT(m.PTo(E->OTOC_delka,aRD));
			E->PTotoc=inPT(ms.MyToDouble(E->mGrid->Cells[3][5].Text));
			E->mGrid->Cells[1][6].Text=ls->Strings[246]+" "+delka_otoce;//"délka "
			E->mGrid->Cells[3][6].Type=E->mGrid->EDIT;E->mGrid->Cells[3][6].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[1][7].Text=ls->Strings[463];E->mGrid->Cells[1][7].BottomBorder->Color=clWhite;
			E->mGrid->Cells[1][8].Text=delka_otoce;
			E->mGrid->Cells[2][7].Text=ls->Strings[275];//"Zóna před "
			E->mGrid->Cells[3][7].Text=outDO(m.round2double(zony_otaceni.x,3));
			E->zona_pred=zony_otaceni.x;
			E->mGrid->Cells[2][8].Text=ls->Strings[276];//"Zóna za "
			E->mGrid->Cells[3][8].Text=outDO(m.round2double(zony_otaceni.x,3));
			E->zona_za=zony_otaceni.y;
			E->mGrid->Cells[0][8].BottomBorder->Width=2;
			E->mGrid->Cells[3][8].BottomBorder->Width=2;
			E->mGrid->Cells[0][9].Text="CT2 "+cas;
			E->mGrid->Cells[3][9].Type=E->mGrid->EDIT;E->mGrid->Cells[3][9].Text=m.round2double(outPT(m.CT(E->data.LO2,aRD)),3);//temp do změny binárky
			E->mGrid->Cells[0][10].Text="PT2 "+cas;
			E->data.PT2=m.CT(E->data.LO2,aRD);
			E->mGrid->Cells[3][10].Type=E->mGrid->EDIT;E->mGrid->Cells[3][10].Text=m.round2double(outPT(E->data.PT2),3);
			E->mGrid->Cells[0][11].Text=t+"2 "+LO;
			E->mGrid->Cells[3][11].Type=E->mGrid->EDIT;E->mGrid->Cells[3][11].Text=m.round2double(outLO(E->data.LO2),3);
			E->mGrid->Cells[0][11].BottomBorder->Width=2;
			E->mGrid->Cells[3][11].BottomBorder->Width=2;
			//vypočet RT
			E->data.RT=0;
			E->mGrid->Cells[0][12].Text="RT "+cas;
			E->mGrid->Cells[3][12].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[0][13].Text="PD jig";
			E->mGrid->Cells[3][13].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[1].Width=2*E->mGrid->Rows[0].Height;
			E->mGrid->Columns[2].Width=sirka_3;
			E->mGrid->Columns[3].Width=sirka_cisla;
			//nastavení hintů
      E->mGrid->Cells[0][1].Hint=ls->Strings[498];//"maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)"
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=t_hint;
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[1][5].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[1][5].ShowHint=true;
			E->mGrid->Cells[1][6].Hint=ls->Strings[243];//"délka otoče";
			E->mGrid->Cells[1][6].ShowHint=true;
			E->mGrid->Cells[2][7].Hint=ls->Strings[277];//"Zóna otáčení před středem";
			E->mGrid->Cells[2][7].ShowHint=true;
			E->mGrid->Cells[2][8].Hint=ls->Strings[278];//"Zóna otáčení za středem";
			E->mGrid->Cells[2][8].ShowHint=true;
			E->mGrid->Cells[0][9].Hint=ls->Strings[498];//"maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)"
			E->mGrid->Cells[0][9].ShowHint=true;
			E->mGrid->Cells[0][10].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][10].ShowHint=true;
			E->mGrid->Cells[0][11].Hint=t_hint;
			E->mGrid->Cells[0][11].ShowHint=true;
			E->mGrid->Cells[0][12].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][12].ShowHint=true;
			E->mGrid->Cells[0][13].Hint="part detect";
			E->mGrid->Cells[0][13].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky"
			//merge
			E->mGrid->MergeCells(0,1,2,1);
			E->mGrid->MergeCells(0,2,2,2);
			E->mGrid->MergeCells(0,3,2,3);
			E->mGrid->MergeCells(1,4,2,4);
			E->mGrid->MergeCells(1,5,2,5);
			E->mGrid->MergeCells(1,6,2,6);
			E->mGrid->MergeCells(0,9,2,9);
			E->mGrid->MergeCells(0,10,2,10);
			E->mGrid->MergeCells(0,11,2,11);
			E->mGrid->MergeCells(0,12,2,12);
      E->mGrid->MergeCells(0,13,2,13);
			E->mGrid->MergeCells(0,4,0,8);
			E->mGrid->Cells[0][8].BottomBorder->Width=2;
			break;
		}
		case 10:case 14:case 18:case 104:case 108:
		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
      //načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL){aRD=E->pohon->aRD;roztec=E->pohon->roztec;}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,8);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->data.PT1);
			E->mGrid->Cells[0][2].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[1][2].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][3].Text=ls->Strings[245]+" "+cas;//"PTo "
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][4].Text="PT2 "+cas;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(E->data.PT2);
			E->mGrid->Cells[0][6].Text="max WT "+cas;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outPT(m.cekani_na_palec(0,roztec,aRD,3));
			E->WT=inPT(ms.MyToDouble(E->mGrid->Cells[1][6].Text));
			E->mGrid->Cells[0][7].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][7].Text=E->sparovany->name;else E->mGrid->Cells[1][7].Text="N/A";
			E->mGrid->Cells[0][5].Text="RT "+cas;
			d.v.reserve_time(E);//výpočet a vypsání
      //pokud je záporné RT, změn PT
			if(E->data.RT<0)
			{
				E->data.PT1=E->data.PT2=(E->data.PT1+E->data.PT2+E->data.RT)/2.0;
				if(E->data.PT1<0)E->data.PT1=E->data.PT2=0;
				E->mGrid->Cells[1][1].Text=outPT(E->data.PT1);
				E->mGrid->Cells[1][4].Text=outPT(E->data.PT2);
				//d.v.reserve_time(E);//opětovná validace
			}
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_4;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][6].ShowHint=true;
			break;
		}
		case 5://otoč pasivní
		{
			//nastavení šířek
			if(language==Tlanguage::CS)
			{
				if(DOtocunit==Tm_mm::MM)sirka_56-=10;
				else sirka_56-=25;
			}
			else
			{
				if(DOtocunit==Tm_mm::MM)sirka_56+=5;
				else sirka_56-=10;
			}
			//načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL)aRD=E->pohon->aRD;
			TPointD zony_otaceni=m.zona_otaceni(d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E),E->rotace_jig,E->OTOC_delka);
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(3,6);
			//definice buněk
			E->mGrid->Cells[0][1].Text=ls->Strings[239]+" [°]";//"rotace [°]"
			E->mGrid->Cells[2][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text=ls->Strings[246]+" "+delka_otoce;//"délka "
			E->mGrid->Cells[2][2].Type=E->mGrid->EDIT;E->mGrid->Cells[2][2].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[0][3].Text="PT "+cas;//PT u pasivní nelze zadat
			E->PTotoc=m.PTo(E->OTOC_delka,aRD);E->mGrid->Cells[2][3].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][4].Font->Orientation=900;E->mGrid->Cells[0][4].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][4].Text=ls->Strings[463];//"Zóna"
			E->mGrid->Cells[1][4].Text=ls->Strings[275]+" "+delka_otoce;//"před "
			E->mGrid->Cells[2][4].Text=outDO(m.round2double(zony_otaceni.x,3));
			E->zona_pred=zony_otaceni.x;
			E->mGrid->Cells[1][5].Text=ls->Strings[276]+" "+delka_otoce;//"za "
			E->mGrid->Cells[2][5].Text=outDO(m.round2double(zony_otaceni.y,3));
			E->zona_za=zony_otaceni.y;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[1].Width=sirka_56;//Delší text
			E->mGrid->Columns[2].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint=ls->Strings[243];//"délka otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[1][4].Hint=ls->Strings[277];//"Zóna otáčení před středem";
			E->mGrid->Cells[1][4].ShowHint=true;
			E->mGrid->Cells[1][5].Hint=ls->Strings[278];//"Zóna otáčení za středem";
			E->mGrid->Cells[1][5].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//marge
			E->mGrid->MergeCells(0,1,1,1);
			E->mGrid->MergeCells(0,2,1,2);
			E->mGrid->MergeCells(0,3,1,3);
			E->mGrid->MergeCells(0,4,0,5);
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
      //načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL){aRD=E->pohon->aRD;roztec=E->pohon->roztec;}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,6);
			//definice buněk
			E->mGrid->Cells[0][1].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[0][4].Text="max WT "+cas;
			if(E->pohon!=NULL)E->WT=m.cekani_na_palec(0,roztec,aRD,3);else E->WT=0;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(m.round2double(E->WT,3));
			E->mGrid->Cells[0][5].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][5].Text=E->sparovany->name;else E->mGrid->Cells[1][5].Text="N/A";
			d.v.reserve_time(E);//výpočet a vypsání
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_2;//56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 200://PM
		case 300://výhybky
		{
      //určení zda budou prohozeny sloupce
			int prvni=3,druhy=4;
			if(prohodit_sloupce_PM(E)){prvni=4;druhy=3;}
			//vytvoření mgridu
			E->mGrid->Create(5,12);
			//nastavení popisků
			if(E->eID==300)E->mGrid->Cells[prvni][1].Text="IN/OUT";
			else E->mGrid->Cells[prvni][1].Text="IN";
			E->mGrid->Cells[druhy][1].Text="OUT";
			E->mGrid->Cells[4][0].Align=TmGrid::RIGHT;
			E->mGrid->Cells[4][0].AutoSizeComponent=3;//nastavý pouze výšku
			E->mGrid->Cells[4][0].Type=TmGrid::glyphBUTTON;
			E->mGrid->Cells[0][1].Text=ls->Strings[447];//"Pohon "
			E->mGrid->Cells[2][3].Text=ls->Strings[451];//"Nastavená"
			E->mGrid->Cells[2][4].Text=ls->Strings[448];//"Rozmezí"
			E->mGrid->Cells[2][6].Text=ls->Strings[209];//"Palce"
			E->mGrid->Cells[2][7].Text=ls->Strings[210];//"Násobek"
			E->mGrid->Cells[2][5].Text=ls->Strings[211];//"Jigů"
			E->mGrid->Cells[2][8].Text=ls->Strings[212];//"Podvozky"
			E->mGrid->Cells[2][9].Text=ls->Strings[213];//"Jigy 0°"
			E->mGrid->Cells[2][10].Text=ls->Strings[214];//"Jigy 90°"
			E->mGrid->Cells[0][11].Text="max "+ls->Strings[224]+" "+cas;
			//nastavení sloučených sloupců
			E->mGrid->Cells[0][3].Text=ls->Strings[452];//+" "+rychlost;//"Rychlost"
			E->mGrid->Cells[1][3].Text=rychlost;//jednotky
			E->mGrid->Cells[0][3].Font->Orientation=900;E->mGrid->Cells[0][3].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[1][3].Font->Orientation=900;E->mGrid->Cells[1][3].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][5].Text=ls->Strings[453];//"Rozteč"
			E->mGrid->Cells[1][5].Text=R;//jednotky
			E->mGrid->Cells[0][5].Font->Orientation=900;E->mGrid->Cells[0][5].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[1][5].Font->Orientation=900;E->mGrid->Cells[1][5].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][8].Text=ls->Strings[215];//"Mezera"
			E->mGrid->Cells[1][8].Text=Rz;//jednotky
			E->mGrid->Cells[0][8].Font->Orientation=900;E->mGrid->Cells[0][8].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[1][8].Font->Orientation=900;E->mGrid->Cells[1][8].Valign=TmGrid::MIDDLE;
			//nastavování hodnot druhého sloupce
			E->mGrid->Cells[3][2].Type=E->mGrid->COMBO;
			if(E->pohon!=NULL)
			{
				E->mGrid->Cells[prvni][3].Type=E->mGrid->EDIT;E->mGrid->Cells[prvni][3].Text=m.round2double(outaRD(E->pohon->aRD),3);
				E->mGrid->Cells[prvni][4].Text=AnsiString(m.round2double(outaRD(E->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(E->pohon->rychlost_do),3));
				/*E->mGrid->Cells[prvni][6].Type=E->mGrid->EDIT;*/E->mGrid->Cells[prvni][6].Text=m.round2double(outR(E->pohon->roztec),3);
				E->mGrid->Cells[prvni][7].Type=E->mGrid->EDIT;E->mGrid->Cells[prvni][7].Text=m.round2double(E->pohon->Rx,3);
				E->mGrid->Cells[prvni][5].Text=m.round2double(outR(E->pohon->Rz),3);
				E->mGrid->Cells[prvni][8].Text=m.round2double(outRz(m.mezera(0,E->pohon->Rz,0)),3);
				E->mGrid->Cells[prvni][9].Text=m.round2double(outRz(m.mezera(0,E->pohon->Rz,1)),3);
				E->mGrid->Cells[prvni][10].Text=m.round2double(outRz(m.mezera(90,E->pohon->Rz,1)),3);
			}
			else
			{
				E->mGrid->Cells[prvni][3].Text="-";
				E->mGrid->Cells[prvni][4].Text="-";
				E->mGrid->Cells[prvni][5].Text="-";
				E->mGrid->Cells[prvni][6].Text="-";
				E->mGrid->Cells[prvni][7].Text="-";
				E->mGrid->Cells[prvni][8].Text="-";
				E->mGrid->Cells[prvni][9].Text="-";
				E->mGrid->Cells[prvni][10].Text="-";
			}
			//E->mGrid->Cells[prvni][11].Text="-";
			//nastavování hodnot třetího sloupce
			E->mGrid->Cells[4][2].Type=E->mGrid->COMBO;
			if(E->eID==300)//pro výhybku
			{
				if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)
				{
					E->mGrid->Cells[druhy][3].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][3].Text=m.round2double(outaRD(E->dalsi2->pohon->aRD),3);
					E->mGrid->Cells[druhy][4].Text=AnsiString(m.round2double(outaRD(E->dalsi2->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(E->dalsi2->pohon->rychlost_do),3));
					/*E->mGrid->Cells[druhy][6].Type=E->mGrid->EDIT;*/E->mGrid->Cells[druhy][6].Text=m.round2double(outR(E->dalsi2->pohon->roztec),3);
					E->mGrid->Cells[druhy][7].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][7].Text=m.round2double(E->dalsi2->pohon->Rx,3);
					E->mGrid->Cells[druhy][5].Text=m.round2double(outR(E->dalsi2->pohon->Rz),3);
					E->mGrid->Cells[druhy][8].Text=m.round2double(outRz(m.mezera(0,E->dalsi2->pohon->Rz,0)),3);
					E->mGrid->Cells[druhy][9].Text=m.round2double(outRz(m.mezera(0,E->dalsi2->pohon->Rz,1)),3);
					E->mGrid->Cells[druhy][10].Text=m.round2double(outRz(m.mezera(90,E->dalsi2->pohon->Rz,1)),3);
				}
				else
				{
					E->mGrid->Cells[druhy][3].Text="-";
					E->mGrid->Cells[druhy][4].Text="-";
					E->mGrid->Cells[druhy][5].Text="-";
					E->mGrid->Cells[druhy][6].Text="-";
					E->mGrid->Cells[druhy][7].Text="-";
					E->mGrid->Cells[druhy][8].Text="-";
					E->mGrid->Cells[druhy][9].Text="-";
					E->mGrid->Cells[druhy][10].Text="-";
				}
			}
			else//pro PM
			{
				Cvektory::TElement *e_pom=E->dalsi;
				if(e_pom==NULL)e_pom=d.v.ELEMENTY->dalsi;//PM na konci linky
				if(e_pom->pohon!=NULL)
				{
					E->mGrid->Cells[druhy][3].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][3].Text=m.round2double(outaRD(e_pom->pohon->aRD),3);
					E->mGrid->Cells[druhy][4].Text=AnsiString(m.round2double(outaRD(e_pom->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(e_pom->pohon->rychlost_do),3));
					/*E->mGrid->Cells[druhy][6].Type=E->mGrid->EDIT;*/E->mGrid->Cells[druhy][6].Text=m.round2double(outR(e_pom->pohon->roztec),3);
					E->mGrid->Cells[druhy][7].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][7].Text=m.round2double(e_pom->pohon->Rx,3);
					E->mGrid->Cells[druhy][5].Text=m.round2double(outR(e_pom->pohon->Rz),3);
					E->mGrid->Cells[druhy][8].Text=m.round2double(outRz(m.mezera(0,e_pom->pohon->Rz,0)),3);
					E->mGrid->Cells[druhy][9].Text=m.round2double(outRz(m.mezera(0,e_pom->pohon->Rz,1)),3);
					E->mGrid->Cells[druhy][10].Text=m.round2double(outRz(m.mezera(90,e_pom->pohon->Rz,1)),3);
				}
				else
				{
					E->mGrid->Cells[druhy][3].Text="-";
					E->mGrid->Cells[druhy][4].Text="-";
					E->mGrid->Cells[druhy][5].Text="-";
					E->mGrid->Cells[druhy][6].Text="-";
					E->mGrid->Cells[druhy][7].Text="-";
					E->mGrid->Cells[druhy][8].Text="-";
					E->mGrid->Cells[druhy][9].Text="-";
					E->mGrid->Cells[druhy][10].Text="-";
				}
				e_pom=NULL;delete e_pom;
			}
			////temp odemknutí editace max WT
			E->mGrid->Cells[prvni][11].Type=E->mGrid->COMBO;
			E->mGrid->Cells[druhy][11].Type=E->mGrid->COMBO;
			////konec temp
//			if(E->WT=0)E->mGrid->Cells[druhy][11].Text=outPT(E->WT);
//			else E->mGrid->Cells[druhy][11].Text="-";
			//nastavení šířek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Columns[1].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[2].Width=105;//90;
			E->mGrid->Columns[3].Width=145;
			E->mGrid->Columns[4].Width=145;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//merge + design hranic, vyjímečný případ, musí být před skrytím řádků !!!!!!!!!!!!!
			E->mGrid->Cells[0][0].RightBorder->Color=clWhite;E->mGrid->Cells[1][0].RightBorder->Color=clWhite;E->mGrid->Cells[2][0].RightBorder->Color=clWhite;E->mGrid->Cells[3][0].RightBorder->Color=clWhite;
			E->mGrid->Cells[0][0].BottomBorder->Width=2;E->mGrid->Cells[1][0].BottomBorder->Width=2;E->mGrid->Cells[2][0].BottomBorder->Width=2;E->mGrid->Cells[3][0].BottomBorder->Width=2;E->mGrid->Cells[4][0].BottomBorder->Width=2;
			E->mGrid->Cells[0][3].RightBorder->Color=clWhite;
	  	E->mGrid->MergeCells(0,3,0,4);E->mGrid->MergeCells(1,3,1,4);//sloučení pro rychlost
	  	E->mGrid->Cells[1][5].LeftBorder->Color=E->mGrid->Cells[1][7].LeftBorder->Color=clWhite;
	  	E->mGrid->MergeCells(0,5,0,7);E->mGrid->MergeCells(1,5,1,6);//sloučení pro rozteč
	  	E->mGrid->Cells[0][8].RightBorder->Color=clWhite;
	  	E->mGrid->MergeCells(0,8,0,10);E->mGrid->MergeCells(1,8,1,10);//sloučení pro mezery
	  	E->mGrid->Cells[0][11].RightMargin = 3;
	  	E->mGrid->Cells[0][11].Align=mGrid->RIGHT;
			E->mGrid->Cells[0][11].Font->Color=(TColor)RGB(128,128,128);
			E->mGrid->MergeCells(0,11,2,11);
			//skrytí řádků
			switch(OBJEKT_akt->stavPM)
			{
				//defaultní stav zobrazeny comba a rychlosti
				case 0:
				{
        	E->mGrid->VisibleRow(5,false,false);
		     	E->mGrid->VisibleRow(6,false,false);
		     	E->mGrid->VisibleRow(7,false,false);
		     	E->mGrid->VisibleRow(8,false,false);
		     	E->mGrid->VisibleRow(9,false,false);
		     	E->mGrid->VisibleRow(10,false,false);
					E->mGrid->VisibleRow(11,false,false);
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					break;
				}
				//minimized, skrytí všeho kromě hlavičky a comb
				case 1:
				{
					E->mGrid->VisibleRow(3,false,false);
					E->mGrid->VisibleRow(4,false,false);
					E->mGrid->VisibleRow(5,false,false);
		     	E->mGrid->VisibleRow(6,false,false);
		     	E->mGrid->VisibleRow(7,false,false);
		     	E->mGrid->VisibleRow(8,false,false);
		     	E->mGrid->VisibleRow(9,false,false);
		     	E->mGrid->VisibleRow(10,false,false);
					E->mGrid->VisibleRow(11,false,false);
					E->mGrid->exBUTTONVisible=false;
					break;
				}
				//věe zobrazeno
				case 2:
				{
          E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					break;
        }
      }
			//hinty
			E->mGrid->Cells[2][11].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[2][11].ShowHint=true;
		}break;
		case 301://spojka
		{
			String p1=ls->Strings[217],p2=ls->Strings[217];//"Žádný pohon";
			if(E->predchozi2->pohon!=NULL && E->dalsi2!=E->predchozi2)p2=E->predchozi2->pohon->name;
			E->mGrid->Create(3,4);
			E->mGrid->Cells[0][1].Text=ls->Strings[447];//"Pohon "
			E->mGrid->Cells[1][1].Text="IN/OUT";E->mGrid->Cells[2][1].Text="IN";
			E->mGrid->Cells[1][2].Text=p1;E->mGrid->Cells[2][2].Text=p2;
			E->mGrid->Cells[1][3].Type=E->mGrid->COMBO;E->mGrid->Cells[2][3].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][3].Text=ls->Strings[224]+" "+cas;//"WT palec "
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=120;
			E->mGrid->Columns[1].Width=sirka_cisla+20;
			E->mGrid->Columns[2].Width=sirka_cisla+20;
			//oddělení sekcí
			E->mGrid->Cells[0][1].RightBorder->Width=2;E->mGrid->Cells[0][3].RightBorder->Width=2;
			E->mGrid->Cells[1][1].RightBorder->Width=2;E->mGrid->Cells[1][2].RightBorder->Width=2;E->mGrid->Cells[1][3].RightBorder->Width=2;
			E->mGrid->Cells[0][3].TopBorder->Width=2;E->mGrid->Cells[1][3].TopBorder->Width=2;E->mGrid->Cells[2][3].TopBorder->Width=2;
			//hinty
			E->mGrid->Cells[0][3].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 400:case 401:case 402://teplomer
		{
			E->mGrid->Create(3,radku_teplomeru);
			E->mGrid->Cells[0][0].Text=ls->Strings[484];
			E->mGrid->Cells[1][0].Text=ls->Strings[485]+" "+cas;
			E->mGrid->Cells[2][0].Text="WT "+cas;
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=75;
			E->mGrid->Columns[1].Width=sirka_cisla;
			E->mGrid->Columns[2].Width=sirka_cisla;
      //nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky"
			break;
		}
		default://pro elementy, které nemají mGrid
		{
			E->mGrid->Create(2,1);
			E->mGrid->VisibleRow(0,false,false);
			break;
		}
	}
}
//---------------------------------------------------------------------------
//další spuštěńí, pouze načítání hodnot ze spojáku
void TForm1::dalsi_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce,AnsiString rychlost,AnsiString R,AnsiString Rz)
{
	log(__func__);//logování
	double puv_RT=E->data.RT;//RT, obsahující validaci
	//extrakce pouze RT
	if(E->data.RT>1000000)E->data.RT-=1000000;
	if(E->data.RT<-1000000)E->data.RT+=1000000;
	switch(E->eID)
	{
		case 0://stop stanice
		{
			unsigned int stav=0;
			if(E->mGrid->RowCount!=0 && E->mGrid->Rows[4].Visible)stav=1;//uchování stavu zda byla tabulka "rozbalená"
			if(E->mGrid->RowCount!=0 && !E->mGrid->Rows[4].Visible)stav=2;//uchování stavu zda byla tabulka "zabalená"
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(3,7);
			//nastavování režimů podle ID objektu
			unsigned int id=d.v.vrat_objekt(E->objekt_n)->id;
			E->mGrid->Cells[2][3].Type=E->mGrid->EDIT;E->mGrid->Cells[2][6].Type=E->mGrid->EDIT;
			UnicodeString hint=ls->Strings[420];//čekání,doba čekání v bufferu
			if(id>=6 && id<=10){hint=ls->Strings[417]+" "+OBJEKT_akt->name.LowerCase()+" "+ls->Strings[418];}//čas "název objektu",maximální čas "název objektu" včetně max. WT palec
			//definice buněk
			E->mGrid->Cells[0][1].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[2][1].Text=AnsiString(E->sparovany->name);else E->mGrid->Cells[2][1].Text="N/A";
			E->mGrid->Cells[0][2].Text="RT "+cas;
			if(E->data.RT>0 && E->data.pocet_voziku>1){E->mGrid->Cells[2][2].Text="OK";E->mGrid->Cells[2][2].Hint=outPT(E->data.RT);E->mGrid->Cells[2][2].ShowHint=true;}
			else {E->mGrid->Cells[2][2].Text=outPT(E->data.RT);E->mGrid->Cells[2][2].Hint="";E->mGrid->Cells[2][2].ShowHint=false;}
			E->mGrid->Cells[0][3].Text=ls->Strings[419]+" "+cas;
			E->mGrid->Cells[2][3].Text=outPT(E->data.WTstop);
			E->mGrid->Cells[0][4].Text=ls->Strings[224]+" "+cas;//"WT palec "
			E->mGrid->Cells[2][4].Text=outPT(E->WT);
			E->mGrid->Cells[0][5].Font->Orientation=900;E->mGrid->Cells[0][5].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][5].Text=ls->Strings[464];//"Pozice"
			E->mGrid->Cells[1][5].Text=ls->Strings[225];//"počet pozic";
			/*if(id>=6 && id<=10 || id==12)*/E->data.pocet_pozic=max_voziku(E);
			E->mGrid->Cells[2][5].Text=E->data.pocet_pozic;
			E->mGrid->Cells[1][6].Text=ls->Strings[226];//"počet vozíků";
			E->mGrid->Cells[2][6].Text=E->data.pocet_voziku;
			//nastavení šířek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[1].Width=sirka_0;
			E->mGrid->Columns[2].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=hint;
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[1][5].Hint=ls->Strings[230];//"maximální možný počet pozic";
			E->mGrid->Cells[1][5].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//merge
			E->mGrid->MergeCells(0,1,1,1);
			E->mGrid->MergeCells(0,2,1,2);
			E->mGrid->MergeCells(0,3,1,3);
			E->mGrid->MergeCells(0,4,1,4);
			E->mGrid->MergeCells(0,5,0,6);
      ////temp odemknutí editace max WT
			E->mGrid->Cells[2][4].Type=E->mGrid->EDIT;//cokoliv
			////konec temp
			//skrytí / zobrazení řádků
			switch(stav)
			{
				//znovuotevření náhledu
				case 0:if(!(id>=6 && id<=10 || id==12)){E->mGrid->VisibleRow(4,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
				break;
				//tabulka byl před posunem rozbalena
				case 1:break;
				//tabulka byla před posunem zabalena
				case 2:{E->mGrid->VisibleRow(4,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
				break;
			}
			break;
		}
		case 7:case 11:case 15:case 101:case 105:
		case 1://robot (kontinuální)
		{
      double aRD=0;if(E->pohon!=NULL)aRD=E->pohon->aRD;
			//načtení popisků
			AnsiString t=ls->Strings[232]+" ",t_hint=ls->Strings[233],t_hint2=ls->Strings[238];//"LO ","délka lakovácího okna","vyosení lakovácího okna"
			if(E->eID!=1)
			{
				t=ls->Strings[234]+" ";//"PO ";
				t_hint=ls->Strings[235];//"délka pracovní oblasti";
				t_hint2=ls->Strings[482];//"vyosení pracovní oblasti";
			}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
			//definice buněk
			E->mGrid->Cells[0][1].Text="CT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(m.CT(E->data.LO1,aRD)),3);//temp do změny binárky
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;
			E->mGrid->Cells[1][2].Text=outPT(E->data.PT1);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[0][4].Text=t+LO;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outLO(E->data.LO1);
			E->mGrid->Cells[0][5].Text=ls->Strings[236]+" "+LO;//"vyosení "
			E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;E->mGrid->Cells[1][5].Text=outLO(E->data.LO_pozice);
			E->mGrid->Cells[1][5].isNegativeNumber->Color=(TColor)RGB(43,87,154);//musí být nastaveno ručně, vymazání podmíněného formátování
			E->mGrid->Cells[0][6].Text="PD jig";
			E->mGrid->Cells[1][6].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_1;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
      E->mGrid->Cells[0][1].Hint=ls->Strings[498];//"maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)"
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,..."
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint=t_hint;
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint=t_hint2;
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint="part detect";
			E->mGrid->Cells[0][6].ShowHint=true;
			break;
		}
		case 8:case 12:case 16:case 102:case 106:
		case 2://robot se stop stanicí
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,5);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->data.PT1);
			E->mGrid->Cells[0][2].Text="RT "+cas;E->mGrid->Cells[1][2].Text=outPT(E->data.RT);
			E->mGrid->Cells[0][3].Text="max WT "+cas;E->mGrid->Cells[1][3].Text=outPT(E->WT);
			E->mGrid->Cells[0][4].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][4].Text=E->sparovany->name;else E->mGrid->Cells[1][4].Text="N/A";
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_2;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][3].ShowHint=true;
      ////temp odemknutí editace max WT
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;//cokoliv
			////konec temp
			break;
		}
		case 9:case 13:case 17:case 103:case 107:
		case 3://robot s pasivní otočí
		{
      double aRD=0;if(E->pohon!=NULL)aRD=E->pohon->aRD;
			bool rozbalena=true;
			if(E->mGrid->RowCount!=0 && !E->mGrid->Rows[7].Visible)rozbalena=false;//uchování stavu zda byla tabulka "rozbalená"
			//načtení popisků
			AnsiString t=ls->Strings[232],t_hint=ls->Strings[233];//"LO","délka lakovácího okna"
			if(E->eID!=3)
			{
				t=ls->Strings[234];//"PO";
				t_hint=ls->Strings[235];//"délka pracovní oblasti";
			}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(4,14);
			//definice buněk
			E->mGrid->Cells[0][1].Text="CT1 "+cas;
			E->mGrid->Cells[3][1].Type=E->mGrid->EDIT;
			E->mGrid->Cells[0][2].Text="PT1 "+cas;
			E->mGrid->Cells[3][2].Type=E->mGrid->EDIT;
			E->mGrid->Cells[0][3].Text=t+"1 "+LO;
			E->mGrid->Cells[3][3].Type=E->mGrid->EDIT;
			E->mGrid->Cells[0][3].BottomBorder->Width=2;
			E->mGrid->Cells[3][3].BottomBorder->Width=2;
			E->mGrid->Cells[0][4].Font->Orientation=900;E->mGrid->Cells[0][4].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][4].Text=ls->Strings[240];//"Otoč"
			E->mGrid->Cells[1][4].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[3][4].Type=E->mGrid->COMBO;
			E->mGrid->Cells[1][5].Text=ls->Strings[241]+" "+cas;//"Otáčení"
			E->mGrid->Cells[1][6].Text=ls->Strings[246]+" "+delka_otoce;//"délka "
			E->mGrid->Cells[3][6].Type=E->mGrid->EDIT;
			E->mGrid->Cells[1][7].Text=ls->Strings[463];E->mGrid->Cells[1][7].BottomBorder->Color=clWhite;
			E->mGrid->Cells[1][8].Text=delka_otoce;
			E->mGrid->Cells[2][7].Text=ls->Strings[275];//"Zóna před "
			E->mGrid->Cells[2][8].Text=ls->Strings[276];//"Zóna za "
			E->mGrid->Cells[0][8].BottomBorder->Width=2;
			E->mGrid->Cells[3][8].BottomBorder->Width=2;
			E->mGrid->Cells[0][9].Text="CT2 "+cas;
			E->mGrid->Cells[3][9].Type=E->mGrid->EDIT;
			E->mGrid->Cells[0][10].Text="PT2 "+cas;
			E->mGrid->Cells[3][10].Type=E->mGrid->EDIT;
			E->mGrid->Cells[0][11].Text=t+"2 "+LO;
			E->mGrid->Cells[3][11].Type=E->mGrid->EDIT;
			E->mGrid->Cells[0][11].BottomBorder->Width=2;
			E->mGrid->Cells[3][11].BottomBorder->Width=2;
			E->mGrid->Cells[0][12].Text="RT "+cas;
			E->mGrid->Cells[0][13].Text="PD jig";
			E->mGrid->Cells[3][13].Type=E->mGrid->COMBO;
			E->mGrid->Cells[3][1].Text=m.round2double(outPT(m.CT(E->data.LO1,aRD)),3);//temp do změny binárky
			E->mGrid->Cells[3][2].Text=outPT(E->data.PT1);
			E->mGrid->Cells[3][3].Text=outLO(E->data.LO1);
			E->mGrid->Cells[3][5].Text=outPT(E->PTotoc);
			E->mGrid->Cells[3][6].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[3][7].Text=outDO(m.round2double(E->zona_pred,3));
			E->mGrid->Cells[3][8].Text=outDO(m.round2double(E->zona_za,3));
			E->mGrid->Cells[3][9].Text=m.round2double(outPT(m.CT(E->data.LO2,aRD)),3);//temp do změny binárky
			E->mGrid->Cells[3][10].Text=outPT(E->data.PT2);
			E->mGrid->Cells[3][11].Text=outLO(E->data.LO2);
      E->mGrid->Cells[3][12].Text=outLO(E->data.RT);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[1].Width=2*E->mGrid->Rows[0].Height;
			E->mGrid->Columns[2].Width=sirka_3;
			E->mGrid->Columns[3].Width=sirka_cisla;
			//nastavení hintů
      E->mGrid->Cells[0][1].Hint=ls->Strings[498];//"maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)"
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=t_hint;
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[1][5].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[1][5].ShowHint=true;
			E->mGrid->Cells[1][6].Hint=ls->Strings[243];//"délka otoče";
			E->mGrid->Cells[1][6].ShowHint=true;
			E->mGrid->Cells[2][7].Hint=ls->Strings[277];//"Zóna otáčení před středem";
			E->mGrid->Cells[2][7].ShowHint=true;
			E->mGrid->Cells[2][8].Hint=ls->Strings[278];//"Zóna otáčení za středem";
			E->mGrid->Cells[2][8].ShowHint=true;
			E->mGrid->Cells[0][9].Hint=ls->Strings[498];//"maximální časový potenciál (čas přejezdu přes pracovní/lakovací oblast)"
			E->mGrid->Cells[0][9].ShowHint=true;
			E->mGrid->Cells[0][10].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][10].ShowHint=true;
			E->mGrid->Cells[0][11].Hint=t_hint;
			E->mGrid->Cells[0][11].ShowHint=true;
			E->mGrid->Cells[0][12].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][12].ShowHint=true;
			E->mGrid->Cells[0][13].Hint="part detect";
			E->mGrid->Cells[0][13].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//merge
			E->mGrid->MergeCells(0,1,2,1);
			E->mGrid->MergeCells(0,2,2,2);
			E->mGrid->MergeCells(0,3,2,3);
			E->mGrid->MergeCells(1,4,2,4);
			E->mGrid->MergeCells(1,5,2,5);
			E->mGrid->MergeCells(1,6,2,6);
			E->mGrid->MergeCells(0,9,2,9);
			E->mGrid->MergeCells(0,10,2,10);
			E->mGrid->MergeCells(0,11,2,11);
			E->mGrid->MergeCells(0,12,2,12);
      E->mGrid->MergeCells(0,13,2,13);
			E->mGrid->MergeCells(0,4,0,8);
			if(!rozbalena)//tabulka byla zbalená
			{
				E->mGrid->MergeCells(0,4,0,6);
				E->mGrid->VisibleRow(7,false,false);
				E->mGrid->VisibleRow(8,false,false);
				E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			}
			else E->mGrid->MergeCells(0,4,0,8);
			E->mGrid->Cells[0][8].BottomBorder->Width=2;
			E->mGrid->Cells[0][8].BottomBorder->Color=E->mGrid->Cells[0][9].BottomBorder->Color;//musí být nastaveno
			break;
		}
		case 10:case 14:case 18:case 104:case 108:
		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,8);
//			E->mGrid->DefaultCell.isLink->Color=clFontRight;//přiřazení barvy fontu
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->data.PT1);
			E->mGrid->Cells[0][2].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[1][2].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][3].Text=ls->Strings[245]+" "+cas;//"PTo "
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][4].Text="PT2 "+cas;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(E->data.PT2);
			E->mGrid->Cells[0][5].Text="RT "+cas;
			E->mGrid->Cells[1][5].Text=outPT(E->data.RT);
			E->mGrid->Cells[0][6].Text="max WT "+cas;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outPT(E->WT);
			E->mGrid->Cells[0][7].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][7].Text=E->sparovany->name;else E->mGrid->Cells[1][7].Text="N/A";
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_4;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint=ls->Strings[237];//"celkový čas procesu, který je složen z dílčích časů";//přesun robota, aretace, lakování, číštění pistole,...
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][6].ShowHint=true;
			break;
		}
		case 5://otoč pasivní
		{
			bool rozbalena=true;
			if(E->mGrid->RowCount!=0 && !E->mGrid->Rows[4].Visible)rozbalena=false;//uchování stavu zda byla tabulka "rozbalená"
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(3,6);
			//definice buněk
			E->mGrid->Cells[0][1].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[2][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text=ls->Strings[246]+" "+delka_otoce;//"délka "
			E->mGrid->Cells[2][2].Type=E->mGrid->EDIT;E->mGrid->Cells[2][2].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[0][3].Text="PT "+cas;//PT u pasivní nelze zadat
			E->mGrid->Cells[2][3].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][4].Font->Orientation=900;E->mGrid->Cells[0][4].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][4].Text=ls->Strings[463];//"Zóna"
			E->mGrid->Cells[1][4].Text=ls->Strings[275]+" "+delka_otoce;//"před "
			E->mGrid->Cells[2][4].Text=outDO(m.round2double(E->zona_pred,3));
			E->mGrid->Cells[1][5].Text=ls->Strings[276]+" "+delka_otoce;//"za "
			E->mGrid->Cells[2][5].Text=outDO(m.round2double(E->zona_za,3));
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[1].Width=sirka_56+5;//Delší text
			E->mGrid->Columns[2].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint=ls->Strings[243];//"délka otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[1][4].Hint=ls->Strings[277];//"Zóna otáčení před středem";
			E->mGrid->Cells[1][4].ShowHint=true;
			E->mGrid->Cells[1][5].Hint=ls->Strings[278];//"Zóna otáčení za středem";
			E->mGrid->Cells[1][5].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//marge
			E->mGrid->MergeCells(0,1,1,1);
			E->mGrid->MergeCells(0,2,1,2);
			E->mGrid->MergeCells(0,3,1,3);
			E->mGrid->MergeCells(0,4,0,5);
			if(!rozbalena)//tabulka byla zbalená
			{
				E->mGrid->VisibleRow(4,false,false);
				E->mGrid->VisibleRow(5,false,false);
				E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
			}
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,6);
			//definice buněk
			E->mGrid->Cells[0][1].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[1][3].Text=outPT(E->data.RT);
			E->mGrid->Cells[0][4].Text="max WT "+cas;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(m.round2double(E->WT,3));
			E->mGrid->Cells[0][5].Text=ls->Strings[223];//"Párová stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][5].Text=E->sparovany->name;else E->mGrid->Cells[1][5].Text="N/A";
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_2;//56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint=ls->Strings[242];//"celkový čas procesu otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint=ls->Strings[227];//"časová rezerva";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 200://PM
		case 300://výhybka
		{
			//určení zda budou prohozeny sloupce
			int prvni=3,druhy=4;
			if(prohodit_sloupce_PM(E)){prvni=4;druhy=3;}
			//vytvoření mgridu
			E->mGrid->Create(5,12);
			//nastavení popisků
			if(E->eID==300)E->mGrid->Cells[prvni][1].Text="IN/OUT";
			else E->mGrid->Cells[prvni][1].Text="IN";
			E->mGrid->Cells[druhy][1].Text="OUT";
			E->mGrid->Cells[4][0].Align=TmGrid::Talign::RIGHT;
			E->mGrid->Cells[4][0].AutoSizeComponent=3;//nastavý pouze výšku
			E->mGrid->Cells[4][0].Type=TmGrid::glyphBUTTON;
			E->mGrid->Cells[0][1].Text=ls->Strings[447];//"Pohon "
			E->mGrid->Cells[2][3].Text=ls->Strings[451];//"Nastavená"
			E->mGrid->Cells[2][4].Text=ls->Strings[448];//"Rozmezí"
			E->mGrid->Cells[2][6].Text=ls->Strings[209];//"Palce"
			E->mGrid->Cells[2][7].Text=ls->Strings[210];//"Násobek"
			E->mGrid->Cells[2][5].Text=ls->Strings[211];//"Jigů"
			E->mGrid->Cells[2][8].Text=ls->Strings[212];//"Podvozky"
			E->mGrid->Cells[2][9].Text=ls->Strings[213];//"Jigy 0°"
			E->mGrid->Cells[2][10].Text=ls->Strings[214];//"Jigy 90°"
			E->mGrid->Cells[0][11].Text="max "+ls->Strings[224]+" "+cas;
			//nastavení sloučených sloupců
			E->mGrid->Cells[0][3].Text=ls->Strings[452];//+" "+rychlost;//"Rychlost"
			E->mGrid->Cells[1][3].Text=rychlost;//jednotky
			E->mGrid->Cells[0][3].Font->Orientation=900;E->mGrid->Cells[0][3].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[1][3].Font->Orientation=900;E->mGrid->Cells[1][3].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][5].Text=ls->Strings[453];//"Rozteč"
			E->mGrid->Cells[1][5].Text=R;//jednotky
			E->mGrid->Cells[0][5].Font->Orientation=900;E->mGrid->Cells[0][5].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[1][5].Font->Orientation=900;E->mGrid->Cells[1][5].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[0][8].Text=ls->Strings[215];//"Mezera"
			E->mGrid->Cells[1][8].Text=Rz;//jednotky
			E->mGrid->Cells[0][8].Font->Orientation=900;E->mGrid->Cells[0][8].Valign=TmGrid::MIDDLE;
			E->mGrid->Cells[1][8].Font->Orientation=900;E->mGrid->Cells[1][8].Valign=TmGrid::MIDDLE;
			//nastavování hodnot druhého sloupce
			E->mGrid->Cells[3][2].Type=E->mGrid->COMBO;
			if(E->pohon!=NULL)
			{
				E->mGrid->Cells[prvni][3].Text=m.round2double(outaRD(E->pohon->aRD),3);
				E->mGrid->Cells[prvni][4].Text=AnsiString(m.round2double(outaRD(E->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(E->pohon->rychlost_do),3));
				E->mGrid->Cells[prvni][6].Text=m.round2double(outR(E->pohon->roztec),3);
				E->mGrid->Cells[prvni][7].Text=m.round2double(E->pohon->Rx,3);
				E->mGrid->Cells[prvni][5].Text=m.round2double(outR(E->pohon->Rz),3);
				E->mGrid->Cells[prvni][8].Text=m.round2double(outRz(m.mezera(0,E->pohon->Rz,0)),3);
				E->mGrid->Cells[prvni][9].Text=m.round2double(outRz(m.mezera(0,E->pohon->Rz,1)),3);
				E->mGrid->Cells[prvni][10].Text=m.round2double(outRz(m.mezera(90,E->pohon->Rz,1)),3);
			}
			else
			{
				E->mGrid->Cells[prvni][3].Text="-";
				E->mGrid->Cells[prvni][4].Text="-";
				E->mGrid->Cells[prvni][5].Text="-";
				E->mGrid->Cells[prvni][6].Text="-";
				E->mGrid->Cells[prvni][7].Text="-";
				E->mGrid->Cells[prvni][8].Text="-";
				E->mGrid->Cells[prvni][9].Text="-";
				E->mGrid->Cells[prvni][10].Text="-";
			}
			//E->mGrid->Cells[prvni][11].Text="-";
			//nastavování hodnot třetího sloupce
			E->mGrid->Cells[4][2].Type=E->mGrid->COMBO;
			if(E->eID==300)//pro výhybku
			{
				if(E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)
				{
					E->mGrid->Cells[druhy][3].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][3].Text=m.round2double(outaRD(E->dalsi2->pohon->aRD),3);
					E->mGrid->Cells[druhy][4].Text=AnsiString(m.round2double(outaRD(E->dalsi2->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(E->dalsi2->pohon->rychlost_do),3));
					/*E->mGrid->Cells[druhy][6].Type=E->mGrid->EDIT;*/E->mGrid->Cells[druhy][6].Text=m.round2double(outR(E->dalsi2->pohon->roztec),3);
					E->mGrid->Cells[druhy][7].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][7].Text=m.round2double(E->dalsi2->pohon->Rx,3);
					E->mGrid->Cells[druhy][5].Text=m.round2double(outR(E->dalsi2->pohon->Rz),3);
					E->mGrid->Cells[druhy][8].Text=m.round2double(outRz(m.mezera(0,E->dalsi2->pohon->Rz,0)),3);
					E->mGrid->Cells[druhy][9].Text=m.round2double(outRz(m.mezera(0,E->dalsi2->pohon->Rz,1)),3);
					E->mGrid->Cells[druhy][10].Text=m.round2double(outRz(m.mezera(90,E->dalsi2->pohon->Rz,1)),3);
				}
				else
				{
					E->mGrid->Cells[druhy][3].Text="-";
					E->mGrid->Cells[druhy][4].Text="-";
					E->mGrid->Cells[druhy][5].Text="-";
					E->mGrid->Cells[druhy][6].Text="-";
					E->mGrid->Cells[druhy][7].Text="-";
					E->mGrid->Cells[druhy][8].Text="-";
					E->mGrid->Cells[druhy][9].Text="-";
					E->mGrid->Cells[druhy][10].Text="-";
				}
			}
			else//pro PM
			{
				Cvektory::TElement *e_pom=E->dalsi;
				if(e_pom==NULL)e_pom=d.v.ELEMENTY->dalsi;//PM na konci linky
				if(e_pom->pohon!=NULL)
				{
					E->mGrid->Cells[druhy][3].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][3].Text=m.round2double(outaRD(e_pom->pohon->aRD),3);
					E->mGrid->Cells[druhy][4].Text=AnsiString(m.round2double(outaRD(e_pom->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(e_pom->pohon->rychlost_do),3));
					/*E->mGrid->Cells[druhy][6].Type=E->mGrid->EDIT;*/E->mGrid->Cells[druhy][6].Text=m.round2double(outR(e_pom->pohon->roztec),3);
					E->mGrid->Cells[druhy][7].Type=E->mGrid->EDIT;E->mGrid->Cells[druhy][7].Text=m.round2double(e_pom->pohon->Rx,3);
					E->mGrid->Cells[druhy][5].Text=m.round2double(outR(e_pom->pohon->Rz),3);
					E->mGrid->Cells[druhy][8].Text=m.round2double(outRz(m.mezera(0,e_pom->pohon->Rz,0)),3);
					E->mGrid->Cells[druhy][9].Text=m.round2double(outRz(m.mezera(0,e_pom->pohon->Rz,1)),3);
					E->mGrid->Cells[druhy][10].Text=m.round2double(outRz(m.mezera(90,e_pom->pohon->Rz,1)),3);
				}
				else
				{
					E->mGrid->Cells[druhy][3].Text="-";
					E->mGrid->Cells[druhy][4].Text="-";
					E->mGrid->Cells[druhy][5].Text="-";
					E->mGrid->Cells[druhy][6].Text="-";
					E->mGrid->Cells[druhy][7].Text="-";
					E->mGrid->Cells[druhy][8].Text="-";
					E->mGrid->Cells[druhy][9].Text="-";
					E->mGrid->Cells[druhy][10].Text="-";
				}
				e_pom=NULL;delete e_pom;
			}
			////temp odemknutí editace max WT
			E->mGrid->Cells[prvni][11].Type=E->mGrid->COMBO;
			E->mGrid->Cells[druhy][11].Type=E->mGrid->COMBO;
			////konec temp
//			if(E->WT!=0)E->mGrid->Cells[druhy][11].Text=outPT(E->WT);
//			else E->mGrid->Cells[druhy][11].Text="-";
			//nastavení šířek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=E->mGrid->Columns[1].Width=E->mGrid->Rows[0].Height;
			E->mGrid->Columns[2].Width=105;//90;
			E->mGrid->Columns[3].Width=145;
			E->mGrid->Columns[4].Width=145;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->ShowHint=true;
			E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky";
			//hinty
			E->mGrid->Cells[2][11].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[2][11].ShowHint=true;
			//merge + design hranic, vyjímečný případ, musí být před skrytím řádků !!!!!!!!!!!!!
			E->mGrid->Cells[0][0].RightBorder->Color=clWhite;E->mGrid->Cells[1][0].RightBorder->Color=clWhite;E->mGrid->Cells[2][0].RightBorder->Color=clWhite;E->mGrid->Cells[3][0].RightBorder->Color=clWhite;
			E->mGrid->Cells[0][0].BottomBorder->Width=2;E->mGrid->Cells[1][0].BottomBorder->Width=2;E->mGrid->Cells[2][0].BottomBorder->Width=2;E->mGrid->Cells[3][0].BottomBorder->Width=2;E->mGrid->Cells[4][0].BottomBorder->Width=2;
			E->mGrid->Cells[0][3].RightBorder->Color=clWhite;
			E->mGrid->MergeCells(0,3,0,4);E->mGrid->MergeCells(1,3,1,4);//sloučení pro rychlost
			E->mGrid->Cells[1][5].LeftBorder->Color=E->mGrid->Cells[1][7].LeftBorder->Color=clWhite;
			E->mGrid->MergeCells(0,5,0,7);E->mGrid->MergeCells(1,5,1,6);//sloučení pro rozteč
			E->mGrid->Cells[0][8].RightBorder->Color=clWhite;
			E->mGrid->MergeCells(0,8,0,10);E->mGrid->MergeCells(1,8,1,10);//sloučení pro mezery
			E->mGrid->Cells[0][11].RightMargin = 3;
			E->mGrid->Cells[0][11].Align=mGrid->RIGHT;
			E->mGrid->Cells[0][11].Font->Color=(TColor)RGB(128,128,128);
			E->mGrid->MergeCells(0,11,2,11);
			//skrytí řádků
			switch(OBJEKT_akt->stavPM)
			{
				//defaultní stav zobrazeny comba a rychlosti
				case 0:
				{
        	E->mGrid->VisibleRow(5,false,false);
		     	E->mGrid->VisibleRow(6,false,false);
		     	E->mGrid->VisibleRow(7,false,false);
		     	E->mGrid->VisibleRow(8,false,false);
		     	E->mGrid->VisibleRow(9,false,false);
		     	E->mGrid->VisibleRow(10,false,false);
					E->mGrid->VisibleRow(11,false,false);
					E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
					break;
				}
				//minimized, skrytí všeho kromě hlavičky a comb
				case 1:
				{
					E->mGrid->VisibleRow(3,false,false);
					E->mGrid->VisibleRow(4,false,false);
					E->mGrid->VisibleRow(5,false,false);
		     	E->mGrid->VisibleRow(6,false,false);
		     	E->mGrid->VisibleRow(7,false,false);
		     	E->mGrid->VisibleRow(8,false,false);
		     	E->mGrid->VisibleRow(9,false,false);
		     	E->mGrid->VisibleRow(10,false,false);
					E->mGrid->VisibleRow(11,false,false);
					E->mGrid->exBUTTONVisible=false;
					break;
				}
				//věe zobrazeno
				case 2:
				{
          E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
					break;
        }
			}
		}break;
		case 301:
		{
			String p1=ls->Strings[217],p2=ls->Strings[217];//"Žádný pohon";
			if(E->pohon!=NULL)p1=E->pohon->name;if(E->predchozi2->pohon!=NULL && E->dalsi2!=E->predchozi2)p2=E->predchozi2->pohon->name;    
			E->mGrid->Create(3,4);
			E->mGrid->Cells[0][1].Text=ls->Strings[447];//"Pohon "
			E->mGrid->Cells[1][1].Text="IN/OUT";E->mGrid->Cells[2][1].Text="IN";
			E->mGrid->Cells[1][2].Text=p1;E->mGrid->Cells[2][2].Text=p2;
			E->mGrid->Cells[1][3].Type=E->mGrid->COMBO;E->mGrid->Cells[2][3].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][3].Text=ls->Strings[224]+" "+cas;//"WT palec "
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=120;
			E->mGrid->Columns[1].Width=sirka_cisla+20;
			E->mGrid->Columns[2].Width=sirka_cisla+20;
			//oddělení sekcí
			E->mGrid->Cells[0][1].RightBorder->Width=2;E->mGrid->Cells[0][3].RightBorder->Width=2;
			E->mGrid->Cells[1][1].RightBorder->Width=2;E->mGrid->Cells[1][2].RightBorder->Width=2;E->mGrid->Cells[1][3].RightBorder->Width=2;
			E->mGrid->Cells[0][3].TopBorder->Width=2;E->mGrid->Cells[1][3].TopBorder->Width=2;E->mGrid->Cells[2][3].TopBorder->Width=2;
			//hinty
			E->mGrid->Cells[0][3].Hint=ls->Strings[229];//"maximální možná doba čekání na palec";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
    case 400:case 401:case 402://teplomer
		{
			E->mGrid->Create(3,1);
			E->mGrid->Cells[0][0].Text=ls->Strings[484];
			E->mGrid->Cells[1][0].Text=ls->Strings[485]+" "+cas;
			E->mGrid->Cells[2][0].Text="WT "+cas;
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=75;
			E->mGrid->Columns[1].Width=sirka_cisla;
			E->mGrid->Columns[2].Width=sirka_cisla;
      //nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;E->mGrid->exBUTTON->Hint=ls->Strings[231];//"Rozšířené položky"
			break;
		}
		default://pro elementy, které nemají mGrid
		{
			E->mGrid->Create(2,1);
			E->mGrid->VisibleRow(0,false,false);
			break;
		}
	}
	//vytvořené nové tabulky, musím přidat znova tag a id, lze předávat n do id, elementy jsou seřazené nehrozí, že 2 elementy budou mít stejné n
	E->mGrid->Tag=6;
	if(E->mGrid->ID==0)E->mGrid->ID=E->n;//zajistí přeindexování pouze v případě otevírání náhledu
	E->data.RT=puv_RT;//navrácení validace k RT hodnotě
}
//---------------------------------------------------------------------------
//slouží k vyčtení stávajícího nastavení jednotek, k jejich úpravě a zanesení do INI
void TForm1::redesign_element()
{
  log(__func__);//logování
	FormX->vstoupeno_elm=false;//zabrání spouštění OnChange při změně jednotek
	FormX->vstoupeno_poh=false;
	AnsiString delka_otoce,LO,cas,rychlost,R,Rz;
	short sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla;
	//když není font roboto nutno rozšířit buňky s textem
	if(aFont->Name!="Roboto Cn"){sirka_0=10;sirka_1=10;sirka_2=10;sirka_3=10;sirka_4=10;sirka_56=25;}
	bool zcas=false,zLO=false,zdelka_otoce=false,zrychlost=false,zR=false,zRz=false;
	//zjištění požadavku (co změnit)
	if(pom_element!=NULL)
	{
  	switch (pom_element->eID)
  	{
			case 0://stopstanice
			{
        if(JID==102 || JID==103 || JID==104)zcas=true;
				break;
      }
			case 1:case 7:case 11:case 15:case 101:case 105://kk roboti / operátoři
  		{
  			if (JID==101 || JID==102 || JID==103) zcas=true;//časové buňky
				if (JID==104 || JID==105) zLO=true;//délkové buňky
  			break;
  		}
			case 2:case 8:case 12:case 16:case 102:case 106://S&G roboti / operátoři
  		{
				if(JID==101 || JID==102 || JID==103)zcas=true;//obsahuje pouze casové buňky
  			break;
  		}
			case 3:case 9:case 13:case 17:case 103:case 107://kk roboti / operátoři s otočí
  		{
				if (JID==101 || JID==102 || JID==105 || JID==109 || JID==110 || JID==112) zcas=true;//čas
				if (JID==103 || JID==111) zLO=true;//delka
				if (JID==106 || JID==107 || JID==108) zdelka_otoce=true;//delka otoče
  			break;
  		}
			case 4:case 10:case 14:case 18:case 104:case 108://S&G roboti / operátoři s otočí
			{
				if(JID==101 || JID==103 || JID==104 || JID==105 || JID==106)zcas=true;//pouze čas
  			break;
  		}
			case 5://otoč pas.
  		{
  			if (JID==102 || JID==104 || JID==105) zdelka_otoce=true;//délka
  			if (JID==103) zcas=true;//čas
  			break;
			}
  		case 6://otoč akt.
  		{
				//if (JID==102) zdelka_otoce=true;//délka
  			if (JID==102 || JID==103 || JID==104) zcas=true;//čas
  			break;
  		}
			case 200://předávací místo
  		case 300://výhybka
			{
				if(JID==104)zrychlost=true;
				if(JID==105 || JID==106)zR=true;
				if(JID==109)zRz=true;
				if(JID==111)zcas=true;
  			break;
			}
			case 301://spojka
			{
				if(JID==103)zcas=true;
				break;
      }
      //teploměry
			case 400:case 401:case 402:
			{
				if(JID==100)zcas=true;
				break;
			}
  	}
	}
	//překlopení aktuálních jednotek
	if (zcas)
	{
		if (PTunit==SEC) {PTunit=MIN;}
		else {PTunit=SEC;}
	}
	if (zLO)
	{
		if (LOunit==M) {LOunit=MM;}
		else {LOunit=M;}
	}
	if (zdelka_otoce)
	{
		if (DOtocunit==M) {DOtocunit=MM;}
		else {DOtocunit=M;}
	}
	if (zrychlost)
	{
		if (aRDunit==SEC) {aRDunit=MIN;}
		else {aRDunit=SEC;}
	}
	if(zR)
	{
		if (Runit==M) Runit=MM;
		else Runit=M;
	}
	if(zRz)
	{
		if (Rzunit==M) Rzunit=MM;
		else Rzunit=M;
	}
  //kontrola, zda má být provedena nějaká změna
	if(zcas || zLO || zdelka_otoce || zrychlost || zR || zRz)
	{
  	//nastavení jednotek podle posledních nastavení
  	if (PTunit==SEC) cas="<a>[s]</a>";//0
  	else cas="<a>[min]</a>";//1
  	if (LOunit==M) LO="<a>[m]</a>";//0
  	else LO="<a>[mm]</a>";//1
  	if (DOtocunit==M) delka_otoce="<a>[m]</a>";//0
  	else delka_otoce="<a>[mm]</a>";//1
  	if (aRDunit==SEC) rychlost="<a>[m/s]</a>";//0
  	else rychlost="<a>m/min</a>";//1
  	if (Runit==M) R="<a>[m]</a>";//0
  	else R="<a>[mm]</a>";//1
  	if (Rzunit==M) Rz="<a>[m]</a>";//0
  	else Rz="<a>[mm]</a>";//1
  	//nastavení šířek
  	if(/*PTunit==SEC && */LOunit==M)sirka_1=90;
  	else sirka_1=105;
  	if(PTunit==0)sirka_2=95;
  	else sirka_2=107;
  	if(PTunit==SEC || language==Tlanguage::EN)sirka_3=45;
  	else sirka_3=55;
  	if(language==Tlanguage::EN)sirka_3+=15;
  	if(PTunit==Tminsec::SEC)sirka_0=80;
		else sirka_0=100;
		if(language!=Tlanguage::CS)sirka_0+=5;
  	sirka_cisla=90;
  	sirka_4=95;
  	sirka_56=96;
		//procházení elementu
		Cvektory::TElement *E=OBJEKT_akt->element;//zde lze přeskočit hlavičku
		if(predchozi_PM!=NULL)E=predchozi_PM;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
  	while (E!=NULL)
		{
  		akt_tabulek(E,LO,delka_otoce,cas,rychlost,R,Rz,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla);
			if(pom_element==NULL)napln_comba_mGridu(E);//pom_element bude nulový v případě změny jazyka... musí dojít k aktualizaci comb elementů
			if(predchozi_PM!=NULL && E==predchozi_PM)E=OBJEKT_akt->element;
			else E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
  	//změna jednotek pro tabulku teploměrů
  	if(OBJEKT_akt->teplomery!=NULL)
  	{
  		Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
  		if(T!=NULL && T->posledni->mGrid!=NULL)akt_tabulek(T->posledni,LO,delka_otoce,cas,rychlost,R,Rz,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla);
  		T=NULL;delete T;
		}
  	E=NULL; delete E;
  	//zápis změn do INI
  	writeINI("nastaveni_nahled", "cas", PTunit);
  	writeINI("nastaveni_nahled", "LO", LOunit);
  	writeINI("nastaveni_nahled", "delka_otoce", DOtocunit);
  	writeINI("nastaveni_form_parametry","RDt", aRDunit);
  	writeINI("nastaveni_nahled","R", Runit);
  	writeINI("nastaveni_nahled","Rz", Rzunit);
  	REFRESH();
		nahled_ulozit(true);
	}
}
//---------------------------------------------------------------------------
//přepisuje jednotky a upravuje šířku sloupců
void TForm1::akt_tabulek (Cvektory::TElement *E,AnsiString LO,AnsiString delka_otoce,AnsiString cas,AnsiString rychlost,AnsiString R,AnsiString Rz,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla)
{
	log(__func__);//logování
	double puv_RT=E->data.RT;//RT, obsahující validaci
	//extrakce pouze RT
	if(E->data.RT>1000000)E->data.RT-=1000000;
	if(E->data.RT<-1000000)E->data.RT+=1000000;
	switch(E->eID)
	{
		case 0://stop stanice
		{        
			E->mGrid->Cells[1][1].Text=ls->Strings[223];//"Párová stop";
			E->mGrid->Cells[1][2].Text="RT "+cas;
			if(E->mGrid->Cells[2][2].Text!="OK")E->mGrid->Cells[2][2].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[2][2].Hint=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[1][3].Text=ls->Strings[419]+" "+cas;
			E->mGrid->Cells[2][3].Text=m.round2double(outPT(E->data.WTstop),3);
			E->mGrid->Cells[0][6].Text=ls->Strings[464];//"Pozice"
			E->mGrid->Cells[1][4].Text=ls->Strings[224]+" "+cas;//"WT palec "
			E->mGrid->Cells[2][4].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Cells[1][5].Text=ls->Strings[225];//"počet pozic";
			E->mGrid->Cells[1][6].Text=ls->Strings[226];//"počet vozíků";
			E->mGrid->Columns[1].Width=sirka_0;
			break;
		}
		case 1:case 7:case 11:case 15:case 101:case 105://robot (kontinuální)
		{
			double aRD=0;if(E->pohon!=NULL)aRD=E->pohon->aRD;
			AnsiString t=ls->Strings[232];//"LO "
			if(E->eID!=1)t=ls->Strings[234];//"PO "
			E->mGrid->Cells[0][1].Text="CT "+cas;
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[0][4].Text=t+LO;
			E->mGrid->Cells[0][5].Text=ls->Strings[236]+" "+LO;//"vyosení "
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(m.CT(E->data.LO1,aRD)),3);
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[1][4].Text=m.round2double(outLO(E->data.LO1),3);
			E->mGrid->Cells[1][5].Text=m.round2double(outLO(E->data.LO_pozice),3);
			E->mGrid->Columns[0].Width=sirka_1;
			//if(sirka_cisla<100)sirka_cisla+=15;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
		{
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[0][2].Text="RT "+cas;
			E->mGrid->Cells[0][3].Text="max WT "+cas;
			E->mGrid->Cells[0][4].Text=ls->Strings[223];//"Párová stop";
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Columns[0].Width=sirka_2;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otočí
		{
			double aRD=0;if(E->pohon!=NULL)aRD=E->pohon->aRD;
			AnsiString t=ls->Strings[232];//"LO"
			if(E->eID!=3)t=ls->Strings[234];//"PO"
			E->mGrid->Cells[2][1].Text="CT1 "+cas;
			E->mGrid->Cells[2][2].Text="PT1 "+cas;
			E->mGrid->Cells[2][3].Text=t+"1 "+LO;
			E->mGrid->Cells[0][8].Text=ls->Strings[240];//"Otoč"
			E->mGrid->Cells[2][4].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[2][5].Text=ls->Strings[241]+" "+cas;//"Otáčení"
			E->mGrid->Cells[2][6].Text=ls->Strings[246]+" "+delka_otoce;//"délka "
			E->mGrid->Cells[1][7].Text=ls->Strings[463];E->mGrid->Cells[1][8].Text=delka_otoce;
			E->mGrid->Cells[2][7].Text=ls->Strings[275];//"Zóna před "
			E->mGrid->Cells[2][8].Text=ls->Strings[276];//"Zóna za "
			E->mGrid->Cells[2][9].Text="CT2 "+cas;
			E->mGrid->Cells[2][10].Text="PT2 "+cas;
			E->mGrid->Cells[2][11].Text=t+"2 "+LO;
			E->mGrid->Cells[2][12].Text="RT "+cas;
			E->mGrid->Cells[2][13].Text="PD jig";
			E->mGrid->Cells[3][1].Text=m.round2double(outPT(m.CT(E->data.LO1,aRD)),3);
			E->mGrid->Cells[3][2].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[3][2].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[3][3].Text=m.round2double(outLO(E->data.LO1),3);
			E->mGrid->Cells[3][5].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[3][6].Text=m.round2double(outDO(E->OTOC_delka),3);
			E->mGrid->Cells[3][7].Text=m.round2double(outDO(E->zona_pred),3);
			E->mGrid->Cells[3][8].Text=m.round2double(outDO(E->zona_za),3);
			E->mGrid->Cells[3][9].Text=m.round2double(outPT(m.CT(E->data.LO2,aRD)),3);
			E->mGrid->Cells[3][10].Text=m.round2double(outPT(E->data.PT2),3);
			E->mGrid->Cells[3][11].Text=m.round2double(outLO(E->data.LO2),3);
			E->mGrid->Cells[3][12].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Columns[2].Width=sirka_3;
			break;
		}
		case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[0][2].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[0][3].Text=ls->Strings[245]+" "+cas;//"PTo
			E->mGrid->Cells[0][4].Text="PT2 "+cas;
			E->mGrid->Cells[0][5].Text="RT "+cas;
			E->mGrid->Cells[0][6].Text="max WT "+cas;
			E->mGrid->Cells[0][7].Text=ls->Strings[223];//"Párová stop";
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->data.PT1),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[1][4].Text=m.round2double(outPT(E->data.PT2),3);
			E->mGrid->Cells[1][5].Text=m.round2double(outPT(E->data.RT),3);
			E->mGrid->Cells[1][6].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Columns[0].Width=sirka_4;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 5://otoč pasivní
		{
      //nastavení šířek
			if(language==Tlanguage::CS)
			{
				if(DOtocunit==Tm_mm::MM)sirka_56-=10;
				else sirka_56-=25;
			}
			else
			{
				if(DOtocunit==Tm_mm::MM)sirka_56+=5;
				else sirka_56-=10;
			}
			E->mGrid->Cells[1][1].Text=ls->Strings[239]+" [°]";//"rotace [°]"
			E->mGrid->Cells[1][2].Text=ls->Strings[246]+" "+delka_otoce;//"délka "
			E->mGrid->Cells[1][3].Text="PT "+cas;
			E->mGrid->Cells[0][5].Text=ls->Strings[463];//"Zóna"
			E->mGrid->Cells[1][4].Text=ls->Strings[275]+" "+delka_otoce;//"před "
			E->mGrid->Cells[1][5].Text=ls->Strings[276]+" "+delka_otoce;//"za "
			E->mGrid->Cells[2][2].Text=m.round2double(outDO(E->OTOC_delka),3);
			E->mGrid->Cells[2][3].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[2][4].Text=m.round2double(outDO(E->zona_pred),3);
			E->mGrid->Cells[2][5].Text=m.round2double(outDO(E->zona_za),3);
			E->mGrid->Columns[1].Width=sirka_56;
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
			E->mGrid->Cells[0][1].Text=ls->Strings[239]+" [°]";//"rotace [°]";
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->data.RT),3);
      E->mGrid->Cells[0][4].Text="max WT "+cas;
			E->mGrid->Cells[1][4].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Cells[0][5].Text=ls->Strings[223];//"Párová stop";
			E->mGrid->Columns[0].Width=sirka_2;//56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 200://PM
		case 300://výhybka
		{
      //určení zda budou prohozeny sloupce
			int prvni=3,druhy=4;
			if(prohodit_sloupce_PM(E)){prvni=4;druhy=3;}
			//upravení popisků
			E->mGrid->Cells[2][2].Text=ls->Strings[447];//"Pohon"
			E->mGrid->Cells[2][3].Text=ls->Strings[451];//"Nastavená"
			E->mGrid->Cells[2][4].Text=ls->Strings[448];//"Rozmezí"
			E->mGrid->Cells[2][6].Text=ls->Strings[209];//"Palce"
			E->mGrid->Cells[2][7].Text=ls->Strings[210];//"Násobek"
			E->mGrid->Cells[2][5].Text=ls->Strings[211];//"Jigů"
			E->mGrid->Cells[2][8].Text=ls->Strings[212];//"Podvozky"
			E->mGrid->Cells[2][9].Text=ls->Strings[213];//"Jigy 0°"
			E->mGrid->Cells[2][10].Text=ls->Strings[214];//"Jigy 90°"
			E->mGrid->Cells[2][11].Text="max "+ls->Strings[224]+" "+cas;//"max WT palec"
			//popisky sloučených sloupců
			E->mGrid->Cells[0][4].Text=ls->Strings[452];///"Rychlost"
			E->mGrid->Cells[1][4].Text=rychlost;//jednotky
			E->mGrid->Cells[0][7].Text=ls->Strings[453];//"Rozteč"
			E->mGrid->Cells[1][6].Text=R;//jednotky
			E->mGrid->Cells[0][10].Text=ls->Strings[215];//"Mezera"
			E->mGrid->Cells[1][10].Text=Rz;//jednotky
			//přepočet dat
			if(E->pohon!=NULL)//společné pro PM a výhybku
			{
				E->mGrid->Cells[prvni][3].Text=m.round2double(outaRD(E->pohon->aRD),3);
				E->mGrid->Cells[prvni][4].Text=AnsiString(m.round2double(outaRD(E->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(E->pohon->rychlost_do),3));
				E->mGrid->Cells[prvni][6].Text=m.round2double(outR(E->pohon->roztec),3);
				if(E->mGrid->Cells[prvni][7].Text!="-")E->mGrid->Cells[prvni][7].Text=m.round2double(E->pohon->Rx,3);
				if(E->mGrid->Cells[prvni][5].Text!="-")E->mGrid->Cells[prvni][5].Text=m.round2double(outR(E->pohon->Rz),3);
				if(E->mGrid->Cells[prvni][8].Text!="-")E->mGrid->Cells[prvni][8].Text=m.round2double(outRz(m.mezera(0,E->pohon->Rz,0)),3);
				if(E->mGrid->Cells[prvni][9].Text!="-")E->mGrid->Cells[prvni][9].Text=m.round2double(outRz(m.mezera(0,E->pohon->Rz,1)),3);
				if(E->mGrid->Cells[prvni][10].Text!="-")E->mGrid->Cells[prvni][10].Text=m.round2double(outRz(m.mezera(90,E->pohon->Rz,1)),3);
			}
			if(E->eID==300 && E->dalsi2!=E->predchozi2 && E->dalsi2->pohon!=NULL)//pro výhybku
			{
				{
					E->mGrid->Cells[druhy][3].Text=m.round2double(outaRD(E->dalsi2->pohon->aRD),3);
					E->mGrid->Cells[druhy][4].Text=AnsiString(m.round2double(outaRD(E->dalsi2->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(E->dalsi2->pohon->rychlost_do),3));
					E->mGrid->Cells[druhy][6].Text=m.round2double(outR(E->dalsi2->pohon->roztec),3);
					if(E->mGrid->Cells[druhy][7].Text!="-")E->mGrid->Cells[druhy][7].Text=m.round2double(E->dalsi2->pohon->Rx,3);
					if(E->mGrid->Cells[druhy][5].Text!="-")E->mGrid->Cells[druhy][5].Text=m.round2double(outR(E->dalsi2->pohon->Rz),3);
					if(E->mGrid->Cells[druhy][8].Text!="-")E->mGrid->Cells[druhy][8].Text=m.round2double(outRz(m.mezera(0,E->dalsi2->pohon->Rz,0)),3);
					if(E->mGrid->Cells[druhy][9].Text!="-")E->mGrid->Cells[druhy][9].Text=m.round2double(outRz(m.mezera(0,E->dalsi2->pohon->Rz,1)),3);
					if(E->mGrid->Cells[druhy][10].Text!="-")E->mGrid->Cells[druhy][10].Text=m.round2double(outRz(m.mezera(90,E->dalsi2->pohon->Rz,1)),3);
				}
			}
			if(E->eID==200)//pro PM
			{
				Cvektory::TElement *e_pom=E->dalsi;
				if(e_pom==NULL)e_pom=d.v.ELEMENTY->dalsi;//v případě PM na konci linky
				if(e_pom->pohon!=NULL)
				{
					E->mGrid->Cells[druhy][3].Text=m.round2double(outaRD(e_pom->pohon->aRD),3);
					E->mGrid->Cells[druhy][4].Text=AnsiString(m.round2double(outaRD(e_pom->pohon->rychlost_od),3))+" - "+AnsiString(m.round2double(outaRD(e_pom->pohon->rychlost_do),3));
					E->mGrid->Cells[druhy][6].Text=m.round2double(outR(e_pom->pohon->roztec),3);
					if(E->mGrid->Cells[druhy][7].Text!="-")E->mGrid->Cells[druhy][7].Text=m.round2double(e_pom->pohon->Rx,3);
					if(E->mGrid->Cells[druhy][5].Text!="-")E->mGrid->Cells[druhy][5].Text=m.round2double(outR(e_pom->pohon->Rz),3);
					if(E->mGrid->Cells[druhy][8].Text!="-")E->mGrid->Cells[druhy][8].Text=m.round2double(outRz(m.mezera(0,e_pom->pohon->Rz,0)),3);
					if(E->mGrid->Cells[druhy][9].Text!="-")E->mGrid->Cells[druhy][9].Text=m.round2double(outRz(m.mezera(0,e_pom->pohon->Rz,1)),3);
					if(E->mGrid->Cells[druhy][10].Text!="-")E->mGrid->Cells[druhy][10].Text=m.round2double(outRz(m.mezera(90,e_pom->pohon->Rz,1)),3);
				}
				e_pom=NULL;delete e_pom;
			}
			napln_comba_mGridu(E);
			break;
		}
		case 301:
		{
			E->mGrid->Cells[0][3].Text=ls->Strings[224]+" "+cas;//"WT palec "
			napln_comba_mGridu(E);
			break;
		}
		case 400:case 401:case 402:
		{
			//změna popisků
			E->mGrid->Cells[0][0].Text=ls->Strings[484];
			E->mGrid->Cells[1][0].Text=ls->Strings[485]+" "+cas;
			E->mGrid->Cells[2][0].Text="WT "+cas;
			double cas=0,WT=0;
			for(unsigned int i=1;i<E->mGrid->RowCount-1;i++)
			{
				//změna popisků
				if(E->mGrid->Cells[0][i].Hint!=0)E->mGrid->Cells[0][i].Text=ls->Strings[ms.MyToDouble(E->mGrid->Cells[0][i].Hint)];
				//extrakce času a WT
				if (PTunit==SEC) {PTunit=MIN;}//nutnost změnit na předchozí jednotky, abych dostal raw data
				else {PTunit=SEC;}
				cas=ms.MyToDouble(inPT(ms.MyToDouble(E->mGrid->Cells[1][i].Text)));
				WT=ms.MyToDouble(inPT(ms.MyToDouble(E->mGrid->Cells[2][i].Text)));
				//navrácení času a WT nově v jiných jednotkách
				if (PTunit==SEC) {PTunit=MIN;}
				else {PTunit=SEC;}
				E->mGrid->Cells[1][i].Text=m.round2double(outPT(cas),3);
				E->mGrid->Cells[2][i].Text=m.round2double(outPT(WT),3);
			}
			//změna popisků
			E->mGrid->Cells[0][E->mGrid->RowCount-1].Text=ls->Strings[487];
			//extrakce celkového času
			if (PTunit==SEC) {PTunit=MIN;}//nutnost změnit na předchozí jednotky, abych dostal raw data
			else {PTunit=SEC;}
			cas=ms.MyToDouble(inPT(ms.MyToDouble(E->mGrid->Cells[2][E->mGrid->RowCount-1].Text)));
			//navrácení času a WT nově v jiných jednotkách
			if (PTunit==SEC) {PTunit=MIN;}
			else {PTunit=SEC;}
			E->mGrid->Cells[2][E->mGrid->RowCount-1].Text=m.round2double(outPT(cas),3);
			break;
		}
	}
  FormX->zmen_jednotky_validace(E);
	E->data.RT=puv_RT;//navrácení validace k RT hodnotě
}
//---------------------------------------------------------------------------
//přepnutí jednotek v kótách, zapíše do globální proměnné a do INI
void TForm1::zmenJednotekKot()
{
  log(__func__);//logování
	switch(scGPComboBox_prepinacKot->ItemIndex)
	{
		case 0:if(DKunit==M) DKunit=MM;else DKunit=M;break;//délkové
		case 1:if(DKunit==SEKUNDY) DKunit=MINUTY;else DKunit=SEKUNDY;break;
	}
	writeINI("nastaveni_nahled","koty_delka", DKunit);
	nahled_ulozit(true);
	REFRESH();
}
//---------------------------------------------------------------------------
//zapíná či vypíná automatickou ortogonalizaci
void TForm1::ortogonalizace_on_off()
{
	log(__func__);//logování
	if(ortogonalizace_stav)//pokud je zapnuta
	{
		ortogonalizace_stav=false;//tak vypne
		SB(ls->Strings[385]);//"Ortogonalizace vypnuta."
	}
	else//pokud vypnuta, tak zapne, zortogonalizuje a zapíše stav do registru
	{
		ortogonalizace_stav=true;
		ortogonalizovat();
		REFRESH();
		SB(ls->Strings[386]);//"Ortogonalizace zapnuta."
	}
}
//---------------------------------------------------------------------------
//volá ortogonalizaci schéma, pokud je ortogonalizace povolena
void TForm1::ortogonalizace()
{
  log(__func__);//logování
	if(ortogonalizace_stav)ortogonalizovat();
}
//---------------------------------------------------------------------------
//ortogonalizuje schéma + přichytne ke mřížce pokud je požadováno
void TForm1::ortogonalizovat()
{
  log(__func__);//logování
  d.v.ortogonalizovat();
}
//---------------------------------------------------------------------------
void TForm1::zneplatnit_minulesouradnice()
{
  log(__func__);//logování
	minule_souradnice_kurzoru=TPoint(-200,-200);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_otoceDrawCell(TObject *Sender, int ACol, int ARow,
					TRect &Rect, TGridDrawState State)
{
	log(__func__);//logování
	if(pom->id!=3)//nevykreslovat nic v POW
	{
  	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
  	int W=DrawGrid_otoce->DefaultColWidth  *Z;
  	int H=DrawGrid_otoce->DefaultRowHeight  *Z;
  	int P=-1*DrawGrid_otoce->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
  	int odsazeni=10,odsazeniX=10;

  	Cantialising a;
  	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
  	bmp_in->Width=DrawGrid_otoce->Width*Z;bmp_in->Height=DrawGrid_otoce->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
  	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

  	//unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
  	double Zoom_back=Zoom;//záloha zoomu
  	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
  	short pocet_elementu=2;
  	AnsiString label1;
  	AnsiString label2;

  	for(unsigned short n=1;n<=pocet_elementu;n++)
  	{
			if(n==1)label1=ls->Strings[264];//"pasivní";
			if(n==2)label1=ls->Strings[263];//"aktivní";
			if(OBJEKT_akt->id!=3 && OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
  		{
				d.vykresli_otoc(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-odsazeniX,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P - 15-odsazeni,label1,label2,n+4,0,180,1);
			}
  		else d.vykresli_otoc(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-odsazeniX,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P - 15-odsazeni,label1,label2,n+4,0,180,-1);
  	}

  	Zoom=Zoom_back;//návrácení původního zoomu
  	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
  	DrawGrid_otoce->Canvas->Draw(0,0,bmp_out);
  	delete (bmp_out);//velice nutné
  	delete (bmp_in);//velice nutné
	}
}
//------------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_ostatniDrawCell(TObject *Sender, int ACol, int ARow,
					TRect &Rect, TGridDrawState State)
{
	log(__func__);//logování
	if(pom->id!=3)//nevykreslovat nic v POW
	{
  	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
  	int W=DrawGrid_ostatni->DefaultColWidth  *Z;
  	int H=DrawGrid_ostatni->DefaultRowHeight  *Z;
  	int P=-1*DrawGrid_ostatni->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
  	int odsazeni=10;
  	scGPLabel_stop->Caption=ls->Strings[60];//"Stop stanice";

  	Cantialising a;
  	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
  	bmp_in->Width=DrawGrid_ostatni->Width*Z;bmp_in->Height=DrawGrid_ostatni->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
  	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

  	double Zoom_back=Zoom;//záloha zoomu
  	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
  	short pocet_elementu=1;
  	for(unsigned short n=1;n<=pocet_elementu;n++)
  	{
			if(OBJEKT_akt->id!=3 && OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
  		{
  			d.vykresli_stopku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-3,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 2-odsazeni,"STOP","",0,180);
  		}
  		else d.vykresli_stopku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-3,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 2-odsazeni,"STOP","",0,180,-1);
  		//ion. tyč
  		if(OBJEKT_akt->id==4)//pouze pro ionizaci
  		{
				odsazeni=25;
				scGPLabel_stop->Caption=ls->Strings[53];//"Ostatní";
				AnsiString label1=ls->Strings[265],label2="";//"ionizační tyč"
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))d.vykresli_ion(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W+6,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P-odsazeni,label1,label2,0,90,1);
  			else d.vykresli_ion(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W+6,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P-odsazeni,label1,label2,0,90,-1);
  		}
  	}
  	Zoom=Zoom_back;//návrácení původního zoomu
  	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
  	DrawGrid_ostatni->Canvas->Draw(0,0,bmp_out);
  	delete (bmp_out);//velice nutné
  	delete (bmp_in);//velice nutné
	}
}

//------------------------------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_poznamkyDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	log(__func__);//logování
	if(pom->id!=3)//nevykreslovat nic v POW
	{
  	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
  	int odsazeni=8;
  	int W=DrawGrid_poznamky->DefaultColWidth  *Z;
  	int H=DrawGrid_poznamky->DefaultRowHeight  *Z;
  	int P=-1*DrawGrid_poznamky->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování

  	Cantialising a;
  	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
  	bmp_in->Width=DrawGrid_poznamky->Width*Z;bmp_in->Height=DrawGrid_poznamky->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
  	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

   //	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
  	double Zoom_back=Zoom;//záloha zoomu
  	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
  	short pocet_elementu=2;
  	AnsiString label1;
  	AnsiString label2;
  	for(unsigned short n=1;n<=pocet_elementu;n++)
  	{
  		if(n==1)
  		{
			 label1=ls->Strings[266];//"text";
  		 label2="";
			 d.vykresli_ikonu_textu(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W+odsazeni,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1,-1);
     //	d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 30,label1,label2,n);
  		 }
      if(n==2)
  		{
  		 label1=ls->Strings[267];//"šipka";
  		 label2="";
  		 d.vykresli_ikonu_sipky(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W+odsazeni,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1,-1);
  		 }

  	}
  	Zoom=Zoom_back;//návrácení původního zoomu
  	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
  	DrawGrid_poznamky->Canvas->Draw(0,0,bmp_out);
  	delete (bmp_out);//velice nutné
  	delete (bmp_in);//velice nutné
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
			TGridDrawState State)
{
	log(__func__);//logování
	if(MOD==EDITACE && pom!=NULL && OBJEKT_akt!=NULL)//ošetření navíc
	{
		scListGroupKnihovObjektu->Caption="Roboti";
		DrawGrid_knihovna->RowCount=2;  //nastaveni poctu radku, aby nedochazelo k posunu gridu pri scrollovani
		if(OBJEKT_akt->id!=3 && (DrawGrid_knihovna->ColCount!=2 || DrawGrid_knihovna->DefaultColWidth!=80)){DrawGrid_knihovna->ColCount=2;DrawGrid_knihovna->DefaultColWidth=80;}//ošetření probliku
		if(OBJEKT_akt->id==3 && (DrawGrid_knihovna->ColCount!=1 || DrawGrid_knihovna->DefaultColWidth!=160)) {DrawGrid_knihovna->ColCount=1;DrawGrid_knihovna->DefaultColWidth=160;}
//		DrawGrid_knihovna->Left=3;
		short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
		int W=DrawGrid_knihovna->DefaultColWidth  *Z;
		int H=DrawGrid_knihovna->DefaultRowHeight  *Z;
		int P=-1*DrawGrid_knihovna->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
		int odsazeni=19;
		Cantialising a;
		Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
		bmp_in->Width=DrawGrid_knihovna->Width*Z;bmp_in->Height=DrawGrid_knihovna->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
		TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty
	 //	unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
		double Zoom_back=Zoom;//záloha zoomu
		Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
		AnsiString label1;
		AnsiString label2,label_pom,KK,KKs,akt_otoc,pas_otoc;
		KK=ls->Strings[256];//"kontinuální";
		KKs=ls->Strings[257];//"kontinuální s";
		pas_otoc=ls->Strings[258];//"pasiv. otočí";
		akt_otoc=ls->Strings[259];//"akt. otočí";
    //překreslení původních
		C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
		C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
		C->Rectangle(-1,-1,m.round(2.1*W),m.round(2.1*H));//překreslní původních robotů ("smazání")

		switch(OBJEKT_akt->id)
		{
			case 3://POW
			{
				label1=ls->Strings[252],label2=ls->Strings[253];//"okap","postřikový rám"
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					d.vykresli_ikonu_komory(C,140,60,label1,0);//"okap"
					d.vykresli_ikonu_komory(C,140,480,label2,1);//"postřikový rám"
				}
				else
				{
					d.vykresli_ikonu_komory(C,140,60,label1,0,-1);//"okap"
					d.vykresli_ikonu_komory(C,140,480,label2,1,-1);//"postřikový rám"
				}
				if(scListGroupPanel_hlavickaOtoce->Top==314)//spouštět pouze při prvním refreshi
				{
					double odsazeni=10000;//
					scListGroupPanel_hlavickaOtoce->Top+=odsazeni;
					scListGroupPanel_hlavickaOstatni->Top+=odsazeni;
					scListGroupPanel_geometrie->Top=DrawGrid_knihovna->Top+DrawGrid_knihovna->Height+5;
					scListGroupPanel_poznamky->Top+=odsazeni;
					DrawGrid_poznamky->Height=1000;//rozšíření, zabránění zobrazení linky pod knihovnou
					scGPLabel_otoce->Top+=odsazeni;
					scGPLabel_stop->Top+=odsazeni;
					scGPLabel_geometrie->Top=DrawGrid_knihovna->Top+DrawGrid_knihovna->Height+5;
					scGPLabel_poznamky->Top+=odsazeni;
				}
			}break;
			case 0:case 9://objekt navěšování + svěšování
			{
				//prvotní vykreslení všech robotů
				Zoom=15;//zvětšení zoomu při použití robotů
				odsazeni=54;//vycentrování lidských robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				label_pom=ls->Strings[254];//"navěšování"
				if(OBJEKT_akt->id==9)label_pom=ls->Strings[255];//"svěšování"
				for(unsigned short n=101;n<=104;n++)
				{
					//nastavení názvů
					if(n==101){label1=KK;label2=label_pom;}
					if(n==102){label1="S&G";label2=label_pom;}
					if(n==103){label1=KKs;label2=pas_otoc;}
					if(n==104){label1=ls->Strings[429];label2=akt_otoc;}
					if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
					{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n);
					}
					else d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					if(OBJEKT_akt->rezim==1)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,101);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,102,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,103);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,104,0,0,-1);
			  	}
					else if (OBJEKT_akt->rezim==0)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,101,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,102);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,103,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,104);
			  	}
				}
			}break;
			case 1://objekt CO2
			{
				//prvotní vykreslení všech robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				for(unsigned short n=15;n<=18;n++)
				{
					//nastavení názvů
					if(n==15){label1=KK;label2="CO2";}
					if(n==16){label1="S&G";label2="CO2";}
					if(n==17){label1=KKs;label2=pas_otoc;}
					if(n==18){label1=ls->Strings[429];label2=akt_otoc;}
					if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
					{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n);
					}
					else d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					if(OBJEKT_akt->rezim==1)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,"CO2",15);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","CO2",16,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,17);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,18,0,0,-1);
			  	}
					else if (OBJEKT_akt->rezim==0)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,"CO2",15,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","CO2",16);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,17,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,18);
			  	}
				}
			}break;
			case 2://objekt ožeh
			{
				//funkce přepínání mezi roboty  lidskými roboty
				unsigned short n_od,n_do,rob1,rob2,rob3,rob4;
				if(scGPSwitch_robot_clovek->State==0){n_od=11;n_do=14;rob1=11;rob2=12;rob3=13;rob4=14;Zoom=10;odsazeni=19;}
				else{n_od=105;n_do=108;rob1=105;rob2=106;rob3=107;rob4=108;Zoom=15;odsazeni=54;}
				//prvotní vykreslení všech robotů
				label_pom=ls->Strings[260];//"ožeh";
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				for(unsigned short n=n_od;n<=n_do;n++)
				{
					//nastavení názvů
					if(n==rob1){label1=KK;label2=label_pom;}//"ožeh"
					if(n==rob2){label1="S&G";label2=label_pom;}//"ožeh"
					if(n==rob3){label1=KKs;label2=pas_otoc;}//"pasiv. otočí"
					if(n==rob4){label1=ls->Strings[429];label2=akt_otoc;}//"akt. otočí"
					if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
					{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n);
					}
					else d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					if(OBJEKT_akt->rezim==1)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,rob1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,rob2,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,rob3);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,rob4,0,0,-1);
			  	}
					else if (OBJEKT_akt->rezim==0)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,rob1,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,rob2);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,rob3,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,rob4);
			  	}
				}
			}break;
			case 4://objekt ionizace
			{
				//prvotní vykreslení všech robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				//funkce přepínání mezi roboty  lidskými roboty
				unsigned short n_od,n_do,rob1,rob2,rob3,rob4;
				if(scGPSwitch_robot_clovek->State==0){n_od=7;n_do=10;rob1=7;rob2=8;rob3=9;rob4=10;Zoom=10;odsazeni=19;}
				else{n_od=105;n_do=108;rob1=105;rob2=106;rob3=107;rob4=108;Zoom=15;odsazeni=54;}
				label_pom=ls->Strings[261];//"ionizace";
				if((OBJEKT_akt->pohon!=NULL && OBJEKT_akt->rezim==-1) || OBJEKT_akt->pohon==NULL)
				{
			  	for(unsigned short n=n_od;n<=n_do;n++)
			  	{
			  		//nastavení názvů
			  		if(n==7||n==105){label1=KK;label2=label_pom;}//"ionizace"
			  		if(n==8||n==106){label1="S&G";label2=label_pom;}//"ionizace"
			  		if(n==9||n==107){label1=KKs;label2=pas_otoc;}
			  		if(n==10||n==108){label1=ls->Strings[429];label2=akt_otoc;}
			  		if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
			  		{
							d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0);
			  		}
						else d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
			  		i++;
			  	}
				}
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					if(OBJEKT_akt->rezim==1)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,rob1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,rob2,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,rob3);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,rob4,0,0,-1);
			  	}
					else if (OBJEKT_akt->rezim==0)
			  	{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,rob1,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,rob2);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,rob3,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,rob4);
			  	}
				}
			}break;
			case 5://objekt lakování
			{
				//funkce přepínání mezi roboty  lidskými roboty
				unsigned short n_od,n_do,rob1,rob2,rob3,rob4;
				if(scGPSwitch_robot_clovek->State==0){n_od=1;n_do=4;rob1=1;rob2=2;rob3=3;rob4=4;Zoom=10;odsazeni=19;}
				else{n_od=105;n_do=108;rob1=105;rob2=106;rob3=107;rob4=108;Zoom=15;odsazeni=54;}
				//prvotní vykreslení všech robotů
				label_pom=ls->Strings[262];//"lakování";
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				for(unsigned short n=n_od;n<=n_do;n++)
				{
					if(n==rob1){label1=KK;label2=label_pom;}//"lakování"
					if(n==rob2){label1="S&G";label2=label_pom;}//"lakování"
					if(n==rob3){label1=KKs;label2=pas_otoc; }
					if(n==rob4){label1=ls->Strings[429];label2=akt_otoc;}
					if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
					{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0);
					}
					else d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					if(OBJEKT_akt->rezim==1)
					{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,rob1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,rob2,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,rob3);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,rob4,0,0,-1);
					}
					else if (OBJEKT_akt->rezim==0)
					{
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,KK,label_pom,rob1,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,rob2);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,KKs,pas_otoc,rob3,0,0,-1);
						d.vykresli_element(C,0,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,ls->Strings[429],akt_otoc,rob4);
					}
				}
			}break;
			default://pro objekty které nemají žádné elementy, přepozicovat zbývající knihovny
			{
				if(scListGroupPanel_hlavickaOtoce->Top==314)//spouštět pouze při prvním refreshi
				{
					double odsazeni=314;//změna pozice o -314
					scListGroupPanel_hlavickaOtoce->Top-=odsazeni;
					scListGroupPanel_hlavickaOstatni->Top-=odsazeni;
					scListGroupPanel_geometrie->Top-=odsazeni;
					scListGroupPanel_poznamky->Top-=odsazeni;
					DrawGrid_poznamky->Height=1000;//rozšíření, zabránění zobrazení linky pod knihovnou
					scGPLabel_otoce->Top-=odsazeni;
					scGPLabel_stop->Top-=odsazeni;
					scGPLabel_geometrie->Top-=odsazeni;
					scGPLabel_poznamky->Top-=odsazeni;
				}
			}break;
		}
		Zoom=Zoom_back;//návrácení původního zoomu
		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
		DrawGrid_knihovna->Canvas->Draw(0,0,bmp_out);
		delete (bmp_out);//velice nutné
		delete (bmp_in);//velice nutné
	}
//	if(MOD==LAYOUT)
//	{
//
//		////////////////////neAA verze
//		scListGroupKnihovObjektu->Caption="Technolog.objekty";
//		DrawGrid_knihovna->Left=14;
//    if(pocet_objektu_knihovny%2!=0)DrawGrid_knihovna->RowCount=(pocet_objektu_knihovny+1)/2,0;//pokud je počet objektu lichý
//		else DrawGrid_knihovna->RowCount=m.round2double(pocet_objektu_knihovny/2,0);//sudý počet objektů
//		DrawGrid_knihovna->ColCount=2;
//		TCanvas* C=DrawGrid_knihovna->Canvas;
//		int W=DrawGrid_knihovna->DefaultColWidth;
//		int H=DrawGrid_knihovna->DefaultRowHeight;
//		int P=-1*DrawGrid_knihovna->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
//
//		unsigned short obdelnik_okrajX=10;unsigned short obdelnik_okrajY=5;unsigned short okraj_packy=obdelnik_okrajY;
//		C->Font->Style = TFontStyles()<< fsBold;
//		C->Font->Size=12;
//		C->Font->Name="Arial";
//		C->Pen->Width=1;
//		C->Pen->Color=(TColor)RGB(190,190,190);//(TColor)RGB(19,115,169);
//		C->Brush->Color=(TColor)RGB(190,190,190);//(TColor)RGB(19,115,169);
//		C->Font->Color=clWhite;
//		for(unsigned short n=1;n<=pocet_objektu_knihovny;n++)
//		{
//			UnicodeString text=knihovna_objektu[n-1].short_name;
//			//symbol objektu
//			if(VyID!=n-1)
//			{
//				//obdélník
//				C->Rectangle(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+obdelnik_okrajY+P,((n+1)%2+1)*W-obdelnik_okrajX,ceil(n/2.0)*H-obdelnik_okrajY+P);
//				//packy
//				C->MoveTo(((n+1)%2)*W+okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);
//				C->MoveTo(((n+1)%2)*W+W-obdelnik_okrajX,(ceil(n/2.0)-1)*H+H/2+P);C->LineTo(((n+1)%2)*W+W-okraj_packy,(ceil(n/2.0)-1)*H+H/2+P);
//				//písmo
//				C->TextOutW((Rect.Right-Rect.Left-C->TextWidth(text))/2+((n+1)%2)*W,(Rect.Bottom-Rect.Top-C->TextHeight(text))/2+(ceil(n/2.0)-1)*H+P,text);
//			}
//			else//výhybka v bmp
//			{
//				Graphics::TBitmap *bmp=new Graphics::TBitmap();
//				ImageList48->GetBitmap(51,bmp);
//				C->Draw(((n+1)%2)*W+obdelnik_okrajX,(ceil(n/2.0)-1)*H+P,bmp);
//				bmp=NULL;delete bmp;
//			}
//		}
//	}
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
	log(__func__);//logování
	int Col,Row;
	//DrawGrid_knihovna->MouseToCell(X,Y,Col,Row);
	Col=DrawGrid_knihovna->Col; Row=DrawGrid_knihovna->Row;
	if(MOD==EDITACE)
	{           
		ESC();//ukončení případné akce
		if(PmG!=NULL && PmG->Rows[3].Visible)FormX->odstranit_korelaci();//pokud je tabulka pohonu v režimu KK je možnost, že je Highlightovaná ... odstranit highlight (korelaci)
		knihovna_id=1;  
		//pro každý objekt jiná podmínka přidání a jiný způsob přiřazení eID
		switch(OBJEKT_akt->id)
		{
			case 0:case 9://navěšování + svěšování
			{
				if(Row==0)element_id=Col+101;
				if(Row==1)element_id=Col+103;
			}break;
			case 1://CO2
			{
				if(Row==0)element_id=Col+15;
				if(Row==1)element_id=Col+17;
			}break;
			case 2://ožeh
			{
        if(scGPSwitch_robot_clovek->State==0)//robot
				{
					if(Row==0)element_id=Col+11;
					if(Row==1)element_id=Col+13;
				}
				else//člověk
				{
        	if(Row==0)element_id=Col+105;
					if(Row==1)element_id=Col+107;
        }
			}break;
			case 3://POW
			{
				if(Row==0)element_id=-1;
				if(Row==1)element_id=-2;
			}break;
			case 4://objekt ionizace
			{
				if(scGPSwitch_robot_clovek->State==0)//robot
				{
					if(Row==0)element_id=Col+7;
					if(Row==1)element_id=Col+9;
				}
				else//člověk
				{
					if(Row==0)element_id=Col+105;
					if(Row==1)element_id=Col+107;
				}
			}break;
			case 5://objekt lakovna
			{

        if(scGPSwitch_robot_clovek->State==0)//robot
				{
					if(Row==0)element_id=Col+1;
					if(Row==1)element_id=Col+3;
				}
				else//člověk
				{
					if(Row==0)element_id=Col+105;
					if(Row==1)element_id=Col+107;
				}
			}break;
		}
		//kontrola zda mohu vložit
		bool pridani=true;      
		if(Col==0 && OBJEKT_akt->rezim==0 && OBJEKT_akt->id)pridani=false;
		if(Col==1 && OBJEKT_akt->rezim==1 && OBJEKT_akt->id)pridani=false; 
		if(OBJEKT_akt->pohon==NULL)pridani=false;
		//pokud byly splněné podmínky pro přidání, zapne událost ADD (vystrčeno mimo z důvodu množení kódu)
		if(pridani && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
		{    
			SB(ls->Strings[387]);//"Kliknutím na libovolné místo umístíte vybraný element."
			Akce=ADD;kurzor(add_o);
		}
		else//pokud bude při již spuštěné akci kliknuto na element, který být přidanný nesmí, je nutné akci vypnout
		{
			Akce=NIC;kurzor(standard);
			if(OBJEKT_akt->pohon==NULL)TIP=ls->Strings[310];//"Přidávání prvků je možné až po výběru pohonu"
			else TIP=ls->Strings[462];
			refresh_mGrid=false;REFRESH();refresh_mGrid=true;
		}
	}
	else//pro LAYOUT
	{
		SB(ls->Strings[384]+" "+knihovna_objektu[Col+Row+Row].name);//"Kliknutím na libovolné místo umístíte objekt "
		//SB(AnsiString(DrawGrid_knihovna->TopRow)+" "+AnsiString(Col)+" "+AnsiString(Row)+" "+knihovna_objektu[Col+Row+Row].name);
		if(Col>-1 && Row>-1)
		{
			vybrany_objekt=Col+Row+Row;
			Akce=ADD;kurzor(add_o);//Screen->Cursor=crCross;
			add_posledni=true;pom=NULL;
			//ShowMessage(vybrany_objekt);
			if(VyID==vybrany_objekt && d.v.OBJEKTY->predchozi->n<3)//pokud je vybraná vyhýbka nejsou alespoň 3 objekty
			{
				MB(ls->Strings[334]);//"Výhybku lze nastavit, pokud jsou k dispozici minimálně 3 technologické objekty!"
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
	log(__func__);//logování
	ESC();//ukončení případné akce
	if(MOD==EDITACE && PmG!=NULL && PmG->Rows[3].Visible)FormX->odstranit_korelaci();
	int Col,Row;
	Col=DrawGrid_otoce->Col; Row=DrawGrid_otoce->Row;
	knihovna_id=2;
	if(Row==0) element_id=Col+5;
	if(OBJEKT_akt->pohon!=NULL && OBJEKT_akt->id!=3 && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
	{
		SB(ls->Strings[387]);//"Kliknutím na libovolné místo umístíte vybraný element."
		Akce=ADD;kurzor(add_o);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_ostatniMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	log(__func__);//logování
	ESC();//ukončení případné akce
	int Col=DrawGrid_ostatni->Col;
	if(MOD==EDITACE && PmG!=NULL && PmG->Rows[3].Visible)FormX->odstranit_korelaci();
	if(Col==0 && OBJEKT_akt->pohon!=NULL && OBJEKT_akt->id!=3 && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
	{
		knihovna_id=3;
		element_id=0;
		SB(ls->Strings[387]);//"Kliknutím na libovolné místo umístíte vybraný element."
		Akce=ADD;kurzor(add_o);
	}
	if(Col==1 && OBJEKT_akt->pohon!=NULL && OBJEKT_akt->id==4 && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
	{
		knihovna_id=3;
		element_id=100;
		SB(ls->Strings[387]);//"Kliknutím na libovolné místo umístíte vybraný element."
		Akce=ADD;kurzor(add_o);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_geometrieMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	log(__func__);//logování
	if(Akce!=GEOMETRIE)ESC();//ukončení případné akce
	int Col,Row;
	Col=DrawGrid_geometrie->Col;Row=DrawGrid_geometrie->Row;
	knihovna_id=4;
	if(Row==0)
	{
  	if(Akce!=GEOMETRIE && Col==0 && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))//zapnutí akce geometrie
  	{
  		Akce=GEOMETRIE;
  		editace_geometrie_spustena=false;
  		scGPCheckBox1_popisky->Checked=false;//vypnutí zobrazení popisků, v budoucnu rozšířit na uložení předchozího stavu
  		scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
  		scGPCheckBox_zobrazit_pozice->Checked=false;
			scGPCheckBox_zobrazit_palce->Checked=false;
  		scGPCheckBox_rozmisteni_voziku->Checked=false;
  		scGPCheckBox_popisek_pohonu->Checked=false;
  		stisknute_leve_tlacitko_mysi=false;//nutné!!! zustává aktivníc z dblclicku
			typElementu=0;
			d.SCENA=1111111;
      vytvor_statickou_scenu();
			REFRESH();
  	}
  	else {ukonceni_geometrie();}//vypunutí akce geometrie
  	if(Col==1 && OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
  	{
  		element_id=200;
  		SB(ls->Strings[387]);//"Kliknutím na libovolné místo umístíte vybraný element."
			Akce=ADD;kurzor(add_o);
		}
	}
	//stoupání klesání
	else
	{
		element_id=MaxInt;
		if(Col==1)element_id*=-1;
		Akce=S_K;
		kurzor(add_o);
		TIP=ls->Strings[501];//"Kliknutím do oblasti pohonu vložíte začátek."
		d.SCENA=1111111;//ZprVozEledElesDopObjHal - vozíky nutno nechat v dynamické (jinak budou překresleny bufferem
		vytvor_statickou_scenu();//vypnutí vrstvy errorů a nastavení zbytku na statickou scénu
		REFRESH();
	}
}
//---------------------------------------------------------------------------
//přeposílá událost na form
//přeposílá událost na form
void __fastcall TForm1::RzSizePanel_knihovna_objektu_OLDMouseEnter(TObject *Sender)
{
	log(__func__);//logování
//DrawGrid_knihovna->Enabled=true;
	//kvůli navigačním liniim, pokud se v průběhu přidávní bodu dostane uživatel zpět do panelu, tak aby zde nezůstaly "viset" chybné line
	if(add_posledni)d.odznac_oznac_objekt_novy_posledni(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y);
	else if(pom!=NULL)d.odznac_oznac_objekt_novy(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
	probehl_zoom=true;zneplatnit_minulesouradnice();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_knihovnaMouseLeave(TObject *Sender)
{
	log(__func__);//logování
		//DrawGrid_knihovna->Enabled=false;
}
//---------------------------------------------------------------------------
void TForm1::zobraz_tip(UnicodeString text,TCanvas* canv)//prázdným (bez paremetrů) voláním  metody se tip smaže
{
	log(__func__);//logování
	TIP=text;
	if(canv==NULL)REFRESH();//pokud není parametr canvas uveden, jedná se o dlouhodobé vykreslování hodnoty TIP
	else d.vykresli_tip(canv);//pokud je parametrem předán Canvas vykreslí se přímo a jednorázově
}
//---------------------------------------------------------------------------
void TForm1::akutalizace_stavu_prichytavani_vSB()
{
	log(__func__);//logování
	switch(prichytavat_k_mrizce)
	{
		//case 0:SB("přichytávat?",5);grid=true;myMessageBox->CheckBox_pamatovat->Checked=false;break;
		case 1:SB(ls->Strings[375],5);grid=true;break;//přichytí automaticky
		case 2:SB(ls->Strings[388],5);break;//automaticky nepřichyt
	}
  //přepínání barev
	if(prichytavat_k_mrizce==1)
	{
		scGPButton_prichytavat->Options->NormalColor=(TColor)RGB(86,120,173);
		scGPButton_prichytavat->Options->FrameNormalColor=(TColor)RGB(86,120,173);
	}
	else
	{
		scGPButton_prichytavat->Options->NormalColor=scGPButton_prichytavat->Options->HotColor;
		scGPButton_prichytavat->Options->FrameNormalColor=scGPButton_prichytavat->Options->HotColor;
	}
}
//---------------------------------------------------------------------------
//přepínání dotazu, přichytávání, nepřychtávání k mřížce
//přepíná mody zobrazení
void __fastcall TForm1::RzStatusPane1Click(TObject *Sender)
{
//  log(__func__);//logování
//	switch(MOD)
//	{
//		case 0:schemaClick(Sender);break;
//		case 1:testovnkapacity1Click(Sender);break;
//		case 2:casoverezervy1Click(Sender);break;
//		case 3:simulace1Click(Sender);break;
//		case 4:schemaClick(Sender);break;
//	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Zavreni programu volbami Konec - ALT+F4 z menu, odchytáváno včetně všeho dalšího pomocí metoda FormCloseQuery
void __fastcall TForm1::Konec1Click(TObject *Sender)
{
 log(__func__);//logování
 Close();//ukončí aplikaci
}
//---------------------------------------------------------------------------
//Zavreni programu volbou krizek + reaguje na reakci zavirani formulare pri Konec1Click
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	log(__func__);//logování
	Timer_getjobid->Enabled=false;
	Timer_neaktivity->Enabled=false;
	if(duvod_k_ulozeni)
	{
		AnsiString FNs=FileName_short(FileName);
		if(FNs=="")FNs="Nový.tispl";
		int result=MB(FNs+" "+ls->Strings[318],MB_YESNOCANCEL);//" byl změněn.\nChcete ho před ukončením uložit?"
		switch(result)
		{
			case mrYes:     UlozitClick(this); if(!stisknuto_storno){ulozit_posledni_otevreny(); vse_odstranit(); CanClose=true;}else CanClose=false; break;
			case mrNo:      ulozit_posledni_otevreny();vse_odstranit();CanClose=true; break;
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
    //AnimateWindow(Handle,500,0x00080000|0x00010000);//pouze efekt při ukončování asi vyhodit, nesmí být zcela na konci, jinak paměťová chyba
		log(__func__,"CanClose");//logování
		Gdiplus::GdiplusShutdown(gdiplusToken);//vypnutí GDI+, nesmí být ve vse_odstranit, jinak paměťová chyba
		d.v.vymaz_seznam_KATALOG();
		//pro ochranu v případě pádu programu
		writeINI("Konec","status","OK");
		//zápis dalšího nastavení
		writeINI("Nastaveni_app","prichytavat",prichytavat_k_mrizce);
		writeINI("Nastaveni_app","ortogonalizace",(short)ortogonalizace_stav);
		//zatím nepoužíváme writeINI("Nastaveni_app","status",STATUS);
		delete aFont; aFont=NULL;
		delete LogFileStream;//zde to bylo z neznáme příčiny zakomentovanén a nevíme proč, přitom se to tu maže správně (nepatří do vse_odstranit)
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Smazat1Click(TObject *Sender)
{
	log(__func__);//logování
	UnicodeString text,text_1,text_2,text_3,text_4,text_5,text_6;
	text=ls->Strings[335]+" ";//"Chcete opravdu smazat "
	text_1=ls->Strings[336]+" ";//"Chcete opravdu smazat bod č. "
	text_2=ls->Strings[337]+" ";//"Chcete opravdu smazat sekci číslo "
	text_3=ls->Strings[338];//"Nelze smazat sekci, v objektu musí být nejméně dvě sekce!"
	text_4=ls->Strings[339]+" ";//"Nelze smazat objekt, který je součástí technologické cesty zakázky např.: "
	text_5=ls->Strings[340]+" ";//"Chcete opravdu objekt "
	text_6=" "+ls->Strings[341];//" smazat?"
	switch (MOD)
	{
		case EDITACE:       //kvůli MB hází při mazání z popUP paměťovou chybu
		{
      duvod_validovat=2;//validace a generování vozíků při následujícím refreshi
			if(Akce!=GEOMETRIE && pom_element!=NULL && pom_element->eID!=MaxInt)//případ mazání běžného elementu
			{
				AnsiString text_PM=ls->Strings[465];
				if(pom_element->pohon!=NULL)text_PM+=" "+pom_element->pohon->name;
				else text_PM+=" nenastaven";
				if((pom_element->eID!=200 && mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text+pom_element_temp->name.UpperCase()+"?","",MB_YESNO)))// || (pom_element->eID==200 && pom_element->dalsi!=NULL && pom_element->objekt_n==OBJEKT_akt->n && pom_element->dalsi->objekt_n==OBJEKT_akt->n) && mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_PM,"",MB_YESNO))
				{
					int eID=pom_element_temp->eID;
					Cvektory::TPohon *p=pom_element_temp->pohon;//slouží k uchování pohonu pro jeho následnou aktualizaci režimu
					if(eID==300)OBJEKT_akt->pohon=pom_element_temp->pohon;//zajištění si zprávného akt. editovaného pohonu
					Cvektory::TElement *dalsi_element=NULL;
					if(pom_element_temp->dalsi!=NULL && p!=NULL && OBJEKT_akt->rezim==0)dalsi_element=pom_element_temp->dalsi;//pokud je kabina ve S&G režimu a za smazaným elementem se nachází další element, je nutné dalšímu přepočítat RT
					d.v.smaz_element(pom_element_temp);
					d.v.aktualizuj_rezim_pohonu(p);//musí být před aktualizací vykreslení knihoven
					d.v.aktualizace_rezimu_objektu(OBJEKT_akt);//aktualizace režimu, např. z důvodu odstranění poslední KK elementu ... režím objektu jež nemůže být KK
					if(eID==200 || eID==300)vlozit_predavaci_misto_aktualizuj_WT();//bylo odstraněno PM
					Akce=NIC;
		  		DrawGrid_knihovna->Refresh();
					DrawGrid_otoce->Refresh();
					pom_element_temp=NULL; delete pom_element_temp;
					//if(OBJEKT_akt->element!=NULL)d.v.uprav_popisky_elementu(OBJEKT_akt->element);//pokud jsou v kabině jěště nějaké elementy dojde k přejmenování
					pom_element=NULL;//přidáno nově 13.5.2019 - v režimu testování kvůli setJobID a předání do pom_element_puv
					if(eID%2==0 && eID!=100 && eID!=200 && eID!=MaxInt)d.v.aktualizuj_sparovane_ukazatele();//odstraněn stop-element, nutná aktualizace
					dalsi_element=NULL;delete dalsi_element;
					d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teploměry aktualizuje ji, jinak vytvoří default cestu
					p=NULL;delete p;
				}
				else if(pom_element->eID==200)
				{
					bool nulovatPM=false;
					nastav_focus();
					if(FormX->aut_mazani_PM)text=ls->Strings[492]+" ";//"Automaticky smazat"
					//pokud mažu PM
					if(pom_element->objekt_n==OBJEKT_akt->n && pom_element->dalsi!=NULL && pom_element->dalsi->objekt_n==OBJEKT_akt->n && pom_element->pohon!=pom_element->dalsi->pohon && mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_PM,"",MB_YESNO))
					{
						if(pom_element_temp==predchozi_PM)nulovatPM=true;
						d.v.smaz_element(pom_element_temp);
						pom_element_temp=NULL;pom_element=NULL;
						FormX->aktualizace_tab_elementu();//odstraněním může dojít k ovlivnění následujícího PM
						FormX->mazatPM();//kontrola PM zda jsou nějaké k mazání
					}
					//pokud se jedná o PM na konci objektu, které je na rozdílných pohonech, nelze smazat
					else if((pom_element->dalsi!=NULL && pom_element->pohon!=pom_element->dalsi->pohon) || (pom_element->dalsi==NULL && pom_element->pohon!=d.v.ELEMENTY->dalsi->pohon))
						MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,ls->Strings[470],"",MB_OK);//"Nelze provést! Odstranění provedete výběrem totožného pohonu před a za předávacím místem"
					else
					{
						//dotaz zda má být PM odstraněno nebo ponecháno
						if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text+pom_element_temp->name.UpperCase()+"?","",MB_YESNO))
						{
							if(pom_element_temp==predchozi_PM)nulovatPM=true;
							d.v.smaz_element(pom_element_temp);
							pom_element_temp=NULL;pom_element=NULL;
						}
						else vlozit_predavaci_misto_aktualizuj_WT();//dojde k refreshi WT
					}
					//kontrola zda vynulovat předchozí PM
					if(nulovatPM)predchozi_PM=NULL;
					mazani=false;
					Akce=NIC;
					if(!FormX->aut_mazani_PM)d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teploměry aktualizuje ji, jinak vytvoří default cestu
				}
        DuvodUlozit(true);
				nahled_ulozit(true);
			}
      //mazání zarážky s S/K
			else if(pom_element!=NULL && pom_element->geo.HeightDepp!=0)//&& pom_element->eID==MaxInt)//mazání zarážky z popup
			{
				String SK=ls->Strings[490];//stoupání
				if(pom_element->geo.HeightDepp<0)SK=ls->Strings[491];//klesání
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text+SK+"?","",MB_YESNO))
				{
					Cvektory::TElement *E=pom_element->dalsi;
					d.v.smaz_element(pom_element->predchozi);
          d.v.smaz_element(pom_element);
					DuvodUlozit(true);
					nahled_ulozit(true);
					d.v.aktualizuj_cestu_teplomeru();//pokud existuje cesta mezi teploměry aktualizuje ji, jinak vytvoří default cestu
					//aktualizace Z elementům
					Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
					while(E!=NULL)
					{
						if(E->predchozi->n!=0)E->Z=E->predchozi->Z+E->geo.HeightDepp;
						else E->Z=0;
						E=d.v.dalsi_krok(VYHYBKY,E);
					}
					d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
					delete E;E=NULL;
				}
			}
			else if(pom_bod_temp!=NULL)//mazání bodu v obrysu kabiny
			{
				if(OBJEKT_akt->body->predchozi->n>2 && mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_1+AnsiString(pom_bod_temp->n),"",MB_YESNO))
				{
					smaz_bod_haly_objektu(pom_bod_temp);
					pom_bod_temp=NULL;delete pom_bod_temp;
				}
			}
			else if(pom_komora_temp!=NULL && OBJEKT_akt->komora->predchozi->n>2)//případ mazání kabiny
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_2+AnsiString(pom_komora_temp->n)+"?","",MB_YESNO))
				{
					d.v.smaz_komoru(OBJEKT_akt,pom_komora_temp);
					pom_komora_temp=NULL;delete pom_komora_temp;
					DuvodUlozit(true);
				}
				Akce=NIC;
			}
			else if(pom_komora_temp!=NULL)//mazání komory, v objektu už bylo dosaženo minimálního množství komor
			{
				MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_3,"",MB_OK);
				Akce=NIC;
			}
			break;
		}
		default://LAYOUT
		{
			//ať to nemusí znovu hledat beru z pom Cvektory::TObjekt *p=d.v.najdi_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
			if(pom_vyhybka!=NULL && pom_element_temp==NULL && pom_bod_temp==NULL)//pokud byl prvek nalezen
			{
				Cvektory::TZakazka *Z=NULL;//d.v.obsahuje_segment_cesty_objekt(pom_vyhybka);
				Cvektory::TElement *E=pom_vyhybka->element;
        Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
				while(E!=NULL)
				{
					Z=d.v.obsahuje_segment_cesty_element(E);
					if(Z!=NULL)break;
					else E=d.v.dalsi_krok(VYHYBKY,E,pom_vyhybka);
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
				E=NULL;delete E;
				if(Z!=NULL)
					MB(text_4+UnicodeString(Z->name));
				else
				{
					if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_5+pom_vyhybka->name.UpperCase()+text_6,"",MB_YESNO))
					{
						d.v.smaz_objekt(pom_vyhybka);//nalezeny můžeme odstranit odstranit
						pom_vyhybka=NULL;//delete p; nepoužívat delete je to ukazatel na ostra data
						pom=NULL;
						duvod_validovat=2;
						DuvodUlozit(true);
			  	}
				}
			}
			if(pom_bod_temp!=NULL)//mazání bodu haly nebo objektu
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_1+AnsiString(pom_bod_temp->n),"",MB_YESNO))
				{
					smaz_bod_haly_objektu(pom_bod_temp);
					pom_bod_temp=NULL;delete pom_bod_temp;
				}
			}
			if(pom_element_temp!=NULL)//sazání výhybky z layoutu
			{
				d.v.smaz_element(pom_element_temp);
				pom_element_temp=NULL;pom_element=NULL;
				duvod_validovat=2;//validovat při REFRESH() zajistí odmazání errorů
				d.v.aktualizuj_cestu_teplomeru();
			}
			break;
		}
	}
	//přeindexování a změna názvů
	Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
	Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	unsigned int n=1;
	while(E!=NULL)
	{
		E->n=n;
		n++;
		E=d.v.dalsi_krok(VYHYBKY,E);
	}
	d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	delete E;E=NULL;
	//d.v.uprav_popisky_elementu(NULL);
	if(d.SCENA!=0)vytvor_statickou_scenu();//aktualizuje BMP statické scény o nový objekt, musí být před REFRESH, není důvod měnit nastavení d.SCENA
	REFRESH();
	//vytvoření obrazu
	vytvor_obraz();
}
//---------------------------------------------------------------------------
//zobrazí paramety jednoho procesu na časových osách
void __fastcall TForm1::Zobrazitparametry1Click(TObject *Sender)
{
  log(__func__);//logování
//	double prozatim_delka_voziku=m.UDV(0);//doplnit rotaci, bylo zde Form1->d.v.PP.delka_jigu;
	AnsiString rezim="";
	AnsiString delka="v tuto chvíli neznamá";
	AnsiString delka_dop=delka;
//	switch(proces_pom->segment_cesty->objekt->rezim)
//	{
//			case 0:rezim="stop & go";break;
//			case 1:rezim="kontinuální";delka=proces_pom->segment_cesty->RD*proces_pom->segment_cesty->CT;delka_dop=delka;break;
//			case 2:
//				rezim="postprocesní";
//				delka=proces_pom->segment_cesty->objekt->kapacita*prozatim_delka_voziku;
//				delka_dop=proces_pom->segment_cesty->objekt->kapacita_dop*prozatim_delka_voziku;
//			break;
//	}
       //R zakomentováno - odebrání časové osy z projektu
//	Form_osa_info->rHTMLLabel_nazev_vypis->Caption=proces_pom->segment_cesty->objekt->name;
//	Form_osa_info->rHTMLLabel_ct_vypis->Caption=proces_pom->segment_cesty->CT;
//	Form_osa_info->rHTMLLabel_cislo_voziku_vypis->Caption=proces_pom->n_v_zakazce;
//	Form_osa_info->rHTMLLabel_rezim_vypis->Caption=rezim;
//	Form_osa_info->rHTMLLabel_dop_kap_vypis->Caption=proces_pom->segment_cesty->objekt->kapacita_dop;
//	Form_osa_info->rHTMLLabel_pozad_kap_vypis->Caption=proces_pom->segment_cesty->objekt->kapacita;
//	Form_osa_info->rHTMLLabel_rd_vypis->Caption=proces_pom->segment_cesty->RD*60.0;
//	Form_osa_info->rHTMLLabel_zkratka_vypis->Caption=proces_pom->segment_cesty->objekt->short_name;

//	if(proces_pom->segment_cesty->objekt->pohon)
//	{
//		Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption=AnsiString(m.cekani_na_palec(0,proces_pom->segment_cesty->objekt->pohon->roztec,proces_pom->segment_cesty->RD,1))+" [sec]";
//		Form_osa_info->rHTMLLabel_nahodna_doba_cekani_vypis->Caption=AnsiString(proces_pom->Trand)+" [sec] - abs. čas na časové ose";
//		Form_osa_info->rHTMLLabel_max_doba_cekani_vypis->Caption=AnsiString(m.cekani_na_palec(0,proces_pom->segment_cesty->objekt->pohon->roztec,proces_pom->segment_cesty->RD,3))+" [sec]";
//		Form_osa_info->rHTMLLabel_palce_vypis->Caption=AnsiString(proces_pom->segment_cesty->objekt->pohon->roztec) +" [m]";
//	}
//	else
//	{
//		Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption="neznámá - nepřiřazen pohon";
//		Form_osa_info->rHTMLLabel_nahodna_doba_cekani_vypis->Caption=Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption;
//		Form_osa_info->rHTMLLabel_max_doba_cekani_vypis->Caption=Form_osa_info->rHTMLLabel_str_doba_cekani_vypis->Caption;
//		Form_osa_info->rHTMLLabel_palce_vypis->Caption=Form_osa_info->rHTMLLabel_max_doba_cekani_vypis->Caption;
//	}

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

//	Form_osa_info->rHTMLLabel_tpoc_vypis->Caption=Tpoc;
//	Form_osa_info->rHTMLLabel_tkon_vypis->Caption=Tkon;
//	Form_osa_info->rHTMLLabel_tdor_vypis->Caption=Tdor;
//	Form_osa_info->rHTMLLabel_tpre_vypis->Caption=Tpre;
//	Form_osa_info->rHTMLLabel_tcek_vypis->Caption=Tcek;
//
//	Form_osa_info->rHTMLLabel_pt_vypis->Caption=proces_pom->Tkon-proces_pom->Tpoc;
//	Form_osa_info->rHTMLLabel_mt_vypis->Caption=proces_pom->Tpre-proces_pom->Tkon;
//	Form_osa_info->rHTMLLabel_wt_vypis->Caption=proces_pom->Tcek-proces_pom->Tpre;
//	Form_osa_info->rHTMLLabel_CT_n_vypis->Caption=proces_pom->Tcek-proces_pom->Tpoc;

	//pozice
//	Form_osa_info->Left=ClientWidth/2-Form_osa_info->Width/2;
//	Form_osa_info->Top=ClientHeight/2-Form_osa_info->Height/2;
	//samotné volání modálního okna
//	Form_osa_info->ShowModal();

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
void __fastcall TForm1::NastavitparametryClick1Click(TObject *Sender)
{
	log(__func__);
	if(MOD==LAYOUT && pom_bod_temp==NULL){pom=pom_vyhybka;if(pom!=NULL)otevri_editaci();}//předání z pomocného ukazatele objekt do pom, zabrání ztráty pom, pokud uživatel sjede kurzorem mimo objekt při otevřeném popup + ochrana proti prázdnému pom (asi zbytečná)
	if(OBJEKT_akt!=NULL && pom_vyhybka!=NULL && OBJEKT_akt->n!=pom_vyhybka->n && pom_bod_temp==NULL)//otevírání náhledu z náhledu, přechot na editaci jiného objektu
	{
		zmena_editovaneho_objektu();
	}
	if(pom_bod_temp!=NULL && OBJEKT_akt==NULL && pom==NULL)//přidání bodu haly
	{
		if(pom_bod_temp->n!=1)d.v.vloz_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,NULL,pom_bod_temp->predchozi);
		else d.v.vloz_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y);
		REFRESH();
	}
	if(OBJEKT_akt!=NULL && pom_komora_temp!=NULL)//změna typu kabiny
	{
		if(pom_komora_temp->typ==0)pom_komora_temp->typ=1;
		else pom_komora_temp->typ=0;
		refresh_mGrid=false;
		REFRESH();
		refresh_mGrid=true;
		nahled_ulozit(true);
	}
	if(OBJEKT_akt!=NULL && pom_bod_temp!=NULL)//přidání bodu objektu
	{
		vloz_bod_haly_objektu(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
    JID=-1;REFRESH();
	}
	if(Akce==GEOMETRIE)//přidávání zarážky při editaci geometrie
	{
		TPointD bod=bod_vlozeni_elementu();
		if(bod.x!=-1000&&bod.y!=-1000)
		{
			short rotace=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),m.L2Px(bod.x),m.L2Py(bod.y));
			Cvektory::TElement *E=d.v.vloz_element(OBJEKT_akt,MaxInt,bod.x,bod.y,rotace,pom_element_temp);//použito force řazení, tzn. nebude přiřazena geometrie a kontrolováno pořadí
			design_element(E,false);//nutné!!!
			//vložení geometrie
			if(E->predchozi->n>0)d.v.vloz_G_element(E,0,E->predchozi->geo.X4,E->predchozi->geo.Y4,0,0,0,0,E->X,E->Y,E->predchozi->geo.orientace);
			else d.v.vloz_G_element(E,0,E->dalsi->geo.X1,E->dalsi->geo.Y1,0,0,0,0,E->X,E->Y,E->dalsi->geo.orientace);
			d.v.vloz_G_element(E->dalsi,0,E->X,E->Y,0,0,0,0,E->dalsi->geo.X4,E->dalsi->geo.Y4,E->dalsi->geo.orientace);
			E=NULL;delete E;
		}
	}
}
//---------------------------------------------------------------------------
void TForm1::otevri_editaci()
{
	log(__func__);//logování
	log("Otevřeni editace, MOD=EDITACE");
	TIP="";
	d.SCENA=0;//odstavení statické scény
	PmG_VID=0;PmG_VID_value=0;
	Timer_getjobid->Enabled=false;
	if(!scSplitView_LEFTTOOLBAR->Opened)scSplitView_LEFTTOOLBAR->Opened=true;
	nastav_focus();
	if(mGrid_knihovna!=NULL)mGrid_knihovna->Delete();
	delete mGrid_knihovna;mGrid_knihovna=NULL;
	//předesignované tlačítko layout - editace
	Schema->Caption=ls->Strings[36]+" "+pom->name.UpperCase();
	if(pom->uzamknout_nahled)Schema->ImageIndex=79;
	else Schema->ImageIndex=-1;
	//kontrola zda má dojít k editaci sekundární větve
	bool editace_vyhybky=false;
	Cvektory::TElement *edit_vyhybka=NULL;
	if(JID==6 && pom_element!=NULL && pom_element->dalsi2->eID==301)//pouze pokud nené nastavená geometrie
	{
		 editace_vyhybky=true;
		 edit_vyhybka=pom_element;
	}

	//zobrazení knihovny pokud je skrytá
	//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
	pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
	scGPButton_prichytavat->Visible=false;//vypnutí tlačítka přichytávat
	scButton_zamek_layoutu->Visible=false;//vypnutí tlačítka pro zámek layoutu
	design_statusbar();//změní pozici zarážek ve statusbaru
	Image_knihovna_objektu->Visible=false;//vypnutí komponenty s knihovnou
	JID=-1;//ošetření, s JID se pracuje i v náhledu
	element_id=99999;//ošetření pro správné zobrazování mgridů
	pom_bod=NULL;pom_bod_temp=NULL;//s těmito ukazateli pracuje jak náhled tak schéma, ošetření
	//zablokování OnChange tabulek
	FormX->input_state=FormX->NO;
	FormX->vstoupeno_poh=false;
	FormX->vstoupeno_elm=false;
	MOD=EDITACE;
	OBJEKT_akt=pom;//podle nového DM - ostrý ukazatel na originál
	nastav_focus();
	popisky_knihovna_nahled(false);//nastavní popisků pro editaci
	DrawGrid_knihovna->Top=33;
	kurzor(standard);
	vytvor_obraz(true);//vytvoření obrazu pro storno a UNDO
	vytvor_seznam_pouzivanych_pohonu();
	////řešení nového zoomu a posunu obrazu pro účely náhldeu
	//zazálohování hodnot posunu a zoomu
	Posun_predchozi2=Posun_predchozi=Posun;
	Zoom_predchozi2=Zoom_predchozi=Zoom;
	JID=-1;
	//DrawGrid_knihovna->Visible=false; //nezobrazí přepozicování elementů
	DrawGrid_knihovna->DefaultRowHeight=140;
	if(OBJEKT_akt->id==3)DrawGrid_knihovna->DefaultColWidth=160;
	else DrawGrid_knihovna->DefaultColWidth=80;
	DrawGrid_knihovna->Left=3;
	DrawGrid_knihovna->Height=DrawGrid_knihovna->DefaultRowHeight*2; // dle počtu řádků
	mazani=false;//pomocná proměnná pro strono / uložit problém (uložit volá storno, nutno rozlišit zda mažu nebo ukládám)

	//objekt ionizace
	if(OBJEKT_akt->id==4 || OBJEKT_akt->id==2 || OBJEKT_akt->id==5){scGPPanel_pomocn_proSwitch->Visible=true;scGPSwitch_robot_clovek->Visible=true;}

	DrawGrid_otoce->DefaultColWidth=80;

	scListGroupPanel_hlavickaOtoce->Visible=true;
	scListGroupPanel_hlavickaOtoce->Top=DrawGrid_knihovna->Height + scGPPanel_mainmenu->Height;
	scGPLabel_otoce->Top = scListGroupPanel_hlavickaOtoce->Top;//  + scGPPanel_mainmenu->Height;//scListGroupPanel_hlavickaOtoce->Top + scGPPanel_mainmenu->Height;
	DrawGrid_otoce->Visible=true;

	scListGroupPanel_hlavickaOstatni->Visible=true;
	scListGroupPanel_hlavickaOstatni->Top=scListGroupPanel_hlavickaOtoce->Top + scListGroupPanel_hlavickaOtoce->Height;
	scGPLabel_stop->Top=scListGroupPanel_hlavickaOstatni->Top -1;// + scGPPanel_mainmenu->Height-1;//MV přidáno z důvodu zobrazování čar v knihovně
	DrawGrid_ostatni->Visible=true;

	scListGroupPanel_geometrie->Visible=true;
	scListGroupPanel_geometrie->Top=scListGroupPanel_hlavickaOstatni->Top +   scListGroupPanel_hlavickaOstatni->Height;
	scGPLabel_geometrie->Top=scListGroupPanel_geometrie->Top-1;// + scGPPanel_mainmenu->Height-1;//MV přidáno z důvodu zobrazování čar v knihovně
	DrawGrid_geometrie->Visible=true;

	scListGroupPanel_poznamky->Visible=true;
	scListGroupPanel_poznamky->Top= scListGroupPanel_geometrie->Top + scListGroupPanel_geometrie->Height;
	scGPLabel_poznamky->Top=scListGroupPanel_poznamky->Top-1;// + scGPPanel_mainmenu->Height-1;//MV přidáno z důvodu zobrazování čar v knihovně
	DrawGrid_poznamky->Visible=true;

	if(OBJEKT_akt->id==4 || OBJEKT_akt->id==2 || OBJEKT_akt->id==5)scGPLabel_roboti->Caption=ls->Strings[55];//"Robot           Operátor";//mezery tvoří místo, kde je zobrazen switch
	else if(OBJEKT_akt->id==3)scGPLabel_roboti->Caption=ls->Strings[56];//"Sekce";
	else if(OBJEKT_akt->id==0 || OBJEKT_akt->id==9)scGPLabel_roboti->Caption=ls->Strings[57];//"Operátoři";
	else scGPLabel_roboti->Caption=ls->Strings[58];//"Roboti";

	scGPLabel_roboti->ContentMarginLeft=10;

	//nastavení tlačítek na výchozí hodnoty
	if(OBJEKT_akt->uzamknout_nahled)
	{
		scButton_zamek->ImageIndex=37; //zamčeno
		//Schema->ImageIndex=79;
		scButton_zamek->Hint=ls->Strings[119];//"Odemknout editaci";
	}
	else
	{
		scButton_zamek->ImageIndex=60;
		//Schema->ImageIndex=78;
		scButton_zamek->Hint=ls->Strings[120];//"Zamknout editaci";
	}
	if(OBJEKT_akt->zobrazit_mGrid)
	{
		scGPButton_viditelnostmGrid->ImageIndex=54;
		scGPButton_viditelnostmGrid->Hint=ls->Strings[121];//"Skrýt tabulky";
	}
	else
	{
		scGPButton_viditelnostmGrid->ImageIndex=55;
		scGPButton_viditelnostmGrid->Hint=ls->Strings[122];//"Zobrazit tabulky";
	}
	if(OBJEKT_akt->zobrazit_koty)
	{
		scGPButton_viditelnostKoty->ImageIndex=56;
		scGPButton_viditelnostKoty->Hint=ls->Strings[123];//"Skrýt kóty";
	}
	else
	{
		scGPButton_viditelnostKoty->ImageIndex=57;
		scGPButton_viditelnostKoty->Hint=ls->Strings[124];//"Zobrazit kóty";
	}
	if(posun_dalsich_elementu)
	{
		scGPButton_posun_dalsich_elementu->ImageIndex=58;
		scGPButton_posun_dalsich_elementu->Hint=ls->Strings[125];//"Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";
	}
	else
	{
		scGPButton_posun_dalsich_elementu->ImageIndex=59;
		scGPButton_posun_dalsich_elementu->Hint=ls->Strings[126];//"Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";
	}
	scGPButton_geometrie->Hint=ls->Strings[443];
	if(scGPComboBox_prepinacKot->ItemIndex==0)DKunit=(TForm1::Tm_mm)1;
	else DKunit=(TForm1::Tm_mm)2;
	if(OBJEKT_akt->pohon!=NULL)scGPComboBox_prepinacKot->Enabled=true;
	else scGPComboBox_prepinacKot->Enabled=false;


	scGPButton_ulozit->Enabled=false;
	//zapnutí spodního panelu
	zapnuti_vypnuti_panelEditace(true);

	nahled_ulozen=false;//nově otevřen, není uložen

	//znovu provedení designu při otevření náhledu, který není prázdný
	bool poh_tab=false;
	if(OBJEKT_akt->element!=NULL)
	{
		Cvektory::TElement *E=OBJEKT_akt->element;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		while (E!=NULL && E->objekt_n==OBJEKT_akt->n)
		{
			if(E->n>0)
			{
				//znovuvytvoření mGridů, podle nového DM nutné!!!
				E->mGrid=new TmGrid(F);
				E->mGrid->Tag=6;//ID formu
				E->mGrid->ID=E->n;
				design_element(E,false);//znovuvytvoření tabulek
				if(E->sparovany!=NULL && E->sparovany->objekt_n==OBJEKT_akt->n)E->sparovany=d.v.vrat_element(OBJEKT_akt,E->sparovany->n);//atualizace ukazatelů
				if(d.v.vrat_druh_elementu(E)==0)//pokud použíju design_element(E,false), false = načtení již vytvořené, nepočítají se data
				{
					d.v.reserve_time(E);//aktualizace RT, v případě, že došlo ke změně přejezdu
          FormX->input_state=FormX->Tinput_state::NO;
          if(E->eID!=0)FormX->validace_PT(E);//validace PT podle RT, u S&G elementů (kromě stopky)
				}
				if(E->eID==200 || E->eID==300)poh_tab=true;//pohonová tabulka v editaci bude exitovat
			}
			E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		E=NULL;delete E;
	}
	//vytvoření tabulky pro teploměr
	vytvor_aktualizuj_tab_teplomeru();
	//hledání a zobrazení tabulky předchozího PM
	if(OBJEKT_akt->element->predchozi->n>0)predchozi_PM=d.v.najdi_posledni_element_podle_eID(200,d.v.vrat_objekt(OBJEKT_akt->element->predchozi->objekt_n));
	if(predchozi_PM!=NULL)
	{
		predchozi_PM->mGrid=new TmGrid(F);
		predchozi_PM->mGrid->Tag=6;//ID formu
		predchozi_PM->mGrid->ID=predchozi_PM->n;
		design_element(predchozi_PM,true);//znovuvytvoření tabulek
		poh_tab=true;//pohonová tabulka v editaci bude exitovat
	}
	//vytovření tab pohonu, pokud je třeba
	vytvoreni_tab_pohon(poh_tab);
	////nastavení zoomu
	if(Zoom<=5)//pokud je zoom větší nědělat nic
	{
		//deklarace
		TRect oblast=vrat_max_oblast(OBJEKT_akt);
		int MaxX=oblast.right,MaxY=oblast.top,MinX=oblast.left,MinY=oblast.bottom;
		int PD_x=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
		int PD_y=ClientHeight-scGPPanel_statusbar->Height-scGPPanel_mainmenu->Height-scGPPanel_bottomtoolbar->Height;
		TPointD centr;
		centr.x=m.P2Lx((MaxX+MinX)/2.0);
		centr.y=m.P2Ly((MaxY+MinY)/2.0);

		//výpočet nového Zoomu
		double Z1=abs(Zoom*PD_x/(MaxX-MinX)),Z2=abs(Zoom*PD_y/abs(MaxY-MinY));
		Z1-=fmod(Z1,0.5);Z2-=fmod(Z2,0.5);
		if(Z1>Z2)Zoom=Z2;else Zoom=Z1;
		if(Zoom<0.5)Zoom=0.5;if(Zoom>10)Zoom=10;
		if(Zoom>5)Zoom=5;//default přiblížení pro editaci
		if(Zoom<=2.5)Zoom=3;//při zoomu 250% nebo menším se nezobrazí mGridy, proto nastavit zoom na 300%

		//vycentrování obrazu
		PD_x=PD_x/2+scSplitView_LEFTTOOLBAR->Width;
		PD_y=PD_y/2+scGPPanel_mainmenu->Height;
		Posun.x+=(m.L2Px(centr.x)-PD_x)/Zoom;
		Posun.y+=(m.L2Py(centr.y)-PD_y)/Zoom;
	}

	zneplatnit_minulesouradnice();

	//ostatni
	scGPSwitch1->Enabled=false;//zakázání přepínáná jazyka během editace
	mGrid_on_mGrid();
	pom_element_temp=OBJEKT_akt->element;//pro pořeby editace geometrie
	on_change_zoom_change_scGPTrackBar();//musí být po design_element
	FormX->input_state=FormX->NOTHING;
	//zapnutí editace pokud je třeba
	if(editace_vyhybky)
	{
		posledni_editovany_element=edit_vyhybka;
		Akce=GEOMETRIE;
		editace_geometrie_spustena=false;
		scGPCheckBox1_popisky->Checked=false;//vypnutí zobrazení popisků, v budoucnu rozšířit na uložení předchozího stavu
		scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
		scGPCheckBox_zobrazit_pozice->Checked=false;
		scGPCheckBox_zobrazit_palce->Checked=false;
		scGPCheckBox_rozmisteni_voziku->Checked=false;
		scGPCheckBox_popisek_pohonu->Checked=false;
		stisknute_leve_tlacitko_mysi=false;//nutné!!! zustává aktivníc z dblclicku
		typElementu=0;
    d.SCENA=1111111;
		vytvor_statickou_scenu();
	}
	else d.SCENA=0;
//	else//normální funkčnost
//	{
//		if(OBJEKT_akt->pohon==NULL && d.v.POHONY->dalsi!=NULL && PmG!=NULL)//otevření COMBA pokud objekt nemá žádný pohon a pokud existují nějaké pohony
//		{
//			TscGPComboBox *C=PmG->getCombo(3,0);
//			if(C!=NULL)
//			{
//				C->DropDown();
//				FormX->vstoupeno_poh=true;
//			}
//			C=NULL;delete C;
//    }
//	}
	//d.SCENA=0, prozatím statická scéna funkční pouze v layoutu, pro editaci se nepoužívá
	REFRESH(true);//přidáno kvůli zobrazení tab. pohonů a kót (při shodném zoomu layout->editace)
	edit_vyhybka=NULL;delete edit_vyhybka;
}
//---------------------------------------------------------------------------
//slouží k přechodu z editace jednoho objektu do editace druhého objektu
void TForm1::zmena_editovaneho_objektu()
{
	log(__func__);//logování
	log("Otevřeni editace, MOD=EDITACE");
	Timer_neaktivity->Enabled=false;//vypnutí timeru pro jistotu
	Timer_getjobid->Enabled=false;
	nastav_focus();
	/////////Uložení náhledu
	bool prepnout=true,validace=false,storno=false;
	bool kontrola_PM=false;//při neuložení minulého náhledmu musí dojík k separátní kontrole
	int result=mrNo;
	if(scGPButton_ulozit->Enabled)result=MB(ls->Strings[343]+" "+OBJEKT_akt->name+"?",MB_YESNO,true);//"Chcete uložit změny objektu"                                                     //"Zadaná data nejsou validní a nebudou uložena. Opravdu přejít do jiného objektu?"
	else if(FormX->validovany_pohon!=0){result=MB(ls->Strings[344],MB_YESNO,true);validace=true;}
	if(validace && result==mrNo)result=33;if(validace && result==mrYes)result=mrNo;
	switch(result)
	{
		case mrYes://nově se při ukládání nic neděje
		{
			byly_pohony_editovany();//kontrola, zda došlo k editaci a následná aktualizace elementů na tom pohonu
			vymaz_seznam_obrazu();//vymazání nepotřebných obrazů
			DuvodUlozit(true);
			nahled_ulozit(false);
			pripnuti_dalsich_objektu();
		}break;
		case mrNo:
		{
			storno=true;
			////kontrola zda je nutné přemazat obraz
			if(!FormX->existuje_validace() && !scGPButton_ulozit->Enabled)
			{
				vymaz_seznam_obrazu();//vymazání nepotřebných obrazů
				d.v.update_akt_zakazky();
				storno=false;
	  	}
		}break;//při storno bude načten poslední obraz projektu
		default:prepnout=false;Akce=BLOK;break;
	}
	smaz_seznam_pouzivanych_pohonu();
	////////Můžu pokračovat?
	if(prepnout)
	{
		/////////Vrácení knihoven a jejich popisků na default pozice
		if(pom->id==3 ||pom->id>5 && pom->id!=9 && (pom_vyhybka->id>=0 && pom_vyhybka->id<=5 || pom_vyhybka->id==3 || pom_vyhybka->id==9))//pokud jdu z objektu, který nemá roboty do objektu, který je má
  	{
  		scListGroupPanel_hlavickaOtoce->Top=314;
  		scListGroupPanel_hlavickaOstatni->Top=404;
			scListGroupPanel_geometrie->Top=494;
			scListGroupPanel_poznamky->Top=641;//584;
  		scGPLabel_otoce->Top=314;
  		scGPLabel_stop->Top=403;
  		scGPLabel_geometrie->Top=493;
			scGPLabel_poznamky->Top=641;//584;
  	}
		/////////Uzavření starého náhledu
  	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//také volá Refresh//smaz_kurzor se zavolá, pouze pokud je to třeba odstraňuje zbytečný problik, dodělal MaKr
  	MOD=LAYOUT;//nutné před zoom, ale za smaz kurzor
  	//mazání mGridů
		Cvektory::TElement *E=OBJEKT_akt->element;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
  	while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
  	{
  		E->mGrid->Delete();
  		E->mGrid=NULL;
			E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
  	if(predchozi_PM!=NULL)
  	{
			predchozi_PM->mGrid->Delete();
			predchozi_PM->mGrid=NULL;
			predchozi_PM=NULL;
		}
		if(OBJEKT_akt->teplomery!=NULL)
		{
			Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
			if(T!=NULL && T->posledni->mGrid!=NULL)//kontrola zda existuje cesta pro akt zakázku
			{
				T->posledni->mGrid->Delete();
				T->posledni->mGrid=NULL;
			}
			T=NULL;delete T;
		}
		E=NULL;delete E;
  	//if(!mazani&&scGPButton_ulozit->Enabled)d.v.uprav_popisky_elementu(NULL);//volání přejmenování elementů, pouze v případě kdy je něco v kabině a bylo stisknuto pouze storno, při ulož je stisk strona volán taky
  	pom=NULL;//pom->pohon=NULL;delete pom->pohon;pom=NULL; toto nelze, odpřiřadilo by to pohon i na ostrém
  	OBJEKT_akt=NULL;delete OBJEKT_akt;
		if(PmG!=NULL)
		{
      nastav_focus();
			PmG->Delete();
			PmG=NULL;delete PmG;
		}
  	//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
  	pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
		//v případě animace vypnutí a nastavení do výchozího stavu
		Timer_simulace->Enabled=false;
  	zobrazit_meritko=scGPSwitch_meritko->State;//navrácení do původního stavu
		//storno funkcionalita
		unsigned long objekt_n=pom_vyhybka->n;//uchovávání, pokud bude stisknuto storno dojde ke smazání objektu
		if(storno)d.v.nacti_z_obrazu_DATA(true);//až po uzavření staré editace
		/////////Přenastavení editovaného objektu
		pom=d.v.vrat_objekt(objekt_n);//pom_vyhybka slouží k uložení ukazatele na pro další náhled

		/////////Otevření nového náhledu
		TIP="";
    PmG_VID=0;PmG_VID_value=0;
		//zobrazení knihovny pokud je skrytá, spíš pro jistotu
	  if(!scSplitView_LEFTTOOLBAR->Opened)scSplitView_LEFTTOOLBAR->Opened=true;
  	//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
  	pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
		nastav_focus();
		JID=-1;//ošetření, s JID se pracuje i v náhledu
  	element_id=99999;//ošetření pro správné zobrazování mgridů
  	pom_bod=NULL;pom_bod_temp=NULL;//s těmito ukazateli pracuje jak náhled tak schéma, ošetření
  	kurzor(standard);
  	//zablokování OnChange tabulek
  	FormX->input_state=FormX->NO;
  	FormX->vstoupeno_poh=false;
  	FormX->vstoupeno_elm=false;
		MOD=EDITACE;
		mazani=false;//pomocná proměnná pro strono / uložit problém (uložit volá storno, nutno rozlišit zda mažu nebo ukládám)
		OBJEKT_akt=pom;//ostrý ukazatel, nové pojetí po změně DM
		//vymazání kroků z layoutu, musí být po nastavení OBJEKT_aktu!!!!!!!!!!!!!
		vytvor_obraz(true);//vytvoření obrazu pro storno + UNDO
		vytvor_seznam_pouzivanych_pohonu();
		//nastavení zoomu na vhodný náhled
  	if(Zoom<=5.0)Zoom=5.0;//ponechání zoomu pokud je vetší jak 5
  	probehl_zoom=true;
  	if(OBJEKT_akt->id==4 || OBJEKT_akt->id==2 || OBJEKT_akt->id==5){scGPPanel_pomocn_proSwitch->Visible=true;scGPSwitch_robot_clovek->Visible=true;}
  	else {scGPPanel_pomocn_proSwitch->Visible=false;scGPSwitch_robot_clovek->Visible=false;}
		if(OBJEKT_akt->id==4 || OBJEKT_akt->id==2 || OBJEKT_akt->id==5)scGPLabel_roboti->Caption=ls->Strings[55];//"Robot           Operátor";//mezery tvoří místo, kde je zobrazen switch
		else if(OBJEKT_akt->id==3)scGPLabel_roboti->Caption=ls->Strings[56];//"Sekce";
		else if(OBJEKT_akt->id==0 || OBJEKT_akt->id==9)scGPLabel_roboti->Caption=ls->Strings[57];//"Operátoři";
		else scGPLabel_roboti->Caption=ls->Strings[58];//"Roboti";
		//nastavení tlačítek na výchozí hodnoty
		if(OBJEKT_akt->uzamknout_nahled)
		{
			scButton_zamek->ImageIndex=37; //zamčeno
			scButton_zamek->Hint=ls->Strings[119];//"Odemknout editaci";
		}
		else
		{
			scButton_zamek->ImageIndex=60;
			scButton_zamek->Hint=ls->Strings[120];//"Zamknout editaci";
		}
		if(OBJEKT_akt->zobrazit_mGrid)
		{
			scGPButton_viditelnostmGrid->ImageIndex=54;
			scGPButton_viditelnostmGrid->Hint=ls->Strings[121];//"Skrýt tabulky";
		}
		else
		{
			scGPButton_viditelnostmGrid->ImageIndex=55;
			scGPButton_viditelnostmGrid->Hint=ls->Strings[122];//"Zobrazit tabulky";
		}
		if(OBJEKT_akt->zobrazit_koty)
		{
			scGPButton_viditelnostKoty->ImageIndex=56;
			scGPButton_viditelnostKoty->Hint=ls->Strings[123];//"Skrýt kóty";
		}
		else
		{
			scGPButton_viditelnostKoty->ImageIndex=57;
			scGPButton_viditelnostKoty->Hint=ls->Strings[124];//"Zobrazit kóty";
		}
		if(posun_dalsich_elementu)
		{
			scGPButton_posun_dalsich_elementu->ImageIndex=58;
			scGPButton_posun_dalsich_elementu->Hint=ls->Strings[125];//"Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";
		}
		else
		{
			scGPButton_posun_dalsich_elementu->ImageIndex=59;
			scGPButton_posun_dalsich_elementu->Hint=ls->Strings[126];//"Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";
		}
		scGPButton_geometrie->Hint=ls->Strings[443];
		if(scGPComboBox_prepinacKot->ItemIndex==0)DKunit=(TForm1::Tm_mm)1;
		else DKunit=(TForm1::Tm_mm)2;
		if(OBJEKT_akt->pohon!=NULL)scGPComboBox_prepinacKot->Enabled=true;
		else {scGPComboBox_prepinacKot->Enabled=false;DKunit=(TForm1::Tm_mm)1;scGPComboBox_prepinacKot->ItemIndex=0;}
		//nastavení záložky
		Schema->Caption=ls->Strings[36]+" "+pom->name.UpperCase();
		if(OBJEKT_akt->uzamknout_nahled)Schema->ImageIndex=79;
	  else Schema->ImageIndex=-1;
		//nutné při změně typu objektu, pouze tyto 2 knihovny se mění se změnou objektu
		DrawGrid_geometrie->Refresh();
		DrawGrid_knihovna->Refresh();
		nahled_ulozen=false;//nově otevřen, není uložen
		//znovu provedení designu při otevření náhledu, který není prázdný
		bool poh_tab_existuje=false;
		if(OBJEKT_akt->element!=NULL)
		{
			Cvektory::TElement *E=OBJEKT_akt->element;
      Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
			while (E!=NULL && E->objekt_n==OBJEKT_akt->n)
			{
				if(E->n>0)
				{
					//znovuvytvoření mGridů, podle nového DM nutné!!!
					E->mGrid=new TmGrid(F);
					E->mGrid->Tag=6;//ID formu
					E->mGrid->ID=E->n;
					design_element(E,true);
					if(E->sparovany!=NULL && E->sparovany->objekt_n==OBJEKT_akt->n)E->sparovany=d.v.vrat_element(OBJEKT_akt,E->sparovany->n);//atualizace ukazatelů
					if(d.v.vrat_druh_elementu(E)==0)//pokud použíju design_element(E,false), false = načtení již vytvořené, nepočítají se data
					{
						d.v.reserve_time(E);//aktualizace RT, v případě, že došlo ke změně přejezdu
            FormX->input_state=FormX->Tinput_state::NO;
						if(E->eID!=0)FormX->validace_PT(E);//validace PT podle RT, u S&G elementů (kromě stopky)
					}
					if(E->eID==200 || E->eID==300)poh_tab_existuje=true;
				}
				E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
			}
			d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
			E=NULL;delete E;
		}
    //vytvoření tabulky pro teploměr
		vytvor_aktualizuj_tab_teplomeru();
		//zjišťování předchozího PM
		predchozi_PM=NULL;
		if(OBJEKT_akt->element->predchozi->n>0)predchozi_PM=d.v.najdi_posledni_element_podle_eID(200,d.v.vrat_objekt(OBJEKT_akt->element->predchozi->objekt_n));
		if(predchozi_PM!=NULL)
		{
			predchozi_PM->mGrid=new TmGrid(F);
			predchozi_PM->mGrid->Tag=6;//ID formu
			predchozi_PM->mGrid->ID=predchozi_PM->n;
			design_element(predchozi_PM,true);//znovuvytvoření tabulek
			poh_tab_existuje=true;
		}
		//vytvoření tab pohonu pokud je třeba
		vytvoreni_tab_pohon(poh_tab_existuje);
		////nastavení zoomu
		//deklarace
		TRect oblast=vrat_max_oblast(OBJEKT_akt);
		int MaxX=oblast.right,MaxY=oblast.top,MinX=oblast.left,MinY=oblast.bottom;
		int PD_x=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
		int PD_y=ClientHeight-scGPPanel_statusbar->Height-scGPPanel_mainmenu->Height-scGPPanel_bottomtoolbar->Height;
		TPointD centr;
		centr.x=m.P2Lx((MaxX+MinX)/2.0);
		centr.y=m.P2Ly((MaxY+MinY)/2.0);
		//výpočet nového Zoomu
		double Z1=abs(Zoom*PD_x/(MaxX-MinX)),Z2=abs(Zoom*PD_y/abs(MaxY-MinY));
		Z1-=fmod(Z1,0.5);Z2-=fmod(Z2,0.5);
		if(Z1>Z2)Zoom=Z2;else Zoom=Z1;
		if(Zoom<0.5)Zoom=0.5;if(Zoom>10)Zoom=10;
		if(Zoom>5)Zoom=5;//default přiblížení pro editaci
    if(Zoom<=2.5)Zoom=3;//při zoomu 250% nebo menším se nezobrazí mGridy, proto nastavit zoom na 300%
		//vycentrování obrazu
		PD_x=PD_x/2+scSplitView_LEFTTOOLBAR->Width;
		PD_y=PD_y/2+scGPPanel_mainmenu->Height;
		Posun.x+=(m.L2Px(centr.x)-PD_x)/Zoom;
		Posun.y+=(m.L2Py(centr.y)-PD_y)/Zoom;

		//ostatní
		mGrid_on_mGrid();
		pom_element_temp=OBJEKT_akt->element;//pro pořeby editace geometrie
		on_change_zoom_change_scGPTrackBar();//musí být po design_element
		FormX->input_state=FormX->NOTHING;
		scGPButton_ulozit->Enabled=false;//otevření náhledu, zde není co ukládat
		if(kontrola_PM)vlozit_predavaci_misto_aktualizuj_WT();
		Akce=BLOK;//blokace spouštění mousedown po této metodě, bez blokace dojde k spuštění akce pan
		REFRESH();//musí být z důvodu změny vykreslení
//		if(OBJEKT_akt->pohon==NULL && d.v.POHONY->dalsi!=NULL && PmG!=NULL)//otevření COMBA pokud objekt nemá žádný pohon a pokud existují nějaké pohony
//		{
//			TscGPComboBox *C=PmG->getCombo(3,0);
//			if(C!=NULL)
//			{
//				C->DropDown();
//				FormX->vstoupeno_poh=true;
//			}
//			C=NULL;delete C;
//		}
	}
}
//---------------------------------------------------------------------------
//vypne editaci bez překleslení a datových věcí
void TForm1::vypni_editaci()
{
	log(__func__);//logování
	nastav_focus();
	MOD=LAYOUT;//nutné před zoom, ale za smaz kurzor
	//mazání mGridů
	Cvektory::TElement *E=OBJEKT_akt->element;
	Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
	{
		E->mGrid->Delete();
		E->mGrid=NULL;
		E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
	}
	d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	if(predchozi_PM!=NULL)
	{
		predchozi_PM->mGrid->Delete();
		predchozi_PM->mGrid=NULL;
		predchozi_PM=NULL;
	}
	E=NULL;delete E;
	pom=NULL;//pom->pohon=NULL;delete pom->pohon;pom=NULL; toto nelze, odpřiřadilo by to pohon i na ostrém
	OBJEKT_akt=NULL;delete OBJEKT_akt;
	if(PmG!=NULL)
	{
    nastav_focus();
		PmG->Delete();
		PmG=NULL;delete PmG;
	}
	//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
	pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
	//vypnutí spodního panelu
	zapnuti_vypnuti_panelEditace(false);
	//scGPPanel_bottomtoolbar->Visible=false;
	//navrácení zoomu a posunu do původních hodnt
	Zoom=Zoom_predchozi2;
	//on_change_zoom_change_scGPTrackBar();//pozor asi volá refresh
	Posun.x=Posun_predchozi2.x;
	Posun.y=Posun_predchozi2.y;

	//předesignované tlačítko layout - editace
	Schema->Caption=ls->Strings[20];
	if(d.v.PP.zamek_layoutu)Schema->ImageIndex=79;
	else Schema->ImageIndex=78;
	//////návrat knihoven a popisku na default místa, zbránění zobrazení knihoven na špatném místě při znovu otevření náhledu
	scListGroupPanel_hlavickaOtoce->Top=314;
	scListGroupPanel_hlavickaOstatni->Top=404;
	scListGroupPanel_geometrie->Top=494;
	scListGroupPanel_poznamky->Top=641;//584;
	DrawGrid_knihovna->DefaultRowHeight=50;
	DrawGrid_knihovna->DefaultColWidth=70;
	DrawGrid_knihovna->Left=14;
	scListGroupPanel_hlavickaOtoce->Visible=false;
	scListGroupPanel_hlavickaOstatni->Visible=false;
	scListGroupPanel_geometrie->Visible=false;
	scListGroupPanel_poznamky->Visible=false;
	DrawGrid_knihovna->Height=400;
	scListGroupKnihovObjektu->Align=alLeft;
	//DrawGrid_knihovna->Invalidate();
	scGPSwitch_robot_clovek->Visible=false;
	scGPPanel_pomocn_proSwitch->Visible=false;
	scGPButton_zmerit_vzdalenost->Visible=false;//schování měření vzdálenosti, pro správné řazení
	scGPComboBox_prepinacKot->ItemIndex=0;//ošetření pokud bylo při vypínání editace nastaveno na časové kóty
	scButton_zamek_layoutu->Visible=true;//zapnutí tlačítka zámek layoutu
	Image_knihovna_objektu->Visible=true;//zapnutí knihovny
	scGPButton_zmerit_vzdalenost->Visible=true;
	scGPButton_prichytavat->Visible=true;//zapnutí tlačítka přichytávat
	vytvoreni_tab_knihovna();
}
//---------------------------------------------------------------------------
//podle zprávy provede posun na daný elment
void TForm1::posun_na_element(unsigned long n_zpravy)
{
	log(__func__);//logování
	//////získání elementu na který má být proveden posun
	Cvektory::TZprava *Z=d.v.vrat_zpravu(n_zpravy);
	if(Z!=NULL && Z->Element!=NULL)
	{
		//////deklarace + nastavení vykreslení zpráv
		Cvektory::TElement *E=Z->Element;
		if(Z->n!=(unsigned)d.zobrazit_celou_zpravu)d.zobrazit_celou_zpravu=Z->n;//zobrazení celé zprávy, před refresh

		//////posun na element + zoom
    posun_na_element(E);

		//////pokud jsem v editaci a editovaný objekt není totožný s objektem ve kterém je element
		if(OBJEKT_akt!=NULL && OBJEKT_akt->n!=E->objekt_n)
		{
			pom_vyhybka=d.v.vrat_objekt(E->objekt_n);//přepnutí editovaného objektu
			zmena_editovaneho_objektu();//provede REFRESH
		}

		//////zapnutí editace, pokud jsem v layoutu
		if(OBJEKT_akt==NULL)
		{
			pom=d.v.vrat_objekt(E->objekt_n);
			otevri_editaci();//provede REFRESH
    }

		//////ostatní
		E=NULL;delete E;
	}
	if(Z!=NULL && Z->Element==NULL)//pokud neobsahuje zpráva ukazatel na Element
	{
		//nastavení zoomu na vhodný náhled
		//if(Zoom<=4 && OBJEKT_akt!=NULL)
		Zoom=10.0;
		zneplatnit_minulesouradnice();
		TPoint Centr;Centr.x=Z->X;Centr.y=Z->Y;
		//vycentrování obrazu na střed
		Posun.x=Centr.x/m2px-ClientWidth/2/Zoom;
		Posun.y=-Centr.y/m2px-(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2/Zoom;
		vytvor_statickou_scenu();//načtení statických záležitostí do statické scény, musí být před REFRESH
		REFRESH();
	}
	Z=NULL;delete Z;
}
//---------------------------------------------------------------------------
void TForm1::posun_na_element(Cvektory::TElement *E)
{
	//nastavení zoomu na vhodný náhled
	if(Zoom<=4 && OBJEKT_akt!=NULL)Zoom=5.0;
	zneplatnit_minulesouradnice();
	TPoint Centr;Centr.x=E->X;Centr.y=E->Y;
	//vycentrování obrazu na střed
	Posun.x=Centr.x/m2px-ClientWidth/2/Zoom;
	Posun.y=-Centr.y/m2px-(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2/Zoom; //ClientHeight-scGPPanel_bottomtoolbar->Height
}
//---------------------------------------------------------------------------
//zaktualizuje ve formuláři parametry objektů combobox na výpis pohonů včetně jednotek uvedeného rozmezí rychlostí, pokud jsou zanechané implicitní parametry short RDunitD=-1,short RDunitT=-1, je načteno nastevní jednotek z INI aplikace pro form parametry objektu, v případech, kdy uvedené parametry nejsou dané hodnotou -1, tak se uvažují jednotky dle S==0,MIN==1 pro RDunitT, resp. M==0,MM==1 pro RDunitD
void TForm1::aktualizace_combobox_pohony_v_PO(short RDunitD,short RDunitT)
{
   //R - 21.1.2020 - ZAKOMENTOVÁNO, VAZBA NA STARÉ PO - ODEBRÁNO Z PROJEKTU
	log(__func__);//logování
//	Cvektory::TPohon *P=Form1->d.v.POHONY->dalsi;//ukazatel na pohony, přeskakuje hlavičku, která je již vytvořena
//	Form_parametry->scComboBox_pohon->Items->Clear();//smazání původního obsahu
//	TscGPListBoxItem *t=NULL;
//	if(P==NULL)//pokud neexitustuje žádný pohon
//	{      //Form_parametry->scComboBox_pohon->Items->
//		t=Form_parametry->scComboBox_pohon->Items->Add(/*tady nelze parametr*/);
//		t->Caption="nebyl nadefinován";
//		Form_parametry->existuje_pohon=false;
//		//nesmí tu být, způsobuje cyklení z důvodu vytoření onchange pohonu Form_parametry->scComboBox_pohon->ItemIndex=0;//pohon nedefinován
//	}
//	else//pokud existuje přidá na první pozici nabídku nepřiřazen dále začne plnit existujícím pohny
//	{
//		Form_parametry->existuje_pohon=true;
//
//		//vytvoření položky nepřiřazen
//		t=Form_parametry->scComboBox_pohon->Items->Add(/*tady nelze parametr*/);
//		t->Caption="nepřiřazen";
//
//		////příprava vypisovaných jednotek
//		//čas
//		double jednotky_cas_pohon=60.0;AnsiString Tcas="min";//tzn. min (ač 60 působí nelogicky)
//		if(RDunitT==-1)
//		{
//			if(readINI("nastaveni_form_parametry","RDt")=="0"){jednotky_cas_pohon=1.0;Tcas="s";}//tzn. sec při načítání z ini
//		}
//		else
//		{
//			if(RDunitT==0){jednotky_cas_pohon=1.0;Tcas="s";}//tzn. sec z parametru
//		}
//
//		//délka
//		double jednotky_delka_pohon=1000.0;AnsiString Td="mm";//tzn. mm
//		if(RDunitD==-1)
//		{
//			if(readINI("nastaveni_form_parametry","RDd")=="0"){jednotky_delka_pohon=1.0;Td="m";}//tzn. m z ini
//		}
//		else
//		{
//			if(RDunitD==0){jednotky_delka_pohon=1.0;Td="m";}//tzn. m z parametru
//		}
//		UnicodeString caption_jednotky=Td+"/"+Tcas;
//		//plnění existujícím pohony
//		while (P!=NULL)
//		{
//			AnsiString dopRD="";
//			t=Form_parametry->scComboBox_pohon->Items->Add(/*tady nelze parametr*/);
//			if((unsigned)Form_parametry->scComboBox_pohon->ItemIndex==P->n && !Form_parametry->pohon_pouzivan && !Form_parametry->ComboBoxPohonyProbihaZmena)//pokud se jedná o aktuální pohon a pohon lze editovat a neprobíhá změna comboboxu a již existuje nějaký pohon
//			{     //pouze pro KK
//				if(Form_parametry->scComboBox_rezim->ItemIndex==1 && m.round2double(Form_parametry->scGPNumericEdit_RD->Value/(1+59.0*RDunitT),8)==m.round2double(m.dopRD(d.v.PP.delka_jig,d.v.PP.sirka_jig,Form_parametry->scComboBox_rotace->ItemIndex,P->roztec,d.v.PP.TT,Form_parametry->scGPNumericEdit_RD->Value/(1+59.0*RDunitT)),8))dopRD="  dopor.";
//				t->Caption=P->name+" - "+m.round2double(Form_parametry->scGPNumericEdit_RD->Value,3,"..")+" ("+AnsiString(m.round2double(P->rychlost_od*jednotky_cas_pohon*jednotky_delka_pohon,2))+"-"+AnsiString(m.round2double(P->rychlost_do*jednotky_cas_pohon*jednotky_delka_pohon,2))+") "+caption_jednotky+dopRD;//vypíše aktuální editovanou hodnotu
//			}
//			else//pro ostatní pohony, či aktální používaný (přiřazen více objektům) pohon - většinová situace
//			{   //pouze pro KK
//				if(Form_parametry->scComboBox_rezim->ItemIndex==1 && m.round2double(P->aRD,8)==m.round2double(m.dopRD(d.v.PP.delka_jig,d.v.PP.sirka_jig,Form_parametry->scComboBox_rotace->ItemIndex,P->roztec,d.v.PP.TT,P->aRD),8))dopRD="  dopor.";
//				t->Caption=P->name+" - "+m.round2double(P->aRD*(1+59.0*RDunitT),3,"..")+" ("+AnsiString(m.round2double(P->rychlost_od*jednotky_cas_pohon*jednotky_delka_pohon,2))+"-"+AnsiString(m.round2double(P->rychlost_do*jednotky_cas_pohon*jednotky_delka_pohon,2))+") "+caption_jednotky+dopRD;
//			}
//			P=P->dalsi;//přesun na další pohon
//		}
//		//nastavení comba, pokud neexistuje pohon
//		if(d.v.POHONY->dalsi==NULL)Form_parametry->scComboBox_pohon->ItemIndex=0;//nepřiřazen
//	}
//	Form_parametry->scComboBox_pohon->Refresh();
}
//---------------------------------------------------------------------------
//aktualizace a přepočet hodnot volaná kvůli časovým osám (maro) a techn.procesům(roma)
void TForm1::aktualizace_maro_a_roma()
{
  log(__func__);//logování   3.6.2020  R - asi ke smazani
//  //toto je v testování - prvni_zakazka.....
//	d.v.vytvor_default_zakazku();//pokud první zakázka neexistuje, založí ji a přiřadí ji cestu dle schématu, pokud existuje, tak ji pouze přiřadí cestu dle schématu
//	if(scGPCheckBox_pocet_voziku_dle_WIP->Checked)//pokud je aktulizace dle hodnoty WIP+1 povolena
//	{
//		short WIP=d.v.WIP(1);
//		if((unsigned)(WIP+1)!=d.v.VOZIKY->n && STATUS==NAVRH)//v případě, že je v návrháři a aktulní WIP neodpovídá počtu vozíku, zakatulizuje se i počet vozíku, resp. se vygenerují nové ve správném novém počtu (podmínka použita pouze pro šetření strojového času)
//		{
//			d.v.ZAKAZKY->dalsi->pocet_voziku=WIP+1;
//			d.v.generuj_VOZIKY();
//		}
//	}
//	d.JIZPOCITANO=false;//nutnost zakutalizovat časové osy
//	if(MOD==TECHNOPROCESY)//v případě technologických procesů (ROMA) i jejich přepočítání
//	{
//		Graphics::TBitmap *bmp_temp=new Graphics::TBitmap;bmp_temp->Width=0;bmp_temp->Height=0;//aby se nevykreslovalo přímo do Form1->Canvasu a neproběhl problik časových os, bmp obcházím nutnost vykreslování do canvasu, protože mi jde jen o přepočet časových os, bohužel ve vykresli_casove osy je společně jak výpočet, tak vykreslování, což není z tohoto pohledu dobře (zachováno bylo z důvodu efektivity při vykreslování/výpočtu)
//		d.vykresli_casove_osy(bmp_temp->Canvas);//u ROMA ještě nutno předtím zaktulizovat výpočet na časových osách
//	 //	g.ShowGrafy(false);  //GRAFY - ODEBRÁNY Z BUILDU  21.1.2020 - ZAKOMENTOVÁNO, ODEBRÁNO Z PROJEKTU
//		bmp_temp=NULL; delete bmp_temp;
//	}
}
//---------------------------------------------------------------------------
//pokud je označený objekt, zajistí jeho zkopírování, připočítá index 1,2,3
void TForm1::kopirovat_objekt()
{
  log(__func__);//logování
	if(pom!=NULL)//pokud je vybraný objekt
	{
		Cvektory::TObjekt *O=pom,*pred=NULL,*po=NULL;//uložení kopírovaného objektu
		Cvektory::TElement *e_posledni=d.v.vrat_posledni_element_objektu(pom);
		Akce=ADD;vybrany_objekt=pom->id;
		pom_vyhybka=add_objekt(m.L2Px(e_posledni->geo.X4),m.L2Py(e_posledni->geo.Y4));
		Akce=NIC;
		pom=O;O=NULL;delete O;//vrácení kopirovaného objektu do pom, metoda add_objekt smaže pom
		//uložení atributů které se přepíší
		double posunx=-pom->element->geo.X1+pom_vyhybka->element->geo.X1,posuny=pom->element->geo.Y1-pom_vyhybka->element->geo.Y1;
		pred=pom_vyhybka->predchozi;po=pom_vyhybka->dalsi;
		d.v.vymaz_komory(pom_vyhybka);
		d.v.kopiruj_objekt(pom,pom_vyhybka);
		move_objekt(posunx,posuny,pom_vyhybka);
		pom_vyhybka->predchozi=pred;pom_vyhybka->dalsi=po;
		pom_vyhybka->n+=1;
		pom_vyhybka=NULL;O=NULL;pred=NULL;po=NULL;e_posledni=NULL;
		delete pom_vyhybka;delete O;delete pred;delete po;delete e_posledni;
		DuvodUlozit(true);
//				bool remove_pre_index=true;
//				if(pom->short_name=="CO2")remove_pre_index=false;//pokud se jedná CO2, tak aby nedával CO3, CO4 atp
//				if(pom->dalsi!=NULL)//pokud po vybraném následuje další objekt, tak nový vkládá přesně mezi ně
//				{
//					d.v.kopiruj_objekt(pom,(pom->X+pom->dalsi->X)/2-pom->X,(pom->Y+pom->dalsi->Y)/2-pom->Y,ms.a2i(pom->short_name.SubString(pom->short_name.Length(),1))+1,remove_pre_index,pom);
//				}
//				else //jinak odsazeně
//				{
//					if(pom==d.v.OBJEKTY->predchozi && pom->n==1)//pokud je jenom jeden objekt
//					d.v.kopiruj_objekt(pom,6,0,ms.a2i(pom->short_name.SubString(pom->short_name.Length(),1))+1,remove_pre_index);
//					if(pom==d.v.OBJEKTY->predchozi)//pokud se jedná o poslední prvek
//					d.v.kopiruj_objekt(pom,(pom->X+d.v.OBJEKTY->dalsi->X)/2-pom->X,(pom->Y+d.v.OBJEKTY->dalsi->Y)/2-pom->Y,ms.a2i(pom->short_name.SubString(pom->short_name.Length(),1))+1,remove_pre_index);
//				}
//				ortogonalizace();
//				REFRESH();
//				DuvodUlozit(true);
	}
}
//---------------------------------------------------------------------------
//volá dialog kalkulátor TT
void __fastcall TForm1::Button_kalkulatorTTClick(TObject *Sender)
{
	log(__func__);//logování
//ZDM
//	Form_TT_kalkulator->Left=0;//akt_souradnice_kurzoru_PX.x+0;
//	Form_TT_kalkulator->Top=0+vyska_menu+RzToolbar1->Height;//akt_souradnice_kurzoru_PX.y+0;
//	Form_TT_kalkulator->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_takt_timeChange(TObject *Sender)
{
	log(__func__);//logování
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
	log(__func__);//logování
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
  log(__func__);//logování
	//Edit_takt_timeChange(this);
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//volá dialog k nastavení parametrů voziku
void __fastcall TForm1::Button_vozik_parametry_OLDClick(TObject *Sender)
{
	log(__func__);//logování
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
	log(__func__);//logování
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
  log(__func__);//logování
	return ms.delete_repeat_all(FileName,"\\");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UlozitClick(TObject *Sender)
{
	log(__func__);//logování
	ESC();//ukončení případné akce
	if(FileName=="")FileName="Nový.tispl";
	if(FileName=="Nový.tispl" || FileName.Pos(".tisplTemp"))UlozitjakoClick(this);
	else
	{
		scSplitView_MENU->Opened=false;
		if(duvod_k_ulozeni)Ulozit_soubor();
		else SB(ls->Strings[389]);//"Soubor byl již uložen..."
	}
	if(MOD==EDITACE && duvod_ulozit_nahled)//uložení z editace = uložím editovaný objekt + celý projekt
	{
		Ulozit_soubor();
		vymaz_seznam_obrazu();//vymazání nepotřebných obrazů
		vytvor_obraz(true);//vytvoření obrazu pro storno a UNDO
		nahled_ulozit(false);
	}
	scButton_ulozit->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UlozitjakoClick(TObject *Sender)
{
  log(__func__);//logování
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
		ulozit_posledni_otevreny();
	}
	else//stisknuto storno
	stisknuto_storno=true;
}
//---------------------------------------------------------------------------
//samotné uložení
void TForm1::Ulozit_soubor()
{
	log(__func__);//logování

	//aktualizace číslování elementů, je důležité aby ukládací alg. měl správně číslované elementy
	Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
  Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	unsigned long n=1;
	while(E!=NULL)
	{
		E->n=n;
		n++;
		E=d.v.dalsi_krok(VYHYBKY,E);
	}
	d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
	delete E;E=NULL;

	//zapis dat do souboru
	d.v.PP.cas_posledni_upravy=TIME.CurrentDateTime();
	d.v.PP.upravil=get_user_name();
	d.v.uloz_do_souboru(FileName);

	//nastavení komponent + výpis
//	String def_setings=FormatSettings.LongTimeFormat;
//	FormatSettings.LongTimeFormat="h:mm";
	SB(String(ls->Strings[390]+" "+TimeToStr(TIME.CurrentTime())));//"Soubor uložen..."
	DuvodUlozit(false);
//	FormatSettings.LongTimeFormat=def_setings;



	//zakazuje UNDO
	//ToolButton_undo->Enabled=false;//přepne příslušně nabídky menu
	//Zpet->Enabled=false;//přepne příslušně nabídky menu
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//otevře soubor z menu či toolbaru
void __fastcall TForm1::Toolbar_OtevritClick(TObject *Sender)
{
	log(__func__);//logování
	ESC();//ukončení případné akce
	scSplitView_MENU->Opened=false;
	scButton_otevrit->Down=false;
	if(duvod_k_ulozeni)//pokud existuje předcházejicí soubor, který je nutný uložit
	{
		AnsiString FNs=FileName_short(FileName);
		if(FNs=="")FNs="Nový.tispl";
		int result=MB(FNs+" "+ls->Strings[318],MB_YESNOCANCEL);//" byl změněn.\nChcete ho před ukončením uložit?"
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
	log(__func__);//logování
	OpenDialog1->Title="Otevřít soubor";
	OpenDialog1->DefaultExt="*.tispl";
	OpenDialog1->Filter="Soubory formátu TISPL (*.tispl)|*.tispl";
	if(OpenDialog1->Execute())
	{
		//zavolá nový soubor/smaže stávajicí
		Novy_soubor(false);

		//otevrení souboru
		Otevrit_soubor(OpenDialog1->FileName);

		//zajištění vykreslení načteného souboru
		REFRESH();
	}
}
//-------------------------------------------------------------------------
unsigned short int TForm1::Otevrit_soubor(UnicodeString soubor)//realizuje samotné otevření souboru
{
	log(__func__);//logování
	unsigned short int ret=d.v.nacti_ze_souboru(soubor);
	//načte data ze souboru a reaguje na návratovou hodnotu
	UnicodeString text=ls->Strings[345]+" ",text_1=" "+ls->Strings[346],text_2=ls->Strings[347];//"Soubor "," nebyl nalezen!","Neplatná verze souboru formátu *.tispl!"
	switch(ret)
	{
		case 0://Soubor nebyl nalezen
		{
			zavrit_uvod();//v případě chybové situace, např. soubor nenalezen, nebo špatný formát souboru zavře úvodní dialog
			MB(text+FileName_short(FileName)+text_1);
			//zavrit_uvod();
			FileName="Nový.tispl";
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
			//d.v.PP.vytvoril=d.v.File_hlavicka.vytvoril;//načteno ve vektorech
			d.v.PP.cas_posledni_upravy=d.v.File_hlavicka.cas_posledni_upravy;
			//d.v.PP.upravil=d.v.File_hlavicka.upravil;//načteno ve vektorech
			d.v.PP.mnozstvi=d.v.File_hlavicka.mnozstvi;
			d.v.PP.hod_den=d.v.File_hlavicka.hod_den;
			d.v.PP.dni_rok=d.v.File_hlavicka.dni_rok;
			d.v.PP.TT=d.v.File_hlavicka.TT;
			d.v.PP.efektivita=d.v.File_hlavicka.efektivita;
			d.v.PP.typ_linky=d.v.File_hlavicka.typ_linky;
			d.v.PP.delka_jig=d.v.File_hlavicka.delka_jig;
			d.v.PP.sirka_jig=d.v.File_hlavicka.sirka_jig;
			d.v.PP.vyska_jig=d.v.File_hlavicka.vyska_jig;
			d.v.PP.delka_podvozek=d.v.File_hlavicka.delka_podvozek;
			d.v.PP.uchyt_pozice=d.v.File_hlavicka.uchyt_pozice;
			d.v.PP.zamek_layoutu=d.v.File_hlavicka.zamek_layoutu;
			d.v.PP.katalog=d.v.File_hlavicka.katalog;
			d.v.PP.typ_linky=d.v.File_hlavicka.typ_linky;
			d.v.PP.radius=d.v.File_hlavicka.radius;
			d.v.pocet_vyhybek=d.v.File_hlavicka.pocet_vyhybek;
			if(d.v.File_hlavicka.zakazka_akt!=0)d.v.ZAKAZKA_akt=d.v.vrat_zakazku(d.v.File_hlavicka.zakazka_akt);
			else d.v.ZAKAZKA_akt=NULL;
			d.v.update_akt_zakazky();
			//MOD=d.v.File_hlavicka.Mod;
			MOD=LAYOUT;//defaultně se bude vždy otvírat v layoutu
//			switch(MOD)
//			{
//					case NO:REFRESH();break; //překreslí obraz pro ostatní případy
//					case LAYOUT: 	/*schemaClick(this);*/break;   //????????????? při modu schéma zapínat schéma ??? odstaveno
//					//ZDM case TESTOVANI:	testovnkapacity1Click(this);break;
//					//ZDM case REZERVY:		casoverezervy1Click(this);break;
//					//ZDM case CASOVAOSA:	editacelinky1Click(this);MOD=LAYOUT;/*casovosa1Click(this);*/break;
//					//ZDM case TECHNOPROCESY:editacelinky1Click(this);MOD=LAYOUT;break;
//					//ZDM case SIMULACE:	editacelinky1Click(this);MOD=LAYOUT;/*simulace1Click(this);*/break;
//					default: schemaClick(this);break;
//			}
			ulozit_historie_otevrenych();
			DuvodUlozit(false);
			duvod_validovat=2;//soubor se zvaliduje při prvním refresh
			//aktualizace statusbaru
			on_change_zoom_change_scGPTrackBar();
			//kontrola jestli má načítaný projekt zamknutý layout, pokud ano a prostřední má layout odemčen, zamče
			if(scButton_zamek_layoutu->ImageIndex!=67 && d.v.PP.zamek_layoutu)scButton_zamek_layoutuClick(this);
			d.SCENA=vychozi_stav_sceny;
			vytvor_statickou_scenu();
      //pokud je v PP raster, ale nelze ho najít vypíše se dotaz na aktualizaci cesty k souboru
			if(d.v.PP.raster.filename!="" && !FileExists(d.v.PP.raster.filename))
	  	{
	  		//extrakce názvu souboru
	  		String name=d.v.PP.raster.filename;
	  		unsigned int i=name.Length();
	  		while(i>0)
	  		{
	  			if(name[i]==92)break;//pokud sem na lomítku znamená to, že sem prošel přes kompletní název souboru, ukončit
	  			else i--;//zpětný průchod
	  		}
	  		name=name.SubString(i+1,name.Length()-i+1);
	  		//kontrola adresáře s EXE, zda neobsahuje podklad
	  		name=ExtractFilePath(Application->ExeName).c_str()+name;
	  		if(FileExists(name))d.v.PP.raster.filename=name;
			}
			if(d.v.PP.raster.filename!="" && !FileExists(d.v.PP.raster.filename))REFRESH();
			if(d.v.PP.raster.filename!="" && !FileExists(d.v.PP.raster.filename) && mrYes==MB(ls->Strings[424]+" "+d.v.PP.raster.filename+ls->Strings[425],MB_YESNO))Nacist_podklad();//"Nepodařilo se načíst podklad filename, zkontrolujte jeho existenci, nebo proveďte nové načtení."
		}break;
		case 2:case 3://jiná chyba pravděpodbně špatný formát souboru + neshodné verze projektu a binárky
		{
			//načtení verze souboru
			String verze=d.v.File_hlavicka.FileVersion;
			int pozice=verze.Pos(",");
			if(0<pozice && pozice<5){verze.Delete(pozice,1);verze.Insert(".",pozice);}//nahrazení "," (double) za "." (String)
      else verze="N/A";//pokud se načte nesmysl
			zavrit_uvod();//v případě chybové situace, např. soubor nenalezen, nebo špatný formát souboru zavře úvodní dialog
			d.v.vse_odstranit();//musí být, při načítání může dojít k chybě v pozdějším stadiu, tz. nečo se načte
			if(ret==2)MB(text_2,MB_OK);
			else MB(ls->Strings[488]+" "+verze+ls->Strings[489]+" "+FileVersion+ls->Strings[495]+" "+ProductVersion+".",MB_OK);//Projekt nelze otevřít. Verze načítaného projektu - "verze_souboru". Požadovaná verze -  "verze_souboru_aktualni". Verze aplikace - “app_version”
			FileName="Nový.tispl";
		}break;
		default:break;
	}
	return ret;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Obnovitzezlohy1Click(TObject *Sender)
{
  log(__func__);//logování
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
//  log(__func__);//logování
//	OpenDialog1->Title="Otevřít šablonu";
//	OpenDialog1->DefaultExt="*tisplTemp";
//	AnsiString nazev="Soubory šablon TISPL (*.tisplTemp)|*.tisplTemp";
//	OpenDialog1->Filter=nazev;
//	if(OpenDialog1->Execute())
//	{
//		//zavolá nový soubor/smaže stávajicí
//		Novy_soubor();
//
//		//otevrení souboru
//		Otevrit_soubor(OpenDialog1->FileName);
//
//		//ještě donutí stávajicí soubor uložit pod novým jménem
//		FileName="";
//		UlozitjakoClick(this);
//	}
}
//---------------------------------------------------------------------------
//uložení posledního otevřeného souboru
void TForm1::ulozit_posledni_otevreny()
{
	log(__func__);//logování
	//-TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
	//-ini->WriteString("otevrene_soubory","posledni_soubor",FileName);
	//-delete ini;
	writeINI("otevrene_soubory","posledni_soubor",FileName);//ukládání posledního otevřeného osuboru
}
//---------------------------------------------------------------------------
//ukládání 3 naposledy otevřených projektů do historie
void TForm1::ulozit_historie_otevrenych()
{
  ulozit_posledni_otevreny();//uloží do posledního editovaného souboru
	UnicodeString ps=readINI("historie","posledni_soubor_1"),ps1=readINI("historie","posledni_soubor_2"),ps2=readINI("historie","posledni_soubor_3");
	if(FileName!="Nový.tispl" && FileName!=ps && FileName!=ps1 && FileName!=ps2)//kontrola zda není ukládány už uložený v posledních otevřených
	{
		if(!scButton_posledni_otevreny->Enabled)scButton_posledni_otevreny->Enabled=true;
		writeINI("historie","posledni_soubor_1",FileName);
		N1projekt1->Caption=FileName;
		writeINI("historie","posledni_soubor_2",ps);
		N2projekt1->Caption=ps;
		writeINI("historie","posledni_soubor_3",ps1);
		N3projekt1->Caption=ps1;
		//zobrazení či skrytí položek
		if(N1projekt1->Caption=="")N1projekt1->Visible=false;
		else N1projekt1->Visible=true;
		if(N2projekt1->Caption=="")N2projekt1->Visible=false;
		else N2projekt1->Visible=true;
		if(N3projekt1->Caption=="")N3projekt1->Visible=false;
		else N3projekt1->Visible=true;
	}
}
//---------------------------------------------------------------------------
//zavře úvodní dialog
void TForm1::zavrit_uvod()
{
  log(__func__);//logování
	Form_uvod->Timer1->Enabled=false;
	Form_uvod->Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//zajišťuje průběžné zálohování aplikace
void __fastcall TForm1::Timer_backupTimer(TObject *Sender)
{
	log(__func__);//logování

	//nastevení adresáře bac souboru k adresáři aplikace pokud se jedná o nový soubor, ještě neuložen pod novým názvem//přidáno 3.6 důsledky neověřeny
	if(FileName=="Nový.omap")SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	//zapis dat do souboru
	if(FileName=="")FileName="Nový.tispl";//někdy se stane, že FileName je prázdný, ošetření převzato z ukládání souboru
	d.v.uloz_do_souboru(FileName+".bac_"+get_user_name()+"_"+get_computer_name());

	//odstranění dlouhé adresy
	AnsiString jen_nazev=FileName;
	while(jen_nazev.Pos("\\")>0)//dokud bude lomítko obsahovat
	jen_nazev.Delete(1,jen_nazev.Pos("\\"));
	SB(jen_nazev+" "+ls->Strings[391]+" "+FileName+".bac_"+get_user_name()+"_"+get_computer_name());//" zálohován do "
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//vše smaže z paměti
void TForm1::vse_odstranit()
{
	log(__func__);//logování
	//TimerKill(TimerSimulaceID);//musí být první, aktuálně nepoužito
	d.v.vse_odstranit();
	if(PmG!=NULL){PmG->Delete();PmG=NULL;delete PmG;}
	if(OBJEKT_akt!=NULL)scGPButton_stornoClick(this);
	if(pom!=NULL){pom->pohon=NULL;delete pom->pohon;}pom=NULL;delete pom;
	if(OBJEKT_akt!=NULL){OBJEKT_akt->pohon=NULL;delete OBJEKT_akt->pohon;}OBJEKT_akt=NULL;delete OBJEKT_akt;
	pom_element=NULL;delete pom_element;
	proces_pom=NULL;delete proces_pom;
	pom_element_temp=NULL; delete pom_element_temp;
	pom_komora=NULL;delete pom_komora;
	pom_komora_temp=NULL;delete pom_komora_temp;
	pom_bod=NULL;delete pom_bod;
	pom_bod_temp=NULL;delete pom_bod_temp;
	posledni_editovany_element=NULL;delete posledni_editovany_element;
	predchozi_PM=NULL;delete predchozi_PM;
	copyObjekt=NULL;delete copyObjekt;
	copyObjektRzRx.x=0;copyObjektRzRx.y=0;
	vlakno_obraz=NULL;delete vlakno_obraz;
	delete[] pouzivane_pohony;pouzivane_pohony=NULL;
	delete(Staticka_scena);Staticka_scena=NULL;//musí být =NULL, vse_odstranit() se spouští např. při stisku nového projektu
	//if(Pan_bmp!=NULL)delete(Pan_bmp);Pan_bmp=NULL;muselo by se alokovat při použítí nikoliv v konstruktoru, vhodné předělat
	if(Pan_bmp_ALL!=NULL)delete(Pan_bmp_ALL);Pan_bmp_ALL=NULL;
	//delete LogFileStream; //zde nesmí být kvůli logování (resp. musí se ukončnovat až při konci aplikace nikoliv projektu), proto to zde nechat jako upozornění
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//načte a vrátí DPI zobraovače
int TForm1::get_DPI ()
{
  log(__func__);//logování
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
  log(__func__);//logování
	wchar_t *pc=new wchar_t[MAX_COMPUTERNAME_LENGTH+1];
	DWORD dwSize=MAX_COMPUTERNAME_LENGTH+1;
  if(GetComputerName(pc,&dwSize))return AnsiString(pc);
	else return "";
}
//---------------------------------------------------------------------------
UnicodeString TForm1::get_user_name()
{
	log(__func__);//logování
	wchar_t *user=new wchar_t[20+1];
	DWORD dwSize_user=20+1;
	if(GetUserName(user,&dwSize_user))return AnsiString(user);
	else return "";
}
//---------------------------------------------------------------------------
UnicodeString TForm1::get_temp_dir()
{
	log(__func__);//logování
	WCHAR lpTempPathBuffer[MAX_PATH];
	GetTempPathW(MAX_PATH,lpTempPathBuffer);
	UnicodeString TempPath=lpTempPathBuffer;
	return TempPath;
}
//---------------------------------------------------------------------------
UnicodeString TForm1::get_Windows_dir()
{
  log(__func__);//logování
	const unsigned int INFO_BUFFER_SIZE=32767;
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetWindowsDirectory( infoBuf, INFO_BUFFER_SIZE ); //GetSystemDirectory
	return infoBuf;
}
//---------------------------------------------------------------------------
//vrátí celkovou paměť na daném zařízení
DWORDLONG TForm1::get_TotalPhysMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullTotalPhys;
}
//---------------------------------------------------------------------------
//vrátí aktuálně dostupnou paměť na daném zařízení
DWORDLONG TForm1::get_AvailPhysMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullAvailPhys;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//načte nastavení programu z INI souboru
void TForm1::nacist_nastaveni()
{
	log(__func__);//logování
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
  log(__func__);//logování
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
	log(__func__);//logování
	//nastavení stavu ovládacích prvků
 //	RzToolButton3->Enabled=stav;
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
  log(__func__);//logování
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
			case LAYOUT:
			if(!antialiasing)d.vykresli_vektory(Bitmap->Canvas);//vykreslování všech vektorů
			else
			{
				Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
				bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
				Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
				d.vykresli_vektory(bmp_in->Canvas);
				Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
				Cantialising a;
				Bitmap=a.antialiasing(bmp_in);
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
		SB(ls->Strings[392]+" "+export_format+" "+ls->Strings[393]);//"Export do "," dokončen."
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Rychlexport1Click(TObject *Sender)
{
  log(__func__);//logování
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
	SB(ls->Strings[394]);//"Rychlý export dokončen."
}
//---------------------------------------------------------------------------
void TForm1::nastaveni_grafickeho_vystupu(Graphics::TBitmap * Bitmap,unsigned int OD,unsigned int PO)
{
  log(__func__);//logování
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
	log(__func__);//logování
	// puvodni export do csv
}
//---------------------------------------------------------------------------
void __fastcall TForm1::html1Click(TObject *Sender)
{
  log(__func__);//logování
	scSplitView_MENU->Opened=false;
	scButton_report->Down=false;
	if(d.v.OBJEKTY->dalsi==NULL)MB(ls->Strings[348]);//pokud existují nějaka data,"Žádná data k reportu!"
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
  log(__func__);//logování
	//nezabira zneplatnit_minulesouradnice();
	Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Vypicestuktempu1Click(TObject *Sender)
{
	log(__func__);//logování
 //	UnicodeString text="<b>"+ls->Strings[349]+" </b>",text_1="<br><br><b>"+ls->Strings[350]+" </b>",text_2=ls->Strings[351];//"<b>Verze: </b>","<br><br><b>Umístění: </b>","Informace o aplikaci"
 //	MB(-1,-1,text+VERZE+text_1+Application->ExeName+"<br><br><b>Temp : </b>"+get_temp_dir(),text_2);
 Form_help->ShowModal();

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxPALCE_Click(TObject *Sender)
{
	log(__func__);//logování
	//d.JIZPOCITANO=true;
	Invalidate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Chart1Click(TObject *Sender)
{
	log(__func__);//logování
	// Panel1->Visible=true;
	// Panel1->Width=ClientWidth;
	// Panel1->Height=ClientHeight;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
//	log(__func__);//logování
//	switch(d.NOLIEX)
//	{
//		case 0:d.NOLIEX++;Button2->Caption="LINEAR";break;
//		case 1:d.NOLIEX++;Button2->Caption="EXPO";break;
//		case 2:d.NOLIEX=0;Button2->Caption="NO";break;
//	}
//	Invalidate();
}
//---------------------------------------------------------------------------
//skryje v době neaktivity (po 50 sec) svislice na myši v modu časové osy (kvůli spořiči obrazovky)
void __fastcall TForm1::Timer_neaktivityTimer(TObject *Sender)
{
	log(__func__);//logování
	if(OBJEKT_akt!=NULL)//pro případ, že uživatel změní hodnotu v tabulce a než se stihne provést validace ukončí editaci
	{
		vytvor_obraz();//obraz pro ctrl+z
		FormX->validace_max_voziku();//metoda rozlišuje zda byla editovaná stopka, pokud ano provede validaci, pokud ne neudělá nic
		//pokud byl poslední editovaný element PM spustí validaci
		if((FormX->posledni_E!=NULL && (FormX->posledni_E->eID==200 || FormX->posledni_E->eID==300)) || FormX->validovat_pohon)FormX->validace_RD(FormX->posledni_E);
		if(d.v.vrat_druh_elementu(FormX->posledni_E)==0 && FormX->posledni_E->eID!=0)FormX->validace_PT(FormX->posledni_E,FormX->posledni_row);
		if(FormX->posledni_E!=NULL && FormX->posledni_E->eID==0)FormX->aktualizace_teplomeru();
		if(PmG!=NULL)FormX->validace_aRD();
		duvod_validovat=2;//může dojít k přenastavení pohonu, nutné přegenerovat vozíky
		nahled_ulozit(true);
		REFRESH(true);//nedocází k refresh tabulek, tabulky jsou v tuto chvíli naplněny aktuálními hodnotami
	}
	Timer_neaktivity->Enabled=false;
}
//---------------------------------------------------------------------------
//tlačítko na spuštění animace v náhledu kabiny
void __fastcall TForm1::scGPGlyphButton_PLAYClick(TObject *Sender)
{
	if(d.v.SIM==NULL)vytvorScenar();

	Timer_simulace->Enabled=!Timer_simulace->Enabled;
	Timer_simulace->Interval=50;
	if(Timer_simulace->Enabled)//běží animace
	{
		scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPause;
		scGPGlyphButton_PLAY->Hint="zastavit animaci";
		if(CAS==0)Memo(String(CAS)+" [s]: START LINKY");
		d.v.VOZIKY->dalsi->stav=-1;
		d.v.VOZIKY->dalsi->dalsi->stav=-1;
		d.v.VOZIKY->dalsi->dalsi->dalsi->stav=-1;
			//MOD=SIMULACE;//mód musí být před vytvořením scény - zde narušuje zobrazení
		d.SCENA=221111;//zprávy nejsou zobrazeny, vozíky a aktivní části elementů jsou v dynamické scéně
		vytvor_statickou_scenu();//načtení statických záležitostí do statické scény, musí být před REFRESH
		provedScenar();//pro čas 0
		REFRESH();
	}
	else
	{
		scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPlay;
		scGPGlyphButton_PLAY->Hint="spustit animaci";
  }



//	if(MOD==LAYOUT || MOD==SIMULACE)
//	{
//		log(__func__);//logování
//		RO-=(1.5*Zoom/m2px)/20.0;
//		sTIME=0;
//
//		Timer_simulace->Enabled=!Timer_simulace->Enabled;
//	//	if(MOD==EDITACE)scGPButton_viditelnostmGridClick(Sender);//zakáže mgridy - dodělat, když nebudou zobrazené....
//	//	zobrazit_meritko=!Timer_animace->Enabled;
//	//	d.v.PP.raster.show=!Timer_animace->Enabled;
//		if(Timer_simulace->Enabled)//běží animace
//		{
//			scGPCheckBox_rozmisteni_voziku->Checked=true;
//			MOD=SIMULACE;//mód musí být před vytvořením scény
//			d.SCENA=221111;//zprávy nejsou zobrazeny, vozíky a aktivní části elementů jsou v dynamické scéně
//			vytvor_statickou_scenu();//načtení statických záležitostí do statické scény, musí být před REFRESH
//			//ovládací prvky
//			scGPButton_bug_report->Visible=false;
//			if(scButton_zamek_layoutu->ImageIndex==68)scButton_zamek_layoutuClick(Sender);//pokud ještě není layout zamčený zamkne, pozor OBSAHUJE REFRESH
//			//tlačítko simulace
//			scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPause;
//			scGPGlyphButton_PLAY->Hint="zastavit animaci";
//			scGPGlyphButton_PLAY->ShowCaption=true;
//			//nastavení timeru
//									//optimálně pohybově nejpomalejšího pohonu či animovaného elementu
//			//Timer_simulace->Interval=floor(m2px/(Zoom/**3*/)/d.v.vrat_min_rychlost_prejezdu()*1000.0/fps);   //ceil(F->m.get_timePERpx(pom->RD,0,d.v.vrat_min_rychlost_prejezdu()));//různá rychlost dle RD, s afps se počítá dle min RD, ale nějak špatně vycházela animace ke konci (nestihl vozík vyjet)
//			Timer_simulace->Interval=40;
//			//Multimediální timer - nevyužito, náročné na synchronizaci vykreslovaných dat, zachováno jako vzor
////			int TimerSimulaceInterval=40;
////			TIMECAPS tc;timeGetDevCaps(&tc,sizeof(TIMECAPS));//zjištění přesnosti multimediálního časovače
////			TimerPresnost=tc.wPeriodMin;//vrácení minimální možné přesnosti (1 ms)
////			timeBeginPeriod(TimerPresnost);//if(TIMERR_NOERROR!=timeBeginPeriod(TimerSimulacePresnost))Application->Terminate();
////			TimerSimulaceID=timeSetEvent(TimerSimulaceInterval,TimerPresnost,TimerSimulaceEvent,0,TIME_PERIODIC);
//		}
//		else//animace zastavena
//		{
//			MOD=LAYOUT;//mód musí být před vytvořením scény
//			d.SCENA=0;
//			scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPlay;
//			scGPGlyphButton_PLAY->Hint="spustit animaci";
//			zobrazit_meritko=scGPSwitch_meritko->State;//navrácení do původního stavu
//			scGPButton_bug_report->Enabled=false;
//			TimerKill(TimerSimulaceID);
//		}
//	}
//	else MB("V této verzi aplikace lze spustit simulaci pouze v layoutu.");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_PLAYplusClick(TObject *Sender)
{
	if(d.v.SIM==NULL)vytvorScenar();
	CAS++;
	Timer_simulaceTimer(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_PLAYminusClick(TObject *Sender)
{
	if(d.v.SIM!=NULL)
	{
		CAS--;
		Timer_simulaceTimer(Sender);
		Memo_testy->Lines->Delete(Memo_testy->Lines->Count-1);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_simulaceTimer(TObject *Sender)
{
	if(d.v.SIM!=NULL)
	{
		if(CAS<=d.v.SIM->predchozi->T2)//dokud se neprojel celý scénář
		{
			CAS+=0.1;
			scGPGlyphButton_PLAY->Caption=String(floor(CAS))+" s";
			provedScenar();
			REFRESH();
		}
		else//simulace znovu od počátku
		{
			CAS=0;
		}

	}
//	if(++pocitadlo_doby_neaktivity*Timer_simulace->Interval>2000)
//	scGPLabel_zamerovac->Visible=true;
//	else scGPLabel_zamerovac->Visible=false;

//	//log(__func__);//logování tady nepoužívat, případně jen v mimořádných testovacích účelech
//	//if(MOD==EDITACE)
//	{
//		double LO=1.5;//lakovací okno
//
//		//rameno - kýve se po šířce lakovacího okna
//		if(ROs==0)
//		{
//			if(RO>-(LO*Zoom/m2px))
//			{
//				RO-=(LO*Zoom/m2px)/20.0;//krok po 20
//			}
//			else ROs=1;
//		}
//		else
//		{
//			if(RO<(LO*Zoom/m2px))
//			{
//				RO+=(LO*Zoom/m2px)/20.0;//krok po 20
//			}
//			else ROs=0;
//		}
//
//		//tryska - jen se kýve ze strany na strany v rozmezí -30 až +30 stupňů
//		if(ROsts==0)
//		{
//			if(ROst<30)ROst+=5;//krok po 5
//			else ROsts=1;
//		}
//		else
//		{
//			if(ROst>-30)ROst-=5;//krok po 5
//			else ROsts=0;
//		}
//
//		//sTIME+=1;/*1*///zajistí posun animace vždy o 1s reálného času (strojového dle Timer_animace->Interval, který by měl reflektovat aktuální rychlosti zajišťující plynulost animace)
//		//d.v.generuj_VOZIKY();//velice prozatim
//		//d.v.posun_palce_retezu();
//		//F->Canvas->Draw(F->sTIME,0,F->Pan_bmp);
//		if(d.v.sIterator==NULL)
//		{
//			//Timer_simulace->Enabled=false;neukoční jen čeká na doplnění bufferu
//			Memo("Konec bufferu");  //ke zvážení čekání třeba 5 s
//		}
//		else
//		{
//			//Canvas->Draw(d.v.sIterator->n-1,0,d.v.sIterator->bmp,100);
//			d.v.sIterator=d.v.sIterator->dalsi;
//			REFRESH();
//		}
//	}
}
//---------------------------------------------------------------------------
//testovácí metody simulace
void TForm1::vytvorScenar()
{
	Cvektory::TVozik *V3=d.v.VOZIKY->dalsi;
	Cvektory::TVozik *V2=d.v.VOZIKY->dalsi->dalsi;
	Cvektory::TVozik *V1=d.v.VOZIKY->dalsi->dalsi->dalsi;

	short start=0;//asi zbytečné
	short nSTOP=3;
	short indexStopV1=3;
	short indexStopV2=indexStopV1-1;
	short indexStopV3=indexStopV1-2;
	double L=3;
	double MT=30;//E1->geo.delka/E1->pohon->aRD;
	double T=start;
	double WTmax=7;//konstantní, maximální

	//níže přípra na optimalizaci kodu...
	for(long i=0;i<nSTOP*2;i++)//počet průjezdů mezi stopkami
	{
		//if(i==0)
		T=(d.v.PP.TT-WTmax-L)*(i)+start;//cykletime mezi otevřením stopek,začátek čekání, na palec VŽDY VE STEJNÝ ČAS, ja taktováno řízením, ne okolnostmi!!! - OTEVŘENO
		//else T+=L;
		if(i>0)d.v.SIM->predchozi->predchozi->predchozi->T2=T;//dodá ještě do přechozího kola dobu čekání do otevření stop
		double WT1=wt();//zjištění výstupního WT, možno RAND
		d.v.vloz_SIM(T,T+WT1,0,gS(indexStopV1),V1);//začátek čekání na palec, po čekání na stop - OTEVŘENO
		T+=WT1;d.v.vloz_SIM(T,T,1,gS(indexStopV1),V1);//uchycení na palec - START
			double WT2=wt();//zjištění výstupního WT, možno RAND
			if(i>0)d.v.SIM->predchozi->predchozi->predchozi->predchozi->T2=T+L;//dodá ještě do přechozího kola dobu čekání do otevření stop
			d.v.vloz_SIM(T+L,T+L+WT2,0,gS(indexStopV2),V2);//začátek čekání na palec,  - OTEVŘENO
			d.v.vloz_SIM(T+L+WT2,T+L+WT2,1,gS(indexStopV2),V2);//uchycení na palec - START
						double WT3=wt();//zjištění výstupního WT, možno RAND
						if(i>0)d.v.SIM->predchozi->predchozi->predchozi->predchozi->predchozi->T2=T+L+WT2+L;//dodá ještě do přechozího kola dobu čekání do otevření stop
						d.v.vloz_SIM(T+L+WT2+L,T+L+WT2+L+WT3,0,gS(indexStopV3),V3);//začátek čekání na palec,  - OTEVŘENO
						d.v.vloz_SIM(T+L+WT2+L+WT3,T+L+WT2+L+WT3,1,gS(indexStopV3),V3);//uchycení na palec - START
		//posun indexu stopek
		if(indexStopV1==nSTOP)indexStopV1=1;else indexStopV1++;
			if(indexStopV2==nSTOP)indexStopV2=1;else indexStopV2++;
				if(indexStopV3==nSTOP)indexStopV3=1;else indexStopV3++;
		//přejezd
		d.v.vloz_SIM(T,T+MT,2,gS(indexStopV1),V1);
			d.v.vloz_SIM(T+L+WT2,T+L+WT2+MT,2,gS(indexStopV2),V2);
				d.v.vloz_SIM(T+L+WT2+L+WT3,T+L+WT2+L+WT3+MT,2,gS(indexStopV3),V3);
		//dokončení přejezdu - STOP
		T+=MT;d.v.vloz_SIM(T,T,-1,gS(indexStopV1),V1);
			d.v.vloz_SIM(T+L+WT2,T+L+WT2,-1,gS(indexStopV2),V2);
				d.v.vloz_SIM(T+L+WT2+L+WT3,T+L+WT2+L+WT3,-1,gS(indexStopV3),V3);
				//T=T+L+WT2+L+WT3;
		/*zde vzniká plovoucí aRT do dalšího cyklu tj. přechod do dalšího kola*/
	}
}
//---------------------------------------------------------------------------
//testovácí metody simulace
void TForm1::provedScenar()
{
	short Z=1;//stav zvukových efektu 1-spuštěno
	Cvektory::TSIM *sim=d.v.SIM->dalsi;
	while(sim!=NULL)
	{
		//nastala okamžitá časová událost (nikoliv přejezd)
		if(m.round2double(sim->T1,1)==m.round2double(CAS,1) /*&& m.round2double(sim->T2,1)==m.round2double(CAS,1) || CAS==0*/)
		{
			String A="";//popis akce
			switch(sim->A)//vyháknutý -1, čeká na palec 0, jede 1
			{
				case -1: sim->S->stav=1; sim->V->stav=-1; sim->V->element=sim->S; sim->V->X=sim->S->X;sim->V->Y=sim->S->Y; A="STOP";break;//zastaven na stop stanici
				case 0:  sim->S->stav=0; sim->V->stav=0;  sim->V->element=sim->S; sim->V->X=sim->S->X;sim->V->Y=sim->S->Y; if(CAS>=d.v.PP.TT)Memo(sim->S->name+" TT (otevřeno): "+String(m.round2double(CAS-sim->S->temp,2))+"[s]");sim->S->temp=CAS; A="OTEVŘENO";break;//čekání na palec, otevření stop stanice
				case 1:  sim->S->stav=1; sim->V->stav=1;  sim->V->element=sim->S; sim->V->X=sim->S->X;sim->V->Y=sim->S->Y; if(CAS>=d.v.PP.TT)Memo(sim->S->name+" TT (start): "+String(m.round2double(CAS-sim->S->temp2,2))+"[s]");sim->S->temp2=CAS; A="START";break;//uchycen na palec a zavření stop stanice
			}
			//výpis
			if(A!="")Memo(sim->S->name+"\tV"+String(sim->V->n)+"\t"+String(sim->T1)+" [s]\t"+A);//přejezd se navypisuje
			//zvuk na vlákně, jinak by brzdil simulaci
			if(Z)PlaySound(sim->A);
		}
		//přejezd
		if(sim->A==2 && sim->T1<CAS && CAS<sim->T2)
		{
			sim->V->stav=1;sim->V->element=sim->S;
			double PR=(CAS-sim->T1)/(sim->T2-sim->T1);
			Cvektory::TElement *E=sim->V->element;
			TPointD_3D Pt=m.getPt(E->geo.radius,E->geo.orientace,E->geo.rotacni_uhel,E->geo.X1,E->geo.Y1,E->geo.X4,E->geo.Y4,PR,PR/*umisteni/E->delka,(umisteni+PP.uchyt_pozice-PP.delka_podvozek/2.0)/E->delka*/);//zjištění aktuálních souřadnic vozíků
			sim->V->X=Pt.x;sim->V->Y=Pt.y;
			E=NULL;delete E;
		}
		//posun na další
		sim=sim->dalsi;
	}
	delete sim;
}
//---------------------------------------------------------------------------
//přehrání zvuku simulace na samostatném vlákně
void TForm1::PlaySound(short A)
{
	TSound *vlakno=new TSound(true);//spustí vlákno
	vlakno->FreeOnTerminate=true;//po skončení bude uvolněno
	vlakno->A=A;
	vlakno->Start();
}
//---------------------------------------------------------------------------
Cvektory::TElement *TForm1::gS(short n)
{
	switch (n)
	{
		case 1: return d.v.ELEMENTY->dalsi;break;
		case 2: return d.v.ELEMENTY->dalsi->dalsi;break;
		case 3: return d.v.ELEMENTY->dalsi->dalsi->dalsi;break;
	}
}
//---------------------------------------------------------------------------
//provizorní metoda
double TForm1::wt()
{
	double WTmax=7;//konstantní, maximální
	//return m.RAND(0,WTmax);
	return WTmax;
}
//---------------------------------------------------------------------------
unsigned int TForm1::getV()
{
	unsigned int RET=0;
	if(d.v.SIM!=NULL && d.v.VOZIKY!=NULL && d.v.VOZIKY->dalsi!=NULL)
	{
		unsigned int V=ceil((akt_souradnice_kurzoru_PX.Y+d.PosunT.y-d.KrokY/2.0/3.0-scGPPanel_mainmenu->Height-5)/(d.KrokY*1.0/3.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
		if(V<=d.v.VOZIKY->dalsi->n)RET=V;
	}
	return RET;
}
//---------------------------------------------------------------------------
//provede jeden takt multimediálního TimeruSimulace - nevyužito, náročné na synchronizaci vykreslovaných dat, zachováno jako vzor
void CALLBACK TimerSimulaceEvent(UINT wTimerID, UINT msg,	DWORD dwUser, DWORD dw1, DWORD dw2)
{
	//F->d.v.posun_palce_retezu();
	//F->sTIME+=1;
	//if((int)F->sTIME%2)
	//F->Canvas->Draw(F->sTIME,0,F->Pan_bmp);
	//else F->Canvas->Draw(0,F->sTIME,F->Pan_bmp_ALL);
	//->d.set_pen2(F->Canvas,clRed,F->d.m.round(1*F->Zoom),PS_ENDCAP_SQUARE,PS_JOIN_MITER,true);
	//F->d.line(F->Canvas,0,500,0+F->sTIME,500);
	//mám problém s vykreslením této linie ve d.vykresli_palce, což se volá pomocí níže zakomentovaného F->REFRESH();
	//F->REFRESH();
}
//---------------------------------------------------------------------------
//ukončí daný (dlew TimerID) timer  - nevyužito, náročné na synchronizaci vykreslovaných dat, zachováno jako vzor
void TForm1::TimerKill(UINT wTimerID)
{
//	timeKillEvent(wTimerID);//ukončí timer
//	timeEndPeriod(TimerPresnost);//odstranění požadovaného rozlišení ze systému, pokud bylo použito timeBeginPeriod(presnost)
}
//---------------------------------------------------------------------------
//MaVL - testovací tlačítko
void __fastcall TForm1::ButtonMaVlClick(TObject *Sender)
{
///////////////////////KÓD PRO STOPKY/////////////////////////////
//	TDateTime start;
//	String s;
//	double cas=0,celkem_otevreni=0,celkem_zavreni=0;
//	unsigned int pocet_kroku=10;
//	for(unsigned int i=0;i<pocet_kroku;i++)
//	{
//		start=Now();
//		pom=d.v.OBJEKTY->dalsi;//predchozi->predchozi->predchozi;
//		otevri_editaci();
//		cas=ms.MyToDouble(TimeToStr(Now()-start).SubString(6,2));
//		celkem_otevreni+=cas;
//		Memo("Čas otevření: "+AnsiString(cas));
//		start=Now();
//		scGPButton_stornoClick(this);
//		cas=ms.MyToDouble(TimeToStr(Now()-start).SubString(6,2));
//		celkem_zavreni+=cas;
//		Memo("Čas zavření: "+AnsiString(cas));
//	}
//	Memo("------------");
//	Memo("Průměrný čas otevření: "+AnsiString(celkem_otevreni/(double)pocet_kroku));
//	Memo("Průměrný čas zavření: "+AnsiString(celkem_zavreni/(double)pocet_kroku));
///////////////////////KÓD PRO SPOJOVÁNÍ OBLOUKŮ/////////////////////////////
//	Cvektory::TElement *e_posledni=d.v.ELEMENTY->predchozi->predchozi->predchozi2;
//	e_posledni->geo.rotacni_uhel+=-45;
//	e_posledni->geo.delka=m.R2Larc(e_posledni->geo.radius,e_posledni->geo.rotacni_uhel);
//	TPointD *PL=m.getArcLine(e_posledni->geo.X1,e_posledni->geo.Y1,e_posledni->geo.orientace,-90,e_posledni->geo.radius);
//	e_posledni->geo.X1=PL[0].x;e_posledni->geo.Y1=PL[0].y;e_posledni->geo.X2=PL[1].x;e_posledni->geo.Y2=PL[1].y;e_posledni->geo.X3=PL[2].x;e_posledni->geo.Y3=PL[2].y;e_posledni->geo.X4=PL[3].x;e_posledni->geo.Y4=PL[3].y;
//	vytvor_statickou_scenu();
//	REFRESH();
//  e_posledni=NULL;delete e_posledni;
	Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
	while(E!=NULL)
	{
    Memo("Orientace: "+String(E->geo.orientace)+"; Rt: "+String(m.round(E->geo.orientace/15.0)*15));
		E=E->dalsi;
	}
	delete E;E=NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//MaKr testovací tlačítko
void __fastcall TForm1::ButtonMaKrClick(TObject *Sender)
{//vždy nechat tento komentář

Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
	while(E!=NULL)
	{
		//Memo(E->name);
		if(E->n==14)
		{
			Memo(String(E->citelna_oblast.rect3.left)+" "+String(E->citelna_oblast.rect3.top)+" "+String(E->citelna_oblast.rect3.right)+" "+String(E->citelna_oblast.rect3.bottom));
			//Canvas->FillRect(TRect(E->citelna_oblast.rect3.left*3,E->citelna_oblast.rect3.top*3,E->citelna_oblast.rect3.right*3,E->citelna_oblast.rect3.bottom*3));
			//REFRESH();
		}
		E=E->dalsi;
	}
	delete E;


//	Cvektory::TPohon *P=d.v.POHONY->dalsi;
//	while(P!=NULL)
//	{
//		d.v.vytvor_retez(P);//a palce
//		P=P->dalsi;
//	}
//	delete P;
//
//			MOD=SIMULACE;//mód musí být před vytvořením scény
//			d.SCENA=221111;//zprávy nejsou zobrazeny, vozíky a aktivní části elementů jsou v dynamické scéně
//			vytvor_statickou_scenu();//načtení statických záležitostí do statické scény, musí být před REFRESH
//	REFRESH();

//Timer1->Enabled=!Timer1->Enabled;


	//	ShowMessage(sizeof(unsigned char));
//	ShowMessage(sizeof(unsigned short));
//	ShowMessage(sizeof(Trgb));
//	ShowMessage(sizeof(PRGBTriple));
//		ShowMessage(sizeof(Cvektory::TmyPx));

//	START();
//	for(UINT i=0;i<500;i++)d.v.vloz_do_BUFFERU();
//	STOP();
//
//	d.v.sBUFFER->predchozi->raster->SaveToFile("test_posledni"+String(d.v.sBUFFER->predchozi->n)+".png");
//	d.v.sBUFFER->dalsi->raster->SaveToFile("test_posledni"+String(d.v.sBUFFER->dalsi->n)+".png");
//	Memo(get_AvailPhysMemory());



//

////
//	Graphics::TBitmap *bmp_out=d.v.dekomprimace(d.v.komprimace(bmp_in),bmp_in->Width,bmp_in->Height);
//
//	bmp_out->SaveToFile("TEST_DEKOMPRIMACE.bmp");
//	delete bmp_out;
//
//	d.v.rast_do_souboru(d.v.komprimace(bmp_in),"TEST.KOMPRIMACE");
//	delete bmp_in;
	//Staticka_scena->SaveToFile("SSoriginal.bmp");
//	STOP();

	//Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	//bmp_in->Assign(Staticka_scena);
//	Cvektory::TmyPx *px;
//
//	TRect oblast=vrat_max_zobrazitelnou_oblast();
//	bool resize=true;
//	//int W=bmp_in->Width;int H=bmp_in->Height;
//	int W=ClientWidth;int H=ClientHeight;
//	if(resize){W=oblast.Right-oblast.Left;H=oblast.Bottom-oblast.Top;}
//
//
//
//	START();
//	for(UINT i=0;i<600;i++)
//	{
//		Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
//		bmp_in->Width=Width;
//		bmp_in->Height=Height;
//		bmp_in->PixelFormat=pf24bit;
//		//simulování přidávání dynamické scény
//		//bmp_in->Assign(Staticka_scena);//nutné vynulování (či nahradit alternativou a přiřazení podkladové statické sceny ale zpomaluje o 1-2 s
//		bmp_in->Canvas->Brush->Style=bsClear;
//		bmp_in->Canvas->Font->Size=150;
//		bmp_in->Canvas->TextOutW(0+i,100,i);
//		//komprese dat
//		//px=d.v.komprese(bmp_in,oblast,resize);
//		Graphics::TBitmap *bmp_out=d.v.dekomprese(d.v.komprese(bmp_in,oblast,resize),W,H);
//		delete bmp_in;
//		//bmp_out->SaveToFile("TEST_OBlast"+String(i)+".bmp");
//
//		Graphics::TBitmap *bmp_t=new Graphics::TBitmap;
//		bmp_t->Assign(Staticka_scena);
//		bmp_t->Canvas->Draw(0,0,bmp_out);//extrémně pomalá metoda
//		delete bmp_out;
//		//bmp_t->SaveToFile("TEST_OBlast"+String(i)+".bmp");
//	}
//	STOP();

//	ZÁVĚR PŮSOBÍ TAKTO: AKT. DYNAMICKOU SCENU KRESLIT DO KOPIE STATICKÉ SCENY A TU KOMPRIMOVAT DO BUFFERU A NESLEDNĚ PO DEKOMPRIMACI POUZE PŘEHRÁVAT
//	BOHUŽEL UKLADAT JEN DYNAMICKOU SCENU BY BYLO VHODNĚJŠÍ, ALE POMALOST PŘI POUŽTÍ DRAW PO DEKOMPIMACI DYNAMICKÉ SCENY DO STATICKÉ JE DOST ČASOVĚ NÁKLADNÉ
//	POPŘ.NAJÍT JEŠTĚ JINOU VYKRESLOVACÍ METODU DO BMP, RESP. NA SLOUČENÍ DVOU BMP
//	???CO TOTO DO DEKOMPRESE POSLAT KOPII STATICKÉ, DO KTERÉ SE PŘI SCANLINE NAPÍŠÍ JEDNOTLIVÉ PIXELY DYNAMICKÉ SCENY, POUZE DYNAMICKÁ BUDE ULOŽENA V BUFFERU

	//bmp_in->SaveToFile("TEST_BMPIN.bmp");
	//delete bmp_in;


//	START();
//	for(UINT i=0;i<600;i++)
//	{
//		Graphics::TBitmap *bmp_out=d.v.dekomprese(px,W,H);
//		//bmp_out->SaveToFile("TEST_OBlast.bmp");
//		//bmp_out->SaveToFile("TEST_OBlast"+String(i)+".bmp");
//		delete bmp_out;
//	}
//	STOP();


//			TMetafile *Metafile = new TMetafile;
//			//Metafile->Width=MaxX;
//			//Metafile->Height=MaxY;
//																																		//Staticka_scena->Canvas->Handle
//			TMetafileCanvas* MetafileCanvas = new TMetafileCanvas(Metafile,0);
//			//Vykresli(MetafileCanvas);
//			MetafileCanvas->Draw(0,0,Staticka_scena);
//			delete MetafileCanvas;
//
//			Metafile->Enhanced=true;
//			Metafile->SaveToFile("testMeteFile.emf");
//			delete Metafile;

//Staticka_scena->SaveToFile("test_bmp.bmp");
//
//
//		TJPEGImage *Jpeg = new TJPEGImage();
//		Jpeg->Assign(Staticka_scena);
//		Jpeg->SaveToFile("test_jpg.jpg");
//
//		TPngImage *Png = new TPngImage;
//		Png->Assign(Staticka_scena);
//		Png->SaveToFile("test_png.png");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
//	if(get_AvailPhysMemory()/(get_TotalPhysMemory()*1.0)<0.3)//ukončí plnění bufferu při dosažení 30% operační paměti
//	{
//		Timer1->Enabled=false;
//		Memo("Dosaženo maximum "+String(get_AvailPhysMemory()/(get_TotalPhysMemory()*1.0)));
//		Memo(get_AvailPhysMemory());
//		Memo(get_TotalPhysMemory());
//	}
//	else
//	{
//		d.v.vloz_do_BUFFERU();
//		Memo(String(d.v.sBUFFER->predchozi->n)+" "+String(get_AvailPhysMemory()));
//	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxVymena_barev_Click(TObject *Sender)
{
//	log(__func__);//logování
//	d.JIZPOCITANO=false;//zajistí přepočet časových os
//	Invalidate();
//	RM();//korekce chyby oskakování pravého menu
}
//-------------------------------------------------------------
//zapne či vypne antialiasing
void __fastcall TForm1::scGPSwitch_AAChangeState(TObject *Sender)
{
	log(__func__);//logování
	antialiasing=!antialiasing;//musí být před scSplitView_MENU->Opened!!!
	scSplitView_MENU->Opened=false;
	DrawGrid_knihovna->Invalidate();
	Invalidate();//v tomto případě lépe než REFRESH - kvůli efektu
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_trTimer(TObject *Sender)
{
	log(__func__);//logování
	//ShowMessage("hu");
	if(!ttr("TimerTr")){Timer_tr->Enabled=false;Close();}//kontrola zda nevypršela trial verze
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SQL_processIDClick(TObject *Sender)
{
	log(__func__);//logování
	FDQuery1->Active = False;
	FDQuery1->Open("select * from app_setup where id=\"1\"");  //id nahradit id z ini a udelat podmínku zda platí lokální údaje o pc s uloženými
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



void __fastcall TForm1::MaxButtonClick(TObject *Sender)
{
	log(__func__);//logování
	if (FMaximized)//zmenšení
	{
		//BoundsRect =  FOldBoundsRect;
		FMaximized = false;
		scLabel_titulek->DragForm = true;
		MaxButton->GlyphOptions->Kind = scgpbgkMaximize;
		if(OBJEKT_akt==NULL){if(!d.v.PP.zamek_layoutu)scGPButton_prichytavat->Visible=false;scButton_zamek_layoutu->Visible=false;scGPButton_zmerit_vzdalenost->Visible=false;scButton_zamek_layoutu->Align=TAlign::alNone;scGPButton_zmerit_vzdalenost->Align=TAlign::alNone;if(!d.v.PP.zamek_layoutu)scGPButton_prichytavat->Align=TAlign::alNone;}
		scGPSizeBox->Visible = true;
		if(OBJEKT_akt==NULL){scButton_zamek_layoutu->Visible=true;scGPButton_zmerit_vzdalenost->Visible=true;if(!d.v.PP.zamek_layoutu)scGPButton_prichytavat->Visible=true;scButton_zamek_layoutu->Align=TAlign::alRight;scGPButton_zmerit_vzdalenost->Align=TAlign::alRight;if(!d.v.PP.zamek_layoutu)scGPButton_prichytavat->Align=TAlign::alRight;}
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
  //přizpůsobení velikosti statusbaru a rozdělení sekcí
	int width=scButton_zamek_layoutu->Width+scGPButton_zmerit_vzdalenost->Width+scLabel_statusbar_2->Left+scLabel_statusbar_2->Width;
	if(!d.v.PP.zamek_layoutu)width+=scGPButton_prichytavat->Width;
	if(scGPSizeBox->Visible)width+=scGPSizeBox->Width;
	width=scGPPanel_statusbar->Width-width-1;
	if(width!=0){scLabel_statusbar_2->Width+=width;Image_rozdelovac_3->Left+=width;}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_OPTIONS_OldClick(TObject *Sender)
{
	log(__func__);//logování
	ESC();//zruší případnou rozdělanou akci
	scSplitView_OPTIONS->Opened = !scSplitView_OPTIONS->Opened;
	if(scSplitView_MENU->Opened)scSplitView_MENU->Close();
	//if(d.v.PP.raster.show==true) scGPCheckBox_zobraz_podklad->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::KonecClick(TObject *Sender)
{
	log(__func__);//logování
	//zalogování vypnutí aplikace
//	AnsiString relation_id=GetCurrentProcessId();
//	AnsiString send_log_time= TIME.CurrentDateTime();
//	AnsiString ID ="1";
//	AnsiString Text="aplikace stop";
//	AnsiString strSQL = "INSERT INTO app_log (app_id,app_start,username,send_log_time,command,relation_id) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\")";
//	FDConnection1->ExecSQL(strSQL);
	int vysledek;
	if (MOD==EDITACE)
	{
		if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
		{
			if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
			if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
			if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
			if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
		}
		if(scGPButton_ulozit->Enabled)
		{
			vysledek=MB(ls->Strings[343]+" "+OBJEKT_akt->name+"?",MB_YESNO,true);//"Chcete uložit změny objektu"
			switch (vysledek)
			{
				case mrYes:scGPButton_OKClick(Sender);break;
				case mrNo:scGPButton_stornoClick(Sender);break;
			}
		}
		else scGPButton_stornoClick(Sender);
	}
	else if(MOD==TVORBA_CESTY)
	{
    scGPButton_stornoClick(this);
  }
	else
	{
		scButton_konec->Down=false;
		Close();//ukončí aplikaci
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_ZOOM_MINUSClick(TObject *Sender)
{
	log(__func__);//logování
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	if(Zoom>0.25)//akcelerátor, aby se zbytečně nevolalo
	{
		log(__func__);//logování
		vycentrovat=false;
		akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
		//fce. ZOOM_OUT, fce. není volaná a to z důvodu opužití jiného kroku zde a jiného kroku v ostatních případech
		if(Zoom>0.5)
   	{
   		Uloz_predchozi_pohled();
   		if(Zoom>10) Zoom-=5.0;
   		else if(Zoom>5) Zoom-=1.0;
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
		//konec fce.
		zobraz_tip(ls->Strings[299]);//"TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu."
	}
	else MessageBeep(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_ZOOM_PLUSClick(TObject *Sender)
{
	log(__func__);//logování
	//pokud je otevřené menu nebo options, zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	if((Zoom<20 && !DEBUG) || (Zoom<30 && DEBUG))//akcelerátor, aby se zbytečně nevolalo
	{
		log(__func__);//logování
		vycentrovat=false;
		akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
		ZOOM_IN();
		zobraz_tip(ls->Strings[298]);//"TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru."
	}
	else MessageBeep(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scLabel_titulekDblClick(TObject *Sender)
{
  log(__func__);//logování
  //zajistí maximalizaci či zmenšení okna po double clicku na titulek
	MaxButtonClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MinButtonClick(TObject *Sender)
{
  log(__func__);//logování
	Application->Minimize();
}
//---------------------------------------------------------------------------
//volání parametrů linky nebo definice zakázek - dle zvoleného režimu
void __fastcall TForm1::Button_dopravnik_parametryClick(TObject *Sender)
{
	log(__func__);//logování
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	Layout->Down=false;//musí se uvlonit po stisku
	if(MOD!=EDITACE/*||OBJEKT_akt->pohon==NULL*/)
	{
		ESC();//zruší případnou rozdělanou akci
		Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
		Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
		Layout->Options->NormalColor=DetailsButton->Options->NormalColor;
		Layout->Options->FrameNormalColor=DetailsButton->Options->NormalColor;
		Schema->Options->NormalColor=scGPPanel_mainmenu->FillColor;
		Schema->Options->FrameNormalColor=scGPPanel_mainmenu->FillColor;
		Form_parametry_linky->ShowModal();//návratová hodnota se řeši v knihovně
		REFRESH();
	}
	else
	{
		KonecClick(this);
		if(OBJEKT_akt==NULL)//došlo k ukončení náhledu
		{
			Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
			Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
			Layout->Options->NormalColor=DetailsButton->Options->NormalColor;
			Layout->Options->FrameNormalColor=DetailsButton->Options->NormalColor;
			Schema->Options->NormalColor=scGPPanel_mainmenu->FillColor;
			Schema->Options->FrameNormalColor=scGPPanel_mainmenu->FillColor;
			Form_parametry_linky->ShowModal();//návratová hodnota se řeši v knihovně
		}
	}
}
//---------------------------------------------------------------------------
//volání superformuláře (definice zakázek)
void __fastcall TForm1::scGPGlyphButton_definice_zakazekClick(TObject *Sender)
{
	log(__func__);//logování
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
	log(__func__);//logování
	ESC();//zruší případnou rozdělanou akci
	DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
	scSplitView_MENU->Opened = !scSplitView_MENU->Opened;
	DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
	if(scSplitView_OPTIONS->Opened) scSplitView_OPTIONS->Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//při změnně palce na tracBaru aktualizuje zoom a zavolá překreslení obrazu
void __fastcall TForm1::scGPTrackBar1Change(TObject *Sender)
{
	log(__func__);//logování
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	switch(scGPTrackBar1->Value)
	{
		case 1:Zoom=0.25;break;
		case 2:Zoom=0.5;break;
		case 3:Zoom=1;break;
		case 4:Zoom=1.5;break;
		case 5:Zoom=2;break;
		case 6:Zoom=2.5;break;
		case 7:Zoom=3;break;
		case 8:Zoom=3.5;break;
		case 9:Zoom=4;break;
		case 10:Zoom=4.5;break;
		case 11:Zoom=5;break;
		case 12:Zoom=6;break;
		case 13:Zoom=7;break;
		case 14:Zoom=8;break;
		case 15:Zoom=9;break;
		case 16:Zoom=10;break;
		case 17:Zoom=15;break;
		case 18:Zoom=20;break;
		case 19:Zoom=25;break;
		case 20:Zoom=30;break;
	}
	scLabel_ZOOM->Caption=AnsiString(Zoom*100)+" %";

  //zobrazení a skrytím mgridů při konkrétním zoomu
	//if(OBJEKT_akt!=NULL && ((Zoom<=2.5 && OBJEKT_akt->zobrazit_mGrid) || (Zoom>=3 && !OBJEKT_akt->zobrazit_mGrid)))scGPButton_viditelnostmGridClick(this);

	if(refreshovat_scGPTrackBar)
	{
    if(MOD==LAYOUT)vytvor_statickou_scenu();//aktualizace BMP statické scény, změna ZOOMu
		REFRESH();
	}
	else refreshovat_scGPTrackBar=true;

  //překrytí tabulek
	if(OBJEKT_akt!=NULL && OBJEKT_akt->zobrazit_mGrid)mGrid_on_mGrid();
}
//---------------------------------------------------------------------------
//při změně zoomu aktulizuje palec na trackBaru, inverzní funkce k funkci předchozí
void TForm1::on_change_zoom_change_scGPTrackBar()
{
	log(__func__);//logování
	refreshovat_scGPTrackBar=false;//aby nedocházelo následnou onchange trackbaru (k metodě výše uvedené) k neřízenému a nadbytečnému refreshování
	if(Zoom==0.25)	scGPTrackBar1->Value=1;
	if(Zoom==0.5)		scGPTrackBar1->Value=2;
	if(Zoom==1)			scGPTrackBar1->Value=3;
	if(Zoom==1.5)		scGPTrackBar1->Value=4;
	if(Zoom==2)			scGPTrackBar1->Value=5;
	if(Zoom==2.5)		scGPTrackBar1->Value=6;
	if(Zoom==3)			scGPTrackBar1->Value=7;
	if(Zoom==3.5)		scGPTrackBar1->Value=8;
	if(Zoom==4)			scGPTrackBar1->Value=9;
	if(Zoom==4.5)		scGPTrackBar1->Value=10;
	if(Zoom==5)			scGPTrackBar1->Value=11;
	if(Zoom==6)		  scGPTrackBar1->Value=12;
	if(Zoom==7)		  scGPTrackBar1->Value=13;
	if(Zoom==8)		  scGPTrackBar1->Value=14;
	if(Zoom==9)		  scGPTrackBar1->Value=15;
	if(Zoom==10)		scGPTrackBar1->Value=16;
	if(Zoom==15)		scGPTrackBar1->Value=17;
	if(Zoom==20)		scGPTrackBar1->Value=18;
	if(Zoom==25)		scGPTrackBar1->Value=19;
	if(Zoom==30)		scGPTrackBar1->Value=20;
	scLabel_ZOOM->Caption=String(Zoom*100)+" %";

	//překrytí tabulek
	if(OBJEKT_akt!=NULL && OBJEKT_akt->zobrazit_mGrid)mGrid_on_mGrid();
	//zobrazení a skrytím mgridů při konkrétním zoomu
	//if(OBJEKT_akt!=NULL && ((Zoom<=2.5 && OBJEKT_akt->zobrazit_mGrid) || (Zoom>=3 && !OBJEKT_akt->zobrazit_mGrid)))scGPButton_viditelnostmGridClick(this);
}
//---------------------------------------------------------------------------
//přepínání režimu aplikace návrh ověřování
void __fastcall TForm1::scGPSwitch_rezimChangeState(TObject *Sender)
{
	log(__func__);//logování
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
		SB(ls->Strings[395],1);//"OVĚŘOVÁNÍ"
	 //	ShowMessage(STATUS);

			// scGPButton_header_def_zakazek->Visible=true;
			// scGPButton_header_param_linky->Visible=false;
		 //	scGPButton_header_projekt->ImageIndex=48;
			//scGPButton_header_projekt->Hint="Definice zakázek";


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

//		scGPButton_header_projekt->ImageIndex=49;
//		scGPButton_header_projekt->Hint="Parametry linky";


		SB(ls->Strings[396],1);//"NÁVRH"
	 //	ShowMessage(STATUS);
	}
}
//---------------------------------------------------------------------------
//pokud je při zavírání okna aktivní AA, tak udělá kopii pracovní oblasti AA vypne a zavře okno, bez toho by se zavíralo trhaně
void __fastcall TForm1::scSplitViewsClosing(TObject *Sender)
{
  log(__func__);//logování
	scSplitViews_closing_on_AA=false;
	SplitViewOpen=true;
	if(antialiasing)
	{
		scSplitViews_closing_on_AA=true;
		int W=scSplitView_LEFTTOOLBAR->Width;
		short H=scLabel_titulek->Height;
		Graphics::TBitmap *bmp_OPT_CLOSE=new Graphics::TBitmap;
		bmp_OPT_CLOSE->Width=ClientWidth-W;bmp_OPT_CLOSE->Height=ClientHeight-H;
		bmp_OPT_CLOSE->Canvas->CopyRect(Rect(0,0,ClientWidth-W,ClientHeight-H),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight));//zkopíruje aktuální výřez
		antialiasing=false;
		Canvas->Draw(0+W,0+H,bmp_OPT_CLOSE);
		delete (bmp_OPT_CLOSE);//velice nutné
	}
}
//---------------------------------------------------------------------------
//pokud byl AA zapnutý, tak navrácení do původního stavu na true
void __fastcall TForm1::scSplitViewsClosed(TObject *Sender)
{
  log(__func__);//logování
	if(scSplitViews_closing_on_AA)antialiasing=true;
	SplitViewOpen=false;
	if(MOD==CASOVAOSA || TECHNOPROCESY)REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scExPanel_vrstvyClick(TObject *Sender)
{
	log(__func__);//logování
	scExPanel_vrstvy->RollUpState=!scExPanel_vrstvy->RollUpState;
}
//---------------------------------------------------------------------------
//vypnutí měřítko
void __fastcall TForm1::scGPSwitch_meritkoChangeState(TObject *Sender)
{
  log(__func__);//logování
	scSplitView_MENU->Opened=false;
	zobrazit_meritko=scGPSwitch_meritko->State;
	REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scSplitView_OPTIONSMouseLeave(TObject *Sender)
{
	log(__func__);//logování
//    scSplitView_OPTIONS->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scSplitView_MENUMouseLeave(TObject *Sender)
{
	log(__func__);//logování
//   scSplitView_MENU->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton2Click(TObject *Sender)
{
//	log(__func__);//logování
//	scSplitView_MENU->Close();//zavře menu
//	Novy_soubor();//prvně zavřu starý, který ho donutím uložit a vytvořím nový soubor
//	//Form_eDesigner->Show();
//	AnsiString FileNameSablona="sablona.tisplTemp";
//
//	//pokud šablona neexistuje, zkusí ji stáhnout
//	//mohla by být v onshow v uvodu
//	if(!FileExists(ExtractFilePath(Application->ExeName)+FileNameSablona))
//	{
//		try
//		{
//			zobraz_tip(ls->Strings[302]);//"Probíhá stahování šablony..."
//			TMemoryStream *MemoryStream=new TMemoryStream();
//			IdHTTP1->Get("http://85.255.0.237/TISPL/"+FileNameSablona,MemoryStream);
//			MemoryStream->SaveToFile(ExtractFilePath(Application->ExeName)+FileNameSablona);//stáhne k souboru exe
//		}
//		catch(...){;}//v případě nedostupnosti internetu atp.
//	}
//
//	//pokud soubor šablony existuje
//	if(FileExists(ExtractFilePath(Application->ExeName)+FileNameSablona))
//	{
//		UnicodeString T1,T2,T3,T4;
//		T1=ls->Strings[397];//"Následně bude otevřena šablona. Zadejte název projektu, pod kterým bude uložena."
//		T2=ls->Strings[398];//"Zadejte parametry linky nebo zvolte storno."
//		T3=ls->Strings[403];//"Zadejte parametry zakázek nebo zvolte storno."
//		T4=ls->Strings[399];//"Vytvořte schéma a pomocí pravého tlačítka nastavte parametry objektů."
//
//		MB(T1);SB(T1);
//		Otevrit_soubor(ExtractFilePath(Application->ExeName)+FileNameSablona); //volaný *.tispl musí být tam, kde exe tisplu nebo zadat adresu
//		DuvodUlozit(true);
//		UlozitjakoClick(this);
//
//		SB(T2);//zobraz_tip(T2);
//		Button_dopravnik_parametryClick(Sender);//volání parametrů linky
//
//		//MB(T3);
//		SB(T3);//zobraz_tip(T3);
//		scGPGlyphButton_definice_zakazekClick(Sender);//volání superformuláře (definice zakázek)
//
//		SB(T4);zobraz_tip(T4);MB(T4,MB_OK,false);
//
//		DuvodUlozit(true);
//	}
//	else//nebyla nalezena šablona
//	{
//		MB(ls->Strings[353]);//"Nepodařilo se najít nebo stáhnout šablonu. Průvodce nebude spuštěn!"
//	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBoxCekaniChange(TObject *Sender)
{
	log(__func__);//logování  // R 3.6.2020 asi ke smazani
	//pokud bych chtěl používat i v v modu vytzení objektu, ale při znovu výberu náhodne hodnoty házelo chybu:if(d.mod_vytizenost_objektu){d.JIZPOCITANO=false;d.mod_vytizenost_objektu=false;d.vykresli_casove_osy(Canvas);d.mod_vytizenost_objektu=true;}
//	if(ComboBoxCekani->ItemIndex==2)//tak se zobrazí vedle toho comba tlačítko pro nové generování
//	{
//		scGPButton_generuj->Visible=true;
//		ComboBoxCekani->Width=196;
//	}
//	else
//	{
//		Form1->scGPButton_generuj->Visible=false;
//		ComboBoxCekani->Width=scSplitView_OPTIONS->Width-7;
//	}
//	//pro uživatele kontrola, zda mají objekty přiřazené pohony a pohony, zda mají přiřazené rozteče
//	if(ComboBoxCekani->ItemIndex>0)
//	{
//		//kontrola přiřazení pohonů + nabídky na nápravu (pouze ve statusu NAVRH)
//		UnicodeString T=d.v.vypsat_objekty_bez_prirazenych_pohonu(),text,text_1,text_2,text_3;
//		text=ls->Strings[354]+" ";//"Pozor, pro objekt "
//		text_1=" "+ls->Strings[355]+"</b><b>"+ls->Strings[356]+"</b>";//" nebyl přiřazen pohon. Doba čekání na palce není u těchto objektů zohledněna!<br><b>Chcete pohony objektům přiřadit nyní?</b>"
//		text_2=ls->Strings[357]+" ";//"Pozor, následující pohony nemají uvedenou rozteč palců řetezu: "
//		text_3=". "+ls->Strings[358]+"<br><b>"+ls->Strings[359]+"</b>";//". Doba čekání na palce není tedy u uvedených objektů zohledněna!<br><b>Chcete chybějící rozteče pohonů nyní zadat?</b>"
//		if(T!="")
//		{
//			if(mrYes==MB(text+T+text_1,MB_YESNO) && STATUS==NAVRH)
//			{
//				Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
//				while(O!=NULL)
//				{
//					if(O->pohon==NULL)
//					{
//						MessageBeep(0);
//						pom=O; //R - 21.1.2020 - ZAKOMENTOVÁNO, VAZBA NA STARÉ PO - ODEBRÁNO Z PROJEKTU
////						if(Form_parametry->ClientHeight==646){int H=366;if(O->rezim==1)H=526;if(O->rezim==1)H=486;Form_parametry->ClientHeight=H;}//pokud se jedná o první spuštění, protože jinak je neznámá výška formu
////						akt_souradnice_kurzoru_PX.x=Form1->ClientWidth/2-Form_parametry->ClientWidth/2-10;
////						akt_souradnice_kurzoru_PX.y=Form1->ClientHeight/2-Form_parametry->ClientHeight/2-10;
////						NP();//volá formulář parametry objektů pro přiřazení pohonu ke konkrétnímu objektu//volá form na nastevení parametrů, dřívější nastavparemetry1click
//					}
//					O=O->dalsi;
//				}
//				O=NULL;delete O;
//			}
//		}
//		//kontrola přiřazení rozteče + nabídka na nápravu (pouze ve statusu NAVRH)
//		T=d.v.vypis_objekty_s_pohony_bez_roztece();
//		if(T!="")
//		{
//			if(mrYes==MB(text_2+T+text_3,MB_YESNO) && STATUS==NAVRH)
//			{
//				Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
//				Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
//				Form_parametry_linky->ShowModal();//návratová hodnota se řeši v knihovně
//			}
//		}
//	}
//	if(ComboBoxCekani->ItemIndex==2 && d.RANDOM)d.JIZPOCITANO=false;//musí být před, zajístí přepočet dle nových hodnot
//	REFRESH();
//	if(ComboBoxCekani->ItemIndex==2)d.RANDOM=false;//musí být až za refresh
//	RM();//korekce chyby oskakování pravého menu, je zajímavé, že tu musí být znovu
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPGlyphButton_zpravy_ikonaClick(TObject *Sender)
{
  log(__func__);//logování
  Form_zpravy->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scSplitView_OPTIONSOpened(TObject *Sender)
{
  log(__func__);//logování
	scSplitView_OPTIONS->Left=ClientWidth-scSplitView_OPTIONS->OpenedWidth;

//	scGPButton_generuj->Options->NormalColor=scSplitView_OPTIONS->Color;
//	scGPButton_generuj->Options->FocusedColor=scGPButton_generuj->Options->NormalColor;
//	scGPButton_generuj->Options->HotColor=scGPButton_generuj->Options->NormalColor;
//	scGPButton_generuj->Options->PressedColor=scGPButton_generuj->Options->NormalColor;
//	scGPButton_generuj->Options->FramePressedColor=scGPButton_generuj->Options->NormalColor;
//
//	ButtonPLAY->Options->NormalColor=scSplitView_OPTIONS->Color;
//	ButtonPLAY->Options->FocusedColor=scGPButton_generuj->Options->NormalColor;
//	ButtonPLAY->Options->HotColor=scGPButton_generuj->Options->NormalColor;
//	ButtonPLAY->Options->PressedColor=scGPButton_generuj->Options->NormalColor;
//	ButtonPLAY->Options->FramePressedColor=scGPButton_generuj->Options->NormalColor;
}
//---------------------------------------------------------------------------


//při otevření panelu
void __fastcall TForm1::scSplitView_OPTIONSPanelPaint(TCanvas *ACanvas, TRect &ARect)
{
	log(__func__);//logování
//užitečná metoda při otevření panelu, onclosing nějak nefunguje, toto ano
}
//---------------------------------------------------------------------------
//zajišťuje vygenerování nové doby čekání na palec
void __fastcall TForm1::scGPButton_generujClick(TObject *Sender)
{
//	log(__func__);//logování
//	d.RANDOM=true;
//	d.JIZPOCITANO=false;
//	REFRESH();
//	d.RANDOM=false;
}
//---------------------------------------------------------------------------
//korekce chyby oskakování pravého menu
void TForm1::RM()
{
  log(__func__);//logování
	if(scSplitView_OPTIONS->Visible)scSplitView_OPTIONS->Align=alRight;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scExPanel_log_headerClick(TObject *Sender)
{
	log(__func__);//logování
	scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scExPanel_log_headerMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	log(__func__);//logování
	scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::scExPanel_log_headerMouseActivate(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)

{
	log(__func__);//logování
	scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------
//v případě zakliknutí či odkliknutí zohlední změny dle počtu vozíku (buď podle WIP či nikoliv)
void __fastcall TForm1::scGPCheckBox_pocet_voziku_dle_WIPClick(TObject *Sender)
{
	log(__func__);//logování
	aktualizace_maro_a_roma();
	REFRESH();
}
//---------------------------------------------------------------------------
//vypíše nápovědu k algoritmu čekání na palce
void __fastcall TForm1::scGPGlyphButton_info_cekaniClick(TObject *Sender)
{
  log(__func__);//logování
	UnicodeString t,t1,t2,t3,t4,t5,t6,t7;
	t="<b>"+ls->Strings[360];//"<b>Čekání na palce nastane</b> dle nastavení v parametrech objektu (není-li uživatelsky zakázáno):"
	t1="• "+ls->Strings[361]+"<br>";//"• po odchodu z objektu v S&G režimu<br>"
	t2="• "+ls->Strings[362]+"<br>";//"• mezi kontinuálními objekty, mají-li jiný pohon<br>"
	t3="• "+ls->Strings[363]+"<br>";//"• mezi postprocesními objekty, mají-li jiný pohon<br>"
	t4="• "+ls->Strings[364]+"<br>";//"• mezi kontinuálním a postprocesním objektem, mají-li jiný pohon<br>"
	t5="• "+ls->Strings[365]+"<br>";//"• mezi postprocesním a kontinuálním objektem<br>"
	t6="• "+ls->Strings[366]+"<br>";//"• vždy, když je za objektem definovaná stopka<br>"
	t7="• "+ls->Strings[367]+"<br>";//"• vždy, když je čekání uživatelsky nařízeno"
	MB(t+AnsiString("<br>")+t1+t2+t3+t4+t5+t6+t7,MB_OK,false,489);
}
//---------------------------------------------------------------------------
void TForm1::db_connection()
{
	log(__func__);//logování
	FDConnection1->Params->Database="tispl_db";
	FDConnection1->Params->UserName="app_tispl";
	FDConnection1->Params->Password="NbtDnKJqGX76rVeB";
	FDConnection1->Params->DriverID="Mysql";
	FDConnection1->Params->Add("Server=81.2.243.72");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonRostaClick(TObject *Sender)
{
//	TscGPGlyphButton *B = new TscGPGlyphButton(this);
//	B->Left=ClientWidth/2;
//	B->Top=ClientHeight/2;
//	B->GlyphOptions->Kind=scgpbgkCancel;
//	//B->Caption="test";
//	B->Parent=this;//musí být až na konci
//	B=NULL;delete B;

//ShowMessage(OBJEKT_akt->elementy->name);

//	TPoint *tab_pruchodu=new TPoint[d.v.pocet_vyhybek];
//	Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
//	while(O!=NULL)
//	{
//		/*if(O->short_name=="V1")*/Memo(O->short_name);
//		O=d.v.dalsi_krok(O,tab_pruchodu);
//	}
//	tab_pruchodu=NULL;delete tab_pruchodu;
//	O=NULL;delete O;

//d.v.POHONY->dalsi->name="ano";
//Form2->ShowModal();

//smazat--;
//Memo3->Visible=true; Memo3->Clear();Memo3->Lines->Add(smazat);
//Memo3->Font->Quality = (System::Uitypes::TFontQuality)4;
//REFRESH();

//Memo3->Visible=true;
//Cvektory::TPohon *P=d.v.POHONY->dalsi;
//while(P!=NULL)
//{
//	 Memo3->Lines->Add(P->name);
//	 P=P->dalsi;
//}
//P=NULL;delete P;
//
//if(pom!=NULL)Sk(pom->pohon->name);
//Sk(d.v.OBJEKTY->dalsi->pohon->name);

//Memo3->Visible=true;
//Cvektory::TElement *E=d.v.OBJEKTY->dalsi->element;
//while(E!=NULL)
//{
//	 Memo3->Lines->Add(E->n);
//	 E=E->dalsi;
//}
//E=NULL;delete E;

// if(MOD==EDITACE)
//   {
//   DrawGrid_knihovna->DefaultRowHeight=50;
//   scListGroupPanel_hlavickaOtoce->Visible=false;
//   scListGroupPanel_hlavickaOstatni->Visible=false;
//   scListGroupPanel_geometrie->Visible=false;
//   scListGroupPanel_poznamky->Visible=false;
//   DrawGrid_knihovna->Height=400;
//   scListGroupKnihovObjektu->Align=alLeft;
//   DrawGrid_knihovna->Invalidate();
//   MOD=LAYOUT;
//   }
//   else
//   {
//
//   MOD=EDITACE;
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

//	d.v.rotace_elementu(OBJEKT_akt,90);
//	REFRESH();
//double r=d.v.PP.katalog;
//double rad=d.v.PP.radius;
//
//Memo(r);
//Memo(rad);

//	 d.v.vloz_zpravu(0,0,0,NULL,ls->Strings[401]);
//	 d.v.vloz_zpravu(0,0,0,NULL,ls->Strings[402]);
//	 d.v.vloz_zpravu(0,0,0,NULL,"test");

//
//   Form_zpravy->ShowModal();
//
//   Sk(d.v.vrat_zpravu(2)->Popisek);
//	 d.v.vymazat_ZPRAVY();

//	F->posun_na_element(1);
	//Form_definice_zakazek->ShowModal();
//  Form2->ShowModal();


	////////////////připrava ladicí konzole
	//vytvoření aktuálního snímku obrazovky
//	pan_create();//vytvoří aktuální printscreen jen pracovní plochy
//	TPngImage* PNG = new TPngImage();//kvůli větší kompresi uloženo do PNG (má větší kompresi než JPG)
//	PNG->Assign(Pan_bmp);
//	PNG->SaveToFile(get_temp_dir() +"TISPL\\" + "tispl_PrtScr"+get_user_name()+"_"+get_computer_name()+".png");

	//připava textových dat
	//+odeslat uživatele,pc,licencci,mod,aktuální log?

	//odeslání e-mailu
	//použit již připraveno metodu mail();
//	TIdMessage *MAIL=new TIdMessage(this);
//	MAIL->Clear();
//	MAIL->From->Address="martin.kratochvil@lyzarskejihlavsko.cz";
//	MAIL->From->Name="Martin Kratochvíl";
//	MAIL->Subject="zkouška 12 včetně přílohy";
//	MAIL->Recipients->EMailAddresses="makr77@gmail.com,kratochvil.martin@seznam.cz";
//	MAIL->CCList->EMailAddresses="";
//	MAIL->BccList->EMailAddresses="";
//	MAIL->ContentType="multipart/related; type=text/html";//text/plain
//	MAIL->CharSet="UTF-8";
//	MAIL->Body->Text=AnsiToUtf8("Snad to <b>půjde</b> 11. port 25. již včetně přílohy Martin");
//	TIdAttachmentFile *Attach = new TIdAttachmentFile(MAIL->MessageParts,"c:\\Users\\rosta\\AppData\\Local\\Temp\\TISPL\\tispl_PrtScrrosta_ROSTA.png");//potřebuje #include <idattachmentfile.hpp>
//
//	TIdSMTP *SMTP=new TIdSMTP(this);
//	SMTP->Host="lyzarskejihlavsko.cz";//"smtp.seznam.cz";
//	SMTP->Username="martin.kratochvil@lyzarskejihlavsko.cz";
//	SMTP->Password="";
//	SMTP->Port=25;//SMTP->UseTLS=utNoTLSSupport; případně použít, pro použití SSL jiný port a zároveň potřeba s SMTP propojit přes IO handler SSL komponentu + 2x patřičné DLL
//	SMTP->Connect();
//	SMTP->Send(MAIL);
//	SMTP->Disconnect(true);
//	delete Attach;//musí být jako první
//	delete MAIL;
//	delete SMTP;

//	TIdMessage *MAIL=new TIdMessage(this);
//	MAIL->Clear();
//	MAIL->From->Address="builderboy@seznam.cz";
//	MAIL->From->Name="TISPL";
//	MAIL->Subject="zkouška 12 včetně přílohy";
//	MAIL->Recipients->EMailAddresses="rosta.slechta@gmail.com,rosta.slechta@seznam.cz";
//	MAIL->CCList->EMailAddresses="";
//	MAIL->BccList->EMailAddresses="";
//	MAIL->ContentType="multipart/related; type=text/html";//text/plain
//	MAIL->CharSet="UTF-8";
//	MAIL->Body->Text=AnsiToUtf8("Snad to <b>půjde</b> 11. port 25. již včetně přílohy Martin");
//	TIdAttachmentFile *Attach = new TIdAttachmentFile(MAIL->MessageParts,"c:\\Users\\rosta\\AppData\\Local\\Temp\\TISPL\\tispl_PrtScrrosta_ROSTA.png");//potřebuje #include <idattachmentfile.hpp>
//
//	TIdSMTP *SMTP=new TIdSMTP(this);
//	SMTP->Host="smtp.seznam.cz";//"smtp.seznam.cz";
//	SMTP->Username="builderboy@seznam.cz";
//	SMTP->Password="camaro69";
//	SMTP->Port=25;//SMTP->UseTLS=utNoTLSSupport; případně použít, pro použití SSL jiný port a zároveň potřeba s SMTP propojit přes IO handler SSL komponentu + 2x patřičné DLL
//	SMTP->Connect();
//	SMTP->Send(MAIL);
//	SMTP->Disconnect(true);
//	delete Attach;//musí být jako první
//	delete MAIL;
//	delete SMTP;
//
//	//odeslání dat na FTP server
//	TIdFTP *FTP=new TIdFTP(this);
//	FTP->Host="lyzarskejihlavsko.cz";//FTP server
//	FTP->Username="hojkov@lyzarskejihlavsko.cz";
//	FTP->Password="doplnit";
//	FTP->TransferType=ftBinary;
//	FTP->Passive=true;//nutno
//	FTP->Connect();
//	FTP->Put("C:\\Users\\Martin\\AppData\\Local\\Temp\\TISPL\\tispl_PrtScrMartin_MARTIN-NOTEBOOK.png");
//	FTP->Disconnect();
//	delete FTP;

//  ShowMessage(WhichFailedToLoad());
//  String Response;
//  IdHTTP1->IOHandler = IdSSLIOHandlerSocketOpenSSL1;
//  Response=IdHTTP1->Get("https://time.is/cs/");
//
//  Memo(Response);
//  Memo(ms.EP(Response,"<time id=\"clock\">","</time>"));
// IdSNMP1->Connect();

//IdDayTime1->Host="128.138.140.44";
//IdDayTime1->ReadTimeout=5000;
//IdDayTime1->Host= "www.time.windows.com";    //server není funkční
//IdDayTime1->Port =37;
//Memo(IdDayTime1->DayTimeStr);

//  IdSNTP1->Host="pool.ntp.org";   // druhé fční řešení
//	TDateTime TIME=IdSNTP1->DateTime;
//	Sk(TIME);

//    pom_element=NULL;
//		Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
//		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
//		while(E!=NULL)
//		{
//			if(d.v.oblast_elementu(E,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
//			{
//				double delka=E->geo.delka;
//				double HeightDeep=5;
//				double delkaSklon=m.delkaSklon(delka,HeightDeep);
//				E->geo.delkaPud=delka;
//				E->geo.delka=delkaSklon;
//				E->geo.HeightDepp=HeightDeep;
//				duvod_validovat=2;
//				vytvor_statickou_scenu();
//				REFRESH();
//				ShowMessage("Nastaveno HeightDeep "+String(HeightDeep*1000)+" mm pro element "+E->name);
//				break;
//			}
//			E=d.v.dalsi_krok(VYHYBKY,E,NULL);
//		}
//		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
//		E=NULL;delete E;

  Form2->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_stornoClick(TObject *Sender)
{
	log(__func__);//logování
	Timer_getjobid->Enabled=false;
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
	{
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
	}
	if(MOD==EDITACE)  //navrácení do módu schéma
	{
		log("Zavření editace, MOD=LAYOUT");
		Timer_neaktivity->Enabled=false;//vypnutí timeru pro jistotu
		if(Akce==GEOMETRIE)ukonceni_geometrie(mazani);//musí být invertováno, mazani==true při uložit
		if(Akce!=NIC || Akce_temp!=NIC)ESC();
		//////
		if(MOD==EDITACE&&index_kurzoru==9999||index_kurzoru==100)
		nastav_focus();
		//if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//také volá Refresh//smaz_kurzor se zavolá, pouze pokud je to třeba odstraňuje zbytečný problik, dodělal MaKr
		MOD=LAYOUT;//nutné před zoom, ale za smaz kurzor
		//smazání případných komor
		//d.v.vymaz_komory(OBJEKT_akt);
		//smazání elementů - musí být napočátku, aby nebyl problik
		//d.v.vymaz_elementy(OBJEKT_akt,true);//&&OBJEKT_akt->element!=NULL)

		////kontrola zda je nutné přemazat obraz
		if(storno && !FormX->existuje_validace() && !scGPButton_ulozit->Enabled)
		{
			vymaz_seznam_obrazu();//vymazání nepotřebných obrazů
			d.v.update_akt_zakazky();
			storno=false;
		}

		////mazání mGridů
		Cvektory::TElement *E=OBJEKT_akt->element;
		Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
		while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
		{
			E->mGrid->Delete();
			E->mGrid=NULL;
			E=d.v.dalsi_krok(VYHYBKY,E,OBJEKT_akt);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		E=NULL;delete E;
		//odstranění předchozího PM, pokud existuje
		if(predchozi_PM!=NULL)
		{
			predchozi_PM->mGrid->Delete();
			predchozi_PM->mGrid=NULL;
			predchozi_PM=NULL;
		}
    //odstranění tabulky pro teploměry
    if(OBJEKT_akt->teplomery!=NULL)
		{
			Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
			if(T!=NULL && T->posledni->mGrid!=NULL)//kontrola zda existuje cesta pro akt zakázku
			{
				T->posledni->mGrid->Delete();
				T->posledni->mGrid=NULL;
			}
			T=NULL;delete T;
		}
		////
		//if(storno && scGPButton_ulozit->Enabled)d.v.uprav_popisky_elementu(NULL);//volání přejmenování elementů, pouze v případě kdy je něco v kabině a bylo stisknuto pouze storno, při ulož je stisk strona volán taky
		pom=NULL;//pom->pohon=NULL;delete pom->pohon;pom=NULL; toto nelze, odpřiřadilo by to pohon i na ostrém
//		d.v.vymaz_elementy(OBJEKT_akt,true);
		//if(OBJEKT_akt!=NULL){OBJEKT_akt->pohon=NULL;delete OBJEKT_akt->pohon;}
		OBJEKT_akt=NULL;
		if(PmG!=NULL)
		{
			nastav_focus();
			PmG->Delete();
			PmG=NULL;delete PmG;
		}  
		//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
		pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
		FormX->posledni_E=NULL;//nutné!! slouží k ukládání posledního editovaného elementu (validace, atd.)
		if(d.v.DATA->dalsi!=NULL && storno)//pokud bylo stisknuto storno a je třeba přepsat obraz
		{
			d.v.nacti_z_obrazu_DATA(true);//načtení projektu před editací a smazání obrazů
			duvod_validovat=2;//pouze v případě, že přepisuji obraz
		}
		vytvor_obraz();//vytvoření nového obrazu pro layout
		//vlozit_predavaci_misto_aktualizuj_WT();//zkontroluje, zda nemusí být přidáno nebo odstraněno předávací místo
		//duvod_validovat=2;//vyvolá validaci, zajistí aktualizaci zpráv a výpisu v miniformu zpráv, NECHAT AŽ ZA FUNKČNÍMI ZÁLEŽITOSTMI
		//v případě animace vypnutí a nastavení do výchozího stavu
		Timer_simulace->Enabled=false;
	 //	ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
	 //	ButtonPLAY->Hint="spustit animaci";
		zobrazit_meritko=scGPSwitch_meritko->State;//navrácení do původního stavu
		//vypnutí spodního panelu
		zapnuti_vypnuti_panelEditace(false);
//		scGPPanel_bottomtoolbar->Visible=false;
//		scGPButton_bug_report->Top+=scGPPanel_bottomtoolbar->Height;//navrácení tlačítka report do původních souřadnic
		//navrácení zoomu a posunu do původních hodnt
		Zoom=Zoom_predchozi2;
		//on_change_zoom_change_scGPTrackBar();//pozor asi volá refresh
		Posun.x=Posun_predchozi2.x;
		Posun.y=Posun_predchozi2.y;

		//předesignované tlačítko layout - editace
		Schema->Caption=ls->Strings[20];
		if(d.v.PP.zamek_layoutu)Schema->ImageIndex=79;
		else Schema->ImageIndex=78;
		//////návrat knihoven a popisku na default místa, zbránění zobrazení knihoven na špatném místě při znovu otevření náhledu
		scListGroupPanel_hlavickaOtoce->Top=314;
		scListGroupPanel_hlavickaOstatni->Top=404;
		scListGroupPanel_geometrie->Top=494;
		scListGroupPanel_poznamky->Top=641;//584;
		DrawGrid_knihovna->DefaultRowHeight=50;
		DrawGrid_knihovna->DefaultColWidth=70;
		DrawGrid_knihovna->Left=14;
		scListGroupPanel_hlavickaOtoce->Visible=false;
		scListGroupPanel_hlavickaOstatni->Visible=false;
		scListGroupPanel_geometrie->Visible=false;
		scListGroupPanel_poznamky->Visible=false;
		DrawGrid_knihovna->Height=400;
		scListGroupKnihovObjektu->Align=alLeft;
		//DrawGrid_knihovna->Invalidate();
		scGPSwitch_robot_clovek->Visible=false;
		scGPPanel_pomocn_proSwitch->Visible=false;
		//REFRESH(); //- asi netřeba  asi vyvolává níže uvedený on_change_zoom_change_scGPTrackBar()
		//scListGroupKnihovObjektu->Visible=false;
		scGPButton_zmerit_vzdalenost->Visible=false;//schování měření vzdálenosti, pro správné řazení
		scGPComboBox_prepinacKot->ItemIndex=0;//ošetření pokud bylo při vypínání editace nastaveno na časové kóty
		scButton_zamek_layoutu->Visible=true;//zapnutí tlačítka zámek layoutu
		Image_knihovna_objektu->Visible=true;//zapnutí knihovny
		scGPButton_zmerit_vzdalenost->Visible=true;
		scGPButton_prichytavat->Visible=true;//zapnutí tlačítka přichytávat
		design_statusbar();//změní pozici zarážek ve statusbaru
		//povolení změny jazyka
		ChDir(ExtractFilePath(Application->ExeName));//přesune k EXE
		UnicodeString File_language="TISPL.language";
		if(FileExists(File_language))scGPSwitch1->Enabled=true;
		//opětovné vytvoření tabulky objektů nebo skrytí panelu
		if(!d.v.PP.zamek_layoutu)vytvoreni_tab_knihovna();
    else scSplitView_LEFTTOOLBAR->Opened=false;
		DrawGrid_knihovna->Top=10000;//musí být zobrazena, odchytává stisk kláves
		on_change_zoom_change_scGPTrackBar();//pozor už nevyvolává refresh
		//d.SCENA=1111111, prozatím statická scéna funkční pouze v layoutu, pro editaci se nepoužívá
		smaz_seznam_pouzivanych_pohonu();
		REFRESH(1111111,false);//false není třeba, mGridy už neexistují
		storno=true;//nastavení do defaultního stavu
	}
  //ukončení tvorby cesty
	if(MOD==TVORBA_CESTY)
	{
    F->log("Ukočení editace cesty, MOD=LAYOUT, Form:Unit1");
  	Akce=NIC;
		MOD=LAYOUT;
		zapnuti_vypnuti_panelEditace(false);
  	d.v.vymaz_cestu_zakazky(Form_definice_zakazek->Z_cesta);
		delete Form_definice_zakazek->Z_cesta;Form_definice_zakazek->Z_cesta=NULL;
		vytvor_statickou_scenu();//musí zde být
  	REFRESH();//odstranění vykreslení cesty z layoutu
  	Form_definice_zakazek->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton_nacist_podkladClick(TObject *Sender)
{
	log(__func__);//logování
	scSplitView_MENU->Opened=false;
	if(d.v.PP.raster.filename=="" || (d.v.PP.raster.filename!="" && FileExists(d.v.PP.raster.filename) && MB(ls->Strings[496],MB_YESNO)==mrYes) || (d.v.PP.raster.filename!="" && !FileExists(d.v.PP.raster.filename)))//"Podklad již existuje, přejete si načíst nový?"
	{
    Nacist_podklad();
	}
  scButton_nacist_podklad->Down=false;
}
//---------------------------------------------------------------------------
void TForm1::Nacist_podklad()
 {
   OpenDialog1->Title="Načíst podklad";
	 OpenDialog1->DefaultExt="*.bmp";
	 OpenDialog1->Filter="Soubory formátu bmp (*.bmp)|*.bmp";
	 if(OpenDialog1->Execute())
	 {
		 //načtení podkladu
		 Nacist_podklad(OpenDialog1->FileName);
		 edice();
	 }
 }
 //---------------------------------------------------------------------------
 unsigned short int  TForm1::Nacist_podklad(UnicodeString soubor,bool akt_filename)
 {
	log(__func__);//logování
	d.v.PP.raster.show=true;
  d.v.PP.raster.filename=soubor;
	if(!akt_filename)
	{
		d.v.PP.raster.X=m.P2Lx(scSplitView_LEFTTOOLBAR->Width +10);d.v.PP.raster.Y=m.P2Ly(scGPPanel_mainmenu->Height + 10);//souřadnice v metrech
  	d.v.PP.raster.resolution=m2px;  //výpočet metry děleno počet PX, výchozí zobrazení v nativním rozlišení (bez usazení do metrického měřítka) je 0.1
  	scGPCheckBox_zobraz_podklad->Checked=true;
		scButton_nacist_podklad->Down=false;  //ošetření proti tmavému vysvícení při dalším zobrazení mainmenu
		auto_settings_open=false;
		TIP=ls->Strings[303];//"Pro správné umístění a nastavení měřítka podkladu, využijte volbu v pravém horním menu."
  //  if(mrOk==MB("Pro správné umístění a nastavení měřítka podkladu, využijte volbu v pravém horním menu. Přejít do nastavení?",MB_OKCANCEL))
  //  {
		auto_settings_open=true;
	 // scGPGlyphButton_OPTIONSClick();  //tohle nesežral
  	scSplitView_OPTIONS->Opened=true;
  	scGPCheckBox_zobraz_podklad->Enabled=true;
		scGPCheckBox_stupne_sedi->Enabled=true;
  	scGPTrackBar_svetelnost_posuvka->Enabled=false; //prozatim zakazano
  	scLabel1_svetelnost->Enabled=false;        // prozatim zakazano
  	scGPButton_kalibrace->Enabled=true;
  	scGPButton_adjustace->Enabled=true;
		scGPButton_smazat->Enabled=true;
		REFRESH();
	}
	return 0;
  //scGPGlyphButton_OPTIONS->ShowHint=true;
 // }
}
 //--------------------------------------------------------------
void __fastcall TForm1::DrawGrid_geometrieDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	log(__func__);//logování
	//if(pom->id!=3)//nevykreslovat nic v POW
	{
  	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
  	int W=DrawGrid_geometrie->DefaultColWidth  *Z;
  	int H=DrawGrid_geometrie->DefaultRowHeight  *Z;
  	int P=-1*DrawGrid_geometrie->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
  	int odsazeni=5;

  	Cantialising a;
  	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
  	bmp_in->Width=DrawGrid_geometrie->Width*Z;bmp_in->Height=DrawGrid_geometrie->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
  	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

  	//unsigned short obdelnik_okrajX=10*Z;unsigned short obdelnik_okrajY=5*Z;
  	double Zoom_back=Zoom;//záloha zoomu
  	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
  	short pocet_elementu=2;
  	AnsiString label1;
		AnsiString label2;
		scGPLabel_geometrie->Caption=ls->Strings[61];//"Geometrie linky";
  	for(unsigned short n=1;n<=pocet_elementu;n++)
  	{
  		if(n==1)
			{
				label1=ls->Strings[268];//"geometrie";
				label2="";
				if(d.v.ZAKAZKA_akt==NULL || (d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					d.vykresli_ikonu_linie(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20-odsazeni,label1);
					d.vykresli_ikonu_stoupani_klesani(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-3,(Rect.Bottom*Z-Rect.Top*Z)/2+(1)*H+P + 20-odsazeni-15,0,ls->Strings[490]);
				}
				else
				{
					d.vykresli_ikonu_linie(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20-odsazeni,label1,-1);
					d.vykresli_ikonu_stoupani_klesani(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-3,(Rect.Bottom*Z-Rect.Top*Z)/2+(1)*H+P + 20-odsazeni-15,0,ls->Strings[490],-1);
				}
			}
  		if(n==2)
  		{
				label1=ls->Strings[269];//"před. místo";
				if(OBJEKT_akt->pohon!=NULL && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))
				{
					d.vykresli_predavaci_misto(C,NULL,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P,label1,0);
					d.vykresli_ikonu_stoupani_klesani(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W+6,(Rect.Bottom*Z-Rect.Top*Z)/2+(1)*H+P + 20-odsazeni-15,1,ls->Strings[491]);
				}
				else
				{
					d.vykresli_predavaci_misto(C,NULL,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P,label1,0,0,-1);
					d.vykresli_ikonu_stoupani_klesani(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W+6,(Rect.Bottom*Z-Rect.Top*Z)/2+(1)*H+P + 20-odsazeni-15,1,ls->Strings[491],-1);
				}
			}
  	}
  	Zoom=Zoom_back;//návrácení původního zoomu
  	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
  	DrawGrid_geometrie->Canvas->Draw(0,0,bmp_out);
  	delete (bmp_out);//velice nutné
		delete (bmp_in);//velice nutné
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_zobraz_podkladClick(TObject *Sender)
{
	log(__func__);//logování
// if(auto_settings_open==false)
// {
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
 // }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_kalibraceClick(TObject *Sender)
{
  log(__func__);//logování
	kurzor(add_o);//pozor kurzor kalibrovat neni vycentrovaný, je třeba jej imageeditoru předělat
	Akce=KALIBRACE;
  scSplitView_OPTIONS->Opened=false;
   }
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_adjustaceClick(TObject *Sender)
{
  log(__func__);//logování
	kurzor(add_o);
	Akce=ADJUSTACE;
  scSplitView_OPTIONS->Opened=false;
  }
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPGlyphButton_OPTIONSClick(TObject *Sender)
{
	log(__func__);//logování
	if(OBJEKT_akt!=NULL)//pokud je v náhledu kliknuto, musí být menu zavřeno takto, zabraňuje probliku zbytků menu v knihovnách
	{
		DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
		DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
	}
  ESC();//zruší případnou rozdělanou akci  - způsobuje problém při zavírání splitview
	//scSplitView_OPTIONS->Opened = !scSplitView_OPTIONS->Opened;

//  scGPGlyphButton_OPTIONS->Active=false;
//  scGPGlyphButton_OPTIONS->GlyphColor=clWhite;
//  scGPGlyphButton_OPTIONS->GlyphThickness=1;

 //jazyk mutace
  //if(readINI("Nastaveni_app","jazyk")=="1"){language=CS; scGPSwitch1->State=scswOn;} else { language=EN; scGPSwitch1->State=scswOff;}

 	//nastavení checkboxu na checked pokud je již rastr uložen a má být zobrazen
	if(d.v.PP.raster.filename!="" &&  d.v.PP.raster.show)
	{
		if(FileExists(d.v.PP.raster.filename))
    {
   // ShowMessage(d.v.PP.raster.dim);
			scGPCheckBox_zobraz_podklad->Checked=true;
   // scGPTrackBar_svetelnost_posuvka->Value=d.v.PP.raster.dim;   zatím zakázáno
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
		scGPButton_smazat->Enabled=false;
		scLabel1_svetelnost->Font->Color=clGray;
  }
  else
  {
  	scGPCheckBox_zobraz_podklad->Enabled=true;
  	scGPCheckBox_stupne_sedi->Enabled=true;
  	scGPTrackBar_svetelnost_posuvka->Enabled=false;   //zatím zakázáno
  	scLabel1_svetelnost->Enabled=false; // zatím zakázáno
		scGPButton_adjustace->Enabled=true;
		scGPButton_kalibrace->Enabled=true;
		scGPButton_smazat->Enabled=true;
 // scLabel1_svetelnost->Font->Color=clWhite;  // odkomentovat, pokud bude povoleno
  }
	edice();
	scSplitView_OPTIONS->Opened = !scSplitView_OPTIONS->Opened;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_OPTIONSMouseEnter(TObject *Sender)
{
	log(__func__);//logování
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_OPTIONSMouseLeave(TObject *Sender)
{
	log(__func__);//logování
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scExPanel_podkladClick(TObject *Sender)
{
//
  log(__func__);//logování
	scExPanel_podklad->RollUpState=!scExPanel_podklad->RollUpState;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPTrackBar_svetelnost_posuvkaChange(TObject *Sender)
{
	log(__func__);//logování
	d.v.PP.raster.dim=scGPTrackBar_svetelnost_posuvka->Value;
	REFRESH();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_stupne_sediClick(TObject *Sender)
{
  log(__func__);//logování
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
void __fastcall TForm1::Button_testClick(TObject *Sender)
{
	log(__func__);//logování
	MOD=LAYOUT;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_OKClick(TObject *Sender)
{
	log(__func__);//logování
	//uložení editace
	if(MOD==EDITACE)
	{
  	Timer_getjobid->Enabled=false;
		if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//uložení změn při zapnuté editaci textu
    byly_pohony_editovany();//musí být před smazáním seznamu obrazů!!!!!!!
		//vymazání nepotřebných obrazů
  	vymaz_seznam_obrazu();//vymazání nepotřebných obrazů
  	d.v.update_akt_zakazky();
  	//d.v.vymaz_komory(pom);
  	//d.v.vymaz_elementy(pom,true);
  	//d.v.kopiruj_objekt(OBJEKT_akt,pom);
  	DuvodUlozit(true);
  	nahled_ulozit(false);
  	mazani=true;//použití proměnné, která se v tomto čase nevyužívá, slouží k rozpoznání zda bylo stisknuto dříve storno či uližit
		//a to z důvodu volání uprav_popisky_elementu(přejmenování změn po stisku storno)
  	storno=false;//důležité nastavení pro storno, rozlišení, že volám storno z tlačítka uložit, tudíž nemazat nic
  	scGPButton_stornoClick(Sender);//další funkcionalita je již stejná jako ve stornu, včetně vymazání ukazatele OBJEKT_akt včetně jeho elementů popř. komor
		mazani=false;
	}
	//uložení vytvořené cesty
	if(MOD==TVORBA_CESTY)
	{
    F->log("Uložení nové cesty, MOD=LAYOUT, Form:Unit1");
  	Akce=NIC;
  	MOD=LAYOUT;
		zapnuti_vypnuti_panelEditace(false);
		vytvor_statickou_scenu();
  	REFRESH();//odstranění vykreslení cesty z layoutu
  	Form_definice_zakazek->ShowModal();
	}
}
//---------------------------------------------------------------------------
//input metoda na převod jednotek LO do Si
double TForm1::inLO(double inLO)
{
	log(__func__);//logování
	double LO=inLO;
	if (LOunit == MM) LO=inLO/1000.0;
	return LO;
}
//output metoda na převod jednotek z jádra do jednotek dle INI
double TForm1::outLO(double outLO)
{
	log(__func__);//logování
	double LO=outLO;
	if (LOunit == MM) LO=outLO*1000.0;
	return LO;
}
//---------------------------------------------------------------------------
double TForm1::inPT(double inPT)
{
	log(__func__);//logování
	double PT=inPT;
	if (PTunit == MIN) PT=inPT*60.0;
	return PT;

}
//---------------------------------------------------------------------------
double TForm1::outPT(double outPT)
{
	log(__func__);//logování
	double PT=outPT;
	if (PTunit == MIN) PT=outPT/60.0;
	return PT;
}
//---------------------------------------------------------------------------
double TForm1::inDO(double inDO)
{
	log(__func__);//logování
	double DO=inDO;
	if (DOtocunit == MM) DO=inDO/1000.0;
	return DO;
}
//---------------------------------------------------------------------------
double TForm1::outDO(double outDO)
{
	log(__func__);//logování
	double DO=outDO;
	if (DOtocunit == MM) DO=outDO*1000.0;
	return DO;
}
//---------------------------------------------------------------------------
//při načítaní hodnoty rozměrů provede převod podle aktuálně nastavených jednotek
double TForm1::inDK(double inDK)
{   //0=M,1=MM,2=SEC,3=MIN
	log(__func__);//logování
	double DK=inDK;
	switch(DKunit)
	{
		case 0:case 1:if (DKunit == MM) DK=inDK/1000.0;break;
		case 2:case 3:if (DKunit == MINUTY) DK=inDK*60.0;break;
	}
	return DK;
}
//---------------------------------------------------------------------------
//při výpisu hodnoty rozměrů provede převod podle aktuálně nastavených jednotek
double TForm1::outDK(double outDK)
{
	log(__func__);//logování
	double DK=outDK;
	switch(DKunit)
	{
		case 0:case 1:if (DKunit == MM) DK=outDK*1000.0;break;
		case 2:case 3:if (DKunit == MINUTY) DK=outDK/60.0;break;
	}
	return DK;
}
//---------------------------------------------------------------------------
double TForm1::inaRD(double inaRD)
{
  log(__func__);//logování
	double aRD=inaRD;
	aRD=inaRD/(1+59.0*aRDunit);
	return aRD;
}
//---------------------------------------------------------------------------
double TForm1::outaRD(double outaRD)
{
	log(__func__);//logování
	double aRD=outaRD;
	aRD=outaRD*(1+59.0*aRDunit);
	return aRD;
}
//---------------------------------------------------------------------------
double TForm1::inR(double inR)
{
	log(__func__);//logování
	double R=inR;
	R=inR/(1+999.0*Runit);
	return R;
}
//---------------------------------------------------------------------------
double TForm1::outR(double outR)
{
	log(__func__);//logování
	double R=outR;
	R=outR*(1+999.0*Runit);
	return R;
}
//---------------------------------------------------------------------------
double TForm1::inRz(double inRz)
{
	log(__func__);//logování
	double Rz=inRz;
	Rz=inRz/(1+999.0*Rzunit);
	return Rz;
}
//---------------------------------------------------------------------------
double TForm1::outRz(double outRz)
{
	log(__func__);//logování
	double Rz=outRz;
	Rz=outRz*(1+999.0*Rzunit);
	return Rz;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPEdit1Change(TObject *Sender)
{
	log(__func__);//logování
	Memo_testy->Lines->Add("onchange");
}
//---------------------------------------------------------------------------
//používál se na smazání editu názvu elementu v tabulce elementů - momentálně nevyužíván
void __fastcall TForm1::TimerSmazEditNazvuElementuVTabulceTimer(TObject *Sender)
{
// log(__func__);//logování
// if(pom_element_temp!=NULL&&index_kurzoru==9999)//pro smazání editu při editaci názvu elementu skrze tabulku
// {
//	 smaz_edit();
// }
// else//pro posun refresh při změně tabulky elementů
// {
//	 REFRESH(true);
//	 FormX->input_state=FormX->NOTHING;
// }
// Timer2->Enabled=false;
}
//---------------------------------------------------------------------------
//volá metodu vykresli_kurzor při editaci textu
void __fastcall TForm1::TimerKurzorTimer(TObject *Sender)
{
	log(__func__);//logování
	vykresli_kurzor(index_kurzoru);
}
//---------------------------------------------------------------------------
//vykresluje a maže kurzor
void TForm1::vykresli_kurzor(int index)
{
  log(__func__);//logování
	//nastavení rozměrů názvu a zkratky objektu
	AnsiString Tn,Tl,Tz;
	short Wn;
	double Xt=0,Yt=0;
	if(OBJEKT_akt!=NULL)
	{
		d.nastavit_text_popisu_objektu_v_nahledu(Canvas);Tn=F->OBJEKT_akt->name.UpperCase();Wn=Canvas->TextWidth(Tn);//název objektu - nastavení
		d.nastavit_text_popisu_objektu_v_nahledu(Canvas);Tl=+" / ";
		d.nastavit_text_popisu_objektu_v_nahledu(Canvas);Tz=F->OBJEKT_akt->short_name.UpperCase();
	}
	//vykreslování kurzoru pro psaní textu
	Canvas->Pen->Style=psSolid;
	Canvas->Pen->Mode=pmNotXor;
	Canvas->Pen->Width=2;
	switch ((index))//index=JID, kde a jaký kurzor vykreslit
	{
		case -8://editace popisku teploměr
		{
			Canvas->Pen->Color=clBlack;
			Canvas->Pen->Width=1;
			Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.top-2);
			Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.bottom+2);
			break;
    }
		case 1://editace názvu elementuu
		{
			Canvas->Pen->Color=clBlack;
			if(pom_element_temp->eID==0)Canvas->Pen->Color=clRed;//stopka má červený text
			Canvas->Pen->Width=1;
			double OR=pom_element_temp->orientace;
			if(pom_element_temp->eID!=0&&pom_element_temp->eID!=5&&pom_element_temp->eID!=6&&pom_element_temp->eID!=100)//roboti mají vykreslován kurzor vodorovně
			{
				if(OR==0 || OR==180)//pro vodorovnou kabinu
				{
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.top-2);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.bottom+2);
				}
				else if(OR==90)//pro vertikálníkabinu, dělení na 2 rotace, text orientován 2 směry
				{
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+2,pom_element_temp->citelna_oblast.rect3.bottom-1);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.left-2,pom_element_temp->citelna_oblast.rect3.bottom-1);
				}
				else
				{
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+2,pom_element_temp->citelna_oblast.rect3.top+1);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.left-2,pom_element_temp->citelna_oblast.rect3.top+1);
				}
			}
			else//otoče a stop mají kurzor otočený o 90°
			{
				if(OR==0 || OR==180)//pro vodorovnou kabinu, 2 orientace textu
				{
					if(OR==0)
					{
						Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+2,pom_element_temp->citelna_oblast.rect3.bottom-1);
						Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.left-2,pom_element_temp->citelna_oblast.rect3.bottom-1);
					}
					else
          {
						Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+2,pom_element_temp->citelna_oblast.rect3.top+1);
						Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.left-2,pom_element_temp->citelna_oblast.rect3.top+1);
					}
				}
				else//pro vetrikální kabinu, tex oriontován jedním způsobem všude
        {
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.top-2);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.bottom+2);
				}
			}
			stav_kurzoru=!stav_kurzoru;
		}break;
		case -2://hodnota kót haly
		case -5://hodnota kót objektu
		{
			Canvas->Pen->Color=clGray;
			Canvas->Pen->Width=1.5;
			Canvas->MoveTo(pom_bod_temp->kota.rect1.right*3.0+1,pom_bod_temp->kota.rect1.top*3.0);
			Canvas->LineTo(pom_bod_temp->kota.rect1.right*3.0+1,pom_bod_temp->kota.rect1.bottom*3.0);
			stav_kurzoru=!stav_kurzoru;
		}break;
		case -6://název objektu
		{
			Canvas->Pen->Color=d.clStenaHaly;
			if(Tn=="")Tn="b";//při prázdném textu nemůžu zjistit TextHeight -> to zapříčiní čpatné vykreslení kurzoru, když uživatel odmaže veškerý text
			Xt=m.L2Px(F->OBJEKT_akt->Xt);Yt=m.L2Py(F->OBJEKT_akt->Yt);
			switch((int)OBJEKT_akt->orientace_text)
			{
				case 0:
				{
					Canvas->MoveTo(Xt-Canvas->TextHeight(Tn)-3,Yt-Wn/2.0-2);
					Canvas->LineTo(Xt+3,Yt-Wn/2.0-2);
				}break;
				case 90:case 270:
				{
					Canvas->MoveTo(Xt+Wn/2.0+2,Yt-Canvas->TextHeight(Tn)-3);
					Canvas->LineTo(Xt+Wn/2.0+2,Yt+3);
				}break;
				case 180:
				{
					Canvas->MoveTo(Xt+Canvas->TextHeight(Tn)+3,Yt+Wn/2.0+2);
					Canvas->LineTo(Xt-3,Yt+Wn/2.0+2);
				}break;
			}
			stav_kurzoru=!stav_kurzoru;
		}break;
		case -11:case -101://kóty elementů, kóty komor, koty mezi LO
		{
			if(OBJEKT_akt->id!=3 || (OBJEKT_akt->id==3 && (Akce==GEOMETRIE || Akce==GEOMETRIE_LIGHT)))//kóty elementů
			{
				Canvas->Pen->Color=clGray;
				Canvas->Pen->Width=1.5;
				if(index!=-101)
				{
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect1.right*3.0+1,pom_element_temp->citelna_oblast.rect1.top*3.0);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect1.right*3.0+1,pom_element_temp->citelna_oblast.rect1.bottom*3.0);
				}
				else
				{
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect4.right*3.0+1,pom_element_temp->citelna_oblast.rect4.top*3.0);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect4.right*3.0+1,pom_element_temp->citelna_oblast.rect4.bottom*3.0);
				}
				stav_kurzoru=!stav_kurzoru;
    	}
    	else//kóty komor
    	{
    		Canvas->Pen->Color=clGray;
    		Canvas->Pen->Width=1.5;
    		Canvas->MoveTo(pom_komora_temp->kota.rect1.right+1,pom_komora_temp->kota.rect1.top);
    		Canvas->LineTo(pom_komora_temp->kota.rect1.right+1,pom_komora_temp->kota.rect1.bottom);
    		stav_kurzoru=!stav_kurzoru;
			}
		}break;
		case -13://editace konce S/K
		{
      Canvas->Pen->Color=clGray;
			Canvas->Pen->Width=1.5;
			Canvas->MoveTo(pom_element_temp->citelna_oblast.rect7.right+1,pom_element_temp->citelna_oblast.rect7.top);
			Canvas->LineTo(pom_element_temp->citelna_oblast.rect7.right+1,pom_element_temp->citelna_oblast.rect7.bottom);
			stav_kurzoru=!stav_kurzoru;
		}break;
		case -14://editace začátku S/K
		{
      Canvas->Pen->Color=clGray;
			Canvas->Pen->Width=1.5;
			Canvas->MoveTo(pom_element_temp->citelna_oblast.rect6.right+1,pom_element_temp->citelna_oblast.rect6.top);
			Canvas->LineTo(pom_element_temp->citelna_oblast.rect6.right+1,pom_element_temp->citelna_oblast.rect6.bottom);
			stav_kurzoru=!stav_kurzoru;
		}break;
	}
}
//---------------------------------------------------------------------------
//smaže kurzor pokud je stále vykreslený i po vypnutí editace textu
void TForm1::smaz_kurzor()
{
  log(__func__);//logování
	TIP="";
	if((Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU) && OBJEKT_akt!=NULL)//ukončí editaci textu
	{
    //pokud je kurzor vykreslen překresli ho
		if(stav_kurzoru)vykresli_kurzor(index_kurzoru);
		TimerKurzor->Enabled=false;
		//pokud bylo zadáno nic přepíše nic původními hodnotamy
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru==-5){if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(OBJEKT_akt->body->predchozi->X,OBJEKT_akt->body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);if(DKunit==2||DKunit==3)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));}
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&(index_kurzoru==-11 || index_kurzoru==-101)&&OBJEKT_akt->id!=3&&Akce!=GEOMETRIE&&Akce!=GEOMETRIE_LIGHT){if(index_kurzoru!=-101){editovany_text=pom_element_temp->geo.delka;if(pom_element_temp->geo.HeightDepp!=0)editovany_text=pom_element_temp->geo.delkaPud;}else editovany_text=outDK(vzdalenost_meziLO(pom_element_temp,OBJEKT_akt->orientace));if(DKunit==2||DKunit==3)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;}
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru==-11&&OBJEKT_akt->id==3&&Akce!=GEOMETRIE&&Akce!=GEOMETRIE_LIGHT)editovany_text=outDK(pom_komora_temp->velikost);
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru==-11 && (Akce==GEOMETRIE || Akce==GEOMETRIE_LIGHT))posledni_editovany_element=pom_element_temp;//{editovany_text=pom_element_temp->geo.delka;if((DKunit==2||DKunit==3) && pom_element_temp->pohon!=NULL)editovany_text=editovany_text/OBJEKT_akt->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));}
		if(editovany_text==""&&index_kurzoru==-13){editovany_text="";double Z=0;if(pom_element_temp->predchozi!=NULL)Z=pom_element_temp->predchozi->Z;if(Z+pom_element_temp->geo.HeightDepp==0)editovany_text="±";if(Z+pom_element_temp->geo.HeightDepp>0)editovany_text="+";editovany_text+=m.round2double((Z+pom_element_temp->geo.HeightDepp)*1000,0);}//konec S/K
		if(editovany_text==""&&index_kurzoru==-14){editovany_text="";double Z=0;if(pom_element_temp->predchozi!=NULL)Z=pom_element_temp->predchozi->Z;if(Z==0)editovany_text="±";if(Z>0)editovany_text="+";editovany_text+=m.round2double(Z*1000,0);}//začátek S/K
		switch(index_kurzoru)
		{
			case -8://hodnota teploměru
			{
				if(pom_element_temp->name=="" || ms.MyToDouble(pom_element_temp->name)==0)
				{
					pom_element_temp->name=nazev_puvodni;
				}
				break;
			}
			case -7:if(OBJEKT_akt->short_name=="")OBJEKT_akt->short_name=nazev_puvodni;break;//objekt short name
			case -6:if(OBJEKT_akt->name=="")OBJEKT_akt->name=nazev_puvodni;break;//objekt name
			case 1://název elementu
			{
				if(pom_element_temp->name=="")pom_element_temp->name=nazev_puvodni;
				//propsání nového názvu do mGridu
				int prvni_sloupec=0;
				if(pom_element_temp->eID==200 || pom_element_temp->eID==300)prvni_sloupec=3;
				pom_element_temp->mGrid->Cells[prvni_sloupec][prvni_sloupec].Text="<a>"+pom_element_temp->name+"</a>";//nasazení linku
				//znovusloučení buňěk
				if(pom_element_temp->eID!=200 && pom_element_temp->eID!=300)pom_element_temp->mGrid->MergeCells(0,0,pom_element_temp->mGrid->ColCount-1,0);
				//aktualizace spárovaných elementů, pokud byl změněn název S&G elementu
				if(d.v.vrat_druh_elementu(pom_element_temp)==0)d.v.aktualizuj_sparovane_ukazatele();//aktualizijue spárovnaný ukazatel všem elementům v projektu
				duvod_validovat=2;
			}break;
			case -5://hodnota kót objektu (hrana objektu)
			{
				double posunx=0,posuny=0;
				//převedení na základní jednotky
				editovany_text=inDK(ms.MyToDouble(editovany_text));
				if(DKunit==2||DKunit==3)editovany_text=editovany_text*OBJEKT_akt->pohon->aRD;//pokud jsou kóty v časovém režimu, převede na vzdálenost;
				//deklarace pomocných proměnných
				double puv=0,nov=ms.MyToDouble(editovany_text),posun=0;
				Cvektory::TBod *A=pom_bod_temp->predchozi;
				if(pom_bod_temp->n==1)A=OBJEKT_akt->body->predchozi;
				//výpočet posunu (jednotlivé souřadnice bodu) + posun
				puv=m.round2double(m.delka(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),3);
				posun=nov-puv;
				posunx=sin(DegToRad(fmod(m.azimut(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),360)))*posun;posuny=cos(DegToRad(fmod(m.azimut(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),360)))*posun;
				pom_bod_temp=pom_bod_temp->predchozi;
				if(pom_bod_temp->n==0)pom_bod_temp=OBJEKT_akt->body->dalsi;
				d.v.posun_bod(pom_bod_temp->X-posunx,pom_bod_temp->Y-posuny,pom_bod_temp);
				//pokud jsou v objektu pouze 4 body, posun i dalšího bodu, tzn. editace obdelníku
				if(OBJEKT_akt->body->predchozi->n==4)
				{
         	pom_bod_temp=pom_bod_temp->predchozi;
					if(pom_bod_temp->n==0)pom_bod_temp=OBJEKT_akt->body->dalsi;
					d.v.posun_bod(pom_bod_temp->X-posunx,pom_bod_temp->Y-posuny,pom_bod_temp);
				}
				A=NULL;delete A;
			}break;
			case -13://editace konce S/K
			{
				if(editovany_text.SubString(1,1)=="±")editovany_text.Delete(1,1);
				if(ms.MyToDouble(editovany_text)!=0 || editovany_text=="0" || editovany_text=="+0" || editovany_text=="-0" || editovany_text=="±0")
				{
					double HeightDepp=ms.MyToDouble(editovany_text)/1000.0;//konec == Z +HeightDepp
					HeightDepp=HeightDepp-pom_element_temp->predchozi->Z;
					pom_element_temp->geo.delka=m.castPrepony(pom_element_temp->geo.delkaPud,pom_element_temp->geo.delkaPud,HeightDepp);
					pom_element_temp->geo.HeightDepp=HeightDepp;
					//aktualizace Z elementům
					Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
					while(pom_element_temp!=NULL)
					{
						if(pom_element_temp->predchozi->n!=0)pom_element_temp->Z=pom_element_temp->predchozi->Z+pom_element_temp->geo.HeightDepp;
						else pom_element_temp->Z=0;
						pom_element_temp=d.v.dalsi_krok(VYHYBKY,pom_element_temp);
					}
					d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
					delete pom_element_temp;pom_element_temp=NULL;
				}
				else
				{
          pom_element=pom_element_temp;
					Smazat1Click(this);
				}
			}break;
			case -14://editace začátku S/K
			{
				if(editovany_text.SubString(1,1)=="±")editovany_text.Delete(1,1);
				if(ms.MyToDouble(editovany_text)!=0 || editovany_text=="0" || editovany_text=="+0" || editovany_text=="-0" || editovany_text=="±0")
				{
					double HeightDepp=ms.MyToDouble(editovany_text)/1000.0;//začátek = predchozi Z
					pom_element_temp->predchozi->Z=HeightDepp;
					HeightDepp=pom_element_temp->Z-pom_element_temp->predchozi->Z;
					pom_element_temp->geo.delka=m.castPrepony(pom_element_temp->geo.delkaPud,pom_element_temp->geo.delkaPud,HeightDepp);
					pom_element_temp->geo.HeightDepp=HeightDepp;
          //aktualizace Z elementům
					Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
					while(pom_element_temp!=NULL)
					{
						if(pom_element_temp->predchozi->n!=0)pom_element_temp->Z=pom_element_temp->predchozi->Z+pom_element_temp->geo.HeightDepp;
						else pom_element_temp->Z=0;
						pom_element_temp=d.v.dalsi_krok(VYHYBKY,pom_element_temp);
					}
					d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
					delete pom_element_temp;pom_element_temp=NULL;
				}
				else
				{
          pom_element=pom_element_temp;
					Smazat1Click(this);
				}
			}break;
		}
		if((index_kurzoru==-11 || index_kurzoru==-101)&&OBJEKT_akt->id!=3&&Akce!=GEOMETRIE&&Akce!=GEOMETRIE_LIGHT)//editace hodnot kót elementů
		{
			//double vzdalenost=m.round2double(pom_element_temp->geo.delka,2);
			editovany_text=inDK(ms.MyToDouble(editovany_text));//převedení na základní jednotky
			if(DKunit==2||DKunit==3)editovany_text=editovany_text*OBJEKT_akt->pohon->aRD;//pokud jsou kóty v časovém režimu nutno přepočítat na vzdálenost
			if(index_kurzoru==-101)editovany_text=ms.MyToDouble(editovany_text)-vzdalenost_meziLO(pom_element_temp,pom_element_temp->geo.orientace)+pom_element_temp->geo.delka;
			d.v.posun_element(pom_element_temp,ms.MyToDouble(editovany_text),posun_dalsich_elementu);//realizace samotného posunu
			//vlidace + posun na původní místo
			if(Akce==EDITACE_TEXTU)Akce=NIC;Akce_temp=NIC;//důležité, pokud se zobrazí MB s překrytím zobrazí se editovaný text, v němž je posun a ne hodnota délky v mm
			short chybne=prekryti_LO(pom_element_temp);//kontrola zda se element nepřekrývá lak. oknem s jiným elementem
			if(chybne!=0)REFRESH(false);//vykreslení posunu, pokud má být zobrazen dotaz
			UnicodeString text=ls->Strings[324],text_1=ls->Strings[325];//"Přesunem dojde k překrytí lakovacích oken, chcete element přesunout?","Přesunem dojde k zásahu do zóny otáčení, chcete element přesunout?"
			if(chybne==1 && mrYes!=MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text,"",MB_YESNO) || chybne>1 && mrYes!=MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,text_1,"",MB_YESNO))
			{
				//posun elementu (bez geo.)
				double posunx=puv_souradnice.x-pom_element_temp->X,posuny=puv_souradnice.y-pom_element_temp->Y;
				pom_element_temp->X=puv_souradnice.x;pom_element_temp->Y=puv_souradnice.y;
				//nutná kontrola změny pořadí!!!
				Cvektory::TElement *E=d.v.vloz_element_pred(OBJEKT_akt,pom_element_temp);
				if(E!=NULL && pom_element_temp->dalsi!=NULL && E->n!=pom_element_temp->dalsi->n)d.v.zmen_poradi_elementu(pom_element_temp,E);
				E=NULL;delete E;
				//uprava geometrie
				d.v.vloz_G_element(pom_element_temp,0,pom_element_temp->geo.X1,pom_element_temp->geo.Y1,0,0,0,0,pom_element_temp->geo.X4+posunx,pom_element_temp->geo.Y4+posuny,pom_element_temp->geo.orientace,pom_element_temp->geo.rotacni_uhel);
				if(posun_dalsich_elementu)
				{
					//vrácení elementů ovlivněných vázaným posunem
					E=pom_element_temp->dalsi;
					while(E!=NULL && E->objekt_n==OBJEKT_akt->n)
					{
						if(E->geo.typ!=NULL)break;
						if(E->eID!=MaxInt && E->eID!=200)
						{
							E->X+=posunx;
							E->Y+=posuny;
							d.v.vloz_G_element(E,0,E->predchozi->geo.X4,E->predchozi->geo.Y4,0,0,0,0,F->d.Rxy(E).x,F->d.Rxy(E).y,E->geo.orientace);
							if(E->dalsi!=NULL)d.v.vloz_G_element(E->dalsi,0,E->geo.X4,E->geo.Y4,0,0,0,0,E->dalsi->geo.X4,E->dalsi->geo.Y4,E->dalsi->geo.orientace);
						}
						E=E->dalsi;
					}
					E=NULL;delete E;
				}
				else
				{
					//aktualizaace geometrie následujícího elementu
					if(pom_element_temp->dalsi!=NULL)
						d.v.vloz_G_element(pom_element_temp->dalsi,0,pom_element_temp->geo.X4,pom_element_temp->geo.Y4,0,0,0,0,pom_element_temp->dalsi->geo.X4,pom_element_temp->dalsi->geo.Y4,pom_element_temp->dalsi->geo.orientace,pom_element_temp->dalsi->geo.rotacni_uhel);
				}
			}
			else//došlo k posunu
			{
				if(m.delka(puv_souradnice.x,puv_souradnice.y,pom_element_temp->X,pom_element_temp->Y)>=1)//pokud byl posun větší něž jeden
				{
					pom_element_temp->Xt-=puv_souradnice.x-pom_element_temp->X;
					pom_element_temp->Yt-=puv_souradnice.y-pom_element_temp->Y;
				}
			}
			d.v.aktualizuj_cestu_teplomeru();
			duvod_validovat=2;
		}
		if(index_kurzoru==-11&&OBJEKT_akt->id==3&&Akce!=GEOMETRIE&&Akce!=GEOMETRIE_LIGHT)//editace rozmeru komor v POW
		{
			editovany_text=inDK(ms.MyToDouble(editovany_text));
			double posun=ms.MyToDouble(editovany_text)-pom_komora_temp->velikost;if(OBJEKT_akt->orientace==270 ||OBJEKT_akt->orientace==180)posun*=-1;
			if(OBJEKT_akt->orientace==90 || OBJEKT_akt->orientace==270)d.v.posun_hranu(posun,0,OBJEKT_akt->body->dalsi->dalsi,OBJEKT_akt->body->dalsi->dalsi->dalsi);
			else d.v.posun_hranu(0,posun,OBJEKT_akt->body->dalsi->dalsi,OBJEKT_akt->body->dalsi->dalsi->dalsi);
			double rozdil=-pom_komora_temp->velikost;
			pom_komora_temp->velikost=ms.MyToDouble(editovany_text);
			//změna délky pohonu
			rozdil+=pom_komora_temp->velikost;
			if(OBJEKT_akt->orientace==180 || OBJEKT_akt->orientace==270)rozdil*=-1;
			if(OBJEKT_akt->orientace==90 || OBJEKT_akt->orientace==270)OBJEKT_akt->element->X+=rozdil;
			else OBJEKT_akt->element->Y+=rozdil;
			d.v.vloz_G_element(OBJEKT_akt->element,0,OBJEKT_akt->element->geo.X1,OBJEKT_akt->element->geo.Y1,0,0,0,0,OBJEKT_akt->element->X,OBJEKT_akt->element->Y,OBJEKT_akt->element->geo.orientace);
		}
		if(index_kurzoru==-11&&OBJEKT_akt->id!=3&&Akce!=GEOMETRIE&&(pom_element_temp->eID==2||pom_element_temp->eID==4))pom_element_temp->mGrid->Refresh();//musí se refreshovat z důvodu přepočtu RT u S&G
    //editace rozměrů přímky v editaci geometrie
		if(index_kurzoru==-11&&(Akce==GEOMETRIE||Akce==GEOMETRIE_LIGHT))
		{
			////pokud je zadána nula dojde ke smazání úseku
			if(editovany_text=="" || ms.MyToDouble(editovany_text)==0)
			{
				if(Akce==EDITACE_TEXTU)Akce=NIC;Akce_temp=NIC;//v tomto případě musí být zde, což je prakticky konec
				smaz_usek_geometrie();
			}
			////načtení změny délky
			else
			{
				double posunx=0,posuny=0,double_text=inDK(ms.MyToDouble(editovany_text));
				if((DKunit==2||DKunit==3) && pom_element_temp->pohon!=NULL)double_text=double_text*pom_element_temp->pohon->aRD;//pokud jsou kóty v časovém režimu nutno přepočítat na vzdálenost
				////výpočet složek posunu
				if(pom_element_temp->geo.HeightDepp==0)double_text-=pom_element_temp->geo.delka;//zjištění posunu bez S/K
				else double_text-=pom_element_temp->geo.delkaPud;//zjištění posunu s S/K, na kótách zobrazeny Pud vzdálenosti
				//else if(pom_element_temp->geo.HeightDepp/double_text<1)double_text=double_text*cos(asin(pom_element_temp->geo.HeightDepp/double_text))-pom_element_temp->geo.delkaPud;//zjištění posunu ve S/K
        //else double_text=0;
				posunx=sin(DegToRad(pom_element_temp->geo.orientace+pom_element_temp->geo.rotacni_uhel))*double_text;
				posuny=cos(DegToRad(pom_element_temp->geo.orientace+pom_element_temp->geo.rotacni_uhel))*double_text;   //Memo("posun: ["+String(posunx)+"; "+String(posuny)+"]");
				////posun editovaného elementu
				d.v.vloz_G_element(pom_element_temp,0,pom_element_temp->geo.X1,pom_element_temp->geo.Y1,0,0,0,0,pom_element_temp->geo.X4+posunx,pom_element_temp->geo.Y4+posuny,pom_element_temp->geo.orientace,pom_element_temp->geo.rotacni_uhel);
				pom_element_temp->X+=posunx;pom_element_temp->Y+=posuny;//souřadnice elementu
				//pom_element_temp=pom_element_temp->dalsi;
	   		////posun ostatních elementů a jejich geometrie
				Cvektory::TElement *E=NULL;
				Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
	   		bool prvni=true;//pomocná proměná, přeskočení úpravy souřadnic pro první, první element musí být součástí průchodu, např. úprava délky úseku před výhybkou by ovlivnila jen hlavní větev pokud bych nezačal průchodový alg. z výhybky
				while(pom_element_temp!=NULL && pom_element_temp->objekt_n==OBJEKT_akt->n)
	   		{
	   			if(!prvni)//neprovádět na prvním elementu, již bylo provedeno
	   			{
	   				pom_element_temp->X+=posunx;pom_element_temp->Y+=posuny;//souřadnice elementu
	   				//geometrie elementu
	   				pom_element_temp->geo.X1+=posunx;pom_element_temp->geo.X2+=posunx;pom_element_temp->geo.X3+=posunx;pom_element_temp->geo.X4+=posunx;
	   				pom_element_temp->geo.Y1+=posuny;pom_element_temp->geo.Y2+=posuny;pom_element_temp->geo.Y3+=posuny;pom_element_temp->geo.Y4+=posuny;
	   			}
	   			else prvni=false;//prošel jsem skrze první
	   			E=pom_element_temp;
					pom_element_temp=d.v.dalsi_krok(VYHYBKY,pom_element_temp,OBJEKT_akt);
	   			if(pom_element_temp!=NULL && pom_element_temp->eID==301 && pom_element_temp->predchozi2==E)break;
				}
				d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
				E=NULL;delete E;
			}
		}
		if(Akce==EDITACE_TEXTU)Akce=NIC;Akce_temp=NIC;//musí být na konci!
	}
	else if((Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU) && editovany_text!="")//pokud neexistuje otevřený objekt + očetření proti zapsání prázdné hodnoty
	{
		//deklarace pomocných proměnných
		double puv=0,nov=inDK(ms.MyToDouble(editovany_text)),posun=0;
		Cvektory::TBod *A=pom_bod_temp->predchozi;
		if(pom_bod_temp->n==1)A=d.v.HALA.body->predchozi;
		//výpočet posunu (jednotlivé souřadnice bodu) + posun
		puv=m.round2double(m.delka(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),3);
		posun=nov-puv;
		d.v.posun_bod(pom_bod_temp->X+sin(DegToRad(fmod(m.azimut(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),360)))*posun,pom_bod_temp->Y+cos(DegToRad(fmod(m.azimut(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),360)))*posun,pom_bod_temp);
		A=NULL;delete A;
	}
	//využívání pro uchování ukazatele při editaci kót
	pom_element_temp=NULL; delete pom_element_temp;
	pom_komora_temp=NULL; delete pom_komora_temp;
	pom_bod_temp=NULL; delete pom_bod_temp;
	//pro jistotu na konec
	if(Akce==EDITACE_TEXTU)Akce=NIC;Akce_temp=NIC;editovany_text="";index_kurzoru=0;
	if(Akce==GEOMETRIE || Akce==GEOMETRIE_LIGHT){d.SCENA=1111111;vytvor_statickou_scenu();}
	else d.SCENA=0;
	REFRESH(true);//uvolnění rastru, překreslení i tabulek (editace názvu elementu
	//nahled_ulozit(true);//přesunuto na začátek editace textů (mousedown)
	vytvor_obraz();
}
//---------------------------------------------------------------------------
//vypnutí/zapnutí zamčení editace
void __fastcall TForm1::scButton_zamekClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==GEOMETRIE)ukonceni_geometrie();//ukončení pouze geometrie, ESC zde zlobí
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//ukončení editace
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
	{
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
	}
	if(scButton_zamek->ImageIndex==37)//zamčeno budu odemykat
	{
		OBJEKT_akt->uzamknout_nahled=false;
		scButton_zamek->ImageIndex=60;//odemčeno
		Schema->ImageIndex=-1;//78
		scButton_zamek->Hint=ls->Strings[120];//"Zamknout editaci";
	}
	else//odemčeno budu zamykat
	{
		OBJEKT_akt->uzamknout_nahled=true;
		scButton_zamek->ImageIndex=37;
		Schema->ImageIndex=79;
		scButton_zamek->Hint=ls->Strings[119];//"Odemknout editaci";
	}
	nahled_ulozit(true);
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();
	pom_element=NULL;
	JID=-1;
	nastav_focus();//vrací focus na knihovnu - důležité z důvodu keydown
	REFRESH();
}
//---------------------------------------------------------------------------
//přepíná viditelnost mGridů
void __fastcall TForm1::scGPButton_viditelnostmGridClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//ukončení editace
	if(Akce==NIC)//pouze pokud není žádná akce
	{
		//pokud je otevřené menu nebo options zavře je
  	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
  	{
  		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
  		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
  		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
  		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
  	}
  	if(scGPButton_viditelnostmGrid->ImageIndex==55)//vypnuto budu zapínat
  	{
  		scGPButton_viditelnostmGrid->ImageIndex=54;//zapnuto
  		scGPButton_viditelnostmGrid->Hint=ls->Strings[121];//"Skrýt tabulky";
  		OBJEKT_akt->zobrazit_mGrid=true;
  	}
  	else//zapnuto budu vypínat
  	{
  		scGPButton_viditelnostmGrid->ImageIndex=55;
  		scGPButton_viditelnostmGrid->Hint=ls->Strings[122];//"Zobrazit tabulky";
  		OBJEKT_akt->zobrazit_mGrid=false;
  	}
  	nahled_ulozit(true);
  	nastav_focus();
		REFRESH();
	}
}
//---------------------------------------------------------------------------
//přepíná viditelnost kót
void __fastcall TForm1::scGPButton_viditelnostKotyClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//ukončení editace
	if(Akce==NIC)//pouze pokud není žádná akce
	{
  	//pokud je otevřené menu nebo options zavře je
  	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
  	{
  		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
  		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
  		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
  		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
  	}
  	if(scGPButton_viditelnostKoty->ImageIndex==57)//vypnuto budu zapínat
  	{
  		scGPButton_viditelnostKoty->ImageIndex=56;//zapnuto
  		scGPButton_viditelnostKoty->Hint=ls->Strings[123];//"Skrýt kóty";
  		OBJEKT_akt->zobrazit_koty=true;
  	}
  	else//zapnuto budu vypínat
  	{
  		scGPButton_viditelnostKoty->ImageIndex=57;
  		scGPButton_viditelnostKoty->Hint=ls->Strings[124];//"Zobrazit kóty";
  		OBJEKT_akt->zobrazit_koty=false;
  	}
  	nahled_ulozit(true);
  	nastav_focus();
		REFRESH();
	}
}
//---------------------------------------------------------------------------
//přepínání posunu dalších elementů
void __fastcall TForm1::scGPButton_posun_dalsich_elementuClick(TObject *Sender)
{
	log(__func__);//logování
	//if(Akce==GEOMETRIE)ukonceni_geometrie();//ukončení pouze geometrie, ESC zde zlobí
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//ukončení editace
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
	{
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
	}
	if(scGPButton_posun_dalsich_elementu->ImageIndex==59)//vypnuto budu zapínat
	{
		scGPButton_posun_dalsich_elementu->ImageIndex=58;
		posun_dalsich_elementu=true;
		scGPButton_posun_dalsich_elementu->Hint=ls->Strings[125];//"Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";
	}
	else
	{
		scGPButton_posun_dalsich_elementu->ImageIndex=59;
		posun_dalsich_elementu=false;
		scGPButton_posun_dalsich_elementu->Hint=ls->Strings[126];//"Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";
	}
	nastav_focus();
}
//---------------------------------------------------------------------------
//combo pro přepínání typu kót
void __fastcall TForm1::scGPComboBox_prepinacKotClick(TObject *Sender)
{
	log(__func__);//logování
	if(MOD==EDITACE)
	{
  	if(Akce==GEOMETRIE)ukonceni_geometrie();//ukončení pouze geometrie, ESC zde zlobí
		if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//ukončení editace
  	//pokud je otevřené menu nebo options zavře je
  	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)
  	{
  		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;
  		if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
  		if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
  		if(OBJEKT_akt!=NULL)DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;
  	}
		if(FormX->input_state==FormX->NOTHING)//ošetření proti spouštění 2x při změně COMBA v tabulce pohonu
  	{
  		//není nutno provádět kontrolu, prováděna jinde -> aktivace / deaktivace komponenty
  		refresh_mGrid=false;
  		switch(scGPComboBox_prepinacKot->ItemIndex)
  		{
  			case 0://nastavena délka
  			{
  				/*if(DKunit==SEKUNDY)DKunit=M;else*/ DKunit=MM;//překlopění základních na základní, ..
  			}break;
  			case 1://nastaven čas
  			{
  				/*if(DKunit==M)*/DKunit=SEKUNDY;/*else DKunit=MINUTY;*///překlopění základních na základní, ..
  			}break;
  		}
  		writeINI("nastaveni_nahled","koty_delka", DKunit);
  		REFRESH(false);
  		refresh_mGrid=true;//navrácení stavu
		}
	}
}
//---------------------------------------------------------------------------
//urychlení vypsání do Memo_testy
void TForm1::Memo(AnsiString Text, bool clear,bool count,bool copyFinalTextToClipboard)
{
  log(__func__);//logování
	if(clear)Memo_testy->Clear();
	if(count){count_memo++;Text=Text+" "+AnsiString(count_memo);}
	if(!scGPPanel_bottomtoolbar->Visible)Memo_testy->Top=scGPPanel_statusbar->Top-Memo_testy->Height;else Memo_testy->Top=scGPPanel_bottomtoolbar->Top-Memo_testy->Height;
	Memo_testy->Width=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
	Memo_testy->Left=scSplitView_LEFTTOOLBAR->Width;
	Memo_testy->Visible=true;
	Memo_testy->Lines->Add(Text);
	if(copyFinalTextToClipboard)
	{
		Memo_testy->SelectAll();
		Memo_testy->CopyToClipboard();
  }
}
//---------------------------------------------------------------------------
//zapínání a vypínaní meření
void __fastcall TForm1::scGPImage_mereni_vzdalenostClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==EDITACE_TEXTU || Akce_temp==EDITACE_TEXTU)smaz_kurzor();//ukončení editace
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	////zapnutí akce
	if(Akce!=MAGNETICKE_LASO)
	{
		//zapnutí akceá
		if(Akce!=NIC)ESC();
		kurzor(add_o);
		scGPButton_zmerit_vzdalenost->Options->FrameNormalColor=(TColor)RGB(86,120,173);
		scGPButton_zmerit_vzdalenost->Options->NormalColor=(TColor)RGB(86,120,173);
		Timer_getjobid->Enabled=false;//odstavení timeru, není potřeba
		Akce=MAGNETICKE_LASO;//až po refresh
		count_memo=0;//ukazatel kolikrát proběhl cyklus načtení dat ze zakázky, pouze jednou
		pocatek_mereni.x=-1;pocatek_mereni.y=-1;
		konec_mereni.x=-1;konec_mereni.y=-1;
    d.SCENA=1111111;//ZprVozEledElesDopObjHal - vozíky nutno nechat v dynamické (jinak budou překresleny bufferem)
		vytvor_statickou_scenu();//vypnutí vrstvy errorů a nastavení zbytku na statickou scénu
		zobraz_tip(ls->Strings[483]);//má v sobě REFRESH(), je nutné volat až po vytvořeni statické scény
	}
  ////vypnutí akce
	else
	{
		Akce=NIC;
		kurzor(standard);
		//zjištění délky a času, první a poslední segment cesty jsou fiktivní elementy
		if(d.v.MAG_LASO->dalsi!=NULL && d.v.MAG_LASO->predchozi->Element->n==MaxInt)
		{
			Form_mereni->ShowModal();
		}
		//měření v časových osách
		if(pocatek_mereni.x!=-1 && pocatek_mereni.y!=-1)
		{
			MB(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"Čas = "+String(m.round2double(m.abs_d((konec_mereni.x-pocatek_mereni.x)/d.PX2SEC*3),2))+" [s]","",MB_OK,true,false,366,true,true);
		}
		scGPImage_mereni_vzdalenost->ClipFrameFillColor=clWhite;
		scGPButton_zmerit_vzdalenost->Options->FrameNormalColor=scGPButton_zmerit_vzdalenost->Options->HotColor;
		scGPButton_zmerit_vzdalenost->Options->NormalColor=scGPButton_zmerit_vzdalenost->Options->HotColor;
		//odstranění stavů
		if(d.v.MAG_LASO->sparovany!=NULL)d.v.MAG_LASO->sparovany->stav=1;
		if(d.v.MAG_LASO->predchozi!=NULL && d.v.MAG_LASO->predchozi->sparovany!=NULL)d.v.MAG_LASO->predchozi->sparovany->stav=1;
		//smazání seznamu MAG_LASO
		d.v.vymaz_seznam_MAG_LASO();
		d.v.hlavicka_MAG_LASO();
		Timer_getjobid->Enabled=true;
		if(d.v.SIM!=NULL)d.SCENA=221111;
		else
		{
      if(OBJEKT_akt!=NULL)d.SCENA=0;
			else d.SCENA=1111111;//ZprVozEledElesDopObjHal
		}
		if(!Timer_simulace->Enabled)
		{
      vytvor_statickou_scenu();//navracení vykreslení errorů
			REFRESH();
		}
	}
	//vrátí focus na form
	nastav_focus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPSwitch_robot_clovekChangeState(TObject *Sender)
{
	log(__func__);//logování
	DrawGrid_knihovna->Refresh();
	ESC();//případné vypnutí akce
}
//---------------------------------------------------------------------------
//aktualizace popisků v tabulkách při změně jazyka
void TForm1::change_languagein_mGrid()
{
	log(__func__);//logování
	if(PmG!=NULL)
	{
    AnsiString aRD,R,Rz,cas;
  	//nastavení jednotek podle posledních nastavení
  	if (aRDunit==SEC) aRD="<a>[m/s]</a>";
  	else aRD="<a>m/min</a>";
  	if (Runit==M) R="<a>[m]</a>";
  	else R="<a>[mm]</a>";//1
  	if (Rzunit==M) Rz="<a>[m]</a>";
		else Rz="<a>[mm]</a>";
  	if (PTunit==SEC) cas="<a>[s]</a>";
		else cas="<a>[min]</a>";
		//změna popisků zde !!!
		PmG->Cells[2][0].Text=ls->Strings[447];//"Výběr pohonu "
  	PmG->Cells[2][1].Text=ls->Strings[451];//"Nastavená"
  	PmG->Cells[2][2].Text=ls->Strings[448];//"Rozmezí"
  	PmG->Cells[2][4].Text=ls->Strings[209];//"Palce"
  	PmG->Cells[2][5].Text=ls->Strings[210];//"Násobek"
  	PmG->Cells[2][3].Text=ls->Strings[211];//"Jigů"
  	PmG->Cells[2][6].Text=ls->Strings[212];//"Podvozky"
  	PmG->Cells[2][7].Text=ls->Strings[213];//"Jigy 0°"
  	PmG->Cells[2][8].Text=ls->Strings[214];//"Jigy 90°"
  	//nastavení sloučených sloupců
  	PmG->Cells[0][2].Text=ls->Strings[452];//+" "+rychlost;//"Rychlost"
  	PmG->Cells[1][2].Text=aRD;//jednotky
  	PmG->Cells[0][5].Text=ls->Strings[453];//"Rozteč"
  	PmG->Cells[1][4].Text=R;//jednotky
  	PmG->Cells[0][8].Text=ls->Strings[215];//"Mezera"
		PmG->Cells[1][8].Text=Rz;//jednotky
		aktualizace_tab_pohon(false,false,false);//aktualizace popisků
		////aktualizace comba
		tab_pohon_COMBO();
		////aktualizace validace
		if(PmG_VID!=0)FormX->validace_aRD();
		//if(OBJEKT_akt->zobrazit_mGrid)REFRESH(true);//zabránění probliku tabulek pokud jsou skryté
	}
	////aktualizace mGrid_knihovna
	if(mGrid_knihovna!=NULL)
	{
		switch(language)
		{
			case CS:
			{
				mGrid_knihovna->Cells[0][1].ImageIndex=15;
				mGrid_knihovna->Cells[0][4].ImageIndex=0;
				mGrid_knihovna->Cells[1][4].ImageIndex=9;
				mGrid_knihovna->Cells[0][7].ImageIndex=2;
				mGrid_knihovna->Cells[0][8].ImageIndex=5;
				mGrid_knihovna->Cells[1][8].ImageIndex=6;
				mGrid_knihovna->Cells[0][9].ImageIndex=7;
				mGrid_knihovna->Cells[0][10].ImageIndex=8;
				mGrid_knihovna->Cells[0][12].ImageIndex=12;
			}break;
			case EN:
			{
				mGrid_knihovna->Cells[0][1].ImageIndex=27;
				mGrid_knihovna->Cells[0][4].ImageIndex=24;
				mGrid_knihovna->Cells[1][4].ImageIndex=25;
				mGrid_knihovna->Cells[0][7].ImageIndex=17;
				mGrid_knihovna->Cells[0][8].ImageIndex=19;
				mGrid_knihovna->Cells[1][8].ImageIndex=20;
				mGrid_knihovna->Cells[0][9].ImageIndex=21;
	    	mGrid_knihovna->Cells[0][10].ImageIndex=22;
				mGrid_knihovna->Cells[0][12].ImageIndex=26;
			}break;
		}
		if(OBJEKT_akt==NULL)mGrid_knihovna->Refresh();
	}
	//aktualizace popisků knihoven
	if(OBJEKT_akt!=NULL)
	{
		if(OBJEKT_akt->id==4 || OBJEKT_akt->id==2 || OBJEKT_akt->id==5)scGPLabel_roboti->Caption=ls->Strings[55];//"Robot           Operátor";//mezery tvoří místo, kde je zobrazen switch
		else if(OBJEKT_akt->id==3)scGPLabel_roboti->Caption=ls->Strings[56];//"Sekce";
		else if(OBJEKT_akt->id==0 || OBJEKT_akt->id==9)scGPLabel_roboti->Caption=ls->Strings[57];//"Operátoři";}
		else scGPLabel_roboti->Caption=ls->Strings[58];//"Roboti";
		scGPLabel_otoce->Caption=ls->Strings[59];//"Otoče";
		scGPLabel_stop->Caption=ls->Strings[60];//"Stop stanice";
		scGPLabel_geometrie->Caption=ls->Strings[61];//"Geometrie linky";
	}
	else
	{
		scGPLabel_roboti->Caption=ls->Strings[50];//"Hala";
		scGPLabel_otoce->Caption=ls->Strings[51];//"Nav/Svěšování";
		scGPLabel_stop->Caption=ls->Strings[52];//"Technologie";
		scGPLabel_geometrie->Caption=ls->Strings[53];//"Ostatní";
	}
	//aktualizace popisků tabulek elementů
	if(OBJEKT_akt!=NULL)
	{
		pom_element=NULL;//pro jistotu
		JID=0;
		redesign_element();
  }
}
//---------------------------------------------------------------------------
//načte zvolený jazyk
unsigned short TForm1::load_language(Tlanguage language,bool akt_mGrid)
{
	log(__func__);//logování

	////nastavení adresáře k místě aplikace
  ChDir(ExtractFilePath(Application->ExeName));    //přesune k EXE
	UnicodeString File_language="TISPL.language";

	////naplnění pro konkrétní jazykovou mutaci
	if(FileExists(File_language))//znovu kontrola po případném stažení souboru
	{
		//načtení jazykového slovníku do temp string listu
		TStringList *ls_temp=new TStringList;
		ls_temp->LoadFromFile(File_language);
		//kontrola zda je v jazykovém slovníku stejný počet záznamů jako má být, pokud ne je použit default
		if(ls_temp->Count==ls->Count)
		{
			ls->LoadFromFile(File_language);
			//vypársování daného jazyka a navrácení do string listu již jen vypársovaného
	  	for(int i=0;i<ls->Count;i++)
			{
	  		switch(language)
	  		{
					case EN:ls->Strings[i]=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);Jazyk=EN; /*anglictina1->Checked=true;mongolstina1->Checked=false;cestina1->Checked=false;*/break;
	  			case MN:ls->Strings[i]=ls->Strings[i].Delete(1,ls->Strings[i].Pos(";"));ls->Strings[i]=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);Jazyk=MN;/*anglictina1->Checked=false;mongolstina1->Checked=true;cestina1->Checked=false;*/break;
	  			case CS:ls->Strings[i]=ls->Strings[i].Delete(1,ls->Strings[i].Pos(";"));ls->Strings[i]=ls->Strings[i].SubString(ls->Strings[i].Pos(";")+1,ls->Strings[i].Length());Jazyk=CS;/*anglictina1->Checked=false;mongolstina1->Checked=false;cestina1->Checked=true;*/break;
					default:EN:ls->Strings[i]=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);Jazyk=EN;/*anglictina1->Checked=true;mongolstina1->Checked=false;cestina1->Checked=false;*/break;
				}
			}
		}
		else
		{
			if(DEBUG && FileExists("../../TISPL.language"))//vývojářské featura, slouží jenom pro úsporu času při vývoji, případně by mohlo řešit i v realesu pro uživatele stažení z netu
			{
				ShowMessage("Byl nalezen nový jazykový slovník. Po stisku OK se přepíše původní slovník uložený u EXE aplikace. Tato hláška se vypisuje pouze v DEBUGu.");
				CopyFile(_T("../../TISPL.language"),_T("TISPL.language"),false);//přepíše starý jazykový slovník novým
				load_language(language,false);//rekurzní zavolání znovu metody
			}
			else
			{
				if(language==CS)MB("Jazykový soubor chybí, nebo není kompletní!");//vypsáno ručně pro různé jazykové verze
				else MB("Language file missing or incomplete!");
				language=CS;//nastavení na default jazyk
			}
		}
		delete ls_temp;ls_temp=NULL;
		//začít od 4!!!
		N11->Caption=ls->Strings[4];
		scGPGlyphButton_odstran_cestu->Hint=ls->Strings[4];
		N21->Caption=ls->Strings[5];
		//zakomentované == volné
//		RzToolButton11->Hint=ls->Strings[15];
//		RzToolButton12->Hint=ls->Strings[16];
//		Button4->Caption=ls->Strings[17];
    ButtonPLAY_O->Caption=ls->Strings[18];
    Konec->Caption=ls->Strings[19];
		if(OBJEKT_akt==NULL)Schema->Caption=ls->Strings[20];
		else Schema->Caption=ls->Strings[36];
    PopupMenuButton->Caption=ls->Strings[21];
    PopupMenuButton->Hint=ls->Strings[22];
    //Synteza->Caption=ls->Strings[23];
    Toolbar_Ulozit->Hint=ls->Strings[24];
    Toolbar_NovySoubor->Hint=ls->Strings[25];
    Toolbar_Otevrit->Caption=ls->Strings[26];
    Toolbar_Otevrit->Hint=ls->Strings[27];
    DetailsButton->Hint=ls->Strings[28];
		Layout->Caption=ls->Strings[29];
    //Simulace->Caption=ls->Strings[30];
    scGPGlyphButton_napoveda->Hint=ls->Strings[31];
    //scLabel_klient->Caption=ls->Strings[32];
    //scLabel_architekt->Caption=ls->Strings[33];
   // scGPButton_header_projekt->Hint=ls->Strings[34];
   // scGPGlyphButton_OPTIONS->Hint=ls->Strings[35];
    Nahled->Caption=ls->Strings[36];
		scGPGlyphButton_PLAY->Hint=ls->Strings[37];
		scLabel_statusbar_1->Hint=ls->Strings[38];
		scLabel_statusbar_2->Hint=ls->Strings[40];
		if(prichytavat_k_mrizce==1)scLabel_statusbar_0->Caption=ls->Strings[375];
		else scLabel_statusbar_0->Caption=ls->Strings[388];
		scLabel_statusbar_0->Hint=ls->Strings[42];
		if(prichytavat_k_mrizce==1)scGPButton_prichytavat->Hint=ls->Strings[411];
		else scGPButton_prichytavat->Hint=ls->Strings[410];
		scButton_zamek_layoutu->Hint=ls->Strings[43];
		scGPButton_zmerit_vzdalenost->Hint=ls->Strings[44];
    scListGroupKnihovObjektu->Caption=ls->Strings[45];
    scListGroupPanel_hlavickaOstatni->Caption=ls->Strings[46];
    scListGroupPanel_hlavickaOtoce->Caption=ls->Strings[47];
		scListGroupPanel_geometrie->Caption=ls->Strings[48];
  	scListGroupPanel_poznamky->Caption=ls->Strings[49];
		scGPLabel_poznamky->Caption=ls->Strings[54];
		//scGPGlyphButton_close_grafy->Caption=ls->Strings[62];
   //scExPanel_log_header->Caption=ls->Strings[63];
    scExPanel_html->Caption=ls->Strings[64];
		//pohonobjekt->Caption=ls->Strings[65];
    //scGPGlyphButton_close_legenda_casove_osy->Caption=ls->Strings[66];
		//CheckBox_pouzit_zadane_kapacity_OLD->Caption=ls->Strings[67];
    //CheckBoxAnimovatSG_OLD->Caption=ls->Strings[68];
		//scGPLabel1->Caption=ls->Strings[69];
		scGPButton_ulozit->Caption=ls->Strings[70];
		scGPButton_zahodit->Caption=ls->Strings[71];
		scGPLabel_prepinacKot->Caption=ls->Strings[72];
		scGPImage_mereni_vzdalenost->Hint=ls->Strings[73];
    scGPImage_zamek_posunu->Hint=ls->Strings[74];
   // ButtonPLAY->Hint=ls->Strings[75];
   // CheckBox_pouzit_zadane_kapacity->Caption=ls->Strings[76];
   // CheckBoxAnimovatSG->Caption=ls->Strings[77];
   // CheckBoxVymena_barev->Caption=ls->Strings[78];
   // CheckBoxVytizenost->Caption=ls->Strings[79];
   // scGPCheckBox_pocet_voziku_dle_WIP->Caption=ls->Strings[80];
   // scLabel_filtrovat->Caption=ls->Strings[81];
   // scLabel_doba_cekani->Caption=ls->Strings[82];
   // scGPButton_generuj->Hint=ls->Strings[83];
    //scGPButton_header_def_zakazek->Hint=ls->Strings[84];
    scButton_konec->Caption=ls->Strings[85];
    scButton_otevrit->Caption=ls->Strings[86];
    scButton4->Caption=ls->Strings[87];
    scButton_zaloha->Caption=ls->Strings[88];
    scButton_report->Caption=ls->Strings[89];
    scLabel11->Caption=ls->Strings[90];
    scButton_ulozjako->Caption=ls->Strings[91];
    scButton_export->Caption=ls->Strings[92];
    scButton_posledni_otevreny->Caption=ls->Strings[93];
    scButton_novy->Caption=ls->Strings[94];
    scButton_ulozit->Caption=ls->Strings[95];
    scButton_nacist_podklad->Caption=ls->Strings[96];
    pravyoption_nadpis->Caption=ls->Strings[97];
    scExPanel_vrstvy->Caption=ls->Strings[98];
    scGPCheckBox_zobrazit_koleje->Caption=ls->Strings[99];
    scGPCheckBox_zobrazit_pozice->Caption=ls->Strings[100];
		scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Caption=ls->Strings[101];
    scGPCheckBox1_popisky->Caption=ls->Strings[102];
    scExPanel_podklad->Caption=ls->Strings[103];
		scGPButton_adjustace->Caption=ls->Strings[104];
    scGPButton_kalibrace->Caption=ls->Strings[105];
    scGPCheckBox_stupne_sedi->Caption=ls->Strings[106];
    scGPCheckBox_zobraz_podklad->Caption=ls->Strings[107];
    scLabel1_svetelnost->Caption=ls->Strings[108];
		scExPanel_ostatni->Caption=ls->Strings[109];
//		scGPCheckBox_ortogon->Caption=ls->Strings[110];
//    scGPCheckBox_ortogon->Hint=ls->Strings[111];
		scLabel16->Caption=ls->Strings[112];
    scLabel5->Caption=ls->Strings[113];
    scLabel1_intenzita->Caption=ls->Strings[114];
    scGPTrackBar_intenzita->Hint=ls->Strings[115];
    scExPanel1->Caption=ls->Strings[116];
    scGPButton_mereni_vzdalenost->Caption=ls->Strings[117];
  	Analyza->Caption=ls->Strings[118];
    //Form_parametry_linky->rHTMLLabel_eDesigner->Caption=ls->Strings[127];
    //Form_parametry_linky->rHTMLLabel_doporuc_pohony->Caption=ls->Strings[128];
    Form_parametry_linky->scHTMLLabel_podlahovy->Caption=ls->Strings[129];
    Form_parametry_linky->scHTMLLabel_podvesny->Caption=ls->Strings[130];
    Form_parametry_linky->Button_save->Caption=ls->Strings[131];
    Form_parametry_linky->Button_storno->Caption=ls->Strings[132];
    Form_parametry_linky->Konec->Caption=ls->Strings[133];
    Form_parametry_linky->scLabel1->Caption=ls->Strings[134];
    Form_parametry_linky->scGPGlyphButton_info->Caption=ls->Strings[135];
    //Form_parametry_linky->scGPGlyphButton_refresh->Caption=ls->Strings[136];
    //Form_parametry_linky->scGPGlyphButton_refresh->Hint=ls->Strings[137];
    Form_parametry_linky->scGPButton_vozik->Caption=ls->Strings[138];
    Form_parametry_linky->scGPButton_pohon->Caption=ls->Strings[139];
    //Form_parametry_linky->scExPanel_doporuc_pohony->Caption=ls->Strings[140];
    //Form_parametry_linky->scGPGlyphButton_add_mezi_pohony->Hint=ls->Strings[141];
    //Form_parametry_linky->scGPButton_jig->Caption=ls->Strings[142];
    Form_parametry_linky->scLabel_zobrazit_parametry->Caption=ls->Strings[143];
    Form_parametry_linky->scLabel_smazat->Caption=ls->Strings[144];
    Form_parametry_linky->scLabel_smazat_nepouzite->Caption=ls->Strings[145];
    Form_parametry_linky->scLabel_kopirovat->Caption=ls->Strings[146];
    Form_parametry_linky->scLabel_nastavit_parametry->Caption=ls->Strings[147];
    //Form_parametry_linky->scGPGlyphButton_DEL_nepouzite->Caption=ls->Strings[148];
    //Form_parametry_linky->scGPGlyphButton_DEL_nepouzite->Hint=ls->Strings[149];
    //Form_parametry_linky->scGPGlyphButton_TT->Hint=ls->Strings[150];
    //Form_parametry_linky->scGPGlyphButton_vozik_edit->Hint=ls->Strings[151];
    Form_parametry_linky->scGPGlyphButton_ADD->Hint=ls->Strings[152];
    Form_parametry_linky->scGPButton_obecne->Caption=ls->Strings[153];
    //Form_parametry_linky->rHTMLLabel_JIG->Caption=ls->Strings[154];
    //Form_parametry_linky->rHTMLLabel_podvozek_zaves->Caption=ls->Strings[155];
    Form_parametry_linky->scHTMLLabel_jig_info->Caption=ls->Strings[156];
    Form_parametry_linky->scGPGlyphButton_ADD_old->Hint=ls->Strings[157];
    Form_parametry_linky->scGPGlyphButton_OPTIONS->Hint=ls->Strings[158];
    Form_parametry_linky->scGPGlyphButton_katalog->Hint=ls->Strings[159];
    Form_parametry_linky->scGPTrackBar_uchyceni->Hint=ls->Strings[160];
		Form_parametry_linky->Button_save->Caption=ls->Strings[161];
		Form_parametry_linky->scHTMLLabel1->Caption=ls->Strings[162];
    PopUPmenu->scLabel_posun_obrysu->Caption=ls->Strings[175];
    PopUPmenu->scLabel_rychly_export->Caption=ls->Strings[176];
    PopUPmenu->scLabel_posouvat->Caption=ls->Strings[177];
    PopUPmenu->scLabel_posunout->Caption=ls->Strings[178];
    PopUPmenu->scLabel_priblizit->Caption=ls->Strings[179];
    PopUPmenu->scLabel_oddálit->Caption=ls->Strings[180];
    PopUPmenu->scLabel_vybrat_oknem->Caption=ls->Strings[181];
		PopUPmenu->scLabel_cely_pohled->Caption=ls->Strings[182];
		PopUPmenu->scLabel_poznamka->Caption=ls->Strings[183];
		PopUPmenu->scLabel_rozmistit_mgridy->Caption=ls->Strings[14];
    myMessageBox->scGPGlyphButton_copy->Hint=ls->Strings[184];
		myMessageBox->CheckBox_pamatovat->Caption=ls->Strings[189];
		Form_katalog->scLabel_header->Caption=ls->Strings[192];
		Form_katalog->Button_save->Caption=ls->Strings[194];
		Form_katalog->Button_storno->Caption=ls->Strings[195];
		scGPCheckBox_zobrazit_palce->Caption=ls->Strings[292];
		scGPCheckBox_rozmisteni_voziku->Caption=ls->Strings[445];
		Form_zpravy->scLabel_header->Caption=ls->Strings[412];
		Form_zpravy->scGPGlyphButton_pripnout->Hint=ls->Strings[413];
		//Form_zpravy->scLabel_chyby->Caption=ls->Strings[414];
		//Form_zpravy->scLabel_varovani->Caption=ls->Strings[415];
		Form_zpravy->Skryt->Hint=ls->Strings[520];
		Form_mereni->scLabel_header->Caption=ls->Strings[521];
		Form_mereni->scGPGlyphButton_pripnout->Hint=ls->Strings[413];
		Form_mereni->Skryt->Hint=ls->Strings[520];
		Form_parametry_linky->scHTMLLabel_posuvnik->Caption=ls->Strings[427];
		scGPButton_smazat->Caption=ls->Strings[428];
		scGPButton_geometrie->Hint=ls->Strings[443];
		scGPGlyphButton_undo->Hint=ls->Strings[449];
		scGPGlyphButton_redo->Hint=ls->Strings[450];
		scGPButton_bug_report->Caption=ls->Strings[458];
		scGPCheckBox_popisek_pohonu->Caption=ls->Strings[461];
		scGPButton_error->Hint=ls->Strings[522];
		scGPButton_warning->Hint=ls->Strings[522];

    //změna zarovnání
		scGPComboBox_prepinacKot->Left=scGPLabel_prepinacKot->Left+scGPLabel_prepinacKot->Width;//nutné!!
		switch((int)language)
		{
			case CS:
			{
				Form_parametry_linky->scGPGlyphButton_ADD_old->Left=98;
			}break;
			case EN:
			{
				Form_parametry_linky->scGPGlyphButton_ADD_old->Left=147;
			}break;
		}
		//aktualizace mGridu
		if(akt_mGrid)change_languagein_mGrid();
		//aktualizace hintů tlačítek v editaci
		if(OBJEKT_akt!=NULL)//stačí pouze při otevřeném náhledu, při otevýrání nového se aktualizace provede v otevri_editaci
		{
      if(OBJEKT_akt->uzamknout_nahled)
   		{
   			scButton_zamek->ImageIndex=37; //zamčeno
   			scButton_zamek->Hint=ls->Strings[119];//"Odemknout editaci";
   		}
   		else
   		{
   			scButton_zamek->ImageIndex=60;
   			scButton_zamek->Hint=ls->Strings[120];//"Zamknout editaci";
   		}
   		if(OBJEKT_akt->zobrazit_mGrid)
   		{
   			scGPButton_viditelnostmGrid->ImageIndex=54;
   			scGPButton_viditelnostmGrid->Hint=ls->Strings[121];//"Skrýt tabulky";
   		}
   		else
   		{
   			scGPButton_viditelnostmGrid->ImageIndex=55;
				scGPButton_viditelnostmGrid->Hint=ls->Strings[122];//"Zobrazit tabulky";
   		}
   		if(OBJEKT_akt->zobrazit_koty)
   		{
   			scGPButton_viditelnostKoty->ImageIndex=56;
   			scGPButton_viditelnostKoty->Hint=ls->Strings[123];//"Skrýt kóty";
   		}
   		else
   		{
   			scGPButton_viditelnostKoty->ImageIndex=57;
   			scGPButton_viditelnostKoty->Hint=ls->Strings[124];//"Zobrazit kóty";
   		}
   		if(posun_dalsich_elementu)
   		{
   			scGPButton_posun_dalsich_elementu->ImageIndex=58;
   			scGPButton_posun_dalsich_elementu->Hint=ls->Strings[125];//"Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";
   		}
   		else
   		{
   			scGPButton_posun_dalsich_elementu->ImageIndex=59;
   			scGPButton_posun_dalsich_elementu->Hint=ls->Strings[126];//"Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";
			}
		}
		//aktualizace SB
		if(d.v.OBJEKTY->dalsi==NULL)SB(ls->Strings[376],4);
		else SB(ls->Strings[378],4);
	}
	else //pokud není nalezen jazykový slovník
	{
		if(language==CS)MB("Jazykový soubor chybí, nebo není kompletní!");//vypsáno ručně pro různé jazykové verze
		else MB("Language file missing or incomplete!");
		language=CS;//nenalezen soubor, jazyk čeština
		scGPSwitch1->Enabled=false;
	}
	return language;
}
//zapnutí posunu haly
void __fastcall TForm1::scGPButton_posun_halyClick(TObject *Sender)
{
	log(__func__);//logování
	if(d.v.HALA.body!=NULL && OBJEKT_akt==NULL)//pokud existuje hala a není aktivní editace objektu, jinak nemá smysl
	{
		scSplitView_OPTIONS->Close();
		kurzor(pan);
		Akce=MOVE_HALA;
		REFRESH();//změna vykreslení kabiny při posunu
	}
}
//---------------------------------------------------------------------------
//zapnutí akce kreslení haly
void __fastcall TForm1::scGPButton_nakreslit_haluClick(TObject *Sender)
{
	log(__func__);//logování
	if(OBJEKT_akt==NULL)//ošetření proti spouštění v náhledu
	{
		scSplitView_OPTIONS->Close();
		if(d.v.HALA.body==NULL||d.v.HALA.body->dalsi==NULL)//pokud existuje hala, jinak nemá smysl
		{
			TIP=ls->Strings[312];//"Klinutím na levé tlačítko myši přidejte bod."
			Akce=DRAW_HALA;
			kurzor(add_o);
			REFRESH();//musí být refresh z důvodu vykreslění mřížky hned po kliku na vytvoření haly
		}
		else if(mrYes==MB(ls->Strings[368],MB_YESNO))//"Dojde k odstranění haly, chcete pokračovat?"
		{
			//smaže starou halu
			d.v.vymaz_body();
			vytvor_statickou_scenu();//aktualizuje BMP statické scény o nový objekt, musí být před REFRESH, není důvod měnit nastavení d.SCENA, pro budoucí refresh
  		scGPButton_nakreslit_haluClick(this);//znovu spuštění metody
		}
	}
}
//---------------------------------------------------------------------------
//pokud existují objekty zapne editaci posledního editovaného, pokud neexistuje poslední editovaný otevře editaci posledního přidaného
void __fastcall TForm1::NahledClick(TObject *Sender)
{
	log(__func__);//logování
//	if(d.v.OBJEKTY->dalsi!=NULL && OBJEKT_akt==NULL)
//	{
//		if(posledni_editovany_objekt!=NULL)pom=posledni_editovany_objekt;
//		else pom=d.v.OBJEKTY->predchozi;
//		otevri_editaci();
//	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPImage_zamek_posunuClick(TObject *Sender)
{
	log(__func__);//logování
	scGPImage_zamek_posunu->ImageIndex=29;//odemčen posun
	scGPImage_zamek_posunu->ClipFrameFillColor=(TColor)RGB(225,225,225);
	Akce=MOVE;
	kurzor(pan);
	TIP=ls->Strings[313];//"Kliknutím a tažením myši posunujete celý objekt."
	nahled_ulozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton_zamek_layoutuClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce!=NIC || Akce_temp==EDITACE_TEXTU)ESC();
  if(Screen->Cursor==standard)kurzor(standard);
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	if(scButton_zamek_layoutu->ImageIndex==68)//odemčeno budu nyní kódem zamykat
	{
		scButton_zamek_layoutu->ImageIndex=67;
		Schema->ImageIndex=79;
		scButton_zamek_layoutu->Hint=ls->Strings[409];//zobrazení textu "Odemknout layout";
		d.v.PP.zamek_layoutu=true;
		//vypnutí knihovny objektů
		nastav_focus();
		mGrid_knihovna->Delete();
		delete mGrid_knihovna;mGrid_knihovna=NULL;
		scSplitView_LEFTTOOLBAR->Opened=false;
		//RzStatusPane5->Visible=false;
		scGPButton_prichytavat->Visible=false;
		TIP="";
		JID=-1;//mazání jidu
		kurzor(standard);
  }
	else//zamčeno budu nyní kódem odemykat
	{
		scButton_zamek_layoutu->ImageIndex=68;
		Schema->ImageIndex=78;
		scButton_zamek_layoutu->Hint=ls->Strings[43];//zobrazení textu "Zamknout layout";
		d.v.PP.zamek_layoutu=false;
		//zapnutí knihovny objektů
		scSplitView_LEFTTOOLBAR->Opened=true;
		//RzStatusPane5->Visible=true;
		scGPButton_prichytavat->Visible=true;
		vytvoreni_tab_knihovna();
	}
	design_statusbar();//změní pozici zarážek ve statusbaru
	DuvodUlozit(true);
	pom=NULL;//nulování, používá se při vykreslování statické scény
	vytvor_statickou_scenu();
	REFRESH();//překreslení pro jistotu, např. focus na hranu objektu
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPTrackBar_intenzitaChange(TObject *Sender)
{
  log(__func__);//logování
  d.pasivni_vektory_intenzita=scGPTrackBar_intenzita->Value;
  REFRESH();
  //ShowMessage(pasivni_vektory_intenzita);
  Form1->writeINI("nastaveni_editace", "intenzita_vektory",d.pasivni_vektory_intenzita);
  DuvodUlozit(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_zobrazit_rotace_jigu_na_otocichClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		if(scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked)rotace_jigu=1;
		else rotace_jigu=0;
		if(Akce==NIC)writeINI("nastaveni_editace","rotace_jigu",rotace_jigu);//ukládat do ini pouze mimo geometrii
    vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_zobrazit_poziceClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		if(scGPCheckBox_zobrazit_pozice->Checked)zobrazit_pozice=1;
		else zobrazit_pozice=0;
		if(Akce==NIC)writeINI("nastaveni_editace","zobrazeni_pozic",zobrazit_pozice);//ukládat do ini pouze mimo geometrii
    vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Memo_testyKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	log(__func__);//logování
	if(Key==116)Invalidate();//při stisku F5 zinvaliduje hlavní form
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox1_popiskyClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		if(scGPCheckBox1_popisky->Checked)zobrazit_popisky=1;
		else zobrazit_popisky=0;
		if(Akce==NIC)writeINI("nastaveni_editace","zobrazit_popisky",zobrazit_popisky);//ukládat do ini pouze mimo geometrii
    vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_rozmisteni_vozikuClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{    //doplnit
		if(scGPCheckBox_rozmisteni_voziku->Checked)zobrazit_rozmisteni_voziku=1;
		else zobrazit_rozmisteni_voziku=0;
		if(Akce==NIC)writeINI("nastaveni_editace","zobrazit_rozmisteni_voziku",zobrazit_rozmisteni_voziku);//ukládat do ini pouze mimo geometrii
    vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_zobrazit_kolejeClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		if(scGPCheckBox_zobrazit_koleje->Checked)zobrazit_koleje=1;
		else zobrazit_koleje=0;
		if(Akce==NIC)writeINI("nastaveni_editace","zobrazit_koleje",zobrazit_koleje);//ukládat do ini pouze mimo geometrii
    vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPSwitch1ChangeState(TObject *Sender)
{
	log(__func__);//logování
	if(scGPSwitch1->State==scswOn) {language=CS;writeINI("Nastaveni_app","jazyk","1");}
	else{language=EN;writeINI("Nastaveni_app","jazyk","0");}
	language=(TForm1::Tlanguage)load_language(language,true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_zobrazit_palceClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		if(scGPCheckBox_zobrazit_palce->Checked)zobrazit_palce=1;
		else zobrazit_palce=0;
		if(Akce==NIC)writeINI("nastaveni_editace","zobrazit_palce",zobrazit_palce);//ukládat do ini pouze mimo geometrii
    vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
	}
}
//---------------------------------------------------------------------------
//přepínání přichytávání
void __fastcall TForm1::scGPButton_prichytavatClick(TObject *Sender)
{
	log(__func__);//logování
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	if(grid)//aby se po zobrazení mřížky zachoval stavající stav,tedy nebude zohledněn další if
	if(++prichytavat_k_mrizce==3)prichytavat_k_mrizce=1;
	if(prichytavat_k_mrizce==1)
	{
		scGPButton_prichytavat->Hint=ls->Strings[411];//"Vypnout přichytávání";
	}
	else
	{
		scGPButton_prichytavat->Hint=ls->Strings[410];//"Zapnout přichytávání";
	}
	akutalizace_stavu_prichytavani_vSB();
	nastav_focus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_errorClick(TObject *Sender)
{
	log(__func__);//logování
	ESC();//ukončení případné akce
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	Form_zpravy->Show();
	if(OBJEKT_akt!=NULL)nastav_focus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_warningClick(TObject *Sender)
{
	log(__func__);//logování
	ESC();//ukončení případné akce
	//pokud je otevřené menu nebo options zavře je
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
	Form_zpravy->Show();
	if(OBJEKT_akt!=NULL)nastav_focus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_smazatClick(TObject *Sender)
{
	log(__func__);//logování
	d.v.PP.raster.filename="";
	d.v.PP.raster.resolution=0;
	d.v.PP.raster.X=0;
	d.v.PP.raster.Y=0;
	d.v.PP.raster.show=false;
	d.v.PP.raster.grayscale=false;
	d.v.PP.raster.dim=0;
	DuvodUlozit(true);
	REFRESH(false);
	scSplitView_OPTIONS->Opened=false;
}
//---------------------------------------------------------------------------
//přepínač geometrického zobrazení, pouze kóty né smartkurzor
void __fastcall TForm1::scGPButton_geometrieClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce!=GEOMETRIE_LIGHT && (d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0))//zapnutí akce geometrie
	{
		Akce=GEOMETRIE_LIGHT;
		editace_geometrie_spustena=false;
		scGPCheckBox1_popisky->Checked=false;//vypnutí zobrazení popisků, v budoucnu rozšířit na uložení předchozího stavu
		scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;
		scGPCheckBox_zobrazit_pozice->Checked=false;
		scGPCheckBox_zobrazit_palce->Checked=false;
		scGPCheckBox_rozmisteni_voziku->Checked=false;
		stisknute_leve_tlacitko_mysi=false;//nutné!!! zustává aktivníc z dblclicku
    typElementu=0;
		REFRESH(false);
		scGPButton_geometrie->ImageIndex=82;
		scGPButton_geometrie->Hint=ls->Strings[444];//vypnout ...
	}
	else if(d.v.ZAKAZKA_akt==NULL || d.v.ZAKAZKA_akt!=NULL && d.v.ZAKAZKA_akt->n==0)
	{
		ukonceni_geometrie();
		Akce=NIC;
		Akce_temp=NIC;
		REFRESH(false);
		scGPButton_geometrie->ImageIndex=83;
		scGPButton_geometrie->Hint=ls->Strings[443];//zapnout ...
	}//vypunutí akce geometrie
	if(OBJEKT_akt!=NULL)nastav_focus();//nutné pro odchytávání kláves
}
//---------------------------------------------------------------------------
//zrušení celé cesty + začátek nové
void __fastcall TForm1::scGPGlyphButton_odstran_cestuClick(TObject *Sender)
{
	log(__func__);//logování
	d.v.inicializace_cesty(Form_definice_zakazek->Z_cesta);//smaže cestu a vytvoří hlavičku nové cesty
	d.v.vloz_cestu_po_hlavni_vetvi(Form_definice_zakazek->Z_cesta,true);//vytvoří první usek cesty
	if(Form_definice_zakazek->Z_cesta->cesta->predchozi->Element==d.v.ELEMENTY->predchozi)scGPButton_ulozit->Enabled=true;
	else scGPButton_ulozit->Enabled=false;
  vytvor_statickou_scenu();
	REFRESH();
}
//---------------------------------------------------------------------------
//smaže jen poslední úsek cesty
void __fastcall TForm1::N21Click(TObject *Sender)
{
	log(__func__);//logování
  //dojde k postupnému odmazávání dokud se nenrazí na začátek mazaného úseku
	Cvektory::TCesta *C=Form_definice_zakazek->Z_cesta->cesta->predchozi,*smaz=NULL;
	while(C!=NULL && C->n>0)
	{
		smaz=C;
		C=C->predchozi;
		C->dalsi=NULL;
		Form_definice_zakazek->Z_cesta->cesta->predchozi=C;
		delete smaz;smaz=NULL;
		//kontrola zda nejsem na konci mazaného úseku, pokud ano přerušit cyklus
		if(C->n==0 || C->Element==d.v.ELEMENTY->predchozi || C->n!=0 && C->Element->eID==300)break;
	}
	C=NULL;delete C;
	//kontrola zda nebyl odmazán první usek cesty, tz. není žádná cesta
	if(Form_definice_zakazek->Z_cesta->cesta->predchozi->n==0)
	{
    //dojde k vytvoření prvního úseku znova
		d.v.inicializace_cesty(Form_definice_zakazek->Z_cesta);
		d.v.vloz_cestu_po_hlavni_vetvi(Form_definice_zakazek->Z_cesta,true);
	}
	//kontrola návaznosti
	if(Form_definice_zakazek->Z_cesta->cesta->predchozi->Element==d.v.ELEMENTY->predchozi)scGPButton_ulozit->Enabled=true;
	else scGPButton_ulozit->Enabled=false;
	//překreslení
  vytvor_statickou_scenu();
	REFRESH();
}
//---------------------------------------------------------------------------
///////otevírání naposledy otevřených projektů
void __fastcall TForm1::N1projekt1Click(TObject *Sender)
{
	log(__func__);//logování
	N1projekt1->Caption=readINI("historie","posledni_soubor_1");//musí být, po kliku dojde k požkození captionu
	Otevri_posledni_ulozeny(N1projekt1->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N2projekt1Click(TObject *Sender)
{
	log(__func__);//logování
	N2projekt1->Caption=readINI("historie","posledni_soubor_2");//musí být, po kliku dojde k požkození captionu
	Otevri_posledni_ulozeny(N2projekt1->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3projekt1Click(TObject *Sender)
{
	log(__func__);//logování
	N3projekt1->Caption=readINI("historie","posledni_soubor_3");//musí být, po kliku dojde k požkození captionu
	Otevri_posledni_ulozeny(N3projekt1->Caption);
}
//---------------------------------------------------------------------------
void TForm1::Otevri_posledni_ulozeny(UnicodeString soubor)
{
	log(__func__);//logování
	if(soubor!=FileName)
	{
  	//zavolá nový soubor/smaže stávajicí
  	Novy_soubor(false);
  	//otevrení souboru
  	Otevrit_soubor(soubor);
  	//zajištění vykreslení načteného souboru
  	REFRESH();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scExPanel1Click(TObject *Sender)
{
  log(__func__);//logování
	scExPanel1->RollUpState=!scExPanel1->RollUpState;
}
//---------------------------------------------------------------------------
//ctlr+z
void __fastcall TForm1::scGPGlyphButton_undoClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce!=NIC || Akce_temp==EDITACE_TEXTU)ESC();
	if(Screen->Cursor==standard)kurzor(standard);
	if(d.v.pozice_data!=1)//pokud nejsem na konci
	{
    //mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
		pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
		//undo
		if(d.v.pozice_data==0)d.v.pozice_data=d.v.DATA->predchozi->predchozi->n;
		else d.v.pozice_data-=1;
		if(d.v.pozice_data>0)//znova kontrola zda nejsem na začátku
		{
			d.v.nacti_z_obrazu_DATA();
			duvod_validovat=2;
			if(OBJEKT_akt==NULL)vytvor_statickou_scenu();
			if(OBJEKT_akt!=NULL)mGrid_on_mGrid();//naplní comba tabulek a zkontroluje překrytí
			if(!scGPGlyphButton_redo->Enabled)scGPGlyphButton_redo->Enabled=true;//pokud bylo provedeno undo a btn na redu neni povolen ... povolit
			if(d.v.pozice_data==1 && scGPGlyphButton_redo->Enabled)scGPGlyphButton_undo->Enabled=false;//jsem na posledním záznamu, nepovolit dále se vracet ... btn undo zakázat
			if(OBJEKT_akt!=NULL)posledni_editovany_element=OBJEKT_akt->element;//musí být pro geometrii, kurzor by zůstal vyset ve vzduchu
			REFRESH();
		}
	}
}
//---------------------------------------------------------------------------
//ctrl+y
void __fastcall TForm1::scGPGlyphButton_redoClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce!=NIC || Akce_temp==EDITACE_TEXTU)ESC();
  if(Screen->Cursor==standard)kurzor(standard);
	if(d.v.pozice_data<d.v.DATA->predchozi->n && d.v.pozice_data!=0)//pokud nejsem na konci
	{
    //mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
		pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
		//redo
		d.v.pozice_data+=1;
		d.v.nacti_z_obrazu_DATA();
		duvod_validovat=2;
		if(OBJEKT_akt==NULL)vytvor_statickou_scenu();
		if(OBJEKT_akt!=NULL)mGrid_on_mGrid();//naplní comba tabulek a zkontroluje překrytí
		if(d.v.pozice_data==d.v.DATA->predchozi->n && scGPGlyphButton_redo->Enabled)scGPGlyphButton_redo->Enabled=false;//dostal jsem se na konec, není kam dál redo použít ... zakázat redo btn
		if(!scGPGlyphButton_redo->Enabled)scGPGlyphButton_undo->Enabled=true;//bylo provedeno redo, pokud není povolený btn na undo ... povolit
		REFRESH();
	}
	else if(scGPGlyphButton_redo->Enabled)scGPGlyphButton_redo->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_bug_reportClick(TObject *Sender)
{
	log(__func__);//logování
	//vytovření backupu pro odeslání
	Timer_backupTimer(Sender);
  if(scSplitView_MENU->Opened) scSplitView_MENU->Opened=false; //pokud je otevřené menu, tak ho zavřu
	//vytvoření printscreeunu
	int W=scSplitView_LEFTTOOLBAR->Width;
	short H=scGPPanel_mainmenu->Height;
	int Gh=0;if(scGPPanel_bottomtoolbar->Visible)Gh=scGPPanel_bottomtoolbar->Height;Gh-=6;//WA, z nějaké důvodu to chce odebrat, aby byla posouváná plocha kompletní
	scGPButton_bug_report->Visible=false;
	Graphics::TBitmap *Screen_bmp=new Graphics::TBitmap();
	Screen_bmp->Width=ClientWidth;Screen_bmp->Height=ClientHeight-H-Gh;//velikost pan plochy
	Screen_bmp->Canvas->CopyRect(Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh));//uloží pan výřez
	if(MOD!=SIMULACE)scGPButton_bug_report->Visible=true;
	TPngImage* PNG = new TPngImage();//kvůli větší kompresi uloženo do PNG (má větší kompresi než JPG)
	PNG->Assign(Screen_bmp);
	delete Screen_bmp;
	PNG->SaveToFile(get_temp_dir() +"TISPL\\" + "tispl_PrtScr"+get_user_name()+"_"+get_computer_name()+".png");
	//zobrazení samotné konzole
	Form_konzole->ShowModal();
}
//---------------------------------------------------------------------------
//timer zavolá setjobid se zpožděním, timer resetován z mousemove
void __fastcall TForm1::Timer_getjobidTimer(TObject *Sender)
{
	log(__func__);
	if(Akce==NIC || Akce==MAGNETICKE_LASO)
	{
		setJobIDOnMouseMove(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
	}
  //kontrola zda je kurzor nad pom_element nebo nad predchozím PM a pokud ano vykreslení spojnice
	if(OBJEKT_akt!=NULL && OBJEKT_akt->zobrazit_mGrid && Akce==NIC)vykresli_spojinici_EmGrid(Canvas,pom_element);
	Timer_getjobid->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPCheckBox_popisek_pohonuClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		if(scGPCheckBox_popisek_pohonu->Checked)zobrazit_popisek_pohonu=1;
		else zobrazit_popisek_pohonu=0;
		if(Akce==NIC)writeINI("nastaveni_editace","zobrazit_popisek_pohonu",zobrazit_popisek_pohonu);//ukládat do ini pouze mimo geometrii
		vytvor_statickou_scenu();//aktualizuje BMP statické scény, došlo ke změně
		REFRESH();
	}
}
//---------------------------------------------------------------------------
//vrátí focus na form1, kdykoli je potřeba vrátit focus, skrze odchytávání kláves
void TForm1::nastav_focus()
{
	log(__func__);
	Edit_proFocus->SetFocus();
}
//---------------------------------------------------------------------------
//slouží k vytvoření obrazu pro storno + undo nebo jen undo
void TForm1::vytvor_obraz(bool stornoUNDO)
{
	log(__func__);
	if(stornoUNDO)vlakno_akce=2;//vytvoření obrazu pro UNDO a storno
	else vlakno_akce=1;//vytvoření obrazu pro UNDO
	vlakno_obraz=new Tvlakno_obraz(true);//spustí vlákno
	vlakno_obraz->FreeOnTerminate=true;//po skončení bude uvolněno
	vlakno_obraz->Start();
	//d.v.vlakno_obraz();
}
//---------------------------------------------------------------------------
//vymaže všechny obrazy v pořadníku
void TForm1::vymaz_seznam_obrazu()
{
	log(__func__);
	vlakno_akce=3;//mazání obrazů
//  vlakno_obraz=new Tvlakno_obraz(true);//spustí vlákno zajišťující stáhnutí mapového podkladu
//	vlakno_obraz->FreeOnTerminate=true;//po skončení bude uvolněno
//	vlakno_obraz->Start();
	d.v.vlakno_obraz();
}
//---------------------------------------------------------------------------
//přepínání stavu přichytávat
void __fastcall TForm1::scLabel_statusbar_0Click(TObject *Sender)
{
	log(__func__);//logování
	scGPButton_prichytavatClick(this);
	//pokud je otevřené menu nebo options zavře je
//	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;
//	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;
//	if(grid)//aby se po zobrazení mřížky zachoval stavající stav,tedy nebude zohledněn další if
//	if(++prichytavat_k_mrizce==3)prichytavat_k_mrizce=1;
//
//	akutalizace_stavu_prichytavani_vSB();
//	REFRESH();
}
//---------------------------------------------------------------------------
//zapnout nebo vypnout panel editace, automaticky podle MODu zobrazí či skryje určité prvky
void TForm1::zapnuti_vypnuti_panelEditace(bool zapnout)
{
	////zapnutí panelu
	if(zapnout)
	{
  	switch(MOD)
		{
      //zapnutí panelu pro editaci
			case EDITACE:
			{
        scGPPanel_bottomtoolbar->FrameColor=clBtnFace;
				scGPPanel_bottomtoolbar->Visible=true;
				scGPImage_mereni_vzdalenost->Visible=true;
				scGPImage_zamek_posunu->Visible=true;
				scGPLabel_prepinacKot->Visible=true;
				scGPComboBox_prepinacKot->Visible=true;
				scGPButton_geometrie->Visible=true;
				scGPButton_viditelnostmGrid->Visible=true;
				scGPButton_viditelnostKoty->Visible=true;
				scButton_zamek->Visible=true;
				scGPButton_posun_dalsich_elementu->Visible=true;
				scGPGlyphButton_odstran_cestu->Visible=false;
			}break;
			//zapnutí panelu pro tvorbu cesty
			case TVORBA_CESTY:
  		{
				scGPPanel_bottomtoolbar->FrameColor=clWhite;
				scGPPanel_bottomtoolbar->Visible=true;
				scGPImage_mereni_vzdalenost->Visible=false;
				scGPImage_zamek_posunu->Visible=false;
				scGPLabel_prepinacKot->Visible=false;
				scGPComboBox_prepinacKot->Visible=false;
				scGPButton_geometrie->Visible=false;
				scGPButton_viditelnostmGrid->Visible=false;
				scGPButton_viditelnostKoty->Visible=false;
				scButton_zamek->Visible=false;
				scGPButton_posun_dalsich_elementu->Visible=false;
				scGPGlyphButton_odstran_cestu->Visible=true;
  		}break;
		}
		//posun tlačítka report
		scGPButton_bug_report->Top-=scGPPanel_bottomtoolbar->Height;
	}

	////vypnutí panelu
	else
	{
		scGPPanel_bottomtoolbar->Visible=false;
		//posun tlačítka report
		scGPButton_bug_report->Top+=scGPPanel_bottomtoolbar->Height;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scExPanel_ostatniClick(TObject *Sender)
{
	log(__func__);//logování
	scExPanel_ostatni->RollUpState=!scExPanel_ostatni->RollUpState;
}
//---------------------------------------------------------------------------
//zajistí rotaci objektu
void TForm1::rotuj_objekt_click(double rotace)
{
  log(__func__);//logování
	if(OBJEKT_akt==NULL && JID!=-6)d.v.rotuj_objekt(F->pom,rotace);
	else
	{
		if(OBJEKT_akt!=NULL)
		{
			OBJEKT_akt->orientace_text=m.Rt90(OBJEKT_akt->orientace_text+rotace);
			nahled_ulozit(true);
		}
		else pom->orientace_text=m.Rt90(pom->orientace_text+rotace);
	}
  vytvor_obraz();
  vytvor_statickou_scenu();//aktualizuje BMP statické scény pro vykreslení aktuálního stavu
	REFRESH();
}
//---------------------------------------------------------------------------
//vytvoří nebo aktualizuje mGrid teplomerů pro OBJEKT_akt
void TForm1::vytvor_aktualizuj_tab_teplomeru()
{
	log(__func__);
  FormX->vstoupeno_elm=false;
	//kontrola zda jsem v editaci a cesta teploměry také existuje
	if(OBJEKT_akt!=NULL && OBJEKT_akt->teplomery!=NULL)
	{
		Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
		//kontrola zda existuje cesta pro aktuální zakázku
		if(T!=NULL)
		{
			//tabulka existuje smaže ji
			if(T->posledni->mGrid!=NULL)
			{
				T->posledni->mGrid->Delete();
				T->posledni->mGrid=NULL;
			}

			////existuje ... aktualizace řádků
			double cas=0,WT=0,delka=0;
			int pocet_voziku=0;
			bool prejezd=true;
			data_teplomeru="";//slouží k uchování dat do buňěk, vyplnění dat až na konci
      radku_teplomeru=1;
			//výpočet času na začátku
			if(T->prvni->sparovany->pohon!=NULL)
			{
				//výpočet délky oblasti
				delka=m.delka(T->prvni->geo.X1,T->prvni->geo.Y1,T->prvni->sparovany->geo.X4,T->prvni->sparovany->geo.Y4);
				delka=m.castPrepony(delka,T->prvni->sparovany->geo.delkaPud,T->prvni->sparovany->geo.HeightDepp);
				if(T->prvni->sparovany==T->posledni->sparovany)delka=m.delka(T->prvni->geo.X1,T->prvni->geo.Y1,T->posledni->geo.X4,T->posledni->geo.Y4);//pokud jsou oba teploměry na stejném úseku, měřit délku mezi jejich souřadnicemi
				//pokud se jedná o stopku počítat s bufferem
				if(T->prvni->sparovany->eID==0)
				{
					//výpočet délky bufferu
					double buf=T->prvni->sparovany->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice;
					//pokud je úsek menší než délka bufferu připočtení wt podle toho na jakém jsem vozíku
					if(delka<=buf)
					{
						prejezd=false;
						pocet_voziku=ceil((buf-delka)/d.v.PP.delka_podvozek);
						if(pocet_voziku<0)pocet_voziku=T->prvni->sparovany->data.pocet_voziku+pocet_voziku;
						cas+=m.V2WT(pocet_voziku,d.v.PP.TT);//připočítání WT na aktuálním vozíku
					}
					//pokud je úsek větší než buffer připočtení WTstop a přejezdu
					else
					{
						delka=delka-buf;
						cas+=delka/T->prvni->sparovany->pohon->aRD;
						//rozpad na přejezd a buffer
						prejezd=true;
						pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;
						prejezd=false;
						cas+=T->prvni->sparovany->data.WTstop;
					}
				}
				else
				{
					cas+=delka/T->prvni->sparovany->pohon->aRD;
					//kontrola zda další není stopka a aktuální element není v bufferu
					if(T->cesta->dalsi!=NULL && T->cesta->dalsi->Element->eID==0 && T->cesta->dalsi->Element->geo.delka<T->cesta->dalsi->Element->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice)
					{
						//element je v bufferu odečíst část bufferu
						delka=T->cesta->dalsi->Element->geo.delka-(T->cesta->dalsi->Element->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice);
						cas+=delka/T->prvni->sparovany->pohon->aRD;
					}
				}
				WT+=T->prvni->sparovany->WT;
			}

			//výpočet času na cestě
			Cvektory::TCesta *CE=T->cesta->dalsi;
			while(CE!=NULL)
			{
				if(CE->Element->pohon!=NULL)
				{
					if(CE->Element->eID==0)
					{
						//výpočet času
						if(!prejezd){pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl před tím buffer, změna, potřebuju zapsat přejezd
						delka=CE->Element->geo.delka-(CE->Element->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice);
						if(delka<0)delka=0;//pokud je délka záporná, znamená to, že v bufferu je PM, nepřidávat žádný čas
						cas+=delka/CE->Element->pohon->aRD;
						prejezd=true;
						pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//zapsání části přejezdu
						prejezd=false;
						cas+=CE->Element->data.WTstop;
					}
					else
					{
						if(!prejezd){pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl před tím buffer, změna, bude následovat přejezd
						prejezd=true;
						cas+=CE->Element->geo.delka/CE->Element->pohon->aRD;
						//kontrola zda další není stopka a aktuální element není v bufferu
						if(CE->dalsi!=NULL && CE->dalsi->Element->eID==0 && CE->dalsi->Element->geo.delka<CE->dalsi->Element->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice)
						{
              //element je v bufferu odečíst část bufferu
							delka=CE->dalsi->Element->geo.delka-(CE->dalsi->Element->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice);
							cas+=delka/CE->Element->pohon->aRD;
						}
						if(CE->dalsi==NULL && T->posledni->sparovany->eID==0 && T->posledni->sparovany->geo.delka<T->posledni->sparovany->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice)
						{
							//element je v bufferu odečíst část bufferu
							delka=T->posledni->sparovany->geo.delka-(T->posledni->sparovany->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice);
							cas+=delka/CE->Element->pohon->aRD;
						}
					}
					WT+=CE->Element->WT;
				}
				CE=CE->dalsi;
			}
			//ukazatelové záležitosti
			CE=NULL;delete CE;

			//výpočet času na konci
			if(T->posledni->sparovany->pohon!=NULL && T->posledni->sparovany!=T->prvni->sparovany)
			{
				//výpočet délky oblasti
				delka=m.delka(T->posledni->sparovany->geo.X1,T->posledni->sparovany->geo.Y1,T->posledni->geo.X4,T->posledni->geo.Y4);
				delka=m.castPrepony(delka,T->posledni->sparovany->geo.delkaPud,T->posledni->sparovany->geo.HeightDepp);
				//pokud se jedná o stopku počítat s bufferem
				if(T->posledni->sparovany->eID==0)
				{
					//výpočet délky bufferu
					double buf=T->posledni->sparovany->data.pocet_voziku*d.v.PP.delka_podvozek-d.v.PP.uchyt_pozice;
					buf=T->posledni->sparovany->geo.delka-buf;
					if(!prejezd){pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}//pokud byl buffer, změna, bude následovat přejezd
					//pokud je úsek menší než délka bufferu připočtení wt podle toho na jakém jsem vozíku
					if(delka>buf)
					{
						if(buf>0)cas+=buf/T->posledni->sparovany->pohon->aRD;
						prejezd=true;
						pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//pokud byl přejezd, změna, bude následovat buffer
						prejezd=false;
						WT+=T->posledni->sparovany->WT;     Sv(delka-buf);
            pocet_voziku=ceil((delka-buf)/d.v.PP.delka_podvozek);
						if(pocet_voziku<0)pocet_voziku=T->prvni->sparovany->data.pocet_voziku+pocet_voziku;
						cas+=m.V2WT(pocet_voziku,d.v.PP.TT);//připočítání WT na aktuálním vozíku
						pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;
					}
					//pokud je úsek větší než buffer připočtení WTstop a přejezdu
					else
					{
						cas+=delka/T->posledni->sparovany->pohon->aRD;
						prejezd=true;
						//rozpad na přejezd a buffer
						pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;//zapsání části přejezdu
					}
				}
				else//není stop
				{
					if(!prejezd){pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}
					prejezd=true;
					cas+=delka/T->posledni->sparovany->pohon->aRD;
					WT+=T->posledni->sparovany->WT;
				}
			}
			//kontrola zda nejsou ještě k vypsání nějaké hodnoty, může nastat, když budou oba teploměry na jedné stoce
			if(cas>0 || WT>0){pridej_radek_tab_teplomeru(T->posledni,cas,WT,prejezd);cas=0;WT=0;}
			//vložení řádku součtu
			pridej_radek_tab_teplomeru(T->posledni,cas,WT,false,true);
		}

		//ukazatelové záležitosti
		T=NULL;delete T;

		//překrytí mGridu
	  mGrid_on_mGrid();
	}
}
//---------------------------------------------------------------------------
//vloží záznam na samostatný řádek v tabulce teploměru, 3 možnosti: přejezd, buffer, celkem
void TForm1::pridej_radek_tab_teplomeru(Cvektory::TElement *E,double cas,double WT,bool prejezd,bool celkem)
{
	log(__func__);//logování
	//načtení výchozích parametrů
	AnsiString text=ls->Strings[486];//přejezd
	if(!prejezd)text="buffer";
	if(celkem)
	{
		text=ls->Strings[487];//celkem
		cas=0;WT=0;
	}

	//přidání řádku
	radku_teplomeru++;
	//vytvoření tabulky, naplnění + design
	if(celkem)
	{
		//naplnění buňek
		E->mGrid=new TmGrid(F);
		E->mGrid->Tag=6;
		E->mGrid->ID=E->n;
		design_element(E,true);
		int oddelovac=0;String hodnota="";
		for(unsigned int i=1;i<E->mGrid->RowCount-1;i++)
		{
			for(unsigned int j=0;j<E->mGrid->ColCount;j++)
			{
      	//extrakce hodnoty z řetezce
				oddelovac=data_teplomeru.Pos(";");
				hodnota=data_teplomeru.SubString(1,oddelovac-1);
				data_teplomeru.Delete(1,oddelovac);//smazání hodnoty z řetězce
				E->mGrid->Cells[j][i].Text=hodnota;
				if(j==0 && hodnota==ls->Strings[487])E->mGrid->Cells[j][i].Hint=487;else E->mGrid->Cells[j][i].Hint=0;
        if(j==1)cas+=ms.MyToDouble(hodnota);if(j==2)WT+=ms.MyToDouble(hodnota);
			}
		}
		E->mGrid->Cells[0][E->mGrid->RowCount-1].Text=text;
		E->mGrid->Cells[1][E->mGrid->RowCount-1].Text=m.round2double(cas+WT,3);//není třeba převádět skrze outPT, sčítájí se hodnoty v tabulkce, které již prošly přes outpt
		E->mGrid->MergeCells(1,E->mGrid->RowCount-1,2,E->mGrid->RowCount-1);
    data_teplomeru="";
    radku_teplomeru=1;
		//deklarace barev
		TColor clBackgroundHidden=(TColor)RGB(240,240,240);
		TColor clFontLeft=(TColor)RGB(128,128,128);
		//design tabulky
		E->mGrid->Cells[0][0].Align=TmGrid::Talign::RIGHT;E->mGrid->Cells[0][0].RightMargin=5;
		E->mGrid->Cells[1][0].Align=TmGrid::Talign::RIGHT;E->mGrid->Cells[1][0].RightMargin=5;
		E->mGrid->Cells[2][0].Align=TmGrid::Talign::RIGHT;E->mGrid->Cells[2][0].RightMargin=5;
		//finalní design tabulky
		for(unsigned int i=1;i<=E->mGrid->RowCount-1;i++)
		{
			//ohraničení
			E->mGrid->Cells[0][i].RightBorder->Width=2;
			if(i==1){E->mGrid->Cells[0][i].TopBorder->Width=2;E->mGrid->Cells[1][i].TopBorder->Width=2;E->mGrid->Cells[2][i].TopBorder->Width=2;}
			if(i==E->mGrid->RowCount-1)E->mGrid->Cells[1][i].RightBorder->Color=clBackgroundHidden;
			//zarovnání v buňkách
			E->mGrid->Cells[0][i].Align=TmGrid::Talign::RIGHT;
			E->mGrid->Cells[1][i].Align=TmGrid::Talign::RIGHT;
			E->mGrid->Cells[2][i].Align=TmGrid::Talign::RIGHT;
			//odsazení textu, normální buňka 5px
			E->mGrid->Cells[0][i].RightMargin=5;
			E->mGrid->Cells[1][i].RightMargin=5;
			E->mGrid->Cells[2][i].RightMargin=5;
			//barva fontu v buňkách
			E->mGrid->Cells[0][i].Font->Color=clFontLeft;
			E->mGrid->Cells[1][i].Font->Color=clFontLeft;
			E->mGrid->Cells[2][i].Font->Color=clFontLeft;
			//barva pozadí needitovatelných buněk
			E->mGrid->Cells[1][i].Background->Color=clBackgroundHidden;
			E->mGrid->Cells[2][i].Background->Color=clBackgroundHidden;
		}
	}
  //uložení dat tabulky
	else
	{
		data_teplomeru+=text+";";
		data_teplomeru+=String(m.round2double(outPT(cas),3))+";";
		data_teplomeru+=String(m.round2double(outPT(WT),3))+";";
	}
}
//---------------------------------------------------------------------------
//zapne stopky
void TForm1::START()
{
	start=Now();
}
//---------------------------------------------------------------------------
//vypne stopky, pokud je parementr metody nastaven na false (což je implicitně), je zajištěn výpis do mema, pokud na true tak do ShowMessage
void TForm1::STOP(bool MB)
{
	MessageBeep(0);
	String R=TimeToStr(Now()-start);
	if(MB)S(R);else Memo(R);
}
//---------------------------------------------------------------------------
void TForm1::GetTime(short int rezim)
{



}
//---------------------------------------------------------------------------
//zborazí upozornění, že došlo ke změně geometrie a resetuje oblasti teplomerů
void TForm1::reset_teplomeru(Cvektory::TObjekt *Objekt)
{
	log(__func__);
	//kontrola zda má oběkt záznam o teploměrech
	if(Objekt!=NULL && Objekt->teplomery!=NULL)
	{
    //kontrola zda má záznam pro konkrétní zakázku
		Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(Objekt,d.v.ZAKAZKA_akt);
		if(T!=NULL)
		{
			//mazání teplomerů a vytvoření default
			d.v.vymaz_seznam_teplomery(Objekt);
			d.v.vytvor_default_c_teplomery(Objekt);
			if(Objekt==OBJEKT_akt)vytvor_aktualizuj_tab_teplomeru();
      //pokud mám zobrazit upozornění, zobrazím
			if(zobrazit_upozorneni_teplomery && Objekt==OBJEKT_akt)MB(-1,-1,ls->Strings[511],"",MB_OK,true,true);//"Byla změněna geometrie linky, oblast teploměrů bude obnovena."
    }
	}
}
//---------------------------------------------------------------------------
//provde zarovnání zarážek a změní velikost výpisů ve statusbaru
void TForm1::design_statusbar()
{
	log(__func__);
	int rozdil=0;
	if(scButton_zamek_layoutu->Visible)rozdil+=scButton_zamek_layoutu->Width;
	if(scGPButton_zmerit_vzdalenost->Visible)rozdil+=scGPButton_zmerit_vzdalenost->Width;
	if(scGPButton_prichytavat->Visible)rozdil+=scGPButton_prichytavat->Width;
	Image_rozdelovac_3->Left=ClientWidth-rozdil-1;
	scLabel_statusbar_2->Width=Image_rozdelovac_3->Left-scLabel_statusbar_2->Left;
}
//---------------------------------------------------------------------------
//vrátí Major verzi z FileVersion
String TForm1::get_major_version(String version)
{
  log(__func__);

	//deklarace
	String major="";
	int pozice=1;

	//extrakce major verze
	while(pozice<=version.Length())
	{
		//pokud není substring číslo, jedná se o oddělovač před minor verzí, přerušít průchod
		if(ms.MyToDouble(version.SubString(pozice,1))==0 && version.SubString(pozice,1)!="0")break;
		//zapsání do major
		major+=version.SubString(pozice,1);
    //posun na další substring
		pozice++;
	}

	//return major verze
	return major;
}
//---------------------------------------------------------------------------
//kopíruje text do Clipboardu
void TForm1::copy_to_clipboard(String text)
{
  log(__func__);
	Edit_proFocus->Text=text;
  Edit_proFocus->SelectAll();
  Edit_proFocus->CopyToClipboard();
}
//---------------------------------------------------------------------------
//přepínání meřidla časy
void __fastcall TForm1::scGPCheckBox_meridlo_casyClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		short pripocitavat_casy_prvniho_el;
		if(scGPCheckBox_meridlo_casy->Checked)pripocitavat_casy_prvniho_el=1;
		else pripocitavat_casy_prvniho_el=0;
		writeINI("nastaveni_editace","pripocitavat_casy_prvniho_el",pripocitavat_casy_prvniho_el);
	}
}
//---------------------------------------------------------------------------
//vytvoří seznam, kam se zapíší pohony, které jsou používány jinde
void TForm1::vytvor_seznam_pouzivanych_pohonu()
{
	log(__func__);//logování
  //kontrola, zda existují pohony
	if(d.v.POHONY!=NULL && d.v.POHONY->dalsi!=NULL)
	{
   	//pokud není seznam prázdný, smazat
   	if(pouzivane_pohony!=NULL)smaz_seznam_pouzivanych_pohonu();
		//vytvoření seznamu
		pouzivane_pohony=new unsigned long[d.v.POHONY->predchozi->n];
		//nulování seznamu + plnění
		Cvektory::TPohon *p=d.v.POHONY->dalsi;
		for(unsigned int i=0;i<d.v.POHONY->predchozi->n;i++)
		{
      //kontrola zda je pohon používán
			if(d.v.pohon_je_pouzivan(p->n,false))pouzivane_pohony[i]=p->n;
			else pouzivane_pohony[i]=0;
			//přesun na další pohon pro příští krok
			p=p->dalsi;
		}
    //ukazatelové záležitosti
    delete p;p=NULL;
	}
}
//---------------------------------------------------------------------------
//provede kontrolu, zda je pohon v seznamu používaných, vrátí výsledek
bool TForm1::je_pohon_pouzivan(unsigned long n)
{
	log(__func__);//logování
  //deklarace
	bool ret=false;
	//kontrola, tda edistuje seznam používanýh pohonů, kontrola správnosti n
	if(pouzivane_pohony!=NULL && 0<n && n<=d.v.POHONY->predchozi->n)
	{
		//průchod přes seznam používaných elementů
		for(unsigned int i=0;i<d.v.POHONY->predchozi->n;i++)
		{
			//kontrola, zda se n nachází v seznamu používaných
			if(pouzivane_pohony[i]==n)
			{
				ret=true;
        break;
      }
    }
	}
	//vracení výsledku
	return ret;
}
//---------------------------------------------------------------------------
//smaže seznam používaných pohonů
void TForm1::smaz_seznam_pouzivanych_pohonu()
{
	log(__func__);//logování
	delete[] pouzivane_pohony;
	pouzivane_pohony=NULL;
}
//---------------------------------------------------------------------------
//provede kontrolu jaké pohony byly editovány
void TForm1::byly_pohony_editovany()
{
	log(__func__);//logování
	if(d.v.POHONY!=NULL && d.v.POHONY->dalsi!=NULL && d.v.DATA!=NULL && d.v.DATA->Pohony!=NULL && d.v.DATA->Pohony->dalsi!=NULL)
	{
		//načtení pohonů
		Cvektory::TPohon *p=d.v.POHONY->dalsi,*pD=d.v.DATA->Pohony->dalsi;
		//vytovoření seznamu editovaných + jeho nulování
		unsigned long *editovane=new unsigned long[d.v.POHONY->predchozi->n];
		for(unsigned int i=0;i<d.v.POHONY->predchozi->n;i++)
		{
			editovane[i]=0;
    }
		//průchod skrze pohony + kontrola rozdílnosti
		bool uloz=false;
		unsigned pozice=0;//pozice pro ukládání do seznamu
		while(p!=NULL && pD!=NULL)
		{
      //kontrola rozdílnosti atributů před vstupem do editace a po editaci
			uloz=false;
			if(p->aRD!=pD->aRD)uloz=true;
			if(p->Rx!=pD->Rx)uloz=true;
			if(p->Rz!=pD->Rz)uloz=true;
			if(p->roztec!=pD->roztec)uloz=true;
      //uložení editovaného pohonu do seznamu
			if(uloz)
			{
				editovane[pozice]=p->n;
				pozice++;
      }
			//posun na další
			p=p->dalsi;
			pD=pD->dalsi;
		}
		//ukazatelové záležitosti
		delete p;p=NULL;
		delete pD;pD=NULL;
    //akualizace parametrů, výpis upozornění pro uživatele
		TIP="";
		for(unsigned int i=0;i<d.v.POHONY->predchozi->n;i++)
		{
      //pokud byl editován používaný pohon, aktualizace jeho dat
			if(editovane[i]!=0 && je_pohon_pouzivan(editovane[i]))
			{
				//aktualizace dat elementů na upraveném pohonu
				d.v.aktualizuj_data_elementum_na_pohonu(editovane[i]);
        //načítání názvu pohonu do výpisu pro uživatele
				p=d.v.vrat_pohon(editovane[i]);
				if(TIP=="")TIP=ls->Strings[494]+" "+p->name;//"Došlo ke změně parametrů prvků na pohonu"
				else TIP+=", "+p->name;
        //ukazatelové záležitosti
				p=NULL;delete p;
				//nastavení pro validaci
				duvod_validovat=2;//pokud doško ke změně na nějakém pohonu, při příštím REFRESHI provést validaci
			}
    }
		//mazání seznamů
		delete[] editovane;editovane=NULL;
	}
}
//---------------------------------------------------------------------------
//rovnoměrně rozmístí mGridny, počátek je left top
void TForm1::rozmisti_mGridy()
{
  log(__func__);
	//nastavení oblastí
	TRect celek;
	celek.left=scSplitView_LEFTTOOLBAR->Width;celek.top=scGPPanel_mainmenu->Height;
	celek.right=ClientWidth;celek.bottom=ClientHeight-scGPPanel_statusbar->Height-scGPPanel_bottomtoolbar->Height;

	//nastavení odsazení
	int oX=25,oY=30;
	int left=celek.left+oX,top=celek.top+oY,top_next=top;

	//průchod skrze elementy
	Cvektory::TElement *E=OBJEKT_akt->element;
	if(predchozi_PM!=NULL)E=predchozi_PM;//začátek průchodu od předchozího PM
	Cvektory::T2Element *VYH=d.v.hlavicka_seznam_VYHYBKY();
	while(E!=NULL)
	{
		//má smysl pouze pro elementy, které mají tabulky
		if(E->eID!=MaxInt && E->eID!=100)
		{
			//hontrola, zda nejsem mimo oblast
			if(left>=celek.right || left+oX+E->mGrid->Width>=celek.right)
			{
				//přesun na další sloupec
				left=celek.left+oX;
				top=top_next+oY;
			}
			//umístění tabulky
			E->Xt=m.P2Lx(left+oX);
			E->Yt=m.P2Ly(top);
			//přičtení tabulky do posledních souřadnic
			left+=oX+E->mGrid->Width;
			if(top+E->mGrid->Height>top_next)top_next=top+E->mGrid->Height;
		}
		//posun na další element
		if(E==predchozi_PM && predchozi_PM!=NULL)E=OBJEKT_akt->element;
		else E=d.v.dalsi_krok(VYH,E,OBJEKT_akt);
	}
	//ukazatelové záležitosti
	d.v.vymaz_seznam_VYHYBKY(VYH);
	E=NULL;delete E;

	//sežazení PmG, pokud existuje
	if(PmG!=NULL)
	{
    //hontrola, zda nejsem mimo oblast
		if(left>=celek.right || left+oX+PmG->Width>=celek.right)
		{
			//přesun na další sloupec
			left=celek.left+oX;
			top=top_next+oY;
		}
		//umístění tabulky
		OBJEKT_akt->Xp=m.P2Lx(left+oX);
		OBJEKT_akt->Yp=m.P2Ly(top);
		//přičtení tabulky do posledních souřadnic
		left+=oX+PmG->Width;
		if(top+PmG->Height>top_next)top_next=top+PmG->Height;
	}

	//seřatení tabulky teploměrů, pokud existuje
	Cvektory::TTeplomery *T=d.v.vrat_teplomery_podle_zakazky(OBJEKT_akt,d.v.ZAKAZKA_akt);
	if(T!=NULL && T->posledni->mGrid!=NULL)
	{
    //hontrola, zda nejsem mimo oblast
		if(left>=celek.right || left+oX+T->posledni->mGrid->Width>=celek.right)
		{
			//přesun na další sloupec
			left=celek.left+oX;
			top=top_next+oY;
		}
		//umístění tabulky
		T->posledni->Xt=m.P2Lx(left+oX);
		T->posledni->Yt=m.P2Ly(top);
		//přičtení tabulky do posledních souřadnic
		left+=oX+T->posledni->mGrid->Width;
		if(top+T->posledni->mGrid->Height>top_next)top_next=top+T->posledni->mGrid->Height;
	}
	T=NULL;delete T;

	//překreslení
	REFRESH();
	nahled_ulozit(true,false);//povolit uložení náhledu bez validace
  mGrid_on_mGrid();//odstraní překrytí, pokud bylo
}
//---------------------------------------------------------------------------
//vypočíta WT pro PM, výhybku a spojku
void TForm1::vypocet_WT(Cvektory::TElement *E)
{
	log(__func__);
	//deklrace
	Cvektory::TElement *e_pom=NULL;
	bool error=false;

	//naplnění poomcného elementu
	if(E!=NULL)
	{
   	switch(E->eID)
   	{
   		case 200:
   		{
				e_pom=E->dalsi;
   			if(e_pom==NULL)e_pom=d.v.ELEMENTY->dalsi;
   			break;
   		}
			case 300:if(E->dalsi2!=E->predchozi2)e_pom=E->dalsi2;break;
			case 301:if(E->dalsi2!=E->predchozi2)e_pom=E->predchozi2;break;
   		default:error=true;break;
		}
	}

	//výpočt WT podle zvoleného pohonu
	if(!error && e_pom!=NULL)
	{           
		switch((int)E->WT_index)
  	{
  		//první nastavování
  		case 0:default:
			{    
        //defaultní hodnota
				E->WT=0;
				if(e_pom->pohon!=NULL)
				{       
					if(e_pom->pohon==E->pohon)
					{
						E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
						E->WT_index=2;//defaultně
					}
  				else
  				{
  					E->WT=m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);
  					E->WT_index=2;
  					if(OBJEKT_akt!=NULL && (E->objekt_n==F->OBJEKT_akt->n || E==predchozi_PM))napln_comba_mGridu(E);
					}
  			}
  			break;
			}
  		//nastaveno na první pohon
			case 1:
			{     
        //defaultní hodnota
				E->WT=0;
				if(E->pohon!=NULL)E->WT=m.cekani_na_palec(0,E->pohon->roztec,E->pohon->aRD,3);
  			break;
  		}
  		//nastaveno na druhý pohon
			case 2:
			{      
        //defaultní hodnota
				E->WT=0;
				if(e_pom->pohon!=NULL)E->WT=m.cekani_na_palec(0,e_pom->pohon->roztec,e_pom->pohon->aRD,3);
  			break;
			}
		}   
	}

	//ukazatelové záležitosti
	e_pom=NULL;delete e_pom;
}
//---------------------------------------------------------------------------
//přepínání antialiasingu
void __fastcall TForm1::scGPCheckBox_antialiasingClick(TObject *Sender)
{
	log(__func__);//logování
	//AA on - off
	antialiasing=scGPCheckBox_antialiasing->Checked;
	//uložení do INI
	AnsiString T=0;if(antialiasing)T="1";
	writeINI("Nastaveni_app","antialiasing",T); //zobrazit rotaci jigu
	//překreslení
  if(d.SCENA!=0)vytvor_statickou_scenu();
  REFRESH();
}
//---------------------------------------------------------------------------
