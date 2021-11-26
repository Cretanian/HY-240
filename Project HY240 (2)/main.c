\
/*************************************************************
 * @file   main.c                                            *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Source file for the needs of cs-240a project 2018 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Persida.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


//-------------------//
struct Army *RootGAS;

struct HorsePQ horseD;

struct GA_Tree GAtree;

struct Army *Generas[5];

unsigned int maxprime,numbofArray;

//-------------------//

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void){
	
	RootGAS = NULL;
	//---------------//
	horseD.horsePQ =  malloc(sizeof(struct Horse)*max_horses);
	horseD.size = 0;
	//--------------//
	GAtree.size = 0;
	GAtree.Ga_tree = NULL;
	//----------------//
	maxprime = 0;
	int i =0;
	while(1){
		if(primes_g[i]>max_soldiers_id_g){
			maxprime = primes_g[i];	//find the the prime that I want 
			break;	
		}
		i++;
	}

	numbofArray = (75*max_soldiers_g)/100;
	hash_table = malloc(sizeof(struct AR_Battle)*numbofArray);	
	for(i = 0; i<numbofArray; i++){
		hash_table[i] = NULL;
	}
	//--------------//
	
	Generas[0] = NULL;
	Generas[1] = NULL;
	Generas[2] = NULL;
	Generas[3] = NULL;
	Generas[4] = NULL;

	return 1;
}

//-------------------------------------------------------//

/**
 * 
 *finds parent 
 *
 *Takes an int becauce we want to know if we want the parent
 * of the last node or the parent that we want to add a node
 */
struct GA_Battle *findpar(struct GA_Battle *H,int  CH){
	int numbofleaf = GAtree.size;	
	int height=0,allleafs=0,mylastlvlleaf=0;

	if(H == NULL){
		return NULL;
	}

	height = log2(numbofleaf); 
	
	int k;
	for(k=0; k<height;k++){	//adds all the leaves till height - 1
		allleafs += pow(2,k);
	}

	mylastlvlleaf = numbofleaf - allleafs; //find how many leaves that my tree has

	if(CH == 0){ //find the parent tha I want to add a new node
		
		if(mylastlvlleaf == pow(2,height)){ //if the last level is full 
			while(H->lc != NULL){
				H = H->lc;
			}
			
			return H;
		}

		while(height>1){
			
			if(mylastlvlleaf < pow(2,height-1)){ //move left based one the nodes that the last level has 
				H = H->lc;
			}else{
				H = H->rc;
				mylastlvlleaf = mylastlvlleaf - pow(2,height-1); //deletes all the nodes that the left subtree has
			}
			height--;
		}

	}else{ //returns the father of the last node
		
		if(mylastlvlleaf == pow(2,height)){ //if the last level is full
			
			struct GA_Battle * far = NULL;;
			while(H->rc != NULL){
				far = H;
				H = H->rc;
			}			
			return far;

		}

		while(height>1){
			
			if(mylastlvlleaf <= pow(2,height-1)){ //move left based one the nodes that the last level has
				H = H->lc;
			}else{
				H = H->rc;
				mylastlvlleaf = mylastlvlleaf - pow(2,height-1); //deletes all the nodes that the left subtree has
			}
			height--;
		}
	}
	return H;
}

/**
 * 
 * 
 * finds the last leaf of the tree
 * 
 * 
 */ 
struct GA_Battle *findlastchild(struct GA_Battle *H){
	
	if(H == NULL){
		return NULL;
	}

	int numbofleaf = GAtree.size;
	
	if(numbofleaf == 0){
		
		return H;
	}

	int height=0,allleafs=0,mylastlvlleaf=0;

	height = log2(numbofleaf);
	
	int k;
	for(k=0; k<height;k++){ //adds all the leaves till height - 1
		allleafs += pow(2,k);
	}

	mylastlvlleaf = numbofleaf - allleafs;
	
	if(mylastlvlleaf == pow(2,height)){ //if the last level is full
		while(H->rc != NULL){
			H = H->rc;
		}
		
		return H;
	}

	while(height>0){ //we go till zero cause we want a leaf
		
		if(mylastlvlleaf <= pow(2,height-1)){ //move left based one the nodes that the last level has
			H = H->lc;
		}else{
			H = H->rc;
			mylastlvlleaf = mylastlvlleaf - pow(2,height-1); //deletes all the nodes that the left subtree has
		}
		height--;
	}

	return H;
}

/**
 * 
 * makes the GA_Battle tree
 * 
 */
void BattleTree(struct Army * H){
	//creates a new GA_Battle node that will add in the GA_Battle tree 				
	struct GA_Battle *p = malloc(sizeof(struct GA_Battle)); 
	p->soldier_id = H->id;
	p->lc=NULL;
	p->rc=NULL;
	//finds the parent that the new node has to be added
	struct GA_Battle *tmp = NULL; 		
	tmp = findpar(GAtree.Ga_tree,0);
	//checking where to add the child
	if(tmp == NULL){			
		GAtree.Ga_tree = p;		
	}else{
		if(tmp->lc==NULL){				
			tmp->lc = p;
		}else{			
			tmp->rc = p;
		}		
	}	
	//checking if any horse is avaliable
	if(horseD.size >0){
		p->horse_id = horseD.horsePQ[0].id;
		ChangeHorse();	//keeping the tree in the first provision
	}else{
		p->horse_id = -1;
	}
	GAtree.size ++;
}

/**
 * 
 * 
 * take the first horse and
 * keep the tree sorted
 * 
 * 
 */
void ChangeHorse(void){
		horseD.horsePQ[0] = horseD.horsePQ[horseD.size-1]; //delete the first horse
		horseD.size --;

		int current;
		current = 0;
		
		while(current<horseD.size){
			int left = 2*current + 1; //finds the left child of a given node
			int right = 2*current + 2; //finds the right child of a given node

			if(left >= horseD.size){
				left = -1;
			}
			if(right >= horseD.size){
				right = -1;
			}	

			if(left == -1 && right == -1){
				break;
			}else if(left != -1 && right == -1){

				if(horseD.horsePQ[left].age < horseD.horsePQ[current].age){ //if the id is less swap with first
					struct HorsePQ chng;
					chng.size = -1;
					chng.horsePQ = malloc(sizeof(struct Horse));

					chng.horsePQ[0] = horseD.horsePQ[current];
					horseD.horsePQ[current] =  horseD.horsePQ[left];
					horseD.horsePQ[left] = chng.horsePQ[0];
					current = left;
				}else{
					break;
				}
			}else if(left != -1 && right != -1){
				int pos;

				if(horseD.horsePQ[left].age < horseD.horsePQ[right].age){ //finds the node with the smaller id 
					pos = left;
				}else{
					pos = right;
				}

				if(horseD.horsePQ[pos].age < horseD.horsePQ[current].age){ //if the id is less swap with first
					struct HorsePQ chng;
					chng.size = -1;
					chng.horsePQ = malloc(sizeof(struct Horse));

					chng.horsePQ[0] = horseD.horsePQ[current];
					horseD.horsePQ[current] =  horseD.horsePQ[pos];
					horseD.horsePQ[pos] = chng.horsePQ[0];
					current = pos;
				}else{
					break;
				}
			
			}
		}
}

int counter =0; //Global counter for the nodes that we searched

/**
 * 
 * 
 * 
 */ 
void DeathGAS(struct GA_Battle *H,int X){					
	if(H==NULL) return;

	counter++;
	
	if(counter == X){
		
		
		struct GA_Battle * last = NULL;
		struct GA_Battle * father = NULL;

		last = findlastchild(GAtree.Ga_tree);	//find the last child	
		father = findpar(GAtree.Ga_tree,1);	//find the parent of the last child


		if(father->lc == last){
			H->soldier_id = last->soldier_id;
			H->horse_id = last->horse_id;	
			father->lc = NULL;
			GAtree.size --;
		}else if(father->rc==last){
			H->soldier_id = last->soldier_id;
			H->horse_id = last->horse_id;
			father->rc = NULL;
			GAtree.size --;
		}
					
		free(last);		
		last=NULL;
		counter = 0;
		
	}
	
	DeathGAS(H->lc,X);
	DeathGAS(H->rc,X);

}

/**
 * 
 * Deleting the first 3 soldiers 
 * 
 */ 
void DeletefromHash(int key){
	unsigned int H,a = 12,b = 133;
	H = ((a*key+b)%maxprime)%numbofArray;
	struct AR_Battle * check = hash_table[H];
	
	if(check != NULL){
		
		struct AR_Battle * tmp = check;
		int counter = 0;

		while(counter <2){			
			tmp = tmp->next;
			if(tmp == NULL){
				hash_table[H] = check;
				return;	
			} 
			counter++;
		}
		hash_table[H] = tmp->next;
	}

}

/**
 * printing the id of the Army tree
 * 
 */
void InOrderGAS(struct Army *P){
	if(P==NULL) return;
	InOrderGAS(P->lc);
	printf("<%d>," , P->id);
	InOrderGAS(P->rc);
}

/**
 * 
 * printing the id and the horse id of the Battle tree
 * 
 */ 
void InOrderGAR(struct GA_Battle * P){							
	if(P==NULL) return;	
	InOrderGAR(P->lc);
	printf("<%d,%d>",P->soldier_id,P->horse_id);
	InOrderGAR(P->rc);
}

/**
 * 
 * recursion for the tree
 * 
 */
void InOrderGAT(struct Army * HA){
		
		if(HA == NULL) return;	
		InOrderGAT(HA->lc);	
		BattleTree(HA);		
		InOrderGAT(HA->rc);
}

/**
 * Gets the id from the tree army 
 * 
 * 
 */ 
void findID(struct GA_Battle *H){
	if(H == NULL) return;
	findID(H->lc);
	DeletefromHash(H->soldier_id); //pass the id of a node
	findID(H->rc);
}

/**
 * sets the rigth child to the given tree
 * 
 * 
 */
static void setChild(struct Army * node, int toLeft, struct Army * child){
    if (toLeft == 1) {
        node->lc = child;
    } else {
        node->rc = child;
    }
}

/**
 * 
 * split the Army tree 
 * 
 */ 
struct Army * splitTree(struct Army * root, int k){
    struct Army * root2 = NULL;
    struct Army * parent1 = NULL;
    struct Army * parent2 = NULL;
	int check;
    	
    int toLeft = 0;
	if(root != NULL && k <= root->id){ //finds which side is the first
		toLeft = 1;
	}
	check = toLeft;

    while (root != NULL) {
		 while (root != NULL && k < root->id == toLeft) {
            parent1 = root;
            root = toLeft ? root->lc : root->rc;
        }
        setChild(parent1, toLeft, NULL); //detached the root of the tree 
	    if(toLeft == 0){ //checks for the other way
			toLeft = 1;
		}else if (toLeft == 1){
			toLeft = 0;
		}
		
        if (root2 == NULL) {
            root2 = root; // creates the root of the tree
			
        } else if (parent2 != NULL) {
            setChild(parent2, toLeft, root); //creates the new Parent tree 
			
        }
		
        parent2 = parent1;
        parent1 = NULL;
    }

	if(check){
		return root2;	
	}else{
		struct Army * tmp = RootGAS;
		RootGAS = root2;
		return tmp;	
	}
	
}

//-------------------------------------------------------//

/**
 * @brief Register Alexander the Great soldier 
 *
 * @param sid     The soldier's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_soldier (int sid){
	struct Army * current; //variable tha has the global
	
	struct Army * tmp; //new tree node that i want to add
	tmp = malloc(sizeof(struct Army));
	tmp->lc = NULL;
	tmp->rc = NULL;
	tmp->id = sid;

	if(RootGAS == NULL){ //check if my tree is empty
		RootGAS = tmp;
	}else{
		current = RootGAS; //set the root
		
		while(current!=NULL){
			
			if(sid == current->id){ 
				break;
			}else if(current->id<sid){ //check where to add the new node 
				if(current->rc == NULL){ 
					current->rc = tmp;
				}else{
					current = current->rc; //find the last node and add the new node
				}
			}else if(current->id >sid){
				if(current->lc == NULL){
					current->lc = tmp;
				}else{
					current = current->lc; //find the last node and add the new node
				}
			}
			
		}
	}

	printf("GA Army = ");	
	print_GA_soldiers();
	printf("\n");

	return 1;
	
}

/**
 * @brief Register Alexander the Great horse 
 *
 * @param sid     The horse's id
 * @param age     The horse's age
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_horse (int hid, int age){
	if(horseD.size!=max_horses){ //checking if I can add any more horses left to add
		struct HorsePQ tmp;
		tmp.size = -1;
		tmp.horsePQ = malloc(sizeof(struct Horse));
		
		int  current=0,parent=0;
		//add the new horse
		horseD.horsePQ[horseD.size].age = age;	
		horseD.horsePQ[horseD.size].id = hid;
		horseD.size ++;

		current = horseD.size-1; //find the cell in the array

		while(current>0){

			parent = (current-1)/2; //find the parent of the cell
			
			if(horseD.horsePQ[current].age <= horseD.horsePQ[parent].age){ //check if want to sawp with the parent 
				
				tmp.horsePQ[0] = horseD.horsePQ[current];
				horseD.horsePQ[current] =  horseD.horsePQ[parent];
				horseD.horsePQ[parent] = tmp.horsePQ[0];
				current = parent;
			}else{
				break;
			}
			

		}	
	
	

	double level = 0;
		if(horseD.size!=0){
			level = log2(horseD.size);
		}

		printf("Level 0:");
		printf("<%d,%d>,\n",horseD.horsePQ[0].id,horseD.horsePQ[0].age);

	
		int printed = 1 ,e=1,pos=1;
		while(e<=level){
			printf("Level %d:",e);
			if(1 == e){
				pos = pow(2,e-1);
			}else if(e == 2){
				pos = pow(2,e-1)+1;
			}else{
				pos = pow(2,e)-1;
			}
				
				int i;
				for( i=0;i<pow(2,e);i++){
					
					if(printed<horseD.size){
						printf("<%d,%d>,",horseD.horsePQ[pos].id,horseD.horsePQ[pos].age);
						pos++;
						printed++;
					}
					
				}
				printf("\n");
		e++;	
		}
	}
	return 1;
}

/**
 * @brief Register enemy soldier
 *
 * @param aid     The soldier's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_AR_soldier (int aid){
	
	unsigned int H,a = 12,b = 133,i;
	struct AR_Battle * tmp = malloc(sizeof(struct AR_Battle));
	struct AR_Battle * check = NULL;

	H = ((a*aid+b)%maxprime)%numbofArray; //find where to add the new soldier in the array
	
	tmp->id = aid;
	tmp->next = NULL;
	
	if(hash_table[H] == NULL){
		hash_table[H] = tmp;
	}else{	
		check = hash_table[H];
		while(check->next != NULL){
			check = check ->next;
		}		
		check->next = tmp;	//adds in the end
	}
		
	for(i = 0;i<numbofArray;i++){
		check = hash_table[i];
		printf("Index %d: ",i);
		if(check!=NULL){
			printf("<%d>,",check->id);			
			while(check->next != NULL){
				check = check ->next;
				printf("<%d>,",check->id);
			}
		}
		printf("\n");
	}	
	return 1;
}

/**
 * @brief Prepare Alexander's soldiers for battle
 * 
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_GA (void){
	InOrderGAT(RootGAS); //pass the root of the tree
	printf("GA battle = ");
	InOrderGAR(GAtree.Ga_tree);
	printf("\n");
	printf("%d\n",GAtree.size);
	return 1;
}

/**
 * @brief The ambush against Alexander the Great
 * @param X  Kill 1-every-X soldiers
 * @return 1 on success
 *         0 on failure
 */
int ambush_GA (int X){		
	DeathGAS(GAtree.Ga_tree,X); //pass the root of the tree and the X
	printf("GA battle = ");
	InOrderGAR(GAtree.Ga_tree);//pass the root of the tree
	printf("\n");
	printf("%d\n",GAtree.size);
	return 1;
}

/**
 * @brief Victory of Alexander the Great's army
 *
 * @return 1 on success
 *         0 on failure
 */
int GA_victory (void){
	findID(GAtree.Ga_tree); //pass the root of the tree
	struct AR_Battle * check = NULL;
	int i;
	for(i = 0;i<numbofArray;i++){
		check = hash_table[i];
		printf("Index %d: ",i);
		if(check!=NULL){
			printf("<%d>,",check->id);			
			while(check->next != NULL){
				check = check ->next;
				printf("<%d>,",check->id);
			}
		}
		printf("\n");
	}	
	return 1;
}

/**
 * @brief The death of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int Alexanders_death (void){

	Generas[0] = splitTree(RootGAS,499); //pass the root of the tree and the max soldier id
	Generas[1] = splitTree(RootGAS,999); //pass the root of the tree and the max soldier id
	Generas[2] = splitTree(RootGAS,1499); //pass the root of the tree and the max soldier id
	Generas[3] = splitTree(RootGAS,1999); //pass the root of the tree and the max soldier id
	Generas[4] = splitTree(RootGAS,2499); //pass the root of the tree and the max soldier id
	

	printf("General 0 =");
	InOrderGAS(Generas[0]);
	printf("\n");

	printf("General 1 =");
	InOrderGAS(Generas[1]);
	printf("\n");

	printf("General 2 =");
	InOrderGAS(Generas[2]);
	printf("\n");

	printf("General 3 =");
	InOrderGAS(Generas[3]);
	printf("\n");

	printf("General 4 =");
	InOrderGAS(Generas[4]);
	printf("\n");


	return 1;
}

/**
 * @brief Print all soldiers of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_soldiers (void){
	printf("GA Army =");
	InOrderGAS(RootGAS); //pass the root of the tree
	printf("\n");	
	return 1;
}

/**
 * @brief Print all of Alexander the Great horses
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_horses (void){
		double level = 0;
		int printed = 1 ,e=1,pos=1;

		if(horseD.size!=0){
			level = log2(horseD.size);
			printf("Level 0:");
			printf("<%d,%d>,\n",horseD.horsePQ[0].id,horseD.horsePQ[0].age);

		}
	
		while(e<=level){
			printf("Level %d:",e);
			if(1 == e){
				pos = pow(2,e-1);
			}else if(e == 2){
				pos = pow(2,e-1)+1;
			}else{
				pos = pow(2,e)-1;
			}
				
			int i;
			for( i=0;i<pow(2,e);i++){
				
				if(printed<horseD.size){
					printf("<%d,%d>,",horseD.horsePQ[pos].id,horseD.horsePQ[pos].age);
					pos++;
					printed++;
				}
				
			}
			printf("\n");
		e++;	
		}
	return 1;
}

/**
 * @brief Print Alexander the Great army
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_army (void){
	printf("GA battle = ");
	InOrderGAR(GAtree.Ga_tree); //pass the root of the tree
	printf("\n");
	return 1;
}

/**
 * @brief Print enemy's army
 *
 * @return 1 on success
 *         0 on failure
 */
int print_AR_army (void){
	struct AR_Battle * check = NULL;
	int i;
	for(i = 0;i<numbofArray;i++){
		check = hash_table[i];
		printf("Index %d: ",i);
		if(check!=NULL){
			printf("<%d>,",check->id);			
			while(check->next != NULL){
				check = check ->next;
				printf("<%d>,",check->id);
			}
		}
		printf("\n");
	}	
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
int main(int argc, char** argv){
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


	// Read max horses
	fgets(buff, BUFFER_SIZE, fin);
	sscanf(buff, "%d", &max_horses);
	DPRINT("Max horses: %d\n", max_horses)

	// Read max enemy soldiers' number
	fgets(buff, BUFFER_SIZE, fin);
	sscanf(buff, "%d", &max_soldiers_g);
	DPRINT("Max enemy soldiers: %d\n", max_soldiers_g)

	// Read max enemy soldiers' ID
	fgets(buff, BUFFER_SIZE, fin);
	sscanf(buff, "%d", &max_soldiers_id_g);
	DPRINT("Max enemy soldiers' ID: %d\n", max_soldiers_id_g)
	
	/* Initializations  */
	initialize();


	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Register GA soldier
				 * R <sid> */
			case 'R':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (register_GA_soldier(sid)) {
						DPRINT("R %d succeeded\n", sid);
					} else {
						fprintf(stderr, "R %d failed\n", sid);
					}

					break;
				}

				/* Register GA horse
				 * H <hid> <age> */
			case 'H':
				{
					int hid, age;

					sscanf(buff, "%c %d %d", &event, &hid, &age);
					DPRINT("%c %d %d\n", event, hid, age);

					if (register_GA_horse(hid, age)) {
						DPRINT("H %d %d succeeded\n", hid, age);
					} else {
						fprintf(stderr, "H %d %d failed\n", hid, age);
					}
				
					break;
				}
				/* Register enemy soldier
				 * A <aid> */
			case 'A':
				{
					int aid;

					sscanf(buff, "%c %d", &event, &aid);
					DPRINT("%c %d\n", event, aid);

					if (register_AR_soldier(aid)) {
						DPRINT("A %d succeeded\n", aid);
					} else {
						fprintf(stderr, "A %d failed\n", aid);
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
				/* Ambush against Alexander the Great
				 * T <X> */
			case 'T':
				{
					int X;

					sscanf(buff, "%c %d", &event, &X);
					DPRINT("%c %d\n", event, X);

					if (ambush_GA(X)) {
						DPRINT("T %d succeeded\n", X);
					} else {
						fprintf(stderr, "T %d failed\n", X);
					}

					break;
				}

				/* Victory of Alexander the Great's army
				 * K */
			case 'K':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (GA_victory()) {
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
				/* Print all Alexander the Great horses
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_horses()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}
			/* Print of Alexander the Great army
				 * X */
			case 'Z':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_army()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}
				/* Print all enemy soldiers
				 * Y */
			case 'W':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_AR_army()) {
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
