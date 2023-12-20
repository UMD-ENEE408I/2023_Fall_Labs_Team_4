#include "sd_state_machine.hpp"
#include <cstdlib>
#include <stdio.h>



Wander_State& Wander_State::instance() {
    static Wander_State __instance;
    return __instance;
}
        
Abstract_State<Input, Output>& Wander_State::get_next_state(const Input& input) {
    if(input.receive_packet.next_state == STOP_STATE_CODE) {
        return Stop_State::instance(); // stop if commanded to
    }
    else if(this->bonk_flag) {
        return Stop_State::instance(); // stop if bonk
    }
    else {
        return Wander_State::instance(); // continue if not done
    }
}

void Wander_State::entry_behavior(const Input& input, Output& output) {
    //output.screen.clearDisplay();
    //output.screen.drawString(0, 6, "State: Wander");
    this->bonk_flag = false;
}

void Wander_State::do_behavior(const Input& input, Output& output) {
    

    if (input.accel.acceleration.x < -35) {
        this->bonk_flag = true;
    }

    output.left_motor.set_pwm(110);
    output.right_motor.set_pwm(110);


    snprintf(output.send_packet.state_log, STATE_LOG_LEN, "Bonk: %d \n", 
            this->bonk_flag);
}