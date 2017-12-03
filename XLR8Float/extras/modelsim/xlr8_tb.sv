///////////////////////////////////////////////////////////////////
//=================================================================
//  Copyright(c) Superion Technology Group Inc., 2016
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : xlr8_tb.sv
// Author      : Stephen Fraleigh
//  
//=================================================================
///////////////////////////////////////////////////////////////////

// Top-level testbench for XLR8 board.
`timescale 1ns/1ps

module xlr8_tb;

   //-------------------------------------------------------
   // Local Parameters
   //-------------------------------------------------------
   `include "avr_adr_pack.vh"

   //-------------------------------------------------------
   // Reg/Wire Declarations
   //-------------------------------------------------------
   wire [13:0] Digital;
   wire [5:0]  Ana_Dig;
   wire        A_Result; // from analog compare
   wire        RXD;
   wire        TXD;
   wire        SDA;
   wire        SCL;
   reg         RESET_N;                // To xlr8_inst0 of top.v
   reg         Clock;


   //-------------------------------------------------------
   // Generate clock
   //-------------------------------------------------------
   initial begin
     Clock = 1'b0;
     forever begin
       #31.25; // 16MHz
       Clock = !Clock;
     end
   end

   //-------------------------------------------------------
   // Drive reset
   //-------------------------------------------------------
   initial begin
      RESET_N = 1'b0;
      repeat(5) @(posedge Clock);
      RESET_N = 1'b1;
   end

   //-------------------------------------------------------
   // flash initialization
   //-------------------------------------------------------
`ifdef STRINGIFY
   `undef STRINGIFY
`endif
`define STRINGIFY(str) `"str`"

   //-------------------------------------------------------
   // Instantiate DUT
   //-------------------------------------------------------

   // Initial release only supports simulation of one configuration: 16MHz core, factory image.
   localparam INST0_DESIGN_CONFIG = 9;

   xlr8_board    #(// Parameters
                     .DESIGN_CONFIG      (INST0_DESIGN_CONFIG) )
   xlr8_inst0 
     (.Digital        (Digital),
      .Ana_Dig        (Ana_Dig),
      // Inouts
      .SDA            (SDA),
      .SCL            (SCL),
      .RESET_N        (RESET_N),
      .Clock          (Clock)
      );

   //-------------------------------------------------------
   // Bind the simulation support module to the GPIO registers
   //-------------------------------------------------------
   bind xlr8_gpio xlr8_sim_support xlr8_sim_support_inst(.*);
   // tell xlr8 register that we are in simulation mode.
   // Software can read this reg to detect if
   // it is running in a simulation (1) or on the hardware (0).
   initial force xlr8_inst0.xlr8_top_inst.gpio_inst.GPIOR0[0] = 1'b1;
   
endmodule // xlr8_tb
