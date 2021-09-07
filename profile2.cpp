#include "profile2.h"
#include <complex>
#include <math.h>
#include <iostream>
#include <fstream>
void TrapeAcc::reset(const float dist, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark){
    float vel_mm = std::abs(vel_m) * this->sign(dist);
    this->calParame(this->accel_, acc_s, vel_s, acc_m, vel_mm, jark);
    this->calParame(this->decel_, 0.0, vel_mm, acc_m, 0.0, jark);
    while(std::abs(dist) < std::abs(accel_.dist_1 + accel_.dist_2 + accel_.dist_f + decel_.dist_1 + decel_.dist_2 + decel_.dist_f)){
        vel_mm = vel_mm * 0.8;
        this->calParame(this->accel_, acc_s, vel_s, acc_m, vel_mm, jark);
        this->calParame(this->decel_, 0.0, vel_mm, acc_m, 0.0, jark);
    }
    this->t_ = 0.0;
    this->print(accel_);
    this->print(decel_);
}

// pre calcurate vel_m
void TrapeAcc::calParame(Params& param, const float acc_s, const float vel_s, const float acc_m, const float vel_m, const float jark){
    param.vel_m = vel_m;
    param.acc_m = std::abs(acc_m) * this->sign(param.vel_m - vel_s);
    param.jark_1 = std::abs(jark) * this->sign(param.acc_m - acc_s);
    param.jark_f = -std::abs(jark) * this->sign(param.acc_m);

    float delta_vel_1 = (powf(param.acc_m, 2) - powf(acc_s, 2)) / (2 * param.jark_1);
    float delta_vel_f = 3 * powf(param.acc_m, 2) / (2 * param.jark_f);
    bool is_acc_const = true;
    if (std::abs(delta_vel_1 + delta_vel_f) > std::abs(param.vel_m - vel_s)){
        param.acc_m = std::sqrt(std::abs((2 * param.jark_1 * param.jark_f) * (param.vel_m - vel_s + powf(acc_s, 2) / param.jark_1) / (3 * param.jark_1 + param.jark_f))) * this->sign(param.acc_m);
        is_acc_const = false;
    }
    param.time_1 = std::abs((param.acc_m - acc_s) / param.jark_1);
    param.time_f = std::abs((param.vel_m - vel_s - acc_s * param.time_1 - powf(param.acc_m - acc_s, 2) / (2 * param.jark_1) - powf(param.acc_m, 2) / (2 * param.jark_f)) / param.acc_m) + param.time_1;
    if (is_acc_const){
        param.time_2 = param.time_f - std::abs(param.acc_m / param.jark_f);
    }
    else{
        param.time_2 = param.time_1;
    }
    float vel_1 = delta_vel_1 + vel_s;
    float vel_2 = param.acc_m * (param.time_2 - param.time_1) + vel_1;

    param.dist_1 = acc_s * powf(param.time_1, 2) * 0.5 + param.jark_1 * powf(param.time_1, 3) / 6 + vel_s * param.time_1;
    param.dist_2 = param.acc_m * powf(param.time_2 - param.time_1, 2) * 0.5 + vel_1 * (param.time_2 - param.time_1);
    param.dist_f = param.acc_m * powf(param.time_f - param.time_2, 2) * 0.5 + param.jark_f *powf(param.time_f - param.time_2, 3) / 6 + vel_2 * (param.time_f - param.time_2);
}

bool TrapeAcc::calVelAcc(const float dist, const float period, const float acc, const float vel,  float& acc_next, float& vel_next){
    if (state_ == Acc){
        this->t_ += period;
        if (t_ <= accel_.time_f) this->update(accel_, t_, period, acc, vel, acc_next, vel_next);
        else {
            state_ = Const;
            t_ = 0.0;
            // std::cout << "Switch to const phase" << std::endl;
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
            // std::cout << "Switch to Deceleration phase" <<std::endl;
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

void TrapeAcc::update(Params& param, float t, float period, float acc, float vel, float& acc_next, float& vel_next){
    if (t > param.time_2){
        acc_next = acc + param.jark_f * period;
        vel_next = vel + acc * period + param.jark_f * powf(period, 2.0) * 0.5;
    }
    else if (t > param.time_1){
        acc_next = param.acc_m;
        vel_next = vel + acc * period;
    }
    else {
        acc_next = acc + param.jark_1 * period;
        vel_next = vel + acc * period + param.jark_1 * powf(period, 2.0) * 0.5;
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
void TrapeAcc::Decel(const float jark, const float acc_s, const float vel_s, const float acc_m, const float vel_m){
    if (state_ != Dec) {
        state_ = Dec;
        t_ = 0.0;
        this->calParame(decel_, acc_s, vel_s, acc_m, vel_m, jark);
    }
}

// void TrapeAcc::print(){
//     this->print(accel_);
//     this->print(decel_);
// }


void TrapeAcc::print(Params& param){
    std::cout << param.time_1 << ", " << param.time_2 << ", " << param.time_f << std::endl;
    std::cout << param.dist_1 << ", " << param.dist_2 << ", " << param.dist_f << std::endl;
    std::cout << param.jark_1 << ", " << param.jark_f << std::endl;
    std::cout << param.acc_m << ", " << param.vel_m << std::endl;
}

int main(){
  float distance = 30.0;
  float jark = 1000;
  float max_acc = 90.0;
  float max_vel = 120.0;
  float acc = 0.0;
  float vel = 0.0;
  float acc_n = acc;
  float vel_n = vel;
  float period = 0.001;
  float mileage = 0.0;

  std::ofstream ofs("./out.csv");
  if (!ofs){
      std::cout << "Can't open the file." << std::endl;
      std::cin.get();
      return 0;
  }
  ofs << "dist," << "vel," << "acc" << std::endl;
  TrapeAcc profile;
  int i = 0;
  profile.reset(distance, acc, vel, max_acc, max_vel, jark);
  while (profile.calVelAcc(distance - mileage, period, acc, vel, acc_n, vel_n)){
    mileage += vel * period + acc * powf(period, 2) * 0.5;
    vel = vel_n;
    acc = acc_n;
    ofs << i * period << ", ";
    ofs << mileage << ", ";
    ofs << vel << ", ";
    ofs << acc <<std::endl;
    i++;
  }
  return 1;
}