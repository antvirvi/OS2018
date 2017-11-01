	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	#include <limits.h>


	#define PGSZ 4096 
	#ifndef CHAR_BIT
		#define CHAR_BIT 8 
	#endif

	#define BVS 128  //bit vector size
	/*
	SIMEIWSEIS:
		bit vector. thelei pinaka bits kai oxi bytes!!!
		ti ginetai stin mnimi, eikonika pairnoume selides?

	*/

//	long head_table;

		//-----------------------------------------------------------------------------------------------

	struct node{
		unsigned char bitvector[16];
		int address;
		struct node* next;
		struct node* previous;
		}node;

	//-----------------------------------------------------------------------------------------------


	void * memory = NULL;
	int spare_pages, pages_used = 0;

	//------------------------------------------------------------
void init_vector(unsigned char *vec){
	int i;
	for(i=0;i<sizeof(vec);i++)
		vec[i]=0;
}

int checkbit(unsigned char *bitv,int bit){ //return the bit'th bit of bitv

		int a = bit/CHAR_BIT; //part of char array we want
//		printf("\nbit %d is on the %dth part of bitvector\n",bit,a);
		int b = (bit)%CHAR_BIT;
		int c = pow(2,b);
		unsigned char test = bitv[a];
		test &= c;
		if(test!=0){
		//	printf("The %dth bit of bivector is 1\n",bit);
			//printf("-- %d --\n",test);
			return 1;		//	return 0 if it exists
		}
		else
	//		printf("Zeor bit\n");	printf("-- %d --\n",test);
		return 0;
}

int addbit(unsigned char *bitv,int bit){ //return the bit'th bit of bitv

		int a = bit/CHAR_BIT; //part of char array we want
//		printf("\nbit %d is on the %dth part of bitvector\n",bit,a);
		int b = (bit)%CHAR_BIT;
		int c = pow(2,b);
		bitv[a] |= c;
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

int request_page(void){
	
		if((memory==NULL)||(spare_pages==0)){		// an den exei arxikopoithei i mnimi,e inai diladi i prwti klisi tis mymalloc, i einai adeia i pool apo selides
			memory = malloc(256*PGSZ);
			pages_used = 0;
			if((long int)memory%PGSZ==0)
				spare_pages=256;  //an exoume pollaplasio tou 4096 dieythinsi tote exoume 256 diathesimes selides
			else
				spare_pages=255;   //an exoume pollaplasio tou 4096 dieythinsi tote exoume 256 diathesimes selides
			while((long int)memory%PGSZ)
				memory+=sizeof(char);
			printf("Memory begins at:%ld\n",(long int)memory);
		}

		//request new page an ftasoume edw
		if(spare_pages>0){
			pages_used++;
			spare_pages--;
			void *ptr = memory;
			memory+=PGSZ; //metakinoume ton deikti tis mnimis kata mia selida enw kratame tin progoumeni timi tou.
//			return ptr;
			return (long int)ptr;		
		}
}

struct node* create_node(struct node * ptr){

	ptr = malloc(sizeof(struct node));
	ptr->address = request_page();
	init_vector(ptr->bitvector);
	ptr->next = NULL;
	ptr->previous = NULL;
		
	}



void * return_memory(int size){ // h synartisi ayti koitaei mesa sto idnex an iparxei diathesimos xoros se mia selida





}


	void *mymalloc(size_t cbytes){

		void *memptr; //o deiktis stin thesi mnimis pou tha epistrafei
		int a; 

		if(cbytes>4096)	{  //an zitaei polu megalo megethos to afinoume stin malloc kai termatizoume kateytheian epistrefontas ton ptr
			memptr = malloc(cbytes);
			return memptr;
		}
		

		
	


		struct node * table[8];  //dimiourgia tou pinaka deiktwn sta nodes
		for(a=0;a<8;a++){		//arxikopoisi tou pinaka se NULL
			table[a]=malloc(sizeof(struct node*));
			table[a] = NULL;
		}
		
		

		if(cbytes>=0 && cbytes<=4096){ 			//an einai kalo megethos kanoume mymalloc

			if(table[pos(cbytes)]==NULL){  //an einai NULL ayti i selida, diladi den exei ksanaxrisimopoieithei tetoio megethos, to dimiourgoume
				table[pos(cbytes)] = create_node(table[pos(cbytes)]);
			printf("Dimiourgia node gia tin apothikeysi %lubytes stin klasi %d\n",cbytes,upperclass(cbytes));
			memptr = NULL;  //ayto tha fygei, einai dokimastiko	
			return memptr;  //ayto tha fygei, einai dokimastiko
			}
	
			int i;
			for(i=0;i<PGSZ/upperclass(cbytes);i++){
//				if(table[pos(cbytes)]->bits[i]==0)
	{
					//PAIRNOUEM ATYI TI THESI MNIMIS
					return memptr;
				}	
			}
			
		

		//printf("%zd -> %d\n",cbytes,upperclass(cbytes));
			return memptr;
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
unsigned char panos[16];
	panos[0]= 255;
	panos[1]= 255;
	panos[2]= 255;
	panos[3]= 255;
	panos[4]= 255;
	panos[5]= 255;
	panos[6]= 255;
	panos[7]= 255;
	panos[8]= 255;
	panos[9]= 255;
	panos[10]= 255;
	panos[11]= 255;
	panos[12]= 255;
	panos[13]= 255;
	panos[14]= 255;
	panos[15]= 255;
int a;
	for(a=0; a<BVS;a++)
{		if(!checkbit(panos,a))
			printf("Panos failed %d\n",a);
		 
}
unsigned char number = 255;
	printf("char = %d\n",number);
	
	return 0;
}
