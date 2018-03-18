#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
#include<string>
#include<algorithm>
#include<sstream>
#include<time.h>
#include<map>
#include<unordered_map>
#include<math.h>
#include <stdio.h>
using namespace std;

vector<string> docTok;
string stopList[26][70];
static void stemfile(string &originStr);
void makeStopList();
int VectorSpaceModel(vector<string> doc);

void ParsingLine( const std::string& inString, std::vector<std::string>& outTokenList, const std::string& inDelim );
typedef pair<int,int> wordValue;
map<string,wordValue> WordMap;

unordered_map<string,int> WordIDmap;
unordered_map<int,int> WordDFmap;
unordered_map<int,int> WordSTARTmap;
void WordMapize(){
	ifstream fword;
	
	fword.open("Word.dat");
	string oneline;
	while(getline(fword,oneline)){
				vector<string> vecSpltStrings; //��ūȭ �Ѱ� �����ϴ� ��
				istringstream streamize(oneline);
				string temp;//�� �ܾ �ӽ÷� ������ ��(���Ϳ� ����)
				while(getline(streamize,temp,'\t')){ //���� �� ������ �ڸ���
					vecSpltStrings.push_back(temp);
				}
				int wordId=atoi(vecSpltStrings[0].c_str());

				int wordDf=atoi(vecSpltStrings[2].c_str());
				int startPos=atoi(vecSpltStrings[4].c_str());
//				WordMap[vecSpltStrings[1]].first=wordDf;
//				WordMap[vecSpltStrings[1]].second=startPos;

				WordIDmap[vecSpltStrings[1]]=wordId;
				WordDFmap[wordId]=wordDf;
				WordSTARTmap[wordId]=startPos;
				vecSpltStrings.clear();
				
	}
	fword.close();
}
unordered_map<int,string> DocMap;
unordered_map<int,int>DocLenmap;
double avdl=0;
void DocMapize(){
	ifstream fdoc;
	fdoc.open("Doc.dat");
	string oneline;
	while(getline(fdoc,oneline)){
				vector<string> vecSplt; //��ūȭ �Ѱ� �����ϴ� ��
				istringstream mainL(oneline);
				string temp;//�� �ܾ �ӽ÷� ������ ��(���Ϳ� ����)
				while(getline(mainL,temp,'\t')){ //���� �� ������ �ڸ���
					vecSplt.push_back(temp);
				}
				int docNum=atoi(vecSplt[0].c_str());
				int docLen=atoi(vecSplt[2].c_str());
				transform(vecSplt[1].begin(),vecSplt[1].end(),vecSplt[1].begin(),toupper); //�빮�ڷ� �ٲ۴��� ����
				DocMap[docNum]=vecSplt[1];
				DocLenmap[docNum]=docLen;
				avdl+=docLen;
				vecSplt.clear();
	}
	fdoc.close();
	avdl=avdl/(double)173252;
}



ofstream top10_file;
ifstream indexTxt;
int querynum=0;
clock_t tStart;
int main(){
	int forTest=1;
	ifstream fin;
	string strLine;

	int isDesc=0;
	int isNarr=0;
	int isCon=0;
	
	//�ϴ� ���� �ҿ���Ʈ �����
	
	cout<<"----------�ҿ�� ����Ʈ �����-------------"<<endl;
	makeStopList();
	cout<<"----------Word.txt �� �����---------------"<<endl;
	WordMapize();
	cout<<"----------Doc.txt �� �����----------------"<<endl;
	DocMapize();




	fin.open("topics25.txt");
	indexTxt.open("Index.dat");
	top10_file.open("result.txt");
	if(!fin.is_open()){
		cout<<"topic25.txt���� ����"<<endl;
		return 1;	
	}
	tStart=clock(); //�ð� ��� ����
	while(getline(fin,strLine)){
		
		if(strLine.find("<title>")==0){
			
			//docTok�� �ִ� �� �� ���������� �ȴ�.����
			//���;��� ������ ���Ͽ� ���̵�ο�,���ù�������
			if(!docTok.empty()){
				cout<<"------------ �������� "<<querynum<<"--------------"<<endl;
				int vec=VectorSpaceModel(docTok); 
				top10_file<<setfill('0')<<setw(3)<<++querynum<<endl;
		
			}
			else{
			
					top10_file<<setfill('0')<<setw(3)<<++querynum<<endl;
		

			}
	
			docTok.clear();
			transform(strLine.begin(),strLine.end(),strLine.begin(),tolower); //���� �ҹ��ڷθ����
			ParsingLine(strLine,docTok," ");
			//�� �ٽ� �� ���� tokenize�ϰ� [2]���� docTok������
		}
		if(strLine.find("<desc>")==0){
			isDesc=1;
		}
		else if(strLine.find("<narr>")==0){
			isDesc=0;
			isNarr=1;
		}
		else if(strLine.find("<con>")==0){
			isNarr=0;
			isCon=1;
		}
		else if(strLine.find("<fac>")==0){
			isCon=0;
		}
		else if(isDesc==1 || isNarr==1 || isCon==1){//�־�� �ϴ� �����̸�
			transform(strLine.begin(),strLine.end(),strLine.begin(),tolower);//���� �ҹ��ڷ� �����
			//tokenize�ϰ� stemming�ϰ� docTok�� ����
			ParsingLine(strLine,docTok," ");
		}
	}
	//���������� docTok�� ����ִ��� �������
	cout<<"------------ �������� "<<querynum<<"--------------"<<endl;
	int last=VectorSpaceModel(docTok);
	fin.close();
	top10_file.close();
	indexTxt.close();
	printf("����ð�: %2fs\n",(double)(clock()-tStart)/CLOCKS_PER_SEC);
	return 0;
}

void makeStopList(){
	ifstream readList;
	string str;
	char firstLetter;
	int i;
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
void ParsingLine( const std::string& inString, std::vector<std::string>& outTokenList, const std::string& inDelim )
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
		//�ҿ�� ���� �˻� �� ���� �Ǵ� push back
		
		char first=token[0];
		int firstAlp=first-'a';
		if(firstAlp>=0&&firstAlp<=25){ //���ڸ�
			int isStopword=1;
			int i=0;
			while(stopList[firstAlp][i]!=""){ //�� ���� ���ĺ������� �ҿ�� ����Ʈ �˻�
				if(token==stopList[firstAlp][i]){ //�ҿ���
					isStopword=0;
					token.clear(); //����
					break;
				}
				i++;
			}
			if(isStopword!=0){ //�ҿ� �ƴϸ�
				if(token!="topic:"){
				stemfile(token); //���׹��ϰ�
				if(token.length()>2)//2���̻��̸�
					outTokenList.push_back(token); //���Ϳ� �־�
				}
			}
		}
		else{ //���ڰ� �ƴϸ� 
			//�� ����
			token.clear();
		
		}

		beginIndex = endIndex + 1;
	}
}

#define TRUE 1
#define FALSE 0
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


static void step0(){// �߰���
	
		if(ends("\02" "'s"))k-=2;
		else if(ends("\01" "'"))k-=1;
		else if(ends("\03" "'s'"))k-=3;
}
static void step1ab()
{  if (b[k] == 's')
{  if (ends("\04" "sses")) k -= 2; else
if (ends("\03" "ies")) setto("\01" "i"); else
	if (b[k-1] != 's'&&b[k-1]!='u') k--; //������:u
}
if (ends("\03" "eed")) { if (m() > 0) k--; } else
	if ((ends("\02" "ed") || ends("\03" "ing")) && vowelinstem())
	{  k = j;
if(ends("\1" "i")){if(m()==1)setto("\02" "ie");}else //ies->ie /ies->i�߰���
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


static void step1c() { if (ends("\01" "y")){if(cons(k-1)) b[k] = 'i'; else if(!cons(k-1)||m()==1)b[k]='y'; }} //������! ����y->i �ƴϰų� by���� 1���� y�״��



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
		  if (ends("\03" "bli")) { r("\03" "ble"); break; } //�߰���
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
		  if(ends("\03" "abl")) {k++;r("\04" "able"); break;} //�߰���: ables->abl->able.
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


#define LETTER(ch) (isupper(ch) || islower(ch))

static void stemfile(string &originStr)
{ 	
	char s[60]={0,};
	int i=0;
	
	for(int w=0;w<(int)originStr.length();w++){ //���� string�� �������ڸ� s�� ����
		if(LETTER(originStr[w])){
			s[i]=originStr[w];
			i++;
		}		
	}
	s[i]=0;//������ �˸�
	if(i!=0){
		s[stem(s,0,i-1)+1]=0; //stem����
		originStr = s; //stem�� string���� �ٲ���
	}
	
}
typedef pair<int,int>Key;

int VectorSpaceModel(vector<string> doc){ //�� ���������� ���ͽ����̽��� ����


	unordered_map<int,float> QtfMap;
	unordered_map<int,float>::iterator Qit;

	float querySize=0;
	int docLen=doc.size();
	unordered_map<int,double> BMmap;
	
	//Q_t�� ����� ����
	for(int i=0;i<docLen;i++){
		QtfMap[WordIDmap[doc[i]]]++;//�ܾ��� frequency������
	}
	int cntTitle=0;

	for(Qit=QtfMap.begin();Qit!=QtfMap.end();Qit++){
		Qit->second=log(Qit->second)+1;	

	}

	//Q_t�� �� �������+ �������� ũ�� ���ߴ�


	int N=173252; //�� ��������
	double k1=1.2;
	double K=0;
	double b=0.8;
	int k2=100;

	int df;
	int startLine;
	float qTF;

	int docTF;
	int docId;
	string input;

	for(Qit=QtfMap.begin();Qit!=QtfMap.end();Qit++){ //������ ��� �ܾ ����


		df=WordDFmap[Qit->first];
		startLine=25*WordSTARTmap[Qit->first];
		qTF=Qit->second;

		indexTxt.seekg(startLine);
				//index.txt���� �� ��ġ�� ���� 
				for(int i=0;i<df;i++){ //�� �ܾ ���� ����������ŭ ���ƾ�..
					
					getline(indexTxt, input);

					docId = atoi(input.substr(6, 6).c_str());
					docTF = atoi(input.substr(12, 4).c_str());
					//�迭�� �־�
					K=(double)k1*(1-b+b*(double)DocLenmap[docId]/avdl);
					BMmap[docId]+=log((double)(N-df+0.5)/(double)(df+0.5))*((double)((k1+1)*docTF)/(double)(K+docTF))*((double)((k2+1)*qTF)/(double)(k2+qTF));

				 //������ �о����					
				}
				
		
		//�ٽ� ó������ getline
	}

//���� ������ ��� �ܾ� ��


	QtfMap.clear();



	//��� �ܾ ���ؼ� DocWeight�迭 ����� ��
	//���� ����ϰ� Top10�߷�����..


	map<double,int> BMlist;
	unordered_map<int,double>::iterator BM_it;
	map<double,int>::reverse_iterator list_it;

	for(BM_it=BMmap.begin();BM_it!=BMmap.end();BM_it++){
		BMlist[BM_it->second]=BM_it->first;
	}
	int cnt=0;



	cout.precision(std::numeric_limits<double>::digits10+1);
		for(list_it=BMlist.rbegin();cnt<200;list_it++){

		if(cnt!=199)
			top10_file<<DocMap[list_it->second]<<"\t";//�����̸��ϰ� ����Ʈ�� ����
		else
			top10_file<<DocMap[list_it->second]<<endl;//�����̸��ϰ� ����Ʈ�� ����
		cnt++;
	}

	//���� �Ҵ��Ѻκ� ����
	BMmap.clear();
	BMlist.clear();
	return 0;
}
