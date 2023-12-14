//
//  smm_database.h
//  infection Path
//
//  Created by Juyeop Kim on 2023/11/05.
//	Edited by Nayoun on 2023/12/14.

#ifndef smm_database_h
#define smm_database_h

// 데이터베이스 리스트 번호 정의 (해당하는 데이터베이스 리스트 번호를 정의하는 매크로 상수) 
#define LISTNO_NODE             0 	//노드
#define LISTNO_FOODCARD         1	//음식 카드 
#define LISTNO_FESTCARD         2	//축제 카드 
#define LISTNO_OFFSET_GRADE     3	//성적 등급 

// 데이터베이스에 데이터를 추가하는 함수
int smmdb_addTail(int list_nr, void* obj);          //add data to tail
// 데이터베이스에서 데이터를 삭제하는 함수
int smmdb_deleteData(int list_nr, int index);       //delete data
// 데이터베이스의 길이를 반환하는 함수
int smmdb_len(int list_nr);                   //get database length
// 데이터베이스에서 특정 인덱스의 데이터를 가져오는 함수
void* smmdb_getData(int list_nr, int index);        //get index'th data

#endif /* smm_database_h */
