unsigned short pocet_objektu_knihovny=12;
struct Tknihovna_objektu{unsigned int id; UnicodeString short_name; UnicodeString name;};
Tknihovna_objektu knihovna_objektu[12]=
{
	 {1 , "NAV", "nav�ov�n�"},
	 {2 , "CO2", "CO2"},
	 {3 , "ION", "ionizace"} ,
	 {4 , "LAK", "lakov�n�"},
	 {5 , "VY�", "vyt�k�n�"},
	 {6 , "SU�", "su�en�"},
	 {7 , "CHL", "chlazen�"},
	 {8 , "BUF", "buffer"},
	 {9 , "V�T", "v�tah"},
	 {10, "PRE", "p�ejezd"},
	 {11, "V ", "v�hybka"},
	 {12, "...", "nedefinovan�"},
};