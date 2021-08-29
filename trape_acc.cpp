#include "trape_acc.h"

void TrapeAcc::reset(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark){
    this->jark_ = jark;
    this->acc_m_ = acc_m;
    this->vel_m_ = vel_m;

    // parameters while acceleration    
    this->time1a_ = (acc_m - acc_s) / jark;
    this->timefa_ = (vel_m / acc_m) + this->time1a_;
    this->time2a_ = this->timefa_ - (acc_m / jark);

    float vel_a1 = acc_s * this->time1a_ + jark * std::pow(this->time1a_, 2) * 0.5 + vel_s;
    float vel_a2 = acc_m * (this->time2a_ - this->time1a_) + vel_a1;
    
    this->dist_a1_ = acc_s * std::pow(this->time1a_, 2) * 0.5 + jark * std::pow(this->time1a_, 3) / 6 + vel_s * this->time1a_;
    this->dist_a2_ = acc_m * std::pow(this->time2a_ - this->time1a_, 2) * 0.5 + vel_a1 * (this->time2a_ - this->time1a_);
    this->dist_af_ = acc_m * std::pow(this->timefa_ - this->time2a_, 2) * 0.5 - jark * std::pow(this->timefa_ - this->time2a_, 3) / 6 + vel_a2 * (this->timefa_ - this->time2a_);


    // parameters while deceleration
    // time
    this->time1d_ = acc_m / jark;
    this->timefd_ = (vel_m / acc_m) + this->time1d_;
    this->time2d_ = this->timefa_ - (acc_m / jark);
    // velocity
    float vel_d1 = vel_m - jark * std::pow(this->time1d_, 2) * 0.5;
    float vel_d2 = vel_d1 - acc_m * (this->time2d_ - this->time1d_);
    // distance
    this->dist_d1_ = vel_m * this->time1d_ - jark * std::pow(this->time1d_, 3) / 6;
    this->dist_d2_ = vel_d1 * (this->time2d_ - this->time1d_) - acc_m * std::pow(this->time2d_ - this->time1d_, 2) * 0.5;
    this->dist_df_ = vel_d2 * (this->timefd_ - this->time2d_) - acc_m * std::pow(this->timefd_ - this->time2d_, 2) * 0.5 + 
     jark * std::pow(this->timefd_ - this->time2d_, 3) / 6;

    this->dist_im_ = dist - (this->dist_a1_ + this->dist_a2_ + this->dist_af_) - (this->dist_d1_ + this->dist_d2_ + this->dist_df_);
}

void TrapeAcc::calVelAcc(const float dist, const float period, const float vel, const float acc,  float acc_next, float vel_next){
    if (dist <= this->dist_df_){
        acc_next = std::max(acc + this->jark_ * period, static_cast<float>(0.0));
        vel_next = std::max(vel + acc_next * period, static_cast<float>(0.0));
    }
    else if (dist <= this->dist_d2_ + this->dist_df_){
        acc_next = -this->acc_m_;
        vel_next = std::max(vel + acc_next * period, static_cast<float>(0.0));
    }
    else if (dist <= this->dist_d1_ + this->dist_d2_ + this->dist_df_){
        acc_next = std::max(acc - this->jark_ * period, -this->acc_m_);
        vel_next = std::max(vel + acc_next * period, static_cast<float>(0.0));
    }
    // constant velocity
    else if (dist <= this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_){
        acc_next = static_cast<float>(0.0);
        vel_next = this->vel_m_;
    }
    else if (dist <= this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_){
        acc_next = std::max(acc - this->jark_, static_cast<float>(0.0));
        vel_next = std::min(vel + acc_next * period, this->vel_m_);
    }
    else if (dist <= this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ + this->dist_a2_){
        acc_next = this->acc_m_;
        vel_next = std::min(vel + acc_next * period, this->vel_m_);
    }
    else if (dist <= this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ + this->dist_a2_ + this->dist_a1_){
        acc_next = std::min(acc + this->jark_ * period, this->acc_m_);
        vel_next = std::min(vel + acc_next * period, this->vel_m_);
    }
}

int main(){
    TrapeAcc pro;
    float dist = 100.0;
    float acc_s = 0.0;
    float vel_s = 0.0;
    float acc_m = 20.0;
    float vel_m = 20.0;
    float jark = 10.0;
    float period = 0.01;
    float vel = vel_s;
    float acc = acc_s;
    float vel_n;
    float acc_n;
    pro.reset(dist, acc_s, vel_s, acc_m, vel_m, jark);
    while (dist > 1.0){
        pro.calVelAcc(dist, period, vel, acc, vel_n, acc_n);
        vel = vel_n;
        acc = acc_n;
        dist -= vel*period;
    }
    return 1;
}