#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std;

bool isSorted(const int* array, const int low, const int high) {
    for (int i {high}; i > low; i--) {
        if (array[i] < array[i-1]) {
            return false;
        }
    }
    return true;
}

void shuffle(int* array, const int size) {
    int temp_index {0};

    // Swaps elements 4x the same number of times as the number of values in the array
    for (int x {0}; x < size*4; x++) {
        temp_index = rand() % size;

        // Swap values
        swap(array[0], array[temp_index]);
   }
}

void print(const int* array, const int low, const int high) {
    cout << "[";
    for (int i {low}; i < high; i++) {
        cout << array[i] << ", ";
    }
    cout << array[high] << "]" << endl;
}

void selectionSort(int* array, const int size) {
    auto start {chrono::high_resolution_clock::now()};

    int value_index {0};

    // One by one move boundary of sorted array
    for (int i {0}; i < size-1; i++) {
        value_index = i;
        // Find smallest element in unsorted array
        for (int x {i+1}; x < size; x++) {
            if (array[x] < array[value_index]) {
                value_index = x;
            }
        }
        // Swap minimum element with first unsorted element
        if (value_index != i) {
            swap(array[value_index], array[i]);
        }
    }
    auto stop {chrono::high_resolution_clock::now()};
    cout << "Sorted using selection sort in " << chrono::duration_cast<chrono::milliseconds>(stop-start).count() 
    << " ms." << endl;
}

void bogoSort(int* array, const int size) {
    auto start {chrono::high_resolution_clock::now()};

    // Shuffles array until array is sorted
    while (!isSorted(array, size-1, 0)) {
        shuffle(array, size);
    }
    auto stop {chrono::high_resolution_clock::now()};
    cout << "Sorted using bogo sort in " << chrono::duration_cast<chrono::milliseconds>(stop-start).count() 
    << " ms." << endl;
}

void quickSort(int* arr, const int low, const int high) {
    int left {low};
    int right {high};
    int pivot_val {arr[(low+high)/2]};
    int temp_val;

    while (left < right) {
        if (arr[left] >= pivot_val && arr[right] <= pivot_val) {
            // Swap left and right values
            temp_val = arr[left];
            arr[left] = arr[right];
            arr[right] = temp_val;
            left++;
        }
        else if (arr[left] < pivot_val) { // Left doesn't need to be swapped, move to the right
            left++;
        }
        else { // Right doesn't need to be swapped, move to the left
            right--;
        }
    }

    if (isSorted(arr, low, high)) {
        return;
    }

    if (left == low) {
        quickSort(arr, left + 1, high);
    }
    else {
        quickSort(arr, low, left - 1);
        quickSort(arr, left, high);
    }
}

void storeValues(int* arr, int size) {
    // Stores values into the passed array, starting at 0 and increasing by one for each index.
    for (int i {0}; i < size; i++) {
        arr[i] = i;
    }
}

void storeRandom(int* arr, int size) {
    // Stores random values from 0 to 1000 into the passed array.
    for (int i {0}; i < size; i++) {
        arr[i] = rand() % 1000;
    }
}

int linearSearch(int* arr, int high, int goal) {
    for (int i; i <= high; i++) {
        if (arr[i] == goal) {
            return i;
        }
    }
    return -1;
}

int binarySearch(int* arr, int low, int high, int goal) {
    int mid = (low+high)/2;

    if (arr[mid] < goal) {
        return binarySearch(arr, mid, high, goal);
    }
    else if (arr[mid] > goal) {
        return binarySearch(arr, low, mid, goal);
    }
    return mid;
}

int main() {

    // Seeding the random number generator
    srand (time(NULL));

    const int arr_size {100000};
    int arr[arr_size] = { 0 };
    // int base_arr[arr_size] = {0};

    storeValues(arr, arr_size);
    cout << binarySearch(arr, 0, arr_size-1, 166854) << endl;
    cout << linearSearch(arr, arr_size-1, 166854) << endl;

    shuffle(arr, arr_size);
    
    auto start {chrono::high_resolution_clock::now()};
    quickSort(arr, 0, arr_size-1);
    auto stop {chrono::high_resolution_clock::now()};

    cout << "Sorted using quicksort in " << chrono::duration_cast<chrono::milliseconds>(stop-start).count() 
    << " ms." << endl;
}
