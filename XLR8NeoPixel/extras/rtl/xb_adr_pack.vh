//======================================================================
// Filename    : xb_adr_pack.vh
// Author      : Steve Phillips
// Description : AVR address constants (localparams) for registers 
//               used by Xcelerator Blocks (XBs) 
//
// When allocating register addresses for your XBs, you need to know
// what addresses are available for use by XBs. The reg addresses used
// by XBs should be unused by other functions. The register address
// range is 0x00 thru 0xFF. There are no available addresses in the
// lower half of the range (0x00 - 0x7F), but there are many scattered
// thru the upper half of the address range.
//
// The list below details the allocation of addresses in the upper
// half of the reg address range (0x80 - 0xFF). The addresses listed
// as --UNUSED-- are safe to use for your OpenXLR8 modules. It is
// recommended to use addresses in the big block of --UNUSED-- reg
// address at 0xDE - 0xF3. If that is not enough then other --UNUSED--
// addresses could be used.
//
// Addresses marked as ALLOCATED are actively used by the base AVR
// design. DO NOT USE ALLOCATED Addresses. Use of ALLOCATED addresses
// will cause XLR8 errors.
//
// Addresses marked as --RESERVED (328PB)-- are used in the 328PB
// variant of the AVR architecture. Using these addresses should not
// cause XLR8 errors but should be avoided .
//
// Copyright 2015, Superion Technology Group. All Rights Reserved
//----------------------------------------------------------------------

//======================================================================
// Enter your allocations here, for example:
//

// These are the default values from the avr_adr_pack.vh. Change at your peril.
localparam NEOCR_ADDR      = 8'hF4; // XLR8 NeoPixel XB Control Reg
localparam NEOD0_ADDR      = 8'hF5; // XLR8 NeoPixel XB Data 0 Reg
localparam NEOD1_ADDR      = 8'hF6; // XLR8 NeoPixel XB Data 1 Reg
localparam NEOD2_ADDR      = 8'hF7; // XLR8 NeoPixel XB Data 2 Reg

//
//----------------------------------------------------------------------

//======================================================================
// Reference list for upper half of register address range
//
// Select --UNUSED-- addresses from below to use in your allocations 
// above
//----------------------------------------------------------------------
// localparam ALLOCATED       = 8'h80; // Used by AVR Core
// localparam ALLOCATED       = 8'h81; // Used by AVR Core
// localparam ALLOCATED       = 8'h82; // Used by AVR Core
// localparam UNUSED          = 8'h83; // --UNUSED--
// localparam ALLOCATED       = 8'h84; // Used by AVR Core
// localparam ALLOCATED       = 8'h85; // Used by AVR Core
// localparam ALLOCATED       = 8'h86; // Used by AVR Core
// localparam ALLOCATED       = 8'h87; // Used by AVR Core
// localparam ALLOCATED       = 8'h88; // Used by AVR Core
// localparam ALLOCATED       = 8'h89; // Used by AVR Core
// localparam ALLOCATED       = 8'h8A; // Used by AVR Core
// localparam ALLOCATED       = 8'h8B; // Used by AVR Core
// localparam UNUSED          = 8'h8C; // --UNUSED--
// localparam UNUSED          = 8'h8D; // --UNUSED--
// localparam UNUSED          = 8'h8E; // --UNUSED--
// localparam UNUSED          = 8'h8F; // --UNUSED--
// localparam RESERVED        = 8'h90; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h91; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h92; // --RESERVED (328PB)--
// localparam UNUSED          = 8'h93; // --UNUSED--
// localparam RESERVED        = 8'h94; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h95; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h96; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h97; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h98; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h99; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h9A; // --RESERVED (328PB)--
// localparam RESERVED        = 8'h9B; // --RESERVED (328PB)--
// localparam UNUSED          = 8'h9C; // --UNUSED--
// localparam UNUSED          = 8'h9D; // --UNUSED--
// localparam UNUSED          = 8'h9E; // --UNUSED--
// localparam UNUSED          = 8'h9F; // --UNUSED--
// localparam RESERVED        = 8'hA0; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA1; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA2; // --RESERVED (328PB)--
// localparam UNUSED          = 8'hA3; // --UNUSED--
// localparam RESERVED        = 8'hA4; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA5; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA6; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA7; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA8; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hA9; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hAA; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hAB; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hAC; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hAD; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hAE; // --RESERVED (328PB)--
// localparam UNUSED          = 8'hAF; // --UNUSED--
// localparam ALLOCATED       = 8'hB0; // Used by AVR Core
// localparam ALLOCATED       = 8'hB1; // Used by AVR Core
// localparam ALLOCATED       = 8'hB2; // Used by AVR Core
// localparam ALLOCATED       = 8'hB3; // Used by AVR Core
// localparam ALLOCATED       = 8'hB4; // Used by AVR Core
// localparam UNUSED          = 8'hB5; // --UNUSED--
// localparam ALLOCATED       = 8'hB6; // Used by AVR Core
// localparam UNUSED          = 8'hB7; // --UNUSED--
// localparam ALLOCATED       = 8'hB8; // Used by AVR Core
// localparam ALLOCATED       = 8'hB9; // Used by AVR Core
// localparam ALLOCATED       = 8'hBA; // Used by AVR Core
// localparam ALLOCATED       = 8'hBB; // Used by AVR Core
// localparam ALLOCATED       = 8'hBC; // Used by AVR Core
// localparam ALLOCATED       = 8'hBD; // Used by AVR Core
// localparam UNUSED          = 8'hBF; // --UNUSED--
// localparam ALLOCATED       = 8'hC0; // Used by AVR Core
// localparam ALLOCATED       = 8'hC1; // Used by AVR Core
// localparam ALLOCATED       = 8'hC2; // Used by AVR Core
// localparam RESERVED        = 8'hC3; // --RESERVED (328PB)--
// localparam ALLOCATED       = 8'hC4; // Used by AVR Core
// localparam ALLOCATED       = 8'hC5; // Used by AVR Core
// localparam ALLOCATED       = 8'hC6; // Used by AVR Core
// localparam UNUSED          = 8'hC7; // --UNUSED--
// localparam RESERVED        = 8'hC8; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hC9; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hCA; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hCB; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hCC; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hCD; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hCE; // --RESERVED (328PB)--
// localparam ALLOCATED       = 8'hCF; // Used by AVR Core
// localparam ALLOCATED       = 8'hD0; // Used by AVR Core
// localparam ALLOCATED       = 8'hD1; // Used by AVR Core
// localparam ALLOCATED       = 8'hD2; // Used by AVR Core
// localparam UNUSED          = 8'hD3; // --UNUSED--
// localparam ALLOCATED       = 8'hD4; // Used by AVR Core
// localparam ALLOCATED       = 8'hD5; // Used by AVR Core
// localparam ALLOCATED       = 8'hD6; // Used by AVR Core
// localparam UNUSED          = 8'hD7; // --UNUSED--
// localparam RESERVED        = 8'hD8; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hD9; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hDA; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hDB; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hDC; // --RESERVED (328PB)--
// localparam RESERVED        = 8'hDD; // --RESERVED (328PB)--
// localparam UNUSED          = 8'hDE; // --UNUSED--
// localparam UNUSED          = 8'hDF; // --UNUSED--
// localparam UNUSED          = 8'hE0; // --UNUSED--
// localparam UNUSED          = 8'hE1; // --UNUSED--
// localparam UNUSED          = 8'hE2; // --UNUSED--
// localparam UNUSED          = 8'hE3; // --UNUSED--
// localparam UNUSED          = 8'hE4; // --UNUSED--
// localparam UNUSED          = 8'hE5; // --UNUSED--
// localparam UNUSED          = 8'hE6; // --UNUSED--
// localparam UNUSED          = 8'hE7; // --UNUSED--
// localparam UNUSED          = 8'hE8; // --UNUSED--
// localparam UNUSED          = 8'hE9; // --UNUSED--
// localparam UNUSED          = 8'hEA; // --UNUSED--
// localparam UNUSED          = 8'hEB; // --UNUSED--
// localparam UNUSED          = 8'hEC; // --UNUSED--
// localparam UNUSED          = 8'hED; // --UNUSED--
// localparam UNUSED          = 8'hEE; // --UNUSED--
// localparam UNUSED          = 8'hEF; // --UNUSED--
// localparam UNUSED          = 8'hF0; // --UNUSED--
// localparam UNUSED          = 8'hF1; // --UNUSED--
// localparam UNUSED          = 8'hF2; // --UNUSED--
// localparam UNUSED          = 8'hF3; // --UNUSED--
// localparam ALLOCATED       = 8'hF4; // Used by AVR Core
// localparam ALLOCATED       = 8'hF5; // Used by AVR Core
// localparam ALLOCATED       = 8'hF6; // Used by AVR Core
// localparam ALLOCATED       = 8'hF7; // Used by AVR Core
// localparam UNUSED          = 8'hF8; // --UNUSED--
// localparam UNUSED          = 8'hF9; // --UNUSED--
// localparam ALLOCATED       = 8'hFA; // Used by AVR Core
// localparam UNUSED          = 8'hFB; // --UNUSED--
// localparam ALLOCATED       = 8'hFC; // Used by AVR Core
// localparam ALLOCATED       = 8'hFD; // Used by AVR Core
// localparam UNUSED          = 8'hFE; // --UNUSED--
// localparam UNUSED          = 8'hFF; // --UNUSED--
//----------------------------------------------------------------------
