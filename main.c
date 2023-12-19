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


//board configuration parameters (보드 설정 매개변수)
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

// 플레이어 구조체 정의
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

//function prototypes(함수 프로토타입)
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

// 플레이어의 모든 성적 출력
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

// 과목을 들었는지 안 들었는지를 짜야함. printGrades를 응용하여 함수 작성할 것
 

// 모든 플레이어의 상태 출력
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

// 이전에 제시한 코드의 일부분

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

    // 플레이어 생성
    generatePlayers(numPlayers, initEnergy);

    // 게임 메인 루프
    int turn = 0;
    while (1) {
        printf("========================== PLAYER STATUS ==========================\n");
        printPlayerStatus();

        printf("\nThis is %s's turn :::: Press any key to roll a die (press g to see grade): ", cur_player[turn].name);
        char c;
        scanf(" %c", &c);
        fflush(stdin);

        if (c == 'g') {
            printf("--> result : %d\n", 1); // 예시로 주사위 결과를 1로 가정
            printGrades(turn);
        } else {
            int die_result = rolldie(turn);
            printf("--> result : %d\n", die_result);

            goForward(turn, die_result);
            actionNode(turn);

            // 게임 종료 체크
            if (cur_player[turn].accumCredit >= GRADUATE_CREDIT && cur_player[turn].position == 0) {
                printf("\nCongratulations! %s has graduated!\n", cur_player[turn].name);
                printGrades(turn);
                exit(0);
            }

            // 다음 턴
            turn = (turn + 1) % numPlayers;
        }
    }
}
/*
// 실험 중 상태에서 주사위를 굴려 실험을 시도하는 함수
void attemptExperiment(int player)
		{
   		// 실험 성공 기준값을 1에서 6까지의 랜덤한 숫자로 설정
    	int experimentSuccessThreshold = rand() % 6 + 1;
    	
		// 주사위 굴려서 실험 성공 여부 확인
  		int experimentResult = rolldie(player);

		    // 실험 성공 여부에 따른 처리
   			if (experimentResult >= experimentSuccessThreshold)
   			 {
        		printf("%s successfully completed the experiment!\n", cur_player[player].name);
        		// 실험이 성공하면 실험 중 상태를 종료하고 이동
        		isExperimenting = 0;
        		goForward(player, 1); // 실험 성공 시 1칸 전진
    		}
    		else
    		{
        		printf("%s's experiment failed. Staying in the laboratory.\n", cur_player[player].name);
        		// 실험 실패 시, 이동하지 못하고 실험 중 상태로 머무름
       			printf("Energy before the failed experiment: %d\n", cur_player[player].energy);
        		cur_player[player].energy -= smmObj_getNodeEnergy(boardObj);
        		printf("Energy after the failed experiment: %d\n", cur_player[player].energy);
   			}
			}

				// 실험 중 상태인지 확인하고 실험을 시도하는 부분
				if (isExperimenting)
				{
    			// 실험을 시도하고 실험 중 상태를 종료하면서 이동
    			attemptExperiment(turn);
				}
				else
				{
    			// 실험 중이 아닌 경우에는 일반적인 주사위 굴리기 처리
			    die_result = rolldie(turn);
				}
*/


///////////////////////////////////////////////////////////////////



// 초기 에너지로 플레이어를 생성
void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name:", i); //¾E³≫ ¹®±¸ 
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

// 주사위를 굴려 결과를 반환
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

//action code when a player stays at a node(노드에 머무를 때의 동작을 수행)
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
        case SMMNODE_TYPE_LECTURE: //강의 칸  
        
            // if 에너지가 없을 때 강의를 들을 수 없는 것을 조건문 달기  
            cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr); 
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
        
        
        //grade 생성해야함
		//gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, smmObjGrade_e);
		//gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, smmObjGrade);        

	smmObjGrade_e randomGrade = rand() % (SMMNODE_TYPE_MAX - SMMNODE_TYPE_LECTURE) + SMMNODE_TYPE_LECTURE;
	gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, randomGrade);



		//linkedList에 집어넣는 함수, smmdb_addTail 이다.
        smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
		//Linked List를 활용하여 성적을 꺼내서 출력하거나, 이전에 수강했던 과목이냐를 검색한다거나 할 수 있다.
		//getdata 함수를 활용.  
        break;
           	
           
        #if 0 
           
		 case SMMNODE_TYPE_RESTAURANT:   
		 	cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
		 	
		 	
		case SMMNODE_TYPE_HOME:    //credit:0, energy:18 
		 	cur_player[player].energy += 18;
		  
		case SMMNODE_TYPE_LABORATORY:
  		  // 실험 : 실험중 상태로 전환되면서 실험실로 이동 (주사위 눈 범위에서 실험 성공 기준값을 랜덤으로 지정.)
    		if (cur_player[player].energy >= 3) {
        		cur_player[player].energy -= 3;  // 실험 시도마다 3 에너지 소모
        		int diceResult = rolldie(player);
       			int experimentSuccessThreshold = rand() % 6 + 1;  // 실험 성공 기준값을 1에서 6까지의 랜덤 값으로 설정

        printf("%s is conducting an experiment in the laboratory. Rolled a die: %d\n", cur_player[player].name, diceResult);

        if (diceResult >= experimentSuccessThreshold) {
            // 실험이 성공한 경우
            printf("Experiment successful! %s completed the experiment.\n", cur_player[player].name);
            // 여기에서 실험이 성공했을 때의 추가 동작을 작성할 수 있음
        }
            
        else {
            // 실험이 실패한 경우
            printf("Experiment failed! %s couldn't complete the experiment.\n", cur_player[player].name);
            // 여기에서 실험이 실패했을 때의 추가 동작을 작성할 수 있음
        }
    } 
	else {
        printf("%s doesn't have enough energy to conduct an experiment.\n", cur_player[player].name);
    }
    break;

		case SMMNODE_TYPE_GOTOLAB:
   		// 실험실 : 실험중 상태면 주사위를 굴려서 사전에 지정된 실험 성공 기준값 이상이 나오면 실험이 종료되고, 그렇지 않으면 이동하지 못하고 실험중 상태로 머무름.
   			if (cur_player[player].energy >= 3) {
        	cur_player[player].energy -= 3;  // 실험 시도마다 3 에너지 소모
        	int diceResult = rolldie(player);
        	int experimentSuccessThreshold = rand() % 6 + 1;  // 실험 성공 기준값을 1에서 6까지의 랜덤 값으로 설정

        printf("%s is attempting to go to the laboratory. Rolled a die: %d\n", cur_player[player].name, diceResult);

        if (diceResult >= experimentSuccessThreshold) {
            // 실험이 성공한 경우
            printf("Arrived at the laboratory and successfully started an experiment.\n");
        } 
		else {
            // 실험이 실패한 경우
            printf("Failed to reach the laboratory or start an experiment.\n");
        }
    } 
	else {
        printf("%s doesn't have enough energy to attempt to go to the laboratory.\n", cur_player[player].name);
    }
    break;		
		 
		 case SMMNODE_TYPE_FOODCHANCE:    
    	{
        // 보충 찬스 : marbleFoodConfig.txt 파일에서 랜덤으로 음식을 선택하고 해당 에너지를 현재 에너지에 더함
        FILE *foodFile = fopen("marbleFoodConfig.txt", "r"); //read모드로 읽어와서 파일열기 
        if (foodFile == NULL) {
            printf("[ERROR] Failed to open marbleFoodConfig.txt.\n"); //파일 열기 실패 
            exit(EXIT_FAILURE);
        }

        // 파일에서 랜덤으로 음식 선택
        char foodName[MAX_CHARNAME];
        int foodEnergy;
        int totalFoodCount = 0;

        // 파일에서 총 음식의 개수 파악
        while (fscanf(foodFile, "%s %d", foodName, &foodEnergy) == 2) {
            totalFoodCount++;
        }

        // 랜덤으로 선택된 음식의 번호 생성
        int randomFoodIndex = rand() % totalFoodCount;

        // 파일 포인터를 다시 처음으로 되돌리고 선택된 음식까지 읽음
        fseek(foodFile, 0, SEEK_SET);
        for (int i = 0; i < randomFoodIndex; i++) {
            fscanf(foodFile, "%s %d", foodName, &foodEnergy);
        }

        // 선택된 음식의 에너지를 현재 에너지에 더함
        cur_player[player].energy += foodEnergy;

        printf("%s picked a food chance card and gained %d energy from %s.\n", cur_player[player].name, foodEnergy, foodName);

        fclose(foodFile);
    	}
    	break;


		 
		 case SMMNODE_TYPE_FESTIVAL: 
		 {
	    FILE *f = fopen("marbleFestivalConfig.txt", "r"); //축제카드 파일 읽기 
    		if (!f) {
     	   	printf("[ERROR] Failed to open marbleFestivalConfig.txt.\n"); //열기 실패했을 경우 출력 
   		    exit(EXIT_FAILURE);
  	  		}

    		char mission[MAX_CHARNAME];
    		int c = 0;
    		
			//모든 문자를 읽어들여 mission 변수에 저장
   	 		while (fscanf(f, "%[^\n]%", mission) == 1) c++; //%[^\n] 개행 문자가 나오기 전까지의 모든 문자열을 읽는 형식

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

// 플레이어를 보드 상에서 전진
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

    //1. import parameters (1. 매개변수 가져오기)-----------------------------------------------------------------------
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
        //store the parameter set(매개변수 세트 저장)
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
    	
    	//boardObj에 smmObj_genObject 함수의 반환값을 저장함. 동적할당한 메모리 주소값을 boardObj에 저장할 수 있음. 
         void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
         
        //linkedList에 집어넣는 함수, smmdb_addTail 이다.
        smmdb_addTail(LISTNO_NODE, boardObj);//칸에 해당하는 LISTNO_NODE를 넣음. 0번에 새로 생긴 객체를 넣음.  
		
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
        void *boardObj = smmdb_getData(LISTNO_NODE, i); //구조체 메모리 주소를 얻어내야함. 
        
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
        
    //메모리가 얼만큼 필요하다고 동적할당 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t)); //플레이어 명수만큼 구조체를 만들게끔 
    generatePlayers(player_nr, initEnergy);
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) 
    {
        int die_result;
        
        //4-1. initial printing( 초기 출력)
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)   (주사위 굴리기 (실험 중이 아닌 경우))
        die_result = rolldie(turn);
        
        //4-3. go forward (전진하기)
        goForward(turn, die_result);
	
		//4-4. take action at the destination node of the board(보드의 목적지 노드에서 동작 수행)
        actionNode(turn);
        
        //4-5. next turn (다음 차례)
        turn = (turn + 1)%player_nr;
    }
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
