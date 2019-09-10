//===========================================================================
//  Copyright(c) Alorium Technology Group Inc., 2018
//  ALL RIGHTS RESERVED
//===========================================================================
//
// File name:  : pll16.vh
// Author      : Steve Phillips
// Contact     : support@aloriumtech.com
// Description : 
//
// This file is used to define the PLL clock parameters for the CLK2
// and CLK4 ouputs of the PLL, which then can be used in the OpenXLR8
// module if clocks other than the standard 16,32 or 64 MHz clocks are
// needed.
//
// See the following guide for detailed instructions on configuring the PLL:
//
// https://www.altera.com/content/dam/altera-www/global/en_US/pdfs/literature/ug/ug_altpll.pdf
//
// The directives as listed in the original version of this file from
// GitHub will set CLK2 to a 64MHz clock with a 45 degree phase shift
// and will set CLK4 to a 32MHz clock with a 22.5 degree phase shift.
//
// The Reference clock for the PLL is 16MHz. To generate a different
// clock, set the MULTIPLY and DIVIDE parameters to generate the clock
// desired, as calculated by this formula:
//
//    Desired Clock Rate = 16MHz * (MULTIPLY/DIVIDE)
//
// To shift the clock from a phase shift of zero to a new phase shift,
// specify a value, in picoseconds, of the desired phase shiftin the
// PHASE_SHIFT parameter. To calculate a phase shift in picoseconds
// based on a desired phase shift in degrees, use the following
// formula:
//
//    Let: DCR = Desired Clock Rate
//         DSD = Desired Shift in Degrees
//        
//                    10**12       DSD
//    PHASE_SHIFT =  --------  *  -----
//                      DCR        360
//
// EXAMPLE: Generate a 256MHz clock with a 45% phase shift
// 
//    Desired Clock Rate = 256MHz = 16MHz * 16
//                                = 16MHz * (16/1)
//
//       Therefore: MULTIPLY = 16, DIVIDE   =  1
//
//
//                    10**12       DSD
//    PHASE_SHIFT =  --------  *  -----
//                      DCR        360
//
//                     10**12          45
//                =  -----------  *  -----
//                   256 * 10**6      360
//
//                    10**6   
//                =  ------- * 0.125
//                     256
//
//                =  488 ps
// 
//===========================================================================


// Create the clock called "clk_option2"
// The following creates a 64MHz clock with a 45 degree phase shift
localparam XLR8_PLL_CLK2_DIVIDE_BY = 1;
localparam XLR8_PLL_CLK2_DUTY_CYCLE = 50;
localparam XLR8_PLL_CLK2_MULTIPLY_BY = 4;
localparam XLR8_PLL_CLK2_PHASE_SHIFT = "1953";

// Create the clock called "clk_option4"
// The following creates a 32MHz clock with a 22.5 degree phase shift
localparam XLR8_PLL_CLK4_DIVIDE_BY = 1;
localparam XLR8_PLL_CLK4_DUTY_CYCLE = 50;
localparam XLR8_PLL_CLK4_MULTIPLY_BY = 2;
localparam XLR8_PLL_CLK4_PHASE_SHIFT = "1953";


