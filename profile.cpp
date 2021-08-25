#include "profile.h"
constexpr float pi_profile = atanf(1) * 4;
constexpr float _180pi_profile = 180 / (atanf(1) * 4);

Profile::Profile(){}

Profile::~Profile(){}


float Profile::calPhi(const float vel, const float max_vel){
    float norm_vel = std::abs(vel / max_vel);
    float phi = acosf(1 - 2 * norm_vel);
    if (std::abs(phi) * _180pi_profile < 0.5){
        return 0.0;
    }
    else if (std::abs(phi) * _180pi_profile > 179.5){
        return pi_profile;
    }
    else{
        return phi;
    }
}

float Profile::calTimeF(const float dist, const float phi, const float max_vel){
    return (2 * dist / max_vel) * (2 * pi_profile - phi) / (2 * pi_profile - phi + sinf(phi));
}
float Profile::calTime1(const float phi, const float vel, const float max_vel, const float max_acc){
    return (max_vel - vel) * (pi_profile - phi) / (max_acc * (1 + cosf(phi)));
}
float Profile::calTime2(const float max_vel, const float max_acc){
    return (max_vel * pi_profile) / (2 * max_acc);
}


float Profile::calAcc(const float dist, const float period, const float vel, const float max_vel, const float max_acc){
    float acc = 0.0;
    float phi =  this->calPhi(vel, max_vel);
    if (phi == pi_profile){
        float time2 = this->calTime2(max_vel, max_acc);
        float B = std::abs(max_vel * time2 / 2);
        if (B < dist){
            acc = 0.0;
        }
        else{
            this->speed_up_ = false;
            acc =  -0.1;
        }
    }
    else if (phi == 0.0){
        if (this->speed_up_){
            acc =  0.1;
        }
        else if (dist < 0.0){
            acc =  -0.1;
        }
    }
    else{
        if (this->speed_up_ == false){
            phi = 2 * pi_profile - phi;
            float timeF = this->calTimeF(dist, phi, max_vel);
            acc =  (max_vel / 2) * (2 * pi_profile - phi) * sinf(phi) / timeF;
        }
        else{
            float time1 = this->calTime1(phi, vel, max_vel, max_acc);
            float time2 = this->calTime2(max_vel, max_acc);
            float A = std::abs(max_vel * (time1 + sinf(phi)) * 0.5);
            float B = std::abs(max_vel * time2 * 0.5);

            // decelerate
            if (B - A >= dist){
                this->speed_up_ = false;
                acc =  -max_acc * sinf(phi);
            }
            // accelarate
            else if (A + B < dist){
                acc =  max_acc * sinf(phi);
            }
            else{
                acc = max_acc * sinf(phi);
                float next_vel = vel + acc * period;
                float next_time2 = this->calTime2(next_vel, max_acc);
                float next_B = std::abs(next_vel * next_time2 * 0.5);
                if (next_B >= dist){
                    acc =  0.0;
                }
            }
        }
    }
    return acc;
}

int main(){
    Profile pro;
    float dist = 100.0;
    float vel = 0.0;
    float max_vel = 30.0;
    float max_acc = 30.0;
    const float period = 0.1;
    int i = 0;
    
    while (dist > 1.0){
        float acc = pro.calAcc(dist, period, vel, max_vel, max_acc);
        vel = std::min(vel + acc * period, max_vel);
        dist -= vel * period;
        std::cout << i << ", " << dist << std::endl;
        i++;
    }

    return 1;
}