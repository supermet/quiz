//---------------------------------------------------------------------------
#include "functions.h"
//------------------
CRITICAL_SECTION cs;
bool plugin_started=false;
//wstring AddBotName;
ofstream FError;
extern byte SuperRunnerIcon;
extern string SuperRunner;

extern CppSQLite3DB * sqlite;

extern int PROGRAMM_TYPE;

extern bool gameStarted;
extern bool NextQuestionIsSuper;
extern wstring GameChannel;
extern wstring BOTNAME;

string VERSION = "1.019.2";

//----------------------


DWORD dwPluginID;

typedef VOID (__stdcall *typeCommFortProcess)(DWORD dwPluginID, DWORD dID, BYTE * bOutBuffer, DWORD dwOutBufferSize);
typedef DWORD (__stdcall *typeCommFortGetData)(DWORD dwPluginID, DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize);

typeCommFortProcess CommFortProcess;
typeCommFortGetData CommFortGetData;



extern "C" {
    __declspec(dllexport) BOOL __stdcall PluginStart(DWORD dwThisPluginID, typeCommFortProcess, typeCommFortGetData);
    __declspec(dllexport) VOID __stdcall PluginStop();

    __declspec(dllexport) VOID __stdcall PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize);
    __declspec(dllexport) DWORD __stdcall PluginGetData(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize);

    __declspec(dllexport) VOID __stdcall PluginShowOptions();
    __declspec(dllexport) VOID __stdcall PluginShowAbout();
}

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    return 1;
}
//---------------------------------------------------------------------------

string AskPluginTempPath()
{
    int iSize = (*CommFortGetData)(dwPluginID, 2010, NULL, NULL, NULL, NULL); //получаем объем буфера
    BYTE * cData = new BYTE[iSize];
    (*CommFortGetData)(dwPluginID, 2010,cData,iSize, NULL, NULL);//заполняем буфер

    int iReadOffset = 0;
    std::wstring wpath = fReadText(cData, &iReadOffset);
    std::string path = wstostr(wpath, CP_ACP);
    delete [] cData;
    return path;
    //return "c:\\ProgramData\\CommFort_server\\PluginsData\\";
}



int AskProgrammType()
{
    int iSize = (*CommFortGetData)(dwPluginID, 2000, NULL, NULL, NULL, NULL); //получаем объем буфера
    BYTE * cData = new BYTE[iSize];
    (*CommFortGetData)(dwPluginID, 2000,cData,iSize, NULL, NULL);//заполняем буфер

    int iReadOffset = 0;
    int type = fReadInteger(cData, &iReadOffset);
    delete [] cData;
    return type;
}


int AskConnectionStatus()
{
    int iSize = (*CommFortGetData)(dwPluginID, 11, NULL, NULL, NULL, NULL); //получаем объем буфера
    BYTE * cData = new BYTE[iSize];
    (*CommFortGetData)(dwPluginID, 11,cData,iSize, NULL, NULL);//заполняем буфер

    int iReadOffset = 0;
    int status = fReadInteger(cData, &iReadOffset);
    delete [] cData;
    return status;
}


//Текущий пользователь
//
//ID: 12
//Блок данных (исходящий): [нулевое значение]
//Блок данных (входящий): пользователь(текущий пользователь)
//текст(имя пользователя) + текст(IP-адрес пользователя) + число(иконка пользователя)

string GetBotName()
{
    int iSize = (*CommFortGetData)(dwPluginID, 12, NULL, NULL, NULL, NULL); //получаем объем буфера
    BYTE * cData = new BYTE[iSize];
    (*CommFortGetData)(dwPluginID, 12,cData,iSize, NULL, NULL);//заполняем буфер

    int iReadOffset = 0;
    std::wstring wnick = fReadText(cData, &iReadOffset);
    std::string nick = wstostr(wnick, CP_ACP);
    delete [] cData;
    return nick;
}

BOOL __stdcall PluginStart (DWORD dwThisPluginID, typeCommFortProcess func1, typeCommFortGetData func2)
{
    dwPluginID = dwThisPluginID;
    //При инициализации планину присваивается уникальный идентификатор
    //его необходимо обязательно сохранить, и указывать
    //в качестве первого параметра при инициировании событий и получении данных

    CommFortProcess = func1;
    //указываем функцию обратного вызова,
    //с помощью которой плагин сможет инициировать события

    CommFortGetData = func2;
    //указываем функцию обратного вызова,
    //с помощью которой можно будет запрашивать необходимые данные от программы
    //MessageBox(0, GetBotName().c_str(),"Error",MB_ICONSTOP);
//   FError.open((AskPluginTempPath()+errorfile).c_str(), ios::app);
//    try
//    {
    if (!fExists(AskPluginTempPath()+database))
    {
        MessageBoxA(0, (AskPluginTempPath()+database+ utf2acp(" не существует!")).c_str(),"Error",MB_ICONSTOP);
        return false;
    }

    dllCreate();
    //  MessageBoxA(0, (AskPluginTempPath()+inifile).c_str(), "", 0);

    ConnectUser(BOTNAME, GetConfigValW("ip"), GetConfigValW("password"), strtoint(GetConfigValue("icon")));
    InviteUser(GameChannel);
    ChangeState(GetConfigValW("status_off"));
    InitializeCriticalSection(&cs);
    plugin_started=true;
//    }
//    catch(std::exception& err)
//    {
//        FError<<err.what()<<"\n";
//    }
    //Возвращаемые значения:
    //TRUE - запуск прошел успешно
    //FALSE - запуск невозможен
    return TRUE;
}
//---------------------------------------------------------------------------
VOID __stdcall PluginStop()
{

    dllDestroy();
//    FError.close();
    plugin_started = false;

    //данная функция вызывается при завершении работы плагина
}
//---------------------------------------------------------------------------
VOID __stdcall PluginProcess(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize)
{
    //Функция приема событий
    //Параметры:
    //dwID - идентификатор события
    //bInBuffer - указатель на данные
    //dwInBufferSize - объем данных в байтах
    int iReadOffset = 0;

//    try
//    {
//        if(dwID==3 && PROGRAMM_TYPE==1) // реконнект пользователя/ не уверен нужен ли он
//        {
//            int ClientConnectionStatus = fReadInteger(bInBuffer, &iReadOffset);
//            if (ClientConnectionStatus==0)
//            {
//                ConnectUser(BOTNAME, GetConfigValW("ip"), GetConfigValW("password"), strtoint(GetConfigValue("icon")));
//                InviteUser(GameChannel);
//                ChangeState(GetConfigValW("status_off"));
//                return;
//            }
//        }

    if(dwID==1070 || dwID==5)
    {
        if (PROGRAMM_TYPE==0)
            wstring uVirtualUserLogin = fReadText(bInBuffer, &iReadOffset);
        wstring uSenderLogin = fReadText(bInBuffer, &iReadOffset);
        wstring uSenderIP = fReadText(bInBuffer, &iReadOffset);
        int iSenderIcon = fReadInteger(bInBuffer, &iReadOffset);
        wstring uChannel = fReadText(bInBuffer, &iReadOffset);
        int iMessageMode = fReadInteger(bInBuffer, &iReadOffset);
        wstring uText = fReadText(bInBuffer, &iReadOffset);
        if (iMessageMode!=0) return;
        if (uChannel!=GameChannel) return;
        if (uSenderLogin==BOTNAME) return;
        SwitchMessage(uSenderLogin, uText, iSenderIcon);
        return;
    }
    //    }
//    catch(std::exception& err)
//    {
//        FError<<err.what()<<"\n";
//    }

}

void SwitchMessage(wstring uSenderLogin, wstring uText, byte iSenderIcon)
{
    string SenderLogin = wstostr(uSenderLogin, CP_ACP);
    string SenderLoginFirst = SenderLogin;
    replace_with(SenderLogin, "'", "''");
    wreplace_with(uText, L"'", L"''");
    uSenderLogin = strtows(SenderLogin, CP_ACP);

//    byte FirstPlace = SenderLogin.find("\'");
//    if (FirstPlace < SenderLogin.length() / 2)
//    {
//        do
//        {
//            SenderLogin.replace(FirstPlace, 1, "\\\"");
//            FirstPlace = SenderLogin.find("\'");
//        }
//        while (FirstPlace < SenderLogin.length() / 2);
//    }


    if (uText==L"!список")
    {
        ShowUserList(strtows(SenderLoginFirst, CP_ACP));
        return;
    }
    if (uText == L"!старт")
    {
        if (gameStarted)
        {
            MessageToChannel(GameChannel, strtows(SenderLoginFirst+getFraza("1"), CP_ACP));
            return;
        }
        if ((GetConfigValue("start_admin")=="1") && (!IsAdmin(SenderLogin)))
        {
            MessageToChannel(GameChannel, strtows(SenderLoginFirst+">"+getFraza("8"), CP_ACP));
            return;
        }
        else
        {
            startgame(SenderLogin);
            return;
        }
    }

    if (uText == L"!стоп")
    {
        if (!gameStarted)
        {
            MessageToChannel(GameChannel,strtows(SenderLoginFirst+">"+getFraza("2"), CP_ACP));
            return;
        }
        if ((GetConfigValue("stop_admin")=="1") && (!IsAdmin(SenderLogin)))
        {
            MessageToChannel(GameChannel, strtows(SenderLoginFirst+">"+getFraza("8"), CP_ACP));
            return;
        }
        else
        {
            stopgame();
            MessageToChannel(GameChannel, strtows(SenderLoginFirst+getFraza("7"), CP_ACP));
            return;
        }
    }

    if (uText == L"!след" && gameStarted)
    {
        next_command(SenderLogin);
        return;
    }

    if (uText == L"!супер" && gameStarted && (GetConfigValue("enable_super")=="1") && !NextQuestionIsSuper)
    {
        NextQuestionIsSuper = true;
        SuperRunner = SenderLoginFirst;
        SuperRunnerIcon = iSenderIcon;
        wstring FraseToSend = strtows(getFraza("52"), CP_ACP);
//        if (FraseToSend.length() < 1)
//        {
//            FraseToSend = strtows("Запуск супер-игры. Следующий вопрос будет без подсказок.", CP_ACP);
//            string s = "insert into frazy values (52, '"+chcp(wstostr(FraseToSend, CP_ACP), CP_ACP, CP_UTF8)+"')";
//            CppSQLite3Query stmt = sqlite->execQuery(s);
//        }
        MessageToChannel(GameChannel, FraseToSend);
        return;
    }

    if (uText.substr(0,5) == L"!стат")
    {
        uText = wtrim(uText.erase(0,6));
        (uText.length() > 0) ? PersonalStatistic(wstostr(uText, CP_ACP), SenderLoginFirst) : PersonalStatistic(SenderLogin, SenderLoginFirst);
        return;
    }


    if (uText.substr(0, 4) == L"!топ")
    {
        int iParameter;
        uText = wtrim(uText.erase(0, 5));
        if (uText.length() > 0)
        {
            iParameter = strtoint(wstostr(uText, CP_ACP));
            if ((iParameter > 0) && (iParameter <=200))
                showtop(iParameter, SenderLoginFirst, true);
        }
        else
            showtop(10, "", false);
        return;
    }

    if (uText == L"!сброс")
    {
        if (!IsAdmin(SenderLogin))
        {
            MessageToChannel(GameChannel, strtows(SenderLoginFirst+">"+getFraza("8"), CP_ACP));
            return;
        }
        else
        {
            if (gameStarted)
            {
                MessageToChannel(GameChannel, strtows(SenderLoginFirst+">"+getFraza("49"), CP_ACP));
            }
            else
            {
                Clear_stat();
                MessageToChannel(GameChannel, strtows(getFraza("48"), CP_ACP));
            }
        }
        return;
    }

    if (uText == L"!команды")
    {
        wstring s = strtows("\n"+getFraza("50"), CP_ACP);
        PrivateMessage(strtows(SenderLoginFirst, CP_ACP), s);
        return;
    }

    if (uText == L"!инфо")
    {
        wstring Text = strtows("Игра викторина. Версия "+VERSION+" Разработал [url=mailto:supermet@list.ru]SuperMet[/url]. Доработал [url=mailto:lastik1206@gmail.com]ОреЛ[/url]", CP_UTF8);
        MessageToChannel(GameChannel, Text);
        return;
    }

    EnterCriticalSection( &cs );
    if (gameStarted) CheckAnswer(wstostr(uText, CP_ACP), SenderLogin, iSenderIcon);
    LeaveCriticalSection( &cs );
    return;
}

//---------------------------------------------------------------------------
DWORD __stdcall PluginGetData(DWORD dwID, BYTE * bInBuffer, DWORD dwInBufferSize, BYTE * bOutBuffer, DWORD dwOutBufferSize)
{
    //функция передачи данных программе

    // int iReadOffset = 0; //вспомогательные переменные для упрощения работы с блоком данных
    int iWriteOffset = 0;

    //при значении dwOutBufferSize равным нулю функция должна вернуть объем данных, ничего не записывая

    if(dwID==2800) //предназначение плагина
    {
        if(dwOutBufferSize==0)
            return 4; //объем памяти в байтах, которую необходимо выделить программе

        fWriteInteger(bOutBuffer,&iWriteOffset,0);  //плагин подходит только для клиента

        return 4;//объем заполненного буфера в байтах
    }
    else if(dwID==2810) //название плагина (отображается в списке)
    {
        wstring uName =  strtows("Игра викторина, версия "+VERSION, CP_UTF8);//название плагина
        int iSize = uName.length()*2+4;

        if(dwOutBufferSize==0)
            return iSize; //объем памяти в байтах, которую необходимо выделить программе

        fWriteText(bOutBuffer, &iWriteOffset, uName);

        return iSize;//объем заполненного буфера в байтах
    }


    return 0;//возвращаемое значение - объем записанных данных
}
//---------------------------------------------------------------------------
VOID __stdcall PluginShowOptions()
{
    //данная функция вызывается при нажатии кнопки "Настроить" в списке плагинов
    //если Вы не желаете чтобы активировалась кнопка "Настроить", просто сотрите данную функцию
    if (plugin_started)
        ShellExecuteA(0, "open", (AskPluginTempPath()+inifile).c_str(),0,0, SW_SHOWNORMAL);
    else
        MessageBoxW(0, L"Запустите плагин", L"Инфо", 0);
}
//---------------------------------------------------------------------------
VOID __stdcall PluginShowAbout()
{
    MessageBoxW(0, L"Игра викторина\n Разработал SuperMet (mailto:supermet@list.ru) \n Доработал ОреЛ", L"Инфо", 0);
}

void ConnectUser(wstring Name, wstring ip, wstring password, int icon) //для сервера
{
    if (PROGRAMM_TYPE!=0) return; //для клиента выход
    string ads = fTextTostring(Name)+  //имя
                 fTextTostring(ip)+ //IP-адрес
                 fIntegerTostring(0)+  //пароль в открытом виде
                 fTextTostring(password)+   //пароль
                 fIntegerTostring(icon);
    (*CommFortProcess)(dwPluginID,1001,(BYTE*)ads.c_str(),ads.length());
}

void InviteUser(wstring Chanel)
{
    string ads;
    switch (PROGRAMM_TYPE)
    {
    case 0:
        ads = fTextTostring(BOTNAME) + fTextTostring(Chanel) + fIntegerTostring(1) + fIntegerTostring(0);
        (*CommFortProcess)(dwPluginID, 1026, (BYTE*)ads.c_str(), ads.length());
        break;
    case 1:
        ads = fIntegerTostring(1)+ fTextTostring(Chanel);
        (*CommFortProcess)(dwPluginID, 67, (BYTE*)ads.c_str(), ads.length());
        break;
    }
}

void MessageToChannel (wstring Chanel, wstring Message)
{
    //  Message = ReturnApostrophe(Message);
    std::string ads;
    switch (PROGRAMM_TYPE)
    {
    case 0:
        ads = fTextTostring(BOTNAME) + fIntegerTostring(0) + fTextTostring(Chanel) + fTextTostring(Message);
        (*CommFortProcess)(dwPluginID, 1020, (BYTE*)ads.c_str(), ads.length());
        break;
    case 1:
        ads = fTextTostring(Chanel) + fIntegerTostring(0) + fTextTostring(Message);
        (*CommFortProcess)(dwPluginID, 50, (BYTE*)ads.c_str(), ads.length());
        break;
    }
}

void PrivateMessage (wstring Recipient, wstring Message)
{
//    Recipient = ReturnApostrophe(Recipient);
//    Message = ReturnApostrophe(Message);
    string ads;
    DWORD dwID=0;
    switch (PROGRAMM_TYPE)
    {
    case 0:
        (GetConfigValue("send_to_PM")=="1") ? dwID = 1022 : dwID = 1021;
        ads = fTextTostring(BOTNAME) + fIntegerTostring(0) + fTextTostring(Recipient) + fTextTostring(Message);
        break;
    case 1:
        (GetConfigValue("send_to_PM")=="1") ? dwID = 70 : dwID = 63;
        ads = fTextTostring(Recipient) + fIntegerTostring(0)+ fTextTostring(Message);
        break;
    }
    (*CommFortProcess)(dwPluginID, dwID, (BYTE*)ads.c_str(), ads.length());
}

void ChangeTheme (wstring Chanel, wstring Theme)
{
    string ads;
    switch (PROGRAMM_TYPE)
    {
    case 0:
        ads = fTextTostring(BOTNAME) + fTextTostring(Chanel)+ fTextTostring(Theme);
        (*CommFortProcess)(dwPluginID, 1023, (BYTE*)ads.c_str(), ads.length());
        break;
    case 1:
        ads = fTextTostring(Chanel)+ fTextTostring(Theme);
        (*CommFortProcess)(dwPluginID, 61, (BYTE*)ads.c_str(), ads.length());
        break;
    }
}

void ChangeGreeting (wstring Chanel, wstring Greeting)
{
    string ads;
    switch (PROGRAMM_TYPE)
    {
    case 0:
        ads = fTextTostring(BOTNAME) + fTextTostring(Chanel)+ fTextTostring(Greeting);
        (*CommFortProcess)(dwPluginID, 1024, (BYTE*)ads.c_str(), ads.length());
        break;
    case 1:
        ads = fTextTostring(Chanel)+ fTextTostring(Greeting);
        (*CommFortProcess)(dwPluginID, 62, (BYTE*)ads.c_str(), ads.length());
        break;
    }
}

void ChangeState (wstring State)
{
    DWORD id;
    string ads;
    if (PROGRAMM_TYPE==0)
    {
        ads = fTextTostring(BOTNAME)+fTextTostring(State);
        id=1025;
    }
    else
    {
        ads = fTextTostring(State)+fIntegerTostring(0)+fIntegerTostring(0)+fIntegerTostring(0);
        id=53;
    }
    (*CommFortProcess)(dwPluginID, id, (BYTE*)ads.c_str(), ads.length());
}

//int main ()
//{
//    dllCreate();
//    show_next_question();
//    return 0;
//}





