//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("popUP_menu.cpp", PopUPmenu);
USEFORM("report.cpp", Form_report);
USEFORM("superform.cpp", Form_definice_zakazek);
USEFORM("Z_rozliseni.cpp", Form_Z_rozliseni);
USEFORM("uvod.cpp", Form_uvod);
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit2.cpp", Form2);
USEFORM("UnitX.cpp", FormX);
USEFORM("katalog_dopravniku.cpp", Form_katalog);
USEFORM("konzole.cpp", Form_konzole);
USEFORM("help.cpp", Form_help);
USEFORM("adjustace.cpp", Form_adjustace);
USEFORM("MyMessageBox.cpp", myMessageBox);
USEFORM("parametry_linky.cpp", Form_parametry_linky);
USEFORM("minicolordialog.cpp", Form_color_dialog);
USEFORM("miniform_zpravy.cpp", Form_zpravy);
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
		Application->CreateForm(__classid(TForm_definice_zakazek), &Form_definice_zakazek);
		Application->CreateForm(__classid(TForm_parametry_linky), &Form_parametry_linky);
		Application->CreateForm(__classid(TPopUPmenu), &PopUPmenu);
		Application->CreateForm(__classid(TForm_report), &Form_report);
		Application->CreateForm(__classid(TForm2), &Form2);
		Application->CreateForm(__classid(TForm_adjustace), &Form_adjustace);
		Application->CreateForm(__classid(TForm_Z_rozliseni), &Form_Z_rozliseni);
		Application->CreateForm(__classid(TFormX), &FormX);
		Application->CreateForm(__classid(TFormX), &FormX);
		Application->CreateForm(__classid(TForm_katalog), &Form_katalog);
		Application->CreateForm(__classid(TForm_zpravy), &Form_zpravy);
		Application->CreateForm(__classid(TForm_color_dialog), &Form_color_dialog);
		Application->CreateForm(__classid(TForm_katalog), &Form_katalog);
		Application->CreateForm(__classid(TForm_help), &Form_help);
		Application->CreateForm(__classid(TForm_konzole), &Form_konzole);
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
