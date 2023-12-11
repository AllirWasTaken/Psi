#include "ConvolutionNeuralNetwork.h"
#include <cmath>
#include "ActivationFunctions.h"

using namespace ALib;



Matrix FlatLine(Matrix& input){
    Matrix result(input.Width()*input.Height(),1);
    for(int y=0;y<input.Height();y++){
        for(int x=0;x<input.Width();x++){
            result[0][y*input.Width()+x]=input[y][x];
        }
    }
    return result;
}


ALib::Matrix ConvolutionTask1(ALib::Matrix &input,ALib::Matrix &filtr, int stride)
{


    unsigned yMove=(input.Height()-filtr.Height())/stride+1;
    unsigned xMove=(input.Width()-filtr.Width())/stride+1;
    int segmentCount=(int)yMove*(int)xMove;
    Matrix segmentedInput(filtr.Height()*filtr.Width(),segmentCount);

    for(unsigned yt=0;yt<yMove;yt++){
        for(unsigned xt=0;xt<xMove;xt++){
            for(int y=0;y<filtr.Height();y++){
                for(int x=0;x<filtr.Width();x++){
                    segmentedInput[yt*xMove+xt][y*filtr.Width()+x]=input[yt*stride+y][xt*stride+x];
                }
            }
        }
    }

    Matrix flatFiltr= FlatLine(filtr);

    Matrix output;

    output=segmentedInput*flatFiltr.Transpose();
    int size=3;
    Matrix result(3,3);

    for(int y=0;y<size;y++){
        for(int x=0;x<size;x++){
            result[y][x]=output[y*size+x][0];
        }
    }

    return result;
}

int Test2( ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid){
    int result=0;

    Matrix answers;
    answers=ConvolutionRun2(input,convolution,neuralGrid);



    for(int i=0;i<expected.Width();i++){
        unsigned correct=11;
        for(int j=0;j<expected.Height();j++){
            if(expected[j][i]==1){
                correct=j;
                break;
            }
        }
        float max=answers[0][i];
        unsigned maxI=0;
        for(int j=0;j<expected.Height();j++){
            if(answers[j][i]>max){
                maxI=j;
                max=answers[j][i];
            }
        }
        if(maxI==correct)result++;
    };



    return result;
}


ALib::Matrix ConvolutionRun2(const ALib::Matrix &input,const ALib::Matrix& convolution,ALib::Matrix& neuralGrid){

    int imageSize=input.Height();
    int imageY= (int)sqrt(imageSize);
    int imageX=imageSize/imageY;

    int filterX=3;
    int filterY=3;

    int imageMoveX=imageX-filterX+1;
    int imageMoveY=imageY-filterY+1;


    Matrix result(input.Width(),neuralGrid.Height());


    for(int serial=0;serial<input.Width();serial++){
        Matrix imageSegmented(convolution.Width(),imageMoveX*imageMoveY);
        for(unsigned yt=0;yt<imageMoveY;yt++){
            for(unsigned xt=0;xt<imageMoveX;xt++){
                for(int y=0;y<filterY;y++){
                    for(int x=0;x<filterX;x++){
                        imageSegmented[yt*imageMoveX+xt][y*filterX+x]=input[(y+yt)*imageX+x+xt][serial];
                    }
                }
            }
        }
        Matrix convOut=imageSegmented*convolution.Transpose();
        ActivationFunctions::RectifiedLinearUnit(convOut);
        Matrix flatConv= FlatLine(convOut);
        flatConv=flatConv.Transpose();
        Matrix neuralOut=neuralGrid*flatConv;
        for(int i=0;i<neuralGrid.Height();i++){
            result[i][serial]=neuralOut[i][0];
        }
    }

    return result;

}

ALib::Matrix ConvolutionRun3(const ALib::Matrix &input,const ALib::Matrix& convolution, ALib::Matrix& neuralGrid){

    int imageSize=input.Height();
    int imageY= (int)sqrt(imageSize);
    int imageX=imageSize/imageY;

    int filterX=3;
    int filterY=3;

    int imageMoveX=imageX-filterX+1;
    int imageMoveY=imageY-filterY+1;


    Matrix result(input.Width(),neuralGrid.Height());


    for(int serial=0;serial<input.Width();serial++){
        Matrix imageSegmented(convolution.Width(),imageMoveX*imageMoveY);
        for(unsigned yt=0;yt<imageMoveY;yt++){
            for(unsigned xt=0;xt<imageMoveX;xt++){
                for(int y=0;y<filterY;y++){
                    for(int x=0;x<filterX;x++){
                        imageSegmented[yt*imageMoveX+xt][y*filterX+x]=input[(y+yt)*imageX+x+xt][serial];
                    }
                }
            }
        }
        Matrix convOut=imageSegmented*convolution.Transpose();
        ActivationFunctions::RectifiedLinearUnit(convOut);

        Matrix convPooled(convOut.Width()/2,convOut.Height());
        for(int y=0;y<convOut.Height();y+=2){
            for(int x=0;x<convOut.Width();x+=2){
                float max=convOut[0][0];
                for (int poolY = 0; poolY < 2; ++poolY) {
                    for (int poolX = 0; poolX < 2; ++poolX) {

                    }
                }
            }
        }

        Matrix flatConv= FlatLine(convPooled);
        flatConv=flatConv.Transpose();
        Matrix neuralOut=neuralGrid*flatConv;
        for(int i=0;i<neuralGrid.Height();i++){
            result[i][serial]=neuralOut[i][0];
        }
    }

    return result;


}
void ConvolutionUpdate2(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid, float alpha){



    int imageSize=input.Height();
    int imageY= (int)sqrt(imageSize);
    int imageX=imageSize/imageY;

    int filterX=3;
    int filterY=3;

    int imageMoveX=imageX-filterX+1;
    int imageMoveY=imageY-filterY+1;





    for(int serial=0;serial<input.Width();serial++){
        Matrix imageSegmented(convolution.Width(),imageMoveX*imageMoveY);
        for(unsigned yt=0;yt<imageMoveY;yt++){
            for(unsigned xt=0;xt<imageMoveX;xt++){
                for(int y=0;y<filterY;y++){
                    for(int x=0;x<filterX;x++){
                        imageSegmented[yt*imageMoveX+xt][y*filterX+x]=input[(y+yt)*imageX+x+xt][serial];
                    }
                }
            }
        }
        Matrix currentExpected(1,expected.Height());
        for(int i=0;i<expected.Height();i++){
            currentExpected[i][0]=expected[i][serial];
        }





        Matrix convOut=imageSegmented*convolution.Transpose();
        ActivationFunctions::RectifiedLinearUnit(convOut);
        Matrix flatConv= FlatLine(convOut);
        flatConv=flatConv.Transpose();
        Matrix neuralOut=neuralGrid*flatConv;

        Matrix neuralDelta=(neuralOut-currentExpected)*(float)2/(float)neuralGrid.Height();
        Matrix convDelta=neuralGrid.Transpose()*neuralDelta;
        Matrix reshapedConvDelta(convOut.Width(),convOut.Height());
        for(int y=0;y<convOut.Height();y++){
            for(int x=0;x<convOut.Width();x++){
                reshapedConvDelta[y][x]=convDelta[y*convOut.Width()+x][0];
            }
        }
        Matrix reluDet=convOut;
        ActivationFunctions::RectifiedLinearUnitDer(reluDet);
        reshapedConvDelta=reshapedConvDelta.MultiplyIndexByIndex(reluDet);

        Matrix outputWeight=neuralDelta*flatConv.Transpose();
        Matrix convWeight=reshapedConvDelta.Transpose()*imageSegmented;

        neuralGrid=neuralGrid-outputWeight*alpha;
        convolution=convolution-convWeight*alpha;


    }

}
int Test3( ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid){
    int result=0;

    Matrix answers;
    answers=ConvolutionRun3(input,convolution,neuralGrid);



    for(int i=0;i<expected.Width();i++){
        unsigned correct=11;
        for(int j=0;j<expected.Height();j++){
            if(expected[j][i]==1){
                correct=j;
                break;
            }
        }
        float max=answers[0][i];
        unsigned maxI=0;
        for(int j=0;j<expected.Height();j++){
            if(answers[j][i]>max){
                maxI=j;
                max=answers[j][i];
            }
        }
        if(maxI==correct)result++;
    };



    return result;
}
void ConvolutionUpdate3(ALib::Matrix &input, ALib::Matrix &expected, ALib::Matrix& convolution, ALib::Matrix& neuralGrid, float alpha){



    int imageSize=input.Height();
    int imageY= (int)sqrt(imageSize);
    int imageX=imageSize/imageY;

    int filterX=3;
    int filterY=3;

    int imageMoveX=imageX-filterX+1;
    int imageMoveY=imageY-filterY+1;





    for(int serial=0;serial<input.Width();serial++){
        Matrix imageSegmented(convolution.Width(),imageMoveX*imageMoveY);
        for(unsigned yt=0;yt<imageMoveY;yt++){
            for(unsigned xt=0;xt<imageMoveX;xt++){
                for(int y=0;y<filterY;y++){
                    for(int x=0;x<filterX;x++){
                        imageSegmented[yt*imageMoveX+xt][y*filterX+x]=input[(y+yt)*imageX+x+xt][serial];
                    }
                }
            }
        }
        Matrix currentExpected(1,expected.Height());
        for(int i=0;i<expected.Height();i++){
            currentExpected[i][0]=expected[i][serial];
        }





        Matrix convOut=imageSegmented*convolution.Transpose();
        ActivationFunctions::RectifiedLinearUnit(convOut);
        Matrix flatConv= FlatLine(convOut);
        flatConv=flatConv.Transpose();
        Matrix neuralOut=neuralGrid*flatConv;

        Matrix neuralDelta=(neuralOut-currentExpected)*(float)2/(float)neuralGrid.Height();
        Matrix convDelta=neuralGrid.Transpose()*neuralDelta;
        Matrix reshapedConvDelta(convOut.Width(),convOut.Height());
        for(int y=0;y<convOut.Height();y++){
            for(int x=0;x<convOut.Width();x++){
                reshapedConvDelta[y][x]=convDelta[y*convOut.Width()+x][0];
            }
        }
        Matrix reluDet=convOut;
        ActivationFunctions::RectifiedLinearUnitDer(reluDet);
        reshapedConvDelta=reshapedConvDelta.MultiplyIndexByIndex(reluDet);

        Matrix outputWeight=neuralDelta*flatConv.Transpose();
        Matrix convWeight=reshapedConvDelta.Transpose()*imageSegmented;

        neuralGrid=neuralGrid-outputWeight*alpha;
        convolution=convolution-convWeight*alpha;


    }

}