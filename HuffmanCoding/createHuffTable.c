#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct CharNode{
	char c;
	float weight;
	int Huffcode;
	struct CharNode *left, *right;
}CharNode;

// Takes in the file name and constructs the probability table foe all
// the ASCII charecters that appear in that file .
float* constructFreqTable(char *filename, int *filelength){
	int i = 0;
	float *probabilityTable = (float*)malloc(sizeof(float)*128);
	for(i = 0; i<128; i++){
		probabilityTable[i] = 0.0;
	}

	// to get the total number of charecters in the file
	FILE *fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	*filelength = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// I am not assigning the direct addition value of (1/filelength) here because, 
	// the accuracy will go down as the file sizes increase , cauzing a very very small value
	// to be held in the float variable which is not a scalable idea
	// this takes more time, but a more accurate thing to do
	while(!feof(fp)){
		probabilityTable[fgetc(fp)] += 1.0;
	}

	// The following freeing part gave me an error, 
	// if someone could tell me why , it would be of great help !
	// fclose(fp);

	for(i = 0; i<128; i++){
		probabilityTable[i] /= (float)(*filelength);
	}

	return(probabilityTable);
}


int count(CharNode **HuffTree, int countChars){
	int count = 0;
	for(int i = 0; i < countChars; i++){
		if(HuffTree[i] != NULL){
			count++;
		}
	}
	return(count);
}


// this function does the building of the binary tree, 
// the traversal of the binaary tree will give us the huffman code for each charecter
// an empty string is returned in case the charecter is not used at all
void add(CharNode **HuffTree, CharNode *resultant, int *countChars){
	int i = 0;
	HuffTree[(*countChars)] = resultant;
	*countChars += 1;
}

CharNode* minimumNode(CharNode **HuffTree, int *countChars){
	CharNode* ret = (CharNode*)malloc(sizeof(CharNode));
	int  i = 0, j = 0;
	float min = 2.0;

	for(i = 0; i< (*countChars); i++){
		if((HuffTree[i])->weight < min){
			ret->weight = (HuffTree[i])->weight;
			ret->left = (HuffTree[i])->left;
			ret->right = (HuffTree[i])->right;
			ret->c = (HuffTree[i])->c;
			min = (HuffTree[i])->weight;
			j = i;
			//printf("%f\n", min);
		}
	}
	
	HuffTree[j] = NULL;
	for(i = j; i < (*countChars) -1 ; i++){
		HuffTree[i] = HuffTree[i+1];
	}

	*countChars -= 1;
	return(ret);
}


void checkin(CharNode *root){
	if(root->left != NULL){
		checkin(root->left);
	}
	printf("%c\n", root->c);
	if(root->right != NULL){
		checkin(root->right);
	}
}

void checkpos(CharNode *root){
	if(root->left != NULL){
		checkpos(root->left);
	}
	if(root->right != NULL){
		checkpos(root->right);
	}
	printf("%c\n", root->c);
}

CharNode** makeHuffTree(float* probabilityTable){
	int i = 0, countChars = 0, j = 0;
	char **encodingTable;
	CharNode *min1, *min2, *resultant;

	for(i = 0; i < 128 ; i++){
		if(probabilityTable[i] != 0){
			countChars++;
		}
	}

	CharNode **HuffTree = (CharNode**)malloc(sizeof(CharNode*)*countChars);
	for(i = 0; i < 128; i++){
		if(probabilityTable[i] != 0){
			HuffTree[j] = (CharNode*)malloc(sizeof(CharNode));
			HuffTree[j]->c = (char)i;
			HuffTree[j]->weight = probabilityTable[i];
			HuffTree[j]->left = HuffTree[j]->right = NULL;
			j++;
		}
	}
	
	while(count(HuffTree, countChars) > 1){

		//printf("%d\n", count(HuffTree, countChars));
		min1 = minimumNode(HuffTree, &countChars);
		min2 = minimumNode(HuffTree, &countChars);

		//printf("**** %f %f min1 and min2\n", min1->weight, min2->weight);

		//check(min1);
		//check(min2);

		resultant = (CharNode*)malloc(sizeof(CharNode));
		resultant->left = min1;
		resultant->right = min2;
		resultant->weight = min1->weight + min2->weight;
		resultant->c = '\0';
		
		add(HuffTree, resultant, &countChars);
	}
	
	return(HuffTree);
}


void recurseUpdate(char** FinalTable, CharNode *root , char *curr){
	char *templeft = (char*)malloc(sizeof(strlen(curr)) + 1);
	char *tempright = (char*)malloc(sizeof(strlen(curr)) + 1);
	strcpy(templeft, curr);
	strcpy(tempright, curr);

	if((root->left == NULL) && (root->right == NULL)){
		FinalTable[root->c] = templeft;
		return;
	}
	if(root->left != NULL){
		recurseUpdate(FinalTable, root->left, strcat(templeft, "0"));
	}

	if(root->right != NULL){
		recurseUpdate(FinalTable, root->right, strcat(tempright, "1"));
	}
}

char ** GenHuffCode(CharNode **HuffTree){
	int i = 0;
	char ** FinalTable = (char**)malloc(sizeof(char*)*128);
	for(i = 0; i<128 ; i++){
		FinalTable[i] = (char*)malloc(sizeof(char)*7);
	}

	char *temp = (char*)malloc(sizeof(char)*15);
	recurseUpdate(FinalTable, HuffTree[0], temp);

	return(FinalTable);
}




void main(int argc, char **argv){
	int filelength, i = 0;
	char *filename = argv[1];

	// The probability gtable has been procured
	float *probabilityTable = constructFreqTable(filename, &filelength);
	
	// next task is the construction of the binary tree itself , 
	// this will be a bottom up construction
	// involving combining the 2 least weighted nodes to get 
	// the parent node.
	// one advantage that we can have is that while construction, 
	// we can assign each node its binary coding
	//int i = 0;

	CharNode** HuffTree = makeHuffTree(probabilityTable);
	char ** FinalTable = GenHuffCode(HuffTree);
	FILE *fp = fopen("table.txt", "w");

	for(i = 0; i < 128; i++){
		fputs(strcat(FinalTable[i],"\n"), fp);
	}
}
