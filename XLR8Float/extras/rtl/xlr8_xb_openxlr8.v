///////////////////////////////////////////////////////////////////
//=================================================================
//  Copyright (c) Alorium Technology 2016
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : xlr8_xb_openxlr8.v
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
// 6.) Create clk_scki
//     This section creates a clk_scki which is used by the SPI 
//     XB if it is instantiated. You shouldn't need to modify 
//     this section.
//
//=================================================================
///////////////////////////////////////////////////////////////////

module xlr8_xb_openxlr8
  //----------------------------------------------------------------------
  // 1.) Parameters

  #(
        parameter DESIGN_CONFIG = 8,
    //    {
    //     25'd0, // [31:7] :  reserved
    //     1'b0,  //   [6]  :  MAX10 Size,  0 = M08,             1 = M16
    //     1'b0,  //   [5]  :  ADC_SWIZZLE, 0 = XLR8,            1 = Sno
    //     1'b0,  //   [4]  :  PLL Speed,   0 = 16MHz PLL,       1 = 50Mhz PLL
    //     1'b1,  //   [3]  :  PMEM Size,   0 = 8K (Sim Kludge), 1 = 16K
    //     2'd0,  //  [2:1] :  Clock Speed, 0 = 16MHZ,           1 = 32MHz, 2 = 64MHz, 3=na
    //     1'b0   //   [0]  :  FPGA Image,  0 = CFM Application, 1 = CFM Factory
    //     },
    
    parameter NUM_PINS = 20,// Default is Arduino Uno Digital 0-13 + Analog 0-5
    // NUM_PINS should be 20 for the XLR8 board, ?? for the Sno board

    parameter NUM_OXBS  = 1) // !! EDIT THIS LINE !!
   // NUM_OXBS should equal the number of XBs being instantiated within
   // this module. However, in the case where no XB is being
   // instantiated, the value should be set to 1 rather than zero, so
   // that the logic compiles correctly and we can still provide the
   // correct output values. Called it NUM_OXBS for OpenXLR8 XBs, to 
   // differentiate is from the NUM_XBS parameter used in the top.
   //----------------------------------------------------------------------
   
   //----------------------------------------------------------------------
   // 2.) Inputs and Outputs
   (
    // Clock and Reset
    input                       clk, // Clock
    input                       rstn, // Reset 
    // I/O 
    input [5:0]                 adr, // Reg Address
    input [7:0]                 dbus_in, // Data Bus Input
    output [7:0]                dbus_out, // Data Bus Output
    output                      io_out_en, // IO Output Enable
    input                       iore, // IO Reade Enable
    input                       iowe, // IO Write Enable
    // DM
    input [7:0]                 ramadr, // RAM Address
    input                       ramre, // RAM Read Enable
    input                       ramwe, // RAM Write Enable
    input                       dm_sel, // DM Select
    // Other
    input [255:0]               gprf, // Direct RO access to Reg File
    input                       scki, // SPI clk
    input [NUM_PINS-1:0]        xb_pinx, // pin inputs
    inout                       JT9, // JTAG pin
    inout                       JT7, // JTAG pin
    inout                       JT6, // JTAG pin
    inout                       JT5, // JTAG pin
    inout                       JT3, // JTAG pin
    inout                       JT1, // JTAG pin
    // For iomux
    output logic [NUM_PINS-1:0] xb_ddoe, // override data direction
    output logic [NUM_PINS-1:0] xb_ddov, // data direction value if 
                                           //   overridden (1=output)
    output logic [NUM_PINS-1:0] xb_pvoe, // override output value
    output logic [NUM_PINS-1:0] xb_pvov    // output value if overridden
    );
   //----------------------------------------------------------------------

   
   //----------------------------------------------------------------------
   // 3.) Regs and Wires declarations
   //
   // These are required:

   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_ddoe;
   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_ddov;
   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_pvoe;
   logic [NUM_OXBS-1:0][NUM_PINS-1:0] xbs_pvov;
   
   // Add additional wires and regs here as needed to connect your XBs
   // to the combining logic and to each other if needed. At minimum,
   // with a single XB, you'll need at least something like this:

   wire [7:0]          float_dbus_out;
   wire                float_out_en;

   wire [31:0]         xf_dataa;
   wire [31:0]         xf_datab;
   wire [7:0]          xf_en;
   wire [31:0]         xf_p0_result;
   wire [31:0]         xf_p1_result;
   wire [31:0]         xf_p2_result;
   wire [31:0]         xf_p3_result;
   wire [31:0]         xf_p4_result;
   wire [31:0]         xf_p5_result;
   wire [31:0]         xf_p6_result;
   wire [31:0]         xf_p7_result;
   
   // logic [7:0]		xb1_dbusout;
   // logic 			xb1_out_en;
   //----------------------------------------------------------------------
   
   
   //----------------------------------------------------------------------
   // 4.) Instantiate XBs
   
   //----------------------------------------------------------------------
   // Instantiate XB module and configure control signals
   //

   xlr8_xf 
     #(// Parameters
       .STGI_XF_PRELOAD_DELTA     (8'd3), // operands are set at least this many cycle before cmd is sent
       .STGI_XF_P0_LATENCY        (8'd0), // compare latency=1, 0=not implemented
       .STGI_XF_P1_LATENCY        (8'd1), // add latency
       .STGI_XF_P2_LATENCY        (8'd2), // mult latency
       .STGI_XF_P3_LATENCY        (8'd32),// div latency
       .STGI_XF_P4_LATENCY        (8'd0), // 0=not implemented
       .STGI_XF_P5_LATENCY        (8'd0), // sqrt latency=6, 0=not implemented
       .STGI_XF_P6_LATENCY        (8'd0), // fix2float latency=1, 0=not implemented
       .STGI_XF_P7_LATENCY        (8'd0), // float2fix latency=1, 0=not implemented
       /*AUTOINSTPARAM*/
       // Parameters
       .STGI_XF_CTRL_ADR          (STGI_XF_CTRL_ADR),
       .STGI_XF_STATUS_ADR        (STGI_XF_STATUS_ADR),
       .STGI_XF_R0_ADR            (STGI_XF_R0_ADR),
       .STGI_XF_R1_ADR            (STGI_XF_R1_ADR),
       .STGI_XF_R2_ADR            (STGI_XF_R2_ADR),
       .STGI_XF_R3_ADR            (STGI_XF_R3_ADR))
   xf_inst
     (
      .ireset       (rstn),
      .cp2          (clk),
      // AVR Control
      .adr          (adr),  
      .dbus_in      (dbus_in),
      .dbus_out     (float_dbus_out),
      .iore         (iore), 
      .iowe         (iowe), 
      .out_en       (float_out_en),
      .core_ramadr  (ramadr[7:0]),
      .core_ramre   (ramre),
      .core_ramwe   (ramwe),
      .core_dm_sel  (dm_sel),
      .gprf         (gprf),
      .xf_dataa     (xf_dataa),
      .xf_datab     (xf_datab),
      .xf_en        (xf_en),
      .xf_p0_result (xf_p0_result),
      .xf_p1_result (xf_p1_result),
      .xf_p2_result (xf_p2_result),
      .xf_p3_result (xf_p3_result),
      .xf_p4_result (xf_p4_result),
      .xf_p5_result (xf_p5_result),
      .xf_p6_result (xf_p6_result),
      .xf_p7_result (xf_p7_result)
      );
   
   //-------------------------------------------------------
   // Instantiate the Altera Floating Point functional units
   //-------------------------------------------------------
   
   // FP ADD/SUB
   xlr8_float_add1
     float_add_inst (
                     .clk (clk),
                     .areset (!rstn),
                     .a (xf_dataa),
                     .b (xf_datab),
                     .en (1'b1),
                     .q (xf_p1_result)
                     );
   
   // FP MULT
   xlr8_float_mult2
     float_mult_inst (
                      .clk ( clk ),
                      .areset (!rstn),
                      .en (1'b1),
                      .a (xf_dataa),
                      .b (xf_datab),
                      .q (xf_p2_result)
                      );
   
   // FP DIV
   xlr8_fdiv
     fdiv_inst (
                .clk ( clk ),
                .rst_n (rstn),
                .clken (1'b1),
                .start (xf_en[3]),
                .numer (xf_dataa),
                .denom (xf_datab),
                .q_out (xf_p3_result)
                );
   
   // Unused xlr8_xf ports - set thier results to zero
   assign xf_p0_result = 8'h0;
   assign xf_p4_result = 8'h0;
   assign xf_p5_result = 8'h0;
   assign xf_p6_result = 8'h0;
   assign xf_p7_result = 8'h0;
   

   // Floating point does not use I/O   
   assign xbs_ddoe[0] = {NUM_PINS{1'b0}}; 
   assign xbs_ddov[0] = {NUM_PINS{1'b0}};
   assign xbs_pvoe[0] = {NUM_PINS{1'b0}};
   assign xbs_pvov[0] = {NUM_PINS{1'b0}};

   // End of XB instantiation
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

   assign dbus_out  = float_out_en ? float_dbus_out : 8'h00;
   assign io_out_en = float_out_en;
   
   
   // End of combining logic
   //----------------------------------------------------------------------

   
   //----------------------------------------------------------------------
   // 6.) Create clk_scki for use by a SPI XB, if present
   //     -- \/ -- Do not edit the below lines -- \/ --
   logic 		     clk_scki;
   logic 		     scki_out;
   logic 		     spi_clken;
   assign spi_clken = 1'b1; // assume always enabled
   lcell clk_scki_buffer (.in(scki), .out(scki_out));
   assign clk_scki = scki_out & spi_clken; 
   //     -- /\ -- Do not edit the above lines -- /\ --
   //----------------------------------------------------------------------
   
endmodule // xlr8_xb_openxlr8

