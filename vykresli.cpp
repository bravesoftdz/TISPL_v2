//---------------------------------------------------------------------------
#pragma hdrstop
#include "vykresli.h"
#include "Unit1.h"
#include "kabina_schema.h"
#include "stdlib.h"
//--------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//konstruktor
Cvykresli::Cvykresli()
{
	//parametry vykreslovaného obdelníku technologického objektu
	O_width=50;
	O_height=40;
	V_width=10;//parametry vykreslované spojky či vyhýbky
	//měřítko PX na MIN, globální proměná i pro využítí výpisu ve SB v Unit1
	PX2MIN=30.0;
	KrokY=30;//vizuální rozteč na ose Y mezi jednotlivými vozíky
	mod_vytizenost_objektu=false;
	NOLIEX=2;
	oY=5;//ofset na ose Y, 5 pouze grafická korekce
	grafickeDilema=true;
	Pom_proces=new Cvektory::TProces;
	precision=2;//počet desetinných míst čísel na časové ose
}
//---------------------------------------------------------------------------
//vrátí souřadnice dle typu buď středové nebo excentrické v podobě levého horního rohu objektu
int Cvykresli::CorEx(Cvektory::TObjekt *O)
{
	if(O->id!=F->VyID)//nejde o výhybku
	return m.L2Px(O->X)+O_width*Form1->Zoom/2;
	else//jde o výhybku
	return m.L2Px(O->X);
}
//---------------------------------------------------------------------------
//vrátí souřadnice dle typu buď středové nebo excentrické v podobě levého horního rohu objektu
int Cvykresli::CorEy(Cvektory::TObjekt *O)
{
	if(O->id!=F->VyID)//nejde o výhybku
	return m.L2Py(O->Y)+O_height*Form1->Zoom/2;
	else//jde o výhybku
	return m.L2Py(O->Y);
}
//---------------------------------------------------------------------------
void Cvykresli::vykresli_vektory(TCanvas *canv)
{
	F->Z("",false);//smazání přechozích zpráv
	//cesty ZAKAZeK - jsou li k dispozici
	if(v.ZAKAZKY!=NULL && v.ZAKAZKY->predchozi->n>0)
	{
		 Cvektory::TZakazka *Z=v.ZAKAZKY->dalsi;
		 while(Z!=NULL)//prochází seznam ZAKÁZEK, který obsahuje jednotlivé cesty
		 {
				Cvektory::TCesta *C=Z->cesta->dalsi;
				while(C!=NULL)//prochází jednotlivé prvky cesty
				{
					canv->Pen->Color=Z->barva;
					canv->Pen->Style=psSolid;
					canv->Pen->Width=m.round(2*Form1->Zoom);
					if(Z->barva==clGray)canv->Pen->Mode=pmCopy;else canv->Pen->Mode=pmNotXor;pmMask; //pokud se jedná o defaultní barvu vykopíruje se jina se vytvoří kombinace
					if(C->objekt->n==1)//pro situaci: z posledního prvku na první
					{
						//if(ukaz->n!=ukaz->predchozi->predchozi->n)//pokud jsou minimálně dva prky, ale šipka bude obousměrnná - možná žádoucí
						if(v.OBJEKTY->predchozi->n>=3)//až budou alespoň tři prvky,tj. poslední prvek bude mít index n větší než 3
						{
							canv->MoveTo(m.L2Px(C->predchozi->predchozi->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->predchozi->predchozi->objekt->Y)+O_height*Form1->Zoom/2);
							canv->LineTo(m.L2Px(C->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->objekt->Y)+O_height*Form1->Zoom/2);
							sipka(canv,m.L2Px((C->predchozi->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->predchozi->objekt->X,C->predchozi->predchozi->objekt->Y,C->objekt->X,C->objekt->Y));//zajistí vykreslení šipky - orientace spojovací linie
							if(Z->barva!=clGray)prislusnost_cesty(canv,Z->barva,m.L2Px((C->predchozi->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->predchozi->objekt->X,C->predchozi->predchozi->objekt->Y,C->objekt->X,C->objekt->Y),Z->n);
						}
					}
					else//pro běžné situace
					{
						canv->MoveTo(m.L2Px(C->predchozi->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->predchozi->objekt->Y)+O_height*Form1->Zoom/2);
						canv->LineTo(m.L2Px(C->objekt->X)+O_width*Form1->Zoom/2,m.L2Py(C->objekt->Y)+O_height*Form1->Zoom/2);
						sipka(canv,m.L2Px((C->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->objekt->X,C->predchozi->objekt->Y,C->objekt->X,C->objekt->Y));//zajistí vykreslení šipky - orientace spojovací linie
						if(Z->barva!=clGray)prislusnost_cesty(canv,Z->barva,m.L2Px((C->predchozi->objekt->X+C->objekt->X)/2)+O_width*Form1->Zoom/2,m.L2Py((C->predchozi->objekt->Y+C->objekt->Y)/2)+O_height*Form1->Zoom/2,m.azimut(C->predchozi->objekt->X,C->predchozi->objekt->Y,C->objekt->X,C->objekt->Y),Z->n);
					}
					C=C->dalsi;
				}
				Z=Z->dalsi;
		 }
	}
	else //pokud nejsou k dispozici nadefinované cesty vykreslí se přímo jen spojovací linie mezi objekty (tj. defaultní cesta)
	{
		Cvektory::TObjekt* ukaz=v.OBJEKTY->dalsi;//přeskočí hlavičku
		while (ukaz!=NULL)
		{
			canv->Pen->Style=psSolid;
			canv->Pen->Mode=pmCopy;
			canv->Pen->Width=m.round(1*Form1->Zoom);//musí být tady, jina to přebije nastavení metody sipka
			canv->Pen->Color=clGray;
			if(ukaz->n==1)//pro situaci z posledního prvku na první
			{
				//if(ukaz->n!=ukaz->predchozi->predchozi->n)//pokud jsou minimálně dva prky, ale šipka bude obousměrnná - možná žádoucí
				if(v.OBJEKTY->predchozi->n>=3)//až budou alespoň tři prvky,tj. poslední prvek bude mít index n větší než 3
				{
					canv->MoveTo(CorEx(ukaz->predchozi->predchozi),CorEy(ukaz->predchozi->predchozi));
					canv->LineTo(CorEx(ukaz),CorEy(ukaz));
					sipka(canv,(CorEx(ukaz->predchozi->predchozi)+CorEx(ukaz))/2,(CorEy(ukaz->predchozi->predchozi)+CorEy(ukaz))/2,m.azimut(ukaz->predchozi->predchozi->X,ukaz->predchozi->predchozi->Y,ukaz->X,ukaz->Y));//zajistí vykreslení šipky - orientace spojovací linie
				}
			}
			else
			{
				canv->MoveTo(CorEx(ukaz->predchozi),CorEy(ukaz->predchozi));
				canv->LineTo(CorEx(ukaz),CorEy(ukaz));
				sipka(canv,(CorEx(ukaz->predchozi)+CorEx(ukaz))/2,(CorEy(ukaz->predchozi)+CorEy(ukaz))/2,m.azimut(ukaz->predchozi->X,ukaz->predchozi->Y,ukaz->X,ukaz->Y));//zajistí vykreslení šipky - orientace spojovací linie
			}
			ukaz=ukaz->dalsi;//posun na další prvek
		}
	}

	////OBJEKTY
	//samotné objekty, kreslím až v samostatném následujícím cyklu, aby se nakreslilo do horní vrstvy
	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
	while (O!=NULL)
	{
		vykresli_rectangle(canv,O);//if(O->id!=F->VyID) se řeší až ve vykresli rectangle
		O=O->dalsi;//posun na další prvek
	}
	//povolení zobrazování LAYOUTU a ČASOVÝCH OS, pokud existují objekty, jinak ne
	if(v.OBJEKTY->dalsi!=NULL && !Form1->TZF)
	{
		if(v.OBJEKTY->predchozi->n>3)Form1->Layout->Enabled=true;else Form1->Layout->Enabled=false;///pokud je více jak 3 objekty
		if(DEBUG)Form1->Layout->Enabled=true;
		Form1->Analyza->Enabled=true;
	}
	else
	{
		Form1->Layout->Enabled=false;
		Form1->Analyza->Enabled=false;
	}
  if(F->scHTMLLabel_log_vypis->Caption=="")
	F->Z("<b>Linka v pořádku.</b>",false);
}
//---------------------------------------------------------------------------
//vykreslí barevný čtvereček jako příslušnost k dané cestě
void Cvykresli::prislusnost_cesty(TCanvas *canv,TColor Color,int X,int Y,float A,short N)
{
	short S=m.round(4*Form1->Zoom);//šířka čtverečku od středu
	short O=m.round(10*Form1->Zoom);//odsazení od šipky
	canv->Pen->Width=1;
	canv->Pen->Color=Color;
	canv->Pen->Style=psSolid;
	canv->Pen->Mode=pmCopy;//pmNotXor;//pmMask;
	canv->Brush->Color=Color;
	if(0<=A && A<15 || 135<=A && A<250 || 315<A && A<=360)canv->Rectangle(O+X-S+S*2*(N-1),Y-S,O+X+S+S*2*(N-1),Y+S);//vodorovně za
	if(15<=A && A<135 || 250<=A && A<315 )canv->Rectangle(X-S,O+Y-S+S*2*(N-1),X+S,O+Y+S+S*2*(N-1));//svisle pod
}
//---------------------------------------------------------------------------
//zajistí vykreslení šipky - orientace spojovací linie
void Cvykresli::sipka(TCanvas *canv, int X, int Y, float azimut, bool bez_vyplne, float size,COLORREF color,COLORREF color_brush,TPenMode PenMode,TPenStyle PenStyle)
{
	canv->Pen->Mode=PenMode;
	canv->Pen->Width=1;
	canv->Pen->Style=PenStyle;
	canv->Pen->Color=(TColor)color;
	canv->Brush->Color=(TColor)color_brush;
	size=m.round(size*2*Form1->Zoom);
	short sklon=230;

	//střed v těžišti
	if(!bez_vyplne)//barevná výplň trojúhelníku
	{
		POINT body[3]={{X+m.rotace(1,sklon,azimut).x*size,Y+m.rotace(1,sklon,azimut).y*size},{X+m.rotace(1,0,azimut).x*size,Y+m.rotace(1,0,azimut).y*size},{X+m.rotace(1,360-sklon,azimut).x*size,Y+m.rotace(1,360-sklon,azimut).y*size}};
		canv->Polygon((TPoint*)body,2);
	}
	else//transparentní střed
	{
		canv->MoveTo(X+m.rotace(1,sklon,azimut).x*size,Y+m.rotace(1,sklon,azimut).y*size);
		canv->LineTo(X+m.rotace(1,0,azimut).x*size,Y+m.rotace(1,0,azimut).y*size);
		canv->LineTo(X+m.rotace(1,360-sklon,azimut).x*size,Y+m.rotace(1,360-sklon,azimut).y*size);
		canv->LineTo(X+m.rotace(1,sklon,azimut).x*size,Y+m.rotace(1,sklon,azimut).y*size);
	}
}
//---------------------------------------------------------------------------
void Cvykresli::vykresli_rectangle(TCanvas *canv,Cvektory::TObjekt *ukaz)
{
	if(ukaz->id!=F->VyID)
	{
		//INFO: Zoom_predchozi_AA je v případě nepoužítí AA totožný jako ZOOM

		////referenčni bod jsem nakonce stanovil pravý konec levé packy
		TPoint S=m.L2P(ukaz->X,ukaz->Y);//Převede logické souřadnice na fyzické (displej zařízení), vrací fyzické souřadnice

		unsigned short W=O_width*Form1->Zoom;
		unsigned short H=O_height*Form1->Zoom;

//		if(Form1->zobrazit_barvy_casovych_rezerv)
//		{
			TColor errorColor=set_color(canv,ukaz);
			if(errorColor!=0)
			{
			unsigned short O=m.round(6*Form1->Zoom);//Okraj nutno zaokrouhlit tady
			canv->Rectangle(S.x-O,S.y-O,S.x+W+O,S.y+H+O);
      }
//		}

		////obdelník objektu
		canv->Pen->Style=psSolid;
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=(TColor)RGB(19,115,169);//(TColor)RGB(254,254,254);//nemuže být čiště bílá pokud je zapnut antialising, tak aby se nezobrazoval skrz objekt grid
		canv->Pen->Color=(TColor)RGB(19,115,169);//clBlack;
		canv->Pen->Mode=pmCopy;
		canv->Pen->Width=m.round(2*Form1->Zoom);
		canv->Rectangle(S.x,S.y,S.x+W,S.y+H);

		////packy
		/*unsigned short packy_W=5*Form1->Zoom;
		canv->Pen->Width=1*Form1->Zoom;
		canv->MoveTo(S.x-packy_W,S.y+H/2);canv->LineTo(S.x,S.y+H/2);
		canv->MoveTo(S.x+W,S.y+H/2);canv->LineTo(S.x+W+packy_W,S.y+H/2);*/

		////nastavení písma
		canv->Font->Name="Arial";//canv->Font->Name="MS Sans Serif";
		if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)
		{
			canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
			//asi nepřináší zcela přínos
			canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;
			canv->Font->Size=11*3+3;//+3 grafická korekce protože při AA dochází ke zmenšení písma
		}
		else
		{
			//asi nepřináší zcela přínos
			canv->Font->Pitch = TFontPitch::fpVariable;//každé písmeno fontu stejně široké
			canv->Font->Pitch = System::Uitypes::TFontPitch::fpVariable;
			canv->Font->Size=12;//tady zajist rozšíření písma
		}
		rotace_textu(canv,0);

		//barva
		canv->Font->Color=(TColor)RGB(254,254,254);//clBlack;//nemuže být čiště bílá pokud je zapnut antialising, tak aby se nezobrazoval skrz objekt grid

		short zAA=1;//zvětšení pro antialising, jinak 1
		if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)zAA=3;

		////samotný text - pro jednotlivé zoomu různé podoby výpisu
		//název objektu
		if(Form1->Zoom_predchozi_AA>1)
		{
		 canv->Font->Style = TFontStyles()<< fsBold;//zapnutí tučného písma
		 if(Form1->Zoom_predchozi_AA==1.5)	drawRectText(canv,TRect(S.x,S.y,S.x+W,S.y+H),ukaz->name.UpperCase());//zajistí vykreslení textu vycentrovaného vevnitř objektu/obdelníku
		 else
		 {
			canv->Font->Size=canv->Font->Size; //*(Form1->Zoom_predchozi_AA-1) - zatím nepoužito
			canv->TextOutW(S.x+4*zAA,S.y+2*zAA,ukaz->name.UpperCase());
		 }

		 canv->Font->Style = TFontStyles();//vypnutí tučného písma
		 if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)canv->Font->Size=8*3+2;//+3 grafická korekce protože při AA dochází ke zmenšení písma
		}

		//vypis další datových položek
		if(Form1->Zoom_predchozi_AA>1.5)
		{
		 UnicodeString T=""; unsigned short R=20;//řádkování

		 bool CTunit=0;AnsiString CTunitT="[s]";
		 bool aRDunit=0;AnsiString aRDunitT="[m/s]";
		 bool DDunit=0;AnsiString DDunitT="[m]";
		 if(Form1->readINI("nastaveni_form_parametry", "CT") == "1"){CTunit=1;CTunitT="[min]";}
		 if(Form1->readINI("nastaveni_form_parametry", "RDt") == "1"){aRDunit=1;aRDunitT="[m/min]";}
		 if(Form1->readINI("nastaveni_form_parametry", "DD") == "1") {DDunit=1;DDunitT="[mm]";}

		 switch(ukaz->rezim)
		 {
			case 0:T="STOP & GO";break;
			case 1:T="KONTINUÁLNÍ";break;
			case 2:T="POSTPROCESNÍ";break;
		 }
		 canv->TextOutW(S.x+4*zAA,S.y+R*zAA,T);//výpis režimu
		 if(ukaz->pohon==NULL)
		 {
			 canv->Font->Color=clRed;
			 canv->Font->Style = TFontStyles()<< fsBold;//zapnutí tučného písma
			 canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"POHON NEPŘIŘAZEN");//pohon name
			 canv->Font->Style = TFontStyles();//vypnutí tučného písma
			 canv->Font->Color=(TColor)RGB(254,254,254);
		 }
		 else canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,ukaz->pohon->name);//pohon name
		 if(F->STATUS!=Form1->OVEROVANI && Form1->Zoom_predchozi_AA>2 && ukaz->pohon!=NULL)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"RP: "+F->m.round2double(ukaz->pohon->aRD*(1+aRDunit*59.0),3,"..")+" "+aRDunitT);
		 if(F->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"DD: "+F->m.round2double(ukaz->delka_dopravniku*(1+DDunit*999.0),3,"..")+" "+DDunitT);
		 if(F->STATUS!=Form1->OVEROVANI && Form1->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"CT: "+F->m.round2double(ukaz->CT/(1+CTunit*59.0),3,"..")+" "+CTunitT);
		 if(F->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"Kap.: "+F->m.round2double(ukaz->kapacita,3,"..")+" [v+mz]");
		 if(F->Zoom_predchozi_AA>2)canv->TextOutW(S.x+4*zAA,S.y+(R+=15)*zAA,"Poz.: "+F->m.round2double(ukaz->pozice,3,"..")+" [v]");
		}

    //pro největší oddálení zobrazí jenom zkratku objektu
		if(Form1->Zoom_predchozi_AA<=1)
		{
			if(Form1->Zoom_predchozi_AA==1)canv->Font->Style = TFontStyles()<< fsBold;else canv->Font->Style = TFontStyles();
			if(Form1->Zoom_predchozi_AA==0.25)
			drawRectText(canv,TRect(S.x,S.y,S.x+W,S.y+H),ukaz->short_name.SubString(1,1));
			else
			{
				if(Form1->antialiasing && Form1->Akce!=Form1->ADD && Form1->Akce!=Form1->MOVE)//při antialiasingu se zobrazuje popisek trochu jinak
				drawRectText(canv,TRect(S.x,S.y,S.x+W,S.y+H),ukaz->short_name.UpperCase());//zajistí vykreslení textu vycentrovaného vevnitř objektu/obdelníku
				else
				drawRectText(canv,TRect(S.x-1,S.y,S.x+W,S.y+H),ukaz->short_name.UpperCase());//zajistí vykreslení textu vycentrovaného vevnitř objektu/obdelníku
			}
		}
	}
	else
	vykresli_kruh(canv,ukaz);
}
////---------------------------------------------------------------------------
void Cvykresli::vykresli_kruh(TCanvas *canv, Cvektory::TObjekt *O)
{
		//INFO: Zoom_predchozi_AA je v případě nepoužítí AA totožný jako ZOOM

		////referenčni bod jsem nakonce stanovil pravý konec levé packy
		TPoint S=m.L2P(O->X,O->Y);//Převede logické souřadnice na fyzické (displej zařízení), vrací fyzické souřadnice

		unsigned short W=V_width*Form1->Zoom;

		////obdelník objektu
		canv->Pen->Style=psSolid;
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=(TColor)RGB(19,115,169);//(TColor)RGB(254,254,254);//nemuže být čiště bílá pokud je zapnut antialising, tak aby se nezobrazoval skrz objekt grid
		canv->Pen->Color=(TColor)RGB(19,115,169);//clBlack;
		canv->Pen->Mode=pmCopy;
		canv->Pen->Width=m.round(2*Form1->Zoom);
		canv->Ellipse(S.x-W,S.y-W,S.x+W,S.y+W);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvykresli::drawRectText(TCanvas *canv,TRect Rect,UnicodeString Text)
{
		canv->TextOutW(Rect.Left+Rect.Width()/2-canv->TextWidth(Text)/2,Rect.Top+Rect.Height()/2-canv->TextHeight(Text)/2,Text);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvykresli::vykresli_grid(TCanvas *canv, int size_grid)
{
		//if(Form1->Zoom==1.75 || Form1->Zoom==0.75)bacha u těchto hodnot dochází ke špatnému vykreslování asi zaokrouhlouvaním
		for(int x=m.round((m.round(-1*Form1->Posun.x)%size_grid)*Form1->Zoom);x<=Form1->ClientWidth;x+=m.round(size_grid*Form1->Zoom))
		for(int y=m.round((m.round(-1*Form1->Posun.y)%size_grid)*Form1->Zoom);y<=Form1->ClientHeight;y+=m.round(size_grid*Form1->Zoom))
		{
			canv->Pixels[x][y]=clGray; //při změně barvy nutno provést změnu barevného filtru v podmínce v antialiasingu
		}
}
////---------------------------------------------------------------------------
//void Cvykresli::vykresli_graf_rezervy(TCanvas *canv)
//{
//	 SetBkMode(canv->Handle,OPAQUE);
//	 canv->Font->Size=12;
//	 canv->Font->Name="Arial";
//	 UnicodeString text_horizontal="časová osa procesů";
//	 UnicodeString text_vertical="TAKT TIME [čas na vozík]";
//	 unsigned int L=/*Form1->RzSizePanel_knihovna_objektu->Width+*/canv->TextHeight(text_vertical)+3;
//	 unsigned int P=Form1->ClientWidth-1;
//	 unsigned int D=Form1->ClientHeight-Form1->scGPPanel_statusbar->Height-canv->TextHeight(text_horizontal)-1;
//	 unsigned int H=Form1->RzToolbar1->Height;
//	 unsigned int offset_horizont=0;//bylo 20 - nyní nepoužívám, pouze vizuální záležitost na ose X, pro graf, aby nebyl až ke kraji
//	 unsigned int offset_vertical=100;
//	 double akt_max_TTo=0;
//	 bool graficka_legenda=false;
//	 v.get_LT_a_max_min_TT();
//
//	 ////objekty
//		canv->Brush->Style=bsSolid;
//		canv->Font->Color=clWhite;
//		canv->Font->Style = TFontStyles() /*<< fsItalic << fsBold*/;
//		canv->Font->Size=14;
//		rotace_textu(canv,900);
//
//		Cvektory::TObjekt *ukaz;
//		unsigned int sirka_uzita=L;
//		ukaz=v.OBJEKTY->dalsi;//přeskočí hlavičku
//		if(ukaz==NULL){v.MAX_TT=0;v.MIN_TT=0;}//pokud neexistuje žádný objekt, ošetření, aby se nic nezobrazovalo v grafu
//		while (ukaz!=NULL)//pokud existuje nějaký prvek
//		{
//			if(ukaz->TTo>0)//vykreslí, jenom pokud má objekt TTo>0
//			{
//				//sloupce
//				set_color(canv,ukaz->TTo);
//				canv->Pen->Style=psSolid;
//				canv->Brush->Style=bsSolid;
//				int sirka_sloupce=m.round((P-offset_horizont-L)*ukaz->CT/v.LT);
//				int vyska_sloupce=m.round((D-H-offset_vertical)*ukaz->TTo/v.MAX_TT);//pokud bych chtěl rovnoměrně rozdělovat tak jako u sirka_sloupce:unsigned int vyska_sloupce=m.round((D-H-offset_vertical)*ukaz->time/LT);
//				canv->Rectangle(sirka_uzita,D-vyska_sloupce,sirka_uzita+sirka_sloupce,D);
//				//zajistí grafické oddělení sloupců bílá svislá čára
//				canv->Pen->Color=clWhite;
//				canv->Pen->Width=1;
//				canv->Pen->Style=psSolid;
//				canv->MoveTo(sirka_uzita,D-vyska_sloupce);
//				canv->LineTo(sirka_uzita,D);
//
//				////vykreslení doporučeného TTo či buffrování,je-li důvod,tzn. je-li výška slopuce pod TT
//				akt_max_TTo=(ukaz->TTo > akt_max_TTo) ? ukaz->TTo : akt_max_TTo;//přiřadí aktuálně nejvyšší TT
//				if(akt_max_TTo!=ukaz->TTo)graficka_legenda=true;
//				//doporučení zvýšení TTo
//				canv->Brush->Style=bsDiagCross;
//				canv->Rectangle(sirka_uzita,D-vyska_sloupce,sirka_uzita+sirka_sloupce,D-m.round((D-H-offset_vertical)*akt_max_TTo/v.MAX_TT));
//				//doporučení BUFFROVANÍ či STOP STANIC
//				if(ukaz->dalsi!=NULL && akt_max_TTo<ukaz->dalsi->TTo)
//				{
//					canv->Brush->Style=bsCross;
//					canv->Rectangle(sirka_uzita,D-m.round((D-H-offset_vertical)*akt_max_TTo/v.MAX_TT),sirka_uzita+sirka_sloupce,D-m.round((D-H-offset_vertical)*ukaz->dalsi->TTo/v.MAX_TT));
//				}
//
//				/*//popisek
//				if(ukaz->rezim==0)//S&G
//				{
//						//
//				}
//				else//kontinual, dále postprocesní nemusím z principu řešit
//				{
//					 //
//				}
//				////----
//				*/
//				UnicodeString Popisek=ukaz->name.UpperCase();//standardně celý název jinak zkratka, pokud se nevejde do sloupce
//				if(vyska_sloupce<canv->TextWidth(Popisek))Popisek=ukaz->short_name;
//				if(D-vyska_sloupce/2+canv->TextWidth(Popisek)/2<D)//zajišťuje, aby byl popisek vždy nad osou
//					canv->TextOutW((sirka_uzita+sirka_uzita+sirka_sloupce)/2-canv->TextHeight(Popisek)/2,D-vyska_sloupce/2+canv->TextWidth(Popisek)/2,Popisek);//popisek
//				else
//				canv->TextOutW((sirka_uzita+sirka_uzita+sirka_sloupce)/2-canv->TextHeight(Popisek)/2,D-1,Popisek);//popisek
//
//				sirka_uzita+=sirka_sloupce;
//
//			}
//			ukaz=ukaz->dalsi;//posun na další prvek
//		}
//		//vypnutí tučného písma
//		canv->Font->Style = TFontStyles();
//		canv->Font->Size=12;
//	 ////----------
//
//	 ////legenda a osy
//		canv->Pen->Style=psClear;
//		canv->Font->Color=clGray;
//		canv->Font->Size=9;
//		canv->Pen->Color=clGray;
//		canv->Brush->Color=clGray;
//		rotace_textu(canv,0);
//		//grafická legenda
//		if(graficka_legenda)
//		{
//				canv->Brush->Style=bsDiagCross;
//				UnicodeString P1="zvýšit TAKT TIME objektu";
//				UnicodeString P2="zařadit BUFFER/STOP STANICI";
//				unsigned short T_W=(canv->TextWidth(P1)>canv->TextWidth(P2)) ? canv->TextWidth(P1):canv->TextWidth(P2);//přiřadí aktuálně nejvyšší TT
//				unsigned short T_H=canv->TextHeight(P1);
//				canv->TextOutW(P-T_W-2,(H+10+H+20+10)/2-T_H/2,P1);
//				canv->Rectangle(P-50-T_W-4,H+10,P-T_W-4,H+20+10);
//				canv->Brush->Style=bsCross;
//				canv->TextOutW(P-T_W-2,(H+20+20+H+20+20+20)/2-T_H/2,P2);
//				canv->Rectangle(P-50-T_W-4,H+20+20,P-T_W-4,H+20+20+20);
//		}
//
//		//osy a nastavení
//		canv->Pen->Width=1;
//		canv->Pen->Style=psSolid;
//		canv->Brush->Color=clWhite;
//		//vodorovná osa
//		canv->MoveTo(L,D);canv->LineTo(P,D);//linie
//		canv->LineTo(P-10,D-10);canv->MoveTo(P,D);canv->LineTo(P-10,D+10);//šipka
//		canv->TextOutW((L+P)/2-canv->TextWidth(text_horizontal)/2,D+1,text_horizontal);//popisek
//		//svislá osa
//		canv->MoveTo(L,D);canv->LineTo(L,H);//linie
//		canv->LineTo(L-10,H+10);canv->MoveTo(L,H);canv->LineTo(L+10,H+10);//šipka
//		rotace_textu(canv,900);canv->TextOutW(L-1-canv->TextHeight(text_vertical),(D+H)/2+canv->TextWidth(text_vertical)/2,text_vertical);//popisek
//		//další popisky a osy
//		canv->Pen->Style=psDot;
//		rotace_textu(canv,0);
//		UnicodeString T="";
//		//min TTo
//		if(v.MIN_TT!=Form1->PP.TT && v.MAX_TT!=v.MIN_TT )//zobrazí osu s popiskem pouze pokud nebude stejná jako TT a zároveň MAX_TT
//		{
//			canv->MoveTo(L,D-m.round((D-H-offset_vertical)*v.MIN_TT/v.MAX_TT));canv->LineTo(P,D-m.round((D-H-offset_vertical)*v.MIN_TT/v.MAX_TT));
//			T="min. TAKT TIME objektů";
//			canv->TextOutW(P-canv->TextWidth(T)-1,D-m.round(D-H-offset_vertical)*v.MIN_TT/v.MAX_TT-canv->TextHeight(T),T);
//		}
//		//max TTo
//		if(v.MAX_TT!=Form1->PP.TT)//zobrazí osu s popiskem pouze pokud nebude stejná jako TT
//		{
//			canv->MoveTo(L,D-m.round(D-H-offset_vertical));canv->LineTo(P,D-m.round(D-H-offset_vertical));
//			if(v.MAX_TT!=0)T="uskutečnitelný (max.) TAKT TIME objektů";else T="požadovaný TAKT TIME";
//			canv->TextOutW(P-canv->TextWidth(T)-1,D-m.round(D-H-offset_vertical)-canv->TextHeight(T),T);
//			T="požadovaný TAKT TIME";
//		}
//		else T="požadovaný (uskutečnitelný) TAKT TIME";
//		//TT osa
//		canv->Pen->Style=psDash;
//		if(v.MAX_TT>0)
//		{
//			canv->MoveTo(L,D-m.round((D-H-offset_vertical)*Form1->PP.TT/v.MAX_TT));canv->LineTo(P,D-m.round((D-H-offset_vertical)*Form1->PP.TT/v.MAX_TT));
//			canv->TextOutW(P-canv->TextWidth(T)-1,D-m.round(D-H-offset_vertical)*Form1->PP.TT/v.MAX_TT-canv->TextHeight(T),T);
//    }
//		//vypis hodnoty LT
//		T="LEAD TIME: "+UnicodeString(v.LT)+" [min]";
//		canv->TextOutW(P-canv->TextWidth(T)-11,D+1,T);
//		//Legenda pravý horní roh
//		unsigned short n=0;
//		canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"požadovaný TAKT TIME: "+UnicodeString(Form1->PP.TT)+" [min/vozík]");
//		canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"uskutečnitelný (max.) TAKT TIME: "+UnicodeString(v.MAX_TT)+" [min/vozík]");
//		if(Form1->PP.TT>0)
//		{
//			//canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"Počet realizovaných produktů v čase:...[ks]");
//			canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"WIP: "+UnicodeString((1/Form1->PP.TT)*v.LT)+" [vozíků]");
//			if(v.LT>0)canv->TextOutW(L+10,H+canv->TextHeight(T)*++n,"PCE: "+UnicodeString(m.round2double(v.sum_WT()/v.LT*100,2))+" [%]");//zokrouhleno na dvě desetinná místa
//		}
//		canv->Brush->Style=bsSolid;
//	 ///------
//}
////---------------------------------------------------------------------------
//v případě měření vzdálenosti vykreslí spojnici a popř. vypisuje hodnotu vzdálenosti
void Cvykresli::vykresli_meridlo(TCanvas *canv,int X,int Y)
{
		bool popisek_napravo=true;
		if(X<=F->vychozi_souradnice_kurzoru.x)popisek_napravo=false;

		canv->Font->Size=12;
		canv->Font->Name="Arial";
		canv->Font->Color=clWhite;

		canv->Brush->Color=clWhite;
		//canv->Brush->Style=bsClear;

		//délka text předchozí smazání
		double delka=m.round2double(m.delka(m.P2Lx(F->vychozi_souradnice_kurzoru.X),m.P2Ly(F->vychozi_souradnice_kurzoru.Y),m.P2Lx(F->minule_souradnice_kurzoru.x),m.P2Ly(F->minule_souradnice_kurzoru.y)),2);
		long Xt=F->minule_souradnice_kurzoru.x;
		short Z=1;//znamenko
		if(!popisek_napravo)
		{
			Xt=Xt-canv->TextWidth(AnsiString(delka)+" [m]");
			Z=-1;
		}

		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-10,AnsiString(delka)+" [m]");

		canv->TextOutW(Xt+9*Z,F->minule_souradnice_kurzoru.y-10,AnsiString(delka)+" [m]");
		canv->TextOutW(Xt+11*Z,F->minule_souradnice_kurzoru.y-10,AnsiString(delka)+" [m]");
		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-9,AnsiString(delka)+" [m]");
		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-11,AnsiString(delka)+" [m]");
		canv->TextOutW(Xt+10*Z,F->minule_souradnice_kurzoru.y-8,AnsiString(delka)+" [m]");

//	 nedoděláno	HRGN hreg_old=CreateRectRgn(Xt+10*Z,F->minule_souradnice_kurzoru.y-10,F->minule_souradnice_kurzoru.x+10+canv->TextWidth(AnsiString(delka)+" [m]  "),F->minule_souradnice_kurzoru.y-10+canv->TextHeight(AnsiString(delka)+" [m]"));
		delka=m.round2double(m.delka(m.P2Lx(F->vychozi_souradnice_kurzoru.X),m.P2Ly(F->vychozi_souradnice_kurzoru.Y),m.P2Lx(X),m.P2Ly(Y)),2);
		if(popisek_napravo)Xt=X+10;else Xt=X-10-canv->TextWidth(AnsiString(delka)+" [m]");
//	 nedoděláno	HRGN hreg_new=CreateRectRgn(Xt,Y-10,X+10+canv->TextWidth(AnsiString(delka)+" [m]"),Y-10+canv->TextHeight(AnsiString(delka)+" [m]"));
//		HRGN hreg_diff=CreateRectRgn(0,0,0,0);
//		CombineRgn(hreg_diff,hreg_old,hreg_new,RGN_DIFF);
//		InvalidateRgn(F->Handle,hreg_diff,TRUE);
		//InvertRgn(canv->Handle,hreg_diff);//označí region)

		//délka text aktuální
		//canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		canv->Font->Color=m.clIntensive(clRed,100);
		canv->TextOutW(Xt,Y-10,AnsiString(delka)+" [m]");

		//linie
		set_pen(canv, m.clIntensive(clRed,100),10,PS_ENDCAP_FLAT);
		canv->Pen->Mode=pmNotXor;
		canv->MoveTo(F->vychozi_souradnice_kurzoru.x,F->vychozi_souradnice_kurzoru.y);canv->LineTo(F->minule_souradnice_kurzoru.x,F->minule_souradnice_kurzoru.y);
		F->minule_souradnice_kurzoru=TPoint(X,Y);
		canv->MoveTo(F->vychozi_souradnice_kurzoru.x,F->vychozi_souradnice_kurzoru.y);canv->LineTo(X,Y);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////celkové vykreslení módu časové osy - MaRO algoritmus
void Cvykresli::vykresli_casove_osy(TCanvas *canv)
{
	if(!mod_vytizenost_objektu)//pokud se bude jednat o zobrazení formou časových os, nikoliv formou vytížení objektů
	{
		//nastavení do výchozí stavu, zajištěno pro nový výpočet
		v.vymazat_casovou_obsazenost_objektu_a_pozice_voziku(v.OBJEKTY,v.VOZIKY);//vymaže předchozí časovou obsazenost objektů, jinak by se při každém dalším překreslení objekty posovali o obsazenost z předchozího vykreslení
		if(!JIZPOCITANO)//aby se znovu nevypočítávalo, pokud je již spočítáno a není třeba přepočítávat (např. z důvodu nového volání časových os či změny parametrů)
		{
			v.vymaz_seznam_PROCESY();
			v.hlavicka_PROCESY();//vymaže uložené procesy //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
			for(short i=0;i<8;i++)legenda_polozky[i]=false;//nastaví všechny položky na false
		}
		Pom_proces=v.PROCESY->dalsi;//pomocný ukazatel na (v tomto případě na první)proces, využívá se v při načítání původně vytvořených náhodných hodnot čekání na palec
		double X=0;//výchozí odsazení na ose X
		//KrokY je vizuální rozteč na ose Y mezi jednotlivými vozíky zadáno globálně, kvůli jednotlivým krokům + používáno v Unit1 např. na posun obrazu po vozících
		long Y=Form1->scGPPanel_mainmenu->Height+oY;//+5=oY pouze grafická korekce
		Cvektory::TZakazka *Z=v.ZAKAZKY->dalsi;
		while(Z!=NULL)//jde po zakázkách
		{
			long Yloc;
			Cvektory::TCesta *C=Z->cesta->dalsi;
			while(C!=NULL)//jde po konkrétní cestě
			{
				Yloc=Y;
				unsigned int n=0;//pořádí v rámci zakázky
				Cvektory::TVozik *vozik=v.VOZIKY->dalsi;//ukazatel na první objekt v seznamu VOZÍKŮ, přeskočí hlavičku
				while (vozik!=NULL)//jde po pořadí vozíků, které řeší jenom konkrétní cestu dané zakázky
				{
					if(vozik->zakazka->n==Z->n)//řeší jenom pro konrétní cestu, pokud tedy vozík odpovídá aktuální zakázce
					{
						////nastvení počáteční ale i následující pozice objektu na časové ose daného vozíku
						if(vozik->pozice==-1)vozik->pozice=0;//protože implicitní hodnota pozice vozíku==-1 z důvodu mimo linku, 0 na začátku ve výchozí pozici
						if(C->objekt->rezim==0 && vozik->pozice<=C->objekt->obsazenost)//zohlednění obsazenost objektu v režimu S&G
							X=C->objekt->obsazenost;//převezme se koncová pozice v objektu z předchozího vozíku
						else//pro ostatní režimy
						{
							if(vozik->pozice==0 && vozik->n!=1)//nejednáli se o první vozík a zároveň se jedná o výchozí pozici v režimu kontinuál či pp (sice trochu nelogické, ale může se uvažovat že navěšování bude kontinuál)
							X=vozik->predchozi->start+C->objekt->CT/C->objekt->kapacita/60.0*PX2MIN;//tak stanový výchozí pozici dle CT a kapacity (např. při 3min CT a kapacitě 2 je časový offset mezi vozíky 1,5 min)
							else//ostatní pozice
							X=vozik->pozice;
						}
//						unsigned int WIP=v.WIP(1);
//						if(vozik->pozice==0 && vozik->n>=WIP+1)
//						X=v.vrat_start_a_pozici_vozikuPX(vozik->n-WIP).x;

						////uložení hodnot pro další využítí
						if(C->n==1)vozik->start=X;//uloží výchozí X hodnotu, prvního objektu pro daný vozík
						double X_predchozi=X;//uloží povodní X hodnotu

						////BUFFER vykreslení a uložení buffer pokud předchází
						if(vozik->pozice<X && vozik->pozice>0)
						vykresli_proces(canv,"BUF - "+C->predchozi->objekt->short_name,m.clIntensive(vozik->zakazka->barva,80),4,vozik->pozice-PosunT.x,X-PosunT.x,Yloc-PosunT.y);
						//Nefunguje zatím správněCvektory::TProces *P=new Cvektory::TProces;
						//P->n_v_zakazce=n+1;P->Tpoc=vozik->pozice-PosunT.x/PX2MIN;P->Tkon=X-PosunT.x/PX2MIN;P->Tdor=P->Tkon;P->Tpre=P->Tkon;P->Tcek=P->Tkon;P->cesta=C;P->vozik=vozik;
						//v.vloz_proces(P);

						////čekání na čištění pistole a výměnu barev včetně čekání v rámci OBJEKTU
						if(Form1->CheckBoxVymena_barev->Checked)
						{
								if(C->Opak!=0)
								if(n%C->Opak==0 && n!=0)//čištění, mimo první vozík protože buď je připravená linka (v případě první zakázky nebo je čištění součástí mezizakázkové výměny barev)
								{
									vykresli_proces(canv,"Č",m.clIntensive(vozik->zakazka->barva,-20),5,X-PosunT.x,X+C->Tc/60.0*PX2MIN-PosunT.x,Yloc-PosunT.y);
									X+=C->Tc/60.0*PX2MIN;
									if(!legenda_polozky[1])legenda_polozky[0]++;legenda_polozky[1]=true;
								}
								if(n==0 && Z->n>1)//výměna barev + čistění, mimo první zakázku, u té předpokládáme připravenost linky
								{
									vykresli_proces(canv,"V+Č",m.clIntensive(vozik->zakazka->barva,-40),4,X-PosunT.x,X+C->Tv/60.0*PX2MIN-PosunT.x,Yloc-PosunT.y);
									X+=C->Tv/60.0*PX2MIN;
									if(!legenda_polozky[2])legenda_polozky[0]++;legenda_polozky[2]=true;
								}
								X_predchozi=X;//pokud toto zakomentuji prodlouží se CT resp. vykreslí se např. LAK o ten kus delší
						}

						////vykreslení procesu (jednoho obdelníčku "v plavecké dráze") včetně výpočtu koncové pozice a uložení dílčích hodnot
						X=proces(canv,++n,X_predchozi,X,Yloc,C,vozik);

						//posunutí na ose Y na další vozík
						Yloc+=KrokY;
					}

					///vypis mezivozíkového TAKTIMU (pokud se jedná vozíky od dané cesty (bacha, prochází se všechny) a zároveň pokud se jedná o poslední proces vozíku (např. svě) a nejedná o zcela první vozík
					if(vozik->zakazka->n==Z->n && C->dalsi==NULL && vozik->n!=1)
					{
						vypis_mezivozikovy_takt(canv,vozik,X,Yloc);//ten přímo za vozíky
						vypis_mezivozikovy_takt(canv,vozik,X,KrokY*2.5+oY+3,true);//ten na ose X nahoře +3 grafická korekce
					}
					///-

					vozik=vozik->dalsi;//posun na další prvek v seznamu vozíků
				}
				C=C->dalsi;//posun na další prvek v seznamu segmentů cesty, jde po cestě
			}
			if(Form1->STATUS==Form1->OVEROVANI)//(pokud je status nastaven na klienta, jinak jen první zakázka)
			Z=Z->dalsi;//posun na další prvek v seznamu ZAKÁZEK, jde po zakázakách
			else Z=NULL;

			Y=Yloc;
			delete C;
		}
		delete Z;

		////DALŠÍ VYPLÝVAJÍCÍ NASTAVENÍ
		//výpočet hodnot kapacit pro další využítí (pro grafy, ROMA atd.)
		if(!JIZPOCITANO)v.uloz_doporucene_kapacity_objetku();
		//hodnoty pro další grafické použití či nastavení
		WidthCanvasCasoveOsy=m.round(X);//uchová velikost nejdelší osy, pro použítí pro export canvasu do rastru
		HeightCanvasCasoveOsy=Y-KrokY/2;//uchová výšku grafu
		if(Form1->grid)vykresli_Xosy(canv);//vykreslí statické svislice na časové osy pokud je aktivovaná mřížka
		//povolení zobrazení grafu
		if(Form1->scGPGlyphButton_close_grafy->GlyphOptions->Kind==scgpbgkDownArrow)
		{
			Form1->scGPGlyphButton_close_grafy->Left=Form1->ClientWidth-Form1->scGPGlyphButton_close_grafy->Width;
			Form1->scGPGlyphButton_close_grafy->Top=Form1->Chart2->Top/*-Form1->GlyphButton_close_grafy->Height*/;
			Form1->g.ShowGrafy(true);
		}
		//nastavení a zobrazení zpráv
		if(JIZPOCITANO)Form1->g.zpravy();
		else Form1->g.nastav_zpravy();//nastavují jen v momentu nového výpočtu, tj. zadání nových parametrů nebo nového načtení
		//zobrazení legendy časových os
		vykresli_legendu_casovych_os(canv);
		//už se nebude ukladat proces znovu, protože byl vypočten a už není třeba zatěžovat znovu systémové prostředky (nehledě na to, že to bylo dost znát)
		JIZPOCITANO=true;
		Form1->Synteza->Enabled=true;//povolí mod technologické procesy
	}
	else
	{
		vykresli_vytizenost_objektu(canv);
		if(Form1->grid)vykresli_Xosy(canv);
	}
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//vypočítá konec procesu (odbdelníčku)
double Cvykresli::proces(TCanvas *canv, unsigned int n, double X_predchozi, double X, int Y, Cvektory::TCesta *C/*segment cesty*/,Cvektory::TVozik *vozik)
{
	 TColor barva=vozik->zakazka->barva;
	 if(vozik->typ)barva=clSilver;//pokud se jedná o servisní vozík, tak bude šedivý

	 //uložení hodnot pro zcela další použítí
	 Cvektory::TProces *P=new Cvektory::TProces;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->n_v_zakazce=n;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->segment_cesty=C;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->vozik=vozik;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 P->Tpoc=X_predchozi*60.0/PX2MIN;//uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 //standardní situace
	 X+=C->CT/60.0*PX2MIN;
	 vykresli_proces(canv,C->objekt->short_name,barva,0,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku na časové ose
	 P->Tkon=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 // nestandardní - nelogická situace, pokud bude čas procesu včetně času přejezdu vozíku kratší než u totožného přechozího objektu (vozíky např. v rámci CO2 se nemohou předbíhat), přičte se i tato vzdálenost (vykresleno šrafovaně)
	 double DcS=vozik->zakazka->jig.delka;//old: v.PP.delka_voziku;
	 if(P->segment_cesty->Rotace)DcS=vozik->zakazka->jig.sirka;
	 double Xt=C->objekt->obsazenost;
	 //pokud se nejedná o S&G objekt je třeba ještě zohledňovat nutnou dobu přejezdu/zpoždění vozíku za objektem, naopak u S&G se předpokládá, že může být v kabině jenom jeden vozík, takže netřeba řešit
	 if(P->segment_cesty->objekt->rezim!=0)Xt+=m.prejezd_voziku(DcS,C->RD)/60.0*PX2MIN;//musí být takto vyseparované, protože jinak v podmínce dávalo chybu, že např. 60<60 je true
	 if(X < Xt)//komentář o řádek výše, pokud není u S&G délka může vracet přejezd/zpoždění vozíku
	 {
			//dorovnání na čas předchozího vozíku, je-li to nutné
			X_predchozi=X;//uloží povodní X hodnotu
			X=C->objekt->obsazenost;
			vykresli_proces(canv,C->objekt->short_name,barva,1,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku na časové ose
			P->Tdor=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
			//cas_prekonani_zpozdeni_o_min_delku_jednoho_voziku
			X_predchozi=X;//uloží povodní X hodnotu
			X=Xt;//to samé jako X+=m.prejezd_voziku(DcS,C->RD)/60.0*PX2MIN;
			vykresli_proces(canv,C->objekt->short_name,barva,2,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku na časové ose
			P->Tpre=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 }
	 else//pokud situace NEnastane, tak ošetření proti tomu, aby se neukládaly náhodného hodnoty,ale hodnoty standardního konce procesu tzn. srovnatelné s CT
	 {
		 P->Tdor=X*60.0/PX2MIN;
		 P->Tpre=X*60.0/PX2MIN;
	 }
	 X_predchozi=X;

	 ////PALCE - posun o čekání na palce
	 P->Trand=0;
	 Cvektory::TObjekt *Objekt_dalsi=vozik->zakazka->cesta->dalsi->objekt;if(C->dalsi!=NULL)Objekt_dalsi=C->dalsi->objekt;//pokud neexistuje následující objekt v cestě, uvažuje se o po přechodu poslední/první objekt (tedy typicky svěšování/navěšování) a vezme se pohon prvního objektu, jinak se bere pohon v cestě následující
	 if(Form1->ComboBoxCekani->ItemIndex>0 && //pokud je požadováno v menu
			C->objekt->cekat_na_palce!=0 && //a zároveň nění uživatelsky zakázáno
			 (// a zároveň je splňuje následují:
					 C->objekt->rezim==0 ||//čekání je to po objektu v režimu S&G nebo
					(C->objekt->rezim==1 && Objekt_dalsi->rezim==1 && C->objekt->pohon!=Objekt_dalsi->pohon)||//je to mezi K a K režimem s přechodem na jiný dopravník nebo
					(C->objekt->rezim==1 && Objekt_dalsi->rezim==2 && C->objekt->pohon!=Objekt_dalsi->pohon)||//K->PP a jiný dopravník nebo
					(C->objekt->rezim==2 && Objekt_dalsi->rezim==1)||//PP->K nebo
					(C->objekt->rezim==2 && Objekt_dalsi->rezim==2 && C->objekt->pohon!=Objekt_dalsi->pohon)||//PP->PP a jiný dopravník nebo
					 C->objekt->stopka==1 ||//když je za objektem stopka (//0-ne,1-ano,2-automaticky) nebo
					 C->objekt->cekat_na_palce==1//automaticky-uživaztelsky požadovano zohledňování čekání na palce//0-ne,1-ano,2-automaticky
			 )
	 )
	 {            //případ první/poslední                            //ostatní připady
			double RD=vozik->zakazka->cesta->dalsi->RD;if(C->dalsi!=NULL)RD=C->dalsi->RD;//pokud neexistuje následující objekt v cestě, uvažuje se o po přechodu poslední/první objekt (tedy typicky svěšování/navěšování) a vezme se rychlost pohonu prvního objektu, jinak se bere RD objektu v cestě následujícího
			double R=0;if(Objekt_dalsi->pohon!=NULL)R=Objekt_dalsi->pohon->roztec;//přiřazení rozteče pokud existuje
			double Cekani=m.cekani_na_palec(X*60.0/PX2MIN+C->CT,R,RD,Form1->ComboBoxCekani->ItemIndex)/60.0*PX2MIN;
			if(Form1->ComboBoxCekani->ItemIndex==2)//pokud je vybraná položka o náhodná hodnota
			{
				if(RANDOM){P->Trand=Cekani;}//a má se převzít nová, tak se uloží pro další použítí nově vygenerovanou hodnotu
				else
				{
					Cekani=Pom_proces->Trand;//načte původní vygenerovanou hodnotu
					Pom_proces=Pom_proces->dalsi;//pomocný ukazatel na proces, využívá se v při načítání původně vytvořených náhodných hodnot čekání na palec
				}
			}
			X+=Cekani;
	 }
	 if(X_predchozi!=X)vykresli_proces(canv,C->objekt->short_name,barva,3,m.round(X_predchozi)-PosunT.x,m.round(X)-PosunT.x,Y-PosunT.y);//samotné vykreslení časového obdelníku čakání na palec na časové ose
	 P->Tcek=X*60.0/PX2MIN; //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 ////--

	 //uložení hodnot pro další použití (v dalších kolech)
	 C->objekt->obsazenost=X;//nahraje koncovou X hodnotu do obsaženosti objektu pro další využítí
	 vozik->pozice=X;//uložení pro další použítý vozík
	 if(!JIZPOCITANO)v.vloz_proces(P); //uložení hodnot pro zcela další použítí (pro zjišťování nutné kapacity, pro ROMA metoda, výpis procesu atp.),nejdříve ale smaže starý spoják
	 else {delete P; P=NULL;}

	 return X;
}
//---------------------------------------------------------------------------
//vykreslí jeden dílčí časový proces (obdelníček procesu objektu) pro jeden vozík, vytaženo pouze kvůli přehlednosti
void Cvykresli::vykresli_proces(TCanvas *canv, AnsiString shortname, TColor color, short typ, long X1, long X2, long Y,bool legenda)
{
	////osa
	//set_pen(canv,color,2);//nastavení pera barvy osy
	canv->Pen->Width=1;
	canv->Pen->Mode=pmCopy;
	canv->Pen->Style=psSolid;
	canv->Brush->Color=color;
	if(color==clSilver){if(!legenda_polozky[4])legenda_polozky[0]++;legenda_polozky[4]=true;}
	switch(typ)
	{
			case 0: canv->Brush->Style=bsSolid;canv->Pen->Color=clWhite;break;//pro typ: normální proces
			case 1: canv->Brush->Style=bsDiagCross;canv->Pen->Color=color;if(!legenda_polozky[5])legenda_polozky[0]++;legenda_polozky[5]=true;break;//pro typ: doplněný o konec na čekání na proces totožný předchozí
			case 2: canv->Brush->Style=bsCross;canv->Pen->Color=color;if(!legenda_polozky[6])legenda_polozky[0]++;legenda_polozky[6]=true;break;//pro typ: nutná doba přejezdu vozíku
			case 3: canv->Brush->Style=bsVertical;canv->Pen->Color=color;if(!legenda_polozky[7])legenda_polozky[0]++;legenda_polozky[7]=true;break;//pro typ: doba čekání na palec
			case 4: canv->Brush->Style=bsSolid;canv->Pen->Color=clWhite;canv->Pen->Mode=pmMask;if(!legenda_polozky[3])legenda_polozky[0]++;legenda_polozky[3]=true;//pmNotXor;/*zajistí vykreslení procesu transparentně*/break;//pro typ: obsazenost procesu či buffer
			case 5: canv->Brush->Style=bsSolid;canv->Pen->Color=clWhite;canv->Pen->Mode=pmMask;//výměna barev či čištění pistole
	}
	//samotný obdelníček
	short o=0;if(legenda)o=1;//v případe zobrazeného orámování zmenší o jeden pixel u legendy
	short o2=0;if(typ==1 || typ==2 || typ==3)o2=1;//v případe daného typu zmenší o jeden pixel
	canv->Rectangle(X1+o,Y-KrokY/2+o2,X2+1-o,Y+KrokY/2-o2);//X2+1 pouze grafická záležitost - zmenšení mezery
	//následující musí být mimo switch kvůli pořadí vykreslování po rectanglu
	if(typ==4 && !legenda)//v případě bufferu vykreslení svislice přemaskující bílý spoj, tím se buffer napojí na předchozí objekt
	{
		canv->Pen->Color=color;
		canv->MoveTo(X1,Y-KrokY/2+1);canv->LineTo(X1,Y+KrokY/2-1);//+-1 grafická vyfikundace
	}

	////popisek
	//normal 0    buffer který má popisek menší než délku obdelničku
	if(typ==0 /*|| (typ==4 && canv->TextWidth(shortname)<X2-X1)*/)
	{
			SetBkMode(canv->Handle,OPAQUE);//nastvení netransparentního pozadí
			//if(color!=clBlack)canv->Font->Color=clBlack;else canv->Font->Color=clWhite;//pokud je výplň obdelníčku černě, tak popisek bude bíle
			canv->Font->Color=clWhite;
			canv->Font->Size=9;
			canv->Font->Name="Arial";
			canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
			canv->TextOutW(((X2+X1)/2)-canv->TextWidth(shortname)/2,Y-canv->TextHeight(shortname)/2,shortname);//vypíše vycentrovaný (polovina nových a starých souřadnic a posun referenčního písma o horizontálně=TextWidth/2 a verticálně=TextHeight/2) popisek shorname t-objektu
	}
}
//---------------------------------------------------------------------------
//vykreslí legendu pro jednotlivé procesy na časových osách
void Cvykresli::vykresli_legendu_casovych_os(TCanvas *canv)
{
	if(Form1->scGPGlyphButton_close_legenda_casove_osy->GlyphOptions->Kind==scgpbgkDownArrow)//pokud je legenda zobrazena + napozicování dle toho, zda jsou zobrazeny či skryty grafy
	{
		//pozice
		short P=legenda_polozky[0]+2;//počet obdelníků,kterých se bude vykreslovat
		short L=20;
		int T=Form1->ClientHeight-Form1->scGPPanel_statusbar->Height-P*KrokY-KrokY/2-9;//levé a top usazení legendy
		if(Form1->Chart2->Visible)T=Form1->Chart2->Top-P*KrokY-KrokY/2-9;//pokud jsou zobrazeny grafy, bude TOP pozice dle grafů
		int R=L+KrokY+canv->TextWidth("čištění pistole a výměna barev")+KrokY/2;//pravý okraj
		AnsiString V="";//výpis

		//top pozice
		Form1->scGPGlyphButton_close_legenda_casove_osy->Top=T+KrokY+KrokY/2+Form1->scGPGlyphButton_close_legenda_casove_osy->Options->FrameWidth;//top je nehledě na to, zda je či není zobrazeno
		//levá pozice tlačítka, pokud je legenda zobrazena
		Form1->scGPGlyphButton_close_legenda_casove_osy->Left=R-Form1->scGPGlyphButton_close_legenda_casove_osy->Width-1;

		//vykreslení obdelníku bíléhopozadí
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=clWhite;
		canv->Pen->Style=psSolid;
		canv->Pen->Color=clMedGray;
		canv->Pen->Width=1;
		canv->Pen->Mode=pmCopy;
		canv->Rectangle(L-2,T+KrokY/2-2,R,T+P*KrokY-KrokY/2+2);

		//zjistí barvu (raději) druhého zobrazeného voziků, podle této barvy se vykreslí legenda
		unsigned int Voz=ceil((Form1->scLabel_titulek->Height+PosunT.y-KrokY/2-Form1->scGPPanel_mainmenu->Height)/(KrokY*1.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
		TColor C=v.vrat_vozik(Voz+1)->zakazka->barva;

		//vykreslování samotné legendy, podle položek, které byly použity
		vykresli_proces(canv, "",C,0,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;V="technologický proces";canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,V);
		if(legenda_polozky[1]){vykresli_proces(canv, "",m.clIntensive(C,-20),5,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"čištění pistole");}
		if(legenda_polozky[2]){vykresli_proces(canv, "",m.clIntensive(C,-40),5,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"čištění pistole a výměna barev");}
		if(legenda_polozky[3]){vykresli_proces(canv, "",m.clIntensive(C,80),4,L-1,L+KrokY+1,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"buffer či čekání");}
		if(legenda_polozky[4]){vykresli_proces(canv, "",clSilver,0,L,L+KrokY,T+=KrokY);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+1,T-canv->TextHeight(V)/2,"vozíky čištění a výměny");}
		if(legenda_polozky[5]){vykresli_proces(canv, "",C,1,L,L+KrokY,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+2,T-canv->TextHeight(V)/2,"čekání na předchozí proces");}
		if(legenda_polozky[6]){vykresli_proces(canv, "",C,2,L,L+KrokY,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+2,T-canv->TextHeight(V)/2,"nutná doba přejezdu vozíku");}
		if(legenda_polozky[7]){vykresli_proces(canv, "",C,3,L,L+KrokY,T+=KrokY,true);canv->Brush->Style=bsSolid;canv->TextOutW(L+KrokY+2,T-canv->TextHeight(V)/2,"doba čekání na palec");}
	}
	else//pokud je legenda skryta, tak pouze napozicování dle toho, zda jsou zobrazeny či skryty grafy
	{
		Form1->scGPGlyphButton_close_legenda_casove_osy->Left=0;
		if(Form1->Chart2->Visible)Form1->scGPGlyphButton_close_legenda_casove_osy->Top=Form1->Chart2->Top;
		else Form1->scGPGlyphButton_close_legenda_casove_osy->Top=Form1->scGPPanel_statusbar->Top-Form1->scGPGlyphButton_close_grafy->Height;
	}
}
//---------------------------------------------------------------------------
//textový výpis a kóta mezivozíkového taktu, pouze pro zpřehlednění zapisu samostatně
void Cvykresli::vypis_mezivozikovy_takt(TCanvas *canv,Cvektory::TVozik *vozik,double X,long Y,bool index)
{
		long X1=Form1->m.round(vozik->predchozi->pozice-PosunT.x);
		long X2=Form1->m.round(X-PosunT.x);
		short S=2;//size
		//short O=0;//offset
		double Y0=Y-KrokY*2-PosunT.y; //-3 pouze grafické korekce

		////vykreslení kóty
		//linie
		canv->Pen->Width=1;
		canv->Pen->Color=vozik->zakazka->barva;
		canv->MoveTo(X1,Y0);
		canv->LineTo(X2,Y0);
		//šipky
		POINT bodyL[3]={{X1+S,Y0-S},{X1,Y0},{X1+S,Y0+S}};
		POINT bodyP[3]={{X2-S,Y0-S},{X2,Y0},{X2-S,Y0+S}};
		canv->Polygon((TPoint*)bodyL,2);
		canv->Polygon((TPoint*)bodyP,2);
		//spojovací linie k následujícímu vozíku
		if(!index)
		{
			canv->MoveTo(X2,Y0);
			canv->LineTo(X2,Y0+KrokY/2);
		}
		////text
		SetBkMode(canv->Handle,TRANSPARENT);
		canv->Font->Color=vozik->zakazka->barva;
		canv->Font->Size=9;
		canv->Font->Name="Arial";
		canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		AnsiString T="";
		//pro nezokrouhlený výpis čísla: if(!index)T="TT: "+AnsiString(floor(v.vrat_TT_voziku(vozik)*1000000.0)/1000000.0)+" s";
		//pro nezokrouhlený výpis čísla: else T=AnsiString(floor(v.vrat_TT_voziku(vozik)/60.0*1000000.0)/1000000.0);
		if(!index)T="TT: "+AnsiString(m.round2double(v.vrat_TT_voziku(vozik),precision))+" s";
		else T=AnsiString(m.round2double(v.vrat_TT_voziku(vozik)/60.0,precision));
		canv->TextOut((X1+X2)/2-canv->TextWidth(T)/2,Y0-canv->TextHeight(T),T);
}
//---------------------------------------------------------------------------
//vykreslí pohyblivou svislici na časové osy dle umístění kurzoru myši
void Cvykresli::vykresli_svislici_na_casove_osy(TCanvas *canv,int X,int Y)
{
	if(X!=-200)//pokud je mimo obraz -200 jen nahodilá hodnota pro zneplatenění čí výchozí obraz
	{
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Width=1;
		canv->Pen->Style=psDashDot;//nastevení čarkované čáry
		canv->Pen->Color=clGray;
		canv->Brush->Style=bsClear;
		//svislice
		canv->MoveTo(X,Form1->scGPPanel_mainmenu->Height);
		canv->LineTo(X,Form1->ClientHeight);
		//vodorovna
		if(!mod_vytizenost_objektu)//při modu vytížení objektů se nezobrazí
		{
			canv->MoveTo(0,Y);
			canv->LineTo(Form1->ClientWidth,Y);
			canv->Brush->Style=bsSolid;//vracím raději do původního stavu
			unsigned int V=ceil((Y+PosunT.y-KrokY/2-Form1->scGPPanel_mainmenu->Height)/(KrokY*1.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
			if(V<=v.VOZIKY->predchozi->n)Form1->SB("Vozík: "+AnsiString(V));
			else Form1->SB("");//pokud je už mimo oblast
		}
	}
}
//---------------------------------------------------------------------------
//vypíše labal zaměřovač na pozici kurzoru myši
void Cvykresli::zobrazit_label_zamerovac(int X,int Y)
{
	unsigned int V=ceil((Y+PosunT.y-KrokY/2-Form1->scGPPanel_mainmenu->Height)/(KrokY*1.0));//pozn. KrokY/2 kvůli tomu, že střed osy je ve horozintální ose obdelníku
	if(!mod_vytizenost_objektu && 0<V && V<=v.VOZIKY->predchozi->n) //pokud se nejedná o řežim vytíženost objektu a zároveň se jedná o číslo vozík od min do max vozíků
	{
				vykresli_svislici_na_casove_osy(Form1->Canvas,X,Y);//nový přístup v zobrazování svislic, jen v momentu zobrazování labalu_zamerovac
				Form1->Label_zamerovac->Transparent=false;
				Form1->Label_zamerovac->Color=clWhite;
				Form1->Label_zamerovac->Font->Color=(TColor) RGB(100,100,100);
				Form1->Label_zamerovac->Left=X+5; Form1->Label_zamerovac->Top=Y+20; //+ odsazení
				Form1->Label_zamerovac->Caption=" vozík: "+AnsiString(V)+" \n min: "+AnsiString((X+PosunT.x)/PX2MIN)+" ";
				Form1->Label_zamerovac->Visible=true;
				Form1->RM();//korekce chyby oskakování pravého menu
	}
	else Form1->Label_zamerovac->Visible=false;
}
//---------------------------------------------------------------------------
//vykreslí statické svislice na časové osy
void Cvykresli::vykresli_Xosy(TCanvas *canv)
{
	canv->Pen->Mode=pmNotXor;
	canv->Pen->Width=1;    //nastavení šířky pera
	canv->Pen->Style=psDot;
	canv->Pen->Color=TColor RGB(200,200,200);   //míchání světlě šedé
	canv->Brush->Style=bsClear;
	canv->Font->Color=clGray;
	canv->Font->Size=9;
	canv->Font->Name="Arial";
	canv->Font->Style = TFontStyles();
	canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
	canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;
	short o=1;
	//už používám globálně short oY=5;//ofset na ose Y, 5 pouze grafická korekce
	if(PosunT.x>10)o=-30;
	if(!mod_vytizenost_objektu)canv->TextOutW(o-PosunT.x,0,"voz|min"); //popisek osy x
	else canv->TextOutW(o-PosunT.x,0,"obj|min"); //popisek osy x

	//svislice po dvou minutách
	int start=PX2MIN*2;if(PosunT.x>0)start=0;
	for(int i=start;i<=WidthCanvasCasoveOsy;i+=PX2MIN*2)//po dvou minutách
	{
		canv->MoveTo(i-PosunT.x,oY);
		if(mod_vytizenost_objektu && v.OBJEKTY->predchozi!=NULL)canv->LineTo(i-PosunT.x,v.OBJEKTY->predchozi->n*KrokY+KrokY+1);//+1 pouze optická korekce
		else canv->LineTo(i-PosunT.x,HeightCanvasCasoveOsy+oY-1);//-1 pouze optická korekce
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=clWhite;
		canv->TextOutW(i-canv->TextWidth(i/PX2MIN)/2-PosunT.x,0,i/PX2MIN);
	}

	if(!mod_vytizenost_objektu)
	{
			//svislé číslování vozíků (to barevné úplně nalevo)
			canv->Brush->Style=bsSolid;
			canv->Font->Style=TFontStyles()<< fsBold;
			canv->Font->Color=clWhite;
			Cvektory::TVozik *voz=v.VOZIKY->dalsi;
			while(voz!=NULL)
			{
				canv->Brush->Color=voz->zakazka->barva;
				canv->TextOutW(0,voz->n*KrokY+1-PosunT.y,voz->n); //+1 pouze grafická korekce
				voz=voz->dalsi;
			}

			//začátky a konce zakázek
			Cvektory::TZakazka *ukaz=v.ZAKAZKY->dalsi;
			int konec=KrokY;
			while (ukaz!=NULL)//projede všechny zakázky, cesty
			{                                                                  //ze sekund na min
				TPointD RET=v.vrat_zacatek_a_konec_zakazky(ukaz);RET.x=RET.x/60.0;RET.y=RET.y/60.0;
				konec+=v.vrat_pocet_voziku_zakazky(ukaz)*KrokY;
				canv->Pen->Color=ukaz->barva;
				canv->Pen->Style=psSolid;
				canv->Pen->Width=2;
				canv->Brush->Style=bsSolid;
				canv->Brush->Color=clWhite;
				//x - plete, jedná se jen o začátek zakázky
				if(RET.x>0)
				{
					canv->MoveTo(RET.x*PX2MIN-PosunT.x,0);
					canv->LineTo(RET.x*PX2MIN-PosunT.x,konec-Form1->scGPPanel_mainmenu->Height+KrokY/2-3-PosunT.y/*HeightCanvasCasoveOsy*/);
				}
				//náběh linky
				unsigned int WIP=v.WIP(1);canv->Pen->Style=psDash;canv->Pen->Width=1;
				if(ukaz->n==1)//pro první zakázku               //y -jen konec
				{
					canv->MoveTo(v.VOZIKY->dalsi->pozice-PosunT.x,0);
					canv->LineTo(v.VOZIKY->dalsi->pozice-PosunT.x,WIP*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
					canv->LineTo(0,WIP*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
				}
				//doběh linky
				if(ukaz->predchozi==v.ZAKAZKY->predchozi && v.VOZIKY->predchozi->n-WIP>WIP)//pokud se jedná o poslední zakázku a doběh nastane později než náběh
				{
					canv->MoveTo(v.vrat_start_a_pozici_vozikuPX(v.VOZIKY->predchozi->n-WIP).y-PosunT.x,0);
					canv->LineTo(v.vrat_start_a_pozici_vozikuPX(v.VOZIKY->predchozi->n-WIP).y-PosunT.x,(v.VOZIKY->predchozi->n-WIP)*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
					canv->LineTo(0,(v.VOZIKY->predchozi->n-WIP)*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y);
				}
				//y - plete, jedná se jen o konec zakázky
				canv->Pen->Width=2;	canv->Pen->Style=psSolid;
				if(RET.y>0)
				{
					canv->MoveTo(RET.y*PX2MIN-PosunT.x,0);
					canv->LineTo(RET.y*PX2MIN-PosunT.x,konec-Form1->scGPPanel_mainmenu->Height+KrokY-3-PosunT.y/*HeightCanvasCasoveOsy*/);
				}
				canv->Brush->Style=bsSolid;
				canv->Brush->Color=ukaz->barva;
				canv->Font->Style=TFontStyles()<< fsBold;
				canv->Font->Color=clWhite;
				//výpis začátku zakázky
				if(RET.x>0)canv->TextOutW(RET.x*PX2MIN-canv->TextWidth(RET.x)/2-PosunT.x,0,AnsiString(m.round2double(RET.x,precision))+"<");//zobrazuje pouze větší než začátek obrazovky
				//výpis náběhu linky, pro první zakázku
				if(ukaz->n==1)canv->TextOutW(0,WIP*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-canv->TextHeight("N")-PosunT.y,"NÁBĚH LINKY");
				//výpis doběh linky,pokud se jedná o poslední zakázku a doběh nastane později než náběh
				if(ukaz->predchozi==v.ZAKAZKY->predchozi && v.VOZIKY->predchozi->n-WIP>WIP)canv->TextOutW(0,(v.VOZIKY->predchozi->n-WIP)*KrokY+KrokY+KrokY/2-Form1->scGPPanel_mainmenu->Height+oY+oY+2-PosunT.y,"DOBĚH LINKY");
				//výpis konce zakázky + LT
				AnsiString LT=AnsiString(m.round2double(RET.y,precision));
				if(RET.y>0)canv->TextOutW(RET.y*PX2MIN-canv->TextWidth(LT)/2-PosunT.x,0,"<"+AnsiString(LT));//zobrazuje pouze větší než začátek obrazovky
				ukaz=ukaz->dalsi;
			}
	}
}
//---------------------------------------------------------------------------
//vykreslí vytíženost od daného objektu
void Cvykresli::vykresli_vytizenost_objektu(TCanvas *canv)
{
	int Y=KrokY+oY;
	Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{                      //záměrné nadhodnocení kvůli hledání minima
		int Pocatek=v.PROCESY->predchozi->Tcek/60.0*PX2MIN-PosunT.x;int Konec=0-PosunT.x;//pro další využítí, zatím pouze pro vypis názvu objektu, hlednání min a maxima
		Cvektory::TProces *P=v.PROCESY->dalsi;
		double X=0;
		while (P!=NULL)
		{
			if(ukaz==P->segment_cesty->objekt)
			{
				if(P->segment_cesty->objekt->kapacita_dop==1 || NOLIEX==0)vykresli_proces(canv,"",P->vozik->zakazka->barva,4,P->Tpoc/60.0*PX2MIN-PosunT.x,P->Tcek/60.0*PX2MIN-PosunT.x,Y+oY-PosunT.y);//pro jednokapacitní vytíženost vykreslení přímo maximální, popř. pokud není požadováno vykreslení intenzity
				else vytizenost_procesu(canv,P,X,Y+oY); //pro vícekapacitně včetně škály vytíženosti, pokud je požadována
				if(Pocatek>P->Tpoc/60.0*PX2MIN-PosunT.x)Pocatek=P->Tpoc/60.0*PX2MIN-PosunT.x;//pro další využítí, zatím pouze pro vypis názvu objektu, hlednání min a maxima
				if(Konec<P->Tcek/60.0*PX2MIN-PosunT.x)Konec=P->Tcek/60.0*PX2MIN-PosunT.x;//pro další využítí, zatím pouze pro vypis názvu objektu, hlednání min a maxima
			}
			P=P->dalsi;
		};

		//vycentrovaný popisek v rámci objektu
		SetBkMode(canv->Handle,TRANSPARENT/*OPAQUE*/);//nastvení transparentního pozadí
		canv->Font->Color=clWhite;
		canv->Font->Size=10;
		canv->Font->Name="Arial";
		canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
		canv->TextOutW(((Konec+Pocatek)/2)-canv->TextWidth(ukaz->name.UpperCase())/2,Y+oY-PosunT.y-canv->TextHeight(ukaz->name.UpperCase())/2,ukaz->name.UpperCase());//vypíše vycentrovaný (polovina nových a starých souřadnic a posun referenčního písma o horizontálně=TextWidth/2 a verticálně=TextHeight/2) popisek shorname t-objektu

		//svislé popisování objektů
		canv->Brush->Style=bsSolid;
		canv->Brush->Color=clGray;
		canv->TextOutW(0,Y+oY-canv->TextHeight(ukaz->short_name)/2/*-Form1->scGPPanel_mainmenu->Height+oY*/-PosunT.y,ukaz->short_name);

		ukaz=ukaz->dalsi;
		Y+=KrokY+2;//+2 pouze grafické odsazení pro všechny objekty mimo prvního
	}
}
////---------------------------------------------------------------------------
void Cvykresli::vytizenost_procesu(TCanvas *canv, Cvektory::TProces *P,double X,int Y)
{
	 if(X==0)X=P->Tpoc;
	 Cvektory::TProces *P_dal=P;
	 //exponenciální
	 short I=170;//aktuální intenzita světlá nejvíc , 120 pro linearní 170 pro epxponenicální
	 double IK=sqrt(I*1.0)/(P->segment_cesty->objekt->kapacita/*kapacita_dop*/);//intenzita krok  pro exponencialni
	 double A=0;short B=I;//pomocné pro exponeciální snižování intenzity
	 //linearní
	 if(NOLIEX==1)  //u lineárního je chyba u nejvyšší intenzity
	 {
			I=120;
			IK=I/(P->segment_cesty->objekt->kapacita_dop-1);//intenzita krok pro linearni
	 }

	 do
	 {
		 if(NOLIEX==2)//exponenciální
		 {
				vykresli_proces(canv,"",m.clIntensive(P->vozik->zakazka->barva,B),4,X/60.0*PX2MIN-PosunT.x,P->Tcek/60.0*PX2MIN-PosunT.x,Y-PosunT.y);
				B=m.round(I-pow(A+=IK,2));//exponenciální snižování světlosti
		 }
		 if(NOLIEX==1)//lineární
		 {
				vykresli_proces(canv,"",m.clIntensive(P->vozik->zakazka->barva,I),4,X/60.0*PX2MIN-PosunT.x,P->Tcek/60.0*PX2MIN-PosunT.x,Y-PosunT.y);
				I-=IK;//lineární snižování světlosti
		 }
		 P_dal=v.vrat_nasledujici_proces_objektu(P_dal);
		 if(P_dal!=NULL && X<P_dal->Tpoc)
		 X=P_dal->Tpoc;
		 else break;
	 }
	 while(X<P->Tcek);
	 X=P->Tcek;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
//////ROMA metoda, vykreslí graf technologických procesů vůči jednotlivým t-objektům v čase
void Cvykresli::vykresli_technologicke_procesy(TCanvas *canv)
{
	////////VÝCHOZÍ NASTAVENÍ
	//--nastavení proměnných k účelu filtrace, nastavováno z unit1 či volání animace, pro přehlednost a lenost necháno zde předáním na lokální proměnné
	double K=TP.K;//Krok po kolika minutach se bude zobrazovat
	double OD=TP.OD;//od které min se proces začne vypisovat
	double DO=TP.DO;//konec zakazky v min
	unsigned int Nod=TP.Nod;//rozmezí Jaký se vypíše vozik,
	unsigned int Ndo=TP.Ndo;//rozmezí Jaký se vypíše vozik, pokud bude 0 vypisují se všechny
	bool A=TP.A;//jednořádková animace
	//--
	//výchozí proměnné
	int PXM=50;//měřítko pixelů na metr v tomto modu, zároveň však používám jako krok posunu na ose Y (přetížení proměnné)
	int D=Form1->m.round(v.PP.delka_jig*PXM);//vozik délka
	int S=Form1->m.round(v.PP.sirka_jig*PXM);//vozik šířka
	unsigned int X=0;//posun po X-ové ose
	short Yofset=D;if(S>D)Yofset=S;//výška řádku - daného časového úseku, podle šířky vozíku či největší hodnoty šířka/délka
	unsigned int Y=Yofset;//Posun po Y-oso včetně výchozí pozice
	canv->Font->Size=10;//nutno tady kvůli správné velikosti, pokud dojde ke změně je nutné párově změnit
	Xofset=4+canv->TextWidth(TP.KZ+K)+4;//zajistí správný počátek prvního objektu dle šířky nejdelší vypisované minuty

	////////VÝPOČET A ULOŽENÍ POZICE OBJEKTŮ NA X OSE
	Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		if(ukaz->delka_dopravniku<=0 /*doplnit ještě pokud je požadováno uživatelsky dle kapacity*/)//pokud by byla zadaná neplatná hodnota, tak se vezme dle kapacity zadané
		{
			if(Form1->CheckBox_pouzit_zadane_kapacity->Checked)
			{
				if(ukaz->rotace)X+=ukaz->kapacita*S;//pokud se mají použít zadané kapacity
				else X+=ukaz->kapacita*D;//pokud se mají použít zadané kapacity
			}
			else
			{
				if(ukaz->rotace)X+=ukaz->kapacita_dop*S;//pokud se mají použít zadané kapacity
				else X+=ukaz->kapacita_dop*D;//pokud se mají použít zadané kapacity
			}
		}
		else
		{
			X+=ukaz->delka_dopravniku*PXM;
		}
		ukaz->obsazenost=X;//zneužití proměné obsazenost,//přes ->předchozí asi by nešlo načítat, protože pořadí objektu na cestě a ve spojaku OBJEKTY nemusí být totožné
		ukaz=ukaz->dalsi;
	}

	////////VOZÍKY
	//nastavení popisku
	SetBkMode(canv->Handle,/*TRANSPARENT*/OPAQUE);//nastvení netransparentního pozadí
	canv->Font->Style = TFontStyles()<< fsBold;//normání font (vypnutí tučné, kurzívy, podtrženo atp.)
	canv->Font->Size=12;
	canv->Font->Name="Arial";
	canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
	canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;
	//nastavení pera pro vykreslení vozíčku
	canv->Pen->Mode=pmCopy;
	canv->Pen->Width=1;
	canv->Pen->Style=psSolid;
	canv->Pen->Color=clWhite;
	canv->Brush->Style=bsSolid;
	canv->Font->Color=clWhite;
	//samotné vykreslení, vypíše vždy všechny procesy v dané minutě
	Cvektory::TProces *P=v.PROCESY->dalsi;
	while (P!=NULL)
	{
		for(double MIN=OD;MIN<=DO;MIN+=K)
		{   //filtr na rozsah vozíků, nebo pokud Ndo==0, tak se vypíší všechny             //nemůže být "="    //pro poslední vozík
				if(((Nod<=P->vozik->n && P->vozik->n<=Ndo) || Ndo==0) && P->Tpoc/60.0<=MIN && (MIN<P->Tcek/60.0 || DO==P->Tcek/60.0))//filtr
				{
					int Rx=D;int Ry=S;//rozměr
					if(P->segment_cesty->objekt->rotace)//pokud je požadovaná rotace jigu v objektu
					{
						Rx=S;Ry=D;
						//v případě, že se vozík blíží ke konci objektu orotuje jig zase zpět, Xp - X predikce následného výpočtu
						int Xp=P->segment_cesty->objekt->predchozi->obsazenost+((P->segment_cesty->objekt->obsazenost-P->segment_cesty->objekt->predchozi->obsazenost)*(MIN-P->Tpoc/60.0)/(P->Tcek/60.0-P->Tpoc/60.0))+Xofset+Rx/2;
						//lze odbrat /2 za Xofset či >= předělat na =, ale stále se nejedná o opticky dokonalý jev, rotace totiž "probíhá" v začátku vozíku nikoliv jeho středu, je dobré, si pro otestování vypsat ve filtru krok po 0,1
						if(Xp-Rx/2+Xofset/2>=P->segment_cesty->objekt->obsazenost){Rx=D;Ry=S;}
					}
					//výpočet umístění na ose X už jen v rámci objektu, tzn. aby byl znatelný posun (po částech i v rámci objektu)
					//pro jednokapacitní resp. S&G neanimuje, pokud není nastaveno Checkboxem jina
					if(P->segment_cesty->objekt->kapacita==1 && !Form1->CheckBoxAnimovatSG->Checked)
					{
						X=P->segment_cesty->objekt->obsazenost;//pokud se do objektu vejde pouze jenom jeden objekt
						X+=Xofset-Rx/2;//ještě grafické odsazení o odsazení výchozí osy a o šířku jednoho vozíku
					}
					else//animace i v rámci objektu
					{
						X=P->segment_cesty->objekt->predchozi->obsazenost+
						(
							(P->segment_cesty->objekt->obsazenost-P->segment_cesty->objekt->predchozi->obsazenost)*
							(MIN-P->Tpoc/60.0)/(P->Tcek/60.0-P->Tpoc/60.0)
						);
						X+=Xofset+Rx/2;//ještě grafické odsazení o odsazení výchozí osy a o šířku jednoho vozíku
					}
					//vykreslení samotného vozíku (obdélníčku)
					canv->Brush->Color=P->vozik->zakazka->barva;
					AnsiString T=P->vozik->n;
					//if(P->segment_cesty->objekt->rotace)if(P->segment_cesty->objekt->obsazenost+Xofset<X){Rx=D;Ry=S;}
					if(!A)//pokud se nejedná o animaci, aby bylo možné posouvat obraz na ose Y a při animaci naopak nebylo možné
					{
						Y=Yofset*(MIN-OD)/K+Yofset;//výpočet umístění na ose Y (jedná se pouze o umístění na řádku správné minuty)
						canv->Rectangle(X-Rx/2-PosunT.x,Y-Ry/2-PosunT.y,X+Rx/2+1-PosunT.x,Y+Ry/2-PosunT.y);  //+1 pouze grafická vyfikundace
						canv->TextOutW(X-canv->TextWidth(T)/2-PosunT.x,Y-canv->TextHeight(T)/2-PosunT.y,T);
					}
					else//jedná se o animaci
					{
						canv->Rectangle(X-Rx/2-PosunT.x,Y-Ry/2,X+Rx/2+1-PosunT.x,Y+Ry/2);  //+1 pouze grafická vyfikundace
						canv->TextOutW(X-canv->TextWidth(T)/2-PosunT.x,Y-canv->TextHeight(T)/2,T);
					}
				}
		}
		P=P->dalsi;
	}

	////////POPISKY a svislice
	//nastavení
	canv->Pen->Width=2;
	canv->Pen->Mode=pmCopy;
	canv->Pen->Style=psSolid;
	canv->Pen->Color=clGray;
	canv->Brush->Color=clWhite;
	canv->Font->Color=clGray;
	canv->Font->Size=10;
	Y=4;
	X=Xofset;unsigned int Xpuv=X;
	////vodorovný popisek ale svislé čáry - OBJEKTY
	canv->MoveTo(X-PosunT.x,Y);if(!A)canv->LineTo(X-PosunT.x,Yofset+PXM*DO/K-PosunT.y);else canv->LineTo(X-PosunT.x,Yofset+PXM);//nakreslení první svislice (začátek pravděpodobně navěšování)
	ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
	while (ukaz!=NULL)
	{
		//ověřit proč není použito int Rx=D;
		//int Ry=S;if(ukaz->rotace==90){Rx=S;Ry=D;}//rozměr
		//prověřit§§§§
		int Ry=m.round(m.UDV(S,D,ukaz->rotace));//hodnoty záměrně obráceně
		X=ukaz->obsazenost+Xofset;
		canv->MoveTo(X-PosunT.x,Y);
		if(!A)canv->LineTo(X-PosunT.x,Yofset+Ry*DO/K+Yofset/2-PosunT.y);//pokud se nejedná o animaci, pozn. osa Y si stejně vypisuje nějak divně
		else canv->LineTo(X-PosunT.x,Yofset+Ry);//pokud se jedná o animaci
		AnsiString T=ukaz->short_name;//výpis popisku objektu
		canv->TextOutW((Xpuv+X)/2-canv->TextWidth(T)/2-PosunT.x,Y+1,T);//+1 pouze grafická korekce
		if(ukaz->delka_dopravniku>0 && ukaz->kapacita>1)//v případě vícekapacitního objektu s uvedeného délkou, výpiše i jeho délku a případně rychlost dopravníku
		{
			AnsiString T1="";//další řádek
			T1+=AnsiString(ukaz->delka_dopravniku)+"[m]";
			if(ukaz->RD!=0)T1+=", "+AnsiString(Form1->m.round2double(ukaz->RD,4))+"[m/s]";//pozor zaokrouhleno na 4desetinná
			canv->TextOutW((Xpuv+X)/2-canv->TextWidth(T1)/2-PosunT.x,Y+canv->TextHeight(T1)+2,T1);//+2 pouze grafická korekce
		}
		//posun na další
		Xpuv=X;
		ukaz=ukaz->dalsi;
	}
	////svislý popisek - MINUTY  (ale vodorovné čáry)
	Y=Yofset;
	X=4;//2 - grafické odsazení
	canv->Pen->Width=1;
	canv->Pen->Mode=pmNotXor;
	canv->Pen->Style=psDot;
	canv->Pen->Color=TColor RGB(200,200,200);   //míchání světlě šedé
	canv->Brush->Color=clWhite;
	canv->Font->Color=clGray;
	canv->Font->Size=10;//musí být stejný jako u objektů
	for(double MIN=OD;MIN<=DO;MIN+=K)//po půlminutách či nastaveném kroku
	{
		if(!A)//pokud se nejedná o animaci
		{
			if(Form1->grid)//pokud je požadován grid
			{
				canv->MoveTo(X,Y-PosunT.y-Yofset/2);//-PXM/2 aby linie byly nád a pod vozíkem
				canv->LineTo(Form1->ClientWidth,Y-PosunT.y-Yofset/2);
			}
			canv->TextOutW(X,Y-canv->TextHeight(MIN)/2-PosunT.y,MIN);
			Y+=Yofset;
		}
		else//pokud se jedná o animaci
		{
			canv->TextOutW(X,Y-canv->TextHeight(MIN)/2,MIN);
		}
	}
	//popisek pouze [min]
	canv->TextOutW(1,oY-1,"[min]");//-1 pouze grafická korekce
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
void Cvykresli::rotace_textu(TCanvas *canv, long rotace)//úhel rotace je desetinách stupně
{
		LOGFONT LogRec;
		GetObject(canv->Font->Handle,sizeof(LogRec),&LogRec);
		LogRec.lfEscapement=rotace;
		canv->Font->Handle=CreateFontIndirect(&LogRec);
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////nastaví pero                                                             //http://www.zive.cz/clanky/geometricka-pera/sc-3-a-103079
void Cvykresli::set_pen(TCanvas *canv, TColor color, int width, int style)//PS_ENDCAP_FLAT PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE viz Matoušek III str. 179
{
		DeleteObject(canv->Pen->Handle);//zruší původní pero
		DWORD pStyle = PS_GEOMETRIC | PS_SOLID | style /*| PS_JOIN_BEVEL*/ | PS_INSIDEFRAME;
		DWORD pWidth = width;

		LOGBRUSH lBrush;
		lBrush.lbStyle = BS_SOLID;
		lBrush.lbColor = color;
		lBrush.lbHatch = 0;

		canv->Pen->Handle = ExtCreatePen(pStyle, pWidth, &lBrush, NULL, NULL);
}
////---------------------------------------------------------------------------
TColor Cvykresli::set_color(TCanvas *canv, Cvektory::TObjekt *O)
{
		unsigned short i=0;
		Cvektory::TObjekt *D=O->dalsi;
		if(O!=NULL && D==NULL)D=v.OBJEKTY->dalsi;//jedná se o poslední objekt a další bude první
		if(O->pohon!=NULL && D->pohon!=NULL)//pro situaci kdy je pohon přiřazen
		{
			if(D->pohon->roztec==0)
			{
				i=10;//pokud je u dalšího pohon přiřazen, ale není zadaná rozteč, není kompletní a kompetentní info
				F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - pohon <b>"+D->pohon->name+"</b> přiřazen objektu <b>"+D->name+"</b> nemá přiřazenou rozteč!<br><b>řešení</b> - nastavte pohonu patřičnou rozteč.<br><br>",true);
			}
			else//jsou k dispozici data
			{                         //zvažit zda RD či aRD (kvůli PP)
				if(O->mezera>=m.minM(O->pohon->aRD,D->pohon->aRD,D->pohon->roztec))
				{
					i=0;//je v pořádku
					//nelze přepíše F->Z("<b>Linka v pořádku.</b>",false);
				}
				else
				{
					i=1;//není v pořádku nestíhá se čekání
					F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - nedostatečná doba čekání na palec, resp. rozestup!<br><b>řešení</b> - upravte rychlost pohonu nebo pohonu objektu následujícího či nastavte větší mezeru resp. rozestup mezi vozíky!<br><br>",true);
				}
			}
		}
		else//situace kdy nejsou pohony přiřazeny
		{
			if(O->pohon==NULL && D->pohon==NULL)//ani jeden z nich
			{
				F->Z("<b>"+O->name.UpperCase()+" a "+D->name.UpperCase()+"<br>problém</b> - objekty nemají přiřazeny pohony!<br><b>řešení</b> - přiřaďte patřičné pohony.<br><br>",true);
			}
			else//jenom jeden není přiřazen
			{
				if(O->pohon==NULL)F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - není přiřazen pohon!<br><b>řešení</b> - přiřaďte patřičný pohon.<br><br>",true);
				if(D->pohon==NULL && D!=v.OBJEKTY->dalsi)F->Z("<b>"+O->name.UpperCase()+"<br>problém</b> - následující objekt <b>"+D->name.UpperCase()+"</b> nemá přiřazen pohon!<br><b>řešení</b> - přiřaďte patřičný pohon objektu "+D->name.UpperCase()+".<br><br>",true);
			}
			i=10;//červeně šrafování, není kompletní a kompetentní info
		}

	 /*	rgb(255,140,0) - DarkOrange
		rgb(255,215,0) - Gold (žlutá)
		rgb(218,112,214) - Orchid (fialová)
		LightSkyBlue	rgb(135,206,250)
		//rgb(135,206,235) - SkyBlue
		rgb(152,251,152) -PaleGreen

		*/
		canv->Pen->Style=psClear;
		canv->Pen->Width=1;
		canv->Brush->Style=bsSolid;
		if(m.cele_cislo(Form1->Zoom)==false && i>0)canv->Pen->Style=psSolid;
		switch(i)
		{
			case 0:/*canv->Pen->Color=clWhite;canv->Brush->Color=clWhite*/;break;
			case 1:canv->Pen->Color=clRed;canv->Brush->Color=clRed;break;
			case 2:canv->Pen->Color=(TColor)RGB(255,140,0);canv->Brush->Color=(TColor)RGB(255,140,0);break;
			case 3:canv->Pen->Color=(TColor)RGB(255,215,0);canv->Brush->Color=(TColor)RGB(255,215,0);break;
			case 4:canv->Pen->Color=(TColor)RGB(218,112,214);canv->Brush->Color=(TColor)RGB(218,112,214);break;
			case 5:canv->Pen->Color=(TColor)RGB(135,206,250);canv->Brush->Color=(TColor)RGB(135,206,250);break;
			case 6:canv->Pen->Color=(TColor)RGB(152,251,152);canv->Brush->Color=(TColor)RGB(152,251,152);break;
			//default:
			default:canv->Brush->Style=bsDiagCross;canv->Pen->Color=clRed;canv->Brush->Color=clRed;break;
		}
		if(i==0) return 0;
		else return canv->Pen->Color;
}
//---------------------------------------------------------------------------
//nakreslí editační okno
void Cvykresli::editacni_okno(TCanvas *canv,unsigned int  X1, unsigned int  Y1, unsigned int  X2, unsigned int  Y2, unsigned short int size, COLORREF color)//implicitně černá a velikost 1
{
	canv->Pen->Width=size;
	canv->Pen->Style=psSolid;
	canv->Pen->Color=(TColor)color;
	canv->Pen->Mode=pmNotXor;
	canv->Brush->Style=bsClear;
	canv->Rectangle(X1,Y1,X2,Y2);//obdelník editačního okna
	canv->Pen->Mode=pmCopy;
	canv->Brush->Style=bsSolid;
}
//nakreslí editační okno, přetížená fce
void Cvykresli::editacni_okno(TCanvas *canv, TPoint LH, TPoint PD, unsigned short int size, COLORREF color)
{
	editacni_okno(canv, LH.x, LH.y, PD.x, PD.y, size, color);
}
//---------------------------------------------------------------------------
//označí nebo odznačí objekt používá se při posouvání objektů
void Cvykresli::odznac_oznac_objekt(TCanvas *canv, Cvektory::TObjekt *p, int posunX, int posunY,COLORREF color)
{
		if(p->id!=F->VyID)
		{
			//ShowMessage(UnicodeString(p->X)+" "+UnicodeString(p->Y));
			//nastavení pera
			canv->Pen->Color=(TColor)color;
			canv->Pen->Width=1;
			canv->Pen->Style=psDot;//nastevení čarkované čáry
			canv->Pen->Mode=pmNotXor;
			canv->Brush->Style=bsClear;

			//provizorní spojovací linie + znovupřekreslení zůčastněných objektů pro lepší vzhled
			if(v.OBJEKTY->predchozi->n>=3)//pokud budou alespoň 3 prky
			{
				if(p->n==1)//pokud se jedná o první prvek
				{
					canv->MoveTo(CorEx(p->predchozi->predchozi),CorEy(p->predchozi->predchozi));
					canv->LineTo(CorEx(p)+posunX,CorEy(p)+posunY);
					canv->LineTo(CorEx(p->dalsi),CorEy(p->dalsi));
					sipka(canv,(CorEx(p->predchozi->predchozi)+CorEx(p)+posunX)/2,(CorEy(p->predchozi->predchozi)+CorEy(p)+posunY)/2,m.azimut(p->predchozi->predchozi->X,p->predchozi->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->predchozi->X,p->predchozi->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					sipka(canv,(CorEx(p->dalsi)+CorEx(p)+posunX)/2,(CorEy(p->dalsi)+CorEy(p)+posunY)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,p->dalsi->X,p->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_rectangle(canv,v.OBJEKTY->predchozi);
						vykresli_rectangle(canv,p);
						vykresli_rectangle(canv,p->dalsi);
					}
				}
				if(p->n==v.OBJEKTY->predchozi->n)//pokud se jedná o poslední prvek
				{
					canv->MoveTo(CorEx(p->predchozi),CorEy(p->predchozi));
					canv->LineTo(CorEx(p)+posunX,CorEy(p)+posunY);
					canv->LineTo(CorEx(v.OBJEKTY->dalsi),CorEy(v.OBJEKTY->dalsi));
					sipka(canv,(CorEx(p->predchozi)+CorEx(p)+posunX)/2,(CorEy(p->predchozi)+CorEy(p)+posunY)/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					sipka(canv,(CorEx(v.OBJEKTY->dalsi)+CorEx(p)+posunX)/2,(CorEy(v.OBJEKTY->dalsi)+CorEy(p)+posunY)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((v.OBJEKTY->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((v.OBJEKTY->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_rectangle(canv,p->predchozi);
						vykresli_rectangle(canv,p);
						vykresli_rectangle(canv,v.OBJEKTY->dalsi);
					}
				}
				if(p->n!=1 && p->n!=v.OBJEKTY->predchozi->n)//pokud se nejedná o první ani poslední prvek
				{
					canv->MoveTo(CorEx(p->predchozi),CorEy(p->predchozi));
					canv->LineTo(m.L2Px(p->X)+O_width*Form1->Zoom/2+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom/2+posunY);
					canv->LineTo(CorEx(p->dalsi),CorEy(p->dalsi));
					sipka(canv,(CorEx(p->dalsi)+CorEx(p)+posunX)/2,(CorEy(p->dalsi)+CorEy(p)+posunY)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie následující za prvkem
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,p->dalsi->X,p->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					sipka(canv,(CorEx(p->predchozi)+CorEx(p)+posunX)/2,(CorEy(p->predchozi)+CorEy(p)+posunY)/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_rectangle(canv,p->predchozi);
						vykresli_rectangle(canv,p);
						vykresli_rectangle(canv,p->dalsi);
					}
				}
			}

			if(v.OBJEKTY->predchozi->n==2)//pokud budou pouze 2 prky - zde netřeba řešit výhybku, posun, neměla by být
			{
				if(p->n==1)//pokud se jedná o první prvek
				{
					canv->MoveTo(m.L2Px(p->X)+O_width*Form1->Zoom/2+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom/2+posunY);
					canv->LineTo(m.L2Px(p->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(p->dalsi->Y)+O_height*Form1->Zoom/2);
					sipka(canv,(m.L2Px(p->dalsi->X)+m.L2Px(p->X)+posunX+O_width*Form1->Zoom)/2,(m.L2Py(p->dalsi->Y)+m.L2Py(p->Y)+posunY+O_height*Form1->Zoom)/2,m.azimut(p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom,p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->dalsi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->dalsi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,p->dalsi->X,p->dalsi->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_rectangle(canv,p);
						vykresli_rectangle(canv,p->dalsi);
					}
				}
				else//pokud se jedná o druhý prvek
				{
					canv->MoveTo(m.L2Px(p->predchozi->X)+O_width*Form1->Zoom/2,m.L2Py(p->predchozi->Y)+O_height*Form1->Zoom/2);
					canv->LineTo(m.L2Px(p->X)+O_width*Form1->Zoom/2+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom/2+posunY);
					sipka(canv,(m.L2Px(p->predchozi->X)+m.L2Px(p->X)+O_width*Form1->Zoom+posunX)/2,(m.L2Py(p->predchozi->Y)+m.L2Py(p->Y)+O_height*Form1->Zoom+posunY)/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X+posunX*Form1->m2px/Form1->Zoom,p->Y-posunY*Form1->m2px/Form1->Zoom),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					//nevím k čemu to tady bylo také: sipka(canv,m.L2Px((p->predchozi->X+p->X)/2)+O_width*Form1->Zoom/2,m.L2Py((p->predchozi->Y+p->Y)/2)+O_height*Form1->Zoom/2,m.azimut(p->predchozi->X,p->predchozi->Y,p->X,p->Y),false,3,clBlack);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					{
						vykresli_rectangle(canv,p->predchozi);
						vykresli_rectangle(canv,p);
					}
				}
			}
			//samotná imitace posovaného objektu
			editacni_okno(canv, m.L2Px(p->X)+posunX,m.L2Py(p->Y)+posunY,m.L2Px(p->X)+O_width*Form1->Zoom+posunX,m.L2Py(p->Y)+O_height*Form1->Zoom+posunY,1);
		}
		else
		odznac_oznac_vyhybku(canv,m.L2Px(p->X)+posunX,m.L2Py(p->Y)+posunY,p,true);//mohlo by být zakomponováno výše
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//používá se pro přidávání objektu mezi poslední a první prvek
void Cvykresli::odznac_oznac_objekt_novy_posledni(TCanvas *canv,int X, int Y)
{
		if(F->vybrany_objekt!=F->VyID)
		{
			if(v.OBJEKTY->predchozi->n>0)//pokud už existuje nějaký prvek
			{
				if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
				{
					canv->Pen->Color=clBlack;
					canv->Pen->Width=1;
					canv->Pen->Style=psDot;//nastevení čarkované čáry
					canv->Pen->Mode=pmNotXor;

					canv->MoveTo(m.L2Px(v.OBJEKTY->predchozi->X)+O_width*Form1->Zoom/2,m.L2Py(v.OBJEKTY->predchozi->Y)+O_height*Form1->Zoom/2);
					canv->LineTo(X+O_width*Form1->Zoom/2,Y+O_height*Form1->Zoom/2);
					if(v.OBJEKTY->predchozi->n>1)//pokud je více než jenom jeden prvek
					{
						canv->LineTo(m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2);
						sipka(canv,(m.L2Px(v.OBJEKTY->dalsi->X)+X)/2+O_width*Form1->Zoom/2,(m.L2Py(v.OBJEKTY->dalsi->Y)+Y)/2+O_height*Form1->Zoom/2,m.azimut(m.P2Lx(X),m.P2Ly(Y),v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
						if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
						vykresli_rectangle(canv,v.OBJEKTY->dalsi); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
					}
					sipka(canv,(m.L2Px(v.OBJEKTY->predchozi->X)+X)/2+O_width*Form1->Zoom/2,(m.L2Py(v.OBJEKTY->predchozi->Y)+Y)/2+O_height*Form1->Zoom/2,m.azimut(v.OBJEKTY->predchozi->X,v.OBJEKTY->predchozi->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					vykresli_rectangle(canv,v.OBJEKTY->predchozi); //znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				}
			}
			editacni_okno(canv,X,Y,X+O_width*Form1->Zoom,Y+O_height*Form1->Zoom,1);
		}
		else
		odznac_oznac_vyhybku(canv,X,Y,NULL);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//používá se pro přidvání objektu mimo poslední a první prvek
void Cvykresli::odznac_oznac_objekt_novy(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p)
{
		if(F->vybrany_objekt!=F->VyID)
		{
			if(p->n>0)//pokud už existuje nějaký prvek
			{
				if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
				{
					canv->Pen->Color=clBlack;
					canv->Pen->Width=1;
					canv->Pen->Style=psDot;//nastevení čarkované čáry
					canv->Pen->Mode=pmNotXor;
					//spojovací linie
					canv->MoveTo(m.L2Px(p->X)+O_width*Form1->Zoom/2,m.L2Py(p->Y)+O_height*Form1->Zoom/2);
					canv->LineTo(X+O_width*Form1->Zoom/2,Y+O_height*Form1->Zoom/2);
					canv->LineTo(m.L2Px(p->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(p->dalsi->Y)+O_height*Form1->Zoom/2);
					sipka(canv,(m.L2Px(p->dalsi->X)+X)/2+O_width*Form1->Zoom/2,(m.L2Py(p->dalsi->Y)+Y)/2+O_height*Form1->Zoom/2,m.azimut(m.P2Lx(X),m.P2Ly(Y),p->dalsi->X,p->dalsi->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					vykresli_rectangle(canv,p->dalsi); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
					sipka(canv,(m.L2Px(p->X)+X)/2+O_width*Form1->Zoom/2,(m.L2Py(p->Y)+Y)/2+O_height*Form1->Zoom/2,m.azimut(p->X,p->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
					if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
					vykresli_rectangle(canv,p);//znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				}
			}
			editacni_okno(canv,X,Y,X+O_width*Form1->Zoom,Y+O_height*Form1->Zoom,1);
		}
		else
		odznac_oznac_vyhybku(canv,X,Y,p);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//pří umistivání či posouvání vyhýbky
void Cvykresli::odznac_oznac_vyhybku(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p,bool posun)
{
		//definice souřadnic
		Cvektory::TObjekt *p1=NULL; if(p==NULL)p1=v.OBJEKTY->predchozi;else p1=p;
		Cvektory::TObjekt *p2=NULL;	if(p==NULL)p2=v.OBJEKTY->dalsi;		 else p2=p->dalsi;
		if(posun)p1=p->predchozi;//pokud se jedná o posun objektu
		if(posun && p->dalsi==NULL)p2=v.OBJEKTY->dalsi;//pokud se jedná o posun objektu a jedná se o poslední prvek

    //samotný algoritmus
		if(p1->n>0)//pokud už existuje nějaký prvek
		{
			if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
			{
				//nastavení pera
				canv->Pen->Color=clBlack;
				canv->Pen->Width=1;
				canv->Pen->Style=psDot;//nastevení čarkované čáry
				canv->Pen->Mode=pmNotXor;
				canv->Brush->Style=bsClear;
				//spojovací linie
				canv->MoveTo(m.L2Px(p1->X)+O_width*Form1->Zoom/2,m.L2Py(p1->Y)+O_height*Form1->Zoom/2);
				canv->LineTo(X,Y);
				canv->LineTo(m.L2Px(p2->X)+O_width*Form1->Zoom/2,m.L2Py(p2->Y)+O_height*Form1->Zoom/2);
				sipka(canv,(m.L2Px(p2->X)+O_width*Form1->Zoom/2+X)/2,(m.L2Py(p2->Y)+O_height*Form1->Zoom/2+Y)/2,m.azimut(m.P2Lx(X),m.P2Ly(Y),p2->X,p2->Y),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
				if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
				vykresli_rectangle(canv,p2); //znovupřekreslení zúčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				sipka(canv,(m.L2Px(p1->X)+O_width*Form1->Zoom/2+X)/2,(m.L2Py(p1->Y)+O_height*Form1->Zoom/2+Y)/2,m.azimut(p1->X,p1->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
				if(grafickeDilema)//provizorní proměnná na přepínání stavu, zda se při přidávání objektu a přesouvání objektu bude zmenšovat písmo nebo nepřekreslovat objekt
				vykresli_rectangle(canv,p1);//znovupřekreslení zůčastněných objektů pro lepší vzhled, nyní řešeno v formmousedown viz  d.odznac_oznac_objekt, nevýhodou pouze zůstavá překreslování linie v místě objektu
				//nutné znovu nastavení pera
				canv->Pen->Color=clBlack;
				canv->Pen->Width=1;
				canv->Pen->Style=psDot;//nastevení čarkované čáry
				canv->Pen->Mode=pmNotXor;
				canv->Brush->Style=bsClear;
				//kruh
				canv->Ellipse(X-V_width*F->Zoom,Y-V_width*F->Zoom,X+V_width*F->Zoom,Y+V_width*F->Zoom);
			}
		}
		p1=NULL;delete p1;
		p2=NULL;delete p2;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí či odznačí potenciální umístění větve, další fáze po umístění výhybky
void Cvykresli::odznac_oznac_vetev(TCanvas *canv, int X, int Y,Cvektory::TObjekt *p)
{
		if(X!=-200 && Y!=-200)//spojovací linie  //pokud je mimo obraz -200 jen nahodilá hodnota
		{
			//nastavení pera
			canv->Pen->Color=clBlack;
			canv->Pen->Width=1;
			canv->Pen->Style=psDot;//nastevení čarkované čáry
			canv->Pen->Mode=pmNotXor;
			canv->Brush->Style=bsClear;

			//vykreslení spojovací linie
			canv->MoveTo(m.L2Px(p->X),m.L2Py(p->Y));
			canv->LineTo(X,Y);
			sipka(canv,(m.L2Px(p->X)+X)/2,(m.L2Py(p->Y)+Y)/2,m.azimut(p->X,p->Y,m.P2Lx(X),m.P2Ly(Y)),true,3,clBlack,clWhite,pmNotXor);//zajistí vykreslení šipky - orientace spojovací linie
			canv->Ellipse(X-V_width*F->Zoom,Y-V_width*F->Zoom,X+V_width*F->Zoom,Y+V_width*F->Zoom);
		}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cvykresli::lezi_v_pasmu(TCanvas *c,long X,long Y,Cvektory::TObjekt *p,bool odecti_region)
{
		bool ret=false;

		int W=m.round(O_width*Form1->Zoom);
		int H=m.round(O_height*Form1->Zoom);

		if((p->X<=p->dalsi->X && p->Y<p->dalsi->Y)||(p->X>p->dalsi->X && p->Y>=p->dalsi->Y))
		ret=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y),m.L2Px(p->X)+W,m.L2Py(p->Y)+H,m.L2Px(p->dalsi->X)+W,m.L2Py(p->dalsi->Y)+H,m.L2Px(p->dalsi->X),m.L2Py(p->dalsi->Y),odecti_region);

		if((p->X>p->dalsi->X && p->Y<p->dalsi->Y)||(p->X<=p->dalsi->X && p->Y>=p->dalsi->Y))
		ret=lezi_v_pasmu(c,X,Y,m.L2Px(p->X),m.L2Py(p->Y)+H,m.L2Px(p->X)+W,m.L2Py(p->Y),m.L2Px(p->dalsi->X)+W,m.L2Py(p->dalsi->Y),m.L2Px(p->dalsi->X),m.L2Py(p->dalsi->Y)+H,odecti_region);

		return ret;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cvykresli::lezi_v_pasmu_poslednim(TCanvas *c,long X,long Y,bool odecti_region)
{
		bool ret=false;
		if(v.OBJEKTY->predchozi!=NULL)
		{
			int W=m.round(O_width*Form1->Zoom);
			int H=m.round(O_height*Form1->Zoom);

			if((v.OBJEKTY->predchozi->X<=v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y<v.OBJEKTY->dalsi->Y)||(v.OBJEKTY->predchozi->X>v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y>=v.OBJEKTY->dalsi->Y))
			ret=lezi_v_pasmu(c,X,Y,m.L2Px(v.OBJEKTY->predchozi->X),m.L2Py(v.OBJEKTY->predchozi->Y),m.L2Px(v.OBJEKTY->predchozi->X)+W,m.L2Py(v.OBJEKTY->predchozi->Y)+H,m.L2Px(v.OBJEKTY->dalsi->X)+W,m.L2Py(v.OBJEKTY->dalsi->Y)+H,m.L2Px(v.OBJEKTY->dalsi->X),m.L2Py(v.OBJEKTY->dalsi->Y),odecti_region);

			if((v.OBJEKTY->predchozi->X>v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y<v.OBJEKTY->dalsi->Y)||(v.OBJEKTY->predchozi->X<=v.OBJEKTY->dalsi->X && v.OBJEKTY->predchozi->Y>=v.OBJEKTY->dalsi->Y))
			ret=lezi_v_pasmu(c,X,Y,m.L2Px(v.OBJEKTY->predchozi->X),m.L2Py(v.OBJEKTY->predchozi->Y)+H,m.L2Px(v.OBJEKTY->predchozi->X)+W,m.L2Py(v.OBJEKTY->predchozi->Y),m.L2Px(v.OBJEKTY->dalsi->X)+W,m.L2Py(v.OBJEKTY->dalsi->Y),m.L2Px(v.OBJEKTY->dalsi->X),m.L2Py(v.OBJEKTY->dalsi->Y)+H,odecti_region);
		}
		return ret;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cvykresli::lezi_v_pasmu(TCanvas *c,long X,long Y,long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,bool odecti_region)
{
		POINT *body=new POINT[4];
		body[0].x=x1;body[0].y=y1;
		body[1].x=x2;body[1].y=y2;
		body[2].x=x3;body[2].y=y3;
		body[3].x=x4;body[3].y=y4;

		HRGN hreg=CreatePolygonRgn(body,4,WINDING);//vytvoření regionu
		if(odecti_region)//aby nevybíral oblast i samotných objektů, tento region odečtu
		{
				HRGN hregO=CreateRectRgn(x1,y1,x2,y2);
				HRGN hregO1=CreateRectRgn(x3,y3,x4,y4);
				CombineRgn(hreg,hreg,hregO,RGN_DIFF);
				DeleteObject(hregO);
				CombineRgn(hreg,hreg,hregO1,RGN_DIFF);
				DeleteObject(hregO1);
		}
		//c->Brush->Color = RGB(96, 96, 96);  //testovací
		//FrameRgn(c->Handle,hreg,c->Brush->Handle,5,5);//testovací
		bool ret=PtInRegion(hreg,X,Y);
		delete body;body=NULL;
		DeleteObject(hreg);
		return ret;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje vykreslení layoutu
//void Cvykresli::vykresli_layout(TCanvas *canv)
//{//	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
//	if(O!=NULL)//pokud existuje nějaký objekt
//	{
//		 ////stanovení rozeměrů a orientace nosného obdelníku layoutu
//		 //délka stran nosného obdelníka
//		 double obvod=v.vrat_soucet_delek_vsech_objektu();
//		 int W=Form1->ClientWidth;
//		 int H=Form1->ClientHeight-Form1->scGPPanel_statusbar->Height-Form1->scGPPanel_mainmenu->Height;
//		 double A=m.round(obvod/2*W/((W+H)/1.0));//rozměry poměrově k monitoru, vůči monitoru
//		 double B=obvod/2-A;//B se už jenom dopočítá jako rozdíl
//		 //orietnace nosného obdelníka dle zakresleného schématu
//		 short smerZ=0;if(O->dalsi!=NULL)smerZ=m.round(m.azimut(O->X,O->Y,O->dalsi->X,O->dalsi->Y)/90.0)*90;//udá uvodní směr obdelníku
//		 short smerDo=0;if(O->predchozi!=NULL)smerDo=m.round(m.azimut(O->predchozi->predchozi->X,O->predchozi->predchozi->Y,O->X,O->Y)/90.0)*90;//udá uvodní směr obdelníku
//		 double X1=O->X,Y1=O->Y,X2=O->X,Y2=O->Y,X3=O->X,Y3=O->Y,X4=O->X,Y4=O->Y;
//		 if(smerZ==90&&(smerDo==0||smerDo==90)){X2+=A;X3+=A;Y3-=B;Y4-=B;}
//		 if(smerZ==90&&smerDo==180){X2+=A;X3+=A;Y3+=B;Y4+=B;}
//		 if(smerZ==270&&(smerDo==0||smerDo==270)){X2-=A;X3-=A;Y3-=B;Y4-=B;}
//		 if(smerZ==270&&smerDo==180){X2-=A;X3-=A;Y3+=B;Y4+=B;}
//		 if(smerZ==0&&(smerDo==270||smerDo==0)){Y2+=B;X3+=A;Y3+=B;X4+=A;}
//		 if(smerZ==0&&smerDo==90){Y2+=B;X3+=A;Y3+=B;X4-=A;Y4+=B;}
//		 if(smerZ==180&&(smerDo==270||smerDo==180)){Y2-=B;X3+=A;Y3-=B;X4+=A;}
//		 if(smerZ==180&&smerDo==90){Y2-=B;X3-=A;Y3-=B;X4-=A;}
//		 //uložení bodů nosného obdelníka pro další použití
//		 short velikost_pole=4;
//		 TPointD *P=new TPointD[velikost_pole];//vytovoří pole pro polyline
//		 P[0].x=X1;P[0].y=Y1;P[1].x=X2;P[1].y=Y2;P[2].x=X3;P[2].y=Y3;P[3].x=X4;P[3].y=Y4;
//		 ////----
//
//		 ////grafické nastavení
//		 //pero+výplň
//		 canv->Brush->Color=clWhite;
//		 canv->Brush->Style=bsSolid;
//		 canv->Pen->Color=clRed;
//		 canv->Pen->Width=Form1->Zoom*1;if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*1;
//		 //font
//		 canv->Font->Color=clRed;
//		 SetBkMode(canv->Handle,TRANSPARENT);
//		 canv->Font->Size=Form1->Zoom*9; if(Form1->antialiasing)canv->Font->Size=Form1->Zoom*9;
//		 canv->Font->Name="Arial";
//
//		 ////pomocné promněnné
//		 double akt_pozice=0;//v segmentu
//		 double zbytek=0;//z předchozí segmentu
//		 unsigned int i=0;//číslo prvního bodu segmentu
//		 TPointD S;
//		 S.x=P[i].x;S.y=P[i].y;//na uchování výchozí pozice
//
//		 ////procházení jednotlivých objektů
//		 while (O!=NULL)
//		 {
//			////stanovení resp. načtení délky a šířky objektu
//		 	double delka=0;
//		 	double sirkaV=0;
//		 	double delkaV=0;
//		 	if(O->rezim==0)//S&G, u tohoto režimu se bere délka nebo šířka vozíku, dle nastavení, zda je vozík v objektu na šířku či na délku
//			{
//		 		if(O->rotace==0)
//		 		{
//		 			delka=v.PP.delka_voziku;delkaV=delka;
//		 			sirkaV=v.PP.sirka_voziku;
//				}
//		 		else
//		 		{
//		 			delka=v.PP.sirka_voziku;delkaV=delka;
//		 			sirkaV=v.PP.delka_voziku;
//				}
//		 	}
//		 	else //u kontinuálního a pp se uvažuje jako délka přímo jen délka dopravníku
//		 	{
//		 		delka=O->delka_dopravniku;
//				if(O->rotace==0)
//		 		{
//		 			sirkaV=v.PP.sirka_voziku;//sířka dle rotace (pokud je na délku či šířku)
//		 			delkaV=v.PP.delka_voziku;
//		 		}
//				else
//		 		{
//		 			sirkaV=v.PP.delka_voziku;
//		 			delkaV=v.PP.sirka_voziku;
//		 		}
//			}
//		  ////----
//
//		 	////vykreslení jednoho objektu
//			bool popisek_se_jiz_vypisoval=false;//detekce, zda se má vypsat popisek za zlomem či ne
//			unsigned int Poz_i=0;//pozice se indexují od začátku objektu
//			do
//			{
//					bool posunuti_segmentu=false;//dekece zda se má zvýšit počítadlo pozice segmentu - i
//					unsigned int n=i;if(i<3)n=i+1;else n=0;//"přeindexování" pokude se bude jednat o poslední spojnici (tj. poslední-první prvek)
//					double DS=m.delka(P[i].x,P[i].y,P[n].x,P[n].y);//delka_segmentu obrazce
//		 			if(zbytek>0)delka=zbytek;//zbytek objektu z minulého segmentu
//		 			if(delka+akt_pozice>=DS)//přetekl do dalšího segmentu
//		 			{
//		 				zbytek=delka+akt_pozice-DS;
//						delka-=zbytek;
//						akt_pozice=0;
//						posunuti_segmentu=true;//posunutí segmentu
//		 				//ShowMessage("přetekl "+AnsiString(i));
//		 			}
//					else//nepřetekl
//		 			{
//		 				akt_pozice+=delka;
//		 				zbytek=0;
//						posunuti_segmentu=false;
//						Poz_i=0;
//		 				//ShowMessage("nepřetekl "+AnsiString(i));
//		 			}
//
//					double PO=delka/DS;//pomer delky objektu a segmentu obrazce
//					//ShowMessage(AnsiString(delka)+"/"+AnsiString(DS));
//
//		 			////vykreslení
//					//objekt
//					TPointD S_puv=S;
//					canv->MoveTo(m.L2Px(S.x+sirkaV/2),m.L2Py(S.y));//pero na výchozí (minulou pozici)
//					S.x+=(P[n].x-P[i].x)*PO;//posun ze začátku objektu nakonec
//					S.y+=(P[n].y-P[i].y)*PO;//posun ze začátku objektu nakonec
//					canv->Pen->Width=Form1->Zoom*1;if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*1;canv->Pen->Color=clRed;
//					canv->LineTo(m.L2Px(S.x+sirkaV/2),m.L2Py(S.y));//nakreslení linie
//
//					//pozice - vykreslí pozice v daném objektu
//					Poz_i=vykresli_pozice(canv,Poz_i,S_puv,S,delka,delkaV,sirkaV,O->mezera);
//
////					canv->Pen->Width=Form1->Zoom*3;if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*3;
////					canv->Brush->Color=clRed;
////					canv->FrameRect(TRect(m.L2Px(S_puv.x)-1,m.L2Py(S_puv.y+sirkaV/2)+1,m.L2Px(S.x)+1,m.L2Py(S.y-sirkaV/2)-1));//provizorní řešení potom polyline
//
//
//					//zarazka
//					if(zbytek==0)//zarážka se zobrazí pouze pokud se nepokračuje ve vykreslování objektu v dalším segmentu
//		 			{
//						 double Alfa=m.azimut(S_puv.x,S_puv.y,S.x,S.y)+90;if(Alfa>=360)Alfa-=360;
//						 //ShowMessage(Alfa);
//						 if(posunuti_segmentu)Alfa=135;//v případě rohu je to 45°
//						 Alfa*=(M_PI/180);
//						 set_pen(canv,clRed,Form1->Zoom*2,PS_ENDCAP_SQUARE);//nastavení pera barvy osy
//						 //canv->Pen->Width=Form1->Zoom*2;canv->Pen->Color=clRed;
//
//						 canv->MoveTo(m.L2Px(S.x-sin(Alfa)*sirkaV/2),m.L2Py(S.y-cos(Alfa)*sirkaV/2));
//						 canv->LineTo(m.L2Px(S.x+sin(Alfa)*sirkaV/2),m.L2Py(S.y+cos(Alfa)*sirkaV/2));
//					}
//
////					//popisek objektu
////					canv->Brush->Color=clWhite;
////					canv->Font->Color=clRed;
////					if(zbytek<delka && popisek_se_jiz_vypisoval==false)//zajistí, že se vypisuje pouze jednou a navíc v tom z delších segmentů
////					{
////
////						AnsiString T=O->name.UpperCase();
////						AnsiString T1="K: "+AnsiString(O->kapacita)+"[v] DD: "+AnsiString(O->delka_dopravniku)+" [m]";
////						if(O->rezim==2)T1+=" RD: "+AnsiString(O->RD).SubString(1,5)+" [m/s]";//pokud se jedná o kontinual, tak ještě RD
////						double A=m.azimut(S_puv.x,S_puv.y,S.x,S.y);
////						bool RT=false;//rotace textu ano ne
////						if(A==270 ||  A==90)rotace_textu(canv,0);else {RT=true;rotace_textu(canv,A*10-900);}//rotace textu,ošetření, aby se zprava doleva nevypisovalo obráceně
////						if(!RT)
////						{
////							canv->TextOutW(m.L2Px((S.x+S_puv.x)/2.0)-canv->TextWidth(T)/2,m.L2Py((S.y+S_puv.y+sirkaV)/2.0)-canv->TextHeight(T),T);//vypíše název objektu uprostřed nad
////							canv->TextOutW(m.L2Px((S.x+S_puv.x)/2.0)-canv->TextWidth(T1)/2,m.L2Py((S.y+S_puv.y-sirkaV)/2.0),T1);//vypíše parametry objektu uprostřed pod
////						}
////						else
////						{  //ShowMessage(T+AnsiString(A));
////							if(A==180)
////							{
////								canv->TextOutW(m.L2Px((S.x+S_puv.x-sirkaV)/2.0+sirkaV/2)-canv->TextHeight(T)-1,m.L2Py((S.y+S_puv.y)/2.0)+canv->TextWidth(T)/2,T);//vypíše parametry objektu uprostřed pod
////								canv->TextOutW(m.L2Px((S.x+S_puv.x+sirkaV)/2.0+sirkaV/2)+1,m.L2Py((S.y+S_puv.y)/2.0)+canv->TextWidth(T1)/2,T1);//vypíše název objektu uprostřed nad
////							}
////							else
////							{
////								canv->TextOutW(m.L2Px((S.x+S_puv.x+sirkaV)/2.0+sirkaV/2)+canv->TextHeight(T)+1,m.L2Py((S.y+S_puv.y)/2.0)-canv->TextWidth(T)/2,T);//vypíše název objektu uprostřed nad
////								canv->TextOutW(m.L2Px((S.x+S_puv.x-sirkaV)/2.0+sirkaV/2)-1,m.L2Py((S.y+S_puv.y)/2.0)-canv->TextWidth(T1)/2,T1);//vypíše parametry objektu uprostřed pod
////							}
////						}
////						popisek_se_jiz_vypisoval=true;
////					}
//
//					//posunutí na další segment vykreslovaného obrazce layoutu
//					if(posunuti_segmentu)i++;
//			}
//		 	while(zbytek>0);
//
//			//posun na další prvek
//			O=O->dalsi;
//		 }
//
////		 //prozatim testovac9 //vykreslení oramování obdelníku
////		 canv->Pen->Color=clGray;
////		 canv->Brush->Color=clWhite;
////		 POINT O1[4]={{m.L2Px(X1),m.L2Py(Y1)},{m.L2Px(X2),m.L2Py(Y2)},{m.L2Px(X3),m.L2Py(Y3)},{m.L2Px(X4),m.L2Py(Y4)}};
////		 canv->Polygon((TPoint*)O1,3);
//
//		 ////celkový středový výpis
//		 rotace_textu(canv,0);
//		 canv->Font->Color=clGray;
//		 canv->Brush->Color=clWhite;
//		 W=0;//nejširší text
//
//		 AnsiString T1="obvod linky: "+AnsiString(obvod)+" [m]";if(canv->TextWidth(T1)>W)W=canv->TextWidth(T1);
//		 AnsiString T2="kapacita linky: "+AnsiString(v.WIP(1))+" [voz.]";if(canv->TextWidth(T2)>W)W=canv->TextWidth(T2);
//     //T3-T5 bylo zakázáno používat
//		 /*AnsiString T3="akt. plocha linky: "+AnsiString(A*B)+" [m2]";W=canv->TextWidth(T3);
//		 AnsiString T4="akt. X-rozměr linky: "+AnsiString(A)+" [m]";if(canv->TextWidth(T4)>W)W=canv->TextWidth(T4);
//		 AnsiString T5="akt. Y-rozměr linky: "+AnsiString(B)+" [m]";if(canv->TextWidth(T5)>W)W=canv->TextWidth(T5);*/
//		 AnsiString T6="lead time: "+AnsiString(v.vrat_LT())+" [s]";if(canv->TextWidth(T6)>W)W=canv->TextWidth(T6);
//		 /*if(P[0].x!=P[3].x)
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T1)*3,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T2)*2,T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T2)*1,T3);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2),T4);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)+canv->TextHeight(T5)*1,T5);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)+canv->TextHeight(T6)*2,T6);
//		 }
//		 else
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T1)*3,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T2)*2,T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T3)*1,T3);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2),T4);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)+canv->TextHeight(T5)*1,T5);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)+canv->TextHeight(T6)*2,T6);
//		 }*/
//		 if(P[0].x!=P[3].x)
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)-canv->TextHeight(T1)*1,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2),T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[3].x)/2)-W/2,m.L2Py((P[0].y+P[1].y)/2)+canv->TextHeight(T6)*1,T6);
//		 }
//		 else
//		 {
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)-canv->TextHeight(T1)*1,T1);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2),T2);
//			canv->TextOutW(m.L2Px((P[0].x+P[1].x)/2)-W/2,m.L2Py((P[0].y+P[3].y)/2)+canv->TextHeight(T6)*1,T6);
//		 }
//	}
//}////------------------------------------------------------------------------------------------------------------------------------------------------------//duplicita - pouze pro testovací účely, hlavní odstavena
void Cvykresli::vykresli_layout(TCanvas *canv)
{
	Cvektory::TObjekt *O=v.OBJEKTY->dalsi;//přeskočí hlavičku
	while(O!=NULL)//pokud existuje nějaký objekt
	{
		//vykresli_objekt(canv,O,0,50);
		//vykresli_objekt(bmp_in->Canvas,pom,F->m.P2Lx(Ox/F->m2px),F->m.P2Ly(F->m.round((scGPPanel_hlavicka->Height+Height)*3/2.0)),Poffset,Timer_animace->Enabled);
		O=O->dalsi;	}	O=NULL;delete O;}////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajistí vykreslení náhledu objektu, XY -umístění L začátek (střed dopravníku) objektu v m, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
//za zmínění stojí lokální proměnná KR, což je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, DV - konec,
unsigned int Cvykresli::vykresli_objekt(TCanvas *canv,Cvektory::TObjekt *O,double X,double Y,double Poffset,bool animace)
{
	////vychozí geometrické proměnné
	double DD=O->delka_dopravniku;//délka objektu v metrech
	double dJ=m.UDJ(v.PP.delka_jig,v.PP.sirka_jig,O->rotace);//délka jigu
	double sJ=m.UDJ(v.PP.sirka_jig,v.PP.delka_jig,O->rotace);//šířka jigu a tím pádem i minimální kabiny
	double dP=v.PP.delka_podvozek;
	double DV=dJ;if(dP>dJ)DV=dP;
	double M=O->mezera;//mezera
	double R=0;if(O->pohon!=NULL)R=O->pohon->roztec;//rozteč palců řetězu
	double KR=0;//kalibrace řetězu vůči podvozku např. 0 - střed, -DP/2 - začátek, DP/2 - konec, či libovolný v m od začátku podvozku
	TPointD S;S.x=X;S.y=Y;//Start
	TPointD K;K.x=X+DD;K.y=Y;//Konec
																 //ShowMessage("R="+AnsiString(R)+"Rz="+AnsiString(M+DV)+"Rx="+AnsiString(F->m.round((M+DV)/R)));
	////obrys objektu
	//pero+výplň
	canv->Brush->Color=clWhite;
	canv->Brush->Style=bsSolid;
	canv->Pen->Color=clRed;        //pův. 0.5 bez duble linie
	canv->Pen->Width=Form1->Zoom*0.2;//if(Form1->antialiasing)canv->Pen->Width=Form1->Zoom*0.1;
	//samotné vykreslení obrysu kabiny, dvojitou linii, ale pozor může být nepříjemné ve vykreslování celkového layoutu!!!
	short Ov=Form1->Zoom*0.4;
	canv->Rectangle(m.L2Px(X)-Ov,m.L2Py(Y+sJ/2)-Ov,m.L2Px(K.x)+Ov,m.L2Py(K.y-sJ/2)+Ov);//dvojitý rám - vnější
	canv->Rectangle(m.L2Px(X)+Ov,m.L2Py(Y+sJ/2)+Ov,m.L2Px(K.x)-Ov,m.L2Py(K.y-sJ/2)-Ov);//dvojitý rám - vnitřní
	//canv->Rectangle(m.L2Px(X),m.L2Py(Y+SV/2),m.L2Px(K.x),m.L2Py(K.y-SV/2));//jenom jednoduché orámování

	////vykreslení řetězu a palců řetězu
	if(O->pohon!=NULL && O->rezim==1)//řetez - je-li přiřazen pohon a jedná se kontinuální režim
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Width=F->Zoom*0.5;
		canv->MoveTo(m.L2Px(X),m.L2Py(Y));
		canv->LineTo(m.L2Px(K.x),m.L2Py(K.y));
		//palce, pokud je zadaná rozteč tak se vykreslí
		if(R>0)
		{
			//palce řetězu   //pozn. M+DV je Rz
			int Rx=F->m.round((M+DV)/R);//může být zaokrouhleno, protože musí vycházet celé číslo
													 //*O->pozice - používát jen v animaci, kvůli tomu, aby byl řetěz dostatečně dlouhý
			double startR=-(M+DV)+KR+Poffset;//start je Rz=M+DV (tj. minulý vozík teoreticky mimo objekt, aby se vykreslily i palce před prvním vozíkem v objekt) a K je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, či jiné v m vůči počátku jigu, DV - konec, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
			if(animace)startR=-(M+DV)*ceil(O->pozice)+KR+Poffset;//start je Rz=M+DV (tj. minulý vozík teoreticky mimo objekt, aby se vykreslily i palce před prvním vozíkem v objekt) a K je kalibrace posunutí řetězu, kalibrace řetězu vůči vozíku např. DV/2.0 - střed, 0 - začátek, či jiné v m vůči počátku jigu DV - konec, Poffset - poziční poloha, výchozí poloha prvního vozíku/pozice v objektu (a vůči tomuto objektu),může sloužit na animaci či návaznost v případě layoutu
			unsigned int j=0;      //+R pouze grafická záležitost, aby na výstupu neořezávalo palce
			for(double i=startR;i<=DD+R;i+=R)
			{     //již využívám přemaskování bílým obdélníkem, nakonci této metody, zajišťuje lepší grafický efekt
				if(/*i>=0 && */Rx>0)//zobrazí se pouze ty, které jsou v objektu (řeší pro začátek, konec řeší podmínka, která je součástí for cyklu), druhá část podmínky je pouze ošetření, což paralelně řeší i výjimka
				{
					try//ošetření situaci při real-time nastavování parametrů, tak v situacích, kdy nebyly, ještě hodnoty od uživatele dopsány a přepočítány, Rx bylo 0
					{
						if(j%Rx==0){vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,true);}//palec vyšel do rozestupu, jedná se o aktivní palec
						else vykresli_palec(canv,m.L2Px(X+i),m.L2Py(Y),true,false);//jinak pasivní
					}
					catch(...){;}
				}
				j++;//musí být mimo
			}
		}
	}

	////jednotlivé pozice/vozíky
	unsigned int RET;
	if(!animace)RET=vykresli_pozice(canv,ceil(O->pozice)/*bylo pro číslování od jedné: 1*/,S,K,DD,dJ,sJ,dP,M,Poffset);
	else RET=vykresli_pozice(canv,ceil(O->pozice)*2/*bylo pro číslování od jedné: -O->pozice+1*/,S,K,DD,dJ,sJ,dP,M,-(M+DV)*ceil(O->pozice)+Poffset);

	////maskování vstupu a výstup, řetězu, pokud budu chtít,
	//aby byly vidět vstupující a vystupující vozíky musím přesunout před "////jednotlivé pozice/vozíky"  ,takto nyní nejsou vidět
//	canv->Brush->Color=clWhite;
//	canv->Brush->Style=bsSolid;      //+1 pouze rozšíření přes indexaci vozíků
//	canv->FillRect(TRect(0,m.L2Py(Y+sJ/2+1)-Ov,m.L2Px(X)-Ov-Ov/2,m.L2Py(K.y-sJ/2)+Ov));//nalevo
//	canv->FillRect(TRect(m.L2Px(K.x)+Ov+Ov/2,m.L2Py(Y+sJ/2+1)-Ov,Form_objekt_nahled->Width*3,m.L2Py(K.y-sJ/2)+Ov));//napravo

	return RET;//vrátí index
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje vykreslení pozic v layoutu + příprava konstrukce když nebudu chtít vykreslovat objekt vodorovně, pouze bude nutné zajistit ještě rotaci pozic a podvozků
unsigned int Cvykresli::vykresli_pozice(TCanvas *canv,int i,TPointD OD, TPointD DO,double delka,double delkaV,double sirkaV,double delkaP,double mezera,double akt_pozice)
{
	//grafické nastavení
	canv->Brush->Color=clWhite;//pozadí textu a podvozku a jigu
	canv->Brush->Style=bsClear;//transparentní pozadí (nejenom textu ale ji podvozku a jigu) ALTERNATIVA pro font:SetBkMode(canv->Handle,TRANSPARENT);
	canv->Pen->Width=1/3.0*F->Zoom;

	//font
	rotace_textu(canv,0);
	short SF_puv=canv->Font->Size;//uchování původní velikosti textu
	canv->Font->Name="Arial";
	canv->Font->Style = TFontStyles();
	canv->Font->Pitch = TFontPitch::fpFixed;//každé písmeno fontu stejně široké
	canv->Font->Pitch = System::Uitypes::TFontPitch::fpFixed;

	//vykreslování jednotlivých obdelníčků - pozic
	TPointD S;S=OD;//konstrukce proto,když nebudu chtít vykreslovat objekt vodorovně, pouze bude nutné zajistit ještě rotaci pozic a podvozků
	S.x+=(DO.x-OD.x)*akt_pozice/delka;//sřed pozice
	S.y+=(DO.y-OD.y)*akt_pozice/delka;//střed pozice
	double PO=delkaV/delka;if(delkaV<delkaP)PO=delkaP/delka;//pomer užitné delky vozíku a delky objektu
	double POm=mezera/delka;//poměr délky mezery a délky objekty
	while(akt_pozice<=delka)
	{
		//volání kompelexního vykreslení jednoho vozíku (podvozek včetně jigu)
		vykresli_vozik(canv,i,S.x,S.y,delkaP,delkaV,sirkaV);
		//posun o další vozík
		S.x+=(DO.x-OD.x)*PO;//posun ze začátku objektu nakonec
		S.y+=(DO.y-OD.y)*PO;//posun ze začátku objektu nakonec
		//posun o mezeru
		S.x+=(DO.x-OD.x)*POm;//posun ze začátku objektu nakonec
		S.y+=(DO.y-OD.y)*POm;//posun ze začátku objektu nakonec
		akt_pozice+=delkaV+mezera;//posun na další pozici
		//dekrementace
		i--;
	}
	canv->Font->Size=SF_puv;//vrátí původní velikost fontu
	return i;
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslení jednoho komplexního vozíku (podvozek včetně jigu)
void Cvykresli::vykresli_vozik(TCanvas *canv,int ID, double X,double Y,double dP,double dJ,double sJ,double rotaceP,double rotaceJ,TColor clChassis, TColor clJig)
{
	float sP=0.12;//šířka podvozku, pouze stanovane

	//transparentní pozadí (nejenom textu ale ji podvozku a jigu) ALTERNATIVA pro font:SetBkMode(canv->Handle,TRANSPARENT);
	canv->Brush->Style=bsClear;

	////zvýraznění animovaného dle CT
	if(ID==ceil(Form_objekt_nahled->pom->pozice) && Form_objekt_nahled->Timer_animace->Enabled)//v případě animace zvýrazní pozici, pro kterou se čítá technologický čas
	{
		canv->Pen->Width=3/3.0*F->Zoom;canv->Pen->Color=clWebOrange;//nebo lze použít: clYellow;
		canv->Rectangle(m.L2Px(X-dP/2),m.L2Py(Y+sP),m.L2Px(X+dP/2),m.L2Py(Y-sP));//podvozek
		canv->Rectangle(m.L2Px(X-dJ/2),m.L2Py(Y+sJ/2.0),m.L2Px(X+dJ/2),m.L2Py(Y-sJ/2.0));//jig
		canv->Pen->Width=1/3.0*F->Zoom;
	}

	////podvozek
	canv->Pen->Color=clChassis;
	canv->Rectangle(m.L2Px(X-dP/2),m.L2Py(Y+sP),m.L2Px(X+dP/2),m.L2Py(Y-sP));//vykreslení pozice podvozku

	////jig
	canv->Pen->Color=clJig;
	canv->Rectangle(m.L2Px(X-dJ/2),m.L2Py(Y+sJ/2.0),m.L2Px(X+dJ/2),m.L2Py(Y-sJ/2.0));

	////text - ID vozíku
	//framing
	if(Form1->Zoom>10)//pokud je více přiblížený objekt, tak se používá pouze framing, jinak bílé pozadí, pro lepší přehlednost
	{
		canv->Font->Color=clWhite;
		canv->Font->Style = TFontStyles()<<fsBold;//vypnutí tučného písma
		canv->Font->Size=Form1->Zoom*(4+1); if(Form1->antialiasing)canv->Font->Size=Form1->Zoom*(5+1);
		canv->TextOutW(m.L2Px(X)-canv->TextWidth(ID)/2.0,m.L2Py(Y+sJ/2.0)-canv->TextHeight(ID)/2.0,ID);//indexace pozice v rámci objektu
	}
	//samotný text
	if(Form1->Zoom<=10)canv->Brush->Style=bsSolid;//pokud je více přiblížený objekt, tak se používá pouze framing, jinak bílé pozadí, pro lepší přehlednost//bez bílého pozadí toto zrušit/zakomentovat pokud bych chtěl bílý framing, ten jsem dělal pomocí tučného písma a fontu o 1pt větší
	canv->Font->Color=clJig;
	canv->Font->Style = TFontStyles();//vypnutí tučného písma
	canv->Font->Size=Form1->Zoom*4; if(Form1->antialiasing)canv->Font->Size=Form1->Zoom*5;
	canv->TextOutW(m.L2Px(X)-canv->TextWidth(ID)/2.0,m.L2Py(Y+sJ/2.0)-canv->TextHeight(ID)/2.0,ID);//indexace pozice v rámci objektu
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////zajišťuje vykreslení simulace
//void Cvykresli::vykresli_simulaci(TCanvas *canv)
//{
//	//vykreslení linky
//	vykresli_linku(canv);
//
//	//vozíky
//	Cvektory::TVozik *ukaz1;
//	ukaz1=v.VOZIKY->dalsi;//přeskočí hlavičku
//	while (ukaz1!=NULL)
//	{
//			umisti_vozik(canv,ukaz1);
//			ukaz1=ukaz1->dalsi;//posun na další prvek
//	}
//
//	//vykreslení palce
//	Cvektory::TPalec *ukaz;
//	ukaz=v.PALCE->dalsi;//přeskočí hlavičku
//	while (ukaz!=NULL)
//	{
//			umisti_palec(canv,ukaz);
//			ukaz=ukaz->dalsi;//posun na další prvek
//	}
//
//	//vypis
//	canv->Brush->Color=clWhite;
//	double sec=cas*Form1->Timer_simulace->Interval/1000.0;
//	short h=floor(sec/3600);sec=sec-h*3600;
//	short m=floor(sec/60);sec=sec-m*60;
//	AnsiString H=h; if (h<10)H="0"+H;//pouze přidání 0 prefix
//	AnsiString M=m; if (m<10)M="0"+M;//pouze přidání 0 prefix
//	canv->Pen->Color=clGrayText;
//	canv->TextOutW(10,30,H+":"+M+":"+AnsiString(sec));
//
//	//ukončení simulace
//	//Form1->Timer_simulace->Enabled=false;
//	//sound();
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::vykresli_linku(TCanvas *canv)//zajišťuje vykreslení osy linky
//{
//		//nastavení šířky a barvy linie
//		canv->Pen->Color=(TColor)RGB(255,0,0);
//		canv->Pen->Width=1*Form1->Zoom;
//		canv->Pen->Mode=pmCopy;
//
//		//samotné vykreslení
//		Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//		while (ukaz!=NULL)
//		{
//			//počátek úsečky
//			canv->MoveTo(m.L2Px(ukaz->X)+O_width*Form1->Zoom/2,m.L2Py(ukaz->Y)+O_height*Form1->Zoom/2);
//
//			//konec úsečky
//			if(ukaz->dalsi!=NULL)
//			canv->LineTo(m.L2Px(ukaz->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(ukaz->dalsi->Y)+O_height*Form1->Zoom/2);
//			else//pro spojnici z posledního bodu do prvního
//			canv->LineTo(m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2,m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2);
//
//			//posun na další prvek v seznamu
//			ukaz=ukaz->dalsi;
//		}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::umisti_vozik(TCanvas *canv,Cvektory::TVozik *ukaz)//zajišťuje umístění vozíku na lince
//{
//	STOPKA(ukaz);
//	SG(ukaz);
//	if(ukaz->predchozi!=NULL)
//	if(KOLIZE(ukaz,ukaz->predchozi))Form1->Memo1->Lines->Add("KOLIZE!");
//
//
//	Cvektory::TObjekt *u_seg=ukaz->segment;//pouze zkrácení zápisu
//
//	if(ukaz->stav==1 && cas%10==0)ukaz->stav=2;//pokud vozík čeká na palec a palec přijde nastaví stav na zelenou
//	if(ukaz->stav==2)ukaz->pozice+=1;//pokud je povolen pohyb daného vozíku
//	double rest=0;
//
//	if(u_seg!=NULL)
//	{
//		double X1=m.L2Px(u_seg->X)+O_width*Form1->Zoom/2;
//		double Y1=m.L2Py(u_seg->Y)+O_height*Form1->Zoom/2;
//		double X2=0;double Y2=0;
//
//		if(u_seg->dalsi!=NULL)
//		{
//			X2=m.L2Px(u_seg->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(u_seg->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//		else//pro poslední linii
//		{
//			X2=m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//
//		double delka=m.delka(X1,Y1,X2,Y2);
//		double krok=1.0;//metrů optimálně by měl odpovídat 1px, aby nebylo znatelné problikávání při animaci
//
//		//smazání starého čtverec-vozíku
//		if(ukaz->pozice>-1)vykresli_vozik(canv,ukaz,m.round((ukaz->pozice-1)*Form1->Zoom*(X2-X1)/(delka/krok)+X1),m.round((ukaz->pozice-1)*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1),false);
//
//		//zajistit překreslení umazávaného gridu, toto přimo ještě nefunguje //vykresli_grid(canv);
//
//		//nakreslí nový čtverec-vozík
//		if(ukaz->pozice>-1)vykresli_vozik(canv,ukaz,m.round(ukaz->pozice*Form1->Zoom*(X2-X1)/(delka/krok)+X1),m.round(ukaz->pozice*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1),true);
//
//		//posun na další prvek v seznamu
//		if(krok*(ukaz->pozice+1)*Form1->Zoom>delka)
//		{
//			//výpis zbytkukrok*ukaz->pozice*Form1->Zoom
//			//canv->TextOutW(50,50,AnsiString(krok*ukaz->pozice*Form1->Zoom)+";"+AnsiString(delka));
//			if(ukaz->pozice>-1)vykresli_vozik(canv,ukaz,m.round(ukaz->pozice*Form1->Zoom*(X2-X1)/(delka/krok)+X1),m.round(ukaz->pozice*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1),false);//v případě přechodu na další linii smazání starého
//			rest=delka-(krok*(ukaz->pozice+1)*Form1->Zoom);//rest z minulé úsečky
//			ukaz->pozice=rest;ukaz->segment=u_seg->dalsi;
//		}
//	}
//	else//pokračování do dalšího kola
//	{
//		//pokračování do dalšího kola
//		ukaz->pozice=rest;//rest z minulého kola
//		ukaz->segment=v.OBJEKTY->dalsi;
//		umisti_vozik(canv,ukaz);//nutná rekurze zajišťující, aby se nepřišlo o jeden krok simulace
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::vykresli_vozik(TCanvas *canv,Cvektory::TVozik *ukaz,long X,long Y,bool NEW=true)//zajišťuje vykreslení vozíku při simulaci
//{
//	//záměrná záměna z důvodu špatně navrženého (o 90° orotovaného) výpočtu souřadnic vozíku
//	double delka=ukaz->sirka;
//	double sirka=ukaz->delka;
//
//	//nastavení stylu vykreslování
//	canv->Pen->Width=0;
//	canv->Pen->Mode=pmCopy;
//	if(NEW)
//	{
//		canv->Pen->Color=(TColor)RGB(0,0,0);
//		canv->Brush->Style=bsSolid;
//		canv->Brush->Color=ukaz->barva;
//	}
//	else
//	{
//		canv->Pen->Color=(TColor)RGB(255,255,255);
//		canv->Brush->Style=bsSolid;
//		canv->Brush->Color=clWhite;
//	}
//
//	//příprava souřadnic bodů polygonu dle rotace
//	double size=sqrt(pow(Form1->Zoom/Form1->m2px*delka/2,2)+pow(Form1->Zoom/Form1->m2px*sirka/2,2));
//	double A=0;
//	if(ukaz->segment->dalsi!=NULL)//mimo poslední spojnice
//		A=m.azimut(ukaz->segment->X,ukaz->segment->Y,ukaz->segment->dalsi->X,ukaz->segment->dalsi->Y);
//	else// pro poslední spojnici
//		A=m.azimut(ukaz->segment->X,ukaz->segment->Y,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y);
//	A+=ukaz->rotace;//přídání rotace vozíku k azimut segmentu
//	double U=m.uhel(X,Y,X-delka/2,Y-sirka/2);
//	POINT body[4]=
//	{
//		{m.round(X+m.rotace(1,360-U,A).x*size),m.round(Y+m.rotace(1,360-U,A).y*size)},
//		{m.round(X+m.rotace(1,180-(360-U),A).x*size),m.round(Y+m.rotace(1,180-(360-U),A).y*size)},
//		{m.round(X+m.rotace(1,180-U,A).x*size),m.round(Y+m.rotace(1,180-U,A).y*size)},
//		{m.round(X+m.rotace(1,U,A).x*size),m.round(Y+m.rotace(1,U,A).y*size)}
//	};
//	canv->Polygon((TPoint*)body,3);
//
//	//prozatim, potom budu ukladat celé vnější souřadnice
//	ukaz->X=m.P2Lx(X);ukaz->X=m.P2Ly(Y);
//
//	//provizorní vykreslení středu (pomocí uhlopříček)
//	canv->MoveTo(body[0].x,body[0].y);canv->LineTo(body[2].x,body[2].y);
//	canv->MoveTo(body[1].x,body[1].y);canv->LineTo(body[3].x,body[3].y);
//	if(ukaz->n==1)
//	{
//	canv->TextOutW(50,110,AnsiString(ukaz->pozice));
//	canv->TextOutW(50,130,AnsiString(v.PALCE->dalsi->pozice));
//	}
//
//	//provizorní výpis čísel rohu polygonu
//	/*canv->TextOutW(body[0].x,body[0].y,"0");
//	canv->TextOutW(body[1].x,body[1].y,"1");
//	canv->TextOutW(body[2].x,body[2].y,"2");
//	canv->TextOutW(body[3].x,body[3].y,"3");*/
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////přidá nový palec do seznamu PALCE s umístěním přímo na linku dle stanovené rozteče
//void Cvykresli::priprav_palce()
//{
//	double roztec=10.0;//metr;
//	double i=0.0;//krok pozice palců;
//	double delka=0.0;
//	Cvektory::TObjekt *ukaz=v.OBJEKTY->dalsi;//ukazatel na první objekt v seznamu OBJEKTU, přeskočí hlavičku
//	while (ukaz!=NULL)
//	{
//		if(ukaz->dalsi!=NULL)delka=m.delka(ukaz->X,ukaz->Y,ukaz->dalsi->X,ukaz->dalsi->Y);//mimo posledního segmentu
//		else delka=m.delka(ukaz->X,ukaz->Y,v.OBJEKTY->dalsi->X,v.OBJEKTY->dalsi->Y);// pro poslední segment
//		while(i<=delka)
//		{
//			v.vloz_palec();
//			v.PALCE->predchozi->pozice=i/Form1->m2px;
//			v.PALCE->predchozi->segment=ukaz;
//			i+=roztec;
//		}
//		//ShowMessage(AnsiString(delka)+" | "+AnsiString(i));
//		i-=delka;//rest zbytek chybějící do celé délky řetězu se přenese/zohlední do dalšího segmentu
//		//ShowMessage(AnsiString(i));
//		ukaz=ukaz->dalsi;//posun na další prvek v seznamu
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////zajišťuje aktuální umístění vozíku na lince vůči animaci
//void Cvykresli::umisti_palec(TCanvas *canv,Cvektory::TPalec *ukaz)
//{
//	double rest=0.0;
//  ukaz->pozice+=1.0;
//	//přesunoto z konce:ukaz->pozice+=1; //pozice na dílčí úsečce
//	Cvektory::TObjekt *u_seg=ukaz->segment;//pouze zkrácení zápisu
//	if(u_seg!=NULL)
//	{
//		double X1=m.L2Px(u_seg->X)+O_width*Form1->Zoom/2;
//		double Y1=m.L2Py(u_seg->Y)+O_height*Form1->Zoom/2;
//		double X2=0;double Y2=0;
//
//		if(u_seg->dalsi!=NULL)
//		{
//			X2=m.L2Px(u_seg->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(u_seg->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//		else//pro poslední linii
//		{
//			X2=m.L2Px(v.OBJEKTY->dalsi->X)+O_width*Form1->Zoom/2;
//			Y2=m.L2Py(v.OBJEKTY->dalsi->Y)+O_height*Form1->Zoom/2;
//		}
//
//		double delka=m.delka(X1,Y1,X2,Y2);
//		double krok=1.0;//metrů optimálně by měl odpovídat 1px, aby nebylo znatelné problikávání při animaci
//
//		//smazání starého palec
//		if(ukaz->pozice>-1)vykresli_palec(canv,(ukaz->pozice-1)*Form1->Zoom*(X2-X1)/(delka/krok)+X1,(ukaz->pozice-1)*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1,false);
//
//		//nakreslí nový palec
//		if(ukaz->pozice>-1)vykresli_palec(canv,ukaz->pozice*Form1->Zoom*(X2-X1)/(delka/krok)+X1,ukaz->pozice*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1,true);
//
//		//posun na další prvek v seznamu (na další zub
//		if(krok*(ukaz->pozice+1)*Form1->Zoom>delka)
//		{
//			if(ukaz->pozice>-1)
//			vykresli_palec(canv,(ukaz->pozice)*Form1->Zoom*(X2-X1)/(delka/krok)+X1,(ukaz->pozice)*Form1->Zoom*(Y2-Y1)/(delka/krok)+Y1,false);//v případě přechodu na další linii smazání starého
//			rest=delka-(krok*ukaz->pozice+1*Form1->Zoom);//rest z minulé úsečky
//			ukaz->pozice=rest;ukaz->segment=u_seg->dalsi;
//		}
//	}
//	else//pokračování do dalšího kola (na další segment
//	{
//		//pokračování do dalšího kola
//		ukaz->segment=v.OBJEKTY->dalsi;
//		ukaz->pozice=rest;//přiřadí restovou hodnotu z minulého kola
//		umisti_palec(canv,ukaz);//nutná rekurze zajišťující, aby se nepřišlo o jeden krok simulace
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//zajišťuje samotné vykreslení palce, parametr NEW rozlišuje nový palec a palace starý již ke smazání (to slouží pro simulaci), poslední parametr značí, zda palec označit jako aktivní
void Cvykresli::vykresli_palec(TCanvas *canv,double X,double Y,bool NEW,bool ACTIVE)
{                    //pokud se jedná o aktivní palec ještě přičte jedničku
	double size=1*Form1->Zoom;
	if(NEW)//nový palace
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		if(ACTIVE)canv->Brush->Color=clRed;
		else canv->Brush->Color=clBlack;
		canv->Pen->Width=1;
		canv->Ellipse(m.round(X-size),m.round(Y-size),m.round(X+size),m.round(Y+size));
	}
	else//tento smaže starý - slouží pro simulaci
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Pen->Color=clWhite;
		canv->Brush->Color=clWhite;
		canv->Pen->Width=1;
		canv->Ellipse(m.round(X-size),m.round(Y-size),m.round(X+size),m.round(Y+size));
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//celková vykreslovací metoda, vykreslí buď stopku, robota nebo otoč
void Cvykresli::vykresli_element(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,short stav,double rotace,bool kurzor)
{
	switch(typ)
	{
			case 0: vykresli_stopku(canv,X,Y,name,short_name,stav,rotace,kurzor);break;
			case 1: vykresli_robota(canv,X,Y,name,short_name,typ,stav,rotace,kurzor);break;
			case 2: vykresli_robota(canv,X,Y,name,short_name,typ,stav,rotace,kurzor);break;
			case 3: vykresli_robota(canv,X,Y,name,short_name,typ,stav,rotace,kurzor);break;
			case 4: vykresli_robota(canv,X,Y,name,short_name,typ,stav,rotace,kurzor);break;
			case 5: vykresli_otoc(canv,X,Y,name,short_name,typ,stav,rotace,kurzor);break;
			case 6: vykresli_otoc(canv,X,Y,name,short_name,typ,stav,rotace,kurzor);break;
			default: vykresli_stopku(canv,X,Y,name,short_name,stav,rotace,kurzor);break;
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_robota(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,short stav,double rotace,bool kurzor)
{
	double Z=F->Zoom;

	//vstupní parametry
	double rotace_ramene=0;
	double delka_ramena=12;

	//konstanty
	short sirka_zakladny=10;
	short delka_zakladny=12;
	short zaobleni=4;
	float tloustka_linie=1/3.0;
	TColor barva=clBlack;
	if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	//přidružené elementy
	if(typ==2)vykresli_stopku(canv,X,Y-sirka_zakladny/2*Z-delka_ramena*Z-1*2*Z,"","",stav,rotace,kurzor);//robot se stopkou
	if(typ==3)vykresli_otoc(canv,X,Y-sirka_zakladny/2*Z-delka_ramena*Z-1*2*Z,"","",5,stav,rotace,kurzor);//s pasivní otočí
	if(typ==4)vykresli_otoc(canv,X,Y-sirka_zakladny/2*Z-delka_ramena*Z-1*2*Z,"","",6,stav,rotace,kurzor);//s aktivní otočí (tj. s otočí a se stopkou)

	//nastavení pera
	if(kurzor)//stav kurzor
	{
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Color=barva;
		canv->Pen->Width=1;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Pen->Width=F->m.round(tloustka_linie*Z);
		canv->Brush->Style=bsSolid;
	}
	canv->Pen->Color=barva;
	canv->Brush->Color=clWhite;

	//základna
	TRect zakladna=TRect(X-delka_zakladny/2*Z,Y-sirka_zakladny/2*Z,X+delka_zakladny/2*Z,Y+sirka_zakladny/2*Z);
	canv->RoundRect(zakladna,zaobleni*Z,zaobleni*Z);

	//rameno
	canv->Rectangle(X-1*Z,Y-sirka_zakladny/2*Z,X+1*Z,Y-sirka_zakladny/2*Z-delka_ramena/2*Z);//první část
	canv->Rectangle(X-0.75*Z,Y-sirka_zakladny/2*Z-delka_ramena/2*Z,X+0.75*Z,Y-sirka_zakladny/2*Z-delka_ramena*Z);//druhá část
	canv->Brush->Style=bsClear;

	//triska
	canv->Ellipse(X-1*Z,Y-sirka_zakladny/2*Z-delka_ramena*Z,X+1*Z,Y-sirka_zakladny/2*Z-delka_ramena*Z-2*Z);//tryska

	//text
	if(!kurzor)//v módu kurzor se název nezobrazuje
	{
		canv->Font->Color=barva;
		canv->Font->Size=2*Z;
		canv->Font->Name="Arial";//canv->Font->Name="Courier New";//canv->Font->Name="MS Sans Serif";
		AnsiString T=short_name;if(Z>3)T=name;//od daného zoomu zobrazuje celý název
		//rotace_textu(canv,900);
		drawRectText(canv,zakladna,T);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_stopku(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short stav,double rotace,bool kurzor)
{
	double Z=F->Zoom;

	TColor barva=clRed;
	if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní
	//barva výplně
	if(kurzor)
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Width=1;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Color=clWhite;//bílý framing okolo stopky
		canv->Pen->Width=1*Z;
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Brush->Color=barva;
		canv->Brush->Style=bsSolid;
	}

	short size=7*Form1->Zoom;
	short sklon=45;

	//referenční bode ve špičce, špička je směrem dolu
	POINT body[3]={{X+m.rotace(1,sklon,rotace).x*size/2,Y+m.rotace(1,sklon,rotace).y*size},{X,Y},{X+m.rotace(1,360-sklon,rotace).x/2*size,Y+m.rotace(1,360-sklon,rotace).y*size}};
	canv->Polygon((TPoint*)body,2);

	//text
	if(!kurzor)//v módu kurzor se název nezobrazuje
	{
		canv->Font->Color=barva;
		canv->Font->Size=2*Z;
		canv->Font->Name="Arial";//canv->Font->Name="Courier New";//canv->Font->Name="MS Sans Serif";
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		AnsiString T=name;//short_name;if(Z>3)T=name;//od daného zoomu zobrazuje celý název
		rotace_textu(canv,rotace*10+900);
		canv->TextOutW(X-canv->TextHeight(T)/2,Y-size+1.5*Z,T);
		rotace_textu(canv,0);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
void Cvykresli::vykresli_otoc(TCanvas *canv,long X,long Y,AnsiString name,AnsiString short_name,short typ,short stav,double rotace,bool kurzor)
{
	double Z=F->Zoom;
	short size=m.round(3.5*Z);
	float width=0.8*Z;

	TColor barva=clBlack; if(typ==6)barva=clRed;if(stav==-1)barva=m.clIntensive(barva,180);//pokud je aktivní nebo neaktivní

	if(!kurzor)//v módu kurzor se název nezobrazuje
	{
		canv->Pen->Mode=pmCopy;
		canv->Pen->Style=psSolid;
		canv->Pen->Width=width;
		canv->Pen->Color=barva;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
	}
	else
	{
		canv->Pen->Color=clBlack;
		canv->Pen->Mode=pmNotXor;
		canv->Pen->Style=psDot;
		canv->Pen->Width=1;
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
	}

	//vykreslení objektu
	canv->Ellipse(X-size,Y-size,X+size,Y+size);
	if(!kurzor)//v módu kurzor se název nezobrazuje
	{
		canv->Pen->Color=clWhite;
		canv->Brush->Style=bsSolid;
		//přeříznutí pod šipkou
		canv->Rectangle(X-size,Y-width,X-size+1*Z,Y);//přežíznutí, možná lépe řešit obloukem
		canv->Rectangle(X+size-1*Z,Y,X+size,Y+width);//přežíznutí, možná lépe řešit obloukem
		//šipka
		sipka(canv,m.round(X-size),m.round(Y+width),rotace-25,false,width/Z,barva,barva);//děleno Z na negaci *Zoom v metodě šipka
		sipka(canv,m.round(X+size),m.round(Y-width),rotace-180-25,false,width/Z,barva,barva);//děleno Z na negaci *Zoom v metodě šipka
	}
	else
	{
		//šipka
		sipka(canv,X-size,Y+width,rotace-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z na negaci *Zoom v metodě šipka
		sipka(canv,X+size,Y-width,rotace-180-25,true,m.round(width/Z),clBlack,clWhite,pmNotXor,psDot);//děleno Z na negaci *Zoom v metodě šipka
	}

	//pokud je otoč aktivní tj. se stopkou
	if(typ==6)vykresli_stopku(canv,X,Y,"","",stav,rotace,kurzor);

	//text
	if(!kurzor)//v módu kurzor se název nezobrazuje
	{
		canv->Brush->Color=clWhite;
		canv->Brush->Style=bsClear;
		canv->Font->Color=barva;
		canv->Font->Size=2*Z;
		canv->Font->Name="Arial";//canv->Font->Name="Courier New";//canv->Font->Name="MS Sans Serif";
		AnsiString T=name;//short_name;if(Z>3)T=name;//od daného zoomu zobrazuje celý název
		rotace_textu(canv,rotace*10);
		canv->TextOutW(X+size+width+width,Y-canv->TextHeight(T)/2,T);
		rotace_textu(canv,0);
	}
}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::STOPKA(Cvektory::TVozik *ukaz)//zajištuje základní funkcionalitu technologického objektu v režimu S&G při vykreslování simulaci
//{
//	if(ukaz->segment!=NULL)
//	{
//		if(ukaz->segment->id==11)//stopka -- zatimí provizorně na objekt s ID 11
//		{
//		 if(ukaz->pozice<=0)ukaz->stav=ukaz->segment->stav;
//		}
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::SG(Cvektory::TVozik *ukaz)//zajištuje základní funkcionalitu technologického objektu STOPKA při vykreslování simulaci
//{
//	Cvektory::TObjekt *u_seg=ukaz->segment;//pouze zkrácení zápisu
//	if(u_seg!=NULL)
//	{
//		if(u_seg->rezim==0)//S&G
//		{
//			switch (u_seg->stav)
//			{
//				case 0:
//				{
//					if(ukaz->pozice<=0)//pouze pro nový příchozí prvek
//					{
//						ukaz->stav=0;
//						ukaz->timer=u_seg->CT*60;
//						u_seg->stav=1;//čekací mod
//					}
//					//Form1->Memo1->Lines->Add(ukaz->pozice);
//					break;
//				}
//				case 1:
//				{
//					if(--ukaz->timer==0)
//					{
//						u_seg->stav=2;//vozík může opustit objekt
//						ukaz->stav=1;//vozík bude čekat na palec
//					}
//				}
//				break;
//				case 2:
//				u_seg->stav=0;//objekt pozdrží další vozík
//				break;
//			}
//		}
//	}
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
//bool Cvykresli::KOLIZE(Cvektory::TVozik *V1,Cvektory::TVozik *V2)//vrací logickou hodnotu zda došlo či nedošlo ke kolizi s jiným vozíkem
//{  //provizorní algoritmus není dotažený
//	 if(m.delka(V1->X,V1->Y,V2->X,V2->Y) <= V1->delka+V2->delka)//zatím nedokonalá detekce konfliktu
//		return true;
//	 else
//		return false;
//}
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------------------------------------------------------------------------
//vykresluje měřítko
void Cvykresli::meritko(TCanvas *canv,long X,long Y)
{
	if(Form1->MOD!=Form1->TECHNOPROCESY)
	{
			//proměnné nastavení měřítka
			int L=Form1->scSplitView_LEFTTOOLBAR->Width+5;//umístění na X - levého výchozího kraje měřítka
			if(Form1->scSplitView_LEFTTOOLBAR->Visible==false)L=5;//pokud je levé menu skryto
			if(X>-1 && Y>-1)L=X;
			int T=Form1->scGPPanel_statusbar->Top-20;//umistění na Y - horního výchozího kraje měřítka
      if(Y>-1 && Y>-1)T=Y;
			int H=5;//výška měřítka
			int K=1;//krok v metrech
			if(Form1->Zoom==0.5)K=2;
			if(Form1->Zoom==0.25)K=5;
			int M=10;//MAX políček
			TColor barva_meritko=(TColor)RGB(128,128,128);//barva měřítka
			//TColor barva_meritko=(TColor)RGB(43,87,154);//(0,120,215);barva měřítka

			//nastavení pera a fontu canvasu
			canv->Pen->Color=barva_meritko;
			canv->Pen->Width=1;
			canv->Pen->Style=psSolid;
			canv->Brush->Style=bsSolid;
			canv->Pen->Mode=pmCopy;
			canv->Font->Size=8;
			canv->Font->Name="Arial";
			canv->Font->Color=barva_meritko;

			//popisek 0
			canv->MoveTo(L,T+5);canv->LineTo(L,T+7);//spojnice
			SetBkMode(canv->Handle,TRANSPARENT);//musí být zde znovu, nastavení transparentního pozadí
			canv->TextOutW(L-canv->TextWidth("0")/2+1,T+5,"0");

			//vykreslení políček měřítka
			int i=0;
			for(;i<M;i+=K)
			{
				if(i%(2*K))canv->Brush->Color=barva_meritko;//výplň barevna
				else canv->Brush->Color=clWhite;//výplň bílá                 //+1 pouze grafická korekce
				canv->Rectangle(m.L2Px(m.P2Lx(L)+i),T,m.L2Px(m.P2Lx(L)+(i+K))+1,T+H);
			}

			//musí být zde znovu, nastavení transparentního pozadí
			SetBkMode(canv->Handle,TRANSPARENT);
			//popisek polovina
			if(Form1->Zoom>=1)
			{
				canv->MoveTo(m.L2Px(m.P2Lx(L)+i/2),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i/2),T+7);
				canv->TextOutW(m.L2Px(m.P2Lx(L)+i/2)-canv->TextWidth(M/2)/2,T+5,AnsiString(M/2));
			}
			//popisek MAX
			canv->MoveTo(m.L2Px(m.P2Lx(L)+i),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i),T+7);
			canv->TextOutW(m.L2Px(m.P2Lx(L)+i)-canv->TextWidth(M)/2,T+5,AnsiString(M)+" m");
	}
	else //pro mod technologické procesy
	{
			//proměnné nastavení měřítka
			int L=Form1->scSplitView_OPTIONS->Width-50*2-15;//umístění na X - levého výchozího kraje měřítka
			if(Form1->scSplitView_LEFTTOOLBAR->Visible==false)L=Form1->ClientWidth-50*2-15;//pokud je levé menu skryto
			int T=Form1->scGPPanel_mainmenu->Height+5;//umistění na Y - horního výchozího kraje měřítka
			int H=5;//výška měřítka
			int K=1*5;//krok v metrech
			int M=2*5;//MAX políček
			TColor barva_meritko=(TColor)RGB(128,128,128);//barva měřítka
			//TColor barva_meritko=(TColor)RGB(43,87,154);//(0,120,215);barva měřítka

			//nastavení pera a fontu canvasu
			canv->Pen->Color=barva_meritko;
			canv->Pen->Width=1;
			canv->Pen->Style=psSolid;
			canv->Brush->Style=bsSolid;
			canv->Pen->Mode=pmCopy;
			canv->Font->Size=8;
			canv->Font->Name="Arial";
			canv->Font->Color=barva_meritko;

			//popisek 0
			canv->MoveTo(L,T+5);canv->LineTo(L,T+7);//spojnice
			SetBkMode(canv->Handle,TRANSPARENT);//musí být zde znovu, nastavení transparentního pozadí
			canv->TextOutW(L-canv->TextWidth("0")/2+1,T+5,"0");

			//vykreslení políček měřítka
			int i=0;
			for(;i<M;i+=K)
			{
				if(i%(2*K))canv->Brush->Color=barva_meritko;//výplň barevna
				else canv->Brush->Color=clWhite;//výplň bílá                 //+1 pouze grafická korekce
				canv->Rectangle(m.L2Px(m.P2Lx(L)+i),T,m.L2Px(m.P2Lx(L)+(i+K))+1,T+H);
			}

			//musí být zde znovu, nastavení transparentního pozadí
			SetBkMode(canv->Handle,TRANSPARENT);

			//popisek MAX
			canv->MoveTo(m.L2Px(m.P2Lx(L)+i),T+5);canv->LineTo(m.L2Px(m.P2Lx(L)+i),T+7);
			canv->TextOutW(m.L2Px(m.P2Lx(L)+i)-canv->TextWidth(M)/2,T+5,AnsiString(2)+" m");
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//vykreslí packu jako spojnici mezi komponentami
void Cvykresli::vykresli_packu(TCanvas *canv, int X1,int Y1,int X2,int Y2,TColor color,short Width,short OffsetX,short OffsetY)
{
	//nastavení pera
	canv->Pen->Width=Width;
	canv->Pen->Color=color;
	//vykreslení spojnice
	canv->MoveTo(X1,Y1);
	canv->LineTo(X1+OffsetX,Y1+OffsetY);
	if(OffsetX==0 && OffsetY!=0)canv->LineTo(X2,Y2+OffsetY);//horizontální packy
	else canv->LineTo(X1+OffsetX,Y2+OffsetY);//vertikální packy
	canv->LineTo(X2,Y2);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//void Cvykresli::vykresli_packy_PL(TCanvas *canv,short typ,short zamek_aRD,short zamek_R,short zamek_Rz,short zamek_Rx)
void Cvykresli::vykresli_packy_PL(TCanvas *canv,TscGPButton *zamek_aRD,TscGPButton *zamek_R,TscGPButton *zamek_Rz,TscGPButton *zamek_Rx)
{
	TColor Color=10114859;

	short O=-8;
	short Top=326;
	short aRD=636;
	short R	 =776;
	short Rz =917;
	short Rx =1059;

	//korelace aRD,Rz,Rx
	if(zamek_R->ImageIndex==37){vykresli_packu(canv,aRD,Top,Rz,Top,Color,1,0,O);vykresli_packu(canv,Rz,Top,Rx,Top,Color,1,0,O);}
	//korelace aRD,Rz,R
	if(zamek_Rx->ImageIndex==37){vykresli_packu(canv,aRD,Top,R,Top,Color,1,0,O);vykresli_packu(canv,R,Top,Rz,Top,Color,1,0,O);}
	//korelace R,Rx
	if(zamek_aRD->ImageIndex==37 || zamek_Rz->ImageIndex==37){vykresli_packu(canv,R,Top,Rx,Top,Color,1,0,O);}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------

//vytvoří zvuk
void Cvykresli::sound()
{
	Beep(400,250);	// 440 hertz (A4) for half a second
	Beep(500,250);	// 494 hertz for half a second
	Beep(600,250);	// 523 hertz for half a second
	Beep(700,250);	// 587 hertz for half a second
	Beep(800,500);	// 659 hertz for half a second
}
//------------------------------------------------------------------------------------------------------------------------------------------------------


