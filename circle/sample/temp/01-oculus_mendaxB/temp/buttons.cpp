// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
we need a few additions here, alittle cleanup and the question what code is the best for my need, i tend for  b but maybe c, there should be a clearer status differentiation and also a click reset?!

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



// version 3 shall fix an issue i cant see after extreme scrutiny
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CKernel::button_ping(int p_btn_id)
{
    g_buttons_states[0][BTN_STATUS] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    g_buttons_states[1][BTN_STATUS] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    if (g_buttons_states[p_btn_id][BTN_STATUS] == BTN_PRESSED)
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

/*
#define BTN_PRESSED 0

enum ButtonTSIndex
{
    BTN_STATUS              = 0, // live GPIO state
    BTN_PRESS_ACTIVE        = 1, // 1 while currently pressed
    BTN_DOUBLE              = 2, // event flag
    BTN_LONG                = 3, // state flag
    BTN_RELEASE_LOOPS       = 4, // loops since release (0 = inactive)
    BTN_SINGLE              = 5, // event flag
    BTN_HOLD_TICK           = 6, // accumulated hold-repeat ticks
    BTN_HELD_LOOPS          = 7  // loops held in current press
};
// g_buttons_states[2][8]

// tune these in "loop units"
static const unsigned LONG_LOOPS   = 40; // e.g. 40 loop calls => long press
static const unsigned DOUBLE_LOOPS = 20; // e.g. 20 loop calls window for double

// version C: loop-based timing with cleaner long-press transition (no g_currentTime)
void CKernel::button_ping(int p_btn_id)
{
    // sample both buttons each call
    g_buttons_states[0][BTN_STATUS] = CGPIOPin(SW_PIN_A, GPIOModeInputPullUp).Read();
    g_buttons_states[1][BTN_STATUS] = CGPIOPin(SW_PIN_B, GPIOModeInputPullUp).Read();

    // advance "since release" counter while waiting for possible second click
    if (g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] > 0)
    {
        g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS]++;
    }

    if (g_buttons_states[p_btn_id][BTN_STATUS] == BTN_PRESSED)
    {
        // first loop of this press
        if (g_buttons_states[p_btn_id][BTN_PRESS_ACTIVE] == 0)
        {
            g_buttons_states[p_btn_id][BTN_PRESS_ACTIVE] = 1;
            g_buttons_states[p_btn_id][BTN_HELD_LOOPS]   = 0;

            // second click arrived inside window => double
            if (g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] > 0 &&
                g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] < DOUBLE_LOOPS)
            {
                g_buttons_states[p_btn_id][BTN_DOUBLE]        = 1;
                g_buttons_states[p_btn_id][BTN_SINGLE]        = 0;
                g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] = 0; // consume window
            }
        }

        // held one more loop
        g_buttons_states[p_btn_id][BTN_HELD_LOOPS]++;

        // long state
        if (g_buttons_states[p_btn_id][BTN_HELD_LOOPS] >= LONG_LOOPS)
        {
            if (g_buttons_states[p_btn_id][BTN_LONG] == 0)
            {
                g_buttons_states[p_btn_id][BTN_HOLD_TICK] = 0; // clean long-entry
            }

            g_buttons_states[p_btn_id][BTN_LONG] = 1;
            g_buttons_states[p_btn_id][BTN_HOLD_TICK]++; // repeat each loop while held
        }
    }
    else
    {
        // release edge
        if (g_buttons_states[p_btn_id][BTN_PRESS_ACTIVE] == 1)
        {
            g_buttons_states[p_btn_id][BTN_PRESS_ACTIVE]  = 0;
            g_buttons_states[p_btn_id][BTN_HELD_LOOPS]    = 0;
            g_buttons_states[p_btn_id][BTN_LONG]          = 0;
            g_buttons_states[p_btn_id][BTN_HOLD_TICK]     = 0;
            g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] = 1; // start click window
        }
    }

    // no second click arrived in time => single
    if (g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] >= DOUBLE_LOOPS &&
        g_buttons_states[p_btn_id][BTN_DOUBLE] == 0)
    {
        g_buttons_states[p_btn_id][BTN_SINGLE]        = 1;
        g_buttons_states[p_btn_id][BTN_RELEASE_LOOPS] = 0; // consume window
    }
}
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// okay i need a consumer that processes the output from the g_buttons_states menu.
// here the menu functionality:
// no button pressed means i am in menu_layer 0, i will keep track of the last of menu_layer 1 or 2 for my leds, indicating the last "group" i modified
// only single presses ( beneath the long / double click threshold ) will store a global timestamp variable g_for_my_bpm_function
// hold the upper button will enter the menu_layer 1, 
// hold the lower button will enter the menu_layer 2,
// hold the lower button and single press the upper button will enter the menu_layer 3, 
// than when i press the upper button again while still holding the lower button, and so on, until i release the lower button 
// there will be a global wrapp around variable for the menu_layer g_max_layers
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void consumer()
{
    if (g_buttons_states[0][BTN_SINGLE] == 1)
    {
        g_buttons_states[0][BTN_SINGLE] = 0; // consume event
        g_for_my_bpm_function = g_currentTime; // store timestamp for bpm function
    }

    if (g_buttons_states[1][BTN_SINGLE] == 1)
    {
        g_buttons_states[1][BTN_SINGLE] = 0;            // consume event
        g_for_my_bpm_function = g_currentTime;          // store timestamp for bpm function
    }

    if (g_buttons_states[0][BTN_LONG] == 1 && g_buttons_states[1][BTN_STATUS] == BTN_PRESSED)
    {
        
        g_buttons_states[0][BTN_SINGLE] = 0;            // consume single event to prevent re-entry until next single press        
        menu_layer = (menu_layer + 1) % g_max_layers;   // we inc menu_layer here, we wrapp around max_layers
    }
    else if (g_buttons_states[0][BTN_LONG] == 1)
    {
        menu_layer = 1;                                 // enter menu_layer 1 - what happens here when i consume the long event? that does button_ping right?
    }
    else if (g_buttons_states[1][BTN_LONG] == 1)
    {
        menu_layer = 2;                                 // enter menu_layer 2 - what happens here when i consume the long event? that does button_ping right?
    }
}