#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// No idea why the strcmp() dunction aborted, I guess because of testing two
// NULL strings ? , anyways the below function got the job done so just fine.
int scmp(char *st1, char *st2){
	int len1 = strlen(st1);
	int len2 = strlen(st2);
	int i;
	if((len1) != (len2)) return (-1);
	if(len1 == 0) return (-1);
	
	for(i = 0; i<len2; i++){
		if((int)st1[i] != (int)st2[i]) return(-1);
	}
   
	return(1);
}

int main(int argc, char **argv){

	char *filenamehc = argv[1];
	char *filenamebd = argv[2];
	char *curstr = (char*)malloc(sizeof(char)*25);
	char cht[1] = "";
	char curchar;
	char **HuffCodes = (char**)malloc(sizeof(char*)*129);
	long long int i = 0, j = 0;
	int k = 0;
	FILE *hcode = fopen(filenamehc, "r");
	FILE *binData = fopen(filenamebd, "r");
	FILE *output = fopen("outputText.txt", "w");
	
	while(!feof(hcode)){
		HuffCodes[i] = (char*)malloc(sizeof(char)*15);
		fgets(HuffCodes[i], 14, hcode);
		HuffCodes[i][strlen(HuffCodes[i]) -1] = '\0';
		i++;
	}

	fclose(hcode);
	
	
	while(!feof(binData)){
		curchar = (char)fgetc(binData);
		cht[0] = curchar;
		curstr = strcat(curstr, cht);
		for(k = 0; k < 128; k++){
			if(scmp(curstr, HuffCodes[k]) == 1){
				fputc(k, output);
				j = 0;
				strcpy(curstr, "");
			}
		}
	}
	fclose(output);
	fclose(binData);
	return(0);
}
	
