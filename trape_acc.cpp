#include "trape_acc.h"
#include <fstream>
#include <iostream>
void TrapeAcc::reset(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark){
    this->calParame(dist, acc_s, vel_s, acc_m, vel_m, jark);
    while(std::abs(dist) < std::abs(this->dist_a1_ + this->dist_a2_ + this->dist_af_ + this->dist_d1_ + this->dist_d2_ + this->dist_df_)){
        this->calParame(dist, acc_s, vel_s, acc_m, this->vel_m_ * 0.8, jark);
    }
    std::cout << this->time1a_ << ", " << this->time2a_ << ", " << this->timefa_ << std::endl;
    std::cout << this->time1d_ << ", " << this->time2d_ << ", " << this->timefd_ << std::endl;
    std::cout << this->dist_a1_ << ", " << this->dist_a2_ << ", " << this->dist_af_ << std::endl;
    std::cout << this->dist_d1_ << ", " << this->dist_d2_ << ", " << this->dist_df_ << std::endl;
    std::cout << this->dist_im_ << std::endl;
    std::cout << this->vel_m_ << std::endl;
    std::cout << this->acc_ma_  << ", " << this->acc_md_ << std::endl;
}

void TrapeAcc::calParame(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark){
    this->vel_m_ = std::abs(vel_m) * this->sign(dist);
    this->acc_ma_ = std::abs(acc_m) * this->sign(this->vel_m_ - vel_s);
    this->acc_md_ = -std::abs(acc_m) * this->sign(this->vel_m_);
    
    this->jark_1a_ = std::abs(jark) * this->sign(this->acc_ma_ - acc_s);
    this->jark_fa_ = -std::abs(jark) * this->sign(this->acc_ma_);
    this->jark_1d_ = std::abs(jark) * this->sign(this->acc_md_);
    this->jark_fd_ = -std::abs(jark) * this->sign(this->acc_md_);
    this->t_ = 0.0;

    float delta_vel_a1 = (std::pow(this->acc_ma_, 2) - std::pow(acc_s, 2)) / (2 * this->jark_1a_);
    float delta_vel_af = 3 * std::pow(this->acc_ma_, 2) / (2 * this->jark_fa_);

    bool is_acc_const = true;
    if (std::abs(delta_vel_a1 + delta_vel_af) > std::abs(this->vel_m_ - vel_s)){
        this->acc_ma_ = std::sqrt(std::abs((2 * this->jark_1a_ *  this->jark_fa_) * (this->vel_m_ - vel_s + std::pow(acc_s, 2) / this->jark_1a_) / (3 * this->jark_1a_ + this->jark_fa_))) * this->sign(this->acc_ma_);
        is_acc_const = false;
    }
    // parameters while acceleration    
    this->time1a_ = std::abs((this->acc_ma_ - acc_s) / this->jark_1a_);
    this->timefa_ = std::abs((this->vel_m_ - vel_s - acc_s * this->time1a_ - std::pow(this->acc_ma_ - acc_s, 2) / (2 * this->jark_1a_) - std::pow(this->acc_ma_, 2) / (2 * this->jark_fa_)) / this->acc_ma_) + this->time1a_;
    // this->timefa_ = std::abs((this->vel_m_) / this->acc_ma_) + this->time1a_;
    if (is_acc_const){
        this->time2a_ = this->timefa_ - std::abs(this->acc_ma_ / this->jark_fa_);
    }else{
        this->time2a_ = this->time1a_;
    }

    float vel_a1 = delta_vel_a1 + vel_s;
    float vel_a2 = this->acc_ma_ * (this->time2a_ - this->time1a_) + vel_a1;
    
    this->dist_a1_ = acc_s * std::pow(this->time1a_, 2) * 0.5 + this->jark_1a_ * std::pow(this->time1a_, 3) / 6 + vel_s * this->time1a_;
    this->dist_a2_ = this->acc_ma_ * std::pow(this->time2a_ - this->time1a_, 2) * 0.5 + vel_a1 * (this->time2a_ - this->time1a_);
    this->dist_af_ = this->acc_ma_ * std::pow(this->timefa_ - this->time2a_, 2) * 0.5 + this->jark_fa_ * std::pow(this->timefa_ - this->time2a_, 3) / 6 + vel_a2 * (this->timefa_ - this->time2a_);


    is_acc_const = true;
    float delta_vel_d1 = std::pow(this->acc_md_, 2) / (2 * this->jark_1d_);
    float delta_vel_df = 3 * std::pow(this->acc_md_, 2) / (2 * this->jark_fd_);

    if (std::abs(delta_vel_d1 + delta_vel_df)> std::abs(this->vel_m_)){
        this->acc_md_ = std::sqrt((2 * this->jark_1d_ * this->jark_fd_ * this->vel_m_) / (3 * this->jark_1d_ + this->jark_fd_)) * this->sign(this->acc_md_);
        is_acc_const = false;
    }
    // parameters while deceleration
    // time
    this->time1d_ = std::abs(this->acc_md_ / this->jark_1d_);
    this->timefd_ = std::abs(this->vel_m_ / this->acc_md_) + this->time1d_;
    if(is_acc_const){
        this->time2d_ = this->timefd_ - std::abs(this->acc_md_ / this->jark_fd_);
    }else{
        this->time2d_ = this->time1d_;
    }
    // velocity
    float vel_d1 = this->vel_m_ + this->jark_1d_ * std::pow(this->time1d_, 2) * 0.5;
    float vel_d2 = vel_d1 + this->acc_md_ * (this->time2d_ - this->time1d_);
    // distance
    this->dist_d1_ = this->vel_m_ * this->time1d_ + this->jark_1d_ * std::pow(this->time1d_, 3) / 6;
    this->dist_d2_ = vel_d1 * (this->time2d_ - this->time1d_) +  this->acc_md_ * std::pow(this->time2d_ - this->time1d_, 2) * 0.5;
    this->dist_df_ = vel_d2 * (this->timefd_ - this->time2d_) + this->acc_md_ * std::pow(this->timefd_ - this->time2d_, 2) * 0.5 + 
    this->jark_fd_ * std::pow(this->timefd_ - this->time2d_, 3) / 6;

    this->dist_im_ = dist - (this->dist_a1_ + this->dist_a2_ + this->dist_af_) - (this->dist_d1_ + this->dist_d2_ + this->dist_df_);
}

void TrapeAcc::calVelAcc(const float dist, const float period, const float vel, const float acc,  float& vel_next, float& acc_next){
    this->t_ += period;
    if (std::abs(dist) <= std::abs(this->dist_df_)){
        acc_next = (acc + this->jark_fd_ * period);
        vel_next = (vel + acc * period);
    }
    else if (std::abs(dist) <= std::abs(this->dist_d2_ + this->dist_df_)){
        acc_next = this->acc_md_;
        vel_next = (vel + acc * period);
    }
    else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_)){
    // else if (this->t_ > this->time1d_){
        acc_next = (acc + this->jark_1d_ * period);
        vel_next = (vel + acc * period);
    }
    // constant velocity
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_)){
    else if (this->t_ > this->timefa_){
        acc_next = static_cast<float>(0.0);
        vel_next = this->vel_m_;
    }
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ && this->t_ >= this->time2a_)){
    else if (this->t_ > this->time2a_){
        acc_next = (acc + this->jark_fa_ * period);
        vel_next = (vel + acc * period);
    }
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ + this->dist_a2_)){
    else if (this->t_ > this->time1a_){
        acc_next = this->acc_ma_;
        vel_next = (vel + acc * period);
    }
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ + this->dist_a2_ + this->dist_a1_ + 0.1)){
    else {
        acc_next = (acc + this->jark_1a_ * period);
        vel_next = (vel + acc * period);
    }
}

int main(){
    TrapeAcc pro;
    float dist = 100.0;
    float acc_s = 15.0;
    float vel_s = 15.0;
    float acc_m = 10.0;
    float vel_m = 20.0;
    float jark = 30.0;
    float period = 0.01;
    float vel = vel_s;
    float acc = acc_s;
    float vel_n;
    float acc_n;

    std::ofstream ofs("./out.csv");
    if (!ofs){
        std::cout << "ファイルが開けませんでした。" << std::endl;
        std::cin.get();
        return 0;
    }
    
    int i = 0;
    ofs << "dist, " << "vel, " << "acc" << std::endl;
    pro.reset(dist, acc_s, vel_s, acc_m, vel_m, jark);
    while (std::abs(dist) > 0.0){
        pro.calVelAcc(dist, period, vel, acc, vel_n, acc_n);
        if (dist * (dist - vel*period) < 0.0){
            break;
        }
        dist -= vel*period;
        vel = vel_n;
        acc = acc_n;
        i++;
        
        ofs << i * period << ", ";
        ofs << dist << ", ";
        ofs << vel << ", ";
        ofs << acc <<std::endl;
    }

    std::cout << dist << ", " << vel_n << ", " << acc_n << std::endl;
    return 1;
}