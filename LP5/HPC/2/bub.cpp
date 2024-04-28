#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Sequential bubble sort algorithm
void sequentialBubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel bubble sort algorithm
void parallelBubbleSort(vector<int> &arr)
{
    int n = arr.size();
    int i, j;
#pragma omp parallel for private(i, j)
    for (i = 0; i < n - 1; ++i)
    {
        for (j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
#pragma omp critical
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main()
{
    int n = 10000; // Size of the array
    vector<int> arr(n);

    // Initialize the array with random values
    for (int i = 0; i < n; ++i)
    {
        arr[i] = rand() % 1000;
    }

    // Sequential bubble sort
    vector<int> arr_seq = arr;
    auto start_seq = high_resolution_clock::now();
    sequentialBubbleSort(arr_seq);
    auto stop_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<milliseconds>(stop_seq - start_seq);
    cout << "Sequential Bubble Sort took " << duration_seq.count() << " milliseconds" << endl;

    // Parallel bubble sort
    vector<int> arr_par = arr;
    auto start_par = high_resolution_clock::now();
    parallelBubbleSort(arr_par);
    auto stop_par = high_resolution_clock::now();
    auto duration_par = duration_cast<milliseconds>(stop_par - start_par);
    cout << "Parallel Bubble Sort took " << duration_par.count() << " milliseconds" << endl;

    return 0;
}

#include <omp.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Function to merge two sorted subarrays
void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    vector<int> L(n1), R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }

    // Merge the temporary arrays back into arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }

    // Copy the remaining elements of L[], if any
    while (i < n1)
    {
        arr[k++] = L[i++];
    }

    // Copy the remaining elements of R[], if any
    while (j < n2)
    {
        arr[k++] = R[j++];
    }
}

// Sequential merge sort algorithm
void sequentialMergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel merge sort algorithm
void parallelMergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, l, m);
#pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main()
{
    const int n = 1000000; // Size of the array
    vector<int> arr(n);

    // Initialize the array with random values
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand();
    }

    // Measure the time taken by sequential merge sort
    auto start = high_resolution_clock::now();
    sequentialMergeSort(arr, 0, n - 1);
    auto stop = high_resolution_clock::now();
    auto durationSequential = duration_cast<milliseconds>(stop - start).count();
    cout << "Sequential merge sort took " << durationSequential << " milliseconds." << endl;

    // Reset the array for parallel merge sort
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand();
    }

    // Measure the time taken by parallel merge sort
    start = high_resolution_clock::now();
    parallelMergeSort(arr, 0, n - 1);
    stop = high_resolution_clock::now();
    auto durationParallel = duration_cast<milliseconds>(stop - start).count();
    cout << "Parallel merge sort took " << durationParallel << " milliseconds." << endl;

    return 0;
}
