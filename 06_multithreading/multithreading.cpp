#include <iostream>
#include <thread>
#include <chrono>



void dosomething(int& i) {
    i += 10;
}
 

struct func {
    int& i;
    func(int& i_) : i(i_) {}
    void operator() (){
        for (unsigned j = 0; j < 100000000; ++j)
        {
            dosomething(i);
        }
    }
};




void opps(){
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // my_thread.detach();
    my_thread.join();
    std::cout << "result: "<< my_func.i << std::endl;
}

void some_function(){

};

void some_other_function(){

};

std::thread f() {
    return std::thread(some_function);
}



int	main(int argc, char **argv)

{
    // opps();
    std::thread t1(some_function);
    std::thread t2(some_other_function);
    t1.detach();
    t2.detach();
    return 0;
}
