#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>

using std::cout;
using std::pair;
using std::vector;
using std::set;

int nComp = 0;
vector<set<size_t> > comparedElements;


set<size_t> makeSet(size_t elem1, size_t elem2) {
    set<size_t> s;
    s.insert(elem1);
    s.insert(elem2);
    return s;
}

void addComparedElements(size_t elem1, size_t elem2)
{
    if (comparedElements.empty()) {
        comparedElements.push_back(makeSet(elem1, elem2));
        return;
    }
    auto vecSize = comparedElements.size();
    for(int i = vecSize - 1 ;i >= 0; i--) {
        // if found in previous comparisons
        if (comparedElements[i].find(elem1) != comparedElements[i].end() or
            comparedElements[i].find(elem2) != comparedElements[i].end()) {
            if (i < vecSize - 1) {
                comparedElements[i + 1].insert(elem1);
                comparedElements[i + 1].insert(elem2);
            }
            else {
                comparedElements.push_back(makeSet(elem1, elem2));
            }
            return;
        }
    }
    comparedElements[0].insert(elem1);
    comparedElements[0].insert(elem2);
}

void compare_exchange(pair<size_t, int> &firstElem, pair<size_t, int> &secondElem, bool printComp)
{
    if (printComp) {
        cout << firstElem.first << " " << secondElem.first << std::endl;
    }
    if (firstElem.second > secondElem.second) {
        int tmp = firstElem.second;
        firstElem.second = secondElem.second;
        secondElem.second = tmp;
    }
    nComp++;
    addComparedElements(firstElem.first, secondElem.first);
}

void sortTwoArrays(pair<size_t, int> *array, size_t firstSize, size_t secondSize, bool printComp)
{
    if (firstSize == 0 or secondSize == 0) return;

    if (firstSize == 1 and secondSize == 1) {
        compare_exchange(array[0], array[1], printComp);
        return;
    }

    // make two arrays with even elements
    size_t evenFirstSize1 = round((float)firstSize / 2);
    size_t evenSecondSize1 = round((float)secondSize / 2);
    auto evenArray = new pair<size_t, int>[evenFirstSize1 + evenSecondSize1];
    for(size_t i = 0; i < evenFirstSize1; i++) {
        evenArray[i] = array[2 * i];
    }
    for(size_t i = 0; i < evenSecondSize1; i++) {
        evenArray[evenFirstSize1 + i] = array[firstSize + 2 * i];
    }
    sortTwoArrays(evenArray, evenFirstSize1, evenSecondSize1, printComp);

    // make two arrays with odd elements
    size_t oddFirstSize2 = firstSize - evenFirstSize1;
    size_t oddSecondSize2 = secondSize - evenSecondSize1;
    auto oddArray = new pair<size_t, int>[oddFirstSize2 + oddSecondSize2];
    for(size_t i = 0; i < oddFirstSize2; i++) {
        oddArray[i] = array[2 * i + 1];
    }
    for(size_t i = 0; i < oddSecondSize2; i++) {
        oddArray[oddFirstSize2 + i] = array[firstSize + 2 * i + 1];
    }
    sortTwoArrays(oddArray, oddFirstSize2, oddSecondSize2, printComp);

    // move sorted odd and even parts to array
    for(size_t i = 0; i < evenFirstSize1; i++) {
        array[2 * i] = evenArray[i];
    }
    for(size_t i = 0; i < evenSecondSize1; i++) {
        array[firstSize + 2 * i] = evenArray[evenFirstSize1 + i];
    }
    for(size_t i = 0; i < oddFirstSize2; i++) {
        array[2 * i + 1] = oddArray[i];
    }
    for(size_t i = 0; i < oddSecondSize2; i++) {
        array[firstSize + 2 * i + 1] = oddArray[oddFirstSize2 + i];
    }
    delete[] evenArray;
    delete[] oddArray;

    for(size_t i = 1; i < firstSize + secondSize - 1; i += 2) {
        compare_exchange(array[i], array[i + 1], printComp);
    }
}

void bsort(pair<size_t, int> *arr, size_t arrSize, bool printComp = true)
{
    // there recursion ends
    if (arrSize < 2) return;
    // sort each part
    bsort(arr, arrSize / 2, printComp);
    bsort(arr + arrSize / 2, arrSize - arrSize / 2, printComp);
    sortTwoArrays(arr, arrSize / 2, arrSize - arrSize / 2, printComp);
}

bool testBsort() {
    for (size_t arrSize = 1; arrSize <= 24; arrSize++) {
        auto arr = new pair<size_t, int>[arrSize];
        auto arrNumber = pow(2, arrSize);
        for (size_t i = 0; i < arrNumber; i++) {
            auto curNumber = i;
            for (size_t j = 0; j < arrSize; j++) {
                arr[j] = std::make_pair(j, curNumber % 2);
                curNumber /= 2;
            }
            bsort(arr, arrSize, false);
            for (size_t j = 0; j < arrSize - 1; j++) {
                if (arr[j + 1] < arr[j]) return false;
            }
        }
        delete[] arr;
    }
    return true;
}

int main(int argc, char **argv)
{
    size_t arrSize = atoi(argv[1]);
    cout << arrSize << " 0 0" << std::endl;
    auto arr = new pair<size_t, int>[arrSize];
    for (size_t i = 0; i < arrSize; i++)
        arr[i] = std::make_pair(i, rand() % arrSize);
    bsort(arr, arrSize);
    delete[] arr;
    cout << nComp << std::endl;
    cout << comparedElements.size() << std::endl;
    // assert(testBsort());
    return 0;
}