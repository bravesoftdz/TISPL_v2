const unsigned short pocet_objektu_knihovny=15;
struct Tknihovna_objektu{unsigned int id; UnicodeString short_name; UnicodeString name;};
Tknihovna_objektu const knihovna_objektu[15]=
{
	 {1 , "NAV", "nav�ov�n�"},
	 {2 , "CO2", "CO2"},
	 {3 , "O�E", "o�eh"},
	 {4 , "POW", "powerwash"},
	 {5 , "ION", "ionizace"} ,
	 {6 , "LAK", "lakov�n�"},
	 {7 , "VY�", "vyt�k�n�"},
	 {8 , "SU�", "su�en�"},
	 {9 , "CHL", "chlazen�"},
	 {10, "SV�", "sv�ov�n�"},
	 {11, "BUF", "buffer"},
	 {12, "V�T", "v�tah"},
	 {13, "PRE", "p�ejezd"},
	 {14, "V", "v�hybka"},
	 {15, "...", "nedefinovan�"},
};