//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("popUP_menu.cpp", PopUPmenu);
USEFORM("superform.cpp", Form_definice_zakazek);
USEFORM("parametry_linky.cpp", Form_parametry_linky);
USEFORM("parametry.cpp", Form_parametry);
USEFORM("uvod.cpp", Form_uvod);
USEFORM("Unit1.cpp", Form1);
USEFORM("eDesigner.cpp", Form_eDesigner);
USEFORM("cesty.cpp", Form_cesty);
USEFORM("jig.cpp", Form_jig);
USEFORM("MyMessageBox.cpp", myMessageBox);
USEFORM("kalendar.cpp", Form_kalendar);
USEFORM("casovaOsa_info.cpp", Form_osa_info);
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
		Application->ShowMainForm=false;//zabránění zobrazení hlavního formu
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
