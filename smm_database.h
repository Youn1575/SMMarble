//
//  smm_database.h
//  infection Path
//
//  Created by Juyeop Kim on 2023/11/05.
//	Edited by Nayoun on 2023/12/14.

#ifndef smm_database_h
#define smm_database_h

// �����ͺ��̽� ����Ʈ ��ȣ ���� (�ش��ϴ� �����ͺ��̽� ����Ʈ ��ȣ�� �����ϴ� ��ũ�� ���) 
#define LISTNO_NODE             0 	//���
#define LISTNO_FOODCARD         1	//���� ī�� 
#define LISTNO_FESTCARD         2	//���� ī�� 
#define LISTNO_OFFSET_GRADE     3	//���� ��� 

// �����ͺ��̽��� �����͸� �߰��ϴ� �Լ�
int smmdb_addTail(int list_nr, void* obj);          //add data to tail
// �����ͺ��̽����� �����͸� �����ϴ� �Լ�
int smmdb_deleteData(int list_nr, int index);       //delete data
// �����ͺ��̽��� ���̸� ��ȯ�ϴ� �Լ�
int smmdb_len(int list_nr);                   //get database length
// �����ͺ��̽����� Ư�� �ε����� �����͸� �������� �Լ�
void* smmdb_getData(int list_nr, int index);        //get index'th data

#endif /* smm_database_h */
