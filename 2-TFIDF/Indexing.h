#include<string>
#include<vector>
#include<map>
using namespace std;

typedef pair<string,string> Key;
typedef pair<int,string> docKey;
//typedef pair<int,int> newKey;
//extern map<newKey,int> newMap;
extern map<Key,int> Tmap;
extern map<string,int> Dmap;
//extern map<int,int> newDmap;
extern map<string,int> Cmap;
extern map<string,float> Wmap;
extern map<string,int> WidMap;//단어 아뒤정하는데
extern map<string,int> DidMap;//문서별 아뒤정하는데
extern map<docKey,int> DocMap;
extern void makeId();
extern void makeTF();
extern void makeDF();
extern int N;
extern void makeDenom();
extern void makeDoc_f();
extern void makeTerm_f();
extern void makeIndex_f();
//extern void newDF();
