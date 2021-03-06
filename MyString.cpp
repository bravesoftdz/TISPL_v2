﻿//---------------------------------------------------------------------------
#pragma hdrstop

#include "MyString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//parsování textu mezi zadanými texty, popř. možno využít offsetu  - ELEMENT PARSER
AnsiString TMyString::EP(AnsiString Text,AnsiString StartElement, AnsiString StopElement, unsigned short int startoffset, unsigned short int stopoffset)
{
  if(Text.Pos(StartElement)==0 || Text.Pos(StopElement)==0)
    return AnsiString("");
  else
  {
    Text=Text.Delete(1,Text.Pos(StartElement)+StartElement.Length()+startoffset-1);//smaže vše předchozí, kvůli duplicitám
    return Text.SubString(1,Text.Pos(StopElement)-stopoffset-1);
  }
}
UnicodeString TMyString::EP(UnicodeString Text,UnicodeString StartElement, UnicodeString StopElement, unsigned short int startoffset, unsigned short int stopoffset)
{
  if(Text.Pos(StartElement)==0 || Text.Pos(StopElement)==0)
    return UnicodeString("");
  else
  {
    Text=Text.Delete(1,Text.Pos(StartElement)+StartElement.Length()+startoffset-1);//smaže vše předchozí, kvůli duplicitám
    return Text.SubString(1,Text.Pos(StopElement)-stopoffset-1);
  }
}
//---------------------------------------------------------------------------
//parsování textu včetně ohraničujicích zadaných textů, popř. možno využít offsetu  - ELEMENT PARSER
AnsiString TMyString::EP2(AnsiString Text,AnsiString StartElement, AnsiString StopElement, unsigned short int startoffset, unsigned short int stopoffset)
{
  if(Text.Pos(StartElement)==0 || Text.Pos(StopElement)==0)
    return AnsiString("");
  else
  {
    Text=Text.Delete(1,Text.Pos(StartElement)+startoffset-1);//smaže vše předchozí, kvůli duplicitám
    return Text.SubString(1,Text.Pos(StopElement)+StopElement.Length()-1-stopoffset);
  }
}
//---------------------------------------------------------------------------
//změní první písmeno na velké
AnsiString TMyString::l2h(AnsiString l)
{
 return l.SubString(1,1).UpperCase()+l.SubString(2,l.Length()-1);
}
//---------------------------------------------------------------------------
//nahradí daný text
AnsiString TMyString::replace(AnsiString Text,AnsiString Insert,AnsiString Delete)
{
	if(Insert.Pos(Delete)==0)//pokud neobsahuje nahrazovaný text puvodní text napr. v případě pokud:  "*" za "*:", else vitev ochrana poed zacyklením, if vitev strojovi méni nároenijší
	{
		while(Text.Pos(Delete)!=0) //nefunguje v poípad pokud:  "*" za "*:" se zacyklí
		{
			Text.Insert(Insert,Text.Pos(Delete));
			Text.Delete(Text.Pos(Delete),Delete.Length());
		}
		return Text;
	}
	else
	{
		unsigned long Pocet=count(Text,Delete);//zjistí počet výskytů
		int *Pole=new int[Pocet];//ukládání indexů umístění
		unsigned long Pos;//aktuální pozice
		for(unsigned long i=1;i<=Pocet;i++)
		{
			Pos=Text.Pos(Delete);//najde znak který je na řadě
			Pole[i-1]=Pos;//uloží jeho pozici do pole
			Text.Delete(Pos,Delete.Length());//smaže nalezený znak
		}
		for(unsigned long i=1;i<=Pocet;i++)
		{
			Text.Insert(Insert,Pole[i-1]+(i-1)*Insert.Length());//+pousouvá o pozici o kterou nově zabírá nový řetězec
		}
		delete[] Pole;
		return Text;
	}
}
//---------------------------------------------------------------------------
//nahradí daný text
UnicodeString TMyString::replace(UnicodeString Text,UnicodeString Insert,UnicodeString Delete)
{
	if(Insert.Pos(Delete)==0)//pokud neobsahuje nahrazovaný text puvodní text napr. v případě pokud:  "*" za "*:", else vitev ochrana poed zacyklením, if vitev strojovi méni nároenijší
	{
		while(Text.Pos(Delete)!=0) //nefunguje v poípad pokud:  "*" za "*:" se zacyklí
		{
			Text.Insert(Insert,Text.Pos(Delete));
			Text.Delete(Text.Pos(Delete),Delete.Length());
		}
		return Text;
	}
	else
	{
		unsigned long Pocet=count(Text,Delete);//zjistí počet výskytů
		int *Pole=new int[Pocet];//ukládání indexů umístění
		unsigned long Pos;//aktuální pozice
		for(unsigned long i=1;i<=Pocet;i++)
		{
			Pos=Text.Pos(Delete);//najde znak který je na řadě
			Pole[i-1]=Pos;//uloží jeho pozici do pole
			Text.Delete(Pos,Delete.Length());//smaže nalezený znak
		}
		for(unsigned long i=1;i<=Pocet;i++)
		{
			Text.Insert(Insert,Pole[i-1]+(i-1)*Insert.Length());//+pousouvá o pozici o kterou nově zabírá nový řetězec
		}
		delete[] Pole;
		return Text;
	}
}
//---------------------------------------------------------------------------
//vráti počet výskytů daného řetězce
unsigned int TMyString::count(AnsiString Text,AnsiString Find)
{
  unsigned int count=0;
  while(Text.Pos(Find)!=0)
  {
    count++;
    Text.Delete(Text.Pos(Find),Find.Length());
  }
  return count;
}
unsigned int TMyString::count(UnicodeString Text,UnicodeString Find)
{
	unsigned int count=0;
	while(Text.Pos(Find)!=0)
	{
		count++;
		Text.Delete(Text.Pos(Find),Find.Length());
	}
	return count;
}
//---------------------------------------------------------------------------
//vrátí pozici posledního výskytu řetězce
unsigned int TMyString::lastPos(UnicodeString Text,UnicodeString Find)
{
	unsigned int Pos=0;
	while(Text.Pos(Find)!=0)
	{
		Pos+=Text.Pos(Find);
		Text.Delete(1,Text.Pos(Find));
	}
	return Pos;
}
//---------------------------------------------------------------------------
//smaže vše od počátku textu až po zadaný text X-krát dle zadání
AnsiString TMyString::delete_repeat(AnsiString Text, AnsiString Text_delete,unsigned int Repeat, unsigned short int Offset)
{
  for(unsigned int i=1;i<=Repeat;i++)
  {
    unsigned short int offset=0;
    if(i==Repeat)offset=Offset;
    Text.Delete(1,Text.Pos(Text_delete)+Text_delete.Length()-1-offset);
  }
  return Text;
}
UnicodeString TMyString::delete_repeat_UTF(UnicodeString Text, UnicodeString Text_delete,unsigned int Repeat, unsigned short int Offset)
{
  for(unsigned int i=1;i<=Repeat;i++)
  {
    unsigned short int offset=0;
    if(i==Repeat)offset=Offset;
    Text.Delete(1,Text.Pos(Text_delete)+Text_delete.Length()-1-offset);
  }
  return Text;
}
//---------------------------------------------------------------------------
//smaže všechen výskyt textu k vymazání (předcházející a ostatní text zanecha)
AnsiString TMyString::delete_repeat(AnsiString Text, AnsiString Text_delete)
{
  while(Text.Pos(Text_delete)!=0)
  {
		Text.Delete(Text.Pos(Text_delete),Text_delete.Length());
  }
  return Text;
}
//smaže všechen výskyt textu k vymazání (předcházející a ostatní text zanecha)
UnicodeString TMyString::delete_repeat(UnicodeString Text, UnicodeString Text_delete)
{
  while(Text.Pos(Text_delete)!=0)
  {
		Text.Delete(Text.Pos(Text_delete),Text_delete.Length());
  }
	return Text;
}
//---------------------------------------------------------------------------
//smaže vše od počátku textu až po všechny vyskyty znaku, včetně ostatního textu co předchází
AnsiString TMyString::delete_repeat_all(AnsiString Text, AnsiString Text_delete)
{
	while(Text.Pos(Text_delete)!=0)
	{
		Text.Delete(1,Text.Pos(Text_delete)+Text_delete.Length()-1);
	}
	return Text;
}
//---------------------------------------------------------------------------
UnicodeString TMyString::DeleteSpace(UnicodeString text)//vymaže mezeru
{
	while(text.Pos(" ")>0)
	{
		text=text.Delete(text.Pos(" "),1);
	}
	return text;
}
//---------------------------------------------------------------------------
//ožízne řetezec po znak, to co předchází znaku, vrátí, pozor řeší od prvního výskytu řetězce
AnsiString TMyString::TrimRightFrom(AnsiString Text, AnsiString Trim)
{
	unsigned int Pos=Text.Pos(Trim);
	if(Pos!=0)//pokud byla parsovací zarážka nalezena
	{                           //tady by mělo být asi -OD jako níže
		return Text.SubString(1,Pos-1);
	}
	else//nebyla nalezena, vrátí prázdný řetezec
	return "";
}
UnicodeString  TMyString::TrimRightFrom_UTF(UnicodeString  Text, UnicodeString Trim)
{
	unsigned int Pos=Text.Pos(Trim);
	if(Pos!=0)//pokud byla parsovací zarážka nalezena
	{                           //tady by mělo být asi -OD jako níže
		return Text.SubString(1,Pos-1);
	}
	else//nebyla nalezena, vrátí prázdný řetezec
	return "";
}
//---------------------------------------------------------------------------
//ožízne řetezec od znaku, to co následuje znaku, vrátí, pozor řeší od prvního výskytu řetězce
UnicodeString  TMyString::TrimLeftFrom_UTF(UnicodeString Text, UnicodeString Trim)
{
	unsigned int Pos=Text.Pos(Trim);
	if(Pos!=0)//pokud byla parsovací zarážka nalezena
	{                                  //tady by mělo být asi -OD jako níže
		return Text.SubString(Pos+1,Text.Length());
	}
	else//nebyla nalezena, vrátí prázdný řetezec
	return "";
}
//---------------------------------------------------------------------------
//ožízne řetezec od textu, to co následuje textu, vrátí , pozor řeší od prvního výskytu řetězce
UnicodeString TMyString::TrimLeftFromText(UnicodeString Text, UnicodeString Trim)
{
	unsigned int Pos=Text.Pos(Trim);
	if(Pos!=0)//pokud byla parsovací zarážka nalezena
	{
		short OD=Pos+Trim.Length();
		return Text.SubString(OD,Text.Length()-OD+1);
	}
	else//nebyla nalezena, vrátí prázdný řetezec
	return "";
}
//---------------------------------------------------------------------------
//převede text v kódování WINDOWS-1250 na UTF-8
AnsiString TMyString::Win2UTF(AnsiString Text)
{
  /*WideString WData=Text;
  char *CHData=new char[Text.Length()+4];
  WideCharToMultiByte(CP_UTF8,0,WData,Text.Length()+4,CHData,Text.Length()+4,NULL, NULL );
  AnsiString t=CHData;
  return t;
  */
  return AnsiToUtf8(Text);
}
//---------------------------------------------------------------------------
//převede text v kódování UTF-8 na WINDOWS-1250
AnsiString TMyString::UTF2Win(AnsiString Text)
{
  WCHAR *WData=new WCHAR[Text.Length()];
  MultiByteToWideChar(CP_UTF8,0,Text.c_str(),Text.Length(),WData,Text.Length());
  AnsiString DATA=WData;
  delete WData;
  return DATA;
}
//---------------------------------------------------------------------------
//kod pro remove_abc
//AnsiString PZ=mGrid->Cells[4][2].Text.SubString(mGrid->Cells[4][2].Text.Length(),1);//vrátí poslední znak
//if((int)*PZ.c_str()<48 || (int)*PZ.c_str()>57)mGrid->Cells[4][2].Text.Delete(mGrid->Cells[4][2].Text.Length(),1);//ubere poslední znak
//odstraní českou diakritiku a vrátí stejný (zadaný) řetěze bez diakritiky)
AnsiString TMyString::remove_diacritics(AnsiString text)
{
	for(long i=1;i<=text.Length();i++)
	{
		switch((int)*text.SubString(i,1).c_str())
		{
			case -20:	text.Delete(i,1);text.Insert("e",i);break;//ě
			case -102:	text.Delete(i,1);text.Insert("s",i);break;//š
			case -24:	text.Delete(i,1);text.Insert("c",i);break;//č
			case -8:		text.Delete(i,1);text.Insert("r",i);break;//ř
			case	-98:	text.Delete(i,1);text.Insert("z",i);break;//ž
			case -3:		text.Delete(i,1);text.Insert("y",i);break;//ý
			case -31:	text.Delete(i,1);text.Insert("a",i);break;//á
			case -19:	text.Delete(i,1);text.Insert("i",i);break;//í
			case -23:	text.Delete(i,1);text.Insert("e",i);break;//é
			case -6:		text.Delete(i,1);text.Insert("u",i);break;//ú
			case -7:		text.Delete(i,1);text.Insert("u",i);break;//ů
			case -14:	text.Delete(i,1);text.Insert("n",i);break;//ň
			case -52:	text.Delete(i,1);text.Insert("E",i);break;//Ě
			case -118:	text.Delete(i,1);text.Insert("S",i);break;//Š
			case -56:	text.Delete(i,1);text.Insert("C",i);break;//Č
			case -40:	text.Delete(i,1);text.Insert("R",i);break;//Ř
			case -114:	text.Delete(i,1);text.Insert("Z",i);break;//Ž
			case -35:	text.Delete(i,1);text.Insert("Y",i);break;//Ý
			case -63:	text.Delete(i,1);text.Insert("A",i);break;//Á
			case -51:	text.Delete(i,1);text.Insert("I",i);break;//Í
			case -55:	text.Delete(i,1);text.Insert("E",i);break;//É
			case -38:	text.Delete(i,1);text.Insert("U",i);break;//Ú
			case -39:	text.Delete(i,1);text.Insert("U",i);break;//Ů
			case -46:	text.Delete(i,1);text.Insert("N",i);break;//Ň
			case -99:	text.Delete(i,1);text.Insert("t",i);break;//ť
			case -17:	text.Delete(i,1);text.Insert("d",i);break;//ď
			case -13:	text.Delete(i,1);text.Insert("o",i);break;//ó
			case -115:	text.Delete(i,1);text.Insert("T",i);break;//Ť
			case -49:	text.Delete(i,1);text.Insert("D",i);break;//Ď
			case -45:	text.Delete(i,1);text.Insert("O",i);break;//Ó
		}
	}
	return text;
}
//---------------------------------------------------------------------------
//převede text do double datového typu, řeší nastavení desetinná tečka vs. čárka
double TMyString::MyToDouble(UnicodeString Text)
{
	double number=0.0;
  if(Text!="")//pokud přijde prázdný řetězec přehodí na nulu
	{
    try{number=Text.ToDouble();}//testování výjimky
		catch(...)//zachycení výjimky
		{
			//try{number=replace(Text,",",".").ToDouble();}
			try{number=replace(Text,get_locale_decimal(),".").ToDouble();}//podle oddělovače na daném systému, nahrazeno za konkretní čárku
			catch(...){;}//vráti nulu pokud se ani po změně oddělovače byl řetězec neplatný
		}
  }
	return number;
}
//---------------------------------------------------------------------------
//prevod z carky na tecku v desetiném miste
AnsiString TMyString::c2t(AnsiString Vstup)
{
  int Pozice=Vstup.Pos(",");
  if(Pozice>0)
  {
    Vstup.Delete(Pozice,1);
    Vstup.Insert(".",Pozice);
    if(Vstup.Pos(",")>0)Vstup=c2t(Vstup);//rekurze
  }
  return Vstup;
}
//---------------------------------------------------------------------------
//prevod z tecky na carku v desetiném miste
AnsiString TMyString::t2c(AnsiString Vstup)
{
  int Pozice=Vstup.Pos(".");
  if(Pozice>0)
  {
    Vstup.Delete(Pozice,1);
    Vstup.Insert(",",Pozice);
    if(Vstup.Pos(",")>0)Vstup=t2c(Vstup);//rekurze
  }
  return Vstup;
}
//---------------------------------------------------------------------------
//převod AnsiStringu na integer, řeší, když příjde prázdný řetězec, nebo když se nejedná o číslo
long TMyString::a2i(AnsiString Vstup)
{
  try
  {
	if(Vstup=="")return 0;
	else return Vstup.ToInt();
  }
  catch(...)
  {
	return 0;
  }
}
//převod Unicode na integer, řeší, když příjde prázdný řetězec, nebo když se nejedná o číslo
long TMyString::u2i(UnicodeString Vstup)
{
  try
  {
	if(Vstup=="")return 0;
	else return Vstup.ToInt();
  }
  catch(...)
  {
	return 0;
  }
}
//není přesně do long longu ale alespoň větší než longu, viz lenost
long long TMyString::a2ll(AnsiString Vstup)
{
  try
  {
	if(Vstup=="")return 0;
	else return (long long) Vstup.ToDouble();
  }
  catch(...)
  {
	return 0;
  }
}
//---------------------------------------------------------------------------
//vrátí oddělovač desetinného místa na daném systému
UnicodeString TMyString::get_locale_decimal()
{
 char text[5];
 GetLocaleInfoA(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,text,sizeof(text));
 return UnicodeString(text);
}
//---------------------------------------------------------------------------
//vrátí reálné číslo s oddělovač desetinného místa na daném systému
//určeno pro výstupy ze systému
UnicodeString TMyString::get_locale_double(double number)
{
	UnicodeString oddelovac=get_locale_decimal();
	if(UnicodeString(number).Pos(oddelovac)>0)//oddělovač byl nastaven správne
		return UnicodeString(number);
	else
	{
	   if(UnicodeString(number).Pos(".")>0)
		return replace(UnicodeString(number),oddelovac,".");
	   else
	   	return replace(UnicodeString(number),oddelovac,",");
	}
}
//---------------------------------------------------------------------------
//převede číslo z hexagesimálního tvaru na dekadický
/*
LAT,LONG
49.8703442,16.3115764 níže uvedené převádí do tohoto a navíc dodrží na výstupu pořadí lat,long  pokud je zadané
49.8703442 16.3115764
49.8703442, 16.3115764
49.8703442º N, 16.3115764º Ø//zadává kart.finn.no
49.8703442°N,16.3115764°E
49.8703442°N 16.3115764°E
49.8703442°N, 16.3115764°E
49°52.22065'N,16°18.69458'E
49°52.22065'N 16°18.69458'E
49°52.22065'N, 16°18.69458'E
49°52'13.239"N,16°18'41.675"E
49°52'13.239"N 16°18'41.675"E
49°52'13.239"N, 16°18'41.675"E
*//*
TPointD TMyString::HexGes2Dec(UnicodeString Loc)
{
	Loc=Utf8ToAnsi(Loc);//v případě kart.finn.no
	if(count(Loc," ")>1)Loc=delete_repeat(Loc," ");//pokud je více mezer, tak vše odstraní
	Loc=replace(Loc,"E","O");Loc=replace(Loc,"°","o");//kvůli kart.finn.no
	//sjednocení formátu na variantu s oddělovačem čárkou a bez mezery
	if(Loc.Pos(" ") && Loc.Pos(","))Loc=Loc.Delete(Loc.Pos(" "),1);//pokud obsahuje jak mezeru, tak čárku, tak nechá jenom čárku
	if(Loc.Pos(" ") && !Loc.Pos(","))Loc=replace(Loc,","," ");//pokud je obsažena pouze mezera, tak ji nahradí čárkou

	if(!Loc.Pos("°")) //pokud se jedná o decimální tvar
	{
		 TPointD LongLat;
		 LongLat.y=MyToDouble(Loc.SubString(1,Loc.Pos(",")-1));
		 LongLat.x=MyToDouble(Loc.SubString(Loc.Pos(",")+1,Loc.Length()-Loc.Pos(",")));
		 return LongLat;
  }
	else//pokud se jedná o HEX tvar
	{
			TPointD LongLat_pom;
			bool sirka_prvni;
			double znamenko1=1.0;
			double znamenko2=1.0;

			//pokud je šířka první údaj
			if(Loc.Pos("N,") || Loc.Pos("S,"))sirka_prvni=true;else sirka_prvni=false;//pokud je délka první údaj

			//určení znaménka
			UnicodeString s1=TrimRightFrom_UTF(Loc,",");if(s1.SubString(s1.Length(),1)=="S" || s1.SubString(s1.Length(),1)=="W")znamenko1-1.0;else znamenko1=1.0;
			UnicodeString s2=TrimLeftFrom_UTF(Loc,",");if(s2.SubString(s2.Length(),1)=="S" || s2.SubString(s2.Length(),1)=="W")znamenko2-1.0;else znamenko2=1.0;

			s1=s1.Delete(s1.Length(),1); s2=s2.Delete(s2.Length(),1);//odebere poslední indikátor N,S,E,W

			if(s1.Pos("°") &&  s1.Pos("\'") &&  s1.Pos("\""))//49°52'13.239"N,16°18'41.675"E
			{
				LongLat_pom.y=MyToDouble(TrimRightFrom_UTF(s1,"°"))+(MyToDouble(EP(s1,"°","\'"))/60.0)+(MyToDouble(EP(s1,"\'","\""))/3600.0);
				LongLat_pom.x=MyToDouble(TrimRightFrom_UTF(s2,"°"))+(MyToDouble(EP(s2,"°","\'"))/60.0)+(MyToDouble(EP(s2,"\'","\""))/3600.0);
			}
			else
			{
				if(s1.Pos("\'") && !s1.Pos("\""))//49°52.22065'N,16°18.69458'E
				{
					s1=s1.Delete(s1.Length(),1); s2=s2.Delete(s2.Length(),1);//znovu odebere poslední indikátor minuty
					LongLat_pom.y=MyToDouble(TrimRightFrom_UTF(s1,"°"))+(MyToDouble(TrimLeftFrom_UTF(s1,"°"))/60.0);
					LongLat_pom.x=MyToDouble(TrimRightFrom_UTF(s2,"°"))+(MyToDouble(TrimLeftFrom_UTF(s2,"°"))/60.0);
				}
				else
				if(s1.Pos("°") &&  !s1.Pos("\'") && !s1.Pos("\""))//49.8703442°N,16.3115764°E
				{
					LongLat_pom.y=MyToDouble(s1.Delete(s1.Length(),1));//odebere indikátor stupnu a prevede na cislo
					LongLat_pom.x=MyToDouble(s2.Delete(s2.Length(),1));//odebere indikátor stupnu a prevede na cislo
				}
				}

			//určerní pořadí a znaménka na výstupu
			TPointD LongLat;
			if(sirka_prvni)
			{
				LongLat.y=LongLat_pom.y*znamenko1;LongLat.x=LongLat_pom.x*znamenko2;
			}
			else
			{
				LongLat.y=LongLat_pom.x*znamenko2;LongLat.x=LongLat_pom.y*znamenko1;
			}
			return LongLat;
	}
}  */
//---------------------------------------------------------------------------
//vrátí reálnou část čísla
double TMyString::get_decimal(double number)
{
	try
	{
		double N=TrimLeftFrom_UTF(number,get_locale_decimal()).ToDouble();
		if(N==0)return 0;
		return N;
	}
	catch(...)
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
//vrátí počet desetinných míst
unsigned short TMyString::get_count_decimal(double number)
{
	double N=get_decimal(number);
	if(N==0) return 0;
	return AnsiString(N).Length();
}
//---------------------------------------------------------------------------
//přídá nuly na urovní decimální části do počtu dle precision např. 3,23 -> 3,230, 5 -> 5,000
AnsiString TMyString::addDecimal(double number,unsigned short precision)
{
	AnsiString RET=number;
	unsigned short count_decimal=get_count_decimal(number);
	for(unsigned short i=0;count_decimal+i<precision;i++)
	{
		if(count_decimal==0 && i==0)RET+=get_locale_decimal();//pokud je reálná část nulová, přida ještě oddělovač desetinného místa (čárku či tečku), dle zvoleného systomvého nastavení
		RET+="0";
	}

	return RET;
}
//---------------------------------------------------------------------------
//ověří, že se jedná o číslo
bool TMyString::IsNumber(UnicodeString Text)
{
	if(UnicodeString(MyToDouble(Text))==Text)return true;//po převodu do čísla se jedná stále o stejnou hodnotu
	else return false;
}
//---------------------------------------------------------------------------
//metoda volatelná v onkeypress dané komponenty, dovolí pouze reálné či celé číslo, separátor desetinného místa dle lokálního nastavení, ošetřuje i, aby separátor byl v textu obsažen pouze jeden a aby nebyl na prvním místě, pokud nastane chybně zadaný znak, může systemově zazvonit - poslední parametr, implicitně na true
System::WideChar TMyString::numericFilter(AnsiString aktText,System::WideChar &Key,bool ErrorMessageBeep,short typ)
{
	//filtr kláves
	AnsiString Separator=get_locale_decimal();//zjištění oddělovače v aktuálním systému
	AnsiString Mark;
	if(typ==1)Mark="-";else Mark=Separator;
	if(ErrorMessageBeep && ((long)Key==26 || Key==0x19 || Key==0x16 || Key==0x3))ErrorMessageBeep=false;//vypnutí zvuku pro špatnou klávesu v případě klácesových zkratek
	if(Key!=VK_BACK)//pokud není stisknuta klávesa backspace, nelze vepsat jinou hodnotu než číselnou (to včetně reálného čísla)
	{
	 if(!((Key>=L'0') && (Key<=L'9') || (AnsiString(Key) == Separator) || (AnsiString(Key) == Mark)))Key=0;//pokud se nejedná o číslo nebo oddělovač
	 if(AnsiString(Key) == Separator && aktText.Length()      == 0)Key=0;//oddělovač nemůže být na prvním místě
	 if(AnsiString(Key) == Mark && Mark == "-" && aktText.Pos(Mark)      > 0)Key=0;//znaménko může být jen jedno
	 if(AnsiString(Key) == Separator && aktText.Pos(Separator) > 0)Key=0;//oddělovačů nemůže být více
	 if(Key == L'0' && aktText.Pos("0") > 0 && aktText.Length() == 1)Key=0;//nelze napsat na druhou pozici v čísle 0

	 if(Key==0 && ErrorMessageBeep)MessageBeep(0);//zvuková inicializace špatné klávesy pokud je požadována
	}
	return Key;
}
//---------------------------------------------------------------------------

