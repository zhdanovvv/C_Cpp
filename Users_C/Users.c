#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define PRINT_ACTION 1
#define FIND_ACTION 2
#define ADD_ACTION 3
#define SEARCH_DEL_ACTION 4
#define EXIT_ACTION 5



typedef struct User {
	int id;
	char name[10];
	char surname[10];
	struct User* next;
	
	struct User* (*Find_user)();
	void (*Print_list)();
} User;

User* Add_user(User* head , char n_m[] , char s_n[]);
User* Find_user_(User* head , int* i_d);
void Print_user_(User* head);


User* Add_user(User* head , char n_m[] , char s_n[])
{
	static int user_count = 0;
	User* tmp = (User*) malloc(sizeof(User));
	tmp->next = NULL;
	tmp->id = user_count;
	strcpy(tmp->name , n_m);
	strcpy(tmp->surname , s_n);
	tmp->Find_user = &Find_user_;
	tmp->Print_list = &Print_user_;
	if(head != NULL) {
		while(head->next) {
			head = head->next;
		}
		head->next = tmp;
	}
	user_count++;
	return tmp;
}




User* Find_user_(User* head , int* i_d)
{		
	User* ptr = head;
	while(ptr != NULL){		   
		if(ptr->id != *i_d){
	        ptr = ptr->next;			
	    }else{
		   return  ptr;
	    }
    }
	return NULL;
}

User* Find_same_users(User* head , char s_n[])
{
	User* ptr = head;
	do{
		if(ptr != NULL){		   
			if(strcmp(ptr->surname , s_n) != 0){
				ptr = ptr->next;			
			}else{
				printf("%d %s %s\n" , ptr->id , ptr->name , ptr->surname);
				ptr = ptr->next;
			}
		}
	}while(ptr != NULL);
}	


User* Del_user(User* head , User* p)
{
	User* ptr = head;
	User* ptrPrev = NULL;
	while(ptr != NULL) {
		if(ptr == p){
			if(ptr != head) {
				ptrPrev->next = p->next;
			} else {
				head = p->next;
			}
			free(p);
			return head;
		}
		ptrPrev = ptr;
		ptr = ptr->next;
	}
	return head;
}


void Print_user_(User* head)
{
    User* tmp = NULL;
	printf("----------- list of users ------------\n");
    if(head != NULL) {
        printf("%d %s %s\n" , head->id , head->name , head->surname);
		while(head->next) {
			head = head->next;
			printf("%d %s %s\n" , head->id , head->name , head->surname);
		}
			head->next = tmp;
	}
	printf("\n");
}

int main ()
{	
	User* head = NULL;
	char n_m[10];
	char s_n[10];
	int i_d;
	int input;
	FILE* fptr = fopen("Users.txt" , "r");
	if(fptr == NULL){
		printf("Can't open the file!\n");
		exit(1);
	}
	while(!feof(fptr)){
		fscanf(fptr , "%s %s\n" , n_m , s_n);	
		User* a = Add_user(head, n_m , s_n);
		if(head == NULL){
			head = a;							
		}
	}
	fclose(fptr);
	
	int execute = 1;
	do {
		printf("---------------------------------------------\n");
		printf("%d - Print users\n", PRINT_ACTION);
		printf("%d - Find users with the same last name\n", FIND_ACTION);
		printf("%d - Add a user\n", ADD_ACTION);
		printf("%d - Find user by id and remove it\n", SEARCH_DEL_ACTION);
		printf("%d - Exit\n", EXIT_ACTION);
		printf("Input action: ");
		scanf("%d" , &input);
		switch(input) {
			case PRINT_ACTION: {
				head->Print_list(head);
			} break;
			case FIND_ACTION: {
				printf("\nEnter the surname: ");
				scanf("%s" , s_n);
				Find_same_users(head , s_n);
			} break;
			case ADD_ACTION: {
				printf("\nEnter the name: ");
				scanf("%s" , n_m);
				printf("\nEnter the surname: ");
				scanf("%s" , s_n);
				Add_user(head, n_m , s_n);
			} break;
			case SEARCH_DEL_ACTION: {
				printf("\nEnter the id: ");
				scanf("%s" , &i_d);
				User* b = head->Find_user(head , &i_d);
				head = Del_user(head , b);
			} break;
			case EXIT_ACTION: {
				printf("Bye!\n");
				execute = 0;
			} break;
			default: {
				printf("Incorect input!\n");
			}
		}
	} while(execute);
	
	return 0;
}
	
