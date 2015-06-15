#include "functions.h"
#include "timer.h"

wstring GameChannel;
string question, otvet, podskazka;
vector <string> words;
CppSQLite3DB * sqlite;
string basa;
bool gameStarted, can_answer, super_game, NextQuestionIsSuper, Bonus;
string SuperRunner="";
string CurentBonusPlayer="";
//UserList PlayingUsers {};
////vector <UserList> Users;
//map<string, UserList> Users;
////UserList * Users;
//map<string, UserList>::iterator uIterator;

int podsk_number;
int nomer;
int GamesBeforeSuper;
byte SuperRunnerIcon;
int NowPlaying=0;
int CurentChain, BonusFactor;
time_t BegTime;
int quests_no_anwer;
string LastAnswer;
unsigned int rsd;



wstring BOTNAME;
int PROGRAMM_TYPE;


ofstream FileError;

map<string,string> configs;
map<string,string>::iterator it;

Timer* Timer_first, *Timer_check, *Timer_answer, *Timer_help;

string Samyi_umniy;

int fReadInteger(BYTE * bInBuffer, int * iOffset)
{
    int iLength;
    memcpy(&iLength, bInBuffer + (*iOffset),4);
    (*iOffset)+=4;
    return iLength;
}

wstring fReadText(BYTE * bInBuffer, int * iOffset)
{
    int iLength;
    memcpy(&iLength, bInBuffer + (*iOffset),4);
    (*iOffset)+=4;
    wstring uRet;
    uRet.resize(iLength);
    memcpy((void*)uRet.c_str(), bInBuffer + (*iOffset),iLength*2);
    (*iOffset)+=iLength*2;
    return uRet;
}

void fWriteInteger(BYTE * bOutBuffer, int * iOffset, int iValue)
{
    memcpy(bOutBuffer + (*iOffset), &iValue, 4);
    (*iOffset)+=4;
}

void fWriteText(BYTE * bOutBuffer, int * iOffset, wstring uValue)
{
    int iLength = uValue.length();

    memcpy(bOutBuffer + (*iOffset),&iLength,4);
    (*iOffset)+=4;

    memcpy(bOutBuffer + (*iOffset),uValue.c_str(),iLength*2);
    (*iOffset)+=iLength*2;
}

string fTextTostring(wstring uText)
{

    int iLength = uText.length();

    string aReturn;

    aReturn.resize(4+iLength*2);

    memcpy((void*)aReturn.c_str(),&iLength,4);
    memcpy((void*)(aReturn.c_str()+4),uText.c_str(),iLength*2);

    return aReturn;
}

string fIntegerTostring(int iValue)
{

    string aReturn;
    aReturn.resize(4);
    memcpy((void*)aReturn.c_str(),&iValue,4);

    return aReturn;
}

wstring ReturnApostrophe(wstring Text)
{
    string Temp = wstostr(Text, CP_ACP);
    string::size_type FirstPlace = Temp.find("\\\"");
    if (FirstPlace < Temp.length())
    {
        while (FirstPlace < Temp.length())
        {
            Temp.replace(FirstPlace, 2, "\'");
            FirstPlace = Temp.find("\\\"");
        }
        Text = strtows(Temp, CP_ACP);
    }
    return Text;
}

std::wstring strtows(const std::string str, UINT codePage)
{
    std::wstring ws;
    int n = MultiByteToWideChar(codePage, 0, str.c_str(), str.size()+1, /*dst*/NULL, 0);
    if(n)
    {
        ws.resize(n-1);
        if(MultiByteToWideChar(codePage, 0, str.c_str(), str.size()+1, /*dst*/&ws[0], n) == 0)
            ws.clear();
    }
    return ws;
}

std::string wstostr(const std::wstring ws, UINT codePage)
{
    std::string str;
    int n = WideCharToMultiByte(codePage, 0, ws.c_str(), ws.size()+1, /*dst*/NULL, 0, /*defchr*/0, NULL);
    if(n)
    {
        str.resize(n-1);
        if(WideCharToMultiByte(codePage, 0, ws.c_str(), ws.size()+1, /*dst*/&str[0], n, /*defchr*/0, NULL) == 0)
            str.clear();
    }
    return str;
}

std::string chcp(const std::string str, UINT codePageSrc, UINT codePageDst)
{
    return wstostr(strtows(str, codePageSrc), codePageDst);
}

string utf2acp(string src)
{
    return chcp(src, CP_UTF8, CP_ACP);
}

string acp2utf(string src)
{
    return chcp(src, CP_ACP, CP_UTF8);
}



void replace_with(std::string & src, const std::string & what, const std::string & with)
{
    if (what != with)
    {
        std::string temp;
        std::string::size_type prev_pos = 0, pos = src.find(what, 0);
        while ( std::string::npos != pos )
        {
            temp += std::string(src.begin() + prev_pos, src.begin() + pos) + with;
            prev_pos = pos + what.size();
            pos = src.find(what, prev_pos);
        }
        if ( !temp.empty() )
        {
            src = temp + std::string(src.begin() + prev_pos, src.end());
        }
    }
}

//Example
//std::string strAcpSrc = "";
//std::string strUtf = chcp(strAcpSrc, CP_ACP, CP_UTF8);
//std::string strAcp = chcp(strUtf, CP_UTF8, CP_ACP);
//std::cout << ((strAcpSrc == strAcp) ? "success" : "error");
//std::cin.get();

void wreplace_with(std::wstring & src, const std::wstring & what, const std::wstring & with)
{
    if (what != with)
    {
        std::wstring temp;
        std::wstring::size_type prev_pos = 0, pos = src.find(what, 0);
        while ( std::wstring::npos != pos )
        {
            temp += std::wstring(src.begin() + prev_pos, src.begin() + pos) + with;
            prev_pos = pos + what.size();
            pos = src.find(what, prev_pos);
        }
        if ( !temp.empty() )
        {
            src = temp + std::wstring(src.begin() + prev_pos, src.end());
        }
    }
}

string trim(string str)
{
    std::string::size_type pos = str.find_last_not_of(' ');
    if(pos != std::string::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != std::string::npos) str.erase(0, pos);
    }
    else
        str.erase(str.begin(), str.end());
    return str;
}

wstring wtrim(wstring str)
{
    std::wstring::size_type pos = str.find_last_not_of(' ');
    if(pos != std::wstring::npos)
    {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != std::wstring::npos) str.erase(0, pos);
    }
    else
        str.erase(str.begin(), str.end());
    return str;
}

string toLowerCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
    return str;
}

string toUpperCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), (int(*)(int))toupper);
    return str;
}

string formatString (const string& format, ...)
{
    char buffer[512];
    va_list args;
    va_start (args, format);
    vsprintf (buffer,format.c_str(), args);
    va_end (args);
    return string(buffer);
}

string& replaceFirst(std::string &str, const std::string &format, const std::string &replace)
{
    std::string::size_type pos = str.find(format);
    if (pos!=std::string::npos)
        str.replace(pos, format.length(), replace);
    return str;
}


string inttostr(int num)
{
    stringstream r;
    r << num;
    return r.str();
}

string doubletostr(double num)
{
    char result[20];
    sprintf(result, "%.0f", num);
    return result;
}

int strtoint(string s)
{
    return atoi(s.c_str());
}

//template <typename T>
//std::string toString(T val)
//{
//    std::ostringstream oss;
//    oss<< val;
//    return oss.str();
//}
//template<typename T>
//T fromString(const std::string& s)
//{
//    std::istringstream iss(s);
//    T res;
//    iss >> res;
//    return res;
//}
//string str;
//int iVal=123;
//float fVal=123.45;
//
//str = toString(iVal);
//str = toString(fVal);
//
//iVal = fromString<int>(str);
//fVal = fromString<float>(str);
//
//cout<<iVal<<endl<<fVal<<endl;
//
//getch();


string GetParam(wstring txt)
{
    string parametr="";
    string zaprostrim = trim(wstostr(txt, CP_ACP));
    string::size_type pos = (zaprostrim.find_first_of(' '));
    if (pos!=string::npos)
        parametr = trim(zaprostrim.substr(pos+1));
    return parametr;
}


void LoadConfig()
{
    string line;
    ifstream myfile ((AskPluginTempPath()+inifile).c_str());
    if (!myfile.is_open()) return;
    while (!myfile.eof() )
    {
        getline (myfile,line);
        line = trim(line);
        if (line.substr(0, 1)=="#" || line.substr(0, 1)==";") continue;
        string::size_type pos=line.find("=",0);
        if (pos==string::npos) continue;
        string key = trim(line.substr(0, pos));
        string value = trim(line.substr(pos+1));

        configs[key] = value;
    }
    myfile.close();
}

bool fExists(const string&filename)
{
    ifstream ifs(filename.c_str());
    bool result = (ifs != NULL);
    ifs.close();
    return result;
}

string GetConfigValue(string key)
{
    string res="";
    it = configs.find(key);
    if( it != configs.end() )
        res = it->second;
    return res;
}

wstring GetConfigValW(string key)
{
    return strtows(GetConfigValue(key), CP_ACP);
}


int countchar(string st, char ch)
{
    string::size_type n=0;
    for(string::size_type i=0; i < st.length(); ++i)
    {
        if (st.at(i)==ch) n++;
    }
    return n;
}


string count_simvols(string st)
{
    string::size_type n=0;
    for(string::size_type i=0; i < st.length(); ++i)
    {
        if (st.at(i)!=' ') n++;
    }
    return inttostr(n);
}

int count_words(string str)
{
    std::stringstream ss(str);
    int i(0);
    while(ss >> str)
        i++;
    return i;
}

unsigned int getrandom(unsigned int min, unsigned int max)
{
    rsd = rsd * 0x8088405 + 1;
    unsigned int random = (unsigned __int64) (rsd) * (max-min+1) >> 32;
    return random + min;
}


//int random(int lim){
//    g_RandSeed = g_RandSeed * 0x8088405 + 1;
//    return __int64(g_RandSeed) * lim >> 32;
////134775813
//}

string current_dir()
{
    CHAR Buffer[BUFSIZE];
    DWORD dwRet;
    //dwRet = GetCurrentDirectoryA(BUFSIZE, Buffer);
    dwRet = GetModuleFileNameA(GetModuleHandleA(""), Buffer, BUFSIZE);
    if( dwRet == 0 )
        return ("");
    if(dwRet > BUFSIZE)
        return ("");
    string s(Buffer);
    return s.substr(0,s.find_last_of("\\/"));
}

//string GetCurrentPath(){
//    char CurrentPath[MAX_PATH];
//    getcwd(CurrentPath, MAX_PATH);
//    string result(CurrentPath);
//    return result;
//}

//bool  GetIgraut()
//{
//    return gameStarted;
//}


void __stdcall first_ontimer(HWND hwnd, UINT msg, UINT idtimer, DWORD dwTime)
{
    SetAnswerQuestion();
    show_next_question();

    Timer_check->start();
    Timer_first->stop();
    Timer_answer->start();
    if (!super_game)
        Timer_help->start();
}

void __stdcall check_ontimer(HWND hwnd, UINT msg, UINT idtimer, DWORD dwTime)
{
    if (LastAnswer.length() == 0)
        quests_no_anwer++;
    else
        quests_no_anwer = 0;
    if (quests_no_anwer >= strtoint(GetConfigValue("answers_stop")))
    {
        stopgame();
        MessageToChannel(GameChannel, strtows(getFraza("15"), CP_ACP));
        return;
    }
    SetAnswerQuestion();

    if (NextQuestionIsSuper)
    {
        super_start();
        return;
    }
    GamesBeforeSuper++;
    if (GamesBeforeSuper == strtoint(GetConfigValue("super_run")))
    {
        SuperRunner = "AUTO";
        super_start();
        GamesBeforeSuper=0;
        return;
    }

    show_next_question();

    Timer_help->start();
    Timer_answer->start();
}

void __stdcall answer_ontimer(HWND hwnd, UINT msg, UINT idtimer, DWORD dwTime)
{
    can_answer = false;
    super_game = false;

    if (GetConfigValue("show_answer") == "1")
        MessageToChannel(GameChannel, strtows(getFraza("13") + getFraza("20") + "[url]" + otvet + "[/url]", CP_ACP));
    else
        MessageToChannel(GameChannel, strtows(getFraza("13"), CP_ACP));

    Timer_help->stop();
    Timer_answer->stop();
}

void __stdcall help_ontimer(HWND hwnd, UINT msg, UINT idtimer, DWORD dwTime)
{
    if (countchar(podskazka, '*') < 2) return;
    SetPodskazka();
    podsk_number++;
    string s = "";

    s = getFraza("70");
    replaceFirst(s, utf2acp("%№%"), inttostr(podsk_number));
    replaceFirst(s,"%help%", podskazka);

    MessageToChannel(GameChannel , strtows(s, CP_ACP));
}

//void LoadUsers()
//{
//    string s = "select count(*) from users";
//    CppSQLite3Query stmt = sqlite->execQuery(s);
//    int Count = stmt.getIntField(0);
//    if (Count==0) return;
//    s = "select name from users";
//    stmt = sqlite->execQuery(s);
////    PlayersInBase = 0;
//    while (!(stmt.eof()))
//    {
//        //   PlayingUsers.Name = chcp(stmt.getStringField(0), CP_UTF8, CP_ACP);
//        PlayingUsers.LastAnswer = 0;
//        PlayingUsers.InGame = false;
//        PlayingUsers.WantGame = false;
//
//        Users[utf2acp(stmt.getStringField(0))]= PlayingUsers;
//        stmt.nextRow();
////        PlayersInBase++;
//    }
//}


void dllCreate()
{
    rsd = GetTickCount();
    //  setlocale(LC_ALL,"Russian");
    setlocale(LC_ALL,"");

    gameStarted = false;
    can_answer = false;
    NextQuestionIsSuper = false;

    LoadConfig();
    LoadQuestions();

    GameChannel = GetConfigValW("kanal");
    PROGRAMM_TYPE = AskProgrammType();
    if (PROGRAMM_TYPE==1) BOTNAME = strtows(GetBotName(), CP_ACP);
    else
        BOTNAME = GetConfigValW("botname");

    ChangeState(strtows(GetConfigValue("status_off"), CP_ACP));

    Timer_first = new Timer(&first_ontimer);
    Timer_check = new Timer(&check_ontimer);
    Timer_answer = new Timer(&answer_ontimer);
    Timer_help = new Timer(&help_ontimer);

    basa = acp2utf(AskPluginTempPath() + database);

    sqlite = new CppSQLite3DB();
    sqlite->open(basa.c_str());
    sqlite->execDML("Vacuum");
    sqlite->execDML("update users set in_game=0, want_game=0, last_answer=0, replies=0");

    adsuper();
    adrangs();
}

void dllDestroy()
{
    words.clear();
    Timer_first->stop();
    Timer_check->stop();
    Timer_answer->stop();
    Timer_help->stop();
    delete Timer_first;
    delete Timer_check;
    delete Timer_answer;
    delete Timer_help;
//    Users.clear();
    FileError.close();
    configs.clear();
    sqlite->close();
    delete sqlite;
}

//----------------------------------------------

void LoadQuestions()
{
    string line;
    ifstream myfile ((AskPluginTempPath()+questions).c_str());
    if (myfile.is_open())
    {
        while (!myfile.eof() )
        {
            getline (myfile,line);
            line = trim(line);
            if (line.length()>0)
                words.push_back(line);
        }
        myfile.close();
    }
}

void SetAnswerQuestion()
{
    string s;
    question = "";
    otvet = "";
    podskazka = "";
    if (words.size() ==0) return;
    s = trim(words.at(getrandom(0, words.size()-1)));
    string::size_type p = s.find("|",0);
    if (p!=string::npos)
    {
        question = trim(s.substr(0, p));
        otvet = trim(s.substr(p+1));
        for(string::size_type i=0; i < otvet.length(); ++i)
            switch(otvet.at(i))
            {
            case ' ':
                podskazka+=' ';
                break;
            case '-':
                podskazka+='-';
                break;
            default:
                podskazka+='*';
                break;
            }
    }
}

void SetPodskazka()
{
    vector <int> numbers;
    for(string::size_type i=0; i < podskazka.length(); ++i)
        if (podskazka.at(i)== '*')
            numbers.push_back(i);

    int rand=numbers.at(getrandom(0,numbers.size()-1));
    podskazka.at(rand)=otvet.at(rand);

    numbers.clear();
}

int getcount(string tab, string keyfield, string value)
{
    string s;
    int n=0;
    s="select count(*) from "+tab+" where "+keyfield+" = '"+acp2utf(value)+"' limit 1";
    CppSQLite3Query stmt = sqlite->execQuery(s);
    if (!(stmt.eof()))
    {
        n=stmt.getIntField(0);
    }

    return n;
}

string getfieldvalue(string tab, string fieldname, string keyfield, string value)
{
    string s = "select "+fieldname+" from "+tab+" where "+keyfield+" = '"+acp2utf(value)+"' limit 1";
    CppSQLite3Query stmt = sqlite->execQuery(s);
    string st="";
    if (!(stmt.eof()))
    {
        st=utf2acp(stmt.getStringField(0));
    }
    return st;
}


int getfieldasint(string tab, string fieldname)
{
    int n=0;
    string s="select "+fieldname+" from "+tab+" limit 1";
    CppSQLite3Query stmt = sqlite->execQuery(s);
    if (!(stmt.eof()))
    {
        n=stmt.getIntField(0);
    }
    return n;
}


string getFraza(string key)
{
    string Result;
    Result = getfieldvalue ("frazy", "name", "id", key);
//    if (Result.length()<1)
//    {
//        CppSQLite3Query stmt;
//        string s = "";
//        switch (strtoint(key))
//        {
//        case 52:
//            Result = "Запуск супер-игры. Следующий вопрос будет без подсказок.";
//            s = "insert into frazy values (52, '"+chcp(Result, CP_ACP, CP_UTF8)+"')";
//            stmt = sqlite->execQuery(s);
//            break;
//        case 53:
//            Result = "Вы превысили лимит ответов на этот вопрос.";
//            s = "insert into frazy values (53, '"+chcp(Result, CP_ACP, CP_UTF8)+"')";
//            stmt = sqlite->execQuery(s);
//            break;
//        }
//    }
    return Result;
}

bool slovopodhodit(string varian, string ish)
{
    if (trim(varian).length()==0) return false;
    if (trim(ish).length()==0) return false;

    return (toLowerCase(trim(varian)) == toLowerCase(trim(ish)));
}

string getmesto(string user)
{
    int n=0;
    string s = "select name from users order by ochki desc, name";
    CppSQLite3Query stmt = sqlite->execQuery(s);
    while (!(stmt.eof()))
    {
        n++;
        string dbuser=utf2acp(stmt.getStringField(0));
        replace_with(dbuser, "'", "''");
        if (dbuser==user)
            return inttostr(n);
        stmt.nextRow();
    }
    return "0";
}


string getbest()
{
    CppSQLite3Query stmt = sqlite->execQuery("select name, ochki from users order by ochki desc, name limit 1");
    string n="";
    if (!(stmt.eof()))
        n=utf2acp(stmt.getStringField(0));
    return n;
}

string GetRang(int ochki)
{
    CppSQLite3Query stmt = sqlite->execQuery("select name from rangs where ochki<="+inttostr(ochki)+" order by ochki desc limit 1");
    string rang="";
    if (!(stmt.eof()))
        rang=utf2acp(stmt.getStringField("name"));
    return rang;
}


bool IsAdmin(string user)
{
    return (getfieldvalue("users", "isadmin", "name", user)=="1");
}

void showtop (int n, string to, bool toPrivate)
{
    int i;
    string s;
    CppSQLite3Query stmt = sqlite->execQuery("select ochki, name from users order by ochki desc, name limit "+ inttostr(n));
    i=1;
    s="\n"+getFraza("9")+inttostr(n)+getFraza("4")+"\n";
    while (!(stmt.eof()))
    {
        s+=inttostr(i)+": "+utf2acp(stmt.getStringField(1))+getFraza("22")+stmt.getStringField(0)+"\n";
        i++;
        stmt.nextRow();
    }
    s+="\n"+getFraza("34")+inttostr(words.size())+"\n"+
       getFraza("35")+inttostr(getfieldasint("statistic", "zadano"))+"\n"+
       getFraza("31")+inttostr(getfieldasint("statistic", "otgadano"))+"\n";
    (toPrivate)?PrivateMessage(strtows(to, CP_ACP), strtows(s, CP_ACP)):MessageToChannel(GameChannel, strtows(s, CP_ACP));
}



string formatDate (const string &format)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    // strftime (buffer,80,"%d/%m/%Y в %H:%M:%S",timeinfo);
    strftime (buffer,80, format.c_str(),timeinfo);
    return buffer;
}


void showGamePrivet()
{
    int i;
    double vsego, otgadal;
    char procn[20];
    string s;
    CppSQLite3Query stmt = sqlite->execQuery("select ochki, name, vsego, otgadal from users order by ochki desc, name limit 10");
    i=1;
    s=getFraza("42")+"\n"+getFraza("43")+"\n"+getFraza("44")+"\n";
    while (!(stmt.eof()))
    {
        vsego = stmt.getFloatField(2);
        otgadal = stmt.getFloatField(3);
        if (vsego>0)
            sprintf(procn,"%.1f",floor(otgadal/vsego*1000 + 0.5)/10);
        else
            strcpy(procn,"0,0");
        s+=" "+inttostr(i)+": [url]"+utf2acp(stmt.getStringField(1))+"[/url]"+
           getFraza("22")+"[url]"+stmt.getStringField(0)+"[/url]  "+
           getFraza("45")+"[url]"+procn+"%[/url]\n";
        i++;
        stmt.nextRow();
    }
    s+= getFraza("43")+"\n"+
        getFraza("34")+inttostr(words.size())+"\n"+
        getFraza("35")+inttostr(getfieldasint("statistic", "zadano"))+"\n"+
        getFraza("31")+inttostr(getfieldasint("statistic", "otgadano"))+"\n"+
        getFraza("46")+formatDate("%d/%m/%Y - %H:%M:%S")+"\n"+
        getFraza("43")+"\n"+
        getFraza("50");
    ChangeGreeting(GameChannel , strtows(s, CP_ACP));
}

void PersonalStatistic(string user, string to)
{
    int count = getcount("users", "name", user);
    if (count == 0) return;

    string s="", adminis;
    CppSQLite3Query stmt = sqlite->execQuery("select vsego, otgadal, ochki, otvet_time from users where name='"+acp2utf(user)+"'");
    if (IsAdmin(user))
        adminis=utf2acp(" Да");
    else adminis=utf2acp(" Нет");

    double vsego, otgadal;
    char procn[20];
    string sr_vr;

    if (!(stmt.eof()))
    {
        vsego = stmt.getFloatField(0);
        otgadal = stmt.getFloatField(1);
        if (vsego>0)
            sprintf(procn,"%.1f",floor(otgadal/vsego*1000 + 0.5)/10);
        else
            strcpy(procn,"0,0");

        if (otgadal>0)
            sr_vr = doubletostr(Round(stmt.getFloatField(3)/otgadal));
        else
            sr_vr=" ";

        s="\n[url]"+getFraza("27")+user+":[/url]\n"+
          getFraza("32")+ getmesto(user) +"\n"+
          getFraza("28")+stmt.getStringField(2)+"\n"+
          getFraza("47")+ GetRang(stmt.getIntField(2)) +"\n"+
          getFraza("30")+stmt.getStringField(0)+"\n"+
          getFraza("31")+stmt.getStringField(1)+"\n"+
          getFraza("45")+procn+"%\n"+
          getFraza("26")+ sr_vr +"\n"+
          getFraza("33")+adminis+"\n";
    }
    PrivateMessage(strtows(to, CP_ACP), strtows(s, CP_ACP));
}

void InsertDots(string &str, int prcnt)
{
    if (prcnt<0) return;
    int i=0;
    int cnt = (int) Round(double(prcnt)*str.length()/100);
    while (i<cnt)
    {
        str.insert(getrandom(1, str.length()-1),".");
        i++;
    }
}

void show_next_question()
{
    string s;
    nomer++;
    InsertDots(question, strtoint(GetConfigValue("dots_count")));
    int CountWords = count_words(otvet);
    if (CountWords==1)
        (nomer==1)?s=getFraza("63"):s=getFraza("64");
    else
    {
        (nomer==1)?s=getFraza("65"):s=getFraza("66");
        replaceFirst(s, "%words%", inttostr(CountWords));
    }
    replaceFirst(s, utf2acp("%№%"), inttostr(nomer));
    replaceFirst(s, "%question%", question);
    replaceFirst(s, "%letters%", count_simvols(otvet));


    MessageToChannel(GameChannel, strtows(s, CP_ACP));
    CheckActivity();

    NowPlaying=CountInGame();
    if (!Bonus && (NowPlaying >= strtoint(GetConfigValue("players_for_bonus"))))
    {
        Bonus = true;
        MessageToChannel(GameChannel, strtows(getFraza("54"), CP_ACP));
        CurentChain = 1;
        CurentBonusPlayer = "";
    }
    if (Bonus && (NowPlaying < strtoint(GetConfigValue("players_for_bonus"))))
    {
        //CurentBonusPlayer = "";
        Bonus = false;
        MessageToChannel(GameChannel, strtows(getFraza("55"), CP_ACP));
    }

    ///------------------
    CppSQLite3Query stmt = sqlite->execQuery("select zadano from statistic limit 1");
    if (stmt.eof())
    {
        sqlite->execDML("insert into statistic values(0, 0)");
    }

    sqlite->execDML("update statistic set zadano=zadano+1");
    ///-------------------

    podsk_number=0;
    LastAnswer="";
    sqlite->execDML("update users set replies=0");
    BegTime=time(NULL);
    can_answer=true;

}

void Register_user(string user)
{
    string format, sql;
    int count= getcount("users", "name", user);
    if (count==0)
    {
        format = "insert into users(name, in_game, want_game, last_answer) values ('%s', %s, %s, %s)";
        sql= formatString(format, acp2utf(user).c_str(), "1", "1", inttostr(nomer).c_str());
    }
    else
    {
        format = "update users set in_game=%s, want_game=%s, last_answer=%s where name='%s'";
        sql= formatString(format, "1", "1", inttostr(nomer).c_str(), acp2utf(user).c_str());
        //MessageBoxA(0, sql.c_str(), "", 0);
    }
    sqlite->execDML(sql);
}

void Add_reply(string user)
{
    string format, sql;
    int count=getcount("users", "name", user);
    if (count==0)
        format = "insert into users(name, replies) values ('%s', 1)";
    else
        format = "update users set replies=replies+1 where name='%s'";

    sql= formatString(format, acp2utf(user).c_str());
    sqlite->execDML(sql);
    format = "update users set vsego=vsego+1 where name='%s'";
    sql= formatString(format, acp2utf(user).c_str());
    sqlite->execDML(sql);
}

void startgame(string user)
{
    string s;
    NowPlaying = 0;
    CurentChain = 1;
    Bonus = false;
    NextQuestionIsSuper = false;
    nomer = 0;
    Register_user(user);

    int count=CountWantGame();
    if (count < strtoint(GetConfigValue("gamers_count")))
    {
        s = getFraza("62");
        replaceFirst(s, "%count%", inttostr(count));
        replaceFirst(s, "%needcount%", GetConfigValue("gamers_count"));

        gameStarted = false;
    }
    else
    {
        quests_no_anwer = 0;
        GamesBeforeSuper = 1;
        LastAnswer ="";
        gameStarted = true;
        ChangeState(GetConfigValW("status_on"));
        sqlite->execDML("update users set want_game=0");
        s = user + getFraza("14");
        //  (usericon == 0) ? replaceFirst(s, utf2acp("(ая)"), "") : replaceFirst(s, utf2acp("ий(ая)"), utf2acp("ая"));
        //  replaceFirst(s, utf2acp("а"),)
        Timer_first->interval = strtoint(GetConfigValue("question_pause"))*1000;
        Timer_first->start();
        Timer_check->interval = strtoint(GetConfigValue("interval"))*1000;
        Timer_answer->interval = Timer_check->interval - Timer_first->interval;
        Timer_help->interval = strtoint(GetConfigValue("help_interval"))*1000;
    }
    can_answer = false;
    super_game = false;
    MessageToChannel(GameChannel, strtows(s, CP_ACP));
    if (GetConfigValue("show_privet")=="1")
        if (strtoint(getmesto(user)) < 11)
            showGamePrivet();
}


void stopgame()
{
    Timer_first->stop();
    Timer_check->stop();
    Timer_answer->stop();
    Timer_help->stop();
    gameStarted = false;
    can_answer = false;
    super_game = false;
    ChangeState(GetConfigValW("status_off"));
    sqlite->execDML("update users set in_game=0, want_game=0, last_answer=0, replies=0");
}

string ReplacePointsFrase(int points)
{
    string res = utf2acp("балл");
    byte modulo = points % 10;
    if (modulo == 1) return res;
    if (modulo > 1 && modulo < 5) res+="a";
    if (modulo > 4 || modulo < 1) res+=utf2acp("ов");
    return res;
}

void next_command(string user)
{
    int ochki, minus;
    if (getcount("users", "name", user) == 0) return;

    quests_no_anwer = 0;

    minus = strtoint(GetConfigValue("next_minus"));
    if (minus < 0) minus = 0;
    ochki = strtoint(getfieldvalue("users", "ochki", "name", user)) - minus;
    sqlite->execDML("update users set ochki=" + inttostr(ochki) + "  where name='" + acp2utf(user) + "'");
    //%user%> -%minus% балл(а,ов) за команду !след. %points% балл(а,ов), %place%-е место.
    string s = "";

    s = getFraza("61");
    replaceFirst(s, "%user%", user);
    replaceFirst(s, "%minus%", inttostr(minus));
    replaceFirst(s, "%points%", inttostr(ochki));
    replaceFirst(s, "%place%", getmesto(user));

    replaceFirst(s, utf2acp("балл(а,ов)"), ReplacePointsFrase(minus));
    replaceFirst(s, utf2acp("балл(а,ов)"), ReplacePointsFrase(ochki));

    MessageToChannel(GameChannel,strtows(s, CP_ACP));
    if (NextQuestionIsSuper)
    {
        super_start();
        return;
    }
    GamesBeforeSuper++;
    if (GamesBeforeSuper == strtoint(GetConfigValue("super_run")))
    {
        SuperRunner = "AUTO";
        super_start();
        GamesBeforeSuper = 0;
        return;
    }

    Timer_first->stop();
    Timer_first->start();
    Timer_check->stop();
    Timer_answer->stop();
    Timer_help->stop();
    can_answer=false;
    super_game=false;
}

void super_start()
{
    if (super_game) return;
    //if (getcount("users", "name", user)==0) return;
    if (SuperRunner == "AUTO")
    {
        std::string s = getFraza("52");
        MessageToChannel(GameChannel, strtows(s, CP_ACP));
    }
    else
    {
        //quests_no_anwer=0;
        std::string s = SuperRunner+getFraza("51");

        (SuperRunnerIcon == 0) ? replaceFirst(s, utf2acp("(а)"), "") : replaceFirst(s, utf2acp("(а)"), "a");
        MessageToChannel(GameChannel, strtows(s, CP_ACP));
    }

    Timer_first->stop();
    Timer_first->start();
    Timer_check->stop();
    Timer_answer->stop();
    Timer_help->stop();
    can_answer=false;
    super_game=true;

    NextQuestionIsSuper = false;
}


double Round (double x)
{
    return (floor (x + 0.5));
}


int CountOchki(int max, int len, int num)
{
    int result;
    (len>0 && len>num) ? result = (int) Round(double(max)*(len-num)/len) : result = 1;
    if (result == 0)
        result = 1;
    return result;
}

std::string GenerateAnswer(int interval, int sec)
{
    int part;
    if (interval==0) interval=1;
    if (sec<0) sec=0;
    part= (int) Round(double(sec)/interval*4);
    string result;
    switch (part)
    {
    case 0:
        result=getFraza("36");
        break;
    case 1:
        result=getFraza("37");
        break;
    case 2:
        result=getFraza("38");
        break;
    case 3:
        result=getFraza("39");
        break;
    case 4:
        result=getFraza("40");
        break;
    default:
        result=getFraza("38");
        break;
    }
    return result;
}

bool PochtiUgadal(string mess, string otvet)
{
    std::string::size_type n=0;
    string a,b;
    mess=trim(mess);
    otvet=trim(otvet);
    if (mess.length()>otvet.length()) return false;
    for(string::size_type i=0; i < mess.length(); ++i)
    {
        a = mess.at(i);
        b = otvet.at(i);

        if  (toLowerCase(a)==toLowerCase(b)) n++;
    }
    return (n==otvet.length()-1);
}

//bool findUser(string user, UserList &User)
//{
//    ZeroMemory(&User, sizeof(UserList));
//    uIterator = Users.find(user);
//    if (uIterator!=Users.end())
//    {
//        User = uIterator->second;
//        return true;
//    }
//    else
//        return false;
//}


void CheckActivity() //проверка активности среди играющих
{
    // если последний раз игрок ответил 4 вопросов назад
    string sql = "update users set in_game=0 where ("+inttostr(nomer)+"-last_answer>4) and in_game=1";
    sqlite->execDML(sql);
}

int CountWantGame()
{
    return getcount("users", "want_game", "1");
}

int CountInGame()
{
    return getcount("users", "in_game", "1");
}


void CheckAnswer(string mess, string user, byte usericon) //проверка ответа
{
    int sec, interval;
    int plusochki,  ochki;
    int BonusPlus=0;
    BonusFactor=0;
    string s;
    Register_user(user);
    //блок объявлений переменных
    int max_replies = strtoint(GetConfigValue("max_replies"));
    int replies = strtoint(getfieldvalue("users", "replies", "name", user));
    if  ((replies>max_replies) && (max_replies!=0))
    {
        wstring FraseToSend = strtows(getFraza("53"), CP_ACP);
        MessageToChannel(GameChannel, strtows(user + "> "+ wstostr(FraseToSend, CP_ACP), CP_ACP));
        return;
    }
    //int UserNumber = NumOfUser(user);
    //if (UserNumber<0) return;
    //UserList PlayingUsers = Users.at(UserNumber);

    //  if (!findUser(user, PlayingUsers))
    //      return;

    // PlayingUsers.InGame = true;
    // PlayingUsers.LastAnswer = nomer;
    //   NowPlaying++;

//    if (!Bonus && (NowPlaying >= strtoint(GetConfigValue("players_for_bonus"))))
//    {
//        Bonus = true;
//        MessageToChannel(GameChannel, strtows("Система бонусов включена", CP_UTF8));
//        CurentBonusPlayer = "";
//        CurentChain = 0;
//    }
//    if (Bonus && (NowPlaying < strtoint(GetConfigValue("players_for_bonus"))))
//    {
//        Bonus = false;
//        MessageToChannel(GameChannel, strtows("Система бонусов отключена", CP_UTF8));
//    }
    if (!can_answer) return;

    LastAnswer=mess;
    Add_reply(user);

    if (slovopodhodit(mess, otvet))
    {
        sec = (int) difftime(time(NULL), BegTime);
        if (Bonus)
        {
            if (user == CurentBonusPlayer)
                CurentChain++;
            else
            {
                CurentChain = 1;
                CurentBonusPlayer = user;
                BonusFactor = 0;
            }
        }
        can_answer=false;

        quests_no_anwer=0;

        sqlite->execDML("update statistic set otgadano=otgadano+1");

        sqlite->execDML("update users set otgadal=otgadal+1 where name='"+acp2utf(user)+"'");

        //MessageBoxA(0, doubletostr(sec).c_str(), "", 0);
        sqlite->execDML("update users set otvet_time=otvet_time+"+inttostr(sec)+" where name='"+acp2utf(user)+"'");

        interval = strtoint(GetConfigValue("interval"))-strtoint(GetConfigValue("question_pause"));
        int AnswersForBonus = strtoint(GetConfigValue("answers_for_bonus"));
        if (CurentChain+1 > AnswersForBonus)
            BonusFactor = CurentChain - AnswersForBonus+1;
        (Bonus) ? BonusPlus = strtoint(GetConfigValue("bonus_add")) * BonusFactor : BonusPlus = 0;
        if (super_game && (GetConfigValue("enable_super")=="1"))
        {
            plusochki = CountOchki(strtoint(GetConfigValue("max_scores")),interval, sec) * strtoint(GetConfigValue("super_factor")) + BonusPlus;
        }
        else
        {
            plusochki = CountOchki(strtoint(GetConfigValue("max_scores")), otvet.length(), podsk_number) + BonusPlus;
        }
        super_game = false;
        string LastBestPlayer = getbest();
        ochki=strtoint(getfieldvalue("users", "ochki", "name", user))+plusochki;
        sqlite->execDML("update users set ochki="+inttostr(ochki)+" where name='"+acp2utf(user)+"'");
        string BonusFrase = "";
        if (Bonus && BonusFactor > 0)
            BonusFrase=utf2acp(" (бонус: ")+inttostr(BonusPlus)+")";
        //%user% %genansw% %+% %bonus% балл(а,ов), У %user% %points% балл(а,ов) %place% -е место.
        s=getFraza("60");


        replaceFirst(s, "%user%", user);
        replaceFirst(s, "%genansw%", GenerateAnswer(interval, sec));
        replaceFirst(s,"%+%",inttostr(plusochki));
        replaceFirst(s,"%bonus%", BonusFrase);
        replaceFirst(s,"%user%", user);
        replaceFirst(s, "%points%", inttostr(ochki));
        replaceFirst(s,"%place%", getmesto(user));
        replaceFirst(s,"%answer%", otvet);
        replaceFirst(s,utf2acp("балл(а,ов)"), ReplacePointsFrase(plusochki));
        replaceFirst(s,utf2acp("балл(а,ов)"), ReplacePointsFrase(ochki));
        (usericon == 0) ? replace_with(s, utf2acp("(а)"), "") : replace_with(s, utf2acp("(а)"), utf2acp("а"));

        MessageToChannel(GameChannel,strtows(s, CP_ACP));

        if (GetConfigValue("change_theme")=="1")
        {
            Samyi_umniy=getbest();
            if (Samyi_umniy != LastBestPlayer)
            {
                s = Samyi_umniy+getFraza("18");
                (usericon == 0) ? replaceFirst(s, utf2acp("(ая)"), "") : replaceFirst(s, utf2acp("ий(ая)"), utf2acp("ая"));
                ChangeTheme(GameChannel, strtows(s, CP_ACP));
            }
        }

        /*if (ochki>strtoint(GetConfigValue("scores_to_star")))
        {
        s=user+ getFraza("19");
        MessageToChannel(GameChannel, strtows(s, CP_ACP));
        zvezd=strtoint(getfieldvalue("users", "zvezd", "name", user))+1;
        sqlite->execDML("update users set zvezd="+inttostr(zvezd)+" where name='"+chcp(user, CP_ACP, CP_UTF8)+"'");
        if (GetConfigValue("scores_reset")=="1")
        sqlite->execDML("update users set ochki=0");
        else
        sqlite->execDML("update users set ochki=0 where name='"+chcp(user, CP_ACP, CP_UTF8)+"'");
        }*/
        if (GetConfigValue("show_privet")=="1")
            if (strtoint(getmesto(user)) < 11)
                showGamePrivet();

        if (NextQuestionIsSuper)
        {
            super_start();
            return;
        }
        GamesBeforeSuper++;
        if (GamesBeforeSuper == strtoint(GetConfigValue("super_run")))
        {
            SuperRunner = "AUTO";
            super_start();
            GamesBeforeSuper = 0;
            return;
        }
        Timer_first->stop();
        Timer_first->start();
        Timer_check->stop();
        Timer_answer->stop();
        Timer_help->stop();
    }
    else if (PochtiUgadal(mess, otvet))
    {
        string s = getFraza("41");
        (usericon == 0) ? replaceFirst(s, utf2acp("(а)"), "") : replaceFirst(s, utf2acp("(а)"), "a");
        MessageToChannel(GameChannel, strtows(user + ">" + s, CP_ACP));
    }
}

void adsuper()
{
    sqlite->execDML("update users set isadmin=0");
    string admins = GetConfigValue("admins");
    replace_with(admins, "'", "''");
    vector<string> v;
    string::size_type beg=0, end=0;
    while (beg!=string::npos && end!=string::npos)
    {
        beg = admins.find_first_not_of(",", end);
        if (beg==string::npos)
            break;
        end = admins.find_first_of(",", beg);
        if (end==string::npos)
        {
            v.push_back(trim(admins.substr(beg)));
            break;
        }
        else
            v.push_back(trim(admins.substr(beg, end-beg)));
    }

    sqlite->execDML("BEGIN;");
    for(unsigned int i=0; i < v.size(); ++i)
    {
        (getcount("users", "name", v.at(i))==0) ?
        sqlite->execDML("insert into users(name, isadmin) values ('"+acp2utf(v.at(i))+"', 1)") :
        sqlite->execDML("update users set isadmin=1 where name='"+acp2utf(v.at(i))+"'");
    }
    sqlite->execDML("COMMIT;");
    v.clear();

}


void adrangs()
{
    sqlite->execDML("delete from rangs");
    string line, ochki, name;
    string::size_type pos1, pos2;
    string sql="";
    ifstream myfile ((AskPluginTempPath()+rangs).c_str());
    if (!myfile.is_open()) return;
    sqlite->execDML("BEGIN;");
    while (!myfile.eof())
    {
        getline (myfile,line);
        line = trim(line);
        if (line.length()>0)
        {
            pos1 = line.find_first_of(" ");
            if (pos1!=string::npos)
            {
                ochki=line.substr(0, pos1);
                pos2 = line.find_first_not_of(" ", pos1);
                if (pos2!=string::npos)
                {
                    name = line.substr(pos2);
                    sql = "insert into rangs(ochki, name) values ("+ochki+", '"+acp2utf(name)+"');";
                    sqlite->execDML(sql);
                }
            }
        }
    }
    sqlite->execDML("COMMIT;");
    myfile.close();
}

void Clear_stat() //Очистка таблицы пользователей
{
    string current_date = formatDate("%Y-%m-%d");
    string best_user=getbest();
    replace_with(best_user, "'", "''");
    int count = getcount("archive", "data", current_date);
    string ochki = getfieldvalue("users", "ochki", "name", best_user);
    if (trim(ochki).length()==0) ochki="0";

    if (count==0)
        sqlite->execDML("insert into archive (best_user, ochki) values('"+acp2utf(best_user)+"', "+ochki+")");
    // else
    //   sqlite->execDML("update archive set best_user='"+acp2utf(best_user)+"', ochki="+ochki+" where  data='"+current_date+"'");

    sqlite->execDML("update users set vsego=0, otgadal=0, ochki=0, otvet_time=0");
    sqlite->execDML("update statistic set zadano=0, otgadano=0");
}

void ShowUserList(wstring User)
{
    wstring s = strtows("\nСейчас играют "+inttostr(CountInGame())+" чел.\n", CP_UTF8);
    int schet=1;
    string sql = "select name from users where in_game=1 order by name";
    CppSQLite3Query stmt = sqlite->execQuery(sql);
    while (!(stmt.eof()))
    {
        s += strtows(inttostr(schet)+": "+stmt.getStringField(0)+"\n", CP_UTF8);
        stmt.nextRow();
        schet++;
    }
    PrivateMessage(User, s);
}
