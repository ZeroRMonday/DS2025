#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <map>
#include <stdexcept>
using namespace std;

class Calculator {
private:
    map<char, int> priority;
    
    void initializePriority() {
        priority['+'] = 1;
        priority['-'] = 1;
        priority['*'] = 2;
        priority['/'] = 2;
        priority['^'] = 3;
        priority['('] = 0;
        priority[')'] = 0;
    }
    
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }
    
    double calculate(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw runtime_error("除数不能为零");
                return a / b;
            case '^': return pow(a, b);
            default: throw runtime_error("未知运算符");
        }
    }
    
public:
    Calculator() {
        initializePriority();
    }
    
    double evaluate(const string& expression) {
        stack<double> numStack;
        stack<char> opStack;
        
        for (size_t i = 0; i < expression.length(); i++) {
            char c = expression[i];
            
            if (c == ' ') continue;
            
            if (isDigit(c)) {
                string numStr;
                while (i < expression.length() && (isDigit(expression[i]) || expression[i] == '.')) {
                    numStr += expression[i++];
                }
                i--;
                
                numStack.push(stod(numStr));
            }
            else if (c == '(') {
                opStack.push(c);
            }
            else if (c == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    char op = opStack.top();
                    opStack.pop();
                    
                    double b = numStack.top();
                    numStack.pop();
                    double a = numStack.top();
                    numStack.pop();
                    
                    numStack.push(calculate(a, b, op));
                }
                if (!opStack.empty()) opStack.pop();
            }
            else if (isOperator(c)) {
                while (!opStack.empty() && priority[opStack.top()] >= priority[c]) {
                    char op = opStack.top();
                    opStack.pop();
                    
                    double b = numStack.top();
                    numStack.pop();
                    double a = numStack.top();
                    numStack.pop();
                    
                    numStack.push(calculate(a, b, op));
                }
                opStack.push(c);
            }
            else {
                throw runtime_error("无效字符: " + string(1, c));
            }
        }
        
        while (!opStack.empty()) {
            char op = opStack.top();
            opStack.pop();
            
            double b = numStack.top();
            numStack.pop();
            double a = numStack.top();
            numStack.pop();
            
            numStack.push(calculate(a, b, op));
        }
        
        if (numStack.size() != 1) {
            throw runtime_error("表达式无效");
        }
        
        return numStack.top();
    }
};

int main() {
    cout << "=== 第二题：栈计算器测试 ===" << endl;
    
    Calculator calc;
    
    string testCases[] = {
        "1 + 2 * 3",
        "(1 + 2) * 3",
        "10 / 2 - 3",
        "2 ^ 3 + 1",
        "3 * (4 + 5)",
        "10 - 2 * 3 + 4"
    };
    
    for (const string& expr : testCases) {
        try {
            double result = calc.evaluate(expr);
            cout << expr << " = " << result << endl;
        } catch (const exception& e) {
            cout << expr << " -> 错误: " << e.what() << endl;
        }
    }
    
    // 测试错误情况
    cout << "\n错误测试:" << endl;
    try {
        double result = calc.evaluate("1 / 0");
        cout << "1 / 0 = " << result << endl;
    } catch (const exception& e) {
        cout << "1 / 0 -> 错误: " << e.what() << endl;
    }
    
    cout << "\n=== 计算器测试完成! ===" << endl;
    cout << "按任意键继续...";
    cin.get();
    
    return 0;
}
