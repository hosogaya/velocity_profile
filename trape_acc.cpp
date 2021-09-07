#include "trape_acc.h"
#include <fstream>
#include <iostream>
#include <array>
void TrapeAcc::reset(const double dist, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark){
    // this->calParame(dist, acc_s, vel_s, acc_m, vel_m, jark);
    // while(std::abs(dist) < std::abs(this->dist_a1_ + this->dist_a2_ + this->dist_af_ + this->dist_d1_ + this->dist_d2_ + this->dist_df_)){
    //     this->calParame(dist, acc_s, vel_s, acc_m, this->vel_m_ * 0.8, jark);
    // }
    double vel_mm = std::abs(vel_m) * this->sign(dist);
    this->calParame2(this->accel_, acc_s, vel_s, acc_m, vel_mm, jark);
    this->calParame2(this->decel_, 0.0, vel_mm, acc_m, 0.0, jark);
    while(std::abs(dist) < std::abs(accel_.dist_1 + accel_.dist_2 + accel_.dist_f + decel_.dist_1 + decel_.dist_2 + decel_.dist_f)){
        vel_mm = vel_mm * 0.8;
        this->calParame2(this->accel_, acc_s, vel_s, acc_m, vel_mm, jark);
        this->calParame2(this->decel_, 0.0, vel_mm, acc_m, 0.0, jark);
    }
    this->t_ = 0.0;
    // this->print();
    this->print(accel_);
    this->print(decel_);
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

// pre calcurate vel_m
void TrapeAcc::calParame2(Params& param, const double acc_s, const double vel_s, const double acc_m, const double vel_m, const double jark){
    param.vel_m = vel_m;
    param.acc_m = std::abs(acc_m) * this->sign(param.vel_m - vel_s);
    param.jark_1 = std::abs(jark) * this->sign(param.acc_m - acc_s);
    param.jark_f = -std::abs(jark) * this->sign(param.acc_m);

    double delta_vel_1 = (pow(param.acc_m, 2) - pow(acc_s, 2)) / (2 * param.jark_1);
    double delta_vel_f = 3 * pow(param.acc_m, 2) / (2 * param.jark_f);
    bool is_acc_const = true;
    if (std::abs(delta_vel_1 + delta_vel_f) > std::abs(param.vel_m - vel_s)){
        param.acc_m = std::sqrt(std::abs((2 * param.jark_1 * param.jark_f) * (param.vel_m - vel_s + pow(acc_s, 2) / param.jark_1) / (3 * param.jark_1 + param.jark_f))) * this->sign(param.acc_m);
        is_acc_const = false;
    }
    param.time_1 = std::abs((param.acc_m - acc_s) / param.jark_1);
    param.time_f = std::abs((param.vel_m - vel_s - acc_s * param.time_1 - pow(param.acc_m - acc_s, 2) / (2 * param.jark_1) - pow(param.acc_m, 2) / (2 * param.jark_f)) / param.acc_m) + param.time_1;
    if (is_acc_const){
        param.time_2 = param.time_f - std::abs(param.acc_m / param.jark_f);
    }
    else{
        param.time_2 = param.time_1;
    }
    double vel_1 = delta_vel_1 + vel_s;
    double vel_2 = param.acc_m * (param.time_2 - param.time_1) + vel_1;

    param.dist_1 = acc_s * pow(param.time_1, 2) * 0.5 + param.jark_1 * pow(param.time_1, 3) / 6 + vel_s * param.time_1;
    param.dist_2 = param.acc_m * pow(param.time_2 - param.time_1, 2) * 0.5 + vel_1 * (param.time_2 - param.time_1);
    param.dist_f = param.acc_m * pow(param.time_f - param.time_2, 2) * 0.5 + param.jark_f *pow(param.time_f - param.time_2, 3) / 6 + vel_2 * (param.time_f - param.time_2);
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

bool TrapeAcc::calVelAcc2(const double dist, const double period, const double vel, const double acc,  double& vel_next, double& acc_next){
    if (state_ == Acc){
        this->t_ += period;
        if (t_ <= accel_.time_f) this->update(accel_, t_, period, acc, vel, acc_next, vel_next);
        else {
            state_ = Const;
            t_ = 0.0;
            std::cout << "Switch to const phase" << std::endl;
        }
    }
    if (state_ == Const){
        if (std::abs(dist) > std::abs(decel_.dist_1 + decel_.dist_2 + decel_.dist_f)){
            vel_next = accel_.vel_m;
            acc_next = 0.0;
        }
        else{
            state_ = Dec;
            t_ = 0.0;
            std::cout << "Switch to Deceleration phase" <<std::endl;
        }
    }
    if (state_ == Dec){
        this->t_ += period;
        if (t_ <= decel_.time_f) this->update(decel_, t_, period, acc, vel, acc_next, vel_next);
        else {
            acc_next = 0.0;
            vel_next = 0.0;
            return false;
        }
    }
    return true;
}

void TrapeAcc::update(Params& param, double t, double period, double acc, double vel, double& acc_next, double& vel_next){
    if (t > param.time_2){
        acc_next = acc + param.jark_f * period;
        vel_next = vel + acc * period + param.jark_f * pow(period, 2.0) * 0.5;
    }
    else if (t > param.time_1){
        acc_next = param.acc_m;
        vel_next = vel + acc * period;
    }
    else {
        acc_next = acc + param.jark_1 * period;
        vel_next = vel + acc * period + param.jark_1 * pow(period, 2.0) * 0.5;
    }
}
/**
 * @brief create acceleration profile to goal velocity regardless of the distance
 * @param jark : constant jark
 * @param acc_s : current acceleration
 * @param vel_s : current velocity
 * @param acc_m : maximum amplitude of acceleration
 * @param vel_m : goal velocity
*/
void TrapeAcc::Decel(const double jark, const double acc_s, const double vel_s, const double acc_m, const double vel_m){
    state_ = Dec;
    t_ = 0.0;
    this->calParame2(decel_, acc_s, vel_s, acc_m, vel_m, jark);
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

void TrapeAcc::print(Params& param){
    std::cout << param.time_1 << ", " << param.time_2 << ", " << param.time_f << std::endl;
    std::cout << param.dist_1 << ", " << param.dist_2 << ", " << param.dist_f << std::endl;
    std::cout << param.jark_1 << ", " << param.jark_f << std::endl;
    std::cout << param.acc_m << ", " << param.vel_m << std::endl;
}

void globalRotation(std::array<double, 3>& src, const std::array<double, 3>& pose, std::array<double, 3>& dist){
    double temp = src[2] * cos(pose[0]) - src[0] * sin(pose[0]);
    dist[0] = src[0] * cos(pose[0]) + src[2] * sin(pose[0]);
    dist[2] = temp * cos(pose[1]) + src[1] * sin(pose[1]);
    dist[1] = src[1] * cos(pose[1]) - temp * sin(pose[1]);
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

    bool reset_flag = false;
    bool deceleration_flag = true;

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
        if (i > 800 and deceleration_flag){
            pro_x.Decel(jark, acc_x, vel_x, acc_m, 0.0);
            deceleration_flag = false;
        }
        if (break_flag_x == false) {
            if(pro_x.calVelAcc2(dist_x, period, vel_x, acc_x, vel_nx, acc_nx) == false) break_flag_x = true;
        }
        if (break_flag_y == false) {
            if(pro_y.calVelAcc2(dist_y, period, vel_y, acc_y, vel_ny, acc_ny) == false) break_flag_y = true;
        }
        ofs << i * period << ", ";
        ofs << dist_xi - dist_x << ", ";
        ofs << dist_yi - dist_y << ", ";
        ofs << vel_x << ", ";
        ofs << vel_y << ", ";
        ofs << acc_x << ", ";
        ofs << acc_y <<std::endl;
        if (break_flag_x and break_flag_y) break;
        // if (dist_x * (dist_x - vel_x*period - acc_x * pow(period, 2.0) * 0.5) <= 0.0){
        //     break_flag_x = true;
        // }
        // if (dist_y * (dist_y - vel_y*period - acc_y * pow(period, 2.0) * 0.5) <= 0.0){
        //     break_flag_y = true;
        // }
        
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