#include "sd_state_machine.hpp"
#include <cstdlib>
#include <stdio.h>



Walk_State& Walk_State::instance() {
    static Walk_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Walk_State::get_next_state(const Input& input) {
    if(input.receive_packet.next_state == STOP_STATE_CODE) {
        return Stop_State::instance(); // stop if commanded to
    }
    else if(this->bonk_flag) {
        return Stop_State::instance(); // stop if finished
    }
    else {
        return Walk_State::instance(); // continue spinning if not done
    }
}

void Walk_State::entry_behavior(const Input& input, Output& output) {
    //output.screen.clearDisplay();
    //output.screen.drawString(0, 6, "State: Walk");
    float init_time = input.gyro.timestamp;
    this->bonk_flag = false;
    this->prev_timestamp = input.gyro.timestamp; 
    this->pitch = 0;
    this->w_y_bias = input.gyro.gyro.y;
}

void Walk_State::do_behavior(const Input& input, Output& output) {
    
    int dt = input.gyro.timestamp - this->prev_timestamp;
    this->pitch += (input.gyro.gyro.y-(this->w_y_bias)) * dt * 0.057296f;

    if (input.accel.acceleration.x < -55 || this->pitch < -15) {
        this->bonk_flag = true;
    }

    output.left_motor.set_pwm(110);
    output.right_motor.set_pwm(110);

    this->prev_timestamp = input.gyro.timestamp;

    snprintf(output.send_packet.state_log, STATE_LOG_LEN, "Bonk: %d \n", 
            this->bonk_flag);
}