#include <iostream>
#include <thread>

class threadGuard {
public:
    std::thread& t;
    explicit threadGuard(std::thread& t_) :t(t_) {}
    ~threadGuard(){}
private:

};
int main(){
    return 0;
}