//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("superform.cpp", Form_definice_zakazek);
USEFORM("parametry_linky.cpp", Form_parametry_linky);
USEFORM("popUP_menu.cpp", PopUPmenu);
USEFORM("Unit1.cpp", Form1);
USEFORM("uvod.cpp", Form_uvod);
USEFORM("parametry.cpp", Form_parametry);
USEFORM("jig.cpp", Form_jig);
USEFORM("cesty.cpp", Form_cesty);
USEFORM("MyMessageBox.cpp", myMessageBox);
USEFORM("qDesign.cpp", Form2);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TmyMessageBox), &myMessageBox);
		Application->CreateForm(__classid(TForm_uvod), &Form_uvod);
		Application->CreateForm(__classid(TForm_parametry), &Form_parametry);
		Application->CreateForm(__classid(TForm_definice_zakazek), &Form_definice_zakazek);
		Application->CreateForm(__classid(TForm_jig), &Form_jig);
		Application->CreateForm(__classid(TForm_cesty), &Form_cesty);
		Application->CreateForm(__classid(TForm_parametry_linky), &Form_parametry_linky);
		Application->CreateForm(__classid(TPopUPmenu), &PopUPmenu);
		Application->CreateForm(__classid(TForm2), &Form2);
		Application->ShowMainForm=false;//zabránìní zobrazení hlavního formu
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
