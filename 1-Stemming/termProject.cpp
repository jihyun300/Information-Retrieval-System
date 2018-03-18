#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
using namespace std;

#define TRUE 1
#define FALSE 0

int parsingText();
int countStopWord(int* alpCnt);
void makeStopList();

vector<string> tokenized;
string** stopList;
void Tokenize( const std::string& inString, std::vector<std::string>& outTokenList, const std::string& inDelim );

//<HEAD>등 필요없는 부분 제거 + 불용어 제거하는 함수
int parsingText(string fileName){
	fstream fin;
	fstream foutTemp;
	string strLine;
	string tempLine;
	string allLine;
	string deleteList[10]={"<TEXT>","</TEXT>","<DOC>","<FILEID>","<FIRST>","<SECOND>","<DATELINE>","</DOC>","<BYLINE>","<NOTE>"}; //시작이 이 문자면 그 줄 삭제할 것
	int i=0;


	fin.open(fileName,ios::in);
	foutTemp.open("ex.txt",ios::out);


	if(fin==NULL){
		cout<<"파일 열기 에러"<<endl;
		return 1;
	}

	while(getline(fin,strLine)){
		//strLine에 한줄 저장 

		//수정
		for(i=0;i<10;i++){
			if(strLine.find(deleteList[i])==0){
				//처음 시작 문자열이 삭제해야 할 문장의 문자열이라면
				strLine.clear();//문자열 삭제	
				break;
			}	
		}
		if(strLine.find("<HEAD>")==0){
			if(strLine.rfind("</HEAD>")==strLine.length()-7)
				strLine.erase(strLine.length()-7,7);
			strLine.erase(0,6);							
		}		
		/*	else if(strLine.find("</HEAD>")==strLine.length()-7)
		strLine.erase(strLine.length()-7,7);*/
		else if(strLine.find("<DOCNO>")==0){
			strLine.erase(0,7);
			strLine.erase(strLine.length()-8,8);
		}
		else{
		}


		foutTemp<<strLine<<endl;
		//다음줄계속 저장		
	}



	fin.close();
	foutTemp.close();

	foutTemp.open("ex.txt",ios::in);
	while(getline(foutTemp,tempLine)){
		//하나로 퉁쳐서 읽기//토큰화 위해/
		allLine+=tempLine;
	}
	foutTemp.close();

	//파일을 스페이스 단위로 token화=>tokenized에 저장
	Tokenize(allLine,tokenized," ");

	remove("ex.txt");
	return 0;
}

//알파벳 순으로 찾기위해 'a''b'..몇개씩 있는지 확인하는 함수
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

}

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
	int offset='a'-'A';
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

		//대문자를 소문자로
		if(token[0]>='A' &&token[0]<='Z')
			token[0]+=offset;

		outTokenList.push_back(token);

		beginIndex = endIndex + 1;
	}
}


static char * b;       /* buffer for word to be stemmed */
static int k,k0,j;     /* j is a general offset into the string */

/* cons(i) is TRUE <=> b[i] is a consonant. */

static int cons(int i)
{  switch (b[i])
{  case 'a': case 'e': case 'i': case 'o': case 'u': return FALSE;
case 'y': return (i==k0) ? TRUE : !cons(i-1);
default: return TRUE;
}
}

/* m() measures the number of consonant sequences between k0 and j. if c is
a consonant sequence and v a vowel sequence, and <..> indicates arbitrary
presence,

<c><v>       gives 0
<c>vc<v>     gives 1
<c>vcvc<v>   gives 2
<c>vcvcvc<v> gives 3
....
*/

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

/* vowelinstem() is TRUE <=> k0,...j contains a vowel */

static int vowelinstem()
{  int i; for (i = k0; i <= j; i++) if (! cons(i)) return TRUE;
return FALSE;
}

/* doublec(j) is TRUE <=> j,(j-1) contain a double consonant. */

static int doublec(int j)
{  if (j < k0+1) return FALSE;
if (b[j] != b[j-1]) return FALSE;
return cons(j);
}

/* cvc(i) is TRUE <=> i-2,i-1,i has the form consonant - vowel - consonant
and also if the second c is not w,x or y. this is used when trying to
restore an e at the end of a short word. e.g.

cav(e), lov(e), hop(e), crim(e), but
snow, box, tray.

*/

static int cvc(int i)
{  if (i < k0+2 || !cons(i) || cons(i-1) || !cons(i-2)) return FALSE;
{  int ch = b[i];
if (ch == 'w' || ch == 'x' || ch == 'y') return FALSE;
}
return TRUE;
}

/* ends(s) is TRUE <=> k0,...k ends with the string s. */

static int ends(char * s)
{  int length = s[0];
if (s[length] != b[k]) return FALSE; /* tiny speed-up */
if (length > k-k0+1) return FALSE;
if (memcmp(b+k-length+1,s+1,length) != 0) return FALSE;
j = k-length;
return TRUE;
}

/* setto(s) sets (j+1),...k to the characters in the string s, readjusting
k. */

static void setto(char * s)
{  int length = s[0];
memmove(b+j+1,s+1,length);
k = j+length;
}

/* r(s) is used further down. */

static void r(char * s) { if (m() > 0) setto(s); }

/* step1ab() gets rid of plurals and -ed or -ing. e.g.

caresses  ->  caress
ponies    ->  poni
ties      ->  ti
caress    ->  caress
cats      ->  cat

feed      ->  feed
agreed    ->  agree
disabled  ->  disable

matting   ->  mat
mating    ->  mate
meeting   ->  meet
milling   ->  mill
messing   ->  mess

meetings  ->  meet

*/

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

/* step1c() turns terminal y to i when there is another vowel in the stem. */

static void step1c() { if (ends("\01" "y")){if(cons(k-1)) b[k] = 'i'; else if(!cons(k-1)||m()==1)b[k]='y'; }} //수정함! 자음y->i 아니거나 by같이 1개면 y그대로


/* step2() maps double suffices to single ones. so -ization ( = -ize plus
-ation) maps to -ize etc. note that the string before the suffix must give
m() > 0. */

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

/* step3() deals with -ic-, -full, -ness etc. similar strategy to step2. */

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

/* step4() takes off -ant, -ence etc., in context <c>vcvc<v>. */

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

/* step5() removes a final -e if m() > 1, and changes -ll to -l if
m() > 1. */

static void step5()
{  j = k;
if (b[k] == 'e')
{  int a = m();
if (a > 1 || a == 1 && !cvc(k-1)) k--;
}
if (b[k] == 'l' && doublec(k) && m() > 1) k--;
}

/* In stem(p,i,j), p is a char pointer, and the string to be stemmed is from
p[i] to p[j] inclusive. Typically i is zero and j is the offset to the last
character of a string, (p[j+1] == '\0'). The stemmer adjusts the
characters p[i] ... p[j] and returns the new end-point of the string, k.
Stemming never increases word length, so i <= k <= j. To turn the stemmer
into a module, declare 'stem' as extern, and delete the remainder of this
file.
*/

int stem(char * p, int i, int j)
{  b = p; k = j; k0 = i; /* copy the parameters into statics */
if (k <= k0+1) return k; /*-DEPARTURE-*/

/* With this line, strings of length 1 or 2 don't go through the
stemming process, although no mention is made of this in the
published algorithm. Remove the line to match the published
algorithm. */

step0(); step1ab(); step1c(); step2(); step3(); step4(); step5();
return k;
}

/*--------------------stemmer definition ends here------------------------*/

//#include <stdio.h>
//#include <stdlib.h>      /* for malloc, free */
//#include <ctype.h>     
#include<cctype>/* for isupper, islower, tolower */

static char * s;         /* a char * (=string) pointer; passed into b above */

#define INC 50           /* size units in which s is increased */
static int i_max = INC;  /* maximum offset in s */

void increase_s()
{  i_max += INC;
{  char * new_s = (char *) malloc(i_max+1);
{ int i; for (i = 0; i < i_max; i++) new_s[i] = s[i]; } /* copy across */
free(s); s = new_s;
}
}

#define LETTER(ch) (isupper(ch) || islower(ch))

static void stemfile(FILE * f,string fileName)
{ 
	const char* fileName_char=fileName.c_str();
	FILE* savefile=fopen(fileName_char,"w");

	//		savefile.open("AP880212");
	while(TRUE)
	{  int ch = getc(f);
	if (ch == EOF) return;
	if (LETTER(ch))
	{  int i = 0;
	while(TRUE)
	{  if (i == i_max) increase_s();

	ch = tolower(ch); /* forces lower case */

	s[i] = ch; i++;
	ch = getc(f);
	if (!LETTER(ch)) { ungetc(ch,f); break; }
	}
	s[stem(s,0,i-1)+1] = 0;
	/* the previous line calls the stemmer and uses its result to
	zero-terminate the string in s */
	//        printf("%s",s);
	fprintf(savefile,"%s ",s);
	//	 savefile<<s<<" ";
	}
	else fprintf(savefile,"%c",ch); //putchar(ch);
	}
	fclose(savefile);
	//	savefile.close();
}


int mainStemmer(string fileName){
	int parse_m;
	int cntAlp;
	int  alpCnt[26]={0,};//각 알파벳 얼만큼 나왔는지 확인.
	vector<string> vectemp;

//	string fileName;

	//parsing하는 함수
	parse_m=parsingText(fileName);
	if(parse_m==1) return 1;//에러

	//stopword알파벳별 몇개있는지확인
	cntAlp=countStopWord(alpCnt);
	if(cntAlp==2)return 1;

	//개수를 토대로 이차원배열 만드는 함수.
	stopList=new string*[26];
	for(int u=0;u<26;u++){
		stopList[u]=new string[alpCnt[u]];
		memset(stopList[u],0,sizeof(int)*alpCnt[u]);
	}
	makeStopList();
	int temp=tokenized.size();


	//불용어 제거 함수
	for(int i=0;i<temp;i++){
		if(i==temp-2){
			cout<<"불용어 제거 완료"<<endl;
			break;
		}
		char first=tokenized[i][0];
		int firstAlp=first-'a';//사전찾기
		for(int j=0;j<alpCnt[firstAlp];j++){
			//불용어 리스트 한 줄 읽기
			int stoplist=tokenized[i].compare(stopList[firstAlp][j]);

			if(stoplist==0){ //token이 stopword 리스트에 있을때
				//				cout<<tokenized[i]<<" ";
				//				tokenized.erase(tokenized.begin()+i); //넘 느려!!
				tokenized[i].clear(); //용량이 좀 걸려도 빠른게 좋다!
				break;
			}
		}

	}

	//Stemming밑물작업
	fstream beforeStemfile;//불용어 제거된 파일 저장

	beforeStemfile.open("beforeStem.txt",ios::out);
	for(int i=0;i<tokenized.size();i++){
		beforeStemfile<<tokenized[i]<<" ";
	}
	beforeStemfile.close();
	//"beforeStem.txt" 파일을 읽어서 stem한 후 기존 파일에 옮겨 담자.

	int i;
	s = (char *) malloc(i_max+1);

	FILE * f = fopen("beforeStem.txt","r");
	if (f == 0) {  exit(1); }
	stemfile(f,fileName);
	   remove("beforeStem.txt");
	free(s);

	tokenized.clear();
	//		savefile.close();
	for(int k=0;k<26;++k)
		delete[] stopList[k];
	delete[] stopList;

	return 0;
}
int main(){
	int theFileNumber; //볼 파일의 수
	int cntFile=0; //현재까지 읽은 파일의 수
	int stemmer_m;
	int fileCase;

	int cntFile_dayInt=12;//파일여러개 읽어야 하니까 cnt셈
	int cntFile_monthInt=02;
	int cntFile_yearInt=88;
	int inputfileCount;
	string cntFile_dayStr;
	string cntFile_monthStr;
	string cntFile_yearStr;
	//각 인티져 string으로 바꿔주기 위해
	stringstream sDay;
	stringstream sMonth;
	stringstream sYear;


	string file="ir_data\\AP880212";//읽을 파일명
	cout<<"처음으로 읽을 파일은 ir_data\\AP880212입니다.파일들의 위치를 확인해주세요!"<<endl;
	cout<<"stemming할 파일 개수를 입력하세요: ";
	cin>>theFileNumber;
	ifstream openInmain;
	
	
	while(cntFile<theFileNumber){
		openInmain.open(file);//열어봐!
		if(openInmain.is_open()){//열렸어?
			openInmain.close();//그럼 닫고,
			++cntFile;
			cout<<"------------"<<cntFile<<"번째 파일("<<file<<") stemming 시작------------"<<endl;
			stemmer_m=mainStemmer(file);//함수 실행해.
			cout<<"Stemming 완료"<<endl;
		}
		//다 실행했어?그럼 이제 다음 파일 수행해야지?
		
		if(cntFile_dayInt<=30) 	fileCase=0;//일바꾸기					
		else if(cntFile_dayInt>30 && cntFile_monthInt<12) fileCase=1;//월바꾸기
		else if(cntFile_dayInt==31 && cntFile_monthInt==12) fileCase=2;//년바꾸기

		//날짜 바꿔주는 코드
		switch(fileCase){
		case 0:
			++cntFile_dayInt; //날짜만 더해
			break;
		case 1://월바꾸기 + 1일로 바꿔줌
			cntFile_dayInt=1;
			++cntFile_monthInt;			
			break;
		case 2: //년 바꾸기+1월1일로 바꿔줌
			cntFile_dayInt=1;
			cntFile_monthInt=1;
			++cntFile_yearInt;
			break;
		default:
			break;
		}

		//파일 명에 넣기위해 작업하는 코드
		//1->01 2자리수에 넣기위해 앞에 0채움
		
			sDay<<cntFile_dayInt;
			sMonth<<cntFile_monthInt;
			sYear<<cntFile_yearInt;
			
			if(cntFile_dayInt<10)
				cntFile_dayStr="0"+sDay.str();
			else
				cntFile_dayStr=sDay.str();

			if(cntFile_monthInt<10)
				cntFile_monthStr="0"+sMonth.str();
			else
				cntFile_monthStr=sMonth.str();

			cntFile_yearStr=sYear.str();
	
			file="ir_data\\AP"+cntFile_yearStr+cntFile_monthStr+cntFile_dayStr;
			sDay.str("");
			sMonth.str("");
			sYear.str("");
	}
	return 0;
}