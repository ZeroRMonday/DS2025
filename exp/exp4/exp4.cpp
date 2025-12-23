#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;


struct Box {
    double x1;
    double y1;
    double x2;
    double y2;
    double score;
};


double IoU(const Box &a, const Box &b) {
    double xx1 = max(a.x1, b.x1);
    double yy1 = max(a.y1, b.y1);
    double xx2 = min(a.x2, b.x2);
    double yy2 = min(a.y2, b.y2);

    double w = max(0.0, xx2 - xx1);
    double h = max(0.0, yy2 - yy1);
    double inter = w * h;

    if (inter <= 0.0) return 0.0;

    double areaA = (a.x2 - a.x1) * (a.y2 - a.y1);
    double areaB = (b.x2 - b.x1) * (b.y2 - b.y1);
    double uni = areaA + areaB - inter;
    if (uni <= 0.0) return 0.0;

    return inter / uni;
}


void quickSortIndices(vector<int> &idx, int left, int right, const vector<Box> &boxes) {
    if (left >= right) return;
    int i = left, j = right;
    double pivot = boxes[idx[(left + right) / 2]].score;

    while (i <= j) {
        while (boxes[idx[i]].score > pivot) i++;   
        while (boxes[idx[j]].score < pivot) j--;  
        if (i <= j) {
            int tmp = idx[i];
            idx[i] = idx[j];
            idx[j] = tmp;
            i++;
            j--;
        }
    }
    if (left < j) quickSortIndices(idx, left, j, boxes);
    if (i < right) quickSortIndices(idx, i, right, boxes);
}

vector<int> quick_sort_indices(const vector<Box> &boxes) {
    int n = (int)boxes.size();
    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i;
    if (n > 0)
        quickSortIndices(idx, 0, n - 1, boxes);
    return idx;
}


void mergeIndices(vector<int> &idx, int l, int m, int r, const vector<Box> &boxes) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = idx[l + i];
    for (int j = 0; j < n2; ++j) R[j] = idx[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (boxes[L[i]].score >= boxes[R[j]].score) {
            idx[k++] = L[i++];
        } else {
            idx[k++] = R[j++];
        }
    }
    while (i < n1) idx[k++] = L[i++];
    while (j < n2) idx[k++] = R[j++];
}

void mergeSortIndices(vector<int> &idx, int l, int r, const vector<Box> &boxes) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSortIndices(idx, l, m, boxes);
    mergeSortIndices(idx, m + 1, r, boxes);
    mergeIndices(idx, l, m, r, boxes);
}

vector<int> merge_sort_indices(const vector<Box> &boxes) {
    int n = (int)boxes.size();
    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i;
    if (n > 0)
        mergeSortIndices(idx, 0, n - 1, boxes);
    return idx;
}


void heapify(vector<int> &idx, int n, int i, const vector<Box> &boxes) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && boxes[idx[l]].score > boxes[idx[largest]].score)
        largest = l;
    if (r < n && boxes[idx[r]].score > boxes[idx[largest]].score)
        largest = r;

    if (largest != i) {
        int tmp = idx[i];
        idx[i] = idx[largest];
        idx[largest] = tmp;
        heapify(idx, n, largest, boxes);
    }
}

vector<int> heap_sort_indices(const vector<Box> &boxes) {
    int n = (int)boxes.size();
    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i;


    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(idx, n, i, boxes);

 
    for (int i = n - 1; i >= 0; --i) {
        int tmp = idx[0];
        idx[0] = idx[i];
        idx[i] = tmp;
        heapify(idx, i, 0, boxes);
    }

   
    for (int i = 0; i < n / 2; ++i) {
        int tmp = idx[i];
        idx[i] = idx[n - 1 - i];
        idx[n - 1 - i] = tmp;
    }
    return idx;
}  


vector<int> insertion_sort_indices(const vector<Box> &boxes) {
    int n = (int)boxes.size();
    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i;

    for (int i = 1; i < n; ++i) {
        int key = idx[i];
        double keyScore = boxes[key].score;
        int j = i - 1;
        while (j >= 0 && boxes[idx[j]].score < keyScore) {
            idx[j + 1] = idx[j];
            --j;
        }
        idx[j + 1] = key;
    }
    return idx;
}


vector<Box> nms(const vector<Box> &boxes,
                double iou_threshold,
                const string &algo_name) {
    int n = (int)boxes.size();
    if (n == 0) return vector<Box>();

    vector<int> sorted_idx;
    if (algo_name == "quick") {
        sorted_idx = quick_sort_indices(boxes);
    } else if (algo_name == "merge") {
        sorted_idx = merge_sort_indices(boxes);
    } else if (algo_name == "heap") {
        sorted_idx = heap_sort_indices(boxes);
    } else if (algo_name == "insertion") {
        sorted_idx = insertion_sort_indices(boxes);
    } else {
    
        sorted_idx = quick_sort_indices(boxes);
    }

    vector<bool> suppressed(n, false);
    vector<int> picked;

    for (int i = 0; i < n; ++i) {
        if (suppressed[i]) continue;
        int idx_i = sorted_idx[i];
        picked.push_back(idx_i);

        for (int j = i + 1; j < n; ++j) {
            if (suppressed[j]) continue;
            int idx_j = sorted_idx[j];
            double iou = IoU(boxes[idx_i], boxes[idx_j]);
            if (iou > iou_threshold) {
                suppressed[j] = true;
            }
        }
    }

    vector<Box> result;
    result.reserve(picked.size());
    for (size_t k = 0; k < picked.size(); ++k) {
        result.push_back(boxes[picked[k]]);
    }
    return result;
}


Box make_random_box(int img_w, int img_h) {
    int w = rand() % 181 + 20; // 20~200
    int h = rand() % 181 + 20;
    int x1 = rand() % (img_w - w + 1);
    int y1 = rand() % (img_h - h + 1);
    int x2 = x1 + w;
    int y2 = y1 + h;
    double score = (double)rand() / RAND_MAX;
    Box b;
    b.x1 = x1; b.y1 = y1; b.x2 = x2; b.y2 = y2; b.score = score;
    return b;
}


vector<Box> generate_random_boxes(int n, int img_w = 1000, int img_h = 1000) {
    vector<Box> boxes;
    boxes.reserve(n);
    for (int i = 0; i < n; ++i) {
        boxes.push_back(make_random_box(img_w, img_h));
    }
    return boxes;
}


vector<Box> generate_clustered_boxes(int n,
                                     int img_w = 1000,
                                     int img_h = 1000,
                                     int num_clusters = 5) {
    vector<Box> boxes;
    boxes.reserve(n);


    vector<pair<int,int> > centers;
    for (int i = 0; i < num_clusters; ++i) {
        int cx = rand() % (img_w - 200) + 100;
        int cy = rand() % (img_h - 200) + 100;
        centers.push_back(make_pair(cx, cy));
    }

    for (int i = 0; i < n; ++i) {
        int c = rand() % num_clusters;
        int cx = centers[c].first;
        int cy = centers[c].second;

        int dx = rand() % 101 - 50;
        int dy = rand() % 101 - 50;

        int w = rand() % 181 + 20;
        int h = rand() % 181 + 20;

        int x1 = cx + dx - w / 2;
        int y1 = cy + dy - h / 2;

        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (x1 + w > img_w) x1 = img_w - w;
        if (y1 + h > img_h) y1 = img_h - h;

        int x2 = x1 + w;
        int y2 = y1 + h;
        double score = (double)rand() / RAND_MAX;

        Box b;
        b.x1 = x1; b.y1 = y1; b.x2 = x2; b.y2 = y2; b.score = score;
        boxes.push_back(b);
    }
    return boxes;
}


int main() {
    
    srand((unsigned)time(NULL));

    double iou_threshold = 0.5;

    int sizes[] = {100, 500, 1000, 5000, 10000};
    int num_sizes = 5;

    string distributions[] = {"random", "clustered"};
    int num_dists = 2;

    string algos[] = {"quick", "merge", "heap", "insertion"};
    int num_algos = 4;

    cout << "==== NMS + 不同排序算法性能测试 (C++) ====" << endl;

    for (int d = 0; d < num_dists; ++d) {
        string dist = distributions[d];
        cout << endl << "=== 数据分布: " << dist << " ===" << endl;

        for (int si = 0; si < num_sizes; ++si) {
            int n = sizes[si];
            cout << endl << "--- N = " << n << " ---" << endl;

            vector<Box> boxes;
            if (dist == "random") {
                boxes = generate_random_boxes(n);
            } else {
                boxes = generate_clustered_boxes(n);
            }

            for (int a = 0; a < num_algos; ++a) {
                string algo = algos[a];

         
                clock_t start = clock();
                vector<Box> kept = nms(boxes, iou_threshold, algo);
                clock_t end = clock();

                double elapsed_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;

                cout << "算法: " << algo
                     << " | 用时: " << elapsed_ms << " ms"
                     << " | 保留框数量: " << kept.size()
                     << endl;
            }
        }
    }

    return 0;
}

