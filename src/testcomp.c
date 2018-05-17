#include <stdio.h>
#include <stdint.h>

int main(){

    FILE* f = fopen("example","wb");
    for (int i = 0; i < 256 ; i++){
        fputc(i,f);
    }
    fclose(f);
}