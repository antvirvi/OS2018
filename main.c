	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <math.h>
	#include <limits.h>


	#define PGSZ 4096
	#define HASH 1024  

	#define BVS 128  //bit vector size

		//-----------------------------------------------------------------------------------------------

	struct node{
		unsigned char bitvector[16];
		void * page_start;
		int size;
		struct node* next;
		struct node* previous;
		}node;


	struct bucket{
		size_t address;
		struct node * page_address;
		struct bucket * next;
		};

	//-----------------------------------------------------------------------------------------------
	struct node ** lists = NULL;
	struct bucket * hashtable = NULL; //dimiourgia global deikti sto hashtable
	void * memory = NULL; //dimourgia global deikti stin mnimi
	int spare_pages, pages_used = 0;


int hash(size_t adr){
	printf("hash\n");
	int a = adr%HASH;
	printf("\thash returns %d bucket\n",a);
	return a;
}


int create_lists(){
	printf("create lists\n");
	if(lists!=NULL)
		return 0;
	else{
		int i; 
		lists = malloc(8*sizeof(struct node *));
		for(i=0;i<8;i++){
			lists[i]=malloc(sizeof(struct node *));
			lists[i]=NULL;
		//	printf("\tend Create lists \n");
		}
	}
		
printf("\tend create lists\n");
}

int insert_bucket(void * pointer, struct node * page){ //edw prosthetoume mia timi sto hashtable
	printf("insert bucket\n");
	unsigned int address =  (size_t)&pointer;
//	printf("Pointerr address !! %#16x   %p\n",address,(size_t)&pointer);

	int position = hash(address);
	
	if(hashtable[position].address!=0){
		struct bucket * ptr = hashtable[position].next;
		while(ptr!=NULL)
			ptr=ptr->next;
		ptr=malloc(sizeof(struct bucket));
		ptr->address = address;
		ptr->next = NULL;
		ptr->page_address = page;
		return 1;
	}
	else{
		hashtable[position].address = address;
		hashtable[position].next = NULL;
		hashtable[position].page_address = page;
		return 1;
	}
}



int check_bucket(void * pointer, struct node * page){ //edw prosthetoume mia timi sto hashtable
	printf("check bucket\n");
	unsigned int address =  (size_t)&pointer;

	int position = hash(address);
	struct	bucket * ptr;
	if(hashtable[position].address!=0){
		if (hashtable[position].address == address){
			return 1;
		}
		ptr =  hashtable[position].next;
		while(ptr!=NULL){
			if (hashtable[position].address == address){
				return 1;}
			ptr=ptr->next;
		}
	return 0;
	}

	else{
		return 0;
	}
}



int create_hashtable(){//dimiourgia tou hashtable an den uparxei
	printf("create hash table\n");	
	if(hashtable!=NULL)
		return 0;
	else{
		hashtable = malloc(HASH*sizeof(struct bucket));
		int i;
		for(i=0;i<HASH;i++){
			hashtable[i].address=0;
			hashtable[i].page_address = NULL;
			hashtable[i].next = NULL;
		}
		printf("\t end create hash table\n");	
		return 1;
	}
}


size_t round_down(size_t adr){ //epistrfei to pollaplasio tou 4096 sto opoio anikei to adr
	printf("Round down\n");
	size_t i = adr % PGSZ;
	size_t j = adr - i;
	printf("%u\n", (unsigned int)adr);	
	printf("%u\n", (unsigned int)j);
	printf("\tend Round down\n");
	return i;
}

int checkbit(unsigned char *bitv,int bit){ //return the bit'th bit of bitv
	int a = bit/CHAR_BIT; //part of char array we want
//		printf("\nbit %d is on the %dth part of bitvector\n",bit,a);
	int b = (bit)%CHAR_BIT;
	int c = pow(2,b);
	unsigned char test = bitv[a];
	test &= c;

	if(test!=0){
		return 1;		//	return 0 if it exists
	}
	else{
		return 0;
	}
}

int addbit(unsigned char *bitv,int bit){ //raisethe  bit'th bit of bitv
	printf("Add bit\n");
	int a = bit/CHAR_BIT; //part of char array we want
	printf("we want the %dth part of bitvector\n",a);
	printf("bitv[%d] is %c\n",a,bitv[a]);
	bitv[a] |= 1 <<bit;
	printf("\tend of Add bit\n");

//check if was succesfully completed VV

	int b = (bit)%CHAR_BIT;
	int c = pow(2,b);
	unsigned char test2 = bitv[a];

	if(test2 &=c ){
		return 1;		//	return 1 if it is completed succesfully
	}
	else
		return 0;
	
}

int delbit(unsigned char *bitv,int bit){ //return the bit'th bit of bitv
	if(!checkbit(bitv,bit))
		return 1;
	else{	
		int a = bit/CHAR_BIT; //part of char array we want
	//		printf("\nbit %d is on the %dth part of bitvector\n",bit,a);
		int b = (bit)%CHAR_BIT;
		int c = pow(2,b);
		bitv[a] ^= c;
		if(!checkbit(bitv,bit))
			return 1;
		else return 0;
		}
}


void init_vector(unsigned char *vec,int size){

	int limit = 4096 / size ;  //limit is the quantity of addresses in a page
	printf("init vector for %d bytes\n",size);
	int i,j;
	for(i=0;i<16;i++){
		vec[i] &= 0;
	//	printf("vec[%d] is %d\n",i,vec[i]);
	}
	printf("\t end init vector\n");
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

int pos(int number){
	int tmp=32, i=0;
		if(number<0){
			printf("Odd request! Reconsider\n");
			return -1;
		}
		while(tmp<number){
			tmp*=2;
			i++;
		}
		return i;
}

void * request_page(void){
	printf("request page\n");
	void * ret_ptr;
	if((memory==NULL)||(spare_pages==0)){		// an den exei arxikopoithei i mnimi,e inai diladi i prwti klisi tis mymalloc, i einai adeia i pool apo selides
		memory = malloc(256*PGSZ);
		pages_used = 0;
		if((size_t)memory%PGSZ==0)
			spare_pages=256;  //an exoume pollaplasio tou 4096 dieythinsi tote exoume 256 diathesimes selides
		else
			spare_pages=255;   //an exoume pollaplasio tou 4096 dieythinsi tote exoume 256 diathesimes selides
		while((size_t)memory%PGSZ)
			memory+=sizeof(char);
	}

	//request new page an ftasoume edw
//	mem_ptr = memory;
	if(spare_pages>0){
		pages_used++;
		spare_pages--;
		memory+=PGSZ; //metakinoume ton deikti tis mnimis kata mia selida enw kratame tin progoumeni timi tou.
		void *ptr = memory;
		printf("\tend request page\n");
		return ptr;		

	}
}

struct node * create_node(struct node * ptr, int size){
	printf("create node\n");
	ptr = malloc(sizeof(struct node));
	ptr->page_start = request_page();
	printf("===Size %d, pos %d\n",size, pos(size));
	init_vector(ptr->bitvector,size);
	ptr->size = size ;	

	if(lists[pos(size)]==NULL)
		lists[pos(size)] = ptr;
	else{
		struct node * tmp = ptr;
		lists[pos(size)]->next = ptr;
		ptr->next=tmp;
		tmp->previous = ptr;
		ptr->previous = NULL;
	}
printf("\tend create node\n");
	return ptr;
}


void * return_memory(struct node * listptr,int size){ // h synartisi ayti koitaei mesa sto index an iparxei diathesimos xoros se mia selida kai epistrefei ti thesi mnimi tis
	printf("Return memeory\n");
	struct node * pointer = listptr, * previous;
	int i;
	int limit = PGSZ/size;
	while(pointer!=NULL){
		printf("node we are checking is not NULL\n");
		for(i=0;i<limit;i++){
			if(checkbit(pointer->bitvector,i)==0){
				printf("return memory0 %s %d\n",pointer->bitvector,i);
				addbit(pointer->bitvector,i);
				insert_bucket(pointer->page_start +(i*size), pointer);
				return pointer->page_start + (i*size);
			}
		}
	//	previous = pointer;
		pointer = pointer ->next;
		//printf("return memory>>%p %p\n",pointer, previous);
	}
	printf("Should create node\n");
	pointer = create_node(pointer,size);
//	printf("Pointer bitvector %d\n",(int)pointer->bitvector[0]);
	addbit(pointer->bitvector,0);//kai edw annanewnoume to hash table
	printf("Check vroum vroum\n");
	insert_bucket(pointer->page_start+(i*size), pointer);
	return pointer->page_start;	

}

void bring_to_start(struct node * page_ptr, struct node * head){
	struct node * help_previous, *help_next;
	help_previous = page_ptr->previous;
	help_next = page_ptr->next;
	help_next->previous = help_previous;
	help_previous->next = help_next; //kleisame tin trupa kai exoume ton page ptr meteoro
	
	head->previous = page_ptr;
	head = page_ptr;
}

void *mymalloc(size_t cbytes){
	printf("mymalloc\n");
	void *memptr; //o deiktis stin thesi mnimis pou tha epistrafei
	int a; 

	if(cbytes>4096)	{  //an zitaei polu megalo megethos to afinoume stin malloc kai termatizoume kateytheian epistrefontas ton ptr
		memptr = malloc(cbytes);
		return memptr;
	}
	
	if(!create_lists()){perror("create index");}
	if(!create_hashtable()){perror("create index");}
	if(cbytes==0)
		return NULL;
	else if(cbytes>0 && cbytes<=4096){ 			//an einai kalo megethos kanoume mymalloc

//	printf("%zd -> %d\n",cbytes,pos(cbytes));
		memptr = return_memory(lists[pos(cbytes)],cbytes);
//		*memptr = mem_a;
		return memptr;
	}
	else{
		printf("Odd call of mymalloc\n");
		return NULL;
	}

}



int myfree(void * pointer){
	size_t mom_address = round_down((size_t)&pointer);
	
	return 0;
}

//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//

int main(int argc, char ** argv){


char * array =  mymalloc(8*sizeof(char));
	strcpy(array,"hashfoo231231231312321ASDAD");
	printf("%s\n",array);

/*
		void *mptr = malloc(0);
		printf("%zd\n",sizeof(mptr));
		printf("Memory address: %p\n",mptr);
	*/
myfree(array);
	return 0;
}
