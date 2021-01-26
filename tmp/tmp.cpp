#include <iostream>
class Test {
    int _x, _y;
public:
    Test(int x, int y) :_x(x), _y(y) { std::cout << "ctor\n"; }
    int& getX() { return _x; }
    int& getY() { return _y; }
    ~Test() { std::cout << "dtor\n"; }
};

void* operator new[](std::size_t size) {
    std::cout << "array allocation size= " << size << "\n";
    void* p = malloc(size);
    return p;
}
void* operator new(std::size_t size) {
    std::cout << "allocating size =" << size << " \n";
    void* p = malloc(size);
    return p;

}
void operator delete(void* p) {
    free(p);
}
int main() {
    double* p = new (double[5]){ 1,23,3,4,5 };
    for (int i=0;i<5;++i)
        std::cout << p[i] << ",";
    std::cout << "\n";
    Test* t = new (Test){ 1,2 };
    /*Test* t=new Test(13,18);
    t->getX()=3;
    t->getY()=7;
    std::cout<<++t->getX()<<"\n";
    std::cout<<++t->getY()<<"\n";
    Test *p=(Test *)malloc(sizeof(Test));
    std::cout<<"allocated memory using malloc\n";
    new(p)Test(29,23);
    std::cout<<p->getX()<<"\n";
    p->~Test();
    free(p);
    */
    void* r = ::operator new (sizeof(Test));
    /*    std::cout<<"allocated memory using operator new\n";
        std::cout<<r<<"\n";
        Test* z=new(r)Test(11,12);
        std::cout<<z<<"\n";
        //z->~Test();
        //operator delete(z);
        operator delete (z);
        std::cout<<"finished\n";
        int d=int(1.3);*/
}