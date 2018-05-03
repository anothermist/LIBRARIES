///////////////////////////////////////////////////////////////////
//=================================================================
//  Copyright (c) Alorium Technology 2016
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : openxlr8.v
// Author      : Steve Phillips
// Description : Wrapper module to hold instantiations of 
//               OpenXLR8 XB modules.
//
// This module is edited by the OpenXLR8 user to instantiate their
// XB(s) into the design. This module provides the input, output and
// control signals needed to connect the XB(s) into the logic in the
// top verilog module. Some wiring is required beyond simply
// instantiating the XB, especially in the case of multiple XBs, but
// the comments included here should explain what is needed.
//
// This file is organized in to several sections with helpful 
// comments in each. The sections are:
//
// 1.) Parameters
//     NUM_PINS should not be edited, but NUM_OXBS should be set 
//     to the number of XBs being instantiated in this file.
// 2.) Inputs and Outputs
//     No changes should be needed here. The inputs and outputs 
//     are defined.
// 3.) Regs and Wires
//     This section starts with some required wire definitions 
//     and then provides hints for addition regs and wires that 
//     may be needed, You'll going to need to add at least a 
//     few lines here.
// 4.) Instantiate XBs
//     This is where all the XBs should be specified by instantiating
//     the XB module and setting the xbs_* signals for that XB. The
//     basic template should be repeated for each XB being used. Don't
//     forget to set the NUM_OXBS value back in section 1 to match the
//     number of XBs!
// 5.) Combining logic
//     This section begins with some logic to cobine the xbs_* signals
//     into the xb_* signals needed for the outputs. Then there are
//     some hints and examples for how to write the logic to combine
//     the dbusout and out_en signals from the XBs.
// 6.) Interrupt logic 
//     This section instantiates the xlr8_pcint.v module if there are
//     interrupts needed for the OpenXLR8 implementation. The OpenXLR8
//     module has a single IRQ output that ties into the AVR interrupt
//     handler.
//
//=================================================================
///////////////////////////////////////////////////////////////////
`include "xb_adr_pack.vh"

module openxlr8
  //======================================================================
  // Normally the instantiation parameters and module I/O would be
  // listed inline here. To make the module more easily updated, that
  // code has been moved to an include file located in the XLR8Core. It
  // is inserted here at compile time.
`include "../../../XLR8Core/extras/rtl/openxlr8_module_io.vh"

  // Listed below is the instantiation parameters and module I/O of
  // the current version. It is listed only to provide an example of
  // what they should be. To see the actual code, see the file
  // included above.

  /* === Beginning of Sample Instantiation Parameters and Module I/O ===
  // ----------------------------------------------------------------------
  // 1.) Parameters
  
  #(
    parameter DESIGN_CONFIG = 8,
    //    {
    //     25'd0, // [31:14] - reserved
    //     8'h8,  // [13:6]  - MAX10 Size,  ex: 0x8 = M08, 0x32 = M50
    //     1'b0,  //   [5]   - ADC_SWIZZLE, 0 = XLR8,            1 = Sno
    //     1'b0,  //   [4]   - PLL Speed,   0 = 16MHz PLL,       1 = 50Mhz PLL
    //     1'b1,  //   [3]   - PMEM Size,   0 = 8K (Sim Kludge), 1 = 16K
    //     2'd0,  //  [2:1]  - Clock Speed, 0 = 16MHZ,           1 = 32MHz, 2 = 64MHz, 3=na
    //     1'b0   //   [0]   - FPGA Image,  0 = CFM Application, 1 = CFM Factory
    //     },
    
    parameter NUM_PINS = 20,// Default is Arduino Uno Digital 0-13 + Analog 0-5
    // NUM_PINS should be 20 for the XLR8 board, ?? for the Sno board
    
    parameter OX8ICR_Address = 8'h31,
    parameter OX8IFR_Address = 8'h32,
    parameter OX8MSK_Address = 8'h33
    // The OX8*_Address parameters are used to control the interrupt module
    
    )
   //----------------------------------------------------------------------
   
   //----------------------------------------------------------------------
   // 2.) Inputs and Outputs
   (
    // Clock and Reset
    // The clk input is the CPU core frequency, which could be 16, 32 or 64MHZ 
    // depending on how the image was built
    input                       clk, //       Clock
    input                       rstn, //      Reset 
    // These three clocks are always the stated frequency, regardless of the CPU 
    // core frequency
    input                       clk_64mhz, // 64MHz clock
    input                       clk_32mhz, // 32MHz clock
    input                       clk_16mhz, // 16MHz clock
    input                       clk_option2, // Default: 64MHz, 45 degrees phase
    input                       clk_option4, // Default: 32MHz, 22.5 degrees phase
    // These enables have one shot pulses at the stated intrevals 
    input                       en16mhz, //   Enable for 16MHz timer
    input                       en1mhz, //    Enable for   1MHz timer
    input                       en128khz, //  Enable for 128KHz timer
    // I/O 
    input [5:0]                 adr, //       Reg Address
    input [7:0]                 dbus_in, //   Data Bus Input
    output [7:0]                dbus_out, //  Data Bus Output
    output                      io_out_en, // IO Output Enable
    input                       iore, //      IO Reade Enable
    input                       iowe, //      IO Write Enable
    // DM
    input [7:0]                 ramadr, //    RAM Address
    input                       ramre, //     RAM Read Enable
    input                       ramwe, //     RAM Write Enable
    input                       dm_sel, //    DM Select
    input [7:0]                 dm_dout_rg,// dout held during cpuwait, for UART

    // Other
    input [255:0]               gprf, //      Direct RO access to Reg File
    input [NUM_PINS-1:0]        xb_pinx, //   pin inputs
    inout                       JT9, //       JTAG pin
    inout                       JT7, //       JTAG pin
    inout                       JT6, //       JTAG pin
    inout                       JT5, //       JTAG pin
    inout                       JT3, //       JTAG pin
    inout                       JT1, //       JTAG pin
    // For iomux
    output logic [NUM_PINS-1:0] xb_ddoe, //   override data direction
    output logic [NUM_PINS-1:0] xb_ddov, //   data direction value if 
                                         //     overridden (1=output)
    output logic [NUM_PINS-1:0] xb_pvoe, //   override output value
    output logic [NUM_PINS-1:0] xb_pvov, //    output value if overridden
    // Interrupts
    output logic                xb_irq //    To core
    );
   //----------------------------------------------------------------------
      === End of Sample Instantiation Parameters and Module I/O === */

   
   //----------------------------------------------------------------------
   // 3.) Params, Regs and Wires declarations
   //

  parameter NUM_OXBS  = 1; // !! EDIT THIS LINE !!
   // NUM_OXBS should equal the number of XBs being instantiated within
   // this module. However, in the case where no XB is being
   // instantiated, the value should be set to 1 rather than zero, so
   // that the logic compiles correctly and we can still provide the
   // correct output values. Called it NUM_OXBS for OpenXLR8 XBs, to 
   // differentiate is from the NUM_XBS parameter used in the top.
   
   
   // These are required:

   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_ddoe;
   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_ddov;
   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_pvoe;
   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_pvov;
   
   // Add additional wires and regs here as needed to connect your XBs
   // to the combining logic and to each other if needed. At minimum,
   // with a single XB, you'll need at least something like this:
   
   // logic [7:0]		xb1_dbusout;
   // logic 			xb1_out_en;

   localparam NUM_NEOPIXELS = 15; //  Digital 1-13, plus A0 and A1
   logic [NUM_NEOPIXELS:1]            neopixel_en;
   logic                              neopixel_out;
   logic [7:0]                        neopixel_dbus_out;
   logic                              neopixel_io_out_en;
   

   //----------------------------------------------------------------------
   // 4.) Instantiate XBs
   
   //----------------------------------------------------------------------
   // Instantiate XB module and configure control signals
   //

   xlr8_neopixel
     #(
       .NUM_NEOPIXELS (NUM_NEOPIXELS),
       .NEOCR_ADDR    (XB_NEOCR_ADDR), // neopixel control register
       .NEOD2_ADDR    (XB_NEOD2_ADDR), // neopixel data register 2
       .NEOD1_ADDR    (XB_NEOD1_ADDR), // neopixel data register 1
       .NEOD0_ADDR    (XB_NEOD0_ADDR) // neopixel data register 0
       )
   (
    .clk          (clk),
    .en16mhz      (en16mhz), // clock enable at 16MHz rate
    .rstn         (rstn),
    // Register access for registers in first 64
    .adr          (adr),
    .dbus_in      (dbus_in),
    .dbus_out     (neopixel_dbus_out),
    .iore         (iore),
    .iowe         (iowe),
    .io_out_en    (neopixel_io_out_en),
    // Register access for registers not in first 64
    .ramadr       (ramadr),
    .ramre        (ramre),
    .ramwe        (ramwe),
    .dm_sel       (dm_sel),
    // External inputs/outputs
    .neopixel_en  (neopixel_en[NUM_NEOPIXELS:1]),
    .neopixel_out (neopixel_out)
    );
   
   // Set pin control bits for the above XB. If no XBs are being
   // instantiated then leave these lines uncommented so values will
   // be zeros
   //
   // NOTE: The xbs_* assign statements are required for every XB
   // instantiated, even if it doesn't talk to the I/O. In that case
   // the xbs_* busses should be tied low, as shown below.

   // Here are some definitions of the signals and guidelines for 
   // definition:
   //
   // xbs_ddoe: This controls whether the xbs_ddov signal will be able
   // to control the I/O pin. Setting this to a one allows xbs_ddov to
   // control the pin direction.
   //
   // xbs_ddov: If the corresponding xbs_ddoe bit is set, then this
   // controls the direction of the I/O pin. Setting xbs_ddov to a one
   // will make the pin an output, setting it to a zero will make it
   // an input.
   //
   // xbs_pvoe: This controls whether the xbs_pvov signal will be able
   // to control the I/O pin. Setting this to a one allows xbs_pvov to
   // control the pin value.
   //
   // xbs_pvov: If the corresponding xbs_pvoe bit is set, then this
   // controls the value of the I/O pin. If the xbs_ddoe and xbs_ddov
   // signals have set the pin to be an out put, then, if xbs_pvoe is
   // set, the value of xbs_pvov will be the output value of the pin.
   //
   // Most often, the xbs_ddoe and xbs_ddov signals are controlled by
   // the software library corresponding to the XB being
   // instantiated. If the XB uses output pins, the the XB should
   // provide signals as outputs from the XB that should be connected
   // to the xbs_pvoe and xbs_pvov signals.

   localparam NEO_FILL = NUM_PINS -NUM_NEOPIXELS - 1;
   
   assign xbs_ddoe[0] = {NUM_PINS{1'b0}}; 
   assign xbs_ddov[0] = {NUM_PINS{1'b0}};
   // currently support neopixels on pins 1-13, A0 & A1   
   assign xbs_pvoe[0] = {{NEO_FILL{1'b0}},neopixel_en[NUM_NEOPIXELS:1],1'b0}; 
   assign xbs_pvov[0] = {NUM_PINS{neopixel_out}};

   // End of XB instantiation
   //----------------------------------------------------------------------


   //----------------------------------------------------------------------
   // Additional XB instantiations
   //
   // You can add additional XB instantiations as was done above by
   // repeating the above format for each additional XB. Each must
   // include the instantiation of the XB module and the definition of
   // the xbs_* signals.
   //----------------------------------------------------------------------
   
   
   //----------------------------------------------------------------------
   // 5.) Combine control and busses from multiple XB instantiations
   //
   // Combine the pin control signals from each of the XB
   // instantiations by wire ORing then to form a single set of busses


   //     -- \/ -- Do not edit the below lines -- \/ --
   always_comb begin
      // Initialize to zero
      xb_ddoe = {NUM_PINS{1'b0}};
      xb_ddov = {NUM_PINS{1'b0}};
      xb_pvoe = {NUM_PINS{1'b0}};
      xb_pvov = {NUM_PINS{1'b0}};
      // Wire OR the pin control signals together
      for (int i=0;i<NUM_OXBS;i++) begin
	 xb_ddoe = xb_ddoe | xbs_ddoe[i];
	 xb_ddov = xb_ddov | (xbs_ddoe[i] & xbs_ddov[i]);
	 xb_pvoe = xb_pvoe | xbs_pvoe[i];
	 xb_pvov = xb_pvov | (xbs_pvoe[i] & xbs_pvov[i]);
      end
   end
   //     -- /\ -- Do not edit the above lines -- /\ --

   // Combine the dbusout and io_out_en signals from the instantiated
   // XBs here and then pass them up to the xlr8_top.
   //
   // Here is an example for a single XB:  
   //   assign dbus_out  = xb1_out_en ? xb1_dbusout : 8'h00;
   //   assign io_out_en = xb1_out_en;
   //      
   // Here is an example for three XBs (xb1, xb2 and xb3):
   //   assign dbus_out  = xb1_out_en ? xb1_dbusout :
   //                      xb2_out_en ? xb2_dbusout :
   //                      xb3_out_en ? xb3_dbusout :
   //                                   8'h00;
   //   assign io_out_en = xb1_out_en || 
   //                      xb2_out_en ||
   //                      xb3_out_en;
   //

   assign dbus_out  = neopixel_io_out_en ? neopixel_dbus_out : 8'h00;
   assign io_out_en = neopixel_io_out_en;
   
   // End of combining logic
   //----------------------------------------------------------------------

   
   //----------------------------------------------------------------------
   // 6.) Interrupts
   //
   // If you need to have interrupts for you XBs, then delete the
   // following line that ties off the xb_irq output and instead
   // uncomment the following instantiation of the xlr8_pcint module

   assign xb_irq = 1'b0; // DELETE THIS LINE IF YOU UNCOMMENT THE XLR8_PCINT
   
/* 
   localparam NUM_INTS = 1; // EDIT to be equal to the number of interrupts

   xlr8_pcint
     #(
       .XICR_Address (OX8ICR_Address),
       .XIFR_Address (OX8IFR_Address),
       .XMSK_Address (OX8MSK_Address),
       .WIDTH        (NUM_INTS)
       )
   xb_pcint_inst
     (
      // Clock and Reset
      .rstn         (rstn),
      .clk          (clk),
      // I/O
      .adr          (adr),
      .dbus_in      (dbusin),
      .dbus_out     (dbus_out),
      .iore         (iore),
      .iowe         (iowe),
      .out_en       (io_out_en),
      // DM
      .ramadr       (ramadr),
      .ramre        (ramre),
      .ramwe        (ramwe),
      .dm_sel       (dm_sel),
      // 
      .x_int_in     (), // INSERT YOUR XB INTERRUPTS HERE 
      .x_irq        (xb_irq),
      .x_irq_ack    () // Don't use acks here
      );
*/   
   
endmodule // xlr8_xb_openxlr8

