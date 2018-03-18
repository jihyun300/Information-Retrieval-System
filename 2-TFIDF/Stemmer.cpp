#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<cctype>
#include<algorithm>
#include"Indexing.h"
#include"Stemmer.h"
using namespace std;

#define TRUE 1
#define FALSE 0
//static char * b;       /* buffer for word to be stemmed */
//static int k,k0,j;     /* j is a general offset into the string */

/* cons(i) is TRUE <=> b[i] is a consonant. */

static int cons(int i)
{  switch (b[i])
{  case 'a': case 'e': case 'i': case 'o': case 'u': return FALSE;
case 'y': return (i==k0) ? TRUE : !cons(i-1);
default: return TRUE;
}
}


static int m()
{  int n = 0;
int i = k0;
while(TRUE)
{  if (i > j) return n;
if (! cons(i)) break; i++;
}
i++;
while(TRUE)
{  while(TRUE)
{  if (i > j) return n;
if (cons(i)) break;
i++;
}
i++;
n++;
while(TRUE)
{  if (i > j) return n;
if (! cons(i)) break;
i++;
}
i++;
}
}


static int vowelinstem()
{  int i; for (i = k0; i <= j; i++) if (! cons(i)) return TRUE;
return FALSE;
}


static int doublec(int j)
{  if (j < k0+1) return FALSE;
if (b[j] != b[j-1]) return FALSE;
return cons(j);
}


static int cvc(int i)
{  if (i < k0+2 || !cons(i) || cons(i-1) || !cons(i-2)) return FALSE;
{  int ch = b[i];
if (ch == 'w' || ch == 'x' || ch == 'y') return FALSE;
}
return TRUE;
}


static int ends(char * s)
{  int length = s[0];
if (s[length] != b[k]) return FALSE; /* tiny speed-up */
if (length > k-k0+1) return FALSE;
if (memcmp(b+k-length+1,s+1,length) != 0) return FALSE;
j = k-length;
return TRUE;
}


static void setto(char * s)
{  int length = s[0];
memmove(b+j+1,s+1,length);
k = j+length;
}


static void r(char * s) { if (m() > 0) setto(s); }


static void step0(){// 추가함
	
		if(ends("\02" "'s"))k-=2;
		else if(ends("\01" "'"))k-=1;
		else if(ends("\03" "'s'"))k-=3;
}
static void step1ab()
{  if (b[k] == 's')
{  if (ends("\04" "sses")) k -= 2; else
if (ends("\03" "ies")) setto("\01" "i"); else
	if (b[k-1] != 's'&&b[k-1]!='u') k--; //수정함:u
}
if (ends("\03" "eed")) { if (m() > 0) k--; } else
	if ((ends("\02" "ed") || ends("\03" "ing")) && vowelinstem())
	{  k = j;
if(ends("\1" "i")){if(m()==1)setto("\02" "ie");}else //ies->ie /ies->i추가함
	if (ends("\02" "at")) setto("\03" "ate"); else
		if (ends("\02" "bl")) setto("\03" "ble"); else
			if (ends("\02" "iz")) setto("\03" "ize"); else
				if (doublec(k))
				{  k--;
{  int ch = b[k];
if (ch == 'l' || ch == 's' || ch == 'z') k++;
}
}
				else if (m() == 1 && cvc(k)) setto("\01" "e");
}
}


static void step1c() { if (ends("\01" "y")){if(cons(k-1)) b[k] = 'i'; else if(!cons(k-1)||m()==1)b[k]='y'; }} //수정함! 자음y->i 아니거나 by같이 1개면 y그대로



static void step2() { switch (b[k-1])
{
case 'a': if (ends("\07" "ational")) { r("\03" "ate"); break; }
		  if (ends("\06" "tional")) { r("\04" "tion"); break; }
		  break;
case 'c': if (ends("\04" "enci")) { r("\04" "ence"); break; }
		  if (ends("\04" "anci")) { r("\04" "ance"); break; }
		  break;
case 'e': if (ends("\04" "izer")) { r("\03" "ize"); break; }
		  break;
case 'l': if (ends("\03" "bli")) { r("\03" "ble"); break; } /*-DEPARTURE-*/

		  /* To match the published algorithm, replace this line with
		  case 'l': if (ends("\04" "abli")) { r("\04" "able"); break; } */

		  if (ends("\04" "alli")) { r("\02" "al"); break; }
		  if (ends("\05" "entli")) { r("\03" "ent"); break; }
		  if (ends("\03" "eli")) { r("\01" "e"); break; }
		  if (ends("\05" "ousli")) { r("\03" "ous"); break; }
		  break;
case 'o': if (ends("\07" "ization")) { r("\03" "ize"); break; }
		  if (ends("\05" "ation")) { r("\03" "ate"); break; }
		  if (ends("\04" "ator")) { r("\03" "ate"); break; }
		  break;
case 's': if (ends("\05" "alism")) { r("\02" "al"); break; }
		  if (ends("\07" "iveness")) { r("\03" "ive"); break; }
		  if (ends("\07" "fulness")) { r("\03" "ful"); break; }
		  if (ends("\07" "ousness")) { r("\03" "ous"); break; }
		  break;
case 't': if (ends("\05" "aliti")) { r("\02" "al"); break; }
		  if (ends("\05" "iviti")) { r("\03" "ive"); break; }
		  if(ends("\07" "abiliti")){r("\07" "ability");break; }
		  if (ends("\06" "biliti")) { r("\03" "ble"); break; }
		  if (ends("\03" "bli")) { r("\03" "ble"); break; } //추가함
		  break;
case 'g': if (ends("\04" "logi")) { r("\03" "log"); break; } /*-DEPARTURE-*/

		  /* To match the published algorithm, delete this line */

} }

static void step3() { switch (b[k])
{
case 'e': if (ends("\05" "icate")) { r("\02" "ic"); break; }
		  if (ends("\05" "ative")) { r("\00" ""); break; }
		  if (ends("\05" "alize")) { r("\02" "al"); break; }
		  break;
case 'i': if (ends("\05" "iciti")) { r("\02" "ic"); break; }
		  break;
case 'l': if (ends("\04" "ical")) { r("\02" "ic"); break; }
		  if (ends("\03" "ful")) { r("\00" ""); break; }
		  if(ends("\03" "abl")) {k++;r("\04" "able"); break;} //추가함: ables->abl->able.
		  break;
case 's': if (ends("\04" "ness")) { r("\00" ""); break; }
		  break;
} }



static void step4()
{  switch (b[k-1])
{  case 'a': if (ends("\02" "al")) break; return;
case 'c': if (ends("\04" "ance")) break;
	if (ends("\04" "ence")) break; return;
case 'e': if (ends("\02" "er")) break; return;
case 'i': if (ends("\02" "ic")) break; return;
case 'l': if (ends("\04" "able")) break;
	if (ends("\04" "ible")) break; return;
case 'n': if (ends("\03" "ant")) break;
	if (ends("\05" "ement")) break;
	if (ends("\04" "ment")) break;
	if (ends("\03" "ent")) break; return;
case 'o': if (ends("\03" "ion") && j >= 0 && (b[j] == 's' || b[j] == 't')) break;
	if (ends("\02" "ou")) break; return;
	/* takes care of -ous */
case 's': if (ends("\03" "ism")) break; return;
case 't': if (ends("\03" "ate")) break;
	if (ends("\03" "iti")) break; return;
case 'u': if (ends("\03" "ous")) break; return;
case 'v': if (ends("\03" "ive")) break; return;
case 'z': if (ends("\03" "ize")) break; return;
default: return;
}
if (m() > 1) k = j;
}


static void step5()
{  j = k;
if (b[k] == 'e')
{  int a = m();
if (a > 1 || a == 1 && !cvc(k-1)) k--;
}
if (b[k] == 'l' && doublec(k) && m() > 1) k--;
}



int stem(char * p, int i, int j)
{  b = p; k = j; k0 = i; /* copy the parameters into statics */
if (k <= k0+1) return k; /*-DEPARTURE-*/


step0(); step1ab(); step1c(); step2(); step3(); step4(); step5();
return k;
}

/*--------------------stemmer definition ends here------------------------*/


/* for isupper, islower, tolower */

//static char* s;         /* a char * (=string) pointer; passed into b above */

//#define INC 50           /* size units in which s is increased */
//static int i_max = INC;  /* maximum offset in s */

//void increase_s()
//{  i_max += INC;
//{  char * new_s = (char *) malloc(i_max+1);
//{ int i; for (i = 0; i < i_max; i++) new_s[i] = s[i]; } /* copy across */
//free(s); s = new_s;
//}
//}


#define LETTER(ch) (isupper(ch) || islower(ch))

static void stemfile(string &originStr)
{ 	
	char s[60]={0,};
	int i=0;
	
	for(int w=0;w<(int)originStr.length();w++){ //받은 string중 순수문자만 s에 넣음
		if(LETTER(originStr[w])){
			s[i]=originStr[w];
			i++;
		}		
	}
	s[i]=0;//끝임을 알림
	if(i!=0){
		s[stem(s,0,i-1)+1]=0; //stem시작
		originStr = s; //stem된 string으로 바꿔줌
	}
	
}

int cnt=0; //헤더에 추가해줘야


string stopList[26][60];
void Tokenize( const std::string& inString, std::vector<std::string>& outTokenList, const std::string& inDelim );

void makeStopList();
//<HEAD>등 필요없는 부분 제거 + 불용어 제거하는 함수
int parsingText(string fileName){
	ifstream fin;

	string strLine;
	string tempLine;
	string allLine;
	string deleteList[25]={"<TEXT>","</TEXT>","<DOC>","<FILEID>","<FIRST>","<SECOND>","<DATELINE>","</DOC>","<BYLINE>","<NOTE>","<IN>","<SO>","<DD>","<LP>","<CO>","<G>","<AUTHOR>","<AN>","<RE>","<MS>","<NS>","<DOCID>","<DATE>","<DO>","<ST>"}; //시작이 이 문자면 그 줄 삭제할 것
	
	int i=0;
	int removed=0;
	std::string::size_type offset=0;
	vector<string> docTok_v; //한줄한줄 받은거
	

	fin.open(fileName);

//	foutTemp.open("ex.txt",ios::out);


	if(fin==NULL){
		cout<<"파일 열기 에러"<<endl;
		return 1;
	}

	while(getline(fin,strLine)){
		//strLine에 한줄 저장 
		removed=0;
		//꺽세 지우기 시작
		for(i=0;i<25;i++){
			if(strLine.find(deleteList[i])==0){
				//처음 시작 문자열이 삭제해야 할 문장의 문자열이라면
				strLine.clear();//문자열 삭제
				removed=1;
				break;
			}	
		}
		//꺽세 지우기 끝

		//불용어지우기 시작
		if(removed==0){
			if(strLine.find("<DOCNO>")==0){ //새 문서시작
				if(!docTok_v.empty()){
					//이제까지 들어간거 아예 바로 Tmap에 넣자
					for(int m=0;m<docTok_v.size();m++){
						if(docTok_v[m].find("wsj")==string::npos){
							stemfile(docTok_v[m]);		//Stemming!!!!!!!!!!!!!!!!!!!!!!!1
							if(docTok_v[m].size()>1&&m!=0){
								Tmap[Key(docTok_v[m],docTok_v[0])]++;
							}
						}
						
					}
					DidMap[docTok_v[0]]=++cnt;
				docTok_v.clear();
				vector<string>().swap(docTok_v);
				//후 하위벡터는 비워줌(다시 다음 문서들꺼 채울거니까ㅑ)
				}
				
			}
			//알파벳들 소문자화.
			transform(strLine.begin(),strLine.end(),strLine.begin(),tolower);

	
			//불용어 한줄 지우기 끝
			//이제 토큰화해서 vector에 넣어주어야.
			Tokenize(strLine,docTok_v," ");
		}
			
	}
	//한 파일 다 끝
	fin.close();
	//마지막 문서도 넣어줌
	for(int m=0;m<docTok_v.size();m++){
		if(docTok_v[m].find("wsj")==string::npos){
			stemfile(docTok_v[m]);
			if(docTok_v[m].size()>1&&m!=0){
				Tmap[Key(docTok_v[m],docTok_v[0])]++;
			}
		}
		
	}
	DidMap[docTok_v[0]]=++cnt;
	docTok_v.clear();
	vector<string>().swap(docTok_v);//후 하위벡터는 비워줌(다시 다음 문서들꺼 채울거니까ㅑ)
	//다끝내고 나왔으면 마지막 문서넣어주어야
		

	
//	foutTemp.close();

	return 0;
}

//알파벳 순으로 찾기위해 'a''b'..몇개씩 있는지 확인하는 함수
/*
int countStopWord(int* alpCnt){
	ifstream readList;
	string str;
	char firstLetter;
	int i,j;
	readList.open("stopword.txt");
	if(readList==NULL)
		return 2;
	while(getline(readList,str)){
		if(str!=""){
			firstLetter=str[0];
			i=firstLetter-'a';
			alpCnt[i]++;
		}
	}

	readList.close();
	return 0;

}*/

//몇개씩 있는지 확인했으면 그에 따라 이차원배열 만드는 함수.
void makeStopList(){
	ifstream readList;
	string str;
	char firstLetter;
	int i,j;
	int cntCurrent[26]={0,};
	readList.open("stopword.txt");
	while(getline(readList,str)){
		if(str!=""){
			firstLetter=str[0];
			i=firstLetter-'a';
			stopList[i][cntCurrent[i]++]=str;
		}
	}
	readList.close();
}

//파일 토큰화 하는 함수
void Tokenize( const std::string& inString, std::vector<std::string>& outTokenList, const std::string& inDelim )
{

	std::string::size_type beginIndex = 0;
	std::string::size_type endIndex = 0;

	while(true)
	{
		beginIndex = inString.find_first_not_of(inDelim, beginIndex );
		if( beginIndex == std::string::npos )
		{    break;    }

		endIndex = inString.find_first_of(inDelim, beginIndex );
		if( endIndex == std::string::npos )
		{    
			endIndex = inString.length();
		}
		std::string token = inString.substr( beginIndex, endIndex - beginIndex );
		//불용어 여부 검사 후 삭제 또는 push back
		
		char first=token[0];
		int firstAlp=first-'a';
		if(firstAlp>=0&&firstAlp<=25){ //문자면
			int isStopword=1;
			int i=0;
			while(stopList[firstAlp][i]!=""){ //그 시작 알파벳서부터 불용어 리스트 검사
				if(token==stopList[firstAlp][i]){ //불용어면
					isStopword=0;
					token.clear(); //지워
					break;
				}
				i++;
			}
			if(isStopword!=0) //불용어가 아니면
				outTokenList.push_back(token); //벡터에 넣어
		}
		else{ //문자가 아니면
			if(first='<') //처음 토큰이 꺽세면 
				token.clear(); //지워
			else
				outTokenList.push_back(token);
		}

		beginIndex = endIndex + 1;
	}
}