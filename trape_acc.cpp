#include "trape_acc.h"
#include <fstream>
#include <iostream>
void TrapeAcc::reset(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark){
    this->calParame(dist, acc_s, vel_s, acc_m, vel_m, jark);
    while(std::abs(dist) < std::abs(this->dist_a1_ + this->dist_a2_ + this->dist_af_ + this->dist_d1_ + this->dist_d2_ + this->dist_df_)){
        this->calParame(dist, acc_s, vel_s, acc_m, this->vel_m_ * 0.8, jark);
    }
    this->print();
}

void TrapeAcc::calParame(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark){
    this->vel_m_ = std::abs(vel_m) * this->sign(dist);
    this->acc_ma_ = std::abs(acc_m) * this->sign(this->vel_m_ - vel_s);
    this->acc_md_ = -std::abs(acc_m) * this->sign(this->vel_m_);
    
    this->jark_1a_ = std::abs(jark) * this->sign(this->acc_ma_ - acc_s);
    this->jark_fa_ = -std::abs(jark) * this->sign(this->acc_ma_);
    this->jark_1d_ = std::abs(jark) * this->sign(this->acc_md_);
    this->jark_fd_ = -std::abs(jark) * this->sign(this->acc_md_);
    this->t_ = 0.0;

    double delta_vel_a1 = (pow(this->acc_ma_, 2) - pow(acc_s, 2)) / (2 * this->jark_1a_);
    double delta_vel_af = 3 * pow(this->acc_ma_, 2) / (2 * this->jark_fa_);

    bool is_acc_const = true;
    if (std::abs(delta_vel_a1 + delta_vel_af) > std::abs(this->vel_m_ - vel_s)){
        this->acc_ma_ = std::sqrt(std::abs((2 * this->jark_1a_ *  this->jark_fa_) * (this->vel_m_ - vel_s + pow(acc_s, 2) / this->jark_1a_) / (3 * this->jark_1a_ + this->jark_fa_))) * this->sign(this->acc_ma_);
        is_acc_const = false;
    }
    // parameters while acceleration    
    this->time1a_ = std::abs((this->acc_ma_ - acc_s) / this->jark_1a_);
    this->timefa_ = std::abs((this->vel_m_ - vel_s - acc_s * this->time1a_ - pow(this->acc_ma_ - acc_s, 2) / (2 * this->jark_1a_) - pow(this->acc_ma_, 2) / (2 * this->jark_fa_)) / this->acc_ma_) + this->time1a_;
    // this->timefa_ = std::abs((this->vel_m_) / this->acc_ma_) + this->time1a_;
    if (is_acc_const){
        this->time2a_ = this->timefa_ - std::abs(this->acc_ma_ / this->jark_fa_);
    }else{
        this->time2a_ = this->time1a_;
    }

    double vel_a1 = delta_vel_a1 + vel_s;
    double vel_a2 = this->acc_ma_ * (this->time2a_ - this->time1a_) + vel_a1;
    
    this->dist_a1_ = acc_s * pow(this->time1a_, 2) * 0.5 + this->jark_1a_ * pow(this->time1a_, 3) / 6 + vel_s * this->time1a_;
    this->dist_a2_ = this->acc_ma_ * pow(this->time2a_ - this->time1a_, 2) * 0.5 + vel_a1 * (this->time2a_ - this->time1a_);
    this->dist_af_ = this->acc_ma_ * pow(this->timefa_ - this->time2a_, 2) * 0.5 + this->jark_fa_ * pow(this->timefa_ - this->time2a_, 3) / 6 + vel_a2 * (this->timefa_ - this->time2a_);


    is_acc_const = true;
    double delta_vel_d1 = pow(this->acc_md_, 2) / (2 * this->jark_1d_);
    double delta_vel_df = 3 * pow(this->acc_md_, 2) / (2 * this->jark_fd_);

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
    double vel_d1 = this->vel_m_ + this->jark_1d_ * pow(this->time1d_, 2) * 0.5;
    double vel_d2 = vel_d1 + this->acc_md_ * (this->time2d_ - this->time1d_);
    // distance
    this->dist_d1_ = this->vel_m_ * this->time1d_ + this->jark_1d_ * pow(this->time1d_, 3) / 6;
    this->dist_d2_ = vel_d1 * (this->time2d_ - this->time1d_) +  this->acc_md_ * pow(this->time2d_ - this->time1d_, 2) * 0.5;
    this->dist_df_ = vel_d2 * (this->timefd_ - this->time2d_) + this->acc_md_ * pow(this->timefd_ - this->time2d_, 2) * 0.5 + 
    this->jark_fd_ * pow(this->timefd_ - this->time2d_, 3) / 6;

    this->dist_im_ = dist - (this->dist_a1_ + this->dist_a2_ + this->dist_af_) - (this->dist_d1_ + this->dist_d2_ + this->dist_df_);
}

bool TrapeAcc::calVelAcc(const double dist, const double period, const double vel, const double acc,  double& vel_next, double& acc_next){
    this->t_ += period;

    if (std::abs(dist) <= std::abs(this->dist_df_)){
        acc_next = (acc + this->jark_fd_ * period);
        vel_next = (vel + acc * period + this->jark_fd_ * pow(period, 2.0) * 0.5);
    }
    else if (std::abs(dist) <= std::abs(this->dist_d2_ + this->dist_df_)){
        acc_next = this->acc_md_;
        vel_next = (vel + acc * period);
    }
    else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_)){
    // else if (this->t_ > this->time1d_){
        acc_next = (acc + this->jark_1d_ * period);
        vel_next = (vel + acc * period + this->jark_1d_ * pow(period, 2.0) * 0.5);
    }
    // constant velocity
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_)){
    else if (this->t_ > this->timefa_){
        acc_next = static_cast<double>(0.0);
        vel_next = this->vel_m_;
    }
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ && this->t_ >= this->time2a_)){
    else if (this->t_ > this->time2a_){
        acc_next = (acc + this->jark_fa_ * period);
        vel_next = (vel + acc * period + this->jark_fa_ * pow(period, 2.0) * 0.5);
    }
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ + this->dist_a2_)){
    else if (this->t_ > this->time1a_){
        acc_next = this->acc_ma_;
        vel_next = (vel + acc * period);
    }
    // else if (std::abs(dist) <= std::abs(this->dist_d1_ + this->dist_d2_ + this->dist_df_ + this->dist_im_ + this->dist_af_ + this->dist_a2_ + this->dist_a1_ + 0.1)){
    else {
        acc_next = (acc + this->jark_1a_ * period);
        vel_next = (vel + acc * period + this->jark_1a_ * pow(period, 2.0) * 0.5);
    }
    return true;
}

void TrapeAcc::print(){
    std::cout << this->time1a_ << ", " << this->time2a_ << ", " << this->timefa_ << std::endl;
    std::cout << this->time1d_ << ", " << this->time2d_ << ", " << this->timefd_ << std::endl;
    std::cout << this->dist_a1_ << ", " << this->dist_a2_ << ", " << this->dist_af_ << std::endl;
    std::cout << this->dist_d1_ << ", " << this->dist_d2_ << ", " << this->dist_df_ << std::endl;
    std::cout << this->dist_im_ << std::endl;
    std::cout << this->vel_m_ << std::endl;
    std::cout << this->acc_ma_  << ", " << this->acc_md_ << std::endl;
}

void globalRotation(std::array<double, 3>& src, const std::array<double, 3>& pose, std::array<double, 3>& dist){
    double temp = src[2] * cos(pose[0]) - src[0] * sin(pose[0]);
    dist[0] = src[0] * cos(pose[0]) + src[2] * sin(pose[0]);
    dist[2] = temp * cos(pose[1]) + src[1] * sin(pose[1]);
    dist[1] = src * cos(pose[1]) - temp * sin(pose[1]);
}


int main(){
    TrapeAcc pro_x;
    TrapeAcc pro_y;
    double dist_xi = 100.0;
    double dist_yi = 200.0;
    double dist_x = dist_xi;
    double dist_y = dist_yi;
    double acc_s = 0.0;
    double vel_s = 0.0;
    double acc_m = 60.0;
    double vel_m = 80.0;
    double jark = 500.0;
    double period = 0.001;
    double vel_x = vel_s;
    double vel_y = vel_s;
    double acc_x = acc_s;
    double acc_y = acc_s;
    double vel_nx;
    double vel_ny;
    double acc_nx;
    double acc_ny;
    bool break_flag_x = false;
    bool break_flag_y = false;

    std::ofstream ofs("./out.csv");
    if (!ofs){
        std::cout << "ファイルが開けませんでした。" << std::endl;
        std::cin.get();
        return 0;
    }
    
    int i = 0;
    ofs << "dist_x, " << "dist_y, "<< "vel_x, " << "vel_y, " << "acc_x, " << "acc_y" << std::endl;
    pro_x.reset(dist_x, acc_s, vel_s, acc_m, vel_m, jark);
    pro_y.reset(dist_y, acc_s, vel_s, acc_m, vel_m, jark);
    while (true){
        if (break_flag_x == false) pro_x.calVelAcc(dist_x, period, vel_x, acc_x, vel_nx, acc_nx);
        if (break_flag_y == false) pro_y.calVelAcc(dist_y, period, vel_y, acc_y, vel_ny, acc_ny);
        ofs << i * period << ", ";
        ofs << dist_xi - dist_x << ", ";
        ofs << dist_yi - dist_y << ", ";
        ofs << vel_x << ", ";
        ofs << vel_y << ", ";
        ofs << acc_x << ", ";
        ofs << acc_y <<std::endl;
        if (break_flag_x and break_flag_y) break;
        if (dist_x * (dist_x - vel_x*period - acc_x * pow(period, 2.0) * 0.5) <= 0.0){
            break_flag_x = true;
        }
        if (dist_y * (dist_y - vel_y*period - acc_y * pow(period, 2.0) * 0.5) <= 0.0){
            break_flag_y = true;
        }
        
        dist_x = dist_x - vel_x * period - acc_x * pow(period, 2.0) * 0.5;
        dist_y = dist_y - vel_y * period - acc_y * pow(period, 2.0) * 0.5;
        vel_x = vel_nx;
        vel_y = vel_ny;
        acc_x = acc_nx;
        acc_y = acc_ny;
        i++;
    }

    // std::cout << dist << ", " << vel_n << ", " << acc_n << std::endl;
    return 1;
}