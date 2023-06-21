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
void enc(char *block,char dictionary[65536][100]);
char** fileManage(char * iFilePath, long blockSize,char** blockBuffer);
int searchDictionary(char* pattern,char dictionary[65536][100]);
char* addArr(char* first, char* second, char* dest);
char** insertDictionary(char dictionary[65536][100],char* pattern);
long dictC=1;
int nIt = 0;




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

    //1st: Read file
    
    char *p;
    long blockSize=strtol(argv[2],&p,10);


    
    //blockBuffer = fileManage(argv[1],blockSize,blockBuffer);
    blockBuffer = fileManage("test3.txt",70000,blockBuffer);

    
    
    
    
char dictionary[65536][100];




memset(dictionary,0,sizeof(dictionary[0][0])*M*N);
 for (int r = 0; r < M; r++) {
        //memset(dictionary[r],NULL,100);
        memcpy(dictionary[r],"\0",sizeof(char)*1);
        
    }

for (int i = 1; i <= 256; i++)
{
    int *fixer = malloc(sizeof(int));
    *fixer=i;
    memcpy(dictionary[i],fixer,sizeof(char)*100);
}
dictC = 257;



for (size_t i = 0; i < nIt; i++)
    {
        //printf("\n%s",blockBuffer[i]+1);
        enc(blockBuffer[i],dictionary);
    }


//memcpy(dictionary[3],"AB",sizeof(char)*100);
//memcpy(dictionary[3],"BA",sizeof(char)*100);
//enc("0ABABABBABABAABBABBAB",dictionary);
//   0ABABABBABABAABBABBAB

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
    long fileSize =ftell(inputFile);
    nBlocks=ceil((fileSize/blockSize))+1; //number of block is = round_up(file_size/block_size). rounded up since no byte is to be left out
    nIt = nBlocks;  
    fseek(inputFile,0,SEEK_SET);
    printf("\nN Size: %ld",nBlocks);
    printf("\nBlocksize: %ld",blockSize);

    long toRead = 0;

    //Allocates the amount of memory needed for the matrix rows (and since C99, you don't need to individually allocate memory for each row itself)
    char ** aux = malloc((nBlocks)*sizeof(char*));

    

    //Cycle that goes through the blocks on the file, checks to see if they are able to be read, stores values read into the buffer and seeks for the next block
    for(int i=0;i<nBlocks;i++){
        //probably doesnt need the test and the aux variable, but kept getting werid values without them
        //the test+1 is because, since in the enc fucntion the indexing is started at 1, it needs here to be loaded into trhe second position: 1.
        char * test = (char*)malloc((blockSize+1)*sizeof(char));
        //write(STDOUT_FILENO,&i,sizeof(i));
        long lastPos = ftell(inputFile);
        if(fread(test+1,1,blockSize,inputFile)==0){
            write(STDOUT_FILENO,"\nError reading block number: ",29);       
        }else{
            aux[i]=test;
        }
        
        //the first positon is atributed into a '0' ofr the same reason has its being read into the second position --> indexing in enc is started at 1
        aux[i][0]='0';
        //printf("\n%s",aux[i]);
    }
    //important to do since we are passing by reference
    blockBuffer=aux;
    //---------- // ----------

    
    return blockBuffer;
}

void enc(char* block,char dictionary[65536][100]){
    
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
            dictionary = insertDictionary(dictionary,toPrint);
            
        }
        //------------------------------------------------------------------
        ind+=i;

        write(STDOUT_FILENO,Pa,strlen(Pa));
        if(lenB>=lenA)  memcpy(Pa,Pb,lenB);
        else{
            memset(Pa+lenB,NULL,lenA-lenB);
            memcpy(Pa,Pb,lenB);
            
        }
        
        
        memset(toPrint,0,sizeof(char)*100);

        
        
        
    }
    
    
    write(STDOUT_FILENO,Pb,strlen(Pb));
    //@TODO output code
    

}

int searchDictionary(char* pattern,char dictionary[65536][100]){
    
    int i = 0;
    char *comp = "\n";
    
    

    while(i<M && dictionary[i][0]!='\0'){
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

char** insertDictionary(char dictionary[65536][100],char* pattern){

    int iD=0;
    if((iD=searchDictionary(pattern,dictionary))==0){
    if(dictC>=M){
        for (size_t i = 0; i < M; i++)
        {
            memcpy(dictionary[i],"\0",sizeof(char)*1);
        }
        dictC = 0;

        for (int i = 1; i <= 256; i++)
        {
            int *fixer = malloc(sizeof(int));
            *fixer=i;
            memcpy(dictionary[i],fixer,sizeof(char));
        }
        dictC = 257;
    }
    memcpy(dictionary[dictC],pattern,sizeof(char)*100);
    dictC++;
    }else return iD;

    
    return dictionary;
}
