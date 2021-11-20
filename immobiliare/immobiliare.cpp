//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

//---------------------------------------------------------------------------










USEFORM("TFormFindAssets.cpp", FormFindAssets);
USEFORM("TFormFindCustomers.cpp", FormFindCustomers);
USEFORM("TFormDebug.cpp", FormDebug);
USEFORM("TFormAssets.cpp", FormAssets);
USEFORM("TFormCustomers.cpp", FormCustomers);
USEFORM("TFormMain.cpp", FormMain);
USEFORM("TFormQuerySupplyAndDemand.cpp", FormQuerySupplyAndDemand);
USEFORM("TFormQuerySalesHist.cpp", FormQuerySalesHist);
USEFORM("TFormQueries.cpp", FormQueries);
USEFORM("TFormQuerySales.cpp", FormQuerySales);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
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
