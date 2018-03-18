#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<time.h>
#include"Indexing.h"
#include"Stemmer.h"
using namespace std;

int main(){
	clock_t tStart=clock();
	int stemmer_m;

	int year=87;
	int filenum=1;
	string filenumStr;
	string yearStr="87";
	stringstream sYear;
	stringstream sNum;

	string file="ir_data2\\1987\\WSJ87_001";//읽을 파일명
	cout<<"처음으로 읽을 파일은 ir_data2\\1987\\WSJ87_001입니다.파일들의 위치를 확인해주세요!"<<endl;
		/////////////////////불용어리스트 만들기////////////stopList[][]에 저장////////////////////


	//개수를 토대로 이차원배열 만드는 함수.
	makeStopList();
	cout<<"불용어리스트 다만들었지요"<<endl;
	///////////////////////////불용어리스트 다 만듬////////////////////////////////////////////
	ifstream openInmain;
	cout<<"Stemming 및 TF구하기 시작합니다"<<endl;
	cout<<"87년도 폴더를 읽기 시작합니다."<<endl;
	while(1){
		openInmain.open(file);
		if(year==92 && openInmain.is_open()==false){
			cout<<"Stemming 및 TF구하기 끝났습니다."<<endl;
			break;
		}
		else{
			
			if(openInmain.is_open()){
				openInmain.close();
				stemmer_m=parsingText(file);//Stemming!!!!//한 파일마다 불러오고 stemming 그 안 문서...
				
			}

			else{ //해바뀌여야할때
				
				++year;
				sYear<<year;
				yearStr=sYear.str();
				filenum=0;
				cout<<year<<"년도 폴더를 읽기 시작합니다."<<endl;
			}
			//파일 이름 변경
			++filenum;

			sNum<<filenum;
			if(filenum<10)
				filenumStr="00"+sNum.str();
			else if(filenum>=10 && filenum<100)
				filenumStr="0"+sNum.str();
			else
				filenumStr=sNum.str();
			file="ir_data2\\19"+yearStr+"\\WSJ"+yearStr+"_"+filenumStr;
			sYear.str("");
			sNum.str("");
			
		}
	}

	//이제 indexing시작
	cout<<"Doc.txt파일만들기 시작합니다."<<endl;
	makeDoc_f();
	cout<<"Doc.txt파일 작성하였습니다."<<endl;
	cout<<"TF작업을 시작합니다."<<endl;
//	makeTF();
	cout<<"TF작업을 완료하였습니다."<<endl;
	
	cout<<"DF&CF작업을 시작합니다."<<endl;
	makeDF();
	cout<<"DF&CF작업을 완료하였습니다."<<endl;
	cout<<"Id결정 작업을 시작합니다."<<endl;
	makeId();
	cout<<"문서와 단어들의 Id 값이 모두 정해졌습니다"<<endl;
	cout<<"Word.txt파일만들기 시작합니다."<<endl;
	makeTerm_f();
	cout<<"Word.txt파일 작성하였습니다."<<endl;

	cout<<"Weight를 구하기 위해 문서별 필요한 정보를 저장하는 중입니다."<<endl;
	makeDenom();
	cout<<"준비과정이 끝났습니다.Index.txt파일을 만들기 시작합니다."<<endl;
	makeIndex_f();
	cout<<"Index.txt파일을 작성하였습니다. 프로그램이 종료됩니다."<<endl;
	
	printf("실행시간: %.2fs\n",(double)(clock()-tStart)/CLOCKS_PER_SEC);
	return 0;

}