Information-Retrieval-System
================
쿼리가 주어졌을 때, 600MB의 영어 뉴스 기사 집합에서 각 topic에 맞는 적절한 문서들을 검색해 내는 시스템이다.
고전적인 모델인 TF-IDF와 더불어 Vector Space model, BM25 등을 구현하였다. 

#### Implementation
- Language: C++
- Tool: Visual Studio 2012

### Process
  [1. Stemming](#1-stemming)
 
  [2. Build Index Structure](#2-build-index-structure)
 
  [3. Implement Retrieval Model](#3-implement-retrieval-model)
 
  [4. Evaluate Search Engine](#4-evaluate-search-engine)
 
  [5. Tuning and Result](#5-tuning-and-result)
  
****
## Overview
<p align="center">
   <img src="screenshots/overview.PNG" width="60%"></img>
</p>

## 1. Stemming
영어 문서 검색 엔진 구현을 위해 주어진 파일에서 문서를 추출하여 stemming을 수행한다. 
먼저 기사를 추출하고, stopword를 제거한 후, stemming을 진행할 것이다.

본 프로젝트를 위해 주어진 dataset은 1988년부터 1990년까지의 AP 통신의 242,918개(약 600MB)의 뉴스 기사이다.
주어지는 형태는 아래 그림과 같고, 그림에서 붉은 색 글씨로 표시한 부분만이 유익한 정보를 담고있기 때문에, 본 부분만 추출해서 검색하는데 쓰인다.
<p align="center">
   <img src="screenshots/news.png" width="50%"></img>
</p>

### - 기사 추출
주어진 뉴스 기사 자료에서 문서 번호(DOCNO), 기사 제목(HEAD), 기사 내용(Text)을 추출하고, 나머지 불필요한 부분은 제거한다.

### - 불용어 제거
문서에서 빈번히 출현하는 단어(ex. is, the, a, ...)들은 주제와는 관련이 없다. 이러한 단어들을 불용어(stopword)라고 하며, 
뒤에서 인덱스 정보를 추출하기 이전에 불용어를 제거한다.

### - Stemmer 구현
추출된 각 기사에 있는 단어들을 어간만 남기고 나머지는 제거하는 것을 Stemming이라고 한다. 
한 단어가 단수형, 복수형, 과거형, 분사형 등 여러 형태로 나타나기 때문에 stemming을 통해 어간만을 남겨, 한 단어를 한가지 형태로 표현해줘야만 관련 문서를 찾을 때 정확도를 높일 수 있다.
본 프로젝트에서는 Porter stemmer를 기본적으로 이용하였지만, Porter stemmer의 단점을 개선하여 더 좋은 성능의 stemmer를 구현하였다.

> Stemmer 개선 방법
>	```
> porter2 stemmer를 참고하여, porter stemmer에서 처리되지 않았던 부분을 보완하였다.
> ex) y를 ie/i로 바꾸는 룰, us로 끝나는 단어에 대한 룰, ies->i/ie로 바꾸는 룰 등 추가
>	```

내가 구현한 stemmer의 결과의 일부는 아래 표에서 확인할 수 있다. 첫번째 행은 기사 추출 후 불용어을 제거했을 때의 모습이고, porter's stemmer를 적용하고 나면, 
두번째 행에서 밑줄 친 단어들처럼 어간만 남겨준다. 마지막 행은 내가 개선하여 적용하였을 때의 결과를 보이고 있다. 이전 방식에서 처리해주지 못했던 부분을 추가적으로
수정한 것을 확인할 수 있다.
<p align="center">
  <img src="screenshots/stemmer.png" width="70%"></img>
</p>

## 2. Build Index Structure
이제 전처리를 끝낸 기사의 색인 정보를 추출하고 색인 구조를 구축한다.

사용자가 요청한 정보(query)가 들어 있는 문서를 찾기 위해서는 각 정보가 어떤 문서에 들어 있는지를 미리 조사, 정리하는 
과정이 필요하다.

정보 검색에서는 일반적으로 정보의 단위가 단어이므로, 결국 각각의 단어가 출현한 문서가 어떤 것들이 있는지 조사하여 구조화하는
작업이 필요하다. 작업한 내용과, 만든 파일들은 아래와 같다. 구조화한 결과물은 단어정보파일, 문서정보파일, 그리고 역색인 파일이다.

#### - 색인어 및 문서의 ID 설정
역색인 파일에서 색인어 문자열ㅇ르 그대로 이용할 경우, 각 색인어마다 요구하는 메모리의 양이 다르기 대문에 파일 접근에 어려움이
있고, 글자 하나씩 모두 비교해야 하기 때문에 비효율적이다. 따라서, 나는 색인어와 문서 명에 대해 고정된 길이의 ID를 부여하였다.

#### - TF (Term Frequency)
어떤 문서에서 어떤 단어가 출현한 빈도를 기록한다.

ex) \<문서 번호\>\<단어\>\<출현빈도\>

#### - DF (Document Frequency)
특정 단어가 출현한 문서의 수를 기록한다.

ex) \<단어\>\<단어가 출현한 문서 수\>

#### - CF (Collection Frequency)
특정 단어가 전체 문서 집합에서 출현한 빈도를 기록한다.

ex) \<단어\>\<출현 빈도\>

#### - 단어정보파일
이는 파일로 저장하는데, 앞에서 소개한 DF와 CF를 이용하여 구축한다. 이 파일은 <2-TFIDF/Word.txt>에 해당한다.

형식: \<색인어 ID, 색인어, DF, CF, 역색인에서의 해당 색인어 시작 위치\>

#### - 문서정보파일
문서의 길이는 검색 모델에서 각 문서들을 표현하는 모델을 구하기 위해 필요하다. 이 파일은 <2-TFIDF/Doc.txt>에 해당한다.

형식: \<문서 ID, 문서 명, 문서 길이\>

#### - 역색인 파일
정보 검색 시스템에서 문서의 수가 계속해서 늘어남에 따라 역색인 데이터 역시 계속 증가하기 때문에 메모리에 저장할 수 없다.
따라서 역색인 데이터를 파일로 저장해두고, 검색할 때 파일을 참조해서 검색을 수행해야 한다.
시작 위치와 해당 색인어가 등장한 문서의 개수(DF), 그리고 역색인 정보에서 레코드 하나당 차지하는 byte개수를 알면, 전체 역색인
정보 파일에서 우리가 필요로 하는 색인어 정보 레코드 위치는 다음과 같이 알아낼 수 있다.

> 색인어 정보 레코드 위치
>   > ```
>   > 해당 역색인 정보가 저장되어 있는 파일 내부 위치= 역색인 정보 시작 위치+ (색인어 시작 위치 * 레코드 byte 수)
>   > ```
>   > ```
>   > 해당 역색인 정보가 저장되어 있는 파일 내부의 양= 해당 색인어의 DF * 레코드 byte 수
>   > ```

이를 토대로, TF, CF, DF, 문서 길이를 이용하여, 검색 엔진에서 사용할 수 있는 색인 파일을 만든다. 형식은 아래 그림과 같다.
<p align="center">
  <img src="screenshots/index.png" width="50%"></img>
</p>

형식에서 term weight란 각 문서에 나타난 단어들의 가중치를 의미하는데 이는 아래 TF-IDF계산식에 의해 구한다. 식의 구체적인 의미는 [여기] (https://en.wikipedia.org/wiki/Tf%E2%80%93idf)에서 소개된다.
<p align="center">
  <img src="screenshots/tfidf.png" width="50%"></img>
</p>

## 3. Implement Retrieval Model

## 4. Evaluate Search Engine

##5. Tuning and Result
