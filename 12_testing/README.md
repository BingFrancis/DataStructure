# 12 - Testing（测试）

本模块涵盖 C++ 测试的各种技术和工具。

## 目录结构

```
12_testing/
├── unit_testing/           # 单元测试
│   ├── google_test/        # Google Test 示例
│   ├── catch2_example.cpp  # Catch2 示例
│   └── doctest_example.cpp # doctest 示例
├── integration_testing/    # 集成测试
│   ├── database_test.cpp   # 数据库测试
│   └── api_test.cpp        # API 测试
├── mocking/                # Mock 和桩
│   ├── gmock_example.cpp   # Google Mock 示例
│   └── fake_objects.cpp    # 假对象示例
├── code_coverage/          # 代码覆盖率
│   ├── gcov_example.cpp    # gcov 示例
│   └── clang_coverage.cpp  # Clang 覆盖率
├── property_testing/       # 属性测试
│   ├── quickcheck_example.cpp # QuickCheck 示例
│   └── fuzz_testing.cpp    # 模糊测试
├── ci_cd/                  # CI/CD 配置
│   ├── github_actions/     # GitHub Actions
│   ├── gitlab_ci/          # GitLab CI
│   └── cmake/              # CMake 测试集成
└── README.md               # 本文件
```

## 测试类型

### 1. 单元测试 (Unit Testing)

测试最小的可测试单元（函数、类、方法）。

**优点：**
- 快速反馈
- 易于定位问题
- 支持重构

**工具：**
- Google Test (gtest)
- Catch2
- doctest

### 2. 集成测试 (Integration Testing)

测试多个组件协同工作。

**示例：**
- 数据库集成测试
- API 集成测试
- 模块间通信测试

### 3. 系统测试 (System Testing)

测试整个系统的功能。

### 4. 性能测试 (Performance Testing)

测试系统的性能指标。

**工具：**
- Google Benchmark
- 自定义基准测试

## 测试框架

### Google Test

#### 安装

```bash
# Linux
sudo apt-get install libgtest-dev

# 或从源码编译
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
sudo make install
```

#### 基础示例

```cpp
#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(MathTest, Addition) {
    EXPECT_EQ(add(1, 2), 3);
    EXPECT_EQ(add(-1, 1), 0);
}

TEST(MathTest, AdditionEdgeCases) {
    EXPECT_EQ(add(INT_MAX, 0), INT_MAX);
    EXPECT_EQ(add(INT_MIN, 0), INT_MIN);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

#### 编译和运行

```bash
g++ -std=c++17 test.cpp -lgtest -lgtest_main -pthread -o test
./test
```

### Catch2

#### 安装

```bash
# 单头文件版本
wget https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp
```

#### 基础示例

```cpp
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

int add(int a, int b) {
    return a + b;
}

TEST_CASE("Addition", "[math]") {
    REQUIRE(add(1, 2) == 3);
    REQUIRE(add(-1, 1) == 0);

    SECTION("Edge cases") {
        REQUIRE(add(INT_MAX, 0) == INT_MAX);
        REQUIRE(add(INT_MIN, 0) == INT_MIN);
    }
}
```

#### 编译和运行

```bash
g++ -std=c++17 test.cpp -o test
./test
```

## Mock 和桩

### Google Mock

```cpp
#include <gmock/gmock.h>

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual bool save(const std::string& key, const std::string& value) = 0;
    virtual std::string load(const std::string& key) = 0;
};

class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(bool, save, (const std::string&, const std::string&), (override));
    MOCK_METHOD(std::string, load, (const std::string&), (override));
};

class Service {
public:
    Service(IDatabase* db) : db_(db) {}

    bool process(const std::string& key) {
        auto value = db_->load(key);
        return db_->save(key, value + "_processed");
    }

private:
    IDatabase* db_;
};

TEST(ServiceTest, Process) {
    MockDatabase mockDb;
    Service service(&mockDb);

    EXPECT_CALL(mockDb, load("test"))
        .WillOnce(Return("value"));

    EXPECT_CALL(mockDb, save("test", "value_processed"))
        .WillOnce(Return(true));

    EXPECT_TRUE(service.process("test"));
}
```

## 代码覆盖率

### gcov

```bash
# 编译时添加覆盖率标志
g++ -std=c++17 --coverage test.cpp -o test

# 运行测试
./test

# 生成覆盖率报告
gcov test.cpp

# 使用 lcov 生成 HTML 报告
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory html
```

### clang-cov

```bash
# 编译时添加标志
clang++ -std=c++17 -fprofile-instr-generate -fcoverage-mapping test.cpp -o test
LLVM_PROFILE_FILE=coverage.profraw ./test

# 转换为可视化报告
llvm-profdata merge -sparse coverage.profraw -o coverage.profdata
llvm-cov show ./test -instr-profile=coverage.profdata
llvm-cov report ./test -instr-profile=coverage.profdata
```

## 基准测试

### Google Benchmark

#### 安装

```bash
git clone https://github.com/google/benchmark.git
cd benchmark
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

#### 示例

```cpp
#include <benchmark/benchmark.h>

static void BM_StringCreation(benchmark::State& state) {
    for (auto _ : state) {
        std::string empty_string;
    }
}

static void BM_StringCopy(benchmark::State& state) {
    std::string x = "hello";
    for (auto _ : state) {
        std::string copy(x);
    }
}

BENCHMARK(BM_StringCreation);
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
```

#### 编译和运行

```bash
g++ -std=c++17 benchmark.cpp -lbenchmark -pthread -o benchmark
./benchmark
```

## 测试最佳实践

### 1. 测试命名

```cpp
// ✅ 好的命名
TEST(StackTest, Push_Then_Top_ReturnsLastElement)
TEST(StackTest, Pop_RemovesTopElement)

// ❌ 不好的命名
TEST(StackTest, Test1)
TEST(StackTest, Works)
```

### 2. 测试独立性

```cpp
// ✅ 独立的测试
TEST(StackTest, PushWorks) {
    Stack<int> s;
    s.push(1);
    EXPECT_EQ(s.size(), 1);
}

TEST(StackTest, PopWorks) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.pop();
    EXPECT_EQ(s.size(), 1);
}

// ❌ 依赖的测试
TEST(StackTest, Test1) {
    static Stack<int> s;
    s.push(1);
}

TEST(StackTest, Test2) {
    static Stack<int> s;  // 依赖 Test1 的状态
    // ...
}
```

### 3. 测试覆盖率

- **语句覆盖率**: 执行的代码行数
- **分支覆盖率**: 执行的条件分支
- **路径覆盖率**: 执行的代码路径

目标：
- 关键代码: 90%+ 覆盖率
- 一般代码: 70-80% 覆盖率

### 4. 测试金字塔

```
        /\
       /E2E\      少量端到端测试
      /------\
     /Integration\ 适量集成测试
    /------------\
   /  Unit Tests  \ 大量单元测试
  /----------------\
```

## CI/CD 集成

### GitHub Actions

```yaml
name: CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2

    - name: Install dependencies
      run: sudo apt-get install -y libgtest-dev cmake

    - name: Build
      run: |
        mkdir build
        cd build
        cmake ..
        make

    - name: Test
      run: |
        cd build
        ctest --output-on-failure

    - name: Coverage
      run: |
        cd build
        gcov *.gcno
        bash <(curl -s https://codecov.io/bash)
```

### CMake 集成

```cmake
# CMakeLists.txt
enable_testing()

# Google Test
find_package(GTest REQUIRED)

add_executable(tests tests.cpp)
target_link_libraries(tests GTest::gtest GTest::gtest_main pthread)

add_test(NAME unit_tests COMMAND tests)

# 代码覆盖率
option(ENABLE_COVERAGE "Enable coverage" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(tests PRIVATE --coverage)
    target_link_options(tests PRIVATE --coverage)
endif()
```

## 模糊测试 (Fuzzing)

```cpp
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // 测试代码
    if (size >= 4) {
        int a = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        int b = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);
        add(a, b);  // 调用要测试的函数
    }
    return 0;
}
```

编译和运行：
```bash
clang++ -std=c++17 -fsanitize=fuzzer fuzzer.cpp -o fuzzer
./fuzzer
```

## 推荐学习顺序

1. 学习 Google Test/Catch2 基础用法
2. 编写第一个单元测试
3. 学习 Mock 和桩
4. 配置代码覆盖率
5. 学习基准测试
6. 设置 CI/CD 流水线

## 相关模块

- 所有模块的代码都应该有对应的测试

## 参考资源

- Google Test 官方文档
- Catch2 官方文档
- Google Benchmark 官方文档
- 《单元测试的艺术》
- 《测试驱动开发》- Kent Beck
