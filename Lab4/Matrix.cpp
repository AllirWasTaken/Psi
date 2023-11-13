#include "Matrix.h"

#include <stdexcept>
#include <time.h>


using namespace ALib;



Matrix::Matrix(const std::vector<std::vector<float>> &newMatrix) {
    unsigned a=newMatrix[0].size();
    for(unsigned i=0;i<newMatrix.size();i++){
        if(a!=newMatrix[i].size()){
            throw std::invalid_argument("Matrix rows are not the same size");
        }
    }
    width=newMatrix[0].size();
    height=newMatrix.size();
    matrixData=newMatrix;
}
Matrix::Matrix() = default;

Matrix &Matrix::operator=(const ALib::Matrix &b) {
    matrixData=b.matrixData;
    width=b.width;
    height=b.height;
    return *this;
}

Matrix &Matrix::operator=(const std::vector<std::vector<float>> &b) {
    unsigned a=b[0].size();
    for(unsigned i=0;i<b.size();i++){
        if(a!=b[i].size()){
            throw std::invalid_argument("Matrix rows are not the same size");
        }
    }
    width=b[0].size();
    height=b.size();
    matrixData=b;
    return *this;
}

Matrix Matrix::operator*(const float &b) {
    Matrix result;
    result.SetSize(width,height);
    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            result[y][x]=this->matrixData[y][x]*b;
        }
    }
    return result;
}

void Matrix::SetSize(unsigned int x, unsigned int y) {
    if(!x||!y)return;
    matrixData.resize(y);
    for(unsigned i=0;i<y;i++){
        matrixData[i].resize(x);
    }
    width=x;
    height=y;
}

Matrix &Matrix::operator/=(const float &b) {
    for(unsigned y=0;y<matrixData.size();y++){
        for(unsigned x=0;x<matrixData[y].size();x++){
            matrixData[y][x]/=b;
        }
    }
    return *this;
}

Matrix Matrix::operator/(const float &b) {
    Matrix result;
    result.SetSize(width,height);
    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            result[y][x]=this->matrixData[y][x]/b;
        }
    }
    return result;
}

std::vector<float> &Matrix::operator[](unsigned int index){
    return matrixData[index];
}

const std::vector<float> &Matrix::operator[](unsigned int index) const {
    return matrixData[index];
}

Matrix &Matrix::operator*=(const float &b) {
    for(int y=0;y<matrixData.size();y++){
        for(int x=0;x<matrixData[y].size();x++){
            matrixData[y][x]*=b;
        }
    }
    return *this;
}

Matrix Matrix::operator*(const ALib::Matrix &b) {
    if(width!=b.height){
        throw std::invalid_argument("Width of first matrix is not matching height for second matrix");
    }
    Matrix result;
    result.SetSize(b.width,height);


    //03 in compiler optimizes this better than any attempt to transpose or SIMD, only improvement could be seen in alignment and redoing vector lib
    //which would take too much time for purpose of this library


    for(int y=0;y<result.height;y++){
        for(int x=0;x<result.width;x++){
            result[y][x]=0;
            for(int z=0;z<width;z++){
                result[y][x]+=matrixData[y][z]*b.matrixData[z][x];
            }
        }
    }




    return result;
}



Matrix Matrix::operator+(const ALib::Matrix &b) {
    if(width!=b.width||height!=b.height){
        throw std::invalid_argument("Size of first matrix is not matching size of second matrix");
    }
    Matrix result;
    result.SetSize(width,height);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            result[y][x]=matrixData[y][x]+b.matrixData[y][x];
        }
    }

    return result;
}

Matrix& Matrix::operator+=(const ALib::Matrix &b) {
    if(width!=b.width||height!=b.height){
        throw std::invalid_argument("Size of first matrix is not matching size of second matrix");
    }
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            matrixData[y][x]+=b.matrixData[y][x];
        }
    }

    return *this;
}

Matrix& Matrix::operator-=(const ALib::Matrix &b) {
    if(width!=b.width||height!=b.height){
        throw std::invalid_argument("Size of first matrix is not matching size of second matrix");
    }
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            matrixData[y][x]-=b.matrixData[y][x];
        }
    }

    return *this;
}

Matrix Matrix::operator-(const ALib::Matrix &b) {
    if(width!=b.width||height!=b.height){
        throw std::invalid_argument("Size of first matrix is not matching size of second matrix");
    }
    Matrix result;
    result.SetSize(width,height);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            result[y][x]=matrixData[y][x]-b.matrixData[y][x];
        }
    }

    return result;
}

Matrix Matrix::Transpose() const{
    Matrix result;
    result.SetSize(height,width);
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            result[x][y]=matrixData[y][x];
        }
    }
    return  result;
}

Matrix Matrix::operator->*(const ALib::Matrix &b) {
    if(width!=1||b.width!=1){
        throw std::invalid_argument("Both matrix have to be column (1 width)");
    }

    Matrix result;
    result=(*this)*b.Transpose();

    return result;
}

unsigned int Matrix::Height() const {
    return height;
}

unsigned int Matrix::Width() const{
    return width;
}

std::ostream& ALib::operator<<(std::ostream& stream, const Matrix& matrix){
    for(unsigned y=0;y<matrix.Height();y++){
        for(unsigned x=0;x<matrix.Width();x++){
            stream<<matrix[y][x]<<" ";
        }
        stream<<std::endl;
    }
    return stream;
}

Matrix::Matrix(unsigned int x, unsigned int y) {
    SetSize(x,y);
}

Matrix Matrix::MultiplyIndexByIndex(const ALib::Matrix &b) {
    if(height!=b.height||width!=b.width){
        throw std::invalid_argument("Size of first matrix is not matching size of second matrix");
    }
    Matrix result(width,height);

    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            result[y][x]=matrixData[y][x]*b.matrixData[y][x];
        }
    }
    return result;
}

void Matrix::SetZero() {
    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            matrixData[y][x]=0;
        }
    }
}

void Matrix::Randomize(float min, float max) {
    srand(time(nullptr));

    for(unsigned y=0;y<height;y++){
        for(unsigned x=0;x<width;x++){
            matrixData[y][x]=(((float)rand()/(float)RAND_MAX)*(max-min))+min;
        }
    }

}