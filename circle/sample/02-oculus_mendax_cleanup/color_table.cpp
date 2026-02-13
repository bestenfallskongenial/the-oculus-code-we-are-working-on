#include "kernel.h"
#include "global.h"
// we need to add commentary here that we understand how the mapping is applied, 
// also because we will add more modes later on, alignment is paramount
const unsigned char rgb_tab[49][3] = {

// first four colors are mapped to the different attenuations for the adc mode, i know the math looks chaotic here
{ 0, 0, 0},
{48,32, 0},
{ 0, 0,48},
{ 0,48, 0},

{ 0, 0,48},
{ 0,16,48},
{ 0,24,48},

{56,40, 0},
{32,48,32},

{ 0,48,24},
{ 0,40,40},

{48,48, 0},
{ 8, 0,48},
{16, 0,48},
{24, 0,48},
{32, 0,48},
{48, 0, 0},
{ 0,42, 6},
{12, 0,48},
{36,48, 0},
{ 0,24,42},
{48, 6,36},
{ 6,48,12},
{42, 0,48},
{48,30, 0},
{ 0,48,36},
{36,12,48},
{48, 0,24},
{12,42, 0},
{ 0,18,48},
{48,36, 6},
{24, 0,42},
{ 6,48,30},
{42,12, 0},
{ 0,36,48},
{48, 0,36},
{18,42, 6},
{ 0,12,48},
{48,30, 0},
{ 6, 0,42},
{36,48,12},
{ 0,24,48},
{42, 6, 0},
{12,48,36},
{48, 0,42},
{ 0,36, 6},
{30,12,48},
{48,42, 0},
{ 6, 0,48}
};