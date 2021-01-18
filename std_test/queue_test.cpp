#include <queue>
#include <stdio.h>
#include <string>
#include <vector>

class A
{
public:
    A(const char *a)
    {
        a_ = a;
        printf("A: %s\n", a_.c_str());
    }

    ~A()
    {
        printf("~A: %s\n", a_.c_str());
    }

    void print()
    {
        printf("print: %s\n", a_.c_str());
    }

private:
    std::string a_;
};

void test_queue_class()
{
    std::queue<A> que;

    printf("\n");
    printf("declare...\n");
    A a1("a1");

    printf("\n");
    printf("push...\n");
    que.push(a1);

    printf("\n");
    printf("back...\n");
    que.front().print();

    printf("\n");
    printf("pop...\n");
    que.pop();

    printf("\n");
    printf("exit.\n");
}

void test_queue_class_ptr()
{
    std::queue<A *> que;

    printf("\n");
    printf("declare...\n");
    A *a1 = new A("a1");

    printf("\n");
    printf("push...\n");
    que.push(a1);

    printf("\n");
    printf("back...\n");
    A *a2 = que.back();
    delete a2;

    printf("\n");
    printf("pop...\n");
    que.pop();

    printf("\n");
    printf("exit.\n");
}

void test_queue_fifo()
{
    printf("\n");

    std::queue<A *> que;
    {
        que.push(new A("a1"));
        que.push(new A("a2"));
        que.push(new A("a3"));
    }
    printf("que size = %lu\n", que.size());
    while (!que.empty())
    {
        A *a = que.front();
        a->print();
        delete a;
        que.pop();
    }
    printf("que size = %lu\n", que.size());

    printf("\n");
}

int main(int argc, char *argv[])
{
    std::vector<float> vf(4, 0);
    // vf.push_back(5);
    // vf.push_back(6);
    // vf.push_back(7);
    // vf.push_back(8);
    printf("vf = %.2f / %.2f / %.2F / %.2f\n", vf[0], vf[1], vf[2], vf[3]);
    
    std::vector<float> vf2(vf);
    printf("vf = %.2f / %.2f / %.2F / %.2f\n", vf[0], vf[1], vf[2], vf[3]);
    printf("vf2 = %.2f / %.2f / %.2F / %.2f\n", vf2[0], vf2[1], vf2[2], vf2[3]);
    
    std::vector<float> *pvf = new std::vector<float>(vf);
    (*pvf)[0] += 2;
    printf("pvf = %.2f / %.2f / %.2F / %.2f\n", (*pvf)[0], (*pvf)[1], (*pvf)[2], (*pvf)[3]);
    printf("vf = %.2f / %.2f / %.2F / %.2f\n", vf[0], vf[1], vf[2], vf[3]);
    
    std::vector<float> *pv = new std::vector<float>;
    pv->push_back(1.0);
    pv->push_back(2.0);
    pv->push_back(3.0);
    pv->push_back(4.0);
    printf("v = %.2f / %.2f / %.2F / %.2f\n", (*pv)[0], (*pv)[1], (*pv)[2], (*pv)[3]);

    std::vector<float> *pv1 = new std::vector<float>;
    pv1->push_back(10.0);
    pv1->push_back(20.0);
    pv1->push_back(30.0);
    pv1->push_back(40.0);
    printf("v = %.2f / %.2f / %.2F / %.2f\n", (*pv1)[0], (*pv1)[1], (*pv1)[2], (*pv1)[3]);

    std::vector<float> *pv2 = new std::vector<float>;
    pv2->push_back(100.0);
    pv2->push_back(200.0);
    pv2->push_back(300.0);
    pv2->push_back(400.0);
    printf("v = %.2f / %.2f / %.2F / %.2f\n", (*pv2)[0], (*pv2)[1], (*pv2)[2], (*pv2)[3]);

    std::queue<std::vector<float> *> vels_queue_;
    vels_queue_.push(pv);
    vels_queue_.push(pv1);
    vels_queue_.push(pv2);
    printf("vels_queue_ size = %lu\n", vels_queue_.size());

    while (!vels_queue_.empty())
    {
        //std::vector<float> *pv = vels_queue_.front();
        //printf("pv = %.2f / %.2f / %.2F / %.2f\n", (*pv)[0], (*pv)[1], (*pv)[2], (*pv)[3]);
        //delete pv;
        delete vels_queue_.front();
        vels_queue_.pop();
    }
    printf("vels_queue_ size = %lu\n", vels_queue_.size());

    test_queue_class();
    //test_queue_class_ptr();
    //test_queue_fifo();
    //test_queue_vector();
    return 0;
}
