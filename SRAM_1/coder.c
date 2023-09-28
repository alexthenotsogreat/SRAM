#include <sys/types.h>

#include <sys/time.h>

#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */

#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <math.h>

#include <stdbool.h> 

#include <string.h>



#define ND 256

const int M = 65536;

const int N = 100;

typedef struct TrieNode TrieNode;

struct TrieNode {
    // The Trie Node Structure
    // Each node has N children, starting from the root
    // and a flag to check if it's a leaf node
    unsigned char data; // Storing for printing purposes only
    TrieNode* children[ND];
    int is_leaf;
    int code;
};

void enc(char* block,TrieNode* root);

char** fileManage(char * iFilePath, long blockSize,char** blockBuffer);

char* addArr(char* first, char* second, char* dest);

long dictC=1;

int nIt = 0;


//Function quicker than sprinft(str,"%d",the int) but doesnt add comma --> further optimization would require to use this and add a comma manually
void intToString(int num, char* str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    // Reverse the string
    int length = i;
    for (int j = 0; j < length / 2; j++) {
        char temp = str[j];
        str[j] = str[length - j - 1];
        str[length - j - 1] = temp;
    }
}

TrieNode* make_trienode(unsigned char data) {
    // Allocate memory for a TrieNode
    TrieNode* node = (TrieNode*) calloc (1, sizeof(TrieNode));
    for (int i=0; i<ND; i++)
        node->children[i] = NULL;
    node->is_leaf = 2;
    node->data = data;
    return node;
}

void free_trienode(TrieNode* node) {
    // Free the trienode sequence
    for(int i=0; i<ND; i++) {
        if (node->children[i] != NULL) {
            free_trienode(node->children[i]);
        }
        else {
            continue;
        }
    }
    free(node);
}

TrieNode* insert_trie(TrieNode* root, unsigned char* word) {
    // Inserts the word onto the Trie
    // ASSUMPTION: The word only has lower case characters
    TrieNode* temp = root;
    unsigned int idx;
    for (int i=0; word[i] != '\0'; i++) {
        // Get the relative position in the alphabet list
         idx = (int) word[i];
        if (temp->children[idx] == NULL) {
            // If the corresponding child doesn't exist,
            // simply create that child!
            temp->children[idx] = make_trienode(word[i]);
        }
        else {
            // Do nothing. The node already exists
        }
        // Go down a level, to the child referenced by idx
        // since we have a prefix match
        temp = temp->children[idx];
    }
    // At the end of the word, mark this node as the leaf node
    
    temp->is_leaf = 1;
    temp->code=dictC;
    return root;
}

int search_trie(TrieNode* root, unsigned char* word)
{
    // Searches for word in the Trie
    TrieNode* temp = root;

    for(int i=0; word[i]!='\0'; i++)
    {
        int position = word[i];
        if(temp == NULL) return 0;
        if (temp->children[position] == NULL)
            return 0;
        temp = temp->children[position];
    }
   if (temp != NULL)
    if(temp->is_leaf != NULL)
        if(temp->is_leaf==1) 
        return temp->code;
    return 0;
}




//coder.exe inputFilePath.txt blockSize

//coder.exe file.txt 64000

int main(int argc, char *argv[]){

    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //Buffer to store the values of the file. Its going to be passed by reference
    char **blockBuffer;

    //--------> Operation <--------

    //1st: Read file
    
    char *p;
    long blockSize=strtol(argv[2],&p,10);   

    //blockBuffer = fileManage(argv[1],blockSize,blockBuffer);
    blockBuffer = fileManage("test3.txt",10000,blockBuffer);

    printf("\nDa autoria de Eduardo Cardoso (a89627) e Catarina Neves (a93088).");
    printf("\nFicheiro de entrada: %s. Ficheiro de saída: output.txt.",argv[2]);
    printf("\nTamanho máximo do dicionario: %d. Tipo de limpeza: remoção de todas as entradas e restituicao das 256 entradas ASCII",M);
    

    //2nd: Making dictionary and filling with ASCII Table
    
    TrieNode* root = make_trienode('\0');

        //printf("\n%s",blockBuffer[i]+1);
    dictC=1;
    for (int i = 1; i <= ND; i++)
    {
        unsigned char str[2]= "\0";
        str[0] = (unsigned char)i;
        root = insert_trie(root, str);
        dictC++;
    }

   

    //3rd: Encripts nIt amount of times, calculated in function of the size of the Block <--------
    for (size_t i = 0; i < nIt; i++)
    {
        write(STDOUT_FILENO,"\nBLOCK: ",8);
        enc(blockBuffer[i],root);     
    }

    //---------- // ----------

end = clock();
 
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

printf("\nTime used: %f",cpu_time_used);
printf("\nProcessados %d blocos, de tamanho %d bytes. O ultimo de tamanho %d bytes.",nIt,strlen(blockBuffer[0]),strlen(blockBuffer[nIt-1]));
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
    }
    //important to do since we are passing by reference
    blockBuffer=aux;
    //---------- // ----------
    return blockBuffer;

}



void enc(char* block,TrieNode* root){

    //Enc algorithm variables
    char *Pa = malloc(sizeof(char));
    char *Pb;

    int ind = 2;
    int i = 1;

    int code = 0;
    //---------- // ----------
    int j=0;

    long sizeBlock = strlen(block);

    //Varibles used to keep all the code values and then print them all at once. codeStackHelper just for arithmatics.
    unsigned char *codeStack=(unsigned char*)malloc(sizeof(unsigned char)*sizeBlock);
    unsigned char *codeStackHelper=(unsigned char*)malloc(sizeof(unsigned char));
    //---------- // ----------

    //Arithmatics stuff
    char *dest;
    unsigned char *blockAux = malloc(sizeof(unsigned char));
    memset(blockAux,0,sizeof(unsigned char));

    unsigned char *fixer = malloc(sizeof(unsigned char));
    memset(fixer,0,sizeof(unsigned char));

    char *fixer2 = malloc(sizeof(char));
    memset(fixer2,0,sizeof(unsigned char));

    unsigned char toPrint[100];
    memset(toPrint,NULL,100*sizeof(unsigned char));
    //---------- // ----------
    struct timeval tv;

    strncpy(Pa,block,2);
    Pa[0]=Pa[1];
    Pa[1]='\0';
    
    //More arihmatics stuff that we're scared to move
    char done2[1];
    char codeC;
    //---------- // ----------


    //Enc algorithm
    while(i+ind<=sizeBlock){

        int i = 1;

        char done[1];
        done[0]=block[ind];
        Pb=done;
       
        code=search_trie(root,Pa);

        //Keeps the code in codeStack to be printed, slowest part of the algorithm
        sprintf(codeStackHelper,"%d,",code);
        strcat(codeStack,codeStackHelper);
        //---------- // ----------

        done2[0]=block[ind+i];
        blockAux=done2;
        //Finding next Pb
               
        while ((ind+i<sizeBlock) && (search_trie(root,dest=addArr(Pb,blockAux,dest))))
        {
            i++;

            //has to be updated, inside the loop, after the incrementation on i, in order to be checked again in the while condition
            done2[0]=block[ind+i];
            blockAux=block[ind+i];
            Pb=dest;        
        }

        //Operators for array math
        int lenA = strlen(Pa);
        int lenB = strlen(Pb);
        int k, z;
        //---------- // ----------

        int iterations = lenB;

        if(lenA+lenB<=N){
            for (int l = 0; l < lenA; l++)
            {
                toPrint[l]=Pa[l];
            }

        
        
            for (k = 0, j = 0; k < iterations; k++, j++) {
                toPrint[lenA + k] = Pb[j];
                int iD=0;

                //Adding to dictionary, for some reason couldnt make it its own function
                //if the value is not on the dictionary

                if((iD=search_trie(root,toPrint))==0){

                    //and if the dictionary is full we gotta clean it, add the starter and reset dictC
                    if(dictC>=M){
                        //cleaning it
                        free_trienode(root);

                        //adding the starter dictionary
                        root = make_trienode('\0');
                        dictC=0;

                        for (int i = 1; i <= ND; i++)
                        {
                            unsigned char str[2]= "\0";
                            str[0] = (unsigned char)i;
                            root = insert_trie(root, str);
                            dictC++;
                        }       
                    }
                    //finally inserts it
                    insert_trie(root,toPrint);
                    dictC++;
                }
            }
        }

        ind+=i;

        //Dealing with sizes of the Pa and Pb
        if(lenB>=lenA)  memcpy(Pa,Pb,lenB);

        else{
            memset(Pa+lenB,NULL,lenA-lenB);
            memcpy(Pa,Pb,lenB);         
        }
        //---------- // ----------
        
        //Gotta clean stuff
        memset(toPrint,0,sizeof(char)*100);
    }

    memset(dest,0,strlen(dest));
    memset(fixer,0,strlen(fixer));
    memset(fixer2,0,strlen(fixer2));
    //---------- // ----------

    //Outputs Pb at the end of algoritm
    code=search_trie(root,Pb);
    sprintf(codeStackHelper,"%d,",code);     
    strcat(codeStack,codeStackHelper);

    //--------> File writing <--------
    //instead of opening and closing file every time the code changes, it stores all in codeStack and prints it all at the end
    FILE *outputFile;
    outputFile=fopen("output.txt","w");
    fwrite(codeStack,sizeof(unsigned char),strlen(codeStack),outputFile);
    //---------- // ----------

}

//Should probably just use strcat
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
