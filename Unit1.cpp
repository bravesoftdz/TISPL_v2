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
#include "TmGrid.h"
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
#pragma link "rHTMLLabel"
#pragma link "scImageCollection"
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

	//Načtení z INI
	AnsiString T=readINI("nastaveni_nahled", "cas");
	if(T=="")PTunit=SEC;else if(T==1)PTunit=MIN;else PTunit=SEC;
	T=F->readINI("nastaveni_nahled","LO");
	if(T=="")LOunit=M;else if(T==1)LOunit=MM;else LOunit=M;
	T=F->readINI("nastaveni_nahled","Delka_otoce");
	if(T=="")DOtocunit=M;else if(T==1)DOtocunit=MM;else DOtocunit=M;
	T=F->readINI("nastaveni_nahled","koty_delka");//složiteji řešené, z důvodu kót časových a zároveň délkových
//	if(T=="")DKunit=MM;
//	else if(T==1)DKunit=MM;
//	else if(T==2)DKunit=SEKUNDY;
	/*else*/ DKunit=MM;
	//pro pohon
	T=readINI("nastaveni_form_parametry","RDt");//aktuální rychlost
	if(T==0)aRDunit=SEC;else if(T==1) aRDunit=MIN; else aRDunit=SEC;
	T=readINI("nastaveni_nahled","R"); //rozteč
	if(T==0)Runit=M;else if(T==1) Runit=MM; else Runit=M;
	T=readINI("nastaveni_nahled","Rz"); //rozestup
	if(T==0)Rzunit=M;else if(T==1) Rzunit=MM; else Rzunit=M;

  T=readINI("nastaveni_editace","intenzita_vektory"); //intenzita vektoru needitovanych objektu
 	if(T==0)d.pasivni_vektory_intenzita=50;else d.pasivni_vektory_intenzita=F->ms.MyToDouble(T);
  scGPTrackBar_intenzita->Value = d.pasivni_vektory_intenzita;
  T=readINI("nastaveni_editace","rotace_jigu"); //zobrazit rotaci jigu
 	if(T==0)rotace_jigu=0;else rotace_jigu=1;
  T=readINI("nastaveni_editace","zobrazeni_pozic"); //zobrazit pozice
 	if(T==0)zobrazit_pozice=0;else zobrazit_pozice=1;

  if(rotace_jigu==1) scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=true;
  else scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked=false;

  if(zobrazit_pozice==1)scGPCheckBox_zobrazit_pozice->Checked=true;
  else   scGPCheckBox_zobrazit_pozice->Checked=false;


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

	//licence, logování atp.
	Caption="ELTEP - tispl";
	scLabel_titulek->Caption=Caption+" - [Nový.tispl]";
	Application->Title="TISPL";
	LICENCE="TRIAL_GALATEK";
	EDICE=ARCHITECT;//ARCHITECT,CLIENT,VIEWER,DEMO
	edice();//zakázání či povolení grafických uživatelských prvků dle úrovně edice
	n_prihlaseni=0;
	TZF=!DEBUG;//TRIAL_zakazat_funkcionality - nyní nastaveno pro RELEASE
	if(TZF)scGPSwitch_rezim->Enabled=false;
	LogFileStream=new TFileStream("LOGFILE.txt",fmOpenWrite|fmCreate|fmShareDenyNone);
	logovat=true;

  //pomocné objekty pro kopírování paremtrů v PO
	copyObjekt=new Cvektory::TObjekt;
	copyObjektRzRx.x=0;copyObjektRzRx.y=0;

	//načtení řetězů - jsou-li k dispozici
	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	d.v.nacti_CSV_retezy("řetězy.csv");

	//načtení aktuálního Fontu - aFont dále v kódu neměnit!!!
	aFont=new TFont();//aktuální nastavený výchozí font
	aFont->Name="Arial";aFont->Size=12;aFont->Color=clBlack;
	if(FileExists(get_Windows_dir()+"\\Fonts\\Roboto-Condensed.ttf")){aFont->Name="Roboto Cn";aFont->Size=14;}//pokud je k dispozici Roboto Cn, tak ho nastaví

	//nastavení knihovnky
	//DrawGrid_knihovna->Enabled=false;
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
	language=MN;

  //výhybky
	d.v.pocet_vyhybek=0;//nastavení při každém spuštění, do budoucna načítání z binárky nebo 0
	d.v.akt_vetev=true;

  //vytvoření katalogu dopravníků
	d.v.vytvor_KATALOG();

	//vytvoření knihovny objektů
	vytvoreni_tab_knihovna();

	//ostatní
	Image_knihovna_objektu->Height=ClientHeight-34;
	scSplitView_LEFTTOOLBAR->Visible=true;//zapnuti levého toolbaru
	ID_tabulky=0;//uchovává ID tabulky, použiváné při mousemove
	count_memo=0;//jednoduchý counter zobrazovaný v memo3
	TIP="Kliknutím na objekt v knihovně objektu, tažením a následným usazením přidáte objekt.";//nastavení TIPu defautlně na nápovědu pro vložení objektu, později v metode StartUp dojkde k odmazání tipu, jestliže budou xistovat nějaké objekty
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
	Nahled->Options->PressedColor=light_gray;

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

	if(MOD==SCHEMA) //zobrazeni labelu - je hezci, v hlavicce drawgrid knihovny
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
	scGPImage_zamek_posunu->Left=22-4;//okraj komponenty != okraji obrázku
	scGPImage_mereni_vzdalenost->Left=scGPImage_zamek_posunu->Left+scGPImage_zamek_posunu->Width+22-13;
	scGPLabel1->Left=scGPImage_mereni_vzdalenost->Left+scGPImage_mereni_vzdalenost->Width+22-8;
	scGPLabel_prepinacKot->Left=scGPLabel1->Left;//label k přepínači kót
	scGPComboBox_orientace->Left=scGPLabel1->Left+scGPLabel1->Width;
	scGPComboBox_prepinacKot->Left=scGPLabel_prepinacKot->Left+scGPLabel_prepinacKot->Width;//combobox na přepínání mezi kotami čas -- delka
	scGPButton_posun_dalsich_elementu->Left=scGPPanel_bottomtoolbar->Width-scGPButton_posun_dalsich_elementu->Width-25;
	scButton_zamek->Left=scGPButton_posun_dalsich_elementu->Left-scButton_zamek->Width-18;
	scGPButton_viditelnostKoty->Left=scButton_zamek->Left-scGPButton_viditelnostKoty->Width-19;
	scGPButton_viditelnostmGrid->Left=scGPButton_viditelnostKoty->Left-scGPButton_viditelnostmGrid->Width-22;
	//svislé zarovnání prvků
	scGPButton_ulozit->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_ulozit->Height)/2;
	scGPButton_zahodit->Top=scGPButton_ulozit->Top;
	scGPComboBox_orientace->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_orientace->Height)/2;
	scGPComboBox_prepinacKot->Top=scGPComboBox_orientace->Top;//combobox na přepínání mezi kotami čas -- delka
	scGPLabel1->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel1->Height)/2;
	scGPLabel_prepinacKot->Top=scGPLabel1->Top;
	scGPImage_mereni_vzdalenost->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_mereni_vzdalenost->Height)/2;
	scGPImage_zamek_posunu->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_zamek_posunu->Height)/2;
	scGPButton_posun_dalsich_elementu->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_posun_dalsich_elementu->Height)/2;
	scButton_zamek->Top=(scGPPanel_bottomtoolbar->Height-scButton_zamek->Height)/2;
	scGPButton_viditelnostKoty->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostKoty->Height)/2;
	scGPButton_viditelnostmGrid->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostmGrid->Height)/2;
	////design přepínače člověk robot////
	scGPPanel_pomocn_proSwitch->FillColor=scGPLabel_roboti->FillColor;//barva panelu pod přepínačem určuje barvu pozadí přepínače
	scGPPanel_pomocn_proSwitch->FillColor2=scGPLabel_roboti->FillColor2;
	scGPPanel_pomocn_proSwitch->FillColorAlpha=scGPLabel_roboti->FillColorAlpha;
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
        scLabel_architekt->Visible=false;
        scGPSwitch_rezim->Visible=false;
        scLabel_klient->Visible=false;
        scGPButton_mereni_vzdalenost->Enabled=true;
        scLabel1_svetelnost->Enabled=false;
        scGPCheckBox_stupne_sedi->Enabled=false;
        scGPCheckBox_ortogon->Enabled=true;


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
    log(__func__);//logování
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
  log(__func__);//logování
  //DPI handling
	int DPI;
	DPI = get_DPI();
	if (DPI != 96) Form_Z_rozliseni->ShowModal();
	// startUP() - pokud byl zde, dělalo to "chybu v paměti" při spuštění release verze	startUP();//při aktivaci formuláře startující záležitosti, pro zpřehlednění ko
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//volání založení nový soubor z menu = nové nastavení souboru, nastevení aplikace je v konstruktoru
void __fastcall TForm1::NovySouborClick(TObject *Sender)
{
   log(__func__);//logování
	 if(duvod_k_ulozeni) UlozitClick(this);
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
   log(__func__);//logování
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
       d.v.HALA.body=NULL;
			 d.v.hlavicka_OBJEKTY();//založení spojového seznamu pro technologické objekty
			 d.v.hlavicka_POHONY();//založení spojového seznamu pro pohony
			 d.v.hlavicka_ZAKAZKY();//založení spojového seznamu pro zakázky
			 d.v.hlavicka_VOZIKY();// nemusí tu být pokud nebudu ukládat vozíky do filuzaložení spojového seznamu pro vozíky

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
			 d.v.PP.uchyt_pozice=d.v.PP.delka_jig/2.0;//výchozí umístění vozíku je v polovině
			 d.v.PP.typ_linky=0;
			 d.v.PP.raster.filename="";
			 d.v.PP.raster.resolution=m2px;
			 d.v.PP.raster.X=0;
			 d.v.PP.raster.Y=0;
			 d.v.PP.raster.show=false;
       d.v.PP.katalog=1;
       d.v.PP.radius=1;

			 Akce=NIC;Akce_temp=NIC;Screen->Cursor=crDefault;//změní kurzor na default

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
			 Analyza->Down=false;
			 Schema->Down=true;
       SB("Kliknutím na libovolné místo přidáte objekt z knihovny");
			 FileName="Nový.tispl";
			 scLabel_titulek->Caption=Caption+" - ["+FileName+"]";
			 TIP="";
			 Invalidate();//vhodnější invalidate než refresh
	 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
  log(__func__);//logování
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
  log(__func__);//logování
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
  log(__func__);//logování
  //načtení jazykové mutace, nemůže být v konstruktoru, protože ještě neexistují všechny dílčí formuláře = nelze k nim přistoupit
   //load_language(language);   //aktivovani jazyk mutaci
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

  //aktivace tlačítka editace
	if(d.v.OBJEKTY->dalsi!=NULL)Nahled->Enabled=true;
	else Nahled->Enabled=false;
	//slouží po startu programu k načtení parametrů linky, nemůže být voláno v tomto okamžiku v souboru nový, protože by jinak vedlo k pádu aplikace - pořadí vytváření formulářů, není voláno v případě startu aplikace po pádu
	if(volat_parametry_linky)
	{
		Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
		Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
		Form_parametry_linky->ShowModal();//zavolání formáláře pro prvotní vyplnění či potvzení hodnot parametrů linky
	}
	if(d.v.OBJEKTY->dalsi!=NULL)TIP="";//v případě, že jsou vložené nějaké objekty tak dojde k odmazání tipu pro vkládání objektů
	DrawGrid_knihovna->SetFocus();//nutné při spouštění dávat focus na knihovnu, ta přesměrovává všechny události (např. KeyDown) na Form
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
//Zalogování na webu
//automaticky přidá parametry (čas, uživatel, licence)
void TForm1::log2web(UnicodeString Text)
{
//	if(!DEBUG)
//	{
//		//log2webOnlyText(ms.DeleteSpace(LICENCE)+"_"+get_computer_name()+"_"+get_user_name()+"_"+TIME.CurrentDate()+"_"+TIME.CurrentTime()+"|"+Text);
//		try
//		{
//			AnsiString relation_id=GetCurrentProcessId();
//			AnsiString send_log_time= TIME.CurrentDateTime();
//			AnsiString ID ="1";
//			AnsiString strSQL = "INSERT INTO log_table (app_id,app_start,username,send_log_time,command,relation_id,verze) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\",\""+VERZE+"\")";
//
//			FDConnection1->ExecSQL(strSQL);
//		}
//		catch(...){;}//např. není připojení k internetu, tak pouze nezaloguje, dořešit uložení logu do doby získání připojení a volání opětovného odeslání logu
//	}
}
//---------------------------------------------------------------------------
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
	scGPImage_zamek_posunu->Left=22-4;//okraj komponenty != okraji obrázku
	scGPImage_mereni_vzdalenost->Left=scGPImage_zamek_posunu->Left+scGPImage_zamek_posunu->Width+22-13;
	scGPLabel1->Left=scGPImage_mereni_vzdalenost->Left+scGPImage_mereni_vzdalenost->Width+22-8;
	scGPLabel_prepinacKot->Left=scGPLabel1->Left;//label k přepínači kót
	scGPComboBox_orientace->Left=scGPLabel1->Left+scGPLabel1->Width;
	scGPComboBox_prepinacKot->Left=scGPLabel_prepinacKot->Left+scGPLabel_prepinacKot->Width;//combobox na přepínání mezi kotami čas -- delka
	scGPButton_posun_dalsich_elementu->Left=scGPPanel_bottomtoolbar->Width-scGPButton_posun_dalsich_elementu->Width-25;
	scButton_zamek->Left=scGPButton_posun_dalsich_elementu->Left-scButton_zamek->Width-18;
	scGPButton_viditelnostKoty->Left=scButton_zamek->Left-scGPButton_viditelnostKoty->Width-19;
	scGPButton_viditelnostmGrid->Left=scGPButton_viditelnostKoty->Left-scGPButton_viditelnostmGrid->Width-22;
	//svislé zarovnání prvků
	scGPButton_ulozit->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_ulozit->Height)/2;
	scGPButton_zahodit->Top=scGPButton_ulozit->Top;
	scGPComboBox_orientace->Top=(scGPPanel_bottomtoolbar->Height-scGPComboBox_orientace->Height)/2;
	scGPComboBox_prepinacKot->Top=scGPComboBox_orientace->Top;//combobox na přepínání mezi kotami čas -- delka
	scGPLabel1->Top=(scGPPanel_bottomtoolbar->Height-scGPLabel1->Height)/2;
	scGPLabel_prepinacKot->Top=scGPLabel1->Top;
	scGPImage_mereni_vzdalenost->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_mereni_vzdalenost->Height)/2;
	scGPImage_zamek_posunu->Top=(scGPPanel_bottomtoolbar->Height-scGPImage_zamek_posunu->Height)/2;
	scGPButton_posun_dalsich_elementu->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_posun_dalsich_elementu->Height)/2;
	scButton_zamek->Top=(scGPPanel_bottomtoolbar->Height-scButton_zamek->Height)/2;
	scGPButton_viditelnostKoty->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostKoty->Height)/2;
	scGPButton_viditelnostmGrid->Top=(scGPPanel_bottomtoolbar->Height-scGPButton_viditelnostmGrid->Height)/2;
	//horní lišta
	if(MOD==NAHLED)scGPGlyphButton_zpravy_ikona->Left=Nahled->Left-scGPGlyphButton_zpravy_ikona->Width;
	else scGPGlyphButton_zpravy_ikona->Left=Schema->Left-scGPGlyphButton_zpravy_ikona->Width;
	if(/*MOD==REZERVY ||*/ MOD==CASOVAOSA)Invalidate();
	else REFRESH();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::schemaClick(TObject *Sender)
{
	log(__func__);//logování
	if(pom_temp!=NULL)KonecClick(this);
	else
	{
	ESC();//zruší případnou rozdělanou akci
	MOD=SCHEMA;
	if(zobrazit_barvy_casovych_rezerv){zobrazit_barvy_casovych_rezerv=false;}
	Timer_simulace->Enabled=false;
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
	//scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	//scSplitView_LEFTTOOLBAR->Visible=true;
	scListGroupKnihovObjektu->Top=0;
//	scListGroupNastavProjektu->Visible=true;
	//scListGroupKnihovObjektu->Visible=true;
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
  //scLabel1_svetelnost->Top=scGPCheckBox_stupne_sedi->Top +  44;  //pár px navíc kvůli vzdušnosti
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

	//Invalidate();
	}
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
	log(__func__);//logování
	ESC();//zruší případnou rozdělanou akci
	MOD=LAYOUT;
	scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
 //	scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	//scSplitView_LEFTTOOLBAR->Visible=false;
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
	log(__func__);//logování
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
			//scSplitView_LEFTTOOLBAR->Visible=false;
			//scListGroupKnihovObjektu->Visible=false;
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
  log(__func__);//logování
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
	//scSplitView_LEFTTOOLBAR->Visible=false;
	//scListGroupKnihovObjektu->Visible=false;
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
  log(__func__);//logování
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
	  //case 1:RzStatusPane1->Caption=Text;break;  //zrušený statuspane
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
//usnadňuje přístup k ShowMessage - Rosta
void TForm1::Sr(UnicodeString Text,AnsiString umisteni)
{
	S(umisteni+"/Rosta\n"+Text);
}
//---------------------------------------------------------------------------
//vola rychle myMessageBox
int TForm1::MB(long left,long top,UnicodeString text,UnicodeString caption_text,int mbTYPE,bool centrovat_text,bool checkbox_zobrazit,int width,bool default_button_caption)
{
  log(__func__);//logování
	return myMessageBox->Show(left,top,text,caption_text,mbTYPE,centrovat_text,checkbox_zobrazit,width,default_button_caption);
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
		//scGPGlyphButton_zpravy_ikona->Visible=true;//ikona
		scGPGlyphButton_zpravy_ikona->GlyphOptions->NormalColor=color;//barva ikony
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
	////////při změně rozlišení nebo obrazovky dojde k maximalizaci okna programu 	Problém při ruční minimalizaci!
	if(ClientWidth!=Monitor->Width&&FMaximized)
	{
		//maximalizace formuláře jinak to s novým designem nejde
		Form1->Width=Screen->WorkAreaWidth;
		Form1->Height=Screen->WorkAreaHeight;
		FMaximized=false;MaxButtonClick(this);//aby bylo připraveno minimalizační tlačítko
	}

	////////jednoltivé vrstvy
	////vykreslení gridu
	if(grid && Zoom>0.5 && !antialiasing && MOD!=LAYOUT && MOD!=NAHLED &&/*MOD!=REZERVY &&*/ MOD!=CASOVAOSA && MOD!=TECHNOPROCESY)d.vykresli_grid(Canvas,size_grid);//pokud je velké přiblížení tak nevykreslí
	Zoom_predchozi_AA=Zoom;//musí být tu, před mody (mohl by být i před kreslením gridu)
	////rastr
	Graphics::TBitmap *bmp_total=new Graphics::TBitmap;bmp_total->Width=ClientWidth;bmp_total->Height=ClientHeight;
	if(d.v.PP.raster.show)nacti_podklad(bmp_total->Canvas);
	if(grid && Zoom_predchozi_AA>0.5 && (Akce==MOVE_BOD||Akce==DRAW_HALA) && prichytavat_k_mrizce==1)d.vykresli_grid(bmp_total->Canvas,size_grid);//pokud je velké přiblížení tak nevykreslí//vykreslení gridu
	////vektory
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=ClientWidth*3;bmp_in->Height=ClientHeight*3;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu //zkoušel jsem nastavit plochu antialiasingu bez ovládacích prvků LeftToolbar a menu, ale kopírování do jiné BMP to zpomalovalo více neooptimalizovaná oblast pro 3xbmp
	Zoom*=3;//*3 vyplývá z logiky algoritmu antialiasingu
	d.vykresli_halu(bmp_in->Canvas);
	d.vykresli_vektory(bmp_in->Canvas);
	if(Akce==GEOMETRIE)d.smart_kurzor(bmp_in->Canvas,posledni_editovany_element);
	Zoom=Zoom_predchozi_AA;//navrácení zoomu na původní hodnotu
	Cantialising a;
	Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in,true);delete(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	bmp_total->Canvas->Draw(0,0,bmp_out);delete (bmp_out);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
	////mGridy
	d.vykresli_mGridy(bmp_total->Canvas); //přesunuto do vnitř metody: pom_temp->elementy!=NULL kvůli pohonům
	////grafické měřítko
	if(zobrazit_meritko && Akce!=MOVE_HALA)d.meritko(bmp_total->Canvas);
	//finální předání bmp_out do Canvasu
	Canvas->Draw(0,0,bmp_total);
	delete (bmp_total);//velice nutné
	////TIP
	d.vykresli_tip(Canvas);

	////////vykreslování tabulky pro přidávání objektů, temp řešení
	if(mGrid_knihovna!=NULL && pom_temp==NULL)
	{
		mGrid_knihovna->Redraw=true;
		mGrid_knihovna->buffer=true;//změna filozofie zajistí průběžné buffrování při vykreslování jinak mGrid_knihovna->Buffer(false);
		if(mGrid_knihovna->VisibleComponents>-1)mGrid_knihovna->VisibleComponents=true;//stačí volat toto, protože se pomocí Show (resp. Draw-SetCompontens-Set...) cyklem všechny komponenty na základě tohoto zobrazí pokud je nastaveno na -1 tak se při překreslování zohlední individuální nastavení komponent (z tohoto stavu je však pro další použítí třeba vrátit do stavu 0 nebo 1)
		mGrid_knihovna->Show(Image_knihovna_objektu->Canvas);
	}
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
		else d.v.PP.raster.show=false;
	}
}
//---------------------------------------------------------------------------
void TForm1::REFRESH()
{
	log(__func__);//logování
	FormPaint(this);
	if(Label_wip->Visible)Label_wip->Invalidate();//}//pokude je zapntutý antialiasing neproblikne, ale jen se "přeplácne" bitmapou nedojde k probliknutí
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
void TForm1::REFRESH(bool refreshovat_mGridy)
{
	log(AnsiString(__func__)+"pozn. parametr refreshovat_mGridy"); //logování
	refresh_mGrid=refreshovat_mGridy;
  REFRESH();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	log(__func__);//logování
	funkcni_klavesa=0;
	int HG=0; if(scGPGlyphButton_close_grafy->GlyphOptions->Kind==scgpbgkDownArrow)HG=Chart2->Height;//o výšku grafu
	int PXM=50;int D=Form1->m.round(d.v.PP.delka_jig*PXM);int S=Form1->m.round(d.v.PP.sirka_jig*PXM);short Yofset=D;if(S>D)Yofset=S;//pro posun obrazu v technologických procesech
	switch(Key)
	{
		//BACKSPACE
		case 8:
		{
			////Hala
			if(Akce==DRAW_HALA&&d.v.HALA.body->predchozi->n!=0){d.v.smaz_bod(d.v.HALA.body->predchozi);REFRESH();}
			else if(Akce==DRAW_HALA){Akce=NIC;kurzor(standard);}
			////Geometrie
			if(Akce==GEOMETRIE && !editace_textu && posledni_editovany_element!=NULL && posledni_editovany_element->eID==MaxInt)
			{
				if(posledni_editovany_element->n!=1)
				{
					double posunx=posledni_editovany_element->geo.X1-posledni_editovany_element->geo.X4,posuny=posledni_editovany_element->geo.Y1-posledni_editovany_element->geo.Y4;
					Cvektory::TElement *E=posledni_editovany_element->predchozi;
					d.v.smaz_element(posledni_editovany_element);
					posledni_editovany_element=E;
					E=posledni_editovany_element->dalsi;
					while(E!=NULL)
					{
						E->X+=posunx;E->Y+=posuny;//souřadnice elementu
						//geometrie elementu
						E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
						E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
						E=E->dalsi;
					}
					delete E;E=NULL;
				}
				//smazání posledního úseku
				if(posledni_editovany_element->n==1 && posledni_editovany_element->geo.delka!=0)
				{
					posledni_editovany_element->X=posledni_editovany_element->geo.X1;
					posledni_editovany_element->Y=posledni_editovany_element->geo.Y1;
					d.v.vloz_G_element(posledni_editovany_element,0,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,pom_temp->orientace);
				}
				REFRESH(false);
			}
//			if(Akce==GEOMETRIE && posledni_editovany_element!=NULL && posledni_editovany_element->predchozi!=NULL && posledni_editovany_element->predchozi->n>=1/*pom_temp->elementy->predchozi->n>=2*/)//odmazání poslední zarážky při tvorbě geometrie
//			{
//				if(posledni_editovany_element->n!=1){pom_element_temp=posledni_editovany_element->predchozi;if(posledni_editovany_element->name=="EDIT")pom_element_temp->name="EDIT";}else pom_element_temp=NULL;
//				d.v.smaz_element(posledni_editovany_element);
//				posledni_editovany_element=pom_element_temp;
//				pom_element_temp=NULL;
//				if(posledni_editovany_element!=NULL &&  posledni_editovany_element->dalsi!=NULL)
//				{
//					double posunx=posledni_editovany_element->X-posledni_editovany_element->dalsi->geo.X1,posuny=posledni_editovany_element->Y-posledni_editovany_element->dalsi->geo.Y1;
//					Cvektory::TElement *E=posledni_editovany_element->dalsi;
//					while(E!=NULL)
//					{
//						E->X+=posunx;E->Y+=posuny;//souřadnice elementu
//						//geometrie elementu
//						E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
//						E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
//						E=E->dalsi;
//					}
//					delete E;E=NULL;
//				}
//				if(pom->predchozi!=NULL && pom->predchozi->n>=1 && pom_temp->elementy->predchozi->n==1)
//				{
//					posledni_editovany_element=pom->predchozi->elementy->predchozi;pom_element_temp=pom_temp->elementy->predchozi;
//				}
//				REFRESH(false);
//			}
		}break;
		//ENTER
		case 13:
		{
      ////Geometrie
			if(Akce==GEOMETRIE && !editace_textu)ukonceni_geometrie();
			////Hala
			if(editace_textu)Smaz_kurzor();if(Akce==DRAW_HALA&&d.v.HALA.body!=NULL&&d.v.HALA.body->predchozi->n>2){d.v.vloz_bod(d.v.HALA.body->dalsi->X,d.v.HALA.body->dalsi->Y,pom,NULL,ortogonalizace_stav,true);Akce=NIC;kurzor(standard);TIP="";REFRESH();}
			else if(Akce==DRAW_HALA){d.v.vymaz_body();Akce=NIC;kurzor(standard);TIP="";REFRESH();}
		}break;
		//ESC
		case 27:ESC();break;
		//MEZERNÍK
		case 32: if(Akce!=PAN_MOVE){Akce=PAN;kurzor(pan);}break;
		//DELETE
		case 46:
		{
			////Geometrie stejná funkce jako backspace
			if(Akce==GEOMETRIE && !editace_textu && posledni_editovany_element!=NULL && posledni_editovany_element->eID==MaxInt)
			{
				if(posledni_editovany_element->n!=1)
				{
					double posunx=posledni_editovany_element->geo.X1-posledni_editovany_element->geo.X4,posuny=posledni_editovany_element->geo.Y1-posledni_editovany_element->geo.Y4;
					Cvektory::TElement *E=posledni_editovany_element->predchozi;
					d.v.smaz_element(posledni_editovany_element);
					posledni_editovany_element=E;
					E=posledni_editovany_element->dalsi;
					while(E!=NULL)
					{
						E->X+=posunx;E->Y+=posuny;//souřadnice elementu
						//geometrie elementu
						E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
						E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
						E=E->dalsi;
					}
					delete E;E=NULL;
				}
				//smazání posledního úseku
				if(posledni_editovany_element->n==1 && posledni_editovany_element->geo.delka!=0)
				{
					posledni_editovany_element->X=posledni_editovany_element->geo.X1;
					posledni_editovany_element->Y=posledni_editovany_element->geo.Y1;
					d.v.vloz_G_element(posledni_editovany_element,0,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,pom_temp->orientace);
				}
				REFRESH(false);
			}
		}break;
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
		//+
		case 107:ZOOM_IN();break;
		//-
		case 109:ZOOM_OUT();break;
		//F1 - volání nápovědy
		case 112:break;
		//F2
		case 113:Memo3->CopyToClipboard();Memo3->Clear();break;
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
		case 121:Invalidate();break;
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
//odchytávání znaků pro editaci názvů
void __fastcall TForm1::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	log(__func__);//logování
	////////filtr kláves
	AnsiString key=Key;
	if(AnsiString(Key)==ms.get_locale_decimal()&&editovany_text.Length()==0)key="";
	if(!((Key>=L'0')&&(Key<=L'9')||(AnsiString(Key)==ms.get_locale_decimal())))
		key="";
	if(AnsiString(Key)==ms.get_locale_decimal()&&Pos(ms.get_locale_decimal(),editovany_text)>0)
		key="";
	if(Key==L'0'&&Pos(0,editovany_text)>0&&editovany_text.Length()==1)
		key="";
	////////
	//key = pouze čísla, Key = všechny znaky
	if (editace_textu&&index_kurzoru==-6)//editace nadpisu kabiny
	{
		if(Key==8)//pokud je stisknut backspace
			pom_temp->name=pom_temp->name.SubString(1,pom_temp->name.Length()-1);
		else
			pom_temp->name+=Key;
		nahled_ulozit(true);
		REFRESH(false);
	}
	if (editace_textu&&index_kurzoru==-7)//editace short nadpisu kabiny
	{
		if(Key==8)//pokud je stisknut backspace
			pom_temp->short_name=pom_temp->short_name.SubString(1,pom_temp->short_name.Length()-1);
		else if(pom_temp->short_name.Length()!=4)
			pom_temp->short_name+=Key;
		else MessageBeep(0);
		nahled_ulozit(true);
		REFRESH(false);
	}
	if (editace_textu&&(index_kurzoru<=-11||index_kurzoru==-2||index_kurzoru==-5))//editace kót elementů,kót haly nebo objektu, kót kabiny
	{
		if(Key==8)//pokud je stisknut backspace
			editovany_text=editovany_text.SubString(1,editovany_text.Length()-1);
		else
		{
			editovany_text+=key;
			if(key=="")MessageBeep(0);
		}
		if(index_kurzoru!=-2)nahled_ulozit(true);
		REFRESH(false);
	}
	if(editace_textu&&index_kurzoru==1)//editace názvu elementu skrze popisek elementu
	{
		//2 rozdílné přistupy, u robotu a u totočí jiné
		if(pom_element_temp->eID!=0)//roboti+otoče
		{
    	if(Key==8)//pokud je stisknut backspace
				pom_element_temp->name=pom_element_temp->name.SubString(1,pom_element_temp->name.Length()-1);
			else
				pom_element_temp->name+=Key;//nutné Key s velkým K, toto Key neprochází numerickým filtrem
		}
		else//stopka
		{
			if(Key==8)//pokud je stisknut backspace
			{
				if(pom_element_temp->name.Length()>5)//v tomto případě povolit pouze editaci čísla, názvem "Stop " needitovatelný
					pom_element_temp->name=pom_element_temp->name.SubString(1,pom_element_temp->name.Length()-1);
				else MessageBeep(0);
      }
			else
				pom_element_temp->name+=key;//key s malým k, prochází numerickým filtrem, v tomto případě žádoucí
		}
		nahled_ulozit(true);
		//propsání nového názvu do mGridu
		pom_element_temp->mGrid->Cells[0][0].Text="<a>"+pom_element_temp->name+"</a>";
		pom_element_temp->mGrid->MergeCells(0,0,1,0);
		REFRESH(true);
	}
}
//---------------------------------------------------------------------------
//vytvoří edit na místě hlavičky tabulky, slouží ke změně názvu elementu
void TForm1::vytvor_edit()
{
	log(__func__);//logování
	index_kurzoru=JID;
	editace_textu=true;
	pom_element_temp=pom_element;//uloží ukazatel na element, při editaci může uživatel odjet kurzorem z editovaného elementu, proto je nutné si jej uložit
	pom_element_temp->mGrid->Cells[0][0].Type=pom_element_temp->mGrid->EDIT;
	pom_element_temp->mGrid->Cells[0][0].Text=pom_element_temp->name;
	pom_element_temp->mGrid->Cells[0][0].Font->Color=clBlack;
	pom_element_temp->mGrid->MergeCells(0,0,1,0);//nutné provést znova MergeCells skrze správné zobrazení
	pom_element_temp->mGrid->Update();
	TscGPEdit *edit=pom_element_temp->mGrid->getEdit(0,0);//vytvoření ukazatele na edit, z důvodu vícenásobného přistupování
	//nastavení designu editu
	edit->Options->FrameFocusedColor=pom_element->mGrid->Cells[1][0].Background->Color;
	edit->Options->FrameHotColor=pom_element->mGrid->Cells[1][0].Background->Color;
	edit->Options->FrameNormalColor=pom_element->mGrid->Cells[1][0].Background->Color;
	edit->Transparent=true;
	edit->SetFocus();
	edit->SelStart=pom_element->name.Length();//předání focusu a nastavení kurzoru na konec textu
	edit=NULL; delete edit;
}
//---------------------------------------------------------------------------
//smaže edit, který sloužil pro změnu názvu elementu a nový název zapíše do elementu, defaultně provede refresh, pokud není předáno parametrem jinak
void TForm1::smaz_edit(bool refresh)
{
  log(__func__);//logování
	Konec->SetFocus();//před smazáním komponenty je důležité odstranit fosus!
	TscGPEdit *E=pom_element_temp->mGrid->getEdit(0,0);E->Free();E=NULL;delete E;//smazání
	pom_element_temp->mGrid->Cells[0][0].Type=pom_element_temp->mGrid->DRAW;
	pom_element_temp->mGrid->Cells[0][0].Text="<a>"+pom_element_temp->name+"</a>";//vytvoření linku, pro opětovnou možnost editace
	pom_element_temp->mGrid->MergeCells(0,0,1,0);//nutné provést znova Merge
	pom_element_temp=NULL; delete pom_element_temp;
	index_kurzoru=0;
	editace_textu=false;
	if(refresh)REFRESH();
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
  log(__func__);//logování
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
	log(__func__);//logování
	if(pom_temp==NULL && mGrid_knihovna!=NULL && mGrid_knihovna->CheckPTinTable(X,Y))tab_knihovna_click(X,Y);
	if(TIP!="Kliknutím na objekt v knihovně objektu, tažením a následným usazením přidáte objekt.")TIP="";//odstranění zobrazeného tipu
	if(MOD==NAHLED)if(PmG->CheckLink(X,Y)==TPoint(-2,-2))FormX->naplneni_dopRD();//pokud je mod náhled a bylo kliknuto na dopRD vrátí tuto rychlost do tabulky pohonu
	if(MOD==NAHLED&&index_kurzoru==9999||index_kurzoru==100)smaz_edit(false);//smaže edit a neprovede refresh
	if(editace_textu)ESC();
	if(MOD==NAHLED){FormX->odstranit_korelaci();FormX->naplneni_max_voziku(X,Y);}
	if(scSplitView_OPTIONS->Opened || scSplitView_MENU->Opened)//pokud je oteřeno hamburger menu a klikne se do plochy tak se nejdříve zavře
	{    //testování skrývání knihoven
		if(scSplitView_MENU->Opened){DrawGrid_knihovna->Visible=false;DrawGrid_otoce->Visible=false;DrawGrid_ostatni->Visible=false;DrawGrid_geometrie->Visible=false;DrawGrid_poznamky->Visible=false;}
		scSplitView_MENU->Opened=false;
		if(!DrawGrid_knihovna->Visible){DrawGrid_knihovna->Visible=true;DrawGrid_otoce->Visible=true;DrawGrid_ostatni->Visible=true;DrawGrid_geometrie->Visible=true;DrawGrid_poznamky->Visible=true;}
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
						if(JID==0&&pom_komora==NULL&&pom_element!=NULL){Akce=MOVE_ELEMENT;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;mazani=true;pom_element_temp=pom_element;puv_souradnice.x=pom_element->X;puv_souradnice.y=pom_element->Y;}//ELEMENT posun
						if(1000<=JID && JID<2000){Akce=MOVE_TABLE;kurzor(posun_l);minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;pom_element->mGrid->Highlight;refresh_mGrid=false;d.nabuffrovat_mGridy();}//TABULKA posun
						if(100<JID && JID<900){redesign_element();}//nultý sloupec tabulky, libovolný řádek, přepnutí jednotek
						if(JID==-6) {DrawGrid_knihovna->SetFocus();stav_kurzoru=false;editace_textu=true;index_kurzoru=-6;nazev_puvodni=pom_temp->name;TimerKurzor->Enabled=true;}//editace názvu
						if(JID==-7) {Akce=MOVE_TEXT;;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;nahled_ulozit(true);}//posun názvu
						if(JID==-10)zmenJednotekKot();//přepnutí jednotek všech kót
						if(JID<=-11&&JID>=-101&&pom_temp->id!=3){DrawGrid_knihovna->SetFocus();TimerKurzor->Enabled=true;editace_textu=true;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;if(JID!=-101)editovany_text=m.round2double(d.v.vzdalenost_od_predchoziho_elementu(pom_element_temp),2);else editovany_text=m.round2double(vzdalenost_meziLO(pom_element,pom_temp->orientace),2);if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));}//editace kót elementu
						if(JID<=-11&&JID>=-101&&pom_temp->id==3){if(d.v.PtInKota_komory(pom_temp,X,Y)==-1){Akce=ROZMER_KOMORA;pom_komora_temp=pom_komora;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}else {DrawGrid_knihovna->SetFocus();TimerKurzor->Enabled=true;editace_textu=true;stav_kurzoru=false;index_kurzoru=JID;pom_komora_temp=pom_komora;editovany_text=m.round2double(outDK(pom_komora->velikost),2);}}
						if(JID>=11&&JID<=99){Akce=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//změna offsetu kót elementů, nebo změna rozměru jednotlivých kabin
						if(JID>=4&&JID<=10)zmena_jednotek_tab_pohon();//změna jednotek v tabulce pohonů
						if(JID==100)vytvor_edit();//změna názvu elementu
						if(JID==1){DrawGrid_knihovna->SetFocus();stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;nazev_puvodni=pom_element_temp->name;editace_textu=true;TimerKurzor->Enabled=true;}
						if(JID==0&&pom_komora!=NULL&&pom_element==NULL){Akce=MOVE_KOMORA;pom_komora_temp=pom_komora;}//uchopení a přesun komory, sloužící k jejímu odstranění
            //nové JID pro objekt
						if(JID==-2){Akce=MOVE_USECKA;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;nahled_ulozit(true);}//posun úsečky
						if(JID==-3){Akce=MOVE_BOD;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;ortogonalizace_stav=false;nahled_ulozit(true);}//posun jednoho bodu
						if(JID==-4){Akce=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;nahled_ulozit(true);}//změna offsetu kóty
						if(JID==-5){DrawGrid_knihovna->SetFocus();TimerKurzor->Enabled=true;editace_textu=true;stav_kurzoru=false;index_kurzoru=JID;pom_bod_temp=pom_bod;if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(pom_temp->body->predchozi->X,pom_temp->body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));nahled_ulozit(true);}//editace kót kabiny
						if(JID==-9 || JID==4){Akce=MOVE_TABLE;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//posun tabulky pohonu
						if(JID==-201){pom_temp->pohon=element_temp->pohon;if(pom_temp->pohon!=NULL)prirazeni_pohonu_tab_pohon(pom_temp->pohon->n);else prirazeni_pohonu_tab_pohon(0);} //kliknutí na jeden z pohonů na předávacím místě
						if(JID==-202){if(element_temp->dalsi!=NULL){pom_temp->pohon=element_temp->dalsi->pohon;if(pom_temp->pohon!=NULL)prirazeni_pohonu_tab_pohon(pom_temp->pohon->n);else prirazeni_pohonu_tab_pohon(0);}else {pom_vyhybka=pom->dalsi;zmena_editovaneho_objektu();}}
					}
					else
					{
						if(MOD==SCHEMA)//OBJEKT
						{
//							pom=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.V_width*m2px,d.V_width*m2px/*,VyID*/);//hledá v kruhové oblasti
//							if(pom==NULL)//akcelerátor,aby se následně nehledalo znovu, pokud byla nalezena výhybka
//							pom=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width*m2px,d.O_height*m2px);
							if(JID==3&&scButton_zamek_layoutu->ImageIndex==68){Akce=MOVE;kurzor(posun_l);/*posun_objektu=true;*/minule_souradnice_kurzoru=TPoint(X,Y);}
							else if(JID==-1&&Akce==NIC){Akce=PAN;pan_non_locked=true;}//přímo dovolení PAN pokud se neposová objekt = Rosťova prosba
							if(JID==-2){DrawGrid_knihovna->SetFocus();TimerKurzor->Enabled=true;editace_textu=true;stav_kurzoru=false;index_kurzoru=JID;pom_bod_temp=pom_bod;if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(d.v.HALA.body->predchozi->X,d.v.HALA.body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);editovany_text=outDK(ms.MyToDouble(editovany_text));}//převod na mm
							if(JID==0){Akce=MOVE_BOD;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;ortogonalizace_stav=false;}//posun jednoho bodu
							if(JID==1||JID==4){Akce=MOVE_USECKA;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//posun úsečky
							if(JID==2){Akce=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//změna offsetu kót
						}
						else {Akce=PAN;pan_non_locked=true;}//přímo dovolení PAN pokud se neposová objekt   = Rosťova prosba
					}
				}
				if(funkcni_klavesa==1&&JID>0&&JID<=100){d.v.smaz_bod(pom_bod);funkcni_klavesa=0;REFRESH();pom_bod=NULL;}//ctrl+klik na bod -> odstranění bodu
				else if(funkcni_klavesa==1 || Akce==ZOOM_W_MENU)Akce=ZOOM_W;
				//přidávání bodu na úsečku
				if(funkcni_klavesa==3&&JID>100&&d.v.HALA.body->predchozi->n>2)
				{
					if(pom_bod->n!=1)d.v.vloz_bod(m.P2Lx(X),m.P2Ly(Y),pom,pom_bod->predchozi,ortogonalizace_stav);//ukazatel B ukazuje vždy na druhý bod úsečky
					else d.v.vloz_bod(m.P2Lx(X),m.P2Ly(Y),pom,NULL,ortogonalizace_stav);
					REFRESH();
					funkcni_klavesa=0;
        }
				switch(Akce)
				{
					case PAN:
					{
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
//					case MOVE: d.odznac_oznac_objekt(Canvas,pom,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y); break;
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
            //přichytávání k mřížce
						TPointD souradnice;
						//////kostra přichitávání k mřížce
						if(grid)//pokud je zobrazena mřížka
	        	{
	        		switch(prichytavat_k_mrizce)
	        		{
	        			case 0:
	        			{
									if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete přichytit bod k mřížce?","",MB_YESNO,true,true))
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
						//////
						d.v.vloz_bod(souradnice.x,souradnice.y,pom,NULL,ortogonalizace_stav);
						pom_bod=d.v.HALA.body->predchozi;//plnění ukazatele (vložený bod) pro účely vykreslování spojnice v mousemove
						switch(pom_bod->n)
						{
							case 1:
							case 2:TIP="Pro zakreslení haly je nutné zadat minimálně 3 body.";break;
							case 3:TIP="Pro uzavření haly stiskněte ENTER nebo dvakrát klikněte.";break;
						}
						if(pom_bod->n>3)TIP="Pro uzavření haly stiskněte ENTER nebo dvakrát klikněte.";
						if(pom_bod->n>1)REFRESH();//vykreslovat pouze v případě jednoho a více bodů
						zneplatnit_minulesouradnice();//pro vykreslení spojnice (mousemove)
						break;
					}
					case GEOMETRIE:
					{
						if(d.geoTemp.typ!=-1000 && Screen->Cursor==standard)vlozeni_editace_geometrie();//vložení nebo editace geometrie (obsáhlé přesunuto do metody)
						else//spouštění temp akci nad akcemi
						{
							if(JID<=-11&&!editace_textu){DrawGrid_knihovna->SetFocus();TimerKurzor->Enabled=true;editace_textu=true;stav_kurzoru=false;index_kurzoru=JID;pom_element_temp=pom_element;editovany_text=pom_element->geo.delka;if((DKunit==2||DKunit==3)&&pom_element->pohon!=NULL)editovany_text=editovany_text/pom_element->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));}//editace textu
							if(JID>=11 && JID<=99){Akce_temp=OFFSET_KOTY;minule_souradnice_kurzoru=vychozi_souradnice_kurzoru;}//offset kót
						}
						pan_create();//pro případ posunu obrazu bez akce PAN
						REFRESH(false);
					}
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
	if(MOD!=NAHLED)//v náhledu je doubleclick zcela odstaven
	{
		dblClick=true;
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
				pom=d.v.najdi_objekt(m.P2Lx(X),m.P2Ly(Y),d.O_width*m2px,d.O_height*m2px);
				if(Akce==DRAW_HALA)
				{
					d.v.vloz_bod(d.v.HALA.body->dalsi->X,d.v.HALA.body->dalsi->Y,pom,NULL,ortogonalizace_stav,true);
					kurzor(standard);
					TIP="";
		  		REFRESH();
		  	}
			}break;
		}
		Akce=NIC;Akce_temp=NIC;
		if(pom!=NULL && MOD!=NAHLED)
		{
//			NP();//dřívější volání nastavparametry1click
		}
	}
	else if(Akce==GEOMETRIE)ukonceni_geometrie();
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
		}                 //kontrola pro akci VYH!!!!!!
		case ADD:case VYH://přídávání objektu či elementu, posun navigačních linii
		{        //algoritmy v tomto CASE (včetně dílčích algoritmu) by bylo možné sloučit, ale bylo by to dost práce navíc...
			if(MOD!=NAHLED)
			{                           //oštření proti chybnému vykreslení (chybné souřadnice z tmgrid mousemove)
				if(probehl_zoom==false && akt_souradnice_kurzoru_PX.x>168)//ošetření proti nežádoucímu chování po zoomu
				{

					if(d.v.OBJEKTY->predchozi->n>=2)//pokud už existují alespoň dva prvky, jinak nemá smysl
					{
						Cvektory::TObjekt *p=add_objekt_za();//testuje zda se bude přidávat objekt mimo situace poslední-první objekt-prvek
						if(p!=NULL && p!=pom)if(d.lezi_v_pasmu(Canvas,X,Y,p)>0)//byl nalezen meziprostor k přidávání, porovnám tedy jestli se jedná o nový
						{
							d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
							zneplatnit_minulesouradnice();
							pom=p;add_posledni=false;
							if(d.lezi_v_pasmu(Canvas,X,Y,p)==2)d.v.akt_vetev=false;else d.v.akt_vetev=true;//uložení zda jsem v hlavní nebo sekundární
						}
						if(p==NULL && (d.lezi_v_pasmu_poslednim(Canvas,X,Y) || d.v.OBJEKTY->predchozi->n==2))//kurzor se nenachází v prostoru mezi prkvy, je tedy možné přidávat mezi poslední a první prvek, tedy na konec spojového seznamu
						{
							d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
							zneplatnit_minulesouradnice();
							add_posledni=true;pom=NULL;
							d.v.akt_vetev=true;//na konci vždy hlavní větev
						}
					}
					d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
					minule_souradnice_kurzoru=TPoint(X,Y);
					d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
				}
				probehl_zoom=false;

			}
			if(MOD==NAHLED)
			{
				TPoint souradnice=bod_vlozeni_elementu();//"přilepení" kurzoru na pohon
				short rotace_symbolu=0;if(pom_temp->orientace==0 || pom_temp->orientace==180)rotace_symbolu=90;
				if(souradnice.x==-1000)souradnice=akt_souradnice_kurzoru_PX;
				else if(pom_element_temp!=NULL)rotace_symbolu=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),souradnice.x,souradnice.y);//zjistění rotace symbolu
				souradnice=uprav_bod_vlozeni_elementu(souradnice,rotace_symbolu);//uprava souřadnic posun robota, bod vkládání na ramenu
				if(pom->id==3)d.vykresli_ikonu_komory(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"",element_id);
				else d.vykresli_element(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"","",element_id,-1,Rotace_symbolu_minula);
				minule_souradnice_kurzoru=souradnice;//TPoint(X,Y);
				if(pom->id==3)d.vykresli_ikonu_komory(Canvas,X,Y,"",element_id);
				else d.vykresli_element(Canvas,souradnice.x,souradnice.y,"","",element_id,-1,rotace_symbolu);
				Rotace_symbolu_minula=rotace_symbolu;
				pom_element_temp=NULL;//mazání ukazatele, slouží k uchovávání elementu na jehož geometrii budu vkládat element
			}
			break;
		}
		case MOVE://posun objektu
		{
      //zapnutí kurzoru
			if(pom_temp!=NULL&&stisknute_leve_tlacitko_mysi&&Screen->Cursor!=pan_move)kurzor(pan_move);
			//posun
			if(pom_temp!=NULL&&stisknute_leve_tlacitko_mysi)d.v.posun_objekt(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),pom_temp);
			if(pom_temp==NULL)d.v.posun_objekt(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),pom);
			minule_souradnice_kurzoru=TPoint(X,Y);
			REFRESH(false);
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
			else if(JID==-9 || JID==4)//posun tabulky pohonu
			{
				pom_temp->Xp+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
				pom_temp->Yp+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				REFRESH(false);
			}
			minule_souradnice_kurzoru=TPoint(X,Y);
			nahled_ulozit(true);
			break;
		}
		case MOVE_ELEMENT://posun elementu + příprava na kontrolu
		{
			double orientace_ob=pom_temp->orientace;
			bool nemazat=true;
			TPointD tolerance;//slouží pro přesnost detekce element mimo kabinu (0 = detekce elementu až je na hraně, +1 detekce ještě před hranou
			tolerance.x=0;tolerance.y=0;
			//samotný pohyb, který je vázán na pohon
			if (pom_element->orientace==0 || pom_element->orientace==180)
				{nemazat=d.v.posun_element(pom_element,akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),posun_dalsich_elementu,true);tolerance.x=0.15;}
			else
				{nemazat=d.v.posun_element(pom_element,akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),posun_dalsich_elementu,true);tolerance.y=0.15;}
			//aktuální souřadnice se po posunu zapíší do minule_souřadnice, z důvodu okamžité změny známénka posuvu
			minule_souradnice_kurzoru=TPoint(X,Y);
			//if(!nemazat && !posun_dalsich_elementu)Smazat1Click(this);
			if(!nemazat)TIP="Prvek nelze přeunout!";else TIP="";
			REFRESH(false);
			//když nejsou viditelné tabulky elementu, a když se nejedná o element, který nemá tabulku -> nevykresli spojnici mezi elementem a tabulkou
			if(pom_temp->zobrazit_mGrid&&pom_element->eID!=100&&pom_element->eID!=MaxInt)vykresli_spojinici_EmGrid(Canvas,pom_element);
			nahled_ulozit(true);
			break;
		}
		case MOVE_KOMORA://posun komory vedoucí k jejímu odstranění
		{
			d.vykresli_ikonu_komory(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,"",-1);
			minule_souradnice_kurzoru=TPoint(X,Y);
			d.vykresli_ikonu_komory(Canvas,X,Y,"",-1);
			if(pom_temp->orientace==90 && (m.P2Lx(X)<pom_temp->body->dalsi->X || m.P2Lx(X)>pom_temp->body->dalsi->dalsi->X))Smazat1Click(this);
			if(pom_temp->orientace==180 && (m.P2Ly(Y)>pom_temp->body->dalsi->Y || m.P2Ly(Y)<pom_temp->body->dalsi->dalsi->Y))Smazat1Click(this);
			if(pom_temp->orientace==270 && (m.P2Lx(X)>pom_temp->body->dalsi->X || m.P2Lx(X)<pom_temp->body->dalsi->dalsi->X))Smazat1Click(this);
			if(pom_temp->orientace==0 && (m.P2Ly(Y)<pom_temp->body->dalsi->Y || m.P2Ly(Y)>pom_temp->body->dalsi->dalsi->Y))Smazat1Click(this);
			nahled_ulozit(true);
			break;
		}
		case MOVE_BOD://posun bodu pokud je zapnuté přichytávání k mřížce posun bodu po bodech mřížky
		{
			TPointD souradnice;
			if(grid)//pokud je zobrazena mřížka
			{
				int vel_gridu=size_grid;//nesmí se pracovat se size_grid ->ovlivnilo by to vykreslování
				if(pom_temp!=NULL)vel_gridu/=10;//pokud jsem v editaci, grid je jemější
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
				if(pom_temp!=NULL){A=pom_temp->body->predchozi;B=pom_temp->body->dalsi;}//posun úsečky objektu v náhledu
				else if(pom!=NULL){A=pom->body->predchozi;B=pom->body->dalsi;}//posun úsečky objektu ve schématu
				else {A=d.v.HALA.body->predchozi;B=d.v.HALA.body->dalsi;}//posun úsečky haly
			}
			else {A=pom_bod->predchozi;B=pom_bod;}//ostatní úsečky
			if(A->X==B->X)//posun po x
				d.v.posun_hranu(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),0,A,B);
			else if(A->Y==B->Y)//posun po y
				d.v.posun_hranu(0,akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),A,B);
			else//skosená úsečka
			{
				double posunx,posuny,vysledek;
				posunx=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);//posun po x
				posuny=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);//posun po y
				if(m.abs_d(posunx)>m.abs_d(posuny))posuny=0;else posunx=0;//vylepšení "pocitu" z posunu, není nezbytně nutné
				d.v.posun_hranu(posunx,posuny,A,B);
			}
			//u POW při posunu úsečky totožné s hranou poslední kabiny je nutné rozšiřovat zároven poslední komoru!!
			if(pom_temp!=NULL && pom_temp->id==3 && A->n==2)
			{
				if(pom_temp->orientace==0)pom_temp->komora->predchozi->velikost+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				if(pom_temp->orientace==90)pom_temp->komora->predchozi->velikost+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
				if(pom_temp->orientace==180)pom_temp->komora->predchozi->velikost-=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				if(pom_temp->orientace==270)pom_temp->komora->predchozi->velikost-=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
			}
			minule_souradnice_kurzoru=TPoint(X,Y);
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
				if(pom_temp==NULL)d.v.posun_body(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),pom);
				//posun obrysu v editaci
				else d.v.posun_body(akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x),akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y),pom_temp);
				if(TIP!="")TIP="";
				refresh_mGrid=false;
				REFRESH();
				refresh_mGrid=true;
			}
			minule_souradnice_kurzoru=TPoint(X,Y);//slouží i k určení výchozího bodu pro posun
			break;
		}
		case MOVE_TEXT:
		{
			pom_temp->Xt+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
			pom_temp->Yt+=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
			minule_souradnice_kurzoru=TPoint(X,Y);
			REFRESH(false);
			break;
    }
		case ROZMER_KOMORA://změna rozmerů komor pomocí úchopu
		{
			Cvektory::TKomora *ovlivneny=pom_komora_temp->dalsi;
			if((pom_temp->orientace==90||pom_temp->orientace==270)&&ovlivneny->velikost>=0.5&&pom_komora_temp->velikost>=0.5)
			{
				double posun=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
				if(pom_temp->orientace==270)posun*=-1;
				pom_komora_temp->velikost+=posun;
				ovlivneny->velikost-=posun;
			}
			else if(ovlivneny->velikost>=0.5&&pom_komora_temp->velikost>=0.5)
			{
				double posun=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				if(pom_temp->orientace==0)posun*=-1;
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
			break;
    }
		case OFFSET_KOTY:
		{
			if(JID>=11&&JID<=99)//offset kót elementu a komor
			{
				if(pom_temp->orientace==90||pom_temp->orientace==270)pom_temp->koty_elementu_offset.x-=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
				else pom_temp->koty_elementu_offset.x+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
				minule_souradnice_kurzoru=TPoint(X,Y);
				REFRESH(false);
				nahled_ulozit(true);
			}
			else
			{
		  	//definice předchozího bodu
		  	Cvektory::TBod *Bod;
		  	if(pom_bod->n!=1)Bod=pom_bod->predchozi;
		  	else if(pom_temp==NULL)Bod=d.v.HALA.body->predchozi;
		  	else Bod=pom_temp->body->predchozi;
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
//		case VYH://přidávání vyhýbky, ROZPRACOVÁNO!!!!!!
//		{
//			Cvektory::TObjekt *p=NULL;
//			if(probehl_zoom==false)//ošetření proti nežádoucímu chování po zoomu
//			{
//				if(d.v.OBJEKTY->predchozi->n>=2)//pokud už existují alespoň dva prvky, jinak nemá smysl
//				{
//					p=add_objekt_za();//testuje zda se bude přidávat objekt mimo situace poslední-první objekt-prvek
//					if(p!=NULL && p!=pom)//byl nalezen meziprostor k přidávání, porovnám tedy jestli se jedná o nový
//					{
//							pom=p;add_posledni=false;
//							if(d.lezi_v_pasmu(Canvas,X,Y,p)==2)d.v.akt_vetev=false;else d.v.akt_vetev=true;//uložení zda jsem v hlavní nebo sekundární
//					}
//					if(p==NULL && (d.lezi_v_pasmu_poslednim(Canvas,X,Y) || d.v.OBJEKTY->predchozi->n==2))//kurzor se nenachází v prostoru mezi prkvy, je tedy možné přidávat mezi poslední a první prvek, tedy na konec spojového seznamu
//					{
//						add_posledni=true;pom=NULL;
//						d.v.akt_vetev=true;
//					}
//				}  //if(pom!=NULL&&d.lezi_v_pasmu(Canvas,X,Y,pom)==1)Memo("za: "+pom->short_name+"  před: "+pom->dalsi->short_name,true);
//					 //if(pom!=NULL&&d.lezi_v_pasmu(Canvas,X,Y,pom)==2)Memo("za: "+pom->short_name+"  před: "+pom->dalsi2->short_name,true);
//				d.odznac_oznac_vyhybku(Canvas,minule_souradnice_kurzoru.x,minule_souradnice_kurzoru.y,pom);
//				minule_souradnice_kurzoru=TPoint(X,Y);
//				d.odznac_oznac_vyhybku(Canvas,X,Y,pom);
//				p=NULL;delete p;
//			}
//			probehl_zoom=false;
//			break;
//		}
		case MEASURE://liniové měření vzdálenosti,vykreslení provizorní měřící linie
		{
			//zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
			zobraz_tip("Tažením myši z vybraného bodu do cílového bodu zobrazíte vzdálenost mezi těmito body.",Canvas);
				if(stisknute_leve_tlacitko_mysi)
			{
				d.vykresli_meridlo(Canvas,X,Y);
			}
		}break;
		case KALIBRACE:
		{
			//zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
			zobraz_tip("Tažením myši z vybraného bodu na podkladu směřujte do vybraného technolog. objektu, po puštění myši dojde ke kalibraci obrazu.",Canvas);
			if(stisknute_leve_tlacitko_mysi)
			{
				d.vykresli_meridlo(Canvas,X,Y,true);
			}
		}break;
		case ADJUSTACE:
		{
			//zobraz tip musí být zde, jelikož jinak pravé options tento TIP překryje
			zobraz_tip("Tažením myši z výchozího bodu směřujte do cílového bodu podkladu, po puštění myši zadejte skutečnou vzdálenost mezi body v metrech či milimetrech.",Canvas);
			if(stisknute_leve_tlacitko_mysi)
			{
				d.vykresli_meridlo(Canvas,X,Y);
			}
		}break;
		case GEOMETRIE:
		{
			if(Akce_temp==NIC)
			{
		  	//pan_move při stisknutém levém tlačítku
		  	if(stisknute_leve_tlacitko_mysi){pan_map(Canvas,X,Y);kurzor(pan_move);}else if(Screen->Cursor==pan_move)kurzor(standard);
		  	//vykreslení spojnice mezi posledním editovaným elementem a dalším elementem v objektu (pokud existuje)
		  	if(pom->dalsi!=NULL && pom_temp->elementy->predchozi->geo.X4!=pom->dalsi->elementy->dalsi->geo.X1 && pom_temp->elementy->predchozi->geo.X4!=pom->dalsi->elementy->dalsi->geo.X1)
		  	{
		  		//nastavení pera
		  		Canvas->Pen->Color=clBlack;
		  		Canvas->Pen->Width=1;
		  		Canvas->Pen->Style=psDot;//nastevení čarkované čáry
		  		Canvas->Pen->Mode=pmNotXor;
		  		Canvas->Brush->Style=bsClear;
		  		//nastavení souřadnic
		  		double X=0,Y=0;
		  		if(d.geoTemp.typ!=-1000)
		  		{
		  			X=d.geoTemp.X4;
		  			Y=d.geoTemp.Y4;
		  		}
		  		else
		  		{
		  			X=pom_temp->elementy->predchozi->X;
		  			Y=pom_temp->elementy->predchozi->Y;
		  		}
		  		X=m.L2Px(X);Y=m.L2Py(Y);
		  		Canvas->MoveTo(X,Y);
		  		Canvas->LineTo(m.L2Px(pom->dalsi->elementy->dalsi->geo.X1),m.L2Py(pom->dalsi->elementy->dalsi->geo.Y1));
		  	}
		  	pom_element=d.v.najdi_element(pom_temp,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y);
		  	if((pom_element!=NULL && posledni_editovany_element!=NULL && pom_element->n!=posledni_editovany_element->n) || (pom_element!=NULL && posledni_editovany_element==NULL)){posledni_editovany_element=pom_element;editace_geometrie_spustena=true;}
		  	else if(m.PtInCircle(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom_temp->elementy->dalsi->geo.X1,pom_temp->elementy->dalsi->geo.Y1,0.3) && posledni_editovany_element!=NULL){posledni_editovany_element=NULL;editace_geometrie_spustena=true;}
		  	//////setjobid, při geometrii je třeba mít zjišťování jidů kolem kót elementu (editace přímek)
		  	int puvJID=JID;//záloha původního JID
		  	Cvektory::TBod *pom_bod_puv=pom_bod;
		  	//////getjobid
		  	JID=-1;
		  	if(pom_temp->zobrazit_koty)//pouze pokud je náhled povolen a jsou kóty zobrazeny
		  	{
		  		pom_element=NULL;
		  		short PtInKota_elementu=d.v.PtInKota_elementu(pom_temp,X,Y);
		  		//jednotky kóty buď kabiny nebo kót elementů JID=-10
		  		if(PtInKota_elementu==3)JID=-101;//hodnota LO kóty
		  		else if(pom_temp->kabinaKotaX_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || pom_temp->kabinaKotaY_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || PtInKota_elementu==2)JID=-10;
		  		else if(pom_temp->uzamknout_nahled==false)//hledám kóty kabiny
		  		{
		  			short PtInKota_bod=d.v.PtInKota_bod(pom_temp);//metoda vrací zda jsem v oblasti kóty nebo v její hodnotě + ukládá ukazatel na bod do pom_bod
		  			if(PtInKota_bod==0 && pom_bod!=NULL)JID=-4;//oblast kóty - posun kóty
		  			else if(PtInKota_bod==1 && pom_bod!=NULL)JID=-5;//hodnota kóty
		  			else//kóty elementů RET=11-99
		  			{
		  				if(PtInKota_elementu==0 && pom_element!=NULL)JID=10+pom_element->n;//oblast kóty - posun kóty
		  				if(PtInKota_elementu==1 && pom_element!=NULL)JID=(10+pom_element->n)*(-1);//hodnota kóty
		  			}
		  		}
		  	}
		  	//////setjobid
		  	if(puvJID!=JID)//pokud došlo ke změně JID, nebo změně bodu bez změny JID, jinak nemá smysl řešit
		  	{
		  		kurzor(standard);
		  		if(JID<=-11&&!editace_textu){kurzor(edit_text);refresh_mGrid=false;}//kurzor pro editaci textu
		  		if(JID>=11 && JID<=99){if(pom_element->geo.orientace==90||pom_element->geo.orientace==270)kurzor(zmena_d_y);else kurzor(zmena_d_x);refresh_mGrid=false;}//interaktivní kóty elementů
		  	}
		  	//při pan_move nesmí dojít k refreshi, v ostatních případech musí
		  	if(Screen->Cursor!=pan_move)REFRESH();
			}
		}break;
		case NIC://přejíždění po ploše aplikace, bez aktuálně nastavené akce
		{
			if(MOD!=CASOVAOSA)zneplatnit_minulesouradnice();
			if(MOD==NAHLED && pom_temp!=NULL || MOD==SCHEMA)
			{    //testování odstaveno
//					pocitadlo_doby_neaktivity=0; Timer_neaktivity->Interval=20;
//					if(++pocitadlo_zmeny_pozice.x>10 || ++pocitadlo_zmeny_pozice.y>10){pocitadlo_zmeny_pozice.x=0;pocitadlo_zmeny_pozice.y=0;pocitadlo_doby_neaktivity=1;}//naopak akcelerátor, aby se při rychlém pohybu myší zkontrolovala změna
//					Timer_neaktivity->Enabled=true;//volá se zpožděním kvůli optimalizaci setJobIDOnMouseMove(X,Y);
//					//pokud bych odstavil výše uvedený timer takto toto zprovoznit:
					setJobIDOnMouseMove(X,Y);
			}
			//algoritmus na ověřování zda se kurzor nachází na objektem (a může být tedy povoleno v pop-up menu zobrazení volby nastavit parametry) přesunut do metody mousedownclick, zde se to zbytečně volalo při každém posunu myši
			break;
		}
		default: break;
	}
	////akce nad akcemi
	switch(Akce_temp)
	{
		case OFFSET_KOTY:
		{
			if(Screen->Cursor==zmena_d_y)pom_temp->koty_elementu_offset.y-=akt_souradnice_kurzoru.y-m.P2Ly(minule_souradnice_kurzoru.y);
			else pom_temp->koty_elementu_offset.y+=akt_souradnice_kurzoru.x-m.P2Lx(minule_souradnice_kurzoru.x);
			minule_souradnice_kurzoru=TPoint(X,Y);
			REFRESH(false);
			nahled_ulozit(true);
		}break;
		default:break;
  }
    //fix na pridani 1 obj pro demo
//    if(EDICE==DEMO && MOD==SCHEMA)
//    {
//    if(d.v.OBJEKTY->predchozi->n >= 1) DrawGrid_knihovna->Visible=false;
//    else   DrawGrid_knihovna->Visible=true;
//    }
//    if(MOD==NAHLED) DrawGrid_knihovna->Visible=true;
}
//---------------------------------------------------------------------------
//vykreslí spojnici mezi tabulkou a elementem z nejbližšího rohu tabulky
void TForm1::vykresli_spojinici_EmGrid(TCanvas *Canv,Cvektory::TElement *E)
{
	double levyhorni,pravyhorni,levydolni,pravydolni;
	bool vykresleno=false;
	//výpočet vzdáleností od každého rohu tabulky ke středu elementu
	levyhorni=m.delka(m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt),m.L2Py(pom_element->Yt));//ok
	pravyhorni=m.delka(m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt)+pom_element->mGrid->Width,m.L2Py(pom_element->Yt));//ok
	levydolni=m.delka(m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt),m.L2Py(pom_element->Yt)+pom_element->mGrid->Height);//ok
	pravydolni=m.delka(m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt)+pom_element->mGrid->Width,m.L2Py(pom_element->Yt)+pom_element->mGrid->Height);//ok
	//stanovení nejmenší vzdálenosti
	double delka=Min(Min(levyhorni,pravyhorni),Min(levydolni,pravydolni));
	//vykreslí spojinici v nejmenší délce
	if(delka==levyhorni&&!vykresleno){d.linie(Canv,m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt),m.L2Py(pom_element->Yt),2,(TColor)RGB(200,200,200));vykresleno=true;}
	if(delka==pravyhorni&&!vykresleno){d.linie(Canv,m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt)+pom_element->mGrid->Width,m.L2Py(pom_element->Yt),2,(TColor)RGB(200,200,200));vykresleno=true;}
	if(delka==levydolni&&!vykresleno){d.linie(Canv,m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt),m.L2Py(pom_element->Yt)+pom_element->mGrid->Height,2,(TColor)RGB(200,200,200));vykresleno=true;}
	if(delka==pravydolni&&!vykresleno){d.linie(Canv,m.L2Px(pom_element->X),m.L2Py(pom_element->Y),m.L2Px(pom_element->Xt)+pom_element->mGrid->Width,m.L2Py(pom_element->Yt)+pom_element->mGrid->Height,2,(TColor)RGB(200,200,200));vykresleno=true;}
}
//---------------------------------------------------------------------------
//kontrola zde je mGrid mimo obraz, pokud ano vypnutí komponent
void TForm1::mGrid_mimo_obraz(Cvektory::TElement *E)
{
	double presah;
	int pocet_radku;
	design_element(E,false);//uvedení komponent do základního stavu
	//////kontrola, zda jsou řádky pod spodní lištou
	presah=m.L2Py(E->Yt)+E->mGrid->Height-scGPPanel_bottomtoolbar->Top;
	if(presah>0)
	{
		pocet_radku=Ceil(presah/(double)E->mGrid->DefaultRowHeight);
		for (int i=E->mGrid->RowCount-1;i!=0 && i>=E->mGrid->RowCount-pocet_radku; i--)
		{
			if(E->mGrid->Cells[1][i].Type!=E->mGrid->DRAW)E->mGrid->Cells[1][i].Type=E->mGrid->DRAW;
		}
	}
	//////kontrola, zda je sloupec s komponenty za levou knihovnou
	if(m.L2Px(E->Xt)+E->mGrid->Columns[0].Width<scSplitView_LEFTTOOLBAR->Width)
	{
		for(int i=1;i<=E->mGrid->RowCount-1;i++)
		{
			if(E->mGrid->Cells[1][i].Type!=E->mGrid->DRAW)E->mGrid->Cells[1][i].Type=E->mGrid->DRAW;
		}
	}
	//////kontrola horní lišta
	presah=scGPPanel_mainmenu->Height-m.L2Py(E->Yt);
	pocet_radku=Ceil(presah/(double)E->mGrid->DefaultRowHeight);
	if(pocet_radku>1)
	{
		for (int i=1;i<=E->mGrid->RowCount-1 && i<pocet_radku; i++)
		{
			if(E->mGrid->Cells[1][i].Type!=E->mGrid->DRAW)E->mGrid->Cells[1][i].Type=E->mGrid->DRAW;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y)
{
	if(Button==mbLeft)//zohlední jenom stisk levého tlačítka myši
	{
		refresh_mGrid=true;//globální navracení stavu
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
				else {if(element_id>=0)add_element(X,Y);else add_komoru();/*d.vykresli_element(Canvas,X,Y,"","",element_id,-1,Rotace_symbolu_minula);-již není třeba, někde se refreshuje*/}//přídání elementu v modu NAHLED
				zneplatnit_minulesouradnice();
				kurzor(standard);
				break;
			}
			case VYH:/*Akce=ADD;*/add_objekt(X,Y);zneplatnit_minulesouradnice();break;//přidání objekt
			case MOVE:/*move_objekt(X,Y);*/Akce=NIC;kurzor(standard);if(pom_temp!=NULL)scGPImage_zamek_posunu->ClipFrameFillColor=clWhite;break;//posun objektu
			case MOVE_TABLE:
			{
				if(pom_element!=NULL)
				{
					mGrid_mimo_obraz(pom_element);//kontrola zda je tabulka stále v obraze
					pom_element->mGrid->Update();//pouze WA, aby se před zobrazením komponenty zobrazily na správné pozici a nedošlo k probliku
					JID=-1;setJobIDOnMouseMove(X,Y);kurzor(posun_l);//kvůli rychlé aktualizaci po přesunu včetně Highlightu
				}
				Akce=NIC;kurzor(standard);//REFRESH();znovu zakomentován, protože je volán v setJobIDOnMouseMove
				break;//posun tabulky elementu  - REFRESH() byl 10.5.19 odkomentován, nevím proč byl zakomentovaný (asi z důvodu špatné domněnky, že se i refreshuje průběžně při přesouvání,což je sice pravda, ale není to dostatečné), zposobilo nepřekreslení spojnice mezi tabulkou a elementem po uvolnění myši
			}
			case MOVE_ELEMENT:
			{
				bool chybne=prekryti_LO(pom_element);//kontrola zda se element nepřekrývá lak. oknem s jiným elementem
				FormX->odstranit_korelaci();//přidáno z důvodu odmazávání korelace při posuvu elementu
				TIP="";
				Akce=NIC;kurzor(standard);
				if(chybne && mrYes!=MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Přesunem dojde k překrytí lakovacích oken, chcete element přesunout?","",MB_YESNO))
				{
					pom_element_temp->X=puv_souradnice.x;
					pom_element_temp->Y=puv_souradnice.y;
        }
				pom_element_temp=NULL; delete pom_element_temp;
				break;//posun elementu
			}
			case MOVE_KOMORA:
			{
				d.v.presun_komoru(pom_temp,pom_komora_temp,d.v.najdi_komoru(pom_temp));
				pom_komora_temp=NULL;delete pom_komora_temp;
				Akce=NIC;
				refresh_mGrid=false;
				REFRESH();
				refresh_mGrid=true;
				break;
			}
			case MOVE_TEXT:
			case MOVE_BOD:
			case MOVE_USECKA:Akce=NIC;kurzor(standard);break;
			case MOVE_HALA:Akce=NIC;kurzor(standard);REFRESH();break;//refresh z důvodu znovu zapnutí měřítka a gridu
			case ROZMER_KOMORA:Akce=NIC;break;
			case OFFSET_KOTY:Akce=NIC;break;
			case MEASURE:
			{
				double delka=m.delka(m.P2Lx(vychozi_souradnice_kurzoru.X),m.P2Ly(vychozi_souradnice_kurzoru.Y),m.P2Lx(X),m.P2Ly(Y));
				if(pom_temp!=NULL&&pom_temp->pohon!=NULL)MB("Vzdálenost: "+AnsiString(m.round2double((delka*1000.0),3))+" [mm]; "+"Čas: "+AnsiString(m.round2double((delka/pom_temp->pohon->aRD),3))+" [s]",MB_OK,true,366,true,true,true);
				else MB(AnsiString(m.round2double((delka*1000.0),3))+" [mm]");
				Akce=NIC;kurzor(standard);
				zobraz_tip("");//nahrazuje zároveň Refresh
        scGPImage_mereni_vzdalenost->ClipFrameFillColor=clWhite;
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
				}
				else
				{
					d.v.PP.raster.show=false;
					d.v.PP.raster.filename="";
				}
				Akce=NIC;kurzor(standard);
				zobraz_tip("");//nahrazuje zároveň Refresh
				break;
			}
			case GEOMETRIE:if(Akce_temp==NIC)pan_move_map();break;
			default: break;
		}

		switch(Akce_temp)
		{
			case OFFSET_KOTY:Akce_temp=NIC;break;
			default:break;
    }
	}
	stisknute_leve_tlacitko_mysi=false;

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
//JID=-7;//posunovací kříž nadpisu						Původní: zkratka objektu
//JID=-8;//prázdné														Původní: vodorovná kóta kabiny
//JID=-9;uchop tabulky pohonu									Původní: svislá kóta kabiny
//JID=-10;//jednotky kóty
//JID=-11 až -100;//hodnota kóty konkrétně a n elementu (10+pom_element->n)*(-1);hodnota kóty konkrétně a n komory (10+pom_komora->n)*(-1)
//JID=-101;//hodnota LO kóty elementu
//JID=-201;//pohon1 na předávacím místě
//JID=-202;//pohon2 na předávacím místě
//JID=-1 žádná
//JID= 0 - 3 rezervováno pro element (komoru), 0 - element mimo název (komora, kurzor nad oblastí komory), 1 - citelná oblast název elementu, 2,3 - rezerva, bližší identifikace elementu probíhá pomocí pom_element
//JID= 4;//citelná oblast celé tabulky pohonu
//JID= 5 - 10 první až poslední řádek tabulky pohonů, hlavička nevyužito z důvodu obsazení COMBOBOXem, 5 - aRD, 6 - rozteč, 7 - nevyužito, 8 - rozestup, 9 - mezera jig, 10 - mezera jig
//JID= 11 - 99 - oblast kóty (elementů nebo komor - dle použitého objektu (v POW komory jsou vnímány jako elementy)), 10+pom_element->n nebo komora->n - oblast kóty/posun kóty a n elementu
//JID= 100-999 rezervováno pro odkazy v tabulce, kde 100 znamená nultý řádek libovolného sloupce (ideální pro sloučenou hlavičku), u 100 se více neže o odkaz jedná o interaktivní text, který je určen k editaci formou změny na EDIT, ale definovaný odkazem pomocí tagu <a>název</a>, to zajistí naplnění citelné oblasti
//JID= 1000-1999 rezervováno pro řádky nultého slopce tabulky (v místě, kde není odkaz), kde 1000 znamená nultý řádek tabulky nehledě na sloupec (ideální pro sloučenou hlavičku), kde není odkaz
//JID= 2000-rezervováno pro 2 či další sloupce resp. řádky, 2000 - nevyužito,2001 - první řádek (po nultém)
void TForm1::getJobID(int X, int Y)
{
	log(__func__);//logování
	JID=-1;//výchozí stav, nic nenalezeno
	if(MOD==NAHLED)
	{
  	//nejdříve se zkouší hledat souřadnice myši v TABULCE POHONů
  	if(PmG!=NULL && pom_temp->uzamknout_nahled==false && pom_temp->zobrazit_mGrid)
		{
			pom_element=NULL;
			pom_komora=NULL;
  		int IdxRow=PmG->GetIdxRow(X,Y);
			//if(IdxRow==0)JID=4;//hlavička NEVYUŽITO, je tam COMBO, zachováno jako rezerva
			if(IdxRow>0)//nějaký z řádků mimo nultého tj. hlavičky, nelze použít else, protože IdxRow -1 bude také možný výsledek
  		{
				int IdxCol=PmG->GetIdxColumn(X,Y);
  			if(IdxCol==0)//řádky v prvním sloupeci
  			{
  				if(PmG->CheckLink(X,Y,IdxCol,IdxRow))JID=4+IdxRow;//na daném řádku a daných myších souřadnicích se nachází odkaz
					//else JID=XX+IdxRow;//řádek v nultém sloupci mimo odkaz - NEVYUŽITO
  			}
				//else JID=XX+IdxRow;//řádky v dalších sloupcích - NEVYUŽITO
			}
			//pokud nic nenalezeno snažím se najít PmG i nad komponenty v tabulce
			if(JID==-1 && ((m.L2Px(pom_temp->Xp)<X+m.L2Px(pom_temp->Xp) && X+m.L2Px(pom_temp->Xp)<=m.L2Px(pom_temp->Xp)+PmG->Width && ID_tabulky==9999)||(m.L2Px(pom_temp->Xp)<X && X<=m.L2Px(pom_temp->Xp)+PmG->Width)) && ((m.L2Py(pom_temp->Yp)<Y+m.L2Py(pom_temp->Yp) && Y+m.L2Py(pom_temp->Yp)<m.L2Py(pom_temp->Yp)+PmG->Height && ID_tabulky==9999) || (m.L2Py(pom_temp->Yp)<Y && Y<m.L2Py(pom_temp->Yp)+PmG->Height)))JID=4;
			//citelná oblast kříže pro posun
			if(m.L2Px(F->pom_temp->Xp)-20<X && X<m.L2Px(F->pom_temp->Xp) && m.L2Py(F->pom_temp->Yp)+10>Y && Y>m.L2Py(F->pom_temp->Yp)-25)JID=-9;
		}
		if(JID==-1)//pokud nebyla tabulka pohonu nalezena zkouší hledat další aktivní prvky náhledu
		{
  		//dále TABULKY ELEMENTŮ
			pom_element=F->d.v.najdi_tabulku(pom_temp,m.P2Lx(X),m.P2Ly(Y));
  		if(pom_element!=NULL && pom_temp->zobrazit_mGrid)//možné měnit rozmístění a rozměry a tabulka nalezena, tzn. klik či přejetí myší přes tabulku
  		{
  			int IdxRow=pom_element->mGrid->GetIdxRow(X,Y);
  			if(IdxRow==0)
  			{                                 //kvůli sloučeným buňkám
  				if(pom_element->mGrid->CheckLink(X,Y,0,0) || pom_element->mGrid->CheckLink(X,Y,1,0))JID=100+0;//text názvu (pseudointeraktivní text - formou editu a definovaný odkazem <a>název</a>) v hlavičce
  				else if(pom_temp->uzamknout_nahled==false)JID=1000+0;//hlavička - posouvat tabulku je možné pouze při odemčeném náhledu
  			}
  			if(IdxRow>0)//nějaký z řádků mimo nultého tj. hlavičky, nelze použít else, protože IdxRow -1 bude také možný výsledek
  			{
  				int IdxCol=pom_element->mGrid->GetIdxColumn(X,Y);
  				if(IdxCol==0)//řádky v prvním sloupeci
  				{
  					if(pom_element->mGrid->CheckLink(X,Y,IdxCol,IdxRow))JID=100+IdxRow;//na daném řádku a daných myších souřadnicích se nachází odkaz
  					else if(pom_temp->uzamknout_nahled==false)JID=1000+IdxRow;//řádky bez odkazu možné posouvat tabulku je možné pouze při odemčeném náhledu
  				}
  				else if(pom_temp->uzamknout_nahled==false)JID=2000+IdxRow;//řádky v druhém a dalších sloupcích
				}
  		}
  		else//tabulka nenalezena, takže zkouší najít ELEMENT
			{
				pom_element=NULL;
				if(pom_temp->uzamknout_nahled==false)pom_element=F->d.v.najdi_element(pom_temp,m.P2Lx(X),m.P2Ly(Y));//pouze pokud je možné měnit rozmístění a rozměry,nutné jako samostatná podmínka
				if(pom_element!=NULL)//element nalezen, tzn. klik či přejetí myší přes elemement nikoliv tabulku
  			{
  				if(pom_element->citelna_oblast.rect3.PtInRect(TPoint(X,Y)))JID=1;//byl nalezen název elementu
  				else JID=0; //byl nálezen element nikoliv jeho název, určeno k smazání či posunu elementu
  			}
  			else //ani element nenalezen, hledá tedy interaktivní text, obrys a kóty atp.
				{
					pom_bod=d.v.najdi_bod(pom_temp);//pokouším se najít bod v obrysu kabiny
					if(pom_bod!=NULL)JID=-3;//bod nalezen
					else //bod nenalezen, pokouším se najít hranu kabiny
					{
						pom_bod=d.v.najdi_usecku(pom_temp);
						if(pom_bod!=NULL)JID=-2;//hrana nalezena
						else
				  	{ //testování zda se nejedná o NÁZEV či ZKRATKA objektu, ZATÍM NEREFLEKTUJE ORIENTACI NÁHLEDU
							d.nastavit_text_popisu_objektu_v_nahledu(Canvas,1);AnsiString Tn=F->pom_temp->name.UpperCase();short Wn=Canvas->TextWidth(Tn);//název objektu - nastavení
							if(najdi_nazev_obj(X,Y,pom_temp))JID=-6;//název objektu
							if(najdi_nazev_obj(X,Y,pom_temp,1))JID=-7;
							if(JID==-1)//hledání předávacího místa, pohon 1 nebo pohon 2
							{
								short pohon=najdi_popisky_PM(X,Y,pom_temp);
								if(pohon>0)JID=-200-pohon;
              }
							if(JID==-1)//nejedná tj. testují se KÓTY
							{
								if(pom_temp->zobrazit_koty)//pouze pokud je náhled povolen a jsou kóty zobrazeny
								{
									short PtInKota_elementu=d.v.PtInKota_elementu(pom_temp,X,Y);
									//jednotky kóty buď kabiny nebo kót elementů JID=-10
									if(PtInKota_elementu==3)JID=-101;//hodnota LO kóty
									else if(pom_temp->kabinaKotaX_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || pom_temp->kabinaKotaY_oblastHodnotaAJednotky.rect2.PtInRect(TPoint(X,Y)) || PtInKota_elementu==2)JID=-10;
									else if(pom_temp->uzamknout_nahled==false)//hledám kóty kabiny
									{
										short PtInKota_bod=d.v.PtInKota_bod(pom_temp);//metoda vrací zda jsem v oblasti kóty nebo v její hodnotě + ukládá ukazatel na bod do pom_bod
										if(PtInKota_bod==0 && pom_bod!=NULL)JID=-4;//oblast kóty - posun kóty
										else if(PtInKota_bod==1 && pom_bod!=NULL)JID=-5;//hodnota kóty
										else//kóty elementů RET=11-99
										{
											if(PtInKota_elementu==0 && pom_element!=NULL)JID=10+pom_element->n;//oblast kóty - posun kóty
											if(PtInKota_elementu==1 && pom_element!=NULL)JID=(10+pom_element->n)*(-1);//hodnota kóty
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if(JID==-1&&pom_temp->uzamknout_nahled==false&&pom_temp->id==3)
  	{
  		pom_komora=d.v.najdi_komoru(pom_temp);
  		if(pom_komora!=NULL)//komora nalezena
  		{
  			JID=0;//uložení komory do JID
  			//detekce hrany komory
				switch((int)pom_temp->orientace)
				{
					case 0:if(Y<=m.L2Py(pom_temp->body->dalsi->Y+d.v.vrat_velikosti_komor(pom_komora))+9&&Y>=m.L2Py(pom_temp->body->dalsi->Y+d.v.vrat_velikosti_komor(pom_komora))-3)JID=(10+pom_komora->n)*(-1);break;
					case 90:if(X<=m.L2Px(pom_temp->body->dalsi->X+d.v.vrat_velikosti_komor(pom_komora))+9&&X>=m.L2Px(pom_temp->body->dalsi->X+d.v.vrat_velikosti_komor(pom_komora))-3)JID=(10+pom_komora->n)*(-1);break;
					case 180:if(Y>=m.L2Py(pom_temp->body->dalsi->Y-d.v.vrat_velikosti_komor(pom_komora))-9&&Y<=m.L2Py(pom_temp->body->dalsi->Y-d.v.vrat_velikosti_komor(pom_komora))+3)JID=(10+pom_komora->n)*(-1);break;
					case 270:if(X>=m.L2Px(pom_temp->body->dalsi->X-d.v.vrat_velikosti_komor(pom_komora))-9&&X<=m.L2Px(pom_temp->body->dalsi->X-d.v.vrat_velikosti_komor(pom_komora))+3)JID=(10+pom_komora->n)*(-1);break;
				}
			}
  		else if(pom_temp->zobrazit_koty)//prohledávání kót
			{
				short PtInKota_komory=d.v.PtInKota_komory(pom_temp,X,Y);
				if(PtInKota_komory==0 && pom_komora!=NULL)JID=10+pom_komora->n;//oblast kóty - posun kóty
				if(PtInKota_komory==1 && pom_komora!=NULL)JID=(10+pom_komora->n)*(-1);//hodnota kóty
			}
		}
	}
	else//pro schéma, zjišťování jidů pro body a úsečky
	{
    /////////////JID udává pouze akci, není třeba aby se k němu přičítalo i číslo bodu, bod je držen jako ukazatel pom_bod/////////////
		//-2; hodnota kóty bodu (přímka [A,B] uložena v bodě B)
    //-1=NIC!!!!!!!!!!!!!!!
		//0; bod haly nebo objektu
		//1; úsečka haly nebo objektu
		//2; oblast kóty bodu (přímka [A,B] uložena v bodě B)
		//3; oblas objektu
		//4; hrana objektu
		if(d.v.OBJEKTY->dalsi!=NULL&&Akce==NIC)
		{
			pom=NULL;pom_bod=NULL;
			pom=d.v.PtInObjekt();
			if(pom!=NULL)//byl nalezen objekt
			{
				pom_bod=d.v.najdi_usecku(pom,2);
		  	if(pom_bod!=NULL)JID=4;//nalezena hrana objektu
				else JID=3;//nalezen pouze objekt
			}
		}
		if(JID==-1&&(d.v.HALA.body!=NULL||pom!=NULL&&pom->body!=NULL)&&Akce==NIC)//má smysl pouze pokd existuje hala nebo objekt
		{
			pom_bod=d.v.najdi_bod(pom);//pokouším se najít bod
			if(pom_bod!=NULL)JID=0;//bod nalezen
			else//bod nenalezen, pokusí se najít úsečku
	  	{
				pom_bod=d.v.najdi_usecku(pom);//druhý parametr udává přesnost s jakou hledám, popřípadě velikost oblasti kolem úsečky
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
	//pouze na test zatížení Memo3->Visible=true;Memo3->Lines->Add(s_mazat++);
}
//---------------------------------------------------------------------------
//dle místa kurzoru a vrácené JID (job id) nastaví úlohu
void TForm1::setJobIDOnMouseMove(int X, int Y)
{
	log(__func__);//logování
	if(MOD==NAHLED)
	{
		refresh_mGrid=true;//defaultně se tabulky refreshují, zakázání refreshe se provádí při určitých úkonech, toto zajistí zapnutí refreshe po ukončení úkonu při kterém byl tento refresh potlačen
		if(pom_element!=NULL)//ODSTRANĚNÍ předchozí případného highlightnutí elementu či tabulky
		{
			if(JID==0 || JID==1){pom_element->stav=1;refresh_mGrid=true;}//ELEMENT
  		pom_element->mGrid->Highlight=false;//tabulka zrušení highlightnutí
  	}
  	int puvJID=JID;//záloha původního JID
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
  			if(pom_element_puv!=NULL)pom_element_puv->mGrid->MouseMove(X,Y);//najistotu zrušení hintů a highlignutí odkazu normálních tabulek dodáním pouze aktuálních souřadnic
  			if(puvJID>=4 && puvJID<=10)PmG->MouseMove(X,Y);//najistotu hintů a zrušení highlignutí tabulky pohonu dodáním pouze aktuálních souřadnic
			}
			if(JID==0&&pom_temp->id!=3){if(pom_element->orientace==0||pom_element->orientace==180)kurzor(zmena_d_x);else kurzor(zmena_d_y);pom_element->stav=2;refresh_mGrid=false;}//posun ELEMENT
			if(JID==0&&pom_temp->id==3){kurzor(posun_ind);}//posun komory
			if(JID==1){kurzor(edit_text);pom_element->stav=3;refresh_mGrid=false;}//ELEMENT název
  		//použit závěrečný REFRESH if(pom_element!=pom_element_puv && (puvJID==0 || JID==0)/* || (puvJID==0 && JID==1) || (puvJID==1 && JID==0)*/){REFRESH();}//důvod k REFRESH, pouze v případě změny elementu či přechodu z názvu na celý element a opačně
  		//použit závěrečný REFRESH if(10<JID && JID<1000){REFRESH();}//hodnota kóty
			if(JID==100){kurzor(edit_text);pom_element->mGrid->CheckLink(X,Y);refresh_mGrid=true;}//název elementu v hlavičce tabulky - aktivace dodáním pouze aktuálních souřadnic
  		if(JID==1000){pom_element->mGrid->CheckLink(X,Y);refresh_mGrid=true;}//pouze pro přechod název hlavička, aby název nezůstal tučně - aktivace dodáním pouze aktuálních souřadnic
  		if(1000<=JID && JID<2000){kurzor(posun_ind);pom_element->mGrid->Highlight=true;pom_element->mGrid->MouseMove(X,Y);refresh_mGrid=true;}//indikace posunutí TABULKY, jeji highlignutí probíhá výše a případné volání HINTu
			if(100<JID && JID<1000){kurzor(zmena_j);pom_element->mGrid->CheckLink(X,Y);refresh_mGrid=true;}//první sloupec tabulky, libovolný řádek, v místě, kde je ODKAZ  - aktivace dodáním pouze aktuálních souřadnic
			if((JID==-6||JID<=-11)&&!editace_textu){kurzor(edit_text);refresh_mGrid=false;}//kurzor pro editaci textu
			if(JID<=-11&&JID>=-101&&d.v.PtInKota_komory(pom_temp,X,Y)==-1){if(pom_temp->orientace==90||pom_temp->orientace==270)kurzor(zmena_d_x);else kurzor(zmena_d_y);}//změna rozměru komory
			//použit závěrečný REFRESH if(-9<=JID && JID<=-6){REFRESH();}//refresh při akci s nadpisem či kótou kabiny
			if(JID==-10){/*REFRESH();*/kurzor(zmena_j);}//indikace možnosti změnit jednotky na kótách
			if(JID>=11 && JID<=99){if(pom_temp->orientace==90||pom_temp->orientace==270)kurzor(zmena_d_y);else kurzor(zmena_d_x);refresh_mGrid=false;}//interaktivní kóty elementů
			if(JID>=5 && JID<=10){kurzor(zmena_j);if(PmG->CheckLink(X,Y)!=TPoint(-1,-1));refresh_mGrid=true;PmG->Refresh();}//pohonová tabulka odkazy - aktivace dodáním pouze aktuálních souřadnic
			if(JID==4){PmG->Highlight=true;}//highlightování tabulky pohonů při jejím přejetí
			if(JID==-2)//posun úsečky objektu
			{
				refresh_mGrid=false;
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else A=pom_temp->body->predchozi;
				//zjištění azimutu úsečky + nastavení kurzoru
				if(A->X==B->X)kurzor(zmena_d_x);
				else if(A->Y==B->Y)kurzor(zmena_d_y);
				//úsečka a kóta se posouvají rozdílně, proto jiné kurzory
				else kurzor(posun_ind);
				//smazání pomocných ukazatelů
				A=NULL;B=NULL;delete A;delete B;
			}
			if(JID==-3){kurzor(posun_ind);refresh_mGrid=false;}//kurzor bodo objektu
			if(JID==-4)//oblast kóty objektu
			{
				refresh_mGrid=false;
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else A=pom_temp->body->predchozi;
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
			////inteligentní REFRESH
  		//if(!refresh_mGrid/* && !nabuffrovano*/){d.nabuffrovat_mGridy();nabuffrovano=true;}
  		//d.nabuffrovat_mGridy(pom_element->mGrid);
			//if(!refresh_mGrid)Memo("false");else Memo("true");
			if(element_id!=-1 && Akce !=ADD){refresh_mGrid=true;element_id=-1;}//mazání eid po vkládání, podmínka nutná ke správnému vykreslení mgridů
			REFRESH();
			//refresh_mGrid=true;
		}
		if(puvJID==-1 && JID==-1 && PmG->Highlight){PmG->Highlight=false;REFRESH();}//odstranění highlightu tab pohonu pokud je třeba, zamezuje problikávání highlightu

  	////oblasti poznámek pod čarou - NOTE, nejdou přes JID
  	if(JID==-1)
		{                                                       //zajištuje unhighlight odkazu
			if(PmG!=NULL)if(PmG->CheckLink(X,Y)==TPoint(-2,-2) || PmGCheckLink){PmGCheckLink=true;refresh_mGrid=true;REFRESH();}
			if(FormX->naplneni_max_voziku(X,Y,true) || StopCheckLink){StopCheckLink=true;refresh_mGrid=true;REFRESH();}
			//pokud budu chtít ještě získat CheckLink z tabulek elementů budu muset rozšířit testování oblasti tabulky a aby vrátila platný ukazatel na element resp. jeho mgrid
		}

		////vynulování a odstranění pomocného ukazatele na element
		pom_element_puv=NULL;delete pom_element_puv;
		pom_komora_puv=NULL;delete pom_komora_puv;
	}
	if(MOD==SCHEMA)//pro schéma, změna kurzorů pro body a úsečky
	{
		int puvJID=JID;//záloha původního JID
		Cvektory::TBod *pom_bod_puv=pom_bod;
		getJobID(X,Y);//zjištění aktuálního JID
		if(puvJID!=JID || pom_bod_puv!=pom_bod)//pokud došlo ke změně JID, nebo změně bodu bez změny JID, jinak nemá smysl řešit
		{
			//kurzory (provizorně), ...
			kurzor(standard);
			if(JID==-2)kurzor(edit_text);//hodnota kóty
			if(JID==0||(JID==3&&scButton_zamek_layoutu->ImageIndex==68))kurzor(posun_ind);//posun bodu
			if(JID==1||JID==4)//posun úsečky
			{
				//načtení bodů úsečky
				Cvektory::TBod *A=NULL,*B=pom_bod;//return proměnná + krokování cyklu
				if(pom_bod->n!=1)A=pom_bod->predchozi;
				else if(pom!=NULL&&pom->body!=NULL)A=pom->body->predchozi;
				else A=d.v.HALA.body->predchozi;
				//zjištění azimutu úsečky + nastavení kurzoru
				if(A->X==B->X)kurzor(zmena_d_x);
				else if(A->Y==B->Y)kurzor(zmena_d_y);
				//úsečka a kóta se posouvají rozdílně, proto jiné kurzory
				else kurzor(posun_ind);
				//smazání pomocných ukazatelů
				A=NULL;B=NULL;delete A;delete B;
			}
			if(JID==2)//oblast kóty
			{
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
			REFRESH();
		}
		pom_bod_puv=NULL;delete pom_bod_puv;
	}
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
  if(Screen->Cursor!=standard)kurzor(standard);//ošetření proti zůstávání kurzoru
	//výchozí skrytí všech položek, další postup je založen na postupném odkrývání a zvětšování panelu UP nebo DOWN
	close_all_items_popUPmenu();
	PopUPmenu->Item_zobrazit_parametry->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
	//dle statusu Architek x Klient resp. Návrh x Ověrování
	AnsiString N="Editovat";if(STATUS==OVEROVANI)N="Zobrazit";
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
			pom_vyhybka=d.v.PtInObjekt();//využití prázdného ukazatele pro uchovávání nalezeného objektu (přechod z náhledu do náhledu)
			pom_element_temp=pom_element;   if(pom_vyhybka==NULL)pom_vyhybka=pom_temp;
			mazani=true;
			////přesun z náhledu do náhledu
			if(/*pom_vyhybka!=NULL && pom_vyhybka->n!=pom_temp->n && */AnsiString("Editovat "+pom->name).Length()>19)PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom_vyhybka->name.UpperCase();
			else /*if(pom_vyhybka!=NULL && pom_vyhybka->n!=pom_temp->n)*/PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom_vyhybka->name.UpperCase();
			if (pom_element!=NULL && Akce!=GEOMETRIE)//Pokud bylo kliknuto na element + ošetření pom_element je používan i při tvorbě geometrie
			{
				if(AnsiString("Editovat "+pom_element->name).Length()>19) PopUPmenu->scLabel_smazat->Caption="  Smazat\n  "+pom_element->name.UpperCase();
				else PopUPmenu->scLabel_smazat->Caption="  Smazat "+pom_element->name.UpperCase();
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_komora!=NULL)
			{
				pom_komora_temp=pom_komora;
				PopUPmenu->scLabel_nastavit_parametry->Caption="  Změnit typ sekce";
				PopUPmenu->scLabel_smazat->Caption="  Smazat sekci č. "+AnsiString(pom_komora->n);
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_vyhybka!=NULL && pom_bod==NULL /*&& pom_vyhybka->n!=pom_temp->n*/){PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
			////mazání a přidávání bodu do polygonu obrysu kabiny
			if(pom_bod!=NULL && JID==-3)//bod = mazání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_smazat->Caption="  Smazat bod č. "+AnsiString(pom_bod->n);
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_bod!=NULL && JID==-2)//usečka = přidávání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_nastavit_parametry->Caption="  Přidat bod";
				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_bod!=NULL || (pom_vyhybka!=NULL && pom_vyhybka->n==pom_temp->n)){PopUPmenu->Item_posun_obrysu->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
			////
			//PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_posunout->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_priblizit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_oddalit->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_vybrat_oknem->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			PopUPmenu->Item_cely_pohled->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
			break;
		}
		default://pro SCHEMA
		{
			//povoluje nastavení položek kopírování či smazání objektu
//			pom=d.v.najdi_objekt(m.P2Lx(X),m.P2Ly(Y),d.O_width*m2px,d.O_height*m2px);
//			pom_bod=d.v.najdi_bod(pom);
			if(pom!=NULL)// nelze volat přímo metodu najdi objekt, protože pom se používá dále
			{
        pom_vyhybka=pom;//uchovávání ukazatele pro případ, že uživatel po zobrazení popup menu sjede kurzorem z objektu
				if(AnsiString("Editovat "+pom->name).Length()>19)//pokud je více znaků, tak zalamovat manuálně, lze i automaticky pomocí proporties wordwrap, ale to se nemusí projevit např. u všech různě textově dlouhých položek stejně
				{
					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+"\n  "+pom->name.UpperCase();
					PopUPmenu->scLabel_kopirovat->Caption="  Kopírovat\n  "+pom->name.UpperCase();
					PopUPmenu->scLabel_smazat->Caption="  Smazat\n  "+pom->name.UpperCase();
				}
				else
				{
					PopUPmenu->scLabel_nastavit_parametry->Caption="  "+N+" "+pom->name.UpperCase();
					PopUPmenu->scLabel_kopirovat->Caption="  Kopírovat "+pom->name.UpperCase();
					PopUPmenu->scLabel_smazat->Caption="  Smazat "+pom->name.UpperCase();
				}
				PopUPmenu->Item_otocit_doleva->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				PopUPmenu->Item_otocit_doprava->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				//pozor rozhoduje pořadí
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);//workaround, nutnost takto vytáhnout, jinak se položka zvýrazňuje, musí být tady
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				PopUPmenu->Item_kopirovat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
				if((long)pom->id!=VyID&&(long)pom->id!=pocet_objektu_knihovny+1)
				{PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;}
			}
			if(pom_bod!=NULL && JID==0)//bod = mazání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_smazat->Caption="  Smazat bod č. "+AnsiString(pom_bod->n);
				PopUPmenu->Item_smazat->FillColor=(TColor)RGB(240,240,240);
				PopUPmenu->Item_smazat->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_bod!=NULL && JID==1)//usečka = přidávání bodu
			{
				pom_bod_temp=pom_bod;
				PopUPmenu->scLabel_nastavit_parametry->Caption="  Přidat bod";
				PopUPmenu->Item_nastavit_parametry->Visible=true;PopUPmenu->Panel_UP->Height+=34;
			}
			if(pom_bod!=NULL || d.v.PtInBody()!=NULL || pom!=NULL){PopUPmenu->Item_posun_obrysu->Visible=true;PopUPmenu->Panel_UP->Height+=34;}

			//zobrazení běžných položek, pozor rozhoduje pořadí
			//PopUPmenu->Item_posouvat->Visible=true;PopUPmenu->Panel_DOWN->Height+=34;
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

	PopUPmenu->Panel_UP->Height=0;
	PopUPmenu->Panel_DOWN->Height=0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Piblit1Click(TObject *Sender)
{
	ZOOM_IN();
	zobraz_tip("TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru.");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Priblizit2Click(TObject *Sender)
{
	ZOOM_IN();
	zobraz_tip("TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru.");
}
void __fastcall TForm1::RzToolButton8Click(TObject *Sender)//Zoom in z toolbaru
{
 vycentrovat=false;
 akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
 ZOOM_IN();
 zobraz_tip("TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru.");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Oddlit1Click(TObject *Sender)
{
	ZOOM_OUT();
	zobraz_tip("TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu.");
}
void __fastcall TForm1::Oddalit2Click(TObject *Sender)
{
	ZOOM_OUT();
	zobraz_tip("TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu.");
}
void __fastcall TForm1::RzToolButton9Click(TObject *Sender)//Zoom out z toolbaru
{
	vycentrovat=false;
	akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
	ZOOM_OUT();
	zobraz_tip("TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu.");
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//přiblížení
void TForm1::ZOOM_IN()
{
		if(MOD==NAHLED&&zobrazeni_tabulek&&Zoom==2.5) scGPButton_viditelnostmGridClick(this);
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
		if(MOD==NAHLED)
		{
			if(Zoom>=3)zobrazeni_tabulek=pom_temp->zobrazit_mGrid;
			if(Zoom==3&&pom_temp->zobrazit_mGrid)scGPButton_viditelnostmGridClick(this);
		}
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
//vytvoří výřez pro pan_move
void TForm1::pan_create()
{
	int W=scSplitView_LEFTTOOLBAR->Width;
	if(MOD==CASOVAOSA || MOD==TECHNOPROCESY)W=0;//zajistí, že se posová i číslování vozíků resp.celá oblast
	short H=scGPPanel_mainmenu->Height;
	int Gh=vrat_max_vysku_grafu();if(scGPPanel_bottomtoolbar->Visible)Gh=scGPPanel_bottomtoolbar->Height;
	Gh-=6;//WA, z nějaké důvodu to chce odebrat, aby byla posouváná plocha kompletní
	Pan_bmp->Width=ClientWidth;Pan_bmp->Height=ClientHeight-H-Gh;//velikost pan plochy
	Pan_bmp->Canvas->CopyRect(Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh),Canvas,Rect(0+W,0+H,ClientWidth,ClientHeight-H-Gh));//uloží pan výřez
	//Pan_bmp->SaveToFile("test.bmp");  //pro testovací účely
}
//---------------------------------------------------------------------------
//Posouvá výřez mapy při stisknutém mezerníku a L-myši
void TForm1::pan_map(TCanvas * canv, int X, int Y)
{
	////zajištění skrytí komponent, vedlejší produkt metody d.vykresli_mGridy();, protože má v sobě podmínky při pan_move
	if(pom_temp!=NULL)if(pom_temp->zobrazit_mGrid)d.vykresli_mGridy();

	////vykreslení aktuální pan_bmp
	canv->Brush->Color=clWhite/*clLtGray*/;canv->Brush->Style=bsSolid;
	//maže při posouvání obrazu starý obraz
	canv->FillRect(TRect(0,0,/*X-kurzor_souradnice_puvodni.x+Pan_bmp->Width*/ClientWidth,Y-vychozi_souradnice_kurzoru.y));//horní okraj
	canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y,X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height));//levy okraj
	canv->FillRect(TRect(X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,Y-vychozi_souradnice_kurzoru.y,ClientWidth,ClientHeight));//pravy okraj
	canv->FillRect(TRect(0,Y-vychozi_souradnice_kurzoru.y+Pan_bmp->Height,X-vychozi_souradnice_kurzoru.x+Pan_bmp->Width,ClientHeight));//dolní okraj
	//samotné posouvání Pan_bmp
	canv->Draw(X-vychozi_souradnice_kurzoru.x,Y-vychozi_souradnice_kurzoru.y,Pan_bmp);
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
	if(pom_vyhybka!=NULL || pom_bod!=NULL || (d.v.PtInBody()!=NULL && pom_temp==NULL) || pom!=NULL)//posun obrysu objektu (z editace i z layoutu) a posun haly (pouze layout)
	{
		Akce=MOVE_HALA;
		if(pom_temp!=NULL)nahled_ulozit(true);
		TIP="Kliknutím určíte bod posunu, táhnutím posunete obrys objektu.";
		refresh_mGrid=false;
		REFRESH();
		refresh_mGrid=true;
	}
	else//funkce posouvat
	{
  	Akce=PAN;
		kurzor(pan);
		zobraz_tip("TIP: Posun obrazu lze také vykonat pomocí stisknutého levého tlačítka myší a posunem myši požadovaným směrem.");
  }
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
	zobraz_tip("TIP: Posun obrazu lze také vykonat pomocí stisknutého levého tlačítka myší a posunem myši požadovaným směrem.");
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
		//načtení proměnných
		TRect oblast=vrat_max_oblast();
		double MaxX=m.P2Lx(oblast.right),MaxY=m.P2Ly(oblast.top),MinX=m.P2Lx(oblast.left),MinY=m.P2Ly(oblast.bottom);
		int PD_x=ClientWidth-scSplitView_LEFTTOOLBAR->Width;
		int PD_y=ClientHeight-vyska_menu-scGPPanel_statusbar->Height;//-vyska_menu-RzStatusBar1->Height je navíc nemá tam co dělat
		//výpočet nového Zoomu
		double rozdil=0,PD=0;
		if(m.m2px(MaxX-MinX)>m.m2px(MaxY-MinY)){rozdil=m.m2px(MaxX-MinX);PD=PD_x;}
		else {rozdil=m.m2px(MaxY-MinY);PD=PD_y;}
		Zoom=abs(Zoom*PD/rozdil);
		//přepočtení na používaný krok zoomu
		Zoom-=fmod(Zoom,0.5);
		if(Zoom<0.5)Zoom=0.5;if(Zoom>4)Zoom=4;
		//vycentrování obrazu
		PD_x=ClientWidth+scSplitView_LEFTTOOLBAR->Width;
		PD_y=ClientHeight+vyska_menu-scGPPanel_statusbar->Height;
		Posun.x=(MaxX+MinX)/2/m2px-PD_x/2/Zoom;
		Posun.y=-(MaxY+MinY)/2/m2px-PD_y/2/Zoom;
  	if(MaxX+MinX==0)//v případě, že není objekt
		{
			 Posun.x=-scListGroupKnihovObjektu->Width;if(vyska_menu>0)Posun.y=-vyska_menu+9;else Posun.y=-29;
		}

		on_change_zoom_change_scGPTrackBar();
  }catch(...){};
	REFRESH();
	DuvodUlozit(true);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TForm1::ESC()
{
	if(TIP!="Kliknutím na objekt v knihovně objektu, tažením a následným usazením přidáte objekt.")TIP="";//smazání zobrazeného tipu
	if(Akce==DRAW_HALA&&d.v.HALA.body!=NULL&&d.v.HALA.body->predchozi->n>2){d.v.vloz_bod(d.v.HALA.body->dalsi->X,d.v.HALA.body->dalsi->Y,pom,NULL,ortogonalizace_stav,true);Akce=NIC;kurzor(standard);REFRESH();}else if(Akce==DRAW_HALA){d.v.vymaz_body();Akce=NIC;kurzor(standard);REFRESH();}
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
				d.vykresli_kurzor_kabiny(Canvas,vybrany_objekt,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,pom);
				pom=NULL;//nesmí být v další sekci
			}
			else
			{
				if(pom->id==3)d.vykresli_ikonu_komory(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"",-1);
				else d.vykresli_element(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"","",element_id,-1,Rotace_symbolu_minula);
			}
			REFRESH(false);//odstranění vykresleného kurzoru
		}break;
		case VYH:
		{
			d.odznac_oznac_vetev(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,pom_vyhybka);
			d.v.smaz_objekt(pom_vyhybka);
			REFRESH();//dojde k překreslení odstraněné výhybky
		}break;
		case GEOMETRIE:ukonceni_geometrie();break;
	}
	pom_vyhybka=NULL;
	proces_pom=NULL;
	kurzor(standard);
	if(!editace_textu)Akce=NIC;//musí být nad refresh
	Akce_temp=NIC;
	if(pom_temp!=NULL){scGPImage_mereni_vzdalenost->ClipFrameFillColor=clWhite;scGPImage_zamek_posunu->ClipFrameFillColor=clWhite;}//pro případ ukončení akce měření
	//vrácení původního textu při ukončení editace
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
			case -6:pom_temp->name=nazev_puvodni;break;
			case -7:pom_temp->short_name=nazev_puvodni;break;
			case -5:if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(pom_temp->body->predchozi->X,pom_temp->body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));break;
		}
		if(index_kurzoru==-9||index_kurzoru==-8)//editace kót kabiny
		{
			if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;//pokud jsou kóty v časovém režimu převede vzdálenost na čas
			editovany_text=outDK(ms.MyToDouble(editovany_text));//převede na aktuálně používané jednotky
		}
		if(index_kurzoru<=-11&&pom_temp->id!=3)//editace kót elementů
		{
			if(index_kurzoru!=-101)editovany_text=d.v.vzdalenost_od_predchoziho_elementu(pom_element_temp);//načtení vzdálenosti
			else editovany_text=vzdalenost_meziLO(pom_element_temp,pom_temp->orientace);
			if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;//pokud jsou kóty v časovém režimu převede vzdálenost na čas
			editovany_text=outDK(ms.MyToDouble(editovany_text));//převede na aktuálně používané jednotky
		}
		if(index_kurzoru<=-11&&pom_temp->id==3)editovany_text=outDK(pom_komora_temp->velikost);
		Smaz_kurzor();//uložení změn editace a odstranění kurzoru
	}
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
		//připínání objektu na ostatní
		short oblast=0;
		if(d.v.OBJEKTY->dalsi!=NULL)oblast=d.v.oblast_objektu(d.v.OBJEKTY->predchozi,X,Y);
		if(oblast==1){souradnice.x=d.v.OBJEKTY->predchozi->elementy->predchozi->geo.X4;souradnice.y=d.v.OBJEKTY->predchozi->elementy->predchozi->geo.Y4;}//za objekt
		if(oblast==2 && pom==NULL){souradnice.x=d.v.OBJEKTY->predchozi->elementy->dalsi->geo.X1;souradnice.y=d.v.OBJEKTY->predchozi->elementy->dalsi->geo.Y1;}//před objekt
		//pokud přidávám výhybku je nutné přičíst k počtu vyhybek
		if(vybrany_objekt==VyID&&Akce!=VYH)d.v.pocet_vyhybek++;
		//uložení do paměti
		bool spojka=false;
		if(add_posledni&&Akce==ADD)//vloží za poslední prvek
		{ //do pom_vyhybka přebírá pouze pro případné účely vyhýbky, pro ostatní objekty má význam metoda bez návratové hodnoty
			pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y);
		}
		else if(Akce==ADD)//vkládá prvek mezi prvky
		{ //do pom_vyhybka přebírá pouze pro případné účely vyhýbky,, pro ostatní objekty má význam metoda bez návratové hodnoty
			Cvektory::TObjekt *dalsi;
			if(d.v.akt_vetev)dalsi=pom->dalsi;else dalsi=pom->dalsi2;
			pom_vyhybka=d.v.vloz_objekt(vybrany_objekt,souradnice.x,souradnice.y,pom,dalsi);
			dalsi=NULL;delete dalsi;
			d.v.nove_indexy();//zvýší indexy nasledujicích bodů
		}
		else//zde se bude vkládat spojka, zatím test co je v ukazatelých
		{
			bool vkladat=false;
			if(pom!=NULL)
			{
		  	if(pom->dalsi!=NULL)if(pom->dalsi->n!=pom_vyhybka->n&&pom->n!=pom_vyhybka->n)vkladat=true;
		  	if(pom->dalsi==NULL)if(pom->n!=pom_vyhybka->n)vkladat=true;
				if(pom->dalsi2!=NULL)if(pom->dalsi2->n!=pom_vyhybka->n&&pom->n!=pom_vyhybka->n)vkladat=true;
			}
			else if(d.v.OBJEKTY->predchozi->n!=pom_vyhybka->n)vkladat=true;
			if(vkladat)
			{
				Cvektory::TObjekt *dalsi,*predchozi=pom;//pomocný ukazatel na další a předchozí objekt za spojkou
				if(pom!=NULL)//pokud byl nalezen předchozí objekt a jedná se o výhybku je nutné určit do jaké větve se bude vkládat
				{if(d.lezi_v_pasmu(Canvas,X,Y,pom)==2)dalsi=predchozi->dalsi2;else dalsi=predchozi->dalsi;}//pokud metoda lezi_v_pasmu vrátí 2, znamená to, že se bude vkládat do sekundární větve
				else {dalsi=NULL;predchozi=d.v.OBJEKTY->predchozi;}//pokud neexistuje předchozí element, znamená to, že spojka je vkládána na konec, tudíž pom(předchozí)=poslední objekt, dalsi = NULL
				Cvektory::TObjekt *S=d.v.vloz_objekt(pocet_objektu_knihovny+1,souradnice.x,souradnice.y,pom_vyhybka,predchozi,dalsi);//samotné přidání spojky
				predchozi=NULL;dalsi=NULL;S=NULL;delete S;delete dalsi;delete predchozi;//vynulování a smazání vytvořených ukazatelů
				spojka=true;//důlezité pro vypnutí akce
				if(pom!=NULL)d.v.nove_indexy();//pokud nebylo provedeno vkládání na konec
			}
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
		if(d.v.OBJEKTY->dalsi!=NULL && d.v.OBJEKTY->predchozi->n==1)Nahled->Enabled=true;
		TIP="";//odstranění původní nápovědy pro přidávání objektu
		REFRESH();
		DuvodUlozit(true);
	}
}
//---------------------------------------------------------------------------
Cvektory::TObjekt *TForm1::add_objekt_za()
{
	Cvektory::TObjekt *p=d.v.OBJEKTY->dalsi;//přeskočí hlavičku
	Cvektory::TObjekt *ret=NULL;
	TPoint *tab_pruchodu=new TPoint[F->d.v.pocet_vyhybek+1];//+1 z důvodu indexace výhybka 1 bude mít index 1, nebude se začínat od indexu 0, tabulka.x = vyhybky, tabulka.y = spojky
	while (p!=NULL)
	{ //kontrola zda se jedná o poslední první objekt byla přesunuta do metody lezi_v_pasmu
		if(p->dalsi!=NULL && m.LeziVblizkostiUsecky(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,m.L2Px(p->elementy->predchozi->geo.X4),m.L2Py(p->elementy->predchozi->geo.Y4),m.L2Px(p->dalsi->elementy->dalsi->geo.X1),m.L2Py(p->dalsi->elementy->dalsi->geo.Y1))<=20){ret=p;break;}
//		if(d.lezi_v_pasmu(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,p)>0)
//		{
//			ret=p;
//			break;
//		}
		p=d.v.dalsi_krok(p,tab_pruchodu);//vrátí další krok (dalsi/dalsi2)
	}
	tab_pruchodu=NULL;delete tab_pruchodu;
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
		TPoint *tab_pruchodu=new TPoint[F->d.v.pocet_vyhybek+1];//+1 z důvodu indexace výhybka 1 bude mít index 1, nebude se začínat od indexu 0, tabulka.x = vyhybky, tabulka.y = spojky
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
			ukaz=d.v.dalsi_krok(ukaz,tab_pruchodu);//posun na další prvek v seznamu
		}
    tab_pruchodu=NULL;delete tab_pruchodu;
		//pokud se může jednat o snahu (zjištěno z předchozí navrácenoho RET) o vložení ještě se na to dotazuje u uživatele
		if(RET)
		{
			if(ukaz==d.v.OBJEKTY->predchozi)//první prvek versus poslední
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt "+AnsiString(pom->name.UpperCase())+" umístit v pořadí mezi objekty "+AnsiString(ukaz->name.UpperCase())+" a "+AnsiString(d.v.OBJEKTY->dalsi->name.UpperCase())+"?","",MB_YESNO,true,false))
				{
					d.v.zmen_poradi_objektu(pom,d.v.OBJEKTY->predchozi);//volání realizace samotné záměny
				}
			}
			else//ostatní situace
			{

				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete objekt "+AnsiString(pom->name.UpperCase())+" umístit v pořadí mezi objekty "+AnsiString(ukaz->name.UpperCase())+" a "+AnsiString(ukaz->dalsi->name.UpperCase())+"?","",MB_YESNO,true,false))
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
	TPoint bod_vlozeni=bod_vlozeni_elementu();
	if(bod_vlozeni.x!=-1000 && pom_element_temp!=NULL)//kontrola zda vkládám na přímku
	{
  	FormX->vstoupeno_poh=false;//blokace událostí při vkládání elementu
  	FormX->vstoupeno_elm=false;
  	short trend=pom_temp->orientace;
		short rotace_symbolu=rotace_symbol(m.Rt90(pom_element_temp->geo.orientace),bod_vlozeni.x,bod_vlozeni.y);
		bod_vlozeni=uprav_bod_vlozeni_elementu(bod_vlozeni,rotace_symbolu);
		//vložení elementu na dané souřadnice a do patřičného pomocného spojáku, pro případ storna
		TIP="";//smazání tipu, pro jistotu
		Cvektory::TElement *E=d.v.vloz_element(pom_temp,element_id,m.P2Lx(bod_vlozeni.x),m.P2Ly(bod_vlozeni.y),rotace_symbolu);
		//nadesignuje tabulky daného elementu
		design_element(E,true);
		if(E->eID!=100 && E->eID!=MaxInt)//pokud je alokovaná paměť pro mGrid element bude mít tabulku, pokud není element nebude mít tabulku
		{
			//automatické výchozí umístění mGridové tabulky dle rotace elementu a nadesignováné tabulky (jejích rozměrů) - proto musí být až za nastevením designu
			aut_pozicovani(E,X,Y);
		}
		//dřívě přítomen Update(), nutné před plněním COMBA, Update() se nyní vyskytuje v metodě design_element;
		//d.v.napln_comba_stopek();
		//přiřazení režimu kabině
		switch(E->eID)
		{
			case 0://po přidání stop nebo otoče režim žádny
			case 5:
  		case 6:/*nic*/;break;
			case 1://po přidání KK režim 1
			case 7:case 9:case 11:case 13:case 15:case 17:case 101:case 103:case 105:case 107:
  		case 3:pom_temp->rezim=1;break;
			case 2://po přidání S&G režim 0
			case 8:case 10:case 12:case 14:case 16:case 18:case 102:case 104:case 106:case 108:
			case 4:pom_temp->rezim=0;break;
  		default:break;
		}
		//volání přenastavení tabulky pohonu, pouze v případech, kdy je to nutné
  	int KK1,KK2;//princip je stejný podle režimu mění se pouze eID v různých kabinách
  	switch(pom_temp->id)
  	{
			case 0:case 9:KK1=101;KK2=103;break;//navěšování + svěšování
  		case 1:KK1=15;KK2=17;break;//CO2
  		case 2:if(scGPSwitch_robot_clovek->State==0){KK1=11;KK2=13;}else {KK1=105;KK2=107;}break;//ožeh
  		case 4:if(scGPSwitch_robot_clovek->State==0){KK1=7;KK2=9;}else {KK1=105;KK2=107;}break;//ionizce
  		case 5:if(scGPSwitch_robot_clovek->State==0){KK1=1;KK2=3;}else {KK1=105;KK2=107;}break;//lakovna
  	}
		if((!PmG->Rows[3].Visible&&(E->eID==KK1||E->eID==KK2))||(PmG->Rows[3].Visible&&!PmG->Rows[7].Visible&&(E->eID==KK1||E->eID==KK2||E->eID==5||E->eID==6)))
  		pridani_elementu_tab_pohon(E);
  	//REFRESH(true);//nutné zde z důvodu dobrého vykreslení tabulek
  	if(E->dalsi!=NULL)//spouštěno jen pokud před elementem existuje jiný, snažím se najít další S&G element, tomuto elementu je nutné přepočítat RT
  	{
  		Cvektory::TElement *A=pom_temp->elementy->dalsi;
  		while(A!=NULL)
			{
				if(A->eID%2==0 && A->eID!=100 && A->eID!=200 && A->eID!=MaxInt && A->n!=E->n){d.v.posuv_aktualizace_RT(A);break;}
  			A=A->dalsi;
  		}A=NULL;delete A;
  	}
  	//kontrola překrytí lak. oken
  	bool prekryti=prekryti_LO(E);
		if(prekryti && mrYes!=MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Vložením dojde k překrytí lakovacích oken, chcete element vložit?","",MB_YESNO))
			{d.v.smaz_element(E);E=NULL;}
		//vložení předávacího místa
		if(E!=NULL && E->eID==200)
		{
      E=E->dalsi;//posunutí se za předávací místp
			while(E!=NULL)//odmazání pohonu za zarážkou
			{
				E->pohon=NULL;
				set_enabled_mGrid(E);
				E=E->dalsi;
			}
			vlozit_predavaci_misto();//kontrola zda nemá být na konec kabiny vloženo předávací místo
		}
		if(E!=NULL)d.v.aktualizuj_sparovane_ukazatele();//aktualizace spárovaných ukazatelů
		//až na konec:
		E=NULL;delete E;
		Akce=NIC;
		element_id=99999;//ošetření pro správné zobrazování mgridů
		DrawGrid_knihovna->Invalidate();
		DuvodUlozit(true);
		//Zde vložit podmínku pro kontrolu jaký element byl vložen, na základě toho znemožnit klik na roboty opačné funkcionality
		nahled_ulozit(true);//důvod k uložení náhledu
		refresh_mGrid=true;//nutné pro správné zobrazení mgridů po přidání elementu
	}
	else TIP="Lze vkládat pouze na linie.";//hláška uživateli
	REFRESH();
}
//---------------------------------------------------------------------------
//přidávání komory kabině powerwashe, kontrola zda není součet kabin větší než rozměr kabiny
void TForm1::add_komoru()
{                 //stejné komory přidat jejich šířku jinak 2.5
	log(__func__);//logování
	if(element_id==-1)element_id=0;else element_id=1;//změna typu (předtím typ pro kurzory, nově typ do dat)
	//určení velikosti komory
	Cvektory::TKomora *k=pom_temp->komora->dalsi;
	double velikost=k->velikost;
	while(k!=NULL)//procházení přes všechny komory, suma jejich velikostí
	{
		if(velikost!=k->velikost){velikost=-100;break;}
		k=k->dalsi;
	}
	k=NULL;
	if(velikost==-100)velikost=2.5;//pokud nejsou stejné vložím defaul rozměr
	//vložení komory
	d.v.vloz_komoru(pom_temp,velikost,d.v.najdi_komoru(pom_temp),element_id);//vloží novou komoru, mezi ostatní či jako poslední
	//změna rozmerů kabiny
	k=pom_temp->komora->dalsi;
	double celkem=0;
	while(k!=NULL)//procházení přes všechny komory, suma jejich velikostí
	{
		celkem+=k->velikost;
		k=k->dalsi;
	}
	k=NULL;delete k;
	//změna rozměru kabiny
	double rozmer_kabiny_x=pom_temp->body->dalsi->dalsi->X-pom_temp->body->dalsi->X,rozmer_kabiny_y=pom_temp->body->dalsi->dalsi->Y-pom_temp->body->dalsi->Y;
	double posun_x=celkem-rozmer_kabiny_x,posun_y=celkem-rozmer_kabiny_y;
	if(pom_temp->orientace==270 || pom_temp->orientace==180){posun_x=-celkem-rozmer_kabiny_x;posun_y=-celkem-rozmer_kabiny_y;}
	if((pom_temp->orientace==90 || pom_temp->orientace==270) && m.abs_d(celkem)>m.abs_d(rozmer_kabiny_x))d.v.posun_hranu(posun_x,0,pom_temp->body->dalsi->dalsi,pom_temp->body->dalsi->dalsi->dalsi);
	if((pom_temp->orientace==0 || pom_temp->orientace==180) && m.abs_d(celkem)>m.abs_d(rozmer_kabiny_y))d.v.posun_hranu(0,posun_y,pom_temp->body->dalsi->dalsi,pom_temp->body->dalsi->dalsi->dalsi);
	Akce=NIC;
	refresh_mGrid=false;
	REFRESH();
	refresh_mGrid=true;
}
//---------------------------------------------------------------------------
//projde elementy a objekty, pokud je někde nutnost vložit předávací místo vloží ho tam
void TForm1::vlozit_predavaci_misto()
{
	log(__func__);//logování
	Cvektory::TObjekt *O=d.v.OBJEKTY->dalsi;
	while(O!=NULL)
	{
		Cvektory::TElement *E=O->elementy->dalsi;
		if(pom_temp!=NULL && O->n==pom_temp->n)E=pom_temp->elementy->dalsi;
		while(E!=NULL)
		{
			//deklarace atributů
			long ID=0;
			Cvektory::TObjekt *O_pom=O;//pomocný ukazatel uchovává ukazatel na další objekt, pokud nasledující objekt je právě editován je nutné načíst z pom_temp
			if(O->dalsi!=NULL)O_pom=O->dalsi;
			if(pom_temp!=NULL && O->dalsi!=NULL && O->dalsi->n==pom_temp->n)O_pom=pom_temp;
			//////////Vkládání předávacího místa
			if(E->eID!=200 && ((E->dalsi!=NULL && E->dalsi->pohon!=NULL && E->pohon!=NULL && E->dalsi->pohon->n!=E->pohon->n) || (E->dalsi==NULL && O->dalsi!=NULL && O_pom->elementy->dalsi->pohon!=NULL && E->pohon!=NULL && O_pom->elementy->dalsi->pohon->n!=E->pohon->n) || (E->dalsi!=NULL && (E->dalsi->pohon==NULL && E->pohon!=NULL || E->dalsi->pohon!=NULL && E->pohon==NULL)) || (E->dalsi==NULL && O->dalsi!=NULL && (O_pom->elementy->dalsi->pohon==NULL && E->pohon!=NULL || O_pom->elementy->dalsi->pohon!=NULL && E->pohon==NULL))))
			{
				//WT přiřazení
				E->WT=0;//čekání na palec
				if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)E->WT=m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
				if(E->dalsi==NULL && O->dalsi!=NULL && O_pom->elementy->dalsi->pohon!=NULL)E->WT=m.cekani_na_palec(0,O_pom->elementy->dalsi->pohon->roztec,O_pom->elementy->dalsi->pohon->aRD,3);
				//souřadnice tabulky
				if(O->orientace==90 || O->orientace==270){E->Xt=E->X-1.9;E->Yt=E->Y+1.6;}
				else{E->Xt=E->X+0.6;E->Yt=E->Y+0.5;}
        //změna elemetnu na předávací místo
				E->eID=200;
				//názvy výhybek prozatím neřešeny
				if(pom_temp==NULL)pom_vyhybka=O;else pom_vyhybka=pom_temp;
				unsigned int nTyp=d.v.vrat_poradi_elementu_do(pom_vyhybka,E)+1;
				E->name="Předávací místo "+AnsiString(nTyp);
				E->short_name=E->name.SubString(1,3)+AnsiString(nTyp);
				pom_vyhybka=NULL;
				//smazání a znovuvytvoření mGridu elementu
				if(pom_temp!=NULL && O->n==pom_temp->n)
				{
					ID=E->mGrid->ID;
					E->mGrid->Delete();
					E->mGrid=new TmGrid(F);
					E->mGrid->Tag=6;//ID formu
					E->mGrid->ID=ID;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
					design_element(E,false);//nutné!
				}
			}
      //////////Aktualizace WT
			if(E->eID==200)
			{
				if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)E->WT=m.cekani_na_palec(0,E->dalsi->pohon->roztec,E->dalsi->pohon->aRD,3);
				if(E->dalsi==NULL && O->dalsi!=NULL && O_pom->elementy->dalsi->pohon!=NULL)E->WT=m.cekani_na_palec(0,O_pom->elementy->dalsi->pohon->roztec,O_pom->elementy->dalsi->pohon->aRD,3);
				if(pom_temp!=NULL && pom_temp->n==O->n)E->mGrid->Refresh();
			}
			//////////Mazání nepotřebného předávacího místa
			if(E->eID==200 && (E->dalsi!=NULL && E->dalsi->pohon==NULL && E->pohon==NULL || E->dalsi!=NULL && E->pohon!=NULL && E->dalsi->pohon!=NULL && E->dalsi->pohon->n==E->pohon->n || E->dalsi==NULL && O->dalsi!=NULL && E->pohon!=NULL && O_pom->elementy->dalsi->pohon!=NULL && O_pom->elementy->dalsi->pohon->n==E->pohon->n || E->dalsi==NULL && O->dalsi!=NULL && O_pom->elementy->dalsi->pohon==NULL && E->pohon==NULL))
			{
				//pokud je možné dojde k odstranění elementu
				if(E->dalsi!=NULL && E->dalsi->geo.typ==0 && E->geo.typ==0){d.v.smaz_element(E);break;}
				//pokud ne je přepnut na zarážku
				else
				{
			  	//vynulování WT
			  	E->WT=0;
			  	//změna na zarážku
			  	E->eID=MaxInt;
			  	//smazání a znovuvytvoření mGridu elementu
			  	if(pom_temp!=NULL && O->n==pom_temp->n)
			  	{
			  		ID=E->mGrid->ID;
			  		E->mGrid->Delete();
			  		E->mGrid=new TmGrid(F);
			  		E->mGrid->Tag=6;//ID formu
			  		E->mGrid->ID=ID;//ID tabulky tzn. i ID komponenty, musí být v rámci jednoho formu/resp. objektu unikátní, tzn. použijeme n resp. ID elementu
			  		design_element(E,false);//nutné!
					}
				}
			}
			//ukazatelové záležitosti
			O_pom=NULL;delete O_pom;
			E=E->dalsi;
		}
		//ukazatelové záležitosti
		delete E;E=NULL;
		O=O->dalsi;
	}
	delete O;O=NULL;
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
	if(posledni_editovany_element!=NULL){orientace=posledni_editovany_element->orientace;}else {orientace=pom_temp->elementy->predchozi->orientace;}
	//////první vložení geometrie
	if(posledni_editovany_element==NULL)
	{
		//vložení nového úseku před první element
		if(posun_dalsich_elementu)
		{
    	posunx=d.geoTemp.X4-d.geoTemp.X1;posuny=d.geoTemp.Y4-d.geoTemp.Y1;
			posledni_editovany_element=d.v.vloz_element(pom_temp,MaxInt,d.geoTemp.X4,d.geoTemp.Y4,orientace,pom_temp->elementy->dalsi);
			design_element(posledni_editovany_element,true);//nutné!!!!!!!!
			posledni_editovany_element->geo=d.geoTemp;
		}
		//přepsání geometrie prvnímu elementu v kabině
		else
		{
	  	posunx=d.geoTemp.X4-pom_temp->elementy->dalsi->geo.X4;posuny=d.geoTemp.Y4-pom_temp->elementy->dalsi->geo.Y4;
	  	pom_temp->elementy->dalsi->geo=d.geoTemp;
	  	pom_temp->elementy->dalsi->X+=posunx;pom_temp->elementy->dalsi->Y+=posuny;
	  	posledni_editovany_element=pom_temp->elementy->dalsi;
		}
		//posun následujících elementů, pokud existují
		Cvektory::TElement *E=posledni_editovany_element->dalsi;
		while(E!=NULL)
		{
			E->X+=posunx;E->Y+=posuny;//souřadnice elementu
			//geometrie elementu
			E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
			E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
			E=E->dalsi;
		}
		delete E;E=NULL;
	}
	//////editace pohonu ve středu
	else if(posledni_editovany_element!=NULL && posledni_editovany_element->dalsi!=NULL)
	{
		//vložení úseku
		if(posun_dalsich_elementu)
		{
			posunx=d.geoTemp.X4-d.geoTemp.X1;posuny=d.geoTemp.Y4-d.geoTemp.Y1;
			posledni_editovany_element=d.v.vloz_element(pom_temp,MaxInt,d.geoTemp.X4,d.geoTemp.Y4,orientace,posledni_editovany_element->dalsi);
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
		//posun ostatních úseků geometrie
		Cvektory::TElement *E=posledni_editovany_element->dalsi;
		while(E!=NULL)
		{
			E->X+=posunx;E->Y+=posuny;//souřadnice elementu
			//geometrie elementu
			E->geo.X1+=posunx;E->geo.X2+=posunx;E->geo.X3+=posunx;E->geo.X4+=posunx;
			E->geo.Y1+=posuny;E->geo.Y2+=posuny;E->geo.Y3+=posuny;E->geo.Y4+=posuny;
			E=E->dalsi;
		}
		delete E;E=NULL;
	}
	//////přidávání za poslední geometrii
	else if(posledni_editovany_element!=NULL && posledni_editovany_element->dalsi==NULL)
	{
		if(posledni_editovany_element->geo.delka!=0)//normální provoz
		{
			Cvektory::TElement *E=d.v.vloz_element(pom_temp,MaxInt,posledni_editovany_element->geo.X4,posledni_editovany_element->geo.Y4,orientace,posledni_editovany_element);
			E->geo=posledni_editovany_element->geo;
			design_element(E,true);//nutné!!!!!!!!
			posledni_editovany_element->geo=d.geoTemp;
			posledni_editovany_element->X+=posledni_editovany_element->geo.X4-posledni_editovany_element->geo.X1;
			posledni_editovany_element->Y+=posledni_editovany_element->geo.Y4-posledni_editovany_element->geo.Y1;
			posledni_editovany_element->orientace=m.Rt90(posledni_editovany_element->geo.orientace-90);//změna orientace podle trendu, nefunguje pro oblouky
			E=NULL;delete E;
		}
		else//pokud je veškerá geometrie odstraněna z kabiny
		{
			posledni_editovany_element->geo=d.geoTemp;
			posledni_editovany_element->X=posledni_editovany_element->geo.X4;
			posledni_editovany_element->Y=posledni_editovany_element->geo.Y4;
		}
	}
	nahled_ulozit(true);
}
//---------------------------------------------------------------------------
//ukončení akce geometrie a případné uzavření kruhu
void TForm1::ukonceni_geometrie()
{
	log(__func__);//logování
	//////uzavření kruhu
	if(pom->dalsi==NULL && pom_temp->n!=1 && posledni_editovany_element!=NULL)
	{
		double delka=m.delka(posledni_editovany_element->X,posledni_editovany_element->Y,d.v.OBJEKTY->dalsi->elementy->dalsi->geo.X1,d.v.OBJEKTY->dalsi->elementy->dalsi->geo.Y1);
		if(delka <=2 && posledni_editovany_element->geo.typ==0 && (posledni_editovany_element->X==d.v.OBJEKTY->dalsi->elementy->dalsi->geo.X1 || posledni_editovany_element->Y==d.v.OBJEKTY->dalsi->elementy->dalsi->geo.Y1))
		{
			d.v.vloz_G_element(posledni_editovany_element,0,posledni_editovany_element->geo.X1,posledni_editovany_element->geo.Y1,0,0,0,0,d.v.OBJEKTY->dalsi->elementy->dalsi->geo.X1,d.v.OBJEKTY->dalsi->elementy->dalsi->geo.Y1,posledni_editovany_element->geo.orientace);
			posledni_editovany_element->X=posledni_editovany_element->geo.X4;
			posledni_editovany_element->Y=posledni_editovany_element->geo.Y4;
		}
	}
	//////vypnutí akce
	if(!editace_textu)Akce=NIC;Akce_temp=NIC;//musí být ještě před refresh
	REFRESH(false);
}
//---------------------------------------------------------------------------
//vrátí maximální možný počet vozíků na stopce, podle geometrie před ní
double TForm1::max_voziku(Cvektory::TElement *stopka)
{
	log(__func__);//logování
	double delka=stopka->geo.delka;
	Cvektory::TElement *E=stopka->predchozi;
	while(E!=NULL && E->n!=0)
	{
		if(E->OTOC_delka>0)
		{
			delka-=E->OTOC_delka/2.0;//odsazení před otočí
			double rotace_jigu=d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(E);
			if(rotace_jigu==0 || rotace_jigu==180)delka-=d.v.PP.delka_jig/2.0;else delka-=d.v.PP.sirka_jig/2.0;
		}
		if(E->geo.typ==0 && E->eID==MaxInt)delka+=E->geo.delka;
		else break;
		E=E->predchozi;
	}
	E=NULL;delete E;
	if(delka>0)return floor((delka-d.v.PP.uchyt_pozice)/d.v.PP.delka_podvozek);
	else return 1;
}
//---------------------------------------------------------------------------
//vrací zda se element nachází na pohonu jakéhokoliv sklonu, dvojí způsob použití: zadání parametru X,Y,eID pokud nemám ukazatel na element nebo 0,0,0,ukazatel, vrátí true nebo false
bool TForm1::el_vkabine(double X,double Y,int element_id,short orientace_el,Cvektory::TElement *E,double tolerance_x,double tolerance_y)
{
	log(__func__);//logování
	//pomocná proměnná, pokud není zadán element metoda si vytvoří vlastní pomocný, který musí být smazán
	bool mazat_el=false;
	bool ret=false;
	//vytvoření oblasti pohonu
	double x1,y1,x2,y2;
	x1=pom_temp->elementy->dalsi->geo.X1;y1=pom_temp->elementy->dalsi->geo.Y1;
	x2=pom_temp->elementy->predchozi->geo.X4;y2=pom_temp->elementy->predchozi->geo.Y4;
	//vytvoření pomocného elementu (pokud neexistuje), souřadnice a eID totožné s vkládáným (vytvořen z důvodu použítí makra d.Rxy)
	if(E==NULL)
	{
		E=new Cvektory::TElement;
		E->X=m.P2Lx(X);E->Y=m.P2Ly(Y);E->eID=element_id;E->orientace=orientace_el;
		E->LO1=1.5;E->LO_pozice=0;
		if(element_id==1 || element_id==3 || element_id==9 || element_id==13 || element_id==17 || element_id==103 || element_id==107){E->LO2=1.5;E->OTOC_delka=0.450;}
		mazat_el=true;
	}
	TRect el=souradnice_LO(E);
	//kontrola zda je element vkládán na pohon
	switch((int)pom_temp->orientace)
	{
		case 0:case 180:if(m.LeziVblizkostiUsecky(d.Rxy(E).x+tolerance_x,m.P2Ly(el.top)+tolerance_y,x1,y1,x2,y2)==0 && m.LeziVblizkostiUsecky(d.Rxy(E).x+tolerance_x,m.P2Ly(el.bottom)+tolerance_y,x1,y1,x2,y2)==0)ret=true;break;
		case 90:case 270:if(m.LeziVblizkostiUsecky(m.P2Lx(el.left)+tolerance_x,d.Rxy(E).y+tolerance_y,x1,y1,x2,y2)==0 && m.LeziVblizkostiUsecky(m.P2Lx(el.right)+tolerance_x,d.Rxy(E).y+tolerance_y,x1,y1,x2,y2)==0)ret=true;break;
	}
	return ret;
	//smazání pomocného elementu
	if(mazat_el){delete E;E=NULL;}
}
//---------------------------------------------------------------------------
//vrací max a min hodnoty x a y souřadnic, všecho v layout(elementů, objektů), nebo parametrem Objekt lze hledat max souřadnice v jednom objektu
TRect TForm1::vrat_max_oblast(Cvektory::TObjekt *Objekt)
{
	log(__func__);//logování
	TRect ret;
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
				if(m.L2Px(O->Xt)<ret.left)ret.left=m.L2Px(O->Xt);
				if(m.L2Px(O->Xt)>ret.right)ret.right=m.L2Px(O->Xt);
				if(m.L2Py(O->Yt)<ret.top)ret.top=m.L2Py(O->Yt);
				if(m.L2Py(O->Yt)>ret.bottom)ret.bottom=m.L2Py(O->Yt);
			}
			O=O->dalsi;
		}
		delete O,O=NULL;
	}
	return ret;
}
//---------------------------------------------------------------------------
//vrací souřadnice (PX) lakovacího okna elementu pokud nějaké má,pokud ne vrátí souřadnice elementu
TRect TForm1::souradnice_LO(Cvektory::TElement *E)
{
	log(__func__);//logování
	double x1,x2,y1,y2; TRect ret;
	//pouze pro kontinuální
	if(E->eID==1 || E->eID==3 || E->eID==7 || E->eID==9 || E->eID==11 || E->eID==13 || E->eID==15 || E->eID==17 || E->eID==101 || E->eID==103 || E->eID==105 || E->eID==107)
	{
  	switch((int)pom_temp->orientace)
  	{
  		case 0:
  		{
  			ret.left=ret.right=m.L2Px(E->X-2);
  			if(E->LO2>0)ret.top=m.L2Py(E->Y+E->LO2+(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.top=m.L2Py(E->Y+(E->LO1+E->LO2)/2.0+E->LO_pozice);
  			if(E->LO2>0)ret.bottom=m.L2Py(E->Y-E->LO1-(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.bottom=m.L2Py(E->Y-(E->LO1)/2.0+E->LO_pozice);
  		}break;
  		case 90:
  		{
  			ret.top=ret.bottom=m.L2Py(E->Y+2);
  			if(E->LO2>0)ret.right=m.L2Px(E->X+E->LO2+(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.right=m.L2Px(E->X+(E->LO1+E->LO2)/2.0+E->LO_pozice);
  			if(E->LO2>0)ret.left=m.L2Px(E->X-E->LO1-(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.left=m.L2Px(E->X-(E->LO1)/2.0+E->LO_pozice);
  		}break;
  		case 180:
  		{
  			ret.left=ret.right=m.L2Px(E->X+2);
  			if(E->LO2>0)ret.bottom=m.L2Py(E->Y-E->LO2-(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.bottom=m.L2Py(E->Y-(E->LO1+E->LO2)/2.0+E->LO_pozice);
  			if(E->LO2>0)ret.top=m.L2Py(E->Y+E->LO1+(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.top=m.L2Py(E->Y+(E->LO1)/2.0+E->LO_pozice);
  		}break;
  		case 270:
  		{
  			ret.top=ret.bottom=m.L2Py(E->Y+2);
				if(E->LO2>0)ret.left=m.L2Px(E->X-E->LO2-(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.left=m.L2Px(E->X-(E->LO1+E->LO2)/2.0+E->LO_pozice);
  			if(E->LO2>0)ret.right=m.L2Px(E->X+E->LO1+(E->OTOC_delka)/2.0+E->LO_pozice);
  			else ret.right=m.L2Px(E->X+(E->LO1)/2.0+E->LO_pozice);
  		}break;
  	}
	}
	else//element bez lakovacího okna
	{
		ret.left=ret.right=m.L2Px(E->X);
		ret.top=ret.bottom=m.L2Py(E->Y);
  }
	return ret;
}
//---------------------------------------------------------------------------
//prozkoumá zda se element nepřekrýva lak. oknem se sousedními
bool TForm1::prekryti_LO(Cvektory::TElement *E)
{
	log(__func__);//logování
	bool prekryti=false;
	if(pom_temp!=NULL && E!=NULL && E->eID%2!=0 && E->eID!=5 && E->eID!=MaxInt)
	{
		TRect el1=souradnice_LO(E),el2;
		//kontrola konfliktu s dalším elementem
		if(E->dalsi!=NULL && E->dalsi->eID%2!=0 && E->dalsi->eID!=5 && E->dalsi->eID!=MaxInt)
		{
			el2=souradnice_LO(E->dalsi);
			switch((int)pom_temp->orientace)
			{
				case 0:if(el1.top<el2.bottom)prekryti=true;break;
				case 90:if(el1.right>el2.left)prekryti=true;break;
				case 180:if(el1.bottom>el2.top)prekryti=true;break;
				case 270:if(el1.left<el2.right)prekryti=true;break;
			}
		}
		//kontrola konfliktu s přechozím elementem
		if(E->predchozi!=NULL && E->n!=1 && E->predchozi->eID%2!=0 && E->predchozi->eID!=5 && E->predchozi->eID!=MaxInt)
		{
			el2=souradnice_LO(E->predchozi);
			switch((int)pom_temp->orientace)
			{
				case 0:if(el1.bottom>el2.top)prekryti=true;break;
				case 90:if(el1.left<el2.right)prekryti=true;break;
				case 180:if(el1.top<el2.bottom)prekryti=true;break;
				case 270:if(el1.right>el2.left)prekryti=true;break;
			}
		}
		//kontrola konflikru s prvním a posledním bodem objektu, pokud už nebylo odhaleno překrytí
		if(!prekryti)
		{
			switch((int)pom_temp->orientace)
			{
				case 0:if(el1.bottom>m.L2Py(pom_temp->elementy->dalsi->geo.Y1) || m.L2Py(pom_temp->elementy->predchozi->geo.Y4)>el1.top)prekryti=true;break;
				case 90:if(el1.left<m.L2Px(pom_temp->elementy->dalsi->geo.X1) || m.L2Px(pom_temp->elementy->predchozi->geo.X4)<el1.right)prekryti=true;break;
				case 180:if(el1.top<m.L2Py(pom_temp->elementy->dalsi->geo.Y1) || m.L2Py(pom_temp->elementy->predchozi->geo.Y4)<el1.bottom)prekryti=true;break;
				case 270:if(el1.right>m.L2Px(pom_temp->elementy->dalsi->geo.X1) || m.L2Px(pom_temp->elementy->predchozi->geo.X4)>el1.left)prekryti=true;break;
			}
    }
	}
	return prekryti;
}
//---------------------------------------------------------------------------
//vrati delku v metrech mezi LO elementů
double TForm1::vzdalenost_meziLO(Cvektory::TElement *E,double orientace)
{
	double ret=0;
	TRect souradnice_2=souradnice_LO(E),souradnice_1=souradnice_LO(E->predchozi);//došlo ke kliku na kótu ... předchozí element existuje
	switch((int)orientace)
	{
		case 0:ret=m.px2m(souradnice_1.top-souradnice_2.bottom);break;
		case 90:ret=m.px2m(souradnice_2.left-souradnice_1.right);break;
		case 180:ret=m.px2m(souradnice_2.top-souradnice_1.bottom);break;
		case 270:ret=m.px2m(souradnice_1.left-souradnice_2.right);break;
	}
	return ret;
}
//---------------------------------------------------------------------------
//vratí zda jsem v oblasti textu, typ 0 citelná oblast textu, typ 1 citelná oblast posun. kříže
bool TForm1::najdi_nazev_obj(double X,double Y,Cvektory::TObjekt *Objekt,int typ)
{
	log(__func__);//logování
	bool ret=false;
	//nastavení canvasu
	d.nastavit_text_popisu_objektu_v_nahledu(Canvas,1);AnsiString Tn=F->pom_temp->name.UpperCase();short Wn=Canvas->TextWidth(Tn);//název objektu - nastavení
	if(typ==0)//hledání textu
	{
		switch((int)Objekt->orientace)
		{
			case 0:if(m.L2Px(pom_temp->Xt)>X && X>m.L2Px(pom_temp->Xt)-Canvas->TextHeight(Tn) && m.L2Py(pom_temp->Yt)-m.round(Wn)/2.0<Y && Y<m.L2Py(pom_temp->Yt)+m.round(Wn)/2.0)ret=true;break;
			case 90:case 270:if(m.L2Px(pom_temp->Xt)-m.round(Wn)/2.0<=X && X<=m.L2Px(pom_temp->Xt)+m.round(Wn/2.0) && m.L2Py(pom_temp->Yt)>=Y && Y>=m.L2Py(pom_temp->Yt)-Canvas->TextHeight(Tn))ret=true;break;
			case 180:if(m.L2Px(pom_temp->Xt)<X && X<m.L2Px(pom_temp->Xt)+Canvas->TextHeight(Tn) && m.L2Py(pom_temp->Yt)-m.round(Wn)/2.0<Y && Y<m.L2Py(pom_temp->Yt)+m.round(Wn)/2.0)ret=true;break;
		}
	}
	else// hledání posunovacího kříže
	{
		double x1,x2,y1,y2;
		switch((int)Objekt->orientace)
		{
			case 0:
			{
				x1=m.L2Px(pom_temp->Xt)-Canvas->TextHeight(Tn)-3;y1=m.L2Py(pom_temp->Yt)+m.round(Wn/2.0)-2;
				x2=m.L2Px(pom_temp->Xt)-Canvas->TextHeight(Tn)+16;y2=m.L2Py(pom_temp->Yt)+m.round(Wn/2.0)+20;
			}break;
			case 90:case 270:
			{
				x1=m.L2Px(pom_temp->Xt)-m.round(Wn/2.0)+2;y1=m.L2Py(pom_temp->Yt)-Canvas->TextHeight(Tn)-3;
				x2=m.L2Px(pom_temp->Xt)-m.round(Wn/2.0)-20;y2=m.L2Py(pom_temp->Yt)-Canvas->TextHeight(Tn)+16;
			}break;
			case 180:
			{
				x1=m.L2Px(pom_temp->Xt)+Canvas->TextHeight(Tn)+3;y1=m.L2Py(pom_temp->Yt)-m.round(Wn/2.0)+2;
				x2=m.L2Px(pom_temp->Xt)+Canvas->TextHeight(Tn)-16;y2=m.L2Py(pom_temp->Yt)-m.round(Wn/2.0)-20;
			}break;
		}
		if(m.PtInRectangle(x1,y1,x2,y2,X,Y))ret=true;
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
	if(Akce==NIC && !editace_textu)
	{
		double x1,y1,x2,y2;//pozice popisků u elementu
		Canvas->Font->Size=F->m.round(2.8*Zoom);if(F->aFont->Size==12)Canvas->Font->Size=m.round(2*Zoom);
		int w1,w2;
		int h;
		short K=0.25*Zoom;
		element_temp=NULL;
		//průchod objektu
  	Cvektory::TElement *E=Objekt->elementy->dalsi;
  	while(E!=NULL)
  	{
			if(E->eID==200)
			{
				AnsiString T1="pohon nevybrán",T2="pohon nevybrán";
	    	if(E->pohon!=NULL)T1=E->pohon->name;
	    	if(E->dalsi!=NULL && E->dalsi->pohon!=NULL)T2=E->dalsi->pohon->name;
	    	if(E->dalsi==NULL)
	    	{
					Cvektory::TObjekt *O=d.v.vrat_objekt(E->objekt_n);
	    		if(O->dalsi!=NULL && O->dalsi->elementy->dalsi->pohon!=NULL)T2=O->dalsi->elementy->dalsi->pohon->name;
					if(F->pom_temp!=NULL && O->dalsi->n==F->pom_temp->n && F->pom_temp->elementy->dalsi->pohon!=NULL)T2=F->pom_temp->elementy->dalsi->pohon->name;
					O=NULL;delete O;
				}
				if(pom_temp->orientace==270){AnsiString pom=T1;T1=T2;T2=T1;}//prohození popisků
				w1=Canvas->TextWidth(T1);w2=Canvas->TextWidth(T2);h=Canvas->TextHeight(T1);
				//definice výchozích bodů popisků + zjištění oblasti
				switch((int)E->orientace)
				{
					case 0:case 180:
					{
						x1=m.L2Px(E->X)-w1-m.m2px(0.2);y1=m.L2Py(E->Y)+K;
						x2=m.L2Px(E->X)+m.m2px(0.2);y2=m.L2Py(E->Y)-K-h;
						if(m.PtInRectangle(x1,y1,x1+w1,y1+h,X,Y)){ret=1;if(pom_temp->orientace==270)ret=2;element_temp=E;break;}
						if(m.PtInRectangle(x2,y2,x2+w2,y2+h,X,Y)){ret=2;if(pom_temp->orientace==270)ret=1;element_temp=E;break;}
					}break;
					case 90:
					{
						x1=m.L2Px(E->X)-K;y1=m.L2Py(E->Y)-w1-m.m2px(0.2);
						x2=m.L2Px(E->X)+K+h;y2=m.L2Py(E->Y)+m.m2px(0.2);
						if(m.PtInRectangle(x1,y1,x1-h,y1+w1,X,Y)){ret=1;element_temp=E;break;}
						if(m.PtInRectangle(x2,y2,x2-h,y2+w2,X,Y)){ret=2;element_temp=E;break;}
					}break;
		  		case 270:
					{
						x1=m.L2Px(E->X)-K;y1=m.L2Py(E->Y)+w1+m.m2px(0.2);
						x2=m.L2Px(E->X)-h-K;y2=m.L2Py(E->Y)-m.m2px(0.2);
						if(m.PtInRectangle(x1,y1,x1+h,y1-w1,X,Y)){ret=1;element_temp=E;break;}
						if(m.PtInRectangle(x2,y2,x2+h,y2-w2,X,Y)){ret=2;element_temp=E;break;}
					}break;
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
TPoint TForm1::bod_vlozeni_elementu(double kontr_x,double kontr_y)
{
	log(__func__);//logování
	//nastavení atributů
	pom_element_temp=NULL;
	TPoint ret;
	ret.x=-1000;ret.y=-1000;
	double oblast=1,X1,Y1,X2,Y2,x,y;
	if(kontr_x!=-1000){x=kontr_x;y=kontr_y;}
	else {x=akt_souradnice_kurzoru.x;y=akt_souradnice_kurzoru.y;}
  //průchod elementů objektu
	Cvektory::TElement *E=pom_temp->elementy->dalsi;
	while(E!=NULL)
	{
		if(E->geo.typ==0 && (E->geo.orientace==m.Rt90(E->geo.orientace) || E->geo.orientace==360))//jen pro přímky 0,90,180,270°
		{
			//nastavení oblasti
			if(E->geo.orientace==90 || E->geo.orientace==270){X1=E->geo.X1;X2=E->geo.X4;Y1=E->geo.Y1+oblast;Y2=E->geo.Y4-oblast;}
			else {X1=E->geo.X1+oblast;X2=E->geo.X4-oblast;Y1=E->geo.Y1;Y2=E->geo.Y4;}
			//hledání zda se kurzor nachází v této oblasti
			if(m.PtInRectangle(X1,Y1,X2,Y2,x,y))
			{
				//přiřazení souřadnic pro vložení
				if(E->geo.orientace==90 || E->geo.orientace==270){ret.x=akt_souradnice_kurzoru_PX.x;ret.y=m.L2Py(E->geo.Y1);}
				else {ret.x=m.L2Px(E->geo.X1);ret.y=akt_souradnice_kurzoru_PX.y;}
        pom_element_temp=E;//uchování ukazatele pro následné zjišťování orientce symbolu, nelze se již řídit orientaci kabiny
				break;//zastavení cyklu
			}
		}
		E=E->dalsi;
	}
	//mazání ukazatele
	E=NULL;delete E;
	//vracení souřadnic elementu v logických
	return ret;
}
//---------------------------------------------------------------------------
//upraví bod kurzoru pro vložení elemntu na bod vykreslení elementu (robot na konci ramena)
TPoint TForm1::uprav_bod_vlozeni_elementu(TPoint bod_vlozeni,short rotace_symbolu,int eID)
{
	log(__func__);//logování
	//nastavení atribuutů
	double DoSkRB=0;
	double orientace=pom_temp->orientace;
	if(pom_element_temp!=NULL)orientace=pom_element_temp->geo.orientace;//pokud je element vkládán na přímku, ukazatel na element, který uchovává tuto přímku
	if(eID<0)eID=element_id;
	//zohlednění posunu bodu vložení u robotů
	if(1<=eID && eID<=4 || 7<=eID && eID<=18)//pro roboty, které mají uchopovací bod jinde než referenční
	{
		DoSkRB=m.m2px(d.DoSkRB);//délka od středu (uchopovacího bodu) k referenčnímu bodu, doplnit konstanty
		if(rotace_symbolu==90 || rotace_symbolu==180)DoSkRB*=-1;
	}
	if(101<=eID && eID<=108)//pro lidské roboty, které mají uchopovací bod jinde než referenční
	{
		DoSkRB=m.m2px(d.DkRB);//délka od středu (uchopovacího bodu) k referenčnímu bodu, doplnit konstanty
		if(rotace_symbolu==90 || rotace_symbolu==180)DoSkRB*=-1;
	}
	//úprava souřadnic = přichycení na pohon

	if(orientace==90 || orientace==270)bod_vlozeni.y+=DoSkRB;
	else bod_vlozeni.x+=DoSkRB;
	//vrácení upravených souřadnic
	return bod_vlozeni;
}
//---------------------------------------------------------------------------
//kontroluje zde se bod nachází na geometri, vrací pouze ano/ne, pokud je do metody poslán ukazatel na element prověří zda se tento element nachází na geometrii
bool TForm1::bod_na_geometrii(double X, double Y,Cvektory::TElement *Element)
{
	bool ret=false;
	if(Element!=NULL){X=d.Rxy(Element).x;Y=d.Rxy(Element).y;}
	Cvektory::TElement *E=pom_temp->elementy->dalsi;
	while(E!=NULL)
	{
		if(E->geo.typ==0 && E->geo.orientace==m.Rt90(E->geo.orientace) && m.LeziVblizkostiUsecky(X,Y,E->geo.X1,E->geo.Y1,E->geo.X4,E->geo.Y4)==0){ret=true;break;}
		E=E->dalsi;
	}
	E=NULL;delete E;
	return ret;
}
//---------------------------------------------------------------------------
//metoda pro sledování zda je nutné náhled uložit
void TForm1::nahled_ulozit (bool duvod_ulozit)
{
	log(__func__);//logování
	//aby mohlo být tlačítko aktivované musí k tomu vzniknout důvod (přidání robota, editace hodnot, ...), ale zároveň nesmí být chybná hodnota rychlosti (validace)
	if(duvod_ulozit&&PmG->Note.Text=="") {scGPButton_ulozit->Enabled=true;duvod_ulozit_nahled=true;}
	if(!duvod_ulozit) {scGPButton_ulozit->Enabled=false;duvod_ulozit_nahled=false;}

}
//---------------------------------------------------------------------------
//automatické nekonfliktní pozicování tabulek podle tabulek ostatních elementů
void TForm1::aut_pozicovani(Cvektory::TElement *E, int X, int Y)
{
	log(__func__);//logování
	short O=10*3;//hodnota odsazení
	double x=0,x1=0,y=0,y1=0;//původní a překlopené souřadnice tabulky
	bool hor=false,ver=false;
	int prekryti=0;//počet překrytí
	switch(E->orientace)
	{
		case 0:
		hor=true;//jedná se o horizontálně orientovanou kabinu
		x=X-E->mGrid->Width/2.0;//neměnná souřadnice
		if (1<=element_id && element_id<=4 || 7<=element_id && element_id<=18 || 101<=element_id && element_id<=108)
		{ //roboti
    	y=Y-E->mGrid->Height-d.DoSkRB*Zoom/m2px-O;
			y1=Y+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;
		}
		else//stop a otoče
		{
			y=Y-O-E->mGrid->Height;//původní hodnota
			y1=Y+d.DoSkRB*Zoom/m2px+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;//-E->mGrid->Height; //překlopená hodnota
		}
		break;
		case 90:
		ver=true;//jedná se o vertikálně orientovanou kabinu
		y=Y-E->mGrid->Height/2.0;
		if (1<=element_id && element_id<=4 || 7<=element_id && element_id<=18 || 101<=element_id && element_id<=108)
		{
			x=X+d.DoSkRB*Zoom/m2px+O;
			x1=X-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Width;
		}
		else
		{
    	x=X+O;
			x1=X-d.DoSkRB*Zoom/m2px-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Width;
		}
		break;//pozor invertované DoSkRB
		case 180:
		hor=true;
		x=X-E->mGrid->Width/2.0;
		if (1<=element_id && element_id<=4 || 7<=element_id && element_id<=18 || 101<=element_id && element_id<=108)
		{
			y=Y+d.DoSkRB*Zoom/m2px+O;
			y1=Y-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Height;
		}
		else
		{
    	y=Y+O;
			y1=Y-d.DoSkRB*Zoom/m2px-d.Robot_sirka_zakladny/2.0*Zoom/m2px-O-E->mGrid->Height;
		}
		break;//pozor invertované DoSkRB
		case 270:
		ver=true;
		y=Y-E->mGrid->Height/2.0;
		if (1<=element_id && element_id<=4 || 7<=element_id && element_id<=18 || 101<=element_id && element_id<=108)
		{
			x=X-d.DoSkRB*Zoom/m2px-E->mGrid->Width-O;
			x1=X+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;
		}
		else
		{
    	x=X-O-E->mGrid->Width;
			x1=X+d.DoSkRB*Zoom/m2px+d.Robot_sirka_zakladny/2.0*Zoom/m2px+O;
		}
		break;
	}
	//Pro horizontální překlápění
	if (E->predchozi->n>=1 && hor)//Kontrola zda je už vložený nějaký element
	{
		Cvektory::TElement *O=E->predchozi;
		while(O->n >= 1)//kontrola překrytí napříč všemi dosut přidanými elementy
		{
			if(O->eID!=100)//ošetření z důvodu, že některý element nemá mGrid
			{
	   		if ((x<=m.L2Px(O->Xt)+O->mGrid->Width && x>=m.L2Px(O->Xt)) ||//Překrývají se v X souřadnici
	   		(x+E->mGrid->Width<=m.L2Px(O->Xt)+O->mGrid->Width&&x+E->mGrid->Width>=m.L2Px(O->Xt)))
	   		{
	   			if ((y>=m.L2Py(O->Yt)&&y<=m.L2Py(O->Yt)+O->mGrid->Height) || (y+E->mGrid->Height>=m.L2Py(O->Yt)&&y+E->mGrid->Height<=m.L2Py(O->Yt)+O->mGrid->Height))
					{
	   				y=y1;
	   				Cvektory::TElement *O1=E->predchozi;
	   				while(O1->n>= 1)
	   				{
	   					if ((x<=m.L2Px(O1->Xt)+O->mGrid->Width && x>=m.L2Px(O1->Xt))||(x+E->mGrid->Width<=m.L2Px(O1->Xt)+O1->mGrid->Width&&x+E->mGrid->Width>=m.L2Px(O1->Xt)))
	   					if ((y>=m.L2Py(O1->Yt)&&y<=m.L2Py(O1->Yt)+O1->mGrid->Height) || (y+E->mGrid->Height>=m.L2Py(O1->Yt)&&y+E->mGrid->Height<=m.L2Py(O1->Yt)+O1->mGrid->Height))
	   					{
	   						x=X;
	   						y=Y;
	   						break;
	   					}
	   					O1=O1->predchozi;
	   				}
	   				O1=NULL;delete O1;
	   			}
				}
			}
			O=O->predchozi;
		}
		O=NULL;delete O;
	}
	//Pro vertikální překlápění
	if (E->predchozi->n>=1 && ver)//Kontrola zda je už vložený nějaký element
	{
		Cvektory::TElement *O=E->predchozi;
		while(O->n >= 1)//kontrola překrytí napříč všemi dosut přidanými elementy
		{
			if(O->eID!=100)//ošetření z důvodu, že některý element nemá mGrid
			{
		  	if ((y<=m.L2Py(O->Yt)+O->mGrid->Height && y>=m.L2Py(O->Yt)) ||//Překrývají se v Y souřadnici
		  	(y+E->mGrid->Height<=m.L2Py(O->Yt)+O->mGrid->Height&&y+E->mGrid->Height>=m.L2Py(O->Yt)))
		  	{
		  		if ((x>=m.L2Px(O->Xt)&&x<=m.L2Px(O->Xt)+O->mGrid->Width) || (x+E->mGrid->Width>=m.L2Px(O->Xt)&&x+E->mGrid->Width<=m.L2Px(O->Xt)+O->mGrid->Width))
		  		{
		  			x=x1;
		  			Cvektory::TElement *O1=E->predchozi;
		  			while(O1->n>= 1)
		  			{
		  				if ((y<=m.L2Py(O1->Yt)+O->mGrid->Height && y>=m.L2Py(O1->Yt)) || (y+E->mGrid->Height<=m.L2Py(O1->Yt)+O1->mGrid->Height&&y+E->mGrid->Height>=m.L2Py(O1->Yt)))
		  				if ((x>=m.L2Px(O1->Xt)&&x<=m.L2Px(O1->Xt)+O1->mGrid->Width) || (x+E->mGrid->Width>=m.L2Px(O1->Xt)&&x+E->mGrid->Width<=m.L2Px(O1->Xt)+O1->mGrid->Width))
		  				{
		  					x=X;
		  					y=Y;
		  					break;
		  				}
		  				O1=O1->predchozi;
		  			}
		  			O1=NULL;delete O1;
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
	mGrid_knihovna->Create(2,18);//vytvoření celé tabulky najednou
	mGrid_knihovna->Left=-1;
	mGrid_knihovna->Top=-1;
	mGrid_knihovna->SetColumnAutoFit(-4);
	mGrid_knihovna->Columns[0].Width=mGrid_knihovna->Columns[1].Width=84;//polovina šířky left toolbaru
	/////////nadpisy
	mGrid_knihovna->Cells[0][0].Text="Hala";
	mGrid_knihovna->Cells[0][3].Text="Nav/svěšování";
	mGrid_knihovna->Cells[0][5].Text="Předúpravy";
	mGrid_knihovna->Cells[0][8].Text="Lakovna";
	mGrid_knihovna->Cells[0][10].Text="Ostatní";
	/////////úprava výšky řádků
	for(int i=0;i<=mGrid_knihovna->RowCount-1;i++)
	{
		if(i!=0 && i!=1 && i!=2 && i!=3 && i!=5 && i!=8 && i!=10)mGrid_knihovna->Rows[i].Height=59;
		else
		{
			mGrid_knihovna->Cells[0][i].Align=mGrid_knihovna->Talign::LEFT;
			//mGrid_knihovna->Cells[0][i].Background->Color=clRed;//clBtnFace;
			mGrid_knihovna->Rows[i].Height=32;
			mGrid_knihovna->Cells[0][i].LeftMargin=10;
			mGrid_knihovna->Cells[0][i].Font=scGPLabel_roboti->Font;
		}
	}
	mGrid_knihovna->Rows[17].Height=scSplitView_LEFTTOOLBAR->Height-(mGrid_knihovna->Top+mGrid_knihovna->Height);//přesah mimo obrazovku, nezobrazování spodní hranice tabulky
	/////////sloučení nadpisů
	mGrid_knihovna->MergeCells(0,0,1,0);
	mGrid_knihovna->MergeCells(0,3,1,3);
	mGrid_knihovna->MergeCells(0,5,1,5);
	mGrid_knihovna->MergeCells(0,8,1,8);
	mGrid_knihovna->MergeCells(0,10,1,10);
	/////////hala
	mGrid_knihovna->Cells[0][1].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][1].ImageIndex=15;
	mGrid_knihovna->Cells[0][2].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][2].ImageIndex=16;
	/////////objekty
	//navěšování/svěšování
	mGrid_knihovna->Cells[0][4].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][4].ImageIndex=0;
	mGrid_knihovna->Cells[0][4].Align=mGrid_knihovna->CENTER;
	mGrid_knihovna->Cells[1][4].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][4].ImageIndex=9;
	mGrid_knihovna->Cells[1][4].Align=mGrid_knihovna->CENTER;
	mGrid_knihovna->Rows[4].Height=36;
	//předúpravy
	mGrid_knihovna->Cells[0][6].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][6].ImageIndex=1;
	mGrid_knihovna->Cells[1][6].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][6].ImageIndex=4;
	mGrid_knihovna->Cells[0][7].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][7].ImageIndex=2;
	mGrid_knihovna->Cells[1][7].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][7].ImageIndex=3;
	//lakování
	mGrid_knihovna->Cells[0][9].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][9].ImageIndex=5;
	//postprocesní
	mGrid_knihovna->Cells[0][11].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][11].ImageIndex=6;
	mGrid_knihovna->Cells[0][11].Align=mGrid_knihovna->LEFT;
	mGrid_knihovna->Rows[11].Height=39;
	mGrid_knihovna->Cells[0][12].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][12].ImageIndex=7;
	mGrid_knihovna->Cells[0][12].Align=mGrid_knihovna->LEFT;
	mGrid_knihovna->Rows[12].Height=37;
	mGrid_knihovna->Cells[0][13].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][13].ImageIndex=8;
	mGrid_knihovna->Cells[0][13].Align=mGrid_knihovna->LEFT;
	mGrid_knihovna->Rows[13].Height=61;
	//ostatní
	mGrid_knihovna->Cells[0][14].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][14].ImageIndex=10;
	mGrid_knihovna->Cells[1][14].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][14].ImageIndex=12;
	mGrid_knihovna->Cells[0][15].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][15].ImageIndex=11;
	mGrid_knihovna->Cells[1][15].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[1][15].ImageIndex=14;
	mGrid_knihovna->Cells[0][16].Type=mGrid_knihovna->IMAGE;
	mGrid_knihovna->Cells[0][16].ImageIndex=13;
	/////////centrování komponent
	mGrid_knihovna->Update();
	TscGPImage *I=NULL;
	for (int i=1;i<=mGrid_knihovna->RowCount-1; i++)
	{
		//pro první sloupec
		if(mGrid_knihovna->Cells[0][i].Type==mGrid_knihovna->IMAGE)I=mGrid_knihovna->getImage(0,i);
		if(I!=NULL)
		{
			mGrid_knihovna->Cells[0][i].Valign=mGrid_knihovna->vNO;
			I->Top+=34;
			I=NULL;//nulování pro další sloupec
		}
		//pro druhý sloupec
		if(mGrid_knihovna->Cells[1][i].Type==mGrid_knihovna->IMAGE)I=mGrid_knihovna->getImage(1,i);
		if(I!=NULL)
		{
			mGrid_knihovna->Cells[1][i].Valign=mGrid_knihovna->vNO;
			I->Top+=34;
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
		scGPLabel_roboti->Caption="Hala";
		scGPLabel_otoce->Caption="Nav/Svěšování";scGPLabel_otoce->Top=95;
		scGPLabel_stop->Caption="Předúpravy";scGPLabel_stop->Top=163;
		scGPLabel_geometrie->Caption="Lakování";scGPLabel_geometrie->Top=314;
		scGPLabel_poznamky->Caption="Ostatní";scGPLabel_poznamky->Top=405;
  }
	else
	{
		scGPLabel_otoce->Caption="Otoče";scGPLabel_otoce->Top=224;
		scGPLabel_stop->Caption="Stop stanice";scGPLabel_stop->Top=315;
		scGPLabel_geometrie->Caption="Geometrie linky";scGPLabel_geometrie->Top=398;
		scGPLabel_poznamky->Caption="Poznámky";scGPLabel_poznamky->Top=506;
  }
}
//---------------------------------------------------------------------------
//klik do knihovny objektů, spouštění akce přidávání objektu
void TForm1::tab_knihovna_click(double X,double Y,long Col,long Row)
{
	log(__func__);//logování
	if(Col<0 || Row<0)
	{
		Col=mGrid_knihovna->GetIdxColumn(X,Y);
		Row=mGrid_knihovna->GetIdxRow(X,Y);
	}
	if(mGrid_knihovna->Cells[Col][Row].Type==mGrid_knihovna->IMAGE)
	{
		vybrany_objekt=mGrid_knihovna->Cells[Col][Row].ImageIndex;
		if(vybrany_objekt<15)//klik na objekty
		{
   		SB("Kliknutím na libovolné místo umístíte objekt "+knihovna_objektu[vybrany_objekt].name);
			Akce=ADD;kurzor(add_o);
   		add_posledni=true;pom=NULL;
   		if(VyID==vybrany_objekt && d.v.OBJEKTY->predchozi->n<3)//pokud je vybraná vyhýbka nejsou alespoň 3 objekty
   		{
   			MB("Výhybku lze nastavit, pokud jsou k dispozici minimálně 3 technologické objekty!");
   			Akce=NIC;kurzor(standard);
			}
		}
		else if(vybrany_objekt==15)//klik do sekce hala
		{
      scGPButton_nakreslit_haluClick(this);
    }
	}
}
//---------------------------------------------------------------------------
//vytvoří tabulku pohonu
void TForm1::vytvoreni_tab_pohon()
{
  log(__func__);//logování
	AnsiString aRD,R,Rz;
	//nastavení jednotek podle posledních nastavení
	if (aRDunit==SEC) aRD="<a>[m/s]</a>";
	else aRD="<a>[m/min]</a>";
	if (Runit==M) R="<a>[m]</a>";
	else R="<a>[mm]</a>";//1
	if (Rzunit==M) Rz="<a>[m]</a>";
	else Rz="<a>[mm]</a>";
	PmG=new TmGrid(this);//vždy nutno jako první
	//nastavení defaultního designu
	PmG->Left=-500;PmG->Top=-500;//pouze aby se při prvním zobrazení nezobrazovala formou probliku vlevo nahoře
	PmG->DefaultCell.Font->Name=aFont->Name;
	PmG->DefaultCell.Font->Size=aFont->Size;
	PmG->DefaultCell.isNegativeNumber->Name=aFont->Name;
	PmG->DefaultCell.isNegativeNumber->Size=aFont->Size;
	PmG->DefaultCell.isZero->Name=aFont->Name;
	PmG->DefaultCell.isZero->Size=aFont->Size;
	PmG->DefaultCell.isLink->Name=aFont->Name;
	PmG->DefaultCell.isLink->Size=aFont->Size;
	PmG->DefaultCell.isActiveLink->Name=aFont->Name;
	PmG->DefaultCell.isActiveLink->Size=aFont->Size;
	PmG->Note.Font->Name=aFont->Name;
	PmG->Note.Font->Size=14;
	PmG->AntiAliasing_text=true;
	PmG->Border.Width=2;
	PmG->ID=9999;
	PmG->Tag=6;//ID formu
	//vytvoření konkrétní tabulky podle režimu kabiny
	PmG->Create(2,8);//vytvoření celé tabulky najednou
	//formátování hlavičky tabulky
	PmG->Cells[0][0].Font->Color=clBlack;
	//naplnění buněk
	PmG->SetColumnAutoFit(-4);
	PmG->Cells[0][0].Type=PmG->COMBO;
	PmG->Columns[0].Width=220;
	PmG->Columns[1].Width=118;
	//sloučení hlavičky
	PmG->MergeCells(0,0,1,0);
	//naplnění Comba daty
	if(pom_temp->pohon!=NULL)
	{
		PmG->Cells[0][1].Text="Rychlost "+aRD;
		PmG->Cells[1][1].Type=PmG->EDIT;
		PmG->Cells[1][1].Text=m.round2double(outaRD(pom_temp->pohon->aRD),3);
		PmG->Cells[0][2].Text="Rozteč palce "+R;
		PmG->Cells[1][2].Type=PmG->EDIT;
		PmG->Cells[1][2].Text=m.round2double(outR(pom_temp->pohon->roztec),3);
		PmG->Cells[0][3].Text="Násobek rozteče palců";
		PmG->Cells[1][3].Type=PmG->EDIT;
		pom_temp->pohon->Rx=m.Rx(pom_temp->pohon->aRD,pom_temp->pohon->roztec);
		PmG->Cells[1][3].Text=m.round2double(pom_temp->pohon->Rx,3);
		PmG->Cells[0][4].Text="Rozteč jigů "+Rz;
		pom_temp->pohon->Rz=m.Rz(F->pom_temp->pohon->aRD);
		PmG->Cells[1][4].Text=m.round2double(outRz(pom_temp->pohon->Rz),3);
		PmG->Cells[0][5].Text="Mezera mezi podvozky "+Rz;
		PmG->Cells[1][5].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,0)),3);
		PmG->Cells[0][6].Text="Mezera mezi jigy 0° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,1)),3);
		PmG->Cells[0][7].Text="Mezera mezi jigy 90°"+Rz;
		PmG->Cells[1][7].Text=m.round2double(outRz(m.mezera(90,pom_temp->pohon->Rz,1)),3);
	}
	else//žádný pohon není přířazen, nelze vypočítávat hodnoty
	{
		PmG->Cells[0][1].Text="Rychlost "+aRD;
		PmG->Cells[1][1].Type=PmG->EDIT;
		PmG->Cells[1][1].Text=0;
		PmG->Cells[0][2].Text="Rozteč palce "+R;
		PmG->Cells[1][2].Type=PmG->EDIT;
		PmG->Cells[1][2].Text=0;
		PmG->Cells[0][3].Text="Násobek rozteče palců";
		PmG->Cells[1][3].Type=PmG->EDIT;
		PmG->Cells[1][3].Text=0;
		PmG->Cells[0][4].Text="Rozteč jigů "+Rz;
		PmG->Cells[1][4].Text=0;
		PmG->Cells[0][5].Text="Mezera mezi podvozky "+Rz;
		PmG->Cells[1][5].Text=0;
		PmG->Cells[0][6].Text="Mezera mezi jigy 0° "+Rz;
		PmG->Cells[1][6].Text=0;
		PmG->Cells[0][7].Text="Mezera mezi jigy 90°"+Rz;
		PmG->Cells[1][7].Text=0;
  }
	if(pom_temp->pohon==NULL)//pokud není přiřazen pohon zobrazí se pouze COMBO
	{
		PmG->VisibleRow(1,false,false);
		PmG->VisibleRow(2,false,false);
		PmG->VisibleRow(3,false,false);
		PmG->VisibleRow(4,false,false);
		PmG->VisibleRow(5,false,false);
		PmG->VisibleRow(6,false,false);
		PmG->VisibleRow(7,false,false);
	}
	else
	{
   	switch(pom_temp->rezim)//tabulka má 7 řádků
   	{
   		case -1://nic, stejné jako S&G
   		case 0://S&G, tři řádky
			{
				PmG->VisibleRow(3,false,false);
				PmG->VisibleRow(4,false,false);
   			PmG->VisibleRow(5,false,false);
				PmG->VisibleRow(6,false,false);
				PmG->VisibleRow(7,false,false);
   		}break;
   		case 1://kontinuální režim, zde jsou možné další 2 možnosti, 6 nebo 7 řádků
			{
				if(pocet_vyskytu_elementu(pom_temp)>0);
					//vyskytl se element s otočí -> 2 mezery mezi jigy, neskrývám nic
   			else
					PmG->VisibleRow(7,false,false);//pouze elementy bez otočí, jen jedna mezera mezi jigy
				//exButton bude v obou těchto případech zobrazen
				PmG->exBUTTONVisible=true;
				PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
				PmG->exBUTTON->ShowHint=true;PmG->exBUTTON->Hint="Rozšířené položky";
				//vypnutí zobrazení 4ho a 5tého řádku, defaultně v KK nezobrazen
				PmG->VisibleRow(5,false,false);
   		}break;//kontinuální,počítá se s jednou mezerou mezi jigy
		}
	}
	if(PmG->Rows[7].Visible)//budou zde obě mezeri mezi jigy
	{
		PmG->Cells[0][6].Text="Mezera mezi jigy 0° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,1)),3);
		PmG->Cells[0][7].Text="Mezera mezi jigy 90°"+Rz;
		PmG->Cells[1][7].Text=m.round2double(outRz(m.mezera(90,pom_temp->pohon->Rz,1)),3);
	}
	else if(PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
	{
		double uhel=d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(pom_temp,pom_temp->elementy->dalsi);
		PmG->Cells[0][6].Text="Mezera mezi jigy "+AnsiString(uhel)+"° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(uhel,pom_temp->pohon->Rz,1)),3);
	}
	//finální desing
	PmG->Cells[0][5].Background->Color=(TColor)RGB(240,240,240);
	for(int i=1;i<=ms.MyToDouble(PmG->RowCount-1);i++)
	{
		if (PmG->Cells[1][i].Type==PmG->EDIT)
			PmG->Cells[1][i].InputNumbersOnly=2;
		else
		{
			PmG->Cells[1][i].Font->Color=(TColor)RGB(128,128,128);
			PmG->Cells[1][i].RightMargin=5;
			PmG->Cells[1][i].Background->Color=(TColor)RGB(240,240,240);//m.clIntensive((TColor)RGB(128,128,128),115);
		}
		PmG->Cells[0][i].Align=mGrid->RIGHT;
		PmG->Cells[1][i].Align=mGrid->RIGHT;
		PmG->Cells[0][i].Font->Color=(TColor)RGB(128,128,128);
		PmG->Cells[0][i].RightMargin=3;
	}
	//umístění tabulky
	TRect oblast_kabiny=vrat_max_oblast(pom_temp);
	pom_temp->Xp=m.P2Lx(oblast_kabiny.right+30);
	pom_temp->Yp=m.P2Ly(oblast_kabiny.top-F->PmG->Rows->Height-30);
	//naplnění comba hodnotami
	tab_pohon_COMBO(0);
	if(pom_temp->pohon!=NULL)
	if(d.v.pohon_je_pouzivan(pom_temp->pohon->n,pom)!=NULL)//kontrola zda je pohon používán v jiném objektu, nutné posílat pom místo pom_temp do parametru mimo_objetk!!!!!
	{
		PmG->Update();//musí být přítomný !!!!
		PmG->SetEnabledComponents(false);//nastavení celé tabulky do neaktivního stavu
		PmG->SetEnabledComponent(0,0,true);//aktivace Comba pro výběr pohonu, bude vždy aktvní
	}
	PmG->Refresh();
}
//---------------------------------------------------------------------------
//předesignování tabulky po přidání každého elementu
void TForm1::pridani_elementu_tab_pohon(Cvektory::TElement *E)
{
  log(__func__);//logování
	AnsiString Rz;
	//nastavení jednotek podle posledních nastavení
	if (Rzunit==M) Rz="<a>[m]</a>";
	else Rz="<a>[mm]</a>";
	if(pom_temp->rezim==1&&!PmG->Rows[3].Visible)//změny pouze při přidání kontinuálního robota, vytvoření KK tabulky
	{
		//popřidání prvního elementu nutno spočítat!! a zapsat do buňěk
		pom_temp->pohon->Rz=m.Rz(F->pom_temp->pohon->aRD);
		pom_temp->pohon->Rx=m.Rx(pom_temp->pohon->aRD,pom_temp->pohon->roztec);
		PmG->Cells[1][3].Text=m.round2double(pom_temp->pohon->Rx,3);
		PmG->Cells[1][4].Text=m.round2double(outRz(pom_temp->pohon->Rz),3);
		PmG->Cells[1][5].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,0)),3);
		//samotné skrývání a zobrazování buňěk
		PmG->VisibleRow(3,true,false);
		PmG->VisibleRow(4,true,false);
		PmG->VisibleRow(5,false,false);
		PmG->VisibleRow(6,true,false);
		PmG->exBUTTONVisible=true;
		PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
		PmG->exBUTTON->ShowHint=true;PmG->exBUTTON->Hint="Rozšířené položky";
		FormX->validace_aRD();//spištění validace po předesignu tabulky pohonu na kontinuální režim
	}
	//zapnutí/vypnutí posledního řádku a naplnění hodnotami
	if(pocet_vyskytu_elementu(pom_temp)>0)
		PmG->VisibleRow(7,true,false);
	else PmG->VisibleRow(7,false,false);
	if(PmG->Rows[7].Visible)//budou zde obě mezeri mezi jigy
	{
		PmG->Cells[0][6].Text="Mezera mezi jigy 0° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,1)),3);
		PmG->Cells[0][7].Text="Mezera mezi jigy 90°"+Rz;
		PmG->Cells[1][7].Text=m.round2double(outRz(m.mezera(90,pom_temp->pohon->Rz,1)),3);
	}
	else if(PmG->Rows[6].Visible)//pouze jedna mezera mezi jig, nutná další kontrola, padaly by sem všechny varianty
	{
		double uhel=d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(pom_temp,pom_temp->elementy->dalsi);
		PmG->Cells[0][6].Text="Mezera mezi jigy "+AnsiString(uhel)+"° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(uhel,pom_temp->pohon->Rz,1)),3);
	}
	PmG->Update();
	//není nutná kontrola zda je přiřaznný pohon, tato metoda s spouští jedině v případ, že objekt má přiřazený pohon
	if(d.v.pohon_je_pouzivan(pom_temp->pohon->n,pom)!=NULL)//kontrola zda je pohon používán v jiném objektu, nutné posílat pom místo pom_temp do parametru mimo_objetk!!!!!
	{
		//Update musí být přítomný před!!!!
		PmG->SetEnabledComponent(1,3,false);//komponenta do této chvíle ještě neexistovala
	}
}
//volána po přiřazení pohonu
void TForm1::prirazeni_pohonu_tab_pohon(int index_pohonu)
{
  log(__func__);//logování
	FormX->vstoupeno_poh=false;//blokace událostí při vkládání elementu
	FormX->vstoupeno_elm=false;
	AnsiString Rz;
	//nastavení jednotek podle posledních nastavení
	if (Rzunit==M) Rz="<a>[m]</a>";
	else Rz="<a>[mm]</a>";
	if(index_pohonu!=0)//pokud je přiřazen nějaký pohon
	{
    if(pom_temp->id==3)pom_temp->rezim=1;//pro objekt POW bude vždy zobrazena tabulka v KK režimu
		switch(pom_temp->rezim)//tabulka má 7 řádků
   	{
   		case -1://nic, stejné jako S&G
   		case 0://S&G, tři řádky
   		{
   			PmG->VisibleRow(1,true,false);
   			PmG->VisibleRow(2,true,false);
   			PmG->VisibleRow(3,false,false);
   			PmG->VisibleRow(4,false,false);
   			PmG->VisibleRow(5,false,false);
   			PmG->VisibleRow(6,false,false);
   			PmG->VisibleRow(7,false,false);
   		}break;
   		case 1://kontinuální režim, zde jsou možné další 2 možnosti, 6 nebo 7 řádků
   		{
   			PmG->VisibleRow(1,true,false);
   			PmG->VisibleRow(2,true,false);
   			PmG->VisibleRow(3,true,false);
				PmG->VisibleRow(4,true,false);
   			PmG->VisibleRow(5,false,false);
   			PmG->VisibleRow(6,true,false);
   			if(pocet_vyskytu_elementu(pom_temp)>0)
   				PmG->VisibleRow(7,true,false);//vyskytl se element s otočí -> 2 mezery mezi jigy, neskrývám nic
   			else
   				PmG->VisibleRow(7,false,false);//pouze elementy bez otočí, jen jedna mezera mezi jigy
   			//exButton bude v obou těchto případech zobrazen
   			PmG->exBUTTONVisible=true;
   			PmG->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;
   			PmG->exBUTTON->ShowHint=true;PmG->exBUTTON->Hint="Rozšířené položky";
   		}break;//kontinuální,počítá se s jednou mezerou mezi jigy
		}
	}
	else//pohon byl odebrán
		odstraneni_elementu_tab_pohon(0);
	//po této metodě v OncChange UnitX padá do Timer2 -> refresh, zde není nutné mít refresh
	TscGPComboBox *PCombo=PmG->getCombo(0,0);
	if(PCombo->ItemIndex!=index_pohonu)PCombo->ItemIndex=index_pohonu;
	PCombo=NULL;delete PCombo;
	FormX->vstoupeno_poh=true;//blokace událostí při vkládání elementu
	PmG->Update();
	if(JID==-201 || JID==-202)//při kliku na předávací místo (tj. změna editovaného pohonu) kontrola zda je pohon v tabulce uložen i v pom_temp
	{
		if(index_pohonu==0 && pom_temp->pohon!=NULL)pom_temp->pohon=NULL;
		if(index_pohonu>0 && (pom_temp->pohon!=NULL && index_pohonu!=pom_temp->pohon->n || pom_temp->pohon==NULL))d.v.kopiruj_pohon(d.v.vrat_pohon(index_pohonu),pom_temp);
	}
	if(pom_temp->pohon!=NULL)
	{
		bool temp;//pomocná proměnná, použití u průcohdu elementů, uchovává zda mají být komponenty aktivní či ne
		if(d.v.pohon_je_pouzivan(pom_temp->pohon->n,pom)!=NULL)//kontrola zda je pohon používán v jiném objektu, nutné posílat pom místo pom_temp do parametru mimo_objetk!!!!!
		{
			//Update musí být přítomný před!!!!
			PmG->SetEnabledComponents(false);//nastavení celé tabulky do neaktivního stavu
			PmG->SetEnabledComponent(0,0,true);//aktivace Comba pro výběr pohonu, bude vždy aktvní
			temp=false;
		}
		else {PmG->SetEnabledComponents(true);temp=true;}//pokud není pohon přiřazen aktivace
	}
	REFRESH();
}
//---------------------------------------------------------------------------
//metoda volaná při odstranění elementu a při odebrání pohonu
void TForm1::odstraneni_elementu_tab_pohon(int operace)
{
  log(__func__);//logování
	FormX->vstoupeno_poh=false;//blokace událostí při vkládání elementu
	FormX->vstoupeno_elm=false;
	//0 = odebrání pohonu
	switch(operace)
	{
		case 0://odstranění pohonu, vše skryto zůstane pouze Combo
		{
			PmG->VisibleRow(1,false,false);
			PmG->VisibleRow(2,false,false);
			PmG->VisibleRow(3,false,false);
			PmG->VisibleRow(4,false,false);
			PmG->VisibleRow(5,false,false);
			PmG->VisibleRow(6,false,false);
			PmG->VisibleRow(7,false,false);
			PmG->exBUTTONVisible=false;
			PmG->ShowNote("",clRed,14);
		}break;
		case 1://smazání elementu
		{
			if(pom_temp->elementy->predchozi->eID==MaxInt && pom_temp->elementy->predchozi->n==1)//smazání všech elementů (zůstala pouze zarážka), tabulka se nastavý do základního rozpoložení, tj. S&G tabulka
			{
				PmG->VisibleRow(3,false,false);
				PmG->VisibleRow(4,false,false);
				PmG->VisibleRow(5,false,false);
				PmG->VisibleRow(6,false,false);
				PmG->VisibleRow(7,false,false);
				PmG->exBUTTONVisible=false;
				PmG->ShowNote("",clRed,14);
			}
			else
			{
		  	//odebrán poslední element s otočí, ale zobrazeny obě mezery mezi jig
				if(pocet_vyskytu_elementu(pom_temp)==0&&PmG->Rows[7].Visible)
					PmG->VisibleRow(7,false,false);
				//aktualizace úhlu, pokud si smažu poslední otoč, která není posledním elementem dojde ke změně rotace v kabině
				if(!PmG->Rows[7].Visible&&PmG->Rows[6].Visible)
				{
          AnsiString Rz;
		  		if (Rzunit==M) Rz="<a>[m]</a>";
					else Rz="<a>[mm]</a>";
					double uhel=d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(pom_temp,pom_temp->elementy->dalsi);
					PmG->Cells[0][6].Text="Mezera mezi jigy "+AnsiString(uhel)+"° "+Rz;
					PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(uhel,pom_temp->pohon->Rz,1)),3);
				}
			}
		}break;
	}
	PmG->Update();
	REFRESH();
	FormX->vstoupeno_poh=true;//blokace událostí při vkládání elementu
}
//---------------------------------------------------------------------------
//provede změnu jednotek v tabulce pohonu
void TForm1::zmena_jednotek_tab_pohon()
{
  log(__func__);//logování
	FormX->vstoupeno_poh=false;//blokace událostí při vkládání elementu
	FormX->vstoupeno_elm=false;
	//překlopení jednotek
	switch(JID)
	{
		case 5://rychlost
		{
			if (aRDunit==SEC) aRDunit=MIN;
			else aRDunit=SEC;
		}break;
		case 6://rozteč palce
		{
			if (Runit==M) Runit=MM;
			else Runit=M;
		}break;
		case 7://Násobek rozteče palců
		{

		}break;
		case 8://Rozteč jigů
		{
    	if (Rzunit==M) Rzunit=MM;
			else Rzunit=M;
		}break;
		case 9://mezera mezi podvozky
		{
			if (Rzunit==M) Rzunit=MM;
			else Rzunit=M;
		}break;
		case 10: //mezera mezi jig
		{
			if (Rzunit==M) Rzunit=MM;
			else Rzunit=M;
		}break;
		case 11: //mezera mezi jig
		{
    	if (Rzunit==M) Rzunit=MM;
			else Rzunit=M;
		}break;
	}
	//nastavení jednotek po změně
	AnsiString aRD,R,Rz;
	if (aRDunit==SEC) aRD="<a>[m/s]</a>";
	else aRD="<a>[m/min]</a>";
	if (Runit==M) R="<a>[m]</a>";
	else R="<a>[mm]</a>";//1
	if (Rzunit==M) Rz="<a>[m]</a>";
	else Rz="<a>[mm]</a>";
	//zapsání nových jednotek do INI
	writeINI("nastaveni_form_parametry","RDt", aRDunit);
	writeINI("nastaveni_nahled","R", Runit);
	writeINI("nastaveni_nahled","Rz", Rzunit);
	//změna v tabulce
	PmG->Cells[0][1].Text="Rychlost "+aRD;
	PmG->Cells[1][1].Text=m.round2double(outaRD(pom_temp->pohon->aRD),3);//rychlost raději zaokrouhlovat
	PmG->Cells[0][2].Text="Rozteč palce "+R;
	PmG->Cells[1][2].Text=m.round2double(outR(pom_temp->pohon->roztec),3);
	PmG->Cells[0][4].Text="Rozteč jigů "+Rz;
	PmG->Cells[1][4].Text=m.round2double(outRz(pom_temp->pohon->Rz),3);
	PmG->Cells[0][5].Text="Mezera mezi podvozky "+Rz;
	PmG->Cells[1][5].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,0)),3);
//	PmG->Cells[1][5].Text=1*1;
	if(PmG->Rows[7].Visible)
	{
		PmG->Cells[0][6].Text="Mezera mezi jigy 0° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(0,pom_temp->pohon->Rz,1)),3);
		PmG->Cells[0][7].Text="Mezera mezi jigy 90°"+Rz;
		PmG->Cells[1][7].Text=m.round2double(outRz(m.mezera(90,pom_temp->pohon->Rz,1)),3);
	}
	else if(PmG->Rows[6].Visible)
	{
  	double uhel=d.v.vrat_rotaci_jigu_po_predchazejicim_elementu(pom_temp,pom_temp->elementy->dalsi);
		PmG->Cells[0][6].Text="Mezera mezi jigy "+AnsiString(uhel)+"° "+Rz;
		PmG->Cells[1][6].Text=m.round2double(outRz(m.mezera(uhel,pom_temp->pohon->Rz,1)),3);
	}
	aktualizace_ComboPohon();//v combo jsou vypsány jednotky, proto nutné aktualizovat
	FormX->vstoupeno_poh=true;//blokace událostí při vkládání elementu
}
//---------------------------------------------------------------------------
//prohledá elementy v objektu, vrátí 0 pokud se vyskytuje pouze jedna rotace, vrátí 1 pokud se vyskytují 2 rotace
int TForm1::pocet_vyskytu_elementu(Cvektory::TObjekt *Objekt)
{
  log(__func__);//logování
	int ret=0;int rotace=0;
	Cvektory::TElement *E=Objekt->elementy;//nepřeskakovat hlavičku
	while(E!=NULL)
	{
		if(E->n>0)//ošwtření hlavičky
		{
			if(abs(E->rotace_jig)==90)//zajímá nás pouze 90° rotace
				{ret++;break;}
		}
		E=E->dalsi;
	}
	E=NULL; delete E;
	return ret;
}
//---------------------------------------------------------------------------
//0=načtení pohonů do COMBA, 1=přiřazení pohonu kabině
void TForm1::tab_pohon_COMBO (int index)
{
	log(__func__);//logování
	TscGPComboBox *PCombo=PmG->getCombo(0,0);
	Cvektory::TPohon *P=d.v.POHONY->dalsi;//ukazatel na pohony, přeskakuje hlavičku, která je již vytvořena
	TscGPListBoxItem *t=NULL;
	////příprava vypisovaných jednotek
	AnsiString Tcas="min",Td="m";
	if(aRDunit==SEC)
		Tcas="s";
	UnicodeString caption_jednotky=Td+"/"+Tcas;
	if(index==0)//prvotní vytvoření
	{
		PCombo->Items->Clear();//smazání původního obsahu
		if(P==NULL)//pokud neexitustuje žádný pohon
		{
			t=PCombo->Items->Add(/*tady nelze parametr*/);  //32 znaků se vleze do COMBA
			t->Caption="Žádný pohon k výběru";
			//nesmí tu být, způsobuje cyklení z důvodu vytoření onchange pohonu Form_parametry->scComboBox_pohon->ItemIndex=0;//pohon nedefinován
		}
		else//pokud existuje přidá na první pozici nabídku nepřiřazen dále začne plnit existujícím pohny
		{
			//vytvoření položky nepřiřazen
			t=PCombo->Items->Add(/*tady nelze parametr*/);
			t->Caption="Vyberte pohon";
			//plnění existujícím pohony
			while (P!=NULL)
			{
				AnsiString dopRD="";
				t=PCombo->Items->Add(/*tady nelze parametr*/);
				t->Caption=P->name+" - "+m.round2double(outaRD(P->aRD),3,"..")+" ("+AnsiString(m.round2double(outaRD(P->rychlost_od),2))+"-"+AnsiString(m.round2double(outaRD(P->rychlost_do),2))+") "+caption_jednotky+dopRD;
				P=P->dalsi;//přesun na další pohon
			}
		}
		if(pom_temp->pohon!=NULL){PCombo->ItemIndex=pom_temp->pohon->n;}
		else {PCombo->ItemIndex=0;/*PmG->Cells[0][0].Highlight=true;*/}
	}
	if(index==1 && JID!=-201 && JID!=-202)//přiřazení pohonu
	{
		//deklarace atributů
		TIP="";
		unsigned long p_puvodni=0;
		if(pom_temp->pohon!=NULL)p_puvodni=pom_temp->pohon->n;
		int pohon=PCombo->ItemIndex;
		//zapsání pohonu do aktuálně editovaného
		if(pohon!=0)d.v.kopiruj_pohon(d.v.vrat_pohon(pohon),pom_temp);
		else pom_temp->pohon=NULL;
		Cvektory::TElement *E=pom_temp->elementy->dalsi;
		//přiřazení pohonu elementům
		while(E!=NULL)
		{
			if(E->pohon==NULL && p_puvodni==0 || E->pohon!=NULL && p_puvodni!=0 && E->pohon->n==p_puvodni){E->pohon=pom_temp->pohon; }
			set_enabled_mGrid(E);
			E=E->dalsi;
		}
		E=NULL;delete E;
		//ostatní
		nahled_ulozit(true);
		prirazeni_pohonu_tab_pohon(pohon);
		vlozit_predavaci_misto();//přidání či odebrání předávacího místa
		if(PmG->Rows[3].Visible || PmG->Note.Text!="")FormX->validace_aRD();//odstranění validacez jiného pohonu, pouze pokud je pohonová tabulka v režimu KK nebo, Note tabulky je naplněn
		//zajistí překreslení knihoven když je přidán či odebrán pohon
		DrawGrid_knihovna->Refresh();
		DrawGrid_otoce->Refresh();
		DrawGrid_ostatni->Refresh();
		DrawGrid_geometrie->Refresh();
	}
	//podbarvení červeně pokud není vybrán žádný pohon
//	if(PCombo->ItemIndex==0)
//	{PmG->Cells[0][0].Highlight=true;/*clHighlight=clRed;*/}
//	else
//	{PmG->Cells[0][0].Highlight=false;/*clHighlight=(TColor)RGB(225,128,0);*/}
//	{PCombo->Options->FrameWidth=2;PCombo->Options->FrameFocusedColor=clRed;PCombo->Options->FrameNormalColor=clRed;PCombo->Options->FrameDisabledColor=clRed;PCombo->Options->FrameDisabledColorAlpha=128;}
//	else {PCombo->Options->FrameNormalColor=clGray;PCombo->Options->FrameFocusedColor=clHighlight;PCombo->Options->FrameWidth=1;PCombo->Options->FrameDisabledColor=clBtnShadow;PCombo->Options->FrameDisabledColorAlpha=255;}

	t=NULL; delete t;
	P=NULL; delete P;
	PCombo=NULL; delete PCombo;
	DrawGrid_knihovna->SetFocus();
}
//---------------------------------------------------------------------------
//zapne nebo vypne komponenty megridu v závislosti na tom zda má element přiřazený pohon
void TForm1::set_enabled_mGrid(Cvektory::TElement *E)
{
	//nastavení stavu
	bool stav=true;
	if(E->pohon==NULL)stav=false;
	if(pom_temp->pohon!=NULL && d.v.pohon_je_pouzivan(pom_temp->pohon->n,pom)!=NULL)stav=false;
	//přepnutí buněk
	switch(E->eID)
	{
		case 0:
		{
			if(E->mGrid->Cells[1][3].Type==E->mGrid->EDIT)E->mGrid->SetEnabledComponent(1,3,stav);//měnit pouze pokud se jedná o edit
		}
		break;//stop stanice
		case 1:case 7:case 11:case 15:case 101:case 105://robor kontinuální
		{
			E->mGrid->SetEnabledComponent(1,1,stav);
			E->mGrid->SetEnabledComponent(1,2,stav);
		}
		break;
		case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
		{
			E->mGrid->SetEnabledComponent(1,2,stav);
			E->mGrid->SetEnabledComponent(1,3,stav);
		}
		break;
		case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otočí
		{
			E->mGrid->SetEnabledComponent(1,1,stav);
			E->mGrid->SetEnabledComponent(1,2,stav);
			E->mGrid->SetEnabledComponent(1,5,stav);
			E->mGrid->SetEnabledComponent(1,6,stav);
			E->mGrid->SetEnabledComponent(1,7,stav);
		}
		break;
		case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otočí
		{
			E->mGrid->SetEnabledComponent(1,5,stav);
			E->mGrid->SetEnabledComponent(1,6,stav);
		}break;
		case 5: case 6://otoč pasivní + otoč aktivní
		{
			if(E->mGrid->Cells[1][2].Type==E->mGrid->EDIT)E->mGrid->SetEnabledComponent(1,2,stav);
			if(E->mGrid->Cells[1][3].Type==E->mGrid->EDIT)E->mGrid->SetEnabledComponent(1,3,stav);
		}break;
	}
}
//---------------------------------------------------------------------------
//aktualizuje combo při změně jednotek nebo při změně rychlosti
void TForm1::aktualizace_ComboPohon ()
{
	log(__func__);//logování
	Cvektory::TPohon *P=F->d.v.POHONY->dalsi;//ukazatel na pohony, přeskakuje hlavičku, která je již vytvořena
	TscGPComboBox *Combo=F->PmG->getCombo(0,0);
	Combo->Items->Clear();//smazání původního obsahu
	TscGPListBoxItem *t=NULL;
	if(P==NULL)//pokud neexitustuje žádný pohon
	{
		t=Combo->Items->Add(/*tady nelze parametr*/);
		t->Caption="Žádný pohon k výběru";
	}
	else//pokud existuje přidá na první pozici nabídku nepřiřazen dále začne plnit existujícím pohny
	{
		//vytvoření položky nepřiřazen
		t=Combo->Items->Add(/*tady nelze parametr*/);
		if(Combo->ItemIndex==0) t->Caption="Vyberte pohon";
		else t->Caption="Pohon nepřiřazen";
		////příprava vypisovaných jednotek
		AnsiString Tcas="min",Td="m";
		if(F->aRDunit==F->SEC)Tcas="s";
		UnicodeString caption_jednotky=Td+"/"+Tcas;
		//plnění existujícím pohony
		while (P!=NULL)
		{
			AnsiString dopRD="";
			t=Combo->Items->Add(/*tady nelze parametr*/);
			if(Combo->ItemIndex==(long)P->n)//pokud se jedná o aktuální pohon a pohon lze editovat a neprobíhá změna comboboxu a již existuje nějaký pohon
			{
				t->Caption=F->pom_temp->pohon->name+" - "+F->m.round2double(F->outaRD(F->pom_temp->pohon->aRD),3,"..")+" ("+AnsiString(F->m.round2double(F->outaRD(F->pom_temp->pohon->rychlost_od),2))+"-"+AnsiString(F->m.round2double(F->outaRD(F->pom_temp->pohon->rychlost_do),2))+") "+caption_jednotky;//vypíše aktuální editovanou hodnotu
			}
			else//pro ostatní pohony, či aktální používaný (přiřazen více objektům) pohon - většinová situace
			{
				t->Caption=P->name+" - "+F->m.round2double(F->outaRD(P->aRD),3,"..")+" ("+AnsiString(F->m.round2double(F->outaRD(P->rychlost_od),2))+"-"+AnsiString(F->m.round2double(F->outaRD(P->rychlost_do),2))+") "+caption_jednotky;
			}
			P=P->dalsi;//přesun na další pohon
		}
		//nastavení comba, pokud neexistuje pohon
		if(F->d.v.POHONY->dalsi==NULL)Combo->ItemIndex=0;//nepřiřazen
	}
	Combo->Refresh();
	P=NULL; delete P;
	Combo=NULL; delete Combo;
	t=NULL; delete t;
}
//---------------------------------------------------------------------------
//nadesignuje tabulky daného elementu
void TForm1::design_element(Cvektory::TElement *E,bool prvni_spusteni)
{
  log(__func__);//logování
	//definice barev
	TColor clHeaderFont=clBlack;
	TColor clBackgroundHidden=(TColor)RGB(240,240,240);//m.clIntensive((TColor)RGB(128,128,128),115);
	TColor clFontLeft = (TColor)RGB(128,128,128);
	TColor clFontRight = (TColor)RGB(43,87,154);
	//definice fontu a velikosti písma
	E->mGrid->DefaultCell.Font->Name=aFont->Name;
	E->mGrid->DefaultCell.Font->Size=aFont->Size;
	E->mGrid->DefaultCell.isNegativeNumber->Name=aFont->Name;
	E->mGrid->DefaultCell.isNegativeNumber->Size=aFont->Size;
	E->mGrid->DefaultCell.isZero->Name=aFont->Name;
	E->mGrid->DefaultCell.isZero->Size=aFont->Size;
	E->mGrid->DefaultCell.isLink->Name=aFont->Name;
	E->mGrid->DefaultCell.isLink->Size=aFont->Size;
	E->mGrid->DefaultCell.isActiveLink->Name=aFont->Name;
	E->mGrid->DefaultCell.isActiveLink->Size=aFont->Size;
	E->mGrid->Note.Font->Name=aFont->Name;
	E->mGrid->Note.Font->Size=14;

	//definice jednotek a šířek
	AnsiString LO,cas,delka_otoce;
	short sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla;//hodnoty pro základní jednotky
	//když není font roboto nutno rozšířit buňky s textem
	if(aFont->Name!="Roboto Cn"){sirka_0=10;sirka_1=10;sirka_2=10;sirka_3=10;sirka_4=10;sirka_56=10;}
	//nastavení jednotek podle posledních nastavení
	if (PTunit==0) cas="<a>[s]</a>";//0
	else cas="<a>[min]</a>";//1
	if (LOunit==0) LO="<a>[m]</a>";//0
	else LO="<a>[mm]</a>";//1
	if (DOtocunit==0) delka_otoce="<a>[m]</a>";//0
	else delka_otoce="<a>[mm]</a>";//1
	//nastavení šířek
	if(PTunit==0&&LOunit==0) {sirka_1=57+33;sirka_cisla=70;}//+20 dočasně na rozšíření
	else {sirka_1=105;sirka_cisla=100;}
	if(PTunit==0) {sirka_0=147; sirka_2=88;sirka_4=59;sirka_cisla=70;}
	else {sirka_0=149; sirka_2=107;sirka_4=78;sirka_cisla=100;}
	if(DOtocunit==0&&PTunit==0) {sirka_56=76;sirka_cisla=70;}
	else {sirka_56=90;sirka_cisla=100;}
	if(PTunit==0&&LOunit==0&&DOtocunit==0) {sirka_3=68;sirka_cisla=70;}
	else {sirka_3=81;sirka_cisla=100;}
	//nadesignování tabulek dle typu elementu
	E->mGrid->Left=-1000;
	E->mGrid->Top=-1000;
	if(E->eID==2) E->PT1=60;
	if(prvni_spusteni)prvni_vytvoreni_tab_elementu(E,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla,LO,cas,delka_otoce);
	else dalsi_vytvoreni_tab_elementu(E,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla,LO,cas,delka_otoce);
	//formátování hlavičky tabulky (vždy stejné)
	E->mGrid->Border.Width=2;
	E->mGrid->Cells[0][0].Text="<a>"+E->name+"</a>";//nasazení linku
	E->mGrid->Cells[0][0].isLink->Color=clHeaderFont;
	E->mGrid->Cells[0][0].isActiveLink->Color=clHeaderFont;
	E->mGrid->Cells[0][0].BottomBorder->Width=2;
	//formátování buněk tabulky (vždy stejn=)
	for(int i=1;i<=ms.MyToDouble(E->mGrid->RowCount-1);i++)
	{
		if (E->mGrid->Cells[1][i].Type==E->mGrid->EDIT)E->mGrid->Cells[1][i].InputNumbersOnly=2;
		E->mGrid->Cells[0][i].RightMargin = 3;
		E->mGrid->Cells[1][i].RightMargin = 3;
		E->mGrid->Cells[0][i].Font->Color=clFontLeft;
		E->mGrid->Cells[0][i].Align=mGrid->RIGHT;
		E->mGrid->Cells[1][i].Align=mGrid->RIGHT;
		if (!(E->eID==0 && i==1))E->mGrid->Cells[1][i].Text=m.round2double(ms.MyToDouble(E->mGrid->Cells[1][i].Text),3);//přeskočení prvního řádku v tabulce stopky
		if (E->mGrid->Cells[1][i].Type!=E->mGrid->EDIT&&E->mGrid->Cells[1][i].Type!=E->mGrid->COMBO)
		{
			E->mGrid->Cells[1][i].Background->Color=clBackgroundHidden;
			E->mGrid->Cells[1][i].RightMargin=5;
			E->mGrid->Cells[1][i].Font->Color=clFontLeft;
		}
	}
	//sloučení buněk hlavičky
	E->mGrid->MergeCells(0,0,1,0);//update na tomto místě působí potíže, přesunout do add element asi a do NP_input;
	E->mGrid->Update();//musí být přítomen před zakazováním komponent, před Update tabulka ještě neexistuje
	//naplnění a přiřazení COMBA rotace
	if(E->eID==3||E->eID==4||E->eID==5||E->eID==6||E->eID==9||E->eID==10||E->eID==13||E->eID==14||E->eID==17||E->eID==18||E->eID==103||E->eID==104||E->eID==107||E->eID==108)//elementy s otočí
	{
		int pozice;
		switch(E->eID)//nutnost zjistit pozici komba
		{
			case 3:case 9:case 13:case 17:case 103:case 107:pozice=3;break;
			case 4:case 10:case 14:case 18:case 104:case 108:pozice=2;break;
			case 5:
			case 6:pozice=1;break;
		}
		TscGPComboBox *C=E->mGrid->getCombo(1,pozice);
		C->Clear();
		C->Font->Color=(TColor)RGB(43,87,154);
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
		C=NULL;delete C;
	}
	//naplnění a přiřazení COMBA PD
	if(E->eID==1||E->eID==3||E->eID==7||E->eID==9||E->eID==11||E->eID==13||E->eID==15||E->eID==17||E->eID==101||E->eID==103||E->eID==105||E->eID==107)
	{
		TscGPComboBox *C=E->mGrid->getCombo(1,E->mGrid->RowCount-1);
		C->Clear();
		C->Font->Color=(TColor)RGB(43,87,154);
		C->BiDiMode=bdRightToLeft;
		TscGPListBoxItem *I;
		I=C->Items->Add();
		I->Caption="začátek";//kvůli opačnému zarovnání musí být číslo zapsáno jako řetězec se znaménkem na konci!
		I=C->Items->Add();
		I->Caption="střed";
		I=C->Items->Add();
		I->Caption="celý";
		I=NULL;delete I;
		//přiřazení COMBA
		C->ItemIndex=E->PD;
		C=NULL;delete C;
		if(E->mGrid->Columns[1].Width<100)E->mGrid->Columns[1].Width+=15;//rozčíření skrze combo
	}
	set_enabled_mGrid(E);
//	if(pom_temp->pohon!=NULL)//pokud má objekt přiřazený pohon
//	{
//		if(d.v.pohon_je_pouzivan(pom_temp->pohon->n,pom)!=NULL)//pokud je tento pohon používán mimo objekt, jako parametr mimo_objekt musí být pom!!!!!
//		{
//			switch(E->eID)
//			{
//				case 1:case 7:case 11:case 15:case 101:case 105:E->mGrid->SetEnabledComponent(1,1,false);break;
//				case 3:case 9:case 13:case 17:case 103:case 107:{E->mGrid->SetEnabledComponent(1,1,false);E->mGrid->SetEnabledComponent(1,6,false);}break;
//			}
//		}
//	}
	//////dočasně
	if(E->eID==1||E->eID==7||E->eID==11||E->eID==15||E->eID==101||E->eID==105)E->mGrid->Cells[1][3].InputNumbersOnly=1;
}
//---------------------------------------------------------------------------
//vytvoření tabulek, první výpočty a zapsání do spojáku
void TForm1::prvni_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce)
{
	log(__func__);//logování
	double aRD=0,roztec=0;//proměnné pro ukládání parametrů pohonu
	switch(E->eID)
	{
		case 0://stop stanice
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
			//nastavování režimů podle ID objektu
			unsigned int id=d.v.vrat_objekt(E->objekt_n)->id;
			if(id>=6 && id<=10){E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;}
			//definice buněk
			E->mGrid->Cells[0][1].Text="Sparovaná stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][1].Text=E->sparovany->name;else E->mGrid->Cells[1][1].Text="N/A";
			E->mGrid->Cells[0][2].Text="RT "+cas;E->RT=m.RT(0,d.v.vzdalenost_od_predchoziho_elementu(E,true),pom_temp->pohon->aRD,pom_temp->pohon->roztec,E->WT+E->WTstop);
			E->mGrid->Cells[1][2].Text=outPT(E->RT);
			E->mGrid->Cells[0][3].Text="max. WT stop "+cas;
			E->mGrid->Cells[1][3].Text=outPT(E->WTstop);
			E->mGrid->Cells[0][4].Text="WT palec "+cas;
			E->mGrid->Cells[1][4].Text=outPT(m.cekani_na_palec(0,F->pom_temp->pohon->roztec,F->pom_temp->pohon->aRD,3));
			E->WT=inPT(ms.MyToDouble(E->mGrid->Cells[1][4].Text));
			E->mGrid->Cells[0][5].Text="počet pozic";
			if(id>=6 && id<=10)E->max_pocet_voziku=max_voziku(E);
			E->mGrid->Cells[1][5].Text=E->max_pocet_voziku;
			E->mGrid->Cells[0][6].Text="počet vozíků";
			E->mGrid->Cells[1][6].Text=E->akt_pocet_voziku;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_0;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint="časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="čas uzavřené STOP stanice";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="maximální možná doba čekání na palec";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint="maximální možný počet pozic";
			E->mGrid->Cells[0][5].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;E->mGrid->exBUTTON->Hint="Rozšířené položky";
			if(!(id>=6 && id<=10)){E->mGrid->VisibleRow(3,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
			break;
		}
		case 7:case 11:case 15:case 101:case 105:
		case 1://robot (kontinuální)
		{
			//načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL)aRD=E->pohon->aRD;
			//načtení popisků
			AnsiString t="LO ",t_hint="délka lakovácího okna";
			if(E->eID!=1){t="PO ";t_hint="délka pracovní oblasti";}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,5);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
			E->mGrid->Cells[1][1].Text=outPT(m.PT(E->LO1,aRD));
			E->PT1=inPT(ms.MyToDouble(E->mGrid->Cells[1][1].Text));
			E->mGrid->Cells[0][2].Text=t+LO;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outLO(E->LO1);
			E->mGrid->Cells[0][3].Text="vyosení "+LO;
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=0;
			E->mGrid->Cells[1][3].isNegativeNumber->Color=(TColor)RGB(43,87,154);//musí být nastaveno ručně, vymazání podmíněného formátování
			E->mGrid->Cells[0][4].Text="PD jig";
			E->mGrid->Cells[1][4].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_1;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=t_hint;
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="vyosení lakovácího okna";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="part detect";
			E->mGrid->Cells[0][4].ShowHint=true;
			break;
		}
		case 8:case 12:case 16:case 102:case 106:
		case 2://robot se stop stanicí
		{
			//načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL){aRD=E->pohon->aRD;roztec=E->pohon->roztec;}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,4);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->PT1);
			E->mGrid->Cells[0][3].Text="max WT "+cas;
			E->mGrid->Cells[1][3].Text=outPT(m.cekani_na_palec(0,roztec,aRD,3));
			E->WT=inPT(ms.MyToDouble(E->mGrid->Cells[1][3].Text));
			E->mGrid->Cells[0][2].Text="RT "+cas;
			E->mGrid->Cells[1][2].Text=outPT(m.RT(E->PT1,d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT));
			E->RT=inPT(ms.MyToDouble(E->mGrid->Cells[1][2].Text));
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_2;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint="časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="maximální možná doba čekání na palec";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 9:case 13:case 17:case 103:case 107:
		case 3://robot s pasivní otočí
		{
      //načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL)aRD=E->pohon->aRD;
      //načtení popisků
			AnsiString t="LO ",t_hint="délka lakovácího okna";
			if(E->eID!=3){t="PO ";t_hint="délka pracovní oblasti";}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,9);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(m.PT(E->LO1,aRD));
			E->PT1=inPT(ms.MyToDouble(E->mGrid->Cells[1][1].Text));
			E->mGrid->Cells[0][2].Text=t+LO;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outLO(E->LO1);
			E->mGrid->Cells[0][2].BottomBorder->Width=2;
			E->mGrid->Cells[1][2].BottomBorder->Width=2;
			E->mGrid->Cells[0][3].Text="rotace [°]";
			E->mGrid->Cells[1][3].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][4].Text="otoč "+cas;
			E->mGrid->Cells[1][4].Text=outPT(m.PTo(E->OTOC_delka,aRD));
			E->PTotoc=inPT(ms.MyToDouble(E->mGrid->Cells[1][4].Text));
			E->mGrid->Cells[0][5].Text="otoč "+delka_otoce;
			E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;E->mGrid->Cells[1][5].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[0][5].BottomBorder->Width=2;
			E->mGrid->Cells[1][5].BottomBorder->Width=2;
			E->mGrid->Cells[0][6].Text="PT2 "+cas;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outPT(m.PT(E->LO2,aRD));
			E->PT2=inPT(ms.MyToDouble(E->mGrid->Cells[1][6].Text));
			if(E->eID>100)E->mGrid->Cells[0][7].Text="PO2 "+LO; else E->mGrid->Cells[0][7].Text="LO2 "+LO;
			E->mGrid->Cells[1][7].Type=E->mGrid->EDIT;E->mGrid->Cells[1][7].Text=outLO(E->LO2);
			E->mGrid->Cells[0][7].BottomBorder->Width=2;
			E->mGrid->Cells[1][7].BottomBorder->Width=2;
			E->mGrid->Cells[0][8].Text="PD jig";
			E->mGrid->Cells[1][8].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=81;//sirka_3;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=t_hint;
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint="délka otoče";
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][6].ShowHint=true;
			if(E->eID>100)E->mGrid->Cells[0][7].Hint="délka pracovní oblasti"; else E->mGrid->Cells[0][7].Hint="délka lakovácího okna";
			E->mGrid->Cells[0][7].ShowHint=true;
			E->mGrid->Cells[0][8].Hint="part detect";
			E->mGrid->Cells[0][8].ShowHint=true;
			break;
		}
		case 10:case 14:case 18:case 104:case 108:
		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
      //načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL){aRD=E->pohon->aRD;roztec=E->pohon->roztec;}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->PT1);
			E->mGrid->Cells[0][2].Text="rotace [°]";
			E->mGrid->Cells[1][2].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][3].Text="PTo "+cas;
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][4].Text="PT2 "+cas;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(E->PT2);
			E->mGrid->Cells[0][6].Text="WT "+cas;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outPT(m.cekani_na_palec(0,roztec,aRD,3));
			E->WT=inPT(ms.MyToDouble(E->mGrid->Cells[1][6].Text));
			E->mGrid->Cells[0][5].Text="RT "+cas;
			E->mGrid->Cells[1][5].Text=outPT(m.RT(E->PT1+E->PT2+E->PTotoc,d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT));
			E->RT=inPT(ms.MyToDouble(E->mGrid->Cells[1][5].Text));
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=78;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint="časová rezerva";
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint="maximální možná doba čekání na palec";
			E->mGrid->Cells[0][6].ShowHint=true;
			break;
		}
		case 5://otoč pasivní
		{
      //načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL)aRD=E->pohon->aRD;
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,4);
			//definice buněk
			E->mGrid->Cells[0][1].Text="rotace [°]";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text="délka "+delka_otoce;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[0][3].Text="PT "+cas;//PT u pasivní nelze zadat
			E->mGrid->Cells[1][3].Text=outPT(m.PTo(E->OTOC_delka,aRD));
			E->PTotoc=inPT(ms.MyToDouble(E->mGrid->Cells[1][3].Text));
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_56;//Delší text
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint="délka otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
      //načtení hodnot z pohonu + ošetření proti nepřiřazenému pohonu
			if(E->pohon!=NULL){aRD=E->pohon->aRD;roztec=E->pohon->roztec;}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,4);
			//definice buněk
			E->mGrid->Cells[0][1].Text="rotace [°]";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
			//E->mGrid->Cells[0][2].Text="délka "+delka_otoce;//D u aktivní nelze zadat
			//E->mGrid->Cells[1][2].Text=outDO(m.Dotoc(E->PTotoc,aRD));//původně EDIT, ale background lze nastavit pouze pro text, EDIT se jen slabě orámuje
			//E->OTOC_delka=inDO(ms.MyToDouble(E->mGrid->Cells[1][2].Text));
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->WT=m.cekani_na_palec(0,roztec,aRD,3);
			E->RT=m.RT(E->PTotoc,d.v.vzdalenost_od_predchoziho_elementu(E,true),aRD,roztec,E->WT);
			E->mGrid->Cells[1][3].Text=outPT(E->RT);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
//			E->mGrid->Cells[0][2].Hint="délka otoče";
//			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][2].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="časová rezerva";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 200://předávací místo
		{
			E->mGrid->Create(2,2);
			E->mGrid->Cells[0][1].Text="WT palec "+cas;
			E->mGrid->Cells[1][1].Text=outPT(E->WT);
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=120;
			E->mGrid->Columns[1].Width=sirka_cisla;
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
void TForm1::dalsi_vytvoreni_tab_elementu (Cvektory::TElement *E,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla,AnsiString LO,AnsiString cas,AnsiString delka_otoce)
{
	log(__func__);//logování
	switch(E->eID)
	{
		case 0://stop stanice
		{
			unsigned int stav=0;
			if(E->mGrid->RowCount!=0 && E->mGrid->Rows[3].Visible)stav=1;//uchování stavu zda byla tabulka "rozbalená"
			if(E->mGrid->RowCount!=0 && !E->mGrid->Rows[3].Visible)stav=2;//uchování stavu zda byla tabulka "zabalená"
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
			//nastavování režimů podle ID objektu
			unsigned int id=d.v.vrat_objekt(E->objekt_n)->id;
			if(id>=6 && id<=10){E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;}
			//definice buněk
			E->mGrid->Cells[0][1].Text="Sparovaná stop";
			if(E->sparovany!=NULL)E->mGrid->Cells[1][1].Text=AnsiString(E->sparovany->name);else E->mGrid->Cells[1][1].Text="N/A";
			E->mGrid->Cells[0][2].Text="RT "+cas;
			E->mGrid->Cells[1][2].Text=outPT(E->RT);
			E->mGrid->Cells[0][3].Text="max. WT stop "+cas;
			E->mGrid->Cells[1][3].Text=outPT(E->WTstop);
			E->mGrid->Cells[0][4].Text="WT palec "+cas;
			E->mGrid->Cells[1][4].Text=outPT(E->WT);
			E->mGrid->Cells[0][5].Text="počet pozic";
			if(id>=6 && id<=10)E->max_pocet_voziku=max_voziku(E);
			E->mGrid->Cells[1][5].Text=E->max_pocet_voziku;
			E->mGrid->Cells[0][6].Text="počet vozíků";
			E->mGrid->Cells[1][6].Text=E->akt_pocet_voziku;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_0;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint="časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="čas uzavřené STOP stanice";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="maximální možná doba čekání na palec";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint="maximální možný počet pozic";
			E->mGrid->Cells[0][5].ShowHint=true;
			//nastavení exButtonu, skrývání řádku max.WT Stop
			E->mGrid->exBUTTONVisible=true;
			E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkUpArrow;
			E->mGrid->exBUTTON->ShowHint=true;E->mGrid->exBUTTON->Hint="Rozšířené položky";
			switch(stav)
			{
				//znovuotevření náhledu
				case 0:if(!(id>=6 && id<=10)){E->mGrid->VisibleRow(3,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
				break;
				//tabulka byl před posunem rozbalena
				case 1:break;
				//tabulka byla před posunem zabalena
				case 2:{E->mGrid->VisibleRow(3,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
				break;
      }
			//if(stav==2 && E->mGrid->Rows[3].Visible){E->mGrid->VisibleRow(3,false,false); E->mGrid->VisibleRow(5,false,false); E->mGrid->VisibleRow(6,false,false);E->mGrid->exBUTTON->GlyphOptions->Kind=scgpbgkDownArrow;}
			break;
		}
		case 7:case 11:case 15:case 101:case 105:
		case 1://robot (kontinuální)
		{
      //načtení popisků
			AnsiString t="LO ",t_hint="délka lakovácího okna";
			if(E->eID!=1){t="PO ";t_hint="délka pracovní oblasti";}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,5);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;
			E->mGrid->Cells[1][1].Text=outPT(E->PT1);
			E->mGrid->Cells[0][2].Text=t+LO;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outLO(E->LO1);
			E->mGrid->Cells[0][3].Text="vyosení "+LO;
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outLO(E->LO_pozice);
			E->mGrid->Cells[1][3].isNegativeNumber->Color=(TColor)RGB(43,87,154);//musí být nastaveno ručně, vymazání podmíněného formátování
			E->mGrid->Cells[0][4].Text="PD jig";
			E->mGrid->Cells[1][4].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_1;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			if(E->eID>100)E->mGrid->Cells[0][2].Hint="délka pracovní oblasti"; else E->mGrid->Cells[0][2].Hint="délka lakovácího okna";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="vyosení lakovácího okna";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="part detect";
			E->mGrid->Cells[0][4].ShowHint=true;
			break;
		}
		case 8:case 12:case 16:case 102:case 106:
		case 2://robot se stop stanicí
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,4);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->PT1);
			E->mGrid->Cells[0][2].Text="RT "+cas;E->mGrid->Cells[1][2].Text=outPT(E->RT);
			E->mGrid->Cells[0][3].Text="max WT "+cas;E->mGrid->Cells[1][3].Text=outPT(E->WT);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_2;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint="časová rezerva";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="maximální možná doba čekání na palec";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 9:case 13:case 17:case 103:case 107:
		case 3://robot s pasivní otočí
		{
      //načtení popisků
			AnsiString t="LO ",t_hint="délka lakovácího okna";
			if(E->eID!=3){t="PO ";t_hint="délka pracovní oblasti";}
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,9);
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->PT1);
			E->mGrid->Cells[0][2].Text=t+LO;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outLO(E->LO1);
			E->mGrid->Cells[0][2].BottomBorder->Width=2;
			E->mGrid->Cells[1][2].BottomBorder->Width=2;
			E->mGrid->Cells[0][3].Text="rotace [°]";
			E->mGrid->Cells[1][3].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][4].Text="otoč "+cas;
			E->mGrid->Cells[1][4].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][5].Text="otoč "+delka_otoce;
			E->mGrid->Cells[1][5].Type=E->mGrid->EDIT;E->mGrid->Cells[1][5].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[0][5].BottomBorder->Width=2;
			E->mGrid->Cells[1][5].BottomBorder->Width=2;
			E->mGrid->Cells[0][6].Text="PT2 "+cas;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outPT(E->PT2);
			if(E->eID>100)E->mGrid->Cells[0][7].Text="PO2 "+LO; else E->mGrid->Cells[0][7].Text="LO2 "+LO;
			E->mGrid->Cells[1][7].Type=E->mGrid->EDIT;E->mGrid->Cells[1][7].Text=outLO(E->LO2);
			E->mGrid->Cells[0][7].BottomBorder->Width=2;
			E->mGrid->Cells[1][7].BottomBorder->Width=2;
			E->mGrid->Cells[0][8].Text="PD jig";
			E->mGrid->Cells[1][8].Type=E->mGrid->COMBO;
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=81;//sirka_3;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][2].Hint=t_hint;
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint="délka otoče";
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][6].ShowHint=true;
			if(E->eID>100)E->mGrid->Cells[0][7].Hint="délka pracovní oblasti"; else E->mGrid->Cells[0][7].Hint="délka lakovácího okna";
			E->mGrid->Cells[0][7].ShowHint=true;
			E->mGrid->Cells[0][8].Hint="part detect";
			E->mGrid->Cells[0][8].ShowHint=true;
			break;
		}
		case 10:case 14:case 18:case 104:case 108:
		case 4://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,7);
//			E->mGrid->DefaultCell.isLink->Color=clFontRight;//přiřazení barvy fontu
			//definice buněk
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[1][1].Type=E->mGrid->EDIT;E->mGrid->Cells[1][1].Text=outPT(E->PT1);
			E->mGrid->Cells[0][2].Text="rotace [°]";
			E->mGrid->Cells[1][2].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][3].Text="PTo "+cas;
			E->mGrid->Cells[1][3].Type=E->mGrid->EDIT;E->mGrid->Cells[1][3].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][4].Text="PT2 "+cas;
			E->mGrid->Cells[1][4].Type=E->mGrid->EDIT;E->mGrid->Cells[1][4].Text=outPT(E->PT2);
			E->mGrid->Cells[0][5].Text="RT "+cas;
			E->mGrid->Cells[1][5].Text=outPT(E->RT);
			E->mGrid->Cells[0][6].Text="WT "+cas;
			E->mGrid->Cells[1][6].Type=E->mGrid->EDIT;E->mGrid->Cells[1][6].Text=outPT(E->WT);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=78;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][1].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][1].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			E->mGrid->Cells[0][4].Hint="celkový čas procesu, který je složen z dílčích časů";// (přesun robota, aretace, lakování, číštění pistole,...)";
			E->mGrid->Cells[0][4].ShowHint=true;
			E->mGrid->Cells[0][5].Hint="časová rezerva";
			E->mGrid->Cells[0][5].ShowHint=true;
			E->mGrid->Cells[0][6].Hint="maximální možná doba čekání na palec";
			E->mGrid->Cells[0][6].ShowHint=true;
			break;
		}
		case 5://otoč pasivní
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,4);
			//definice buněk
			E->mGrid->Cells[0][1].Text="rotace [°]";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
			E->mGrid->Cells[0][2].Text="délka "+delka_otoce;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outDO(E->OTOC_delka);
			E->mGrid->Cells[0][3].Text="PT "+cas;//PT u pasivní nelze zadat
			E->mGrid->Cells[1][3].Text=outPT(E->PTotoc);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_56;//Delší text
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
			E->mGrid->Cells[0][2].Hint="délka otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
			//samotné vytvoření matice-tabulky
			E->mGrid->Create(2,4);
			//definice buněk
			E->mGrid->Cells[0][1].Text="rotace [°]";
			E->mGrid->Cells[1][1].Type=E->mGrid->COMBO;
//			E->mGrid->Cells[0][2].Text="délka "+delka_otoce;//D u aktivní nelze zadat
//			E->mGrid->Cells[1][2].Text=outDO(E->OTOC_delka);//původně EDIT, ale background lze nastavit pouze pro text, EDIT se jen slabě orámuje
			E->mGrid->Cells[0][2].Text="PT "+cas;
			E->mGrid->Cells[1][2].Type=E->mGrid->EDIT;E->mGrid->Cells[1][2].Text=outPT(E->PTotoc);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[1][3].Text=outPT(E->RT);
			//automatické nastavení sířky sloupců podle použitých jednotek
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=sirka_56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			//nastavení hintů
//			E->mGrid->Cells[0][2].Hint="délka otoče";
//			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][2].Hint="celkový čas procesu otoče";
			E->mGrid->Cells[0][2].ShowHint=true;
			E->mGrid->Cells[0][3].Hint="časová rezerva";
			E->mGrid->Cells[0][3].ShowHint=true;
			break;
		}
		case 200://předávací místo
		{
			E->mGrid->Create(2,2);
			E->mGrid->Cells[0][1].Text="WT palec "+cas;
			E->mGrid->Cells[1][1].Text=outPT(E->WT);
			E->mGrid->SetColumnAutoFit(-4);
			E->mGrid->Columns[0].Width=120;
			E->mGrid->Columns[1].Width=sirka_cisla;
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
}
//---------------------------------------------------------------------------
//slouží k vyčtení stávajícího nastavení jednotek, k jejich úpravě a zanesení do INI
void TForm1::redesign_element()
{
  log(__func__);//logování
	FormX->vstoupeno_elm=false;//zabrání spouštění OnChange při změně jednotek
	FormX->vstoupeno_poh=false;
	AnsiString delka_otoce,LO,cas;
	short sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla;
	//když není font roboto nutno rozšířit buňky s textem
	if(aFont->Name!="Roboto Cn"){sirka_0=10;sirka_1=10;sirka_2=10;sirka_3=10;sirka_4=10;sirka_56=10;}
	bool zcas=false,zLO=false,zdelka_otoce=false;
	//zjištění požadavku (co změnit)
	switch (pom_element->eID)
	{
		case 1:case 7:case 11:case 15:case 101:case 105:
		{
			if (JID==101) zcas=true;//časové buňky
			if (JID==102 || JID==103) zLO=true;//délkové buňky
			break;
		}
		case 2:case 8:case 12:case 16:case 102:case 106:
		{
			zcas=true;//obsahuje pouze casové buňky
			break;
		}
		case 3:case 9:case 13:case 17:case 103:case 107:
		{
			if (JID==101 || JID==104 || JID==106) zcas=true;//čas
			if (JID==102 || JID==107) zLO=true;//delka
			if (JID==105) zdelka_otoce=true;//delka otoče
			break;
		}
		case 0 :case 4:case 10:case 14:case 18:case 104:case 108:case 200:
		{
			zcas=true;//pouze čas
			break;
		}
		case 5:
		{
			if (JID==102) zdelka_otoce=true;//délka
			if (JID==103) zcas=true;//čas
			break;
		}
		case 6:
		{
			//if (JID==102) zdelka_otoce=true;//délka
			if (JID==102 || JID==103) zcas=true;//čas
			break;
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
	//nastavení jednotek podle posledních nastavení
	if (PTunit==SEC) cas="<a>[s]</a>";//0
	else cas="<a>[min]</a>";//1
	if (LOunit==M) LO="<a>[m]</a>";//0
	else LO="<a>[mm]</a>";//1
	if (DOtocunit==M) delka_otoce="<a>[m]</a>";//0
	else delka_otoce="<a>[mm]</a>";//1
	//nastavení šířek
	if(PTunit==SEC && LOunit==M) {sirka_1=90;sirka_cisla=70;}
	else {sirka_1=105;sirka_cisla=100;}
	if(PTunit==0) {sirka_0=147; sirka_2=88;sirka_4=59;sirka_cisla=70;}
	else {sirka_0=149; sirka_2=107;sirka_4=78;sirka_cisla=100;}
	if(DOtocunit==M && PTunit==SEC) {sirka_56=76;sirka_cisla=70;}
	else {sirka_56=90;sirka_cisla=100;}
	if(PTunit==SEC && LOunit==M && DOtocunit==M) {sirka_3=68;sirka_cisla=70;}
	else {sirka_3=81;sirka_cisla=100;}
	//procházení pomocného spojitého seznamu
	Cvektory::TElement *E=pom_temp->elementy->dalsi;//zde lze přeskočit hlavičku
	while (E!=NULL)
	{
		akt_tabulek(E,LO,delka_otoce,cas,sirka_0,sirka_1,sirka_2,sirka_3,sirka_4,sirka_56,sirka_cisla);
		E=E->dalsi;
	}
	E=NULL; delete E; ;
  //zápis změn do INI
	writeINI("nastaveni_nahled", "cas", PTunit);
	writeINI("nastaveni_nahled", "LO", LOunit);
	writeINI("nastaveni_nahled", "delka_otoce", DOtocunit);
	REFRESH();
	nahled_ulozit(true);
}
//---------------------------------------------------------------------------
//přepisuje jednotky a upravuje šířku sloupců
void TForm1::akt_tabulek (Cvektory::TElement *E,AnsiString LO,AnsiString delka_otoce,AnsiString cas,short sirka_0,short sirka_1,short sirka_2,short sirka_3,short sirka_4,short sirka_56,short sirka_cisla)
{
  log(__func__);//logování
	switch(E->eID)
	{
		case 0://stop stanice
		{
			E->mGrid->Cells[0][2].Text="RT "+cas;
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->RT),3);
			E->mGrid->Cells[0][3].Text="max. WT stop "+cas;
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->WTstop),3);
			E->mGrid->Cells[0][4].Text="WT palec "+cas;
			E->mGrid->Cells[1][4].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Columns[0].Width=sirka_0;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 1:case 7:case 11:case 15:case 101:case 105://robot (kontinuální)
		{
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[0][2].Text="LO "+LO;
			E->mGrid->Cells[0][3].Text="vyosení "+LO;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->PT1),3);
			E->mGrid->Cells[1][2].Text=m.round2double(outLO(E->LO1),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outLO(E->LO_pozice),3);
			E->mGrid->Columns[0].Width=sirka_1;
			if(sirka_cisla<100)sirka_cisla+=15;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 2:case 8:case 12:case 16:case 102:case 106://robot se stop stanicí
		{
			E->mGrid->Cells[0][1].Text="PT "+cas;
			E->mGrid->Cells[0][2].Text="RT "+cas;
			E->mGrid->Cells[0][3].Text="max WT "+cas;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->PT1),3);
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->RT),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Columns[0].Width=sirka_2;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 3:case 9:case 13:case 17:case 103:case 107://robot s pasivní otočí
		{
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[0][2].Text="LO1 "+LO;
			E->mGrid->Cells[0][4].Text="otoč "+cas;
			E->mGrid->Cells[0][5].Text="otoč "+delka_otoce;
			E->mGrid->Cells[0][6].Text="PT2 "+cas;
			E->mGrid->Cells[0][7].Text="LO2 "+LO;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->PT1),3);
			E->mGrid->Cells[1][2].Text=m.round2double(outLO(E->LO1),3);
			E->mGrid->Cells[1][4].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[1][5].Text=m.round2double(outDO(E->OTOC_delka),3);
			E->mGrid->Cells[1][6].Text=m.round2double(outPT(E->PT2),3);
			E->mGrid->Cells[1][7].Text=m.round2double(outLO(E->LO2),3);
			E->mGrid->Columns[0].Width=81;//sirka_3;
			if(sirka_cisla<100)sirka_cisla+=15;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 4:case 10:case 14:case 18:case 104:case 108://robot s aktivní otočí (resp. s otočí a stop stanicí)
		{
			E->mGrid->Cells[0][1].Text="PT1 "+cas;
			E->mGrid->Cells[0][3].Text="PTo "+cas;
			E->mGrid->Cells[0][4].Text="PT2 "+cas;
			E->mGrid->Cells[0][5].Text="RT "+cas;
			E->mGrid->Cells[0][6].Text="WT "+cas;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->PT1),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[1][4].Text=m.round2double(outPT(E->PT2),3);
			E->mGrid->Cells[1][5].Text=m.round2double(outPT(E->RT),3);
			E->mGrid->Cells[1][6].Text=m.round2double(outPT(E->WT),3);
			E->mGrid->Columns[0].Width=78;
			E->mGrid->Columns[1].Width=sirka_cisla;
		break;
		}
		case 5://otoč pasivní
		{
			E->mGrid->Cells[0][2].Text="délka "+delka_otoce;
			E->mGrid->Cells[0][3].Text="PT "+cas;
			E->mGrid->Cells[1][2].Text=m.round2double(outDO(E->OTOC_delka),3);
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Columns[0].Width=sirka_56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 6://otoč aktivní (resp. otoč se stop stanicí)
		{
			//E->mGrid->Cells[0][2].Text="délka "+delka_otoce;//D u aktivní nelze zadat
			E->mGrid->Cells[0][2].Text="PT "+cas;
			//E->mGrid->Cells[1][2].Text=m.round2double(outDO(E->OTOC_delka),3);
			E->mGrid->Cells[1][2].Text=m.round2double(outPT(E->PTotoc),3);
			E->mGrid->Cells[0][3].Text="RT "+cas;
			E->mGrid->Cells[1][3].Text=m.round2double(outPT(E->RT),3);
			E->mGrid->Columns[0].Width=sirka_56;
			E->mGrid->Columns[1].Width=sirka_cisla;
			break;
		}
		case 200:
		{
			E->mGrid->Cells[0][1].Text="WT palec "+cas;
			E->mGrid->Cells[1][1].Text=m.round2double(outPT(E->WT),3);
			break;
    }
	}
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
    if(n==1){ label1= "pasivní"; label2=""; }
    if(n==2){ label1= "aktivní"; label2=""; }
		if(pom_temp->id!=3 && pom_temp->pohon!=NULL)
		{
		  d.vykresli_otoc(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-odsazeniX,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P - 15-odsazeni,label1,label2,n+4,0,180,1);
		}
		else d.vykresli_otoc(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W-odsazeniX,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P - 15-odsazeni,label1,label2,n+4,0,180,-1);
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
  log(__func__);//logování
	short Z=3;//*3 vyplývá z logiky algoritmu antialiasingu
	int W=DrawGrid_ostatni->DefaultColWidth  *Z;
	int H=DrawGrid_ostatni->DefaultRowHeight  *Z;
	int P=-1*DrawGrid_ostatni->TopRow*H;//posun při scrollování, drawgridu nebo při zmenšení okna a scrollování
	int odsazeni=10;

	Cantialising a;
	Graphics::TBitmap *bmp_in=new Graphics::TBitmap;
	bmp_in->Width=DrawGrid_ostatni->Width*Z;bmp_in->Height=DrawGrid_ostatni->Height *Z;//velikost canvasu//*3 vyplývá z logiky algoritmu antialiasingu
	TCanvas* C=bmp_in->Canvas;//pouze zkrácení ukazatelového zápisu/cesty

	double Zoom_back=Zoom;//záloha zoomu
	Zoom=10;//nastavení dle potřeb, aby se robot zobrazil knihovně vždy stejně veliký
	short pocet_elementu=1;
	for(unsigned short n=1;n<=pocet_elementu;n++)
	{
		if(pom_temp->id!=3 && pom_temp->pohon!=NULL)
		{
			d.vykresli_stopku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 2-odsazeni,"STOP","",0,180);
		}
	else d.vykresli_stopku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 2-odsazeni,"STOP","",0,180,-1);


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
  log(__func__);//logování
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
     label1= "text";
     label2="";
		 d.vykresli_ikonu_textu(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W+odsazeni,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1);
   //	d.vykresli_robota(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 30,label1,label2,n);
     }
    if(n==2)
    {
     label1= "šipka";
     label2="";
		 d.vykresli_ikonu_sipky(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W+odsazeni,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20,label1);
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
	log(__func__);//logování
	if(MOD==NAHLED)
	{
		scListGroupKnihovObjektu->Caption="Roboti";
		DrawGrid_knihovna->RowCount=2;  //nastaveni poctu radku, aby nedochazelo k posunu gridu pri scrollovani
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
		AnsiString label2,label_pom;
		short pocet_elementu=4;
		int EID=d.v.vrat_eID_prvniho_pouziteho_robota(pom_temp);
		switch(pom_temp->id)
		{
			case 3:
			{
				DrawGrid_knihovna->ColCount=1;
				if(pom_temp->pohon!=NULL)
				{
					d.vykresli_ikonu_komory(C,140,60,"okap",0);
					d.vykresli_ikonu_komory(C,140,480,"postřikový rám",1);
				}
				else
				{
					d.vykresli_ikonu_komory(C,140,60,"okap",0,-1);
					d.vykresli_ikonu_komory(C,140,480,"postřikový rám",1,-1);
				}
			}break;
			case 0:case 9://objekt navěšování + svěšování
			{
				//prvotní vykreslení všech robotů
				Zoom=15;//zvětšení zoomu při použití robotů
				odsazeni=54;//vycentrování lidských robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				if(pom_temp->id==0)label_pom="navěšování";else label_pom="svěšování";//nastavování zda se jedná o navěšování či svěšování
				for(unsigned short n=101;n<=104;n++)
				{
					//nastavení názvů
					if(n==101){ label1= "kontinuální"; label2=label_pom; }
					if(n==102){ label1= "S&G"; label2=label_pom; }
					if(n==103){ label1= "kontinuální s";  label2="pasiv. otočí"; }
					if(n==104){ label1= "S&G s";  label2="akt. otočí"; }
					if(pom_temp->pohon!=NULL&&EID==-1)
					{
						d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n);
					}
					else d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if((EID==101 || EID==103 || EID==5) && pom_temp->pohon!=NULL)
				{
					C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální",label_pom,101);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,102,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",103);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",104,0,0,-1);
				}
				else if ((EID==102 || EID==104 || EID==6) && pom_temp->pohon!=NULL)
				{
          C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální",label_pom,101,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G",label_pom,102);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",103,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",104);
				}
			}break;
			case 1://objekt CO2
			{
				//prvotní vykreslení všech robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				for(unsigned short n=15;n<=18;n++)
				{
					//nastavení názvů
					if(n==15){ label1= "kontinuální"; label2="CO2"; }
					if(n==16){ label1= "S&G"; label2="CO2"; }
					if(n==17){ label1= "kontinuální s";  label2="pasiv. otočí"; }
					if(n==18){ label1= "S&G s";  label2="akt. otočí"; }
					if(pom_temp->pohon!=NULL&&EID==-1)
					{
						d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n);
					}
					else d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if((EID==15 || EID==17 || EID==5) && pom_temp->pohon!=NULL)
				{
					C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","CO2",15);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","CO2",16,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",17);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",18,0,0,-1);
				}
				else if ((EID==16 || EID==18 || EID==6) && pom_temp->pohon!=NULL)
				{
          C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","CO2",15,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","CO2",16);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",17,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",18);
				}
			}break;
			case 2://objekt ožeh
			{
        //funkce přepínání mezi roboty  lidskými roboty
				unsigned short n_od,n_do,rob1,rob2,rob3,rob4;
				if(scGPSwitch_robot_clovek->State==0){n_od=11;n_do=14;rob1=11;rob2=12;rob3=13;rob4=14;Zoom=10;odsazeni=19;}
				else{n_od=105;n_do=108;rob1=105;rob2=106;rob3=107;rob4=108;Zoom=15;odsazeni=54;}
				//prvotní vykreslení všech robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				for(unsigned short n=n_od;n<=n_do;n++)
				{
					//nastavení názvů
					if(n==rob1){ label1= "kontinuální"; label2="ožeh"; }
					if(n==rob2){ label1= "S&G"; label2="ožeh"; }
					if(n==rob3){ label1= "kontinuální s";  label2="pasiv. otočí"; }
					if(n==rob4){ label1= "S&G s";  label2="akt. otočí"; }
					if(pom_temp->pohon!=NULL&&EID==-1)
					{
						d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n);
					}
					else d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if((EID==11 || EID==13 || EID==105 || EID==107 || EID==5) && pom_temp->pohon!=NULL)
				{
          C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","ožeh",rob1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","ožeh",rob2,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",rob3);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",rob4,0,0,-1);
				}
				else if ((EID==12 || EID==14 || EID==106 || EID==108 || EID==6) && pom_temp->pohon!=NULL)
				{
          C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","ožeh",rob1,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","ožeh",rob2);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",rob3,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",rob4);
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
				for(unsigned short n=n_od;n<=n_do;n++)
				{
					//nastavení názvů
					if(n==7||n==105){ label1= "kontinuální"; label2="ionizace"; }
					if(n==8||n==106){ label1= "S&G"; label2="ionizace"; }
					if(n==9||n==107){ label1= "kontinuální s";  label2="pasiv. otočí"; }
					if(n==10||n==108){ label1= "S&G s";  label2="akt. otočí"; }
					if(pom_temp->pohon!=NULL&&EID==-1)
					{
						d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0);
					}
					else d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				if((EID==7 || EID==9 || EID==105 || EID==107 || EID==5) && pom_temp->pohon!=NULL)
				{                                                                                                                            //S&G
					C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","ionizace",rob1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","ionizace",rob2,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",rob3);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",rob4,0,0,-1);
				}
				else if ((EID==8 || EID==10 || EID==106 || EID==108 || EID==6) && pom_temp->pohon!=NULL)
				{
          C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","ionizace",rob1,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","ionizace",rob2);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",rob3,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",rob4);
				}
			}break;
			case 5://objekt lakování
			{
        //funkce přepínání mezi roboty  lidskými roboty
				unsigned short n_od,n_do,rob1,rob2,rob3,rob4;
				if(scGPSwitch_robot_clovek->State==0){n_od=1;n_do=4;rob1=1;rob2=2;rob3=3;rob4=4;Zoom=10;odsazeni=19;}
				else{n_od=105;n_do=108;rob1=105;rob2=106;rob3=107;rob4=108;Zoom=15;odsazeni=54;}
				//prvotní vykreslení všech robotů
				int i=1;//použití z důvodu kopírování algoritmu z lakovny, kde tuto funkci zastává n z for cyklu
				for(unsigned short n=n_od;n<=n_do;n++)
				{
					if(n==rob1){ label1= "kontinuální"; label2="lakování"; }
					if(n==rob2){ label1= "S&G"; label2="lakování"; }
					if(n==rob3){ label1= "kontinuální s";  label2="pasiv. otočí"; }
					if(n==rob4){ label1= "S&G s";  label2="akt. otočí"; }
					if(pom_temp->pohon!=NULL&&EID==-1)
					{
						d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0);
					}
					else d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((i+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(i/2.0)-1)*H+P+30-odsazeni,label1,label2,n,0,0,-1);
					i++;
				}
				//změna vykreslení robotů v případě vložení elementu
				if((EID==1 || EID==3 || EID==105 || EID==107 || EID==5) && pom_temp->pohon!=NULL)
				{
					C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","lakování",rob1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","lakování",rob2,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",rob3);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",rob4,0,0,-1);
				}
				else if ((EID==2 || EID==4 || EID==106 || EID==108 || EID==6) && pom_temp->pohon!=NULL)
				{
					C->Pen->Color=clWhite;//nastavení barev pro vykreslení obdelníku
					C->Brush->Color=clWhite;//musí být těsně před C->Rectangle
					C->Rectangle(0,0,2*W,2*H);//překreslní původních robotů ("smazání")
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P+30-odsazeni,"kontinuální","lakování",rob1,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P+30-odsazeni,"S&G","lakování",rob2);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((3+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(3/2.0)-1)*H+P+30-odsazeni,"kontinuální s","pasiv. otočí",rob3,0,0,-1);
					d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((4+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(4/2.0)-1)*H+P+30-odsazeni,"S&G s","akt. otočí",rob4);
				}
			}break;
		}
		Zoom=Zoom_back;//návrácení původního zoomu
		Graphics::TBitmap *bmp_out=a.antialiasing(bmp_in);//velice nutné do samostatné bmp, kvůli smazání bitmapy vracené AA
		DrawGrid_knihovna->Canvas->Draw(0,0,bmp_out);
		delete (bmp_out);//velice nutné
		delete (bmp_in);//velice nutné
	}
//	if(MOD==SCHEMA)
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
	if(MOD==NAHLED)
  {
		bool pridani=false;
		if(editace_textu)	Smaz_kurzor();//pokud se vykresluje kurzor a je kluknuto do knihovny ... smazat kurzor
		if(MOD==NAHLED)if(PmG->Rows[3].Visible)FormX->odstranit_korelaci();//pokud je tabulka pohonu v režimu KK je možnost, že je Highlightovaná ... odstranit highlight (korelaci)
		knihovna_id=1;
		int EID=d.v.vrat_eID_prvniho_pouziteho_robota(pom_temp);//kontrola v jakém je kabina režimu (stop&go, kontinuální), podle toho dovolí vkládat roboty pouze stejného režimu
    //pro každý objekt jiná podmínka přidání a jiný způsob přiřazení eID
		switch(pom_temp->id)
		{
			case 0:case 9://navěšování + svěšování
			{
				if(Row==0)element_id=Col+101;
				if(Row==1)element_id=Col+103;
				if(((EID==101||EID==103)&&(element_id==101||element_id==103)||(EID==102||EID==104)&&(element_id==102||element_id==104)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
					pridani=true;
			}break;
			case 1://CO2
			{
				if(Row==0)element_id=Col+15;
				if(Row==1)element_id=Col+17;
				if(((EID==15||EID==17)&&(element_id==15||element_id==17)||(EID==16||EID==18)&&(element_id==16||element_id==18)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
					pridani=true;
			}break;
			case 2://ožeh
			{
        if(scGPSwitch_robot_clovek->State==0)//robot
				{
					if(Row==0)element_id=Col+11;
					if(Row==1)element_id=Col+13;
					if(((EID==11||EID==13)&&(element_id==11||element_id==13)||(EID==12||EID==14)&&(element_id==12||element_id==14)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
						pridani=true;
				}
				else//člověk
				{
        	if(Row==0)element_id=Col+105;
					if(Row==1)element_id=Col+107;
					if(((EID==11||EID==13||EID==105||EID==107)&&(element_id==105||element_id==107)||(EID==8||EID==10||EID==106||EID==108)&&(element_id==106||element_id==108)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
						pridani=true;
        }
			}break;
			case 3://POW
			{
				if(Row==0)element_id=-1;
				if(Row==1)element_id=-2;
				if(pom_temp->pohon!=NULL)pridani=true;
			}break;
			case 4://objekt ionizace
			{
				if(scGPSwitch_robot_clovek->State==0)//robot
				{
					if(Row==0)element_id=Col+7;
					if(Row==1)element_id=Col+9;
					if(((EID==7||EID==9||EID==105||EID==107)&&(element_id==7||element_id==9)||(EID==8||EID==10||EID==106||EID==108)&&(element_id==8||element_id==10)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
						pridani=true;
				}
				else//člověk
				{
					if(Row==0)element_id=Col+105;
					if(Row==1)element_id=Col+107;
					if(((EID==7||EID==9||EID==105||EID==107)&&(element_id==105||element_id==107)||(EID==8||EID==10||EID==106||EID==108)&&(element_id==106||element_id==108)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
						pridani=true;
				}
			}break;
			case 5://objekt lakovna
			{

        if(scGPSwitch_robot_clovek->State==0)//robot
				{
					if(Row==0)element_id=Col+1;
					if(Row==1)element_id=Col+3;
					if(((EID==1||EID==3)&&(element_id==1||element_id==3)||(EID==2||EID==4)&&(element_id==2||element_id==4)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
						pridani=true;
				}
				else//člověk
				{
					if(Row==0)element_id=Col+105;
					if(Row==1)element_id=Col+107;
					if(((EID==1||EID==3||EID==105||EID==107)&&(element_id==105||element_id==107)||(EID==8||EID==10||EID==106||EID==108)&&(element_id==106||element_id==108)||EID==-1||(funkcni_klavesa==2&&DEBUG))&&pom_temp->pohon!=NULL)//při stisku shift lze tuto podmínku v debugu obejít
						pridani=true;
				}
			}break;
		}
    //pokud byly splněné podmínky pro přidání, zapne událost ADD (vystrčeno mimo z důvodu množení kódu)
		if(pridani)
		{
			SB("Kliknutím na libovolné místo umístíte vybraný element.");
			Akce=ADD;kurzor(add_o);
		}
		else//pokud bude při již spuštěné akci kliknuto na element, který být přidanný nesmí, je nutné akci vypnout
		{
			Akce=NIC;kurzor(standard);
			TIP="Přidávání prvků je možné až po výběru pohonu";
			refresh_mGrid=false;REFRESH();refresh_mGrid=true;
    }
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
  log(__func__);//logování
	if(editace_textu)	Smaz_kurzor();
	if(MOD==NAHLED)if(PmG->Rows[3].Visible)FormX->odstranit_korelaci();
	int Col,Row;
	Col=DrawGrid_otoce->Col; Row=DrawGrid_otoce->Row;
	knihovna_id=2;
	if(Row==0) element_id=Col+5;
	if(pom_temp->pohon!=NULL && pom_temp->id!=3)
	{
		SB("Kliknutím na libovolné místo umístíte vybraný element.");
		Akce=ADD;kurzor(add_o);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_ostatniMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
  log(__func__);//logování
	if(editace_textu)	Smaz_kurzor();
	if(MOD==NAHLED)if(PmG->Rows[3].Visible)FormX->odstranit_korelaci();
	if(pom_temp->pohon!=NULL && pom_temp->id!=3)
	{
		int Row;
		Row=DrawGrid_ostatni->Row;
		knihovna_id=3;
		if(Row==0)  element_id=0;
		SB("Kliknutím na libovolné místo umístíte vybraný element.");
		Akce=ADD;kurzor(add_o);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawGrid_geometrieMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{
	log(__func__);//logování
	int Col,Row;
	Col=DrawGrid_geometrie->Col; Row=DrawGrid_geometrie->Row;
	knihovna_id=4;
	if(pom_temp->id==4&&pom_temp->pohon!=NULL)//zatím jen v ionizaci
	{
		if(Col==0)element_id=100;
		else element_id=200;
		SB("Kliknutím na libovolné místo umístíte vybraný element.");
		Akce=ADD;kurzor(add_o);
	}
	else
	{
		if(Akce!=GEOMETRIE && Col==0 && pom->id!=3)//zapnutí akce geometrie
		{
	  	Akce=GEOMETRIE;
	  	editace_geometrie_spustena=false;
	  	//pokud ještě nebyla editována geometrie a existuje předchozí element, zapíše ho do posledni_editovany_element
	  	//if(pom->predchozi!=NULL && posledni_editovany_element==NULL)posledni_editovany_element=pom->predchozi->elementy->predchozi;
			REFRESH(false);
		}
		else {Akce=NIC;Akce_temp=NIC;REFRESH(false);}//vypunutí akce geometrie
		if(Col==1 && pom_temp->pohon!=NULL)
		{
			element_id=200;
			SB("Kliknutím na libovolné místo umístíte vybraný element.");
			Akce=ADD;kurzor(add_o);
    }
	}
}
//---------------------------------------------------------------------------
//přeposílá událost na form
void __fastcall TForm1::DrawGrid_knihovnaMouseWheelDown(TObject *Sender, TShiftState Shift,
					TPoint &MousePos, bool &Handled)
{
  log(__func__);//logování
	//SB("down");
	FormMouseWheelDown(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------
//přeposílá událost na form
void __fastcall TForm1::DrawGrid_knihovnaMouseWheelUp(TObject *Sender, TShiftState Shift,
					TPoint &MousePos, bool &Handled)
{
  log(__func__);//logování
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
			case 0:SB("přichytávat?",5);grid=true;myMessageBox->CheckBox_pamatovat->Checked=false;break;
			case 1:SB("přichytávat",5);grid=true;break;//přichytí automaticky
			case 2:SB("nepřichytávat",5);break;//automaticky nepřichyt
		}
}
//---------------------------------------------------------------------------
//přepínání dotazu, přichytávání, nepřychtávání k mřížce
void __fastcall TForm1::RzStatusPane5Click(TObject *Sender)
{
    log(__func__);//logování
		if(grid)//aby se po zobrazení mřížky zachoval stavající stav,tedy nebude zohledněn další if
		if(++prichytavat_k_mrizce==3)prichytavat_k_mrizce=0;

		akutalizace_stavu_prichytavani_vSB();
		Invalidate();
}
//---------------------------------------------------------------------------
//přepíná mody zobrazení
void __fastcall TForm1::RzStatusPane1Click(TObject *Sender)
{
    log(__func__);//logování
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
 log(__func__);//logování
 Close();//ukončí aplikaci
}
//---------------------------------------------------------------------------
//Zavreni programu volbou krizek + reaguje na reakci zavirani formulare pri Konec1Click
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  log(__func__);//logování
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
    log(__func__,"CanClose");//logování
    d.v.vymaz_seznam_KATALOG();
		//pro ochranu v případě pádu programu
		//TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
		writeINI("Konec","status","OK");
		//zápis dalšího nastavení
		writeINI("Nastaveni_app","prichytavat",prichytavat_k_mrizce);
		writeINI("Nastaveni_app","ortogonalizace",(short)ortogonalizace_stav);
		//zatím nepoužíváme writeINI("Nastaveni_app","status",STATUS);
    delete aFont; aFont=NULL;
    delete LogFileStream;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::Smazat1Click(TObject *Sender)
{
  log(__func__);//logování
	switch (MOD)
	{
		case NAHLED:       //kvůli MB hází při mazání z popUP paměťovou chybu
		{
			if(pom_element!=NULL)//případ mazání elementu
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu smazat "+pom_element_temp->name.UpperCase()+"?","",MB_YESNO))
				{
					int eID=pom_element_temp->eID;
					Cvektory::TElement *dalsi_element=NULL;
		  		if(pom_element_temp->dalsi!=NULL&&pom_temp->rezim==0)dalsi_element=pom_element_temp->dalsi;//pokud je kabina ve S&G režimu a za smazaným elementem se nachází další element, je nutné dalšímu přepočítat RT
		  		d.v.smaz_element(pom_element_temp);
					Akce=NIC;
					REFRESH();
					DuvodUlozit(true);
		  		nahled_ulozit(true);
		  		DrawGrid_knihovna->Refresh();
					DrawGrid_otoce->Refresh();
		  		pom_element_temp=NULL; delete pom_element_temp;
					if(eID%2!=0 && eID!=200)odstraneni_elementu_tab_pohon(1);//přenastavení tabulky pohonu po odstranění elementu
					if(dalsi_element!=NULL && dalsi_element->eID!=MaxInt)//pokud existuje další element za smazaným dojde k přepočítání jeho RT
						d.v.posuv_aktualizace_RT(dalsi_element);
		  		if(pom_temp->elementy->dalsi!=NULL)d.v.uprav_popisky_elementu(pom_temp,pom_temp->elementy->dalsi);//pokud jsou v kabině jěště nějaké elementy dojde k přejmenování
		  		pom_element=NULL;//přidáno nově 13.5.2019 - v režimu testování kvůli setJobID a předání do pom_element_puv
					if(eID%2==0 && eID!=100 && eID!=200 && eID!=MaxInt)d.v.aktualizuj_sparovane_ukazatele();//odstraněn stop-element, nutná aktualizace
					dalsi_element=NULL;delete dalsi_element;
				}else {mazani=false;Akce=NIC;}
			}
			if(pom_bod_temp!=NULL)//mazání bodu v obrysu kabiny
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu smazat bod č. "+AnsiString(pom_bod_temp->n),"",MB_YESNO))
				{
					//pokud se jedná o druhý bod, smaže automaticky i bod první
					if(pom_temp->body->predchozi->n==2)d.v.smaz_bod(pom_bod_temp->predchozi,pom_temp);
					d.v.smaz_bod(pom_bod_temp,pom_temp);
					pom_bod_temp=NULL;delete pom_bod_temp;
					refresh_mGrid=false;
					REFRESH();
					refresh_mGrid=true;
					DuvodUlozit(true);
					nahled_ulozit(true);
				}
			}
			if(pom_komora_temp!=NULL && pom_temp->komora->predchozi->n>2)//případ mazání kabiny
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu smazat sekci číslo "+AnsiString(pom_komora_temp->n)+"?","",MB_YESNO))
				{
					int n=pom_komora_temp->n;
					d.v.smaz_komoru(pom_temp,pom_komora_temp);
					pom_komora_temp=NULL;delete pom_komora_temp;
					DuvodUlozit(true);
					refresh_mGrid=false;  editace_textu=true;
					REFRESH();
					refresh_mGrid=true;
				}
				Akce=NIC;
			}
			else if(pom_komora_temp!=NULL)//mazání komory, v objektu už bylo dosaženo minimálního množství komor
			{
				MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Nelze smazat sekci, v objektu musí být nejméně dvě sekce!","",MB_OK);
				Akce=NIC;
			}
			break;
		}
		default://SCHEMA
		{
			//ať to nemusí znovu hledat beru z pom Cvektory::TObjekt *p=d.v.najdi_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width,d.O_height);
			if(pom!=NULL)//pokud byl prvek nalezen
			{
				Cvektory::TZakazka *Z=d.v.obsahuje_segment_cesty_objekt(pom);
				if(Z!=NULL)
					MB("Nelze smazat objekt, který je součástí technologické cesty zakázky např.: "+UnicodeString(Z->name));
				else
				{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu objekt "+pom->name.UpperCase()+" smazat?","",MB_YESNO))
				{
					if(((long)pom->id==VyID||(long)pom->id==pocet_objektu_knihovny+1)&&(pom->dalsi2!=pom->predchozi2))
					{
						//pokud není sekundární větev prázdná, musím smazat vše co je v ní
						Cvektory::TObjekt *smaz=NULL;
						if((long)pom->id==VyID)smaz=pom->dalsi2;//rozlišení pokud mažu z výhybky nebo spojky
						else smaz=pom->dalsi2->dalsi2;
						while(pom->dalsi2!=pom->predchozi2)
						{
							d.v.smaz_objekt(smaz);
							smaz=smaz->dalsi;
						}
						smaz=NULL;delete smaz;
					}
					d.v.smaz_objekt(pom);//nalezeny můžeme odstranit odstranit
					//pokud byla mazána výhybka nebo spojka je nutné přejmenovat zbylé výhybky
					if((long)pom->id==VyID||(long)pom->id==pocet_objektu_knihovny+1){d.v.nove_indexy(true);d.v.pocet_vyhybek--;d.v.nove_nazvy();}
					else d.v.nove_indexy();
					pom=NULL;//delete p; nepoužívat delete je to ukazatel na ostra data
          if(d.v.OBJEKTY->dalsi==NULL){Nahled->Enabled=false;posledni_editovany_objekt=NULL;}//zakázání záložky editace pokud již neexistuje žádný objekt
					REFRESH();
					DuvodUlozit(true);
				}
				}
			}
			else if(pom_bod_temp!=NULL)//mazání bodu haly nebo objektu
			{
				if(mrYes==MB(akt_souradnice_kurzoru_PX.x+10,akt_souradnice_kurzoru_PX.y+10,"Chcete opravdu smazat bod č. "+AnsiString(pom_bod_temp->n),"",MB_YESNO))
				{
					//pokud se jedná o druhý bod, smaže automaticky i bod první
					if(d.v.HALA.body!=NULL&&d.v.HALA.body->predchozi->n==2)d.v.smaz_bod(pom_bod_temp->predchozi);
					d.v.smaz_bod(pom_bod_temp);
					pom_bod_temp=NULL;delete pom_bod_temp;
					REFRESH();
					DuvodUlozit(true);
				}
			}
			else S("nenalezen");
			break;
		}
	}
}
//---------------------------------------------------------------------------
//zobrazí paramety jednoho procesu na časových osách
void __fastcall TForm1::Zobrazitparametry1Click(TObject *Sender)
{
  log(__func__);//logování
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
void __fastcall TForm1::NastavitparametryClick1Click(TObject *Sender)
{
	log(__func__);
	if(MOD==SCHEMA && pom_bod_temp==NULL){pom=pom_vyhybka;if(pom!=NULL)NP_input();}//předání z pomocného ukazatele objekt do pom, zabrání ztráty pom, pokud uživatel sjede kurzorem mimo objekt při otevřeném popup + ochrana proti prázdnému pom (asi zbytečná)
	if(pom_bod_temp!=NULL && pom_temp==NULL && pom==NULL)//přidání bodu haly
	{
		if(pom_bod_temp->n!=1)d.v.vloz_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom,pom_bod_temp->predchozi);
		else d.v.vloz_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom);
		REFRESH();
	}
	if(pom_temp!=NULL && pom_komora_temp!=NULL)//změna typu kabiny
	{
		if(pom_komora_temp->typ==0)pom_komora_temp->typ=1;
		else pom_komora_temp->typ=0;
		refresh_mGrid=false;
		REFRESH();
		refresh_mGrid=true;
		nahled_ulozit(true);
	}
	if(pom_temp!=NULL && pom_vyhybka!=NULL && pom_temp->n!=pom_vyhybka->n)//otevírání náhledu z náhledu, přechot na editaci jiného objektu
	{
		zmena_editovaneho_objektu();
	}
	if(pom_temp!=NULL && pom_bod_temp!=NULL)//přidání bodu objektu
	{
		if(pom_bod_temp->n!=1)d.v.vloz_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom_temp,pom_bod_temp->predchozi);
		else d.v.vloz_bod(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,pom_temp);
		refresh_mGrid=false;
		REFRESH();
		refresh_mGrid=true;
		nahled_ulozit(true);
	}
}
//---------------------------------------------------------------------------
//podpůrná metoda řeší vstupní část dat, vyseparováno, z důvodu toho, že z GAPO aktulizauji případně spuštěné PO a nemohu volat NP, protože to v sobě obsahu ShowModal - vedlo k chybě, nutno řešit převody jednotek
void TForm1::NPin()
{
	log(__func__);//logování
	short CTunit=0;if(Form1->readINI("nastaveni_form_parametry", "CT") == "1") CTunit=1;
	short RDunit=0;if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1")RDunit=1;
	short DDunit=0;if(Form1->readINI("nastaveni_form_parametry", "DD") == "1") DDunit=1;
	short Munit=0;if(Form1->readINI("nastaveni_form_parametry", "DM") == "1")  Munit=1;
	//////////////////////////plnění dat do formu z daného objektu
	////plnění daty
	Form_parametry->scComboBox_rotace->ItemIndex=pom->orientace;//musí být nad aktualizací
	aktualizace_combobox_pohony_v_PO();
	if(pom->pohon!=NULL)Form_parametry->scComboBox_pohon->ItemIndex=pom->pohon->n;else Form_parametry->scComboBox_pohon->ItemIndex=0;//musí být takto separé, protože metoda se volá z více míst
	//předání hodnoty objektů ze souboru resp. strukutry do Form_Parametry v SI jednotkách
	Form_parametry->input_state=Form_parametry->NO;//zakázání akcí vyplývající ze změny editů
	Form_parametry->input_clicked_edit=Form_parametry->empty_klik; //při načítání dat není kliknuto na žádný editbox
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
  log(__func__);//logování
	if(pom!=NULL)
	{
		//////////////////////////plnění dat do formu z daného objektu
		////plnění daty
		Form_parametry->scComboBox_rotace->ItemIndex=pom->orientace;//musí být nad aktualizací
		aktualizace_combobox_pohony_v_PO();
		if(pom->pohon!=NULL)Form_parametry->scComboBox_pohon->ItemIndex=pom->pohon->n;else Form_parametry->scComboBox_pohon->ItemIndex=0;//musí být takto separé, protože metoda se volá z více míst
		//předání hodnoty objektů ze souboru resp. strukutry do Form_Parametry v SI jednotkách
		Form_parametry->input_state=Form_parametry->NO;//zakázání akcí vyplývající ze změny editů
		Form_parametry->input_clicked_edit=Form_parametry->empty_klik; //při načítání dat není kliknuto na žádný editbox
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
				pom->orientace=Form_parametry->scComboBox_rotace->ItemIndex;
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
		pom=NULL;
	}
}
//---------------------------------------------------------------------------
void TForm1::NP_input()
{
	 log(__func__);//logování
	 TIP="";
	 //mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
	 pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
	 scButton_zamek_layoutu->Visible=false;//vypnutí tlačítka pro zámek layoutu
	 Image_knihovna_objektu->Visible=false;//vypnutí komponenty s knihovnou
   scGPButton_zmerit_vzdalenost->Visible=false;//schování měření vzdálenosti
	 JID=-1;//ošetření, s JID se pracuje i v náhledu
	 element_id=99999;//ošetření pro správné zobrazování mgridů
	 pom_bod=NULL;pom_bod_temp=NULL;//s těmito ukazateli pracuje jak náhled tak schéma, ošetření
	 //zablokování OnChange tabulek
	 FormX->input_state=FormX->NO;
	 FormX->vstoupeno_poh=false;
	 FormX->vstoupeno_elm=false;
	 MOD=NAHLED;
	 //založení pomocného tempového ukazatele pro akutálně editovaný objekt a překopírování jeho atributů
	 pom_temp=new Cvektory::TObjekt; pom_temp->pohon=NULL; pom_temp->pohon=new Cvektory::TPohon; pom_temp->elementy=NULL;
	 //zkopíruje atributy objektu bez ukazatelového propojení, kopírování proběhne včetně spojového seznamu elemementu opět bez ukazatelového propojení s originálem, pouze mGrid je propojen
	 d.v.kopiruj_objekt(pom,pom_temp);//pokud elementy existují nakopíruje je do pomocného nezávislého spojáku pomocného objektu
	 posledni_editovany_objekt=pom;
	 DrawGrid_knihovna->SetFocus();
	 mGrid_knihovna->SetVisibleComponents(false);//vypnutí komponent v mgridu
	 popisky_knihovna_nahled(false);//nastavní popisků pro editaci
	 DrawGrid_knihovna->Top=33;
	 kurzor(standard);
	 ////řešení nového zoomu a posunu obrazu pro účely náhldeu
	 //zazálohování hodnot posunu a zoomu
	 Posun_predchozi2=Posun_predchozi=Posun;
	 Zoom_predchozi2=Zoom_predchozi=Zoom;
	 //nastavení zoomu na vhodný náhled
   bool zmena_zoom=false;  //pomocna promena pro dalsi nastaveni - vycentrovani obrazu
   if(Zoom<=4)  { Zoom=5.0;zmena_zoom=true; } //pokud je zoom pri prechodu do editace mensi nebo roven 4, dojde k nastaveni zoomu na 5 a vycentrovani obrazu
	 else { Zoom=Zoom; zmena_zoom=false;} //pokud je v layoutu zoom jiny (vetsi) tak tento zoom ponecham i pro editace a nemenim nijak pozici objektu
	 probehl_zoom=true;
   zneplatnit_minulesouradnice();
	 //prozatim definice kabiny
	 TPoint Centr;
	 Centr.x=(pom_temp->elementy->dalsi->geo.X1+pom_temp->elementy->predchozi->geo.X4)/2.0;
	 Centr.y=(pom_temp->elementy->dalsi->geo.Y1+pom_temp->elementy->predchozi->geo.Y4)/2.0;
	 //vycentrování obrazu na střed
   if(zmena_zoom)
   {
	 Posun.x=Centr.x/m2px-ClientWidth/2/Zoom;
	 Posun.y=-Centr.y/m2px-(ClientHeight-scGPPanel_statusbar->Height-scLabel_titulek->Height)/2/Zoom; //ClientHeight-scGPPanel_bottomtoolbar->Height
   }
	 //vycentruje kurzor na střed monitoru - na X nefunguje přesně, problém při vstupu do editace pomocí záložky (uskakování kurzoru)
	 //if(vycentrovat)Mouse->CursorPos=TPoint(m.L2Px(akt_souradnice_kurzoru.x),m.L2Py(akt_souradnice_kurzoru.y)+vyska_menu);
	 //vycentrovat=true;
	 JID=-1;
	 //DrawGrid_knihovna->Visible=false; //nezobrazí přepozicování elementů
	 DrawGrid_knihovna->DefaultRowHeight=140;
	 if(pom_temp->id==3)DrawGrid_knihovna->DefaultColWidth=160;
	 else DrawGrid_knihovna->DefaultColWidth=80;
	 DrawGrid_knihovna->Left=3;
	 DrawGrid_knihovna->Height=DrawGrid_knihovna->DefaultRowHeight*2; // dle počtu řádků
	 //přesunoto níže k refresh: DrawGrid_knihovna->Invalidate();

	 //objekt ionizace
	 if(pom_temp->id==4 || pom_temp->id==2 || pom_temp->id==5){scGPPanel_pomocn_proSwitch->Visible=true;scGPSwitch_robot_clovek->Visible=true;}

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

	 if(pom_temp->id==4 || pom_temp->id==2 || pom_temp->id==5)scGPLabel_roboti->Caption="Robot           Operátor";//mezery tvoří místo, kde je zobrazen switch
	 else if(pom_temp->id==3)scGPLabel_roboti->Caption="Sekce";
	 else scGPLabel_roboti->Caption="Roboti";

	 scGPLabel_roboti->ContentMarginLeft=10;

	//nastavení tlačítek na výchozí hodnoty
	if(pom_temp->uzamknout_nahled)
	{
		scButton_zamek->ImageIndex=37; //zamčeno
		scButton_zamek->Hint="Odemknout náhled";
	}
	else
	{
		scButton_zamek->ImageIndex=60;
		scButton_zamek->Hint="Zamknout náhled";
	}
	if(pom_temp->zobrazit_mGrid)
	{
		scGPButton_viditelnostmGrid->ImageIndex=54;
		scGPButton_viditelnostmGrid->Hint="Skrýt tabulky";
	}
	else
	{
		scGPButton_viditelnostmGrid->ImageIndex=55;
		scGPButton_viditelnostmGrid->Hint="Zobrazit tabulky";
	}
	if(pom_temp->zobrazit_koty)
	{
		scGPButton_viditelnostKoty->ImageIndex=56;
		scGPButton_viditelnostKoty->Hint="Skrýt kóty";
	}
	else
	{
		scGPButton_viditelnostKoty->ImageIndex=57;
		scGPButton_viditelnostKoty->Hint="Zobrazit kóty";
	}
	if(posun_dalsich_elementu)
	{
		scGPButton_posun_dalsich_elementu->ImageIndex=58;
		scGPButton_posun_dalsich_elementu->Hint="Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";
	}
	else
	{
		scGPButton_posun_dalsich_elementu->ImageIndex=59;
		scGPButton_posun_dalsich_elementu->Hint="Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";
	}
	if(scGPComboBox_prepinacKot->ItemIndex==0)DKunit=1;
	else DKunit=2;
	//nastavení tlačítka pro spouštění animace za podmínky přiřazení pohonu
	if(pom_temp->pohon!=NULL)scGPGlyphButton_PLAY->Enabled=true;
	else scGPGlyphButton_PLAY->Enabled=false;

	 scGPButton_ulozit->Enabled=false;
	 //zapnutí spodního panelu
	 scGPPanel_bottomtoolbar->Visible=true;

	//zmena horní lišty vlevo
	scLabel_architekt->Visible=false;
	scGPSwitch_rezim->Visible=false;
	scLabel_klient->Visible=false;
	scGPGlyphButton_PLAY->Visible=true;
	scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPlay;
	scGPGlyphButton_PLAY->Hint="spustit animaci";

	Schema->Down=false;
	Nahled->Down=true;

	vytvoreni_tab_pohon();
	nahled_ulozen=false;//nově otevřen, není uložen
	DrawGrid_knihovna->Invalidate();
	PmG->Update();
	//znovu provedení designu při otevření náhledu, který není prázdný
	if(pom_temp->elementy!=NULL)
	{
		bool odmazan_pohon=false;
		Cvektory::TElement *E=pom_temp->elementy;
		while (E!=NULL)
		{
			if(E->n>0)
			{
				if(E->pohon!=NULL)//aktualizace pohonu
				{
					Cvektory::TPohon *p=d.v.vrat_pohon(E->pohon->n);
					if(p!=NULL)E->pohon->name=p->name;
					else{E->pohon=NULL;odmazan_pohon=true;}
					p=NULL;delete p;
        }
				design_element(E,false);//znovuvytvoření tabulek
			}
			E=E->dalsi;
		}
		E=NULL; delete E;
		if(odmazan_pohon)vlozit_predavaci_misto();
	}
	pom_element_temp=pom_temp->elementy->dalsi;//pro pořeby editace geometrie
	on_change_zoom_change_scGPTrackBar();//musí být po design_element
	FormX->input_state=FormX->NOTHING;
  REFRESH(); //přidáno kvůli zobrazení tab. pohonů a kót (při shodném zoomu layout->editace)
}
//---------------------------------------------------------------------------
//slouží k přechodu z editace jednoho objektu do editace druhého objektu
void TForm1::zmena_editovaneho_objektu()
{
	log(__func__);//logování
	/////////Uložení náhledu
	if(scGPButton_ulozit->Enabled && MB("Chcete uložit změny v náhledu?",MB_YESNO,true)==mrYes)
	{
    d.v.vymaz_komory(pom);
		d.v.vymaz_elementy(pom,true);
		d.v.kopiruj_objekt(pom_temp,pom);
		DuvodUlozit(true);
		nahled_ulozit(false);
	}

	/////////Uzavření starého náhledu
	if(MOD==NAHLED&&index_kurzoru==9999||index_kurzoru==100)
	smaz_edit(false);//smaže edit a neprovede refresh
	DrawGrid_knihovna->SetFocus();
	if(editace_textu)Smaz_kurzor();//také volá Refresh//smaz_kurzor se zavolá, pouze pokud je to třeba odstraňuje zbytečný problik, dodělal MaKr
	MOD=SCHEMA;//nutné před zoom, ale za smaz kurzor
	//smazání případných komor
	d.v.vymaz_komory(pom_temp);
	//smazání elementů - musí být napočátku, aby nebyl problik
	d.v.vymaz_elementy(pom_temp,true);   //&&pom_temp->elementy->dalsi!=NULL)
	if(!mazani&&scGPButton_ulozit->Enabled)d.v.uprav_popisky_elementu(pom,NULL);//volání přejmenování elementů, pouze v případě kdy je něco v kabině a bylo stisknuto pouze storno, při ulož je stisk strona volán taky
	pom=NULL;//pom->pohon=NULL;delete pom->pohon;pom=NULL; toto nelze, odpřiřadilo by to pohon i na ostrém
	if(pom_temp!=NULL){pom_temp->pohon=NULL;delete pom_temp->pohon;}pom_temp=NULL;delete pom_temp;
	PmG->Delete(); PmG=NULL; delete PmG;
	//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
	pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
	//v případě animace vypnutí a nastavení do výchozího stavu
	Timer_animace->Enabled=false;
	ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
	ButtonPLAY->Hint="spustit animaci";
	zobrazit_meritko=scGPSwitch_meritko->State;//navrácení do původního stavu

	/////////Přenastavení editovaného objektu
	pom=pom_vyhybka;//pom_vyhybka slouží k uložení ukazatele na pro další náhled

	/////////Otevření nového náhledu
	TIP="";
	//mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
	pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
	DrawGrid_knihovna->SetFocus();
	scButton_zamek_layoutu->Visible=false;//vypnutí tlačítka pro zámek layoutu
	Image_knihovna_objektu->Visible=false;//vypnutí komponenty s knihovnou
  scGPButton_zmerit_vzdalenost->Visible=false;
	mGrid_knihovna->SetVisibleComponents(false);//vypnutí komponent v mgridu
	JID=-1;//ošetření, s JID se pracuje i v náhledu
	element_id=99999;//ošetření pro správné zobrazování mgridů
	pom_bod=NULL;pom_bod_temp=NULL;//s těmito ukazateli pracuje jak náhled tak schéma, ošetření
	kurzor(standard);
	//zablokování OnChange tabulek
	FormX->input_state=FormX->NO;
	FormX->vstoupeno_poh=false;
	FormX->vstoupeno_elm=false;
	MOD=NAHLED;
	//založení pomocného tempového ukazatele pro akutálně editovaný objekt a překopírování jeho atributů
	pom_temp=new Cvektory::TObjekt; pom_temp->pohon=NULL; pom_temp->pohon=new Cvektory::TPohon; pom_temp->elementy=NULL;
	//zkopíruje atributy objektu bez ukazatelového propojení, kopírování proběhne včetně spojového seznamu elemementu opět bez ukazatelového propojení s originálem, pouze mGrid je propojen
	d.v.kopiruj_objekt(pom,pom_temp);//pokud elementy existují nakopíruje je do pomocného nezávislého spojáku pomocného objektu
	posledni_editovany_objekt=pom;
	//nastavení zoomu na vhodný náhled
	if(Zoom<=5.0)Zoom=5.0;//ponechání zoomu pokud je vetší jak 5
	probehl_zoom=true;
	if(pom_temp->id==4 || pom_temp->id==2 || pom_temp->id==5){scGPPanel_pomocn_proSwitch->Visible=true;scGPSwitch_robot_clovek->Visible=true;}
	else {scGPPanel_pomocn_proSwitch->Visible=false;scGPSwitch_robot_clovek->Visible=false;}
	if(pom_temp->id==4 || pom_temp->id==2 || pom_temp->id==5)scGPLabel_roboti->Caption="Robot           Operátor";//mezery tvoří místo, kde je zobrazen switch
	else if(pom_temp->id==3)scGPLabel_roboti->Caption="Sekce";
	else scGPLabel_roboti->Caption="Roboti";
  //nutné při změně typu objektu, pouze tyto 2 knihovny se mění se změnou objektu
	DrawGrid_geometrie->Refresh();
	DrawGrid_knihovna->Refresh();
	vytvoreni_tab_pohon();
	nahled_ulozen=false;//nově otevřen, není uložen
	PmG->Update();
	//znovu provedení designu při otevření náhledu, který není prázdný
	if(pom_temp->elementy!=NULL)
	{
		Cvektory::TElement *E=pom_temp->elementy;
		while (E!=NULL)
		{
			if(E->n>0)
				design_element(E,false);
			E=E->dalsi;
		}
		E=NULL; delete E;
	}
	pom_element_temp=pom_temp->elementy->dalsi;//pro pořeby editace geometrie
	on_change_zoom_change_scGPTrackBar();//musí být po design_element
	FormX->input_state=FormX->NOTHING;
	REFRESH();//musí být z důvodu změny vykreslení
}
//---------------------------------------------------------------------------
//zaktualizuje ve formuláři parametry objektů combobox na výpis pohonů včetně jednotek uvedeného rozmezí rychlostí, pokud jsou zanechané implicitní parametry short RDunitD=-1,short RDunitT=-1, je načteno nastevní jednotek z INI aplikace pro form parametry objektu, v případech, kdy uvedené parametry nejsou dané hodnotou -1, tak se uvažují jednotky dle S==0,MIN==1 pro RDunitT, resp. M==0,MM==1 pro RDunitD
void TForm1::aktualizace_combobox_pohony_v_PO(short RDunitD,short RDunitT)
{
		log(__func__);//logování
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
{
    log(__func__);//logování
    //toto je v testování - prvni_zakazka.....
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
    log(__func__);//logování
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
   log(__func__);//logování
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
  log(__func__);//logování
	return ms.delete_repeat_all(FileName,"\\");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UlozitClick(TObject *Sender)
{
  log(__func__);//logování
	if(duvod_ulozit_nahled&&MOD==NAHLED)
	{
		//toto ne: d.v.vymaz_elementy(pom,false); ani vymaz_komory
		d.v.kopiruj_objekt(pom_temp,pom);
		DuvodUlozit(true);
		nahled_ulozit(false);
		if(editace_textu)Smaz_kurzor();
	}
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
	}
	else//stisknuto storno
	stisknuto_storno=true;
}
//---------------------------------------------------------------------------
//samotné uložení
void TForm1::Ulozit_soubor()
{
    log(__func__);//logování
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
  log(__func__);//logování
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
  log(__func__);//logování
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
  log(__func__);//logování
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
			d.v.PP.typ_linky=d.v.File_hlavicka.typ_linky;
			d.v.PP.delka_jig=d.v.File_hlavicka.delka_jig;
			d.v.PP.sirka_jig=d.v.File_hlavicka.sirka_jig;
			d.v.PP.vyska_jig=d.v.File_hlavicka.vyska_jig;
			d.v.PP.delka_podvozek=d.v.File_hlavicka.delka_podvozek;
      d.v.PP.uchyt_pozice=d.v.File_hlavicka.uchyt_pozice;
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
		default: return 2;
		break;
	}
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
  log(__func__);//logování
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
  log(__func__);//logování
	//-TIniFile *ini = new TIniFile(get_temp_dir() +"TISPL\\" + "tispl_"+get_user_name()+"_"+get_computer_name()+".ini");
	//-ini->WriteString("otevrene_soubory","posledni_soubor",FileName);
	//-delete ini;
	writeINI("otevrene_soubory","posledni_soubor",FileName);
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
    log(__func__);//logování
		d.v.vse_odstranit();
		if(pom!=NULL){pom->pohon=NULL;delete pom->pohon;}pom=NULL;delete pom;
		if(pom_temp!=NULL){pom_temp->pohon=NULL;delete pom_temp->pohon;}pom_temp=NULL;delete pom_temp;
		pom_element=NULL;delete pom_element;
		proces_pom=NULL;delete proces_pom;
		pom_element_temp=NULL; delete pom_element_temp;
		pom_komora=NULL;delete pom_komora;
		pom_komora_temp=NULL;delete pom_komora_temp;
		pom_bod=NULL;delete pom_bod;
		pom_bod_temp=NULL;delete pom_bod_temp;
		posledni_editovany_objekt=NULL;delete posledni_editovany_objekt;
		posledni_editovany_element=NULL;delete posledni_editovany_element;
		copyObjekt=NULL;delete copyObjekt;
		copyObjektRzRx.x=0;copyObjektRzRx.y=0;
	 //	delete LogFileStream; //zde nesmí být kvůli logování
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
			SB("Export do "+export_format+" dokončen.");
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
	 SB("Rychlý export dokončen.");
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
  log(__func__);//logování
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
  log(__func__);//logování
	//nezabira zneplatnit_minulesouradnice();
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_simulaceTimer(TObject *Sender)
{
   log(__func__);//logování
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
  log(__func__);//logování
	MB(-1,-1,"<b>Verze: </b>"+VERZE+"<br><br><b>Umístění: </b>"+Application->ExeName+"<br><br><b>Temp : </b>"+get_temp_dir(),"Informace o aplikaci");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  log(__func__);//logování
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
//		if(MOD==CASOVAOSA && ++pocitadlo_doby_neaktivity==2)
//		{
//			Timer_neaktivity->Enabled=false;
//			d.zobrazit_label_zamerovac(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
//		}
//		if(MOD==NAHLED && ++pocitadlo_doby_neaktivity==2)
//		{
//			Timer_neaktivity->Enabled=false;
//			setJobIDOnMouseMove(akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y);
//		}
	FormX->validace_max_voziku();//metoda rozlišuje zda byla editovaná stopka, pokud ano provede validaci, pokud ne neudělá nic
	REFRESH(true); //nedocází k refresh tabulek, tabulky jsou v tuto chvíli naplněny aktuálními hodnotami
	Timer_neaktivity->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonPLAY_OClick(TObject *Sender)
{
  log(__func__);//logování
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
//tlačítko na spuštění animace v náhledu kabiny
void __fastcall TForm1::scGPGlyphButton_PLAYClick(TObject *Sender)
{
  log(__func__);//logování
	RO-=(1.5*Zoom/m2px)/20.0;
	Poffset=0;
	scGPButton_viditelnostmGridClick(Sender);//zakáže mgridy
	Timer_animace->Enabled=!Timer_animace->Enabled;
	zobrazit_meritko=!Timer_animace->Enabled;
	d.v.PP.raster.show=!Timer_animace->Enabled;
	if(Timer_animace->Enabled)//běží animace
	{
		scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPause;
		scGPGlyphButton_PLAY->Hint="zastavit animaci";
		scGPGlyphButton_PLAY->ShowCaption=true;
		if(pom_temp->pohon!=NULL)Timer_animace->Interval=F->m.round(F->m.get_timePERpx(pom_temp->pohon->aRD,0));//stejná rychlost pro všechny RD
		else Timer_animace->Interval=40;//prozatim
		//Timer_animace->Interval=ceil(F->m.get_timePERpx(pom->RD,0,F->d.v.vrat_min_rychlost_prejezdu()));//různá rychlost dle RD, s afps se počítá dle min RD, ale nějak špatně vycházela animace ke konci (nestihl vozík vyjet)
	}
	else//animace zastavena
	{
		scGPGlyphButton_PLAY->GlyphOptions->Kind=scgpbgkPlay;
		scGPGlyphButton_PLAY->Hint="spustit animaci";
		zobrazit_meritko=scGPSwitch_meritko->State;//navrácení do původního stavu
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer_animaceTimer(TObject *Sender)
{
  log(__func__);//logování
	if(MOD==NAHLED)
	{
		short LO=1.5;//lakovací okno

		//rameno - kýve se po šířce lakovacího okna
		if(ROs==0)
		{
			if(RO>-(LO*Zoom/m2px))
			{
				RO-=(LO*Zoom/m2px)/20.0;
			}
			else ROs=1;
		}
		else
		{
			if(RO<(LO*Zoom/m2px))
			{
				RO+=(LO*Zoom/m2px)/20.0;
			}
			else ROs=0;
		}

		//tryska - jen se kýve ze strany na strany v rozmezí -30 až +30 stupňů
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

		Poffset+=1*m2px/Zoom;//zajistí posun animace o 1px (tedy nejmenší možnou grafickou jednotku), ale posouvání probíhá v metrech

		REFRESH();
	}
	else//ROMA metoda
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
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxVytizenost_Click(TObject *Sender)
{
  log(__func__);//logování
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
//MaVL - testovací tlačítko
void __fastcall TForm1::Button13Click(TObject *Sender)
{
//	Akce=GEOMETRIE;
//	editace_geometrie_spustena=false;
//	REFRESH(false);
	Cvektory::TElement *E=pom_temp->elementy->dalsi;  Memo3->Clear();
	while(E!=NULL && E->n!=0)
	{
		Memo(E->name);
		E=E->dalsi;
	} E=NULL;delete E;
//	Memo(vzdalenost_meziLO(E,pom_temp->orientace));
//	TRect A=vrat_max_oblast();
 //	Canvas->Pen->Color=clRed;
 //	d.line(Canvas,m.L2Px(E->X),m.L2Py(E->Y),0,0);
	//d.line(Canvas,m.L2Px(E->geo.X1),m.L2Py(E->geo.Y1),m.L2Px(E->geo.X4),m.L2Py(E->geo.Y4));
}
//---------------------------------------------------------------------------
//MaKr testovací tlačítko
void __fastcall TForm1::Button14Click(TObject *Sender)
{
 //log(__func__);
 //Form2->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxVymena_barev_Click(TObject *Sender)
{
	log(__func__);//logování
	d.JIZPOCITANO=false;//zajistí přepočet časových os
	Invalidate();
	RM();//korekce chyby oskakování pravého menu
}
//-------------------------------------------------------------
void __fastcall TForm1::ComboBoxODminChange(TObject *Sender)
{
  log(__func__);//logování
	//ještě ošetření aby zadal hodnotu od menší nebo rovno hodnotě do
	d.TP.OD=ms.MyToDouble(ComboBoxODmin->Text);
	Invalidate();
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBoxDOminChange(TObject *Sender)
{
  log(__func__);//logování
	//ještě ošetření aby zadal hodnotu od menší nebo rovno hodnotě do
	d.TP.DO=ms.MyToDouble(ComboBoxDOmin->Text);
	Invalidate();
	RM();//korekce chyby oskakování pravého menu
}
//---------------------------------------------------------------------------
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
	//ShowMessage("hu");
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
  log(__func__);//logování
	if (FMaximized)//zmenšení
	{
			 //BoundsRect =  FOldBoundsRect;
			 FMaximized = false;
			 scLabel_titulek->DragForm = true;
			 MaxButton->GlyphOptions->Kind = scgpbgkMaximize;
			 if(pom_temp==NULL){scButton_zamek_layoutu->Visible=false;scGPButton_zmerit_vzdalenost->Visible=false;}
			 scGPSizeBox->Visible = true;
			 if(pom_temp==NULL){scButton_zamek_layoutu->Visible=true; scGPButton_zmerit_vzdalenost->Visible=true;}
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
	log(__func__);//logování
	//zalogování vypnutí aplikace
//	AnsiString relation_id=GetCurrentProcessId();
//	AnsiString send_log_time= TIME.CurrentDateTime();
//	AnsiString ID ="1";
//	AnsiString Text="aplikace stop";
//	AnsiString strSQL = "INSERT INTO app_log (app_id,app_start,username,send_log_time,command,relation_id) VALUES (\""+ID+"\",\""+send_log_time+"\",\""+get_user_name()+"\",\""+send_log_time+"\",\""+Text+"\",\""+relation_id+"\")";
//	FDConnection1->ExecSQL(strSQL);
	int vysledek;
	if (MOD==NAHLED)
	{
		if (scGPButton_ulozit->Enabled)
		{
			vysledek=MB("Chcete uložit změny v editovaném objektu?",MB_YESNO,true);
			switch (vysledek)
			{
				case mrYes:scGPButton_OKClick(Sender);break;
				case mrNo:scGPButton_stornoClick(Sender);break;
			}
		}
		else scGPButton_stornoClick(Sender);
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
	if(Zoom>0.25)//akcelerátor, aby se zbytečně nevolalo
	{
		log(__func__);//logování
		vycentrovat=false;
		akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
		ZOOM_OUT();
		zobraz_tip("TIP: Oddálení obrazu lze také vykonat pomocí stisku klávesy - či F8 nebo CTRL a otáčením kolečko myši směrem dolu.");
	}
	else MessageBeep(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPGlyphButton_ZOOM_PLUSClick(TObject *Sender)
{
	if(Zoom<10)//akcelerátor, aby se zbytečně nevolalo
	{
		log(__func__);//logování
		vycentrovat=false;
		akt_souradnice_kurzoru=m.P2L(ClientWidth/2+scSplitView_LEFTTOOLBAR->Width,ClientHeight/2);
		ZOOM_IN();
		zobraz_tip("TIP: Přiblížení obrazu lze také vykonat pomocí stisku klávesy + či F7 nebo CTRL a otáčením kolečko myši směrem nahoru.");
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
void __fastcall TForm1::scGPGlyphButton2Click(TObject *Sender)
{
  log(__func__);//logování
	scSplitView_LEFTTOOLBAR->Opened = !scSplitView_LEFTTOOLBAR->Opened;
}
//---------------------------------------------------------------------------
//volání parametrů linky nebo definice zakázek - dle zvoleného režimu
void __fastcall TForm1::Button_dopravnik_parametryClick(TObject *Sender)
{
	log(__func__);//logování
	if(scSplitView_MENU->Opened)scSplitView_MENU->Opened=false;//zavře případně otevřené menu
	if(scSplitView_OPTIONS->Opened)scSplitView_OPTIONS->Opened=false;//zavře případně otevřené options
 //	scGPGlyphButton_OPTIONS->Down=false;//vypne případné podsvícení buttnu (aktivitu)
	if(scGPButton_header_projekt->ImageIndex==49)
	{
		if(MOD!=NAHLED/*||pom_temp->pohon==NULL*/)
		{
			ESC();//zruší případnou rozdělanou akci
			Form_parametry_linky->Left=Form1->ClientWidth/2-Form_parametry_linky->Width/2;
			Form_parametry_linky->Top=Form1->ClientHeight/2-Form_parametry_linky->Height/2;
			Form_parametry_linky->ShowModal();//návratová hodnota se řeši v knihovně
			REFRESH();
		}
		else {MB("Nelze editovat z náhledu.");Layout->Down=false;Nahled->Down=true;}
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
				//if(!TZF)Analyza->Enabled=true;//stačí takto pokud první zakázka nepůjde smazat nebo se v případě neexistence bude vytvářet nová, což se momentálně děje při příchodu do časových os
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
  log(__func__);//logování
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
  log(__func__);//logování
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
void __fastcall TForm1::scExPanel_ostatniClick(TObject *Sender)
{
  log(__func__);//logování
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
  log(__func__);//logování
//	if(!start_ortogonalizace)//pokud se nejedná o start aplikace, aby se nevolalo v tomto okamžiku
//	ortogonalizace_on_off();
	if(scGPCheckBox_ortogon->State==0)ortogonalizace_stav=false;
	else ortogonalizace_stav=true;
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
//	Cvektory::TObjekt *p=d.v.najdi_objekt(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.O_width*m2px,d.O_height*m2px);
//
//	Memo2->Lines->Add(AnsiString(p->name)+";"+AnsiString(p->short_name)+";"+AnsiString(p->rezim)+";"+AnsiString(p->pohon->n)+";"+AnsiString(p->delka_dopravniku)+";"+AnsiString(p->cekat_na_palce)+";"+AnsiString(p->odchylka)+";"+AnsiString(p->kapacita));

		Cvektory::TZakazka *ukaz2=d.v.ZAKAZKY->dalsi;
		if (ukaz2==NULL)
		{
ShowMessage("NEmam data");
//Analyza->Enabled=false;
		}
		else {
		 ShowMessage("mam data");
		 //Analyza->Enabled=true;
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
   log(__func__);//logování
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
  log(__func__);//logování
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
  log(__func__);//logování
//	scExPanel_log_header->Visible=false;
	//scGPGlyphButton_zpravy_ikona->Visible=true;
	scExPanel_log_header->Visible=false;
	scSplitView_OPTIONS->Opened=false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPGlyphButton_zpravy_ikonaClick(TObject *Sender)
{
  log(__func__);//logování
	if(scExPanel_log_header->Visible==false)
	{
		scExPanel_log_header->Visible=true;

  //pozice  zprav
	scExPanel_log_header->Left	 = Form1->Width/2-scExPanel_log_header->Width/2;
	scExPanel_log_header->Top 	 = Form1->Height/2-scExPanel_log_header->Height/2;
	scExPanel_log_header->Height = 300;
	scExPanel_log_header->Width	 = 715;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scSplitView_OPTIONSOpened(TObject *Sender)
{
  log(__func__);//logování
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
//	TscGPGlyphButton *B = new TscGPGlyphButton(this);
//	B->Left=ClientWidth/2;
//	B->Top=ClientHeight/2;
//	B->GlyphOptions->Kind=scgpbgkCancel;
//	//B->Caption="test";
//	B->Parent=this;//musí být až na konci
//	B=NULL;delete B;

//ShowMessage(pom_temp->elementy->name);

//	TPoint *tab_pruchodu=new TPoint[d.v.pocet_vyhybek+1];
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
//Cvektory::TElement *E=d.v.OBJEKTY->dalsi->elementy->dalsi;
//while(E!=NULL)
//{
//	 Memo3->Lines->Add(E->n);
//	 E=E->dalsi;
//}
//E=NULL;delete E;

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
//double r=d.v.PP.katalog;
//double rad=d.v.PP.radius;
//
//Memo(r);
//Memo(rad);
  Form_definice_zakazek->ShowModal();




}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_stornoClick(TObject *Sender)
{
	log(__func__);//logování
	if(MOD==NAHLED)  //navrácení do módu schéma
	{
		if(MOD==NAHLED&&index_kurzoru==9999||index_kurzoru==100)
		smaz_edit(false);//smaže edit a neprovede refresh
		DrawGrid_knihovna->SetFocus();
		if(editace_textu)Smaz_kurzor();//také volá Refresh//smaz_kurzor se zavolá, pouze pokud je to třeba odstraňuje zbytečný problik, dodělal MaKr
		MOD=SCHEMA;//nutné před zoom, ale za smaz kurzor
		//smazání případných komor
		d.v.vymaz_komory(pom_temp);
		//smazání elementů - musí být napočátku, aby nebyl problik
		d.v.vymaz_elementy(pom_temp,true);   //&&pom_temp->elementy->dalsi!=NULL)
		if(!mazani&&scGPButton_ulozit->Enabled)d.v.uprav_popisky_elementu(pom,NULL);//volání přejmenování elementů, pouze v případě kdy je něco v kabině a bylo stisknuto pouze storno, při ulož je stisk strona volán taky
		pom=NULL;//pom->pohon=NULL;delete pom->pohon;pom=NULL; toto nelze, odpřiřadilo by to pohon i na ostrém
//		d.v.vymaz_elementy(pom_temp,true);
		if(pom_temp!=NULL){pom_temp->pohon=NULL;delete pom_temp->pohon;}pom_temp=NULL;delete pom_temp;
		PmG->Delete(); PmG=NULL; delete PmG;
    //mazání pomocných ukazatelů při odchodu z náhledu, důležité!! (při rychlem posunu myší mohou zůstávat v paměti)
		pom_element_temp=NULL;delete pom_element_temp;pom_komora=NULL;delete pom_komora;pom_komora_temp=NULL;delete pom_komora_temp;pom_element=NULL;delete pom_element;pom_bod=NULL;delete pom_bod;pom_bod_temp=NULL;delete pom_bod_temp;posledni_editovany_element=NULL;delete posledni_editovany_element;JID=-1;Akce=NIC;
    //v případě animace vypnutí a nastavení do výchozího stavu
		Timer_animace->Enabled=false;
		ButtonPLAY->GlyphOptions->Kind=scgpbgkPlay;
		ButtonPLAY->Hint="spustit animaci";
		zobrazit_meritko=scGPSwitch_meritko->State;//navrácení do původního stavu
		//vypnutí spodního panelu
		scGPPanel_bottomtoolbar->Visible=false;
		//vlevo
		scLabel_klient->Visible=true;
		scGPSwitch_rezim->Visible=true;
		scLabel_architekt->Visible=true;
		scGPGlyphButton_PLAY->Visible=false;
		//navrácení zoomu a posunu do původních hodnt
		Zoom=Zoom_predchozi2;
		//on_change_zoom_change_scGPTrackBar();//pozor asi volá refresh
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

//		scGPLabel_roboti->Visible=false;
//		scGPLabel_otoce->Visible=false;
//		scGPLabel_stop->Visible=false;
//		scGPLabel_geometrie->Visible=false;
//		scGPLabel_poznamky->Visible=false;

		//scGPLabel_roboti->Font->Style = TFontStyles(); // zrušení tučného písma resp. všech případných dalších Font style nastavení
//		scGPLabel_roboti->Visible=true;
		//scGPLabel_roboti->Caption="Technolog. objekty";
		//scGPLabel_roboti->ContentMarginLeft=4;
		scGPSwitch_robot_clovek->Visible=false;
		scGPPanel_pomocn_proSwitch->Visible=false;

		Schema->Down=false;//musí zde být, zavírání přes tlačítko layout by jinak nebylo plně funkční!!
		Schema->Down=true;
		//REFRESH(); //- asi netřeba  asi vyvolává výše uvedený on_change_zoom_change_scGPTrackBar()
		//scListGroupKnihovObjektu->Visible=false;
		scGPComboBox_prepinacKot->ItemIndex=0;//ošetření pokud bylo při vypínání editace nastaveno na časové kóty
		scButton_zamek_layoutu->Visible=true;//zapnutí tlačítka zámek layoutu
		Image_knihovna_objektu->Visible=true;//zapnutí knihovny
    scGPButton_zmerit_vzdalenost->Visible=true;
		mGrid_knihovna->SetVisibleComponents(true);//zapnutí komponent v mgridu
		popisky_knihovna_nahled(true);//nastavení popisků pro knihovnu
		DrawGrid_knihovna->Top=10000;//musí být zobrazena, odchytává stisk kláves
		vlozit_predavaci_misto();//skontroluje zda nemusí být přidáno nebo odstraněno předávací místo
		on_change_zoom_change_scGPTrackBar();//pozor asi volá refresh, změna pořadí
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton_nacist_podkladClick(TObject *Sender)
{
  log(__func__);//logování
	scSplitView_MENU->Opened=false;
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
 unsigned short int  TForm1::Nacist_podklad(UnicodeString soubor)
 {
  log(__func__);//logování
  d.v.PP.raster.show=true;
	d.v.PP.raster.filename=soubor;
	d.v.PP.raster.X=m.P2Lx(scSplitView_LEFTTOOLBAR->Width +10);d.v.PP.raster.Y=m.P2Ly(scGPPanel_mainmenu->Height + 10);//souřadnice v metrech
	d.v.PP.raster.resolution=m2px;  //výpočet metry děleno počet PX, výchozí zobrazení v nativním rozlišení (bez usazení do metrického měřítka) je 0.1
  scGPCheckBox_zobraz_podklad->Checked=true;
  scButton_nacist_podklad->Down=false;  //ošetření proti tmavému vysvícení při dalším zobrazení mainmenu
  REFRESH();
  auto_settings_open=false;
	zobraz_tip("Pro správné umístění a nastavení měřítka podkladu, využijte volbu v pravém horním menu.");
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
  //scGPGlyphButton_OPTIONS->ShowHint=true;
 // }

}
 //--------------------------------------------------------------
void __fastcall TForm1::DrawGrid_geometrieDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	log(__func__);//logování
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
	if(pom_temp->id!=4)//pro ostatní objekty
	{
		scGPLabel_geometrie->Caption="Geometrie linky";
		for(unsigned short n=1;n<=pocet_elementu;n++)
		{
			if(n==1)
			{
			 label1= "linie";
			 label2="";
			 if(pom->id!=3)d.vykresli_ikonu_linie(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20-odsazeni,label1);
			 else d.vykresli_ikonu_linie(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20-odsazeni,label1,-1);
			}
			if(n==2)
			{
			 label1= "oblouky";
			 label2="";
//  		 if(pom->id!=5) d.vykresli_ikonu_oblouku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20-odsazeni,label1);
			 /*else*/           //d.vykresli_ikonu_oblouku(C,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P + 20-odsazeni,label1/*,-1*/);
				if(pom_temp->pohon!=NULL)d.vykresli_predavaci_misto(C,NULL,(Rect.Right*Z-Rect.Left*Z)/2+((n+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(n/2.0)-1)*H+P,"před. místo",0);
				else d.vykresli_predavaci_misto(C,NULL,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P,"před. místo",0,0,-1);
			}

		}
	}
	else//pro ionizaci zobrazí ionizační tyč
	{
		scGPLabel_geometrie->Caption="Ostatní";
		if(pom_temp->pohon!=NULL)
		{
			d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P,"ionizační tyč","",100,0,90);
			d.vykresli_predavaci_misto(C,NULL,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P,"před. místo",0);
		}
		else
		{
			d.vykresli_element(C,(Rect.Right*Z-Rect.Left*Z)/2+((1+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(1/2.0)-1)*H+P,"ionizační tyč","",100,0,90,-1);
			d.vykresli_predavaci_misto(C,NULL,(Rect.Right*Z-Rect.Left*Z)/2+((2+1)%2)*W,(Rect.Bottom*Z-Rect.Top*Z)/2+(ceil(2/2.0)-1)*H+P,"před. místo",0,0,-1);
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
 // scLabel1_svetelnost->Font->Color=clWhite;  // odkomentovat, pokud bude povoleno
  }
  edice();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::scGPGlyphButton_OPTIONSMouseEnter(TObject *Sender)
{
log(__func__);//logování
scGPGlyphButton_OPTIONS->GlyphColor=clWhite;//(TColor)RGB(0,120,215);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::scGPGlyphButton_OPTIONSMouseLeave(TObject *Sender)
{
log(__func__);//logování
scGPGlyphButton_OPTIONS->GlyphColor=(TColor)RGB(255,128,0);
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



void __fastcall TForm1::DrawGrid_ostatniMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
log(__func__);//logování
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
log(__func__);//logování
FormMouseWheelDown(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_ostatniMouseWheelUp(TObject *Sender, TShiftState Shift,
					TPoint &MousePos, bool &Handled)
{
  log(__func__);//logování
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_geometrieMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
  log(__func__);//logování
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DrawGrid_poznamkyMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
  log(__func__);//logování
	FormMouseWheelUp(Sender,Shift,MousePos,Handled);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button_testClick(TObject *Sender)
{
MOD=SCHEMA;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPButton_OKClick(TObject *Sender)
{
	log(__func__);//logování
	d.v.vymaz_komory(pom);
	d.v.vymaz_elementy(pom,true);
	d.v.kopiruj_objekt(pom_temp,pom);
	DuvodUlozit(true);
	nahled_ulozit(false);
	mazani=true;//použití proměnné, která se v tomto čase nevyužívá, slouží k rozpoznání zda bylo stisknuto dříve storno či uližit
	//a to z důvodu volání uprav_popisky_elementu(přejmenování změn po stisku storno)
	scGPButton_stornoClick(Sender);//další funkcionalita je již stejná jako ve stornu, včetně vymazání ukazatele pom_temp včetně jeho elementů popř. komor
  mazani=false;
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
//při načítaní hodnoty rozměrů provede převod podle aktuálně nastavených jednotek
double TForm1::inDK(double inDK)
{   //0=M,1=MM,2=SEC,3=MIN
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
	double aRD=inaRD;
	aRD=inaRD/(1+59.0*aRDunit);
	return aRD;
}
double TForm1::outaRD(double outaRD)
{
	double aRD=outaRD;
	aRD=outaRD*(1+59.0*aRDunit);
	return aRD;
}
double TForm1::inR(double inR)
{
	double R=inR;
	R=inR/(1+999.0*Runit);
	return R;
}
double TForm1::outR(double outR)
{
	double R=outR;
	R=outR*(1+999.0*Runit);
	return R;
}
double TForm1::inRz(double inRz)
{
	double Rz=inRz;
	Rz=inRz/(1+999.0*Rzunit);
	return Rz;
}
double TForm1::outRz(double outRz)
{
	double Rz=outRz;
	Rz=outRz*(1+999.0*Rzunit);
	return Rz;
}
void __fastcall TForm1::scGPEdit1Change(TObject *Sender)
{
Memo3->Lines->Add("onchange");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
 log(__func__);//logování
 if(pom_element_temp!=NULL&&index_kurzoru==9999)//pro smazání editu při editaci názvu elementu skrze tabulku
 {
	 smaz_edit();
 }
 else//pro posun refresh při změně tabulky elementů
 {
	 REFRESH(true);
	 FormX->input_state=FormX->NOTHING;
 }
 Timer2->Enabled=false;
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
	short Wn,Wl,Wz;
	double Xt=0,Yt=0;
	if(pom_temp!=NULL)
	{
		d.nastavit_text_popisu_objektu_v_nahledu(Canvas,1);Tn=F->pom_temp->name.UpperCase();Wn=Canvas->TextWidth(Tn);//název objektu - nastavení
		d.nastavit_text_popisu_objektu_v_nahledu(Canvas,0);Tl=+" / ";Wl=Canvas->TextWidth(Tl);//lomítko objektu - nastavení
		d.nastavit_text_popisu_objektu_v_nahledu(Canvas,2);Tz=F->pom_temp->short_name.UpperCase();Wz=Canvas->TextWidth(Tz);//zkratka objektu - nastavení
	}
	//vykreslování kurzoru pro psaní textu
	Canvas->Pen->Style=psSolid;
	Canvas->Pen->Mode=pmNotXor;
	Canvas->Pen->Color=clRed;
	Canvas->Pen->Width=2;
	switch ((index))//index=JID, kde a jaký kurzor vykreslit
	{
		case 1://editace názvu elementu
		{
			Canvas->Pen->Color=clBlack;
			Canvas->Pen->Width=1;
			if(pom_element_temp->eID!=0&&pom_element_temp->eID!=5&&pom_element_temp->eID!=6&&pom_element_temp->eID!=100)//roboti mají vykreslován kurzor vodorovně
			{
				if(pom_element_temp->orientace==0||pom_element_temp->orientace==180)//pro vodorovnou kabinu
				{
					Canvas->MoveTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.top-2);
					Canvas->LineTo(pom_element_temp->citelna_oblast.rect3.right+1,pom_element_temp->citelna_oblast.rect3.bottom+2);
				}
				else if(pom_element_temp->orientace==90)//pro vertikálníkabinu, dělení na 2 rotace, text orientován 2 směry
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
				if(pom_element_temp->orientace==0||pom_element_temp->orientace==180)//pro vodorovnou kabinu, 2 orientace textu
				{
					if(pom_element_temp->orientace==0)
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
			Canvas->MoveTo(pom_bod_temp->kota.rect1.right+1,pom_bod_temp->kota.rect1.top);
			Canvas->LineTo(pom_bod_temp->kota.rect1.right+1,pom_bod_temp->kota.rect1.bottom);
			stav_kurzoru=!stav_kurzoru;
		}break;
		case -6://název objektu
		{
			if(Tn=="")Tn="b";//při prázdném textu nemůžu zjistit TextHeight -> to zapříčiní čpatné vykreslení kurzoru, když uživatel odmaže veškerý text
			Xt=m.L2Px(F->pom_temp->Xt);Yt=m.L2Py(F->pom_temp->Yt);
			switch((int)pom_temp->orientace)
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
	}
	if(index<=-11&&pom_temp->id!=3)//kóty elementů
	{
		Canvas->Pen->Color=clGray;
		Canvas->Pen->Width=1.5;
		if(index!=-101)
		{
			Canvas->MoveTo(pom_element_temp->citelna_oblast.rect1.right+1,pom_element_temp->citelna_oblast.rect1.top);
			Canvas->LineTo(pom_element_temp->citelna_oblast.rect1.right+1,pom_element_temp->citelna_oblast.rect1.bottom);
		}
		else
		{
			Canvas->MoveTo(pom_element_temp->citelna_oblast.rect4.right+1,pom_element_temp->citelna_oblast.rect4.top);
			Canvas->LineTo(pom_element_temp->citelna_oblast.rect4.right+1,pom_element_temp->citelna_oblast.rect4.bottom);
		}
		stav_kurzoru=!stav_kurzoru;
	}
	if(index<=-11&&pom_temp->id==3)//kóty komor
	{
		Canvas->Pen->Color=clGray;
		Canvas->Pen->Width=1.5;
		Canvas->MoveTo(pom_komora_temp->kota.rect1.right+1,pom_komora_temp->kota.rect1.top);
		Canvas->LineTo(pom_komora_temp->kota.rect1.right+1,pom_komora_temp->kota.rect1.bottom);
		stav_kurzoru=!stav_kurzoru;
  }
}
//smaže kurzor pokud je stále vykreslený i po vypnutí editace textu
void TForm1::Smaz_kurzor()
{
  log(__func__);//logování
	TIP="";
	if(editace_textu && pom_temp!=NULL)//ukončí editaci textu
	{
    //pokud je kurzor vykreslen překresli ho
		if (stav_kurzoru) vykresli_kurzor(index_kurzoru);
		TimerKurzor->Enabled=false;
		editace_textu=false;
		//pokud bylo zadáno nic přepíše nic původními hodnotamy
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru==-5){if(pom_bod_temp->n!=1)editovany_text=m.round2double(m.delka(pom_bod_temp->predchozi->X,pom_bod_temp->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);else editovany_text=m.round2double(m.delka(pom_temp->body->predchozi->X,pom_temp->body->predchozi->Y,pom_bod_temp->X,pom_bod_temp->Y),3);if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));}
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru<=-11&&pom_temp->id!=3&&Akce!=GEOMETRIE){if(index_kurzoru!=-101)editovany_text=d.v.vzdalenost_od_predchoziho_elementu(pom_element_temp);else editovany_text=outDK(vzdalenost_meziLO(pom_element_temp,pom_temp->orientace));if(DKunit==2||DKunit==3)editovany_text=editovany_text/pom_temp->pohon->aRD;}
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru<=-11&&pom_temp->id==3&&Akce!=GEOMETRIE)editovany_text=outDK(pom_komora_temp->velikost);
		if((editovany_text==""||ms.MyToDouble(editovany_text)==0)&&index_kurzoru<=-11&&Akce==GEOMETRIE){editovany_text=pom_element_temp->geo.delka;if((DKunit==2||DKunit==3) && pom_element_temp->pohon!=NULL)editovany_text=editovany_text/pom_temp->pohon->aRD;editovany_text=outDK(ms.MyToDouble(editovany_text));}
		switch(index_kurzoru)
		{
			case -7:if(pom_temp->short_name=="")pom_temp->short_name=nazev_puvodni;break;
			case -6:if(pom_temp->name=="")pom_temp->name=nazev_puvodni;break;
			case 1:
			{
				if(pom_element_temp->name=="")pom_element_temp->name=nazev_puvodni;
				//propsání nového názvu do mGridu
				pom_element_temp->mGrid->Cells[0][0].Text="<a>"+pom_element_temp->name+"</a>";
				pom_element_temp->mGrid->MergeCells(0,0,1,0);
			}break;
			case -5://hodnota kót objektu (hrana objektu)
			{
				//převedení na základní jednotky
				editovany_text=inDK(ms.MyToDouble(editovany_text));
				if(DKunit==2||DKunit==3)editovany_text=editovany_text*pom_temp->pohon->aRD;//pokud jsou kóty v časovém režimu, převede na vzdálenost;
				//deklarace pomocných proměnných
				double puv=0,nov=ms.MyToDouble(editovany_text),posun=0;
				Cvektory::TBod *A=pom_bod_temp->predchozi;
				if(pom_bod_temp->n==1)A=pom_temp->body->predchozi;
				//výpočet posunu (jednotlivé souřadnice bodu) + posun
				puv=m.round2double(m.delka(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),3);
				posun=nov-puv;
				d.v.posun_bod(pom_bod_temp->X+sin(DegToRad(fmod(m.azimut(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),360)))*posun,pom_bod_temp->Y+cos(DegToRad(fmod(m.azimut(A->X,A->Y,pom_bod_temp->X,pom_bod_temp->Y),360)))*posun,pom_bod_temp);
				A=NULL;delete A;
			}break;
		}
		if(index_kurzoru<=-11&&pom_temp->id!=3&&Akce!=GEOMETRIE)//editace hodnot kót elementů
		{
			editovany_text=inDK(ms.MyToDouble(editovany_text));//převedení na základní jednotky
			if(index_kurzoru==-101)editovany_text=ms.MyToDouble(editovany_text)-vzdalenost_meziLO(pom_element_temp,pom_temp->orientace)+d.v.vzdalenost_od_predchoziho_elementu(pom_element_temp);
			if(DKunit==2||DKunit==3)editovany_text=editovany_text*pom_temp->pohon->aRD;//pokud jsou kóty v časovém režimu nutno přepočítat na vzdálenost
			d.v.posun_element(pom_element_temp,ms.MyToDouble(editovany_text),posun_dalsich_elementu);//realizace samotného posunu
		}
		if(index_kurzoru<=-11&&pom_temp->id==3&&Akce!=GEOMETRIE)//editace rozmeru komor v POW
		{
			editovany_text=inDK(ms.MyToDouble(editovany_text));
			double posun=ms.MyToDouble(editovany_text)-pom_komora_temp->velikost;if(pom_temp->orientace==270 ||pom_temp->orientace==180)posun*=-1;
			if(pom_temp->orientace==90 || pom_temp->orientace==270)d.v.posun_hranu(posun,0,pom_temp->body->dalsi->dalsi,pom_temp->body->dalsi->dalsi->dalsi);
			else d.v.posun_hranu(0,posun,pom_temp->body->dalsi->dalsi,pom_temp->body->dalsi->dalsi->dalsi);
			pom_komora_temp->velikost=ms.MyToDouble(editovany_text);
		}
		if(index_kurzoru<=-11&&pom_temp->id!=3&&Akce!=GEOMETRIE&&(pom_element_temp->eID==2||pom_element_temp->eID==4))pom_element_temp->mGrid->Refresh();//musí se refreshovat z důvodu přepočtu RT u S&G
		if(index_kurzoru<=-11&&Akce==GEOMETRIE)//editace rozměrů přímky v editaci geometrie
		{
			double posunx=pom_element_temp->geo.X4,posuny=pom_element_temp->geo.Y4;
			editovany_text=inDK(ms.MyToDouble(editovany_text));
			double posun=ms.MyToDouble(editovany_text)-pom_element_temp->geo.delka;
			if((DKunit==2||DKunit==3) && pom_element_temp->pohon!=NULL)editovany_text=editovany_text*pom_element_temp->pohon->aRD;//pokud jsou kóty v časovém režimu nutno přepočítat na vzdálenost
			d.v.posun_element(pom_element_temp,ms.MyToDouble(editovany_text),posun_dalsich_elementu,false,false);//změna délky přímky posunem elementu (force posun)
			posunx=pom_element_temp->geo.X4-posunx;posuny=pom_element_temp->geo.Y4-posuny;
			pom_element_temp=pom_element_temp->dalsi;
			//posun ostatních elementů a jejich geometrie
			while(pom_element_temp!=NULL)
			{
				pom_element_temp->X+=posunx;pom_element_temp->Y+=posuny;//souřadnice elementu
				//geometrie elementu
				pom_element_temp->geo.X1+=posunx;pom_element_temp->geo.X2+=posunx;pom_element_temp->geo.X3+=posunx;pom_element_temp->geo.X4+=posunx;
				pom_element_temp->geo.Y1+=posuny;pom_element_temp->geo.Y2+=posuny;pom_element_temp->geo.Y3+=posuny;pom_element_temp->geo.Y4+=posuny;
				pom_element_temp=pom_element_temp->dalsi;
			}
		}
	}
	else if(editace_textu && editovany_text!="")//pokud neexistuje otevřený objekt + očetření proti zapsání prázdné hodnoty
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
	REFRESH(true);//uvolnění rastru
}
//---------------------------------------------------------------------------
//vypnutí/zapnutí zamčení náhledu
void __fastcall TForm1::scButton_zamekClick(TObject *Sender)
{
	log(__func__);//logování
	if(scButton_zamek->ImageIndex==37)//zamčeno budu odemykat
	{
		pom_temp->uzamknout_nahled=false;
		scButton_zamek->ImageIndex=60;//odemčeno
		scButton_zamek->Hint="Zamknout náhled";
	}
	else//odemčeno budu zamykat
	{
		pom_temp->uzamknout_nahled=true;
		scButton_zamek->ImageIndex=37;
		scButton_zamek->Hint="Odemknout náhled";
	}
	Smaz_kurzor();
	pom_element=NULL;
	JID=-1;
	DrawGrid_knihovna->SetFocus();//vrací focus na knihovnu - důležité z důvodu keydown
	REFRESH();
}
//---------------------------------------------------------------------------
//přepíná viditelnost mGridů
void __fastcall TForm1::scGPButton_viditelnostmGridClick(TObject *Sender)
{
  log(__func__);//logování
	if(scGPButton_viditelnostmGrid->ImageIndex==55)//vypnuto budu zapínat
	{
		scGPButton_viditelnostmGrid->ImageIndex=54;//zapnuto
		scGPButton_viditelnostmGrid->Hint="Skrýt tabulky";
		pom_temp->zobrazit_mGrid=true;
	}
	else//zapnuto budu vypínat
	{
		scGPButton_viditelnostmGrid->ImageIndex=55;
		scGPButton_viditelnostmGrid->Hint="Zobrazit tabulky";
		pom_temp->zobrazit_mGrid=false;
	}
	nahled_ulozit(true);
	REFRESH();
	DrawGrid_knihovna->SetFocus();

}
//---------------------------------------------------------------------------
//přepíná viditelnost kót
void __fastcall TForm1::scGPButton_viditelnostKotyClick(TObject *Sender)
{
  log(__func__);//logování
	if(scGPButton_viditelnostKoty->ImageIndex==57)//vypnuto budu zapínat
	{
		scGPButton_viditelnostKoty->ImageIndex=56;//zapnuto
		scGPButton_viditelnostKoty->Hint="Skrýt kóty";
		pom_temp->zobrazit_koty=true;
	}
	else//zapnuto budu vypínat
	{
		scGPButton_viditelnostKoty->ImageIndex=57;
		scGPButton_viditelnostKoty->Hint="Zobrazit kóty";
		pom_temp->zobrazit_koty=false;
	}
	nahled_ulozit(true);
	REFRESH();
	DrawGrid_knihovna->SetFocus();
}
//---------------------------------------------------------------------------
//přepínání posunu dalších elementů
void __fastcall TForm1::scGPButton_posun_dalsich_elementuClick(TObject *Sender)
{
  log(__func__);//logování
	if(scGPButton_posun_dalsich_elementu->ImageIndex==59)//vypnuto budu zapínat
		{
			scGPButton_posun_dalsich_elementu->ImageIndex=58;
			posun_dalsich_elementu=true;
			scGPButton_posun_dalsich_elementu->Hint="Zakázat vázaný posun robotů, stop stanic a otočí v editovaném objektu";
		}
	else
		{
			scGPButton_posun_dalsich_elementu->ImageIndex=59;
			posun_dalsich_elementu=false;
			scGPButton_posun_dalsich_elementu->Hint="Povolit vázaný posun robotů, stop stanic a otočí v editovaném objektu";
		}
	DrawGrid_knihovna->SetFocus();
}
//---------------------------------------------------------------------------
//combo pro přepínání typu kót
void __fastcall TForm1::scGPComboBox_prepinacKotClick(TObject *Sender)
{
  log(__func__);//logování
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
  	REFRESH();
  	refresh_mGrid=true;//navrácení stavu
	}
}
//---------------------------------------------------------------------------
//urychlení vypsání do Mema
void TForm1::Memo(AnsiString Text, bool clear,bool count)
{
  log(__func__);//logování
	if(clear)Memo3->Clear();
	if(count){count_memo++;Text=count_memo;}
	Memo3->Visible=true;
	Memo3->Lines->Add(Text);
	Memo3->CopyToClipboard();
}
//---------------------------------------------------------------------------
//zapínání a vypínaní meření
void __fastcall TForm1::scGPImage_mereni_vzdalenostClick(TObject *Sender)
{
	log(__func__);//logování
	if(Akce==NIC)
	{
		scSplitView_OPTIONS->Close();
		Akce=MEASURE;
		kurzor(add_o);
		scGPImage_mereni_vzdalenost->ClipFrameFillColor=(TColor)RGB(225,225,225);
	}
	else
	{
		Akce=NIC;
		kurzor(standard);
		scGPImage_mereni_vzdalenost->ClipFrameFillColor=clWhite;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPSwitch_robot_clovekChangeState(TObject *Sender)
{
  log(__func__);//logování
	DrawGrid_knihovna->Refresh();
	ESC();//případné vypnutí akce
}
//---------------------------------------------------------------------------
//načte zvolený jazyk
unsigned short TForm1::load_language(Tlanguage language)
{
   log(__func__);//logování
   //nastavení adresáře k místě aplikace
  ChDir(ExtractFilePath(Application->ExeName));    //přesune k EXE
  UnicodeString File_language= "MK.language"; //cache_dir+"MK.language" už nenačítám v tempu aplikace

  /*if(!FileExists(File_language)) //pokud bych chtěl jazyk stahovat
  {
	try
	{
		URLDownloadToFile(0,AnsiString("http://www.omapy.cz/files/CSIS/"+File_language).c_str(),File_language.c_str(),0,0);//pokud nebyl nalezen jazykový slovník, zkusí stáhnout
	}
	catch(...){;}
  }*/
  ls=new TStringList;
  for(unsigned short i=0;i<=114;i++)ls->Insert(i,"");//vyčištění řetězců, ale hlavně založení pro default! proto nelze použít  ls->Clear();

    if(FileExists(File_language))//znovu kontrola po případném stažení souboru
  {
	//načtení jazykového slovníku do string listu
  ShowMessage(language);
	ls->LoadFromFile(File_language);

	//vypársování daného jazyka a navrácení do string listu již jen vypársovaného
	for(unsigned int i=0;i<ls->Count;i++)
	{
		switch(language)
		{
			case EN:ls->Strings[i]=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);Jazyk=EN; /*anglictina1->Checked=true;mongolstina1->Checked=false;cestina1->Checked=false;*/break;
			case MN:ls->Strings[i]=ls->Strings[i].Delete(1,ls->Strings[i].Pos(";"));ls->Strings[i]=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);Jazyk=MN;/*anglictina1->Checked=false;mongolstina1->Checked=true;cestina1->Checked=false;*/break;
			case CS:ls->Strings[i]=ls->Strings[i].Delete(1,ls->Strings[i].Pos(";"));ls->Strings[i]=ls->Strings[i].SubString(ls->Strings[i].Pos(";")+1,ls->Strings[i].Length());Jazyk=CS;/*anglictina1->Checked=false;mongolstina1->Checked=false;cestina1->Checked=true;*/break;
			default:EN:ls->Strings[i]=ls->Strings[i].SubString(0,ls->Strings[i].Pos(";")-1);Jazyk=EN;/*anglictina1->Checked=true;mongolstina1->Checked=false;cestina1->Checked=false;*/break;
		}
	}
											   //pokud se nejedná o angličtinu
	scLabel_titulek->Caption=ls->Strings[0];if(language>1)scLabel_titulek->Caption+=" (Language)";
	scLabel_titulek->Caption=ls->Strings[3];scLabel_titulek->Caption+=" (EN)";
	scLabel_titulek->Caption=ls->Strings[2];scLabel_titulek->Caption+=" (MN)";
	scLabel_titulek->Caption=ls->Strings[1];scLabel_titulek->Caption+=" (CS)";
	//-----------------------
  //Example
  Form_parametry_linky->scGPButton_pohon->Caption =  	ls->Strings[8];
  Layout->Caption =  	ls->Strings[9];
  //Canvas->TextOutW(500,500,ls->Strings[9]); //OK
	return 1;
	}
  else //pokud není nalezen jazykový slovník
  {
	//defaultní hlášky Form1
//	unsigned short ui=36;
//	ls->Insert(ui++,"Repeat or press ENTER or double click when finished | press ESC to cancel");
	return 0; //načte defaultní jazykové nastavení tzn. AJ
  }
}
//zapnutí posunu haly
void __fastcall TForm1::scGPButton_posun_halyClick(TObject *Sender)
{
  log(__func__);//logování
	if(d.v.HALA.body!=NULL && pom_temp==NULL)//pokud existuje hala a není aktivní editace objektu, jinak nemá smysl
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
	if(pom_temp==NULL)//ošetření proti spouštění v náhledu
	{
  	scSplitView_OPTIONS->Close();
  	if(d.v.HALA.body==NULL||d.v.HALA.body->dalsi==NULL)//pokud existuje hala, jinak nemá smysl
  	{
    	TIP="Klinutím na levé tlačítko myši přidejte bod.";
  		Akce=DRAW_HALA;
  		kurzor(add_o);
  	}
  	else if(mrYes==MB("Dojde k odstranění haly, chcete pokračovat?",MB_YESNO))
  	{
  		//smaže starou halu
  		Cvektory::TBod *B=d.v.HALA.body->dalsi;
  		while(B!=NULL)
  		{
  			d.v.smaz_bod(B);
  			B=B->dalsi;
  		}
  		delete B;B=NULL;
  		scGPButton_nakreslit_haluClick(this);//znovu spuštění metody
		}
	}
}
//---------------------------------------------------------------------------
//pokud existují objekty zapne editaci posledního editovaného, pokud neexistuje poslední editovaný otevře editaci posledního přidaného
void __fastcall TForm1::NahledClick(TObject *Sender)
{
	if(d.v.OBJEKTY->dalsi!=NULL && pom_temp==NULL)
	{
		if(posledni_editovany_objekt!=NULL)pom=posledni_editovany_objekt;
		else pom=d.v.OBJEKTY->predchozi;
		NP_input();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPImage_zamek_posunuClick(TObject *Sender)
{
	log(__func__);//logování
	scGPImage_zamek_posunu->ClipFrameFillColor=(TColor)RGB(225,225,225);
	Akce=MOVE;
	kurzor(pan);
	TIP="Kliknutím a tažením myši posunujete celý objekt.";
	nahled_ulozit(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scButton_zamek_layoutuClick(TObject *Sender)
{
	if(scButton_zamek_layoutu->ImageIndex==68)//odemčeno budu zamykat
	{
		scButton_zamek_layoutu->ImageIndex=67;
		scButton_zamek_layoutu->Hint="Odemknout layout";
  }
	else
	{
		scButton_zamek_layoutu->ImageIndex=68;
		scButton_zamek_layoutu->Hint="Zamknout layout";
	}
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
if(scGPCheckBox_zobrazit_rotace_jigu_na_otocich->Checked) writeINI("nastaveni_editace","rotace_jigu", 1);
else  writeINI("nastaveni_editace","rotace_jigu", 0);
REFRESH();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::scGPCheckBox_zobrazit_poziceClick(TObject *Sender)
{
if(scGPCheckBox_zobrazit_pozice->Checked) writeINI("nastaveni_editace","zobrazeni_pozic", 1);
else  writeINI("nastaveni_editace","zobrazeni_pozic", 0);
REFRESH();
}
//---------------------------------------------------------------------------

