#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <utility>

#include "../newton_method_continuation.hpp"


int main() {
    std::cout << std::setprecision(15);
    static constexpr int N = 2;
    // Solució inicial
    newton_method_continuation<N>::solution s_0{{1, 1}, 2};



    std::cout << "Nombre de solucions = ";
    int n = -1;
    while (std::cin >> n && n <= 0)
        ;

    std::cout << "h = ";
    double h;
    std::cin >> h;

    newton_method_continuation<N> metode([](const aVectorN<N>& x, const adept::adouble& p, aVectorN<N>& y) {
        // Modifiqueu aquesta funció pel que desitgeu
        y[0] = x[0] * x[0] + x[1] * x[1] - p;
        y[1] = x[0] - x[1] * x[1] * x[1];
    }, h);

    metode.add_solution(s_0);

    for (int i = 0; i < n; ++i) {
        metode.calculate();
        std::cout << "p = " << metode.last_solution().p << ", x = " << metode.last_solution().x << std::endl;
    }
}
