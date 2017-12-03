///////////////////////////////////////////////////////////////////
//=================================================================
//  Copyright(c) Superion Technology Group Inc., 2015
//  ALL RIGHTS RESERVED
//  $Id:  $
//=================================================================
//
// File name:  : xlr8_fdiv.v
// Author      : Steve Berg
// Description : Floating point divide unit.
//
//=================================================================
///////////////////////////////////////////////////////////////////
module xlr8_fdiv
  #(parameter DENOM_W = 32,
    parameter NUMER_W = 32,
    parameter EXP_W   = 8,
    parameter FRAC_W =  23,
    parameter QUOTI_W = 32,
    parameter REMAI_W = DENOM_W)

  (input logic clk,
   input logic                rst_n,
   input logic                clken,

   input logic [DENOM_W-1:0]  denom,
   input logic [NUMER_W-1:0]  numer,
   input logic                start, // start a new divide

   output logic [QUOTI_W-1:0] q_out
 //  output logic [REMAI_W-1:0] remain, // FIXME: unused
//   output logic             quotient_val                  // result valid: one shot
   );


   localparam MANT_W = FRAC_W+1;
   localparam QUO_W = MANT_W+3;
   localparam CNT_W = $clog2(MANT_W+1);
  
  // need to do full width subtracts
//  localparam SUB_W = DIVISOR_W > NUMER_W ? DIVISOR_W : NUMER_W;
   localparam SUB_W = 28; //MANT_W+1;

  logic                       sign;
  logic [EXP_W-1:0]           exp_q;
  logic [EXP_W-1:0]           temp_exp;
  logic [EXP_W-1:0]           exp_numer;
  logic [EXP_W-1:0]           exp_denom;
  logic [FRAC_W-1:0]          frac_numer;
  logic [FRAC_W-1:0]          frac_denom;
  logic [MANT_W-1:0]          mant_numer;
  logic [MANT_W-1:0]          mant_denom;
  logic [MANT_W-1:0]          mant_q;
  
  logic                     exp_numer_0;
  logic                     exp_denom_0;
  logic                     exp_numer_255;
  logic                     exp_denom_255;
  logic                     frac_numer_0;
  logic                     frac_denom_0;
  logic                     numer_nan;
  logic                     denom_nan;
  logic                     numer_inf;
  logic                     denom_inf;
  logic                     numer_0;
  logic                     denom_0;
  
  logic [CNT_W-1:0]         cnt;
  logic [CNT_W-1:0]         cnt_nxt;
  logic [MANT_W:0]          q_rnd;
//  logic [MANT_W-1:0]      quotient_nxt;
   logic [QUO_W:0]          quotient;
   logic [QUO_W:0]          quotient_nxt;
   logic                    bsy;
  logic [MANT_W-1:0]        divisor, divisor_nxt;
  logic [MANT_W:0]          dividend, dividend_nxt, dividend_mux;
  logic                     quotient_val;       // result valid: one shot

  logic [QUO_W:0] q_adjst;
  logic [EXP_W-1:0] exp_adjst;
                            
  assign exp_numer = numer[23 +: EXP_W];
  assign exp_denom = denom[23 +: EXP_W];
  assign frac_numer = numer[FRAC_W-1:0];
  assign frac_denom = denom[FRAC_W-1:0];
  assign mant_numer = {1'b1,numer[FRAC_W-1:0]};
  assign mant_denom = {1'b1,denom[FRAC_W-1:0]};

  //calculate and hold sign and exp for result
  always_ff @(posedge clk or negedge rst_n)
    if (!rst_n) begin
      sign <= 1'b0;
      temp_exp <= 8'h0;
    end
    else begin
      sign <= numer[31]^denom[31]; //sign - calculate and hold
      temp_exp <= exp_numer - exp_denom + 'd127; //result exponent
    end

  always_comb begin
    exp_numer_0 = exp_numer == 0;
    exp_denom_0 = exp_denom == 0;
    exp_numer_255 = exp_numer == 8'hff;
    exp_denom_255 = exp_denom == 8'hff;
    
    frac_numer_0 = frac_numer == 0;
    frac_denom_0 = frac_denom == 0;

    numer_nan = (exp_numer_255) && !frac_numer_0;
    denom_nan = (exp_denom_255) && !frac_denom_0;
    numer_inf = (exp_numer_255) && frac_numer_0;
    denom_inf = (exp_denom_255) && frac_denom_0;
    numer_0 = exp_numer_0;
    denom_0 = exp_denom_0;
     
  end

  // figure out special cases up front
  // assuming no denormals! If exp of operand is zero, then input is zero
  always_comb begin
    // nan: divide by 0, either input is NAN, both inputs==0, both inputs==inf
    if (numer_nan || denom_nan || (numer_0 && denom_0) || (numer_inf && denom_inf)) begin
      q_out = 32'h7fffffff;
    end
    
    // inf
    else if (numer_inf || denom_0) begin
      q_out = {sign,8'hff,23'h0};
    end
    // zero
         else if (numer_0 || denom_inf) begin
           q_out = {sign,31'h0};
         end

              else begin
//              q_out = {sign,exp_q,mant_q[FRAC_W-1:0]};
                q_out = {sign,exp_adjst,q_rnd[FRAC_W-1:0]};
              end
  end // always_comb
  
   logic [23:0] rslt;
   logic        brw;      // borrow bit


  always_comb begin: calc_nxt
    {brw,rslt} = dividend - divisor;
     dividend_mux = brw ? dividend : rslt;
     dividend_nxt = dividend_mux << 1;
     quotient_nxt = (brw) ? {quotient[26:0],1'b0} : {quotient[26:0],1'b1};
     divisor_nxt = divisor;
    cnt_nxt = cnt - 'd1;
    bsy = |cnt;
  end
   
  always_ff @(posedge clk or negedge rst_n)
    if (!rst_n) begin
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      cnt <= {CNT_W{1'b0}};
      dividend <= {25{1'b0}};
      divisor <= {MANT_W{1'b0}};
      quotient <= {QUO_W{1'b0}};
      // End of automatics
    end
    else begin
      if (clken) begin
         cnt <= start && !bsy ? SUB_W :
                   bsy ? cnt_nxt : cnt;
         
         divisor <= start && !bsy ? mant_denom :
                       bsy ? divisor_nxt :
                       divisor;

        dividend <= start && !bsy ? {1'b0, mant_numer} :
                        bsy ? dividend_nxt :
                        dividend;

        quotient <= start && !bsy ? '0 :
                        bsy ? quotient_nxt :
                        quotient;
      end

    end // else: !if(!rst_n)
  
  logic stcky, g, r;
  logic [25:0] q_inc_g;

//adjust exponent & mantissa  
  always_comb begin
    if (quotient >= 28'h8000000) begin
      exp_adjst = temp_exp ;
//      mant_q = q_rnd >> 1;
      q_adjst = quotient >>1;
    end
     
    else begin
      q_adjst = quotient;
      exp_adjst = temp_exp-1;
    end // else: !if(quotient >= 25'h1000000)
  end // always_comb begin

//round
     
//  always_comb begin
//    stcky = quotient[0] || |dividend;
//    g = quotient[2];
//    r = quotient[1];
//    q_inc_g = quotient[27:2] +1;
//    
//    q_rnd[0] = g&&!r&&!stcky ? 1'b0 : q_inc_g[1];
//    q_rnd[24:1] = q_inc_g[25:2];
//    
//  end

  always_comb begin
    stcky = q_adjst[0] || |dividend;
    g = q_adjst[2];
    r = q_adjst[1];
    q_inc_g = q_adjst[27:2] +1;
    
    q_rnd[0] = g&&!r&&!stcky ? 1'b0 : q_inc_g[1];
    q_rnd[24:1] = q_inc_g[25:2];
    
  end
   
  //mantissa/exp adjust needed for case when round
  //increments an all 1 case?
//  logic quo_gteq;
//  assign  quo_gteq = q_rnd >= 25'h1000000;
   
//  always_comb begin
//    if (q_rnd >= 25'h1000000) begin
//      exp_q = temp_exp;
//      exp_q = exp_adjst+1;
//      mant_q = q_rnd >> 1;
//    end
//     
//    else begin
//      mant_q = q_rnd;
//      exp_q = exp_adjst;
//    end
//  end
   
//   always_comb begin
//       quotient_val = bsy && (~|cnt_nxt);
//  end
  always_ff @(posedge clk) begin
    quotient_val <= bsy && (~|cnt_nxt);
  end

 //`ifndef VERILATOR
//  ERROR_quoti_w_ne_numer_w: assert property
//  (@(posedge clk) disable iff (!rst_n)
//   start |-> (NUMER_W == QUOTI_W))
//    else $error("quotient width  != numer width is unsupported");

//  ERROR_numer_w_lt_denom_w: assert property
//  (@(posedge clk) disable iff (!rst_n)
//   start |-> (NUMER_W >= DENOM_W))
//    else $error("numerator width  has to be at least as big as denominator width");

//`endif //  `ifndef VERILATOR
  

endmodule // xlr8_fdiv
