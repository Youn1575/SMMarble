//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//	Edited by Nayoun on 2023/12/19.

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"


//board configuration parameters (���� ���� �Ű�����)
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

// �÷��̾� ����ü ����
typedef struct player {
        int energy;
        int position;
        char name[MAX_CHARNAME];
        int accumCredit;
        int flag_graduate;
} player_t;

static player_t *cur_player;
//static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes(�Լ� ������Ÿ��)
#if 0
int isGraduated(void); //check if any player is graduated
 //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif

// �÷��̾��� ��� ���� ���
void printGrades(int player)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
     }
}

// ������ ������� �� ��������� ¥����. printGrades�� �����Ͽ� �Լ� �ۼ��� ��
 

// ��� �÷��̾��� ���� ���
void printPlayerStatus(void)
{
     int i;
     
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n", 
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
}

/////////////////////////////////////////////////////////////

// ������ ������ �ڵ��� �Ϻκ�

void startGame() {
    
    printf("\n\n=======================================================================\n");
    printf("-----------------------------------------------------------------------\n");
    printf("                Sookmyung Marble !! Let's Graduate (total credit : %d)!!\n", GRADUATE_CREDIT);
    printf("-----------------------------------------------------------------------\n");
    printf("=======================================================================\n\n");

    int numPlayers;
    do {
        printf("Input No. of players (1 ~ %d) :", MAX_PLAYER);
        scanf("%d", &numPlayers);
        fflush(stdin);
    } while (numPlayers <= 0 || numPlayers > MAX_PLAYER);

    // �÷��̾� ����
    generatePlayers(numPlayers, initEnergy);

    // ���� ���� ����
    int turn = 0;
    while (1) {
        printf("========================== PLAYER STATUS ==========================\n");
        printPlayerStatus();

        printf("\nThis is %s's turn :::: Press any key to roll a die (press g to see grade): ", cur_player[turn].name);
        char c;
        scanf(" %c", &c);
        fflush(stdin);

        if (c == 'g') {
            printf("--> result : %d\n", 1); // ���÷� �ֻ��� ����� 1�� ����
            printGrades(turn);
        } else {
            int die_result = rolldie(turn);
            printf("--> result : %d\n", die_result);

            goForward(turn, die_result);
            actionNode(turn);

            // ���� ���� üũ
            if (cur_player[turn].accumCredit >= GRADUATE_CREDIT && cur_player[turn].position == 0) {
                printf("\nCongratulations! %s has graduated!\n", cur_player[turn].name);
                printGrades(turn);
                exit(0);
            }

            // ���� ��
            turn = (turn + 1) % numPlayers;
        }
    }
}
/*
// ���� �� ���¿��� �ֻ����� ���� ������ �õ��ϴ� �Լ�
void attemptExperiment(int player)
		{
   		// ���� ���� ���ذ��� 1���� 6������ ������ ���ڷ� ����
    	int experimentSuccessThreshold = rand() % 6 + 1;
    	
		// �ֻ��� ������ ���� ���� ���� Ȯ��
  		int experimentResult = rolldie(player);

		    // ���� ���� ���ο� ���� ó��
   			if (experimentResult >= experimentSuccessThreshold)
   			 {
        		printf("%s successfully completed the experiment!\n", cur_player[player].name);
        		// ������ �����ϸ� ���� �� ���¸� �����ϰ� �̵�
        		isExperimenting = 0;
        		goForward(player, 1); // ���� ���� �� 1ĭ ����
    		}
    		else
    		{
        		printf("%s's experiment failed. Staying in the laboratory.\n", cur_player[player].name);
        		// ���� ���� ��, �̵����� ���ϰ� ���� �� ���·� �ӹ���
       			printf("Energy before the failed experiment: %d\n", cur_player[player].energy);
        		cur_player[player].energy -= smmObj_getNodeEnergy(boardObj);
        		printf("Energy after the failed experiment: %d\n", cur_player[player].energy);
   			}
			}

				// ���� �� �������� Ȯ���ϰ� ������ �õ��ϴ� �κ�
				if (isExperimenting)
				{
    			// ������ �õ��ϰ� ���� �� ���¸� �����ϸ鼭 �̵�
    			attemptExperiment(turn);
				}
				else
				{
    			// ���� ���� �ƴ� ��쿡�� �Ϲ����� �ֻ��� ������ ó��
			    die_result = rolldie(turn);
				}
*/


///////////////////////////////////////////////////////////////////



// �ʱ� �������� �÷��̾ ����
void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name:", i); //��E���� ���硾�� 
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position
         //player_position[i] = 0;
         cur_player[i].position = 0;
         
         //set energy
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0;
     }
}

// �ֻ����� ���� ����� ��ȯ
int rolldie(int player)
{
    char c; 
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}

//action code when a player stays at a node(��忡 �ӹ��� ���� ������ ����)
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType(boardPtr);
    
    char *name = smmObj_getNodeName(boardPtr);
    void *gradePtr;
    
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE: //���� ĭ  
        
            // if �������� ���� �� ���Ǹ� ���� �� ���� ���� ���ǹ� �ޱ�  
            cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr); 
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
        
        
        //grade �����ؾ���
		//gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, smmObjGrade_e);
		//gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, smmObjGrade);        

	smmObjGrade_e randomGrade = rand() % (SMMNODE_TYPE_MAX - SMMNODE_TYPE_LECTURE) + SMMNODE_TYPE_LECTURE;
	gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, randomGrade);



		//linkedList�� ����ִ� �Լ�, smmdb_addTail �̴�.
        smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
		//Linked List�� Ȱ���Ͽ� ������ ������ ����ϰų�, ������ �����ߴ� �����̳ĸ� �˻��Ѵٰų� �� �� �ִ�.
		//getdata �Լ��� Ȱ��.  
        break;
           	
           
        #if 0 
           
		 case SMMNODE_TYPE_RESTAURANT:   
		 	cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
		 	
		 	
		case SMMNODE_TYPE_HOME:    //credit:0, energy:18 
		 	cur_player[player].energy += 18;
		  
		case SMMNODE_TYPE_LABORATORY:
  		  // ���� : ������ ���·� ��ȯ�Ǹ鼭 ����Ƿ� �̵� (�ֻ��� �� �������� ���� ���� ���ذ��� �������� ����.)
    		if (cur_player[player].energy >= 3) {
        		cur_player[player].energy -= 3;  // ���� �õ����� 3 ������ �Ҹ�
        		int diceResult = rolldie(player);
       			int experimentSuccessThreshold = rand() % 6 + 1;  // ���� ���� ���ذ��� 1���� 6������ ���� ������ ����

        printf("%s is conducting an experiment in the laboratory. Rolled a die: %d\n", cur_player[player].name, diceResult);

        if (diceResult >= experimentSuccessThreshold) {
            // ������ ������ ���
            printf("Experiment successful! %s completed the experiment.\n", cur_player[player].name);
            // ���⿡�� ������ �������� ���� �߰� ������ �ۼ��� �� ����
        }
            
        else {
            // ������ ������ ���
            printf("Experiment failed! %s couldn't complete the experiment.\n", cur_player[player].name);
            // ���⿡�� ������ �������� ���� �߰� ������ �ۼ��� �� ����
        }
    } 
	else {
        printf("%s doesn't have enough energy to conduct an experiment.\n", cur_player[player].name);
    }
    break;

		case SMMNODE_TYPE_GOTOLAB:
   		// ����� : ������ ���¸� �ֻ����� ������ ������ ������ ���� ���� ���ذ� �̻��� ������ ������ ����ǰ�, �׷��� ������ �̵����� ���ϰ� ������ ���·� �ӹ���.
   			if (cur_player[player].energy >= 3) {
        	cur_player[player].energy -= 3;  // ���� �õ����� 3 ������ �Ҹ�
        	int diceResult = rolldie(player);
        	int experimentSuccessThreshold = rand() % 6 + 1;  // ���� ���� ���ذ��� 1���� 6������ ���� ������ ����

        printf("%s is attempting to go to the laboratory. Rolled a die: %d\n", cur_player[player].name, diceResult);

        if (diceResult >= experimentSuccessThreshold) {
            // ������ ������ ���
            printf("Arrived at the laboratory and successfully started an experiment.\n");
        } 
		else {
            // ������ ������ ���
            printf("Failed to reach the laboratory or start an experiment.\n");
        }
    } 
	else {
        printf("%s doesn't have enough energy to attempt to go to the laboratory.\n", cur_player[player].name);
    }
    break;		
		 
		 case SMMNODE_TYPE_FOODCHANCE:    
    	{
        // ���� ���� : marbleFoodConfig.txt ���Ͽ��� �������� ������ �����ϰ� �ش� �������� ���� �������� ����
        FILE *foodFile = fopen("marbleFoodConfig.txt", "r"); //read���� �о�ͼ� ���Ͽ��� 
        if (foodFile == NULL) {
            printf("[ERROR] Failed to open marbleFoodConfig.txt.\n"); //���� ���� ���� 
            exit(EXIT_FAILURE);
        }

        // ���Ͽ��� �������� ���� ����
        char foodName[MAX_CHARNAME];
        int foodEnergy;
        int totalFoodCount = 0;

        // ���Ͽ��� �� ������ ���� �ľ�
        while (fscanf(foodFile, "%s %d", foodName, &foodEnergy) == 2) {
            totalFoodCount++;
        }

        // �������� ���õ� ������ ��ȣ ����
        int randomFoodIndex = rand() % totalFoodCount;

        // ���� �����͸� �ٽ� ó������ �ǵ����� ���õ� ���ı��� ����
        fseek(foodFile, 0, SEEK_SET);
        for (int i = 0; i < randomFoodIndex; i++) {
            fscanf(foodFile, "%s %d", foodName, &foodEnergy);
        }

        // ���õ� ������ �������� ���� �������� ����
        cur_player[player].energy += foodEnergy;

        printf("%s picked a food chance card and gained %d energy from %s.\n", cur_player[player].name, foodEnergy, foodName);

        fclose(foodFile);
    	}
    	break;


		 
		 case SMMNODE_TYPE_FESTIVAL: 
		 {
	    FILE *f = fopen("marbleFestivalConfig.txt", "r"); //����ī�� ���� �б� 
    		if (!f) {
     	   	printf("[ERROR] Failed to open marbleFestivalConfig.txt.\n"); //���� �������� ��� ��� 
   		    exit(EXIT_FAILURE);
  	  		}

    		char mission[MAX_CHARNAME];
    		int c = 0;
    		
			//��� ���ڸ� �о�鿩 mission ������ ����
   	 		while (fscanf(f, "%[^\n]%", mission) == 1) c++; //%[^\n] ���� ���ڰ� ������ �������� ��� ���ڿ��� �д� ����

    		fseek(f, 0, SEEK_SET);

    		for (int i = 0, r = rand() % c; i < r; fscanf(f, "%[^\n]%*c", mission), i++);

    		printf("%s picked a festival card and got the mission: %s.\n", cur_player[player].name, mission);

    		fclose(f);
			} 

			break;

         #endif  
 	
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
            
            break;
            
        default:
            break;
    }
}

// �÷��̾ ���� �󿡼� ����
void goForward(int player, int step)
{
     void *boardPtr;
     cur_player[player].position += step;
     boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
     

    printf("%s go to node %i (name: %s)\n",
           cur_player[player].name, cur_player[player].position,
           smmObj_getNodeName(boardPtr)); 
}




int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    startGame();

    //1. import parameters (1. �Ű����� ��������)-----------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set(�Ű����� ��Ʈ ����)
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
    	
    	//boardObj�� smmObj_genObject �Լ��� ��ȯ���� ������. �����Ҵ��� �޸� �ּҰ��� boardObj�� ������ �� ����. 
         void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
         
        //linkedList�� ����ִ� �Լ�, smmdb_addTail �̴�.
        smmdb_addTail(LISTNO_NODE, boardObj);//ĭ�� �ش��ϴ� LISTNO_NODE�� ����. 0���� ���� ���� ��ü�� ����.  
		
		smmObjGrade_e grade;
				 
    //	void smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade);
 
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
   
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i); //����ü �޸� �ּҸ� ������. 
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    #if 0
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
   
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while () //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    #endif  
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
        
    //�޸𸮰� ��ŭ �ʿ��ϴٰ� �����Ҵ� 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t)); //�÷��̾� �����ŭ ����ü�� ����Բ� 
    generatePlayers(player_nr, initEnergy);
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) 
    {
        int die_result;
        
        //4-1. initial printing( �ʱ� ���)
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)   (�ֻ��� ������ (���� ���� �ƴ� ���))
        die_result = rolldie(turn);
        
        //4-3. go forward (�����ϱ�)
        goForward(turn, die_result);
	
		//4-4. take action at the destination node of the board(������ ������ ��忡�� ���� ����)
        actionNode(turn);
        
        //4-5. next turn (���� ����)
        turn = (turn + 1)%player_nr;
    }
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
