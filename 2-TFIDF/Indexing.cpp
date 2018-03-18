#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<math.h>
#include<map>
#include<sstream>
#include<iomanip>
#include"Indexing.h"
#include"Stemmer.h"
using namespace std;

int N;
typedef pair<string,string> Key;
typedef pair<int,string> docKey;
//typedef pair<int,int> newKey;
//map<newKey,int> newMap;
map<Key,int> Tmap;
map<string,int> Dmap;
//map<int,int> newDmap;
map<string,int> Cmap;
map<string,float> Wmap;
map<string,int> WidMap;//�ܾ� �Ƶ����ϴµ�
map<string,int> DidMap;//������ �Ƶ����ϴµ�
//map<string,int> DocMap;//Doc���� ����� ��
map<docKey,int> DocMap;

//////////////////////�츮��//////////////////////////////////////////////////
//<����id>	<������>	<��������>
void makeDoc_f(){
	ofstream docFile;
	unsigned int i;
	docFile.open("Doc.txt");

	map<Key,int>::iterator itDoc;
	map<docKey,int>::iterator ittemp;
	for(itDoc=Tmap.begin();itDoc!=Tmap.end();itDoc++){
		DocMap[docKey(DidMap[itDoc->first.second],itDoc->first.second)]+=Tmap[Key(itDoc->first.first,itDoc->first.second)];
	}
	N=DidMap.size();//���� �Ѱ���

	//Doc.txt���
	for(ittemp=DocMap.begin();ittemp!=DocMap.end();ittemp++)
		docFile<<ittemp->first.first<<"\t"<<ittemp->first.second<<"\t"<<ittemp->second<<endl;
	docFile.close();
	DocMap.clear();
}
///////////////////////�츮��///////////////////////////////////////////////////


/*
void makeTF(){
	
	
	for(unsigned int i=0;i<docNum_v.size();i++){
		for(unsigned int j=1;j<docNum_v[i].size();j++){
			if(docNum_v[i][j].size()>1)
				Tmap[Key(docNum_v[i][j],docNum_v[i][0])]++; //key�� ������ value ������
		//�� �������� �� �ܾ ���� Ƚ���� value;
		
		}
		

	}
	for(unsigned int i=0;i<docNum_v.size();i++){
		for(unsigned int j=0;j<docNum_v[i].size();j++){
			docNum_v[i][j].clear();
		}
		docNum_v[i].clear();
	}
	docNum_v.clear(); //vector�� ���� �Ⱦ��Ŵϱ� ���֢a

	
}*/

//DF�� CF����
void makeDF(){
		map<Key,int>::iterator it;
	for(it=Tmap.begin();it!=Tmap.end();it++){
		Dmap[it->first.first]++; //�� �ܾ ���� ������ ����ŭ ������
		Cmap[it->first.first]+=it->second; //�� �ܾ ���� �������� �� �ܾ ������Դ��� �˼������ϱ� ������
	}
	
}
/*
void newDF(){
	map<string,int>::iterator it;
	for(it=Dmap.begin();it!=Dmap.end();it++){
		newDmap[WidMap[it->first]]=it->second; //�ܾ �ܾ��� ���̵�� �ٲ��� why?weight���� �ʿ�
	}
	Dmap.clear();
}*/

void makeId(){

	map<string,int>::iterator itD;
	int wid=0;


	for(itD=Dmap.begin();itD!=Dmap.end();itD++){
		if(WidMap[itD->first]==0) //�ܾ� id������ id�� 1����
			WidMap[itD->first]=++wid;
	}
}
void makeTerm_f(){
	ofstream fout;
	fout.open("Word.txt");
	map<string,int>::iterator it_d;
	map<string,int>::iterator it_c;
	int id=0;
	it_c=Cmap.begin();
	int startPoint=0;
	for(it_d=Dmap.begin();it_d!=Dmap.end();it_d++){
		fout<<WidMap[it_d->first]<<"\t"<<it_d->first<<"\t"<<it_d->second<<"\t"<<it_c->second<<"\t"<<startPoint<<endl;
		it_c++;
		startPoint+=it_d->second;
	}
	Cmap.clear();
	fout.close();
}
void makeDenom(){

	map<string,float>::iterator it_w;
	map<Key,int>::iterator it_t;
	for(it_t=Tmap.begin();it_t!=Tmap.end();it_t++){//��� ���� //it_t->first�� ������ ���� ����
		Wmap[it_t->first.second]+=pow((log10((float)it_t->second)+1.0)*log10((float)N/(float)Dmap[it_t->first.first]),2);
		//�ϴ� ���� ������ ���� �� ���� �ϰ��� ��Ʈ����ɷ� �ٲ���.
	}
	for(it_w=Wmap.begin();it_w!=Wmap.end();it_w++){
		it_w->second=pow(it_w->second,0.5f);
		
	}

}


void makeIndex_f(){
	
	
	ofstream fout;
	fout.open("Index.txt");

	map<Key,int>::iterator it_t;
	float weight;
	

	for(it_t=Tmap.begin();it_t!=Tmap.end();it_t++){
		weight=(log10((float)it_t->second)+1)*(log10((float)N/(float)Dmap[it_t->first.first]))/Wmap[it_t->first.second];
	
		fout<<setfill('0')<<setw(6)<<WidMap[it_t->first.first];
		fout<<setw(6)<<setfill('0')<<DidMap[it_t->first.second];
		fout<<setw(4)<<setfill('0')<<it_t->second;
		fout<<fixed<<setprecision(5)<<setfill('0')<<weight<<endl;
	//	fout<<WidMap[it_t->first.first]<<"\t"<<DidMap[it_t->first.second]<<"\t"<<it_t->second<<"\t"<<weight<<endl;
	
	}


	//<�ܾ�id> <����id> <df> <weight>
	fout.close();
}

