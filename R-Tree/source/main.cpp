#include <iostream>
#include "Shapes.hpp"
#include "R-Tree.hpp"

#define DIMENSION 2


int main(){
    // variable booleana para indicar que estoy en modo test, para hacer algunos tests
    bool TestMode = 1;


    if (TestMode) {
        cout << "SPlitTest:\n";
        std::vector<HyperRect<DIMENSION>> myRectSet;
        std::vector<Entry<double, DIMENSION>> newroot;

        double myDouble = 5;
        double* ptr = &myDouble;

        Interval AD[DIMENSION];
        AD[0] = Interval(4.0, 4.5);
        AD[1] = Interval(3.5, 5.5);

        HyperRect<DIMENSION> A(AD);
        myRectSet.push_back(A);
        Entry<double, DIMENSION> entry(A, nullptr, ptr);
        newroot.push_back(entry);


        AD[0] = Interval(5, 7);
        AD[1] = Interval(2, 4);

        A.setIntervals(AD);
        myRectSet.push_back(A);
        entry.setValues(A, nullptr, ptr);
        newroot.push_back(entry);

        AD[0] = Interval(7, 9);
        AD[1] = Interval(5, 6);

        A.setIntervals(AD);
        myRectSet.push_back(A);
        entry.setValues(A, nullptr, ptr);
        newroot.push_back(entry);

        AD[0] = Interval(6.5, 8.0);
        AD[1] = Interval(2.5, 3.5);

        A.setIntervals(AD);
        myRectSet.push_back(A);
        entry.setValues(A, nullptr, ptr);
        newroot.push_back(entry);

        AD[0] = Interval(5.5, 6.5);
        AD[1] = Interval(4.5, 5.5);

        A.setIntervals(AD);
        myRectSet.push_back(A);
        entry.setValues(A, nullptr, ptr);
        newroot.push_back(entry);

        AD[0] = Interval(7.5, 9);
        AD[1] = Interval(2, 4.5);

        A.setIntervals(AD);
        myRectSet.push_back(A);
        entry.setValues(A, nullptr, ptr);
        newroot.push_back(entry);

        HyperRect<DIMENSION> myMBR = MBR<DIMENSION>(myRectSet);

        myMBR.show_intervals();

        //std::vector<Entry<double, DIMENSION>>* _root = &newroot;
        //std::vector<Entry<double, DIMENSION>>* myL = new vector<Entry<double, DIMENSION>>;
        //std::vector<Entry<double, DIMENSION>>* myLL = new vector<Entry<double, DIMENSION>>;

        RTree<double, DIMENSION> myRTree(3, 2);
        
        for (int i = 0; i < newroot.size(); ++i) {
            myRTree.Insert(newroot[i]);
        }

        /*
        myRTree.SplitNode(_root, myL, myLL);

        // Some Tests
        cout << "Set L:\n";
        for (int i = 0; i < myL->size(); ++i) {
            (*myL)[i].Rect.show_intervals();
        }
        cout << "Set LL:\n";
        for (int i = 0; i < myLL->size(); ++i) {
            (*myLL)[i].Rect.show_intervals();
        }
        delete myL;
        delete myLL;


       
        // Search test
        cout << "Search Test see the code: " << endl;
        std::vector<Entry<double, DIMENSION>> myResults;
        AD[0] = Interval(8.5, 9.0);
        AD[1] = Interval(3.0, 4.5);
        // Search window
        HyperRect<DIMENSION> SWin(AD);

        std::cout << myRTree.Search(myResults, SWin, myRTree.root) << "\n";

        for (size_t i = 0; i < myResults.size(); ++i) {
            myResults[i].Rect.show_intervals();
        }/**/
    }

    

    return 0;
}