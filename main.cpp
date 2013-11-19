/**
* Fourier-Motzkin Elimination
* @author Jan Zaloudek
*/

#include <iostream>
#include <vector>
#include <exception>
#include <cmath>

using namespace std;

class AbstractException {
    public:
        AbstractException(string message, int code, string exception_name = "") {
            this->message = message;
            this->code = code;
            this->exception_name = exception_name;
        }

        void Print(ostream & out) {
            if(this->exception_name != "") {
                out << exception_name << ": ";
            }

            out << "[" << this->code << "]" << " " << this->message << endl;
        }

        void Print() {
            this->Print(cout);
        }
    private:
        string message, exception_name;
        int code;
};

class MathException : public AbstractException {
    public:
        MathException(string message) : AbstractException(message, 101, "MathException") {

        }
};

// madafaka vektor
class CVector {
    public:

    CVector(int size) {
        this->values = new double[size];
        this->size = size;

        for(int i = 0; i < size; i++) {
            values[i] = 0;
        }
    }

    void Print() {
        cout.width(5);
        for(int i = 0; i < this->GetSize(); i++) {
            cout << values[i] << " ";
        }

        cout << endl;
    }

    double GetMember(int i) {
        //cout << "shit: " << this->values[i];
        return this->values[i];
    }

    void SetMember(int i, double value) {
        this->values[i] = value;
    }

    int GetSize() {
        return this->size;
    }

    private:
    double *values;
    int size;
};

// motofoko metrix klas
class CMatrix {
    public:

    CMatrix(int width, int height) {
        this->Initialize(width, height);
    }

    CVector * GetRow(int i) {
        return vectors[i];
    }

    void InsertRow(CVector * row) {
        if(usedHeight == height) {
            throw new MathException("Uz je tu plno");
        }

        this->vectors[usedHeight++] = row;
    }

    void UpdateCell(double value, int x, int y) {
        this->vectors[y]->SetMember(x, value);
    }

    double GetCell(int x, int y) const {
        return this->vectors[y]->GetMember(x);
    }

    void Print(bool raw = false) const {
        if(!raw) {
            cout << "CMatrix Width: " << width << " x Height: " << height << endl;
        }

        for(int i = 0; i < usedHeight; i++) {
            if(!this->vectors[i]) {
                cout << "No vector on this line" << endl;

                continue;
            }

            this->vectors[i]->Print();
            cout << endl;
        }
    }

    void EliminateDups() {
        cout << "TODO: Eliminate duplicates" << endl;
    }

    // getters, setters
    int GetWidth() const {
        return this->width;
    }

    int GetHeight() const {
        return this->usedHeight;
    }

    int GetAllocatedHeight() const {
        return this->height;
    }

    CVector * VectorFactory() {
        return new CVector(this->width);
    }

    private:
    CVector ** vectors;
    int width, height, usedHeight;

    void Initialize(int width, int height) {
        this->height = height;
        this->width = width;
        this->usedHeight = 0;

        vectors = new CVector *[height];

        for(int i = 0; i < height; i++) {
            //vectors[i] = this->VectorFactory();
            vectors[i] = NULL;
        }
    }
};

class CFourierMotzkinElimination {
    public:
        CFourierMotzkinElimination(CMatrix *A) {
            this->matrix = A;
        }

        /* CMatrix * Eliminate() {
            cout << "Elimination..." << endl << endl;

            CMatrix *result;

            CMatrix *r = this->matrix;

            for(int i = 0; i < this->matrix->GetWidth()-1; i++) {
                r->Print(true);
                cout << "----- " << endl;

                vector<CVector*> w0, wPlus, wMinus;

                for(int j = 0; j < r->GetHeight(); j++) {
                    double x = r->GetCell(i, j);
                    CVector *row = r->GetRow(j);

                    cout << "Eliminating by " << i << " column (value " << x << ") - ";
                    row->Print();

                    if(x < 0) {
                        wMinus.push_back(r->GetRow(j));
                    } else
                    if(x > 0) {
                        wPlus.push_back(r->GetRow(j));
                    } else { // x == 0
                        w0.push_back(r->GetRow(j));
                    }
                }

                CMatrix *mat = new CMatrix(this->matrix->GetWidth(), w0.size()+(wMinus.size()*wPlus.size()));
                for(auto c = w0.begin(); c != w0.end(); c++) {
                    mat->InsertRow((*c));
                }

                r = mat;

//                cout << "|w0| : " << w0.size() << ", |w+| : " << wPlus.size() << ", |w-| : " << wMinus.size() << endl;

                for(auto j = wPlus.begin(); j != wPlus.end(); j++) {
                    CVector *a = (*j);

                    for(auto k = wMinus.begin(); k != wMinus.end(); k++) {
                        CVector *b = (*k);

                        CVector *c = this->matrix->VectorFactory();

                        cout << "Creating new vector ";

                        for(int u = 0; u < c->GetSize(); u++) {
                            c->SetMember(u, a->GetMember(u) + b->GetMember(u));
                        }

                        c->Print();

                        r->InsertRow(c);
                    }
                }

                r->EliminateDups();
            }

            return r;
        } */

        CMatrix * Eliminate2() {
            CMatrix *r = this->matrix;
            for(int i = this->matrix->GetWidth()-2; i >= 0; --i) {
                r->Print(true);
                cout << "-----" << endl;

                Reduce(r, i);

                vector<CVector*> w0, wPlus, wMinus;

                for(int j = 0; j < r->GetHeight(); j++) {
                    double x = r->GetCell(i, j);
                    CVector *row = r->GetRow(j);

                    cout << "Eliminating by " << i << " column (value " << x << ") - ";
                    row->Print();

                    if(x < 0) {
                        wMinus.push_back(r->GetRow(j));
                    } else
                    if(x > 0) {
                        wPlus.push_back(r->GetRow(j));
                    } else { // x == 0
                        w0.push_back(r->GetRow(j));
                    }
                }

                if(wPlus.size() == 0 || wMinus.size() == 0) {
                    cout << "Skipping, no elimination" << endl;
                    continue;
                }


                CMatrix *mat = new CMatrix(this->matrix->GetWidth(), w0.size()+(wMinus.size()*wPlus.size()));
                for(auto c = w0.begin(); c != w0.end(); c++) {
                    mat->InsertRow((*c));
                }

                r = mat;

//                cout << "|w0| : " << w0.size() << ", |w+| : " << wPlus.size() << ", |w-| : " << wMinus.size() << endl;

                for(auto j = wPlus.begin(); j != wPlus.end(); j++) {
                    CVector *a = (*j);

                    for(auto k = wMinus.begin(); k != wMinus.end(); k++) {
                        CVector *b = (*k);

                        CVector *c = this->matrix->VectorFactory();

                        cout << "Creating new vector ";

                        bool valid = false;
                        for(int u = 0; u < c->GetSize(); u++) {
                            double member = a->GetMember(u) + b->GetMember(u);

                            if(member != 0) {
                                valid = true;
                            }

                            c->SetMember(u, member);
                        }

                        c->Print();

                        if(valid)
                            r->InsertRow(c);
                    }
                }

                r->EliminateDups();
            }


            return r;
        }


    // should be protected one time
    static void Reduce(CMatrix *matrix, int reduced_index) {
        for(int y = 0; y < matrix->GetHeight(); y++) {
            CVector *row = matrix->GetRow(y);
            double reduced_value = fabs(row->GetMember(reduced_index));

            if(!reduced_value) {
                continue;
            }

            for(int x = 0; x < row->GetSize(); x++) {
                row->SetMember(x, row->GetMember(x) / reduced_value);
            }
        }
    }

    void Elimination() {

    }

    protected:
        CMatrix *matrix;


};


int main()
{
    CMatrix* matrix;

    // loading matrix

    int width, height;
    cout << "Width: ";
    cin >> width;

    cout << "Height: ";
    cin >> height;

    matrix = new CMatrix(width, height);

    for(int i = 0; i < height; i++) {
        CVector *ineq = matrix->VectorFactory();

        for(int j = 0; j < width; j++) {
            double value;

            cout << i << " x " << j << ": ";
            cin >> value;

            ineq->SetMember(j, value);
        }

        matrix->InsertRow(ineq);
    }

    matrix->Print();

    CFourierMotzkinElimination *elimination = new CFourierMotzkinElimination(matrix);

    CMatrix *result = elimination->Eliminate2();

    cout << "Result" << endl;

    result->Print();

    return 0;
}
