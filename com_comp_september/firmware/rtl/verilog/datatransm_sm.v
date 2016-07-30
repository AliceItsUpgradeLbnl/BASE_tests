//=============================================================================
// Title: Data transmission state machine
// Author: Alberto Collu
// Institution: Lawrence Berkeley National Laboratory (LBNL)
// Date of creation: 3/18/2016
// Last modified: 3/18/2016 (AC)
// Description: writes acquired data to the control interface buffer
//=============================================================================

module datatransm_sm (

  // Clock interface
  input  CLK,                         // System clock (50MHz)
  input  RST_N,                       // Active low reset
  
  // Send/busy interface (comm with transaction sm)
  input  SEND_DATA,
  input  SEND_HEADER,
  input  SEND_TRAILER,
  output reg BUSY,                    // Indicates transaction in progress

  // Command word from PC
  input  [31:0] CMD,                      

  // Data from SPI/I2C interfaces (either on ADC-DAC board and DUT)
  input  [95:0]  DATA_ADC_BLOCK_0,    // 8 ADC channels
  input  [95:0]  DATA_ADC_BLOCK_1,    // 8 ADC channels
  input  [95:0]  DATA_ADC_BLOCK_2,    // 8 ADC channels
  input  [95:0]  DATA_ADC_BLOCK_3,    // 8 ADC channels
  input  [255:0] DATA_DIFFIO_CHECKER,
  input  [23:0]  DATA_DP,
  input  [23:0]  DATA_I2C_DUT,
  input  [23:0]  DATA_I2C_DUT_AUX,
  input  [23:0]  DATA_SPI_DUT,

  // FIFO interface (comm with control interface)
  input      FIFO_FULL,
  output reg [31:0] FIFO_DATA,
  output reg FIFO_WREN

);

//=======================================================================
//                            Internal Signals
//=======================================================================

reg  [4:0] adcwrd_rdptr;
reg  [31:0] data_from_adc;
reg  [31:0] data_from_diffio_checker;
reg  [31:0] data; 
reg  [4:0] wrd_counter;
reg  [2:0] sm_state;
reg  incr_wrd_counter;
reg  rst_wrd_counter;
reg  [31:0] header;
reg  [15:0] time_stamp;
reg  [25:0] unit_time;
reg  header_state, trailer_state;
reg  [4:0] words_to_send;

wire [383:0] data_adc_allblocks;
wire [2:0] dev_type;
wire last_dataword;
wire [3:0] op_type;
wire [31:0] trailer;
wire multi_sample;

//=======================================================================
//                           Combinatorial Logic
//=======================================================================

// Fields of command from PC
assign dev_type = CMD[10:8];
assign op_type = CMD[7:4];

// Flags from CMD decoding
assign dev_is_adc = (dev_type == 3'd0);
assign dev_is_dp  = (dev_type == 3'd3);
assign dev_is_i2c_dut = (dev_type == 3'd4);
assign dev_is_i2c_dut_aux = (dev_type == 3'd6);
assign dev_is_diffio_checker = (dev_type == 3'd7 && op_type == 4'd0);

// Creating ADC payload from the ADC data blocks (tied high bits 28 and 24 to comply with data format rules)
assign data_adc_allblocks = {
  DATA_ADC_BLOCK_3[87:85],1'b1,DATA_ADC_BLOCK_3[75:73],1'b1,DATA_ADC_BLOCK_3[95:88],DATA_ADC_BLOCK_3[83:76],DATA_ADC_BLOCK_3[71:64],DATA_ADC_BLOCK_3[63:61],1'b1,
  DATA_ADC_BLOCK_3[51:49],1'b1,DATA_ADC_BLOCK_3[59:52],DATA_ADC_BLOCK_3[47:40],DATA_ADC_BLOCK_3[35:28],DATA_ADC_BLOCK_3[39:37],1'b1,DATA_ADC_BLOCK_3[27:25],1'b1,DATA_ADC_BLOCK_3[23:0],
  DATA_ADC_BLOCK_2[87:85],1'b1,DATA_ADC_BLOCK_2[75:73],1'b1,DATA_ADC_BLOCK_2[95:88],DATA_ADC_BLOCK_2[83:76],DATA_ADC_BLOCK_2[71:64],DATA_ADC_BLOCK_2[63:61],1'b1,
  DATA_ADC_BLOCK_2[51:49],1'b1,DATA_ADC_BLOCK_2[59:52],DATA_ADC_BLOCK_2[47:40],DATA_ADC_BLOCK_2[35:28],DATA_ADC_BLOCK_2[39:37],1'b1,DATA_ADC_BLOCK_2[27:25],1'b1,DATA_ADC_BLOCK_2[23:0],
  DATA_ADC_BLOCK_1[87:85],1'b1,DATA_ADC_BLOCK_1[75:73],1'b1,DATA_ADC_BLOCK_1[95:88],DATA_ADC_BLOCK_1[83:76],DATA_ADC_BLOCK_1[71:64],DATA_ADC_BLOCK_1[63:61],1'b1,
  DATA_ADC_BLOCK_1[51:49],1'b1,DATA_ADC_BLOCK_1[59:52],DATA_ADC_BLOCK_1[47:40],DATA_ADC_BLOCK_1[35:28],DATA_ADC_BLOCK_1[39:37],1'b1,DATA_ADC_BLOCK_1[27:25],1'b1,DATA_ADC_BLOCK_1[23:0],
  DATA_ADC_BLOCK_0[87:85],1'b1,DATA_ADC_BLOCK_0[75:73],1'b1,DATA_ADC_BLOCK_0[95:88],DATA_ADC_BLOCK_0[83:76],DATA_ADC_BLOCK_0[71:64],DATA_ADC_BLOCK_0[63:61],1'b1,
  DATA_ADC_BLOCK_0[51:49],1'b1,DATA_ADC_BLOCK_0[59:52],DATA_ADC_BLOCK_0[47:40],DATA_ADC_BLOCK_0[35:28],DATA_ADC_BLOCK_0[39:37],1'b1,DATA_ADC_BLOCK_0[27:25],1'b1,DATA_ADC_BLOCK_0[23:0]
};

// Remapping adc word pointer depending on which adc 8-channel blocks are enabled
always @(*) begin
 case (op_type)
   4'd0: begin
     adcwrd_rdptr  = wrd_counter;
   end
   4'd1: begin
     adcwrd_rdptr  = wrd_counter;
   end
   4'd2: begin
     adcwrd_rdptr  = wrd_counter + 4'd3;
   end
   4'd3: begin
     adcwrd_rdptr  = wrd_counter;
   end
   4'd4: begin
     adcwrd_rdptr  = wrd_counter + 4'd6;
   end
   4'd5: begin
     adcwrd_rdptr  = ({4{(wrd_counter <= 4'd2)}}  & wrd_counter) | ({4{(wrd_counter > 4'd2)}} & (wrd_counter + 4'd3));
   end 
   4'd6: begin
     adcwrd_rdptr  = wrd_counter + 4'd3;
   end
   4'd7: begin
     adcwrd_rdptr  = wrd_counter;
   end
   4'd8: begin
     adcwrd_rdptr  = wrd_counter + 4'd9;
   end
   4'd9: begin
     adcwrd_rdptr  = ({4{(wrd_counter <= 4'd2)}} & wrd_counter) | ({4{(wrd_counter > 4'd2)}} & (wrd_counter + 4'd6));
   end
   4'd10: begin
     adcwrd_rdptr  = ({4{(wrd_counter <= 4'd2)}} & (wrd_counter + 4'd3)) | ({4{(wrd_counter > 4'd2)}} & (wrd_counter + 4'd6));
   end
   4'd11: begin
     adcwrd_rdptr  = ({4{(wrd_counter <= 4'd5)}} & wrd_counter) | ({4{(wrd_counter > 4'd5)}} & (wrd_counter + 4'd3));
   end
   4'd12: begin
     adcwrd_rdptr  = wrd_counter + 4'd6;
   end
   4'd13: begin
     adcwrd_rdptr  = ({4{(wrd_counter <= 4'd2)}} & wrd_counter) | ({4{(wrd_counter > 4'd2)}} & (wrd_counter + 4'd3));
   end
   4'd14: begin
     adcwrd_rdptr  = wrd_counter + 4'd3;
   end
   4'd15: begin
     adcwrd_rdptr  = wrd_counter;
   end
  endcase
end

// Word selector for the ADC payload
always @(*) begin
 case (adcwrd_rdptr)
   5'd0: begin
     data_from_adc = data_adc_allblocks[31:0];
   end
   5'd1: begin
     data_from_adc = data_adc_allblocks[63:32];
   end
   5'd2: begin
     data_from_adc = data_adc_allblocks[95:64];
   end
   5'd3: begin
     data_from_adc = data_adc_allblocks[127:96];
   end
   5'd4: begin
     data_from_adc = data_adc_allblocks[159:128];
   end
   5'd5: begin
     data_from_adc = data_adc_allblocks[191:160];
   end
   5'd6: begin
     data_from_adc = data_adc_allblocks[223:192];
   end
   5'd7: begin
     data_from_adc = data_adc_allblocks[255:224];
   end
   5'd8: begin
     data_from_adc = data_adc_allblocks[287:256];
   end
   5'd9: begin
     data_from_adc = data_adc_allblocks[319:288];
   end
   5'd10: begin
     data_from_adc = data_adc_allblocks[351:320];
   end
   5'd11: begin
     data_from_adc = data_adc_allblocks[383:352];
   end
   default: begin
     data_from_adc = 32'hFFFFFFFF;
   end
  endcase
end

// Word selector for the differential IO checker
always @(*) begin
 case (wrd_counter)
   5'd0: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[31:0];
   end
   5'd1: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[63:32];
   end
   5'd2: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[95:64];
   end
   5'd3: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[127:96];
   end
   5'd4: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[159:128];
   end
   5'd5: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[191:160];
   end
   5'd6: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[223:192];
   end
   5'd7: begin
     data_from_diffio_checker = DATA_DIFFIO_CHECKER[255:224];
	end
   default: begin
     data_from_diffio_checker = 32'hFFFFFFFF;
   end
  endcase
end

// Word type selector
always @(*) begin: fifo_data_sel
  if (header_state)
    FIFO_DATA = header;
  else if (trailer_state)
    FIFO_DATA = trailer;
  else
    FIFO_DATA = data;
end

// Device selector
always @(*) begin: dev_sel
  if (dev_is_adc)
    data = data_from_adc;
  else if (dev_is_dp)
    data = DATA_DP;
  else if (dev_is_i2c_dut)
    data = DATA_I2C_DUT;
  else if (dev_is_i2c_dut_aux)
    data = DATA_I2C_DUT_AUX;
  else if (dev_is_diffio_checker)
    data = data_from_diffio_checker;
  else
    data = DATA_SPI_DUT;
end

// Header type selector
always @(*) begin
  if (dev_is_adc) begin
    if (multi_sample)
      header = {16'hADC1,time_stamp};
    else
      header = {16'hADC0,time_stamp};
  end
  else if (dev_is_dp)
    header   = {16'h0DEE,time_stamp};
  else if (dev_is_i2c_dut)
    header   = {16'hABCD,time_stamp};
  else if (dev_is_i2c_dut_aux)
    header   = {16'hABCF,time_stamp};
  else if (dev_is_diffio_checker)
    header   = {16'h010D,time_stamp};
  else
    header   = {16'h0123,time_stamp};
end

assign trailer = {32'hFEEDBEEF};

// Number of payload words per data transmission
always @(*) begin
  if (dev_is_adc)
    words_to_send = (op_type[0] + op_type[1] + op_type[2] + op_type[3])*2'd3;
  else if (dev_is_diffio_checker)
    words_to_send = 5'd8;    
  else
    words_to_send = 5'd1;
end

assign last_dataword = (wrd_counter == (words_to_send - 1'b1));
assign multi_sample = (CMD[23:16] != 8'd0);

//=======================================================================
//                            Local parameters
//=======================================================================

localparam IDLE = 3'b000;
localparam SENDHEADER = 3'b001;
localparam SENDDATA = 3'b010;
localparam SENDTRAILER = 3'b011;

//=======================================================================
//                           State machine core
//=======================================================================

always @(posedge CLK or negedge RST_N) begin
  if(!RST_N)
    sm_state       <= IDLE;
  else begin
    case (sm_state)
      IDLE: begin
        if (SEND_HEADER)
          sm_state <= SENDHEADER;
        else if (SEND_DATA && (words_to_send != 0)) 
          sm_state <= SENDDATA;
        else if (SEND_TRAILER)
          sm_state <= SENDTRAILER;
        else
          sm_state <= IDLE;
      end
      SENDHEADER: begin
        if (FIFO_FULL)
          sm_state <= SENDHEADER;
        else
          sm_state <= IDLE;
      end
      SENDDATA: begin
        if (!FIFO_FULL && last_dataword)
          sm_state <= IDLE;
        else
          sm_state <= SENDDATA;        
      end
      SENDTRAILER: begin
        if (FIFO_FULL)
          sm_state <= SENDTRAILER;
        else 
          sm_state <= IDLE;        
      end
      default:
        sm_state   <= IDLE;
    endcase
  end
end

//=======================================================================
//                         State machine outputs
//=======================================================================

always @(*) begin
  case (sm_state)
    IDLE: begin
      BUSY                = 1'b0;
      FIFO_WREN           = 1'b0;
      header_state        = 1'b0;
      incr_wrd_counter    = 1'b0;
      rst_wrd_counter     = 1'b0;
      trailer_state       = 1'b0;
    end
    SENDHEADER: begin
      BUSY                = 1'b1;
      header_state        = 1'b1;
      incr_wrd_counter    = 1'b0;
      rst_wrd_counter     = 1'b0;
      trailer_state       = 1'b0;
      if (!FIFO_FULL)
        FIFO_WREN         = 1'b1;
      else
        FIFO_WREN         = 1'b0;
    end
    SENDDATA: begin
      BUSY                = 1'b1;
      header_state        = 1'b0;
      trailer_state       = 1'b0;
      if (!FIFO_FULL && !last_dataword) begin
        incr_wrd_counter  = 1'b1;
        rst_wrd_counter   = 1'b0;
        FIFO_WREN         = 1'b1;
      end else if (!FIFO_FULL) begin
        incr_wrd_counter  = 1'b0;
        rst_wrd_counter   = 1'b1;
        FIFO_WREN         = 1'b1;
      end else begin
        incr_wrd_counter  = 1'b0;
        rst_wrd_counter   = 1'b0;
        FIFO_WREN         = 1'b0;
      end     
    end
    SENDTRAILER: begin
      BUSY                = 1'b1;
      header_state        = 1'b0;
      incr_wrd_counter    = 1'b0;
      rst_wrd_counter     = 1'b0;
      trailer_state       = 1'b1;
      if (!FIFO_FULL)
        FIFO_WREN         = 1'b1;
      else
        FIFO_WREN         = 1'b0;
    end
    default: begin
      BUSY                = 1'b0;
      FIFO_WREN           = 1'b0;
      header_state        = 1'b0;
      incr_wrd_counter    = 1'b0;
      rst_wrd_counter     = 1'b0;
      trailer_state       = 1'b0;
    end
  endcase
end


//=======================================================================
//                           Sequential logic
//=======================================================================

// Word counter
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    wrd_counter <= 5'd0;
  else if (rst_wrd_counter)
    wrd_counter <= 5'd0;
  else if (incr_wrd_counter)
    wrd_counter <= wrd_counter + 1'b1;
end

// Time counter
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    time_stamp <= 16'd0;
  else if (unit_time == 26'd49999999)
    time_stamp <= time_stamp + 1'b1;
end

// Unit time counter
always @(posedge CLK or negedge RST_N) begin
  if (!RST_N)
    unit_time <= 26'd0;
  else if (unit_time == 26'd49999999) // count one second
    unit_time <= 26'd0;
  else
    unit_time <= unit_time + 1'b1;
end


endmodule
