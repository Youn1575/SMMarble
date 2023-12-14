//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//	Edited by Nayoun on 2023/12/14.

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

// �ִ� ��� Ÿ��, ���, ��� ���� ����
#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100

// ��� Ÿ�Կ� ���� �̸��� �����ϴ� 2���� �迭
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "����",
       "�Ĵ�",
       "�����",
       "��",
       "����Ƿ��̵�",
       "��������",
       "�����ð�"
};

// ��� Ÿ���� �������� �Լ�
char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}


// ��� ���(����)�� ��Ÿ���� ������
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

// ��带 ��Ÿ���� ����ü ����
//1. ����ü ���� ����
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


// ��ü ���� �Լ�
//3. ���� �Լ� ���� 
//object generation

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    // �޸� �Ҵ�
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

// ����� �̸��� �������� �Լ�
//3. ���� �Լ� ���� 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

// ����� Ÿ���� �������� �Լ�
//3. ���� �Լ� ���� 
int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}

// ����� ������ �������� �Լ�
int smmObj_getNodeCredit(int node_nr)
{
    return smm_node[node_nr].credit;
}

// ����� �������� �������� �Լ�
int smmObj_getNodeEnergy(int node_nr)
{
    return smm_node[node_nr].energy;
}
