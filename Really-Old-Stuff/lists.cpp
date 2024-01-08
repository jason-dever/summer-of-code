#include <iostream>

using namespace std;

template <typename T> 
struct List {
    // Member variables; values is a dynamic array that will store the values of the list;
    // max_size is equal to the size of values, and current_size is equal to the number of values populating the values array. 
    T* values {};
    int max_size;
    int current_size;

    List() {
        values = new T[4];
        max_size = 4;
        current_size = 0;
    }

    ~ List() {
        delete values;
        values = nullptr;
    }

    // Appends a value to the end of the list
    void append(const T& value) {
        current_size++;

        if (current_size <= max_size) { // Can array fit the new value?
            values[current_size-1] = value;
            return;
        }
        doubleSize();

        values[current_size-1] = value;
        return;
    }

    // Removes a value from the list at a given index
    void pop(const int& index) { 
        if (index >= current_size) { // Passed index out of range?
            cerr << "IndexError: pop index out of range" << endl;
            return;
        }

        for (int x {index}; x < current_size; x++) {
            values[x] = values[x+1];
        }
        current_size -= 1;
        return;
    }

    // Inserts a value into the list into a given index
    void insert(const int& index, const T& value) {
        if (index > current_size) {
            cerr << "IndexError: insert index out of range" << endl;
            return;
        } 
        current_size++;

        if (current_size <= max_size) { // Array can fit new value?
            for (int x {current_size}; x >= index; x--) { // Shift each value greater than the passed index up by one index
                values[x] = values[x-1];
            }
            values[index] = value;
            return;
        }
        doubleSize();

        for (int x {current_size}; x >= index; x--) { 
            values[x] = values[x-1];
        }
        values[index] = value;
        return;

    }

    void print() {
        cout << "[";
        for (int x {0}; x < current_size-1; x++) {
            cout << values[x] << ", ";
        }
        cout << values[current_size-1] << "]" << endl;
        return;
    }

    private:
        void doubleSize() {
            T* temp_values {new T[max_size]};

            // Copies values into temp_values
            for (int x {0}; x < max_size; x++) {
                temp_values[x] = values[x];
            }

            max_size *= 2;
            delete values;
            values = new T[max_size];

            // Copying list values back into the new values array 
            for (int x {0}; x < max_size; x++) {
                values[x] = temp_values[x];
            }
            delete temp_values;
            temp_values = nullptr;
            return;
        }
};

int main() {
    List<int> intList;
    intList.append(5);
    intList.append(23);
    intList.append(0);
    intList.append(56);
    intList.append(-23);

    intList.print();
    
    intList.pop(30);
    intList.pop(3);
    intList.pop(0);
      
    intList.print();

    intList.insert(30, 5);
    intList.insert(2, 500);
    intList.insert(0, 1000); 

    intList.print();

    return 0;
}