#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <iomanip>
#include <vector>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {
private:
    Node<T> **x,**y;
    unsigned rows, columns;
    bool findx(unsigned x1, Node<T> **(&pointer)) const{
        while(*pointer){
            if((*pointer)->x<=x1){
                if (x1 == (*pointer)->x) return true;
            } else {return false;}
            pointer = &((*pointer)->down);      
        }
        return false;
    }
    bool findy(unsigned y1, Node<T> **(&pointer)) const{
        while(*pointer){
            if((*pointer)->y<=y1){
                if (y1 == (*pointer)->y) return true;
            } else { return false;}
            pointer = &((*pointer)->next);
        }
        return false;
    }
    void deleteAll(Node<T>* node){
        if(node->next) deleteAll(node->next);
        delete node;
    }

public:
    Matrix(unsigned rows, unsigned columns);
    Matrix(const Matrix<T>& other);

    void set(unsigned, unsigned, T);
    T operator()(unsigned, unsigned) const;
    Matrix<T> operator*(T scalar) const;
    Matrix<T> operator*(const Matrix<T> &other) const;
    Matrix<T> operator+(const Matrix<T> &other) const;
    Matrix<T> operator-(const Matrix<T> &other) const;
    Matrix<T>& operator=(const Matrix<T> &other);
    Matrix<T> transpose() const;
    void print();

    ~Matrix();
};


template <typename T>
Matrix<T>::Matrix(unsigned rows, unsigned columns):rows(rows),columns(columns){
    x = new Node<T>*[rows];
    y = new Node<T>*[columns];
}


template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other):rows(other.rows),columns(other.columns){
    x = new Node<T>*[other.rows];
    y = new Node<T>*[other.columns];
    for(int i=0;i<other.rows;++i){
        for(int j=0;j<other.columns;++j){
            this->set(i,j,other(i,j));
        }
    }
}


template <typename T>
void Matrix<T>::set(unsigned r, unsigned c, T data){
    if(r<rows && c<columns){
        auto px = &(x[r]);
        auto py = &(y[c]);
        findx(r,py);
        if(findy(c,px)){
            if(data) {(*px)->data=data;return;}
            auto tempx = (*px)->next;
            (*py) = (*py)->down;
            delete (*px); 
            (*px) = tempx;
            return;
        }
        if(data){
            Node<T>* newNode = new Node<T>(r,c,data);
            newNode->next = (*px); (*px) = newNode;
            newNode->down = (*py); (*py) = newNode;
        }
    }
}


template <typename T>
T Matrix<T>::operator()(unsigned r, unsigned c) const{
    if(r<rows && c<columns){
        auto temp = &(x[r]);
        if(findy(c,temp)) return (*temp)->data;
        return 0;
    }

    // Falta un return para estos casos, lo ideal sería un throw
}


template <typename T>
Matrix<T> Matrix<T>::operator*(T scalar) const{
    Matrix<T> c(rows,columns);
    if(scalar)
        for(int i=0;i<rows;++i){
            auto px = x[i];
            while(px){
                c.set(px->x,px->y,px->data*scalar);
                px = px->next;
            }
        }
    return c;
}


template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const{
    Matrix<T> c(rows,other.columns);
    if(columns==other.rows){
        auto y_2 = other.y;
        int i,j,k,sum;
        for(i=0;i<rows;++i){
            for(j=0;j<other.columns;++j){
                auto px = x[i];
                auto py_2 = &(y_2[j]);
                sum = 0;
                while(px){
                    if(other.findx(px->y,py_2))
                        sum += px->data*(*py_2)->data;
                    px = px->next;
                }
                c.set(i,j,sum);
            }
        }
    }
    return c;
}


template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const{
    Matrix<T> c(rows,columns);
    if(rows==other.rows && columns==other.columns){
        for(int i=0;i<rows;++i){
            auto px = x[i], px2 = other.x[i];
            while(px && px2){
                if(px->y == px2->y){
                    auto sum = px->data+px2->data;
                    c.set(i,px->y,sum);
                    px = px->next;px2 = px2->next;
                } else if(px->y < px2->y) {
                    c.set(i,px->y,px->data);
                    px = px->next;
                } else {
                    c.set(i,px2->y,px2->data);
                    px2 = px2->next;
                }
            }
            while(px){c.set(i,px->y,px->data);px = px->next;}
            while(px2){c.set(i,px2->y,px2->data);px2 = px2->next;}
        }
    }
    return c;
}


template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const{
    Matrix<T> c(rows,columns);
    if(rows==other.rows && columns==other.columns){
        for(int i=0;i<rows;++i){
            auto px = x[i], py = other.x[i];
            while(px && py){
                if(px->y == py->y){
                    auto sum = px->data-py->data;
                    if(sum) c.set(px->x,px->y,sum);
                    px = px->next;py = py->next;
                } else if(px->y < py->y) {
                    c.set(px->x,px->y,px->data);
                    px = px->next;
                } else {
                    c.set(py->x,py->y,-1*py->data);
                    py = py->next;
                }
            }
            while(px){c.set(px->x,px->y,px->data);px = px->next;}
            while(py){c.set(py->x,py->y,-1*py->data);py = py->next;}
        }
    }
    return c;
}


template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &other){
    for(int i=0;i<other.rows;++i){
        for(int j=0;j<other.columns;++j){
            this->set(i,j,other(i,j));
        }
    }
    return *this;
}


template <typename T>
Matrix<T> Matrix<T>::transpose() const{
    Matrix<T> c(columns,rows);
    for(int i=0;i<rows;++i){
        auto px = x[i];
        while(px){
            c.set(px->y,px->x,px->data);
            px = px->next;
        }
    }
    return c;
}


template <typename T>
void Matrix<T>::print(){
    for(int i=0;i<rows;++i){
        Node<T>* px = x[i];
        for(int j=0; j<columns; ++j){
            if(px && px->y==j){
                cout << setw(10) << px->data;
                px = px->next;
            } else cout << setw(10) << 0;
        }
        cout << '\n';
    }
    cout << '\n';
}


template <typename T>
Matrix<T>::~Matrix(){
    for(int i=0;i<rows;++i){
        deleteAll(x[i]);
    }
}

#endif //SPARSE_MATRIX_MATRIX_H