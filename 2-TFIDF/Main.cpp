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

	string file="ir_data2\\1987\\WSJ87_001";//���� ���ϸ�
	cout<<"ó������ ���� ������ ir_data2\\1987\\WSJ87_001�Դϴ�.���ϵ��� ��ġ�� Ȯ�����ּ���!"<<endl;
		/////////////////////�ҿ���Ʈ �����////////////stopList[][]�� ����////////////////////


	//������ ���� �������迭 ����� �Լ�.
	makeStopList();
	cout<<"�ҿ���Ʈ �ٸ��������"<<endl;
	///////////////////////////�ҿ���Ʈ �� ����////////////////////////////////////////////
	ifstream openInmain;
	cout<<"Stemming �� TF���ϱ� �����մϴ�"<<endl;
	cout<<"87�⵵ ������ �б� �����մϴ�."<<endl;
	while(1){
		openInmain.open(file);
		if(year==92 && openInmain.is_open()==false){
			cout<<"Stemming �� TF���ϱ� �������ϴ�."<<endl;
			break;
		}
		else{
			
			if(openInmain.is_open()){
				openInmain.close();
				stemmer_m=parsingText(file);//Stemming!!!!//�� ���ϸ��� �ҷ����� stemming �� �� ����...
				
			}

			else{ //�عٲ���Ҷ�
				
				++year;
				sYear<<year;
				yearStr=sYear.str();
				filenum=0;
				cout<<year<<"�⵵ ������ �б� �����մϴ�."<<endl;
			}
			//���� �̸� ����
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

	//���� indexing����
	cout<<"Doc.txt���ϸ���� �����մϴ�."<<endl;
	makeDoc_f();
	cout<<"Doc.txt���� �ۼ��Ͽ����ϴ�."<<endl;
	cout<<"TF�۾��� �����մϴ�."<<endl;
//	makeTF();
	cout<<"TF�۾��� �Ϸ��Ͽ����ϴ�."<<endl;
	
	cout<<"DF&CF�۾��� �����մϴ�."<<endl;
	makeDF();
	cout<<"DF&CF�۾��� �Ϸ��Ͽ����ϴ�."<<endl;
	cout<<"Id���� �۾��� �����մϴ�."<<endl;
	makeId();
	cout<<"������ �ܾ���� Id ���� ��� ���������ϴ�"<<endl;
	cout<<"Word.txt���ϸ���� �����մϴ�."<<endl;
	makeTerm_f();
	cout<<"Word.txt���� �ۼ��Ͽ����ϴ�."<<endl;

	cout<<"Weight�� ���ϱ� ���� ������ �ʿ��� ������ �����ϴ� ���Դϴ�."<<endl;
	makeDenom();
	cout<<"�غ������ �������ϴ�.Index.txt������ ����� �����մϴ�."<<endl;
	makeIndex_f();
	cout<<"Index.txt������ �ۼ��Ͽ����ϴ�. ���α׷��� ����˴ϴ�."<<endl;
	
	printf("����ð�: %.2fs\n",(double)(clock()-tStart)/CLOCKS_PER_SEC);
	return 0;

}