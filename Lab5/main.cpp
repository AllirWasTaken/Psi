#include <iostream>
#include <cstdint>
#include "Matrix.h"

using namespace ALib;

void LoadDataBase(const char *labelName,const char *imagesName,Matrix &input,Matrix &expected){

    FILE *f = fopen(labelName,"rb");

    unsigned labelCount,temp;
    fread(&labelCount,4,1,f);
    fread(&labelCount,4,1,f);
    {
        union test {
            unsigned char c[4];
            unsigned number;
        } testUnion;
        testUnion.number = labelCount;
        unsigned char help[4];
        help[0]=testUnion.c[3];
        help[1]=testUnion.c[2];
        help[2]=testUnion.c[1];
        help[3]=testUnion.c[0];

        testUnion.c[0]=help[0];
        testUnion.c[1]=help[1];
        testUnion.c[2]=help[2];
        testUnion.c[3]=help[3];

        labelCount=testUnion.number;
    }

    //labelCount=1000;

    Matrix newExpected(labelCount,10);
    newExpected.SetZero();

    uint8_t smol;


    for(unsigned i=0;i<labelCount;i++){
        fread(&smol,1,1,f);
        newExpected[smol][i]=1;
    }

    fclose(f);
    expected=newExpected;
    f= fopen(imagesName,"rb");

    fread(&temp,4,1,f);
    fread(&temp,4,1,f);

    unsigned imageSizeX,imageSizeY;

    fread(&imageSizeY,4,1,f);
    fread(&imageSizeX,4,1,f);
    imageSizeY=28;
    imageSizeX=28;

    uint8_t tempSmal;
    float tempFloat;
    unsigned imageVectorSize=imageSizeX*imageSizeY;

    Matrix newInput(labelCount,imageVectorSize);


    for(unsigned i=0;i<labelCount;i++){
        for(int y=0;y<imageVectorSize;y++){
            fread(&tempSmal,1,1,f);
            tempFloat=tempSmal;
            tempFloat/=255.0f;
            newInput[y][i]=tempFloat;
        }
    }

    fclose(f);
    input=newInput;
}


void Zad1(){

}


void Zad2(){

}


void Zad3(){

}


int main() {
    //Zad1();
    //Zad2();
    //Zad3();
}
