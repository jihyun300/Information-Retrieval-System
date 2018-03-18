#include<fstream>
#include<string>
using namespace std;

static char *b;
static int k,k0,j;
static int cons(int i);
static int m();
static int vowelinstem();
static int doublec(int j);
static int cvc(int i);
static int ends(char* s);
static void setto(char* s);
static void r(char *s);
static void step0();
static void step1ab();
static void step1c();
static void step2();
static void step3();
static void step4();
static void step5();
extern int stem(char *p,int i,int j);
//static char *s;
static int i_max;
extern void increase_s();
static void stemfile(string &originStr);
//extern int mainStemmer(string fileName);
extern int cnt;
extern vector<string> docTok_v; //한줄한줄 받은거
extern string stopList[26][60];
extern int parsingText(string fileName);
extern void makeStopList();


extern void Tokenize( const std::string& inString, std::vector<std::string>& outTokenList, const std::string& inDelim );
