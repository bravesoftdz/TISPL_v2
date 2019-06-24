const unsigned short pocet_objektu_knihovny=15;
struct Tknihovna_objektu{unsigned int id; UnicodeString short_name; UnicodeString name;};
Tknihovna_objektu const knihovna_objektu[15]=
{
	 {1 , "NAV", "navìšování"},
	 {2 , "CO2", "CO2"},
	 {3 , "OE", "oeh"},
	 {4 , "POW", "powerwash"},
	 {5 , "ION", "ionizace"} ,
	 {6 , "LAK", "lakování"},
	 {7 , "VY", "vytìkání"},
	 {8 , "SUŠ", "sušení"},
	 {9 , "CHL", "chlazení"},
	 {10, "SVÌ", "svìšování"},
	 {11, "BUF", "buffer"},
	 {12, "VİT", "vıtah"},
	 {13, "PRE", "pøejezd"},
	 {14, "V", "vıhybka"},
	 {15, "...", "nedefinovanı"},
};