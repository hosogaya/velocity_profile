#pragma once

#include <iostream>
#include <array>
#include <complex>

class Profile{
    public:
        Profile();
        ~Profile();
        float calAcc(const float dist, const float period, const float vel, const float max_vel, const float max_acc);
    
    private:
        float calPhi(const float vel, const float max_vel);
        float calTimeF(const float dist, const float phi, const float max_vel);
        float calTime1(const float phi, const float vel, const float max_vel, const float max_acc);
        float calTime2(const float max_vel, const float max_acc);

    private:
        bool speed_up_ = true;
        float thres_ = 0.95;

};