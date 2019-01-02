#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>

int isPower2(int n){
  int fail=0;
    while(n!=1)
    {
        if(n % 2 != 0){
            fail=1;
            break;
        }
        n=n/2;
    }
    if (fail == 1){
      return -1;
    }else{
      return 1;
    }
}

/* Global Variables */
    //1-Direct Map 2- N way assoc 3-fully assoc
    int cacheType;
    int cacheSize;
    int blockSize;
    int nway = 0;
    int nset;
    //1-FIFO,2-LRU
    int rpolicy;

  int found;
  int pt = 0;

    int tagBits = 0;
    int blockoffBits = 0;
    int indexBits = 0;

    unsigned long long int tag =0;
    unsigned long long int indx =0;
    unsigned long long blockOff =0;




    //a
     unsigned int reada = 0;
     unsigned int writea = 0;
     unsigned int missa = 0;
     unsigned int hita = 0;
    //b
     unsigned int readb=0;
     unsigned int writeb=0;
     unsigned int missb=0;
     unsigned int hitb=0;
      unsigned int readab = 0;
     unsigned int writeab = 0;
     unsigned int missab = 0;
     unsigned int hitab = 0;
    //b
     unsigned int readbb=0;
     unsigned int writebb=0;
     unsigned int missbb=0;
     unsigned int hitbb=0;

/* Structures */

//Cache Set
typedef struct setNode{
  struct blockNode * b;
  int linkCount; // 1 if first, otherwise will be 0 (Used for FIFO Replacement Algorithmn)
  unsigned long long int * recentTag;
}setNode;

//Cache Block (Cache Line)
typedef struct blockNode{
int valid ; // 0 if not valid, 1 if valid
 unsigned long int firstNode; // 0 if never used, otherwise will count number of accesses (Used for LRU Replacement Algorithmn)
unsigned long long int tag;
struct blockNode *next;
}blockNode;

struct setNode *set;



/* Function Declarations */
int inputIsValid(char** argv);
void initializeCache();
blockNode* setBlocks(blockNode * head);
void traverseBlocks(blockNode * head);
void printCache();
void simulate(char *argv);
void simulate2(char *argv);
void getBitSizes();
void addExtract(unsigned long long int add);
void cacheOperation(int op);
void cacheOperation2(int op);
void cacheOperationP(int op);
void printOutput();
void printVALIDCache();
void cacheOp(int op);
void cacheOpP(int op);
void cacheOpP2(int op);

/* Functions */
int main(int argc, char** argv){


  if (argc != 6){
    printf("error\n");
    return 0;
  }

  if (inputIsValid(argv) == -1){ 
    printf("error\n");
    return 0;
  }
 
 

  if (nway == 0){ nway = 1;}

int numSets = cacheSize / (nway * blockSize);
nset = numSets;


if (cacheType == 1){ //DIRECT MAP
    initializeCache();
    for (int i= 0; i< numSets; i++){
      set[i].b = setBlocks(set[i].b);
    }
    for (int i= 0; i< numSets; i++){
     //printf("set:%d",i);
    traverseBlocks(set[i].b);
    }
    //getBitSizes();
    simulate(argv[5]);
    initializeCache();
    for (int i= 0; i< numSets; i++){
      set[i].b = setBlocks(set[i].b);
    }
    for (int i= 0; i< numSets; i++){
     //printf("set:%d",i);
    traverseBlocks(set[i].b);
    }
    simulate2(argv[5]);
//printOutput();
}else if(cacheType == 3){ //FULLY ASSOC
////////////////////////////////////////////////////////////////
/*SPECIAL CACHE INITIALIZATION*/
    numSets =1;
    nset = numSets;
   /// printf("numSets: %d \n", numSets);
    set = (struct setNode*) malloc(sizeof(struct setNode) * numSets);
    for (int i = 0; i < numSets; i++){
     set[0].linkCount=0;
     set[0].b=NULL;
     set[0].recentTag = NULL;
     //printf("set[%d] initialized\n",i);
    }
/////////////////////////////////////////////////////////////////
    int numBlocks = cacheSize/blockSize;

    for (int i= 0; i< numBlocks; i++){
      set[0].b = setBlocks(set[0].b);
    }
   /// printf("numblocks: %d\n", numBlocks);
    nway = numBlocks;
    // printf("%d",numBlocks);
    // printCache();
      simulate(argv[5]);
      ////////////////////////////////////////////////////////////////
/*SPECIAL CACHE INITIALIZATION*/
    numSets =1;
    nset = numSets;
    ///printf("numSets: %d \n", numSets);
    set = (struct setNode*) malloc(sizeof(struct setNode) * numSets);
    for (int i = 0; i < numSets; i++){
     set[0].linkCount=0;
     set[0].b=NULL;
     set[0].recentTag = NULL;
   }
/////////////////////////////////////////////////////////////////
     numBlocks = cacheSize/blockSize;

    for (int i= 0; i< numBlocks; i++){
      set[0].b = setBlocks(set[0].b);
    }
    nway = numBlocks;
      simulate2(argv[5]);

}else if (cacheType == 2){ //N-WAY ASSOC
    initializeCache();
    //keep
    for(int j =0; j <nset; j++){
    for (int i = 0; i< nway; i++){
    set[j].b = setBlocks(set[j].b);
    }
    }
       simulate(argv[5]);
        initializeCache();
    for(int j =0; j <nset; j++){
    for (int i = 0; i< nway; i++){
    set[j].b = setBlocks(set[j].b);
    }
    }
    
      simulate2(argv[5]);
}
 //MEMORY ADDRESS ARE UP TO 48 BITS, USE 64 BIT INTEGERS: unsigned long long int 
    printOutput();
    return 0;
}

void simulate(char *argv){
  getBitSizes();
  if (rpolicy == 2){ //set lru array of recents 
    for (int i = 0; i <nset; i++){
      set[i].recentTag = (unsigned long long*) malloc(sizeof(unsigned long long) * (nway-1));
      for (int j=0; j<nway-1; j++){
          set[i].recentTag[j]=0;
      }
    }
}

  //Opening File
  FILE *fp;
  fp = fopen(argv,"r");
    char line[128];
		char pc[100];
		char mode;
		char address[100];
    
    while (fgets(line, 128, fp) != NULL) {
        int fpCount = sscanf(line, "%s %c %s", pc, &mode, address);
      if (fpCount==3) {
        unsigned long long int add = strtoull(address,NULL,0);
        addExtract(add);

				  if (mode=='R') {
          
           if (rpolicy == 2){
             cacheOp(0);
           }else{
            cacheOperation(0);
           }
           
				  }
				  else if (mode=='W') {
           
           if (rpolicy == 2){
                cacheOp(1);
           }else{
            cacheOperation(1);
           }
           
          } else {																																		
				
				  }
          
			} else if (fpCount==1) {
				if (strcmp(pc, "#eof")==0) {
					break;
				} else {
				}
			}
      int i =1;
        blockNode * ptr = set[0].b;
        while (ptr != NULL){
          i++;
          ptr = ptr->next;
        }
    }

}

void simulate2(char *argv){
  getBitSizes();
   if (rpolicy == 2){ //set lru array of recents 
    for (int i = 0; i <nset; i++){
      set[i].recentTag = (unsigned long long*) malloc(sizeof(unsigned long long) * (nway-1));
      for (int j=0; j<nway-1; j++){
          set[i].recentTag[j]=0;
      }
    }
}
  //Opening File
  FILE *fp;
  fp = fopen(argv,"r");
    char line[128];
		char pc[100];
		char mode;
		char address[100];
    
    while (fgets(line, 128, fp) != NULL) {
        int fpCount = sscanf(line, "%s %c %s", pc, &mode, address);
      if (fpCount==3) {
        unsigned long long int add = strtoull(address,NULL,0);
        addExtract(add);
				  if (mode=='R') {
           
           
           unsigned long long int i = indx;
            if (rpolicy == 2){
                cacheOpP(0);
                if (found ==0){
                       
              add = add + blockSize;
              addExtract(add);
              if (i != indx || cacheType == 3 || cacheType == 2){
                  pt = 1;
              }
              
              cacheOpP2(0);
            }
            }else{
            
            cacheOperation2(0);
            if (found ==0){

              add = add + blockSize;
              addExtract(add);
              if (i != indx || cacheType == 3){
                  pt = 1;
              }
              cacheOperationP(0);
            }
            }
				  }
				  else if (mode=='W') {
           
           unsigned long long int i = indx;
            if (rpolicy == 2){
                cacheOpP(1);
                if (found ==0){
                        //    printf("%llx -> %llx\n",add,add+blockSize);

              add = add + blockSize;
              addExtract(add);
              if (i != indx || cacheType == 3 || cacheType == 2){
                  pt = 1;
              }
              cacheOpP2(1);
            }
            
            }else{
            
            
            cacheOperation2(1);
            if (found == 0){
              add = add + blockSize;
              
              addExtract(add);
               if (i != indx || cacheType == 3){
                  pt = 1;
              }
              cacheOperationP(1);
            }
            }
          
          } else {																																		
				  }
			} else if (fpCount==1) {
				if (strcmp(pc, "#eof")==0) {
					break;
				} else {
				}
			}

    }
    

}

//0 read, 1 write
void cacheOperationP(int op){
  int found = 0;
  int ct = 0;
  
   int dex = (int)indx;
   
blockNode * ptr = set[dex].b;
if (pt ==1){
  ct=1;
}
 while(ptr != NULL){
    
   if (ptr->valid == 1 && ptr->tag == tag && ct==1){ //hit
  //printf("PREFETCH HIT\n");
   if (op == 0){
    //  hitb++;
   }else{
    // hitb++;
    // writeb++;
   }
   found = 1;
   break;
 }
 if (ptr->valid == 1 && ptr->tag == tag){
      ct++;
    }
 ptr = ptr->next;
}

if (found == 0){ //miss
//printf("PREFETCH MISS\n");
readb++;
blockNode * ptr = set[dex].b;
        while (ptr != NULL){
         if (ptr->valid != 1){
            break;
         }
          ptr = ptr->next;
        }
          if (ptr != NULL){ //space in set available
            ptr->valid = 1;
            ptr->tag = tag;
            if (set[dex].linkCount == 0){ //first element in set
              set[dex].linkCount = 1;
            }
          }else{//no space in set available
            blockNode * ptr = set[dex].b;
              for (int i= 1; i < set[dex].linkCount; i++){
                 ptr = ptr->next;
              }
              ptr->tag = tag;
              ptr->valid = 1;
              if (set[dex].linkCount + 1 <= nway){
              set[dex].linkCount++; //make sure to check if its not bigger then number of blocks
              }else{
                set[dex].linkCount = 1;
              }
          }

          if (op == 0){
            //missb++;
          }else{
           // missb++;
            //writeb++;
          }

}
}
//0 read, 1 write
void cacheOperation2(int op){
  found = 0;
   int dex = (int)indx;
blockNode * ptr = set[dex].b;
 while(ptr != NULL){
   if (ptr->valid == 1 && ptr->tag == tag){ //hit
   ///printf("HIT\n");
   if (op == 0){
      hitb++;
   }else{
     hitb++;
     writeb++;
   }
   found = 1;
   break;
 }
 ptr = ptr->next;
}

if (found == 0){ //miss
///printf("MISS\n");
readb++;
blockNode * ptr = set[dex].b;
        while (ptr != NULL){
         if (ptr->valid != 1){
            break;
         }
          ptr = ptr->next;
        }
          if (ptr != NULL){ //space in set available
            ptr->valid = 1;
            ptr->tag = tag;
            if (set[dex].linkCount == 0){ //first element in set
              set[dex].linkCount = 1;
            }
          }else{//no space in set available
            blockNode * ptr = set[dex].b;
              for (int i= 1; i < set[dex].linkCount; i++){
                 ptr = ptr->next;
              }
              ptr->tag = tag;
              ptr->valid = 1;
              if (set[dex].linkCount + 1 <= nway){
              set[dex].linkCount++; //make sure to check if its not bigger then number of blocks
              }else{
                set[dex].linkCount = 1;
              }
          }

          if (op == 0){
            missb++;
          }else{
            missb++;
            writeb++;
          }

}
}

//0 read, 1 write
void cacheOperation(int op){
  int found = 0;
   int dex = (int)indx;
blockNode * ptr = set[dex].b;
 while(ptr != NULL){
   if (ptr->valid == 1 && ptr->tag == tag){ //hit
   if (op == 0){
      hita++;
   }else{
     hita++;
     writea++;
   }
   found = 1;
   break;
 }
 ptr = ptr->next;
}

if (found == 0){ //miss
reada++;
blockNode * ptr = set[dex].b;
        while (ptr != NULL){
         if (ptr->valid != 1){
            break;
         }
          ptr = ptr->next;
        }
          if (ptr != NULL){ //space in set available
            ptr->valid = 1;
            ptr->tag = tag;
            if (set[dex].linkCount == 0){ //first element in set
              set[dex].linkCount = 1;
            }
          }else{//no space in set available
            blockNode * ptr = set[dex].b;
              for (int i= 1; i < set[dex].linkCount; i++){
                 ptr = ptr->next;
              }
              ptr->tag = tag;
              ptr->valid = 1;
              if (set[dex].linkCount + 1 <= nway){
              set[dex].linkCount++; //make sure to check if its not bigger then number of blocks
              }else{
                set[dex].linkCount = 1;
              }
          }

          if (op == 0){
            missa++;
          }else{
            missa++;
            writea++;
          }

}
}





























































void cacheOpP2(int op){
  int found = 0;
  int ct = 0;
  
   int dex = (int)indx;
   
blockNode * ptr = set[dex].b;
if (pt ==1){
  ct=1;
}
 while(ptr != NULL){
    
   if (ptr->valid == 1 && ptr->tag == tag && ct==1){ //hit
   if (op == 0){

   }else{

   }
  
   found = 1;
   break;
 }
 if (ptr->valid == 1 && ptr->tag == tag){
      ct = 1;
    }
 ptr = ptr->next;
}

if (found == 0){ //miss
//printf("PREFETCH MISS\n");
readb++;
blockNode * ptr = set[dex].b;
        while (ptr != NULL){
         if (ptr->valid != 1){
            break;
         }
          ptr = ptr->next;
        }
          if (ptr != NULL){ //space in set available
            ptr->valid = 1;
            ptr->tag = tag;
            ptr->firstNode = 1;
            
         for (int k = nway-2; k > 0; k--){        
   set[dex].recentTag[k]=set[dex].recentTag[k-1];
}

   //set this as last accessed node
   set[dex].recentTag[0] = tag;



          }else{//no space in set available
            blockNode * ptr = set[dex].b;
            blockNode * minptr = NULL;
            minptr = ptr;
             //printf("mininitial: %llx\n",minptr->tag);
            while (ptr != NULL){
              //if (ptr->firstNode <= minptr->firstNode){ //make sure this is not last accessed node 
              int flag = 0;
              for ( int i =0; i < nway-1; i++){
                if (ptr->tag == set[dex].recentTag[i]){
                  //printf("flag set\n");
                  flag = 1;
                }
              }
                if (flag == 0){ // does not exist in recent array
                minptr = ptr;
                }
                
              
              //}
              ptr = ptr->next;
            }
            ptr = set[dex].b;
              while (ptr != NULL){
                if (ptr == minptr){
                  break;
                }
                ptr = ptr->next;
              }
              //printf("min: %llx\n", minptr->tag);
              ptr = minptr;
              ptr->tag = tag;
              ptr->valid = 1;
              ptr->firstNode =1;
              blockNode * ptr2 = ptr;
               ptr = set[dex].b;
            while(ptr != NULL){
              ptr->firstNode =0; //making all firstNodes 0 in set due to it being full 
              ptr = ptr->next;
            }
            ptr2->firstNode = 1;
          }


    for (int k = nway-2; k > 0; k--){        
    set[dex].recentTag[k]=set[dex].recentTag[k-1];
}

   

   //set this as last accessed node
   set[dex].recentTag[0] = tag;

          if (op == 0){

          }else{
         
          }

}
}

















































//0 read, 1 write
void cacheOpP(int op){
   found = 0;
   int dex = (int)indx;
blockNode * ptr = set[dex].b;
//printf("nLRU COUNT %llu\n",ptr->firstNode);
 while(ptr != NULL){
   if (ptr->valid == 1 && ptr->tag == tag){ //hit
   //printf("HITyo\n");
   //printf("tag HIT is : %llx\n",tag);
   int aE =-1;
for(int i =0; i < nway-2; i++){
  if (tag == set[dex].recentTag[i]){
    aE = i;
    break;
  }
}
if (aE == -1){
   for (int k = nway-2; k > 0; k--){        
    set[dex].recentTag[k]=set[dex].recentTag[k-1];
}
}


if (aE !=-1){
//set[dex].recentTag[0] = tag;
unsigned long long int t = set[dex].recentTag[aE];
for (int i =aE; i > 0; i--){
set[dex].recentTag[i]=set[dex].recentTag[i-1];
}
set[dex].recentTag[0] = t;

}else{
  //printf("tag HIT is here and good : %llx\n",tag);

 set[dex].recentTag[0] = tag;
}
   //set[dex].recentTag[0] = tag;
   if (op == 0){
      hitb++;
   }else{
     hitb++;
     writeb++;
   }
   ptr->firstNode++;
   

   //set this as last accessed node
   
   found = 1;
   break;
 }
 ptr = ptr->next;
}

if (found == 0){ //miss
//printf("MISS\n");
readb++;
blockNode * ptr = set[dex].b;
        while (ptr != NULL){
         if (ptr->valid != 1){
            break;
         }
          ptr = ptr->next;
        }
          if (ptr != NULL){ //space in set available
            ptr->valid = 1;
            ptr->tag = tag;
            ptr->firstNode = 1;
           for (int k = nway-2; k > 0; k--){        
   set[dex].recentTag[k]=set[dex].recentTag[k-1];
}

   //set this as last accessed node
   set[dex].recentTag[0] = tag;
            //set[dex].recentTag = tag;
          }else{//no space in set available
            blockNode * ptr = set[dex].b;
            blockNode * minptr = NULL;
            minptr = ptr;


            while (ptr != NULL){
             // if (ptr->firstNode <= minptr->firstNode){ //make sure this is not last accessed node 
              int flag = 0;
              for ( int i =0; i < nway-1; i++){
                if (ptr->tag == set[dex].recentTag[i]){
                  flag = 1;
                }
              }
                if (flag == 0){ // does not exist in recent array
                minptr = ptr;
                }
                
              
              //}
              ptr = ptr->next;
            }
            ptr = set[dex].b;
              while (ptr != NULL){
                if (ptr == minptr){
                  break;
                }
                ptr = ptr->next;
              }
              ptr = minptr;
              ptr->tag = tag;
              ptr->valid = 1;
              ptr->firstNode =1;
              blockNode * ptr2 = ptr;
               ptr = set[dex].b;
            while(ptr != NULL){
              ptr->firstNode =0; //making all firstNodes 0 in set due to it being full 
              ptr = ptr->next;
            }
            ptr2->firstNode = 1;
          }
         // for (int k = 1; k < nway-2; k++){        
   // set[dex].recentTag[k]=0;
//}
    for (int k = nway-2; k > 0; k--){        
    set[dex].recentTag[k]=set[dex].recentTag[k-1];
}


   //set this as last accessed node
   set[dex].recentTag[0] = tag;
          if (op == 0){
            missb++;
          }else{
            missb++;
            writeb++;
          }


}
}




















































//0 read, 1 write
void cacheOp(int op){
  int found = 0;
   int dex = (int)indx;
blockNode * ptr = set[dex].b;
 while(ptr != NULL){
   if (ptr->valid == 1 && ptr->tag == tag){ //hit
   //printf("HIT\n");
   if (op == 0){
      hita++;
        int c = ptr->firstNode;
        c++;
        ptr->firstNode = c;
   }else{
     hita++;
     writea++;
      int c =  ptr->firstNode;
         c++;
         ptr->firstNode = c;
        // printf("ptrdata: %llx\n",ptr->tag);
         //printf("ptrfirstnode: %lu\n",ptr->firstNode);
   }
   int aE =-1;
for(int i =0; i < nway-2; i++){
  if (tag == set[dex].recentTag[i]){
    aE = i;
    break;
  }
}
if (aE == -1){
   for (int k = nway-2; k > 0; k--){        
    set[dex].recentTag[k]=set[dex].recentTag[k-1];
}
}


if (aE == -1){
set[dex].recentTag[0] = tag;
}else{
 unsigned long long int t = set[dex].recentTag[aE];
for (int i =aE; i > 0; i--){
set[dex].recentTag[i]=set[dex].recentTag[i-1];
}
set[dex].recentTag[0] = t;

}

   found = 1;
   break;
 }
 ptr = ptr->next;
}

if (found == 0){ //miss
//printf("MISS\n");
reada++;
blockNode * ptr = set[dex].b;
        while (ptr != NULL){
         if (ptr->valid != 1){
            break;
         }
          ptr = ptr->next;
        }
          if (ptr != NULL){ //space in set available
            ptr->valid = 1;
            ptr->tag = tag;
            //ptr->firstNode = 1;
             for (int k = nway-2; k > 0; k--){        
   set[dex].recentTag[k]=set[dex].recentTag[k-1];
}

   //set this as last accessed node
   set[dex].recentTag[0] = tag;
          }else{//no space in set available
            blockNode * ptr = set[dex].b;
            blockNode * minptr = NULL;
            minptr = ptr;
            
            while (ptr != NULL){
              //if (ptr->firstNode <= minptr->firstNode){ //make sure this is not last accessed node 
              int flag = 0;
              for ( int i =0; i < nway-1; i++){
                if (ptr->tag == set[dex].recentTag[i]){
                  flag = 1;
                }
              }
                if (flag == 0){ // does not exist in recent array
                minptr = ptr;
                }
                
              
              //}
              ptr = ptr->next;
            }
            ptr = set[dex].b;
              while (ptr != NULL){
                if (ptr == minptr){
                  break;
                }
                ptr = ptr->next;
              }
              ptr = minptr;
              ptr->tag = tag;
              ptr->valid = 1;
             // ptr->firstNode =1;
              //blockNode * ptr2 = ptr;
               ptr = set[dex].b;
            while(ptr != NULL){
             // ptr->firstNode =0; //making all firstNodes 0 in set due to it being full 
              ptr = ptr->next;
            }
            //ptr2->firstNode = 1;
          }
for (int k = nway-2; k > 0; k--){        
    set[dex].recentTag[k]=set[dex].recentTag[k-1];
}

   //set this as last accessed node
   //set[dex].recentTag[0] = tag;

   //set this as last accessed node
   set[dex].recentTag[0] = tag;
          if (op == 0){
            missa++;
          }else{
            missa++;
            writea++;
          }

}
}
//////////////////////////




















void initializeCache(){
int numSets = cacheSize / (nway * blockSize);
///printf("numSets: %d \n", numSets);
set = (struct setNode*) malloc(sizeof(struct setNode) * numSets);

for (int i = 0; i < numSets; i++){
  set[i].linkCount=0;
  set[i].b=NULL;
  set[i].recentTag = NULL;
  //printf("set[%d] initialized\n",i);
}


}

blockNode* setBlocks( blockNode * head){
  blockNode* nb = (blockNode *)malloc(sizeof(blockNode));
  nb->next=NULL;
  nb->valid=0;
  nb->tag=0;
  nb->firstNode=0;
  if (head == NULL){
    head = nb;
  }else{
    //blockNode* ptr = head;
    //while (ptr->next != NULL){
      //  ptr= ptr->next;
    //}
    //ptr->next = nb;
    nb->next = head;
    head = nb;
  }
  return head;
}

void getBitSizes(){
  int address = 48;
  blockoffBits = log2(blockSize);
  indexBits = log2(nset);
  tagBits = address - blockoffBits - indexBits;
  ///printf("num blockoffset bits:%d\n",blockoffBits);
  ///printf("num index bits:%d\n",indexBits);
  ///printf("num tag bits:%d\n",tagBits);
  ///printf("\n\n\n\n");
}


void addExtract(unsigned long long int add){
            int pow1 = (int) pow(2,blockoffBits);
            blockOff = add % pow1;
            //add = add >> blockoffBits;
            indx = add >> blockoffBits;
            //indx = add >> blockOff;
            int pow2 = (int) pow(2,indexBits);
            indx = indx % pow2;
            tag = add >> (indexBits + blockoffBits);
          ///printf("blockoff: 0x%llx\n", blockOff);
         ///printf("index: 0x%llx\n", indx);
          ///printf("tag: 0x%llx\n", tag);
}

void printVALIDCache(){
  for(int i = 0; i <nset;i++){

blockNode * ptr = set[i].b;
int j = 1;
while(ptr != NULL){
  if(ptr->valid == 1){
  printf("SET[%d]:\n",i);
  printf("Block[%d]: [ VALID:%d\tFIRSTNODE:%lu\tTAG:%llx ]\n",j,set[i].b->valid,set[i].b->firstNode,set[i].b->tag);
  printf("Linkcount: %d\n",set[i].linkCount);
  printf("\n");
  }
  j++;
  ptr = ptr->next;
}
}
}

void printCache(){
for(int i = 0; i <nset;i++){
printf("SET[%d]:\n",i);
blockNode * ptr = set[i].b;
int j = 1;
while(ptr != NULL){
  printf("Block[%d]: [ VALID:%d\tFIRSTNODE:%lu\tTAG:%llx ]\n",j,set[i].b->valid,set[i].b->firstNode,set[i].b->tag);
  printf("Linkcount: %d\n",set[i].linkCount);
  printf("\n");
  j++;
  ptr = ptr->next;
}
}
}








void printOutput(){
    printf("no-prefetch\n");
    printf("Memory reads: %d\n", reada);
    printf("Memory writes: %d\n", writea);
    printf("Cache hits: %d\n", hita);
    printf("Cache misses: %d\n", missa);
    printf("with-prefetch\n");
    printf("Memory reads: %d\n", readb);
    printf("Memory writes: %d\n", writeb);
    printf("Cache hits: %d\n", hitb);
    printf("Cache misses: %d\n", missb);
}







void traverseBlocks(blockNode * head){
 // printf("\n");
  if (head == NULL){
    printf("NO BLOCKS EXIST!\n");
  }
  blockNode *ptr = head;
  int i = 1;
 
  while(ptr != NULL){
     if (i > 1){
  //printf("->");
  }
    //printf("Block[%d]",i);
  
    ptr = ptr->next;
    i++;
  }
  //printf("\n");
}


/* 
Checks if argumets are correct
Sample Input: ./first 32 assoc:2 fifo 4 trace2.txt
Returns -1 if invalid, 1 if valid
*/
int inputIsValid(char** argv){
  //arg5 check
  FILE * tf;
  tf = fopen(argv[5], "r");
int num =0;
  if (tf == NULL){ return -1; }
//printf("training file good...\n");
 // int n = isPower2(atoi(argv[1]));
 if (atoi(argv[1]) == 0){ return -1; }
  if (isPower2(atoi(argv[1])) == -1){ return -1; }
  //printf("cache size good... \n");
  if (atoi(argv[4]) == 0){ return -1; }
  if (isPower2(atoi(argv[4])) == -1){ return -1; }
  //printf("block size good...\n");
  
   cacheSize = atoi(argv[1]);
   blockSize = atoi(argv[4]);

  if (blockSize > cacheSize){ return -1;}

  //printf("block size is not greater than cache size, we good...\n");
  if(strcmp(argv[2], "direct")==0){
    //printf("it is direct!\n");
    if (strlen(argv[2])!= 6){return -1;}
    cacheType = 1;
    return 1;
  }else if (strcmp(argv[2], "assoc") == 0 && strlen(argv[2]) == 5){
      //printf("it is associative!\n");
      cacheType = 3;
      return 1;
  }else{
      char *pch = strstr(argv[2], "assoc:");
      if (pch == NULL){
        return -1;
        //argv[6] to access num
       // printf("it is n way associative!\n");
      }
      //char n[1000];
      int s = strlen(argv[2]);
      s = s *100;
      //printf("s:%d",s);
      //strcat(n,&argv[2][6]);
      char *n = (char *) malloc(sizeof(char) * s);
      strcpy(n,&argv[2][6]);
       num = atoi(n);
      //printf("n is %d\n", num);
  }
  if (num <= 0){
    return -1;
  }
  if(num > cacheSize) {return -1;}
  if (isPower2(num) == -1){return -1;}
  int flag = 0;
  if (strcmp(argv[3], "lru") == 0){flag = 1; rpolicy=2;}
  if (strcmp(argv[3], "fifo") == 0){flag = 1; rpolicy=1;}
  //printf("flag: %d",flag);
  if (flag == 0){return -1;}
  nway = num;
  cacheType = 2;
return 1;
}