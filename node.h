#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
	unsigned x,y;
	T data;

public:
    explicit Node(unsigned x, unsigned y, T data):x(x),y(y),data(data),next(0),down(0){};
    void killSelf() {
        if(next) next->killSelf();
        delete this;
    }
    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
