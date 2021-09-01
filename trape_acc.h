#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
class TrapeAcc{
    public:
        TrapeAcc(){};
        ~TrapeAcc(){};
        void reset(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark);
        void calParame(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark);
        void calVelAcc(const float dist, const float period, const float vel, const float acc,  float& vel_next, float& acc_next);
        void print();
    private:
        float sign(float val){return val >= 0 ? 1.0 : -1.0;}
    
    private:
        float jark_1a_;
        float jark_fa_;
        float jark_1d_;
        float jark_fd_;

        float acc_ma_;
        float acc_md_;
        float vel_m_;
        float t_;

        float time1a_;
        float time1d_;
        float time2a_;
        float time2d_;
        float timefa_;
        float timefd_;

        float dist_a1_;
        float dist_a2_;
        float dist_af_;
        float dist_d1_;
        float dist_d2_;
        float dist_df_;
        float dist_im_;
};
