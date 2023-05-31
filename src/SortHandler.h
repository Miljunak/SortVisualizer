#ifndef SORTVISUALIZER_SORTHANDLER_H
#define SORTVISUALIZER_SORTHANDLER_H

#include <string>
#include <utility>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_COLUMN_HEIGHT 600.f
#define MAX_COLUMN_WIDTH 1800.f

class SortHandler {
public:
    // Constructor
    SortHandler(std::string sortType, int high, int sleepInterval);

    // Destructor
    ~SortHandler();

    // Main function
    void sorting();

private:
    // Variables
    std::string sortType_;
    int sleepInterval_;
    int high_;
    std::vector<int> array_;
    std::vector<float> positions_;
    std::vector<sf::RectangleShape> redRects_;
    std::thread sortingThread_;
    bool windowOpen_{true};
    bool fast_{false};

    // Sorting algorithms
    void bubbleSort(std::vector<int>& arr);
    void selectionSort(std::vector<int>& arr);
    void insertionSort(std::vector<int>& arr);
    void quickSort(std::vector<int>& arr, int left, int right);
    void mergeSort(std::vector<int>& arr, int start, int end);
    void shellSort(std::vector<int>& arr);
    void combSort(std::vector<int>& arr);
    void heapSort(std::vector<int>& arr);
    void radixSort(std::vector<int>& arr);
    void countingSort(std::vector<int>& arr);
    void bogoSort(std::vector<int>& arr);

    // Helper functions
    void sortSwap(int x, int y);
    void sortMove(int index, float newPos);
    void merge(std::vector<int>& arr, int start, int mid, int end);
    void heapify(std::vector<int>& arr, int n, int i);
    void radixSortPass(std::vector<int>& arr, int exp);
};


#endif //SORTVISUALIZER_SORTHANDLER_H
