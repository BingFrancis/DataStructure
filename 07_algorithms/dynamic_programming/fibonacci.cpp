/**
 * 斐波那契数列 - 动态规划示例
 *
 * 定义：F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)
 *
 * 不同方法的时间复杂度对比：
 * - 递归：O(2^n) - 指数级，有大量重复计算
 * - 记忆化递归：O(n)
 * - 自底向上 DP：O(n)
 * - 空间优化 DP：O(n)
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 方法1：普通递归（效率低）
long long fibonacciRecursive(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacciRecursive(n - 1) + fibonacciRecursive(n - 2);
}

// 方法2：记忆化递归（Top-Down）
vector<long long> memo;

long long fibonacciMemoization(int n) {
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }
    memo[n] = fibonacciMemoization(n - 1) + fibonacciMemoization(n - 2);
    return memo[n];
}

// 方法3：自底向上动态规划（Bottom-Up）
long long fibonacciDP(int n) {
    if (n <= 1) {
        return n;
    }

    vector<long long> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    return dp[n];
}

// 方法4：空间优化的动态规划
long long fibonacciDPOptimized(int n) {
    if (n <= 1) {
        return n;
    }

    long long prev2 = 0;  // dp[i-2]
    long long prev1 = 1;  // dp[i-1]
    long long current;

    for (int i = 2; i <= n; i++) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}

// 方法5：矩阵快速幂（O(log n)）
void multiply(long long F[2][2], long long M[2][2]) {
    long long x = F[0][0] * M[0][0] + F[0][1] * M[1][0];
    long long y = F[0][0] * M[0][1] + F[0][1] * M[1][1];
    long long z = F[1][0] * M[0][0] + F[1][1] * M[1][0];
    long long w = F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = x;
    F[0][1] = y;
    F[1][0] = z;
    F[1][1] = w;
}

void power(long long F[2][2], int n) {
    if (n == 0 || n == 1) {
        return;
    }

    long long M[2][2] = {{1, 1}, {1, 0}};

    power(F, n / 2);
    multiply(F, F);

    if (n % 2 != 0) {
        multiply(F, M);
    }
}

long long fibonacciMatrix(int n) {
    if (n <= 1) {
        return n;
    }

    long long F[2][2] = {{1, 1}, {1, 0}};
    power(F, n - 1);

    return F[0][0];
}

// 测试并计时
void testFunction(string name, long long (*func)(int), int n) {
    auto start = high_resolution_clock::now();
    long long result = func(n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << name << ": F(" << n << ") = " << result
         << ", 耗时: " << duration.count() << " 微秒" << endl;
}

int main() {
    int n = 40;  // 小一点的数用于普通递归测试

    cout << "=== 斐波那契数列不同方法对比 ===" << endl;
    cout << endl;

    cout << "测试 n = " << n << endl;
    testFunction("普通递归", fibonacciRecursive, n);

    memo.assign(n + 1, -1);
    testFunction("记忆化递归", fibonacciMemoization, n);

    testFunction("自底向上DP", fibonacciDP, n);
    testFunction("空间优化DP", fibonacciDPOptimized, n);
    testFunction("矩阵快速幂", fibonacciMatrix, n);

    cout << endl;
    cout << "=== 测试更大的数 (n = 90) ===" << endl;
    n = 90;
    testFunction("自底向上DP", fibonacciDP, n);
    testFunction("空间优化DP", fibonacciDPOptimized, n);
    testFunction("矩阵快速幂", fibonacciMatrix, n);

    return 0;
}
