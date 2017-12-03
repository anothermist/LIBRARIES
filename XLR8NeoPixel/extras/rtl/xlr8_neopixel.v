/////////////////////////////////
// Filename    : xlr8_neopixel.v
// Author      : Matt Weber
// Description : A state machine for driving neo pixel strips based
//     on the WS2811 and WS2812 devices.
//    Pixel data is stored in a memory. The state machine
//     reads that pixel data and sends it to the neopixels
//     according to the timing requirements of the WS2811/2812
//    The software using this unit is responsible for
//     getting the pixel data in the correct order/memory
//     locations. This block just increments through memory
//     sending the data out.
//    Timing to the WS2812 is 
//       Logic 0 : 0.35us high, then 0.8us low (both +/-150ns)
//       Logic 1 : 0.7us high, then 0.6us low (both +/-150ns)
//       Reset between sequences : More than 50us low
//      With 16MHz clock, this is
//       Logic 0 : 4-8cycles high, then 11-15cycles low
//       Logic 1 : 9-13cycles high, then 8-12cycles low
//       Reset between sequences : More than 800cycles low
//      The Adafruit Neopixel library appears to do (not confirmed yet)
//       Logic 0 : 5cycles high, then 15cycles low
//       Logic 1 : 13cycles high, then 7cycles low
//       Total per bit = 20cycles = 800kHz
//      To better match the spec, we'll do
//       Logic 0 : 5cycles high, then 15cycles low
//       Logic 1 : 12cycles high, then 8cycles low
//       Total per bit = 20cycles = 800kHz
//       Let the software handle getting 50us between sequences
//    Timing to the WS2811 (which is less used) is
//       Logic 0 : 0.5us high, then 2.0us low (both +/-150ns)
//       Logic 1 : 1.2us high, then 1.3us low (both +/-150ns)
//       Reset between sequences : More than 50us low
//      With 16MHz clock, this is
//       Logic 0 : 6-10cycles high, then 30-34cycles low
//       Logic 1 : 17-21cycles high, then 19-23cycles low
//       Reset between sequences : More than 800cycles low
//      The Adafruit Neopixel library appears to do (not confirmed yet)
//       Logic 0 : 8cycles high, then 32cycles low
//       Logic 1 : 20cycles high, then 20cycles low
//       Total per bit = 40cycles = 400kHz
//      We'll also do
//       Logic 0 : 8cycles high, then 32cycles low
//       Logic 1 : 20cycles high, then 20cycles low
//       Total per bit = 40cycles = 400kHz
//       Let the software handle getting 50us between sequences
//   The state machine that sends the data out to the NeoPixels
//       (kicked off by the software show() function), is
//       configured with the number of bytes to send, the timing
//       parameters (WS2811 or WS2812), and an index into a command
//       buffer. Each entry in the command buffer defines a
//       section of the pixel memory (a start address and a
//       length), and a brightness level. If a section is
//       smaller than the number of bytes that need to be
//       sent to the NeoPixel, the state machine automatically
//       reads the next entry from the command buffer and
//       continues, using that memory section. If a section
//       is larger than the NeoPixel needs, the transmission
//       simply terminates leaving some of the section unused.
//   The memory is built with four M9K memories. Three of the
//       memories hold only pixel data. The fourth holds 64 command
//       buffer locations and the remainder is pixel data.
//       The memories are dual port 256x36 with one port used for
//       the CSR block's read/write access and the other for
//       the state machine's read-only access. As pixel data,
//       the 36b port is divided into 4 sections of 8b color
//       value plus 1b unused. As command buffer data, there is
//       12b for section memory start address, 12b for section
//       length, 8b for brightness, and 4b reserved for future
//       use. A total of 3840 bytes is available for pixel
//       data (4x(1024 - 64)), enough for 1280 3color pixels
//       or 960 4color pixels. 
//   Note command buffer location 0 is at the ending memory address,
//       location 1 is one below that, etc., while the pixel data
//       starts at memory address zero and goes up. This makes it
//       possible to adjust how much memory is used for each section
//       without as much redesign if that's needed.
//   The CSR block has 4 registers, 1 control/status and 3 data
//       CNTL : 4b PinNum/BusyID - For the show command, this specifies
//                   which pin to send the data to. For no-op 
//                   a pin number or other ID can be used. For other commands
//                   this field is ignored and not stored. For show the
//                   value clears to zero when the command completes and thereby
//                   can be used as a busy indication. While the
//                   hardware doesn't provide any locks, this may help
//                   software manage having multiple objects share this hardware
//                   nicely. The outputs are indexed starting at 1 because
//                   using the clear to zero to indicate busy/notbusy wouldn't
//                   work with pin 0. It is probably okay to write pixel memory
//                   while show is running (although probably just sections of
//                   memory that aren't used by the current show)
//              4b command  0000 = no-op, but set PinNum/BusyID (won't autoclear until after show)
//                          0001 = get memsize. D2=cmd buf size (entries), D1/D0= pixel mem size (bytes)
//                          0010 = show WS2812. D2=starting cmd buffer D1/D0=length (bytes)
//                          0011 = show WS2811. D2=starting cmd buffer D1/D0=length (bytes)
//                          0100-0111 = Reserved.
//                          1000 = set color. D2=color value, D1/D0=memory address (autoincrement)
//                          1001 = set cmd buf entry-addr. D2=cmd buf addr, D1/D0=section start addr
//                          1010 = set cmd buf entry-length. D2=cmd buf addr, D1/D0=section length
//                          1011 = set cmd buf entry-brightness. D2=cmd buf addr, D1=reserved, D0=brightness
//                          1100 = get color. D2=color value, D1/D0=memory address (autoincrement)
//                          1101 = get cmd buf entry-addr. D2=cmd buf addr, D1/D0=section start addr
//                          1110 = get cmd buf entry-length. D2=cmd buf addr, D1/D0=section length
//                          1111 = get cmd buf entry-brightness. D2=cmd buf addr, D1=reserved, D0=brightness
//        D2/D1/D0 : 8b data registers used as described above
//                       For set, and show operations, D2/D1/D0 are loaded before doing the set/show
//                           cmd in the CNTL register (except possibly doing a no-op to set the busy ID)
//                       For get color, D1/D0 are set first, then CNTL, then the color value can be
//                           read from D2
//                       For get cmd buf, D2 is set first, then CNTL, then the cmd buf info can be
//                           read from D1/D0
//                       For get memsize, CNTL is written, then the memsize can be read from D2/D1/D0
//
//   The command buffer can be used in many different ways:
//    1. The easy, default usage would be to have one command buffer
//       location with the start address=0 and length=(num bytes per pixel *
//       number of pixels in string). If driving multiple NeoPixel strings, just
//       have them use different regions of the pixel memory.
//    2. Slide: Set segment length > NeoPixel length. Can slide a pattern back and forth
//       through strip without rewriting memory by just changing start point
//    3. Wrap: If have ring, can rotate pattern by setting up two segments. Just changing
//       first segment's start point and length causes pattern to rotate
//    4. Background: Could set up long sequence that alternates between background and
//       forground patterns. Can move foreground pattern simply by
//       incrementing or decrementing length of previous background segment,
//       and incrementing/decrementing length and start of net background segment.
//    4. Appear/disappear: Could set up long sequence that alternates between background and
//       forground patterns. Can make foreground pattern disappear by setting
//       its length to zero and increasing the previous background segment's length
//       to cover over it. Reapper by simply putting the lengths back. 
//    5. Animate 1: Could have multiple sets of data for the neopixel and just pick which
//       one to send by specifying a different cmd buf location
//    6. Animate 2: Could have multiple same-size memory locations pointed to by
//       consecutive command buffer entries and choose which one to display by setting its
//       length on and the other lengths to zero. Animate without changing pixel memory
//       simply by changing which segment has non-zero length. The data being
//       sent to the output pin is slow enough (1 byte every 160 cycles) that the
//       state machine can read through many consecutive zero length commands before getting
//       "real" data and still be ready soon enough to meet the timing requirements (the
//       max number is not yet determined)
//
//   The data being sent to the NeoPixel can go on any pin, so there's a small block that does
//      that steering.

// Copyright 2015, Alorium Technology Inc. All Rights Reserved
/////////////////////////////////

module xlr8_neopixel
 #(parameter NUM_NEOPIXELS = 15,
   parameter NEOCR_ADDR  = 6'h0, // neopixel control register
   parameter NEOD2_ADDR = 6'h0, // neopixel data register 2
   parameter NEOD1_ADDR = 6'h0, // neopixel data register 1
   parameter NEOD0_ADDR = 6'h0) // neopixel data register 0
  (input logic clk,
  input  logic                  en16mhz, // clock enable at 16MHz rate
  input logic                   rstn,
  // Register access for registers in first 64
  input [5:0]                   adr,
  input [7:0]                   dbus_in,
  output [7:0]                  dbus_out,
  input                         iore,
  input                         iowe,
  output wire                   io_out_en,
  // Register access for registers not in first 64
  input wire [7:0]              ramadr,
  input wire                    ramre,
  input wire                    ramwe,
  input wire                    dm_sel,
  // External inputs/outputs
  output logic [NUM_NEOPIXELS:1] neopixel_en, // valid pin numbers start at 1
  output logic neopixel_out
  );

  /////////////////////////////////
  // Local Parameters
  /////////////////////////////////
  // Registers in I/O address range x0-x3F (memory addresses -x20-0x5F)
  //  use the adr/iore/iowe inputs. Registers in the extended address
  //  range (memory address 0x60 and above) use ramadr/ramre/ramwe
  localparam  NEOCR_DM_LOC   = (NEOCR_ADDR >= 16'h60) ? 1 : 0;
  localparam  NEOD2_DM_LOC   = (NEOD2_ADDR >= 16'h60) ? 1 : 0;
  localparam  NEOD1_DM_LOC   = (NEOD1_ADDR >= 16'h60) ? 1 : 0;
  localparam  NEOD0_DM_LOC   = (NEOD0_ADDR >= 16'h60) ? 1 : 0;

  localparam NEOPIN_LSB   = 4;
  localparam NEOCMD_LSB   = 0;
  localparam CMD_NOOP  = 4'b0000, // no-op, but set PinNum/BusyID (it won't autoclear)                     
             CMD_GTSZ  = 4'b0001, // get memsize. D2=cmd buf size (entries), D1/D0= pixel mem size (bytes)
             CMD_SHOW  = 4'b0010, // show WS2812. D2=starting cmd buffer D1/D0=length (bytes) 
             CMD_SHOW2 = 4'b0011, // show WS2811. D2=starting cmd buffer D1/D0=length (bytes) 
             CMD_SCOL  = 4'b1000, // set color. D2=color value, D1/D0=memory address (autoincrement)           
             CMD_SCBA  = 4'b1001, // set cmd buf entry-addr. D2=cmd buf addr, D1/D0=section start addr         
             CMD_SCBL  = 4'b1010, // set cmd buf entry-length. D2=cmd buf addr, D1/D0=section length           
             CMD_SCBB  = 4'b1011, // set cmd buf entry-brightness. D2=cmd buf addr, D1=reserved, D0=brightness
             CMD_GCOL  = 4'b1100, // get color. D2=color value, D1/D0=memory address (autoincrement)           
             CMD_GCBA  = 4'b1101, // get cmd buf entry-addr. D2=cmd buf addr, D1/D0=section start addr         
             CMD_GCBL  = 4'b1110, // get cmd buf entry-length. D2=cmd buf addr, D1/D0=section length           
             CMD_GCBB  = 4'b1111; // get cmd buf entry-brightness. D2=cmd buf addr, D1=reserved, D0=brightness
  localparam CMDBUF_SIZE = 8'd64;
  localparam COLOR_SIZE  = 4*1024 - (CMDBUF_SIZE*4);
  localparam CMDBUF_END_ADDR = CMDBUF_SIZE - 8'd1;
  localparam COLOR_END_ADDR = COLOR_SIZE - 12'd1;
  
  /////////////////////////////////
  // Signals
  /////////////////////////////////
  logic neocr_sel;
  logic neod2_sel;
  logic neod1_sel;
  logic neod0_sel;
  logic neocr_we ;
  logic neod2_we ;
  logic neod1_we ;
  logic neod0_we ;
  logic neocr_re ;
  logic neod2_re ;
  logic neod1_re ;
  logic neod0_re ;
  logic [7:0] neocr_rdata;
  logic [3:0]   cmd_in;
  logic [3:0]   NEOPIN;
  logic [3:0]   NEOCMD;
  logic [15:0]  NEOCMD_onehot;
  logic [7:0]   NEOD2,NEOD1,NEOD0;
  logic         csr_mem_rd;
  logic         send_pixels_start;
  logic [9:0]   csr_mem_address;
  logic         csr_mem_wren;
  logic [3:0]   csr_mem_byteena;
  logic [35:0]  csr_mem_wtdata;
  logic [35:0]  csr_mem_rddata;
  logic [9:0]   pixel_mem_address;
  logic [35:0]  pixel_mem_rddata;
  logic [7:0]   pixel_color_rddata;
  logic [7:0]   pixel_color_scaled;
  logic [5:0]   pixel_cmdbuf_addr;
  logic         pixel_cmdbuf_rddatavalid;
  logic         pixel_color_rddatavalid; 
  logic         pixel_color_rddatataken; 
  logic [11:0]  segment_addr;
  logic [11:0]  segment_remain;
  logic [7:0]   segment_brightness;
  logic         get_first_byte;
  logic [7:0]   pixel_shift_onehot;
  logic [7:0]   pixel_shift_reg;
  logic [11:0]  show_remain;
  logic         pixel_bit_sent;
  logic [5:0]   bit_timing_count;
  logic         read_next_segment;
  logic         show_idle, show_idle_d1;
  logic         show_complete;
  logic         neocr_we_d1;
  logic         run_show,run_show2;

  /////////////////////////////////
  // Functions and Tasks
  /////////////////////////////////

  /////////////////////////////////
  // Main Code
  /////////////////////////////////

  assign neocr_sel = NEOCR_DM_LOC ?  (dm_sel && ramadr == NEOCR_ADDR ) : (adr[5:0] == NEOCR_ADDR[5:0] ); 
  assign neod2_sel = NEOD2_DM_LOC ?  (dm_sel && ramadr == NEOD2_ADDR ) : (adr[5:0] == NEOD2_ADDR[5:0] );
  assign neod1_sel = NEOD1_DM_LOC ?  (dm_sel && ramadr == NEOD1_ADDR ) : (adr[5:0] == NEOD1_ADDR[5:0] );
  assign neod0_sel = NEOD0_DM_LOC ?  (dm_sel && ramadr == NEOD0_ADDR ) : (adr[5:0] == NEOD0_ADDR[5:0] );
  assign neocr_we = neocr_sel && (NEOCR_DM_LOC ?  ramwe : iowe); 
  assign neod2_we = neod2_sel && (NEOD2_DM_LOC ?  ramwe : iowe);
  assign neod1_we = neod1_sel && (NEOD1_DM_LOC ?  ramwe : iowe); 
  assign neod0_we = neod0_sel && (NEOD0_DM_LOC ?  ramwe : iowe); 
  assign neocr_re = neocr_sel && (NEOCR_DM_LOC ?  ramre : iore); 
  assign neod2_re = neod2_sel && (NEOD2_DM_LOC ?  ramre : iore);
  assign neod1_re = neod1_sel && (NEOD1_DM_LOC ?  ramre : iore); 
  assign neod0_re = neod0_sel && (NEOD0_DM_LOC ?  ramre : iore); 

  assign dbus_out =  ({8{neocr_sel}} & neocr_rdata) |
                     ({8{neod2_sel}} & NEOD2) | 
                     ({8{neod1_sel}} & NEOD1) | 
                     ({8{neod0_sel}} & NEOD0); 
  assign io_out_en = neocr_re || 
                     neod2_re ||
                     neod1_re ||
                     neod0_re; 

  // Control Registers
  assign cmd_in = dbus_in[NEOCMD_LSB +: 4];
  always @(posedge clk or negedge rstn) begin
    if (!rstn)  begin
      {NEOPIN,NEOCMD} <= 8'h0;
      NEOCMD_onehot <= 16'h1;
      run_show <= 1'b0;
      run_show2 <= 1'b0;  
    end else if (neocr_we) begin
      if ((cmd_in == CMD_SHOW) || (cmd_in == CMD_SHOW2) || (cmd_in == CMD_NOOP)) begin
        NEOPIN <= dbus_in[NEOPIN_LSB +: 4];
      end
      NEOCMD <= cmd_in;
      NEOCMD_onehot <= 16'h1 << cmd_in;
      run_show <= run_show || (cmd_in == CMD_SHOW);
      run_show2 <= run_show2 || (cmd_in == CMD_SHOW2);  
    end else if (show_complete) begin
      NEOPIN <= 4'h0;
      run_show <= 1'b0;
      run_show2 <= 1'b0;  
    end
  end // always @ (posedge clk or negedge rstn)
  assign neocr_rdata = ({4'h0,NEOPIN} << NEOPIN_LSB) |
                       ({4'h0,NEOCMD} << NEOCMD_LSB);
  always @(posedge clk or negedge rstn) begin
    if (!rstn)  begin
      NEOD2 <= 8'h0;
      NEOD1 <= 8'h0;
      NEOD0 <= 8'h0;
    end else if (neod2_we) begin
      NEOD2  <= dbus_in;
    end else if (neod1_we) begin
      NEOD1  <= dbus_in;
    end else if (neod0_we) begin
      NEOD0  <= dbus_in;
    end else if (neocr_we && (cmd_in == CMD_GTSZ)) begin
      NEOD2 <=  CMDBUF_SIZE;
      {NEOD1,NEOD0} <=  COLOR_SIZE;
    end else if (neocr_we_d1 && (NEOCMD_onehot[CMD_GCOL])) begin // one cycle for read data to come back
      NEOD2 <= (NEOD0[1:0] == 2'b00) ? csr_mem_rddata[7:0]   :
               (NEOD0[1:0] == 2'b01) ? csr_mem_rddata[16:9]  :
               (NEOD0[1:0] == 2'b10) ? csr_mem_rddata[25:18] :
                                       csr_mem_rddata[34:27];
      {NEOD1,NEOD0} <= ({4'h0,NEOD1[3:0],NEOD0} == COLOR_END_ADDR) ? 16'h0 :
                                           {4'h0,NEOD1[3:0],NEOD0} + 16'h1; // autoincrement mem address
    // reads from pixel color section of mem needs a cycle, but the
    //  cmd buf section is the default path to the mem address so it
    //  has already been sent to the memory and the rddata is already available.
    end else if (neocr_we && (cmd_in == CMD_GCBA)) begin
      {NEOD1,NEOD0} <= {4'h0,csr_mem_rddata[11:0]};
    end else if (neocr_we && (cmd_in == CMD_GCBL)) begin
      {NEOD1,NEOD0} <= {4'h0,csr_mem_rddata[23:12]};
    end else if (neocr_we && (cmd_in == CMD_GCBB)) begin
      {NEOD1,NEOD0} <= {4'h0,csr_mem_rddata[35:24]};
    end else if (neocr_we_d1 && (NEOCMD_onehot[CMD_SCOL])) begin
      {NEOD1,NEOD0} <= ({4'h0,NEOD1[3:0],NEOD0} == COLOR_END_ADDR) ? 16'h0 :
                                           {4'h0,NEOD1[3:0],NEOD0} + 16'h1; // autoincrement mem address
    end 
  end // always @ (posedge clk or negedge rstn)
  assign send_pixels_start = neocr_we && ((cmd_in == CMD_SHOW) || (cmd_in == CMD_SHOW2));

  // writing the cmd buf needs to be read-modify-write. We have the
  //  address and therefore the read data ahead of time, so this doesn't
  //  take too much work
  // do we want to do this on the same cycle as iowe/ramwe or the next cycle?
  //  affects autoincrement timing as well as ability to register read-modify-write path
  always @(posedge clk) neocr_we_d1 <= neocr_we;
  assign csr_mem_address = (NEOCMD_onehot[CMD_GCOL]) ? ({NEOD1,NEOD0} >> 2) :
                           (NEOCMD_onehot[CMD_SCOL]) ? ({NEOD1,NEOD0} >> 2) :
                                           ~{2'h0,NEOD2}; // invert puts cmd buf at end of memory
  assign csr_mem_wren    = neocr_we_d1 && // perhaps could use neocr_we and cmd_in instead
                           ((NEOCMD_onehot[CMD_SCOL]) || (NEOCMD_onehot[CMD_SCBA]) ||
                            (NEOCMD_onehot[CMD_SCBL]) || (NEOCMD_onehot[CMD_SCBB]));
  assign csr_mem_byteena = (NEOCMD_onehot[CMD_SCOL]) ? (4'h1 << NEOD0[1:0]) : 4'hF;
  assign csr_mem_wtdata  = (NEOCMD_onehot[CMD_SCBA]) ? {csr_mem_rddata[35:12],NEOD1[3:0],NEOD0[7:0]} :
                           (NEOCMD_onehot[CMD_SCBL]) ? {csr_mem_rddata[35:24],NEOD1[3:0],NEOD0[7:0],csr_mem_rddata[11:0]} :
                           (NEOCMD_onehot[CMD_SCBB]) ? {NEOD1[3:0],NEOD0[7:0],csr_mem_rddata[23:0]} :
                                                  {4{1'b0,NEOD2}}; //(NEOCMD == CMD_SCOL)
  
  // The memory
  /* ram2p1024x36 AUTO_TEMPLATE (.\(.*\)_a (csr_mem_\1[]),
                                 .\(.*\)_b (pixel_mem_\1[]),
                                 .q_a (csr_mem_rddata[]),
                                 .q_b (pixel_mem_rddata[]),
                                 .data_a (csr_mem_wtdata[]),
                                 .data_b (pixel_mem_wtdata[]),
                                 );
   */
  ram2p1024x36 neomem (.clock(clk),
                       .data_b(36'h0),
                       .wren_b(1'b0),
                       /*AUTOINST*/
                       // Outputs
                       .q_a             (csr_mem_rddata[35:0]),  // Templated
                       .q_b             (pixel_mem_rddata[35:0]), // Templated
                       // Inputs
                       .address_a       (csr_mem_address[9:0]),  // Templated
                       .address_b       (pixel_mem_address[9:0]), // Templated
                       .byteena_a       (csr_mem_byteena[3:0]),  // Templated
                       .data_a          (csr_mem_wtdata[35:0]),  // Templated
                       .wren_a          (csr_mem_wren));                 // Templated
  

  // Read the cmd buf
  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      pixel_cmdbuf_addr <= 6'h0;
      segment_addr <= 12'h0;
      segment_brightness <= 8'h0;
      segment_remain <= 12'h0;
      // End of automatics
    end else begin
      if (send_pixels_start) begin
        pixel_cmdbuf_addr <= NEOD2[5:0];
        segment_remain <= 12'h0;
      end else if (pixel_cmdbuf_rddatavalid) begin
        segment_addr   <= pixel_mem_rddata[11:0];
        segment_remain <= pixel_mem_rddata[23:12]; // start at length and countdown
        segment_brightness <= pixel_mem_rddata[31:24];
        // bits 35:32 currently unused
        pixel_cmdbuf_addr <= (pixel_cmdbuf_addr == CMDBUF_END_ADDR) ? 6'h0 : 
                             (pixel_cmdbuf_addr + 6'h1); // after one read done, get ready for the next
      end else if (pixel_color_rddatataken) begin // when pixel color read, can move to next one
        segment_addr   <= (segment_addr == COLOR_END_ADDR) ? 12'h0 : (segment_addr + 12'h1);
        segment_remain <= |segment_remain ? (segment_remain - 12'h1) : segment_remain; // remaining to get from memory
      end
    end // else: !if(!rstn)
  end // always @ (posedge clk or negedge rstn)

  // Read the pixel color byte data
  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      pixel_shift_onehot <= 8'h1;
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      get_first_byte <= 1'h0;
      pixel_shift_reg <= 8'h0;
      show_remain <= 12'h0;
      // End of automatics
    end else begin
      if (send_pixels_start) begin
        get_first_byte <= 1'b1;
        show_remain <= {NEOD1[3:0],NEOD0[7:0]};
        pixel_shift_reg <= 8'h0;
        pixel_shift_onehot <= 8'h1;
      end else if (pixel_color_rddatataken) begin
        get_first_byte <= 1'b0;
        pixel_shift_reg <= pixel_color_scaled;
        pixel_shift_onehot <= 8'h80; // send msb first
        show_remain <= get_first_byte ? show_remain : 
                       |show_remain ? (show_remain - 12'h1) : 12'h0; // remaining to pass to bit_timing
      end else if (pixel_bit_sent) begin
        pixel_shift_reg <= pixel_shift_reg << 1;
        pixel_shift_onehot <= (pixel_shift_onehot == 8'h1) ? 8'h1 : (pixel_shift_onehot >> 1);
      end
    end // else: !if(!rstn)
  end // always @ (posedge clk or negedge rstn)

  // Send each bit out
  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      bit_timing_count <= 6'h0;
      // End of automatics
    end else begin
      if (send_pixels_start) begin
        // start with sending the zero that should be in pixel_shift_reg
        //  until we get the first byte loaded
        bit_timing_count <= (cmd_in == CMD_SHOW2) ? 6'd38 : 6'd18;
      end else if (!show_idle) begin
        if (en16mhz) begin
          if (bit_timing_count == 6'd0) begin
            bit_timing_count <= run_show2 ? 6'd39 : 6'd19;
          end else begin
            bit_timing_count <= bit_timing_count - 6'd1;
          end
        end
      end else begin // If not running, no need to have the counter flailing about
        bit_timing_count <= 6'd0;
      end // else: !if(|show_remain || |bit_timing_count)
    end // else: !if(!rstn)
  end // always @ (posedge clk or negedge rstn)
  
  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      neopixel_en <= {(1+(NUM_NEOPIXELS)-(1)){1'b0}};
      neopixel_out <= 1'h0;
      // End of automatics
    end else if (en16mhz) begin
      if (!show_idle) begin
        neopixel_en <= {{NUM_NEOPIXELS-1{1'b0}},1'b1} << (NEOPIN-4'h1); // minus 1 because neopixel_en is [:1]
        if (run_show) begin
          case (bit_timing_count)
            6'd19 : neopixel_out <= 1'b1;
            6'd14 : neopixel_out <= pixel_shift_reg[7]; // cycle 5
            6'd7  : neopixel_out <= 1'b0; // cycle 12
          endcase // case (bit_timing_count)
        end else begin
          case (bit_timing_count)
            6'd39 : neopixel_out <= 1'b1;
            6'd31 : neopixel_out <= pixel_shift_reg[7]; // cycle 8
            6'd19 : neopixel_out <= 1'b0; // cycle 20
          endcase // case (bit_timing_count)
        end
      end else begin // if ((show_remain != 12'h0) || (bit_timing_count != 6'h0))
        // shutting off the enable returns control of the pin to the ordinary
        //  DDRx/PORTx registers and the software library would have those
        //  set to output a low value.
        neopixel_en <= {NUM_NEOPIXELS{1'b0}};
        neopixel_out <= 1'b0;
      end // else: !if((show_remain != 12'h0) || (bit_timing_count != 6'h0))
    end // if (en16mhz)
  end // always @ (posedge clk or negedge rstn)

  assign pixel_bit_sent = (run_show  && (bit_timing_count == 6'd14)) ||
                          (run_show2 && (bit_timing_count == 6'd31));
  
  // if (segment_remain == 0 && show_remain != 0) read cmd buf, else read pixel data
  assign read_next_segment = (show_remain != 0) && 
                             (pixel_cmdbuf_rddatavalid ? ~|pixel_mem_rddata[23:12] : ~|segment_remain);
  assign show_idle = (show_remain == 12'h0) && (bit_timing_count == 6'h0);
  assign show_complete = show_idle && !show_idle_d1;
  assign pixel_mem_address = read_next_segment ? ~{2'h0,pixel_cmdbuf_addr} :
                             segment_addr[11:2];
  assign pixel_color_rddata = (segment_addr[1:0] == 2'b00) ? pixel_mem_rddata[7:0] :
                              (segment_addr[1:0] == 2'b01) ? pixel_mem_rddata[16:9] :
                              (segment_addr[1:0] == 2'b10) ? pixel_mem_rddata[25:18] :
                                                             pixel_mem_rddata[34:27];
  // to make the math easier (do div by 256 instead of div by 255), brightness
  //  is interpreted as 1-256, except with the 8bit field, 256 (intentionally) wraps
  //  back to zero, so we have 0=max brightness, 1=min, 255=just less than max
  assign pixel_color_scaled = (segment_brightness == 8'h0) ? pixel_color_rddata :
                                ({8'h0,segment_brightness} * {8'h0,pixel_color_rddata}) >> 8;
  always @(posedge clk or negedge rstn) begin
    if (!rstn) begin
      /*AUTORESET*/
      // Beginning of autoreset for uninitialized flops
      pixel_cmdbuf_rddatavalid <= 1'h0;
      pixel_color_rddatavalid <= 1'h0;
      show_idle_d1 <= 1'h0;
      // End of automatics
    end else begin
      show_idle_d1 <= show_idle;
      pixel_cmdbuf_rddatavalid <= !show_idle && read_next_segment;
      pixel_color_rddatavalid <= !show_idle && !read_next_segment;
    end
  end
  assign pixel_color_rddatataken = pixel_color_rddatavalid && pixel_bit_sent && pixel_shift_onehot[0];
  
   /////////////////////////////////
   // Assertions
   /////////////////////////////////

`ifdef STGI_ASSERT_ON
  // Currently uses of d1/d0 should be less than the
  //  number of memory bytes. When doing CMD_GTSZ it equals
  ERROR_addr_range: assert property
  (@(posedge clk) disable iff (!rstn)
   ({NEOD1,NEOD0} <= COLOR_SIZE));
  ERROR_neocmd_onehot: assert property
  (@(posedge clk) disable iff (!rstn)
   $onehot(NEOCMD_onehot));
  ERROR_pixel_shift_onehot: assert property
  (@(posedge clk) disable iff (!rstn)
   $onehot(pixel_shift_onehot));


`endif

   /////////////////////////////////
   // Cover Points
   /////////////////////////////////

`ifdef STGI_COVER_ON
`endif

endmodule

// Local Variables:
// verilog-library-flags:("-y ../ram2p1024x36")
// End:
