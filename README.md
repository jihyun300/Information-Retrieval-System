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
   <img src="screenshots/overview.PNG" width="80%"></img>
</p>

## 1. Stemming
영어 문서 검색 엔진 구현을 위해 주어진 파일에서 문서를 추출하여 stemming을 수행한다. 
먼저 기사를 추출하고, stopword를 제거한 후, stemming을 진행할 것이다.

본 프로젝트를 위해 주어진 dataset은 1988년부터 1990년까지의 AP 통신의 242,918개(약 600MB)의 뉴스 기사이다.
주어지는 형태는 아래 그림과 같고, 그림에서 붉은 색 글씨로 표시한 부분만이 유익한 정보를 담고있기 때문에, 본 부분만 추출해서 검색하는데 쓰인다.
<p align="center">
   <img src="screenshots/news.png" width="60%"></img>
</p>

### 기사 추출
주어진 뉴스 기사 자료에서 문서 번호(<DOCNO>), 기사 제목(<HEAD>), 기사 내용(<Text>)을 추출하고, 나머지 불필요한 부분은 제거한다.

### 불용어 제거
문서에서 빈번히 출현하는 단어(ex. is, the, a, ...)들은 주제와는 관련이 없다. 이러한 단어들을 불용어(stopword)라고 하며, 
뒤에서 인덱스 정보를 추출하기 이전에 불용어를 제거한다.

### Stemmer 구현
<p align="center">
  <img src="screenshots/stemmer.png" width="70%"></img>
</p>
추출된 각 기사에 있는 단어들을 어간만 남기고 나머지는 제거하는 것을 Stemming이라고 한다. 
한 단어가 단수형, 복수형, 과거형, 분사형 등 여러 형태로 나타나기 때문에 stemming을 통해 어간만을 남겨, 한 단어를 한가지 형태로 표현해줘야만 관련 문서를 찾을 때 정확도를 높일 수 있다.
본 프로젝트에서는 Porter stemmer를 기본적으로 이용하였지만, Porter stemmer의 단점을 개선하여 더 좋은 성능의 stemmer를 구현하였다.

> Stemmer 개선 방법
>	```
> porter2 stemmer를 참고하여, porter stemmer에서 처리되지 않았던 부분을 보완하였다.
> ex) y를 ie/i로 바꾸는 룰, us로 끝나는 단어에 대한 룰, ies->i/ie로 바꾸는 룰 등 추가
>	```

내가 구현한 stemmer의 결과의 일부는 위 표에서 확인할 수 있다. 첫번째 행은 기사 추출 후 불용어을 제거했을 때의 모습이고, porter's stemmer를 적용하고 나면, 
두번째 행에서 밑줄 친 단어들처럼 어간만 남겨준다. 마지막 행은 내가 개선하여 적용하였을 때의 결과를 보이고 있다. 이전 방식에서 처리해주지 못했던 부분을 추가적으로
수정한 것을 확인할 수 있다.

##2. Build Index Structure

##3. Implement Retrieval Model

##4. Evaluate Search Engine

##5. Tuning and Result
