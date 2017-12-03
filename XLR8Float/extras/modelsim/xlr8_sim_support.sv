///////////////////////////////////////////////////////////////////
//=================================================================
//  Copyright(c) Superion Technology Group Inc., 2015
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : xlr8_sim_support.sv
// Author      : Stephen Fraleigh
// Description : Works with Arduino software library XLR8SimSupport to
//    print messages and get control info.
//    This is intended to be bound to an instance of xlr8_gpio.
//
//    This implements the following:
//       1. SW writes messages one character at a time to GPIOR2.
//          $display them into the transcript/log.
//       2. SW writes an end of transaction identifier to GPIOR1.
//          bit 7 means end of sim. Other bits carry error count.
//
//    It would also be possible to pass data the
//    other direction, which could be useful for simulation-only
//    testing but not obvious what to do when running the SW on
//    on the board. We'd have to revise the protocol to do this.
//  
//=================================================================
///////////////////////////////////////////////////////////////////

`ifndef XLR8_SIM_SUPPORT_SV
 `define XLR8_SIM_SUPPORT_SV
`timescale 1ns/1ps

module xlr8_sim_support

  (// clks/resets - - - - - -
   input       clk,
   input       rstn,
   input       clken, // used to power off the function
  // Register access for registers in first 64
   input [5:0] adr,
   input [7:0] dbus_in,
   input       iore,
   input       iowe,
   // Register access for registers not in first 64
   input [7:0] ramadr,
   input       ramre,
   input       ramwe,
   input       dm_sel,

   //
   // Internals of xlr8_gpio
   //
   input [7:0] GPIOR0,
   input       gpior1_we,
   input       gpior2_we   
   );



//   initial begin
//      // tell xlr8 register that we are in simulation mode.
//      // Software can read this reg during setup to detect if
//      // it is running in a simulation (1) or on the hardware (0).
//      force GPIOR0[0] = 1'b1;
//   end

   always @(posedge clk) begin
      if (clken && gpior2_we) begin
         $write("%s", dbus_in);
      end
   end
   
   always @(posedge clk) begin
      if (clken && gpior1_we) begin : end_test
         bit       end_sim;
         bit [6:0] err_code;
         { end_sim, err_code } = dbus_in;

         if( end_sim ) begin
            if( err_code ) begin
               $display("test_done. Test ***failed*** with error code %d", err_code);
            end
            else begin
               $display("test_done. Test passed with code %d", err_code );
            end
            #10;
            $finish;
         end
         else begin
            $display("GPIOR1 written with value %d", dbus_in);
         end
      end
   end
   
   
endmodule // xlr8_sim_support

`endif
