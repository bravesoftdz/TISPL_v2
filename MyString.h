//---------------------------------------------------------------------------
#ifndef MyStringH
#define MyStringH
#include <vcl.h>
//#include "my.h"
//---------------------------------------------------------------------------
//struktury
//struktury
class TMyString
{
private:

public:
  AnsiString EP(AnsiString Text,AnsiString StartElement, AnsiString StopElement, unsigned short int startoffset=0, unsigned short int stopoffset=0);//parsov�n� textu mezi zadan�mi texty, pop�. mo�no vyu��t offsetu  - ELEMENT PARSER
  UnicodeString TMyString::EP(UnicodeString Text,UnicodeString StartElement, UnicodeString StopElement, unsigned short int startoffset, unsigned short int stopoffset);
  AnsiString EP2(AnsiString Text,AnsiString StartElement, AnsiString StopElement, unsigned short int startoffset=0, unsigned short int stopoffset=0);//parsov�n� textu mezi zadan�mi texty, pop�. mo�no vyu��t offsetu  - ELEMENT PARSER
	AnsiString replace(AnsiString Text,AnsiString Insert,AnsiString Delete);//nahrad� dan� text
	UnicodeString replace(UnicodeString Text,UnicodeString Insert,UnicodeString Delete);//nahrad� dan� text
	unsigned int count(AnsiString Text,AnsiString Find);//vr�ti po�et v�skyt� dan�ho �et�zce
	unsigned int count(UnicodeString Text,UnicodeString Find);//vr�ti po�et v�skyt� dan�ho �et�zce
	unsigned int lastPos(UnicodeString Text,UnicodeString Find);//vr�t� pozici posledn�ho v�skytu �et�zce
  AnsiString l2h(AnsiString l);//zm�n� prvn� p�smeno na velk�
  AnsiString delete_repeat(AnsiString Text, AnsiString Text_delete,unsigned int Repeat, unsigned short int Offset=0);//sma�e v�e po zadan� text X-kr�t
  UnicodeString delete_repeat_UTF(UnicodeString Text, UnicodeString Text_delete,unsigned int Repeat, unsigned short int Offset=0);
  AnsiString delete_repeat(AnsiString Text, AnsiString Text_delete);
  UnicodeString delete_repeat(UnicodeString Text, UnicodeString Text_delete);
	AnsiString delete_repeat_all(AnsiString Text, AnsiString Text_delete);
	UnicodeString DeleteSpace(UnicodeString text);
  AnsiString TrimRightFrom(AnsiString Text,AnsiString Trim);//o��zne �etezec po znak, to co p�edch�z� znaku, vr�t�
	UnicodeString TrimRightFrom_UTF(UnicodeString Text,UnicodeString Trim);//o��zne �etezec po znak, to co p�edch�z� znaku, vr�t�
	UnicodeString TrimLeftFrom_UTF(UnicodeString  Text, UnicodeString Trim);//o��zne �etezec po znaku, to co n�sleduje znaku, vr�t�
	UnicodeString TrimLeftFromText(UnicodeString  Text, UnicodeString Trim);//o��zne �etezec od textu, to co n�sleduje textu, vr�t�
	AnsiString Win2UTF(AnsiString Text);//p�evede text v k�dov�n� WINDOWS-1250 na UTF-8
	AnsiString UTF2Win(AnsiString Text);//p�evede text v k�dov�n� UTF-8 na WINDOWS-1250
	AnsiString remove_diacritics(AnsiString text);//odstran� �eskou diakritiku a vr�t� stejn� (zadan�) �et�ze bez diakritiky)
	double MyToDouble (UnicodeString Text);//p�evede UnicodeString na Double, v p��pad� ��rky p�evede na te�ku
	AnsiString c2t(AnsiString Vstup);//desetinn� ��rka na desetinnou te�ku
	AnsiString t2c(AnsiString Vstup);//desetinn� te�ka na desetinnou ��rku
	long a2i(AnsiString Vstup);//AnsiString do Int
	long u2i(UnicodeString Vstup);//UnicodeString do Int
	long long a2ll(AnsiString Vstup);//AnsiString do long long
	UnicodeString get_locale_decimal();//vr�t� odd�lova� desetinn�ho m�sta na dan�m syst�mu
	UnicodeString get_locale_double(double number);//vr�t� re�ln� ��slo s odd�lova� desetinn�ho m�sta na dan�m syst�mu
	double get_decimal(double number);//vr�t� re�lnou ��st ��sla
	unsigned short get_count_decimal(double number);//vr�t� po�et desetinn�ch m�st
	AnsiString addDecimal(double number,unsigned short precision);//p��d� nuly na urovn� decim�ln� ��sti do po�tu dle precision nap�. 3,23 -> 3,230, 5 -> 5,000
	bool IsNumber(UnicodeString Text);//ov���, �e se jedn� o ��slo
	//TPointD HexGes2Dec(UnicodeString Loc);//p�evede ��slo z hexagesim�ln�ho tvaru na dekadick�
	System::WideChar numericFilter(AnsiString aktText,System::WideChar &Key,bool ErrorMessageBeep=true,short typ=2);//metoda volateln� v onkeypress dan� komponenty, dovol� pouze re�ln� �i cel� ��slo, separ�tor desetinn�ho m�sta dle lok�ln�ho nastaven�, o�et�uje i, aby separ�tor byl v textu obsa�en pouze jeden a aby nebyl na prvn�m m�st�, pokud nastane chybn� zadan� znak, m��e systemov� zazvonit - posledn� parametr, implicitn� na true, parametr typ ur�uje typ filtru: 1 = re�ln� ��sla, 2 = kladn� re�ln� ��sla, implicitn� = 2
protected:
};
//---------------------------------------------------------------------------
#endif
