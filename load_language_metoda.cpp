//naète zvolený jazyk
unsigned short TForm1::load_language(Tlanguage language)
{
   //nastavení adresáøe k místì aplikace
  ChDir(ExtractFilePath(Application->ExeName));    //pøesune k EXE
  UnicodeString File_language= "Settings\\MK.language"; //cache_dir+"MK.language" už nenaèítám v tempu aplikace

  /*if(!FileExists(File_language)) //pokud bych chtìl jazyk stahovat
  {
	try
	{
		URLDownloadToFile(0,AnsiString("http://www.omapy.cz/files/CSIS/"+File_language).c_str(),File_language.c_str(),0,0);//pokud nebyl nalezen jazykový slovník, zkusí stáhnout
	}
	catch(...){;}
  }*/
  ls=new TStringList;
  for(unsigned short i=0;i<=114;i++)ls->Insert(i,"");//vyèištìní øetìzcù, ale hlavnì založení pro default! proto nelze použít  ls->Clear();


  if(FileExists(File_language))//znovu kontrola po pøípadném stažení souboru
  {
	//naètení jazykového slovníku do string listu
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
											   //pokud se nejedná o angliètinu
	Jazyk1->Caption=			ls->Strings[0];if(language>1)Jazyk1->Caption+=" (Language)";
	anglictina1->Caption=       ls->Strings[3];anglictina1->Caption+=" (EN)";
	mongolstina1->Caption=      ls->Strings[2];mongolstina1->Caption+=" (MN)";
	cestina1->Caption=          ls->Strings[1];cestina1->Caption+=" (CS)";
	//-----------------------
	//main menu
	Soubor->Caption=			ls->Strings[4];
	Konec1->Caption=			ls->Strings[5];
	//------
	Pohled1->Caption=			ls->Strings[6];
	Zoom_IN2->Caption=			ls->Strings[7];
	Zoom_OUT2->Caption=			ls->Strings[8];
	Posouvat1->Caption=			ls->Strings[9]+"...";
	Posunout2->Caption=			ls->Strings[10]+"...";
	Zoom_level_menu->Caption=	ls->Strings[11];
	ZoomOknem1->Caption=		ls->Strings[12]+"...";
	Hledat2->Caption=			ls->Strings[13]+"...";
	Predchozi_pohled1->Caption=	ls->Strings[14];
	PrejitNaGPS1->Caption=		ls->Strings[15];
	Prekreslit->Caption=		ls->Strings[16];
Nastznovupodkladovmapy1->Caption=ls->Strings[17];
	Transparentareas->Caption=	ls->Strings[18];
	HatchAreas->Caption=		ls->Strings[19];
	//------
	GPS1->Caption=				ls->Strings[20];
	OnOffGPS->Caption=			ls->Strings[21];
								ls->Strings[22];
	Nastaveni_gps_menu->Caption=ls->Strings[23]+"...";
	//-----------------------
	Nastaveni1->Caption=		ls->Strings[24];
	Clearmapscache1->Caption=   ls->Strings[25];
	Windowstayontop1->Caption=	ls->Strings[26];
	//-----------------------
	//pop-up Menu
	Addpoint->Caption=			ls->Strings[27];
	Deletepoint->Caption=		ls->Strings[28];
	Newpolygonspoints->Caption=	ls->Strings[29];
	Zoom_IN->Caption=			ls->Strings[30];
	Zoom_OUT->Caption=			ls->Strings[31];
	Pan->Caption=				ls->Strings[32];
	Posunout1->Caption=			ls->Strings[33];
	Vybratoknem_popup->Caption=	ls->Strings[34];
	identify1->Caption=			ls->Strings[35];
	SB(ls->Strings[36],5,false);
	//-------
	//hledání lokace
	Button_hledat->Caption=		ls->Strings[93];
	//-------

	//Panel vrstev
	Label_layers->Caption=		ls->Strings[94];

	CheckBoxDcmntP->Caption=	ls->Strings[95];
	CheckBox_ContSites->Caption=	ls->Strings[96];
	CheckBox_Indications->Caption=	ls->Strings[97];
	CheckBox_Sectors->Caption=	ls->Strings[98];
	CheckBox_Region->Caption=	ls->Strings[99];
	CheckBox_District->Caption=	ls->Strings[100];
	CheckBox_showhide_map->Caption=	ls->Strings[101];

	RadioGroup1->Caption=		ls->Strings[102];
	RadioButton1->Caption=		ls->Strings[103];
	RadioButton2->Caption=		ls->Strings[104];
	RadioButton3->Caption=		ls->Strings[105];
	RadioButton4->Caption=		ls->Strings[106];

	RadioGroup2->Caption=		ls->Strings[107];
	RadioButton5->Caption=		ls->Strings[103];
	RadioButton6->Caption=		ls->Strings[104];
	RadioButton7->Caption=		ls->Strings[105];
	RadioButton8->Caption=		ls->Strings[106];

	RadioGroup4->Caption=		ls->Strings[108];
	RadioButton9->Caption=		ls->Strings[103];
	RadioButton10->Caption=		ls->Strings[104];
	RadioButton11->Caption=		ls->Strings[105];

	RadioGroup3->Caption=		ls->Strings[109];
	RadioButton12->Caption=		ls->Strings[103];
	CheckBox_full_identify->Caption=ls->Strings[110];
	ListBox_location->Hint=		ls->Strings[112];
	//-------
	//FORM GPS SETTINGS v patøièném konstruktoru
	//Ostatní FORM tlaèítka  v patøièném konstruktoru daného FORM
	return 1;
  }
  else //pokud není nalezen jazykový slovník
  {
	//defaultní hlášky Form1
	unsigned short ui=36;
	ls->Insert(ui++,"NO GPS");
	ls->Insert(ui++,"Repeat or press ENTER or double click when finished | press ESC to cancel");
	ls->Insert(ui++,"The operation timed out. Check your network connection and try again!");
	ls->Insert(ui++,"Map error!");
	ls->Insert(ui++,"GPS connected");
	ls->Insert(ui++,"Off");
	ls->Insert(ui++,"On");
	ls->Insert(ui++,"GPS off");
	ls->Insert(ui++,"Make sure the GPS and bluetooth enabled.\nDo you want to set GPS?");
	ls->Insert(ui++,"GPS disconnected!");
	ls->Insert(ui++,"Click to add the new point of the polygon");
	ls->Insert(ui++,"Click to add the new point");
	ls->Insert(ui++,"move,add,delete points | move the polygon (SHIFT+drag&drop) | create a new polygon");
	ls->Insert(ui++,"Click to change the position");
	ls->Insert(ui++,"Click to add the new point of the polygon");
	ls->Insert(ui++,"Location not found!");
	ls->Insert(ui++,"Invalid request!");
	ls->Insert(ui++,"Internet connection not available!");
	ls->Insert(ui++,"Outside the area of interest!");
	ls->Insert(ui++,"about");
	ls->Insert(ui++,"Zoom");
	ls->Insert(ui++,"Zone");
	ls->Insert(ui++,"contributors");
	ls->Insert(ui++,"Do you want to close the window of the map?");
	ls->Insert(ui++,"Area size");
	ls->Insert(ui++,"Perimeter");
	ls->Insert(ui++,"km");
	ls->Insert(111 ,"Offline. The map may be not displayed!");
	ls->Insert(114,"Snap to");
	//defaultní hlášky gps_settings_form
	ui=79;
	ls->Insert(ui++,"COM");
	ls->Insert(ui++,"Search next");
	ls->Insert(ui++,"testing");
	ls->Insert(ui++,"GPS not found!");
	ls->Insert(ui++,"not found");
	ls->Insert(ui++,"m a.s.l.");
	ls->Insert(ui++,"E");
	ls->Insert(ui++,"W");
	ls->Insert(ui++,"N");
	ls->Insert(ui++,"S");
	ls->Insert(ui++,"GPS found");
	ls->Insert(ui++,"Search again");
	ls->Insert(ui++,"port found, GPS not found");
	ls->Insert(ui++,"not tested!");

	return 0; //naète defaultní jazykové nastavení tzn. AJ
  }
}