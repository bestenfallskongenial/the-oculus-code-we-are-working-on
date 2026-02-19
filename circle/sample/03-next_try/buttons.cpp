// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
we need a few additions here, alittle cleanup and the question what code is the best for my need, i tend for  b but maybe c, there should be a clearer state differentiation and also a click reset?!

*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum ButtonTSIndex
{
    BTN_PRESS_START = 0,
    BTN_DOUBLE      = 1,
    BTN_LONG        = 2,
    BTN_RELEASE     = 3,
    BTN_SINGLE      = 4,
    BTN_HOLD_TICK   = 5   // COUNTER: increases while held
};
// buttons_pressed_timestamps[2][6]


// version a
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::button_ping(int btn_id)
{
    buttons_states[0] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    buttons_states[1] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (buttons_states[btn_id] == ISPRESSED)
    {
        if (buttons_pressed_timestamps[btn_id][BTN_PRESS_START] == 0)
        {
            buttons_pressed_timestamps[btn_id][BTN_PRESS_START] = currentTime;

            if (buttons_pressed_timestamps[btn_id][BTN_RELEASE] > 0 &&
                (currentTime - buttons_pressed_timestamps[btn_id][BTN_RELEASE]) < dbl_clk_time)
            {
                buttons_pressed_timestamps[btn_id][BTN_DOUBLE]  = 1;
                buttons_pressed_timestamps[btn_id][BTN_RELEASE] = 0;
            }
        }

        unsigned held_time =
            currentTime - buttons_pressed_timestamps[btn_id][BTN_PRESS_START];

        if (held_time >= lng_clk_time)
        {
            if (buttons_pressed_timestamps[btn_id][BTN_LONG] == 0)
            {
                buttons_pressed_timestamps[btn_id][BTN_HOLD_NEXT] = currentTime + hold_tick_time;
                buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
            }

            buttons_pressed_timestamps[btn_id][BTN_LONG] = 1;

            // ---- COUNTER INCREMENT ----
            while (currentTime >= buttons_pressed_timestamps[btn_id][BTN_HOLD_NEXT])
            {
                buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK]++;   // COUNT
                buttons_pressed_timestamps[btn_id][BTN_HOLD_NEXT] += hold_tick_time;
            }
        }
    }
    else
    {
        if (buttons_pressed_timestamps[btn_id][BTN_PRESS_START] > 0)
        {
            buttons_pressed_timestamps[btn_id][BTN_RELEASE]     = currentTime;
            buttons_pressed_timestamps[btn_id][BTN_PRESS_START] = 0;
        }

        buttons_pressed_timestamps[btn_id][BTN_LONG]      = 0;
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;   // reset counter
        buttons_pressed_timestamps[btn_id][BTN_HOLD_NEXT] = 0;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_RELEASE] > 0 &&
        (currentTime - buttons_pressed_timestamps[btn_id][BTN_RELEASE]) >= dbl_clk_time &&
        buttons_pressed_timestamps[btn_id][BTN_DOUBLE] == 0)
    {
        buttons_pressed_timestamps[btn_id][BTN_SINGLE]  = 1;
        buttons_pressed_timestamps[btn_id][BTN_RELEASE] = 0;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// version b increment the is hold counter each loop

void CKernel::button_ping(int btn_id)
{
    buttons_states[0] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    buttons_states[1] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (buttons_states[btn_id] == ISPRESSED)
    {
        if (buttons_pressed_timestamps[btn_id][BTN_PRESS_START] == 0)
        {
            buttons_pressed_timestamps[btn_id][BTN_PRESS_START] = currentTime;

            if (buttons_pressed_timestamps[btn_id][BTN_RELEASE] > 0 &&
                (currentTime - buttons_pressed_timestamps[btn_id][BTN_RELEASE]) < dbl_clk_time)
            {
                buttons_pressed_timestamps[btn_id][BTN_DOUBLE]  = 1;
                buttons_pressed_timestamps[btn_id][BTN_RELEASE] = 0;
            }
        }

        if ((currentTime - buttons_pressed_timestamps[btn_id][BTN_PRESS_START]) >= lng_clk_time)
        {
            buttons_pressed_timestamps[btn_id][BTN_LONG] = 1;
            buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK]++; // increase while held
        }
    }
    else
    {
        if (buttons_pressed_timestamps[btn_id][BTN_PRESS_START] > 0)
        {
            buttons_pressed_timestamps[btn_id][BTN_RELEASE]     = currentTime;
            buttons_pressed_timestamps[btn_id][BTN_PRESS_START] = 0;
        }

        buttons_pressed_timestamps[btn_id][BTN_LONG]      = 0;
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_RELEASE] > 0 &&
        (currentTime - buttons_pressed_timestamps[btn_id][BTN_RELEASE]) >= dbl_clk_time &&
        buttons_pressed_timestamps[btn_id][BTN_DOUBLE] == 0)
    {
        buttons_pressed_timestamps[btn_id][BTN_SINGLE]  = 1;
        buttons_pressed_timestamps[btn_id][BTN_RELEASE] = 0;
    }
}


// version 3 shall fix an issue i cant see after extreme scrutiny
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::button_ping(int btn_id)
{
    buttons_states[0] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    buttons_states[1] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (buttons_states[btn_id] == ISPRESSED)
    {
        if (buttons_pressed_timestamps[btn_id][BTN_PRESS_START] == 0)
        {
            buttons_pressed_timestamps[btn_id][BTN_PRESS_START] = currentTime;

            if (buttons_pressed_timestamps[btn_id][BTN_RELEASE] > 0 &&
                (currentTime - buttons_pressed_timestamps[btn_id][BTN_RELEASE]) < dbl_clk_time)
            {
                buttons_pressed_timestamps[btn_id][BTN_DOUBLE]  = 1;
                buttons_pressed_timestamps[btn_id][BTN_RELEASE] = 0;
            }
        }

        if ((currentTime - buttons_pressed_timestamps[btn_id][BTN_PRESS_START]) >= lng_clk_time)
        {
            if (buttons_pressed_timestamps[btn_id][BTN_LONG] == 0)
            {
                buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
            }

            buttons_pressed_timestamps[btn_id][BTN_LONG] = 1;
            buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK]++;
        }
    }
    else
    {
        if (buttons_pressed_timestamps[btn_id][BTN_PRESS_START] > 0)
        {
            buttons_pressed_timestamps[btn_id][BTN_RELEASE]     = currentTime;
            buttons_pressed_timestamps[btn_id][BTN_PRESS_START] = 0;
        }

        buttons_pressed_timestamps[btn_id][BTN_LONG]      = 0;
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_RELEASE] > 0 &&
        (currentTime - buttons_pressed_timestamps[btn_id][BTN_RELEASE]) >= dbl_clk_time &&
        buttons_pressed_timestamps[btn_id][BTN_DOUBLE] == 0)
    {
        buttons_pressed_timestamps[btn_id][BTN_SINGLE]  = 1;
        buttons_pressed_timestamps[btn_id][BTN_RELEASE] = 0;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// consume button version a

void CKernel::consume_button(int btn_id, int &var)
{
    if (buttons_pressed_timestamps[btn_id][BTN_DOUBLE])
    {
        var = 0;
        buttons_pressed_timestamps[btn_id][BTN_DOUBLE]    = 0;
        buttons_pressed_timestamps[btn_id][BTN_SINGLE]    = 0;
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
        return;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_SINGLE])
    {
        ++var;
        buttons_pressed_timestamps[btn_id][BTN_SINGLE] = 0;
        return;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] > 0)
    {
        var -= buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK];
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0; // consume count
    }
}

// version b

void CKernel::consume_button(int btn_id, int &var)
{
    if (buttons_pressed_timestamps[btn_id][BTN_DOUBLE])
    {
        var = 0;
        buttons_pressed_timestamps[btn_id][BTN_DOUBLE]    = 0;
        buttons_pressed_timestamps[btn_id][BTN_SINGLE]    = 0;
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
        return;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_SINGLE])
    {
        ++var;
        buttons_pressed_timestamps[btn_id][BTN_SINGLE] = 0;
        return;
    }

    if (buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] > 0)
    {
        var -= buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK];
        buttons_pressed_timestamps[btn_id][BTN_HOLD_TICK] = 0;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------