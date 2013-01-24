#include <iostream>

using namespace std;

// this class encapsulates the "+" operation.
struct pls {
    static double apply( double a, double b) {
        return a+b;
    }
};


// the node in the parse tree.
template <typename Left, typename Op, typename Right>
struct X {
    Left    left;
    Right   right;

    X( Left t1, Right t2) : left(t1), right(t2)  { }

    double operator[](int i) {
        return Op::apply( left[i], right[i] );
    }
};


struct Array {
    // constructor
    Array( double *data, int N) : data_(data), N_(N) { }

    // assign an expression to the array
    template <typename Left, typename Op, typename Right>
    void operator=( X<Left,Op,Right> expr) {
        for ( int i = 0; i < N_; ++i)
            data_[i] = expr[i];
    }

    double operator[](int i) {
        return data_[i];
    }

    double *data_;
    int N_;
};


template <typename Left>
X<Left, pls, Array> operator+( Left a, Array b) {
    return X<Left, pls, Array>(a,b);
}


int main() {
    double a_data[] = { 2, 3, 5, 9 };
    double b_data[] = { 1, 0, 0, 1 };
    double c_data[] = { 3, 0, 2, 5 };
    double d_data[4];

    Array A(a_data,4);
    Array B(b_data,4);
    Array C(c_data,4);
    Array D(d_data,4);

    D = A + B + C;

    for ( int i = 0; i < 4; ++i )
        cout << D[i] << " ";
    cout << endl;
}

