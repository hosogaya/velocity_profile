#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
class TrapeAcc{
    public:
        TrapeAcc(){};
        ~TrapeAcc(){};
        void reset(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark);
        void calParame(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark);
        bool calVelAcc(const double dist, const double period, const double vel, const double acc,  double& vel_next, double& acc_next);
        void print();
    private:
        double sign(double val){return val >= 0 ? 1.0 : -1.0;}
    
    private:
        double jark_1a_;
        double jark_fa_;
        double jark_1d_;
        double jark_fd_;

        double acc_ma_;
        double acc_md_;
        double vel_m_;
        double t_;

        double time1a_;
        double time1d_;
        double time2a_;
        double time2d_;
        double timefa_;
        double timefd_;

        double dist_a1_;
        double dist_a2_;
        double dist_af_;
        double dist_d1_;
        double dist_d2_;
        double dist_df_;
        double dist_im_;
};
