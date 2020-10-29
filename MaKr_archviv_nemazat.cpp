
//MAKR - archiv
 //log(__func__);

////  	//odeslání dat na FTP server
//	TIdFTP *FTP=new TIdFTP(this);
//	FTP->Host="lyzarskejihlavsko.cz";//FTP server
//	FTP->Username="hojkov@lyzarskejihlavsko.cz";
//	FTP->Password="modryextra";
//	FTP->TransferType=ftBinary;
//	FTP->Passive=true;//nutno
//	FTP->Connect();
//	//FTP->Put("C:\\Users\\Martin\\AppData\\Local\\Temp\\TISPL\\tispl_PrtScrMartin_MARTIN-NOTEBOOK.png");
//
//
//	HANDLE h;
//	WIN32_FIND_DATA wfd;
//	String text;
//	short pocet_souboru=-1;
//
//	h=FindFirstFile(L"*.*",&wfd);
//	if(h!=INVALID_HANDLE_VALUE)
//	{
//		while(FindNextFile(h,&wfd))
//		{
//			//text+=String(wfd.cFileName)+"\t";
//			if(++pocet_souboru>0)
//			{
//				FTP->Put(ExtractFilePath(Application->ExeName)+wfd.cFileName);
//			}
//		}
//		FindClose(h);
//	}
////
////	Memo(text);
//
//	FTP->Disconnect();
//	delete FTP;



// Form2->ShowModal();
//	d.v.vytvor_retez(d.v.POHONY->dalsi);
//	d.vykresli_retez(Canvas,d.v.POHONY->dalsi->retez);
////	d.v.vytvor_retez(d.v.POHONY->predchozi);
//	Memo("____-");
//	Cvektory::TRetez *R=d.v.POHONY->dalsi->retez;
//	while(R!=NULL)
//	{
//		Memo(R->eID);
//		R=R->dalsi;
//	}
//	delete R;
//	Memo("____________");
//	R=d.v.POHONY->predchozi->retez->dalsi;
//	while(R!=NULL)
//	{
//		Memo(R->eID);
//		R=R->dalsi;
//	}
//	delete R;

//		IdTime1->Host="128.138.140.44";//testovací TIME SERVER
//		TDateTime TIME=IdTime1->DateTime;
//		Sk(TIME);

	 //d.v.vloz_zpravu(0,0,0,NULL,ls->Strings[401]);
	 //d.v.vloz_zpravu(0,0,0,NULL,ls->Strings[402]);
//	 if(d.v.ZPRAVY!=NULL)
//	 {
//		 Cvektory::TZprava *Z=d.v.ZPRAVY->dalsi;
//		 while(Z!=NULL)
//		 {
//			 Memo(Z->n);Memo(d.v.getVID(Z->VID));if(Z->Element!=NULL){if(Z->Element->name=="")Memo(Z->Element->name);else {Memo(Z->Element->n);Memo(Z->Element->eID);}}else Memo("není");
//			 Memo("______________________");
//			 Z=Z->dalsi;
//		 }
//		 delete Z;
//	 }
	 //Sk(d.v.vrat_zpravu(2)->Popisek);
	 //d.v.vymazat_ZPRAVY();


//		Form_zpravy->scGPListBox_zpravy->Items->Clear();
//		Memo((short)Akce);
//		Memo((short) duvod_validovat);
//		Memo("_____________");

//d.TextOut(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,"Ahoj\ntoto je nějaký text řádku1\ntoto je nějaký text řádku 22\nhaf",Cvykresli::CENTER,Cvykresli::MIDDLE,-1);
//d.v.PP.uchyt_pozice=0.380/2.0;

	//d.v.generuj_VOZIKY();
	//d.vykresli_vyrobek(Canvas,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,0);



//duvod_validovat=2;
//REFRESH();



//		Memo("_____________________");
//		Cvektory::TVozik *V=d.v.VOZIKY->dalsi;
//		while(V!=NULL)
//		{
//			Memo(String(V->n)+" | "+V->temp+" | "+String(V->element->name)+" | "+String(V->element->objekt_n));
//			V=V->dalsi;
//		}
//		delete V;


//	Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
//	while(E!=NULL)
//	{
//		Memo(String(E->n)+" "+String(E->name));
//		E=d.v.dalsi_krok(E);
//	}
//	E=NULL;delete E; //pokud nedojde algoritmus na konec seznamu
//	delete E;E=NULL; //pokud dojde na konec seznamu
//
//	Memo("_________________");
//
//	//proleze např. spojku vícekrát
//	E=d.v.ELEMENTY->dalsi;
//	TPoint *tab_pruchodu=new TPoint[d.v.pocet_vyhybek+1];//.x uchovává počet průchodu přes výhybku, .y uchovává počet průchodů přes spojku
//	int n=0;
//	while(E!=NULL)
//	{
//		Memo(String(++n)+" "+String(E->n)+" "+String(E->name));
//		E=d.v.sekvencni_zapis_cteni(E,tab_pruchodu,NULL);//použití sekvenčního algoritmu, podle stejného bude soubor načítán, tz. stejný počet elementů v sekvenčním řazení
//	}
//	delete E;E=NULL;
//	delete []tab_pruchodu;

//	Tvlakno_obraz *vlakno=new Tvlakno_obraz(true);//spustí vlákno zajišťující stáhnutí mapového podkladu
//	vlakno->FreeOnTerminate=true;//po skončení bude uvolněno
//	vlakno->Start();
//	//delete vlakno;

		//Sk(ceil(m2px/Zoom/d.v.vrat_min_rychlost_prejezdu()*1000.0/fps));   //ceil(F->m.get_timePERpx(pom->RD,0,d.v.vrat_min_rychlost_prejezdu()));//různá rychlost dle RD, s afps se počítá dle min RD, ale nějak špatně vycházela animace ke konci (nestihl vozík vyjet)
		//Sk(Form_parametry_linky->scComboBox_vyber_produkt->ItemIndex);


//		TDateTime start;
//    String s;
//    Cvektory::TElement *E=d.v.ELEMENTY->dalsi,*e=NULL;
//    double cas=0,celkem_otevreni=0,celkem_zavreni=0;
//		unsigned int pocet_kroku=50;
//    for(unsigned int i=0;i<pocet_kroku;i++)
//    {
//        start=Now();
//        E=d.v.ELEMENTY->dalsi;
//        while(E!=NULL)
//        {
//            e=new Cvektory::TElement;d.v.kopiruj_element(E,e);
//            delete e;e=NULL;
//            E=E->dalsi;
//        }
//        delete E;E=NULL;
//        cas=ms.MyToDouble(TimeToStr(Now()-start).SubString(6,2));
//        celkem_otevreni+=cas;
//        Memo("Čas kopírování: "+AnsiString(cas));
//    }
//    Memo("------------");
//		Memo("Průměrný čas kopírování: "+AnsiString(celkem_otevreni/(double)pocet_kroku));


	//d.SCENA=22111;
//	MOD=SIMULACE;
//	d.SCENA=0;
//	if(d.SCENA>0)vytvor_statickou_scenu();//načtení statických záležitostí do statické scény, musí být před REFRESH
//	REFRESH();

//REFRESH(Edit1->Text.ToInt(),false);

//detekce test na první elementu v aktuálním projektu
//	 Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
//	 if(m.PtInSegment(E->geo.X1,E->geo.Y1,E->geo.typ,E->geo.orientace,E->geo.rotacni_uhel,E->geo.radius,E->geo.delka,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))Memo("v");
//	 else Memo("mimo");


///////////////////
	 //testovací hodnoty
//	 double a=1;//radius - E->geo.radius
//	 double b=1.2;//vzdálenost od bodu kliku ke středovému bod oblouku (ke středu kružnice, z které je oblouk tvořen) tj. vrátit si souřadnice středu (asi udělat ještě metodu
//	 double c=0.5;//vzdálenost mezi bodem kliku a výchozím bodem oblouku (E->geo.X1,E->geo.Y1) tj. = m.delka(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,E->geo.X1,E->geo.Y1)
//	 double uhel=m.AngleFromTriangle(a,b,c,3);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
//	 ShowMessage(uhel);
//	 double R=10;//E->geo.radius
//	 double RA=-90;//E->geo.rotacni_uhel //	double RA=F->Edit_rotace->Text.ToDouble();//rotační úhel, pod kterým je oblouk rotován - směřován (proti směru hodinových ručiček), může být záporný (po směru hodinových ručiček)
//	 double OR=90;//E->orientace
//	 double Xoblouku=40,Yoblouku=-30;//E->geo.X a E->geo.Y
//	 double uhel=m.uhelObloukuVsMys(Xoblouku,Yoblouku,OR,RA,R,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y);//úhel, mezi souřadnicemi myši, středem kružnice z které je tvořen oblouk a výchozím bodem oblouku, což je úhel i výstupní
//	 double delka=m.R2Larc(R,uhel);//požadovaná délka na oblouku vybraná myší, vracení délky dané výseče, tj. k na(při)počítání měřené délky
//	 d.vykresli_Gelement(Canvas,Xoblouku,Yoblouku,OR,RA,R,clBlue,2);//podkladový element (tj. normálně vykreslená linka)
//	 d.vykresli_Gelement(Canvas,Xoblouku,Yoblouku,OR,uhel,R,clRed,1,String(m.round2double(delka*1000,2))+" [mm]");//vykreslení měřícího kurzoru, metodu ještě vylepším
//																																							//výpis délky dané výseče
////	 TPointD S=m.getArcCenter(akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,OR,RA,R);
////	 Memo(String(S.x)+" "+String(S.y));
////	 d.line(Canvas,akt_souradnice_kurzoru_PX.x,akt_souradnice_kurzoru_PX.y,m.L2Px(S.x),m.L2Py(S.y));//pouze na testy označení stredu
////	//standardní vykreslení oblouku
//	float SA=Edit1->Text.ToDouble();//výchozí úhel, pod kterým oblouk začíná, musí být kladný - 0° je na 3 hodinách
//	d.set_pen(Canvas,clGreen,1/**F->Zoom*/,PS_ENDCAP_FLAT);
//	Canvas->MoveTo(m.L2Px(Xoblouku),m.L2Py(Yoblouku));//musí se přesunout pero na začátek, oblouku, v případě kontinuálního kreslení netřeba
//	Canvas->AngleArc(m.L2Px(Xoblouku),m.L2Py(Yoblouku),m.m2px(R),SA,RA);

//	d.v.ELEMENTY->dalsi->eID=401;
//	//d.v.smaz_element(d.v.ELEMENTY->dalsi);
//	vytvor_statickou_scenu();
//	REFRESH();

 //	 TPointD *souradnice_k_dalsimu_pouziti=vykresli_Gelement(canv,Xoblouku,Yoblouku,OR,RA,R,clBlue,1);
//	 F->Memo(String(souradnice_k_dalsimu_pouziti[3].x)+" "+String(souradnice_k_dalsimu_pouziti[3].y));
	//s-start, k-konec, 1-první úsečka, 2-druhá úsečka

	///////testovací vstupy - různé situace
	////test totožné (i protisměrné)
	//double xs1=0;double ys1=0;double xk1=20;double yk1=20;
	//double xs2=20;double ys2=20;double xk2=0;double yk2=0;
	////test totožné svislé (i protisměrné)
	//double xs1=0;double ys1=0;double xk1=0;double yk1=20;
	//double xs2=0;double ys2=20;double xk2=0;double yk2=0;
	////test totožné vodorovné (i protisměrné)
	//double xs1=0;double ys1=0;double xk1=20;double yk1=0;
	//double xs2=20;double ys2=0;double xk2=0;double yk2=0;
	////test rovnoběžné (i protisměrné)
	//double xs1=0;double ys1=0;double xk1=20;double yk1=20;
	//double xs2=20+20;double ys2=20;double xk2=0+20;double yk2=0;
	////mimoběžné, průsečík mimo úsečky, pouze přímek
	//double xs1=0;double ys1=0;double xk1=20;double yk1=20;
	//double xs2=20;double ys2=50;double xk2=40;double yk2=-50;
	//mimoběžné, průsečík i úsečky
	//double xs1=0;double ys1=0;double xk1=20;double yk1=20;
	//double xs2=20;double ys2=50;double xk2=10;double yk2=-50;
	//první svislá - hází na Y NAN
	//double xs1=0;double ys1=0;double xk1=0;double yk1=30;
	//double xs2=-10;double ys2=10;double xk2=10;double yk2=30;
	//druhá svislá - je ok
	//double xs1=-10;double ys1=10;double xk1=10;double yk1=30;
	//double xs2=0;double ys2=0;double xk2=0;double yk2=30;
	//kolmé obě - první svislá hazelo na Y NAN
	//double xs1=0;double ys1=0;double xk1=0;double yk1=30;
	//double xs2=-10;double ys2=10;double xk2=10;double yk2=10;
	//kolmé obě - druhá svislá je OK
	//double xs1=-10;double ys1=10;double xk1=10;double yk1=10;
	//double xs2=0;double ys2=0;double xk2=0;double yk2=30;
	//kolmé obě - MV test, původně hazelo na Y NAN, po změně pořadí bylo OK
//	double xs1=38;double ys1=-41;double xk1=38;double yk1=-37;
//	double xs2=35;double ys2=-39;double xk2=45;double yk2=-39;
//	double xs1=46;double ys1=-36;double xk1=46;double yk1=-32;
//	double xs2=43;double ys2=-34;double xk2=53;double yk2=-34;
//
//	//vykreslení testovacích úseček
//	d.line(Canvas,m.L2Px(xs1),m.L2Py(ys1),m.L2Px(xk1),m.L2Py(yk1));
//	d.line(Canvas,m.L2Px(xs2),m.L2Py(ys2),m.L2Px(xk2),m.L2Py(yk2));
//
//	//výpočet průsečíku
//	TPointD P=m.PrusecikPrimek(xs1,ys1,xk1,yk1,xs2,ys2,xk2,yk2);
//
//	//nutné ošetření výstupů pří různých situacícíh
//	if(IsNan(P.x) || IsNan(P.y))Memo("nemají průsečík, jsou totožné: "+String(P.x)+" "+String(P.y));
//	else if(IsInfinite(P.x) || IsInfinite(P.y))Memo("nemají průsečík, jsou rovnoběžné: "+String(P.x)+" "+String(P.y));
//	else Memo("mají průsečík: "+String(P.x)+" "+String(P.y));
//
//	if(!IsNan(P.x) && !IsNan(P.y) && !IsInfinite(P.x) && !IsInfinite(P.y))Memo("MV - mají průsečík");



	if(m.PtInTeplomer(d.v.ELEMENTY->dalsi->X,d.v.ELEMENTY->dalsi->Y,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y,d.v.ELEMENTY->dalsi->orientace))Memo("v");else Memo("mimo");

TDateTime start;
	String s;
	double cas=0,celkem_otevreni=0,celkem_zavreni=0;
	unsigned int pocet_kroku=10;
	for(unsigned int i=0;i<pocet_kroku;i++)
	{
		start=Now();
		pom=d.v.OBJEKTY->dalsi;//predchozi->predchozi->predchozi;
		otevri_editaci();
		cas=ms.MyToDouble(TimeToStr(Now()-start).SubString(6,2));
		celkem_otevreni+=cas;
		Memo("Čas otevření: "+AnsiString(cas));
		start=Now();
		scGPButton_stornoClick(this);
		cas=ms.MyToDouble(TimeToStr(Now()-start).SubString(6,2));
		celkem_zavreni+=cas;
		Memo("Čas zavření: "+AnsiString(cas));
	}
	Memo("------------");
	Memo("Průměrný čas otevření: "+AnsiString(celkem_otevreni/(double)pocet_kroku));
	Memo("Průměrný čas zavření: "+AnsiString(cas/(double)pocet_kroku));

	for(unsigned long i=0;i<=1000;i++)
	{
		Memo(m.RAND(1.1,9.1,1,true,false),false,false,true);
	}



		TColor clRetez=clBlue;if((int)m.RAND(1,2)%2)clRetez=clGreen;
		set_pen(canv,clRetez,F->Zoom*0.5*2);//nastavení pera
		Cvektory::TRetez *R=Retez->dalsi;
		unsigned long i=0;
		unsigned long pocet=4*Retez->predchozi->n-(Retez->predchozi->n-1);
		TPoint *POLE=new TPoint[pocet];
		while(R!=NULL)
		{
			//plnění jednoho segmentu do pole
			if(i==0)POLE[i]=TPoint(m.L2Px(R->geo.X1),m.L2Py(R->geo.Y1));
			POLE[i]=TPoint(m.L2Px(R->geo.X2),m.L2Py(R->geo.Y2));
			POLE[i]=TPoint(m.L2Px(R->geo.X3),m.L2Py(R->geo.Y3));
			POLE[i]=TPoint(m.L2Px(R->geo.X4),m.L2Py(R->geo.Y4));
			i++;
			//ukazatelové záležitosti
			R=R->dalsi;//posun na další element
		}
		//vykreslení celého segmentu
		canv->PolyBezier(POLE,pocet-1);


//	Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
//	while(E!=NULL)
//	{
////		if(floor(E->X)==41)// -0,0406901325296329)09523809524
////		{
////			Memo(String(E->n)+" "+E->name);
////		}
//		//if(E->name=="Zarážka 70") Memo(String(E->n)+" "+E->X);
//		E=d.v.dalsi_krok(E);
//	}
//	E=NULL;delete E;


//		pom_element=NULL;
		Cvektory::TElement *E=d.v.ELEMENTY->dalsi;
    Cvektory::T2Element *VYHYBKY=d.v.hlavicka_seznam_VYHYBKY();
//		{
//
//			if(d.v.oblast_elementu(E,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
//			{
////				//smazání zarážky v linii
////				if(E->geo.typ==0 && E->eID==MaxInt)
////				{
////					E->dalsi->geo.X1=E->geo.X1;
////					E->dalsi->geo.Y1=E->geo.Y1;
////					E->dalsi->geo.X2=E->geo.X1;
////					E->dalsi->geo.Y2=E->geo.Y1;
////					E->dalsi->geo.delka=m.delka(E->dalsi->geo.X1,E->dalsi->geo.Y1,E->dalsi->geo.X4,E->dalsi->geo.Y4);
////					d.v.smaz_element(E);
////				}
//
////přidání stoupání/klesání
//				double delka=E->geo.delka;
//				double HeightDeep=-0.7;
//				double delkaSklon=m.delkaSklon(delka,HeightDeep);//přepona
//				E->geo.delkaPud=delka;//jenom pokud je jiná než delka, jinak 0
//				E->geo.delka=delkaSklon;
//				E->geo.HeightDepp=HeightDeep;
////nulování stoupání/klesání
////				E->geo.delka=E->geo.delkaPud;
////				E->geo.delkaPud=0;
////				E->geo.HeightDepp=0;
//				duvod_validovat=2;
//				vytvor_statickou_scenu();
//				REFRESH();
//				ShowMessage("Nastaveno HeightDeep "+String(HeightDeep*1000)+" mm pro element "+E->name);
//				break;
//			}
//			E=d.v.dalsi_krok(E);
//		}
//		E=NULL;delete E;
		while(E!=NULL)
		{

			if(d.v.oblast_elementu(E,akt_souradnice_kurzoru.x,akt_souradnice_kurzoru.y))
			{
//				//smazání zarážky v linii
//				if(E->geo.typ==0 && E->eID==MaxInt)
//				{
//					E->dalsi->geo.X1=E->geo.X1;
//					E->dalsi->geo.Y1=E->geo.Y1;
//					E->dalsi->geo.X2=E->geo.X1;
//					E->dalsi->geo.Y2=E->geo.Y1;
//					E->dalsi->geo.delka=m.delka(E->dalsi->geo.X1,E->dalsi->geo.Y1,E->dalsi->geo.X4,E->dalsi->geo.Y4);
//					d.v.smaz_element(E);
//				}

//přidání stoupání/klesání
				double delka=E->geo.delka;
				double HeightDeep=-0.7;
				double delkaSklon=m.delkaSklon(delka,HeightDeep);//přepona
				E->geo.delkaPud=delka;//jenom pokud je jiná než delka, jinak 0
				E->geo.delka=delkaSklon;
				E->geo.HeightDepp=HeightDeep;
//nulování stoupání/klesání
//				E->geo.delka=E->geo.delkaPud;
//				E->geo.delkaPud=0;
//				E->geo.HeightDepp=0;
				duvod_validovat=2;
				vytvor_statickou_scenu();
				REFRESH();
				ShowMessage("Nastaveno HeightDeep "+String(HeightDeep*1000)+" mm pro element "+E->name);
				break;
			}
			E=d.v.dalsi_krok(VYHYBKY,E);
		}
		d.v.vymaz_seznam_VYHYBKY(VYHYBKY);
		delete VYHYBKY;VYHYBKY=NULL;
		E=NULL;delete E;