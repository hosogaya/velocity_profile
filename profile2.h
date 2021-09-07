#pragma once
// #include <Arduino.h>

class TrapeAcc{
    public:
        TrapeAcc(){};
        ~TrapeAcc(){};
        typedef struct{
            float jark_1;
            float jark_f;
            float acc_m;
            float vel_m;
            float time_1;
            float time_2;
            float time_f;
            float dist_1;
            float dist_2;
            float dist_f;
        }Params;

        void reset(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark);
        bool calVelAcc(const float dist, const float period, const float acc, const float vel,  float& acc_next, float& vel_next);
        void Decel(const float jark, const float acc_s, const float vel_s, const float acc_m, const float vel_m);
        void print();
        void print(Params& param);
    private:
        float sign(float val){return val >= 0 ? 1.0 : -1.0;}
        void calParame(Params& param, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark);
        void update(Params& param, float t, float period, float acc, float vel, float& acc_next, float& vel_next);
    
    private:
        float t_;
        Params accel_;
        Params decel_;

        enum{
            Acc, Const, Dec
        } state_ = Acc;
};
