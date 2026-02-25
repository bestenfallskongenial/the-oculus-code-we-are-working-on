// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
we need a few additions here, alittle cleanup and the question what code is the best for my need, i tend for  b but maybe c, there should be a clearer state differentiation and also a click reset?!

*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define BTN_PRESSED 0 // correct??
/*
enum ButtonTSIndex
{
    BTN_STATUS      = 0,
    BTN_PRESS_START = 1,
    BTN_DOUBLE      = 2,
    BTN_LONG        = 3,
    BTN_RELEASE     = 4,
    BTN_SINGLE      = 5,
    BTN_HOLD_TICK   = 6   // COUNTER: increases while held
};
*/
// g_buttons_states[2][7]


// version a
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// g_ for global-variable       g_buttons_states[2][7], g_long_click_time*, g_double_click_time*            * also possible macro definition? 
// p_for signature-parameter    p_btn_id
// f_ for function-variable     f_held_time, 
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::button_ping(int p_btn_id)
{
    g_buttons_states[0][BTN_STATUS] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    g_buttons_states[1][BTN_STATUS] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (g_buttons_states[p_btn_id] == BTN_PRESSED)
    {
        if (g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
        {
            g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;

            if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
                (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
            {
                g_buttons_states[p_btn_id][BTN_DOUBLE]  = 1;
                g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
            }
        }

        unsigned f_held_time =
            g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START];

        if (f_held_time >= g_long_click_time)
        {
            if (g_buttons_states[p_btn_id][BTN_LONG] == 0)
            {
                g_buttons_states[p_btn_id][BTN_HOLD_NEXT] = g_currentTime + f_hold_tick_time;
                g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
            }

            g_buttons_states[p_btn_id][BTN_LONG] = 1;

            // ---- COUNTER INCREMENT ----
            while (g_currentTime >= g_buttons_states[p_btn_id][BTN_HOLD_NEXT])
            {
                g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;   // COUNT
                g_buttons_states[p_btn_id][BTN_HOLD_NEXT] += f_hold_tick_time;
            }
        }
    }
    else
    {
        if (g_buttons_states[p_btn_id][BTN_PRESS_START] > 0)
        {
            g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
            g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
        }

        g_buttons_states[p_btn_id][BTN_LONG]      = 0;
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;   // reset counter
        g_buttons_states[p_btn_id][BTN_HOLD_NEXT] = 0;
    }

    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
        (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) >= g_double_click_time &&
        g_buttons_states[p_btn_id][BTN_DOUBLE] == 0)
    {
        g_buttons_states[p_btn_id][BTN_SINGLE]  = 1;
        g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// DO WE HAVE AN INDICATOR / TIMESTAMP FOR THE LAST SIGLE PRESS EVENT HERE?!?! IMPORTANT FOR OUR TAB-BPM MECHANISM!!
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// version b increment the is hold counter each loop

void CKernel::button_ping(int p_btn_id)
{
    g_buttons_states[0][BTN_STATUS] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    g_buttons_states[1][BTN_STATUS] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (g_buttons_states[p_btn_id] == BTN_PRESSED)
    {
        if (g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
        {
            g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;

            if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
                (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
            {
                g_buttons_states[p_btn_id][BTN_DOUBLE]  = 1;
                g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
            }
        }

        if ((g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
        {
            g_buttons_states[p_btn_id][BTN_LONG] = 1;
            g_buttons_states[p_btn_id][BTN_HOLD_TICK]++; // increase while held
        }
    }
    else
    {
        if (g_buttons_states[p_btn_id][BTN_PRESS_START] > 0)
        {
            g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
            g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
        }

        g_buttons_states[p_btn_id][BTN_LONG]      = 0;
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
    }

    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
        (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) >= g_double_click_time &&
        g_buttons_states[p_btn_id][BTN_DOUBLE] == 0)
    {
        g_buttons_states[p_btn_id][BTN_SINGLE]  = 1;
        g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
    }
}


// version 3 shall fix an issue i cant see after extreme scrutiny
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::button_ping(int p_btn_id)
{
    g_buttons_states[0][BTN_STATUS] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    g_buttons_states[1][BTN_STATUS] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (g_buttons_states[p_btn_id] == BTN_PRESSED)
    {
        if (g_buttons_states[p_btn_id][BTN_PRESS_START] == 0)
        {
            g_buttons_states[p_btn_id][BTN_PRESS_START] = g_currentTime;

            if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
                (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) < g_double_click_time)
            {
                g_buttons_states[p_btn_id][BTN_DOUBLE]  = 1;
                g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
            }
        }

        if ((g_currentTime - g_buttons_states[p_btn_id][BTN_PRESS_START]) >= g_long_click_time)
        {
            if (g_buttons_states[p_btn_id][BTN_LONG] == 0)
            {
                g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
            }

            g_buttons_states[p_btn_id][BTN_LONG] = 1;
            g_buttons_states[p_btn_id][BTN_HOLD_TICK]++;
        }
    }
    else
    {
        if (g_buttons_states[p_btn_id][BTN_PRESS_START] > 0)
        {
            g_buttons_states[p_btn_id][BTN_RELEASE]     = g_currentTime;
            g_buttons_states[p_btn_id][BTN_PRESS_START] = 0;
        }

        g_buttons_states[p_btn_id][BTN_LONG]      = 0;
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
    }

    if (g_buttons_states[p_btn_id][BTN_RELEASE] > 0 &&
        (g_currentTime - g_buttons_states[p_btn_id][BTN_RELEASE]) >= g_double_click_time &&
        g_buttons_states[p_btn_id][BTN_DOUBLE] == 0)
    {
        g_buttons_states[p_btn_id][BTN_SINGLE]  = 1;
        g_buttons_states[p_btn_id][BTN_RELEASE] = 0;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// consume button version a

void CKernel::consume_button(int p_btn_id, int &var)
{
    if (g_buttons_states[p_btn_id][BTN_DOUBLE])
    {
        var = 0;
        g_buttons_states[p_btn_id][BTN_DOUBLE]    = 0;
        g_buttons_states[p_btn_id][BTN_SINGLE]    = 0;
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
        return;
    }

    if (g_buttons_states[p_btn_id][BTN_SINGLE])
    {
        ++var;
        g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
        return;
    }

    if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] > 0)
    {
        var -= g_buttons_states[p_btn_id][BTN_HOLD_TICK];
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0; // consume count
    }
}

// version b

void CKernel::consume_button(int p_btn_id, int &var)
{
    if (g_buttons_states[p_btn_id][BTN_DOUBLE])
    {
        var = 0;
        g_buttons_states[p_btn_id][BTN_DOUBLE]    = 0;
        g_buttons_states[p_btn_id][BTN_SINGLE]    = 0;
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
        return;
    }

    if (g_buttons_states[p_btn_id][BTN_SINGLE])
    {
        ++var;
        g_buttons_states[p_btn_id][BTN_SINGLE] = 0;
        return;
    }

    if (g_buttons_states[p_btn_id][BTN_HOLD_TICK] > 0)
    {
        var -= g_buttons_states[p_btn_id][BTN_HOLD_TICK];
        g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0;
    }
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------