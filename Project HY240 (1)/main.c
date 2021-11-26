/*************************************************************
 * @file   main.c                                            *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Source file for the needs of cs-240a project 2018 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Gaugamela.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void){
 
    GA_army[1] = malloc(sizeof(struct Army));
    GA_army[0] = malloc(sizeof(struct Army));
    GA_army[1]->next=NULL;
    GA_army[1]->id = -1;
    GA_army[1]->general = -1;
    GA_army[1]->type = -1;
    GA_army[0]->next=NULL;
    GA_army[0]->id = -1;
    GA_army[0]->general = -1;
    GA_army[0]->type = -1;
    //--------------//
    GA_battle_list = malloc(sizeof(struct GA_Battle));
    GA_battle_list->next=NULL;
    GA_battle_list->horse_id = -2;
    GA_battle_list->general = -2;
    GA_battle_list->soldier_id = -2;
    //-----------------//
    satrapies_list = malloc(sizeof(struct Satrapy));
	satrapies_list->next = NULL;
    satrapies_list->prev = NULL;
    satrapies_list->id = -1;
    satrapies_list->s_army = NULL;
    satrapies_list->sentinel = NULL;
    //---------------------// 
    S_battle_list = malloc(sizeof(struct S_Battle));
    S_battle_list->next = NULL;
    S_battle_list->satrapy_id = -1;
    S_battle_list->soldier_id = -1;
    S_battle_list->type = -1;
    //--------------------//
    GA_Generals[1] = malloc(sizeof(struct GA_Battle));
    GA_Generals[2] = malloc(sizeof(struct GA_Battle));
    GA_Generals[3] = malloc(sizeof(struct GA_Battle));
    GA_Generals[4] = malloc(sizeof(struct GA_Battle));
    GA_Generals[0] = malloc(sizeof(struct GA_Battle));
    GA_Generals[1]->next=NULL;
    GA_Generals[1]->horse_id = -2;
    GA_Generals[1]->general = -2;
    GA_Generals[1]->soldier_id = -2;
    GA_Generals[2]->next=NULL;
    GA_Generals[2]->horse_id = -2;
    GA_Generals[2]->general = -2;
    GA_Generals[2]->soldier_id = -2;
    GA_Generals[3]->next=NULL;
    GA_Generals[3]->horse_id = -2;
    GA_Generals[3]->general = -2;
    GA_Generals[3]->soldier_id = -2;
    GA_Generals[4]->next=NULL;
    GA_Generals[4]->horse_id = -2;
    GA_Generals[4]->general = -2;
    GA_Generals[4]->soldier_id = -2;
    GA_Generals[0]->next=NULL;
    GA_Generals[0]->horse_id = -2;
    GA_Generals[0]->general = -2;
    GA_Generals[0]->soldier_id = -2;
    //-------------------//
   
	return 1;
}

/**
 * @brief Register Alexander the Great soldier 
 *
 * @param sid     The soldier's id
 * @param type    The soldier's type
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_soldier (int sid, int type, int general){
   
    struct Army * current = GA_army[1];
    struct Army * myptr = GA_army[1];
       

     
    while(current != NULL){

        if(current->id == sid){
              return 0;
        }
        current = current -> next;
    }

    current = myptr;
  
    while(current != NULL && current->id < sid) {
        myptr=current;
        current = current->next;
    }

    struct Army * tmp = NULL;
    tmp = malloc(sizeof(struct Army));
    tmp->id=sid;
    tmp->type=type;
    tmp->general = general;

    if(!(myptr->id<sid)){
        tmp->next = current;
        myptr-> next = tmp;
    }else{
        tmp->next = myptr->next;
        myptr->next = tmp;
    }

    struct Army * soldier = GA_army[1]->next;
    printf("GA Soldiers =");
    while(soldier!=NULL){
        
        printf("<%d,%d,%d>,",soldier->id,soldier->type,soldier->general);
        soldier = soldier->next;

    }
    printf("\nDONE\n");

	return 1;
}

/**
 * @brief Register Alexander the Great horse 
 *
 * @param sid     The horse's id
 * @param type    Always the value of 0
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_horse (int hid, int type, int general){
  
    struct Army * current = GA_army[0];
    struct Army * myptr = GA_army[0];
     
    while(current != NULL){

        if(current->id == hid){
              return 0;
        }
        current = current -> next;
    }

    current = myptr;

    while(current != NULL && current->id < hid) {
        myptr=current;
        current = current->next;
    }

    struct Army * tmp = NULL;
    tmp = malloc(sizeof(struct Army));
    tmp->id=hid;
    tmp->type=type;
    tmp->general = general;

    if(!(myptr->id<hid)){
        tmp->next = current;
        myptr-> next = tmp;
    }else{
        tmp->next = myptr->next;
        myptr->next = tmp;
    }

    struct Army * horse = GA_army[0]->next;
    printf("GA Horses =");
    while(horse!=NULL){
        
        printf("<%d,%d,%d>,",horse->id,horse->type,horse->general);
        horse = horse->next;

    }
    printf("\nDONE\n");
	return 1;
 
}

/**
 * @brief Register Satrapy
 *
 * @param sid The Satrapy's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_satrapy (int sid){

    struct Satrapy * current = satrapies_list;


    struct Satrapy * tmp = NULL;
    tmp = malloc(sizeof(struct Satrapy));
    tmp->id = sid;
	tmp ->next = NULL;
	tmp->prev = NULL;

    struct Army * guard = NULL;
    guard = malloc(sizeof(struct Army));
    guard->id = -1;
    guard->general = -1;
    guard->next = NULL;
    guard->type = -1;

    tmp->s_army=NULL;
    tmp->sentinel = guard;
	
    if(satrapies_list->next == NULL){
        tmp->next = satrapies_list;
        tmp->prev = satrapies_list;    
        satrapies_list->next=tmp;
        satrapies_list->prev=tmp;
               
    }else{
                
        tmp->next = satrapies_list->next;
        tmp->prev = satrapies_list;
        satrapies_list->next = tmp;
        tmp->next->prev = tmp;
       
    }
	
	
    current = satrapies_list->next;

    printf("Satrapies =");
    while(current != satrapies_list){
        
        printf("<%d>,",current->id);
        current = current->next;

    }

    printf("\nDONE\n");

	return 1;
}

/**
 * @brief Register Satrapy soldier
 *
 * @param sid     The id of the Satrapy the soldier belongs to
 * @param aid     The soldier's id
 * @param type    The type of soldier
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_S_soldier (int sid, int aid, int type, int general){

     struct Satrapy * current = satrapies_list->next;
  

    while(current != NULL && current->id != -1){
        
        if(current->id == sid){
            break;
        }
        current = current->next;
    }


    if(current->id == -1){
        return -1;
    }

    
    struct Army * myptr = NULL;
    myptr = malloc(sizeof(struct Army));
    myptr->id = aid;
    myptr->general = general;
    myptr->type= type;
    
    if(current->s_army!=NULL){
        struct Army * tmp = NULL;
        tmp =  current->s_army;
        current->s_army = myptr;
        myptr->next = tmp;
         
    }else{
        current->s_army = myptr;
        myptr->next = current->sentinel;
        
    }

    current = satrapies_list->next;
   
    int c = 0;
    while(current != satrapies_list){
        c++;
        printf("Statrapy %d =",c);
        
        if(current->s_army == NULL){
             printf("\n");
        }else{
            struct Army * tmp = NULL;
            tmp = current->s_army;
            while(tmp != current->sentinel){
                printf("<%d,%d,%d>,",tmp->id,tmp->type,tmp->general);
                tmp = tmp->next;
            }
            printf("\n"); 
        }
        current = current->next;
    }

    printf("DONE\n");

	return 1;
}

/**
 * @brief Prepare Alexander's soldiers for battle
 * 
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_GA (void){
   
    struct Army * soldier = GA_army[1]->next;
    struct Army * horse = GA_army[0]->next;

    struct GA_Battle  * myptr = GA_battle_list;  
    struct GA_Battle  * tmp = NULL;
    while(soldier !=NULL){
            

        
            if(soldier->type == 1){
                tmp = malloc(sizeof(struct GA_Battle)); 
                tmp->soldier_id = soldier->id;
                if(horse != NULL){
                    tmp->horse_id = horse->id;
                    horse = horse->next;
                }else{
                    tmp->horse_id = -1;
                    soldier->type = 3;
                }
                tmp->general = soldier->general;
                tmp->next = NULL;
                myptr->next =tmp;
                myptr=myptr->next;
                }
            
    

        
        soldier = soldier->next;
       
    }

    soldier = GA_army[1]->next;
    while(soldier !=NULL){

        if(soldier->type == 2){
                tmp = malloc(sizeof(struct GA_Battle)); 
                tmp->soldier_id = soldier->id;
                tmp->horse_id = -1;
                tmp->general = soldier->general;
                tmp->next = NULL;
                myptr->next =tmp;
                myptr=myptr->next;
            }
       
        soldier = soldier->next;

    }

    soldier = GA_army[1]->next;
    while(soldier !=NULL){

        if(soldier->type == 3){
                tmp = malloc(sizeof(struct GA_Battle)); 
                tmp->soldier_id = soldier->id;
                tmp->horse_id = -1;
                tmp->general = soldier->general;
                tmp->next = NULL;
                myptr->next =tmp;
                myptr=myptr->next;
            }
       
        soldier = soldier->next;

    }

    soldier = GA_army[1]->next;
    while(soldier !=NULL){

        if(soldier->type == 4){
                tmp = malloc(sizeof(struct GA_Battle)); 
                tmp->soldier_id = soldier->id;
                tmp->horse_id = -1;
                tmp->general = soldier->general;
                tmp->next = NULL;
                myptr->next =tmp;
                myptr=myptr->next;
            }
       
        soldier = soldier->next;

    }

    myptr->next = NULL;

    myptr = GA_battle_list->next;

    printf("GA Battle =");
    while(myptr!=NULL){
        
        printf("<%d,%d,%d>,",myptr->soldier_id,myptr->horse_id,myptr->general);
        myptr = myptr->next;

    }

    printf("\nDONE\n");

	return 1;
}

/**
 * @brief Prepare Satrapies for battle
 *
 * @param sid The satrapy's id
 *
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_S (int sid){

    struct Satrapy * current = satrapies_list->next;
    struct S_Battle  * tmp = S_battle_list ;
    struct Satrapy * keep =NULL;

    int counter = 0;
    while(current != satrapies_list){

        if(current->s_army!=NULL){
            counter++;
        }

        if(current->id==sid){
             keep = current;
        }
        current = current ->next;
    }

    if(keep==NULL){
        return -1;
    }else{
       current = keep;
    }

    struct Army * myptr = current->s_army;

    while(myptr != current->sentinel){

        struct S_Battle * yolo = NULL;
        yolo = malloc(sizeof(struct S_Battle));
        yolo->satrapy_id = current->id;
        yolo->soldier_id = myptr->id;
        yolo->type = myptr->type;

        tmp->next = yolo;
        tmp = tmp->next;

        myptr = myptr->next;
    }


    counter= counter-1;
    struct S_Battle * yolo = NULL;
    struct Satrapy * LP = current->prev;
    struct Satrapy * RP = current->next;
   

    int R=0;

     
    while(LP != RP && R<counter){
      
       
        if(LP != satrapies_list && LP->s_army !=NULL){
            myptr = LP->s_army;

            while(myptr != LP->sentinel){

                
                yolo = malloc(sizeof(struct S_Battle));
                yolo->satrapy_id = LP->id;
                yolo->soldier_id = myptr->id;
                yolo->type = myptr->type;

                tmp->next = yolo;
                tmp = tmp->next;

                myptr = myptr->next;
            }
            R++;
        }

        if(RP != satrapies_list && RP->s_army !=NULL){
            myptr = RP->s_army;

            while(myptr != RP->sentinel){

                struct S_Battle * yolo = NULL;
                yolo = malloc(sizeof(struct S_Battle));
                yolo->satrapy_id = RP->id;
                yolo->soldier_id = myptr->id;
                yolo->type = myptr->type;

                tmp->next = yolo;
                tmp = tmp->next;

                myptr = myptr->next;
            }
            R++;
        }

        LP = LP->prev;
        RP = RP->next;
       
    }

    if(LP == RP && LP !=satrapies_list){

        myptr = RP->s_army;

        while(myptr != RP->sentinel){

            struct S_Battle * yolo = NULL;
            yolo = malloc(sizeof(struct S_Battle));
            yolo->satrapy_id = RP->id;
            yolo->soldier_id = myptr->id;
            yolo->type = myptr->type;

            tmp->next = yolo;
            tmp = tmp->next;

            myptr = myptr->next;
        }

    }

    tmp->next = NULL;
    tmp = S_battle_list->next ;
    printf("Satrapy Battle =");
    while(tmp!=NULL){
       
        printf("<%d,%d,%d>,",tmp->soldier_id,tmp->satrapy_id,tmp->type);
        tmp = tmp->next;
    }
    
    printf("\nDONE\n");
    
    
	return 1;
}

/**
 * @brief The death of a soldier
 * 
 * @param sid  The soldier's id
 * @param flag If 0, the soldier belongs to a Satrapy
 *             If 1, the soldier belongs to Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int soldiers_death (int sid, int flag){

    struct GA_Battle * GA = GA_battle_list->next;
    struct S_Battle * S = S_battle_list->next;

    if(flag == 1){
        struct GA_Battle * tmp = GA;

        while(GA!=NULL){

            if(GA->soldier_id == sid){
                break;
            }
            tmp = GA;
            GA=GA->next;
        }

        if(GA != NULL){
            tmp->next = GA->next;
			 
        }else{
            return -1;
        }
        
        struct Army * myS = GA_army[1]->next;
        struct Army * Atmp = myS;

        while(myS != NULL){

            if(GA->soldier_id == myS->id){
                break;
            }
            Atmp = myS;
            myS = myS->next;

        } 

        if(myS != NULL){
            Atmp->next = myS->next;
			
        }else{
            return -1;
        }


    }else if(flag == 0){
        struct S_Battle * tmp = NULL;

        while(S != NULL){

            if(S->soldier_id == sid){
                break;
            }
            tmp = S;
            S = S->next;
        }

        if(S != NULL){
            tmp->next = S->next;
			
        }else{
            return -1;
        }

        struct Satrapy * Stmp = satrapies_list->next;
        
        while(S->satrapy_id != Stmp->id && Stmp != satrapies_list){           
            Stmp = Stmp->next;
        }
        

        struct Army * Ssol = Stmp->s_army;
        struct Army *Sdel = Ssol;
        while(Ssol != Stmp->sentinel){
            if(Ssol->id == S->soldier_id){
                break;
            }
            Sdel = Ssol;
            Ssol = Ssol -> next;
        }

        if(Ssol != Stmp->sentinel){
            if(Sdel==Ssol){
                Stmp->s_army = Ssol->next;
            }else{
                Sdel->next = Ssol->next;
            }
			
        }else{
            return -1;
        }

    }else{
        return -1;
    }

    GA = GA_battle_list->next;
    printf("GA Battle =");
    while(GA!=NULL){
       
        printf("<%d,%d,%d>,",GA->soldier_id,GA->horse_id,GA->general);
        GA = GA->next;
    }
    printf("\n");

    S = S_battle_list->next ;
    printf("Satrapy Battle =");
    while(S!=NULL){
       
        printf("<%d,%d,%d>,",S->soldier_id,S->satrapy_id,S->type);
        S = S->next;
    }

     printf("\nDONE\n");

	return 1;
}

/**
 * @brief Victory of Alexander the Great's horsemen
 *
 * @return 1 on success
 *         0 on failure
 */
int GA_horsemen_victory (void){

    struct GA_Battle * GA = GA_battle_list->next;   
    struct S_Battle * S = S_battle_list->next;
    struct S_Battle * Spnt = S_battle_list;

   
    while(S!=NULL){
        if(S->type==1){
            Spnt->next = S->next;
            S = S_battle_list;
           
        }
        Spnt = S;
        S = S->next; 
    }
   
    struct GA_Battle * Stmp1 = GA_battle_list;
    int counter = 0;
    while(GA !=NULL){
        if(GA->horse_id != -1){
            counter++;
        }
        if(counter%3 == 0){
            Stmp1->next = GA->next;
        }
        Stmp1 = GA;
        GA = GA->next;
    }
    
    GA = GA_battle_list->next;
    printf("GA Battle =");
    while(GA!=NULL){
       
        printf("<%d,%d,%d>,",GA->soldier_id,GA->horse_id,GA->general);
        GA = GA->next;
    }
    printf("\n");

    S = S_battle_list->next ;
    printf("Satrapy Battle =");
    while(S!=NULL){
       
        printf("<%d,%d,%d>,",S->soldier_id,S->satrapy_id,S->type);
        S = S->next;
    }

     printf("\nDONE\n");


	return 1;
}

/**
 * @brief The death of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int Alexanders_death (void){
    struct GA_Battle * GA = GA_battle_list->next;
    struct GA_Battle * G1 = GA_Generals[1];
    struct GA_Battle * G2 = GA_Generals[2];
    struct GA_Battle * G3 = GA_Generals[3];
    struct GA_Battle * G4 = GA_Generals[4];
    struct GA_Battle * G0 = GA_Generals[0];
    int i=0;

    while(GA!=NULL){

        struct GA_Battle * tmp = NULL;
        tmp = malloc(sizeof(struct GA_Battle));
        tmp->general = GA->general;
        tmp->horse_id = GA->horse_id;
        tmp->soldier_id = GA->soldier_id;

        if(GA->general==1){
            i=1;
        }else if(GA->general==2){
            i=2;
        }else if(GA->general==3){
            i=3;
        }else if(GA->general==4){
            i=4;
        }else if(GA->general==0){
            i=0;
        }else{
            return -1;
        }

        GA_Generals[i]->next = tmp;
        GA_Generals[i] = GA_Generals[i]->next;
        GA = GA->next;
    }

    GA_Generals[1] = G1;
    GA_Generals[2] = G2;
    GA_Generals[3] = G3;
    GA_Generals[4] = G4;
    GA_Generals[0] = G0;

    int c = 0;
    while(c<5){
        GA = GA_Generals[c]->next;
        printf("General %d =",c);
        while(GA != NULL){
            printf("<%d,%d,%d>,",GA->soldier_id,GA->horse_id,GA->general);
            GA = GA->next;
        }
        printf("\n");
        c++;
    }

    printf("DONE\n");
    

	return 1;
}

/**
 * @brief Print all soldiers of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_soldiers (void){
  struct Army * soldier = GA_army[1]->next;
    printf("GA Soldiers =");
    while(soldier!=NULL){
        
        printf("<%d,%d,%d>,",soldier->id,soldier->type,soldier->general);
        soldier = soldier->next;

    }  
	  printf("\n");
	 struct Army * horse = GA_army[0]->next;
	 printf("GA Horses =");
	 while(horse!=NULL){
	 	 printf("<%d,%d,%d>,",horse->id,horse->type,horse->general);
		 horse = horse->next;
	 }
		horse = NULL;
    printf("\nDONE\n");
   	return 1;
}

/**
 * @brief Print all Satrapy soldiers
 *
 * @return 1 on success
 *         0 on failure
 */
int print_S_soldiers (void){
    struct Satrapy * current = satrapies_list->next;
    current = satrapies_list->next;
   
    int c = 0;
    while(current != satrapies_list){
        c++;
        printf("Statrapy %d =",c);
        
        if(current->s_army == NULL){
             printf("\n");
        }else{
            struct Army * tmp = NULL;
            tmp = current->s_army;
            while(tmp != current->sentinel){
                printf("<%d,%d,%d>,",tmp->id,tmp->type,tmp->general);
                tmp = tmp->next;
            }
            printf("\n"); 
        }
        current = current->next;
    }

    printf("DONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
 
int free_all(void){
 

	return 1;
}

/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Register GA soldier
				 * R <sid> <type> <general> */
			case 'R':
				{
					int sid, type, general;

					sscanf(buff, "%c %d %d %d", &event, &sid, &type, &general);
					DPRINT("%c %d %d %d\n", event, sid, type, general);

					if (register_GA_soldier(sid, type, general)) {
						DPRINT("R %d %d %d succeeded\n", sid, type, general);
					} else {
						fprintf(stderr, "R %d %d %d failed\n", sid, type, general);
					}

					break;
				}

				/* Register GA horse
				 * H <hid> <type> <general> */
			case 'H':
				{
					int hid, type, general;

					sscanf(buff, "%c %d %d %d", &event, &hid, &type, &general);
					DPRINT("%c %d %d %d\n", event, hid, type, general);

					if (register_GA_horse(hid, type, general)) {
						DPRINT("H %d %d %d succeeded\n", hid, type, general);
					} else {
						fprintf(stderr, "H %d %d %d failed\n", hid, type, general);
					}

					break;
				}

				/* Register Satrapy
				 * S <sid> */
			case 'S':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (register_satrapy(sid)) {
						DPRINT("S %d succeeded\n", sid);
					} else {
						fprintf(stderr, "S %d failed\n", sid);
					}

					break;
				}

				/* Register Satrapy soldier
				 * A <sid> <aid> <type> <general> */
			case 'A':
				{
					int sid, aid, type, general;

					sscanf(buff, "%c %d %d %d %d", &event, &sid, &aid, &type, &general);
					DPRINT("%c %d %d %d %d\n", event, sid, aid, type, general);

					if (register_S_soldier(sid, aid, type, general)) {
						DPRINT("A %c %d %d %d %d succeeded\n", event, sid, aid, type, general);
					} else {
						fprintf(stderr, "A %c %d %d %d %d failed\n", event, sid, aid, type, general);
					}

					break;
				}

				/* Prepare Alexander's soldiers for battle
				 * P */
			case 'P':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (prepare_for_battle_GA()) {
						DPRINT("P %c succeeded\n", event);
					} else {
						fprintf(stderr, "P %c failed\n", event);
					}

					break;
				}

				/* Prepare Satrapies for battle
				 * B <sid> */
			case 'B':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (prepare_for_battle_S(sid)) {
						DPRINT("B %c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "B %c %d failed\n", event, sid);
					}

					break;
				}

				/* The death of a soldier
				 * K <sid> <flag> */
			case 'K':
				{
					int sid, flag;

					sscanf(buff, "%c %d %d", &event, &sid, &flag);
					DPRINT("%c %d %d\n", event, sid, flag);

					if (soldiers_death(sid, flag)) {
						DPRINT("K %c %d %d succeeded\n", event, sid, flag);
					} else {
						fprintf(stderr, "K %c %d %d failed\n", event, sid, flag);
					}

					break;
				}

				/* Victory of Alexander the Great's horsemen
				 * V */
			case 'V':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (GA_horsemen_victory()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* The death of Alexander the Great
				 * D */
			case 'D':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (Alexanders_death()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print all soldiers of Alexander the Great
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_soldiers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print all Satrapy soldiers
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_S_soldiers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
