#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <ctime>
#include <cstdlib>
using namespace std;

class Histogram {
public:
    // 暴力解法 O(n^2)
    int largestRectangleAreaBrute(const vector<int>& heights) {
        int maxArea = 0;
        int n = heights.size();
        
        for (int i = 0; i < n; i++) {
            int minHeight = heights[i];
            for (int j = i; j < n; j++) {
                minHeight = min(minHeight, heights[j]);
                maxArea = max(maxArea, minHeight * (j - i + 1));
            }
        }
        
        return maxArea;
    }
    
    // 栈解法 O(n)
    int largestRectangleAreaStack(const vector<int>& heights) {
        stack<int> st;
        int maxArea = 0;
        int n = heights.size();
        
        for (int i = 0; i <= n; i++) {
            int currentHeight = (i == n) ? 0 : heights[i];
            
            while (!st.empty() && currentHeight < heights[st.top()]) {
                int height = heights[st.top()];
                st.pop();
                int width = st.empty() ? i : i - st.top() - 1;
                maxArea = max(maxArea, height * width);
            }
            
            st.push(i);
        }
        
        return maxArea;
    }
    
    // 生成随机测试数据
    vector<int> generateRandomHeights(int size, int maxHeight = 100) {
        vector<int> heights;
        for (int i = 0; i < size; i++) {
            heights.push_back(rand() % (maxHeight + 1));
        }
        return heights;
    }
};

int main() {
    cout << "=== 第三题：柱状图最大面积测试 ===" << endl;
    
    Histogram hist;
    
    // 示例测试
    vector<int> example1 = {2, 1, 5, 6, 2, 3};
    vector<int> example2 = {2, 4};
    
    cout << "示例1 [2,1,5,6,2,3]:" << endl;
    cout << "  暴力解法: " << hist.largestRectangleAreaBrute(example1) << endl;
    cout << "  栈解法: " << hist.largestRectangleAreaStack(example1) << endl;
    
    cout << "示例2 [2,4]:" << endl;
    cout << "  暴力解法: " << hist.largestRectangleAreaBrute(example2) << endl;
    cout << "  栈解法: " << hist.largestRectangleAreaStack(example2) << endl;
    
    // 随机测试
    cout << "\n随机数据测试（5组）:" << endl;
    cout << "序号\t数据规模\t暴力解法\t栈解法\t结果一致" << endl;
    cout << "------------------------------------------------" << endl;
    
    for (int i = 1; i <= 5; i++) {
        int size = 5 + i * 3;
        vector<int> heights = hist.generateRandomHeights(size, 50);
        
        int result1 = hist.largestRectangleAreaBrute(heights);
        int result2 = hist.largestRectangleAreaStack(heights);
        
        cout << i << "\t" << size << "\t\t" << result1 << "\t\t" 
             << result2 << "\t\t" << (result1 == result2 ? "是" : "否") << endl;
    }
    
    cout << "\n=== 柱状图测试完成! ===" << endl;
    cout << "按任意键继续...";
    cin.get();
    
    return 0;
}
