#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define PGSZ 4096 

/*
SIMEIWSEIS:
	bit vector. thelei pinaka bits kai oxi bytes!!!
	ti ginetai stin mnimi, eikonika pairnoume selides?

*/

long head_table;
struct node{
	char bits[128];
	int address;
	struct node* next;
	struct node* previous;
}node;

struct head{
	int size;
	struct node* list;
}head;

struct node* create_node(int size,int addr){
	int c,d;
	struct node * ptr = malloc(sizeof(struct node*));
	ptr->address = addr;
	c=PGSZ/size;
	for(d=0;d<c;d++)
		ptr->bits[c]=0;
	//prosoxi edw min kseperasoume se kapoion elegxo to megethos
}

int upperclass(int number){
int tmp=32;
	if(number<0){
		printf("Odd request! Reconsider\n");
		return -1;
	}
	while(tmp<number)
		tmp*=2;

	return tmp;
}

void *mymalloc(size_t cbytes){

	void *ptr;
	void * memory = malloc(256*PGSZ);
	int spare_pages,pages_used = 0,a;
	if((long int)memory%PGSZ==0)
		spare_pages=256;
	else
		spare_pages=255;
	while((long int)memory%PGSZ)
		memory+=sizeof(char);
	printf("Memory begins at:%ld\n",(long int)memory);

	struct head * table[8];
	for(a=0;a<8;a++){
		int b, sizee = 32;

		table[a]=malloc(sizeof(struct head));
		for(b=0;b<a;b++){
			sizee*=2;
		}
		printf("SIZE = %d\n",sizee);
		table[a]->size = sizee;
		table[a]->list = create_node(sizee,123);
	}
	
	


	if(cbytes>4096)	{
		ptr = malloc(cbytes);
		return ptr;
	}
	else if(cbytes>=0 && cbytes<=4096){
		//printf("%zd -> %d\n",cbytes,upperclass(cbytes));
		return ptr;
	}
	else{
	printf("Odd call of mymalloc\n");
	return NULL;
	}

}

int myfree(char * pointer){
return 0;
}

int main(int argc, char ** argv){


void *mptr = mymalloc(2587);
	
//free(memory);
return 0;

}
