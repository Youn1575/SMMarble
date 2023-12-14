//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//	Edited by Nayoun on 2023/12/14.

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

// 최대 노드 타입, 등급, 노드 개수 정의
#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100

// 노드 타입에 대한 이름을 저장하는 2차원 배열
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "강의",
       "식당",
       "실험실",
       "집",
       "실험실로이동",
       "음식찬스",
       "축제시간"
};

// 노드 타입을 가져오는 함수
char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}


// 노드 등급(성적)을 나타내는 열거형
typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;

// 노드를 나타내는 구조체 정의
//1. 구조체 형식 정의
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;


// 객체 생성 함수
//3. 관련 함수 변경 
//object generation

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    // 메모리 할당
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

// 노드의 이름을 가져오는 함수
//3. 관련 함수 변경 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

// 노드의 타입을 가져오는 함수
//3. 관련 함수 변경 
int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}

// 노드의 학점을 가져오는 함수
int smmObj_getNodeCredit(int node_nr)
{
    return smm_node[node_nr].credit;
}

// 노드의 에너지를 가져오는 함수
int smmObj_getNodeEnergy(int node_nr)
{
    return smm_node[node_nr].energy;
}
