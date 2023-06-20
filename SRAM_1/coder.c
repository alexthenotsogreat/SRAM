#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


const int M = 65536;
const int N = 100;
void enc(char *block,char **dictionary);
char** fileManage(char * iFilePath, long blockSize,char** blockBuffer);
int searchDictionary(char* pattern,char **dictionary);
char* addArr(char* first, char* second, char* dest);
long dictC=1;




//coder.exe inputFilePath.txt blockSize
//coder.exe file.txt 64000
int main(int argc, char *argv[]){
    //--------> Variable Declaration <--------

    //Buffer to store the values of the file. Its going to be passed by reference
    char **blockBuffer;

    //---------- // ----------

    //--------> Cleaning dictionary <--------
    

    //---------- // ----------

    //--------> Operation <--------
/*
    //1st: Read file
    char *p;
    long blockSize=strtol(argv[2],&p,10);


    
    blockBuffer = fileManage(argv[1],blockSize,blockBuffer);

    //---------- // ----------

    //2nd: Process file
    //@TODO pass rest of blocks aswell
    //Number of rows is sizeof(blockBuffer)/sizeof(blockBuffer[0])-the part that isnt filled from the last block
    //enc(blockBuffer[0]);

    //---------- // ----------
*/  
char **dictionary = (char**)malloc(M*sizeof(char));
for (int r = 0; r < M; r++) {
        dictionary[r] = (char *)malloc(N * sizeof(char));
        memcpy(dictionary[r],"\n",sizeof(char)*100);
    }



memcpy(dictionary[1],"A",sizeof(char)*100);
memcpy(dictionary[2],"B",sizeof(char)*100);
//memcpy(dictionary[3],"AB",sizeof(char)*100);
//memcpy(dictionary[3],"BA",sizeof(char)*100);
enc("0ABABABBABABAABBABBAB",dictionary);
//    ABABABBABABAABBABBAB

/*
for (int i = 0; i < M; i++) {
        free(dictionary[i]);
    }
    free(dictionary);
*/
return 0;
}


//This that opens the input file, reads it block by block and stores it into the global blockBuffer
char** fileManage(char * iFilePath, long blockSize,char** blockBuffer){
    //--------> Variable Declaration <--------
    
    //Variable for the input file
    FILE* inputFile;
    //Variable to hold number of blocks
    long nBlocks = 0;

    //--------> File Reading <--------
    
    //Opens input File
    inputFile =fopen(iFilePath,"r");

    //Seeks for the end of the file (lseek has O(log n) complexity in the worst case but mostly O(1). So its quick to get file size most of the times.
    //It also doesnt have to go through every position, it acquires the last position of the file from meta data written on file creation
    fseek(inputFile,0,SEEK_END);
    nBlocks=ceil((ftell(inputFile)/blockSize))+1; //number of block is = round_up(file_size/block_size). rounded up since no byte is to be left out
    fseek(inputFile,0,SEEK_SET);

    //Allocates the amount of memory needed for the matrix rows (and since C99, you don't need to individually allocate memory for each row itself)
    char ** aux = malloc(nBlocks*sizeof(char*));
    

    //Cycle that goes through the blocks on the file, checks to see if they are able to be read, stores values read into the buffer and seeks for the next block
    for(int i=0;i<nBlocks;i++){
        //probably doesnt need the test and the aux variable, but kept getting werid values without them
        char * test = (char*)malloc(blockSize*sizeof(char));
        write(STDOUT_FILENO,&i,sizeof(i));
        if(fread(test,1,blockSize,inputFile)==0){
            //just to print the block where the error happened
            write(STDOUT_FILENO,"\nError reading block number: ",29);
            
            char c = i+'0';
            write(STDOUT_FILENO,c,1);
        }else{
            aux[i]=test;
        }
    }
    //important to do since we are passing by reference
    blockBuffer=aux;
    //---------- // ----------

    
    return blockBuffer;
}

void enc(char* block,char **dictionary){
    
    char *Pa;
    char *Pb;
    int ind = 2;
    int i = 1;
    int code = 0;
    int j=0;
    long sizeBlock = strlen(block);
    char *dest;
    char *blockAux = malloc(sizeof(char));
    char *fixer = malloc(sizeof(char));
    char *fixer2 = malloc(sizeof(char));
    char toPrint[100];
   

    //@TODO insert all symbols be4

    //blockAux needs to be used here since we cant know the length the Pa and Pb blocks before the ending of the algorithm
    *fixer=block[i];
    Pa=fixer;

    while(i+ind<=sizeBlock){
        int i = 1;
        *fixer2=block[ind];
        Pb=fixer2;

        code=searchDictionary(Pa,dictionary);
        *blockAux=block[ind+i];
        //Finding next Pbs
        //blockAux is needed to convert char to array (to be concatenated with Pb) in a safe manner
        
        while ((ind+i<sizeBlock) && (searchDictionary(dest=addArr(Pb,blockAux,dest),dictionary)))
        {
            i++;
            //has to be updated, inside the loop, after the incrementation on i, in order to be checked again in the while condition
            *blockAux=block[ind+i];

            Pb=dest;

        }
        /*
        j=1;
        while(j<=i){
            *blockAux=block[ind+j];
            memcpy(dictionary[dictC],dest=addArr(Pa,blockAux,dest),sizeof(char)*100);
            
            j++;
            dictC++;
        }
        */
        //Replacement while teach doesnt answear
        int lenA = strlen(Pa);
        int lenB = strlen(Pb);
        int k, z;

        // Determine the maximum number of iterations
        int iterations = lenB;

        for (int l = 0; l < lenA; l++)
        {
            toPrint[l]=Pa[l];
        }
        
        for (i = 0, j = 0; i < iterations; i++, j++) {
            toPrint[lenA + i] = Pb[j];
            memcpy(dictionary[dictC],toPrint,sizeof(char)*100);
            dictC++;
            //@TODO DONT ADD THE ONES IT ALREADY HAS
        }
        //------------------------------------------------------------------
        ind+=i;
        
        write(STDOUT_FILENO,Pa,sizeof(Pa));
        memcpy(Pa,Pb,sizeof(Pb));
        
        
        memset(toPrint,0,sizeof(char)*100);

        
        
        
    }
    
    
    write(STDOUT_FILENO,Pb,sizeof(Pb));
    //output code
    

}

int searchDictionary(char* pattern,char **dictionary){
    
    int i = 0;
    char *comp = malloc(sizeof(char)*100);
    memcpy(comp,"\n",sizeof(char)*100);


    while(strcmp(dictionary[i],comp)!=0){
        if(strcmp(dictionary[i],pattern)==0){
            return i;
        }
        i++;
    }
    

    
    return 0;
}


char* addArr(char* first, char* second, char* dest){

    int firstN = strlen(first);
    int secondN = strlen(second);
    

    char *total;
    dest = (char*)malloc(firstN+secondN);

    int j = 0;
    
    for (size_t i = 0; i < firstN; i++)
    {
        dest[j]=first[i];
        j++;
    }

    for (size_t i = 0; i < secondN; i++)
    {
        dest[j]=second[i];
        j++;
    }
    
    

    return dest;

}
