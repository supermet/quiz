#ifndef FUNC_H__
#define FUNC_H__
#include <windows.h>
//#include <process.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <io.h>
#include <time.h>
#include <math.h>
#include "map"
#include "CppSQLite3.h"

#define BUFSIZE MAX_PATH

#define inifile "viktorina\\config.ini"
#define database "viktorina\\base.db"
#define questions "viktorina\\questions.txt"
#define rangs "viktorina\\rangs.txt"
#define errorfile "viktorina\\error.log"

using namespace std;

//-----------------
void ConnectUser(wstring Name, wstring ip, wstring password, int icon);
void InviteUser(wstring Chanel);
void PersonalMessage (wstring Poluchatel, wstring text, byte vazhn);
void MessageToChannel (wstring Chanel, wstring Message);
void PrivateMessage (wstring Recipient, wstring Message);
void ChangeTheme (wstring Chanel, wstring Theme);
void ChangeGreeting (wstring Chanel, wstring Greeting);
void ChangeState (wstring State);
string AskPluginTempPath();
int AskProgrammType();
string GetBotName();
//-----------------


//-----------------------------------------------------
int fReadInteger(BYTE * bInBuffer, int * iOffset);
wstring fReadText(BYTE * bInBuffer, int * iOffset);
void fWriteInteger(BYTE * bOutBuffer, int * iOffset, int iValue);
void fWriteText(BYTE * bOutBuffer, int * iOffset, wstring uValue);
string fTextTostring(wstring uText);
string fIntegerTostring(int iValue);
//-----------------------------------------------------
std::wstring strtows(const std::string str, UINT codePage);
std::string wstostr(const std::wstring ws, UINT codePage);
std::string chcp(const std::string str, UINT codePageSrc, UINT codePageDst);
string utf2acp(string src);
string acp2utf(string src);
std::string trim(std::string str);
wstring wtrim(wstring str);
std::string toLowerCase(std::string str);
std::string toUpperCase(std::string str);
void replace_with(std::string & src, const std::string & what, const std::string & with);
void wreplace_with(std::wstring & src, const std::wstring & what, const std::wstring & with);
string inttostr(int a);
int strtoint(string s);
string GetParam(wstring txt);
int countchar(string st, char ch);
wstring ReturnApostrophe(wstring Text);

unsigned int getrandom(unsigned int min, unsigned int max);
double Round (double x);

string current_dir();
void LoadConfig();
bool fExists(const string&filename);
string GetConfigValue(string key);
wstring GetConfigValW(string key);

void LoadQuestions();
void dllCreate();
void dllDestroy();

void SetAnswerQuestion();
void show_next_question();

int getcount(string tab, string keyfield, string value);
string getFraza(string key);
void SetPodskazka();
bool IsAdmin(string user);
void startgame(string user);
void stopgame();
void CheckAnswer(string mess, string user, byte usericon);
void super_start();
void next_command(string user);
void showtop (int n, string to, bool privat);
void PersonalStatistic(string user, string to);

void adsuper();
void adrangs();
void Clear_stat();
void ShowUserList(wstring User);
void SwitchMessage(wstring usenderLogin, wstring uText, byte iUserIcon);
void CheckActivity();
int CountWantGame();
int CountInGame();

//struct UserList
//{
//    bool InGame;
//    int LastAnswer;
//    bool WantGame;
//};
//bool findUser(string user, UserList &User);

#endif
