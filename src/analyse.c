#include "../include/analyse.h"
void init_tab_freq( uint64_t freq[256]){
	for (int i = 0; i < 256; i++)
	{
		freq[i]=0;
	}
}

void ecriture_tableau_freq( uint64_t freq[256]){

	for (int i = 0; i < 256; i++)
	{
		printf("%c :",i );
		printf( PRIu64 "\n", freq[i]);
	}
}


void frequences(char* filename, uint64_t freq[256]){

	FILE *f;
	char c;
	f=fopen( filename, "r");
	if(f == NULL){
		#ifdef DEBUG
			printf("Error opening file \n");
		#endif
		return;
	}

	init_tab_freq(freq);

	while( !feof(f)){
		fscanf(f,"%c", &c);
		freq[(int)c]++;
	}
	ecriture_tableau_freq(freq);
	fclose(f);
}


