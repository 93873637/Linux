// Boost.Accumulators is both a library for incremental statistical computation as well as an extensible framework for incremental calculation in general. The library deals primarily with the concept of an accumulator, which is a primitive computational entity that accepts data one sample at a time and maintains some internal state. These accumulators may offload some of their computations on other accumulators, on which they depend. Accumulators are grouped within an accumulator set. Boost.Accumulators resolves the inter-dependencies between accumulators in a set and ensures that accumulators are processed in the proper order.

// The rolling mean is the mean over the last N samples. It is computed by dividing the rolling sum by the rolling count.

// Lazy or iterative calculation of the mean over the last N samples. The lazy calculation is associated with the tag::lazy_rolling_mean feature, and the iterative calculation (which is the default) with the tag::immediate_rolling_mean feature. Both can be extracted using the tag::rolling_mean() extractor.

// 把连续取得的N个采样值看成一个队列，队列的长度固定为N，
// 每次采样到一个新数据放入队尾，并扔掉原来队首的一次数据（先进先出原则），
// 把队列中的N个数据进行算术平均运算，获得新的滤波结果。

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <iostream>

using namespace boost::accumulators;

int rolling_mean_test()
{
    accumulator_set<int, stats<tag::rolling_mean>> acc(tag::rolling_window::window_size = 7);

    // push in some data ...
    acc(1);
    acc(2);
    acc(3);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;

    acc(4);
    acc(5);
    acc(6);
    acc(7);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;

    acc(8);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;

    acc(9);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;
}

int rolling_mean_test2()
{
    accumulator_set<double, stats<tag::rolling_mean>> acc(tag::rolling_window::window_size = 5);

    // push in some data ...
    acc(1);
    acc(2);
    acc(3);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;

    acc(4);
    acc(5);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;

    acc(6);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;

    acc(1.0);
    std::cout << "Mean: " << rolling_mean(acc) << std::endl;
}

// 输出
// Mean: 2
// Mean: 4
