#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
class TrapeAcc{
    public:
        TrapeAcc(){};
        ~TrapeAcc(){};
        typedef struct{
            double jark_1;
            double jark_f;
            double acc_m;
            double vel_m;
            double time_1;
            double time_2;
            double time_f;
            double dist_1;
            double dist_2;
            double dist_f;
        }Params;

        void reset(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark);
        void calParame(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark);
        bool calVelAcc(const double dist, const double period, const double vel, const double acc,  double& vel_next, double& acc_next);
        bool calVelAcc2(const double dist, const double period, const double vel, const double acc,  double& vel_next, double& acc_next);
        void print();
        void print(Params& param);
        void Decel(const double jark, const double acc_s, const double vel_s, const double acc_m, const double vel_m);
    private:

        double sign(double val){return val >= 0 ? 1.0 : -1.0;}
        void calParame2(Params& param, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark);
        void calVelAcc3(Params& param, double& acc_next, double vel_next);
        void update(Params& param, double t, double period, double acc, double vel, double& acc_next, double& vel_next);
    
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

        Params accel_;
        Params decel_;

        enum{
            Acc, Const, Dec
        } state_ = Acc;
};
