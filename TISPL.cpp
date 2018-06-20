//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("report.cpp", Form_report);
USEFORM("superform.cpp", Form_definice_zakazek);
USEFORM("poznamky.cpp", Form_poznamky);
USEFORM("pl_priority.cpp", Form_PL_priority);
USEFORM("popUP_menu.cpp", PopUPmenu);
USEFORM("uvod.cpp", Form_uvod);
USEFORM("Unit1.cpp", Form1);
USEFORM("parametry_linky.cpp", Form_parametry_linky);
USEFORM("eDesigner.cpp", Form_eDesigner);
USEFORM("jig.cpp", Form_jig);
USEFORM("casovaOsa_info.cpp", Form_osa_info);
USEFORM("cesty.cpp", Form_cesty);
USEFORM("kabina_schema.cpp", Form_objekt_nahled);
USEFORM("MyMessageBox.cpp", myMessageBox);
USEFORM("parametry.cpp", Form_parametry);
USEFORM("kalendar.cpp", Form_kalendar);
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
		Application->CreateForm(__classid(TForm_eDesigner), &Form_eDesigner);
		Application->CreateForm(__classid(TForm_kalendar), &Form_kalendar);
		Application->CreateForm(__classid(TForm_osa_info), &Form_osa_info);
		Application->CreateForm(__classid(TForm_report), &Form_report);
		Application->CreateForm(__classid(TForm_PL_priority), &Form_PL_priority);
		Application->CreateForm(__classid(TForm_poznamky), &Form_poznamky);
		Application->CreateForm(__classid(TForm_objekt_nahled), &Form_objekt_nahled);
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
