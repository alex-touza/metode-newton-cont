#ifndef NEWTON_METHOD_CONTINUATION_HPP
#define NEWTON_METHOD_CONTINUATION_HPP

#include <adept.h>
#include <adept_arrays.h>
#include "../types.h"

template<int N>
class newton_method_continuation {
public:
    struct solution {
        VectorN<N> x;
        double p;
    };


private:
    std::function<void(const aVectorN<N>& x, const adept::adouble& p, aVectorN<N>& y)> function_;
    std::vector<solution> solutions_;

    double current_p{};
    VectorN<N> current_z;
    double h_;

    void calculate_jacobian(const VectorN<N>& x_val, double p_val, VectorN<N>& y_val, adept::Matrix& jacobian) {
        adept::Stack stack;
        aVectorN<N> x = x_val;
        adept::adouble p = p_val;

        stack.new_recording();
        aVectorN<N> y;
        this->function_(x, p, y);
        stack.independent(x);
        stack.independent(p);

        stack.dependent(y);


        jacobian = stack.jacobian();
        y_val = value(y);
    }

public:
    explicit newton_method_continuation(
            std::function<void(const aVectorN<N>& x, const adept::adouble& p, aVectorN<N>& y)>&& function, double h) :
        function_(std::move(function)), h_(h) {}

    const solution& last_solution() { return solutions_.back(); }

    /**
     *
     * @return Una matriu columna de mida N amb
     */
    void continuation() {
        const VectorN<N>& x_k = this->last_solution().x;
        const double p_k = this->last_solution().p;
        VectorN<N> y_k;
        adept::Matrix jacobian;
        this->calculate_jacobian(x_k, p_k, y_k, jacobian);

        // std::cout << "Jacobian: " << jacobian << std::endl;

        adept::Matrix df_dx = jacobian(adept::range(0, N - 1), adept::range(0, N - 1));
        adept::Matrix df_dp = jacobian(adept::range(0, N - 1), adept::range(N, N));

        // Calculem dx/dp (p_k)
        adept::Matrix dx_dp_pk = -adept::matmul(adept::inv(df_dx), df_dp).cast();
        VectorN<N> dx_dp_pk_vec = dx_dp_pk(adept::range(0, N - 1), 0);
        this->current_z = x_k + this->h_ * dx_dp_pk_vec;
        this->current_p = p_k + this->h_;
    }

    bool step() {
        // Resolem el sistema DF(z_(k-1)) Delta z = - F(z_(k-1))

        adept::Matrix jacobian;
        VectorN<N> F_z;
        calculate_jacobian(this->current_z, this->current_p, F_z, jacobian);

        if (adept::norm2(F_z) < epsilon)
            return false;

        F_z *= -1;
        VectorN<N> delta_z = solve(jacobian, F_z);
        this->current_z += delta_z;

        return true;
    }

    void add_solution(solution&& solution) { solutions_.emplace_back(std::move(solution)); }

    void add_solution(const solution& solution) { solutions_.push_back(solution); }

    void calculate(solution&& solution) {
        this->add_solution(solution);
        this->calculate();
    }

    void calculate(const solution& solution) {
        this->add_solution(solution);
        this->calculate();
    }

    void calculate() {
        assert(!solutions_.empty());

        this->continuation();
        while (step())
            ;
        this->solutions_.push_back({this->current_z, this->current_p});
    }

    static constexpr double epsilon = 1e-8;
};

#endif // NEWTON_METHOD_CONTINUATION_HPP