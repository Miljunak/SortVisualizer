#include "SortHandler.h"


SortHandler::SortHandler(std::string sortType, int high, int sleepInterval) :
        sortType_{std::move(sortType)}, sleepInterval_{sleepInterval}
{

    // Create array, positions and height unit to work on while sort_swap() happens
    float heightUnit = MAX_COLUMN_HEIGHT / static_cast<float>(high);
    float widthUnit = MAX_COLUMN_WIDTH / static_cast<float>(high);

    for (int i = 0; i < high; i++)
        array_.push_back(i + 1);

    std::shuffle(array_.begin(),
                 array_.end(),
                 std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    for (int i = 0; i < high; i++)
        positions_.push_back(static_cast<float>(array_[i] - 1) * widthUnit);

    // Create SFML window
    sf::RenderWindow window(
            sf::VideoMode(MAX_COLUMN_WIDTH,MAX_COLUMN_HEIGHT),
            sortType_);

    for (int i = 0; i < high; i++)
    {
        float pos = positions_[i];
        sf::RectangleShape rect(sf::Vector2f(widthUnit, -heightUnit * static_cast<float>(i + 1)));
        rect.setFillColor(sf::Color::Red);
        rect.setPosition(pos, MAX_COLUMN_HEIGHT);
        redRects_.push_back(rect);
    }

    if (sortType_ == "BUBBLESORT") {
        sortingThread_ = std::thread(&SortHandler::bubbleSort, this, std::ref(array_));
    }
    else if (sortType_ == "SELECTIONSORT") {
        sortingThread_ = std::thread(&SortHandler::selectionSort, this, std::ref(array_));
    }
    else if (sortType_ == "INSERTIONSORT") {
        sortingThread_ = std::thread(&SortHandler::insertionSort, this, std::ref(array_));
    }
    else if (sortType_ == "QUICKSORT") {
        sortingThread_ = std::thread(&SortHandler::quickSort, this, std::ref(array_), 0, high - 1);
        fast_ = true;
    }
    else if (sortType_ == "MERGESORT") {
        sortingThread_ = std::thread(&SortHandler::mergeSort, this, std::ref(array_), 0, high - 1);
    }
    else if (sortType_ == "SHELLSORT") {
        sortingThread_ = std::thread(&SortHandler::shellSort, this, std::ref(array_));
        fast_ = true;
    }
    else if (sortType_ == "COMBSORT") {
        sortingThread_ = std::thread(&SortHandler::combSort, this, std::ref(array_));
        fast_ = true;
    }
    else if (sortType_ == "HEAPSORT") {
        sortingThread_ = std::thread(&SortHandler::heapSort, this, std::ref(array_));
        fast_ = true;
    }
    else if (sortType_ == "RADIXSORT") {
        sortingThread_ = std::thread(&SortHandler::radixSort, this, std::ref(array_));
        fast_ = true;
    }
    else {
        sortingThread_ = std::thread(&SortHandler::countingSort, this, std::ref(array_));
    }


    // Main loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        // Clear the window
        window.clear();

        for (int i = 0; i < array_.size(); i++)
            window.draw(redRects_[i]);

        if (sleepInterval_ < 1000 && fast_)
        {
            for (int i = 0; i < 5000; i++)
                std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval_));
        }
        std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval_));

        window.display();
    }
}

void SortHandler::sortSwap(int x, int y)
{
    int temp = array_[x];
    array_[x] = array_[y];
    array_[y] = temp;

    // Update positions for the swapped elements
    float tempPos = positions_[x];
    redRects_[x].setPosition(positions_[y], MAX_COLUMN_HEIGHT);
    positions_[x] = positions_[y];
    redRects_[y].setPosition(tempPos, MAX_COLUMN_HEIGHT);
    positions_[y] = tempPos;
    if (sleepInterval_ < 1000 && fast_)
    {
        for (int i = 0; i < 5000; i++)
            std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval_));
    }
    std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval_));
}

void SortHandler::sortMove(int index, float newPos)
{
    redRects_[index].setPosition(newPos, MAX_COLUMN_HEIGHT);
    positions_[index] = newPos;
    if (sleepInterval_ < 1000 && fast_)
    {
        for (int i = 0; i < 5000; i++)
            std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval_));
    }
    std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval_));
}

void SortHandler::bubbleSort(std::vector<int> &arr)
{
    int n = static_cast<int>(arr.size());
    bool swapped = true;
    for (int i = 0; i < n - 1 && swapped; i++)
    {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                sortSwap(j, j + 1);

                // Check the flag to see if the window is still open
                if (!windowOpen_)
                    return;

                swapped = true;
            }
        }
    }
}


void SortHandler::selectionSort(std::vector<int>& arr)
{
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        sortSwap(min_idx, i);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }
}

void SortHandler::insertionSort(std::vector<int>& arr)
{
    for (int i = 1; i < arr.size(); i++)
    {
        int key = arr[i];
        int j = i - 1;
        float keyPos = positions_[i];

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            sortMove(j + 1, positions_[j]);
            j--;
        }

        arr[j + 1] = key;
        sortMove(j + 1, keyPos);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }
}

void SortHandler::quickSort(std::vector<int>& arr, int left, int right)
{
    if (left >= right) return;

    // Select the pivot element
    int pivot = arr[(left + right) / 2];

    // Partition the array
    int i = left, j = right;
    while (i <= j)
    {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j)
        {
            sortSwap(i, j);

            // Check the flag to see if the window is still open
            if (!windowOpen_)
                return;

            i++;
            j--;
        }
    }

    quickSort(arr, left, j);
    quickSort(arr, i, right);
}

void SortHandler::shellSort(std::vector<int>& arr)
{
    int n = static_cast<int>(arr.size());

    // Start with a big gap, then reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        // Do a gapped insertion sort for this gap size.
        for (int i = gap; i < n; i++)
        {
            // Store the current element and its original position
            int temp = arr[i];
            float tempPos = positions_[i];

            // Shift elements of the array that are greater than the current element to the right
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
            {
                arr[j] = arr[j - gap];
                sortMove(j, positions_[j - gap]);

                // Check the flag to see if the window is still open
                if (!windowOpen_)
                    return;
            }

            // Move the current element to its correct position
            arr[j] = temp;
            sortMove(j, tempPos);

            // Check the flag to see if the window is still open
            if (!windowOpen_)
                return;
        }
    }
}

void SortHandler::combSort(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    int gap = n;
    bool swapped = true;
    while (gap != 1 || swapped)
    {
        gap = gap * 10 / 13;
        if (gap < 1)
            gap = 1;

        swapped = false;
        for (int i = 0; i < n - gap; i++) {
            if (arr[i] > arr[i + gap]) {
                sortSwap(i, i + gap);
                swapped = true;

                // Check the flag to see if the window is still open
                if (!windowOpen_)
                    return;
            }
        }
    }
}

void SortHandler::heapify(std::vector<int>& arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
    {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest])
    {
        largest = right;
    }

    if (largest != i)
    {
        sortSwap(i, largest);
        heapify(arr, n, largest);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }
}

void SortHandler::heapSort(std::vector<int>& arr)
{
    int n = static_cast<int>(arr.size());

    // Build the max heap
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }

    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--)
    {
        sortSwap(0, i);
        heapify(arr, i, 0);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }
}

void SortHandler::countingSort(std::vector<int>& arr)
{
    int n = static_cast<int>(arr.size());
    int maxElement = *std::max_element(arr.begin(), arr.end());
    int minElement = *std::min_element(arr.begin(), arr.end());
    int range = maxElement - minElement + 1;

    std::vector<int> count(range), output(n), sorted_positions(n);

    // Count the occurrences of each element
    for (int i = 0; i < n; i++)
    {
        count[arr[i] - minElement]++;
    }

    // Generate the cumulative count
    for (int i = 1; i < range; i++)
    {
        count[i] += count[i - 1];
    }

    // Create the output array and update positions
    for (int i = n - 1; i >= 0; i--)
    {
        int index = count[arr[i] - minElement] - 1;
        output[index] = arr[i];
        sorted_positions[index] = static_cast<int>(positions_[i]);
        count[arr[i] - minElement]--;
    }

    // Update redRects' positions to match the sorted elements
    for (int i = 0; i < n; i++)
    {
        sortMove(i, static_cast<float>(sorted_positions[i]));

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }

    // Copy the output array to the original array
    for (int i = 0; i < n; i++)
    {
        arr[i] = output[i];
    }
}


void SortHandler::merge(std::vector<int>& arr, int start, int mid, int end)
{
    // Check the flag to see if the window is still open
    if (!windowOpen_)
        return;

    int start2 = mid + 1;

    // If the direct merge is already sorted
    if (arr[mid] <= arr[start2]) {
        return;
    }

    // Two pointers to maintain start of both vectors to merge
    while (start <= mid && start2 <= end) {
        // If element 1 is in the right place
        if (arr[start] <= arr[start2]) {
            start++;
        } else {
            // Swap element 2 with all elements to the right of element 1
            for (int i = start2; i > start; i--)
                sortSwap(i, i-1);

            // Update all the pointers
            start++;
            mid++;
            start2++;
        }
    }
}

void SortHandler::mergeSort(std::vector<int> &arr, int start, int end)
{
    if (start < end)
    {
        int mid = start + (end - start) / 2;
        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);
        merge(arr, start, mid, end);
    }
}


void SortHandler::radixSortPass(std::vector<int>& arr, int exp)
{
    int n = static_cast<int>(arr.size());
    std::vector<int> output(n);
    std::vector<int> count(10, 0);
    std::vector<float> sorted_positions(n);

    // Count the occurrences of each digit
    for (int i = 0; i < n; i++)
    {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // Generate the cumulative count
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Create the output array and update positions
    for (int i = n - 1; i >= 0; i--)
    {
        int digit = (arr[i] / exp) % 10;
        int index = count[digit] - 1;
        output[index] = arr[i];
        sorted_positions[index] = positions_[i];
        count[digit]--;
    }

    // Copy the output array to the original array and update redRects' positions
    for (int i = 0; i < n; i++)
    {
        arr[i] = output[i];
        sortMove(i, sorted_positions[i]);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }
}

void SortHandler::radixSort(std::vector<int>& arr)
{
    int maxElement = *std::max_element(arr.begin(), arr.end());

    for (int exp = 1; maxElement / exp > 0; exp *= 10)
    {
        radixSortPass(arr, exp);

        // Check the flag to see if the window is still open
        if (!windowOpen_)
            return;
    }
}


SortHandler::~SortHandler()
{
    windowOpen_ = false;
    if (sortingThread_.joinable()) {
        sortingThread_.join();
    }
}