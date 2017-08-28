//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("parametry.cpp", Form_parametry);
USEFORM("superform.cpp", Form_vozik_nastaveni);
USEFORM("TT_kalkulator.cpp", Form_TT_kalkulator);
USEFORM("Unit1.cpp", Form1);
USEFORM("uvod.cpp", Form_uvod);
USEFORM("dopravniky.cpp", Form_dopravnik);
USEFORM("MyMessageBox.cpp", MyMessageBox);
USEFORM("cesty.cpp", Form_cesty);
USEFORM("jig.cpp", Form_jig);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TMyMessageBox), &MyMessageBox);
		Application->CreateForm(__classid(TForm_uvod), &Form_uvod);
		Application->CreateForm(__classid(TForm_parametry), &Form_parametry);
		Application->CreateForm(__classid(TForm_TT_kalkulator), &Form_TT_kalkulator);
		Application->CreateForm(__classid(TForm_dopravnik), &Form_dopravnik);
		Application->CreateForm(__classid(TForm_vozik_nastaveni), &Form_vozik_nastaveni);
		Application->CreateForm(__classid(TForm_cesty), &Form_cesty);
		Application->CreateForm(__classid(TForm_jig), &Form_jig);
		Application->ShowMainForm=false;//zabr�n�n� zobrazen� hlavn�ho formu
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
