#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char **argv){
	char *filename = argv[1];
	char **HuffCodes = (char**)malloc(sizeof(char*)*128);
	long long int i = 0, k = 0;
	FILE *fp = fopen(filename, "r");

	while(!feof(fp)){
		HuffCodes[i] = (char*)malloc(sizeof(char)*15);
		fgets(HuffCodes[i], 20, fp);
		//printf("%d %s", i, HuffCodes[i]);
		i++;
	}

	//close(fp);
	
	FILE *fp_file = fopen(argv[2], "r");
	FILE *fp_op = fopen("encodedData.txt", "w");

	while(!feof(fp_file)){
		i = (long long int)fgetc(fp_file);
		if(i != -1){
			for(k = 0; k < strlen(HuffCodes[i])-1; k++){
				fputc(HuffCodes[i][k], fp_op);
			}
		}
	}
	
	//fclose(fp_file);
	//fclose(fp_op);
}